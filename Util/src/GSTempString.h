/*
******************************************
Copyright (C): GOSUN
Filename : GSTEMPSTRING.H
Author :  zouyx
Description: 
********************************************
*/

#ifndef _GS_H_GSTEMPSTRING_H_
#define _GS_H_GSTEMPSTRING_H_

#include <string>

#include "GSUtilDefs.h"

#if  defined(_MSWINDOWS_) || (!defined(_ANDROID_) && !defined(_IOS_)) 
#include <iostream>
#endif

class GSUTIL_CLASS GSTempString
{
private :
	char m_szBase[128];
	char *m_pStrBuf;
	int m_nBufSize;
	int m_iStrLen;
	
public:
	GSTempString(void);
	GSTempString(const GSTempString &csDest);
	GSTempString(const char *czStr);
	~GSTempString(void);

	bool empty(void) const;
	void clear(void);
	int size(void) const;
	int  length(void) const;
	const char *c_str(void) const;

	GSTempString &operator=(const GSTempString &csDest);
	GSTempString &operator +=(const GSTempString &csDest);
	GSTempString &operator=(const char *czStr);
	GSTempString &operator += (const char *czStr);

	GSTempString operator+(const GSTempString &csDest);

	bool operator==(const GSTempString &csDest) const;
	bool operator==(const char *czDest) const;	
	bool operator!=(const GSTempString &csDest) const;
	bool operator!=(const char *czDest) const;
	bool operator<(const GSTempString &csDest) const;	
	bool operator>(const GSTempString &csDest) const;
	char &operator [](int i);
	const char operator [](int i) const;

    const char *operator()() const;

#if  defined(_MSWINDOWS_) || (!defined(_ANDROID_) && !defined(_IOS_)) 
	
	friend  std::ostream &operator << (std::ostream &out,const GSTempString &src)
	{
		if( !src.empty() )
		{
			out << src.c_str();
		}
		return out;
	}

#endif
	friend GSTempString operator+(const GSTempString &c1, const GSTempString &c2);
	
};


#endif //end _GS_H_GSTEMPSTRING_H_
