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
������������ʽ��
ħ���֣�GCRYPT��| ������Ϣ���ȣ����ܷ�ʽ��ʶ+������Ϣ�� | ���ܷ�ʽ��ʶ | ������Ϣ | ��������
*/

typedef enum EnumStreamEncryptType
{
	AES,  //��ӦStruStreamEncryptInfo_AES
}EnumStreamEncryptType;

typedef struct StruStreamEncryptHead
{
	UInt32 nMaigc1;//GCRY
	UInt16 nMaigc2;//PT
	UInt16 nLength;//���¼�����Ϣ���ݵĳ���
	UInt16 nType;//EnumStreamEncryptType
}GS_ENCRYPT_ATTRIBUTE_PACKED StruStreamEncryptHead;

typedef struct StruStreamEncryptInfo_AES
{
	UInt32 nEncryptLen;    //���ڼ���ʱ����ʾ��Ҫ���ܵ����ݳ���
	                       //���ܺ󣬱�ʾ���ܲ��ֵ����ݳ���
}GS_ENCRYPT_ATTRIBUTE_PACKED StruStreamEncryptInfo_AES;

#ifdef _WIN32
#pragma pack( pop )
#endif


/****************************************************************
* ��������
* ����˵����eType	             (�������)  ���ܷ�ʽ
            pEncryptInfo         (�������)  ���ܷ�ʽ��Ӧ�����������Ϣ
			pUserKey             (�������)  ������Կ
			nKeyLen              (�������)  ������Կ����
			pData                (�������)  ����������
			nDataSize            (�������)  ���������ݳ���
			pOutputData          (�������)  ���ڴ�ż��ܺ����ݵ��ڴ棬����ΪNULL
			nOutputSize        (����\������) ����ʱ��ʾpOutputData�Ĵ�С������ΪNULL�����ʱ����ʾ���ܺ����ݵĳ��ȡ�
			                                 ��pOutputData̫С���᷵��ʧ�ܣ�ͬʱҲ��ͨ����ָ�뷵�������ڴ��С��
* ����ֵ˵�����ɹ�����AES_RET_SUCCESS��ʧ�ܷ�������AES_RET_*�Ĵ�����
******************************************************************/
GSUTIL_API int GSStreamEncrypt(EnumStreamEncryptType eType, void* pEncryptInfo, const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int *nOutputSize);

/****************************************************************
* ��������
* ����˵����pUserKey             (�������)  ��Կ
			nKeyLen              (�������)  ��Կ����
			pData                (�������)  ����������
			nDataSize            (�������)  ���������ݳ���
			pOutputData          (�������)  ���ڴ�Ž��ܺ����ݵ��ڴ棬����ΪNULL
			nOutputSize        (����\������) ����ʱ��ʾpOutputData�Ĵ�С������ΪNULL�����ʱ����ʾ���ܺ����ݵĳ��ȡ�
			                                 ��pOutputData̫С���᷵��ʧ�ܣ�ͬʱҲ��ͨ����ָ�뷵�������ڴ��С��
* ����ֵ˵�����ɹ�����AES_RET_SUCCESS��ʧ�ܷ���AES_RET_*�Ĵ�����
******************************************************************/
GSUTIL_API int GSStreamDecrypt(const char* pUserKey, int nKeyLen, const char* pData, int nDataSize, char* pOutputData, int* nOutputSize);

/****************************************************************
* �ж��Ƿ��������,
* ����˵����pData                (�������)  ����
			nDataSize            (�������)  ���ݳ���
* ����ֵ˵�����Ǽ����������Ҽ�����Ϣ��������1�����򷵻�0
******************************************************************/
GSUTIL_API BOOL IsEncryptedStream( const char* pData, int nDataSize );