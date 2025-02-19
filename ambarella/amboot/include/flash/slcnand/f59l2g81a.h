/**
 * @file system/include/flash/slcnand/f59l2g81a.h
 *
 * History:
 *    2012/12/18 - [Ken He] created file
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


#ifndef __F59L2G81A_H__
#define __F59L2G81A_H__

/**
 * nand control register initial setting
 */
#define __NAND_CONTROL						  \
	(NAND_CTR_C2		|				  \
	 NAND_CTR_P3		|				  \
	 NAND_CTR_I4		|				  \
	 NAND_CTR_RC		|				  \
	 NAND_CTR_CC		|				  \
	 NAND_CTR_IE		|				  \
	 NAND_CTR_SZ_2G		|				  \
	 NAND_CTR_WD_8BIT)

#define NAND_MANID		0xC8
#define NAND_DEVID		0xDA
#define NAND_ID3		0x90
#define NAND_ID4		0x95
#define NAND_ID5		0x44
/**
 * define for device info
 */
#define NAND_MAIN_SIZE		2048
#define NAND_SPARE_SIZE		64
#define NAND_PAGE_SIZE		2112
#define NAND_PAGES_PER_BLOCK	64
#define NAND_BLOCKS_PER_PLANE	1024
#define NAND_BLOCKS_PER_ZONE	1024
#define NAND_BLOCKS_PER_BANK	2048
#define NAND_PLANES_PER_BANK	(NAND_BLOCKS_PER_BANK / NAND_BLOCKS_PER_PLANE)
#define NAND_BANKS_PER_DEVICE	1
#define NAND_TOTAL_BLOCKS	(NAND_BLOCKS_PER_BANK * NAND_BANKS_PER_DEVICE)
#define NAND_TOTAL_ZONES	(NAND_TOTAL_BLOCKS / NAND_BLOCKS_PER_ZONE)
#define NAND_TOTAL_PLANES	(NAND_TOTAL_BLOCKS / NAND_BLOCKS_PER_PLANE)

/* Copyback must be in the same plane, so we have to know the plane address */
#define NAND_BLOCK_ADDR_BIT	18
#define NAND_PLANE_ADDR_BIT	28 /* A28 must be the same for copyback */
#define NAND_PLANE_MASK		0x1
/* Used to mask the plane address according to block address in the same bank */
#define NAND_PLANE_ADDR_MASK	(NAND_PLANE_MASK << (NAND_PLANE_ADDR_BIT - \
						     NAND_BLOCK_ADDR_BIT))

#define NAND_PLANE_MAP		NAND_PLANE_MAP_2
#define NAND_COLUMN_CYCLES	2
#define NAND_PAGE_CYCLES	3
#define NAND_ID_CYCLES		5
#define NAND_CHIP_WIDTH		8
#define NAND_CHIP_SIZE_MB	256
#define NAND_BUS_WIDTH		8

#define NAND_NAME	"ESMT F59L2G81A_256MB_PG2K"

#if defined(CONFIG_NAND_1DEVICE)
#define NAND_DEVICES		1
#elif defined(CONFIG_NAND_2DEVICE)
#define NAND_DEVICES		2
#elif defined(CONFIG_NAND_4DEVICE)
#define NAND_DEVICES		4
#endif

#define NAND_TOTAL_BANKS	(NAND_DEVICES * NAND_BANKS_PER_DEVICE)

#if (NAND_TOTAL_BANKS == 1)
#define NAND_CONTROL		(__NAND_CONTROL | NAND_CTR_1BANK)
#elif (NAND_TOTAL_BANKS == 2)
#define NAND_CONTROL		(__NAND_CONTROL | NAND_CTR_2BANK)
#elif (NAND_TOTAL_BANKS == 4)
#define NAND_CONTROL		(__NAND_CONTROL | NAND_CTR_4BANK)
#elif (NAND_TOTAL_BANKS > 4)
#error Unsupport nand flash banks
#endif

#define NAND_BB_MARKER_OFFSET	0	/* bad block information */

/**
 * define for partition info
 */
#define NAND_RSV_BLKS_PER_ZONE	24

/**
 * timing parameter in ns
 */
#define NAND_TCLS		12
#define NAND_TALS		12
#define NAND_TCS		20
#define NAND_TDS		12
#define NAND_TCLH		5
#define NAND_TALH		5
#define NAND_TCH		50
#define NAND_TDH		5
#define NAND_TWP		12
#define NAND_TWH		10
#define NAND_TWB		100
#define NAND_TRR		20
#define NAND_TRP		12
#define NAND_TREH		10
#define NAND_TRB		100 /* not defined in datasheet */
#define NAND_TCEH		70  /* trhz - tchz = 100 - 30 = 70 */
#define NAND_TRDELAY		20  /* trea */
#define NAND_TCLR		10
#define NAND_TWHR		60
#define NAND_TIR		0
#define NAND_TWW		100  /* not defined in datasheet */
#define NAND_TRHZ		100
#define NAND_TAR		10

#endif
