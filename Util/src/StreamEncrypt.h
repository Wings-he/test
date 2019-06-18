#pragma once

#include "GSAES.h"

#ifdef _WIN32
#pragma pack( push,1 )
#define GS_ENCRYPT_ATTRIBUTE_PACKED 
#else
#define GS_ENCRYPT_ATTRIBUTE_PACKED    __attribute__ ((packed)) 
#endif

static const char *GS_STREAM_ENCRYPT_MAGIC = "GCRYPT";

/*
加密码流的形式：
魔术字（GCRYPT）| 加密信息长度（加密方式标识+加密信息） | 加密方式标识 | 加密信息 | 加密数据
*/

typedef enum EnumStreamEncryptType
{
	AES,  //对应StruStreamEncryptInfo_AES
}EnumStreamEncryptType;

typedef struct StruStreamEncryptHead
{
	UInt32 nMaigc1;//GCRY
	UInt16 nMaigc2;//PT
	UInt16 nLength;//余下加密信息数据的长度
	UInt16 nType;//EnumStreamEncryptType
}GS_ENCRYPT_ATTRIBUTE_PACKED StruStreamEncryptHead;

typedef struct StruStreamEncryptInfo_AES
{
	UInt32 nEncryptLen;    //用于加密时，表示需要加密的数据长度
	                       //加密后，表示加密部分的数据长度
}GS_ENCRYPT_ATTRIBUTE_PACKED StruStreamEncryptInfo_AES;

#ifdef _WIN32
#pragma pack( pop )
#endif


/****************************************************************
* 加密码流
* 参数说明：eType	             (输入参数)  加密方式
            pEncryptInfo         (输入参数)  加密方式对应的所需加密信息
			pUserKey             (输入参数)  加密密钥
			nKeyLen              (输入参数)  加密密钥长度
			pData                (输入参数)  待加密数据
			nDataSize            (输入参数)  待加密数据长度
			pOutputData          (输出参数)  用于存放加密后数据的内存，不可为NULL
			nOutputSize        (输入\出参数) 输入时表示pOutputData的大小，不可为NULL。输出时，表示加密后数据的长度。
			                                 若pOutputData太小，会返回失败，同时也会通过该指针返回所需内存大小。
* 返回值说明：成功返回AES_RET_SUCCESS，失败返回其它AES_RET_*的错误码
******************************************************************/
GSUTIL_API int GSStreamEncrypt(EnumStreamEncryptType eType, void* pEncryptInfo, const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int *nOutputSize);

/****************************************************************
* 解密码流
* 参数说明：pUserKey             (输入参数)  密钥
			nKeyLen              (输入参数)  密钥长度
			pData                (输入参数)  待解密数据
			nDataSize            (输入参数)  待解密数据长度
			pOutputData          (输出参数)  用于存放解密后数据的内存，不可为NULL
			nOutputSize        (输入\出参数) 输入时表示pOutputData的大小，不可为NULL。输出时，表示解密后数据的长度。
			                                 若pOutputData太小，会返回失败，同时也会通过该指针返回所需内存大小。
* 返回值说明：成功返回AES_RET_SUCCESS，失败返回AES_RET_*的错误码
******************************************************************/
GSUTIL_API int GSStreamDecrypt(const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize);

/****************************************************************
* 判断是否加密码流,
* 参数说明：pData                (输入参数)  数据
			nDataSize            (输入参数)  数据长度
* 返回值说明：是加密码流并且加密信息完整返回1，否则返回0
******************************************************************/
GSUTIL_API BOOL IsEncryptedStream( const char* pData, int nDataSize );