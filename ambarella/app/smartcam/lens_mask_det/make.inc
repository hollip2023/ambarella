##
## make.inc
##
## History:
##    2016/11/7 - [Richard Ren] Created file
##
## Copyright (c) 2016 Ambarella, Inc.
##
## This file and its contents ( "Software" ) are protected by intellectual
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


ifeq ($(BUILD_AMBARELLA_SMARTCAM_APP_LENS_MASK_DET), y)
LOCAL_PATH	:= $(call my-dir)
UTILS_PATH	:= $(LOCAL_PATH)/../utils

###
include $(CLEAR_VARS)

LOCAL_TARGET    := test_lens_mask_det
LOCAL_SRCS      := $(LOCAL_PATH)/test_lens_mask_det.c $(UTILS_PATH)/arch_$(AMBARELLA_ARCH)/iav.c $(UTILS_PATH)/arch_$(AMBARELLA_ARCH)/fb.c
LOCAL_CFLAGS	:= -I$(UTILS_PATH) -I$(AMB_TOPDIR)/prebuild/ambarella/library/smartcam/lens_mask_det/include
LOCAL_LDFLAGS   := -L$(AMB_TOPDIR)/prebuild/ambarella/library/smartcam/lens_mask_det/lib/arch_$(AMBARELLA_ARCH) -lm -llens_mask_det

include $(BUILD_APP)

.PHONY: $(LOCAL_TARGET)

$(LOCAL_TARGET): PRIVATE_PATH:=$(LOCAL_PATH)
$(LOCAL_TARGET): $(LOCAL_MODULE)
	@mkdir -p $(APP_INSTALL_PATH)/
	@cp -dpRf $< $(APP_INSTALL_PATH)/
	@chmod +x $(APP_INSTALL_PATH)/$@
	@echo "Build $@ Done."

$(call add-target-into-build, $(LOCAL_TARGET))

endif
