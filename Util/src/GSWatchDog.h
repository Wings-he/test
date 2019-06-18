/*
******************************************
Copyright (C): GOSUN
Filename : GSWATCHDOG.H
Author :  zouyx
Description: 看门狗模块
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

	//启动看门狗检测
	bool Start(void);
	//关闭看门狗检测
	void Stop(void);

	//添加看护对象， czName 对象名称， iSecTimeout 超时时间， 单位秒
	//返回看护句柄  isEnable 是否使能
	long AddWatcher(const char *czName, long iSecTimeout, bool isEnable = true);
	//移除看护对象， h 看护句柄
	void RemoveWatcher( long h);
	//喂狗，h 看护句柄, 必须在 iSecTimeout 时间内循环喂狗， 否则程序将断出
	void WatcherActive( long h);
	//控制看护开始/停止
	void CtrlWatcher(long h, bool isEnable);
	
	GS_CLASS_NOCOPY(GSWatchDog)
};

#endif //end _GS_H_GSWATCHDOG_H_
