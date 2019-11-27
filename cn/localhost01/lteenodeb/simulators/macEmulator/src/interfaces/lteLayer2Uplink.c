/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2Uplink.c,v 1.3.6.1.4.2.2.2 2010/09/29 05:10:16 gur04640 Exp $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the functions interfacing the MAC 
 *                     and receiving messages from the Physical layer. 
 *                     - Initiating the receiver thread.
 *                     - Reading the message from the socket, decoding it 
 *                       and then putting the entry in the ACK/NACK Queue
 *                       along with stopping the associated timer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2Uplink.c,v $
 * Revision 1.3.6.1.4.2.2.2  2010/09/29 05:10:16  gur04640
 * scheduled tick passed to RLC
 *
 * Revision 1.3.6.1.4.2.2.1  2010/09/21 15:53:36  gur20491
 * FAPI changes
 *
 * Revision 1.3.6.1  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2010/02/16 07:36:06  gur15697
 * ul Dataprocess thread removed
 *
 * Revision 1.2  2010/02/13 10:47:58  gur15697
 * changes for UL flow Stability
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.18  2009/09/22 13:46:11  gur19413
 * getBSRIndex optimised changes
 *
 * Revision 1.17  2009/09/16 11:02:10  gur19413
 * optimised the code
 *
 * Revision 1.16  2009/09/03 14:26:15  gur20439
 * ltePanic put in macro LTE_MAC_UT_LOG.
 *
 * Revision 1.15  2009/09/02 13:34:50  gur20439
 * printf put in UT_TESTING flag.
 *
 * Revision 1.14  2009/07/29 15:19:27  gur20439
 * look and feel to simulator fix.
 *
 * Revision 1.13  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.12  2009/07/12 08:12:45  gur15697
 * (tbsize)/8 to changed to tbsize
 *
 * Revision 1.11  2009/06/25 11:45:24  gur18569
 * fixed bugs
 *
 * Revision 1.10  2009/06/18 15:01:05  gur20439
 * ut bug fixes.
 *
 * Revision 1.9  2009/06/16 09:11:57  gur20439
 * changes to use traffic generator.
 *
 * Revision 1.8  2009/06/15 13:55:03  gur20439
 * modified for release 1.2.
 *
 * Revision 1.7  2009/06/10 14:30:55  gur20439
 * changes made for random access.
 *
 * Revision 1.6  2009/06/08 13:58:36  gur20439
 * ut bug fixing.
 *
 * Revision 1.5  2009/06/01 10:53:31  gur20439
 * include/interfaces/phySimulatorApiHandler.h
 *
 * Revision 1.4  2009/05/26 08:47:42  gur20439
 * lteLayer2Uplink.c
 *
 * Revision 1.3  2009/05/26 07:29:44  gur20439
 * modified for mac release 1.2
 *
 * Revision 1.2  2009/05/01 06:58:58  gur20439
 * msg Type included as BSR.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.44  2009/03/02 10:59:01  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.43  2009/02/24 14:01:13  gur11912
 * Optimization Changes
 *
 * Revision 1.42  2009/02/20 14:29:27  gur18569
 * uplink throughput changes
 *
 * Revision 1.41  2009/02/19 07:00:53  gur19413
 * remove memcpy function
 *
 * Revision 1.40  2009/02/03 05:57:58  gur18569
 * removed unnecessary sem ackNackQueueSem_g
 *
 * Revision 1.39  2009/01/29 18:55:03  gur03939
 * Mdoifeid code for message queues with PHY SIM
 *
 * Revision 1.38  2009/01/29 11:50:26  gur03939
 * Changed code for Uplink and Downlink throughput
 *
 * Revision 1.37  2009/01/28 12:20:59  gur03939
 * Changes to avoid memcpy with shared memory interfaces
 *
 * Revision 1.36  2009/01/27 12:15:42  gur19413
 * throughput function added
 *
 * Revision 1.35  2009/01/27 07:21:57  gur20052
 * Incorporated Vandana's Comments
 *
 * Revision 1.34  2009/01/25 11:51:52  gur20052
 * Added During Regression Testing
 *
 * Revision 1.33  2009/01/24 13:49:05  gur18569
 * changed bandnum and cqi index of size one byte
 *
 * Revision 1.32  2009/01/24 10:17:24  gur11912
 * Regression bugs fixed
 *
 * Revision 1.31  2009/01/20 16:14:29  gur11912
 * Updated for Porting
 *
 * Revision 1.30  2009/01/20 07:14:48  gur19413
 * updated
 *
 * Revision 1.29  2009/01/17 20:38:41  gur11447
 * Changes for 1 msec
 *
 * Revision 1.28  2009/01/13 05:39:10  gur11912
 * SubBand Pointer reinitialized to PNULL
 *
 * Revision 1.27  2009/01/07 12:32:34  gur12905
 * initializedall the parameters of controlInfo and ulMsg with default
 * or NULL values.
 *
 * Revision 1.26  2009/01/06 16:43:33  gur12905
 * timing advance in controlInfo is initialized with 0
 *
 * Revision 1.25  2009/01/06 15:59:48  gur12905
 * cqiFlag in controlInfo is initialized with false
 *
 * Revision 1.24  2009/01/06 15:37:30  gur12905
 * Pointer not incremented in fillPeriodicCqiMode20Info in case of rmode 1
 *
 * Revision 1.23  2008/12/31 06:44:28  gur20052
 * changes for AllocTBSize are added
 *
 * Revision 1.22  2008/12/31 06:39:38  gur12905
 * Changes done for setting numSubBands and widebandCqi Index to 0 and -1
 * in case of periodic CQI when any one of these do not arrived.
 *
 * Revision 1.21  2008/12/30 06:46:06  gur11447
 * Chnges added for handling if Inclomg TTI is zero --- KT
 *
 * Revision 1.20  2008/12/29 12:08:36  gur12905
 * Changes done for case if invalid CQI reports arrived
 *
 * Revision 1.19  2008/12/26 15:09:05  gur12905
 * Changes done for ZERO buffer copy
 *
 * Revision 1.18  2008/12/24 13:07:41  gur12905
 * Log Added for UpLink SR Request
 *
 * Revision 1.17  2008/12/24 09:13:50  gur12905
 * Changes done for new thread for demux manager.
 *
 * Revision 1.16  2008/12/23 08:13:26  gur12905
 * Logs and validations done
 *
 * Revision 1.15  2008/12/23 07:13:43  gur12905
 * Modified for decoding length and RNTI in decodeIncoming data function
 *
 * Revision 1.14  2008/12/19 13:19:06  gur20052
 * The pthread sys calls are replaced with wrappers
 *
 * Revision 1.13  2008/12/18 12:33:54  gur12905
 * Review comments incorporated
 *
 * Revision 1.12  2008/12/10 10:20:57  gur12905
 * Updated for logs
 *
 * Revision 1.11  2008/12/09 11:49:55  gur19836
 * Updated during linking
 *
 * Revision 1.10  2008/12/08 13:01:42  gur12905
 * Modified during phase-2 merging
 *
 * Revision 1.9  2008/12/05 12:46:53  gur18569
 * multicore changes
 *
 * Revision 1.8  2008/12/02 12:22:54  gur11912
 * Incorporated multiple worker threads changes
 *
 * Revision 1.7  2008/10/22 12:38:51  gur11912
 * fixed for performance changes
 *
 * Revision 1.6  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.5  2008/10/14 06:18:40  gur11912
 * Log Level added
 *
 * Revision 1.4  2008/09/27 15:41:20  gur11912
 * UT Defect Fixed
 *
 * Revision 1.3  2008/09/26 07:14:51  gur11912
 * UT Defect Fixed
 *
 * Revision 1.2  2008/09/19 07:13:36  gur11912
 * review comments incorporated
 *
 * Revision 1.1  2008/09/09 05:12:53  gur11974
 * Initial Version
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include "lteLayer2UEContext.h"
#include "lteLayer2RLCInterface.h"
#include "lteLayer2UplinkUEContext.h"
#include "lteFapiControlMsgHandler.h"
#include "phySimulatorApiHandler.h"
/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
SInt32 recvSockFD_g = 0;

/*CLPC_CHG*/
extern PHRProfile phrProfileInfo_g[MAX_NUM_CELLS][MAX_PHR_PROFILES];
extern ueProfile ueProfileInfo_g[MAX_NUM_CELLS][MAX_UE_PROFILES];
extern ueContextForUplink *uplinkContextInfoMap_g[MAX_UE_SUPPORTED];
extern dynamicUEContext *dynUEContextInfoMap_g[MAX_UE_SUPPORTED];
extern void pushEntryInOnlyBsrQueue(  UInt8  *bsrPdu_p,
                                             UInt16 bsrPduLen,
                                             UInt16 crnti,
                                             UInt8  sendSF,
                                             UInt16 sendSFN,
                                             UInt8  qIndex,
                                             UInt16 pduChoice,
                                             UInt8  cqiMode
                                          );

MacRetType distributeRBsAndSendReqToRLC( UInt16 ueIndex, 
                                         UInt32 tbsize, 
                                         UInt8 bsrPresentFlag, 
                                         void *uldataNode_p, 
                                         UInt8 **data_p, 
                                         UInt16 *dataLen_p,
                                         UInt8 phrTriggered
#ifdef FDD_CONFIG
                                         ,TransmissionAndRLCData **transmissionBuffer_p
#endif
                                         ,UInt8 cellIndex );
extern void prepareDataPduNode( UInt8  *pdu_p,
    UInt16 pduLen,
    UInt16 crnti,
    UInt8  sendSF,
    UInt16 sendSFN,
    UInt8  qIndex,
    UInt16 pduChoice,
    UInt8  cqiMode
    );

extern void getULDataPduNode( UInt8  *pdu_p,
                              UInt16 pduLen,
                              UInt16 crnti,
                              UInt8  sendSF,
                              UInt16 sendSFN,
                              UInt8  qIndex,
                              UInt16 pduChoice,
                              UInt8  cqiMode
                            );

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : prepHeaderInfo
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/
UInt64 prepHeaderInfo(UInt8 **sendBufer_p,UInt8 transportInfo,
        UInt8 controlUlRa, UInt16 crntiOrRarnti,
        UInt32 ttiCounter)
{
    UInt8 *sendBuffer_p = *sendBufer_p;
    *sendBuffer_p = transportInfo;
    sendBuffer_p++;
    *sendBuffer_p = controlUlRa;
    sendBuffer_p++;
    *sendBuffer_p = (crntiOrRarnti & 0x00FF);
    sendBuffer_p++;
    *sendBuffer_p = (crntiOrRarnti & 0xFF00) >> 8;
    sendBuffer_p++;
    *sendBuffer_p = (ttiCounter & 0x00FF);
    sendBuffer_p++;
    *sendBuffer_p = (ttiCounter & 0x0000FF00) >> 8;
    sendBuffer_p++;
    *sendBuffer_p = (ttiCounter & 0x00FF0000) >> 16;
    sendBuffer_p++;
    *sendBuffer_p = (ttiCounter & 0xFF000000) >> 24;
    sendBuffer_p++;

    (*sendBufer_p) = sendBuffer_p;
    return 1;
}

/****************************************************************************
 * Function Name  : distributeRBsAndSendReqToRLC 
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/

MacRetType distributeRBsAndSendReqToRLC( UInt16 ueIndex, 
                                         UInt32 tbsize, 
                                         UInt8 bsrPresentFlag,
                                         void *uldataNode_p,
                                         UInt8 **data_p,
                                         UInt16 *dataLen_p,
                                         UInt8 phrTriggered
#ifdef FDD_CONFIG /** For TTIB_Code */
                                         ,TransmissionAndRLCData **transmissionBuffer_p
#endif
                                         ,UInt8 cellIndex
                                       )/*CLPC_CHG*/
{
    UInt8 lcId[11]         ={0};
    UInt8 lcCounter        = 0;
    UInt8 cnt              = 0;
    UInt8 possible_lc_cnt  = 0;
    UInt8 count            = 0;
    UInt8 i                = 0; /* So that existing logic doesnt change*/
    UInt32 tempTBSize      = tbsize;
    UInt8  adjFlag         = 0;
    UInt8  crntiCEFlag     = 0;
    /** SPR 3657 Changes Start **/
    UInt32 initCount = 0;
    /** SPR 3657 Changes End **/

#ifdef UT_TESTING
    if(tempTBSize >=9500*2)
    {
        tempTBSize=9500*2 -1;
        lteWarning("Resizing UL Data\n");
    }
#endif    
    /* Get the number of LCs for this Ue and collect all Lc Ids*/
    ueContextForUplink *ueContextInfo_p = uplinkContextInfoMap_g[ueIndex];

#ifdef UT_TESTING
    if(PNULL == ueContextInfo_p) 
    {
        return MAC_FAILURE;
    }
#endif    
    // make an entry in ue context for tbsize which will be used later for calculation of padding.    
//    uplinkContextInfoMap_g[ueIndex]->tbSize = tbsize;
    for (cnt = 0; cnt < 11; cnt++)
    {
        if( 0 != ueContextInfo_p->lcInfo[cnt].queueLoad)
        {
            if( TRUE == ueContextInfo_p->isSpsOccasion ) 
            {
                if (TRUE == ueContextInfo_p->lcInfo[cnt].isSpsLc )
                {
                    lcId[i++] = cnt;
                    lcCounter++;
                    break;
                }
            }
            else
            {
                if( TRUE == ueContextInfo_p->lcInfo[cnt].isSpsLc )
                {
                    if( PNULL == ueContextInfo_p->occListNode_p )
                    {
                        lcId[i++] = cnt;
                        lcCounter++;
                    }
                }
                else
                {
                    lcId[i++] = cnt;
                    lcCounter++;
                }
            }
        }
    }
    /* Reset isSpsOccasion for this occasion */
    ueContextInfo_p->isSpsOccasion = FALSE;

    if ( 0 == lcCounter )
    {
        //lteWarning("Q load empty for this UE in MAC Emulator. So Return \n");
#ifdef UE_SIM_ENABLE_PRINTF_LOGS        
        fprintf(stderr,"Q load empty for this UE in MAC Emulator. So Return MAC_PARTIAL_SUCCESS\n");
#endif
        return MAC_PARTIAL_SUCCESS ;
    }

    /* MIN_MAC_PDU_LEN = (3 Bytes* 1-lc) + 1 Byte (Data) */
#ifdef UT_TESTING
    if ( tempTBSize < 4) /* ULA_UTP */
    {
        LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"Error - Grant is less that MIN_MAC_PDU_LEN\n" );
        return MAC_FAILURE;
    }
#endif    
    crntiCEFlag = ueContextInfo_p->crntiCEFlag;

    if(crntiCEFlag)
    {
        if( (tempTBSize - 3) < 4) /* ULA_UTP */
        {
            LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"Error - Assigned Len is not enough for with CRNTI CE case\n");
            return MAC_FAILURE;
        }
        /* updated tbsize   */
        tempTBSize -= 3;  /* 2-CRNTI CE + 1 SduHeader*/
    }

    if(bsrPresentFlag)
    {
        /* Prepare BSR Data after Qload is decremented,
           based upon remaining Qload in each LCG */

        if( (tempTBSize - 4) < 4) /* ULA_UTP */
        {
            LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"Error - Assigned Len is not enough for with BSR case\n");
            return MAC_FAILURE;
        }
        /* updated tbsize   */
        tempTBSize -= 4;  /* 3-BSR + 1 SduHeader*/
    }

    /*CLPC_CHG*/
    if(phrTriggered)
    {
        tempTBSize -= 2;
#ifdef PHR_TESTING
        fprintf(stderr,"--distributDataAndSendReqToRLC--,tbSize=%d\n\n",tempTBSize);
#endif
    }
    if ( tempTBSize >= ( 4 * lcCounter) )
    {
        /* Decrement for all LCs*/
        tempTBSize = ( tempTBSize - ( 3 * lcCounter) );
        possible_lc_cnt = lcCounter ;
    }
    else
    {
        possible_lc_cnt = tempTBSize / 4;
        /* Decrement for possible number of LCs*/
        tempTBSize = ( tempTBSize - ( 3 * possible_lc_cnt) );
        LTE_LOG( LOG_MINOR, &MACModuleLogDetail_g,
                "  Assigned Len is not enough for all LCs, Possible lc [%d]\n", possible_lc_cnt );
        /* Turn adjustment flag ON*/
        adjFlag  = 1;
    }
                
    TransmissionAndRLCData *transmissionAndRLCData_p = PNULL;           
    UInt32 resourceAvailPerLc = 0;
    transmissionAndRLCData_p = (TransmissionAndRLCData*)getMemFromPool
                                (sizeof(TransmissionAndRLCData),PNULL);
                                 
    if(PNULL == transmissionAndRLCData_p) 
    {
        ltePanic("Memory allocation fails for uplink data in MAC Emulator\n");
    }
#ifdef FDD_CONFIG
    if(ueContextInfo_p->ttiBundling)
    {
        memSet(transmissionAndRLCData_p,PNULL,sizeof(TransmissionAndRLCData));
    }
#endif    
#if 0    
    for ( i = 0 ; i < 11; i++ ) 
    {
        transmissionAndRLCData_p->rlcDataBufferArr[i].lcID = -1;
    }
#endif 
    // added by manish
    transmissionAndRLCData_p->dataPtr_p = data_p;
    transmissionAndRLCData_p->dataLen_p = dataLen_p;
    transmissionAndRLCData_p->tbSize    =  tbsize;

    transmissionAndRLCData_p->uldataNode_p = uldataNode_p;
    
#ifdef UE_SIM_ENABLE_PRINTF_LOGS        
    fprintf(stderr, "\n ************distributeRBsAndSendReqToRLC() tbsize [%d]\n",tbsize);
#endif
    resourceAvailPerLc = tempTBSize / possible_lc_cnt;
    i = 0;
    count = possible_lc_cnt;
    
    while ( tempTBSize ) 
    {
        if ( possible_lc_cnt > 1 )
        {
            transmissionAndRLCData_p->rlcDataBufferArr[i].requestedDataSize = resourceAvailPerLc;
            transmissionAndRLCData_p->rlcDataBufferArr[i].lcID = lcId[i];
            
            tempTBSize -= resourceAvailPerLc;
        } 
        else 
        {
            transmissionAndRLCData_p->rlcDataBufferArr[i].requestedDataSize = tempTBSize;
            transmissionAndRLCData_p->rlcDataBufferArr[i].lcID = lcId[i];

            
            tempTBSize -= tempTBSize;
        }
        i++;
        possible_lc_cnt--;
    }  
    transmissionAndRLCData_p->rlcDataBufferArr[i].lcID = -1;
    /** SPR 3657 Changes Start **/
    for(;initCount < count; initCount++)
    {
        queueInit(&(transmissionAndRLCData_p->rlcDataBufferArr[initCount].reTxRlcPduQ));
    }
#ifdef FDD_CONFIG
    /** For TTIB_Code */
    if(ueContextInfo_p->ttiBundling)
    {
        *transmissionBuffer_p = transmissionAndRLCData_p;
        (*transmissionBuffer_p)->lccount = count;
    }
#endif
    /** SPR 3657 Changes End **/
    macUeOpportunityInd(ueIndex,count,(RLCDataBuffer*) transmissionAndRLCData_p,
                                               globalTTITickCount_g, cellIndex
		    );
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : getBSRIdxfrmBytes
 * Inputs         : bytes - Buffer Size value in bytes
 * Outputs        :
 * Returns        : BSR Index
 * Variables      :
 * Description    : Returns the BSR index for a given buffer size according to
 Spec 36.321
 ****************************************************************************/
UInt8 getBSRIdxfrmBytes( UInt32 bytes )
{
    UInt16 bsr_idx = INVALID_BSR_INDEX ;
    /* Returns the BSR Index for a particular buffer size (in bytes)
       as in Spec 36.321 */

    if ( 0 == bytes )
    {
        bsr_idx = 0;
    }
    else if ( bytes <= 10)
    {
        bsr_idx = 1;
    }
    else if ( bytes <= 12)
    {
        bsr_idx = 2;
    }
    else if ( bytes <= 14)
    {
        bsr_idx = 3;
    }
    else if ( bytes <= 17)
    {
        bsr_idx = 4;
    }
    else if ( bytes <= 19)
    {
        bsr_idx = 5;
    }
    else if ( bytes <= 22)
    {
        bsr_idx = 6;
    }
    else if ( bytes <= 26)
    {
        bsr_idx = 7;
    }
    else if ( bytes <= 31)
    {
        bsr_idx = 8;
    }
    else if ( bytes <= 36)
    {
        bsr_idx = 9;
    }
    else if ( bytes <= 42)
    {
        bsr_idx = 10;
    }
    else if ( bytes <= 49 )
    {
        bsr_idx = 11;
    }
    else if ( bytes <= 57 )
    {
        bsr_idx = 12;
    }
    else if ( bytes <= 67)
    {
        bsr_idx = 13;
    }
    else if ( bytes <= 78)
    {
        bsr_idx = 14;
    }
    else if ( bytes <= 91)
    {
        bsr_idx = 15;
    }
    else if ( bytes <= 107)
    {
        bsr_idx = 16;
    }
    else if ( bytes <= 125)
    {
        bsr_idx = 17;
    }
    else if ( bytes <= 146)
    {
        bsr_idx = 18;
    }
    else if ( bytes <= 171)
    {
        bsr_idx = 19;
    }
    else if ( bytes <= 200)
    {
        bsr_idx = 20;
    }
    else if ( bytes <= 234)
    {
        bsr_idx = 21;
    }
    else if ( bytes <= 274)
    {
        bsr_idx = 22;
    }
    else if ( bytes <= 321)
    {
        bsr_idx = 23;
    }
    else if ( bytes <= 376)
    {
        bsr_idx = 24;
    }
    else if ( bytes <= 440)
    {
        bsr_idx = 25;
    }
    else if ( bytes <= 515)
    {
        bsr_idx = 26;
    }
    else if ( bytes <= 603)
    {
        bsr_idx = 27;
    }
    else if ( bytes <= 706)
    {
        bsr_idx = 28;
    }
    else if ( bytes <= 826)
    {
        bsr_idx = 29;
    }
    else if ( bytes <= 967)
    {
        bsr_idx = 30;
    }
    else if ( bytes <= 1132)
    {
        bsr_idx = 31;
    }
    else if ( bytes <= 1326)
    {
        bsr_idx = 32;
    }
    else if ( bytes <= 1552)
    {
        bsr_idx = 33;
    }
    else if ( bytes <= 1817)
    {
        bsr_idx = 34;
    }
    else if ( bytes <= 2127)
    {
        bsr_idx = 35;
    }
    else if ( bytes <= 2490)
    {
        bsr_idx = 36;
    }
    else if ( bytes <= 2915)
    {
        bsr_idx = 37;
    }
    else if ( bytes <= 3413)
    {
        bsr_idx = 38;
    }
    else if ( bytes <= 3995)
    {
        bsr_idx = 39;
    }
    else if ( bytes <= 4677)
    {
        bsr_idx = 40;
    }
    else if ( bytes <= 5476)
    {
        bsr_idx = 41;
    }
    else if ( bytes <= 6411)
    {
        bsr_idx = 42;
    }
    else if (  bytes <= 7505)
    {
        bsr_idx = 43;
    }
    else if ( bytes <= 8787)
    {
        bsr_idx = 44;
    }
    else if ( bytes <= 10287)
    {
        bsr_idx = 45;
    }
    else if ( bytes <= 12043)
    {
        bsr_idx = 46;
    }
    else if ( bytes <= 14099)
    {
        bsr_idx = 47;
    }
    else if ( bytes <= 16507)
    {
        bsr_idx = 48;
    }
    else if ( bytes <= 19325)
    {
        bsr_idx = 49;
    }
    else if ( bytes <= 22624)
    {
        bsr_idx = 50;
    }
    else if ( bytes <= 26487)
    {
        bsr_idx = 51;
    }
    else if ( bytes <= 31009)
    {
        bsr_idx = 52;
    }
    else if ( bytes <= 36304)
    {
        bsr_idx = 53;
    }
    else if ( bytes <= 42502)
    {
        bsr_idx = 54;
    }
    else if ( bytes <= 49759)
    {
        bsr_idx = 55;
    }
    else if ( bytes <= 58255)
    {
        bsr_idx = 56;
    }
    else if ( bytes <= 68201)
    {
        bsr_idx = 57;
    }
    else if ( bytes <= 79846)
    {
        bsr_idx = 58;
    }
    else if ( bytes <= 93479)
    {
        bsr_idx = 59;
    }
    else if ( bytes <= 109439)
    {
        bsr_idx = 60;
    }
    else if ( bytes <= 128125)
    {
        bsr_idx = 61;
    }
    else if ( bytes <= 150000)
    {
        bsr_idx = 62;
    }
    else if ( 150000 < bytes )
    {
        bsr_idx = 63;
    }

    LTE_LOG(LOG_INFO, &MACModuleLogDetail_g,"\nBSR Index [%d] for buffer size [%d]", bsr_idx, bytes);
    return bsr_idx ;
}

/****************************************************************************
 * Function Name  : prepareBSR()
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :UeId and global expire tick is saved in the sQueue.
 *
 ****************************************************************************/
void prepareBSR(UInt8 **sendBufer_p,UInt8 isLongBSR,
        UInt8 bsr0,UInt8 bsr1, UInt8 bsr2, UInt8 bsr3,UInt8 lcId)
{
    UInt8 tempBSR = 0;

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"BSR IDs : %d %d %d %d \n",bsr0,bsr1,bsr2,bsr3);
    UInt8 *sendBuffer_p = *sendBufer_p;
    if(isLongBSR)
    {
        tempBSR |= bsr0 << 2;
        tempBSR |= ((bsr1 >> 4) & 0x03);
        *sendBuffer_p = tempBSR;
        sendBuffer_p++;

        tempBSR = 0;
        tempBSR |= bsr1 << 4;
        tempBSR |= ((bsr2 >> 2) & 0x0F);
        *sendBuffer_p = tempBSR;
        sendBuffer_p++;

        tempBSR = 0;
        tempBSR |= bsr2 << 6;
        tempBSR |= (bsr3 & 0x3F);
        *sendBuffer_p = tempBSR;
        sendBuffer_p++;
    }
    else
    {
        tempBSR |= lcId << 6 ;
        tempBSR |= (bsr0 & 0x3F);
        *sendBuffer_p = tempBSR;
        sendBuffer_p++;

    }
    (*sendBufer_p) = sendBuffer_p;
}

/****************************************************************************
 * Function Name  : prepareSduSubHeader
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/

void prepareSduSubHeader(UInt8** sendBufer_p,UInt8 R1,
        UInt8 R2, UInt8 E, UInt8 lcId,UInt8 format, UInt32 length)
{
    UInt8 sduSubHeader;
    UInt8 formatLen1;
    UInt8 formatLen2;
    UInt8 *sendBuffer_p = *sendBufer_p;
    sduSubHeader = 0;
    formatLen1 = 0;
    formatLen2 = 0;
    sduSubHeader |= (R1 << 7);
    sduSubHeader |= (R2 << 6);
    sduSubHeader |= (E  << 5);
    sduSubHeader |=  lcId & 0x1F;
    *sendBuffer_p = sduSubHeader;
    sendBuffer_p++;
    if( lcId != 30 && lcId != 31 && lcId != 0 && lcId != 27 && lcId != 26 )
    {
        formatLen1 |= format << 7;
        if(!format)
        {
            formatLen1 |= length & 0x7F;
            *sendBuffer_p = formatLen1;
        }
        else
        {
            formatLen1 |= (length>>8) & 0x7F;
            formatLen2 = length & 0xFF;
            *sendBuffer_p = formatLen1;
            sendBuffer_p++;
            *sendBuffer_p = formatLen2;
        }
        sendBuffer_p++;
    }
    (*sendBufer_p) = sendBuffer_p;
}
/****************************************************************************
 * Function Name  : prepareLongBSR 
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/
inline void prepareLongBSR(dynamicUEContext *ueDynContextInfo_p, UInt16 crnti, UInt8 sf, UInt16 sfn, UInt8 qIndex)
{
	UInt8 bsr0,bsr1,bsr2,bsr3;
	UInt8 bsrBuffer[24];
	UInt8 *bsrBuffer_p;
	UInt8 *sendBuffer_p = PNULL;
	UInt8 *start_p      = PNULL;
	//static const UInt8 controlUlRa_s = UL_FLAG;
	bsrBuffer_p = bsrBuffer;
	sendBuffer_p = (UInt8 *) getMemFromPool(40,PNULL);
	if ( sendBuffer_p == PNULL )
	{
		lteWarning("Error!! malloc failed in prepareLongBSR \n");
		return; 
	}
	start_p = sendBuffer_p;
	//prepHeaderInfo(&sendBuffer_p, ULSCH, controlUlRa_s, crnti, (getCurrentglobaltick() + 4));
	/* Form BSR depending upon remaining Qload in each LCG*/

	bsr0 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[0]);
	bsr1 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[1]);
	bsr2 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[2]);
	bsr3 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[3]);
	prepareSduSubHeader(&sendBuffer_p, 0/*R1*/,
			0/*R2*/, 0/*E*/, 30/*Long BSR lcId*/, 0/*F*/, 0/*bsrSize*/);

	prepareBSR(&bsrBuffer_p,1,bsr0,bsr1,bsr2,bsr3,0);
//    fprintf(stderr,"---------bsr0 = %d------------\n", bsr0);
	*sendBuffer_p = bsrBuffer[0];
	sendBuffer_p++;
	*sendBuffer_p = bsrBuffer[1];
	sendBuffer_p++;
	*sendBuffer_p = bsrBuffer[2];
	sendBuffer_p++;
	//prepareUplinkInfo(&sendBuffer_p,1,4/*BSR VALUE*/,bsrBuffer,30);
#ifdef LTE_MAC_UT_LOG
        LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"inside prepareLongBSR crnti = %d\n", crnti);
        LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"sf = %d sfn = %d\n",sf, sfn);
#endif
	//prepareDataPduNode(start_p, sendBuffer_p - start_p, crnti, sf, sfn, qIndex, ULSCH_PDU,0xFF);   
	//getULDataPduNode(start_p, sendBuffer_p - start_p, crnti, sf, sfn, qIndex, ULSCH_PDU,0xFF);  
//    fprintf(stderr,"crnti[%d], sf[%d],sfn[%d],qIndex[%d]",crnti, sf, sfn, qIndex);
    pushEntryInOnlyBsrQueue(start_p, sendBuffer_p - start_p, crnti, sf, sfn, qIndex, ULSCH_PDU,0xFF);
}
/****************************************************************************
 * Function Name  : fapiPrepareLongBSRAndPHR
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/
inline void fapiPrepareLongBSRAndPHR(dynamicUEContext *ueDynContextInfo_p, UInt16 rnti, 
           UInt16 sendSfnSf, UInt8 qIndex, ulschDataQueueNode* ulDataNode_p, 
           UInt32 tbSize, UInt8 isPhrTriggerFlag, UInt8 cellIndex)
{
    UInt8 bsr0,bsr1,bsr2,bsr3;
    UInt8 *sendBuffer_p = PNULL;
    UInt8 *start_p      = PNULL;
    UInt32 paddingLength = tbSize;
    UInt32 bsrLength =4;
    UInt32 startPaddingHeader=0;
    UInt8  phrSize = 0;

    ueContextForUplink *ueContextInfo_p = PNULL;
    if(isPhrTriggerFlag)
    {
        phrSize = 2;
        /* SPR 3156 START */
        ueContextInfo_p = getUEContext(rnti, cellIndex);
        /* SPR 3156 END */
        if(NULL == ueContextInfo_p)
        {
            lteWarning("Ue Context For Uplink is Null\n");
            return MAC_FAILURE;
        }
    }


    if (paddingLength >= (4 + phrSize))
    {
        paddingLength = paddingLength - (bsrLength + phrSize);

        if (paddingLength > 2)
        {
            startPaddingHeader=0;// end Padding
            paddingLength=1; //padding header length
        }
        else
        {
            if ( paddingLength )
            {
                startPaddingHeader=1;
            }
        }     
    }
    else
    {
        // Truncated BSR
        fprintf (stderr, "Only BSR Case, TB Size[%d] is < 4\n", tbSize);
    }


    start_p = ( UInt8 *) getMemFromPool (tbSize, PNULL);

    if ( PNULL == start_p )
    {
        fprintf(stderr, "Not able to get memory for fapiPrepareLongBSRAndPHR\n");
        return; 
    }
    sendBuffer_p = start_p;

    ulDataNode_p->rnti = rnti;
    ulDataNode_p->qIndex = qIndex;
    ulDataNode_p->sendSfnSf = sendSfnSf;
    ulDataNode_p->pduLen = tbSize;
    ulDataNode_p->SF = sendSfnSf & FAPI_SF_MASK;
    ulDataNode_p->SFN = (sendSfnSf & FAPI_SFN_MASK) >> FAPI_SFN_VALUE_SHIFT; 

    bsr0 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[0]);
    bsr1 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[1]);
    bsr2 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[2]);
    bsr3 = getBSRIdxfrmBytes(ueDynContextInfo_p->bsrNet[3]);

    if (paddingLength)
    {
        if (startPaddingHeader)
        {
            UInt8 sduSubHeader = 0;
            if (paddingLength == 1)
            {
                    sduSubHeader   |= (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (1  << 5);
                    sduSubHeader   |=  31;
                    *sendBuffer_p++ = sduSubHeader;
            }
            else
            { // padding Length ==2
                    sduSubHeader   = (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (1  << 5);
                    sduSubHeader   |=  31;
                    *sendBuffer_p++ = sduSubHeader;
                    
                    sduSubHeader   = (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (1  << 5);
                    sduSubHeader   |=  31;
                    
                    *sendBuffer_p++ = sduSubHeader;
               
            }
            sduSubHeader = (0 << 7);
            sduSubHeader |= (0 << 6);
            if (isPhrTriggerFlag)
            {    
                sduSubHeader |= (1  << 5);
            }
            else
            {
                sduSubHeader |= (0  << 5);
            }
            sduSubHeader |=  30 & 0x1F;

            *sendBuffer_p++ = sduSubHeader;

            if (isPhrTriggerFlag)
            {
                sduSubHeader = (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (0  << 5);
                sduSubHeader |=  26 & 0x1F;

                *sendBuffer_p++ = sduSubHeader;
            }
        }
        else
        {
            UInt8 sduSubHeader = 0;
            sduSubHeader = (0 << 7);
            sduSubHeader |= (0 << 6);
            sduSubHeader |= (1  << 5);
            sduSubHeader |=  30 & 0x1F;

            *sendBuffer_p++ = sduSubHeader;
            
            if (isPhrTriggerFlag)
            {
                sduSubHeader = (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (1  << 5);
                sduSubHeader |=  26 & 0x1F;

                *sendBuffer_p++ = sduSubHeader;
            }
            
            sduSubHeader   = (0 << 7);
            sduSubHeader   |= (0 << 6);
            sduSubHeader   |= (0 << 5);
            sduSubHeader   |=  31;
            *sendBuffer_p++ = sduSubHeader;

        }
    }
    else
    {
        UInt8 sduSubHeader = 0;
        sduSubHeader = (0 << 7);
        sduSubHeader |= (0 << 6);
        if (isPhrTriggerFlag)
        {    
            sduSubHeader |= (1  << 5);
        }
        else
        {
            sduSubHeader |= (0  << 5);
        }
        sduSubHeader |=  30 & 0x1F;

        *sendBuffer_p++ = sduSubHeader;
        
        if (isPhrTriggerFlag)
        {
            sduSubHeader = (0 << 7);
            sduSubHeader |= (0 << 6);
            sduSubHeader |= (0  << 5);
            sduSubHeader |=  26 & 0x1F;

            *sendBuffer_p++ = sduSubHeader;
        }
    }

    prepareBSR(&sendBuffer_p,1,bsr0,bsr1,bsr2,bsr3,0);
    
    /*Fill the buffer with PHR Values*/
    if(isPhrTriggerFlag)
	{
		*sendBuffer_p = phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
			[ueContextInfo_p->ueProfileId].phrProfileId].
			phrValues[ueContextInfo_p->phrIndex] & 0x3F;
		ueContextInfo_p->phrIndex = (ueContextInfo_p->phrIndex + 1) %
			phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
			[ueContextInfo_p->ueProfileId].phrProfileId].phrCount;
		sendBuffer_p ++;
	}
#ifdef UE_SIM_ENABLE_PRINTF_LOGS        
    fprintf(stderr,"inside fapiPrepareLongBSRAndPHR crnti = %d\n", rnti);
    fprintf(stderr,"---------bsr0 = %d------------\n", bsr0);
#endif
/* 
    *sendBuffer_p = bsrBuffer[0];
    sendBuffer_p++;
    *sendBuffer_p = bsrBuffer[1];
    sendBuffer_p++;
    *sendBuffer_p = bsrBuffer[2];
    sendBuffer_p++;
    uldataNode_p = (ulschDataQueueNode *) 
                 getMemFromPool(sizeof(ulschDataQueueNode), PNULL);
*/
    ulDataNode_p->pdudata_p = start_p;
    pushNode(&onlyBsrPhrQueue[cellIndex][qIndex], &(ulDataNode_p->ulDataAnchor) );
}
