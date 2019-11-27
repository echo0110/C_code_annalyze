/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2Types.h,v 1.1.1.1.16.1 2010/09/21 15:53:16 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2Types.h,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:16  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.6  2009/07/24 14:39:08  gur20439
 * support added for non adaptive retransmission for UPLINK.
 *
 * Revision 1.5  2009/07/12 10:16:39  gur15697
 * added macro SBUFFERSIZE
 *
 * Revision 1.4  2009/06/16 07:06:25  gur20439
 * changes for release 1.2.
 *
 * Revision 1.3  2009/06/15 13:51:19  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.2  2009/06/11 08:58:18  gur19413
 * UT bug fixing.
 *
 * Revision 1.1  2009/03/30 10:20:16  gur19836
 * Initial Version
 *
 * Revision 1.41  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.40  2009/03/02 11:03:52  gur18569
 * ultb minimum size is set to 48
 *
 * Revision 1.39  2009/02/24 17:19:52  gur18569
 * changed minimum bsr load weight factor to 0
 *
 * Revision 1.38  2009/02/20 14:32:44  gur18569
 * performance changes
 *
 * Revision 1.37  2009/01/31 19:06:45  gur18569
 * changed value of number of encoder threads
 *
 * Revision 1.36  2009/01/31 01:02:26  gur11912
 * Regression Bugs Fixed
 *
 * Revision 1.35  2009/01/29 13:25:28  gur20052
 * Added Changed related to the Stats MAnager of Uplink
 *
 * Revision 1.34  2009/01/27 20:29:45  gur18569
 * added dispatcherArrInuse flag in dispatcherNode
 *
 * Revision 1.33  2009/01/27 07:12:32  gur20052
 * Incorporated Vandana's Comments
 *
 * Revision 1.32  2009/01/25 11:45:28  gur20052
 * added After Regression tests
 *
 * Revision 1.31  2009/01/24 10:19:38  gur11912
 * Regression bugs fixed
 *
 * Revision 1.30  2009/01/21 08:45:09  gur18569
 * added INUSE macro for dispatcher arr in use
 *
 * Revision 1.29  2009/01/17 20:20:40  gur11447
 * Changes for minimum BSR length
 *
 * Revision 1.28  2009/01/15 05:00:24  gur20052
 * Review comments of vandana are incorporated
 *
 * Revision 1.27  2009/01/08 18:57:38  gur18569
 * Review comments incorporated
 *
 * Revision 1.26  2009/01/08 13:14:08  gur20052
 * Added INVALID_SR_PERIODICITY
 *
 * Revision 1.25  2009/01/05 13:01:58  gur18569
 * added uecontext and harq context in use macros for RLC
 *
 * Revision 1.24  2009/01/01 15:42:52  gur20052
 * Chnages to Avoid Overwriting ACk/NACK with resource allocation
 *
 * Revision 1.23  2008/12/30 05:33:02  gur12905
 * lteMsgPool.h included in file
 *
 * Revision 1.22  2008/12/29 05:36:42  gur20052
 * changes incorporated for RA Queue handling
 *
 * Revision 1.20  2008/12/26 05:36:27  gur11912
 * modified enums for different Data Types
 *
 * Revision 1.19  2008/12/23 11:56:59  gur11912
 * code review comments incorporated
 *
 * Revision 1.18  2008/12/23 05:13:55  gur11912
 * code review comments incorporated for Phase2
 *
 * Revision 1.17  2008/12/22 11:10:44  gur12905
 * MAX_SIZE_DB changed to MAX_NUM_CHANNELS
 *
 * Revision 1.16  2008/12/19 06:58:55  gur18569
 * review comments incorporated for phase2
 *
 * Revision 1.15  2008/12/18 12:34:42  gur12905
 * Review comments incorporated
 *
 * Revision 1.14  2008/12/15 08:45:59  gur20052
 * Upper Ceiling of  RA_WINDOW_SIZE is added
 *
 * Revision 1.13  2008/12/11 13:46:13  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.12  2008/12/10 07:54:10  gur20052
 * Initial version for Phase 2
 *
 * Revision 1.11  2008/12/08 12:58:34  gur12905
 * Modified during phase-2 merging
 *
 * Revision 1.10  2008/12/05 13:48:57  gur18569
 * multicore changs
 *
 * Revision 1.9  2008/12/02 12:08:38  gur11912
 * Incorporated multiple worker threads changes
 *
 * Revision 1.8  2008/10/22 12:25:41  gur11912
 * Fixed for performance changes
 *
 * Revision 1.7  2008/10/14 05:31:48  gur11912
 * LC Priority Range updated
 *
 * Revision 1.6  2008/10/07 08:03:55  gur11912
 * UT Defect fixed
 *
 * Revision 1.5  2008/10/03 08:42:54  gur11912
 * TB Minimum size defined
 *
 * Revision 1.4  2008/10/03 06:16:37  gur11912
 * PADDING Removed from header size calculation
 *
 * Revision 1.3  2008/09/19 07:10:22  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/09 16:16:30  gur11974
 * Updating for integration
 *
 * Revision 1.1  2008/09/09 06:36:04  gur11974
 * "Initial Version"
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_LTELAYER2_TYPES_H
#define INCLUDED_LTELAYER2_TYPES_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteLog.h"
#include "lteHash.h"
#include "lteMemPool.h"
#include "lteList.h"
#include "lteSQueue.h"
#include "lteThread.h"
#include "lteSync.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "port.h"
#include "lteMisc.h"
#include "lteMsgPool.h"
#include "lteLayer2CommanTypes.h"


/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* CLPC_CHG */
/* SPR 2479 Fixes Begins */
/* SPR 7964 changes start  */
#define ACK_NACK_ARRAY_SIZE 10
/* SPR 7964 changes start  */
/* SPR 2479 Fixes Ends */
/*CLPC_CHG*/
#define MAX_NUM_CELLS 2
/* SPR 21660 changes start_Sahil */
/* SPR 21660 changes end_Sahil */
#define MAIN_CELL 0
#define PCELL 1
#define SCELL 2
/* SPR 22501 Fix Start */
/* SPR 20526 Fix reverted */
/* SPR 22579 Review Comments + */
#ifdef HIGH_SCALABILITY_L2_MAX_UE
#define MAX_RNTI_VALUE MAX_UE_SUPPORTED
#else
#define MAX_RNTI_VALUE 600
#endif
/* SPR 22579 Review Comments - */
/* SPR 22501 Fix End */
#define SR_HARQ_NODE_PRESENT 1

#define GENERATE_SUBFRAME_SFNSF(sfn,sf) ( ( ((sfn) & 0x0FFF) << FAPI_SFN_VALUE_SHIFT) | ( (sf) & 0xf) )
/****************************************************************************
 * Exported Types
 ****************************************************************************/

/*CLPC_CHG*/
typedef struct TpcPdcchConfigT
{
    /*Indicates whether PDCCH configuration is setup or released for the UE */
    UInt8 release;
    /*Indicates the index assigned to the UE for receiving TPC command through
     ***** format 3/3A*/
    UInt8 tpcBitMapIndex;
    /*Indicates the assigned TPC RNTI for the UE (present if release== false)*/
    UInt16 tpcRnti;
    /*Configured for DCI Format 3 or 3A*/
    UInt8 isConfiguredForDci3Or3A;
}TpcPdcchConfig;
/*CLPC_CHG*/

/* Structure for defining the MsgType*/
typedef enum UplinkMsgTypeT
{
    NACK = 0,
    ACK,
    DATA
}UplinkMsgType;

/*Boolean enum : To return values true or false */
typedef enum MacRetTypeT
{
    MAC_FAILURE,
    MAC_SUCCESS,
    MAC_PARTIAL_SUCCESS
}MacRetType;

/*Modulation Scheme */
typedef enum ModulationSchemeT
{
    QPSK = 2,
    SIXTEENQAM = 4,
    SIXFOURQAM = 6
} ModulationScheme;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

#define SBUFFERSIZE 10000

#define MAX_NUM_UE                         MAX_UE_SUPPORTED

#define MAX_NUMBER_OF_LOGICAL_CHANNEL      MAX_LOGICAL_CHANNEL

/** SPR 3657 Changes Start **/
#define MAX_NUM_UL_PKTS        106
/** SPR 3657 Changes End **/
#define MAX_NUM_CHANNELS      17
#define MAX_HARQ_PROCESS_NB                8
#ifdef FDD_CONFIG
/** For TTIB_Code */
#define MAX_TTIB_HARQ_PROCESS_NB           4
#endif

#ifdef TDD_CONFIG
#define TDD_MAX_UL_HARQ_PROCESS  7  /* TS 36.213-880, table 8-1 */
#endif

#define MAX_HARQ_RETRANSMISSION           8
#define MAX_UE_PRIORITY_RANGE              3

#define CODING_MULTIPLIER             1024

#define CODING_RATE_120               120
#define CODING_RATE_157               157   
#define CODING_RATE_193               193
#define CODING_RATE_251               251 
#define CODING_RATE_308               308
#define CODING_RATE_379               379
#define CODING_RATE_449               449
#define CODING_RATE_526               526
#define CODING_RATE_602               602
#define CODING_RATE_679               679
#define CODING_RATE_340               340
#define CODING_RATE_378               378
#define CODING_RATE_434               434 
#define CODING_RATE_490               490 
#define CODING_RATE_553               553
#define CODING_RATE_616               616
#define CODING_RATE_658               658
#define CODING_RATE_438               438
#define CODING_RATE_466               466
#define CODING_RATE_517               517
#define CODING_RATE_567               567
#define CODING_RATE_616               616 
#define CODING_RATE_666               666 
#define CODING_RATE_719               719
#define CODING_RATE_772               772
#define CODING_RATE_822               822
#define CODING_RATE_873               873
#define CODING_RATE_910               910
#define CODING_RATE_948               948

#define MCS_INDEX_QPSK_0              0
#define MCS_INDEX_QPSK_1              1
#define MCS_INDEX_QPSK_2              2
#define MCS_INDEX_QPSK_3              3
#define MCS_INDEX_QPSK_4              4
#define MCS_INDEX_QPSK_5              5
#define MCS_INDEX_QPSK_6              6
#define MCS_INDEX_QPSK_7              7
#define MCS_INDEX_QPSK_8              8
#define MCS_INDEX_QPSK_9              9
#define MCS_INDEX_16QAM_10            10
#define MCS_INDEX_16QAM_11            11
#define MCS_INDEX_16QAM_12            12
#define MCS_INDEX_16QAM_13            13
#define MCS_INDEX_16QAM_14            14
#define MCS_INDEX_16QAM_15            15
#define MCS_INDEX_16QAM_16            16
#define MCS_INDEX_64QAM_17            17
#define MCS_INDEX_64QAM_18            18
#define MCS_INDEX_64QAM_19            19
#define MCS_INDEX_64QAM_20            20
#define MCS_INDEX_64QAM_21            21
#define MCS_INDEX_64QAM_22            22
#define MCS_INDEX_64QAM_23            23
#define MCS_INDEX_64QAM_24            24
#define MCS_INDEX_64QAM_25            25
#define MCS_INDEX_64QAM_26            26
#define MCS_INDEX_64QAM_27            27
#define MCS_INDEX_64QAM_28            28

#define TRUE 1
#define FALSE 0
/*SPR 13884 changes start*/
#define INVALID_ACK_NACK  7
/*SPR 13884 changes end */
#define INVALID_BSR_INDEX 65


#ifdef TDD_CONFIG
#define HARQ_PROCESS_FREE 0
#endif
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  
