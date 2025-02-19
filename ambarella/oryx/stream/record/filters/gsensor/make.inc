##
## /S2l/oryx/stream/record/filters/gsensor/make.inc
##
## History:
##   2016-11-28 - [ccjing] created file
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

GSENSOR_SRC_TARGET = filter-gsensor-source.so
GSENSOR_SRC_CONF   = filter-gsensor-source.acs

LOCAL_TARGET := $(GSENSOR_SRC_TARGET)
LOCAL_SRCS   := $(LOCAL_PATH)/am_gsensor_source_config.cpp \
                $(LOCAL_PATH)/am_gsensor_source.cpp  \
                $(LOCAL_PATH)/am_gsensor_reader.cpp

LOCAL_CFLAGS := -I$(LOCAL_PATH)                     \
                -I$(ORYX_DIR)/include               \
                -I$(ORYX_DIR)/include/configure     \
                -I$(ORYX_DIR)/include/stream        \
                -I$(ORYX_DIR)/include/utility       \
                -I$(ORYX_DIR)/stream/include/amf    \
                -I$(ORYX_DIR)/stream/include/common \
                -I$(ORYX_DIR)/stream/include/record

LOCAL_CFLAGS += -std=c++11 -Werror

LOCAL_LIBS := libamutil.so              \
              libamosal.so              \
              libamconfig.so            \
              libamstream-iid-record.so \
              libamstream-amf.so

GSENSOR_SRC_SO_NAME   = $(GSENSOR_SRC_TARGET)
GSENSOR_SRC_SO_TARGET = $(GSENSOR_SRC_TARGET)

LOCAL_SO_NAME := $(GSENSOR_SRC_SO_NAME)

include $(BUILD_APP)

.PHONY: $(GSENSOR_SRC_TARGET)

$(LOCAL_TARGET): GSENSOR_SRC_PATH := $(LOCAL_PATH)
$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_FILTER_DIR)/
	@mkdir -p $(ORYX_CONF_DIR)/stream/filter/
	@cp -dpRf $(GSENSOR_SRC_PATH)/$(GSENSOR_SRC_CONF) $(ORYX_CONF_DIR)/stream/filter/$(GSENSOR_SRC_CONF)
	@cp -dpRf $< $(ORYX_FILTER_DIR)/$(GSENSOR_SRC_SO_TARGET)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))
