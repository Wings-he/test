APP_STL := gnustl_static
APP_PLATFORM := android-19
APP_OPTIM := $(GS_OPTIM)
APP_ABI := $(GS_ABI)


# 在此处多个 android.mk 脚本
# 	注：application.mk 等同于 sln，每个 android.mk 等同于一个 vcproject
APP_BUILD_SCRIPT := ./src/Android.mk


