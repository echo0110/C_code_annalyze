/*******************************************************************************
 * File Name   : oam_xml_enum.h
 *
 * Description : This is a header file which contains xml functions
 *
 *
 * Revision History:
 *
 * Date         	Author       	  Reference            Comments
 * -----        	------       	  ---------            --------
 * September,2012   PURSHOTTAM GUPTA  OAM Design Document  Initial Version
 * April, 2014      Dinesh Dua        CSR 00070293 Fix
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ******************************************************************************/

/*******************************************************************************
 Private Types
 ******************************************************************************/

#ifndef _xml_enum_header_h_
#define _xml_enum_header_h_

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <rrm_oam_defines.h>

/****************************************************************************
 **  MACROS
 ****************************************************************************/
#define MAX_STRING_LENGTH_TR196 128


/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
/* structure for String to Integer mapping */
typedef struct string_mapping_tbl_t
{
    char display_string[MAX_STRING_LENGTH_TR196];			//element for TR.196i2 specified char string.
	int internal_value;										//element for Layer specific integer value.
}enumerated_string_fields_mapping_tbl_t;

/* SPR 16407 START */
extern int CELL_BARRED[OAM_TWO][OAM_TWO];
/* SPR 16407 END */


/*-----------------------------------------------Integer To Integer-----------------------------------------------*/
extern enumerated_string_fields_mapping_tbl_t RRM_GERANFREQ_BANDINDICATOR[OAM_TWO];
/*SPR:9345_start*/
extern enumerated_string_fields_mapping_tbl_t SON_GSM_BANDINDICATOR[OAM_TWO];
/*SPR:9345_end*/

/* CSR 00070293 FIX START */
extern enumerated_string_fields_mapping_tbl_t RRM_GSM_BANDINDICATOR[OAM_TWO];
/* CSR 00070293 FIX END */
extern  enumerated_string_fields_mapping_tbl_t hoppingMode[2];
extern enumerated_string_fields_mapping_tbl_t DLBandwidth[6];
extern enumerated_string_fields_mapping_tbl_t UlBandwidth[6];
extern enumerated_string_fields_mapping_tbl_t srsBandwidthConfig[8];
extern  enumerated_string_fields_mapping_tbl_t DeltaPUCCHShift[3];
extern enumerated_string_fields_mapping_tbl_t AccessMode[3];
extern enumerated_string_fields_mapping_tbl_t Action_load[11];
extern enumerated_string_fields_mapping_tbl_t RAN_Mobility_ConnMode_IRAT_MeasQuantityCDMA2000[2];
extern enumerated_string_fields_mapping_tbl_t SUPPORTED_SYS[4];
extern enumerated_string_fields_mapping_tbl_t scell_deact_timer_map[8];
/* SPR 20653 Fix Start */
extern int RBSize[2][2];
extern int Qos_BucketSizeDuration[2][6];
extern int Qos_PrioritizedBitRate[2][8];
/* SPR 20653 Fix End */
/* CSR 00070293 FIX START */
extern int RRM_CELL_TRESELECTIONSF[OAM_TWO][OAM_FOUR];
/* CSR 00070293 FIX END */
extern int delta_f_pucch_format_1[2][3];
extern int delta_f_pucch_format_1b[2][3];
extern int delta_f_pucch_format_2[2][4];
extern int delta_f_pucch_format_2a[2][3];
extern int delta_f_pucch_format_2b[2][3];
extern int RAN_PHY_ULPowerControl_Alpha[2][8]; 
extern int RAN_PHY_PRS_NumConsecutivePRSSubfames[2][4];
extern int RAN_MAC_RACH_NumberOfRaPreambles[2][16];
extern int RAN_MAC_RACH_sizeOfRaGroupA[2][15];
extern int RAN_MAC_RACH_messageSizeGroupA[2][4];
extern int RAN_MAC_RACH_messagePowerOffsetGroupB[2][8];
extern int RAN_MAC_RACH_powerRampingStep[2][4];
extern int RAN_MAC_RACH_preambleInitialReceivedTargetPower[2][16];
extern int RAN_MAC_RACH_preambleTransMax[2][11];
extern int RAN_MAC_RACH_ResponseWindowSize[2][8];
extern int RAN_MAC_RACH_ContentionResolutionTimer[2][8];
/* SPR 21737 Start */
extern int RAN_MAC_DRX_OnDurationTimer[OAM_TWO][ONDURATIONTIMER_COUNT];
extern int RAN_MAC_DRX_drxInactivityTimer[OAM_TWO][DRXINACTIVITYTIMER_COUNT];
extern int RAN_MAC_DRX_drxRetransmissionTimer[OAM_TWO][DRXRETRANSMISSIONTIMER_COUNT];
extern int RAN_MAC_DRX_longDRXCycle[OAM_TWO][LONGDRXCYCLE_COUNT];

#ifdef ENDC_ENABLED
extern int RAN_MAC_DRX_longDRXCycleR15[OAM_TWO][LONGDRXCYCLE_COUNT_R15];
#endif

extern int RAN_MAC_DRX_shortDRXCycle[OAM_TWO][DRXSHORTCYCLE_COUNT];
/* SPR 21737 End */
extern int RAN_RLC_SRB1_TPollRetransmit[2][55];
extern int RAN_RLC_SRB1_pollPDU[2][8]; 
extern int RAN_RLC_SRB1_pollByte[2][15];
extern int RAN_RLC_SRB1_MaxRetxThreshold[2][8];
extern int RAN_RLC_SRB1_TReordering[2][31];
extern int RAN_RLC_SRB1_TStatusProhibit[2][56];
extern int RAN_RRCTimers_T304EUTRA[2][7];
extern int RAN_RRCTimers_T304IRAT [2][7];
extern int RAN_RRCTimers_T310[2][7];
extern int RAN_RRCTimers_T311[2][7];
extern int RAN_RRCTimers_T320[2][7];
extern int RAN_RRCTimers_N310[2][8];
extern int RAN_RRCTimers_N311[2][8];
extern int EPC_QoS_PacketDelayBudget[2][14];
extern int RAN_PHY_PDSCH_PA[2][8];
extern int RAN_Mobility_IdleMode_IntraFreq_TReselectionEUTRASFMedium[2][4];
extern int RAN_Mobility_IdleMode_IntraFreq_TReselectionEUTRASFMedium[2][4];
extern int RAN_Mobility_IdleMode_IntraFreq_TReselectionEUTRASFHigh[2][4];
extern int RAN_RRCTimers_T300[2][8];
extern int RAN_RRCTimers_T301[2][8]; 
extern int RAN_RRCTimers_T302[2][8];
extern int NR_Blacklisted[2][2];
/*SPR 15527 START */
extern int packetErrorLossRate[OAM_TWO][PCKT_ERR_LOSS_RATE_COUNT]; 
extern int EPC_MAX_RETX_THRESHOLD[OAM_TWO][OAM_EIGHT];
/*SPR 15527 END */

/*------------------------------------------------String To Integer-----------------------------------------------*/

/*---------------Type-------------------*/
extern enumerated_string_fields_mapping_tbl_t Type[2];

/*---------------S1ConnectionMode-----------------------*/
extern enumerated_string_fields_mapping_tbl_t S1ConnectionMode[2];

/*---------------------------------AllowedCipheringAlgorithmList-----------------------------------------*/
extern enumerated_string_fields_mapping_tbl_t AllowedCipheringAlgorithmList[3];
/*---------------------------------AllowedIntegrityProtectionAlgorithmList---------------------------------*/
extern enumerated_string_fields_mapping_tbl_t AllowedIntegrityProtectionAlgorithmList[3];
/*---------------------------QHyst----------------------------*/
extern int RAN_Mobility_IdleMode_Common_QHyst[2][16];
/*-------------------QHystSFMedium-----------------------*/
extern int RAN_Mobility_IdleMode_Common_QHystSFMedium[2][4];
														
/*-------------------QHystSFHigh-----------------------*/
extern int RAN_Mobility_IdleMode_Common_QHystSFHigh[2][4];
/*-------------------------TEvaluation-------------------*/
extern int RAN_Mobility_IdleMode_Common_TEvaluation[2][5];

/*-------------------------THystNormal---------------------*/
extern int RAN_Mobility_IdleMode_Common_THystNormal[2][5];

	
extern int RAN_Mobility_ConnMode_EUTRA_FilterCoefficientRSRQ[2][15];
extern int RAN_Mobility_ConnMode_EUTRA_TimeToTrigger[2][16];
extern enumerated_string_fields_mapping_tbl_t TriggerQuantity[2];
extern enumerated_string_fields_mapping_tbl_t ReportQuantity[2];
extern int RAN_Mobility_ConnMode_EUTRA_ReportInterval[2][13];
extern int RAN_Mobility_ConnMode_EUTRA_ReportAmount[2][8];
extern enumerated_string_fields_mapping_tbl_t MeasQuantityUTRAFDD[2];
extern int RAN_Mobility_ConnMode_IRAT_TimeToTrigger[2][16];
extern int RAN_Mobility_ConnMode_IRAT_ReportInterval[2][13];
extern int RAN_Mobility_ConnMode_IRAT_ReportAmount[2][8];
extern int RAN_Mobility_ConnMode_IRAT_FilterCoefficientUTRA[2][15];
extern int RAN_Mobility_ConnMode_IRAT_FilterCoefficientGERAN[2][15];

/* LWA changes start*/
/* LWA changes end*/
/* REL 7.0 changes start*/
extern int RAN_NeighborList_LTECell_QOffset[2][31];
extern int RAN_NeighborList_LTECell_CIO[2][31];
/* REL 7.0 changes end*/

/* SPR 12920 FIX START */
extern enumerated_string_fields_mapping_tbl_t qosDiscardTimer[OAM_EIGHT];
extern enumerated_string_fields_mapping_tbl_t qosSrPeriodicityVal[OAM_SEVEN];
extern enumerated_string_fields_mapping_tbl_t qosSrMaxTransmission[OAM_EIGHT];
extern enumerated_string_fields_mapping_tbl_t CellRestrictionIntraFreqReselection[OAM_TWO];
extern enumerated_string_fields_mapping_tbl_t PhyCellidRange[OAM_SIXTEEN];
extern enumerated_string_fields_mapping_tbl_t MeasBandwidth[OAM_SIX];
/*SPR 15527 START */
extern enumerated_string_fields_mapping_tbl_t SupportedRat_MAP[MAX_SUPPORTED_RAT];
/*SPR 15527 END */

extern enumerated_string_fields_mapping_tbl_t systemTimeInfo[RRM_OAM_SYN_SYS_TIME_COUNT];
extern int altBandClass[OAM_TWO][OAM_TWENTY_TWO];
/* SPR 12920 FIX END */
 /*SPR 15604 START */
/*SPR 15527 START */
int oam_xml_search_string_to_integer
(
        char  *parmeter_value,
        enumerated_string_fields_mapping_tbl_t  *par_table_address,
        int array_size
);
/*SPR 15527 END */
 /*SPR 15604 END */

#endif /* _xml_enum_header_h_ */
