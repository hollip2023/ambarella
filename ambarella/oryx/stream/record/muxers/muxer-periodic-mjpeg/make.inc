##
## /S2l/oryx/stream/record/muxers/muxer-periodic-jpeg/make.inc
##
## History:
##   2016-04-22 - [ccjing] created file
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

### muxer-periodic-mjpeg.so
include $(CLEAR_VARS)

MUXER_PERIODIC_MJPEG = muxer-periodic-mjpeg.so
MUXER_PERIODIC_MJPEG_CONF = muxer-periodic-mjpeg.acs
LOCAL_TARGET := $(MUXER_PERIODIC_MJPEG)
LOCAL_SRCS   := $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_CFLAGS := -std=c++11 -Werror                        \
                -I$(LOCAL_PATH)                           \
                -I$(ORYX_DIR)/include                     \
                -I$(ORYX_DIR)/include/stream              \
                -I$(ORYX_DIR)/include/configure           \
                -I$(ORYX_DIR)/include/utility             \
                -I$(ORYX_DIR)/include/video               \
                -I$(ORYX_DIR)/include/stream              \
                -I$(ORYX_DIR)/stream/include/amf          \
                -I$(ORYX_DIR)/stream/include/common       \
                -I$(ORYX_DIR)/stream/include/common/io    \
                -I$(ORYX_DIR)/stream/include/stream       \
                -I$(ORYX_DIR)/stream/include/record       \
                -I$(ORYX_DIR)/stream/include/common/media

LOCAL_LDFLAGS := -Wl,-rpath=$(patsubst $(FAKEROOT_DIR)/%,%,$(ORYX_MUXER_DIR))

LOCAL_LIBS := libamutil.so      \
              libamosal.so      \
              libamfile-sink.so

ifeq ($(CONFIG_AMBARELLA_ORYX_STREAM_RECORD_MUXER_ORIG), y)
LOCAL_LIBS += libammuxer-codec-iid.so
endif

ifeq ($(CONFIG_AMBARELLA_ORYX_STREAM_RECORD_MUXER_ACTIVE), y)
LOCAL_LIBS += libamactivemuxer-codec-iid.so
endif

MUXER_PERIODIC_MJPEG_NAME = $(MUXER_PERIODIC_MJPEG)
MUXER_PERIODIC_MJPEG_TARGET = $(MUXER_PERIODIC_MJPEG)
LOCAL_SO_NAME := $(MUXER_PERIODIC_MJPEG)

include $(BUILD_APP)
.PHONY: $(LOCAL_TARGET)

$(LOCAL_TARGET): MUXER_PERIODIC_MJPEG_SRC_PATH := $(LOCAL_PATH)
$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(ORYX_MUXER_DIR)/
	@mkdir -p $(ORYX_CONF_DIR)/stream/muxer
	@cp -dpRf $(MUXER_PERIODIC_MJPEG_SRC_PATH)/$(MUXER_PERIODIC_MJPEG_CONF) $(ORYX_CONF_DIR)/stream/muxer/$(MUXER_PERIODIC_MJPEG_CONF)
	@cp -dpRf $< $(ORYX_MUXER_DIR)/$(MUXER_PERIODIC_MJPEG_TARGET)
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))
