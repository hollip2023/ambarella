//------------------------------------------------------------------------------
// <copyright file="target_reg_table.h" company="Atheros">
//    Copyright (c) 2004-2010 Atheros Corporation.  All rights reserved.
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
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Target register table macros and structure definitions
//
// Author(s): ="Atheros"
//==============================================================================

#ifndef TARGET_REG_TABLE_H_
#define TARGET_REG_TABLE_H_

#include "targaddrs.h"
/*** WARNING : Add to the end of the TABLE! do not change the order ****/
typedef struct targetdef_s {
    A_UINT32 d_RTC_SOC_BASE_ADDRESS;
    A_UINT32 d_RTC_WMAC_BASE_ADDRESS;
    A_UINT32 d_SYSTEM_SLEEP_OFFSET;
    A_UINT32 d_WLAN_SYSTEM_SLEEP_OFFSET;
    A_UINT32 d_WLAN_SYSTEM_SLEEP_DISABLE_LSB;
    A_UINT32 d_WLAN_SYSTEM_SLEEP_DISABLE_MASK;
    A_UINT32 d_CLOCK_CONTROL_OFFSET;
    A_UINT32 d_CLOCK_CONTROL_SI0_CLK_MASK;
    A_UINT32 d_RESET_CONTROL_OFFSET;
    A_UINT32 d_RESET_CONTROL_MBOX_RST_MASK;
    A_UINT32 d_RESET_CONTROL_SI0_RST_MASK;
    A_UINT32 d_WLAN_RESET_CONTROL_OFFSET;
    A_UINT32 d_WLAN_RESET_CONTROL_COLD_RST_MASK;
    A_UINT32 d_WLAN_RESET_CONTROL_WARM_RST_MASK;
    A_UINT32 d_GPIO_BASE_ADDRESS;
    A_UINT32 d_GPIO_PIN0_OFFSET;
    A_UINT32 d_GPIO_PIN1_OFFSET;
    A_UINT32 d_GPIO_PIN0_CONFIG_MASK;
    A_UINT32 d_GPIO_PIN1_CONFIG_MASK;
    A_UINT32 d_SI_CONFIG_BIDIR_OD_DATA_LSB;
    A_UINT32 d_SI_CONFIG_BIDIR_OD_DATA_MASK;
    A_UINT32 d_SI_CONFIG_I2C_LSB;
    A_UINT32 d_SI_CONFIG_I2C_MASK;
    A_UINT32 d_SI_CONFIG_POS_SAMPLE_LSB;
    A_UINT32 d_SI_CONFIG_POS_SAMPLE_MASK;
    A_UINT32 d_SI_CONFIG_INACTIVE_CLK_LSB;
    A_UINT32 d_SI_CONFIG_INACTIVE_CLK_MASK;
    A_UINT32 d_SI_CONFIG_INACTIVE_DATA_LSB;
    A_UINT32 d_SI_CONFIG_INACTIVE_DATA_MASK;
    A_UINT32 d_SI_CONFIG_DIVIDER_LSB;
    A_UINT32 d_SI_CONFIG_DIVIDER_MASK;
    A_UINT32 d_SI_BASE_ADDRESS;
    A_UINT32 d_SI_CONFIG_OFFSET;
    A_UINT32 d_SI_TX_DATA0_OFFSET;
    A_UINT32 d_SI_TX_DATA1_OFFSET;
    A_UINT32 d_SI_RX_DATA0_OFFSET;
    A_UINT32 d_SI_RX_DATA1_OFFSET;
    A_UINT32 d_SI_CS_OFFSET;
    A_UINT32 d_SI_CS_DONE_ERR_MASK;
    A_UINT32 d_SI_CS_DONE_INT_MASK;
    A_UINT32 d_SI_CS_START_LSB;
    A_UINT32 d_SI_CS_START_MASK;
    A_UINT32 d_SI_CS_RX_CNT_LSB;
    A_UINT32 d_SI_CS_RX_CNT_MASK;
    A_UINT32 d_SI_CS_TX_CNT_LSB;
    A_UINT32 d_SI_CS_TX_CNT_MASK;
    A_UINT32 d_BOARD_DATA_SZ;
    A_UINT32 d_BOARD_EXT_DATA_SZ;
    A_UINT32 d_MBOX_BASE_ADDRESS;
    A_UINT32 d_LOCAL_SCRATCH_OFFSET;
    A_UINT32 d_CPU_CLOCK_OFFSET;
    A_UINT32 d_LPO_CAL_OFFSET;
    A_UINT32 d_GPIO_PIN10_OFFSET;
    A_UINT32 d_GPIO_PIN11_OFFSET;
    A_UINT32 d_GPIO_PIN12_OFFSET;
    A_UINT32 d_GPIO_PIN13_OFFSET;
    A_UINT32 d_CLOCK_GPIO_OFFSET;
    A_UINT32 d_CPU_CLOCK_STANDARD_LSB;
    A_UINT32 d_CPU_CLOCK_STANDARD_MASK;
    A_UINT32 d_LPO_CAL_ENABLE_LSB;
    A_UINT32 d_LPO_CAL_ENABLE_MASK;
    A_UINT32 d_CLOCK_GPIO_BT_CLK_OUT_EN_LSB;
    A_UINT32 d_CLOCK_GPIO_BT_CLK_OUT_EN_MASK;
    A_UINT32 d_ANALOG_INTF_BASE_ADDRESS;
} TARGET_REGISTER_TABLE;

#define ATH_UNSUPPORTED_REG_OFFSET 0xffffffff
#define ATH_SUPPORTED_BY_TARGET(reg_offset) ((reg_offset) != ATH_UNSUPPORTED_REG_OFFSET)

#define BOARD_DATA_SZ_MAX 2048

#if defined(MY_TARGET_DEF) /* { */
#if defined(ATHR_WIN_DEF)
#define ATH_REG_TABLE_DIRECT_ASSIGN
#endif

/* Cross-platform compatibility */
#if !defined(SOC_RESET_CONTROL_OFFSET) && defined(RESET_CONTROL_OFFSET)
#define SOC_RESET_CONTROL_OFFSET RESET_CONTROL_OFFSET
#endif
#if !defined(CLOCK_GPIO_OFFSET)
#define CLOCK_GPIO_OFFSET ATH_UNSUPPORTED_REG_OFFSET
#define CLOCK_GPIO_BT_CLK_OUT_EN_LSB 0
#define CLOCK_GPIO_BT_CLK_OUT_EN_MASK 0
#endif

#ifdef ATH_REG_TABLE_DIRECT_ASSIGN

static struct targetdef_s my_target_def = {
    RTC_SOC_BASE_ADDRESS,
    RTC_WMAC_BASE_ADDRESS,
    SYSTEM_SLEEP_OFFSET,
    WLAN_SYSTEM_SLEEP_OFFSET,
    WLAN_SYSTEM_SLEEP_DISABLE_LSB,
    WLAN_SYSTEM_SLEEP_DISABLE_MASK,
    CLOCK_CONTROL_OFFSET,
    CLOCK_CONTROL_SI0_CLK_MASK,
    SOC_RESET_CONTROL_OFFSET,
    RESET_CONTROL_MBOX_RST_MASK,
    RESET_CONTROL_SI0_RST_MASK,
    WLAN_RESET_CONTROL_OFFSET,
    WLAN_RESET_CONTROL_COLD_RST_MASK,
    WLAN_RESET_CONTROL_WARM_RST_MASK,
    GPIO_BASE_ADDRESS,
    GPIO_PIN0_OFFSET,
    GPIO_PIN1_OFFSET,
    GPIO_PIN0_CONFIG_MASK,
    GPIO_PIN1_CONFIG_MASK,
    SI_CONFIG_BIDIR_OD_DATA_LSB,
    SI_CONFIG_BIDIR_OD_DATA_MASK,
    SI_CONFIG_I2C_LSB,
    SI_CONFIG_I2C_MASK,
    SI_CONFIG_POS_SAMPLE_LSB,
    SI_CONFIG_POS_SAMPLE_MASK,
    SI_CONFIG_INACTIVE_CLK_LSB,
    SI_CONFIG_INACTIVE_CLK_MASK,
    SI_CONFIG_INACTIVE_DATA_LSB,
    SI_CONFIG_INACTIVE_DATA_MASK,
    SI_CONFIG_DIVIDER_LSB,
    SI_CONFIG_DIVIDER_MASK,
    SI_BASE_ADDRESS,
    SI_CONFIG_OFFSET,
    SI_TX_DATA0_OFFSET,
    SI_TX_DATA1_OFFSET,
    SI_RX_DATA0_OFFSET,
    SI_RX_DATA1_OFFSET,
    SI_CS_OFFSET,
    SI_CS_DONE_ERR_MASK,
    SI_CS_DONE_INT_MASK,
    SI_CS_START_LSB,
    SI_CS_START_MASK,
    SI_CS_RX_CNT_LSB,
    SI_CS_RX_CNT_MASK,
    SI_CS_TX_CNT_LSB,
    SI_CS_TX_CNT_MASK,
    MY_TARGET_BOARD_DATA_SZ,
    MY_TARGET_BOARD_EXT_DATA_SZ,
    MBOX_BASE_ADDRESS,
    LOCAL_SCRATCH_OFFSET,
    CPU_CLOCK_OFFSET,
    LPO_CAL_OFFSET,
    GPIO_PIN10_OFFSET,
    GPIO_PIN11_OFFSET,
    GPIO_PIN12_OFFSET,
    GPIO_PIN13_OFFSET,
    CLOCK_GPIO_OFFSET,
    CPU_CLOCK_STANDARD_LSB,
    CPU_CLOCK_STANDARD_MASK,
    LPO_CAL_ENABLE_LSB,
    LPO_CAL_ENABLE_MASK,
    CLOCK_GPIO_BT_CLK_OUT_EN_LSB,
    CLOCK_GPIO_BT_CLK_OUT_EN_MASK,
    ANALOG_INTF_BASE_ADDRESS,
};

#else

static struct targetdef_s my_target_def = {
    .d_RTC_SOC_BASE_ADDRESS = RTC_SOC_BASE_ADDRESS,                      
    .d_RTC_WMAC_BASE_ADDRESS = RTC_WMAC_BASE_ADDRESS,
    .d_SYSTEM_SLEEP_OFFSET = WLAN_SYSTEM_SLEEP_OFFSET,
    .d_WLAN_SYSTEM_SLEEP_OFFSET = WLAN_SYSTEM_SLEEP_OFFSET,
    .d_WLAN_SYSTEM_SLEEP_DISABLE_LSB = WLAN_SYSTEM_SLEEP_DISABLE_LSB,
    .d_WLAN_SYSTEM_SLEEP_DISABLE_MASK = WLAN_SYSTEM_SLEEP_DISABLE_MASK,
    .d_CLOCK_CONTROL_OFFSET = CLOCK_CONTROL_OFFSET,
    .d_CLOCK_CONTROL_SI0_CLK_MASK = CLOCK_CONTROL_SI0_CLK_MASK,
    .d_RESET_CONTROL_OFFSET = SOC_RESET_CONTROL_OFFSET,
    .d_RESET_CONTROL_MBOX_RST_MASK = RESET_CONTROL_MBOX_RST_MASK,
    .d_RESET_CONTROL_SI0_RST_MASK = RESET_CONTROL_SI0_RST_MASK,
    .d_WLAN_RESET_CONTROL_OFFSET = WLAN_RESET_CONTROL_OFFSET,
    .d_WLAN_RESET_CONTROL_COLD_RST_MASK = WLAN_RESET_CONTROL_COLD_RST_MASK,
    .d_WLAN_RESET_CONTROL_WARM_RST_MASK = WLAN_RESET_CONTROL_WARM_RST_MASK,
    .d_GPIO_BASE_ADDRESS = GPIO_BASE_ADDRESS,
    .d_GPIO_PIN0_OFFSET = GPIO_PIN0_OFFSET,
    .d_GPIO_PIN1_OFFSET = GPIO_PIN1_OFFSET,
    .d_GPIO_PIN0_CONFIG_MASK = GPIO_PIN0_CONFIG_MASK,
    .d_GPIO_PIN1_CONFIG_MASK = GPIO_PIN1_CONFIG_MASK,
    .d_SI_CONFIG_BIDIR_OD_DATA_LSB = SI_CONFIG_BIDIR_OD_DATA_LSB,
    .d_SI_CONFIG_BIDIR_OD_DATA_MASK = SI_CONFIG_BIDIR_OD_DATA_MASK,
    .d_SI_CONFIG_I2C_LSB = SI_CONFIG_I2C_LSB,
    .d_SI_CONFIG_I2C_MASK = SI_CONFIG_I2C_MASK,
    .d_SI_CONFIG_POS_SAMPLE_LSB = SI_CONFIG_POS_SAMPLE_LSB,
    .d_SI_CONFIG_POS_SAMPLE_MASK = SI_CONFIG_POS_SAMPLE_MASK,
    .d_SI_CONFIG_INACTIVE_CLK_LSB = SI_CONFIG_INACTIVE_CLK_LSB,
    .d_SI_CONFIG_INACTIVE_CLK_MASK = SI_CONFIG_INACTIVE_CLK_MASK,
    .d_SI_CONFIG_INACTIVE_DATA_LSB = SI_CONFIG_INACTIVE_DATA_LSB,
    .d_SI_CONFIG_INACTIVE_DATA_MASK = SI_CONFIG_INACTIVE_DATA_MASK,
    .d_SI_CONFIG_DIVIDER_LSB = SI_CONFIG_DIVIDER_LSB,
    .d_SI_CONFIG_DIVIDER_MASK = SI_CONFIG_DIVIDER_MASK,
    .d_SI_BASE_ADDRESS = SI_BASE_ADDRESS,
    .d_SI_CONFIG_OFFSET = SI_CONFIG_OFFSET,
    .d_SI_TX_DATA0_OFFSET = SI_TX_DATA0_OFFSET,
    .d_SI_TX_DATA1_OFFSET = SI_TX_DATA1_OFFSET,
    .d_SI_RX_DATA0_OFFSET = SI_RX_DATA0_OFFSET,
    .d_SI_RX_DATA1_OFFSET = SI_RX_DATA1_OFFSET,
    .d_SI_CS_OFFSET = SI_CS_OFFSET,
    .d_SI_CS_DONE_ERR_MASK = SI_CS_DONE_ERR_MASK,
    .d_SI_CS_DONE_INT_MASK = SI_CS_DONE_INT_MASK,
    .d_SI_CS_START_LSB = SI_CS_START_LSB,
    .d_SI_CS_START_MASK = SI_CS_START_MASK,
    .d_SI_CS_RX_CNT_LSB = SI_CS_RX_CNT_LSB,
    .d_SI_CS_RX_CNT_MASK = SI_CS_RX_CNT_MASK,
    .d_SI_CS_TX_CNT_LSB = SI_CS_TX_CNT_LSB,
    .d_SI_CS_TX_CNT_MASK = SI_CS_TX_CNT_MASK,
    .d_BOARD_DATA_SZ = MY_TARGET_BOARD_DATA_SZ,
    .d_BOARD_EXT_DATA_SZ = MY_TARGET_BOARD_EXT_DATA_SZ,
    .d_MBOX_BASE_ADDRESS = MBOX_BASE_ADDRESS,
    .d_LOCAL_SCRATCH_OFFSET = LOCAL_SCRATCH_OFFSET,
    .d_CPU_CLOCK_OFFSET = CPU_CLOCK_OFFSET,
    .d_LPO_CAL_OFFSET = LPO_CAL_OFFSET,
    .d_GPIO_PIN10_OFFSET = GPIO_PIN10_OFFSET,
    .d_GPIO_PIN11_OFFSET = GPIO_PIN11_OFFSET,
    .d_GPIO_PIN12_OFFSET = GPIO_PIN12_OFFSET,
    .d_GPIO_PIN13_OFFSET = GPIO_PIN13_OFFSET,
    .d_CLOCK_GPIO_OFFSET = CLOCK_GPIO_OFFSET,
    .d_CPU_CLOCK_STANDARD_LSB = CPU_CLOCK_STANDARD_LSB,
    .d_CPU_CLOCK_STANDARD_MASK = CPU_CLOCK_STANDARD_MASK,
    .d_LPO_CAL_ENABLE_LSB = LPO_CAL_ENABLE_LSB,
    .d_LPO_CAL_ENABLE_MASK = LPO_CAL_ENABLE_MASK,
    .d_CLOCK_GPIO_BT_CLK_OUT_EN_LSB = CLOCK_GPIO_BT_CLK_OUT_EN_LSB,
    .d_CLOCK_GPIO_BT_CLK_OUT_EN_MASK = CLOCK_GPIO_BT_CLK_OUT_EN_MASK,
    .d_ANALOG_INTF_BASE_ADDRESS = ANALOG_INTF_BASE_ADDRESS,
};

#endif

#if MY_TARGET_BOARD_DATA_SZ > BOARD_DATA_SZ_MAX
#error "BOARD_DATA_SZ_MAX is too small"
#endif

struct targetdef_s *MY_TARGET_DEF = &my_target_def;

#else /* } { */

#define RTC_SOC_BASE_ADDRESS                     (targetdef->d_RTC_SOC_BASE_ADDRESS)
#define RTC_WMAC_BASE_ADDRESS                    (targetdef->d_RTC_WMAC_BASE_ADDRESS)
#define SYSTEM_SLEEP_OFFSET                      (targetdef->d_SYSTEM_SLEEP_OFFSET)
#define WLAN_SYSTEM_SLEEP_OFFSET                 (targetdef->d_WLAN_SYSTEM_SLEEP_OFFSET)
#define WLAN_SYSTEM_SLEEP_DISABLE_LSB            (targetdef->d_WLAN_SYSTEM_SLEEP_DISABLE_LSB)
#define WLAN_SYSTEM_SLEEP_DISABLE_MASK           (targetdef->d_WLAN_SYSTEM_SLEEP_DISABLE_MASK)
#define CLOCK_CONTROL_OFFSET                     (targetdef->d_CLOCK_CONTROL_OFFSET)
#define CLOCK_CONTROL_SI0_CLK_MASK               (targetdef->d_CLOCK_CONTROL_SI0_CLK_MASK)
#define RESET_CONTROL_OFFSET                     (targetdef->d_RESET_CONTROL_OFFSET)
#define RESET_CONTROL_MBOX_RST_MASK              (targetdef->d_RESET_CONTROL_MBOX_RST_MASK)
#define RESET_CONTROL_SI0_RST_MASK               (targetdef->d_RESET_CONTROL_SI0_RST_MASK)
#define WLAN_RESET_CONTROL_OFFSET                (targetdef->d_WLAN_RESET_CONTROL_OFFSET)
#define WLAN_RESET_CONTROL_COLD_RST_MASK         (targetdef->d_WLAN_RESET_CONTROL_COLD_RST_MASK)
#define WLAN_RESET_CONTROL_WARM_RST_MASK         (targetdef->d_WLAN_RESET_CONTROL_WARM_RST_MASK)
#define GPIO_BASE_ADDRESS                        (targetdef->d_GPIO_BASE_ADDRESS)
#define GPIO_PIN0_OFFSET                         (targetdef->d_GPIO_PIN0_OFFSET)
#define GPIO_PIN1_OFFSET                         (targetdef->d_GPIO_PIN1_OFFSET)
#define GPIO_PIN0_CONFIG_MASK                    (targetdef->d_GPIO_PIN0_CONFIG_MASK)
#define GPIO_PIN1_CONFIG_MASK                    (targetdef->d_GPIO_PIN1_CONFIG_MASK)
#define SI_CONFIG_BIDIR_OD_DATA_LSB              (targetdef->d_SI_CONFIG_BIDIR_OD_DATA_LSB)
#define SI_CONFIG_BIDIR_OD_DATA_MASK             (targetdef->d_SI_CONFIG_BIDIR_OD_DATA_MASK)
#define SI_CONFIG_I2C_LSB                        (targetdef->d_SI_CONFIG_I2C_LSB)
#define SI_CONFIG_I2C_MASK                       (targetdef->d_SI_CONFIG_I2C_MASK)
#define SI_CONFIG_POS_SAMPLE_LSB                 (targetdef->d_SI_CONFIG_POS_SAMPLE_LSB)
#define SI_CONFIG_POS_SAMPLE_MASK                (targetdef->d_SI_CONFIG_POS_SAMPLE_MASK)
#define SI_CONFIG_INACTIVE_CLK_LSB               (targetdef->d_SI_CONFIG_INACTIVE_CLK_LSB)
#define SI_CONFIG_INACTIVE_CLK_MASK              (targetdef->d_SI_CONFIG_INACTIVE_CLK_MASK)
#define SI_CONFIG_INACTIVE_DATA_LSB              (targetdef->d_SI_CONFIG_INACTIVE_DATA_LSB)
#define SI_CONFIG_INACTIVE_DATA_MASK             (targetdef->d_SI_CONFIG_INACTIVE_DATA_MASK)
#define SI_CONFIG_DIVIDER_LSB                    (targetdef->d_SI_CONFIG_DIVIDER_LSB)
#define SI_CONFIG_DIVIDER_MASK                   (targetdef->d_SI_CONFIG_DIVIDER_MASK)
#define SI_BASE_ADDRESS                          (targetdef->d_SI_BASE_ADDRESS)
#define SI_CONFIG_OFFSET                         (targetdef->d_SI_CONFIG_OFFSET)
#define SI_TX_DATA0_OFFSET                       (targetdef->d_SI_TX_DATA0_OFFSET)
#define SI_TX_DATA1_OFFSET                       (targetdef->d_SI_TX_DATA1_OFFSET)
#define SI_RX_DATA0_OFFSET                       (targetdef->d_SI_RX_DATA0_OFFSET)
#define SI_RX_DATA1_OFFSET                       (targetdef->d_SI_RX_DATA1_OFFSET)
#define SI_CS_OFFSET                             (targetdef->d_SI_CS_OFFSET)
#define SI_CS_DONE_ERR_MASK                      (targetdef->d_SI_CS_DONE_ERR_MASK)
#define SI_CS_DONE_INT_MASK                      (targetdef->d_SI_CS_DONE_INT_MASK)
#define SI_CS_START_LSB                          (targetdef->d_SI_CS_START_LSB)
#define SI_CS_START_MASK                         (targetdef->d_SI_CS_START_MASK)
#define SI_CS_RX_CNT_LSB                         (targetdef->d_SI_CS_RX_CNT_LSB)
#define SI_CS_RX_CNT_MASK                        (targetdef->d_SI_CS_RX_CNT_MASK)
#define SI_CS_TX_CNT_LSB                         (targetdef->d_SI_CS_TX_CNT_LSB)
#define SI_CS_TX_CNT_MASK                        (targetdef->d_SI_CS_TX_CNT_MASK)
#define EEPROM_SZ                                (targetdef->d_BOARD_DATA_SZ)
#define EEPROM_EXT_SZ                            (targetdef->d_BOARD_EXT_DATA_SZ)
#define MBOX_BASE_ADDRESS                        (targetdef->d_MBOX_BASE_ADDRESS)
#define LOCAL_SCRATCH_OFFSET                     (targetdef->d_LOCAL_SCRATCH_OFFSET)
#define CPU_CLOCK_OFFSET                         (targetdef->d_CPU_CLOCK_OFFSET)
#define LPO_CAL_OFFSET                           (targetdef->d_LPO_CAL_OFFSET)
#define GPIO_PIN10_OFFSET                        (targetdef->d_GPIO_PIN10_OFFSET)
#define GPIO_PIN11_OFFSET                        (targetdef->d_GPIO_PIN11_OFFSET)
#define GPIO_PIN12_OFFSET                        (targetdef->d_GPIO_PIN12_OFFSET)
#define GPIO_PIN13_OFFSET                        (targetdef->d_GPIO_PIN13_OFFSET)
#define CLOCK_GPIO_OFFSET                        (targetdef->d_CLOCK_GPIO_OFFSET)
#define CPU_CLOCK_STANDARD_LSB                   (targetdef->d_CPU_CLOCK_STANDARD_LSB)
#define CPU_CLOCK_STANDARD_MASK                  (targetdef->d_CPU_CLOCK_STANDARD_MASK)
#define LPO_CAL_ENABLE_LSB                       (targetdef->d_LPO_CAL_ENABLE_LSB)
#define LPO_CAL_ENABLE_MASK                      (targetdef->d_LPO_CAL_ENABLE_MASK)
#define CLOCK_GPIO_BT_CLK_OUT_EN_LSB             (targetdef->d_CLOCK_GPIO_BT_CLK_OUT_EN_LSB)
#define CLOCK_GPIO_BT_CLK_OUT_EN_MASK            (targetdef->d_CLOCK_GPIO_BT_CLK_OUT_EN_MASK)
#define ANALOG_INTF_BASE_ADDRESS                 (targetdef->d_ANALOG_INTF_BASE_ADDRESS)

/* SET macros */
#define WLAN_SYSTEM_SLEEP_DISABLE_SET(x)         (((x) << WLAN_SYSTEM_SLEEP_DISABLE_LSB) & WLAN_SYSTEM_SLEEP_DISABLE_MASK)
#define SI_CONFIG_BIDIR_OD_DATA_SET(x)           (((x) << SI_CONFIG_BIDIR_OD_DATA_LSB) & SI_CONFIG_BIDIR_OD_DATA_MASK)
#define SI_CONFIG_I2C_SET(x)                     (((x) << SI_CONFIG_I2C_LSB) & SI_CONFIG_I2C_MASK)
#define SI_CONFIG_POS_SAMPLE_SET(x)              (((x) << SI_CONFIG_POS_SAMPLE_LSB) & SI_CONFIG_POS_SAMPLE_MASK)
#define SI_CONFIG_INACTIVE_CLK_SET(x)            (((x) << SI_CONFIG_INACTIVE_CLK_LSB) & SI_CONFIG_INACTIVE_CLK_MASK)
#define SI_CONFIG_INACTIVE_DATA_SET(x)           (((x) << SI_CONFIG_INACTIVE_DATA_LSB) & SI_CONFIG_INACTIVE_DATA_MASK)
#define SI_CONFIG_DIVIDER_SET(x)                 (((x) << SI_CONFIG_DIVIDER_LSB) & SI_CONFIG_DIVIDER_MASK)
#define SI_CS_START_SET(x)                       (((x) << SI_CS_START_LSB) & SI_CS_START_MASK)
#define SI_CS_RX_CNT_SET(x)                      (((x) << SI_CS_RX_CNT_LSB) & SI_CS_RX_CNT_MASK)
#define SI_CS_TX_CNT_SET(x)                      (((x) << SI_CS_TX_CNT_LSB) & SI_CS_TX_CNT_MASK)
#define LPO_CAL_ENABLE_SET(x)                    (((x) << LPO_CAL_ENABLE_LSB) & LPO_CAL_ENABLE_MASK)
#define CPU_CLOCK_STANDARD_SET(x)                (((x) << CPU_CLOCK_STANDARD_LSB) & CPU_CLOCK_STANDARD_MASK)
#define CLOCK_GPIO_BT_CLK_OUT_EN_SET(x)          (((x) << CLOCK_GPIO_BT_CLK_OUT_EN_LSB) & CLOCK_GPIO_BT_CLK_OUT_EN_MASK)
extern struct targetdef_s *targetdef;

#endif /* } */

#endif /*TARGET_REG_TABLE_H_*/
