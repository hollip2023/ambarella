##
## oryx/audio/device/make.inc
##
## History:
##    2014/12/15 - [Hanbo Xiao] Created file
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

LOCAL_PATH   := $(call my-dir)

ifeq ($(BUILD_AMBARELLA_ORYX_AUDIO_DEVICE), y)

include $(CLEAR_VARS)

AUDIO_DEVICE_VER_FILE   = $(ORYX_DIR)/include/audio/device/version.h
AUDIO_DEVICE_VER_PREFIX = ADEVICE

AUDIO_DEVICE_TARGET = libamaudio-device.so
LOCAL_TARGET := $(AUDIO_DEVICE_TARGET)
LOCAL_SRCS   := $(LOCAL_PATH)/am_audio_device.cpp
LOCAL_CFLAGS := -I$(ORYX_DIR)/include                        \
                -I$(ORYX_DIR)/include/utility                \
                -I$(ORYX_DIR)/include/audio/device          \
                -I$(ORYX_DIR)/audio/include                  \
                -I$(PREBUILD_3RD_PARTY_DIR)/alsa-lib/include \
                -std=c++11 -Werror

LOCAL_LIBS := libamutil.so

ifeq ($(BUILD_AMBARELLA_ORYX_AUDIO_DEVICE_ALSA), y)
LOCAL_LDFLAGS := -L$(PREBUILD_3RD_PARTY_DIR)/alsa-lib/usr/lib -lasound
endif

ifeq ($(BUILD_AMBARELLA_ORYX_AUDIO_DEVICE_PULSE), y)
LOCAL_SRCS += $(LOCAL_PATH)/am_audio_device_pulse.cpp
LOCAL_CFLAGS += -I$(PREBUILD_3RD_PARTY_DIR)/pulseaudio/include
LOCAL_LDFLAGS += -L$(PREBUILD_3RD_PARTY_DIR)/pulseaudio/usr/lib -lpulse
endif


AUDIO_DEVICE_SO_VER_MAJOR  := $(shell awk '/define\ $(AUDIO_DEVICE_VER_PREFIX)_LIB_MAJOR/{print $$3}' $(AUDIO_DEVICE_VER_FILE))
AUDIO_DEVICE_SO_VER_MINOR  := $(shell awk '/define\ $(AUDIO_DEVICE_VER_PREFIX)_LIB_MINOR/{print $$3}' $(AUDIO_DEVICE_VER_FILE))
AUDIO_DEVICE_SO_VER_PATCH  := $(shell awk '/define\ $(AUDIO_DEVICE_VER_PREFIX)_LIB_PATCH/{print $$3}' $(AUDIO_DEVICE_VER_FILE))
AUDIO_DEVICE_SO_VER_STRING := $(AUDIO_DEVICE_SO_VER_MAJOR).$(AUDIO_DEVICE_SO_VER_MINOR).$(AUDIO_DEVICE_SO_VER_PATCH)

AUDIO_DEVICE_SO_NAME   = $(patsubst %.so,%.so.$(AUDIO_DEVICE_SO_VER_MAJOR), $(AUDIO_DEVICE_TARGET))
AUDIO_DEVICE_SO_TARGET = $(patsubst %.so,%.so.$(AUDIO_DEVICE_SO_VER_STRING), $(AUDIO_DEVICE_TARGET))

LOCAL_SO_NAME := $(AUDIO_DEVICE_SO_NAME)

include $(BUILD_APP)

.PHONY: $(LOCAL_TARGET)

$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_LIB_DIR)/
	@cp -dpRf $< $(ORYX_LIB_DIR)/$(AUDIO_DEVICE_SO_TARGET)
	@ln -sf $(AUDIO_DEVICE_SO_TARGET) $(ORYX_LIB_DIR)/$(AUDIO_DEVICE_SO_NAME)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))

endif
