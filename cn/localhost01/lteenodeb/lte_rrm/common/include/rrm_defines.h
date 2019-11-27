/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_defines.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains basic RRM types definitions.
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   3 APR 2012      gur30487    Initial
 *  28 APR 2014      gur27356    SPR-10397       Updated for PDCP KPI stat.
 *  12 May 2014      gur32417    SPR 11026       RRM not sending Cell Index bitmask 
 *                                               when Scell Add.
 *  7  MAY 2014      gur29990    SPR-10949       Bug Fixed.
 *  31 JUL 2014      gur30784    SPR-9464        DRX profile changes
 ******************************************************************************/

#ifndef _RRM_DEFINES_H_
#define _RRM_DEFINES_H_

#ifdef HAVE_CONFIG_H
/*! \headerfile <> config.h
 */
#include "config.h"
#endif

/*!	\headerfile <> cspl.h
 *	\brief		 CSPL types 
 */
#include <cspl.h>
/*! \headerfile <> stacklayer.h 
 */
#include <stacklayer.h>
/*! \headerfile <> rrm_api_types.h
 */
#include "rrm_api_types.h"
/*! \headerfile <> rrm_api_defines.h
 */
#include "rrm_api_defines.h"
/*! \headerfile <> rrc_rrm_intf.h
 */
#include "rrc_rrm_intf.h"
/** ALARM Framework */
#ifdef ALM_FRWK
/*! \headerfile <> alarm.h 
 */
	#include <alarm.h>
#endif

#ifdef ALM_FRWK
/*! \def ALARM_MSG_RRM
 */
    #define ALARM_MSG_RRM    		ALARM_MSG
 /*! \def ALARM_FMT_MSG_RRM
 */   
 #define ALARM_FMT_MSG_RRM    	ALARM_FMT_MSG
#else
/*! \def ALARM_MSG_RRM
 */
    #define ALARM_MSG_RRM(src_module_id, alarm_id, criticality)
 /*! \def ALARM_FMT_MSG_RRM
 */   
 #define ALARM_FMT_MSG_RRM(src_module_id, alarm_id, criticality , ...)
#endif
/*! \def RRM_RRC_SEND_FAIL
 */
#define RRM_RRC_SEND_FAIL		1
/*! \def RRM_ZERO
 */
#define RRM_ZERO                0

#define RRM_RRC_SEND_FAIL               1

/*! \def RRM_MINUS_ONE
 */
#define RRM_MINUS_ONE                -1
/* SPR 15776 Fix Start */
/*! \def RRM_MINUS_TWENTY_FOUR
 */
#define RRM_MINUS_TWENTY_FOUR -24

/*! \def RRM_MINUS_TWENTY_TWO
 */
#define RRM_MINUS_TWENTY_TWO  -22

/*! \def RRM_MINUS_TWENTY
 */
#define RRM_MINUS_TWENTY      -20

/*! \def RRM_MINUS_EIGHTEEN
 */
#define RRM_MINUS_EIGHTEEN    -18

/*! \def RRM_MINUS_SIXTEEN
 */
#define RRM_MINUS_SIXTEEN     -16

/*! \def RRM_MINUS_FOURTEEN
 */
#define RRM_MINUS_FOURTEEN    -14

/*! \def RRM_MINUS_TWELVE
 */
#define RRM_MINUS_TWELVE      -12

/*! \def RRM_MINUS_TEN
 */
#define RRM_MINUS_TEN         -10

/*! \def RRM_MINUS_EIGHT
 */
#define RRM_MINUS_EIGHT       -8

/*! \def RRM_MINUS_SIX
 */
#define RRM_MINUS_SIX         -6

/*! \def RRM_MINUS_FIVE
 */
#define RRM_MINUS_FIVE        -5

/*! \def RRM_MINUS_FOUR
 */
#define RRM_MINUS_FOUR        -4

/*! \def RRM_MINUS_THREE
 */
#define RRM_MINUS_THREE       -3

/*! \def RRM_MINUS_TWO
 */
#define RRM_MINUS_TWO         -2

/*! \def RRM_EIGHTEEN
 */
#define RRM_EIGHTEEN          18

/* SPR 15776 Fix Stop */

/*! \def RRM_U16_LOW_BYTE_MASK
 */
#define RRM_U16_LOW_BYTE_MASK         0x00FF

/*! \def RRM_U16_HIGH_BYTE_MASK
 */
#define RRM_U16_HIGH_BYTE_MASK         0xFF00

/*! \def RRM_HIGH_NIBBLE
 */
#define RRM_HIGH_NIBBLE		0xF0

/*! \def RRM_LOW_NIBBLE
 */
#define RRM_LOW_NIBBLE		0x0F

/*added for compilation will be exposed by MAC*/
/*! \def RRM_L2_KPI_STATS_IND
 */
#define RRM_L2_KPI_STATS_IND 41
/*! \def RRM_L2_GET_SNR_REPORT
 */
#define RRM_L2_GET_SNR_REPORT 42
/*! \def RRM_L2_SNR_REPORT
 */
#define RRM_L2_SNR_REPORT    43

/*! \def RRM_MEAS_REPORT_PERIODICITY_BUFFER
 */
#define RRM_MEAS_REPORT_PERIODICITY_BUFFER          100

/*! \def RRM_MEAS_REPORT_PERIODICITY
 */
#define RRM_MEAS_REPORT_PERIODICITY_120             120
#define RRM_MEAS_REPORT_PERIODICITY_240             240
#define RRM_MEAS_REPORT_PERIODICITY_480             480
#define RRM_MEAS_REPORT_PERIODICITY_640             640
#define RRM_MEAS_REPORT_PERIODICITY_1024            1024
#define RRM_MEAS_REPORT_PERIODICITY_2048            2048
/*SPR 19096 Start*/
#define RRM_MEAS_REPORT_PERIODICITY_5120            10120
/*SPR 19096 End*/
#define RRM_MEAS_REPORT_PERIODICITY_10240           10240
#define RRM_MEAS_REPORT_PERIODICITY_MIN_1           (1*60*1000)
#define RRM_MEAS_REPORT_PERIODICITY_MIN_6           (6*60*1000)
#define RRM_MEAS_REPORT_PERIODICITY_MIN_12          (12*60*1000)
#define RRM_MEAS_REPORT_PERIODICITY_MIN_30          (30*60*1000)
#define RRM_MEAS_REPORT_PERIODICITY_MIN_60          (60*60*1000)

/*! \def RRM_PACKET_DELAY_BUDGET
 */
#define RRM_PACKET_DELAY_BUDGET_MS50                50
#define RRM_PACKET_DELAY_BUDGET_MS100               100
#define RRM_PACKET_DELAY_BUDGET_MS150               150
#define RRM_PACKET_DELAY_BUDGET_MS200               200
#define RRM_PACKET_DELAY_BUDGET_MS300               300
#define RRM_PACKET_DELAY_BUDGET_MS400               400
#define RRM_PACKET_DELAY_BUDGET_MS500               500
#define RRM_PACKET_DELAY_BUDGET_MS600               600
#define RRM_PACKET_DELAY_BUDGET_MS700               700
#define RRM_PACKET_DELAY_BUDGET_MS800               800
#define RRM_PACKET_DELAY_BUDGET_MS900               900
#define RRM_PACKET_DELAY_BUDGET_MS1000              1000
#define RRM_PACKET_DELAY_BUDGET_MS1500              1500
#define RRM_PACKET_DELAY_BUDGET_MS2000              2000

/*! \def RRM_PACKET_ERR_LOSS_RATE
 */
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_0         1000000
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_1         100000
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_2         10000
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_3         1000
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_4         100
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_5         10
#define RRM_PACKET_ERR_LOSS_RATE_10_POWER_6         1

/* Scell State */
#define RRM_SCELL_ACTIVE 1
#define RRM_SCELL_INACTIVE 0

/* CA bitmasks : Start */
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_COMMON_SCELL 0x00008000000000ULL
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_DEDICATED_SCELL 0x00004000000000ULL
#define RRM_SCELL_ADD_MOD_MAC_MAIN_CONFIG_EXT_SCELL 0x00002000000000ULL
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_DEDICATED_NON_UL 0x00001000000000ULL
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_DEDICATED_UL 0x00000800000000ULL
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_DEDICATED_NON_UL_TM_MODE_CHANGE 0x00000400000000ULL
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_DEDICATED_NON_UL_PA_CHANGE 0x00000200000000ULL
#define RRM_SCELL_ADD_MOD_RADIO_RESOURCE_CONFIG_DEDICATED_UL_CQI_PARAM_CHANGE 0x00000100000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_DL_NUM_HARQ_PROCESS 0x00000080000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_DL_I_MCS 0x00000040000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_NUM_OF_LAYER 0x00000020000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_CODE_BOOK_INDEX 0x00000010000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_USER_LOCATION_TYPE 0x00000008000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_HALF_DUPLEX_FDD 0x00000004000000ULL
#define RRM_MAC_MAIN_CONFIG_EXT_SCELL_ACTIVATE_STATUS 0x00000002000000ULL
#define RRM_PCELL_PHY_CONFIG_DEDICATED_PRESENT 0x00000001000000ULL
#define RRM_PCELL_MAC_CONFIG_PARAM_PRESENT 0x00000000800000ULL       /*If Scell Deact Timer value changed from OAM.*/
#define PCELL_ANTENNA_INFO_R10_PRESENT 0x00000000400000ULL
#define PCELL_CQI_INFO_R10_PRESENT 0x00000000200000ULL
#define PCELL_PHY_CONFIG_DEDICATED_EXT_PRESENT 0x00000000100000ULL
#define RRM_UE_RECONFIG_REQ_RR_RECONFIG_PARAMS_PCELL_PRESENT 0x00000000080000ULL
#define RRM_UE_RECONFIG_REQ_SCELL_CONFIG_PARAMS_PRESENT 0x00000000040000ULL
/* SPR_11026_Fix: Start */
#define RRM_SCELL_ADD_MOD_SCELL_IDX_AND_ID 0x00000000020000ULL
/* SPR_11026_Fix: End */
/*  LAA Rel13 Code + */
/*  LAA Rel13 Code - */

/* R-10 TM Mode Num of Bits */ 
#define RRM_NUM_CBSR_BITS_N2_TX_ANTENNA_TM3 2
#define RRM_NUM_CBSR_BITS_N4_TX_ANTENNA_TM3 4
#define RRM_NUM_CBSR_BITS_N2_TX_ANTENNA_TM4 6
#define RRM_NUM_CBSR_BITS_N4_TX_ANTENNA_TM4 64
#define RRM_NUM_CBSR_BITS_N2_TX_ANTENNA_TM5 4
#define RRM_NUM_CBSR_BITS_N4_TX_ANTENNA_TM5 16
#define RRM_NUM_CBSR_BITS_N2_TX_ANTENNA_TM6 4
#define RRM_NUM_CBSR_BITS_N4_TX_ANTENNA_TM6 16
#define RRM_NUM_CBSR_BITS_N2_TX_ANTENNA_TM8 6
#define RRM_NUM_CBSR_BITS_N4_TX_ANTENNA_TM8 32

/* CA bitmasks : End */

/* CA_Stage_3_Scell_TM_change: Start */
/* bit 0 of byte 0 indicates presence Scell report - LTE MAC Api doc */
#define RRM_MAC_UE_PERIODIC_SCELL_REPORT_PRESENCE_BITMASK 0x01
/* CA_Stage_3_Scell_TM_change: End */
/* review comment changes start */
//code deleted
/* review comment changes end */
/*Rel10 CR 393 changes start*/
/*cause of load balancing req from x2ap to rrm */
/*! \enum   rrm_x2ap_report_characteristics_set_et
 *  \brief Action on report characteristics from x2ap
 */
typedef enum
{
    RRM_X2AP_PRB_REPORT_CHAR_SET = 1,
    RRM_X2AP_TNL_LOAD_REPORT_CHAR_SET = 2,
    RRM_X2AP_HW_LOAD_REPORT_CHAR_SET = 4,
    RRM_X2AP_BOTH_TNL_AND_HW_REPORT_CHAR_SET = 6,
    RRM_X2AP_COMPOSITE_CAPACITY_REPORT_CHAR_SET = 8,
/* eICIC_PHASE_1_2_CHANGES_START */
    RRM_X2AP_ABS_STATUS_REPORT_CHAR_SET = 16,
/* eICIC_PHASE_1_2_CHANGES_END */
}rrm_x2ap_report_characteristics_set_et;
/*Rel10 CR 393 changes end*/
/*SPR 18994 START*/
typedef enum
{
    RRM_PRB_REPORT_CHAR_SET ,
    RRM_TNL_LOAD_REPORT_CHAR_SET ,
    RRM_HW_LOAD_REPORT_CHAR_SET,
    RRM_COMPOSITE_CAPACITY_REPORT_CHAR_SET ,
/* eICIC_PHASE_1_2_CHANGES_START */
    RRM_ABS_STATUS_REPORT_CHAR_SET ,
/* eICIC_PHASE_1_2_CHANGES_END */
}rrm_report_characteristics_et;
/*SPR 18994 END*/
/*health monitoring changes start */
/*! \def RRM_MAX_THREADS
 */
#define RRM_MAX_THREADS 2
/*  DYNAMIC ICIC CHANGES START  */
/*SPR21886 Start*/
#define RRM_MAX_NUM_L2_INSTANCE                 MAX_NUM_L2_INSTANCE
/*SPR21886 End*/
/*cause of UE Location from CELLM to UEM*/
/*! \enum   rrm_location_update_action_et
 *  \brief Action on UE location from CC to CE and vice-versa
 */
typedef enum
{
    UE_LOCATION_UPDATE_CE_TO_CC,
    UE_LOCATION_UPDATE_CC_TO_CE,
    INVALID_REASON
}rrm_location_update_action_et;

/*cause of UE OLPC Action from CELLM to UEM*/
/*! \enum   rrm_location_update_action_et
 *  \brief Action on UE location from CC to CE and vice-versa
 */
typedef enum
{
    UE_OLPC_REDUCE_POWER_ACTION,
    UE_OLPC_INVALID_POWER_ACTION
}rrm_olpc_reduce_power_action_et;

/*failure cause of Action from UEM to CELLM*/
/*! \enum  rrm_icic_action_response_et 
 *  \brief Response Action from UEM
 */
typedef enum
{
    UE_OLPC_REDUCE_POWER_ACTION_SUCCESS,
    UE_LOCATION_INFO_ACTION_SUCCESS,
    UE_OLPC_REDUCE_POWER_ACTION_FAILURE,
    UE_LOCATION_INFO_ACTION_FAILURE,
    UE_PARTIAL_SUCCESS_WITH_OLPC_REDUCE_POWER_ACTION_FAILED,
    UE_PARTIAL_SUCCESS_WITH_LOCATION_INFO_ACTION_FAILED,
    UE_COMBINED_ACTION_FAILURE,
    UE_COMBINED_ACTION_SUCCESS
}rrm_icic_action_response_et;

/*  DYNAMIC ICIC CHANGES END  */

/* UE Positioning start */
typedef enum
{
    RRM_ECID_MEAS_REPORT_FAILURE,           /* eNodeb fails to fetch the reqd reports */
    RRM_ECID_MEAS_REPORT_SUCCESS,           /* eNodeB succeeds in fetching the reqd reports */
    RRM_ECID_MEAS_REPORT_PARTIAL_SUCCESS,   /* eNodeB succeeds in fetching some type of reports and fails in others */
    INVALID_ECID_MEAS_REQ_API_LEN           /* Indicates invalid IE length is received for RRM_MAC_E_CID_MEAS_REPORT_REQ */
}rrm_mac_meas_report_response_code_et;

/* Fix for CSR: 00059124 */
typedef enum
{
    RRM_CELL_EDGE_UES_SUPPORTING_AUTONOMOUS_GAP,
    RRM_CELL_EDGE_UES_NOT_SUPPORTING_AUTONOMOUS_GAP,
    RRM_INACTIVE_UES_SUPPORTING_AUTONOMOUS_GAP,
    RRM_INACTIVE_UES_NOT_SUPPORTING_AUTONOMOUS_GAP,
    /* Start fix for SPR 14987 */
    RRM_REMAINING_ACTIVE_UES,
    /* End fix for SPR 14987 */
    RRM_MAX_ANR_MEAS_CONFIG_UE_CATEGORY
}rrm_anr_meas_config_ue_category_et;
/* Fix for CSR: 00059124 */

#define RRM_UE_POS_TRIGGER_QTY_RSRP             0x01
#define RRM_UE_POS_TRIGGER_QTY_RSRQ             0x02
#define RRM_UE_POS_TRIGGER_QTY_UE_RXTX_DIFF     0x04
/* UE Positioning end */

/*spr 7996 start*/
#define RRM_UE_RELASE_GUARD_TIMER 5
/*spr 7996 end*/
/* SPR 15336 Fix Start */
/* earlier value 5 was very less so increased to 5000 ms */
#define RRM_DEFAULT_CSFB_BLIND_TIMER 5000
/* SPR 15336 Fix End */

/*! \def NO_OF_MODULES
 */
#define NO_OF_MODULES                           5
/*! \def RRMRRC
 */
#define RRMRRC                                  1
/*! \def RRMITFMGR
 */
#define RRMITFMGR                               2
/*! \def RRM_API_HEADER_SIZE
 */
#define RRM_API_HEADER_SIZE 					16

 /*! \def RRM_INTERFACE_API_HEADER_SIZE 
  *  \brief A macro that stores the message External API header size
   */
#define RRM_INTERFACE_API_HEADER_SIZE           16

/*! \def ENDIAN_INIT
 */
#define ENDIAN_INIT 							1
/*! \def IS_LITTLE_ENDIAN
 */
#define IS_LITTLE_ENDIAN(endian_check) 			(*((U8*)&endian_check)?1:0)
/*! \def IS_BIG_ENDIAN
 */
#define IS_BIG_ENDIAN(endian_check) 			(*((U8*)&endian_check)?0:1)

/*! \def RRM_NULL
 */
#define RRM_NULL        						0
#ifndef _PNULL_
#define _PNULL_
/*! \def RRM_PNULL
 */
#define RRM_PNULL           					((void *)0)
#endif

/*! \def RRM_PNULL
 */
#define RRM_PNULL           					((void *)0)

/*! \def RRM_OUT_OF_RANGE
 */
#define RRM_OUT_OF_RANGE                                             0xFF

/*! \def RRM_OUT_OF_RANGE_TWO_BYTE
 */
#define RRM_OUT_OF_RANGE_TWO_BYTE				     0xFFFF

/*! \def RRM_OUT_OF_RANGE_FOUR_BYTE
 */
#define RRM_OUT_OF_RANGE_FOUR_BYTE                                   0xFFFFFFFF

/*! \def RRM_SEC_TO_MILLI_SEC_CONV
 */
#define RRM_SEC_TO_MILLI_SEC_CONV                                    1000

/*! \def RRM_MEGA_BIT
 */
#define RRM_MEGA_BIT                                            (1024 * 1024 * 8)

/*! \def RRM_MEGA
 */
#define RRM_MEGA                                                  1000000

/*! \def RRM_TIME_STAMP_BASE_YEAR
 */
#define RRM_TIME_STAMP_BASE_YEAR                                     1900

/*! \def RRM_API_ID_INDEX
 */
#define RRM_API_ID_INDEX						6
/*! \def RRM_MSG_SIZE_INDEX
 */
#define RRM_MSG_SIZE_INDEX						8
/*! \def RRM_SRC_ID_INDEX
 */
#define	RRM_SRC_ID_INDEX						2

/*! \def RRM_CSPL_API_ID_INDEX
 */
#define RRM_CSPL_API_ID_INDEX                   5
/*! \def RRM_INTF_TRANS_ID_INDEX
 */
#define RRM_INTF_TRANS_ID_INDEX                 0
/*! \def RRM_INTF_MSG_ID_INDEX
 */
#define RRM_INTF_MSG_ID_INDEX                   8
#ifndef PNULL
/*! \def PNULL
 */
#define PNULL 									NULL
#endif

/*! \def RRM_APPS_MODULE_ID
 */
#define RRM_APPS_MODULE_ID						6
/*! \def UNKNOWN_MODULE_NAME
 */
#define UNKNOWN_MODULE_NAME						8
/*! \def RRM_MAX_MTU_SIZE
 */
#define RRM_MAX_MTU_SIZE               			64*1024
/*! \def MAX_RX_DATA_SIZE
 */
#define MAX_RX_DATA_SIZE               			65535
/*! \def IP_PORT_STR_MAX_LEN
 */
#define IP_PORT_STR_MAX_LEN						10
/*! \def NEXT_EXPIRY_TIMER
 */
#define NEXT_EXPIRY_TIMER  						500

/*! \def EXT_MSG_TRANSACTION_ID_OFFSET
 */
#define EXT_MSG_TRANSACTION_ID_OFFSET  			0
/*! \def EXT_MSG_SRC_MODULE_ID_OFFSET
 */
#define EXT_MSG_SRC_MODULE_ID_OFFSET  			2
/*! \def EXT_MSG_DEST_MODULE_ID_OFFSET
 */
#define EXT_MSG_DEST_MODULE_ID_OFFSET  			4
/*! \def EXT_MSG_API_OFFSET
 */
#define EXT_MSG_API_OFFSET          			6
/*! \def EXT_MSG_BUF_LEN_OFFSET
 */
#define EXT_MSG_BUF_LEN_OFFSET      			8

/*! \def EXT_CELL_INDEX_OFFSET
 */
#define EXT_CELL_INDEX_OFFSET                   10


/* STACKAPI Header Offsets */
/*! \def STACK_API_SRC_MODULE_ID_OFFSET
 */
#define STACK_API_SRC_MODULE_ID_OFFSET          1
/*! \def STACK_API_DEST_MODULE_ID_OFFSET
 */
#define STACK_API_DEST_MODULE_ID_OFFSET         3
/*! \def STACK_API_API_OFFSET
 */
#define STACK_API_API_OFFSET                    5
/*! \def STACK_API_PARAM_LEN_OFFSET
 */
#define STACK_API_PARAM_LEN_OFFSET              7
/*! \def STACK_API_PAYLOAD_LEN_OFFSET
 */
#define STACK_API_PAYLOAD_LEN_OFFSET            11

/*! \def RRM_API_PRIORITY
 */
#define RRM_API_PRIORITY            			0
/*! \def RRM_VERSION_ID
 */
#define RRM_VERSION_ID              			0x01

/* OAM Agent Module Id - 1 */
#ifndef RRM_OAM_MODULE_ID
/*! \def RRM_MIN_EXT_MODULE_ID
 */
#define RRM_MIN_EXT_MODULE_ID   				1
/*! \def RRM_OAM_MODULE_ID
 */
#define RRM_OAM_MODULE_ID       				(RRM_MIN_EXT_MODULE_ID + 0)
/*! \def RRM_MAX_EXT_MODULE_ID
 */
#define RRM_MAX_EXT_MODULE_ID   				 RRM_OAM_MODULE_ID
#endif
/* RRM Module Id = 2 */
/*! \def RRM_MODULE_ID
 */
#ifndef RRM_MODULE_ID
/*! \def RRM_MODULE_ID
 */
#define RRM_MODULE_ID							    0x2
#endif

#ifndef RRC_MODULE_ID
/*! \def RRC_MODULE_ID
 */
#define RRC_MODULE_ID							    0x3
#endif

/*! \def RRM_L2_MODULE_ID
 */
#define RRM_L2_MODULE_ID                            0x7

#ifndef OAM_IPR
/*PDCP module id*/
/*! \def PDCP_MODULE_ID
 */
#define PDCP_MODULE_ID                              0x5
/*MAC module id*/
/*! \def MAC_MODULE_ID
 */
#define MAC_MODULE_ID                               0x7
#endif

/*! \def RRM_ONE_THOUSAND_MS
 */
#define RRM_ONE_THOUSAND_MS					1000
/*PLATFORM module id*/
/*! \def RRM_PLATFORM_MODULE_ID
 */
#define RRM_PLATFORM_MODULE_ID                      0x74
/* SON Module Id = 100 */
/*! \def RRM_SON_MODULE_ID
 */
#define RRM_SON_MODULE_ID       				0x64
/*! \def RRM_SON_APPS_MODULE_ID
 */
#define RRM_SON_APPS_MODULE_ID                                  0x6466 
/*! \def RRM_SON_ANR_MODULE_ID
 */
#define RRM_SON_ANR_MODULE_ID       				0x6467
/*! \def RRM_SON_ES_MODULE_ID
 */
#define RRM_SON_ES_MODULE_ID        				0x6468
/*! \def RRM_SON_NMM_MODULE_ID
 */
#define RRM_SON_NMM_MODULE_ID       				0x6469

/*! \def RRM_SON_MLB_MODULE_ID
 */
#define RRM_SON_MLB_MODULE_ID                                   0x646B

/*! \def RRM_SON_MRO_MODULE_ID
 */
#define RRM_SON_MRO_MODULE_ID                                   0x646A

/* RACH_OPTIMIZATION_CHANGES_START */
#define RRM_SON_RACH_OPT_MODULE_ID                                  0x646C
/* RACH_OPTIMIZATION_CHANGES_END */

/* Internal Module Ids MIF-21, CellM-22, UeM-23*/
/*! \def RRM_MIN_INT_MODULE_ID
 */
#define RRM_MIN_INT_MODULE_ID   				 RRM_MAX_EXT_MODULE_ID + 20
/*! \def RRM_MIF_MODULE_ID
 */
#define RRM_MIF_MODULE_ID                       (RRM_MIN_INT_MODULE_ID + 0)
/*! \def RRM_CM_MODULE_ID
 */
#define RRM_CM_MODULE_ID                        (RRM_MIN_INT_MODULE_ID + 1)
/*! \def RRM_UEM_MODULE_ID
 */
#define RRM_UEM_MODULE_ID   					(RRM_MIN_INT_MODULE_ID + 2)
/*! \def RRM_LOGGER_MODULE_ID
 */
#define RRM_LOGGER_MODULE_ID   					(RRM_MIN_INT_MODULE_ID + 3)
/*! \def RRM_MAX_INT_MODULE_ID
 */
#define RRM_MAX_INT_MODULE_ID 					RRM_LOGGER_MODULE_ID
/*! \def RRM_FULL_INTERFACE_HEADERS_SIZE
 */
#define RRM_FULL_INTERFACE_HEADERS_SIZE \
        (RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE)
/*! \def RRM_BUFFER_SHOULD_BE_RELEASED
 */
#define RRM_BUFFER_SHOULD_BE_RELEASED 			7
/*! \def RRM_MAX_UTRAN_NBRS
 */
#define RRM_MAX_UTRAN_NBRS                      32
#define RRM_MAX_GERAN_NBRS                      32

#define RRM_MAX_CDMA_NBRS                       72
/* Start Bug Fix 786 */
#define RRM_MAX_CDMA_1X_RTT_NBRS                40
#define RRM_MAX_CDMA_HRPD_NBRS                  32
/* End Bug Fix 786 */
/*! \def RRM_UTRAN_CELL_IDENTITY_OCTETS
*/
/*+ Fix for array bounds +*/
#define RRM_UTRAN_CELL_IDENTITY_OCTETS                   2
/*- Fix for array bounds -*/
/*! \def RRM_MAX_RNC_ID
*/
#define RRM_MAX_RNC_ID                                   4095
/*Start: SPR 9289 */
#define RRM_MERGED_UTRAN_CELL_IDENTITY_OCTETS   4
/*End: SPR 9289 */

/*! \def MAX_UMTS_NCL_ROWS
 */
#define MAX_UMTS_NCL_ROWS 						32
/*! \def MAX_UMTS_NCL_COLS
 */
#define MAX_UMTS_NCL_COLS 						32
/*! \def MAX_EAID
 */
#define MAX_EAID								3
/*! \def MAX_QOS_CONFIG_PARAMS
 */
#define MAX_QOS_CONFIG_PARAMS					9
/*! \def MAX_NO_INTER_FREQ
 */
#define MAX_NO_INTER_FREQ 						8
/*! \def MAX_NO_EUTRAN_TO_UTRAN_FDD
 */
#define MAX_NO_EUTRAN_TO_UTRAN_FDD 				16
/*! \def MAX_NO_EUTRAN_TO_UTRAN_TDD
 */
#define MAX_NO_EUTRAN_TO_UTRAN_TDD              16
/*! \def MAX_NO_GERAN_FREQ_GROUUP
 */
#define MAX_NO_GERAN_FREQ_GROUUP 				16

/*! \def MAX_NO_CDMA_FREQ_GROUUP
 *  */
#define MAX_NO_CDMA_FREQ_GROUUP                 16


/*! \def MAX_NO_DRX_PROFILE
 */
/*bug_9464_changes_start*/
#define MAX_NO_DRX_PROFILE 						9 /* assuming DRX for nine profiles only */
                                                /*  for each QCI */
/*bug_9464_changes_end*/
/*! \def MAX_NO_SRB
 */
#define	MAX_NO_SRB 								2 /** Choice of default configuration**/
						   						/**	  or else parameters			    **/
/*! \def MAX_NO_INTER_FREQ_PARAM
 */
#define MAX_NO_INTER_FREQ_PARAM 				8
/*! \def MAX_MCC_DIGITS
 */
#define MAX_MCC_DIGITS 							3 /** also defined in rrm_cell_context.h    */
/*! \def MAX_MNC_DIGITS
 */
#define MAX_MNC_DIGITS 							3 /** also defined in rrm_cell_context.h    */
/*! \def MAX_CID_BYTES
 */
#define MAX_CID_BYTES 							16
/*! \def MAX_SUBFRAME_ALLOCATIONS
 */
#define MAX_SUBFRAME_ALLOCATIONS 				4
/*! \def MAX_MB_SFN_SUBFRAME_LIST
 */
#define MAX_MB_SFN_SUBFRAME_LIST 				8
/*! \def CELLM_RRMIM_BASE
 */
#define CELLM_RRMIM_BASE 						0

/*!< MACRO for filling functions for UEM */
/*! \def MAC_UE_PRIORITY
 */
#define MAC_UE_PRIORITY 						0
/*! \def CODE_BOOK_INDEX
 */
#define CODE_BOOK_INDEX 					    0 	
/*! \def MAX_NUM_SNR_VALUES
 */
/*! \def LOGICAL_CH_GRP_SRB1_DEFAULT_VAL
 */
#define LOGICAL_CH_GRP_SRB1_DEFAULT_VAL 		0
/*! \def PRIORITY_SRB1_DEFAULT_VAL
 */
#define PRIORITY_SRB1_DEFAULT_VAL 				1
/*! \def SRB1_PRIORITY
 */
#define SRB1_PRIORITY 							1
/*! \def LOGICAL_CH_GRP_SRB2_DEFAULT_VAL
 */
#define LOGICAL_CH_GRP_SRB2_DEFAULT_VAL 		0
/*! \def PRIORITY_SRB2_DEFAULT_VAL
 */
#define PRIORITY_SRB2_DEFAULT_VAL 				3
/*! \def SRB2_PRIORITY
 */
#define SRB2_PRIORITY 							1

/*! \def NON_GBR
 */
#define RRM_NON_GBR                             0
/*! \def GBR
 */
#define RRM_GBR                                     1



/*! \def NO_DRX_PROFILE
 */
#define NO_DRX_PROFILE                          255
/*! \def MAX_LC_ID_INDEX
 */
#define MAX_LC_ID_INDEX                         8
/*! \def RRM_MAX_DEFAULT_LCID_VALUES            
 */
#define RRM_MAX_DEFAULT_LCID_VALUES             {3,4,5,6,7,8,9,10}

/*OPTION3X Changes Start*/
/*! \def RRM_MAX_DEFAULT_DRB_ID_VALUES            
 */
#define RRM_MAX_DEFAULT_DRB_ID_VALUES           {3,4,5,6,7,8,9,10}
//{3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32}

#define RRM_MAX_DRB_ID_INDEX                    8
/*OPTION3X Changes Stop*/

/*! \def MAX_DRB_ID_INDEX
 */
#define MAX_DRB_ID_INDEX                        32
/*! \def SAP_FLAGS
 */
#define SAP_FLAGS                               15
/*! \def MAX_OUTPUT_POWER
 */
#define MAX_OUTPUT_POWER                        23
/*! \def QOS_PROFILE_DATA_SIZE
 */
#define QOS_PROFILE_DATA_SIZE                   10
/*! \def NUM_TPC_ID_DCI_FRMT_3A
 */
//#define NUM_TPC_ID_DCI_FRMT_3A                  31
/*! \def NUM_TPC_ID_DCI_FRMT_3
 */
//#define NUM_TPC_ID_DCI_FRMT_3                       15
/*! \def AM_MODE
 */
#define AM_MODE                                 1
/*! \def UM_MODE
 */
#define UM_MODE                                 2
/*!< MACRO releated to configuration file
 *   reading functions
 */
/*! \def MAX_LABEL
 */
#define MAX_LABEL 								10
/*! \def IPADDR_PORT_MAX_LEN
 */
#define IPADDR_PORT_MAX_LEN 					300
/*! \def RRM_CONFIG_FILE
 */
#define RRM_CONFIG_FILE 						"../cfg/eNodeB_Configuration.cfg"
/*! \def MAX_KEY_SIZE
 */
#define MAX_KEY_SIZE  							128
/*! \def MAX_BUFF_SIZE
 */
#define MAX_BUFF_SIZE 							256
/*! \def RRM_PORT_INDEX
 */
#define RRM_PORT_INDEX 							0
/*! \def RRC_PORT_INDEX
 */
#define RRC_PORT_INDEX 							1
/*! \def OAM_PORT_INDEX
 */
#define OAM_PORT_INDEX 							2
/*! \def SON_PORT_INDEX
 */
#define SON_PORT_INDEX 							3
/*SPR 20856 Start*/
/*! \def MAC_PORT_INDEX
 */
#define MAC_PORT_START_INDEX						4
/*! \def PDCP_PORT_INDEX
 */
#define PDCP_PORT_START_INDEX 						5
/*! \def MAX_IP_ADD_LEN_STR
 */
/*SPR 20856 End*/
#define MAX_IP_ADD_LEN_STR                                              16
/*! \def RRM_254
 */
#define RRM_254                                 254
/*! \def RRM_255
 */
#define RRM_255                                 255
/*! \def RRM_256
 */
#define RRM_256                                 256
/*! \def INDEX_0
 */
#define INDEX_0                                 0
/*! \def RRM_INVALID_RETURN
*/
#define RRM_INVALID_RETURN			0xff
/*! \def RRM_TX_MODE_TABLE_SIZE
*/
#define RRM_TX_MODE_TABLE_SIZE		  	7	
/*spr 22474 start*/
#define NVARPOOL    27
#define MINBUF      8
#define NVARMEMPOOL    27

/* This is used in debug GET Command */
typedef enum rrm_GetDebugTypeE
{
    RRM_GET_DEBUG_INFO_MEM_STATS, /* Mem Pool Statistics */
}rrm_DebugType;
typedef enum      
{ 
    RRM_GET_MEM_POOL_STATS=0,
}rrm_oam_debug_info_req_et;
typedef enum
{
    RRM_WRONG_DEBUG_TYPE
}rrm_oam_debug_info_fail_cause_et;

/*spr 22474 end*/
/* BUG_11317_FIX_START */
/* Uplink power control starts */
#define RRM_OLPC_CATEGORY_TABLE_SIZE		4
/* Uplink power control end */
/* BUG_11317_FIX_END */

#define RRM_MAX_PSC   511

/* eMTC changes start */
/* 
** Number of REs per RB Pair = 
**         84 RE per RB * 2 Slot = 168 RE
*/
#define NUM_RE_PER_RB_PAIR     168  
/* eMTC changes stop */

/* SPR_11104_FIX_START */
/* FGI Feature Start */
#define RRM_ONE            1
#define RRM_TWO            2
#define RRM_THREE          3
#define RRM_FOUR           4
#define RRM_FIVE           5
#define RRM_SIX            6
#define RRM_SEVEN          7
#define RRM_EIGHT          8
#define RRM_NINE           9
#define RRM_TEN            10
#define RRM_ELEVEN         11
#define RRM_TWELVE         12
#define RRM_THIRTEEN       13
#define RRM_FOURTEEN       14
#define RRM_FIFTEEN        15
#define RRM_SIXTEEN        16
/* SPR 16406 8SEP start */
#define RRM_SEVENTEEN      17
#define RRM_EIGHTEEN       18
#define RRM_NINETEEN            19
/* SPR 16406 8SEP end */
#define RRM_TWENTY         20
#define RRM_TWENTY_ONE          21
#define RRM_TWENTY_TWO     22
#define RRM_TWENTY_THREE   23
#define RRM_TWENTY_FOUR    24
#define RRM_TWENTY_FIVE    25
#define RRM_TWENTY_SIX     26
#define RRM_TWENTY_SEVEN   27
#define RRM_TWENTY_EIGHT        28
#define RRM_TWENTY_NINE         29
#define RRM_THIRTY              30
/* SPR 17728 Fix Start */
#define RRM_THIRTY_ONE          31
/* SPR 17728 Fix End */
#define RRM_THIRTY_TWO     32
#define RRM_THIRTY_THREE        33
#define RRM_THIRTY_FOUR         34
#define RRM_THIRTY_FIVE         35
#define RRM_THIRTY_SIX          36
#define RRM_THIRTY_SEVEN        37
#define RRM_THIRTY_EIGHT        38
#define RRM_THIRTY_NINE         39
#define RRM_FORTY               40
#define RRM_FORTY_ONE           41
#define RRM_FORTY_TWO           42
#define RRM_FORTY_THREE         43
#define RRM_FORTY_FOUR          44
#define RRM_FORTY_FIVE          45
#define RRM_FORTY_SIX           46
#define RRM_FORTY_SEVEN         47
#define RRM_FORTY_EIGHT         48
#define RRM_FORTY_NINE          49
#define RRM_FIFTY               50
#define RRM_FIFTY_ONE           51
#define RRM_FIFTY_TWO           52
#define RRM_FIFTY_THREE         53
#define RRM_FIFTY_FOUR          54
#define RRM_FIFTY_FIVE          55
#define RRM_FIFTY_SIX           56
#define RRM_FIFTY_SEVEN         57
#define RRM_FIFTY_EIGHT         58
#define RRM_FIFTY_NINE          59
#define RRM_SIXTY               60
#define RRM_SIXTY_ONE           61
#define RRM_SIXTY_TWO           62
#define RRM_SIXTY_THREE         63
#define RRM_SIXTY_FOUR          64
#define RRM_SIXTY_FIVE          65
#define RRM_SIXTY_SIX           66
#define RRM_SIXTY_SEVEN         67
#define RRM_SIXTY_EIGHT         68
#define RRM_SIXTY_NINE          69
#define RRM_SEVENTY             70
#define RRM_SEVENTY_ONE         71
#define RRM_SEVENTY_TWO         72
#define RRM_SEVENTY_THREE       73
#define RRM_SEVENTY_FOUR        74
#define RRM_SEVENTY_FIVE        75
#define RRM_SEVENTY_SIX         76
#define RRM_SEVENTY_SEVEN       77
#define RRM_SEVENTY_EIGHT       78
#define RRM_SEVENTY_NINE        79
#define RRM_EIGHTY              80
#define RRM_EIGHTY_ONE          81
#define RRM_EIGHTY_TWO          82
#define RRM_EIGHTY_THREE        83
#define RRM_EIGHTY_FOUR         84
#define RRM_EIGHTY_FIVE         85
#define RRM_EIGHTY_SIX          86
#define RRM_EIGHTY_SEVEN        87
#define RRM_EIGHTY_EIGHT        88
#define RRM_EIGHTY_NINE         89
#define RRM_NINETY              90
#define RRM_NINETY_ONE          91
#define RRM_NINETY_TWO          92
#define RRM_NINETY_THREE        93
#define RRM_NINETY_FOUR         94
#define RRM_NINETY_FIVE         95
#define RRM_NINETY_SIX          96
#define RRM_NINETY_SEVEN        97
#define RRM_NINETY_EIGHT        98
#define RRM_NINETY_NINE         99
#define RRM_HUNDRED        100
#define RRM_HUNDRED_ONE                 101
#define RRM_HUNDRED_TWO                 102
#define RRM_HUNDRED_THREE               103
#define RRM_HUNDRED_FOUR                104
#define RRM_HUNDRED_FIVE                105
#define RRM_HUNDRED_SIX                 106
#define RRM_HUNDRED_SEVEN               107
#define RRM_HUNDRED_EIGHT               108
#define RRM_HUNDRED_NINE                109
#define RRM_HUNDRED_TEN                 110
#define RRM_HUNDRED_ELEVEN              111
#define RRM_HUNDRED_TWELVE              112
#define RRM_HUNDRED_THIRTEEN            113
#define RRM_HUNDRED_FOURTEEN            114
#define RRM_HUNDRED_FIFTEEN             115
#define RRM_HUNDRED_SIXTEEN             116
#define RRM_HUNDRED_SEVENTEEN           117
#define RRM_HUNDRED_EIGHTEEN            118
#define RRM_HUNDRED_NINETEEN            119
#define RRM_HUNDRED_TWENTY              120
#define RRM_HUNDRED_TWENTY_ONE          121
#define RRM_HUNDRED_TWENTY_TWO          122
#define RRM_HUNDRED_TWENTY_THREE        123
#define RRM_HUNDRED_TWENTY_FOUR         124
#define RRM_HUNDRED_TWENTY_FIVE         125
#define RRM_HUNDRED_TWENTY_SIX          126
#define RRM_HUNDRED_TWENTY_SEVEN        127
#define RRM_HUNDRED_TWENTY_EIGHT        128
#define RRM_HUNDRED_TWENTY_NINE         129
#define RRM_HUNDRED_THIRTY              130
#define RRM_HUNDRED_THIRTY_ONE          131
#define RRM_HUNDRED_THIRTY_TWO          132
#ifdef ENDC_ENABLED
#define RRM_THREE_HUNDRED               300
#define RRM_SIX_HUNDRED                 600
#define RRM_ONE_FIFTY                   150
#endif
/* SPR_11104_FIX_END */
/* SPR-16447 START */
#define RRM_ARFCN_SPACING 8
#define RRM_FARFCN1024    1024
/* SPR-16447 END */

/* Bug_944_start */
#define RRM_DEFAULT_CAPACITY_THRESH          50 
/* Bug_944_end */
/*DYNAMIC SIB SCHEDULING END*/
#define RRM_TWO_FIFTY_FIVE 255
/* FGI Feature End */

/*! \def  RRM_EVENT_HANDLER_MODULE_ID
*/
#define  RRM_EVENT_HANDLER_MODULE_ID             11 
/*! \def EVENT_HANDLER_PORT_INDEX
*/
#define  EVENT_HANDLER_PORT_INDEX                5
/*! \def RRM_EVENT_NOTIFICATION_HEADER_LEN
*/
#define  RRM_EVENT_NOTIFICATION_HEADER_LEN       10

/* RRM_MAC_RECONF_SCHEDULER_CHANGES_START */
/*! \def RRM_MAX_CQI_VAL
*/ 
#define RRM_MAX_CQI_VAL                          15    
/*! \def RRM_NUM_DCI_SIZE_CATEGORY
*/
/*! \def RRM_NUM_DCI_SIZE_CATEGORY
*/                       
#define RRM_NUM_DCI_SIZE_CATEGORY                     3
/*! \def RRM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT
*/
#define RRM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT  4
/*! \def RRM_MAX_NUM_TPC_FOR_PUCCH
*/
#define RRM_MAX_NUM_TPC_FOR_PUCCH             4
/*! \def RRM_MAX_NUM_TPC_FOR_PUSCH
*/
#define RRM_MAX_NUM_TPC_FOR_PUSCH             4
/*! \def RRM_MAX_NUM_ALPHA_COUNT
*/
#define RRM_MAX_NUM_ALPHA_COUNT               8
/*! \def RRM_MAX_NUM_PATHLOSS_TO_SINR_COUNT
*/
#define RRM_MAX_NUM_PATHLOSS_TO_SINR_COUNT   10
/* RRM_MAC_RECONF_SCHEDULER_CHANGES_ENDS */

/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
#define MAX_SSB_IDX_R15        32
typedef U16 phy_scell_id_nr_r15_t;
typedef U8  rsrp_result_nr_r15_t;
typedef U8  rsrq_result_nr_r15_t;
typedef U8  rs_sinr_result_r15_t;
typedef U8  rs_index_nr_r15_t;
typedef struct _endc_eligible_erab_list_t
{
  U8 drb_index;
  U8 drb_type;  /*rrm_drb_type_et*/
}endc_eligible_erab_list_t;
typedef enum {
    DRB_TYPE_SN_TERMINATED_SCG = 1,
    DRB_TYPE_SN_TERMINATED_SPLIT,
    DRB_TYPE_INVALID
}rrm_drb_type_et;
#endif
/* ENDC_MEAS_CHANGES_END */

typedef U8 rrm_cell_index_t;
typedef U16 rrm_ue_index_t;
/*SPR 22151 Fix Start*/
typedef U32 rrm_prb_t;
/*SPR 22151 Fix End*/

/*Local Event Handling Feature Start*/
/*! \def RRM_INVALID_CORE_ID
*/
#define RRM_INVALID_CORE_ID   0xff
/* CSR_00040799_CHANGES_START */
/*! \def RRM_INVALID_UE_ID
*/
#define RRM_INVALID_UE_ID     0xffff
/* CSR_00040799_CHANGES_END */
/*! \def RRM_INVALID_CELL_ID
*/
#define RRM_INVALID_CELL_ID   0xff
/*! \def RRM_INVALID_MOM_ID
*/
#define RRM_INVALID_MOM_ID    0xff
/*Local Event Handling Feature End*/
/*start supported_rat*/
/*! \def RRM_EUTRAN_RAT_SUPPORTED
*/
#define RRM_EUTRAN_RAT_SUPPORTED 0x01
/*! \def RRM_UTRAN_RAT_SUPPORTED
*/
#define RRM_UTRAN_RAT_SUPPORTED 0x02
/*! \def RRM_GERAN_RAT_SUPPORTED
*/
#define RRM_GERAN_RAT_SUPPORTED 0x04
/*! \def RRM_CDMA_1xRTT_RAT_SUPPORTED
*/
#define RRM_CDMA_1xRTT_RAT_SUPPORTED 0x08
/*! \def RRM_CDMA_HPRD_RAT_SUPPORTED
*/
#define RRM_CDMA_HPRD_RAT_SUPPORTED 0x10
/*! \def  RRM_GET_MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
*/
#define RRM_GET_MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define RRM_GET_MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
/*end supported_rat*/
/*start tgt cell ranking*/
#define RRM_LOWEST_RANK 0
/*end tgt cell ranking*/
#define MAX_BUFFER      	100
/*RIM changes start*/
#define RRM_MAX_RSN                   4294967296ULL /* 2^32   Maximum RSN allowed*/
#define RRM_MAX_RSN_BY_2              2147483648ULL /* 2^31   Maximum RSN allowed BY 2*/
#define RRM_INCREMENT_RSN(s) (((s + 1)) % (RRM_MAX_RSN))
/*RIM changes end*/
/*CCO changes start*/
/* SPR_14067_Fix_Start */
#define RRM_NCC_MASK   0x038
#define RRM_BSCC_MASK  0x007
/*CCO changes end*/
/*CCO changes start*/
//#define RRM_NCC_MASK   0x700
//#define RRM_BSCC_MASK  0x007
/* SPR_14067_Fix_End */
#define RRM_MAX_PSC   511
/*CCO changes end*/
/* Start fix for bug 715 */
#define RRM_MAX_PCI_UTRAN_TDD   127
/* End fix for bug 715 */
/* SPR 18641_18843 fix start */
/* Code Removed */
/* SPR 18641_18843 fix end */


/* UE Positioning */
#define MAX_NUM_ANTENNA   4
/* UE Positioning */

/* Carrier_Aggregation_Start */
/*SPR 19705 +*/
#define RRM_SCELL_DEACT_TIMER_RF2_M_SEC               20
#define RRM_SCELL_DEACT_TIMER_RF4_M_SEC               40
#define RRM_SCELL_DEACT_TIMER_RF8_M_SEC               80
#define RRM_SCELL_DEACT_TIMER_RF16_M_SEC              160
#define RRM_SCELL_DEACT_TIMER_RF32_M_SEC              320
#define RRM_SCELL_DEACT_TIMER_RF64_M_SEC              640
#define RRM_SCELL_DEACT_TIMER_RF128_M_SEC             1280
/*SPR 19705 -*/
#define RRM_IS_CA_ELIGIBLE_DEFAULT_VALUE        RRM_FALSE
#define RRM_IS_MASA_DEFAULT_VALUE               RRM_FALSE
#define RRM_NUM_OF_REPORT_DEFAULT_VALUE         3
#define RRM_CA_APPL_BITRATE_UL_DL_DEFAULT_VALUE 0xC00000000ULL
//#define RRM_MAX_SCELL                           4
/* Carrier_Aggregation_End */

/** eICIC_PHASE_1_2_CHANGES_START */
#define RRM_EICIC_RSRP_DEFAULT_VALUE                        50
#define RRM_EICIC_RSRQ_DEFAULT_VALUE                        20
#define RRM_DL_SINR_THERSHOLD_DEFAULT_LOW_VALUE             50
#define RRM_DL_SINR_THERSHOLD_DEFAULT_HIGH_VALUE            90
#define RRM_NUM_ABS_REPORT_DEFAULT_VALUE                    12
#define RRM_ABS_USAGE_LOW_THRESHOLD_DEFAULT_VALUE           50
#define RRM_ABS_USAGE_HIGH_THRESHOLD_DEFAULT_VALUE          90
#define RRM_AGGRESSOR_SEL_TIMER_DEFAULT_VALUE               60
#define RRM_LOAD_INFORMATION_GUARD_TIMER_DEFAULT_VALUE      10 
#define RRM_LOAD_INFORMATION_COLLATION_TIMER_DEFAULT_VALUE  10
#define RRM_EICIC_ELIGIBLE_UE_DEFAULT_VALUE                 70
/** eICIC_PHASE_1_2_CHANGES_END */


/* Macros Added for default Values for parameters which are not coming from OAM */

#define RRM_DEFINED_DEFAULT_VALUE_FOR_QOS_PROFILE_DATA_SIZE                  9
#define RRM_DEFINED_DEFAULT_VALUE_FOR_QOS_PROFILE_DATA                       1

#define RRM_DEFINED_DEFAULT_VALUE_FOR_PUCCH_N_CS_AN                          0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_TDD_ACK_NACK_FEEDBACK_MODE             0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_64_QAM                          1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_GROUP_ASGMNT_PUSCH                     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_REF_SIG_PUSCH_CYCLIC_SHIFT          0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DELTA_PREAMBLE_MSG_3                   4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SIMULTANEOUS_ACK_NACK_CQI              0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MODIFICATION_PERIOD_COEFF              1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RS_EPRE                            56
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PUCCH_CQI_SNR_VALUE                    0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_SYNC_LOSS_TIMER                     RRM_UL_SYNC_LOSS_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_N_GAP                                  RRM_NGAP_1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_MCS                                 19
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_MCS                                 28
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_NUM_UE_PER_CELL                    64
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PRB_BUDGET_GBR                      50
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_PRB_BUDGET_GBR                      50
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PRB_BUDGET_NGBR                     50
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_PRB_BUDGET_NGBR                     50
#define RRM_DEFINED_DEFAULT_VALUE_FOR_HARQ_BLER_CLPC_PUCCH_ENABLE            0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_CQI_SINR_CLPC_PUCCH_ENABLE             1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_CLPC_PUSCH_ENABLE                      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_CLPC_PUSCH_FREQ_SEL_ENABLE             1

#define RRM_DEFINED_DEFAULT_VALUE_FOR_TPC_PDCCH_CONFIG_PUCCH_ENABLE             1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_TPC_PDCCH_CONFIG_PUSCH_ENABLE             1


#define RRM_DEFINED_DEFAULT_VALUE_FOR_BLIND_HO_TIMER_DURATION                0
  /* SPR 17784 Start */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_CGI_TIMER_DURATION                     0
  /* SPR 17784 End */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RNTP_THRESHOLD                         0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_Q_QUAL_MIN_R9                          0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_Q_QUAL_MIN_OFFSET_R9_PRESENT           1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_TIMER_REORDERING             0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SI_WINDOW_LEN                          5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SI_SCHEDULING_COUNT                    1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SI_PERIODICITY                         2
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PER_SI_SIB_MAPPING_COUNT               0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_START_TPC_RNTI_PUCCH                   351
#define RRM_DEFINED_DEFAULT_VALUE_FOR_END_TPC_RNTI_PUCCH                     355
#define RRM_DEFINED_DEFAULT_VALUE_FOR_START_TPC_RNTI_PUSCH                   356
#define RRM_DEFINED_DEFAULT_VALUE_FOR_END_TPC_RNTI_PUSCH                     360
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCCH_POW_OR_AGG_ENABLE                0

/* 36.331 - Sec 9.2.2 */
/* 36.331 - Sec 9.2.2 */
/* 36.331 - Sec 9.2.1 */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SRB_0_PRIORITY                         1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SRB_1_PRIORITY                         3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SRB_PRIORITIZED_BIT_RATE               RRMC_BIT_RATE_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SRB_BUCKET_SIZE_DURATION               RRMC_BUCKET_SIZE_MS_300
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SRB_LOGICAL_CHANNEL_GROUP              0
/* 36.331 - Sec 9.2.1 */
/* Load congestion parameters' default values */
/* SPR 17844 fix start */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SOFT_LMT                               0
/* SPR 17844 fix end */
/* High action applied limit in % */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_HIGH_ACTN_APPLIED_LMT                  10
/* Period (in Second ) at which cell starts processing load action */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LD_PROCESS_TIMER                       60
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_PERCENTAGE_OVER                   70
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_PERCENTAGE_HIGH                   60
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_PERCENTAGE_MID                    50
/*Bug:9947 start*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_ACTION_OVER                       40
/*Bug:9947 end*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_ACTION_HIGH                       2
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_ACTION_MID                        1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOAD_NUM_USER                          1
/* Load congestion parameters' default values */
/* ECID default values */
#define RRM_DRX_SF10_IN_MSECS         10
#define RRM_DRX_SF20_IN_MSECS         20
#define RRM_DRX_SF32_IN_MSECS         32
#define RRM_DRX_SF40_IN_MSECS         40
#define RRM_DRX_SF64_IN_MSECS         64
#define RRM_DRX_SF80_IN_MSECS         80
#define RRM_DRX_SF128_IN_MSECS        128
#define RRM_DRX_SF160_IN_MSECS        160
#define RRM_DRX_SF256_IN_MSECS        256
#define RRM_DRX_SF320_IN_MSECS        320
#define RRM_DRX_SF512_IN_MSECS        512
#define RRM_DRX_SF640_IN_MSECS        640
#define RRM_DRX_SF1024_IN_MSECS       1024
#define RRM_DRX_SF1280_IN_MSECS       1280
#define RRM_DRX_SF2048_IN_MSECS       2048
#define RRM_DRX_SF2560_IN_MSECS       2560
#define RRM_DRX_SF5120_IN_MSECS       5120
#define RRM_DRX_SF10240_IN_MSECS      10240

#define RRM_MIN_UE_RX_TX_MEASURE_TIME 200
#define RRM_MAX_DRX_CYCLE             5
/* B1 and B2 CDMA2000 threshold default values */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_B1_THRESHOLD_CDMA2000                  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_B2_THRESHOLD_CDMA2000                  1
/* B1 and B2 CDMA2000 threshold default values */
/* Path Loss Threshold default value */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PL_THRESHOLD                           70 

/* CHANGES_FRM_XML_TO_CFG START */

#define QCI_VAL_1               1
#define QCI_VAL_2               2
#define QCI_VAL_3               3
#define QCI_VAL_4               4
#define QCI_VAL_5               5
#define QCI_VAL_6               6
#define QCI_VAL_7               7
#define QCI_VAL_8               8
#define QCI_VAL_9               9

#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_NUM_HARQ_PROCESS                8
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DCI_FORMAT_SI_MSG                  RRM_OAM_RBS_PER_TBS_DCI_1A
#define RRM_DEFINED_DEFAULT_VALUE_FOR_NUM_RBS_PER_TBS_DCI_1A             4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_NUM_RBS_PER_TBS_DCI_1C             4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_UE_ADMSN              4 
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TMR_UE_ADMSN           0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_UE_ADMSN               RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_UE_ADMSN        RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_UE_ADMSN        4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_UE_ADMSN        4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_UE_ADMSN        1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_UE_ADMSN            RRC_RETX_BSR_TIMER_SF_2560

/* IN-MEMORY LOGGING DEFAULT VALUES */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_WAIT_INTERVAL    2
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_PATH             "."
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_SHM_SIZE         5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_FILE_SIZE        10
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_CHUNK_CNT        10
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_CPU_BITMAP       1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOG_FILE_CNT        255

/* RRM MACROS FOR QCI=1 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_1           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_1      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_1         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_1               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_1     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_1       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_1    1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_1   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_1     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_1           RRM_TRUE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_1       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_1    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_1     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_1            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_1       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_1       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_1       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_1       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_1       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_1       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_1       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_1       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_1       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_1               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_1           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_1    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_1     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_1     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_1     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_1         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_1            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_1               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_1      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_1      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_1  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_1  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_1  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_1         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_1           16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_1     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_1     0
/* RRM MACROS FOR QCI=1 END */

/* RRM MACROS FOR QCI=2 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_2           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_2      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_2         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_2               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_2     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_2       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_2    2
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_2   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_2     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_2           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_2       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_2    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_2     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_2            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_2       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_2       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_2       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_2       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_2       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_2       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_2       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_2       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_2       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_2               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_2           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_2    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_2     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_2     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_2     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_2         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_2            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_2               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_2      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_2      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_2  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_2  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_2  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_2         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_2           16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_2     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_2     0
/* RRM MACROS FOR QCI=2 END */

/* RRM MACROS FOR QCI=3 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_3           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_3      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_3         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_3               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_3     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_3       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_3    2
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_3   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_3     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_3           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_3       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_3    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_3     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_3            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_3       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_3       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_3       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_3       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_3       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_3       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_3       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_3       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_3       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_3               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_3           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_3    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_3     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_3     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_3     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_3         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_3            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_3               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_3      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_3      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_3  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_3  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_3  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_3         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_3           16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_3     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_3     0
/* RRM MACROS FOR QCI=3 END */

/* RRM MACROS FOR QCI=4 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_4           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_4      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_4         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_4               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_4     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_4       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_4    2
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_4   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_4     7 
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_4           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_4       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_4    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_4     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_4            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_4       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_4       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_4       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_4       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_4       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_4       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_4       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_4       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_4       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_4               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_4           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_4    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_4     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_4     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_4     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_4         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_4            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_4               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_4      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_4      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_4  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_4  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_4  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_4         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_4           16 
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_4     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_4     0
/* RRM MACROS FOR QCI=4 END */

/* RRM MACROS FOR QCI=5 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_5           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_5      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_5         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_5               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_5     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_5       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_5    3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_5   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_5     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_5           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_5       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_5    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_5     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_5            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_5       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_5       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_5       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_5       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_5       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_5       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_5       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_5       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_5       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_5               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_5           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_5    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_5     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_5     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_5     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_5         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_5            1 
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_5               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_5      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_5      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_5  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_5  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_5  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_5         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_5           16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_5     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_5     0
/* RRM MACROS FOR QCI=5 END */

/* RRM MACROS FOR QCI=6 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_6           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_6      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_6         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_6               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_6     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_6       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_6    3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_6   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_6     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_6           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_6       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_6    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_6     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_6            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_6       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_6       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_6       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_6       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_6       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_6       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_6       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_6       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_6       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_6               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_6           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_6    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_6     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_6     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_6     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_6         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_6            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_6               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_6      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_6      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_6  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_6  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_6  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_6         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_6           16 
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_6     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_6     0
/* RRM MACROS FOR QCI=6 END */

/* RRM MACROS FOR QCI=7 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_7           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_7      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_7         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_7               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_7     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_7       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_7    3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_7   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_7     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_7           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_7       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_7    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_7     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_7            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_7       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_7       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_7       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_7       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_7       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_7       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_7       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_7       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_7       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_7               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_7           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_7    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_7     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_7     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_7     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_7         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_7            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_7               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_7      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_7      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_7  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_7  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_7  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_7         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_7           16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_7     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_7     0
/* RRM MACROS FOR QCI=7 END */

/* RRM MACROS FOR QCI=8 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_8           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_8      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_8         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_8               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_8     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_8       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_8    3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_8   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_8     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_8           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_8       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_8    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_8     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_8            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_8       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_8       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_8       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_8       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_8       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_8       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_8       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_8       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_8       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_8               13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_8           RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_8    RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_8     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_8     4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_8     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_8         RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_8            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_8               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_8      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_8      1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_8  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_8  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_8  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_8         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_8           16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_8     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_8     0
/* RRM MACROS FOR QCI=8 END */

/* RRM MACROS FOR QCI=9 START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MAX_TRANS_QCI_9           4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PROHIBIT_TIMER_QCI_9      0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_PERIODICITY_QCI_9         RRM_OAM_SR_PERIODICITY_2_MS
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITY_QCI_9               1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_BUCKET_SZ_DURATION_QCI_9     3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_REORDERNING_REQD_QCI_9       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_LOGICAL_CHANL_GROUP_QCI_9    3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PRIORITIZED_BIT_RATE_QCI_9   1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PDCP_DISCARD_TIMER_QCI_9     7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SR_MASK_FLAG_QCI_9           RRM_FALSE
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_RETRANS_QCI_9       6
/* SPR_10949_CHANGES_START */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_UM_PDCP_SN_SIZE_QCI_9    0
/* SPR_10949_CHANGES_END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DATA_TRANSFER_MODE_QCI_9     1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ENABLE_ROHC_QCI_9            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0001_QCI_9       0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0002_QCI_9       0  
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0003_QCI_9       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0004_QCI_9       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0006_QCI_9       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0101_QCI_9       0                   
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0102_QCI_9       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0103_QCI_9       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_ROHC_PROF_0x0104_QCI_9       0                    
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_CID_QCI_9                 13
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_HARQ_TX_QCI_9                RRC_MAX_HARQ_TX_N_5
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_BSR_TIMER_QCI_9         RRC_PERIODIC_BSR_TIMER_INFINITY
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PERIODIC_PHR_TIMER_QCI_9         4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_PROHIBIT_PHR_TIMER_QCI_9         4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_PATHLOSS_CHANGE_QCI_9         1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RETX_BSR_TIMER_QCI_9             RRC_RETX_BSR_TIMER_SF_2560
/* FOR AM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_AM_QCI_9            1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_POLL_BYTE_AM_QCI_9               4
#define RRM_DEFINED_DEFAULT_VALUE_FOR_MAX_RETX_THRESHOLD_AM_QCI_9      3
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_STATUS_RPT_REQ_AM_QCI_9      1 
#define RRM_DEFINED_DEFAULT_VALUE_FOR_RLC_ENB_STATUS_RPT_REQ_AM_QCI_9  1
#define RRM_DEFINED_DEFAULT_VALUE_FOR_DL_AM_RLC_STATUS_PROHIBIT_QCI_9  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_RETRANSMIT_QCI_9  0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_UL_AM_RLC_POLL_PDU_QCI_9         0
/* FOR UM MODE*/
#define RRM_DEFINED_DEFAULT_VALUE_FOR_T_REORDERING_UM_QCI_9            16
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_UL_RLC_QCI_9     0
#define RRM_DEFINED_DEFAULT_VALUE_FOR_SN_FIELD_LEN_UM_DL_RLC_QCI_9     0
/* RRM MACROS FOR QCI=9 END */
#define RRM_DEFINED_DEFAULT_VALUE_FOR_HEALTH_TIMER                     5

/* CHANGES_FRM_XML_TO_CFG END*/
/*Start:Bug 762*/
#define RRM_FGI_BYTES                                                  17
/*End:Bug 762*/
/*Bug_776_changes_start*/
#define RRM_MAX_NUM_RAB_IDS  16
/*Bug_776_changes_end*/

/* SPR-10397-fix start */
/* Code Deleted */
/* SPR-10397-fix end */

/* Carrier Aggregation start */
#define RRM_MAX_SCELL  7
#define RRM_DEFINED_DEFAULT_VALUE_FOR_CELL_LOAD_MULTICASTING_TIMER  20 
/* Carrier Aggregation end */

/*  LAA Rel13 Code + */
/*  LAA Rel13 Code - */
#define INVALID_FRAME 0xFF
#define INVALID_SLOT  0xFF
/* warnings removal: macro INVALID_RI_INDEX is 
   defined to initialize 'ri_index' and INVALID_RES_INDEX 
   is changed from 0xFFFF to 0xFF, 10/08/2012 */
#define INVALID_RI_INDEX 0xFFFF
#define INVALID_RES_INDEX 0xFF
#define INVALID_SR_CFG_INDEX 0xFF
#define INVALID_CQI_CFG_INDEX 0xFFFF


#define MIN_NUM_MNC_DIGIT  2
#define MAX_NUM_MNC_DIGIT  3
/*CA STAGE 2*/
#define MAX_CQI_INDEX 16
#define RRM_CFG_FILE_NAME_LEN 20
/* SPR_14230_start */
#define RRM_DELTA_MCS_ENABLED RRM_ZERO 
#define RRM_ACCUMULATION_ENABLED RRM_ONE
#define RRM_MIN_RB_FOR_PL_PHR_CALC RRM_ONE
/* SPR_14230_stop */

/* SPR_20653_start */
#ifndef LOGGING_ENHANCEMENT_CELL_AND_UE_INDEX_STRUCT
#define LOGGING_ENHANCEMENT_CELL_AND_UE_INDEX_STRUCT
typedef struct _cell_and_ue_index
{
    U8  cell_index;
    U16 ue_index;
} cell_and_ue_index;
/* SPR_20653_stop */

#endif


/*eMTC changes start*/
#define RRM_OAM_DEFAULT_MOD_PERIOD_COEFF_BR                1
#define RRM_OAM_DEFAULT_START_SF_CSS_FDD                   0
#define RRM_OAM_DEFAULT_PRACH_STARTING_SF                  0 
#define RRM_OAM_DEFAULT_MAX_NUM_PREAMBLE_ATTEMPT_CE        0
#define RRM_OAM_DEFAULT_NPUCCH_NUM_REP_CE_MSG4_LEVEL0      0
#define RRM_OAM_DEFAULT_NPUCCH_NUM_REP_CE_MSG4_LEVEL1      0
#define RRM_OAM_DEFAULT_PUCCH_NUM_REP_CE_FORMAT1           0
#define RRM_OAM_DEFAULT_FDD_UPLINK_SF_BITMAP_BR           0xFFC0
#define RRM_OAM_DEFAULT_SF_PATTERN_MSB_8BITS              0xFF
#define RRM_OAM_DEFAULT_SF_PATTERN_LSB_2BITS              0xC0
/*eMTC changes stop*/
#endif
