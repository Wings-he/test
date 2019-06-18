#include "GSTempString.h"


#define STRUCT_INI() \
	bzero(m_szBase,sizeof(m_szBase) ); \
	m_pStrBuf = m_szBase; \
	m_nBufSize = sizeof(m_szBase); \
	m_iStrLen = 0

#define _FIT_BUF(x) \
	if( (x) >= m_nBufSize ) {\
		if( m_pStrBuf != m_szBase && m_pStrBuf ){ free(m_pStrBuf);} \
		m_iStrLen = 0; 	m_nBufSize = (x)+sizeof(m_szBase); \
		m_pStrBuf = (char*)malloc(m_nBufSize); \
		if( !m_pStrBuf ){ m_nBufSize=0; GSAbort(m_pStrBuf); }\
	}



GSTempString::GSTempString(void)
{
	STRUCT_INI();
}

GSTempString::GSTempString(const GSTempString &csDest)

{
	STRUCT_INI();

	*this = csDest;
}

GSTempString::GSTempString(const char *czStr)
{
	STRUCT_INI();
	*this = czStr;
}

GSTempString::~GSTempString(void)
{
	if( m_pStrBuf != m_szBase && m_pStrBuf )
	{
		free(m_pStrBuf);
		STRUCT_INI();
	}
}


bool GSTempString::empty(void) const
{
	return (m_iStrLen==0);
}

void GSTempString::clear(void)
{
	m_iStrLen = 0;
}

int GSTempString::size(void) const
{
	return m_iStrLen;
}

int  GSTempString::length(void) const
{
	return m_iStrLen;
}

const char *GSTempString::c_str(void) const
{
	return m_pStrBuf;
}

GSTempString &GSTempString::operator=(const GSTempString &csDest)
{
	if( this != &csDest )
	{
		_FIT_BUF(csDest.m_iStrLen);
		memcpy(m_pStrBuf, csDest.m_pStrBuf,csDest.m_iStrLen+1);		
		m_iStrLen = csDest.m_iStrLen;
	}
	return *this;
}

GSTempString &GSTempString::operator += (const GSTempString &csDest)
{
	if( (m_iStrLen+csDest.m_iStrLen) >=  m_nBufSize )
	{
		
		int nNewBufSize = (m_iStrLen+csDest.m_iStrLen)+sizeof(m_szBase);
		char *czNew = (char*)malloc(nNewBufSize); 

		if( czNew == NULL )
		{
			GSAbort(0);
			return *this;
		}
		if( m_iStrLen )
		{
			memcpy( czNew, m_pStrBuf, m_iStrLen+1);
		}
		if(  m_pStrBuf!=m_szBase && m_pStrBuf )
		{
			free(m_pStrBuf);
		}
		m_pStrBuf = czNew;
		m_nBufSize = nNewBufSize;
	}
	memcpy(&m_pStrBuf[m_iStrLen], csDest.m_pStrBuf,csDest.m_iStrLen+1);
	m_iStrLen += csDest.m_iStrLen;	
	return *this;
}

GSTempString &GSTempString::operator=(const char *czStr)
{
	m_pStrBuf[0] = '\0';
	m_iStrLen = 0;
	if( czStr != NULL )
	{		
		int n  = strlen(czStr);
		_FIT_BUF(n);
		memcpy(m_pStrBuf, czStr,n+1);
		m_iStrLen = n;
	}
	return *this;	
}

GSTempString &GSTempString::operator += (const char *czStr)
{
	if( czStr )
	{
		int n  = strlen(czStr);
		if( (m_iStrLen+n) >=  m_nBufSize )
		{

			int nNewBufSize = (m_iStrLen+n)+sizeof(m_szBase);
			char *czNew = (char*)malloc(nNewBufSize); 

			if( czNew == NULL )
			{
				GSAbort(0);
				return *this;
			}
			if( m_iStrLen )
			{
				memcpy( czNew, m_pStrBuf, m_iStrLen+1);
			}
			if( m_pStrBuf && m_pStrBuf!=m_szBase )
			{
				free(m_pStrBuf);
			}
			m_pStrBuf = czNew;
			m_nBufSize = nNewBufSize;
		}
		memcpy(&m_pStrBuf[m_iStrLen], czStr,n+1);
		m_iStrLen += n;
	}
	return *this;
}

GSTempString GSTempString::operator+(const GSTempString &csDest)
{
	GSTempString ret = *this;
	ret += csDest;
	return ret;
}


bool GSTempString::operator==(const GSTempString &csDest) const
{
	if( this != &csDest )
	{
		return (0==strcmp(m_pStrBuf, csDest.m_pStrBuf));
	}
	return false;
}

bool GSTempString::operator==(const char *czDest) const
{
	if( czDest )
	{
		if( czDest == m_pStrBuf )
		{
			return true;
		}
		return (0==strcmp(m_pStrBuf, czDest) );
	}	
	return m_iStrLen==0;	
}

bool GSTempString::operator!=(const GSTempString &csDest) const
{
	return !(*this==csDest);
}

bool GSTempString::operator!=(const char *czDest) const
{
	return !(*this==czDest);
}

bool GSTempString::operator<(const GSTempString &csDest) const
{
	if( this != &csDest )
	{
		return (strcmp(m_pStrBuf, csDest.m_pStrBuf)<0 );
	}
	return false;
}

bool GSTempString::operator>(const GSTempString &csDest) const
{
	if( this != &csDest )
	{
		return (strcmp(m_pStrBuf, csDest.m_pStrBuf)>0 );
	}
	return false;
}

char &GSTempString::operator [](int i)
{	
	GSAbort(i>=0 && i<(m_iStrLen+1));
	return m_pStrBuf[i];

}

const char GSTempString::operator [](int i) const
{
	GSAbort(i>=0 && i<(m_iStrLen+1));
	return m_pStrBuf[i];
}

const char *GSTempString::operator()() const
{
	return m_pStrBuf;
}

GSTempString operator+(const GSTempString &c1, const GSTempString &c2)
{
	GSTempString ret = c1;
	ret += c2;
	return ret;
}


