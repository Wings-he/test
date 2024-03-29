#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define INNER_VERSION 0
#define REV_VERSION   37220

#define _VER2STR(ver) #ver
#define VER2STR(ver) _VER2STR(ver)

#define VERSION  VER2STR(MAJOR_VERSION) \
                     "."VER2STR(MINOR_VERSION) \
                     "."VER2STR(INNER_VERSION) \
                     "."VER2STR(REV_VERSION)

#define MAKE_DATETIME_SVN	2019/06/18 17:36:09
#define MAKE_DATETIME	VER2STR(MAKE_DATETIME_SVN)

#define PRODUCT_NAME		"���ƻ�����"
#define PRODUCT_VER_VAL		1.0.0.0
#define PRODUCT_VER_STR		"1.0.0.0\0"
#define COMPANY_NAME		"Gosuncn technology group Co., Ltd."
#define LEGAL_COPYRIGHT		"Copyright (C) 2015 Gosuncn technology group Co., Ltd."

#define FILE_VERSION_VAL	 MAJOR_VERSION,MINOR_VERSION,INNER_VERSION,REV_VERSION
#define FILE_VERSION_STR	 VER2STR(MAJOR_VERSION.MINOR_VERSION.INNER_VERSION.REV_VERSION)
#ifdef WIN64
#define FILE_DESC		 "GSUtil ��̬���ӿ�(64 bit)"
#else
#define FILE_DESC		 "GSUtil ��̬���ӿ�(32 bit)"
#endif
#define ORIGINAL_FILE_NAME	 ""
#define FILE_COMMENTS  "���ƻ����� ��̬���ӿ�"
