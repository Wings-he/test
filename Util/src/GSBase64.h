/*******************************************
Copyright (C): GOSUN
Filename : GSBase64.h
Author :  sw
Description: base64���ݱ����
********************************************/


#ifndef _GSBASE64_H_
#define _GSBASE64_H_

#include "GSUtilDefs.h"

#ifdef  __cplusplus   
extern "C" {
#endif 


/*  ����˵��: BASE64����
 *  ����˵��: [IN]  pData: Դ����
              [IN]  nResultSize: ��������ݳ���
              [IN]  bTrimTrailingZeros: �Ƿ�ĩβ�������, Ĭ��TRUE 
              
 *  ����ֵ:   ��������
 *            ע��: ��Ҫ����GSBase64FreeBuffer�ͷŷ��صĽ��������ڴ�
 */
GSUTIL_API unsigned char* GSBase64Decode(char const* pData, unsigned& nResultSize, BOOL bTrimTrailingZeros = TRUE);



/*  ����˵��: BASE64����
 *  ����˵��: [IN]  pData: Դ����
              [IN]  nSize: Դ���ݳ���
              
 *  ����ֵ:   �������� 
 *            ע��: ��Ҫ����GSBase64FreeBuffer�ͷŷ��صı��������ڴ�
 */
GSUTIL_API char* GSBase64Encode(char const* pData, unsigned nSize);


/*  ����˵��: �ͷű��㷨������ڴ�
 *  ����˵��: [IN]  pBuffer: �ڴ�ָ��

 *  ����ֵ:   ��
 */
GSUTIL_API void GSBase64FreeBuffer(const char * const pBuffer);

#ifdef  __cplusplus
}


#endif

#endif
