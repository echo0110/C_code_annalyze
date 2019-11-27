/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPhyInterface.h,v 1.25 2009/12/01 15:37:37 gur19413 Exp $
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
 *
 ****************************************************************************/

#ifndef LTE_LAYER2_TDDMGR_H
#define LTE_LAYER2_TDDMGR_H 

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteTypes.h"


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct Dci0HiPduT
{
  UInt16 dci0SFN;
  UInt8  dci0SF;
  UInt8  dciQIdx;
  UInt16 hiSFN;
  UInt8  hiSF;
  UInt8  hiQIdx;
}Dci0HiPdu;

// Refer TS 36.211, table 5.7.1-3
typedef struct TddRachConfigInfoT {
    UInt8 preambleFormat;
    UInt8 prachDensity;     // range 0.5 - 6, 0.5 is stored as 1 (and considered so too)
} TddRachConfigInfo;

/* RachResLocationQuad: TS 36.211, section 5.7.1 */
typedef struct RachResLocationQuadT {
    UInt8 fra;
    UInt8 t0ra;
    UInt8 t1ra;
    UInt8 t2ra;
} RachResLocationQuad;

extern const TddRachConfigInfo TddRachConfigInfo_gc[];
extern UInt8 DaiIndexForSource_gc[7][MAX_SUBFRAME];
extern UInt8 *daiTrackIndexForSource_gp;

#if 0
typedef struct DaiTrackingRegisterT
{
    UInt8 daiCount;         // Counter to keep track of received data
    UInt8 lastReceivedDai;  // DAI value received in last non-zero DCI format info
        //Used for the cases when both UL and DL of a UE are active
    UInt8 daiUpdated;
} DaiTrackingRegister;
#endif
/* CA TDD Changes Start */
extern UInt8 tddUlDlConfig_g[MAX_NUM_CELLS];
/* CA TDD Changes End */
/*SPR 13884 changes start */        
extern UInt32 tddAckNackval_g[8];   
/*SPR 13884 changes end */          
extern const char UlDlConfig_gc[7][MAX_SUBFRAME];
/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define MAC_TDD_DL_ADVANCE_1_PHY_DELAY_2

#ifdef MAC_TDD_DL_ADVANCE_2_PHY_DELAY_1
#define MAC_ADVANCE_TTI 2
#define PEER_MAC_DELAY 1
#endif

#ifdef MAC_TDD_DL_ADVANCE_1_PHY_DELAY_2
#define MAC_ADVANCE_TTI 1
#ifdef OTA_DL_ADVANCE_2
#define PEER_MAC_DELAY 1
#else
#define PEER_MAC_DELAY 2
#endif
#endif

#ifdef MAC_TDD_DL_ADVANCE_1_PHY_DELAY_1
#define MAC_ADVANCE_TTI 1
#define PEER_MAC_DELAY 1
#endif

/* SPR 609 changes start */
#ifndef PEER_MAC_ADVANCE
/* SPR 609 changes end */
#define PEER_MAC_ADVANCE MAC_ADVANCE_TTI
/* SPR 609 changes start */
#endif
/* SPR 609 changes endif */

#define MAX_UL_ARRAY     80
#define MAX_UL_DL_CONFIG 7
#define MAX_SUB_FRAME_PER_RADIO_FRAME 10240
#define MAX_RADIO_FRAME_FOR_DL_ACK_NACK  8
#define MAX_NO_TB                        2
#define TB1_INDEX                       0
#define TB2_INDEX                       1
#define     MAX_HALF_SUBFRAME       (MAX_SUBFRAME / 2)

/* CA TDD Changes Start */
#define IS_UL_SUBFRAME(x, internalCellIndex)   (UlDlConfig_gc[tddUlDlConfig_g[internalCellIndex]][(x)] == 'U')
#define IS_DL_SUBFRAME(x, internalCellIndex)   (UlDlConfig_gc[tddUlDlConfig_g[internalCellIndex]][(x)] == 'D')
#define IS_SPL_SUBFRAME(x, internalCellIndex)  (UlDlConfig_gc[tddUlDlConfig_g[internalCellIndex]][(x)] == 'S')
/* CA TDD Changes End */

#ifndef NA
    #define NA (-1)
    #endif  // ifndef NA

/* +    SPR 11297 changes start */
#define MAX_TDD_RACH_DENSITY    7
/* Denotes '*' for S-SUBFRAME for PRACH format 4 as in table 5-7-1.4
 * in 36.213 */
#define SSF MAX_TDD_RACH_DENSITY + 1 
#define SF_S_BASE               1
#define MAX_PRACH_CONFIGS       58  // Max number of PRACH config indices (0-47)
/* -    SPR 11297 changes end */
#define SF_UL_BASE              2   // Start location of UL subframes in a TDD
                                    // half subframe (for all configs)
                                    //
#define     MAX_UL_HARQ_TIMER_ARRAY 20

/* qIndex represent the Queue Index correspondence the SubFrame */
#define GET_QUEUE_IDX(x)  ((SFN_g[x] * MAX_SUBFRAME) + SF_g[x] ) % MAX_UL_ARRAY;

extern UInt8 dlSfForUlAckNack_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME];
/*SPR 2984 Fix*/
typedef struct SfForRachULConfigT
{
    UInt8 ulGrantSfForMsg3;
    UInt8 containerIndex;
}SfForRachULConfig;

/* CA TDD Changes Start */
inline UInt8 getMsg3SFandQIndex( UInt16 *sendSFN,
                                 UInt8  *sendSF,
                                 UInt8 cellIndex);
inline UInt8 getSfForMsg3 (UInt8 subframeNum, UInt8 cellIndex);
/* CA TDD Changes End */
/*SPR 2984 Fix*/
#endif
