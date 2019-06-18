#include "StreamEncrypt.h"

static size_t FillEncryptHead( char * pOutBuffer, EnumStreamEncryptType eType, UInt16 nLen )
{
	strncpy(pOutBuffer, GS_STREAM_ENCRYPT_MAGIC, strlen(GS_STREAM_ENCRYPT_MAGIC));
	((StruStreamEncryptHead*)pOutBuffer)->nLength =  nLen;
	((StruStreamEncryptHead*)pOutBuffer)->nType = eType;
	return sizeof(StruStreamEncryptHead);
}

int GSStreamEncrypt( EnumStreamEncryptType eType, void* pEncryptInfo, const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize )
{
	assert(NULL != pEncryptInfo);
	StruStreamEncryptInfo_AES *pstStreamEncrypt = (StruStreamEncryptInfo_AES*)pEncryptInfo;
	assert(pstStreamEncrypt->nEncryptLen <= (UInt32)nDataSize);


	int nExtraLen = sizeof(StruStreamEncryptHead) + sizeof(StruStreamEncryptInfo_AES);
	char *pEncryptBuffer = pOutputData + nExtraLen;
	int nRemainDatalen = MAX(nDataSize - pstStreamEncrypt->nEncryptLen, 0);
	int nEncryptOutSize = *nOutputSize - nExtraLen - nRemainDatalen;

	int nRet = GSAES_Encrypt(pUserKey, nKeyLen, pData, pstStreamEncrypt->nEncryptLen, pEncryptBuffer, &nEncryptOutSize);
	if(AES_RET_SUCCESS != nRet)
	{
		if(AES_RET_BUFFER_NOT_ENOUGH == nRet)
		{
			*nOutputSize = nExtraLen + nRemainDatalen + nEncryptOutSize;
		}
		return nRet;
	}
	*nOutputSize = nExtraLen + nRemainDatalen + nEncryptOutSize;//返回实际长度

	FillEncryptHead(pOutputData, eType, sizeof(StruStreamEncryptInfo_AES));

	StruStreamEncryptInfo_AES stMyEncryptInfo = {nEncryptOutSize};
	memcpy(pOutputData + sizeof(StruStreamEncryptHead), &stMyEncryptInfo, sizeof(StruStreamEncryptInfo_AES));

	memcpy(pOutputData + nExtraLen + nEncryptOutSize, pData + pstStreamEncrypt->nEncryptLen, nRemainDatalen);

	return AES_RET_SUCCESS;
}

static StruStreamEncryptHead * ExtractEncryptHead( const char* pData, int nDataSize )
{
	if(nDataSize < sizeof(StruStreamEncryptHead))
	{
		return NULL;
	}

	if (0 != strncmp(pData, GS_STREAM_ENCRYPT_MAGIC, strlen(GS_STREAM_ENCRYPT_MAGIC)))
	{
		return NULL;
	}

	StruStreamEncryptHead *pHead = (StruStreamEncryptHead*) pData ;
	if (nDataSize - sizeof(StruStreamEncryptHead) < pHead->nLength)
	{
		return NULL;
	}

	return pHead;
}

BOOL IsEncryptedStream( const char* pData, int nDataSize )
{
	return ExtractEncryptHead(pData, nDataSize) != NULL;
}

int GSStreamDecrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize )
{
	StruStreamEncryptHead *pHead = ExtractEncryptHead(pData, nDataSize);
	if(pHead == NULL)
	{
		return AES_RET_DATA_ERROR;
	}

	int nExtraLen = sizeof(StruStreamEncryptHead) + sizeof(StruStreamEncryptInfo_AES);

	StruStreamEncryptInfo_AES* pEncryptInfo = (StruStreamEncryptInfo_AES*)(pData + sizeof(StruStreamEncryptHead));
	if (nDataSize - nExtraLen < (int)pEncryptInfo->nEncryptLen)
	{
		return AES_RET_DATA_ERROR;
	}

	int nRemainDatalen = MAX(nDataSize - nExtraLen - pEncryptInfo->nEncryptLen, 0);
	int nDecryptOutSize = *nOutputSize - nRemainDatalen;

	int nRet = GSAES_Decrypt(pUserKey, nKeyLen, pData + nExtraLen, pEncryptInfo->nEncryptLen, pOutputData, &nDecryptOutSize);

	if(AES_RET_SUCCESS != nRet)
	{
		if (nRet == AES_RET_BUFFER_NOT_ENOUGH)
		{
			*nOutputSize = nDecryptOutSize + nRemainDatalen;
		}
		return nRet;
	}

	*nOutputSize = nDecryptOutSize + nRemainDatalen;//返回实际长度
	memcpy(pOutputData + nDecryptOutSize, pData + nExtraLen + pEncryptInfo->nEncryptLen, nRemainDatalen);
	return AES_RET_SUCCESS;
}