/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacCCCH.c,v 1.1.1.1.6.1.6.1 2010/10/11 12:56:58 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the declarations,definations and 
 *                     includes for CCCH messages
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacCCCH.c,v $
 * Revision 1.1.1.1.6.1.6.1  2010/10/11 12:56:58  gur10121
 * SPR #706 fix
 *
 * Revision 1.1.1.1.6.1.10.1  2010/10/08 07:42:41  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.29.34.1  2010/07/21 09:39:24  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Merged Endian fixes for receiving data from RRC.
 *
 * Revision 1.29  2009/09/01 16:57:08  gur20439
 * DL Strategy Panic BUG fix.
 *
 * Revision 1.28  2009/08/26 11:55:10  gur12140
 * Optimzed For DCI CCE Array
 *
 * Revision 1.27  2009/07/27 15:08:32  gur15697
 * compilation warnings removed
 *
 * Revision 1.26  2009/07/26 08:15:23  gur15697
 * compilation warnings removed
 *
 * Revision 1.25  2009/07/21 15:15:42  gur15697
 * UT bugfix
 *
 * Revision 1.24  2009/07/21 08:17:04  gur15697
 * comments incorporated
 *
 * Revision 1.23  2009/07/13 12:01:32  gur15697
 * second argument to LTE_MAC_UT_LOG modifed
 *
 * Revision 1.22  2009/07/13 10:19:36  gur15697
 * dciformat filled in dlCCCHMsgInfo
 *
 * Revision 1.21  2009/07/07 08:57:15  gur15697
 * txIndicator = NEW_TX added
 *
 * Revision 1.20  2009/07/04 08:01:33  gur15697
 * fillCCCHInfo() prototype changed
 *
 * Revision 1.19  2009/07/03 04:53:55  gur15697
 * review commets incorporated
 *
 * Revision 1.18  2009/06/27 05:38:44  gur15697
 * recvCCCHMessage() arguments modifed
 *
 * Revision 1.17  2009/06/20 12:37:34  gur15697
 * review comments incorporated
 *
 * Revision 1.16  2009/06/19 13:20:18  gur15697
 * review comments incorporated
 *
 * Revision 1.15  2009/06/17 05:23:40  gur15697
 * sendMsgToRRC() changed to sendToRRC()
 *
 * Revision 1.14  2009/06/15 07:25:29  gur11083
 * UT bug fix
 *
 * Revision 1.13  2009/06/15 05:36:02  gur15697
 * UT bug fix
 *
 * Revision 1.12  2009/06/08 09:09:09  gur15697
 * code updated to fill taDrxFlag in Harq context
 *
 * Revision 1.11  2009/06/05 08:03:47  gur15697
 * init and cleanup functions added
 *
 * Revision 1.10  2009/06/05 04:13:03  gur15697
 * updated the file w.r.t PDCCH manager changes
 *
 * Revision 1.9  2009/06/03 12:36:01  gur15697
 * review comments incorporated
 *
 * Revision 1.8  2009/06/01 06:45:35  gur18569
 * removed compilation errors
 *
 * Revision 1.7  2009/05/28 10:05:06  gur15697
 * updated as per the modified DL and UL context
 *
 * Revision 1.6  2009/05/28 05:51:39  gur15697
 * comments encorporated
 *
 * Revision 1.5  2009/05/27 09:23:08  gur15697
 * Updated code as per the modified UE contex
 *
 * Revision 1.4  2009/05/22 06:29:54  gur15697
 * code updated as per the modified UL and DL UE context
 *
 * Revision 1.3  2009/05/21 04:24:50  gur15697
 * sendMsgToRRC() arguments modified
 *
 * Revision 1.2  2009/05/20 04:59:39  gur15697
 * code for CCCH message handling added
 *
 * Revision 1.1  2009/05/04 07:17:49  gur15697
 * File name changed
 *
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
/* Bandwidth Fix */
#include "lteMacCellConfMgr.h"
#include "lteMacDLResourceInfoMgr.h"
/* Bandwidth Fix */
#include "lteMacCCCH.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacTBSize.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
/******************************************************************************
 Private Definitions
 *****************************************************************************/

/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
/*Cyclomatic Complexity changes - starts here*/
static  MacRetType validateCcchQueueNodeForCRNTI(CCCHQueueNode *ccchQueueNode_p,
                                                       RNTIInfo *rntiInfo_p,
                                                       InternalCellIndex internalCellIndex);
static  void updatePdcchULOutputInfoForSuccessFullPdcchAlloc(UInt32 ueIndex,
        /* +- SPR 17777 */
        DLMsgInfo *dlMsgInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 retVal,
        /* +- SPR 17777 */
        CCCHQueueNode *ccchQueueNode_p
#ifdef TDD_CONFIG
        ,UInt32 subFrameNum
        ,InternalCellIndex internalCellIndex
#endif
        /* +- SPR 17777 */
        );
STATIC  void updateCCCHQueueNodeForTCRNTI(CCCHQueueNode *ccchQueueNode_p,
                                                RNTIInfo *rntiInfo_p,
                                                InternalCellIndex internalCellIndex);
static  MacRetType allocCCEFromCommonSearchSpace ( 
                                       UInt32               *availableCommonCCE_p,
                                       SInt32               *availableDLRBCount_p,
                                       PdcchDLOutputInfo    *pdcchDLOutputInfo_p,
                                       UInt32               *availableCCE_p,
                                       DciCCEContainerInfo  *dciCCEContainerInfo_p,
                                       DLUEContext          *ueContext_p,
                                       PendingCCCHQueueNode *pendingCcchQueueNode_p,
                                       /* SPR 9685 changes start */
                                       DLHARQProcess         *harqProcess_p,
                                       /* SPR 9685 changes start */
                                       InternalCellIndex internalCellIndex
                                       );
/*Cyclomatic Complexity changes - ends here*/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* SPR 4395 changes start */
LTE_SQUEUE *ccchQueue_gp[MAX_NUM_CELL];
LTE_SQUEUE *pendingCcchQueue_gp[MAX_NUM_CELL];
/* SPR 4395 changes end */


/*****************************************************************************
 * Function Name  : initCCCHMgr
 * Inputs         : cellIndex - Cell for which CCH queues are configured
 * Outputs        : None
 * Returns        : None
 * Description    : This fuction will initializes the global queues and 
 *                  variables required by the CCCH manager. This function is called at cell config
 *****************************************************************************/
void initCCCHMgr(InternalCellIndex cellIndex)
{
    LTE_MAC_UT_LOG(LOG_DETAIL ,CCCH_MGR,
            "[%s]: initCCCHMgr()\n",__func__);

    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(LTE_SQUEUE), 2);
    }

    GET_MEM_FROM_POOL(LTE_SQUEUE, ccchQueue_gp[cellIndex],sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(ccchQueue_gp[cellIndex]);

    /* SPR 4395 changes start */
    GET_MEM_FROM_POOL(LTE_SQUEUE, pendingCcchQueue_gp[cellIndex],sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(pendingCcchQueue_gp[cellIndex]);
    /* SPR 4395 changes end */

    return;
}

/****************************************************************************
* Function Name  : cleanupCCCHMgr
* Inputs         : internalCellIndex- specific cellIndex for which cleanup is called.
* Outputs        : None
* Returns        : None
* Description    : This API cleanup the CCCH Manager. (called at cell stop)
****************************************************************************/
void cleanupCCCHMgr(InternalCellIndex internalCellIndex)
{
    /* UInt32 queueCount = 0; TDD Warning Fix */
    CCCHQueueNode *ccchQueueNode_p = PNULL;
    /* SPR 4395 changes start */
    PendingCCCHQueueNode *pendingCcchQueueNode_p = PNULL;
    /* SPR 4395 changes end */

    LTE_MAC_UT_LOG(LOG_DETAIL ,CCCH_MGR,
            "[%s]: cleanupCCCHMgr()\n",__func__);

    ccchQueueNode_p = (CCCHQueueNode *) popNode(ccchQueue_gp[internalCellIndex]);

    while(ccchQueueNode_p)
    {
        if (ccchQueueNode_p->msg_p)
        {
            msgFree(ccchQueueNode_p->msg_p);
        }
        freeMemPool(ccchQueueNode_p);

        ccchQueueNode_p = (CCCHQueueNode *) popNode(ccchQueue_gp[internalCellIndex]);
    }

    /* SPR 4395 changes start */    
    pendingCcchQueueNode_p = (PendingCCCHQueueNode *) popNode(pendingCcchQueue_gp[internalCellIndex]);

    while(pendingCcchQueueNode_p)
    {
        if (pendingCcchQueueNode_p->data_p)
        {
            msgFree(pendingCcchQueueNode_p->data_p);
        }
        freeMemPool(pendingCcchQueueNode_p);

        pendingCcchQueueNode_p = (PendingCCCHQueueNode *) popNode(pendingCcchQueue_gp[internalCellIndex]);
    }
    /* SPR 4395 changes end */
    return;
}

/*****************************************************************************
 * Function Name  : deInitCCCHMgr
 * Inputs         : internalCellIndex- specific cellIndex for which deInit is called.
 * Outputs        : None
 * Returns        : None
 * Description    : This fuction will release the memory for global queues and 
 *                  variables required by the CCCH manager. This is called at the time of Cell delete.
 *****************************************************************************/
void deInitCCCHMgr(InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_DETAIL ,CCCH_MGR,
            "[%s]: initCCCHMgr()\n",__func__);

    if(ccchQueue_gp[internalCellIndex])
    {
        freeMemPool(ccchQueue_gp[internalCellIndex]);
        ccchQueue_gp[internalCellIndex] = PNULL;
    }
    if(pendingCcchQueue_gp[internalCellIndex])
    {
        freeMemPool(pendingCcchQueue_gp[internalCellIndex]);
        pendingCcchQueue_gp[internalCellIndex] = PNULL;
    }

    return;
}


/********************************************************************************
 * Function Name  : recvCCCHMessage
 * Inputs         : data_p     - Pointer to the data points to Tag field,
 *                  ccchMsg_p  - zero buffer,
 *                  dataLen    - Length of the data including RNTI field,
 *                  connRejFlag - Set ot TRUE if Connection Rejection indication
 *                                received otherwise FLASE
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : None
 * Returns        : MAC_FAILUERE/MAC_SUCCESS
 * Description    : This fuction will make the queue node based on input 
 *                  parameters received and push the node to ccchQueue.
 ********************************************************************************/
MacRetType recvCCCHMessage(UInt8  *data_p,
                           void   *ccchMsg_p,
                           UInt32 dataLen,
                           UInt32 connRejFlag,
                           InternalCellIndex internalCellIndex)
{
    CCCHQueueNode *ccchQueueNode_p = PNULL;
    UInt32 dataPtrPos = RRC_MAC_API_HEADER_LEN;
    UInt16 rnti    = 0;
    UInt16 lowerRNTIBound = 0;
    UInt16 upperRNTIBound = 0;
    UInt16 startRARNTI    = 0;
    UInt16 endRARNTI      = 0;

    InitParams *initParams_p =
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;         

    LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
            "Entry [%s]\n",__func__);
    //CellAndInitParams *cellConfigAndInitParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex];
    /*Now pointer points to 2 bytes of RNTI filed*/
    rnti = LTE_GET_U16BIT(data_p);
    data_p     += 2;

    {
        lowerRNTIBound = initParams_p->lowerRNTIBound;
        upperRNTIBound = initParams_p->upperRNTIBound;
        startRARNTI    = cellParams_p->startRARNTI;
        endRARNTI      = cellParams_p->endRARNTI;
    }

    if ((rnti < lowerRNTIBound) || (rnti > upperRNTIBound)||
        ( (rnti >= startRARNTI) && (rnti <= endRARNTI) ))
    {
        lteWarning("[%s]: Invalid RNTI received\n",__func__);
        return MAC_FAILURE;
    }

    dataPtrPos += 2;
    /*Decrement the RNTI field length*/
    dataLen -= 2;
    GET_MEM_FROM_POOL(CCCHQueueNode , ccchQueueNode_p,sizeof(CCCHQueueNode),PNULL);
    /* coverity-530 CID 24490*/
    if(PNULL == ccchQueueNode_p)
    {
       return MAC_FAILURE;
    }
    /* coverity-530 CID 24490*/

    ccchQueueNode_p->msgLen  = dataLen;
    ccchQueueNode_p->rnti    = rnti;
    ccchQueueNode_p->rejFlag = connRejFlag;

    SPLIT_ZERO_BUFFER_COPY(ccchMsg_p, ccchQueueNode_p->msg_p, dataPtrPos);

    LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
            "[%s]: Received CCCH message Info:\n"
            "RNTI [%d] dataLen [%d]",__func__,
            ccchQueueNode_p->rnti,ccchQueueNode_p->msgLen);
    
    if (pushNode(ccchQueue_gp[internalCellIndex], &ccchQueueNode_p->ccchNodeAnchor))
    {
        lteWarning("[%s]: ERROR: pushNode to ccchQueue_g failed\n",__func__);
        /* +COVERITY 32465 */
        msgFree(ccchQueueNode_p->msg_p);
        freeMemPool(ccchQueueNode_p);

        /* +COVERITY 32465 */
        return MAC_FAILURE;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
            "[%s]: CCCH queue node created and pushed to ccchQueue_g Queue\n",
            __func__);
    }

    LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
                   "Exit [%s]\n",__func__);
    return MAC_SUCCESS;
}
/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : allocCCEFromCommonSearchSpace 
 * Inputs         : availableCommonCCE_p - Available CCE in commn space,
 *                  availableDLRBCount_p - DL RBs available,
 *                  availableCCE_p - Total available CCE,
 *                  ueContext_p - Pointer to DL UE context,
 *                  pendingCcchQueueNode_p - Pointer to CCCH queue node 
 *                  harqProcess_p - Pointer to Harq Process
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : pdcchDLOutputInfo_p - Pointer to DL output container,
 *                  dciCCEContainerInfo_p - Pointer to CCE container
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will allocate CCE from Common Search Space.
 *****************************************************************************/
static  MacRetType allocCCEFromCommonSearchSpace 
(
   UInt32               *availableCommonCCE_p,
   SInt32               *availableDLRBCount_p,
   PdcchDLOutputInfo    *pdcchDLOutputInfo_p,
   UInt32               *availableCCE_p,
   DciCCEContainerInfo  *dciCCEContainerInfo_p,
   DLUEContext          *ueContext_p,
   PendingCCCHQueueNode *pendingCcchQueueNode_p,
   /* SPR 9685 changes start */
   DLHARQProcess *harqProcess_p,
   /* SPR 9685 changes end */
   InternalCellIndex internalCellIndex )
{
    UInt32 aggregationLevel = INVALID_AGGREGATION_LEVEL;
    UInt8  tbSizeFound      = 0;
    DciCCEInfo           *dciCCEInfo_p = NULL;
    UInt8  numRB            = 0;
    DLMsgInfo     *dlMsgInfo_p   = PNULL;

    /* + Coverity 42187 */
    MacRetType       retVal = MAC_FAILURE;
    /* - Coverity 42187 */
    UInt32 ueIndex          = MAX_UE_SUPPORTED;
    UInt8         *transmitBlock_p   = PNULL;
    
    ueIndex = pendingCcchQueueNode_p->ueId; 
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    
    if (*availableCommonCCE_p >= AGGREGATION_LEVEL_FOR_COMMON_SPACE)
    {
        aggregationLevel = AGGREGATION_LEVEL_FOR_COMMON_SPACE;

        tbSizeFound = pendingCcchQueueNode_p->dataLen;
        UInt32 x = tbSizeFound;
        /* SPR 5999 changes start */
        numRB = DETERMINE_RBs_DL(MCS_INDEX_FOR_CCCH, x, internalCellIndex);
        /* SPR 5999 changes end */
        tbSizeFound = x;

        if ((numRB) && (numRB <= *availableDLRBCount_p))
        {
            UInt32        count = 0;

            count = pdcchDLOutputInfo_p->countCommonChannelMsg;
            /* ICIC changes start */
            dlMsgInfo_p = 
                &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
            /* ICIC changes end */

            /*Allocate CCE*/
            /* Fix SPR 21492 start */ 
            if (FALSE == pendingCcchQueueNode_p->rejFlag)
            {
                /* Fix SPR 21492 end */ 

                retVal = allocateCCEInCommonSpaceForCCCH(aggregationLevel,
                        availableCCE_p,
                        dciCCEInfo_p,
                        pdcchDLOutputInfo_p,
                        ueContext_p->crnti,
                        DCI_FORMAT_1A_FOR_CCCH,
                        internalCellIndex);
                /* Fix SPR 21492 start */ 
            }
            else
            {
                retVal = allocateCCEInCommonSpaceForCCCH(aggregationLevel,
                        availableCCE_p,
                        dciCCEInfo_p,
                        pdcchDLOutputInfo_p,
                        pendingCcchQueueNode_p->ueId,
                        DCI_FORMAT_1A_FOR_CCCH,
                        internalCellIndex);
            }
            /* Fix SPR 21492 end */ 
            if (MAC_SUCCESS == retVal)
            {
                /*In case of Connection reject message no need to use
                 *HARQ retrans
                 */
                /* +COVERITY 10865 */
                if (FALSE == pendingCcchQueueNode_p->rejFlag)
                    /* -COVERITY 10865 */
                {
                    /* + SPR 11491 Changes */
                    if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
                    {
                        ueContext_p->isPdcchAllocatedFlag = TRUE;
                    }
                    else
                    {
                        ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = TRUE;
                    }                       
                    /* - SPR 11491 Changes */

                    FILL_HARQ_INFO(ueContext_p,harqProcess_p,
                            dlMsgInfo_p,tbSizeFound,numRB,internalCellIndex);

                    dciCCEContainerInfo_p->\
                        dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                        dciPduInfo.ueIndex = ueIndex;
                }
                else
                {
                    dciCCEContainerInfo_p->\
                        dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                        dciPduInfo.harqProcessId = INVALID_HARQ_ID;
                }
                dciCCEContainerInfo_p->arrayCCEAllocated[
                    dciCCEContainerInfo_p->countOfCCEAllocated] =
                    dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
                /*SPR 21589 Fix  ++*/
                /*Code removed*/      
                /*SPR 21589 Fix  --*/   
                /*Add padding header and normal header*/
                transmitBlock_p = pendingCcchQueueNode_p->data_p;

                if (PNULL != transmitBlock_p)
                {
                    /*SPR 21589 Fix  ++*/
                    dciCCEContainerInfo_p->countOfCCEAllocated++;
                    /*SPR 21589 Fix  --*/
                    DLCCCHMsgInfo *dlCCCHMsgInfo_p = PNULL;
                    /* ICIC changes start */
                    dlCCCHMsgInfo_p = &pdcchDLOutputInfo_p->\
                                      dlMsgTypeCommonMsgInfo[(pdcchDLOutputInfo_p->countCommonChannelMsg - 1)].\
                                      dlMsgInfo.dlCCCHMsgInfo;
                    /* ICIC changes end */

                    dlCCCHMsgInfo_p->dataLen = tbSizeFound;
                    dlCCCHMsgInfo_p->data_p = transmitBlock_p;
                    dlCCCHMsgInfo_p->resourceAllocInput.requiredRB = numRB;

                    /*Decrement the number of DL RBs available*/
                    *availableDLRBCount_p -= numRB;
                    /*Decrement the available CCE from Common space*/
                    *availableCommonCCE_p -= aggregationLevel;

                    if (harqProcess_p){
                        harqProcess_p->transmissionBuffer_p = 
                            transmitBlock_p;
                        /* SPR 2051 changes start */
                        harqProcess_p->totalTBSize = tbSizeFound;  
                        /* SPR 2051 changes end */
                    }
                }
                else
                {
                    lteWarning("[%s]: ERROR formTB() returned failure\n",
                            __func__);
                    retVal = MAC_FAILURE;
                }
            }
        }
    }
    return retVal;
}
/* cyclomatic changes */

/* SPR 4395 changes start */
/**************************************************************************************
 * Function Name  : schedulePendingCCCHMsg
 * Inputs         : availableCommonCCE_p - Available CCE in commn space,
 *                  availableCCE_p - Total available CCE,
 *                  availableDLRBCount_p - DL RBs available,
 *                  subFrameNum - Sub frame number w.r.t DL delay
 *                  maxUeScheduledInDownlink_p - Maximum UE's Scheduled for Downlink
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : pdcchDLOutputInfo_p - Pointer to PdcchDLOutputInfo,
 *                  dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                  pdcchULOutputInfo_p - Pointer to PdcchULOutputInfo
 * Returns        : None
 * Description    : This will schedules the pending CCCH messages.
 **************************************************************************************/
void  schedulePendingCCCHMsg (PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                              DciCCEContainerInfo *dciCCEContainerInfo_p,
                              UInt32 *availableCommonCCE_p,
                              UInt32 *availableCCE_p,
                              SInt32 *availableDLRBCount_p,
                              UInt32 subFrameNum,
                              PdcchULOutputInfo *pdcchULOutputInfo_p
                              /* 5492 changes start */
                              ,UInt32 *maxUeScheduledInDownlink_p,
                              /* 5492 changes end */
                              InternalCellIndex internalCellIndex
                              )
{
    DciCCEInfo    *dciCCEInfo_p      = PNULL;
    PendingCCCHQueueNode *pendingCcchQueueNode_p   = PNULL;
    RNTIInfo      *rntiInfo_p        = PNULL;
    DLUEContextInfo *ueContextInfo_p = PNULL;
    DLUEContext   *ueContext_p       = PNULL;
    UInt8         *transmitBlock_p   = PNULL;
    UInt32 queueCount           = 0;
    UInt32 msgFreeFlag          = FALSE;
    UInt32 allocFromCommon      = FALSE;
    UInt32 ueContextPresentFlag = FALSE;
    UInt32 retVal           = MAC_FAILURE;
    UInt32 aggregationLevel = INVALID_AGGREGATION_LEVEL;
    UInt8  numRB            = 0;
    UInt32 ueIndex          = MAX_UE_SUPPORTED;
    UInt8  tbSizeFound      = 0;
    /* SPR 9685 changes start */
    DLHARQProcess *harqProcess_p = PNULL;
    PendingCCCHQueueNode *tempPendingCcchQueueNode_p = PNULL;
    /* SPR 9685 changes end */
    /*SPR 953 changes start*/
    UInt8  *count_p = PNULL;
    DLMsgInfo     *dlMsgInfo_p   = PNULL;
    /*SPR 953 changes end*/

    /* SPR 6021 Changes Start */
    /* PDCCHRachNode pdcchRachNode;*/
    /* SPR 6021 Changes End */
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    UInt8 tempUECount = 0;
    /* SPR 4128 Start */
    UInt32 maxCCCHUe = MAX_CCCH_UE_SCHEDULED;
    /* SPR 4128 End */
    /* 5492 changes start */
    /* 5492 changes end */
    /* + SPR 11469 Changes */
    DLHARQEntity *dlHarqContext_p = PNULL;
    /* - SPR 11469 Changes */
    /* Fix SPR 21492 start*/ 
    UInt32 pendingCcchQNodeDeletionNeeded = FALSE;
    /* Fix SPR 21492 end */ 
#ifdef TDD_CONFIG
    if (checkForSpecialSubframe(subFrameNum, internalCellIndex))
    {
        return;
    }
#endif
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    /*Get the Queue count from pendingCcchQueue_g*/
    queueCount = sQueueCount(pendingCcchQueue_gp[internalCellIndex]);
    /* 5492 changes start */
    /* 5492 changes end */
    /* SPR 9685 changes start */
    pendingCcchQueueNode_p = (PendingCCCHQueueNode *) getFirstNode(pendingCcchQueue_gp[internalCellIndex]);
    /* SPR 9685 changes end */
    while (
            (*maxUeScheduledInDownlink_p)&&
            ((queueCount) && (maxCCCHUe)))
        /* SPR 4128 End */
    {
        /* 5492 changes start */
        allocFromCommon = FALSE;
        /* 5492 changes end */
        /* SPR 4128 Start */
        /* +COVERITY 10865 */
        ueContextPresentFlag = FALSE;
        /* +COVERITY 10865 */

        /* SPR 9685 changes start */
        harqProcess_p = PNULL;
        /* SPR 9685 changes end */

        /* SPR 4128 End */

        if (PNULL == pendingCcchQueueNode_p)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                    "[%s]: ERROR  CCCH Queue node is NULL\n",
                    __func__);
            return;
        }
        /* SPR 9685 changes start */
        tempPendingCcchQueueNode_p = (PendingCCCHQueueNode*)
            getNextNode((const LTE_SNODE *)pendingCcchQueueNode_p);
        /* SPR 9685 changes end */

        ueIndex = pendingCcchQueueNode_p->ueId; 
        /* Fix SPR 21492 start */

        rntiInfo_p = getUeIdxFromRNTIMap(pendingCcchQueueNode_p->ueId, internalCellIndex);
        if(PNULL == rntiInfo_p)
        {
            /* + SPR_6061_6062 */	
            pendingCcchQueueNode_p = (PendingCCCHQueueNode *) popNode(pendingCcchQueue_gp[internalCellIndex]);
            if (pendingCcchQueueNode_p)
            {
                if (pendingCcchQueueNode_p->data_p)
                {
                    msgFree(pendingCcchQueueNode_p->data_p);
                }
                freeMemPool(pendingCcchQueueNode_p);
                pendingCcchQueueNode_p = PNULL;
            }
            /* SPR 9685 changes start */
            pendingCcchQueueNode_p = tempPendingCcchQueueNode_p;
            /* SPR 9685 changes end */
            queueCount--;

            /* - SPR_6061_6062 */	
            continue;
        }
        if (TCRNTI == rntiInfo_p->rntiFlag && pendingCcchQueueNode_p->rejFlag)
        {
            pendingCcchQueueNode_p = (PendingCCCHQueueNode *) popNode(pendingCcchQueue_gp[internalCellIndex]);
            if (pendingCcchQueueNode_p)
            {
                TempCRNTICtx *tempUEContex_p = PNULL; 

                tempUEContex_p = getTempCRNTIContextPointer(pendingCcchQueueNode_p->ueId, internalCellIndex);

                /* Currently there is no HARQ retransmission support of UE with 
                 * temporary Context.When HARQ retransmission support is added 
                 * deletion of temporary context should be done after reception 
                 * of ACK or MAX retransmission of Connection Reject */
                if (tempUEContex_p)
                {
                    /* SPR 6021 Changes Start */
                    PDCCHRachNode *pdcchRachNode_p = PNULL;
                    GET_MEM_FROM_POOL(PDCCHRachNode,
                            pdcchRachNode_p,
                            sizeof(PDCCHRachNode), PNULL);
                    /*CID 114991 fix +*/
                    if(PNULL != pdcchRachNode_p)
                    {

                        pdcchRachNode_p->rnti = tempUEContex_p->tcRNTI;
                        /* CID 114991 + */
                        if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(FREE_TCRNTI, pdcchRachNode_p,
                                internalCellIndex))
                        {
                            freeMemPool(pdcchRachNode_p);
                        }
                        /* CID 114991 - */
                    }
                    /*CID 114991 fix -*/
                    /* SPR 6021 Changes End */
                }
            }
            /* Coverity 114490 fix start */
            else
            {
                pendingCcchQueueNode_p = tempPendingCcchQueueNode_p;
                queueCount--;
                continue;
            }
            /* Coverity 114490 fix end */
            
            queueCount--;
            pendingCcchQNodeDeletionNeeded = TRUE; 
            /* Coverity 114490 Fix Start */
            continue;
            /* Coverity 114490 Fix End */
        }
        else if (CRNTI == rntiInfo_p->rntiFlag )
        {
            /* Fix SPR 21492 end */ 
            ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            /* SPR#706 Changes Start */
            if ( (ueContextInfo_p->pendingDeleteFlag)||
                    ( (PNULL == ueContextInfo_p->dlUEContext_p) && 
                      (FALSE == pendingCcchQueueNode_p->rejFlag) ))
                /* SPR#706 Changes End */
            {
                msgFreeFlag = TRUE;
            }
            else
            {
                ueContextPresentFlag = TRUE;
                ueContext_p = ueContextInfo_p->dlUEContext_p;
            }

            if ((TRUE == msgFreeFlag) 
                    /* COVERITY 10867 */
                    || (ueContext_p == PNULL))
                /* COVERITY 10867 */
            {
                pendingCcchQueueNode_p = (PendingCCCHQueueNode *) popNode(pendingCcchQueue_gp[internalCellIndex]);
                if (pendingCcchQueueNode_p)
                {
                    if (pendingCcchQueueNode_p->data_p)
                    {
                        msgFree(pendingCcchQueueNode_p->data_p);
                    }
                    freeMemPool(pendingCcchQueueNode_p);
                    pendingCcchQueueNode_p = PNULL;
                }
                /* SPR 9685 changes start */
                pendingCcchQueueNode_p = tempPendingCcchQueueNode_p;
                /* SPR 9685 changes end */
                queueCount--;
                msgFreeFlag = FALSE;
                continue;
            }
        }
        /* Fix SPR 21492 start */ 
        /*code removed*/
        /* Fix SPR 21492 end */ 
        /* SPR 9685 changes start */

        if ((TRUE == ueContextPresentFlag) &&
                (FALSE == pendingCcchQueueNode_p->rejFlag))
        {
            /* + SPR 11469 Changes */
            if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
            {
                dlHarqContext_p = ueContext_p->dlHarqContext_p;
            }
            else
            {
                dlHarqContext_p= ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
            }
            harqProcess_p = checkForFreeHarqProcess(ueContext_p, 
                    dlHarqContext_p, DRA_SCHEDULING,internalCellIndex);
            /* - SPR 11469 Changes */
            if(PNULL == harqProcess_p)
            {
                pendingCcchQueueNode_p = (PendingCCCHQueueNode *) \
                                         popNode(pendingCcchQueue_gp[internalCellIndex]);
                if (pendingCcchQueueNode_p)
                {
                    if (pendingCcchQueueNode_p->data_p)
                    {
                        msgFree(pendingCcchQueueNode_p->data_p);
                    }
                    freeMemPool(pendingCcchQueueNode_p);
                    pendingCcchQueueNode_p = PNULL;
                }
                queueCount--;
                continue;
            }
        }
        /* SPR 9685 changes end */

        /*If CCE are available in UE specific search space allocate from
         *UE specific search space, otherwise allocate from common  
         *search space
         */
        if ( *availableCCE_p )
        {
            /* ICIC changes start */
            aggregationLevel = 
                dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1A_FOR_CCCH];
            /* ICIC changes end */

            if (aggregationLevel <= *availableCCE_p)
            {
                tbSizeFound = pendingCcchQueueNode_p->dataLen;
                UInt32 x = tbSizeFound;
                /* Bandwidth Fix */
                if(MIN_REQD_RB_FOR_CCCH > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB)
                {
                    /* SPR 5999 changes start */
                    numRB = DETERMINE_RBs_DL(MAX_MCS_INDEX_FOR_CCCH, x, internalCellIndex);
                    /* SPR 5999 changes end */
                }
                else
                {
                    /* SPR 5999 changes start */
                    numRB = DETERMINE_RBs_DL(MCS_INDEX_FOR_CCCH, x, internalCellIndex);
                    /* SPR 5999 changes end */
                }
                /* Bandwidth Fix */
                tbSizeFound = x;

                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    UInt32        count = 0;

                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    /* ICIC changes start */
                    dlMsgInfo_p = 
                        &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
                    /* ICIC changes end */

                    /*Allocate CCE*/
                    /* Fix SPR 21492 start */ 
                    if ((TRUE == ueContextPresentFlag) &&
                            (FALSE == pendingCcchQueueNode_p->rejFlag))
                    {
                        /* Fix SPR 21492 end */ 
                        retVal = allocateCCEInUESpecificSpaceForCCCH(
                                (rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex] + subFrameNum),
                                DCI_FORMAT_1A_FOR_CCCH,
                                availableCCE_p, 
                                dciCCEInfo_p,
                                pdcchDLOutputInfo_p,
                                ueContext_p->crnti,
                                /*pdcch for scell changes start*/
                                ueContext_p,
                                dciCCEContainerInfo_p,
                                /*pdcch for scell changes end*/
                                internalCellIndex);
                        /* Fix SPR 21492 start */
                    }
                    else
                    {
                        retVal = allocateCCEInCommonSpaceForCCCH(aggregationLevel,
                                availableCCE_p,
                                dciCCEInfo_p,
                                pdcchDLOutputInfo_p,
                                pendingCcchQueueNode_p->ueId,
                                DCI_FORMAT_1A_FOR_CCCH,
                                internalCellIndex);
                    }
                    /* Fix SPR 21492 end */ 
                    if ( MAC_SUCCESS == retVal )
                    {
                        /*In case of Connection reject message no need to use
                         *HARQ retrans
                         */
                        if (TRUE == ueContextPresentFlag && 
                                FALSE == pendingCcchQueueNode_p->rejFlag)
                        {
                            /* + SPR 11491 Changes */
                            if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
                            {
                                ueContext_p->isPdcchAllocatedFlag = TRUE;
                            }
                            else
                            {
                                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = TRUE;
                            }                                
                            /* - SPR 11491 Changes */

                            FILL_HARQ_INFO(ueContext_p,harqProcess_p,
                                    dlMsgInfo_p,tbSizeFound,numRB,internalCellIndex);

                            dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.ueIndex = ueIndex;
                        }
                        else
                        {
                            dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.harqProcessId = INVALID_HARQ_ID;
                        }
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
                        /*SPR 21589 Fix  ++*/
                        /*Code removed*/
                        /*SPR 21589 Fix  --*/
                        /*Add padding header and normal header*/
                        transmitBlock_p = pendingCcchQueueNode_p->data_p;

                        if (PNULL != transmitBlock_p)
                        {
                            /*SPR 21589 Fix  ++*/
                            dciCCEContainerInfo_p->countOfCCEAllocated++;
                            /*SPR 21589 Fix  --*/

                            DLCCCHMsgInfo *dlCCCHMsgInfo_p = PNULL;
                            /* ICIC changes start */
                            dlCCCHMsgInfo_p = &pdcchDLOutputInfo_p->\
                                              dlMsgTypeCommonMsgInfo[(pdcchDLOutputInfo_p->countCommonChannelMsg - 1)].\
                                              dlMsgInfo.dlCCCHMsgInfo;
                            /* ICIC changes end */

                            dlCCCHMsgInfo_p->dataLen = tbSizeFound;
                            dlCCCHMsgInfo_p->data_p = transmitBlock_p;
                            dlCCCHMsgInfo_p->resourceAllocInput.requiredRB = numRB;

                            /*Decrement the number of DL RBs available*/
                            *availableDLRBCount_p -= numRB;
                            if (harqProcess_p){
                                harqProcess_p->transmissionBuffer_p = 
                                    transmitBlock_p;
                                /* SPR 2051 changes start */
                                harqProcess_p->totalTBSize = tbSizeFound;  
                                /* SPR 2051 changes end */
                            }
                        }
                        /* 5492 changes start */
                        (*maxUeScheduledInDownlink_p)--;
                        /* 5492 changes end */
                        maxCCCHUe--;
                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                                "[%s]: WARNING: CCEs are not available in "
                                "UE specific space. Try from the Common space\n",
                                __func__);

                        allocFromCommon = TRUE;
                    }
                }
                else
                {
                    /* SPR 9685 changes start */
                    if(harqProcess_p)
                    {    
                        harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                        harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                    }
                    /* SPR 9685 changes end */
                    LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                            "[%s]: ERROR Required RBs [%d] for CCCH message are"
                            "greater than the available RBs [%d]\n",
                            __func__,numRB,*availableDLRBCount_p);
                    return;
                }
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                        "[%s]: ERROR aggregationLevel [%d] > *availableCCE_p [%d]\n",
                        __func__,aggregationLevel,*availableCCE_p);
                allocFromCommon = TRUE;
            }
        }
        /* SPR 4128 Start */
        else
        {
            allocFromCommon = TRUE;
        }
        /* SPR 4128 End */

        if ((TRUE  == allocFromCommon) &&
                (TRUE == ueContextPresentFlag) &&
                (FALSE == pendingCcchQueueNode_p->rejFlag))
        {
            /* cyclomatic changes */
            MacRetType ret_val = MAC_FAILURE;
            ret_val = allocCCEFromCommonSearchSpace(availableCommonCCE_p,
                    availableDLRBCount_p,
                    pdcchDLOutputInfo_p,
                    availableCCE_p,
                    dciCCEContainerInfo_p,
                    ueContext_p,
                    pendingCcchQueueNode_p,
                    /* SPR 9685 changes start */
                    harqProcess_p,
                    /* SPR 9685 changes end */
                    internalCellIndex);

            if(MAC_FAILURE == ret_val)
            {
                /* SPR 9685 changes start */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl, 
                        dciCCEInfo_p,internalCellIndex);
                if(harqProcess_p)
                {    
                    harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                    harqProcess_p->harqProcessId = INVALID_HARQ_ID;
                }
                /* SPR 9685 changes end */
                return;
            }
            else
            {
                /* 5492 changes start */
                (*maxUeScheduledInDownlink_p)--;
                /* 5492 changes end */
                maxCCCHUe--;
            }
            /* cyclomatic changes */
        }
        /* SPR 4128 Start */
        /* SPR 4128 End */

        if (MAC_SUCCESS == retVal)
        {
            /* Coverity 61387 fix start */
            if((MAX_UE_SUPPORTED > ueIndex) &&
                    (TRUE == ueContextPresentFlag) && (FALSE == pendingCcchQueueNode_p->rejFlag))
                /* Coverity 61387 fix end */
            {
#ifdef TDD_CONFIG
                /** SPR 14204 Fix : Start **/

                /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
                if(setCCEIndexInULContex(ueIndex, subFrameNum, dlMsgInfo_p->dlCCCHMsgInfo.cceIndex,internalCellIndex,
                            harqProcess_p->isTBOneValid,harqProcess_p->isTBTwoValid))

                    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
                    /** SPR 14204 Fix : End **/
                {
                    count_p = &pdcchULOutputInfo_p->pdcchDLUEInfo.count;
                    pdcchULOutputInfo_p->pdcchDLUEInfo.pdcchDLUEArray[*count_p] = ueIndex;
                    (*count_p)++;

                    pdcchULOutputInfo_p->pdcchDLUEInfo.pdcchDLUEArrayForDlGroup
                        [pdcchULOutputInfo_p->pdcchDLUEInfo.cumulativeUeCountForDlGroup] = ueIndex;
                    pdcchULOutputInfo_p->pdcchDLUEInfo.cumulativeUeCountForDlGroup++;
                }
#elif FDD_CONFIG
                /*SPR 953 changes start*/
                count_p = &pdcchULOutputInfo_p->pdcchDLUEInfo.count;
                pdcchULOutputInfo_p->pdcchDLUEInfo.pdcchDLUEArray[*count_p] = ueIndex;
                (*count_p)++;

                pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex = 
                    dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
                /*SPR 1551 changes start*/
                pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlDciFormat =          
                    dlMsgInfo_p->dlCCCHMsgInfo.dciFormat; 
                /*SPR 1551 changes end*/
                /*SPR 953 changes end*/
#endif
            }
            else
            {
                /* To Send UCI HARQ in UL Config for UE with 
                 * temporary context */
                /* + SPR 4301 changes */
#ifdef FDD_CONFIG    
                tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 
                tempUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
#elif TDD_CONFIG
                tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchULUEArrayForTempUlGroup; 
                tempUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup;
#endif
                tcrntiCCEIndexInfo_p[tempUECount].cceIndex = 
                    dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
#ifdef TDD_CONFIG
                tcrntiCCEIndexInfo_p[tempUECount].dlSubFrame = subFrameNum;
#endif      
                /* Coverity 10867 fix start */
                if((TRUE == ueContextPresentFlag) && (FALSE == pendingCcchQueueNode_p->rejFlag))
                {
                    tcrntiCCEIndexInfo_p[tempUECount].tcrnti = ueContext_p->crnti;
                }
                else
                { 
                    tcrntiCCEIndexInfo_p[tempUECount].tcrnti = pendingCcchQueueNode_p->ueId;
                }
                /* Coverity 10867 fix end */
#ifdef FDD_CONFIG    
                pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count++;
#elif TDD_CONFIG
                pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup++;
#endif
                /* - SPR 4301 changes */
            }
        }

        /* Fix SPR 21492 start */
        if(pendingCcchQNodeDeletionNeeded)
        {
            freeMemPool(pendingCcchQueueNode_p);
            pendingCcchQueueNode_p = PNULL;
            pendingCcchQNodeDeletionNeeded = FALSE;
        }
        /*code removed*/
        /* Fix SPR 21492 end */ 
        /* SPR 9685 changes start */
        pendingCcchQueueNode_p = tempPendingCcchQueueNode_p;
        /* SPR 9685 changes end */
        queueCount--;
    }
}
/* SPR 4395 changes end */


/**************************************************************************************
 * Function Name  : scheduleCCCHMsg
 * Inputs         : availableCommonCCE_p - Available CCE in commn space,
 *                  availableCCE_p - Total available CCE,
 *                  availableDLRBCount_p - DL RBs available,
 *                  subFrameNum - Sub frame number w.r.t DL delay,
 *                  maxUeScheduledInDownlink_p - Maximum UE's Scheduled for Downlink
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : pdcchDLOutputInfo_p - Pointer to PdcchDLOutputInfo,
 *                  dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                  pdcchULOutputInfo_p - Pointer to PdcchULOutputInfo
 * Returns        : None
 * Description    : This will schedules the CCCH messages.
 **************************************************************************************/
void  scheduleCCCHMsg (PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                       DciCCEContainerInfo *dciCCEContainerInfo_p,
                       UInt32 *availableCommonCCE_p,
                       UInt32 *availableCCE_p,
                       SInt32 *availableDLRBCount_p,
                       UInt32 subFrameNum,
                       PdcchULOutputInfo *pdcchULOutputInfo_p
                       /* 5492 changes start */
                       ,UInt32 *maxUeScheduledInDownlink_p,
                       /* 5492 changes end */
                       InternalCellIndex internalCellIndex
                       )
{
    DciCCEInfo    *dciCCEInfo_p      = PNULL;
    CCCHQueueNode *ccchQueueNode_p   = PNULL;
    RNTIInfo      *rntiInfo_p        = PNULL;
    DLUEContextInfo *ueContextInfo_p = PNULL;
    DLUEContext   *ueContext_p       = PNULL;
    UInt8         *transmitBlock_p   = PNULL;
    UInt32 queueCount           = 0;
    UInt32 allocFromCommon      = FALSE;
    UInt32 ueContextPresentFlag = FALSE;
    UInt32 retVal           = MAC_FAILURE;
    UInt32 buffLen          = 0;
    UInt32 aggregationLevel = INVALID_AGGREGATION_LEVEL;
    UInt8  numRB            = 0;
    UInt32 ueIndex          = MAX_UE_SUPPORTED;
    UInt32 headerLen        = 0;
    UInt32  tbSizeFound      = 0;

    /*SPR 953 changes start*/
    DLMsgInfo     *dlMsgInfo_p   = PNULL;
    /*SPR 953 changes end*/

/* SPR 4128 Start */
    UInt32 maxCCCHUe = MAX_CCCH_UE_SCHEDULED;
    /* SPR 4128 End */
/* 5492 changes start */
/* 5492 changes end */
    /* SPR 9685 changes start */
    DLHARQProcess *harqProcess_p = PNULL;
    CCCHQueueNode *tempCcchQueueNode_p = PNULL;
    /* SPR 9685 changes end */
    /* + SPR 11469 Changes */
    DLHARQEntity *dlHarqContext_p = PNULL;
    /* - SPR 11469 Changes */
#ifdef TDD_CONFIG
	if (checkForSpecialSubframe(subFrameNum, internalCellIndex))
	{
		return;
	}
#endif
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /*Get the Queue count from ccchQueue_g*/
    queueCount = sQueueCount(ccchQueue_gp[internalCellIndex]);
/* 5492 changes start */
/* 5492 changes end */
    /* SPR 9685 changes start */
    ccchQueueNode_p = (CCCHQueueNode *) getFirstNode(ccchQueue_gp[internalCellIndex]);
    /* SPR 9685 changes end */
    /* SPR 4128 Start */
    while (
            (*maxUeScheduledInDownlink_p) && 
            ((queueCount) && (maxCCCHUe)))
    /* SPR 4128 End */
    {
        /* 5492 changes start */
        allocFromCommon = FALSE;
        /* 5492 changes end */
        /* SPR 4128 Start */
        /* +COVERITY 10865 */
        ueContextPresentFlag = FALSE;
        /* +COVERITY 10865 */

        /* SPR 4128 End */
        /* SPR 9685 changes start */
        harqProcess_p = PNULL;
        /* SPR 9685 changes end */

        if (PNULL == ccchQueueNode_p)
        {
            /* SPR 16766 fix start */
            LOG_MAC_MSG( MAC_NULL_NODE, LOGWARNING, MAC_CCCH,
                    getCurrentTick(), __LINE__, 0,0,0,0, 0,0,
                    __func__, "ccchQueue_g" );
            /* SPR 16766 fix end */
            LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                   "[%s]: ERROR  CCCH Queue node is NULL\n",
                   __func__);
            return;
        }
        /* SPR 9685 changes start */
        tempCcchQueueNode_p = (CCCHQueueNode*)
            getNextNode((const LTE_SNODE *)ccchQueueNode_p);
        /* SPR 9685 changes end */
        rntiInfo_p = getUeIdxFromRNTIMap(ccchQueueNode_p->rnti, internalCellIndex);
        /*Cyclomatic Complexity changes - starts here*/
        if (MAC_FAILURE == validateCcchQueueNodeForCRNTI(ccchQueueNode_p,rntiInfo_p,
                                                        internalCellIndex))
        {
            /* SPR 9685 changes start */
            ccchQueueNode_p = tempCcchQueueNode_p;
            /* SPR 9685 changes end */
            queueCount--;
            continue;
        }

        if (CRNTI == rntiInfo_p->rntiFlag)
        {
            ueContextPresentFlag = TRUE;
            ueIndex = rntiInfo_p->index;
            ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            ueContext_p = ueContextInfo_p->dlUEContext_p;
        }
        /*Cyclomatic Complexity changes - ends here*/

        /* SPR 23490 Fix +- */
        /* SPR 9685 changes start */

        if (TRUE == ueContextPresentFlag && 
                FALSE == ccchQueueNode_p->rejFlag)
        {
            /* + SPR 11469 Changes */
            if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
            {
                dlHarqContext_p = ueContext_p->dlHarqContext_p;
            }
            else
            {
                dlHarqContext_p= ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
            }
            harqProcess_p = checkForFreeHarqProcess(ueContext_p, 
                    dlHarqContext_p, DRA_SCHEDULING,
                    internalCellIndex);
            /* - SPR 11469 Changes */
            if(PNULL == harqProcess_p)
            {
                ccchQueueNode_p = (CCCHQueueNode *) \
                                  popNode(ccchQueue_gp[internalCellIndex]);
                if (ccchQueueNode_p)
                {
                    if (ccchQueueNode_p->msg_p)
                    {
                        msgFree(ccchQueueNode_p->msg_p);
                    }
                    freeMemPool(ccchQueueNode_p);
                    ccchQueueNode_p = PNULL;
                }
                queueCount--;
                continue;
            }
        }
        /* SPR 9685 changes end */

        /*If CCE are available in UE specific search space allocate from
         *UE specific search space, otherwise allocate from common  
         *search space
         */
        if ( *availableCCE_p )
        {
            /* ICIC changes start */
            aggregationLevel = 
                dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1A_FOR_CCCH];
            /* ICIC changes end */

            if (aggregationLevel <= *availableCCE_p)
            {
                buffLen = ccchQueueNode_p->msgLen;
                
                /*If length of the data is greater than 128 use three byte
                 *header otherwise use two byte header
                 */
                /*If buffer length is less than 128 Use two byte header field
                 *Use three byte header field
                 */
                (buffLen < 128)?(headerLen = TWO_BYTE_HEADER):\
                                (headerLen = THREE_BYTE_HEADER); 

                tbSizeFound = buffLen + headerLen;

                UInt32 x = tbSizeFound;
                /* Bandwidth Fix */
                if(MIN_REQD_RB_FOR_CCCH > (dlRAInfo_gp[internalCellIndex] + \
                            dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB)
                {
                    /* SPR 5999 changes start */
                    numRB = DETERMINE_RBs_DL(MAX_MCS_INDEX_FOR_CCCH, x, internalCellIndex);
                    /* SPR 5999 changes end */
                }
                else
                {
                    /* SPR 5999 changes start */
                    numRB = DETERMINE_RBs_DL(MCS_INDEX_FOR_CCCH, x, internalCellIndex);
                    /* SPR 5999 changes end */
                }
                /* Bandwidth Fix */
                tbSizeFound = x;

                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    /*Soft Buffer Changes Start*/
                    if (TRUE == ueContextPresentFlag && 
                            FALSE == ccchQueueNode_p->rejFlag&&
                            tbSizeFound > ueContext_p->permissibleTBSize)
                    {
                        /* SPR 9685 changes start */
                        harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                        harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                        /* SPR 9685 changes end */
                        /* SPR 16766 fix start */
                        LOG_MAC_MSG( MAC_DROP_CCCH_NODE, LOGWARNING, MAC_CCCH,
                                getCurrentTick(), __LINE__, 
                                ueContext_p->ueIndex, 
                                ueContext_p->permissibleTBSize,
                                ueContextPresentFlag, ccchQueueNode_p->rejFlag,
                                0,0, __func__, "" );
                        LOG_MAC_MSG( MAC_DROP_CCCH_NODE_1, LOGWARNING, MAC_CCCH,
                                getCurrentTick(), ccchQueueNode_p->rnti, numRB,
                                *availableDLRBCount_p, tbSizeFound, 0, 0,0,
                                __func__, "" );
                        /* SPR 16766 fix end */
                        return;
                    }
                    /*Soft Buffer Changes End*/

                    UInt32        count = 0;

                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    /* ICIC changes start */
                    dlMsgInfo_p = 
                      &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
                    /* ICIC changes end */

                    /*Allocate CCE*/
                    retVal = allocateCCEInUESpecificSpaceForCCCH(
                                   (rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex] + subFrameNum),
                                   DCI_FORMAT_1A_FOR_CCCH,
                                   availableCCE_p, 
                                   dciCCEInfo_p,
                                   pdcchDLOutputInfo_p,
                                   ccchQueueNode_p->rnti,
                                   /*pdcch for scell changes start*/
                                   ueContext_p,
                                   dciCCEContainerInfo_p,
                                   /*pdcch for scell changes end*/
                                   internalCellIndex);
                    if ( MAC_SUCCESS == retVal )
                    {
                        /*In case of Connection reject message no need to use
                         *HARQ retrans
                         */
                        if (TRUE == ueContextPresentFlag && 
                            FALSE == ccchQueueNode_p->rejFlag)
                        {
                            /* + SPR 11491 Changes */
                            if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
                            {
                                ueContext_p->isPdcchAllocatedFlag = TRUE;
                            }
                            else
                            {
                                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = TRUE;
                            }
                            /* - SPR 11491 Changes */

                            FILL_HARQ_INFO(ueContext_p,harqProcess_p,
                                               dlMsgInfo_p,tbSizeFound,numRB,internalCellIndex);
//SPR 8546 FIX 
#ifdef TDD_CONFIG
                            UInt8 isSplSubFrame = 0;
                            isSplSubFrame = checkForSpecialSubframe(subFrameNum, internalCellIndex);
                            setSubFrameTypeForDlHarq(isSplSubFrame,harqProcess_p);
#endif

                            dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.ueIndex = ueIndex;
                        }
                        else
                        {
                            dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.harqProcessId = INVALID_HARQ_ID;
                                /* Fix SPR 21492 start*/ 
							dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.ueIndex = ccchQueueNode_p->rnti;
                                /* Fix SPR 21492 end*/
                        }
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
                        /*SPR 21589 Fix  ++*/
                        /*Code removed*/
                        /*SPR 21589 Fix  --*/
                        /*Add padding header and normal header*/
                        transmitBlock_p = formTB(buffLen, &headerLen, tbSizeFound);

                        if (PNULL != transmitBlock_p)
                        {
                            /*SPR 21589 Fix  ++*/
                            dciCCEContainerInfo_p->countOfCCEAllocated++;
                            /*SPR 21589 Fix  --*/
                            fillCCCHInfo(
                                         pdcchDLOutputInfo_p, 
                                         ccchQueueNode_p, 
                                         transmitBlock_p, numRB, headerLen,
                                         tbSizeFound);
                            /*Decrement the number of DL RBs available*/
                            *availableDLRBCount_p -= numRB;
                            if (harqProcess_p){
                                harqProcess_p->transmissionBuffer_p = 
                                                            transmitBlock_p;
                                /* SPR 2051 changes start */
                                harqProcess_p->totalTBSize = tbSizeFound;  
                                /* SPR 2051 changes end */
                                /*Soft Buffer Changes Start*/
                                ueContext_p->permissibleTBSize -= tbSizeFound;
                                /*Soft Buffer Changes End*/

                            }
                        }
                        /* SPR 9685 changes start */
                        else
                        {
                            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl, dciCCEInfo_p, internalCellIndex);
                            if(harqProcess_p)
                            {    
                                harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                                harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                            }
                            LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                                    "[%s]: ERROR formTB() returned failure\n",
                                    __func__);
                            return;
                        }
                        /* SPR 9685 changes end */


                        /* 5492 changes start */
                        (*maxUeScheduledInDownlink_p)--;
                        /* 5492 changes end */
                        maxCCCHUe--;
                    }
                    else
                    {
                        /* SPR 16766 fix start */
                        LOG_MAC_MSG( MAC_CCCH_CCE_UNAVAILABLE, LOGWARNING, 
                                MAC_CCCH, getCurrentTick(), __LINE__, 
                                ccchQueueNode_p->rnti, *availableCCE_p,
                                0, 0,  0,0, __func__, "UeSpecific" );
                        /* SPR 16766 fix end */
                        allocFromCommon = TRUE;
                    }
                }
                else
                {
                    /* SPR 9685 changes start */
                    if(harqProcess_p)
                    {    
                        harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                        harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                    }
                    /* SPR 9685 changes end */
                    /* SPR 16766 fix start */
                    LOG_MAC_MSG( MAC_DROP_CCCH_NODE_1, LOGWARNING, MAC_CCCH,
                            getCurrentTick(), ccchQueueNode_p->rnti, numRB, 
                            *availableDLRBCount_p, tbSizeFound, 0, 0,0,
                            __func__, "" );
                    /* SPR 16766 fix end */
                    return;
                }
            }
            else
            {
                /* SPR 16766 fix start */
                LOG_MAC_MSG( MAC_AGG_LVL_MORE_THAN_AV_CCE, LOGWARNING, MAC_CCCH,
                        getCurrentTick(), __LINE__, ccchQueueNode_p->rnti, 
                        aggregationLevel, *availableCCE_p, 0, 0,0,
                        __func__, "UeSpecific" );
                /* SPR 16766 fix end */
                allocFromCommon = TRUE;
            }
        }
        /* SPR 4128 Start */
        else
        {
            allocFromCommon = TRUE;
        }
        /* SPR 4128 End */

        if (TRUE  == allocFromCommon)
        {
            if (*availableCommonCCE_p >= AGGREGATION_LEVEL_FOR_COMMON_SPACE)
            {
                aggregationLevel = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
    
                /*Calculate the lenght of the buffer including the 
                 *header length
                 */
                buffLen = ccchQueueNode_p->msgLen;
    
                /*If length of the data is greater than 128 use three byte
                 *header otherwise use two byte header
                 */
                /*If buffer length is less than 128 Use two byte header field
                 *Use three byte header field
                 */
                (buffLen < 128)?(headerLen = TWO_BYTE_HEADER):\
                                (headerLen = THREE_BYTE_HEADER); 

                tbSizeFound = buffLen + headerLen;
                UInt32 x = tbSizeFound;
                /* Bandwidth Fix */
                if(MIN_REQD_RB_FOR_CCCH > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])-> totalAllowedRB)
                {
                    /* SPR 5999 changes start */
                    numRB = DETERMINE_RBs_DL(MAX_MCS_INDEX_FOR_CCCH, x, internalCellIndex);
                    /* SPR 5999 changes end */
                }
                else
                {
                    /* SPR 5999 changes start */
                    numRB = DETERMINE_RBs_DL(MCS_INDEX_FOR_CCCH, x, internalCellIndex);
                    /* SPR 5999 changes end */
                }
                /* Bandwidth Fix */
                tbSizeFound = x;

                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    /*Soft Buffer Changes Start*/
                    if (TRUE == ueContextPresentFlag && 
                            FALSE == ccchQueueNode_p->rejFlag&&
                            tbSizeFound > ueContext_p->permissibleTBSize)
                    {
                        /* SPR 16766 fix start */
                        LOG_MAC_MSG( MAC_DROP_CCCH_NODE, LOGWARNING, MAC_CCCH,
                                getCurrentTick(), __LINE__, 
                                ueContext_p->ueIndex, 
                                ueContext_p->permissibleTBSize,
                                ueContextPresentFlag, ccchQueueNode_p->rejFlag,
                                0,0, __func__, "" );
                        LOG_MAC_MSG( MAC_DROP_CCCH_NODE_1, LOGWARNING, MAC_CCCH,
                                getCurrentTick(), ccchQueueNode_p->rnti, numRB,
                                *availableDLRBCount_p, tbSizeFound, 0, 0,0,
                                __func__, "" );
                        /* SPR 16766 fix end */
                        return;
                    }
                    /*Soft Buffer Changes End*/

                    UInt32        count = 0;
    
                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    /* ICIC changes start */
                    dlMsgInfo_p = 
                      &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
                    /* ICIC changes end */
    
                    /*Allocate CCE*/
                    retVal = allocateCCEInCommonSpaceForCCCH(aggregationLevel,
                                                      availableCCE_p,
                                                      dciCCEInfo_p,
                                                      pdcchDLOutputInfo_p,
                                                      ccchQueueNode_p->rnti,
                                                      DCI_FORMAT_1A_FOR_CCCH,
                                                      internalCellIndex);
                    if ( MAC_SUCCESS == retVal )
                    {
    
                        /*In case of Connection reject message no need to use
                         *HARQ retrans
                         */
                        /* Coverity 5.0.1 ID:24911 fix start*/
                        if (TRUE == ueContextPresentFlag &&
                            FALSE == ccchQueueNode_p->rejFlag)
                        {
			    /* Coverity 5.0.1 ID:24911 fix end*/
                            /* + SPR 11491 Changes */
                            if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
                            {
                                ueContext_p->isPdcchAllocatedFlag = TRUE;
                            }
                            else
                            {
                                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = TRUE;
                            }
                            /* - SPR 11491 Changes */
    
                            FILL_HARQ_INFO(ueContext_p,harqProcess_p,
                                           dlMsgInfo_p,tbSizeFound,numRB,internalCellIndex);

                            dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.ueIndex = ueIndex;
                        }
                        else
                        {
                            dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.harqProcessId = INVALID_HARQ_ID;
                            /* Fix SPR 21492 start*/
							dciCCEContainerInfo_p->\
                                dciCCEInfo[dlMsgInfo_p->dlCCCHMsgInfo.cceIndex].\
                                dciPduInfo.ueIndex =ccchQueueNode_p->rnti;
                            /* Fix SPr 21492 end*/ 
                        }
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;

                        /*SPR 21589 Fix  ++*/
                        /*code removed*/
                        /*SPR 21589 Fix  --*/
                        /*Add padding header and normal header*/
                        transmitBlock_p = formTB(buffLen,
                                                 &headerLen,
                                                 tbSizeFound);
                        if (PNULL != transmitBlock_p)
                        {
                            /*SPR 21589 Fix  ++*/
                            dciCCEContainerInfo_p->countOfCCEAllocated++;
                            /*SPR 21589 Fix  --*/
                            fillCCCHInfo(
                                         pdcchDLOutputInfo_p, 
                                         ccchQueueNode_p, 
                                         transmitBlock_p, numRB, headerLen,
                                         tbSizeFound);
    
                            /*Decrement the number of DL RBs available*/
                            *availableDLRBCount_p -= numRB;
                            /*Decrement the available CCE from Common space*/
                            *availableCommonCCE_p -= aggregationLevel;
    
                            if (harqProcess_p){
                                harqProcess_p->transmissionBuffer_p = 
                                                            transmitBlock_p;
                                /* SPR 2051 changes start */
                                harqProcess_p->totalTBSize = tbSizeFound;  
                                /* SPR 2051 changes end */
                                /*Soft Buffer Changes Start*/
                                ueContext_p->permissibleTBSize -= tbSizeFound;
                                /*Soft Buffer Changes End*/
                            }
                        }
                        else
                        {
                            /* SPR 9685 changes start */
                            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                                    dciCCEInfo_p, internalCellIndex);
                            if(harqProcess_p)
                            {    
                                harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                                harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                            }
                            /* SPR 9685 changes end */
                            LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                                "[%s]: ERROR formTB() returned failure\n",
                                __func__);
                            return;
                        }
                        /* 5492 changes start */
                        (*maxUeScheduledInDownlink_p)--;
                        /* 5492 changes end */
                        maxCCCHUe--;
                    }
                    else
                    {
                        /* SPR 9685 changes start */
                        if(harqProcess_p)
                        {    
                            harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                            harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                        }
                        /* SPR 9685 changes end */
                        /* SPR 16766 fix start */
                        LOG_MAC_MSG( MAC_CCCH_CCE_UNAVAILABLE, LOGWARNING, 
                                MAC_CCCH, getCurrentTick(), __LINE__, 
                                ccchQueueNode_p->rnti, *availableCCE_p,
                                0, 0,  0,0, __func__, "CommonSpace" );
                        /* SPR 16766 fix end */
                        return;
                    }
                }
                else
                {
                    /* SPR 9685 changes start */
                    if(harqProcess_p)
                    {    
                        harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                        harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                    }
                    /* SPR 9685 changes end */
                    /* SPR 16766 fix start */
                    LOG_MAC_MSG( MAC_DROP_CCCH_NODE_1, LOGWARNING, MAC_CCCH,
                            getCurrentTick(), ccchQueueNode_p->rnti, numRB, 
                            *availableDLRBCount_p, tbSizeFound, 0, 0,0,
                            __func__, "" );
                    /* SPR 16766 fix end */
                    return;
                }
            }
            else
            {
                /* SPR 9685 changes start */
                if(harqProcess_p)
                {    
                    harqProcess_p->harqStatus = HARQ_PROCESS_FREE;
                    harqProcess_p->harqProcessId = INVALID_HARQ_ID; 
                }
                /* SPR 16766 fix start */
                LOG_MAC_MSG( MAC_AGG_LVL_MORE_THAN_AV_CCE, LOGWARNING, MAC_CCCH,
                        getCurrentTick(), __LINE__, ccchQueueNode_p->rnti, 
                        AGGREGATION_LEVEL_FOR_COMMON_SPACE, 
                        *availableCommonCCE_p, 0, 0,0,
                        __func__, "UeSpecific" );
                /* SPR 16766 fix end */
                /* SPR 9685 changes end */
                return;
            }
        }
        /* SPR 4128 Start */
        /* SPR 4128 End */
        /*Cyclomatic Complexity changes - starts here*/
        updatePdcchULOutputInfoForSuccessFullPdcchAlloc(ueIndex,
                /* +- SPR 17777 */
                dlMsgInfo_p,
                pdcchULOutputInfo_p,
                retVal,
                ccchQueueNode_p
                /** SPR 14204 Fix : Start **/
#ifdef TDD_CONFIG
                ,subFrameNum
                ,internalCellIndex
#endif
                /* +- SPR 17777 */
                /** SPR 14204 Fix : End **/
                );
        /*Cyclomatic Complexity changes - ends here*/
        ccchQueueNode_p = (CCCHQueueNode *) popNode(ccchQueue_gp[internalCellIndex]);
        if (ccchQueueNode_p)
        {
            /*Cyclomatic Complexity changes - starts here*/
            updateCCCHQueueNodeForTCRNTI(ccchQueueNode_p,
                                         rntiInfo_p,
                                         internalCellIndex);
            /*Cyclomatic Complexity changes - ends here*/
            freeMemPool(ccchQueueNode_p);
            ccchQueueNode_p = PNULL;
        }
        /* SPR 9685 changes start */
        ccchQueueNode_p = tempCcchQueueNode_p;
        /* SPR 9685 changes end */
        queueCount--;
    }
}

/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : validateCcchQueueNodeForCRNTI
 * Inputs         : ccchQueueNode_p - Pointer to CCCHQueueNode,
 *                  rntiInfo_p - Pointer to RNTIInfo
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will Validate the CCCH Queue node for CRNTI.
 *****************************************************************************/
static  MacRetType validateCcchQueueNodeForCRNTI(CCCHQueueNode *ccchQueueNode_p,
                                                       RNTIInfo *rntiInfo_p,
                                                       InternalCellIndex internalCellIndex)
{
    UInt32 msgFreeFlag          = FALSE;
    DLUEContextInfo *ueContextInfo_p = PNULL;
    UInt32 ueIndex          = MAX_UE_SUPPORTED;

    /*SPR 4017 PH2 Start*/
    if(rntiInfo_p)
    {
        if (CRNTI == rntiInfo_p->rntiFlag)
        {
            ueIndex = rntiInfo_p->index;
            ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

            /* SPR#706 Changes Start */
            if ( (ueContextInfo_p->pendingDeleteFlag)||
                 ( (PNULL == ueContextInfo_p->dlUEContext_p) && 
                 (FALSE == ccchQueueNode_p->rejFlag) ))
            /* SPR#706 Changes End */
            {
                LTE_MAC_UT_LOG(LOG_WARNING,CCCH_MGR,
                            "[%s]: Pending delete flag is set for UE Id [%d]\n",
                            __func__,ccchQueueNode_p->rnti);
                msgFreeFlag = TRUE;
            }
        }
    }
    /*SPR 4017 PH2 End*/
    /* +coverity_10736 */
    else
    {
        msgFreeFlag = TRUE;
    }
    /* -coverity_10736 */

    if (TRUE == msgFreeFlag)
    {
        ccchQueueNode_p = (CCCHQueueNode *) popNode(ccchQueue_gp[internalCellIndex]);
        if (ccchQueueNode_p)
        {
            if (ccchQueueNode_p->msg_p)
            {
                msgFree(ccchQueueNode_p->msg_p);
            }
            freeMemPool(ccchQueueNode_p);
        }
        
	return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : updatePdcchULOutputInfoForSuccessFullPdcchAlloc
 * Inputs         : ueIndex - Index of UE,
 *                  subFrameNum - Sub frame number w.r.t DL delay,
 *                  dlMsgInfo_p - Pointer to DLMsgInfo,
 *                  retVal,
 *                  ccchQueueNode_pp - Pointer to CCCHQueueNode               
 * Outputs        : pdcchDLOutputInfo_p - Pointer to PdcchDLOutputInfo
 * Returns        : None
 * Description    : This will will update the PDCCH UL output information
 *                  for successfull allocations of PDCCH
 *****************************************************************************/
static  void updatePdcchULOutputInfoForSuccessFullPdcchAlloc(UInt32 ueIndex,
        /* +- SPR 17777 */
        DLMsgInfo *dlMsgInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 retVal,
        CCCHQueueNode *ccchQueueNode_p
        /** SPR 14204 Fix : Start **/
#ifdef TDD_CONFIG
        ,UInt32 subFrameNum
        ,InternalCellIndex internalCellIndex
#endif
        /* +- SPR 17777 */
        /** SPR 14204 Fix : End **/
                                                        )
{
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    /* +- SPR 18268 */
    UInt16 tempUECount = 0;
    /* +- SPR 18268 */
    if (MAC_SUCCESS == retVal)
    {
         if(MAX_UE_SUPPORTED > ueIndex)
         {
#ifdef TDD_CONFIG
             /** SPR 14204 Fix : Start **/

             /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
             if(setCCEIndexInULContex(ueIndex, subFrameNum, dlMsgInfo_p->dlCCCHMsgInfo.cceIndex,internalCellIndex,
                                       1,0))

                 /* SPR 19679: TDD HARQ Multiplexing  Changes End */
             /** SPR 14204 Fix : End **/
             {
                 pdcchULOutputInfo_p->pdcchDLUEInfo.pdcchDLUEArray[pdcchULOutputInfo_p->pdcchDLUEInfo.count] 
                                                            = ueIndex;
                 pdcchULOutputInfo_p->pdcchDLUEInfo.count += 1;
    
                 pdcchULOutputInfo_p->pdcchDLUEInfo.pdcchDLUEArrayForDlGroup
                       [pdcchULOutputInfo_p->pdcchDLUEInfo.cumulativeUeCountForDlGroup] = ueIndex;
                 pdcchULOutputInfo_p->pdcchDLUEInfo.cumulativeUeCountForDlGroup =+ 1;
             }
#elif FDD_CONFIG
             /*SPR 953 changes start*/
             pdcchULOutputInfo_p->pdcchDLUEInfo.pdcchDLUEArray[pdcchULOutputInfo_p->pdcchDLUEInfo.count] 
                                                                      = ueIndex;
             pdcchULOutputInfo_p->pdcchDLUEInfo.count += 1 ;

             pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex = 
                        dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
             /*SPR 1551 changes start*/
             pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlDciFormat =          
                    dlMsgInfo_p->dlCCCHMsgInfo.dciFormat; 
             /*SPR 1551 changes end*/
             /*SPR 953 changes end*/
#endif
        }
        else
        {
            /* To Send UCI HARQ in UL Config for UE with 
             * temporary context */
            /* + SPR 4301 changes */
#ifdef FDD_CONFIG    
            tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

            tempUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
#elif TDD_CONFIG
            tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchULUEArrayForTempUlGroup; 
            tempUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup;
#endif
            tcrntiCCEIndexInfo_p[tempUECount].cceIndex = 
                dlMsgInfo_p->dlCCCHMsgInfo.cceIndex;
#ifdef TDD_CONFIG
            tcrntiCCEIndexInfo_p[tempUECount].dlSubFrame = subFrameNum;
#endif        
            tcrntiCCEIndexInfo_p[tempUECount].tcrnti = ccchQueueNode_p->rnti;
#ifdef FDD_CONFIG    
            pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count++;
#elif TDD_CONFIG
            pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup++;
#endif
            /* - SPR 4301 changes */
        }
    }
}

/*****************************************************************************
 * Function Name  : updateCCCHQueueNodeForTCRNTI
 * Inputs         : ccchQueueNode_p - Pointer to CCCHQueueNode,
 *                  rntiInfo_p - Pointer to RNTIInfo
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : None
 * Returns        : None
 * Description    : This will update the CCCH Queue node for TCRNTI.
 *****************************************************************************/
STATIC  void updateCCCHQueueNodeForTCRNTI(CCCHQueueNode *ccchQueueNode_p,
                                                RNTIInfo *rntiInfo_p,
                                                InternalCellIndex internalCellIndex)
{
    if (TCRNTI == rntiInfo_p->rntiFlag)
    {
        void *tempUEContex_p = PNULL; 
        {
            tempUEContex_p = (void*)getTempCRNTIContextPointer(ccchQueueNode_p->rnti, internalCellIndex);
        }

        /* Currently there is no HARQ retransmission support of UE with 
         * temporary Context.When HARQ retransmission support is added 
         * deletion of temporary context should be done after reception 
         * of ACK or MAX retransmission of Connection Reject */

        if ((tempUEContex_p) &&
            (TRUE == ccchQueueNode_p->rejFlag))
        {
            /* SPR 5235 : Start*/
            PDCCHRachNode *pdcchRachNode_p = PNULL;
	    GET_MEM_FROM_POOL(PDCCHRachNode,
				   pdcchRachNode_p,
				   sizeof(PDCCHRachNode), PNULL);
	    if(PNULL != pdcchRachNode_p)
	    {
	        pdcchRachNode_p->rnti = ccchQueueNode_p->rnti;
                /*SPR 21103 Start*/
                if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(FREE_TCRNTI, pdcchRachNode_p,
                            internalCellIndex))
                {
                    freeMemPool(pdcchRachNode_p);
                }
                /*SPR 21103 Start*/
            }
            /* SPR 5235 : End*/
        }
    }
            
    if(ccchQueueNode_p->msg_p)
    {
        msgFree(ccchQueueNode_p->msg_p);
        ccchQueueNode_p->msg_p = PNULL;
    }
}

/*Cyclomatic Complexity changes - ends here*/
/*********************************************************************************
 * Function Name  : formTB
 * Inputs         : buffLen - Length of the message including header length,
 *                  headerType_p - Header type (Two byte or three byte header),
 *                  tbSizeFound - TB size found from the Table given in 36.213
 * Outputs        : headerType_p - Length of the header field allocated
 * Returns        : orgMsg_p - contains the header
 * Description    : This will decide the number of bytes padding to be added and 
 *                  allocated mem and fills the header fields.
 *********************************************************************************/
UInt8* formTB(UInt32 buffLen,
              UInt32 *headerType_p,
              UInt32 tbSizeFound)
{
    UInt8  *msg_p    = PNULL;
    UInt8  *orgMsg_p = PNULL;
    UInt32 paddingBytes = 0;

    /*If zero buffer copy not used allocate the memory for 
     *Total TB, Otherwise allocate memory for only header
     */
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */
    orgMsg_p = msgAlloc(PNULL,0,0,0);
    msgReserve(orgMsg_p,0, (tbSizeFound + DATA_MSG_API_HEADER_LEN));
    msg_p = msgSegNext(orgMsg_p,0,&segment);
    msg_p = segment.base;
    msg_p += DATA_MSG_API_HEADER_LEN;
    paddingBytes = (tbSizeFound - (buffLen + *headerType_p));

    if (!paddingBytes)
    {
        if (TWO_BYTE_HEADER == *headerType_p)
        {
            *headerType_p = 2;

            /*One byte padding sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 1, MAC_PADDING_LCH);
                
            /*One byte CCCH LCH sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 0, MAC_UL_CCCH_LCH);
        }
        else if (THREE_BYTE_HEADER == *headerType_p)
        {
            *headerType_p = 3;

            /*One byte padding sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 1, MAC_PADDING_LCH);

            /*One byte padding sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 1, MAC_PADDING_LCH);
            
            /*One byte CCCH LCH sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 0, MAC_UL_CCCH_LCH);
        }
    }
    else if (1 == paddingBytes)
    {
        if (TWO_BYTE_HEADER == *headerType_p)
        {
            *headerType_p = 3;

            /*One byte padding sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 1, MAC_PADDING_LCH);
            
            /*One byte padding sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 1, MAC_PADDING_LCH);
                
            /*One byte CCCH LCH sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 0, MAC_UL_CCCH_LCH);
        }
        else if (THREE_BYTE_HEADER == *headerType_p)
        {
            *headerType_p = 4;

            /*Three byte subheader*/
            MAC_SUBHEADER_THREE_BYTE(msg_p, 1, buffLen, MAC_UL_CCCH_LCH);

            /*One byte padding sub header*/
            MAC_SUBHEADER_ONE_BYTE(msg_p, 0, MAC_PADDING_LCH);
        }
    }
    else if (paddingBytes >= 2)
    {
        *headerType_p = 3;

        /*Two byte subheader*/
        MAC_SUBHEADER_TWO_BYTE(msg_p, 1, buffLen, MAC_UL_CCCH_LCH);

        /*One byte padding sub header*/
        MAC_SUBHEADER_ONE_BYTE(msg_p, 0, MAC_PADDING_LCH);
    }

    return orgMsg_p;
}

/*****************************************************************************
 * Function Name  : sendCCCHMsgToRRC
 * Inputs         : msg_p - CCCH message buffer,
 *                  dataLen - Length of the CCCH message data,
 *                  rnti - UE ID,
 *                  timingAdvance,
 *                  sinr
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will create the rrcInfoData node and 
 *                  calls sendMsgToRRC() which sends the message to RRC.
 *****************************************************************************/
 void sendCCCHMsgToRRC(void  *msg_p,
                             UInt32 dataLen,
                             UInt16 rnti,
                             UInt16 timingAdvance,
                             UInt8 sinr,
                             InternalCellIndex internalCellIndex)
{
    rrcInfoData  *rrcData_p = PNULL;
    RRCReqAPI msgType = MAC_CCCH_MSG_IND;

    LTE_MAC_UT_LOG(LOG_DETAIL,CCCH_MGR,
            "Entry [%s]\n",__func__);

    GET_MEM_FROM_POOL(rrcInfoData , rrcData_p , sizeof(rrcInfoData) ,PNULL);
    /* +coverity-530-CID 24491*/
    if(PNULL == rrcData_p)
    {
        return;
    }
    /* +coverity-530-CID 24491*/


    /*Populate the rrcData_p with the input parameters received*/
    rrcData_p->msgType  = msgType;
    rrcData_p->dataLen  = (UInt16)dataLen;
    rrcData_p->rnti     = rnti;
    /*CLPC Change Begin*/
    rrcData_p->timingAdvance = timingAdvance;
    rrcData_p->sinr = sinr;
    /*CLPC Change End*/
    /*Calling function NULL chek the msg_p*/
    rrcData_p->buffer_p = msg_p;

    LTE_MAC_UT_LOG(LOG_DETAIL,CCCH_MGR,
            "[%s]: Sending CCCH message to RRC. rnti [%d] msgType [%d]"
            "dataLen [%d]\n",__func__,rrcData_p->rnti,
            rrcData_p->msgType,rrcData_p->dataLen);
    /* + SPR 24051 changes */
    /*Below both flag are used for the INTEL*/
    /*Call the RRC interface API to send the data to RRC*/
    if(MAC_SEND_ERROR == sendToRRC(PNULL, PNULL, msgType, rrcData_p,
                macCommonGetRRCCellIndex(internalCellIndex)
          /*SPR 4870 Changes Start*/
                /*SPR 16855 +-*/
                ,PNULL,PNULL
                /*SPR 16855 +-*/
          /*SPR 4870 Changes End*/ 
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        msgFree(rrcData_p->buffer_p);
        
        LOG_MAC_MSG(MAC_SEND_CCCH_TO_RRC_ID, LOGWARNING, MAC_CCCH,
                getCurrentTick(),
                rnti,
                dataLen, 
                timingAdvance, 
                sinr,
                internalCellIndex,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "SEND FAIL" );
    }
    else
    {       
        LOG_MAC_MSG(MAC_SEND_CCCH_TO_RRC_ID, LOGWARNING, MAC_CCCH,
                getCurrentTick(),
                rnti,
                dataLen, 
                timingAdvance, 
                sinr,
                internalCellIndex,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "SEND SUCC" );
    }

    /* SPR 924 Start */    
    freeMemPool(rrcData_p);
    /* SPR 924 End */    
    LTE_MAC_UT_LOG(LOG_DETAIL,CCCH_MGR,
            "[%s]: sendToRRC() with msgId [%d]\n",
            __func__,MAC_CCCH_MSG_IND);

    LTE_MAC_UT_LOG(LOG_DETAIL,CCCH_MGR,
            "Exit [%s]\n",__func__);
}

/*****************************************************************************
 * Function Name  : allocateCCEInCommonSpaceForCCCH
 * Inputs         : aggregationLevel - agregation level,
 *                  availableCCE_p  - Available CCE,
 *                  rnti - RNTI value received,
 *                  dciFormat - DCI format
 *                  internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs        : dciCCEInfo_p - Pointer to DciCCEInfo,
 *                  pdcchDLOutputInfo_p - Pointer to the PdcchDLOutputInfo
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will allocate the CCE from common space.
 *****************************************************************************/
MacRetType allocateCCEInCommonSpaceForCCCH(UInt32    aggregationLevel,
                                          UInt32     *availableCCE_p,
                                          DciCCEInfo *dciCCEInfo_p,
                                          PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                                          UInt16     rnti,
                                          DCIFormat  dciFormat,
                                          InternalCellIndex internalCellIndex)
{
    UInt32 cceAllocated = INVALID_CCE_INDEX;
    /*coverity-530 CID-32299*/
    MacRetType macRetType   = MAC_FAILURE;
    /*coverity-530 CID-32299*/
    UInt32 count        = 0;
    DLMsgInfo  *dlMsgInfo_p  = PNULL;

    /* SPR 6856 Changes start */
    /* Try allocating CCE in common search space */
    macRetType = allocatePDCCHForDCIFormatCommonSpace (
			  	             aggregationLevel,&cceAllocated, 
			  	             availableCCE_p, dciCCEInfo_p,
                             internalCellIndex);
    /* SPR 6856 Changes end */

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
            "[%s]: CCE allocated successfully cceAllocated [%d]\n",
            __func__,cceAllocated);
        
        count = pdcchDLOutputInfo_p->countCommonChannelMsg;
        /* ICIC changes start */
        dlMsgInfo_p = &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
        /* ICIC changes end */
        
        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = aggregationLevel;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = rnti;
        dciCCEInfo_p[cceAllocated].dciPduInfo.resrcAllocType = RA_POLICY_2_LOCALIZED;
        dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = NEW_TX;

        /* SPR 6855 Changes start */
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
        /* SPR 6855 Changes end */

        /* ICIC changes start */
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = CCCH_MSG;
        /* ICIC changes end */
        dlMsgInfo_p->dlCCCHMsgInfo.dciFormat  = dciFormat;
        dlMsgInfo_p->dlCCCHMsgInfo.ueId      = rnti;
        dlMsgInfo_p->dlCCCHMsgInfo.cceIndex  = cceAllocated;
        dlMsgInfo_p->dlCCCHMsgInfo.dciFormat = dciFormat;
        /* SPR 13388 changes start */
        if(MIN_REQD_RB_FOR_CCCH > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])-> totalAllowedRB)
        {
            dlMsgInfo_p->dlCCCHMsgInfo.resourceAllocInput.mcsIndexForCommonChannel =\
                   MAX_MCS_INDEX_FOR_CCCH;
        }
        else
        {
            dlMsgInfo_p->dlCCCHMsgInfo.resourceAllocInput.mcsIndexForCommonChannel =\
                   MCS_INDEX_FOR_CCCH;
        }
        /* SPR 13388 changes end */
        /*SPR 9203 Fix Starts*/
        dlMsgInfo_p->dlCCCHMsgInfo.harqProcessId = INVALID_HARQ_ID; 
        /*SPR 9203 Fix Ends*/
      
        pdcchDLOutputInfo_p->countCommonChannelMsg++;
        /* 5492 changes start */
        /* 5492 changes end */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
                "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                __func__,aggregationLevel);
    }

    return macRetType;
}

/*******************************************************************************************
 * Function Name    : allocateCCEInUESpecificSpaceForCCCH
 * Inputs           : ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo
 *                                          structure which holds the CCEs Index for a UE
 *                                          at each of the aggregation level,
 *                    dciFormat - This holds the DCI Format for which PDCCH region has to
 *                                be allocated,
 *                    rnti - RNTI value
 *                    ueContext_p -  Pointer to UE context
 *                    dciCCEContainerInfo_p -
 *                    internalCellIndex - specific cellIndex for which 
 *                                      function is invoked.
 * Outputs          : availableCCE_p - This represent the number of available CCE
 *                                     and if CCE's are allocated this would be
 *                                     deceremented by  one,
 *                    dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                                   Information is there and once cce is allocated
 *                                   updated information is written on the reserved CCE,
 *                    pdcchDLOutputInfo_p - Ponter to the Pdcch DL output container
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtions will find the aggregation level for the DCI 
 *                    Format and based on aggregation level it call the relevant
 *                    funtions for finding the free CCE.
 *******************************************************************************************/
MacRetType allocateCCEInUESpecificSpaceForCCCH(
                            UESpecificSearchSpaceInfo  *ueSearchSpaceInfo_p,
                            DCIFormat         dciFormat,
                            UInt32            *availableCCE_p, 
                            DciCCEInfo        *dciCCEInfo_p,
                            PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                            UInt16 rnti,
                            /*pdcch for scell changes start*/
                            DLUEContext   *ueContext_p,   
                            DciCCEContainerInfo *dciCCEContainerInfo_p,
                            /*pdcch for scell changes end*/
                            InternalCellIndex internalCellIndex)
{
    UInt32     cceAllocated     = INVALID_CCE_INDEX;
    DLMsgInfo  *dlMsgInfo_p     = PNULL;
    UInt32     count = 0;
    MacRetType macRetType = MAC_FAILURE;
    
    LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
          "Entered in function = %s \n",__func__);

    macRetType = allocatePDCCHForDCIFormatUESpace(
                                                  ueContext_p,
                                                  ueSearchSpaceInfo_p,
                                                  &cceAllocated,
                                                  availableCCE_p, 
                                                  dciCCEInfo_p,
                                                  AGGREGATION_LEVEL_FOR_CCCH_MSG,
                                                  dciCCEContainerInfo_p,
                                                  internalCellIndex);

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
            "[%s]: CCE allocated successfully cceAllocated [%d]\n",
            __func__,cceAllocated);

        count = pdcchDLOutputInfo_p->countCommonChannelMsg;
        /* ICIC changes start */
        dlMsgInfo_p = &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
        
     dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl=  AGGREGATION_LEVEL_FOR_CCCH_MSG;
     /* SPR 3878 FIX END */

        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = rnti;
        dciCCEInfo_p[cceAllocated].dciPduInfo.resrcAllocType = RA_POLICY_2_LOCALIZED;
        dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = NEW_TX;

        /* SPR 6855 Changes start */
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
        /* SPR 6855 Changes end */

        /* ICIC changes start */
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = CCCH_MSG;
        dlMsgInfo_p = 
            &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
        /* ICIC changes end */
        dlMsgInfo_p->dlCCCHMsgInfo.cceIndex   = cceAllocated;
        dlMsgInfo_p->dlCCCHMsgInfo.harqProcessId = INVALID_HARQ_ID;
        dlMsgInfo_p->dlCCCHMsgInfo.dciFormat  = dciFormat;
        /* Bandwidth Fix */
        if(MIN_REQD_RB_FOR_CCCH > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB)
        {
             dlMsgInfo_p->dlCCCHMsgInfo. \
                   resourceAllocInput.mcsIndexForCommonChannel = MAX_MCS_INDEX_FOR_CCCH;
        }
        else
        {
             dlMsgInfo_p->dlCCCHMsgInfo.resourceAllocInput.mcsIndexForCommonChannel =\
                    MCS_INDEX_FOR_CCCH;
        }
        /* Bandwidth Fix */
        pdcchDLOutputInfo_p->countCommonChannelMsg++;
        /* 5492 changes start */
        /* 5492 changes end */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
                "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                __func__,dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][dciFormat]);
        return MAC_FAILURE;
    }

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : fillCCCHInfo
 * Inputs         : pdcchDLOutputInfo_p - Pointer to PdcchDLOutputInfo,
 *                  ccchQueueNode_p - Pointer to CCCHQueueNode,
 *                  transmitBlock_p - Block to be transmitted,
 *                  numRB - Number of RB's,
 *                  headerLen - Length of Header,
 *                  tbSizeFound - TB size found from the Table given in 36.213
 * Outputs        : None
 * Returns        : None
 * Description    : This will prepares the CCCH info
 *******************************************************************************/
void fillCCCHInfo(
                  PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                  CCCHQueueNode     *ccchQueueNode_p, 
                  UInt8  *transmitBlock_p, 
                  UInt8  numRB,
                  UInt32 headerLen,
                  UInt32  tbSizeFound)
{
    UInt8 *msg_p = PNULL;
    DLCCCHMsgInfo *dlCCCHMsgInfo_p = PNULL;

    /*Insert the prepared header at the beggining 
     *of the CCCH message
     */
    void     *ptr = PNULL;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */
    ptr = msgSegNext(ccchQueueNode_p->msg_p, 0, 
                     &segment);
    if (PNULL == ptr)
    {
        ltePanic("msgSegNext() returned NULL");
    }
    /*SPR 1768 - start*/
    else
    {
    /*SPR 1768 - End*/
        msg_p = segment.base;
    }
    
    ptr = msgSegNext(transmitBlock_p, 0, &segment);
    ptr = segment.base;
    /*SPR 4017 PH2 Start*/
    if(ptr && msg_p)
    {
        memCpy((ptr + headerLen +
                DATA_MSG_API_HEADER_LEN),
                msg_p, ccchQueueNode_p->msgLen);
    }
    /*SPR 4017 PH2 End*/
    /* ICIC changes start */
    dlCCCHMsgInfo_p = &pdcchDLOutputInfo_p->\
     dlMsgTypeCommonMsgInfo[(pdcchDLOutputInfo_p->countCommonChannelMsg - 1)].\
     dlMsgInfo.dlCCCHMsgInfo;
    /* ICIC changes end */

    dlCCCHMsgInfo_p->dataLen = tbSizeFound;
    dlCCCHMsgInfo_p->data_p = transmitBlock_p;
    dlCCCHMsgInfo_p->resourceAllocInput.requiredRB = numRB;
    /* SPR 4395 changes start */
    dlCCCHMsgInfo_p->rejFlag = ccchQueueNode_p->rejFlag;  
    /* SPR 4395 changes end */
}


