##
## oryx/services/media_service/make.inc
##
## History:
##    2014/10/21 - [Louis Sun] Created file
##
## Copyright (c) 2016 Ambarella, Inc.
##
## This file and its contents ("Software") are protected by intellectual
## property rights including, without limitation, U.S. and/or foreign
## copyrights. This Software is also the confidential and proprietary
## information of Ambarella, Inc. and its licensors. You may not use, reproduce,
## disclose, distribute, modify, or otherwise prepare derivative works of this
## Software or any portion thereof except pursuant to a signed license agreement
## or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
## In the absence of such an agreement, you agree to promptly notify and return
## this Software to Ambarella, Inc.
##
## THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
## INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
## MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
## IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
## INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
## (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
## LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
##


ifeq ($(BUILD_AMBARELLA_ORYX_SERVICE_MEDIA), y)
LOCAL_PATH   := $(call my-dir)
include $(CLEAR_VARS)

###############################################################################
MEDIA_SVC_NAME =media_svc
LOCAL_TARGET := $(MEDIA_SVC_NAME)
LOCAL_SRCS   := $(wildcard $(LOCAL_PATH)/am_media_service_main.cpp     \
                           $(LOCAL_PATH)/am_media_service_instance.cpp \
                           $(LOCAL_PATH)/am_media_service_msg_action.cpp)
LOCAL_CFLAGS :=  -I$(ORYX_DIR)/include                    \
                 -I$(ORYX_DIR)/include/audio              \
                 -I$(ORYX_DIR)/include/utility            \
                 -I$(ORYX_DIR)/include/ipc                \
                 -I$(ORYX_DIR)/include/services           \
                 -I$(ORYX_DIR)/include/image_quality      \
                 -I$(ORYX_DIR)/include/services/commands  \
                 -I$(ORYX_DIR)/services/include           \
                 -I$(ORYX_DIR)/include/stream             \
                 -I$(ORYX_DIR)/stream/include/record      \
                 -I$(LOCAL_PATH)                          \
                 -std=c++11 -Werror

LOCAL_LDFLAGS := -Wl,-rpath-link=$(FAKEROOT_DIR)/usr/lib
LOCAL_LIBS    := libamutil.so            \
                 libamosal.so            \
                 libamipc.so             \
                 libamservice-frame.so   \
                 libamstream-amf.so
ifeq ($(BUILD_AMBARELLA_ORYX_MEDIA_SERVICE_RECORD), y)
LOCAL_LIBS    += libamstream-record.so
endif
ifeq ($(BUILD_AMBARELLA_ORYX_MEDIA_SERVICE_PLAYBACK), y)
LOCAL_LIBS    += libamstream-playback.so
endif

include $(BUILD_APP)

.PHONY: $(LOCAL_TARGET)

$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_BIN_DIR)/
	@cp -dpRf $< $(ORYX_BIN_DIR)/
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))

### libammedia-data-struct.so
include $(CLEAR_VARS)

MEDIA_DATA_STRUCT = libammedia-data-struct.so

LOCAL_TARGET := $(MEDIA_DATA_STRUCT)
LOCAL_SRCS   := $(wildcard $(LOCAL_PATH)/am_media_service_data_structure.cpp)

LOCAL_CFLAGS := -I$(ORYX_DIR)/include                 \
                -I$(ORYX_DIR)/include/audio           \
                -I$(ORYX_DIR)/include/utility         \
                -I$(ORYX_DIR)/include/services        \
                -I$(ORYX_DIR)/include/stream          \
                -I$(ORYX_DIR)/include/service/commands\
                -I$(ORYX_DIR)/stream/include/record   \
                -I$(ORYX_DIR)/include/ipc             \
                -std=c++11 -Werror

LOCAL_LIBS := libamutil.so

MEDIA_DATA_STRUCT_NAME   = $(MEDIA_DATA_STRUCT)
MEDIA_DATA_STRUCT_TARGET = $(MEDIA_DATA_STRUCT)
LOCAL_SO_NAME := $(MEDIA_DATA_STRUCT)

include $(BUILD_APP)
.PHONY: $(LOCAL_TARGET)

$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_LIB_DIR)/
	@cp -dpRf $< $(ORYX_LIB_DIR)/$(MEDIA_DATA_STRUCT_TARGET)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))

endif
