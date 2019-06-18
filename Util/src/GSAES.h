/*
******************************************
Copyright (C): GOSUN
Filename : GSAES.H
Author :  sw
Description: AES�㷨�ӽ������ݼ��ļ�����.Ŀǰֻ֧��ECBģʽ��pkcs7padding���.
********************************************
*/
#ifndef _GS_H_GSAES_H_
#define _GS_H_GSAES_H_

#include "GSUtilDefs.h"
 
#define AES_RET_SUCCESS                     0   //�ɹ�
#define AES_RET_FAIL                        -1  //ʧ��
#define AES_RET_OPENFILE_FAIL               -2  //���ļ�ʧ��
#define AES_RET_WRITEFILE_FAIL              -3  //д�ļ�ʧ��
#define AES_RET_OUT_OF_MEMORY               -4  //�ڴ�������
#define AES_RET_DATA_SIZE_INVALID           -5  //���ݳ��ȷǷ�
#define AES_RET_DATA_ERROR                  -6  //���ݴ���
#define AES_RET_BUFFER_NOT_ENOUGH           -7  //��������С����

#ifdef  __cplusplus   
extern "C" {
#endif 


/*  ����˵��: ����AES��Կ
 *  ����˵��: [OUT]  pUserKey:  ��Կ
              [IN]  nUserKeyLen: ��Կ���ȣ���bitΪ��λ�� ��֧��128λ��196λ��256λ�������ֵΪ0����Ĭ��ʹ��128λ

 *  ����ֵ:   �ɹ�����0������Ϊ������
 *            ע��: ��Ҫ����GSAES_FreeBuffer�ͷ�pUserKey�ڴ�
 */
GSUTIL_API int GSAES_CreateKey(char**pUserKey,  unsigned int nUserKeyLen);


/*  ����˵��: ���ݼ���
 *  ����˵��: [IN]	      pUserKey:       ��Կ
			  [IN]        nKeyLen:        ��Կ���ȣ���bitΪ��λ
              [IN]        pData:          ����
			  [IN]        nDataSize:      ���ݳ���
			  [IN\OUT]    pOutputData��   ���ڴ�ż��ܺ����ݵ��ڴ棬����ΪNULL
			  [IN\OUT]    nOutputSize��   ����ʱ����ʾpOutputData�Ĵ�С������ΪNULL�����ʱ����ʾ���ܺ����ݵĳ��ȡ�
			                              ��pOutputData̫С���᷵��ʧ�ܣ�ͬʱҲ��ͨ����ָ�뷵�������ڴ��С��
 *  ����ֵ:   �ɹ�����0������Ϊ�����롣pOutputData̫С�᷵��AES_RET_BUFFER_NOT_ENOUGH����Ҫ���������㹻����ڴ��ټ���
 */
GSUTIL_API int GSAES_Encrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize );


/*  ����˵��: ���ݽ���
 *  ����˵��: [IN]	      pUserKey:       ��Կ
			  [IN]        nKeyLen:        ��Կ���ȣ���bitΪ��λ
              [IN]        pData:          ����
			  [IN]        nDataSize:      ���ݳ���
			  [IN\OUT]    pOutputData��   ���ڴ�Ž��ܺ����ݵ��ڴ棬����ΪNULL
			  [IN\OUT]    nOutputSize��   ����ʱ����ʾpOutputData�Ĵ�С������ΪNULL�����ʱ����ʾ���ܺ����ݵĳ��ȡ�
			                              ��pOutputData̫С���᷵��ʧ�ܣ�ͬʱҲ��ͨ����ָ�뷵�������ڴ��С��
 *  ����ֵ:   �ɹ�����0 ,����Ϊ�����롣pOutputData̫С�᷵��AES_RET_BUFFER_NOT_ENOUGH����Ҫ���������㹻����ڴ��ٽ���
 */
GSUTIL_API int GSAES_Decrypt( const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize );


/*  ����˵��: �ļ����ݼ���	
 *  ����˵��: [IN]  pUserKey:  ��Կ
              [IN]  nKeyLen:   ��Կ���ȣ���bitΪ��λ
              [IN]  pSrcFile:  Դ�ļ�
			  [IN]  pDstFile:  Ŀ���ļ� (����)

 *  ����ֵ:   �ɹ�����0������Ϊ������
 */
GSUTIL_API int GSAES_EncryptFile(const char* pUserKey, int nKeyLen, const char * pSrcFile, const char* pDstFile);


/*  ����˵��: �ļ����ݽ���
 *  ����˵��: [IN]  pUserKey: ��Կ
              [IN]  nKeyLen:  ��Կ���ȣ���bitΪ��λ
              [IN]  pSrcFile: Դ�ļ�
			  [IN]  pDstFile: Ŀ���ļ� (����)

 *  ����ֵ:   �ɹ�����0������Ϊ������
 */
GSUTIL_API int GSAES_DecryptFile(const char* pUserKey, int nKeyLen, const char* pSrcFile, const char* pDstFile);


/*  ����˵��: �ͷű��㷨������ڴ�
 *  ����˵��: [IN]  pBuffer: �ڴ�ָ��

 *  ����ֵ:   ��
 */
GSUTIL_API void GSAES_FreeBuffer(void *pBuffer); 


/////////////////////////////���½ӿ��Ѳ�����ʹ��/////////////////////////////////////////

/*  ����˵��: ���ݼ��ܣ�����ʹ��GSAES_Encrypt��
 *  ����˵��: [IN]  pUserKey:  ��Կ
              [IN]  pData: ����
			  [IN]  nDataSize: ���ݳ���
			  [OUT] pOutputData: ���ܺ�����
			  [OUT] nOutputSize: ���ܺ����ݳ���

 *  ����ֵ:   �ɹ�����0������Ϊ������
 *            ע��: ��Ҫ����GSAES_FreeBuffer�ͷ�pOutputData�ڴ�
 */
GSUTIL_API int GSAES_EncodeData(const char* pUserKey,const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);

/*  ����˵��: ���ݽ��ܣ�����ʹ��GSAES_Decrypt��
 *  ����˵��: [IN]  pUserKey: ��Կ
              [IN]  pData: ����
			  [IN]  nDataSize: ���ݳ���
			  [OUT] pOutputData: ���ܺ�����
			  [OUT] nOutputSize: ���ܺ����ݳ���

 *  ����ֵ:   �ɹ�����0������Ϊ������
 *            ע��: ��Ҫ����GSAES_FreeBuffer�ͷ�pOutputData�ڴ�
 */
GSUTIL_API int GSAES_DecodeData(const char* pUserKey,const char* pData, int nDataSize,
								char** pOutputData, int& nOutputSize);

/*  ����˵��: �ļ����ݼ��ܣ�����ʹ��GSAES_EncryptFile��
 *  ����˵��: [IN]  pUserKey:  ��Կ
              [IN]  pSrcFile: Դ�ļ�
			  [IN]  pDstFile: Ŀ���ļ� (����)

 *  ����ֵ:   �ɹ�����0������Ϊ������
 */
GSUTIL_API int GSAES_EncodeFile(const char* pUserKey, const char * pSrcFile, const char* pDstFile);



/*  ����˵��: �ļ����ݽ��ܣ�����ʹ��GSAES_DecryptFile��
 *  ����˵��: [IN]  pUserKey: ��Կ
              [IN]  pSrcFile: Դ�ļ�
			  [IN]  pDstFile: Ŀ���ļ� (����)

 *  ����ֵ:   �ɹ�����0������Ϊ������
 */
GSUTIL_API int GSAES_DecodeFile(const char* pUserKey, const char* pSrcFile, const char* pDstFile);
////////////////////////////////////////////////////////////////////////////////////////

#ifdef  __cplusplus
}

#endif

#endif
