/******************************************************************************
  **    FILE:
  **        Contains the API IDs and MIX MAX definitions for API elements.
  *****************************************************************************
  **
  **    FILE NAME:
  **        rrc_api_defs.h
  **
  **    DESCRIPTION:
  **        This file contains all the API IDs and Min Max Values of API 
  **        elements 
  **
  **
  **    DATE        AUTHOR              REF         REASON
  **    ----        ------              ---         ------
  **    22 May 2002 sraktale            ---         Original
  **	26 Sep 2002 vinagpal			SPR431		Increased 
  **											RRC_MAX_ENCODED_PDU_LENGTH.
  **											
  **	30 Oct 2002	manohar				Fix			SPR #650
  **
  **     Copyright (C) 2006 Aricent Inc . All Rights Reserved
  **    Use TS=4 for best viewing
*******************************************************************************/

#ifndef _RRC_API_DEFS_H_
#define _RRC_API_DEFS_H_

#define RRC_API_BASE	0
/*********************** RRC User to RRC API(s) *******************************/

#define RRC_RRC_USER_TO_RRC_MIN_API		RRC_API_BASE

/**************** API(s) corresponding to DL DCCH messages ********************/
#define RRC_RRC_USER_TO_RRC_MIN_DCFE_API			(RRC_RRC_USER_TO_RRC_MIN_API + 0)

#define RRC_ACTIVE_SET_UPDATE_REQ					(RRC_RRC_USER_TO_RRC_MIN_API + 0) 
#define RRC_ASSIST_DATA_DELIVERY_REQ				(RRC_RRC_USER_TO_RRC_MIN_API + 1)
#define RRC_CELL_CHANGE_ORDER_FROM_UTRAN_REQ		(RRC_RRC_USER_TO_RRC_MIN_API + 2)
#define RRC_CELL_UPDATE_CNF_RESP                    (RRC_RRC_USER_TO_RRC_MIN_API + 3)
#define RRC_COUNTER_CHECK_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 4)
#define RRC_DOWNLINK_DIRECT_TRANS_REQ				(RRC_RRC_USER_TO_RRC_MIN_API + 5)
#define RRC_HANDOVER_FROM_UTRAN_CMD   			    (RRC_RRC_USER_TO_RRC_MIN_API + 6)
#define RRC_MEAS_CTRL_REQ							(RRC_RRC_USER_TO_RRC_MIN_API + 7)
#define RRC_PAGING_TYPE_2_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 8)
#define RRC_PHY_CH_RECONFIG_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 9)
#define RRC_RADIO_BEARER_RECONFIG_REQ				(RRC_RRC_USER_TO_RRC_MIN_API + 10)
#define RRC_RADIO_BEARER_REL_REQ				    (RRC_RRC_USER_TO_RRC_MIN_API + 11)
#define RRC_RADIO_BEARER_SETUP_REQ					(RRC_RRC_USER_TO_RRC_MIN_API + 12)
#define RRC_CONN_REL_REQ							(RRC_RRC_USER_TO_RRC_MIN_API + 13)
#define RRC_SEC_MODE_CMD_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 14)
#define RRC_SIG_CONN_REL_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 15)
#define RRC_TR_CH_RECONFIG_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 16)
#define RRC_TFCS_CTRL_REQ							(RRC_RRC_USER_TO_RRC_MIN_API + 17)
#define RRC_UE_CAPAB_ENQUIRY_REQ					(RRC_RRC_USER_TO_RRC_MIN_API + 18)
#define RRC_URA_UPDATE_CNF_RESP                     (RRC_RRC_USER_TO_RRC_MIN_API + 19)
#define RRC_UTRAN_MOB_INFO_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 20)

/******************* API(s) corresponding to DL CCCH messages *****************/

#define RRC_CONN_SETUP_RESP						    (RRC_RRC_USER_TO_RRC_MIN_API + 21)

/*************************** LOCAL API(s) *************************************/

#define RRC_CLEAR_REQ								(RRC_RRC_USER_TO_RRC_MIN_API + 22)
#define RRC_DECODE_TO_TRNC_CONTAINER_REQ			(RRC_RRC_USER_TO_RRC_MIN_API + 23)
#define RRC_HANDOVER_TO_UTRAN_CMD_REQ			    (RRC_RRC_USER_TO_RRC_MIN_API + 24)
#define RRC_RELOC_CONTAINER_REQ					    (RRC_RRC_USER_TO_RRC_MIN_API + 25)
#define RRC_SRNC_RELOC_CANCEL_REQ				    (RRC_RRC_USER_TO_RRC_MIN_API + 26)
#define RRC_SRNC_RELOC_CMD_REQ					    (RRC_RRC_USER_TO_RRC_MIN_API + 27)
#define RRC_SRNC_USTI_REQ							(RRC_RRC_USER_TO_RRC_MIN_API + 28)
#define RRC_TRNC_RELOC_COMMIT_REQ					(RRC_RRC_USER_TO_RRC_MIN_API + 29)
#define RRC_TRNC_RELOC_SETUP_REQ					(RRC_RRC_USER_TO_RRC_MIN_API + 30)
#define RRC_TRNC_SRNC_CONTAINER_REQ					(RRC_RRC_USER_TO_RRC_MIN_API + 31)
#define RRC_MACD_MEAS_CONFIG_REQ		            (RRC_RRC_USER_TO_RRC_MIN_API + 32)

/* DCFE APIs End */

#define RRC_RRC_USER_TO_RRC_MAX_DCFE_API			(RRC_RRC_USER_TO_RRC_MIN_API + 32)

#define RRC_RRC_USER_TO_RRC_NUM_DCFE_API			\
	(RRC_RRC_USER_TO_RRC_MAX_DCFE_API - RRC_RRC_USER_TO_RRC_MIN_DCFE_API + 1)
	
/* CCFE APIs begin */
#define RRC_RRC_USER_TO_RRC_MIN_CCFE_API			(RRC_RRC_USER_TO_RRC_MIN_API + 33)

#define RRC_PAGING_TYPE_1_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 33)
#define RRC_CONFIG_CELL_REQ                         (RRC_RRC_USER_TO_RRC_MIN_API + 34)
#define RRC_RECONFIG_CELL_REQ                       (RRC_RRC_USER_TO_RRC_MIN_API + 35)
#define RRC_DELETE_CELL_REQ                         (RRC_RRC_USER_TO_RRC_MIN_API + 36)
#define RRC_CONN_REJ_RESP                           (RRC_RRC_USER_TO_RRC_MIN_API + 37)
#define RRC_SIB10_BROADCAST_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 38)
#define RRC_NOTIFICATION_REQ                        (RRC_RRC_USER_TO_RRC_MIN_API + 39)
#define RRC_DRNC_DSTI_REQ							(RRC_RRC_USER_TO_RRC_MIN_API + 40)

/* CCFE APIs End */
#define RRC_RRC_USER_TO_RRC_MAX_CCFE_API			(RRC_RRC_USER_TO_RRC_MIN_API + 40)

#define RRC_RRC_USER_TO_RRC_NUM_CCFE_API			\
	(RRC_RRC_USER_TO_RRC_MAX_CCFE_API - RRC_RRC_USER_TO_RRC_MIN_CCFE_API + 1)
	
/*********************** API(s) corresponding to RRC Config **************************/

#define RRC_RRC_USER_TO_RRC_MIN_CONFIG_API			(RRC_RRC_USER_TO_RRC_MIN_API + 41)

#define RRC_PREDEFINED_CONFIG_REQ       			(RRC_RRC_USER_TO_RRC_MIN_API + 41)
#define RRC_DEFAULT_CONFIG_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 42)
#define RRC_DRX_CYCLE_CONFIG_REQ        			(RRC_RRC_USER_TO_RRC_MIN_API + 43)
#define RRC_CONFIG_COUNTER_REQ						(RRC_RRC_USER_TO_RRC_MIN_API + 44)
	
#define RRC_RRC_USER_TO_RRC_MAX_CONFIG_API			(RRC_RRC_USER_TO_RRC_MIN_API + 44)

#define RRC_RRC_USER_TO_RRC_NUM_CONFIG_API			\
	(RRC_RRC_USER_TO_RRC_MAX_CONFIG_API - RRC_RRC_USER_TO_RRC_MIN_CONFIG_API + 1)		

#define RRC_RRC_USER_TO_RRC_MAX_API					(RRC_RRC_USER_TO_RRC_MIN_API + 44)

#define RRC_RRC_USER_TO_RRC_NUM_API					\
	(RRC_RRC_USER_TO_RRC_MAX_API - RRC_RRC_USER_TO_RRC_MIN_API + 1)	

/************************ RRC to RRC User API(s) ******************************/

#define RRC_RRC_TO_RRC_USER_MIN_API					(RRC_RRC_USER_TO_RRC_MAX_API + 1)

/***************** API(s) corresponding to UL DCCH messages *******************/
#define RRC_RRC_TO_RRC_USER_MIN_DCFE_API			(RRC_RRC_TO_RRC_USER_MIN_API + 0)
	
#define RRC_ACTIVE_SET_UPDATE_COMP_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 0)
#define RRC_ACTIVE_SET_UPDATE_FAIL_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 1)
#define RRC_CELL_CHANGE_ORDER_FROM_UTRAN_FAIL_CNF	(RRC_RRC_TO_RRC_USER_MIN_API + 2)
#define RRC_COUNTER_CHECK_RESP_CNF					(RRC_RRC_TO_RRC_USER_MIN_API + 3)
#define RRC_HANDOVER_TO_UTRAN_COMP_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 4)
#define RRC_INITIAL_DIRECT_TRANS_IND				(RRC_RRC_TO_RRC_USER_MIN_API + 5)
#define RRC_HANDOVER_FROM_UTRAN_FAIL_CNF			(RRC_RRC_TO_RRC_USER_MIN_API + 6)
#define RRC_MEAS_CTRL_FAIL_CNF						(RRC_RRC_TO_RRC_USER_MIN_API + 7)	
#define RRC_MEAS_REP_IND							(RRC_RRC_TO_RRC_USER_MIN_API + 8)
#define RRC_PHY_CH_RECONFIG_COMP_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 9)
#define RRC_PHY_CH_RECONFIG_FAIL_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 10)
#define RRC_RADIO_BEARER_RECONFIG_COMP_CNF			(RRC_RRC_TO_RRC_USER_MIN_API + 11)
#define RRC_RADIO_BEARER_RECONFIG_FAIL_CNF			(RRC_RRC_TO_RRC_USER_MIN_API + 12)
#define RRC_RADIO_BEARER_REL_COMP_CNF			    (RRC_RRC_TO_RRC_USER_MIN_API + 13)
#define RRC_RADIO_BEARER_REL_FAIL_CNF			    (RRC_RRC_TO_RRC_USER_MIN_API + 14)
#define RRC_RADIO_BEARER_SETUP_COMP_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 15)
#define RRC_RADIO_BEARER_SETUP_FAIL_CNF				(RRC_RRC_TO_RRC_USER_MIN_API + 16)
#define RRC_CONN_REL_COMP_CNF						(RRC_RRC_TO_RRC_USER_MIN_API + 17)
#define RRC_CONN_SETUP_COMP_CNF						(RRC_RRC_TO_RRC_USER_MIN_API + 18)
#define RRC_SEC_MODE_CMD_COMP_CNF					(RRC_RRC_TO_RRC_USER_MIN_API + 19)
#define RRC_SEC_MODE_CMD_FAIL_CNF					(RRC_RRC_TO_RRC_USER_MIN_API + 20)
#define RRC_SIG_CONN_REL_IND						(RRC_RRC_TO_RRC_USER_MIN_API + 21)
#define RRC_TR_CH_RECONFIG_COMP_CNF					(RRC_RRC_TO_RRC_USER_MIN_API + 22)
#define RRC_TR_CH_RECONFIG_FAIL_CNF					(RRC_RRC_TO_RRC_USER_MIN_API + 23)
#define RRC_TFCS_CTRL_FAIL_CNF						(RRC_RRC_TO_RRC_USER_MIN_API + 24)
#define RRC_UE_CAPAB_INFO_IND						(RRC_RRC_TO_RRC_USER_MIN_API + 25)
#define RRC_UPLINK_DIRECT_TRANS_IND					(RRC_RRC_TO_RRC_USER_MIN_API + 26)
#define RRC_UTRAN_MOB_INFO_COMP_CNF					(RRC_RRC_TO_RRC_USER_MIN_API + 27)
#define RRC_UTRAN_MOB_INFO_FAIL_CNF		    		(RRC_RRC_TO_RRC_USER_MIN_API + 28)

/****************** API(s) corresponding to UL CCCH messages ******************/

#define RRC_CELL_UPDATE_IND					        (RRC_RRC_TO_RRC_USER_MIN_API + 29)
#define RRC_CONN_REQ_IND						    (RRC_RRC_TO_RRC_USER_MIN_API + 30)
#define RRC_URA_UPDATE_IND						    (RRC_RRC_TO_RRC_USER_MIN_API + 31)

/***************************** LOCAL API(s) ***********************************/

#define RRC_CELL_UPDATE_CNF_FAIL_CNF                (RRC_RRC_TO_RRC_USER_MIN_API + 32)
#define RRC_URA_UPDATE_CNF_FAIL_CNF                 (RRC_RRC_TO_RRC_USER_MIN_API + 33)
#define RRC_CLEAR_IND                               (RRC_RRC_TO_RRC_USER_MIN_API + 34)
#define RRC_CONN_SETUP_FAIL_CNF                     (RRC_RRC_TO_RRC_USER_MIN_API + 35)
#define RRC_DECODE_SRNC_RELOC_CONTAINER_INFO_CNF    (RRC_RRC_TO_RRC_USER_MIN_API + 36)
#define RRC_DECODE_TO_TRNC_CONTAINER_FAIL_CNF       (RRC_RRC_TO_RRC_USER_MIN_API + 37)
#define RRC_HANDOVER_TO_UTRAN_CMD_COMP_CNF          (RRC_RRC_TO_RRC_USER_MIN_API + 38)
#define RRC_HANDOVER_TO_UTRAN_CMD_FAIL_CNF          (RRC_RRC_TO_RRC_USER_MIN_API + 39)
#define RRC_INTER_RAT_HANDOVER_INFO_WITH_CAP_CNF    (RRC_RRC_TO_RRC_USER_MIN_API + 40) 
#define RRC_RELOC_CONTAINER_COMP_CNF                (RRC_RRC_TO_RRC_USER_MIN_API + 41)
#define RRC_RELOC_CONTAINER_FAIL_CNF                (RRC_RRC_TO_RRC_USER_MIN_API + 42)
#define RRC_SRNC_DSTI_IND                           (RRC_RRC_TO_RRC_USER_MIN_API + 43)
#define RRC_SRNC_RELOC_CMD_FAIL_CNF                 (RRC_RRC_TO_RRC_USER_MIN_API + 44)
#define RRC_SRNC_USTI_FAIL_CNF                      (RRC_RRC_TO_RRC_USER_MIN_API + 45)
#define RRC_TRNC_RELOC_COMMIT_COMP_CNF              (RRC_RRC_TO_RRC_USER_MIN_API + 46)
#define RRC_TRNC_RELOC_COMMIT_FAIL_CNF              (RRC_RRC_TO_RRC_USER_MIN_API + 47)
#define RRC_TRNC_RELOC_COMP_CNF                     (RRC_RRC_TO_RRC_USER_MIN_API + 48)
#define RRC_TRNC_RELOC_SETUP_COMP_CNF               (RRC_RRC_TO_RRC_USER_MIN_API + 49)
#define RRC_TRNC_RELOC_SETUP_FAIL_CNF               (RRC_RRC_TO_RRC_USER_MIN_API + 50)
#define RRC_TRNC_SRNC_CONTAINER_COMP_CNF            (RRC_RRC_TO_RRC_USER_MIN_API + 51)
#define RRC_TRNC_SRNC_CONTAINER_FAIL_CNF            (RRC_RRC_TO_RRC_USER_MIN_API + 52)
#define RRC_DECODE_RELOC_CONTAINER_CNF              (RRC_RRC_TO_RRC_USER_MIN_API + 53)
#define RRC_MACD_INST_ID_NOTIFY_IND                 (RRC_RRC_TO_RRC_USER_MIN_API + 54)
#define RRC_MACD_MEAS_CONFIG_CNF		            (RRC_RRC_TO_RRC_USER_MIN_API + 55)
#define RRC_MACD_MEAS_IND							(RRC_RRC_TO_RRC_USER_MIN_API + 56)
#define RRC_STATUS_CNF                              (RRC_RRC_TO_RRC_USER_MIN_API + 57)
#define RRC_UNRECOVER_LL_ERROR_IND                  (RRC_RRC_TO_RRC_USER_MIN_API + 58)
#define RRC_LL_STATUS_IND                  			(RRC_RRC_TO_RRC_USER_MIN_API + 59)

/* Common */
#define RRC_RRC_TO_RRC_USER_MIN_CCFE_API			(RRC_RRC_TO_RRC_USER_MIN_API + 60)

#define RRC_LOCAL_FAIL_CNF                          (RRC_RRC_TO_RRC_USER_MIN_API + 60)

#define RRC_RRC_TO_RRC_USER_MAX_DCFE_API			(RRC_RRC_TO_RRC_USER_MIN_API + 60)

#define RRC_RRC_TO_RRC_USER_NUM_DCFE_API			\
	(RRC_RRC_TO_RRC_USER_MAX_DCFE_API - RRC_RRC_TO_RRC_USER_MIN_DCFE_API + 1)
	
/* Cell */
#define RRC_DRNC_USTI_IND                           (RRC_RRC_TO_RRC_USER_MIN_API + 61)
#define RRC_CONFIG_CELL_CNF                         (RRC_RRC_TO_RRC_USER_MIN_API + 62)
#define RRC_DELETE_CELL_CNF                         (RRC_RRC_TO_RRC_USER_MIN_API + 63)
#define RRC_RECONFIG_CELL_CNF                       (RRC_RRC_TO_RRC_USER_MIN_API + 64)

#define RRC_RRC_TO_RRC_USER_MAX_CCFE_API			(RRC_RRC_TO_RRC_USER_MIN_API + 64)

#define RRC_RRC_TO_RRC_USER_NUM_CCFE_API			\
	(RRC_RRC_TO_RRC_USER_MAX_CCFE_API - RRC_RRC_TO_RRC_USER_MIN_CCFE_API + 1)
	
/* Config */
#define RRC_RRC_TO_RRC_USER_MIN_CONFIG_API			(RRC_RRC_TO_RRC_USER_MIN_API + 65)
	
#define RRC_DEFAULT_CONFIG_CNF                 		(RRC_RRC_TO_RRC_USER_MIN_API + 65)
#define RRC_DRX_CYCLE_CONFIG_CNF               		(RRC_RRC_TO_RRC_USER_MIN_API + 66)
#define RRC_PREDEFINED_CONFIG_CNF                   (RRC_RRC_TO_RRC_USER_MIN_API + 67)
#define RRC_CONFIG_COUNTER_CNF						(RRC_RRC_TO_RRC_USER_MIN_API + 68)


#define RRC_RRC_TO_RRC_USER_MAX_CONFIG_API			(RRC_RRC_TO_RRC_USER_MIN_API + 68)

#define RRC_RRC_TO_RRC_USER_NUM_CONFIG_API			\
	(RRC_RRC_TO_RRC_USER_MAX_CONFIG_API - RRC_RRC_TO_RRC_USER_MIN_CONFIG_API + 1)
	
#define RRC_RRC_TO_RRC_USER_MAX_API					(RRC_RRC_TO_RRC_USER_MIN_API + 68)

#define RRC_RRC_TO_RRC_USER_NUM_API		(RRC_RRC_TO_RRC_USER_MAX_API - RRC_RRC_TO_RRC_USER_MIN_API + 1)

#define RRC_MAX_API									(RRC_RRC_TO_RRC_USER_MAX_API)
#define RRC_NUM_API									(RRC_MAX_API - RRC_API_BASE + 1)

#ifdef RRC_PT
#define RRC_PT_DISPLAY_PT_TIME						(RRC_MAX_API + 1)
#define RRC_INVALID_API_ID							(RRC_MAX_API + 2)
#else
#define RRC_INVALID_API_ID							(RRC_MAX_API + 1)
#endif /* RRC_PT */
#endif /* _RRC_API_DEFS_H_ */