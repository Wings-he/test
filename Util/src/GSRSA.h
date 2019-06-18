/*
******************************************
Copyright (C): GOSUN
Filename : GSRSA.H
Author :  sw
Description: RSA�㷨�ӽ������ݼ��ļ�����
********************************************
*/
#ifndef _GS_H_GSRSA_H_
#define _GS_H_GSRSA_H_

#include "GSUtilDefs.h"

#ifdef  __cplusplus   
extern "C" {
#endif 


/*  ����˵��: ���ɹ�Կ��˽Կ
 *  ����˵��: [OUT]  pPublicKey:  ��Կ
              [OUT]  pPrivateKey: ˽Կ

 *  ����ֵ:   �ɹ�����0������Ϊ-1
 *            ע��: ��Ҫ�������ͷ�pPublicKey��pPrivateKey�ڴ�
 */
	GSUTIL_API int GSRSA_CreateKey(char**pPublicKey,  int &nPublicKey,
	                               char**pPrivateKey, int &nPrivateKey);


/*  ����˵��: ���ݼ���
 *  ����˵��: [IN]  pPrivateKey:  ˽Կ
              [IN]  pData: ����
			  [IN]  nDataSize: ���ݳ���
			  [OUT] pOutputData: ���ܺ�����
			  [OUT] nOutputSize: ���ܺ����ݳ���

 *  ����ֵ:   �ɹ�����0������Ϊ-1
 *            ע��: ��Ҫ�������ͷ�pOutputData�ڴ�
 */
GSUTIL_API int GSRSA_EncodeData(const char* pPrivateKey, const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);


/*  ����˵��: ���ݽ���
 *  ����˵��: [IN]  pPublicKey: ��Կ
              [IN]  pData: ����
			  [IN]  nDataSize: ���ݳ���
			  [OUT] pOutputData: ���ܺ�����
			  [OUT] nOutputSize: ���ܺ����ݳ���

 *  ����ֵ:   �ɹ�����0������Ϊ-1
 *            ע��: ��Ҫ�������ͷ�pOutputData�ڴ�
 */
GSUTIL_API int GSRSA_DecodeData(const char* pPublicKey, const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);


/*  ����˵��: �ļ����ݼ���
 *  ����˵��: [IN]  pPrivateKey:  ˽Կ
              [IN]  pSrcFile: Դ�ļ�
			  [IN]  pDstFile: Ŀ���ļ� (����)

 *  ����ֵ:   �ɹ�����0������Ϊ-1
 */
GSUTIL_API int GSRSA_EncodeFile(char* pPrivateKey, char const* pSrcFile, const char* pDstFile);



/*  ����˵��: �ļ����ݽ���
 *  ����˵��: [IN]  pPublicKey: ��Կ
              [IN]  pSrcFile: Դ�ļ�
			  [IN]  pDstFile: Ŀ���ļ� (����)

 *  ����ֵ:   �ɹ�����0������Ϊ-1
 */
GSUTIL_API int GSRSA_DecodeFile(char* pPublicKey, const char* pSrcFile, const char* pDstFile);

#ifdef  __cplusplus
}

#endif

#endif
