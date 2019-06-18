/*
******************************************
Copyright (C): GOSUN
Filename : GSWATCHDOG.H
Author :  zouyx
Description: ���Ź�ģ��
********************************************
*/

#ifndef _GS_H_GSWATCHDOG_H_
#define _GS_H_GSWATCHDOG_H_

#include "GSUtilDefs.h"

class  GSWatchDogCtx;

class GSUTIL_CLASS GSWatchDog
{
private :
	GSWatchDogCtx *m_pCtx;
public :
	GSWatchDog(void);
	~GSWatchDog(void);

	//�������Ź����
	bool Start(void);
	//�رտ��Ź����
	void Stop(void);

	//��ӿ������� czName �������ƣ� iSecTimeout ��ʱʱ�䣬 ��λ��
	//���ؿ������  isEnable �Ƿ�ʹ��
	long AddWatcher(const char *czName, long iSecTimeout, bool isEnable = true);
	//�Ƴ��������� h �������
	void RemoveWatcher( long h);
	//ι����h �������, ������ iSecTimeout ʱ����ѭ��ι���� ������򽫶ϳ�
	void WatcherActive( long h);
	//���ƿ�����ʼ/ֹͣ
	void CtrlWatcher(long h, bool isEnable);
	
	GS_CLASS_NOCOPY(GSWatchDog)
};

#endif //end _GS_H_GSWATCHDOG_H_
