LOCAL_PATH := $(call my-dir)

ifeq ($(APP_OPTIM),debug)
    cmd-strip := 
endif
include $(CLEAR_VARS)
LOCAL_MODULE	:=	GSFoundation
LOCAL_SRC_FILES	:=	./../../Goctcp_lib/Foundation/armeabi-v7a/libGSFoundation.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Goctcp_lib/Foundation
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)


##################			##################
##################			##################
##################			##################

LOCAL_C_INCLUDES := $(LOCAL_PATH)/  \
                    $(LOCAL_PATH)/../../Include \


LOCAL_CPPFLAGS  +=  -w

LOCAL_MODULE := GSUtil

LOCAL_SHARED_LIBRARIES	+=	GSFoundation
		 



LOCAL_SRC_FILES := cJSON.cpp \
				   GSIni.cpp \
				   GSMD5.cpp \
				   GSTempString.cpp \
				   GSWatchDog.cpp \
				   tinystr.cpp	\
				   tinyxml.cpp	\
				   tinyxmlerror.cpp	\
				   xmlParser.cpp \
				   tinyxmlparser.cpp \
				   mdump.cpp \
				   GSBase64.cpp \
				   GSAES.cpp \
				   AES.cpp \
				   StreamEncrypt.cpp





include $(BUILD_SHARED_LIBRARY)

