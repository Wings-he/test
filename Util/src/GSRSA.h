/*
******************************************
Copyright (C): GOSUN
Filename : GSRSA.H
Author :  sw
Description: RSA算法加解密数据及文件内容
********************************************
*/
#ifndef _GS_H_GSRSA_H_
#define _GS_H_GSRSA_H_

#include "GSUtilDefs.h"

#ifdef  __cplusplus   
extern "C" {
#endif 


/*  函数说明: 生成公钥与私钥
 *  参数说明: [OUT]  pPublicKey:  公钥
              [OUT]  pPrivateKey: 私钥

 *  返回值:   成功返回0，否则为-1
 *            注意: 需要调用者释放pPublicKey与pPrivateKey内存
 */
	GSUTIL_API int GSRSA_CreateKey(char**pPublicKey,  int &nPublicKey,
	                               char**pPrivateKey, int &nPrivateKey);


/*  函数说明: 数据加密
 *  参数说明: [IN]  pPrivateKey:  私钥
              [IN]  pData: 数据
			  [IN]  nDataSize: 数据长度
			  [OUT] pOutputData: 加密后数据
			  [OUT] nOutputSize: 加密后数据长度

 *  返回值:   成功返回0，否则为-1
 *            注意: 需要调用者释放pOutputData内存
 */
GSUTIL_API int GSRSA_EncodeData(const char* pPrivateKey, const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);


/*  函数说明: 数据解密
 *  参数说明: [IN]  pPublicKey: 公钥
              [IN]  pData: 数据
			  [IN]  nDataSize: 数据长度
			  [OUT] pOutputData: 解密后数据
			  [OUT] nOutputSize: 解密后数据长度

 *  返回值:   成功返回0，否则为-1
 *            注意: 需要调用者释放pOutputData内存
 */
GSUTIL_API int GSRSA_DecodeData(const char* pPublicKey, const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);


/*  函数说明: 文件数据加密
 *  参数说明: [IN]  pPrivateKey:  私钥
              [IN]  pSrcFile: 源文件
			  [IN]  pDstFile: 目标文件 (加密)

 *  返回值:   成功返回0，否则为-1
 */
GSUTIL_API int GSRSA_EncodeFile(char* pPrivateKey, char const* pSrcFile, const char* pDstFile);



/*  函数说明: 文件数据解密
 *  参数说明: [IN]  pPublicKey: 公钥
              [IN]  pSrcFile: 源文件
			  [IN]  pDstFile: 目标文件 (解密)

 *  返回值:   成功返回0，否则为-1
 */
GSUTIL_API int GSRSA_DecodeFile(char* pPublicKey, const char* pSrcFile, const char* pDstFile);

#ifdef  __cplusplus
}

#endif

#endif
