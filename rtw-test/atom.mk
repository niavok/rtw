LOCAL_PATH := $(call my-dir)

###############################################################################
# rtw-test: librtw test tool
###############################################################################

include $(CLEAR_VARS)

LOCAL_MODULE := rtw-test
LOCAL_DESCRIPTION := librtw test tool
LOCAL_CATEGORY_PATH := tools

LOCAL_LIBRARIES := \
	librtw

LOCAL_SRC_FILES := \
	main.c

include $(BUILD_EXECUTABLE)
