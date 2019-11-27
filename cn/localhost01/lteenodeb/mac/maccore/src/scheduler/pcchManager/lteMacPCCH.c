/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPCCH.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definitions for
 *                     Paging functionality.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPCCH.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.29.34.1  2010/07/21 09:39:55  gur24420
 * Aligned changes with respect to eNode Integration activity(based on 
 * RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Merged Endian fixes for receiving data from RRC.
 *
 * Revision 1.29  2009/08/28 09:06:50  gur19413
 * SIT bug fix
 *
 * Revision 1.28  2009/07/26 08:16:01  gur15697
 * compilation warnings removed
 *
 * Revision 1.27  2009/07/13 11:50:03  gur15697
 * second argument of LTE_MAC_UT_LOG modified
 *
 * Revision 1.26  2009/07/10 07:04:46  gur15697
 * compilation warnings removed
 *
 * Revision 1.25  2009/07/01 06:20:01  gur15697
 * LTE_LOG changed to LTE_MAC_UT_LOG
 *
 * Revision 1.24  2009/06/27 05:36:28  gur15697
 * recvPagingMessage() arguments modified
 *
 * Revision 1.23  2009/06/24 04:42:54  gur15697
 * UT fix
 *
 * Revision 1.22  2009/06/22 14:15:11  gur15697
 * UT bug fix
 *
 * Revision 1.21  2009/06/22 09:06:09  gur15697
 * RACH review comments incorporated
 *
 * Revision 1.20  2009/06/22 08:47:33  gur15697
 * review comments incorporated
 *
 * Revision 1.19  2009/06/20 12:39:06  gur15697
 * review comments incorporated
 *
 * Revision 1.18  2009/06/18 04:54:15  gur15697
 * UT bugFix
 *
 * Revision 1.17  2009/06/17 12:37:39  gur15697
 * initPCCHMgr()  modified
 *
 * Revision 1.16  2009/06/17 08:46:03  gur15697
 * Review comments incorporated
 *
 * Revision 1.15  2009/06/16 14:36:52  gur15697
 * UT bug fix
 *
 * Revision 1.14  2009/06/15 11:19:54  gur15697
 * UT bug fix
 *
 * Revision 1.13  2009/06/05 08:03:14  gur15697
 * init and cleanup functions added
 *
 * Revision 1.12  2009/06/05 04:13:20  gur15697
 * updated the file w.r.t PDCCH manager changes
 *
 * Revision 1.11  2009/06/03 11:57:24  gur15697
 * initPCCHMgr() added
 *
 * Revision 1.10  2009/05/27 09:53:42  gur15697
 * compilation errors removed
 *
 * Revision 1.9  2009/05/19 14:04:29  gur15697
 * memCpy() handling added which is needed for Transmitter
 *
 * Revision 1.8  2009/05/18 06:44:26  gur15697
 * code for zero buffer copy added, review comments incorporated
 *
 * Revision 1.7  2009/05/12 13:25:51  gur15697
 * comments added
 *
 * Revision 1.6  2009/05/12 12:51:16  gur15697
 * Compilation errors removed
 *
 * Revision 1.5  2009/05/10 08:35:25  gur15697
 * chages made as per the structure changed in PDCCH
 *
 * Revision 1.4  2009/05/08 14:28:58  gur15697
 * compilation errors removed
 *
 * Revision 1.2  2009/05/05 14:13:49  gur15697
 * functions updated
 *
 * Revision 1.1  2009/05/04 07:40:48  gur20052
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
#include "lteMacCellConfMgr.h" 
#include "lteMacPCCH.h"

/******************************************************************************
 Private Definitions
 *****************************************************************************/

/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* CA changes start */
#define GET_PAGING_INDEX(sf,internalCellIndex) pcchGlobals_g[internalCellIndex].pageSf2Idx[sf]
/* CA changes end */

STATIC PCCHGlobals pcchGlobals_g[MAX_NUM_CELL];

/*Cyclomatic Complexity changes - Starts here */
static  void updateCreateNodeFlag(UInt32   pagingFrameNum, 
                                        UInt8    pagingOccasion, 
                                        UInt32 * createNodeFlag, 
                                        UInt32   currentSFN, 
                                        UInt32 * currentSF,
                                        UInt16   numSFN,
                                        UInt8 * wrapAroundFlag,
                                        UInt32 sysFrameNum );
/*Cyclomatic Complexity changes - end here */

/* Cyclomatic_complexity_changes_start */
UInt32 validatePagingParams(UInt16 numSFN,
        UInt8 *pagingOccasion_p,
        UInt32 pagingFrameNum,
	UInt8     emtcPaging,
        InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_end */

/*****************************************************************************
 * Function Name  : initPCCHMgr
 * Inputs         : numOfCells - Number of Cells configured
 * Outputs        : None
 * Returns        : None
 * Description    : This will initialises the data for PCCH manager. 
 *                  This is called at MAC_INIT_LAYER _REQ
 *****************************************************************************/
void initPCCHMgr(InternalCellIndex numOfCells)
{
    UInt32 i = 0;
    UInt32 j = 0;
    InternalCellIndex cellIndex = 0;

    LTE_MAC_UT_LOG(LOG_DETAIL ,PCCH_MGR,
            "[%s]: initPCCHMgr()\n",__func__);

    for(cellIndex = 0; cellIndex < numOfCells; cellIndex++)
    {
        for (i = 0; i <= MAX_PAGING_SFN_RANGE; i++)
        {
            for (j = 0; j < MAX_PAGING_SF_NUM; j++)
            {
                pcchGlobals_g[cellIndex].pcchMsgArr[i][j] = PNULL;
            }
        }
        for (i = 0; i < MAX_SUB_FRAME; i++)
        {
            pcchGlobals_g[cellIndex].pageSf2Idx[i] = 0xFF;
        }
    }
}

/****************************************************************************
* Function Name  : cleanupPCCHMgr
* Inputs         : internalCellIndex - Cell-Index at MAC
* Outputs        : None
* Returns        : None
* Description    : This API cleanup the PCCH Manager. called at cell stop
****************************************************************************/
void cleanupPCCHMgr(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 j = 0;

    /* SPR 11109 28-JUN-2014 Fix start */
    PCCHInfoNode *pcchNode_p = PNULL;
    /* SPR 11109 28-JUN-2014 Fix end */

    LTE_MAC_UT_LOG(LOG_DETAIL ,PCCH_MGR,
            "[%s]: initPCCHMgr()\n",__func__);

    for (i = 0; i <= MAX_PAGING_SFN_RANGE; i++)
    {
        for (j = 0; j < MAX_PAGING_SF_NUM; j++)
        {
            if ( pcchGlobals_g[internalCellIndex].pcchMsgArr[i][j])
            {
                /* SPR 11109 28-JUN-2014 Fix start */
                pcchNode_p = pcchGlobals_g[internalCellIndex].pcchMsgArr[i][j];

                if (pcchNode_p->pagingData_p)
                {
                    pcchNode_p->numRepititions--;

                    if (!pcchNode_p->numRepititions)
                    {
                        freeMemPool(pcchNode_p->pagingData_p);
                        pcchNode_p->pagingData_p = PNULL;
                        freeMemPool(pcchNode_p);
                    }
                }
                else
                {
                    freeMemPool(pcchNode_p);
                }                            
                pcchNode_p = PNULL;
                /* SPR 11109 28-JUN-2014 Fix end */
                pcchGlobals_g[internalCellIndex].pcchMsgArr[i][j] = PNULL;
            }
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : deInitPCCHMgr
 * Inputs         : Internal cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This will re-initialises the data for PCCH manager. 
 *                  This is called at cell delete
 *****************************************************************************/
void deInitPCCHMgr(InternalCellIndex cellIndex)
{
    UInt32 i = 0;

    LTE_MAC_UT_LOG(LOG_DETAIL ,PCCH_MGR,
            "[%s]: deInitPCCHMgr()\n",__func__);

    for (i = 0; i < MAX_SUB_FRAME; i++)
    {
        pcchGlobals_g[cellIndex].pageSf2Idx[i] = 0xFF;
    }
    return;
}

/*****************************************************************************
 * Function Name  : recvPagingMessage
 * Inputs         : dataLen - length of the data to decode,
 *                  data_p - Pointer to the data
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE/MAC_SFN_ERROR
 * Description    : This will prepares node and inserts in pcchMsgArr[] array.
 *****************************************************************************/
UInt32 recvPagingMessage(UInt32 dataLen,
                         UInt8  *data_p,
                         InternalCellIndex internalCellIndex )
/* CA Changes end */
{
    UInt32 pagingFrameNum = 0; /*Received PF*/
    UInt8  pagingOccasion = 0; /*Received PO*/
    UInt32 currentSFN  = 0;
    UInt32 currentSF   = 0;
    UInt32 sysFrameNum = 0;
    UInt32 createNodeFlag       = TRUE;
    UInt8  wrapAroundFlag       = FALSE;
    PCCHInfoNode *pcchInfoNode_p = PNULL;
	UInt8 isPagingForeMTC = 0;

    UInt16  numSFN = 0; /* REL_3.0_INTEG_CHG */
    UInt8   pagingIdx = 0;
    UInt8   numPagingOccasions;
    UInt8   i;
    UInt8 tpc = 0;

    UInt32 retVal = MAC_SUCCESS;
    LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
            "Entry [%s]\n",__func__);

    pagingFrameNum = LTE_GET_U16BIT(data_p);
    data_p += 2;    
    dataLen -= 2 /*Paging Frame field length*/;

    pagingOccasion = *data_p;
    data_p++;
    /* REL_3.0_INTEG_CHG */
    dataLen -= 1; /*Paging Occasion field length*/

    numSFN =  LTE_GET_U16BIT(data_p);
    data_p += 2;    
    dataLen -= 2 /* no_of_sfn field length*/;

	isPagingForeMTC =  *data_p;
	data_p += 1;    
	dataLen -= 1 /* no_of_sfn field length*/;

	



    /* REL_3.0_INTEG_CHG */

    /* Cov_124276_Fix_Start */
    /*DCI_1C_Comment Start */
	DCIFormat dciFormat =  INVALID_DCI_FORMAT;
    /*DCI_1C_Comment End */
    /* Cov_124276_Fix_End */

    /* Cyclomatic_complexity_changes_start */
    retVal = validatePagingParams(numSFN,
            &pagingOccasion,
            pagingFrameNum,
			isPagingForeMTC,
            internalCellIndex);

    if (MAC_SUCCESS != retVal)
    {
        return retVal;
    }
    /* cyclomatic_complexity_changes_end */

    LTE_MAC_UT_LOG(LOG_INFO,PCCH_MGR,
            "[%s]: Paging Info received:\n"
            "DataLength [%d] pagingFrameNum [%d] pagingOccasion [%d]\n",
            __func__,dataLen,pagingFrameNum,pagingOccasion);

    /*DCI_1C_Comment Start */
    dciFormat = cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->dciFormatForCommonChnlMsg;

    /*Validate the Length of the message received*/
    /*In DCI format 1A maximum TB size should not be > 57 bytes (456 bits)*/
    if ( (dataLen == 0) || (( DCI_FORMAT_1A == dciFormat ) && dataLen > MAX_PAGING_MSG_SIZE_FOR_DCI_FORMAT_1A) 
            || ((DCI_FORMAT_1C == dciFormat ) && dataLen > MAX_PAGING_MSG_SIZE_FOR_DCI_FORMAT_1C) )

        /*DCI_1C_Comment End */
    {
        lteWarning(
                "[%s]: ERROR TB size of received paging message (%d) > %d bits",
                __func__,MAX_PAGING_MSG_SIZE_FOR_DCI_FORMAT_1A,
                dataLen);
        return MAC_FAILURE;
    }

    getSFAndSFN(&currentSFN, &currentSF,internalCellIndex);
    /*Cyclomatic Complexity changes - Starts here */

    updateCreateNodeFlag(pagingFrameNum, pagingOccasion, &createNodeFlag, 
            currentSFN, &currentSF,numSFN, &wrapAroundFlag,sysFrameNum);


    /*Cyclomatic Complexity changes - end here */

    if (TRUE == createNodeFlag)
    {
        UInt32 index = 0;
        /* SPR 1614 changes start */
        UInt8 numRB = 0;
        UInt8 mcsIndex = 0;
        /* SPR 13250 fix start */
        UInt16 tbSize = 0;
        /* SPR 13250 fix start */
        UInt8 sfnGap = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                       ->cellParams_p->sfnGap;
        /* SPR 23797 fix start */
		UInt32 pagingCycle = getPagingCycle(internalCellIndex);
        /* SPR 23797 fix end */
        UInt16 pagingRepetitions;
        /*DCI_1C_Comment Start */

        /*SPR 6208 Start*/

        /*KLOCKWORK_CHANGES_START_2 */
        if(DCI_FORMAT_1A ==  dciFormat )
        {
            determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1A
                (
                 dataLen,
                 &tbSize,
                 &numRB,
                 &mcsIndex,
                 &tpc,
                 cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p
                );
        }
        else if(DCI_FORMAT_1C ==  dciFormat )
        {
            determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1C(
                    dataLen,
                    &tbSize,
                    &numRB,
                    &mcsIndex,
                    &tpc,
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p);

        }

			/* Populate the pcchMsgArr based on the Num of SFNs */

        /*SPR 6208 End*/
        /*KLOCKWORK_CHANGES_STOP_2 */
        /*DCI_1C_Comment End */

        /* -COVERITY 10012*/

        GET_MEM_FROM_POOL(PCCHInfoNode , pcchInfoNode_p , sizeof(PCCHInfoNode),
                PNULL);
        /*+COVERITY 5.3.0 - 25477, 24500 */
        if(PNULL == pcchInfoNode_p )
        {
            return MAC_FAILURE;
        }
        /*-COVERITY 5.3.0 - 25477, 24500 */

        GET_MEM_FROM_POOL(UInt8 , pcchInfoNode_p->pagingData_p , 
                (tbSize + DATA_MSG_API_HEADER_LEN) , PNULL);
        /*+COVERITY 5.3.0 - 25477, 24500 */
        if( PNULL == pcchInfoNode_p->pagingData_p)
        {
            /*KLOCKWORK_CHANGES_START_68*/ 
            freeMemPool(pcchInfoNode_p);
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,MAC_PCCH,
                    getCurrentTick(), __LINE__,
                    sizeof(PCCHInfoNode), 0,0,0,
                    0.0,0.0,__FILE__,__func__);
            /*KLOCKWORK_CHANGES_STOP_68*/ 
            return MAC_FAILURE ;
        }
        /*-COVERITY 5.3.0 - 25477, 24500 */

        memSet(pcchInfoNode_p->pagingData_p, 0, 
                (tbSize + DATA_MSG_API_HEADER_LEN));
        /* SPR 1614 changes end */

        memCpy((pcchInfoNode_p->pagingData_p + DATA_MSG_API_HEADER_LEN),
                data_p, dataLen);

        pcchInfoNode_p->dataLen = tbSize;
        pcchInfoNode_p->numRB   = numRB;
        pcchInfoNode_p->mcsIndex = mcsIndex;
        pcchInfoNode_p->numRepititions = 0;

        /* Wrap around case. Hence increment the Paging Cycle */ 
        if (wrapAroundFlag == TRUE)
        {
            pagingCycle++;
            if (pagingCycle > MAX_PAGING_CYCLE)
            {
                pagingCycle = 0;
            }
        }

        pagingFrameNum = pagingFrameNum + (pagingCycle * MAX_SFN_VALUE);

        if (pagingFrameNum > MAX_PAGING_SFN_RANGE)
        {
            pagingFrameNum = pagingFrameNum - (MAX_PAGING_SFN_RANGE+1);
        }


        /* SI Modification Case */
        if (numSFN > 1)
        {     
            numPagingOccasions = cellSpecificParams_g.cellConfigAndInitParams_p\
                                 [internalCellIndex]->cellParams_p->Ns;
            pagingRepetitions = numSFN/sfnGap;
        }
        else
        {  
		pagingIdx = GET_PAGING_INDEX(pagingOccasion,internalCellIndex);
		numPagingOccasions = 1;
		pagingRepetitions = 0;  
        }        

        /*Coverity 25496 Fix Start*/
        if(0 == numPagingOccasions)
        {
            freeMemPool(pcchInfoNode_p->pagingData_p);
            /* SPR 11109 28-JUN-2014 Fix start */
            pcchInfoNode_p->pagingData_p = PNULL;
            /* SPR 11109 28-JUN-2014 Fix end */
            freeMemPool(pcchInfoNode_p);
            /* SPR 11109 28-JUN-2014 Fix start */
            pcchInfoNode_p = PNULL;
            /* SPR 11109 28-JUN-2014 Fix end */
        }
        /*Coverity 25496 Fix End*/

        /* Populate the pcchMsgArr based on the Num of SFNs */
        do 
        {
            index =  pagingFrameNum;

            if (numSFN > 1)
            {    
                pagingIdx = 0;  
            }

            /*If Paging message already present free the memory first and 
             *Insert the new received message
             */
            for (i = 0; i < numPagingOccasions; i++)
            {
                if (PNULL !=  pcchGlobals_g[internalCellIndex].pcchMsgArr[index][pagingIdx])
                {
                    PCCHInfoNode *pcchNode_p = pcchGlobals_g[internalCellIndex].pcchMsgArr[index][pagingIdx];

                    if (pcchNode_p->pagingData_p)
                    {
                        pcchNode_p->numRepititions--;

                        if (!pcchNode_p->numRepititions)
                        {    
                            freeMemPool(pcchNode_p->pagingData_p);
                            pcchNode_p->pagingData_p = PNULL;//SPR 11109 28-JUN-2014 Fix
                            freeMemPool(pcchNode_p);
                        }
                    }
                    else
                    {    
                        freeMemPool(pcchNode_p);
                    }

                    pcchGlobals_g[internalCellIndex].pcchMsgArr[index][pagingIdx] = PNULL;
                }

                LTE_MAC_UT_LOG(LOG_INFO,PCCH_MGR,
                        "[%s]: Inserting the received message at index (%d, %d)\n",
                        __func__,index, pagingIdx);

                pcchGlobals_g[internalCellIndex].pcchMsgArr[index][pagingIdx] = pcchInfoNode_p;

                pagingIdx++;
                pcchInfoNode_p->numRepititions++;
            }
            pagingFrameNum += sfnGap;

            if (pagingFrameNum > MAX_PAGING_SFN_RANGE)
            {
                pagingFrameNum = pagingFrameNum - (MAX_PAGING_SFN_RANGE+1);
            }

            if (pagingRepetitions)
            {
                pagingRepetitions--;
            }

        }while (pagingRepetitions > 0);

	}

    else
    {
        lteWarning("MAC_SFN_ERROR: pagingFrameNum [%d] pagingOccasion [%d] "
                "currentSFN [%d] currentSF [%d]\n", 
                pagingFrameNum, pagingOccasion,  currentSFN, currentSF);

        LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                "Exit [%s]\n",__func__);

        return MAC_SFN_ERROR;
    }

    LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
            "Exit [%s]\n",__func__);

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : schedulePCCHMsg
 * Inputs         : pdcchDLUEOutputInfo_p - pointer to the PdcchDLOutputInfo 
 *                                          container,
 *                  availableCommonCCE_p - Available CCE in common search space,
 *                  availableDLRBCount_p - Available DL RB Count,
 *                  subFrameNum - Sub frame number w.r.t dl delay
 *                  sysFrameNum - Sys frame number w.r.t dl delay
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : count - Number of PDU scheduled
 * Description    : This fuction will process the pcchDataQueue_g and schedules 
 *                  the paging message.
 *****************************************************************************/
UInt32 schedulePCCHMsg(
                      PdcchDLOutputInfo *pdcchDLUEOutputInfo_p, 
                      UInt32 *availableCommonCCE_p,
                      SInt32 *availableDLRBCount_p,
                      UInt32 subFrameNum,
                      UInt32 sysFrameNum,
                      InternalCellIndex internalCellIndex
                      )
{
    UInt32 count         = 0;
    UInt32 pagingSfn     = 0;
    UInt8  pagingIdx     = GET_PAGING_INDEX(subFrameNum,internalCellIndex);
    PCCHInfoNode *pcchNode_p   = PNULL;
    UInt8  pagingCycle    = getPagingCycle(internalCellIndex);
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt32 agregationLvl = 0;
    UInt8  numRB = 0;
    UInt8  freePagingNode = FALSE;
    void * dataTB_p = PNULL;
    void * dataTB_temp_p = PNULL;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */ 
    /* Check For paging ocassion */
    if ( 0xFF == pagingIdx)
    {
        return count; 
    }

    LTE_MAC_UT_LOG(LOG_INFO,PCCH_MGR,
            "Entry [%s]\n",__func__); 

    /* Check SFN SF 1023,9 for wrap around case */     
    if ((sysFrameNum == 0) && 
            (subFrameNum == 0)
       )
    {
        pagingCycle++;

        if (pagingCycle > MAX_PAGING_CYCLE)
        {
            pagingCycle = 0;
        }
    }    

    pagingSfn = (pagingCycle*MAX_SFN_VALUE) + sysFrameNum;

    pcchNode_p = pcchGlobals_g[internalCellIndex].pcchMsgArr[pagingSfn][pagingIdx];

    if ((pcchNode_p)&&(pcchNode_p->pagingData_p))
    {
        DLPCCHMsgInfo       *pcchMsgInfo_p = PNULL; 
        PdcchDLOutputInfo   *pdcchDLInfo_p = PNULL;

        agregationLvl = AGGREGATION_LEVEL_FOR_COMMON_SPACE;

        if (agregationLvl <= *availableCommonCCE_p)
        {
            numRB = pcchNode_p->numRB;    
            if (*availableDLRBCount_p >= numRB)
            {
                pdcchDLInfo_p = pdcchDLUEOutputInfo_p;
                /* ICIC changes start */
                pcchMsgInfo_p = &pdcchDLInfo_p->dlMsgTypeCommonMsgInfo[ 
                    pdcchDLInfo_p->countCommonChannelMsg].
                    dlMsgInfo.dlPCCHMsgInfo;

                dataTB_p = msgAlloc(PNULL,0,0,0);

                /* +COVERITY 10868 */
                if(dataTB_p == PNULL)
                {
                    ltePanic("msgAlloc failed \n");
                }

                msgReserve(dataTB_p,0, pcchNode_p->dataLen+DATA_MSG_API_HEADER_LEN);
                dataTB_temp_p = msgSegNext(dataTB_p,0,&segment);

                if(dataTB_temp_p == PNULL)
                {
                    msgFree(dataTB_p);
                    /* SPR 7385 Changes Starts */
                    /* SPR 7976 */
                    lteWarning("Paging messages can not be scheduled \n");
                    /* SPR 7976 */
                    /* SPR 7385 Changes Ends */
                    return count;
                }
                /* -COVERITY 10868 */
                dataTB_temp_p = segment.base;
                memCpy(dataTB_temp_p, pcchNode_p->pagingData_p, pcchNode_p->dataLen);
                pcchMsgInfo_p->data_p  = dataTB_p;
                pcchMsgInfo_p->dataLen = pcchNode_p->dataLen;

                /*DCI_1C_Comment Start */
                pcchMsgInfo_p->dciFormat = cellParams_p->dciFormatForCommonChnlMsg;
                /*DCI_1C_Comment End */

                pcchMsgInfo_p->resourceAllocInput.requiredRB = numRB;
                pcchMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = 
                    pcchNode_p->mcsIndex;
/* SPR_10231_FIX_START */
                pcchMsgInfo_p->tpc = getTpcFromMcsTbSize(pcchNode_p->dataLen+DATA_MSG_API_HEADER_LEN);
/*  SPR_10231_FIX_END */

                *availableDLRBCount_p -= numRB;
                *availableCommonCCE_p -= agregationLvl;
                count++;

                LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                        "[%s]: Paging message scheduled successfully\n"
                        "agregationLvl [%d] numRB [%d]"
                        "*availableDLRBCount_p [%d] *availableCommonCCE_p [%d]\n",
                        __func__,agregationLvl,numRB,*availableDLRBCount_p,
                        *availableCommonCCE_p);
            }
            else /* Free the paging Node In case RBs are not available */
            {
                LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                        "[%s]: ERROR Required number of RBs [%d] are greater"
                        " than the DL available RBs [%d]\n",
                        __func__,numRB,*availableDLRBCount_p); 

                freePagingNode = TRUE;
            }
        }
        else /* Free the paging Node In case the CCE are not available */
        {
            freePagingNode = TRUE;
        }
        if (TRUE == freePagingNode)
        { 
            pcchNode_p->numRepititions--;

            if (!pcchNode_p->numRepititions)
            {   
                freeMemPool(pcchNode_p->pagingData_p);
                pcchNode_p->pagingData_p = PNULL;//SPR 11109 28-JUN-2014 Fix
                freeMemPool(pcchNode_p);
            }

            pcchGlobals_g[internalCellIndex].pcchMsgArr[pagingSfn][pagingIdx] = PNULL;
        }

    }
    else
    {
        LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                "[%s]: ERROR agregationLvl [%d] is less than the"
                "available CCE in common space [%d] free the memory\n",
                __func__,agregationLvl,*availableCommonCCE_p);

        if (pcchNode_p)
        {    
            freeMemPool(pcchNode_p);
            pcchGlobals_g[internalCellIndex].pcchMsgArr[pagingSfn][pagingIdx] = PNULL;
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,PCCH_MGR,
            "Exit [%s]\n",__func__);

    return count;
}

/*****************************************************************************
 * Function Name  : calculatePagingOccasion 
 * Inputs         : Ns
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Calculates the Paging Occasion and stores in the global
 *                  array 
 * Returns        : None
 * Description    : This fuction derives the Paging Occasions using the nB
 *                  parameter (SIB2). List of possible Paging Occasion depend
 *                  upon Ns = MAX (1, nB/T). nB is in terms of T. 
 *                  3GPP TS 36.304, Section 7.2.
 *****************************************************************************/
void calculatePagingOccasion(UInt8 Ns,InternalCellIndex internalCellIndex)
{
#ifdef FDD_CONFIG

    if (Ns == 4) 
    {
        /* Populate the Page to Idx Array */   
        pcchGlobals_g[internalCellIndex].pageSf2Idx[0] = 0; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[4] = 1; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[5] = 2; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[9] = 3;

        pcchGlobals_g[internalCellIndex].firstPagingOccasion = 0;
    }    
    else if (Ns == 2) 
    {
        /* Populate the Page to Idx Array */   
        pcchGlobals_g[internalCellIndex].pageSf2Idx[4] = 0; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[9] = 1; 

        pcchGlobals_g[internalCellIndex].firstPagingOccasion = 4;
    }
    else if (Ns == 1)
    {
        /* Populate the Page to Idx Array */   
        pcchGlobals_g[internalCellIndex].pageSf2Idx[9] = 0; 

        pcchGlobals_g[internalCellIndex].firstPagingOccasion = 9;
    }
#elif TDD_CONFIG
    if (Ns == 4) 
    {
        /* Populate the Page to Idx Array */   
        pcchGlobals_g[internalCellIndex].pageSf2Idx[0] = 0; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[1] = 1; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[5] = 2; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[6] = 3; 
    }    
    else if (Ns == 2) /* Ns = 2 */
    {
        /* Populate the Page to Idx Array */   
        pcchGlobals_g[internalCellIndex].pageSf2Idx[0] = 0; 
        pcchGlobals_g[internalCellIndex].pageSf2Idx[5] = 1; 
    }
    else if (Ns == 1) /* Ns = 1 */
    {
        /* Populate the Page to Idx Array */   
        pcchGlobals_g[internalCellIndex].pageSf2Idx[0] = 0; 
    }        
    pcchGlobals_g[internalCellIndex].firstPagingOccasion = 0;
#endif
}


/*****************************************************************************
 * Function Name  : freePagingMsg 
 * Inputs         : sysFrame,
 *                  subFrame
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This fuction checks the current Paging Node and frees it.
 *****************************************************************************/
void freePagingMsg(UInt32 sysFrame, UInt32 subFrame,
        InternalCellIndex internalCellIndex)
{
    UInt32 pagingSfn     = 0;
    UInt8  pagingIdx     = GET_PAGING_INDEX(subFrame,internalCellIndex);
    PCCHInfoNode *pcchNode_p   = PNULL;
    UInt8  pagingCycle    = getPagingCycle(internalCellIndex);

    if (pagingIdx == 0xFF)
    {
        ltePanic ("[%d %d] Invalid Paging Tx\n", sysFrame, subFrame);
        return;
    }


    /* Check SFN SF 1023,9 for wrap around case */
    if ((sysFrame == 0) &&
            (subFrame == 0)
       )
    {
        pagingCycle++;

        if (pagingCycle > MAX_PAGING_CYCLE)
        {
            pagingCycle = 0;
        }
    }

    pagingSfn = (pagingCycle*MAX_SFN_VALUE) + sysFrame;

    pcchNode_p = pcchGlobals_g[internalCellIndex].pcchMsgArr[pagingSfn][pagingIdx];


    if (pcchNode_p)
    {
        pcchNode_p->numRepititions--;

        if (!pcchNode_p->numRepititions)
        {
            freeMemPool(pcchNode_p->pagingData_p);
            pcchNode_p->pagingData_p = PNULL;//SPR 11109 28-JUN-2014 Fix
            freeMemPool(pcchNode_p);
        }

        pcchGlobals_g[internalCellIndex].pcchMsgArr[pagingSfn][pagingIdx] = PNULL;
    }
}





/*Cyclomatic Complexity changes - Starts here */
/*****************************************************************************
 * Function Name  : updateCreateNodeFlag 
 * Inputs         : pagingFrameNum,
 *                  pagingOccasion,
 *                  createNodeFlag,
 *                  currentSFN
 *                  currentSF,
 *                  numSFN,
 *                  wrapAroundFlag,
 *                  sysFrameNum
 * Outputs        : None 
 * Returns        : None
 * Description    : This fuction update the Create Node Flag.
 *****************************************************************************/

static  void updateCreateNodeFlag(UInt32   pagingFrameNum, 
                                        UInt8    pagingOccasion, 
                                        UInt32 * createNodeFlag, 
                                        UInt32   currentSFN, 
                                        UInt32 * currentSF,
                                        UInt16   numSFN,
                                        UInt8  * wrapAroundFlag,
                                        UInt32   sysFrameNum )
{
    if (pagingFrameNum > currentSFN)
    {
        UInt32 numSysFramediff = 0;

        numSysFramediff = pagingFrameNum - currentSFN;

        /*Check that Paging message should receive atleast 
         *PAGING_MSG_IN_ADVANCE_MIN (5 milliseconds) and 
         *PAGING_MSG_IN_ADVANCE_MAX (32 Radio frames) in advance
         */
        /*If the difference is 0/1 check the difference between
         *Paging Occation and current Sub Frame number
         */
        if (numSysFramediff == 1)
        {
            if (pagingOccasion < *currentSF)
            {
                /* SPR_5037_FIX Start */
                if ((*currentSF - pagingOccasion)> PAGING_MSG_IN_ADVANCE_MIN)
                    /* SPR_5037_FIX End */
                {
                    *createNodeFlag = FALSE;
                }
            }
        }
        /* In Case of SysInfo Modification  (numSFN > 0), the check for Max 
         * limit is not needed */
        else if ((numSFN == 1) && 
                (numSysFramediff > PAGING_MSG_IN_ADVANCE_MAX)
                )
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                    "[%s]: ERROR Paging message received [%d] RF before\n",
                    __func__,numSysFramediff);  

            *createNodeFlag = FALSE;
        }
    }
    /* Check for 5 milisecond Advance */
    else if (pagingFrameNum == currentSFN)
    {
        if ((pagingOccasion <= *currentSF) ||
                ((pagingOccasion - *currentSF) < PAGING_MSG_IN_ADVANCE_MIN))
        {
            *createNodeFlag = FALSE;  
        }
    }
    else /* SFN wrap around case */
    {
        *wrapAroundFlag = TRUE;
        if  (currentSFN < (MAX_SFN_VALUE - PAGING_MSG_IN_ADVANCE_MAX)) 
        {
            /* In Case of SysInfo Modification (numSFN > 0), the check for Max 
             * limit is not needed */
            if (numSFN == 1)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                        "[%s]: ERROR Paging message received [%d] RF before\n",
                        __func__,(sysFrameNum - pagingFrameNum)); 

                *createNodeFlag = FALSE;
            }

            /* Case for handling when paging frame number is less than current
               SFN and Wrap around case is not valid for current SFN.
               And when Paging frame number is not within window of 5 SF to 320 SF */
            if(pagingFrameNum < currentSFN)
            {
                *createNodeFlag = FALSE;
            }
        }
        else
        {
            /*This is wrap around case*/
            LTE_MAC_UT_LOG(LOG_INFO,PCCH_MGR,
                    "[%s]: This is the round of case currentSFN [%d] PF [%d]\n",
                    __func__,currentSFN, pagingFrameNum);

            /*based on the PAGING_MSG_IN_ADVANCE_MAX calculate the 
             *maximum system frame number value
             */
            sysFrameNum = (currentSFN + PAGING_MSG_IN_ADVANCE_MAX) % 1024;

            if (pagingFrameNum > sysFrameNum) 
            {
                /* In Case of SysInfo Modification (numSFN > 0), the check for Max 
                 * limit is not needed */
                if (numSFN == 1)
                {
                    LTE_MAC_UT_LOG(LOG_WARNING,PCCH_MGR,
                            "[%s]: ERROR Paging message received [%d] RF before\n",
                            __func__,(sysFrameNum - pagingFrameNum));  

                    *createNodeFlag = FALSE;
                }
            }
            else
            {
                /*If the SFN is 1023 and PF is 0, check for the no.of 
                 *milliseconds before Paging message received
                 */
                if (((MAX_SFN_VALUE - 1) == currentSFN) && 
                        (!pagingFrameNum) &&
                        (*currentSF > PAGING_MSG_IN_ADVANCE_MIN))
                {
                    *currentSF = (*currentSF + PAGING_MSG_IN_ADVANCE_MIN) % 
                        MAX_SUB_FRAME;

                    if (pagingOccasion <= *currentSF)
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,PCCH_MGR,
                                "[%s]: ERROR Paging message received [%d]ms\n"
                                "before",__func__,(pagingOccasion - *currentSF));                
                        *createNodeFlag = FALSE;
                    }
                }
            }
        }
    }   
}
/*Cyclomatic Complexity changes - end here */

/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : validatePagingParams 
 * Inputs         : numSFN
 *                  pagingOccasion_p
 *                  pagingFrameNum
 *                  internalCellIndex
 * Outputs        : None 
 * Returns        : MAC_FAILURE/MAC_SUCCESS
 * Description    : This fuction validates the Paging parms which is received
 *                  on paging message.
 *****************************************************************************/
UInt32 validatePagingParams(UInt16 numSFN,
        UInt8 *pagingOccasion_p,
        UInt32 pagingFrameNum,
	UInt8     emtcPaging,
        InternalCellIndex internalCellIndex)
{

    /* In case of SI Paging calculate the PO */
    if (numSFN > 1)
    {
        /* The starting PO in an SFN */

        *pagingOccasion_p = pcchGlobals_g[internalCellIndex].firstPagingOccasion;
    }

    if (pagingFrameNum >= MAX_SFN_VALUE)
    {
        lteWarning("[%s]: ERROR: Invalid data pagingFrameNum [%d]\n",
                __func__, pagingFrameNum);

        return MAC_SFN_ERROR;
    }
    if(*pagingOccasion_p >= MAX_SUB_FRAME)
    {
        lteWarning("[%s]: ERROR: Invalid data pagingFrame [%d]\n",
                __func__, *pagingOccasion_p);

        return MAC_FAILURE;
    }

    if ((numSFN == 1) && (0xFF == GET_PAGING_INDEX((*pagingOccasion_p),internalCellIndex)))
    {
        lteWarning("[%s]: ERROR: Invalid data pagingOccasion [%d]\n",
                __func__,*pagingOccasion_p);
        return MAC_FAILURE;
    }



    /* +coverity_10136 */
    if (numSFN > MAX_PAGING_SFN_RANGE)
        /* -coverity_10136 */
    {
        lteWarning("[%s]: ERROR: Invalid data Num Of SFNs [%d] \n",
                __func__,numSFN);

        return MAC_FAILURE;
    }


    return MAC_SUCCESS;
}
/* Cyclomatic_complexity_changes_end */

