#ifdef TDD_CONFIG
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacTddMgr.c,v 1.1.2.26 2010/10/28 15:07:35 gur19413 Exp $
 *
 ******************************************************************************
 *
 *  File Description :
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacTddMgr.c,v $
 * Revision 1.1.2.26  2010/10/28 15:07:35  gur19413
 * Support for DL Advance and PHY delay
 *
 * Revision 1.1.2.25  2010/10/26 11:41:16  gur19413
 * number of count changes in ul/dl config 1
 *
 * Revision 1.1.2.24  2010/10/26 09:30:16  gur19413
 * removed PROCESS_DEMUX_QUEUE
 *
 * Revision 1.1.2.23  2010/10/26 08:15:26  gur23792
 * Modified Execution Legs to support different special sub frame patterns
 *
 * Revision 1.1.2.22  2010/10/26 06:06:15  gur24420
 * Updated the execution legs for strategy
 *
 * Revision 1.1.2.21  2010/10/25 13:11:57  gur19413
 * Self Review fixes
 *
 * Revision 1.1.2.20  2010/10/25 11:35:45  gur24420
 * Added pointer to hold Ack/Nack Handling function for current UL/DL configuration
 *
 * Revision 1.1.2.19  2010/10/25 09:25:33  gur24420
 * "cosmetic changes"
 *
 * Revision 1.1.2.18  2010/10/23 12:41:33  gur19413
 * self review fixes
 *
 * Revision 1.1.2.17  2010/10/21 12:23:56  gur24420
 * 1.Comments updated in TDD manager file.
 * 2.DL Strategy function call during the special subframe is
 *   handeled based on Special subframe configuration.
 *
 * Revision 1.1.2.16  2010/10/18 09:22:22  gur19413
 * Log file creation moved under UT flag
 *
 * Revision 1.1.2.15  2010/10/18 08:59:28  gur19413
 * X+2 fixes
 *
 * Revision 1.1.2.14  2010/10/18 07:01:01  gur23792
 * *** empty log message ***
 *
 * Revision 1.1.2.13  2010/10/18 06:49:45  gur23792
 * *** empty log message ***
 *
 * Revision 1.1.2.12  2010/09/29 07:25:01  gur24420
 * Bugs fixed in UT
 *
 * Revision 1.1.2.11  2010/09/23 13:17:45  gur24420
 * Bug fixes during IT
 *
 * Revision 1.1.2.10  2010/09/23 05:27:43  gur24420
 * cosmetic changes
 *
 * Revision 1.1.2.9  2010/09/20 14:58:10  gur19413
 * bug fixed
 *
 * Revision 1.1.2.8  2010/09/20 13:54:07  gur19413
 * bug fixed
 *
 * Revision 1.1.2.7  2010/09/09 19:04:24  gur24420
 * Testing
 *
 * Revision 1.1.2.6  2010/09/09 11:53:16  gur24420
 * TDD Manager Sync up changes
 *
 * Revision 1.1.2.5  2010/09/07 20:04:11  gur19413
 * removed erros
 *
 * Revision 1.1.2.4  2010/09/07 13:27:33  gur24420
 * Updated Initilaization of executon flow pointer
 *
 * Revision 1.1.2.3  2010/09/07 11:08:48  gur24420
 * Review changes for DL
 *
 * Revision 1.1.2.2  2010/08/31 08:10:17  gur19413
 * convert dos2unix added FDD_CONFIG flag
 *
 * Revision 1.1.2.1  2010/08/27 13:06:14  gur24420
 * Added New file
 *
 *
 *****************************************************************************/

#include "lteMacTddMgr.h"
#include "lteMacExecutionLegs.h"
#include "lteMacReceiver.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacTDDConfig.h"

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/

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
/*FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/*SPR 13047 fix*/
extern UInt8 firstTickRecvFlag_g[MAX_NUM_CELL];
extern UInt8 startExecutionAfterPhyStart_g[MAX_NUM_CELL];
#endif
/*FAPI L2 AS MASTER END */

extern volatile UInt32 stoppedProcessing_g;
/* TDD One Tick Pdu */
/*SPR# 3107 Changes Starts */
#ifdef UTFWK_SIMULATION
extern UInt16 noOfTickToBeProcess_g;
extern UInt8 recvFromOamDone_g; 
#endif
/*SPR# 3107 Changes Ends */
/* SPR 13768 Changes Start */
extern ExecutionFlowCreationParam *executionFlowCreationParamArr_g[MAX_NUM_CELL];
/* SPR 13768 Changes End */

extern volatile UInt8 phyCellConfigured_g; 
void populateTddELSchedulerArray(CellConfigParams *cellParams_p, InternalCellIndex internalCellIndex );

#if (defined(DL_UL_SPLIT_TDD) && defined(FAPI_CONTROL_FLOW))
sem_t waitForFAPIMsgControlFlowInd_g;
#endif

extern void ElProcessOAMAndRRCMsgFSM(UInt32 delayToPhy, UInt8* recvBuffer_p, UInt16 byteRead);

extern UInt8 waitFromStateEvent_g ;
/* SPR 5147 changes start */
/* SPR 3227 START */
extern UInt32 phyStarted_g; 
/* SPR 3227 END */
/* SPR 5147 changes end */
//SYNC Change start
//UInt16              macIterationCounter_g = 0;
//UInt32      signalToPdcp_g;
//UInt32     mutexToSyncPdcp_g;
//SYNC Change end
/*SPR 16855 +-*/
extern SEM_T waitForCellSetupInd_g;
/** SPR 14079 Changes Start **/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
extern SEM_T waitForCellSetupIndSemArr_g[NUM_OF_EXEC_FLOWS * MAX_NUM_CELL];
#endif
/** SPR 14079 Changes End **/
/*SPR 16855 +-*/

/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
#ifdef DL_UL_SPLIT_TDD
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
#define NUM_OF_THREADS ( NUM_OF_EXEC_FLOWS + 1 )                      /* +1 for RRC/OAM thread */

__thread UInt64 prof_start_count_split_g[NUM_OF_THREADS], prof_end_count_split_g[NUM_OF_THREADS];
__thread UInt64 prof_cycle_count_used_split_g[NUM_OF_THREADS];
__thread TimeSpec prof_start_clock_split_g[NUM_OF_THREADS],prof_end_clock_split_g[NUM_OF_THREADS];
#endif
#endif
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
void executionFlowThreadForTDDMAC(ExecutionFlowArguments *execFlowThreadArgs_p);
/*TDD Tick overshoot changes start*/
#ifdef OVERSHOOT_CORRECTION
/** CA2 Changes Start **/
void setOverLoadLevel(OverLoadLevel tickOverLoadLevel,InternalCellIndex cellIndex);
/** CA2 Changes End **/
#endif
/*TDD Tick overshoot changes end*/
static void  updateElArgs(
    ExecutionLegInfo * elArgTypesSrc, 
    ExecutionLegInfo * elArgsDest, 
    UInt8 airSf,
    InternalCellIndex internalCellIndex);

/*
 *  The following is the 3-dimentional Array which is initialised with 
 *  appropriate Index to PDCCH_PDSCH function pointer Table for each D and S
 *  subframes of all UL/DL config and Special Subframe configuration.
 *  The below table is used when the PDCCH_PDSCH leg is executed to call
 *  appropriate function from the PDCCH_PDSCH function pointer Table.
 */
UInt32 tddMacPdcchPdschFtnPtrIndexer
        [MAX_SPECIAL_SF_CONFIG][MAX_UL_DL_CONFIG][MAX_SUB_FRAME] = 
{
	/* Special SF 0 */
	{
		/*UL-DL Config 0 */
		{
            
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_UL_ONLY,                  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_UL_ONLY,                  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA                              /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_UL_ONLY,                  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_UL_ONLY,                  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DUMMY,                    /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DUMMY,                    /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */
			//PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DUMMY,                    /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DUMMY,                    /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DUMMY,	                /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_UL_ONLY,                  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_UL_ONLY,                  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 1 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
            /* SPR 10896 fix start */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
            /* SPR 10896 fix end */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
            /* SPR 10896 fix start */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
            /* SPR 10896 fix end */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA  	 /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
            PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
            PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 2 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA                              /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 3 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA                              /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 4 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA                              /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 5 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_UL_ONLY,                  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_UL_ONLY,                  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA       /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_UL_ONLY,		            /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_UL_ONLY,		            /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DUMMY,                    /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DUMMY,                    /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DUMMY,                    /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DUMMY,                    /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DUMMY,                    /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_UL_ONLY,                  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_UL_ONLY,		            /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 6 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
            /* SPR 10896 fix start */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
            /* SPR 10896 fix end */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA                              /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 7 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA                              /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	},
    /* Special SF 8 */
	{
		/*UL-DL Config 0 */
		{
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			NA 	   /* Subframe Number 9 */
		},
		/*UL-DL Config 1 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,	/* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 2 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 9 */

		},
		/*UL-DL Config 3 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */ 
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */ 
			NA,                             /* Subframe Number 2 */ 
			NA,                             /* Subframe Number 3 */ 
			NA,                             /* Subframe Number 4 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */ 
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */ 
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */
		},
		/*UL-DL Config 4 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		},
		/*UL-DL Config 5 */
		{

			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 0 */
			PDCCH_DL_ONLY_PDSCH_TRUNCATED,  /* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 3 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 4 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 5 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 6 */
			PDCCH_DL_ONLY_PDSCH_NORMAL,     /* Subframe Number 7 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 8 */
			PDCCH_DL_ONLY_PDSCH_NORMAL      /* Subframe Number 9 */

		},
		/*UL-DL Config 6 */
		{

			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 0 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 1 */
			NA,                             /* Subframe Number 2 */
			NA,                             /* Subframe Number 3 */
			NA,                             /* Subframe Number 4 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL,   /* Subframe Number 5 */
			PDCCH_DL_AND_UL_PDSCH_TRUNCATED,/* Subframe Number 6 */
			NA,                             /* Subframe Number 7 */
			NA,                             /* Subframe Number 8 */
			PDCCH_DL_AND_UL_PDSCH_NORMAL    /* Subframe Number 9 */

		}
	}
};

/*
 * Pointer to hold the PDCCH and PDSCH indexer for current 
 * UL/DL config and Special Subframe configuration configured by 
 * RRC
 */
/** CA-TDD Changes Start **/
UInt32 *tddMacPdcchPdschFtnPtrIndexer_p[MAX_NUM_CELL];
/** CA-TDD Changes End **/

#ifdef LTE_EMBMS_SUPPORTED
UInt32 tddMacEmbmsFtnPtrIndexer [MAX_UL_DL_CONFIG][MAX_SUB_FRAME] = 
{
	/*UL-DL Config 0 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		NA,                           /* Subframe Number 3 */
		NA,                           /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		NA,                           /* Subframe Number 7 */
		NA,                           /* Subframe Number 8 */
		NA                            /* Subframe Number 9 */
	},
	/*UL-DL Config 1 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		NA,                           /* Subframe Number 3 */
		EMBMS_SCH,                             /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		NA,                           /* Subframe Number 7 */
		NA,                           /* Subframe Number 8 */
		EMBMS_SCH                              /* Subframe Number 9 */
	},
	/*UL-DL Config 2 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		EMBMS_SCH,                             /* Subframe Number 3 */
		EMBMS_SCH,                             /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		NA,                           /* Subframe Number 7 */
		EMBMS_SCH,                             /* Subframe Number 8 */
		EMBMS_SCH                              /* Subframe Number 9 */
	},
	/*UL-DL Config 3 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		NA,                           /* Subframe Number 3 */
		NA,                           /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		EMBMS_SCH,                             /* Subframe Number 7 */
		EMBMS_SCH,                             /* Subframe Number 8 */
		EMBMS_SCH                              /* Subframe Number 9 */
	},
	/*UL-DL Config 4 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		NA,                           /* Subframe Number 3 */
		EMBMS_SCH,                             /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		EMBMS_SCH,                             /* Subframe Number 7 */
		EMBMS_SCH,                             /* Subframe Number 8 */
		EMBMS_SCH                              /* Subframe Number 9 */
	},
	/*UL-DL Config 5 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		EMBMS_SCH,                             /* Subframe Number 3 */
		EMBMS_SCH,                             /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		EMBMS_SCH,                             /* Subframe Number 7 */
		EMBMS_SCH,                             /* Subframe Number 8 */
		EMBMS_SCH                              /* Subframe Number 9 */
	},
	/*UL-DL Config 6 */
	{
		EMBMS_DUMMY,                           /* Subframe Number 0 */
		EMBMS_DUMMY,                           /* Subframe Number 1 */
		NA,                           /* Subframe Number 2 */
		NA,                           /* Subframe Number 3 */
		NA,                           /* Subframe Number 4 */
		EMBMS_DUMMY,                           /* Subframe Number 5 */
		EMBMS_DUMMY,                           /* Subframe Number 6 */
		NA,                           /* Subframe Number 7 */
		NA,                           /* Subframe Number 8 */
		EMBMS_SCH                              /* Subframe Number 9 */
	},
};

UInt32 *tddMacEmbmsFtnPtrIndexer_p[MAX_NUM_CELL]; 
#endif


/* 
 * Below is the dummy execution leg, passed as an arguement during 
 * execuion flow thread creation, and the arguement will be updated 
 * after cell configuration for the configured UL-DL config and Special 
 * Subframe config. 
 */
#ifdef DL_UL_SPLIT_TDD

ExecutionFlowCreationParam executionFlowCreationParamArrTDDConfigDefault_g[NUM_OF_MAC_EXEC_FLOWS]
= {
    {
        1, /*Thread id*/
        99, /* thread priority */
        /*43,  thread priority */
        {
            INIT_DL_THREAD_PARAMS,
            0,  /* Thread bind mask */
            1,  /* Thread Index*/
            10, /* Max Sub-Frame Loop count */
            2,  /* Core Id*/
            { 
            }
        } 
    },
    {
        2, /*Thread id*/
        99, /* thread priority */
        /*43,  thread priority */
        {
            INIT_UL_THREAD_PARAMS,
            0,  /* Thread bind mask */
            2,  /* Thread Index*/
            10, /* Max Sub-Frame Loop count */
            3,  /* Core Id*/
            { 
            }
        } 
    }
};

#else
/** CA-TDD Changes Start **/
ExecutionFlowCreationParam executionFlowCreationParamArrTDDConfigDefault_g[MAX_NUM_CELL][NUM_OF_EXEC_FLOWS]
/** CA-TDD Changes End **/
= {
    {{
        0, /*Thread id*/
        0, /* thread priority */
        {
            REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC,
            0, /* Thread bind mask */
            10, /* Max Sub-Frame Loop count */
            0,
            { 
                {
                    0
                }
            },
            0
        } 
    }}
};
/** CA-TDD Changes Start:06 **/
ElArguments elArgsTdd_g[MAX_NUM_CELL][MAX_SUB_FRAME];
/** CA-TDD Changes End:06 **/
#endif


SchedulerFunctionalitySet SchFnSet_g[FN_MAX] = 
{
    /* FN_DL_ALLOC_ONLY */
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            /* + SPS_TDD_Changes */
            {PROCESS_DL_SPS_STRATEGY, ELARG_DL_SF_TO_WORK_FOR,0,0},
            /* - SPS_TDD_Changes */
            /*+ SPR 17439*/            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_STRATEGY_FD,0,0,0},
            {PROCESS_CHECK_STRATEGY_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
#ifdef LTE_EMBMS_SUPPORTED
            {PROCESS_EMBMS_STRATEGY,ELARG_DL_SF_TO_WORK_FOR,0,0 },
#endif            
            {PROCESS_DL_SCH_STRATEGY, ELARG_DL_SF_TO_WORK_FOR,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_PDCCH_PDSCH_FD,0,0,0},
            {PROCESS_PRS_TRANSMISSION,0,0,0},  /* PRS_CHANGES */
            {PROCESS_CHECK_PDCCH_PDSCH_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* TDD Config 0 Changes Start */
            {PROCESS_PDCCH_PDSCH_WITH_MUX, ELARG_DL_SF_TO_WORK_FOR, ELARG_DL_HARQ_ACKNACK_U_SF, ELARG_UL_SF_TO_WORK_FOR},
            /* TDD Config 0 Changes End */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_MUX_FD,0,0,0},
            {PROCESS_CHECK_MUX_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_MUX_QUEUE,0,0,0},
#ifndef DL_DATA_SEPARATION            
            {RLC_HANDLE_TX_OPPORTUNITY,0,0,0},
#endif            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            
            {PROCESS_SET_ENCODER_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
#ifndef DL_DATA_SEPARATION            
            {PROCESS_ENCODER_QUEUE_WITHOUT_TX,0,0,0},
#endif            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_CHECK_ENCODER_FD,0,0,0},
            {PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {TX_DL_CONTROL_MSG,0,0,0},
#if (defined(DL_UL_SPLIT_TDD) && defined(FAPI_CONTROL_FLOW))
            {FAPI_DL_FLOW_SYNC,0,0,0},
#endif
#ifndef DL_DATA_SEPARATION            
            {TX_DL_DATA_MSG,0,0,0},
#else
            {RLC_UPDATE_BUFF_OCC_TO_MAC,0,0,0},
            {PROCESS_DL_SYNC_TX_DATA,0,0,0},
#endif
            /* + SPS_TDD_Changes */
            {PROCESS_SPS_PDSCH, ELARG_DL_SF_TO_WORK_FOR,0,0},
            /* - SPS_TDD_Changes */
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* FN_UL_ALLOC_ONLY */
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            /* + SPS_TDD_Changes */
            {PROCESS_UL_SPS_STRATEGY,0,0,0},
            /* - SPS_TDD_Changes */
            /* TDD Config 0 changes start */
            {PROCESS_UL_SCH_STRATEGY, ELARG_UL_SF_TO_WORK_FOR,0,0},
            /* TDD Config 0 changes end */
#ifndef DL_UL_SPLIT_TDD
            /* TDD Config 0 Changes Start */
            {PROCESS_PDCCH_PDSCH_WITH_MUX, ELARG_DL_SF_TO_WORK_FOR, ELARG_DL_HARQ_ACKNACK_U_SF, ELARG_UL_SF_TO_WORK_FOR},
            /* TDD Config 0 Changes End */
#else
            {PROCESS_UL_PDCCH_WITH_MUX, ELARG_DL_SF_TO_WORK_FOR, ELARG_DL_HARQ_ACKNACK_U_SF,0},
#endif
            /* TDD Config 0 changes start */
            /* PUSCH EL will be passed two ulSubfarme.
             * 2nd ulSubframe will be valid only for TDD Config 0 */
            {PROCESS_PUSCH_FOR_DELAY_ONE, ELARG_UL_SF_TO_WORK_FOR, ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR,0},
            /* TDD Config 0 changes end */
#ifndef DL_UL_SPLIT_TDD
            {TX_DL_CONTROL_MSG,0,0,0},
            /* + SPS_TDD_Changes */
            {PROCESS_SPS_PUSCH,0,0,0},
            /* - SPS_TDD_Changes */
#else
#ifdef FAPI_CONTROL_FLOW
            {FAPI_UL_FLOW_SYNC,0,0,0},
#endif
            {TX_HI_DCI0_MSG,0,0,0},
#endif            
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
#ifndef DL_UL_SPLIT_TDD
    /* FN_DL_UL_ALLOC */
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* + SPS_TDD_Changes */
            {PROCESS_DL_SPS_STRATEGY, ELARG_DL_SF_TO_WORK_FOR,0,0},
            /* - SPS_TDD_Changes */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_STRATEGY_FD,0,0,0},
            {PROCESS_CHECK_STRATEGY_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
#ifdef LTE_EMBMS_SUPPORTED
            {PROCESS_EMBMS_STRATEGY,ELARG_DL_SF_TO_WORK_FOR,0,0 },
#endif 
            {PROCESS_DL_SCH_STRATEGY, ELARG_DL_SF_TO_WORK_FOR,0,0},
            /* + SPS_TDD_Changes */
            {PROCESS_UL_SPS_STRATEGY,0,0,0},
            /* - SPS_TDD_Changes */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_PDCCH_PDSCH_FD,0,0,0},
            /* TDD Config 0 changes start */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_UL_SCH_STRATEGY, ELARG_UL_SF_TO_WORK_FOR,0,0},
            /* TDD Config 0 changes end */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_PRS_TRANSMISSION,0,0,0},  /* PRS_CHANGES */
            {PROCESS_CHECK_PDCCH_PDSCH_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* TDD Config 0 Changes Start */
            {PROCESS_PDCCH_PDSCH_WITH_MUX, ELARG_DL_SF_TO_WORK_FOR, ELARG_DL_HARQ_ACKNACK_U_SF, ELARG_UL_SF_TO_WORK_FOR},
            /* TDD Config 0 Changes End */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_MUX_FD,0,0,0},
            {PROCESS_CHECK_MUX_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_MUX_QUEUE,0,0,0},
#ifndef DL_DATA_SEPARATION            
            {RLC_HANDLE_TX_OPPORTUNITY,0,0,0},
#endif            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_ENCODER_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* TDD Config 0 changes start */
            /* PUSCH EL will be passed two ulSubfarme.
             * 2nd ulSubframe will be valid only for TDD Config 0 */
            {PROCESS_PUSCH_FOR_DELAY_ONE, ELARG_UL_SF_TO_WORK_FOR, ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR,0},
            /* TDD Config 0 changes end */
#ifndef DL_DATA_SEPARATION            
            {PROCESS_ENCODER_QUEUE_WITHOUT_TX,0,0,0},
#endif            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_CHECK_ENCODER_FD,0,0,0},
            {PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {TX_DL_CONTROL_MSG,0,0,0},
#ifndef DL_DATA_SEPARATION            
            {TX_DL_DATA_MSG,0,0,0},
#else
            {RLC_UPDATE_BUFF_OCC_TO_MAC,0,0,0},
            {PROCESS_DL_SYNC_TX_DATA,0,0,0},
#endif
            /* + SPS_TDD_Changes */
/* SPR 23393 Changes Start */            
#ifdef INTEL_SPS            
/* SPR 23393 Changes End */
            {PROCESS_SPS_PDSCH, ELARG_DL_SF_TO_WORK_FOR,0,0},
            {PROCESS_SPS_PUSCH,0,0,0},
/* SPR 23393 Changes Start */            
#endif            
/* SPR 23393 Changes End */
            /* - SPS_TDD_Changes */
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
#endif
    /* TDD Config 0 changes start */
    /* FN_DL_UL_ALLOC_S_SUBFRAME */
    /* This functin set is used only for TDD config 0, S subframe */
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_STRATEGY_FD,0,0,0},
            {PROCESS_CHECK_STRATEGY_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_DL_SCH_STRATEGY, ELARG_DL_SF_TO_WORK_FOR,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_PDCCH_PDSCH_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* UL Strategy EL will be called two times */
            {PROCESS_UL_SCH_STRATEGY, ELARG_UL_SF_TO_WORK_FOR,0,0},
            {PROCESS_UL_SCH_STRATEGY, ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR,0,0},
            {PROCESS_MERGE_UL_STARTEGY_NODES,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_PRS_TRANSMISSION,0,0,0},  /* PRS_CHANGES */
            {PROCESS_CHECK_PDCCH_PDSCH_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* TDD Config 0 Changes Start */
            {PROCESS_PDCCH_PDSCH_WITH_MUX, ELARG_DL_SF_TO_WORK_FOR, ELARG_DL_HARQ_ACKNACK_U_SF, ELARG_UL_SF_TO_WORK_FOR},
            /* TDD Config 0 Changes End */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_MUX_FD,0,0,0},
            {PROCESS_CHECK_MUX_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_MUX_QUEUE,0,0,0},
#ifndef DL_DATA_SEPARATION            
            {RLC_HANDLE_TX_OPPORTUNITY,0,0,0},
#endif            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SET_ENCODER_FD,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            /* PUSCH EL will be passed two ulSubfarme.
             * 2nd ulSubframe will be valid only for TDD Config 0 */
            {PROCESS_PUSCH_FOR_DELAY_ONE, ELARG_UL_SF_TO_WORK_FOR, ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR,0},
#ifndef DL_DATA_SEPARATION            
            {PROCESS_ENCODER_QUEUE_WITHOUT_TX,0,0,0},
#endif            
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_CHECK_ENCODER_FD,0,0,0},
            {PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,0,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {TX_DL_CONTROL_MSG,0,0,0},
#ifndef DL_DATA_SEPARATION            
            {TX_DL_DATA_MSG,0,0,0},
#else
            {RLC_UPDATE_BUFF_OCC_TO_MAC,0,0,0},
            {PROCESS_DL_SYNC_TX_DATA,0,0,0},
#endif
            /* + SPS_TDD_Changes */
/* SPR 23393 Changes Start */            
#ifdef INTEL_SPS            
/* SPR 23393 Changes End */
            {PROCESS_SPS_PDSCH, ELARG_DL_SF_TO_WORK_FOR,0,0},
            {PROCESS_SPS_PUSCH,0,0,0},
/* SPR 23393 Changes Start */            
#endif            
/* SPR 23393 Changes End */
            /* - SPS_TDD_Changes */
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* TDD Config 0 changes end */
    /* FN_UL_CONFIG_SRS */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {TX_UL_CONFIG_MSG,0,0,0},
            /* TDD Config 0 changes Start */
            /* PROCESS_SRS_FOR_U_SUBFRAME and PROCESS_UL_CONFIG_MSG will be called 
             * 4 tick before the actual TX_UL_CONFIG_MSG */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SRS_FOR_U_SUBFRAME, ELARG_4TH_TICK_UL_SF_TO_WORK_FOR,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_UL_CONFIG_MSG, ELARG_4TH_TICK_UL_SPL_SF_TO_WORK_FOR,0,0},
            /* TDD Config 0 changes End */
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* FN_SSF_CONFIG */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {PROCESS_PRACH_FOR_S_SUBFRAME, ELARG_SPL_SF_TO_WORK_FOR,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_SRS_FOR_S_SUBFRAME, ELARG_SPL_SF_TO_WORK_FOR,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* FN_DL_HARQ */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {PROCESS_DL_HARQ_ACK_NACK_QUEUE,0,0,0},
            {PROCESS_HARQ_TIMER_EXPIRY,ELARG_DL_HARQ_EXPIRY_U_SF,0,0},
#ifdef OBSELETE_REL_8
            {RLC_HANDLE_HARQ_FAIL_IND,0,0,0},
#endif
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* FN_UL_HARQ */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {PROCESS_UL_HARQ_TIMER_EXPIRY, ELARG_UL_HARQ_EXPIRY_U_SF,0,0},
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* TDD Config 0 Changes Start */
    /* FN_UL_HARQ_D_SUBFRAME */
    /* This function set is used only for TDD config 0 ,D subframe */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            /* For TDD config 0 at D subframe there will be 
             * Harq Expiry of two U subframe . So calling the
             * PROCESS_UL_HARQ_TIMER_EXPIRY EL two times with two U SF */
            {PROCESS_UL_HARQ_TIMER_EXPIRY, ELARG_UL_HARQ_EXPIRY_U_SF,0,0},
            {PROCESS_UL_HARQ_TIMER_EXPIRY, ELARG_UL_HARQ_EXPIRY_2ND_U_SF,0,0},
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* TDD Config 0 Changes End */
    /* FN_MANDATORY_MISC */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {PROCESS_DRX_FOR_ALL_UE,0,0,0},
            {PROCESS_MEAS_GAP_FOR_ALL_UE,0,0,0},
            /* SPR 11364 Start */
            {PROCESS_DELETE_SCHEDULE_LC_LIST,0,0,0},
            /* SPR 11364 End */
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {PROCESS_DL_CA_TIMER_EXPIRY,ELARG_DL_HARQ_EXPIRY_U_SF,0,0},
/* SPR 23393 Changes Start */            
/* SPR 23393 Changes End */
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
#ifndef DL_UL_SPLIT_TDD
    /* FN_OPTIONAL_MISC */
    { 
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {MAX_EXECUTION_LEGS,0,0,0},
            {PROCESS_UL_RA, ELARG_CLOSEST_PDSCH_DELAY,0,0},
/*SPR 22919 Start*/
/*SPR 22919 End*/
            {PROCESS_DEMUX_QUEUE,0,0,0},
            {PROCESS_CQI_REPORTS,0,0,0},
            {PROCESS_UL_CSI_SINR_REPORTS,0,0,0},
            {PROCESS_E_CID_REPORTS,0,0,0}, /*E-CID REPORTS*/
            {UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,0,0,0},
            /*SPR 16855 +-*/
            {RLC_TIMER_CHK_FOR_TX_EXPIRY,0,0,0},
            {PROCESS_PENDING_DELETE_UE,0,0,0},
	    /* +- SPR 22494 */ 
            {PROCESS_MISC_UPDATES,0,0,0},
            /*SPR 16855 +-*/
            {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},
            {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},
            {PROCESS_UL_PENDING_DELETE_UE,0,0,0},
            /*SPR 16855 +-*/
            {RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE,0,0,0},
            /* SPR 13373 fix start */
            {RLC_HANDLE_PENDING_DELETE_UE,0,0,0},
            /* SPR 13373 fix end */
#ifdef LTE_EMBMS_SUPPORTED
            {RLC_HANDLE_PRE_OPPORTUNITY,0,0,0},
#endif
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    }
#else
    /*FN_UL_SYNC_DCI0*/
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {PROCESS_DL_PDCCH_SYNC,0,0,0},
#ifdef FAPI_CONTROL_FLOW
            {FAPI_UL_FLOW_SYNC,0,0,0},
#endif
            {TX_HI_DCI0_MSG,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* FN_OPTIONAL_MISC_DL*/
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {MAX_EXECUTION_LEGS,0,0,0},
            {PROCESS_CQI_REPORTS,0,0,0},
            {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},
            {PROCESS_PENDING_DELETE_UE,0,0,0},
            {PROCESS_DL_PERFORMANCE_STATS_UPDATE,ELARG_DL_PERF_STATS,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}
        }
    },
    /* FN_UL_OPTIONAL_AND_DLRLC*/
    {
        /* Counts are filled up via code using 
         * MAX_EXECUTION_LEGS as delim
         */
        0,
        0,
        {
            {MAX_EXECUTION_LEGS,0,0,0},
            {PROCESS_UL_RA,ELARG_CLOSEST_PDSCH_DELAY,0,0,0},
            {PROCESS_DEMUX_QUEUE,0,0,0},
            {PROCESS_UL_CSI_SINR_REPORTS,0,0,0},
            {PROCESS_RLC_DL_CHECK_TIMER_EXPIRY,0,0,0},
            {PROCESS_RLC_DL_HDLR_REESTABLISH_Q,0,0,0},
            {PROCESS_RLC_DL_HDLR_PEN_DEL_Q,0,0,0},
            {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},
            {PROCESS_UL_PENDING_DELETE_UE,0,0,0},
            {PROCESS_MISC_UPDATES,0,0,0},
	    /* +- SPR 22494 */
            {PROCESS_UL_PERFORMANCE_STATS_UPDATE,ELARG_UL_PERF_STATS,0,0},
            {MAX_EXECUTION_LEGS,0,0,0}

        }
    }
/*- SPR 17439*/    
#endif
};

#ifndef DL_UL_SPLIT_TDD
SchedulerFunctionalityType execSchedulerMnemonic_TddConfig_g[MAX_UL_DL_CONFIG][MAX_SUB_FRAME][FN_MAX] =
{
    /* Config 0 */
    {
        /* TDD Config 0 changes start */
        /* SF 0 */
        {
            FN_UL_HARQ_D_SUBFRAME,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC_S_SUBFRAME,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_DL_HARQ,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_UL_HARQ_D_SUBFRAME,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC_S_SUBFRAME,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_DL_HARQ, 
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
        /* TDD Config 0 changes end */
    },
    /* Config 1 */
    {
        /* SF 0 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC, 
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_DL_HARQ,
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_DL_HARQ,
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
    },
    /* Config 2 */
    {
        /* SF 0 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
    },
    /* Config 3 */
    {
        /* SF 0 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
    },
    /* Config 4 */
    {
        /* SF 0 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
    },
    /* Config 5 */
    {
        /* SF 0 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_DL_ALLOC_ONLY,
            FN_UL_CONFIG_SRS, 
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
    },
    /* Config 6 */
    {
        /* TDD Config 6 Changes Start */
        /* SF 0 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 1 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 2 */
        {
            FN_DL_HARQ,
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 3 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 4 */
        {
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 5 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 6 */
        {
            FN_DL_HARQ,
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 7 */
        {
            FN_DL_HARQ,
            FN_SSF_CONFIG,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 8 */
        {
            FN_DL_HARQ,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        },
        /* SF 9 */
        {
            FN_UL_HARQ,
            FN_DL_UL_ALLOC,
            FN_UL_CONFIG_SRS,
            FN_MANDATORY_MISC,
            FN_OPTIONAL_MISC,
            FN_MAX
        }
        /* TDD Config 6 Changes End */
    }
};
#else
SchedulerFunctionalityType execSchedulerMnemonic_TddConfig_g[MAX_UL_DL_CONFIG][NUM_OF_EXEC_FLOWS][MAX_SUB_FRAME][FN_MAX] =
{
    /*Config 0*/ 
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_MAX
            },
            /* SF 1 */
            {
                FN_MAX
            },
            /* SF 2 */
            {
                FN_MAX
            },
            /* SF 3 */
            {
                FN_MAX
            },
            /* SF 4 */
            {
                FN_MAX
            },
            /* SF 5 */
            {
                FN_MAX
            },
            /* SF 6 */
            {
                FN_MAX
            },
            /* SF 7 */
            {
                FN_MAX
            },
            /* SF 8 */
            {
                FN_MAX
            },
            /* SF 9 */
            { 
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_MAX
            },
            /*SF 2*/
            {
                FN_MAX
            },
            /*SF 3*/
            {
                FN_MAX
            },
            /*SF 4*/
            {
                FN_MAX
            },
            /*SF 5*/
            {
                FN_MAX
            },
            /*SF 6*/
            {
                FN_MAX
            },
            /*SF 7*/
            {
                FN_MAX
            },
            /*SF 8*/
            {
                FN_MAX
            },
            /*SF 9*/
            {
                FN_MAX
            }
        }
    },
    /*Config 1*/
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 1 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, //need to be seen for SSF
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 2 */
            {
                FN_DL_HARQ,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 3 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 4 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 5 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 6 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, //need to be seen for SSF
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 7 */
            {
                FN_DL_HARQ,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 8 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 9 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS,  //For ul config advance 1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_UL_HARQ,  
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 2*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 3*/
            {
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 4*/
            {
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 5*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 6*/
            {
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 7*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 8*/
            {
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 9*/
            {
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            }
        }
    }, 
    /*Config 2*/
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 1 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 2 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 3 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 4 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 5 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 6 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 7 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 8 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 9 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 2*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 3*/
            {
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 4*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 5*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 6*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 7*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 8*/
            {   
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 9*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            }
        }
    }, 
    /*Config 3*/
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 1 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 2 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 3 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 4 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 5 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 6 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 7 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 8 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 9 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, //For ul config advance 1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 2*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 3*/
            {
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 4*/
            {
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 5*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 6*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 7*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 8*/
            {   
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 9*/
            {
                FN_UL_HARQ,
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            }
        }
    }, 
    /*Config 4*/
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 1 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 2 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 3 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 4 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 5 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 6 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 7 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 8 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 9 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 2*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 3*/
            {
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 4*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 5*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 6*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 7*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 8*/
            {   
                FN_UL_HARQ,  
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 9*/
            {
                FN_UL_HARQ,  
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            }
        }
    }, 
    /*Config 5*/
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 1 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 2 */
            {
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 3 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 4 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 5 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 6 */
            {
                FN_DL_HARQ,
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 7 */
            {
                FN_DL_ALLOC_ONLY, 
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 8 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            },
            /* SF 9 */
            {
                FN_DL_ALLOC_ONLY,
                FN_MANDATORY_MISC,
                FN_OPTIONAL_MISC_DL,
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, //For ul config advance 0/1
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 2*/
            {
                FN_SSF_CONFIG,
                FN_UL_CONFIG_SRS, //For ul config advance 0
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 3*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 4*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 5*/
            {
                FN_UL_SYNC_DCI0,  
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 6*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 7*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 8*/
            {
                FN_UL_HARQ,  
                FN_UL_ALLOC_ONLY,
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            },
            /*SF 9*/
            {
                FN_UL_SYNC_DCI0, 
                FN_UL_CONFIG_SRS, 
                FN_UL_OPTIONAL_AND_DLRLC,
                FN_MAX
            }
        }
    },
    /*Config 6*/ 
    {
        /* DL Thread*/
        {
            /* SF 0 */
            {
                FN_MAX
            },
            /* SF 1 */
            {
                FN_MAX
            },
            /* SF 2 */
            {
                FN_MAX
            },
            /* SF 3 */
            {
                FN_MAX
            },
            /* SF 4 */
            {
                FN_MAX
            },
            /* SF 5 */
            {
                FN_MAX
            },
            /* SF 6 */
            {
                FN_MAX
            },
            /* SF 7 */
            {
                FN_MAX
            },
            /* SF 8 */
            {
                FN_MAX
            },
            /* SF 9 */
            { 
                FN_MAX
            }
        },
        /*UL Thread*/
        {
            /*SF 0*/
            {
                FN_MAX
            },
            /*SF 1*/ /*KG*/
            {
                FN_MAX
            },
            /*SF 2*/
            {
                FN_MAX
            },
            /*SF 3*/
            {
                FN_MAX
            },
            /*SF 4*/
            {
                FN_MAX
            },
            /*SF 5*/
            {
                FN_MAX
            },
            /*SF 6*/
            {
                FN_MAX
            },
            /*SF 7*/
            {
                FN_MAX
            },
            /*SF 8*/
            {
                FN_MAX
            },
            /*SF 9*/
            {
                FN_MAX
            }
        }
    }
};
#endif

#ifdef DL_UL_SPLIT_TDD
/*ULRLC ELs*/
ExecutionFlowCreationParamRlc executionFlowCreationParamRlc_g =
{
    3, /* thread id */
    50, /* thread priority */
    /*40,  thread priority */
    {
        INIT_RLC_PARAMS,
        0,
        3,
        DL_ADVANCE_TICK, /* delay to phy */
        5,
        2, /* core id */
        {
            {WAIT_FOR_SUBFRAME_IND,0,0,0},
            {PROCESS_RLC_HANDLE_DATA_FROM_MAC,0,0,0},
            {PROCESS_RLC_UL_CHECK_TIMER_EXPIRY,0,0,0},
            {PROCESS_RLC_UL_HDLR_REESTABLISH_Q,0,0,0},
            {PROCESS_RLC_UL_HDLR_PEN_DEL_Q,0,0,0}
        }
    }
};
#endif

/* The below table is from Table 7.3-Y: 36.213 ,
 * Provides info on which subgrame N_UL_DAI is transmitted for each UL_DL_CONFIG
 * Also which all subframes info will be transmitted on it.
 */
UInt8 tddDci0DaiSfIndex_g[MAX_UL_DL_CONFIG][MAX_SUB_FRAME] = 
{
    /* Subframe 0 */
    {0},
    /* Subframe 1 */
    {1,1,6,9,4,6,6,1,4,9},
    /* Subframe 2 */
    {3,3,8,3,8,8,8,3,8,3},
    /* Subframe 3 */
    {0,8,8,9,0,8,8,9,9,0},
    /* Subframe 4 */
    {8,8,8,9,8,8,9,9,9,9},
    /* Subframe 5 */
    {8,8,8,8,8,8,8,8,8,8},
    /* TDD Config 6 Changes Start */
    /* Subframe 6 */
    {0,1,5,6,9,5,6,0,1,9}
    /* TDD Config 6 Changes End */
};

/*
 * this table is used to verify and reset the DAI value when a new PDSCH is 
 * been sent to a UE, during the a DAI txn period.
 * For configuration 1 as mentioned below
 * { 0, 1, NA, NA, 0, 0, 1, NA, NA, 0},
 * there are four DAI txn period are 0-1, 4, 5-6, 9 
 * If the diff between the current subframe and the last transmitted subframe 
 * is equal to or less tahn the value present in the above array, then the 
 * PDSCH already transmitted during this DAI txn period
 * else the DAI value need to be re-initialized.
 */

 /* In the current design, the UL subframe stores the max difference between the 
 * first pdsch transmission of that UL DAI group, and the DL subframe on which 
 * the DCI 0 will be sent carrying the UL DAI value of that group.
 * So for example in config 3, 
 * SF 2 of SFN X -> dci 0 is at sf 8 of SFN X -1. This will carry the UL DAI 
 * of SFs 6,5 and 1 of SFN X-1. The maximum difference is thus 8-1 = 7. 
 */

/* SPR 4420 Fix */
UInt8 tddValidateDai_g[MAX_UL_DL_CONFIG][MAX_SUB_FRAME] = 
{
    /* Subframe 0 */
    {0},
    /* Subframe 1 */
    {0,1,1,0,0,0,1,1,0,0},
    /* Subframe 2 */
    {1,2,4,4,0,1,2,4,4,0},
    /* Subframe 3 */
    {1,0,7,2,1,4,5,0,1,0},
    /* Subframe 4 */
    {0,1,8,3,4,5,0,1,2,3},
    /* Subframe 5 */
    {1,2,10,4,5,6,7,8,9,0},
    /* Subframe 6 */
    {0,0,NA,NA,NA,0,0,NA,NA,0}
};
/* SPR 4420 Fix */

/*
 * Pointer to store the above array's for a particular UL-DL config
 */
/** CA-TDD Changes Start **/
UInt8 *tddDci0DaiSfIndex_p[MAX_NUM_CELL];
UInt8 *tddValidateDai_p[MAX_NUM_CELL];
/** CA-TDD Changes End **/

extern void ElprocessOAMAndRRCMsg(UInt8 numOfMsg, UInt32 delayToPhy);
/*+ SPR 17439*/
extern MacRetType sendStartReqToPhy(void);
/*- SPR 17439*/

/*###########################################################################################################*/
/*                                          MODIFIED FUNCTIONS                                               */
/*###########################################################################################################*/

/*****************************************************************************
 * Function Name  : initExecutionScheduler
 * Inputs         : numOfCells
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This functions does the initializations necessary for 
 *                  execution scheduler which includes creation of socket/msgQ
 *                  interfaces b/w MAC-RRC/OAM and MAC-PHY.It calls 
 *                  createExecutionFlowThreads which creates execution flow
 *                  threads.
 *****************************************************************************/
MacRetType initExecutionScheduler(UInt8 numOfCells)
{
    LTE_MAC_UT_LOG(LOG_DETAIL,TDD_MANAGER,"[%s] Entry.",__func__);
    MacRetType returnValue = MAC_SUCCESS;
    /** 14079 Changes Start **/
    UInt32 flowCount = 0;
    /** 14079 Changes End **/

    initTddExecutionLegFunctionArr();

    /** 14079 Changes Start **/
    for (flowCount = 0; flowCount < NUM_OF_EXEC_FLOWS * numOfCells; flowCount++)
    {
        semInitDefault(&waitForCellSetupIndSemArr_g[flowCount], 0);
    }
    /* Initialise waitForSubFrameIndSemArr_g , execution flow threads
     * wait on their correspondind index semaphore for tick indication*/
#ifdef DL_UL_SPLIT_TDD
    semInitDefault(&waitForSubFrameIndSemArr_g, 0);
    /** 14079 Changes End **/

#ifdef FAPI_CONTROL_FLOW
    semInitDefault(&waitForFAPIMsgControlFlowInd_g,0);
#endif

    /* Threads are created before cell configuration due to this 
       executionFlowCreationParamArr_g is initialized with default values 
       required for thread creation.*/
    executionFlowCreationParamArr_g=executionFlowCreationParamArrTDDConfigDefault_g;

    if (MAC_FAILURE == createExecutionFlowThreads(
                              &executionFlowCreationParamArrTDDConfigDefault_g[0]))
    {
        ltePanic("[%s]Error in execution flow thread creation",__func__);
    }
#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,TDD_MANAGER,"[%s] Exit.",__func__);
    return returnValue;
}

/*****************************************************************************
 * Function Name  : initUlDLExecutionFlow
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize the global execution flow pointer 
 *                  based on the UL DL configuration provided from RRC.
 *****************************************************************************/
/** CA-TDD Changes Start **/

void initUlDLExecutionFlow(InternalCellIndex internalCellIndex)
{

    /** CA-TDD Changes Start **/
    #ifndef DL_UL_SPLIT_TDD
    /* SPR 13768 Changes Start */
    executionFlowCreationParamArr_g[internalCellIndex] = executionFlowCreationParamArrTDDConfigDefault_g[internalCellIndex];
    /* SPR 13768 Changes End */
    #endif
    /** CA-TDD Changes End **/

    /* 
     * Update the global pointer with the PDCCH and PDSCH indexr for the
     * configured UL-DL config and Special subframe config value
     */
    tddValidateDai_p[internalCellIndex] = &tddValidateDai_g[cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign][0];
    tddDci0DaiSfIndex_p[internalCellIndex] = &tddDci0DaiSfIndex_g[cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign][0];

    tddMacPdcchPdschFtnPtrIndexer_p[internalCellIndex] = 
            &tddMacPdcchPdschFtnPtrIndexer
            [cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            splSubFramePatterns][cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->subFrameAssign][0];
#ifdef LTE_EMBMS_SUPPORTED
    tddMacEmbmsFtnPtrIndexer_p[internalCellIndex] = 
            &tddMacEmbmsFtnPtrIndexer
            [cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
	    cellParams_p->subFrameAssign][0];
#endif
    /*
     * Initialize the functon for putting entry in Ack/Nack Queue 
     * for particular UL/DL Config
     */
    tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex] = 
            tddPutEntryInAckNackQueueForDiffConfig_g[cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign];
    /** CA-TDD Changes End **/


}

#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : executionFlowThreadRlc
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : The thread executes execution legs passed to it through
 *                  arguments.
 *****************************************************************************/
void* executionFlowThreadRlc(void * executionFlowThreadArgs_p)
{
	ExecutionLegFunctionArg execLegFuncArg = {0};
	ExecutionFlowArgumentsRlc * threadArg_p =
		(ExecutionFlowArgumentsRlc *)executionFlowThreadArgs_p;
	ExecutionLegInfo * executionLegArr_p = threadArg_p->executionLegArr;
	UInt32 executionLegCount;

	UInt8 logger_file_name[LTE_MAX_FILE_NAME_SIZE] = {0};
	/* Bind this thread to the core number specified in the structure definition */
	THREAD_TO_BIND_SET_CORE_NO(threadSelf(), threadArg_p->coreId);
    /*Set Thread Priority*/
	threadSetRtPriority(SCHED_FIFO, executionFlowCreationParamRlc_g.threadPriority);

    /* Initialize logging for execution flows */
    /*Coverity 26339 Fix Start*/ 
    prepareLoggerFileName((Char8 *)logger_file_name,LTE_L2_LOG_REGION_FLOW,threadArg_p->threadIndex,LTE_MAX_FILE_NAME_SIZE-1);
    /*Coverity 26339 Fix End*/ 
	logger_init((LTE_L2_LOG_REGION_MAX+threadArg_p->threadIndex), ( Char8*)logger_file_name, l2Input.shmSize);

	execLegFuncArg.execFlowIndex = threadArg_p->threadIndex;
	execLegFuncArg.delayToPhy = threadArg_p->dlPathDelay;

    /*SPR 16855 +-*/
    initRlcRrcCnfQueue(&rlcRrcCnfInputQId_g);
    initRlcRrcCnfQueue(&rlcRrcCnfOutputQId_g);
    /*SPR 16855 +-*/
    /* this is the main loop of the thread. This will keep looping till the thread is alive.*/
	while(1)
	{
		/* wait for cell setup indication */
		semWaitDefault(&waitForCellSetupIndSemArr_g[execLegFuncArg.execFlowIndex]);
/* SPR 4915 : Start*/
		while(cellConfigured_g && !stoppedProcessing_g && recvFromPhyDone_g)
/* SPR 4915 : End*/
        {

            /* The first leg wait for TICK indication , after 
             * tick reception we get SF and SFN*/
            executionLegCount=0; 
            while(executionLegCount <  threadArg_p->executionLegCount)
            {
                //RDTSC_AND_CLOCK_START_SPLIT_TDD( execLegFuncArg.execFlowIndex,executionLegArr_p[0].executionLeg);

                execLegFuncArg.globalTick = getCurrentTick();

                execLegFuncArg.execLegSpecificArg1 = 
                    executionLegArr_p[executionLegCount].legSpecificArg1;

                execLegFuncArg.execLegSpecificArg2 = 
                    executionLegArr_p[executionLegCount].legSpecificArg2;
               
                /* TDD Config 0 Changes Start */
                execLegFuncArg.execLegSpecificArg3 = 
                    executionLegArr_p[executionLegCount].legSpecificArg3;
               
                /* TDD Config 0 Changes End */
                RDTSC_AND_CLOCK_START_TDD(executionLegArr_p[executionLegCount].executionLeg);


                /* execute handler for first EL, this will help to get Subframe indication & tick */
                executionFunctionArrTdd_g[executionLegArr_p[executionLegCount].executionLeg]
                    (&execLegFuncArg);
                
                RDTSC_AND_CLOCK_END_ULRLC(executionLegArr_p[executionLegCount].executionLeg);

                executionLegCount++;   
                //RDTSC_AND_CLOCK_END_SPLIT_TDD( execLegFuncArg.execFlowIndex,executionLegArr_p[0].executionLeg);
            }


/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
        }
	}
	return PNULL;
}

/*****************************************************************************
 * Function Name  : executionTDDFlowThread 
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : The thread executes execution legs passed to it through
 *                  arguments.
 *****************************************************************************/
void* executionTDDFlowThread(void* executionFlowThreadArgs_p) 
{

    ExecutionFlowArguments *execFlowThreadArgs_p=
        (ExecutionFlowArguments *)executionFlowThreadArgs_p;
    UInt8 threadIndex;
    TDDSubFrame *subFrameInf_p = PNULL;
    ExecutionLegFunctionArg execLegFuncArg = {0};
    UInt8  maxExecutionLegCount = 0;
    UInt8 executionLegCount = 0;
    UInt32 systemFrameNum = 0;
    UInt32  subframeNum   = 0;
    UInt8 logger_file_name[LTE_MAX_FILE_NAME_SIZE] = {0};


    /* Bind this thread to the core number specified in the structure definition */
    THREAD_TO_BIND_SET_CORE_NO(threadSelf(), execFlowThreadArgs_p->coreId);
    threadSetRtPriority(SCHED_FIFO, executionFlowCreationParamArr_g[execFlowThreadArgs_p->threadIndex].threadPriority);

    /* Initialize logging for execution flows */
    /*Coverity 26339 Fix Start*/ 
    prepareLoggerFileName((Char8 *)logger_file_name,LTE_L2_LOG_REGION_FLOW,execFlowThreadArgs_p->threadIndex,LTE_MAX_FILE_NAME_SIZE-1);
    /*Coverity 26339 Fix End*/ 
    logger_init((LTE_L2_LOG_REGION_MAX+execFlowThreadArgs_p->threadIndex), ( Char8*)logger_file_name, l2Input.shmSize);


    threadIndex=execFlowThreadArgs_p->threadIndex;

    semWaitDefault(&waitForCellSetupIndSemArr_g[threadIndex]);

    execFlowThreadArgs_p = &(executionFlowCreationParamArr_g[threadIndex].threadArguments);
    subFrameInf_p= &(execFlowThreadArgs_p->subFrameInfo[0]);


    /*For DL Thread First Tick is considered as tick receievd in start PHY
      For UL Thread first tick is recived here. */

    if(INIT_UL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
    {
        processRxFirstSubFrameIndUl();
    }

    while(1)
    {
        /* SPR 3932 Chg*/
        if(phyStarted_g == PHY_IDLE || phyStarted_g == PHY_CONFIG_WITH_EXPLICIT_START)
            /* SPR 3932 Chg*/
        {
            semWaitDefault(&waitForCellSetupIndSemArr_g[threadIndex]);
            /*In case cell stop and start,for DL Thread First Tick is considered as tick receievd 
             * in start PHY for UL Thread first tick is recived here*/
            if(INIT_UL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
            {
                processRxFirstSubFrameIndUl();
            }
        }

#ifdef CYCLE_COUNT_MAC_RLC_PDCP
        if(INIT_UL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
        {    
            lteIncrementL2Cycles();
        }
#endif
        if(INIT_UL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
        {
            getUlSFAndSFN(&systemFrameNum, &subframeNum);
            execLegFuncArg.globalTick = getCurrentTickUl();
        }
        else if(INIT_DL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
        {
            getSFAndSFN(&systemFrameNum, &subframeNum);
            execLegFuncArg.globalTick = getCurrentTick();
        }
        execLegFuncArg.sysFrame = systemFrameNum;
        execLegFuncArg.subFrame = subframeNum;

        /* set advance 2 subframe */
        execLegFuncArg.advanceDLGlobalTick = 
            execLegFuncArg.globalTick + DL_ADVANCE_TICK;

        subframeNum += DL_ADVANCE_TICK; /* DL_ADVANCE_TICK 2 */
        if ( subframeNum >= MAX_SUB_FRAME)
        {
            subframeNum -= MAX_SUB_FRAME;
            systemFrameNum++;

            if ( systemFrameNum == MAX_SFN_VALUE)
            {
                systemFrameNum = 0;
            }
        } 
        execLegFuncArg.advanceDLSystemframe = systemFrameNum;
        execLegFuncArg.advanceDLSubframe    = subframeNum;

        maxExecutionLegCount = subFrameInf_p[execLegFuncArg.subFrame].subFrameExecCount;  

        executionLegCount = 1;

        LTE_MAC_UT_LOG( LOG_DETAIL, TDD_MANAGER,
                "[%s] SFN [%d] SF[%d] TTI [%d] ExecutionLeg Count[%d]",
                __func__, systemFrameNum, subframeNum, execLegFuncArg.globalTick + DL_ADVANCE_TICK,
                maxExecutionLegCount);

        while ((executionLegCount < maxExecutionLegCount) &&
                (FALSE == stoppedProcessing_g))
        {
            /* SPR 3932 Chg*/
            if( phyStarted_g == PHY_IDLE || phyStarted_g == PHY_CONFIG_WITH_EXPLICIT_START)
                /* SPR 3932 Chg*/
            {
                break;
            }

            execLegFuncArg.execLegSpecificArg1 = 
                subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                [executionLegCount].legSpecificArg1;

            execLegFuncArg.execLegSpecificArg2 = 
                subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                [executionLegCount].legSpecificArg2;

            /* TDD Config 0 Changes Start */
            execLegFuncArg.execLegSpecificArg3 = 
                subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                [executionLegCount].legSpecificArg3;
            /* TDD Config 0 Changes End */
            /*------SPR 2534 start----*/
            RDTSC_AND_CLOCK_START_SPLIT_TDD(execLegFuncArg.execFlowIndex,subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg);
            /*------SPR 2534 end----*/
            executionFunctionArrTdd_g[subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                [executionLegCount].executionLeg](&execLegFuncArg);
            RDTSC_AND_CLOCK_END_SPLIT_TDD(execLegFuncArg.execFlowIndex,subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg);

            /*break from the loop and wait for the init req from OAM*/
            if (TRUE == stoppedProcessing_g)
            {
                break;
            }

            executionLegCount++;
        }

        /*------SPR 2534 start----*/
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
        /*------SPR 2534 end----*/

        /*break from the loop and wait for the init req from OAM*/
        if (TRUE == stoppedProcessing_g)
        {
            break;
        }

        /* The first leg wait for TICK indication , after 
         * tick reception we get SF and SFN*/
        if(INIT_UL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
        {
            getUlSFAndSFN(&execLegFuncArg.sysFrame, &execLegFuncArg.subFrame);
            execLegFuncArg.globalTick = getCurrentTickUl();
        }
        else if(INIT_DL_THREAD_PARAMS == execFlowThreadArgs_p->startExecutionLeg)
        {
            execLegFuncArg.globalTick = getCurrentTick();
            getSFAndSFN(&execLegFuncArg.sysFrame, &execLegFuncArg.subFrame);
        }

        execLegFuncArg.execLegSpecificArg1 = 
            subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].legSpecificArg1;

        execLegFuncArg.execLegSpecificArg2 = 
            subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].legSpecificArg2;

        /* TDD Config 0 Changes Start */
        execLegFuncArg.execLegSpecificArg3 = 
            subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].legSpecificArg3;
        /* TDD Config 0 Changes End */
        /*------SPR 2534 start----*/
        RDTSC_AND_CLOCK_START_SPLIT_TDD(execLegFuncArg.execFlowIndex,subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].executionLeg);
        /*------SPR 2534 end----*/

        executionFunctionArrTdd_g[subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].executionLeg]
            (&execLegFuncArg);
        RDTSC_AND_CLOCK_END_SPLIT_TDD(execLegFuncArg.execFlowIndex,subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].executionLeg); 

    }

    return PNULL;
}

#else
 /* CA TDD Changes Start */
/*****************************************************************************
 * Function Name  : executionSingleTDDFlowThread
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : The thread executes execution Legs of TDD Passed 
 *                  it through the arguments
 *****************************************************************************/
/* SPR 23393 Changes Start */
UInt32 debug_where_is_EL_g = 0; 
/* SPR 23393 Changes End */
void executionSingleTDDFlowThread(InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_DETAIL,TDD_MANAGER,"[%s] Entry.",__func__);

    /*SPR 16855 +-*/
    ExecutionLegFunctionArg execLegFuncArg = {0};
    TDDSubFrame *subFrameInf_p = PNULL;
    UInt8  maxExecutionLegCount = 0;
/*TDD Tick overshoot changes start*/
#ifdef OVERSHOOT_CORRECTION
    UInt64 rem_cycles = 0;
    UInt8  mandatoryExecutionLegCount = 0;
#endif
/*TDD Tick overshoot changes end*/
    UInt8 executionLegCount = 0;
    UInt32 systemFrameNum = 0;
    UInt32  subframeNum   = 0;
    
    /* CA TDD Changes Start */
    execLegFuncArg.cellIndex = internalCellIndex;
    /* CA TDD Changes End */
    /*SPR 16855 +-*/
    /* SPR 13768 Changes Start */
    ExecutionFlowArguments *execFlowThreadArgs_p =
        &(executionFlowCreationParamArr_g[internalCellIndex]->threadArguments);   
    /* SPR 13768 Changes End */
    subFrameInf_p= &(execFlowThreadArgs_p->subFrameInfo[0]);

    LTE_MAC_UT_LOG(LOG_DETAIL,TDD_MANAGER,
            "[%s] UL/DL Configuration [%d] SpecialSubframeConfig [%d]\n",
            __func__, cellConfig_g.subFrameAssign, cellConfig_g.splSubFramePatterns);

    /*SPR 16855 +-*/
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
            /* CA Stats Changes Start */
            lteIncrementL2Cycles(execLegFuncArg.cellIndex);
            /* CA Stats Changes End */
#endif 
/*SPR# 3107 Changes Starts */
                /* If message is recieved from OAM process 
                 * Continue the execution till noOfTickToBeProcess_g becomes Zero.
                 * If noOfTickToBeProcess_g is 0, Stop the execution till another pdu is 
                 * received */
#ifdef UTFWK_SIMULATION
                if(1 == recvFromOamDone_g) 
                {
                    while(0 == noOfTickToBeProcess_g)
                    { 
                        ElprocessOAMAndRRCMsg(1,0);
                        continue;
                    }
                noOfTickToBeProcess_g--;
                }
#endif
/*SPR# 3107 Changes Ends */
            execLegFuncArg.globalTick = getCurrentTick();
                getSFAndSFN(&execLegFuncArg.sysFrame, &execLegFuncArg.subFrame, execLegFuncArg.cellIndex);

                execLegFuncArg.execLegSpecificArg1 = 
                    subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].legSpecificArg1;

                execLegFuncArg.execLegSpecificArg2 = 
                    subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].legSpecificArg2;

                /* TDD Config 0 Changes Start */
                execLegFuncArg.execLegSpecificArg3 = 
                    subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].legSpecificArg3;
                /* TDD Config 0 Changes End */
                /*------SPR 2534 start----*/
                RDTSC_AND_CLOCK_START_TDD(subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].executionLeg);
                /*------SPR 2534 end----*/

                executionFunctionArrTdd_g[subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].executionLeg]
                    (&execLegFuncArg);
                /* CA Stats Changes Start */
                RDTSC_AND_CLOCK_END_LEG(subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[0].executionLeg,
                        execLegFuncArg.cellIndex); 
                /* CA Stats Changes End */

                /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
                /*
                ** As we have updated MAC tick with L2 tick when we received
                ** PHY Start message from L1 Layer. Check whether
                ** startExecutionAfterPhyStart_g is set.
                */
                /*SPR 13047 fix start*/
                if (startExecutionAfterPhyStart_g[execLegFuncArg.cellIndex] == TRUE)
                {
                    startExecutionAfterPhyStart_g[execLegFuncArg.cellIndex] = FALSE;
                    /* Wait for tti to be received before calling remaining ELs */
                    if(TRUE != firstTickRecvFlag_g[execLegFuncArg.cellIndex])
                    {
                        return;
                    }
                    firstTickRecvFlag_g[execLegFuncArg.cellIndex] = FALSE;
                }
                /*SPR 13047 fix end*/
                /*
                ** Checks whether new TICK has been received from the system.
                ** If yes, then process the remaining ELs.
                */
                else if (execLegFuncArg.globalTick != getCurrentTickFrmL2Tick(execLegFuncArg.cellIndex))
                {
                    /*
                    ** Set the currentTick, systemFrame and subframe globals
                    ** for MAC.
                    ** Initialize some other global variables which is required
                    ** before starting ELs execution.
                    */
                    /*SPR 16855 +-*/
                }
                else
                {
                    /* Waiting for the Tick from system */
                    return;
                }
#else
                /* + SPR 7521 + */
                if (execLegFuncArg.globalTick == getCurrentTick())
                {
                    /* SPR 11482 Start */
                    return;
                    /* SPR 11482 End */

                    /* CA TDD code Changes Start */

                    /* Wait for tti to be received before calling remaining ELs */

/*                    if(TRUE != firstTickRecvFlag_g)
                    {
                        return;
                    }
                    firstTickRecvFlag_g = FALSE;
*/
                    /* CA TDD code Changes End */
                }
                /* - SPR 7521 - */
#endif
                /* FAPI L2 AS MASTER END */

                getSFAndSFN(&systemFrameNum, &subframeNum, execLegFuncArg.cellIndex);
                execLegFuncArg.globalTick = getCurrentTick();
                execLegFuncArg.sysFrame = systemFrameNum;
                execLegFuncArg.subFrame = subframeNum;

                /* set advance 2 subframe */
                execLegFuncArg.advanceDLGlobalTick = 
                    execLegFuncArg.globalTick + DL_ADVANCE_TICK;

                subframeNum += DL_ADVANCE_TICK; /* DL_ADVANCE_TICK 2 */
                if ( subframeNum >= MAX_SUB_FRAME)
                {
                    subframeNum -= MAX_SUB_FRAME;
                    systemFrameNum++;

                    if ( systemFrameNum == MAX_SFN_VALUE)
                    {
                        systemFrameNum = 0;
                    }
                } 
                execLegFuncArg.advanceDLSystemframe = systemFrameNum;
                execLegFuncArg.advanceDLSubframe    = subframeNum;

                maxExecutionLegCount = subFrameInf_p[execLegFuncArg.subFrame].subFrameExecCount;   

                executionLegCount = 1;
                /* SPR 23393 Changes Start */
                debug_where_is_EL_g = 255;
                /* SPR 23393 Changes End */
            
                LTE_MAC_UT_LOG( LOG_DETAIL, TDD_MANAGER,
                 "[%s] SFN [%d] SF[%d] TTI [%d] ExecutionLeg Count[%d]",
                __func__, systemFrameNum, subframeNum, execLegFuncArg.globalTick + DL_ADVANCE_TICK,
                maxExecutionLegCount);
/*TDD Tick overshoot changes start*/

#ifdef OVERSHOOT_CORRECTION 

                mandatoryExecutionLegCount = subFrameInf_p[execLegFuncArg.subFrame].mandatoryExecutionLegCount;   
                while ((executionLegCount < mandatoryExecutionLegCount)
                    /* CA TDD Changes Start */
                    /* &&
                        (FALSE == stoppedProcessing_g)*/
                    /* CA TDD Changes End */
                        )
#else



                while ((executionLegCount < maxExecutionLegCount) )
                    /* CA TDD Changes Start */
                        //(FALSE == stoppedProcessing_g[MAIN_CELL]))
                    /* CA TDD Changes End */
#endif //OVERSHOOT_CORRECTION
            {
                    /*SPR 16855 +-*/
                    execLegFuncArg.execLegSpecificArg1 = 
                        subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                        [executionLegCount].legSpecificArg1;

                    execLegFuncArg.execLegSpecificArg2 = 
                        subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                        [executionLegCount].legSpecificArg2;

                    /* TDD Config 0 Changes Start */
                    execLegFuncArg.execLegSpecificArg3 = 
                        subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                        [executionLegCount].legSpecificArg3;
                    /* TDD Config 0 Changes End */
                   /*------SPR 2534 start----*/
                    RDTSC_AND_CLOCK_START_TDD(subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg);
                   /*------SPR 2534 end----*/
                    executionFunctionArrTdd_g[subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                        [executionLegCount].executionLeg](&execLegFuncArg);
                    /* CA Stats Changes Start */
                    RDTSC_AND_CLOCK_END_LEG(subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg,
                            execLegFuncArg.cellIndex);
                    /* CA Stats Changes End */
                     /* SPR 23393 Changes Start */
                    debug_where_is_EL_g = executionLegCount;
                    /* SPR 23393 Changes End */
                    /*SPR 16855 +-*/
                    executionLegCount++;
                }
#ifdef OVERSHOOT_CORRECTION
                while((executionLegCount < maxExecutionLegCount) 
                    /* CA TDD Changes Start */
                    /*&&
                            (FALSE == stoppedProcessing_g)*/)
                    /* CA TDD Changes End */
                { 
                    /*SPR 16855 +-*/
                    GET_PROCESSING_CYCLES_LEFT(rem_cycles);
                    if (( !noTimeToProcessflag_g ) && 
                            (rem_cycles >=
                             reqOptionalElProcessingCyclesTdd_g
                             /*+ Dyn UEs To Be SchPerTTI +*/
                             [executionLegCount - mandatoryExecutionLegCount][reqIndexForUeSchdInDlUl_g]))
                             /*- Dyn UEs To Be SchPerTTI -*/
                    {
                        execLegFuncArg.execLegSpecificArg1 = 
                            subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                            [executionLegCount].legSpecificArg1;

                        execLegFuncArg.execLegSpecificArg2 = 
                            subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                            [executionLegCount].legSpecificArg2;

                        /* TDD Config 0 Changes Start */
                        execLegFuncArg.execLegSpecificArg3 = 
                            subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                            [executionLegCount].legSpecificArg3;
                        /* TDD Config 0 Changes End */
                        /*------SPR 2534 start----*/
                        RDTSC_AND_CLOCK_START_TDD(subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg);
                        /*------SPR 2534 end----*/
                        executionFunctionArrTdd_g[subFrameInf_p[execLegFuncArg.subFrame].executionLegArr
                            [executionLegCount].executionLeg](&execLegFuncArg);
                        /* CA Stats Changes Start */
                        RDTSC_AND_CLOCK_END_LEG(subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg,
                                execLegFuncArg.cellIndex);
                        /* CA Stats Changes End */


                    }
                    else
                    {
                        if(currTickOverLoad_g[execLegFuncArg.cellIndex] != OVERLOAD_LEVEL_CRITICAL)
                        {
                            if ((subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg == PROCESS_UL_RA)
                                    || (subFrameInf_p[execLegFuncArg.subFrame].executionLegArr[executionLegCount].executionLeg == PROCESS_DEMUX_QUEUE))

                            {
                                currTickOverLoad_g[execLegFuncArg.cellIndex] = OVERLOAD_LEVEL_HIGH;

                                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                        L2_SYS_FAIL,
                                        getCurrentTick(),
                                        __LINE__,
                                        currTickOverLoad_g[execLegFuncArg.cellIndex],
                                        /** CA-TDD Changes Start **/
                                        macOverLoadLevel_g[execLegFuncArg.cellIndex],
                                        /*+ Dyn UEs To Be SchPerTTI +*/
                                        dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                                        /** CA-TDD Changes Stop **/
                                        /*- Dyn UEs To Be SchPerTTI -*/
                                        "Overload level changed.");
                            }
                            else
                            {
                                currTickOverLoad_g[execLegFuncArg.cellIndex] = OVERLOAD_LEVEL_MEDIUM;
                                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                        L2_SYS_FAIL,
                                        getCurrentTick(),
                                        __LINE__,
                                        currTickOverLoad_g[execLegFuncArg.cellIndex],
                                        /** CA-TDD Changes Start **/
                                        macOverLoadLevel_g[execLegFuncArg.cellIndex],
                                        /*+ Dyn UEs To Be SchPerTTI +*/
                                        dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                                        /** CA-TDD Changes Stop **/
                                        /*- Dyn UEs To Be SchPerTTI -*/
                                        "Overload level changed.");
                            }
                        }
                        break;
                    }
                    executionLegCount++;
                    /*SPR 16855 +-*/
                }
                /** CA2 Changes Start **/
                /* + SPR 13853 Changes */
                setOverLoadLevel(currTickOverLoad_g[execLegFuncArg.cellIndex],execLegFuncArg.cellIndex);
                /* - SPR 13853 Changes */
                /** CA2 Changes End **/
                currTickOverLoad_g[execLegFuncArg.cellIndex] = OVERLOAD_LEVEL_NORMAL;
                noTimeToProcessflag_g = 0;
#endif
/*TDD Tick overshoot changes end*/

                /*SPR 16855 +-*/
//	    lteMacUpdateStatsPerfEnd();
/*------SPR 2534 start----*/
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
/*------SPR 2534 end----*/
//UInt32 tddHandleProcessRxSubFrameFlow ()/*merge_check*/


                /*SPR 16855 +-*/
                /* The first leg wait for TICK indication , after 
                 * tick reception we get SF and SFN*/
                /*SPR 16855 +-*/
    LTE_MAC_UT_LOG(LOG_DETAIL,TDD_MANAGER,"[%s] Exit .",__func__);
}
#endif


/*****************************************************************************
 * Function Name  : updateElArgs 
 * Inputs         : elArgTypesSrc,
 *                  elArgsDest,
 *                  airSf,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This updates every ELs required arguments in the execution
 *                  Flow Array.
 *****************************************************************************/
/** CA-TDD Changes Start:06 **/
static void  updateElArgs(ExecutionLegInfo * elArgTypesSrc, ExecutionLegInfo * elArgsDest, 
                                UInt8 airSf,InternalCellIndex internalCellIndex)
{
    elArgsDest->legSpecificArg1 = elArgsTdd_g[internalCellIndex][airSf].\
                                  elArgArray[elArgTypesSrc->legSpecificArg1];
    elArgsDest->legSpecificArg2 = elArgsTdd_g[internalCellIndex][airSf].\
                                  elArgArray[elArgTypesSrc->legSpecificArg2];
    elArgsDest->legSpecificArg3 = elArgsTdd_g[internalCellIndex][airSf].\
                                  elArgArray[elArgTypesSrc->legSpecificArg3];
/** CA-TDD Changes End:06 **/
}

#ifndef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : populateTddELSchedulerArray 
 * Inputs         : cellParams_p - pointer to CellConfigParams
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : Populates EL scheduler array from meta info.
 *****************************************************************************/

void populateTddELSchedulerArray(CellConfigParams *cellParams_p, InternalCellIndex internalCellIndex )
{
    SchedulerFunctionalityType (* execSchedulerMnemonic_TddConfig_pp)[FN_MAX] = NULL;
    UInt8 airSf = 0;
    execSchedulerMnemonic_TddConfig_pp = &execSchedulerMnemonic_TddConfig_g[cellParams_p->subFrameAssign][0];

    for (airSf = 0; airSf < MAX_SUB_FRAME; airSf++)
    {
        UInt8 fn_set_count = 0;
        UInt8 workingSf = airSf + DL_ADVANCE_TICK;
        if (workingSf >= MAX_SUB_FRAME)
        {
            workingSf -= MAX_SUB_FRAME;
        }
        UInt8 totalElCountPerSf = 2;
#ifdef OVERSHOOT_CORRECTION
        UInt8 totaloptionalElCount = 0;
#endif
        /* SPR 13768 Changes Start */
        TDDSubFrame * elArray_p =  &(executionFlowCreationParamArr_g[internalCellIndex]->threadArguments.subFrameInfo[airSf]);
        /* SPR 13768 Changes End */
        while( FN_MAX != execSchedulerMnemonic_TddConfig_pp[workingSf][fn_set_count] )
        {
            //Fill up EL array
            SchedulerFunctionalityType fn_set = execSchedulerMnemonic_TddConfig_pp[workingSf][fn_set_count];
            UInt8 mandatoryElCnt = SchFnSet_g[fn_set].mandatoryElCount;
            UInt8 optionalElCnt =  SchFnSet_g[fn_set].optionalElCount;
#ifdef OVERSHOOT_CORRECTION
            totaloptionalElCount += optionalElCnt;
#endif
            UInt8 constituentElIndex = 0;
            while( constituentElIndex < mandatoryElCnt)
            {
                elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex].executionLeg = 
                    SchFnSet_g[fn_set].schSetConstituentELsAndArgs[constituentElIndex].executionLeg;
                updateElArgs(&SchFnSet_g[fn_set].schSetConstituentELsAndArgs[constituentElIndex], 
                        &(elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex]), airSf, internalCellIndex);
                constituentElIndex++;
            }
            totalElCountPerSf += mandatoryElCnt;
            constituentElIndex = 0;
            /*
             * For optional ELs first EL will be delimiter
             * MAX_EXECUTION_LEGS
             * Therefore indexing to next EL
             */

            while( constituentElIndex < optionalElCnt)
            {
                elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex].executionLeg = 
                    SchFnSet_g[fn_set].schSetConstituentELsAndArgs[mandatoryElCnt + 
                    constituentElIndex + 1].executionLeg;
                updateElArgs(&SchFnSet_g[fn_set].schSetConstituentELsAndArgs[mandatoryElCnt + 
                        constituentElIndex + 1], 
                        &(elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex]), airSf, internalCellIndex);
                constituentElIndex++;
            }
            totalElCountPerSf += optionalElCnt;
            fn_set_count++;
        }
        elArray_p->executionLegArr[0].executionLeg = WAIT_FOR_TDD_SUBFRAME_IND;
/* SPR 23393 Changes Start */
		elArray_p->executionLegArr[0].legSpecificArg1 = 0;
		elArray_p->executionLegArr[0].legSpecificArg2 = 0;
/* SPR 23393 Changes End */
        elArray_p->executionLegArr[1].executionLeg = PROCESS_PENDING_EVENT_FD;
        elArray_p->subFrameExecCount = totalElCountPerSf;
#ifdef OVERSHOOT_CORRECTION
        elArray_p->mandatoryExecutionLegCount = totalElCountPerSf - totaloptionalElCount;
#endif

    }

/*TDD Tick overshoot changes start*/
#if defined (OVERSHOOT_CORRECTION)
    /* SPR 13768 Changes Start */
    /* +- SPR 17777 */
    updateElCycleCountTDD(internalCellIndex);
    /* +- SPR 17777 */
    /* SPR 13768 Changes End */
#endif
/*TDD Tick overshoot changes end*/

}
#else
/*****************************************************************************
 * Function Name  : populateTddELSchedulerArray 
 * Inputs         : cellParams_p - pointer to CellConfig
 * Outputs        : None
 * Returns        : None
 * Description    : Populates EL scheduler array from meta info
 * *****************************************************************************/

void populateTddELSchedulerArray(CellConfigParams *cellParams_p)
{
    SchedulerFunctionalityType (* execSchedulerMnemonic_TddConfig_pp)[FN_MAX] = NULL;
    UInt8 airSf = 0,execFlow = 0;

    for(execFlow=0;execFlow<NUM_OF_MAC_EXEC_FLOWS;execFlow++)
    {    
        execSchedulerMnemonic_TddConfig_pp = &execSchedulerMnemonic_TddConfig_g[cellParams_p->subFrameAssign][execFlow][0];
        for (airSf = 0; airSf < MAX_SUB_FRAME; airSf++)
        {
            UInt8 fn_set_count = 0;
            UInt8 workingSf = airSf + DL_ADVANCE_TICK;
            if (workingSf >= MAX_SUB_FRAME)
            {
                workingSf -= MAX_SUB_FRAME;
            }
            UInt8 totalElCountPerSf = 1;
            TDDSubFrame * elArray_p =  &(executionFlowCreationParamArr_g[execFlow].threadArguments.subFrameInfo[airSf]);
            while( FN_MAX != execSchedulerMnemonic_TddConfig_pp[workingSf][fn_set_count] )
            {
                SchedulerFunctionalityType fn_set = execSchedulerMnemonic_TddConfig_pp[workingSf][fn_set_count];
                UInt8 mandatoryElCnt = SchFnSet_g[fn_set].mandatoryElCount;
                UInt8 optionalElCnt =  SchFnSet_g[fn_set].optionalElCount;
                UInt8 constituentElIndex = 0;
                while( constituentElIndex < mandatoryElCnt)
                {
                    elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex].executionLeg = 
                        SchFnSet_g[fn_set].schSetConstituentELsAndArgs[constituentElIndex].executionLeg;
                    updateElArgs(&SchFnSet_g[fn_set].schSetConstituentELsAndArgs[constituentElIndex], 
                            &(elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex]), airSf);
                    constituentElIndex++;
                }
                totalElCountPerSf += mandatoryElCnt;
                constituentElIndex = 0;
                /*
                 * For optional ELs first EL will be delimiter
                 * MAX_EXECUTION_LEGS
                 * Therefore indexing to next EL
                 */

                while( constituentElIndex < optionalElCnt)
                {
                    elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex].executionLeg =
                        SchFnSet_g[fn_set].schSetConstituentELsAndArgs[mandatoryElCnt +
                        constituentElIndex + 1].executionLeg;
                    updateElArgs(&SchFnSet_g[fn_set].schSetConstituentELsAndArgs[mandatoryElCnt +
                            constituentElIndex + 1],
                            &(elArray_p->executionLegArr[totalElCountPerSf + constituentElIndex]), airSf);
                    constituentElIndex++;
                }
                totalElCountPerSf += optionalElCnt;
                fn_set_count++;
            }
            elArray_p->executionLegArr[0].executionLeg = (execFlow==DL_THD?WAIT_FOR_TDD_SUBFRAME_IND:PROCESS_SUB_FRAME_IND_UL);
            elArray_p->subFrameExecCount = totalElCountPerSf;
        }
    } 

}
#endif

/*****************************************************************************
 * Function Name  : initElArgs 
 * Inputs         : ulGrantPhichTableInfo_p - pointer to UlGrantHIPduSubFrameInfo,
 *                  dlAckNackSubFrameNumInfo_p - pointer to DLAckNackSubFrameNumInfo
 *                  internalCellIndex - Cell-Index at MAC
 *                  ulSfNumFrmPhichInfo_p - points to ulSfNumFrmPhich_g 
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes EL arguments in combined context.
 *****************************************************************************/
/** CA-TDD Changes Start:06 **/
void initElArgs(UlGrantHIPduSubFrameInfo * ulGrantPhichTableInfo_p, 
                DLAckNackSubFrameNumInfo * dlAckNackSubFrameNumInfo_p,
                InternalCellIndex internalCellIndex,
                UInt8 * ulSfNumFrmPhichInfo_p)
{
    UInt8 sf = 0;
    for (sf = 0; sf < MAX_SUB_FRAME; sf++)
    {
        UInt8 workingSf = (sf + DL_ADVANCE_TICK)%MAX_SUB_FRAME;\
        SET_ARG_DL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        SET_ARG_UL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        SET_ARG_SPL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        SET_ARG_DL_HARQ_U_SF(sf, workingSf, ulGrantPhichTableInfo_p, 
                             dlAckNackSubFrameNumInfo_p,internalCellIndex);
        SET_ARG_DL_HARQ_EXPIRY_U_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        SET_ARG_PDSCH_DELAY(sf, workingSf, tddMacPdcchPdschFtnPtrIndexer_p,internalCellIndex);
        /* TDD Config 0, 6 Changes Start */
        SET_ARG_UL_HARQ_EXPIRY_U_SF(sf, workingSf, ulGrantPhichTableInfo_p, ulSfNumFrmPhichInfo_p,internalCellIndex);
        SET_ARG_UL_HARQ_EXPIRY_2ND_U_SF_FOR_CONFIG0(
                sf, workingSf, ulGrantPhichTableInfo_p, ulSfNumFrmPhichInfo_p,internalCellIndex);
        SET_ARG_2ND_UL_SF_FOR_CONFIG0(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        SET_ARG_4TH_TICK_UL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        SET_ARG_4TH_TICK_UL_SPL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex);
        /* TDD Config 0, 6 Changes End */
        //SET_ARG_MEM_CLEANUP(sf, workingSf, ulGrantPhichTableInfo_p);
#ifdef DL_UL_SPLIT_TDD
        SET_ARG_PERF_STATS(sf);
#endif
    }
}

/*****************************************************************************
 * Function Name  : initCountofFnSets 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes count of ELs per FN Set.
 *****************************************************************************/


void initCountofFnSets(void )
{
    UInt32 fnSetIterator = 0;
    for (fnSetIterator =0; fnSetIterator < FN_MAX; fnSetIterator++)
    {
        UInt32 elPerFnSet = 0;
        while (MAX_EXECUTION_LEGS != SchFnSet_g[fnSetIterator].schSetConstituentELsAndArgs[elPerFnSet].executionLeg)
        {
            elPerFnSet++;
        }
        SchFnSet_g[fnSetIterator].mandatoryElCount = elPerFnSet;
        while (MAX_EXECUTION_LEGS != SchFnSet_g[fnSetIterator].schSetConstituentELsAndArgs[elPerFnSet + 1].executionLeg)
        {
            elPerFnSet++;
        }
        SchFnSet_g[fnSetIterator].optionalElCount = elPerFnSet - SchFnSet_g[fnSetIterator].mandatoryElCount;
    }
}



#endif
