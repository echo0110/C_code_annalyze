/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPhyInterface.h,v 1.1.1.1.6.2.6.2 2010/10/25 09:35:21 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This files contains the PHY Interface defintions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPhyInterface.h,v $
 * Revision 1.1.1.1.6.2.6.2  2010/10/25 09:35:21  gur23971
 * warning removed
 *
 * Revision 1.1.1.1.6.2.6.1  2010/09/21 15:45:53  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.25.26.1  2010/07/21 08:53:54  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Module ID aligned as agreed with MAC and RRC teams(PHY Module).
 * 2. Data type for port number variables changed to unsigned short
 *
 * Revision 1.25  2009/12/01 15:37:37  gur19413
 * added new function prototype
 *
 * Revision 1.24  2009/10/27 07:09:21  gur19413
 * reverted RAW Socket changes
 *
 * Revision 1.23  2009/10/26 13:08:28  gur11912
 * modified for RAW Sockets
 *
 * Revision 1.22  2009/10/26 10:15:29  gur19413
 * modified socket functionality
 *
 * Revision 1.21  2009/10/21 05:08:30  gur19413
 * Socket Functionality Modified
 *
 * Revision 1.20  2009/09/02 17:24:27  gur19413
 * socket interface related changed.
 *
 * Revision 1.19  2009/07/28 06:10:19  gur20052
 * changed the size of SH_MESSAGE_UL_BUF_SIZE to 10000*2
 *
 * Revision 1.18  2009/07/22 07:08:15  gur19413
 * added socket variable extern.
 *
 * Revision 1.17  2009/07/20 13:18:54  gur18569
 * changed SH_MESSAGE_UL_GRANT_SIZE to SH_MESSAGE_UL_BUF_SIZE
 *
 * Revision 1.16  2009/07/14 14:51:17  gur20439
 * phy module id changed from 03 to 06.
 *
 * Revision 1.15  2009/07/13 04:35:37  gur19413
 * added new macro for log
 *
 * Revision 1.14  2009/07/01 13:25:22  gur19413
 * add cleanup function
 *
 * Revision 1.13  2009/07/01 12:32:37  gur19413
 * enum modified
 *
 * Revision 1.12  2009/06/18 13:45:18  gur18550
 * Errors removed
 *
 * Revision 1.11  2009/06/18 10:14:17  gur19413
 * PHY_MODULE_VALUE changed
 *
 * Revision 1.10  2009/06/11 06:18:32  gur19413
 * remove macro MAC_MODULE_ID
 *
 * Revision 1.9  2009/06/10 13:25:56  gur19413
 * comments incorporated
 *
 * Revision 1.8  2009/06/08 15:35:18  gur19413
 * added new function prototype
 *
 * Revision 1.7  2009/06/05 12:23:28  gur19413
 * added new function
 *
 * Revision 1.6  2009/06/05 09:48:19  gur19413
 * remove compilation error
 *
 * Revision 1.5  2009/05/29 06:30:03  gur19413
 * added new macro
 *
 * Revision 1.4  2009/05/27 04:33:43  gur18550
 * API VALUE updated
 *
 * Revision 1.3  2009/05/26 13:20:51  gur19413
 * DCI_FORMAT 's rename
 *
 * Revision 1.2  2009/05/25 22:52:36  gur19413
 * addded new enum
 *
 * Revision 1.1  2009/05/22 13:55:38  gur19413
 * converting into enum's
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PHY_INTERFACE
#define LTE_MAC_PHY_INTERFACE 

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteLog.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/**** API Identifier                 Value  ****/
#define PHY_MODULE_ID 0x08 
#define PHY_MAX_REQ_API 100
#define PHY_MAX_CNF_API 200
#define PHY_MAX_IND_API 300

#define SIZE_OF_UL_GRANT_TO_PHY   19

/* SPR 1768 changes start */
#define FAPI_CTRL_MSG_MAX_BUF_SZ 2000
/* SPR 1768 changes end */

/* Maximum size of Uplink MAC PDU */
/*UL_MU_MIMO_CHG Start*/
#ifdef UL_MU_MIMO_BUILD_ENABLED
#define SH_MESSAGE_UL_BUF_SIZE (19000)
#else
#define SH_MESSAGE_UL_BUF_SIZE (10000)
#endif
/*UL_MU_MIMO_CHG End*/
/* Maximum size of Uplink UL Grant */
#define SH_MESSAGE_UL_GRANT_SIZE (SIZE_OF_UL_GRANT_TO_PHY+4)

#define SHM_MSG_DL_ID 0x12345678
#define SHM_MSG_UL_ID 0x12345679

#define MSG_QUEUE_EXIST 17
#define PHY_INTERFACE   "PHY INTF"
/* If MAC and PHY have different endianness, this  macro's value should
 * be 1, else its value should be 0 */
#ifdef MAC_PHY_DIFF_ENDIAN
    /* Macro to convert 16 bit integer from MAC endianness to PHY endianness */
#   define MAC_PHY_CONVERT_16(x)   bswap16Wrap(x)
    /* Macro to convert 32 bit integer from MAC endianness to PHY endianness */
#   define MAC_PHY_CONVERT_32(x)   bswap32Wrap(x)

#else /* MAC_PHY_DIFF_ENDIAN == 1 */

    /* Macro to convert 16 bit integer from MAC endianness to PHY endianness */
#   define MAC_PHY_CONVERT_16(x)    (x)
    /* Macro to convert 32 bit integer from MAC endianness to PHY endianness */
#   define MAC_PHY_CONVERT_32(x)    (x)
#endif /* MAC_PHY_DIFF_ENDIAN == 1 */
/**** TAG Identifier                 Value  ****/
#define PHY_CONFIG_CELL_REQ             1
#define PHY_CONFIG_CELL_CNF             2
/* Reconfig Chg start*/
#define PHY_DELETE_CELL_REQ		3
#define PHY_DELETE_CELL_CNF		4
/* Reconfig Chg ends */
#define PHY_CREATE_UE_ENTITY_REQ        5
#define PHY_CREATE_UE_ENTITY_CNF        6
#define PHY_DELETE_UE_ENTITY_REQ        7
#define PHY_DELETE_UE_ENTITY_CNF        8
#define PHY_RECONFIG_UE_ENTITY_REQ      9
#define PHY_RECONFIG_UE_ENTITY_CNF      10
#define PHY_RECONFIG_CELL_REQ           11
#define PHY_RECONFIG_CELL_CNF           12
#define PHY_CHANGE_CRNTI_REQ            13
#define PHY_CHANGE_CRNTI_CNF            14 
/* CELL START STOP CHG */
#define PHY_CELL_START_REQ              15
#define PHY_CELL_START_CNF              16
#define PHY_CELL_STOP_REQ               17
#define PHY_CELL_STOP_CNF               18
/* CELL START STOP CHG */

#define RRC_API_HEADER_LEN             MAC_API_HEADER_LEN //10

typedef enum PHYTagIndentifierT
{
    DCI_0,
    DCI_1,
    DCI_1A,
    DCI_1B,
    DCI_1C,
    DCI_1D,
    DCI_2,
    DCI_2A,
    DCI_3,
    DCI_3A,
    MAC_DCI_PDU_INFO,
    MAC_HI_PDU_INFO,
    MAC_TB_INFO,
    MAC_BCH_PDU_INFO,
    MAC_DLSCH_PDU_INFO,
    MAC_PCH_PDU_INFO,
    SR_HARQ_PDU_INFO,
    CRC_HARQ_PDU_INFO,
    CRC_PDU_INFO,
    SR_PDU_INFO,
    HARQ_PDU_INFO,
    HARQ_TB_PDU_INFO,
    RACH_PDU_INFO,
    ULSCH_PDU,
    ULSCH_CQI_RI_PDU,
    CQI_PDU,
    TIMING_ADVANCE_PDU_INFO,
    DL_ERROR_INFO,
    UL_ERROR_INFO,
    APERIODIC_CQI_MODE_2_0,
    APERIODIC_CQI_MODE_3_0,
    APERIODIC_CQI_MODE_3_1,
    APERIODIC_CQI_MODE_1_2,
    APERIODIC_CQI_MODE_2_2,
    PERIODIC_CQI_MODE_1_0_TYPE_3,
    PERIODIC_CQI_MODE_1_0_TYPE_4,
    PERIODIC_CQI_MODE_1_1_TYPE_2,
    PERIODIC_CQI_MODE_1_1_TYPE_3,
    PERIODIC_CQI_MODE_2_0_TYPE_1,
    PERIODIC_CQI_MODE_2_0_TYPE_3,
    PERIODIC_CQI_MODE_2_0_TYPE_4,
    PERIODIC_CQI_MODE_2_1_TYPE_1,
    PERIODIC_CQI_MODE_2_1_TYPE_2,
    PERIODIC_CQI_MODE_2_1_TYPE_3
}PHYTagIndentifier;

/* API Identifier Value */


typedef enum MacPhyRequestApiT
{
    MIN_MAC_PHY_REQ_API = 0, 
    MAC_REGISTER_REQ,  
    MAC_DL_CONTROL_MSG_REQ,
    MAC_DL_DATA_MSG_REQ,
    MAX_MAC_PHY_REQ_API = 49 
}MacPhyRequestApi;


typedef enum MacPhyConfirmApiT
{
    MIN_MAC_PHY_CNF_API = MAX_MAC_PHY_REQ_API + 1,
    MAC_REGISTER_CNF,
    MAX_MAC_PHY_CNF_API = 99
}MacPhyConfirmApi;

typedef enum MacPhyIndicationT
{
    MIN_MAC_PHY_IND_API = MAX_MAC_PHY_CNF_API + 1,
    MAC_SUB_FRAME_START_IND,      
    MAC_UL_CONTROL_MSG_IND,       
    MAC_UL_DATA_MSG_IND,          
    MAC_UL_SUBFRAME_CONFIG_IND,   
    MAC_ERROR_IND,
    MAX_MAC_PHY_IND_API = 149
}MacPhyIndicationT;                


typedef struct ShMsgULGrantBufT {
    UInt32   mtype;
    UInt8    mtext[SH_MESSAGE_UL_BUF_SIZE];
} ShDLMessageQBuf;

typedef struct ShMsgULBufT {
    UInt32   mtype;
    UInt8    mtext[SH_MESSAGE_UL_BUF_SIZE];
} ShULMessageQBuf;


/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*Share Message Queue ID for Uplink*/
extern SInt32 shMsgQULId_g;

/*Share Message Queue ID for Downlink*/
extern SInt32 shMsgQDLId_g;


extern UChar8 macEthernetAddress_g[HOSTNAME_MAX_LEN];
extern UChar8 phyEthernetAddress_g[HOSTNAME_MAX_LEN];
extern Char8 macIPAddress_g[HOSTNAME_MAX_LEN];
/* CA Changes start */
extern Char8 phyIPAddress_g[MAX_NUM_CELL][HOSTNAME_MAX_LEN];
extern UInt16 phyTxPort_g[MAX_NUM_CELL];
extern UInt16 phyRxPort_g[MAX_NUM_CELL];
/* CA Changes end */
#ifdef DL_UL_SPLIT_TDD
extern SInt32 phyRxSockUlFD_g;
extern UInt16 phyRxPortUl_g;
#endif


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*CA Changes start  */
void initPHYInterface(UInt8 numCellsConfigured);
/*CA Changes end  */
/* CHG PURIFY start */
/* CA Changes start */
SInt32 recvDataFromPhysicalByLength (void* msgBuf_p, UInt32 recvLen,
                       InternalCellIndex internalCellIndex);
void initPhySocketInterface(InternalCellIndex cellIndex);
/* CA Changes end */
/* CHG PURIFY end */
/*CA Changes start  */
/*CA Changes end  */
SInt32 sendDataToPhysical (UInt8 *bufferToSend_p, UInt32 bufferLen, 
                              InternalCellIndex internalCellIndex);
/* CA Changes start */
SInt32 sendControlInfoToPhy(UInt8 *bufferToSend_p,UInt32 bufferLen, 
                  InternalCellIndex InternalCellIndex);
SInt32 recvDataFromPhysical (void* msgBuf_p, InternalCellIndex cellIndex);
SInt32 recvControlInfoFromPhysical(void* msgBuf_p,
                  InternalCellIndex InternalCellIndex);
/* CA Changes end */
SInt32 recvFrameIndFromPhysical(void* msgBuf_p);
/*CA Changes start  */
void cleanupPHYInterface(InternalCellIndex cellIndex);
/*CA Changes end  */
void deInitPHYInterface(InternalCellIndex internalCellIndex);
SInt32 recvDataFromPhysicalNonBlocking (void* msgBuf_p,
             InternalCellIndex internalCellIndex);
/* SPR 609 changes start */  
/* CA Stats Changes Start */
extern void lteMacUpdateStatsPhyRx(UInt8 *pBufferReceived, UInt32 bufferLen,
        InternalCellIndex internalCellIndex);
extern void lteMacUpdateStatsPhyTx(UInt8 *pBufferToSend, UInt32 bufferLen,
        InternalCellIndex internalCellIndex);
/* CA Stats Changes End */
/* SPR 609 changes end */   
/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef DL_UL_SPLIT_TDD 
extern void lteMacUpdateUlStatsPhyRx(UInt8 *pBufferReceived, UInt32 bufferLen);
SInt32 recvUlDataFromPhysical (void* msgBuf_p);
#endif
#endif  /* LTE_MAC_PHY_INTERFACE */

