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

//============================================================
// include files
//============================================================

#include "odm_precomp.h"
#define ADAPTIVITY_VERSION	"5.0"


VOID
ODM_ChangeDynamicInitGainThresh(
	IN	PVOID		pDM_VOID,
	IN	u4Byte		DM_Type,
	IN	u4Byte		DM_Value
	)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T			pDM_DigTable = &pDM_Odm->DM_DigTable;

	if (DM_Type == DIG_TYPE_THRESH_HIGH)
	{
		pDM_DigTable->RssiHighThresh = DM_Value;		
	}
	else if (DM_Type == DIG_TYPE_THRESH_LOW)
	{
		pDM_DigTable->RssiLowThresh = DM_Value;
	}
	else if (DM_Type == DIG_TYPE_ENABLE)
	{
		pDM_DigTable->Dig_Enable_Flag	= TRUE;
	}	
	else if (DM_Type == DIG_TYPE_DISABLE)
	{
		pDM_DigTable->Dig_Enable_Flag = FALSE;
	}	
	else if (DM_Type == DIG_TYPE_BACKOFF)
	{
		if(DM_Value > 30)
			DM_Value = 30;
		pDM_DigTable->BackoffVal = (u1Byte)DM_Value;
	}
	else if(DM_Type == DIG_TYPE_RX_GAIN_MIN)
	{
		if(DM_Value == 0)
			DM_Value = 0x1;
		pDM_DigTable->rx_gain_range_min = (u1Byte)DM_Value;
	}
	else if(DM_Type == DIG_TYPE_RX_GAIN_MAX)
	{
		if(DM_Value > 0x50)
			DM_Value = 0x50;
		pDM_DigTable->rx_gain_range_max = (u1Byte)DM_Value;
	}
}	// DM_ChangeDynamicInitGainThresh //

VOID
odm_NHMCounterStatisticsInit(
	IN		PVOID			pDM_VOID
	)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;

	if(pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
	{
		//PHY parameters initialize for ac series
		ODM_Write2Byte(pDM_Odm, ODM_REG_NHM_TIMER_11AC+2, 0x2710);	//0x990[31:16]=0x2710	Time duration for NHM unit: 4us, 0x2710=40ms
		ODM_Write2Byte(pDM_Odm, ODM_REG_NHM_TH9_TH10_11AC+2, 0xffff);	//0x994[31:16]=0xffff	th_9, th_10
		//ODM_Write4Byte(pDM_Odm, ODM_REG_NHM_TH3_TO_TH0_11AC, 0xffffff5c);	//0x998=0xffffff5c 		th_3, th_2, th_1, th_0
		ODM_Write4Byte(pDM_Odm, ODM_REG_NHM_TH3_TO_TH0_11AC, 0xffffff52);	//0x998=0xffffff52 		th_3, th_2, th_1, th_0
		ODM_Write4Byte(pDM_Odm, ODM_REG_NHM_TH7_TO_TH4_11AC, 0xffffffff);	//0x99c=0xffffffff		th_7, th_6, th_5, th_4
		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH8_11AC, bMaskByte0, 0xff);		//0x9a0[7:0]=0xff		th_8
		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH9_TH10_11AC, BIT8|BIT9|BIT10, 0x7);	//0x994[9:8]=3			enable CCX
		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_9E8_11AC, BIT0, 0x1);		//0x9e8[7]=1			max power among all RX ants	
				
		//panic_printk("RTL8812AU phy parameters init %s,%d\n", __FUNCTION__, __LINE__);
	}
	else if (pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
	{
		//PHY parameters initialize for n series
		ODM_Write2Byte(pDM_Odm, ODM_REG_NHM_TIMER_11N+2, 0x2710);	//0x894[31:16]=0x2710	Time duration for NHM unit: 4us, 0x2710=40ms
		//ODM_Write2Byte(pDM_Odm, ODM_REG_NHM_TIMER_11N+2, 0x4e20);	//0x894[31:16]=0x4e20	Time duration for NHM unit: 4us, 0x4e20=80ms
		ODM_Write2Byte(pDM_Odm, ODM_REG_NHM_TH9_TH10_11N+2, 0xffff);	//0x890[31:16]=0xffff	th_9, th_10
		//ODM_Write4Byte(pDM_Odm, ODM_REG_NHM_TH3_TO_TH0_11N, 0xffffff5c);	//0x898=0xffffff5c 		th_3, th_2, th_1, th_0
		ODM_Write4Byte(pDM_Odm, ODM_REG_NHM_TH3_TO_TH0_11N, 0xffffff52);	//0x898=0xffffff52 		th_3, th_2, th_1, th_0
		ODM_Write4Byte(pDM_Odm, ODM_REG_NHM_TH7_TO_TH4_11N, 0xffffffff);	//0x89c=0xffffffff		th_7, th_6, th_5, th_4
		ODM_SetBBReg(pDM_Odm, ODM_REG_FPGA0_IQK_11N, bMaskByte0, 0xff);		//0xe28[7:0]=0xff		th_8
		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH9_TH10_11N, BIT10|BIT9|BIT8, 0x7);	//0x890[9:8]=3			enable CCX
		ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTC_11N, BIT7, 0x1);		//0xc0c[7]=1			max power among all RX ants				
	}
}

VOID
odm_NHMCounterStatistics(
	IN		PVOID			pDM_VOID
	)
{
	PDM_ODM_T	pDM_Odm = (PDM_ODM_T)pDM_VOID;
	u4Byte		value32 = 0;

	if (pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
		value32 = ODM_GetBBReg(pDM_Odm, ODM_REG_NHM_CNT_11AC, bMaskDWord);
	else if (pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
		value32 = ODM_GetBBReg(pDM_Odm, ODM_REG_NHM_CNT_11N, bMaskDWord);

	pDM_Odm->NHM_cnt_0 = (u1Byte)(value32 & bMaskByte0);
	
	// Reset NHM counter
	if (pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
	{           		
    		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH9_TH10_11AC, BIT1, 0);
    		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH9_TH10_11AC, BIT1, 1);
	}	\
	else if (pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
	{
    		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH9_TH10_11N, BIT1, 0);
    		ODM_SetBBReg(pDM_Odm, ODM_REG_NHM_TH9_TH10_11N, BIT1, 1);
	}
}


VOID
odm_NHMBBInit(
	IN		PVOID			pDM_VOID
)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;

	pDM_Odm->adaptivity_flag = 0;
	pDM_Odm->tolerance_cnt = 3;
	pDM_Odm->NHMLastTxOkcnt = 0;
	pDM_Odm->NHMLastRxOkcnt = 0;
	pDM_Odm->NHMCurTxOkcnt = 0;
	pDM_Odm->NHMCurRxOkcnt = 0;
}

//
VOID
odm_NHMBB(
	IN		PVOID			pDM_VOID
)
{
	PDM_ODM_T	pDM_Odm = (PDM_ODM_T)pDM_VOID;
	u4Byte		value32;
	//u1Byte 	test_status;
	//PFALSE_ALARM_STATISTICS pFalseAlmCnt = &(pDM_Odm->FalseAlmCnt);

	pDM_Odm->NHMCurTxOkcnt = *(pDM_Odm->pNumTxBytesUnicast) - pDM_Odm->NHMLastTxOkcnt;
	pDM_Odm->NHMCurRxOkcnt = *(pDM_Odm->pNumRxBytesUnicast) - pDM_Odm->NHMLastRxOkcnt;
	pDM_Odm->NHMLastTxOkcnt = *(pDM_Odm->pNumTxBytesUnicast);
	pDM_Odm->NHMLastRxOkcnt = *(pDM_Odm->pNumRxBytesUnicast);	
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("NHM_cnt_0=%d, NHMCurTxOkcnt = %llu, NHMCurRxOkcnt = %llu\n", 
		pDM_Odm->NHM_cnt_0, pDM_Odm->NHMCurTxOkcnt, pDM_Odm->NHMCurRxOkcnt));

	
	if ( (pDM_Odm->NHMCurTxOkcnt) + 1 > (u8Byte)(pDM_Odm->NHMCurRxOkcnt<<2) + 1)		//Tx > 4*Rx possible for adaptivity test
	{
		if(pDM_Odm->NHM_cnt_0 >= 190 || pDM_Odm->adaptivity_flag == TRUE)
		{
			//Enable EDCCA since it is possible running Adaptivity testing
			//test_status = 1;
            		pDM_Odm->adaptivity_flag = TRUE;
            		ODM_SetMACReg(pDM_Odm, REG_TX_PTCL_CTRL, BIT15, 0);	//don't ignore EDCCA	 reg520[15]=0
			ODM_SetMACReg(pDM_Odm, REG_RD_CTRL, BIT11, 1);	//reg524[11]=1	
			pDM_Odm->tolerance_cnt = 0;
#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
			pDM_Odm->TH_L2H_ini = pDM_Odm->TH_L2H_ini_backup;
			pDM_Odm->TH_EDCCA_HL_diff = pDM_Odm->TH_EDCCA_HL_diff_backup ;
#endif
            	}
		else
		{
			if(pDM_Odm->tolerance_cnt<3)
				pDM_Odm->tolerance_cnt = pDM_Odm->tolerance_cnt + 1;
			else
				pDM_Odm->tolerance_cnt = 4;	
			//test_status = 5;
			if(pDM_Odm->tolerance_cnt > 3)
			{
				//test_status = 3;
#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
			pDM_Odm->TH_L2H_ini = pDM_Odm->TH_L2H_ini_mode2;
			pDM_Odm->TH_EDCCA_HL_diff = pDM_Odm->TH_EDCCA_HL_diff_mode2 ;
#else    			
				ODM_SetMACReg(pDM_Odm, REG_TX_PTCL_CTRL, BIT15, 1);	//ignore EDCCA	reg520[15]=1
            			ODM_SetMACReg(pDM_Odm, REG_RD_CTRL, BIT11, 0);		//reg524[11]=0
#endif
            			pDM_Odm->adaptivity_flag = FALSE;
			}
		}
	}
	else	// TX<RX 
	{
		if(pDM_Odm->adaptivity_flag == TRUE && pDM_Odm->NHM_cnt_0 <= 200)
		{
			//test_status = 2;
			ODM_SetMACReg(pDM_Odm, REG_TX_PTCL_CTRL, BIT15, 0);	//don't ignore EDCCA	 reg520[15]=0
			ODM_SetMACReg(pDM_Odm, REG_RD_CTRL, BIT11, 1);	//reg524[11]=1	
			pDM_Odm->tolerance_cnt = 0;
#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
			pDM_Odm->TH_L2H_ini = pDM_Odm->TH_L2H_ini_backup;
			pDM_Odm->TH_EDCCA_HL_diff = pDM_Odm->TH_EDCCA_HL_diff_backup ;
#endif
		}
		else
		{
			if(pDM_Odm->tolerance_cnt<3)
				pDM_Odm->tolerance_cnt = pDM_Odm->tolerance_cnt + 1;
			else
				pDM_Odm->tolerance_cnt = 4;	
			//test_status = 5;
			if(pDM_Odm->tolerance_cnt >3)
			{
				//test_status = 4;
#if(DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
			pDM_Odm->TH_L2H_ini = pDM_Odm->TH_L2H_ini_mode2;
			pDM_Odm->TH_EDCCA_HL_diff = pDM_Odm->TH_EDCCA_HL_diff_mode2 ;
#else
				ODM_SetMACReg(pDM_Odm, REG_TX_PTCL_CTRL, BIT15, 1);	//ignore EDCCA	reg520[15]=1
            			ODM_SetMACReg(pDM_Odm, REG_RD_CTRL, BIT11, 0);		//reg524[11]=0
#endif
            			pDM_Odm->adaptivity_flag = FALSE;
			}
		}
	}
	 
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("adaptivity_flag = %d\n ", pDM_Odm->adaptivity_flag));
}

VOID
odm_SearchPwdBLowerBound(
	IN		PVOID		pDM_VOID,
	IN		u1Byte		IGI_target
)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	u4Byte			value32 =0;
	u1Byte			cnt, IGI;
	BOOLEAN			bAdjust=TRUE;
	s1Byte 			TH_L2H_dmc, TH_H2L_dmc;
	s1Byte 			TH_L2H, TH_H2L, Diff;

	IGI = 0x50; // find H2L, L2H lower bound
	ODM_Write_DIG(pDM_Odm, IGI);
	

	Diff = IGI_target -(s1Byte)IGI;
	TH_L2H_dmc = pDM_Odm->TH_L2H_ini + Diff;
		if(TH_L2H_dmc > 10) 	
			TH_L2H_dmc = 10;
	TH_H2L_dmc = TH_L2H_dmc - pDM_Odm->TH_EDCCA_HL_diff;
	if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
		{
		ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte0, (u1Byte)TH_L2H_dmc);
		ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte2, (u1Byte)TH_H2L_dmc);
		}
	else
		ODM_SetBBReg(pDM_Odm, rFPGA0_XB_LSSIReadBack, 0xFFFF, ((u1Byte)TH_H2L_dmc<<8) | (u1Byte)TH_L2H_dmc);
					
	ODM_delay_ms(5);
		
		while(bAdjust)
			{
			for(cnt=0; cnt<20; cnt ++)
				{
				if (pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
					value32 = ODM_GetBBReg(pDM_Odm,ODM_REG_RPT_11N, bMaskDWord);
				else if(pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
					value32 = ODM_GetBBReg(pDM_Odm,ODM_REG_RPT_11AC, bMaskDWord);
			
				if (value32 & BIT30 && (pDM_Odm->SupportICType & (ODM_RTL8723A|ODM_RTL8723B|ODM_RTL8188E)))
					pDM_Odm->txEdcca1 = pDM_Odm->txEdcca1 + 1;
				else if(value32 & BIT29)
					pDM_Odm->txEdcca1 = pDM_Odm->txEdcca1 + 1;
				else
					pDM_Odm->txEdcca0 = pDM_Odm->txEdcca0 + 1;
				}
			//DbgPrint("txEdcca1 = %d, txEdcca0 = %d\n", pDM_Odm->txEdcca1, pDM_Odm->txEdcca0);
			
				if(pDM_Odm->txEdcca1 > 5 )
				{
					IGI = IGI -1;
					TH_L2H_dmc = TH_L2H_dmc + 1;
						if(TH_L2H_dmc > 10)
							TH_L2H_dmc = 10;
					TH_H2L_dmc = TH_L2H_dmc - pDM_Odm->TH_EDCCA_HL_diff;
					if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
					{
						ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte0, (u1Byte)TH_L2H_dmc);
						ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte2, (u1Byte)TH_H2L_dmc);
					}
					else
						ODM_SetBBReg(pDM_Odm, rFPGA0_XB_LSSIReadBack, 0xFFFF, ((u1Byte)TH_H2L_dmc<<8) | (u1Byte)TH_L2H_dmc);

					pDM_Odm->TxHangFlg = TRUE;
					pDM_Odm->txEdcca1 = 0;
					pDM_Odm->txEdcca0 = 0;

					if(TH_L2H_dmc == 10)
						{
						bAdjust = FALSE;
						pDM_Odm->TxHangFlg = FALSE;
						pDM_Odm->txEdcca1 = 0;
						pDM_Odm->txEdcca0 = 0;
						pDM_Odm->H2L_lb = TH_H2L_dmc;
						pDM_Odm->L2H_lb = TH_L2H_dmc;
						pDM_Odm->Adaptivity_IGI_upper = IGI;
						}
					}
				else
				{
					bAdjust = FALSE;
					pDM_Odm->TxHangFlg = FALSE;
					pDM_Odm->txEdcca1 = 0;
					pDM_Odm->txEdcca0 = 0;
					pDM_Odm->H2L_lb = TH_H2L_dmc;
					pDM_Odm->L2H_lb = TH_L2H_dmc;	
					pDM_Odm->Adaptivity_IGI_upper = IGI;
				}
			}

							
ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("IGI = 0x%x, H2L_lb = 0x%x, L2H_lb = 0x%x\n", IGI, pDM_Odm->H2L_lb , pDM_Odm->L2H_lb));
		
}

VOID
odm_AdaptivityInit(
	IN 	PVOID	 	pDM_VOID
)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
#if(DM_ODM_SUPPORT_TYPE == ODM_WIN)
	PADAPTER		pAdapter	= pDM_Odm->Adapter;
	PMGNT_INFO		pMgntInfo = &(pAdapter->MgntInfo);
	pDM_Odm->Carrier_Sense_enable = (BOOLEAN)pMgntInfo->RegEnableCarrierSense;
#endif

#if(DM_ODM_SUPPORT_TYPE & (ODM_CE|ODM_WIN))

	if(pDM_Odm->Carrier_Sense_enable == FALSE)
	{
		pDM_Odm->TH_L2H_ini = 0xf7; // -7
	}
	else
		pDM_Odm->TH_L2H_ini = 0xa; 

	pDM_Odm->AdapEn_RSSI = 20;
	pDM_Odm->TH_EDCCA_HL_diff = 7;

#elif (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))

	if(pDM_Odm->Carrier_Sense_enable){
		pDM_Odm->TH_L2H_ini = 10;
		pDM_Odm->TH_EDCCA_HL_diff = 3;		
		pDM_Odm->AdapEn_RSSI = 30;
	}
	else
	{
		pDM_Odm->TH_L2H_ini = 0xf7;
		pDM_Odm->TH_EDCCA_HL_diff = 7;
		pDM_Odm->AdapEn_RSSI = 20;
	}

	pDM_Odm->TH_L2H_ini_mode2 = 20;
	pDM_Odm->TH_EDCCA_HL_diff_mode2 = 8;
	pDM_Odm->TH_L2H_ini_backup = pDM_Odm->TH_L2H_ini;
	pDM_Odm->TH_EDCCA_HL_diff_backup = pDM_Odm->TH_EDCCA_HL_diff ;
	
#endif

	pDM_Odm->IGI_Base = 0x32;	
	pDM_Odm->IGI_target = 0x1c;
	pDM_Odm->ForceEDCCA = 0;
	pDM_Odm->NHM_disable = FALSE;
	pDM_Odm->TxHangFlg = TRUE;
	pDM_Odm->txEdcca0 = 0;
	pDM_Odm->txEdcca1 = 0;
	pDM_Odm->H2L_lb= 0;
	pDM_Odm->L2H_lb= 0;
	pDM_Odm->Adaptivity_IGI_upper = 0;
	odm_NHMBBInit(pDM_Odm);

	ODM_SetBBReg(pDM_Odm, REG_RD_CTRL, BIT11, 1); // stop counting if EDCCA is asserted
}


VOID
odm_Adaptivity(
	IN		PVOID			pDM_VOID,
	IN		u1Byte			IGI
)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	s1Byte TH_L2H_dmc, TH_H2L_dmc;
	s1Byte Diff, IGI_target;
	BOOLEAN EDCCA_State = FALSE;

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	PADAPTER		pAdapter	= pDM_Odm->Adapter;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(pAdapter);
	BOOLEAN		bFwCurrentInPSMode=FALSE;	
	PMGNT_INFO				pMgntInfo = &(pAdapter->MgntInfo);
	
	pAdapter->HalFunc.GetHwRegHandler(pAdapter, HW_VAR_FW_PSMODE_STATUS, (pu1Byte)(&bFwCurrentInPSMode));	

	// Disable EDCCA mode while under LPS mode, added by Roger, 2012.09.14.
	if(bFwCurrentInPSMode)
		return;
#endif

	if(!(pDM_Odm->SupportAbility & ODM_BB_ADAPTIVITY))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("Go to odm_DynamicEDCCA() \n"));
		// Add by Neil Chen to enable edcca to MP Platform 
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
		// Adjust EDCCA.
		if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
			odm_DynamicEDCCA(pDM_Odm);
#endif
		return;
	}
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_Adaptivity() =====> \n"));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("ForceEDCCA=%d, IGI_Base=0x%x, TH_L2H_ini = %d, TH_EDCCA_HL_diff = %d, AdapEn_RSSI = %d\n", 
		pDM_Odm->ForceEDCCA, pDM_Odm->IGI_Base, pDM_Odm->TH_L2H_ini, pDM_Odm->TH_EDCCA_HL_diff, pDM_Odm->AdapEn_RSSI));

	if(pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
		ODM_SetBBReg(pDM_Odm, 0x800, BIT10, 0); //ADC_mask enable

	if(*pDM_Odm->pBandWidth == ODM_BW20M) //CHANNEL_WIDTH_20
		IGI_target = pDM_Odm->IGI_Base;
	else if(*pDM_Odm->pBandWidth == ODM_BW40M)
		IGI_target = pDM_Odm->IGI_Base + 2;
	else if(*pDM_Odm->pBandWidth == ODM_BW80M)
		IGI_target = pDM_Odm->IGI_Base + 2;
	else
		IGI_target = pDM_Odm->IGI_Base;
	pDM_Odm->IGI_target = (u1Byte) IGI_target;
	
	//Search pwdB lower bound
	if(pDM_Odm->TxHangFlg == TRUE)
	{
		if (pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
			ODM_SetBBReg(pDM_Odm,ODM_REG_DBG_RPT_11N, bMaskDWord, 0x208);
		else if(pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
			ODM_SetBBReg(pDM_Odm,ODM_REG_DBG_RPT_11AC, bMaskDWord, 0x209);
		odm_SearchPwdBLowerBound(pDM_Odm, pDM_Odm->IGI_target );
	}
		
	if((!pDM_Odm->bLinked)||(*pDM_Odm->pChannel > 149)) // Band4 doesn't need adaptivity
	{
		if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
	  	{
		   ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte0, 0x7f);
		   ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte2, 0x7f);
	  	}
		else
		   ODM_SetBBReg(pDM_Odm, rFPGA0_XB_LSSIReadBack, 0xFFFF, (0x7f<<8) | 0x7f);
		return;
	}

	if(!pDM_Odm->ForceEDCCA)
	{
		if(pDM_Odm->RSSI_Min > pDM_Odm->AdapEn_RSSI)
			EDCCA_State = 1;
		else if(pDM_Odm->RSSI_Min < (pDM_Odm->AdapEn_RSSI - 5))
			EDCCA_State = 0;
	}
	else
		EDCCA_State = 1;

	if(pDM_Odm->bLinked && pDM_Odm->Carrier_Sense_enable == FALSE && pDM_Odm->NHM_disable == FALSE &&pDM_Odm->TxHangFlg == FALSE)
		odm_NHMBB(pDM_Odm);

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("BandWidth=%s, IGI_target=0x%x, EDCCA_State=%d\n",
		(*pDM_Odm->pBandWidth==ODM_BW80M)?"80M":((*pDM_Odm->pBandWidth==ODM_BW40M)?"40M":"20M"), IGI_target, EDCCA_State));

	if(EDCCA_State == 1)
	{
		Diff = IGI_target -(s1Byte)IGI;
		TH_L2H_dmc = pDM_Odm->TH_L2H_ini + Diff;
		if(TH_L2H_dmc > 10) 	
			TH_L2H_dmc = 10;
				
		TH_H2L_dmc = TH_L2H_dmc - pDM_Odm->TH_EDCCA_HL_diff;

		//replace lower bound to prevent EDCCA always equal 1
			if(TH_H2L_dmc < pDM_Odm->H2L_lb)				
				TH_H2L_dmc = pDM_Odm->H2L_lb;
			if(TH_L2H_dmc < pDM_Odm->L2H_lb)
				TH_L2H_dmc = pDM_Odm->L2H_lb;
	}
	else
	{
		TH_L2H_dmc = 0x7f;
		TH_H2L_dmc = 0x7f;
	}
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("IGI=0x%x, TH_L2H_dmc = %d, TH_H2L_dmc = %d\n", 
		IGI, TH_L2H_dmc, TH_H2L_dmc));
	if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
	{
		ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte0, (u1Byte)TH_L2H_dmc);
		ODM_SetBBReg(pDM_Odm,rOFDM0_ECCAThreshold, bMaskByte2, (u1Byte)TH_H2L_dmc);
	}
	else
		ODM_SetBBReg(pDM_Odm, rFPGA0_XB_LSSIReadBack, 0xFFFF, ((u1Byte)TH_H2L_dmc<<8) | (u1Byte)TH_L2H_dmc);
}

int 
getIGIForDiff(int value_IGI)
{
	#define ONERCCA_LOW_TH		0x30
	#define ONERCCA_LOW_DIFF	8

	if (value_IGI < ONERCCA_LOW_TH) {
		if ((ONERCCA_LOW_TH - value_IGI) < ONERCCA_LOW_DIFF)
			return ONERCCA_LOW_TH;
		else
			return value_IGI + ONERCCA_LOW_DIFF;
	} else {
		return value_IGI;
	}
}

VOID
ODM_Write_DIG(
	IN	PVOID			pDM_VOID,
	IN	u1Byte			CurrentIGI
	)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T			pDM_DigTable = &pDM_Odm->DM_DigTable;

	if(pDM_DigTable->bStopDIG)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("Stop Writing IGI\n"));
		return;
	}

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_TRACE, ("ODM_REG(IGI_A,pDM_Odm)=0x%x, ODM_BIT(IGI,pDM_Odm)=0x%x \n",
		ODM_REG(IGI_A,pDM_Odm),ODM_BIT(IGI,pDM_Odm)));

	if(pDM_DigTable->CurIGValue != CurrentIGI)
	{
		//1 Check initial gain by upper bound		
		if(!pDM_DigTable->bPSDInProgress)
		{
			if(CurrentIGI > pDM_DigTable->rx_gain_range_max)
			{
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_TRACE, ("CurrentIGI(0x%02x) is larger than upper bound !!\n",pDM_DigTable->rx_gain_range_max));
				CurrentIGI = pDM_DigTable->rx_gain_range_max;
			}

		}

		//1 Set IGI value
		if(pDM_Odm->SupportPlatform & (ODM_WIN|ODM_CE))
		{ 
			ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_A,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);

			if(pDM_Odm->RFType > ODM_1T1R)
				ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_B,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);

			if((pDM_Odm->SupportICType & ODM_IC_11AC_SERIES) && (pDM_Odm->RFType > ODM_2T2R))
			{
				ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_C,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
				ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_D,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
			}
		}
		else if(pDM_Odm->SupportPlatform & (ODM_AP|ODM_ADSL))
		{
			switch(*(pDM_Odm->pOnePathCCA))
			{
				case ODM_CCA_2R:
					ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_A,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);

					if(pDM_Odm->RFType > ODM_1T1R)
						ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_B,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
					
					if((pDM_Odm->SupportICType & ODM_IC_11AC_SERIES) && (pDM_Odm->RFType > ODM_2T2R))
					{
						ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_C,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
						ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_D,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
					}
					break;
				case ODM_CCA_1R_A:
					ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_A,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
					if(pDM_Odm->RFType != ODM_1T1R)
						ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_B,pDM_Odm), ODM_BIT(IGI,pDM_Odm), getIGIForDiff(CurrentIGI));
					break;
				case ODM_CCA_1R_B:
					ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_A,pDM_Odm), ODM_BIT(IGI,pDM_Odm), getIGIForDiff(CurrentIGI));
					if(pDM_Odm->RFType != ODM_1T1R)
						ODM_SetBBReg(pDM_Odm, ODM_REG(IGI_B,pDM_Odm), ODM_BIT(IGI,pDM_Odm), CurrentIGI);
					break;
			}
		}
		pDM_DigTable->CurIGValue = CurrentIGI;
	}

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_TRACE, ("CurrentIGI(0x%02x). \n",CurrentIGI));
	
}

VOID
odm_PauseDIG(
	IN		PVOID					pDM_VOID,
	IN		ODM_Pause_DIG_TYPE		PauseType,
	IN		u1Byte					IGIValue
)
{
	PDM_ODM_T			pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T				pDM_DigTable = &pDM_Odm->DM_DigTable;
	static	BOOLEAN		bPaused = FALSE;

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG()=========>\n"));

#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)	
	if(*pDM_DigTable->pbP2pLinkInProgress)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): P2P in progress !!\n"));
		return;
	}
#endif

	if((pDM_Odm->SupportAbility & ODM_BB_ADAPTIVITY) && pDM_Odm->TxHangFlg == TRUE)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Dynamic adjust threshold in progress !!\n"));
		return;
	}

	if(!bPaused && (!(pDM_Odm->SupportAbility & ODM_BB_DIG) || !(pDM_Odm->SupportAbility & ODM_BB_FA_CNT)))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Return: SupportAbility ODM_BB_DIG or ODM_BB_FA_CNT is disabled\n"));
		return;
	}
	
	switch(PauseType)
	{
		//1 Pause DIG
		case ODM_PAUSE_DIG:
			//2 Disable DIG
			ODM_CmnInfoUpdate(pDM_Odm, ODM_CMNINFO_ABILITY, pDM_Odm->SupportAbility & (~ODM_BB_DIG));
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Pause DIG !!\n"));

			//2 Backup IGI value
			if(!bPaused)
			{
				pDM_DigTable->IGIBackup = pDM_DigTable->CurIGValue;
				bPaused = TRUE;
			}
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Backup IGI  = 0x%x\n", pDM_DigTable->IGIBackup));

			//2 Write new IGI value
			ODM_Write_DIG(pDM_Odm, IGIValue);
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Write new IGI = 0x%x\n", IGIValue));
			break;

		//1 Resume DIG
		case ODM_RESUME_DIG:
			if(bPaused)
			{
				//2 Write backup IGI value
				ODM_Write_DIG(pDM_Odm, pDM_DigTable->IGIBackup);
				bPaused = FALSE;
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Write original IGI = 0x%x\n", pDM_DigTable->IGIBackup));

				//2 Enable DIG
				ODM_CmnInfoUpdate(pDM_Odm, ODM_CMNINFO_ABILITY, pDM_Odm->SupportAbility | ODM_BB_DIG);	
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Resume DIG !!\n"));
			}
			break;

		default:
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_PauseDIG(): Wrong  type !!\n"));
			break;
	}
}

BOOLEAN 
odm_DigAbort(
	IN		PVOID			pDM_VOID
	)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	prtl8192cd_priv				priv = pDM_Odm->priv;
#elif(DM_ODM_SUPPORT_TYPE & ODM_WIN)
	PADAPTER		pAdapter	= pDM_Odm->Adapter;
	pRXHP_T			pRX_HP_Table  = &pDM_Odm->DM_RXHP_Table;
#endif

	//SupportAbility
	if(!(pDM_Odm->SupportAbility & ODM_BB_FA_CNT))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: SupportAbility ODM_BB_FA_CNT is disabled\n"));
		return	TRUE;
	}

	//SupportAbility
	if(!(pDM_Odm->SupportAbility & ODM_BB_DIG))
	{	
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: SupportAbility ODM_BB_DIG is disabled\n"));
		return	TRUE;
	}

	//ScanInProcess
	if(*(pDM_Odm->pbScanInProcess))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: In Scan Progress \n"));
	    	return	TRUE;
	}

	//add by Neil Chen to avoid PSD is processing
	if(pDM_Odm->bDMInitialGainEnable == FALSE)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: PSD is Processing \n"));
		return	TRUE;
	}

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	#if OS_WIN_FROM_WIN7(OS_VERSION)
	if(IsAPModeExist( pAdapter) && pAdapter->bInHctTest)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: Is AP mode or In HCT Test \n"));
	    	return	TRUE;
	}
	#endif

	if(pDM_Odm->bBtHsOperation)
	{
		odm_DigForBtHsMode(pDM_Odm);
	}	

	if(!(pDM_Odm->SupportICType &(ODM_RTL8723A|ODM_RTL8188E)))
	{
		if(pRX_HP_Table->RXHP_flag == 1)
		{
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: In RXHP Operation \n"));
			return	TRUE;	
		}
	}
#elif (DM_ODM_SUPPORT_TYPE == ODM_CE)
	#ifdef CONFIG_SPECIAL_SETTING_FOR_FUNAI_TV	
	if((pDM_Odm->bLinked) && (pDM_Odm->Adapter->registrypriv.force_igi !=0))
	{	
		printk("pDM_Odm->RSSI_Min=%d \n",pDM_Odm->RSSI_Min);
		ODM_Write_DIG(pDM_Odm,pDM_Odm->Adapter->registrypriv.force_igi);
		return	TRUE;
	}
	#endif
#else
	if (!(priv->up_time > 5))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Return: Not In DIG Operation Period \n"));
		return	TRUE;
	}
#endif

	return	FALSE;
}

VOID
odm_DIGInit(
	IN		PVOID		pDM_VOID
	)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T						pDM_DigTable = &pDM_Odm->DM_DigTable;
	PFALSE_ALARM_STATISTICS 	FalseAlmCnt = &(pDM_Odm->FalseAlmCnt);

	pDM_DigTable->bStopDIG = FALSE;
	pDM_DigTable->bPSDInProgress = FALSE;
	pDM_DigTable->CurIGValue = (u1Byte) ODM_GetBBReg(pDM_Odm, ODM_REG(IGI_A,pDM_Odm), ODM_BIT(IGI,pDM_Odm));
	pDM_DigTable->RssiLowThresh 	= DM_DIG_THRESH_LOW;
	pDM_DigTable->RssiHighThresh 	= DM_DIG_THRESH_HIGH;
	pDM_DigTable->FALowThresh	= DM_FALSEALARM_THRESH_LOW;
	pDM_DigTable->FAHighThresh	= DM_FALSEALARM_THRESH_HIGH;
	pDM_DigTable->BackoffVal = DM_DIG_BACKOFF_DEFAULT;
	pDM_DigTable->BackoffVal_range_max = DM_DIG_BACKOFF_MAX;
	pDM_DigTable->BackoffVal_range_min = DM_DIG_BACKOFF_MIN;
	pDM_DigTable->PreCCK_CCAThres = 0xFF;
	pDM_DigTable->CurCCK_CCAThres = 0x83;
	pDM_DigTable->ForbiddenIGI = DM_DIG_MIN_NIC;
	pDM_DigTable->LargeFAHit = 0;
	pDM_DigTable->Recover_cnt = 0;
	pDM_DigTable->bMediaConnect_0 = FALSE;
	pDM_DigTable->bMediaConnect_1 = FALSE;

	//To Initialize pDM_Odm->bDMInitialGainEnable == FALSE to avoid DIG error
	pDM_Odm->bDMInitialGainEnable = TRUE;

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	pDM_DigTable->DIG_Dynamic_MIN_0 = 0x25;
	pDM_DigTable->DIG_Dynamic_MIN_1 = 0x25;

	// For AP\ ADSL modified DIG
	pDM_DigTable->bTpTarget = FALSE;
	pDM_DigTable->bNoiseEst = TRUE;
	pDM_DigTable->IGIOffset_A = 0;
	pDM_DigTable->IGIOffset_B = 0;
	pDM_DigTable->TpTrainTH_min = 0;

	// For RTL8881A
	FalseAlmCnt->Cnt_Ofdm_fail_pre = 0;

	//Dyanmic EDCCA
	if(pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
	{
		ODM_SetBBReg(pDM_Odm, 0xC50, 0xFFFF0000, 0xfafd);
	}
#else
	pDM_DigTable->DIG_Dynamic_MIN_0 = DM_DIG_MIN_NIC;
	pDM_DigTable->DIG_Dynamic_MIN_1 = DM_DIG_MIN_NIC;

	//To Initi BT30 IGI
	pDM_DigTable->BT30_CurIGI=0x32;

	#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
	*pDM_DigTable->pbP2pLinkInProgress= FALSE;
	#endif
#endif

	if(pDM_Odm->BoardType & (ODM_BOARD_EXT_PA|ODM_BOARD_EXT_LNA))
	{
		pDM_DigTable->rx_gain_range_max = DM_DIG_MAX_NIC;
		pDM_DigTable->rx_gain_range_min = DM_DIG_MIN_NIC;
	}
	else
	{
		pDM_DigTable->rx_gain_range_max = DM_DIG_MAX_NIC;
		pDM_DigTable->rx_gain_range_min = DM_DIG_MIN_NIC;
	}
	
}


VOID 
odm_DIG(
	IN		PVOID		pDM_VOID
	)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
	PADAPTER					pAdapter	= pDM_Odm->Adapter;
	HAL_DATA_TYPE				*pHalData = GET_HAL_DATA(pDM_Odm->Adapter);
#elif (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	prtl8192cd_priv				priv = pDM_Odm->priv;
	PSTA_INFO_T   				pEntry;
#endif

	// Common parameters
	pDIG_T						pDM_DigTable = &pDM_Odm->DM_DigTable;
	PFALSE_ALARM_STATISTICS		pFalseAlmCnt = &pDM_Odm->FalseAlmCnt;
	BOOLEAN						FirstConnect,FirstDisConnect;
	u1Byte						DIG_MaxOfMin, DIG_Dynamic_MIN, i;
	u1Byte						dm_dig_max, dm_dig_min;
	u1Byte						CurrentIGI = pDM_DigTable->CurIGValue;
	u1Byte						offset;
	u4Byte						dm_FA_thres[3];
	u1Byte						Adap_IGI_Upper = 0;
	u4Byte						TxTp = 0, RxTp = 0;
	BOOLEAN						bDFSBand = FALSE;
	BOOLEAN						bPerformance = TRUE, bFirstTpTarget = FALSE, bFirstCoverage = FALSE;
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	u4Byte						TpTrainTH_MIN = DM_DIG_TP_Target_TH0;
	static		u1Byte			TimeCnt = 0;
#endif

	if(odm_DigAbort(pDM_Odm) == TRUE)
		return;

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG()===========================>\n\n"));

	if(pDM_Odm->adaptivity_flag == TRUE)
		Adap_IGI_Upper = pDM_Odm->Adaptivity_IGI_upper;
	

	//1 Update status
	if(pDM_Odm->SupportICType == ODM_RTL8192D)
	{
		if(*(pDM_Odm->pMacPhyMode) == ODM_DMSP)
		{
			if(*(pDM_Odm->pbMasterOfDMSP))
			{
				DIG_Dynamic_MIN = pDM_DigTable->DIG_Dynamic_MIN_0;
				FirstConnect = (pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_0 == FALSE);
				FirstDisConnect = (!pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_0 == TRUE);
			}
			else
			{
				DIG_Dynamic_MIN = pDM_DigTable->DIG_Dynamic_MIN_1;
				FirstConnect = (pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_1 == FALSE);
				FirstDisConnect = (!pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_1 == TRUE);
			}
		}
		else
		{
			if(*(pDM_Odm->pBandType) == ODM_BAND_5G)
			{
				DIG_Dynamic_MIN = pDM_DigTable->DIG_Dynamic_MIN_0;
				FirstConnect = (pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_0 == FALSE);
				FirstDisConnect = (!pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_0 == TRUE);
			}
			else
			{
				DIG_Dynamic_MIN = pDM_DigTable->DIG_Dynamic_MIN_1;
				FirstConnect = (pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_1 == FALSE);
				FirstDisConnect = (!pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_1 == TRUE);
			}
		}
	}
	else
	{	
		DIG_Dynamic_MIN = pDM_DigTable->DIG_Dynamic_MIN_0;
		FirstConnect = (pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_0 == FALSE);
		FirstDisConnect = (!pDM_Odm->bLinked) && (pDM_DigTable->bMediaConnect_0 == TRUE);
	}

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	//1 Noise Floor Estimate
	//pDM_DigTable->bNoiseEst = (FirstConnect)?TRUE:pDM_DigTable->bNoiseEst;
	//odm_InbandNoiseCalculate (pDM_Odm);
	
	//1 Mode decision
	if(pDM_Odm->bLinked)
	{
		//2 Calculate total TP
		for (i=0; i<ODM_ASSOCIATE_ENTRY_NUM; i++)
		{
			pEntry = pDM_Odm->pODM_StaInfo[i];
			if(IS_STA_VALID(pEntry))
			{
				RxTp += (u4Byte)(pEntry->rx_byte_cnt_LowMAW>>7);
				TxTp += (u4Byte)(pEntry->tx_byte_cnt_LowMAW>>7);			//Kbps
			}
		}
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): TX TP = %dkbps, RX TP = %dkbps\n", TxTp, RxTp));
	}

	switch(pDM_Odm->priv->pshare->rf_ft_var.dig_cov_enable)
	{
		case 0:
		{
			bPerformance = TRUE;
			break;
		}
		case 1:
		{
			bPerformance = FALSE;
			break;
		}
		case 2:
		{
			if(pDM_Odm->bLinked)
			{
				if(pDM_DigTable->TpTrainTH_min > DM_DIG_TP_Target_TH0)
					TpTrainTH_MIN = pDM_DigTable->TpTrainTH_min;

				if(pDM_DigTable->TpTrainTH_min > DM_DIG_TP_Target_TH1)
					TpTrainTH_MIN = DM_DIG_TP_Target_TH1;

				ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): TP training mode lower bound = %dkbps\n", TpTrainTH_MIN));

				//2 Decide DIG mode by total TP
				if((TxTp + RxTp) > DM_DIG_TP_Target_TH1)			// change to performance mode
				{
					bFirstTpTarget = (!pDM_DigTable->bTpTarget)?TRUE:FALSE;
					pDM_DigTable->bTpTarget = TRUE;
					bPerformance = TRUE;
				}
				else if((TxTp + RxTp) < TpTrainTH_MIN)	// change to coverage mode
				{
					bFirstCoverage = (pDM_DigTable->bTpTarget)?TRUE:FALSE;
					
					if(TimeCnt < DM_DIG_TP_Training_Period)
					{
						pDM_DigTable->bTpTarget = FALSE;
						bPerformance = FALSE;
						TimeCnt++;
					}
					else
					{
						pDM_DigTable->bTpTarget = TRUE;
						bPerformance = TRUE;
						bFirstTpTarget = TRUE;
						TimeCnt = 0;
					}
				}
				else										// remain previous mode
				{
					bPerformance = pDM_DigTable->bTpTarget;

					if(!bPerformance)
					{
						if(TimeCnt < DM_DIG_TP_Training_Period)
							TimeCnt++;
						else
						{
							pDM_DigTable->bTpTarget = TRUE;
							bPerformance = TRUE;
							bFirstTpTarget = TRUE;
							TimeCnt = 0;
						}
					}
				}

				if(!bPerformance)
					pDM_DigTable->TpTrainTH_min = RxTp + TxTp;

			}
			else
			{
				bPerformance = FALSE;
				pDM_DigTable->TpTrainTH_min = 0;
			}
			break;
		}
		default:
			bPerformance = TRUE;
	}

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("====== DIG mode = %d  ======\n", pDM_Odm->priv->pshare->rf_ft_var.dig_cov_enable));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("====== bPerformance = %d ======\n", bPerformance));
#endif

	//1 Boundary Decision
	if((pDM_Odm->SupportICType & ODM_RTL8192C) && (pDM_Odm->BoardType & (ODM_BOARD_EXT_LNA | ODM_BOARD_EXT_PA)))
	{
		//2 High power case
		if(pDM_Odm->SupportPlatform & (ODM_AP|ODM_ADSL))
		{
			dm_dig_max = DM_DIG_MAX_AP_HP;
			dm_dig_min = DM_DIG_MIN_AP_HP;
		}
		else
		{
			dm_dig_max = DM_DIG_MAX_NIC_HP;
			dm_dig_min = DM_DIG_MIN_NIC_HP;
		}
		DIG_MaxOfMin = DM_DIG_MAX_AP_HP;
	}
	else
	{
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
		//2 For AP\ADSL
		if(!bPerformance)
		{
			dm_dig_max = DM_DIG_MAX_AP_COVERAGR;
			dm_dig_min = DM_DIG_MIN_AP_COVERAGE;
			DIG_MaxOfMin = DM_DIG_MAX_OF_MIN_COVERAGE;
		}
		else
		{
			dm_dig_max = DM_DIG_MAX_AP;
			dm_dig_min = DM_DIG_MIN_AP;
			DIG_MaxOfMin = DM_DIG_MAX_OF_MIN;
		}

		//4 DFS band
		if (((*pDM_Odm->pChannel>= 52) &&(*pDM_Odm->pChannel <= 64)) ||
			((*pDM_Odm->pChannel >= 100) &&	(*pDM_Odm->pChannel <= 140)))
		{
			bDFSBand = TRUE;
			dm_dig_min = DM_DIG_MIN_AP_DFS;
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): ====== In DFS band ======\n"));
		}
		
		//4 TX2path
		if (priv->pmib->dot11RFEntry.tx2path && !bDFSBand && (*(pDM_Odm->pWirelessMode) == ODM_WM_B))
				dm_dig_max = 0x2A;

#if RTL8192E_SUPPORT
#ifdef HIGH_POWER_EXT_LNA
		if ((pDM_Odm->SupportICType & (ODM_RTL8192E)) && (pDM_Odm->ExtLNA))
			dm_dig_max = 0x42;						
#endif
#endif

#else
		//2 For WIN\CE
		if(pDM_Odm->SupportICType >= ODM_RTL8188E)
			dm_dig_max = 0x5A;
		else
			dm_dig_max = DM_DIG_MAX_NIC;
		
		if(pDM_Odm->SupportICType != ODM_RTL8821)
			dm_dig_min = DM_DIG_MIN_NIC;
		else
			dm_dig_min = 0x1C;

		DIG_MaxOfMin = DM_DIG_MAX_AP;
#endif	
	}
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Absolutly upper bound = 0x%x, lower bound = 0x%x\n",dm_dig_max, dm_dig_min));

#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
	// for P2P case
	if(0 < *pDM_Odm->pu1ForcedIgiLb)
	{
		ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): P2P case: Force IGI lb to: %u !!!!!!\n", *pDM_Odm->pu1ForcedIgiLb));
		dm_dig_min = *pDM_Odm->pu1ForcedIgiLb;
		dm_dig_max = (dm_dig_min <= dm_dig_max) ? (dm_dig_max) : (dm_dig_min + 1);
	}
#endif

	//1 Adjust boundary by RSSI
	if(pDM_Odm->bLinked && bPerformance)
	{
		//2 Modify DIG upper bound
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
		offset = 15;
#else
		//4 Modify DIG upper bound for 92E, 8723A\B, 8821 & 8812 BT
		if((pDM_Odm->SupportICType & (ODM_RTL8192E|ODM_RTL8723B|ODM_RTL8812|ODM_RTL8821|ODM_RTL8723A)) && (pDM_Odm->bBtLimitedDig==1))
		{
			offset = 10;
			ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Coex. case: Force upper bound to RSSI + %d !!!!!!\n", offset));		
		}
		else
			offset = 15;
#endif

		if((pDM_Odm->RSSI_Min + offset) > dm_dig_max )
			pDM_DigTable->rx_gain_range_max = dm_dig_max;
		else if((pDM_Odm->RSSI_Min + offset) < dm_dig_min )
			pDM_DigTable->rx_gain_range_max = dm_dig_min;
		else
			pDM_DigTable->rx_gain_range_max = pDM_Odm->RSSI_Min + offset;

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
		//2 Modify DIG lower bound
		//if(pDM_Odm->bOneEntryOnly)
		{
			if(pDM_Odm->RSSI_Min < dm_dig_min)
				DIG_Dynamic_MIN = dm_dig_min;
			else if (pDM_Odm->RSSI_Min > DIG_MaxOfMin)
				DIG_Dynamic_MIN = DIG_MaxOfMin;
			else
				DIG_Dynamic_MIN = pDM_Odm->RSSI_Min;
		}
#else
		{
			//4 For AP
#ifdef __ECOS
			HAL_REORDER_BARRIER();
#else
			rmb();
#endif
			if (bDFSBand)
			{
				DIG_Dynamic_MIN = dm_dig_min;
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): DFS band: Force lower bound to 0x%x after link !!!!!!\n", dm_dig_min));
			}
			else 
			{
				if(pDM_Odm->RSSI_Min < dm_dig_min)
					DIG_Dynamic_MIN = dm_dig_min;
				else if (pDM_Odm->RSSI_Min > DIG_MaxOfMin)
					DIG_Dynamic_MIN = DIG_MaxOfMin;
				else
					DIG_Dynamic_MIN = pDM_Odm->RSSI_Min;
			}
		}
#endif
	}
	else
	{
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
		if(bPerformance && bDFSBand)
		{
			pDM_DigTable->rx_gain_range_max = 0x28;
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): DFS band: Force upper bound to 0x%x before link !!!!!!\n", pDM_DigTable->rx_gain_range_max));
		}
		else
#endif
		{
			pDM_DigTable->rx_gain_range_max = dm_dig_max;
		}
		DIG_Dynamic_MIN = dm_dig_min;
	}
	
	//1 Force Lower Bound for AntDiv
	if(pDM_Odm->bLinked && !pDM_Odm->bOneEntryOnly)
	{
		if((pDM_Odm->SupportICType & ODM_ANTDIV_SUPPORT) && (pDM_Odm->SupportAbility & ODM_BB_ANT_DIV))
		{
			if(pDM_Odm->AntDivType == CG_TRX_HW_ANTDIV || pDM_Odm->AntDivType == CG_TRX_SMART_ANTDIV ||pDM_Odm->AntDivType == S0S1_SW_ANTDIV)
			{
				if(pDM_DigTable->AntDiv_RSSI_max > DIG_MaxOfMin)
					DIG_Dynamic_MIN = DIG_MaxOfMin;
				else
					DIG_Dynamic_MIN = (u1Byte) pDM_DigTable->AntDiv_RSSI_max;
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("odm_DIG(): Antenna diversity case: Force lower bound to 0x%x !!!!!!\n", DIG_Dynamic_MIN));
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("odm_DIG(): Antenna diversity case: RSSI_max = 0x%x !!!!!!\n", pDM_DigTable->AntDiv_RSSI_max));
			}
		}
	}
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Adjust boundary by RSSI Upper bound = 0x%x, Lower bound = 0x%x\n",
		pDM_DigTable->rx_gain_range_max, DIG_Dynamic_MIN));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Link status: bLinked = %d, RSSI = %d, bFirstConnect = %d, bFirsrDisConnect = %d\n\n",
		pDM_Odm->bLinked, pDM_Odm->RSSI_Min, FirstConnect, FirstDisConnect));

	//1 Modify DIG lower bound, deal with abnormal case
	//2 Abnormal false alarm case
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	if(bDFSBand)
	{
		pDM_DigTable->rx_gain_range_min = DIG_Dynamic_MIN;
	}
	else
#endif
	{
		if(FirstDisConnect)
		{
			pDM_DigTable->rx_gain_range_min = DIG_Dynamic_MIN;
			pDM_DigTable->ForbiddenIGI = DIG_Dynamic_MIN;
		}
		else
			pDM_DigTable->rx_gain_range_min = odm_ForbiddenIGICheck(pDM_Odm, DIG_Dynamic_MIN, CurrentIGI);
	}

	//2 Abnormal # beacon case
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)
		if(IS_STA_VALID(pDM_Odm->pODM_StaInfo[0]) && !ACTING_AS_AP(pAdapter))
			pDM_Odm->bsta_state = TRUE;
#endif


#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
	if(pDM_Odm->bLinked && !FirstConnect)
	{
		if((pDM_Odm->PhyDbgInfo.NumQryBeaconPkt < 5) && (pDM_Odm->bsta_state))
		{
			pDM_DigTable->rx_gain_range_min = dm_dig_min;
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Abnrormal #beacon (%d) case in STA mode: Force lower bound to 0x%x !!!!!!\n\n",
				pDM_Odm->PhyDbgInfo.NumQryBeaconPkt, pDM_DigTable->rx_gain_range_min));
		}
	}
#endif

	//2 Abnormal lower bound case
	if(pDM_DigTable->rx_gain_range_min > pDM_DigTable->rx_gain_range_max)
	{
		pDM_DigTable->rx_gain_range_min = pDM_DigTable->rx_gain_range_max;
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Abnrormal lower bound case: Force lower bound to 0x%x !!!!!!\n\n",pDM_DigTable->rx_gain_range_min));
	}

	
	//1 False alarm threshold decision
	odm_FAThresholdCheck(pDM_Odm, bDFSBand, bPerformance, RxTp, TxTp, dm_FA_thres);
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): False alarm threshold = %d, %d, %d \n\n", dm_FA_thres[0], dm_FA_thres[1], dm_FA_thres[2]));

	//1 Adjust initial gain by false alarm
	if(pDM_Odm->bLinked && bPerformance)
	{
		//2 After link
		ODM_RT_TRACE(pDM_Odm,	ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Adjust IGI after link\n"));

		if(bFirstTpTarget || (FirstConnect && bPerformance))
		{	
			pDM_DigTable->LargeFAHit = 0;
			
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
			if(bDFSBand)
			{
				if(pDM_Odm->RSSI_Min > 0x28)
					CurrentIGI = 0x28;
				else
					CurrentIGI = pDM_Odm->RSSI_Min;
				ODM_RT_TRACE(pDM_Odm,	ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): DFS band: One-shot to 0x28 upmost!!!!!!\n"));
			}
			else
#endif
			{
				if(pDM_Odm->RSSI_Min < DIG_MaxOfMin)
				{
					if(CurrentIGI < pDM_Odm->RSSI_Min)
						CurrentIGI = pDM_Odm->RSSI_Min;
				}
				else
				{
					if(CurrentIGI < DIG_MaxOfMin)
						CurrentIGI = DIG_MaxOfMin;
				}

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
				if(pDM_Odm->SupportICType == ODM_RTL8812)
					ODM_ConfigBBWithHeaderFile(pDM_Odm, CONFIG_BB_AGC_TAB_DIFF);
#endif
			}

			ODM_RT_TRACE(pDM_Odm,	ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): First connect case: IGI does on-shot to 0x%x\n", CurrentIGI));

		}
		else
		{
			if(pFalseAlmCnt->Cnt_all > dm_FA_thres[2])
				CurrentIGI = CurrentIGI + 4;
			else if (pFalseAlmCnt->Cnt_all > dm_FA_thres[1])
				CurrentIGI = CurrentIGI + 2;
			else if(pFalseAlmCnt->Cnt_all < dm_FA_thres[0])
				CurrentIGI = CurrentIGI - 2;

			//4 Abnormal # beacon case
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
			if(IS_STA_VALID(pDM_Odm->pODM_StaInfo[0]) && !ACTING_AS_AP(pAdapter)) //STA mode is linked to AP
				pDM_Odm->bsta_state = _TRUE;
#endif

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
			if((pDM_Odm->PhyDbgInfo.NumQryBeaconPkt < 5) && (pFalseAlmCnt->Cnt_all < DM_DIG_FA_TH1) && (pDM_Odm->bsta_state))
			{						
				CurrentIGI = pDM_DigTable->rx_gain_range_min;
				ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Abnormal #beacon (%d) case: IGI does one-shot to 0x%x\n", 
					pDM_Odm->PhyDbgInfo.NumQryBeaconPkt, CurrentIGI));
			}
#endif
		}
	}	
	else
	{
		//2 Before link
		ODM_RT_TRACE(pDM_Odm,	ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Adjust IGI before link\n"));
		
		if(FirstDisConnect || bFirstCoverage)
		{
			CurrentIGI = dm_dig_min;
			ODM_RT_TRACE(pDM_Odm,	ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): First disconnect case: IGI does on-shot to lower bound\n"));
		}
		else
		{
			if(pFalseAlmCnt->Cnt_all > dm_FA_thres[2])
				CurrentIGI = CurrentIGI + 4;
			else if (pFalseAlmCnt->Cnt_all > dm_FA_thres[1])
				CurrentIGI = CurrentIGI + 2;
			else if(pFalseAlmCnt->Cnt_all < dm_FA_thres[0])
				CurrentIGI = CurrentIGI - 2;
		}
	}

	//1 Check initial gain by upper/lower bound
	if(CurrentIGI < pDM_DigTable->rx_gain_range_min)
		CurrentIGI = pDM_DigTable->rx_gain_range_min;
	
	if(CurrentIGI > pDM_DigTable->rx_gain_range_max)
		CurrentIGI = pDM_DigTable->rx_gain_range_max;

	ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): CurIGValue=0x%x, TotalFA = %d\n\n", CurrentIGI, pFalseAlmCnt->Cnt_all));

	//1 Force upper bound and lower bound for adaptivity
	if(pDM_Odm->SupportAbility & ODM_BB_ADAPTIVITY && pDM_Odm->adaptivity_flag == TRUE)
	{
		if(CurrentIGI > Adap_IGI_Upper)
			CurrentIGI = Adap_IGI_Upper;
		
		if(pDM_Odm->IGI_LowerBound != 0)
		{
			if(CurrentIGI < pDM_Odm->IGI_LowerBound)
				CurrentIGI = pDM_Odm->IGI_LowerBound;
		}
		ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Adaptivity case: Force upper bound to 0x%x !!!!!!\n", Adap_IGI_Upper));
		ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Adaptivity case: Force lower bound to 0x%x !!!!!!\n\n", pDM_Odm->IGI_LowerBound));
	}
	

	//1 High power RSSI threshold
#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)	
	if((pDM_Odm->SupportICType == ODM_RTL8723A)&& (pHalData->UndecoratedSmoothedPWDB > DM_DIG_HIGH_PWR_THRESHOLD))
	{
		// High power IGI lower bound
		ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): UndecoratedSmoothedPWDB(%#x)\n", pHalData->UndecoratedSmoothedPWDB));
		if(CurrentIGI < DM_DIG_HIGH_PWR_IGI_LOWER_BOUND)
		{
			ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): CurIGValue(%#x)\n", pDM_DigTable->CurIGValue));
			//pDM_DigTable->CurIGValue = DM_DIG_HIGH_PWR_IGI_LOWER_BOUND;
			CurrentIGI=DM_DIG_HIGH_PWR_IGI_LOWER_BOUND;
		}
	}
	if((pDM_Odm->SupportICType & ODM_RTL8723A) && IS_WIRELESS_MODE_G(pAdapter))
	{
		if(pHalData->UndecoratedSmoothedPWDB > 0x28)
		{
			if(CurrentIGI < DM_DIG_Gmode_HIGH_PWR_IGI_LOWER_BOUND)
			{
			 	//pDM_DigTable->CurIGValue = DM_DIG_Gmode_HIGH_PWR_IGI_LOWER_BOUND;
				CurrentIGI = DM_DIG_Gmode_HIGH_PWR_IGI_LOWER_BOUND;
			}	
		} 
	}
#endif

	//1 Update status
#if (RTL8192D_SUPPORT==1) 
	if(pDM_Odm->SupportICType == ODM_RTL8192D)
	{
		//sherry  delete DualMacSmartConncurrent 20110517
		if(*(pDM_Odm->pMacPhyMode) == ODM_DMSP)
		{
			ODM_Write_DIG_DMSP(pDM_Odm, CurrentIGI);//ODM_Write_DIG_DMSP(pDM_Odm, pDM_DigTable->CurIGValue);
			if(*(pDM_Odm->pbMasterOfDMSP))
			{
				pDM_DigTable->bMediaConnect_0 = pDM_Odm->bLinked;
				pDM_DigTable->DIG_Dynamic_MIN_0 = DIG_Dynamic_MIN;
			}
			else
			{
				pDM_DigTable->bMediaConnect_1 = pDM_Odm->bLinked;
				pDM_DigTable->DIG_Dynamic_MIN_1 = DIG_Dynamic_MIN;
			}
		}
		else
		{
			ODM_Write_DIG(pDM_Odm, CurrentIGI);//ODM_Write_DIG(pDM_Odm, pDM_DigTable->CurIGValue);
			if(*(pDM_Odm->pBandType) == ODM_BAND_5G)
			{
				pDM_DigTable->bMediaConnect_0 = pDM_Odm->bLinked;
				pDM_DigTable->DIG_Dynamic_MIN_0 = DIG_Dynamic_MIN;
			}
			else
			{
				pDM_DigTable->bMediaConnect_1 = pDM_Odm->bLinked;
				pDM_DigTable->DIG_Dynamic_MIN_1 = DIG_Dynamic_MIN;
			}
		}
	}
	else
#endif
	{
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
		if(pDM_Odm->bBtHsOperation)
		{
			if(pDM_Odm->bLinked)
			{
				if(pDM_DigTable->BT30_CurIGI > (CurrentIGI))
					ODM_Write_DIG(pDM_Odm, CurrentIGI);
				else
					ODM_Write_DIG(pDM_Odm, pDM_DigTable->BT30_CurIGI);
					
				pDM_DigTable->bMediaConnect_0 = pDM_Odm->bLinked;
				pDM_DigTable->DIG_Dynamic_MIN_0 = DIG_Dynamic_MIN;
			}
			else
			{
				if(pDM_Odm->bLinkInProcess)
					ODM_Write_DIG(pDM_Odm, 0x1c);
				else if(pDM_Odm->bBtConnectProcess)
					ODM_Write_DIG(pDM_Odm, 0x28);
				else
					ODM_Write_DIG(pDM_Odm, pDM_DigTable->BT30_CurIGI);//ODM_Write_DIG(pDM_Odm, pDM_DigTable->CurIGValue);	
			}
		}	
		else		// BT is not using
#endif
		{
			ODM_Write_DIG(pDM_Odm, CurrentIGI);//ODM_Write_DIG(pDM_Odm, pDM_DigTable->CurIGValue);
			pDM_DigTable->bMediaConnect_0 = pDM_Odm->bLinked;
			pDM_DigTable->DIG_Dynamic_MIN_0 = DIG_Dynamic_MIN;
		}
	}
}

VOID
odm_DIGbyRSSI_LPS(
	IN		PVOID		pDM_VOID
	)
{
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PFALSE_ALARM_STATISTICS		pFalseAlmCnt = &pDM_Odm->FalseAlmCnt;

#if (DM_ODM_SUPPORT_TYPE & ODM_CE)
#if 0		//and 2.3.5 coding rule
	struct mlme_priv	*pmlmepriv = &(pAdapter->mlmepriv);
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(pAdapter);	
	struct dm_priv	*pdmpriv = &pHalData->dmpriv;
#endif
#endif

	u1Byte	RSSI_Lower=DM_DIG_MIN_NIC;   //0x1E or 0x1C
	u1Byte	CurrentIGI=pDM_Odm->RSSI_Min;

	CurrentIGI=CurrentIGI+RSSI_OFFSET_DIG;

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIGbyRSSI_LPS()==>\n"));

	// Using FW PS mode to make IGI
	//Adjust by  FA in LPS MODE
	if(pFalseAlmCnt->Cnt_all> DM_DIG_FA_TH2_LPS)
		CurrentIGI = CurrentIGI+4;
	else if (pFalseAlmCnt->Cnt_all > DM_DIG_FA_TH1_LPS)
		CurrentIGI = CurrentIGI+2;
	else if(pFalseAlmCnt->Cnt_all < DM_DIG_FA_TH0_LPS)
		CurrentIGI = CurrentIGI-2;	


	//Lower bound checking

	//RSSI Lower bound check
	if((pDM_Odm->RSSI_Min-10) > DM_DIG_MIN_NIC)
		RSSI_Lower =(pDM_Odm->RSSI_Min-10);
	else
		RSSI_Lower =DM_DIG_MIN_NIC;

	//Upper and Lower Bound checking
	 if(CurrentIGI > DM_DIG_MAX_NIC)
	 	CurrentIGI=DM_DIG_MAX_NIC;
	 else if(CurrentIGI < RSSI_Lower)
		CurrentIGI =RSSI_Lower;

	
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIGbyRSSI_LPS(): pFalseAlmCnt->Cnt_all = %d\n",pFalseAlmCnt->Cnt_all));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIGbyRSSI_LPS(): pDM_Odm->RSSI_Min = %d\n",pDM_Odm->RSSI_Min));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIGbyRSSI_LPS(): CurrentIGI = 0x%x\n",CurrentIGI));

	ODM_Write_DIG(pDM_Odm, CurrentIGI);//ODM_Write_DIG(pDM_Odm, pDM_DigTable->CurIGValue);
#endif
}

VOID
odm_DigForBtHsMode(
	IN		PVOID		pDM_VOID
	)
{
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	PDM_ODM_T				pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T					pDM_DigTable=&pDM_Odm->DM_DigTable;
	u1Byte					digForBtHs=0;
	u1Byte					digUpBound=0x5a;
	
	if(pDM_Odm->bBtConnectProcess)
	{
		if(pDM_Odm->SupportICType&(ODM_RTL8723A))
			digForBtHs = 0x28;
		else
			digForBtHs = 0x22;
	}
	else
	{
		//
		// Decide DIG value by BT HS RSSI.
		//
		digForBtHs = pDM_Odm->btHsRssi+4;
		
		//DIG Bound
		if(pDM_Odm->SupportICType&(ODM_RTL8723A))
			digUpBound = 0x3e;
		
		if(digForBtHs > digUpBound)
			digForBtHs = digUpBound;
		if(digForBtHs < 0x1c)
			digForBtHs = 0x1c;

		// update Current IGI
		pDM_DigTable->BT30_CurIGI = digForBtHs;
	}
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DigForBtHsMode() : set DigValue=0x%x\n", digForBtHs));
#endif
}


//3============================================================
//3 FASLE ALARM CHECK
//3============================================================

VOID 
odm_FalseAlarmCounterStatistics(
	IN		PVOID		pDM_VOID
	)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PFALSE_ALARM_STATISTICS 	FalseAlmCnt = &(pDM_Odm->FalseAlmCnt);
	u4Byte 						ret_value;

#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
//Mark there, and check this in odm_DMWatchDog
#if 0 //(DM_ODM_SUPPORT_TYPE == ODM_AP)
	prtl8192cd_priv priv		= pDM_Odm->priv;
	if( (priv->auto_channel != 0) && (priv->auto_channel != 2) )
		return;
#endif
#endif

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	if((pDM_Odm->SupportICType == ODM_RTL8192D) &&
		(*(pDM_Odm->pMacPhyMode)==ODM_DMSP)&&    ////modify by Guo.Mingzhi 2011-12-29
		(!(*(pDM_Odm->pbMasterOfDMSP))))
	{
		odm_FalseAlarmCounterStatistics_ForSlaveOfDMSP(pDM_Odm);
		return;
	}
#endif		

	if(!(pDM_Odm->SupportAbility & ODM_BB_FA_CNT))
		return;

	if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
	{

		//hold ofdm counter
		ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_HOLDC_11N, BIT31, 1); //hold page C counter
		ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTD_11N, BIT31, 1); //hold page D counter
	
		ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_OFDM_FA_TYPE1_11N, bMaskDWord);
		FalseAlmCnt->Cnt_Fast_Fsync = (ret_value&0xffff);
		FalseAlmCnt->Cnt_SB_Search_fail = ((ret_value&0xffff0000)>>16);		

		ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_OFDM_FA_TYPE2_11N, bMaskDWord);
		FalseAlmCnt->Cnt_OFDM_CCA = (ret_value&0xffff); 
		FalseAlmCnt->Cnt_Parity_Fail = ((ret_value&0xffff0000)>>16);	

		ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_OFDM_FA_TYPE3_11N, bMaskDWord);
		FalseAlmCnt->Cnt_Rate_Illegal = (ret_value&0xffff);
		FalseAlmCnt->Cnt_Crc8_fail = ((ret_value&0xffff0000)>>16);

		ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_OFDM_FA_TYPE4_11N, bMaskDWord);
		FalseAlmCnt->Cnt_Mcs_fail = (ret_value&0xffff);

		FalseAlmCnt->Cnt_Ofdm_fail = 	FalseAlmCnt->Cnt_Parity_Fail + FalseAlmCnt->Cnt_Rate_Illegal +
								FalseAlmCnt->Cnt_Crc8_fail + FalseAlmCnt->Cnt_Mcs_fail +
								FalseAlmCnt->Cnt_Fast_Fsync + FalseAlmCnt->Cnt_SB_Search_fail;

#if (RTL8188E_SUPPORT==1)
		if(pDM_Odm->SupportICType == ODM_RTL8188E)
		{
			ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_SC_CNT_11N, bMaskDWord);
			FalseAlmCnt->Cnt_BW_LSC = (ret_value&0xffff);
			FalseAlmCnt->Cnt_BW_USC = ((ret_value&0xffff0000)>>16);
		}
#endif

#if (RTL8192D_SUPPORT==1) 
		if(pDM_Odm->SupportICType == ODM_RTL8192D)
		{
			odm_GetCCKFalseAlarm_92D(pDM_Odm);
		}
		else
#endif
		{
			//hold cck counter
			ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11N, BIT12, 1); 
			ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11N, BIT14, 1); 
		
			ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_CCK_FA_LSB_11N, bMaskByte0);
			FalseAlmCnt->Cnt_Cck_fail = ret_value;

			ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_CCK_FA_MSB_11N, bMaskByte3);
			FalseAlmCnt->Cnt_Cck_fail +=  (ret_value& 0xff)<<8;

			ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_CCK_CCA_CNT_11N, bMaskDWord);
			FalseAlmCnt->Cnt_CCK_CCA = ((ret_value&0xFF)<<8) |((ret_value&0xFF00)>>8);
		}
	
		FalseAlmCnt->Cnt_all = (	FalseAlmCnt->Cnt_Fast_Fsync + 
							FalseAlmCnt->Cnt_SB_Search_fail +
							FalseAlmCnt->Cnt_Parity_Fail +
							FalseAlmCnt->Cnt_Rate_Illegal +
							FalseAlmCnt->Cnt_Crc8_fail +
							FalseAlmCnt->Cnt_Mcs_fail +
							FalseAlmCnt->Cnt_Cck_fail);	

		FalseAlmCnt->Cnt_CCA_all = FalseAlmCnt->Cnt_OFDM_CCA + FalseAlmCnt->Cnt_CCK_CCA;

#if (RTL8192C_SUPPORT==1)
		if(pDM_Odm->SupportICType == ODM_RTL8192C)
			odm_ResetFACounter_92C(pDM_Odm);
#endif

#if (RTL8192D_SUPPORT==1)
		if(pDM_Odm->SupportICType == ODM_RTL8192D)
			odm_ResetFACounter_92D(pDM_Odm);
#endif

		if(pDM_Odm->SupportICType >=ODM_RTL8723A)
		{
			//reset false alarm counter registers
			ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTC_11N, BIT31, 1);
			ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTC_11N, BIT31, 0);
			ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTD_11N, BIT27, 1);
			ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTD_11N, BIT27, 0);

			//update ofdm counter
			ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_HOLDC_11N, BIT31, 0); //update page C counter
			ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RSTD_11N, BIT31, 0); //update page D counter

			//reset CCK CCA counter
			ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11N, BIT13|BIT12, 0); 
			ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11N, BIT13|BIT12, 2); 
			//reset CCK FA counter
			ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11N, BIT15|BIT14, 0); 
			ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11N, BIT15|BIT14, 2); 
		}
		
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Enter odm_FalseAlarmCounterStatistics\n"));
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Fast_Fsync=%d, Cnt_SB_Search_fail=%d\n",
			FalseAlmCnt->Cnt_Fast_Fsync, FalseAlmCnt->Cnt_SB_Search_fail));
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Parity_Fail=%d, Cnt_Rate_Illegal=%d\n",
			FalseAlmCnt->Cnt_Parity_Fail, FalseAlmCnt->Cnt_Rate_Illegal));
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Crc8_fail=%d, Cnt_Mcs_fail=%d\n",
		FalseAlmCnt->Cnt_Crc8_fail, FalseAlmCnt->Cnt_Mcs_fail));
	}
	else if(pDM_Odm->SupportICType & ODM_IC_11AC_SERIES)
	{
		u4Byte CCKenable;
		u4Byte Cnt_Ofdm_fail_temp = 0;
		
		//read OFDM FA counter
		FalseAlmCnt->Cnt_Ofdm_fail = ODM_GetBBReg(pDM_Odm, ODM_REG_OFDM_FA_11AC, bMaskLWord);
		FalseAlmCnt->Cnt_Cck_fail = ODM_GetBBReg(pDM_Odm, ODM_REG_CCK_FA_11AC, bMaskLWord);

		//read CCK/OFDM CCA counter
		ret_value = ODM_GetBBReg(pDM_Odm, ODM_REG_CCK_CCA_CNT_11AC, bMaskDWord);
		FalseAlmCnt->Cnt_OFDM_CCA = (ret_value & 0xffff0000) >> 16;
		FalseAlmCnt->Cnt_CCK_CCA = ret_value & 0xffff;

		// For 8881A
		if(pDM_Odm->SupportICType == ODM_RTL8881A)
		{
			if(FalseAlmCnt->Cnt_Ofdm_fail >= FalseAlmCnt->Cnt_Ofdm_fail_pre)
			{
				Cnt_Ofdm_fail_temp = FalseAlmCnt->Cnt_Ofdm_fail_pre;
				FalseAlmCnt->Cnt_Ofdm_fail_pre = FalseAlmCnt->Cnt_Ofdm_fail;
				FalseAlmCnt->Cnt_Ofdm_fail = FalseAlmCnt->Cnt_Ofdm_fail - Cnt_Ofdm_fail_temp;
			}
			else
				FalseAlmCnt->Cnt_Ofdm_fail_pre = FalseAlmCnt->Cnt_Ofdm_fail;
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Ofdm_fail=%d\n",	FalseAlmCnt->Cnt_Ofdm_fail_pre));
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Ofdm_fail_pre=%d\n",	Cnt_Ofdm_fail_temp));
			
			// Reset FA counter by enable/disable OFDM
			if(FalseAlmCnt->Cnt_Ofdm_fail_pre >= 0x7fff)
			{
				// reset OFDM
				ODM_SetBBReg(pDM_Odm, ODM_REG_BB_RX_PATH_11AC, BIT29,0);
				ODM_SetBBReg(pDM_Odm, ODM_REG_BB_RX_PATH_11AC, BIT29,1);
				FalseAlmCnt->Cnt_Ofdm_fail_pre = 0;
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Reset false alarm counter\n"));
			}
		}

		// reset OFDM FA coutner
		ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RST_11AC, BIT17, 1);
		ODM_SetBBReg(pDM_Odm, ODM_REG_OFDM_FA_RST_11AC, BIT17, 0);

		// reset CCK FA counter
		ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11AC, BIT15, 0);
		ODM_SetBBReg(pDM_Odm, ODM_REG_CCK_FA_RST_11AC, BIT15, 1);

		// reset CCA counter
		ODM_SetBBReg(pDM_Odm, ODM_REG_RST_RPT_11AC, BIT0, 1);
		ODM_SetBBReg(pDM_Odm, ODM_REG_RST_RPT_11AC, BIT0, 0);

		CCKenable =  ODM_GetBBReg(pDM_Odm, ODM_REG_BB_RX_PATH_11AC, BIT28);
		if(CCKenable)//if(*pDM_Odm->pBandType == ODM_BAND_2_4G)
		{
			FalseAlmCnt->Cnt_all = FalseAlmCnt->Cnt_Ofdm_fail + FalseAlmCnt->Cnt_Cck_fail;
			FalseAlmCnt->Cnt_CCA_all = FalseAlmCnt->Cnt_CCK_CCA + FalseAlmCnt->Cnt_OFDM_CCA;
		}
		else
		{
			FalseAlmCnt->Cnt_all = FalseAlmCnt->Cnt_Ofdm_fail;
			FalseAlmCnt->Cnt_CCA_all = FalseAlmCnt->Cnt_OFDM_CCA;
		}

	}

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_OFDM_CCA=%d\n",	FalseAlmCnt->Cnt_OFDM_CCA));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_CCK_CCA=%d\n",	FalseAlmCnt->Cnt_CCK_CCA));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_CCA_all=%d\n",	FalseAlmCnt->Cnt_CCA_all));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Ofdm_fail=%d\n",	FalseAlmCnt->Cnt_Ofdm_fail));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Cck_fail=%d\n",	FalseAlmCnt->Cnt_Cck_fail));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Cnt_Ofdm_fail=%d\n",	FalseAlmCnt->Cnt_Ofdm_fail));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_FA_CNT, ODM_DBG_LOUD, ("Total False Alarm=%d\n",	FalseAlmCnt->Cnt_all));
}


VOID
odm_FAThresholdCheck(
	IN		PVOID			pDM_VOID,
	IN		BOOLEAN			bDFSBand,
	IN		BOOLEAN			bPerformance,
	IN		u4Byte			RxTp,
	IN		u4Byte			TxTp,
	OUT		u4Byte*			dm_FA_thres
	)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	
	if(pDM_Odm->bLinked && (bPerformance||bDFSBand))
	{
		if(pDM_Odm->SupportICType == ODM_RTL8192D)
		{
			// 8192D special case
			dm_FA_thres[0] = DM_DIG_FA_TH0_92D;
			dm_FA_thres[1] = DM_DIG_FA_TH1_92D;
			dm_FA_thres[2] = DM_DIG_FA_TH2_92D;
		}
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
		else if(pDM_Odm->SupportPlatform & (ODM_AP|ODM_ADSL))
		{
			// For AP
			if((RxTp>>2) > TxTp && RxTp < 10000 && RxTp > 500)			// 10Mbps & 0.5Mbps
			{
				dm_FA_thres[0] = 0x080;
				dm_FA_thres[1] = 0x100;
				dm_FA_thres[2] = 0x200;			
			}
			else
			{
				dm_FA_thres[0] = 0x100;
				dm_FA_thres[1] = 0x200;
				dm_FA_thres[2] = 0x300;	
			}
		}
#else
		else if(pDM_Odm->SupportICType == ODM_RTL8723A && pDM_Odm->bBtLimitedDig)
		{
			// 8723A BT special case
			dm_FA_thres[0] = DM_DIG_FA_TH0;
			dm_FA_thres[1] = 0x250;
			dm_FA_thres[2] = 0x300;
		}
#endif
		else
		{
			// For NIC
			dm_FA_thres[0] = DM_DIG_FA_TH0;
			dm_FA_thres[1] = DM_DIG_FA_TH1;
			dm_FA_thres[2] = DM_DIG_FA_TH2;
		}
	}
	else
	{
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
		if(bDFSBand)
		{
			// For DFS band and no link
			dm_FA_thres[0] = 250;
			dm_FA_thres[1] = 1000;
			dm_FA_thres[2] = 2000;
		}
		else
#endif
		{
			dm_FA_thres[0] = 2000;
			dm_FA_thres[1] = 4000;
			dm_FA_thres[2] = 5000;
		}
	}
	return;
}

u1Byte
odm_ForbiddenIGICheck(
	IN		PVOID			pDM_VOID,
	IN		u1Byte			DIG_Dynamic_MIN,
	IN		u1Byte			CurrentIGI
	)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T						pDM_DigTable = &pDM_Odm->DM_DigTable;
	PFALSE_ALARM_STATISTICS 	pFalseAlmCnt = &(pDM_Odm->FalseAlmCnt);
	u1Byte						rx_gain_range_min = pDM_DigTable->rx_gain_range_min;

	if(pFalseAlmCnt->Cnt_all > 10000)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Abnormally false alarm case. \n"));

		if(pDM_DigTable->LargeFAHit != 3)
			pDM_DigTable->LargeFAHit++;
		
		if(pDM_DigTable->ForbiddenIGI < CurrentIGI)//if(pDM_DigTable->ForbiddenIGI < pDM_DigTable->CurIGValue)
		{
			pDM_DigTable->ForbiddenIGI = CurrentIGI;//pDM_DigTable->ForbiddenIGI = pDM_DigTable->CurIGValue;
			pDM_DigTable->LargeFAHit = 1;
		}

		if(pDM_DigTable->LargeFAHit >= 3)
		{
			if((pDM_DigTable->ForbiddenIGI + 2) > pDM_DigTable->rx_gain_range_max)
				rx_gain_range_min = pDM_DigTable->rx_gain_range_max;
			else
				rx_gain_range_min = (pDM_DigTable->ForbiddenIGI + 2);
			pDM_DigTable->Recover_cnt = 1800;
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Abnormally false alarm case: Recover_cnt = %d \n", pDM_DigTable->Recover_cnt));
		}
	}
	else
	{
		if(pDM_DigTable->Recover_cnt != 0)
		{
			pDM_DigTable->Recover_cnt --;
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Normal Case: Recover_cnt = %d \n", pDM_DigTable->Recover_cnt));
		}
		else
		{
			if(pDM_DigTable->LargeFAHit < 3)
			{
				if((pDM_DigTable->ForbiddenIGI - 2) < DIG_Dynamic_MIN) //DM_DIG_MIN)
				{
					pDM_DigTable->ForbiddenIGI = DIG_Dynamic_MIN; //DM_DIG_MIN;
					rx_gain_range_min = DIG_Dynamic_MIN; //DM_DIG_MIN;
					ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Normal Case: At Lower Bound\n"));
				}
				else
				{
					pDM_DigTable->ForbiddenIGI -= 2;
					rx_gain_range_min = (pDM_DigTable->ForbiddenIGI + 2);
					ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_DIG(): Normal Case: Approach Lower Bound\n"));
				}
			}
			else
			{
				pDM_DigTable->LargeFAHit = 0;
			}
		}
	}
	
	return rx_gain_range_min;

}

VOID
odm_InbandNoiseCalculate (	
	IN		PVOID		pDM_VOID
	)
{
#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
	PDM_ODM_T			pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T				pDM_DigTable = &pDM_Odm->DM_DigTable;
	u1Byte				IGIBackup, TimeCnt = 0, ValidCnt = 0;
	BOOLEAN				bTimeout = TRUE;
	s1Byte				sNoise_A, sNoise_B;
	s4Byte				NoiseRpt_A = 0,NoiseRpt_B = 0;
	u4Byte				tmp = 0;
	static	u1Byte		failCnt = 0;

	if(!(pDM_Odm->SupportICType & (ODM_RTL8192E)))
		return;

	if(pDM_Odm->RFType == ODM_1T1R || *(pDM_Odm->pOnePathCCA) != ODM_CCA_2R)
		return;

	if(!pDM_DigTable->bNoiseEst)
		return;

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("odm_InbandNoiseEstimate()========>\n"));
	
	//1 Set initial gain.
	IGIBackup = pDM_DigTable->CurIGValue;
	pDM_DigTable->IGIOffset_A = 0;
	pDM_DigTable->IGIOffset_B = 0;
	ODM_Write_DIG(pDM_Odm, 0x24);

	//1 Update idle time power report	
	if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
		ODM_SetBBReg(pDM_Odm, ODM_REG_TX_ANT_CTRL_11N, BIT25, 0x0);

	delay_ms(2);

	//1 Get noise power level
	while(1)
	{
		//2 Read Noise Floor Report
		if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
			tmp = ODM_GetBBReg(pDM_Odm, 0x8f8, bMaskLWord);

		sNoise_A = (s1Byte)(tmp & 0xff);
		sNoise_B = (s1Byte)((tmp & 0xff00)>>8);

		//ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("sNoise_A = %d, sNoise_B = %d\n",sNoise_A, sNoise_B));

		if((sNoise_A < 20 && sNoise_A >= -70) && (sNoise_B < 20 && sNoise_B >= -70))
		{
			ValidCnt++;
			NoiseRpt_A += sNoise_A;
			NoiseRpt_B += sNoise_B;
			//ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("sNoise_A = %d, sNoise_B = %d\n",sNoise_A, sNoise_B));
		}

		TimeCnt++;
		bTimeout = (TimeCnt >= 150)?TRUE:FALSE;
		
		if(ValidCnt == 20 || bTimeout)
			break;

		delay_ms(2);
		
	}

	//1 Keep idle time power report	
	if(pDM_Odm->SupportICType & ODM_IC_11N_SERIES)
		ODM_SetBBReg(pDM_Odm, ODM_REG_TX_ANT_CTRL_11N, BIT25, 0x1);

	//1 Recover IGI
	ODM_Write_DIG(pDM_Odm, IGIBackup);
	
	//1 Calculate Noise Floor
	if(ValidCnt != 0)
	{
		NoiseRpt_A  /= (ValidCnt<<1);
		NoiseRpt_B  /= (ValidCnt<<1);
	}
	
	if(bTimeout)
	{
		NoiseRpt_A = 0;
		NoiseRpt_B = 0;

		failCnt ++;
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("Noise estimate fail time = %d\n", failCnt));
		
		if(failCnt == 3)
		{
			failCnt = 0;
			pDM_DigTable->bNoiseEst = FALSE;
		}
	}
	else
	{
		NoiseRpt_A = -110 + 0x24 + NoiseRpt_A -6;
		NoiseRpt_B = -110 + 0x24 + NoiseRpt_B -6;
		pDM_DigTable->bNoiseEst = FALSE;
		failCnt = 0;
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("NoiseRpt_A = %d, NoiseRpt_B = %d\n", NoiseRpt_A, NoiseRpt_B));
	}

	//1 Calculate IGI Offset
	if(NoiseRpt_A > NoiseRpt_B)
	{
		pDM_DigTable->IGIOffset_A = NoiseRpt_A - NoiseRpt_B;
		pDM_DigTable->IGIOffset_B = 0;
	}
	else
	{
		pDM_DigTable->IGIOffset_A = 0;
		pDM_DigTable->IGIOffset_B = NoiseRpt_B - NoiseRpt_A;
	}

#endif
	return;
}

//3============================================================
//3 CCK Packet Detect Threshold
//3============================================================

VOID
odm_PauseCCKPacketDetection(
	IN		PVOID					pDM_VOID,
	IN		ODM_Pause_CCKPD_TYPE	PauseType,
	IN		u1Byte					CCKPDThreshold
)
{
	PDM_ODM_T			pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T				pDM_DigTable = &pDM_Odm->DM_DigTable;
	static	BOOLEAN		bPaused = FALSE;

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("odm_PauseCCKPacketDetection()=========>\n"));

#if (DM_ODM_SUPPORT_TYPE & ODM_WIN)	
	if(*pDM_DigTable->pbP2pLinkInProgress)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("P2P in progress !!\n"));
		return;
	}
#endif

	if(!bPaused && (!(pDM_Odm->SupportAbility & ODM_BB_CCK_PD) || !(pDM_Odm->SupportAbility & ODM_BB_FA_CNT)))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Return: SupportAbility ODM_BB_CCK_PD or ODM_BB_FA_CNT is disabled\n"));
		return;
	}

	switch(PauseType)
	{
		//1 Pause CCK Packet Detection Threshold
		case ODM_PAUSE_CCKPD:
			//2 Disable DIG
			ODM_CmnInfoUpdate(pDM_Odm, ODM_CMNINFO_ABILITY, pDM_Odm->SupportAbility & (~ODM_BB_CCK_PD));
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Pause CCK packet detection threshold !!\n"));

			//2 Backup CCK Packet Detection Threshold value
			if(!bPaused)
			{
				pDM_DigTable->CCKPDBackup = pDM_DigTable->CurCCK_CCAThres;
				bPaused = TRUE;
			}
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Backup CCK packet detection tgreshold  = %d\n", pDM_DigTable->CCKPDBackup));

			//2 Write new CCK Packet Detection Threshold value
			ODM_Write_CCK_CCA_Thres(pDM_Odm, CCKPDThreshold);
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Write new CCK packet detection tgreshold = %d\n", CCKPDThreshold));
			break;
			
		//1 Resume CCK Packet Detection Threshold
		case ODM_RESUME_CCKPD:
			if(bPaused)
			{
				//2 Write backup CCK Packet Detection Threshold value
				ODM_Write_CCK_CCA_Thres(pDM_Odm, pDM_DigTable->CCKPDBackup);
				bPaused = FALSE;
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Write original CCK packet detection tgreshold = %d\n", pDM_DigTable->CCKPDBackup));

				//2 Enable CCK Packet Detection Threshold
				ODM_CmnInfoUpdate(pDM_Odm, ODM_CMNINFO_ABILITY, pDM_Odm->SupportAbility | ODM_BB_CCK_PD);		
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Resume CCK packet detection threshold  !!\n"));
			}
			break;
			
		default:
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("Wrong  type !!\n"));
			break;
	}	
	return;
}


VOID 
odm_CCKPacketDetectionThresh(
	IN		PVOID		pDM_VOID
	)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PFALSE_ALARM_STATISTICS 	FalseAlmCnt = &(pDM_Odm->FalseAlmCnt);
	u1Byte						CurCCK_CCAThres;


#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
//modify by Guo.Mingzhi 2011-12-29
	if (pDM_Odm->bDualMacSmartConcurrent == TRUE)
//	if (pDM_Odm->bDualMacSmartConcurrent == FALSE)
		return;
	if(pDM_Odm->bBtHsOperation)
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("odm_CCKPacketDetectionThresh() write 0xcd for BT HS mode!!\n"));
		ODM_Write_CCK_CCA_Thres(pDM_Odm, 0xcd);
		return;
	}
#endif

	if((!(pDM_Odm->SupportAbility & ODM_BB_CCK_PD)) ||(!(pDM_Odm->SupportAbility & ODM_BB_FA_CNT)))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("odm_CCKPacketDetectionThresh()  return==========\n"));
		return;
	}

#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
	if(pDM_Odm->ExtLNA)
		return;
#endif

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("odm_CCKPacketDetectionThresh()  ==========>\n"));

	if(pDM_Odm->bLinked)
	{
		if(pDM_Odm->RSSI_Min > 25)
			CurCCK_CCAThres = 0xcd;
		else if((pDM_Odm->RSSI_Min <= 25) && (pDM_Odm->RSSI_Min > 10))
			CurCCK_CCAThres = 0x83;
		else
		{
			if(FalseAlmCnt->Cnt_Cck_fail > 1000)
				CurCCK_CCAThres = 0x83;
			else
				CurCCK_CCAThres = 0x40;
		}
	}
	else
	{
		if(FalseAlmCnt->Cnt_Cck_fail > 1000)
			CurCCK_CCAThres = 0x83;
		else
			CurCCK_CCAThres = 0x40;
	}
	
#if (RTL8192D_SUPPORT==1) 
	if((pDM_Odm->SupportICType == ODM_RTL8192D) && (*pDM_Odm->pBandType == ODM_BAND_2_4G))
		ODM_Write_CCK_CCA_Thres_92D(pDM_Odm, CurCCK_CCAThres);
	else
#endif
		ODM_Write_CCK_CCA_Thres(pDM_Odm, CurCCK_CCAThres);

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_CCK_PD, ODM_DBG_LOUD, ("odm_CCKPacketDetectionThresh()  CurCCK_CCAThres = 0x%x\n",CurCCK_CCAThres));
}

VOID
ODM_Write_CCK_CCA_Thres(
	IN	PVOID			pDM_VOID,
	IN	u1Byte			CurCCK_CCAThres
	)
{
	PDM_ODM_T			pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T				pDM_DigTable = &pDM_Odm->DM_DigTable;

	if(pDM_DigTable->CurCCK_CCAThres!=CurCCK_CCAThres)		//modify by Guo.Mingzhi 2012-01-03
	{
		ODM_Write1Byte(pDM_Odm, ODM_REG(CCK_CCA,pDM_Odm), CurCCK_CCAThres);
	}
	pDM_DigTable->PreCCK_CCAThres = pDM_DigTable->CurCCK_CCAThres;
	pDM_DigTable->CurCCK_CCAThres = CurCCK_CCAThres;
}

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)

VOID
odm_EnableEDCCA(
	IN		PVOID					pDM_VOID
)
{

	// This should be moved out of OUTSRC
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PADAPTER		pAdapter	= pDM_Odm->Adapter;
	// Enable EDCCA. The value is suggested by SD3 Wilson.

	//
	// Revised for ASUS 11b/g performance issues, suggested by BB Neil, 2012.04.13.
	//
	if((pDM_Odm->SupportICType == ODM_RTL8723A)&&(IS_WIRELESS_MODE_G(pAdapter)))
	{
		//PlatformEFIOWrite1Byte(Adapter, rOFDM0_ECCAThreshold, 0x00);
		ODM_Write1Byte(pDM_Odm,rOFDM0_ECCAThreshold,0x00);
		ODM_Write1Byte(pDM_Odm,rOFDM0_ECCAThreshold+2,0xFD);
		
	}	
	else
	{
		//PlatformEFIOWrite1Byte(Adapter, rOFDM0_ECCAThreshold, 0x03);
		ODM_Write1Byte(pDM_Odm,rOFDM0_ECCAThreshold,0x03);
		ODM_Write1Byte(pDM_Odm,rOFDM0_ECCAThreshold+2,0x00);
	}	
	
	//PlatformEFIOWrite1Byte(Adapter, rOFDM0_ECCAThreshold+2, 0x00);
}

VOID
odm_DisableEDCCA(
	IN		PVOID					pDM_VOID
)
{	
	// Disable EDCCA..
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	ODM_Write1Byte(pDM_Odm, rOFDM0_ECCAThreshold, 0x7f);
	ODM_Write1Byte(pDM_Odm, rOFDM0_ECCAThreshold+2, 0x7f);
}

//
// Description: According to initial gain value to determine to enable or disable EDCCA.
//
// Suggested by SD3 Wilson. Added by tynli. 2011.11.25.
//
VOID
odm_DynamicEDCCA(
	IN		PVOID					pDM_VOID
)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	PADAPTER		pAdapter	= pDM_Odm->Adapter;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(pAdapter);
	u1Byte			RegC50, RegC58;
	BOOLEAN			bEDCCAenable = FALSE;
	
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN))	
	BOOLEAN			bFwCurrentInPSMode=FALSE;	

	pAdapter->HalFunc.GetHwRegHandler(pAdapter, HW_VAR_FW_PSMODE_STATUS, (pu1Byte)(&bFwCurrentInPSMode));	

	// Disable EDCCA mode while under LPS mode, added by Roger, 2012.09.14.
	if(bFwCurrentInPSMode)
		return;
#endif
	//
	// 2013/11/14 Ken According to BB team Jame's suggestion, we need to disable soft AP mode EDCCA.
	// 2014/01/08 MH For Miracst AP mode test. We need to disable EDCCA. Otherwise, we may stop
	// to send beacon in noisy environment or platform.
	//
	if(ACTING_AS_AP(pAdapter) || ACTING_AS_AP(GetFirstAPAdapter(pAdapter)))
	//if(ACTING_AS_AP(pAdapter))
	{
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("At least One Port as AP disable EDCCA\n"));
		odm_DisableEDCCA(pDM_Odm);
		if(pHalData->bPreEdccaEnable)
			odm_DisableEDCCA(pDM_Odm);
		pHalData->bPreEdccaEnable = FALSE;
		return;
	}
	
	RegC50 = (u1Byte)ODM_GetBBReg(pDM_Odm, rOFDM0_XAAGCCore1, bMaskByte0);
	RegC58 = (u1Byte)ODM_GetBBReg(pDM_Odm, rOFDM0_XBAGCCore1, bMaskByte0);


 	if((RegC50 > 0x28 && RegC58 > 0x28) ||
  		((pDM_Odm->SupportICType == ODM_RTL8723A && IS_WIRELESS_MODE_G(pAdapter) && RegC50>0x26)) ||
  		(pDM_Odm->SupportICType == ODM_RTL8188E && RegC50 > 0x28))
	{
		if(!pHalData->bPreEdccaEnable)
		{
			odm_EnableEDCCA(pDM_Odm);
			pHalData->bPreEdccaEnable = TRUE;
		}
		
	}
	else if((RegC50 < 0x25 && RegC58 < 0x25) || (pDM_Odm->SupportICType == ODM_RTL8188E && RegC50 < 0x25))
	{
		if(pHalData->bPreEdccaEnable)
		{
			odm_DisableEDCCA(pDM_Odm);
			pHalData->bPreEdccaEnable = FALSE;
		}
	}
}

// <20130108, Kordan> E.g., With LNA used, we make the Rx power smaller to have a better EVM. (Asked by Willis)
VOID
odm_RFEControl(
	IN	PDM_ODM_T	pDM_Odm,
	IN  u8Byte		RSSIVal
	)
{
	PADAPTER		Adapter = (PADAPTER)pDM_Odm->Adapter;
    HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	static u1Byte 	TRSW_HighPwr = 0;
	 
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("===> odm_RFEControl, RSSI = %d, TRSW_HighPwr = 0x%X, pHalData->RFEType = %d\n",
		         RSSIVal, TRSW_HighPwr, pHalData->RFEType ));

    if (pHalData->RFEType == 3) {	   
		
        pDM_Odm->RSSI_TRSW = RSSIVal;

        if (pDM_Odm->RSSI_TRSW >= pDM_Odm->RSSI_TRSW_H) 
		{				 
            TRSW_HighPwr = 1; // Switch to
            PHY_SetBBReg(Adapter, r_ANTSEL_SW_Jaguar, BIT1|BIT0, 0x1);  // Set ANTSW=1/ANTSWB=0  for SW control
            PHY_SetBBReg(Adapter, r_ANTSEL_SW_Jaguar, BIT9|BIT8, 0x3);  // Set ANTSW=1/ANTSWB=0  for SW control
            
        } 
		else if (pDM_Odm->RSSI_TRSW <= pDM_Odm->RSSI_TRSW_L) 
        {	  
            TRSW_HighPwr = 0; // Switched back
            PHY_SetBBReg(Adapter, r_ANTSEL_SW_Jaguar, BIT1|BIT0, 0x1);  // Set ANTSW=1/ANTSWB=0  for SW control
            PHY_SetBBReg(Adapter, r_ANTSEL_SW_Jaguar, BIT9|BIT8, 0x0);  // Set ANTSW=1/ANTSWB=0  for SW control

        }
    }  

	
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("(pDM_Odm->RSSI_TRSW_H, pDM_Odm->RSSI_TRSW_L) = (%d, %d)\n", pDM_Odm->RSSI_TRSW_H, pDM_Odm->RSSI_TRSW_L));		
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("(RSSIVal, RSSIVal, pDM_Odm->RSSI_TRSW_iso) = (%d, %d, %d)\n", 
				 RSSIVal, pDM_Odm->RSSI_TRSW_iso, pDM_Odm->RSSI_TRSW));
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_LOUD, ("<=== odm_RFEControl, RSSI = %d, TRSW_HighPwr = 0x%X\n", RSSIVal, TRSW_HighPwr));	
}

VOID
odm_MPT_DIGWorkItemCallback(
    IN PVOID            pContext
    )
{
	PADAPTER	Adapter = (PADAPTER)pContext;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	PDM_ODM_T		pDM_Odm = &pHalData->DM_OutSrc;

	ODM_MPT_DIG(pDM_Odm);
}

VOID
odm_MPT_DIGCallback(
	PRT_TIMER		pTimer
)
{
	PADAPTER		Adapter = (PADAPTER)pTimer->Adapter;
       HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	  PDM_ODM_T		pDM_Odm = &pHalData->DM_OutSrc;


	#if DEV_BUS_TYPE==RT_PCI_INTERFACE
		#if USE_WORKITEM
			PlatformScheduleWorkItem(&pDM_Odm->MPT_DIGWorkitem);
		#else
			ODM_MPT_DIG(pDM_Odm);
		#endif
	#else
		PlatformScheduleWorkItem(&pDM_Odm->MPT_DIGWorkitem);
	#endif

}

#endif

#if (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL))
VOID
odm_MPT_DIGCallback(
	IN		PVOID					pDM_VOID
)
{
	PDM_ODM_T			pDM_Odm = (PDM_ODM_T)pDM_VOID;
#if USE_WORKITEM
	PlatformScheduleWorkItem(&pDM_Odm->MPT_DIGWorkitem);
#else
	ODM_MPT_DIG(pDM_Odm);
#endif
}
#endif

#if (DM_ODM_SUPPORT_TYPE != ODM_CE)
VOID
odm_MPT_Write_DIG(
	IN		PVOID					pDM_VOID,
	IN		u1Byte					CurIGValue
)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T						pDM_DigTable = &pDM_Odm->DM_DigTable;

	ODM_Write1Byte( pDM_Odm, ODM_REG(IGI_A,pDM_Odm), CurIGValue);

	if(pDM_Odm->RFType > ODM_1T1R)
		ODM_Write1Byte( pDM_Odm, ODM_REG(IGI_B,pDM_Odm), CurIGValue);

	if((pDM_Odm->SupportICType & ODM_IC_11AC_SERIES) && (pDM_Odm->RFType > ODM_2T2R))
	{
		ODM_Write1Byte( pDM_Odm, ODM_REG(IGI_C,pDM_Odm), CurIGValue);
		ODM_Write1Byte( pDM_Odm, ODM_REG(IGI_D,pDM_Odm), CurIGValue);	
	}

	pDM_DigTable->CurIGValue = CurIGValue;
	
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("CurIGValue = 0x%x\n", CurIGValue));
	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("pDM_Odm->RFType = 0x%x\n", pDM_Odm->RFType));
}

VOID
ODM_MPT_DIG(
	IN		PVOID					pDM_VOID
	)
{
	PDM_ODM_T					pDM_Odm = (PDM_ODM_T)pDM_VOID;
	pDIG_T						pDM_DigTable = &pDM_Odm->DM_DigTable;
	PFALSE_ALARM_STATISTICS		pFalseAlmCnt = &pDM_Odm->FalseAlmCnt;
	u1Byte						CurrentIGI = pDM_DigTable->CurIGValue;
	u1Byte						DIG_Upper = 0x40, DIG_Lower = 0x20;
	u4Byte						RXOK_cal;
	u4Byte						RxPWDBAve_final;
	u1Byte						IGI_A = 0x20, IGI_B = 0x20;
	
#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)

	#if ODM_FIX_2G_DIG
	IGI_A = 0x22;
	IGI_B = 0x24;		
	#endif
	
#else
	if (!(pDM_Odm->priv->pshare->rf_ft_var.mp_specific && pDM_Odm->priv->pshare->mp_dig_on))
		return;

	if (*pDM_Odm->pBandType == ODM_BAND_5G)
		DIG_Lower = 0x22;
#endif

	ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("===> ODM_MPT_DIG, pBandType = %d\n", *pDM_Odm->pBandType));
	
#if (ODM_FIX_2G_DIG | (DM_ODM_SUPPORT_TYPE & (ODM_AP|ODM_ADSL)))
	if (*pDM_Odm->pBandType == ODM_BAND_5G)	 // for 5G
#else
	if (1) // for both 2G/5G
#endif
		{
		odm_FalseAlarmCounterStatistics(pDM_Odm);

		RXOK_cal = pDM_Odm->PhyDbgInfo.NumQryPhyStatusCCK + pDM_Odm->PhyDbgInfo.NumQryPhyStatusOFDM;
		RxPWDBAve_final = (RXOK_cal != 0)?pDM_Odm->RxPWDBAve/RXOK_cal:0;

		pDM_Odm->PhyDbgInfo.NumQryPhyStatusCCK = 0;
		pDM_Odm->PhyDbgInfo.NumQryPhyStatusOFDM = 0;
		pDM_Odm->RxPWDBAve = 0;
		pDM_Odm->MPDIG_2G = FALSE;

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
		pDM_Odm->Times_2G = 0;
#endif

		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("RX OK = %d\n", RXOK_cal));
		ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("RSSI = %d\n", RxPWDBAve_final));
	
		if (RXOK_cal >= 70 && RxPWDBAve_final <= 40)
		{
			if (CurrentIGI > 0x24)
				odm_MPT_Write_DIG(pDM_Odm, 0x24);
		}
		else
		{
			if(pFalseAlmCnt->Cnt_all > 1000){
				CurrentIGI = CurrentIGI + 8;
			}
			else if(pFalseAlmCnt->Cnt_all > 200){
				CurrentIGI = CurrentIGI + 4;
			}
			else if (pFalseAlmCnt->Cnt_all > 50){
				CurrentIGI = CurrentIGI + 2;
			}
			else if (pFalseAlmCnt->Cnt_all < 2){
				CurrentIGI = CurrentIGI - 2;
			}
			
			if (CurrentIGI < DIG_Lower ){
				CurrentIGI = DIG_Lower;
			}

			if(CurrentIGI > DIG_Upper){
				CurrentIGI = DIG_Upper;
			}

			odm_MPT_Write_DIG(pDM_Odm, CurrentIGI);
			ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("DIG = 0x%x, Cnt_all = %d, Cnt_Ofdm_fail = %d, Cnt_Cck_fail = %d\n", 
				CurrentIGI, pFalseAlmCnt->Cnt_all, pFalseAlmCnt->Cnt_Ofdm_fail, pFalseAlmCnt->Cnt_Cck_fail));
		}
	}
	else
	{
		if(pDM_Odm->MPDIG_2G == FALSE)
		{
			if((pDM_Odm->SupportPlatform & ODM_WIN) && !(pDM_Odm->SupportICType & ODM_RTL8814A))
			{
				ODM_RT_TRACE(pDM_Odm,ODM_COMP_DIG, ODM_DBG_LOUD, ("===> Fix IGI\n"));
				ODM_Write1Byte( pDM_Odm, ODM_REG(IGI_A,pDM_Odm), IGI_A);
				ODM_Write1Byte( pDM_Odm, ODM_REG(IGI_B,pDM_Odm), IGI_B);
				pDM_DigTable->CurIGValue = IGI_B;
			}
			else
				odm_MPT_Write_DIG(pDM_Odm, IGI_A);
		}

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
		pDM_Odm->Times_2G++;

		if (pDM_Odm->Times_2G == 3)
#endif
		{
			pDM_Odm->MPDIG_2G = TRUE;
		}
	}

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
	if (pDM_Odm->SupportICType == ODM_RTL8812)
		odm_RFEControl(pDM_Odm, RxPWDBAve_final);
#endif

	ODM_SetTimer(pDM_Odm, &pDM_Odm->MPT_DIGTimer, 700);
}
#endif
