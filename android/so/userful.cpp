#include <jni.h>
#include <string>
#include <android/log.h>

#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO, "========= Info =========   ", __VA_ARGS__)

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "========= Error =========   ", __VA_ARGS__)

#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO, "========= Debug =========   ", __VA_ARGS__)

#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, "========= Warn =========   ", __VA_ARGS__)

//获取app包名
static int getPackageName(JNIEnv* env,
                          jobject  object, char * buf, int bufLen ){
    jclass contextClass = (jclass)(env)->NewGlobalRef((env)->FindClass("android/content/Context"));
    jmethodID getPackageNameId = (env)->GetMethodID(contextClass, "getPackageName","()Ljava/lang/String;");
    jstring packNameString =  (jstring)(env)->CallObjectMethod(object, getPackageNameId);
    const char* packName = env->GetStringUTFChars(packNameString, NULL);
    if(packName && (strlen(packName) <= bufLen) ){
        LOGI("packageName: %s", packName);
        strncpy(buf,packName,bufLen);

        env->ReleaseStringUTFChars(packNameString, packName);
        return  0;
    }
    env->ReleaseStringUTFChars(packNameString, packName);


    return -1;
}

