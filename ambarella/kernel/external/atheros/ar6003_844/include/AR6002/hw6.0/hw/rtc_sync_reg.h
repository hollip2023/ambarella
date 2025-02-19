// ------------------------------------------------------------------
// Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
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
// 	Wifi driver for AR6003
// </summary>
//
// ------------------------------------------------------------------
//===================================================================
// Author(s): ="Atheros"
//===================================================================

/* Copyright (C) 2010 Denali Software Inc.  All rights reserved              */
/* THIS FILE IS AUTOMATICALLY GENERATED BY DENALI BLUEPRINT, DO NOT EDIT     */


#ifndef _RTC_SYNC_REG_REG_H_
#define _RTC_SYNC_REG_REG_H_


/* macros for RTC_SYNC_RESET */
#define RTC_SYNC_RESET_ADDRESS                                                            0x00000240
#define RTC_SYNC_RESET_OFFSET                                                             0x00000240
#define RTC_SYNC_RESET_RESET_L_MSB                                                                 0
#define RTC_SYNC_RESET_RESET_L_LSB                                                                 0
#define RTC_SYNC_RESET_RESET_L_MASK                                                       0x00000001
#define RTC_SYNC_RESET_RESET_L_GET(x)                                      (((x) & 0x00000001) >> 0)
#define RTC_SYNC_RESET_RESET_L_SET(x)                                      (((x) << 0) & 0x00000001)

/* macros for RTC_SYNC_STATUS */
#define RTC_SYNC_STATUS_ADDRESS                                                           0x00000244
#define RTC_SYNC_STATUS_OFFSET                                                            0x00000244
#define RTC_SYNC_STATUS_SHUTDOWN_STATE_MSB                                                         0
#define RTC_SYNC_STATUS_SHUTDOWN_STATE_LSB                                                         0
#define RTC_SYNC_STATUS_SHUTDOWN_STATE_MASK                                               0x00000001
#define RTC_SYNC_STATUS_SHUTDOWN_STATE_GET(x)                              (((x) & 0x00000001) >> 0)
#define RTC_SYNC_STATUS_ON_STATE_MSB                                                               1
#define RTC_SYNC_STATUS_ON_STATE_LSB                                                               1
#define RTC_SYNC_STATUS_ON_STATE_MASK                                                     0x00000002
#define RTC_SYNC_STATUS_ON_STATE_GET(x)                                    (((x) & 0x00000002) >> 1)
#define RTC_SYNC_STATUS_SLEEP_STATE_MSB                                                            2
#define RTC_SYNC_STATUS_SLEEP_STATE_LSB                                                            2
#define RTC_SYNC_STATUS_SLEEP_STATE_MASK                                                  0x00000004
#define RTC_SYNC_STATUS_SLEEP_STATE_GET(x)                                 (((x) & 0x00000004) >> 2)
#define RTC_SYNC_STATUS_WAKEUP_STATE_MSB                                                           3
#define RTC_SYNC_STATUS_WAKEUP_STATE_LSB                                                           3
#define RTC_SYNC_STATUS_WAKEUP_STATE_MASK                                                 0x00000008
#define RTC_SYNC_STATUS_WAKEUP_STATE_GET(x)                                (((x) & 0x00000008) >> 3)
#define RTC_SYNC_STATUS_WRESET_MSB                                                                 4
#define RTC_SYNC_STATUS_WRESET_LSB                                                                 4
#define RTC_SYNC_STATUS_WRESET_MASK                                                       0x00000010
#define RTC_SYNC_STATUS_WRESET_GET(x)                                      (((x) & 0x00000010) >> 4)
#define RTC_SYNC_STATUS_PLL_CHANGING_MSB                                                           5
#define RTC_SYNC_STATUS_PLL_CHANGING_LSB                                                           5
#define RTC_SYNC_STATUS_PLL_CHANGING_MASK                                                 0x00000020
#define RTC_SYNC_STATUS_PLL_CHANGING_GET(x)                                (((x) & 0x00000020) >> 5)

/* macros for RTC_SYNC_DERIVED */
#define RTC_SYNC_DERIVED_ADDRESS                                                          0x00000248
#define RTC_SYNC_DERIVED_OFFSET                                                           0x00000248
#define RTC_SYNC_DERIVED_BYPASS_MSB                                                                0
#define RTC_SYNC_DERIVED_BYPASS_LSB                                                                0
#define RTC_SYNC_DERIVED_BYPASS_MASK                                                      0x00000001
#define RTC_SYNC_DERIVED_BYPASS_GET(x)                                     (((x) & 0x00000001) >> 0)
#define RTC_SYNC_DERIVED_BYPASS_SET(x)                                     (((x) << 0) & 0x00000001)
#define RTC_SYNC_DERIVED_FORCE_MSB                                                                 1
#define RTC_SYNC_DERIVED_FORCE_LSB                                                                 1
#define RTC_SYNC_DERIVED_FORCE_MASK                                                       0x00000002
#define RTC_SYNC_DERIVED_FORCE_GET(x)                                      (((x) & 0x00000002) >> 1)
#define RTC_SYNC_DERIVED_FORCE_SET(x)                                      (((x) << 1) & 0x00000002)
#define RTC_SYNC_DERIVED_FORCE_SWREG_PWD_MSB                                                       2
#define RTC_SYNC_DERIVED_FORCE_SWREG_PWD_LSB                                                       2
#define RTC_SYNC_DERIVED_FORCE_SWREG_PWD_MASK                                             0x00000004
#define RTC_SYNC_DERIVED_FORCE_SWREG_PWD_SET(x)                            (((x) << 2) & 0x00000004)
#define RTC_SYNC_DERIVED_FORCE_LPO_PWD_MSB                                                         3
#define RTC_SYNC_DERIVED_FORCE_LPO_PWD_LSB                                                         3
#define RTC_SYNC_DERIVED_FORCE_LPO_PWD_MASK                                               0x00000008
#define RTC_SYNC_DERIVED_FORCE_LPO_PWD_SET(x)                              (((x) << 3) & 0x00000008)

/* macros for RTC_SYNC_FORCE_WAKE */
#define RTC_SYNC_FORCE_WAKE_ADDRESS                                                       0x0000024c
#define RTC_SYNC_FORCE_WAKE_OFFSET                                                        0x0000024c
#define RTC_SYNC_FORCE_WAKE_ENABLE_MSB                                                             0
#define RTC_SYNC_FORCE_WAKE_ENABLE_LSB                                                             0
#define RTC_SYNC_FORCE_WAKE_ENABLE_MASK                                                   0x00000001
#define RTC_SYNC_FORCE_WAKE_ENABLE_GET(x)                                  (((x) & 0x00000001) >> 0)
#define RTC_SYNC_FORCE_WAKE_INTR_MSB                                                               1
#define RTC_SYNC_FORCE_WAKE_INTR_LSB                                                               1
#define RTC_SYNC_FORCE_WAKE_INTR_MASK                                                     0x00000002
#define RTC_SYNC_FORCE_WAKE_INTR_GET(x)                                    (((x) & 0x00000002) >> 1)
#define RTC_SYNC_FORCE_WAKE_INTR_SET(x)                                    (((x) << 1) & 0x00000002)

/* macros for RTC_SYNC_INTR_CAUSE */
#define RTC_SYNC_INTR_CAUSE_ADDRESS                                                       0x00000250
#define RTC_SYNC_INTR_CAUSE_OFFSET                                                        0x00000250
#define RTC_SYNC_INTR_CAUSE_SHUTDOWN_STATE_MSB                                                     0
#define RTC_SYNC_INTR_CAUSE_SHUTDOWN_STATE_LSB                                                     0
#define RTC_SYNC_INTR_CAUSE_SHUTDOWN_STATE_MASK                                           0x00000001
#define RTC_SYNC_INTR_CAUSE_SHUTDOWN_STATE_GET(x)                          (((x) & 0x00000001) >> 0)
#define RTC_SYNC_INTR_CAUSE_SHUTDOWN_STATE_SET(x)                          (((x) << 0) & 0x00000001)
#define RTC_SYNC_INTR_CAUSE_ON_STATE_MSB                                                           1
#define RTC_SYNC_INTR_CAUSE_ON_STATE_LSB                                                           1
#define RTC_SYNC_INTR_CAUSE_ON_STATE_MASK                                                 0x00000002
#define RTC_SYNC_INTR_CAUSE_ON_STATE_GET(x)                                (((x) & 0x00000002) >> 1)
#define RTC_SYNC_INTR_CAUSE_ON_STATE_SET(x)                                (((x) << 1) & 0x00000002)
#define RTC_SYNC_INTR_CAUSE_SLEEP_STATE_MSB                                                        2
#define RTC_SYNC_INTR_CAUSE_SLEEP_STATE_LSB                                                        2
#define RTC_SYNC_INTR_CAUSE_SLEEP_STATE_MASK                                              0x00000004
#define RTC_SYNC_INTR_CAUSE_SLEEP_STATE_GET(x)                             (((x) & 0x00000004) >> 2)
#define RTC_SYNC_INTR_CAUSE_SLEEP_STATE_SET(x)                             (((x) << 2) & 0x00000004)
#define RTC_SYNC_INTR_CAUSE_WAKEUP_STATE_MSB                                                       3
#define RTC_SYNC_INTR_CAUSE_WAKEUP_STATE_LSB                                                       3
#define RTC_SYNC_INTR_CAUSE_WAKEUP_STATE_MASK                                             0x00000008
#define RTC_SYNC_INTR_CAUSE_WAKEUP_STATE_GET(x)                            (((x) & 0x00000008) >> 3)
#define RTC_SYNC_INTR_CAUSE_WAKEUP_STATE_SET(x)                            (((x) << 3) & 0x00000008)
#define RTC_SYNC_INTR_CAUSE_SLEEP_ACCESS_MSB                                                       4
#define RTC_SYNC_INTR_CAUSE_SLEEP_ACCESS_LSB                                                       4
#define RTC_SYNC_INTR_CAUSE_SLEEP_ACCESS_MASK                                             0x00000010
#define RTC_SYNC_INTR_CAUSE_SLEEP_ACCESS_GET(x)                            (((x) & 0x00000010) >> 4)
#define RTC_SYNC_INTR_CAUSE_SLEEP_ACCESS_SET(x)                            (((x) << 4) & 0x00000010)
#define RTC_SYNC_INTR_CAUSE_PLL_CHANGING_MSB                                                       5
#define RTC_SYNC_INTR_CAUSE_PLL_CHANGING_LSB                                                       5
#define RTC_SYNC_INTR_CAUSE_PLL_CHANGING_MASK                                             0x00000020
#define RTC_SYNC_INTR_CAUSE_PLL_CHANGING_GET(x)                            (((x) & 0x00000020) >> 5)
#define RTC_SYNC_INTR_CAUSE_PLL_CHANGING_SET(x)                            (((x) << 5) & 0x00000020)

/* macros for RTC_SYNC_INTR_ENABLE */
#define RTC_SYNC_INTR_ENABLE_ADDRESS                                                      0x00000254
#define RTC_SYNC_INTR_ENABLE_OFFSET                                                       0x00000254
#define RTC_SYNC_INTR_ENABLE_SHUTDOWN_STATE_MSB                                                    0
#define RTC_SYNC_INTR_ENABLE_SHUTDOWN_STATE_LSB                                                    0
#define RTC_SYNC_INTR_ENABLE_SHUTDOWN_STATE_MASK                                          0x00000001
#define RTC_SYNC_INTR_ENABLE_SHUTDOWN_STATE_GET(x)                         (((x) & 0x00000001) >> 0)
#define RTC_SYNC_INTR_ENABLE_SHUTDOWN_STATE_SET(x)                         (((x) << 0) & 0x00000001)
#define RTC_SYNC_INTR_ENABLE_ON_STATE_MSB                                                          1
#define RTC_SYNC_INTR_ENABLE_ON_STATE_LSB                                                          1
#define RTC_SYNC_INTR_ENABLE_ON_STATE_MASK                                                0x00000002
#define RTC_SYNC_INTR_ENABLE_ON_STATE_GET(x)                               (((x) & 0x00000002) >> 1)
#define RTC_SYNC_INTR_ENABLE_ON_STATE_SET(x)                               (((x) << 1) & 0x00000002)
#define RTC_SYNC_INTR_ENABLE_SLEEP_STATE_MSB                                                       2
#define RTC_SYNC_INTR_ENABLE_SLEEP_STATE_LSB                                                       2
#define RTC_SYNC_INTR_ENABLE_SLEEP_STATE_MASK                                             0x00000004
#define RTC_SYNC_INTR_ENABLE_SLEEP_STATE_GET(x)                            (((x) & 0x00000004) >> 2)
#define RTC_SYNC_INTR_ENABLE_SLEEP_STATE_SET(x)                            (((x) << 2) & 0x00000004)
#define RTC_SYNC_INTR_ENABLE_WAKEUP_STATE_MSB                                                      3
#define RTC_SYNC_INTR_ENABLE_WAKEUP_STATE_LSB                                                      3
#define RTC_SYNC_INTR_ENABLE_WAKEUP_STATE_MASK                                            0x00000008
#define RTC_SYNC_INTR_ENABLE_WAKEUP_STATE_GET(x)                           (((x) & 0x00000008) >> 3)
#define RTC_SYNC_INTR_ENABLE_WAKEUP_STATE_SET(x)                           (((x) << 3) & 0x00000008)
#define RTC_SYNC_INTR_ENABLE_SLEEP_ACCESS_MSB                                                      4
#define RTC_SYNC_INTR_ENABLE_SLEEP_ACCESS_LSB                                                      4
#define RTC_SYNC_INTR_ENABLE_SLEEP_ACCESS_MASK                                            0x00000010
#define RTC_SYNC_INTR_ENABLE_SLEEP_ACCESS_GET(x)                           (((x) & 0x00000010) >> 4)
#define RTC_SYNC_INTR_ENABLE_SLEEP_ACCESS_SET(x)                           (((x) << 4) & 0x00000010)
#define RTC_SYNC_INTR_ENABLE_PLL_CHANGING_MSB                                                      5
#define RTC_SYNC_INTR_ENABLE_PLL_CHANGING_LSB                                                      5
#define RTC_SYNC_INTR_ENABLE_PLL_CHANGING_MASK                                            0x00000020
#define RTC_SYNC_INTR_ENABLE_PLL_CHANGING_GET(x)                           (((x) & 0x00000020) >> 5)
#define RTC_SYNC_INTR_ENABLE_PLL_CHANGING_SET(x)                           (((x) << 5) & 0x00000020)

/* macros for RTC_SYNC_INTR_MASK */
#define RTC_SYNC_INTR_MASK_ADDRESS                                                        0x00000258
#define RTC_SYNC_INTR_MASK_OFFSET                                                         0x00000258
#define RTC_SYNC_INTR_MASK_SHUTDOWN_STATE_MSB                                                      0
#define RTC_SYNC_INTR_MASK_SHUTDOWN_STATE_LSB                                                      0
#define RTC_SYNC_INTR_MASK_SHUTDOWN_STATE_MASK                                            0x00000001
#define RTC_SYNC_INTR_MASK_SHUTDOWN_STATE_GET(x)                           (((x) & 0x00000001) >> 0)
#define RTC_SYNC_INTR_MASK_SHUTDOWN_STATE_SET(x)                           (((x) << 0) & 0x00000001)
#define RTC_SYNC_INTR_MASK_ON_STATE_MSB                                                            1
#define RTC_SYNC_INTR_MASK_ON_STATE_LSB                                                            1
#define RTC_SYNC_INTR_MASK_ON_STATE_MASK                                                  0x00000002
#define RTC_SYNC_INTR_MASK_ON_STATE_GET(x)                                 (((x) & 0x00000002) >> 1)
#define RTC_SYNC_INTR_MASK_ON_STATE_SET(x)                                 (((x) << 1) & 0x00000002)
#define RTC_SYNC_INTR_MASK_SLEEP_STATE_MSB                                                         2
#define RTC_SYNC_INTR_MASK_SLEEP_STATE_LSB                                                         2
#define RTC_SYNC_INTR_MASK_SLEEP_STATE_MASK                                               0x00000004
#define RTC_SYNC_INTR_MASK_SLEEP_STATE_GET(x)                              (((x) & 0x00000004) >> 2)
#define RTC_SYNC_INTR_MASK_SLEEP_STATE_SET(x)                              (((x) << 2) & 0x00000004)
#define RTC_SYNC_INTR_MASK_WAKEUP_STATE_MSB                                                        3
#define RTC_SYNC_INTR_MASK_WAKEUP_STATE_LSB                                                        3
#define RTC_SYNC_INTR_MASK_WAKEUP_STATE_MASK                                              0x00000008
#define RTC_SYNC_INTR_MASK_WAKEUP_STATE_GET(x)                             (((x) & 0x00000008) >> 3)
#define RTC_SYNC_INTR_MASK_WAKEUP_STATE_SET(x)                             (((x) << 3) & 0x00000008)
#define RTC_SYNC_INTR_MASK_SLEEP_ACCESS_MSB                                                        4
#define RTC_SYNC_INTR_MASK_SLEEP_ACCESS_LSB                                                        4
#define RTC_SYNC_INTR_MASK_SLEEP_ACCESS_MASK                                              0x00000010
#define RTC_SYNC_INTR_MASK_SLEEP_ACCESS_GET(x)                             (((x) & 0x00000010) >> 4)
#define RTC_SYNC_INTR_MASK_SLEEP_ACCESS_SET(x)                             (((x) << 4) & 0x00000010)
#define RTC_SYNC_INTR_MASK_PLL_CHANGING_MSB                                                        5
#define RTC_SYNC_INTR_MASK_PLL_CHANGING_LSB                                                        5
#define RTC_SYNC_INTR_MASK_PLL_CHANGING_MASK                                              0x00000020
#define RTC_SYNC_INTR_MASK_PLL_CHANGING_GET(x)                             (((x) & 0x00000020) >> 5)
#define RTC_SYNC_INTR_MASK_PLL_CHANGING_SET(x)                             (((x) << 5) & 0x00000020)


#ifndef __ASSEMBLER__

typedef struct rtc_sync_reg_reg_s {
  volatile char pad__0[0x240];                                         /*        0x0 - 0x240      */
  volatile unsigned int RTC_SYNC_RESET;                                /*      0x240 - 0x244      */
  volatile unsigned int RTC_SYNC_STATUS;                               /*      0x244 - 0x248      */
  volatile unsigned int RTC_SYNC_DERIVED;                              /*      0x248 - 0x24c      */
  volatile unsigned int RTC_SYNC_FORCE_WAKE;                           /*      0x24c - 0x250      */
  volatile unsigned int RTC_SYNC_INTR_CAUSE;                           /*      0x250 - 0x254      */
  volatile unsigned int RTC_SYNC_INTR_ENABLE;                          /*      0x254 - 0x258      */
  volatile unsigned int RTC_SYNC_INTR_MASK;                            /*      0x258 - 0x25c      */
} rtc_sync_reg_reg_t;

#endif /* __ASSEMBLER__ */

#endif /* _RTC_SYNC_REG_REG_H_ */
