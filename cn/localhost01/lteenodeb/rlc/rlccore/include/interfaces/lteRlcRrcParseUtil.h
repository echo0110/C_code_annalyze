/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRrcParseUtil.h,v 1.1.1.1.6.1.4.2.2.2 2010/10/20 05:36:49 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the LTE RLC Layer
 *                     Interface with RRC Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcRrcParseUtil.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/10/20 05:36:49  gur20491
 * rlc core fix U8 change
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:49:50  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1.2.1  2010/08/10 12:16:30  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.17.2.4  2010/07/17 11:36:43  gur23054
 * Review Comments fixed
 *
 * Revision 1.17.2.3  2010/07/15 06:31:17  gur23054
 * Review comments fixes
 *
 * Revision 1.17.2.2  2010/06/22 09:13:50  gur19836
 * Reconfig API Support Feature Enhancement
 *
 *
 *
 ****************************************************************************/

#ifndef LTERLC_PARSE_UTIL_H
#define LTERLC_PARSE_UTIL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteTypes.h"
#include "lteMemPool.h"
#include "lteRlcRrcExtInterface.h"
#include "lteRlcUeContext.h"
#include "lteRlcRrcInterface.h"


/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* validating UE ID */
#define IS_UE_ID_INVALID(ui16_ueIndex) \
    ((ui16_ueIndex >= MAX_UE_ID) ? RLC_INVALID_UE_ID : RLC_SUCCESS)

/* checking if UE Context already exists */
#define IS_UE_ID_NOT_EXISTS(ui16_ueIndex) \
    ((rlcUEContextList_g[ui16_ueIndex] != PNULL) ? RLC_UE_ID_EXISTS : RLC_SUCCESS)

/* validating LCID */
#define IS_LCID_INVALID(ui16_lcId) \
            ((ui16_lcId >= MAX_LC_ID) ? RLC_INVALID_LCID : RLC_SUCCESS)

/* checking if lcId already exists */    
#define IS_LCID_NOT_EXISTS(ui16_ueIndex,ui16_lcId) \
    ((rlcUEContextList_g[ui16_ueIndex]->rlcEntityInfoList[ui16_lcId] \
                           != PNULL) ? RLC_LCID_EXISTS : RLC_SUCCESS)


/****************************************************************************
 * Exported Types
 ****************************************************************************/


/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/* SPR 4010 Fix Start */
#define MAX_RES_DATASIZE                     128
/* SPR 4010 Fix End */
/* SPR 5178 changes starts here */
#define MAX_REQ_DATASIZE                     1000
#define MAX_DATASIZE                     1000
/* SPR 5178 changes ends here */
#define MAX_COMMOM_CHANNEL_DATASIZE          2500

   /* TAGS */
#define     CREATE_TX_UM_RLC_ENTITY         11
#define     CREATE_RX_UM_RLC_ENTITY         12
#define     CREATE_TX_RX_UM_RLC_ENTITY      13
#define     CREATE_TX_RX_AM_RLC_ENTITY      14
#define     CREATE_TX_TM_RLC_ENTITY         15
#define     CREATE_RX_TM_RLC_ENTITY         16
#define     DELETE_TX_UM_RLC_ENTITY         17
#define     DELETE_RX_UM_RLC_ENTITY         18
#define     DELETE_TX_RX_UM_RLC_ENTITY      19
#define     DELETE_TX_RX_AM_RLC_ENTITY      20
#define     RECONFIG_TX_UM_RLC_ENTITY       21
#define     RECONFIG_RX_UM_RLC_ENTITY       22
#define     RECONFIG_TX_RX_UM_RLC_ENTITY    23
#define     RECONFIG_TX_RX_AM_RLC_ENTITY    24
#define     CREATE_COMMON_CHANNEL           25
#define     DELETE_COMMON_CHANNEL           26
#define     RE_ESTABLISH_COMMON_CHANNEL     27
#define     ENTITY_LCID                     28
#define     CREATE_ENTITY_ERROR             29 
#define     DELETE_ENTITY_ERROR             30 
#define     RECONFIG_ENTITY_ERROR           31 
#define     RE_ESTABLISH_ENTITY_ERROR       32

/** Rel 4.0 Changes Start **/
#define     RLC_UE_BIT_RATE_INFO            33
#define     RLC_QCI_INFO                    34
#define     RLC_TX_BIT_RATE_INFO            35
#define     RLC_RX_BIT_RATE_INFO            36
#define     RLC_QUEUE_SIZE_INFO             37
/** Rel 4.0 Changes End **/
/* SPR 4994 Change Start */
#define     RLC_ENTITY_STATE                43
/* SPR 4994 Change End */
#define     RLC_SPS_CONFIG                  38
#define     RLC_SPS_DL_INFO                 39
/*SPR 6861 Fix start*/
#define   RLC_SPS_DL_SETUP_INFO       40
#define     RLC_SPS_UL_INFO                 41
#define   RLC_SPS_UL_SETUP_INFO       42
#ifdef ENDC_ENABLED
#define RLC_DC_BEARER_TYPE_INFO             48
#endif
/*SPR 6861 Fix end*/
/* Common Channel Type */
#define     LC_CCCH                     0
#define     LC_BCCH                     1
#define     LC_PCCH                     2

/* Error Codes */
#define RLC_INVALID_TAG_VALUE           3
#define RLC_INVALID_UE_ID               4
#define RLC_UE_ID_EXISTS                5
#define RLC_UE_NOT_EXISTS               6
#define RLC_INVALID_LCID                7
#define RLC_UECONTEXT_IN_USE            8
#define RLC_LCID_EXISTS                 9
#define RLC_LCID_NOT_EXISTS             10
#define RLC_LCID_IN_USE                 11
#define RLC_UEENTITY_IN_USE             12
#define RLC_COM_CH_IN_USE               13
#define RLC_COM_CH_NOT_EXISTS           14
#define RLC_COM_CH_EXISTS               15
#define RLC_INVALID_COM_CH_TYPE         16
#define RLC_INVALID_POLL_PDU            17
#define RLC_INVALID_POLL_BYTE           18
#define RLC_INVALID_MAX_RETRA_THRESHLD  19
#define RLC_INVALID_STATUS_PROHBT       20
#define RLC_INTERNAL_ERROR              21
#define RLC_SYNTAX_ERROR                22
#define RLC_ENTITY_IN_PENDING_DEL_Q     23
#define RLC_RECONFIG_ENTITY_NOT_SUPPORTED 24
#define RLC_ENTITY_ALREADY_IN_PENDING_DEL_Q     25
#define RLC_MAX_ACTIVE_UE_REACHED_ERR       26
#define RLC_UEENTITY_NOT_EXIST          27

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
#define RLC_WRONG_DC_BEARER_CONFIGURATION 28
#endif


#define ADD_RLC_UE_MANDATORY_LENGTH                 4
#define RECNFG_REESTAB_DEL_UE_MANDATORY_LENGTH      2
#define COMCHNL_UE_MANDATORY_LENGTH                 3
#define ENTITY_ERROR_TAG_LENGTH                     7
#define LCID_TAG_LENGTH                             5
//shreya_qos
/** Chnage For Change CRNTI Start **/
#define CHANGE_CRNTI_MANDATORY_LENGTH               6
/** Chnage For Change CRNTI End **/

/*RLC Rel 2.0 Changes Start*/
#define UM_ENTITY_RECNFIG_TAG_LENGTH                7
#define AM_ENTITY_RECNFIG_TAG_LENGTH               17
/*RLC Rel 2.0 Changes End*/
#define TX_UM_ENTITY_TAG_LENGTH                     7
#define RX_UM_ENTITY_TAG_LENGTH                     9
#define TX_RX_UM_ENTITY_TAG_LENGTH                 11 
#define TX_RX_AM_ENTITY_TAG_LENGTH                 17
#define CREATE_COMMON_CHANNEL_TAG_LENGTH			5

/** Rel 4.0 Changes Start **/
#define QCI_INFO_TAG_LENGTH                        5
#define UE_BIT_RATE_TAG_LENGTH                     12
#define TX_BIT_RATE_INFO_TAG_LENGTH                8
#define RX_BIT_RATE_INFO_TAG_LENGTH                8
#define QUEUE_SIZE_INFO_TAG_LENGTH                 8
#define CREATE_TX_UM_RLC_ENTITY_MIN_LENGTH         7
#define CREATE_RX_UM_RLC_ENTITY_MIN_LENGTH         9
#define CREATE_TX_RX_UM_RLC_ENTITY_MIN_LENGTH      11
#define CREATE_TX_RX_AM_RLC_ENTITY_MIN_LENGTH      17
#define RECONFIG_TX_UM_RLC_ENTITY_MIN_LENGTH       5
#define RECONFIG_RX_UM_RLC_ENTITY_MIN_LENGTH       7
#define RECONFIG_TX_RX_UM_RLC_ENTITY_MIN_LENGTH    7
#define RECONFIG_TX_RX_AM_RLC_ENTITY_MIN_LENGTH    17
/** Rel 4.0 Changes End **/
#define RLC_SPS_CONFIG_MIN_LENGTH                  4
#define RLC_SPS_DL_INFO_MIN_LENGTH                 5
#define RLC_SPS_UL_INFO_MIN_LENGTH                 5
#define RLC_SPS_DL_INFO_LENGTH                     10
#define RLC_SPS_UL_INFO_LENGTH                     10
#define RLC_SPS_DL_SETUP_INFO_LENGTH               5
#define RLC_SPS_UL_SETUP_INFO_LENGTH               5
/* SPR 4994 Change Start */
#define RLC_RE_ESTABLISH_COMPLETE_MANDATORY_LEN     7
/* SPR 4994 Change End */

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESING)
#define DC_BEARER_TYPE_INFO_TAG_LENGTH              5
#endif

/* API Ids between RLC RRC Interface */
#define     RLC_CREATE_UE_ENTITY_REQ                1
#define     RLC_CREATE_UE_ENTITY_CNF                2
#define     RLC_RECONFIG_UE_ENTITY_REQ              3
#define     RLC_RECONFIG_UE_ENTITY_CNF              4
#define     RLC_DELETE_UE_ENTITY_REQ                5
#define     RLC_DELETE_UE_ENTITY_CNF                6
#define     RLC_RE_ESTABLISH_UE_ENTITY_REQ          7
#define     RLC_RE_ESTABLISH_UE_ENTITY_CNF          8
#define     RLC_COMMON_CHANNEL_DATA_REQ             9
#define     RLC_COMMON_CHANNEL_DATA_IND             10
#define     RLC_CONFIG_COMMON_CHANNEL_ENTITIES_REQ  11
/* Rlc Rel 2.0 change start */
#define     RLC_UE_ENTITY_ERROR_IND                 12
/* Rlc Rel 2.0 change stop */
/** Chnage For Change CRNTI Start **/
#define     RLC_CHANGE_CRNTI_REQ                    13
#define     RLC_CHANGE_CRNTI_CNF                    14
/** Chnage For Change CRNTI End**/
/* SPR 4994 Change Start */
#define RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND     15
#define RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF     16
/* SPR 4994 Change End */


/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* This API is to parse the RRC message and based on the values it calls
    different APIs to perform desired operation */
/* SPR 5599_129119 Fix Start */
UInt32 parseRRCMsg(UInt8* msgBuf, SInt32 bytesRead);
/* SPR 5599_129119 Fix End */
/* SPR 5599_129119 Fix Start */
inline void prepareCnfBufferHeader ( UInt8 *cnfMsgBuf,
                                     UInt16 cnfMsgId,
                                     /* + Layer2 NON CA Changes */
                                     UInt16 transactionId, 
                                     RrcCellIndex cellId,
                                     /* - Layer2 NON CA Changes */
                                     UInt16 length
                                   );
/* SPR 5599_129119 Fix End */
extern SInt32 recvMsgWithIccQ(SInt32 mqdes, const char *msg_ptr);

typedef enum RlcProcessRrcOamMsgStateT
{
    RLC_CTRL_IDLE_ST = 0,

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    WAITING_FOR_HP_RLC_DELETE_UE_CONTEXT_RES_ST,
    WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST,
    WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST,
    WAITING_FOR_HP_RLC_REESTAB_ENTITY_RES_ST,
#endif    
    RLC_CTRL_MAX_ST
}RlcProcessRrcOamMsgState;

typedef enum RlcProcessRrcOamMsgEventT
{
    RLC_CTRL_NULL_EV = 0,
    RLC_RECV_MSG_FROM_RRCOAM_EV,
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    RLC_UE_CONTEXT_NOT_EXIST_EV,
    RLC_DELETE_UE_CONTEXT_EV,
    RLC_DELETE_TX_ENTITY_EV,
    RLC_DELETE_RX_ENTITY_EV,
    /*SPR 6631 Start*/
    RLC_DELETE_TX_RX_ENTITY_EV,
    /*SPR 6631 End*/
    RLC_RECONFIG_TX_ENTITY_EV,
    RLC_RECONFIG_RX_ENTITY_EV,
    RLC_REESTAB_ENTITY_EV,
#endif    
    RLC_CTRL_MAX_EV
}RlcProcessRrcOamMsgEvent;


#endif 
