/*******************************************
Copyright (C): GOSUN
Filename : GSBase64.h
Author :  sw
Description: base64数据编解码
********************************************/


#ifndef _GSBASE64_H_
#define _GSBASE64_H_

#include "GSUtilDefs.h"

#ifdef  __cplusplus   
extern "C" {
#endif 


/*  函数说明: BASE64解码
 *  参数说明: [IN]  pData: 源数据
              [IN]  nResultSize: 解码后数据长度
              [IN]  bTrimTrailingZeros: 是否末尾补零对齐, 默认TRUE 
              
 *  返回值:   解码数据
 *            注意: 需要调用GSBase64FreeBuffer释放返回的解码数据内存
 */
GSUTIL_API unsigned char* GSBase64Decode(char const* pData, unsigned& nResultSize, BOOL bTrimTrailingZeros = TRUE);



/*  函数说明: BASE64编码
 *  参数说明: [IN]  pData: 源数据
              [IN]  nSize: 源数据长度
              
 *  返回值:   编码数据 
 *            注意: 需要调用GSBase64FreeBuffer释放返回的编码数据内存
 */
GSUTIL_API char* GSBase64Encode(char const* pData, unsigned nSize);


/*  函数说明: 释放本算法申请的内存
 *  参数说明: [IN]  pBuffer: 内存指针

 *  返回值:   无
 */
GSUTIL_API void GSBase64FreeBuffer(const char * const pBuffer);

#ifdef  __cplusplus
}


#endif

#endif
