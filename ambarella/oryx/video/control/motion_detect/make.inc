##
##/oryx/video/control/motion_detect/make.inc
##
## History:
##   May 3, 2016 - [binwang] created file
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
##

ifeq ($(BUILD_AMBARELLA_VIDEO_PLUGIN_MD), y)

MD_PLUGIN_PATH := $(call my-dir)
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
VPLUGIN_MD = video-motion-detect.so
VPLUGIN_MD_CONF = $(MD_PLUGIN_PATH)/motion-detect.acs

MDET_LIB_PATH := $(AMB_TOPDIR)/prebuild/ambarella/library/smartcam/mdet

LOCAL_TARGET := $(VPLUGIN_MD)
LOCAL_SRCS   := $(wildcard $(MD_PLUGIN_PATH)/*.cpp)

LOCAL_CFLAGS := -std=c++11 -Werror              \
                -I$(ORYX_DIR)/include           \
                -I$(ORYX_DIR)/include/utility   \
                -I$(ORYX_DIR)/include/video     \
                -I$(ORYX_DIR)/include/configure \
                -I$(ORYX_DIR)/video/include     \
                -I$(WARP_PLUGIN_PATH)/          \
                -I$(MDET_LIB_PATH)/include      \
                -I$(AMB_TOPDIR)/packages/utils

LOCAL_LIBS := libamvideo-reader.so libamvideo-address.so

ifeq ($(BUILD_AMBARELLA_PACKAGES_PROPRIETARY_SOURCES), y)
LOCAL_LIBS += libmdet.so
else
LOCAL_LDFLAGS += -L$(MDET_LIB_PATH)/lib/arch_$(AMBARELLA_ARCH) -lmdet
endif

VPLUGIN_MD_SO_NAME   = $(VPLUGIN_MD).$(AMBARELLA_ARCH)
VPLUGIN_MD_SO_TARGET = $(VPLUGIN_MD)
LOCAL_SO_NAME := $(VPLUGIN_MD_SO_NAME)

include $(BUILD_APP)

.PHONY: $(VPLUGIN_MD)

$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(FAKEROOT_DIR)/$(BUILD_AMBARELLA_ORYX_CONF_DIR)/video
	@cp -dpRf $(VPLUGIN_MD_CONF) $(FAKEROOT_DIR)/$(BUILD_AMBARELLA_ORYX_CONF_DIR)/video
	@mkdir -p $(ORYX_VIDEO_PLUGIN_DIR)
	@cp -dpRf $< $(ORYX_VIDEO_PLUGIN_DIR)/$(VPLUGIN_MD_SO_TARGET)

$(call add-target-into-build, $(LOCAL_TARGET))

endif
