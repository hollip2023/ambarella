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


#ifndef _BRIDGE_INTR_REG_GMAC_1_1_REG_H_
#define _BRIDGE_INTR_REG_GMAC_1_1_REG_H_

#define GMAC_INTERRUPT_ADDRESS                   0x00000000
#define GMAC_INTERRUPT_OFFSET                    0x00000000
#define GMAC_INTERRUPT_TX_0_END_MSB              24
#define GMAC_INTERRUPT_TX_0_END_LSB              24
#define GMAC_INTERRUPT_TX_0_END_MASK             0x01000000
#define GMAC_INTERRUPT_TX_0_END_GET(x)           (((x) & GMAC_INTERRUPT_TX_0_END_MASK) >> GMAC_INTERRUPT_TX_0_END_LSB)
#define GMAC_INTERRUPT_TX_0_END_SET(x)           (((x) << GMAC_INTERRUPT_TX_0_END_LSB) & GMAC_INTERRUPT_TX_0_END_MASK)
#define GMAC_INTERRUPT_TX_0_COMPLETE_MSB         16
#define GMAC_INTERRUPT_TX_0_COMPLETE_LSB         16
#define GMAC_INTERRUPT_TX_0_COMPLETE_MASK        0x00010000
#define GMAC_INTERRUPT_TX_0_COMPLETE_GET(x)      (((x) & GMAC_INTERRUPT_TX_0_COMPLETE_MASK) >> GMAC_INTERRUPT_TX_0_COMPLETE_LSB)
#define GMAC_INTERRUPT_TX_0_COMPLETE_SET(x)      (((x) << GMAC_INTERRUPT_TX_0_COMPLETE_LSB) & GMAC_INTERRUPT_TX_0_COMPLETE_MASK)
#define GMAC_INTERRUPT_RX_0_END_MSB              8
#define GMAC_INTERRUPT_RX_0_END_LSB              8
#define GMAC_INTERRUPT_RX_0_END_MASK             0x00000100
#define GMAC_INTERRUPT_RX_0_END_GET(x)           (((x) & GMAC_INTERRUPT_RX_0_END_MASK) >> GMAC_INTERRUPT_RX_0_END_LSB)
#define GMAC_INTERRUPT_RX_0_END_SET(x)           (((x) << GMAC_INTERRUPT_RX_0_END_LSB) & GMAC_INTERRUPT_RX_0_END_MASK)
#define GMAC_INTERRUPT_RX_0_COMPLETE_MSB         0
#define GMAC_INTERRUPT_RX_0_COMPLETE_LSB         0
#define GMAC_INTERRUPT_RX_0_COMPLETE_MASK        0x00000001
#define GMAC_INTERRUPT_RX_0_COMPLETE_GET(x)      (((x) & GMAC_INTERRUPT_RX_0_COMPLETE_MASK) >> GMAC_INTERRUPT_RX_0_COMPLETE_LSB)
#define GMAC_INTERRUPT_RX_0_COMPLETE_SET(x)      (((x) << GMAC_INTERRUPT_RX_0_COMPLETE_LSB) & GMAC_INTERRUPT_RX_0_COMPLETE_MASK)

#define GMAC_INTERRUPT_MASK_ADDRESS              0x00000004
#define GMAC_INTERRUPT_MASK_OFFSET               0x00000004
#define GMAC_INTERRUPT_MASK_TX_0_END_MASK_MSB    24
#define GMAC_INTERRUPT_MASK_TX_0_END_MASK_LSB    24
#define GMAC_INTERRUPT_MASK_TX_0_END_MASK_MASK   0x01000000
#define GMAC_INTERRUPT_MASK_TX_0_END_MASK_GET(x) (((x) & GMAC_INTERRUPT_MASK_TX_0_END_MASK_MASK) >> GMAC_INTERRUPT_MASK_TX_0_END_MASK_LSB)
#define GMAC_INTERRUPT_MASK_TX_0_END_MASK_SET(x) (((x) << GMAC_INTERRUPT_MASK_TX_0_END_MASK_LSB) & GMAC_INTERRUPT_MASK_TX_0_END_MASK_MASK)
#define GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_MSB 16
#define GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_LSB 16
#define GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_MASK 0x00010000
#define GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_GET(x) (((x) & GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_MASK) >> GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_LSB)
#define GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_SET(x) (((x) << GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_LSB) & GMAC_INTERRUPT_MASK_TX_0_COMPLETE_MASK_MASK)
#define GMAC_INTERRUPT_MASK_RX_0_END_MASK_MSB    8
#define GMAC_INTERRUPT_MASK_RX_0_END_MASK_LSB    8
#define GMAC_INTERRUPT_MASK_RX_0_END_MASK_MASK   0x00000100
#define GMAC_INTERRUPT_MASK_RX_0_END_MASK_GET(x) (((x) & GMAC_INTERRUPT_MASK_RX_0_END_MASK_MASK) >> GMAC_INTERRUPT_MASK_RX_0_END_MASK_LSB)
#define GMAC_INTERRUPT_MASK_RX_0_END_MASK_SET(x) (((x) << GMAC_INTERRUPT_MASK_RX_0_END_MASK_LSB) & GMAC_INTERRUPT_MASK_RX_0_END_MASK_MASK)
#define GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_MSB 0
#define GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_LSB 0
#define GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_MASK 0x00000001
#define GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_GET(x) (((x) & GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_MASK) >> GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_LSB)
#define GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_SET(x) (((x) << GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_LSB) & GMAC_INTERRUPT_MASK_RX_0_COMPLETE_MASK_MASK)


#ifndef __ASSEMBLER__

typedef struct bridge_intr_reg_gmac_1_1_reg_s {
  volatile unsigned int gmac_interrupt;
  volatile unsigned int gmac_interrupt_mask;
} bridge_intr_reg_gmac_1_1_reg_t;

#endif /* __ASSEMBLER__ */

#endif /* _BRIDGE_INTR_REG_GMAC_1_1_H_ */
