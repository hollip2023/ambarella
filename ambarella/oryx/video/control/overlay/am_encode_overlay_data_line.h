/*******************************************************************************
 * am_encode_overlay_data_line.cpp
 *
 * History:
 *   2016-12-6 - [Huaiqing Wang] created file
 *
 * Copyright (c) 2015 Ambarella, Inc.
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella, Inc. and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/


#ifndef AM_ENCODE_OVERLAY_DATA_LINE_H_
#define AM_ENCODE_OVERLAY_DATA_LINE_H_

#include "am_encode_overlay_data.h"

class AMOverlayLineData: public AMOverlayData
{
  public:
    static AMOverlayData* create(AMOverlayArea *area, AMOverlayAreaData *data);
    virtual void destroy();

  protected:
    AMOverlayLineData(AMOverlayArea *area);
    virtual ~AMOverlayLineData();

    virtual AM_RESULT add(AMOverlayAreaData *data);
    virtual AM_RESULT update(AMOverlayAreaData *data);
    virtual AM_RESULT blank();

  protected:
    AM_RESULT check_point_param(AMPoint &p, AMResolution &size);
    void draw_straight_line(AMPoint &p1, AMPoint &p2, uint8_t color,
                            int32_t thickness, const AMResolution &size);

};

#endif /* AM_ENCODE_OVERLAY_DATA_LINE_H_ */

