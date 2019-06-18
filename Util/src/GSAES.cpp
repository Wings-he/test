#include "GSAES.h"
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <time.h>
#include "AES.h"

static const UInt32 ONCE_ENCODE_SIZE = 1024 * 1024;//文件加密时，每次加密的最大数据长度，单位字节。
static const UInt32 BLOCK_SIZE = 16;//aes 加密解密需要明文是16的倍数，如果不是则需要填充，如果明文是16的倍数，也额外填充16位数据,不然不知道填没填充
static const UInt32 DEFAULT_USERKEY_LENGTH = 16;

#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

//编码输出长度一定是BLOCK_SIZE的倍数，并且是不多于输入长度BLOCK_SIZE的大小
#define CALCULATE_ENCODE_OUTPUT_SIZE(nEncodeInputSize) ALIGN(nEncodeInputSize + 1, BLOCK_SIZE);


int GSAES_CreateKey( char**pUserKey, unsigned int nUserKeyLen)
{
	if ((nUserKeyLen != 128)
		&& (nUserKeyLen != 192)
		&& (nUserKeyLen != 256)
		&& (nUserKeyLen != 0))
	{
		return AES_RET_FAIL;
	}

	int nRealUserKeyLen = nUserKeyLen==0?DEFAULT_USERKEY_LENGTH:(nUserKeyLen/8);

	char *pUserKeyTemp = (char*)malloc(nRealUserKeyLen + 1);
	if (!pUserKeyTemp)
	{
		return AES_RET_OUT_OF_MEMORY;
	}

	srand((unsigned int)(time(NULL)));
	for (int i = 0; i < nRealUserKeyLen; ++i)
	{
		pUserKeyTemp[i] = (char)( 1 + (unsigned int)rand() % MAX_UINT8 );
	}
	pUserKeyTemp[nRealUserKeyLen] = '\0';
	*pUserKey = pUserKeyTemp;
	return AES_RET_SUCCESS;
}


static void EncryptWithPadding( AES &aes, char* pOutputData, const char* pData,  int nLen)
{
	assert(nLen < BLOCK_SIZE);

	byte pBuffer[BLOCK_SIZE];
	memcpy(pBuffer, pData , nLen);

	//填充方式采用PKCS #7， 填充字符串由一个字节序列组成，每个字节都填充该填充字节序列的长度。
	int nPaddingLen = BLOCK_SIZE - nLen;
	memset(pBuffer + nLen, nPaddingLen, nPaddingLen);

	aes.encrypt(pBuffer, (byte*)pOutputData);
}


GSUTIL_API int GSAES_Encrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize )
{
	if (nDataSize <= 0)
	{
		return AES_RET_DATA_SIZE_INVALID;
	}

	int nNeedLen = CALCULATE_ENCODE_OUTPUT_SIZE(nDataSize);
	assert(((nNeedLen / BLOCK_SIZE) - 1) ==  (nDataSize / BLOCK_SIZE));
	if (*nOutputSize < nNeedLen)
	{
		*nOutputSize = nNeedLen;
		return AES_RET_BUFFER_NOT_ENOUGH;
	}

	*nOutputSize = nNeedLen;
	AES aes;
	aes.set_key((byte*)pUserKey, nKeyLen);

	int i = 0 ;
	for (; i < nDataSize; i += BLOCK_SIZE)
	{
		aes.encrypt((byte*)pData + i, (byte*)pOutputData + i);
	}
	
	//上面for不一定加密完所有数据的，因为nDataSize不一定是BLOCK_SIZE的倍数，余下的要通过填充后再加密，
	//如果刚好加密完，为了统一也额外加密BLOCK_SIZE个字节的填充数据。
	int nEncryptedLen = nNeedLen - BLOCK_SIZE;
	EncryptWithPadding(aes, pOutputData + nEncryptedLen, pData + nEncryptedLen, nDataSize - nEncryptedLen);
	return AES_RET_SUCCESS;
}

bool CheckDecryptData( const char * pBack ) 
{
	UInt8 nPaddingSize = (UInt8)pBack[0];
	if ((0 >= nPaddingSize) || (nPaddingSize > BLOCK_SIZE))
	{
		return false;
	}

	for (UInt8 i = 1; i < nPaddingSize; ++i)
	{
		if(UInt8(pBack[-i]) != nPaddingSize)
		{//填充的字符应该等于填充的字节数
			return false;
		}
	}

	return true;
}

GSUTIL_API int GSAES_Decrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize )
{
	if((ALIGN(nDataSize, BLOCK_SIZE) != nDataSize) || (0 == nDataSize))
	{
		return AES_RET_DATA_SIZE_INVALID;
	}

	if (*nOutputSize < nDataSize)
	{
		*nOutputSize = nDataSize;
		return AES_RET_BUFFER_NOT_ENOUGH;
	}

	AES aes;
	aes.set_key((byte*)pUserKey, nKeyLen);

	for (int i = 0; i < nDataSize; i += BLOCK_SIZE)
	{
		aes.decrypt((byte*)pData + i , (byte*)pOutputData + i);
	}

	//解析出来的数据包含加密时填充的字节序列，填充方式采用PKCS #7， 填充字符串由一个字节序列组成，每个字节为该填充字节序列的长度。
	if(!CheckDecryptData(pOutputData + nDataSize - 1))
	{
		return AES_RET_DATA_ERROR;
	}

	*nOutputSize = nDataSize - pOutputData[nDataSize - 1];
	return AES_RET_SUCCESS;
}

static INLINE void _FilenameFormatOS(std::string &strPath )
{
#ifdef _MSWINDOWS_
	std::replace(strPath.begin(), strPath.end(), '/', '\\' );
#else
	std::replace(strPath.begin(), strPath.end(), '\\', '/');
#endif
}

static Int64 _GSGetFileSize(char const* szFileName)
{
	if( !szFileName )
	{
		return -1;
	}

	std::string strTemp(szFileName);
	_FilenameFormatOS(strTemp);

#ifdef _MSWINDOWS_
	struct _stat64  f_stat;
	if ( 0 == _stat64(strTemp.c_str(), &f_stat)) 
		return f_stat.st_size;
#else
	struct stat  f_stat;
	if ( 0 == stat(strTemp.c_str(), &f_stat)) 
		return f_stat.st_size;
#endif

	return  -1;
}

typedef int (*AES_HandleDataFunc)( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize );

int GSAES_HandleFile(const char* pUserKey, int nKeyLen, char const* pSrcFile, const char* pDstFile, UInt32 nMaxOnceHandleSize, AES_HandleDataFunc pHandleDataFunc)
{
	Int64 nFileSize = _GSGetFileSize(pSrcFile);
	if (nFileSize <= 0)
	{
		return AES_RET_OPENFILE_FAIL;
	}

	FILE *pSrcF = fopen(pSrcFile, "rb");
	if (pSrcF == NULL)
	{
		return AES_RET_OPENFILE_FAIL;
	}

	FILE *pDstF = fopen(pDstFile, "wb");
	if (pDstF == NULL)
	{
		fclose(pSrcF);
		return AES_RET_OPENFILE_FAIL;
	}

	char *pDataBuf = (char*) malloc(nMaxOnceHandleSize);
	if (pDataBuf == NULL)
	{
		fclose(pSrcF);
		fclose(pDstF);
		return AES_RET_OUT_OF_MEMORY;
	}
	
	int nBufferSize = CALCULATE_ENCODE_OUTPUT_SIZE(nMaxOnceHandleSize);
	char *pBuffer = new char[nBufferSize];
	if (pBuffer == NULL)
	{
		fclose(pSrcF);
		fclose(pDstF);
		free(pDataBuf);
		return AES_RET_OUT_OF_MEMORY;
	}

	int nRet = AES_RET_SUCCESS;
	for(UInt32 nReadLen = 0; nReadLen = (UInt32)fread(pDataBuf, sizeof(char), nMaxOnceHandleSize, pSrcF);)
	{
		int nOutDataLen = nBufferSize;
		nRet = pHandleDataFunc(pUserKey, nKeyLen, pDataBuf, nReadLen, pBuffer, &nOutDataLen);
		if (nRet == AES_RET_BUFFER_NOT_ENOUGH)
		{
			delete[] pBuffer;
			pBuffer = new char[nBufferSize = nOutDataLen];
			if (pBuffer != NULL)
			{
				nRet = pHandleDataFunc(pUserKey, nKeyLen, pDataBuf, nReadLen, pBuffer, &nOutDataLen);
			}
		}

		if (nRet != AES_RET_SUCCESS)
		{
			break;
		}

		bool bWriteSuccess = nOutDataLen == fwrite(pBuffer, sizeof(char), nOutDataLen, pDstF);
		if(!bWriteSuccess )
		{
			nRet = AES_RET_WRITEFILE_FAIL;
			break;
		}
	}

	delete[] pBuffer;
	free(pDataBuf);
	fclose(pSrcF);
	fclose(pDstF);
	return nRet;
}


int GSAES_EncryptFile( const char* pUserKey, int nKeyLen, const char * pSrcFile, const char* pDstFile )
{
	return GSAES_HandleFile(pUserKey, nKeyLen, pSrcFile, pDstFile, ONCE_ENCODE_SIZE, GSAES_Encrypt);
}

int GSAES_DecryptFile(const char* pUserKey, int nKeyLen, const char* pSrcFile, const char* pDstFile)
{
	int nEnocdeOutputSize = CALCULATE_ENCODE_OUTPUT_SIZE(ONCE_ENCODE_SIZE);
	GSAssert(((nEnocdeOutputSize / BLOCK_SIZE) - 1) ==  (ONCE_ENCODE_SIZE / BLOCK_SIZE));

	return GSAES_HandleFile(pUserKey, nKeyLen, pSrcFile, pDstFile, nEnocdeOutputSize, GSAES_Decrypt);
}

void GSAES_FreeBuffer( void* pBuffer )
{
	free(pBuffer);
}

int GSAES_EncodeData( const char* pUserKey, const char* pData, int nDataSize, char** pOutputData, int& nOutputSize )
{
	if (nDataSize <= 0)
	{
		return AES_RET_DATA_SIZE_INVALID;
	}

	nOutputSize = CALCULATE_ENCODE_OUTPUT_SIZE(nDataSize);
	GSAssert(((nOutputSize / BLOCK_SIZE) - 1) ==  (nDataSize / BLOCK_SIZE));

	*pOutputData = (char*)malloc(nOutputSize);
	if (!*pOutputData)
	{
		return AES_RET_OUT_OF_MEMORY;
	}

	return GSAES_Encrypt(pUserKey, strlen(pUserKey), pData, nDataSize, *pOutputData, &nOutputSize);
}

int GSAES_DecodeData(const char* pUserKey, const char* pData, int nDataSize, char** pOutputData, int& nOutputSize)
{
	if((ALIGN(nDataSize, BLOCK_SIZE) != nDataSize) || (0 == nDataSize))
	{//GSAES_EncodeData加密后的数据长度一定是16的倍数
		return AES_RET_DATA_SIZE_INVALID;
	}

	*pOutputData = (char*)malloc(nOutputSize = nDataSize);
	if (!*pOutputData)
	{
		return AES_RET_OUT_OF_MEMORY;
	}

	return GSAES_Decrypt(pUserKey, strlen(pUserKey), pData, nDataSize, *pOutputData, &nOutputSize);
}


int GSAES_EncodeFile(const char* pUserKey, char const* pSrcFile, const char* pDstFile )
{
	return GSAES_EncryptFile(pUserKey, strlen(pUserKey), pSrcFile, pDstFile);
}

int GSAES_DecodeFile(const char* pUserKey, const char* pSrcFile, const char* pDstFile )
{
	return GSAES_DecryptFile(pUserKey, strlen(pUserKey), pSrcFile, pDstFile);
}