#include "GSWatchDog.h"
#include "GSFoundation.h"
#include "GSStrUtil.hpp"
#include <map>

#ifdef _LINUX
#include <sys/types.h>
#include <signal.h>
#endif


class GSWatchDogCtx
{
private :
	GSThread m_thread;
public  :
	long m_iHandleSeq;
	typedef struct _StruWatcher
	{
		long hHandle;
		long iIntervalTicks;
		long iTimeouts;
		GSString strName;
		bool isEnable;
		_StruWatcher(void) : hHandle(-1), 
			iIntervalTicks(0)
			,iTimeouts(300)
			,strName("")
			,isEnable(true)
		{

		}
	}StruWatcher;

	std::map<long, StruWatcher > m_mapWatcher;
	GSMutex m_mutex;		
public :
	GSWatchDogCtx(void)
	{
		m_iHandleSeq = 0;
	}

	~GSWatchDogCtx(void)
	{
		
	}

	bool Start(void)
	{
		return  m_thread.Start(ThreadProxy, this);
	}
	void Stop(void)
	{
		m_thread.Stop();
		m_thread.Join();
	}
private :
	static void GS_CALLBACK ThreadProxy(GSThread &thread, void *pPara)
	{
		GSWatchDogCtx *p = (GSWatchDogCtx*)pPara;
		p->Entry();
	}

	void Entry(void)
	{
		bool bAbort = false;
		GSString strAbortObjName = "";
		while( !m_thread.TestExit() && !bAbort )
		{
			GSSleep(100);	
			m_mutex.Lock();
			for( std::map<long, StruWatcher >::iterator csIt=m_mapWatcher.begin();
				csIt != m_mapWatcher.end(); ++csIt )
			{
				if( !csIt->second.isEnable )
				{
					continue;
				}
				csIt->second.iIntervalTicks++;
				long iInterval = (long)(csIt->second.iIntervalTicks/10);

				if( iInterval > csIt->second.iTimeouts )
				{
					strAbortObjName = csIt->second.strName;
					break;
				}
			}
			m_mutex.Unlock();
		}
		if( bAbort )
		{
			WriteExit(strAbortObjName);			
#ifdef _LINUX		
			GSSleep(100);
			raise(SIGKILL);
			GSSleep(200);
			_exit(-1);			
#else		
			GSAbort(0);
			exit(-1);		
#endif		
		}
	}

	void WriteExit( const GSString &strObjName )
	{
		GSString strApp = GSGetApplicationPath();
		strApp += "watchdog.log";		
		GSDateTime csTime;
		std::stringstream sstr;
		sstr << csTime.Format();
		sstr << " 看门狗对象:" << strObjName << " 超时没有活动. 程序将退出!\n";
		GSString strInfo = sstr.str();
		fprintf( stderr, "%s", strInfo.c_str());

		FILE *fp = fopen(strApp.c_str(), "a+");
		if( fp)
		{
			fputs(strInfo.c_str(), fp);
			fclose(fp);
		}

	}
};



GSWatchDog::GSWatchDog(void)
{
	m_pCtx = new GSWatchDogCtx();
}

GSWatchDog::~GSWatchDog(void)
{
	if( m_pCtx )
	{
		m_pCtx->Stop();
		delete m_pCtx;
		m_pCtx = NULL;
	}
}


bool GSWatchDog::Start(void)
{
	if( m_pCtx )
	{
		return m_pCtx->Start();
	}
	return false;
}

void GSWatchDog::Stop(void)
{
	if( m_pCtx )
	{
		return m_pCtx->Stop();
	}
}

void GSWatchDog::CtrlWatcher(long h, bool isEnable)
{
	if(m_pCtx )
	{

		GSAutoMutex locker(m_pCtx->m_mutex);		
		std::map<long, GSWatchDogCtx::StruWatcher >::iterator csIt;
		csIt = m_pCtx->m_mapWatcher.find(h);
		if( csIt != m_pCtx->m_mapWatcher.end() )
		{
			if( isEnable )
			{
				csIt->second.iIntervalTicks = 0;
			}
			csIt->second.isEnable = isEnable;
			
		}
	}
}

long GSWatchDog::AddWatcher(const char *czName, long iSecTimeout, bool isEnable)
{
	GSAbort(m_pCtx);
	GSAutoMutex locker(m_pCtx->m_mutex);
	GSWatchDogCtx::StruWatcher w;
	do 
	{
		w.hHandle = m_pCtx->m_iHandleSeq++;
	}while ( m_pCtx->m_mapWatcher.find(w.hHandle)!=m_pCtx->m_mapWatcher.end() );

	w.iTimeouts = iSecTimeout;
	w.strName = czName;
	w.iIntervalTicks = 0;
	w.isEnable = isEnable;
	m_pCtx->m_mapWatcher.insert(std::make_pair(w.hHandle, w));
	return w.hHandle;
}

void GSWatchDog::RemoveWatcher( long h)
{
	if(m_pCtx )
	{
		GSAutoMutex locker(m_pCtx->m_mutex);
		m_pCtx->m_mapWatcher.erase(h);
	}
}

void GSWatchDog::WatcherActive( long h)
{
	if(m_pCtx )
	{

		GSAutoMutex locker(m_pCtx->m_mutex);		
		std::map<long, GSWatchDogCtx::StruWatcher >::iterator csIt;
		csIt = m_pCtx->m_mapWatcher.find(h);
		if( csIt != m_pCtx->m_mapWatcher.end() )
		{
			csIt->second.iIntervalTicks = 0;
		}
	}
}