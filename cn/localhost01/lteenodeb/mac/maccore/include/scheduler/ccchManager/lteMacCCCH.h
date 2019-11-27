/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacCCCH.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the macros, extern variables and 
 *                     function declarations related to CCCH manager
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacCCCH.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.19  2009/08/03 09:36:53  gur20052
 * Fix for UT_LOGS
 *
 * Revision 1.18  2009/07/26 08:10:49  gur15697
 * compilation warnings removed
 *
 * Revision 1.17  2009/07/21 08:18:34  gur15697
 * FILL_HARQ_INFO() new argument added
 *
 * Revision 1.16  2009/07/13 12:01:52  gur15697
 * CCCH_MGR added
 *
 * Revision 1.15  2009/07/04 08:01:50  gur15697
 * fillCCCHInfo() prototype changed
 *
 * Revision 1.14  2009/07/03 04:58:36  gur15697
 * review commets incorporated
 *
 * Revision 1.13  2009/06/27 05:38:08  gur15697
 * recvCCCHMessage() arguments modifed
 *
 * Revision 1.12  2009/06/19 13:24:40  gur15697
 * sendCCCHMsgToRRC() modified
 *
 * Revision 1.11  2009/06/15 07:28:32  gur11083
 * UT bug fix
 *
 * Revision 1.10  2009/06/05 08:05:20  gur15697
 * init and cleanup functions added
 *
 * Revision 1.9  2009/06/03 12:33:41  gur15697
 * initCCCHMgr() added
 *
 * Revision 1.8  2009/05/28 06:18:09  gur15697
 * Included lteMacDLUEContext.h and  lteMacULUEContext.h
 *
 * Revision 1.7  2009/05/27 09:22:47  gur15697
 * Updated code as per the modified UE contex
 *
 * Revision 1.6  2009/05/20 05:00:01  gur15697
 * code for CCCH message handling added
 *
 * Revision 1.5  2009/05/19 12:45:28  gur15697
 * Compilation errors removed
 *
 * Revision 1.4  2009/05/13 12:15:45  gur15697
 * new function sendCCCHMsgToRRC() added
 *
 * Revision 1.3  2009/05/04 07:52:31  gur15697
 * Initial version
 *
 *
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_CCCH_H
#define LTE_MAC_CCCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacTBSize.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacEncoder.h"
#include "lteMacParseUtil.h"
#include "lteMacRandomAccess.h"
#include "lteMacDLHarqMgr.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define CCCH_MGR "ccch_mgr"

#define TWO_BYTE_HEADER   2

#define THREE_BYTE_HEADER 3

    /* SPR 3876 FIX START */
#define FILL_HARQ_INFO(ueContext_p,harqProcess_p,dlMsgInfo_p,tbSizeFound,numRB,internalCellIndex)\
{\
    /* SPR 9685 changes start */ \
/*    harqProcess_p = checkForFreeHarqProcess(ueContext_p, \
                          ueContext_p->dlHarqContext_p, DRA_SCHEDULING, \
                          internalCellIndex); */\
    /* SPR 9685 changes end */ \
    if (harqProcess_p)\
    {\
      dlMsgInfo_p->dlCCCHMsgInfo.harqProcessId = harqProcess_p->harqProcessId;\
      harqProcess_p->isTBOneValid = TRUE;\
      harqProcess_p->isTBTwoValid = FALSE;\
      harqProcess_p->dlHARQTBOneInfo.mcsIndex = MCS_INDEX_FOR_CCCH;\
      harqProcess_p->dlHARQTBOneInfo.tbSize = tbSizeFound;\
      harqProcess_p->transmissionType = CCCH_TX;\
      harqProcess_p->assignedResourcesNB = numRB;\
      harqProcess_p->dciFormat = DCI_FORMAT_1A_FOR_CCCH;\
    }\
    else\
    {\
      LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,\
                    "[%s]: WARNING: Free HARQ ID not available "\
                    "for CCCH message\n",\
                    __func__);\
    }\
}
    /* SPR 3876 FIX END */
/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct CCCHQueueNodeT
{
    LTE_SNODE ccchNodeAnchor;
    void   *msg_p; /*zero copy buffer*/
    UInt16 rnti; /*Represents CRNTI/TCRNTI*/
    UInt8  msgLen;
    UInt8  rejFlag; /*Connection Rejection message flag*/
}CCCHQueueNode;

/* SPR 4395 changes start */
typedef struct PendingCCCHQueueNodeT
{
    LTE_SNODE pendingCcchNodeAnchor;
    UInt8 *data_p; /*CCCH message buffer*/
    UInt8  dataLen; /*CCCH message buffer length*/
    UInt8  rejFlag; /*Connection Rejection message flag*/
    UInt16 ueId; /*rnti*/
}PendingCCCHQueueNode;
/* SPR 4395 changes end */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
extern void initCCCHMgr(InternalCellIndex internalCellIndex);
/* + SPR_10891 */
extern void cleanupCCCHMgr(InternalCellIndex internalCellIndex);
/* - SPR_10891 */
 void deInitCCCHMgr(InternalCellIndex internalCellIndex);

extern void  scheduleCCCHMsg (PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                              DciCCEContainerInfo *dciCCEContainerInfo_p,
                              UInt32 *availableCommonCCE_p,
                              UInt32 *availableCCE_p,
                              SInt32 *availableDLRBCount_p,
                              UInt32 subFrameNum,
                              PdcchULOutputInfo *pdcchULOutputInfo_p
                              /* 5492 changes start */
                              ,UInt32 *maxUeScheduledInDownlink_p
                              /* 5492 changes end */
                              ,InternalCellIndex internalCellIndex
                              );

/* SPR 4395 changes start */
extern void  schedulePendingCCCHMsg (PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                              DciCCEContainerInfo *dciCCEContainerInfo_p,
                              UInt32 *availableCommonCCE_p,
                              UInt32 *availableCCE_p,
                              SInt32 *availableDLRBCount_p,
                              UInt32 subFrameNum,
                              PdcchULOutputInfo *pdcchULOutputInfo_p
                              /* 5492 changes start */
                              ,UInt32 *maxUeScheduledInDownlink_p
                              /* 5492 changes end */
                              ,InternalCellIndex internalCellIndex
                              );
/* SPR 4395 changes end */

extern UInt8* formTB(UInt32 buffLen,
                     UInt32 *headerType_p,
                     UInt32 tbSizeFound);

/* SPR 5599 changes start (ZIP ID 129059) */
extern void sendCCCHMsgToRRC(void  *msg_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                    UInt32 dataLen,
                                    UInt16 rnti,
                                    UInt16 timingAdvance,
                                    UInt8 sinr,
                                    InternalCellIndex internalCellIndex);

extern MacRetType recvCCCHMessage(UInt8  *data_p,
                                  void   *ccchMsg_p,
                                  UInt32 dataLen,
                                  UInt32 connRejFlag,
                                  InternalCellIndex internalCellIndex);

extern MacRetType allocateCCEInCommonSpaceForCCCH(UInt32    aggregationLevel,
                                          UInt32     *availableCCE_p,
                                          DciCCEInfo *dciCCEInfo_p,
                                          PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                                          UInt16     rnti,
                                          DCIFormat  dciFormat,
                                          InternalCellIndex internalCellIndex);

extern MacRetType allocateCCEInUESpecificSpaceForCCCH(
                            UESpecificSearchSpaceInfo  *ueSearchSpaceInfo_p,
                            DCIFormat         dciFormat,
                            UInt32            *availableCCE_p, 
                            DciCCEInfo        * dciCCEInfo_p,
                            PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                            UInt16 rnti,
                            DLUEContext   *ueContext_p,
                            DciCCEContainerInfo *dciCCEContainerInfo_p,
                            InternalCellIndex internalCellIndex);

extern void fillCCCHInfo(
                         PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                         CCCHQueueNode     *ccchQueueNode_p, 
                         UInt8  *transmitBlock_p, 
                         UInt8  numRB,
                         UInt32 headerLen,
                         /* SPR 24051 change start */
                         UInt32  tbSizeFound);
                         /* SPR 24051 change end */

/* SPR 609 changes start */
MacRetType setCCEIndexInULContex( UInt16 ueIndex,
        UInt8 subframeNum,
        UInt8 cceIndex,
        /** SPR 14204 Fix : Start **/
        InternalCellIndex internalCellIndex
        /** SPR 14204 Fix : End **/
        /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
        ,UInt8 isTBOneValid
        ,UInt8 isTBTwoValid
        /* SPR 19679: TDD HARQ Multiplexing  Changes End */
        );
/* SPR 609 changes end */


#endif
