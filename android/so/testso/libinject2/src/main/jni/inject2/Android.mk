LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := inject2
LOCAL_SRC_FILES := inject.c

LOCAL_LDLIBS := -llog -Wpointer-sign -ldl -w

include $(BUILD_EXECUTABLE)
