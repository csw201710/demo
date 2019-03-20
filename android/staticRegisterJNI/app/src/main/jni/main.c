#include <jni.h>
#define jintJNICALL
#ifndef _Included_com_example_caculate_MainActivity
#define _Included_com_example_caculate_MainActivity
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jintJNICALL Java_com_example_caculate_MainActivity_Add
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    return (jint)(num1 + num2+2);
}


JNIEXPORT jintJNICALL Java_com_example_caculate_MainActivity_Sub
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    return (jint)(num1 - num2+2);
}


JNIEXPORT jintJNICALL Java_com_example_caculate_MainActivity_Mul
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    return (jint)(num1 * num2+2);
}

JNIEXPORT jintJNICALL Java_com_example_caculate_MainActivity_Div
        (JNIEnv *env, jobject obj, jdouble num1, jdouble num2)
{
    if (num2 == 0) return 0;
    return (jint)(num1 / num2+2);
}
#ifdef __cplusplus
}
#endif
#endif