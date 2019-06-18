/*
******************************************
Copyright (C): GOSUN
Filename : GSUTILEXPORT.H
Author :  sw
Description: 导出函数定义
********************************************
*/

#ifndef _GS_H_GSUTILEXPORT_H_
#define _GS_H_GSUTILEXPORT_H_


#include "GSTypes.h"
#include "GSDefs.h"

#ifdef GSUTIL_STATIC

# define GSUTIL_CLASS  
# define GSUTIL_API    
#else

#ifdef GSUTIL_EXPORTS

# define GSUTIL_CLASS  EXPORT_CLASS
# define GSUTIL_API    EXPORT_API

#else

# define GSUTIL_CLASS  IMPORT_CLASS
# define GSUTIL_API    IMPORT_API

#endif
#endif


#ifdef _LINUX


#endif





#endif //end _GS_H_GSUTILEXPORT_H_
