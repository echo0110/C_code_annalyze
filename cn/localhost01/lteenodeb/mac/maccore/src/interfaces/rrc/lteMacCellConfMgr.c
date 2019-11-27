/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacCellConfMgr.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacCellConfMgr.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.13  2009/08/24 12:54:36  gur11912
 * cellConfigured_g defined as volatile
 *
 * Revision 1.12  2009/07/17 06:24:58  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.11  2009/07/13 04:48:04  gur19413
 * removed warnings
 *
 * Revision 1.10  2009/07/13 04:07:31  gur19413
 * change log type
 *
 * Revision 1.9  2009/06/22 06:22:38  gur19413
 * added new parameter in cell config structure
 *
 * Revision 1.8  2009/06/10 13:28:26  gur19413
 * comments incorportated
 *
 * Revision 1.7  2009/06/05 13:36:54  gur19413
 * added new parameters
 *
 * Revision 1.6  2009/06/05 13:24:46  gur19413
 * remove functions
 *
 * Revision 1.5  2009/06/02 07:45:58  gur18569
 * defined cellConfigured_g
 *
 * Revision 1.4  2009/06/02 04:18:31  gur18569
 * removed compilation errors
 *
 * Revision 1.3  2009/06/01 07:47:15  gur18569
 * removed compilation errors
 *
 * Revision 1.2  2009/05/15 06:08:55  gur19413
 * remove compilation error
 *
 * Revision 1.1  2009/04/30 12:10:30  gur19413
 * Initial Version
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacULCSIManager.h"
#include "lteMacPuschPowerControl.h"
#include "lteMacPucchPowerControl.h"
#include "lteMacParseOAMMsg.h" 
#ifdef TDD_CONFIG
#include "lteMacPUCCH.h"
#include "lteMacDLHarqMgr.h"
#endif
#include "lteMacPRSMgr.h"  
/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define MIN_LBIT_LABEL_PERIODIC_VALUE        1
#define MAX_LBIT_LABEL_PERIODIC_VALUE        2
#define MIN_LBIT_LABEL_APERIODIC_VALUE       2
#define MAX_LBIT_LABEL_APERIODIC_VALUE      18
/* + CQI_5.0 */
#define MAX_RB_INDEX_FOR_PERIODIC_LBIT_LABEL 111
#define MAX_J_FOR_PERIODIC_LBIT_LABEL       5
#define MAX_N_FOR_APERIODIC_LBIT_LABEL      29
#define MAX_M_FOR_APERIODIC_LBIT_LABEL      7
/* CA Changes start */
#define GET_N_VALUE(nValue,k,dlAvailableRBs)\
{\
            nValue = ((dlAvailableRBs - 1)/k) + 1; \
\
}
/* CA Changes end */
/* - CQI_5.0 */
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

/* Variable to store the % increment in bler when UL NACK received*/
UInt8 puschBlerNackWeightage_g   = 0;
/* Variable to store the % decrement in bler when UL ACK received*/
UInt8 puschBlerAckWeightage_g   = 0;

UInt8 dataForSpecialSubFrame = TRUE;

/* rbsForPUCCH_g denotes the number of RBs reserved at one end of the RB Map
 * for PUCCH
 */
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
/*CA Changes start */
UInt8 rbsForPUCCH_g[MAX_NUM_CELL];
/*CA Changes end */
/*UInt8 rbsForPUCCH_g;*/
#else
UInt8 rbsForPUCCH_g[MAX_NUM_CELL][MAX_SUB_FRAME];
#endif
/* TDD Config 0 Changes End */
/* SS_S2 */
/* The below structure combines: 
   Table 7.2.1-5 and 7.2.2-2 from spec 36.213 */
BandwidthParamsInfo bandwidthParams_g[MAX_BANDWIDTH_INFO_ENTRIES] = 
{
/* N{DL-RB} start , N{DL-RB} end   , k(M), k(J), M, J */
   { 6            , 7              , 0   , 0   , 0, 0 }, 
   { 8            , 10             , 2   , 4   , 1, 1 }, 
   { 11           , 26             , 2   , 4   , 3, 2 },
   { 27           , 63             , 3   , 6   , 5, 3 }, 
   { 64           , 110            , 4   , 8   , 6, 4 } 
};
/* SPR 13143 Changes start */
 void calculateDci3And3AShiftOffsets(InternalCellIndex internalCellIndex);
/* SPR 13143 Changes end */
/* + CQI_5.0 */
UInt8 lBitLabelForPeriodic[MAX_RB_INDEX_FOR_PERIODIC_LBIT_LABEL]
                                 [MAX_J_FOR_PERIODIC_LBIT_LABEL] = 
      { 
              /*    0   1  2  3  4   */
       /* 0 */    { 0 , 0, 0, 0, 0}, 
       /* 1 */    { 0 , 0, 0, 0, 0}, 
       /* 2 */    { 0 , 0, 0, 0, 0}, 
       /* 3 */    { 0 , 0, 0, 0, 0}, 
       /* 4 */    { 0 , 0, 0, 0, 0}, 
       /* 5 */    { 0 , 0, 0, 0, 0}, 
       /* 6 */    { 0 , 0, 0, 0, 0}, 
       /* 7 */    { 0 , 0, 0, 0, 0}, 
       /* 8 */    { 0 , 1, 0, 0, 0}, 
       /* 9 */    { 0 , 2, 0, 0, 0}, 
       /* 10 */   { 0 , 2, 0, 0, 0}, 
       /* 11 */   { 0 , 0, 1, 0, 0}, 
       /* 12 */   { 0 , 0, 1, 0, 0}, 
       /* 13 */   { 0 , 0, 1, 0, 0}, 
       /* 14 */   { 0 , 0, 1, 0, 0}, 
       /* 15 */   { 0 , 0, 1, 0, 0}, 
       /* 16 */   { 0 , 0, 1, 0, 0}, 
       /* 17 */   { 0 , 0, 2, 0, 0}, 
       /* 18 */   { 0 , 0, 2, 0, 0}, 
       /* 19 */   { 0 , 0, 2, 0, 0}, 
       /* 20 */   { 0 , 0, 2, 0, 0}, 
       /* 21 */   { 0 , 0, 2, 0, 0},
       /* 22 */   { 0 , 0, 2, 0, 0},
       /* 23 */   { 0 , 0, 2, 0, 0},
       /* 24 */   { 0 , 0, 2, 0, 0},
       /* 25 */   { 0 , 0, 2, 0, 0},
       /* 26 */   { 0 , 0, 2, 0, 0},
       /* 27 */   { 0 , 0, 0, 1, 0},
       /* 28 */   { 0 , 0, 0, 1, 0},
       /* 29 */   { 0 , 0, 0, 1, 0},
       /* 30 */   { 0 , 0, 0, 1, 0},
       /* 31 */   { 0 , 0, 0, 1, 0},
       /* 32 */   { 0 , 0, 0, 1, 0},
       /* 33 */   { 0 , 0, 0, 1, 0},
       /* 34 */   { 0 , 0, 0, 1, 0},
       /* 35 */   { 0 , 0, 0, 1, 0},
       /* 36 */   { 0 , 0, 0, 1, 0},
       /* 37 */   { 0 , 0, 0, 2, 0},
       /* 38 */   { 0 , 0, 0, 2, 0},
       /* 39 */   { 0 , 0, 0, 2, 0},
       /* 40 */   { 0 , 0, 0, 2, 0},
       /* 41 */   { 0 , 0, 0, 2, 0},
       /* 42 */   { 0 , 0, 0, 2, 0},
       /* 43 */   { 0 , 0, 0, 2, 0},
       /* 44 */   { 0 , 0, 0, 2, 0},
       /* 45 */   { 0 , 0, 0, 2, 0},
       /* 46 */   { 0 , 0, 0, 2, 0},
       /* 47 */   { 0 , 0, 0, 2, 0},
       /* 48 */   { 0 , 0, 0, 2, 0},
       /* 49 */   { 0 , 0, 0, 2, 0},
       /* 50 */   { 0 , 0, 0, 2, 0},
       /* 51 */   { 0 , 0, 0, 2, 0},
       /* 52 */   { 0 , 0, 0, 2, 0},
       /* 53 */   { 0 , 0, 0, 2, 0},
       /* 54 */   { 0 , 0, 0, 2, 0},
       /* 55 */   { 0 , 0, 0, 2, 0},
       /* 56 */   { 0 , 0, 0, 2, 0},
       /* 57 */   { 0 , 0, 0, 2, 0},
       /* 58 */   { 0 , 0, 0, 2, 0},
       /* 59 */   { 0 , 0, 0, 2, 0},
       /* 60 */   { 0 , 0, 0, 2, 0},
       /* 61 */   { 0 , 0, 0, 2, 0},
       /* 62 */   { 0 , 0, 0, 2, 0},
       /* 63 */   { 0 , 0, 0, 2, 0},
       /* 64 */   { 0 , 0, 0, 0, 1},
       /* 65 */   { 0 , 0, 0, 0, 2},
       /* 66 */   { 0 , 0, 0, 0, 2},
       /* 67 */   { 0 , 0, 0, 0, 2},
       /* 68 */   { 0 , 0, 0, 0, 2},
       /* 69 */   { 0 , 0, 0, 0, 2},
       /* 70 */   { 0 , 0, 0, 0, 2},
       /* 71 */   { 0 , 0, 0, 0, 2},
       /* 72 */   { 0 , 0, 0, 0, 2},
       /* 73 */   { 0 , 0, 0, 0, 2},
       /* 74 */   { 0 , 0, 0, 0, 2},
       /* 75 */   { 0 , 0, 0, 0, 2},
       /* 76 */   { 0 , 0, 0, 0, 2},
       /* 77 */   { 0 , 0, 0, 0, 2},
       /* 78 */   { 0 , 0, 0, 0, 2},
       /* 79 */   { 0 , 0, 0, 0, 2},
       /* 80 */   { 0 , 0, 0, 0, 2},
       /* 81 */   { 0 , 0, 0, 0, 2},
       /* 82 */   { 0 , 0, 0, 0, 2},
       /* 83 */   { 0 , 0, 0, 0, 2},
       /* 84 */   { 0 , 0, 0, 0, 2},
       /* 85 */   { 0 , 0, 0, 0, 2},
       /* 86 */   { 0 , 0, 0, 0, 2},
       /* 87 */   { 0 , 0, 0, 0, 2},
       /* 88 */   { 0 , 0, 0, 0, 2},
       /* 89 */   { 0 , 0, 0, 0, 2},
       /* 90 */   { 0 , 0, 0, 0, 2},
       /* 91 */   { 0 , 0, 0, 0, 2},
       /* 92 */   { 0 , 0, 0, 0, 2},
       /* 93 */   { 0 , 0, 0, 0, 2},
       /* 94 */   { 0 , 0, 0, 0, 2},
       /* 95 */   { 0 , 0, 0, 0, 2},
       /* 96 */   { 0 , 0, 0, 0, 2},
       /* 97 */   { 0 , 0, 0, 0, 2},
       /* 98 */   { 0 , 0, 0, 0, 2},
       /* 99 */   { 0 , 0, 0, 0, 2},
       /* 100 */  { 0 , 0, 0, 0, 2}, 
       /* 101 */  { 0 , 0, 0, 0, 2}, 
       /* 102 */  { 0 , 0, 0, 0, 2},
       /* 103 */  { 0 , 0, 0, 0, 2}, 
       /* 104 */  { 0 , 0, 0, 0, 2},
       /* 105 */  { 0 , 0, 0, 0, 2},
       /* 106 */  { 0 , 0, 0, 0, 2},
       /* 107 */  { 0 , 0, 0, 0, 2},
       /* 108 */  { 0 , 0, 0, 0, 2},
       /* 109 */  { 0 , 0, 0, 0, 2},
       /* 110 */  { 0 , 0, 0, 0, 2} 
     };

UInt32 lBitLabelForAperiodic[MAX_N_FOR_APERIODIC_LBIT_LABEL]
                               [MAX_M_FOR_APERIODIC_LBIT_LABEL] = 
      {
           /* 0  1  2  3  4  5   6 */ 
    /* 0 */ { 0, 0, 0, 0, 0, 0,  0 }, 
    /* 1 */ { 0, 0, 0, 0, 0, 0,  0 }, 
    /* 2 */ { 0, 0, 0, 0, 0, 0,  0 }, 
    /* 3 */ { 0, 0, 0, 0, 0, 0,  0 }, 
    /* 4 */ { 0, 2, 0, 0, 0, 0,  0 }, 
    /* 5 */ { 0, 3, 0, 0, 0, 0,  0 }, 
    /* 6 */ { 0, 0, 0, 5, 0, 0,  0 }, 
    /* 7 */ { 0, 0, 0, 6, 0, 0,  0 }, 
    /* 8 */ { 0, 0, 0, 6, 0, 0,  0 }, 
    /* 9 */ { 0, 0, 0, 7, 0, 7,  0 }, 
   /* 10 */ { 0, 0, 0, 7, 0, 8,  0 }, 
   /* 11 */ { 0, 0, 0, 8, 0, 9,  0 }, 
   /* 12 */ { 0, 0, 0, 8, 0, 10, 0 }, 
   /* 13 */ { 0, 0, 0, 9, 0, 11, 0 }, 
   /* 14 */ { 0, 0, 0, 0, 0, 11, 0 }, 
   /* 15 */ { 0, 0, 0, 0, 0, 12, 0 }, 
   /* 16 */ { 0, 0, 0, 0, 0, 13, 13 }, 
   /* 17 */ { 0, 0, 0, 0, 0, 13, 14 }, 
   /* 18 */ { 0, 0, 0, 0, 0, 14, 15 }, 
   /* 19 */ { 0, 0, 0, 0, 0, 14, 15 }, 
   /* 20 */ { 0, 0, 0, 0, 0, 14, 16 }, 
   /* 21 */ { 0, 0, 0, 0, 0, 15, 16 }, 
   /* 22 */ { 0, 0, 0, 0, 0, 0,  17 }, 
   /* 23 */ { 0, 0, 0, 0, 0, 0,  17 }, 
   /* 24 */ { 0, 0, 0, 0, 0, 0,  18 }, 
   /* 25 */ { 0, 0, 0, 0, 0, 0,  18 }, 
   /* 26 */ { 0, 0, 0, 0, 0, 0,  18 }, 
   /* 27 */ { 0, 0, 0, 0, 0, 0,  19 }, 
   /* 28 */ { 0, 0, 0, 0, 0, 0,  19 } 
     };
/* - CQI_5.0 */
/* + CQI_4.1 */
/* nCm_g will store the binomial coefficient of N and M */
/* CA Changes start */
UInt32 nCm_g[MAX_NUM_CELL][MAX_UE_SELECTED_N][MAX_UE_SELECTED_SUBBAND] = {{{0},{0},{0}}};
/* CA Changes end */
/* SPR 13143 Changes start */
UInt32 dci3RightShiftOffset_g[MAX_NUM_CELL] = {0} ;
UInt32 dci3ARightShiftOffset_g[MAX_NUM_CELL] = {0};
/* SPR 13143 Changes end */
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
UInt8 numRBGConfigured_g[MAX_NUM_CELL] = {0};
UInt8 rbgSize_g[MAX_NUM_CELL] = {0};
/* These mask are taken to cater last RB left in RBG sets during RAT 1
 * allocation specially in 15,10,5 and 3 MHz bandwidth */
UInt32 lastRbgIdxMask_g = 0;
#define LAST_RBG_IDX_MASK_15MHZ     0x40000
#define LAST_RBG_IDX_MASK_10MHZ     0x10000
#define LAST_RBG_IDX_MASK_5MHZ      0x1000 
#define LAST_RBG_IDX_MASK_3MHZ      0x80 
#define LAST_RBG_IDX_MASK_1DOT4MHZ  0x20 
#endif
/* SPR 21958 PUSCH RAT1 Support End */
/****************************************************************************
 * Function Name  : initUESelectedBinomalValues
 * Inputs         : N - number of subbands
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Initialize binomial values
 * Returns        : None
 * Description    : This function initialize nCm_g with binomal coefficient
 *                  (nCr) from N and M.
 ****************************************************************************/
/* CA Changes start */
void initUESelectedBinomalValues(UInt8 N , UInt8 M,InternalCellIndex internalCellIndex)
{
    UInt8 numSubbands = 0 , preferredSubbands = 0;

    /* nCm is the binomial coefficient of N and M 
     *if N >= M then nCm_g will contain nCm ( n = N , m = M )
     *if N< M then nCm_g will contain 0
     */

    for (preferredSubbands = 1; preferredSubbands <= M; preferredSubbands++) 
    {
        nCm_g[internalCellIndex][0][preferredSubbands] = 0;
    }

    for (numSubbands  = 0; numSubbands <= N; numSubbands++) 
    {
        nCm_g[internalCellIndex][numSubbands][0] = 1;
    }
    
    for (numSubbands  = 1; numSubbands <= N; numSubbands++)
    {
        for(preferredSubbands = 1; preferredSubbands <= M; preferredSubbands++)
        {
             nCm_g[internalCellIndex][numSubbands][preferredSubbands] = 
              nCm_g[internalCellIndex][numSubbands - 1][preferredSubbands - 1] + 
                           nCm_g[internalCellIndex][numSubbands - 1][preferredSubbands];
        }
    }
    return;

}  

/* CA Changes end */
/****************************************************************************
 * Function Name  : getBinomialCoefficient
 * Inputs         : N - Number of Subbands,
 *                  M - preferred Subbands 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : binomial coefficient
 * Returns        : binomial coefficient
 * Description    : This function access nCm_g to get binomal coefficient
 *                  (nCr) of N and M.
 ****************************************************************************/
/* CA Changes start */
UInt32  getBinomialCoefficient(UInt8 N , UInt8 M,InternalCellIndex internalCellIndex)
{
        return  nCm_g[internalCellIndex][N][M];
}
/* CA Changes end */
/* - CQI_4.1 */
/****************************************************************************
 * Function Name  : getNumberOfSubbandsM 
 * Inputs         : valK_p - value of subband size k and
 *                  valM_p - number of subbands M
 *                  dlAvailableRBs - Downlink available RB count.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function searches the bandwidthParams_g structure and
 *                  extracts the value of k and M.
 ****************************************************************************/
/* CA Changes start */
MacRetType getNumberOfSubbandsM(UInt8 *valK_p, UInt8 *valM_p, UInt8 dlAvailableRBs)
{
   UInt8 index = 0;
   MacRetType infoFound = MAC_FAILURE;
    
   for (index = 0; index < MAX_BANDWIDTH_INFO_ENTRIES; index++)
   {
       if ((dlAvailableRBs >= bandwidthParams_g[index].
               bandWidthStart)
             && (dlAvailableRBs <= bandwidthParams_g[index].
             bandWidthEnd))
       {
           *valK_p = bandwidthParams_g[index].kForM; 
           *valM_p = bandwidthParams_g[index].M;
           infoFound = MAC_SUCCESS;
           break;
       }
   }

   if((MAC_FAILURE == infoFound))
   {
       lteWarning("Invalid Value of System bandwith for DL\n");
   }
   
   return infoFound;
}

/****************************************************************************
 * Function Name  : getBandwidthPartsJ
 * Inputs         : valJ_p - bandwidth parts J and
 *                  valK_p - value of subband size k
 *                  dlAvailableRBs - Downlink available RB count.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function searches the bandwidthParams_g structure and
 *                  extracts the value of k and J.
 ****************************************************************************/
MacRetType getBandwidthPartsJ(UInt8 *valJ_p, UInt8 *valK_p,UInt8 dlAvailableRBs)
{
   UInt8 index = 0;
   MacRetType infoFound = MAC_FAILURE;
    
   for (index = 0; index < MAX_BANDWIDTH_INFO_ENTRIES; index++)
   {
       if ((dlAvailableRBs >= bandwidthParams_g[index].
               bandWidthStart)
             && (dlAvailableRBs <= bandwidthParams_g[index].
             bandWidthEnd))
       {
           *valK_p = bandwidthParams_g[index].kForJ; 
           *valJ_p = bandwidthParams_g[index].J;
           infoFound = MAC_SUCCESS;
           break;
       }
   }

   if((MAC_FAILURE == infoFound))
   {
       lteWarning("Invalid Value of System bandwith for DL\n");
   }
   
   return infoFound;
}
/* SS_S2 */
/* CA Changes end */
/* QOS_CHG */
/****************************************************************************
 * Function Name  : qosUpdateConfigParam 
 * Inputs         : initParams_p,dlAvailableRBs,ulAvailableRBs 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to update qosStrategyOverallocFactor 
 *                  which is received at the time of MAC Init.
 ****************************************************************************/
/* CA Changes start */
void qosUpdateConfigParam(InitParams *initParams_p, UInt8 dlAvailableRBs,
                         UInt8    ulAvailableRBs)
{
    initParams_p->dlSchdConfig.strategyBufferRB = ((dlAvailableRBs *
              initParams_p->dlSchdConfig.strategyBufferRB)/100);
     initParams_p->ulSchdConfig.strategyBufferRB = ((ulAvailableRBs *
       initParams_p->ulSchdConfig.strategyBufferRB)/100);
}
/* CA Changes end */
/* QOS_CHG */

/*****************************************************************************
 * Private Variables (Must be declared static)
*****************************************************************************/
/****************************************************************************
 * Function Name  : processCellConfig 
 * Inputs         : configCellReq_p - Its point to ConfigCellReq. This structure 
 *                                    contains information releated to DL/UL RBs.
 *                  internalCellIndex - Cell-Index 
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API is used to set the available number of Resource
 *                  Blocks that are available for allocation per TTI as per 
 *                  the frequency band configured at RRC.
 ****************************************************************************/
/* CA Changes start */
MacRetType processCellConfig( ConfigCellReq *configCellReq_p,
        InternalCellIndex internalCellIndex)
{
    /* SS_S2 */
    UInt8  k = 0; /* subband size */
    UInt8  J = 0; /* bandwidth parts */
    UInt8  M = 0; /* preferred subbands */
    CellConfigParams *cellParams_p =  cellSpecificParams_g.cellConfigAndInitParams_p
        [internalCellIndex]->cellParams_p;   
    InitParams *initParams_p = cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    PRSConfigInfo *prsConfigInfo_p =  &cellParams_p->prsConfigInfo;
    MacRetType retval = MAC_SUCCESS;
    UInt8 nB;
    puschBlerNackWeightage_g = (100 - MAC_TARGET_BLER);
    puschBlerAckWeightage_g = MAC_TARGET_BLER;
    cellParams_p->contnFreeRachTimer = MAC_MIN_CFR_TIMER_VAL;
    /*SPR_3061_FIX*/
    cellParams_p->ulSyncLossTimer = UL_SYNC_TIMER_INFINITY; 
    cellParams_p->pucchCQISINRThreshold = 0;
    /*Dynamic Pdcch Changes Start*/
    cellParams_p->isDynamicPdcchDisabled = 0;
    /*Dynamic Pdcch Changes End*/
    /* + PRS_CHANGES */
    /*CA Changes start JAVED */
    prsConfigInfo_p->isPRSConfigured = 0;
    prsConfigInfo_p->icicConfigured  = 0;
    prsConfigInfo_p->dlPRSMapIndex = 0;
    prsConfigInfo_p->dlPRSRATPoliciesIndex = 0;
    prsConfigInfo_p->prsResInfo[prsConfigInfo_p->dlPRSRATPoliciesIndex].\
        prsAllowedWithICIC = 0;
    prsConfigInfo_p->prsResInfo[!prsConfigInfo_p->dlPRSRATPoliciesIndex].\
        prsAllowedWithICIC = 0;
    prsConfigInfo_p->prsSubframeCounter = 0;
    /*CA Changes end JAVED */

    PRSParseConfigInfo  prsParseConfigInfo = configCellReq_p->prsParseConfigInfo;
    /* - PRS_CHANGES */
#ifdef KPI_STATS
    /*++klockwork warning */
    UInt32 activeIndexForMacStats =0;

    if(gMacStats.pStats)
    {
        /* + KPI_CA */
        activeIndexForMacStats = (gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
        /* - KPI_CA */
    }
    /*--klockwork warning */
#endif
    /*SPR_3061_FIX*/
    /* SS_S2 */
    /* + CQI_4.1 */
    /* This will contain Binomial coefficient of ue selected N and 
     * M preferred subband  from nCm_g */
    UInt32 nCm = 0;
    /* - CQI_4.1 */

    /* Configure Duplexing Mode */
    cellParams_p->duplexingMode = configCellReq_p->duplexingMode;
    /* Configure the Downlink Resources */
    cellParams_p->dlAvailableRBs  = configCellReq_p->dlResBlocks;

    /*Dynamic Pdcch Changes Start*/
    cellParams_p->dlBandwidth = configCellReq_p->dlBandwidth;
    /*Dynamic Pdcch Changes End */
    if (cellParams_p->dlAvailableRBs)
    {

    }
    /* Configure the Uplink Resources */
    cellParams_p->ulAvailableRBs = configCellReq_p->ulResBlocks;
#ifdef KPI_STATS
    if(gMacStats.pStats)
    {
        /* + KPI_CA */
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].\
            uplinkCellBandwidth = (UInt32)180000*configCellReq_p->ulResBlocks;
        /* - KPI_CA */
    }
#endif
    /** opt **/
    /*+- SPR 20899 Fix*/
    cellParams_p->resAllocInitInfo.lsbForFillRar = 
        ceil_wrapper( (log10_wrapper(((UDouble32)cellParams_p->ulAvailableRBs) *
                        DIVIDE_BY_TWO(cellParams_p->ulAvailableRBs + 1 ))/0.30103));
    /*+- SPR 20899 Fix*/
    cellParams_p->resAllocInitInfo.msbForFillRar =
        cellParams_p->resAllocInitInfo.lsbForFillRar - 10;

    /** opt **/
    /* Configure the Start and End Range of RA-RNTI */
    cellParams_p->startRARNTI    = configCellReq_p->startRARntiRange;
    cellParams_p->endRARNTI      = configCellReq_p->endRARntiRange;

    /* Configure the Number of TX Antennas */
    cellParams_p->numOfTxAnteenas = configCellReq_p->numOfTxAntennas;

    /* Set Total Number of Acwith default Value 0*/
    cellParams_p->totalActiveUE  =  0;

    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    if(DL_BW_20_MHZ == cellParams_p->dlBandwidth)
    {
	    numRBGConfigured_g[internalCellIndex] = NUM_OF_RBG_BW20;
	    rbgSize_g[internalCellIndex] = RBG_SIZE_FOR_BW20;
    }
    else if(DL_BW_15_MHZ == cellParams_p->dlBandwidth)
    {
	    numRBGConfigured_g[internalCellIndex] = NUM_OF_RBG_BW15;
	    rbgSize_g[internalCellIndex] = RBG_SIZE_FOR_BW15;
	    lastRbgIdxMask_g = LAST_RBG_IDX_MASK_15MHZ; 
    }
    else if(DL_BW_10_MHZ == cellParams_p->dlBandwidth)
    {
	    numRBGConfigured_g[internalCellIndex] = NUM_OF_RBG_BW10;
	    rbgSize_g[internalCellIndex] = RBG_SIZE_FOR_BW10;
	    lastRbgIdxMask_g = LAST_RBG_IDX_MASK_10MHZ; 
    }
    else if(DL_BW_5_MHZ == cellParams_p->dlBandwidth)
    {
	    numRBGConfigured_g[internalCellIndex] = NUM_OF_RBG_BW5;
	    rbgSize_g[internalCellIndex] = RBG_SIZE_FOR_BW5;
	    lastRbgIdxMask_g = LAST_RBG_IDX_MASK_5MHZ; 
    }
    else if(DL_BW_3_MHZ == cellParams_p->dlBandwidth)
    {
	    numRBGConfigured_g[internalCellIndex] = NUM_OF_RBG_BW3;
	    rbgSize_g[internalCellIndex] = RBG_SIZE_FOR_BW3;
	    lastRbgIdxMask_g = LAST_RBG_IDX_MASK_3MHZ; 
    }
    /* SPR 22298 Fix Start */
    else if(DL_BW_1DOT4_MHZ == cellParams_p->dlBandwidth)
    {
	    numRBGConfigured_g[internalCellIndex] = NUM_OF_RBG_BW1DOT4;
	    rbgSize_g[internalCellIndex] = RBG_SIZE_FOR_BW1DOT4;
	    lastRbgIdxMask_g = LAST_RBG_IDX_MASK_1DOT4MHZ; 
    }
    /* SPR 22298 Fix End */
    else
    {
        LOG_MAC_WARNING(MAC_RRC_INF,"PUSCH RAT1 : Incorrect Bandwidth Configuration\n");
    }

#endif    
    /* SPR 21958 PUSCH RAT1 Support End */
    
    if ( cellParams_p->ulAvailableRBs)
    {
        /*CA Changes start JAVED */
        initializeResourceMap(internalCellIndex);    
        /*CA Changes end JAVED */
    }

    cellParams_p->maxHarqRetransmission = configCellReq_p->maxHarqRetrans;
    cellParams_p->phichDuration         = configCellReq_p->phichDuration;
    cellParams_p->phichResource         = configCellReq_p->phichResource;
    cellParams_p->cyclicPrefix          = configCellReq_p->ulCyclicLenthPrefix;
    /* SS_S2 */
    /*Dynamic Pdcch Changes Start*/
    if (1 == cellParams_p->phichDuration)
    {
        cellParams_p->isDynamicPdcchDisabled = 1;
    }
    /*Dynamic Pdcch Changes End*/
    /* + CQI_4.1 */
    if( 7 < cellParams_p->dlAvailableRBs)
    {   
        retval = getBandwidthPartsJ(&J, &k,cellParams_p->dlAvailableRBs);

        if (MAC_FAILURE == retval)
        {
            return MAC_FAILURE;
        }

        cellParams_p->bandwidthParts = J;

        GET_N_VALUE(cellParams_p->N,k,cellParams_p->dlAvailableRBs); 

        cellParams_p->LBitLabel = lBitLabelForPeriodic[cellParams_p->dlAvailableRBs][J];

        if ((MIN_LBIT_LABEL_PERIODIC_VALUE > cellParams_p->LBitLabel) 
                || ( MAX_LBIT_LABEL_PERIODIC_VALUE < cellParams_p->LBitLabel) )
        {
            lteWarning("Invalid Lbit Label [ %d] for periodic mode in [%s]\n",
                    cellParams_p->LBitLabel,__func__);
            return MAC_FAILURE;

        }    

        /* + CQI_4.1 */
        fillMinRBsForAperiodicModes(cellParams_p->dlAvailableRBs, internalCellIndex);
        /* - CQI_4.1 */

        retval = getNumberOfSubbandsM(&k, &M, cellParams_p->dlAvailableRBs);
        if (MAC_FAILURE == retval)
        {
            return MAC_FAILURE;
        }

        cellParams_p->preferredSubbands = M;

        GET_N_VALUE(cellParams_p->ueSelectedN,k,cellParams_p->dlAvailableRBs); 

        initUESelectedBinomalValues(cellParams_p->ueSelectedN,
                cellParams_p->preferredSubbands,
                internalCellIndex);

        nCm = getBinomialCoefficient(cellParams_p->ueSelectedN,M,internalCellIndex);
        cellParams_p->maxCombinatorialIndex = nCm - 1;

        /* number of bits to be used for M preferred subband ref 36.213 sec 7.2.1 */
        cellParams_p->LBitLabelForAperiodic =  
            lBitLabelForAperiodic[cellParams_p->ueSelectedN][M];
        if ((MIN_LBIT_LABEL_APERIODIC_VALUE > cellParams_p->LBitLabelForAperiodic) || 
                (MAX_LBIT_LABEL_APERIODIC_VALUE < cellParams_p->LBitLabelForAperiodic))
        {
            lteWarning("Invalid Lbit Label [ %d] for aperiodic mode in [%s]\n",
                    cellParams_p->LBitLabelForAperiodic,__func__);
            return MAC_FAILURE;

        } 

    }
    /* - CQI_4.1 */
#ifdef TDD_CONFIG
    cellParams_p->subFrameAssign = configCellReq_p->subFrameAssign;
    cellParams_p->splSubFramePatterns = configCellReq_p->splSubFramePatterns;

    if ((0 == configCellReq_p->splSubFramePatterns) || 
            (5 == configCellReq_p->splSubFramePatterns))
    {
        dataForSpecialSubFrame = FALSE;
    }

    assignTddPdschTxMapOfDLHarqPointer(cellParams_p->subFrameAssign, internalCellIndex);
    setDLHarqHarqTimerList(internalCellIndex);

    /*End TDD changes */
#endif

    /* SRS_CHG */
    /* InitCellSpecificSRSConfig, shall only be called if srsConfigType is 
       SETUP */
    cellParams_p->srsConfigType = configCellReq_p->srsConfigType;
    cellParams_p->srsCommonSetupInfo = configCellReq_p->srsCommonSetupInfo;
    /*CA Changes start  */
    initSRSMgr(cellParams_p->srsConfigType, internalCellIndex);
    /*CA Changes end  */
    /*SPR 3600 Start*/
    cellParams_p->macEnable64QAM = configCellReq_p->puschConfigCommonInfo.enable64QAM;
    /*SPR 3600 End*/
    /* SRS_CHG */
    /* +- SPR 19066 */
    cellParams_p->contnFreeRachTimer = configCellReq_p->contnFreeRachTimer;
    /* ULA_CHG */
    /* FREQ_SELECTIVE CHG */
    cellParams_p->freqSelectiveSchUL = configCellReq_p->freqSelectiveSchUL;
    /* Create MemPool for storing MCSToPRBMapping */
    if(FREQUENCY_SELECTIVE_ON == cellParams_p->freqSelectiveSchUL)
    {
        /* The mem pool will be created only first time when cell was configured */ 
        if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
        {
            /* Start 128 UE: Memory Changes */
            createMemPool((sizeof (MCSToPrbMapInfo) * MAX_MCS_INDEX),(initParams_p->macMaxNumOfUeSupported)); 
            /* End 128 UE: Memory Changes */
        }
    }

    cellParams_p->sfnGap = configCellReq_p->sfnGap;
    cellParams_p->freqSelectiveSchDL = configCellReq_p->freqSelectiveSchDL;
    /* SPR 19310 +- */

    /*SPR_3061_FIX*/
    cellParams_p->pucchCQISINRThreshold = configCellReq_p->pucchCQISINRThreshold;
    cellParams_p->ulSyncLossTimer = configCellReq_p->ulSyncLossTimer;
    /*SPR_3061_FIX*/
    /* Derive Ns from nB */
    nB = configCellReq_p->nB;
    if (nB == 0)
    {
        cellParams_p->Ns = 4;
    }
    else if (nB == 1)
    {
        cellParams_p->Ns = 2;
    }
    else if (nB >= 2)
    {
        cellParams_p->Ns = 1;
    }
    /* SPR 19309 + */
    cellParams_p->cfi = configCellReq_p->cfi;
    /* SPR 19309 - */
    /* SPR 19310 + */
    cellParams_p->numEUL = configCellReq_p->numEUL;
    cellParams_p->sizeEUL = configCellReq_p->sizeEUL;
    /* SPR 19310 - */
/* + SPR 22903 + */
/* - SPR 22903 - */

    /* Populate the Paging Ocassion table */
    calculatePagingOccasion(cellParams_p->Ns,internalCellIndex);     

    /* Cell Start Stop Chg */
    if(TRUE == configCellReq_p->explicitStartRequired)
    {
        cellParams_p->macCellState = MAC_CONFIG_WITH_EXP_START;
    }
    else
    {
        cellParams_p->macCellState = MAC_CONFIG_WIHOUT_EXP_START;
    }
    /* FREQ_SELECTIVE CHG */
    /* ULA_CHG */

    /* QOS_CHG */
    qosUpdateConfigParam(initParams_p,cellParams_p->dlAvailableRBs,
            cellParams_p->ulAvailableRBs);
    /* QOS_CHG */

    /*CLPC Change Begin - CLPC_CHG*/
    cellParams_p->tpcRntiRange = configCellReq_p->tpcRntiRange;
    cellParams_p->maxTpcPucchRntiRange = (configCellReq_p->tpcRntiRange.endTpcRntiPucch 
            - configCellReq_p->tpcRntiRange.startTpcRntiPucch); 
    cellParams_p->maxTpcPuschRntiRange = (configCellReq_p->tpcRntiRange.endTpcRntiPusch 
            - configCellReq_p->tpcRntiRange.startTpcRntiPusch); 
    cellParams_p->pdschConfigCommonInfo = configCellReq_p->pdschConfigCommonInfo;
    cellParams_p->commonDLPowerControlInfo = configCellReq_p->commonDLPowerControlInfo;
    cellParams_p->commonULPowerControlInfo = configCellReq_p->commonULPowerControlInfo;
    cellParams_p->powerControlEnableInfo = configCellReq_p->powerControlEnableInfo;
    /* Possible values of alpha are 0,4,5,6,7,8,9 and 10. so if alpha comes
     * is less than 3 use index 0. However values 1,2 and 3 should not come 
     */
    if(cellParams_p->commonULPowerControlInfo.alpha <= 3)
    {
        /* ICIC changes start */
        macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] = 
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + 
             schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForPUSCH[0];
        /* ICIC changes end */
        macPathlossToTargetSINRMapForSpsPUSCH_p[internalCellIndex] = 
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
             schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForSpsPUSCH[0];
    }
    else if (cellParams_p->commonULPowerControlInfo.alpha <= 10)
    {
        /* ICIC changes start */
        macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] = \
                                                                  (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                                                                   schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForPUSCH\
                                                                  [cellParams_p->commonULPowerControlInfo.alpha - 3];
        /* ICIC changes end */
        macPathlossToTargetSINRMapForSpsPUSCH_p[internalCellIndex] = \
                                                                     (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                                                                      schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForSpsPUSCH
                                                                     [cellParams_p->commonULPowerControlInfo.alpha - 3];
    }
    else
    {
        return MAC_FAILURE;
    }

    if (configCellReq_p->isDynamicPdcchDisabled != 0)
    {
        cellParams_p->isDynamicPdcchDisabled = configCellReq_p->isDynamicPdcchDisabled;
    }
    /* !
     * \code
     *  - Saving the SPS Cell Info in cell structure
     *    with params received form RRC.
     * \endcode
     */
    if (TRUE == initParams_p->spsSysWideParam.enableSps)
    {
        /* SPS_CHG */
        /* !
         * \code
         * SPS-CRNTI range for semi-persistent scheduling is not configured
         * by RRC/RRM in cell setup this feature shall be turned off
         * \endcode
         */
        if (INVALID_SPS_CRNTI == configCellReq_p->spsCommonCellInfo.startSpsCRnti ||
                INVALID_SPS_CRNTI == configCellReq_p->spsCommonCellInfo.endSpsCRnti)
        {
            initParams_p->spsSysWideParam.enableSps = FALSE;
        }
        cellParams_p->spsCommonCellInfo.startSpsCRnti = 
            configCellReq_p->spsCommonCellInfo.startSpsCRnti;

        cellParams_p->spsCommonCellInfo.endSpsCRnti = 
            configCellReq_p->spsCommonCellInfo.endSpsCRnti;
    }
    /* If PRS is configured then saving paramters in 
     * cell structure and calculating required parameters.
     */   
    if (0 != prsParseConfigInfo.isPRSConfigured)
    {
        prsConfigInfo_p->isPRSConfigured = 1;
        prsConfigInfo_p->prsResInfo[prsConfigInfo_p->dlPRSRATPoliciesIndex].\
            prsAllowedWithICIC = 1;
        prsConfigInfo_p->prsBandwidth =
            prsParseConfigInfo.prsBandwidth;
        prsConfigInfo_p->prsSubframes =
            prsParseConfigInfo.prsSubframes;
        prsConfigInfo_p->prsTransmissionPower = 
            prsParseConfigInfo.prsTransmissionPower;

        initPRSMaps(prsParseConfigInfo,
                configCellReq_p->dlBandwidth,
                internalCellIndex);
    }

    updatePrbToSINRGroupIndexMap(
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->srsSubBandGroupSize,
            schParamsIndexUL_g[internalCellIndex], internalCellIndex);
    /* SPR 13143 Changes start */
    calculateDci3And3AShiftOffsets(internalCellIndex);
    /* SPR 13143 Changes end */


    LTE_MAC_UT_LOG(LOG_DETAIL,CELL_CONF,"Cell Configuration is OK !!!\n");

    LOG_MAC_MSG(RRC_MAC_CELL_CONFIG_ID,LOGINFO,MAC_RRC_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            TRUE,cellParams_p->ulAvailableRBs,
            cellParams_p->dlAvailableRBs,cellParams_p->startRARNTI,
            cellParams_p->endRARNTI,
            (UDouble32)cellParams_p->numOfTxAnteenas,
            (UDouble32 )cellParams_p->phichDuration,
            FUNCTION_NAME,"CELL_CONFIG_SUCCESS");
    return MAC_SUCCESS; /* SS_S2 */
}

/* CA Changes end */
/****************************************************************************
 * Function Name  : resetULPRBUsage
 * Inputs         : ulPRBUsageInfo_p - Pointer to ULPRBUsageStatistic and
 *                  currentTTI - value of Curent TTi
 * Outputs        : None
 * Returns        : None
 * Description    : Ref 36.314 Sec 4.1.1.1
 *                  Calculated Total PRB usage.
 ****************************************************************************/
 void resetULPRBUsage(
    ULPRBUsageStatistic *ulPRBUsageInfo_p, 
    /* SPR 15909 fix start */
    tickType_t currentTTI)
    /* SPR 15909 fix end */
{
    ulPRBUsageInfo_p->cumulativePRBsScheduled = 0;
    ulPRBUsageInfo_p->cumulativePRBsAvailable = 0;
    ulPRBUsageInfo_p->lastULPRBUsageCalcTTI = currentTTI;
}


/****************************************************************************
 * Function Name  : calculateULPRBUsage 
 * Inputs         : ulPRBUsageInfo_p - Pointer to ULPRBUsageStatistic and
 *                  currentTTI - value of Curent TTI
 * Outputs        : None
 * Returns        : None
 * Description    : Ref 36.314 Sec 4.1.1.1
 *                  Calculated Total PRB usage.
 ****************************************************************************/
 void calculateULPRBUsage(
    ULPRBUsageStatistic *ulPRBUsageInfo_p, 
    /* SPR 15909 fix start */
    tickType_t currentTTI)
    /* SPR 15909 fix end */
{
    ulPRBUsageInfo_p->totalPRBUsage = \
        (UDouble64)ulPRBUsageInfo_p->cumulativePRBsScheduled*100/ulPRBUsageInfo_p->cumulativePRBsAvailable;
    
    resetULPRBUsage(ulPRBUsageInfo_p, currentTTI);
}

/* SPR 13143 Changes start */
/*****************************************************************************
* Function Name  : calculateDci3And3AShiftOffsets
* Inputs         : None
* Outputs        : It will provide the correct position at which TPC command
*				   should be filled for pertcular UE
* Returns        : None
* Description    : It will calculate the bits need to ignore from MSB based 
*                  on Bandwidth used, i.e in case of 10Mhz, 6 MSB bits needs 
*                  to be ignored and TPC Index 1 should be filled on bit 25&26
*****************************************************************************/

 void calculateDci3And3AShiftOffsets(InternalCellIndex internalCellIndex)
{
     CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                      cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
   switch( cellParams_p->dlAvailableRBs)
   {
        case DL_RES_BLKS_6RBS:
        {
#ifdef FDD_CONFIG
            dci3RightShiftOffset_g[internalCellIndex] = 12;
            dci3ARightShiftOffset_g[internalCellIndex] = 11;
#else
            dci3RightShiftOffset_g[internalCellIndex] = 10;
            dci3ARightShiftOffset_g[internalCellIndex] = 9;
#endif
            break;
        }
        case DL_RES_BLKS_15RBS:
        {
#ifdef FDD_CONFIG
            dci3RightShiftOffset_g[internalCellIndex] = 10;
            dci3ARightShiftOffset_g[internalCellIndex] = 10;
#else
            dci3RightShiftOffset_g[internalCellIndex] = 8;
            dci3ARightShiftOffset_g[internalCellIndex] = 7;
#endif
            break;
        }
        case DL_RES_BLKS_25RBS:
        {
#ifdef FDD_CONFIG
            dci3RightShiftOffset_g[internalCellIndex] = 8;
            dci3ARightShiftOffset_g[internalCellIndex] = 7;
#else
            dci3RightShiftOffset_g[internalCellIndex] = 6;
            dci3ARightShiftOffset_g[internalCellIndex] = 5;
#endif
	    break;
        }
        case DL_RES_BLKS_50RBS:
        {
#ifdef FDD_CONFIG
            dci3RightShiftOffset_g[internalCellIndex] = 6;
            dci3ARightShiftOffset_g[internalCellIndex] = 5;
#else
            dci3RightShiftOffset_g[internalCellIndex] = 4;
            dci3ARightShiftOffset_g[internalCellIndex] = 3;
#endif
            break;
        }
        case DL_RES_BLKS_75RBS:
        {
#ifdef FDD_CONFIG
            dci3RightShiftOffset_g[internalCellIndex] = 6;
            dci3ARightShiftOffset_g[internalCellIndex] = 5;
#else
            dci3RightShiftOffset_g[internalCellIndex] = 2;
            dci3ARightShiftOffset_g[internalCellIndex] = 2;
#endif
            break;
        }
        case DL_RES_BLKS_100RBS:
        {
#ifdef FDD_CONFIG
            dci3RightShiftOffset_g[internalCellIndex] = 4;
            dci3ARightShiftOffset_g[internalCellIndex] = 4;
#else
            dci3RightShiftOffset_g[internalCellIndex] = 2;
            dci3ARightShiftOffset_g[internalCellIndex] = 1;
#endif
            break;
        }
    }
}
/* SPR 13143 Changes end */

