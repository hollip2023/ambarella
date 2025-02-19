/**
 * ambhw/gpio.h
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
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
 */


#ifndef __AMBHW__GPIO_H__
#define __AMBHW__GPIO_H__

/* ==========================================================================*/
#if (CHIP_REV == A7L)
#define GPIO_INSTANCES			4
#define GPIO_MAX_LINES			128
#elif (CHIP_REV == A5S)
#define GPIO_INSTANCES			3
#define GPIO_MAX_LINES			96
#elif (CHIP_REV == S2) || (CHIP_REV == S2E)
#define GPIO_INSTANCES			5
#define GPIO_MAX_LINES			138
#elif (CHIP_REV == A8)
#define GPIO_INSTANCES			1
#define GPIO_MAX_LINES			16
#elif (CHIP_REV == S2L) || (CHIP_REV == S3L)
#define GPIO_INSTANCES			4
#define GPIO_MAX_LINES			114
#elif (CHIP_REV == S3)
#define GPIO_INSTANCES			7
#define GPIO_MAX_LINES			201
#else
#error "Not supported!"
#endif

/* ==========================================================================*/
#define GPIO0_OFFSET			0x9000
#define GPIO1_OFFSET			0xA000
#define GPIO2_OFFSET			0xE000
#if (CHIP_REV == A5S)
#define GPIO3_OFFSET			0x1F000
#elif (CHIP_REV == A7L)
#define GPIO3_OFFSET			0x1E000
#else
#define GPIO3_OFFSET			0x10000
#endif
#define GPIO4_OFFSET			0x11000
#if (CHIP_REV == S3)
#define GPIO5_OFFSET			0xD000
#else
#define GPIO5_OFFSET			0x12000
#endif
#define GPIO6_OFFSET			0x14000

#define GPIO0_BASE			(APB_BASE + GPIO0_OFFSET)
#define GPIO1_BASE			(APB_BASE + GPIO1_OFFSET)
#define GPIO2_BASE			(APB_BASE + GPIO2_OFFSET)
#define GPIO3_BASE			(APB_BASE + GPIO3_OFFSET)
#define GPIO4_BASE			(APB_BASE + GPIO4_OFFSET)
#define GPIO5_BASE			(APB_BASE + GPIO5_OFFSET)
#define GPIO6_BASE			(APB_BASE + GPIO6_OFFSET)

#define GPIO0_REG(x)			(GPIO0_BASE + (x))
#define GPIO1_REG(x)			(GPIO1_BASE + (x))
#define GPIO2_REG(x)			(GPIO2_BASE + (x))
#define GPIO3_REG(x)			(GPIO3_BASE + (x))
#define GPIO4_REG(x)			(GPIO4_BASE + (x))
#define GPIO5_REG(x)			(GPIO5_BASE + (x))
#define GPIO6_REG(x)			(GPIO6_BASE + (x))

/* ==========================================================================*/
#if (CHIP_REV == A5S)
#define GPIO_PAD_PULL_CTRL_SUPPORT		0
#else
#define GPIO_PAD_PULL_CTRL_SUPPORT		1
#endif

#if (CHIP_REV == A7L) || (CHIP_REV == S2) || (CHIP_REV == S2E) || (CHIP_REV == A8)
#define GPIO_PAD_PULL_OFFSET			0xD000
#else
#define GPIO_PAD_PULL_OFFSET			0x15000
#endif

#define GPIO_PAD_PULL_EN_0_OFFSET	0x80
#define GPIO_PAD_PULL_EN_1_OFFSET	0x84
#define GPIO_PAD_PULL_EN_2_OFFSET	0x88
#define GPIO_PAD_PULL_EN_3_OFFSET	0x8C
#define GPIO_PAD_PULL_EN_4_OFFSET	0x90
#define GPIO_PAD_PULL_EN_5_OFFSET	0x100
#define GPIO_PAD_PULL_EN_6_OFFSET	0x104

#define GPIO_PAD_PULL_DIR_0_OFFSET	0x94
#define GPIO_PAD_PULL_DIR_1_OFFSET	0x98
#define GPIO_PAD_PULL_DIR_2_OFFSET	0x9C
#define GPIO_PAD_PULL_DIR_3_OFFSET	0xA0
#define GPIO_PAD_PULL_DIR_4_OFFSET	0xA4
#define GPIO_PAD_PULL_DIR_5_OFFSET	0x10C
#define GPIO_PAD_PULL_DIR_6_OFFSET	0x110

#define GPIO_PAD_PULL_EN_OFFSET(bank)	((bank) >= 5 ? \
					(0x100 + (((bank) - 5) * 4)) : \
					(0x80 + ((bank) * 4)))
#define GPIO_PAD_PULL_DIR_OFFSET(bank)	((bank) >= 5 ? \
					(0x10C + (((bank) - 5) * 4)) : \
					(0x94 + ((bank) * 4)))

#define GPIO_PAD_PULL_BASE		(APB_BASE + GPIO_PAD_PULL_OFFSET)
#define GPIO_PAD_PULL_REG(x)		(GPIO_PAD_PULL_BASE + (x))

/* ==========================================================================*/
#define GPIO_DATA_OFFSET		0x00
#define GPIO_DIR_OFFSET			0x04
#define GPIO_IS_OFFSET			0x08
#define GPIO_IBE_OFFSET			0x0c
#define GPIO_IEV_OFFSET			0x10
#define GPIO_IE_OFFSET			0x14
#define GPIO_AFSEL_OFFSET		0x18
#define GPIO_RIS_OFFSET			0x1c
#define GPIO_MIS_OFFSET			0x20
#define GPIO_IC_OFFSET			0x24
#define GPIO_MASK_OFFSET		0x28
#define GPIO_ENABLE_OFFSET		0x2c

/* ==========================================================================*/
#if (CHIP_REV == S2L) || (CHIP_REV == S3) || (CHIP_REV == S3L)
#define	IOMUX_SUPPORT			1
#else
#define	IOMUX_SUPPORT			0
#endif

#define IOMUX_REG0_0_OFFSET		0x00
#define IOMUX_REG0_1_OFFSET		0x04
#define IOMUX_REG0_2_OFFSET		0x08
#define IOMUX_REG1_0_OFFSET		0x0c
#define IOMUX_REG1_1_OFFSET		0x10
#define IOMUX_REG1_2_OFFSET		0x14
#define IOMUX_REG2_0_OFFSET		0x18
#define IOMUX_REG2_1_OFFSET		0x1c
#define IOMUX_REG2_2_OFFSET		0x20
#define IOMUX_REG3_0_OFFSET		0x24
#define IOMUX_REG3_1_OFFSET		0x28
#define IOMUX_REG3_2_OFFSET		0x2c
#define IOMUX_REG4_0_OFFSET		0x30
#define IOMUX_REG4_1_OFFSET		0x34
#define IOMUX_REG4_2_OFFSET		0x38
#define IOMUX_REG5_0_OFFSET		0x3c
#define IOMUX_REG5_1_OFFSET		0x40
#define IOMUX_REG5_2_OFFSET		0x44
#define IOMUX_REG6_0_OFFSET		0x48
#define IOMUX_REG6_1_OFFSET		0x4c
#define IOMUX_REG6_2_OFFSET		0x50
#define IOMUX_CTRL_SET_OFFSET		0xf0
#define IOMUX_REG_OFFSET(bank, n)	(((bank) * 0xc) + ((n) * 4))

#define IOMUX_OFFSET			0x16000
#define IOMUX_BASE			(APB_BASE + IOMUX_OFFSET)
#define IOMUX_REG(x)			(IOMUX_BASE + (x))

/* ==========================================================================*/
#define GPIO_BANK_SIZE			32
#define AMBGPIO_SIZE			(GPIO_INSTANCES * GPIO_BANK_SIZE)

#ifndef CONFIG_AMBARELLA_EXT_GPIO_NUM
#define CONFIG_AMBARELLA_EXT_GPIO_NUM	(64)
#endif
#define EXT_GPIO(x)			(AMBGPIO_SIZE + x)

#define ARCH_NR_GPIOS			EXT_GPIO(CONFIG_AMBARELLA_EXT_GPIO_NUM)

/* ==========================================================================*/
#define GPIO(x)				(x)

#define GPIO_HIGH			1
#define GPIO_LOW			0

#define GPIO_FUNC_SW_INPUT		0
#define GPIO_FUNC_SW_OUTPUT		1
#define GPIO_FUNC_HW			2

/* ==========================================================================*/

#define PINID_TO_BANK(p)		((p) >> 5)
#define PINID_TO_OFFSET(p)		((p) & 0x1f)

/* ==========================================================================*/

#ifndef __ASM__

extern void gpio_init(void);
extern void gpio_mini_init(void);

#ifdef CONFIG_AMBOOT_ENABLE_GPIO
extern int gpio_config_hw(int gpio, int alt_func);
extern int gpio_config_sw_in(int gpio);
extern int gpio_config_sw_out(int gpio);
extern int gpio_set(int gpio);
extern int gpio_clr(int gpio);
extern int gpio_get(int gpio);
#else
static inline int gpio_config_hw(int gpio, int alt_func) {return -1; }
static inline int gpio_config_sw_in(int gpio) {return -1; }
static inline int gpio_config_sw_out(int gpio) {return -1; }
static inline int gpio_set(int gpio) {return -1; }
static inline int gpio_clr(int gpio) {return -1; }
static inline int gpio_get(int gpio) {return -1; }

#endif

/* ==========================================================================*/
#endif
/* ==========================================================================*/

#endif

