/*******************************************************************************
 * rtp_rtcp_sink_pcm.h
 *
 * History:
 *    2013/11/07 - [Zhi He] create file
 *
 * Copyright (c) 2016 Ambarella, Inc.
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

#ifndef __RTP_RTCP_SINK_PCM__
#define __RTP_RTCP_SINK_PCM__

DCONFIG_COMPILE_OPTION_HEADERFILE_BEGIN
DCODE_DELIMITER;

//-----------------------------------------------------------------------
//
// CPCMRTPRTCPSink
//
//-----------------------------------------------------------------------
class CPCMRTPRTCPSink: public CObject, public IStreamingTransmiter
{
    typedef CObject inherited;

public:
    static IStreamingTransmiter *Create(const TChar *pname, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink);
    virtual void Destroy();
    virtual void Delete();

protected:
    CPCMRTPRTCPSink(const TChar *pname, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink);
    EECode Construct();
    virtual ~CPCMRTPRTCPSink();

public:
    virtual EECode SetupContext(StreamTransportType type, ProtocolType protocol_type, const SStreamCodecInfos *infos, TTime start_time = 0);
    virtual EECode DestroyContext();
    virtual EECode UpdateStreamFormat(StreamType type, StreamFormat format);

    virtual EECode Start();
    virtual EECode Stop();

    virtual EECode SetExtraData(TU8 *pdata, TMemSize size);
    virtual EECode SendData(CIBuffer *pBuffer);

    virtual EECode AddSubSession(SSubSessionInfo *p_sub_session);
    virtual EECode RemoveSubSession(SSubSessionInfo *p_sub_session);
    virtual EECode SetSrcPort(TSocketPort port, TSocketPort port_ext);
    virtual EECode GetSrcPort(TSocketPort &port, TSocketPort &port_ext);

protected:
    void udpSendData(TU8 *p_data, TMemSize size, TTime pts);
    EECode setupUDPSocket();

private:
    StreamTransportType mStreamingType;
    ProtocolType mProtocolType;
    StreamFormat mFormat;
    StreamType mType;
    CIClockReference   *mpClockReference;
    IMutex *mpMutex;

private:
    const volatile SPersistMediaConfig *mpConfig;
    IMsgSink *mpMsgSink;

private:
    CIDoubleLinkedList mSubSessionList;
    TU8 mbRTPSocketSetup;
    TU8 mPayloadType;
    TU8 mReserved0[2];

private:
    TSocketHandler mRTPSocket;
    TSocketHandler mRTCPSocket;
    TSocketPort mRTPPort;
    TSocketPort mRTCPPort;

private:
    TU8 *mpRTPBuffer;
    TUint mRTPBufferLength;
    TUint mRTPBufferTotalLength;

private:
    TU8 *mpExtraData;
    TMemSize mExtraDataSize;
};

DCONFIG_COMPILE_OPTION_HEADERFILE_END

#endif

