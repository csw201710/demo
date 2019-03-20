#include <jni.h>
#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


JNIEXPORT jint JNICALL native_Add
(JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
return (jint)(num1 + num2 +1);
}


JNIEXPORT jint JNICALL native_Sub
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    return (jint)(num1 - num2 +1);
}


JNIEXPORT jint JNICALL native_Mul
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    return (jint)(num1 * num2 +1);
}

JNIEXPORT jint JNICALL native_Div
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    if (num2 == 0) return 0;
    return (jint)(num1 / num2 +1);
}

//Java和JNI函数的绑定表
static JNINativeMethod gMethods[] = {
        {"Add", "(DD)I", (void *)native_Add},
        {"Sub", "(DD)I", (void *)native_Sub},
        {"Mul", "(DD)I", (void *)native_Mul},
        {"Div", "(DD)I", (void *)native_Div},
};




//注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className,
                                JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods,numMethods) < 0){
        return JNI_FALSE;
    }

    return JNI_TRUE;
}


int register_ndk_load(JNIEnv *env)
{

    return registerNativeMethods(env, "com/example/caculate/MainActivity",
                                 gMethods,sizeof(gMethods) / sizeof(gMethods[0]));
                                 //NELEM(gMethods));
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    register_ndk_load(env);

    // 返回jni的版本
    return JNI_VERSION_1_4;
}