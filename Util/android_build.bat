@echo off
@echo. ----------------------------------
@echo. Android Build Script
rem 修改(1)
@echo.              GSUtil
@echo. ----------------------------------
@echo.

rem 修改(2)
set dst_dir=.\..\Goctcp_lib\Util\
@echo. -----------------
@echo. 当前目录   %cd%
@echo. 输出目录   %dst_dir%
@echo. -----------------
@echo.
set GS_OPTIM=%1
set GS_ABI=%2

if /I "%GS_OPTIM%" == "" (
	set GS_OPTIM=release
)
if /I "%GS_ABI%" == "" (
    set GS_ABI=armeabi-v7a
)

rem 编译工具默认目录    C:\android-ndk-r16\ndk-build
if defined gs-ndk-build (
    set my_ndk_build=%gs-ndk-build%
)else (
    set my_ndk_build=C:\android-ndk-r16\ndk-build
)

@echo. -----------------
@echo. NDK 编译工具路径 = %my_ndk_build%
@echo. 正在编译......
@echo. -----------------

call %my_ndk_build% -j 4 NDK_PROJECT_PATH=.    ^
    NDK_APPLICATION_MK=./Application.mk         ^
    NDK_APP_DST_DIR=%dst_dir%%GS_ABI%  ^
    GS_OPTIM=%GS_OPTIM% ^
    GS_ABI=%GS_ABI%

if %errorlevel% NEQ 0 (
    @echo.
    @echo.      ***** 编译过程出错 *****
    @echo.
    pause
    goto END
)
@echo.


@echo. -----------------
@echo. 复制头文件
@echo. -----------------
rem 修改(3)
echo /d | xcopy /Y/C ".\src\GSUtilDefs.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSIni.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSUtil.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\tinystr.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\tinyxml.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\xmlParser.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\mdump.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\cJSON.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSWatchDog.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSMD5.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSTempString.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSAES.h" %dst_dir%
echo /d | xcopy /Y/C ".\src\CommonXml.hpp" %dst_dir%
echo /d | xcopy /Y/C ".\src\GSBase64.h" %dst_dir%

@echo.

:END
@echo. -----------------
@echo. done!
@echo. -----------------
@echo.

