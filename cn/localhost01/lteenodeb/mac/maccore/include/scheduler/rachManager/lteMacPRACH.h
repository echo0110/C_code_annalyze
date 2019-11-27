/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPRACH.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function declarations, includes
 *                     and exported variables for PRACH.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPRACH.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.4  2009/08/27 09:59:38  gur20439
 * SIT bug fix for test case MAC_SIT_FUNC_26.
 *
 * Revision 1.3  2009/07/13 12:06:46  gur15697
 * PRACH_MGR added
 *
 * Revision 1.2  2009/07/02 08:04:25  gur15697
 * NUM_PRACH_PHYSICAL_RESOURCES  changed to 6
 *
 * Revision 1.1  2009/06/08 08:52:17  gur15697
 * Initial vertion
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PRACH_H
#define LTE_MAC_PRACH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteLayer2CommanTypes.h"
#include "lteMacRandomAccess.h"
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

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#ifdef LOG_PRINT_ENABLED

/* SPR 15909 fix start */
/* + SPR 17439 */
extern tickType_t getCurrentTick(void);
/* - SPR 17439 */
/* SPR 15909 fix end */

#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#define PRACH_MGR "prach_mgr"

/*As per 36.211 Table 5.7.1-2 "Frame structure type 1 random access 
 *configuration for preamble format 0-3" PRACH Configuration Index
 *value is 0 - 63
 */
#define MAX_PRACH_CONFIGURATION_INDEX 64
#define MAX_PRACH_FREQUENCY_OFFSET    94

/*Number of physical resources to be allocated in a subframe*/
#define NUM_PRACH_PHYSICAL_RESOURCES  6

#define PREAMBLE_GROUPB_MSG_SIZE_DELTA 120

#define DEFAULT_PREAMBLE_GROUPA_MSG_SIZE PREAMBLE_GROUPA_MSG_SIZE_B56 +\
                                            PREAMBLE_GROUPB_MSG_SIZE_DELTA

#define CON_FREE_HAND_OVER_GRANT_BYTES  14                                        
#define CON_FREE_PDCCH_ORDER_GRANT_BYTES  19 

typedef enum PRACHSysFrameNumT
{
    EVEN = 0,
    ANY,
    INVALID_PRACH_SFN
}PRACHSysFrameNum;

#ifdef TDD_CONFIG
typedef enum PRACHSysFrameNoT
{
    SFN_ALL  = 0,
    SFN_EVEN  ,
    SFN_ODD   ,
    SFN_INVALID
}PRACHSysFrameNo;

typedef enum PRACHSubFrameNumT
{
    SF_INVALID =0,
    SF_VALID 
}PRACHSubFrameNum;
#endif

/* Enum to define Preamble formats. 
   For frame structure type 1 preamble format is 0 - 3 
   For frame structure type 2 preamble format is 0 - 4 */
typedef enum PreambleFormatT
{
    PREAMBLE_FORMAT_0 = 0,
    PREAMBLE_FORMAT_1,
    PREAMBLE_FORMAT_2,
    PREAMBLE_FORMAT_3,
#ifdef TDD_CONFIG
    PREAMBLE_FORMAT_4,
#endif
    INVALID_PREAMBLE_FORMAT
}PreambleFormat;

typedef enum RARspWindowSizeT
{
    RA_RSP_WINDOW_SIZE_2 = 2,
    RA_RSP_WINDOW_SIZE_3 = 3,
    RA_RSP_WINDOW_SIZE_4 = 4,
    RA_RSP_WINDOW_SIZE_5 = 5,
    RA_RSP_WINDOW_SIZE_6 = 6,
    RA_RSP_WINDOW_SIZE_7 = 7,
    RA_RSP_WINDOW_SIZE_8 = 8,
    RA_RSP_WINDOW_SIZE_10 = 10
}RARspWindowSize;

typedef enum MacContentionResTimerT
{
    CONTENTION_RES_TIMER_8  = 8,
    CONTENTION_RES_TIMER_16 = 16,
    CONTENTION_RES_TIMER_24 = 24,
    CONTENTION_RES_TIMER_32 = 32,
    CONTENTION_RES_TIMER_40 = 40,
    CONTENTION_RES_TIMER_48 = 48,
    CONTENTION_RES_TIMER_56 = 56,
    CONTENTION_RES_TIMER_64 = 64
}MacContentionResTimer;

/*Don't change the order of structure elements*/
typedef struct PRACHConfigInfoT
{
    PreambleFormat   preambleFormat;
    PRACHSysFrameNum prachSFN;
    UInt8 subFrame[MAX_SUB_FRAME];
}PRACHConfigInfo;


#ifdef TDD_CONFIG
typedef struct  PRACHResT
{ 
	UInt8   ValidityFlag;        
	UInt8   FreqResIndx;
	UInt8   PRACHResCount;
}PRACHResInfo;
#endif

#define IS_EVEN(x) (MODULO_TWO(x)?(0):(1))

#ifdef TDD_CONFIG
#define IS_ODD(x) (MODULO_TWO(x))
#endif

#define UPDATE_CONTENTION_RES_TIMER_ACTIVE_INDEX(x,s)\
{\
    if (x.rachConfiguration.contentionResUpdateFlag)\
    {\
        if (s == x.sfn)\
        {\
            x.rachConfiguration.activeIdxContentionRes = \
            ((x.rachConfiguration.activeIdxContentionRes)?(0):(1));\
        }\
    }\
}

#define UPDATE_RAR_WINDOW_SIZE_ACTIVE_INDEX(x,s)\
{\
    if ((x).rachConfiguration.raRspWindowUpdateFlag)\
    {\
        if (s == (x).sfn)\
        {\
            (x).rachConfiguration.activeIdxRARspWindow = \
              (((x).rachConfiguration.activeIdxRARspWindow)?(0):(1));\
        }\
    }\
}

#define UPDATE_PRACH_CONFIGURATION_ACTIVE_INDEX(x,s)\
{\
    if (x.prachUpdateFlag)\
    {\
        if (s == x.sfn)\
        {\
            x.prachActiveIndex = ((x.prachActiveIndex)?(0):(1));\
	    x.prachUpdateFlag = FALSE;\
        }\
    }\
}

#define FILL_RAR_SUB_HEADER(sh_p, preamble, flagE)\
{\
    if (flagE)\
    {\
        *(sh_p) = ((preamble &(0x3F))|(0xC0));\
    }\
    else\
    {\
        *(sh_p) =  (0x40);\
        *(sh_p) |=  (preamble & (0x3F));\
    }\
}

/*****************************************************************************
 * Function Name  : preambleFormatFrmPrachConfigIndex
 * Inputs         : prachConfigIndex in case of TDD only
 *                  internalCellIndex
 * Outputs        : None 
 * Returns        : preamble format corresponding to PRACH Config Index
 * Description    : This function will return the Preamble format corresponding
 *                  to PRACH Config Index
 *****************************************************************************/
 /* +- SPR 17777 */
#ifdef FDD_CONFIG
PreambleFormat preambleFormatFrmPrachConfigIndex(InternalCellIndex internalCellIndex);
#else
    PreambleFormat preambleFormatFrmPrachConfigIndex(UInt8 prachConfigIndex);
#endif 
 /* +- SPR 17777 */

#ifdef TDD_CONFIG
/*****************************************************************************
 * Following function will update the Ul RB map for PRACH resources wrt TDD 
 * UL/DL config 0 - 6
 *****************************************************************************/
void updateUplinkRbMapForPrach( UInt8 prachConfigIndex,
    UInt32  NraOffset, UInt32 rbMapNum, UInt32 ulSubFrame, 
    InternalCellIndex internalCellIndex );

/*****************************************************************************
 * Following function will return if Prach Config Index received is valid wrt
 * TDD UL/DL config 0 - 6
 *****************************************************************************/
UInt8 isPrachConfigValid( UInt8 prachConfigIndex, UInt8 tddConfig );

/* SPR 11489 fix start */
/* Following function validates if the number of RBs required for PRACH can be
 * allocated for PRACH configuration Index for all TDD UL/DL config 0 - 6 at
 * the time of cell setup only. */
MacRetType validateFraWrtResBlks( UInt8 ulResBlocks, UInt8 subFrameAssign,
    UInt8 prachConfigIndex, UInt8 prachFrequencyOffset );
/* SPR 11489 fix end */
#endif

#endif  /* LTE_MAC_PRACH_H */
