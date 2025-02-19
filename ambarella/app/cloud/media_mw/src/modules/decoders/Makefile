###############################################################################
## $(MODULE_NAME_TAG)/media_mw/src/modules/decoders/Makefile
##
## History:
##    2013/04/15 - [Zhi He] Create File
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
##################################################################################

default: all

include ../../../../build/core/linux/pre_requirement.mk

include $(BUILDSYSTEM_DIR)/$(MODULE_NAME_TAG)/build/core/linux/config_linux.mk

ifeq ($(CONFIG_ARCH_I1), y)
include $(BUILDSYSTEM_DIR)/$(MODULE_NAME_TAG)/build/core/linux/platform_i1_linux.mk
endif

ifeq ($(CONFIG_ARCH_S2), y)
include $(BUILDSYSTEM_DIR)/$(MODULE_NAME_TAG)/build/core/linux/platform_s2_linux.mk
endif

include $(BUILDSYSTEM_DIR)/$(MODULE_NAME_TAG)/build/core/linux/extern_lib.mk

MODULE_INC = -I$(BUILDSYSTEM_DIR)/$(MODULE_NAME_TAG)/media_mw/src/internal_include 
MODULE_INC += $(EXTERN_LIB_FFMPEG_INC)
MODULE_INC += $(EXTERN_LIB_LIBAAC_INC)
MODULE_INC += $(EXTERN_BUILD_INC)

MODULE_SRC := $(wildcard *.cpp)

ifeq ($(BUILD_CONFIG_MODULE_LIBAAC), n)
MODULE_LIBAAC_DECODER_SRC = aac_audio_decoder.cpp
MODULE_SRC := $(filter-out $(MODULE_LIBAAC_DECODER_SRC),$(MODULE_SRC))
endif

ifeq ($(BUILD_CONFIG_MODULE_FFMPEG), n)
MODULE_AUDIO_FFMPEG_DECODER_SRC = ffmpeg_audio_decoder.cpp ffmpeg_video_decoder.cpp
MODULE_SRC := $(filter-out $(MODULE_AUDIO_FFMPEG_DECODER_SRC),$(MODULE_SRC))
endif

ifeq ($(BUILD_CONFIG_MODULE_AMBA_DSP), n)
MODULE_AMBA_DECODER_SRC = amba_video_decoder.cpp
MODULE_SRC := $(filter-out $(MODULE_AMBA_DECODER_SRC),$(MODULE_SRC))
endif

MODULE_OBJ := $(MODULE_SRC:.cpp=.o)
MODULE_DEF = $(V_COMPILE_CFLAGS)

media_mw_modules_decoders_src		=	$(MODULE_SRC)
media_mw_modules_decoders_obj		=	$(media_mw_modules_decoders_src:.cpp=.o)

include $(BUILDSYSTEM_DIR)/$(MODULE_NAME_TAG)/build/core/linux/rules_linux.mk



