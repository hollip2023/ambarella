##
## /S2E/oryx/video/address/make.inc
##
## History:
##   Sep 11, 2015 - [ypchang] created file
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

LOCAL_PATH := $(call my-dir)
ADDRESS_PATH := $(call my-dir)

include $(CLEAR_VARS)
ADDRESS_VER_FILE   = $(ORYX_DIR)/include/video/version.h
ADDRESS_VER_PREFIX = ADDRESS

ADDRESS_TARGET := libamvideo-address.so

LOCAL_TARGET := $(ADDRESS_TARGET)
LOCAL_SRCS   := $(wildcard $(ADDRESS_PATH)/*.cpp)

LOCAL_CFLAGS := -I$(ORYX_DIR)/include           \
                -I$(ORYX_DIR)/include/utility   \
                -I$(ORYX_DIR)/include/video     \
                -I$(ORYX_DIR)/include/configure \
                -I$(ADDRESS_PATH)/../include    \
                -std=c++11 -Werror

LOCAL_LIBS := libamvideo-platform.so

ADDRESS_SO_VER_MAJOR  := $(shell awk '/define\ $(ADDRESS_VER_PREFIX)_LIB_MAJOR/{print $$3}' $(ADDRESS_VER_FILE))
ADDRESS_SO_VER_MINOR  := $(shell awk '/define\ $(ADDRESS_VER_PREFIX)_LIB_MINOR/{print $$3}' $(ADDRESS_VER_FILE))
ADDRESS_SO_VER_PATCH  := $(shell awk '/define\ $(ADDRESS_VER_PREFIX)_LIB_PATCH/{print $$3}' $(ADDRESS_VER_FILE))
ADDRESS_SO_VER_STRING := $(ADDRESS_SO_VER_MAJOR).$(ADDRESS_SO_VER_MINOR).$(ADDRESS_SO_VER_PATCH)

ADDRESS_SO_NAME   = $(patsubst %.so,%.so.$(ADDRESS_SO_VER_MAJOR), $(ADDRESS_TARGET))
ADDRESS_SO_TARGET = $(patsubst %.so,%.so.$(ADDRESS_SO_VER_STRING), $(ADDRESS_TARGET))

LOCAL_SO_NAME := $(ADDRESS_SO_NAME)

include $(BUILD_APP)

.PHONY: $(ADDRESS_TARGET)

$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_LIB_DIR)/
	@cp -dpRf $< $(ORYX_LIB_DIR)/$(ADDRESS_SO_TARGET)
	@ln -sf $(ADDRESS_SO_TARGET) $(ORYX_LIB_DIR)/$(ADDRESS_SO_NAME)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))