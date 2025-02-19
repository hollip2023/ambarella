/*
 *
 * Copyright (c) 2004-2010 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
 *
 */

#ifndef __AGGR_RECV_API_H__
#define __AGGR_RECV_API_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (* RX_CALLBACK)(void * dev, void *osbuf);

typedef void (* ALLOC_NETBUFS)(A_NETBUF_QUEUE_T *q, A_UINT16 num);

/*
 * aggr_init:
 * Initialises the data structures, allocates data queues and 
 * os buffers. Netbuf allocator is the input param, used by the
 * aggr module for allocation of NETBUFs from driver context.
 * These NETBUFs are used for AMSDU processing.
 *
 * Also registers OS call back function to deliver the
 * frames to OS. This is generally the topmost layer of
 * the driver context, after which the frames go to
 * IP stack via the call back function.
 * This dispatcher is active only when aggregation is ON.
 * Returns A_OK if init success, else returns A_ERROR
 */
A_UINT8
aggr_init(ALLOC_NETBUFS netbuf_allocator, RX_CALLBACK fn);

/*
 * aggr_init_conn:
 * Initialises the data structures, allocates data queues and 
 * os buffers. Returns the context for a single conn aggr.
 * For each supported conn, this API should be called.
 */
void *
aggr_init_conn(void);

/*
 * aggr_process_bar:
 * When target receives BAR, it communicates to host driver
 * for modifying window parameters. Target indicates this via the 
 * event: WMI_ADDBA_REQ_EVENTID. Host will dequeue all frames
 * up to the indicated sequence number.
 */
void
aggr_process_bar(void *cntxt, A_UINT8 tid, A_UINT16 seq_no);


/*
 * aggr_recv_addba_req_evt:
 * This event is to initiate/modify the receive side window.
 * Target will send WMI_ADDBA_REQ_EVENTID event to host - to setup 
 * recv re-ordering queues. Target will negotiate ADDBA with peer, 
 * and indicate via this event after succesfully completing the 
 * negotiation. This happens in two situations:
 *  1. Initial setup of aggregation
 *  2. Renegotiation of current recv window.
 * Window size for re-ordering is limited by target buffer
 * space, which is reflected in win_sz.
 * (Re)Start the periodic timer to deliver long standing frames,
 * in hold_q to OS.
 */
void
aggr_recv_addba_req_evt(void * cntxt, A_UINT8 tid, A_UINT16 seq_no, A_UINT8 win_sz);


/*
 * aggr_recv_delba_req_evt:
 * Target indicates deletion of a BA window for a tid via the
 * WMI_DELBA_EVENTID. Host would deliver all the frames in the 
 * hold_q, reset tid config and disable the periodic timer, if 
 * aggr is not enabled on any tid.
 */
void
aggr_recv_delba_req_evt(void * cntxt, A_UINT8 tid);



/*
 * aggr_process_recv_frm:
 * Called only for data frames. When aggr is ON for a tid, the buffer 
 * is always consumed, and osbuf would be NULL. For a non-aggr case,
 * osbuf is not modified.
 * AMSDU frames are consumed and are later freed. They are sliced and 
 * diced to individual frames and dispatched to stack.
 * After consuming a osbuf(when aggr is ON), a previously registered
 * callback may be called to deliver frames in order.
 */
void
aggr_process_recv_frm(void *cntxt, A_UINT8 tid, A_UINT16 seq_no, A_BOOL is_amsdu, void **osbuf);


/*
 * aggr_module_destroy:
 * Frees up all the queues and frames in them.
 */
void
aggr_module_destroy(void);

/*
 *  * aggr_module_destroy_timers:
 *   * Disarm the timers.
 *    */
void
aggr_module_destroy_timers(void *cntxt);

/*
 * aggr_module_destroy_conn:
 * Frees up all the queues and frames in them. Releases the cntxt to OS.
 */
void
aggr_module_destroy_conn(void *cntxt);

/*
 * Dumps the aggregation stats 
 */
void
aggr_dump_stats(void *cntxt, PACKET_LOG **log_buf);

/* 
 * aggr_reset_state -- Called when it is deemed necessary to clear the aggregate
 *  hold Q state.  Examples include when a Connect event or disconnect event is 
 *  received. 
 */
void
aggr_reset_state(void *cntxt, void *dev);


#ifdef __cplusplus
}
#endif

#endif /*__AGGR_RECV_API_H__ */
