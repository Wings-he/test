/*************************************************
  Copyright (C), 2011-2012, GOSUN 
  File name   : mdump.H      
  Author      : HongShuGuang
  Version     : V1.0.0        
  DateTime    : 2012/5/18
  Description : �����쳣�����ɼ�¼dump���������
*************************************************/

#ifndef _GS_H_MDUMP_H_
#define _GS_H_MDUMP_H_

#include "GSUtilDefs.h"


#ifdef  __cplusplus   
extern "C" {
#endif 

//����UnhandledExceptionFilter,���������������Ӧdump�ļ�������ڳ���һ��ʼ�ĵط����á�
//IsDumpWithFullMemory�Ƿ����������ڴ��dump�ļ�(dump���ܶ�)��true��������dump��false����normal��dump
//IsPreventOtherExceptionHandling������ó�true�����������ط���SetUnhandledExceptionFilter�ĵ���
GSUTIL_API void  MiniDump(bool IsDumpWithFullMemory = true,
			  bool IsPreventOtherExceptionHandling = true);


//IsDumpWithFullMemory�Ƿ����������ڴ��dump�ļ�(dump���ܶ�)��true��������dump��false����normal��dump
GSUTIL_API bool   getDump(bool IsDumpWithFullMemory = true);


#ifdef  __cplusplus
}


#endif


#endif