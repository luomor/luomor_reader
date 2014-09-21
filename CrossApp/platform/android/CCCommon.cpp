

#include "platform/CCCommon.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <android/log.h>
#include <stdio.h>
#include <jni.h>

NS_CC_BEGIN

#define MAX_LEN         (CrossApp::kMaxLogLen + 1)

void CCLog(const char * pszFormat, ...)
{
    char buf[MAX_LEN];

    va_list args;
    va_start(args, pszFormat);
    vsnprintf(buf, MAX_LEN, pszFormat, args);
    va_end(args);

    __android_log_print(ANDROID_LOG_DEBUG, "CrossApp debug info", "%s", buf);
}

void CCMessageBox(const char * pszMsg, const char * pszTitle)
{
    showDialogJNI(pszMsg, pszTitle);
}

void CCLuaLog(const char * pszFormat)
{
    __android_log_print(ANDROID_LOG_DEBUG, "CrossApp debug info", "%s", pszFormat);
}

NS_CC_END
