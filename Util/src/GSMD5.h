/*
******************************************
Copyright (C): GOSUN
Filename : GSMD5.H
Author :  zouyx
Description: 
********************************************
*/

#ifndef _GS_H_GSMD5_H_
#define _GS_H_GSMD5_H_


#include "GSUtilDefs.h"
#include <string>


typedef UInt16 UINT2;
typedef UInt32 UINT4;



/* MD5 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];                               /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;


#define MD5_LEN 16

GSUTIL_API void GSMD5Init(MD5_CTX *context);
GSUTIL_API void GSMD5Update(MD5_CTX *context, const void *pInputData, unsigned int inputLen);
GSUTIL_API void GSMD5Final (unsigned char digest[MD5_LEN], MD5_CTX *); 
GSUTIL_API void GSMD5Sum(const void *pInputData, int inputLen, unsigned char md5res[MD5_LEN] );

static INLINE std::string GSMD5toString(unsigned char md5res[MD5_LEN])
{
	char szTemp[MD5_LEN*2+2];
	char *p = szTemp;
	for( int i = 0; i<MD5_LEN; i++ )
	{
		gs_snprintf(p, 3, "%02x", md5res[i] );
		p += 2;
	}
	*p = '\0';
	return std::string(szTemp);
}




#endif //end _GS_H_GSMD5_H_
