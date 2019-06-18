/*
******************************************
Copyright (C): GOSUN
Filename : GSAES.H
Author :  sw
Description: AES算法加解密数据及文件内容.目前只支持ECB模式与pkcs7padding填充.
********************************************
*/
#ifndef _GS_H_GSAES_H_
#define _GS_H_GSAES_H_

#include "GSUtilDefs.h"
 
#define AES_RET_SUCCESS                     0   //成功
#define AES_RET_FAIL                        -1  //失败
#define AES_RET_OPENFILE_FAIL               -2  //打开文件失败
#define AES_RET_WRITEFILE_FAIL              -3  //写文件失败
#define AES_RET_OUT_OF_MEMORY               -4  //内存分配错误
#define AES_RET_DATA_SIZE_INVALID           -5  //数据长度非法
#define AES_RET_DATA_ERROR                  -6  //数据错误
#define AES_RET_BUFFER_NOT_ENOUGH           -7  //缓冲区大小不足

#ifdef  __cplusplus   
extern "C" {
#endif 


/*  函数说明: 生成AES密钥
 *  参数说明: [OUT]  pUserKey:  密钥
              [IN]  nUserKeyLen: 密钥长度，以bit为单位。 仅支持128位、196位、256位，如果传值为0，则默认使用128位

 *  返回值:   成功返回0，否则为错误码
 *            注意: 需要调用GSAES_FreeBuffer释放pUserKey内存
 */
GSUTIL_API int GSAES_CreateKey(char**pUserKey,  unsigned int nUserKeyLen);


/*  函数说明: 数据加密
 *  参数说明: [IN]	      pUserKey:       密钥
			  [IN]        nKeyLen:        密钥长度，以bit为单位
              [IN]        pData:          数据
			  [IN]        nDataSize:      数据长度
			  [IN\OUT]    pOutputData：   用于存放加密后数据的内存，不可为NULL
			  [IN\OUT]    nOutputSize：   输入时，表示pOutputData的大小，不可为NULL。输出时，表示加密后数据的长度。
			                              若pOutputData太小，会返回失败，同时也会通过该指针返回所需内存大小。
 *  返回值:   成功返回0，否则为错误码。pOutputData太小会返回AES_RET_BUFFER_NOT_ENOUGH，需要重新申请足够大的内存再加密
 */
GSUTIL_API int GSAES_Encrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize );


/*  函数说明: 数据解密
 *  参数说明: [IN]	      pUserKey:       密钥
			  [IN]        nKeyLen:        密钥长度，以bit为单位
              [IN]        pData:          数据
			  [IN]        nDataSize:      数据长度
			  [IN\OUT]    pOutputData：   用于存放解密后数据的内存，不可为NULL
			  [IN\OUT]    nOutputSize：   输入时，表示pOutputData的大小，不可为NULL。输出时，表示解密后数据的长度。
			                              若pOutputData太小，会返回失败，同时也会通过该指针返回所需内存大小。
 *  返回值:   成功返回0 ,否则为错误码。pOutputData太小会返回AES_RET_BUFFER_NOT_ENOUGH，需要重新申请足够大的内存再解密
 */
GSUTIL_API int GSAES_Decrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize );


/*  函数说明: 文件数据加密	
 *  参数说明: [IN]  pUserKey:  密钥
              [IN]  nKeyLen:   密钥长度，以bit为单位
              [IN]  pSrcFile:  源文件
			  [IN]  pDstFile:  目标文件 (加密)

 *  返回值:   成功返回0，否则为错误码
 */
GSUTIL_API int GSAES_EncryptFile(const char* pUserKey, int nKeyLen, const char * pSrcFile, const char* pDstFile);


/*  函数说明: 文件数据解密
 *  参数说明: [IN]  pUserKey: 密钥
              [IN]  nKeyLen:  密钥长度，以bit为单位
              [IN]  pSrcFile: 源文件
			  [IN]  pDstFile: 目标文件 (解密)

 *  返回值:   成功返回0，否则为错误码
 */
GSUTIL_API int GSAES_DecryptFile(const char* pUserKey, int nKeyLen, const char* pSrcFile, const char* pDstFile);


/*  函数说明: 释放本算法申请的内存
 *  参数说明: [IN]  pBuffer: 内存指针

 *  返回值:   无
 */
GSUTIL_API void GSAES_FreeBuffer(void *pBuffer); 


/////////////////////////////以下接口已不建议使用/////////////////////////////////////////

/*  函数说明: 数据加密（建议使用GSAES_Encrypt）
 *  参数说明: [IN]  pUserKey:  密钥
              [IN]  pData: 数据
			  [IN]  nDataSize: 数据长度
			  [OUT] pOutputData: 加密后数据
			  [OUT] nOutputSize: 加密后数据长度

 *  返回值:   成功返回0，否则为错误码
 *            注意: 需要调用GSAES_FreeBuffer释放pOutputData内存
 */
GSUTIL_API int GSAES_EncodeData(const char* pUserKey,const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);

/*  函数说明: 数据解密（建议使用GSAES_Decrypt）
 *  参数说明: [IN]  pUserKey: 密钥
              [IN]  pData: 数据
			  [IN]  nDataSize: 数据长度
			  [OUT] pOutputData: 解密后数据
			  [OUT] nOutputSize: 解密后数据长度

 *  返回值:   成功返回0，否则为错误码
 *            注意: 需要调用GSAES_FreeBuffer释放pOutputData内存
 */
GSUTIL_API int GSAES_DecodeData(const char* pUserKey,const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);

/*  函数说明: 文件数据加密（建议使用GSAES_EncryptFile）
 *  参数说明: [IN]  pUserKey:  密钥
              [IN]  pSrcFile: 源文件
			  [IN]  pDstFile: 目标文件 (加密)

 *  返回值:   成功返回0，否则为错误码
 */
GSUTIL_API int GSAES_EncodeFile(const char* pUserKey, const char * pSrcFile, const char* pDstFile);



/*  函数说明: 文件数据解密（建议使用GSAES_DecryptFile）
 *  参数说明: [IN]  pUserKey: 密钥
              [IN]  pSrcFile: 源文件
			  [IN]  pDstFile: 目标文件 (解密)

 *  返回值:   成功返回0，否则为错误码
 */
GSUTIL_API int GSAES_DecodeFile(const char* pUserKey, const char* pSrcFile, const char* pDstFile);
////////////////////////////////////////////////////////////////////////////////////////

#ifdef  __cplusplus
}

#endif

#endif
