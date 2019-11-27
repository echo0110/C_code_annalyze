/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2ControllerThread.c,v 1.1.1.1.6.2.6.2.2.1 2010/09/21 15:53:29 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file defines the functionality of  controller 
 *                     thread. It keeps working on containers which gets 
 *                     changed at every millisecond. It iterates through the
 *                     CQI Queue and contol_info Queue and do the processing.
 *                     This file also provides configuration APIs to RRC 
 *                     for MAC Uplink.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2ControllerThread.c,v $
 * Revision 1.1.1.1.6.2.6.2.2.1  2010/09/21 15:53:29  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.10  2009/09/02 13:32:53  gur20439
 * UL DATA flow for NACK case bug fix.
 *
 * Revision 1.9  2009/07/29 15:17:57  gur20439
 * look and feel to simulator fix.
 *
 * Revision 1.8  2009/07/28 15:18:02  gur20439
 * delete ue from rrc support.
 *
 * Revision 1.7  2009/07/24 14:39:58  gur20439
 * support added for non adaptive retransmission for UPLINK.
 *
 * Revision 1.6  2009/07/15 14:32:45  gur20439
 * Timing advance fix.
 *
 * Revision 1.5  2009/07/14 14:49:48  gur20439
 * rrc interface code change.
 *
 * Revision 1.4  2009/07/06 10:02:21  gur20439
 * TA init.
 *
 * Revision 1.3  2009/06/25 11:46:28  gur18569
 * fixed UT bugs
 *
 * Revision 1.2  2009/06/01 10:53:31  gur20439
 * include/interfaces/phySimulatorApiHandler.h
 *
 * Revision 1.1  2009/03/30 10:20:38  gur19836
 * Initial Version
 *
 * Revision 1.41  2009/03/23 09:20:39  gur19140
 * Integration changes first waves
 *
 * Revision 1.40  2009/03/13 08:00:13  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.39  2009/02/25 12:12:14  gur10445
 *  RA resouce list init fix
 *
 * Revision 1.38  2009/02/25 11:02:19  gur10445
 * Removed double allocation of UL RB resources
 *
 * Revision 1.37  2009/02/23 07:42:24  gur10445
 * UL BSR FIX
 *
 * Revision 1.36  2009/02/20 14:14:00  gur18569
 * added createMemPool for data related to uplink
 *
 * Revision 1.35  2009/01/27 20:28:29  gur18569
 * added dispatcherArrInuse flag in dispatcherNode
 *
 * Revision 1.34  2009/01/27 07:21:28  gur20052
 * Incorporated Vandana's Comments
 *
 * Revision 1.33  2009/01/27 06:46:34  gur18569
 * memsetting globals in initMACUplink
 *
 * Revision 1.32  2009/01/25 11:49:39  gur20052
 * Added During Regression Testing
 *
 * Revision 1.31  2009/01/23 16:17:43  gur18569
 * initializing num logical channels
 *
 * Revision 1.30  2009/01/23 15:16:32  gur18569
 * removed unused variables
 *
 * Revision 1.29  2009/01/23 15:15:02  gur18569
 * initializing dynUEContext fields
 *
 * Revision 1.28  2009/01/23 12:53:39  gur18569
 * resetting globals
 *
 * Revision 1.27  2009/01/21 08:47:39  gur18569
 * initializing ulresInfo parameters and dispatcherArrInUse
 *
 * Revision 1.26  2009/01/20 07:46:16  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.25  2009/01/14 17:19:32  gur20052
 * Added Fix for UT Defects, for checking the Scheduler Flag while inserting the RA and NACK information in the container
 *
 * Revision 1.24  2009/01/12 09:43:23  gur19836
 * numLogicalChannels variable added in UL UE Context
 *
 * Revision 1.23  2009/01/07 09:01:36  gur20052
 * UT Defects Fixed
 *
 * Revision 1.22  2009/01/05 12:16:38  gur18569
 * memSet dispatcherArrayNode during container's initialization
 *
 * Revision 1.21  2008/12/31 10:41:17  gur19836
 * Warning messages modified for configUplinkRBs
 *
 * Revision 1.20  2008/12/30 14:18:44  gur11912
 * Phase 2 UT Defect Fixed
 *
 * Revision 1.19  2008/12/30 08:59:05  gur19836
 * Enhancement for mapping crntis to Idx in crntiToIdxTbl_g
 *
 * Revision 1.18  2008/12/29 09:52:49  gur19836
 * UT Defects Fixed
 *
 * Revision 1.17  2008/12/24 09:07:50  gur12905
 * Changed done for initDeMuxManager thread
 *
 * Revision 1.16  2008/12/23 10:50:43  gur19836
 * Changes done for srPeriodicity
 *
 * Revision 1.15  2008/12/23 07:49:00  gur20052
 * Phase 2 review comments incorporated
 *
 * Revision 1.14  2008/12/22 11:53:08  gur19836
 * Updated after incorporating review comments
 *
 * Revision 1.13  2008/12/19 13:21:05  gur20052
 * The pthread sys calls are replaced with wrappers
 *
 * Revision 1.12  2008/12/19 13:18:54  gur20052
 * The pthread sys calls are replaced with wrappers
 *
 * Revision 1.11  2008/12/19 10:32:15  gur19836
 * Review Comments incorporated for Phase 2
 *
 * Revision 1.10  2008/12/19 09:14:31  gur18569
 * added semPost of rbsULConfiguredSem_g after UL RB conf
 *
 * Revision 1.9  2008/12/19 07:55:06  gur12905
 * uplinkContainer changed to UplinkContainer
 *
 * Revision 1.8  2008/12/19 07:24:32  gur12905
 * Function initializeResourceMap called from configAvilableResourceBlock()
 *
 * Revision 1.7  2008/12/11 04:54:31  gur19836
 * Headers updated and logs added
 *
 * Revision 1.6  2008/12/10 10:37:10  gur12905
 * Updated for transmitter thread creation
 *
 * Revision 1.5  2008/12/10 10:19:57  gur12905
 * Updated file after adding some error checking and Logs
 *
 * Revision 1.4  2008/12/09 11:49:06  gur19836
 * Updated during linking
 *
 * Revision 1.3  2008/12/08 12:59:39  gur12905
 * Updated
 *
 * Revision 1.2  2008/12/08 12:42:46  gur12905
 * INITial version
 *
 * Revision 1.1  2008/12/08 12:35:11  gur12905
 * INITIAL VERSION
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2UplinkUEContext.h"
#include "lteLayer2UplinkController.h"
#include "lteLayer2ParseUtil.h"
#include "lteLayer2TDDMgr.h"
#ifdef FDD_CONFIG
#include "phySimulatorApiHandler.h"
#endif

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
/* SPR 609 changes start */
#ifndef NA 
/* SPR 609 changes end */
 #define NA            255
/* SPR 609 changes start */
#endif 
/* SPR 609 changes end */
#ifdef FDD_CONFIG
#define TTI_BUNDLE_SIZE 4
#define TTI_BUNDLE_RTT  16
#endif
/* static UE Context Array which contains pointers to the UE contexts 
   configured */
ueContextForUplink* uplinkContextInfoMap_g[MAX_UE_SUPPORTED];

/* Dynamic UE Context Array which contains pointers to the UE contexts 
   configured */
dynamicUEContext *dynUEContextInfoMap_g[MAX_UE_SUPPORTED];

extern UInt32 cqiPmi_dynamic_RI_1[PMI_ARRAY_SIZE];
extern UInt32 cqiPmi_dynamic_RI_2[PMI_ARRAY_SIZE/2];
/* ULA_CHG */
extern UInt8 lastUeProfileId_g;
/* +SPR_10708 Changes */
extern UInt8 ueProfileNum[MAX_NUM_CELLS];
/* -SPR_10708 Changes */
extern ueProfile ueProfileInfo_g[MAX_NUM_CELLS][MAX_UE_PROFILES];
extern SRSProfile srsProfileInfo_g[MAX_SRS_PROFILES];
/* ULA_CHG */
/*CLPC_CHG*/
extern PHRProfile phrProfileInfo_g[MAX_NUM_CELLS][MAX_PHR_PROFILES];
extern UInt8 tb1_ack_nack_value_array[MAX_NUM_CELLS]
[ACK_NACK_ARRAY_SIZE][ACK_NACK_ARRAY_SIZE];
extern UInt8 tb2_ack_nack_value_array[MAX_NUM_CELLS]
[ACK_NACK_ARRAY_SIZE][ACK_NACK_ARRAY_SIZE];
static inline MacRetType reconfigureTpcPdcchConfigInfoForPusch(
        UInt8 release,
        UInt16 rnti,
        UInt8 tpcBitMapIndex,
        UInt8 dciFormat,
        ueContextForUplink *ueContext_p);
extern TpcInfo tpcRntiInfoMap[TPC_RNTI_COUNT];


/* HO RESET Changes */
extern hiRntiMap hi_pdu_crnti_array_g[MAC_MAX_RESOURCE_BLOCKS][MAX_HARQ_PROCESS_NB];
/* SPR 11445 Changes Start */
extern SpsUeSimTTIContext spsSimTTIMap_g[MAX_NUM_CELLS][MAX_SPS_INTERVAL];
/* SPR 11445 Changes End */
extern UInt16 spsIntervalMap_g[SPS_INTERVAL_MAX];
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
typedef struct NumUlHarqProcessT {
    UInt8 numNormalHarq;
    UInt8 numSFbundlingHarq;
} NumULHarqProcess;
/*temp added */
/* CA TDD Changes Start */
extern UInt8 tddUlDlConfig_g[MAX_NUM_CELLS];
/* CA TDD Changes End */
static const NumULHarqProcess NumULHarqProcesses_sc[7] = {
    {7, 3},     // 0
    {4, 2},     // 1
    {2, 0xFF},  // 2
    {3, 0xFF},  // 3
    {2, 0xFF},  // 4
    {1, 0xFF},  // 5
    {6, 3}      // 6
};

/* DaiIndexForSource_gc: See daiTrackingRegister in ueContextForUplink */
UInt8 DaiIndexForSource_gc[7][MAX_SUBFRAME] =
{
    {4, 7, NA, NA, NA, 9, 2, NA, NA, NA},   // UL/DL config 0
    {7, 7, NA, NA, 8, 2, 2, NA, NA, 3},     // UL/DL config 1
    {7, 7, NA, 7, 2, 2, 2, NA, 2, 7},       // UL/DL config 2
    {4, 2, NA, NA, NA, 2, 2, 3, 3, 4},      // UL/DL config 3
    {2, 2, NA, NA, 2, 2, 3, 3, 3, 3},       // UL/DL config 4
    {2, 2, NA, 2, 2, 2, 2, 2, 2, 2},        // UL/DL config 5
    {7, 8, NA, NA, NA, 2, 3, NA, NA, 4}     // UL/DL config 6
};

/* daiTrackIndexForSource_gp: contains indices that point to
 * daiTrackingRegister instances where DL data can be received.  DAI
 * tracking info instances indexed by these contain DAI counters that are
 * incremented when DL data is received. Multiple indices may point to the
 * same daiTrackingRegister instance if their (N)acks are to be
 * bundled/multiplexed together as specified in 3GPP TS 36.213 section 10.1.
 * These instances also store the DAI value received during reception of
 * the last non-zero DCI format info, thus enabling the Emulator to check
 * for errors in transmission */
UInt8 *daiTrackIndexForSource_gp = PNULL;

#ifdef TDD_CONFIG
/* +    SPR 11297 changes start */
const TddRachConfigInfo TddRachConfigInfo_gc[58] = 
{
/*      0       */      {0,     1},
/*      1       */      {0,     1},
/*      2       */      {0,     1},
/*      3       */      {0, 1},
/*      4       */      {0, 1},
/*      5       */      {0, 1},
/*      6       */      {0, 2},
/*      7       */      {0, 2},
/*      8       */      {0, 2},
/*      9       */      {0, 3},
/*      10      */      {0, 3},
/*      11      */      {0, 3},
/*      12      */      {0, 4},
/*      13      */      {0, 4},
/*      14      */      {0, 4},
/*      15      */      {0, 5},
/*      16      */      {0, 5},
/*      17      */      {0, 5},
/*      18      */      {0, 6},
/*      19      */      {0, 6},
/*      20      */      {1,     1},
/*      21      */      {1,     1},
/*      22      */      {1,     1},
/*      23      */      {1, 1},
/*      24      */      {1, 1},
/*      25      */      {1, 2},
/*      26      */      {1, 3},
/*      27      */      {1, 4},
/*      28      */      {1, 5},
/*      29      */      {1, 6},
/*      30      */      {2,     1},
/*      31      */      {2,     1},
/*      32      */      {2,     1},
/*      33      */      {2, 1},
/*      34      */      {2, 1},
/*      35      */      {2, 2},
/*      36      */      {2, 3},
/*      37      */      {2, 4},
/*      38      */      {2, 5},
/*      39      */      {2, 6},
/*      40      */      {3,     1},
/*      41      */      {3,     1},
/*      42      */      {3,     1},
/*      43      */      {3, 1},
/*      44      */      {3, 1},
/*      45      */      {3, 2},
/*      46      */      {3, 3},
/*      47      */      {3, 4},
/*      48      */      {4,     1},
/*      49      */      {4,     1},
/*      50      */      {4,     1},
/*      51      */      {4, 1},
/*      52      */      {4, 1},
/*      53      */      {4, 2},
/*      54      */      {4, 3},
/*      55      */      {4, 4},
/*      56      */      {4, 5},
/*      57      */      {4, 6},
};
/* -    SPR 11297 changes end */

#endif

UInt8 ulImplicitReleaseAfterMap[INVALID_IMPLICIT_RELEASE] = {2 /* 0 */,
                                                             3 /* 1 */,
                                                             4 /* 2 */,
                                                             8 /* 3 */};

/*****************************************************************************
 * Function Name  : validateCodingRate 
 * Inputs         : modScheme - Modulation Scheme configured for UE
 *                  codingRate - Coding rate configured for UE
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Variables      : 
 * Description    : This API validates the codingrate for modulationScheme.
 *****************************************************************************/

MacRetType validateCodingRate(UInt16 modScheme, UInt32 codingRate)
{
    UInt8 ret = MAC_FAILURE;
    codingRate = codingRate / CODING_MULTIPLIER;

    switch(modScheme)
    {
        case QPSK:
            /* If modulation Scheme is QPSK, codingrate should be one of 
               these */
            if((codingRate == CODING_RATE_120) ||
               (codingRate == CODING_RATE_157) ||
               (codingRate == CODING_RATE_193) ||
               (codingRate == CODING_RATE_251) ||
               (codingRate == CODING_RATE_308) ||
               (codingRate == CODING_RATE_379) ||
               (codingRate == CODING_RATE_449) ||
               (codingRate == CODING_RATE_526) ||
               (codingRate == CODING_RATE_602) ||
               (codingRate == CODING_RATE_679))
            {
                ret = MAC_SUCCESS;
            }    
            break;

        case SIXTEENQAM:
            /* If modulation Scheme is 16QAM, codingrate should be one of 
               these */
            if((codingRate == CODING_RATE_340) ||
               (codingRate == CODING_RATE_378) ||
               (codingRate == CODING_RATE_434) ||
               (codingRate == CODING_RATE_490) ||
               (codingRate == CODING_RATE_553) ||
               (codingRate == CODING_RATE_616) ||
               (codingRate == CODING_RATE_658))
            {
                ret = MAC_SUCCESS;
            }    

            break;

        case SIXFOURQAM:
            /* If modulation Scheme is 64QAM, codingrate should be one of 
               these */
            if((codingRate == CODING_RATE_438) ||
                    (codingRate == CODING_RATE_466) ||
                    (codingRate == CODING_RATE_517) ||
                    (codingRate == CODING_RATE_567) ||
                    (codingRate == CODING_RATE_616) ||
                    (codingRate == CODING_RATE_666) ||
                    (codingRate == CODING_RATE_719) ||
                    (codingRate == CODING_RATE_772) ||
                    (codingRate == CODING_RATE_822) ||
                    (codingRate == CODING_RATE_873) ||
                    (codingRate == CODING_RATE_910) ||
                    (codingRate == CODING_RATE_948))
            {
                ret = MAC_SUCCESS;
            }    
            break;

        default:
            lteWarning("Invalid modulationScheme\n");
            break;
    }        

    return ret;
}

/*****************************************************************************
 * Function Name  : createMACULUEEntity 
 * Inputs         : idx - User Equipment Index
 *                  lchCount - Number of Logical Channels configured for UE
 *                  ueContextInfo_p - Pointer to UE related UL Info
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Variables      : 
 * Description    : This function creates a UE Context for Uplink at MAC Layer
 *****************************************************************************/
MacRetType createMACULUEEntity(UInt16 idx, UInt8 lchCount, 
        ueContextForUplink *ueContextInfo_p, UInt8 cellIndex)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);
    UInt8 n = 0;
    UInt16 resp = MAC_SUCCESS;
    /* + CL-MIMO RSM*/
    UInt8 cbsrIndex = 0;
    UInt8 cbsrInit = 0;
    UInt8 i = 0;
    /* - CL-MIMO RSM*/

    /*CLPC_CHG*/
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentglobaltick();
    /* SPR 15909 fix end */
    UInt8 row = 0;
    UInt8 column = 0;
    /*CLPC_CHG*/
    
    /* SPR 3719 changes Start */ 
    UInt8 counter = 0; 
    /* SPR 3719 changes End */ 
    /* EICIC +*/
    UInt8 	absLoop = 0;
    UInt8 numBitsCsiPattern;
    /* EICIC -*/

    ueContextForUplink *ulUEContext_p = PNULL;
    dynamicUEContext *dynUEContext_p = PNULL;

    /* Allocate memory for UL Static UE Context */
    ulUEContext_p = 
        (ueContextForUplink *)getMemFromPool(sizeof(ueContextForUplink), PNULL);
    if(PNULL == ulUEContext_p) {
        ltePanic("getMemFromPool() failed for ulUEContext creation\n");
        return MAC_INTERNAL_ERROR;
    }

    /* SPR 3719 changes Start */ 
    for(counter = 0; counter < QSIZE; counter++) 
    {
        ulUEContext_p->crcIndRefNode_p[counter]=PNULL;
    }

    ulUEContext_p->shiftInd = ulUEContext_p->crcIndex = 0;

    /* SPR 3719 changes End */ 

    /* Allocate memory for UL Dynamic UE Context */
    dynUEContext_p = 
        (dynamicUEContext *)getMemFromPool(sizeof(dynamicUEContext), PNULL);
    if(PNULL == dynUEContext_p) {
        freeMemPool(ulUEContext_p);
        ulUEContext_p = PNULL;
        ltePanic("getMemFromPool() failed for dynUEContext creation\n");
        return MAC_INTERNAL_ERROR;
    }

    /* Static UE Context initialization */
    ulUEContext_p->crnti = ueContextInfo_p->crnti;

    /* 4x2 MIMO S */
    ulUEContext_p->ueCategory = ueContextInfo_p->ueCategory;
/* 4X4 DL MIMO CHG START */
    ulUEContext_p->ueCategory = 5;
/* 4X4 DL MIMO CHG END */
    fprintf(stderr,"###### UE UL Category [%d] #######\n",ulUEContext_p->ueCategory);
    /* 4x2 MIMO E */
    ulUEContext_p->srTriggered = FALSE;
    ulUEContext_p->availGrantCounter = 0;
    ulUEContext_p->queueLoad = 0;

    /*SPR 2137 changes Start*/
    ulUEContext_p->phrFlag = FALSE;
    ulUEContext_p->phrIndex = 0;
    ulUEContext_p->bsrPresentFlag = FALSE;
    /*SPR 2137 changes End*/
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    ulUEContext_p->isHDFddFlag = ueContextInfo_p->isHDFddFlag;
    if(TRUE == ulUEContext_p->isHDFddFlag)
    {
        ueContextInfo_p->spsUplinkInfo.requestType = SPS_RELEASE;
    }
#endif
    /* HD FDD Changes End */
    /*CLPC_CHG*/
    memcpy(&(ulUEContext_p->dedicatedULPowerControlInfo),&(ueContextInfo_p->dedicatedULPowerControlInfo),
            sizeof(ueContextInfo_p->dedicatedULPowerControlInfo));

   if( !ueContextInfo_p->tpcPdcchConfigPusch.release )
   {
       if( ueContextInfo_p->dedicatedULPowerControlInfo.accumulationEnabled == TRUE)
       {

           memcpy(&(ulUEContext_p->tpcPdcchConfigPusch),&(ueContextInfo_p->tpcPdcchConfigPusch),
                   sizeof(ueContextInfo_p->tpcPdcchConfigPusch));
       }
       else
       {
           fprintf(stderr,"Cannot Configure tpcPdcchConfigPusch if \
                   Accumulation Enabled is False\n");
           return MAC_FAILURE;
       }
   }
    /*CLPC_CHG*/
    for(n = 0; n <MAX_NUMBER_OF_LOGICAL_CHANNEL; n++)
    {
        ulUEContext_p->lcInfo[n].lchId = 0;
        /*Rohit: Fix for addign UL Logical channel, there is validation check for lcgId
          while adding logical channel */        
        //ulUEContext_p->lcInfo[n].lcGId = 0;
        ulUEContext_p->lcInfo[n].lcGId = LCGID_INVALID_VALUE;
        /*Rohit: Till here */        
        ulUEContext_p->lcInfo[n].queueLoad = 0;
        ulUEContext_p->lcInfo[n].isSpsLc = FALSE;
    }

#ifdef TDD_CONFIG
    /* SPR 5067 Fix Start */
    memset(ulUEContext_p->harqNode_p, 0, sizeof(intptr_t)*20);
    /* SPR 5067 Fix End */
    ULHARQEntity *ulHarqEntity_p = PNULL;
    //UInt8 i = 0;
    ulHarqEntity_p = ulUEContext_p->ulHarqEntity_p
        = (ULHARQEntity *)getMemFromPool(sizeof(ULHARQEntity), PNULL);

    ulHarqEntity_p->nackQueueLoad = 0;

    for (i = 0; i < MAX_SUBFRAME; i++)
    {
        ulHarqEntity_p->hiReceived[i] = 0;
        ulHarqEntity_p->harqId[i] = 0;
    }

    /* CA TDD Changes Start */
    ulHarqEntity_p->numHARQProcess = NumULHarqProcesses_sc[tddUlDlConfig_g[cellIndex]].numNormalHarq;
    /* CA TDD Changes End */
    for(n = 0; n <TDD_MAX_UL_HARQ_PROCESS; n++)
    {
        ulHarqEntity_p->ulHarqProcess[n].harqProcessId = n;
        ulHarqEntity_p->ulHarqProcess[n].harqStatus = HARQ_PROCESS_FREE;
        ulHarqEntity_p->ulHarqProcess[n].ulgrantrecvCount = 0;
        ulHarqEntity_p->ulHarqProcess[n].currentRetransCount = 0;
        ulHarqEntity_p->ulHarqProcess[n].assignedResourcesNB = 0;
        ulHarqEntity_p->ulHarqProcess[n].transmissionBuffer_p = PNULL;
        ulHarqEntity_p->ulHarqProcess[n].totalTBSize = 0;
        ulHarqEntity_p->ulHarqProcess[n].timer_p = PNULL;
    }
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    //Code Removed
    ulUEContext_p->ackNackMode = ueContextInfo_p->dlAckNackMode;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */

#elif FDD_CONFIG

    for(n = 0; n <MAX_HARQ_PROCESS_NB; n++)
    {
        dynUEContext_p->ulresInfo[n].riStart = 0; /*indicates the starting index of the resorces*/
        dynUEContext_p->ulresInfo[n].riLength = 0;    /*indicates number of contigiously allocated RBs*/
        dynUEContext_p->ulresInfo[n].ackNack = 0;    /*indicates ACK or NACK (0 for NACK /1 for ACK) */
        dynUEContext_p->ulresInfo[n].currentRetransCount = 0;/*indicates the retrans count for current TTI/HARQ*/
        dynUEContext_p->ulresInfo[n].deltabsrLoad = 0; /*UL_BSR_FIX*/

        ulUEContext_p->ulharqInfo[n].riStart = 0;
        ulUEContext_p->ulharqInfo[n].riLength = 0;
        ulUEContext_p->ulharqInfo[n].ackNack = 0;
        ulUEContext_p->ulharqInfo[n].currentRetransCount = 0;/*indicates the retrans count for current TTI/HARQ*/
        ulUEContext_p->ulharqInfo[n].deltabsrLoad = 0; /*UL_BSR_FIX*/
        ulUEContext_p->ulharqInfo[n].tbSizeForNackCase = 0; 
    }
#endif
    for(n = 0; n <NUM_OF_LCG; n++)
    {
        dynUEContext_p->bsrNet[n] = 0;
        dynUEContext_p->lcCount[n] =0;
    }
    dynUEContext_p->cqiInd =0; 
    dynUEContext_p->inUseFlag =0;
    dynUEContext_p->pendingToDeleteFlag =0;
    dynUEContext_p->lastScheduledTick =0;
    dynUEContext_p->bsrNetTotal =0;
    /* Configure Logical channels if lchCount > 0 */
    ulUEContext_p->numLogicalChannels = 0;
    if(lchCount) 
    {
        if (MAC_FAILURE == addULUELogicalChannel(ulUEContext_p, lchCount,
                    ueContextInfo_p->lcInfo, dynUEContext_p))
        {
            resp = MAC_LOG_CH_ERROR;
            ltePanic("LC creation bug\n");
        }
    }
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"The number of Logical Channels provisioned for UL - %d \n",
            ulUEContext_p->numLogicalChannels);

    ulUEContext_p->ueIdx = idx;
    ulUEContext_p->uePriority = ueContextInfo_p->uePriority;

    /* max harq tx changes */
    //ulUEContext_p->maxRB = ueContextInfo_p->maxRB;
    ulUEContext_p->maxUlHarqTx = ueContextInfo_p->maxUlHarqTx;
    /* max harq tx changes */

    ulUEContext_p->codingrate = ueContextInfo_p->codingrate;


    ulUEContext_p->uePriority = 0;
    ulUEContext_p->modulationSchemeFactor = 0;
    dynUEContext_p->lastScheduledTick = globalTTITickCount_g;

    dynUEContext_p->bsrNet[0] = 0;
    dynUEContext_p->bsrNet[1] = 0;
    dynUEContext_p->bsrNet[2] = 0;
    dynUEContext_p->bsrNet[3] = 0;
    ulUEContext_p->taToBeSentFlag = 0;
    ulUEContext_p->timingAdvance = 0xFF;
    ulUEContext_p->tbSize = 0;

    for (n = 0; n < 10; n++ )
    {
        ulUEContext_p->ulControlNode_p[n] = PNULL; 
        ulUEContext_p->ulDataNode_p[n] = PNULL;
    }
    /* SPS_CHG */
    /* Initialize SPS variables in UL UE Context */
    ulUEContext_p->isSpsOccasion = FALSE;
    ulUEContext_p->occListNode_p = PNULL;
    ulUEContext_p->occListNode20_p = PNULL;
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    ulUEContext_p->occListNode40_p = PNULL;
    ulUEContext_p->occListNode60_p = PNULL;
#endif
    memset(&ulUEContext_p->spsOccCtx,0,sizeof(ulUEContext_p->spsOccCtx));
    /* - SPS_TDD_Changes */
    ulUEContext_p->ttiIndex = MAX_SPS_INTERVAL;
    ulUEContext_p->ulEmptyTx = 0;
    ulUEContext_p->spsUplinkInfo.requestType = SPS_RELEASE;
    
    /* 128ue_tdd_support */
    ulUEContext_p->ackNackIndex = 0;
    ulUEContext_p->sysNum = 0;
    /* + SPR 14341 Changes */
    ulUEContext_p->scellAckNackIndex = 0;
    ulUEContext_p->scellSysNum = 0;
    /* - SPR 14341 Changes */
#ifdef FDD_CONFIG
    ulUEContext_p->rowCounter = 0;
    ulUEContext_p->columnCounter = 0;
#endif
    /*HARQ NACK fix*/
    ulUEContext_p->rowSCellCounter= 0;
    ulUEContext_p->columnSCellCounter = 0;
    
    if (ueContextInfo_p->isSpsUlInfoPresent)
    {
        ulUEContext_p->spsUplinkInfo.requestType = 
                  ueContextInfo_p->spsUplinkInfo.requestType;
        if (SPS_SETUP == ueContextInfo_p->spsUplinkInfo.requestType)
        {
            ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.semiPersistentSchedIntervalUL = 
                ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.semiPersistentSchedIntervalUL; 
            ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.implicitReleaseAfter = 
                ulImplicitReleaseAfterMap[ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.implicitReleaseAfter];
#if 0
            ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent = 
                ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent;
            if (ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent)
            {
                ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0NominalPuschPersistent = 
                    ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0NominalPuschPersistent;
                ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0UePuschPersistent = 
                    ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0UePuschPersistent;
            }
            else
            {
                ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0NominalPuschPersistent = 0;
                ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0UePuschPersistent = 0;
            }
#endif
            /* + SPS_TDD_Changes */
            ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent = 
                ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent;
            if ( ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent)
            {
                ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.twoIntervalsConfig = 
                    ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.twoIntervalsConfig;
            }
            else
            {
                ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.twoIntervalsConfig = FALSE; 
            }
            /* - SPS_TDD_Changes */
        }
        else
        {
            ulUEContext_p->spsUplinkInfo.spsUplinkSetupInfo.semiPersistentSchedIntervalUL = SPS_INTERVAL_MAX;
        }
    }
    /* SPS_CHG */

#ifdef TDD_CONFIG
    // Fill SRI schedule info
    ulUEContext_p->lastSRISchedule.sfn = 0;
    ulUEContext_p->lastSRISchedule.sf = 0;
    ulUEContext_p->lastSRISchedule.isValid = FALSE;

    // ulUEContext_p->srconfigflag = ueContextInfo_p->srconfigflag;
    ulUEContext_p->srconfigflag = 1; //ueContextInfo_p->srconfigflag;
    ulUEContext_p->srSetupflag = 1;//ueContextInfo_p->srSetupflag;
    ulUEContext_p->srPucchResourceIndex = ueContextInfo_p->srPucchResourceIndex;
    ulUEContext_p->srConfigurationIndex = ueContextInfo_p->srConfigurationIndex;
    ulUEContext_p->srConfigurationIndex = 2;
    ulUEContext_p->srPeriodicity = ueContextInfo_p->srPeriodicity;
    ulUEContext_p->srSubframeOffset = ueContextInfo_p->srSubframeOffset;

    // Calculate the first SRI opportunity to be sent by the UE according
    // to the current SFN.SF
    //Vikrant help needed :)  
    calcFirstSRILocation(ulUEContext_p, NULL, NULL, cellIndex);
    memset(ulUEContext_p->daiTrackingRegister,
            0, sizeof(ulUEContext_p->daiTrackingRegister));
#endif

    ulUEContext_p->srPduNode_p = PNULL;
    /* + CL-MIMO RSM*/
    ulUEContext_p->codeBookSubsetfromRRC.cbsrType = 
        ueContextInfo_p->codeBookSubsetfromRRC.cbsrType;
    for (cbsrIndex = 0; cbsrIndex < MAX_CBSR_SIZE; cbsrIndex++)
    {
        ulUEContext_p->codeBookSubsetfromRRC.cbsrValue[cbsrIndex] = 
            ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[cbsrIndex];
    }  
    /* + LJA Hardcoding the cbsrType and cbsrValue till interface is ready*/
    /*ulUEContext_p->codeBookSubsetfromRRC.cbsrType = 2;
      ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0] = 0xFF;*/
    /* - LJA Hardcoding the cbsrType and cbsrValue till interface is ready*/

    if(0x04 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
    {
        cqiPmi_dynamic_RI_1[0] = 0x30;
        cbsrInit = 1;
    }
    if(0x08 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
    {
        cqiPmi_dynamic_RI_1[1] = 0x34;
        cbsrInit = 2;
    }
    if(0x10 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
    {
        cqiPmi_dynamic_RI_1[2] = 0x38;
        cbsrInit = 3;
    }
    if(0x20 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
    {
        cqiPmi_dynamic_RI_1[3] = 0x3c;
        cbsrInit = 4;
    }
    for(i = 0; i < PMI_ARRAY_SIZE; i++)
    {
        if(0 == cqiPmi_dynamic_RI_1[i])
        {
            switch (cbsrInit)
            {
                case 1:cqiPmi_dynamic_RI_1[i] =  0x10;
                       break;
                case 2:cqiPmi_dynamic_RI_1[i] =  0x14;
                       break;
                case 3:cqiPmi_dynamic_RI_1[i] =  0x18;
                       break;
                case 4:cqiPmi_dynamic_RI_1[i] =  0x1c;
                       break;
                default:
                       cqiPmi_dynamic_RI_1[i] = 0x10;
            }
        }
    }

   if(0x04 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
   {
       cqiPmi_dynamic_RI_1[0] = 0x30;
       cbsrInit = 1;
   }
   if(0x08 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
   {
       cqiPmi_dynamic_RI_1[1] = 0x34;
       cbsrInit = 2;
   }
   if(0x10 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
   {
       cqiPmi_dynamic_RI_1[2] = 0x38;
       cbsrInit = 3;
   }
   if(0x20 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
   {
       cqiPmi_dynamic_RI_1[3] = 0x3c;
       cbsrInit = 4;
   }
   for(i = 0; i < PMI_ARRAY_SIZE; i++)
   {
       if(0 == cqiPmi_dynamic_RI_1[i])
       {
           switch (cbsrInit)
           {
               case 1:cqiPmi_dynamic_RI_1[i] =  0x10;
                      break;
               case 2:cqiPmi_dynamic_RI_1[i] =  0x14;
                      break;
               case 3:cqiPmi_dynamic_RI_1[i] =  0x18;
                      break;
               case 4:cqiPmi_dynamic_RI_1[i] =  0x1c;
                      break;
               default:
                      cqiPmi_dynamic_RI_1[i] = 0x10;
           }
       }
   }

   cbsrInit = 0;
   if(0x40 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
   {
       cqiPmi_dynamic_RI_2[0] = 0xf0;
       cbsrInit = 1;
   }
   if(0x80 & ueContextInfo_p->codeBookSubsetfromRRC.cbsrValue[0])
   {
       cqiPmi_dynamic_RI_2[1] = 0xf1;
       cbsrInit = 2;
   }

   for(i = 0; i < PMI_ARRAY_SIZE/2; i++)
   {
       if(0 == cqiPmi_dynamic_RI_2[i])
       {
           switch (cbsrInit)
           {
               case 1:cqiPmi_dynamic_RI_2[i] =  0xF0;
                      break;
               case 2:cqiPmi_dynamic_RI_2[i] =  0xF1;
                      break;
               default:
                      cqiPmi_dynamic_RI_2[i] = 0xF0;
           }
       }
   }

   /* - CL-MIMO RSM*/

   /* ULA_CHG */
   /* +SPR_10708 Changes */
   ulUEContext_p->ueProfileId = lastUeProfileId_g++ % ueProfileNum[cellIndex]; 
   /* -SPR_10708 Changes */
   //ulUEContext_p->ueProfileId = lastUeProfileId_g++ % MAX_UE_PROFILES; 
   ulUEContext_p->srsSeqUsedIdx = 0;
   ulUEContext_p->startPosition = srsProfileInfo_g[ueProfileInfo_g[cellIndex][ulUEContext_p
       ->ueProfileId].srsProfileId].startPosition;
   ulUEContext_p->enableHopping = srsProfileInfo_g[ueProfileInfo_g[cellIndex][ulUEContext_p
       ->ueProfileId].srsProfileId].hoppingEnable;
   ulUEContext_p->ulschSinrSeqUsedIdx = 0;
   /* CLPC_CHG */
   ulUEContext_p->cqiSinrSeqUsedIdx = 0;
   /* CLPC_CHG */
   /* ULA_CHG */
    /*CLPC_CHG*/
    if(ueProfileInfo_g[cellIndex][ulUEContext_p->ueProfileId].phrProfileId == PHR_INACTIVE)
    {
        ulUEContext_p->phrFlag = FALSE;
        ulUEContext_p->phrTime = currentTick - 1; 
    }
    else
    {
        ulUEContext_p->phrFlag = TRUE;
        ulUEContext_p->phrTime = currentTick + 
            phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
			[ulUEContext_p->ueProfileId].phrProfileId].phrTimer ;
    }
#ifdef PHR_TESTING
    fprintf(stderr,"createMACULUEEntity:phrProfileId=%d,phrFlag=%d\n",
        ueProfileInfo_g[cellIndex][ulUEContext_p->ueProfileId].phrProfileId,
            ulUEContext_p->phrFlag);
    /* SPR 15909 fix start */
    fprintf(stderr,"createMACULUEEntity:currentTick=%llu,phrTime=%llu\n",currentTick,ulUEContext_p->phrTime);
    /* SPR 15909 fix end */
#endif

    /* reset changes */
    ulUEContext_p->resetFlag = 0;

#ifdef FDD_CONFIG
    /** For TTIB_Code */
     ulUEContext_p->ttiBundling = ueContextInfo_p->ttiBundling;
     ulUEContext_p->ttibInfo.bundleStartIndex = NA; //INVALID
#endif

	/* EICIC +*/

#ifdef FDD_CONFIG
       numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif
    for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
    {	
		ulUEContext_p->csiMeasurementSubset1[absLoop] = 0;
		ulUEContext_p->csiMeasurementSubset2[absLoop] = 0;
	}

 	if(ueContextInfo_p->cqiConfigRequestType == SETUP)
 	{
        ulUEContext_p->riConfigIndex_2 = ueContextInfo_p->riConfigIndex_2 ;
        ulUEContext_p->cqiPMIConfigIndex_2 =  ueContextInfo_p->cqiPMIConfigIndex_2 ;
 	}

    if (SETUP >= ueContextInfo_p->cqiSubsetConfigRequestType)
	{
		for (absLoop = 0; absLoop < numBitsCsiPattern; absLoop++)
		{	
		    ulUEContext_p->csiMeasurementSubset1[absLoop] = ueContextInfo_p->csiMeasurementSubset1[absLoop];
		    ulUEContext_p->csiMeasurementSubset2[absLoop] = ueContextInfo_p->csiMeasurementSubset2[absLoop];
		}
    }

	/* EICIC -*/
    uplinkContextInfoMap_g[idx] = ulUEContext_p;
    dynUEContextInfoMap_g[idx] = dynUEContext_p;

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit\n", __func__);
    return resp;
}


/*****************************************************************************
 * Function Name  : deleteMACULUEEntity 
 * Inputs         : idx - User Quipment Index
 *                  transactionId - transaction ID received from RRC Layer
 * Outputs        : None
 * Returns        : MAC_SUCCESS, MAC_PENDING_DELETE or MAC_FAILURE
 * Variables      : 
 * Description    : This function deletes UE Context for Uplink at MAC Layer
 *****************************************************************************/

/* SPR 11445 Changes Start */
MacRetType deleteMACULUEEntity(UInt16 idx, UInt16 transactionId, UInt8 cellIndex)
/* SPR 11445 Changes End */
{
	LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"Entry %s function\n", __func__);
	UInt8 ret = MAC_FAILURE;
	ueContextForUplink *ueContextInfo_p = PNULL;
	LP_MacSpsOccList             occListNode_p = PNULL;
	UInt16  ttiIndex               = 0;
	LP_SpsUeSimTTIContext ttiContext_p              = PNULL;
	LP_SpsUeSimTTIContext ttiContextPeriodicity20_p = PNULL;
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    LP_SpsUeSimTTIContext ttiContextPeriodicity40_p = PNULL;
    LP_SpsUeSimTTIContext ttiContextPeriodicity60_p = PNULL;
    /* Offset for two interval config in uplink SPS-TDD */
    SInt8 subFrameOffset = SPS_TWO_INTERVAL_OFFSET_NA;
#endif
    /* - SPS_TDD_Changes */

	if((PNULL == dynUEContextInfoMap_g[idx]) ||
			(PNULL == uplinkContextInfoMap_g[idx]))
	{
		lteWarning("UE does not exist at this idx\n");
		return MAC_FAILURE;
	}
	ueContextInfo_p = uplinkContextInfoMap_g[idx];
	ttiIndex = ueContextInfo_p->ttiIndex;
/* SPR 11445 Changes Start */
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
	ttiContextPeriodicity20_p = &spsSimTTIMap_g[cellIndex][ (ttiIndex + SPS_INTERVAL_20)\
				    % MAX_SPS_INTERVAL];
#elif TDD_CONFIG
    /* If twoIntervalConfig is enabled consider the subFrame offset for corresponding SubFrame*/
    /* 40ms periodicity: offset is considered at ttiIndex+40 */
    /* 20ms periodicity: offset is considered at ttiIndex+20 and ttiIndex+60 */
    if (TRUE == ueContextInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent)
    {
        subFrameOffset = getSubFrameOffsetForSpsTDD((ttiIndex % MAX_SUBFRAME), cellIndex);
    }
    ttiContextPeriodicity20_p = &spsSimTTIMap_g[cellIndex][ (ttiIndex + SPS_INTERVAL_20 +\
            subFrameOffset) % MAX_UL_SPS_INTERVAL];
    if( SPS_INTERVAL_20 == spsIntervalMap_g[ueContextInfo_p->spsUplinkInfo.\
            spsUplinkSetupInfo.semiPersistentSchedIntervalUL] )
    {
        ttiContextPeriodicity40_p = &spsSimTTIMap_g[cellIndex][ 
            (ttiIndex + SPS_INTERVAL_40) % MAX_UL_SPS_INTERVAL];
    }
    else if( SPS_INTERVAL_40 == spsIntervalMap_g[ueContextInfo_p->spsUplinkInfo.\
            spsUplinkSetupInfo.semiPersistentSchedIntervalUL] )
    {
        ttiContextPeriodicity40_p = &spsSimTTIMap_g[cellIndex][ 
            (ttiIndex + SPS_INTERVAL_40 + subFrameOffset) % MAX_UL_SPS_INTERVAL];
    }
    ttiContextPeriodicity60_p = &spsSimTTIMap_g[cellIndex][ (ttiIndex + SPS_INTERVAL_40 +\
            SPS_INTERVAL_20 + subFrameOffset) % MAX_UL_SPS_INTERVAL];
#endif
    /* - SPS_TDD_Changes */
	ttiContext_p = &spsSimTTIMap_g[cellIndex][ttiIndex];
/* SPR 11445 Changes End */
	occListNode_p = ueContextInfo_p->occListNode_p ;
	ueContextInfo_p->ulNdi =0;
	ueContextInfo_p->isSpsOccasion = FALSE;

	if(ttiContext_p->numUlSpsOccasionPerTti)
	{
	        if(PNULL == ueContextInfo_p->occListNode_p)
            {
    /*Start: Fix for SPR 10971, release the memory before return*/ 
	freeMemPool(uplinkContextInfoMap_g[idx]);
	dynUEContextInfoMap_g[idx] = PNULL;
	uplinkContextInfoMap_g[idx] = PNULL;
    /*End: Fix for SPR 10971*/ 

		    return;
            }
		DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsOccasionList,
				occListNode_p );
		(ttiContext_p->numUlSpsOccasionPerTti)--;
		freeMemPool( occListNode_p );
		occListNode_p = PNULL;

        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        occListNode_p = ueContextInfo_p->occListNode40_p ;
        if(PNULL == ueContextInfo_p->occListNode40_p)
            return;
        DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContextPeriodicity40_p->spsOccasionList,
                occListNode_p );
        (ttiContextPeriodicity40_p->numUlSpsOccasionPerTti)--;
        freeMemPool( occListNode_p );
        occListNode_p = PNULL;
#endif
        /* - SPS_TDD_Changes */

		if( SPS_INTERVAL_20 == spsIntervalMap_g[ueContextInfo_p->spsUplinkInfo.\
				spsUplinkSetupInfo.semiPersistentSchedIntervalUL] )
		{
			occListNode_p = ueContextInfo_p->occListNode20_p;
			if(ttiContextPeriodicity20_p->numUlSpsOccasionPerTti)
			{
				DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContextPeriodicity20_p->spsOccasionList, occListNode_p );
				(ttiContextPeriodicity20_p->numUlSpsOccasionPerTti)--;
				freeMemPool( occListNode_p );
				occListNode_p = PNULL;
			}
            /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
            occListNode_p = ueContextInfo_p->occListNode60_p;
            if(ttiContextPeriodicity60_p->numUlSpsOccasionPerTti)
            {
                DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContextPeriodicity60_p->spsOccasionList, 
                        occListNode_p );
                (ttiContextPeriodicity60_p->numUlSpsOccasionPerTti)--;
                freeMemPool( occListNode_p );
                occListNode_p = PNULL;
            }
#endif
            /* - SPS_TDD_Changes */
		}
		ueContextInfo_p->occListNode_p = PNULL;
		ueContextInfo_p->occListNode20_p = PNULL;
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        ueContextInfo_p->occListNode40_p = PNULL;
        ueContextInfo_p->occListNode60_p = PNULL;
#endif
        /* - SPS_TDD_Changes */
	}

	freeMemPool(uplinkContextInfoMap_g[idx]);
	dynUEContextInfoMap_g[idx] = PNULL;
	uplinkContextInfoMap_g[idx] = PNULL;

	LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"Exit %s function\n", __func__);
	return ret;
}


/****************************************************************************
 * Function Name  : reconfigureMACULUEEntity 
 * Inputs         : ueIdx - UE Identifier Index
 *                  maxRB - Maximum number of Resource Blocks that can be
 *                          allocated to this UE per TTI.
 *                  logicalChCount - Number of logical channels being currently
 *                                   configured for this UE.
 *                  lcInfo_p - A pointer to an array for different Logical
 *                             Channel Configuration data.
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      :
 * Description    : This API is used to fetch the UE Context pointer from the
 *                  Index Table and then reconfigure the various UE Context
 *                  parameters for the requested UE.
 ****************************************************************************/

MacRetType reconfigureMACULUEEntity (UInt16 ueIdx,
                                     ueContextForUplink *uplkUEInfo_p,
                                     UInt32 logicalChCount,
                                     ulLogicalChannelInfo *lcInfo_p, 
                                     UInt8 cellIndex)
{
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"%s Entry.\n",__func__);
    UInt32 n = 0;
#ifdef FDD_CONFIG
      /** For TTIB_Code */
    UInt8 count = 0;
    /* +- SPR 18268 */
    UInt16 ttibUeCount = 0;
    /* +- SPR 18268 */
    ttibDataQueueNode *ttibQNode_p = PNULL;
    RNTIInfo               *rntiInfo_p = PNULL;
    ueContextForUplink *ueContextInfo_p = PNULL;
    ULHarqInfo* ulHarq_p = PNULL;
#endif
    UInt32 tempLogicalChID        = 0;
    UInt32 logChConfigResp        = MAC_SUCCESS;
    UInt32 returnType             = MAC_SUCCESS;
    UInt8 absLoop = 0 ; /* EICIC +-*/
    UInt8 numBitsCsiPattern = 0 ;/* EICIC +-*/
    /* Get the pointer to ueContext from static UE context Table */
    ueContextForUplink *ueContext_p = PNULL;
    dynamicUEContext   *dynUeContext_p = PNULL;
    dynUeContext_p =  dynUEContextInfoMap_g[ueIdx];
    ueContext_p = uplinkContextInfoMap_g[ueIdx];
    if (PNULL == ueContext_p || PNULL == dynUeContext_p )
    {
        lteWarning (" Entry does not exists in the UEContext Idx Table.\n");
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                    " Unable to reconfigure the UE Entity. \n");
        return MAC_FAILURE;
    }
    if (PNULL == dlUECtxInfoArr_g[ueIdx].dlUEContext_p)
    {
        lteWarning (" Entry does not exists in the dynUEContext Table.\n");
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                 " Unable to reconfigure the UE Entity. \n");
        return MAC_FAILURE;
    }

    /* max harq tx changes */
#if 0
    /* Reconfiguring the provisioned UE Context parameters */
    if (maxRB)
    {
        ueContext_p->maxRB = maxRB;
    }
#endif
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    ueContext_p->isHDFddFlag = uplkUEInfo_p->isHDFddFlag;
    if(TRUE == ueContext_p->isHDFddFlag)
    {
        uplkUEInfo_p->spsUplinkInfo.requestType = SPS_RELEASE;
    }
#endif
    /* HD FDD Changes End */
    /* max harq tx changes */
    if ( INVALID_SIMULTANEOUS_ACK_NACK_VALUE != 
                             uplkUEInfo_p->simultaneousACKNACKAndCQI )
    {   
        ueContext_p->simultaneousACKNACKAndCQI =
                                  uplkUEInfo_p->simultaneousACKNACKAndCQI;
    }
    if (PNULL == lcInfo_p)
    {
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                   "Received a NULL Pointer for Logical Channels.\n");
        return MAC_SUCCESS;
    }
    /*CLPC_CHG*/
    if (uplkUEInfo_p->isDedicatedULPowerControlInfoPresent == TRUE)
    {
        /* Update the reconfigured p0UePusch value */
        ueContext_p->dedicatedULPowerControlInfo.p0UePusch =
            uplkUEInfo_p->dedicatedULPowerControlInfo.p0UePusch;
        ueContext_p->prevPowerCorrectionForPusch = 0;
        ueContext_p->currPowerCorrectionForPusch = 0;
    }
    if (uplkUEInfo_p->isTpcPdcchConfigPuschPresent == TRUE)
    {
        returnType = reconfigureTpcPdcchConfigInfoForPusch (
                uplkUEInfo_p->tpcPdcchConfigPusch.release,
                uplkUEInfo_p->tpcPdcchConfigPusch.tpcRnti,
                uplkUEInfo_p->tpcPdcchConfigPusch.tpcBitMapIndex,
                uplkUEInfo_p->tpcPdcchConfigPusch.isConfiguredForDci3Or3A,
                ueContext_p);
        if (returnType ==  MAC_FAILURE)
        {
            return returnType;
        }
    }
    /* SPS_CHG */
    if (uplkUEInfo_p->isSpsUlInfoPresent && 
        ueContext_p->spsUplinkInfo.requestType != uplkUEInfo_p->spsUplinkInfo.requestType)
    {
         ueContext_p->spsUplinkInfo.requestType = uplkUEInfo_p->spsUplinkInfo.requestType;
         if (SPS_SETUP == uplkUEInfo_p->spsUplinkInfo.requestType)
         {
             ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.semiPersistentSchedIntervalUL = 
                uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.semiPersistentSchedIntervalUL; 
             ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.implicitReleaseAfter = 
                ulImplicitReleaseAfterMap[uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.implicitReleaseAfter]; 
#if 0
             ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent = 
                uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent;
             if (uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isP0TagPresent)
             {
                 ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0NominalPuschPersistent = 
                     uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0NominalPuschPersistent;
                 ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0UePuschPersistent = 
                     uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0UePuschPersistent;
             }
             else
             {
                 ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0NominalPuschPersistent = 0;
                 ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.p0PersistentInfo.p0UePuschPersistent = 0;
             }
#endif
             /* + SPS_TDD_Changes */
             ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent = 
                uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent;
             if ( uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.isTwoIntervalConfigPresent)
             {
                 ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.twoIntervalsConfig = 
                     uplkUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.twoIntervalsConfig;
             }
             else
             {
                 ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.twoIntervalsConfig = FALSE; 
             }
             /* - SPS_TDD_Changes */
         }
         else
         {
             ueContext_p->spsUplinkInfo.spsUplinkSetupInfo.semiPersistentSchedIntervalUL = SPS_INTERVAL_MAX;
         }
    }
    /* SPS_CHG */
#ifdef FDD_CONFIG
      /** For TTIB_Code */
    ueContext_p->ttiBundling = uplkUEInfo_p->ttiBundling;
    if((!ueContext_p->ttiBundling) && ueContext_p->ttibInfo.bundleStartIndex != TTIB_INVALID_VALUE)
    {
        ueContext_p->ttibInfo.bundleStartIndex = TTIB_INVALID_VALUE;

        /****  Freeing the ttibData Queue ****/
        for (count = 0; count < TTI_BUNDLE_RTT; count++)
        {
/* SPR_11526_Fix Start */
            ttibUeCount = queueCount(&(ttibDataQueue_g[cellIndex][count]));
/* SPR_11526_Fix End */
            while( ttibUeCount-- )
            {
/* SPR_11526_Fix Start */
                ttibQNode_p = (ttibDataQueueNode *)deQueue( &(ttibDataQueue_g[cellIndex][count]));
/* SPR_11526_Fix End */
                rntiInfo_p = getRntiInfofromRNTIMap(ttibQNode_p->dciPdu.rnti, cellIndex);
                if (rntiInfo_p )
                {
                    if(ueContext_p->ueIdx == rntiInfo_p->ueIndex)
                    {
/* SPR_11526_Fix Start */
                        enQueue(&(ttibDataQueue_g[cellIndex][ttibQNode_p->ttibqIndex]),&(ttibQNode_p->node));
/* SPR_11526_Fix End */
                    }
                    else
                    {
                        freeMemPool(ttibQNode_p);
                        ttibQNode_p = PNULL;
                    }
                }
            }
        }

        /**** Freeing the memory pointer occupied while tti bundlig on *****/

        for (count = 0; count < TTI_BUNDLE_SIZE; count++)
        {
           ulHarq_p = &ueContext_p->ulharqInfo[count];

           if (ulHarq_p->transmissionBuffer_p)
           {
                freeMemPool(ulHarq_p->transmissionBuffer_p);
                ulHarq_p->transmissionBuffer_p = PNULL;
           }
        }
    }
#endif

    /* Reconfiguring the Logical Channel Structures */
    for (n = 0; n < logicalChCount; n++)
    {
        tempLogicalChID = lcInfo_p[n].lchId;
        UInt32 lcOperationType = 0;

        lcOperationType = lcInfo_p[n].operationType;
        if (ADD == lcOperationType)
        {
            if(ueContext_p->lcInfo[tempLogicalChID].lcGId != 
                                        LCGID_INVALID_VALUE)
            {
                lteWarning ("Logical Channel already added.\n");
                logChConfigResp = MAC_LOG_CH_ERROR;
                lcInfo_p[n].result = MAC_LOG_CH_EXISTS;

                continue;
            }
            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                    "Adding a new logical channel. \n");
            returnType = addULUELogicalChannel(ueContext_p, 1, &lcInfo_p[n], dynUeContext_p );
            if (MAC_FAILURE == returnType)
            {
                logChConfigResp = MAC_LOG_CH_ERROR;
            }
        }
        else if (RECONFIGURE == lcOperationType)
        {
            if (LCGID_INVALID_VALUE == ueContext_p->
                    lcInfo[tempLogicalChID].lcGId)
            {
                lteWarning ("Trying to reconfigure a Logical Channel \
                        that does not exists.\n");
                lcInfo_p[n].result = MAC_LOG_CH_NOT_EXISTS;
                logChConfigResp = MAC_LOG_CH_ERROR;
                continue;
            }

            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"Reconfiguring a logical channel. \n");
            returnType = reconfigULUELogicalChannel(ueContext_p,&lcInfo_p[n]);
            if (MAC_FAILURE == returnType)
            {
                logChConfigResp = MAC_LOG_CH_ERROR;
            }
        }
        else if (DELETE == lcOperationType)
        {
            if (ueContext_p->lcInfo[tempLogicalChID].lcGId == 
                    LCGID_INVALID_VALUE)
            {
                lteWarning ("Trying to delete a Logical Channel that \
                        does not exists.\n");
                lcInfo_p[n].result = MAC_LOG_CH_NOT_EXISTS;
                logChConfigResp = MAC_LOG_CH_ERROR;
                continue;
            }
            deleteULUELogicalChannel( ueContext_p , tempLogicalChID);
            lcInfo_p[n].result = MAC_SUCCESS;
        }
        else
        {
            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"Invalid operation to be performed.\n");
            continue;
        }
    }
   
    if(MAC_SUCCESS != logChConfigResp)
    {
        return logChConfigResp;
    }


#ifdef FDD_CONFIG
       numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
   }
   else
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
   }
#endif


	/* EICIC +*/
    for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
	{	
		ueContext_p->csiMeasurementSubset1[absLoop] = 0;
		ueContext_p->csiMeasurementSubset2[absLoop] = 0;
	}


	if ( uplkUEInfo_p->cqiConfigRequestType == SETUP )
	{
        ueContext_p->riConfigIndex_2 = uplkUEInfo_p->riConfigIndex_2 ;
        ueContext_p->cqiPMIConfigIndex_2 =  uplkUEInfo_p->cqiPMIConfigIndex_2 ;

	}
    if (SETUP >= uplkUEInfo_p->cqiSubsetConfigRequestType)
	{
		for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
		{	
		    ueContext_p->csiMeasurementSubset1[absLoop] = uplkUEInfo_p->csiMeasurementSubset1[absLoop];
		    ueContext_p->csiMeasurementSubset2[absLoop] = uplkUEInfo_p->csiMeasurementSubset2[absLoop];
		}
    }

	/* EICIC -*/
	
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}


/****************************************************************************
 * Function Name  : addULUELogicalChannel
 * Inputs         : newUEContext_p - Pointer to ue context created
 *                  logicalChCount - Number of logical channels to be configured
 *                  logicalChConfig_p - A pointer to the ulLogicalChannelInfo
 *                                      array containing the various
 *                                      configuration parameters for the
 *                                      logical channel.
 *                  dynUEContext_p - Pointer to dynamic UE Context.
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      :
 * Description    : This API is used to add a logical channel as per the
 *                  configuration for a given UE in uplink UE context.
 ****************************************************************************/
MacRetType addULUELogicalChannel (ueContextForUplink* newUEContext_p, 
        UInt8 logicalChCount,
        ulLogicalChannelInfo* logicalChInfo_p,
        dynamicUEContext *dynUEContext_p)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);

    SInt8 tempLogicalChId = 0;
    UInt8 n = 0;
    UInt8 count = 0;

    if (PNULL == logicalChInfo_p)
    {
        LTE_LOG(LOG_WARNING,&MACModuleLogDetail_g,"Received a NULL Pointer for Logical Channels.\n");
        return MAC_FAILURE;
    }

    for (n = 0; n < logicalChCount; n++)
    {
        tempLogicalChId = logicalChInfo_p[n].lchId;
        if(logicalChInfo_p[n].result != MAC_FAILURE)
        {
            lteWarning("Failure in DL LC addition, thus not adding in UL\n");
            continue;
        }    
        if(newUEContext_p->numLogicalChannels < MAX_NUMBER_OF_LOGICAL_CHANNEL)
        {
            //newUEContext_p->lcInfo[tempLogicalChId].transportChType
              //                   = logicalChInfo_p[n].transportChType;

            if (newUEContext_p->lcInfo[tempLogicalChId].lcGId != 
                    LCGID_INVALID_VALUE)
            {
                lteWarning("The logical channel has already been added \
                        before - Not adding again.\n");
                logicalChInfo_p[n].result = MAC_LOG_CH_EXISTS;        
                continue;
            }

            newUEContext_p->lcInfo[tempLogicalChId].lchId
                = logicalChInfo_p[n].lchId;
            newUEContext_p->lcInfo[tempLogicalChId].lcGId
                = logicalChInfo_p[n].lcGId;
            newUEContext_p->lcInfo[tempLogicalChId].rlcMode
                = logicalChInfo_p[n].rlcMode;

            dynUEContext_p->lcCount[logicalChInfo_p[n].lcGId] += 1;
            /* SPS_CHG */
            newUEContext_p->lcInfo[tempLogicalChId].bearerType
                = logicalChInfo_p[n].bearerType;
            newUEContext_p->lcInfo[tempLogicalChId].qci
                = logicalChInfo_p[n].qci;
            newUEContext_p->lcInfo[tempLogicalChId].isSpsLc
                = logicalChInfo_p[n].isSpsLc;
            /* SPS_CHG */

            /* Updating the Logical Channel count */
            newUEContext_p->numLogicalChannels++;
            count++;
            logicalChInfo_p[n].result = MAC_SUCCESS;        
        }
        else
        {
            LTE_LOG(LOG_WARNING,&MACModuleLogDetail_g,"Unable to add the logical channel \
                    (Faulty Configuration).\n");
            LTE_LOG(LOG_WARNING,&MACModuleLogDetail_g,"The tempLogicalChId - %d",tempLogicalChId);
            logicalChInfo_p[n].result = MAC_MAX_ULLOG_CH_CONFIG;
        }
    }

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);
    if(count < logicalChCount)
    {
        return MAC_FAILURE;
    }

    return MAC_SUCCESS;
}


/****************************************************************************
 * Function Name  : reconfigULUELogicalChannel
 * Inputs         : ueContext_p - Pointer to UE context for which the 
 *                         logical channel needs to be reconfigured.
 *                  logicalChInfo_p - A pointer to the ulLogicalChannelInfo
 *                                      array containing the various
 *                                      configuration parameters for the
 *                                      logical channel.
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      :
 * Description    : This API is used to reconfigure a logical channel as
 *                  per the configuration for a given UE.
 *                  If the logical channel does not exist, then return 
 *                  MAC_FAILURE.
 ****************************************************************************/

MacRetType reconfigULUELogicalChannel(ueContextForUplink *ueContext_p,
        ulLogicalChannelInfo* logicalChInfo_p)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);

    /* Temporary variable for Logical Channel Id to be reconfigured */
    SInt8 tempLogicalChId = 0;

    tempLogicalChId = logicalChInfo_p->lchId;

    /* Adding the new Logical Channel in the UE Context */
    if ((tempLogicalChId < MAX_NUMBER_OF_LOGICAL_CHANNEL) &&
            (tempLogicalChId > 0))
    {
        if (ueContext_p->lcInfo[tempLogicalChId].lcGId ==
                LCGID_INVALID_VALUE)
        {
            lteWarning ("The Logical Channel requested to be reconfigured \
                    has not been added yet. \n");
            logicalChInfo_p->result = MAC_LOG_CH_NOT_EXISTS;        
            return MAC_FAILURE;
        }
        //ueContext_p->lcInfo[tempLogicalChId].mBitRate =
          //  logicalChInfo_p->mBitRate;
        //ueContext_p->lcInfo[tempLogicalChId].gBitRate =
          //  logicalChInfo_p->gBitRate;
    }
    else
    {
        lteWarning ("Unable to reconfigure the logical channel (Faulty \
            Configuration).\n");
        return MAC_FAILURE;
    }

    logicalChInfo_p->result = MAC_SUCCESS;        
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);
    return MAC_SUCCESS;
}


/****************************************************************************
 * Function Name  : deleteULUELogicalChannel
 * Inputs         : ueContext_p - Pointer to UE context from which LC 
 *                           needs to be deleted.
 *                  ueLCId - The Logical Channel ID that needs to be deleted.
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      :
 * Description    : This API is used to delete a logical channel as per the
 *                  request for a given UE.
 *****************************************************************************/

MacRetType deleteULUELogicalChannel(ueContextForUplink *ueContext_p,
        UInt8 ueLCId)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);

    /* Set the logical channel and lcGId to invalid */
    ueContext_p->lcInfo[ueLCId].lchId = 0;
    ueContext_p->lcInfo[ueLCId].queueLoad = 0;

    if(dynUEContextInfoMap_g[ueContext_p->ueIdx]->
            lcCount[ueContext_p->lcInfo[ueLCId].lcGId] == 0)
    {
        lteWarning("lcCount for this LCG already 0\n");
    }
    else 
    {
        dynUEContextInfoMap_g[ueContext_p->ueIdx]->
            lcCount[ueContext_p->lcInfo[ueLCId].lcGId] -= 1; 
    }              
    ueContext_p->lcInfo[ueLCId].lcGId = LCGID_INVALID_VALUE;
    ueContext_p->numLogicalChannels--;

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);
    return MAC_SUCCESS;
}


/* HO Reset */

MacRetType resetUeContext ( ueContextForUplink *ueContext_p )
{
    UInt32 i = 0;
    UInt32 j = 0;
    dynamicUEContext   *dynUeContext_p = PNULL;
    dynUeContext_p =  dynUEContextInfoMap_g[ueContext_p->ueIdx];
#ifdef TDD_CONFIG
    if ( PNULL == ueContext_p->ulHarqEntity_p )
    {
        freeMemPool(ueContext_p->ulHarqEntity_p);
        ueContext_p->ulHarqEntity_p = PNULL;    
    }
#endif

    for ( i = 0; i < MAX_NUMBER_OF_LOGICAL_CHANNEL; i++ )
    {
        ueContext_p->lcInfo[i].queueLoad = 0;
    }
    for ( i = 0; i < 10; i++ )
    {
        if ( PNULL != ueContext_p->ulControlNode_p[i] )
        {
            freeMemPool(ueContext_p->ulControlNode_p[i]);
            ueContext_p->ulControlNode_p[i] = PNULL;
        }
        if ( PNULL != ueContext_p->ulDataNode_p[i] )
        {
            freeMemPool(ueContext_p->ulDataNode_p[i]);
            ueContext_p->ulDataNode_p[i] = PNULL;
        }
        if ( PNULL != ueContext_p->ulDataPduInfo[i].Pdudata_p )
        {
            freeMemPool(ueContext_p->ulDataPduInfo[i].Pdudata_p);
            ueContext_p->ulDataPduInfo[i].Pdudata_p = PNULL;
        }
        ueContext_p->ulHarqTimerNodeArrrayIndex[i] = 0;

    }
    if ( PNULL != ueContext_p->srPduNode_p )
    {
        freeMemPool(ueContext_p->srPduNode_p);
        ueContext_p->srPduNode_p = PNULL;
    }
    ueContext_p->ulSchPduRcvdFlag = FALSE;
    ueContext_p->uciSrRcvdFlag = FALSE;
    ueContext_p->latDci0RcvdTick = 0;
    ueContext_p->lastQUpdateTime = 0;
    ueContext_p->srTriggered = FALSE;
    ueContext_p->availGrantCounter = 0;
    ueContext_p->tbSize = 0;
    ueContext_p->queueLoad = 0;
    ueContext_p->txTTI = 0;
    ueContext_p->taToBeSentFlag = FALSE;
    ueContext_p->hiReceived = FALSE;

   for( i = 0; i < MAC_MAX_RESOURCE_BLOCKS; i++)
   {
       for( j = 0; j < MAX_HARQ_PROCESS_NB; j++ )
       {
          hi_pdu_crnti_array_g[i][j].crnti = 0; 
       }
   }
   for ( i = 0; i < 8; i++ )
   {
       ueContext_p->ulharqInfo[i].deltabsrLoad = 0;
       ueContext_p->ulharqInfo[i].riStart = 0;
       ueContext_p->ulharqInfo[i].riLength = 0;
       ueContext_p->ulharqInfo[i].ackNack = 0;
       ueContext_p->ulharqInfo[i].currentRetransCount = 0;
       ueContext_p->ulharqInfo[i].ulgrantrecvCount = 0;
       ueContext_p->ulharqInfo[i].tbSizeForNackCase = 0;
   }
    for ( i =0; i < NUM_OF_LCG; i++ )
    {
        dynUeContext_p->bsrNet[i] = 0;
    }
    dynUeContext_p->bsrNetTotal = 0;
    
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : reconfigureTpcPdcchConfigInfoForPusch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  dciFormat - indicates whether tpc rnti is configured with
 *                  DCI FORMAT 3/3A.
 *                  release - Indicates whether tpc info needs to be configured
 *                  released.
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates updates the tpc info in ue context and
 *                  tpc rnti corresponding to the tpc rnti configured for the
 *                  UE.
 ******************************************************************************/
static inline MacRetType reconfigureTpcPdcchConfigInfoForPusch(
        UInt8 release,
        UInt16 rnti,
        UInt8 tpcBitMapIndex,
        UInt8 dciFormat,
        ueContextForUplink *ueContext_p)
{
    MacRetType retVal = MAC_SUCCESS;
    UInt8 idx = 0;
    UInt8 flag = 0;

    if ( release )
    {
        if (ueContext_p->tpcPdcchConfigPusch.tpcRnti == 0)
        {
            return MAC_SUCCESS;
        }
        TpcInfo tpcInfo = tpcRntiInfoMap[(ueContext_p->tpcPdcchConfigPusch.tpcRnti)-START_TPC_RNTI];
        tpcInfo.configuredCount--;

        for(idx = 0; idx< 16; idx++)
        {
            if(tpcInfo.ueIndex[idx] == ueContext_p->ueIdx)
            {
                tpcInfo.isValid[idx] = FALSE;
                break;
            }
        }
        ueContext_p->tpcPdcchConfigPusch.tpcRnti = 0;
        retVal = MAC_SUCCESS;
    }
    else if (ueContext_p->tpcPdcchConfigPusch.tpcRnti == 0)
    {
        if(ueContext_p->dedicatedULPowerControlInfo.accumulationEnabled ==
                TRUE)
        {

            ueContext_p->tpcPdcchConfigPusch.tpcRnti = rnti;
            ueContext_p->tpcPdcchConfigPusch.tpcBitMapIndex = tpcBitMapIndex;

            TpcInfo reconfigTpcInfo = tpcRntiInfoMap[(rnti) - START_TPC_RNTI];
            if(reconfigTpcInfo.isValid[tpcBitMapIndex] == FALSE)
            {
                reconfigTpcInfo.ueIndex[tpcBitMapIndex] = ueContext_p->ueIdx;
                reconfigTpcInfo.isValid[tpcBitMapIndex] = TRUE;
                reconfigTpcInfo.configuredCount++;
            }
            else
            {
                return MAC_FAILURE;
            }
            retVal = MAC_SUCCESS;
        }
        else
        {
            return MAC_FAILURE;
        }
    }
    else if((ueContext_p->tpcPdcchConfigPusch.tpcRnti != rnti) || 
                (ueContext_p->tpcPdcchConfigPusch.tpcBitMapIndex != tpcBitMapIndex))
    {
        return MAC_FAILURE;
    }
    return retVal;

}
