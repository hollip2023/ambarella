/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#define _SDIO_HALINIT_C_

#include <drv_types.h>
#include <rtl8188e_hal.h>
#include "hal_com_h2c.h"
#ifndef CONFIG_SDIO_HCI
#error "CONFIG_SDIO_HCI shall be on!\n"
#endif

/*
 * Description:
 *	Call this function to make sure power on successfully
 *
 * Return:
 *	_SUCCESS	enable success
 *	_FAIL	enable fail
 */

static int PowerOnCheck(PADAPTER padapter)
{
	u32	val_offset0, val_offset1, val_offset2, val_offset3;
	u32 val_mix = 0;
	u32 res = 0;
	u8	ret = _FAIL;
	int index = 0;

	val_offset0 = rtw_read8(padapter, REG_CR);
	val_offset1 = rtw_read8(padapter, REG_CR+1);
	val_offset2 = rtw_read8(padapter, REG_CR+2);
	val_offset3 = rtw_read8(padapter, REG_CR+3);

	if (val_offset0 == 0xEA || val_offset1 == 0xEA ||
			val_offset2 == 0xEA || val_offset3 ==0xEA) {
		DBG_871X("%s: power on fail, do Power on again\n", __func__);
		return ret;
	}

	val_mix = val_offset3 << 24 | val_mix;
	val_mix = val_offset2 << 16 | val_mix;
	val_mix = val_offset1 << 8 | val_mix;
	val_mix = val_offset0 | val_mix;

	res = rtw_read32(padapter, REG_CR);

	DBG_871X("%s: val_mix:0x%08x, res:0x%08x\n", __func__, val_mix, res);

	while(index < 100) {
		if (res == val_mix) {
			DBG_871X("%s: 0x100 the result of cmd52 and cmd53 is the same.\n", __func__);
			ret = _SUCCESS;
			break;
		} else {
			DBG_871X("%s: 0x100 cmd52 and cmd53 is not the same(index:%d).\n", __func__, index);
			res = rtw_read32(padapter, REG_CR);
			index ++;
			ret = _FAIL;
		}
	}

	if (ret) {
		index = 0;
		while(index < 100) {
			rtw_write32(padapter, 0x1B8, 0x12345678);
			res = rtw_read32(padapter, 0x1B8);
			if (res == 0x12345678) {
				DBG_871X("%s: 0x1B8 test Pass.\n", __func__);
				ret = _SUCCESS;
				break;
			} else {
				index ++;
				DBG_871X("%s: 0x1B8 test Fail(index: %d).\n", __func__, index);
				ret = _FAIL;
			}
		}
	} else {
		DBG_871X("%s: fail at cmd52, cmd53.\n", __func__);
	}
	return ret;
}

#ifdef CONFIG_EXT_CLK
void EnableGpio5ClockReq(PADAPTER Adapter, u8 in_interrupt, u32 Enable)
{
	u32	value32;
	HAL_DATA_TYPE	*pHalData;

	pHalData = GET_HAL_DATA(Adapter);
	if(IS_D_CUT(pHalData->VersionID))
		return;

	//dbgdump("%s Enable:%x time:%d", __RTL_FUNC__, Enable, rtw_get_current_time());

	if(in_interrupt)
		value32 = _sdio_read32(Adapter, REG_GPIO_PIN_CTRL);
	else
    		value32 = rtw_read32(Adapter, REG_GPIO_PIN_CTRL);

	//open GPIO 5
	if (Enable)
		value32 |= BIT(13);//5+8
	else
		value32 &= ~BIT(13);

	//GPIO 5 out put
	value32 |= BIT(21);//5+16

	//if (Enable)
	//	rtw_write8(Adapter, REG_GPIO_PIN_CTRL + 1, 0x20);
	//else
	//	rtw_write8(Adapter, REG_GPIO_PIN_CTRL + 1, 0x00);

	if(in_interrupt)
		_sdio_write32(Adapter, REG_GPIO_PIN_CTRL, value32);
	else
		rtw_write32(Adapter, REG_GPIO_PIN_CTRL, value32);

} //end of _rtl8192cs_disable_gpio()

void _InitClockTo26MHz(
	IN	PADAPTER Adapter
	)
{
	u8 u1temp = 0;
	HAL_DATA_TYPE	*pHalData;

	pHalData = GET_HAL_DATA(Adapter);

	if(IS_D_CUT(pHalData->VersionID)) {
		//FW special init
		u1temp =  rtw_read8(Adapter, REG_XCK_OUT_CTRL);
		u1temp |= 0x18;
		rtw_write8(Adapter, REG_XCK_OUT_CTRL, u1temp);
		MSG_8192C("D cut version\n");
	}

	EnableGpio5ClockReq(Adapter, _FALSE, 1);

	//0x2c[3:0] = 5 will set clock to 26MHz
	u1temp = rtw_read8(Adapter, REG_APE_PLL_CTRL_EXT);
	u1temp = (u1temp & 0xF0) | 0x05;
	rtw_write8(Adapter, REG_APE_PLL_CTRL_EXT, u1temp);
}
#endif


static void rtl8188es_interface_configure(PADAPTER padapter)
{
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(padapter);
	struct dvobj_priv		*pdvobjpriv = adapter_to_dvobj(padapter);
	struct registry_priv	*pregistrypriv = &padapter->registrypriv;
	BOOLEAN		bWiFiConfig	= pregistrypriv->wifi_spec;


	pdvobjpriv->RtOutPipe[0] = WLAN_TX_HIQ_DEVICE_ID;
	pdvobjpriv->RtOutPipe[1] = WLAN_TX_MIQ_DEVICE_ID;
	pdvobjpriv->RtOutPipe[2] = WLAN_TX_LOQ_DEVICE_ID;

	if (bWiFiConfig)
		pHalData->OutEpNumber = 2;
	else
		pHalData->OutEpNumber = SDIO_MAX_TX_QUEUE;

	switch(pHalData->OutEpNumber){
		case 3:
			pHalData->OutEpQueueSel=TX_SELE_HQ| TX_SELE_LQ|TX_SELE_NQ;
			break;
		case 2:
			pHalData->OutEpQueueSel=TX_SELE_HQ| TX_SELE_NQ;
			break;
		case 1:
			pHalData->OutEpQueueSel=TX_SELE_HQ;
			break;
		default:				
			break;
	}

	Hal_MappingOutPipe(padapter, pHalData->OutEpNumber);
}

/*
 * Description:
 *	Call power on sequence to enable card
 *
 * Return:
 *	_SUCCESS	enable success
 *	_FAIL		enable fail
 */
static u8 _CardEnable(PADAPTER padapter)
{
	u8 bMacPwrCtrlOn;
	u8 ret;

	DBG_871X("=>%s\n", __FUNCTION__);

	rtw_hal_get_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	if (bMacPwrCtrlOn == _FALSE)
	{
#ifdef CONFIG_PLATFORM_SPRD
		u8 val8;
#endif // CONFIG_PLATFORM_SPRD

		// RSV_CTRL 0x1C[7:0] = 0x00
		// unlock ISO/CLK/Power control register
		rtw_write8(padapter, REG_RSV_CTRL, 0x0);

#ifdef CONFIG_EXT_CLK
		_InitClockTo26MHz(padapter);
#endif //CONFIG_EXT_CLK

#ifdef CONFIG_PLATFORM_SPRD
		val8 =  rtw_read8(padapter, 0x4);
		val8 = val8 & ~BIT(5);
		rtw_write8(padapter, 0x4, val8);
#endif // CONFIG_PLATFORM_SPRD

		ret = HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, Rtl8188E_NIC_ENABLE_FLOW);
		if (ret == _SUCCESS) {
			u8 bMacPwrCtrlOn = _TRUE;
			rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
		}
		else
		{
			DBG_871X(KERN_ERR "%s: run power on flow fail\n", __func__);
			return _FAIL;	
		}
		
	} 
	else
	{
		
		DBG_871X("=>%s bMacPwrCtrlOn == _TRUE do nothing !!\n", __FUNCTION__);	
		ret = _SUCCESS;
	}

	DBG_871X("<=%s\n", __FUNCTION__);

	return ret;
	
}

static u32 _InitPowerOn_8188ES(PADAPTER padapter)
{
	u8 value8;
	u16 value16;
	u32 value32;
	u8 ret;

	DBG_871X("=>%s\n", __FUNCTION__);

	ret = _CardEnable(padapter);
	if (ret == _FAIL) {
		return ret;
	}

/*
	// Radio-Off Pin Trigger
	value8 = rtw_read8(padapter, REG_GPIO_INTM+1);
	value8 |= BIT(1); // Enable falling edge triggering interrupt
	rtw_write8(padapter, REG_GPIO_INTM+1, value8);
	value8 = rtw_read8(padapter, REG_GPIO_IO_SEL_2+1);
	value8 |= BIT(1);
	rtw_write8(padapter, REG_GPIO_IO_SEL_2+1, value8);
*/

	// Enable power down and GPIO interrupt
	value16 = rtw_read16(padapter, REG_APS_FSMCO);
	value16 |= EnPDN; // Enable HW power down and RF on
	rtw_write16(padapter, REG_APS_FSMCO, value16);


	// Enable MAC DMA/WMAC/SCHEDULE/SEC block
	value16 = rtw_read16(padapter, REG_CR);
	value16 |= (HCI_TXDMA_EN | HCI_RXDMA_EN | TXDMA_EN | RXDMA_EN
				| PROTOCOL_EN | SCHEDULE_EN | ENSEC | CALTMR_EN);
	// for SDIO - Set CR bit10 to enable 32k calibration. Suggested by SD1 Gimmy. Added by tynli. 2011.08.31.
	
	rtw_write16(padapter, REG_CR, value16);



	// Enable CMD53 R/W Operation
//	bMacPwrCtrlOn = TRUE;
//	rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, (pu8)(&bMacPwrCtrlOn));

	DBG_871X("<=%s\n", __FUNCTION__);
	
	return _SUCCESS;
	
}

//Tx Page FIFO threshold
static void _init_available_page_threshold(PADAPTER padapter, u8 numHQ, u8 numNQ, u8 numLQ, u8 numPubQ)
{
	u16	HQ_threshold, NQ_threshold, LQ_threshold;

	HQ_threshold = (numPubQ + numHQ + 1) >> 1;
	HQ_threshold |= (HQ_threshold<<8);

	NQ_threshold = (numPubQ + numNQ + 1) >> 1;
	NQ_threshold |= (NQ_threshold<<8);

	LQ_threshold = (numPubQ + numLQ + 1) >> 1;
	LQ_threshold |= (LQ_threshold<<8);

	rtw_write16(padapter, 0x218, HQ_threshold);
	rtw_write16(padapter, 0x21A, NQ_threshold);
	rtw_write16(padapter, 0x21C, LQ_threshold);
	DBG_8192C("%s(): Enable Tx FIFO Page Threshold H:0x%x,N:0x%x,L:0x%x\n", __FUNCTION__, HQ_threshold, NQ_threshold, LQ_threshold);
}

static void _InitQueueReservedPage(PADAPTER padapter)
{
#ifdef RTL8188ES_MAC_LOOPBACK	

//#define MAC_LOOPBACK_PAGE_NUM_PUBQ		0x26
//#define MAC_LOOPBACK_PAGE_NUM_HPQ		0x0b
//#define MAC_LOOPBACK_PAGE_NUM_LPQ		0x0b
//#define MAC_LOOPBACK_PAGE_NUM_NPQ		0x0b // 71 pages=>9088 bytes, 8.875k

	rtw_write16(padapter, REG_RQPN_NPQ, 0x0b0b);
	rtw_write32(padapter, REG_RQPN, 0x80260b0b);
	
#else //TX_PAGE_BOUNDARY_LOOPBACK_MODE
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(padapter);
	struct registry_priv	*pregistrypriv = &padapter->registrypriv;
	u32			outEPNum	= (u32)pHalData->OutEpNumber;
	u32			numHQ		= NORMAL_PAGE_NUM_HPQ_88E;
	u32			numLQ		= NORMAL_PAGE_NUM_LPQ_88E;
	u32			numNQ		= NORMAL_PAGE_NUM_NPQ_88E;
	u32			numPubQ	= 0x00;
	u32			value32;
	u8			value8;
	BOOLEAN			bWiFiConfig	= pregistrypriv->wifi_spec;

	if(bWiFiConfig)
	{
		if (pHalData->OutEpQueueSel & TX_SELE_HQ)
		{
			numHQ =  WMM_NORMAL_PAGE_NUM_HPQ_88E;
		}

		if (pHalData->OutEpQueueSel & TX_SELE_LQ)
		{
			numLQ = WMM_NORMAL_PAGE_NUM_LPQ_88E;
		}

		// NOTE: This step shall be proceed before writting REG_RQPN.
		if (pHalData->OutEpQueueSel & TX_SELE_NQ) {
			numNQ = WMM_NORMAL_PAGE_NUM_NPQ_88E;
		}
	}

	value8 = (u8)_NPQ(numNQ);
	rtw_write8(padapter, REG_RQPN_NPQ, value8);

	numPubQ = TX_TOTAL_PAGE_NUMBER_88E - numHQ - numLQ - numNQ;

	// TX DMA
	value32 = _HPQ(numHQ) | _LPQ(numLQ) | _PUBQ(numPubQ) | LD_RQPN;
	rtw_write32(padapter, REG_RQPN, value32);

	rtw_hal_set_sdio_tx_max_length(padapter, numHQ, numNQ, numLQ, numPubQ);

#ifdef CONFIG_SDIO_TX_ENABLE_AVAL_INT
	_init_available_page_threshold(padapter, numHQ, numNQ, numLQ, numPubQ);
#endif
#endif
	return;	
}

static void _InitTxBufferBoundary(PADAPTER padapter, u8 txpktbuf_bndy)
{
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
	//HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);

	//u16	txdmactrl;

	rtw_write8(padapter, REG_BCNQ_BDNY, txpktbuf_bndy);
	rtw_write8(padapter, REG_MGQ_BDNY, txpktbuf_bndy);
	rtw_write8(padapter, REG_WMAC_LBK_BF_HD, txpktbuf_bndy);
	rtw_write8(padapter, REG_TRXFF_BNDY, txpktbuf_bndy);
	rtw_write8(padapter, REG_TDECTRL+1, txpktbuf_bndy);

}

static VOID
_InitNormalChipRegPriority(
	IN	PADAPTER	Adapter,
	IN	u16		beQ,
	IN	u16		bkQ,
	IN	u16		viQ,
	IN	u16		voQ,
	IN	u16		mgtQ,
	IN	u16		hiQ
	)
{	
	u16 value16	= (rtw_read16(Adapter, REG_TRXDMA_CTRL) & 0x7);

	value16 |=	_TXDMA_BEQ_MAP(beQ) 	| _TXDMA_BKQ_MAP(bkQ) |
				_TXDMA_VIQ_MAP(viQ) 	| _TXDMA_VOQ_MAP(voQ) |
				_TXDMA_MGQ_MAP(mgtQ)| _TXDMA_HIQ_MAP(hiQ);

	rtw_write16(Adapter, REG_TRXDMA_CTRL, value16);
}

static VOID
_InitNormalChipOneOutEpPriority(
	IN	PADAPTER Adapter
	)
{
	HAL_DATA_TYPE	*pHalData	= GET_HAL_DATA(Adapter);

	u16	value = 0;
	switch(pHalData->OutEpQueueSel)
	{
		case TX_SELE_HQ:
			value = QUEUE_HIGH;
			break;
		case TX_SELE_LQ:
			value = QUEUE_LOW;
			break;
		case TX_SELE_NQ:
			value = QUEUE_NORMAL;
			break;
		default:
			//RT_ASSERT(FALSE,("Shall not reach here!\n"));
			break;
	}

	_InitNormalChipRegPriority(Adapter,
								value,
								value,
								value,
								value,
								value,
								value
								);

}

static VOID
_InitNormalChipTwoOutEpPriority(
	IN	PADAPTER Adapter
	)
{
	HAL_DATA_TYPE	*pHalData	= GET_HAL_DATA(Adapter);
	struct registry_priv *pregistrypriv = &Adapter->registrypriv;
	u16			beQ,bkQ,viQ,voQ,mgtQ,hiQ;


	u16	valueHi = 0;
	u16	valueLow = 0;

	switch(pHalData->OutEpQueueSel)
	{
		case (TX_SELE_HQ | TX_SELE_LQ):
			valueHi = QUEUE_HIGH;
			valueLow = QUEUE_LOW;
			break;
		case (TX_SELE_NQ | TX_SELE_LQ):
			valueHi = QUEUE_NORMAL;
			valueLow = QUEUE_LOW;
			break;
		case (TX_SELE_HQ | TX_SELE_NQ):
			valueHi = QUEUE_HIGH;
			valueLow = QUEUE_NORMAL;
			break;
		default:
			//RT_ASSERT(FALSE,("Shall not reach here!\n"));
			break;
	}

	if(!pregistrypriv->wifi_spec ){
		beQ		= valueLow;
		bkQ		= valueLow;
		viQ		= valueHi;
		voQ		= valueHi;
		mgtQ	= valueHi;
		hiQ		= valueHi;
	}
	else{//for WMM ,CONFIG_OUT_EP_WIFI_MODE
		beQ		= valueLow;
		bkQ		= valueHi;
		viQ		= valueHi;
		voQ		= valueLow;
		mgtQ	= valueHi;
		hiQ		= valueHi;
	}

	_InitNormalChipRegPriority(Adapter,beQ,bkQ,viQ,voQ,mgtQ,hiQ);

}

static VOID
_InitNormalChipThreeOutEpPriority(
	IN	PADAPTER padapter
	)
{
	struct registry_priv *pregistrypriv = &padapter->registrypriv;
	u16			beQ, bkQ, viQ, voQ, mgtQ, hiQ;

	if (!pregistrypriv->wifi_spec){// typical setting
		beQ		= QUEUE_LOW;
		bkQ 		= QUEUE_LOW;
		viQ 		= QUEUE_NORMAL;
		voQ 		= QUEUE_HIGH;
		mgtQ 	= QUEUE_HIGH;
		hiQ 		= QUEUE_HIGH;
	}
	else {// for WMM
		beQ		= QUEUE_LOW;
		bkQ 		= QUEUE_NORMAL;
		viQ 		= QUEUE_NORMAL;
		voQ 		= QUEUE_HIGH;
		mgtQ 	= QUEUE_HIGH;
		hiQ 		= QUEUE_HIGH;
	}
	_InitNormalChipRegPriority(padapter,beQ,bkQ,viQ,voQ,mgtQ,hiQ);
}

static VOID
_InitNormalChipQueuePriority(
	IN	PADAPTER Adapter
	)
{
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(Adapter);

	switch(pHalData->OutEpNumber)
	{
		case 1:
			_InitNormalChipOneOutEpPriority(Adapter);
			break;
		case 2:
			_InitNormalChipTwoOutEpPriority(Adapter);
			break;
		case 3:
			_InitNormalChipThreeOutEpPriority(Adapter);
			break;
		default:
			//RT_ASSERT(FALSE,("Shall not reach here!\n"));
			break;
	}


}


static void _InitQueuePriority(PADAPTER padapter)
{
	_InitNormalChipQueuePriority(padapter);
}

static void _InitPageBoundary(PADAPTER padapter)
{
	// RX Page Boundary	
	u16 rxff_bndy = MAX_RX_DMA_BUFFER_SIZE_88E-1;

	rtw_write16(padapter, (REG_TRXFF_BNDY + 2), rxff_bndy);

}

void _InitDriverInfoSize(PADAPTER padapter, u8 drvInfoSize)
{
	rtw_write8(padapter, REG_RX_DRVINFO_SZ, drvInfoSize);
}

void _InitNetworkType(PADAPTER padapter)
{
	u32 value32;

	value32 = rtw_read32(padapter, REG_CR);

	// TODO: use the other function to set network type
//	value32 = (value32 & ~MASK_NETTYPE) | _NETTYPE(NT_LINK_AD_HOC);
	value32 = (value32 & ~MASK_NETTYPE) | _NETTYPE(NT_LINK_AP);

	rtw_write32(padapter, REG_CR, value32);
}

void _InitWMACSetting(PADAPTER padapter)
{
	u16 value16;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);


	//pHalData->ReceiveConfig = RCR_AAP | RCR_APM | RCR_AM | RCR_AB | RCR_CBSSID_DATA | RCR_CBSSID_BCN | RCR_AMF | RCR_HTC_LOC_CTRL | RCR_APP_PHYST_RXFF | RCR_APP_ICV | RCR_APP_MIC;
	// don't turn on AAP, it will allow all packets to driver
	pHalData->ReceiveConfig = RCR_APM | RCR_AM | RCR_AB | RCR_CBSSID_DATA | RCR_CBSSID_BCN | RCR_AMF | RCR_HTC_LOC_CTRL | RCR_APP_PHYST_RXFF | RCR_APP_ICV | RCR_APP_MIC;

	rtw_write32(padapter, REG_RCR, pHalData->ReceiveConfig);

	// Accept all data frames
	value16 = 0xFFFF;
	rtw_write16(padapter, REG_RXFLTMAP2, value16);

	// 2010.09.08 hpfan
	// Since ADF is removed from RCR, ps-poll will not be indicate to driver,
	// RxFilterMap should mask ps-poll to gurantee AP mode can rx ps-poll.
	value16 = 0x400;
	rtw_write16(padapter, REG_RXFLTMAP1, value16);

	// Accept all management frames
	value16 = 0xFFFF;
	rtw_write16(padapter, REG_RXFLTMAP0, value16);
	
}

void _InitAdaptiveCtrl(PADAPTER padapter)
{
	u16	value16;
	u32	value32;

	// Response Rate Set
	value32 = rtw_read32(padapter, REG_RRSR);
	value32 &= ~RATE_BITMAP_ALL;
	value32 |= RATE_RRSR_CCK_ONLY_1M;
	rtw_write32(padapter, REG_RRSR, value32);

	// CF-END Threshold
	//m_spIoBase->rtw_write8(REG_CFEND_TH, 0x1);

	// SIFS (used in NAV)
	value16 = _SPEC_SIFS_CCK(0x10) | _SPEC_SIFS_OFDM(0x10);
	rtw_write16(padapter, REG_SPEC_SIFS, value16);

	// Retry Limit
	value16 = _LRL(0x30) | _SRL(0x30);
	rtw_write16(padapter, REG_RL, value16);
}	

void _InitEDCA(PADAPTER padapter)
{
	// Set Spec SIFS (used in NAV)
	rtw_write16(padapter, REG_SPEC_SIFS, 0x100a);
	rtw_write16(padapter, REG_MAC_SPEC_SIFS, 0x100a);

	// Set SIFS for CCK
	rtw_write16(padapter, REG_SIFS_CTX, 0x100a);

	// Set SIFS for OFDM
	rtw_write16(padapter, REG_SIFS_TRX, 0x100a);

	// TXOP
	rtw_write32(padapter, REG_EDCA_BE_PARAM, 0x005EA42B);
	rtw_write32(padapter, REG_EDCA_BK_PARAM, 0x0000A44F);
	rtw_write32(padapter, REG_EDCA_VI_PARAM, 0x005EA324);
	rtw_write32(padapter, REG_EDCA_VO_PARAM, 0x002FA226);
}

void _InitRateFallback(PADAPTER padapter)
{
	// Set Data Auto Rate Fallback Retry Count register.
	rtw_write32(padapter, REG_DARFRC, 0x00000000);
	rtw_write32(padapter, REG_DARFRC+4, 0x10080404);
	rtw_write32(padapter, REG_RARFRC, 0x04030201);
	rtw_write32(padapter, REG_RARFRC+4, 0x08070605);

}

void _InitRetryFunction(PADAPTER padapter)
{
	u8	value8;

	value8 = rtw_read8(padapter, REG_FWHW_TXQ_CTRL);
	value8 |= EN_AMPDU_RTY_NEW;
	rtw_write8(padapter, REG_FWHW_TXQ_CTRL, value8);

	// Set ACK timeout
	rtw_write8(padapter, REG_ACKTO, 0x40);
}

static void HalRxAggr8188ESdio(PADAPTER padapter)
{
#if 1
	struct registry_priv *pregistrypriv;
	u8	valueDMATimeout;
	u8	valueDMAPageCount;


	pregistrypriv = &padapter->registrypriv;

	if (pregistrypriv->wifi_spec)
	{
		// 2010.04.27 hpfan
		// Adjust RxAggrTimeout to close to zero disable RxAggr, suggested by designer
		// Timeout value is calculated by 34 / (2^n)
		valueDMATimeout = 0x0f;
		valueDMAPageCount = 0x01;
	}
	else
	{
		valueDMATimeout = 0x06;
		//valueDMAPageCount = 0x0F;
		//valueDMATimeout = 0x0a;  
		valueDMAPageCount = 0x24;
	}

	rtw_write8(padapter, REG_RXDMA_AGG_PG_TH+1, valueDMATimeout);
	rtw_write8(padapter, REG_RXDMA_AGG_PG_TH, valueDMAPageCount);
#endif
}

void sdio_AggSettingRxUpdate(PADAPTER padapter)
{
#if 1
	//HAL_DATA_TYPE *pHalData;
	u8 valueDMA;


	//pHalData = GET_HAL_DATA(padapter);

	valueDMA = rtw_read8(padapter, REG_TRXDMA_CTRL);
	valueDMA |= RXDMA_AGG_EN;
	rtw_write8(padapter, REG_TRXDMA_CTRL, valueDMA);

#if 0
	switch (RX_PAGE_SIZE_REG_VALUE)
	{
		case PBP_64:
			pHalData->HwRxPageSize = 64;
			break;
		case PBP_128:
			pHalData->HwRxPageSize = 128;
			break;
		case PBP_256:
			pHalData->HwRxPageSize = 256;
			break;
		case PBP_512:
			pHalData->HwRxPageSize = 512;
			break;
		case PBP_1024:
			pHalData->HwRxPageSize = 1024;
			break;
		default:
			RT_TRACE(_module_hci_hal_init_c_, _drv_err_,
				("%s: RX_PAGE_SIZE_REG_VALUE definition is incorrect!\n", __FUNCTION__));
			break;
	}
#endif
#endif
}

void _initSdioAggregationSetting(PADAPTER padapter)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);

	// Tx aggregation setting
	//sdio_AggSettingTxUpdate(padapter);

	// Rx aggregation setting
	HalRxAggr8188ESdio(padapter);
	sdio_AggSettingRxUpdate(padapter);

	// 201/12/10 MH Add for USB agg mode dynamic switch.
	pHalData->UsbRxHighSpeedMode = _FALSE;
}


void _InitOperationMode(PADAPTER padapter)
{
	PHAL_DATA_TYPE pHalData;
	struct mlme_ext_priv *pmlmeext;
	u8				regBwOpMode = 0;
	u32				regRATR = 0, regRRSR = 0;
	u8				MinSpaceCfg = 0;


	pHalData = GET_HAL_DATA(padapter);
	pmlmeext = &padapter->mlmeextpriv;

	//1 This part need to modified according to the rate set we filtered!!
	//
	// Set RRSR, RATR, and REG_BWOPMODE registers
	//
	switch(pmlmeext->cur_wireless_mode)
	{
		case WIRELESS_MODE_B:
			regBwOpMode = BW_OPMODE_20MHZ;
			regRATR = RATE_ALL_CCK;
			regRRSR = RATE_ALL_CCK;
			break;
		case WIRELESS_MODE_A:
//			RT_ASSERT(FALSE,("Error wireless a mode\n"));
#if 0
			regBwOpMode = BW_OPMODE_5G |BW_OPMODE_20MHZ;
			regRATR = RATE_ALL_OFDM_AG;
			regRRSR = RATE_ALL_OFDM_AG;
#endif
			break;
		case WIRELESS_MODE_G:
			regBwOpMode = BW_OPMODE_20MHZ;
			regRATR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
			regRRSR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
			break;
		case WIRELESS_MODE_AUTO:
#if 0
			if (padapter->bInHctTest)
			{
				regBwOpMode = BW_OPMODE_20MHZ;
				regRATR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
				regRRSR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
			}
			else
#endif
			{
				regBwOpMode = BW_OPMODE_20MHZ;
				regRATR = RATE_ALL_CCK | RATE_ALL_OFDM_AG | RATE_ALL_OFDM_1SS | RATE_ALL_OFDM_2SS;
				regRRSR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
			}
			break;
		case WIRELESS_MODE_N_24G:
			// It support CCK rate by default.
			// CCK rate will be filtered out only when associated AP does not support it.
			regBwOpMode = BW_OPMODE_20MHZ;
			regRATR = RATE_ALL_CCK | RATE_ALL_OFDM_AG | RATE_ALL_OFDM_1SS | RATE_ALL_OFDM_2SS;
			regRRSR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
			break;
		case WIRELESS_MODE_N_5G:
//			RT_ASSERT(FALSE,("Error wireless mode"));
#if 0
			regBwOpMode = BW_OPMODE_5G;
			regRATR = RATE_ALL_OFDM_AG | RATE_ALL_OFDM_1SS | RATE_ALL_OFDM_2SS;
			regRRSR = RATE_ALL_OFDM_AG;
#endif
			break;

		default: //for MacOSX compiler warning.
			break;
	}

	rtw_write8(padapter, REG_BWOPMODE, regBwOpMode);

}


void _InitBeaconParameters(PADAPTER padapter)
{
	PHAL_DATA_TYPE pHalData;


	pHalData = GET_HAL_DATA(padapter);

	rtw_write16(padapter, REG_BCN_CTRL, 0x1010);

	// TODO: Remove these magic number
	rtw_write16(padapter, REG_TBTT_PROHIBIT, 0x6404);// ms
	rtw_write8(padapter, REG_DRVERLYINT, DRIVER_EARLY_INT_TIME_8188E);//ms
	rtw_write8(padapter, REG_BCNDMATIM, BCN_DMA_ATIME_INT_TIME_8188E);

	// Suggested by designer timchen. Change beacon AIFS to the largest number
	// beacause test chip does not contension before sending beacon. by tynli. 2009.11.03
	rtw_write16(padapter, REG_BCNTCFG, 0x660F);

	
	pHalData->RegBcnCtrlVal = rtw_read8(padapter, REG_BCN_CTRL);
	pHalData->RegTxPause = rtw_read8(padapter, REG_TXPAUSE); 
	pHalData->RegFwHwTxQCtrl = rtw_read8(padapter, REG_FWHW_TXQ_CTRL+2);
	pHalData->RegReg542 = rtw_read8(padapter, REG_TBTT_PROHIBIT+2);
	pHalData->RegCR_1 = rtw_read8(padapter, REG_CR+1);
	
}

void _InitBeaconMaxError(PADAPTER padapter, BOOLEAN InfraMode)
{
#ifdef RTL8192CU_ADHOC_WORKAROUND_SETTING
	rtw_write8(padapter, REG_BCN_MAX_ERR, 0xFF);
#endif
}

void _InitInterrupt(PADAPTER padapter)
{

	//HISR write one to clear
	rtw_write32(padapter, REG_HISR_88E, 0xFFFFFFFF);
	
	// HIMR - turn all off
	rtw_write32(padapter, REG_HIMR_88E, 0);

	//
	// Initialize and enable SDIO Host Interrupt.
	//
	InitInterrupt8188ESdio(padapter);


	//
	// Initialize and enable system Host Interrupt.
	//
	//InitSysInterrupt8188ESdio(Adapter);//TODO:
	
	//
	// Enable SDIO Host Interrupt.
	//
	//EnableInterrupt8188ESdio(padapter);//Move to sd_intf_start()/stop
	
}

void _InitRDGSetting(PADAPTER padapter)
{
	rtw_write8(padapter, REG_RD_CTRL, 0xFF);
	rtw_write16(padapter, REG_RD_NAV_NXT, 0x200);
	rtw_write8(padapter, REG_RD_RESP_PKT_TH, 0x05);
}


static void _InitRxSetting(PADAPTER padapter)
{
	rtw_write32(padapter, REG_MACID, 0x87654321);
	rtw_write32(padapter, 0x0700, 0x87654321);
}


static void _InitRFType(PADAPTER padapter)
{
	struct registry_priv *pregpriv = &padapter->registrypriv;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	//BOOLEAN is92CU = IS_92C_SERIAL(pHalData->VersionID);
	BOOLEAN	 is2T2R = IS_2T2R(pHalData->VersionID);

#if	DISABLE_BB_RF
	pHalData->rf_chip	= RF_PSEUDO_11N;
	return;
#endif

	pHalData->rf_chip	= RF_6052;

	//if (_FALSE == is92CU) {
	if(_FALSE == is2T2R){
		pHalData->rf_type = RF_1T1R;
		DBG_8192C("Set RF Chip ID to RF_6052 and RF type to 1T1R.\n");
		return;
	}

	// TODO: Consider that EEPROM set 92CU to 1T1R later.
	// Force to overwrite setting according to chip version. Ignore EEPROM setting.
	//pHalData->RF_Type = is92CU ? RF_2T2R : RF_1T1R;
	MSG_8192C("Set RF Chip ID to RF_6052 and RF type to %d.\n", pHalData->rf_type);
}

// Set CCK and OFDM Block "ON"
static void _BBTurnOnBlock(PADAPTER padapter)
{
#if (DISABLE_BB_RF)
	return;
#endif

	PHY_SetBBReg(padapter, rFPGA0_RFMOD, bCCKEn, 0x1);
	PHY_SetBBReg(padapter, rFPGA0_RFMOD, bOFDMEn, 0x1);
}

#if 0
static void _InitAntenna_Selection(PADAPTER padapter)
{
	rtw_write8(padapter, REG_LEDCFG2, 0x82);
}
#endif

static void _InitPABias(PADAPTER padapter)
{
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(padapter);
	u8			pa_setting;
	BOOLEAN		is92C = IS_92C_SERIAL(pHalData->VersionID);

	//FIXED PA current issue
	//efuse_one_byte_read(padapter, 0x1FA, &pa_setting);
	pa_setting = EFUSE_Read1Byte(padapter, 0x1FA);

	//RT_TRACE(COMP_INIT, DBG_LOUD, ("_InitPABias 0x1FA 0x%x \n",pa_setting));

	if(!(pa_setting & BIT0))
	{
		PHY_SetRFReg(padapter, RF_PATH_A, 0x15, 0x0FFFFF, 0x0F406);
		PHY_SetRFReg(padapter, RF_PATH_A, 0x15, 0x0FFFFF, 0x4F406);
		PHY_SetRFReg(padapter, RF_PATH_A, 0x15, 0x0FFFFF, 0x8F406);
		PHY_SetRFReg(padapter, RF_PATH_A, 0x15, 0x0FFFFF, 0xCF406);
		//RT_TRACE(COMP_INIT, DBG_LOUD, ("PA BIAS path A\n"));
	}

	if(!(pa_setting & BIT1) && is92C)
	{
		PHY_SetRFReg(padapter,RF_PATH_B, 0x15, 0x0FFFFF, 0x0F406);
		PHY_SetRFReg(padapter,RF_PATH_B, 0x15, 0x0FFFFF, 0x4F406);
		PHY_SetRFReg(padapter,RF_PATH_B, 0x15, 0x0FFFFF, 0x8F406);
		PHY_SetRFReg(padapter,RF_PATH_B, 0x15, 0x0FFFFF, 0xCF406);
		//RT_TRACE(COMP_INIT, DBG_LOUD, ("PA BIAS path B\n"));
	}

	if(!(pa_setting & BIT4))
	{
		pa_setting = rtw_read8(padapter, 0x16);
		pa_setting &= 0x0F;
		rtw_write8(padapter, 0x16, pa_setting | 0x80);
		rtw_write8(padapter, 0x16, pa_setting | 0x90);
	}
}

#if 0
VOID
_InitRDGSetting_8188E(
	IN	PADAPTER Adapter
	)
{
	PlatformEFIOWrite1Byte(Adapter,REG_RD_CTRL,0xFF);
	PlatformEFIOWrite2Byte(Adapter, REG_RD_NAV_NXT, 0x200);
	PlatformEFIOWrite1Byte(Adapter,REG_RD_RESP_PKT_TH,0x05);
}
#endif

static u32 rtl8188es_hal_init(PADAPTER padapter)
{
	s32 ret;
	u8	txpktbuf_bndy;
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(padapter);
	struct pwrctrl_priv		*pwrctrlpriv = adapter_to_pwrctl(padapter);
	struct registry_priv	*pregistrypriv = &padapter->registrypriv;
	u8 is92C = IS_92C_SERIAL(pHalData->VersionID);
	rt_rf_power_state	eRfPowerStateToSet;
	u8 value8;
	u16 value16;

	u32 init_start_time = rtw_get_current_time();

#ifdef DBG_HAL_INIT_PROFILING
	enum HAL_INIT_STAGES {
		HAL_INIT_STAGES_BEGIN = 0,
		HAL_INIT_STAGES_INIT_PW_ON,
		HAL_INIT_STAGES_MISC01,
		HAL_INIT_STAGES_DOWNLOAD_FW,
		HAL_INIT_STAGES_MAC,
		HAL_INIT_STAGES_BB,
		HAL_INIT_STAGES_RF,	
		HAL_INIT_STAGES_EFUSE_PATCH,
		HAL_INIT_STAGES_INIT_LLTT,	
		
		HAL_INIT_STAGES_MISC02,
		HAL_INIT_STAGES_TURN_ON_BLOCK,
		HAL_INIT_STAGES_INIT_SECURITY,
		HAL_INIT_STAGES_MISC11,
		HAL_INIT_STAGES_INIT_HAL_DM,
		//HAL_INIT_STAGES_RF_PS,
		HAL_INIT_STAGES_IQK,
		HAL_INIT_STAGES_PW_TRACK,
		HAL_INIT_STAGES_LCK,
		//HAL_INIT_STAGES_MISC21,
		HAL_INIT_STAGES_INIT_PABIAS,		
		//HAL_INIT_STAGES_ANTENNA_SEL,
		HAL_INIT_STAGES_MISC31,
		HAL_INIT_STAGES_END,
		HAL_INIT_STAGES_NUM
	};

	char * hal_init_stages_str[] = {
		"HAL_INIT_STAGES_BEGIN",
		"HAL_INIT_STAGES_INIT_PW_ON",
		"HAL_INIT_STAGES_MISC01",
		"HAL_INIT_STAGES_DOWNLOAD_FW",		
		"HAL_INIT_STAGES_MAC",		
		"HAL_INIT_STAGES_BB",
		"HAL_INIT_STAGES_RF",
		"HAL_INIT_STAGES_EFUSE_PATCH",
		"HAL_INIT_STAGES_INIT_LLTT",
		"HAL_INIT_STAGES_MISC02",		
		"HAL_INIT_STAGES_TURN_ON_BLOCK",
		"HAL_INIT_STAGES_INIT_SECURITY",
		"HAL_INIT_STAGES_MISC11",
		"HAL_INIT_STAGES_INIT_HAL_DM",
		//"HAL_INIT_STAGES_RF_PS",
		"HAL_INIT_STAGES_IQK",
		"HAL_INIT_STAGES_PW_TRACK",
		"HAL_INIT_STAGES_LCK",
		//"HAL_INIT_STAGES_MISC21",
		"HAL_INIT_STAGES_INIT_PABIAS"
		//"HAL_INIT_STAGES_ANTENNA_SEL",
		"HAL_INIT_STAGES_MISC31",
		"HAL_INIT_STAGES_END",
	};	


	int hal_init_profiling_i;
	u32 hal_init_stages_timestamp[HAL_INIT_STAGES_NUM]; //used to record the time of each stage's starting point

	for(hal_init_profiling_i=0;hal_init_profiling_i<HAL_INIT_STAGES_NUM;hal_init_profiling_i++)
		hal_init_stages_timestamp[hal_init_profiling_i]=0;

	#define HAL_INIT_PROFILE_TAG(stage) hal_init_stages_timestamp[(stage)]=rtw_get_current_time();
#else
	#define HAL_INIT_PROFILE_TAG(stage) do {} while(0)
#endif //DBG_HAL_INIT_PROFILING

	DBG_8192C("+rtl8188es_hal_init\n");

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_BEGIN);
	// Disable Interrupt first.
//	rtw_hal_disable_interrupt(padapter);
//	DisableInterrupt8188ESdio(padapter);
	
#ifdef CONFIG_WOWLAN
	if(rtw_read8(padapter, REG_MCUFWDL)&BIT7 &&
		(pwrctrlpriv->wowlan_wake_reason & FWDecisionDisconnect)) {
		u8 reg_val=0;
		DBG_8192C("+Reset Entry+\n");
		rtw_write8(padapter, REG_MCUFWDL, 0x00);
		_8051Reset88E(padapter);
		//reset BB
		reg_val = rtw_read8(padapter, REG_SYS_FUNC_EN);
		reg_val &= ~(BIT(0) | BIT(1));
		rtw_write8(padapter, REG_SYS_FUNC_EN, reg_val);
		//reset RF
		rtw_write8(padapter, REG_RF_CTRL, 0);
		//reset TRX path
		rtw_write16(padapter, REG_CR, 0);
		//reset MAC, Digital Core
		reg_val = rtw_read8(padapter, REG_SYS_FUNC_EN+1);
		reg_val &= ~(BIT(4) | BIT(7));
		rtw_write8(padapter, REG_SYS_FUNC_EN+1, reg_val);
		reg_val = rtw_read8(padapter, REG_SYS_FUNC_EN+1);
		reg_val |= BIT(4) | BIT(7);
		rtw_write8(padapter, REG_SYS_FUNC_EN+1, reg_val);
		DBG_8192C("-Reset Entry-\n");
	}
#endif //CONFIG_WOWLAN

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_INIT_PW_ON);
	ret = _InitPowerOn_8188ES(padapter);
	if (_FAIL == ret) {
		RT_TRACE(_module_hci_hal_init_c_, _drv_err_, ("Failed to init Power On!\n"));
		goto exit;
	}
	
	ret = PowerOnCheck(padapter);
	if (_FAIL == ret ) {
		DBG_871X("Power on Fail! do it again\n");
		ret = _InitPowerOn_8188ES(padapter);
		if (_FAIL == ret) {
			DBG_871X("Failed to init Power On!\n");
			goto exit;
		}
	}
	DBG_871X("Power on ok!\n");


HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_MISC01);
	if (!pregistrypriv->wifi_spec) {
		txpktbuf_bndy = TX_PAGE_BOUNDARY_88E;
	} else {
		// for WMM
		txpktbuf_bndy = WMM_NORMAL_TX_PAGE_BOUNDARY_88E;
	}
	_InitQueueReservedPage(padapter);	
	_InitQueuePriority(padapter);
	_InitPageBoundary(padapter);
	_InitTransferPageSize(padapter);
	
#ifdef CONFIG_IOL_IOREG_CFG
	_InitTxBufferBoundary(padapter, 0);		
#endif
	//
	// Configure SDIO TxRx Control to enable Rx DMA timer masking.
	// 2010.02.24.
	//
	value8 = SdioLocalCmd52Read1Byte(padapter, SDIO_REG_TX_CTRL);
	SdioLocalCmd52Write1Byte(padapter, SDIO_REG_TX_CTRL, 0x02);

	rtw_write8(padapter, SDIO_LOCAL_BASE|SDIO_REG_HRPWM1, 0);
	
HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_DOWNLOAD_FW);
#if (MP_DRIVER == 1)
	if (padapter->registrypriv.mp_mode == 1)
	{
		_InitRxSetting(padapter);
	}
#endif //MP_DRIVER == 1
	{	
#if 0
	padapter->bFWReady = _FALSE; //because no fw for test chip	
	pHalData->fw_ractrl = _FALSE;
#else

	ret = rtl8188e_FirmwareDownload(padapter, _FALSE);

	if (ret != _SUCCESS) {
		DBG_871X("%s: Download Firmware failed!!\n", __FUNCTION__);
		padapter->bFWReady = _FALSE;
		pHalData->fw_ractrl = _FALSE;
		goto exit;
	} else {
		RT_TRACE(_module_hci_hal_init_c_, _drv_info_, ("Initializepadapter8192CSdio(): Download Firmware Success!!\n"));
		padapter->bFWReady = _TRUE;
		pHalData->fw_ractrl = _FALSE;
	}
#endif
	}

	rtl8188e_InitializeFirmwareVars(padapter);

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_MAC);
#if (HAL_MAC_ENABLE == 1)
	ret = PHY_MACConfig8188E(padapter);
	if(ret != _SUCCESS){
//		RT_TRACE(COMP_INIT, DBG_LOUD, ("Initializepadapter8192CSdio(): Fail to configure MAC!!\n"));
		goto exit;
	}
#endif

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_BB);
	//
	//d. Initialize BB related configurations.
	//
#if (HAL_BB_ENABLE == 1)
	ret = PHY_BBConfig8188E(padapter);
	if(ret != _SUCCESS){
//		RT_TRACE(COMP_INIT, DBG_SERIOUS, ("Initializepadapter8192CSdio(): Fail to configure BB!!\n"));
		goto exit;
	}
#endif


HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_RF);

#if (HAL_RF_ENABLE == 1)
	ret = PHY_RFConfig8188E(padapter);

	if(ret != _SUCCESS){
//		RT_TRACE(COMP_INIT, DBG_LOUD, ("Initializepadapter8192CSdio(): Fail to configure RF!!\n"));
		goto exit;
	}
HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_EFUSE_PATCH);
#if defined(CONFIG_IOL_EFUSE_PATCH)	
	ret = rtl8188e_iol_efuse_patch(padapter);
	if(ret != _SUCCESS){
		DBG_871X("%s  rtl8188e_iol_efuse_patch failed \n",__FUNCTION__);
		goto exit;
	}
#endif
	_InitTxBufferBoundary(padapter, txpktbuf_bndy);
HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_INIT_LLTT);
	ret = InitLLTTable(padapter, txpktbuf_bndy);
	if (_SUCCESS != ret) {
		RT_TRACE(_module_hci_hal_init_c_, _drv_err_, ("Failed to init LLT Table!\n"));
		goto exit;
	}

#if (RATE_ADAPTIVE_SUPPORT==1)
	{//Enable TX Report
		//Enable Tx Report Timer   
		value8 = rtw_read8(padapter, REG_TX_RPT_CTRL);
		rtw_write8(padapter,  REG_TX_RPT_CTRL, (value8|BIT1|BIT0));
		//Set MAX RPT MACID
		rtw_write8(padapter,  REG_TX_RPT_CTRL+1, 2);//FOR sta mode ,0: bc/mc ,1:AP
		//Tx RPT Timer. Unit: 32us
		rtw_write16(padapter, REG_TX_RPT_TIME, 0xCdf0);
	}
#endif	

#if 0
	if(pHTInfo->bRDGEnable){
		_InitRDGSetting_8188E(Adapter);
	}
#endif

#ifdef CONFIG_TX_EARLY_MODE	
	if( pHalData->bEarlyModeEnable)
	{
		RT_TRACE(_module_hci_hal_init_c_, _drv_info_,("EarlyMode Enabled!!!\n"));

		value8 = rtw_read8(padapter, REG_EARLY_MODE_CONTROL);
#if RTL8188E_EARLY_MODE_PKT_NUM_10 == 1
		value8 = value8|0x1f;
#else
		value8 = value8|0xf;
#endif
		rtw_write8(padapter, REG_EARLY_MODE_CONTROL, value8);

		rtw_write8(padapter, REG_EARLY_MODE_CONTROL+3, 0x80);

		value8 = rtw_read8(padapter, REG_TCR+1);
		value8 = value8|0x40;
		rtw_write8(padapter,REG_TCR+1, value8);
	}
	else
#endif
	{
		rtw_write8(padapter, REG_EARLY_MODE_CONTROL, 0);
	}


#if(SIC_ENABLE == 1)
	SIC_Init(padapter);
#endif


	if (pwrctrlpriv->reg_rfoff == _TRUE) {
		pwrctrlpriv->rf_pwrstate = rf_off;
	}

	// 2010/08/09 MH We need to check if we need to turnon or off RF after detecting
	// HW GPIO pin. Before PHY_RFConfig8192C.
	HalDetectPwrDownMode88E(padapter);


	// Set RF type for BB/RF configuration
	_InitRFType(padapter);

	// Save target channel
	// <Roger_Notes> Current Channel will be updated again later.
	pHalData->CurrentChannel = 1;



HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_MISC02);
	// Get Rx PHY status in order to report RSSI and others.
	_InitDriverInfoSize(padapter, 4);
	hal_init_macaddr(padapter);
	_InitNetworkType(padapter);
	_InitWMACSetting(padapter);
	_InitAdaptiveCtrl(padapter);
	_InitEDCA(padapter);
	_InitRateFallback(padapter);
	_InitRetryFunction(padapter);
	_initSdioAggregationSetting(padapter);
	_InitOperationMode(padapter);
	_InitBeaconParameters(padapter);
	_InitBeaconMaxError(padapter, _TRUE);
	_InitInterrupt(padapter);
	
	// Enable MACTXEN/MACRXEN block
	value16 = rtw_read16(padapter, REG_CR);
	value16 |= (MACTXEN | MACRXEN);
	rtw_write8(padapter, REG_CR, value16);

	rtw_write32(padapter,REG_MACID_NO_LINK_0,0xFFFFFFFF);
	rtw_write32(padapter,REG_MACID_NO_LINK_1,0xFFFFFFFF);
	
#if defined(CONFIG_CONCURRENT_MODE) || defined(CONFIG_TX_MCAST2UNI)

#ifdef CONFIG_CHECK_AC_LIFETIME
	// Enable lifetime check for the four ACs
	rtw_write8(padapter, REG_LIFETIME_CTRL, 0x0F);
#endif	// CONFIG_CHECK_AC_LIFETIME	
	
#ifdef CONFIG_TX_MCAST2UNI
	rtw_write16(padapter, REG_PKT_VO_VI_LIFE_TIME, 0x0400);	// unit: 256us. 256ms
	rtw_write16(padapter, REG_PKT_BE_BK_LIFE_TIME, 0x0400);	// unit: 256us. 256ms
#else	// CONFIG_TX_MCAST2UNI
	rtw_write16(padapter, REG_PKT_VO_VI_LIFE_TIME, 0x3000);	// unit: 256us. 3s
	rtw_write16(padapter, REG_PKT_BE_BK_LIFE_TIME, 0x3000);	// unit: 256us. 3s
#endif	// CONFIG_TX_MCAST2UNI
#endif	// CONFIG_CONCURRENT_MODE || CONFIG_TX_MCAST2UNI



	
#endif //HAL_RF_ENABLE == 1
	

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_TURN_ON_BLOCK);
	_BBTurnOnBlock(padapter);

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_INIT_SECURITY);
#if 1
	invalidate_cam_all(padapter);
#else
	CamResetAllEntry(padapter);
	padapter->HalFunc.EnableHWSecCfgHandler(padapter);
#endif

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_MISC11);
	// 2010/12/17 MH We need to set TX power according to EFUSE content at first.
	PHY_SetTxPowerLevel8188E(padapter, pHalData->CurrentChannel);
	// Record original value for template. This is arough data, we can only use the data
	// for power adjust. The value can not be adjustde according to different power!!!
//	pHalData->OriginalCckTxPwrIdx = pHalData->CurrentCckTxPwrIdx;
//	pHalData->OriginalOfdm24GTxPwrIdx = pHalData->CurrentOfdm24GTxPwrIdx;

// Move by Neo for USB SS to below setp
//_RfPowerSave(padapter);
#if 0 //ANTENNA_SELECTION_STATIC_SETTING
#if 0
	if (!IS_92C_SERIAL( pHalData->VersionID) && (pHalData->AntDivCfg!=0))
#else
	if (IS_1T1R( pHalData->VersionID) && (pHalData->AntDivCfg!=0))
#endif
	{ //for 88CU ,1T1R
		_InitAntenna_Selection(padapter);
	}
#endif

	//
	// Disable BAR, suggested by Scott
	// 2010.04.09 add by hpfan
	//
	rtw_write32(padapter, REG_BAR_MODE_CTRL, 0x0201ffff);

	// HW SEQ CTRL
	// set 0x0 to 0xFF by tynli. Default enable HW SEQ NUM.
	rtw_write8(padapter, REG_HWSEQ_CTRL, 0xFF);


#ifdef RTL8188ES_MAC_LOOPBACK
	value8 = rtw_read8(padapter, REG_SYS_FUNC_EN);
	value8 &= ~(FEN_BBRSTB|FEN_BB_GLB_RSTn);
	rtw_write8(padapter, REG_SYS_FUNC_EN, value8);//disable BB, CCK/OFDM

	rtw_write8(padapter, REG_RD_CTRL, 0x0F);
	rtw_write8(padapter, REG_RD_CTRL+1, 0xCF);
	//rtw_write8(padapter, REG_TXPKTBUF_WMAC_LBK_BF_HD, 0x80);//to check _InitPageBoundary()
	rtw_write32(padapter, REG_CR, 0x0b0202ff);//0x100[28:24]=0x01011, enable mac loopback, no HW Security Eng.
#endif

	
HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_INIT_HAL_DM);
	//	InitHalDm(padapter);
	rtl8188e_InitHalDm(padapter);
		

#if (MP_DRIVER == 1)
	if (padapter->registrypriv.mp_mode == 1)
	{
		padapter->mppriv.channel = pHalData->CurrentChannel;
		MPT_InitializeAdapter(padapter, padapter->mppriv.channel);
	}
	else
#endif //(MP_DRIVER == 1)
	{
	//
	// 2010/08/11 MH Merge from 8192SE for Minicard init. We need to confirm current radio status
	// and then decide to enable RF or not.!!!??? For Selective suspend mode. We may not
	// call init_adapter. May cause some problem??
	//
	// Fix the bug that Hw/Sw radio off before S3/S4, the RF off action will not be executed
	// in MgntActSet_RF_State() after wake up, because the value of pHalData->eRFPowerState
	// is the same as eRfOff, we should change it to eRfOn after we config RF parameters.
	// Added by tynli. 2010.03.30.
	pwrctrlpriv->rf_pwrstate = rf_on;
	RT_CLEAR_PS_LEVEL(pwrctrlpriv, RT_RF_OFF_LEVL_HALT_NIC);

	// 20100326 Joseph: Copy from GPIOChangeRFWorkItemCallBack() function to check HW radio on/off.
	// 20100329 Joseph: Revise and integrate the HW/SW radio off code in initialization.
//	pHalData->bHwRadioOff = _FALSE;
	pwrctrlpriv->b_hw_radio_off = _FALSE;
	eRfPowerStateToSet = rf_on;

	// 2010/-8/09 MH For power down module, we need to enable register block contrl reg at 0x1c.
	// Then enable power down control bit of register 0x04 BIT4 and BIT15 as 1.
	if(pHalData->pwrdown && eRfPowerStateToSet == rf_off)
	{
		// Enable register area 0x0-0xc.
		rtw_write8(padapter, REG_RSV_CTRL, 0x0);

		//
		// <Roger_Notes> We should configure HW PDn source for WiFi ONLY, and then
		// our HW will be set in power-down mode if PDn source from all  functions are configured.
		// 2010.10.06.
		//
		if(IS_HARDWARE_TYPE_8723AS(padapter))
		{
			value8 = rtw_read8(padapter, REG_MULTI_FUNC_CTRL);
			rtw_write8(padapter, REG_MULTI_FUNC_CTRL, (value8|WL_HWPDN_EN));
		}
		else
		{
			rtw_write16(padapter, REG_APS_FSMCO, 0x8812);
		}
	}
	//DrvIFIndicateCurrentPhyStatus(padapter); // 2010/08/17 MH Disable to prevent BSOD.

	// 2010/08/26 MH Merge from 8192CE.
	if(pwrctrlpriv->rf_pwrstate == rf_on)
	{
	
HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_IQK);
		if(pHalData->odmpriv.RFCalibrateInfo.bIQKInitialized){
//			PHY_IQCalibrate(padapter, _TRUE);
			PHY_IQCalibrate_8188E(padapter,_TRUE);
		}
		else
		{
//			PHY_IQCalibrate(padapter, _FALSE);
			PHY_IQCalibrate_8188E(padapter,_FALSE);
			pHalData->odmpriv.RFCalibrateInfo.bIQKInitialized = _TRUE;
		}

//		dm_CheckTXPowerTracking(padapter);
//		PHY_LCCalibrate(padapter);

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_PW_TRACK);
		ODM_TXPowerTrackingCheck(&pHalData->odmpriv );

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_LCK);
		PHY_LCCalibrate_8188E(&pHalData->odmpriv );

		
	}
}

HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_INIT_PABIAS);
	//if(pHalData->eRFPowerState == eRfOn)
	{
		_InitPABias(padapter);
	}

	// Init BT hw config.
//	HALBT_InitHwConfig(padapter);


HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_MISC31);
	// 2010/05/20 MH We need to init timer after update setting. Otherwise, we can not get correct inf setting.
	// 2010/05/18 MH For SE series only now. Init GPIO detect time
#if 0
	if(pDevice->RegUsbSS)
	{
		RT_TRACE(COMP_INIT, DBG_LOUD, (" call GpioDetectTimerStart\n"));
		GpioDetectTimerStart(padapter);	// Disable temporarily
	}
#endif

	// 2010/08/23 MH According to Alfred's suggestion, we need to to prevent HW enter
	// suspend mode automatically.
	//HwSuspendModeEnable92Cu(padapter, FALSE);

	// 2010/12/17 MH For TX power level OID modification from UI.
//	padapter->HalFunc.GetTxPowerLevelHandler( padapter, &pHalData->DefaultTxPwrDbm );
	//DbgPrint("pHalData->DefaultTxPwrDbm = %d\n", pHalData->DefaultTxPwrDbm);

//	if(pHalData->SwBeaconType < HAL92CSDIO_DEFAULT_BEACON_TYPE) // The lowest Beacon Type that HW can support
//		pHalData->SwBeaconType = HAL92CSDIO_DEFAULT_BEACON_TYPE;

	//
	// Update current Tx FIFO page status.
	//
	HalQueryTxBufferStatus8189ESdio(padapter);
	HalQueryTxOQTBufferStatus8189ESdio(padapter);
	pHalData->SdioTxOQTMaxFreeSpace = pHalData->SdioTxOQTFreeSpace;


	if(pregistrypriv->wifi_spec)
		rtw_write16(padapter,REG_FAST_EDCA_CTRL ,0);


	//TODO:Setting HW_VAR_NAV_UPPER !!!!!!!!!!!!!!!!!!!!
	//rtw_hal_set_hwreg(Adapter, HW_VAR_NAV_UPPER, ((pu1Byte)&NavUpper));

	if(IS_HARDWARE_TYPE_8188ES(padapter))
	{
		value8= rtw_read8(padapter, 0x4d3);
		rtw_write8(padapter, 0x4d3, (value8|0x1));		
	}

	//pHalData->PreRpwmVal = PlatformEFSdioLocalCmd52Read1Byte(Adapter, SDIO_REG_HRPWM1)&0x80;


	// enable Tx report.
	rtw_write8(padapter,  REG_FWHW_TXQ_CTRL+1, 0x0F);
/*
	// Suggested by SD1 pisa. Added by tynli. 2011.10.21.
	PlatformEFIOWrite1Byte(Adapter, REG_EARLY_MODE_CONTROL+3, 0x01);

*/	//tynli_test_tx_report.
	rtw_write16(padapter, REG_TX_RPT_TIME, 0x3DF0);
	//RT_TRACE(COMP_INIT, DBG_TRACE, ("InitializeAdapter8188EUsb() <====\n"));

	
	//enable tx DMA to drop the redundate data of packet
	rtw_write16(padapter,REG_TXDMA_OFFSET_CHK, (rtw_read16(padapter,REG_TXDMA_OFFSET_CHK) | DROP_DATA_EN));

//#debug print for checking compile flags
	//DBG_8192C("RTL8188E_FPGA_TRUE_PHY_VERIFICATION=%d\n", RTL8188E_FPGA_TRUE_PHY_VERIFICATION);
	DBG_8192C("DISABLE_BB_RF=%d\n", DISABLE_BB_RF);	
	DBG_8192C("IS_HARDWARE_TYPE_8188ES=%d\n", IS_HARDWARE_TYPE_8188ES(padapter));
//#

#ifdef CONFIG_PLATFORM_SPRD
	// For Power Consumption, set all GPIO pin to ouput mode
	//0x44~0x47 (GPIO 0~7), Note:GPIO5 is enabled for controlling external 26MHz request
	rtw_write8(padapter, GPIO_IO_SEL, 0xFF);//Reg0x46, set to o/p mode

       //0x42~0x43 (GPIO 8~11)
	value8 = rtw_read8(padapter, REG_GPIO_IO_SEL);
	rtw_write8(padapter, REG_GPIO_IO_SEL, (value8<<4)|value8);
	value8 = rtw_read8(padapter, REG_GPIO_IO_SEL+1);
	rtw_write8(padapter, REG_GPIO_IO_SEL+1, value8|0x0F);//Reg0x43
#endif //CONFIG_PLATFORM_SPRD


#ifdef CONFIG_XMIT_ACK
	//ack for xmit mgmt frames.
	rtw_write32(padapter, REG_FWHW_TXQ_CTRL, rtw_read32(padapter, REG_FWHW_TXQ_CTRL)|BIT(12));
#endif //CONFIG_XMIT_ACK

	//RT_TRACE(COMP_INIT, DBG_LOUD, ("<---Initializepadapter8192CSdio()\n"));
	DBG_8192C("-rtl8188es_hal_init\n");

exit:
HAL_INIT_PROFILE_TAG(HAL_INIT_STAGES_END);

	DBG_871X("%s in %dms\n", __FUNCTION__, rtw_get_passing_time_ms(init_start_time));

	#ifdef DBG_HAL_INIT_PROFILING
	hal_init_stages_timestamp[HAL_INIT_STAGES_END]=rtw_get_current_time();

	for(hal_init_profiling_i=0;hal_init_profiling_i<HAL_INIT_STAGES_NUM-1;hal_init_profiling_i++) {
		DBG_871X("DBG_HAL_INIT_PROFILING: %35s, %u, %5u, %5u\n"
			, hal_init_stages_str[hal_init_profiling_i]
			, hal_init_stages_timestamp[hal_init_profiling_i]
			, (hal_init_stages_timestamp[hal_init_profiling_i+1]-hal_init_stages_timestamp[hal_init_profiling_i])
			, rtw_get_time_interval_ms(hal_init_stages_timestamp[hal_init_profiling_i], hal_init_stages_timestamp[hal_init_profiling_i+1])
		);
	}	
	#endif

	return ret;
	
}

static void hal_poweroff_8188es(PADAPTER padapter)
{
	u8		u1bTmp;
	u16		u2bTmp;
	u32		u4bTmp;
	u8		bMacPwrCtrlOn = _FALSE;
	u8		ret;

#ifdef CONFIG_PLATFORM_SPRD
	struct pwrctrl_priv *pwrpriv = adapter_to_pwrctl(padapter);
#endif //CONFIG_PLATFORM_SPRD	

	rtw_hal_get_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	if(bMacPwrCtrlOn == _FALSE)
	{
		DBG_871X("=>%s bMacPwrCtrlOn == _FALSE return !!\n", __FUNCTION__);	
		return;
	}
	DBG_871X("=>%s\n", __FUNCTION__);


	//Stop Tx Report Timer. 0x4EC[Bit1]=b'0
	u1bTmp = rtw_read8(padapter, REG_TX_RPT_CTRL);
	rtw_write8(padapter, REG_TX_RPT_CTRL, u1bTmp&(~BIT1));
	
	// stop rx 
	rtw_write8(padapter,REG_CR, 0x0);


#ifdef CONFIG_EXT_CLK //for sprd For Power Consumption.
	EnableGpio5ClockReq(padapter, _FALSE, 0);	
#endif //CONFIG_EXT_CLK

#if 1
	// For Power Consumption.
	u1bTmp = rtw_read8(padapter, GPIO_IN);
	rtw_write8(padapter, GPIO_OUT, u1bTmp);
	rtw_write8(padapter, GPIO_IO_SEL, 0xFF);//Reg0x46

	u1bTmp = rtw_read8(padapter, REG_GPIO_IO_SEL);
	rtw_write8(padapter, REG_GPIO_IO_SEL, (u1bTmp<<4)|u1bTmp);
	u1bTmp = rtw_read8(padapter, REG_GPIO_IO_SEL+1);
	rtw_write8(padapter, REG_GPIO_IO_SEL+1, u1bTmp|0x0F);//Reg0x43
#endif


	// Run LPS WL RFOFF flow	
	ret = HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, Rtl8188E_NIC_LPS_ENTER_FLOW);
	if (ret == _FALSE) {
		DBG_871X("%s: run RF OFF flow fail!\n", __func__);
	}

	//	==== Reset digital sequence   ======

	u1bTmp = rtw_read8(padapter, REG_MCUFWDL);
	if ((u1bTmp & RAM_DL_SEL) && padapter->bFWReady) //8051 RAM code
	{
		//rtl8723a_FirmwareSelfReset(padapter);
		//_8051Reset88E(padapter);		
		
		// Reset MCU 0x2[10]=0.
		u1bTmp = rtw_read8(padapter, REG_SYS_FUNC_EN+1);
		u1bTmp &= ~BIT(2);	// 0x2[10], FEN_CPUEN
		rtw_write8(padapter, REG_SYS_FUNC_EN+1, u1bTmp);
	}	

	//u1bTmp = rtw_read8(padapter, REG_SYS_FUNC_EN+1);
	//u1bTmp &= ~BIT(2);	// 0x2[10], FEN_CPUEN
	//rtw_write8(padapter, REG_SYS_FUNC_EN+1, u1bTmp);

	// MCUFWDL 0x80[1:0]=0
	// reset MCU ready status
	rtw_write8(padapter, REG_MCUFWDL, 0);

	//==== Reset digital sequence end ======
	

	bMacPwrCtrlOn = _FALSE;	// Disable CMD53 R/W
	rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);


/*
	if((pMgntInfo->RfOffReason & RF_CHANGE_BY_HW) && pHalData->pwrdown)
	{// Power Down
		
		// Card disable power action flow
		ret = HalPwrSeqCmdParsing(Adapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, Rtl8188E_NIC_PDN_FLOW);	
	}	
	else
*/	
	{ // Non-Power Down

		// Card disable power action flow
		ret = HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, Rtl8188E_NIC_DISABLE_FLOW);

		 
		if (ret == _FALSE) {
			DBG_871X("%s: run CARD DISABLE flow fail!\n", __func__);
		}
	}


/*
	// Reset MCU IO Wrapper, added by Roger, 2011.08.30
	u1bTmp = rtw_read8(padapter, REG_RSV_CTRL+1);
	u1bTmp &= ~BIT(0);
	rtw_write8(padapter, REG_RSV_CTRL+1, u1bTmp);
	u1bTmp = rtw_read8(padapter, REG_RSV_CTRL+1);
	u1bTmp |= BIT(0);
	rtw_write8(padapter, REG_RSV_CTRL+1, u1bTmp);
*/


	// RSV_CTRL 0x1C[7:0]=0x0E
	// lock ISO/CLK/Power control register
	rtw_write8(padapter, REG_RSV_CTRL, 0x0E);

	padapter->bFWReady = _FALSE;
	bMacPwrCtrlOn = _FALSE;
	rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	
	DBG_871X("<=%s\n", __FUNCTION__);

}

static u32 rtl8188es_hal_deinit(PADAPTER padapter)
{
	DBG_871X("=>%s\n", __FUNCTION__);

	if (padapter->hw_init_completed == _TRUE)
		hal_poweroff_8188es(padapter);
	
	DBG_871X("<=%s\n", __FUNCTION__);

	return _SUCCESS;
}

static u32 rtl8188es_inirp_init(PADAPTER padapter)
{
	u32 status;

_func_enter_;

	status = _SUCCESS;

_func_exit_;

	return status;
}

static u32 rtl8188es_inirp_deinit(PADAPTER padapter)
{
	RT_TRACE(_module_hci_hal_init_c_,_drv_info_,("+rtl8188es_inirp_deinit\n"));

	RT_TRACE(_module_hci_hal_init_c_,_drv_info_,("-rtl8188es_inirp_deinit\n"));

	return _SUCCESS;
}

static void rtl8188es_init_default_value(PADAPTER padapter)
{
	PHAL_DATA_TYPE pHalData;
	struct pwrctrl_priv *pwrctrlpriv;
	struct dm_priv *pdmpriv;
	u8 i;

	pHalData = GET_HAL_DATA(padapter);
	pwrctrlpriv = adapter_to_pwrctl(padapter);
	pdmpriv = &pHalData->dmpriv;
	padapter->registrypriv.wireless_mode = WIRELESS_11BG_24N;

	//init default value
	pHalData->fw_ractrl = _FALSE;		
	if(!pwrctrlpriv->bkeepfwalive)
		pHalData->LastHMEBoxNum = 0;	

	//init dm default value
	pHalData->odmpriv.RFCalibrateInfo.bIQKInitialized = _FALSE;
	pHalData->odmpriv.RFCalibrateInfo.TM_Trigger = 0;//for IQK
	//pdmpriv->binitialized = _FALSE;
//	pdmpriv->prv_traffic_idx = 3;
//	pdmpriv->initialize = 0;
	pHalData->pwrGroupCnt = 0;
	pHalData->PGMaxGroup= 13;
	pHalData->odmpriv.RFCalibrateInfo.ThermalValue_HP_index = 0;
	for(i = 0; i < HP_THERMAL_NUM; i++)
		pHalData->odmpriv.RFCalibrateInfo.ThermalValue_HP[i] = 0;

	// interface related variable
	pHalData->SdioRxFIFOCnt = 0;
	pHalData->EfuseHal.fakeEfuseBank = 0;
	pHalData->EfuseHal.fakeEfuseUsedBytes = 0;
	_rtw_memset(pHalData->EfuseHal.fakeEfuseContent, 0xFF, EFUSE_MAX_HW_SIZE);
	_rtw_memset(pHalData->EfuseHal.fakeEfuseInitMap, 0xFF, EFUSE_MAX_MAP_LEN);
	_rtw_memset(pHalData->EfuseHal.fakeEfuseModifiedMap, 0xFF, EFUSE_MAX_MAP_LEN);

}

//
//	Description:
//		We should set Efuse cell selection to WiFi cell in default.
//
//	Assumption:
//		PASSIVE_LEVEL
//
//	Added by Roger, 2010.11.23.
//
static void _EfuseCellSel(
	IN	PADAPTER	padapter
	)
{
	//HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);

	u32			value32;

	//if(INCLUDE_MULTI_FUNC_BT(padapter))
	{
		value32 = rtw_read32(padapter, EFUSE_TEST);
		value32 = (value32 & ~EFUSE_SEL_MASK) | EFUSE_SEL(EFUSE_WIFI_SEL_0);
		rtw_write32(padapter, EFUSE_TEST, value32);
	}
}

static VOID
_ReadRFType(
	IN	PADAPTER	Adapter
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);

#if DISABLE_BB_RF
	pHalData->rf_chip = RF_PSEUDO_11N;
#else
	pHalData->rf_chip = RF_6052;
#endif
}

static void
Hal_EfuseParsePIDVID_8188ES(
	IN	PADAPTER		pAdapter,
	IN	u8*			hwinfo,
	IN	BOOLEAN			AutoLoadFail
	)
{
//	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(pAdapter);

	//
	// <Roger_Notes> The PID/VID info was parsed from CISTPL_MANFID Tuple in CIS area before.
	// VID is parsed from Manufacture code field and PID is parsed from Manufacture information field.
	// 2011.04.01.
	//

//	RT_TRACE(COMP_INIT, DBG_LOUD, ("EEPROM VID = 0x%4x\n", pHalData->EEPROMVID));
//	RT_TRACE(COMP_INIT, DBG_LOUD, ("EEPROM PID = 0x%4x\n", pHalData->EEPROMPID));
}

static void
Hal_EfuseParseMACAddr_8188ES(
	IN	PADAPTER		padapter,
	IN	u8*			hwinfo,
	IN	BOOLEAN			AutoLoadFail
	)
{
	u16			i, usValue;
	u8			sMacAddr[6] = {0x00, 0xE0, 0x4C, 0x81, 0x88, 0x77};
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(padapter);

	if (AutoLoadFail)
	{
//		sMacAddr[5] = (u1Byte)GetRandomNumber(1, 254);
		for (i=0; i<6; i++)
			pEEPROM->mac_addr[i] = sMacAddr[i];
	}
	else
	{
		//Read Permanent MAC address
		_rtw_memcpy(pEEPROM->mac_addr, &hwinfo[EEPROM_MAC_ADDR_88ES], ETH_ALEN);

	}
//	NicIFSetMacAddress(pAdapter, pAdapter->PermanentAddress);

	DBG_871X("Hal_EfuseParseMACAddr_8188ES: Permanent Address = %02x-%02x-%02x-%02x-%02x-%02x\n",
		  pEEPROM->mac_addr[0], pEEPROM->mac_addr[1],
		  pEEPROM->mac_addr[2], pEEPROM->mac_addr[3],
		  pEEPROM->mac_addr[4], pEEPROM->mac_addr[5]);
}


#ifdef CONFIG_EFUSE_CONFIG_FILE
static u32 Hal_readPGDataFromConfigFile(
	PADAPTER	padapter)
{
	u32 i;
	struct file *fp;
	mm_segment_t fs;
	u8 temp[3];
	loff_t pos = 0;
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(padapter);
	u8	*PROMContent = pEEPROM->efuse_eeprom_data;


	temp[2] = 0; // add end of string '\0'

	fp = filp_open("/system/etc/wifi/wifi_efuse.map", O_RDONLY, 0);
	if (IS_ERR(fp)) {
		pEEPROM->bloadfile_fail_flag = _TRUE;
		DBG_871X("Error, Efuse configure file doesn't exist.\n");
		return _FAIL;
	}

	fs = get_fs();
	set_fs(KERNEL_DS);

	for (i=0; i<HWSET_MAX_SIZE_88E; i++) {
		vfs_read(fp, temp, 2, &pos);
		PROMContent[i] = simple_strtoul(temp, NULL, 16 );
		pos += 1; // Filter the space character
	}

	set_fs(fs);
	filp_close(fp, NULL);
	pEEPROM->bloadfile_fail_flag = _FALSE;

#ifdef CONFIG_DEBUG
	DBG_871X("Efuse configure file:\n");
	for (i=0; i<HWSET_MAX_SIZE_88E; i++)
	{
		if (i % 16 == 0)
			printk("\n");

		printk("%02X ", PROMContent[i]);
	}
	printk("\n");
#endif

	return _SUCCESS;
}

static void
Hal_ReadMACAddrFromFile_8188ES(
	PADAPTER		padapter
	)
{
	u32 i;
	struct file *fp;
	mm_segment_t fs;
	u8 source_addr[18];
	loff_t pos = 0;
	u32	curtime = rtw_get_current_time();
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(padapter);
	u8 *head, *end;

	u8 null_mac_addr[ETH_ALEN] = {0, 0, 0,0, 0, 0};
	u8 multi_mac_addr[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	_rtw_memset(source_addr, 0, 18);
	_rtw_memset(pEEPROM->mac_addr, 0, ETH_ALEN);

	fp = filp_open("/data/wifimac.txt", O_RDWR,  0644);
	if (IS_ERR(fp)) {
		pEEPROM->bloadmac_fail_flag = _TRUE;
		DBG_871X("Error, wifi mac address file doesn't exist.\n");
	} else {
		fs = get_fs();
		set_fs(KERNEL_DS);

		DBG_871X("wifi mac address:\n");
		vfs_read(fp, source_addr, 18, &pos);
		source_addr[17] = ':';

		head = end = source_addr;
		for (i=0; i<ETH_ALEN; i++) {
			while (end && (*end != ':') )
				end++;

			if (end && (*end == ':') )
				*end = '\0';

			pEEPROM->mac_addr[i] = simple_strtoul(head, NULL, 16 );

			if (end) {
				end++;
				head = end;
			}
			DBG_871X("%02x \n", pEEPROM->mac_addr[i]);
		}
		DBG_871X("\n");
		set_fs(fs);
		pEEPROM->bloadmac_fail_flag = _FALSE;
		filp_close(fp, NULL);
	}

	if ( (_rtw_memcmp(pEEPROM->mac_addr, null_mac_addr, ETH_ALEN)) ||
		(_rtw_memcmp(pEEPROM->mac_addr, multi_mac_addr, ETH_ALEN)) ) {
		pEEPROM->mac_addr[0] = 0x00;
		pEEPROM->mac_addr[1] = 0xe0;
		pEEPROM->mac_addr[2] = 0x4c;
		pEEPROM->mac_addr[3] = (u8)(curtime & 0xff) ;
		pEEPROM->mac_addr[4] = (u8)((curtime>>8) & 0xff) ;
		pEEPROM->mac_addr[5] = (u8)((curtime>>16) & 0xff) ;
	}

	DBG_871X("Hal_ReadMACAddrFromFile_8188ES: Permanent Address = %02x-%02x-%02x-%02x-%02x-%02x\n",
		  pEEPROM->mac_addr[0], pEEPROM->mac_addr[1],
		  pEEPROM->mac_addr[2], pEEPROM->mac_addr[3],
		  pEEPROM->mac_addr[4], pEEPROM->mac_addr[5]);
}
#endif //CONFIG_EFUSE_CONFIG_FILE

static VOID
readAdapterInfo_8188ES(
	IN PADAPTER			padapter
	)
{
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(padapter);

	/* parse the eeprom/efuse content */
	Hal_EfuseParseIDCode88E(padapter, pEEPROM->efuse_eeprom_data);
	Hal_EfuseParsePIDVID_8188ES(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	
#ifdef CONFIG_EFUSE_CONFIG_FILE
	Hal_ReadMACAddrFromFile_8188ES(padapter);
#else //CONFIG_EFUSE_CONFIG_FILE
	Hal_EfuseParseMACAddr_8188ES(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
#endif //CONFIG_EFUSE_CONFIG_FILE
	
	Hal_ReadPowerSavingMode88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadTxPowerInfo88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_EfuseParseEEPROMVer88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	rtl8188e_EfuseParseChnlPlan(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_EfuseParseXtal_8188E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_EfuseParseCustomerID88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	//Hal_ReadAntennaDiversity88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_EfuseParseBoardType88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadThermalMeter_88E(padapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	//
	// The following part initialize some vars by PG info.
	//
	Hal_InitChannelPlan(padapter);
#ifdef CONFIG_WOWLAN
	Hal_DetectWoWMode(padapter);
#endif
}

static void _ReadPROMContent(
	IN PADAPTER 		padapter
	)
{
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(padapter);
	u8			eeValue;

	/* check system boot selection */
	eeValue = rtw_read8(padapter, REG_9346CR);
	pEEPROM->EepromOrEfuse = (eeValue & BOOT_FROM_EEPROM) ? _TRUE : _FALSE;
	pEEPROM->bautoload_fail_flag = (eeValue & EEPROM_EN) ? _FALSE : _TRUE;

	DBG_871X("%s: 9346CR=0x%02X, Boot from %s, Autoload %s\n",
		  __FUNCTION__, eeValue,
		  (pEEPROM->EepromOrEfuse ? "EEPROM" : "EFUSE"),
		  (pEEPROM->bautoload_fail_flag ? "Fail" : "OK"));

//	pHalData->EEType = IS_BOOT_FROM_EEPROM(Adapter) ? EEPROM_93C46 : EEPROM_BOOT_EFUSE;

#ifdef CONFIG_EFUSE_CONFIG_FILE
	Hal_readPGDataFromConfigFile(padapter);
#else //CONFIG_EFUSE_CONFIG_FILE
	Hal_InitPGData88E(padapter);
#endif //CONFIG_EFUSE_CONFIG_FILE	
	readAdapterInfo_8188ES(padapter);
}

static VOID
_InitOtherVariable(
	IN PADAPTER 		Adapter
	)
{
	//HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);


	//if(Adapter->bInHctTest){
	//	pMgntInfo->PowerSaveControl.bInactivePs = FALSE;
	//	pMgntInfo->PowerSaveControl.bIPSModeBackup = FALSE;
	//	pMgntInfo->PowerSaveControl.bLeisurePs = FALSE;
	//	pMgntInfo->keepAliveLevel = 0;
	//}


}

//
//	Description:
//		Read HW adapter information by E-Fuse or EEPROM according CR9346 reported.
//
//	Assumption:
//		PASSIVE_LEVEL (SDIO interface)
//
//
static s32 _ReadAdapterInfo8188ES(PADAPTER padapter)
{
	u32 start;

		
	RT_TRACE(_module_hci_hal_init_c_, _drv_info_, ("+_ReadAdapterInfo8188ES\n"));

	// before access eFuse, make sure card enable has been called
	if(_CardEnable(padapter) == _FAIL)
	{
		DBG_871X(KERN_ERR "%s: run power on flow fail\n", __func__);
		return _FAIL;	
	}

	start = rtw_get_current_time();
	
//	Efuse_InitSomeVar(Adapter);
//	pHalData->VersionID = ReadChipVersion(Adapter);
//	_EfuseCellSel(padapter);

	_ReadRFType(padapter);//rf_chip -> _InitRFType()
	_ReadPROMContent(padapter);
	
	// 2010/10/25 MH THe function must be called after borad_type & IC-Version recognize.
	//ReadSilmComboMode(Adapter);
	_InitOtherVariable(padapter);


	//MSG_8192C("%s()(done), rf_chip=0x%x, rf_type=0x%x\n",  __FUNCTION__, pHalData->rf_chip, pHalData->rf_type);
	MSG_8192C("<==== ReadAdapterInfo8188ES in %d ms\n", rtw_get_passing_time_ms(start));

	return _SUCCESS;
}

static void ReadAdapterInfo8188ES(PADAPTER padapter)
{
	// Read EEPROM size before call any EEPROM function
	padapter->EepromAddressSize = GetEEPROMSize8188E(padapter);

	_ReadAdapterInfo8188ES(padapter);
}



// todo static 
#if 0
void hw_var_set_opmode(PADAPTER Adapter, u8 variable, u8* val)
{
	u8	val8;
	u8	mode = *((u8 *)val);
	//HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);

#ifdef CONFIG_CONCURRENT_MODE
	if(Adapter->iface_type == IFACE_PORT1)
	{
		// disable Port1 TSF update
		rtw_write8(Adapter, REG_BCN_CTRL_1, rtw_read8(Adapter, REG_BCN_CTRL_1)|BIT(4));
		
		// set net_type
		val8 = rtw_read8(Adapter, MSR)&0x03;
		val8 |= (mode<<2);
		rtw_write8(Adapter, MSR, val8);
		
		DBG_871X("%s()-%d mode = %d\n", __FUNCTION__, __LINE__, mode);

		if((mode == _HW_STATE_STATION_) || (mode == _HW_STATE_NOLINK_))
		{
			if(!check_buddy_mlmeinfo_state(Adapter, WIFI_FW_AP_STATE))			
			{
				#ifdef CONFIG_INTERRUPT_BASED_TXBCN	

				#ifdef  CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT	
				rtw_write8(Adapter, REG_DRVERLYINT, 0x05);//restore early int time to 5ms
				UpdateInterruptMask8188ESdio(Adapter, 0, SDIO_HIMR_BCNERLY_INT_MSK);	
				#endif // CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
				
				#ifdef CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR		
				UpdateInterruptMask8188ESdio(Adapter, 0, (SDIO_HIMR_TXBCNOK_MSK|SDIO_HIMR_TXBCNERR_MSK));	
				#endif// CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
					
				#endif //CONFIG_INTERRUPT_BASED_TXBCN		
			

				StopTxBeacon(Adapter);
			}
			
			rtw_write8(Adapter,REG_BCN_CTRL_1, 0x11);//disable atim wnd and disable beacon function
			//rtw_write8(Adapter,REG_BCN_CTRL_1, 0x18);
		}
		else if((mode == _HW_STATE_ADHOC_) /*|| (mode == _HW_STATE_AP_)*/)
		{
			ResumeTxBeacon(Adapter);
			rtw_write8(Adapter,REG_BCN_CTRL_1, 0x1a);
			//BIT4 - If set 0, hw will clr bcnq when tx becon ok/fail or port 1
			rtw_write8(Adapter, REG_MBID_NUM, rtw_read8(Adapter, REG_MBID_NUM)|BIT(3)|BIT(4));
		}
		else if(mode == _HW_STATE_AP_)
		{
#ifdef CONFIG_INTERRUPT_BASED_TXBCN			
			#ifdef  CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
			UpdateInterruptMask8188ESdio(Adapter, SDIO_HIMR_BCNERLY_INT_MSK, 0);
			#endif//CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT

			#ifdef CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR	
			UpdateInterruptMask8188ESdio(Adapter, (SDIO_HIMR_TXBCNOK_MSK|SDIO_HIMR_TXBCNERR_MSK), 0);
			#endif//CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
	
#endif //CONFIG_INTERRUPT_BASED_TXBCN

			ResumeTxBeacon(Adapter);
	
			rtw_write8(Adapter, REG_BCN_CTRL_1, 0x12);

			//enable SW Beacon
			rtw_write32(Adapter, REG_CR, rtw_read32(Adapter, REG_CR)|BIT(8));

			//Set RCR
			//rtw_write32(padapter, REG_RCR, 0x70002a8e);//CBSSID_DATA must set to 0
			rtw_write32(Adapter, REG_RCR, 0x7000208e);//CBSSID_DATA must set to 0,Reject ICV_ERROR packets
			
			//enable to rx data frame				
			rtw_write16(Adapter, REG_RXFLTMAP2, 0xFFFF);
			//enable to rx ps-poll
			rtw_write16(Adapter, REG_RXFLTMAP1, 0x0400);

			//Beacon Control related register for first time 
			rtw_write8(Adapter, REG_BCNDMATIM, 0x02); // 2ms		

			//rtw_write8(Adapter, REG_BCN_MAX_ERR, 0xFF);
			rtw_write8(Adapter, REG_ATIMWND_1, 0x0a); // 10ms for port1
			rtw_write16(Adapter, REG_BCNTCFG, 0x00);
			rtw_write16(Adapter, REG_TBTT_PROHIBIT, 0xff04);
			rtw_write16(Adapter, REG_TSFTR_SYN_OFFSET, 0x7fff);// +32767 (~32ms)

			//reset TSF2	
			rtw_write8(Adapter, REG_DUAL_TSF_RST, BIT(1));


			//BIT4 - If set 0, hw will clr bcnq when tx becon ok/fail or port 1
			rtw_write8(Adapter, REG_MBID_NUM, rtw_read8(Adapter, REG_MBID_NUM)|BIT(3)|BIT(4));
		       	//enable BCN1 Function for if2
			//don't enable update TSF1 for if2 (due to TSF update when beacon/probe rsp are received)
			rtw_write8(Adapter, REG_BCN_CTRL_1, (DIS_TSF_UDT0_NORMAL_CHIP|EN_BCN_FUNCTION | EN_TXBCN_RPT|BIT(1)));

#ifdef CONFIG_CONCURRENT_MODE
			if(check_buddy_fwstate(Adapter, WIFI_FW_NULL_STATE))
				rtw_write8(Adapter, REG_BCN_CTRL, 
					rtw_read8(Adapter, REG_BCN_CTRL) & ~EN_BCN_FUNCTION);
#endif
                        //BCN1 TSF will sync to BCN0 TSF with offset(0x518) if if1_sta linked
			//rtw_write8(Adapter, REG_BCN_CTRL_1, rtw_read8(Adapter, REG_BCN_CTRL_1)|BIT(5));
			//rtw_write8(Adapter, REG_DUAL_TSF_RST, BIT(3));
					
			//dis BCN0 ATIM  WND if if1 is station
			rtw_write8(Adapter, REG_BCN_CTRL, rtw_read8(Adapter, REG_BCN_CTRL)|BIT(0));

#ifdef CONFIG_TSF_RESET_OFFLOAD
			// Reset TSF for STA+AP concurrent mode
			if ( check_buddy_fwstate(Adapter, (WIFI_STATION_STATE|WIFI_ASOC_STATE)) ) {
				if (reset_tsf(Adapter, IFACE_PORT1) == _FALSE)
					DBG_871X("ERROR! %s()-%d: Reset port1 TSF fail\n",
						__FUNCTION__, __LINE__);
			}
#endif	// CONFIG_TSF_RESET_OFFLOAD	
		}
	}
	else
#endif //CONFIG_CONCURRENT_MODE
	{
		// disable Port0 TSF update
		rtw_write8(Adapter, REG_BCN_CTRL, rtw_read8(Adapter, REG_BCN_CTRL)|BIT(4));
		
		// set net_type
		val8 = rtw_read8(Adapter, MSR)&0x0c;
		val8 |= mode;
		rtw_write8(Adapter, MSR, val8);
		
		DBG_871X("%s()-%d mode = %d\n", __FUNCTION__, __LINE__, mode);
		
		if((mode == _HW_STATE_STATION_) || (mode == _HW_STATE_NOLINK_))
		{
#ifdef CONFIG_CONCURRENT_MODE
			if(!check_buddy_mlmeinfo_state(Adapter, WIFI_FW_AP_STATE))		
#endif //CONFIG_CONCURRENT_MODE
			{
			#ifdef CONFIG_INTERRUPT_BASED_TXBCN	
				#ifdef  CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
				rtw_write8(Adapter, REG_DRVERLYINT, 0x05);//restore early int time to 5ms					
				UpdateInterruptMask8188ESdio(Adapter, 0, SDIO_HIMR_BCNERLY_INT_MSK);	
				#endif//CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
				
				#ifdef CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR		
				UpdateInterruptMask8188ESdio(Adapter, 0, (SDIO_HIMR_TXBCNOK_MSK|SDIO_HIMR_TXBCNERR_MSK));	
				#endif //CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
			#endif //CONFIG_INTERRUPT_BASED_TXBCN		
				StopTxBeacon(Adapter);
			}
			
			rtw_write8(Adapter,REG_BCN_CTRL, 0x19);//disable atim wnd
			//rtw_write8(Adapter,REG_BCN_CTRL, 0x18);
		}
		else if((mode == _HW_STATE_ADHOC_) /*|| (mode == _HW_STATE_AP_)*/)
		{
			ResumeTxBeacon(Adapter);
			rtw_write8(Adapter,REG_BCN_CTRL, 0x1a);
			//BIT3 - If set 0, hw will clr bcnq when tx becon ok/fail or port 0
			rtw_write8(Adapter, REG_MBID_NUM, rtw_read8(Adapter, REG_MBID_NUM)|BIT(3)|BIT(4));
		}
		else if(mode == _HW_STATE_AP_)
		{

#ifdef CONFIG_INTERRUPT_BASED_TXBCN			
			#ifdef  CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
			UpdateInterruptMask8188ESdio(Adapter, SDIO_HIMR_BCNERLY_INT_MSK, 0);
			#endif//CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT

			#ifdef CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
			UpdateInterruptMask8188ESdio(Adapter, (SDIO_HIMR_TXBCNOK_MSK|SDIO_HIMR_TXBCNERR_MSK), 0);
			#endif//CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
#endif //CONFIG_INTERRUPT_BASED_TXBCN


			ResumeTxBeacon(Adapter);

			rtw_write8(Adapter, REG_BCN_CTRL, 0x12);

			//enable SW Beacon
			rtw_write32(Adapter, REG_CR, rtw_read32(Adapter, REG_CR)|BIT(8));

			//Set RCR
			//rtw_write32(padapter, REG_RCR, 0x70002a8e);//CBSSID_DATA must set to 0
			rtw_write32(Adapter, REG_RCR, 0x7000208e);//CBSSID_DATA must set to 0,reject ICV_ERR packet
			//enable to rx data frame
			rtw_write16(Adapter, REG_RXFLTMAP2, 0xFFFF);
			//enable to rx ps-poll
			rtw_write16(Adapter, REG_RXFLTMAP1, 0x0400);

			//Beacon Control related register for first time
			rtw_write8(Adapter, REG_BCNDMATIM, 0x02); // 2ms			
			
			//rtw_write8(Adapter, REG_BCN_MAX_ERR, 0xFF);
			rtw_write8(Adapter, REG_ATIMWND, 0x0a); // 10ms
			rtw_write16(Adapter, REG_BCNTCFG, 0x00);
			rtw_write16(Adapter, REG_TBTT_PROHIBIT, 0xff04);
			rtw_write16(Adapter, REG_TSFTR_SYN_OFFSET, 0x7fff);// +32767 (~32ms)

			//reset TSF
			rtw_write8(Adapter, REG_DUAL_TSF_RST, BIT(0));

			//BIT3 - If set 0, hw will clr bcnq when tx becon ok/fail or port 0
			rtw_write8(Adapter, REG_MBID_NUM, rtw_read8(Adapter, REG_MBID_NUM)|BIT(3)|BIT(4));
	
		        //enable BCN0 Function for if1
			//don't enable update TSF0 for if1 (due to TSF update when beacon/probe rsp are received)
			rtw_write8(Adapter, REG_BCN_CTRL, (DIS_TSF_UDT0_NORMAL_CHIP|EN_BCN_FUNCTION | EN_TXBCN_RPT|BIT(1)));

#ifdef CONFIG_CONCURRENT_MODE
			if(check_buddy_fwstate(Adapter, WIFI_FW_NULL_STATE))
				rtw_write8(Adapter, REG_BCN_CTRL_1, 
					rtw_read8(Adapter, REG_BCN_CTRL_1) & ~EN_BCN_FUNCTION);
#endif

			//dis BCN1 ATIM  WND if if2 is station
			rtw_write8(Adapter, REG_BCN_CTRL_1, rtw_read8(Adapter, REG_BCN_CTRL_1)|BIT(0));	
#ifdef CONFIG_TSF_RESET_OFFLOAD
			// Reset TSF for STA+AP concurrent mode
			if ( check_buddy_fwstate(Adapter, (WIFI_STATION_STATE|WIFI_ASOC_STATE)) ) {
				if (reset_tsf(Adapter, IFACE_PORT0) == _FALSE)
					DBG_871X("ERROR! %s()-%d: Reset port0 TSF fail\n",
						__FUNCTION__, __LINE__);
			}
#endif	// CONFIG_TSF_RESET_OFFLOAD
		}
	}

}
#endif

static void SetHwReg8188ES(PADAPTER Adapter, u8 variable, u8* val)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	
_func_enter_;

	switch(variable)
	{			
		case HW_VAR_RXDMA_AGG_PG_TH:
			rtw_write8(Adapter, REG_RXDMA_AGG_PG_TH, *((u8 *)val));
			break;
			
		case HW_VAR_SET_RPWM:
#ifdef CONFIG_LPS_LCLK
			{
				u8	ps_state = *((u8 *)val);
				//rpwm value only use BIT0(clock bit) ,BIT6(Ack bit), and BIT7(Toggle bit) for 88e.
				//BIT0 value - 1: 32k, 0:40MHz.
				//BIT6 value - 1: report cpwm value after success set, 0:do not report.
				//BIT7 value - Toggle bit change.
				//modify by Thomas. 2012/4/2.
				ps_state = ps_state & 0xC1;

#ifdef CONFIG_EXT_CLK //for sprd
				if(ps_state&BIT(6)) // want to leave 32k
				{
					//enable ext clock req before leave LPS-32K
					//DBG_871X("enable ext clock req before leaving LPS-32K\n");					
					EnableGpio5ClockReq(Adapter, _FALSE, 1);
				}
#endif //CONFIG_EXT_CLK

				//DBG_871X("##### Change RPWM value to = %x for switch clk #####\n",ps_state);
				rtw_write8(Adapter, SDIO_LOCAL_BASE|SDIO_REG_HRPWM1, ps_state);
			}
#endif
			break;
		
#ifdef CONFIG_WOWLAN
		case HW_VAR_WOWLAN:
		{
			struct wowlan_ioctl_param *poidparam;
			struct recv_buf *precvbuf;
			struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(Adapter);
			struct security_priv *psecuritypriv = &Adapter->securitypriv;
			int res, i;
			u32 tmp;
			u64 iv_low = 0, iv_high = 0;
			u16 len = 0;
			u8 mstatus = (*(u8 *)val);
			u8 trycnt = 100;
			u8 data[4];
			u8 val8;

			poidparam = (struct wowlan_ioctl_param *)val;
			switch (poidparam->subcode){
				case WOWLAN_ENABLE:
					DBG_871X_LEVEL(_drv_always_, "WOWLAN_ENABLE\n");

					val8 = (psecuritypriv->dot11AuthAlgrthm == dot11AuthAlgrthm_8021X)? 0xcc: 0xcf;
					rtw_write8(Adapter, REG_SECCFG, val8);
					DBG_871X_LEVEL(_drv_always_, "REG_SECCFG: %02x\n", rtw_read8(Adapter, REG_SECCFG));

					SetFwRelatedForWoWLAN8188ES(Adapter, _TRUE);

					rtl8188e_set_FwJoinBssReport_cmd(Adapter, 1);
					rtw_msleep_os(2);

					//Set Pattern
					//if(pwrctl->wowlan_pattern==_TRUE)
					//	rtw_wowlan_reload_pattern(Adapter);

					//disable TX Report, only use in 88e
					rtw_write8(Adapter, REG_TX_RPT_CTRL,
						((rtw_read8(Adapter, REG_TX_RPT_CTRL)&~BIT(1)))&~BIT(5));
					DBG_871X("disable TXRPT:0x%02x\n",
						rtw_read8(Adapter, REG_TX_RPT_CTRL));

					//RX DMA stop
					DBG_871X_LEVEL(_drv_always_, "Pause DMA\n");
					rtw_write32(Adapter,REG_RXPKT_NUM,(rtw_read32(Adapter,REG_RXPKT_NUM)|RW_RELEASE_EN));
					do{
						if((rtw_read32(Adapter, REG_RXPKT_NUM)&RXDMA_IDLE)) {
							DBG_871X_LEVEL(_drv_always_, "RX_DMA_IDLE is true\n");
							break;
						} else {
							// If RX_DMA is not idle, receive one pkt from DMA
							res = sdio_local_read(Adapter, SDIO_REG_RX0_REQ_LEN, 4, (u8*)&tmp);
							len = le16_to_cpu(tmp);
							DBG_871X_LEVEL(_drv_always_, "RX len:%d\n", len);

							if (len > 0)
								res = RecvOnePkt(Adapter, len);
							else
								DBG_871X_LEVEL(_drv_always_, "read length fail %d\n", len);

							DBG_871X_LEVEL(_drv_always_, "RecvOnePkt Result: %d\n", res);
						}
					}while(trycnt--);
					if(trycnt ==0)
						DBG_871X_LEVEL(_drv_always_, "Stop RX DMA failed...... \n");

					//Enable CPWM2 only.
					DBG_871X_LEVEL(_drv_always_, "Enable only CPWM2\n");
					res = sdio_local_read(Adapter, SDIO_REG_HIMR, 4, (u8*)&tmp);
					if (!res)
					    DBG_871X_LEVEL(_drv_info_, "read SDIO_REG_HIMR: 0x%08x\n", tmp);
					else
					    DBG_871X_LEVEL(_drv_info_, "sdio_local_read fail\n");

					tmp = SDIO_HIMR_CPWM2_MSK;

					res = sdio_local_write(Adapter, SDIO_REG_HIMR, 4, (u8*)&tmp);

					if (!res){
					    res = sdio_local_read(Adapter, SDIO_REG_HIMR, 4, (u8*)&tmp);
					    DBG_871X_LEVEL(_drv_info_, "read again SDIO_REG_HIMR: 0x%08x\n", tmp);
					}else
					    DBG_871X_LEVEL(_drv_info_, "sdio_local_write fail\n");

					//Set WOWLAN H2C command.
					DBG_871X_LEVEL(_drv_always_, "Set WOWLan cmd\n");
					rtl8188es_set_wowlan_cmd(Adapter, 1);

					mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
					trycnt = 10;

					while(!(mstatus&BIT1) && trycnt>1) {
						mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
						DBG_871X_LEVEL(_drv_always_, "Loop index: %d :0x%02x\n", trycnt, mstatus);
						trycnt --;
						rtw_msleep_os(2);
					}

					pwrctl->wowlan_wake_reason = rtw_read8(Adapter, REG_WOWLAN_WAKE_REASON);
					DBG_871X_LEVEL(_drv_always_, "wowlan_wake_reason: 0x%02x\n",
										pwrctl->wowlan_wake_reason);
					
					//rtw_msleep_os(10);
					break;
				case WOWLAN_DISABLE:
					trycnt = 10;
					
					DBG_871X_LEVEL(_drv_always_, "WOWLAN_DISABLE\n");
					rtl8188e_set_FwJoinBssReport_cmd(Adapter, 0);

					rtw_write8(	Adapter, REG_SECCFG, 0x0c|BIT(5));// enable tx enc and rx dec engine, and no key search for MC/BC
					DBG_871X_LEVEL(_drv_always_, "REG_SECCFG: %02x\n", rtw_read8(Adapter, REG_SECCFG));
					
					pwrctl->wowlan_wake_reason = 
												rtw_read8(Adapter, REG_WOWLAN_WAKE_REASON);
					DBG_871X_LEVEL(_drv_always_, "wakeup_reason: 0x%02x\n",
												pwrctl->wowlan_wake_reason);
					rtl8188es_set_wowlan_cmd(Adapter, 0);
					mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
					DBG_871X_LEVEL(_drv_info_, "%s mstatus:0x%02x\n", __func__, mstatus);

					while(mstatus&BIT1 && trycnt>1) {
						mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
						DBG_871X_LEVEL(_drv_always_, "Loop index: %d :0x%02x\n", trycnt, mstatus);
						trycnt --;
						rtw_msleep_os(2);
					}

					if (mstatus & BIT1) {
						DBG_871X_LEVEL(_drv_always_, "Disable WOW mode fail!!\n");
						DBG_871X("Set 0x690=0x00\n");
						rtw_write8(Adapter, REG_WOW_CTRL, (rtw_read8(Adapter, REG_WOW_CTRL)&0xf0));
						DBG_871X_LEVEL(_drv_always_, "Release RXDMA\n");
						rtw_write32(Adapter, REG_RXPKT_NUM,(rtw_read32(Adapter,REG_RXPKT_NUM)&(~RW_RELEASE_EN)));
					}

					//enable TX Report, only use in 88e
					rtw_write8(Adapter, REG_TX_RPT_CTRL,
						((rtw_read8(Adapter, REG_TX_RPT_CTRL)|BIT(1)))|BIT(5));
					DBG_871X("enable TX_RPT:0x%02x\n", rtw_read8(Adapter, REG_TX_RPT_CTRL));

					// 3.1 read fw iv
					iv_low = rtw_read32(Adapter, REG_TXPKTBUF_IV_LOW);
					//only low two bytes is PN, check AES_IV macro for detail
					iv_low &= 0xffff;
					iv_high = rtw_read32(Adapter, REG_TXPKTBUF_IV_HIGH);
					//get the real packet number
					pwrctl->wowlan_fw_iv = iv_high << 16 | iv_low;
					DBG_871X_LEVEL(_drv_always_, "fw_iv: 0x%016llx\n", pwrctl->wowlan_fw_iv);
					//Update TX iv data.
					rtw_set_sec_pn(Adapter);

					SetFwRelatedForWoWLAN8188ES(Adapter, _FALSE);

					if((pwrctl->wowlan_wake_reason != FWDecisionDisconnect) &&
						(pwrctl->wowlan_wake_reason != Rx_Pairwisekey) &&
						(pwrctl->wowlan_wake_reason != Rx_DisAssoc) &&
						(pwrctl->wowlan_wake_reason != Rx_DeAuth))
						rtl8188e_set_FwJoinBssReport_cmd(Adapter, 1);

					rtw_msleep_os(5);

					break;
				default:
					break;
			}
		}
		break;
#endif //CONFIG_WOWLAN
		
		default:
			SetHwReg8188E(Adapter, variable, val);
			break;
	}

_func_exit_;
}

static void GetHwReg8188ES(PADAPTER padapter, u8 variable, u8 *val)
{
	PHAL_DATA_TYPE 	pHalData= GET_HAL_DATA(padapter);	
_func_enter_;

	switch (variable)
	{
		default:
			GetHwReg8188E(padapter, variable, val);
			break;
	}

_func_exit_;
}

//
//	Description:
//		Query setting of specified variable.
//
u8
GetHalDefVar8188ESDIO(
	IN	PADAPTER				Adapter,
	IN	HAL_DEF_VARIABLE		eVariable,
	IN	PVOID					pValue
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);	
	u8			bResult = _SUCCESS;

	switch(eVariable)
	{
		case HW_VAR_MAX_RX_AMPDU_FACTOR:
			*(( u32*)pValue) = MAX_AMPDU_FACTOR_16K;
			break;

		case HAL_DEF_TX_LDPC:
		case HAL_DEF_RX_LDPC:
			*((u8 *)pValue) = _FALSE;
			break;
		case HAL_DEF_TX_STBC:
			*((u8 *)pValue) = 0;
			break;
		case HAL_DEF_RX_STBC:
			*((u8 *)pValue) = 1;
			break;
		default:
			bResult = GetHalDefVar8188E(Adapter, eVariable, pValue);
			break;
	}

	return bResult;
}




//
//	Description:
//		Change default setting of specified variable.
//
u8
SetHalDefVar8188ESDIO(
	IN	PADAPTER				Adapter,
	IN	HAL_DEF_VARIABLE		eVariable,
	IN	PVOID					pValue
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	u8			bResult = _TRUE;

	switch(eVariable)
	{
		default:
			bResult = SetHalDefVar(Adapter, eVariable, pValue);
			break;
	}

	return bResult;
}

static VOID
_BeaconFunctionEnable(
	IN	PADAPTER		padapter,
	IN	BOOLEAN			Enable,
	IN	BOOLEAN			Linked
	)
{
	rtw_write8(padapter, REG_BCN_CTRL, (BIT4 | BIT3 | BIT1));
//	RT_TRACE(_module_hci_hal_init_c_, _drv_info_, ("_BeaconFunctionEnable 0x550 0x%x\n", rtw_read8(padapter, 0x550)));

	rtw_write8(padapter, REG_RD_CTRL+1, 0x6F);
}

void SetBeaconRelatedRegisters8188ESdio(PADAPTER padapter)
{
	u32	value32;
	//HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);
	struct mlme_ext_priv	*pmlmeext = &(padapter->mlmeextpriv);
	struct mlme_ext_info	*pmlmeinfo = &(pmlmeext->mlmext_info);
	u32 bcn_ctrl_reg 		= REG_BCN_CTRL;
	//reset TSF, enable update TSF, correcting TSF On Beacon

	//REG_BCN_INTERVAL
	//REG_BCNDMATIM
	//REG_ATIMWND
	//REG_TBTT_PROHIBIT
	//REG_DRVERLYINT
	//REG_BCN_MAX_ERR
	//REG_BCNTCFG //(0x510)
	//REG_DUAL_TSF_RST
	//REG_BCN_CTRL //(0x550)


#ifdef CONFIG_CONCURRENT_MODE
	if (padapter->iface_type == IFACE_PORT1){
		bcn_ctrl_reg = REG_BCN_CTRL_1;
	}
#endif	
	//
	// ATIM window
	//
	rtw_write16(padapter, REG_ATIMWND, 2);

	//
	// Beacon interval (in unit of TU).
	//
	rtw_write16(padapter, REG_BCN_INTERVAL, pmlmeinfo->bcn_interval);

	_InitBeaconParameters(padapter);

	rtw_write8(padapter, REG_SLOT, 0x09);

	//
	// Force beacon frame transmission even after receiving beacon frame from other ad hoc STA
	//
	//PlatformEFIOWrite1Byte(Adapter, BCN_ERR_THRESH, 0x0a); // We force beacon sent to prevent unexpect disconnect status in Ad hoc mode

	//
	// Reset TSF Timer to zero, added by Roger. 2008.06.24
	//
	value32 = rtw_read32(padapter, REG_TCR);
	value32 &= ~TSFRST;
	rtw_write32(padapter,  REG_TCR, value32);

	value32 |= TSFRST;
	rtw_write32(padapter, REG_TCR, value32);

	// TODO: Modify later (Find the right parameters)
	// NOTE: Fix test chip's bug (about contention windows's randomness)
//	if (OpMode == RT_OP_MODE_IBSS || OpMode == RT_OP_MODE_AP)
	if (check_fwstate(&padapter->mlmepriv, WIFI_ADHOC_STATE|WIFI_AP_STATE) == _TRUE)
	{
		rtw_write8(padapter, REG_RXTSF_OFFSET_CCK, 0x50);
		rtw_write8(padapter, REG_RXTSF_OFFSET_OFDM, 0x50);
	}

	_BeaconFunctionEnable(padapter, _TRUE, _TRUE);

	ResumeTxBeacon(padapter);
	rtw_write8(padapter, bcn_ctrl_reg, rtw_read8(padapter, bcn_ctrl_reg)|BIT(1));
}

void rtl8188es_set_hal_ops(PADAPTER padapter)
{
	struct hal_ops *pHalFunc = &padapter->HalFunc;

_func_enter_;

	pHalFunc->hal_power_on = _InitPowerOn_8188ES;
	pHalFunc->hal_power_off = hal_poweroff_8188es;
	
	pHalFunc->hal_init = &rtl8188es_hal_init;
	pHalFunc->hal_deinit = &rtl8188es_hal_deinit;

	pHalFunc->inirp_init = &rtl8188es_inirp_init;
	pHalFunc->inirp_deinit = &rtl8188es_inirp_deinit;

	pHalFunc->init_xmit_priv = &rtl8188es_init_xmit_priv;
	pHalFunc->free_xmit_priv = &rtl8188es_free_xmit_priv;

	pHalFunc->init_recv_priv = &rtl8188es_init_recv_priv;
	pHalFunc->free_recv_priv = &rtl8188es_free_recv_priv;

	pHalFunc->InitSwLeds = &rtl8188es_InitSwLeds;
	pHalFunc->DeInitSwLeds = &rtl8188es_DeInitSwLeds;

	pHalFunc->init_default_value = &rtl8188es_init_default_value;
	pHalFunc->intf_chip_configure = &rtl8188es_interface_configure;
	pHalFunc->read_adapter_info = &ReadAdapterInfo8188ES;

	pHalFunc->enable_interrupt = &EnableInterrupt8188ESdio;
	pHalFunc->disable_interrupt = &DisableInterrupt8188ESdio;
	pHalFunc->check_ips_status = &CheckIPSStatus;
#ifdef CONFIG_WOWLAN
	pHalFunc->clear_interrupt = &ClearInterrupt8188ESdio;
#endif
	pHalFunc->SetHwRegHandler = &SetHwReg8188ES;
	pHalFunc->GetHwRegHandler = &GetHwReg8188ES;

	pHalFunc->GetHalDefVarHandler = &GetHalDefVar8188ESDIO;
 	pHalFunc->SetHalDefVarHandler = &SetHalDefVar8188ESDIO;

	pHalFunc->SetBeaconRelatedRegistersHandler = &SetBeaconRelatedRegisters8188ESdio;

	pHalFunc->hal_xmit = &rtl8188es_hal_xmit;
	pHalFunc->mgnt_xmit = &rtl8188es_mgnt_xmit;
	pHalFunc->hal_xmitframe_enqueue = &rtl8188es_hal_xmitframe_enqueue;

#ifdef CONFIG_HOSTAPD_MLME
	pHalFunc->hostap_mgnt_xmit_entry = NULL;
//	pHalFunc->hostap_mgnt_xmit_entry = &rtl8192cu_hostap_mgnt_xmit_entry;
#endif
#ifdef CONFIG_XMIT_THREAD_MODE
	pHalFunc->xmit_thread_handler = &rtl8188es_xmit_buf_handler;
#endif
	rtl8188e_set_hal_ops(pHalFunc);
_func_exit_;

}

