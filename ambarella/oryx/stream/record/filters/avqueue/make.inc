##
## /S2l/oryx/stream/record/filters/avqueue/make.inc
##
## History:
##   2014-12-30 - [Shupeng Ren] created file
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

include $(CLEAR_VARS)

AVQUEUE_SRC_TARGET = filter-avqueue.so
AVQUEUE_SRC_CONF   = filter-avqueue.acs

LOCAL_TARGET := $(AVQUEUE_SRC_TARGET)
LOCAL_SRCS   := $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_CFLAGS := -I$(LOCAL_PATH)                           \
                -I$(ORYX_DIR)/include                     \
                -I$(ORYX_DIR)/include/video               \
                -I$(ORYX_DIR)/include/audio               \
                -I$(ORYX_DIR)/include/configure           \
                -I$(ORYX_DIR)/include/utility             \
                -I$(ORYX_DIR)/include/stream              \
                -I$(ORYX_DIR)/stream/include/amf          \
                -I$(ORYX_DIR)/stream/include/common       \
                -I$(ORYX_DIR)/stream/include/common/media \
                -I$(ORYX_DIR)/stream/include/record       \
                -std=c++11 -Werror

LOCAL_LIBS := libamutil.so              \
              libamosal.so              \
              libamconfig.so            \
              libamstream-iid-record.so \
              libamstream-amf.so

AVQUEUE_SRC_SO_NAME   = $(AVQUEUE_SRC_TARGET)
AVQUEUE_SRC_SO_TARGET = $(AVQUEUE_SRC_TARGET)

LOCAL_SO_NAME := $(AVQUEUE_SRC_SO_NAME)

include $(BUILD_APP)

.PHONY: $(AVQUEUE_SRC_TARGET)

$(LOCAL_TARGET): AVQUEUE_SRC_PATH := $(LOCAL_PATH)
$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_FILTER_DIR)/
	@mkdir -p $(ORYX_CONF_DIR)/stream/filter/
	@cp -dpRf $(AVQUEUE_SRC_PATH)/$(AVQUEUE_SRC_CONF) $(ORYX_CONF_DIR)/stream/filter/$(AVQUEUE_SRC_CONF)
	@cp -dpRf $< $(ORYX_FILTER_DIR)/$(AVQUEUE_SRC_SO_TARGET)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))
