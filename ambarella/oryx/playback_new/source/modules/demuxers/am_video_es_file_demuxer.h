/*******************************************************************************
 * video_es_file_demuxer.h
 *
 * History:
 *    2016/04/12 - [Zhi He] create file
 *
 * Copyright (C) 2015 Ambarella, Inc.
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

#ifndef __VIDEO_ES_FILE_DEMUXER_H__
#define __VIDEO_ES_FILE_DEMUXER_H__

//-----------------------------------------------------------------------
//
// CVideoESFileDemuxer
//
//-----------------------------------------------------------------------

#define DMAX_MEDIA_DATA_BOX_NUMBER 2

class CVideoESFileDemuxer
  : public CActiveObject
  , public IDemuxer
  , public IEventListener
{
  typedef CActiveObject inherited;

public:
  static IDemuxer *Create(const TChar *pname, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink, TU32 index);
  virtual void Delete();

protected:
  CVideoESFileDemuxer(const TChar *pname, const volatile SPersistMediaConfig *pPersistMediaConfig, IMsgSink *pMsgSink, TU32 index);
  EECode Construct();
  virtual ~CVideoESFileDemuxer();

public:
  virtual CObject *GetObject0() const;

public:
  virtual EECode SetupOutput(COutputPin *p_output_pins[], CBufferPool *p_bufferpools[], IMemPool *p_mempools[], IMsgSink *p_msg_sink);

  virtual EECode SetupContext(TChar *url, void *p_agent = NULL, TU8 priority = 0, TU32 request_receive_buffer_size = 0, TU32 request_send_buffer_size = 0);
  virtual EECode DestroyContext();
  virtual EECode ReconnectServer();

  virtual EECode Seek(TTime &target_time, ENavigationPosition position = ENavigationPosition_Invalid);
  virtual EECode Start();
  virtual EECode Stop();

  virtual EECode Suspend();
  virtual EECode Pause();
  virtual EECode Resume();
  virtual EECode Flush();
  virtual EECode ResumeFlush();
  virtual EECode Purge();

  virtual EECode SetPbRule(TU8 direction, TU8 feeding_rule, TU16 speed);
  virtual EECode SetPbLoopMode(TU32 *p_loop_mode);

  virtual EECode EnableVideo(TU32 enable);
  virtual EECode EnableAudio(TU32 enable);

public:
  virtual EECode SetVideoPostProcessingCallback(void *callback_context, void *callback);
  virtual EECode SetAudioPostProcessingCallback(void *callback_context, void *callback);

public:
  virtual EECode QueryContentInfo(const SStreamCodecInfos *&pinfos) const;

public:
  virtual EECode UpdateContext(SContextInfo *pContext);
  virtual EECode GetExtraData(SStreamingSessionContent *pContent);

public:
  virtual EECode NavigationSeek(SContextInfo *info);
  virtual EECode ResumeChannel();

public:
  virtual void PrintStatus();

public:
  virtual void EventNotify(EEventType type, TU64 param1, TPointer param2);

public:
  virtual void OnRun();

private:
  EECode getMediaInfo();
  EECode sendExtraData();
  EECode processCmd(SCMD &cmd);
  void sendEOSBuffer();

private:
  const volatile SPersistMediaConfig *mpPersistMediaConfig;
  IMsgSink *mpMsgSink;

  COutputPin *mpOutputPins[EConstMaxDemuxerMuxerStreamNumber];
  CBufferPool *mpBufferPool[EConstMaxDemuxerMuxerStreamNumber];
  IMemPool *mpMemPool[EConstMaxDemuxerMuxerStreamNumber];

  COutputPin *mpCurOutputPin;
  IBufferPool *mpCurBufferPool;
  IMemPool *mpCurMemPool;

  CIBuffer mPersistVideoEOSBuffer;

private:
  TU8 mbRun;
  TU8 mbPaused;
  TU8 mbTobeStopped;
  TU8 mbTobeSuspended;

  EModuleState msState;

private:
  StreamFormat mVideoFormat;

private:
  IMediaFileParser *mpMediaParser;

private:
  TU32 mVideoWidth;
  TU32 mVideoHeight;
  TU8 *mpExtradata;
  TU32 mExtradataLen;

  TU32 mVideoTimeScale;
  TU32 mVideoFrameTick;

  TTime mCurrentTimestamp;
};

#endif


