LOCAL_PATH := $(call my-dir)

###############################################################################
# librtw: Reinventing the wheel library
###############################################################################

include $(CLEAR_VARS)

LOCAL_MODULE := librtw
LOCAL_DESCRIPTION := Reinventing the wheel library
LOCAL_CATEGORY_PATH := tools

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include \

LOCAL_SRC_FILES := \
	src/rtw_tracer.c \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/

include $(BUILD_LIBRARY)
