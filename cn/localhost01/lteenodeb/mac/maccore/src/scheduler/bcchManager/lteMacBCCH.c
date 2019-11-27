/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacBCCH.c,v 1.1.1.1.6.2.4.2.2.5 2010/10/20 06:07:10 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains functions for receiving,validating and
 *                     scheduling the MIB/SIB type 1/SI messages.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacBCCH.c,v $
 * Revision 1.1.1.1.6.2.4.2.2.5  2010/10/20 06:07:10  gur20491
 * Tweak for SFN
 *
 * Revision 1.1.1.1.6.2.4.2.2.4  2010/10/15 10:58:14  gur20491
 * *Removed Compilation errors for RRC_2_0_1 Integration.
 *
 * Revision 1.1.1.1.6.2.4.2.2.3  2010/10/15 10:43:25  gur20491
 * *Improved functionality for SI parsing as introduced in RRC_2_0_1
 * *Applied the uniform Macro for enabling RRC_2_0_1 support
 *
 * Revision 1.1.1.1.6.2.4.2.2.2  2010/10/14 17:26:55  gur19413
 * RRC_2_0_1_INT
 *
 * Revision 1.1.1.1.6.2.4.2.2.1  2010/09/21 15:46:13  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.39.34.1  2010/07/21 09:39:05  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Merged Endian fixes for receiving data from RRC.
 * 2. Added a tweak defined for parsed systemframe number.
 *
 * Revision 1.39  2009/07/31 12:01:21  gur15697
 * splint warnings
 *
 * Revision 1.38  2009/07/26 08:14:57  gur15697
 * compilation warnings removed
 *
 * Revision 1.37  2009/07/13 11:57:59  gur15697
 * second argument to LTE_MAC_UT_LOG changed
 *
 * Revision 1.36  2009/07/12 10:17:39  gur15697
 * removed compilation errors
 *
 * Revision 1.35  2009/07/12 05:15:42  gur15697
 * UT bugFix
 *
 * Revision 1.34  2009/07/10 07:04:59  gur15697
 * compilation warnings removed
 *
 * Revision 1.33  2009/07/10 06:51:26  gur15697
 * sysFrameNum = sysFrameNum % MAX_SIMAP_TABLE_NODES added
 *
 * Revision 1.32  2009/07/08 15:15:09  gur15697
 * ZERO_BUFFER_COPY removed from cleanup function
 *
 * Revision 1.31  2009/07/03 11:54:15  gur15697
 * validation of start SF modified if siWindowSize is 1ms
 *
 * Revision 1.30  2009/07/01 15:12:46  gur15697
 * memSetting the memory bofore copy
 *
 * Revision 1.29  2009/07/01 06:20:52  gur15697
 * LTE_LOG changed to LTE_MAC_UT_LOG
 *
 * Revision 1.28  2009/06/25 07:13:03  gur15697
 * UT Bug fixes
 *
 * Revision 1.27  2009/06/22 08:47:18  gur15697
 * review comments incorporated
 *
 * Revision 1.26  2009/06/20 12:37:22  gur15697
 * review comments incorporated
 *
 * Revision 1.25  2009/06/20 11:25:31  gur15697
 * review comments incorporated
 *
 * Revision 1.24  2009/06/19 06:17:50  gur11083
 * updated for cell config
 *
 * Revision 1.23  2009/06/19 05:26:52  gur15697
 * review comments incorporated
 *
 * Revision 1.22  2009/06/18 10:17:02  gur15697
 * Review comments incorporated
 *
 * Revision 1.21  2009/06/18 04:53:08  gur15697
 * UT bugFix
 *
 * Revision 1.20  2009/06/17 05:12:02  gur12140
 * Removed Allocated MIB Funtiion as already taken care in PDSCH
 *
 * Revision 1.19  2009/06/16 05:40:53  gur15697
 * printfs removed
 *
 * Revision 1.18  2009/06/15 11:58:18  gur15697
 * UT bug fix
 *
 * Revision 1.17  2009/06/12 12:28:50  gur15697
 * UT bug fixes
 *
 * Revision 1.16  2009/06/11 13:21:15  gur11083
 * UT bug fix
 *
 * Revision 1.15  2009/06/11 11:40:27  gur15697
 * UT bug fixes
 *
 * Revision 1.14  2009/06/08 15:38:26  gur15697
 * UT bug fix
 *
 * Revision 1.13  2009/06/05 08:04:00  gur15697
 * init and cleanup functions added
 *
 * Revision 1.12  2009/06/05 04:12:37  gur15697
 * updated the file w.r.t PDCCH manager changes
 *
 * Revision 1.11  2009/06/03 12:00:30  gur15697
 * initBCCHMgr() added
 *
 * Revision 1.10  2009/05/28 05:51:21  gur15697
 * comments encorporated
 *
 * Revision 1.9  2009/05/27 09:44:38  gur15697
 * compilation errors removed
 *
 * Revision 1.8  2009/05/26 10:25:24  gur15697
 * Function name receiveAndStoreBroadcastMsg() changed to parseAndStoreBroadcastMsg()
 *
 * Revision 1.7  2009/05/20 05:11:39  gur15697
 * comments incorporated
 *
 * Revision 1.6  2009/05/19 13:58:04  gur15697
 * memCpy() handling added which is needed for Transmitter
 *
 * Revision 1.5  2009/05/18 08:56:26  gur15697
 * Review comments incorporated
 *
 * Revision 1.4  2009/05/13 11:15:59  gur15697
 * Validation for SI-Window length and SI-Periodicity added
 *
 * Revision 1.3  2009/05/12 14:24:16  gur15697
 * comments added
 *
 * Revision 1.2  2009/05/12 10:42:07  gur15697
 * Code added for BCCH
 *
 * Revision 1.1  2009/04/30 05:40:51  gur15697
 * Initial vertion
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
#include "lteMacParseOAMMsg.h"
#include "lteMacBCCH.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
/******************************************************************************
 Private Definitions
 *****************************************************************************/
/*Contains new and current MIB message information*/

/* SIB8_Code Start */
UInt8   siMsg8Indx_g[MAX_NUM_CELL] ={0};
/* SIB8_Code End */
/******************************************************************************
 Private Types
 *****************************************************************************/
    /* SPR 3062 Chg */
    /*--coverity-530-32298*/
    RedundancyVerVal dci1ARiV[4] = {RED_VER_0, RED_VER_2, RED_VER_3, RED_VER_1};
    /*--coverity-530-32298*/
    /* SPR 3062 Chg Ends */
/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
#ifdef LTE_EMBMS_SUPPORTED
extern UInt8 isEmbmsValidSf_g[MAX_NUM_CELL];
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : initBCCHMgr
 * Inputs         : numOfCells -  Number of cells to be configured in system
 * Outputs        : None
 * Returns        : None
 * Description    : This will initializes the global queues and variables 
 *                  required for BCCH manager. 
 *****************************************************************************/
void initBCCHMgr(UInt8 numOfCells)
{
    InternalCellIndex cellIndex = 0;
    LTE_MAC_UT_LOG(LOG_DETAIL ,BCCH_MGR,
           "[%s]: initBCCHMgr()\n",__func__);
    for(cellIndex =0;cellIndex < numOfCells; cellIndex++)
    {
        CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->\
                                         cellParams_p;
        memSet(&(cellParams_p->siMsgInfo), 0, sizeof(SIInfo));
        memSet(&(cellParams_p->mibInfo), 0, sizeof(MIBMsgInfo));
        memSet(&(cellParams_p->sib1MsgInfo), 0, sizeof(SIBType1Info));
   /* SIB8_Code Start */
    /* +COVERITY 25151 */
        memSet(&(cellParams_p->siMsg8buff[0]), 0, sizeof(SIMsg8));
        memSet(&(cellParams_p->siMsg8buff[1]), 0, sizeof(SIMsg8));
    /* -COVERITY 25151 */
   /* SIB8_Code End */
    }

    return;
}

/****************************************************************************
* Function Name  : cleanupBCCHMgr
* Inputs         : internalCellIndex - Cell-Index used at MAC
* Outputs        : None
* Returns        : None
* Description    : This API cleanup the BCCH Manager.
****************************************************************************/
void cleanupBCCHMgr(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;

    LTE_MAC_UT_LOG(LOG_DETAIL ,BCCH_MGR,
            "[%s]: cleanupBCCHMgr()\n",__func__);

    /*Clean MIB*/
    /* Review comment fix start GG6 */
    UInt32 numMIBs     = 0;
    UInt32 activeIndex = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    MIBMsgInfo *mibMsgInfo_p = &(cellParams_p->mibInfo);
    SIBType1Info *sib1MsgInfo_p = &(cellParams_p->sib1MsgInfo );
    SIInfo       *siMsgInfo_p = &(cellParams_p->siMsgInfo );
    for (activeIndex = 0; activeIndex < 2; activeIndex++)
    {
        for (numMIBs = 0; numMIBs < MAX_MIB_FROM_RRC; numMIBs++)
        {
            if (mibMsgInfo_p->mibMsg[activeIndex][numMIBs].msg_p)
            {
                freeMemPool(mibMsgInfo_p->mibMsg[activeIndex][numMIBs].msg_p);
                /*SPR 17048 Fix Start*/
                mibMsgInfo_p->mibMsg[activeIndex][numMIBs].msg_p = PNULL;
                mibMsgInfo_p->mibMsg[activeIndex][numMIBs].dataLen = 0;   
                mibMsgInfo_p->mibMsg[activeIndex][numMIBs].isMIBPresent = 0;   
                /*SPR 17048 Fix End*/
            }
        }
    }
    /* Review comment fix end */
    memSet(mibMsgInfo_p, 0, sizeof(MIBMsgInfo)); 


    for (i = 0; i < 2; i++)
    {
        /*Clean SIB Type-1*/
        if (sib1MsgInfo_p->sibType1Msg[i].msg_p)
        {
            freeMemPool(sib1MsgInfo_p->sibType1Msg[i].msg_p);
            /* SPR 10799 [BUG 833] FIX START */
            sib1MsgInfo_p->sibType1Msg[i].msg_p = PNULL;
            /* SPR 10799 [BUG 833] FIX END */
        }

        /*Clean SI*/
        cleanSIMsg(siMsgInfo_p, i, i);
        /* SIB8_Code Start */
        cleanS8buffer(&(cellParams_p->siMsg8buff[i]));
        /* SIB8_Code End */
    }
    /* SPR 13872 fix start */
    memSet((void *) siMsgInfo_p , 0,sizeof(SIInfo));
    memSet((void *) sib1MsgInfo_p , 0,sizeof(SIBType1Info));
    /* SPR 13872 fix end */
}

/**********************************************************************************
 * Function Name  : scheduleBroadcastMsgs
 * Inputs         : pdcchDLOutputInfo_p - Pointer to PdcchDLOutputInfo container,
 *                  dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                  availableCommonCCE_p - Available CCE in common search space,
 *                  sysFrameNum - System frame number w.r.t DL delay,
 *                  subFrameNum - Sub frame number w.r.t DL delay,
 *                  availableDLRBCount_p - No.of available RBs
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo
 * Returns        : count - No.of PDUs scheduled
 * Description    : This will schedules the MIB/SIBType1/SI messages.
 **********************************************************************************/
UInt32 scheduleBroadcastMsgs(PdcchDLOutputInfo  *pdcchDLOutputInfo_p,
                           DciCCEContainerInfo *dciCCEContainerInfo_p,
                           UInt32 *availableCommonCCE_p, 
                           UInt32 sysFrameNum, 
                           UInt32 subFrameNum, 
                           SInt32 *availableDLRBCount_p
                           ,InternalCellIndex internalCellIndex)
{
    UInt32 count = 0;
    SIBType1Info *sib1MsgInfo_p = &(cellSpecificParams_g.
            cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->sib1MsgInfo);

    if (0 == subFrameNum)
    {
        /*MIB uses a fixed schedule with a periodicity of 40 ms and repetitions 
         *made within 40 ms. The first transmission of the MIB is scheduled in 
         *subframe #0 of radio frames for which the SFN mod 4 = 0, and 
         *repetitions are scheduled in subframe #0 of all other radio frames
         */ 
        scheduleMIB(sysFrameNum, 
                /* +- SPR 17777 */
                dciCCEContainerInfo_p
                , availableDLRBCount_p
                ,internalCellIndex
                );




    }
    else if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->mibInfo.firstMIBsentFlag)
    {
        if ((0 == (MODULO_TWO(sysFrameNum))) && (5 == subFrameNum))
        {
            /*SIBType1 uses a fixed schedule with a periodicity
             *of 80 ms and repetitions made within 80 ms. First transmission 
             *of SystemInformationBlockType1 is scheduled in subframe #5 of 
             *radio frames for which the SFN mod 8 = 0, and repetitions are 
             *scheduled in subframe #5 of all other radio frames for which 
             *SFN mod 2 = 0.
             */        
            if((FALSE == sib1MsgInfo_p->firstSIBT1SentFlag) && 
                    (0 == (MODULO_EIGHT(sysFrameNum))) && 
                    (5 == subFrameNum))
            {
                sib1MsgInfo_p->firstSIBT1SentFlag = TRUE;
            }

            if( sib1MsgInfo_p->firstSIBT1SentFlag)
            {
                count = scheduleSIBType1(pdcchDLOutputInfo_p, 
                        availableCommonCCE_p, 
                        sysFrameNum, 
                        /* +- SPR 17777 */
                        availableDLRBCount_p,
                        internalCellIndex);
            }

        }

    }
    /* SPR 3067 Chg */ 
    if ((sib1MsgInfo_p->firstSIBT1SentFlag))  
        /* SPR 3067 Chg Ends */ 
    {
        /*SI messages are transmitted within periodically occurring time
         *domain windows (referred to as SI-windows) using dynamic 
         *scheduling. Each SI message is associated with a SI-window and 
         *the SI-windows of different SI messages do not overlap.
         *Within the SI-window, the corresponding SI message can be 
         *transmitted a number of times in any subframe other than 
         *MBSFN subframes and subframe #5 of radio frames for which 
         *SFN mod 2 = 0
         */        
        if(subFrameNum !=5)
        { 
            count = scheduleSI(pdcchDLOutputInfo_p, 
                    availableCommonCCE_p,
                    sysFrameNum, 
                    subFrameNum,
                    availableDLRBCount_p,
                    internalCellIndex);
        }
        /* Change done to scheduled SI(s) at SF = 5 where SFN % 2 != 0 
         * As per spec 36.331 Section 5.2.1.2 */
        else if((0 != (MODULO_TWO(sysFrameNum))) && (5 == subFrameNum))
        {
            count = scheduleSI(pdcchDLOutputInfo_p, 
                    availableCommonCCE_p,
                    sysFrameNum, 
                    subFrameNum,
                    availableDLRBCount_p,
                    internalCellIndex);
        }
    }

    return count;
}

/*****************************************************************************
 * Function Name  : scheduleMIB
 * Inputs         : sysFrameNum - System frame number w.r.t DL delay,
 *                  subFrameNum - Sub frame number w.r.t DL delay,
 *                  dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo,
 *                  availableDLRBCount_p - No.of available RBs,
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : availableDLRBCount_p - No.of available RBs,
 *                  dciCCEContainerInfo_p - Pointer to DciCCEContainerInfo
 * Returns        : None
 * Description    : This will schedule the MIB and send to the Trasmitter.
 *****************************************************************************/
void scheduleMIB(UInt32 sysFrameNum, 
        /* +- SPR 17777 */
                 DciCCEContainerInfo *dciCCEContainerInfo_p
				 ,SInt32 *availableDLRBCount_p
                 ,InternalCellIndex internalCellIndex
				 )
{
    MIBMsgInfo *mibMsgInfo_p = PNULL;
    /*SPR 21589 Fix  ++*/
    UInt8 *msgPtr = PNULL;
    /*SPR 21589 Fix --*/

    void * dataTB_p = PNULL;
    void * dataTB_temp_p = PNULL;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */ 
    mibMsgInfo_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                       ->cellParams_p-> mibInfo);
    InitParams *initParams_p = cellSpecificParams_g.\
                  cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
                                
    /* CA Changes end */

    /*If new 256 MIBs received from RRC, receiveAndStoreBroadcastMsg()
     *will decode and save MIBs and set the updateFlag.
     */
    if (mibMsgInfo_p->updateFlag) 
    {
        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: updateFlag set Current\n",__func__);
        /*If activeIndex is 0, new MIBs are copied at index 1
         *So change the active index to 1 and visevarsa. 
         */
        mibMsgInfo_p->activeIndex = (mibMsgInfo_p->activeIndex)?(0):(1);
        mibMsgInfo_p->updateFlag  = FALSE;
    }

    /*Periodicity of MIB message is 40 milliseconds*/    
    if (0 == (MODULO_FOUR(sysFrameNum)))
    { 
        MIBMsg *mibInfo_p = PNULL;
        UInt32 index = 0;

        index = DIVIDE_BY_FOUR(sysFrameNum);

        mibInfo_p = &mibMsgInfo_p->mibMsg[mibMsgInfo_p->activeIndex][index];

        if (TRUE == mibInfo_p->isMIBPresent)
        {
            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: Scheduling MIB\n",__func__);

            dataTB_p = msgAlloc(PNULL,0,0,0);
            
            /* +COVERITY 10866 */
            if (PNULL == dataTB_p)
            {
                ltePanic("msgAlloc failed \n");
            }
            /*SPR 21589 Fix  ++*/
            msgPtr = msgReserve(dataTB_p,0, (mibInfo_p->dataLen+DATA_MSG_API_HEADER_LEN+MIB_LENGTH_FIELD_SIZE));
            if(PNULL == msgPtr)
            {
                msgFree(dataTB_p);
                ltePanic("%s: can't allocate buffer",__FUNCTION__);
            }
            /*SPR 21589 Fix  --*/
            dataTB_temp_p = msgSegNext(dataTB_p,0,&segment);
            
            /*SPR 21589 Fix  ++*/
            if (dataTB_temp_p == PNULL)
            {
                msgFree(dataTB_p);
                ltePanic("%s: msgSegNext() returned NULL",__FUNCTION__);
            }
            /* -COVERITY 10866 */
            /*SPR 21589 Fix  --*/
            
            dataTB_temp_p = segment.base;
            memCpy(dataTB_temp_p, mibInfo_p->msg_p, (mibInfo_p->dataLen + DATA_MSG_API_HEADER_LEN + MIB_LENGTH_FIELD_SIZE));
            dciCCEContainerInfo_p->mibData_p = dataTB_p;
            dciCCEContainerInfo_p->mibDataLen = mibInfo_p->dataLen;
            /*SPR 3306 fix starts */
            if ((!(initParams_p->commChWithMIBAllowed))&&
                            (!(initParams_p->dlSchWithMIBAllowed)))
            /*SPR 3306 fix ends */
		    *availableDLRBCount_p  -= RB_FOR_MIB;

            /*Set the flag so that SIBType1 message will be scheduled*/
            mibMsgInfo_p->firstMIBsentFlag = TRUE;
        }
    }
    else if (TRUE == mibMsgInfo_p->firstMIBsentFlag)
    {
        MIBMsg *mibInfo_p = PNULL;
        UInt32 index = 0;

        index = DIVIDE_BY_FOUR(sysFrameNum);

        mibInfo_p = &mibMsgInfo_p->mibMsg[mibMsgInfo_p->activeIndex][index];

        if (TRUE == mibInfo_p->isMIBPresent)
        {
            /*This is the MIB retransmission, PHY will handle the MIB 
             *retransmissions, MAC only need to allocate resources.
             *in subframe #0 of each SFN MIB will be retransmitted
             */
            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: Retrans of MIB\n",__func__);
            /*SPR 3306 fix starts */
            if ((!(initParams_p->commChWithMIBAllowed))&&
                     (!(initParams_p->dlSchWithMIBAllowed)))
            /*SPR 3306 fix ends */
		    *availableDLRBCount_p  -= RB_FOR_MIB;
        }
    }

    return;
}

/*****************************************************************************
 * Function Name  : scheduleSIBType1
 * Inputs         : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo,
 *                  availableCommonCCE_p - Available CCE in common search space,
 *                  sysFrameNum - System frame number w.r.t DL delay,
 *                  subFrameNum - Sub frame number w.r.t DL delay,
 *                  availableDLRBCount_p  - DL RBs available
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo,
 *                  availableDLRBCount_p  - DL RBs available,
 *                  availableCommonCCE_p - Available CCE in common search space    
 * Returns        : None
 * Description    : This will schedule the SIB type1 message.
 *****************************************************************************/
UInt32 scheduleSIBType1(PdcchDLOutputInfo  *pdcchDLOutputInfo_p, 
                      UInt32 *availableCommonCCE_p, 
                      UInt32 sysFrameNum, 
                      /* +- SPR 17777 */
                      SInt32 *availableDLRBCount_p,
                      InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    UInt32       agregationLvl  = 0;
    UInt32       count  = 0;
    SIBType1Msg  *sib1Msg_p     = PNULL;
    /* SIB1 RV changes */
    UInt16 inputRedundanVerCal  = 0;
    /* SIB1 RV changes */
    /*SPR 21589 Fix  ++*/
    UInt8 *msgPtr = PNULL;
    /*SPR 21589 Fix  --*/
    void * dataTB_p = PNULL;
    void * dataTB_temp_p = PNULL;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */ 

    SIBType1Info *sib1MsgInfo_p = &(cellParams_p->sib1MsgInfo);

    if (sib1MsgInfo_p->updateFlag)
    {
        UInt32 index = 0;

        index = (sib1MsgInfo_p->activeIndex)?(0):(1);

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: Update flag set for SIB Type1 message\n",__func__);

        if (sysFrameNum == sib1MsgInfo_p->sibType1Msg[index].sfn)
        {
            /*If activeIndex is 0, new SIBType1 message is copied at index 1
             *So change the active index to 1 and visevarsa.
             */
            sib1MsgInfo_p->activeIndex = (sib1MsgInfo_p->activeIndex)?(0):(1);
            sib1MsgInfo_p->updateFlag  = FALSE;
        }
    }

    sib1Msg_p     = &(sib1MsgInfo_p->sibType1Msg[sib1MsgInfo_p->activeIndex]);
    agregationLvl = AGGREGATION_LEVEL_FOR_COMMON_SPACE;

    if ((agregationLvl <= *availableCommonCCE_p) &&
            (*availableDLRBCount_p > 0))
    {
        if (PNULL != sib1Msg_p->msg_p)
        {
            DLBCCHMsgInfo *bcchMsgInfo_p = PNULL;

            /* ICIC changes start */
            bcchMsgInfo_p = 
                &pdcchDLOutputInfo_p->\
                dlMsgTypeCommonMsgInfo[pdcchDLOutputInfo_p->countCommonChannelMsg].\
                dlMsgInfo.dlBCCHMsgInfo;
            /* ICIC changes end */

            dataTB_p = msgAlloc(PNULL,0,0,0);
            /* SPR 21117 Fix Start */
            /*SPR 21589 Fix  ++*/
            if (PNULL == dataTB_p)
            {
                ltePanic("msgAlloc failed \n");
            }
            /* SPR 21117 Fix End */
            msgPtr = msgReserve(dataTB_p,0, sib1Msg_p->msgLen+DATA_MSG_API_HEADER_LEN);
            if(PNULL == msgPtr)
            {
                msgFree(dataTB_p);
                ltePanic("%s: can't allocate buffer",__FUNCTION__);
            }
            /*SPR 21589 Fix  --*/
            dataTB_temp_p = msgSegNext(dataTB_p,0,&segment);
            /*SPR 21589 Fix  ++*/
            if(PNULL == dataTB_temp_p)
            {
                msgFree(dataTB_p);
                ltePanic("%s: msgSegNext() returned NULL",__FUNCTION__);
            } 
            /*SPR 21589 Fix  --*/
            dataTB_temp_p = segment.base;
            /*SPR 4017 PH2 Start*/
            if(dataTB_temp_p)
            {
                memCpy(dataTB_temp_p, sib1Msg_p->msg_p, sib1Msg_p->msgLen+DATA_MSG_API_HEADER_LEN);
            }
            /*SPR 4017 PH2 End*/

            bcchMsgInfo_p->dataLen = sib1Msg_p->msgLen;
            bcchMsgInfo_p->data_p = dataTB_p;
            /* SPR 8879 Fix */
            bcchMsgInfo_p->dciFormat = cellParams_p->dciFormatForCommonChnlMsg;
            /* SPR 3063 Chg */
            if( *availableDLRBCount_p >= sib1Msg_p->numRB)
            {
                bcchMsgInfo_p->resourceAllocInput.requiredRB = sib1Msg_p->numRB;
                *availableDLRBCount_p -= sib1Msg_p->numRB;
            }
            else
            {
                bcchMsgInfo_p->resourceAllocInput.requiredRB = *availableDLRBCount_p;
                *availableDLRBCount_p -= *availableDLRBCount_p;
            }
            /* SPR 3063 Chg */
            bcchMsgInfo_p->resourceAllocInput.preferredRATPolicy = 
                RA_POLICY_2_DISTRIBUTED;
            bcchMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = \
                                                                         sib1Msg_p->mcsIndex;
            /* SPR 3062 Chg */
            bcchMsgInfo_p->tpc = sib1Msg_p->tpc;
            /* SPR 3062 Chg Ends */

            if(DCI_FORMAT_1A == bcchMsgInfo_p->dciFormat)
            {
                /* SIB1 RV Changes */
                inputRedundanVerCal = DIVIDE_BY_TWO(sysFrameNum);
                inputRedundanVerCal = MODULO_FOUR(inputRedundanVerCal);
                bcchMsgInfo_p->redundancyVer = redundancyVerCalc(inputRedundanVerCal);
                /* SIB1 RV Changes */
            }
            else if (DCI_FORMAT_1C == bcchMsgInfo_p->dciFormat)
            {
                inputRedundanVerCal = DIVIDE_BY_TWO(sysFrameNum);
                inputRedundanVerCal = MODULO_FOUR(inputRedundanVerCal);
                bcchMsgInfo_p->redundancyVer = redundancyVerCalc(inputRedundanVerCal);
            }
            *availableCommonCCE_p -= agregationLvl;
            count++;


            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: SIBType1 message scheduled *availableCommonCCE_p [%d]"
                    "count [%d]\n",__func__,*availableCommonCCE_p,count); 
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: ERROR sib1Msg_p->msg_p is NULL\n",__func__);

            sib1MsgInfo_p->firstSIBT1SentFlag = FALSE;
        }
    }
    else
    {
        lteWarning("[%s]: *availableCommonCCE_p [%d] < agregationLvl [%d] or\n"
                "*availableDLRBCount_p [%d] < numRB [%d]\n",
                __func__,*availableCommonCCE_p,agregationLvl,
                availableDLRBCount_p,sib1Msg_p->numRB);
        sib1MsgInfo_p->firstSIBT1SentFlag = FALSE;
    }

    return count;
}

/* SPR_4748_FIX Start */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : siWindowMover
 * Inputs         : sysFrameNum - System frame numberi,
 *                  subFrameNum - Sub frame number
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : tddWindowVariable 
 * Description    : This function will return  tddWindowVariable which decreases 
 *                  SI window size in TDD mode.
 *****************************************************************************/
UInt8 siWindowMover(UInt32 sysFrameNum,
    UInt32 subFrameNum,
    /* CA Changes Start */
    InternalCellIndex internalCellIndex
    /* CA Changes End */
) 
{
	UInt16 tddConfigType = 0;
/* SPR_4830_FIX Start */
	UInt16 splSubFramePatterns = 0;
/* SPR_4830_FIX End */
	UInt8 tddWindowVariable = 0;
	/*
	 * tddWindowVariable will be updated based on TDD Config subframes
	 * example for TDD Config_3 tddWindowVariable is 4
	 * TDD Config 3 will be as  D S U U U D D D D D 
	 */

	tddConfigType = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign;
/* SPR_4830_FIX Start */
	splSubFramePatterns  = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->splSubFramePatterns; 

      if(splSubFramePatterns  == 0 ||  splSubFramePatterns  == 5)
      {
	      if((1 == tddConfigType) || (2 == tddConfigType) )
	      {
		      if((0 == subFrameNum) || ((5 == subFrameNum) && (MODULO_TWO(sysFrameNum) != 0)))
		      {
			      tddWindowVariable = (1 == tddConfigType)?3:2; 
		      }
		      else if((4 == subFrameNum) && (MODULO_TWO(sysFrameNum) == 0 ))
		      {
			      tddWindowVariable = (1 == tddConfigType)?4:3; 
		      }
	      }
	      else if((3 == tddConfigType) || (4 == tddConfigType) || (5 == tddConfigType) )
	      {
		      if(0 == subFrameNum)
		      {
			      if(MODULO_TWO(sysFrameNum) == 0)
			      {
				      tddWindowVariable = (3 == tddConfigType)?5:((4 == tddConfigType)?3:2);
			      }
			      else
			      {
				      tddWindowVariable = (3 == tddConfigType)?4:((4 == tddConfigType)?3:2);
			      }
		      }
		      else if((4 == subFrameNum) && (MODULO_TWO(sysFrameNum) == 0) )
		      {
			      tddWindowVariable = 1; 
		      }
	      }
      }
      else
      {
	      if((1 == tddConfigType) || (2 == tddConfigType) )
	      {
		      if((1 == subFrameNum) || ((6 == subFrameNum)))
		      {
			      tddWindowVariable = (1 == tddConfigType)?2:1; 
		      }
		      else if((4 == subFrameNum) && (MODULO_TWO(sysFrameNum) == 0 ))
		      {
			      tddWindowVariable = 1; 
		      }
	      }
	      else if((3 == tddConfigType) || (4 == tddConfigType) || (5 == tddConfigType) )
	      {
		      if(1 == subFrameNum)
		      {
			      tddWindowVariable = (3 == tddConfigType)?3:((4 == tddConfigType)?2:1);

                              if((MODULO_TWO(sysFrameNum) == 0 ) &&(3 == tddConfigType))
                              {
				      tddWindowVariable = tddWindowVariable+1;
                              }
		      }
		      else if((4 == subFrameNum) && (MODULO_TWO(sysFrameNum) == 0 ))
		      {
			      tddWindowVariable = (3 == tddConfigType)?0:1;

		      }
	      }

      } 
/* SPR_4830_FIX End */

	return tddWindowVariable;
}
#endif
/* SPR_4748_FIX End */

/*********************************************************************************
 * Function Name  : scheduleSI8
 * Inputs         : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo,
 *                  availableCommonCCE_p - Available CCE in common search space,
 *                  sysFrameNum - System frame number,
 *                  subFrameNum - Sub frame number,
 *                  availableDLRBCount_p  - No.of available DL RBs
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo,
 *                  availableDLRBCount_p  - No.of available DL RBs,
 *                  availableCommonCCE_p - Available CCE in common search space
 * Returns        : count
 * Description    : This will schedule the SI message.
 *********************************************************************************/
UInt32 scheduleSI8(PdcchDLOutputInfo  *pdcchDLOutputInfo_p, 
                   UInt32 *availableCommonCCE_p, 
                   UInt32 sysFrameNum, 
                   UInt32 subFrameNum, 
                   SInt32 *availableDLRBCount_p,UInt8 flag,
                   InternalCellIndex internalCellIndex)
{
    UInt32 count = 0;
    UInt8 segmentIndex = 0;
    SIMsg8  *scheduleSIMsg8buff_p = PNULL;
    UInt32 agregationLvl = 0;
    UInt8  RVIndex  = 0;
    /*SPR 14155 change start*/
    UInt16 tbSize = 0;
    /*SPR 14155 change end*/
    /*SPR 21589 Fix  ++*/
    UInt8 *msgPtr = PNULL;
    /*SPR 21589 Fix  --*/

#ifdef TDD_CONFIG 
    UInt8 tddWindowVariable = 0;
#endif

    void * dataTB_p = PNULL;
    void * dataTB_temp_p = PNULL;
    QSEGMENT segment = {0};

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    scheduleSIMsg8buff_p = &(cellParams_p->siMsg8buff[siMsg8Indx_g[internalCellIndex]]);

    agregationLvl = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
    segmentIndex = scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled;


    if(0 == scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled && (scheduleSIMsg8buff_p->indexStatus == INDEX_STATUS_FILLED))
    {
       scheduleSIMsg8buff_p->indexStatus =  INDEX_STATUS_SCHEDULING; 
    }

    if(scheduleSIMsg8buff_p->indexStatus != INDEX_STATUS_SCHEDULING)
    {
        return 0;
    }
    
#ifndef LTE_EMBMS_SUPPORTED
    if ((flag == TRUE))
#else
    if ((flag == TRUE) && 
        (isEmbmsValidSf_g[internalCellIndex] == FALSE))
#endif
    {
        /* Following check is need for scheduling any SI message at current 
         * SFN/SF, here transmissionCount represents Max number of
         * SI transmission and currentTransmissionCount represents number 
         * tranmission done, currentTransmissionCount range is
         * 0 - (transmissionCount-1) */ 
        if( scheduleSIMsg8buff_p->currentTransmissionCount <  scheduleSIMsg8buff_p->transmissionCount)
        {
            if((PNULL != scheduleSIMsg8buff_p->msg_p[segmentIndex]) 
                    &&  (*availableDLRBCount_p >= scheduleSIMsg8buff_p->numRB[segmentIndex]) 

              )
            {

                dataTB_p = msgAlloc(PNULL,0,0,0);

                /* SPR 21117 Fix Start */
                /*SPR 21589 Fix  ++*/
                if (PNULL == dataTB_p)
                {
                    ltePanic("msgAlloc failed \n");
                }
                /* SPR 21117 Fix End */
                msgPtr = msgReserve(dataTB_p,0, scheduleSIMsg8buff_p->msgLen[segmentIndex] + DATA_MSG_API_HEADER_LEN);
                if(PNULL == msgPtr)
                {
                    msgFree(dataTB_p);
                    ltePanic("%s: can't allocate buffer",__FUNCTION__);
                }
                /*SPR 21589 Fix  --*/

                dataTB_temp_p = msgSegNext(dataTB_p,0,&segment);
                /*SPR 21589 Fix  ++*/
                if(PNULL == dataTB_temp_p)
                {
                    msgFree(dataTB_p);
                    ltePanic("%s: msgSegNext() returned NULL",__FUNCTION__);
                } 
                /*SPR 21589 Fix  --*/
                dataTB_temp_p = segment.base;
                if(dataTB_temp_p)
                {
                    memCpy(dataTB_temp_p, scheduleSIMsg8buff_p->msg_p[segmentIndex],
                            scheduleSIMsg8buff_p->msgLen[segmentIndex]+ DATA_MSG_API_HEADER_LEN);
                }
                DLBCCHMsgInfo *bcchMsgInfo_p = PNULL;
/* ICIC changes start */
                bcchMsgInfo_p = 
                    &pdcchDLOutputInfo_p->\
                    dlMsgTypeCommonMsgInfo[pdcchDLOutputInfo_p->countCommonChannelMsg].\
                    dlMsgInfo.dlBCCHMsgInfo;
/* ICIC changes end */
                bcchMsgInfo_p->dataLen = scheduleSIMsg8buff_p->msgLen[segmentIndex];
                bcchMsgInfo_p->data_p = dataTB_p;
                bcchMsgInfo_p->dciFormat = scheduleSIMsg8buff_p->dciFormat;
                bcchMsgInfo_p->resourceAllocInput.requiredRB = scheduleSIMsg8buff_p->numRB[segmentIndex];
                bcchMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = scheduleSIMsg8buff_p->mcsIndex[segmentIndex];
                bcchMsgInfo_p->tpc = scheduleSIMsg8buff_p->tpc[segmentIndex];

                if(DCI_FORMAT_1A == bcchMsgInfo_p->dciFormat)
                {
                    RVIndex = MODULO_FOUR(scheduleSIMsg8buff_p->currentTransmissionCount); 
                    bcchMsgInfo_p->redundancyVer = dci1ARiV[RVIndex];
                    /*SPR 14155 Change Start */
                    determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1A(
                                                                                 bcchMsgInfo_p->dataLen, &tbSize, 
                                                                                 &bcchMsgInfo_p->resourceAllocInput.requiredRB , 
                                                                                 &bcchMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel,
                                                                                 &bcchMsgInfo_p->tpc, cellParams_p);
                    /*SPR 14155 Change End*/
                }
                else if (DCI_FORMAT_1C==bcchMsgInfo_p->dciFormat)
                {
                    UInt32 inputRedundanVerCal  = 0;
                    inputRedundanVerCal = 
                        MODULO_FOUR(scheduleSIMsg8buff_p->offsetValue[scheduleSIMsg8buff_p->currentTransmissionCount]);
                    bcchMsgInfo_p->redundancyVer = redundancyVerCalc(inputRedundanVerCal);
                }

                scheduleSIMsg8buff_p->currentTransmissionCount = scheduleSIMsg8buff_p->currentTransmissionCount + 1;

                *availableDLRBCount_p -= scheduleSIMsg8buff_p->numRB[segmentIndex];
                *availableCommonCCE_p -= agregationLvl;

                count++;
            }
        }

        scheduleSIMsg8buff_p->remaingWindowSize = scheduleSIMsg8buff_p->remaingWindowSize - 1;

#ifdef FDD_CONFIG
        if( ((subFrameNum == 4) && (0 == (MODULO_TWO(sysFrameNum)))) && (scheduleSIMsg8buff_p->remaingWindowSize > 0))
        {
            scheduleSIMsg8buff_p->remaingWindowSize = scheduleSIMsg8buff_p->remaingWindowSize - 1;
        }
#elif TDD_CONFIG

	tddWindowVariable = siWindowMover(sysFrameNum,subFrameNum, internalCellIndex);

	scheduleSIMsg8buff_p->remaingWindowSize = 
     (scheduleSIMsg8buff_p->remaingWindowSize >= tddWindowVariable)?scheduleSIMsg8buff_p->remaingWindowSize - tddWindowVariable:0;

#endif

        /* Reset remaingWindowSize and currentTransmissionCount to initial range 
         * as while window size ends for that SI message. To take care other 
         * tranmission cycles of SI messages */
        if(0 == scheduleSIMsg8buff_p->remaingWindowSize )
        {
            scheduleSIMsg8buff_p->currentTransmissionCount = 0;

            scheduleSIMsg8buff_p->remaingWindowSize = scheduleSIMsg8buff_p->siWindowSize;


            scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled++;
            scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled = (scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled)%
                (scheduleSIMsg8buff_p->totalSiMsg8Sgmnts);

            if(0 == scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled)
            {
               scheduleSIMsg8buff_p->indexStatus =  INDEX_STATUS_FREE; 
               siMsg8Indx_g[internalCellIndex] =  cellParams_p->siMsg8latestIndexFilled;

            }

        }
    }
    else
    {
            scheduleSIMsg8buff_p->remaingWindowSize = scheduleSIMsg8buff_p->remaingWindowSize - 1;

            /* Decreasing remaingWindowSize by 1 when SFN % 2 == 0 at SF=4,
             * because SI schedule function is not called at SF = 5 for this
             * condition */
#ifdef FDD_CONFIG
            if( ((subFrameNum == 4) && (0 == (MODULO_TWO(sysFrameNum)))) && (scheduleSIMsg8buff_p->remaingWindowSize > 0))
            {
                scheduleSIMsg8buff_p->remaingWindowSize = scheduleSIMsg8buff_p->remaingWindowSize - 1;
            }
#elif TDD_CONFIG

            tddWindowVariable = siWindowMover(sysFrameNum,subFrameNum, internalCellIndex);

            scheduleSIMsg8buff_p->remaingWindowSize = (scheduleSIMsg8buff_p->remaingWindowSize
                    >= tddWindowVariable)?scheduleSIMsg8buff_p->remaingWindowSize - tddWindowVariable:0;
            
#endif
            /* Reset remaingWindowSize and currentTransmissionCount to initial range 
             * as while window size ends for that SI message. To take care other 
             * tranmission cycles of SI messages */
            if(0 == scheduleSIMsg8buff_p->remaingWindowSize )
            {    
                scheduleSIMsg8buff_p->currentTransmissionCount = 0;
                scheduleSIMsg8buff_p->remaingWindowSize = scheduleSIMsg8buff_p->siWindowSize;


                scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled++;
                scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled = (scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled)%
                    (scheduleSIMsg8buff_p->totalSiMsg8Sgmnts);

                if(0 == scheduleSIMsg8buff_p->siMsg8SgmntToBeScheduled)
                {
                    scheduleSIMsg8buff_p->indexStatus =  INDEX_STATUS_FREE; 
                    siMsg8Indx_g[internalCellIndex] =  cellParams_p->siMsg8latestIndexFilled;
                }
            }
    }

    return count;
}

/*********************************************************************************
 * Function Name  : scheduleSI
 * Inputs         : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo,
 *                  availableCommonCCE_p - Available CCE in common search space,
 *                  sysFrameNum - System frame number,
 *                  subFrameNum - Sub frame number,
 *                  availableDLRBCount_p  - No.of available DL RBs
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : pdcchDLOutputInfo_p - pointer to the PdcchDLUEOutputInfo,
 *                  availableDLRBCount_p  - No.of available DL RBs,
 *                  availableCommonCCE_p - Available CCE in common search space
 * Returns        : None
 * Description    : This will schedule the SI message.
 *********************************************************************************/
UInt32 scheduleSI(PdcchDLOutputInfo  *pdcchDLOutputInfo_p, 
                   UInt32 *availableCommonCCE_p, 
                   UInt32 sysFrameNum, 
                   UInt32 subFrameNum, 
                   SInt32 *availableDLRBCount_p,
                   InternalCellIndex internalCellIndex)
{
    UInt32 agregationLvl = 0;
    UInt32 msgType       = 0;
    UInt32 count         = 0;
    SIInfo *siInfo_p = PNULL;
    SIMsg  *siMsg_p  = PNULL;
    UInt8  RVIndex  = 0;
    /*SPR 21589 Fix  ++*/
    UInt8 *msgPtr = PNULL;
    /*SPR 21589 Fix  --*/
    /* FDD_Warning_Fix */
#ifdef TDD_CONFIG 
    UInt8 tddWindowVariable = 0;
#endif
    /* FDD_Warning_Fix */
    void * dataTB_p = PNULL;
    void * dataTB_temp_p = PNULL;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */
    /* ETWS changes */
    UInt8 segmentIndex = 0;
    /* ETWS changes ends */
     siInfo_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->siMsgInfo); 


    if (TRUE == siInfo_p->siInfoUpdateFlag)
    {
        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: siInfoUpdateFlag is set\n",__func__);

        /* SI messages changes only at modification boundary*/
        if (sysFrameNum == siInfo_p->sfn)
        {
            siInfo_p->siMapActiveIndex = (siInfo_p->siMapActiveIndex)?(0):(1);

            siInfo_p->siMsgActiveIndex = (siInfo_p->siMsgActiveIndex)?(0):(1);

            siInfo_p->siInfoUpdateFlag = FALSE;
        }
    }

    agregationLvl = AGGREGATION_LEVEL_FOR_COMMON_SPACE;

    sysFrameNum = sysFrameNum % MAX_SIMAP_TABLE_NODES;

    /* SPR 3067 Chg */
    msgType = siInfo_p->siMapTable[siInfo_p->siMapActiveIndex][sysFrameNum].subFrame[subFrameNum].siMsgType;

    /* ETWS changes */
    /* SI REP CHANGE */
    /* Get the Index to Redundancy version */
    /* ETWS changes ends */

    if ((msgType <= INVALID_SI_MSG) || (msgType >= MAX_SI_MSG))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Invalid message type [%d]\n",__func__,msgType);        
        return count;
    }

    siMsg_p = &siInfo_p->siMsg[siInfo_p->siMsgActiveIndex].siMsgBuff[msgType-1];

    while((siMsg_p->splSIMsgflag == TRUE) && (siMsg_p->schedulingCompleted != 0))
    {
        msgType = siMsg_p->nextCMASMessageToBeScheduled;
        if (msgType == INVALID_SI_MSG)
        {
            return count;
        }
        siMsg_p = &siInfo_p->siMsg[siInfo_p->siMsgActiveIndex].siMsgBuff[msgType-1];
    }

    /* ETWS changes */
    /* Segment identifier for SIB11 and SIB12 for single segment in 
     * single window and make sure transmission of all the segments 
     * */
    /* ETWS changes ends */

    /*Check CCE are available in Common space or not*/
    if ((siInfo_p->siMapTable[siInfo_p->siMapActiveIndex][sysFrameNum].subFrame[subFrameNum].sendSIMsgFlag) && 
            (agregationLvl <= *availableCommonCCE_p))
    {

        /* SIB8_Code Start */
        /* SIB8_Code_phase2 Start */
        if(NO_SIB8 != siMsg_p->si8Flag)
        /* SIB8_Code_phase2 End*/
        {
            count = scheduleSI8(pdcchDLOutputInfo_p,availableCommonCCE_p,sysFrameNum,subFrameNum,availableDLRBCount_p,TRUE,
                    internalCellIndex);
            return count;
        }
        /* SIB8_Code End */

        /* SPR_3898 changes start*/
        segmentIndex = siMsg_p->siMsgSgmntToBeScheduled;
        /* SPR_3444 changes end */

        /* Following check is need for scheduling any SI message at current 
         * SFN/SF, here transmissionCount represents Max number of
         * SI transmission and currentTransmissionCount represents number 
         * tranmission done, currentTransmissionCount range is
         * 0 - (transmissionCount-1) */ 
#ifndef LTE_EMBMS_SUPPORTED
        if((siMsg_p->currentTransmissionCount < siMsg_p->transmissionCount))
#else
        if((siMsg_p->currentTransmissionCount < siMsg_p->transmissionCount) &&
           (isEmbmsValidSf_g[internalCellIndex] == FALSE))
#endif
        {
            if((PNULL != siMsg_p->msg_p[segmentIndex]) 
                    /* ETWS changes */
                    &&  (*availableDLRBCount_p >= siMsg_p->numRB[segmentIndex]) 
                    /* ETWS changes ends */

              )
            {

                dataTB_p = msgAlloc(PNULL,0,0,0);
                
                /* SPR 21117 Fix Start */
               /*SPR 21589 Fix  ++*/
                if (PNULL == dataTB_p)
                {
                    ltePanic("msgAlloc failed \n");
                }
            /* SPR 21117 Fix End */
                /* ETWS changes */
                msgPtr = msgReserve(dataTB_p,0, siMsg_p->msgLen[segmentIndex] + DATA_MSG_API_HEADER_LEN);
                if(PNULL == msgPtr)
                {
                    msgFree(dataTB_p);
                    ltePanic("%s: can't allocate buffer",__FUNCTION__);
                }
                /*SPR 21589 Fix  --*/
                /* ETWS changes ends */
                dataTB_temp_p = msgSegNext(dataTB_p,0,&segment);
                /*SPR 21589 Fix  ++*/
                if(PNULL == dataTB_temp_p)
                {
                    msgFree(dataTB_p);
                    ltePanic("%s: msgSegNext() returned NULL",__FUNCTION__);
                } 
                /*SPR 21589 Fix  --*/
                dataTB_temp_p = segment.base;
                /*SPR 4017 PH2 Start*/
                if(dataTB_temp_p)
                {
                    /* ETWS changes */
                    memCpy(dataTB_temp_p, siMsg_p->msg_p[segmentIndex],
                            siMsg_p->msgLen[segmentIndex]+ DATA_MSG_API_HEADER_LEN);
                    /* ETWS changes ends */
                }
                /*SPR 4017 PH2 End*/
                DLBCCHMsgInfo *bcchMsgInfo_p = PNULL;
/* ICIC changes start */
                bcchMsgInfo_p = 
                    &pdcchDLOutputInfo_p->\
                    dlMsgTypeCommonMsgInfo[pdcchDLOutputInfo_p->countCommonChannelMsg].\
                    dlMsgInfo.dlBCCHMsgInfo;
/* ICIC changes end */
                /* ETWS changes */
                bcchMsgInfo_p->dataLen = siMsg_p->msgLen[segmentIndex];
                /* ETWS changes ends */
                bcchMsgInfo_p->data_p = dataTB_p;
                /* ETWS changes */
                bcchMsgInfo_p->dciFormat = siMsg_p->dciFormat;
                bcchMsgInfo_p->resourceAllocInput.requiredRB = siMsg_p->numRB[segmentIndex];
                bcchMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = \
                                                                             siMsg_p->mcsIndex[segmentIndex];
                /* SPR 3062 Chg */
                bcchMsgInfo_p->tpc = siMsg_p->tpc[segmentIndex];
                /* SPR 3062 Chg Ends */
                /* ETWS changes ends */

                if(DCI_FORMAT_1A == bcchMsgInfo_p->dciFormat)
                {
                    /* SPR 3067 Chg */
                    /* RV now calculated on the basis of number of Transmission 
                     * for SI as provided by L3 */
                    RVIndex = MODULO_FOUR(siMsg_p->currentTransmissionCount); 
                    bcchMsgInfo_p->redundancyVer = dci1ARiV[RVIndex];
                    /* SPR 3067 Chg Ends */
                }
                else if (DCI_FORMAT_1C==bcchMsgInfo_p->dciFormat)
                {
                    UInt32 inputRedundanVerCal  = 0;
                    /* SPR 3914 Chg */
                    inputRedundanVerCal = MODULO_FOUR(siMsg_p->offsetValue[siMsg_p->currentTransmissionCount]);
                    /*                     inputRedundanVerCal= MODULO_FOUR(subFrameNum); */
                    /* SPR 3914 Chg */
                    bcchMsgInfo_p->redundancyVer = 
                        redundancyVerCalc(inputRedundanVerCal);
                }

                /* SPR 3067 Chg */
                siMsg_p->currentTransmissionCount = siMsg_p->currentTransmissionCount + 1;
                /* SPR 3067 Chg Ends */

                /* ETWS changes */
                *availableDLRBCount_p -= siMsg_p->numRB[segmentIndex];
                /* ETWS changes ends */
                *availableCommonCCE_p -= agregationLvl;
                count++;


                /* ETWS changes */
                LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                        "[%s]: scheduled SI message Info siMapActiveIndex [%d]\n"
                        "msgType [%d] numRB [%d]",
                        __func__,siInfo_p->siMsgActiveIndex,
                        (msgType - 1),siMsg_p->numRB[segmentIndex]);
                /* ETWS changes ends */
            }
        }

        siMsg_p->remaingWindowSize = siMsg_p->remaingWindowSize - 1;

        /* Decreasing remaingWindowSize by 1 when SFN % 2 == 0 at SF=4,
         * because SI schedule function is not called at SF = 5 for this
         * condition */
/* SPR_4748_FIX Start */
#ifdef FDD_CONFIG
        if( ((subFrameNum == 4) && (0 == (MODULO_TWO(sysFrameNum)))) && (siMsg_p->remaingWindowSize > 0))
        {
            siMsg_p->remaingWindowSize = siMsg_p->remaingWindowSize - 1;
        }
#elif TDD_CONFIG

	tddWindowVariable = siWindowMover(sysFrameNum,subFrameNum, internalCellIndex);

	siMsg_p->remaingWindowSize = (siMsg_p->remaingWindowSize >= tddWindowVariable)?siMsg_p->remaingWindowSize - tddWindowVariable:0;

#endif
/* SPR_4748_FIX End */

        /* Reset remaingWindowSize and currentTransmissionCount to initial range 
         * as while window size ends for that SI message. To take care other 
         * tranmission cycles of SI messages */
        if(0 == siMsg_p->remaingWindowSize )
        {
            siMsg_p->currentTransmissionCount = 0;

            siMsg_p->remaingWindowSize = siMsg_p->siWindowSize;

            /* SPR_3898 changes start*/

            /*
             * Updating segment counter in SI message 
             * which needs to be scheduled 
             */
            siMsg_p->siMsgSgmntToBeScheduled++;
            siMsg_p->siMsgSgmntToBeScheduled = (siMsg_p->siMsgSgmntToBeScheduled)%(siMsg_p->siMsgSgmnts);
            /* SPR_3898 changes end */

            /* CMAS_CODE START */
            if((0 == siMsg_p->siMsgSgmntToBeScheduled) &&(TRUE == siMsg_p->splSIMsgflag))
            {
                siMsg_p->siMsgReptitions--;
                if(0 == siMsg_p->siMsgReptitions)
                {
                    siMsg_p->schedulingCompleted  = 1;
                }
            }
            /* CMAS_CODE END */

        }
        /* SPR 3067 Chg Ends */
    }
    else
    {
        if (siInfo_p->siMapTable[siInfo_p->\
                siMapActiveIndex][sysFrameNum].subFrame[subFrameNum].siMsgType
                != INVALID_SI_MSG)
        {    
            /* SIB8_Code Start */
        /* SIB8_Code_phase2 Start */
            if(NO_SIB8 != siMsg_p->si8Flag)
        /* SIB8_Code_phase2 End*/
            {
                count = scheduleSI8(pdcchDLOutputInfo_p,availableCommonCCE_p,sysFrameNum,subFrameNum,availableDLRBCount_p,FALSE,
                        internalCellIndex);
                return count;
            }
            /* SIB8_Code End */

            siMsg_p->remaingWindowSize = siMsg_p->remaingWindowSize - 1;

            /* Decreasing remaingWindowSize by 1 when SFN % 2 == 0 at SF=4,
             * because SI schedule function is not called at SF = 5 for this
             * condition */
/* SPR_4748_FIX Start */
#ifdef FDD_CONFIG
            if( ((subFrameNum == 4) && (0 == (MODULO_TWO(sysFrameNum)))) && (siMsg_p->remaingWindowSize > 0))
            {
                siMsg_p->remaingWindowSize = siMsg_p->remaingWindowSize - 1;
            }
#elif TDD_CONFIG

	tddWindowVariable = siWindowMover(sysFrameNum,subFrameNum, internalCellIndex);

	siMsg_p->remaingWindowSize = (siMsg_p->remaingWindowSize >= tddWindowVariable)?siMsg_p->remaingWindowSize - tddWindowVariable:0;
            
#endif
/* SPR_4748_FIX End */
            /* Reset remaingWindowSize and currentTransmissionCount to initial range 
             * as while window size ends for that SI message. To take care other 
             * tranmission cycles of SI messages */
            if(0 == siMsg_p->remaingWindowSize )
            {    
                siMsg_p->currentTransmissionCount = 0;
                siMsg_p->remaingWindowSize = siMsg_p->siWindowSize;
                /* SPR_3898 changes start*/

                /*
                 * Updating segment counter in SI message 
                 * which needs to be scheduled 
                 */
                siMsg_p->siMsgSgmntToBeScheduled++;
                siMsg_p->siMsgSgmntToBeScheduled = (siMsg_p->siMsgSgmntToBeScheduled)%(siMsg_p->siMsgSgmnts);

                /* SPR_3898 changes end*/
                /* CMAS_CODE START */
                if((0 == siMsg_p->siMsgSgmntToBeScheduled) &&(TRUE == siMsg_p->splSIMsgflag))
                {
                    siMsg_p->siMsgReptitions--;
                    if(0 == siMsg_p->siMsgReptitions)
                    {
                        siMsg_p->schedulingCompleted  = 1;
                    }
                }
                /* CMAS_CODE END */
            }
        }
    }
    return count;
}
/*  _RED_VER_ */
/*******************************************************************************
 * Function Name  : redundancyVerCalc
 * Inputs         : inputValK  - input value k to calculate redundancy version
 *                               for BCCH msg
 * Outputs        : None
 * Returns        : RedundancyVerVal for BCCH msg
 * Description    : This will calculate redundancy version Value for BCCH msg.
 *****************************************************************************/
RedundancyVerVal redundancyVerCalc(UInt32 inputValK)
{
    UInt32 ceilVal= 0;
    ceilVal=ceil_wrapper((3.0/2.0) * (float)inputValK);
    return (RedundancyVerVal) MODULO_FOUR(ceilVal);
}
/********************************************************************************
 * Function Name  : parseAndStoreBroadcastMsg
 * Inputs         : data_p - Pointer to the data,
 *                  remLen - Remaining buffer length,
 *                  cellConfigFalg - Falg is set during the cell config time,
 *                  sysFrameNum - System frame number
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : MAC_FAILURE/MAC_SUCCESS/MAC_SFN_ERROR
 * Description    : This will parse the BCCH messages received (MIB/SIB1/SI)from 
 *                  RRC and save in corresponding datastructures.
 ********************************************************************************/
UInt32 parseAndStoreBroadcastMsg(UInt8 **data_p,
        /* SPR 609 changes start */   
        UInt16 *remLen_p,
        /* SPR 609 changes end */   
        UInt32 cellConfigFlag,
        UInt32 sysFrameNum 
        ,InternalCellIndex internalCellIndex )
{
    UInt32 tagVal   = 0;
    UInt32 retVal   = 0;
    UInt32 exitFlag = FALSE;
    UInt32 mibFlag  = FALSE;
    UInt32 sibFlag  = FALSE;
    UInt32 siFlag   = FALSE;    
    UInt8  *msg_p   = PNULL;
    UInt32 remLen = *remLen_p ;


    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /*If BCCH messages received in Cell config request
     *MIB,SI and SIBType1 are mandatory. So check 
     *for these before decoding the message 
     */
    if (cellConfigFlag)
    {
        /*Check MIB, SIBType1 and SI message should sent*/
        UInt32 madatoryMsgs = 0;
        UInt32 totalLen = 0;
        UInt32 length   = 0;

        msg_p = *data_p;

        while (totalLen < remLen)
        {
            tagVal = LTE_GET_U16BIT(msg_p);
            msg_p += 2;

            if (MIB_MSG_INFO  == tagVal)
            {
                if (FALSE == mibFlag)
                {
                    madatoryMsgs++;
                    mibFlag = TRUE;
                }
            }
            else if (SIBTYPE1_MSG_INFO == tagVal)
            {
                if (FALSE == sibFlag)
                {
                    madatoryMsgs++;
                    sibFlag = TRUE;
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"Failure 2\n");
                    LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                            "[%s]: ERROR More than one SIBTYPE1_MSG_INFO Messages"
                            "            received in CELL_CONFIG_REQ\n",
                            __func__);
                    return MAC_FAILURE;
                }
            }    
            else if (SI_MSG_INFO == tagVal)
            {
                if (FALSE == siFlag)
                {
                    madatoryMsgs++;
                    siFlag = TRUE;
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"Failure 3\n");
                    LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                            "[%s]: ERROR More than one SI_MSG_INFO Messages"
                            "            received in CELL_CONFIG_REQ\n",
                            __func__);
                    return MAC_FAILURE;
                }
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"tagVal = %d, Failure 4\n", tagVal);
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s]: ERROR Mandatory TLV missing in CELL_CONFIG_REQ [%d]\n",
                        __func__,madatoryMsgs);
                return MAC_FAILURE;            
            }
            if (3 == madatoryMsgs)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s]: All Mandatory TLV are present in CELL_CONFIG_REQ [%d]\n",
                        __func__,madatoryMsgs);            
                break;
            }
            else
            {
                length = 0;
                length = LTE_GET_U16BIT(msg_p);
                length -= TAG_FIELD_LEN;
                msg_p += length;
                totalLen += length;
            }
        }
        if (3 != madatoryMsgs)
        {
            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"Failure 5\n");
            LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                    "[%s]: Error: Mandatory TLV missing CELL_CONFIG_REQ [%d]\n",
                    __func__,madatoryMsgs); 
                return MAC_FAILURE;
            }


        mibFlag  = FALSE;
        sibFlag  = FALSE;
    }

    msg_p = *data_p;


    do
    {
        tagVal = 0;
        tagVal = LTE_GET_U16BIT(msg_p);

        switch (tagVal)
        {
            case MIB_MSG_INFO:
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                            "[%s]: MIB_MSG_INFO Received\n",
                            __func__);

                    retVal = decodeMIBMsg(&msg_p, 
                            &remLen,
                            cellConfigFlag,
                            cellParams_p);
                    if (MAC_SUCCESS == retVal)
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                                "[%s]: Decoding of MIB_MSG_INFO Successfully"
                                "Completed\n",__func__);

                        mibFlag = TRUE;

                        if (remLen <= 0)
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                                    "[%s]: pointer reached at the end of the buffer"
                                    "remLen [%d]\n",
                                    __func__, remLen);                       
                            exitFlag = TRUE;
                        }                    

                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                                "[%s]: decodeMIBMsg() returned failure returnig from the function "
                                "with failure\n",__func__);

                        return retVal;
                    }
                }
                break;

            case SIBTYPE1_MSG_INFO:
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                            "[%s]: SIBTYPE1_MSG_INFO Received\n",__func__);   

                    retVal = decodeSIBType1Msg(&msg_p, 
                            &remLen,
                            cellConfigFlag,
                            sysFrameNum,
                            cellParams_p
#ifdef LTE_EMBMS_SUPPORTED
                            ,internalCellIndex
#endif
                     );

                    if (MAC_SUCCESS == retVal)
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                                "[%s]: Decoding of SIBTYPE1_MSG_INFO Successfully"
                                "Completed\n",__func__);

                        sibFlag = TRUE;

                        if (remLen <= 0)
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                                    "[%s]: pointer reached at the end of the buffer"
                                    "remLen [%d]\n",
                                    __func__, remLen);                       
                            exitFlag = TRUE;
                        }
                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                                "[%s]: decodeSIBType1Msg() returned failure returnig from the function "
                                "with failure\n",__func__);

                        /*Do not use the latest MIB received*/
                        if (TRUE == mibFlag)
                        {
                           cellParams_p->mibInfo.updateFlag = FALSE;
                        }

                        return retVal;
                    }       
                }
                break;

            case SI_MSG_INFO:
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                            "[%s]: SI_MSG_INFO Received\n",__func__);

                    retVal = decodeSIMsg(&msg_p, 
                            &remLen,
                            cellConfigFlag,
                            sysFrameNum,
                            internalCellIndex);
                     /* CA Changes end */
                    if (MAC_SUCCESS == retVal)
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                                "[%s]: Decoding of SI_MSG_INFO Successfully"
                                "Completed\n",__func__);
                        if (remLen <= 0)
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                                    "[%s]: pointer reached at the end of the buffer"
                                    "remLen [%d]\n",
                                    __func__, remLen);                       
                            exitFlag = TRUE;
                        }
                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                                "[%s]: decodeSIMsg() returned failure returnig from the function "
                                "with failure\n",__func__); 

                        /*Do not use the latest MIB received*/
                        if (TRUE == mibFlag)
                        {
                           cellParams_p->mibInfo.updateFlag = FALSE;
                        }

                        /*Do not use the latest SIB Type1 received*/
                        if (TRUE == sibFlag)
                        {   
                           cellParams_p->sib1MsgInfo.updateFlag = FALSE;
                        }

                        return retVal;
                    }                 
                }
                break;

            default:
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                            "[%s] Invalid tag value [%d]\n",__func__, tagVal);
                    exitFlag = TRUE;
                }
        }
    }while(FALSE == exitFlag);

    *remLen_p = remLen;
    *data_p = msg_p;

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : decodeMIBMsg
 * Inputs         : msg_p   - double pointer to the message buffer,
 *                  remLen_p - Remaining buffer length ,
 *                  cellCfgFlag - Cell config flag
 *                  cellParams_p - Cell global param pointer
 * Outputs        : msg_p   - double pointer to the message buffer,
 *                  dataPtrPos_p - Position of the data pointer,
 *                  remLen_p - Remaining buffer length 
 * Returns        : MAC_SUCCESS/MAC_FAILURE/MAC_SFN_ERROR
 * Description    : This will decode the MIBs from the received buffer.
 *****************************************************************************/
UInt32 decodeMIBMsg(UInt8  **msg_p, 
        UInt32 *remLen_p,
        UInt32 cellCfgFlag,
        CellConfigParams *cellParams_p )
{
    UInt32 i = 0;
    UInt32 tagVal  = 0;
    UInt32 numMIBs = 0;
    UInt32 length  = 0;
    UInt32 activeIndex = 0;
    UInt32 sysFrameNum = 0;
    /* Review comment fix start GG5 */
    /*+COVERITY 5.3.0 - 52878*/
    UInt8  updateFlag = FALSE;
    /*-COVERITY 5.3.0 - 52878*/
    /* Review comment fix end GG5 */

    /* + Coverity_Stack Fix */
    
    MIBMsgFromRRC *mibInfoReceived = PNULL;
    GET_MEM_FROM_POOL(MIBMsgFromRRC , mibInfoReceived ,
                MAX_MIB_FROM_RRC*sizeof(MIBMsgFromRRC),PNULL);
    
    /* - Coverity_Stack Fix */
    /*+COVERITY 5.3.0 - 32119*/
    if ( PNULL == mibInfoReceived  )
    {
        return MAC_SFN_ERROR;
    }
    /*-COVERITY 5.3.0 - 32119*/
    MIBMsgInfo *mibMsgInfo_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "Entry [%s]()\n",__func__);

    /*Loop and decode the MIB message till the end of buffer
     *or till the tag value is not MAC_MIB_MSG_INFO
     */
    do
    {
        tagVal = LTE_GET_U16BIT(*msg_p);
        if (MIB_MSG_INFO != tagVal) /*Tag value*/
        {
            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: Tag value [%d] is not equal to MIB_MSG_INFO"
                    "break from the loop\n",__func__,**msg_p);
            break;
        }

        /*Increment the pointer to point to Length field*/
        *msg_p += TAG_FIELD_LEN;
        *remLen_p -= TAG_FIELD_LEN;

        length = LTE_GET_U16BIT(*msg_p);

        /*Increment the pointer to point to SFN field*/
        *msg_p += LENGTH_FIELD_LEN;
        *remLen_p -= LENGTH_FIELD_LEN;

        sysFrameNum = LTE_GET_U16BIT(*msg_p);
        if ((sysFrameNum >= MAX_SFN_VALUE)||
                (0 != MODULO_FOUR(sysFrameNum)))
        {
            LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                    "[%s]: Invalid System Frame Number received [%d]\n"
                    "returning with failure",__func__,sysFrameNum);
            /* + Coverity_Stack Fix */
            freeMemPool(mibInfoReceived);
            /* - Coverity_Stack Fix */
            return MAC_SFN_ERROR;
        }

        mibInfoReceived[numMIBs].sysFrameNum = sysFrameNum;

        /*Increment the pointer to point to MIB message*/
        *msg_p += SFN_FIELD_LEN;
        *remLen_p -= SFN_FIELD_LEN;

        /*Minus the Tag, length and SFN field length from the length*/
        length -= (TAG_FIELD_LEN + LENGTH_FIELD_LEN + SFN_FIELD_LEN);

        if (length != MAX_MIB_MSG_LEN)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                    "[%s]: Invalid MIB length received [%d]\n"
                    "      MIB message length should be = %d\n",
                    __func__,length,MAX_MIB_MSG_LEN);
            
            /* + Coverity_Stack Fix */
            freeMemPool(mibInfoReceived);
            /* - Coverity_Stack Fix */

            return MAC_SFN_ERROR;
        }

        mibInfoReceived[numMIBs].dataLen = length;
        mibInfoReceived[numMIBs].msg_p = *msg_p;

        /*Removed logging of each MIB message as its not of any help*/

        /*Increment the Pointer to point to next MIB*/
        *msg_p = *msg_p + length;
        *remLen_p -= length;

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: sysFrameNum [%d] \n",
                __func__,mibInfoReceived[numMIBs].sysFrameNum);

        numMIBs++;
/* SPR 18470 Start */ 
/* SPR 17729 Start */
}while((*remLen_p)&&(MAX_MIB_FROM_RRC>numMIBs));
/* SPR 17729 End */
/* SPR 18470 End */
     mibMsgInfo_p = &(cellParams_p->mibInfo);
    /*If 256 MIBs received save the messages at new index
     *Otherwise at the current active index
     */
    /* Review comment fix start GG8 */
    if (numMIBs > MAX_MIB_FROM_RRC)
    {
            LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                    "[%s]: Invalid Number of times MIB received [%d] in sysFrameNum [%d]\n"
                    "returning with failure",__func__,numMIBs,sysFrameNum);
            freeMemPool(mibInfoReceived);
            return MAC_SFN_ERROR;
    }
    /* Review comment fix end */
    if (MAX_MIB_FROM_RRC == numMIBs)
    {
        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: %d MIBs received, setting the MIB array\n",
                __func__,numMIBs);

        /*If the mibMsgInfo_p->activeIndex is 0, copy the new 
         *MIBs at index 1 and viseversa
         */
        if (FALSE == cellCfgFlag)
        {
            activeIndex = (0 == mibMsgInfo_p->activeIndex)?(1):(0);
            /* Review comment fix start GG7 */
            /* SPR 16748 Fix Start */
            /*memSet(&mibMsgInfo_p->mibMsg[activeIndex], 0, sizeof(mibMsgInfo_p->mibMsg[activeIndex]));*/
            /* SPR 16748 Fix Start */
            /* Review comment fix end */

            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: Mem setting the MIB array at active index %d\n",
                    __func__,activeIndex);

            /* Review comment fix start GG5 */
            updateFlag = TRUE;
            /* Review comment fix end GG5 */

            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: Update flag in MIB info set to TRUE",__func__); 
        }
    }
    else
    {
        activeIndex = mibMsgInfo_p->activeIndex;
    }

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "[%s]: Number of MIBs received [%d] activeIndex [%d]\n",
            __func__,numMIBs,activeIndex); 

    /*Save the received MIB in static global array*/
    for (i = 0; i < numMIBs; i++)
    {
        sysFrameNum = mibInfoReceived[i].sysFrameNum;
        if (sysFrameNum)
        {
            sysFrameNum = DIVIDE_BY_FOUR(sysFrameNum);
        }
         /*SPR 7885 Change Start*/
        /* Review comment fix start MAC_AT_3 */
        if (PNULL != mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].msg_p)
        {
            freeMemPool(mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].msg_p);
            mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].msg_p = PNULL;
            mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].dataLen = 0;   
            mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].isMIBPresent = 0;   
        }
        /* Review comment fix end MAC_AT_3 */
         /*SPR 7885 Change End*/
        GET_MEM_FROM_POOL(UInt8 , mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].msg_p,
                (mibInfoReceived[i].dataLen + DATA_MSG_API_HEADER_LEN +
                 MIB_LENGTH_FIELD_SIZE), PNULL);
        memSet(mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].msg_p, 0, 
                (mibInfoReceived[i].dataLen + DATA_MSG_API_HEADER_LEN));
        memCpy(mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].msg_p + DATA_MSG_API_HEADER_LEN +
                MIB_LENGTH_FIELD_SIZE,
                mibInfoReceived[i].msg_p,
                mibInfoReceived[i].dataLen);

        mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].dataLen = 
            mibInfoReceived[i].dataLen;

        mibMsgInfo_p->mibMsg[activeIndex][sysFrameNum].isMIBPresent = TRUE;
    }

    /* Review comment fix start GG5 */
    if (TRUE == updateFlag)
    {
        mibMsgInfo_p->updateFlag = TRUE;
    }
    /* Review comment fix end GG5 */

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "Exit [%s]\n",__func__);
    
    /* + Coverity_Stack Fix */
    freeMemPool(mibInfoReceived);
    /* - Coverity_Stack Fix */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : decodeSIBType1Msg
 * Inputs         : msg_p   - Double Pointer to the message buffer,
 *                  remLen_p - Remainig buffer length,
 *                  cellCfgFlag - Cell config flag,
 *                  tgtSysFrameNum - The Target SFN        
 *                  cellParams_p - Cell global param pointer
 * Outputs        : zeroBuff_p/msg_p/dataPtrPos_p/remLen_p
 * Returns        : MAC_SUCCESS/MAC_FAILURE/MAC_SFN_ERROR
 * Description    : This will decode the SIBType1 message from received buffer.
 *****************************************************************************/
UInt32 decodeSIBType1Msg(UInt8  **msg_p,
        UInt32 *remLen_p,
        UInt32 cellCfgFlag, 
        UInt32 tgtSysFrameNum,
       CellConfigParams *cellParams_p
#ifdef LTE_EMBMS_SUPPORTED
        ,InternalCellIndex internalCellIndex
#endif
)
{
    UInt32 length      = 0;
    UInt32 sysFrameNum = 0;
    /* SPR 13250 fix start */
    UInt16  tbSize      = 0;
    /* SPR 13250 fix end */
    /* SPR 8879 Fix Start */
    UInt8 dciFormat = cellParams_p->dciFormatForCommonChnlMsg;
    /* SPR 8879 Fix End*/
    SIBType1Info *sibType1Info_p = PNULL;
    SIBType1Msg  *siType1Msg_p   = PNULL;
    /* SPR 1542 changes start */
    UInt8 siMappingChanged = 0;
    /* SPR 1542 changes end */
#ifdef LTE_EMBMS_SUPPORTED
#ifdef TDD_CONFIG
    UInt8 sib1Sf=0;
    UInt8 dlSubframe=0;
#endif
#endif

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "Entry [%s]()\n",__func__);

    /*increment by 1 to point to length field*/
    *msg_p += TAG_FIELD_LEN;
    *remLen_p -= TAG_FIELD_LEN;

    /*2 bytes of length field*/
    length = LTE_GET_U16BIT(*msg_p);
    *msg_p += LENGTH_FIELD_LEN;
    *remLen_p -= LENGTH_FIELD_LEN;

    if (length < MIN_SIBTYPE1_MSG_INFO_LEN)
    {
        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: MIN_SIBTYPE1_MSG_INFO_LEN Invalid length received [%d]\n",
                __func__,length);
        return MAC_FAILURE;
    }

    /* SPR 1542 changes start */
    siMappingChanged = **msg_p;
    *remLen_p -= SI_MAPPING_FIELD_LEN;
    *msg_p += SI_MAPPING_FIELD_LEN;
    /* SPR 1542 changes end */


    /*2 bytes of SFN value*/
    sysFrameNum = LTE_GET_U16BIT(*msg_p);
    /* In case of reconfig calculate the actual SFN */
    if (cellCfgFlag == FALSE)
    {
        sysFrameNum += tgtSysFrameNum; 
        /* + SPR_17858_322_355_335_CHANGES */
        sysFrameNum = ((sysFrameNum + (SI_PERIODICITY_8 - (sysFrameNum & 7))))& 1023;
        /* - SPR_17858_322_355_335_CHANGES */
    }            
    *msg_p += SFN_FIELD_LEN;
    *remLen_p -= SFN_FIELD_LEN;

    if (sysFrameNum >= MAX_SFN_VALUE)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Invalid sysFrameNum [%d]\n",
                __func__, sysFrameNum);
        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "Exit[%s]()\n",__func__);
        return MAC_SFN_ERROR;
    }

/* SPR_4993_FIX Start */
    if (sysFrameNum % 2)
    {
      sysFrameNum = ((sysFrameNum + 1)%MAX_SFN_VALUE);
    }
/* SPR_4993_FIX End */


    /*length of the SIBType1 message*/
    /* SPR 1542 changes start */
    length -= (TAG_FIELD_LEN + LENGTH_FIELD_LEN + SFN_FIELD_LEN + SI_MAPPING_FIELD_LEN);
    /* SPR 1542 changes end */

    /* SPR 8879 Fix Start */
    if ((!length) || 
            (DCI_FORMAT_1A ==  dciFormat && length > MAX_SIB1_MSG_SIZE_FOR_DCI_1A) ||
            (DCI_FORMAT_1C ==  dciFormat && length > MAX_SIB1_MSG_SIZE_FOR_DCI_1C) ||
            (length > *remLen_p))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Invalid SIB Type1 message size [%d]\n",
                __func__,length);

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "Exit[%s]()\n",__func__);

        return MAC_FAILURE;
    }
    /* SPR 8879 Fix End*/
    sibType1Info_p = &cellParams_p->sib1MsgInfo;
    if (FALSE == cellCfgFlag)
    {
        siType1Msg_p = (sibType1Info_p->activeIndex)?\
                       (&sibType1Info_p->sibType1Msg[0]):\
                       (&sibType1Info_p->sibType1Msg[1]);
    }
    else
    {
        siType1Msg_p = &sibType1Info_p->sibType1Msg[sibType1Info_p->activeIndex];
    }

    siType1Msg_p->msgLen = length;
    siType1Msg_p->sfn    = sysFrameNum;
    /* SPR 1542 changes start */
    siType1Msg_p->si_mapping_changed  = siMappingChanged;
    /* SPR 1542 changes end */
    #ifdef LTE_EMBMS_SUPPORTED
#ifdef TDD_CONFIG
        sib1Sf=5;
        /* Toget D subframe */
        dlSubframe =getNextDLOccasion(sib1Sf, internalCellIndex);
        if (dlSubframe < sib1Sf)
        {
           sysFrameNum++;
           if (MAX_SYS_FRAME_NUM < sysFrameNum)
           {
              sysFrameNum = 0;
           }
        }

       embmsConfigParams_g[internalCellIndex].sib1TargetSfnSf=sysFrameNum*10+dlSubframe;
#else
       embmsConfigParams_g[internalCellIndex].sib1TargetSfnSf=sysFrameNum*10+5;
#endif

#endif



    /* SPR 3062 Chg */
    /* SPR 1614 changes start */

    /* SPR 8879 Fix Start */
    /*KLOCKWORK_CHANGES_START_2*/ 
    if (DCI_FORMAT_1A ==  dciFormat)
    {
            determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1A
            ( siType1Msg_p->msgLen,
              &tbSize,
              &siType1Msg_p->numRB,
              &siType1Msg_p->mcsIndex,
              &siType1Msg_p->tpc,
              cellParams_p);
        /* SPR 3062 Chg Ends */
    }
    else if(DCI_FORMAT_1C ==  dciFormat)
    {
            determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1C
            ( siType1Msg_p->msgLen,
              &tbSize,
              &siType1Msg_p->numRB,
              &siType1Msg_p->mcsIndex,
              &siType1Msg_p->tpc,
              cellParams_p);
    }	
    /*KLOCKWORK_CHANGES_STOP_2*/

        /*Free the memory if message present stored last time*/
        /*SPR 8094 Fix Start*/
         if(PNULL != siType1Msg_p->msg_p)
         {
             freeMemPool(siType1Msg_p->msg_p);
             siType1Msg_p->msg_p = PNULL;
         }
         /*SPR 8094 Fix End*/ 

        /*Allocate memory*/
        GET_MEM_FROM_POOL(UInt8 , siType1Msg_p->msg_p ,
                (tbSize + DATA_MSG_API_HEADER_LEN),PNULL);
        /* + coverity 24489 */
        /*coevrity-530 CID 24489 */
        if ( PNULL == siType1Msg_p->msg_p )
        {
            /*CRITICAL! Memory error*/
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, 
                    tbSize + DATA_MSG_API_HEADER_LEN, 0,0, 0, 0,0, 
                    __FILE__, __func__);
            ALARM_MSG(MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            return MAC_FAILURE;
        }
        /*coevrity-530 CID 24489 */
        /* - coverity 24489 */

        memSet(siType1Msg_p->msg_p, 0, 
                (tbSize + DATA_MSG_API_HEADER_LEN));
        memCpy(siType1Msg_p->msg_p + DATA_MSG_API_HEADER_LEN,
                *msg_p, siType1Msg_p->msgLen);
    /* Review comment fix start SYNC_LOSS_2*/
    LOG_MAC_MSG(MAC_SIBTYPE1_MSG_INFO_PARAM,LOGINFO,MAC_RRC_INF,
            getCurrentTick(),
            siMappingChanged,
            sysFrameNum,
            siType1Msg_p->msgLen,
            DEFAULT_INT_VALUE, 
            DEFAULT_INT_VALUE, 
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            __func__,"MAC_SIBTYPE1_MSG_INFO_PARAM");
    /* Review comment fix end SYNC_LOSS_2*/

        *msg_p += length;
        *remLen_p -= length;
        siType1Msg_p->msgLen = tbSize;

        if (FALSE == cellCfgFlag)
        {
            sibType1Info_p->updateFlag = TRUE;

            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s] sibType1Info_p->updateFlag [%d] is set to TRUE\n",
                    __func__,sibType1Info_p->updateFlag);
        }

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "Exit[%s]()\n",__func__);

        return MAC_SUCCESS;
    /* SPR 1614 changes end */
}

/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : getSIPeriodicity
 * Inputs         : val,
 *                : siPeriodicity_p 
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will update the siPeriodicity.
 *****************************************************************************/
STATIC  MacRetType getSIPeriodicity(UInt8 val, UInt16 *siPeriodicity_p)
{
   switch (val) /*SI-Periodicity*/
   {
       case 1:
           *siPeriodicity_p = SI_PERIODICITY_8;
           break;

       case 2:
           *siPeriodicity_p = SI_PERIODICITY_16;                    
           break;

       case 3:
           *siPeriodicity_p = SI_PERIODICITY_32;                    
           break;

       case 4:
           *siPeriodicity_p = SI_PERIODICITY_64;                    
           break;

       case 5:
           *siPeriodicity_p = SI_PERIODICITY_128;                    
           break;

       case 6:
           *siPeriodicity_p = SI_PERIODICITY_256;
           break;

       case 7:
           *siPeriodicity_p = SI_PERIODICITY_512;
           break;

       default:
           {
               LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                       "[%s]: ERROR Invalid SI-Periodicity [%d]\n",
                       __func__, *siPeriodicity_p);
               LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                       "Exit[%s]()\n",__func__);
               return MAC_FAILURE;
           }
   }
   return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : getSIWindowSize
 * Inputs         : val,
 *                : siWindowSize_p
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will update the siWindowSize.
 *****************************************************************************/
STATIC  MacRetType getSIWindowSize(UInt8 val, UInt32 *siWindowSize_p)
{
    /*1 byte of SI window length*/
    switch(val)/*SI-Window length*/
    {
        case 1:
            *siWindowSize_p = SI_WINDOW_LEN_1;
            break;
        case 2:
            *siWindowSize_p = SI_WINDOW_LEN_2;
            break; 
        case 5:
            *siWindowSize_p = SI_WINDOW_LEN_5;
            break; 
        case 10:
            *siWindowSize_p = SI_WINDOW_LEN_10;
            break; 
        case 15:
            *siWindowSize_p = SI_WINDOW_LEN_15;
            break; 
        case 20:
            *siWindowSize_p = SI_WINDOW_LEN_20;
            break; 
        case 40:
            *siWindowSize_p = SI_WINDOW_LEN_40;
            break; 

        default:
            {
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s]: ERROR Invalid SI-Window length [%d]\n",
                        __func__, *siWindowSize_p);
                LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                        "Exit[%s]()\n",__func__);

                return MAC_FAILURE;                    
            }
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : cleanupForSIDecodeFail
 * Inputs         : i - isFailureFlag
 *                  siMsgReceived_p 
 * Outputs        : None
 * Returns        : None
 * Description    : This will free the SI message on failure.
 *****************************************************************************/
static  void cleanupForSIDecodeFail(UInt32 i,SIMsgInfoFromRRC* siMsgReceived_p)
{
        /*Free the memory*/
        UInt32 idx = 0;
        UInt8  siMsgSegCntr= 0; 

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: ERROR: Decode failed no.of SI msgs received [%d]\n"
                "      no.of SI msgs decoded [%d] isFailureFlag [%d]\n",
                __func__,numSIMsg,i,isFailureFlag);
        /* SPR 1768 changes start*/
        while (idx < i)
        {
            /* SPR  3729 changes start */
            for(siMsgSegCntr = 0;siMsgSegCntr < siMsgReceived_p[idx].siMsgSgmnts;siMsgSegCntr++)
            {
                if (NULL != siMsgReceived_p[idx].msg_p[siMsgSegCntr])
                {
                    freeMemPool(siMsgReceived_p[idx].msg_p[siMsgSegCntr]);
                    siMsgReceived_p[idx].msg_p[siMsgSegCntr] = NULL;
                }
            }
            /* SPR  3729 changes End */

            /* SPR 1768 changes end*/
            idx++;
        }

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "Exit[%s]()\n",__func__);

        return;
}
/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : decodeSIMsg
 * Inputs         : msg_p   - Double Pointer to the message buffer,
 *                  remLen_p - Remainig buffer length,
 *                  cellCfgFlag - Cell config flag,
 *                  sysFrameNum - System frame number
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : zeroBuff_p/msg_p/dataPtrPos_p/remLen_p
 * Returns        : MAC_SUCCESS/MAC_FAILURE/MAC_SFN_ERROR
 * Description    : This will decode the SI messages from received buffer.
 *****************************************************************************/
UInt32 decodeSIMsg(UInt8  **msg_p,
        UInt32 *remLen_p,
        UInt32 cellCfgFlag, 
        UInt32 sysFrameNum,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt32 length   = 0;
    UInt32 tagVal = 0;
    /* SPR 13250 fix start */
    UInt16  tbSize = 0;
    /* SPR 13250 fix end */

    /* CMAS_CODE START */
    UInt8  siIndex = 0;
    /* CMAS_CODE END */

    /* SIB8_Code Start */
    UInt8  si8Flag = 0;
    /* SIB8_Code End */

    UInt32 siWindowSize = 0;


    UInt32 numSIMsg     = 0;
    UInt32 i = 0;
    UInt32 isFailureFlag = FALSE;
    MacRetType ret_val = MAC_FAILURE;

    /*Coverity Fix 10267 Start*/

    SIMsgInfoFromRRC *siMsgReceived_p = cellParams_p->siMsgReceived;
    SIInfo       *siMsgInfo_p = &cellParams_p->siMsgInfo;
    /*Coverity Fix 10267 End*/

    /* REL_3_0_CHG */
    /* ETWS changes */
    UInt8   siMsgSegCntr= 0; 
    /* ETWS changes ends */
    UInt16  siTlvLen = 0;
    /* REL_3_0_CHG */
    /* SPR 3062 Chg */

    /* CMAS_CODE START */
    UInt8 numOfTransmissions = 0;
    /* CMAS_CODE END */

    UInt8 siStartOffset = 0;
    /* SPR 3062 Chg Ends */

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"Entry [%s]()\n",__func__);

    /*Coverity Fix 10267 Start*/

    memSet((Char8*) cellParams_p->siMsgReceived,0,MAX_SI_MSG*sizeof(SIMsgInfoFromRRC));

    /*Coverity Fix 10267 End*/


    if (sysFrameNum >= MAX_SFN_VALUE)
    {
        return MAC_SFN_ERROR;
    }

    *msg_p += TAG_FIELD_LEN;
    *remLen_p -= TAG_FIELD_LEN;

    /*2 bytes of length field*/
    length = LTE_GET_U16BIT(*msg_p);

    if ((length < MIN_SI_MSG_INFO_LEN) || 
            (length > (*remLen_p + TAG_FIELD_LEN)))
    {
        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "[%s]: MIN_SI_MSG_INFO_LEN Invalid length received [%d]\n",
                __func__,length);
        return MAC_FAILURE;
    }
    *msg_p += LENGTH_FIELD_LEN;
    *remLen_p -= LENGTH_FIELD_LEN;

    /* cyclomatic changes */
    ret_val = getSIWindowSize(**msg_p, &siWindowSize);
    if(MAC_FAILURE == ret_val)
    {
        lteWarning("[%s]: ERROR Invalid SI-Window length [%d]\n",
                __func__,siWindowSize);
        return MAC_FAILURE;
    }
    /* cyclomatic changes */
    *msg_p += 1;
    *remLen_p -= 1;

    /*Number of SI messages*/
    numSIMsg = **msg_p;
    *msg_p += 1;
    *remLen_p -= 1;
    /* Review comment fix start SYNC_LOSS_2*/
    LOG_MAC_MSG(MAC_SI_MSG_INFO_PARAM,LOGINFO,MAC_RRC_INF,
            getCurrentTick(),
            siWindowSize,
            numSIMsg,
            DEFAULT_INT_VALUE, 
            DEFAULT_INT_VALUE, 
            DEFAULT_INT_VALUE, 
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            __func__,"MAC_SI_MSG_INFO_PARAM");
    /* Review comment fix end SYNC_LOSS_2*/

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "\n[%s]: TargetSFN [%d]  siWindowSize [%d] numSIMsg [%d]\n",
            __func__,sysFrameNum,siWindowSize,numSIMsg);

    if ((!numSIMsg) || (numSIMsg > MAX_SI_MSG))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Invalid numSIMsg [%d]\n",
                __func__, numSIMsg);

        LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                "Exit[%s]()\n",__func__);

        return MAC_FAILURE;
    }


    for (i=0; i < numSIMsg; i++)
    {
        tagVal = LTE_GET_U16BIT(*msg_p);

        if (SI_MSG != tagVal)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                    "[%s]: ERROR Invalid tag [%d]\n",
                    __func__, tagVal);

            break;
        }
        else
        {
            *msg_p += TAG_FIELD_LEN;
            *remLen_p -= TAG_FIELD_LEN;

            /*2 bytes of length field*/
            length = LTE_GET_U16BIT(*msg_p);

            if ((length < MIN_SI_MSG_LEN) || 
                    (length > (*remLen_p + TAG_FIELD_LEN)))
            {
                LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                        "[%s]: MIN_SI_MSG_LEN Invalid length received [%d]\n",
                        __func__,length);

                LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                        "Exit[%s]()\n",__func__);
                /*SPR 3821 Start*/
                isFailureFlag = TRUE;
                break;
                /* return MAC_FAILURE;*/
                /*SPR 3821 Ends*/

            }
            *remLen_p -= LENGTH_FIELD_LEN;
            *msg_p += LENGTH_FIELD_LEN;

            length -= (TAG_FIELD_LEN + LENGTH_FIELD_LEN);

            /* CMAS_CODE START */

            siIndex = **msg_p;
            *msg_p += 1;
            *remLen_p -= 1;
            length -= 1;
            if(!siIndex || siIndex > MAX_SI_MSG)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s] The SI Index Parameter is Wrong(%d)",__func__,siIndex);;
                /*SPR 3821 Start*/
                isFailureFlag = TRUE;
                break;
                /* return MAC_FAILURE;*/
                /*SPR 3821 Ends*/ 
            }

            /* CMAS_CODE END */

            /* SPR 3067 Chg */
            /* siWindowSize introduced to take care of multiple tranmission of 
             * SI in the same window, siWindowSize is as given by L3 while 
             * remaingWindowSize range is (siWindowSize - 0) */
            siMsgReceived_p[i].siWindowSize = siWindowSize;
            siMsgReceived_p[i].remaingWindowSize = siWindowSize;
            /* SPR 3067 Chg Ends */
            /* cyclomatic changes */
            ret_val = getSIPeriodicity(**msg_p, &(siMsgReceived_p[i].siPeriodicity));
            if(MAC_FAILURE == ret_val)
            {
                /*SPR 3821 Start*/
                isFailureFlag = TRUE;
                lteWarning("[%s]: ERROR Invalid SI-Periodicity [%d]\n",
                        __func__,siMsgReceived_p[i].siPeriodicity);
                break;
                /*return MAC_FAILURE;*/
                /*SPR 3821 Ends*/
            }
            /* cyclomatic changes */
            *msg_p += 1;
            *remLen_p -= 1;
            length -= 1;

            siMsgReceived_p[i].startSFN = LTE_GET_U16BIT(*msg_p);
            *msg_p += SFN_FIELD_LEN;
            *remLen_p -= SFN_FIELD_LEN;
            length -= SFN_FIELD_LEN;

            if (siMsgReceived_p[i].startSFN >= MAX_SFN_VALUE)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s]: ERROR Invalid startSFN [%d]\n",
                        __func__,siMsgReceived_p[i].startSFN);
                /*SPR 3821 Start*/
                isFailureFlag = TRUE;
                break;
                /* return MAC_SFN_ERROR; */
                /*SPR 3821 Ends*/ 
            }

            siMsgReceived_p[i].startSubFrame = **msg_p;
            *msg_p += 1;
            *remLen_p -= 1;
            length -= 1;

            if (siMsgReceived_p[i].startSubFrame >= MAX_SUB_FRAME)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s]: ERROR Invalid startSubFrame[%d]\n",
                        __func__, siMsgReceived_p[i].startSubFrame);
                /*SPR 3821 Start*/
                isFailureFlag = TRUE;
                break;
                /* return MAC_FAILURE;*/
                /*SPR 3821 Ends*/ 
            }



            if (MAC_FAILURE == validateStartSFNAndSF(&siMsgReceived_p[i], siWindowSize, siIndex))
            {
                isFailureFlag = TRUE;
                break;
            }

            LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                    "[%s]: SI-Periodicity [%d] startSFN [%d] startSubFrame [%d]\n",
                    __func__, siMsgReceived_p[i].siPeriodicity, 
                    siMsgReceived_p[i].startSFN, siMsgReceived_p[i].startSubFrame);

            /* Changed to make it compliant with MAC 2.6 API Doc, only parsing is changed,
              internally no changes done for this. */
            /* REL_3_0_CHG */
            /* ETWS changes */

            /* SPR_4832_FIX Start */
            if( (siMsgReceived_p[i].startSubFrame == 5) && ((0 == (MODULO_TWO( siMsgReceived_p[i].startSFN ))) )) 
            {
                siMsgReceived_p[i].siWindowSize = siMsgReceived_p[i].siWindowSize-1;
                siMsgReceived_p[i].remaingWindowSize =  siMsgReceived_p[i].siWindowSize;
            }
            /* SPR_4832_FIX End */

            siMsgReceived_p[i].siMsgSgmnts = **msg_p;

            if ((!siMsgReceived_p[i].siMsgSgmnts)||(MAX_NO_SI_MSG_SGMNTS < siMsgReceived_p[i].siMsgSgmnts))
            {
                LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                        "[%s]: ERROR out of max message segments[%d]\n",
                        __func__, siMsgReceived_p[i].siMsgSgmnts);
                isFailureFlag = TRUE;
                break;
            }
            /* ETWS changes ends */

            *msg_p += 1;
            *remLen_p -= 1;
            length -= 1;

            /* Review comment fix start SYNC_LOSS_2*/
            LOG_MAC_MSG(MAC_SI_MSG_INFO_PARAM_SET1,LOGINFO,MAC_RRC_INF,
                    getCurrentTick(),
                    siIndex,
                    siMsgReceived_p[i].siPeriodicity,
                    siMsgReceived_p[i].startSFN,
                    siMsgReceived_p[i].startSubFrame, 
                    siMsgReceived_p[i].siMsgSgmnts, 
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__,"MAC_SI_MSG_INFO_PARAM_SET1");
            /* Review comment fix end SYNC_LOSS_2*/
            /* CMAS_CODE START */

            tagVal = LTE_GET_U16BIT(*msg_p);
            if(NUM_SI_REPETITIONS == tagVal)
            {
                /*Increment the pointer to point to Length field*/
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;
                /* ETWS changes */
                length -= TAG_FIELD_LEN;

                siTlvLen = LTE_GET_U16BIT(*msg_p);
                /* SIB8_Code Start */

                if(NUM_SI_REPETITIONS_TAG_LEN  != siTlvLen)
                {
                    ltePanic("Invalid length for tag NUM_SI_REPETITIONS %d\n",siTlvLen);
                    break;
                }
                /* SIB8_Code End */


                /*Increment the pointer to point to num of repitions field*/
                *msg_p += LENGTH_FIELD_LEN;
                *remLen_p -= LENGTH_FIELD_LEN;
                length -= LENGTH_FIELD_LEN;

                siMsgReceived_p[i].siMsgReptitions = LTE_GET_U16BIT(*msg_p);
                /* Review comment fix start SYNC_LOSS_2*/
                LOG_MAC_MSG(MAC_NUM_SI_REPETITIONS_PARAM,LOGINFO,MAC_RRC_INF,
                        getCurrentTick(),
                        i,
                        siMsgReceived_p[i].siMsgReptitions,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__,"MAC_NUM_SI_REPETITIONS_PARAM");
                /* Review comment fix end SYNC_LOSS_2*/

                *msg_p += 2;
                *remLen_p -= 2;
                length -= 2;

            }
            /* ETWS changes */
            else
            {
                siMsgReceived_p[i].siMsgReptitions = MIN_SI_REPETITIONS;
            }


            /* SPR 3067 Chg */
            /* SI number of repetition represent in how many SF 
             * in the SI window SI tranmission can be done, 
             * value provided by L3 default value is 1 represent by 
             * MIN_SI_REPETITIONS */

            tagVal = LTE_GET_U16BIT(*msg_p);
            if(NUM_SI_TRANSMISSIONS == tagVal)
            {
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;

                length = LTE_GET_U16BIT(*msg_p);
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;


                numOfTransmissions = **msg_p;
                *msg_p += 1;
                *remLen_p -= 1;
                /* Review comment fix start SYNC_LOSS_2*/
                LOG_MAC_MSG(MAC_NUM_SI_TRANSMISSIONS_PARAM,LOGINFO,MAC_RRC_INF,
                        getCurrentTick(),
                        numOfTransmissions,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__,"MAC_NUM_SI_TRANSMISSIONS_PARAM");
                /* Review comment fix end SYNC_LOSS_2*/

                if((numOfTransmissions >= 1) && (numOfTransmissions <= 8))
                {
                    siMsgReceived_p[i].transmissionCount = numOfTransmissions;
                    siMsgReceived_p[i].currentTransmissionCount = 0;
                }
                else
                {
                    /*SPR 3821 Start*/
                    isFailureFlag = TRUE;
                    break;
                    /* return MAC_FAILURE;*/
                    /*SPR 3821 Ends*/ 
                }
            }
            else
            {
                siMsgReceived_p[i].transmissionCount = MIN_SI_REPETITIONS;
                siMsgReceived_p[i].currentTransmissionCount = 0;
            }
            /* Parsing Number of SI Offset*/
            /* SI Start Offset represent after how many SF in the 
             * SI window SI tranmission can be done, value provided
             * by L3 else default value is 0 */

            tagVal = LTE_GET_U16BIT(*msg_p);
            if(SI_START_OFFSET == tagVal)
            {
                UInt8 offsetNumber = siMsgReceived_p[i].transmissionCount;
                UInt8 offsetIndex = 0;
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;

                length = LTE_GET_U16BIT(*msg_p);
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;

                for(offsetIndex = 0; offsetIndex < MAX_TRANSMISSION_COUNT; offsetIndex++)
                {
                    if(offsetIndex >= offsetNumber)
                    {
                        *msg_p += 1;
                        *remLen_p -= 1;
                        continue;
                    }
                    siStartOffset = **msg_p;
                    *msg_p += 1;
                    /* Review comment fix start SYNC_LOSS_2*/
                    LOG_MAC_MSG(MAC_SI_START_OFFSET_PARAM,LOGINFO,MAC_RRC_INF,
                            getCurrentTick(),
                            offsetIndex,
                            siStartOffset,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            __func__,"MAC_SI_START_OFFSET_PARAM");
                    /* Review comment fix end SYNC_LOSS_2*/
                    /* Range check for SI Offset 0-39
                     * No check for Invalid Offset while 
                     * Window size is less and off set is greater
                     * it must be take care by L3 
                     * */
                    if(siStartOffset < 40)
                    {  
                        siMsgReceived_p[i].offsetValue[offsetIndex] = 
                            siStartOffset;
                    }
                    else
                    {
                        /*SPR 3821 Start*/
                        isFailureFlag = TRUE;
                        break;
                        /* return MAC_FAILURE;*/
                        /*SPR 3821 Ends*/ 
                    }
                    *remLen_p -= 1;
                }
            }
            else
            {
                siMsgReceived_p[i].offsetValue[0] = 0;
            }
            /*  SPR 3067 Chg Ends */

            /* CMAS_CODE END */


            /* Parsing each SI message segment from msg buffer */
            for(siMsgSegCntr = 0;siMsgSegCntr < siMsgReceived_p[i].siMsgSgmnts;siMsgSegCntr++)
            {

                tagVal = LTE_GET_U16BIT(*msg_p);

                if( SI_MSG_BUFFER != tagVal)
                {
                    ltePanic("Invalid tag received = %d\n", tagVal);
                    break;
                }

                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;
                length -= TAG_FIELD_LEN;

                siTlvLen = LTE_GET_U16BIT(*msg_p);

                if((6 > siTlvLen) || (siTlvLen > (*remLen_p + TAG_FIELD_LEN)))
                {
                    ltePanic("Invalid length for tag SI_MSG_BUFFER %d\n",siTlvLen);
                    break;
                }

                *msg_p += LENGTH_FIELD_LEN;
                *remLen_p -= LENGTH_FIELD_LEN;
                length -= LENGTH_FIELD_LEN;

                /*  
                 * Getting length of each SI message segment
                 */
                siMsgReceived_p[i].msgLen[siMsgSegCntr] = LTE_GET_U16BIT(*msg_p);
                *msg_p += 2;
                *remLen_p -= 2;
                length -= 2;


                /* ETWS changes ends */

                /* ETWS changes */
                /*  
                 * Validating length of each SI message segment based on DCI Format
                 * and allocating RB's and mcs index based on msg length.
                 * Msg length should be greater than zero and lesser than
                 * max length of DCI_format depends upon DCI_Format.
                 */
                /*KLOCKWORK_CHANGES_START_2*/ 				
                /* dci_format_parameter_name chaged */
                if ( DCI_FORMAT_1A == cellParams_p->dciFormatForCommonChnlMsg )
                {
                    if( (MAX_SI_MSG_SIZE_FOR_DCI_1A  >= siMsgReceived_p[i].msgLen[siMsgSegCntr]) )
                    {
                        determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1A(
                                siMsgReceived_p[i].msgLen[siMsgSegCntr], &tbSize, 
                                &siMsgReceived_p[i].numRB[siMsgSegCntr], 
                                &siMsgReceived_p[i].mcsIndex[siMsgSegCntr],
                                &siMsgReceived_p[i].tpc[siMsgSegCntr],
                                cellParams_p);
                    }
                    else
                    {
                        lteWarning("[%s]:For DCI Format 1A, Invalid SI message length [%d]\n",
                                __func__,siMsgReceived_p[i].msgLen[siMsgSegCntr]);
                        isFailureFlag = TRUE;
                        break;
                    }
                }
                else if( DCI_FORMAT_1C == cellParams_p->dciFormatForCommonChnlMsg )
                {
                    if((MAX_SI_MSG_SIZE_FOR_DCI_1C  >= siMsgReceived_p[i].msgLen[siMsgSegCntr]) )
                    { 
                        determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1C(
                                siMsgReceived_p[i].msgLen[siMsgSegCntr], &tbSize,
                                &siMsgReceived_p[i].numRB[siMsgSegCntr],
                                &siMsgReceived_p[i].mcsIndex[siMsgSegCntr],
                                &siMsgReceived_p[i].tpc[siMsgSegCntr],
                                cellParams_p);
                    }
                    else
                    {
                        lteWarning("[%s]:For DCI Format1C, Invalid SI message length [%d]\n",
                                __func__,siMsgReceived_p[i].msgLen[siMsgSegCntr]);
                        isFailureFlag = TRUE;
                        break;
                    }
                }
                /*KLOCKWORK_CHANGES_STOP_2*/
                {
                    /*Allocate memory*/
                    /* ETWS changes */
                    GET_MEM_FROM_POOL(void,siMsgReceived_p[i].msg_p[siMsgSegCntr], 
                            /* ETWS changes ends */
                            ( tbSize + DATA_MSG_API_HEADER_LEN),
                            PNULL);

                    /* ETWS changes */
                    memSet(siMsgReceived_p[i].msg_p[siMsgSegCntr], 0,
                            (tbSize + DATA_MSG_API_HEADER_LEN));
                    memCpy(siMsgReceived_p[i].msg_p[siMsgSegCntr] + DATA_MSG_API_HEADER_LEN,
                            *msg_p, siMsgReceived_p[i].msgLen[siMsgSegCntr]);

                    /*Removed log from here*/

                    *msg_p += siMsgReceived_p[i].msgLen[siMsgSegCntr];
                    *remLen_p -= siMsgReceived_p[i].msgLen[siMsgSegCntr];
                    length -= siMsgReceived_p[i].msgLen[siMsgSegCntr];

                    siMsgReceived_p[i].msgLen[siMsgSegCntr] = tbSize;
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                            "\n[%s]: SI Msg - %d\n"
                            "\t msgLen  [%d]\n"
                            "\t numRB   [%d]\n"
                            "\t siPeriodicity [%d]\n"
                            "\t startSFN      [%d]\n"
                            "\t startSubFrame [%d]\n",
                            __func__,(i+1),siMsgReceived_p[i].msgLen[siMsgSegCntr],
                            siMsgReceived_p[i].numRB[siMsgSegCntr],
                            siMsgReceived_p[i].siPeriodicity,
                            siMsgReceived_p[i].startSFN,
                            siMsgReceived_p[i].startSubFrame);
                }
            }//for(siMsgSegCntr)

            if(siMsgSegCntr != siMsgReceived_p[i].siMsgSgmnts)
            {
                lteWarning("[%s]: SI message parsing failed due to invalid tag value\n",
                        __func__);
                break;
            }
            /* ETWS changes ends */
            do
            {
                /* SPR 1614 changes end */
                tagVal = LTE_GET_U16BIT(*msg_p);
                if(SI_MSG_INFO_PARAM != tagVal)
                {
                    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
                            "[%s]: Tag value [%d] is not equal to SI_MSG_INFO_PARAM"
                            "break from the loop\n",__func__,**msg_p);
                    break;
                }
                /* ETWS changes */
                /* ETWS changes ends */
                /*Increment the pointer to point to Length field*/
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;

                length = LTE_GET_U16BIT(*msg_p);
                /*Increment the pointer to point to SFN field*/
                *msg_p += LENGTH_FIELD_LEN;
                *remLen_p -= LENGTH_FIELD_LEN;

                /* Ignore the parameters in this TLV for time being */
                *msg_p += (length - TAG_FIELD_LEN - LENGTH_FIELD_LEN);
                *remLen_p -= (length - TAG_FIELD_LEN - LENGTH_FIELD_LEN);
            }while(*remLen_p > 0);

            /* SIB8_Code Start */

            tagVal = LTE_GET_U16BIT(*msg_p);
            if( SCHEDULING_REQD_FOR_SIB8_INFO == tagVal)
            {
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;

                length = LTE_GET_U16BIT(*msg_p);
                *msg_p += TAG_FIELD_LEN;
                *remLen_p -= TAG_FIELD_LEN;

                if( SCHEDULING_REQD_FOR_SIB8_TAG_LEN != length )
                {
                    ltePanic("Invalid tag length for SCHEDULING_REQD_FOR_SIB8_INFO = %d\n", length );
                }

                si8Flag = **msg_p;
                /* Review comment fix start SYNC_LOSS_2*/
                LOG_MAC_MSG(MAC_SCHEDULING_REQD_FOR_SIB8_INFO_PARAM,LOGINFO,MAC_RRC_INF,
                        getCurrentTick(),
                        si8Flag,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__,"MAC_SCHEDULING_REQD_FOR_SIB8_INFO_PARAM");
                /* Review comment fix end SYNC_LOSS_2*/

                *msg_p += 1;
                *remLen_p -= 1;
                length -= 1;

                /* SIB8_Code_phase2 Start */
                if( SIB8_MAX_TAG <= si8Flag )
                {
                    ltePanic("Invalid tag value for SCHEDULING_REQD_FOR_SIB8_INFO = %d\n", si8Flag );
                }
                /* SIB8_Code_phase2 End*/

                siMsgReceived_p[i].si8Flag = si8Flag;

            }
            /* SIB8_Code End */

        }
    }

    if ((i == numSIMsg) && (FALSE == isFailureFlag))
    {
        siMsgInfo_p->sfn = (UInt16)sysFrameNum;

        if(MAC_FAILURE == createSIMapTable(
                    &siMsgReceived_p[0], 
                    numSIMsg, 
                    siWindowSize,
                    cellCfgFlag,
                    internalCellIndex))
        {
            isFailureFlag = TRUE;
        }        
    }
    /*SPR 8094 Fix Start*/
    else
    {
        isFailureFlag = TRUE;
    }
    /*SPR 8094 Fix End*/

    if (TRUE == isFailureFlag)
    {
        /* cyclomatic changes */
        cleanupForSIDecodeFail(i,&siMsgReceived_p[0]);
        return MAC_FAILURE;
        /* cyclomatic changes */
    }

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "Exit[%s]()\n",__func__);

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : validateStartSFNAndSF
 * Inputs         : siMsgInfoReceived_p - Pointer to array of type 
 *                                        SIMsgInfoFromRRCT structure,
 *                  siWindowSize - SI-Window length,
 *                  orderOfSIMsg - order of entry in the list of SI messages in 
 *                                 SIB1 msg
 * Outputs        : None
 * Returns        : MAC_FAILURE/MAC_SUCCESS
 * Description    : This will validate start SFN and SF.
 *****************************************************************************/
MacRetType validateStartSFNAndSF(SIMsgInfoFromRRC *siMsgInfoReceived_p,
        UInt8 siWindowSize,UInt8 orderOfSIMsg)
{
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNum = 0;
    UInt32 x = 0;
    UInt32 y = 0;
    UInt32 periodicity = 0;

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"Entry [%s]()\n",__func__);

    /*E-UTRAN should configure an SI-window of 1 ms only if all SIs are 
     *scheduled before subframe #5 in radio frames for which SFN mod 2 = 0.
     */
    if ((1 == siWindowSize) &&
            (0 == MODULO_TWO(siMsgInfoReceived_p->startSFN)) &&
            (siMsgInfoReceived_p->startSubFrame > 4)
       )  
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Validation failed Invalid start SF received [%d].\n"
                "E-UTRAN should configure an SI-window of 1 ms only if all SIs are\n" 
                "scheduled before subframe #5 in radio frames for which SFN mod 2 = 0\n"
                "siWindowSize [%d]\n",
                __func__,siMsgInfoReceived_p->startSubFrame, subFrameNum,siWindowSize);

        /*Validation failed*/
        return MAC_FAILURE;
    }

    /*As per the specification 36.331 start SFN and SF are calculated by using 
     *following formula
     *x = (n - 1)*w, where w is the si-WindowLength
     *n which corresponds to the order of entry in the list of SI messages 
     *configured by schedulingInfoList in SystemInformationBlockType1
     *Subframe a = (x mod 10)
     *starting SFN is calculated by (SFN mod T) = FLOOR(x/10) 
     *where T is the si-Periodicity of the concerned SI message
     */
    x = (orderOfSIMsg - 1) * siWindowSize;
    /*Find the starting SF*/
    subFrameNum = x % 10;

    /*If the received SF != SF obtained from formule*/
    if (subFrameNum != siMsgInfoReceived_p->startSubFrame)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Validation failed startSubFrame [%d] != "
                "subFrameNum [%d] calculated from formule\n",
                __func__,
                siMsgInfoReceived_p->startSubFrame, subFrameNum);

        /*Validation failed*/
        return MAC_FAILURE;
    }
    periodicity = siMsgInfoReceived_p->siPeriodicity;
    /** ETWS changes **/
    y = (UInt32)floor_wrapper((float)x/10);
    /** ETWS changes **/
    /*Find the start SFN*/

    while((sysFrameNum % periodicity) != y)
    {
        if (MAX_SFN_VALUE == sysFrameNum)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                    "[%s]: ERROR Validation failed received startSFN [%d]"
                    "not stisfying the formule\n",
                    __func__,siMsgInfoReceived_p->startSFN);            

            return MAC_FAILURE;
        }
        else
        {
            sysFrameNum++;
            /*
               if(sysFrameNum % periodicity == 0)
               y-=periodicity;*/
            continue;
        }

    }

    /* ETWS changes */
    /* ETWS changes ends */
    if (sysFrameNum != siMsgInfoReceived_p->startSFN)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,BCCH_MGR,
                "[%s]: ERROR Validation failed received startSFN [%d]\n"
                "      not equal to the SFN [%d]calculated from formule\n",
                __func__,siMsgInfoReceived_p->startSFN,sysFrameNum);

        return MAC_FAILURE;
    }
    /* ETWS changes ends */

    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,
            "Exit[%s]()\n",__func__); 

    return MAC_SUCCESS;
}    


/*****************************************************************************
 * Function Name  : fillCmasScheduleInfo
 * Inputs         : siMapActiveIndex - SI Map Active Index,
 *                  siMsgActiveIndex - SI Message Active Index,
 *                  tempSFN - Temporaray System frame number,
 *                  subFrame - Current Subframe,
 *                  i,
 *                  flag
 * Outputs        : siMsgInfo_p - Message info of SI 
 * Returns        : None
 * Description    : This funcrion will update the CMAS related informatin in
 *                  SI message
 *****************************************************************************/
static  void fillCmasScheduleInfo(SIInfo *siMsgInfo_p,UInt8 siMapActiveIndex,
        UInt8 siMsgActiveIndex,UInt16 tempSFN,UInt8 subFrame,UInt8 i,UInt8 flag)
{
    UInt8 prevCMASMessage  = 0;

    if(siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].lastCMASMessage == 0)
    {
        prevCMASMessage = (siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType)-1;
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[prevCMASMessage].splSIMsgflag = TRUE;
    }
    else
    {
        prevCMASMessage = (siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].lastCMASMessage)-1;
    }

    siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].splSIMsgflag= TRUE;
    siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].sendSIMsgFlag = flag;
    siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[prevCMASMessage].nextCMASMessageToBeScheduled = i+1;
    siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].lastCMASMessage = i+1;

}

/***********************************************************************************
 * Function Name  : createSIMapTable
 * Inputs         : siMsgInfoReceived_p - Pointer to array of type 
 *                                        SIMsgInfoFromRRCT structure,
 *                  numSIMsg - Number of SI messages received,
 *                  siWindowSize - SI-Window length,
 *                  cellCfgFlag - Cell config req flag
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will creates the SI map table based on the received input.
 ***********************************************************************************/
MacRetType createSIMapTable(SIMsgInfoFromRRC *siMsgInfoReceived_p,
        UInt32 numSIMsg,
        UInt32 siWindowSize,
        UInt32 cellCfgFlag,
        InternalCellIndex internalCellIndex)
/* CA Changes end */

{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* CMAS_CODE START */
    /*coverity-530 CID-32297,96,95*/
    UInt32 i = 0;
    /*coverity-530 CID-32297*/
    UInt8 siMapActiveIndex = 0;
    UInt8 siMsgActiveIndex = 0;

    UInt8  numOfSITransmission = 0;
    UInt8  offsetIndex = 0;
    SIInfo *siMsgInfoPerCell_p = &cellParams_p->siMsgInfo;
    UInt8 splSiPeriodicityCheck = 0;
    UInt16 sysFrame    = 0;
    UInt8 subFrame    = 0;
    UInt16 tempSFN     = 0;
    /* CMAS_CODE END */
    /* SPR 3067 Chg */
    UInt8  offsetValue = 0;
    UInt8  currentOffset = 0;
    UInt32 siWindowLen = 0;
    /* SPR 3067 Chg Ends */
    /* ETWS changes */
    UInt8   siMsgSegCntr= 0;
    UInt8  siMsgSgmnts = 0;

    /* ETWS changes ends */
    SIInfo *siMsgInfo_p = PNULL;

    /* SIB8_Code Start */
    UInt8  si8Flag = 0;
    /* SIB8_Code End */


    LTE_MAC_UT_LOG(LOG_INFO,BCCH_MGR,"Entry [%s]()\n",__func__);
    siMsgInfo_p = siMsgInfoPerCell_p;

    if (FALSE == cellCfgFlag)
    {
        siMsgActiveIndex = (siMsgInfo_p->siMsgActiveIndex)?(0):(1);
        siMapActiveIndex = (siMsgInfo_p->siMapActiveIndex)?(0):(1);
    }
    else
    {
        siMsgActiveIndex = siMsgInfo_p->siMsgActiveIndex;
        siMapActiveIndex = siMsgInfo_p->siMapActiveIndex;
    }

    cleanSIMsg(siMsgInfo_p, siMsgActiveIndex, siMapActiveIndex);

    for (i = 0; i < numSIMsg; i++)
    {

        /* ETWS changes */
        /*  
         * Filling each SI message info for each message segment.  
         * DCI format for all message segments is Unique
         */

        siMsgSgmnts = siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].siMsgSgmnts
            = (siMsgInfoReceived_p + i)->siMsgSgmnts;

        /* dci_format_parameter_name chaged */
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].dciFormat = 
            cellParams_p->dciFormatForCommonChnlMsg;
        /* dci_format_parameter_name chaged */

        /* CMAS_CODE START */
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].siMsgSgmntToBeScheduled = 0;
        /* CMAS_CODE END */

        /* SIB8_Code Start */
        si8Flag = siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].si8Flag = (siMsgInfoReceived_p + i)->si8Flag;

        /* SIB8_Code_phase2 Start */
        if(NO_SIB8 != si8Flag && SIB8_NO_CHANGE != si8Flag)
            /* SIB8_Code _phase2 End */
        {

            UInt8  si8Transmission = 0;
            UInt8  si8OffsetIndex = 0;

            SIMsg8  *siMsg8buff_p = PNULL;

            siMsg8buff_p = &(cellParams_p->siMsg8buff[siMsg8Indx_g[internalCellIndex]]);

            /* SIB8_Code_phase2 Start */
            if((INDEX_STATUS_SCHEDULING == siMsg8buff_p->indexStatus) && (SIB8_FLUSH_OLD_BUFFER_APPLY_NEW_BUFFER != si8Flag))
                /* SIB8_Code_phase2 End*/
            {
                siMsg8Indx_g[internalCellIndex] =  (siMsg8Indx_g[internalCellIndex]+1)%MAX_NO_SI8_MSG_INDX;
                siMsg8buff_p = &(cellParams_p->siMsg8buff[siMsg8Indx_g[internalCellIndex]]);
            }
            else
            {
                //Free buffer siMsg8buff_p[siMsg8Indx]
                //This part of code will be moved to seprate function after code
                //review . Memory needs to be cleaned in case of cleanBCCH Mgr
                cleanS8buffer(siMsg8buff_p);


            }
            /* dci_format_parameter_name chaged */
            siMsg8buff_p->dciFormat = cellParams_p->dciFormatForCommonChnlMsg;
            /* dci_format_parameter_name chaged */
            siMsg8buff_p->siMsgReptitions= (siMsgInfoReceived_p + i)->siMsgReptitions;
            siMsg8buff_p->siWindowSize = (siMsgInfoReceived_p + i)->siWindowSize;
            siMsg8buff_p->remaingWindowSize = (siMsgInfoReceived_p + i)->remaingWindowSize;
            si8Transmission = siMsg8buff_p->transmissionCount = (siMsgInfoReceived_p + i)->transmissionCount;
            siMsg8buff_p->currentTransmissionCount = (siMsgInfoReceived_p + i)->currentTransmissionCount;

            while(si8Transmission)
            {
                siMsg8buff_p->offsetValue[offsetIndex] = (siMsgInfoReceived_p + i)->offsetValue[offsetIndex];
                si8OffsetIndex++;si8Transmission--;
            }

            siMsg8buff_p->totalSiMsg8Sgmnts = siMsgSgmnts;
            for(siMsgSegCntr = 0 ; siMsgSegCntr  < siMsg8buff_p->totalSiMsg8Sgmnts ;siMsgSegCntr++)
            {

                /*SPR 8094 Fix Start*/
                if (NULL != siMsg8buff_p->msg_p[siMsgSegCntr])
                {
                    freeMemPool(siMsg8buff_p->msg_p[siMsgSegCntr]);
                    siMsg8buff_p->msg_p[siMsgSegCntr] = PNULL; 
                }
                /*SPR 8094 Fix End*/
                siMsg8buff_p->msg_p[siMsgSegCntr] = (siMsgInfoReceived_p + i)->msg_p[siMsgSegCntr];
                siMsg8buff_p->msgLen[siMsgSegCntr] = (siMsgInfoReceived_p + i)->msgLen[siMsgSegCntr];
                siMsg8buff_p->numRB[siMsgSegCntr] = (siMsgInfoReceived_p + i)->numRB[siMsgSegCntr];
                siMsg8buff_p->mcsIndex[siMsgSegCntr] = (siMsgInfoReceived_p + i)->mcsIndex[siMsgSegCntr];
                /*SPR 7885 Change Start*/
                siMsg8buff_p->tpc[ siMsgSegCntr] = (siMsgInfoReceived_p + i)->tpc[siMsgSegCntr];
                /*SPR 7885 Change End */
            } //for_loop(siMsgSegCntr)
            siMsg8buff_p->indexStatus =  INDEX_STATUS_FILLED;
            cellParams_p->siMsg8latestIndexFilled  = siMsg8Indx_g[internalCellIndex];

        }
        /* SIB8_Code End */

        else
        {
            for(siMsgSegCntr = 0;siMsgSegCntr < siMsgSgmnts;siMsgSegCntr++)
            {
                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].msg_p[siMsgSegCntr] = 
                    (siMsgInfoReceived_p + i)->msg_p[siMsgSegCntr];

                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].msgLen[siMsgSegCntr] = 
                    (siMsgInfoReceived_p + i)->msgLen[siMsgSegCntr];

                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].numRB[siMsgSegCntr] = 
                    (siMsgInfoReceived_p + i)->numRB[siMsgSegCntr];

                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].mcsIndex[siMsgSegCntr] = 
                    (siMsgInfoReceived_p + i)->mcsIndex[siMsgSegCntr];

                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].tpc[siMsgSegCntr] = 
                    (siMsgInfoReceived_p + i)->tpc[siMsgSegCntr];

            }//for_loop(siMsgSegCntr)
        }
        /* ETWS changes ends */

        /* SPR 3067 Chg */
        /* Below parameters added to support multiple tranmission of
         * SI message in the same SI window with offset and RV value */
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].siWindowSize =
            (siMsgInfoReceived_p + i)->siWindowSize;

        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].remaingWindowSize =
            (siMsgInfoReceived_p + i)->remaingWindowSize;

        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].transmissionCount =
            (siMsgInfoReceived_p + i)->transmissionCount;

        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].currentTransmissionCount =
            (siMsgInfoReceived_p + i)->currentTransmissionCount;

        /* CMAS_CODE START */
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].siMsgReptitions = (siMsgInfoReceived_p + i)->siMsgReptitions;
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].nextCMASMessageToBeScheduled = 0;
        siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].schedulingCompleted = 0;

        /* CMAS_CODE END */

        numOfSITransmission = 
            siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].transmissionCount;

        offsetIndex = 0;

        /* Getting SI Offset Values */
        while(numOfSITransmission)
        {
            siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].offsetValue[offsetIndex] =
                (siMsgInfoReceived_p + i)->offsetValue[offsetIndex];
            offsetIndex++;
            numOfSITransmission--;
        }

        /* SPR 3067 Chg Ends */



        /* ETWS changes */
        /*For each SI message go to the periodic node and 
         *set the sendSIMsgFlag and initialize siMsgType to 
         *i (i.e SI message Type)
         */
        sysFrame = (siMsgInfoReceived_p + i)->startSFN;
        /* ETWS changes ends */

        while (sysFrame < MAX_SI_PERIODICITY)
        {
            subFrame    = (siMsgInfoReceived_p + i)->startSubFrame;
            tempSFN     = sysFrame;
            /* ETWS changes */
            /* ETWS changes ends */

            /* SPR 3067 Chg */
            numOfSITransmission = 
                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].transmissionCount;
            /* SPR 3067 Chg Ends */

            /*Each SI message is associated with a SI-window and the 
             *SI-windows of different SI messages do not overlap. 
             *That is, within one SI-window only the corresponding 
             *SI is transmitted. The length of the SI-window is common 
             *for all SI messages, and is configurable. Within the SI-window
             *the corresponding SI message can be transmitted a number of 
             *times in any subframe subframe #5 of radio frames for which 
             *SFN mod 2 = 0. 
             */

            /* SPR 3067 Chg */
            offsetIndex = 0;
            siWindowLen = siWindowSize;
            currentOffset = 0;

            /* CMAS_CODE START */
            while (siWindowLen) 
            {
                offsetValue = 
                    siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].\
                    offsetValue[offsetIndex];
                if((currentOffset == offsetValue)
                        && (offsetIndex < numOfSITransmission))
                {
                    /* SPR 3067 Chg Ends */
#ifdef FDD_CONFIG
                    if (( (0 != (MODULO_TWO(tempSFN))) && (5 == subFrame)) || (subFrame != 5))
#elif TDD_CONFIG
                        /* SPR_4748_FIX Start */
                        if( (( (0 != (MODULO_TWO(tempSFN))) && (5 == subFrame))  || (subFrame != 5)) &&  (DL == getSubframeType(subFrame, internalCellIndex)))
                            /* SPR_4748_FIX End */
#endif
                        {
                            if(TRUE == siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].sendSIMsgFlag)
                            {
                                splSiPeriodicityCheck = (siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType)-1;
                                if( (siMsgInfoReceived_p + splSiPeriodicityCheck)->siPeriodicity != (siMsgInfoReceived_p + i)->siPeriodicity )
                                {
                                    return MAC_FAILURE;
                                }
                                fillCmasScheduleInfo(siMsgInfo_p,siMapActiveIndex,siMsgActiveIndex,tempSFN,subFrame,i,TRUE);

                            }
                            else
                            {
                                siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].sendSIMsgFlag = TRUE;
                                /*--coverity--530-32297*/
                                siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType =(SIMsgType)( i+1);
                                /*--coverity--530-32297*/
                            }
                        }
                        else 
                        {    
                            if(siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType == INVALID_SI_MSG)
                            {  
                                /*--coverity--530-32295*/
                                siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType = (SIMsgType)(i+1);
                                /*--coverity--530-32295*/
                            }
                            else
                            {
                                splSiPeriodicityCheck = (siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType)-1;
                                if( (siMsgInfoReceived_p + splSiPeriodicityCheck)->siPeriodicity != (siMsgInfoReceived_p + i)->siPeriodicity )
                                {
                                    return MAC_FAILURE;
                                }
                                fillCmasScheduleInfo(siMsgInfo_p,siMapActiveIndex,siMsgActiveIndex,tempSFN,subFrame,i,FALSE);
                            }
                        }
                    offsetIndex++;
                }
                else if(numOfSITransmission == offsetIndex)
                {
                    if(TRUE == siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].sendSIMsgFlag)
                    { 
                        splSiPeriodicityCheck = (siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType)-1;
                        if( (siMsgInfoReceived_p + splSiPeriodicityCheck)->siPeriodicity != (siMsgInfoReceived_p + i)->siPeriodicity )
                        {
                            return MAC_FAILURE;
                        }
                        fillCmasScheduleInfo(siMsgInfo_p,siMapActiveIndex,siMsgActiveIndex,tempSFN,subFrame,i,TRUE);

                    }
                    else
                    {
                        siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].
                            subFrame[subFrame].sendSIMsgFlag = TRUE;
                        /*--coverity--530-32296*/
                        siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].
                            subFrame[subFrame].siMsgType =(SIMsgType)( i+1);
                        /*--coverity--530-32296*/
                    }
                }
                else
                {
                    if(siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType == INVALID_SI_MSG)
                    {  
                        /*--coverity--530-32295*/
                        siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType = (SIMsgType)(i+1);
                        /*--coverity--530-32295*/
                    }
                    else
                    {
                        splSiPeriodicityCheck = (siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType)-1;
                        if( (siMsgInfoReceived_p + splSiPeriodicityCheck)->siPeriodicity != (siMsgInfoReceived_p + i)->siPeriodicity )
                        {
                            return MAC_FAILURE;
                        }
                        fillCmasScheduleInfo(siMsgInfo_p,siMapActiveIndex,siMsgActiveIndex,tempSFN,subFrame,i,FALSE);
                    }
                    //siMsgInfo_p->siMapTable[siMapActiveIndex][tempSFN].subFrame[subFrame].siMsgType = i;
                }

                subFrame++;
                if (subFrame >= 10)
                {
                    subFrame = 0;
                    tempSFN++;
                    if (tempSFN >= 512)
                        tempSFN = 0;
                }
                currentOffset++;
                siWindowLen--;
            }
            /* CMAS_CODE END */
            sysFrame += (siMsgInfoReceived_p + i)->siPeriodicity;
        }
    }
    siMsgInfo_p->siMsg[siMsgActiveIndex].numSImsg = numSIMsg;

    if (FALSE == cellCfgFlag)
    {
        siMsgInfo_p->siInfoUpdateFlag  = TRUE;
    }

    return MAC_SUCCESS;
}


/* SIB8_Code Start */

/*****************************************************************************
 * Function Name  : cleanS8buffer
 * Inputs         : SIMsg8 siMsg8buff_p - Pointer to structure SIMsg8
 * Outputs        : None
 * Returns        : None
 * Description    : This will clean the S8 Buffer.
 *****************************************************************************/
void cleanS8buffer(SIMsg8 *siMsg8buff_p)
{
 
  UInt8 siMsgSegCntr = 0;

    for(siMsgSegCntr = 0;siMsgSegCntr < siMsg8buff_p->totalSiMsg8Sgmnts;siMsgSegCntr++)
    {
        if (siMsg8buff_p->msg_p[siMsgSegCntr])
        {
            freeMemPool(siMsg8buff_p->msg_p[siMsgSegCntr]);
            siMsg8buff_p->msg_p[siMsgSegCntr] = PNULL;
        }
    }
    memSet((void *) siMsg8buff_p , 0,sizeof(SIMsg8));

}
/* SIB8_Code End */

/*****************************************************************************
 * Function Name  : cleanSIMsg
 * Inputs         : siMsgInfo_p - Pointer to array of type 
 *                                SI messages structure,
 *                  siMsgActiveIndex - Current SI active Index,
 *                  siMapActiveIndex - Current SI Map Active Index
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup all the SI messages and Map.
 *****************************************************************************/

void cleanSIMsg(SIInfo *siMsgInfo_p, UInt32 siMsgActiveIndex,
        UInt32 siMapActiveIndex)
{
    UInt32 i = 0;

    /* ETWS changes */
    UInt8  siMsgSgmnts = 0;
    UInt8  siMsgSegCntr = 0;
    /* ETWS changes ends */

    /*Free SI messages*/
    for (i = 0; i < siMsgInfo_p->siMsg[siMsgActiveIndex].numSImsg; i++)
    {
        /* ETWS changes */
        /*  
         * Cleaning each SI message according to segment wise 
         */
        siMsgSgmnts = siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].siMsgSgmnts;

        for(siMsgSegCntr = 0;siMsgSegCntr < siMsgSgmnts;siMsgSegCntr++)
        {
            if (siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].msg_p[siMsgSegCntr])
            {
                freeMemPool(siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].msg_p[siMsgSegCntr]);
                siMsgInfo_p->siMsg[siMsgActiveIndex].siMsgBuff[i].msg_p[siMsgSegCntr] = PNULL;
            }
        }
        /* ETWS changes ends */
    }

    memSet((void *)&siMsgInfo_p->siMapTable[siMapActiveIndex] , 0, 
            (MAX_SIMAP_TABLE_NODES * sizeof(SIMapTable)));

    memSet((void *)&siMsgInfo_p->siMsg[siMsgActiveIndex] , 0, 
            sizeof(SIMsgInfo));

    return;
}

/****************************************************************************
 * Function Name  : initNbBCCHMgr 
 * Inputs         : Number of cells configured
 * Outputs        : Initialize the Queue's & List
 * Returns        : None 
 * Description    : This function is used to initialize the Queue's and List
 ****************************************************************************/
