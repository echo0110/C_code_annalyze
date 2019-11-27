/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacRlcInterface.c 
 *
 ******************************************************************************
 *
 *  File Description : This file contains interface functions between the
 *                     MAC and the RLC Layer in the LTE system. It contains
 *                     different functions for:
 *                     - Requesting data from the RLC Layer for different 
 *                       logical channels for a UE.
 *                     - Updation of the queueload for a Logical Channel by
 *                       RLC.
 *                     - Requesting the buffer occupancy for a UE's Logical 
 *                       Channel.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacOAMInterface.h"
#include "lteMacRlcInterface.h"
#include "lteMacULUEContext.h"
#include "lteMacDLUEContext.h"
#include "lteMacEncoder.h"
#include "lteMacDLQueueLoadMgr.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacDLQosTokenAlgo.h"
#include "atomic-ops.h"
#include "lteMacTypes.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacTypes.h"
#include "ltePerfStats.h" 

/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define MAX_LC_ID 11


/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
/*UInt32 UpdateRlcSduSize(UInt16 ueIndex,UInt8 lcID, UInt8 rlcSduSizeType,
                UInt32 rlcSduSize);*/
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 3608 changes start */
extern void putEntryInDLPriorityQueue( UInt32 ueIndex, InternalCellIndex internalCellIndex);
/* SPR 3608 changes end */
extern LTE_SEM boSem_g[MAX_NUM_CELL];/*Global Lock for Zero Non Zero Queue Access*/
/****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static void dlProcessSpsUeForActDeact(
        DLUEContext *ueContext_p,
        UInt8       lcId,
        LCQueueLoad *lcQueueLoadArr_p ,
        InternalCellIndex internalcellIndex);
/****************************************************************************
 * Function Name  : macUpdateBufferOccupancyReq
 * Inputs         : ueIndex -  UE Index,
 *                  lcCount -  Logical channel Count
 *                  lcQueueLoadArr_p - pointer to the LCQueueLoad 
 *                  boFlag - Buffer Occupancey
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : INTERFACE_SUCCESS/INTERFACE_FAILURE
 * Description    : It calls the updateLCQueueLoadForUE() and 
 *                  updateUEQueueLoad () fucntions for updating the Queue Load
 *                  values for the Logical Channel and UE as requested by RLC. 
 ****************************************************************************/
UInt32 macUpdateBufferOccupancyReq(UInt16 ueIndex, UInt16 lcCount, 
            /* SPR 1273 Start */
                LCQueueLoad * lcQueueLoadArr_p, BOType boFlag,
                           InternalCellIndex internalCellIndex
)
           /* SPR 1273 End */
{
    UInt32 ueQload = 0;
    DLUEContext *ueContext_p = PNULL;
    UInt8 lcId = INVALID_LC_ID;
    UInt32 qLoad = 0;
    UInt8 lcTrigIndex = 0;
    UInt32 prevQload = 0;
    UInt32 scheduledDataBytes  = 0;
    UInt32 nonGbrQload = 0;
    UInt32 lcIndex = 0;
    AddDelTrigLCNode addDelTrigLCNode[MAX_LOGICAL_CHANNEL] = {{0}};
    /* SPR 16916 FIX START */
#ifdef KPI_STATS
    UInt8 lcTrigIndexForKpi = 0;
    UInt8 addTrigLCNodeForKpi[MAX_LOGICAL_CHANNEL] = {0};
    /* + SPR 17404 */
    UInt8 numLcForDelTrig = 0;
    UInt8 delTrigLCNodeForKpi[MAX_LOGICAL_CHANNEL] = {0};
    /* - SPR 17404 */
#endif
    /* SPR 16916 FIX END */
    DLAmbrInfo  *dlAmbrInfo_p = PNULL;
    UInt64 packetArrivalTS = 0;
    /* Fix for incorrect (extra) add trigger in mimo*/
    UInt16 recvdLcCount = lcCount;

#ifdef PERF_STATS
    LteUePdcpDLPerfStats *lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats *)
        &gPdcpUePerfStats_p[ueIndex].lteUePdcpDLPerfStats;
#endif


    LTE_MAC_UT_LOG(LOG_INFO, "RLC INTERFACE"," [%s] Entry \n",__func__);
    /* CA Changes start */
    if ((internalCellIndex >= MAX_NUM_CELL) ||
            (CELL_PHY_INIT >= cellSpecificParams_g.\
             cellConfigAndInitParams_p[internalCellIndex]->cellState))
        /* CA Changes end */
    {
        lteWarning("Stopped Processing\n");
        return INTERFACE_FAILURE;
    }   
    /* SPR 3822 Changes Start */
    /* Moved semWait after the Check fro stppedProcessing_g */ 
    semWait(&boSem_g[internalCellIndex]);
    /* SPR 3822 Changes End */
    ueContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    if((lcCount == 0) || (PNULL == ueContext_p))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,"RLC INTERFACE" ,"In the function" \
                "[%s] the value of LC COUNT = %d and lcQueueLoadArr_p is " \
                " probably null \n",__func__,lcCount);
        /* SPR 3822 Changes Start */
        semPost(&boSem_g[internalCellIndex]);
        /* SPR 3822 Changes End */
        return INTERFACE_FAILURE;
    }
    /* Fix for incorrect (and extra) add trigger in mimo*/
    for(lcCount = 0; lcCount < recvdLcCount; lcCount++)
    {
        DlLcQosInfo *dlLcQosInfo_p = PNULL;


        lcId = lcQueueLoadArr_p[lcCount].lcId;
        qLoad = lcQueueLoadArr_p[lcCount].lcQueueLoad;

        /* +- SPR 22784 */
        if(RLC_AM_MODE == ueContext_p->logicalChannel[lcId].rlcMode)
        {
            qLoad += lcQueueLoadArr_p[lcCount].ctrlQueueLoad + 
                lcQueueLoadArr_p[lcCount].retxnPduSize;

            /* SPR 4725 Start */
            /* SPR 6978 Fix Start */
            if ( ((lcId == 1 || lcId == 2 )) && 0 != qLoad )
                /* SPR 6978 Fix End */
            {
                ueContext_p->srbQloadAvailbl = TRUE;
            }
            /* SPR 4725 Start */
        }
        /* SPR# 2882 Changes Ends */
        scheduledDataBytes = lcQueueLoadArr_p[lcCount].scheduledDataBytes;

        if ((INVALID_LCID != lcId)&&(MAX_LCID >= lcId)) 
        {    
            /* + SPR 22784 */
            if (lcQueueLoadArr_p[lcCount].headPktTS && qLoad) //expected in bo update/new with positive qload
            {
                packetArrivalTS  = lcQueueLoadArr_p[lcCount].headPktTS;
            }
            else if( ueContext_p->logicalChannel[lcId].queueLoad && qLoad )
            {
                /* Case when Qload (for RLC Control PDU) is only received but
                 * existing Qload is also present */
                packetArrivalTS  = ueContext_p->logicalChannel[lcId].pktArrivalTS;
            }
            else if (qLoad) //qload present but pktarrival in correct
            {
                packetArrivalTS  = getCurrentTick();
            }
            else //no qload
            {
                packetArrivalTS  = 0;
            }
            /* - SPR 22784 */
            /*! \code
             * If LC is an SPS LC
             *    Call dlProcessSpsUeForActDeact()
             * \endcode
             * */
            /* SPR 6690 fix Start */
            if((SPS_SETUP == ueContext_p->spsDlInfo.requestType) &&
                    (TRUE == ueContext_p->logicalChannel[lcId].isSpsLc)) /* SPS SPR 7113 Fix */
                /* SPR 6690 fix End */
            {
                dlProcessSpsUeForActDeact(ueContext_p, lcId,
                        &lcQueueLoadArr_p[lcCount] ,
                        internalCellIndex);
            }
            ueContext_p->logicalChannel[lcId].pktArrivalTS = packetArrivalTS;
            /* SPS_CHG */
            /* SPR 16916 FIX START */
            do{
                prevQload = ueContext_p->logicalChannel[lcId].queueLoad;

            }while(! BOOL_COMPARE_AND_SWAP(&ueContext_p->logicalChannel[lcId].queueLoad, prevQload, qLoad) ) ;

#ifdef KPI_STATS
            /* If queload of LC becomes non zero from zero 
             * Add the LC information for KPI update */
            if ((0 == prevQload) && (0 != qLoad))
            {
                addTrigLCNodeForKpi[lcTrigIndexForKpi] = lcId;
                lcTrigIndexForKpi++;
                LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGERROR,
                        MAC_DL_Strategy, getCurrentTick (), 
                        ueContext_p->ueIndex,lcId,
                        ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI,
                        qLoad,
                        boFlag,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "DL_QLOAD");
            }
            /* + SPR 17404 */
            /* else if queueload of LC becomes zero from non-zero, 
             * add this LC information for KPI update */
            else if( ( 0 != prevQload ) && ( 0 == qLoad) )
            {
                delTrigLCNodeForKpi[numLcForDelTrig] = lcId;
                numLcForDelTrig++;
            }
            /* - SPR 17404 */

#endif
            /* SPR 16916 FIX END */

            if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    initParams_p->dlSchdConfig.dlSchedulerStrategy)
            {
                /* SPR 16916 FIX START */
                /* +- */
                /* SPR 16916 FIX END */
                /* SPR 3679 QOS Changes Start */
                ueContext_p->logicalChannel[lcId].queueLoad = qLoad;
                /* SPR 3679 QOS Changes End */
                /*
                 * decrement the tokens by scheduled RLC data bytes
                 */
                dlLcQosInfo_p = &ueContext_p->logicalChannel[lcId].dlLcQosInfo;
#ifdef UTFWK_SIMULATION
                if (ueContext_p->logicalChannel[lcId].logicalChannelId == INVALID_LCID)
                {
                    /* SPR 3822 Changes Start */
                    semPost(&boSem_g);
                    /* SPR 3822 Changes End */
                    return INTERFACE_FAILURE;
                }
#endif
                /* QOS_UT */
                dlLcQosInfo_p->availToken -= scheduledDataBytes;
                /* SPR 12801 Fix Start */
                if (dlLcQosInfo_p->availToken <= dlLcQosInfo_p->tokenLimit)
                {
                    dlLcQosInfo_p->availToken = dlLcQosInfo_p->tokenLimit;
                }
                /* SPR 12801 Fix End */

                if(RLC_AM_MODE == ueContext_p->logicalChannel[lcId].rlcMode)
                {
                    dlLcQosInfo_p->ctrlQload = 
                        lcQueueLoadArr_p[lcCount].ctrlQueueLoad;
                    dlLcQosInfo_p->reTxnQueueLoad = 
                        lcQueueLoadArr_p[lcCount].retxnPduSize;
                    dlLcQosInfo_p->retxnPduSize = 
                        lcQueueLoadArr_p[lcCount].retxnPduSize;
                    dlLcQosInfo_p->eRlcAmStatus = 
                        lcQueueLoadArr_p[lcCount].ctrlFlag;
                }
                /* SPR 8566 Start */
                else
                {
                    dlLcQosInfo_p->ctrlQload = 0;
                    dlLcQosInfo_p->reTxnQueueLoad = 0;
                    dlLcQosInfo_p->retxnPduSize = 0;
                }
                /* SPR 8566 End */

                /*
                 * If NON-GBR LC, Decrement the AMBR token
                 */
                if(NON_GBR == dlLcQosInfo_p->channelType)
                {
                    dlAmbrInfo_p = &ueContext_p->dlAmbrInfo;
                    dlAmbrInfo_p->availAmbrToken -= scheduledDataBytes;
                    /* SPR 12801 Fix Start */
                    if (dlAmbrInfo_p->availAmbrToken <= dlAmbrInfo_p->ambrTokenLimit)
                    {
                        dlAmbrInfo_p->availAmbrToken = dlAmbrInfo_p->ambrTokenLimit;
                    }
                    /* SPR 12801 Fix End */
                    /* This has already been done once above */
                    dlAmbrInfo_p->scheduledToken[dlAmbrInfo_p->curTknWindowInd]
                        +=  scheduledDataBytes;
                    nonGbrQload += qLoad;
                    dlAmbrInfo_p->queueLoad = nonGbrQload;
                }
                else
                {
                    dlLcQosInfo_p->scheduledToken
                        [dlLcQosInfo_p->curTknWindowInd] += scheduledDataBytes;

                    /* The below check is kept here to avoid the unncessary
                     * checks for SRB's. Now as we have set SRB bearer type as 
                     * GBR, the below check is present here
                     */
                    if((1 == lcId) || (2 == lcId))
                    {
                        dlLcQosInfo_p->availToken = qLoad;
                        /* + SPR 11472 Changes */
                        dlLcQosInfo_p->dlQosStrategyLCNode_p->schedulableTokensPcell = qLoad;
                        /* - SPR 11472 Changes */
                        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SRB_TOKENS(ueIndex, qLoad);
                    }
                }

                /*! \code
                 * - If UE State is SPS_ACTIVATED  and LC is an SPS LC
                 * \endcode
                 * */
                if((SPS_ACTIVATED == ueContext_p->spsDlInfo.spsState || 
                            SPS_ACTIVATION_INITIATED == ueContext_p->spsDlInfo.spsState )
                        &&(TRUE == ueContext_p->logicalChannel[lcId].isSpsLc))
                {
                    /*! \code
                     * - For QOS Strategy, Do not generate ADD/DELETE Triggers for SPS LC. 
                     * \endcode
                     * */
                }
                else
                {
                    /*
                     * If qLoad or tokens transits from Non - Zero to Zero
                     */
                    if((0 != prevQload) && (0 == qLoad))
                    {
                        addDelTrigLCNode[lcTrigIndex].lcId = lcId;
                        addDelTrigLCNode[lcTrigIndex].addDeleteTrigger = 
                            DELETE_TRIGGER;
                        lcTrigIndex++;
#ifdef PERF_STATS
                        if ( lcId > 2  ) 
                        {
                            pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].currentIndex =\
                                                                                     ! (pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].currentIndex);
                            pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].previousIndex =\
                                                                                      ! (pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].previousIndex);
                            /* Store the value in Shared Memory from Previous Index */
                            lteUePdcpDLPerfStats_p->pdcpLastTtiTransVolDl[lcId -3] =\
                                                                                    pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].pdcpDataSize
                                                                                    [ pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].previousIndex];
                        }
#endif

                        /* +- SPR 17404 */
                    }
                    else if ((0 == prevQload) && (0 != qLoad))
                    {
                        addDelTrigLCNode[lcTrigIndex].lcId = lcId;
                        addDelTrigLCNode[lcTrigIndex].addDeleteTrigger = 
                            ADD_TRIGGER;
                        lcTrigIndex++;
                    }
                }
            }
            else
            {

                /* this is already being taken care of above this if-else condition */
                /* SPR 3063 Chg Starts */
                /* SPR 3063 Chg Ends */

                /* changes for atomically Qload updationi start*/
                ueContext_p->logicalChannel[lcId].queueLoad = qLoad;
                /* changes for atomically Qload updationi end*/
                /*Changes deleted for SPR SPR 1273 Start */

            }
            /* SPR 10027 FIX START */
            if(lcId >= MAC_LCID_3 && lcId <= MAC_LCID_10)
            {
                ueContext_p->lastScheduledTick = getCurrentTick();
            }
            /* SPR 10027 FIX END */
        }
        else
        {
            continue;
        }
    }
    ueQload = 0;
    for( lcIndex = 0; lcIndex < MAX_LC_ID; lcIndex++)
    {
        if( (SPS_ACTIVATED == ueContext_p->spsDlInfo.spsState || 
                    SPS_ACTIVATION_INITIATED == ueContext_p->spsDlInfo.spsState ) && 
                TRUE == ueContext_p->logicalChannel[lcIndex].isSpsLc )
        {
            /*! \code
             * For RR/PFS, Do not add SPS LC qLoad to total ueQueueLoad 
             * \endcode
             * */
        }
        else
        {
            ueQload += ueContext_p->logicalChannel[lcIndex].queueLoad;
        }

    }
    /* SPR 16916 FIX START */
#ifdef KPI_STATS
    if (lcTrigIndexForKpi)
    {
        macUpdateActiveUePerQciInfo(lcTrigIndexForKpi,ueContext_p,addTrigLCNodeForKpi,internalCellIndex);
    }
    /* + SPR 17404 */
    if( 0 != numLcForDelTrig )
    {
        macUpdateKPIForDelTrigger( numLcForDelTrig, ueContext_p, 
                delTrigLCNodeForKpi );
    }
    /* - SPR 17404 */

#endif
    /* SPR 16916 FIX END */
            if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    initParams_p->dlSchdConfig.dlSchedulerStrategy)
    {
        /*SPR 3697 QOS Changes Start*/
        ueContext_p->ueQueueLoad = ueQload;
        /*SPR 3697 QOS Changes End*/
        if(RLC_BO_TYPE_UPDATE == boFlag)
        {
            /*Qos phase-3 changes start*/
            if(IS_CA_ENABLED())
            {
                scellStateMngrCheckQloadForActivationDeactivation(ueContext_p,internalCellIndex);
                /*Qos phase-3 changes end*/
            }	
            dlQosUpdateScheduledTokens(ueContext_p,internalCellIndex);		    
        }

        if(lcTrigIndex)
        {
            /* SPR 3608 changes end */
            dlQosPutEntryInZeroNonZeroQueueLoadTriggerQueue(ueContext_p,
                    addDelTrigLCNode, 
                    lcTrigIndex,internalCellIndex);
        }
    }
    else
    {
        /* SPR 1273 Start */
        updateUEQueueLoad (ueContext_p, ueQload, boFlag);
        /* SPR 1273 End */
    }
    LTE_MAC_UT_LOG(LOG_INFO, "RLC INTERFACE"," [%s] Exit \n",__func__);
    semPost(&boSem_g[internalCellIndex]);
    return INTERFACE_SUCCESS;
}
/*+- SPR 17777 */
/* _RLC_AM_MODE_ */
/****************************************************************************
 * Function Name  : sendARQIndicationToRLC
 * Inputs         : ueIndex - UE Identifier,
 *                  tbInfo, TBFlag,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : It inicates RLC about the max re-transmission 
 *                  exceded for the RLC PDU of seqNum.
 ****************************************************************************/
/* SPR 11402 Fix  */ 
/* + KPI_CA */
void sendARQIndicationToRLC (UInt32 ueIndex,DLHARQTBInfo *tbInfo,TbValue TBFlag,
                              InternalCellIndex internalCellIndex)
{
    UInt8 lcCount=0;
    UInt8 lc = 0;
    UInt8 qci = 0; 
#ifdef KPI_STATS
    UInt8 sduCount = 0;                                 
    /* +- SPR 17777 */
#else
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
#ifndef OBSELETE_REL_8
  LTE_GCC_UNUSED_PARAM(ueIndex)
#endif
    /* +- SPR 17777 */
#endif
    RLCDataBuffer* rlcDataBufferTB = PNULL;
    rlcDataBufferTB = &(tbInfo->transmissionAndRLCData_p->rlcDataBufferArr[0]);

    RLCHarqFailureInfo *rlcHarqFailureInfo = PNULL;
    rlcHarqFailureInfo = &(tbInfo->transmissionAndRLCData_p->rlcHarqFailureInfo[0]);
#ifdef KPI_STATS
/*spr 19296 fix start*/
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    LteMacKPIStats * pLteMacKPIStats = &(gMacStats.
      pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
/*spr 19296 fix end*/
/* - KPI_CA */

#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,"RLC INTERFACE","%s Entry.\n", __func__);

    if(rlcDataBufferTB != NULL)
    {
        if(TBFlag == TB_ONE)
            lcCount = tbInfo->transmissionAndRLCData_p->lcCountTBOne; 
        else
            lcCount = tbInfo->transmissionAndRLCData_p->lcCountTBTwo;
 
        for(lc = 0; lc < lcCount; lc++)
        {
            qci = rlcDataBufferTB[lc].qci;
            if (qci > 0 && qci <= MAX_QCI)      
            {
                if(rlcHarqFailureInfo[lc].harqFailureReporting) 
                {
                    /* SPR 8082 Fix start */
#ifdef OBSELETE_REL_8
                    /* SPR 8082 Fix end */
                    macUeHarqFailureInd(ueIndex,&rlcHarqFailureInfo[lc]);
#endif
                    /* SPR 8082 Fix end */
                    /* +- SPR 17777 */
                    lteMacUpdateStatsRlcHarqFailure(rlcHarqFailureInfo);
                    /* +- SPR 17777 */
                }	   

#ifdef KPI_STATS
            /* SPR 16843 start */
            DLUEContext *dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            if(PNULL != dlUEContext_p)
            {
                if (RLC_UM_MODE == dlUEContext_p->logicalChannel[rlcDataBufferTB[lc].lcID].rlcMode)
                {
                if(rlcDataBufferTB[lc].sduCount > MAX_RLC_SDU_IN_PDU_FOR_KPI)
                    sduCount = MAX_RLC_SDU_IN_PDU_FOR_KPI;
                else
                    sduCount = rlcDataBufferTB[lc].sduCount;

/*spr 19296 fix start*/
                pLteMacKPIStats->totalDlDrbUuLossRatePerQci[--qci] += sduCount;
/*spr 19296 fix end*/
                }
            }
            /* SPR 16843 end */
#endif
            }
        }
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,"RLC INTERFACE","%s Exit.\n", __func__);
    return ;
}

/* SPR 11402 Fix  */ 

/* _RLC_AM_MODE_ */
/****************************************************************************
 * Function Name  : macUeDataReq
 * Inputs         : ueIndex - UE Identifier,
 *                  lcCount - valid logical channel count,
 *                  rlcBuffer_p - pointer to RLCDataBuffer,
 *                  successOrFailure - whether get rlc data request was success
 *                  or failure,
 *                  rlcFailureInfo - pointer to RLCHarqFailureInfo and
 *                  currentTick - to check current required fields.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Called by RLC in response to request by MAC layer.
 *                  It creates a node and fills the info/data given by RLC
 *                  and puts the node into encoder queue.
 ****************************************************************************/

UInt32 macUeDataReq(UInt16 ueIndex,UInt8 lcCount,RLCDataBuffer * rlcBuffer_p
                                                       ,UInt8 successOrFailure,RLCHarqFailureInfo *rlcFailureInfo
#ifdef DL_DATA_SEPARATION                
                                                        /* SPR 15909 fix start */
                                                       ,tickType_t currentTick
                                                        /* SPR 15909 fix end */
#endif                
                                                       ,InternalCellIndex internalCellIndex

                                                       )
{
#ifdef LINUX_PC_TEST
	extern void adapter_txrlc_pdu_build_cfm(UInt16 ueIndex, RLCDataBuffer *rlcBuffer_p);
	adapter_txrlc_pdu_build_cfm(ueIndex, rlcBuffer_p);
	return MAC_SUCCESS;
#endif	
    LTE_MAC_UT_LOG(LOG_INFO, "RLC INTERFACE"," [%s] Entry \n",__func__);
    TransmissionAndRLCData* transmissionAndRLCData_p = PNULL;
    UInt32 index = 0;
#ifdef PERF_STATS 
    UInt32 count = 0;
    UInt32 currentIndex  = 0;
    UInt32 lcId = 0;
    UInt32 sduCount =0;
#endif
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(successOrFailure)
    /* +- SPR 17777 */
    /* SPS_CHG */
    DLHARQProcess *dlHarqProcess_p = PNULL;
    /* SPS_CHG */
    transmissionAndRLCData_p = (TransmissionAndRLCData *)rlcBuffer_p;

    /* Coverity 67176 fix start */
    if (PNULL == transmissionAndRLCData_p)
    {
        return MAC_FAILURE;
    }
    /* Coverity 67176 fix end */


    LTE_MAC_UT_LOG(LOG_INFO,"RLC INTERFACE" ,\
            "[%s] received RLC data for ueIndex %d reqstd tick  %d\n", 
            __func__,ueIndex,transmissionAndRLCData_p->scheduledTick);

    LTE_MAC_UT_LOG(LOG_INFO, "RLC INTERFACE",\
            "[%s] Calling putEntryInEncoderQueue with following\n" \
            "transmissionAndRLCData_p->paddingBytesForTBOne = %d\n"
            "transmissionAndRLCData_p->paddingBytesForTBTwo = %d\n"
            "transmissionAndRLCData_p->harqProcessId = %d\n"
            "transmissionAndRLCData_p->cceIndex = %d\n"
            "transmissionAndRLCData_p->sizeTBOne = %d\n"
            "transmissionAndRLCData_p->sizeTBTwo = %d\n"
            "transmissionAndRLCData_p->lcCountTBOne = %d\n"
            "transmissionAndRLCData_p->lcCountTBTwo = %d\n"
            ,__func__
            ,transmissionAndRLCData_p->paddingBytesForTBOne
            ,transmissionAndRLCData_p->paddingBytesForTBTwo
            ,transmissionAndRLCData_p->harqProcessId
            ,transmissionAndRLCData_p->cceIndex
            ,transmissionAndRLCData_p->sizeTBOne
            ,transmissionAndRLCData_p->sizeTBTwo
            ,transmissionAndRLCData_p->lcCountTBOne
            ,transmissionAndRLCData_p->lcCountTBTwo
            );

    if (INVALID_UE_INDEX != ueIndex)
    {
        LOG_MAC_MSG (MAC_UE_DATA_REQ_ID, LOGBRIEF, MAC_MUX,
                getCurrentTick(), 
                ueIndex,transmissionAndRLCData_p->harqProcessId, lcCount,
                transmissionAndRLCData_p->sizeTBOne,
                transmissionAndRLCData_p->sizeTBTwo,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "");
    }

    {
#ifdef OBSELETE_REL_8
        memCpy(transmissionAndRLCData_p->rlcHarqFailureInfo,rlcFailureInfo,lcCount * sizeof(RLCHarqFailureInfo));
#endif        
    }
    /* SPS_CHG */
    /* + SPR_14897 */ 
    {
    if((INVALID_UE_INDEX != ueIndex ) && (cellSpecificParams_g.\
    /* - SPR_14897 */ 
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps))
    {
    /* Coverity 67218 Fix End */
        UInt8 count = 0;
        UInt32 totalTBSize = 0;
        UInt8 harqId = transmissionAndRLCData_p->harqProcessId;
        dlHarqProcess_p = &dlUECtxInfoArr_g[ueIndex].dlUEContext_p->dlHarqContext_p->harqProcess[harqId];
        for(count = 0; count < lcCount ; count++)
        {
            totalTBSize += rlcBuffer_p[count].totalPduLen ; 
        }
        if( (totalTBSize == 0) &&
            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == INVALID_TA_DRX_MSG) &&
            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == INVALID_SCELL_ACT_DEACT_MSG))
        {
            dlHarqProcess_p->isSpsOnlyPaddingPresent = TRUE;  
        }
    }
    }
    /* SPS_CHG */
    putEntryInEncoderQueue(ueIndex,
            lcCount,
            /* + SPR 5584 (COVERITY CID 25520) FIX */
            &transmissionAndRLCData_p,
            /* - COVERITY CID 25520 FIX */
#ifndef DL_DATA_SEPARATION            
            (transmissionAndRLCData_p->scheduledTick %MAX_ENCODER_CONTAINERS)
#else            
            (currentTick %MAX_ENCODER_CONTAINERS), currentTick
#endif            
            ,internalCellIndex
            );
    /* + SPR_14897 */
    if (ueIndex != INVALID_UE_INDEX)
    {
        /* - SPR_14897 */
        for ( index = 0 ; index < lcCount ; index ++)
        {
            LOG_MAC_MSG (MAC_UE_DATA_REQ_EACH_LC_ID, LOGBRIEF, MAC_MUX,
                    getCurrentTick(), 
                    transmissionAndRLCData_p->rlcDataBufferArr[index].lcID,
                    transmissionAndRLCData_p->rlcDataBufferArr[index].statusPduLen,
                    transmissionAndRLCData_p->rlcDataBufferArr[index].newPduLen,
                    transmissionAndRLCData_p->rlcDataBufferArr[index].totalPduLen,
                    transmissionAndRLCData_p->rlcDataBufferArr[index].remainingDataSize,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME, "");
           
#ifdef PERF_STATS
            sduCount =   transmissionAndRLCData_p->rlcDataBufferArr[index].completeEncodedSduCount;
            lcId = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID;
            if (lcId > 2)
            {
                currentIndex =  pdcpLastTtiTransVolDl_g[ueIndex][lcId -3 ].currentIndex;
                for(; count < sduCount; count++ )
                {
                    pdcpLastTtiTransVolDl_g[ueIndex][lcId  - 3 ].pdcpDataSize[currentIndex] += \
                                      transmissionAndRLCData_p->rlcDataBufferArr[index].pdcpSduSize[count];
                }
            }
#endif
        }
    /* 32.425 Events code changes*/
#ifdef KPI_STATS
        /* This function will update the T1 and T2 Timer for the Data Burst */
        /* Coverity 67218 Fix - Removed earlier fix */
        lteMacUpdateKpiStatsIPThpDlOnAdd(ueIndex,rlcBuffer_p,lcCount);/*Coverity 94897 fix*/
#endif
    /* 32.425 Events code changes*/
        /* - COVERITY CID 25520 FIX */
            /* +- SPR 17777 */
            lteMacUpdateStatsRlcRx(rlcBuffer_p);
            /* +- SPR 17777 */
        /* + SPR 5584 (COVERITY CID 25520) FIX */
    }
    /* - COVERITY CID 25520 FIX */
    LTE_MAC_UT_LOG(LOG_INFO, "RLC INTERFACE"," [%s] Exit \n",__func__);
    return MAC_SUCCESS;
}

/* SPS_CHG*/

/****************************************************************************
 * Function Name  : dlProcessSpsUeForActDeact
 * Inputs         : ueContext_p - pointer to DLUEContext,
 *                  lcId - logical Channel Id and
 *                  lcQueueLoadArr_p - pointer to LCQueueLoad
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This functions checks for the different conditions for SPS
 *                  Activation and Deactivation. On detection of SPS
 *                  Activation/Deactivation it pushes the entry in respective
 *                  queue to be processed by SPS Strategy.
 ****************************************************************************/
void dlProcessSpsUeForActDeact(
        DLUEContext *ueContext_p,
        UInt8       lcId,
        LCQueueLoad *lcQueueLoadArr_p ,
        InternalCellIndex internalCellIndex)
{
    LP_MacDlSpsActReq      macSpsActReq_p         = PNULL;
    LP_SpsDLIntervalProfile   dlSpsIntervalProfile_p = PNULL;
    LP_SpsCommIntervalProfile commSpsIntProfile_p    = PNULL;
    LP_SpsDlInfo              spsDlInfo_p            = PNULL;
    UInt16                 dlSpsInterval          = 0;
    UInt8       voiceSilenceType       = lcQueueLoadArr_p->voiceSilenceType;
    UInt32      headPktSize            = lcQueueLoadArr_p->headPktQueueLoad;
    DLLogicalChannelInfo   *lcInfo_p   = &ueContext_p->logicalChannel[lcId];
    
    /*!
     * \code
     * Calculate InterPacketArrival Time.
     * Get the SPS Interval for the UE.
     * \endcode
     * */
    spsDlInfo_p = &ueContext_p->spsDlInfo;

    if( 0 != lcQueueLoadArr_p->headPktTS )
    {
        /* Store Pkt Arrival TS in SPS context */
        spsDlInfo_p->pktArrivalTs = lcQueueLoadArr_p->headPktTS;
    }
    
    dlSpsInterval = spsDlInfo_p->spsDlSetupInfo_p->\
                             semiPersistentSchedIntervalDL;
    dlSpsIntervalProfile_p = 
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
        initParams_p->dlSchdConfig.spsDlIntervalProfile[dlSpsInterval]) ;
    
    commSpsIntProfile_p = &cellSpecificParams_g.cellConfigAndInitParams_p\
        [internalCellIndex]->initParams_p->spsCommIntervalProfile[dlSpsInterval];

    switch (spsDlInfo_p->spsState)
    {
        case SPS_CONFIGURED:
        {
            /*!
             * \code
             *  - If (UE State == SPS_CONFIGURED ) and 
             *       (Minimum Sps MCS >= UE Mcs) 
             *    Activation condition satisfied. Push entry in dlSpsActQ_g
             *  \endcode
             * */
            if( (dlSpsIntervalProfile_p->dlMinMcsSps <= 
                        ueContext_p->dlCQIInfo.mcsIndexCodeWordOne) )
            {
                if ( IPV4_TYPE == lcQueueLoadArr_p->ipPktType )
                {              
                    spsDlInfo_p->avgRlcSduSize = 
                        commSpsIntProfile_p->maxPdcpPduSizeIpv4[
                        lcInfo_p->rohcProfile ];
                }
                else
                {
                    spsDlInfo_p->avgRlcSduSize = 
                        commSpsIntProfile_p->maxPdcpPduSizeIpv6[ 
                        lcInfo_p->rohcProfile];
                }
                /* Allocate memory for Activation queue node */
                macSpsActReq_p = (LP_MacDlSpsActReq)getMemFromPool(
                        sizeof(MacDlSpsActReq), PNULL);
                if ( PNULL == macSpsActReq_p  )
                {
                    LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                            getCurrentTick(), __LINE__, sizeof(MacDlSpsActReq),
                            0,0,0, 0,0, __FILE__, __func__);
                    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                            CRITICAL_ALARM);
                    ltePanic("getMemFromPool fails %s", __func__);
		            /* Klockwork warning Changes Start */
		            return ;
	                /* Klockwork warning Changes End */
                }
                /* Fill the activation queue node */
                macSpsActReq_p->ueIndex = ueContext_p->ueIndex;
                macSpsActReq_p->cause = FIRST_NON_ZERO_QUEUE_LOAD;
                /* Enqueue in dlSpsActQ_g */
                /* Coverity 63548 Fix Start */
               if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsActQ_gp[internalCellIndex], macSpsActReq_p))
               {
                   freeMemPool(macSpsActReq_p);
               }
                /* Coverity 63548 Fix End */
                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_ACT_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsActQ_gp[internalCellIndex] ),
                        internalCellIndex);
                /* CA Stats Changes End */
            }
            /* Update SPS Q load to UE context */
            spsDlInfo_p->headPktSize = headPktSize; 
            break;
        }       
        case SPS_DEACTIVATED:
        {
            if( 0 != spsDlInfo_p->maxPktsAfterDeactivatedForPdschFailure )
            {
                (spsDlInfo_p->maxPktsAfterDeactivatedForPdschFailure)--;
            }
            else
            {
                /*! 
                 * \code
                 * For Subsequent activation i.e. when UE State = SPS_DEACTIVATED
                 * and voice packet and interPktArrivalTime < 
                 * interPktDelayForSilenceDetection * periodicity.
                 * - If Voice Packet is Received, push entry in dlSpsActQ_g             
                 * \endcode
                 * */
                if( ( (dlSpsIntervalProfile_p->dlMinMcsSps + \
                                dlSpsIntervalProfile_p->dlMcsMargin) <= 
                            ueContext_p->dlCQIInfo.mcsIndexCodeWordOne) && 
                        ( (0 != headPktSize) && (VOICE_PKT == voiceSilenceType)
                        ) )
                {
                    /* Allocate memory for Activation queue node */
                    macSpsActReq_p = (LP_MacDlSpsActReq)getMemFromPool(
                            sizeof(MacDlSpsActReq), PNULL);
                    if ( PNULL == macSpsActReq_p  )
                    {
                        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                                getCurrentTick(), __LINE__, sizeof(MacDlSpsActReq),
                                0,0,0, 0,0, __FILE__, __func__);
                        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                                CRITICAL_ALARM);
                        ltePanic("getMemFromPool fails %s", __func__);
                        /*KLOCWORK_FIX_START_56*/
                        return;
                        /*KLOCWORK_FIX_END_56*/
                    }
                    /* Fill the activation queue node */
                    macSpsActReq_p->ueIndex = ueContext_p->ueIndex;
                    macSpsActReq_p->cause = VOICE_PACKET; 
                    /* Enqueue in dlSpsActQ_g */
                    /* + Coverity Id - 32592 */
                    if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsActQ_gp[internalCellIndex], macSpsActReq_p))
                    {
                        freeMemPool(macSpsActReq_p);
                    }
                    /* - Coverity Id - 32592 */
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( DL_SPS_ACT_Q, 
                            QCOUNT_MAC_DL_SPS_Q( dlSpsActQ_gp[internalCellIndex] ),
                            internalCellIndex);
                    /* CA Stats Changes End */
                }
            }
            /* Update SPS Q load to UE context */
            spsDlInfo_p->headPktSize = headPktSize;
            /* Fill avgRunningSduSize to avgRlcSduSize in UE Context */
            /* Phase supports only OAM configured grant for activation */
            if ( IPV4_TYPE == lcQueueLoadArr_p->ipPktType )
            {              
                spsDlInfo_p->avgRlcSduSize = 
                    commSpsIntProfile_p->maxPdcpPduSizeIpv4[
                      lcInfo_p->rohcProfile ];
            }
            else
            {
                spsDlInfo_p->avgRlcSduSize = 
                    commSpsIntProfile_p->maxPdcpPduSizeIpv6[ 
                      lcInfo_p->rohcProfile];
            }
            break ; 
        }
        case SPS_ACTIVATED:
        {
            /* Update SPS Q load to UE context */
            spsDlInfo_p->headPktSize = headPktSize; 
            
            break ;
        }
        case SPS_RES_RSV_REQ_INITIATED:
        case SPS_ACTIVATION_INITIATED:
        case SPS_DEACTIVATION_INITIATED:
        case SPS_REACTIVATION_INITIATED:
        {
            /* Update SPS Q load to UE context */
            spsDlInfo_p->headPktSize = headPktSize;

            break;
        }
        default:
        {
            break;
        }
    }
}

