/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE FUNCTIONS RELATED TO Allocation of
 *                     CCEs on UE Specific and Common Search Space for UE whose 
 *                     either New or Re Transmission need to be done.It also 
 *                     have funtions for  allocation of CCE for Common Channel 
 *                     Messages.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPDCCHMgr.c,v $
 * Revision 1.2.6.1.4.2.2.4.9.1  2012/07/20 06:06:28  gur31292
 * Changes related to DCI Format3A
 *
 * Revision 1.2.6.1.4.2.2.4.8.1  2010/11/08 06:06:28  gur10121
 * Semi static changes merging
 *
 * Revision 1.2.6.1.4.2.2.4.2.2  2010/10/27 17:13:56  gur10121
 * UT fixes + review comments incorporation
 *
 * Revision 1.2.6.1.4.2.2.4  2010/10/14 17:18:12  gur19413
 * DMRS FIXES
 *
 * Revision 1.2.6.1.4.2.2.3.6.1  2010/10/15 18:53:16  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.2.6.1.4.2.2.3  2010/10/01 14:44:13  gur20491
 * mac/maccore/src/scheduler/dlSchManager/lteMacPDCCHMgr.c
 *
 * Revision 1.2.6.1.4.2.2.2  2010/09/27 16:49:16  gur10121
 * SPR 662 Fix
 *
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:46:15  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.138  2009/12/03 10:07:51  gur18550
 * Bug solved related to availableULRBCount_g
 *
 * Revision 1.137  2009/11/07 17:10:19  gur11912
 * applied availableRBCount check
 *
 * Revision 1.136  2009/11/06 06:31:53  gur11912
 * modified for MIMO case
 *
 * Revision 1.135  2009/10/05 08:03:54  gur11912
 * optimization changes
 *
 * Revision 1.134  2009/09/14 07:04:20  gur18550
 * Function Updated
 *
 * Revision 1.133  2009/09/14 06:48:51  gur18550
 * Inc. new PDCCH
 *
 * Revision 1.132  2009/09/03 09:05:41  gur19413
 * bug fixed
 *
 * Revision 1.131  2009/09/02 09:41:35  gur19413
 * isStopAdvanceFlag used instead of isPdcchAllocatedFlag
 *
 * Revision 1.130  2009/09/01 15:16:10  gur12140
 * Solved Bug Related toUL and DL
 *
 * Revision 1.129  2009/08/28 11:45:29  gur11912
 * updated correct CCE Index and fixed memory leak
 *
 * Revision 1.128  2009/08/27 14:36:37  gur11912
 * instead of subFrameNum %= MAX_SUB_FRAME used subFrameNum -= MAX_SUB_FRAME
 *
 * Revision 1.127  2009/08/26 12:07:58  gur12140
 * Optimized for DCI CCE Array
 *
 * Revision 1.126  2009/08/26 08:41:42  gur12140
 * Added Code for Optimized in Dci CCE Array
 *
 * Revision 1.125  2009/08/25 14:06:13  gur12140
 * Optimzed DCI CCE Array
 *
 * Revision 1.124  2009/08/11 14:09:57  gur18550
 * Function Updated
 *
 * Revision 1.123  2009/08/08 08:41:43  gur12140
 * Solved Bug
 *
 * Revision 1.122  2009/08/07 10:24:19  gur18550
 * Removed UMR error of Purify
 *
 * Revision 1.121  2009/08/07 07:39:11  gur12140
 * Removed Purify UMR
 *
 * Revision 1.120  2009/08/07 05:05:16  gur12140
 * Solved Bug related to acessing UL UE Context in
 * allocatedPDCCHForDLNewReAdvanceTX funtion
 *
 * Revision 1.119  2009/08/06 10:11:26  gur12140
 * Removed UMR Read for Purify
 *
 * Revision 1.118  2009/08/06 08:12:19  gur12140
 * Incorporated Comments
 *
 * Revision 1.117  2009/08/06 06:52:39  gur12140
 * Removed printf
 *
 * Revision 1.116  2009/08/06 05:36:40  gur12140
 * Removed printf
 *
 * Revision 1.115  2009/08/04 10:54:54  gur12140
 * Solved Bug Related to Number of Times UE is pushed
 * in Advance DL Queue
 *
 * Revision 1.114  2009/08/04 10:42:57  gur12140
 * Incorporated Comments and Solved Bug
 *
 * Revision 1.113  2009/08/03 08:21:24  gur20052
 * Fix for UT_LOGS
 *
 * Revision 1.112  2009/07/30 13:42:00  gur12140
 * Removed #def code
 *
 * Revision 1.111  2009/07/30 13:40:31  gur12140
 * Added UT_LOGS
 *
 * Revision 1.110  2009/07/29 10:37:11  gur18550
 * HAndling of UL Nack
 *
 * Revision 1.109  2009/07/29 06:46:56  gur18550
 * UL Harq Updated for Non Allocation case
 *
 * Revision 1.108  2009/07/29 06:37:14  gur12140
 * Solved Bug
 *
 * Revision 1.107  2009/07/27 15:12:47  gur15697
 * compilation warnings removed
 *
 * Revision 1.106  2009/07/27 15:08:06  gur15697
 * pritfs removed
 *
 * Revision 1.105  2009/07/26 08:15:40  gur15697
 * compilation warnings removed
 *
 * Revision 1.104  2009/07/23 14:16:24  gur18550
 * MAX_HARQ_RETRANSMISSION_COUNT - 1
 *
 * Revision 1.103  2009/07/22 14:51:00  gur18550
 * ULNACK
 *
 * Revision 1.102  2009/07/22 13:34:49  gur18550
 * UL Nack
 *
 * Revision 1.101  2009/07/22 12:05:13  gur19413
 * Solved Bug Related to PUCCH
 *
 * Revision 1.100  2009/07/22 09:20:03  gur20052
 * Added Debug Logs and debug functions in UT_TESTING flag
 *
 * Revision 1.99  2009/07/22 05:21:54  gur12140
 * Removed Splint Warnings
 *
 * Revision 1.98  2009/07/21 05:32:29  gur12140
 * Added UT LOG
 *
 * Revision 1.97  2009/07/20 07:00:47  gur18550
 * UL NACK Updated
 *
 * Revision 1.96  2009/07/20 05:15:45  gur12140
 * Replaced MACModuleLogDetail_g wih PDCCH_MGR
 *
 * Revision 1.95  2009/07/17 15:21:14  gur18550
 * UL NACK Updated
 *
 * Revision 1.94  2009/07/17 09:46:38  gur18550
 * Logs Updated
 *
 * Revision 1.93  2009/07/16 11:51:19  gur12140
 * Solved Bug
 *
 * Revision 1.92  2009/07/16 06:31:34  gur12140
 * Solved Bug
 *
 * Revision 1.91  2009/07/15 14:23:52  gur12140
 * Solved Bug
 *
 * Revision 1.90  2009/07/15 08:31:17  gur12140
 * Solved Bug
 *
 * Revision 1.89  2009/07/15 06:28:51  gur12140
 * Incorporated Comments and Solved Bug
 *
 * Revision 1.88  2009/07/15 05:28:38  gur18550
 * Log Updated
 *
 * Revision 1.87  2009/07/14 05:00:06  gur12140
 * Incorporated Comments
 *
 * Revision 1.86  2009/07/14 04:30:22  gur12140
 * Removed LOG
 *
 * Revision 1.85  2009/07/14 04:07:27  gur12140
 * Indented Code
 *
 * Revision 1.84  2009/07/12 12:58:09  gur15697
 * ulStrategyAdvanceTxQueue_g initialized
 *
 * Revision 1.83  2009/07/10 06:24:10  gur18550
 * DELAY THREE Updated
 *
 * Revision 1.82  2009/07/08 12:09:14  gur12140
 * Solved Bug Related to CCE Allocation Failuer for UL UEs
 *
 * Revision 1.81  2009/07/07 13:19:33  gur18550
 * DELAY_THREE Added
 *
 * Revision 1.80  2009/07/07 08:56:50  gur15697
 * CCE available check removed in case of PCCH
 *
 * Revision 1.79  2009/07/06 06:17:06  gur12140
 * Solved Bug
 *
 * Revision 1.78  2009/07/04 08:37:14  gur15697
 * preferredRATPolicy = RA_POLICY_2_DISTRIBUTED added for PCCH
 *
 * Revision 1.77  2009/07/04 07:43:02  gur15697
 * review comments incorporated
 *
 * Revision 1.76  2009/07/03 09:54:03  gur15697
 * RA_POLICY_2_DISTRIBUTED added
 *
 * Revision 1.75  2009/07/03 06:38:50  gur12140
 * Incorporated Comments
 *
 * Revision 1.73  2009/07/02 08:31:01  gur12140
 * Wrongly CheckIn Earlier ,now Merged Changes and Checking in
 *
 * Revision 1.72  2009/07/02 08:22:23  gur12140
 * Incorporated Comments
 *
 * * Revision 1.71  2009/07/02 07:17:06  gur15697
 * added dciPduInfo.tb1Info.txIndicator = NEW_TX
 *
 * Revision 1.70  2009/07/01 06:20:36  gur15697
 * LTE_LOG changed to LTE_MAC_UT_LOG
 *
 * Revision 1.69  2009/06/30 07:44:16  gur18550
 * *** empty log message ***
 *
 * Revision 1.68  2009/06/29 13:41:52  gur12140
 * Incorporated Comments
 *
 * Revision 1.66  2009/06/26 07:18:52  gur18550
 * Error removed
 *
 * Revision 1.65  2009/06/24 13:51:19  gur12140
 * Solved Bug
 *
 * Revision 1.64  2009/06/24 08:06:02  gur12140
 * LTE_LOG Replaced by LTE_MAC_UT_LOG
 *
 * Revision 1.62  2009/06/24 07:39:25  gur12140
 * Solved Bug
 *
 * Revision 1.61  2009/06/24 05:59:49  gur12140
 * Solbed Bug
 *
 * Revision 1.60  2009/06/24 04:06:58  gur15697
 * ZERO_BUFFER_COPY removed for BCCH and PCCH
 *
 * Revision 1.59  2009/06/23 07:18:40  gur18550
 * UL NACK DELAY 3
 *
 * Revision 1.58  2009/06/22 11:27:24  gur12140
 * Removed Error
 *
 * Revision 1.57  2009/06/22 10:13:52  gur12140
 * Incorporated Comments
 *
 * Revision 1.55  2009/06/22 06:07:17  gur12140
 * Solved Bug and Optimized it
 *
 * Revision 1.54  2009/06/22 05:20:54  gur18550
 * Final comments inc in UL NACK
 *
 * Revision 1.53  2009/06/22 04:52:34  gur15697
 * review comments incorporated
 *
 * Revision 1.52  2009/06/20 14:44:33  gur12140
 * Solbed Bug
 *
 * Revision 1.51  2009/06/19 14:45:29  gur12140
 * Added funtion to print UE Specific Search Space
 *
 * Revision 1.50  2009/06/19 13:30:35  gur12140
 * Solved Bug in isCCEAvailableAggrLvlOneUESpace isCCEAvailableAggrLvlTwoUESpace
 * isCCEAvailableAggrLvlFourUESpace isCCEAvailableAggrLvlEightUESpace
 *
 * Revision 1.49  2009/06/19 09:07:34  gur12140
 * Incorporated Comments
 *
 * Revision 1.48  2009/06/19 05:37:06  gur12140
 * Solved Bug and Optimized It
 *
 * Revision 1.47  2009/06/18 15:02:02  gur12140
 * Solved Bug in isCCEAvailableAggrLvlOneUESpace
 * isCCEAvailableAggrLvlTwoUESpace
 * isCCEAvailableAggrLvlFourUESpace
 * isCCEAvailableAggrLvlEightUESpace funtions
 *
 * Revision 1.46  2009/06/18 10:41:37  gur12140
 * Solved Bug
 *
 * Revision 1.45  2009/06/18 09:14:55  gur15697
 * lowerCRNTIBound_g and upperCRNTIBound_g modified to  
 * lowerRNTIBound_g and upperRNTIBound_g
 *
 * Revision 1.44  2009/06/18 07:53:06  gur12140
 * Removed Bugs and Optimized it
 *
 * Revision 1.43  2009/06/17 13:23:53  gur12140
 *  Solved Bug
 *
 * Revision 1.41  2009/06/17 05:50:45  gur12140
 * Incorporated Comments
 *
 * Revision 1.40  2009/06/16 15:23:50  gur12140
 * Removed Compilation Error in Random Access
 *
 * Revision 1.39  2009/06/16 14:15:33  gur12140
 * Incorporated Comments
 *
 * Revision 1.38  2009/06/16 05:40:20  gur15697
 * maxCCEInCommonSpace_g added
 *
 * Revision 1.37  2009/06/15 08:57:52  gur18569
 * made maxCCEAvailable_g global
 *
 * Revision 1.36  2009/06/12 05:03:55  gur12140
 * Incorporated Comments
 *
 * Revision 1.35  2009/06/10 09:11:06  gur12140
 * Incorporated Comments
 *
 * Revision 1.34  2009/06/10 08:43:57  gur18550
 * Compilation Error Removed
 *
 * Revision 1.33  2009/06/09 14:17:10  gur18550
 * UL NACK ADDED
 *
 * Revision 1.32  2009/06/05 16:30:18  gur20052
 * updated the names of putEntry In DL/UL Scheduler ReTx/New Queue
 *
 * Revision 1.31  2009/06/05 11:01:48  gur12140
 * Removed Linking Errors
 *
 * Revision 1.30  2009/06/05 08:11:49  gur12140
 * Removed Linking Errors
 *
 * Revision 1.29  2009/06/05 04:11:25  gur15697
 * allocatePdcchForBCCH() updated
 *
 * Revision 1.28  2009/06/04 14:36:10  gur12140
 * Revision History Added.
 *
 * Revision 1.27  2009/06/04 14:33:20  gur12140
 * Incorporated Comments
 *
 * Revision 1.26  2009/06/04 09:58:01  gur15697
 * allocatePdcchCommonChannel() modified
 *
 * Revision 1.25  2009/06/02 13:15:22  gur15697
 * changed freeHarqProcess to freeDLHarqProcess
 *
 * Revision 1.24  2009/06/01 12:56:50  gur12140
* Removed Compilation Error due to change in cellConfig_g structure
*
* Revision 1.23  2009/06/01 07:42:52  gur12140
* Compilation Error Removed Due to DLUEContext structure Changes
*
* Revision 1.22  2009/05/27 08:43:48  gur15697
* RACH functionality added
*
* Revision 1.21  2009/05/21 08:27:10  gur12140
* Incorporated HARQ Structrue Changes
*
* Revision 1.20  2009/05/20 12:09:53  gur12140
* Incorporated Comments
*
* Revision 1.18  2009/05/18 08:56:55  gur15697
* Function header updated
*
* Revision 1.17  2009/05/18 08:30:47  gur15697
* allocatePdcchForBCCH() updated
*
* Revision 1.16  2009/05/18 06:54:35  gur15697
* allocatePdcchForPCCH() updated
*
* Revision 1.15  2009/05/12 06:49:23  gur15697
* CCE allocation for BCCH added
*
* Revision 1.14  2009/05/11 13:24:07  gur12140
* Modified UESpecificSearchSpace Funtion in PCCH
*
* Revision 1.13  2009/05/11 10:59:18  gur12140
* Did Changes as some funtionality was missing.
*
* Revision 1.12  2009/05/10 09:13:57  gur12140
* Removed extern defintion of rrcConfig_g
*
* Revision 1.11  2009/05/10 07:53:32  gur12140
* Incorporated Changes for DCI CCE Info structure as discussed in Team
*
* Revision 1.10  2009/05/09 16:33:55  gur12140
* Made file Compiled
*
* Revision 1.9  2009/05/08 14:52:33  gur12140
* Modified Files for removing Warnings
*
* Revision 1.5  2009/05/05 13:07:29  gur15697
* Common Space CCE allocation functions updated
*
* Revision 1.4  2009/05/05 13:01:38  gur15697
* Common Space CCE allocation functions updated
*
* Revision 1.3  2009/05/05 09:10:19  gur12140
* Modifed File
*
* Revision 1.1  2009/04/30 05:35:59  gur12140
* INITIAL VERSION
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/


/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacPDCCHMgr.h"
#include "lteMacRandomAccess.h"
#include "lteMacCellConfMgr.h"
#include "lteMacPDSCHMgr.h"
#include "lteMacPUSCH.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacPRACH.h"
#include "lteMacULQosScheduler.h"
#include "lteMacTBSize.h"
#ifdef FDD_CONFIG
#include "lteMacPHICH.h"
#endif
#ifdef TDD_CONFIG
#include "lteMacReceiver.h"
#include "lteMacTDDConfig.h"
#include "lteMacTddMgr.h"
#endif
#include "lteMacStatsManager.h"
#include "lteMacPowerControl.h"
#include "lteMacDLPowerControlData.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacSpsPUSCHmgr.h"
#include "lteMacPRSMgr.h"
#include "ltePerfStats.h"
#include "lteMacCCCH.h"
#include "lteCommonQueueDefs.h"
#include "lteMacECIDMgr.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
extern TddPdschTxMapOfDLHarq *tddPdschTxMapOfDLHarq_p[MAX_NUM_CELL];
#ifdef FDD_CONFIG
/* SPR 6363 FIX start */ 
/* Queue for TTI bundling scheduling */
TTIB_SCHEDULE_TYPE*     ttibScheduleQueue_gp[MAX_NUM_CELL] = {PNULL};
TtiBundleMapQueueInfo   ttiBundleMapQueueInfo_g[MAX_TTIB_SCHEDULE_QUEUE];
/* SPR 6363 FIX END */ 
/* +- SPR 18481 */
/* As per 36.212 Rel 10 The DCI calculation has been made
after considering all the optional parameters and padding.
These are used only for calculation of CCE,and not for sending
the bits(which is done by L1)*/
UInt8 dciSize_g[MAX_DCI_FORMATS][MAX_DL_BW][ANTENNA_PORT_VARIATION]=
{

    {/* DCI_FORMAT_0 */
        /* No of Antennas {1,2,3,4}*/
        {26,26,26,26},/*1.4MZ*/
        {28,28,28,28},/*3MZ*/
        {29,29,29,29},/*5MZ*/
        {33,33,33,33},/*10MZ*/
        {34,34,34,34},/*15MZ*/
        {35,35,35,35} /*20MZ*/
    },
    {/* DCI_FORMAT_1 */
        {23,23,23,23},/*1.4MZ*/
        {25,25,25,25},/*3MZ*/
        {33,33,33,33},/*5MZ*/
        {34,34,34,34},/*10MZ*/
        {36,36,36,36},/*15MZ*/
        {42,42,42,42} /*20MZ*/
    },
    {/* DCI_FORMAT_1A */
        /* No of Antennas {1,2,3,4}*/
        {26,26,26,26},/*1.4MZ*/
        {28,28,28,28},/*3MZ*/
        {29,29,29,29},/*5MZ*/
        {33,33,33,33},/*10MZ*/
        {34,34,34,34},/*15MZ*/
        {35,35,35,35} /*20MZ*/
    },
    {/* DCI_FORMAT_1B */
        /* No of Antennas {1,2,3,4}*/
        {25,25,27,27},/*1.4MZ*/
        {27,27,29,29},/*3MZ*/
        {29,29,31,31},/*5MZ*/
        {31,31,33,33},/*10MZ*/
        {33,33,34,34},/*15MZ*/
        {33,33,35,35} /*20MZ*/
    },
    {/* DCI_FORMAT_1C */
        /* No of Antennas {1,2,3,4}*/
        {9,9,9,9},/*1.4MZ*/
        {11,11,11,11},/*3MZ*/
        {13,13,13,13},/*5MZ*/
        {13,13,13,13},/*10MZ*/
        {15,15,15,15},/*15MZ*/
        {15,15,15,15} /*20MZ*/
    },
    {/* DCI_FORMAT_1D */
        /* No of Antennas {1,2,3,4}*/
        {25,25,27,27},/*1.4MZ*/
        {27,27,29,29},/*3MZ*/
        {29,29,31,31},/*5MZ*/
        {31,31,33,33},/*10MZ*/
        {33,33,34,34},/*15MZ*/
        {33,33,35,35} /*20MZ*/
    },
    {/* DCI_FORMAT_2 */
        /* No of Antennas {1,2,3,4}*/
        {34,34,37,37},/*1.4MZ*/
        {37,37,41,41},/*3MZ*/
        {45,45,47,47},/*5MZ*/
        {46,46,49,49},/*10MZ*/
        {48,48,51,51},/*15MZ*/
        {54,54,57,57} /*20MZ*/
    },
    {/* DCI_FORMAT_2A */
        /* No of Antennas {1,2,3,4}*/
        {31,31,33,33},/*1.4MZ*/
        {34,34,36,36},/*3MZ*/
        {41,41,43,43},/*5MZ*/
        {43,43,45,45},/*10MZ*/
        {45,45,47,47},/*15MZ*/
        {51,51,53,53} /*20MZ*/
    },
    {/* DCI_FORMAT_2B */
        /* No of Antennas {1,2,3,4}*/
        {33,33,33,33},/*1.4MZ*/
        {35,35,35,35},/*3MZ*/
        {42,42,42,42},/*5MZ*/
        {45,45,45,45},/*10MZ*/
        {46,46,46,46},/*15MZ*/
        {52,52,52,52} /*20MZ*/
    },
    {/* DCI_FORMAT_2C */
        /* No of Antennas {1,2,3,4}*/
        {34,34,34,34},/*1.4MZ*/
        {37,37,37,37},/*3MZ*/
        {45,45,45,45},/*5MZ*/
        {46,46,46,46},/*10MZ*/
        {48,48,48,48},/*15MZ*/
        {54,54,54,54} /*20MZ*/
    },
    {/* DCI_FORMAT_3 */
        /* No of Antennas {1,2,3,4}*/
        {26,26,26,26},/*1.4MZ*/
        {28,28,28,28},/*3MZ*/
        {29,29,29,29},/*5MZ*/
        {33,33,33,33},/*10MZ*/
        {34,34,34,34},/*15MZ*/
        {35,35,35,35} /*20MZ*/
    },
    {/* DCI_FORMAT_3A */
        /* No of Antennas {1,2,3,4}*/
        {26,26,26,26},/*1.4MZ*/
        {28,28,28,28},/*3MZ*/
        {29,29,29,29},/*5MZ*/
        {33,33,33,33},/*10MZ*/
        {34,34,34,34},/*15MZ*/
        {35,35,35,35} /*20MZ*/
    },
    {/* DCI_FORMAT_4 */
        /* No of Antennas {1,2,3,4}*/
        {34,34,37,37},/*1.4MZ*/
        {36,36,39,39},/*3MZ*/
        {38,38,41,41},/*5MZ*/
        {41,41,45,45},/*10MZ*/
        {41,41,45,45},/*15MZ*/
        {42,42,45,45} /*20MZ*/
    },
    {/* DCI_FORMAT_0_MSG3 */
        /* No of Antennas {1,2,3,4}*/
        {26,26,26,26},/*1.4MZ*/
        {28,28,28,28},/*3MZ*/
        {29,29,29,29},/*5MZ*/
        {33,33,33,33},/*10MZ*/
        {34,34,34,34},/*15MZ*/
        {35,35,35,35} /*20MZ*/
    }
};
    /* +- SPR 18481 */

#elif TDD_CONFIG
/* +- SPR 18481 */
/* As per 36.212 Rel 10 The DCI calculation has been made
after considering all the optional parameters and padding.
These are used only for calculation of CCE,and not for sending
the bits(which is done by L1)*/
UInt8 dciSize_g[MAX_DCI_FORMATS][MAX_DL_BW][ANTENNA_PORT_VARIATION]=
{

    {/* DCI_FORMAT_0 */
        /* No of Antennas {1,2,3,4}*/
        {30,30,30,30},/*1.4MZ*/
        {33,33,33,33},/*3MZ*/
        {33,33,33,33},/*5MZ*/
        {37,37,37,37},/*10MZ*/
        {38,38,38,38},/*15MZ*/
        {39,39,39,39} /*20MZ*/
    },
    {/* DCI_FORMAT_1 */
        {26,26,26,26},/*1.4MZ*/
        {28,28,28,28},/*3MZ*/
        {35,35,35,35},/*5MZ*/
        {37,37,37,37},/*10MZ*/
        {39,39,39,39},/*15MZ*/
        {45,45,45,45} /*20MZ*/
    },
    {/* DCI_FORMAT_1A */
        /* No of Antennas {1,2,3,4}*/
        /* No of Antennas {1,2,3,4}*/
        {30,30,30,30},/*1.4MZ*/
        {33,33,33,33},/*3MZ*/
        {33,33,33,33},/*5MZ*/
        {37,37,37,37},/*10MZ*/
        {38,38,38,38},/*15MZ*/
        {39,39,39,39} /*20MZ*/
    },
    {/* DCI_FORMAT_1B */
        /* No of Antennas {1,2,3,4}*/
        {28,28,30,30},/*1.4MZ*/
        {30,30,33,33},/*3MZ*/
        {33,33,34,34},/*5MZ*/
        {34,34,36,36},/*10MZ*/
        {35,35,37,37},/*15MZ*/
        {36,36,38,38} /*20MZ*/
    },
    {/* DCI_FORMAT_1C */
        /* No of Antennas {1,2,3,4}*/
        {9,9,9,9},/*1.4MZ*/
        {11,11,11,11},/*3MZ*/
        {13,13,13,13},/*5MZ*/
        {13,13,13,13},/*10MZ*/
        {15,15,15,15},/*15MZ*/
        {15,15,15,15} /*20MZ*/
    },
    {/* DCI_FORMAT_1D */
        /* No of Antennas {1,2,3,4}*/
        {28,28,30,30},/*1.4MZ*/
        {30,30,33,33},/*3MZ*/
        {33,33,34,34},/*5MZ*/
        {34,34,36,36},/*10MZ*/
        {35,35,37,37},/*15MZ*/
        {36,36,38,38} /*20MZ*/
    },
    {/* DCI_FORMAT_2 */
        /* No of Antennas {1,2,3,4}*/
        {37,37,41,41},/*1.4MZ*/
        {41,41,45,45},/*3MZ*/
        {47,47,50,50},/*5MZ*/
        {49,49,52,52},/*10MZ*/
        {51,51,54,54},/*15MZ*/
        {57,57,60,60} /*20MZ*/
    },
    {/* DCI_FORMAT_2A */
        /* No of Antennas {1,2,3,4}*/
        {34,34,36,36},/*1.4MZ*/
        {37,37,39,39},/*3MZ*/
        {45,45,47,47},/*5MZ*/
        {46,46,48,48},/*10MZ*/
        {48,48,50,50},/*15MZ*/
        {54,54,57,57} /*20MZ*/
    },
    {/* DCI_FORMAT_2B */
        /* No of Antennas {1,2,3,4}*/
        {36,36,36,36},/*1.4MZ*/
        {39,39,39,39},/*3MZ*/
        {46,46,46,46},/*5MZ*/
        {48,48,48,48},/*10MZ*/
        {50,50,50,50},/*15MZ*/
        {57,57,57,57} /*20MZ*/
    },
    {/* DCI_FORMAT_2C */
        /* No of Antennas {1,2,3,4}*/
        {38,38,38,38},/*1.4MZ*/
        {41,41,41,41},/*3MZ*/
        {48,48,48,48},/*5MZ*/
        {50,50,50,50},/*10MZ*/
        {52,52,52,52},/*15MZ*/
        {58,58,58,58} /*20MZ*/
    },
    {/* DCI_FORMAT_3 */
        /* No of Antennas {1,2,3,4}*/
        {30,30,30,30},/*1.4MZ*/
        {33,33,33,33},/*3MZ*/
        {33,33,33,33},/*5MZ*/
        {37,37,37,37},/*10MZ*/
        {38,38,38,38},/*15MZ*/
        {39,39,39,39} /*20MZ*/
    },
    {/* DCI_FORMAT_3A */
        /* No of Antennas {1,2,3,4}*/
        {30,30,30,30},/*1.4MZ*/
        {33,33,33,33},/*3MZ*/
        {33,33,33,33},/*5MZ*/
        {37,37,37,37},/*10MZ*/
        {38,38,38,38},/*15MZ*/
        {39,39,39,39} /*20MZ*/
    },
    {/* DCI_FORMAT_4 */
        /* No of Antennas {1,2,3,4}*/
        {39,39,42,42},/*1.4MZ*/
        {41,41,45,45},/*3MZ*/
        {43,43,46,46},/*5MZ*/
        {41,41,45,45},/*10MZ*/
        {46,46,46,46},/*15MZ*/
        {48,48,48,48} /*20MZ*/
    },
    {/* DCI_FORMAT_0_MSG3 */
        /* No of Antennas {1,2,3,4}*/
        {30,30,30,30},/*1.4MZ*/
        {33,33,33,33},/*3MZ*/
        {33,33,33,33},/*5MZ*/
        {37,37,37,37},/*10MZ*/
        {38,38,38,38},/*15MZ*/
        {39,39,39,39} /*20MZ*/
    }
    /* +- SPR 18481 */
};
#endif

/* End of change for VARIBLE Declaration */

/* ICIC changes start */
/*This global variable stores the DCI Format to Aggregation Level Mapping ,Each
  Index represents DCI Format and value to that index represents the 
  aggregation level on which DCI Format is mapped */
/* dciFormatToAggregationLevel_g[0][<dciFormat>] stores the DCI Format to 
   Aggregation Level Mapping for cell centre user & 
   dciFormatToAggregationLevel_g[1][<dciFormat>] stores the DCI Format to 
   Aggregation Level Mapping for cell edge user */
DCIFormatToAggregationLevel dciFormatToAggrLvlMapping_g[MAX_NUM_CELL];   
/* ICIC changes end */
/*DYN_PDCCH_TDD_FIX*/
UInt8 changeCFIInThisTTI_g[MAX_NUM_CELL] = {0};
/* + SPR 8190 */
UInt8 highestCCENumberCommonSpace_g[MAX_NUM_CELL] = {0};
/* - SPR 8190 */
/*DYN_PDCCH_TDD_FIX*/
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
SInt32 availableULRBCount_g[MAX_NUM_CELL] = {0};
#endif
/* TDD Config 0 Changes End */

/* This variable holds the maximum number of PDCCH Order based RA which can be
   handled in one TTI. Its value is configured from RRC, if not configured 
   default value is used*/
UInt8 maxPdcchOrderperTTI_g[MAX_NUM_CELL] ;

/*Queue which maintains list of UEs for which PDCCH order to be sent*/
PDCCH_ORDER_TYPE *pdcchOrderQueue_gp[MAX_NUM_CELL];

/*Queue to trigger add/delete UE from DL Scheduler Active List*/
UL_SYNC_LOSS_TRIGGERQ_DL_TYPE* ulSyncLossTriggerQDL_gp[MAX_NUM_CELL] = {PNULL};

#ifdef DL_UL_SPLIT
/* +- SPR 17777 */
extern SInt32 recvMsgWithIccQ(SInt32 mqdes, const void *msg_ptr);
extern MacRetType putEntryInULSchedulerNewTxQueueUsingICC(ULStrategyTxNode *ulStrategyTxNode_p, InternalCellIndex internalCellIndex);
#endif
/* CA_phase2_csi_code start */
UInt8 servingCellBitMask_g[MAX_NUM_CELL] = {BITMASK_0, 
                                            BITMASK_1};
/* CA_phase2_csi_code end */

/*Queue to trigger add/delete UE from UL Scheduler Active List*/
UL_SYNC_LOSS_TRIGGERQ_UL_TYPE* ulSyncLossTriggerQUL_gp[MAX_NUM_CELL] = {PNULL};

/* Higher Priority Queue than pdcchOrderQueue_g 
 * which maintains list of UEs for which CCE Allocation for PDCCH Order is failed */
LTE_SQUEUE *pdcchOrderAdvanceQueue_gp[MAX_NUM_CELL];

extern UInt8   phichSequenceNoFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_NUM_RBS][MAX_DMRS_VALUE];
#ifdef TDD_CONFIG
PdcchULOutputInfo pdcchULOutputInfo_g[MAX_NUM_CELL][MAX_TDD_UL_PDCCH_CONTAINER][MAX_SUB_FRAME];
/*TDD Config 0 Changes Start*/
PdcchInfo* getPdcchInfoNode(PdcchInfo* localPdcchInfoMap[],
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,UInt32 sysFrameNum,UInt8 ulSfFromNackNode, InternalCellIndex internalCellIndex);
void cleanlocalPdcchInfoMap(PdcchInfo* localPdcchInfoMap[]);
extern UInt8   phichGroupFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_IPHICH_VALUE][MAX_NUM_RBS][MAX_DMRS_VALUE];
/*TDD Config 0 Changes End*/
#elif FDD_CONFIG
/*This is a structure which stores inforamtion of those UL UEs which have 
  been allocated the PDCCH region and for UpLink Grant need to be sent*/
PdcchULOutputInfo *pdcchULOutputInfo_gp[MAX_NUM_CELL];
#endif
DciCCEContainerInfo *dciCCEContainerInfo_gp[MAX_NUM_CELL];
/* SPR 5333 changes start */
#ifdef FDD_CONFIG
STATIC UInt8 maxCCEInCommonSpaceForPDCCH_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
#endif
/* SPR 5333 changes end */
/* + CQI_5.0 */
DCIFormat preferredPrimaryDCIFormatForSingleTB_g[MAX_TRANSMISSION_MODES +  1];  
DCIFormat preferredSecondaryDCIFormatForSingleTB_g[MAX_TRANSMISSION_MODES + 1];
/* - CQI_5.0 */

UInt32  spsCceIndex_g[MAX_NUM_CELL] = {0};
/* UL_MU_MIMO_CHG_START */
/* This parameter keeps the total RB count for MaxDataSize in UL_MU_MIMO.
 *    It is reset to 0 at at the beginning of every tick */
#ifdef FDD_CONFIG
UInt16 aggregateMaxRBsRequired_g[MAX_NUM_CELL];
#endif
/* UL_MU_MIMO_CHG_END */

/* + PRS_CHANGES */
UInt8 allocateCCEsForSIPagingULOnly[MAX_NUM_CELL];
#define CHECK_FOR_PRS_OCCASION(prsConfigInfo_p,dlBandwidth,internalCellIndex)\
{\
    /* If this is PRS ocassion, also PRS and system bandwidth are equal */ \
    /* or if ICIC is configured and left unblocked RBs are less than 6 or */ \
    /* this is muting period then CCEs will be allocated to Paging, SI and UL*/\
    /*  messages only. */ \
\
    if ((prsConfigInfo_p->isPRSConfigured) && (prsConfigInfo_p->prsSubframeCounter) \
	    &&( (!prsConfigInfo_p->prsMutingConfig[prsConfigInfo_p->prsOccasionCounter - 1]) ||\
            (prsConfigInfo_p->prsBandwidth == dlBandwidth) || \
		    (!prsConfigInfo_p->prsResInfo[prsConfigInfo_p->dlPRSRATPoliciesIndex].prsAllowedWithICIC))) \
    {\
	     allocateCCEsForSIPagingULOnly[internalCellIndex] = TRUE;\
    }\
}
/* - PRS_CHANGES */

/* EICIC +*/
UInt8 isUeTypeInvalid_g[MAX_NUM_CELL] = {LTE_FALSE};
extern UInt8 	absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern UInt8   isEicicEnabled_g[MAX_NUM_CELL];
extern UInt8 absPatternIndex_g[MAX_NUM_CELL];

#define CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex)\
  absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) %numBitsAbsPattern) ; \
  if (LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum]) \
  {\
      isUeTypeInvalid_g[internalCellIndex] =  LTE_TRUE; \
 }\
  else\
  {\
      isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;\
   }
  /* EICIC -*/



/*TDD Config 0 Changes Start*/
STATIC  void cleanUpStrategyQueueIfCCENotAvlbl(InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs);
#ifdef TDD_CONFIG
void processUlMUMIMOUe(UeScheduledInfo *ueScheduledInfo_p,
                     PdcchULOutputInfo *pdcchULOutputInfo_p,
                     ULUEContext * ulUEContext_p,
                     ULStrategyTxNode  *txNode_p,
                     UInt8 rbMapNum,
                     InternalCellIndex internalCellIndex);
void updateUeScheduleInfo(ULStrategyTxNode  *txNode_p,
                          UeScheduledInfo *ueScheduledInfo_p,
                          PdcchULNewUEInfo *pdcchULNewUEInfo_p,
                          UInt8 countULNewUE,
                          UInt32 allocatedCCEIndex,
                          UInt8 ulHarqProcessId,
                          /* SPR 11065 Changes Start */
                          UInt8 isNodePresentIn2UlSubframe
                          /* SPR 11065 Changes End */
                          );
/* SPR 11236 Changes Start */
void updateULOutputInfoArray(PdcchULNewUEInfo *pdcchULNewUEInfo_p,
                             UInt8 posInULOutputInfo,
                             UeScheduledInfo *ueScheduledInfo_p,
                             UInt8 ulSubframeNum,
                             /** SPR 14204 Fix : Start **/
                             InternalCellIndex internalCellIndex
                             /** SPR 14204 Fix : End **/
        );
void copyULOutputInfo(PdcchULNewUEInfo *pdcchULNewUEInfo_p,
        UInt8 toIndex,
        UInt8 fromIndex,
        UInt8 ulSubframeNum,
        /** SPR 14204 Fix : Start **/
        InternalCellIndex internalCellIndex
        /** SPR 14204 Fix : End **/
        );
/* SPR 11236 Changes End */
/* + CA_TDD_HARQ_CHNAGES */
/* SPR 15909 fix start */
STATIC void calculateDAI( tickType_t currentGlobaTick,
                   UInt8  tddValidateDaiVal,
                   UInt32 tddDaiLastScheduled,
                   UInt8  *tempDai,
                   /* + SPS_TDD */
                   UInt8 *tempDaiSps);
/* - SPS_TDD */
/* SPR 15909 fix end */
/* - CA_TDD_HARQ_CHNAGES */
#endif
/*TDD Config 0 Changes End*/
/* DL DATA SEP Changes */
 /** SPR 13599 Changes Start **/
 /* SPR 22252 fix start */
STATIC  void cleanUpHarqAndStrategyNode( UInt16 ueIndex,
                        DLUEContext *dlUEContext_p,
                        DLStrategyTxNode *dlStrategyTxNode,
                        UInt8 *isPdcchAllocatedFlag_p,
                        InternalCellIndex internalCellIndex);

STATIC  void updateRBsAndPutEntryinFailureQueue( DLStrategyTxNode *dlStrategyTxNode,
                        InternalCellIndex internalCellIndex );
/* SPR 22252 fix end */
 void resetAllocatedCCEs(DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex);
/** SPR 13599 Changes End **/

/*Cyclomatic Complexity changes - starts here */
STATIC  MacRetType allocPDCCHPdcchPowerOrAggregationEnabledWithWideBandReports(DLUEContext *dlUEContext_p,
                                                                               DCIFormat  *dciFormat,
                                                                               UInt32 *powerOffset,
                                                                               DLStrategyTxNode *dlStrategyNode_p,
                                                                               UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
                                                                               UInt32 *availableCCE_p,
                                                                               UInt32 *allocatedCCEIndex_p,
                                                                               DciCCEInfo *dciCCEInfo_p,
                                                                               DciCCEContainerInfo *dciCCEContainerInfo_p,
                                                                               InternalCellIndex internalCellIndex);
STATIC  MacRetType allocPDCCHPdcchPowerOrAggregationDisabledOrNoWideBandReports(DCIFormat  *dciFormat,
                                                                                DLStrategyTxNode *dlStrategyNode_p,                                     
                                                                                UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
                                                                                UInt32 *availableCCE_p,
                                                                                UInt32 *allocatedCCEIndex_p,
                                                                                DciCCEInfo *dciCCEInfo_p,
                                                                                DLUEContext *dlUEContext_p,
                                                                                UInt32 *powerOffset,
                                                                                DciCCEContainerInfo *dciCCEContainerInfo_p,
                                                                                InternalCellIndex internalCellIndex);
/* SPR 9546 Fix Start */
STATIC  void putEntryInPdcchPdschFailureQueueForTaDrxMsgForQos(
        DLHARQProcess *harqProcess_p,
        UInt16 ueIndex,
        UInt16 timingAdvanceValue,
        InternalCellIndex internalCellIndex );
/* SPR 9546 Fix End */
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
STATIC  MacRetType checkAvailabilityOfHIResources(
        UInt8 hiIndex,
        UInt8 iPhich,
        ULHarqInfo* tempULHarqInfo_p,
        InternalCellIndex internalCellIndex);
#endif
/*TDD Config 0 Changes End*/ 

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 **************************************************************************/
#ifdef TDD_CONFIG
/* + SPS_TDD_Changes */
 MacRetType processULNackQForSpsUEinTDD(
        /*TDD Config 0 changes Start*/
        PdcchInfo* localPdcchInfoMap[],
        /*TDD Config 0 changes End*/
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt8 *maxUeScheduledInUplink_p,
        InternalCellIndex internalCellIndex
        );
/* - SPS_TDD_Changes */

#endif
#ifdef FDD_CONFIG
MacRetType processTTIBundleReTxQ(
         UInt8 ulDelay,
         /* SPR 15909 fix start */
         tickType_t currentGlobalTick,
         /* SPR 15909 fix end */
         UInt32 *maxUeScheduledInUplink_p,
         InternalCellIndex internalCellIndex
         );
MacRetType processULNackQForTTIBundledUe(
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        UInt32 *maxUeScheduledInUplink_p,
        InternalCellIndex  internalCellIndex
        );
#endif
#ifdef FDD_CONFIG
#ifdef DL_UL_SPLIT
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQ(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
         /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
         /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        ContainerNackQueueNode * nackNode_p
        /* SPR 5798 Fix Start */
        ,UInt32 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex  internalCellIndex
        );
#else
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQ(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
         /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
         /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p
        /* SPR 5798 Fix Start */
        ,UInt32 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex  internalCellIndex
        );
#endif

#ifndef DL_UL_SPLIT
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQForTempUE(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
         /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p
        /* SPR 5798 Fix Start */
        ,UInt32 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex  internalCellIndex
        );
#else
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQForTempUE(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
         /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        ContainerNackQueueNode * nackNode_p
        /* SPR 5798 Fix Start */
        ,UInt32 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex  internalCellIndex
        );
#endif
#endif // FDD_CONFIG

/* SPR 5599 changes start (ZIP ID 129195) */            
void processPdcchOrderQueue (
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 *availableCCE_p,
        DciCCEContainerInfo * dciCCEContainerInfo_p,
        DciCCEInfo * dciCCEInfo_p,
        UInt32 subFrameNum,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick
         /* SPR 15909 fix end */
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        );

/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processPdcchOrderQueueNode(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 *availableCCE_p,
        DciCCEContainerInfo * dciCCEContainerInfo_p,
        DciCCEInfo * dciCCEInfo_p,
        UInt32 subFrameNum,
        PDCCHOrderNode *pdcchOrderNode_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick
         /* SPR 15909 fix end */
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        );

/* SPR 5599 changes start (ZIP ID 129195) */            
/* spr 23717 fix + */
 void putEntryInUlSyncLossOnOffTriggerQ(UInt32 ueIndex,
/* SPR 5599 changes end (ZIP ID 129195) */            
         UlSyncLossOnOff onOffTrigger,
         InternalCellIndex internalCellIndex
         );

/* SPR 5599 changes start (ZIP ID 129195) */            
/* SPR 15909 fix start */
/* +- SPR 18268 */
 MacRetType putEntryInPdcchOrderQ(UInt8 preambleIndex, UInt16 ueIndex, tickType_t currentGlobalTTI, InternalCellIndex internalCellIndex);
/* spr 23717 fix - */
/* +- SPR 18268 */
/* SPR 15909 fix end */ 
/* SPR 5599 changes end (ZIP ID 129195) */            

STATIC MacRetType allocatePDCCHForDLNewReAdvanceTx(
        DLStrategyTxNode *dlStrategyNode_p,
        UInt32 *allocatedCCEIndex_p,
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
        UInt32 *availableCCE_p,   
        DciCCEInfo *dciCCEInfo_p,
        DLUEContext *dlUEContext_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType allocatePDCCHForULNewReTx(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UESpecificSearchSpaceInfo*ueSearchSpaceInfo_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo * dciCCEInfo_p,
        ULUEContext * ulUEContext_p,
        ULStrategyTxNode  *txNode_p, //CA_phase2_csi_code
        InternalCellIndex internalCellIndex);

#ifdef FDD_CONFIG
STATIC MacRetType  allocatePDCCHForUL(
        ULStrategyTxNode  *txNode_p,
        UESpecificSearchSpaceInfo 
        *ueSearchSpaceInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        DciCCEInfo *dciCCEInfo_p,
        UInt32 *availableCCE_p,
        ULUEContext *ulUEContext_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);

#elif TDD_CONFIG
/*TDD Config 0 Changes Start*/
STATIC MacRetType  allocatePDCCHForUL(
        ULStrategyTxNode  *txNode_p,
        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        DciCCEInfo *dciCCEInfo_p,
        UInt32 *availableCCE_p,
        UeScheduledInfo *ueScheduledInfo_p,
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        PdcchInfo* localPdcchInfoMap[],
        UInt8 rbMapNum,
        UInt8 *maxUeScheduledInUplink_p);
/*TDD Config 0 Changes End*/
#endif

/** SPR 13599 Changes Start **/
#ifdef FDD_CONFIG
STATIC  MacRetType allocatePDCCHDLReTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DirectIndexingUEInfo *directIndexingUEInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        );
#elif TDD_CONFIG
STATIC  MacRetType allocatePDCCHDLReTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        );
#endif

#ifdef FDD_CONFIG
STATIC  MacRetType allocatePDCCHDLNewTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DirectIndexingUEInfo *directIndexingUEInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        );
STATIC  void  allocatePDCCHULTx(
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo *dciCCEInfo_p,
        UInt32 subFrameNum,
        DciCCEContainerInfo *dciCCEContainerInfo_p
        /* SPR 5798 Fix Start */
        ,UInt32 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex internalCellIndex
        ,ExecutionLegFunctionArg* execArgs
        );
#elif TDD_CONFIG
STATIC  MacRetType allocatePDCCHDLNewTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        );
#endif

/** SPR 13599 Changes End **/
/*START : DYN_PDCCH*/
                  /* SPR 15909 fix start */
/* spr 23717 fix + */
 UInt8 calculateCFI(tickType_t currentGlobalTickWithDelay,
/* spr 23717 fix - */
                  /* SPR 15909 fix end */
                          PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                          /* +- SPR 17777 */
#ifdef FDD_CONFIG
                          PdcchULOutputInfo *pdcchULOutputInfo_p,
#endif
                          /* +- SPR 17777 */
                          DLULConfigType ulDlConfigInSF,
                          UInt8 *totalCCERequired
                          ,InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
UInt8 calculateRequiredCCEs(tickType_t currentGlobalTickWithDelay,
 /* SPR 15909 fix end */
                          PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                          /* +- SPR 17777 */
#ifdef FDD_CONFIG
                          PdcchULOutputInfo *pdcchULOutputInfo_p,
#endif
                          /* +- SPR 17777 */
                          DLULConfigType ulDlConfigInSF,
                          InternalCellIndex internalCellIndex);


UInt8 minimumCCERequiredDLNewTx(SInt32* availableDLRBCount,
                          UInt8* maxUEsToBeScheduledDL,
			  PdcchDLOutputInfo *pdcchDLOutputInfo_p,
              InternalCellIndex internalCellIndex);

UInt8 minimumCCERequiredDLReTx(SInt32* availableDLRBCount,
                          UInt8* maxUEsToBeScheduledDL,
                          PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                          /* SPR 15909 fix start */
                          /* +- SPR 17777 */
#ifdef TDD_CONFIG
                          tickType_t currentGlobalTickWithDelay,
#endif
                          /* +- SPR 17777 */
                          /* SPR 15909 fix end */
                          InternalCellIndex internalCellIndex);

#ifndef DL_UL_SPLIT_TDD
UInt8 calculateCCERequiredInULNewTx(SInt32* availableULRBCount,
                          UInt8* maxUEsToBeScheduledUL,
                          /* +- SPR 17777 */
#ifdef FDD_CONFIG
                          PdcchULOutputInfo *pdcchULOutputInfo_p,
#endif
                          /* +- SPR 17777 */
                          /* +- SPR 17777 */
                          InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
UInt8 calculateCCERequiredInULReTx(tickType_t currentGlobalTickWithDelay,
/* SPR 15909 fix end */
                          SInt32* availableULRBCount,
                          UInt8* maxUEsToBeScheduledUL,
                          InternalCellIndex  internalCellIndex);
#else
/* + SPR 17439 */
UInt8 calculateCCERequiredInULTx(void);
/* - SPR 17439 */
#endif
UInt8 getArrayIndexFromAggregationLevel(UInt8 aggregationLevel);
#ifdef TDD_CONFIG
/* + SPR 17439 */
 void calculateMaxCCEwithoutPhich(void);
 void calculateMaxCCEForSpecialSF(void);
 void calculateMaxCCESpecialSFWithoutPhich(void);
/* - SPR 17439 */ 
#endif

STATIC void initDCICCEInfoContainer(InternalCellIndex  internalCellIndex);

/* spr 23717 fix + */
STATIC  void calculateCCEsDCIFormat2A(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		);

STATIC  void calculateCCEsDCIFormat2(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		);

STATIC  void  calculateCCEsDCIFormat1B(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     modulationScheme,
        UInt32 codingRate
		);

STATIC  void  calculateCCEsDCIFormat1A(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     modulationScheme,
        UInt32 codingRate
		);

STATIC  void  calculateCCEsDCIFormat1(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		);

STATIC  void  calculateCCEsDCIFormat0(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     modulationScheme,
        UDouble32 codingRate
		);

/* + TM7_8 Changes Start */
STATIC  void calculateCCEsDCIFormat2B(
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		);
/* spr 23717 fix - */

/* - TM7_8 Changes End */

STATIC void calculateDCIFormatToCCEMapping(InternalCellIndex internalCellIndex);

STATIC  void initUESecificSearchSpace(
        UInt32 ueIndex ,
        UInt16 ueId,
/* SPR 5333 changes start */
#ifdef FDD_CONFIG
        UInt8 maxNumCCE,
        /* +- SPR 17777 */
#endif
/* SPR 11353 Start */
        UInt8 cellIndex,
/* SPR 11353 End */

/* SPR 5333 changes end */
        InternalCellIndex internalCellIndex
	);

STATIC void initializeDirectIndexingArray(InternalCellIndex internalCellIndex);



#ifndef DL_UL_SPLIT
MacRetType processULNackQForSpsUE(
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex  internalCellIndex);
#endif

/* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
void setCurrentCFIValueInPDCCH(CellConfigParams *cellParams_p,
        UInt32 maxCommonSpaceCCE,
        UInt32 *availableCCE_p,
        UInt8 totalCCERequired,
        InternalCellIndex  internalCellIndex);
#endif
/* Cyclomatic_complexity_changes_end */
void eicicUpdateSfAndSfn(UInt32 *sysFrameNum,UInt32 *subFrameNum);
UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex);
/* + SPR 22243 Fix*/
void putEntryInPdcchPdschFailureDRXQueue( UInt32 ueIndex, 
                                         InternalCellIndex internalCellIndex);
/* - SPR 22243 Fix*/
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
extern UInt8 rbgSize_g[MAX_NUM_CELL];
#endif
/* SPR 21958 PUSCH RAT1 Support End */
extern RNTIInfo *crntiToIdxTbl_g[MAX_NUM_CELL];

extern UInt16 lowerRNTIBound_g;

extern UInt16 crntiRange_g;

/* CR changes start */ 
#ifdef FDD_CONFIG

extern PHICHSequenceNoPerPhichGroup phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_PHICH_CONTAINER][MAX_PHICH_GRP];
/* CR changes end */ 
#elif TDD_CONFIG
extern UInt8 phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_SUB_FRAME][MAX_PHICH_GRP];
#endif
extern UInt32  conBasedPDCCHOrderTimerValue_g;



/* CLPC_CHG */
extern LTE_SQUEUE* scheduledFormat3Or3AQueueForPusch_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *scheduledFormat3Or3AQueueForPucch_gp[MAX_NUM_CELL];
extern TpcRntiContext * tpcPuschRntiContext_gp[MAX_NUM_CELL];
extern TpcRntiContext * tpcPucchRntiContext_gp[MAX_NUM_CELL];
extern LTE_SQUEUE tpcTriggerQueueForPucch_g;
/* SPR 15909 fix start */
extern void triggerDciForPucchPowerControl(tickType_t currentTick, InternalCellIndex internalCellIndex);
extern void triggerDciForPuschPowerControl(tickType_t currentTick, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
/* SPR 11700 Start */
extern UInt8 getDlAckNackOffset( UInt8 subframeNum,InternalCellIndex internalCellIndex);
/* SPR 11700 End */
#ifdef DL_UL_SPLIT
extern void putEntryInPuschSignalQueueUsingICC(SInt8 isCCEAllocated, InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
void initPUSCHSignalQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
#endif
/* CLPC_CHG END */

extern RACHGlobals *rachGlobals_gp[MAX_NUM_CELL];
/****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/*This is List which is populated by DL Stategy for those UE for which DL 
  fresh transmission need to be done .This is read by PDCCH for it's processing
  */
LTE_SQUEUE *dlStrategyNewTxQueue_gp[MAX_NUM_CELL];

/*This is List which is populated by DL Stategy for those UE for which DL Re 
  transmission need to be done .This is read by PDCCH for it's processing*/
LTE_SQUEUE   *dlStrategyReTxQueue_gp[MAX_NUM_CELL];

/*This is List which is populated by UL Stategy for those UE for which PDCCH
  region need to be allocated so the Uplink Grant Message can be sent */

UL_STRATEGY_TX_CIRC_TYPE* ulStrategyTxCircQueue_gp[MAX_NUM_CELL] = {PNULL};

#ifdef DL_UL_SPLIT
LTE_SQUEUE *ulStrategyReTxQueue_gp[MAX_NUM_CELL] = {PNULL};
LTE_SQUEUE *ulStrategyNewTxQueue_gp[MAX_NUM_CELL]= {PNULL};
#ifdef CIRC_QUEUE_IMPL
extern SEM_T waitForULStrategyTxInd_g[MAX_NUM_CELL];

extern SEM_T waitForULNackQueue_g[MAX_NUM_CELL];
extern SEM_T waitForULNackForTempUE_g[MAX_NUM_CELL];
#else
extern LTE_SQUEUE* ulStrategyTxQueue_gp[MAX_NUM_CELL];
extern SInt32 ulStrategyTxQueueIdDL_g[MAX_NUM_CELL];
#endif
#else
LTE_SQUEUE* ulStrategyTxQueue_gp[MAX_NUM_CELL] = {PNULL};
#endif
/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
/* For TDD config 0 ,S subframe ulStrategyTdd0SsfTxList_g will be used to sotre 
 * the StrategyTx node of ulSubframe +k and ulStrategyTdd0SsfTxListUSfOffset7_g 
 * will be used for ulSubframe +7 */
/* CA TDD CHANGES START */
LTE_LIST ulStrategyTdd0SsfTxList_g[MAX_NUM_CELL];
LTE_LIST ulStrategyTdd0SsfTxListUSfOffset7_g[MAX_NUM_CELL];
/* CA TDD CHANGES END */
#endif
/* TDD Config 0 Changes End */

#ifdef DL_UL_SPLIT_TDD
LTE_CIRCQUEUE pdcchSignalCircQueue_g;
#endif

/*This is List which is populated by UL Stategy for those UE for which PDCCH
  region allocation has been failed in previous tick and for which in current tick PDCCH 
  region need to be allocated so the Uplink Grant Message can be sent */
LTE_SQUEUE *ulStrategyAdvanceTxQueue_gp[MAX_NUM_CELL];

/*This stores the Number of OFDMA Symbol used for PDCCH Region */
/*START : DYN_PDCCH*/
UInt8 valueCFI_g[MAX_NUM_CELL]; 
/*END   : DYN_PDCCH*/ 
/*START : DYN_PDCCH*/
UInt8 contentionFactor_g[AGGREGATION_LEVEL_MAX];
extern RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
extern UInt8 cceCorrectionStepUpFactor_g[AGGREGATION_LEVEL_MAX];
extern UInt8 cceCorrectionStepDownFactor_g[AGGREGATION_LEVEL_MAX];
UInt8 currentCFIValue_g[MAX_NUM_CELL];
/*DYN_PDCCH_TDD_FIX*/
UInt8 currentsubframe_g;
/*DYN_PDCCH_TDD_FIX*/
/*END   : DYN_PDCCH*/ 

/*Dyanmic PDCCH + 5117 changes starts */
/* SPR 5384 changes start */
/*START : DYN_PDCCH*/
#ifdef FDD_CONFIG
UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
#else
UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE][MAX_VALUE_M_I];
#endif
/*END   : DYN_PDCCH*/
/* SPR 5384 changes end */
/*Dyanmic PDCCH + 5117 changes ends */
/* SPR 5333 changes start */
#ifdef FDD_CONFIG
/*This strores the Maximum Number of CCE Available for PDCCH Region */
/*This strores the Maximum Number of CCE Available for PDCCH Region 
 *In common search space
 */
/*START : DYN_PDCCH*/
UInt8  maxCCEInCommonSpace_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
/*END   : DYN_PDCCH*/
#else
/*This strores the Maximum Number of CCE Available for PDCCH Region */
/*This strores the Maximum Number of CCE Available for PDCCH Region 
 *In common search space
 */
/*START : DYN_PDCCH*/
/*CA TDD CHANGES START*/
UInt8  maxCCEInCommonSpace_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE][MAX_VALUE_M_I];
/*CA TDD CHANGES END */
/*END   : DYN_PDCCH*/
/** CA-TDD Changes Start **/
UInt8  miValueForSubframe_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
#endif
/* SPR 5333 changes end */

/*This is Queue and stores information of Those UE for which the Resources
  have been reserved in Advance */
extern LTE_SQUEUE *advanceAllocInfoQueue_gp[MAX_NUM_CELL];
/* SPR 5295 changes start*/
extern BufferPool ulBufferPool_g[MAX_NUM_CELL];
/* SPR 5295 changes end*/
/* CA_phase2_harq_code Start */
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
#ifdef LTE_EMBMS_SUPPORTED
extern EmbmsConfigParams embmsConfigParams_g[MAX_NUM_CELL];
extern UInt8 isEmbmsValidSf_g[MAX_NUM_CELL];
extern UInt8 isDci1cOcassion_g[MAX_NUM_CELL];
#endif

/********************************************************************************************************
 * Function Name    : isCCEAvailableAggrLvlOneUESpaceforCAUE
 * Inputs           : cceIndexArray_p - This is pointer to UInt8 array which holds
 *                                      the CCE Number
 *                                      Index[0] -->Holds CCE number of 1st CCE  at the Aggr Level one
 *                                      Index[1] -->Holds CCE number of 2nd CCE  at the Aggr Level one
 *                                      Index[2] -->Holds CCE number of 3rd CCE  at the Aggr Level one
 *                                      Index[3] -->Holds CCE number of 4th CCE  at the Aggr Level one
 *                                      Index[4] -->Holds CCE number of 5th CCE  at the Aggr Level one
 *                                      Index[5] -->Holds CCE number of 6th CCE  at the Aggr Level one
 *                    dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                                   Information is there and once cce is allocated, updated 
 *                                   information is written on the reserved CCE,
 *                    dciCCEContainerInfo_p -
 * Outputs          : cceAllocated_p - This holds the value of CCE Index which is
 *                                     allocated,
 *                    availableCCE_p - This represent the number of available CCE
 *                                     and if CCE's are allocated this would be deceremented by one 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtions search for two consecutives Free CCEs in UE Search Space and if
 *                    CCE are found to be free ,it will mark those CCE as busy 
 *                    and update the  dciCCEInfo_p ,cceAllocated_p and availableCCE_p variable.
 ******************************************************************************************************/

 MacRetType isCCEAvailableAggrLvlOneUESpaceforCAUE(
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo *dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p)
{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;
    UInt8 firstCCEIndex = 0;
    UInt8 secCCEIndex = 0;
    UInt8 numPdcchCandidates = 0;

    dciCCEInfoTemp_p = &dciCCEInfo_p[0];

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);


    while (numPdcchCandidates < 
		    /* SPR 22423 Fix Start */ 
		    NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_ONE_UE_SPACE)
		    /* SPR 22423 Fix End */ 
    {
        firstCCEIndex = *cceIndexArray_p;
        secCCEIndex  = *(cceIndexArray_p + 1);
        if (dciCCEInfoTemp_p[firstCCEIndex].isCCEFree &&
                    dciCCEInfoTemp_p[secCCEIndex].isCCEFree) 
        {

            dciCCEInfoTemp_p[firstCCEIndex].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[secCCEIndex].isCCEFree = CCE_ALLOCATED;

            dciCCEInfoTemp_p[firstCCEIndex].dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_ONE;
            dciCCEInfoTemp_p[secCCEIndex].dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_ONE;

            dciCCEInfoTemp_p[firstCCEIndex].dciPduInfo.cceIndex = firstCCEIndex;
            dciCCEInfoTemp_p[secCCEIndex].dciPduInfo.cceIndex = secCCEIndex;

            *availableCCE_p = *availableCCE_p - AGGREGATION_LEVEL_TWO;
            *cceAllocated_p = firstCCEIndex;

            dciCCEContainerInfo_p->arrayExtraCCEAllocated
                [dciCCEContainerInfo_p->countOfExtraCCEAllocated] = secCCEIndex;
            dciCCEContainerInfo_p->countOfExtraCCEAllocated++;


            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);

            return MAC_SUCCESS;
        }

        cceIndexArray_p += AGGREGATION_LEVEL_ONE;

        /*
         * For Aggr level 1 each CCE Index is one
         * Pdcch Candidate.
         * Hence Candiates were incrementing by 1
         */

        numPdcchCandidates += AGGREGATION_LEVEL_ONE;
    }

    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
            getCurrentTick(),\
            0,\
            dciCCEInfoTemp_p->msgType,\
            dciCCEInfoTemp_p->cceIndex,\
            0,\
            0,\
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            __func__, "CCE_ALLOC_FAIL");

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"CCE not Allocated Leaving \
            wit Failure\n");
    return MAC_FAILURE;
}
/* CA_phase2_harq_code End */

/********************************************************************************************************
 * Function Name    : isCCEAvailableAggrLvlOneUESpace
 * Inputs           : cceIndexArray_p - This is pointer to UInt8 array which holds
 *                                      the CCE Number
 *                                      Index[0] -->Holds CCE number of 1st CCE  at the Aggr Level one
 *                                      Index[1] -->Holds CCE number of 2nd CCE  at the Aggr Level one
 *                                      Index[2] -->Holds CCE number of 3rd CCE  at the Aggr Level one
 *                                      Index[3] -->Holds CCE number of 4th CCE  at the Aggr Level one
 *                                      Index[4] -->Holds CCE number of 5th CCE  at the Aggr Level one
 *                                      Index[5] -->Holds CCE number of 6th CCE  at the Aggr Level one
 *                    dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                                   Information is there and once cce is allocated, updated 
 *                                   information is written on the reserved CCE,
 * Outputs          : cceAllocated_p - This holds the value of CCE Index which is
 *                                     allocated,
 *                    availableCCE_p - This represent the number of available CCE
 *                                     and if CCE's are allocated this would be deceremented by one 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtions search the Free CCE in UE Search Space and if
 *                    CCE are found to be free ,it will mark those CCE as busy 
 *                    and update the  dciCCEInfo_p ,cceAllocated_p and availableCCE_p variable.
 ******************************************************************************************************/

/* SPR 5599 changes start (ZIP ID 129195) */            
 MacRetType isCCEAvailableAggrLvlOneUESpace(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo *dciCCEInfo_p)
{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);

    /*we will use cceIndexArray_p array to get the CCE Index which need to
          be checked if it is free or not .If CCE is found to be free then it is 
          marking that CCE Busy ,decerementing the available CCE and  updating
          the cceAllocated_p with the index of CCE which is marked as  busy */

    dciCCEInfoTemp_p = &dciCCEInfo_p[0];
    
        if (dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree ||
                dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree ||
                dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree ||
                dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree ||
                dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree ||
                dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree )
        {
            dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_ONE;
            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.cceIndex = *cceIndexArray_p;

            *availableCCE_p = *availableCCE_p -1; 
            *cceAllocated_p = *cceIndexArray_p;
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);
            return MAC_SUCCESS;
        }
    

    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    /* SPE 16766 fix start */
                    dciCCEInfo_p->dciPduInfo.ueIndex, \
                    /* SPE 16766 fix end */
                    dciCCEInfoTemp_p->msgType,\
                    dciCCEInfoTemp_p->cceIndex,\
                    0,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");
    
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"CCE not Allocated Leaving \
        wit Failure\n");
    return MAC_FAILURE;
}


/*********************************************************************************************************
 * Function Name    : isCCEAvailableAggrLvlTwoUESpace
 * Inputs           : cceIndexArray_p - This is pointer to UInt8 array which holds
 *                                      the CCE Number
 *                                      Index[0] -->Holds CCE number of 1st CCE  at the Aggr Level one
 *                                      Index[1] -->Holds CCE number of 2nd CCE  at the Aggr Level one
 *                                      Index[2] -->Holds CCE number of 3rd CCE  at the Aggr Level one
 *                                      Index[3] -->Holds CCE number of 4th CCE  at the Aggr Level one
 *                                      Index[4] -->Holds CCE number of 5th CCE  at the Aggr Level one
 *                                      Index[5] -->Holds CCE number of 6th CCE  at the Aggr Level one
 *                                      Index[6] -->Holds CCE number of 7th CCE  at the Aggr Level one
 *                                      Index[7] -->Holds CCE number of 8th CCE  at the Aggr Level one
 *                                      Index[8] -->Holds CCE number of 9th CCE  at the Aggr Level one
 *                                      Index[9] -->Holds CCE number of 4th CCE  at the Aggr Level one
 *                                      Index[10-->Holds CCE number of 5th CCE  at the Aggr Level one
 *                                      Index[11]->Holds CCE number of 6th CCE  at the Aggr Level one
 *                    dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                                    Information is there and once cce is allocated, updated 
 *                                    information is written on the reserved CCE
 * Outputs            cceAllocated_p - This holds the value of CCE Index which is allocated,
 *                    availableCCE_p - This represent the number of available CCE
 *                                      and if CCE's are allocated this would be deceremented by one
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This funtions search the Free CCE in UE Search Space and if
 *                   CCE are found to be free ,it will mark those CCE as busy 
 *                   and update the  dciCCEInfo_p ,cceAllocated_p and availableCCE_p variable.
 *********************************************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType isCCEAvailableAggrLvlTwoUESpace(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p)
{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;    
    UInt32 numPdcchCandidates = 0;
    UInt8 *tempCCEIndex_p =  PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);

    dciCCEInfoTemp_p = &dciCCEInfo_p[0];
    tempCCEIndex_p = cceIndexArray_p;

    while (numPdcchCandidates < NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_TWO_UE_SPACE)
    {
        if (dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree) 
        {
            dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;

            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_TWO;
            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.cceIndex = 
                *cceIndexArray_p;

            *availableCCE_p = *availableCCE_p - AGGREGATION_LEVEL_TWO; 
            *cceAllocated_p = *cceIndexArray_p;      

            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);

            return MAC_SUCCESS;
        }

        cceIndexArray_p = tempCCEIndex_p;
        cceIndexArray_p += AGGREGATION_LEVEL_TWO;
        tempCCEIndex_p = cceIndexArray_p;
        numPdcchCandidates++;
    }

    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    /* SPE 16766 fix start */
                    dciCCEInfo_p->dciPduInfo.ueIndex, \
                    /* SPE 16766 fix end */
                    dciCCEInfoTemp_p->msgType,\
                    dciCCEInfoTemp_p->cceIndex,\
                    0,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "CCE Allocation is Failed Leaving function = %s with Failure\n", __func__);
    return MAC_FAILURE;
}

/********************************************************************************************
 * Function Name  : isCCEAvailableAggrLvlFourUESpace
 * Inputs         : cceIndexArray_p - This is pointer to UInt8 array which holds
 *                                     the CCE Number.
 *                           Index[0] -->Holds CCE number of 1st CCE  at the Aggr Level one
 *                           Index[1] -->Holds CCE number of 2nd CCE  at the Aggr Level one
 *                           Index[2] -->Holds CCE number of 3rd CCE  at the Aggr Level one
 *                           Index[3] -->Holds CCE number of 4th CCE  at the Aggr Level one
 *                           Index[4] -->Holds CCE number of 5th CCE  at the Aggr Level one
 *                           Index[5] -->Holds CCE number of 6th CCE  at the Aggr Level one
 *                           Index[6] -->Holds CCE number of 7th CCE  at the Aggr Level one
 *                           Index[7] -->Holds CCE number of 8th CCE  at the Aggr Level one
 *                    dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                                    Information is there and once cce is allocated, updated 
 *                                    information is written on the reserved CCE
 * Outputs        : cceAllocated_p - This holds the value of CCE Index which is
 *                                   allocated.
 *                  availableCCE_p - This represent the number of available CCE
 *                  and if CCE's are allocated this would be deceremented by one .
 * Returns        : MacRetType
 * Description    : This funtions search the Free CCE in UE Search Space and if
 *                  CCE are found to be free ,it will mark those CCE as busy 
 *                  and update the  dciCCEInfo_p ,cceAllocated_p and 
 *                  availableCCE_p variable.
 **********************************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType isCCEAvailableAggrLvlFourUESpace(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p)   
{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;
    UInt32 numPdcchCandidates = 0;
    UInt8 *tempCCEIndex_p =  PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);

    dciCCEInfoTemp_p = &dciCCEInfo_p[0];
    tempCCEIndex_p = cceIndexArray_p;

    while (numPdcchCandidates < NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_FOUR_UE_SPACE)
    {
        if (dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree) 
        {
            dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;

            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_FOUR;
            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.cceIndex = 
                *cceIndexArray_p;

            *availableCCE_p = *availableCCE_p - AGGREGATION_LEVEL_FOUR; 
            *cceAllocated_p = *cceIndexArray_p;

            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);
            return MAC_SUCCESS;
        }
        
        cceIndexArray_p = tempCCEIndex_p;
        cceIndexArray_p += AGGREGATION_LEVEL_FOUR;
        tempCCEIndex_p = cceIndexArray_p;
        numPdcchCandidates++;
    }

    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    /* SPE 16766 fix start */
                    dciCCEInfo_p->dciPduInfo.ueIndex, \
                    /* SPE 16766 fix end */
                    dciCCEInfoTemp_p->msgType,\
                    dciCCEInfoTemp_p->cceIndex,\
                    0,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"CCE not Allocated Leaving \
        wit Failure\n");
    return MAC_FAILURE;
}

/**********************************************************************************
 * Function Name  : isCCEAvailableAggrLvlEightUESpace
 * Inputs         : cceIndexArray_p - This is pointer to UInt8 array which holds
 *                                    the CCE Number.
 *                   Index[0] -->Holds CCE number of 1st CCE  at the Aggr Level one.
 *                   Index[1] -->Holds CCE number of 2nd CCE  at the Aggr Level one .
 *                   Index[2] -->Holds CCE number of 3rd CCE  at the Aggr Level one .
 *                   Index[3] -->Holds CCE number of 4th CCE  at the Aggr Level one .
 *                   Index[4] -->Holds CCE number of 5th CCE  at the Aggr Level one .
 *                   Index[5] -->Holds CCE number of 6th CCE  at the Aggr Level one .
 *                   Index[6] -->Holds CCE number of 7th CCE  at the Aggr Level one.
 *                   Index[7] -->Holds CCE number of 8th CCE  at the Aggr Level one .
 *                   Index[8] -->Holds CCE number of 9th CCE  at the Aggr Level one .
 *                   Index[9] -->Holds CCE number of 10th CCE  at the Aggr Level one .
 *                   Index[10]-->Holds CCE number of 11th CCE  at the Aggr Level one .
 *                   Index[11]->Holds CCE number of 12th CCE  at the Aggr Level one .
 *                   Index[12] -->Holds CCE number of 13th CCE  at the Aggr Level one.
 *                   Index[13] -->Holds CCE number of 14th CCE  at the Aggr Level one .
 *                   Index[14] -->Holds CCE number of 15th CCE  at the Aggr Level one .
 *                   Index[15] -->Holds CCE number of 16th CCE  at the Aggr Level one . 
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                               Information is there and once cce is allocated ,updated 
 *                               information is written on the reserved CCE.
 * Outputs        : cceAllocated_p - This holds the value of CCE Index which is
 *                                    allocated.
 *                  availableCCE_p - This represent the number of available CCE
 *                      and if CCE's are allocated this would be deceremented by one .
 * Returns        : MacRetType
 * Description    : This funtions search the Free CCE in UE Search Space and if
 *                  CCE are found to be free ,it will mark those CCE as busy 
 *                  and update the  dciCCEInfo_p ,cceAllocated_p and 
 *                  availableCCE_p variable..
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType isCCEAvailableAggrLvlEightUESpace(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 *cceIndexArray_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p)

{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;
    UInt32 numPdcchCandidates = 0;
    UInt8 *tempCCEIndex_p =  PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);

    dciCCEInfoTemp_p = &dciCCEInfo_p[0];
    tempCCEIndex_p = cceIndexArray_p;

    while (numPdcchCandidates < NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_EIGHT_UE_SPACE)
    {
        if (dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree &&
                    dciCCEInfoTemp_p[*++cceIndexArray_p].isCCEFree) 
        {
            dciCCEInfoTemp_p[*cceIndexArray_p].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            dciCCEInfoTemp_p[*(--cceIndexArray_p)].isCCEFree = CCE_ALLOCATED;
            
            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_EIGHT;
            dciCCEInfoTemp_p[*cceIndexArray_p].dciPduInfo.cceIndex = 
                *cceIndexArray_p;

            *availableCCE_p = *availableCCE_p - AGGREGATION_LEVEL_EIGHT; 
            *cceAllocated_p = *cceIndexArray_p;   
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);
            return MAC_SUCCESS;
        }

        cceIndexArray_p = tempCCEIndex_p;
        cceIndexArray_p += AGGREGATION_LEVEL_EIGHT;
        tempCCEIndex_p = cceIndexArray_p;
        numPdcchCandidates++;
    }
    
    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    /* SPE 16766 fix start */
                    dciCCEInfo_p->dciPduInfo.ueIndex, \
                    /* SPE 16766 fix end */
                    dciCCEInfoTemp_p->msgType,\
                    dciCCEInfoTemp_p->cceIndex,\
                    0,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"CCE not Allocated Leaving \
        with Failure\n");
    return MAC_FAILURE;
}

/******************************************************************************
 * Function Name   : isCCEAvailableAggrLvlFourCommonSpace
 * Inputs          : internalCellIndex - Cell-Index at MAC
 * Outputs         : cceAllocated_p - This holds the value of CCE Index which is
 *                     allocated.
 *                   availableCCE_p : This represent the number of available CCE
 *                     and if CCE's are allocated this would be deceremented by
 *                     one .
 *                   dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                     Information is there and once cce is allocated ,updated 
 *                     information is written on the reserved CCE.
 * Returns         : MacRetType
 * Description     : This funtions search the Free CCE in UE Search Space and if
 *                   CCE are found to be free ,it will mark those CCE as busy 
 *                   and update the  dciCCEInfo_p ,cceAllocated_p and 
 *                   availableCCE_p variable.
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType isCCEAvailableAggrLvlFourCommonSpace(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        InternalCellIndex internalCellIndex)   
{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;
    UInt32 numOfPDCCHCandidate = 0;
    UInt32 countPDCCHCandidate = 0;
    /*CID 111495 fix (variable test_temp removed)*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);
    /*Get the number of PDCCH Candidate that can fit in the available number 
     *of CCEs*/

/* SPR 5333 changes start */
#ifdef FDD_CONFIG
    /*START : DYN_PDCCH*/
    numOfPDCCHCandidate = maxCCEInCommonSpaceForPDCCH_g[internalCellIndex][valueCFI_g[internalCellIndex] -1] / AGGREGATION_LEVEL_FOUR;
    /*END   : DYN_PDCCH*/
#else
    /*START : DYN_PDCCH*/
    /*START:SPR:6080*/
/* SPR 19505 fix Start */
    if(((currentsubframe_g == 1) || (currentsubframe_g == 6)) && (3 == valueCFI_g[internalCellIndex]))
    /*END:SPR:6080*/ 
    {
/* SPR 19505 fix End */
    /*START:SPR:6080*/
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        numOfPDCCHCandidate = maxCCEInCommonSpace_g[internalCellIndex][CFI_VALUE_SPECIAL_SUBFRAME - 1/*1*/][miValueForSubframe_g[internalCellIndex]] 
            / AGGREGATION_LEVEL_FOUR;
        /*CA TDD CHANGES END*/
        /** CA-TDD Changes Stop **/
    /*END:SPR:6080*/ 
    }
    else
    {
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        numOfPDCCHCandidate = maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1][miValueForSubframe_g[internalCellIndex]] / AGGREGATION_LEVEL_FOUR;
        /*CA TDD CHANGES START*/
        /** CA-TDD Changes Stop **/
    }
    /*END   : DYN_PDCCH*/
#endif
/* SPR 5333 changes end */
    countPDCCHCandidate = 0;
    /*Below while loop is iterating for numOfPDCCHCandidate times and in each
     * iteration it checks if the CCEs are free .If Free it is updating 
     *availableCCE_p and cceAllocated_p with relevant information*/
    while ( numOfPDCCHCandidate-- )
    {
        dciCCEInfoTemp_p = &dciCCEInfo_p[countPDCCHCandidate];
        /*Below if check is checking for FREE CCE and if a CCE is found to be 
          free it is markign that CCE Busy ,decerementing the available CCE and 
          updating the cceAllocated_p with the index of CCE which is marked as 
          busy */
        /* + SPR 8190 */
        if ((*availableCCE_p >= AGGREGATION_LEVEL_FOUR)&&
                    ((dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree && 
                    (++dciCCEInfoTemp_p)->isCCEFree) )

        /* - SPR 8190 */
        {
            dciCCEInfoTemp_p->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            *availableCCE_p = *availableCCE_p - 4; 
            *cceAllocated_p = dciCCEInfoTemp_p->cceIndex;
            dciCCEInfoTemp_p->dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_FOUR;
            dciCCEInfoTemp_p->dciPduInfo.cceIndex = *cceAllocated_p;
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);
            return MAC_SUCCESS;
        }  
        countPDCCHCandidate += AGGREGATION_LEVEL_FOUR;
    }

    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    0,\
                    /*BUG:8926:START*/
                    /*dciCCEInfoTemp_p->msgType*/0,\
                    /*dciCCEInfoTemp_p->cceIndex*/0,\
                    /*BUG:8926:END*/
                    0,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"CCE not Allocated Leaving \
        wit Failure\n");
    return MAC_FAILURE;
}

/******************************************************************************
 * Function Name   : isCCEAvailableAggrLvlEightCommonSpace 
 * Inputs          : internalCellIndex - Cell-Index at MAC
 * Outputs         : cceAllocated_p - This holds the value of CCE Index which is
 *                     allocated.
 *                   availableCCE_p : This represent the number of available CCE
 *                     and if CCE's are allocated this would be deceremented by
 *                     one .
 *                   dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                     Information is there and once cce is allocated ,updated 
 *                     information is written on the reserved CCE.
 * Returns         : MacRetType
 * Description     : This funtions search the Free CCE in UE Search Space and if
 *                   CCE are found to be free ,it will mark those CCE as busy 
 *                   and update the  dciCCEInfo_p ,cceAllocated_p and 
 *                   availableCCE_p variable.
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType isCCEAvailableAggrLvlEightCommonSpace(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        InternalCellIndex internalCellIndex)

{
    DciCCEInfo *dciCCEInfoTemp_p = PNULL;
    UInt32 numOfPDCCHCandidate = 0;
    UInt32 countPDCCHCandidate = 0;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);
    /*Get the number of PDCCH Candidate that can fit in the available number 
     *of CCEs*/
/* SPR 5333 changes start */
#ifdef FDD_CONFIG
    /*START : DYN_PDCCH*/
    numOfPDCCHCandidate = maxCCEInCommonSpaceForPDCCH_g[internalCellIndex][valueCFI_g[internalCellIndex] -1] / AGGREGATION_LEVEL_EIGHT;
    /*END   : DYN_PDCCH*/
#else
    /*START : DYN_PDCCH*/
    /*START:SPR:6080*/
/* SPR 19505 fix Start */
    if(((currentsubframe_g == 1) || (currentsubframe_g == 6)) && (3 == valueCFI_g[internalCellIndex]))
    /*END:SPR:6080*/ 
    {
/* SPR 19505 fix End */
    /*START:SPR:6080*/
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        numOfPDCCHCandidate = maxCCEInCommonSpace_g[internalCellIndex][CFI_VALUE_SPECIAL_SUBFRAME - 1][miValueForSubframe_g[internalCellIndex]] / AGGREGATION_LEVEL_EIGHT;
      /*CA TDD CHANGES START*/
        /** CA-TDD Changes Stop **/
    /*END:SPR:6080*/ 
    }
    else
    {
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        numOfPDCCHCandidate = maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] / AGGREGATION_LEVEL_EIGHT;
        /*CA TDD CHANGES END*/
        /** CA-TDD Changes Stop **/
    }
    /*END   : DYN_PDCCH*/
#endif
/* SPR 5333 changes end */
    countPDCCHCandidate = 0;
    /*Below while loop is iterating for numOfPDCCHCandidate times and in each
     * iteration it checks if the CCEs are free .If Free it is updating 
     *availableCCE_p and cceAllocated_p with relevant information*/
    while ( numOfPDCCHCandidate-- )
    {
        dciCCEInfoTemp_p = &dciCCEInfo_p[countPDCCHCandidate];
        /*Below if check is checking for FREE CCE and if a CCE is found to be 
          free it is markign that CCE Busy ,decerementing the available CCE and 
          updating the cceAllocated_p with the index of CCE which is marked as 
          busy */
        /* + SPR 8190 */
        if ((*availableCCE_p >= AGGREGATION_LEVEL_EIGHT)&&
                    ((dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree && 
                    (++dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree &&
                    (++dciCCEInfoTemp_p)->isCCEFree && 
                    (++dciCCEInfoTemp_p)->isCCEFree) )
       /* - SPR 8190 */
        {
            dciCCEInfoTemp_p->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            (--dciCCEInfoTemp_p)->isCCEFree = CCE_ALLOCATED;
            *availableCCE_p = *availableCCE_p - 8; 
            *cceAllocated_p = dciCCEInfoTemp_p->cceIndex;
            dciCCEInfoTemp_p->dciPduInfo.aggregationLvl = 
                AGGREGATION_LEVEL_EIGHT;
            dciCCEInfoTemp_p->dciPduInfo.cceIndex = *cceAllocated_p;
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "CCE Allocated = %u Leaving function = %s with Success",
                    *cceAllocated_p,__func__);
            return MAC_SUCCESS;
        }
        countPDCCHCandidate += AGGREGATION_LEVEL_EIGHT;
    }

    LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    0,\
                    /*BUG:8926:START*/
                    /*dciCCEInfoTemp_p->msgType*/0,\
                    /*dciCCEInfoTemp_p->cceIndex*/0,\
                    /*BUG:8926:END*/
                    0,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"CCE not Allocated Leaving \
        wit Failure\n");
    return MAC_FAILURE;  
}

/* SPR 6856 Changes start */
/*********************************************************************************
 * Function Name : allocatePDCCHForDCIFormatCommonSpace
 * Inputs        : aggregationLevel -Either 4 or 8
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : cceAllocated_p -returns the allocated CCE index
 *                 availableCCE_p - This represent the number of available CCE
 *                      and if CCE's are allocated this would be deceremented by  one.
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE
 *                      Information is there and once cce is allocated ,updated
 *                      information is written on the reserved CCE.
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:    This function will alloacte the CCE in common space
 ************************************************************************************/
MacRetType allocatePDCCHForDCIFormatCommonSpace(
            UInt32 aggregationLevel,
            UInt32 *cceAllocated_p,
            UInt32 *availableCCE_p,
            DciCCEInfo * dciCCEInfo_p,
            InternalCellIndex internalCellIndex)
{
    MacRetType macRetType = MAC_FAILURE; /* Return if default switch */
    switch(aggregationLevel)
    {
        /*Rel_523_Coverity_10047 Fix start*/
#if (AGGREGATION_LEVEL_FOUR == AGGREGATION_LEVEL_FOR_COMMON_SPACE)
        case AGGREGATION_LEVEL_FOUR:
        {
            LTE_MAC_UT_LOG(LOG_INFO ,CCCH_MGR,
                    "[%s]: Allocating CCE at Aggr Level = %d ",
                    __func__,aggregationLevel);
            macRetType = isCCEAvailableAggrLvlFourCommonSpace(
                    cceAllocated_p,availableCCE_p,dciCCEInfo_p,
                    internalCellIndex);
        }
        break;
#elif ( AGGREGATION_LEVEL_EIGHT == AGGREGATION_LEVEL_FOR_COMMON_SPACE)
        case AGGREGATION_LEVEL_EIGHT:
        {
            LTE_MAC_UT_LOG(LOG_INFO,CCCH_MGR,
                    "[%s]: Allocating CCE at Aggr Level = %d",
                    __func__,aggregationLevel);
            macRetType = isCCEAvailableAggrLvlEightCommonSpace(
                    cceAllocated_p,availableCCE_p,dciCCEInfo_p,
                    internalCellIndex);
        }
        break;
#endif
        /*Rel_523_Coverity_10047 Fix end*/
    }/*End of Switch Statement */
    
    /* + SPR 8190 */
    if ((*cceAllocated_p > highestCCENumberCommonSpace_g[internalCellIndex])
            && (MAC_SUCCESS == macRetType))
    {
        highestCCENumberCommonSpace_g[internalCellIndex] = *cceAllocated_p;
    }
    /* - SPR 8190 */
    return macRetType;
}
/* SPR 6856 Changes end */

/******************************************************************************
 * Function Name : allocatePDCCHForDCIFormatUESpace
 * Inputs        : ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo 
 *                     structure which holds the CCEs Index for a UE at each of the 
 *                     aggregation level
 *                  dciFormat - This holds the DCI Format for which PDCCH region has to
 *                     be allocated.             
 *                 dciCCEContainerInfo_p -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : ceAllocated_p - This holds the value of CCE Index which is
 *                    allocated.
 *                 availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by  one.
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 * Returns       : MacRetType
 * Description   : This funtions will find the aggregation level for the DCI 
 *                 Format and based on aggregation level it call the relevant
 *                 funtions for finding the free CCE 
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType allocatePDCCHForDCIFormatUESpace(
        /* SPR 5599 changes end (ZIP ID 129195) */            
        DLUEContext *dlUEContext_p,
        UESpecificSearchSpaceInfo*ueSearchSpaceInfo_p,
        UInt32 *cceAllocated_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo * dciCCEInfo_p,
        UInt8 aggregationLevel,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex)
{
    MacRetType macRetType = MAC_FAILURE;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n", __func__);
/* SPR 5333 changes start */
/*  SPR_9402_FIX_START */ 
#ifdef FDD_CONFIG
    /* + SPR 8190 */
    if ( *availableCCE_p < aggregationLevel )
    /* - SPR 8190 */
    {
        LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    0,\
                    0,\
                    0,\
                    aggregationLevel,\
                    *availableCCE_p,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");

        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                " *availableCCE_p < aggregationLevel .Returning \
                from function allocatePDCCHForDCIFormatUESpace \
                with Failure\n");
        return MAC_FAILURE;
    }
#else
    /* + SPR 8190 */
    if ( *availableCCE_p < aggregationLevel )
    /* - SPR 8190 */
    {
        /** CA-TDD Changes Start **/
        LOG_MAC_MSG (MAC_CCE_ALLOC_FAIL_ID, LOGWARNING, MAC_PDCCH,\
                    getCurrentTick(),\
                    0,\
                    0,\
                    0,\
                    aggregationLevel,\
                    *availableCCE_p,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "CCE_ALLOC_FAIL");
        /** CA-TDD Changes Stop **/
        return MAC_FAILURE;
    }
/*  SPR_9402_FIX_END */ 
#endif
/* SPR 5333 changes end */
    switch(aggregationLevel)
    {
        case AGGREGATION_LEVEL_ONE:
            {
                /*Comes here when the Aggregagation Level of dciFormat is One*/
                LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                        "Trying to Allocate CCE at Aggr Level = %d",
                        aggregationLevel); 
                  /*pdcch for scell changes start*/
/*SPR 21129 changes start*/                  
                /* SPR 22423 Fix Start */ 
                /* In CA case, when 2 TB case configured, then the next PUCCH resource should also be free for use. 
                 * PUCCH resource for format-1b with channel selection use second resource as (N1PUCCH + nCCE) and (N1PUCCH + nCCE + 1) */
				if((dlUEContext_p) && (dciCCEContainerInfo_p) && (IS_CA_ENABLED()) &&
						(ALLOCATE_EXTRA_CCE( dlUEContext_p,internalCellIndex )))
				{
					macRetType = isCCEAvailableAggrLvlOneUESpaceforCAUE(ueSearchSpaceInfo_p->\
							cceAggrLvlOneInfo[currentCFIValue_g[internalCellIndex]-1],/*DYN_PDCCH*/
							cceAllocated_p,availableCCE_p,dciCCEInfo_p,dciCCEContainerInfo_p);
                /* SPR 22423 Fix End */ 
				}
				else
				{
/*SPR 21129 changes end*/                  
                /*Calling funtion isCCEAvailableAggrLvlOneUESpace which will
                  check if the CCEs are free at aggregation level one.If free
                  it will return Success else Failure*/
                macRetType =  isCCEAvailableAggrLvlOneUESpace(
                        ueSearchSpaceInfo_p->cceAggrLvlOneInfo[currentCFIValue_g[internalCellIndex]-1],/*DYN_PDCCH*/
                        cceAllocated_p,availableCCE_p,dciCCEInfo_p);
                  /*pdcch for scell changes end*/
/*SPR 21129 changes start*/                  
                }
/*SPR 21129 changes end*/                  
            }
            break;
        case AGGREGATION_LEVEL_TWO:
            {
                /*Comes here when the Aggregagation Level of dciFormat is Two*/
                LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                        "Trying to Allocate CCE at Aggr Level = %d",
                        aggregationLevel);   
                /*Calling funtion isCCEAvailableAggrLvlTwoUESpace which will
                  check if the CCEs are free at aggregation level Two.If free
                  it will return Success else Failure*/
                macRetType = isCCEAvailableAggrLvlTwoUESpace(
                        ueSearchSpaceInfo_p->cceAggrLvlTwoInfo[currentCFIValue_g[internalCellIndex]-1],/*DYN_PDCCH*/
                        cceAllocated_p,availableCCE_p,dciCCEInfo_p);
            }
            break;
        case AGGREGATION_LEVEL_FOUR:
            {
                /*Comes here when the Aggregagation Level of dciFormat is Four*/
                LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                        "Trying to Allocate CCE at Aggr Level = %d",
                        aggregationLevel);   
                /*Calling funtion isCCEAvailableAggrLvlFourUESpace which will
                  check if the CCEs are free at aggregation level Four.If free
                  it will return Success else Failure*/
                macRetType = isCCEAvailableAggrLvlFourUESpace(
                        ueSearchSpaceInfo_p->cceAggrLvlFourInfo[currentCFIValue_g[internalCellIndex]-1],/*DYN_PDCCH*/
                        cceAllocated_p,availableCCE_p,dciCCEInfo_p);
            }
            break;
        case AGGREGATION_LEVEL_EIGHT:
            {   
                /*Comes here when the Aggregagation Level of dciFormat is Eight*/
                LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                        "Trying to Allocate CCE at Aggr Level = %d",
                        aggregationLevel);   
                /*Calling funtion isCCEAvailableAggrLvlEightUESpace which will
                  check if the CCEs are free at aggregation level Eight.If free
                  it will return Success else Failure*/
                macRetType = isCCEAvailableAggrLvlEightUESpace(
                        ueSearchSpaceInfo_p->cceAggrLvlEightInfo[currentCFIValue_g[internalCellIndex]-1],/*DYN_PDCCH*/
                        cceAllocated_p,availableCCE_p,dciCCEInfo_p);
            }
            break;
        default:
            {
                LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                        "InValid Aggrgation Level\n");        
            }
    }/*End of Switch Statement */
/*START : DYN_PDCCH*/
    CALCULATE_AND_UPDATE_CCE_CORRECTION_FACTOR(aggregationLevel, macRetType, internalCellIndex);
/*END   : DYN_PDCCH*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n",__func__); 
    /*DYN_PDCCH_TDD_FIX*/
    if (macRetType == MAC_SUCCESS)
    {
        changeCFIInThisTTI_g[internalCellIndex] = TRUE;
    }
    /*DYN_PDCCH_TDD_FIX*/
    return macRetType;
}

/*************************************************************************************
 * Function Name : allocatePDCCHForDLNewReAdvanceTx
 * Inputs        : ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo 
 *                     structure which holds the CCEs Index for a UE at each of the 
 *                     aggregation level.
 *                 isRBAllocInAdvance - This holds the infomation that is If the 
 *                     RBs has already been allocated in advance earlier to this UE.
 *                 dlStrategyNode_p -  This holds the pointer to structure which 
 *                     store the information of those UE for either Re or New 
 *                     Transmission is needed.
 *                 dlUEContext_p - This holds pointer to DLUEContext structure.
 *                 dciCCEContainerInfo_p -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : dciCCEInfo_p -  This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 *                 availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by
 *                    one .
 *                 allocatedCCEIndex_p - This holds the value of Allocated CCE Index
 *                 dlMsgTypeMsgInfo_p - This holds the pointer to structure on which  
 *                     once CCEs are allocate,relevant information would be written.
 * Returns       : MacRetType
 * Description   : This funtions will call the funtion allocatePDCCHForDCIFormat 
 *                 that will check for CCEs Availability and If Funtion return 
 *                 Success ,it will update the  dlMsgTypeMsgInfo_p and 
 *                 pdcchUEInfo_p structure.
 *************************************************************************************/
STATIC MacRetType allocatePDCCHForDLNewReAdvanceTx(
        DLStrategyTxNode *dlStrategyNode_p,
        UInt32 *allocatedCCEIndex_p,
        DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
        UInt32 *availableCCE_p,   
        DciCCEInfo *dciCCEInfo_p,
        DLUEContext *dlUEContext_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 powerOffset = 0;
    DCIFormat  dciFormat = MAX_NUM_DCI_FORMAT;
    DLDataMsgInfo *dlDataMsgInfo_p = PNULL;
    /* + TM7_8 Changes Start */
    DLHARQProcess  *dlHarqProcess_p = PNULL;
    /* + SPR 11469 Changes */
    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        dlHarqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess
                [dlStrategyNode_p->harqProcessId];
    }
    else
    {
        dlHarqProcess_p = &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->\
                    ScelldlHarqContext_p->harqProcess[dlStrategyNode_p->harqProcessId];
    }
    /* - SPR 11469 Changes */
    /* - TM7_8 Changes End */
    
    /* SPS_CHG_START */
    /*! \fn static MacRetType allocatePDCCHForDLNewReAdvanceTx(
     *         DLStrategyTxNode *dlStrategyNode_p,
     *         UInt32 *allocatedCCEIndex_p,
     *         DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p,
     *         UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
     *         UInt32 *availableCCE_p,
     *         DciCCEInfo *dciCCEInfo_p,
     *         DLUEContext *dlUEContext_p)
     * \brief Function Modification
     * \code
     * - Check If Message type is SPS_OCCASION
     *       - Allocate the Cce Index above the MAX_CCE
     *       - Increment the spsCceIndex_g
     *       - Populating the dlMsgTypeMsgInfo_p structure which 
     *         will be used by PDSCH for allocating the RBs
     * \endcode
     * */
    DLCQIInfo *dlCQIInfo_p = PNULL;
    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if(dlCQIInfo_p == PNULL)
    {
        /* + SPR 13605 Fix */
        if(!IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex) && 
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
        {
           dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus = 0x00; 
               /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.
                sCellSchInfo[dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame] = 0x00; 
#endif
               /** SPR 14277 Changes End **/
        }
        /* - SPR 13605 Fix */
        return MAC_FAILURE;
    }

    if (dlStrategyNode_p->msgType == SPS_OCCASION)
    {

        *allocatedCCEIndex_p = MAX_CCE + spsCceIndex_g[internalCellIndex];
        spsCceIndex_g[internalCellIndex]++;
        if (spsCceIndex_g[internalCellIndex] == MAX_SPS_CCE)
        {
            spsCceIndex_g[internalCellIndex] = 0;
        }

        /*Populating the dlMsgTypeMsgInfo_p structure which will be used by PDSCH
          for allocating the RBs */
        dlDataMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;  
        dlMsgTypeMsgInfo_p->msgType = dlStrategyNode_p->msgType;
        dlDataMsgInfo_p->dlStrategyTxNode_p = dlStrategyNode_p;      
        dlDataMsgInfo_p->cceIndex = *allocatedCCEIndex_p; 
        dlDataMsgInfo_p->dciFormat = DCI_FORMAT_1A; 
        dciCCEInfo_p[*allocatedCCEIndex_p].cceIndex = dlDataMsgInfo_p->cceIndex;
        dciCCEInfo_p[*allocatedCCEIndex_p].isCCEFree = CCE_ALLOCATED;
       
        return MAC_SUCCESS;
    }
    /* SPS_CHG_END */
    if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable && 
            dlCQIInfo_p->isWideBandReportAvailable)
    {
        /*Cyclomatic Complexity changes - starts here */
        if (MAC_FAILURE == allocPDCCHPdcchPowerOrAggregationEnabledWithWideBandReports(dlUEContext_p,
                                                                    &dciFormat,
                                                                    &powerOffset,
                                                                    dlStrategyNode_p,
                                                                    ueSearchSpaceInfo_p,
                                                                    availableCCE_p,   
                                                                    allocatedCCEIndex_p,
                                                                    dciCCEInfo_p,
                                                                    dciCCEContainerInfo_p,
                                                                    internalCellIndex)) 
        {
            /* + SPR 13605 Fix */
            if(!IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex) && 
                    dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
            {
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus = 0x00; 
               /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.
                sCellSchInfo[dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame] = 0x00; 
#endif
               /** SPR 14277 Changes End **/
            }
            /* - SPR 13605 Fix */
            return MAC_FAILURE;
        }
        /*Cyclomatic Complexity changes - ends here */
    }
    else
    {
        /*Cyclomatic Complexity changes - starts here */
        if (MAC_FAILURE == allocPDCCHPdcchPowerOrAggregationDisabledOrNoWideBandReports(&dciFormat,
                                                                                        dlStrategyNode_p,
                                                                                        ueSearchSpaceInfo_p,
                                                                                        availableCCE_p,
                                                                                        allocatedCCEIndex_p,
																						dciCCEInfo_p,dlUEContext_p,&powerOffset,
																						dciCCEContainerInfo_p,
																						internalCellIndex))
        {
            /* + SPR 13605 Fix */
            if(!IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex) && 
                    dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
            {
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus = 0x00; 
               /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.
                sCellSchInfo[dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame] = 0x00; 
#endif
               /** SPR 14277 Changes End **/
            }
            /* - SPR 13605 Fix */
            return MAC_FAILURE;
        }
        /*Cyclomatic Complexity changes - ends here */
    }
    /*Populating the dlMsgTypeMsgInfo_p structure which will be used by PDSCH
      for allocating the RBs */
    dlDataMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;  
    dlMsgTypeMsgInfo_p->msgType = dlStrategyNode_p->msgType;
    dlDataMsgInfo_p->dlStrategyTxNode_p = dlStrategyNode_p;      
    dlDataMsgInfo_p->cceIndex = *allocatedCCEIndex_p;   
    dlDataMsgInfo_p->dciFormat = dciFormat;    
    /* + TM7_8 Changes Start */
    dlHarqProcess_p->dciFormat = dciFormat;
    /* - TM7_8 Changes End */
    dciCCEInfo_p[*allocatedCCEIndex_p].dciPduInfo.txPower = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
        cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset + powerOffset;
    /* SPR 3267 START */
    if (dciCCEInfo_p[*allocatedCCEIndex_p].dciPduInfo.txPower > MAX_TX_POWER_OFFSET)
    {
        dciCCEInfo_p[*allocatedCCEIndex_p].dciPduInfo.txPower = MAX_TX_POWER_OFFSET;
    }
    /* SPR 3267 END */

    return MAC_SUCCESS;   
}

/*Cyclomatic Complexity changes - starts here */
/**********************************************************************************
 * Function Name  : allocPDCCHPdcchPowerOrAggregationEnabledWithWideBandReports 
 * Inputs         : dlUEContext_p - This holds pointer to DLUEContext structure. 
 *                  dlStrategyNode_p -  This holds the pointer to structure which 
 *                     store the information of those UE for either Re or New 
 *                     Transmission is needed.
 *                 ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo 
 *                     structure which holds the CCEs Index for a UE at each of the 
 *                     aggregation level.
 *                 dciCCEContainerInfo_p -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : dciFormat,powerOffset
 *                 dciCCEInfo_p -  This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 *                 availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by
 *                    one .
 *                 allocatedCCEIndex_p - This holds the value of Allocated CCE Index
 * Returns        : MacRetType
 * Description    : This func will allocate CCE when PDCCHPdcchPowerOrAggregation 
 *                     enabled and WideBandReports are enaabled   
 ************************************************************************************/
STATIC  MacRetType allocPDCCHPdcchPowerOrAggregationEnabledWithWideBandReports(DLUEContext *dlUEContext_p,
                                                                               DCIFormat  *dciFormat,
                                                                               UInt32 *powerOffset,
                                                                               DLStrategyTxNode *dlStrategyNode_p, 
                                                                               UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
                                                                               UInt32 *availableCCE_p,
                                                                               UInt32 *allocatedCCEIndex_p,
                                                                               DciCCEInfo *dciCCEInfo_p,
                                                                               /*pdcch for scell changes start*/
                                                                               DciCCEContainerInfo *dciCCEContainerInfo_p,
                                                                               /*pdcch for scell changes end*/
                                                                               InternalCellIndex internalCellIndex
                                                                               )
{
    UInt32 cqiValue = 0;
    UInt32 dciSize = 0;
    AggregationPowerOffset *aggregationLevelPowerOffset_p = PNULL;
    /* + coverity 32301 */
    /*pdcch allocation For scell changes start*/
    UserLocation userLocationType = (UserLocation) getUserLocationType(dlUEContext_p,internalCellIndex);
    /*pdcch allocation For scell changes end*/
    /* - coverity 32301 */
    UInt32 dciSizeCategory = 0;
    UInt32 aggrLvlCount = 0;
    UInt32 aggregationLevel = 0;
    UInt32 aggrLvlBitMap = 0;
    UInt32 localCount = 0;
    MacRetType retVal = MAC_FAILURE;

    /* CA_phase2_harq_code Start */
    DLHARQProcess  *harqProcess_p = PNULL;
    UInt32 harqProcessId = 0;
    /* CA_phase2_harq_code End */
    CellConfigParams *cellParams_p = \
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*pdcch allocation For scell changes start*/
    DLMIMOInfo *dlMIMOInfo_p = PNULL;
    DLCQIInfo *dlCQIInfo_p = PNULL;

    dlMIMOInfo_p=getdlMIMOInfo(dlUEContext_p,internalCellIndex);
    dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
    if((dlMIMOInfo_p == PNULL) ||(dlCQIInfo_p == PNULL))
    {
      return MAC_FAILURE;
    }
    /*pdcch allocation For scell changes end*/

    
 
    if ((TX_MODE_3 == dlMIMOInfo_p->transmissionMode ||
                TX_MODE_4 == dlMIMOInfo_p->transmissionMode ||
                /* + TM7_8 Changes Start */ 
                ((TX_MODE_8 == dlMIMOInfo_p->transmissionMode) &&
                 (1 < cellParams_p->numOfTxAnteenas)))&&
            /* - TM7_8 Changes End */ 
            (dlCQIInfo_p->widebandCQICodeWordOne >=
             dlCQIInfo_p->widebandCQICodeWordTwo))
    { 
        cqiValue = dlCQIInfo_p->widebandCQICodeWordTwo;
    }
    else
    {
        cqiValue = dlCQIInfo_p->widebandCQICodeWordOne;

    }
	


    /*Populating the dciFormat with the Primary DCI Format ,So that if CCEs
      can be allocated on Primary DCI Format ,dlMsgTypeMsgInfo_p structure
      can be populated by this dciFormat.dlMsgTypeMsgInfo_p structure will
      be used by PDSCH Module for PDSCH region Allocation*/
    *dciFormat = dlStrategyNode_p->primaryDCIFormat;

    /* + TM7_8 Changes Start */ 
    if((TX_MODE_7 == dlMIMOInfo_p->transmissionMode)&&
            (4 == currentCFIValue_g[internalCellIndex]))
    {
        *dciFormat = DCI_FORMAT_1A;
    }
    /* - TM7_8 Changes End */ 

    /*Wideband CQI is available, aggregation level will be decided using
     * DCISize*/
    dciSize = getDCIBitSize( *dciFormat, 
            cellParams_p->numOfTxAnteenas, 
            cellParams_p->dlAvailableRBs );

    /*Now find if this DCI size lies in big/small/medium DCI size category.
     * This result will be used to find the aggregation level*/
    dciSizeCategory = getDCISizeCategory(dciSize);
    /* ICIC changes start */
    aggregationLevelPowerOffset_p = 
        &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                pdcchAggregationAndPowerOffset[userLocationType].\
                aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);
    /* ICIC changes end */

    /* CA_phase2_harq_code Start */
    harqProcessId = dlStrategyNode_p->harqProcessId;
    /* + SPR 11469 Changes */
    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        harqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess[harqProcessId];
    }
    else
    {
        harqProcess_p = &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->\
                            ScelldlHarqContext_p->harqProcess[harqProcessId];
    }
    /* - SPR 11469 Changes */

    /* CA_phase2_harq_code End */

    if (aggregationLevelPowerOffset_p->count)
    {
        /*The static power control table returned valid values.*/
        aggrLvlCount = aggregationLevelPowerOffset_p->count;

        aggrLvlCount = ((aggrLvlCount <= 4)?aggrLvlCount:4);

        while (aggrLvlCount > localCount)
        {
            aggregationLevel = aggregationLevelPowerOffset_p->
                aggregationPowerOffsetTuples[localCount].aggregationLevel;

            if ( !(aggrLvlBitMap & aggregationLevel))
            {
                *powerOffset = aggregationLevelPowerOffset_p->
                    aggregationPowerOffsetTuples[localCount].powerOffset;

                /* CA_phase2_harq_code End */
                retVal = allocatePDCCHForDCIFormatUESpace(
                        dlUEContext_p,
                        ueSearchSpaceInfo_p,
                        allocatedCCEIndex_p,
                        availableCCE_p,
                        dciCCEInfo_p,
                        aggregationLevel,
                        dciCCEContainerInfo_p,
                        internalCellIndex);

                if (retVal == MAC_SUCCESS)
                {
                    break;
                }
                else
                {
                    LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                            getCurrentTick(),\
                            /* SPR 16766 fix start */
                            dlUEContext_p->ueIndex, \
                            /* SPR 16766 fix en */
                            *dciFormat,\
                            aggregationLevel,\
                            cellParams_p->powerControlEnableInfo.\
                            pdcchPowOrAggregationEnable,\
                            0,\
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            __func__, "PDCCH_FAIL");
                }
            }

            aggrLvlBitMap |= aggregationLevel;
            localCount++;
        }
    }

    if (retVal == MAC_FAILURE)
    {
        LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                getCurrentTick(),\
                /* SPR 16766 fix start */
                dlUEContext_p->ueIndex, \
                /* SPR 16766 fix en */
                *dciFormat,\
                aggregationLevel,\
                cellParams_p->powerControlEnableInfo.\
                pdcchPowOrAggregationEnable,\
                0,\
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "PDCCH_FAIL");

        /* + CQI_4.1 */ 
        if ( MAX_NUM_DCI_FORMAT <= dlStrategyNode_p->secondaryDCIFormat)
        {
            return MAC_FAILURE;
        }
        /* - CQI_4.1 */ 
        *dciFormat = dlStrategyNode_p->secondaryDCIFormat;
        /* SPR 5633 changes start */
        if(DCI_FORMAT_1A == dlStrategyNode_p->secondaryDCIFormat)    
        {
            if(harqProcess_p->isTBTwoValid)
            {
                if(RE_TX != harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                {
                    harqProcess_p->isTBTwoValid = TB_INVALID;
                    harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
                }
                else if(RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                {
                    return MAC_FAILURE;
                }
            }
        }
        /* SPR 5633 changes end */
        dciSize =
            getDCIBitSize(*dciFormat, 
                    cellParams_p->numOfTxAnteenas, 
                    cellParams_p->dlAvailableRBs);

        /*Now find if this DCI size lies in big/small/medium DCI size 
          category.This result will be used to find the aggregation level*/
        dciSizeCategory = getDCISizeCategory(dciSize);
        /* ICIC changes start */
        aggregationLevelPowerOffset_p = 
            &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->pdcchAggregationAndPowerOffset\
                    [userLocationType].aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);
        /* ICIC changes end */
        if (aggregationLevelPowerOffset_p->count)
        {
            localCount = 0;
            /*The static power control table returned valid values.*/
            aggrLvlCount = aggregationLevelPowerOffset_p->count;

            aggrLvlCount = ((aggrLvlCount <= 4)?aggrLvlCount:4);

            while (aggrLvlCount > localCount)
            {
                aggregationLevel = aggregationLevelPowerOffset_p->
                    aggregationPowerOffsetTuples[localCount].\
                    aggregationLevel;

                if ( !(aggrLvlBitMap & aggregationLevel))
                {
                    *powerOffset = aggregationLevelPowerOffset_p->
                        aggregationPowerOffsetTuples[localCount].\
                        powerOffset;

                    /* CA_phase2_harq_code Start */
                                            /* CA_phase2_harq_code End */

                    retVal = allocatePDCCHForDCIFormatUESpace(
                            dlUEContext_p,
                            ueSearchSpaceInfo_p,
                            allocatedCCEIndex_p,
                            availableCCE_p,
                            dciCCEInfo_p,
                            aggregationLevel,
                            dciCCEContainerInfo_p,
                            internalCellIndex);

                    if (retVal == MAC_SUCCESS)
                    {
                        break;
                    }
                    else
                    {
                        LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                                getCurrentTick(),\
                                /* SPR 16766 fix start */
                                dlUEContext_p->ueIndex, \
                                /* SPR 16766 fix en */
                                *dciFormat,\
                                aggregationLevel,\
                                cellParams_p->powerControlEnableInfo.\
                                pdcchPowOrAggregationEnable,\
                                0,\
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                __func__, "PDCCH_FAIL");
                    }
                }

                aggrLvlBitMap |= aggregationLevel;
                localCount++;
            }
        }

        if (retVal == MAC_FAILURE)
        {
            LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                    getCurrentTick(),\
                    /* SPR 16766 fix start */
                    dlUEContext_p->ueIndex, \
                    /* SPR 16766 fix en */
                    *dciFormat,\
                    aggregationLevel,\
                    cellParams_p->powerControlEnableInfo.\
                    pdcchPowOrAggregationEnable,\
                    0,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "PDCCH_FAIL");
            /* +  SPR_14042 */
            /* For a UE, CCEs can not be allocated from comman search space for Scell */
            if(!IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
            {
                return MAC_FAILURE;
            } 
            /* + SPR_14042 */
            /*Now when we try to allocate the CCE On Common Search Space
             *we will try to allocate DCI Format 1A.In case of Spatial
             *Multiplexing when both TB are Valid and either of ReTransmitted
             *TB,we should not even try to allocated DCI Format 1A as it can
             *have only One TB Transmission,where as we need to transmit
             *Two TB*/
            if ( TX_MODE_3 == dlMIMOInfo_p->transmissionMode ||
                    TX_MODE_4 == dlMIMOInfo_p->transmissionMode ||
                    /* + TM7_8 Changes Start */ 
                    ((TX_MODE_8 == dlMIMOInfo_p->transmissionMode) &&
                     (1 < cellParams_p->numOfTxAnteenas)))
                /* - TM7_8 Changes End */ 
            {
                if ( harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid )
                {
                    /* SPR 5633 changes start */
                    /* SPR 1868 changes start */
                    if ((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) ||
                            (RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
                        /* SPR 1868 changes end */
                        /* SPR 5633 changes end */
                    {
                        return MAC_FAILURE;
                    }
                    else if ( NEW_TX == 
                        harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                    {
                        harqProcess_p->isTBTwoValid = TB_INVALID;
                    }
                    else
                    {
                        harqProcess_p->isTBOneValid = TB_INVALID;
                    }
                }
                /* SPR 5633 changes start */
                else if( ((harqProcess_p->isTBTwoValid) && 
                            (RE_TX == 
                             harqProcess_p->dlHARQTBTwoInfo.txIndicator)) ||
                        (((DCI_FORMAT_2A == harqProcess_p->dciFormat) ||
                          (DCI_FORMAT_2 == harqProcess_p->dciFormat)||
                          (DCI_FORMAT_2B == harqProcess_p->dciFormat)) &&
                         ((harqProcess_p->isTBOneValid) && 
                          (RE_TX == 
                           harqProcess_p->dlHARQTBTwoInfo.txIndicator))) )
                {
                    return MAC_FAILURE;
                }
                /* SPR 5633 changes end */
            }
            /*Comes here when the Aggregagation Level of dciFormat
              is Four*/

            *dciFormat = DCI_FORMAT_1A;

            if ( MAC_SUCCESS == isCCEAvailableAggrLvlFourCommonSpace(
                        allocatedCCEIndex_p,availableCCE_p,dciCCEInfo_p,
                        internalCellIndex) )
            {
                *powerOffset = 0;
                /* SPR 6858 Changes start */
                /* Search the power offset value corresponding to aggrgation level 4 */
                if (aggrLvlCount)
                {
                    for (localCount=0; localCount< aggrLvlCount; localCount++)
                    {
                        if (AGGREGATION_LEVEL_FOUR == 
                                aggregationLevelPowerOffset_p->aggregationPowerOffsetTuples[localCount].aggregationLevel)
                        {
                            *powerOffset = aggregationLevelPowerOffset_p->aggregationPowerOffsetTuples[localCount].powerOffset;
                        }
                    }
                }
                /* SPR 6858 Changes end */
            }
            else
            {
                LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                        getCurrentTick(),\
                        /* SPR 16766 fix start */
                        dlUEContext_p->ueIndex, \
                        /* SPR 16766 fix en */
                        *dciFormat,\
                        aggregationLevel,\
                        cellParams_p->powerControlEnableInfo.\
                        pdcchPowOrAggregationEnable,\
                        0,\
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__, "COM_FAIL");

                return MAC_FAILURE;
            }
        }
    }
    return MAC_SUCCESS;
}
/*************************************************************************************
 * Function Name  : allocPDCCHPdcchPowerOrAggregationDisabledOrNoWideBandReports 
 * Inputs         : dlStrategyNode_p -  This holds the pointer to structure which 
 *                     store the information of those UE for either Re or New 
 *                     Transmission is needed.
 *                 ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo 
 *                     structure which holds the CCEs Index for a UE at each of the 
 *                     aggregation level.
 *                 dlUEContext_p - This holds pointer to DLUEContext structure.
 *                 dciCCEContainerInfo_p -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : dciFormat,powerOffset
 *                 dciCCEInfo_p -  This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 *                 availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by
 *                    one .
 *                 allocatedCCEIndex_p - This holds the value of Allocated CCE Index
 * Returns       : MacRetType
 * Description   : This func will allocate CCE when PDCCHPdcchPowerOrAggregationDisabled 
*                   or WideBandReports are disabled   
 ****************************************************************************************/
STATIC  MacRetType allocPDCCHPdcchPowerOrAggregationDisabledOrNoWideBandReports(DCIFormat  *dciFormat,
                                                                                DLStrategyTxNode *dlStrategyNode_p,                                          
                                                                                UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
                                                                                UInt32 *availableCCE_p,
                                                                                UInt32 *allocatedCCEIndex_p,
                                                                                DciCCEInfo *dciCCEInfo_p,
                                                                                DLUEContext *dlUEContext_p,
                                                                                UInt32 *powerOffset,
                                                                                DciCCEContainerInfo *dciCCEContainerInfo_p,
                                                                                InternalCellIndex internalCellIndex)
{
    UInt32 aggregationLevel = 0;
#ifdef LOG_PRINT_ENABLED
    ULUEContext *ulUEContext_p = PNULL;
#endif
    /* + coverity 32301 */
    /*pdcch allocation For scell changes start*/
    UserLocation userLocationType = (UserLocation) getUserLocationType(dlUEContext_p,internalCellIndex);
    /*pdcch allocation For scell changes end*/
    /* - coverity 32301 */

    /*Populating the dciFormat with the Primary DCI Format ,So that if CCEs
      can be allocated on Primary DCI Format ,dlMsgTypeMsgInfo_p structure
      can be populated by this dciFormat.dlMsgTypeMsgInfo_p structure will 
      be used by PDSCH Module for PDSCH region Allocation*/
    *dciFormat = dlStrategyNode_p->primaryDCIFormat;

    CellConfigParams *cellParams_p = \
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* CA_phase2_harq_code Start */

    MacRetType retVal = MAC_FAILURE;

    DLHARQProcess  *harqProcess_p = PNULL;
    UInt32 harqProcessId = 0;
    /* CA_phase2_harq_code End */

    /*pdcch allocation For scell changes start*/
    DLMIMOInfo *dlMIMOInfo_p = PNULL;
    dlMIMOInfo_p=getdlMIMOInfo(dlUEContext_p,internalCellIndex);
    if((dlMIMOInfo_p==PNULL) )
    {
      return MAC_FAILURE;
    }
    /*pdcch allocation For scell changes end*/

    /* + TM7_8 Changes Start */ 
    if((TX_MODE_7 == dlMIMOInfo_p->transmissionMode)&&
            (4 == currentCFIValue_g[internalCellIndex]))
    {
        *dciFormat = DCI_FORMAT_1A;
    }
    /* - TM7_8 Changes End */ 

#ifdef LOG_PRINT_ENABLED
    UInt32 ueIndex = dlStrategyNode_p->ueIndex;
    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
#endif

    /* CA_phase2_harq_code Start */
    harqProcessId = dlStrategyNode_p->harqProcessId;
    /* + SPR 11469 Changes */
    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        harqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess[harqProcessId];
    }
    else
    {
        harqProcess_p= &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->\
                    ScelldlHarqContext_p->harqProcess[harqProcessId];
    }
    /* - SPR 11469 Changes */
   
    /* CA_phase2_harq_code End */

    /* ICIC changes start */ 
    aggregationLevel = dciFormatToAggrLvlMapping_g[internalCellIndex].\
                      dciFormatToAggregationLevel[userLocationType][dlStrategyNode_p->primaryDCIFormat];
    /* ICIC changes end */ 

    /* CA_phase2_harq_code Start */

    retVal = allocatePDCCHForDCIFormatUESpace(
            dlUEContext_p,
            ueSearchSpaceInfo_p,
            allocatedCCEIndex_p,availableCCE_p,dciCCEInfo_p,
            aggregationLevel,dciCCEContainerInfo_p,internalCellIndex);

    
    if (MAC_FAILURE == retVal)
    {
    /* CA_phase2_harq_code End */
        LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                getCurrentTick(),\
                ueIndex,\
                *dciFormat,\
                aggregationLevel,\
                cellParams_p->powerControlEnableInfo.\
                pdcchPowOrAggregationEnable,\
                ulUEContext_p->isDLWideBandCQIReportAvailable,\
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "COM_FAIL");

        /*Checking if the Secondary DCI Format is Valid or Not */   

        *dciFormat = dlStrategyNode_p->secondaryDCIFormat;

        if ( MAX_NUM_DCI_FORMAT <= dlStrategyNode_p->secondaryDCIFormat)
        {
            return MAC_FAILURE;           
        }
        /* ICIC changes start */ 
        aggregationLevel =dciFormatToAggrLvlMapping_g[internalCellIndex].\
                          dciFormatToAggregationLevel[userLocationType][*dciFormat];
        /* ICIC changes end */ 

        /*Calling funtion allocatePDCCHForDCIFormatUESpace which will  try to 
          allocate CCE according to Secondary  DCI Format ,If CCE allocation
          for Primary DCI Format is Failed */

        /* CA_phase2_harq_code Start */

        retVal = allocatePDCCHForDCIFormatUESpace(
                dlUEContext_p,
                ueSearchSpaceInfo_p,
                allocatedCCEIndex_p,availableCCE_p,dciCCEInfo_p,
                aggregationLevel,dciCCEContainerInfo_p,internalCellIndex);

        if (MAC_FAILURE == retVal)
        {        
            /* CA_phase2_harq_code End */
            LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                    getCurrentTick(),\
                    ueIndex,\
                    *dciFormat,\
                    aggregationLevel,\
                    cellParams_p->powerControlEnableInfo.\
                    pdcchPowOrAggregationEnable,\
                    ulUEContext_p->isDLWideBandCQIReportAvailable,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "PDCCH_FAIL");
            /* + SPR_14042 */
            /* For a UE, CCEs can not be allocated from comman search space for Scell */
            if(!IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
            {
                return MAC_FAILURE;
            } 
            /* + SPR_14042 */

            /*Now when we try to allocate the CCE On Common Search Space
             *we will try to allocate DCI Format 1A.In case of Spatial
             *Multiplexing when both TB are Valid and either of ReTransmitted
             *TB,we should not even try to allocated DCI Format 1A as it can
             *have only One TB Transmission,where as we need to transmit
             *Two TB*/            
            /* SPR 6136 start */
            if ((RE_TX != harqProcess_p->dlHARQTBOneInfo.txIndicator) ||
                (RE_TX != harqProcess_p->dlHARQTBTwoInfo.txIndicator))
            { 
                if ( TX_MODE_3 == dlMIMOInfo_p->transmissionMode ||
                        TX_MODE_4 == dlMIMOInfo_p->transmissionMode ||
                        /* + TM7_8 Changes Start */ 
                        ((TX_MODE_8 == dlMIMOInfo_p->transmissionMode) &&
                         (1 < cellParams_p->numOfTxAnteenas)))
                        /* - TM7_8 Changes End */ 
                {
                    if ( harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid )
                    {
                        /* SPR 1868 changes start */
                        if ((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) &&
                            (RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
                        /* SPR 1868 changes end */
                        {
                            return MAC_FAILURE;
                        }
                        else if ( NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                        {
                            harqProcess_p->isTBTwoValid = TB_INVALID;   
                        }
                        else
                        {
                            harqProcess_p->isTBOneValid = TB_INVALID;
                        }
                    }
                }
            }
            if ((RE_TX != harqProcess_p->dlHARQTBOneInfo.txIndicator) || 
	        (RE_TX != harqProcess_p->dlHARQTBTwoInfo.txIndicator))
            {
                *dciFormat = DCI_FORMAT_1A;

            }
            /* SPR 6136 end */


            /*Comes here when the Aggregagation Level of dciFormat
              is Four*/
            if ( MAC_SUCCESS == isCCEAvailableAggrLvlFourCommonSpace(
                        allocatedCCEIndex_p,availableCCE_p,dciCCEInfo_p,
                        internalCellIndex) )
            {
                /*Coverity_31963 Fix start*/
                *powerOffset = 0;
                /*Coverity_31963 Fix end*/
                    
                LOG_MAC_MSG ( MAC_COMMON_CCES_ALLOCATED_ID, LOGDEBUG, MAC_DL_PC,\
                        getCurrentTick(),\
                        ueIndex,\
                        *dciFormat,\
                        *allocatedCCEIndex_p,\
                        dciCCEInfo_p[ *allocatedCCEIndex_p].dciPduInfo.aggregationLvl,\
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                        __func__, "COM_CCE");
            }
            else
            {
                LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                        getCurrentTick(),\
                            ueIndex,\
                            *dciFormat,\
                            aggregationLevel,\
                            cellParams_p->powerControlEnableInfo.\
                            pdcchPowOrAggregationEnable,\
                            ulUEContext_p->isDLWideBandCQIReportAvailable,\
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            __func__, "COM_FAIL");

                return MAC_FAILURE;
            }       
        }
        else
        {
            *dciFormat = dlStrategyNode_p->secondaryDCIFormat;

            if ( TX_MODE_3 == dlMIMOInfo_p->transmissionMode ||
                        TX_MODE_4 == dlMIMOInfo_p->transmissionMode ||
                      /* + TM7_8 Changes Start */ 
                      ((TX_MODE_8 == dlMIMOInfo_p->transmissionMode) &&
                       (1 < cellParams_p->numOfTxAnteenas)))
                      /* - TM7_8 Changes End */ 
            {
                if ( harqProcess_p->isTBOneValid && harqProcess_p->isTBTwoValid )
                {
                    if ((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) &&
                            (RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
                    {
                        mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl, dciCCEInfo_p,
                                internalCellIndex);
                        /*pdcch for scell changes start*/
                        mac_reset_extra_cce(dciCCEContainerInfo_p,dciCCEInfo_p,dlUEContext_p,internalCellIndex);
                        /*pdcch for scell changes end*/
                        return MAC_FAILURE;
                    }
                    else if ( NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
                    {
                        harqProcess_p->isTBTwoValid = TB_INVALID;   
                    }
                    else
                    {
                        harqProcess_p->isTBOneValid = TB_INVALID;
                    }
                }
            }
        }
    }
    return MAC_SUCCESS;
}
/******************************************************************************
 * Function Name   : extraCCEAllocated
 * Inputs          : dciCCEContainerInfo_p-Pointer to DCI CCE Container Information
 *                   cceIndex-cce index to be checked in array of Extra CCE Allocated
 * Outputs         : None
 * Returns         : None
 * Description     :This Function checks whether cce Index is present in 
 *                   arrayExtraCCEAllocated or not.
 *******************************************************************************/
UInt8 extraCCEAllocated(
        DciCCEContainerInfo *dciCCEContainerInfo_p,UInt8 cceIndex)
{
    UInt8 cceCount=0;
    cceCount = dciCCEContainerInfo_p->countOfExtraCCEAllocated;
    while(cceCount--)
    {
       if( cceIndex==dciCCEContainerInfo_p->arrayExtraCCEAllocated[cceCount])
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*Cyclomatic Complexity changes - ends here */
/******************************************************************************
 * Function Name   : emptyDLStrategyQueues
 * Inputs          : internalCellIndex - Cell-Index at MAC
 * Outputs         : None
 * Returns         : None
 * Description     : This func empties any strategy nodes which 
 *                   couldnt be processed by pdcch and frees the harq processes
 *                   in case of new transmissions and QoS based scheduling
 *******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            

/** SPR 13599 Changes Start **/
STATIC  void emptyDLStrategyQueues(InternalCellIndex internalCellIndex) 
/** SPR 13599 Changes End **/
/* SPR 5599 changes end (ZIP ID 129195) */            
{
	DLStrategyTxNode *dlStrategyTxNode = PNULL;
	UInt32 countDLUEReTx  = sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex]);
	UInt16 ueIndex = 0; 
	UInt32 countDLUENewTx  = sQueueCount(dlStrategyNewTxQueue_gp[internalCellIndex]);
	DLUEContextInfo *dlUEContextInfo_p = PNULL;
	DLUEContext *dlUEContext_p = PNULL;
    DLStrategyTxNode **tempDlStrategyTxNode_p = PNULL;
	while ( countDLUEReTx-- )
	{
		dlStrategyTxNode = (DLStrategyTxNode*)
			popNode( dlStrategyReTxQueue_gp[internalCellIndex]);        
		if ( PNULL ==  dlStrategyTxNode )
		{
			LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
					"sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex] ) = %u\n",
                    sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex]));
            continue;
        }
        ueIndex =  dlStrategyTxNode->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        /* SPR# 9910 fix start */
        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /* SPR# 9910 fix end */
        if ( dlUEContextInfo_p->pendingDeleteFlag )
        {
            freeMemPool(dlStrategyTxNode);
            /* SPR# 9910 fix start */
            dlUEContext_p->dlStrategyTxNode_p = PNULL;
            /* SPR# 9910 fix end */
            continue;
        }

        /* ICIC changes start */
        UPDATE_EXCESS_POOL_RB(dlStrategyTxNode->resourceAllocInput.
                resourceRegionAllocated, 
                dlStrategyTxNode->resourceAllocInput.requiredRB,
                internalCellIndex);
        if (CE_REGION == 
                dlStrategyTxNode->resourceAllocInput.resourceRegionAllocated)
        {
            excessPoolRb_g[internalCellIndex][CC_CE_REGION] += 
                dlStrategyTxNode->resourceAllocInput.requiredRB;
        }
        /* ICIC changes end */

        /* dl Qos Changes */
        /* reset the flag for New Transmission */
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            dlUEContext_p->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
        }
        else
        {
            dlUEContext_p->dlUeScellContext_p[1]->scheduleFlag = DL_QOS_NEW_TRANSMISSION; 
        }
        putEntryInRetransmissionFailureQueue(dlStrategyTxNode,internalCellIndex);
    }

    while ( countDLUENewTx-- )
    {
        dlStrategyTxNode = (DLStrategyTxNode*)
            popNode( dlStrategyNewTxQueue_gp[internalCellIndex]);        
        if ( PNULL ==  dlStrategyTxNode )
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "sQueueCount(dlStrategyNewTxQueue_gp[internalCellIndex] ) = %u\n",
                    sQueueCount(dlStrategyNewTxQueue_gp[internalCellIndex]));
            continue;
        }
        ueIndex =  dlStrategyTxNode->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        /* SPR# 9910 fix start */
        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /* SPR# 9910 fix end */
        if ( dlUEContextInfo_p->pendingDeleteFlag )
        {
            freeMemPool(dlStrategyTxNode);
            /* SPR# 9910 fix start */
            dlUEContext_p->dlStrategyTxNode_p = PNULL;
            /* SPR# 9910 fix end */
            continue;
        }

        /* dl Qos Changes */
        /* reset the flag for New Transmission */
        DLHARQProcess  *harqProcess_p = PNULL;
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            dlUEContext_p->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
            harqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess
                [dlStrategyTxNode->harqProcessId];
            tempDlStrategyTxNode_p = &(dlUEContext_p->dlStrategyTxNode_p);
        }
        else
        {
            dlUEContext_p->dlUeScellContext_p[1]->scheduleFlag = DL_QOS_NEW_TRANSMISSION; 
            harqProcess_p = &dlUEContext_p->dlUeScellContext_p[1]->\
                        ScelldlHarqContext_p->harqProcess[dlStrategyTxNode->harqProcessId];
            tempDlStrategyTxNode_p = &(dlUEContext_p->dlUeScellContext_p[1]->dlStrategyTxNode_p);
        }

        /* ICIC changes start */
        UPDATE_EXCESS_POOL_RB(dlStrategyTxNode->resourceAllocInput.
                resourceRegionAllocated, 
                dlStrategyTxNode->resourceAllocInput.requiredRB,
                internalCellIndex);
        if (CE_REGION == 
                dlStrategyTxNode->resourceAllocInput.resourceRegionAllocated)
        {
            excessPoolRb_g[internalCellIndex][CC_CE_REGION] += 
                dlStrategyTxNode->resourceAllocInput.requiredRB;
        }
        /* SPR 3750 Fix Start */
        if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
	                initParams_p->dlSchdConfig.dlSchedulerStrategy)
            /* SPR 3750 Fix End */
        {
            switch(harqProcess_p->dlHARQTBOneInfo.taDrxFlag)
            {
                case TA_MSG:
                    /* SPR 9546 Fix Start */
                    putEntryInPdcchPdschFailureTAQueue(ueIndex,
                            dlUEContext_p->timingAdvanceValue, 
                            internalCellIndex);
                    /* SPR 9546 Fix End */
                    break;
                default:
                    break;
                    /* No TA_DRX Confiured. Hence do nothing */
            }

            if( ( (harqProcess_p->isTBOneValid) &&
                  (harqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)) ||
                ( (harqProcess_p->isTBTwoValid) &&  
                  (harqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)))
            {
                putEntryInPendingFailureDlCAMacCEQueue( dlStrategyTxNode->ueIndex,
                                                        harqProcess_p,
                                                        internalCellIndex);
            }

        }
        else
        {        switch(harqProcess_p->dlHARQTBOneInfo.taDrxFlag)
            {
                case TA_MSG:
                    /* +coverity-530-CID 24492*/
                    if(MAC_FAILURE == putEntryInAdvanceAllocInfoQueue(dlStrategyTxNode,internalCellIndex))
                    {
                        freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);
                        freeMemPool(dlStrategyTxNode);
                        *tempDlStrategyTxNode_p = PNULL;
                        
                    }
                    /* +coverity-530-CID 24492*/
                    /* + SPR 5584/5510 Fix Start */
                    continue;
                    /* - SPR 5584/5510 Fix End */
                default:
                    break;
                    /* No TA_DRX Confiured. Hence do nothing */
            }

        }
        /* ICIC changes end */
        freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);
        /* SPR 3750 Fix Start */
        freeMemPool(dlStrategyTxNode);
        /* SPR 3750 Fix End */
        /* SPR# 4906 Changes Starts   */
        *tempDlStrategyTxNode_p = PNULL;
        /* SPR# 4906 Changes Ends */
    }
}

/*************************************************************************************************
 * Function Name  : allocatePDCCHDLReTx               
 * Inputs         : subFrameNum - This holds the subFrame Number
 *                  maxUeScheduledInDownlink_p - Maximum Number of UE's to be scheduled in DL
 *                  dciCCEContainerInfo_p -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : availableDLRBCount_p - This holds the number of RBs in Downlink.Once 
 *                      CCEs are allocated for a UE,this would be decremented by the number of 
 *                      required RB for that UE.
 *                  availableCCE_p - This represent the number of available CCE
 *                      and if CCE's are allocated this would be deceremented by
 *                      the number of CCEs' allocated .
 *                  dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                      Information is there and once cce is allocated ,updated 
 *                      information is written on the reserved CCE.
 *                  pdcchDLOutputInfo_p - This is pointer to structure which holds 
 *                      the information of those UE for which PDCCH region has been 
 *                      allocated and for which PDSCH region need to be allocated.
 *                      Once CCE are allocated , allocatePDCCHDLReTx funtions writes 
 *                      relevant information to the structure which will be used by 
 *                      PDSCH.
 *                  directIndexingUEInfo_p - This stores pointer to 
 *                      DirectIndexingUEInfo structure    
 *                  pdcchDLUEInfo_p - This holds pointer to PdcchDLOutputInfo structure.
 * Returns        : None
 * Description    : This funtions process the DL Re Transmission Queue and call
 *                  funtion allocatePDCCHForDLNewReTx which will return status
 *                  if the CCEs are allocated or not.If return status is Success 
 *                  then it will update the pdcchDLOutputInfo_p and 
 *                  pdcchULOutputInfo_p  structure with relevant information.
 *****************************************************************************i*********************/
/** SPR 13599 Changes Start **/
#ifdef FDD_CONFIG
STATIC  MacRetType allocatePDCCHDLReTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DirectIndexingUEInfo *directIndexingUEInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        )
#elif TDD_CONFIG
STATIC  MacRetType allocatePDCCHDLReTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        )
#endif
/** SPR 13599 Changes End **/
{
    UInt32 countDLUEReTx =0;
    UInt16 ueIndex = 0; 
    UInt32 arrayIndextDlMsgTypeMsg = 0;   
    SInt32 tempAvailableDLRBCount= 0;
    DLStrategyTxNode *dlStrategyTxNode = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p = PNULL;    
    UInt32 allocatedCCEIndex = 0;
    UInt32 arrayIndexDLUE = 0;
    UInt16 *tempPdcchDLUEArrayInfo_p = PNULL;
    /* ICIC changes start */
    UInt8 userLocationType = CC_USER;
    /* ICIC changes start */
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p = PNULL;

    /* + SPR 11491 Changes */
    UInt8 *isPdcchAllocatedFlag_p = PNULL;
    /* - SPR 11491 Changes */
/* SPR 609 changes start
 * dlDataMsgInfo_p is used in FDD_CONFIG
 * */
#ifdef FDD_CONFIG
        DLDataMsgInfo *dlDataMsgInfo_p = PNULL;
#endif
/* SPR 609 changes end */
#ifdef TDD_CONFIG
    UInt8 isSpecialSubFrame = checkForSpecialSubframe(subFrameNum, internalCellIndex);
    UeScheduledInfo * ueScheduledInfo_p = PNULL;
#endif
    /* SPR 22198 fix start */
    DLStrategyTxNode** ueDlStrategyTxNode_p = PNULL;
    UInt8 harqProcessId = INVALID_HARQ_ID;
    DLHARQProcess *harqProcess_p = PNULL;
    /* SPR 22198 fix end */
    DLMIMOInfo  *dlMIMOInfo_p=PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__); 

    /*Getting  the Array Index in dlMsgTypeMsgInfo array of pdcchDLOutputInfo_p
      structure from where if CCE are allocated , allocatePDCCHForDL funtion can
      write relavant information */

    arrayIndexDLUE = pdcchDLUEInfo_p->count;
    tempPdcchDLUEArrayInfo_p = &pdcchDLUEInfo_p->pdcchDLUEArray[arrayIndexDLUE];

    /*Getting the count of DownLink UE For which Re Transmission is requested
      and for which PDCCH Region need to be allocated */
    countDLUEReTx  = sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex] );
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Number of ReTx Node  = %u\n",countDLUEReTx);  
    tempAvailableDLRBCount = *availableDLRBCount_p;
    /*Below while loop is working on the dlStrategyReTxQueue_g queue and for
      each node popped,it get the UE and try to allocate the PDCCH region to
      it.It will iterate  through while loop until both of the condition  is 
      meet
      1) Number of RB allocated is less than max number of RB in downlink
      2) Number of DL UE for which PDCCH need to be allocated 
      is allocated countDLUEReTx greater than 0*/
    while ( countDLUEReTx-- )
    {
        dlStrategyTxNode = (DLStrategyTxNode*)
            popNode( dlStrategyReTxQueue_gp[internalCellIndex]);        
        if ( PNULL ==  dlStrategyTxNode )
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex] ) = %u\n",
                    sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex]));
            continue;
        }
        ueIndex =  dlStrategyTxNode->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        /* SPR# 9910 fix start */
        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /* SPR# 9910 fix end */
        if ( dlUEContextInfo_p->pendingDeleteFlag )
        {
            freeMemPool(dlStrategyTxNode);
            /* SPR# 9910 fix start */
            dlUEContext_p->dlStrategyTxNode_p = PNULL;
            /* SPR# 9910 fix end */
            continue;
        }
        LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                "UE =%u  is in Processing \n",ueIndex);

        /* SPR 22198 fix start */
        /* pdcch for scell changes start*/ 
        dlMIMOInfo_p=getdlMIMOInfo(dlUEContext_p,internalCellIndex);
        if(dlMIMOInfo_p==PNULL )
        {
            return MAC_FAILURE;
        }
        /* pdcch for scell changes end*/ 
        harqProcessId = dlStrategyTxNode->harqProcessId;
        /* SPR 22198 fix end */
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            dlUEContext_p->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
       	    /* SPR 14381 */
            /* SPR 22198 fix start */
            harqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess[harqProcessId];
            ueDlStrategyTxNode_p = &(dlUEContext_p->dlStrategyTxNode_p);
            /* SPR 22198 fix end */
        }
        else
        {
            dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
            /* SPR 22198 fix start */
            harqProcess_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
                            ScelldlHarqContext_p->harqProcess[harqProcessId]);
            ueDlStrategyTxNode_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
       	    /* SPR 14381 */
            /* SPR 22198 fix end */
            /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
            dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame = subFrameNum;
            dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSchInfo[subFrameNum] = SINGLE_TB;
#endif
           /** SPR 14277 Changes End **/
        }

	/* +- SPR 22198 fix */
#ifdef TDD_CONFIG
        if(DL == harqProcess_p->subFrameType)
        {
            if ((isSpecialSubFrame) &&
                    (RE_TX == dlStrategyTxNode->transmissionType ))
            {
                /* SPR 23393 Changes Start */
                putEntryInRetransmissionFailureQueue(dlStrategyTxNode,internalCellIndex);
                /* SPR 23393 Changes End */
                continue;
            }
        }
        else if(SP == harqProcess_p->subFrameType)
        {
            if ((!isSpecialSubFrame) &&
                    (RE_TX == dlStrategyTxNode->transmissionType))
            {
                /* SPR 23393 Changes Start */
                putEntryInRetransmissionFailureQueue(dlStrategyTxNode,internalCellIndex);
                    /* +coverity-530-CID 24492*/
                /* SPR 23393 Changes End */
                continue;
            }
        }
#endif

        /*Checking if Pdcch has been allocated earlier or not .If allocated 
          then skip allocating CCE for this UE .If not allocated then set the 
          flag isPdcchAllocated in UE Context  to true */
        /* + SPR 11491 Changes */
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            isPdcchAllocatedFlag_p = &(dlUEContext_p->isPdcchAllocatedFlag);    
        }
        else
        {
            isPdcchAllocatedFlag_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag);
        }
            
        if (*isPdcchAllocatedFlag_p)
        /* - SPR 11491 Changes */
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "PDCCH has been allocated earlier for this UE = %u",ueIndex);
            /* SPR 22198 fix start */
            freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);
            freeMemPool(dlStrategyTxNode);
            *ueDlStrategyTxNode_p = PNULL;
            /* SPR 22198 fix end */
            continue;
        }
        /* SPR 2527 Changes start */
        /* ICIC changes start */
    /*pdcch allocation For scell changes start*/
        userLocationType = getUserLocationType(dlUEContext_p,internalCellIndex);
    /*pdcch allocation For scell changes end*/
 
        /* SPR 22252 fix start */
        /* There is possibilty that even CE user limit has exhausted CC user
         * will be there. So continue for CC user handling even CE user limit
         * has bee reached*/
        if(((userLocationType == CE_USER)&&
                    (pdcchDLOutputInfo_p->countMsg[CE_USER] ==
                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     initParams_p->dlSchdConfig.maxCellEdgeUEsToBeScheduledDL))&&
                ((pdcchDLOutputInfo_p->countMsg[CC_USER] + 
                  pdcchDLOutputInfo_p->countMsg[CE_USER]) < 
                 cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 initParams_p->dlSchdConfig.maxUEsToBeScheduledDL ))
        {
            updateRBsAndPutEntryinFailureQueue(dlStrategyTxNode,internalCellIndex);
            continue;
        }
        else if ((pdcchDLOutputInfo_p->countMsg[CC_USER] + 
                    pdcchDLOutputInfo_p->countMsg[CE_USER]) >= 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->dlSchdConfig.maxUEsToBeScheduledDL ) 
        {
            updateRBsAndPutEntryinFailureQueue(dlStrategyTxNode,internalCellIndex);
    	    ltePanic("Total number of scheduled users greater than max scheduled UE per TTI"); 
            return MAC_FAILURE;
        }
        /* SPR 22252 fix end */

        /* ICIC changes start */
        arrayIndextDlMsgTypeMsg = pdcchDLOutputInfo_p->countMsg[userLocationType];
        /* Fetching Respectuve structure based on UE location Type */
        dlMsgTypeMsgInfo_p = &pdcchDLOutputInfo_p->
            dlMsgTypeMsgInfo[userLocationType][arrayIndextDlMsgTypeMsg];
        /* ICIC changes end */
        /* SPR 2527 Changes end */
#ifdef FDD_CONFIG
        /*SPS_CHG Check if this Strategy node is for SPS DL Retx*/
        directIndexingUEInfo_p[ueIndex].isSPSCCE = LTE_FALSE;
        /* + SPS_TDD_Changes */
#else
        ulUECtxInfoArr_g[ueIndex].ulUEContext_p->\
            dlUeScheduledInfoPointerArray[internalCellIndex][subFrameNum]->\
            isSPSCCE[subFrameNum] = LTE_FALSE;


#endif
        /* ++ SPR 21406 */
#if TDD_CONFIG        
        /* --  SPR 21406 */
        /* - SPS_TDD_Changes */
        if(dlStrategyTxNode->msgType == SPS_RETX)
            ueSearchSpaceInfo_p =  &dlUEContext_p->spsUeSearchSpaceInfo_p[subFrameNum];
        else
#endif            
        {
           /*pdcch changes start*/
           ueSearchSpaceInfo_p=getDLUEContextSearchSpaceInfo(dlUEContext_p,subFrameNum,internalCellIndex);
           if(!ueSearchSpaceInfo_p)
           {
               continue;
           }
           /*pdcch changes end*/
        }
        /*SPS_CHG Check if this Strategy node is for SPS DL Retx*/
  
        /*Calling funtion allocatePDCCHForDLNewReAdvanceTx which will 
          allocate the CCE to UE */
        if (( *maxUeScheduledInDownlink_p ) && 
                ( MAC_SUCCESS == allocatePDCCHForDLNewReAdvanceTx(
                    dlStrategyTxNode,
                    &allocatedCCEIndex,
                    dlMsgTypeMsgInfo_p,
                    ueSearchSpaceInfo_p,
                    availableCCE_p,
                    dciCCEInfo_p,
                    dlUEContext_p,
                   dciCCEContainerInfo_p,
                    internalCellIndex)) )
        {
            UInt32 cceIndex = 0;
            cceIndex = allocatedCCEIndex;
            dciCCEContainerInfo_p->arrayCCEAllocated
                [dciCCEContainerInfo_p->countOfCCEAllocated] = cceIndex;
            dciCCEContainerInfo_p->countOfCCEAllocated++;
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "cceIndex = %u",cceIndex);
            /*If the DL UE has got Aggregation Level One ,then UL UE cannot be 
             *Scheduled on same Aggregation level ,So mark isPdcchAllocated to
             *one*/
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "aggregationLvl = %u",dciCCEInfo_p[cceIndex].
                    dciPduInfo.aggregationLvl);
            /* + SPR 11491 Changes */
            *isPdcchAllocatedFlag_p = 1;
            /* - SPR 11491 Changes */
            /* ICIC changes start */
            pdcchDLOutputInfo_p->countPriorityMsg[userLocationType]++;
            pdcchDLOutputInfo_p->countMsg[userLocationType]++;
            /* ICIC changes end */
#ifdef FDD_CONFIG            
            directIndexingUEInfo_p[ueIndex].dlCCEIndex = cceIndex;

            /*DCI Format Updated in the directIndexingUEInfo_p*/
            dlDataMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;  
            directIndexingUEInfo_p[ueIndex].dlDciFormat = 
                dlDataMsgInfo_p->dciFormat;
            /* SS_S2 */
            directIndexingUEInfo_p[ueIndex].transMode = dlMIMOInfo_p->
                transmissionMode;
            /* SS_S2 */
            /* + CQI_4.1 */
            directIndexingUEInfo_p[ueIndex].ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;         
/* SPR 14877 SRS Changes */
            directIndexingUEInfo_p[ueIndex].ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
/* SPR 14877 SRS Changes */
            /* - CQI_4.1 */
#ifdef FDD_CONFIG
            /*SPS_CHG*/
            if( dlStrategyTxNode->msgType == SPS_RETX  )
              directIndexingUEInfo_p[ueIndex].isSPSCCE = LTE_TRUE;
            /*SPS_CHG*/ 
#endif
#elif TDD_CONFIG
            /** SPR 14204 Fix : Start **/

            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
            if(setCCEIndexInULContex(ueIndex, subFrameNum,cceIndex,internalCellIndex,
                 dlStrategyTxNode->harqProcess_p->isTBOneValid,

                 /* SPR 19679: TDD HARQ Multiplexing  Changes End */
                 dlStrategyTxNode->harqProcess_p->isTBTwoValid))
            /** SPR 14204 Fix : End **/
            {
              *tempPdcchDLUEArrayInfo_p = ueIndex;           
              arrayIndexDLUE++;
               pdcchDLUEInfo_p->pdcchDLUEArrayForDlGroup[pdcchDLUEInfo_p->cumulativeUeCountForDlGroup] = ueIndex;
               pdcchDLUEInfo_p->cumulativeUeCountForDlGroup++;
           }
            /* SS_S2 */
            /** SPR 14204 Fix : Start **/
            ueScheduledInfo_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->
                dlUeScheduledInfoPointerArray[internalCellIndex][subFrameNum];
            /** SPR 14204 Fix : End **/
           ueScheduledInfo_p->transMode = dlMIMOInfo_p->
             transmissionMode;
           /* + SPS_TDD_Changes */
           if( SPS_RETX == dlStrategyTxNode->msgType )
           {
               ueScheduledInfo_p->isSPSCCE[subFrameNum] = LTE_TRUE;
           }
           /* - SPS_TDD_Changes */
           if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
           {
               ueScheduledInfo_p->pcellHarqProcessId = dlStrategyTxNode->harqProcessId;
           }    
            /* SS_S2 */ 
#endif /* TDD_CONFIG */
           
            /*Decerementing the tempAvailableDLRBCount by the nummber of 
              required RB for UE*/
            tempAvailableDLRBCount -= dlStrategyTxNode->resourceAllocInput.
                requiredRB;
            /* 5492 changes start */
            (*maxUeScheduledInDownlink_p)--;
            /* 5492 changes end */
#ifdef FDD_CONFIG                
            *tempPdcchDLUEArrayInfo_p = ueIndex;            
            arrayIndexDLUE++;
#endif            
            tempPdcchDLUEArrayInfo_p++;

            /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
            /* check if UE is HD FDD and schedule ReTx*/
            if(dlUEContext_p->isHDFddFlag )
            {
                /* SPR 15909 fix start */
                tickType_t localCurrTick = getCurrentTick() + PHY_DL_DELAY;
                /* SPR 15909 fix end */
                SCHEDULE_DL_FOR_HD_UE(localCurrTick, dlUEContext_p->ueIndex,
                hdUeContextInfo_g.hdfddUeSchedMap,internalCellIndex );
            }
#endif
            /* HD FDD Changes End */

            if ( !*availableCCE_p || (tempAvailableDLRBCount <= 0) ) 
            {
                LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                        "Number of CCE Becomes Zero or tempAvailableDLRBCount \
                        is Zero \n");
                LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                        "tempAvailableDLRBCount = %u ,availableCCE_p =%u\n",
                        tempAvailableDLRBCount, *availableCCE_p);
                *availableDLRBCount_p = 0;
                pdcchDLUEInfo_p->count = arrayIndexDLUE;
                return MAC_FAILURE;
            }
        }     
        else
        {
            UPDATE_EXCESS_POOL_RB(dlStrategyTxNode->resourceAllocInput.
                    resourceRegionAllocated, 
                    dlStrategyTxNode->resourceAllocInput.requiredRB,
                    internalCellIndex);
            if (CE_REGION == 
                    dlStrategyTxNode->resourceAllocInput.resourceRegionAllocated)
            {
                excessPoolRb_g[internalCellIndex][CC_CE_REGION] += 
                    dlStrategyTxNode->resourceAllocInput.requiredRB;
            }
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "PDCCH Allocation is Failed for UE = %u",ueIndex);
            putEntryInRetransmissionFailureQueue(dlStrategyTxNode,internalCellIndex);
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Coming Out from While Loop \n");
    *availableDLRBCount_p =  tempAvailableDLRBCount;
    pdcchDLUEInfo_p->count = arrayIndexDLUE;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Leaving function = %s\n",
            __func__);
    return MAC_SUCCESS;
}

/********************************************************************************************
 * Function Name  : allocatePDCCHDLNewTx               
 * Inputs         : subFrameNum - This holds the subFrame Number
 *                  maxUeScheduledInDownlink_p - Maximum Number of UE's to be scheduled in DL
 *                  dciCCEContainerInfo_p -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : availableDLRBCount_p - This holds the number of RBs in Downlink.Once 
 *                      CCEs are allocated for a UE,this would be decremented by the 
 *                      number of required RB for that UE.
 *                  availableCCE_p - This represent the number of available CCE
 *                      and if CCE's are allocated this would be deceremented by
 *                      the number of CCEs' allocated .
 *                  dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                      Information is there and once cce is allocated ,updated 
 *                      information is written on the reserved CCE.
 *                  pdcchDLOutputInfo_p: This is pointer to structure which holds 
 *                      the information of those UE for which PDCCH region has been 
 *                      allocated and for which PDSCH region need to be allocated.
 *                      Once CCE are allocated , allocatePDCCHDLReTx funtions writes 
 *                      relevant information to the structure which will be used by 
 *                      PDSCH.
 *                  directIndexingUEInfo_p : This stores pointer to    DirectIndexingUEInfo
 *                      structure
 *                  pdcchDLUEInfo_p -This holds pointer to PdcchDLOutputInfo structure.
 *Returns         : None
 *Description     : This funtions process the DL New Transmission Queue and call
 *                  funtion allocatePDCCHForDLNewReTx which will return status if 
 *                  the CCEs  are allocated or not.If return status is Success then it 
 *                  will update the pdcchDLOutputInfo_p and pdcchULOutputInfo_p  
 *                  structure with relevant information.
 **********************************************************************************************/
/** SPR 13599 Changes Start **/
#ifdef FDD_CONFIG
STATIC  MacRetType allocatePDCCHDLNewTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DirectIndexingUEInfo *directIndexingUEInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *maxUeScheduledInDownlink_p,
        InternalCellIndex internalCellIndex)
#elif TDD_CONFIG
STATIC  MacRetType allocatePDCCHDLNewTx(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32  *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchDLUEInfo *pdcchDLUEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        )
#endif
/** SPR 13599 Changes End **/
{
    UInt32 countDLUENewTx =0;
    UInt16 ueIndex = 0; 
    UInt32 arrayIndextDlMsgTypeMsg = 0;   
    SInt32 tempAvailableDLRBCount= 0;
    DLStrategyTxNode *dlStrategyTxNode = PNULL; 
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    DLMsgTypeMsgInfo *dlMsgTypeMsgInfo_p = PNULL;      
    UInt32 allocatedCCEIndex = 0;
    UInt32 arrayIndexDLUE = 0;
    UInt16 *tempPdcchDLUEArrayInfo_p = PNULL;
    LP_MacDlSpsStatus macDlSpsStatus_p = PNULL;
    UESpecificSearchSpaceInfo  *ueSearchSpaceInfo_p = PNULL;
/* SPR 609 changes start
 * dlDataMsgInfo_p is used in FDD_CONFIG
 * */
#ifdef FDD_CONFIG
        DLDataMsgInfo *dlDataMsgInfo_p = PNULL;
#endif
/* SPR 609 changes end */
#ifdef TDD_CONFIG
    UeScheduledInfo *ueScheduledInfo_p = PNULL;
#endif
    /* ICIC changes start */
    UInt8 userLocationType =  CC_USER;
    /* ICIC changes end */
    DLStrategyTxNode** ueDlStrategyTxNode_p = PNULL;
    /* + SPR 11491 Changes */
    UInt8 *isPdcchAllocatedFlag_p = PNULL;
    /* + SPR 11491 Changes */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);  
    DLMIMOInfo  *dlMIMOInfo_p=PNULL;

    /*Getting  the Array Index in dlMsgTypeMsgInfo array of pdcchDLOutputInfo_p
      structure from where if CCE are allocated , allocatePDCCHForDL funtion can
      write relavant information */

    arrayIndexDLUE = pdcchDLUEInfo_p->count;
    tempPdcchDLUEArrayInfo_p = &pdcchDLUEInfo_p->pdcchDLUEArray[arrayIndexDLUE];

    /*Getting the count of DownLink UE For which New Transmission is requested
      and for which PDCCH Region need to be allocated */
    countDLUENewTx  = sQueueCount(dlStrategyNewTxQueue_gp[internalCellIndex] );
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Number of NewTX Node  = %u\n",countDLUENewTx);  
    tempAvailableDLRBCount = *availableDLRBCount_p;
    /*Below while loop is working on the dlStrategyNewTxQueue_g queue and for
      each node popped,it get the UE and try to allocate the PDCCH region to
      it.It will iterate  through while loop until both of the condition  is 
      meet
      1) Number of RB allocated is less than max number of RB in downlink
      2) Number of DL UE for which PDCCH need to be allocated 
      is allocated is greater than 0*/
    while ( countDLUENewTx-- )
    {
        dlStrategyTxNode = (DLStrategyTxNode*)
            popNode( dlStrategyNewTxQueue_gp[internalCellIndex]);        
        if ( PNULL ==  dlStrategyTxNode )
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "sQueueCount(dlStrategyNewTxQueue_gp[internalCellIndex] ) = %u\n",
                    sQueueCount(&dlStrategyNewTxQueue_g));
            continue;
        }
        ueIndex =  dlStrategyTxNode->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        /* SPR# 9910 fix start */
        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /* SPR# 9910 fix end */
        if ( dlUEContextInfo_p->pendingDeleteFlag )
        {
            freeMemPool(dlStrategyTxNode);
            /* SPR# 9910 fix start */
            dlUEContext_p->dlStrategyTxNode_p = PNULL;
            /* SPR# 9910 fix end */
            continue;
        }
        LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                "UE =%u  is in Processing \n",ueIndex);      

        /* dl Qos Changes */
        /* reset the flag for New Transmission */
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            dlUEContext_p->scheduleFlag = DL_QOS_NEW_TRANSMISSION;
            ueDlStrategyTxNode_p = &(dlUEContext_p->dlStrategyTxNode_p);
            /* + SPR 11491 Changes */
            isPdcchAllocatedFlag_p = &(dlUEContext_p->isPdcchAllocatedFlag);            
            /* - SPR 11491 Changes */
        }
        else
        {
            dlUEContext_p->dlUeScellContext_p[1]->scheduleFlag = DL_QOS_NEW_TRANSMISSION; 
            ueDlStrategyTxNode_p = &(dlUEContext_p->dlUeScellContext_p[1]->dlStrategyTxNode_p);
            /* + SPR 11491 Changes */
            isPdcchAllocatedFlag_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag);
            /* - SPR 11491 Changes */
            /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
            dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame = subFrameNum;
            dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSchInfo[subFrameNum] = SINGLE_TB;
#endif
            /** SPR 14277 Changes End **/
        }

        dlMIMOInfo_p=getdlMIMOInfo(dlUEContext_p,internalCellIndex);
        if(dlMIMOInfo_p == PNULL )
        {
            return MAC_FAILURE;
        }

        /*Checking if Pdcch has been allocated earlier or not .If allocated 
          then skip allocating CCE for this UE .If not allocated then set the 
          flag isPdcchAllocated in UE Context  to true */
        /* SPR 2527 Changes start */
        /* + SPR 11491 Changes */
	/* SPR 22252 fix start */
        userLocationType = getUserLocationType(dlUEContext_p,internalCellIndex);
        if (*isPdcchAllocatedFlag_p)
        {
            cleanUpHarqAndStrategyNode(ueIndex,dlUEContext_p,dlStrategyTxNode,isPdcchAllocatedFlag_p,internalCellIndex);
            *ueDlStrategyTxNode_p = PNULL;
            continue;
        }
        /* - SPR 11491 Changes */
	if(((userLocationType == CE_USER)&&
                    (pdcchDLOutputInfo_p->countMsg[CE_USER] ==
                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     initParams_p->dlSchdConfig.maxCellEdgeUEsToBeScheduledDL))&&(((pdcchDLOutputInfo_p->countMsg[CC_USER] +
                             pdcchDLOutputInfo_p->countMsg[CE_USER]) <
                         cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         initParams_p->dlSchdConfig.maxUEsToBeScheduledDL + numDlSpsUeInTti_g[internalCellIndex])))
        {
            cleanUpHarqAndStrategyNode(ueIndex,dlUEContext_p,dlStrategyTxNode,isPdcchAllocatedFlag_p,internalCellIndex);
            *ueDlStrategyTxNode_p = PNULL;
            continue;
        }
	else if((pdcchDLOutputInfo_p->countMsg[CC_USER] + 
                    pdcchDLOutputInfo_p->countMsg[CE_USER]) >= 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->dlSchdConfig.maxUEsToBeScheduledDL + numDlSpsUeInTti_g[internalCellIndex])
            /* SPR 2527 Changes end */
        {
            cleanUpHarqAndStrategyNode(ueIndex,dlUEContext_p,dlStrategyTxNode,isPdcchAllocatedFlag_p,internalCellIndex);
            *ueDlStrategyTxNode_p = PNULL;
            return MAC_FAILURE;
        }
        /* ICIC changes start */
        /* SPR 22252 fix end */
	if( SPS_SCHEDULING == dlUEContext_p->schType )
	{
		userLocationType = CC_USER;
	}

        arrayIndextDlMsgTypeMsg = pdcchDLOutputInfo_p->countMsg[userLocationType];
        /* Fetching Respectuve structure based on UE location Type */
        dlMsgTypeMsgInfo_p = &pdcchDLOutputInfo_p->
            dlMsgTypeMsgInfo[userLocationType][arrayIndextDlMsgTypeMsg];
        /* ICIC changes end */
        /*Calling funtion allocatePDCCHForDLNewReAdvanceTx which will allocate
          the CCE to UE */
        /* SPS_CHG start */
#ifdef FDD_CONFIG
        directIndexingUEInfo_p[ueIndex].isSPSCCE = LTE_FALSE;
        /* + SPS_TDD_Changes */
#else
        ulUECtxInfoArr_g[ueIndex].ulUEContext_p->\
            dlUeScheduledInfoPointerArray[internalCellIndex][subFrameNum]->\
            isSPSCCE[subFrameNum] = LTE_FALSE;
#endif
        /* - SPS_TDD_Changes */
        if( SPS_SCHEDULING == dlUEContext_p->schType )
        /* SPR 6690 fix Start */
        {
        /* ++ SPR 21406 */
#if TDD_CONFIG        
        /* --  SPR 21406 */
            ueSearchSpaceInfo_p =  &dlUEContext_p->spsUeSearchSpaceInfo_p[subFrameNum];
#else              
            ueSearchSpaceInfo_p=getDLUEContextSearchSpaceInfo(dlUEContext_p,subFrameNum,internalCellIndex);
            if(!ueSearchSpaceInfo_p)
            {
                continue;
            }
#endif            
              /* + SPS_TDD_Changes */
              (*maxUeScheduledInDownlink_p)++;
              /* - SPS_TDD_Changes */
        }
        else
        {
            /*pdcch changes start*/
            ueSearchSpaceInfo_p=getDLUEContextSearchSpaceInfo(dlUEContext_p,subFrameNum,internalCellIndex);
            if(!ueSearchSpaceInfo_p)
            {
                continue;
            }
            /*pdcch changes end*/
        }
        /* SPR 6690 fix End */
        /* SPS_CHG end */
        /* SPR 6690 fix Start */
        if ( ( (dlStrategyTxNode->msgType == SPS_OCCASION) || 
                ( *availableCCE_p && (tempAvailableDLRBCount > 0) && (*maxUeScheduledInDownlink_p))) && 
                ( MAC_SUCCESS == allocatePDCCHForDLNewReAdvanceTx(
                    dlStrategyTxNode,
                    &allocatedCCEIndex,
                    dlMsgTypeMsgInfo_p,
                    ueSearchSpaceInfo_p,
                    availableCCE_p,
                    dciCCEInfo_p,
					dlUEContext_p,
					dciCCEContainerInfo_p,
					internalCellIndex)) ) 
			/* SPR 6690 fix End */
        {
            UInt32 cceIndex = 0;
            cceIndex = allocatedCCEIndex;
            dciCCEContainerInfo_p->arrayCCEAllocated
                [dciCCEContainerInfo_p->countOfCCEAllocated] = cceIndex;
            dciCCEContainerInfo_p->countOfCCEAllocated++;
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "cceIndex = %u",cceIndex);
            /*If the DL UE has got Aggregation Level One ,then UL UE cannot be 
             *Scheduled on same Aggregation level ,So mark isPdcchAllocated to
             *one*/
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "aggregationLvl = %u",dciCCEInfo_p[cceIndex].
                    dciPduInfo.aggregationLvl); 
            /* + SPR 11491 Changes */
            *isPdcchAllocatedFlag_p = 1;
            /* - SPR 11491 Changes */
#ifdef FDD_CONFIG

            directIndexingUEInfo_p[ueIndex].dlCCEIndex = cceIndex;
            /*SPS_CHG*/
            if( SPS_SCHEDULING == dlUEContext_p->schType )
              directIndexingUEInfo_p[ueIndex].isSPSCCE = LTE_TRUE;
            /*SPS_CHG*/
           /*DCI Format Updated in the directIndexingUEInfo_p*/
            dlDataMsgInfo_p = &dlMsgTypeMsgInfo_p->dlMsgInfo.dlDataMsgInfo;  
            directIndexingUEInfo_p[ueIndex].dlDciFormat = 
                dlDataMsgInfo_p->dciFormat;
            /* SS_S2 */
            directIndexingUEInfo_p[ueIndex].transMode = dlMIMOInfo_p->
                transmissionMode;
            /* SS_S2 */
            /* + CQI_4.1 */
            directIndexingUEInfo_p[ueIndex].ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;         
/* SPR 14877 SRS Changes */
            directIndexingUEInfo_p[ueIndex].ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
/* SPR 14877 SRS Changes */
            /* - CQI_4.1 */
            
#elif TDD_CONFIG
            /** SPR 14204 Fix : Start **/

            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
            if(setCCEIndexInULContex(ueIndex, subFrameNum,cceIndex,internalCellIndex,
                dlStrategyTxNode->harqProcess_p->isTBOneValid,
                dlStrategyTxNode->harqProcess_p->isTBTwoValid))

                /* SPR 19679: TDD HARQ Multiplexing  Changes End */
            /** SPR 14204 Fix : End **/
            {
                *tempPdcchDLUEArrayInfo_p = ueIndex;
                arrayIndexDLUE++;
                pdcchDLUEInfo_p->pdcchDLUEArrayForDlGroup[pdcchDLUEInfo_p->cumulativeUeCountForDlGroup] = ueIndex;
                pdcchDLUEInfo_p->cumulativeUeCountForDlGroup++;

          }
            /* SS_S2 */
            /** SPR 14204 Fix : Start **/
            ueScheduledInfo_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->
                dlUeScheduledInfoPointerArray[internalCellIndex][subFrameNum];
            /** SPR 14204 Fix : End **/
           ueScheduledInfo_p->transMode = dlMIMOInfo_p->
                                                        transmissionMode;
           
           /* + SPS_TDD_Changes */
            if( SPS_SCHEDULING == dlUEContext_p->schType )
            {
              ueScheduledInfo_p->isSPSCCE[subFrameNum] = LTE_TRUE;
            }
           /* - SPS_TDD_Changes */
           if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
           {
               ueScheduledInfo_p->pcellHarqProcessId = dlStrategyTxNode->harqProcessId;
           }    
           /* SS_S2 */ 
#endif /* TDD_CONFIG */
           /* ICIC changes start */
            pdcchDLOutputInfo_p->countMsg[userLocationType]++;
           /* ICIC changes end */
            /*Decerementing the tempAvailableDLRBCount by the nummber of 
              required RB for UE*/
            tempAvailableDLRBCount -= dlStrategyTxNode->resourceAllocInput.
                requiredRB;
            /* 5492 changes start */
            (*maxUeScheduledInDownlink_p)--;
            /* 5492 changes end */
#ifdef FDD_CONFIG
            *tempPdcchDLUEArrayInfo_p = ueIndex;            
            arrayIndexDLUE++;
#endif            
            tempPdcchDLUEArrayInfo_p++;
            /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
            /* check if UE is HD FDD and schedule New Tx*/
            if(dlUEContext_p->isHDFddFlag )
            {
                /* SPR 15909 fix start */
                tickType_t localCurrTick = getCurrentTick()  + PHY_DL_DELAY;
                /* SPR 15909 fix end */
                SCHEDULE_DL_FOR_HD_UE(localCurrTick, dlUEContext_p->ueIndex, 
                hdUeContextInfo_g.hdfddUeSchedMap,internalCellIndex );
            }
            /* HD FDD Changes End */
#endif

        /* SPR 6690 fix Start */
        /* SPR 6690 fix End */
            /* SPR 4746 changes */
        }     
        else
        {
            /* SPR 6690 fix Start */
            /*coverity_530_32862*/
            if(( SPS_SCHEDULING == dlUEContext_p->schType) &&(*maxUeScheduledInDownlink_p) )
            /*coverity_530_32862*/
            {
            /* + SPS_TDD_Changes */
                (*maxUeScheduledInDownlink_p)--;
            /* - SPS_TDD_Changes */
            }
            /*SPR 6690 fix End */
            /* ICIC changes start */
            UPDATE_EXCESS_POOL_RB(dlStrategyTxNode->resourceAllocInput.
                    resourceRegionAllocated, 
                    dlStrategyTxNode->resourceAllocInput.requiredRB,
                    internalCellIndex);
            if (CE_REGION == 
                    dlStrategyTxNode->resourceAllocInput.resourceRegionAllocated)
            {
                excessPoolRb_g[internalCellIndex][CC_CE_REGION] += 
                    dlStrategyTxNode->resourceAllocInput.requiredRB;
            }
            /* ICIC changes end */
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "PDCCH Allocation is Failed for UE = %u",ueIndex);
            DLHARQProcess  *harqProcess_p = PNULL;
            /* + SPR 11469 Changes */
            if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
            {
                harqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess
                    [dlStrategyTxNode->harqProcessId];
            }
            else
            {
                harqProcess_p = &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
                    ScelldlHarqContext_p->harqProcess[dlStrategyTxNode->harqProcessId];
            }
            /* - SPR 11469 Changes */

            /*SPS_CHG*/
            if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     initParams_p->dlSchdConfig.dlSchedulerStrategy ||
                dlUEContext_p->schType == SPS_SCHEDULING)
            {
                if( SPS_DEACTIVATION == dlStrategyTxNode->msgType )
                {
                    GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                            sizeof(MacDlSpsStatus), PNULL);
                    /* coverity-530 CID-25228*/
                    if(PNULL == macDlSpsStatus_p)
                    {
                        freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);
                        freeMemPool(dlStrategyTxNode);
                        *ueDlStrategyTxNode_p = PNULL;
                        continue;
                    }
                    /* coverity-530 CID-25228*/
                    macDlSpsStatus_p->ueIndex = dlUEContext_p->ueIndex;
                    macDlSpsStatus_p->response = PDCCH_CCE_ALLOC_FAILURE;
                    /* + Coverity Id - 32595 */
                    if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                            macDlSpsStatus_p))
                    {
                        freeMemPool(macDlSpsStatus_p);
                        macDlSpsStatus_p = PNULL;
                    }
                    /* - Coverity Id - 32595 */
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                            QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ),
                            internalCellIndex);
                    /* CA Stats Changes End */
                }
                /*Cyclomatic Complexity changes - starts here*/
                /* SPR 9546 Fix Start */
                putEntryInPdcchPdschFailureQueueForTaDrxMsgForQos(harqProcess_p,
                        ueIndex,
                        dlUEContext_p->timingAdvanceValue,
                        internalCellIndex);
                /* SPR 9546 Fix End */
            }

            /* SPR 1805 Fix Begin*/
            if ((DL_QOS_BASED_SCHEDULING != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                         initParams_p->dlSchdConfig.dlSchedulerStrategy) && 
                    ( (harqProcess_p->dlHARQTBOneInfo.taDrxFlag != INVALID_TA_DRX_MSG )
                || (harqProcess_p->dlHARQTBTwoInfo.taDrxFlag != INVALID_TA_DRX_MSG)))
            {
                    /* +coverity-530-CID 24492*/
                if(MAC_FAILURE == putEntryInAdvanceAllocInfoQueue(dlStrategyTxNode,internalCellIndex))
                {
                    freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);
                    freeMemPool(dlStrategyTxNode);
                    *ueDlStrategyTxNode_p = PNULL;
                    continue;
                }
                /* +coverity-530-CID 24492*/
            }
            else
            {
                freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);
                freeMemPool(dlStrategyTxNode);
                /* SPR# 4906 Changes Starts   */
                *ueDlStrategyTxNode_p = PNULL;
                /* SPR# 4906 Changes Ends   */

                /* SPR 990 */
                /*SPR 2453 Start*/
                continue;
                /*SPR 2453 End*/
            }
            /* SPR 1805 Fix End*/
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Coming Out from While Loop \n");
    *availableDLRBCount_p =  tempAvailableDLRBCount;
    pdcchDLUEInfo_p->count = arrayIndexDLUE;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Leaving function = %s\n",
            __func__);
    return MAC_SUCCESS;
}
/*Cyclomatic Complexity changes - starts here*/
/******************************************************************************
 * Function Name  :  putEntryInPdcchPdschFailureQueueForTaDrxMsgForQos 
 * Inputs         : harqProcess_p - Pointer to Harq process info,ueIndex
                    timingAdvanceValue -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : This func will in TA or DRX failure when there is 
 *                  PDCCH allocation failure of TA or DRX msg   
 *******************************************************************************/
STATIC  void putEntryInPdcchPdschFailureQueueForTaDrxMsgForQos(
        DLHARQProcess *harqProcess_p,
        UInt16 ueIndex,UInt16 timingAdvanceValue,
        InternalCellIndex internalCellIndex)
{
    switch(harqProcess_p->dlHARQTBOneInfo.taDrxFlag)
    {
        case TA_MSG:
        /* + SPR 22243 Fix*/
        case TA_DRX_MSG: 
        /* - SPR 22243 Fix*/
        /* SPR 9546 Fix Start */
        putEntryInPdcchPdschFailureTAQueue(ueIndex,
                timingAdvanceValue,
                internalCellIndex);
        /* SPR 9546 Fix End */
            break;
            /* + SPR 22243 Fix*/
        case DRX_MSG:
                putEntryInPdcchPdschFailureDRXQueue(ueIndex,internalCellIndex);
            break;
            /* - SPR 22243 Fix*/
        default:
            /* No TA_DRX Confiured. Hence do nothing */
            break;
     }
}
/* + SPR 22243 Fix*/
/*****************************************************************************
 * Function Name  : putEntryInPdcchPdschFailureDRXQueue
 * Inputs         : ueIndex - Index of UE and
 *                  internalCellIndex - cell id  
 * Outputs        : Puts the DRXQueueNode in pdcchPdschFailureDRXQueue.
 * Returns        : None
 * Description    : This function creates a new DRXQueue Node and inserts 
 *                  this node in the pdcchPdschFailureDRXQueue_gp to be processed
 *                  by Strategy.
 *****************************************************************************/
void putEntryInPdcchPdschFailureDRXQueue( UInt32 ueIndex, 
                                         InternalCellIndex internalCellIndex)
{
    DRXQueueNode *drxQueueNode_p = PNULL;        
    GET_MEM_NODE_DRX_Q(drxQueueNode_p,DRXQueueNode);
    if (PNULL != drxQueueNode_p)
    {
        drxQueueNode_p->ueIdx = ueIndex;
        if (!(ENQUEUE_DRX_Q(pdcchPdschFailureDRXQueue_gp[internalCellIndex],DRXQueueNode,&drxQueueNode_p->dRXQueueNodeAnchor)))
        {
            FREE_MEM_NODE_DRX_Q(drxQueueNode_p);
        }
    }
    else
    {
        /* Not able to allocate Memory */
    }
}
/* - SPR 22243 Fix*/

/*Cyclomatic Complexity changes - ends here*/
/******************************************************************************
 * Function Name  : allocatePDCCHForUL
 * Inputs         : txNode_p - This is pointer to ULStrategyTxNode structur which 
 *                      containe information of those UE for which Uplink Grant need to be
 *                      sent and for which PDCCH Region need to be allocated            
 *                  ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo
 *                      structure which holds the CCEs Index for a UE at each of the
 *                      aggregation level. 
 *                  ulUEContext_p -
 *                  dciCCEContainerInfo_p -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : pdcchULOutputInfo_p : This is pointer to PdcchULOutputInfo structure
 *                  on, which once cce are allocated for this UE,relevant infomation can be
 *                  written.
 *                  availableCCE_p : This represent the number of available CCE
 *                      and if CCE's are allocated this would be deceremented by
 *                      one .
 *                  dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                      Information is there and once cce is allocated ,updated 
 *                      information is written on the reserved CCE
 * Returns        : MacRetType
 * Description    : This funtions will call the funtion 
 *                  allocatePDCCHForDCIFormatUESpacethat will check for CCEs 
 *                  Availability and If Funtion return  Success ,it will update the
 *                  pdcchULOutputInfo_p structure.It will also update pdcchULMUMIMOCandidateInfo_p
 *                  structure if UL MU MIMO is successful.              
 ******************************************************************************/
#ifdef FDD_CONFIG 
STATIC MacRetType  allocatePDCCHForUL(
        ULStrategyTxNode  *txNode_p,
        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        DciCCEInfo *dciCCEInfo_p,
        UInt32 *availableCCE,
        ULUEContext * ulUEContext_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        )
{
    UInt32 allocatedCCEIndex = 0;
    UInt32 countULNewUE = 0;    
    DirectIndexingUEInfo *directIndexingUEInfo_p = PNULL;
    PdcchULNewUEInfo *pdcchULNewUEInfo_p = PNULL;
    UInt16 ueIndex  = 0;
    /* UL_MU_MIMO_CHG_START */
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
    PdcchULMUMIMOCandidateInfo *pdcchULMUMIMOCandidateInfo_p = PNULL;
    UInt32 countULMUMIMO = 0;
    UInt32 rbsForMaxTBSize = 0;
    UInt8 modScheme = 0;
    UInt8 retVal = FALSE;
    /* UL_MU_MIMO_CHG_END */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);  
    ueIndex = txNode_p->ueIndex;
    /* SPS_CHG_START */
    /*! \fn static MacRetType allocatePDCCHForUL(
     *                   ULStrategyTxNode  *txNode_p,
     *                   UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
     *                   PdcchULOutputInfo *pdcchULOutputInfo_p,
     *                   DciCCEInfo *dciCCEInfo_p,
     *                   UInt32 *availableCCE,
     *                   UeScheduledInfo *ueScheduledInfo_p,
     *                   ULUEContext * ulUEContext_p)
     * \brief Function Modification
     * \code
     * - Check If Message type is SPS_OCCASION
     *       - Allocate the Cce Index above the MAX_CCE
     *       - Increment the spsCceIndex_g
     *       - Populating the pdcchULNewUEArray structure which 
     *         will be used by PUSCH for allocating the RBs
     * \endcode
     * */
 /* TTIB_Code Start */
    if (UL_SPS_OCCASION == txNode_p->ulGrantRequestType 
        ||(TTIB_UL_SPS_OCCASION == txNode_p->ulGrantRequestType))
 /* TTIB_Code End */
    {

        allocatedCCEIndex = MAX_CCE + spsCceIndex_g[internalCellIndex];
        spsCceIndex_g[internalCellIndex]++;
        if (spsCceIndex_g[internalCellIndex] == MAX_SPS_CCE)
        {
            spsCceIndex_g[internalCellIndex] = 0;
        }
        pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo[0];
        countULNewUE = pdcchULNewUEInfo_p->count;        
        directIndexingUEInfo_p = &pdcchULOutputInfo_p->
            directIndexingUEInfo[ueIndex];

        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].ueIndex = ueIndex;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS = 
                                                         txNode_p->avgMcs;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].minDataSize = 
                                                         txNode_p->minDataSize;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize = 

        directIndexingUEInfo_p->ulCCEIndex = allocatedCCEIndex;
        directIndexingUEInfo_p->strictRBAllocation= txNode_p->strictAllocRB;

        directIndexingUEInfo_p->requiredRB= 
                         getUplinkMaxRBsInPowerOfTwoThreeFive(txNode_p->requiredRB);  

        directIndexingUEInfo_p->ulGrantRequestType = txNode_p->ulGrantRequestType;
        directIndexingUEInfo_p->ttiCounter= txNode_p->ttiCounter;
        directIndexingUEInfo_p->uePosInPdcchULNewUEArray = countULNewUE;
        /* + CQI_4.1 */
        directIndexingUEInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;         
/* SPR 14877 SRS Changes */
            directIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
/* SPR 14877 SRS Changes */
        /* - CQI_4.1 */
        /* UL_MU_MIMO_CHG_START */
        directIndexingUEInfo_p->buddyUeIndex = INVALID_UE_INDEX;
        /* UL_MU_MIMO_CHG_END */
        countULNewUE++;
        pdcchULNewUEInfo_p->count = countULNewUE;

        dciCCEInfo_p[allocatedCCEIndex].msgType = SPS_OCCASION;
        dciCCEInfo_p[allocatedCCEIndex].isCCEFree = CCE_ALLOCATED;
        return MAC_SUCCESS;
    }
    /* SPS_CHG_END */
    /*Calling funtion allocatePDCCHForDLDCIFormat to allocate the CCE to a UE 
      whose UpLink Grant Need to be send based on DCI Format 0 */
    /*klockwork warning fix*/
    if (MAC_SUCCESS == allocatePDCCHForULNewReTx(
                ueSearchSpaceInfo_p,
                &allocatedCCEIndex,
                availableCCE,
                dciCCEInfo_p,
                ulUEContext_p,
                txNode_p, //CA_phase2_csi_code
                internalCellIndex) && ulUEContext_p->userLocationType < MAX_USER_LOCATION)
    {       
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "Allocation of CCE For UE = %d is Successful\n",ueIndex);    
        /* ICIC changes start */
        pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType];
        /* ICIC changes end */
        countULNewUE = pdcchULNewUEInfo_p->count;        
        directIndexingUEInfo_p = &pdcchULOutputInfo_p->
            directIndexingUEInfo[ueIndex];

        /*Populating the pdcchULNewUEArray structure which will be used by 
          PUCCH for reserving the Resources for DL ACK Nack in advance for the
          DL UE for which PDCCH region has been allocated */     
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].ueIndex = ueIndex;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS = 
                                                         txNode_p->avgMcs;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].minDataSize = 
                                                         txNode_p->minDataSize;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize = 
                                                         txNode_p->maxDataSize;

        /*Poplating the Information in directIndexingUEInfo_p structure which
          will be used by PUCCH and PUSCH .As this is direct index array where 
          index is mapped to ueIndex
          1)PUCCH maintain a list of UE for which the periodic report is 
          expected in the subframe ,.PUCCH will check if the UE ,whose report
          is expected,has been allocated CCE Region by using the 
          directIndexingUEInfo structure ,If allocated ,it will not reserve 
          the uplink resources  for this UE for periodic reports.IF not 
          allocated then it will Reserve the  resources for periodic reports.
          2)PUSCH will use this structure for getting information as 
          requiredRB,cceIndex,strictRBFlag for it's processing*/

        directIndexingUEInfo_p->ulCCEIndex = allocatedCCEIndex;
        /* + SPR 5551 */
        dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.cqiRequest = 0;
        /* - SPR 5551 */

        directIndexingUEInfo_p->strictRBAllocation= txNode_p->strictAllocRB;

        directIndexingUEInfo_p->requiredRB= 
                         getUplinkMaxRBsInPowerOfTwoThreeFive(txNode_p->requiredRB);  

        directIndexingUEInfo_p->ulGrantRequestType = txNode_p->ulGrantRequestType;
        directIndexingUEInfo_p->ttiCounter= txNode_p->ttiCounter;
        directIndexingUEInfo_p->uePosInPdcchULNewUEArray = countULNewUE;
        /* SPR 12013 fix start */
        directIndexingUEInfo_p->transMode = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->
            dlMIMOInfo.transmissionMode;
        /* SPR 12013 fix end */
        /* + CQI_4.1 */
        directIndexingUEInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;         
        /* SPR 14877 SRS Changes */
        directIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
        /* SPR 14877 SRS Changes */
        /* - CQI_4.1 */

        /* UL_MU_MIMO_CHG_START */
        directIndexingUEInfo_p->buddyUeIndex = INVALID_UE_INDEX;
        modScheme = ulUEContext_p->modulationScheme;
        MAC_GET_MOD_TYPE(modScheme);
        /*klockworks warning fix*/
        if(modScheme > 2)
        {
            pdcchULMUMIMOCandidateInfo_p = &pdcchULOutputInfo_p->pdcchULMUMIMOCandidateInfo[ulUEContext_p->userLocationType][modScheme];
            countULMUMIMO = pdcchULMUMIMOCandidateInfo_p->count;
        }
        ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

        /* Checking if MU MIMO is to be done or not.
         * If all the criteria for MU MIMO are met then add the ueIndex in
         * PdcchULMUMIMOCandidateInfo[userLocation][modulationType]list and reference Index in UEContext. */

        UL_MU_MIMO_CANDIDATE_SELECTION(ulUEContextInfo_p,ulUEContext_p, pdcchULNewUEInfo_p,countULNewUE,directIndexingUEInfo_p,retVal,internalCellIndex);
        if (retVal && (pdcchULMUMIMOCandidateInfo_p != PNULL))                                       
        {
            pdcchULMUMIMOCandidateInfo_p->pdcchULMUMIMOCandidateUE[countULMUMIMO].ueIndex = ueIndex;
            countULMUMIMO++;
            pdcchULMUMIMOCandidateInfo_p->count = countULMUMIMO;
            /*SPR_5875 Start*/
            /* SPR 5999 changes start */
            rbsForMaxTBSize = DETERMINE_RBs_UL(pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS,
                    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize,
                    internalCellIndex);
            /* SPR 5999 changes end */
            rbsForMaxTBSize = getUplinkMaxRBsInPowerOfTwoThreeFive(rbsForMaxTBSize);
            aggregateMaxRBsRequired_g[internalCellIndex] += rbsForMaxTBSize;
            /*SPR_5875 End*/
            /* The UE is selected as a candidate UE */ 
            LOG_MAC_MSG(MAC_CAND_SEL_UE_INFO_ID,LOGDEBUG,MAC_PDCCH,
                    getCurrentTick(),
                    ueIndex,
                    countULMUMIMO,aggregateMaxRBsRequired_g[internalCellIndex]
                    ,pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS,
                    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
       }
       else
       {
           if ((!ulUEContextInfo_p->pendingDeleteFlag) && 
                       (ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus))
           {
               /* The various parameter values will help deduce the reason for failure of UE as a candidate UE */\
                   LOG_MAC_MSG(MAC_CAND_FAIL_UE_INFO_ID,LOGDEBUG,MAC_PDCCH,
                           getCurrentTick(),
                           ueIndex,
                           ulUEContextInfo_p->pendingDeleteFlag,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                           pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS,
                           directIndexingUEInfo_p->ulGrantRequestType,DEFAULT_FLOAT_VALUE,
                           DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
           }
       }

       /* UL_MU_MIMO_CHG_END */

        countULNewUE++;
        pdcchULNewUEInfo_p->count = countULNewUE;
        if(txNode_p->ulGrantRequestType == UL_SPS_ACTIVATION)
        {
            dciCCEInfo_p[allocatedCCEIndex].msgType = SPS_ACTIVATION;
        }
        else if(txNode_p->ulGrantRequestType == UL_SPS_DEACTIVATION)
        {
            dciCCEInfo_p[allocatedCCEIndex].msgType = SPS_DEACTIVATION;
        }
        return MAC_SUCCESS;
    }   
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Allocation of CCE For \
            UE = %d is Failed\n",ueIndex);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Leaving function = %s\n",
            __func__);  
    return MAC_FAILURE;
}
#elif TDD_CONFIG
/******************************************************************************
 * Function Name : allocatePDCCHForUL
 * Inputs        : txNode_p - This is pointer to ULStrategyTxNode structur which 
 *                     containe information of those UE for which Uplink Grant need to be
 *                     sent and for which PDCCH Region need to be allocated            
 *                 ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo
 *                     structure which holds the CCEs Index for a UE at each of the
 *                     aggregation level.             
 *                 ueScheduledInfo_p -
 *                 ulUEContext_p -
 *                 internalCellIndex - Cell-Index at MAC
 *                 currentGlobalTickWithDelay
 *                 localPdcchInfoMap
 *                 rbMapNum
 *                 maxUeScheduledInUplink_p
 * Outputs       : pdcchULOutputInfo_p - This is pointer to PdcchULOutputInfo structure
 *                     on, which once cce are allocated for this UE,relevant infomation can be
 *                     written.
 *                 availableCCE_p - This represent the number of available CCE
 *                     and if CCE's are allocated this would be deceremented by
 *                     one .
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                     Information is there and once cce is allocated ,updated 
 *                     information is written on the reserved CCE
 * Returns      : MacRetType
 * Description  : This funtions will call the funtion 
 *                allocatePDCCHForDCIFormatUESpacethat will check for CCEs 
 *                Availability and If Funtion return  Success ,it will update the
 *                pdcchULOutputInfo_p structure.              
 *****************************************************************************/
STATIC MacRetType  allocatePDCCHForUL(
                        ULStrategyTxNode  *txNode_p,
                        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
                        PdcchULOutputInfo *pdcchULOutputInfo_p,
                        DciCCEInfo *dciCCEInfo_p,
                        UInt32 *availableCCE,
						UeScheduledInfo *ueScheduledInfo_p,
						ULUEContext * ulUEContext_p,
						InternalCellIndex internalCellIndex,
                        /* SPR 15909 fix start */
                        tickType_t currentGlobalTickWithDelay,
                        /* SPR 15909 fix end */
                        PdcchInfo* localPdcchInfoMap[],
                        UInt8 rbMapNum,
                        UInt8 *maxUeScheduledInUplink_p
        )
/* + coverity 55311 */
{
    UInt32 allocatedCCEIndex = 0;
    PdcchULNewUEInfo *pdcchULNewUEInfo_p = PNULL;

    /*TDD Config 0 Changes Start*/
    UInt8 ulSfFor2ndSf = 0;
    UeScheduledInfo *ueScheduledInfoFor2ndsf_p = PNULL;
    MacRetType retVal = MAC_SUCCESS;
    /*TDD Config 0 Changes End*/
    /* SPR 11065 Changes Start */
    UInt8 ulSfForkthSf = 0;
    PdcchInfo *pdcchInfo_p = PNULL;
    UeScheduledInfo *ueScheduledInfoForkthsf_p = PNULL;
    /* SPR 11065 Changes End */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
    /* + SPS_TDD_Changes */
    UInt32 countULNewUE = 0;
    UInt16 ueIndex = 0;
    ueIndex = txNode_p->ueIndex;

    if (UL_SPS_OCCASION == txNode_p->ulGrantRequestType) 
    {

        allocatedCCEIndex = MAX_CCE + spsCceIndex_g[internalCellIndex];
        spsCceIndex_g[internalCellIndex]++;
        if (spsCceIndex_g[internalCellIndex] == MAX_SPS_CCE)
        {
            spsCceIndex_g[internalCellIndex] = 0;
        }
        pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo[0];
        countULNewUE = pdcchULNewUEInfo_p->count;        

        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].ueIndex = ueIndex;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS = 
                                                         txNode_p->avgMcs;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].minDataSize = 
                                                         txNode_p->minDataSize;
        pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize = 

        ueScheduledInfo_p->ulCCEIndex = allocatedCCEIndex;
        ueScheduledInfo_p->strictRBAllocation= txNode_p->strictAllocRB;

        ueScheduledInfo_p->requiredRB= 
                         getUplinkMaxRBsInPowerOfTwoThreeFive(txNode_p->requiredRB);  

        ueScheduledInfo_p->ulGrantRequestType = txNode_p->ulGrantRequestType;
        ueScheduledInfo_p->ttiCounter= txNode_p->ttiCounter;
        ueScheduledInfo_p->uePosInPdcchULNewUEArray = countULNewUE;
        /* + CQI_4.1 */
        ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;         
        /* - CQI_4.1 */
        /* UL_MU_MIMO_CHG_START */
        ueScheduledInfo_p->buddyUeIndex = INVALID_UE_INDEX;
        /* UL_MU_MIMO_CHG_END */
        countULNewUE++;
        pdcchULNewUEInfo_p->count = countULNewUE;

        dciCCEInfo_p[allocatedCCEIndex].msgType = SPS_OCCASION;
        dciCCEInfo_p[allocatedCCEIndex].isCCEFree = CCE_ALLOCATED;
        dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.dciFormat = DCI_FORMAT_0;
        dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.aggregationLvl = AGGREGATION_LEVEL_ONE;
        return MAC_SUCCESS;
    }
    /* - SPS_TDD_Changes */

    /*Calling funtion allocatePDCCHForDLDCIFormat to allocate the CCE to a UE
      whose UpLink Grant Need to be send based on DCI Format 0 */
    if (MAC_FAILURE == allocatePDCCHForULNewReTx(
                ueSearchSpaceInfo_p,
                &allocatedCCEIndex,
                availableCCE,
                dciCCEInfo_p,
                ulUEContext_p,
                txNode_p, //CA_phase2_csi_code
                internalCellIndex))
    {
        if ( MAC_SUCCESS == isCCEAvailableAggrLvlFourCommonSpace(                  
                    &allocatedCCEIndex,availableCCE,dciCCEInfo_p,
                    internalCellIndex) )
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "Allocation on Aggr Lvl 4 CommonSearch Space UE Index [%d]for DCI 0\
                    is Successful\n",txNode_p->ueIndex);
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "Allocation on Aggr Lvl 4 Common Space is Failed\n");
            /*TDD Config 0 Changes Start*/
            retVal = MAC_FAILURE;
            /*TDD Config 0 Changes End*/
        }
    }
    /* + SPS_TDD_Changes */
    if(txNode_p->ulGrantRequestType == UL_SPS_ACTIVATION)
    {
        dciCCEInfo_p[allocatedCCEIndex].msgType = SPS_ACTIVATION;
    }
    else if(txNode_p->ulGrantRequestType == UL_SPS_DEACTIVATION)
    {
        dciCCEInfo_p[allocatedCCEIndex].msgType = SPS_DEACTIVATION;
    }
    /* - SPS_TDD_Changes */
    /*TDD Config 0 Changes Start*/
    /* Check whether CCE has been allocated or not */
    if (retVal != MAC_FAILURE)
    {
        /* When schedule count is 2 and the cce allocation is sucess for the 1st node, we have to 
         * update the CCE index in both scheduledInfo node for +k and +7 */
        if (txNode_p->scheduledCount == NUM_TWO) 
        {
            //1st node of Schedule Count 2
            if (INVALID_CCE_INDEX == ueScheduledInfo_p->ulCCEIndex)
            {
                ulSfFor2ndSf = (currentGlobalTickWithDelay + ULTX_OFFSET) % MAX_SUB_FRAME;
                /* Check whether ulsubframe Number is equal to ulsubframe number of 7th offset
                 * If yes, that means the node for ulsubframe number of kth offset has been dropped.
                 * Reason to drop the node could be maxUeScheduled for kth ulsubframe has
                 * been reached maximum.
                */
                if (txNode_p->ulSubFrameNum != ulSfFor2ndSf)
                {
                    if ( maxUeScheduledInUplink_p[ulSfFor2ndSf] )
                    {
                        pdcchInfo_p = localPdcchInfoMap[ulSfFor2ndSf];

                        //Update Schedule Info for +7th Subframe
                        pdcchULNewUEInfo_p = 
                            &pdcchInfo_p->pdcchULOutput_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType];
                        /** SPR 14204 Fix : Start **/
                        ueScheduledInfoFor2ndsf_p = 
                            ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSfFor2ndSf]; 
                        /** SPR 14204 Fix : End **/
                        updateUeScheduleInfo(txNode_p,ueScheduledInfoFor2ndsf_p,pdcchULNewUEInfo_p,
                                pdcchULNewUEInfo_p->count,allocatedCCEIndex,INVALID_HARQ_ID,
                                /* SPR 11065 Changes Start */
                                FALSE
                                /* SPR 11065 Changes End */
                                );
                        pdcchULNewUEInfo_p->count++;
                        maxUeScheduledInUplink_p[ulSfFor2ndSf]--;
                    }
                    /*Else case will be handled in allocatePDCCHULTxInTdd while processing the second node*/
                }
                /*No need to handle else case only update the UL output info for kth subframe*/

                //Update Schedule Info for +Kth Subframe
                pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType];
                updateUeScheduleInfo(txNode_p,ueScheduledInfo_p,pdcchULNewUEInfo_p,
                        pdcchULNewUEInfo_p->count,allocatedCCEIndex,txNode_p->ulHarqProcessId,
                        /* SPR 11065 Changes Start */
                        FALSE
                        /* SPR 11065 Changes Start */
                        );
                pdcchULNewUEInfo_p->count++;
                maxUeScheduledInUplink_p[txNode_p->ulSubFrameNum]--;
            }
            //2nd node of Schedule Count 2 
            else
            {
                ueScheduledInfo_p->ulCCEIndex         = allocatedCCEIndex;
                ueScheduledInfo_p->ulHarqProcessId    = txNode_p->ulHarqProcessId;
                ueScheduledInfo_p->strictRBAllocation = txNode_p->strictAllocRB;
            }
        }
        else if (txNode_p->scheduledCount == NUM_ONE)
        {
            pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType];
            //Update Schedule Info for +Kth Subframe
            updateUeScheduleInfo(txNode_p,ueScheduledInfo_p,pdcchULNewUEInfo_p,
                    pdcchULNewUEInfo_p->count,allocatedCCEIndex,txNode_p->ulHarqProcessId,
                    /* SPR 11065 Changes Start */
                    FALSE
                    /* SPR 11065 Changes End */
                    );
            processUlMUMIMOUe(ueScheduledInfo_p,pdcchULOutputInfo_p,ulUEContext_p,
                    txNode_p,rbMapNum, internalCellIndex);
            pdcchULNewUEInfo_p->count++;
            maxUeScheduledInUplink_p[txNode_p->ulSubFrameNum]--;
        }
        else
        {
            ltePanic("Scheduled Count : %d in Strategy Node is Invalid\n",txNode_p->scheduledCount);
        }
    }
    else
    {
        /*
         ** Update the Strategy Node info in Schedule Info node while 
         ** CCE Allocation failure for 2nd node with schedule count 2
         */
        if ( (txNode_p->scheduledCount == NUM_TWO) &&
                (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
        {
            /* SPR 11236 Changes Start */
            /*
            ** This function puts the current UE index with scheduled count 2 to top of 
            ** the uloutput Info array which will forced the PUSCH to allocate the PUSCH
            ** resource for these UEs first in 7th ulsubframe.
            **
            ** Function does the following:
            ** a) Copies the content of uloutput info array for this UE (on which currently working)
            **    into local structure.
            ** b) Then, Moves each entry to its next index starting from 0 index to next index
            **    till the index in which current UE is present. Let say, Current ueindex
            **    is present at the index 3.
            **      
            **    Ueindex present at 0 index in uloutput info array will be moved to 1.
            **    Ueindex present at 1 index in uloutput info array will be moved to 2.
            **    Ueindex present at 2 index in uloutput info array will be moved to 3.
            */
            if (ueScheduledInfo_p->uePosInPdcchULNewUEArray != NUM_ZERO)
            {
                updateULOutputInfoArray(&pdcchULOutputInfo_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType],
                                        ueScheduledInfo_p->uePosInPdcchULNewUEArray,
                                        ueScheduledInfo_p,
                                        txNode_p->ulSubFrameNum,
                                        /** SPR 14204 Fix : Start **/
                                        internalCellIndex
                                        /** SPR 14204 Fix : End **/
                        );
            }
            else
            {
                /* No need to traverse the function */
            }

            /* SPR 11236 Changes End */
            ueScheduledInfo_p->ulHarqProcessId = txNode_p->ulHarqProcessId;
            /* Schedule is set to One while CCE allocation is sucess for two strategy nodes 
             * having schedule count 2. But set the schedule count 2 for 2nd stategy  
             * node having schedule count 2*/
            pdcchULOutputInfo_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType].
                pdcchULNewUEArray[ueScheduledInfo_p->uePosInPdcchULNewUEArray].
                scheduleCount = NUM_TWO;
            /* SPR 11065 Changes Start */
            /*
             ** In Kth ulsubframe, update 7th node is present.
             ** PUSCH uses it to resetCCE in case of PUSCH resource
             ** failure.
             */
            ulSfForkthSf = txNode_p->ulSubFrameNum - NUM_ONE; 
            pdcchInfo_p = localPdcchInfoMap[ulSfForkthSf];
            pdcchULNewUEInfo_p = 
                &pdcchInfo_p->pdcchULOutput_p->pdcchULNewUEInfo[ulUEContext_p->userLocationType];
            /** SPR 14204 Fix : Start **/
            ueScheduledInfoForkthsf_p = 
                ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSfForkthSf]; 
            /** SPR 14204 Fix : End **/
            pdcchULNewUEInfo_p->pdcchULNewUEArray[ueScheduledInfoForkthsf_p->uePosInPdcchULNewUEArray].
               isNodePresentIn2UlSubframe = TRUE; 
            /* SPR 11065 Changes End */
       }

    }
    /*TDD Config 0 Changes End*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Leaving function = %s\n",
            __func__);
    return retVal;
}
/***************************************************************************************
 * Function Name : allocatePDCCHULTxInTdd
 * Inputs        :  localPdcchInfoMap
 *                  currentGlobalTickWithDelay
 *                  subFrameNum : This stores the subFrame Number on which UE 
 *                     Specific Search Space need to be found.
 *                  availableCCE_p
 *                  dciCCEInfo_p
 *                  dciCCEContainerInfo_p
 *                  maxUeScheduledInUplink_p - Maximum Number of UE's to 
 *                      be scheduled in UL
 *                  execArgs
 *                  internalCellIndex
 * Outputs       : availableCCE_p : This represent the number of available CCE
 *                     and if CCE's are allocated this would be deceremented by
 *                     the number of CCEs' allocated .
 *                 dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                     Information is there and once cce is allocated ,updated 
 *                     information is written on the reserved CCE.              
 *                 pdcchULOutputInfo_p : This is pointer to structure which 
 *                     further holds structure of type PdcchDLUEInfo in which those DL
 *                     UE Information is written whose PDCCH region has been allocated.
 *                     PUCCH  will use this for reserving the Resources for DL ACK 
 *                     Nack in advance for the DL UE for which PDCCH region has been 
 *                     allocated. 
 * Returns       : None
 * Description   : This funtion will Process those  UE  whose Uplink Grant need to
 *                 be sent and then call funtion allocatePDCCHForUL which will
 *                 allocate the CCEs for them 
 *****************************************************************************************/

STATIC void  allocatePDCCHULTxInTdd(
        PdcchInfo* localPdcchInfoMap[],
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,
        UInt32 *availableCCE_p, 
        DciCCEInfo *dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt8 *maxUeScheduledInUplink_p,
        ExecutionLegFunctionArg* execArgs
        ,InternalCellIndex internalCellIndex
        )
{
    UInt32 countULUETx                  = 0;
    UInt16 ueIndex                      = 0;       
    ULStrategyTxNode *ulStrategyTxNode  = PNULL;
    ULUEContextInfo *ulUEContextInfo_p  = PNULL;
    ULUEContext* ulUEContext_p          = PNULL;           
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p = PNULL;             
    /*TDD Config 0 changes Start*/
    UeScheduledInfo *ueScheduledInfo_p = PNULL;
    UInt8 tokenUpdateFromStr = LTE_FALSE;
    UInt8 ulSfFromStrategyNode          = 0;
    PdcchInfo *pdcchNodeInfo_p          = PNULL;
    ULHarqInfo* tempULHarqInfo_p        = PNULL;
    RbMapNode *rbMapNode_p              = PNULL;
    /*TDD Config 0 changes End*/
    /* + SPS_TDD_Changes */
    LP_MacULSpsStatus macUlSpsStatus_p = PNULL;
    /* - SPS_TDD_Changes */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n",__func__);

    /* Getting the count of Uplink UE For which UpLink Grant Message need 
       to be send and for which PDCCH Region need to be allocated */
    countULUETx = sQueueCount(ulStrategyTxQueue_gp[internalCellIndex]); 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Number of new UL UE =%u \n",countULUETx);
    /* Below while loop is working on the ulStrategyTxQueue_g queue and for
       each node popped,it get the UE and try to allocate the PDCCH region to 
       it.It will iterate  through while loop until both of the condition is 
       meet.    
       1) Number of RB allocated is less than max number of RB in uplink
       2) Number of UL UE for which PDCCH need to be allocated 
       is allocated is greater than 0*/   

    /*DYN_PDCCH_DL_UL_SPLIT_FIX*/
    while ( countULUETx--)
    {
        ulStrategyTxNode = (ULStrategyTxNode*)popNode( ulStrategyTxQueue_gp[internalCellIndex]);
        /*DYN_PDCCH_DL_UL_SPLIT_FIX*/
        if ( PNULL ==  ulStrategyTxNode )
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "countULUETx =%u \n",sQueueCount(ulStrategyTxQueue_gp[internalCellIndex]));
            continue;
        }
        ueIndex = ulStrategyTxNode->ueIndex;     
        LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                "Processing on UE = %u\n",ueIndex); 
        ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        if ( ulUEContextInfo_p->pendingDeleteFlag)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "UE Deletetion In Progress \n");
            freeMemPool(ulStrategyTxNode);
            continue;
        }
        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

        /*TDD Config 0 Changes Start*/
        ulSfFromStrategyNode = ulStrategyTxNode->ulSubFrameNum;
        /*
         ** Memory is already allocated for PdcchInfoNode in parent function.
         */
        pdcchNodeInfo_p = localPdcchInfoMap[ulSfFromStrategyNode];

        /** SPR 14204 Fix : Start **/
        ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSfFromStrategyNode];
        /** SPR 14204 Fix : End **/
        tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[ulStrategyTxNode->ulHarqProcessId]);
        rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + pdcchNodeInfo_p->rbMapNum;

        /*TDD Config 0 Changes End*/
        if ( ueScheduledInfo_p == PNULL)
        {
            /* coverity fix- 63371 start */
            freeMemPool(ulStrategyTxNode);
            ltePanic("ueScheduledInfo_p is NULL subFrame [%d]\n", ulSfFromStrategyNode);
            return; 
            /* coverity fix- 63371 end */
        }

        /* ul Qos Changes */
        /* reset the flag for New Transmission */
        /* ulUEContext_p->scheduleFlag = UL_QOS_NEW_TRANSMISSION;
         *  this code is being commented as there is no need of changing the UE
         * state here */

        /*Check for RECEIVED_NACK_RE_TRANS_COMPELETE, UL_NON_ADAPTIVE_NO_CCE and UL_ADAPTIVE_CCE
         * has been removed, as if a UE's retx is going on, it shouldn't be schedule by strategy*/
        /*SPS_CHG Check if the node is for SPS UL New Tx CCE allocation*/
        if(  ulUEContext_p->schType == SPS_SCHEDULING )
        {
            ueSearchSpaceInfo_p =  &ulUEContext_p->spsUeSearchSpaceInfo_p[subFrameNum];
            /* + SPS_TDD_Changes */
            //#ifdef FDD_CONFIG
            /*SPR 6690 fix Start */
            maxUeScheduledInUplink_p[ulSfFromStrategyNode]++;
            /*SPR 6690 fix End */
            //#endif
            /* - SPS_TDD_Changes */
        }
        else
        {
            ueSearchSpaceInfo_p =  &ulUEContext_p->ueSearchSpaceInfo_p[subFrameNum];
        }
        /*SPS_CHG Check if the node is for SPS UL New Tx CCE allocation*/

        /*
         * Incase of Scheduled Count is 2 in ulstrategy node, maxUeScheduled is decremented for
         * both kth and 7th ulsubframe to indicate that UE has been scheduled in that ulsubframe
         * while processing kth ulsubframe strategy node.
         * So, while processing ulstrategy node for 7th ulsubframe, maxUeScheduledInUplink might be
         * 0 but in case ueScheduledInfo for that ulsubframe should not be INVALID_CCE_INDEX.
        */
        if ((maxUeScheduledInUplink_p[ulSfFromStrategyNode] || (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))&&
            ( MAC_SUCCESS == allocatePDCCHForUL(
                        ulStrategyTxNode,
                        ueSearchSpaceInfo_p,
                        pdcchNodeInfo_p->pdcchULOutput_p,
                        dciCCEInfo_p,
                        availableCCE_p,
                        ueScheduledInfo_p,
                        ulUEContext_p,
                        internalCellIndex,
                        /*TDD Config 0 Changes Start*/
                        currentGlobalTickWithDelay,
                        localPdcchInfoMap,
                        pdcchNodeInfo_p->rbMapNum,
                        maxUeScheduledInUplink_p)))
            /*TDD Config 0 Changes End*/
        {   
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "Allocation of CCE is Successful\n");
            tempULHarqInfo_p->isPdcchAllocated = UL_ADAPTIVE_CCE;

            /* Transmitter works on this CCE array, depending upon number
               of UEs that had been allocated CCE in this subFrame */
            dciCCEContainerInfo_p->arrayCCEAllocated
                [dciCCEContainerInfo_p->countOfCCEAllocated] = 
                ueScheduledInfo_p->ulCCEIndex;

            ueScheduledInfo_p->flag = NEW_ALLOCATION;

            dciCCEContainerInfo_p->countOfCCEAllocated++;
            freeMemPool(ulStrategyTxNode);
            ulStrategyTxNode = PNULL;

            /* Qos changes_v start*/
            /* In case of QoS scheduling no need to check the available RBs. It will be checked on PUSCH. But 
               in the case of RR, it is done here. */
            /*SPR 6690 fix Start */
            /*SPR 6690 fix End */
        }     
        else if (INVALID_CCE_INDEX == ueScheduledInfo_p->ulCCEIndex)
        {
            /*SPR 6690 fix Start */
            /*coverity_530_32863*/
            if(  (ulUEContext_p->schType == SPS_SCHEDULING ) &&
                    maxUeScheduledInUplink_p[ulSfFromStrategyNode])
                /*coverity_530_32863*/
            {
                maxUeScheduledInUplink_p[ulSfFromStrategyNode]--;
            }
            if(UL_SPS_DEACTIVATION == ulStrategyTxNode->ulGrantRequestType )
            {
                GET_MEM_FROM_POOL(MacULSpsStatus,macUlSpsStatus_p,
                        sizeof(MacULSpsStatus), PNULL);
                /*+COVERITY 5.3.0 - 25229*/
                if (PNULL != macUlSpsStatus_p )
                {
                    /*-COVERITY 5.3.0 - 25229*/
                    macUlSpsStatus_p->ueIndex = ulUEContext_p->ueIndex;
                    macUlSpsStatus_p->response = PDCCH_CCE_ALLOC_FAILURE;
                    /* + Coverity Id - 32596 */
                    if(!ENQUEUE_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex],
                                macUlSpsStatus_p))
                    {
                        freeMemPool(macUlSpsStatus_p);
                        macUlSpsStatus_p = PNULL;
                    }
                    /* - Coverity Id - 32596 */
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,
                            QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                            internalCellIndex);
                    /* CA Stats Changes End */
                }
                /*-COVERITY 5.3.0 - 25229*/
            }
            /*SPR 6690 fix End */
            /* SPR 5295 changes start*/
            /*TDD Config 0 Changes Start*/
            rbMapNode_p->ulAvailRB.totalRBs += ulStrategyTxNode->requiredRB;
            if (CE_USER == ulUEContext_p->userLocationType)
            {
                rbMapNode_p->ulAvailRB.cellEdgeRBs += ulStrategyTxNode->requiredRB;
            }
            /*TDD Config 0 Changes End*/
            /* SPR 5295 changes end*/
            {
                putEntryInPUSCHFailureQueue(ueIndex,
                        ulStrategyTxNode->ttiCounter,
                        ulStrategyTxNode->ulGrantRequestType,
                        internalCellIndex);
                /* CA_phase2_csi_code start */
                ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
                /* CA_phase2_csi_code end */

                freeMemPool(ulStrategyTxNode);
                ulStrategyTxNode = PNULL;
                /* TDD Config 0 Changes Start */
                ulQosUpdateScheduledTokens(
                        ulUEContext_p,
                        0,
                        INVALID_HARQ_ID
                        ,internalCellIndex
                        ,ulSfFromStrategyNode
                        ,tokenUpdateFromStr
                        ,execArgs);
                /* TDD Config 0 Changes End */
            }
        }
        else
        {
            /*It will come in case of CCE allocation failure in 2nd strategy 
             * node having schedule count 2  */
            tempULHarqInfo_p->isPdcchAllocated = UL_ADAPTIVE_CCE;
            ueScheduledInfo_p->flag = NEW_ALLOCATION;
            freeMemPool(ulStrategyTxNode);
            ulStrategyTxNode = PNULL;
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);        
    return;
}
#endif


/***************************************************************************************
 * Function Name : allocatePDCCHULTx
 * Inputs        : subFrameNum : This stores the subFrame Number on which UE 
 *                     Specific Search Space need to be found.            
 *                  maxUeScheduledInUplink_p - Maximum Number of UE's to 
 *                      be scheduled in UL
 *                 dciCCEContainerInfo_p -
 *                 internalCellIndex - Cell-Index at MAC
 *                 execArgs
 * Outputs       : availableCCE_p : This represent the number of available CCE
 *                     and if CCE's are allocated this would be deceremented by
 *                     the number of CCEs' allocated .
 *                 dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                     Information is there and once cce is allocated ,updated 
 *                     information is written on the reserved CCE.              
 *                 pdcchULOutputInfo_p : This is pointer to structure which 
 *                     further holds structure of type PdcchDLUEInfo in which those DL
 *                     UE Information is written whose PDCCH region has been allocated.
 *                     PUCCH  will use this for reserving the Resources for DL ACK 
 *                     Nack in advance for the DL UE for which PDCCH region has been 
 *                     allocated. 
 * Returns       : None
 * Description   : This funtion will Process those  UE  whose Uplink Grant need to
 *                 be sent and then call funtion allocatePDCCHForUL which will
 *                 allocate the CCEs for them 
 *****************************************************************************************/

#ifdef FDD_CONFIG
/** SPR 13599 Changes Start **/
STATIC  void  allocatePDCCHULTx(
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 *availableCCE_p, 
        DciCCEInfo *dciCCEInfo_p,
        UInt32 subFrameNum,
        DciCCEContainerInfo *dciCCEContainerInfo_p
        /* SPR 5798 Fix Start */
        ,UInt32 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex internalCellIndex
        ,ExecutionLegFunctionArg* execArgs
        )
/** SPR 13599 Changes End **/
{
    UInt32 countULUETx =0;
    UInt16 ueIndex = 0;       
    ULStrategyTxNode *ulStrategyTxNode = PNULL;
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;           
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p = PNULL;             
    LP_MacULSpsStatus macUlSpsStatus_p = PNULL;
    SInt32 availableULRBCount = availableULRBCount_g[internalCellIndex];    

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n",__func__);

    /* Getting the count of Uplink UE For which UpLink Grant Message need 
       to be send and for which PDCCH Region need to be allocated */
#ifndef DL_UL_SPLIT
    countULUETx = sQueueCount(ulStrategyTxQueue_gp[internalCellIndex]); 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Number of new UL UE =%u \n",countULUETx);
#else
    countULUETx = sQueueCount(ulStrategyNewTxQueue_gp[internalCellIndex]);
#endif
/* UL_MU_MIMO_CHG_START */

    UInt8 userModulationType = 0;
    UInt8 userLocationType = 0;
    for (userLocationType = 0; userLocationType < MAX_USER_LOCATION; userLocationType++)
        {
            for (userModulationType = 0; userModulationType < MAC_MAX_NUM_MOD_SCHEME; userModulationType++)
            {
                pdcchULOutputInfo_p->pdcchULMUMIMOCandidateInfo[userLocationType][userModulationType].count = 0;
            }
        }


    /* This parameter is reset to 0 in every tick for storing UL MU MIMO RB requirement count per tick */
    aggregateMaxRBsRequired_g[internalCellIndex] = 0;
/* UL_MU_MIMO_CHG_END */
    /* Below while loop is working on the ulStrategyTxQueue_g queue and for
       each node popped,it get the UE and try to allocate the PDCCH region to 
       it.It will iterate  through while loop until both of the condition is 
       meet.    
       1) Number of RB allocated is less than max number of RB in uplink
       2) Number of UL UE for which PDCCH need to be allocated 
       is allocated is greater than 0*/   

/*DYN_PDCCH_DL_UL_SPLIT_FIX*/
    while ( countULUETx--)
    {
#ifndef DL_UL_SPLIT   
        ulStrategyTxNode = (ULStrategyTxNode*)popNode(ulStrategyTxQueue_gp[internalCellIndex]);
#else
        ulStrategyTxNode = (ULStrategyTxNode*)popNode(ulStrategyNewTxQueue_gp[internalCellIndex]);
#endif
/*DYN_PDCCH_DL_UL_SPLIT_FIX*/
        if ( PNULL ==  ulStrategyTxNode )
        {
#ifndef DL_UL_SPLIT    
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "countULUETx =%u \n",sQueueCount(ulStrategyTxQueue_gp[internalCellIndex]));
#endif
            continue;
        }
        ueIndex = ulStrategyTxNode->ueIndex;     
#ifdef DL_UL_SPLIT    
        if (ueIndex == INVALID_UE_INDEX)
        {
            // Delimiter node received so stop processing of this queue and break out
            freeMemPool(ulStrategyTxNode);
/*DYN_PDCCH_DL_UL_SPLIT_FIX*/
            ulStrategyTxNode = PNULL;
/*DYN_PDCCH_DL_UL_SPLIT_FIX*/
            break;
        }
#endif
        LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                "Processing on UE = %u\n",ueIndex); 
        ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
/*SPR 20021 Fix start*/
        if ( PNULL ==  ulUEContextInfo_p )
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "NO UE context, UE deleted already \n");
            continue;
        }
/*SPR 20021 Fix end*/

        if ( ulUEContextInfo_p->pendingDeleteFlag)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "UE Deletetion In Progress \n");
            freeMemPool(ulStrategyTxNode);
            continue;
        }
        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;


        /* ul Qos Changes */

        if ( pdcchULOutputInfo_p->directIndexingUEInfo[
                                ulStrategyTxNode->ueIndex].flag ==
                                RECEIVED_NACK_RE_TRANS_COMPELETE)
        {
            {
                putEntryInPUSCHFailureQueue(ueIndex,
                        ulStrategyTxNode->ttiCounter,
                        ulStrategyTxNode->ulGrantRequestType,
                        internalCellIndex);
                freeMemPool(ulStrategyTxNode);
                    ulUEContext_p->ulStrategyTxNode_p = PNULL; 
                ulQosUpdateScheduledTokens(ulUEContext_p,0,INVALID_HARQ_ID,
                                           internalCellIndex, execArgs);
            }

            continue;
        }
        
        /* if ulUEContext_p->isPdcchAllocated is TRUE then this means that either
           this UE has been scheduled in UPLINK already or in DOWNLINK. So put
           this entry in PdcchAdvance Queue. */

        switch ( ulUEContext_p->isPdcchAllocated )
        {
            case UL_NON_ADAPTIVE_NO_CCE:
            case UL_ADAPTIVE_CCE:
                LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                        "PDCCH Region  been allocated Earlier\n");

                {
                    putEntryInPUSCHFailureQueue(ueIndex,
                        ulStrategyTxNode->ttiCounter,
                        ulStrategyTxNode->ulGrantRequestType,
                        internalCellIndex);
                    freeMemPool(ulStrategyTxNode);
                    ulUEContext_p->ulStrategyTxNode_p = PNULL; 
                ulQosUpdateScheduledTokens(ulUEContext_p,0,INVALID_HARQ_ID,
                                           internalCellIndex,execArgs);
                }
                continue;
            default:
                LTE_MAC_UT_LOG(LOG_INFO, PDCCH_MGR, "CCE Type %d Received\n", 
                                            ulUEContext_p->isPdcchAllocated);
        }
/*SPS_CHG Check if the node is for SPS UL New Tx CCE allocation*/
        if(  ulUEContext_p->schType == SPS_SCHEDULING )
        {
            /* ++ SPR 21406 */
            ueSearchSpaceInfo_p=getULUEContextSearchSpaceInfo(ulUEContext_p,subFrameNum,internalCellIndex);
            /* -- SPR 21406 */
            if(!ueSearchSpaceInfo_p)
            {
                return ;
            }
        /*SPR 6690 fix Start */
              (*maxUeScheduledInUplink_p)++;
        /*SPR 6690 fix End */
        }
        else
        {

            /*pdcch changes start*/
            ueSearchSpaceInfo_p=getULUEContextSearchSpaceInfo(ulUEContext_p,subFrameNum,internalCellIndex);
            if(!ueSearchSpaceInfo_p)
            {
                return ;
            }
            /*pdcch changes end*/
        }
/*SPS_CHG Check if the node is for SPS UL New Tx CCE allocation*/
 
            /* 5798 Fix Start */
        /*SPR 6690 fix Start */
        if ( ((ulStrategyTxNode->ulGrantRequestType == UL_SPS_OCCASION) || 
             ( *availableCCE_p && ( (UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             initParams_p->ulSchdConfig.ulSchedulerStrategy) || availableULRBCount >0) && \
               (*maxUeScheduledInUplink_p))) && 
             ( MAC_SUCCESS == allocatePDCCHForUL(
        /*SPR 6690 fix End */
                    ulStrategyTxNode,
                    ueSearchSpaceInfo_p,
                    pdcchULOutputInfo_p,
                    dciCCEInfo_p,
                    availableCCE_p,
                    ulUEContext_p,
                    /* +- SPR 17777 */
                    internalCellIndex)))
            /* 5798 Fix End */
        {   
            LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
                    "Allocation of CCE is Successful\n");
            ulUEContext_p->isPdcchAllocated = UL_ADAPTIVE_CCE;

            /* 5798 Fix Start */
            (*maxUeScheduledInUplink_p)--;
            /* 5798 Fix End */
            /* Transmitter works on this CCE array, depending upon number
               of UEs that had been allocated CCE in this subFrame */
            dciCCEContainerInfo_p->arrayCCEAllocated
                [dciCCEContainerInfo_p->countOfCCEAllocated] = 
                        pdcchULOutputInfo_p->directIndexingUEInfo[
                        ulStrategyTxNode->ueIndex].ulCCEIndex;
            
            pdcchULOutputInfo_p->directIndexingUEInfo[
                    ulStrategyTxNode->ueIndex].flag = NEW_ALLOCATION;
            dciCCEContainerInfo_p->countOfCCEAllocated++;
            availableULRBCount -= ulStrategyTxNode->requiredRB;
            freeMemPool(ulStrategyTxNode);
            ulStrategyTxNode = PNULL;
            ulUEContext_p->ulStrategyTxNode_p = PNULL; 

            /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
            /* check if UE is HD FDD and schedule UL Tx*/
            if(ulUEContext_p->isHDFddFlag)
            {
                /* SPR 15909 fix start */
                tickType_t localCurrTick = getCurrentTick()+ PHY_DL_DELAY;
                /* SPR 15909 fix end */
                SCHEDULE_UL_FOR_HD_UE(localCurrTick, ulUEContext_p->ueIndex, 
                hdUeContextInfo_g.hdfddUeSchedMap,internalCellIndex);
            }
            /* HD FDD Changes End */
#endif

             /* Qos changes_v start*/
            /* In case of QoS scheduling no need to check the available RBs. It will be checked on PUSCH. But 
               in the case of RR, it is done here. */
        /*SPR 6690 fix Start */
        /*SPR 6690 fix End */
        }     
        else
        {
             /*SPR 6690 fix Start */
            /*coverity_530_32863*/
             if( 
                  (ulUEContext_p->schType == SPS_SCHEDULING ) && 
                    (*maxUeScheduledInUplink_p))
            /*coverity_530_32863*/
             {
                 (*maxUeScheduledInUplink_p)--;
             }
             if(UL_SPS_DEACTIVATION == ulStrategyTxNode->ulGrantRequestType )
             {
                 GET_MEM_FROM_POOL(MacULSpsStatus,macUlSpsStatus_p,
                                   sizeof(MacULSpsStatus), PNULL);
                 /*+COVERITY 5.3.0 - 25229*/
                 if (PNULL != macUlSpsStatus_p )
                 {
                     /*-COVERITY 5.3.0 - 25229*/
                     macUlSpsStatus_p->ueIndex = ulUEContext_p->ueIndex;
                     macUlSpsStatus_p->response = PDCCH_CCE_ALLOC_FAILURE;
                     /* + Coverity Id - 32596 */
                     if(!ENQUEUE_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex],
                             macUlSpsStatus_p))
                     {
                         freeMemPool(macUlSpsStatus_p);
                         macUlSpsStatus_p = PNULL;
                     }
                     /* - Coverity Id - 32596 */
                     /* Update Q Stats */
                     /* CA Stats Changes Start */
                     LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,
                             QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                             internalCellIndex);
                     /* CA Stats Changes End */
                 }
                 /*-COVERITY 5.3.0 - 25229*/
             }
            /*SPR 6690 fix End */
           /* SPR 5295 changes start*/
            ulBufferPool_g[internalCellIndex].totalRBs += ulStrategyTxNode->requiredRB;
            if (CE_USER == ulUEContext_p->userLocationType)
            {
                ulBufferPool_g[internalCellIndex].cellEdgeRBs += ulStrategyTxNode->requiredRB;
            }
           /* SPR 5295 changes end*/
            {
                putEntryInPUSCHFailureQueue(ueIndex,
                        ulStrategyTxNode->ttiCounter,
                        ulStrategyTxNode->ulGrantRequestType,
                        internalCellIndex);
                /* CA_phase2_csi_code start */
                ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
                /* CA_phase2_csi_code end */

                freeMemPool(ulStrategyTxNode);
                    ulStrategyTxNode = PNULL;
                    ulQosUpdateScheduledTokens(ulUEContext_p,0,INVALID_HARQ_ID,
                                               internalCellIndex,execArgs);
                    ulUEContext_p->ulStrategyTxNode_p = PNULL; 
            }
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);        
    return;
}
#endif

/*****************************************************************************
 * Function Name : cleanUpStrategyQueueIfCCENotAvlbl
 * Inputs        : internalCellIndex - Cell-Index at MAC
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion cleans the strategy queue
 *****************************************************************************/
STATIC  void cleanUpStrategyQueueIfCCENotAvlbl(InternalCellIndex internalCellIndex,
                                                     ExecutionLegFunctionArg* execArgs)
{
    UInt8 tempCount = 0;
    ULStrategyTxNode *ulStrategyTxNode = PNULL;
    /* +- SPR 18268 */
    UInt16 ueIndex = 0;
    /* +- SPR 18268 */
    ULUEContext *ulUEContext_p = PNULL;
#ifdef DL_UL_SPLIT
    tempCount = sQueueCount(ulStrategyNewTxQueue_gp[internalCellIndex]);
#else
    tempCount = sQueueCount(ulStrategyTxQueue_gp[internalCellIndex]);
#endif
    while ( tempCount--)
    {
#ifndef DL_UL_SPLIT
        ulStrategyTxNode = (ULStrategyTxNode*)popNode(ulStrategyTxQueue_gp[internalCellIndex]);
#else
        ulStrategyTxNode = (ULStrategyTxNode*)popNode(ulStrategyNewTxQueue_gp[internalCellIndex]);
#endif
        if ( PNULL ==  ulStrategyTxNode )
        {
            continue;
        }
        ueIndex = ulStrategyTxNode->ueIndex;
#ifdef DL_UL_SPLIT
        if (ueIndex == INVALID_UE_INDEX)
        {
            freeMemPool(ulStrategyTxNode);
            ulStrategyTxNode = PNULL;
            break;
        }
#endif
        ulUEContext_p =ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
        putEntryInPUSCHFailureQueue(ueIndex,
                                    ulStrategyTxNode->ttiCounter,
                                    ulStrategyTxNode->ulGrantRequestType
                                    , internalCellIndex);
#ifdef FDD_CONFIG
        ulUEContext_p->ulStrategyTxNode_p = PNULL;
        ulQosUpdateScheduledTokens(ulUEContext_p,0,INVALID_HARQ_ID, internalCellIndex, execArgs);
#elif TDD_CONFIG
        ulQosUpdateScheduledTokens(ulUEContext_p,
                                   0,
                                   INVALID_HARQ_ID,
                                   internalCellIndex,
                                   ulStrategyTxNode->ulSubFrameNum,
                                   LTE_FALSE,
                                   execArgs
                                   );
#endif
        freeMemPool(ulStrategyTxNode);
    }
}

/*****************************************************************************
 * Function Name : allocPdcchPdschForDL
 * Inputs        : withMuxFlag-This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 flagToTriggerPUSCH - This will be used to trigger PUSCH 
 *                     functionality once PDCCH complete it processing.
 *                 delayToPhy-This stores delay from current module to PHY
 *                 currentGlobalTick-This stores current Global Tick.
 *                 subFrame : This stores the subFrame Number 
 *                 internalCellIndex - Cell-Index at MAC
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will call funtions which will allocate CCE for both 
 *                 DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *                 PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/

#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDL(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* +- SPR 17777 */
        UInt32 delayToPhy,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 subFrame,
        InternalCellIndex  internalCellIndex
        ,ExecutionLegFunctionArg* execArgs
        )
{
    /* + Coverity 54551 */
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    DciCCEInfo   *dciCCEInfo_p = PNULL;
    UInt32 availableCCE = 0;    
    UInt32 subFrameNum = 0;
    SInt32 availableDLRBCount = 0;
    PdcchULOutputInfo *pdcchULOutputInfo_p = PNULL;
    /* + coverity 55830 */
    PdcchDLOutputInfo* pdcchDLOutputInfo_p = PNULL;
    /* - coverity 55830 */
    UInt32 containerIndex = 0;
    UInt8  nackQContainerTick =0;
    /*DYN_PDCCH_DL_UL_SPLIT_FIX*/
    UInt16 nackQueueCount = 0;
    /*DYN_PDCCH_DL_UL_SPLIT_FIX*/
#ifndef DL_UL_SPLIT    
    UInt16 nackQueueCountForTempUE = 0;
    UInt16 nackQueueCountSpsUE = 0;
#endif    
    UplinkContainer* ulContainerQueue_p = PNULL;  
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay = currentGlobalTick + delayToPhy;
    /* SPR 15909 fix end */
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
    UInt32 maxCommonSpaceCCE = 0;
    /* 5492 changes start */
    CellConfigParams *cellParams_p = \
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt32 maxUeScheduledInDownlink = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
        dlSchdConfig.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    UInt8               allocSPSFirst               = 0;
    UInt32              spsRMIndex                  = 0;
    UInt8               spsRBs                      = 0;
#ifdef PERF_STATS
    UInt32              utilizedCCE                 = 0;
    UInt8               utilizedCCEIndex            = 0;
/*  SPR_9402_FIX_START */ 
    UInt32 availableCCE_max_local = 0;
/*  SPR_9402_FIX_END */
#endif

    /* TTIB_Code Start */
    UInt16              nackQueueCountBundledUE     = 0;
    UInt8               ttiBundleQIndex             = 0;
    MultiplexerQueueNode *muxQueueNode_p            = PNULL;
    /* TTIB_Code End*/
   /* EICIC +*/
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNumTemp = 0;
    UInt8 absSubFrameNum = 0;
    UInt8 numBitsAbsPattern = 0;
   /* EICIC -*/
   
    /* 5798 changes start */
    UInt32 ulsubFrameNum = (currentGlobalTickWithDelay + FDD_DATA_OFFSET) % 
        NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    UInt32 maxUeScheduledInUplink = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                    initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;


    /*SPR 8416 Start*/
    if (maxUeScheduledInUplink >= rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[ulsubFrameNum])
    {
        maxUeScheduledInUplink = maxUeScheduledInUplink - rachGlobals_gp[internalCellIndex]->\
                                 msg3ScheduledCount[ulsubFrameNum]; 
    }
    else
    {
        maxUeScheduledInUplink = 0;
    }
    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[ulsubFrameNum] = 0;
    /*SPR 8416 End*/
    /* 5798 changes end */

    /* + coverity 55830 */
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);

    if(PNULL == pdcchDLOutputInfo_p)
    {
        /*A MUX NODE WITH DUMMY MSG TYPE Should be inserted, So that 
          Multiplex can continue */
        GET_MEM_FROM_POOL(MultiplexerQueueNode , muxQueueNode_p ,
                sizeof(MultiplexerQueueNode) , PNULL);
        if(PNULL == muxQueueNode_p)
        {
            return;
        }
        muxQueueNode_p->msgType = DUMMY_MSG;
        putEntryInMultiplexerQueue(muxQueueNode_p,internalCellIndex );
        return;
    }
    /* - coverity 55830 */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__); 
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
    /* SPS_CHG */
    spsRMIndex = (currentGlobalTick + delayToPhy ) % MAX_SPS_INTERVAL; 
    spsRBs = spsStrTTIMap_g[internalCellIndex][spsRMIndex].numDlSpsRbsPerTti;
    /* SPS_CHG */
    pdcchULOutputInfo_p = (pdcchULOutputInfo_gp[internalCellIndex] + containerIndex);
    /* ICIC changes start */
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
    /* SPR 1115 CHG */    
    pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    /* SPR 4866 Fix Start */
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* SPR 4866 Fix End */
    /* ICIC changes end */

    UInt8  rbMapNum = 0;
    UInt8 spsArrayTti = MAX_SPS_INTERVAL;
    UInt8 totalCCERequired = 0;

    /* + PRS_CHANGES */
    PRSConfigInfo   *prsConfigInfo_p = PNULL;
    UInt32  pdcchOrderQCount = 0;
    PDCCHOrderNode *pdcchOrderNode_p = PNULL;
    UInt8    pdcchOrderPerTTICount = 0;
    /* - PRS_CHANGES */
#ifdef LTE_EMBMS_SUPPORTED
    UInt8 eMbmsCfiValue;   // Not Initializing
#endif


    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
    /*START : DYN_PDCCH*/
    availableCCE  = maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex]-1];
    /*END   : DYN_PDCCH*/
#ifdef LTE_EMBMS_SUPPORTED
    if((isEmbmsValidSf_g[internalCellIndex] == TRUE) && 
       (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
    {
         /* SPR 18482 fix start*/
         eMbmsCfiValue = embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
         /* SPR 18482 fix end*/                      
         availableCCE  = maxCCEForCFI_g[internalCellIndex][eMbmsCfiValue - 1];
    }
#endif
    /*Get the subframeNum on which UE Specific Search Space Need to be Found*/
    /* SPR 23393 Changes End */
    subFrameNum =  (currentGlobalTick + delayToPhy ) % 
        NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;   
/*  SPR_9402_FIX_START */ 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR, \
            "subFrameNum = %u\n maxCCEForCFI_g[valueCFI_g -1]= %u\n availableDLRBCount = %d\n");
    /*Getting the Max DownLink RB  which is configured by
      RRM */
    /* ICIC changes start */
    availableDLRBCount  =  (dlRAInfo_gp[internalCellIndex] +dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;   
    availableULRBCount_g[internalCellIndex] = (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB;
    /* ICIC changes start */

    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + containerIndex);

    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx ,internalCellIndex);

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch & 
       tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick + delayToPhy, internalCellIndex );
    triggerDciForPucchPowerControl( currentGlobalTick + delayToPhy, internalCellIndex );

    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU.
      CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
      that are left after allocating common space CCEs to BCCH, PCCH, RACH 
      and CCCH message */
    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */

    /* + PRS_CHANGES */
    allocateCCEsForSIPagingULOnly[internalCellIndex] = FALSE;
    prsConfigInfo_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo);
    CHECK_FOR_PRS_OCCASION(prsConfigInfo_p,
                           cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                           cellParams_p->dlBandwidth,internalCellIndex);
    /* - PRS_CHANGES */
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g[internalCellIndex] = 0;
    /* - SPR 8190 */
    
 /* EICIC +*/
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
   getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);
    
        /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNumTemp = subFrameNumTemp + delayToPhy;
	    /*Complexity Start*/    
    eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNumTemp);
    numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
	    /*Complexity End*/
 	isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
	CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex);
    }
    else
    {
	    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    }
    /* EICIC Optimization +- */
       /* EICIC -*/

    maxCommonSpaceCCE = allocatePdcchCommonChannel(
            &availableCCE, 
            &availableDLRBCount,
            pdcchDLOutputInfo_p,
            dciCCEContainerInfo_p,
            delayToPhy,
            pdcchULOutputInfo_p,
            /* +- SPR 17777 */
            &maxUeScheduledInDownlink,
            internalCellIndex);

    /*START : DYN_PDCCH*/
    /* This function processes scheduleFormat3Or3AQueue to schedule 
       DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if ((availableCCE) && (!isUeTypeInvalid_g[internalCellIndex]))
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfo_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTickWithDelay,
                internalCellIndex);
    }

    /* CLPC_CHG END */
    currentCFIValue_g[internalCellIndex] = calculateCFI(currentGlobalTickWithDelay,
            pdcchDLOutputInfo_p,
            /* +- SPR 17777 */
#ifdef FDD_CONFIG
            pdcchULOutputInfo_p,
#endif
            BOTH_DL_UL,
            /* +- SPR 17777 */
            &totalCCERequired,
            internalCellIndex);
    /* Cyclomatic_complexity_changes_start */
    setCurrentCFIValueInPDCCH(cellParams_p,
            maxCommonSpaceCCE,
            &availableCCE,
            totalCCERequired,
            internalCellIndex);
    /* Cyclomatic_complexity_changes_end */
    /* - PRS_CHANGES */

    /*END  : DYN_PDCCH*/
#ifdef LTE_EMBMS_SUPPORTED
    if((isEmbmsValidSf_g[internalCellIndex] == TRUE) &&
       (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
    {
        currentCFIValue_g[internalCellIndex] = \
         /* SPR 18482 fix start*/
         embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
         /* SPR 18482 fix end*/
		availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1]\
                       - (maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex] - 1] - maxCommonSpaceCCE);
    }
#endif
    
    /* SPR_9402_FIX_START */
#ifdef PERF_STATS
     availableCCE_max_local = 
         maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1];
#endif
     /*  SPR_9402_FIX_END */
    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
#ifdef TDD_CONFIG
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
    /** CA-TDD Changes Stop **/
#endif
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g[internalCellIndex].totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g[internalCellIndex].totalRBs = 0;
        dlBufferPool_g[internalCellIndex].cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g[internalCellIndex].totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);

        if (dlBufferPool_g[internalCellIndex].cellEdgeRBs > dlBufferPool_g[internalCellIndex].totalRBs)
        {
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = dlBufferPool_g[internalCellIndex].totalRBs;
        }
    }
    excessPoolRb_g[internalCellIndex][CC_CE_REGION] = dlBufferPool_g[internalCellIndex].totalRBs;
    excessPoolRb_g[internalCellIndex][CE_REGION] = dlBufferPool_g[internalCellIndex].cellEdgeRBs;

    LOG_UT (MAC_PDSCH_INITIAL_BUFF_POOL_DETAILS,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),\
            dlBufferPool_g[internalCellIndex].totalRBs,\
            dlBufferPool_g[internalCellIndex].cellEdgeRBs,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
            "Initial Values of Buffer Pool");
    /* ICIC changes end */
#ifdef FDD_CONFIG
    /* SPS_CHG */
    /*SPR 6925 Optimization Start*/
    SpsSysWideParam *spsSysWideParam_p = \
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam);
    if ((TRUE == spsSysWideParam_p->enableSps) &&
            ((spsRBs + RESERVE_SPS_FIRST_FACTOR*(availableDLRBCountTemp - availableDLRBCount)) < availableDLRBCountTemp ))
    {
        allocSPSFirst = 1;
    }
    /*SPR 6925 Optimization End*/

    processAdvancePUCCHAndPRACHReports(
            MODULO_SIXTEEN(currentGlobalTickWithDelay + FDD_HARQ_OFFSET),\
            currentGlobalTickWithDelay + FDD_HARQ_OFFSET ,internalCellIndex);

    rbMapNum = MODULO_SIXTEEN(currentGlobalTick + delayToPhy + FDD_HARQ_OFFSET);
    spsArrayTti = (currentGlobalTick + delayToPhy)%MAX_SPS_INTERVAL; 
    /*SPR 6925 Optimization Start*/
    if(TRUE == spsSysWideParam_p->enableSps)
    {
        markULSpsRBsInRBMap(rbMapNum, spsArrayTti, NORMAL_LOCATION_UPDATE, currentGlobalTick, delayToPhy, internalCellIndex);
    }
    /*SPR 6925 Optimization End*/

#endif
    /* SPS_CHG */

    nackQContainerTick = MODULO_EIGHT(currentGlobalTickWithDelay - FDD_HARQ_OFFSET);
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;
#ifndef DL_UL_SPLIT
        /*SPR 6925 Optimization Start*/
        if(TRUE == spsSysWideParam_p->enableSps)
        {
            nackQueueCountSpsUE = sQueueCount(&(ulContainerQueue_p->nackQueueForSps));
        }
    /*SPR 6925 Optimization End*/
    if(nackQueueCountSpsUE)
    {
        processULNackQForSpsUE(delayToPhy, currentGlobalTick , &availableCCE,
            dciCCEInfo_p,dciCCEContainerInfo_p,
            internalCellIndex);
    }
    else
    {
#endif
        /* ICIC changes start */
        pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.adaptiveCount[CE_USER] = 0;
        /* ICIC changes end */
        pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.nonAdaptiveCount = 0;
#ifdef DL_UL_SPLIT
        // Free memory of the Delimiter node
        //freeMemPool(nackNode_p);
        //nackNode_p = PNULL;
#else
    }
#endif
    /* TTIB_Code Start */
    pdcchULOutputInfo_p->pdcchULNackUEInfo.nonAdaptiveCount = 0;
    /* Process/schedule next 3 ticks of bundle in case of new trasnmission and 
       all 4 ticks of bundle in case of retransmission */
    ttiBundleQIndex = (currentGlobalTick + delayToPhy)% MAX_TTIB_SCHEDULE_QUEUE; 
    if( COUNT_TTIB_SCHEDULE_Q((ttibScheduleQueue_gp[internalCellIndex] + ttiBundleQIndex)))
    {
        //CODE_COMMENT_L6_FIXED
        processTTIBundleReTxQ(delayToPhy, currentGlobalTick 
                ,&maxUeScheduledInUplink, internalCellIndex);
    }
    /* Schedule Nack and decide adaptive/non- adaptive reTx for ULSCH for TTI Bundled UE */
    nackQueueCountBundledUE = COUNT_NACK_QUEUE_FOR_TTIB_UE_Q
        (&(ulContainerQueue_p->nackQueueForTtiBundledUe));
    LTE_MAC_UT_LOG(LOG_DETAIL,PDCCH_MGR,"[%s] nackQueueCountBundledUE [%d]\n",nackQueueCountBundledUE);
    if(nackQueueCountBundledUE)
    {
        //CODE_COMMENT_L6_FIXED
        processULNackQForTTIBundledUe(delayToPhy,
            currentGlobalTick,
            &availableCCE,
            &maxUeScheduledInUplink,
            internalCellIndex
            );
    }
    /* TTIB_Code End */
#ifdef DL_UL_SPLIT
    ContainerNackQueueNode* nackNodeForTempUE_p = PNULL;
#ifdef CIRC_QUEUE_IMPL
    semWaitDefault(&waitForULNackForTempUE_g[internalCellIndex]);
    DEQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(ulContainerQueue_p->nackCircQueueForTempUE),
            ContainerNackQueueNode, (void *)&nackNodeForTempUE_p);
#else
    /* +- SPR 17777 */
    recvMsgWithIccQ(ulContainerQueue_p->ulContainerNackQueueForTempUEIdDL, 
            &nackNodeForTempUE_p);
    /* +- SPR 17777 */
#endif
    if((nackNodeForTempUE_p != PNULL) && (nackNodeForTempUE_p->ueIdx != INVALID_UE_INDEX))
#else
    /* + Coverity 54551 */
    nackQueueCountForTempUE = sQueueCount(&(ulContainerQueue_p->nackQueueForTempUE));
    /* - Coverity 54551 */
    if (nackQueueCountForTempUE)
#endif
    {
        /* SPR 5798 Fix Start */
#ifndef DL_UL_SPLIT
        processULNackQForTempUE(delayToPhy, currentGlobalTick , &availableCCE,
                dciCCEInfo_p,dciCCEContainerInfo_p, &maxUeScheduledInUplink,
                internalCellIndex);
#else        
        processULNackQForTempUE(delayToPhy, currentGlobalTick , &availableCCE,
                dciCCEInfo_p,dciCCEContainerInfo_p, nackNodeForTempUE_p, &maxUeScheduledInUplink,
                internalCellIndex);
#endif        
        /* SPR 5798 Fix End */

    }
    else
    {
        /* ICIC changes start */
        pdcchULOutputInfo_p->pdcchULNackTempUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackTempUEInfo.adaptiveCount[CE_USER] = 0;
        /* ICIC changes end */
        pdcchULOutputInfo_p->pdcchULNackTempUEInfo.nonAdaptiveCount = 0;
#ifdef DL_UL_SPLIT
        /* +COVERITY 10558 */
        if (nackNodeForTempUE_p)
        {
            FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNodeForTempUE_p);
        }
        /* -COVERITY 10558 */
#endif
    }

#ifdef DL_UL_SPLIT
    ContainerNackQueueNode* nackNode_p = PNULL;
    nackQueueCount = sQueueCount(ulStrategyReTxQueue_gp[internalCellIndex]);
#else
    nackQueueCount = sQueueCount(&(ulContainerQueue_p->nackQueue));
#endif
    if(nackQueueCount)
    {
        /* SPR 5798 Fix Start */
#ifndef DL_UL_SPLIT
        processULNackQ(delayToPhy, currentGlobalTick , &availableCCE,
            dciCCEInfo_p,dciCCEContainerInfo_p,&maxUeScheduledInUplink,
            internalCellIndex);
#else        
        processULNackQ(delayToPhy, currentGlobalTick , &availableCCE,
            dciCCEInfo_p,dciCCEContainerInfo_p, nackNode_p,&maxUeScheduledInUplink,
            internalCellIndex);
#endif        
        /* SPR 5798 Fix End */
    }
    else
    {
        /* ICIC changes start */
        pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CE_USER] = 0;
        /* ICIC changes end */
#ifdef DL_UL_SPLIT
        // Free memory of the Delimiter node
        /* +COVERITY 10559 */
        if (nackNode_p)
        {
            freeMemPool(nackNode_p);
            nackNode_p = PNULL;
        }
        /* -COVERITY 10559 */
#endif
    }
    /* SPR 5798 Fix Start */
    ulContainerQueue_p->nackQueueCount = 0;
    /* SPR 5798 Fix End */

    /* 5492 changes start */
    /* SPR 4063 Start */
    if(availableCCE)
    {
        /* + PRS_CHANGES */
        /* If it is PRS occasion then push all the entries in new  PDCCH ORDER 
         * queue in to  PDCCH ORDER advance queue.
         */ 
        if ((!allocateCCEsForSIPagingULOnly[internalCellIndex]) && (!isUeTypeInvalid_g[internalCellIndex]))
        { 
            /* Calling Function processPdcchOrderQueue which will process PDCCH
             * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
             * starting contention free rach. Only DCI 1A PDU will be sent here*/
            processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                    dciCCEInfo_p, subFrameNum, currentGlobalTickWithDelay,&maxUeScheduledInDownlink,internalCellIndex);
        }
        else 
        {
            pdcchOrderQCount = COUNT_PDCCH_ORDER_Q(internalCellIndex);

            /*Assign pdcchOrderPerTTICount with a value not to exceed maximum PDCCH
              Order that can be processed in one tick */
            pdcchOrderPerTTICount = maxPdcchOrderperTTI_g[internalCellIndex];
            /* 5492 changes start */
#ifdef FDD_CONFIG
            if(pdcchOrderPerTTICount > (maxUeScheduledInDownlink))
            {
                pdcchOrderPerTTICount = (maxUeScheduledInDownlink);
            }
#endif
            /* 5492 changes end */
            if (pdcchOrderPerTTICount && pdcchOrderQCount)
            {
                while((pdcchOrderPerTTICount--) && (pdcchOrderQCount--))

                {
                    DEQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, (void *)&pdcchOrderNode_p, internalCellIndex);
                    if(pdcchOrderNode_p)
                    {
                        /* COVERITY CID 56013 Fix start */ 
                        if( pushNode(pdcchOrderAdvanceQueue_gp[internalCellIndex], 
                            &(pdcchOrderNode_p->anchorNode)) )
                        {
                            lteWarning ("pushNode() failed for pdcch Order Advance queue");
                            freeMemPool(pdcchOrderNode_p);
                            /*coverity 61382*/
                            pdcchOrderNode_p = PNULL;
                            /*coverity 61382*/
                            continue;
                        }
                        /* COVERITY CID 56013 Fix end */ 
                        pdcchOrderNode_p = PNULL;
                    }
                }
            } 
        }

        /* - PRS_CHANGES */
    }  
    /* SPR 4063 End */
    /* 5492 changes end */ 
    /* ICIC changes start */
    /* + PRS_CHANGES */
    /* If it is not PRS occasion then only process this queue */
/** SPR 13599 Changes Start **/
   /* EICIC_Coverity CID 54551 Start */ 
#ifndef LTE_EMBMS_SUPPORTED
   /* If eMBMS is Enabled PDCCH allocation for Downlink Re-Transmission 
      is done only on non embms sub frame */
    if (availableDLRBCount && availableCCE && 
       (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
       (!isUeTypeInvalid_g[internalCellIndex]))
#else 
    if (availableDLRBCount && availableCCE && 
       (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
       (!isUeTypeInvalid_g[internalCellIndex]) &&
       (isEmbmsValidSf_g[internalCellIndex] == FALSE)) 
#endif
  /* EICIC_Coverity CID 54551 End */  	
/** SPR 13599 Changes End **/
        /* - PRS_CHANGES */ 
        /* ICIC changes end */
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                pdcchULOutputInfo_p->directIndexingUEInfo,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfo_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex
                );
    }

    if ( availableCCE )
    {   
        /* SPR 2453 Start*/
        /* + PRS_CHANGES */
        /* If it is not PRS occasion then only process this queue */
        /** SPR 13599 Changes Start **/
        /* EICIC_Coverity CID 54551 Start */
#ifndef LTE_EMBMS_SUPPORTED
        if( availableDLRBCount && (!allocateCCEsForSIPagingULOnly[internalCellIndex]))
#else
        // TBD: No need to check for availableCCE as already checked in parent block 
        if (availableDLRBCount &&    
           (!allocateCCEsForSIPagingULOnly[internalCellIndex]) &&
           (isEmbmsValidSf_g[internalCellIndex] == FALSE))
#endif
       /* EICIC_Coverity CID 54551 End */ 	
        /** SPR 13599 Changes End **/
            /* - PRS_CHANGES */
        {
            /*Calling funtion allocatePDCCHDLNewTx funtions which will 
              process those UE whos new Transmission need to be done and then 
              allocating CCEs for them */
            allocatePDCCHDLNewTx(
                    pdcchDLOutputInfo_p,
                    pdcchULOutputInfo_p->directIndexingUEInfo,
                    &availableCCE,
                    dciCCEInfo_p,
                    &availableDLRBCount,
                    subFrameNum,
                    &pdcchULOutputInfo_p->pdcchDLUEInfo,
                    dciCCEContainerInfo_p,
                    /* 5492 changes start */
                    &maxUeScheduledInDownlink,
                    /* 5492 changes end */
                    internalCellIndex
                    );
        }

        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */
        /* SPR 5798 Fix Start */     

        if(!isUeTypeInvalid_g[internalCellIndex])         /* EICIC +*/
        {

        allocatePDCCHULTx(
                pdcchULOutputInfo_p,
                &availableCCE ,
                dciCCEInfo_p,
                subFrameNum,
                dciCCEContainerInfo_p
                ,&maxUeScheduledInUplink
                ,internalCellIndex
                ,execArgs
                );
        } /* EICIC -*/
        /* SPR 5798 Fix End */
        /* SPR 2453 End*/ 
    }
#ifdef DL_UL_SPLIT
    putEntryInPuschSignalQueueUsingICC(CCE_ALLOCATED, internalCellIndex);
#endif

    cleanUpStrategyQueueIfCCENotAvlbl(internalCellIndex,execArgs);
    /*SPR_7217_changes_start*/
    /*if ( flagToTriggerPUSCH)
    {
        semPost(&pdcchTriggerToPuschSem_g);
    }*/
    /*SPR_7217_changes_end*/
    emptyDLStrategyQueues(internalCellIndex);
    LOG_MAC_MSG(MAC_PDCCH_SCHEDULING_DETAILS,LOGDEBUG,
            MAC_PDSCH,getCurrentTick(),
            (UInt32)pdcchDLOutputInfo_p->countCommonChannelMsg,
            (UInt32)pdcchDLOutputInfo_p->countMsg[CC_USER],
            (UInt32)pdcchDLOutputInfo_p->countMsg[CE_USER],
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");
#ifdef PERF_STATS
    /* Get the Utilized CCE from the availableCCE */
   /*  SPR_9402_FIX_START */ 
    utilizedCCE = availableCCE_max_local - availableCCE ;
    /* Get the Utilized CCE Index from the Utilized CCE */
    /* special handling for utilizedCCE */
    /* SPR 18479 fix start*/
    if((utilizedCCE) && (availableCCE_max_local != 0))
    {
    /* SPR 18479 fix end*/
    	utilizedCCEIndex = ((((utilizedCCE * MAX_CCE_UTIL_INDEX)+ availableCCE_max_local - 1)/ availableCCE_max_local ) - 1); 
    }    
    else
    {
        utilizedCCEIndex = 0;
    }    
    /*  SPR_9402_FIX_START */ 
    /* Increment the Counter for the PDCCH utilization for the utilizedCCEIndex */
	/* + PERF_CA */
    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.totalPdcchCceUtil[utilizedCCEIndex]++;
	/* - PERF_CA */
#endif
    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
#if LTE_EMBMS_SUPPORTED
    if (isEmbmsValidSf_g[internalCellIndex] == FALSE)
    {   
#endif
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst
            ,subFrame,
            internalCellIndex
            );
#if LTE_EMBMS_SUPPORTED
    }   
#endif
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
    /* - Coverity 54551 */
    /* + coverity 55830 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 55830 */
}
#endif

#ifdef TDD_CONFIG
#ifndef DL_UL_SPLIT_TDD
/***********************************************************************************
 * Function Name : allocPdcchPdschForDCIFormat0AndDLInNormalSF
 * Inputs        : withMuxFlag - This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 currentGlobalTick - This stores current Global Tick.
 *                 SFN - system frame number
 *                 subFrame : This stores the subFrame Number 
 *                 ulSubframeNum - This store the uplink subframe number in TDD 
 *                 internalCellIndex - Cell-Index at MAC
 *                 ulSubframeNumForDlAckNack - UL Subframe number for DL data ack/nack
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will call funtions which will allocate CCE for both 
 *                 DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *                 PDSCH funtion which will reserve the resorce for UEs
 ************************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDCIFormat0AndDLInNormalSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
       )
{
    currentsubframe_g = subFrameNum;
    UInt32 availableCCE       = 0;    
    SInt32 availableDLRBCount = 0;
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    DciCCEInfo   *dciCCEInfo_p                       = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForUL_p      = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForDL_p      = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p       = PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndexForDL                = MODULO_TWO(SFN);
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay                = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
    /* CLPC_CHG */
    UInt32 maxCommonSpaceCCE = 0;
    /* CLPC_CHG END */
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    /* miValueForSubframe_g = miValueForSubframe(subFrameNum); */
    /* - TDD - Reset CCE Changes */
/* SPR 5333 changes end */

    UInt8 allocSPSFirst = 0;
    /* + SPS_TDD_Changes */
    UInt32 spsRMIndex = 0;
    UInt8 spsRBs      = 0;
    UInt8 spsArrayTti = MAX_UL_SPS_INTERVAL;
    UInt8 rbMapNum    = 0;
    /* - SPS_TDD_Changes */

    /*START : DYN_PDCCH*/
    /*Dynamic Pdcch Changes End*/
    /*END   : DYN_PDCCH*/
    UInt8 totalCCERequired = 0;
    /*START:SPR:6080*/ 
    UInt8 maxAllowedCFIValue = 0;
    /*END:SPR:6080*/ 
    /* 5492 changes start */
    UInt32 maxUeScheduledInDownlink = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                                       initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    /* 5798 changes start */
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay = currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    /* TDD Config 0 Changes Start*/
    UInt32 msg3CountContainerFor2ndSf                       = 0;
    PdcchInfo *pdcchInfo_p                                  = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]              = {PNULL};
    UInt8 maxUeScheduledInUplink[MAX_SUBFRAME]              = {0};
    UInt8 ulSubframeNumFor2ndSf                             = 0;
    /* TDD Config 0 Changes End*/
    UInt32 msg3CountContainer = (currentGlobalTickWithDelay + getUlOffset(subFrameNum,ulSubframeNum)) % 
        MAX_NUMBER_OF_CONTAINER;
    /* TDD Config 0 Changes End*/
    /*TDD Config 0 Chages Start*/
    /* In TDD Config 0 we need to schedule for two U Subframe in one D/S subframe
     * In D Subframe we have to process 
     *      Nack queue for two U Subframe
     * One for +Kth SF
     * Two for +7th SF
     *      New UE for one U Subframe
     *          One for +K SF
     ***********************************************************************/
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {   
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Set the max UE to be schedule in Uplink for UL Subframe(+7th) 
        //which will be processed in Nack Queue processing
        maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                                        initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
        //Deduct the Msg3 UE from Max Uplink UE 
        msg3CountContainerFor2ndSf = (currentGlobalTickWithDelay + ULTX_OFFSET) % 
                MAX_NUMBER_OF_CONTAINER;

        if (maxUeScheduledInUplink[ulSubframeNumFor2ndSf] >= rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf])
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = 
                maxUeScheduledInUplink[ulSubframeNumFor2ndSf] - rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf]; 
        }
        else
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = 0;
        }
        /*  No need to reset msg3ScheduleCount for ulSubframeNumFor2ndSf, as it will be used 
         *  in S Subframe as well*/

        //Initialise all the variables for Nack Info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
                subFrameNum,SFN,ulSubframeNumFor2ndSf, internalCellIndex);
        if (PNULL == pdcchInfo_p)
        {
            return;    
        }
    }
    //Set the max UE to be schedule in Uplink for UL Subframe Passed to this function(Kth Tick)
    maxUeScheduledInUplink[ulSubframeNum] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                                        initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
            subFrameNum,SFN,ulSubframeNum, internalCellIndex);
    if (PNULL == pdcchInfo_p)
    {
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    pdcchULOutputInfoForUL_p = pdcchInfo_p->pdcchULOutput_p;
    /*TDD Config 0 Chages End*/
    /*SPR 8416 Start*/
    if (maxUeScheduledInUplink[ulSubframeNum] >= rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer])
    {
        maxUeScheduledInUplink[ulSubframeNum] = maxUeScheduledInUplink[ulSubframeNum] - rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer]; 
    }
    else
    {
        maxUeScheduledInUplink[ulSubframeNum] = 0;
    }
    /*SPR 8416 End*/
    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] = 0;
    /* 5798 changes end */
    
    /* + PRS_CHANGES */
    PRSConfigInfo   *prsConfigInfo_p = PNULL;
    UInt32  pdcchOrderQCount = 0;
    UInt8 pdcchOrderPerTTICount = 0;  
    PDCCHOrderNode *pdcchOrderNode_p = PNULL; 
    /* - PRS_CHANGES */
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);

    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
       return;
    }
    /* - coverity 29871 */    

    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
    
    /* UL Output Info Container according to the U 
     * for which DCI 0 needs to be sent */ 
    /*TDD Config 0 Changes Start*/
    pdcchULOutputInfoForUL_p->pdcchTcrntiUEInfo.count = 0;
    /*TDD Config 0 Changes End*/
    /* UL Output Info container according to the U 
     * on which Ack will be recieved for DL */
    pdcchULOutputInfoIndexForDL =
        MODULO_TWO( pdcchULOutputInfoIndexForDL + 
                        getDlAckNackSfnFactor (subFrameNum, internalCellIndex));
    /* TDD Config 0 Changes Start */
    pdcchULOutputInfoForDL_p = 
     /** CA-TDD Changes Start **/
        &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndexForDL][ulSubframeNumForDlAckNack];
    /* TDD Config 0 Changes End */
       /* EICIC +*/
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNumTemp = 0;
    UInt8 absSubFrameNum = 0;
    UInt8 numBitsAbsPattern = 0;
#ifdef LTE_EMBMS_SUPPORTED
    UInt8 eMbmsCfiValue=0;   
#endif

   /* EICIC -*/ 
   
     /** CA-TDD Changes End **/
    /* ICIC changes start */
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfoForDL_p->pdcchDLUEInfo.count    = 0;
    pdcchULOutputInfoForDL_p->pdcchTcrntiUEInfo.count = 0;

    pdcchDLOutputInfo_p->countCommonChannelMsg     = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER]         = 0;   
    pdcchDLOutputInfo_p->countMsg[CE_USER]         = 0;   
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */
    /* + SPS_TDD_Changes */
    spsRMIndex = (currentGlobalTick + delayToPhy ) % MAX_SPS_INTERVAL; 
    spsRBs = spsStrTTIMap_g[internalCellIndex][spsRMIndex].numDlSpsRbsPerTti;
    /* - SPS_TDD_Changes */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START:SPR:6080*/ 
    /* SPR_9453_FIX_START */
    if (TRUE == checkForSpecialSubframe(subFrameNum, internalCellIndex))
    {
    /* SPR_9453_FIX_END */
        maxAllowedCFIValue = CFI_VALUE_SPECIAL_SUBFRAME;
    }
    else
    {
        maxAllowedCFIValue = valueCFI_g[internalCellIndex];
    }
    /*END:SPR:6080*/ 
    /*START : DYN_PDCCH*/

    /* SPR 5333 changes end */
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    availableDLRBCount  =  (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;
    /* ICIC changes start */


    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;
    /** CA-TDD Changes End **/

    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx,internalCellIndex );

    /* SPR 11230 fix start */
    /* Reset HARQ map */
    mapIdx = (currentGlobalTick + delayToPhy ) %
        MAX_HARQ_CONTAINER_MAP_SIZE;
    /* CA TDD Changes Start */
    resetEntryInHarqSentMap( mapIdx , internalCellIndex);
    /* CA TDD Changes End */
    /* SPR 11230 fix end */

     /** CA-TDD Changes Start **/
     miValueForSubframe_g[internalCellIndex] = miValueForSubframe(subFrameNum, internalCellIndex); 
    availableCCE  = maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]];
#ifdef LTE_EMBMS_SUPPORTED
    if((isEmbmsValidSf_g[internalCellIndex] == TRUE) && 
       (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
    {
        eMbmsCfiValue = \
        embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
        availableCCE  = maxCCEForCFI_g[internalCellIndex][eMbmsCfiValue-1][miValueForSubframe_g[internalCellIndex]];
    }
#endif
    

    /** CA-TDD Changes Start **/
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "subFrameNum = %u\n maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]]=\
            %u\n availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, availableCCE, availableDLRBCount,
            containerIndex);  
    /** CA-TDD Changes Stop **/

     /** CA-TDD Changes Stop **/
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch & 
       tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick, internalCellIndex );
    triggerDciForPucchPowerControl( currentGlobalTick, internalCellIndex);
    
    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU  
      CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
      that are left after allocating common space CCEs to BCCH, PCCH, RACH 
      and CCCH message */
    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g[internalCellIndex] = 0;
    /* - SPR 8190 */
 
    /* + PRS_CHANGES */
    allocateCCEsForSIPagingULOnly[internalCellIndex] = FALSE;
    prsConfigInfo_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->prsConfigInfo);
    CHECK_FOR_PRS_OCCASION(prsConfigInfo_p,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->dlBandwidth,internalCellIndex);
    /* - PRS_CHANGES */

    /*EICIC +*/
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
    
   getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);
    
        /*Calculate the SFN and SF by considering the delay to PHY*/
   subFrameNumTemp = subFrameNumTemp + delayToPhy;
	    /*Complexity Start*/
   eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNumTemp);
   numBitsAbsPattern =  eicicFindNumOfBitsinABSPattern(internalCellIndex);
	    /*Complexity End */
   isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
   CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex);
    }
    else
    {
	    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    }
       /* EICIC -*/
    /* EICIC Optimization +- */

    maxCommonSpaceCCE = allocatePdcchCommonChannel(
                            &availableCCE, 
                            &availableDLRBCount,
                            pdcchDLOutputInfo_p,
                            dciCCEContainerInfo_p,
                            delayToPhy,
                            subFrameNum,
                            SFN,
                            /* +- SPR 17777 */
                            currentGlobalTick,
                            pdcchULOutputInfoForDL_p,
                            /* +- SPR 17777 */
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink,
                            /* 5492 changes end */
                            internalCellIndex);

    /*START : DYN_PDCCH*/
    
    /* This function processes scheduleFormat3Or3AQueue to schedule 
       DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if (maxCommonSpaceCCE && (!isUeTypeInvalid_g[internalCellIndex]))
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfoForUL_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTick,
                internalCellIndex);
    }
    /* CLPC_CHG END */
    
    if (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->isDynamicPdcchDisabled) 
    {
        currentCFIValue_g[internalCellIndex] = calculateCFI(currentGlobalTickWithDelay,
                                    pdcchDLOutputInfo_p,
                                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                    pdcchULOutputInfoForUL_p,
#endif
                                    BOTH_DL_UL,
                                    /* +- SPR 17777 */
                                    &totalCCERequired,
                                    internalCellIndex);
        /*START:SPR:6080*/ 
        /* SPR_9453_FIX_START */
        if((currentCFIValue_g[internalCellIndex] == 3) && 
            (TRUE == checkForSpecialSubframe(subFrameNum, internalCellIndex )))
        {
        /* SPR_9453_FIX_END */
            currentCFIValue_g[internalCellIndex] = 2;
        }
        /*END:SPR:6080*/ 
        /*START:SPR:6080*/              
        /*CA TDD CHANGES START*/
        while(((maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE) 
              > maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]])            
              && (currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue/*3*/))
        /*CA TDD CHANGES END*/    
            /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
        {
            currentCFIValue_g[internalCellIndex] = currentCFIValue_g[internalCellIndex] + 1;
        }
        /*START:SPR:6080*/
        /*CA TDD CHANGES START*/
        while (((totalCCERequired + (maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE)) 
               > maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]]) 
               && (currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue/*3*/))
        /*CA TDD CHANGES END*/    
        /*END:SPR:6080*/ 
        {
           currentCFIValue_g[internalCellIndex]++;
        }
        /* + SPR 8190 */
        while ((highestCCENumberCommonSpace_g[internalCellIndex] != INVALID_CCE_INDEX)/** CA-TDD Changes Start **/
                && (maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] <                                    highestCCENumberCommonSpace_g[internalCellIndex])
                &&(currentCFIValue_g[internalCellIndex] <maxAllowedCFIValue ))
            /** CA-TDD Changes Stop **/
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        highestCCENumberCommonSpace_g[internalCellIndex] = 0;
        /* - SPR 8190 */

        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->dlAvailableRBs == 6 && currentCFIValue_g[internalCellIndex] < 4)
        {
            currentCFIValue_g[internalCellIndex] += 1;
            /*START:SPR:6080*/ 
            /* SPR_9453_FIX_START */
            if(TRUE == checkForSpecialSubframe(
                        subFrameNum, internalCellIndex))
            {
            /* SPR_9453_FIX_END */
                currentCFIValue_g[internalCellIndex] = 2;
            }
            /*END:SPR:6080*/ 
        }
        /* + PRS_CHANGES */
        if((0 != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->prsConfigInfo.prsSubframeCounter) &&
          (10 > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
           cellParams_p->dlAvailableRBs) && (4 == currentCFIValue_g[internalCellIndex]))
        {
            currentCFIValue_g[internalCellIndex] -= 1;
        }
        /* - PRS_CHANGES */

        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] - 
            (maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);
        /*CA TDD CHANGES END*/
        /** CA-TDD Changes Stop **/
    }
    /*END   : DYN_PDCCH*/
#ifdef LTE_EMBMS_SUPPORTED
    if((isEmbmsValidSf_g[internalCellIndex] == TRUE) &&
       (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
    {
        currentCFIValue_g[internalCellIndex] = \
        embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
		availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] - 
            (maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex] - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);
    }
#endif
    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
    /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g[internalCellIndex].totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g[internalCellIndex].totalRBs = 0;
        dlBufferPool_g[internalCellIndex].cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g[internalCellIndex].totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);
        
        if (dlBufferPool_g[internalCellIndex].cellEdgeRBs > dlBufferPool_g[internalCellIndex].totalRBs)
        {
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = dlBufferPool_g[internalCellIndex].totalRBs;
        }
    }
    excessPoolRb_g[internalCellIndex][CC_CE_REGION] = dlBufferPool_g[internalCellIndex].totalRBs;
    excessPoolRb_g[internalCellIndex][CE_REGION] = dlBufferPool_g[internalCellIndex].cellEdgeRBs;
    
    LOG_UT (MAC_PDSCH_INITIAL_BUFF_POOL_DETAILS,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),\
            dlBufferPool_g[internalCellIndex].totalRBs,\
            dlBufferPool_g[internalCellIndex].cellEdgeRBs,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
            "Initial Values of Buffer Pool");
    /* ICIC changes end */


    /* + SPS_TDD_Changes */
    /*Coverity 87558:Fix Indentation corrected*/
    spsArrayTti = (currentGlobalTick + getExactULSubFrame(subFrameNum, internalCellIndex))%MAX_UL_SPS_INTERVAL; 
    if(TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->spsSysWideParam.enableSps)
    {
        rbMapNum = (currentGlobalTick + getExactULSubFrame(subFrameNum, internalCellIndex))% getMaxULRBMaps(internalCellIndex);
        markULSpsRBsInRBMap(rbMapNum, spsArrayTti, NORMAL_LOCATION_UPDATE, 
#ifdef FDD_CONFIG
                currentGlobalTick, delayToPhy,
#endif
                subFrameNum, internalCellIndex
                );
        if ((spsRBs + RESERVE_SPS_FIRST_FACTOR*(availableDLRBCountTemp - availableDLRBCount)) 
                < availableDLRBCountTemp )
        {
            allocSPSFirst = 1;
        }
    }
    processULNackQForSpsUEinTDD(
            /*TDD Config 0 Changes Start*/
            localPdcchInfoMap,
            /*TDD Config 0 Changes End*/
            currentGlobalTick,
            &availableCCE,
            dciCCEInfo_p,
            dciCCEContainerInfo_p,
            /*TDD Config 0 Changes Start*/
            maxUeScheduledInUplink,
            /*TDD Config 0 Changes End*/
            internalCellIndex
            );
    /* - SPS_TDD_Changes */
    processULNackQinTDD(
            /*TDD Config 0 Changes Start*/
            localPdcchInfoMap,
            /*TDD Config 0 Changes End*/
            currentGlobalTick,
            &availableCCE,
            dciCCEInfo_p,
            dciCCEContainerInfo_p,
    /* 5798 changes start */
            /*TDD Config 0 Changes Start*/
            maxUeScheduledInUplink
            /*TDD Config 0 Changes End*/
    /* 5798 changes end */
            ,internalCellIndex
 );
    /* 5492 changes start */
    /* SPR 4063 Start */
    if(availableCCE)
    {
        /* + PRS_CHANGES */
        /* If it is PRS occasion then push all the entries in new  PDCCH ORDER 
         * queue in to  PDCCH ORDER advance queue.
         */ 
        if ((!allocateCCEsForSIPagingULOnly[internalCellIndex]) && (!isUeTypeInvalid_g[internalCellIndex]))
        { 
        /* Calling Function processPdcchOrderQueue which will process PDCCH
         * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
         * starting contention free rach. Only DCI 1A PDU will be sent here*/
        processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                dciCCEInfo_p, subFrameNum, currentGlobalTick,
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink,
                            /* 5492 changes end */
                            internalCellIndex);
        }
        else
        {
            pdcchOrderQCount = COUNT_PDCCH_ORDER_Q(internalCellIndex);

            /*Assign pdcchOrderPerTTICount with a value not to exceed maximum PDCCH
              Order that can be processed in one tick */
            pdcchOrderPerTTICount = maxPdcchOrderperTTI_g[internalCellIndex];
            if (pdcchOrderPerTTICount && pdcchOrderQCount)
            {
                while((pdcchOrderPerTTICount--) && (pdcchOrderQCount--))

                {
                    DEQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, (void *)&pdcchOrderNode_p, internalCellIndex);
                    if(pdcchOrderNode_p)
                    {
                        /* coverity 64943 28June2014 */
                        if ( pushNode(pdcchOrderAdvanceQueue_gp[internalCellIndex], &(pdcchOrderNode_p->anchorNode)))
                        {
                            freeMemPool(pdcchOrderNode_p);
                            /*Coverity ID <65225> Fix Changes Start*/
                            freeMemPool(pdcchDLOutputInfo_p);
                            /*Coverity ID <65225> Fix Changes End*/
                            return;
                        }
                        /* coverity 64943 28June2014 */

                        pdcchOrderNode_p = PNULL;
                    }
                }
            } 
        }

    }
    /* SPR 4063 End */
    /* 5492 changes end */

    /* ICIC changes start */

    /* + PRS_CHANGES */
    /* If it is not PRS occasion then only process this queue */
#ifndef LTE_EMBMS_SUPPORTED
		if (availableDLRBCount && availableCCE && 
		   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
		   (!isUeTypeInvalid_g[internalCellIndex]))
#else 
		if (availableDLRBCount && availableCCE && 
		   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
		   (!isUeTypeInvalid_g[internalCellIndex]) &&
		   (isEmbmsValidSf_g[internalCellIndex] == FALSE)) 
#endif

        /* ICIC changes end */
    /* - PRS_CHANGES */
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex);

    }

    if ( availableCCE )
    {   
        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */

        /* + PRS_CHANGES */
        /* If it is not PRS occasion then only process this queue */
#ifndef LTE_EMBMS_SUPPORTED
				if (availableDLRBCount && availableCCE && 
				   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
                   (!isUeTypeInvalid_g[internalCellIndex]))
#else 
				if (availableDLRBCount && availableCCE && 
				   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
				   (!isUeTypeInvalid_g[internalCellIndex]) &&
				   (isEmbmsValidSf_g[internalCellIndex] == FALSE)) 
#endif

            /* - PRS_CHANGES */
        {	
            /*Calling funtion allocatePDCCHDLNewTx funtions which will 
              process those UE whos new Transmission need to be done and then 
              allocating CCEs for them */
            allocatePDCCHDLNewTx(
                    pdcchDLOutputInfo_p,
                    &availableCCE,
                    dciCCEInfo_p,
                    &availableDLRBCount,
                    subFrameNum,
                    &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                    dciCCEContainerInfo_p,
                    /* 5492 changes start */
                    &maxUeScheduledInDownlink,
                    /* 5492 changes end */
                    internalCellIndex
                    );
		}

        /*TDD Config 0 Changes Start*/       
        /* _empty_ul_sf change start*/
        if((!isUeTypeInvalid_g[internalCellIndex]) 
         ) 
        {
            allocatePDCCHULTxInTdd(
                localPdcchInfoMap,            
                currentGlobalTickWithDelay,
                subFrameNum,
                &availableCCE ,
                dciCCEInfo_p,
                dciCCEContainerInfo_p
                ,maxUeScheduledInUplink,
                execArgs,
                internalCellIndex
                );
        }
    }
    /*TDD Config 0 Changes End*/        
    cleanUpStrategyQueueIfCCENotAvlbl(internalCellIndex, execArgs);
    emptyDLStrategyQueues(internalCellIndex);
    /*START : DYN_PDCCH*/
    /*END   : DYN_PDCCH*/
    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
#if LTE_EMBMS_SUPPORTED
    if (isEmbmsValidSf_g[internalCellIndex] == FALSE)
    {   
#endif
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex, 
	        allocSPSFirst, 
            internalCellIndex,
            subFrameNum,
            SFN);

#if LTE_EMBMS_SUPPORTED
   }
#endif
    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */
    /*TDD Config 0 Changes Start*/        
    cleanlocalPdcchInfoMap(localPdcchInfoMap);
    /*TDD Config 0 Changes End*/        

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
}


/*****************************************************************************
 * Function Name : allocPdcchPdschForDCIFormat0AndDLInSpecialSF
 * Inputs        : withMuxFlag - This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 currentGlobalTick - This stores current Global Tick.
 *                 SFN - system frame number
 *                 subFrame : This stores the subFrame Number 
 *                 ulSubframeNum - This store the uplink subframe number in TDD 
 *                 internalCellIndex - Cell-Index at MAC
 *                 ulSubframeNumForDlAckNack - UL Subframe number for DL data ack/nack
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will call funtions which will allocate CCE for both 
 *                 DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *                 PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDCIFormat0AndDLInSpecialSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		)
{
    currentsubframe_g = subFrameNum;
    UInt8 allocSPSFirst = 0;
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
    UInt32 availableCCE       = 0;    
    SInt32 availableDLRBCount = 0;
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
/* SPR 19505 fix Start */
    UInt8 cfiValue            = 0;
/* SPR 19505 fix End */
    DciCCEInfo   *dciCCEInfo_p                 = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForUL_p     = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForDL_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndexForDL                = MODULO_TWO(SFN);
    /* CLPC_CHG */
    UInt32 maxCommonSpaceCCE = 0;
    /* CLPC_CHG END */
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay                = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
    UInt8 totalCCERequired = 0;
       /* EICIC +*/
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNumTemp = 0;
    UInt8 absSubFrameNum = 0;
    UInt8 numBitsAbsPattern = 0;
   /* EICIC -*/ 
    /* + SPS_TDD_Changes */
    UInt32 spsRMIndex = 0;
    UInt8 spsRBs      = 0;
    UInt8 spsArrayTti = MAX_UL_SPS_INTERVAL;
    UInt8 rbMapNum    = 0;
    /* - SPS_TDD_Changes */

    /* TDD Config 0 Changes Start*/
    UInt32 msg3CountContainerFor2ndSf           = 0;
    PdcchInfo *pdcchInfo_p                      = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]  = {PNULL};
    UInt8 maxUeScheduledInUplink[MAX_SUBFRAME]  = {0};
    UInt8 ulSubframeNumFor2ndSf                 = 0;
    /*Nack queue count that is already scheduled in previous SF*/
    RbMapNode *rbMapNode_p = PNULL;
    UInt8 nackQCount                            = 0;
    /* TDD Config 0 Changes End*/

    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    

    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay = currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    

    /* 5492 changes start */
    UInt32 maxUeScheduledInDownlink = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                                       initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    /* 5798 changes start */
    UInt32 msg3CountContainer = (currentGlobalTickWithDelay + getExactULSubFrame(subFrameNum, internalCellIndex)) % 
                MAX_NUMBER_OF_CONTAINER;
    /* TDD Config 0 Changes Start*/
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {
        /* In TDD Config 0 we need to schedule for two U Subframe in one D/S subframe
         * In S Subframe we have to process Strategy node for two U Subframe
         * One for +Kth SF
         * Two for +7th SF   */
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Set the max UE to be schedule in Uplink for UL Subframe(+7th) 
        //which will be processed in Nack Queue processing
        maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                      initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
        //Initialise all the variables for New Ue info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
                subFrameNum,SFN,ulSubframeNumFor2ndSf, internalCellIndex);
        if (PNULL == pdcchInfo_p)
        {
            freeMemPool(pdcchDLOutputInfo_p);
            cleanlocalPdcchInfoMap(localPdcchInfoMap);
            return;    
        }

        //Deduct the Msg3 UE from Max Uplink UE 
        msg3CountContainerFor2ndSf = (currentGlobalTickWithDelay + ULTX_OFFSET) % 
            MAX_NUMBER_OF_CONTAINER;

        if (maxUeScheduledInUplink[ulSubframeNumFor2ndSf]>= rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf])
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = maxUeScheduledInUplink[ulSubframeNumFor2ndSf] 
                - rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf]; 
        }
        else
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = 0;
        }
        rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf] = 0;

    }
    //Set the max UE to be schedule in Uplink for UL Subframe Passed to this function(Kth Tick)
    maxUeScheduledInUplink[ulSubframeNum] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                      initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
            subFrameNum,SFN,ulSubframeNum, internalCellIndex);
    if (PNULL == pdcchInfo_p)
    {
        freeMemPool(pdcchDLOutputInfo_p);
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    /*TDD Config 0 Changes Start*/
    /*We need to deduct the number of UEs which are already scheduled 
     * while Nack Queue processing in last Tick*/
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {
        rbMapNode_p =rbMapNodeArr_gp[internalCellIndex] + pdcchInfo_p->rbMapNum;
        nackQCount = rbMapNode_p->nackQueueCount;
    }
    /*TDD Config 0 Changes End*/
    pdcchULOutputInfoForUL_p = pdcchInfo_p->pdcchULOutput_p;
    /* TDD Config 0 Changes End*/
    /*SPR 8416 Start*/
    if (maxUeScheduledInUplink[ulSubframeNum] >= 
            (rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] + nackQCount))
    {
        maxUeScheduledInUplink[ulSubframeNum] = maxUeScheduledInUplink[ulSubframeNum] - (rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] + nackQCount); 
    }
    else
    {
        maxUeScheduledInUplink[ulSubframeNum] = 0;
    }
    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] = 0;
    /* 5798 changes end */
    /*SPR 8416 End*/
    

/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    /* miValueForSubframe_g = miValueForSubframe(subFrameNum); */
    /* - TDD - Reset CCE Changes */
/* SPR 5333 changes end */

    /* UL Output Info Container according to the U 
     * for which DCI 0 needs to be sent */ 
    /* UL Output Info container according to the U 
     * on which Ack will be recieved for DL */
    pdcchULOutputInfoIndexForDL =
        MODULO_TWO( pdcchULOutputInfoIndexForDL + 
                        getDlAckNackSfnFactor (subFrameNum, internalCellIndex));
    /* TDD Config 0 Changes Start */
    pdcchULOutputInfoForDL_p = 
    /** CA-TDD Changes Start **/
        &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndexForDL][ulSubframeNumForDlAckNack];
    /* TDD Config 0 Changes End */
    /** CA-TDD Changes End **/
    /* ICIC changes start */
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfoForDL_p->pdcchDLUEInfo.count    = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */
    /* + SPS_TDD_Changes */
    spsRMIndex = currentGlobalTickWithDelay % MAX_SPS_INTERVAL; 
    spsRBs = spsStrTTIMap_g[internalCellIndex][spsRMIndex].numDlSpsRbsPerTti;
    /* - SPS_TDD_Changes */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START : DYN_PDCCH*/
    /* + TDD - Reset CCE Changes */
    /* availableCCE  = maxCCEForCFI_g[CFI_VALUE_SPECIAL_SUBFRAME -1][miValueForSubframe_g]; */
    /* - TDD - Reset CCE Changes */
    /*END   : DYN_PDCCH*/
     /* SPR 5333 changes end */
/* SPR 19505 fix Start */
    if (valueCFI_g[internalCellIndex]<=2) 
    {
       cfiValue = valueCFI_g[internalCellIndex];
    }
    else
    {
       cfiValue = CFI_VALUE_SPECIAL_SUBFRAME;
    }
/* SPR 19505 fix end */

    
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    availableDLRBCount  =  (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;   
    /* ICIC changes start */

    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;

    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx ,internalCellIndex);

    /* SPR 11230 fix start */
    /* Reset HARQ map */
    mapIdx = (currentGlobalTick + delayToPhy ) %
        MAX_HARQ_CONTAINER_MAP_SIZE;
    /* CA TDD Changes Start */
    resetEntryInHarqSentMap( mapIdx , internalCellIndex);
    /* CA TDD Changes End */
    /* SPR 11230 fix end */


    /* + SPR 8190 */
    highestCCENumberCommonSpace_g[internalCellIndex] = 0;
    /* - SPR 8190 */
 
    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
     miValueForSubframe_g[internalCellIndex] = miValueForSubframe(subFrameNum, internalCellIndex); 
     /** CA-TDD Changes Stop **/
    /** CA-TDD Changes Start **/
/* SPR 19505 fix Start */
    availableCCE  = maxCCEForCFI_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]];
    /** CA-TDD Changes Stop **/
    /** CA-TDD Changes Start **/
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "subFrameNum = %u\n maxCCEForCFI_g[internalCellIndex][cfiValue -1][miValueForSubframe_g[internalCellIndex]]\
            = %u\n availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, availableCCE, availableDLRBCount,
            containerIndex); 
    /** CA-TDD Changes Stop **/
/* SPR 19505 fix End */

    /* - TDD - Reset CCE Changes */
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch & 
       tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick, internalCellIndex );
    triggerDciForPucchPowerControl( currentGlobalTick, internalCellIndex );
    
    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU */
    /* CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
       that are left after allocating common space CCEs to BCCH, PCCH, RACH
       and CCCH message */

    /* EICIC +*/
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
    getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);
    
        /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNumTemp = subFrameNumTemp + delayToPhy;
	    /* Complexity Start*/
    eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNumTemp);
    numBitsAbsPattern =  eicicFindNumOfBitsinABSPattern(internalCellIndex);
	    /* Complexity End*/
 	isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
 	CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex);
    }
    else
    {
	    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    }
       /* EICIC -*/
    /* EICIC Optimization +- */


    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */
    maxCommonSpaceCCE = allocatePdcchCommonChannel(
                            &availableCCE, 
                            &availableDLRBCount,
                            pdcchDLOutputInfo_p,
                            dciCCEContainerInfo_p,
                            delayToPhy,
                            subFrameNum,
                            SFN,
                            /* +- SPR 17777 */
                            currentGlobalTick,
                            pdcchULOutputInfoForDL_p,
                            /* +- SPR 17777 */
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink,
                            /* 5492 changes end */
                            internalCellIndex);

    /*START : DYN_PDCCH*/
    
    /* This function processes scheduleFormat3Or3AQueue to schedule 
       DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if (availableCCE && (!isUeTypeInvalid_g[internalCellIndex]))
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfoForUL_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTick,
                internalCellIndex);
    }
    /* CLPC_CHG END */
    
    if (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->isDynamicPdcchDisabled)
    {
        currentCFIValue_g[internalCellIndex] = calculateCFI(currentGlobalTickWithDelay,
                /* +- SPR 17777 */
                pdcchDLOutputInfo_p,
#ifdef FDD_CONFIG
                pdcchULOutputInfoForUL_p,
#endif
                BOTH_DL_UL,
                /* +- SPR 17777 */
                 &totalCCERequired,
                 internalCellIndex);
        if(currentCFIValue_g[internalCellIndex] == 3)
        {
            /*In special subframe max value of CFI is 2*/
            currentCFIValue_g[internalCellIndex] = 2;
        }
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
/* SPR 19505 fix Start */
        while(((maxCCEInCommonSpace_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE) 
            > maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]])&& (currentCFIValue_g[internalCellIndex] < 2))/** CA-TDD Changes Stop **/
        /*CA TDD CHANGES END*/    
        {
            currentCFIValue_g[internalCellIndex] = currentCFIValue_g[internalCellIndex] + 1;
        }
        /*CA TDD CHANGES START*/
        while (((totalCCERequired + (maxCCEInCommonSpace_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE)) 
            > maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]])
        /*START:SPR:6080*/ 
            && (currentCFIValue_g[internalCellIndex] < cfiValue/*3*/))
        /*CA TDD CHANGES END*/    
/* SPR 19505 fix end */
        /*END:SPR:6080*/ 
        /** CA-TDD Changes Stop **/

        {
           currentCFIValue_g[internalCellIndex]++;
        }
        /* + SPR 8190 */
        /** CA-TDD Changes Start **/
/* SPR 19505 fix Start */
        while ((highestCCENumberCommonSpace_g[internalCellIndex] != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] < 
                highestCCENumberCommonSpace_g[internalCellIndex])  
                &&(currentCFIValue_g[internalCellIndex] < cfiValue))
/* SPR 19505 fix end */
        /** CA-TDD Changes Stop **/    
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        highestCCENumberCommonSpace_g[internalCellIndex] = 0;
        /* - SPR 8190 */



        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->dlAvailableRBs == 6)
        {
            currentCFIValue_g[internalCellIndex] = 2;
        }
       /** CA-TDD Changes Start **/
       /*CA TDD CHANGES START*/
/* SPR 19505 fix Start */
    availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] - (maxCCEInCommonSpace_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);
      /*CA TDD CHANGES END*/
/* SPR 19505 fix End */
       /** CA-TDD Changes Stop **/
    }
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
                                                         /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
                                                         /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g[internalCellIndex].totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g[internalCellIndex].totalRBs = 0;
        dlBufferPool_g[internalCellIndex].cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g[internalCellIndex].totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);
        
        if (dlBufferPool_g[internalCellIndex].cellEdgeRBs > dlBufferPool_g[internalCellIndex].totalRBs)
        {
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = dlBufferPool_g[internalCellIndex].totalRBs;
        }
    }
    excessPoolRb_g[internalCellIndex][CC_CE_REGION] = dlBufferPool_g[internalCellIndex].totalRBs;
    excessPoolRb_g[internalCellIndex][CE_REGION] = dlBufferPool_g[internalCellIndex].cellEdgeRBs;
    

    /* + SPS_TDD_Changes */
    spsArrayTti = (currentGlobalTick + getExactULSubFrame(subFrameNum, internalCellIndex))%MAX_UL_SPS_INTERVAL; 
    if(TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->spsSysWideParam.enableSps)
    {
        rbMapNum = (currentGlobalTick + getExactULSubFrame(subFrameNum, internalCellIndex))% getMaxULRBMaps(internalCellIndex);
        markULSpsRBsInRBMap(rbMapNum, spsArrayTti, NORMAL_LOCATION_UPDATE, 
#ifdef FDD_CONFIG
                currentGlobalTick, delayToPhy,
#endif
                subFrameNum, internalCellIndex
                );
        if ((spsRBs + RESERVE_SPS_FIRST_FACTOR*(availableDLRBCountTemp - availableDLRBCount)) 
                < availableDLRBCountTemp )
        {
            allocSPSFirst = 1;
        }
    }
    /*Coverity 61235: Fix Indentation corrected*/
    processULNackQForSpsUEinTDD(
            /*TDD Config 0 Changes Start*/
            localPdcchInfoMap,
            /*TDD Config 0 Changes End*/
            currentGlobalTick,
            &availableCCE,
            dciCCEInfo_p,
            dciCCEContainerInfo_p,
            /*TDD Config 0 Changes Start*/
            maxUeScheduledInUplink,
            /*TDD Config 0 Changes End*/
            internalCellIndex
            );
    /* - SPS_TDD_Changes */

    processULNackQinTDD( 
            /*TDD Config 0 Changes Start*/
            localPdcchInfoMap,
            /*TDD Config 0 Changes End*/
            currentGlobalTick,
            &availableCCE,
            dciCCEInfo_p,
            dciCCEContainerInfo_p,
            /* 5798 changes start */
            /*TDD Config 0 Changes Start*/
            maxUeScheduledInUplink
            /*TDD Config 0 Changes End*/
            /* 5798 changes end */
            ,internalCellIndex
 );
    /* 5492 changes start */
    /* SPR 4063 Start */ 
    if(availableCCE && (!isUeTypeInvalid_g[internalCellIndex]))
    {
        /* Calling Function processPdcchOrderQueue which will process PDCCH
         * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
         * starting contention free rach. Only DCI 1A PDU will be sent here*/
        processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                dciCCEInfo_p, subFrameNum, currentGlobalTick,
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink,
                            /* 5492 changes end */
                            internalCellIndex);
    }
    /* SPR 4063 End */
    /* 5492 changes end */
    /* ICIC changes start */
    if ( availableDLRBCount && availableCCE && (!isUeTypeInvalid_g[internalCellIndex])) 
        /* ICIC changes end */
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex);

    }
    if ( availableCCE )
    {   
        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */
        
        if ( availableDLRBCount && !(isUeTypeInvalid_g[internalCellIndex])) 
        {	
            /*Calling funtion allocatePDCCHDLNewTx funtions which will 
              process those UE whos new Transmission need to be done and then 
              allocating CCEs for them */
            allocatePDCCHDLNewTx(
                    pdcchDLOutputInfo_p,
                    &availableCCE,
                    dciCCEInfo_p,
                    &availableDLRBCount,
                    subFrameNum,
                    &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                    dciCCEContainerInfo_p,
                    /* 5492 changes start */
                    &maxUeScheduledInDownlink,
                    /* 5492 changes end */
                    internalCellIndex
                    );
        }
        if ((!isUeTypeInvalid_g[internalCellIndex]) 
          )
        {
            /*TDD Config 0 Changes Start*/        
            allocatePDCCHULTxInTdd(
                localPdcchInfoMap,            
                currentGlobalTickWithDelay,
                subFrameNum,
                &availableCCE ,
                dciCCEInfo_p,
                dciCCEContainerInfo_p
                /* 5798 changes start */
                ,maxUeScheduledInUplink,
                /* 5798 changes end */
                 execArgs,
                internalCellIndex
                );
         }

    }
    /*TDD Config 0 Changes End*/        
    cleanUpStrategyQueueIfCCENotAvlbl(internalCellIndex, execArgs);
    emptyDLStrategyQueues(internalCellIndex);
    /*START : DYN_PDCCH*/
    /*Dynamic Pdcch Changes End*/
    /*END   : DYN_PDCCH*/
    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst,
            internalCellIndex,
            subFrameNum,
            SFN);

    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */
    /*TDD Config 0 Changes Start*/        
    cleanlocalPdcchInfoMap(localPdcchInfoMap);
    /*TDD Config 0 Changes End*/        

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
}



/*****************************************************************************
 * Function Name : allocPdcchPdschForDCIFormat0
 * Inputs        : withMuxFlag - This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 currentGlobalTick - This stores current Global Tick.
 *                 SFN - system frame number
 *                 subFrame : This stores the subFrame Number 
 *                 ulSubframeNum - This store the uplink subframe number in TDD 
 *                 internalCellIndex - Cell-Index at MAC
 *                 ulSubframeNumForDlAckNack
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will call funtions which will allocate CCE for both 
 *                 DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *                 PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDCIFormat0(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ulSubframeNumForDlAckNack)
    /* +- SPR 17777 */
    currentsubframe_g = subFrameNum;
    UInt32 availableCCE       = 0;    
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    DciCCEInfo   *dciCCEInfo_p                 = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfo_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 allocSPSFirst=0;
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t  currentGlobalTickWithDelay         = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    /* miValueForSubframe_g = miValueForSubframe(subFrameNum); */
    /* - TDD - Reset CCE Changes */
    /* CLPC_CHG */
    /*START : DYN_PDCCH*/
    /** CA-TDD Changes Start **/
    /*CA TDD CHANGES START*/
    UInt32 maxCommonSpaceCCE = maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1][miValueForSubframe_g[internalCellIndex]];
    /*CA TDD CHANGES END*/
    /** CA-TDD Changes Stop **/
    /*END   : DYN_PDCCH*/
    /* CLPC_CHG END */
/* SPR 5333 changes end */
    UInt8 totalCCERequired = 0;
    /*START:SPR:6080*/ 
    UInt8 maxAllowedCFIValue = 0;
    /*END:SPR:6080*/ 
    /* + SPS_TDD_Changes */
    UInt8 spsArrayTti = MAX_SPS_INTERVAL;
    UInt8  rbMapNum = 0;
    /* - SPS_TDD_Changes */
    /* TDD Config 0 Changes Start*/
    UInt32 msg3CountContainerFor2ndSf                           = 0;
    PdcchInfo *pdcchInfo_p                                      = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]                  = {PNULL};
    UInt8 maxUeScheduledInUplink[MAX_SUBFRAME]              = {0};
    UInt8 ulSubframeNumFor2ndSf                             = 0;
    /*Nack queue count that is already scheduled in previous SF*/
    RbMapNode *rbMapNode_p = PNULL;
    UInt8 nackQCount                                        = 0;
    /* TDD Config 0 Changes End*/
       /* EICIC +*/
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNumTemp = 0;
    UInt8 absSubFrameNum = 0;
    UInt8 numBitsAbsPattern = 0;
   /* EICIC -*/ 


    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    

    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay =currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
       /* 5798 changes start */
    UInt32 msg3CountContainer = (currentGlobalTickWithDelay + getExactULSubFrame(subFrameNum, internalCellIndex)) % 
                MAX_NUMBER_OF_CONTAINER;
    /*SPR 8416 Start*/
    /* TDD Config 0 Changes Start*/
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Set the max UE to be schedule in Uplink for UL Subframe(+7th) 
        //which will be processed in Nack Queue processing
        maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                        initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
        /* In TDD Config 0 we need to schedule for two U Subframe in one D/S subframe
         * In S Subframe we have to process Strategy node for two U Subframe
         * One for +Kth SF
         * Two for +7th SF   */
        //Initialise all the variables for New Ue  and Nack Queue info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
                subFrameNum,SFN,ulSubframeNumFor2ndSf, internalCellIndex);
        if (PNULL == pdcchInfo_p)
        {
            freeMemPool(pdcchDLOutputInfo_p);
            cleanlocalPdcchInfoMap(localPdcchInfoMap);
            return;    
        }
        //Deduct the Msg3 UE from Max Uplink UE 
        msg3CountContainerFor2ndSf = (currentGlobalTickWithDelay + ULTX_OFFSET) % 
            MAX_NUMBER_OF_CONTAINER;

        if (maxUeScheduledInUplink[ulSubframeNumFor2ndSf] >= rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf])
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = maxUeScheduledInUplink[ulSubframeNumFor2ndSf]
                - rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf]; 
        }
        else
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = 0;
        }

        rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainerFor2ndSf] = 0;
    }
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
            subFrameNum,SFN,ulSubframeNum, internalCellIndex);
    if (PNULL == pdcchInfo_p)
    {
        freeMemPool(pdcchDLOutputInfo_p);
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    /* TDD Config 0 Changes Start*/
    maxUeScheduledInUplink[ulSubframeNum] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                        initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    /*We need to deduct the number of UEs which are already scheduled 
     * while Nack Queue processing in last Tick*/
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {
        rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + pdcchInfo_p->rbMapNum;
        nackQCount = rbMapNode_p->nackQueueCount;
    }
    pdcchULOutputInfo_p = pdcchInfo_p->pdcchULOutput_p;
    if (maxUeScheduledInUplink[ulSubframeNum] >= 
            (rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] + nackQCount))
    {
        maxUeScheduledInUplink[ulSubframeNum] = maxUeScheduledInUplink[ulSubframeNum] - 
            (rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] + nackQCount); 
    }
    else
    {
        maxUeScheduledInUplink[ulSubframeNum] = 0;
    }
    /*SPR 8416 End*/
    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] = 0;
    /* 5798 changes end */
    
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START:SPR:6080*/ 
    /* SPR_9453_FIX_START */
/* SPR 19505 fix Start */
    if (TRUE == checkForSpecialSubframe(
                subFrameNum, internalCellIndex ) && (3 == valueCFI_g[internalCellIndex]))
    {
/* SPR 19505 fix End */
    /* SPR_9453_FIX_END */
        maxAllowedCFIValue = CFI_VALUE_SPECIAL_SUBFRAME; 
    }
    else
    {
        maxAllowedCFIValue = valueCFI_g[internalCellIndex];
    }
    /*END:SPR:6080*/ 
    /*START : DYN_PDCCH*/
    /* + TDD - Reset CCE Changes */
    /* availableCCE  = maxCCEForCFI_g[maxAllowedCFIValue-1][miValueForSubframe_g]; */
    /* - TDD - Reset CCE Changes */
    /*END   : DYN_PDCCH*/
/* SPR 5333 changes end */    
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
        /* ICIC changes start */
    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;

    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx,internalCellIndex );

    /* SPR 11230 fix start */
    /* Reset HARQ map */
    mapIdx = (currentGlobalTick + delayToPhy ) %
        MAX_HARQ_CONTAINER_MAP_SIZE;
    /* CA TDD Changes Start */
    resetEntryInHarqSentMap( mapIdx , internalCellIndex);
    /* CA TDD Changes End */
    /* SPR 11230 fix end */

    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
     miValueForSubframe_g[internalCellIndex] = miValueForSubframe(subFrameNum, internalCellIndex); 
     /** CA-TDD Changes Stop **/
     /** CA-TDD Changes Start **/
     availableCCE  = maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]];
     /** CA-TDD Changes Start **/
     LTE_MAC_UT_LOG( LOG_CRITICAL, PDCCH_MGR,
             "subFrameNum = %u\n  maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]]= %u\n \
             availableDLRBCount = %d\n \
             containerIndex = %d \n",subFrameNum, availableCCE, (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB,
             containerIndex);  
     /** CA-TDD Changes Stop **/

     /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick, internalCellIndex );
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g[internalCellIndex] = 0;
    /* - SPR 8190 */

    /* EICIC +*/
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
    getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);
    
        /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNumTemp = subFrameNumTemp + delayToPhy;
    /*Complexity Start*/
    eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNumTemp);
    numBitsAbsPattern =  eicicFindNumOfBitsinABSPattern(internalCellIndex);
    /*Complexity End*/
 	isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex);
    }
    else
    {
	    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    }
    /* EICIC Optimization +- */
       /* EICIC -*/

    /* This function processes scheduleFormat3Or3AQueue to schedule 
    DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if (maxCommonSpaceCCE &&  (!isUeTypeInvalid_g[internalCellIndex]))
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfo_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTick,
                internalCellIndex);
    }
    /* CLPC_CHG END */

    /*START : DYN_PDCCH*/
    if (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->isDynamicPdcchDisabled)
    {
        currentCFIValue_g[internalCellIndex] = calculateCFI(currentGlobalTickWithDelay,
                                    pdcchDLOutputInfo_p,
                                    /* +- SPR 17777 */

#ifdef FDD_CONFIG
                                   pdcchULOutputInfo_p,
#endif
                                   ONLY_UL,
                                   /* +- SPR 17777 */
                                   &totalCCERequired,
                                   internalCellIndex);
        /*START:SPR:6080*/ 
        /* SPR_9453_FIX_START */
        if((currentCFIValue_g[internalCellIndex] == 3) &&
             (TRUE == checkForSpecialSubframe(subFrameNum, internalCellIndex )))
        {
        /* SPR_9453_FIX_END */
            currentCFIValue_g[internalCellIndex] = 2;
        }
        /*END:SPR:6080*/ 
        /*START:SPR:6080*/          /** CA-TDD Changes Start **/ 
        /*CA TDD CHANGES START*/
        while(((maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE) 
               > maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]])
              && (currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue/*3*/)) /** CA-TDD Changes Stop **/
        /*CA TDD CHANGES END*/      
        /*END:SPR:6080*/ 
        {
            currentCFIValue_g[internalCellIndex] = currentCFIValue_g[internalCellIndex] + 1;
        }
        /*START:SPR:6080*/
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        while (((totalCCERequired + (maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE)) 
               > maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]]) 
               && (currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue/*3*/))
        /*CA TDD CHANGES END*/    
        /*END:SPR:6080*/ /** CA-TDD Changes Stop **/
        {
           currentCFIValue_g[internalCellIndex]++;
        }
        /* + SPR 8190 */
        /** CA-TDD Changes Start **/
        while ((highestCCENumberCommonSpace_g[internalCellIndex] != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] < 
                highestCCENumberCommonSpace_g[internalCellIndex])
                &&(currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue))
        /** CA-TDD Changes Stop **/
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        highestCCENumberCommonSpace_g[internalCellIndex] = 0;
        /* - SPR 8190 */


        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->dlAvailableRBs == 6 && currentCFIValue_g[internalCellIndex] < 4)
        {
            currentCFIValue_g[internalCellIndex] += 1;
            /*START:SPR:6080*/ 
            /* SPR_9453_FIX_START */
            if(TRUE == checkForSpecialSubframe(
                        subFrameNum, internalCellIndex))
            {
            /* SPR_9453_FIX_END */
                currentCFIValue_g[internalCellIndex] = 2;
            }
            /*END:SPR:6080*/ 
        }
        /** CA-TDD Changes Start **/
        availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] - 
        /*START:SPR:6080*/ 
        /*CA TDD CHANGES START*/
                   (maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);
         /*CA TDD CHANGES END*/          
        /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
    }
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
    /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */

    /* + SPS_TDD_Changes */
    spsArrayTti = (currentGlobalTick + getExactULSubFrame(subFrameNum, internalCellIndex))%MAX_UL_SPS_INTERVAL; 
    if(TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->spsSysWideParam.enableSps)
    {
        rbMapNum = (currentGlobalTick + getExactULSubFrame(subFrameNum, internalCellIndex))% getMaxULRBMaps(internalCellIndex);
        markULSpsRBsInRBMap(rbMapNum, spsArrayTti, NORMAL_LOCATION_UPDATE,
#ifdef FDD_CONFIG
                currentGlobalTick, delayToPhy,
#endif
                subFrameNum, internalCellIndex
                );
    }
    /*Coverity 61235: Fix Indentation corrected*/
    processULNackQForSpsUEinTDD(
            /*TDD Config 0 Changes Start*/
            localPdcchInfoMap,
            /*TDD Config 0 Changes End*/
            currentGlobalTick,
            &availableCCE,
            dciCCEInfo_p,
            dciCCEContainerInfo_p,
            /*TDD Config 0 Changes Start*/
            maxUeScheduledInUplink,
            /*TDD Config 0 Changes End*/
            internalCellIndex
            );
    /* - SPS_TDD_Changes */

    processULNackQinTDD(
            /*TDD Config 0 Changes Start*/
            localPdcchInfoMap,
            /*TDD Config 0 Changes End*/
            currentGlobalTick,
            &availableCCE,
            dciCCEInfo_p,
            dciCCEContainerInfo_p,
            /* 5798 changes start */
            maxUeScheduledInUplink
            /* 5798 changes end */
            ,internalCellIndex
 );


    if ( availableCCE )
    {   
        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */
        /*TDD Config 0 Changes Start*/        
        allocatePDCCHULTxInTdd(
                localPdcchInfoMap,            
                currentGlobalTickWithDelay,
                subFrameNum,
                &availableCCE ,
                dciCCEInfo_p,
                dciCCEContainerInfo_p
                ,maxUeScheduledInUplink,
                execArgs,
                /*TDD Config 0 Changes End*/        
                internalCellIndex
                );
    }
    /*TDD Config 0 Changes End*/        
    cleanUpStrategyQueueIfCCENotAvlbl(internalCellIndex, execArgs);

    /*START : DYN_PDCCH*/
    /*END   : DYN_PDCCH*/
    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst,
            internalCellIndex,
            subFrameNum,
            SFN);

    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */
    /*TDD Config 0 Changes Start*/        
    cleanlocalPdcchInfoMap(localPdcchInfoMap);
    /*TDD Config 0 Changes End*/        

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);

}
#else /*DL_UL_SPLIT_TDD START*/
/*****************************************************************************
 * Function Name : allocPdcchPdschForDCIFormat0AndDLInNormalSF
 * INPUTS :1)withMuxFlag-This when set to one Indicates that we need to call
 *           directly invoke Mux Execution Logic.
 *         2)flagToTriggerPUSCH - This will be used to trigger PUSCH 
 *           functionality once PDCCH complete it processing.
 *         3)delayToPhy-This stores delay from current module to PHY
 *         4)currentGlobalTick-This stores current Global Tick.
 *         ulSubframeNumForDlAckNack
 * Outputs :None
 * Returns : None
 * Description:This funtion will call funtions which will allocate CCE for both 
 *             DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *             PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDCIFormat0AndDLInNormalSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
       )
{
    currentsubframe_g = subFrameNum;
    UInt32 availableCCE       = 0;    
    SInt32 availableDLRBCount = 0;
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    DciCCEInfo   *dciCCEInfo_p                       = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForUL_p      = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForDL_p      = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p       = PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndexForDL                = MODULO_TWO(SFN);
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay                = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
    /* CLPC_CHG */
    UInt32 maxCommonSpaceCCE = 0;
    /* CLPC_CHG END */
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    miValueForSubframe_g = miValueForSubframe(subFrameNum);
    /* - TDD - Reset CCE Changes */
/* SPR 5333 changes end */

    UInt8 allocSPSFirst = 0;

    /* 3Mhz + Dyn PDCCH Fix Start*/
    UInt8 totalCCERequired = 0;
    /* 3Mhz + Dyn PDCCH Fix End*/
    /*START:SPR:6080*/ 
    UInt8 maxAllowedCFIValue = 0;
    /*END:SPR:6080*/ 
    /* 5492 changes start */
    UInt32 maxUeScheduledInDownlink = dlSchdConfig_g.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    /* TDD Config 0 Changes Start*/
    PdcchInfo *pdcchInfo_p                                  = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]              = {PNULL};
    UInt8 ulSubframeNumFor2ndSf                             = 0;
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {   
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Initialise all the variables for Nack Info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
                subFrameNum,SFN,ulSubframeNumFor2ndSf);
        if (PNULL == pdcchInfo_p)
        {
            return;    
        }
    }
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
            subFrameNum,SFN,ulSubframeNum);
    if (PNULL == pdcchInfo_p)
    {
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    pdcchULOutputInfoForUL_p = pdcchInfo_p->pdcchULOutput_p;
    /*TDD Config 0 Chages End*/
    /* + PRS_CHANGES */
    PRSConfigInfo   *prsConfigInfo_p = PNULL;
    UInt32  pdcchOrderQCount = 0;
    UInt8 pdcchOrderPerTTICount = 0;  
    PDCCHOrderNode *pdcchOrderNode_p = PNULL; 
    /* - PRS_CHANGES */

    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);

    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay = currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
    
    /* UL Output Info Container according to the U 
     * for which DCI 0 needs to be sent */ 
    /* ICIC changes start */
    pdcchULOutputInfoForUL_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfoForUL_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    /* SPR 1115 CHG */    
    pdcchULOutputInfoForUL_p->pdcchTcrntiUEInfo.count = 0;
    pdcchULOutputInfoForUL_p->pdcchDLUEInfo.count    = 0;

    /* UL Output Info container according to the U 
     * on which Ack will be recieved for DL */
    pdcchULOutputInfoIndexForDL =
        MODULO_TWO( pdcchULOutputInfoIndexForDL + 
                        getDlAckNackSfnFactor (subFrameNum));
    /* TDD Config 0 Changes Start */
    pdcchULOutputInfoForDL_p = 
        &pdcchULOutputInfo_g[pdcchULOutputInfoIndexForDL][ulSubframeNumForDlAckNack];
    /* TDD Config 0 Changes End */
    /* ICIC changes start */
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfoForDL_p->pdcchDLUEInfo.count    = 0;
    pdcchULOutputInfoForDL_p->pdcchTcrntiUEInfo.count = 0;

    pdcchDLOutputInfo_p->countCommonChannelMsg     = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER]         = 0;   
    pdcchDLOutputInfo_p->countMsg[CE_USER]         = 0;   
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START:SPR:6080*/ 
/* SPR 19505 fix Start */
    if ((subFrameNum == 6) && (3 == valueCFI_g[internalCellIndex]))
    {
        maxAllowedCFIValue = CFI_VALUE_SPECIAL_SUBFRAME;
    }
    else
    {
        maxAllowedCFIValue = valueCFI_g[internalCellIndex];
    }
    /*END:SPR:6080*/ 
/* SPR 19505 fix End */

    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "subFrameNum = %u\n maxCCEAvailable_g[miValueForSubframe_g]= %u\n availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, availableCCE, availableDLRBCount,
            containerIndex);  
/* SPR 5333 changes end */
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    availableDLRBCount  =  dlRAInfo_g[dlRAInfoIndex_g].totalAllowedRB;   
    /* ICIC changes start */


    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = &dciCCEContainerInfo_g[containerIndex];
    
    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p);

    /* + TDD - Reset CCE Changes */
    availableCCE  = maxCCEForCFI_g[maxAllowedCFIValue-1][miValueForSubframe_g];
    /* - TDD - Reset CCE Changes */
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch & 
       tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick );
    triggerDciForPucchPowerControl( currentGlobalTick );
    
    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU  
      CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
      that are left after allocating common space CCEs to BCCH, PCCH, RACH 
      and CCCH message */
    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g = 0;
    /* - SPR 8190 */

    /* + PRS_CHANGES */
    allocateCCEsForSIPagingULOnly = FALSE;
    prsConfigInfo_p = &(cellConfig_g.prsConfigInfo);
    CHECK_FOR_PRS_OCCASION(prsConfigInfo_p,cellConfig_g.dlBandwidth);
    /* - PRS_CHANGES */
    maxCommonSpaceCCE = allocatePdcchCommonChannel(
                            &availableCCE, 
                            &availableDLRBCount,
                            pdcchDLOutputInfo_p,
                            dciCCEContainerInfo_p,
                            delayToPhy,
                            subFrameNum,
                            SFN,
                            /* +- SPR 17777 */
                            currentGlobalTick,
                            pdcchULOutputInfoForDL_p,
                            /* +- SPR 17777 */
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink);
                            /* 5492 changes end */

    /*START : DYN_PDCCH*/
    
    /* This function processes scheduleFormat3Or3AQueue to schedule 
       DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if (maxCommonSpaceCCE)
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfoForUL_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTick);
    }
    /* CLPC_CHG END */
    
    if (!cellConfig_g.isDynamicPdcchDisabled) 
    {
        currentCFIValue_g = calculateCFI(currentGlobalTickWithDelay,
                                    pdcchDLOutputInfo_p,
                                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                    pdcchULOutputInfoForUL_p,
#endif
                                    BOTH_DL_UL,
                                    /* +- SPR 17777 */
                                    /* 3Mhz + Dyn PDCCH Fix Start*/
                                    &totalCCERequired);
                                    /* 3Mhz + Dyn PDCCH Fix End*/
        /*START:SPR:6080*/ 
        if((currentCFIValue_g == 3) && (subFrameNum == 6))
        {
            currentCFIValue_g = 2;
        }
        /*END:SPR:6080*/ 
        /*START:SPR:6080*/ 
        while(((maxCCEInCommonSpace_g[maxAllowedCFIValue-1][miValueForSubframe_g] - maxCommonSpaceCCE) 
              > maxCCEInCommonSpace_g[currentCFIValue_g-1][miValueForSubframe_g])
              && (currentCFIValue_g < maxAllowedCFIValue/*3*/))
        /*END:SPR:6080*/ 
        {
            currentCFIValue_g = currentCFIValue_g + 1;
        }
        /* 3Mhz + Dyn PDCCH Fix Start*/
        /*START:SPR:6080*/ 
        while (((totalCCERequired + (maxCCEInCommonSpace_g[maxAllowedCFIValue - 1][miValueForSubframe_g] - maxCommonSpaceCCE)) 
               > maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g]) 
               && (currentCFIValue_g < maxAllowedCFIValue/*3*/))
        /*END:SPR:6080*/ 
        {
           currentCFIValue_g++;
        }
        /* + SPR 8190 */
        while ((highestCCENumberCommonSpace_g != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g] < highestCCENumberCommonSpace_g)
                &&(currentCFIValue_g <maxAllowedCFIValue ))
        {
                currentCFIValue_g++;
        }
        highestCCENumberCommonSpace_g = 0;
        /* - SPR 8190 */

        /* 3Mhz + Dyn PDCCH Fix End*/
        if(cellConfig_g.dlAvailableRBs == 6 && currentCFIValue_g < 4)
        {
            currentCFIValue_g += 1;
            /*START:SPR:6080*/ 
            if(subFrameNum == 6)
            {
                currentCFIValue_g = 2;
            }
            /*END:SPR:6080*/ 
        }
        /* + PRS_CHANGES */
        if((0 != cellConfig_g.prsConfigInfo.prsSubframeCounter) &&
          (10 > cellConfig_g.dlAvailableRBs) && (4 == currentCFIValue_g))
        {
            currentCFIValue_g -= 1;
        }
        /* - PRS_CHANGES */

        availableCCE = maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g] - 
                     (maxCCEInCommonSpace_g[maxAllowedCFIValue - 1][miValueForSubframe_g] - maxCommonSpaceCCE);
    }
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g;
    /* + TDD - Reset CCE Changes */
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g;
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g.totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g.totalRBs = 0;
        dlBufferPool_g.cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g.totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);
        
        if (dlBufferPool_g.cellEdgeRBs > dlBufferPool_g.totalRBs)
        {
            dlBufferPool_g.cellEdgeRBs = dlBufferPool_g.totalRBs;
        }
    }
    excessPoolRb_g[CC_CE_REGION] = dlBufferPool_g.totalRBs;
    excessPoolRb_g[CE_REGION] = dlBufferPool_g.cellEdgeRBs;
    
    LOG_UT (MAC_PDSCH_INITIAL_BUFF_POOL_DETAILS,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),\
            dlBufferPool_g.totalRBs,\
            dlBufferPool_g.cellEdgeRBs,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
            "Initial Values of Buffer Pool");
    /* ICIC changes end */
    
    /* 5492 changes start */
    /* SPR 4063 Start */
    if(availableCCE)
    {
        /* + PRS_CHANGES */
        /* If it is PRS occasion then push all the entries in new  PDCCH ORDER 
         * queue in to  PDCCH ORDER advance queue.
         */ 
        if (!allocateCCEsForSIPagingULOnly)
        { 
        /* Calling Function processPdcchOrderQueue which will process PDCCH
         * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
         * starting contention free rach. Only DCI 1A PDU will be sent here*/
        processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                dciCCEInfo_p, subFrameNum, currentGlobalTick,
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink);
                            /* 5492 changes end */
        }
        else
        {
            pdcchOrderQCount = COUNT_PDCCH_ORDER_Q();

            /*Assign pdcchOrderPerTTICount with a value not to exceed maximum PDCCH
              Order that can be processed in one tick */
            pdcchOrderPerTTICount = maxPdcchOrderperTTI_g;
            if (pdcchOrderPerTTICount && pdcchOrderQCount)
            {
                while((pdcchOrderPerTTICount--) && (pdcchOrderQCount--))

                {
                    DEQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, (void *)&pdcchOrderNode_p);
                    if(pdcchOrderNode_p)
                    {
                        pushNode(&pdcchOrderAdvanceQueue_g, &(pdcchOrderNode_p->anchorNode));
                        pdcchOrderNode_p = PNULL;
                    }
                }
            } 
        }

    }
    /* SPR 4063 End */
    /* 5492 changes end */

    /* ICIC changes start */

    /* + PRS_CHANGES */
    /* If it is not PRS occasion then only process this queue */
    if ( (!allocateCCEsForSIPagingULOnly) && availableDLRBCount && availableCCE) 
        /* ICIC changes end */
    /* - PRS_CHANGES */
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink);
                /* 5492 changes end */

    }

    if ( availableCCE )
    {   
        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */

        /* + PRS_CHANGES */
        /* If it is not PRS occasion then only process this queue */
        if ( (!allocateCCEsForSIPagingULOnly) && availableDLRBCount )
            /* - PRS_CHANGES */
        {	
            /*Calling funtion allocatePDCCHDLNewTx funtions which will 
              process those UE whos new Transmission need to be done and then 
              allocating CCEs for them */
            allocatePDCCHDLNewTx(
                    pdcchDLOutputInfo_p,
                    &availableCCE,
                    dciCCEInfo_p,
                    &availableDLRBCount,
                    subFrameNum,
                    &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                    dciCCEContainerInfo_p,
                    /* 5492 changes start */
                    &maxUeScheduledInDownlink);
                    /* 5492 changes end */
		}

    }
    emptyDLStrategyQueues();
    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex, 
            allocSPSFirst, 
            subFrameNum,
            SFN);

    putEntryInUlPdcchSignalQueue(availableCCE);
    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
}


/*****************************************************************************
 * Function Name : allocPdcchPdschForDCIFormat0AndDLInSpecialSF
 * INPUTS :1)withMuxFlag-This when set to one Indicates that we need to call
 *           directly invoke Mux Execution Logic.
 *         2)flagToTriggerPUSCH - This will be used to trigger PUSCH 
 *           functionality once PDCCH complete it processing.
 *         3)delayToPhy-This stores delay from current module to PHY
 *         4)currentGlobalTick-This stores current Global Tick.
 *         ulSubframeNumForDlAckNack
 * Outputs :None
 * Returns : None
 * Description:This funtion will call funtions which will allocate CCE for both 
 *             DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *             PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDCIFormat0AndDLInSpecialSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		)
{
    currentsubframe_g = subFrameNum;
    UInt8 allocSPSFirst = 0;
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
    UInt32 availableCCE       = 0;    
    SInt32 availableDLRBCount = 0;
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    DciCCEInfo   *dciCCEInfo_p                 = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForUL_p     = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfoForDL_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndexForDL                = MODULO_TWO(SFN);
    /* CLPC_CHG */
    UInt32 maxCommonSpaceCCE = 0;
    /* CLPC_CHG END */
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t  currentGlobalTickWithDelay         = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
    /* 3Mhz + Dyn PDCCH Fix Start*/
    UInt8 totalCCERequired = 0;
    /* 3Mhz + Dyn PDCCH Fix End*/

    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay = currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    

    /* 5492 changes start */
    UInt32 maxUeScheduledInDownlink = dlSchdConfig_g.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    /* TDD Config 0 Changes Start*/
    PdcchInfo *pdcchInfo_p                                  = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]              = {PNULL};
    UInt8 ulSubframeNumFor2ndSf                             = 0;
    if (UL_DL_CONFIG_0 == cellConfig_g.subFrameAssign)
    {   
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Initialise all the variables for Nack Info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
                subFrameNum,SFN,ulSubframeNumFor2ndSf);
        if (PNULL == pdcchInfo_p)
        {
            return;    
        }
    }
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
            subFrameNum,SFN,ulSubframeNum);
    if (PNULL == pdcchInfo_p)
    {
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    pdcchULOutputInfoForUL_p = pdcchInfo_p->pdcchULOutput_p;
    /*TDD Config 0 Chages End*/

/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    miValueForSubframe_g = miValueForSubframe(subFrameNum);
    /* - TDD - Reset CCE Changes */
/* SPR 5333 changes end */

    /* UL Output Info Container according to the U 
     * for which DCI 0 needs to be sent */ 
    /* ICIC changes start */
    pdcchULOutputInfoForUL_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfoForUL_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    /*SPR 1115 CHG*/
    pdcchULOutputInfoForUL_p->pdcchDLUEInfo.count    = 0;

    /* UL Output Info container according to the U 
     * on which Ack will be recieved for DL */
    pdcchULOutputInfoIndexForDL =
        MODULO_TWO( pdcchULOutputInfoIndexForDL + 
                        getDlAckNackSfnFactor (subFrameNum));
    /* TDD Config 0 Changes Start */
    pdcchULOutputInfoForDL_p = 
        &pdcchULOutputInfo_g[pdcchULOutputInfoIndexForDL][ulSubframeNumForDlAckNack];
    /* TDD Config 0 Changes End */
    /* ICIC changes start */
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfoForDL_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfoForDL_p->pdcchDLUEInfo.count    = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START : DYN_PDCCH*/
    /* + TDD - Reset CCE Changes */
    /*availableCCE  = maxCCEForCFI_g[CFI_VALUE_SPECIAL_SUBFRAME -1][miValueForSubframe_g];*/
    /* - TDD - Reset CCE Changes */
    /*END   : DYN_PDCCH*/
/* SPR 19505 fix Start */
    if (valueCFI_g[internalCellIndex]<=2) 
    {
       cfiValue = valueCFI_g[internalCellIndex];
    }
    else
    {
       cfiValue = CFI_VALUE_SPECIAL_SUBFRAME;
    }
/* SPR 19505 fix end */

    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "subFrameNum = %u\n maxCCEAvailable_g[miValueForSubframe_g]= %u\n availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, availableCCE, availableDLRBCount,
            containerIndex); 
 /* SPR 5333 changes end */
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    availableDLRBCount  =  dlRAInfo_g[dlRAInfoIndex_g].totalAllowedRB;   
    /* ICIC changes start */

    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = &dciCCEContainerInfo_g[containerIndex];
    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p);
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g = 0;
    /* - SPR 8190 */

    /* + TDD - Reset CCE Changes */
/* SPR 19505 fix Start */
    availableCCE  = maxCCEForCFI_g[cfiValue - 1][miValueForSubframe_g];
/* SPR 19505 fix End */
    /* - TDD - Reset CCE Changes */
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch & 
       tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick );
    triggerDciForPucchPowerControl( currentGlobalTick );
    
    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU */
    /* CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
       that are left after allocating common space CCEs to BCCH, PCCH, RACH
       and CCCH message */
    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */
    maxCommonSpaceCCE = allocatePdcchCommonChannel(
                            &availableCCE, 
                            &availableDLRBCount,
                            pdcchDLOutputInfo_p,
                            dciCCEContainerInfo_p,
                            delayToPhy,
                            subFrameNum,
                            SFN,
                            /* +- SPR 17777 */
                            currentGlobalTick,
                            pdcchULOutputInfoForDL_p,
                            /* +- SPR 17777 */
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink);
                            /* 5492 changes end */

    /*START : DYN_PDCCH*/
    
    /* This function processes scheduleFormat3Or3AQueue to schedule 
       DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if (availableCCE)
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfoForUL_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTick);
    }
    /* CLPC_CHG END */
    
    if (!cellConfig_g.isDynamicPdcchDisabled)
    {
        currentCFIValue_g = calculateCFI(currentGlobalTickWithDelay,
                /* +- SPR 17777 */
                pdcchDLOutputInfo_p,
#ifdef FDD_CONFIG
                pdcchULOutputInfoForUL_p,
#endif
                BOTH_DL_UL,
                /* +- SPR 17777 */
               /* 3Mhz + Dyn PDCCH Fix Start*/
                 &totalCCERequired);
               /* 3Mhz + Dyn PDCCH Fix End*/
        if(currentCFIValue_g == 3)
        {
            /*In special subframe max value of CFI is 2*/
            currentCFIValue_g = 2;
        }
/* SPR 19505 fix Start */
        while(((maxCCEInCommonSpace_g[cfiValue-1][miValueForSubframe_g] - maxCommonSpaceCCE) 
            > maxCCEInCommonSpace_g[currentCFIValue_g-1][miValueForSubframe_g])&& (currentCFIValue_g < 2))
        {
            currentCFIValue_g = currentCFIValue_g + 1;
        }
        /* 3Mhz + Dyn PDCCH Fix Start*/
        while (((totalCCERequired + (maxCCEInCommonSpace_g[cfiValue-1][miValueForSubframe_g] - maxCommonSpaceCCE)) 
            > maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g]) 
        /*START:SPR:6080*/ 
            && (currentCFIValue_g < cfiValue/*3*/))
        /*END:SPR:6080*/ 

        {
           currentCFIValue_g++;
        }
        /* 3Mhz + Dyn PDCCH Fix End*/
        /* + SPR 8190 */
        while ((highestCCENumberCommonSpace_g != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g] < highestCCENumberCommonSpace_g)
                &&(currentCFIValue_g < cfiValue))
/* SPR 19505 fix End */
        {
            currentCFIValue_g++;
        }
        highestCCENumberCommonSpace_g = 0;
        /* - SPR 8190 */

        if(cellConfig_g.dlAvailableRBs == 6)
        {
            currentCFIValue_g = 2;
        }
/* SPR 19505 fix Start */
    availableCCE = maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g] - (maxCCEInCommonSpace_g[cfiValue-1][miValueForSubframe_g] - maxCommonSpaceCCE);
/* SPR 19505 fix End */
    }
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g;
    /* + TDD - Reset CCE Changes */
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g;
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g.totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g.totalRBs = 0;
        dlBufferPool_g.cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g.totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);
        
        if (dlBufferPool_g.cellEdgeRBs > dlBufferPool_g.totalRBs)
        {
            dlBufferPool_g.cellEdgeRBs = dlBufferPool_g.totalRBs;
        }
    }
    excessPoolRb_g[CC_CE_REGION] = dlBufferPool_g.totalRBs;
    excessPoolRb_g[CE_REGION] = dlBufferPool_g.cellEdgeRBs;
    
    /* 5492 changes start */
    /* SPR 4063 Start */ 
    if(availableCCE)
    {
        /* Calling Function processPdcchOrderQueue which will process PDCCH
         * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
         * starting contention free rach. Only DCI 1A PDU will be sent here*/
        processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                dciCCEInfo_p, subFrameNum, currentGlobalTick,
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink);
                            /* 5492 changes end */
    }
    /* SPR 4063 End */
    /* 5492 changes end */
    /* ICIC changes start */
    if ( availableDLRBCount && availableCCE) 
        /* ICIC changes end */
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink);
                /* 5492 changes end */

    }
    if ( availableCCE )
    {   
        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */
        
        if ( availableDLRBCount ) 
        {	
            /*Calling funtion allocatePDCCHDLNewTx funtions which will 
              process those UE whos new Transmission need to be done and then 
              allocating CCEs for them */
            allocatePDCCHDLNewTx(
                    pdcchDLOutputInfo_p,
                    &availableCCE,
                    dciCCEInfo_p,
                    &availableDLRBCount,
                    subFrameNum,
                    &pdcchULOutputInfoForDL_p->pdcchDLUEInfo,
                    dciCCEContainerInfo_p,
                    /* 5492 changes start */
                    &maxUeScheduledInDownlink);
                    /* 5492 changes end */
        }
   
    }

    emptyDLStrategyQueues();
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst,
            subFrameNum,
            SFN);

    putEntryInUlPdcchSignalQueue(availableCCE);

    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
}



/*****************************************************************************
 * Function Name : allocPdcchPdschForDCIFormat0
 * INPUTS :1)withMuxFlag-This when set to one Indicates that we need to call
 *           directly invoke Mux Execution Logic.
 *         2)flagToTriggerPUSCH - This will be used to trigger PUSCH 
 *           functionality once PDCCH complete it processing.
 *         3)delayToPhy-This stores delay from current module to PHY
 *         4)currentGlobalTick-This stores current Global Tick.
 *         ulSubframeNumForDlAckNack
 * Outputs :None
 * Returns : None
 * Description:This funtion will call funtions which will allocate CCE for both 
 *             DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *             PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDCIFormat0(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		)
{
    currentsubframe_g = subFrameNum;
    UInt32 availableCCE       = 0;    
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    PdcchULOutputInfo *pdcchULOutputInfo_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 allocSPSFirst=0;
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t  currentGlobalTickWithDelay         = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    miValueForSubframe_g = miValueForSubframe(subFrameNum);
    /* - TDD - Reset CCE Changes */
    /* CLPC_CHG */
    /*START : DYN_PDCCH*/
    UInt32 maxCommonSpaceCCE = maxCCEInCommonSpace_g[valueCFI_g - 1][miValueForSubframe_g];
    /*END   : DYN_PDCCH*/
    /* CLPC_CHG END */
/* SPR 5333 changes end */
    /* 3Mhz + Dyn PDCCH Fix Start*/
    UInt8 totalCCERequired = 0;
    /* 3Mhz + Dyn PDCCH Fix End*/
    /*START:SPR:6080*/ 
    UInt8 maxAllowedCFIValue = 0;
    /*END:SPR:6080*/ 
    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay =currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
    /* TDD Config 0 Changes Start*/
    PdcchInfo *pdcchInfo_p                                  = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]              = {PNULL};
    UInt8 ulSubframeNumFor2ndSf                             = 0;
    if (UL_DL_CONFIG_0 == cellConfig_g.subFrameAssign)
    {   
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Initialise all the variables for Nack Info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
                subFrameNum,SFN,ulSubframeNumFor2ndSf);
        if (PNULL == pdcchInfo_p)
        {
            return;    
        }
    }
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTickWithDelay,
            subFrameNum,SFN,ulSubframeNum);
    if (PNULL == pdcchInfo_p)
    {
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    pdcchULOutputInfo_p = pdcchInfo_p->pdcchULOutput_p;
    /*TDD Config 0 Chages End*/
    /* ICIC changes start */
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START:SPR:6080*/ 
/* SPR 19505 fix Start */
    if (((subFrameNum == 1)||(subFrameNum == 6))  && (3 == valueCFI_g))
    {
/* SPR 19505 fix End */
        maxAllowedCFIValue = CFI_VALUE_SPECIAL_SUBFRAME; 
    }
    else
    {
        maxAllowedCFIValue = valueCFI_g;
    }
    /*END:SPR:6080*/ 
/* SPR 5333 changes end */    
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    LTE_MAC_UT_LOG( LOG_CRITICAL, PDCCH_MGR,
            "subFrameNum = %u\n maxCCEAvailable_g[miValueForSubframe_g]= %u\n availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, availableCCE, dlRAInfo_g[dlRAInfoIndex_g].totalAllowedRB,
            containerIndex);  
    /* ICIC changes start */
    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = &dciCCEContainerInfo_g[containerIndex];
    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p);

    /* + TDD - Reset CCE Changes */
    availableCCE  = maxCCEForCFI_g[maxAllowedCFIValue-1][miValueForSubframe_g];
    /* - TDD - Reset CCE Changes */

    /* CLPC_CHG */
    /* Calling function to process nodes from tcpTriggerQueueForPusch */
    triggerDciForPuschPowerControl( currentGlobalTick );
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g = 0;
    /* - SPR 8190 */
    
    /* This function processes scheduleFormat3Or3AQueue to schedule 
    DCI Format 3/3A for sending TPC for PUSCH/PUCCH power control */
    if (maxCommonSpaceCCE)
    {
        processScheduledFormat3Or3AQueue( 
                &maxCommonSpaceCCE, 
                pdcchDLOutputInfo_p,
                &pdcchULOutputInfo_p->pdcchTpcMsgInfoForPusch,
                dciCCEContainerInfo_p, 
                &availableCCE, 
                currentGlobalTick);
    }
    /* CLPC_CHG END */

    /*START : DYN_PDCCH*/
    if (!cellConfig_g.isDynamicPdcchDisabled)
    {
        currentCFIValue_g = calculateCFI(currentGlobalTickWithDelay,
                                    pdcchDLOutputInfo_p,
                                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                   pdcchULOutputInfo_p,
#endif
                                   ONLY_UL,
                                   /* +- SPR 17777 */
                                   /* 3Mhz + Dyn PDCCH Fix Start*/
                                   &totalCCERequired);
                                   /* 3Mhz + Dyn PDCCH Fix End*/
        /*START:SPR:6080*/ 
        if((currentCFIValue_g == 3) && ((subFrameNum == 1) || (subFrameNum == 6)))
        {
            currentCFIValue_g = 2;
        }
        /*END:SPR:6080*/ 
        /*START:SPR:6080*/ 
        while(((maxCCEInCommonSpace_g[maxAllowedCFIValue-1][miValueForSubframe_g] - maxCommonSpaceCCE) 
               > maxCCEInCommonSpace_g[currentCFIValue_g-1][miValueForSubframe_g])
              && (currentCFIValue_g < maxAllowedCFIValue/*3*/))
        /*END:SPR:6080*/ 
        {
            currentCFIValue_g = currentCFIValue_g + 1;
        }
        /* 3Mhz + Dyn PDCCH Fix Start*/
        /*START:SPR:6080*/ 
        while (((totalCCERequired + (maxCCEInCommonSpace_g[maxAllowedCFIValue - 1][miValueForSubframe_g] - maxCommonSpaceCCE)) 
               > maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g]) 
               && (currentCFIValue_g < maxAllowedCFIValue/*3*/))
        /*END:SPR:6080*/ 
        {
           currentCFIValue_g++;
        }
        /* 3Mhz + Dyn PDCCH Fix End*/
        /* + SPR 8190 */
        while ((highestCCENumberCommonSpace_g != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g] < highestCCENumberCommonSpace_g)
                &&(currentCFIValue_g < maxAllowedCFIValue))
        {
            currentCFIValue_g++;
        }
        highestCCENumberCommonSpace_g = 0;
        /* - SPR 8190 */
        if(cellConfig_g.dlAvailableRBs == 6 && currentCFIValue_g < 4)
        {
            currentCFIValue_g += 1;
            /*START:SPR:6080*/ 
            if((subFrameNum == 1) || (subFrameNum == 6))
            {
                currentCFIValue_g = 2;
            }
            /*END:SPR:6080*/ 
        }
        availableCCE = maxCCEForCFI_g[currentCFIValue_g-1][miValueForSubframe_g] - 
        /*START:SPR:6080*/ 
                   (maxCCEInCommonSpace_g[maxAllowedCFIValue - 1][miValueForSubframe_g] - maxCommonSpaceCCE);
        /*END:SPR:6080*/ 
    }
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g;
    /* + TDD - Reset CCE Changes */
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g;
    /* - TDD - Reset CCE Changes */
    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst,
            subFrameNum,
            SFN);

    putEntryInUlPdcchSignalQueue(availableCCE);

    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);

}

/*****************************************************************************
 * Function Name : allocPdcchForUl
 * INPUTS :1)withMuxFlag-This when set to one Indicates that we need to call
 *           directly invoke Mux Execution Logic.
 *         2)flagToTriggerPUSCH - This will be used to trigger PUSCH 
 *           functionality once PDCCH complete it processing.
 *         3)delayToPhy-This stores delay from current module to PHY
 *         4)currentGlobalTick-This stores current Global Tick.
 *         ulSubframeNumForDlAckNack
 * Outputs :None
 * Returns : None
 * Description:This funtion will call funtions which will allocate CCE  
 *             UL UE.
 *****************************************************************************/
void  allocPdcchForUl(
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		)
{
    UInt32 containerIndex     = 0;
    UInt32 availableCCE       = 0;    
    DciCCEInfo   *dciCCEInfo_p                 = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    PdcchSignalNodeP * node_p = PNULL;
    /* TDD Config 0 Changes Start*/
    UInt32 msg3CountContainerFor2ndSf                           = 0;
    PdcchInfo *pdcchInfo_p                                      = PNULL;
    PdcchInfo* localPdcchInfoMap[MAX_SUBFRAME]                  = {PNULL};
    UInt8 maxUeScheduledInUplink[MAX_SUBFRAME]              = {0};
    UInt8 ulSubframeNumFor2ndSf                             = 0;
    /*Nack queue count that is already scheduled in previous SF*/
    UInt8 nackQCount                                        = 0;
    /* TDD Config 0 Changes End*/
    /* + SPS_TDD_Changes */
    UInt8 spsArrayTti = MAX_SPS_INTERVAL;
    UInt8  rbMapNum = 0;
    /* - SPS_TDD_Changes */

    /* 5798 changes start */
    UInt32 msg3CountContainer = (currentGlobalTick + getExactULSubFrame(subFrameNum)) %
                MAX_NUMBER_OF_CONTAINER;
    /*SPR 8416 Start*/
    /* TDD Config 0 Changes Start*/
    if (UL_DL_CONFIG_0 == cellConfig_g.subFrameAssign)
    {
        ulSubframeNumFor2ndSf = (subFrameNum + ULTX_OFFSET)%MAX_SUBFRAME;
        //Set the max UE to be schedule in Uplink for UL Subframe(+7th) 
        //which will be processed in Nack Queue processing
        maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = ulSchdConfig_g.maxUEsToBeScheduledUL;
        /* In TDD Config 0 we need to schedule for two U Subframe in one D/S subframe
         * In S Subframe we have to process Strategy node for two U Subframe
         * One for +Kth SF
         * Two for +7th SF   */
        //Initialise all the variables for New Ue  and Nack Queue info for +7th U SF
        pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTick,
                subFrameNum,SFN,ulSubframeNumFor2ndSf);
        if (PNULL == pdcchInfo_p)
        {
            cleanlocalPdcchInfoMap(localPdcchInfoMap);
            return;    
        }
        //Deduct the Msg3 UE from Max Uplink UE 
        msg3CountContainerFor2ndSf = (currentGlobalTick + ULTX_OFFSET) % 
            MAX_NUMBER_OF_CONTAINER;

        if (maxUeScheduledInUplink[ulSubframeNumFor2ndSf] >= msg3ScheduledCount_g[msg3CountContainerFor2ndSf])
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = maxUeScheduledInUplink[ulSubframeNumFor2ndSf]
                - msg3ScheduledCount_g[msg3CountContainerFor2ndSf]; 
        }
        else
        {
            maxUeScheduledInUplink[ulSubframeNumFor2ndSf] = 0;
        }

        if ( SP == getSubframeType(subFrameNum))
        {
            msg3ScheduledCount_g[msg3CountContainerFor2ndSf] = 0;
        }
    }
    //Initialise all the variables for Nack Info and New Ue info for +Kth U SF
    pdcchInfo_p = getPdcchInfoNode(localPdcchInfoMap,currentGlobalTick,
            subFrameNum,SFN,ulSubframeNum);
    if (PNULL == pdcchInfo_p)
    {
        cleanlocalPdcchInfoMap(localPdcchInfoMap);
        return;    
    }
    /* TDD Config 0 Changes Start*/
    maxUeScheduledInUplink[ulSubframeNum] = ulSchdConfig_g.maxUEsToBeScheduledUL;
    /*We need to deduct the number of UEs which are already scheduled 
     * while Nack Queue processing in last Tick*/
    if ((UL_DL_CONFIG_0 == cellConfig_g.subFrameAssign) 
            && (SP == getSubframeType(subFrameNum)))
    {
        nackQCount = rbMapNodeArr_g[pdcchInfo_p->rbMapNum].nackQueueCount;
    }
    if (maxUeScheduledInUplink[ulSubframeNum] >= (msg3ScheduledCount_g[msg3CountContainer]
                + nackQCount) )
    {
        maxUeScheduledInUplink[ulSubframeNum] = maxUeScheduledInUplink[ulSubframeNum]
            - msg3ScheduledCount_g[msg3CountContainer] - nackQCount; 
    }
    else
    {
        maxUeScheduledInUplink[ulSubframeNum] = 0;
    }
    msg3ScheduledCount_g[msg3CountContainer] = 0;
    /* TDD Config 0 Chages End*/
    /*SPR 8416 End*/
    /* 5798 changes end */

    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
            __func__, currentGlobalTick, SFN, subFrameNum);

    /* ICIC changes end */

    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    containerIndex = currentGlobalTick % MAX_CCE_CONTAINER;

    dciCCEContainerInfo_p = &dciCCEContainerInfo_g[containerIndex];
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /*Semaphore post will be done by DL Thread after CCE allocation in DL*/

    semWaitDefault(&waitForPDCCHSignalInd_g);
    popNodeCircQ(&pdcchSignalCircQueue_g, (void *)&node_p);

    if(node_p == NULL)
    {
        ltePanic("DL Thread should always insert Node\n");
    }
    else
    {
        availableCCE=node_p->availableCCE;
        freeMemPool(node_p);
    }

    if(availableCCE)
    {
        /* + SPS_TDD_Changes */
        spsArrayTti = (currentGlobalTick + getExactULSubFrame(subFrameNum))%MAX_UL_SPS_INTERVAL; 
        if(TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->spsSysWideParam.enableSps)
        {
            rbMapNum = (currentGlobalTick + getExactULSubFrame(subFrameNum), internalCellIndex)% getMaxULRBMaps(internalCellIndex);
            /* + SPS_TDD_Changes */
            markULSpsRBsInRBMap(rbMapNum, spsArrayTti, NORMAL_LOCATION_UPDATE, 
#ifdef FDD_CONFIG
                    currentGlobalTick, delayToPhy,
#endif
                    subFrameNum, internalCellIndex
                    );
            /* - SPS_TDD_Changes */
        }
        processULNackQForSpsUEinTDD(
                /*TDD Config 0 Changes Start*/
                localPdcchInfoMap,
                /*TDD Config 0 Changes End*/
                currentGlobalTick,
                &availableCCE,
                dciCCEInfo_p,
                dciCCEContainerInfo_p,
                /* TDD Adaptive Trasmission Prach Allocation changes */
                //ulSubframeNum
                /* TDD Adaptive Trasmission Prach Allocation changes */
                maxUeScheduledInUplink,
                internalCellIndex
                );
        /* - SPS_TDD_Changes */

        processULNackQinTDD(
                localPdcchInfoMap,
                currentGlobalTick,
                &availableCCE,
                dciCCEInfo_p,
                dciCCEContainerInfo_p,
                /* 5798 changes start */
                maxUeScheduledInUplink
                /* 5798 changes end */
                );  
    }
    if(availableCCE)
    {
        /*Calling funtion allocatePDCCHULTx funtions which will  Process those
          UE whose Uplink Grant need to be sent and then allocate the CCEs for 
          them */
        /*TDD Config 0 Changes Start*/        
        allocatePDCCHULTxInTdd(
                localPdcchInfoMap,            
                currentGlobalTick,
                subFrameNum,
                &availableCCE ,
                dciCCEInfo_p,
                dciCCEContainerInfo_p
                ,maxUeScheduledInUplink,
                execArgs,
                internalCellIndex);
        /*TDD Config 0 Changes End*/        
       /* allocatePDCCHULTx(
                pdcchULOutputInfo_p,
                &availableCCE ,
                dciCCEInfo_p,
                subFrameNum,
                dciCCEContainerInfo_p,
                maxUeScheduledInUplink,
                execArgs);
       */
       

    }


}
#endif

/*****************************************************************************
 * Function Name : dummyAllocForSpecialSF
 * Inputs        : withMuxFlag - This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 currentGlobalTick - This stores current Global Tick.
 *                 SFN - system frame number
 *                 subFrame : This stores the subFrame Number 
 *                 ulSubframeNum - This store the uplink subframe number in TDD 
 *                 internalCellIndex - Cell-Index at MAC
 *                 ulSubframeNumForDlAckNack
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion is called when no DL and UL processing need to be
 *                 done
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  dummyAllocForSpecialSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ulSubframeNum)
  LTE_GCC_UNUSED_PARAM(execArgs)
    currentsubframe_g = subFrameNum;
    /* +- SPR 17777 */
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
   /* DciCCEInfo   *dciCCEInfo_p  = PNULL; TDD warning Fix */
    PdcchULOutputInfo *pdcchULOutputInfo_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndex                = MODULO_TWO(SFN);

/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    /* miValueForSubframe_g = miValueForSubframe(subFrameNum);*/
    /* - TDD - Reset CCE Changes */
/* SPR 5333 changes end */
    UInt8 allocSPSFirst = 0;
    /*START : DYN_PDCCH*/
    /*END   : DYN_PDCCH*/

    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    
    LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;
    pdcchULOutputInfoIndex =
        MODULO_TWO( pdcchULOutputInfoIndex + 
                        getDlAckNackSfnFactor (subFrameNum, internalCellIndex));
    /** CA-TDD Changes Start **/
    pdcchULOutputInfo_p = &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndex][ulSubframeNumForDlAckNack];
    /** CA-TDD Changes End **/
    /* ICIC changes start */
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */
    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/

    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
/* SPR 5333 changes start */
    /** CA-TDD Changes Start **/
    LTE_MAC_UT_LOG( LOG_CRITICAL, PDCCH_MGR,
            "subFrameNum = %u\n maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]]= %u\n \
            availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue-1][miValueForSubframe_g[internalCellIndex]],
            (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB,
            containerIndex);
   /** CA-TDD Changes Stop **/
/* SPR 5333 changes end */
    /* ICIC changes start */
    /*This is pointer to structure on which Free CCE Information is there
      and once cce is allocated ,updated  information is written on the
      reserved CCE */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;

    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx,internalCellIndex );

    /* SPR 11230 fix start */
    /* Reset HARQ map */
    mapIdx = (currentGlobalTick + delayToPhy ) %
		MAX_HARQ_CONTAINER_MAP_SIZE;
	/* CA TDD Changes Start */
	resetEntryInHarqSentMap( mapIdx , internalCellIndex);
	/* CA TDD Changes End */
    /* SPR 11230 fix end */

    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
     miValueForSubframe_g[internalCellIndex] = miValueForSubframe(subFrameNum, internalCellIndex);
     /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */
   /* dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];TDD Warning Fix */

    /*START : DYN_PDCCH*/
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
    /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */

    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst,
            internalCellIndex,
            subFrameNum,
            SFN);

#ifdef DL_UL_SPLIT_TDD
    putEntryInUlPdcchSignalQueue(0);
#endif
    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);

}

/*****************************************************************************
 * Function Name : allocPdcchPdschForDLInNormalSF
 * Inputs        : withMuxFlag - This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 currentGlobalTick - This stores current Global Tick.
 *                 SFN - system frame number
 *                 subFrame : This stores the subFrame Number 
 *                 ulSubframeNum - This store the uplink subframe number in TDD 
 *                 internalCellIndex - Cell-Index at MAC
 *                 ulSubframeNumForDlAckNack
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will call funtions which will allocate CCE for both 
 *                 DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *                 PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDLInNormalSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /*SPR 15909 fix start*/
        tickType_t currentGlobalTick ,
        /*SPR 15909 fix end*/
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
		)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ulSubframeNum)
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    currentsubframe_g = subFrameNum;
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
    UInt32 availableCCE       = 0;    
    SInt32 availableDLRBCount = 0;
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    DciCCEInfo   *dciCCEInfo_p                 = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfo_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndex               = MODULO_TWO(SFN);
    /* CLPC_CHG */
    UInt32 maxCommonSpaceCCE = 0;
    /* CLPC_CHG END */
/* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    /* miValueForSubframe_g = miValueForSubframe(subFrameNum, internalCellIndex); */
    /* - TDD - Reset CCE Changes */
/* SPR 5333 changes end */

    UInt8 allocSPSFirst = 0;
    /* + SPS_TDD_Changes */
    UInt32              spsRMIndex                  = 0;
    UInt8               spsRBs                      = 0;
    /* - SPS_TDD_Changes */
    /*START : DYN_PDCCH*/
     /*SPR 15909 fix start*/
     tickType_t currentGlobalTickWithDelay         = 0;
     /*SPR 15909 fix end*/
    /*END   : DYN_PDCCH*/

    UInt8 totalCCERequired = 0;
    /*START:SPR:6080*/ 
    UInt8 maxAllowedCFIValue = 0;
    /*END:SPR:6080*/

    /* + PRS_CHANGES */
    PRSConfigInfo   *prsConfigInfo_p = PNULL;
    UInt32  pdcchOrderQCount = 0;
    UInt8 pdcchOrderPerTTICount = 0;  
    PDCCHOrderNode *pdcchOrderNode_p = PNULL; 
    /* - PRS_CHANGES */
       /* EICIC +*/
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNumTemp = 0;
    UInt8 absSubFrameNum = 0;
    UInt8 numBitsAbsPattern = 0;
   /* EICIC -*/ 
#ifdef LTE_EMBMS_SUPPORTED
	   UInt8 eMbmsCfiValue=0; 
#endif
    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    

    LTE_MAC_UT_LOG( LOG_CRITICAL , PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay =currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/
    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
    /* 5492 changes start */
    /* + SPS_TDD_Changes */
    spsRMIndex = currentGlobalTickWithDelay % MAX_SPS_INTERVAL; 
    spsRBs = spsStrTTIMap_g[internalCellIndex][spsRMIndex].numDlSpsRbsPerTti;
    /* - SPS_TDD_Changes */
    UInt32 maxUeScheduledInDownlink = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                                       initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    pdcchULOutputInfoIndex =
        MODULO_TWO( pdcchULOutputInfoIndex + 
                        getDlAckNackSfnFactor (subFrameNum, internalCellIndex));
    /** CA-TDD Changes Start **/
    /* TDD Config 0 Changes Start */
    pdcchULOutputInfo_p = &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndex][ulSubframeNumForDlAckNack];
    /* TDD Config 0 Changes End */
    /** CA-TDD Changes End **/
    /* ICIC changes start */
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
    pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    /* ICIC changes end */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START:SPR:6080*/ 
    /* SPR_9453_FIX_START */ 
    if (TRUE == checkForSpecialSubframe(subFrameNum, internalCellIndex ))
    {
    /* SPR_9453_FIX_END */
        maxAllowedCFIValue = CFI_VALUE_SPECIAL_SUBFRAME;
    }
    else
    {
        maxAllowedCFIValue = valueCFI_g[internalCellIndex];
    }
    /*END:SPR:6080*/ 
/* SPR 5333 changes end */
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    availableDLRBCount  =  (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;   
    /* ICIC changes start */

    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;

    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx,internalCellIndex );

    /* SPR 11230 fix start */
    /* Reset HARQ map */
    mapIdx = (currentGlobalTick + delayToPhy ) %
		MAX_HARQ_CONTAINER_MAP_SIZE;
	/* CA TDD Changes Start */
	resetEntryInHarqSentMap( mapIdx , internalCellIndex);
	/* CA TDD Changes End */
    /* SPR 11230 fix end */

    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
    miValueForSubframe_g[internalCellIndex] = miValueForSubframe(subFrameNum, internalCellIndex); 
    /** CA-TDD Changes Stop **/
    /** CA-TDD Changes Start **/
    availableCCE  = maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue/*valueCFI_g*/ - 1][miValueForSubframe_g[internalCellIndex]];
    /** CA-TDD Changes Stop **/
	
#ifdef LTE_EMBMS_SUPPORTED
    if((isEmbmsValidSf_g[internalCellIndex] == TRUE) && 
       (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
    {
        eMbmsCfiValue =\
        embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
        availableCCE  = maxCCEForCFI_g[internalCellIndex][eMbmsCfiValue-1][miValueForSubframe_g[internalCellIndex]];
    }
#endif
     LTE_MAC_UT_LOG( LOG_CRITICAL , PDCCH_MGR,
            "subFrameNum = %u\n \
            maxCCEForCFI_g[internalCellIndex][maxAllowedCFIValue/*valueCFI_g*/ - 1][miValueForSubframe_g[internalCellIndex]]= %u\n \
            availableDLRBCount = %d\n \
            containerIndex = %d \n",subFrameNum, availableCCE, availableDLRBCount,
            containerIndex);

    /* - TDD - Reset CCE Changes */
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch */
    triggerDciForPucchPowerControl( currentGlobalTick , internalCellIndex);
    
    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU */
    /* CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
       that are left after allocating common space CCEs to BCCH, PCCH, RACH
       and CCCH message */
    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g[internalCellIndex] = 0;
    /* - SPR 8190 */
 
    /* + PRS_CHANGES */
    allocateCCEsForSIPagingULOnly[internalCellIndex] = FALSE;
    prsConfigInfo_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->prsConfigInfo);
    CHECK_FOR_PRS_OCCASION(prsConfigInfo_p,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->dlBandwidth,internalCellIndex);
    /* - PRS_CHANGES */

    /* EICIC +*/
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
   getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);
    
        /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNumTemp = subFrameNumTemp + delayToPhy;
	    /*Complexity Start*/
    eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNumTemp);
    numBitsAbsPattern =  eicicFindNumOfBitsinABSPattern(internalCellIndex);
	    /*Complexity End*/
    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex);
    }
    else
    {
	    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    }
    /* EICIC Optimization +- */
       /* EICIC -*/

    maxCommonSpaceCCE = allocatePdcchCommonChannel(
            &availableCCE, 
            &availableDLRBCount,
            pdcchDLOutputInfo_p,
            dciCCEContainerInfo_p,
            delayToPhy,
            subFrameNum,
            SFN,
            /* +- SPR 17777 */
            currentGlobalTick,
            pdcchULOutputInfo_p,
            /* +- SPR 17777 */
            /* 5492 changes start */
            &maxUeScheduledInDownlink,
            /* 5492 changes end */
            internalCellIndex);


    /*START : DYN_PDCCH*/
    if (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->isDynamicPdcchDisabled)
    {
        currentCFIValue_g[internalCellIndex] = calculateCFI(currentGlobalTickWithDelay,
                                    pdcchDLOutputInfo_p,
                                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                   pdcchULOutputInfo_p,
#endif
                                   ONLY_DL,
                                   /* +- SPR 17777 */
                                   &totalCCERequired,
                                   internalCellIndex);
        /*START:SPR:6080*/ 
        /* SPR_9453_FIX_START */
        if((currentCFIValue_g[internalCellIndex] == 3) &&
            (TRUE == checkForSpecialSubframe(subFrameNum, internalCellIndex)))
        {
        /* SPR_9453_FIX_END */
            currentCFIValue_g[internalCellIndex] = 2;
        }
        /*END:SPR:6080*/ 
        /*START:SPR:6080*/
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        while(((maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue/*valueCFI_g*/-1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE) 
            > maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]])
            && (currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue/*3*/))
        /*CA TDD CHANGES END*/    
        /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
        {
            currentCFIValue_g[internalCellIndex] = currentCFIValue_g[internalCellIndex] + 1;
        }
        /*START:SPR:6080*/
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        while (((totalCCERequired + (maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE))             
               > maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]]) 
               && (currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue/*3*/)) 
         /*CA TDD CHANGES END*/   
            /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
        {
           currentCFIValue_g[internalCellIndex]++;
        }
        /* + SPR 8190 */
        while ((highestCCENumberCommonSpace_g[internalCellIndex] != INVALID_CCE_INDEX)/** CA-TDD Changes Start **/
                && (maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] < 
                highestCCENumberCommonSpace_g[internalCellIndex])/** CA-TDD Changes Stop **/
                &&(currentCFIValue_g[internalCellIndex] < maxAllowedCFIValue))
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        highestCCENumberCommonSpace_g[internalCellIndex] = 0;
        /* - SPR 8190 */


        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->dlAvailableRBs == 6 && currentCFIValue_g[internalCellIndex] < 4)
        {
            currentCFIValue_g[internalCellIndex] += 1;
            /*START:SPR:6080*/ 
            /* SPR_9453_FIX_START */
            if(TRUE == checkForSpecialSubframe(subFrameNum, internalCellIndex))
            {
            /* SPR_9453_FIX_END */
                currentCFIValue_g[internalCellIndex] = 2;
            }
            /*END:SPR:6080*/ 
        }
         /** CA-TDD Changes Start **/
         /*CA TDD CHANGES START*/
        availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] 
        /*START:SPR:6080*/ 
                      - (maxCCEInCommonSpace_g[internalCellIndex][maxAllowedCFIValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);             /*CA TDD CHANGES END*/    
                      /** CA-TDD Changes Stop **/

        /*END:SPR:6080*/ 
    }
    /*END   : DYN_PDCCH*/
#ifdef LTE_EMBMS_SUPPORTED
		if((isEmbmsValidSf_g[internalCellIndex] == TRUE) &&
		   (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
		{
			currentCFIValue_g[internalCellIndex] = \
            embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
			availableCCE = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] - 
				(maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex] - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);
		}
#endif

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
    /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g[internalCellIndex].totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g[internalCellIndex].totalRBs = 0;
        dlBufferPool_g[internalCellIndex].cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g[internalCellIndex].totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);
        
        if (dlBufferPool_g[internalCellIndex].cellEdgeRBs > dlBufferPool_g[internalCellIndex].totalRBs)
        {
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = dlBufferPool_g[internalCellIndex].totalRBs;
        }
    }
    excessPoolRb_g[internalCellIndex][CC_CE_REGION] = dlBufferPool_g[internalCellIndex].totalRBs;
    excessPoolRb_g[internalCellIndex][CE_REGION] = dlBufferPool_g[internalCellIndex].cellEdgeRBs;
    /* + SPS_TDD_Changes */
    if ((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->spsSysWideParam.enableSps) &&
            ((spsRBs + RESERVE_SPS_FIRST_FACTOR*(availableDLRBCountTemp - availableDLRBCount)) < availableDLRBCountTemp ))
    {
        allocSPSFirst = 1;
    }
    /* - SPS_TDD_Changes */
    

    /* CLPC_CHG END */
    /* 5492 changes start */
    /* SPR 4063 Start */
    if(availableCCE)
    {
        /* + PRS_CHANGES */
        /* If it is PRS occasion then push all the entries in new  PDCCH ORDER 
         * queue in to  PDCCH ORDER advance queue.
         */ 
        if (!allocateCCEsForSIPagingULOnly[internalCellIndex] && (!isUeTypeInvalid_g[internalCellIndex]))
        { 
        /* Calling Function processPdcchOrderQueue which will process PDCCH
         * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
         * starting contention free rach. Only DCI 1A PDU will be sent here*/
        processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                dciCCEInfo_p, subFrameNum, currentGlobalTick,
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink,
                            /* 5492 changes end */
                            internalCellIndex);
        }
        else 
        {
            pdcchOrderQCount = COUNT_PDCCH_ORDER_Q(internalCellIndex);

            /*Assign pdcchOrderPerTTICount with a value not to exceed maximum PDCCH
              Order that can be processed in one tick */
            pdcchOrderPerTTICount = maxPdcchOrderperTTI_g[internalCellIndex];
            if (pdcchOrderPerTTICount && pdcchOrderQCount)
            {
                while((pdcchOrderPerTTICount--) && (pdcchOrderQCount--))

                {
                    DEQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, (void *)&pdcchOrderNode_p, internalCellIndex);
                    if(pdcchOrderNode_p)
                    {
                        /* coverity 64944 28June2014 */
                        if ( pushNode(pdcchOrderAdvanceQueue_gp[internalCellIndex], &(pdcchOrderNode_p->anchorNode)) )
                        {
                            freeMemPool(pdcchOrderNode_p);
                            /*Coverity ID <65227> Fix Changes Start*/
                            freeMemPool(pdcchDLOutputInfo_p);
                            /*Coverity ID <65227> Fix Changes End*/
                            return;
                        }
                        /* coverity 64944 28June2014 */

                        pdcchOrderNode_p = PNULL;
                    }
                }
            } 
        }

        /* - PRS_CHANGES */
    }
    /* SPR 4063 End */
    /* 5492 changes end */

    /* ICIC changes start */
    /* + PRS_CHANGES */
    /* If it is not PRS occasion then only process this queue */
#ifndef LTE_EMBMS_SUPPORTED
		if (availableDLRBCount && availableCCE && 
		   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
		   (!isUeTypeInvalid_g[internalCellIndex]))
#else 
		if (availableDLRBCount && availableCCE && 
		   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
		   (!isUeTypeInvalid_g[internalCellIndex]) &&
		   (isEmbmsValidSf_g[internalCellIndex] == FALSE)) 
#endif 
        /* ICIC changes end */
    /* - PRS_CHANGES */ 
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                /*QOS_TDD_FIX*/
                &pdcchULOutputInfo_p->pdcchDLUEInfo,
                /*QOS_TDD_FIX*/
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex);

    }

    /* + PRS_CHANGES */
    /* If it is not PRS occasion then only process this queue */
#ifndef LTE_EMBMS_SUPPORTED
					if (availableDLRBCount && availableCCE && 
					   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
					   (!isUeTypeInvalid_g[internalCellIndex]))
#else 
					if (availableDLRBCount && availableCCE && 
					   (!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
					   (!isUeTypeInvalid_g[internalCellIndex]) &&
					   (isEmbmsValidSf_g[internalCellIndex] == FALSE)) 
#endif

    /* - PRS_CHANGES */ 
    {	
        /*Calling funtion allocatePDCCHDLNewTx funtions which will 
          process those UE whos new Transmission need to be done and then 
          allocating CCEs for them */
        allocatePDCCHDLNewTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfo_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex
                );
    }
    emptyDLStrategyQueues(internalCellIndex);
    /*START : DYN_PDCCH*/
    /*END   : DYN_PDCCH*/

    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
#if LTE_EMBMS_SUPPORTED
    if (isEmbmsValidSf_g[internalCellIndex] == FALSE)
    {   
#endif
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
	        allocSPSFirst,
            internalCellIndex,
            subFrameNum,
            SFN);
#if LTE_EMBMS_SUPPORTED
     }
#endif
#ifdef DL_UL_SPLIT_TDD
    putEntryInUlPdcchSignalQueue(availableCCE);
#endif
    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
}

/*****************************************************************************
 * Function Name : allocPdcchPdschForDLInSpecialSF
 * Inputs        : withMuxFlag - This when set to one Indicates that we need to call
 *                     directly invoke Mux Execution Logic.
 *                 currentGlobalTick - This stores current Global Tick.
 *                 SFN - system frame number
 *                 subFrame : This stores the subFrame Number 
 *                 ulSubframeNum - This store the uplink subframe number in TDD 
 *                 internalCellIndex - Cell-Index at MAC
 *                 ulSubframeNumForDlAckNack
 *                 execArgs
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will call funtions which will allocate CCE for both 
 *                 DL and UL UE.After Allocating CCEs for DL and UL ,it will call
 *                 PDSCH funtion which will reserve the resorce for UEs
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void  allocPdcchPdschForDLInSpecialSF(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 SFN,
        UInt32 subFrameNum,
        UInt8 ulSubframeNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ulSubframeNum)
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    currentsubframe_g = subFrameNum;
    /* ICIC changes start */
    SInt32 availableDLRBCountTemp = 0;
    /* ICIC changes end */
    UInt32 availableCCE       = 0;    
    SInt32 availableDLRBCount = 0;
    UInt32 containerIndex     = 0;
    UInt8 delayToPhy          = 0;
    DciCCEInfo   *dciCCEInfo_p                 = PNULL;
    PdcchULOutputInfo *pdcchULOutputInfo_p     = PNULL;
    DciCCEContainerInfo *dciCCEContainerInfo_p =  PNULL;    
    /* + coverity 29871 */    
    PdcchDLOutputInfo *pdcchDLOutputInfo_p           = PNULL;
    /* - coverity 29871 */    
    UInt8 pdcchULOutputInfoIndex               = MODULO_TWO(SFN);
    /* CLPC_CHG */
    UInt32 maxCommonSpaceCCE = 0;
    /* CLPC_CHG END */
    UInt8 allocSPSFirst=0;
/* SPR 19505 fix Start */
    UInt8 cfiValue            = 0;
/* SPR 19505 fix End */
    
    /* SPR 5333 changes start */
    /* + TDD - Reset CCE Changes */
    /* miValueForSubframe_g = miValueForSubframe(subFrameNum, internalCellIndex); */
    /* - TDD - Reset CCE Changes */
    /* SPR 5333 changes end */
    /*START : DYN_PDCCH*/
    /* SPR 15909 fix start */
    tickType_t  currentGlobalTickWithDelay         = 0;
    /* SPR 15909 fix end */
    /*END   : DYN_PDCCH*/
    UInt8 totalCCERequired = 0;
       /* EICIC +*/
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNumTemp = 0;
    UInt8 absSubFrameNum = 0;
    UInt8 numBitsAbsPattern = 0;
   /* EICIC -*/ 
    /* + SPS_TDD_Changes */
    UInt32 spsRMIndex = 0;
    UInt8  spsRBs     = 0;
    /* - SPS_TDD_Changes */
    /* + coverity 29871 */    
    pdcchDLOutputInfo_p = (PdcchDLOutputInfo *)getMemFromPool(sizeof(PdcchDLOutputInfo), NULL);
    if(PNULL == pdcchDLOutputInfo_p)
    {
       return;
    }
    /* - coverity 29871 */    

    LTE_MAC_UT_LOG( LOG_CRITICAL , PDCCH_MGR,
            "Entered function = %s currentGlobalTick [%d] SFN [%d] SF[%d]\n",
                __func__, currentGlobalTick, SFN, subFrameNum);
    /*START : DYN_PDCCH*/
    currentGlobalTickWithDelay =currentGlobalTick + delayToPhy;
    /*END   : DYN_PDCCH*/

    containerIndex = (currentGlobalTick + delayToPhy ) % MAX_CCE_CONTAINER;    
    /* 5492 changes start */
    UInt32 maxUeScheduledInDownlink = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                                       initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
    /* 5492 changes end */
    pdcchULOutputInfoIndex =
        MODULO_TWO( pdcchULOutputInfoIndex + 
                        getDlAckNackSfnFactor (subFrameNum, internalCellIndex));
    /* TDD Config 0 Changes Start */
    pdcchULOutputInfo_p = &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndex][ulSubframeNumForDlAckNack];
    /* TDD Config 0 Changes End */
    /* ICIC changes start */
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
    pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
    /* ICIC changes end */
    pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
    pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count = 0;
    pdcchDLOutputInfo_p->countCommonChannelMsg = 0;
    /* ICIC changes start */
    pdcchDLOutputInfo_p->countMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CE_USER] = 0;
    pdcchDLOutputInfo_p->countPriorityMsg[CC_USER] = 0;
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    /* ICIC changes end */
    /* + SPS_TDD_Changes */
    spsRMIndex = currentGlobalTickWithDelay % MAX_SPS_INTERVAL; 
    spsRBs = spsStrTTIMap_g[internalCellIndex][spsRMIndex].numDlSpsRbsPerTti;
    /* - SPS_TDD_Changes */

    /*Set the available CCE to maxAvailable CCE .As we will allocate CCE,we 
      will subtract number of allocated CCE from this .We will keep allocating
      CCE until this becomes zero*/
/* SPR 5333 changes start */
    /*START : DYN_PDCCH*/
    /* + TDD - Reset CCE Changes */
    /* availableCCE  = maxCCEForCFI_g[CFI_VALUE_SPECIAL_SUBFRAME - 1][miValueForSubframe_g]; */
    /* - TDD - Reset CCE Changes */
    /*END   : DYN_PDCCH*/

   /* SPR 5333 changes end */  
    /*Getting the Max DownLink RB  which is configured by
      RRC */
    /* ICIC changes start */
    availableDLRBCount  =  (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;   
    /* ICIC changes end */

    /*This is pointer to structure on which Free CCE Information is there 
      and once cce is allocated ,updated  information is written on the 
      reserved CCE */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;
    /* + SPR 8190 */
    highestCCENumberCommonSpace_g[internalCellIndex] = 0;
    /* - SPR 8190 */
 
    /* DL DATA SEP Changes */
    resetAllocatedCCEs(dciCCEContainerInfo_p,internalCellIndex); 
    
    /* Reset TPC map */
    UInt8 mapIdx = (currentGlobalTick + delayToPhy ) % 
        MAX_TPC_CONTAINER_MAP_SIZE;
    resetEntryInTpcSentMap( mapIdx,internalCellIndex );

    /* SPR 11230 fix start */
    /* Reset HARQ map */
    mapIdx = (currentGlobalTick + delayToPhy ) %
		MAX_HARQ_CONTAINER_MAP_SIZE;
	/* CA TDD Changes Start */
	resetEntryInHarqSentMap( mapIdx , internalCellIndex);
	/* CA TDD Changes End */
    /* SPR 11230 fix end */
/* SPR 19505 fix Start */
    if (2 != valueCFI_g[internalCellIndex]) 
    {
       cfiValue = valueCFI_g[internalCellIndex];
    }
    else
    {
       cfiValue = CFI_VALUE_SPECIAL_SUBFRAME;
    }
/* SPR 19505 fix end */


    /* + TDD - Reset CCE Changes */
     /** CA-TDD Changes Start **/
     miValueForSubframe_g[internalCellIndex] = miValueForSubframe(subFrameNum, internalCellIndex); 
/* SPR 19505 fix Start */
     availableCCE  = maxCCEForCFI_g[internalCellIndex][cfiValue-1][miValueForSubframe_g[internalCellIndex]]; 
     LTE_MAC_UT_LOG(LOG_CRITICAL, PDCCH_MGR,
             "subFrameNum = %u\n \
              maxCCEForCFI_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]]= %u\n \
             availableDLRBCount = %d\n \
             containerIndex = %d \n",subFrameNum, availableCCE, availableDLRBCount,
             containerIndex);
/* SPR 19505 fix End */

     /** CA-TDD Changes Stop **/
     /* - TDD - Reset CCE Changes */

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /* CLPC_CHG */
    /* Calling function to process nodes from tpcTriggerQueueForPucch_g */
    triggerDciForPuschPowerControl( currentGlobalTick, internalCellIndex );
    triggerDciForPucchPowerControl( currentGlobalTick, internalCellIndex );


/* EICIC +*/
   /* EICIC Optimization +- */

    if(isEicicEnabled_g[internalCellIndex])
    {
   getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);
    
        /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNumTemp = subFrameNumTemp + delayToPhy;
	    /*Complexity Start*/
    eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNumTemp);
    numBitsAbsPattern =  eicicFindNumOfBitsinABSPattern(internalCellIndex);
	    /*Complexity End*/
 	isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
 	CHECK_FOR_MUTING_OCCASION(absSubFrameNum,sysFrameNum,subFrameNumTemp,numBitsAbsPattern,internalCellIndex);
    }
    else
    {
	    isUeTypeInvalid_g[internalCellIndex] = LTE_FALSE;
    }/* EICIC Optimization +- */
       /* EICIC -*/

    /*Calling funtion allocatePdcchCommonChannel which will allocate 
      Control Region to BCCH,RAR,PCCH and CCCH PDU */
    /* CLPC_CHG - This function is modified to return the maxCommonSpaceCCE 
       that are left after allocating common space CCEs to BCCH, PCCH, RACH
       and CCCH message */
    /* ICIC changes start */
    availableDLRBCountTemp = availableDLRBCount;
    /* ICIC changes end */
    maxCommonSpaceCCE = allocatePdcchCommonChannel(
            &availableCCE, 
            &availableDLRBCount,
            pdcchDLOutputInfo_p,
            dciCCEContainerInfo_p,
            delayToPhy,
            subFrameNum,
            SFN,
            /* +- SPR 17777 */
            currentGlobalTick,
            pdcchULOutputInfo_p,
            /* +- SPR 17777 */
            /* 5492 changes start */
            &maxUeScheduledInDownlink,
            /* 5492 changes end */
            internalCellIndex);

    /*START : DYN_PDCCH*/
    if (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->isDynamicPdcchDisabled)
    {
        currentCFIValue_g[internalCellIndex] = calculateCFI(currentGlobalTickWithDelay,
                                   pdcchDLOutputInfo_p,
                                   /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                   pdcchULOutputInfo_p,
#endif
                                   ONLY_DL,
                                   /* +- SPR 17777 */
                                   &totalCCERequired,
                                   internalCellIndex);
        if(currentCFIValue_g[internalCellIndex] == 3)
        {
            /*In special subframe max value of CFI is 2*/
            currentCFIValue_g[internalCellIndex] = 2;
        }
        /*START:SPR:6080*/ 
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
/* SPR 19505 fix Start */
        while(((maxCCEInCommonSpace_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE)
            > maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]])&& (currentCFIValue_g[internalCellIndex] < cfiValue/*2*/))
/* SPR 19505 fix End */
        /*CA TDD CHANGES END*/    
            /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
        {
            currentCFIValue_g[internalCellIndex] = currentCFIValue_g[internalCellIndex] + 1;
        }
        /*START:SPR:6080*/ 
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
/* SPR 19505 fix Start */
        while (((totalCCERequired + (maxCCEInCommonSpace_g[internalCellIndex][cfiValue - 1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE)) 
                   > maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]]) && (currentCFIValue_g[internalCellIndex] < cfiValue/*3*/))
/* SPR 19505 fix End */
         /*CA TDD CHANGES END*/   
         /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
        {
           currentCFIValue_g[internalCellIndex]++;
        }
        /* + SPR 8190 */
        /** CA-TDD Changes Start **/
/* SPR 19505 fix Start */
        while ((highestCCENumberCommonSpace_g[internalCellIndex] != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] < 
                highestCCENumberCommonSpace_g[internalCellIndex])
                &&(currentCFIValue_g[internalCellIndex] < cfiValue))
        /** CA-TDD Changes Stop **/
/* SPR 19505 fix end */
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        highestCCENumberCommonSpace_g[internalCellIndex] = 0;
        /* - SPR 8190 */
        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->dlAvailableRBs == 6)
        {
            currentCFIValue_g[internalCellIndex] = 2;
        }
        /*START:SPR:6080*/
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
/* SPR 19505 fix Start */
        availableCCE = maxCCEForCFI_g[internalCellIndex][cfiValue-1 ][miValueForSubframe_g[internalCellIndex]] 
                        - (maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]] - maxCommonSpaceCCE);
/* SPR 19505 fix End */
        /*CA TDD CHANGES END*/                
        /** CA-TDD Changes Stop **/
        /*END:SPR:6080*/ 
    }
    /*END   : DYN_PDCCH*/

    /* DL DATA SEP Changes */
    dciCCEContainerInfo_p->CFIValueForThisContainer = currentCFIValue_g[internalCellIndex]; 
    /* + TDD - Reset CCE Changes */
    /** CA-TDD Changes Start **/
    dciCCEContainerInfo_p->MIValueForThisContainer = miValueForSubframe_g[internalCellIndex];
    /** CA-TDD Changes Stop **/
    /* - TDD - Reset CCE Changes */

    /* ICIC changes start */
    /* Reduce RBs allocated for common channel from excess buffer pool, 
     * if it contais any */ 
    if (dlBufferPool_g[internalCellIndex].totalRBs <= (availableDLRBCountTemp - availableDLRBCount))
    {
        dlBufferPool_g[internalCellIndex].totalRBs = 0;
        dlBufferPool_g[internalCellIndex].cellEdgeRBs = 0;
    }
    else
    {
        dlBufferPool_g[internalCellIndex].totalRBs -= 
            (availableDLRBCountTemp - availableDLRBCount);
        
        if (dlBufferPool_g[internalCellIndex].cellEdgeRBs > dlBufferPool_g[internalCellIndex].totalRBs)
        {
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = dlBufferPool_g[internalCellIndex].totalRBs;
        }
    }
    excessPoolRb_g[internalCellIndex][CC_CE_REGION] = dlBufferPool_g[internalCellIndex].totalRBs;
    excessPoolRb_g[internalCellIndex][CE_REGION] = dlBufferPool_g[internalCellIndex].cellEdgeRBs;
    /* + SPS_TDD_Changes */
    if ((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->spsSysWideParam.enableSps) &&
            ((spsRBs + RESERVE_SPS_FIRST_FACTOR*(availableDLRBCountTemp - availableDLRBCount)) < availableDLRBCountTemp ))
    {
        allocSPSFirst = 1;
    }
    /* - SPS_TDD_Changes */
    

    /* CLPC_CHG END */
    /* 5492 changes start */
    /* SPR 4063 Start */
    if(availableCCE && (!isUeTypeInvalid_g[internalCellIndex]))
    {
        /* Calling Function processPdcchOrderQueue which will process PDCCH
         * ORDER Queue for MAX_PDCCH_ORDERS_PER_TTI to send a trigger to UE for
         * starting contention free rach. Only DCI 1A PDU will be sent here*/
        processPdcchOrderQueue(&availableCCE, dciCCEContainerInfo_p,
                dciCCEInfo_p, subFrameNum, currentGlobalTick,
                            /* 5492 changes start */
                            &maxUeScheduledInDownlink,
                            /* 5492 changes end */
                            internalCellIndex);
    }
    /* SPR 4063 End */
    /* 5492 changes end */

    /* ICIC changes start */
    if ( availableDLRBCount && availableCCE && (!isUeTypeInvalid_g[internalCellIndex])) 
        /* ICIC changes end */
    {
        /*Calling funtion allocatePDCCHDLReTx funtions which will process 
          those UE whose Re Transmission need to be done and then allocating CCEs
          for them */
        allocatePDCCHDLReTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                /*QOS_TDD_FIX*/
                &pdcchULOutputInfo_p->pdcchDLUEInfo,
                /*QOS_TDD_FIX*/
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex);

    }
    if ( availableCCE && availableDLRBCount && (!isUeTypeInvalid_g[internalCellIndex])) 
    {	
        /*Calling funtion allocatePDCCHDLNewTx funtions which will 
          process those UE whos new Transmission need to be done and then 
          allocating CCEs for them */
        allocatePDCCHDLNewTx(
                pdcchDLOutputInfo_p,
                &availableCCE,
                dciCCEInfo_p,
                &availableDLRBCount,
                subFrameNum,
                &pdcchULOutputInfo_p->pdcchDLUEInfo,
                dciCCEContainerInfo_p,
                /* 5492 changes start */
                &maxUeScheduledInDownlink,
                /* 5492 changes end */
                internalCellIndex
                );
    }
    emptyDLStrategyQueues(internalCellIndex);
    /*START : DYN_PDCCH*/
    /*END   : DYN_PDCCH*/

    /*Calling funtion allocPdschForDL which will allocate the RBs to UE*/
    allocPdschForDL(
            withMuxFlag,
            delayToPhy,
            currentGlobalTick,
            pdcchDLOutputInfo_p,
            containerIndex,
            allocSPSFirst,
            internalCellIndex,
            subFrameNum,
            SFN);

#ifdef DL_UL_SPLIT_TDD
    putEntryInUlPdcchSignalQueue(availableCCE);
#endif
    /* + coverity 29871 */
    freeMemPool(pdcchDLOutputInfo_p);
    /* - coverity 29871 */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "leaving  function = %s \n", __func__);
}
#endif


#ifdef FDD_CONFIG
/***********************************************************************************
 * Function Name : initUESecificSearchSpace               
 * Inputs        : ueIndex - The ueIndex of the UE which will be used to get the UE 
 *                     Context address
 *                 UeId - This holds the rnti Value of UE.
 *                 maxNumCCE - This holds the maximum number of CCE
 *                 cellIndex - cell-Index at other serving/on-serving c
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None          
 * Returns       : None
 * Description   : This funtions will find  the UE Specific Search Space for a UE 
 *                 and populate it in the UE Context structure
 *****************************************************************************/
STATIC void initUESecificSearchSpace(
        UInt32 ueIndex ,
        UInt16 ueId,
        UInt8 maxNumCCE,
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex)
/* + coverity 55312 */
{
    UInt8 pdcchCandidateCount=0;  
    UInt8 subFrmCount=0;
    UInt8 cceIndex =0;
    UInt8 count = 0;  
    UInt32 Yk =0;
    UInt32 A =0;
    UInt32 D =0;
    /*START : DYN_PDCCH*/
    UInt8 cfi    = 0; 
    /*END   : DYN_PDCCH*/

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n",__func__);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Max num of CCE = %u \n",maxNumCCE);
    if (!maxNumCCE)
    {
        ltePanic("Max Number of CCE = 0\n");
    }
    /* Below logic has come from section 9.1.1  of secification 3GPP TS 36.213 
       V8.5.0 (2008-12) which is used fo calculation of UE Specific Search 
       Space per Aggregation Level*/
    A = 39827;
    D = 65537;
    Yk = ueId;
    /*Below For loop is executing for 10 times and for each iteration it is 
      calculaitng the UE Specific Search Space for that SubFrame */
    /*START : DYN_PDCCH*/ 
    for (; cfi < valueCFI_g[cellIndex]; cfi++)
    {
        if ((0 == cfi) &&
                (cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->\
                 cellParams_p->dlAvailableRBs<10))
        {
            continue;
        }

        maxNumCCE = maxCCEForCFI_g[cellIndex][cfi];
        for (subFrmCount = 0,Yk = ueId; subFrmCount < NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME ; subFrmCount++)
        {
            /*END  : DYN_PDCCH*/
            Yk = (A* Yk)%D;
            /*Below for loop is calculating  the UE Specific Search Space for
              each PDCCH Candidate at Aggregation Level One */
            for ( pdcchCandidateCount =0; 
                    pdcchCandidateCount < 
                    NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_ONE_UE_SPACE ;
                    pdcchCandidateCount ++)
            {
                UInt8 *cceAggrLvlOneInfo_p = PNULL;
                /* UInt8 maxCCEDividedByAggrLvOne = 0;
                   maxCCEDividedByAggrLvOne = maxNumCCE / AGGREGATION_LEVEL_ONE;
                   TDD warning Fix */
                cceAggrLvlOneInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                    [subFrmCount].cceAggrLvlOneInfo[cfi]; //DYN_PDCCH 
                for ( count =0; count < AGGREGATION_LEVEL_ONE; count++)
                {
                    /*Below for loop is calculating  the UE Specific Search 
                      Space for one PDCCH Candidate  */
                    cceIndex = AGGREGATION_LEVEL_ONE * ((Yk + 
                                pdcchCandidateCount) % (maxNumCCE / 
                                    AGGREGATION_LEVEL_ONE)) + count;                             
                    cceAggrLvlOneInfo_p[(pdcchCandidateCount*
                            AGGREGATION_LEVEL_ONE) + count] = cceIndex;
                }
            }
            if ( maxNumCCE >= AGGREGATION_LEVEL_TWO)
            {
                /*Below for loop is calculating  the UE Specific Search Space for
                  each PDCCH Candidate at Aggregation Level Two */
                for ( pdcchCandidateCount =0; 
                        pdcchCandidateCount < 
                        NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_TWO_UE_SPACE ;
                        pdcchCandidateCount ++)
                {
                    UInt8 *cceAggrLvlTwoInfo_p = PNULL;
                    /* UInt8 maxCCEDividedByAggrLvlTwo = 0;
                       maxCCEDividedByAggrLvlTwo = maxNumCCE / AGGREGATION_LEVEL_TWO;
                       TDD Warning Fix */
                    cceAggrLvlTwoInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                        [subFrmCount].cceAggrLvlTwoInfo[cfi]; //DYN_PDCCH
                    for ( count =0; count < AGGREGATION_LEVEL_TWO; count++)
                    {
                        /*Below for loop is calculating  the UE Specific Search 
                          Space for one PDCCH Candidate  */
                        cceIndex = AGGREGATION_LEVEL_TWO * ((Yk + 
                                    pdcchCandidateCount) % (maxNumCCE / 
                                        AGGREGATION_LEVEL_TWO)) + count;                             
                        cceAggrLvlTwoInfo_p[(pdcchCandidateCount*
                                AGGREGATION_LEVEL_TWO) + count] = cceIndex;
                    }
                }
            }
            /*Below for loop is calculating  the UE Specific Search Space for
              each PDCCH Candidate at Aggregation Level Four */
            if ( maxNumCCE >= AGGREGATION_LEVEL_FOUR)
            {
                for ( pdcchCandidateCount =0; 
                        pdcchCandidateCount < 
                        NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_FOUR_UE_SPACE ;
                        pdcchCandidateCount ++)
                {
                    UInt8 *cceAggrLvlFourInfo_p = PNULL;
                    /* UInt8 maxCCEDividedByAggrLvlFour = 0;
                       maxCCEDividedByAggrLvlFour = maxNumCCE / AGGREGATION_LEVEL_FOUR;
                       TDD warning Fix */
                    cceAggrLvlFourInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                        [subFrmCount].cceAggrLvlFourInfo[cfi]; //DYN_PDCCH
                    for ( count =0; count < AGGREGATION_LEVEL_FOUR; count++)
                    {
                        /*Below for loop is calculating  the UE Specific Search 
                          Space for one PDCCH Candidate  */
                        cceIndex = AGGREGATION_LEVEL_FOUR * ((Yk + 
                                    pdcchCandidateCount) % (maxNumCCE / 
                                        AGGREGATION_LEVEL_FOUR)) + count;                             
                        cceAggrLvlFourInfo_p[(pdcchCandidateCount*
                                AGGREGATION_LEVEL_FOUR) + count] = cceIndex;
                    }
                }
            }
            if (maxNumCCE >= AGGREGATION_LEVEL_EIGHT)
            {
                /*Below for loop is calculating  the UE Specific Search Space for
                  each PDCCH Candidate at Aggregation Level Eight */
                for ( pdcchCandidateCount =0; 
                        pdcchCandidateCount < 
                        NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_EIGHT_UE_SPACE ;
                        pdcchCandidateCount ++)
                {
                    UInt8 *cceAggrLvlEightInfo_p = PNULL;
                    UInt8 maxCCEDividedByAggrLvlEight = 0;
                    maxCCEDividedByAggrLvlEight = maxNumCCE / AGGREGATION_LEVEL_EIGHT;
                    cceAggrLvlEightInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                        [subFrmCount].cceAggrLvlEightInfo[cfi]; //DYN_PDCCH
                    for ( count =0; count < AGGREGATION_LEVEL_EIGHT; count++)
                    {
                        /*Below for loop is calculating  the UE Specific Search 
                          Space for one PDCCH Candidate  */
                        cceIndex = AGGREGATION_LEVEL_EIGHT * ((Yk + 
                                    pdcchCandidateCount) % 
                                (maxCCEDividedByAggrLvlEight)) + count;                             
                        cceAggrLvlEightInfo_p[(pdcchCandidateCount*
                                AGGREGATION_LEVEL_EIGHT) + count] = cceIndex;
                    }
                }
            }
        }
        /*START : DYN_PDCCH*/
    }
    /*END   :DYN_PDCCH*/

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s \n",__func__);
}
/* - coverity 55312 */

#else
/***********************************************************************************
 * Function Name : initUESecificSearchSpace               
 * Inputs        : ueIndex - The ueIndex of the UE which will be used to get the UE 
 *                     Context address
 *                 UeId - This holds the rnti Value of UE.
 *                 maxNumCCE - This holds the maximum number of CCE
 *                 cellIndex - cell-Index at other serving/on-serving c
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None          
 * Returns       : None
 * Description   : This funtions will find  the UE Specific Search Space for a UE 
 *                 and populate it in the UE Context structure
 *****************************************************************************/
STATIC void initUESecificSearchSpace(
		UInt32 ueIndex ,
		UInt16 ueId,
        /* SPR 5333 changes start */
        /* +- SPR 17777 */
                UInt8 cellIndex,
/* SPR 5333 changes end */
        InternalCellIndex internalCellIndex
		)
{
    /* + coverity 55312 */    
    UInt8 pdcchCandidateCount=0;  
    UInt8 subFrmCount=0;
    UInt8 cceIndex =0;
    UInt8 count = 0;  
    UInt32 Yk =0;
    UInt32 A =0;
    UInt32 D =0;
    /* SPR 5333 changes start */
    UInt8 miValueForPhich = 0; 
    /* SPR 5333 changes end */
    /*START : DYN_PDCCH*/
    UInt8 cfi = 0; 
    UInt8 maxCFI = 0;
    maxCFI = MAX_POSSIBLE_CFI_VALUE;
    /*END  : DYN_PDCCH*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n",__func__);
    /* +- SPR 17777 */
    /* Below logic has come from section 9.1.1  of secification 3GPP TS 36.213 
       V8.5.0 (2008-12) which is used fo calculation of UE Specific Search 
       Space per Aggregation Level*/
    A = 39827;
    D = 65537;
    Yk = ueId;
    /*Below For loop is executing for 10 times and for each iteration it is 
      calculaitng the UE Specific Search Space for that SubFrame */
    /*START : DYN_PDCCH*/ 
    for (; cfi < maxCFI; cfi++)
    {
        if ((0 == cfi) &&
                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->dlAvailableRBs<10))
        {
            continue;
        }


        for (subFrmCount = 0 ,Yk = ueId; subFrmCount < NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME ; subFrmCount++)
        {
            /*END  : DYN_PDCCH*/
            Yk = (A* Yk)%D;
            /* SPR 5333 changes start */
            if(UL != getSubframeType(subFrmCount, internalCellIndex))
            {
                miValueForPhich = miValueForSubframe(subFrmCount, internalCellIndex); 
                /* miValueForPhich returns NA only when Uplink Subframe is passed for which 
                   there is no need to initialize */
                if (!maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich])
                {
                    ltePanic("Max Number of CCE = 0\n");
                }
                /* SPR 5333 changes end */
                /*Below for loop is calculating  the UE Specific Search Space for
                  each PDCCH Candidate at Aggregation Level One */
                for ( pdcchCandidateCount =0; 
                        pdcchCandidateCount < 
                        NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_ONE_UE_SPACE ;
                        pdcchCandidateCount ++)
                {
                    UInt8 *cceAggrLvlOneInfo_p = PNULL;
                    /* UInt8 maxCCEDividedByAggrLvOne = 0;
                       maxCCEDividedByAggrLvOne = maxNumCCE / AGGREGATION_LEVEL_ONE;
                       TDD warning Fix */
/* SPR 11353 Start */
                cceAggrLvlOneInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                    [subFrmCount].cceAggrLvlOneInfo[cfi]; //DYN_PDCCH 
/* SPR 11353 End */
                    for ( count =0; count < AGGREGATION_LEVEL_ONE; count++)
                    {
                        /*Below for loop is calculating  the UE Specific Search 
                          Space for one PDCCH Candidate  */
                        /* SPR 5333 changes start */
                        cceIndex = AGGREGATION_LEVEL_ONE * ((Yk + 
                                    pdcchCandidateCount) % (maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] / //DYN_PDCCH 
                                        AGGREGATION_LEVEL_ONE)) + count;                             
                        /* SPR 5333 changes end */
                        cceAggrLvlOneInfo_p[(pdcchCandidateCount*
                                AGGREGATION_LEVEL_ONE) + count] = cceIndex;
                    }
                }
                /* SPR 5333 changes start */
                if ( maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] >= AGGREGATION_LEVEL_TWO)//DYN_PDCCH
                    /* SPR 5333 changes end */
                {
                    /*Below for loop is calculating  the UE Specific Search Space for
                      each PDCCH Candidate at Aggregation Level Two */
                    for ( pdcchCandidateCount =0; 
                            pdcchCandidateCount < 
                            NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_TWO_UE_SPACE ;
                            pdcchCandidateCount ++)
                    {
                        UInt8 *cceAggrLvlTwoInfo_p = PNULL;
                        /* UInt8 maxCCEDividedByAggrLvlTwo = 0;
                           maxCCEDividedByAggrLvlTwo = maxNumCCE / AGGREGATION_LEVEL_TWO;
                           TDD Warning Fix */
/* SPR 11353 Start */
                    cceAggrLvlTwoInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                        [subFrmCount].cceAggrLvlTwoInfo[cfi]; //DYN_PDCCH
/* SPR 11353 End */

                        for ( count =0; count < AGGREGATION_LEVEL_TWO; count++)
                        {
                            /*Below for loop is calculating  the UE Specific Search 
                              Space for one PDCCH Candidate  */
                            /* SPR 5333 changes start */
                            cceIndex = AGGREGATION_LEVEL_TWO * ((Yk + 
                                        pdcchCandidateCount) % (maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] / //DYN_PDCCH 
                                            AGGREGATION_LEVEL_TWO)) + count;                             
                            /* SPR 5333 changes end */
                            cceAggrLvlTwoInfo_p[(pdcchCandidateCount*
                                    AGGREGATION_LEVEL_TWO) + count] = cceIndex;
                        }
                    }
                }
                /*Below for loop is calculating  the UE Specific Search Space for
                  each PDCCH Candidate at Aggregation Level Four */
                /* SPR 5333 changes start */
                if ( maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] >= AGGREGATION_LEVEL_FOUR)//DYN_PDCCH
                    /* SPR 5333 changes end */
                {
                    for ( pdcchCandidateCount =0; 
                            pdcchCandidateCount < 
                            NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_FOUR_UE_SPACE ;
                            pdcchCandidateCount ++)
                    {
                        UInt8 *cceAggrLvlFourInfo_p = PNULL;
                        /* UInt8 maxCCEDividedByAggrLvlFour = 0;
                           maxCCEDividedByAggrLvlFour = maxNumCCE / AGGREGATION_LEVEL_FOUR;
                           TDD warning Fix */
/* SPR 11353 Start */
                    cceAggrLvlFourInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                        [subFrmCount].cceAggrLvlFourInfo[cfi]; //DYN_PDCCH
/* SPR 11353 End */

                        for ( count =0; count < AGGREGATION_LEVEL_FOUR; count++)
                        {
                            /*Below for loop is calculating  the UE Specific Search 
                              Space for one PDCCH Candidate  */
                            /* SPR 5333 changes start */
                            cceIndex = AGGREGATION_LEVEL_FOUR * ((Yk + 
                                        pdcchCandidateCount) % (maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] / //DYN_PDCCH 
                                            AGGREGATION_LEVEL_FOUR)) + count;                             
                            /* SPR 5333 changes end */
                            cceAggrLvlFourInfo_p[(pdcchCandidateCount*
                                    AGGREGATION_LEVEL_FOUR) + count] = cceIndex;
                        }
                    }
                }
                /* SPR 5333 changes start */
                if (maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] >= AGGREGATION_LEVEL_EIGHT)//DYN_PDCCH
                    /* SPR 5333 changes end */
                {
                    /*Below for loop is calculating  the UE Specific Search Space for
                      each PDCCH Candidate at Aggregation Level Eight */
                    for ( pdcchCandidateCount =0; 
                            pdcchCandidateCount < 
                            NUM_PDCCH_CANDIDATE_AGGREGATION_LEVEL_EIGHT_UE_SPACE ;
                            pdcchCandidateCount ++)
                    {
                        UInt8 *cceAggrLvlEightInfo_p = PNULL;
                        UInt8 maxCCEDividedByAggrLvlEight = 0;
                        /* SPR 5333 changes start */
                        maxCCEDividedByAggrLvlEight = maxCCEForCFI_g[internalCellIndex][cfi][miValueForPhich] 
                            / AGGREGATION_LEVEL_EIGHT;//DYN_PDCCH
                        /*DYN_PDCCH*/
                        /* SPR 5333 changes end */
                        /* SPR 11353 Start */
                        cceAggrLvlEightInfo_p = crntiToIdxTbl_g[internalCellIndex][ueIndex].ueSearchSpaceInfo_p[cellIndex]
                            [subFrmCount].cceAggrLvlEightInfo[cfi]; //DYN_PDCCH
                        /* SPR 11353 End */

                        for ( count =0; count < AGGREGATION_LEVEL_EIGHT; count++)
                        {
                            /*Below for loop is calculating  the UE Specific Search 
                              Space for one PDCCH Candidate  */
                            cceIndex = AGGREGATION_LEVEL_EIGHT * ((Yk + 
                                        pdcchCandidateCount) % 
                                    (maxCCEDividedByAggrLvlEight)) + count;                             
                            cceAggrLvlEightInfo_p[(pdcchCandidateCount*
                                    AGGREGATION_LEVEL_EIGHT) + count] = cceIndex;
                        }
                    }
                }
            }
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s \n",__func__);
    /* - coverity 55312 */    
}
#endif
/******************************************************************************
 * Function Name : initUESecificSearchSpaceOfAllUEs               
 * Inputs        : maxNumCCE - This holds the maximum number of CCE
 *                 cellIndex - cell-Index at other serving/on-serving c
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None          
 * Returns       : None
 * Description   : This funtions will call the initUESecificSearchSpace funtion 
 *                 for each possible rnti value and store it in the 
 *                 relevant structure
 ******************************************************************************/
/* SPR 5333 changes start */
#ifdef FDD_CONFIG
void initUESecificSearchSpaceOfAllUEs(UInt8 maxNumCCE,UInt8 cellIndex,InternalCellIndex internalCellIndex)
#else
    /* +- SPR 17777 */
void initUESecificSearchSpaceOfAllUEs(UInt8 cellIndex ,InternalCellIndex internalCellIndex)
    /* +- SPR 17777 */
#endif
/* SPR 5333 changes end */
{
    UInt32 count = 0;
    UInt16 ueId  = 0;

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered in function = %s \n",__func__);
    InitParams *initParams_p =    cellSpecificParams_g.\
                  cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p =  cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "UE Specific Search Space need to be Calculated for %d UEs",
           initParams_p->crntiRange);

    for (count = 0, ueId = initParams_p->lowerRNTIBound;
            ueId <= initParams_p->upperRNTIBound;
            ueId++)
    {
        if ((ueId >= cellParams_p->startRARNTI) &&
                (ueId <=  cellParams_p->endRARNTI))
        {
            continue;
        }
#ifdef FDD_CONFIG
        initUESecificSearchSpace(count, ueId, maxNumCCE, cellIndex,internalCellIndex);
#else
        /* SPR 11353 Start */
        /* +- SPR 17777 */
        initUESecificSearchSpace(count, ueId,cellIndex,internalCellIndex);
        /* +- SPR 17777 */
        /* SPR 11353 End */
#endif
        count++;
    }

    /*ue specific search space info should be allocated for crnti range only
    for sps rnti range ue specific search space info will be present on primary 
        cell only*/
    /*pdcch change start*/
    if ((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->spsSysWideParam.enableSps) && (internalCellIndex==cellIndex))
    { 
    /*pdcch change end*/
        /* SPR 15105 Changes Starts */
        for ( count = initParams_p->crntiRange,  ueId = cellParams_p->spsCommonCellInfo.startSpsCRnti;
              ueId <=cellParams_p->spsCommonCellInfo. endSpsCRnti ; ueId++, count++ )
        /* SPR 15105 Changes Ends */
        {
            /* ++ SPR 21406 -- code removed */
#ifdef TDD_CONFIG
            /* +- SPR 17777 */
            initUESecificSearchSpace(count, ueId, cellIndex,internalCellIndex);
            /* +- SPR 17777 */
#endif
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Leaving function = %s \n",
            __func__);
}

/******************************************************************************
 * Function Name : calculateCFIValue
 * Inputs        : internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will calculate the number of OFDAM Symbol used for 
 *                 PDCCH region .
 ******************************************************************************/
/* spr 23717 fix + */
 void calculateCFIValue(InternalCellIndex internalCellIndex)
/* spr 23717 fix - */
{
    /* SPR 19309 + */
    valueCFI_g[internalCellIndex] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->cfi;
   /*BUG:13001:start*/
    currentCFIValue_g[internalCellIndex] = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->cfi;
   /*BUG:13001:end*/
    /* SPR 19309 - */
 /* +- SPR 20736 */
/* SPR 5384 changes start */
    if(6 == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->dlAvailableRBs)
    {
       valueCFI_g[internalCellIndex] = MAX_POSSIBLE_CFI_VALUE;
    }
/* SPR 5384 changes end */
}


/******************************************************************************
 * Function Name : calculatePHICHGroup
 * Inputs        : internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will calculate the number of PHICH Group
 ******************************************************************************/
void calculatePHICHGroup(InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
        cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    InitParams *initParams_p = cellSpecificParams_g.\
       cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    UInt32 dlRBs = 0;
    UDouble32 ngValue = 0;
    dlRBs = cellParams_p->dlAvailableRBs;  
    switch (cellParams_p->phichResource )
    {
        case 0:
            {
                ngValue = NG_0_MAPPED_TO_ONE_BY_SIX;
            }break;
        case 1:
            {
                ngValue = NG_1_MAPPED_TO_ONE_BY_TWO;
            }break;
        case 2:
            {
                ngValue = NG_2_MAPPED_TO_ONE;
            }break;
        case 3:
            {
                ngValue = NG_3_MAPPED_TO_TWO;
            }break;

        default:
            ltePanic("\n Invalid Ng Value\n");
    }

    cellParams_p->numPHICHGroup = ceilf_wrapper(ngValue * (((UDouble32) dlRBs) / 8));
    if ((cellParams_p->numPHICHGroup *8) <  initParams_p->ulSchdConfig.maxUEsToBeScheduledUL )
    {
        initParams_p->ulSchdConfig.maxUEsToBeScheduledUL = cellParams_p->numPHICHGroup*8;
    }    


    /* Initialize the PHICH_Group Array and PHICH_Sequence Array */
    addEntryToPhichGrpAndSeqArrays(
#ifdef TDD_CONFIG 
    cellParams_p->numPHICHGroup,
#endif
    internalCellIndex);

}

/******************************************************************************
 * Function Name : calculateMaxCCE
 * Inputs        : internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : This funtion will calculate the maximum number of CCEs which 
 *                 can be used for allocatind PDCCH Region.
 *****************************************************************************/
void calculateMaxCCE(InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt32 maxResourceElement  =0;    
    UInt32 resourceElementAvailableForCCE =0;
    UInt32 dlRBs = 0;
    /*Dynamic Pdcch Changes Start*/
    UInt8 cfi = 1;
    /*Dynamic Pdcch Changes End*/
/* SPR 5333 changes start */
#ifdef TDD_CONFIG
    UInt8 m_i =0;
#endif
/* SPR 5333 changes start */
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Entering function = %s \n",
            __func__);
    /*This stores the DL bandwidth in terms of RBs*/
    dlRBs = cellParams_p->dlAvailableRBs;
    /* Below is calculatig the maxResource element In the Control region */
    /*Dynamic Pdcch Changes Start*/
    if (cellParams_p->isDynamicPdcchDisabled)
    {
        currentCFIValue_g[internalCellIndex] = valueCFI_g[internalCellIndex];
    }
#ifdef FDD_CONFIG
    for (; cfi <= MAX_POSSIBLE_CFI_VALUE; cfi++)
    {
        if ((cfi == MAX_POSSIBLE_CFI_VALUE) && (dlRBs > 6))
        {
            break;
        }
        maxResourceElement = dlRBs * cfi * NUM_RESOURCE_ELEMENT_FOR_ONE_RB;
        /*Dynamic Pdcch Changes End*/

        /* SPR 947 Fix start */
        /* Below is calculating number of Resource Element available for CCEs */
        /* SPR 1531 changes start*/
        /*
         * SPEC 36.211 sec 6.2.4
         * In case a single cell-specific reference signal is configured, 
         * cell-specific reference signals shall be assumed to be present 
         * on antenna ports 0 and 1 for the purpose of mapping a 
         * symbol-quadruplet to a resource-element group, 
         * otherwise the number of cell-specific reference signals 
         * shall be assumed equal to the actual number of antenna ports 
         * used for cell-specific reference signals 
         */
        /* 4x2 MIMO S */
        /* for 1/2 Antenna ports the 4 CRS are present on first OFDM
         * symbol in PDCCH region for Normal cyclic prefix */
        /* for 4 Antenna ports the 4/4 CRS are present on first/second OFDM
         * symbol in PDCCH region for Normal cyclic prefix */
        if ( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) ||
             (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2) ||
             ( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&
               (cfi == 1) ) )
        {
            resourceElementAvailableForCCE =  maxResourceElement -
                (cellParams_p->numPHICHGroup * NUM_RESOURCE_ELEMENT_FOR_ONE_PHICH_GROUP
                 + NUM_RESOURCE_ELEMENT_FOR_PCFICH +  dlRBs
                 * NUM_RESOURCE_ELEMENT_FOR_TWO_ANTENNA_PORT_REF_SIGNAL);
        }
        else if ( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)&&
                  (cfi > 1) )
        {
            resourceElementAvailableForCCE =  maxResourceElement -
                (cellParams_p->numPHICHGroup * NUM_RESOURCE_ELEMENT_FOR_ONE_PHICH_GROUP
                + NUM_RESOURCE_ELEMENT_FOR_PCFICH +  dlRBs
                * NUM_RESOURCE_ELEMENT_FOR_FOUR_ANTENNA_PORT_REF_SIGNAL);
        } 
        /* 4x2 MIMO E */
        /* SPR 947 Fix End */
        /* Below is dividing by 36 as 36 Resource Element make 1 CCE */
/*  SPR_9402_FIX_START */  
        maxCCEForCFI_g[internalCellIndex][cfi -1] = resourceElementAvailableForCCE 
            / NUM_RESOURCE_ELEMENT_IN_ONE_CCE;

        if (maxCCEForCFI_g[internalCellIndex][cfi -1] < MAX_NUM_CCE_FOR_COMMON_SPACE)
        {
            /*START : DYN_PDCCH*/
            maxCCEInCommonSpace_g[internalCellIndex][cfi -1] = maxCCEForCFI_g[internalCellIndex][cfi -1];
            /*END   : DYN_PDCCH*/
        }
        else
        {
            /*START : DYN_PDCCH*/
            maxCCEInCommonSpace_g[internalCellIndex][cfi -1] = MAX_NUM_CCE_FOR_COMMON_SPACE;
            /*END   : DYN_PDCCH*/
        }

#ifdef PERF_STATS
    /*Update the Counters for the total max number of CCEs available in a cell */
	/* + PERF_CA */
    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedDLPerfStats.totalCceAvail = maxCCEForCFI_g[internalCellIndex][cfi -1];
	/* - PERF_CA */
#endif
	    maxCCEInCommonSpaceForPDCCH_g[internalCellIndex][cfi -1] = maxCCEInCommonSpace_g[internalCellIndex][cfi -1];
        /*  SPR_9402_FIX_END */ 
    }
#else
    for (; cfi <= MAX_POSSIBLE_CFI_VALUE; cfi++)
    {
        /* Below is calculatig the maxResource element In the Control region */
        maxResourceElement = dlRBs * cfi * NUM_RESOURCE_ELEMENT_FOR_ONE_RB;

        /* SPR 947 Fix start */
        /* Below is calculating number of Resource Element available for CCEs */
        /* SPR 1531 changes start*/
        /*
         * SPEC 36.211 sec 6.2.4
         * In case a single cell-specific reference signal is configured, 
         * cell-specific reference signals shall be assumed to be present 
         * on antenna ports 0 and 1 for the purpose of mapping a 
         * symbol-quadruplet to a resource-element group, 
         * otherwise the number of cell-specific reference signals 
         * shall be assumed equal to the actual number of antenna ports 
         * used for cell-specific reference signals 
         */

        for(m_i =0 ; m_i < MAX_VALUE_M_I; m_i++)
        {
            /* 4x2 MIMO S */
            /* for 1/2 Antenna ports the 4 CRS are present on first OFDM
             * symbol in PDCCH region for Normal cyclic prefix */
            /* for 4 Antenna ports the 4/4 CRS are present on first/second OFDM
             * symbol in PDCCH region for Normal cyclic prefix */
            if ( (cellParams_p->numOfTxAnteenas ==NUM_OF_TX_ANTENNAS_1) ||
                 (cellParams_p->numOfTxAnteenas ==NUM_OF_TX_ANTENNAS_2) ||
                 ((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)&&
                  (cfi == 1)) )
            {
                resourceElementAvailableForCCE =  maxResourceElement -
                    ((m_i * cellParams_p->numPHICHGroup * NUM_RESOURCE_ELEMENT_FOR_ONE_PHICH_GROUP)
                     + NUM_RESOURCE_ELEMENT_FOR_PCFICH +  dlRBs
                     * NUM_RESOURCE_ELEMENT_FOR_TWO_ANTENNA_PORT_REF_SIGNAL);
            }
            else if ( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&
                      (cfi > 1) )
            {
                /*  SPR_9452_FIX_START */  
                resourceElementAvailableForCCE =  maxResourceElement -
                    (m_i*cellParams_p->numPHICHGroup * NUM_RESOURCE_ELEMENT_FOR_ONE_PHICH_GROUP
                    + NUM_RESOURCE_ELEMENT_FOR_PCFICH +  dlRBs
                    * NUM_RESOURCE_ELEMENT_FOR_FOUR_ANTENNA_PORT_REF_SIGNAL);
            	/*  SPR_9452_FIX_END */
            }
            /* 4x2 MIMO E */

            /* SPR 947 Fix End */
            /*  SPR_9402_FIX_START */
            /* Below is dividing by 36 as 36 Resource Element make 1 CCE */
            maxCCEForCFI_g[internalCellIndex][cfi -1][m_i] = resourceElementAvailableForCCE 
                / NUM_RESOURCE_ELEMENT_IN_ONE_CCE;


            if (maxCCEForCFI_g[internalCellIndex][cfi -1][m_i] < MAX_NUM_CCE_FOR_COMMON_SPACE)
            {
                /*START_DYN_PDCCH_TDD_CHANGE*/
                /*CA TDD CHANGES START*/
                maxCCEInCommonSpace_g[internalCellIndex][cfi -1][m_i] = maxCCEForCFI_g[internalCellIndex][cfi -1][m_i];
                /*CA TDD CHANGES END*/
                /*END_DYN_PDCCH_TDD_CHANGE*/
            }
            else
            {
                /*START_DYN_PDCCH_TDD_CHANGE*/
                /*CA TDD CHANGES START*/
                maxCCEInCommonSpace_g[internalCellIndex][cfi -1][m_i] = MAX_NUM_CCE_FOR_COMMON_SPACE;
                /*CA TDD CHANGES END*/
                /*END_DYN_PDCCH_TDD_CHANGE*/
            }

        }
        /*  SPR_9402_FIX_END */ 
    /*START_DYN_PDCCH_TDD_CHANGE*/
    }
    /*END_DYN_PDCCH_TDD_CHANGE*/

#endif
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Leaving function = %s \n",
            __func__);
}

/* +GCC 4.7 */

/*****************************************************************************
 * Function Name  : initPDCCH
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : initialize all the Queue and List Used by the DL PDCCH
 * Returns        : None
 * Description    : This function initialize All queues and lists to be used 
 *                  by DL PDCCH Module. This is called at cell configuration.
 *****************************************************************************/
void initPDCCH (InternalCellIndex internalCellIndex)
{
    UInt16 macMaxNumOfUeSupported = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->macMaxNumOfUeSupported;

    /* CA changes start */
    maxPdcchOrderperTTI_g[internalCellIndex] = MAX_PDCCH_ORDERS_PER_TTI;

    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        /* Initializing DL Strategy New Tx, Re Tx, Advance Alloc,
         * UL Strategy and PDCCH Order Advance Queue*/
        createMemPool(sizeof(LTE_SQUEUE), 5);
        /* + SPR 22243 Fix*/
        /* Initializing PDSCH failure for TA and DRX, UL/DL Sync loss and PDCCH order CQueues */
        createMemPool(sizeof(LTE_CIRCQUEUE), 5);
        /* - SPR 22243 Fix*/
        /*NODES FOR NEW/RE TRANSMISSION and Advance alloc UE Info nodes */
        createMemPool(sizeof(DLStrategyTxNode),MAX_DL_UE_SCHEDULED * 3);

        /*NODES FOR RETRANSMISSION*/
        createMemPool(sizeof(TimeAdvanceQueueNode),MAX_DL_UE_SCHEDULED);

        createMemPool(sizeof(ULStrategyTxNode),MAX_UL_UE_SCHEDULED);

        createMemPool(sizeof(PDCCHOrderNode), (4 * maxPdcchOrderperTTI_g[internalCellIndex]));

        createMemPool(sizeof(ULSyncLossQueueNode),
                maxPdcchOrderperTTI_g[internalCellIndex] * 4);

        if(TRUE == 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps)
        {    
            createMemPool(sizeof(MacDlSpsStatus), 
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsDciPerTti));
            createMemPool(sizeof(MacDlSpsActReq), 
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsDciPerTti));
            createMemPool(sizeof(MacDlSpsResReq), 
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsDciPerTti));
            createMemPool(sizeof(MacDlSpsDeactReq),
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsDciPerTti));
            createMemPool(sizeof(MacDlSpsExclude),
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsDciPerTti));
            createMemPool(sizeof(MacSpsOccList),
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsOccasionPerTti));
            createMemPool(sizeof(MacSpsDciListTTI),
                    ((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                     spsDlSchInfoPerTti.maxDlSpsDciPerTti));
            createMemPool(sizeof(MacULSpsStatus),
                    ((macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                     spsUlSchInfoPerTti.maxUlSpsDciPerTti));
            createMemPool(sizeof(MacULSpsActReq),
                    ((macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                     spsUlSchInfoPerTti.maxUlSpsDciPerTti));
            createMemPool(sizeof(MacULSpsResReq),
                    ((macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                     spsUlSchInfoPerTti.maxUlSpsDciPerTti));
            createMemPool(sizeof(MacULSpsDeactReq),
                    ((macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                     spsUlSchInfoPerTti.maxUlSpsDciPerTti));
            createMemPool(sizeof(MacULSpsOccList),
                    ((macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                     spsUlSchInfoPerTti.maxUlSpsOccasionPerTti));
            createMemPool(sizeof(MacULSpsDciListTTI),
                    ((macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                     spsUlSchInfoPerTti.maxUlSpsDciPerTti));
            createMemPool(sizeof(SpsCrntiNode), (macMaxNumOfUeSupported));
        }

        createMemPool(sizeof(DLAdvanceQueueNode),
                (macMaxNumOfUeSupported * MAX_HARQ_PROCESS_NB));

        createMemPool(sizeof(DLRetxFailureQueueNode), 
                (macMaxNumOfUeSupported * MAX_HARQ_PROCESS_NB));

        createMemPool(sizeof(bsrQueueNode), 
                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 initParams_p->ulSchdConfig.maxUEsToBeScheduledUL));

        createMemPool((sizeof(ULHarqInfo) * MAX_HARQ_PROCESS_NB),1);

        createMemPool(sizeof(nonZeroBsrListNode),macMaxNumOfUeSupported);
#ifdef DL_UL_SPLIT
        createMemPool(sizeof(LTE_SQUEUE), 2);
#else
        createMemPool(sizeof(LTE_SQUEUE), 1);
#endif
#ifdef FDD_CONFIG
        createMemPool(sizeof(PdcchULOutputInfo) * MAX_CCE_CONTAINER, 1);
#endif
        createMemPool(sizeof(DciCCEContainerInfo) * MAX_CCE_CONTAINER, 1);

        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
        createMemPool(sizeof(PdcchInfo),MAX_SUBFRAME);
#endif
        /*TDD Config 0 Changes End*/
    }

    /* Initializing DL Strategy New Tx Queue*/
    GET_MEM_FROM_POOL(LTE_SQUEUE,dlStrategyNewTxQueue_gp[internalCellIndex],
            sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(dlStrategyNewTxQueue_gp[internalCellIndex]);

    /* Initializing DL Strategy Re Tx Queue*/
    GET_MEM_FROM_POOL(LTE_SQUEUE,dlStrategyReTxQueue_gp[internalCellIndex],
            sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(dlStrategyReTxQueue_gp[internalCellIndex] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE,pdcchPdschFailureTAQueue_gp[internalCellIndex],
            sizeof(LTE_CIRCQUEUE),PNULL);
    /* SPR 13088 Fix Start */
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
    /* SPR# 6404,8008 Changes Starts */
    circQInit(pdcchPdschFailureTAQueue_gp[internalCellIndex],MAX_NODES_SPLIT);
    /* SPR# 6404,8008 Changes Ends */
    }
    else
    {
        circQInitAfterCellSetup(pdcchPdschFailureTAQueue_gp[internalCellIndex],MAX_NODES_SPLIT);
    }
    /* SPR 13088 Fix End */
    /* + SPR 22243 Fix*/
    GET_MEM_FROM_POOL(LTE_CIRCQUEUE,pdcchPdschFailureDRXQueue_gp[internalCellIndex],
            sizeof(LTE_CIRCQUEUE),PNULL);
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
    circQInit(pdcchPdschFailureDRXQueue_gp[internalCellIndex],MAX_NODES_SPLIT);
    }
    else
    {
        circQInitAfterCellSetup(pdcchPdschFailureDRXQueue_gp[internalCellIndex],MAX_NODES_SPLIT);
    }
    /* - SPR 22243 Fix*/

#ifdef DL_UL_SPLIT
    GET_MEM_FROM_POOL(LTE_SQUEUE,ulStrategyReTxQueue_gp[internalCellIndex], sizeof(LTE_SQUEUE), PNULL);
    sQueueInit (ulStrategyReTxQueue_gp[internalCellIndex]);

    GET_MEM_FROM_POOL(LTE_SQUEUE,ulStrategyNewTxQueue_gp[internalCellIndex], sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(ulStrategyNewTxQueue_gp[internalCellIndex]);
#else 
    GET_MEM_FROM_POOL(LTE_SQUEUE, ulStrategyTxQueue_gp[internalCellIndex],sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(ulStrategyTxQueue_gp[internalCellIndex]);
#endif

    /* Initializing Advance Alloc UE Info Queue*/
    GET_MEM_FROM_POOL(LTE_SQUEUE,advanceAllocInfoQueue_gp[internalCellIndex],
            sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(advanceAllocInfoQueue_gp[internalCellIndex]);

    GET_MEM_FROM_POOL(LTE_SQUEUE,ulStrategyAdvanceTxQueue_gp[internalCellIndex],
            sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(ulStrategyAdvanceTxQueue_gp[internalCellIndex]);

    /* Initializing PDCCH Order Queue and mempool created for the nodes*/
    /* SPR 13088 Fix Start */
    GET_MEM_FROM_POOL(LTE_CIRCQUEUE,pdcchOrderQueue_gp[internalCellIndex],sizeof(LTE_CIRCQUEUE),PNULL) 

    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        INIT_PDCCH_ORDER_Q(internalCellIndex);
    }
    else
    {
        circQInitAfterCellSetup(pdcchOrderQueue_gp[internalCellIndex],MAX_NODES_SPLIT);
    }
    /* SPR 13088 Fix End */

    GET_MEM_FROM_POOL(LTE_SQUEUE,pdcchOrderAdvanceQueue_gp[internalCellIndex],
            sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(pdcchOrderAdvanceQueue_gp[internalCellIndex]);

    /* Initializing UL SYNC LOSS Trigger Queues for UL/DL Strategies
       to add/remove UE from active UE list*/ 
    /* SPR 13088 Fix Start */ 
    GET_MEM_FROM_POOL(UL_SYNC_LOSS_TRIGGERQ_UL_TYPE, ulSyncLossTriggerQUL_gp[internalCellIndex], 
            sizeof(UL_SYNC_LOSS_TRIGGERQ_UL_TYPE), PNULL);
    GET_MEM_FROM_POOL(UL_SYNC_LOSS_TRIGGERQ_DL_TYPE, ulSyncLossTriggerQDL_gp[internalCellIndex], 
            sizeof(UL_SYNC_LOSS_TRIGGERQ_DL_TYPE), PNULL);

    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce)) 
    {
        CONSTRUCT_AND_INIT_UL_SYNC_LOSS_TRIGGERQ_UL_Q(internalCellIndex);
        CONSTRUCT_AND_INIT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex);
    }
    else
    {
        circQInitAfterCellSetup(ulSyncLossTriggerQUL_gp[internalCellIndex], MAX_NODES_SPLIT);
        circQInitAfterCellSetup(ulSyncLossTriggerQDL_gp[internalCellIndex], MAX_NODES_SPLIT); 
    }
    /* SPR 13088 Fix End */ 


    /*Initialize DCI CCE Info Container*/
    initDCICCEInfoContainer(internalCellIndex);

    /*Funtion for finding number of CCE for DCI Format*/
    calculateDCIFormatToCCEMapping(internalCellIndex);

    /*Function initialize DIrect Indexing Array in each
     *of the container*/
    initializeDirectIndexingArray(internalCellIndex);
}
/*coverity changes CID-54604 */
/*****************************************************************************
 * Function Name  : cleanupPDCCH
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : cleans  all the Circular Queue used by the DL PDCCH
 * Returns        : None
 * Description    : This function cleans All Circular queues to be used 
 *                   by DL PDCCH Module. This is called at Cell Stop.
 *****************************************************************************/
void cleanupPDCCH (InternalCellIndex internalCellIndex)
{
    UInt32 count=0,loopVar=0;
    TimeAdvanceQueueNode * tempTimeAdvanceQueue_p=PNULL;
    PDCCHOrderNode *pdcchOrderNode_p = PNULL;
    ULSyncLossQueueNode *ulSyncLossQueueNode_p = PNULL;
    /* + SPR 22243 Fix*/
    DRXQueueNode *tempDrxNode_p = PNULL;
    /* - SPR 22243 Fix*/
    /* deallocating circular Queue pdcchPdschFailureTAQueue_g*/
    count = COUNT_TA_Q(pdcchPdschFailureTAQueue_gp[internalCellIndex]);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_TA_Q((pdcchPdschFailureTAQueue_gp[internalCellIndex]),TimeAdvanceQueueNode,
                (void **)&(tempTimeAdvanceQueue_p));
        if (tempTimeAdvanceQueue_p)
        {
            FREE_MEM_NODE_TA_Q(tempTimeAdvanceQueue_p);
        }

    }
    /* deallocating circular Queue pdcchOrderQueue_g*/
    count = COUNT_PDCCH_ORDER_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        /* SPR 13088 Fix Start */
        DEQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, (void **)&pdcchOrderNode_p, internalCellIndex);
        /* SPR 13088 Fix End */
        if (pdcchOrderNode_p)
        {
            FREE_MEM_NODE_PDCCH_ORDER_Q(pdcchOrderNode_p);
        }

    }
    /* deallocating circular Queue ulSyncLossTriggerQUL_g*/
    count = COUNT_UL_SYNC_LOSS_TRIGGERQ_UL_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        /* SPR 13088 Fix Start */
        DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode, (void **)&ulSyncLossQueueNode_p, internalCellIndex);
        /* SPR 13088 Fix End */
        if (ulSyncLossQueueNode_p)
        {
            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossQueueNode_p);
        }

    }
    /* deallocating circular Queue ulSyncLossTriggerQDL_g*/
    count = COUNT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ULSyncLossQueueNode,(void **)&ulSyncLossQueueNode_p, internalCellIndex);
        if (ulSyncLossQueueNode_p)
        {
            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossQueueNode_p);
        }

    }
    /* + SPR 22243 Fix*/
   /* deallocating circular Queue pdcchPdschFailureDRXQueue_g*/
    count = COUNT_DRX_Q(pdcchPdschFailureDRXQueue_gp[internalCellIndex]);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_DRX_Q((pdcchPdschFailureDRXQueue_gp[internalCellIndex]),DRXQueueNode,
                (void **)&(tempDrxNode_p));
        if (tempDrxNode_p)
        {
            FREE_MEM_NODE_DRX_Q(tempDrxNode_p);
        }

    }
    /* - SPR 22243 Fix*/
}
/*****************************************************************************
 * Function Name  : deInitPDCCH
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : cleans  all the Circular Queue used by the DL PDCCH
 * Returns        : None
 * Description    : This function DeInits All Circular queues to be used 
 *                  by DL PDCCH Module. This is called at cell delete.
 *****************************************************************************/
void deInitPDCCH (InternalCellIndex internalCellIndex)
{
    circQDeInit(pdcchPdschFailureTAQueue_gp[internalCellIndex]);
    circQDeInit(pdcchOrderQueue_gp[internalCellIndex]);
    circQDeInit(ulSyncLossTriggerQUL_gp[internalCellIndex]);
    circQDeInit(ulSyncLossTriggerQDL_gp[internalCellIndex]);
    /* + SPR 22243 Fix*/
    circQDeInit(pdcchPdschFailureDRXQueue_gp[internalCellIndex]);
    /* - SPR 22243 Fix*/

    if(dlStrategyNewTxQueue_gp[internalCellIndex])
    {
        freeMemPool(dlStrategyNewTxQueue_gp[internalCellIndex]);
        dlStrategyNewTxQueue_gp[internalCellIndex] = PNULL;
    }

    if(dlStrategyReTxQueue_gp[internalCellIndex])
    {
        freeMemPool(dlStrategyReTxQueue_gp[internalCellIndex]);
        dlStrategyReTxQueue_gp[internalCellIndex] = PNULL;
    }

    if(pdcchPdschFailureTAQueue_gp[internalCellIndex])
    {
        freeMemPool(pdcchPdschFailureTAQueue_gp[internalCellIndex]);
        pdcchPdschFailureTAQueue_gp[internalCellIndex] = PNULL;
    }
    /* + SPR 22243 Fix */
    if(pdcchPdschFailureDRXQueue_gp[internalCellIndex])
    {
        freeMemPool(pdcchPdschFailureDRXQueue_gp[internalCellIndex]);
        pdcchPdschFailureDRXQueue_gp[internalCellIndex] = PNULL;
    }
    /* - SPR 22243 Fix */

#ifdef DL_UL_SPLIT    
    if(ulStrategyNewTxQueue_gp[internalCellIndex])
    {
        freeMemPool(ulStrategyNewTxQueue_gp[internalCellIndex]);
        ulStrategyNewTxQueue_gp[internalCellIndex] = PNULL;
    }
    if(ulStrategyReTxQueue_gp[internalCellIndex])
    {
        freeMemPool(ulStrategyReTxQueue_gp[internalCellIndex]);
        ulStrategyReTxQueue_gp[internalCellIndex] = PNULL;
    }
#else
    if(ulStrategyTxQueue_gp[internalCellIndex])
    {
        freeMemPool(ulStrategyTxQueue_gp[internalCellIndex]);
        ulStrategyTxQueue_gp[internalCellIndex] = PNULL;
    }
#endif

    if(advanceAllocInfoQueue_gp[internalCellIndex])
    {
        freeMemPool(advanceAllocInfoQueue_gp[internalCellIndex]);
        advanceAllocInfoQueue_gp[internalCellIndex] = PNULL;
    }

    if(ulStrategyAdvanceTxQueue_gp[internalCellIndex])
    {
        freeMemPool(ulStrategyAdvanceTxQueue_gp[internalCellIndex]);
        ulStrategyAdvanceTxQueue_gp[internalCellIndex] = PNULL;
    }

    if(pdcchOrderQueue_gp[internalCellIndex])
    {
        freeMemPool(pdcchOrderQueue_gp[internalCellIndex]);
        pdcchOrderQueue_gp[internalCellIndex] = PNULL;
    }

    if(pdcchOrderAdvanceQueue_gp[internalCellIndex])
    {
        freeMemPool(pdcchOrderAdvanceQueue_gp[internalCellIndex]);
        pdcchOrderAdvanceQueue_gp[internalCellIndex] = PNULL;
    }

    if(ulSyncLossTriggerQDL_gp[internalCellIndex])
    {
        freeMemPool(ulSyncLossTriggerQDL_gp[internalCellIndex]);
        ulSyncLossTriggerQDL_gp[internalCellIndex] = PNULL;
    }

    if(ulSyncLossTriggerQUL_gp[internalCellIndex])
    {
        freeMemPool(ulSyncLossTriggerQUL_gp[internalCellIndex]);
        ulSyncLossTriggerQUL_gp[internalCellIndex] = PNULL;
    }

    if(dciCCEContainerInfo_gp[internalCellIndex])
    {
        freeMemPool(dciCCEContainerInfo_gp[internalCellIndex]);
        dciCCEContainerInfo_gp[internalCellIndex] = PNULL;
    }

#ifdef FDD_CONFIG
    if(pdcchULOutputInfo_gp[internalCellIndex])
    {
        freeMemPool(pdcchULOutputInfo_gp[internalCellIndex]);
        pdcchULOutputInfo_gp[internalCellIndex] = PNULL;
    }
#endif

    return;
}
/*coverity changes CID-54604 */
/*****************************************************************************
 * Function Name  : initializeDirectIndexingArray
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize DIrect Indexing Array in each
 *                  of the container
 *****************************************************************************/
STATIC void initializeDirectIndexingArray(InternalCellIndex internalCellIndex)
{
    PdcchULOutputInfo *pdcchULOutputInfo_p = PNULL;

#ifdef TDD_CONFIG
    UInt8 i = 0;
    UInt8 j = 0;

    for ( i = 0; i <= 1; i++)
    {
        for ( j = 0; j < MAX_SUB_FRAME; j++)
        {
           /** CA-TDD Changes Start **/
            pdcchULOutputInfo_p = &pdcchULOutputInfo_g[internalCellIndex][i][j];
            /** CA-TDD Changes End **/

            pdcchULOutputInfo_p->pdcchULNackUEInfo.nonAdaptiveCount = 0;
            /* ICIC changes start */
            pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] = 0;
            pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] = 0;
            pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
            pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
            /* ICIC changes end */
            /* + SPS_TDD_Changes */
            pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.nonAdaptiveCount = 0;
            pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.adaptiveCount[CC_USER] = 0;
            pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.adaptiveCount[CE_USER] = 0;
            /* - SPS_TDD_Changes */
            pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
/* SPR 4301 chg */
            pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup = 0;
/* SPR 4301 chg */

        }
    }
#elif FDD_CONFIG
    UInt32 containerCount = MAX_CCE_CONTAINER;
    UInt32 ueCount = MAX_UE_SUPPORTED;
    DirectIndexingUEInfo *directIndexingUEInfo_p = PNULL;

    GET_MEM_FROM_POOL(PdcchULOutputInfo, pdcchULOutputInfo_gp[internalCellIndex],
             sizeof(PdcchULOutputInfo) * MAX_CCE_CONTAINER , PNULL);
    pdcchULOutputInfo_p = pdcchULOutputInfo_gp[internalCellIndex] +(containerCount - 1);

    while (containerCount--)
    {
        pdcchULOutputInfo_p->pdcchULNackUEInfo.nonAdaptiveCount = 0;
        /* ICIC changes start */
        pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CE_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count = 0;
        pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count = 0;
        /* ICIC changes end */
        pdcchULOutputInfo_p->pdcchULNackTempUEInfo.nonAdaptiveCount = 0;
        /* ICIC changes start */
        pdcchULOutputInfo_p->pdcchULNackTempUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackTempUEInfo.adaptiveCount[CE_USER] = 0;
        
        /* + SPR_11076 */  
        pdcchULOutputInfo_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[CE_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackTtiBundledUEInfo.nonAdaptiveCount = 0;

        pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.nonAdaptiveCount = 0;
        pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.adaptiveCount[CC_USER] = 0;
        pdcchULOutputInfo_p->pdcchULNackSpsUEInfo.adaptiveCount[CE_USER] = 0;
        /* - SPR_11076 */  
        /* ICIC changes end */
       
        pdcchULOutputInfo_p->pdcchDLUEInfo.count = 0;
        ueCount = MAX_UE_SUPPORTED;
        directIndexingUEInfo_p = &pdcchULOutputInfo_p->
            directIndexingUEInfo[ueCount - 1];
        while ( ueCount--)
        {
            directIndexingUEInfo_p->flag = UNSET_FLAG;
            directIndexingUEInfo_p->dlCCEIndex = INVALID_CCE_INDEX;
            directIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            directIndexingUEInfo_p->ulschUciInfoIndex = 
                ULSCH_UCI_INVALID_INDEX;
            /* SPR 14877 SRS Changes */
            directIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
            /* SPR 14877 SRS Changes */
            
            directIndexingUEInfo_p->dlDciFormat = 
                MAX_NUM_DCI_FORMAT;
            /* SS_S2 */
            directIndexingUEInfo_p->transMode = 0;
            /* SS_S2 */
            directIndexingUEInfo_p->ttiCounter = 0;
            directIndexingUEInfo_p->harqIndex = INVALID_HARQ_ID;
            directIndexingUEInfo_p->strictRBAllocation = 0;
            directIndexingUEInfo_p->requiredRB = 0;
            /*SPS_CHG*/
            directIndexingUEInfo_p->isSPSCCE = LTE_FALSE;
            /*SPS_CHG*/
            directIndexingUEInfo_p--;
        }

        ueCount = MAX_UE_SUPPORTED;
        directIndexingUEInfo_p = &pdcchULOutputInfo_p->
            directIndexingTempUEInfo[ueCount - 1];
        while ( ueCount--)
        {
            directIndexingUEInfo_p->flag = UNSET_FLAG;
            directIndexingUEInfo_p->dlCCEIndex = INVALID_CCE_INDEX;
            directIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            directIndexingUEInfo_p->ulschUciInfoIndex = 
                ULSCH_UCI_INVALID_INDEX;
            /* SPR 14877 SRS Changes */
            directIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
            /* SPR 14877 SRS Changes */

            directIndexingUEInfo_p->dlDciFormat = 
                MAX_NUM_DCI_FORMAT;
            /* SS_S2 */
            directIndexingUEInfo_p->transMode = 0;
            /* SS_S2 */
            directIndexingUEInfo_p->ttiCounter = 0;
            directIndexingUEInfo_p->harqIndex = INVALID_HARQ_ID;
            directIndexingUEInfo_p->strictRBAllocation = 0;
            directIndexingUEInfo_p->requiredRB = 0;
            /*SPS_CHG*/
            directIndexingUEInfo_p->isSPSCCE = LTE_FALSE;
            /*SPS_CHG*/
            directIndexingUEInfo_p--;
        }
        pdcchULOutputInfo_p--;
    }
#endif   
}

/*****************************************************************************
 * Function Name  : initDCICCEInfoContainer
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize DCI CCE Info Container
 *****************************************************************************/
STATIC void initDCICCEInfoContainer(InternalCellIndex internalCellIndex)
{
    UInt32 containerCount = MAX_CCE_CONTAINER;
    UInt32 cceCount = 0;
    DciCCEContainerInfo *dciCCEContainerInfo_p = PNULL;
    DciCCEInfo *dciCCEInfo_p =  PNULL;
    UInt32 maxCCE = 0;
/* SPR 5333 changes start */
#ifdef FDD_CONFIG
    /*START : DYN_PDCCH*/
    maxCCE = maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1];
    /*END   : DYN_PDCCH*/
#else
/* It should be the maximum of three, which is always the case when no PHICH goes in a subframe */
    /*START : DYN_PDCCH*/
    maxCCE = maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1][0];
    /*END   : DYN_PDCCH*/
#endif
/* SPR 5333 changes end */

    GET_MEM_FROM_POOL(DciCCEContainerInfo, dciCCEContainerInfo_gp[internalCellIndex],
            sizeof(DciCCEContainerInfo) * MAX_CCE_CONTAINER , PNULL);

     /* SPR 11109 FIX start - 22-May-2014 */
     memSet(dciCCEContainerInfo_gp[internalCellIndex],0,(sizeof(DciCCEContainerInfo) * MAX_CCE_CONTAINER));
     /* SPR 11109 FIX end - 22-May-2014 */

    while( containerCount--)
    {
        cceCount = maxCCE + MAX_SPS_CCE;
        dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerCount;
        dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[cceCount - 1];
        while (cceCount--)
        {
            dciCCEInfo_p->cceIndex = cceCount;  
            dciCCEInfo_p->isCCEFree = CCE_FREE;
            /* SPR 2393 2394 Fix Start */
         /*Initializing the preamble index and msgType to default. It is used by
         * transmitter for sending DCI 1A PDU for PDCCH Order*/
            dciCCEInfo_p->dciPduInfo.preambleIdx = INVALID_PREAMBLE_IDX;
            dciCCEInfo_p->msgType = BCCH_MSG;
            /* SPR 2393 2394 Fix end */
            dciCCEInfo_p--;
        }
    }
}

/******************************************************************************
 * Function Name: allocatePdcchCommonChannel
 * Inputs       : availableCCE_p - Contains the available maximum CCEs
 *                availableDLRBCount_p -
 *                dciCCEContainerInfo_p -
 *                delayToPhy - Delay between PDCCH and PHY
 *                subFrameNum,sysFrameNum in TDD
 *                pdcchULOutputInfo_p -
 *                currentGlobalTick -
 *                maxUeScheduledInDownlink_p -
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                the information of UEs for which PDCCH region has been 
 *                allocated and for which PDSCH region need to be allocated.
 *                dciCCEInfo_p - DCI pdu array 
 * Returns      : maxCommonSpaceCCE left after allocation.
 * Description  : This will call allocatePdcchForBCCH(), allocatePdcchForPCCH() 
 *                allocatePdcchForRACH() and allocatePdcchForCCCH() for allocating
 *                CCEs
 ******************************************************************************/
UInt32 allocatePdcchCommonChannel(
#ifdef FDD_CONFIG
#endif
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32     delayToPhy,
#ifdef TDD_CONFIG
        UInt8      subFrameNum,
        UInt16     sysFrameNum,
        /* +- SPR 17777 */
        tickType_t currentGlobalTick,
#endif
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
/* SPR 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p
/* SPR 5492 changes end */
        ,InternalCellIndex internalCellIndex
        )
{
    /* + PRS_CHANGES */
    UInt32 rarQueueCount = 0;
    DLRARspQueueNode *raRspQnode_p = PNULL;
    /* - PRS_CHANGES */

    changeCFIInThisTTI_g[internalCellIndex] = 0;
    /*This will store the number of CCE used for a DCI Format*/
/* SPR 5333 changes start */
#ifdef TDD_CONFIG
    UInt32 maxCommonSpaceCCE = 0;
    if(getSubframeType(subFrameNum, internalCellIndex)== SP)
    {   
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        maxCommonSpaceCCE = maxCCEInCommonSpace_g[internalCellIndex][1][miValueForSubframe_g[internalCellIndex]];
        /*CA TDD CHANGES END*/
        /** CA-TDD Changes Stop **/

    /* SPR_7516_FIX */
        if (currentCFIValue_g[internalCellIndex] > 2)
	{
          currentCFIValue_g[internalCellIndex] = 2;
	}    
    /* SPR_7516_FIX */
    }
    else
    {
        /** CA-TDD Changes Start **/
        /*CA TDD CHANGES START*/
        maxCommonSpaceCCE = maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1][miValueForSubframe_g[internalCellIndex]];
        /*CA TDD CHANGES END*/
        /** CA-TDD Changes Stop **/
        currentCFIValue_g[internalCellIndex] = valueCFI_g[internalCellIndex];
#ifdef LTE_EMBMS_SUPPORTED
        if((isEmbmsValidSf_g[internalCellIndex] == TRUE) &&
             (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
        {
            UInt8 eMbmsCfiValue = embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
			maxCommonSpaceCCE = maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1][miValueForSubframe_g[internalCellIndex]];
            currentCFIValue_g[internalCellIndex] = eMbmsCfiValue;
        }

#endif
    }
#else
    UInt32 maxCommonSpaceCCE = maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1];
     currentCFIValue_g[internalCellIndex] = valueCFI_g[internalCellIndex];

#ifdef LTE_EMBMS_SUPPORTED
     if((isEmbmsValidSf_g[internalCellIndex] == TRUE) &&
             (currAreaIndex_g[internalCellIndex] != INVALID_AREA_INDEX))
     {
         /* SPR 18482 fix start*/
         UInt8 eMbmsCfiValue = embmsAreaContext_g[internalCellIndex][currAreaIndex_g[internalCellIndex]].\
                               mcchAreaContext_p->nonMBSFNregionLength;
         /* SPR 18482 fix end*/
         maxCommonSpaceCCE = maxCCEInCommonSpace_g[internalCellIndex][eMbmsCfiValue - 1];
         currentCFIValue_g[internalCellIndex] = eMbmsCfiValue;
     }

#endif
/* SPR 5333 changes end */
    UInt32 sysFrameNum = 0;
    UInt32 subFrameNum = 0; 

    getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

    /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNum = subFrameNum + delayToPhy;
    if(subFrameNum >= MAX_SUB_FRAME)
    {
        subFrameNum -= MAX_SUB_FRAME;
        sysFrameNum++;
        if (sysFrameNum == MAX_SFN_VALUE)
        {
            sysFrameNum = 0;
        }
    }
#endif

    LTE_MAC_UT_LOG(LOG_INFO ,PDCCH_MGR,
            "[%s]: maxCommonSpaceCCE [%d] \n"
            "                            sysFrameNum [%d]\n"
            "                            subFrameNum [%d]\n",
            __func__,maxCommonSpaceCCE,sysFrameNum,subFrameNum);

#ifdef FDD_CONFIG
#endif
#ifdef LTE_EMBMS_SUPPORTED
    if (maxCommonSpaceCCE && *availableCCE_p  && (isDci1cOcassion_g[internalCellIndex] == TRUE))
    {
        allocatePdcchForDCI1CNotification(&maxCommonSpaceCCE,
                dciCCEContainerInfo_p,
                availableCCE_p,
                internalCellIndex);
    }
#endif
    {
        if (maxCommonSpaceCCE && *availableCCE_p)
        {
            allocatePdcchForBCCH(pdcchDLOutputInfo_p,
                    &maxCommonSpaceCCE,
                    dciCCEContainerInfo_p,
                    availableCCE_p,
                    availableDLRBCount_p,
                    sysFrameNum,
                    subFrameNum,
                    internalCellIndex);
        }

        /*Even the CCE/RBs not available it is necessary to call this 
         *function to free the paging message intended for this SFN and SF
         */
        allocatePdcchForPCCH(pdcchDLOutputInfo_p, 
                &maxCommonSpaceCCE, 
                dciCCEContainerInfo_p,
                availableCCE_p,
                availableDLRBCount_p,
                sysFrameNum,
                subFrameNum,
                internalCellIndex);

        /*Even the CCE/RBs not available it is necessary to call this 
         *function to send the negetive RAR ack to the UL scheduler
         */

        /* + PRS_CHANGES */
        /* if it is PRS occasion then put all the RAR nodes in 
         * ULNegAckRARspQueue
         */ 
 
#ifndef LTE_EMBMS_SUPPORTED
    if ((!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
        (!isUeTypeInvalid_g[internalCellIndex]))
#else
    if ((!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
        (!isUeTypeInvalid_g[internalCellIndex]) &&
        (isEmbmsValidSf_g[internalCellIndex] == FALSE))
#endif 
        {
            allocatePdcchForRACH(pdcchDLOutputInfo_p,
                    &maxCommonSpaceCCE,
                    &dciCCEContainerInfo_p->dciCCEInfo[0], 
                    availableCCE_p,
                    availableDLRBCount_p,
                    subFrameNum,
                    dciCCEContainerInfo_p,
                    pdcchULOutputInfo_p,
                    /* +- SPR 17777 */
#ifdef TDD_CONFIG
                    (currentGlobalTick+delayToPhy),
#endif
                    /* SPR 5492 changes start */
                    maxUeScheduledInDownlink_p
                    /* +- SPR 17777 */
                    /* SPR 5492 changes end */
                    ,internalCellIndex
                    );
        }
        else 
        {
            rarQueueCount = COUNT_DL_RA_RSP_QUEUE_Q(internalCellIndex);
            while (rarQueueCount)
            {
                DEQUEUE_DL_RA_RSP_QUEUE_Q(DLRARspQueueNode, (void**)&raRspQnode_p, internalCellIndex);
                if (PNULL == raRspQnode_p)
                {
                    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                            "[%s]: Node poped from dlRARspQueue_g Queue is NULL\n",
                            __func__);
                    break;
                }
                putEntryInULNegAckRARspQueue(raRspQnode_p, internalCellIndex);
                rarQueueCount--;
            }
        }
#ifndef LTE_EMBMS_SUPPORTED
        /* If it is not PRS occasion then only process this queue */
    if (((!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
          maxCommonSpaceCCE && *availableCCE_p) && 
         (!isUeTypeInvalid_g[internalCellIndex]))
#else
    if (((!allocateCCEsForSIPagingULOnly[internalCellIndex]) && 
          maxCommonSpaceCCE && *availableCCE_p) && 
         (!isUeTypeInvalid_g[internalCellIndex]) &&
         (isEmbmsValidSf_g[internalCellIndex] == FALSE))
#endif
            /* - PRS_CHANGES */
        {
            /*CCCH messages comes under UE specific search space*/
            allocatePdcchForCCCH(pdcchDLOutputInfo_p,
                    &maxCommonSpaceCCE, 
                    dciCCEContainerInfo_p,
                    availableCCE_p,
                    availableDLRBCount_p,
                    subFrameNum ,
                    pdcchULOutputInfo_p
                    /* SPR 5492 changes start */
                    ,maxUeScheduledInDownlink_p
                    /* SPR 5492 changes end */,
                    internalCellIndex
                    );

        }
    }

    return maxCommonSpaceCCE;
    /* CLPC_CHG END */
}
#ifdef FDD_CONFIG
#endif

/******************************************************************************
 * Function Name: allocatePdcchForBCCH
 * Inputs       : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                     the information of UEs for which PDCCH region has been 
 *                     allocated and for which PDSCH region need to be allocated
 *                maxCommonSpaceCCE_p - CCE available in Common space
 *                dciCCEContainerInfo_p - DCI CCE container contains the Info of 
 *                                      UE for which CCE allocated
 *                availableCCE_p - Available CCE in UE specific space
 *                availableDLRBCount_p - Available RB count
 *                sysFrameNum - SFN w.r.t DL delay
 *                subFrameNum - SF w.r.t DL delay
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                     the information of UEs for which PDCCH region has been 
 *                     allocated and for which PDSCH region need to be allocated
 *                maxCommonSpaceCCE_p - CCE available in Common space
 *                availableDLRBCount_p - Available RB count
 *                dciCCEContainerInfo_p - DCI CCE container contains the Info of 
 *                                      UE for which CCE allocated
 * Returns      : None
 * Description  : This will call scheduleBroadcastMsgs() and allocate the CCE for 
 *                BCCH messages from Common search space
 ******************************************************************************/
void allocatePdcchForBCCH(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 *maxCommonSpaceCCE_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 sysFrameNum,
        UInt32 subFrameNum,
        InternalCellIndex internalCellIndex
        )
{
    /* This will store the number of CCE used for a DCI Format */
    UInt32 aggregationLevel = 0;
    /*Allocated CCE*/
    UInt32 cceAllocated     = 0;
    UInt32 count            = 0;
    UInt32 tmpCount         = 0;
    UInt32 macRetType       = MAC_FAILURE;
    DLMsgInfo  *dlMsgInfo_p = PNULL;
    DciCCEInfo *dciCCEInfo_p = PNULL;

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    tmpCount = 0;
    tmpCount =  scheduleBroadcastMsgs(pdcchDLOutputInfo_p, 
            dciCCEContainerInfo_p,
            maxCommonSpaceCCE_p, 
            sysFrameNum,
            subFrameNum,
            availableDLRBCount_p,
            internalCellIndex);

    if (tmpCount)
    {
        count = pdcchDLOutputInfo_p->countCommonChannelMsg;
        aggregationLevel = AGGREGATION_LEVEL_FOR_COMMON_SPACE;

        /* SPR 6856 Changes start */
	/* Try allocating in common search space */
        macRetType = allocatePDCCHForDCIFormatCommonSpace (
			  	             aggregationLevel,&cceAllocated, 
			  	             availableCCE_p, dciCCEInfo_p,
                                             internalCellIndex);
        /* SPR 6856 Changes end */

        if (MAC_SUCCESS == macRetType)
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "[%s]: CCE allocated successfully cceAllocated [%d]\n",
                    __func__,cceAllocated);
            dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = 
                aggregationLevel;
            dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
            dciCCEContainerInfo_p->arrayCCEAllocated
                [dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
            dciCCEContainerInfo_p->countOfCCEAllocated++;
            dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = SIRNTI;
            /* CLPC_CHG */
            dciCCEInfo_p[cceAllocated].dciPduInfo.txPower = 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->commonDLPowerControlInfo.pbchTransmissionPower;
            /* CLPC_CHG END */
            /* SPR 893 Fix start*/
            dciCCEInfo_p[cceAllocated].dciPduInfo.numLayer = MIN_DLSCH_NUM_LAYER; 
            /* SPR 893 Fix end*/

            /* SPR 17112 Fix Start */
            /* SPR 903 Fix start*/
            dciCCEInfo_p[cceAllocated].dciPduInfo.ueCategory  = MAC_MIN_UE_CATEGORY ;
            /* SPR 903 Fix Endt*/
            /* SPR 17112 Fix End */

            /* CLPC_CHG */
            dciCCEInfo_p[cceAllocated].dciPduInfo.pA = DEFAULT_PA_VALUE;
            /* CLPC_CHG END */
            /* ICIC changes start */
            pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = BCCH_MSG;
            dlMsgInfo_p = 
                &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
            /* ICIC changes  end */
            dlMsgInfo_p->dlBCCHMsgInfo.cceIndex   = cceAllocated;
            dlMsgInfo_p->dlBCCHMsgInfo.resourceAllocInput.preferredRATPolicy = 
                                                                               RA_POLICY_2_DISTRIBUTED;
            dciCCEInfo_p[cceAllocated].dciPduInfo.dataTB_p = 
                                                             dlMsgInfo_p->dlBCCHMsgInfo.data_p;

            dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = INVALID_HARQ_ID;

            dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.tbSize = 
                                                                   dlMsgInfo_p->dlBCCHMsgInfo.dataLen;

			
			/* SPR 1488 + SPR 1589 + SPR 1686 Starts */
			if ( GAP_VALUE_ONE == cellSpecificParams_g.cellConfigAndInitParams_p\
                                     [internalCellIndex]->cellParams_p->gapValue)
			{
                dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = 0;
			}
			else
			{
                dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = 1;
			}
			/* SPR 1488 + SPR 1589 + SPR 1686 Ends	*/



            pdcchDLOutputInfo_p->countCommonChannelMsg++;
        }
        else
        {
            /*SPR 3844 Start*/
            DLBCCHMsgInfo *bcchMsgInfo_p = PNULL;
            /* ICIC changes start */
            /*SPR 6720 Changes Start*/
            bcchMsgInfo_p = &pdcchDLOutputInfo_p->\
                dlMsgTypeCommonMsgInfo[pdcchDLOutputInfo_p->
                countCommonChannelMsg].dlMsgInfo.dlBCCHMsgInfo;
            /*SPR 6720 Changes End*/
            /* ICIC changes end */
            msgFree(bcchMsgInfo_p->data_p);
            /*SPR 3844 End*/
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                    __func__,aggregationLevel);

            *maxCommonSpaceCCE_p -= aggregationLevel;
            return;
        }
    }

}

/******************************************************************************
 * Function Name: allocatePdcchForPCCH
 * Inputs       : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                     the information of UEs for which PDCCH region has been 
 *                     allocated and for which PDSCH region need to be allocated
 *                maxCommonSpaceCCE_p - CCE available in Common space
 *                dciCCEContainerInfo_p - DCI CCE container contains the Info of 
 *                                      UE for which CCE allocated
 *                availableCCE_p - Available CCE in UE specific space
 *                availableDLRBCount_p - Available RB count
 *                sysFrameNum - SFN w.r.t DL delay
 *                subFrameNum - SF w.r.t DL delay
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                     the information of UEs for which PDCCH region has been 
 *                     allocated and for which PDSCH region need to be allocated
 *                maxCommonSpaceCCE_p - CCE available in Common space
 *                availableDLRBCount_p - Available RB count
 *                dciCCEContainerInfo_p - DCI CCE container contains the Info of 
 *                                      UE for which CCE allocated
 * Returns      : None
 * Description  : This will call schedulePCCHMsg() and allocate the CCE for 
 *                Paging message from Common search space
 ******************************************************************************/
void allocatePdcchForPCCH(PdcchDLOutputInfo *pdcchDLOutputInfo_p, 
        UInt32 *maxCommonSpaceCCE_p, 
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 sysFrameNum,
        UInt32 subFrameNum,
        InternalCellIndex internalCellIndex)
{
    /* This will store the number of CCE used for a DCI Format */
    UInt32 aggregationLevel  = 0;
    /*Allocated CCE*/
    UInt32 cceAllocated     = 0;
    UInt32 count            = 0;
    UInt32 tmpCount         = 0;
    UInt32 macRetType        = MAC_FAILURE;
    DLMsgInfo  *dlMsgInfo_p  = PNULL;
    DciCCEInfo *dciCCEInfo_p = PNULL;

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    tmpCount = schedulePCCHMsg(
            pdcchDLOutputInfo_p, 
            maxCommonSpaceCCE_p, 
            availableDLRBCount_p,
            subFrameNum,
            sysFrameNum,
            internalCellIndex);

    if (tmpCount)
    {
        count = pdcchDLOutputInfo_p->countCommonChannelMsg;
        aggregationLevel = AGGREGATION_LEVEL_FOR_COMMON_SPACE;  

        /* SPR 6856 Changes start */
	/* Try allocating in common search space */
        macRetType = allocatePDCCHForDCIFormatCommonSpace (
			  	             aggregationLevel,&cceAllocated, 
			  	             availableCCE_p, dciCCEInfo_p,
                             internalCellIndex);
        /* SPR 6856 Changes end */

        if (MAC_SUCCESS == macRetType)
        {
            dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = aggregationLevel;
            dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
            dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = PRNTI;
            /* SPR 893 Fix start*/
            /* + SPR_7309 */ 
            dciCCEInfo_p[cceAllocated].dciPduInfo.numLayer = cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
            /* - SPR_7309 */ 
            /* SPR 893 Fix end*/

            /* CLPC_CHG */
            dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
               cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
               cellParams_p->commonDLPowerControlInfo.pchTransmissionPower;
            /* CLPC_CHG END */

            /* SPR 17112 Fix Start */
            /* SPR 903 Fix start*/
            dciCCEInfo_p[cceAllocated].dciPduInfo.ueCategory  = MAC_MIN_UE_CATEGORY ;
            /* SPR 903 Fix Endt*/
            /* SPR 17112 Fix End */

            /* CLPC_CHG */
            dciCCEInfo_p[cceAllocated].dciPduInfo.pA = DEFAULT_PA_VALUE;
            /* CLPC_CHG END */

            dciCCEContainerInfo_p->arrayCCEAllocated
                [dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
            dciCCEContainerInfo_p->countOfCCEAllocated++;

            /* ICIC changes start */
            pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = PCCH_MSG;
            /* ICIC changes end */

            dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = INVALID_HARQ_ID;

			/* SPR 1488 + SPR 1589 + SPR 1686 Starts */
			if ( GAP_VALUE_ONE == cellSpecificParams_g.cellConfigAndInitParams_p\
                                          [internalCellIndex]->cellParams_p->gapValue)
			{
                dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = 0;
			}
			else
			{
                dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = 1;
			}
			/* SPR 1488 + SPR 1589 + SPR 1686 Ends	*/

            /* ICIC changes start */
            dlMsgInfo_p = 
                &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;
            /* ICIC changes end */
            dlMsgInfo_p->dlPCCHMsgInfo.cceIndex = cceAllocated;
            dlMsgInfo_p->dlPCCHMsgInfo.resourceAllocInput.preferredRATPolicy = \
                                                                               RA_POLICY_2_DISTRIBUTED;
            pdcchDLOutputInfo_p->countCommonChannelMsg ++;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                    "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                    __func__,aggregationLevel);
            /* + SPR_17858_506_CHANGES */
            freePagingMsg(sysFrameNum,subFrameNum,internalCellIndex);
            /* - SPR_17858_506_CHANGES */

            /* ICIC changes start */
            /* SPR 6022 Changes Start */
            msgFree((void *)pdcchDLOutputInfo_p->\
                    dlMsgTypeCommonMsgInfo[count].dlMsgInfo.dlPCCHMsgInfo.data_p);
            /* + SPR_17858_506_CHANGES */
            pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo.dlPCCHMsgInfo.data_p = PNULL;
            /* - SPR_17858_506_CHANGES */
            /* SPR 6022 Changes End */
            /* ICIC changes end */

            *maxCommonSpaceCCE_p -= aggregationLevel;
            return;
        }
    }
}

/******************************************************************************
 * Function Name: allocatePdcchForRACH
 * Inputs       : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                     the information of UEs for which PDCCH region has been 
 *                     allocated and for which PDSCH region need to be allocated
 *                maxCommonSpaceCCE_p - CCE available in Common space
 *                dciCCEInfo_p - DCI CCE Info of UE for which CCE allocated
 *                availableCCE_p - Available CCE in UE specific space
 *                availableDLRBCount_p - Available RB count
 *                subFrameNum - 
 *                dciCCEContainerInfo_p -
 *                pdcchULOutputInfo_p -
 *                currentGlobalTick -
 *                maxUeScheduledInDownlink_p -
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : pdcchDLOutputInfo_p, maxCommonSpaceCCE_p, availableDLRBCount_p
 *                dciCCEInfo_p
 * Returns      : None
 * Description  : This will call the scheduleRACHMsg() which schedule the RAR and
 *                Contention resolution messages and allocates the CCEs
 ******************************************************************************/
void allocatePdcchForRACH(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 *maxCommonSpaceCCE_p,
        DciCCEInfo *dciCCEInfo_p, 
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        tickType_t currentGlobalTick,
#endif
        /* SPR 15909 fix end */
/* SPR 5492 changes start */
        UInt32 *maxUeScheduledInDownlink_p
        /* +- SPR 17777 */
        /* SPR 5492 changes end */
        ,InternalCellIndex internalCellIndex
        )
{
    scheduleRACHMsg(pdcchDLOutputInfo_p,
            dciCCEInfo_p,
            availableCCE_p,
            maxCommonSpaceCCE_p, 
            availableDLRBCount_p,
            subFrameNum,
            dciCCEContainerInfo_p,
            pdcchULOutputInfo_p,
            /* +- SPR 17777 */
#ifdef TDD_CONFIG
            currentGlobalTick,
#endif
            /* SPR 5492 changes start */
            maxUeScheduledInDownlink_p
            /* +- SPR 17777 */
            /* SPR 5492 changes end */
            ,internalCellIndex
            );
}

/******************************************************************************
 * Function Name: allocatePdcchForCCCH
 * Inputs       : pdcchDLOutputInfo_p - Pointer to structure which holds 
 *                     the information of UEs for which PDCCH region has been 
 *                     allocated and for which PDSCH region need to be allocated
 *                maxCommonSpaceCCE_p - CCE available in Common space
 *                dciCCEContainerInfo_p - DCI CCE container contains the Info of 
 *                                      UE for which CCE allocated
 *                availableCCE_p - Available CCE in UE specific space
 *                availableDLRBCount_p - Available RB count
 *                subFrameNum -
 *                maxUeScheduledInDownlink_p -
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : pdcchULOutputInfo_p, maxCommonSpaceCCE_p, availableDLRBCount_p
 *                dciCCEContainerInfo_p
 * Returns      : None
 * Description  : This will call the scheduleCCCHMsg() to schedule the CCCH 
 *                messages and allocates the CCEs
 ******************************************************************************/
void allocatePdcchForCCCH(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        UInt32 *maxCommonSpaceCCE_p, 
        DciCCEContainerInfo * dciCCEContainerInfo_p,
        UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        )
{
    /* SPR 4395 changes start */
    schedulePendingCCCHMsg(pdcchDLOutputInfo_p,
            dciCCEContainerInfo_p,
            maxCommonSpaceCCE_p,
            availableCCE_p,
            availableDLRBCount_p,
            subFrameNum,
            pdcchULOutputInfo_p
/* 5492 changes start */
            ,maxUeScheduledInDownlink_p,
            /* 5492 changes end */
            internalCellIndex
            );
    /* SPR 4395 changes end */

    scheduleCCCHMsg(pdcchDLOutputInfo_p, 
            dciCCEContainerInfo_p, 
            maxCommonSpaceCCE_p, 
            availableCCE_p, 
            availableDLRBCount_p, 
            subFrameNum,
            pdcchULOutputInfo_p
/* 5492 changes start */
            ,maxUeScheduledInDownlink_p,
            /* 5492 changes end */
            internalCellIndex
            );
}



/*****************************************************************************
 * Function Name  : putEntryInDLSchedulerNewTxQueue
 * Inputs         : dlStrategyTxNode_p - This is pointer to DLStrategyTxNode
 *                  structure
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the DLStrategyTxNode Node in the 
 *                  dlStrategyNewTxQueue_g Queue
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType putEntryInDLSchedulerNewTxQueue(
/* SPR 5599 changes end (ZIP ID 129195) */            
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex)
{
    dlStrategyTxNode_p->numPushedInAdvanceQueue = 0;

    if (pushNode(dlStrategyNewTxQueue_gp[internalCellIndex], &(dlStrategyTxNode_p->dlStrategyTxNodeAnchor)))
    {
        freeMemPool(dlStrategyTxNode_p);
        dlStrategyTxNode_p = PNULL;
        return MAC_FAILURE ;
    }

    LOG_MAC_MSG(MAC_PUT_ENTRY_IN_DLSCHEDULER_NEWTX_QUEUE_ID,LOGBRIEF,MAC_DL_Strategy,
            getCurrentTick(),
            dlStrategyTxNode_p->ueIndex,
            dlStrategyTxNode_p->harqProcessId,
            dlStrategyTxNode_p->msgType,
            dlStrategyTxNode_p->minTBSize,
            dlStrategyTxNode_p->maxTBSize,
            0.0,0.0,
            FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInDLSchedulerReTxQueue
 * Inputs         : dlStrategyTxNode_p - This is pointer to DLStrategyTxNode
 *                  structure
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the DLStrategyTxNode Node in the 
 *                     dlStrategyReTxQueue_g Queue
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType putEntryInDLSchedulerReTxQueue(
/* SPR 5599 changes end (ZIP ID 129195) */            
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex)
{
    dlStrategyTxNode_p->numPushedInAdvanceQueue = 0;

    if(pushNode(dlStrategyReTxQueue_gp[internalCellIndex],
                &(dlStrategyTxNode_p->dlStrategyTxNodeAnchor)))
    {
        ltePanic("putEntryInDLSchedulerReTxQueue fail!!\n");
        return MAC_FAILURE;
    }
    
    LOG_MAC_MSG(MAC_PUT_ENTRY_IN_RETX_FAILURE_QUEUE_ID,LOGINFO,MAC_DL_HARQ,
            getCurrentTick(),
            dlStrategyTxNode_p->ueIndex,
            dlStrategyTxNode_p->harqProcessId,
            dlStrategyTxNode_p->msgType,
            dlStrategyTxNode_p->harqProcess_p->isTBOneValid,
            dlStrategyTxNode_p->harqProcess_p->isTBTwoValid,
            0.0,0.0,
            FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInULSchedulerNewTxQueue
 * Inputs         : ulStrategyTxNode_p - This is pointer to ULStrategyTxNode
 *                  structure
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the ulStrategyTxNode_p Node in the 
 *                     ulStrategyTxQueue_g Queue
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType putEntryInULSchedulerNewTxQueue(
/* SPR 5599 changes end (ZIP ID 129195) */            
        ULStrategyTxNode *ulStrategyTxNode_p,
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        ,UInt8 ulSubFrameNum
        ,ULUEContext   *ueULContext_p
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* + coverity 32791, 32792 */
    MacRetType ratval = MAC_SUCCESS;
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt8 subFrameNum = (currentGlobalTTITickCount % MAX_SUB_FRAME);
    /* SPR 23393 Changes End */
    /* Updating the Scheduling count and ulSubFrameNum in Straegy node */
    ulStrategyTxNode_p->ulSubFrameNum = ulSubFrameNum;
    ulStrategyTxNode_p->scheduledCount++;
    /* Check if current subframe is TDD config 0, S subframe */
    if (LTE_TRUE == isTddConfig0SsubFrame(subFrameNum,internalCellIndex))
    {
        /* Put Entry in ULSchedulerNewTxList */
        ratval = putEntryInULSchedulerNewTxList(
                subFrameNum,
                ulSubFrameNum,
                ulStrategyTxNode_p,
                ueULContext_p,
                /* CA TDD CHANGES START */
                internalCellIndex
                /* CA TDD CHANGES END */
                );

    } else
#endif
        /* TDD Config 0 Changes End */
#ifndef DL_UL_SPLIT
    if (pushNode(ulStrategyTxQueue_gp[internalCellIndex],
                 &(ulStrategyTxNode_p->ulStrategyTxNodeAnchor)))
    {
        ratval = MAC_FAILURE;
    }
#else
    {
    ratval = putEntryInULSchedulerNewTxQueueUsingICC(ulStrategyTxNode_p,internalCellIndex);
    }
#endif

    LOG_MAC_MSG(MAC_PUT_ENTRY_IN_ULSCHEDULER_NEWTX_QUEUE_ID,LOGBRIEF,MAC_UL_Strategy,
            getCurrentTick(),
            ulStrategyTxNode_p->ueIndex,
            ulStrategyTxNode_p->ulHarqProcessId,
            ulStrategyTxNode_p->requiredRB,
            ulStrategyTxNode_p->minDataSize,
            ulStrategyTxNode_p->maxDataSize,
            0.0,0.0,
            FUNCTION_NAME,"");

    return ratval;
    /* - coverity 32791, 32792 */
}
/*****************************************************************************
 * Function Name  : putEntryInAdvanceAllocUEInfoQueue
 * Inputs         : dlStrategyTxNode_p - This is pointer to DLStrategyTxNode
 *                  structure
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the DLStrategyTxNode Node in the 
 *                     advanceAllocInfoQueue_g Queue
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType putEntryInAdvanceAllocInfoQueue(
/* SPR 5599 changes end (ZIP ID 129195) */            
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex)
{
/* SPR 3430 */
    DLUEContext *ueContext_p;
    DLHARQProcess *harqProcess_p;
    ueContext_p =dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].dlUEContext_p; 
    /* + SPR 11469 Changes */
    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
    {
        harqProcess_p = &ueContext_p->dlHarqContext_p->harqProcess[
            dlStrategyTxNode_p->harqProcessId];
    }
    else
    {
        harqProcess_p = &ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->\
            ScelldlHarqContext_p->harqProcess[dlStrategyTxNode_p->harqProcessId];
    }
    /* - SPR 11469 Changes */
    DLAdvanceQueueNode *dlAdvNode_p   = PNULL;
                        
/* SPR 3430 */
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Entering function = %s  \n",__func__);
/* SPR 3430 */    
    if (dlStrategyTxNode_p->allocPushCount < 4)
    {  
        GET_MEM_FROM_POOL(DLAdvanceQueueNode, dlAdvNode_p,
                sizeof(DLAdvanceQueueNode), PNULL);
        /* +coverity-530-CID 24492*/
        if(PNULL == dlAdvNode_p)
        {
            return MAC_FAILURE;
        }
        /* +coverity-530-CID 24492*/
        dlAdvNode_p->isNodeInvalid        = FALSE;
        dlAdvNode_p->dlStrategyTxNode     = dlStrategyTxNode_p;
        /* COVERITY CID 25444 Fix start */
        if (pushNode(advanceAllocInfoQueue_gp[internalCellIndex],
            &(dlAdvNode_p->dlAdvanceAnchorNode)) )
        {
            lteWarning ("pushNode() failed for advance Alloc Info Queue");
            freeMemPool(dlAdvNode_p);
            return MAC_FAILURE;
        }
        /* COVERITY CID 25444 Fix end */
        dlStrategyTxNode_p->allocPushCount++;
   }
    else
    {
        freeDLHarqProcess(ueContext_p,harqProcess_p,internalCellIndex);
        /* SPR 20417 Fix start */
        freeMemPool(dlStrategyTxNode_p);
        /* SPR 20417 Fix end */
        /* SPR# 4906 Changes Starts   */
        if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
        {
            ueContext_p->dlStrategyTxNode_p = PNULL;
        }
        else
        {
            ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = PNULL;
        }
        /* SPR# 4906 Changes Ends   */
    }
/* SPR 3430 */
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Leaving function = %s  \n",__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInRetransmissionFailureQueue
 * Inputs         : dlStrategyTxNode_p - This is pointer to DLStrategyTxNode
 *                  structure
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the DLStrategyTxNode Node in the 
 *                     reTransmissionFailureQueue_g 
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType putEntryInRetransmissionFailureQueue(
/* SPR 5599 changes end (ZIP ID 129195) */            
        DLStrategyTxNode *dlStrategyTxNode_p,
        InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Entering function = %s  \n",__func__);

    DLRetxFailureQueueNode *dlRetxFailureQueueNode_p  = PNULL;

    GET_MEM_FROM_POOL(DLRetxFailureQueueNode, dlRetxFailureQueueNode_p,
            sizeof(DLRetxFailureQueueNode), PNULL);
    /*coverity-530 CID 24494*/
    if(PNULL == dlRetxFailureQueueNode_p)
    {
       return MAC_FAILURE;
    }
    /*coverity-530 CID 24494*/
    dlRetxFailureQueueNode_p->ueIndex = dlStrategyTxNode_p->ueIndex;
    dlRetxFailureQueueNode_p->isNodeInvalid = FALSE;

    dlRetxFailureQueueNode_p->dlStrategyTxNode_p = dlStrategyTxNode_p;

      /* + SPR 12085 Changes */
      if( (dlStrategyTxNode_p->harqProcess_p->isTBOneValid ) &&
          (dlStrategyTxNode_p->harqProcess_p->dlHARQTBOneInfo.txIndicator == NEW_TX))
      {
          freeDLHarqTB(
                    dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].dlUEContext_p,
                    dlStrategyTxNode_p->harqProcess_p,
                    TB_ONE, 
                    internalCellIndex);
      }
      else if((dlStrategyTxNode_p->harqProcess_p->isTBTwoValid ) &&
          (dlStrategyTxNode_p->harqProcess_p->dlHARQTBTwoInfo.txIndicator == NEW_TX))
      {
          freeDLHarqTB(
                    dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].dlUEContext_p,
                    dlStrategyTxNode_p->harqProcess_p,
                    TB_TWO, 
                    internalCellIndex);
      }
      /* - SPR 12085 Changes */

    /* COVERITY CID 25445 Fix start */
    if ( pushNode(dlRetransmissionFailureQueue_gp[internalCellIndex],
            &(dlRetxFailureQueueNode_p->anchorNode)) )
    {
        lteWarning ("pushNode() failed for dlRetransmission Failure Queue");
        freeMemPool(dlRetxFailureQueueNode_p);
        return MAC_FAILURE;
    }
    /* COVERITY CID 25445 Fix end */

    if(IS_PCELL_OF_UE(dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].dlUEContext_p, internalCellIndex ))
    {
        dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].dlUEContext_p->\
            dlStrategyTxNode_p = PNULL;
    }
    else
    {
        dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex].dlUEContext_p->\
            dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = PNULL;
    }

	
	LOG_MAC_MSG(MAC_PUT_ENTRY_IN_RETX_FAILURE_QUEUE_ID,LOGINFO,MAC_DL_HARQ,
			getCurrentTick(),
			dlStrategyTxNode_p->ueIndex,
			dlStrategyTxNode_p->harqProcessId,
			dlStrategyTxNode_p->msgType,
			dlStrategyTxNode_p->harqProcess_p->isTBOneValid,
			dlStrategyTxNode_p->harqProcess_p->isTBTwoValid,
			0.0,0.0,
			FUNCTION_NAME,"");

    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,
            "Leaving function = %s  \n",__func__);
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat2A 
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  rbgSize - The holds Resources Block Group Size
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate - This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
        /* spr 23717 fix + */
STATIC   void  calculateCCEsDCIFormat2A(
        /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
    resourceAllocationBits = ceil_wrapper( (UDouble32)dlRBs / rbgSize );
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "resourceAllocationBits  = %u\n",resourceAllocationBits);
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_2A; 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "totalBits  = %u\n",totalBits);
    bitsAfterBlockCoding = totalBits * codingRate;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "bitsAfterBlockCoding  = %u\n",bitsAfterBlockCoding);
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);  
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "InValid Number of CCEs\n");
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "numCCE_p  = %u\n",*numCCE_p);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}
/* + TM7_8 Changes Start */
/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat2B 
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  rbgSize - The holds Resources Block Group Size
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate- This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
 /* spr 23717 fix + */
 STATIC void  calculateCCEsDCIFormat2B(
 /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;
    resourceAllocationBits = ceil_wrapper( (UDouble32)dlRBs / rbgSize );
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_2B; 
    bitsAfterBlockCoding = totalBits * codingRate;
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);  
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
}

/* - TM7_8 Changes End */


/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat2
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  rbgSize - The holds Resources Block Group Size
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate- This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
        /* spr 23717 fix + */
STATIC  void  calculateCCEsDCIFormat2(
        /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
    resourceAllocationBits = ceil_wrapper( (UDouble32)dlRBs / rbgSize );
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "resourceAllocationBits  = %u\n",resourceAllocationBits);
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_2; 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "totalBits  = %u\n",totalBits);
    bitsAfterBlockCoding = totalBits * codingRate;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "bitsAfterBlockCoding  = %u\n",bitsAfterBlockCoding);
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "InValid Number of CCEs\n");
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "numCCE_p  = %u\n",*numCCE_p);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}

/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat1B
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate- This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
        /* spr 23717 fix + */
STATIC  void  calculateCCEsDCIFormat1B(
        /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     modulationScheme,
        UInt32 codingRate
		)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;


    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
        /*SPR 20899 Fix Start*/
    resourceAllocationBits = ceil_wrapper( (UDouble32)(log_wrapper(DIVIDE_BY_TWO(dlRBs * (dlRBs + 1)) )) 
            / log_wrapper(2) );
        /*SPR 20899 Fix Stop*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "resourceAllocationBits  = %u\n",resourceAllocationBits);
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_1B; 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "totalBits  = %u\n",totalBits);
    bitsAfterBlockCoding = totalBits * codingRate;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "bitsAfterBlockCoding  = %u\n",bitsAfterBlockCoding);
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "InValid Number of CCEs\n");
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "numCCE_p  = %u\n",*numCCE_p);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}


/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat1A 
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate- This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
        /* spr 23717 fix + */
STATIC  void  calculateCCEsDCIFormat1A(
        /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     modulationScheme,
        UInt32 codingRate
		)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;


    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
        /*SPR 20899 Fix Start*/
    resourceAllocationBits = ceil_wrapper( (UDouble32)(log_wrapper(DIVIDE_BY_TWO(dlRBs * (dlRBs + 1)) )) 
            / log_wrapper(2) );
        /*SPR 20899 Fix Stop*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "resourceAllocationBits  = %u\n",resourceAllocationBits);
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_1A; 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "totalBits  = %u\n",totalBits);
    bitsAfterBlockCoding = totalBits * codingRate;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "bitsAfterBlockCoding  = %u\n",bitsAfterBlockCoding);
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "InValid Number of CCEs\n");
    }        
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "numCCE_p  = %u\n",*numCCE_p);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}


/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat1
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  rbgSize -
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate- This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
        /* spr 23717 fix + */
STATIC  void  calculateCCEsDCIFormat1(
        /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     dlRBs,
        UInt32     rbgSize,
        UInt32     modulationScheme,
        UInt32 codingRate
		)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
    resourceAllocationBits = ceil_wrapper( (UDouble32)dlRBs / rbgSize );
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "resourceAllocationBits  = %u\n",resourceAllocationBits);
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_1; 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "totalBits  = %u\n",totalBits);
    bitsAfterBlockCoding = totalBits * codingRate;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "bitsAfterBlockCoding  = %u\n",bitsAfterBlockCoding);
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);  
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "InValid Number of CCEs\n");
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "numCCE_p  = %u\n",*numCCE_p);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}

/*****************************************************************************
 * Function Name  : calculateCCEsDCIFormat0
 * Inputs         : dlRBs - The holds number of DownLink RBs allocated.
 *                  modulationScheme - The holds modulation Scheme
 *                  codingRate- This represents Coding Rate     
 * Outputs        : numCCE_p - This holds number of CCE need to fill the DCI Format.
 * Returns        : None
 * Description    : The function calculates the number of CCE allocated on the 
 *                  basis of the formula provided in the Technical Specification
 *                  TS 36.213. 
 *****************************************************************************/
        /* spr 23717 fix + */
STATIC  void  calculateCCEsDCIFormat0(
        /* spr 23717 fix - */
        UInt32     *numCCE_p,
        UInt32     ulRBs,
        UInt32     modulationScheme,
        UDouble32 codingRate

		)
{

    UInt32 resourceAllocationBits = 0;
    UInt32 totalBits = 0;
    UInt32 bitsAfterBlockCoding = 0;
    UInt32 numModSymb = 0;


    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
        /*SPR 20899 Fix Start*/
    resourceAllocationBits = ceil_wrapper( (UDouble32)(log_wrapper(DIVIDE_BY_TWO(ulRBs * (ulRBs + 1)))) 
            / log_wrapper(2) );
        /*SPR 20899 Fix Stop*/
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "resourceAllocationBits  = %u\n",resourceAllocationBits);
    /* The value corresponds to addtion of all the no. of mandatory bytes */
    totalBits = resourceAllocationBits + CONSTANT_BYTES_OF_DCIFORMAT_0; 
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "totalBits  = %u\n",totalBits);
    bitsAfterBlockCoding = totalBits * codingRate;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "bitsAfterBlockCoding  = %u\n",bitsAfterBlockCoding);
    numModSymb = ceil_wrapper((UDouble32)bitsAfterBlockCoding / modulationScheme);
    *numCCE_p = ceil_wrapper( (UDouble32)numModSymb / MAX_RE_IN_CCE);
    if ( *numCCE_p > 0 && *numCCE_p <= 1)
    {
        *numCCE_p = AGGREGATION_LEVEL_ONE;
    }
    else if ( *numCCE_p >1 && *numCCE_p <= 2)
    {
        *numCCE_p = AGGREGATION_LEVEL_TWO;
    }
    else if ( *numCCE_p >2 && *numCCE_p <=4)
    {
        *numCCE_p = AGGREGATION_LEVEL_FOUR;
    }
    else if ( *numCCE_p >4 && *numCCE_p <=8)
    {
        *numCCE_p = AGGREGATION_LEVEL_EIGHT;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "InValid Number of CCEs\n");
    }
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "numCCE_p  = %u\n",*numCCE_p);
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}

/*****************************************************************************
 * Function Name : calculateDCIFormatToCCEMapping
 * Inputs        : internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : The function call relevant funtions for finding the number of 
 *                 CCE for DCI Formats   
 *****************************************************************************/
STATIC void calculateDCIFormatToCCEMapping(InternalCellIndex internalCellIndex)
{
    UInt32 dlRBs = 0;
    UInt32 ulRBs = 0;
    UInt32 rbgSize = 0;
    UInt32 modulationScheme = 0;
    UInt32 codingRate = CODING_RATE_ONE_BY_THREE;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Entered function = %s\n", __func__);
    dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->dlAvailableRBs;
    ulRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->ulAvailableRBs;
    rbgSize = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo.rat0Info.rbgSize;
    modulationScheme = QPSK;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "dlRBs  = %u\n rbgSize  = %u\n ",dlRBs, rbgSize);
    /* ICIC changes start */

    /* Aggregation level for cell centre user is calculated based on formula 
     * given in TS 36.213. After this aggregation level for cell edge user is
     * derived by jumping cell centre aggregation level by 
     * CELL_EDGE_AGGR_LEVEL_OFFSET.
     * e.g if CELL_EDGE_AGGR_LEVEL_OFFSET is 2 then aggregation levels will be
     *          CC_USER      CE_USER     
     *             1            4 
     *             2            8
     *             4            8
     *             8            8
     */
    calculateCCEsDCIFormat2A(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_2A]),
            dlRBs,
            rbgSize,
            modulationScheme,
            codingRate
			);

    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2A] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_2A] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2A] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2A] = 
            AGGREGATION_LEVEL_EIGHT;
    }

    calculateCCEsDCIFormat2(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_2]),
            dlRBs,
            rbgSize,
            modulationScheme,
            codingRate
			);

    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_2] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2] = 
            AGGREGATION_LEVEL_EIGHT;
    }
    
    calculateCCEsDCIFormat1B(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1B]),
            dlRBs,
            modulationScheme,
            codingRate
			);
    
    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1B] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1B] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1B] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1B] = 
            AGGREGATION_LEVEL_EIGHT;
    }

    calculateCCEsDCIFormat1A(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1A]),
            dlRBs,
            modulationScheme,
            codingRate
        );

    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1A] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1A] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1A] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1A] = 
            AGGREGATION_LEVEL_EIGHT;
    }

    calculateCCEsDCIFormat1(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1]),
            dlRBs,
            rbgSize,
            modulationScheme,
            codingRate
			);

    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_1] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_1] = 
            AGGREGATION_LEVEL_EIGHT;
    }

    calculateCCEsDCIFormat0(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_0]),
            ulRBs,
            modulationScheme,
            codingRate
			);
    
    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_0] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_0] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_0] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_0] = 
            AGGREGATION_LEVEL_EIGHT;
    }
    /* ICIC changes end */

    /* + TM7_8 Changes Start */
    calculateCCEsDCIFormat2B(
            &(dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_2B]),
            dlRBs,
            rbgSize,
            modulationScheme,
            codingRate
		);

    dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2B] = 
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
        dciFormatToAggregationLevel[CC_USER][DCI_FORMAT_2B] * 
        pow_wrapper(2, CELL_EDGE_AGGR_LEVEL_OFFSET);

    if (dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2B] > 
            AGGREGATION_LEVEL_EIGHT)
    {
        dciFormatToAggrLvlMapping_g[internalCellIndex].\
            dciFormatToAggregationLevel[CE_USER][DCI_FORMAT_2B] = 
            AGGREGATION_LEVEL_EIGHT;
    }

    /* - TM7_8 Changes End */

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
            "Leaving function = %s\n", __func__);
}

/* CR changes start */
#ifdef FDD_CONFIG
/*****************************************************************************
 Function Name  : checkHIResources
 Inputs         : bitMaskOld,bitMaskNew 
 Outputs        : None
 Returns        : MAC_SUCCESS or MAC_FAILURE
 Description    : This function checks if same hi resources are 
		          available or not.
*****************************************************************************/
/* spr 23717 fix + */
 MacRetType checkHIResources(
/* spr 23717 fix - */
        UInt8 bitMaskOld,
        UInt8 bitMaskNew )                        
{
    if(bitMaskOld & bitMaskNew)
    {
        return MAC_SUCCESS; 
    }        
    else
    {
        return MAC_FAILURE; 
    }        
}
#endif
/* CR changes end */

/*********************************************************************************
 * Function Name : processULNackQForTempUE
 * Inputs        : ulDelay - The delay of execution legs in terms of the number
 *                           subFrames w.r.t. PHY & the current processing being
 *                           done at MAC for intended tick at PHY.
 *                 currentGlobalTick - The present Global Tick
 *                 availableCCE_p - This represent the number of available CCE
 *                 and if CCE's are allocated this would be deceremented by  one.
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                 Information is there and once cce is allocated ,updated 
 *                 information is written on the reserved CCE.
 *                 dciCCEContainerInfo_p -
 *                 maxUeScheduledInUplink_p -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None   
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:  : This function will read the Nack queue and store the  
 *                 information in the PDCCH UPLINK OUTPUT container. 
 ***********************************************************************************/
#ifdef FDD_CONFIG
#ifndef DL_UL_SPLIT
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQForTempUE(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* SPR 5798 Fix Start */
        UInt32 *maxUeScheduledInUplink_p,
        /* SPR 5798 Fix End */
        InternalCellIndex  internalCellIndex)
{
#else
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQForTempUE(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        ContainerNackQueueNode * nackNode_p,
        /* SPR 5798 Fix Start */
        UInt32 *maxUeScheduledInUplink_p,
        /* SPR 5798 Fix End */
        InternalCellIndex  internalCellIndex)
{
#endif
    UInt8  nackQContainerTick =0;
    UInt16 tempUeIndex=0;
    /* SPR 15909 fix start */
    tickType_t tempPhyTti = 0;
    /* SPR 15909 fix end */
#ifndef DL_UL_SPLIT
    ContainerNackQueueNode * nackNode_p = PNULL;
    UInt16 nackQueueForTempUECount = 0;
#endif
    UInt16 pdcchUpLinkOutputContainerTick = 0;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    UplinkContainer* ulContainerQueue_p = PNULL;     

    PdcchULNackUEInfo *nackTempUEInfo_p = PNULL;
    
    UInt16 nonAdaptiveArrayTempUEIndex = 0;
    /* ICIC changes start */
    UInt16 adaptiveArrayTempUEIndex[MAX_USER_LOCATION] = {0,0};
    /* SPR 5798 Fix Start */
    UInt8 userLocationType = CC_USER;
    /* SPR 5798 Fix End */
    /* ICIC changes end */
   
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    SInt32 availableULRBCount = availableULRBCount_g[internalCellIndex];

    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Entry",__func__);
    nackQContainerTick = MODULO_EIGHT(currentGlobalTickWithDelay - 4);
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;

#ifndef DL_UL_SPLIT
    nackQueueForTempUECount = sQueueCount(&(ulContainerQueue_p->nackQueueForTempUE));
#endif
    
    /*
       we calculate the value of pdcchUpLinkOutputContainerTick  
       All ticks are with respect to the tick when the information is to be sent to PHY interface.
       */
    pdcchUpLinkOutputContainerTick = currentGlobalTickWithDelay % MAX_PDCCH_CONTAINER;

    tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + pdcchUpLinkOutputContainerTick);

    nackTempUEInfo_p = &tempPdcchULOutput_p->pdcchULNackTempUEInfo;
    /* ICIC changes start */
    nackTempUEInfo_p->adaptiveCount[CC_USER] = 0;
    nackTempUEInfo_p->adaptiveCount[CE_USER] = 0;
    /* ICIC changes end */
    nackTempUEInfo_p->nonAdaptiveCount = 0;
   
    nonAdaptiveArrayTempUEIndex = nackTempUEInfo_p->nonAdaptiveCount;
    /* ICIC changes start */
    adaptiveArrayTempUEIndex[CC_USER] = nackTempUEInfo_p->adaptiveCount[CC_USER];
    adaptiveArrayTempUEIndex[CE_USER] = nackTempUEInfo_p->adaptiveCount[CE_USER];
    /* ICIC changes end */
 #ifndef DL_UL_SPLIT
    while(nackQueueForTempUECount--)
    {
        nackNode_p = (ContainerNackQueueNode *)popNode(&(ulContainerQueue_p->nackQueueForTempUE));
        
        /* +COVERITY 10886 */
        if (NULL == nackNode_p)
        {
            break;
        }
        /* +COVERITY 10886 */
        tempUeIndex =  nackNode_p->ueIdx;
#else
    while(1)
    {
        if(PNULL == nackNode_p)
        {
#ifdef CIRC_QUEUE_IMPL
            semWaitDefault(&waitForULNackForTempUE_g[internalCellIndex]);
            DEQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(ulContainerQueue_p->nackCircQueueForTempUE), 
            ContainerNackQueueNode, (void *)&nackNode_p);
#else
            /* +- SPR 17777 */
            recvMsgWithIccQ(ulContainerQueue_p->ulContainerNackQueueForTempUEIdDL, &nackNode_p);
#endif
            /* + Coverity_21454 */
            if(PNULL == nackNode_p)
            {
                continue;
            }
            /* - Coverity_21454 */
        }
        tempUeIndex =  nackNode_p->ueIdx;
        if( tempUeIndex == INVALID_UE_INDEX)
        {
            /* Delimeter node has been received -  Free the memory of delimeter node */
            FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p);
            break;
        }
#endif
        /* SPR 5798 Fix Start */
        /* SPR 5798 Fix End */
        tempPhyTti = nackNode_p->ttiCounter;
        if (MAC_FAILURE == handleNackNodeForTcrnti(tempUeIndex,
                    tempPhyTti, ulDelay,
                    currentGlobalTick,
                    availableCCE_p, dciCCEInfo_p,
                    dciCCEContainerInfo_p,
                    &availableULRBCount,
                    &nonAdaptiveArrayTempUEIndex,
                    adaptiveArrayTempUEIndex,
                    userLocationType,
                    /* SPR 5798 Fix Start */
                    maxUeScheduledInUplink_p,
                    /* SPR 5798 Fix End */
                    internalCellIndex
                    ))
        {
        }
        FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p);
        continue;
     }

    nackTempUEInfo_p->nonAdaptiveCount = nonAdaptiveArrayTempUEIndex;
    nackTempUEInfo_p->adaptiveCount[CC_USER] = adaptiveArrayTempUEIndex[CC_USER];
    nackTempUEInfo_p->adaptiveCount[CE_USER] = adaptiveArrayTempUEIndex[CE_USER];
    availableULRBCount_g[internalCellIndex] = availableULRBCount;

    return MAC_SUCCESS;
}
#endif

/*TTIB_Code Start*/
#ifdef FDD_CONFIG
/*********************************************************************************
 * Function Name : processTTIBundleReTxQ
 * Inputs        : ulDelay - The delay of execution legs in terms of the number
 *                           subFrames w.r.t. PHY & the current processing being
 *                           done at MAC for intended tick at PHY.
 *                 currentGlobalTick - The present Global Tick
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : maxUeScheduledInUplink_p
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:  : This function will read the TTI bundle queue and store the  
 *                 information in the PDCCH UPLINK OUTPUT container. 
 ***********************************************************************************/
MacRetType processTTIBundleReTxQ(
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 *maxUeScheduledInUplink_p,
        InternalCellIndex internalCellIndex
        )
{
    UInt16                  tempUeIndex                     = 0;
    UInt16                  pdcchUpLinkOutputContainerTick  = 0;
    UInt16                  bundleReTxContainerTick         = 0;
    SInt32                  availableULRBCount              = availableULRBCount_g[internalCellIndex]; 
    DirectIndexingUEInfo*   tempDirectIndexingUEInfo_p      = PNULL;
    PdcchULOutputInfo*      tempPdcchULOutput_p             = PNULL;
    ULHarqInfo*             tempULHarqInfo_p                = PNULL; 
    ULUEContext*            ulUEContext_p                   = PNULL;
    ULUEContextInfo*        ulUEContextInfo_p               = PNULL;
    TtiBScheduleNode*       ttiBScheduleNode_p              = PNULL;
    PdcchULNackUEInfo *     nackUEInfo_p                    = PNULL;
    /* SPR 15909 fix start */
    tickType_t              currentGlobalTickWithDelay      = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    UInt16                  reTxQueueCount                  = 0;
    UInt16                  nonAdaptiveArrayIndex           = 0;

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Entering : [%s]\n",__func__);
    bundleReTxContainerTick = MODULO_SIXTEEN(currentGlobalTickWithDelay);

    reTxQueueCount = COUNT_TTIB_SCHEDULE_Q((ttibScheduleQueue_gp[internalCellIndex] + bundleReTxContainerTick));

    pdcchUpLinkOutputContainerTick = currentGlobalTickWithDelay % MAX_PDCCH_CONTAINER;

    tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + pdcchUpLinkOutputContainerTick);
    nackUEInfo_p = &tempPdcchULOutput_p->pdcchULNackUEInfo;
    nackUEInfo_p->nonAdaptiveCount = 0;
    nonAdaptiveArrayIndex = nackUEInfo_p->nonAdaptiveCount;

    while(reTxQueueCount-- )
    {
        DEQUEUE_TTIB_SCHEDULE_Q((ttibScheduleQueue_gp[internalCellIndex] + bundleReTxContainerTick),
                TtibScheduleNode,(void **)&ttiBScheduleNode_p);
        if (PNULL == ttiBScheduleNode_p)
        {
            LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"[%s] TtibScheduleNode is NULL\n", __func__); 
            continue;
        }
        tempUeIndex =  ttiBScheduleNode_p->ueIndex;
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"[%s] Tick [%d] Dequeue Queue ttibScheduleQueue_g \
                        at Index %d UE ID %d HARQ ID %d \n", __func__,
                            getCurrentTick(),bundleReTxContainerTick,tempUeIndex,INVALID_HARQ_ID); 
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {

            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]);

            tempDirectIndexingUEInfo_p->harqIndex = ttiBScheduleNode_p->harqProcessId;
            tempDirectIndexingUEInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
            /* SPR 14877 SRS Changes */
            tempDirectIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
            /* SPR 14877 SRS Changes */
  	        /* + SPR 20701 */
            if (TRUE == isTTIInMeasGapPeriod
                    (ulUEContext_p, (currentGlobalTickWithDelay + FDD_HARQ_OFFSET)))
            {
                tempDirectIndexingUEInfo_p->flag = MEAS_GAP_RE_TX_NO_CCE_NO_GRANT;
            }
            else
            {
            tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                /* TTIBug1_Fix1 Start */
            (*maxUeScheduledInUplink_p)--;
	    /* Coverity 31810 fix Start */
	    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[ttiBScheduleNode_p->harqProcessId]);
	    /* Coverity 31810 fix End */
	    availableULRBCount -= tempULHarqInfo_p->riLength;
            }
  	        /* + SPR 20701 */
            tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
            nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] =
                tempUeIndex;
            nonAdaptiveArrayIndex++;
	}

	FREE_MEM_NODE_TTIB_SCHEDULE_Q(ttiBScheduleNode_p);
/* TTIBug1_Fix1 End */

    }

    nackUEInfo_p->nonAdaptiveCount = nonAdaptiveArrayIndex;
    

    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"[%s] Tick [%d]  \
            Non adaptive nodes count %d\n" , __func__,
            getCurrentTick(),nackUEInfo_p->nonAdaptiveCount); 

    availableULRBCount_g[internalCellIndex] = availableULRBCount;
    LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,"Exiting : [%s]\n",__func__);
    return MAC_SUCCESS;
}
/*TTIB cyclomatic complexity Fix*/
/*********************************************************************************
 * Function Name : checkResourcesAndFillNextRetxType 
 * Inputs        : ueIndex - Ue identifier
 *                 harqIndex - Harq Id
 *                 currentGlobalTick - The present Global Tick
 *                 ulDelay - The delay of execution legs in terms of the number
 *                           subFrames w.r.t. PHY & the current processing being
 *                           done at MAC for intended tick at PHY.
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None 
 * Description:  : This function will check the resoursece and fill next transmission 
 ***********************************************************************************/
void checkResourcesAndFillNextRetxType(UInt16 ueIndex,
                                  UInt8 harqIndex,
                                  /* SPR 15909 fix start */
                                  tickType_t currentGlobalTick,
                                  /* SPR 15909 fix end */
                                  UInt8 ulDelay,
                                  InternalCellIndex internalCellIndex)
{
    UInt8                           nPhichGrp                       = 0;
    UInt8                           nPhichSf                        = 0;
    ULHarqInfo*                     tempULHarqInfo_p                = PNULL; 
    ULUEContext*                    ulUEContext_p                   = PNULL;
    ULUEContextInfo*                ulUEContextInfo_p               = PNULL;

    ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];         
    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);

    nPhichGrp = tempULHarqInfo_p->nPhichGrp;
    nPhichSf  = tempULHarqInfo_p->sfUsed;

    /* If the next retransmission type is adaptive, select the current 
     * transmission type as adaptive. */
								/* SPR 10753 Changes */
    /* SPR 10753 Changes */
    /* Do non-adaptive transmission if same resources are available 
     * and there is no TPC to send and same PHICH group and sequence 
     *   are available for the next retx*/
    /* SPR 10753 Changes */
    if(ADAPTIVE_RETRANS_AFTER_HI_ACK == tempULHarqInfo_p->forceAckNextRetxType)
    {
        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
    }
    /* SPR 10753 Changes */
    else if ( MAC_SUCCESS == checkNumOfBundleResFromRBStartIndex( 
                tempULHarqInfo_p->riStart, 
                tempULHarqInfo_p->riLength,
                (currentGlobalTick + TTI_PHICH_TO_DCI_OFFSET),
                ulDelay,
                internalCellIndex
                ) 
            && (ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue == TPC_NO_CHANGE) 
            && (MAC_SUCCESS == checkHIResources(\
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask,\
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][(nPhichSf + FDD_DATA_OFFSET + 
                        FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET + TTIB_FIRST_TO_LAST_TICK_OFFSET) 
                    % MAX_PHICH_CONTAINER][nPhichGrp].bitMask)) )
    {
        tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
        /* SPR 10753 Changes */
        tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
        /* SPR 10753 Changes */
    }
    else
    {
        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
    }
    LTE_MAC_UT_LOG(LOG_INFO, "PDCCH" ,"[%s][%d] TTIB Retx is %d .\n",
            __func__,getCurrentTick(),tempULHarqInfo_p->nextReTransmissionType);

}
/*TTIB cyclomatic complexity Fix*/

/*********************************************************************************
 * Function Name : processULNackQForTTIBundledUe
 * Inputs        : ulDelay - The delay of execution legs in terms of the number
 *                           subFrames w.r.t. PHY & the current processing being
 *                           done at MAC for intended tick at PHY.
 *                 currentGlobalTick - The present Global Tick
 *                 availableCCE_p - This represent the number of available CCE
 *                 and if CCE's are allocated this would be deceremented by  one.
 *                 maxUeScheduledInUplink_p - maximum number of Ue's to be scheduled
 *                           in Uplink
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:  : This function will read the Nack queue and store the
 *                 information in the PDCCH UPLINK OUTPUT container for 
 *                 TTI bundled UE.
 ***********************************************************************************/

MacRetType processULNackQForTTIBundledUe(
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        UInt32 *maxUeScheduledInUplink_p,
        InternalCellIndex  internalCellIndex
        )
{
    UInt8                           nackQContainerTick              = 0;
    UInt8                           harqIndex                       = INVALID_HARQ_ID;     
    UInt8                           maxUlHARQTx                     = 0;
    UInt8                           containerIndex                  = 0;
    UInt8                           bundleTick                      = 0;
    UInt8                           isMeasGapPresent                = TRUE;
    UInt16                          nackQueueCount                  = 0;
    UInt16                          tempUeIndex                     = 0;
    UInt16                          nonAdaptiveArrayIndex           = 0;
    UInt16                          adaptiveArrayIndex[MAX_USER_LOCATION] = {0,0};
    /* SPR 15909 fix start */
    tickType_t                      tempPhyTti                      = 0;
    /* SPR 15909 fix end */
    UInt32                          cceAllocated                    = 0;
    UInt32                          subFrameNum                     = 0;
    /* SPR 15909 fix start */
    tickType_t                          currentGlobalTickWithDelay      = currentGlobalTick + ulDelay;
    tickType_t                          availableULRBCount              = availableULRBCount_g[internalCellIndex];
    tickType_t 			                tickWithDelayAndOffset 	    = 0;
    /* SPR 15909 fix end */
    ContainerNackQueueNode *        nackNode_p                      = PNULL;
    DirectIndexingUEInfo*           tempDirectIndexingUEInfo_p      = PNULL;
    PdcchULOutputInfo*              tempPdcchULOutput_p             = PNULL;
    UplinkContainer*                ulContainerQueue_p              = PNULL;     
    ULHarqInfo*                     tempULHarqInfo_p                = PNULL; 
    ULUEContext*                    ulUEContext_p                   = PNULL;
    ULUEContextInfo*                ulUEContextInfo_p               = PNULL;

    DciCCEContainerInfo*            dciCCEContainerInfo_p           = PNULL;
    PdcchULNackUEInfo               *nackUEInfo_p                   = PNULL;
    DciCCEInfo*                     dciCCEInfo_p                    = PNULL;

    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Entry",__func__);
    /* For allocating CCE in case of retx at n+5 */
    containerIndex = (currentGlobalTick + ulDelay + TTI_PHICH_TO_DCI_OFFSET) % MAX_CCE_CONTAINER;
    dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + containerIndex);
    dciCCEContainerInfo_p->countOfCCEAllocated = 0;
    dciCCEContainerInfo_p->countCmnChannelMsg = 0;
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0]; 

    tickWithDelayAndOffset = currentGlobalTick + ulDelay + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET;
    nackQContainerTick = MODULO_EIGHT(currentGlobalTickWithDelay - FDD_HARQ_OFFSET);
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;

    nackQueueCount = COUNT_NACK_QUEUE_FOR_TTIB_UE_Q(
                        &(ulContainerQueue_p->nackQueueForTtiBundledUe));

    /* SPR 23393 Changes End */
    subFrameNum = currentGlobalTickWithDelay % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
   
    /*
       All ticks are with respect to the tick when the information is to be sent to PHY interface.
       */

    tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + containerIndex);


    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
            "[%s] DCI container Index %d nackQueueCount = %d \n"
            ,__func__,containerIndex,nackQueueCount);

    nackUEInfo_p = &tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo;
    nackUEInfo_p->adaptiveCount[CC_USER] = 0;
    nackUEInfo_p->adaptiveCount[CE_USER] = 0;
    nackUEInfo_p->nonAdaptiveCount = 0;
    nonAdaptiveArrayIndex = nackUEInfo_p->nonAdaptiveCount;

    while(nackQueueCount--)
    {
        DEQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(&(ulContainerQueue_p->nackQueueForTtiBundledUe),
                                        ContainerNackQueueNode,(void **)&nackNode_p);
        if (PNULL == nackNode_p)
        {
            LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", "[%s] TTIB Nack Node is NULL.\n",__func__);
            continue;
        }
        tempUeIndex =  nackNode_p->ueIdx;
        if( tempUeIndex == INVALID_UE_INDEX)
        {
            /* Delimeter node has been received -  Free the memory of delimeter node */
            LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", "[%s] [%d]TTIB Deleimeter Node Recieved is NULL.\n",__func__,
                        getCurrentTick());
            FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
            break;
        }
        tempPhyTti = nackNode_p->ttiCounter;
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         

        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
        harqIndex = nackNode_p->ulHarqProcessId;

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            /* SPR 3430 */
            tempDirectIndexingUEInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
            /* SPR 14877 SRS Changes */ 
            tempDirectIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;          
            /* SPR 14877 SRS Changes */ 
            /* SPR 3430 */
            if(HARQ_PROCESS_RESET == tempULHarqInfo_p->harqStatus)
            {
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */

            updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);

                FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", "[%s] [%d]TTIB HARQ ID %d \
                        HARQ process staus is RESET.\n",__func__,
                        getCurrentTick(),harqIndex);
                LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,harqIndex, \
                        tempPhyTti,\
                        /*SPR 1768 -Start*/
                        INVALID_TX,\
                        /*SPR 1768 -End*/
                        0,\
                        0.0,0.0,__func__,"NACK_DROP_RESET");
                continue;
            }
            /* + SPS_TDD_Changes */
            if( SPS_SCHEDULING == ulUEContext_p->schType )
            {
                FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,harqIndex, \
                        tempPhyTti,\
                        /*SPR 1768 -Start*/
                        INVALID_TX,\
                        /*SPR 1768 -End*/
                        0,\
                        0.0,0.0,__func__,"NACK_DROP_RESET");
                continue;
            }
            /* - SPS_TDD_Changes */

            if(!tempULHarqInfo_p->riLength)
            {
                LTE_MAC_UT_LOG(LOG_CRITICAL,LOG_PDCCH,"\n[%s]\nNACK received for UE RNTI =%d " \
                        "Non Exixtence HARQ ID =%d \n NackQContainerTick %d\n\n",__func__, 
                        tempUeIndex, harqIndex, nackQContainerTick);


            }/*if(!tempULHarqInfo_p->riLength) */
            else 
            {
                LTE_MAC_UT_LOG(LOG_INFO,"PDCCH","[%s]\n########\n"
                        "NACK received for UE RNTI =%d " \
                        "for HARQ ID =%d currentRetransCount = %d "
                        " NackQContainerTick %d \n"
                        "currentGlobalTick = %d ulDelay = %d "
                        "tempPhyTti = %d \n########\n", __func__,
                        tempUeIndex, harqIndex, 
                        tempULHarqInfo_p->currentRetransCount,
                        nackQContainerTick,currentGlobalTick,ulDelay, tempPhyTti);            
                if (currentGlobalTickWithDelay <= (tempPhyTti + FDD_HARQ_OFFSET))
                {

                    tempDirectIndexingUEInfo_p->harqIndex = harqIndex;
                    tempDirectIndexingUEInfo_p->ttiCounter = tempPhyTti;
                    tempDirectIndexingUEInfo_p->strictRBAllocation = \
                                                                    tempULHarqInfo_p->strictRBAllocation;
                    /* + SPS_TDD_Changes */
                    if (SPS_SCHEDULING ==  nackNode_p->schUsed  )
                    {
                        maxUlHARQTx = ulUEContext_p->spsUlInfo.maxSpsUlHARQTx - 1;
                    }
                    else
                    {
                        maxUlHARQTx = ulUEContext_p->maxUlHARQTx -1;
                    }
                    /* - SPS_TDD_Changes */

                    /*If Current Retx Counter is less than MAX_Retx Counter*/
                    if ((tempULHarqInfo_p->currentRetransCount + TTI_BUNDLE_SIZE) <= maxUlHARQTx)
                    {   
                        for(bundleTick = 0 ;bundleTick < TTI_BUNDLE_SIZE ; bundleTick++)
                        {
                             /* if any tick is not in meas gap then retransmission can be scheduled in meas gap */
                            if (TRUE != isTTIInMeasGapPeriod(ulUEContext_p, (tickWithDelayAndOffset + bundleTick)))
                            {
                                isMeasGapPresent = FALSE;
                                break;
                            }
                        }                     
                        if (!isMeasGapPresent)
                        {
                            /*TTIB cyclomatic complexity Fix*/
                            checkResourcesAndFillNextRetxType(tempUeIndex,
                                                              harqIndex,
                                                              currentGlobalTick,
                                                              ulDelay,
                                                              internalCellIndex);
                            /*TTIB cyclomatic complexity Fix*/
                        } // if (isTTIInMeasGapPeriod)
                        else
                        {
                            tempDirectIndexingUEInfo_p->flag =
                                MEAS_GAP_RE_TX_NO_CCE_NO_GRANT;
                            tempDirectIndexingUEInfo_p->ulCCEIndex =
                                CCE_NOT_ALLOCATED;
                            ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE; 
                            tempPdcchULOutput_p->pdcchULNackUEInfo.
                                pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = 
                                tempUeIndex;
                            nonAdaptiveArrayIndex++;
                            FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                            continue;
                        }
                        /* MEAS_GAP_CHG */

                        if (ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType != \
                                NON_ADAPTIVE_RE_TX)
                        {
                            /* If a UL Nack is Received for the UE and 
                             * re-transmission is Adaptive then the 
                             * previous nPhichSeq th bit from the LSB of 
                             * the array phichSequenceNoPerPhichGroup_g
                             * [Subframe No][nPhichGrp] 
                             * should be made to set(PHICH is released).
                             */
                            /* CR changes start */
                            /*phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask
                              |= (ONE << nPhichSeq);
                             */
                            /* CR changes end */

                            if (MAC_SUCCESS == allocatePDCCHForULNewReTx(
                                        &ulUEContext_p->\
                                        ueSearchSpaceInfo_p[subFrameNum],
                                        &cceAllocated,
                                        availableCCE_p,
                                        dciCCEInfo_p,
                                        ulUEContext_p,
                                        PNULL, //CA_phase2_csi_code
                                        internalCellIndex))
                            {
                                tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
                                tempDirectIndexingUEInfo_p->ulCCEIndex = cceAllocated;
                                ulUEContext_p->isPdcchAllocated = UL_ADAPTIVE_CCE;
                                /*klockworks warning fix*/
                                if(dciCCEContainerInfo_p->countOfCCEAllocated < (MAX_CCE + MAX_SPS_CCE))
                                {
                                    dciCCEContainerInfo_p->arrayCCEAllocated[
                                        dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
                                }
                                dciCCEContainerInfo_p->countOfCCEAllocated++;
                                nackUEInfo_p->pdcchULNackAdaptiveUEArray[ulUEContext_p->userLocationType]
                                    [adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
                                    tempUeIndex;
                                adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
                                /* + SPR 5551 */
                                dciCCEInfo_p[cceAllocated].dciPduInfo.cqiRequest = 0;
                                /* - SPR 5551 */
								/* SPR 10753 Changes */
								tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
								/* SPR 10753 Changes */

                                LTE_MAC_UT_LOG(LOG_INFO, "PDCCH","[%s][%d] CCE Allocated index%d\n",
                                    __func__,tempDirectIndexingUEInfo_p->ulCCEIndex);

                                LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
                                        currentGlobalTick,tempUeIndex,harqIndex, \
                                        tempPhyTti,\
                                        tempULHarqInfo_p->nextReTransmissionType,\
                                        cceAllocated,\
                                        0.0,0.0,__func__,"NACK_DCI_SUCC_ULHARQ");
                            }/* If CCE allocation success*/
                            else
                            {
                                /* FORCED_ACK SPR#9374 Changes Start */
                                tempDirectIndexingUEInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
                                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
								/* SPR 10753 Changes */
								tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
								/* SPR 10753 Changes */
                                ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                                nackUEInfo_p->pdcchULNackAdaptiveUEArray
                                    [ulUEContext_p->userLocationType]
                                    [adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
                                    tempUeIndex;
                                adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
                                /* FORCED_ACK SPR#9374 Changes End */

                                LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
                                        currentGlobalTick,tempUeIndex,harqIndex, \
                                        tempPhyTti,\
                                        tempULHarqInfo_p->nextReTransmissionType,\
                                        tempULHarqInfo_p->currentRetransCount,\
                                        0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");

                                LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "[%s]The function" \
                                        " allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE"\
                                        " CCE allocation FAILs\n",__func__);            
                            }

                        }/* if(ADAPTIVE_RE_TX) */
                        else
                        {
                            tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                            tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                            ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
							/* SPR 10753 Changes */
							tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
							/* SPR 10753 Changes */
                            nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] =
                                tempUeIndex;
                            nonAdaptiveArrayIndex++;

                            LOG_MAC_MSG(MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                    getCurrentTick(),
                                    tempUeIndex,harqIndex,
                                    tempULHarqInfo_p->currentRetransCount,
                                    tempULHarqInfo_p->nextReTransmissionType,
                                    DEFAULT_INT_VALUE,
                                    0.0,0.0,FUNCTION_NAME,"");

                            LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Non_Adaptive_Re_Trans" \
                                    " of NACK Case \n",__func__);            
                        } 
                        (*maxUeScheduledInUplink_p)--;
                        availableULRBCount -= tempULHarqInfo_p->riLength;
                    }/* If retrans count < Maximum_Retrans_count*/ 
                    else
                    {
                        tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
                        tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                        LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, 
                                "[%s]RECEIVED_NACK_RE_TRANS_COMPELETE for UE Index = %d and Harq ID = %d",
                                __func__,tempUeIndex,harqIndex); 
                        ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                        nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = tempUeIndex;
                        nonAdaptiveArrayIndex++;
                    }
                }/*if (currentGlobalTickWithDelay <= (tempPhyTti + FDD_HARQ_OFFSET))*/
                else
                {
                    ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                    tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                    freeULHarqProcess(tempULHarqInfo_p);
#endif
                    /* SPR 16916 FIX END */
                    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
                            "[%s] [%d] No CCE allocation No CAK NAck shoulbd be scheduled\n"
                            ,__func__,getCurrentTick());
                   /* 
                    LOG_MAC_MSG(MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,harqIndex, \
                            tempPhyTti,\
                            tempULHarqInfo_p->nextReTransmissionType,\
                            scheduledTTI,\
                            0.0,0.0,__func__,"DELAY_NACK_FREE_ULHARQ");*/
                    updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                }
            }
        }/*if (!ulUEContextInfo_p->pendingDeleteFlag)*/
        else
        { 
            LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] UL NACK of UE ID:%d" \
                    "Having PHY TTI %d,ULSch TTI %d Pending to delete is set",
                    __func__, tempUeIndex,tempPhyTti,currentGlobalTick);

            LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
                    currentGlobalTick,tempUeIndex,harqIndex, \
                    tempPhyTti,\
                    /*SPR 1768 -Start*/
                    INVALID_TX,\
                    /*SPR 1768 -End*/
                    0,\
                    0.0,0.0,__func__,"NACK_DROP_UEDELETE");

        }
        FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
    } /*while()*/
    nackUEInfo_p->nonAdaptiveCount = nonAdaptiveArrayIndex;
    nackUEInfo_p->adaptiveCount[CC_USER] = adaptiveArrayIndex[CC_USER];
    nackUEInfo_p->adaptiveCount[CE_USER] = adaptiveArrayIndex[CE_USER];
    availableULRBCount_g[internalCellIndex] = availableULRBCount;
    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
            "[%s] \n\tnonAdaptiveCount = %d,"
            " adaptiveCount[CC_USER] = %d"
            " adaptiveCount[CE_USER] = %d\n"
            ,__func__,nackUEInfo_p->nonAdaptiveCount,
            nackUEInfo_p->adaptiveCount[CC_USER],nackUEInfo_p->adaptiveCount[CE_USER]);
    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Exit",__func__);
    return MAC_SUCCESS;
}
/*TTIB_Code End*/
#endif

/*********************************************************************************
 * Function Name : processULNackQ
 * Inputs        : ulDelay - The delay of execution legs in terms of the number
 *                           subFrames w.r.t. PHY & the current processing being
 *                           done at MAC for intended tick at PHY.
 *                 currentGlobalTick - The present Global Tick
 *                 availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by  one.
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE
 *                    Information is there and once cce is allocated ,updated
 *                    information is written on the reserved CCE.
 *                 dciCCEContainerInfo_p -
 *                 maxUeScheduledInUplink_p - maximum number of Ue's to be scheduled
 *                    in Uplink
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:  : This function will read the Nack queue and store the
 *                 information in the PDCCH UPLINK OUTPUT container.
 ***********************************************************************************/
#ifdef FDD_CONFIG
#ifndef DL_UL_SPLIT
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQ(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* SPR 5798 Fix Start */
        UInt32 *maxUeScheduledInUplink_p,
        /* SPR 5798 Fix End */
        InternalCellIndex  internalCellIndex)
{
#else
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQ(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        ContainerNackQueueNode * nackNode_p,
        /* SPR 5798 Fix Start */
        UInt32 *maxUeScheduledInUplink_p,
        /* SPR 5798 Fix End */
        InternalCellIndex  internalCellIndex)
{
#endif
		UInt16 nackQueueCount = 0;
		UInt16 tempUeIndex=0;
		UInt8 harqIndex = INVALID_HARQ_ID;     
		/* SPR 15909 fix start */
		tickType_t tempPhyTti = 0;
		/* SPR 15909 fix end */
#ifndef DL_UL_SPLIT
		ContainerNackQueueNode * nackNode_p = PNULL;
		/* +GCC 4.7 */
		UplinkContainer* ulContainerQueue_p = PNULL;     
		UInt8  nackQContainerTick =0;
		/* -GCC 4.7 */
#endif
		UInt16 pdcchUpLinkOutputContainerTick = 0;
		DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
		PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
		ULHarqInfo*   tempULHarqInfo_p = PNULL; 
		ULUEContext* ulUEContext_p = PNULL;
		ULUEContextInfo* ulUEContextInfo_p = PNULL;

		PdcchULNackUEInfo *nackUEInfo_p = PNULL;
		UInt16 nonAdaptiveArrayIndex = 0;
		/* ICIC changes start */
		UInt16 adaptiveArrayIndex[MAX_USER_LOCATION] = {0,0};
		/* ICIC changes end */
		UInt8 rbMapNum = MODULO_SIXTEEN((currentGlobalTick + ulDelay + FDD_HARQ_OFFSET));

		UInt8 nPhichGrp = 0;
		UInt8 nPhichSf = 0;
		UInt32 cceAllocated = 0;
		UInt32 subFrameNum = 0;
		/* SPR 18052 Start */
		/* UL_MU_MIMO_CHG_START */
		/* SPR 10753 Changes Start */
		UInt8 localReTxRBMap[INVALID_RI_START + 1] = {0};
		/* SPR 10753 Changes End */
		/* UL_MU_MIMO_CHG_END */
		/* SPR 18052 End */
		UInt8  maxUlHARQTx = 0;
		/*QOS_KLOCKWORK_FIX*/
#ifdef LOG_PRINT_ENABLED    
		/* SPR 15909 fix start */
		tickType_t scheduledTTI = 0;
		/* SPR 15909 fix end */
#endif
		/*QOS_KLOCKWORK_FIX*/
		/* SPR 15909 fix start */
		tickType_t currentGlobalTickWithDelay = currentGlobalTick + ulDelay;
		/* SPR 15909 fix end */
		SInt32 availableULRBCount = availableULRBCount_g[internalCellIndex];
#ifdef LOG_PRINT_ENABLED
		UInt32 nackRecvdCount=0x0;
#endif     

		/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
		/* SPR 15909 fix start */
		tickType_t localTick = 0;
		/* SPR 15909 fix end */
#endif
		/*HD FDD Changes End*/
		/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
		UInt8 numRbsAllocatedThRAT1 = 0;
		MacRetType retValue = MAC_FAILURE;
#endif    
		/* SPR 21958 PUSCH RAT1 Support End */
		LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Entry",__func__);
#ifndef DL_UL_SPLIT
		nackQContainerTick = MODULO_EIGHT(currentGlobalTickWithDelay - 4);
		ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;

		nackQueueCount = sQueueCount(&(ulContainerQueue_p->nackQueue));
#else
		nackQueueCount = sQueueCount(ulStrategyReTxQueue_gp[internalCellIndex]);
#endif

    /* SPR 23393 Changes End */
		subFrameNum = currentGlobalTickWithDelay % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;

		/*
		   we calculate the value of pdcchUpLinkOutputContainerTick  
		   All ticks are with respect to the tick when the information is to be sent to PHY interface.
		   */
		pdcchUpLinkOutputContainerTick = currentGlobalTickWithDelay % MAX_PDCCH_CONTAINER;

		tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + pdcchUpLinkOutputContainerTick);
#ifndef DL_UL_SPLIT
#ifdef LOG_PRINT_ENABLED
		nackRecvdCount=nackQueueCount;
#endif  
		LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
						"[%s]      nackQueueCount = %d pdcchUpLinkOutputContainerTick = %d\n"
						,__func__,nackQueueCount,pdcchUpLinkOutputContainerTick);
#endif  

		nackUEInfo_p = &tempPdcchULOutput_p->pdcchULNackUEInfo;
		/* ICIC changes start */
		nackUEInfo_p->adaptiveCount[CC_USER] = 0;
		nackUEInfo_p->adaptiveCount[CE_USER] = 0;
		/* ICIC changes end */

		/* For TTIB_Code */
		nackUEInfo_p->nonAdaptiveCount = 0;
		nonAdaptiveArrayIndex = nackUEInfo_p->nonAdaptiveCount;

		while(nackQueueCount--)
		{
#ifndef DL_UL_SPLIT
				nackNode_p = (ContainerNackQueueNode *)popNode(&(ulContainerQueue_p->nackQueue));
#else 
				nackNode_p = (ContainerNackQueueNode *)popNode(ulStrategyReTxQueue_gp[internalCellIndex]);
#endif
				/* +COVERITY 10885 */
				if (NULL == nackNode_p)
				{
						continue;
				}
				/* -COVERITY 10885 */
				tempUeIndex =  nackNode_p->ueIdx;
				if( tempUeIndex == INVALID_UE_INDEX)
				{
						/* Delimeter node has been received -  Free the memory of delimeter node */
						freeMemPool((void *)nackNode_p);
						break;
				}
				tempPhyTti = nackNode_p->ttiCounter;
				harqIndex = MODULO_EIGHT(tempPhyTti);
				ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         

				if (!ulUEContextInfo_p->pendingDeleteFlag)
				{
						ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
						tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
						tempDirectIndexingUEInfo_p = \
													 &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
						/* SPR 3430 */
						tempDirectIndexingUEInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
						/* SPR 14877 SRS Changes */ 
						tempDirectIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
						/* SPR 14877 SRS Changes */ 
						/* SPR 3430 */
						if(HARQ_PROCESS_RESET == tempULHarqInfo_p->harqStatus)
						{
								/* SPR 19288 change start */
								/* SPR 19288 change end */
								/* SPR 16916 FIX START */
#ifdef KPI_STATS
								freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
								freeULHarqProcess(tempULHarqInfo_p);
#endif
								/* SPR 16916 FIX END */

								updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
								/* TTIB_Code End */

								freeMemPool((void *)nackNode_p);    
								nackNode_p = PNULL;
								LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
												currentGlobalTick,tempUeIndex,harqIndex, \
												tempPhyTti,\
												/*SPR 1768 -Start*/
												INVALID_TX,\
												/*SPR 1768 -End*/
												0,\
												0.0,0.0,__func__,"NACK_DROP_RESET");
								continue;
						}
						/* + SPS_TDD_Changes */
						if (SPS_SCHEDULING == ulUEContext_p->schType) 
						{
								freeMemPool((void *)nackNode_p);    
								nackNode_p = PNULL;
								continue;
						}
						/* - SPS_TDD_Changes */
						/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
						localTick = getCurrentTick() + PHY_DL_DELAY;
						/* For Half Duplex UE, if CRC was received as NACK, then check if the UE can be marked
						 * with UL checks for retransmission (UL data in N+4th TTI + PHICH in N+8th TTI) 
						 * If no resources found, let UE retransmit the data and it may be possible that
						 * no HARQ could be sent on PHICH because the corresponding subframes are not possible
						 * as DL. PHICH will be sent even if UE does not listen */
						if(TRUE == ulUEContext_p->isHDFddFlag) 
						{
								if(CAN_SCHEDULE_UL_FOR_HD_UE(localTick, ulUEContext_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))
								{
										/* Mark the corresponding TTIs reserved for UL and DL respectively */
										SCHEDULE_UL_FOR_HD_UE(localTick, ulUEContext_p->ueIndex,
														hdUeContextInfo_g.hdfddUeSchedMap,internalCellIndex);
										tempULHarqInfo_p->hdFddUlNackHandlingFlag = FALSE;
								}
								else
								{
										/*SPR 5424 fix start*/
										/* Mark the the corresponding TTIs reserved for UL and DL respectively*/
										SCHEDULE_UL_NACK_FOR_HD_UE(localTick, ulUEContext_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap);
										/*SPR 5424 fix end*/
										/* Set the hdFddUlNackHandlingFlag to TRUE to indicate that in 
										 * next Tx the UL packet should be discarded.  */
										tempULHarqInfo_p->hdFddUlNackHandlingFlag = TRUE;
								}    
						}
#endif
						/*HD FDD Changes End*/
						nPhichGrp = tempULHarqInfo_p->nPhichGrp;
						nPhichSf  = tempULHarqInfo_p->sfUsed;
						/* SPR 21958 PUSCH RAT1 Support Start */
						if(TRUE==tempULHarqInfo_p->RAT1Allocation)
						{
								numRbsAllocatedThRAT1 = 
										((tempULHarqInfo_p->endRbgIndexforSet1 - tempULHarqInfo_p->startRbgIndexforSet1)+
										 (tempULHarqInfo_p->endRbgIndexforSet2 - tempULHarqInfo_p->startRbgIndexforSet2))*
										rbgSize_g[internalCellIndex];
						}
						/* SPR 21958 PUSCH RAT1 Support End */ 
						if(!tempULHarqInfo_p->riLength)
						{
								LTE_MAC_UT_LOG(LOG_CRITICAL,LOG_PDCCH,"\n[%s]\nNACK received for UE RNTI =%d " \
												"Non Exixtence HARQ ID =%d \n currentGlobalTick %d\n\n",__func__, 
												tempUeIndex, harqIndex, currentGlobalTick);

								LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
												currentGlobalTick,tempUeIndex,harqIndex, \
												tempULHarqInfo_p->nextReTransmissionType,\
												tempULHarqInfo_p->currentRetransCount,\
												nackRecvdCount,\
												0.0,0.0,__func__,"NACK_ZERO_BAND");

						}
						else 
						{
								LTE_MAC_UT_LOG(LOG_INFO,"PDCCH","[%s]\n########\n"
												"NACK received for UE RNTI =%d " \
												"for HARQ ID =%d currentRetransCount = %d "
												" NackQContainerTick %d \n"
												"currentGlobalTick = %d ulDelay = %d "
												"tempPhyTti = %d \n########\n", __func__,
												tempUeIndex, harqIndex, 
												tempULHarqInfo_p->currentRetransCount,
												nackQContainerTick,currentGlobalTick,ulDelay, tempPhyTti);            
								if (currentGlobalTickWithDelay <= (tempPhyTti + FDD_HARQ_OFFSET))
								{

										tempDirectIndexingUEInfo_p->harqIndex = harqIndex;
										tempDirectIndexingUEInfo_p->ttiCounter = tempPhyTti;
										tempDirectIndexingUEInfo_p->strictRBAllocation = \
																						 tempULHarqInfo_p->strictRBAllocation;
										/* +- SPR 17669*/
										/* UL_MU_MIMO_CHG_START */
#ifdef SSI_DEBUG_STATS
										if( UL_MU_MIMO_ENABLED == tempULHarqInfo_p->ulMuMimoStatus )
										{
												/* CA Stats Changes Start */
												LTE_MAC_UPDATE_UL_MIMO_UE_RETX(internalCellIndex);
												/* CA Stats Changes End */
												tempULHarqInfo_p->ulMuMimoStatus = UL_MU_MIMO_DISABLED;
										}
#endif
										tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
										/* UL_MU_MIMO_CHG_END */

										/* SPS CHANGES */
										/* + SPS_TDD_Changes */
										maxUlHARQTx = ulUEContext_p->maxUlHARQTx -1;
										/* - SPS_TDD_Changes */
										/* 5798 Fix Start */
										if( !(*maxUeScheduledInUplink_p) )
										{
												tempDirectIndexingUEInfo_p->flag = MAX_UE_SCHEDULED_NO_RETX_NO_GRANT;
												tempDirectIndexingUEInfo_p->ulCCEIndex =
														CCE_NOT_ALLOCATED;
												ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE; 
												tempPdcchULOutput_p->pdcchULNackUEInfo.
														pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = 
														tempUeIndex;
												nonAdaptiveArrayIndex++;
												/* SPR 2379 Changes Start */ 
												freeMemPool((void *)nackNode_p);
												/* SPR 2379 Changes End */ 
												nackNode_p = PNULL;
												continue;

										}
										/* 5798 Fix End */

										/*If Current Retx Counter is less than MAX_Retx Counter*/
										if (tempULHarqInfo_p->currentRetransCount < maxUlHARQTx)
										{                            
												/* SPS CHANGES */
												/* MEAS_GAP_CHG */
												if (FALSE == isTTIInMeasGapPeriod
																/* Review comment fix start GG11*/
																(ulUEContext_p, (tempPhyTti + FDD_NEXT_DCI_OFFSET)))
														/* Review comment fix end GG11*/
												{
														if (FALSE == isTTIInMeasGapPeriod
																		/* Review comment fix start GG11*/
																		(ulUEContext_p, (tempPhyTti + FDD_HARQ_OFFSET)))
																/* Review comment fix end GG11*/
														{
																/* SPR 5467 changes start*/
																/* If the next retransmission type is
																 * adaptive, select the current 
																 * transmission type as adaptive. */
																if (/*UL_MU_MIMO_CHG Start*/
																				localReTxRBMap[tempULHarqInfo_p->riStart] ||
																				/*UL_MU_MIMO_CHG End*/

																				(!tempULHarqInfo_p->newDataIndicator && 
																				 ADAPTIVE_RE_TX == tempULHarqInfo_p->
																				 nextReTransmissionType ) )
																{
																		tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
																}
																/* Do non-adaptive transmission 
																 * if same resources are available 
																 * and there is no TPC to send
																 * and same PHICH group and sequence 
																 *   are available for the next retx*/
																/* SPR 21958 PUSCH RAT1 Support Start */
																else
																{
																		if (TRUE==tempULHarqInfo_p->RAT1Allocation)
																		{
																				retValue = checkNumRBGFromRBGSet1StartIndex(
																								tempULHarqInfo_p->startRbgIndexforSet1,
																								tempULHarqInfo_p->endRbgIndexforSet1,
																								tempULHarqInfo_p->startRbgIndexforSet2,
																								tempULHarqInfo_p->endRbgIndexforSet2,
																								rbMapNum,
																								internalCellIndex);

																		}
																		else
																		{

																				retValue = checkNumResourcesFromRBStartIndex(
																								/* SPR 5467 changes end*/
																								tempULHarqInfo_p->riStart, 
																								tempULHarqInfo_p->riLength, 
																								rbMapNum, internalCellIndex); 
																		}
																		if ((MAC_SUCCESS == retValue)
																						/* SPR 21958 PUSCH RAT1 Support End */
																						&& (ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue == TPC_NO_CHANGE) 
																						/* CR changes start */
																						&& (MAC_SUCCESS == checkHIResources(\
																										phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask,\
																										phichSequenceNoPerPhichGroup_g[internalCellIndex][(nPhichSf + FDD_DATA_OFFSET + FDD_HARQ_OFFSET) % MAX_PHICH_CONTAINER][nPhichGrp].bitMask)) )
																				/* CR changes end */
																		{
																				tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
																				localReTxRBMap[tempULHarqInfo_p->riStart] = tempULHarqInfo_p->riLength;
																				/* UL_MU_MIMO_CHG_END */
																		}
																		else
																		{
																				tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
																		}
																		/* SPR 21958 PUSCH RAT1 Support Start */
																}
																/* SPR 21958 PUSCH RAT1 Support End */
																/* UL_MU_MIMO_CHG_START */
																LOG_MAC_MSG (MAC_UL_MIMO_RE_TX_INFO_ID,LOGDEBUG,MAC_PDCCH,\
																				currentGlobalTick,tempUeIndex,tempULHarqInfo_p->riStart,\
																				tempULHarqInfo_p->riLength,\
																				tempULHarqInfo_p->currentRetransCount,tempULHarqInfo_p->nextReTransmissionType,\
																				0.0,0.0,__func__,"UE_RETX_INFO");
																/* UL_MU_MIMO_CHG_END */
														}
														else
														{
																/* + SPR 11099 */
																/* FORCED_ACK SPR#9374 Changes Start */ 
																tempDirectIndexingUEInfo_p->flag = 
																		ADAPTIVE_RETRANS_NO_CCE;
																tempDirectIndexingUEInfo_p->ulCCEIndex = 
																		INVALID_CCE_INDEX;
																tempULHarqInfo_p->nextReTransmissionType = 
																		ADAPTIVE_RE_TX;
																ulUEContext_p->isPdcchAllocated = 
																		NO_CCE_ALLOCATED;
																nackUEInfo_p->pdcchULNackAdaptiveUEArray
																		[ulUEContext_p->userLocationType]
																		[adaptiveArrayIndex[ulUEContext_p->
																				userLocationType]] = tempUeIndex;
																		adaptiveArrayIndex[ulUEContext_p->
																				userLocationType]++;
																		/* FORCED_ACK SPR#9374 Changes End */
																		/* - SPR 11099 */
																		/* SPR 2379 Changes Start */ 
																		freeMemPool((void *)nackNode_p);
																		/* SPR 2379 Changes End */ 
																		nackNode_p = PNULL;
																		continue;
														}
												}
												else
												{
														tempDirectIndexingUEInfo_p->flag =
																MEAS_GAP_RE_TX_NO_CCE_NO_GRANT;
														tempDirectIndexingUEInfo_p->ulCCEIndex =
																CCE_NOT_ALLOCATED;
														ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE; 
														tempPdcchULOutput_p->pdcchULNackUEInfo.
																pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = 
																tempUeIndex;
														nonAdaptiveArrayIndex++;
														/* SPR 2379 Changes Start */ 
														freeMemPool((void *)nackNode_p);
														/* SPR 2379 Changes End */ 
														nackNode_p = PNULL;
														continue;
												}
												/* MEAS_GAP_CHG */

												if (ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType != \
																NON_ADAPTIVE_RE_TX)
												{
														/* If a UL Nack is Received for the UE and 
														 * re-transmission is Adaptive then the 
														 * previous nPhichSeq th bit from the LSB of 
														 * the array phichSequenceNoPerPhichGroup_g
														 * [Subframe No][nPhichGrp] 
														 * should be made to set(PHICH is released).
														 */
														/* CR changes start */
														/*phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask
														  |= (ONE << nPhichSeq);
														  */
														/* CR changes end */

														/* _empty_ul_sf changes start*/
														if ((LTE_TRUE == isUeTypeInvalid_g[internalCellIndex]))
														{
																/* _empty_ul_sf changes start*/
																/* FORCED_ACK SPR#9374 Changes Start */ 
																tempDirectIndexingUEInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
																tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
																tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
																ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
																nackUEInfo_p->pdcchULNackAdaptiveUEArray
																		[ulUEContext_p->userLocationType]
																		[adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
																				tempUeIndex;
																		adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
																		/* FORCED_ACK SPR#9374 Changes End */

																		LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
																						currentGlobalTick,tempUeIndex,harqIndex, \
																						tempPhyTti,\
																						tempULHarqInfo_p->nextReTransmissionType,\
																						tempULHarqInfo_p->currentRetransCount,\
																						0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");

																		LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
																						" allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");

														}
														else
														{
																if (MAC_SUCCESS == allocatePDCCHForULNewReTx(
																						&ulUEContext_p->\
																						ueSearchSpaceInfo_p[subFrameNum],
																						&cceAllocated,
																						availableCCE_p,
																						dciCCEInfo_p,
																						ulUEContext_p,
																						PNULL, //CA_phase2_csi_code
																						internalCellIndex))
																{
																		tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
																		tempDirectIndexingUEInfo_p->ulCCEIndex = cceAllocated;
																		ulUEContext_p->isPdcchAllocated = UL_ADAPTIVE_CCE;
																		dciCCEContainerInfo_p->arrayCCEAllocated[
																				dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
																		dciCCEContainerInfo_p->countOfCCEAllocated++;
																		nackUEInfo_p->pdcchULNackAdaptiveUEArray[ulUEContext_p->userLocationType]
																		[adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
																		tempUeIndex;
																		adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
																		/* + SPR 5551 */
																		dciCCEInfo_p[cceAllocated].dciPduInfo.cqiRequest = 0;
																		/* - SPR 5551 */

																		/* SPR 10753 Changes Start */
																		tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
																		/* SPR 10753 Changes End */

																		LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
																						currentGlobalTick,tempUeIndex,harqIndex, \
																						tempPhyTti,\
																						tempULHarqInfo_p->nextReTransmissionType,\
																						cceAllocated,\
																						0.0,0.0,__func__,"NACK_DCI_SUCC_ULHARQ");
																}
																else
																{
																		/* FORCED_ACK SPR#9374 Changes Start */ 
																		tempDirectIndexingUEInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
																		tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
																		tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
																		/* SPR 10753 Changes Start */
																		tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
																		/* SPR 10753 Changes End */
																		ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
																		nackUEInfo_p->pdcchULNackAdaptiveUEArray
																				[ulUEContext_p->userLocationType]
																				[adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
																						tempUeIndex;
																				adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
																				/* FORCED_ACK SPR#9374 Changes End */

																				LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
																								currentGlobalTick,tempUeIndex,harqIndex, \
																								tempPhyTti,\
																								tempULHarqInfo_p->nextReTransmissionType,\
																								tempULHarqInfo_p->currentRetransCount,\
																								0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");

																				LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
																								" allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");            
																}
														}
												}
												else
												{
														tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
														tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
														ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
														nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] =
																tempUeIndex;
														nonAdaptiveArrayIndex++;

														/* SPR 10753 Changes Start */
														tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
														/* SPR 10753 Changes End */

                                                        LOG_MAC_MSG(MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                                                getCurrentTick(),
                                                                tempUeIndex,harqIndex,
                                                                tempULHarqInfo_p->currentRetransCount,
                                                                tempULHarqInfo_p->nextReTransmissionType,
                                                                DEFAULT_INT_VALUE,
                                                                0.0,0.0,FUNCTION_NAME,"");

														LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, " Non_Adaptive_Re_Trans" \
																		" of NACK Case \n");            
												} 
												/* 5798 Fix Start */
												(*maxUeScheduledInUplink_p)--;
												/* 5798 Fix End */
												/* SPR 21958 PUSCH RAT1 Support Start */
												if(TRUE==tempULHarqInfo_p->RAT1Allocation)
												{
														availableULRBCount -= numRbsAllocatedThRAT1;
												}
												else
												{
														availableULRBCount -= tempULHarqInfo_p->riLength;
												} 
										} 
										/* SPR 21958 PUSCH RAT1 Support End */
										else
										{
												tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
												tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
												LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, 
																"RECEIVED_NACK_RE_TRANS_COMPELETE for UE Index = %d and Harq ID = %d",
																tempUeIndex,harqIndex); 
												ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
												nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = tempUeIndex;
												nonAdaptiveArrayIndex++;
										}
								}
								else
								{
										ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
										tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
										tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

										/* SPR 1996 Start*/
										LOG_MAC_MSG(MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
														currentGlobalTick,tempUeIndex,harqIndex, \
														tempPhyTti,\
														tempULHarqInfo_p->nextReTransmissionType,\
														scheduledTTI,\
														0.0,0.0,__func__,"DELAY_NACK_FREE_ULHARQ");
										/* SPR 16916 FIX START */
#ifdef KPI_STATS
										freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
										freeULHarqProcess(tempULHarqInfo_p);
#endif
										/* SPR 16916 FIX END */
										/* TTIB_Code Start */
										updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
										/* TTIB_Code End */
								}
						}
				}
				else
				{ 
						LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "UL NACK of UE ID:%d" \
										"Having PHY TTI %d,ULSch TTI %d Pending to delete is set",
										tempUeIndex,tempPhyTti,currentGlobalTick);

						LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
										currentGlobalTick,tempUeIndex,harqIndex, \
										tempPhyTti,\
										/*SPR 1768 -Start*/
										INVALID_TX,\
										/*SPR 1768 -End*/
										0,\
										0.0,0.0,__func__,"NACK_DROP_UEDELETE");

				}
				freeMemPool((void *)nackNode_p);
				nackNode_p = PNULL;
		}
		nackUEInfo_p->nonAdaptiveCount = nonAdaptiveArrayIndex;
		nackUEInfo_p->adaptiveCount[CC_USER] = adaptiveArrayIndex[CC_USER];
		nackUEInfo_p->adaptiveCount[CE_USER] = adaptiveArrayIndex[CE_USER];
		availableULRBCount_g[internalCellIndex] = availableULRBCount;
		LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
						"[%s] \n\ttempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount = %d,"
						" ttempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount = %d"
						,__func__,tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount,
						tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount);
		LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Exit",__func__);
		return MAC_SUCCESS;
}
#elif TDD_CONFIG
/*********************************************************************************
 * Function Name : processULNackQinTDD 
 * Inputs        : localPdcchInfoMap: This is array of pointers that contains
 *                 pointers of PdcchInfo structure for ulsubframe's.
 *                 currentGlobalTickWithDelay - The present Global Tick with Delay
 *                 availableCCE_p : This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented 
 *                    by one.
 *                 dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 *                 dciCCEContainerInfo_p: This is a pointer to 
 *                    dciCCEContainerInfo_g.
 *                 ulSubFrame - This holds the UL subframe number in TDD
 *                 maxUeScheduledInUplink_p - maximum number of Ue's to be scheduled
 *                    in Uplink
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:    This function will read the Nack queue and store the  
 *                 information in the PDCCH UPLINK OUTPUT container. 
 ***********************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processULNackQinTDD(
/* SPR 5599 changes end (ZIP ID 129195) */            
        /*TDD Config 0 changes Start*/
        PdcchInfo* localPdcchInfoMap[],
        /*TDD Config 0 changes End*/
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p, 
        /* SPR 5798 Fix Start */
        UInt8 *maxUeScheduledInUplink_p
        /* SPR 5798 Fix End */
        ,InternalCellIndex internalCellIndex 
)
{
    UInt8  nackQContainerTick = 0;
    UInt16 nackQueueCount     = 0;
    UInt16 nackQueueCountForTempUE         = 0;
    UInt16 tempUeIndex        = 0;
    UInt8  harqIndex          = INVALID_HARQ_ID;
    /* SPR 15909 fix start */
    tickType_t tempPhyTti         = 0;
    /* SPR 15909 fix end */
    ContainerNackQueueNode * nackNode_p    = PNULL;
    UplinkContainer* ulContainerQueue_p    = PNULL;
    ULHarqInfo*   tempULHarqInfo_p         = PNULL;
    ULUEContext* ulUEContext_p             = PNULL;
    ULUEContextInfo* ulUEContextInfo_p     = PNULL;
    UeScheduledInfo *ueScheduledInfo_p     = PNULL;
    UInt32 cceAllocated                    = 0;
    UInt32 subFrameNum                     = 0;
    UInt32 sysFrameNum                     = 0;
    UInt8  packetSubFrameNum                  = 0;
    /* TDD Config 6 Changes Start */
    UInt16 packetSysFrameNum               = 0;
    /* TDD Config 6 Changes End */
    UInt8  hiPduSubFrameNum                = 0;
    /* UL_MU_MIMO_CHG_START */
    /*TDD Config 0 changes Start*/
    UInt8 phichSubFrameNum                 = 0;
    UInt8 iPhich                           = 0;
    UInt8 ulSfFromNackNode                 = 0;
    PdcchInfo *pdcchInfo_p                 = PNULL;
    UInt8  measGapFlag = FALSE;
    UInt8  ulschSubFrameNum = 0;

    /* SPR 23393 Changes Start */
    GET_SFN_SF_FROM_TTI(currentGlobalTickWithDelay,sysFrameNum,subFrameNum);
    /* SPR 23393 Changes End */

    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Entry",__func__);
    /* Getting Container Tick*/
    nackQContainerTick = (currentGlobalTickWithDelay)% getMaxULCrcContainer(internalCellIndex);
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;

    /*************************** msg3_retx_tdd_start *****************************/
    /* msg3_retx_tdd_start */
    nackQueueCountForTempUE = sQueueCount(&(ulContainerQueue_p->nackQueueForTempUE));
    nackQueueCount = sQueueCount(&(ulContainerQueue_p->nackQueue));
    /* msg3_retx_tdd_end */
    while(nackQueueCountForTempUE--)
    {
        nackNode_p = (ContainerNackQueueNode *)popNode(&(ulContainerQueue_p->nackQueueForTempUE));

        if(nackNode_p != PNULL)
        {
            tempUeIndex =  nackNode_p->ueIdx;
            tempPhyTti = nackNode_p->ttiCounter;
            harqIndex = nackNode_p->ulHarqProcessId;
            /*TDD Config 0 Change Start*/
            ulSfFromNackNode = nackNode_p->ulSubFrame;
            pdcchInfo_p      = localPdcchInfoMap[ulSfFromNackNode];
            /*TDD Config 0 Change End*/

            if(MAC_FAILURE == handleNackNodeForTcrntiTdd(
                        harqIndex,
                        tempUeIndex,
                        tempPhyTti,
                        currentGlobalTickWithDelay,
                        availableCCE_p,
                        dciCCEInfo_p,
                        dciCCEContainerInfo_p,
                        CC_USER,
                        maxUeScheduledInUplink_p,
                        ulSfFromNackNode,
                        pdcchInfo_p,
                        internalCellIndex))
            {
                //warning message
            }
            freeMemPool((void *)nackNode_p);
        }
        else
        {
            //warning message;
        }
    }
    /*************************** msg3_retx_tdd_end *****************************/

    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "nackQContainerTick [%d] nackQueueCount[%d]",
            nackQContainerTick,nackQueueCount);
    /* TDD Adaptive Trasmission Prach Allocation changes */

    while(nackQueueCount--)
    {
        nackNode_p = (ContainerNackQueueNode *)popNode(&(ulContainerQueue_p->nackQueue));
        if(PNULL != nackNode_p)
        {
            tempUeIndex =  nackNode_p->ueIdx;
            tempPhyTti = nackNode_p->ttiCounter;

            ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];

            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                /*TDD Config 0 Change Start*/
                ulSfFromNackNode = nackNode_p->ulSubFrame;
                pdcchInfo_p = localPdcchInfoMap[ulSfFromNackNode];
                /*TDD Config 0 Change End*/

                /*getting harq id from subframe map */ 
                /* TDD Config 6 Changes Start */
                GET_SFN_SF_FROM_TTI(tempPhyTti, packetSysFrameNum, packetSubFrameNum);
                harqIndex      = getULHarqIDFromSubFrame (packetSubFrameNum, ulUEContext_p, internalCellIndex, packetSysFrameNum);
                /* TDD Config 6 Changes End */

                if ( INVALID_HARQ_ID == harqIndex )
                {
                    LOG_MAC_MSG(MAC_INVALID_HID_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                            currentGlobalTickWithDelay,\
                            tempUeIndex,packetSubFrameNum,0,0,0,\
                            0.0,0.0,__func__,\
                            "Harq Id is already free for this sub frame");
                    freeMemPool((void *)nackNode_p);
                    nackNode_p = PNULL;
                    /* + COVERITY 5.0 */
                    continue;
                    /* - COVERITY 5.0 */
                }
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
                /*TDD Config 0 Changes Start*/
                phichSubFrameNum = nackNode_p->ulSubFrame + getHiPduSubframeNum(nackNode_p->ulSubFrame,internalCellIndex);
                phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
                iPhich = calculateIPhichValue(nackNode_p->ulSubFrame,internalCellIndex);
                /*TDD Config 0 Changes End*/

                if(!tempULHarqInfo_p->riLength)
                {
                    LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                            currentGlobalTickWithDelay,tempUeIndex,harqIndex, \
                            tempULHarqInfo_p->nextReTransmissionType,\
                            tempULHarqInfo_p->currentRetransCount,\
                            0,\
                            0.0,0.0,__func__,"NACK_ZERO_BAND");
                    //return MAC_FAILURE;                            
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,"PDCCH","[%s]\n########\n"
                            "NACK received for UE RNTI =%d " \
                            "for HARQ ID =%d currentRetransCount = %d "
                            " NackQContainerTick %d \n"
                            "currentGlobalTickWithDelay = %d "
                            "tempPhyTti = %d \n########\n", __func__,
                            tempUeIndex, harqIndex,
                            ulUEContext_p->ulresInfo[harqIndex].currentRetransCount,
                            nackQContainerTick,currentGlobalTickWithDelay, tempPhyTti);

                    if (! isValidULSubFrameForHIPdu ( packetSubFrameNum, &hiPduSubFrameNum, internalCellIndex ))
                    {
                        ltePanic("Invalid SubFrame[%d] for UL HI Pdu\n", packetSubFrameNum);
                    }

                    if (!isValidSubframeNumForDCI_0 (subFrameNum, internalCellIndex))
                    {
                        ltePanic("Invalid SubFrame[%d] for DCI -0\n", subFrameNum);

                    }
                    /** SPR 14204 Fix : Start **/
                    ueScheduledInfo_p = ulUEContext_p->
                        ulUeScheduledInfoPointerArray[internalCellIndex][ulSfFromNackNode];
                    /** SPR 14204 Fix : End **/
                    if ( PNULL == ueScheduledInfo_p )
                    {
                        ltePanic(" ueScheduledInfo_p is NULL 3 subFrameNum [%d]\n", subFrameNum);
                    }

                    if (currentGlobalTickWithDelay <= (tempPhyTti + hiPduSubFrameNum))
                    {
                        ueScheduledInfo_p->strictRBAllocation = tempULHarqInfo_p->strictRBAllocation;
                        ueScheduledInfo_p->ttiCounter         = tempPhyTti;
                        ueScheduledInfo_p->ulHarqProcessId    = harqIndex;
                        /* +- SPR 17669*/
                        /* UL_MU_MIMO_CHG_START */
#ifdef SSI_DEBUG_STATS
                        if( UL_MU_MIMO_ENABLED == tempULHarqInfo_p->ulMuMimoStatus )
                        {
                            /* CA Stats Changes Start */
                            LTE_MAC_UPDATE_UL_MIMO_UE_RETX(internalCellIndex);
                            /* CA Stats Changes End */
                            tempULHarqInfo_p->ulMuMimoStatus = UL_MU_MIMO_DISABLED;
                        }
#endif
                        tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
                        /* UL_MU_MIMO_CHG_END */
                        /* 5798 Fix Start */
                        if( !(maxUeScheduledInUplink_p[ulSfFromNackNode]) )
                        {
                            ueScheduledInfo_p->flag = MAX_UE_SCHEDULED_NO_RETX_NO_GRANT;
                            ueScheduledInfo_p->ulCCEIndex =
                                CCE_NOT_ALLOCATED;
                            tempULHarqInfo_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE; 
                            /*TDD Config 0 Changes Start*/ 
                            pdcchInfo_p->nackUEInfo_p->
                                pdcchULNackNonAdaptiveUEArray
                                [pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount] = 
                                tempUeIndex;
                            pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount++;
                            /*TDD Config 0 Changes End*/ 
                            /* SPR 2379 Changes Start */ 
                            freeMemPool((void *)nackNode_p);
                            /* SPR 2379 Changes End */ 
                            nackNode_p = PNULL;
                            continue;

                        }
                        /* 5798 Fix End */
                        /* SPR 10803 [BUG 848] FIX START */
                        /* SPR 10753 Changes Start */
                        if ((!tempULHarqInfo_p->newDataIndicator && 
                                    ADAPTIVE_RETRANS_AFTER_HI_ACK == tempULHarqInfo_p->
                                    forceAckNextRetxType ) )
                        {
                            tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        }
                        /* SPR 10753 Changes End */

                        if ((MAC_SUCCESS == checkNumResourcesFromRBStartIndex( 
                                            /* SPR 21921 start end*/
                                            /* SPR 10803 [BUG 848] FIX END */
                                            tempULHarqInfo_p->riStart, 
                                            tempULHarqInfo_p->riLength, 
                                            pdcchInfo_p->rbMapNum, internalCellIndex)) && 
                                    /* SPR 10753 Changes Start */
                                    /*
                                       ( ADAPTIVE_RE_TX != 
                                       tempULHarqInfo_p->nextReTransmissionType) && */
                                    /* SPR 10753 Changes End */
                                    /*TDD Config 0 Changes Start*/ 
                                    /*Check if same HI resources are available as used
                                      earlier*/
                                    ( MAC_SUCCESS == checkAvailabilityOfHIResources(
                                                                                    phichSubFrameNum, iPhich,
                                                                                    tempULHarqInfo_p , internalCellIndex) ) )
                                /*TDD Config 0 Changes End*/ 
                        {
                                /* UL_MU_MIMO_CHG_START */
                                if (!pdcchInfo_p->localReTxRBMap[tempULHarqInfo_p->riStart]) 
                                {
                                    tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                                    pdcchInfo_p->localReTxRBMap[tempULHarqInfo_p->riStart] = 
                                        tempULHarqInfo_p->riLength;
                                }
                                else
                                {
                                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                }   
                                /* UL_MU_MIMO_CHG_END */    
                        }
                        else
                        {
                            tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        }
                        /* UL_MU_MIMO_CHG_START */
                        LOG_MAC_MSG (MAC_UL_MIMO_RE_TX_INFO_ID,LOGDEBUG,MAC_PDCCH,\
                                getCurrentTick(),tempUeIndex,tempULHarqInfo_p->riStart,\
                                tempULHarqInfo_p->riLength,\
                                tempULHarqInfo_p->currentRetransCount,tempULHarqInfo_p->nextReTransmissionType,\
                                0.0,0.0,__func__,"UE_RETX_INFO");
                        /* UL_MU_MIMO_CHG_END */

                        /*If Current Retx Counter is less than MAX_Retx Counter*/
                        /* SPR 21742 changes start*/
                        if ((tempULHarqInfo_p->currentRetransCount) < (ulUEContext_p->maxUlHARQTx -1))
                        /* SPR 21742 changes end */
                        {

                            /* MEAS_GAP_CHG */
                            /*TDD Config 0 Start*/
                            /*    ulschSubFrameNum = getExactULSubFrame((tempPhyTti + \
                                  hiPduSubFrameNum)%MAX_SUB_FRAME, internalCellIndex); */
                            ulschSubFrameNum = getUlOffset(subFrameNum,ulSfFromNackNode);
                            /*TDD Config 0 Start*/
                            if(TRUE == isTTIInMeasGapPeriod
                                    (ulUEContext_p, (tempPhyTti + hiPduSubFrameNum)))
                            {
                                measGapFlag=TRUE;
                                ueScheduledInfo_p->flag =
                                    NON_ADAPTIVE_RE_TRANS;
                            }                                
                            else if(  TRUE == isTTIInMeasGapPeriod
                                    (ulUEContext_p, (tempPhyTti +
                                                     hiPduSubFrameNum + ulschSubFrameNum)))
                            {
                                measGapFlag=TRUE;
                                ueScheduledInfo_p->flag =
                                    MEAS_GAP_RE_TX_NO_CCE_NO_GRANT;

                            }

                            if(TRUE == measGapFlag)
                            {


                                ueScheduledInfo_p->ulCCEIndex =
                                    CCE_NOT_ALLOCATED;
                                tempULHarqInfo_p->isPdcchAllocated = 
                                    UL_NON_ADAPTIVE_NO_CCE;

                                pdcchInfo_p->nackUEInfo_p->
                                    pdcchULNackNonAdaptiveUEArray
                                    [pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount] = 
                                    tempUeIndex;
                                pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount++;
                                /* SPR 2379 Changes Start */ 
                                freeMemPool((void *)nackNode_p);
                                /* SPR 2379 Changes End */ 
                                continue;
                            } 

                            if (ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType != \
                                    NON_ADAPTIVE_RE_TX)
                            {
                                /* If a UL Nack is Received for the UE and 
                                 * re-transmission is Adaptive then the 
                                 * previous nPhichSeq th bit from the LSB of 
                                 * the array phichSequenceNoPerPhichGroup_g
                                 * [Subframe No][nPhichGrp] 
                                 * should be made to set(PHICH is released).
                                 */
                                /*TDD Config 0 Changes Start*/
                                /*We do not need to free d phich resources since
                                 * they are being reset in PUSCH*/
                                /*TDD Config 0 Changes End*/ 
                                /* _empty_ul_sf*/
                                if ((LTE_TRUE == isUeTypeInvalid_g[internalCellIndex])
                                    )

                                {
                                    /* FORCED_ACK SPR#9374 Changes Start */
                                    ueScheduledInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
                                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                    ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                                    pdcchInfo_p->nackUEInfo_p->pdcchULNackAdaptiveUEArray
                                        [ulUEContext_p->userLocationType]
                                        [pdcchInfo_p->nackUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]] =
                                        tempUeIndex;
                                    pdcchInfo_p->nackUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]++;
                                    /* FORCED_ACK SPR#9374 Changes End */
                                    LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
                                            " allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");
                                }
                                else
                                {
                                    if (MAC_SUCCESS == allocatePDCCHForULNewReTx(
                                                &ulUEContext_p->\
                                                ueSearchSpaceInfo_p[subFrameNum],
                                                &cceAllocated,
                                                availableCCE_p,
                                                dciCCEInfo_p,
                                                ulUEContext_p,
                                                PNULL, //CA_phase2_csi_code
                                                internalCellIndex))
                                    {
                                        ueScheduledInfo_p->flag = RECEIVED_NACK;
                                        ueScheduledInfo_p->ulCCEIndex = cceAllocated;

                                        tempULHarqInfo_p->isPdcchAllocated = UL_ADAPTIVE_CCE;

                                        dciCCEContainerInfo_p->arrayCCEAllocated[
                                            dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
                                        dciCCEContainerInfo_p->countOfCCEAllocated++;
                                        /* ICIC changes start */
                                        /*TDD Config 0 Changes Start*/
                                        pdcchInfo_p->nackUEInfo_p->pdcchULNackAdaptiveUEArray
                                            [ulUEContext_p->userLocationType]
                                            [pdcchInfo_p->nackUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]] =
                                            tempUeIndex;
                                        pdcchInfo_p->nackUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]++;
                                        /*TDD Config 0 Changes End*/
                                        /* ICIC changes end */
                                        /* + SPR 5551 */
                                        dciCCEInfo_p[cceAllocated].dciPduInfo.cqiRequest = 0;
                                        /* - SPR 5551 */
                                        /* SPR 10753 Changes Start */
                                        tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
                                        /* SPR 10753 Changes End */
                                        
                                        LOG_MAC_MSG(MAC_UL_HARQ_ADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                                getCurrentTick(),
                                                tempUeIndex,harqIndex,
                                                tempULHarqInfo_p->currentRetransCount,
                                                DEFAULT_INT_VALUE,
                                                DEFAULT_INT_VALUE,
                                                0.0,0.0,FUNCTION_NAME,"PDCCH_ALLOC_SUCC");
                                    }
                                    else
                                    {
                                        /* FORCED_ACK SPR#9374 Changes Start */
                                        ueScheduledInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
                                        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                        /* SPR 10753 Changes Start */
                                        tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
                                        /* SPR 10753 Changes End */
                                        tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                                        pdcchInfo_p->nackUEInfo_p->pdcchULNackAdaptiveUEArray
                                            [ulUEContext_p->userLocationType]
                                            [pdcchInfo_p->nackUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]] =
                                            tempUeIndex;
                                        pdcchInfo_p->nackUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]++;
                                        /* FORCED_ACK SPR#9374 Changes End */
                                        LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
                                                " allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");

                                        LOG_MAC_MSG(MAC_UL_HARQ_ADAP_RETX_ID,LOGINFO,MAC_UL_HARQ,
                                                getCurrentTick(),
                                                tempUeIndex,harqIndex,
                                                tempULHarqInfo_p->currentRetransCount,
                                                DEFAULT_INT_VALUE,
                                                DEFAULT_INT_VALUE,
                                                0.0,0.0,FUNCTION_NAME,"PDCCH_ALLOC_FAIL");
                                    }
                                }
                            }
                            else
                            {
                                ueScheduledInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                                ueScheduledInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;

                                tempULHarqInfo_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                                pdcchInfo_p->nackUEInfo_p->pdcchULNackNonAdaptiveUEArray
                                    [pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount] =
                                    tempUeIndex;
                                pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount++;
                                /* SPR 10753 Changes Start */
                                tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
                                /* SPR 10753 Changes End */
                                LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, " Non_Adaptive_Re_Trans" \
                                        " of NACK Case \n");

                                LOG_MAC_MSG(MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                        getCurrentTick(),
                                        tempUeIndex,harqIndex,
                                        tempULHarqInfo_p->currentRetransCount,
                                        ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType,
                                        DEFAULT_INT_VALUE,
                                        0.0,0.0,FUNCTION_NAME,"");
                            }
                            /* 5798 Fix Start */
                            /*TDD Config 0 Changes Start*/
                            maxUeScheduledInUplink_p[ulSfFromNackNode]--;
                            /*TDD Config 0 Changes End*/
                            /* 5798 Fix End */
                        }
                        else
                        {  
                            ueScheduledInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
                            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                            LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH,
                                    "RECEIVED_NACK_RE_TRANS_COMPELETE for UE Index = %d and Harq ID = %d",
                                    tempUeIndex,harqIndex);
                            tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                            pdcchInfo_p->nackUEInfo_p->pdcchULNackNonAdaptiveUEArray
                                [pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount] = tempUeIndex;
                            pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount++;

							/* ysl add 20190912: fix ho crash - if another CFRA preamble comes and previous msg3 is in retransmission, ul harq process maintenance has some problem.
												So drop this new CFRA preamble */
							if ((PREAMBLE_BUSY == dlUECtxInfoArr_g[tempUeIndex].dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleStatus)
								&& (INVALID_PREAMBLE_IDX != dlUECtxInfoArr_g[tempUeIndex].dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx))
							{
								ContentionFreePreamble *localContentionFreePreamble_p =
									getContentionFreePreambleInfo(dlUECtxInfoArr_g[tempUeIndex].dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, internalCellIndex);
							
								localContentionFreePreamble_p->contentionFreeRACHRecvd = 0;
							}

                            LOG_MAC_MSG(MAC_UL_HARQ_RETX_MAX_TIME_ID,LOGINFO,MAC_UL_HARQ,
                                    getCurrentTick(),
                                    tempUeIndex,harqIndex,
                                    tempULHarqInfo_p->currentRetransCount,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    0.0,0.0,FUNCTION_NAME,"");			
                        }
                    }
                    else
                    { 
                        tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                        ueScheduledInfo_p->flag = UNSET_FLAG;
                        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                        if ((tempULHarqInfo_p->currentRetransCount) >= (ulUEContext_p->maxUlHARQTx -1))
                        {                        
                            resetHarqId( packetSubFrameNum,ulUEContext_p, internalCellIndex, packetSysFrameNum);
                            /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                            freeULHarqProcess(tempULHarqInfo_p);
#endif
                            /* SPR 16916 FIX END */
                        }
                        else
                        {
#ifndef DL_UL_SPLIT
                            tempULHarqInfo_p->currentRetransCount += \
                                                                     ((currentGlobalTickWithDelay - tempPhyTti)/MAX_SUB_FRAME) + 1;
                            ulHarqTimerStart( subFrameNum,
                                    sysFrameNum,
                                    tempUeIndex,
                                    harqIndex,
                                    /* + SPS_TDD_Changes */
                                    DRA_SCHEDULING,
                                    /* - SPS_TDD_Changes */
                                    internalCellIndex,
                                    ulSfFromNackNode);
#else
                            /* SPR 16916 FIX START */
#ifdef KPI_STATS
                            freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                            freeULHarqProcess(tempULHarqInfo_p);
#endif
                            /* SPR 16916 FIX END */
#endif
                        }
                        LTE_MAC_UT_LOG(LOG_WARNING, LOG_PDCCH,
                                "nackQueue Node later than n+3, so discarding the UE for"
                                "NACK  retransmission resources\n");

                        LOG_MAC_MSG(MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                                getCurrentTick(),
                                tempUeIndex,harqIndex,
                                tempPhyTti,
                                tempULHarqInfo_p->nextReTransmissionType,
                                DEFAULT_INT_VALUE,
                                0.0,0.0,__func__,"DELAY_NACK_FREE_ULHARQ");                                
                    }
                }
            }
            else
            {
                LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "UL NACK of UE ID:%d" \
                        "Having PHY TTI %d,Pending to delete is set",
                        tempUeIndex,tempPhyTti);
            }
            freeMemPool((void *)nackNode_p);
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PDCCH, "nackQueue Node not valid...\n");
        }
    }
    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH",
            "[%s] \n\t nackUEInfo_p->nonAdaptiveCount = %d nackUEInfo_p->adaptiveCount[CC_USER] = %d availableULRBCount_g[%d]\n"
            ,__func__,nackUEInfo_p->nonAdaptiveCount,nackUEInfo_p->adaptiveCount[CC_USER], availableULRBCount_g);
    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Exit",__func__);
    return MAC_SUCCESS;
}
#endif

/*SPR_3305_FIX_START*/
/*********************************************************************************
 * Function Name : handleNackNodeForTcrnti
 * Inputs        : tempUeIndex, tempPhyTti
 *                 ulDelay - The delay of execution legs in terms of the number
 *                           subFrames w.r.t. PHY & the current processing being
 *                           done at MAC for intended tick at PHY.
 *                 currentGlobalTick - The present Global Tick
 *                 availableCCE_p : This represent the number of available CCE
 *                     and if CCE's are allocated this would be deceremented by  one.
 *                 dciCCEInfo_p : This is pointer to structure on which Free CCE
 *                     Information is there and once cce is allocated ,updated
 *                     information is written on the reserved CCE.
 *                 dciCCEContainerInfo_p -
 *                 availableULRBCount_p- Number of available uplink resources
 *                 userLocationType - User location(Cell centre/cell edge)
 *                 maxUeScheduledInUplink_p - maximum number of Ue's to be scheduled
 *                    in Uplink
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : nonAdaptiveArrayIndex_p,adaptiveArrayIndex_p
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:  : This function will read the Nack queue and store the
 *                 information in the PDCCH UPLINK OUTPUT container.
 ************************************************************************************/
#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129195) */            
    /* SPR 15909 fix start */
MacRetType handleNackNodeForTcrnti( UInt16 tempUeIndex, tickType_t tempPhyTti,
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt8 ulDelay, tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        SInt32 *availableULRBCount_p,
        UInt16 *nonAdaptiveArrayIndex_p,
        UInt16 *adaptiveArrayIndex_p,
        /* ICIC changes start */
        UInt8 userLocationType,
        /* ICIC changes end */
        /* SPR 5798 Fix Start */
        UInt32 *maxUeScheduledInUplink_p,
        /* SPR 5798 Fix End */
        InternalCellIndex internalCellIndex
        )
{
    UInt8 nPhichGrp                        = 0;
    UInt8 nPhichSf                         = 0;
    UInt8 harqIndex = INVALID_HARQ_ID;
    UInt32 cceAllocated = 0;
    TempCRNTICtx *tempUEContext_p = PNULL;
    ULHarqInfo *   tempULHarqInfo_p = PNULL;
    PdcchULNackUEInfo *nackUEInfo_p = PNULL;
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    UInt16 pdcchUpLinkOutputContainerTick = 0;

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */

    /*
     * we calculate the value of pdcchUpLinkOutputContainerTick
     * All ticks are with respect to the tick when the information is to be sent to PHY interface.
     */
    pdcchUpLinkOutputContainerTick = currentGlobalTickWithDelay % MAX_PDCCH_CONTAINER;
    UInt8 rbMapNum = MODULO_SIXTEEN((currentGlobalTickWithDelay + FDD_HARQ_OFFSET));
    harqIndex = MODULO_EIGHT(tempPhyTti);
    /*Get the temp Context using received index*/
    tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[tempUeIndex].tempCRNTICtx_p;
    tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + pdcchUpLinkOutputContainerTick);
    nackUEInfo_p = &tempPdcchULOutput_p->pdcchULNackTempUEInfo;

    if (tempUEContext_p != PNULL)
    {
        tempULHarqInfo_p = &(tempUEContext_p->ulHarq);
        tempDirectIndexingUEInfo_p = \
                                     &(tempPdcchULOutput_p->directIndexingTempUEInfo[tempUeIndex]);
        nPhichGrp = tempULHarqInfo_p->nPhichGrp;
        nPhichSf  = tempULHarqInfo_p->sfUsed;

        if(!tempULHarqInfo_p->riLength)
        {
            /* SPR 5235 : Start*/
            PDCCHRachNode *pdcchRachNode_p = PNULL;
            /* SPR 5235 : End*/
            LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                    currentGlobalTick,tempUeIndex,harqIndex, \
                    tempULHarqInfo_p->nextReTransmissionType,\
                    tempULHarqInfo_p->currentRetransCount,\
                    DEFAULT_INT_VALUE,\
                    0.0,0.0,__func__,"NACK_ZERO_BAND");
            /* SPR 5235 : Start*/
            GET_MEM_FROM_POOL(PDCCHRachNode,
                    pdcchRachNode_p,
                    sizeof(PDCCHRachNode), PNULL);
            if(PNULL != pdcchRachNode_p)
            {
                pdcchRachNode_p->rnti = tempUEContext_p->tcRNTI;
                /*SPR 21103 Start*/
                if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(FREE_TCRNTI, pdcchRachNode_p,
                                                  internalCellIndex))
                {
                    freeMemPool(pdcchRachNode_p);
                }
                /*SPR 21103 End*/
            }
            /* SPR 5235 : End*/

            return MAC_FAILURE;
        }
        else
        {
            /* SPR 5798 Fix Start */
            if(!(*maxUeScheduledInUplink_p))
            {
                tempDirectIndexingUEInfo_p->flag = MAX_UE_SCHEDULED_NO_RETX_NO_GRANT_FOR_MSG3;
                tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                tempUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[*nonAdaptiveArrayIndex_p] =
                    tempUeIndex;
                (*nonAdaptiveArrayIndex_p)++;
                return MAC_SUCCESS;

            }
            /* SPR 5798 Fix End */
            if (currentGlobalTickWithDelay <= (tempPhyTti + FDD_HARQ_OFFSET))
            {

                tempDirectIndexingUEInfo_p->harqIndex = harqIndex;
                tempDirectIndexingUEInfo_p->ttiCounter = tempPhyTti;
                tempDirectIndexingUEInfo_p->strictRBAllocation = \
                                                                tempULHarqInfo_p->strictRBAllocation;
                /*If Current Retx Counter is less than MAX_Retx Counter*/
                if (tempULHarqInfo_p->currentRetransCount <
                        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                         rachConfigInfo.rachConfiguration.maxHarqMsg3Tx -1))
                {

                    if ((MAC_SUCCESS == checkNumResourcesFromRBStartIndex(
                                    tempULHarqInfo_p->riStart,
                                    tempULHarqInfo_p->riLength,
                                    rbMapNum, internalCellIndex)) 
                            && (MAC_SUCCESS == checkHIResources(\
                                    phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask,\
                                    phichSequenceNoPerPhichGroup_g[internalCellIndex][(nPhichSf + FDD_DATA_OFFSET + FDD_HARQ_OFFSET) % MAX_PHICH_CONTAINER][nPhichGrp].bitMask)))

                    {
                        tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                    }
                    else
                    {
                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    }

                    if (tempUEContext_p->ulHarq.nextReTransmissionType != \
                            NON_ADAPTIVE_RE_TX)
                    {   

                      if (LTE_TRUE == isUeTypeInvalid_g[internalCellIndex])
		      {
                            tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                            tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_NO_CCE_FOR_MSG3;
                            tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                            tempUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                            nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[*nonAdaptiveArrayIndex_p] =
                                tempUeIndex;
                            (*nonAdaptiveArrayIndex_p)++;

                            LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    currentGlobalTick,tempUeIndex,harqIndex, \
                                    isUeTypeInvalid_g[internalCellIndex],\
                                    tempULHarqInfo_p->nextReTransmissionType,\
                                    tempULHarqInfo_p->currentRetransCount,\
                                    0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");

		      }
                      else
		      {		      
                        /* SPR 6859 changes start */
                        if (MAC_SUCCESS == allocatePDCCHForDCIFormatCommonSpace(
                                    AGGREGATION_LEVEL_FOR_COMMON_SPACE,
                                    &cceAllocated,
                                    availableCCE_p,
                                    dciCCEInfo_p,
                                    internalCellIndex))
                        /* SPR 6859 changes end */
                        {
                            tempDirectIndexingUEInfo_p->flag = \
                                                               RECEIVED_NACK;
                            tempDirectIndexingUEInfo_p->ulCCEIndex =\
                                                                    cceAllocated;
                            tempUEContext_p->isPdcchAllocated =\
                                                               UL_ADAPTIVE_CCE;
                            dciCCEContainerInfo_p->arrayCCEAllocated[
                                dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
                            dciCCEContainerInfo_p->countOfCCEAllocated++;
                            /* ICIC changes start */
                            nackUEInfo_p->pdcchULNackAdaptiveUEArray[userLocationType]
                                [adaptiveArrayIndex_p[userLocationType]] = 
                                tempUeIndex;
                            adaptiveArrayIndex_p[userLocationType]++;
                            /* ICIC changes end */
                            /* + SPR 5551 */
                            dciCCEInfo_p[cceAllocated].dciPduInfo.cqiRequest = 0;
                            /* - SPR 5551 */
                            /* SPR 6859 Changes start */
                            dciCCEInfo_p[cceAllocated].dciPduInfo.txPower = 
                                       cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                       cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
                            /* SPR 6859 Changes end */
                            LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    currentGlobalTick,tempUeIndex,harqIndex, \
                                    tempPhyTti,\
                                    tempULHarqInfo_p->nextReTransmissionType,\
                                    cceAllocated,\
                                    0.0,0.0,__func__,"NACK_DCI_SUCC_ULHARQ");
                        }
                        else
                        {
                            tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                            tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_NO_CCE_FOR_MSG3;
                            tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                            tempUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                            nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[*nonAdaptiveArrayIndex_p] =
                                tempUeIndex;
                            (*nonAdaptiveArrayIndex_p)++;

                            LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    currentGlobalTick,tempUeIndex,harqIndex, \
                                    tempPhyTti,\
                                    tempULHarqInfo_p->nextReTransmissionType,\
                                    tempULHarqInfo_p->currentRetransCount,\
                                    0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");
                        }
		      }
                    }
                    else
                    {
                        tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                        tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                        tempUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                        nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[*nonAdaptiveArrayIndex_p] =
                            tempUeIndex;
                        (*nonAdaptiveArrayIndex_p)++;

                        LOG_MAC_MSG(MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                getCurrentTick(),
                                tempUeIndex,harqIndex,
                                tempULHarqInfo_p->currentRetransCount,
                                tempULHarqInfo_p->nextReTransmissionType,
                                DEFAULT_INT_VALUE,
                                0.0,0.0,FUNCTION_NAME,"");        
                    }
                    /* SPR 5798 Fix Start */
                    (*maxUeScheduledInUplink_p)--;
                    /* SPR 5798 Fix End */
                    *availableULRBCount_p -= tempULHarqInfo_p->riLength;
                }
                else
                {
                    tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    tempUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                    nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[*nonAdaptiveArrayIndex_p] = tempUeIndex;
                    (*nonAdaptiveArrayIndex_p)++;
                }

            }
            else
            {
                /* SPR 5235 : Start*/
                PDCCHRachNode *pdcchRachNode_p = PNULL;
                /* SPR 5235 : End*/
                tempUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /* SPR 5235 : Start*/
                GET_MEM_FROM_POOL(PDCCHRachNode,
                        pdcchRachNode_p,
                        sizeof(PDCCHRachNode), PNULL);
                if(PNULL != pdcchRachNode_p)
                {
                    pdcchRachNode_p->rnti = tempUEContext_p->tcRNTI;
                    /*SPR 21103 Start*/
                    if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(FREE_TCRNTI, pdcchRachNode_p,
                                internalCellIndex))
                    {
                        freeMemPool(pdcchRachNode_p);
                    }
                    /*SPR 21103 End*/
                }
                /* SPR 5235 : End*/

                /* SPR 1996 Start*/
                LOG_MAC_MSG(MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,harqIndex, \
                        tempPhyTti,\
                        tempULHarqInfo_p->nextReTransmissionType,\
                        DEFAULT_INT_VALUE,\
                        0.0,0.0,__func__,"DELAY_NACK_FREE_ULHARQ");
            }
        }
    }
    else
    {
        /* ADD LOG */
    }

    return MAC_SUCCESS;
}

#endif
    /*SPR_3305_FIX_END*/
/*********************************************************************************************
 * Function Name : handleNackNodeForTcrntiTdd
 * Inputs        : harqIndex
 *                 tempUeIndex
 *                 tempPhyTti
 *                 currentGlobalTick
 *                 availableCCE_p
 *                 dciCCEInfo_p
 *                 dciCCEContainerInfo_p
 *                 userLocationType
 *                 maxUeScheduledInUplink_p
 *                 ulSubFrameNum
 *                 pdcchInfo_p
 *                 internalCellIndex
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:  : This function will read the Nack queue and store the
 *                 information in the PDCCH UPLINK OUTPUT container in TDD
**********************************************************************************************/
/* msg3_retx_tdd_support_start */
#ifdef TDD_CONFIG
MacRetType handleNackNodeForTcrntiTdd( 
        UInt8 harqIndex,
        UInt16 tempUeIndex, 
        /* SPR 15909 fix start */
        tickType_t tempPhyTti,
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt8 userLocationType,
        UInt8 *maxUeScheduledInUplink_p,
        UInt8 ulSubFrameNum,
        PdcchInfo* pdcchInfo_p,
        InternalCellIndex internalCellIndex) 
{
    UInt8 phichSubFrameNum                 = 0;
    UInt8 iPhich                           = 0;
    UInt32 cceAllocated = 0;
    TempCRNTICtx *tempCRNTICtx_p = PNULL;
    ULHarqInfo *   tempULHarqInfo_p = PNULL;
    PdcchULNackUEInfo *nackUEInfo_p = PNULL;
    UeScheduledInfoForMsg3 *ueScheduledInfo_p = PNULL;

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithDelay = currentGlobalTick;
    /* SPR 15909 fix end */
    UInt8  hiPduSubFrameNum                = 0;
    /*
     * we calculate the value of pdcchUpLinkOutputContainerTick
     * All ticks are with respect to the tick when the information is to be sent to PHY interface.
     */
    tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[tempUeIndex].tempCRNTICtx_p;
    UInt8 packetSubFrameNum = currentGlobalTickWithDelay%MAX_SUB_FRAME;
    UInt8 subFrameNum = tempPhyTti%MAX_SUB_FRAME;
                    
    ueScheduledInfo_p = &tempCRNTICtx_p->ulUeScheduledInfoPointerArray; 
    nackUEInfo_p = pdcchInfo_p->nackTempUEInfo_p;

    if (tempCRNTICtx_p != PNULL)
    {
        tempULHarqInfo_p = &(tempCRNTICtx_p->ulHarq);
        /*TDD Config 0 Changes Start*/
        phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
        phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
        iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
        /*TDD Config 0 Changes End*/

        if(!tempULHarqInfo_p->riLength)
        {
            /* SPR 5235 : Start*/
            PDCCHRachNode *pdcchRachNode_p = PNULL;
            /* SPR 5235 : End*/
            LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                    currentGlobalTick,tempUeIndex,harqIndex, \
                    tempULHarqInfo_p->nextReTransmissionType,\
                    tempULHarqInfo_p->currentRetransCount,\
                    DEFAULT_INT_VALUE,\
                    0.0,0.0,__func__,"NACK_ZERO_BAND");
            /* SPR 5235 : Start*/
            GET_MEM_FROM_POOL(PDCCHRachNode,
                    pdcchRachNode_p,
                    sizeof(PDCCHRachNode), PNULL);
            if(PNULL != pdcchRachNode_p)
            {
                pdcchRachNode_p->rnti = tempCRNTICtx_p->tcRNTI;
                /*SPR 21103 Start*/
                if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(FREE_TCRNTI, pdcchRachNode_p,
                            internalCellIndex))
                {
                    freeMemPool(pdcchRachNode_p);
                }
                /*SPR 21103 End*/
            }
            /* SPR 5235 : End*/

            return MAC_FAILURE;
        }
        else
        {
            /* msg3_retx_tdd_support_start */
            if (! isValidULSubFrameForHIPdu ( subFrameNum, &hiPduSubFrameNum ,internalCellIndex))
            {
                ltePanic("Invalid SubFrame[%d] for UL HI Pdu\n", subFrameNum);
            }

            if (!isValidSubframeNumForDCI_0 (packetSubFrameNum,internalCellIndex))
            {
                ltePanic("Invalid SubFrame[%d] for DCI -0\n", packetSubFrameNum);

            }
            /* msg3_retx_tdd_support_end */
            /* SPR 5798 Fix Start */
            if(!maxUeScheduledInUplink_p[ulSubFrameNum])
            {
                /* Filling the ACK NACK status */
                ueScheduledInfo_p->flag = MAX_UE_SCHEDULED_NO_RETX_NO_GRANT_FOR_MSG3;
                ueScheduledInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                /* Filling CCE allocation type */
                tempULHarqInfo_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nackUEInfo_p->nonAdaptiveCount] =
                    tempUeIndex;
                nackUEInfo_p->nonAdaptiveCount++;
                return MAC_SUCCESS;

            }
            /* SPR 5798 Fix End */
            if (currentGlobalTickWithDelay <= (tempPhyTti + hiPduSubFrameNum))
            {

                ueScheduledInfo_p->ulHarqProcessId = harqIndex;
                ueScheduledInfo_p->ttiCounter = tempPhyTti;
                ueScheduledInfo_p->strictRBAllocation = tempULHarqInfo_p->strictRBAllocation;
                /*If Current Retx Counter is less than MAX_Retx Counter*/
                if (tempULHarqInfo_p->currentRetransCount <
                        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                                                 rachConfigInfo.rachConfiguration.maxHarqMsg3Tx -1))
                {

                    if ((MAC_SUCCESS == checkNumResourcesFromRBStartIndex(
                                    tempULHarqInfo_p->riStart,
                                    tempULHarqInfo_p->riLength,
                                    pdcchInfo_p->rbMapNum,
                                    internalCellIndex))
                              /*TDD Config 0 Changes Start*/ 
                              /*Check if same HI resources are available as used
                              earlier*/
                        && (MAC_SUCCESS == checkAvailabilityOfHIResources(
                                      phichSubFrameNum,
                                      iPhich,
                                      tempULHarqInfo_p, internalCellIndex)))
                             /*TDD Config 0 Changes End*/ 
                                 
                    {
                        tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                    }
                    else
                    {
                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    }
                    if (tempCRNTICtx_p->ulHarq.nextReTransmissionType != \
                            NON_ADAPTIVE_RE_TX)
                    {
                        /*TDD Config 0 Changes Start*/
                        /*We do not need to free d phich resources since
                         * they are being reset in PUSCH*/
                        /*TDD Config 0 Changes End*/ 

                        /* SPR 6859 changes start */
                        if (MAC_SUCCESS == allocatePDCCHForDCIFormatCommonSpace(
                                    AGGREGATION_LEVEL_FOR_COMMON_SPACE,
                                    &cceAllocated,
                                    availableCCE_p,
                                    dciCCEInfo_p,
                                    internalCellIndex
                                    ))
                        /* SPR 6859 changes end */
                        {
                            ueScheduledInfo_p->flag = \
                                                               RECEIVED_NACK;
                            ueScheduledInfo_p->ulCCEIndex =\
                                                                    cceAllocated;
                            tempULHarqInfo_p->isPdcchAllocated =\
                                                               UL_ADAPTIVE_CCE;
                            dciCCEContainerInfo_p->arrayCCEAllocated[
                                dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
                            dciCCEContainerInfo_p->countOfCCEAllocated++;
                            /* ICIC changes start */
                            nackUEInfo_p->pdcchULNackAdaptiveUEArray[userLocationType]
                                [nackUEInfo_p->adaptiveCount[userLocationType]] = 
                                tempUeIndex;
                            nackUEInfo_p->adaptiveCount[userLocationType]++;
                            /* ICIC changes end */
                            /* + SPR 5551 */
                            dciCCEInfo_p[cceAllocated].dciPduInfo.cqiRequest = 0;
                            /* - SPR 5551 */
                            /* SPR 6859 Changes start */
                            dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
                                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
                            /* SPR 6859 Changes end */
                            LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    currentGlobalTick,tempUeIndex,harqIndex, \
                                    tempPhyTti,\
                                    tempULHarqInfo_p->nextReTransmissionType,\
                                    cceAllocated,\
                                    0.0,0.0,__func__,"NACK_DCI_SUCC_ULHARQ");

                            LOG_MAC_MSG(MAC_UL_HARQ_ADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                    getCurrentTick(),
                                    tempUeIndex,harqIndex,
                                    tempULHarqInfo_p->currentRetransCount,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    0.0,0.0,FUNCTION_NAME,"PDCCH_TCRNTI_ALLOC_SUCC");
                        }
                        else
                        {
                            tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                            /* In case of unavailability of CCEs for Adaptive
                             * ReTx in Msg3,we shall put the entry in
                             * Nonadaptive queue and send forced ack while
                             * processing it in PHICH module*/
                            ueScheduledInfo_p->flag = NON_ADAPTIVE_NO_CCE_FOR_MSG3;
                            ueScheduledInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                            tempULHarqInfo_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                            nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nackUEInfo_p->nonAdaptiveCount] =
                                tempUeIndex;
                            nackUEInfo_p->nonAdaptiveCount++;

                            LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    currentGlobalTick,tempUeIndex,harqIndex, \
                                    tempPhyTti,\
                                    tempULHarqInfo_p->nextReTransmissionType,\
                                    tempULHarqInfo_p->currentRetransCount,\
                                    0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");

                            LOG_MAC_MSG(MAC_UL_HARQ_ADAP_RETX_ID,LOGINFO,MAC_UL_HARQ,
                                    getCurrentTick(),
                                    tempUeIndex,harqIndex,
                                    tempULHarqInfo_p->currentRetransCount,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    0.0,0.0,FUNCTION_NAME,"PDCCH_TCRNTI_ALLOC_FAIL");
                        }
                    }
                    else
                    {
                        ueScheduledInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                        ueScheduledInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                        tempULHarqInfo_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                        nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nackUEInfo_p->nonAdaptiveCount] =
                            tempUeIndex;
                        nackUEInfo_p->nonAdaptiveCount++;

                        LOG_MAC_MSG(MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                getCurrentTick(),
                                tempUeIndex,harqIndex,
                                tempULHarqInfo_p->currentRetransCount,
                                tempULHarqInfo_p->nextReTransmissionType,
                                DEFAULT_INT_VALUE,
                                0.0,0.0,FUNCTION_NAME,"");
                    }
                    /* SPR 5798 Fix Start */
                    maxUeScheduledInUplink_p[ulSubFrameNum]--;
                    /* SPR 5798 Fix End */
                }
                else
                {
                    ueScheduledInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                    nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nackUEInfo_p->nonAdaptiveCount] 
                            = tempUeIndex;
                    nackUEInfo_p->nonAdaptiveCount++;

                    LOG_MAC_MSG(MAC_UL_HARQ_RETX_MAX_TIME_ID,LOGINFO,MAC_UL_HARQ,
                            getCurrentTick(),
                            tempUeIndex,harqIndex,
                            tempULHarqInfo_p->currentRetransCount,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            0.0,0.0,FUNCTION_NAME,"");	
                }

            }
            else
            {
                /* SPR 5235 : Start*/
                PDCCHRachNode *pdcchRachNode_p = PNULL;
                /* SPR 5235 : End*/
                tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                ueScheduledInfo_p->flag = UNSET_FLAG;
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /* SPR 5235 : Start*/
                GET_MEM_FROM_POOL(PDCCHRachNode,
                        pdcchRachNode_p,
                        sizeof(PDCCHRachNode), PNULL);
                if(PNULL != pdcchRachNode_p)
                {
                    pdcchRachNode_p->rnti = tempCRNTICtx_p->tcRNTI;
                    /*SPR 21103 Start*/
                    if(MAC_FAILURE == putEntryInPdcchRachInterfaceQueue(FREE_TCRNTI, pdcchRachNode_p,
                                internalCellIndex))
                    {
                        freeMemPool(pdcchRachNode_p);
                    }
                    /*SPR 21103 End*/
                }
                /* SPR 5235 : End*/

                /* SPR 1996 Start*/
                LOG_MAC_MSG(MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,harqIndex, \
                        tempPhyTti,\
                        tempULHarqInfo_p->nextReTransmissionType,\
                        DEFAULT_INT_VALUE,\
                        0.0,0.0,__func__,"DELAY_NACK_FREE_ULHARQ");
            }
        }
    }
    else
    {
        /* ADD LOG */
    }

    return MAC_SUCCESS;
}

#endif
/* msg3_retx_tdd_support_end */

/******************************************************************************
 * Function Name : processPdcchOrderQueue
 * Inputs        : subFrameNum -  current sub frame number for which CCEs 
 *                       are being allocated.
 *                 currentGlobalTick - The present Global Tick
 *                                     CCE and if CCE's are allocated this 
 *                                     would be deceremented by one.
 *                 maxUeScheduledInUplink_p - maximum number of Ue's to be scheduled
 *                    in Uplink
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : availableCCE_p : This represent the number of available
 *                                  CCE and if CCE's are allocated this
 *                                  would be deceremented by one.
 *                 dciCCEContainerInfo_p - DCI CCE container contains the 
 *                                  Info of UE for which CCE allocated
 *                 dciCCEInfo_p : This is pointer to structure on which Free
 *                                CCE Information is there and once cce is
 *                                allocated, updated information is written
 *                                on the reserved CCE.
 * Returns       : None
 * Description   : This function will process the PDCCH Order Queue nodes 
 *                 for not more than max count configured by RRC.  
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
void processPdcchOrderQueue (
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 *availableCCE_p,
        DciCCEContainerInfo * dciCCEContainerInfo_p,
        DciCCEInfo * dciCCEInfo_p,
        UInt32 subFrameNum,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick
        /* SPR 15909 fix end */
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
        )
{
    UInt8 pdcchOrderPerTTICount = 0;
    PDCCHOrderNode *pdcchOrderNode_p = PNULL;
    MacRetType returnValue = MAC_SUCCESS;
    UInt32 pdcchOrderAdvQCount = sQueueCount(pdcchOrderAdvanceQueue_gp[internalCellIndex]);
    UInt32 pdcchOrderQCount = COUNT_PDCCH_ORDER_Q(internalCellIndex);

    UInt32 totalPdcchOrderQCnt =  pdcchOrderAdvQCount + pdcchOrderQCount;
       /*EICIC +*/
    UInt32 sysFrameNum 			= 0;

    UInt8  absSubFrameNum 		= 0;
    UInt32 	numBitsAbsPattern = 0;
    UInt32 ueTypeToSchedule[MAX_NUM_CELL] = {0};

    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
   getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

    /*Calculate the SFN and SF by considering the delay to PHY*/
	    subFrameNum=subFrameNum+PHY_DL_DELAY;
    
    if(subFrameNum >= MAX_SUB_FRAME)
    {
        subFrameNum -= MAX_SUB_FRAME;
        sysFrameNum++;
        if (sysFrameNum == MAX_SFN_VALUE)
        {
            sysFrameNum = 0;
        }
    }
   
#ifdef FDD_CONFIG
       numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
   {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
   {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
   }
   else
   {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
   }
#endif

     absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) %numBitsAbsPattern) ;

  if ((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (isEicicEnabled_g[internalCellIndex])) /* Work as victim*/
  {
      ueTypeToSchedule[internalCellIndex] = VICTIM_UE;

  }
  else
  {
    	ueTypeToSchedule[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
  }
    }
    else
    {
	    ueTypeToSchedule[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
    }
    /* EICIC Optimization +- */
    /*EICIC +*/
  
  /* 5492 changes start */
    /* 5492 changes end */

    /*If no node in the pdcchOrderQueue, return to the calling function*/
    if(0 == totalPdcchOrderQCnt)
    {
        return;
    }    

    /*Assign pdcchOrderPerTTICount with a value not to exceed maximum PDCCH
      Order that can be processed in one tick */
    if(totalPdcchOrderQCnt > maxPdcchOrderperTTI_g[internalCellIndex])
    {
        pdcchOrderPerTTICount = maxPdcchOrderperTTI_g[internalCellIndex];
    }
    else
    {
        pdcchOrderPerTTICount = totalPdcchOrderQCnt;
    }
    /* 5492 changes start */
    if(pdcchOrderPerTTICount > (*maxUeScheduledInDownlink_p))
    {
        pdcchOrderPerTTICount = (*maxUeScheduledInDownlink_p);
    }
    /* 5492 changes end */

    if(pdcchOrderAdvQCount) 
    {   
        while((pdcchOrderPerTTICount--) && (pdcchOrderAdvQCount--) 
               && (*maxUeScheduledInDownlink_p ) )
        {
            pdcchOrderNode_p = (PDCCHOrderNode*)getFirstNode(pdcchOrderAdvanceQueue_gp[internalCellIndex]);
       /* EICIC_Coverity CID 71110 Start */     
         if(pdcchOrderNode_p)
         {
      /* EICIC_Coverity CID 71110 End */   
	    if ((ueTypeToSchedule[internalCellIndex]== NON_VICTIM_UE) && (dlUECtxInfoArr_g[pdcchOrderNode_p->ueIndex].dlUEContext_p->userType == VICTIM_UE))		
	    	{
	    	    pdcchOrderNode_p =
                        /* SPR 3342 Fix Start */
                        (PDCCHOrderNode*)popNode(pdcchOrderAdvanceQueue_gp[internalCellIndex]);    
	    	    /* EICIC_Coverity CID 10145 Start */
	    	    if(pdcchOrderNode_p)
                 {
	            if(pushNode(pdcchOrderAdvanceQueue_gp[internalCellIndex], 
                            &(pdcchOrderNode_p->anchorNode)))
		    {
		       lteWarning ("EICIC : pushNode() failed for processPdcchOrderQueue ");
	            		freeMemPool(pdcchOrderNode_p);
						/* + Coverity 71233 */ 
						pdcchOrderNode_p = PNULL;
						/* - Coverity 71233 */ 
	    	    }
                 }		
	    	    /* EICIC_Coverity CID 10145 End */
				pdcchOrderPerTTICount++;
				
				continue;
	    	}
                returnValue = processPdcchOrderQueueNode(
                        availableCCE_p,
                        dciCCEContainerInfo_p,
                        dciCCEInfo_p,
                        subFrameNum,
                        pdcchOrderNode_p,
                        currentGlobalTick
                        /* 5492 changes start */
                        ,maxUeScheduledInDownlink_p,
                        /* 5492 changes end */
                        internalCellIndex
                        );

                if(MAC_SUCCESS ==  returnValue )
                {
                    pdcchOrderNode_p =
                        /* SPR 3342 Fix Start */
                        (PDCCHOrderNode*)popNode(pdcchOrderAdvanceQueue_gp[internalCellIndex]);
                    /* SPR 3342 Fix End */
                    /* +COVERITY 10145 */
                    if (pdcchOrderNode_p)
                    {
                        freeMemPool(pdcchOrderNode_p);
                        pdcchOrderNode_p = PNULL;
                    }
                    /* -COVERITY 10145 */
                }
                else if(MAC_FAILURE == returnValue)
                {
                    return;
                }

            }
        }/*End of advance queue while loop*/
    }
    if((pdcchOrderPerTTICount) && (pdcchOrderQCount))
    {
        while((pdcchOrderPerTTICount--) && (pdcchOrderQCount--) 
                && (*maxUeScheduledInDownlink_p))
        {

            DEQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, (void *)&pdcchOrderNode_p, internalCellIndex);
	    if(pdcchOrderNode_p)
            { 
	      if ((ueTypeToSchedule[internalCellIndex] == NON_VICTIM_UE) && (dlUECtxInfoArr_g[pdcchOrderNode_p->ueIndex].dlUEContext_p->userType == VICTIM_UE))		
	    	{
	          /* EICIC_Coverity CID 24364 Start */	
	            if(pushNode(pdcchOrderAdvanceQueue_gp[internalCellIndex], 
                            &(pdcchOrderNode_p->anchorNode)))
	            {
	                     lteWarning ("EICIC : pushNode() failed for processPdcchOrderQueue ");
	            		freeMemPool(pdcchOrderNode_p);
				pdcchOrderNode_p=PNULL;
	            }	
	            /* EICIC_Coverity CID 24364 End */
				pdcchOrderPerTTICount++;
				
				continue;
	    	}
         
                returnValue = processPdcchOrderQueueNode(
                        availableCCE_p,
                        dciCCEContainerInfo_p,
                        dciCCEInfo_p,
                        subFrameNum,
                        pdcchOrderNode_p,
                        currentGlobalTick
                        /* 5492 changes start */
                        ,maxUeScheduledInDownlink_p,
                        /* 5492 changes end */
                        internalCellIndex

                        );

                if(MAC_SUCCESS ==  returnValue )
                {
                   FREE_MEM_NODE_PDCCH_ORDER_Q(pdcchOrderNode_p);

                }
                else if(MAC_FAILURE == returnValue)
                {

                    /* Coverity CID 24364 FIX start */
                    if( pushNode(pdcchOrderAdvanceQueue_gp[internalCellIndex], 
                        &(pdcchOrderNode_p->anchorNode)) )
                    {
                        lteWarning ("pushNode() failed for pdcch Order Advance Queue");
                        FREE_MEM_NODE_PDCCH_ORDER_Q(pdcchOrderNode_p);
                    }
                    /* Coverity CID 24364 FIX end */
                    pdcchOrderNode_p = PNULL;

                }
            }
        }/*End of while loop*/
    }
    return;
} 
/******************************************************************************
 * Function Name : processPdcchOrderQueueNode
 * Inputs        : subFrameNum -  current sub frame number for which CCEs 
 *                       are being allocated.
 *                 currentGlobalTick - The present Global Tick
 *                                     CCE and if CCE's are allocated this 
 *                                     would be deceremented by one.
 *                 maxUeScheduledInDownlink_p - maximum number of Ue's to be scheduled
 *                    in DL
 *                                   on the reserved CCE.
 *                 pdcchOrderNode_p : Pointer to PDCCH Order Queue Node
 *                 internalCellIndex - Cell-Index at MAC
 *                    
 * Outputs       : availableCCE_p : This represent the number of available
 *                                  CCE and if CCE's are allocated this
 *                                  would be deceremented by one.
 *                 dciCCEContainerInfo_p - DCI CCE container contains the 
 *                                  Info of UE for which CCE allocated
 *                 dciCCEInfo_p : This is pointer to structure on which Free
 *                                CCE Information is there and once cce is
 *                                allocated, updated information is written
 *                                on the reserved CCE.
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will process the PDCCH Order Queue nodes 
 *                  for not more than max count configured by RRC.  
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType processPdcchOrderQueueNode(
/* SPR 5599 changes end (ZIP ID 129195) */            
        UInt32 *availableCCE_p,
        DciCCEContainerInfo * dciCCEContainerInfo_p,
        DciCCEInfo * dciCCEInfo_p,
        UInt32 subFrameNum,
        PDCCHOrderNode *pdcchOrderNode_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick
        /* SPR 15909 fix end */
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p,
        /* 5492 changes end */
        InternalCellIndex internalCellIndex
       )
{
    DLUEContext *dlUEContext_p = PNULL;
    UInt32 allocatedCCEIndex = 0;
    DRXContext *drxContext_p = PNULL;
    MeasGapContextDL *measGapContext_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    UInt8 eCidReportReqFlag = TYPE_2_TA_REPORT_REQUIRED;
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p=PNULL;
    dlUEContext_p = dlUECtxInfoArr_g[pdcchOrderNode_p->ueIndex].dlUEContext_p;
    /* + E_CID_5.3 */
    ulUEContext_p = ulUECtxInfoArr_g[pdcchOrderNode_p->ueIndex].ulUEContext_p;
    /* - E_CID_5.3 */
    /*If UE is already in pending deletion state, do not send PDCCH
     * Order for that UE, remove it from queue and free the memory*/
    if(PNULL != dlUEContext_p &&
            (!(dlUECtxInfoArr_g[dlUEContext_p->ueIndex].pendingDeleteFlag)))
    {
        /* + E_CID_5.3 */
        if(PNULL != ulUEContext_p)
        {
            eCidReportReqFlag &= ulUEContext_p->eCidReportParams.typeofReportsRequired;
        }
        if(UL_SYNC_LOSS_OFF ==
                dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState && \
                (0 == eCidReportReqFlag))
        {
        /* - E_CID_5.3 */
            LOG_MAC_MSG(MAC_PDCCH_ORDER_PARAM,LOGDEBUG,MAC_PDCCH,\
                    getCurrentTick(),\
                    dlUEContext_p->ueIndex,\
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder,\
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,\
                    maxPdcchOrderperTTI_g[internalCellIndex],\
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                    "UL_SYNC_LOSS_OFF_FOR_UE");
            return MAC_SUCCESS;
        }

        dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx = pdcchOrderNode_p->preambleIndex;

        /* PDCCH Order will not be sent in case Meas Gap or DRX is ON 
           for a UE. In Strategy once DRX and MEas Gap are OFF, PDCCH Order 
           will be triggered again.*/
        drxContext_p = &dlUEContext_p->drxContext;
        measGapContext_p = &dlUEContext_p->measGapContext;

        if((DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p)) ||
                (TRUE == CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p)))
        {
            dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = TRUE;

            LOG_MAC_MSG(MAC_PDCCH_ORDER_PARAM,LOGDEBUG,MAC_PDCCH,\
                    getCurrentTick(),\
                    dlUEContext_p->ueIndex,\
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder,\
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,\
                    maxPdcchOrderperTTI_g[internalCellIndex],\
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,\
                    "DRX_OR_MEAS_GAP_PERIOD_ON");

            return MAC_SUCCESS;
        }
        /*pdcch changes start*/
        ueSearchSpaceInfo_p=getDLUEContextSearchSpaceInfo(dlUEContext_p,subFrameNum,internalCellIndex);
        if(!ueSearchSpaceInfo_p)
        {
            return MAC_FAILURE;
        }
        /*pdcch changes end*/
        /* SPR 6856 Changes start */
        /* Search PDCCH resources in UE and common search space */
        if (MAC_SUCCESS == allocatePdcchForPDCCHOrderMsg(
                            dlUEContext_p,
                            ueSearchSpaceInfo_p,
                            &allocatedCCEIndex,
                            availableCCE_p,
                            dciCCEInfo_p,
							dciCCEContainerInfo_p,
                            internalCellIndex))
        /* SPR 6856 Changes end */
        {
            /*Fill DCI 1A parameters in CCE container required by 
             * Transmitter to send the DCI_1A PDU to PHY*/

            /* + SPR 11491 Changes */
            if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
            {
                dlUEContext_p->isPdcchAllocatedFlag = 0;
            }
            else
            {
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag = 0;
            }
            /* - SPR 11491 Changes */
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.cceIndex
                = allocatedCCEIndex;
            dciCCEContainerInfo_p->arrayCCEAllocated[
                dciCCEContainerInfo_p->countOfCCEAllocated]
                = allocatedCCEIndex;
            dciCCEContainerInfo_p->countOfCCEAllocated++;
            /* 5492 changes start */
            /* 5492 changes end */

            dciCCEInfo_p[allocatedCCEIndex].msgType  = PDCCH_ORDER_MSG;

            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.rnti
                = dlUEContext_p->crnti;
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.numLayer 
                = MIN_DLSCH_NUM_LAYER;
            /* SPR 17112 Fix Start */
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.ueCategory
                = MAC_MIN_UE_CATEGORY;
            /* SPR 17112 Fix End */
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.harqProcessId
                = INVALID_HARQ_ID;
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.tb1Info.
                txIndicator = NEW_TX;
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.dciFormat
                = DCI_FORMAT_1A;
            /* SPR 17275 Fix Start */
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.pA = DEFAULT_PA_VALUE;
            /* SPR 17275 Fix End */
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.isTBOneValid = 0;
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.isTBTwoValid = 0;

            if(INVALID_PREAMBLE_IDX == pdcchOrderNode_p->preambleIndex)
            {
                dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.preambleIdx = 0;

                LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                        LOGWARNING,
                        MAC_PDCCH,
                        getCurrentTick(),
                        pdcchOrderNode_p->preambleIndex,
                        DEFAULT_INT_VALUE,
                        allocatedCCEIndex,
                        DEFAULT_INT_VALUE,
                        pdcchOrderNode_p->ueIndex,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "In DCI prambleidx as 0 is saved");

            }
            else
            {
                dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.preambleIdx
                    = pdcchOrderNode_p->preambleIndex;
                LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                        LOGWARNING,
                        MAC_PDCCH,
                        getCurrentTick(),
                        pdcchOrderNode_p->preambleIndex,
                        DEFAULT_INT_VALUE,
                        allocatedCCEIndex,
                        DEFAULT_INT_VALUE,
                        pdcchOrderNode_p->ueIndex,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "PreambleIdx stored in DCI");
            }

            /* CLPC_REWORK CHG */
            dciCCEInfo_p[allocatedCCEIndex].dciPduInfo.txPower =
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                commonDLPowerControlInfo.pdcchPowerOffset;
            /* CLPC_REWORK_CHG END */

            /* 5492 changes start */
            (*maxUeScheduledInDownlink_p)--;
            /* 5492 changes end */
            putEntryInPdcchOrderTimerTriggerQueue(pdcchOrderNode_p->ueIndex,
                    pdcchOrderNode_p->preambleIndex,
                    currentGlobalTick,
            /*+ SPR 8564*/
                    internalCellIndex);
            if (PNULL != ulUEContext_p)
            {
                ulUEContext_p->waitForMsg3BeforeResetting = TRUE;
            }
            /*- SPR 8564*/

        }
        else
        {
            return MAC_FAILURE;
        }
    } 

    return MAC_SUCCESS;
}    
/*****************************************************************************
 * Function Name :  checkUlSyncLossForUE
 * Inputs        :  dlUEContext_p - Pointer to DL UE Context for which PDCCH
 *                                  Order is to be triggered
 *                  currentGlobalTTITickCount -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs       :  None 
 * Returns       :  UL_SYNC_LOSS_ON/UL_SYNC_LOSS_OFF
 * Description   :  This function will check for Ul Sync Loss for a UE and   
 *                  trigger PDCCH Order and removal of UE from active list 
 *                  from DL and UL Scheduler.  
 ****************************************************************************/
/* SPR 15909 fix start */
/* spr 23717 fix + */
 UlSyncLossOnOff  checkUlSyncLossForUE(DLUEContext *dlUEContext_p, tickType_t currentGlobalTTITickCount,InternalCellIndex internalCellIndex)
/* spr 23717 fix - */
/* SPR 15909 fix end */
{
    /*SPR_3061*/
  /* CA_pahse2_ulSync_code Start */
    if (( IS_PCELL_OF_UE(dlUEContext_p , internalCellIndex)) &&
            /* CA_pahse2_ulSync_code End */
            (
             ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulSyncLossTimer != 
               UL_SYNC_TIMER_INFINITY) &&((getCurrentTick() - dlUEContext_p->lastSyncDetectedTick) >=
                   cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulSyncLossTimer))
             /*SPR 15208 Fix */
             ||
             ((TIME_ALIGNMENT_TIMER_INFINITY != dlUEContext_p->timeAlignmentTimer) &&
              ((getCurrentTick()- dlUEContext_p->lastTASentTick) > ((UInt32)dlUEContext_p->timeAlignmentTimer - UL_SYNC_TIME_TO_PROCESS_TA)))
            )   
       )
   /*SPR 15208 Fix */
             {
        /*SPR_3061*/
        /*UL Sync Loss has been detected for this UE. Put an entry in UL/DL
         * Trigger Queue to remove UE from active list and Put an entry in PDCCH
         * Order Queue to send PDCCH Order to a UE*/
        if(UL_SYNC_LOSS_ON !=
                dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState)
        {
            putEntryInUlSyncLossOnOffTriggerQ(dlUEContext_p->ueIndex, UL_SYNC_LOSS_ON,internalCellIndex);

            putEntryInRachDLStrategyQueue(dlUEContext_p->ueIndex, currentGlobalTTITickCount,internalCellIndex);

            dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
            dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = UL_SYNC_LOSS_ON;      
            /* + TM6_5.2 */
            if(TX_MODE_6 ==dlUEContext_p->dlMIMOInfo.transmissionMode) 
	    {
                /* + SPR 5953 Fix */ 
                dlUEContext_p->tm6DCIFormat = DCI_FORMAT_1A;
                if((((PERIODIC_CQI_REPORTING_MODE == dlUEContext_p->dlCQIInfo.cqiTypeIndicator) &&
                    (1 == dlUEContext_p->dlCQIInfo.cqiRequestType)) ||
                    (APERIODIC_CQI_REPORTING_MODE == dlUEContext_p->dlCQIInfo.cqiTypeIndicator) ||
                    (PERIODIC_APERIODIC_MODE == dlUEContext_p->dlCQIInfo.cqiTypeIndicator)) &&
                    (INVALID_CODE_BOOK_INDEX != dlUEContext_p->dlMIMOInfo.precodingIndex))
                /* - SPR 5953 Fix */ 
                {
                    dlUEContext_p->tm6DCIFormat = DCI_FORMAT_1B;
                    dlUEContext_p->dlCQIInfo.wideBandPMIIndex = 
                        dlUEContext_p->dlMIMOInfo.precodingIndex;
                }
	    }
            /* - TM6_5.2 */


            /*SPR_3065*/
            sendSyncIndToRRC(dlUEContext_p->ueIndex,
                    dlUEContext_p->crnti,OUT_OF_SYNC_INITIAL,
                    internalCellIndex);
            /*SPR_3065*/

            LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DETECTED,LOGWARNING,MAC_PDCCH,\
                    getCurrentTick(),\
                    dlUEContext_p->timeAlignmentTimer,\
                    dlUEContext_p->lastTASentTick,\
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,\
                    dlUEContext_p->ueIndex,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                    FUNCTION_NAME,"UL_SYNC_LOSS_ON detected for UE");
        }
        else
        {    
            LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DETECTED,LOGWARNING,MAC_PDCCH,\
                    getCurrentTick(),\
                    dlUEContext_p->timeAlignmentTimer,\
                    dlUEContext_p->lastTASentTick,\
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,\
                    dlUEContext_p->ueIndex,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                    FUNCTION_NAME,"UE IS ALREADY UL_SYNC_LOSS_ON");
        }
        return UL_SYNC_LOSS_ON;
    }
    return UL_SYNC_LOSS_OFF;
}

/*****************************************************************************
 * Function Name :  checkSyncLossTimerExpiry
 * Inputs        :  dlUEContext_p - Pointer to DL UE Context for which PDCCH
 *                                  Order is to be triggered
 * Outputs       :  None 
 * Returns       :  UL_SYNC_LOSS_ON/UL_SYNC_LOSS_OFF
 * Description   :  This function will check for Ul Sync Loss for a UE and   
 *                  trigger PDCCH Order and removal of UE from active list 
 *                 from DL and UL Scheduler.  
 *****************************************************************************/
/* spr 23717 fix + */
 UlSyncLossOnOff  checkSyncLossTimerExpiry(DLUEContext *dlUEContext_p)
/* spr 23717 fix - */
{
    if (dlUEContext_p)
    {
        UInt8 internalCellIndex = dlUEContext_p->internalCellIndex;

        if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->ulSyncLossTimer != UL_SYNC_TIMER_INFINITY) &&
                ((getCurrentTick() - dlUEContext_p->lastSyncDetectedTick) >=
                 cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->ulSyncLossTimer))
        {
            return UL_SYNC_LOSS_ON;
        }
    }
    return UL_SYNC_LOSS_OFF;
}

/*****************************************************************************
 * Function Name :  putEntryInUlSyncLossOnOffTriggerQ
 * Inputs        :  ueIndex  
 *                  onOffTrigger - Flag to set state as UL_SYNC_LOSS_ON or
 *                                  UL_SYNC_LOSS_OFF.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs       :  None
 * Returns       :  NOne
 * Description   :  This function will add a trigger to delete UE from  Active
 *                  List both from UL Scheduler.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
/* spr 23717 fix + */
 void putEntryInUlSyncLossOnOffTriggerQ(UInt32 ueIndex,
/* spr 23717 fix - */
/* SPR 5599 changes end (ZIP ID 129195) */            
        UlSyncLossOnOff onOffTrigger,
        InternalCellIndex internalCellIndex)
{
    ULSyncLossQueueNode *ulSyncLossUlQueueNode_p = PNULL;
    ULSyncLossQueueNode *ulSyncLossDlQueueNode_p = PNULL;

    GET_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossUlQueueNode_p,
            ULSyncLossQueueNode);
    /*+coverity-530-CID 24495 */
     if(PNULL == ulSyncLossUlQueueNode_p)
     {
         return;
     }
    /*+coverity-530-CID 24495 */
    GET_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossDlQueueNode_p,
            ULSyncLossQueueNode);

    /*+COVERITY 5.3.0 - 25476*/
    if ( PNULL == ulSyncLossDlQueueNode_p )
    {
        FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossUlQueueNode_p);
        return;
    }
    /*-COVERITY 5.3.0 - 25476*/
    /* Put an entry in Trigger Queue to delete an entry from UL Scheduler*/
    ulSyncLossUlQueueNode_p->ueIndex = ueIndex;
    ulSyncLossUlQueueNode_p->onOffTrigger = onOffTrigger;
    /*** Coverity 25572 Fix : Start ****/
    if (!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode,
                (void *)&(ulSyncLossUlQueueNode_p->anchorNode), internalCellIndex))
    {
        FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossUlQueueNode_p);
        freeMemPool(ulSyncLossUlQueueNode_p);
        return;
    }
    /*** Coverity 25572 Fix : End ****/

    /* Put an entry in Trigger Queue to delete an entry from DL Scheduler*/
    ulSyncLossDlQueueNode_p->ueIndex = ueIndex;
    ulSyncLossDlQueueNode_p->onOffTrigger = onOffTrigger;
    /*** Coverity 25572 Fix : Start ****/
    if (!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ULSyncLossQueueNode, &(ulSyncLossDlQueueNode_p->anchorNode),internalCellIndex))
    {
        freeMemPool(ulSyncLossDlQueueNode_p);
        return ;
    }
    /*** Coverity 25572 Fix : End ****/
}

/*****************************************************************************
 * Function Name :  putEntryInPdcchOrderQ 
 * Inputs        :  preambleIndex -
 *                  ueIndex- Index of the UE
 *                  currentGlobalTTI -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs       :  None
 * Returns       :  MAC_SUCCESS/MAC_FAILURE
 * Description   :  This function will push an entry into PDCCHOrder Queue
 *                  and add a trigger to delete UE from Active List both from
 *                  DL and UL Scheduler.
 *****************************************************************************/
/* SPR 15909 fix start */
/* +- SPR 18268 */
/* spr 23717 fix + */
 MacRetType putEntryInPdcchOrderQ(UInt8 preambleIndex, UInt16 ueIndex, tickType_t currentGlobalTTI, InternalCellIndex internalCellIndex)
/* spr 23717 fix - */
    /* +- SPR 18268 */
/* SPR 15909 fix end */    
{
    PDCCHOrderNode *pdcchOrderNode_p = PNULL;

    /* Put an entry in PDCCH Order Queue to send a PDCCH Order to UL Out of
     *        Sync UE */
     GET_MEM_NODE_PDCCH_ORDER_Q(pdcchOrderNode_p,PDCCHOrderNode);
     /*coverity-530-24493*/
     if(PNULL == pdcchOrderNode_p)
     {
        ltePanic(" Memory exhausted [%s] \n",__func__);
        return MAC_FAILURE;
     }
     /*coverity-530-24493*/
    pdcchOrderNode_p->preambleIndex = preambleIndex;
    pdcchOrderNode_p->ueIndex = ueIndex;
    pdcchOrderNode_p->currentGlobalTTI = currentGlobalTTI;
    if(ENQUEUE_PDCCH_ORDER_Q(PDCCHOrderNode, 
                (void *)&(pdcchOrderNode_p->anchorNode), internalCellIndex))
    {
        return MAC_SUCCESS;
    }
    else
    {
        FREE_MEM_NODE_PDCCH_ORDER_Q(pdcchOrderNode_p);
        return MAC_FAILURE;
    }
}


#ifdef TDD_CONFIG
/*********************************************************************************
 * Function Name   :  updateDai
 * Inputs          :  ueContext_p - pointer to UE context
 *                    currentGlobaTick - Current global tick
 *                    subframeWithDelay - Current subframe
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs         :  None
 * Returns         :  DAI value: V_ul_DAI According to Table 7.3-X of 36.213
 * Description     :  This function will update the DAI and 
 *                    returns the updated value
 *                    The DAI value will be increamented if the DAI for this DAI 
 *                    txn period is initialized already else DAI value will to 
 *                    initialized to ONE
 **********************************************************************************/
UInt8 updateDai(DLUEContext *ueContext_p,
        /* SPR 15909 fix start */
            tickType_t currentGlobaTick,
        /* SPR 15909 fix end */    
            UInt32 subframeWithDelay,
            InternalCellIndex internalCellIndex) 
{
    /** CA-TDD Changes Start:06 **/
    UInt8 dci0DaiIndex = tddDci0DaiSfIndex_p[internalCellIndex][subframeWithDelay];
    /** CA-TDD Changes End:06 **/
    /* + CA_TDD_HARQ_CHNAGES */
    UInt8 tempDai = 0;
    /* + SPS_TDD_Changes */
    UInt8 tempDaiSps = 0;
    /* - SPS_TDD_Changes */
    DLUESCellContext *dlUeScellContext_p = PNULL;
    dlUeScellContext_p  =  ueContext_p->dlUeScellContext_p[START_SCELL_INDEX];

    /*TDD Config 0 Changes Start*/
    /* In case of Tdd config 0 the value of Dai is 0*/
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign)
    {
        return 0; 
    }
    /*TDD Config 0 Changes End*/ 

    /* get the time/tick difference, from last PDSCH transmitted */
    if (IS_PCELL_OF_UE (ueContext_p, internalCellIndex))
    {
        tempDai = ueContext_p->tddDai[dci0DaiIndex]; 

        /* + SPS_TDD_Changes */
        tempDaiSps = ueContext_p->tddDaiSPS[dci0DaiIndex]; 
        /* - SPS_TDD_Changes */

        calculateDAI(currentGlobaTick,
                tddValidateDai_p[internalCellIndex][subframeWithDelay],
                ueContext_p->tddDaiLastScheduled[dci0DaiIndex],&tempDai
                /* + SPS_TDD_Changes */
                ,&tempDaiSps);
        /* - SPS_TDD_Changes */

        ueContext_p->tddDai[dci0DaiIndex] = tempDai;
        /* + SPS_TDD_Changes */
        ueContext_p->tddDaiSPS[dci0DaiIndex] = tempDaiSps;
        /* - SPS_TDD_Changes */
        ueContext_p->tddDaiLastScheduled[dci0DaiIndex] = currentGlobaTick;	

    }
    else if ((IS_CA_ENABLED()) && dlUeScellContext_p)
    {
        tempDai = dlUeScellContext_p->tddDaiOfSCell[dci0DaiIndex];

        calculateDAI(currentGlobaTick,
                tddValidateDai_p[internalCellIndex][subframeWithDelay],
                dlUeScellContext_p->tddDaiLastScheduledOfSCell[dci0DaiIndex]
                ,&tempDai,
                /* + SPS_TDD_Changes */
                &tempDaiSps);
        /* - SPS_TDD_Changes */

        dlUeScellContext_p->tddDaiOfSCell[dci0DaiIndex] = tempDai;
        dlUeScellContext_p->tddDaiLastScheduledOfSCell[dci0DaiIndex] = 
                                                      currentGlobaTick;	
    }    
   /* - CA_TDD_HARQ_CHNAGES */

    //fprintf(stderr,"tempDai [%d] ue crnti[%d]Tick [%llu]\n", tempDai, ueContext_p->crnti,getCurrentTick());

    /* Returning the DAI value as per 
     * Table 7.3-X: Value of Downlink Assignment Index of 36.213
     */
    tempDai = tempDai & 0x03;	
    /*BUG:10656:start*/
    /*BUG:10656:end*/
	tempDai= ((tempDai) ? tempDai : 0x04);
/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
    ueContext_p->nBundle[(currentGlobaTick / MAX_SUBFRAME + tddPdschTxMapOfDLHarq_p[internalCellIndex][subframeWithDelay].sfnFactor) % 2][tddPdschTxMapOfDLHarq_p[internalCellIndex][subframeWithDelay].dlAckNackSf] = tempDai;
    //fprintf(stderr," nBundle [%d] Tick [%llu]\n", ueContext_p->nBundle, getCurrentTick());
#endif
/* ULSCH_HARQ_issue_fix - */

    return tempDai;
}
/* + SPS_TDD_Changes */
/*********************************************************************************
 * Function Name   :  updateDaiSPS
 * Inputs          :  ueContext_p - pointer to UE context
 *                    currentGlobaTick - Current global tick
 *                    subframeWithDelay - Current subframe
 * Outputs         :  None
 * Returns         :  DAI value: V_ul_DAI According to Table 7.3-X of 36.213
 * Description     :  This function will update the DAI and 
 *                    returns the updated value
 *                    The DAI value will be increamented if the DAI for this DAI 
 *                    txn period is initialized already else DAI value will to 
 *                    initialized to ONE
 **********************************************************************************/
UInt8 updateDaiSPS(DLUEContext *ueContext_p,
		/* SPR 15909 fix start */
		tickType_t currentGlobaTick,
		/* SPR 15909 fix end */    
		UInt32 subframeWithDelay,
        InternalCellIndex internalCellIndex) 
{
    /* SPR 15909 fix start */
    tickType_t timeDiff = 0;
    /* SPR 15909 fix end */    
    /** CA-TDD Changes Start:06 **/
    UInt8 dci0DaiIndex = tddDci0DaiSfIndex_p[internalCellIndex][subframeWithDelay];
    /** CA-TDD Changes End:06 **/
    UInt8 tempDai;
    /* SPR 609 changes start */    
    /* SPR 15909 fix start */
    tickType_t tickRoundoff = 0xffffffff;
    /* SPR 15909 fix end */    
    /* SPR 609 changes start */    

    /*TDD Config 0 Changes Start*/
    /* In case of Tdd config 0 the value of Dai is 0*/
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign)
    {
        return 0; 
    }
    /*TDD Config 0 Changes End*/ 

    /* get the time/tick difference, from last PDSCH transmitted */
    if(ueContext_p->tddDaiLastScheduled[dci0DaiIndex] < currentGlobaTick)
    {
        timeDiff = currentGlobaTick - 
            ueContext_p->tddDaiLastScheduled[dci0DaiIndex];
    }
    else
    {
        /* SPR 609 changes start */    
        timeDiff = currentGlobaTick + tickRoundoff + 1 - 
            ueContext_p->tddDaiLastScheduled[dci0DaiIndex];
        /* SPR 609 changes end */    
    }

    /* If the last PDSCH transmission is for previous DAI txn, 
     * Reset DAI and Initialize to ONE
     * Else, Increment the DAI value
     */
    if(timeDiff > tddValidateDai_p[internalCellIndex][subframeWithDelay])
    {
        ueContext_p->tddDaiSPS[dci0DaiIndex] = 1;
        ueContext_p->tddDai[dci0DaiIndex] = 0;
    }
    else
    {
        ueContext_p->tddDaiSPS[dci0DaiIndex] += 1;
    }
    ueContext_p->tddDaiLastScheduled[dci0DaiIndex] = currentGlobaTick;	

    /* Returning the DAI value as per 
     * Table 7.3-X: Value of Downlink Assignment Index of 36.213
     */
    tempDai = ueContext_p->tddDaiSPS[dci0DaiIndex];

    tempDai = tempDai & 0x03;	
    return tempDai ? tempDai : 0x04;
}
/* - SPS_TDD_Changes */
/*********************************************************************************
 * Function Name   :  getDai
 * Inputs          :  ueContext_p - pointer to UE context
 *                    currentGlobaTick - Current global tick
 *                    subframeWithDelay - Current subframe
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs         :  None
 * Returns         :  DAI value: V_ul_DAI According to Table 7.3-X of 36.213
 * Description     :  This function will return the DAI value for current 
 *                    DCI_0 DAI Txn
 *                    The DAI value will be returned if the DAI for this DAI txn
 *                    period is initialized/updated else ZERO will be returned
 **********************************************************************************/
UInt8 getDai(UInt16 ueIndex,
        /* SPR 15909 fix start */
            tickType_t currentGlobaTick,
            /* SPR 15909 fix end */
            UInt8 subframeWithDelay,
            InternalCellIndex internalCellIndex) 
{
    /* SPR 15909 fix start */
    tickType_t timeDiff = 0;
    /* SPR 15909 fix end */
    /** CA-TDD Changes Start:05 **/
    UInt8 ulGrantSubFrame = getULGrantSubframeNum(subframeWithDelay, 
                              GET_ULGRANT_SFN_FOR_DWNLINK_DATA_AND_DCI0,
                              internalCellIndex);
    UInt8 dci0DaiIndex = tddDci0DaiSfIndex_p[internalCellIndex][ulGrantSubFrame];
    /** CA-TDD Changes End:05 **/
    UInt8 tempDai;
    /* SPR 609 changes start */    
    /* SPR 15909 fix start */
    tickType_t tickRoundoff = 0xffffffffffffffff;
    /* SPR 15909 fix end */
    /* SPR 609 changes end */    
    DLUEContext *ueContext_p = PNULL;
    DLUESCellContext *dlUeScellContext_p = PNULL;

    ueContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    dlUeScellContext_p =  ueContext_p->dlUeScellContext_p[START_SCELL_INDEX];
    /*TDD Config 0 Changes Start*/
    /* In case of Tdd config 0 the value of Dai is 0*/
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.\
           cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign)
    {
        tempDai = 0;
        return tempDai;
    }
    /*TDD Config 0 Changes End*/ 

    /* get the time/tick difference, from last PDSCH transmitted */
    if(ueContext_p->tddDaiLastScheduled[dci0DaiIndex] < currentGlobaTick)
    {
        timeDiff = currentGlobaTick - ueContext_p->tddDaiLastScheduled[dci0DaiIndex];
    }
    else
    {
        /* SPR 609 changes start */    
        timeDiff = currentGlobaTick + tickRoundoff + 1 - 
                   ueContext_p->tddDaiLastScheduled[dci0DaiIndex];
        /* SPR 609 changes end */    
    }

    /* If there is any PDSCH transmission in current DAI txn period, 
     * return the DAI value
     * Else, return ZERO
     */
     /** CA-TDD Changes Start **/
    if(timeDiff <= tddValidateDai_p[internalCellIndex][ulGrantSubFrame])
    /** CA-TDD Changes End **/
    {
        /* + SPS_TDD_Changes */
        tempDai = ueContext_p->tddDai[dci0DaiIndex] + ueContext_p->tddDaiSPS[dci0DaiIndex];
        /* - SPS_TDD_Changes */
    }
    else
    {
        tempDai = 0;
    }

    /* If CA is enable and Scell of UE exists then add SCell DAI */
    if ((IS_CA_ENABLED()) && dlUeScellContext_p)
    {   
        /* get the time/tick difference, from last PDSCH transmitted */
        if(dlUeScellContext_p->tddDaiLastScheduledOfSCell[dci0DaiIndex] <
                                                         currentGlobaTick)
        {
            timeDiff = currentGlobaTick -
                         dlUeScellContext_p->tddDaiLastScheduledOfSCell[dci0DaiIndex];
        }
        else
        {
            /* SPR 609 changes start */    
            timeDiff = currentGlobaTick + tickRoundoff + 1 - 
                       dlUeScellContext_p->tddDaiLastScheduledOfSCell[dci0DaiIndex]; 
            /* SPR 609 changes end */    
        }

        /** CA-TDD Changes Start **/
        if(timeDiff <= tddValidateDai_p[internalCellIndex][ulGrantSubFrame])
            /** CA-TDD Changes End **/
        {
            tempDai += dlUeScellContext_p->tddDaiOfSCell[dci0DaiIndex];
        }
    }
    /* Returning the DAI value as per 
     * Table 7.3-X: Value of Downlink Assignment Index of 36.213
     */

    tempDai = tempDai & 0x03;	
    return tempDai ? tempDai : 0x04;
}
#endif

/******************************************************************************
 * Function Name   :  processScheduledFormat3Or3AQueue
 * Inputs          :  maxCommonSpaceCCE - max CCEs thats are available in 
 *                       common control region for the allocation to DCI 
 *                       Format 3/3A.
 *                    pdcchDLOutputInfo_p -
 *                    pdcchTpcMsgInfoForPusch_p -
 *                    dciCCEContainerInfo - DCI CCE container info.
 *                    availableCCE_p - CCEs that are available in the system.
 *                    currentGlobalTick -
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs         :  None
 * Returns         :  None
 * Description     :  This function processes scheduleFormat3Or3AQueue_g to 
 *                    schedule DCI Format 3/3A. It shall allocate CCEs to the 
 *                    TPC-RNTI in common search space and fills the information
 *                    required by transmitter to send the DL DCI. This function
 *                    shall be called after allocatePdcchCommonChannel().
 *****************************************************************************/
void processScheduledFormat3Or3AQueue (
        UInt32 *maxCommonSpaceCCE, 
        PdcchDLOutputInfo * pdcchDLOutputInfo_p,
        PdcchTpcMsgInfoForPusch * pdcchTpcMsgInfoForPusch_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p, 
        UInt32 *availableCCE_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    /* We are first processing the scheduledFormat3Or3AQueueForPusch_g and then
       process the scheduledFormat3Or3AQueueForPucch_g */

    UInt8 queueCount = 0;
    UInt8 aggregationLvl = 0;
    UInt32 cceAllocated = INVALID_CCE_INDEX;
    MacRetType macRetType = MAC_FAILURE;
    ScheduledFormat3Or3AQueueNode * scheduledFormat3Or3AQueueNode_p = PNULL;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    DciCCEInfo * dciCCEInfo_p = PNULL;
    /* SPR 23393 Changes Start */
#ifdef TDD_CONFIG    
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
#endif
    /* SPR 23393 Changes End */
    /* +- SPR 22354 */
    UInt32 sysFrameNum 			= 0;
    UInt32 subFrameNumTemp 			= 0;
    UInt8  absSubFrameNum 		= 0;
    UInt32 numBitsAbsPattern = 0;
    UInt32 ueTypeToSchedule[MAX_NUM_CELL] = {0};
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {

    getSFAndSFN(&sysFrameNum, &subFrameNumTemp,internalCellIndex);

    /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNumTemp = subFrameNumTemp + PHY_DL_DELAY;

    if(subFrameNumTemp >= MAX_SUB_FRAME)
    {
        subFrameNumTemp -= MAX_SUB_FRAME;
        sysFrameNum++;
        if (sysFrameNum == MAX_SFN_VALUE)
        {
            sysFrameNum = 0;
        }
    }
#ifdef FDD_CONFIG
       numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
   {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
   {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
   }
   else
   {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
   }
#endif

    absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNumTemp)) %numBitsAbsPattern) ;
	    if ((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (isEicicEnabled_g[internalCellIndex])) /* Work as victim*/
  {
      ueTypeToSchedule[internalCellIndex] = VICTIM_UE;

  }
  else
  {
    	ueTypeToSchedule[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
  }
    }
    else
    {
	    ueTypeToSchedule[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
    }
    /* EICIC Optimization +- */
    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    pdcchDLOutputInfo_p->countTpcMsg = 0;
    pdcchTpcMsgInfoForPusch_p->count = 0;

    /* Get the count of the scheduledFormat3Or3AQueueForPusch_g */
    queueCount = sQueueCount(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);

    /* Loop through the nodes of the queue */
    while( (queueCount--) && *maxCommonSpaceCCE)
    {
        /* Get the first node from the queue */
        scheduledFormat3Or3AQueueNode_p = 
            (ScheduledFormat3Or3AQueueNode*)\
            getFirstNode(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);
        /* Get the tpcRntiContext using the tpcRntiIndex extracted from the 
           node popped from the queue */
        /* Coverity 60251 Fix */
        if(scheduledFormat3Or3AQueueNode_p)
        {
            tpcRntiContext_p =
                (tpcPuschRntiContext_gp[internalCellIndex] + (scheduledFormat3Or3AQueueNode_p->\
                tpcRntiIndex));
            if(tpcRntiContext_p)/*CID 111427*/
            {
                /* + SPR 22354 */
            if ((ueTypeToSchedule[internalCellIndex] == NON_VICTIM_UE) &&
                        ( MAC_SUCCESS == macCheckIfAnyVictimUe( tpcRntiContext_p ) ) )
	    	{
                    /* Victim UE shall be scheduled in next non-Victim tick, hence
                     * pushing this node at the end of queue */
                    /* - SPR 22354 */
   scheduledFormat3Or3AQueueNode_p =
                        (ScheduledFormat3Or3AQueueNode*)popNode(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);
 
            	/* + Coverity 71108 */
	            if ( pushNode(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex], 
                            	&(scheduledFormat3Or3AQueueNode_p->nodeAnchor)))
				{
				   	lteWarning (" pushNode() failed for scheduledFormat3Or3AQueue ");
				   	freeMemPool(scheduledFormat3Or3AQueueNode_p);		
				}	
            	/* - Coverity 71108 */

				continue;
	    	}
                /* Check if triggered count in the tpc rnti context is valid or not. 
                   Otherwise there is no need to send the DCI PDU */
                if(tpcRntiContext_p->triggeredCount)
                {
                    aggregationLvl = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
                    /* SPR 6856 Changes start */
                    macRetType= allocatePDCCHForDCIFormatCommonSpace (
                            aggregationLvl,&cceAllocated, 
                            availableCCE_p, dciCCEInfo_p,
                            internalCellIndex);
                    /* SPR 6856 Changes end */

                    /* Check if CCE allocation is successful or not */
                    if (macRetType == MAC_SUCCESS)
                    {
                        dciCCEContainerInfo_p->\
                            arrayCCEAllocated[dciCCEContainerInfo_p->\
                            countOfCCEAllocated] = cceAllocated;
                        dciCCEInfo_p[cceAllocated].msgType = TPC_MSG;
                        dciCCEInfo_p[cceAllocated].dciPduInfo.dciFormat = 
                            tpcRntiContext_p->isConfiguredForDciFormat3Or3A;
                        dciCCEContainerInfo_p->countOfCCEAllocated++;
                        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti = 
                            tpcRntiContext_p->tpcRnti;
                        /* SPR 6855 Changes start */
                        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
                        /* SPR 6855 Changes end */

                        /* ULOutputInfo to be used by PUSCH to encode the bitmap */
                        pdcchTpcMsgInfoForPusch_p->tpcMsgInfoForPusch[\
                            pdcchTpcMsgInfoForPusch_p->count].dciFormat = 
                            dciCCEInfo_p[cceAllocated].dciPduInfo.dciFormat;

                        pdcchTpcMsgInfoForPusch_p->tpcMsgInfoForPusch[\
                            pdcchTpcMsgInfoForPusch_p->count].cceIndex = 
                            cceAllocated;

                        pdcchTpcMsgInfoForPusch_p->tpcMsgInfoForPusch[\
                            pdcchTpcMsgInfoForPusch_p->count].tpcRntiIndex = 
                            tpcRntiContext_p->tpcRntiIndex;

                        pdcchTpcMsgInfoForPusch_p->count++;

                        /* Update the applied UL tick in tpcRntiContext to help in 
                           detecting collision with DCI Format 0 */
#ifdef FDD_CONFIG                    
                        tpcRntiContext_p->appliedUlTick = 
                            currentGlobalTick + FDD_HARQ_OFFSET;
#elif TDD_CONFIG
                        tpcRntiContext_p->appliedUlTick =
                            currentGlobalTick + 
                            getExactULSubFrame(subFrameNum, internalCellIndex);
#endif
                        /* Update the maxCommonSpaceCCE_p are left */
                        *maxCommonSpaceCCE -= aggregationLvl;

                        LOG_MAC_MSG (MAC_SCHEDULED_DCI3OR3A_PUSCH_ID,\
                                LOGDEBUG, MAC_UL_PC,\
                                currentGlobalTick,\
                                tpcRntiContext_p->tpcRntiIndex,\
                                cceAllocated,\
                                dciCCEInfo_p[cceAllocated].dciPduInfo.dciFormat,\
                                tpcRntiContext_p->triggeredCount,\
                                tpcRntiContext_p->appliedUlTick,\
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                                __func__, "SCH_DCI3OR3A_PUSCH" );
                    }
                    else
                    {
                        /* CCEs are not available. Hence break from the loop */
                        break;
                    }
                } /* tpcRntiContext_p->triggeredCount > 0 */
                else
                {
                    tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
                }
            }
        }

        /* Pop the node from the queue and free the mempool */
        scheduledFormat3Or3AQueueNode_p = 
            (ScheduledFormat3Or3AQueueNode*)\
            popNode(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);

        /* +COVERITY 10146 */
        if (scheduledFormat3Or3AQueueNode_p)
        {
            freeMemPool(scheduledFormat3Or3AQueueNode_p);
        }
        /* -COVERITY 10146 */
    }/* scheduledFormat3Or3AQueueForPusch_g */

    /* Get the count of the scheduledFormat3Or3AQueueForPucch_g */
    queueCount = sQueueCount(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);

    /* Loop through the nodes of the queue */
    while( (queueCount--) && *maxCommonSpaceCCE)
    {
        /* Get the first node from the queue */
        scheduledFormat3Or3AQueueNode_p = 
            (ScheduledFormat3Or3AQueueNode*)\
            getFirstNode(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);

        /* Get the tpcRntiContext using the tpcRntiIndex extracted from the node 
           popped from the queue */
        /* Coverity 60251 Fix Start */
        if(scheduledFormat3Or3AQueueNode_p)
        /* Coverity 60251 Fix End */
        {
            tpcRntiContext_p =
                (tpcPucchRntiContext_gp[internalCellIndex] + scheduledFormat3Or3AQueueNode_p->tpcRntiIndex);

	    /* Coverity 60495 Fix for Null check*/
	    if(tpcRntiContext_p)
	    {
                /* + SPR 22354 */
		    if ((ueTypeToSchedule[internalCellIndex] == NON_VICTIM_UE) && 
                        ( MAC_SUCCESS == macCheckIfAnyVictimUe( tpcRntiContext_p ) ) )
		    {
                    /* Victim UE shall be scheduled in next non-Victim tick, hence
                     * pushing this node at the end of queue */
                    /* - SPR 22354 */
			    scheduledFormat3Or3AQueueNode_p =
				    (ScheduledFormat3Or3AQueueNode*)popNode(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);    
			    /* EICIC_Coverity CID 71108 Start */
			    if(pushNode(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex], 
					    	&(scheduledFormat3Or3AQueueNode_p->nodeAnchor)))
			    {
				   	lteWarning ("EICIC : pushNode() failed for processScheduledFormat3Or3AQueue ");
				   	freeMemPool(scheduledFormat3Or3AQueueNode_p);		
			    }

			    /* EICIC_Coverity CID 71108 End */     

			    continue;

		    }		

/* EICIC -*/

		    /* Check if triggered count in the tpc rnti context is valid or not. 
		       Otherwise there is no need to send the DCI PDU */
		    if((tpcRntiContext_p->triggeredCount) &&  
				    (pdcchDLOutputInfo_p->countTpcMsg < MAX_TPC_MSG_PER_TICK ) )
		    {
			    aggregationLvl = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
			    /* SPR 6856 Changes start */
			    macRetType= allocatePDCCHForDCIFormatCommonSpace (
					    aggregationLvl,&cceAllocated, 
					    availableCCE_p, dciCCEInfo_p,
					    internalCellIndex);
			    /* SPR 6856 Changes end */

			    /* Check if CCE allocation is successful or not */
			    if (macRetType == MAC_SUCCESS)
			    {
				    dciCCEContainerInfo_p->\
					    arrayCCEAllocated[dciCCEContainerInfo_p->\
					    countOfCCEAllocated] = cceAllocated;
				    dciCCEInfo_p[cceAllocated].msgType = TPC_MSG;
				    dciCCEInfo_p[cceAllocated].dciPduInfo.dciFormat = 
					    tpcRntiContext_p->isConfiguredForDciFormat3Or3A;
				    dciCCEContainerInfo_p->countOfCCEAllocated++;
				    dciCCEInfo_p[cceAllocated].dciPduInfo.rnti = 
					    tpcRntiContext_p->tpcRnti;
				    /* SPR 6855 Changes start */
				    dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
					    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
					    cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
				    /* SPR 6855 Changes end */

				    /* DLOutputInfo to be used by PUCCH to encode the bitmap */
				    pdcchDLOutputInfo_p->tpcMsgInfoForPucch[\
					    pdcchDLOutputInfo_p->countTpcMsg].dciFormat = 
					    dciCCEInfo_p[cceAllocated].dciPduInfo.dciFormat;

				    pdcchDLOutputInfo_p->tpcMsgInfoForPucch[\
					    pdcchDLOutputInfo_p->countTpcMsg].cceIndex =
					    cceAllocated;

				    pdcchDLOutputInfo_p->tpcMsgInfoForPucch[\
					    pdcchDLOutputInfo_p->countTpcMsg].tpcRntiIndex =
					    tpcRntiContext_p->tpcRntiIndex;

				    pdcchDLOutputInfo_p->countTpcMsg++;

				    /* Update the applied UL tick in tpcRntiContext to help in 
				       detecting collision with DCI Format 1x/2x */
#ifdef FDD_CONFIG                    
				    tpcRntiContext_p->appliedUlTick = 
					    currentGlobalTick + FDD_HARQ_OFFSET + PHY_UL_CONTROL_DELAY;
#elif TDD_CONFIG
				    tpcRntiContext_p->appliedUlTick =
					    currentGlobalTick + 
					    /* SPR 11700 Start */
					    getDlAckNackOffset(subFrameNum,internalCellIndex) + PHY_UL_CONTROL_DELAY;
				    /* SPR 11700 End */
#endif
				    /* Update the maxCommonSpaceCCE_p are left */
				    *maxCommonSpaceCCE -= aggregationLvl;
			    }
			    else
			    {
				    /* CCEs are not available. Hence break from the loop */
				    break;
			    }
		    } /* tpcRntiContext_p->triggeredCount > 0 */
		    else
		    {
			    tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
		    }
	    }
	}
	/* Pop the node from the queue and free the mempool */
	scheduledFormat3Or3AQueueNode_p = 
		(ScheduledFormat3Or3AQueueNode*)\
		popNode(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);
	/*SPR 4017 PH2 Start*/
	if(scheduledFormat3Or3AQueueNode_p)
        {
            freeMemPool(scheduledFormat3Or3AQueueNode_p);
        }
        /*SPR 4017 PH2 End*/
    }/* scheduledFormat3Or3AQueueForPusch_g */
}/* processScheduledFormat3Or3AQueue */

/******************************************************************************
 * Function Name : allocatePDCCHForULNewReTx
 * Inputs        : ueSearchSpaceInfo_p - This is pointer to UESpecificSearchSpaceInfo 
 *                    structure which holds the CCEs Index for a UE at each of the 
 *                    aggregation level
 *                 ulUEContext_p -
 *                 txNode_p -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : ceAllocated_p - This holds the value of CCE Index which is
 *                    allocated.
 *                 availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by  one.
 *                 dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 * Returns       : MacRetType
 * Description   : This funtions will find the aggregation level for the DCI 
 *                 Format 0 and based on aggregation level it call the relevant
 *                 funtions for finding the free CCE 
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129195) */            
MacRetType allocatePDCCHForULNewReTx(
/* SPR 5599 changes end (ZIP ID 129195) */            
            UESpecificSearchSpaceInfo*ueSearchSpaceInfo_p,
            UInt32 *cceAllocated_p,
            UInt32 *availableCCE_p, 
            DciCCEInfo * dciCCEInfo_p,
            ULUEContext * ulUEContext_p,
            ULStrategyTxNode  *txNode_p, //CA_phase2_csi_code
            InternalCellIndex internalCellIndex)
{
    UInt32 aggrLvlCount = 0;
    UInt32 dciSize = 0;
    UInt32 dciSizeCategory = 0;
    UInt32 cqiValue = 0;
    UInt32 localCount = 0;
    UInt32 powerOffset = 0;
    UInt32 aggregationLevel = INVALID_AGGREGATION_LEVEL;
    DCIFormat  dciFormat = MAX_NUM_DCI_FORMAT;
    AggregationPowerOffset *aggregationLevelPowerOffset_p = PNULL;
    MacRetType retVal = MAC_FAILURE;
    UInt8 commonSpaceChk = FALSE, count=0; /* SPR 6858 changes start */
    CellConfigParams *cellParams_p = \
                                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* CA_phase2_csi_code Start */
    UInt8 tempBitmaskForAperiodicCqi = 0;
    /* CA_phase2_csi_code End */
    /*coverity 69351 +-*/
    if (cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable && 
            ulUEContext_p->isDLWideBandCQIReportAvailable && 
            (ulUEContext_p->dlWidebandCQICodeWordOne != INVALID_CQI_VALUE))
    {
    
    
        /* SPR 3256 START */
            if ((ulUEContext_p->dlWidebandCQICodeWordTwo != INVALID_CQI_VALUE)
                && (ulUEContext_p->dlWidebandCQICodeWordOne >=
                    ulUEContext_p->dlWidebandCQICodeWordTwo)) 
            /* SPR 3256 END */
           { 
                cqiValue = ulUEContext_p->dlWidebandCQICodeWordTwo;
           }
           else
           {
                cqiValue = ulUEContext_p->dlWidebandCQICodeWordOne;
            }
           
		   	


        dciFormat = DCI_FORMAT_0;

        /*Wideband CQI is available, aggregation level will be decided using
         * DCISize*/
        dciSize =
            getDCIBitSize( dciFormat, 
                    cellParams_p->numOfTxAnteenas, 
                    cellParams_p->dlAvailableRBs);

        /*Now find if this DCI size lies in big/small/medium DCI size category.
         * This result will be used to find the aggregation level*/
        dciSizeCategory = getDCISizeCategory(dciSize);
        /* ICIC changes start*/
        aggregationLevelPowerOffset_p = 
            &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->pdcchAggregationAndPowerOffset\
                    [ulUEContext_p->userLocationType].aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);
        /* ICIC changes end */
        /*The static power control table returned valid values.*/
        aggrLvlCount = aggregationLevelPowerOffset_p->count;

        while (localCount < aggrLvlCount)
        {
            aggregationLevel = aggregationLevelPowerOffset_p->
                aggregationPowerOffsetTuples[localCount].aggregationLevel;
            powerOffset = aggregationLevelPowerOffset_p->
                aggregationPowerOffsetTuples[localCount].powerOffset;


            localCount++;

            retVal = allocatePDCCHForDCIFormatUESpace(
                    PNULL,
                    ueSearchSpaceInfo_p,
                    cceAllocated_p,
                    availableCCE_p,
                    dciCCEInfo_p,
                    aggregationLevel,
                    PNULL,
                    internalCellIndex);

            if (retVal == MAC_SUCCESS)
            {
                /* CA_phase2_csi_code start */
                /* According to 36.212- 5.3.3.1.1 The 2-bit field applies to UEs
                 * that are configured with more than one DL cell and when the
                 * corresponding DCI format is mapped onto the UE specific
                 * search space given by the C-RNTI  otherwise
                 * the 1-bit field applies */
                if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
                {
                    /* Scell is configured */
                    /* + SPR_14047 */
                    dciCCEInfo_p[*cceAllocated_p].dciPduInfo.csiFieldSize = 1;
                    /* - SPR_14047 */
                }
                else
                {
                    /* + SPR_14047 */
                    dciCCEInfo_p[*cceAllocated_p].dciPduInfo.csiFieldSize = 0;
                    /* - SPR_14047 */
                }
                if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL && 
                        ((ulUEContext_p->aperiodicCQIToBeRequested) & 
                         servingCellBitMask_g[START_SCELL_INDEX]))
                {
                    if(!IS_SCELL_ACTIVE(ulUEContext_p->ueIndex,START_SCELL_INDEX))
                    {
                        if(txNode_p != PNULL)
                        {
                            /* update minRB and  data size in
                             * txNode */
                            txNode_p->minDataSize -= ulUEContext_p->
                                ulUeScellContext_p[START_SCELL_INDEX]->
                                minDataSize;
                            txNode_p->maxDataSize -= ulUEContext_p->
                                ulUeScellContext_p[START_SCELL_INDEX]->
                                minDataSize;
                            txNode_p->requiredRB -= ulUEContext_p->
                                ulUeScellContext_p[START_SCELL_INDEX]->
                                minRBs;
                            /*Updation of bufferpool in case of required
                             * RBs is reduced */
#ifdef FDD_CONFIG
                            ulBufferPool_g[internalCellIndex].totalRBs += 
                                ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                            if (CE_USER == ulUEContext_p->userLocationType)
                            {
                                ulBufferPool_g[internalCellIndex].cellEdgeRBs += 
                                    ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                            }
#endif
                            ulUEContext_p->aperiodicCQIToBeRequested = 
                                ulUEContext_p->aperiodicCQIToBeRequested &
                                ~servingCellBitMask_g[START_SCELL_INDEX];
                        }
                    }
                }

                /* CA_phase2_csi_code end */
                break;
            }
            else
            {
                /* SPR 6858 Changes start */
                /* if common space is not checked alrady otw not required to chk again */
                if (!commonSpaceChk)
                {

                    /* CA_phase2_csi_code Start */
                    if(txNode_p != PNULL)
                    {
                        if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
                        {
                            if((ulUEContext_p->aperiodicCQIToBeRequested ==
                                    servingCellBitMask_g[START_SCELL_INDEX]) &&
                                        (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == 
                                         txNode_p->ulGrantRequestType))
                            {
                                /* case when report for only Scell is
                                 * expected so DCI0 cannot be scheduled in 
                                 * common search space.*/
                                ulUEContext_p->aperiodicCQIToBeRequested = 
                                    ulUEContext_p->aperiodicCQIToBeRequested & 
                                    ~servingCellBitMask_g[START_SCELL_INDEX];

                                return MAC_FAILURE;
                            }
                        }
                    }
                    /* CA_phase2_csi_code End */
                    /*START : SPR5381*/
                    retVal = isCCEAvailableAggrLvlFourCommonSpace(
                            cceAllocated_p,
                            availableCCE_p,dciCCEInfo_p,
                            internalCellIndex);
                    if(MAC_SUCCESS == retVal) 
                    {
                        powerOffset = 0;
                        if (aggrLvlCount)
                        {
                            for (count=0; count< aggrLvlCount; count++)
                            {
                                if (AGGREGATION_LEVEL_FOUR == 
                                        aggregationLevelPowerOffset_p->aggregationPowerOffsetTuples[count].aggregationLevel)
                                {
                                    powerOffset = aggregationLevelPowerOffset_p->aggregationPowerOffsetTuples[count].powerOffset;
                                }
                            }
                        }
                        /* SPR 6858 Changes end */
                        /* CA_phase2_csi_code Start */
                        if(txNode_p != PNULL)
                        {
                            if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
                            {
                                /* Case when Aperiodic Report requested for both PCell
                                 * and Scell, so dropping report for Scell */
                                if((ulUEContext_p->aperiodicCQIToBeRequested)
                                        & servingCellBitMask_g[START_SCELL_INDEX])
                                {
                                    /* 1- update variable
                                     * aperiodicCQIToBeRequested
                                     * 2- put entry in PUSCH failure queue
                                     * 3- update RBs and dataSize in txNode*/
                                    tempBitmaskForAperiodicCqi = ulUEContext_p->aperiodicCQIToBeRequested;
                                    /* temporary assigning value of cell index
                                     * whose cqi is to be dropped  in
                                     * ulUEContext_p->aperiodicCQIToBeRequested
                                     */
                                    ulUEContext_p->aperiodicCQIToBeRequested = 
                                        servingCellBitMask_g[START_SCELL_INDEX];
                                    putEntryInPUSCHFailureQueue(txNode_p->ueIndex,
                                            txNode_p->ttiCounter,
                                            txNode_p->ulGrantRequestType,
                                            internalCellIndex);
                                    ulUEContext_p->aperiodicCQIToBeRequested = tempBitmaskForAperiodicCqi;

                                    /* update minRB and  data size in
                                     * txNode */
                                    txNode_p->minDataSize -= ulUEContext_p->
                                        ulUeScellContext_p[START_SCELL_INDEX]->minDataSize;
                                    txNode_p->maxDataSize -= 
                                        ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
                                        minDataSize;
                                    txNode_p->requiredRB -= 
                                        ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
                                        minRBs;
                                    /*Updation of bufferpool in case of required
                                     * RBs is reduced */
#ifdef FDD_CONFIG
                                    ulBufferPool_g[internalCellIndex].totalRBs += 
                                        ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                                    if (CE_USER == ulUEContext_p->userLocationType)
                                    {
                                        ulBufferPool_g[internalCellIndex].cellEdgeRBs += 
                                            ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                                    }
#endif
                                    if(ulUEContext_p->aperiodicCQIToBeRequested == 
                                            servingCellBitMask_g[START_SCELL_INDEX])
                                    {
                                        /* case when report for only scell was
                                         * expected and grant type is  CQI_WITH_DATA so
                                         * we have dropped the cqi so changing
                                         * grant type to NEW_UL_GRANT for
                                         * processing in PUSCH */
                                         txNode_p->ulGrantRequestType = 
                                             NEW_ULGRANT_REQ_MSG;
                                    }
                                    ulUEContext_p->aperiodicCQIToBeRequested = 
                                        ulUEContext_p->aperiodicCQIToBeRequested &
                                        ~servingCellBitMask_g[START_SCELL_INDEX];
                                }
                            }
                        }
                        /* If cce allocated from common search space, csi field size
                         * will be 1 bit in case of Rel 10 UE */
                         /* + SPR_14047 */
                        dciCCEInfo_p[*cceAllocated_p].dciPduInfo.csiFieldSize = 0;
                         /* - SPR_14047 */
                        /* CA_phase2_csi_code End */
                        break;
                    }
                    else{
                        LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                                getCurrentTick(),\
                                ulUEContext_p->ueIndex,\
                                DCI_FORMAT_0,
                                aggregationLevel,
                                cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable,\
                                ulUEContext_p->isDLWideBandCQIReportAvailable,\
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                __func__, "PDCCH_FAIL");
                        return retVal; /* SPR 6856 changes start */
                    }
                    /*END   : SPR5381*/
                }
            }
        }
    }
    else
    {
        /* ICIC changes start */
        aggregationLevel = dciFormatToAggrLvlMapping_g[internalCellIndex].\
                           dciFormatToAggregationLevel\
                           [ulUEContext_p->userLocationType][DCI_FORMAT_0];
        /* ICIC changes end */

        powerOffset = 0;
/*SPR 21001 Changes start */
        retVal = allocatePDCCHForDCIFormatUESpace(
                PNULL,
                ueSearchSpaceInfo_p,
                cceAllocated_p,
                availableCCE_p,
                dciCCEInfo_p,
                aggregationLevel,
                PNULL,
                internalCellIndex);
/*SPR 21001 Changes end */
        if ( retVal == MAC_FAILURE)
        {
            /* CA_phase2_csi_code Start */
            if(txNode_p != PNULL)
            {
                if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
                {
                    if((ulUEContext_p->aperiodicCQIToBeRequested ==
                                servingCellBitMask_g[START_SCELL_INDEX]) &&
                            (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == 
                             txNode_p->ulGrantRequestType))
                    {
                        /* case when report for only Scell is
                         * expected so DCI0 cannot be scheduled in 
                         * common search space.*/
                        ulUEContext_p->aperiodicCQIToBeRequested = 
                            ulUEContext_p->aperiodicCQIToBeRequested & 
                            ~servingCellBitMask_g[START_SCELL_INDEX];

                        return MAC_FAILURE;
                    }
                }
            }
            /* CA_phase2_csi_code End */
            /*START : SPR5381*/
            retVal = isCCEAvailableAggrLvlFourCommonSpace(
                    cceAllocated_p,
                    availableCCE_p,dciCCEInfo_p,
                    internalCellIndex);
            if(MAC_FAILURE == retVal)
            {
                LOG_MAC_MSG (MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
                        getCurrentTick(),\
                        ulUEContext_p->ueIndex,\
                        DCI_FORMAT_0,
                        aggregationLevel,
                        cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable,\
                        ulUEContext_p->isDLWideBandCQIReportAvailable,\
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__, "PDCCH_FAIL");

                return retVal; /* SPR 6856 changes start */
            }
            /*END   : SPR5381*/
            /* CA_phase2_csi_code Start */ 
            else
            {
                if(txNode_p != PNULL)
                {
                    if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
                    {
                        /* Case when Aperiodic Report requested for both PCell
                         * and Scell, so dropping report for Scell */
                        if((ulUEContext_p->aperiodicCQIToBeRequested)
                                & servingCellBitMask_g[START_SCELL_INDEX])
                        {
                            /* 1- update variable
                             * aperiodicCQIToBeRequested
                             * 2- put entry in PUSCH failure queue
                             * 3- update RBs and dataSize in txNode*/
                            tempBitmaskForAperiodicCqi = ulUEContext_p->aperiodicCQIToBeRequested;
                            ulUEContext_p->aperiodicCQIToBeRequested = 
                                servingCellBitMask_g[START_SCELL_INDEX];
                            putEntryInPUSCHFailureQueue(txNode_p->ueIndex,
                                    txNode_p->ttiCounter,
                                    txNode_p->ulGrantRequestType,
                                    internalCellIndex);
                            ulUEContext_p->aperiodicCQIToBeRequested = tempBitmaskForAperiodicCqi;
                            /* update minRB and  data size in
                             * txNode */
                            txNode_p->minDataSize -= ulUEContext_p->
                                ulUeScellContext_p[START_SCELL_INDEX]->
                                minDataSize;
                            txNode_p->maxDataSize -= ulUEContext_p->
                                ulUeScellContext_p[START_SCELL_INDEX]->
                                minDataSize;
                            txNode_p->requiredRB -= ulUEContext_p->
                                ulUeScellContext_p[START_SCELL_INDEX]->
                                minRBs;
                            /*Updation of bufferpool in case of required
                             * RBs is reduced */
#ifdef FDD_CONFIG
                            ulBufferPool_g[internalCellIndex].totalRBs += 
                                ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                            if (CE_USER == ulUEContext_p->userLocationType)
                            {
                                ulBufferPool_g[internalCellIndex].cellEdgeRBs += 
                                    ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                            }
#endif
                            if(ulUEContext_p->aperiodicCQIToBeRequested == 
                                    servingCellBitMask_g[START_SCELL_INDEX])
                            {
                                /* case when report for only scell was
                                 * expected and grant type is  CQI_WITH_DATA so
                                 * we have dropped the cqi so changing
                                 * grant type to NEW_UL_GRANT for
                                 * processing in PUSCH */
                                txNode_p->ulGrantRequestType = 
                                    NEW_ULGRANT_REQ_MSG;
                            }
                            ulUEContext_p->aperiodicCQIToBeRequested = 
                                ulUEContext_p->aperiodicCQIToBeRequested &
                                ~servingCellBitMask_g[START_SCELL_INDEX];
                        }
                    }
                }
                /* If cce allocated from common search space, csi field size
                 * will be 1 bit in case of Rel 10 UE */
                /* + SPR_14047 */
                dciCCEInfo_p[*cceAllocated_p].dciPduInfo.csiFieldSize = 0;
                /* - SPR_14047 */
            }
            /* CA_phase2_csi_code end */ 
        }
        /* CA_phase2_csi_code Start */ 
        else
        {
            /* According to 36.212- 5.3.3.1.1 The 2-bit field applies to UEs
             * that are configured with more than one DL cell and when the
             * corresponding DCI format is mapped onto the UE specific
             * search space given by the C-RNTI  otherwise
             * the 1-bit field applies */
            if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
            {
                /* Scell is configured */
                /* + SPR_14047 */
                dciCCEInfo_p[*cceAllocated_p].dciPduInfo.csiFieldSize = 1;
                /* - SPR_14047 */
            }
            else
            {
                /* + SPR_14047 */
                dciCCEInfo_p[*cceAllocated_p].dciPduInfo.csiFieldSize = 0;
                /* - SPR_14047 */
            }
            if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL && 
                    ((ulUEContext_p->aperiodicCQIToBeRequested) & 
                     servingCellBitMask_g[START_SCELL_INDEX]))
            {
                if(!IS_SCELL_ACTIVE(ulUEContext_p->ueIndex,START_SCELL_INDEX))
                {
                    if(txNode_p != PNULL)
                    {
                        /* update minRB and  data size in
                         * txNode */
                        txNode_p->minDataSize -= ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
                            minDataSize;
                        txNode_p->maxDataSize -= ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
                            minDataSize;
                        txNode_p->requiredRB -= ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
                            minRBs;
                        /*Updation of bufferpool in case of required
                         * RBs is reduced */
#ifdef FDD_CONFIG
                        ulBufferPool_g[internalCellIndex].totalRBs += 
                            ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                        if (CE_USER == ulUEContext_p->userLocationType)
                        {
                            ulBufferPool_g[internalCellIndex].cellEdgeRBs += 
                                ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs;
                        }
#endif
                        ulUEContext_p->aperiodicCQIToBeRequested = 
                            ulUEContext_p->aperiodicCQIToBeRequested &
                            ~servingCellBitMask_g[START_SCELL_INDEX];
                    }
                }
            }
            /* CA_phase2_csi_code end */ 
        }
    }

    dciCCEInfo_p[*cceAllocated_p].dciPduInfo.txPower =
        cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset + powerOffset;
    /*Cov 69351 + */
    /*SPR_10440_Changes Start*/
    dciCCEInfo_p[*cceAllocated_p].dciPduInfo.dciFormat = DCI_FORMAT_0;
    /*SPR_10440_Changes End*/
    /*Cov 69351 -*/
    /* SPR 3267 START */
    if (dciCCEInfo_p[*cceAllocated_p].dciPduInfo.txPower > MAX_TX_POWER_OFFSET)
    {
        dciCCEInfo_p[*cceAllocated_p].dciPduInfo.txPower = MAX_TX_POWER_OFFSET;
    }
    /* SPR 3267 END */

    return retVal;
}

/*********************************************************************************
 * Function Name :    processULNackQForSpsUE
 * Inputs        :    ulDelay - The delay of execution legs in terms of the number
 *                        subFrames w.r.t. PHY & the current processing being
 *                        done at MAC for intended tick at PHY.
 *                    currentGlobalTick - The present Global Tick
 *                    availableCCE_p - This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented by  one.
 *                    dciCCEInfo_p - This is pointer to structure on which Free CCE 
 *                       Information is there and once cce is allocated ,updated 
 *                       information is written on the reserved CCE.
 *                    dciCCEContainerInfo_p - pointer to the DCI container info
 *                    nackNode_p (DL_UL_SPLT Only)
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs       :   None
 * Returns       :   MAC_SUCCESS /MAC_FAILURE
 * Description   :   This function will read the Nack queue and store the  
 *                    information in the PDCCH UPLINK OUTPUT container. 
 ***********************************************************************************/
#ifdef FDD_CONFIG
 MacRetType processULNackQForSpsUE(
        UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p
#ifdef DL_UL_SPLIT
            , ContainerNackQueueNode * nackNode_p
#endif
        ,InternalCellIndex  internalCellIndex
        )
{
    /* - Coverity 54550 */
#ifndef DL_UL_SPLIT
    UInt16 nackQueueCount = 0;
    /* +GCC 4.7 */
    UInt8  nackQContainerTick =0;
    /* -GCC 4.7 */
#endif
    UInt16 tempUeIndex=0;
    UInt8 harqIndex = INVALID_HARQ_ID;     
    /* SPR 15909 fix start */
    tickType_t tempPhyTti = 0;
    /* SPR 15909 fix end */
#ifndef DL_UL_SPLIT
    ContainerNackQueueNode * nackNode_p = PNULL;
    /* +GCC 4.7 */    
    UplinkContainer* ulContainerQueue_p = PNULL;     
    /* -GCC 4.7 */    
#endif
    UInt16 pdcchUpLinkOutputContainerTick = 0;
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL; 
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;

    PdcchULNackUEInfo *nackUEInfo_p = PNULL;
    UInt16 nonAdaptiveArrayIndex = 0;
    UInt16 adaptiveArrayIndex[MAX_USER_LOCATION] = {0,0};
    UInt8 rbMapNum = MODULO_SIXTEEN((currentGlobalTick + ulDelay + FDD_HARQ_OFFSET));
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSf = 0;
    UInt32 cceAllocated = 0;
    UInt32 subFrameNum = 0;
    UInt8  maxUlHARQTx = 0;

#ifdef LOG_PRINT_ENABLED    
    /* SPR 15909 fix start */
    tickType_t scheduledTTI = 0;
#endif

    tickType_t currentGlobalTickWithDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    SInt32 availableULRBCount = availableULRBCount_g[internalCellIndex];
#ifdef LOG_PRINT_ENABLED
    UInt32 nackRecvdCount=0x0;
#endif     


#ifndef DL_UL_SPLIT
    nackQContainerTick = MODULO_EIGHT(currentGlobalTickWithDelay - 4);
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;
        nackQueueCount = sQueueCount(&(ulContainerQueue_p->nackQueueForSps));
#endif

    subFrameNum = currentGlobalTickWithDelay % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;

    /*
       we calculate the value of pdcchUpLinkOutputContainerTick  
       All ticks are with respect to the tick when the information is to be sent to PHY interface.
       */
    pdcchUpLinkOutputContainerTick = currentGlobalTickWithDelay % MAX_PDCCH_CONTAINER;

    tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + pdcchUpLinkOutputContainerTick);
#ifndef DL_UL_SPLIT
#ifdef LOG_PRINT_ENABLED
    nackRecvdCount=nackQueueCount;
#endif  
    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
            "[%s]      nackQueueCount = %d pdcchUpLinkOutputContainerTick = %d\n"
            ,__func__,nackQueueCount,pdcchUpLinkOutputContainerTick);
#endif  

    nackUEInfo_p = &tempPdcchULOutput_p->pdcchULNackSpsUEInfo;
    /* ICIC changes start */
    nackUEInfo_p->adaptiveCount[CC_USER] = 0;
    nackUEInfo_p->adaptiveCount[CE_USER] = 0;
    /* ICIC changes end */
    nackUEInfo_p->nonAdaptiveCount = 0;
    nonAdaptiveArrayIndex = nackUEInfo_p->nonAdaptiveCount;

#ifndef DL_UL_SPLIT
    while(nackQueueCount--)
    {
        nackNode_p = (ContainerNackQueueNode *)popNode(&(ulContainerQueue_p->nackQueueForSps));
        if (NULL == nackNode_p)
        {
            break;
        }
        tempUeIndex =  nackNode_p->ueIdx;
#else
        while(1)
        {
            if (NULL == nackNode_p)
            {
                break;
            }
            tempUeIndex =  nackNode_p->ueIdx;
            if( tempUeIndex == INVALID_UE_INDEX)
            {
                /* Delimeter node has been received -  Free the memory of delimeter node */
                freeMemPool(nackNode_p);
                break;
            }
#endif            
            tempPhyTti = nackNode_p->ttiCounter;
            harqIndex = MODULO_EIGHT(tempPhyTti);
            ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         

            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
                tempDirectIndexingUEInfo_p = \
                                             &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
                /* SPR 3430 */
                tempDirectIndexingUEInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
            /* SPR 14877 SRS Changes */ 
                tempDirectIndexingUEInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;         
            /* SPR 14877 SRS Changes */ 
                /* SPR 3430 */
                if(HARQ_PROCESS_RESET == tempULHarqInfo_p->harqStatus)
                {
                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                    freeULHarqProcess(tempULHarqInfo_p);
#endif
                    /* SPR 16916 FIX END */
                    /* TTIB_Code Start */
                    updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);

                    /* TTIB_Code End */
                    freeMemPool((void *)nackNode_p);    
                    nackNode_p = PNULL;
                    LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,harqIndex, \
                            tempPhyTti,\
                            /*SPR 1768 -Start*/
                            INVALID_TX,\
                            /*SPR 1768 -End*/
                            0,\
                            0.0,0.0,__func__,"NACK_DROP_RESET");
                    continue;
                }
                /* + SPS_TDD_Changes */
                if (SPS_SCHEDULING == ulUEContext_p->schType)
                {
                    freeMemPool((void *)nackNode_p);    
                    nackNode_p = PNULL;
                    continue;
                }
                /* - SPS_TDD_Changes */
                nPhichGrp = tempULHarqInfo_p->nPhichGrp;
                nPhichSf  = tempULHarqInfo_p->sfUsed;
                if(!tempULHarqInfo_p->riLength)
                {
                    LTE_MAC_UT_LOG(LOG_CRITICAL,LOG_PDCCH,"\n[%s]\nNACK received for UE RNTI =%d " \
                            "Non Exixtence HARQ ID =%d \n currentGlobalTick %d\n\n",__func__, 
                            tempUeIndex, harqIndex, currentGlobalTick);

                    LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,harqIndex, \
                            tempULHarqInfo_p->nextReTransmissionType,\
                            tempULHarqInfo_p->currentRetransCount,\
                            nackRecvdCount,\
                            0.0,0.0,__func__,"NACK_ZERO_BAND");

                }
                else 
                {
                    LTE_MAC_UT_LOG(LOG_INFO,"PDCCH","[%s]\n########\n"
                            "NACK received for UE RNTI =%d " \
                            "for HARQ ID =%d currentRetransCount = %d "
                            "currentGlobalTick = %d ulDelay = %d "
                            "tempPhyTti = %d \n########\n", __func__,
                            tempUeIndex, harqIndex, 
                            tempULHarqInfo_p->currentRetransCount,
                            currentGlobalTick,ulDelay, tempPhyTti);            
                    if (currentGlobalTickWithDelay <= (tempPhyTti + FDD_HARQ_OFFSET))
                    {

                        tempDirectIndexingUEInfo_p->harqIndex = harqIndex;
                        tempDirectIndexingUEInfo_p->ttiCounter = tempPhyTti;
                        tempDirectIndexingUEInfo_p->strictRBAllocation = \
                                                                         tempULHarqInfo_p->strictRBAllocation;
                        /* +- SPR 17669*/

                        /* + SPS_TDD_Changes */
                        maxUlHARQTx = ulUEContext_p->spsUlInfo.maxSpsUlHARQTx - 1;
                        /* - SPS_TDD_Changes */
                        /*If Current Retx Counter is less than MAX_Retx Counter*/
                        if (tempULHarqInfo_p->currentRetransCount < maxUlHARQTx)
                        {                            
                            /* SPS CHANGES */
                            /* MEAS_GAP_CHG */
                            if (FALSE == isTTIInMeasGapPeriod
                                    (ulUEContext_p, (tempPhyTti + 8)))
                            {
                                if (FALSE == isTTIInMeasGapPeriod
                                        (ulUEContext_p, (tempPhyTti + 4)))
                                {
                                    /* SPR 5467 changes start*/
                                    /* If the next retransmission type is
                                     * adaptive, select the current 
                                     * transmission type as adaptive. */
                                    /* SPR 10753 Changes Start */
                                    if(ADAPTIVE_RETRANS_AFTER_HI_ACK == tempULHarqInfo_p->forceAckNextRetxType)
                                    {
                                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                    } 
                                    /*
                                    if ( !tempULHarqInfo_p->newDataIndicator && 
                                            ADAPTIVE_RE_TX == tempULHarqInfo_p->
                                            nextReTransmissionType )
                                    {
                                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                    }
                                    */
                                        /* SPR 10753 Changes End */
                                    /* Do non-adaptive transmission 
                                     * if same resources are available 
                                     * and there is no TPC to send
                                     * and same PHICH group and sequence 
                                     *   are available for the next retx*/
                                    else if (MAC_SUCCESS == checkNumResourcesFromRBStartIndex(
                                                /* SPR 5467 changes end*/                                            
                                                tempULHarqInfo_p->riStart, 
                                                tempULHarqInfo_p->riLength, 
                                                rbMapNum,internalCellIndex) 
                                            && (ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue == TPC_NO_CHANGE) 
                                            /* CR changes start */
                                            && (MAC_SUCCESS == checkHIResources(\
                                                    phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask,\
                                                    phichSequenceNoPerPhichGroup_g[internalCellIndex][(nPhichSf + FDD_DATA_OFFSET + FDD_HARQ_OFFSET) % MAX_PHICH_CONTAINER][nPhichGrp].bitMask)) )
                                        /* CR changes end */
                                    {
                                        tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                                    }
                                    else
                                    {
                                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                    }
                                }
                                else
                                {
                                    tempDirectIndexingUEInfo_p->flag =
                                        NON_ADAPTIVE_RE_TRANS;
                                    tempDirectIndexingUEInfo_p->ulCCEIndex =
                                        CCE_NOT_ALLOCATED;
                                    ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE; 
                                    tempPdcchULOutput_p->pdcchULNackUEInfo.
                                        pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = 
                                        tempUeIndex;
                                    nonAdaptiveArrayIndex++;
                                    /* SPR 2379 Changes Start */ 
                                    freeMemPool((void *)nackNode_p);
                                    /* SPR 2379 Changes End */ 
                                    nackNode_p = PNULL;
                                    continue;
                                }

                            }
                            else
                            {
                                tempDirectIndexingUEInfo_p->flag =
                                    MEAS_GAP_RE_TX_NO_CCE_NO_GRANT;
                                tempDirectIndexingUEInfo_p->ulCCEIndex =
                                    CCE_NOT_ALLOCATED;
                                ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE; 
                                tempPdcchULOutput_p->pdcchULNackUEInfo.
                                    pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = 
                                    tempUeIndex;
                                nonAdaptiveArrayIndex++;
                                /* SPR 2379 Changes Start */ 
                                freeMemPool((void *)nackNode_p);
                                /* SPR 2379 Changes End */ 
                                nackNode_p = PNULL;
                                continue;
                            }
                            /* MEAS_GAP_CHG */

                            if (ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType != \
                                    NON_ADAPTIVE_RE_TX)
                            {
                                /* If a UL Nack is Received for the UE and 
                                 * re-transmission is Adaptive then the 
                                 * previous nPhichSeq th bit from the LSB of 
                                 * the array phichSequenceNoPerPhichGroup_g
                                 * [Subframe No][nPhichGrp] 
                                 * should be made to set(PHICH is released).
                                 */
                                /* CR changes start */
                                /*phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask
                                  |= (ONE << nPhichSeq);
                                  */
                                /* CR changes end */

                                if (MAC_SUCCESS == allocatePDCCHForULNewReTx(
                                            &ulUEContext_p->\
                                            /* ++ SPR 21406 */
                                            ueSearchSpaceInfo_p[subFrameNum],
                                            /* -- SPR 21406 */
                                            &cceAllocated,
                                            availableCCE_p,
                                            dciCCEInfo_p,
                                            ulUEContext_p,
                                            PNULL, //CA_phase2_csi_code
                                            internalCellIndex))
                                {
                                    tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
                                    tempDirectIndexingUEInfo_p->ulCCEIndex = cceAllocated;
                                    ulUEContext_p->isPdcchAllocated = UL_ADAPTIVE_CCE;
                                    dciCCEContainerInfo_p->arrayCCEAllocated[
                                        dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
                                    dciCCEContainerInfo_p->countOfCCEAllocated++;
                                    nackUEInfo_p->pdcchULNackAdaptiveUEArray[ulUEContext_p->userLocationType]
                                        [adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
                                        tempUeIndex;
                                    adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
                                    /* SPR 10753 Changes Start */
                                    tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
                                    /* SPR 10753 Changes End */

                                    LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
                                            currentGlobalTick,tempUeIndex,harqIndex, \
                                            tempPhyTti,\
                                            tempULHarqInfo_p->nextReTransmissionType,\
                                            cceAllocated,\
                                            0.0,0.0,__func__,"NACK_DCI_SUCC_ULHARQ");
                                }
                                else
                                {
                                    /* FORCED_ACK SPR#9374 Changes Start */
                                    tempDirectIndexingUEInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
                                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                    /* SPR 10753 Changes Start */
                                    tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
                                    /* SPR 10753 Changes End */
                                    ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                                    nackUEInfo_p->pdcchULNackAdaptiveUEArray
                                        [ulUEContext_p->userLocationType]
                                        [adaptiveArrayIndex[ulUEContext_p->userLocationType]] =
                                        tempUeIndex;
                                    adaptiveArrayIndex[ulUEContext_p->userLocationType]++;
                                    /* FORCED_ACK SPR#9374 Changes End */

                                    LOG_MAC_MSG(MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
                                            currentGlobalTick,tempUeIndex,harqIndex, \
                                            tempPhyTti,\
                                            tempULHarqInfo_p->nextReTransmissionType,\
                                            tempULHarqInfo_p->currentRetransCount,\
                                            0.0,0.0,__func__,"NACK_CCE_ALLOC_FAIL");

                                    LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
                                            " allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");            
                                }
                            }
                            else
                            {
                                tempDirectIndexingUEInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                                tempDirectIndexingUEInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;
                                ulUEContext_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                                nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] =
                                    tempUeIndex;
                                nonAdaptiveArrayIndex++;
                                /* SPR 10753 Changes Start */
                                tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
                                /* SPR 10753 Changes End */

                                LOG_MAC_MSG(MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,
                                        getCurrentTick(),
                                        tempUeIndex,harqIndex,
                                        tempULHarqInfo_p->currentRetransCount,
                                        tempULHarqInfo_p->nextReTransmissionType,
                                        DEFAULT_INT_VALUE,
                                        0.0,0.0,FUNCTION_NAME,"");

                                LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, " Non_Adaptive_Re_Trans" \
                                        " of NACK Case \n");            
                            } 
                            availableULRBCount -= tempULHarqInfo_p->riLength;
                        } 
                        else
                        {
                            /* SPS CHANGES */
                            handleMaxSpsHarqRetxOnOccasion(ulUEContext_p, harqIndex, internalCellIndex);
                            /* SPS CHANGES */
                            tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
                            tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                            LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, 
                                    "RECEIVED_NACK_RE_TRANS_COMPELETE for UE Index = %d and Harq ID = %d",
                                    tempUeIndex,harqIndex); 
                            ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                            nackUEInfo_p->pdcchULNackNonAdaptiveUEArray[nonAdaptiveArrayIndex] = tempUeIndex;
                            nonAdaptiveArrayIndex++;
                        }
                    }
                    else
                    {
                        ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                        tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                        tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                        /* SPR 1996 Start*/
                        LOG_MAC_MSG(MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                                currentGlobalTick,tempUeIndex,harqIndex, \
                                tempPhyTti,\
                                tempULHarqInfo_p->nextReTransmissionType,\
                                scheduledTTI,\
                                0.0,0.0,__func__,"DELAY_NACK_FREE_ULHARQ");

                        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                        freeULHarqProcess(tempULHarqInfo_p);
#endif
                        /* SPR 16916 FIX END */
                        /* TTIB_Code Start */
                        updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                        /* TTIB_Code End */


                    }
                }
            }
            else
            { 
                LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "UL NACK of UE ID:%d" \
                        "Having PHY TTI %d,ULSch TTI %d Pending to delete is set",
                        tempUeIndex,tempPhyTti,currentGlobalTick);

                LOG_MAC_MSG(MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,harqIndex, \
                        tempPhyTti,\
                        /*SPR 1768 -Start*/
                        INVALID_TX,\
                        /*SPR 1768 -End*/
                        0,\
                        0.0,0.0,__func__,"NACK_DROP_UEDELETE");

            }
            freeMemPool((void *)nackNode_p);
            nackNode_p = PNULL;
#ifndef DL_UL_SPLIT
        }//while loop
#else
    }//while loop
#endif    

    nackUEInfo_p->nonAdaptiveCount = nonAdaptiveArrayIndex;
    nackUEInfo_p->adaptiveCount[CC_USER] = adaptiveArrayIndex[CC_USER];
    nackUEInfo_p->adaptiveCount[CE_USER] = adaptiveArrayIndex[CE_USER];
    availableULRBCount_g[internalCellIndex] = availableULRBCount;
    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH", 
            "[%s] \n\ttempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount = %d,"
            " ttempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount = %d"
            ,__func__,tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount,
            tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount);
    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Exit",__func__);
    return MAC_SUCCESS;
    /* - Coverity 54550 */
}
/* + SPS_TDD_Changes */
#else
/*********************************************************************************
 * Function Name : processULNackQForSpsUEinTDD 
 * Inputs        : tempPdcchULOutput_p: This is pointer to structure that 
 *                    stores the information about the ues that are scheduled. 
 *                 currentGlobalTick - The present Global Tick
 *                 availableCCE_p : This represent the number of available CCE
 *                    and if CCE's are allocated this would be deceremented 
 *                    by one.
 *                 dciCCEInfo_p : This is pointer to structure on which Free CCE 
 *                    Information is there and once cce is allocated ,updated 
 *                    information is written on the reserved CCE.
 *                 dciCCEContainerInfo_p: This is a pointer to 
 *                    dciCCEContainerInfo_g.
 *                 maxUeScheduledInUplink_p - maximum number of Ue's to be scheduled
 *                    in Uplink
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description:    This function will read the SPS Nack queue and store the  
 *                 information in the PDCCH UPLINK OUTPUT container. 
 ***********************************************************************************/
 MacRetType processULNackQForSpsUEinTDD(
        /*TDD Config 0 changes Start*/
        PdcchInfo* localPdcchInfoMap[],
        /*TDD Config 0 changes End*/
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        UInt32 * availableCCE_p,
        DciCCEInfo * dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p, 
        UInt8 *maxUeScheduledInUplink_p,
        InternalCellIndex internalCellIndex 
        )
{
    UInt8  nackQContainerTick = 0;
    UInt16 nackQueueCount     = 0;
    UInt16 tempUeIndex        = 0;
    UInt8  harqIndex          = INVALID_HARQ_ID;
    UInt32 tempPhyTti         = 0;
    ContainerNackQueueNode * nackNode_p    = PNULL;
    UplinkContainer* ulContainerQueue_p    = PNULL;
    ULHarqInfo*   tempULHarqInfo_p         = PNULL;
    ULUEContext* ulUEContext_p             = PNULL;
    ULUEContextInfo* ulUEContextInfo_p     = PNULL;
    UeScheduledInfo *ueScheduledInfo_p     = PNULL;

    UInt32 cceAllocated                    = 0;
    UInt32 subFrameNum                     = 0;
    UInt32 sysFrameNum                     = 0;
    //UInt32 scheduledTTI                    = 0;
    UInt8  packetSubFrameNum                  = 0;
    /* TDD Config 6 Changes Start */
    UInt16 packetSysFrameNum               = 0;
    /* TDD Config 6 Changes End */
    UInt8  hiPduSubFrameNum                = 0;
    /*TDD Config 0 changes Start*/
    UInt8 phichSubFrameNum                 = 0;
    UInt8 iPhich                           = 0;
    UInt8 ulSfFromNackNode                 = 0;
    PdcchInfo *pdcchInfo_p                 = PNULL;
    UInt8  maxUlHARQTx = 0;

#ifdef KPI_STATS
    UInt8 qciIdx = 0; 
    /* SPR 15429 fix start */
    //UInt32 activeIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats.activeIndexForMacStats);
    /* SPR 15429 fix end */
#endif
    UInt8  measGapFlag = FALSE;
    UInt8  ulschSubFrameNum = 0;

    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Entry",__func__);
    /* Getting Container Tick*/
    nackQContainerTick = (currentGlobalTickWithDelay)% getMaxULCrcContainer(internalCellIndex);
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;

    nackQueueCount = sQueueCount(&(ulContainerQueue_p->nackQueueForSps));
    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "nackQContainerTick [%d] nackQueueForSps[%d]",
            nackQContainerTick,nackQueueCount);

    /* SPR 23393 Changes Start */
    subFrameNum = currentGlobalTickWithDelay % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    sysFrameNum = (currentGlobalTickWithDelay/ MAX_SUB_FRAME) % MAX_SFN_VALUE;
    /* SPR 23393 Changes End */

    while(nackQueueCount--)
    {
        nackNode_p = (ContainerNackQueueNode *)popNode(&(ulContainerQueue_p->nackQueueForSps));
        if(PNULL != nackNode_p)
        {
            tempUeIndex =  nackNode_p->ueIdx;
            tempPhyTti = nackNode_p->ttiCounter;

            ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];

            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                /*TDD Config 0 Change Start*/
                ulSfFromNackNode = nackNode_p->ulSubFrame;
                pdcchInfo_p = localPdcchInfoMap[ulSfFromNackNode];
                /*TDD Config 0 Change End*/
                /*getting harq id from subframe map */ 
                /* TDD Config 6 Changes Start */
                GET_SFN_SF_FROM_TTI(tempPhyTti, packetSysFrameNum, packetSubFrameNum);
                harqIndex      = getULHarqIDFromSubFrame (packetSubFrameNum, ulUEContext_p, internalCellIndex, packetSysFrameNum);
                // harqIndex = nackNode_p->ulHarqProcessId;
                /* TDD Config 6 Changes End */

                //SPR_16908_Changes
                /* if ( SPS_SCHEDULING == ulUEContext_p->schType) 
                   {
                   freeMemPool((void *)nackNode_p);
                   nackNode_p = PNULL;
                   continue;
                   }*/
                //SPR_16908_Changes
                if ( INVALID_HARQ_ID == harqIndex )
                {
                    LOG_MSG(MAC_INVALID_HID_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                            currentGlobalTickWithDelay,\
                            tempUeIndex,packetSubFrameNum,0,0,0,\
                            0.0,0.0,__func__,\
                            "Harq Id is already free for this sub frame");
                    freeMemPool((void *)nackNode_p);
                    nackNode_p = PNULL;
                    continue;
                }
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
                /*TDD Config 0 Changes Start*/
                phichSubFrameNum = nackNode_p->ulSubFrame + getHiPduSubframeNum(nackNode_p->ulSubFrame,internalCellIndex);
                phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
                iPhich = calculateIPhichValue(nackNode_p->ulSubFrame,internalCellIndex);
                /*TDD Config 0 Changes End*/

                if(!tempULHarqInfo_p->riLength)
                {
                    LOG_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                            currentGlobalTickWithDelay,tempUeIndex,harqIndex, \
                            tempULHarqInfo_p->nextReTransmissionType,\
                            tempULHarqInfo_p->currentRetransCount,\
                            0,\
                            0.0,0.0,__func__,"NACK_ZERO_BAND");
                    //return MAC_FAILURE;                            
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,"PDCCH","[%s]\n########\n"
                            "NACK received for UE RNTI =%d " \
                            "for HARQ ID =%d currentRetransCount = %d "
                            " NackQContainerTick %d \n"
                            "currentGlobalTickWithDelay = %d "
                            "tempPhyTti = %d \n########\n", __func__,
                            tempUeIndex, harqIndex,
                            ulUEContext_p->ulresInfo[harqIndex].currentRetransCount,
                            nackQContainerTick,currentGlobalTickWithDelay, tempPhyTti);

                    if (! isValidULSubFrameForHIPdu ( packetSubFrameNum, &hiPduSubFrameNum, internalCellIndex ))
                    {
                        ltePanic("Invalid SubFrame[%d] for UL HI Pdu\n", packetSubFrameNum);
                    }

                    if (!isValidSubframeNumForDCI_0 (subFrameNum, internalCellIndex))
                    {
                        ltePanic("Invalid SubFrame[%d] for DCI -0\n", subFrameNum);

                    }
                    /** SPR 14204 Fix : Start **/
                    ueScheduledInfo_p = ulUEContext_p->
                        ulUeScheduledInfoPointerArray[internalCellIndex][ulSfFromNackNode];
                    /** SPR 14204 Fix : End **/
                    if ( PNULL == ueScheduledInfo_p )
                    {
                        ltePanic(" ueScheduledInfo_p is NULL 3 subFrameNum [%d]\n", subFrameNum);
                        /* + Coverity 88285 */
                        break; 
                        /* - Coverity 88285 */
                    }
                    maxUlHARQTx = ulUEContext_p->spsUlInfo.maxSpsUlHARQTx - 1;
                    /*If Current Retx Counter is less than MAX_Retx Counter*/

                    if (currentGlobalTickWithDelay <= (tempPhyTti + hiPduSubFrameNum))
                    {
                        ueScheduledInfo_p->strictRBAllocation = tempULHarqInfo_p->strictRBAllocation;
                        ueScheduledInfo_p->ttiCounter         = tempPhyTti;
                        ueScheduledInfo_p->ulHarqProcessId    = harqIndex;
#ifdef KPI_STATS
                        for ( qciIdx = 0;qciIdx < MAX_QCI ; qciIdx++ )
                        {
                            if (tempULHarqInfo_p->qciInfo[qciIdx] == 1 )
                            {
                                if (!ulUEContext_p->qciInfoFlag[qciIdx])
                                {    
                                    /* SPR 15429 fix start */
                                    /* + KPI_CA */
                                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qciIdx]++;
                                    /* - KPI_CA */
                                    /* SPR 15429 fix end */
                                    ulUEContext_p->qciInfoFlag[qciIdx] = 1;
                                }   

                            }
                        }
#endif
                        /* UL_MU_MIMO_CHG_START */
#ifdef SSI_DEBUG_STATS
                        if( UL_MU_MIMO_ENABLED == tempULHarqInfo_p->ulMuMimoStatus )
                        {
                            /* CA Stats Changes Start */
                            LTE_MAC_UPDATE_UL_MIMO_UE_RETX(internalCellIndex);
                            /* CA Stats Changes End */
                            tempULHarqInfo_p->ulMuMimoStatus = UL_MU_MIMO_DISABLED;
                        }
#endif
                        tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
                        /* UL_MU_MIMO_CHG_END */
                        /* SPR 10803 [BUG 848] FIX START */
                        /* SPR 10753 Changes Start */
                        if ((!tempULHarqInfo_p->newDataIndicator && 
                                    ADAPTIVE_RETRANS_AFTER_HI_ACK == tempULHarqInfo_p->
                                    forceAckNextRetxType ) )
                        {
                            tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        }
                        /* SPR 10753 Changes End */

                        else if (MAC_SUCCESS == checkNumResourcesFromRBStartIndex( 
                                    /* SPR 10803 [BUG 848] FIX END */
                                    tempULHarqInfo_p->riStart, 
                                    tempULHarqInfo_p->riLength, 
                                    pdcchInfo_p->rbMapNum, internalCellIndex) && 
                                /* SPR 10753 Changes Start */
                                /*
                                   ( ADAPTIVE_RE_TX != tempULHarqInfo_p->nextReTransmissionType)*/
                                /* SPR 10753 Changes End */
                                /*TDD Config 0 Changes Start*/ 
                                /*Check if same HI resources are available as used
                                  earlier*/
                                ( MAC_SUCCESS == checkAvailabilityOfHIResources(
                                                                                phichSubFrameNum, iPhich,
                                                                                tempULHarqInfo_p , internalCellIndex) ) )
                            /*TDD Config 0 Changes End*/ 
                        {
                            /* UL_MU_MIMO_CHG_START */
                            if (!pdcchInfo_p->localReTxRBMap[tempULHarqInfo_p->riStart]) 
                            {
                                tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                                pdcchInfo_p->localReTxRBMap[tempULHarqInfo_p->riStart] = 
                                    tempULHarqInfo_p->riLength;
                            }
                            else
                            {
                                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                            }
                            /* UL_MU_MIMO_CHG_END */    
                        }
                        else
                        {
                            tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        }
                        /* UL_MU_MIMO_CHG_START */
                        LOG_MSG (MAC_UL_MIMO_RE_TX_INFO_ID,LOGDEBUG,MAC_PDCCH,\
                                getCurrentTick(),tempUeIndex,tempULHarqInfo_p->riStart,\
                                tempULHarqInfo_p->riLength,\
                                tempULHarqInfo_p->currentRetransCount,tempULHarqInfo_p->nextReTransmissionType,\
                                0.0,0.0,__func__,"UE_RETX_INFO");
                        /* UL_MU_MIMO_CHG_END */

                        if ((tempULHarqInfo_p->currentRetransCount) < maxUlHARQTx)
                        {
                            /* MEAS_GAP_CHG */
                            /*TDD Config 0 Start*/
                            /*    ulschSubFrameNum = getExactULSubFrame((tempPhyTti + \
                                  hiPduSubFrameNum)%MAX_SUB_FRAME, internalCellIndex); */
                            ulschSubFrameNum = getUlOffset(subFrameNum,ulSfFromNackNode);
                            /*TDD Config 0 Start*/
                            if(TRUE == isTTIInMeasGapPeriod
                                    (ulUEContext_p, (tempPhyTti + hiPduSubFrameNum)))
                            {
                                measGapFlag=TRUE;
                                ueScheduledInfo_p->flag =
                                    NON_ADAPTIVE_RE_TRANS;
                            }                                
                            else if(  TRUE == isTTIInMeasGapPeriod
                                    (ulUEContext_p, (tempPhyTti +
                                                     hiPduSubFrameNum + ulschSubFrameNum)))
                            {
                                measGapFlag=TRUE;
                                ueScheduledInfo_p->flag =
                                    MEAS_GAP_RE_TX_NO_CCE_NO_GRANT;

                            }

                            if(TRUE == measGapFlag)
                            {
                                ueScheduledInfo_p->ulCCEIndex =
                                    CCE_NOT_ALLOCATED;
                                tempULHarqInfo_p->isPdcchAllocated = 
                                    UL_NON_ADAPTIVE_NO_CCE;

                                /*tempPdcchULOutput_p->pdcchULNackUEInfo.
                                  pdcchULNackUEArray[arrayIndex] =  tempUeIndex;*/

                                /* SPR 18510 - start*/
                                pdcchInfo_p->nackSpsUEInfo_p->
                                    pdcchULNackNonAdaptiveUEArray[pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount] = 
                                    tempUeIndex;
                                pdcchInfo_p->nackSpsUEInfo_p->nonAdaptiveCount++;
                                /* SPR 18510 - end*/
                                /* SPR 2379 Changes Start */ 
                                freeMemPool((void *)nackNode_p);
                                nackNode_p = NULL;
                                /* SPR 2379 Changes End */ 
                                continue;
                            } 

                            if (ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType != \
                                    NON_ADAPTIVE_RE_TX)
                            {
                                /* If a UL Nack is Received for the UE and 
                                 * re-transmission is Adaptive then the 
                                 * previous nPhichSeq th bit from the LSB of 
                                 * the array phichSequenceNoPerPhichGroup_g
                                 * [Subframe No][nPhichGrp] 
                                 * should be made to set(PHICH is released).
                                 */
                                /*TDD Config 0 Changes Start*/
                                /*We do not need to free d phich resources since
                                 * they are being reset in PUSCH*/
                                /*TDD Config 0 Changes End*/ 
                                if (LTE_TRUE == isUeTypeInvalid_g[internalCellIndex])
                                {
                                    /* FORCED_ACK SPR#9374 Changes Start */
                                    ueScheduledInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
                                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                    ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                                    /* SPR 18510 - start*/
                                    pdcchInfo_p->nackSpsUEInfo_p->pdcchULNackAdaptiveUEArray
                                        [ulUEContext_p->userLocationType]
                                        [pdcchInfo_p->nackSpsUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]] =
                                        tempUeIndex;
                                    pdcchInfo_p->nackSpsUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]++;
                                    /* SPR 18510 - end*/
                                    /* FORCED_ACK SPR#9374 Changes End */
                                    LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
                                            " allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");
                                }
                                else
                                {
                                    if (MAC_SUCCESS == allocatePDCCHForULNewReTx(
                                                &ulUEContext_p->\
                                                spsUeSearchSpaceInfo_p[subFrameNum],
                                                &cceAllocated,
                                                availableCCE_p,
                                                dciCCEInfo_p,
                                                ulUEContext_p,
                                                PNULL, //CA_phase2_csi_code
                                                internalCellIndex))
                                    {
                                        ueScheduledInfo_p->flag = RECEIVED_NACK;
                                        ueScheduledInfo_p->ulCCEIndex = cceAllocated;

                                        tempULHarqInfo_p->isPdcchAllocated = UL_ADAPTIVE_CCE;

                                        dciCCEContainerInfo_p->arrayCCEAllocated[
                                            dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
                                        dciCCEContainerInfo_p->countOfCCEAllocated++;
                                        /* ICIC changes start */
                                        /*TDD Config 0 Changes Start*/
                                        /* SPR 18510 - start*/
                                        pdcchInfo_p->nackSpsUEInfo_p->pdcchULNackAdaptiveUEArray
                                            [ulUEContext_p->userLocationType]
                                            [pdcchInfo_p->nackSpsUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]] =
                                            tempUeIndex;
                                        pdcchInfo_p->nackSpsUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]++;
                                        /* SPR 18510 - end*/
                                        /*TDD Config 0 Changes End*/
                                        /* ICIC changes end */
                                        /* + SPR 5551 */
                                        dciCCEInfo_p[cceAllocated].dciPduInfo.cqiRequest = 0;
                                        /* - SPR 5551 */
                                        /* SPR 10753 Changes Start */
                                        tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
                                        /* SPR 10753 Changes End */
                                    }
                                    else
                                    {
                                        /* FORCED_ACK SPR#9374 Changes Start */
                                        ueScheduledInfo_p->flag = ADAPTIVE_RETRANS_NO_CCE;
                                        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                        tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                                        /* SPR 10753 Changes Start */
                                        tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
                                        /* SPR 10753 Changes End */
                                        tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                                        /* SPR 18510 - start*/
                                        pdcchInfo_p->nackSpsUEInfo_p->pdcchULNackAdaptiveUEArray
                                            [ulUEContext_p->userLocationType]
                                            [pdcchInfo_p->nackSpsUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]] =
                                            tempUeIndex;
                                        pdcchInfo_p->nackSpsUEInfo_p->adaptiveCount[ulUEContext_p->userLocationType]++;
                                        /* SPR 18510 - end*/
                                        /* FORCED_ACK SPR#9374 Changes End */

                                        LTE_MAC_UT_LOG (LOG_WARNING, LOG_PDCCH, "The function" \
                                                " allocatePDCCHForDCIFormatUESpace returned MAC_FAILURE");
                                    }
                                }
                            }
                            else
                            {
                                ueScheduledInfo_p->flag = NON_ADAPTIVE_RE_TRANS;
                                ueScheduledInfo_p->ulCCEIndex = CCE_NOT_ALLOCATED;

                                tempULHarqInfo_p->isPdcchAllocated = UL_NON_ADAPTIVE_NO_CCE;
                                /* SPR 18510 - start*/
                                pdcchInfo_p->nackSpsUEInfo_p->pdcchULNackNonAdaptiveUEArray
                                    [pdcchInfo_p->nackSpsUEInfo_p->nonAdaptiveCount] =
                                    tempUeIndex;
                                pdcchInfo_p->nackSpsUEInfo_p->nonAdaptiveCount++;
                                /* SPR 18510 - end*/
                                /* SPR 10753 Changes Start */
                                tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
                                /* SPR 10753 Changes End */
                                LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, " Non_Adaptive_Re_Trans" \
                                        " of NACK Case \n");
                            }
                            /* 5798 Fix Start */
                            /*TDD Config 0 Changes Start*/
                            maxUeScheduledInUplink_p[ulSfFromNackNode]--;
                            /*TDD Config 0 Changes End*/
                            /* 5798 Fix End */
                        }
                        else
                        { 
                            handleMaxSpsHarqRetxOnOccasion(ulUEContext_p,harqIndex, internalCellIndex);
                            ueScheduledInfo_p->flag = RECEIVED_NACK_RE_TRANS_COMPELETE;
                            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                            LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH,
                                    "RECEIVED_NACK_RE_TRANS_COMPELETE for UE Index = %d and Harq ID = %d",
                                    tempUeIndex,harqIndex);
                            tempULHarqInfo_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                            /* SPR 18510 - start*/
                            pdcchInfo_p->nackSpsUEInfo_p->pdcchULNackNonAdaptiveUEArray
                                [pdcchInfo_p->nackSpsUEInfo_p->nonAdaptiveCount] = tempUeIndex;
                            pdcchInfo_p->nackSpsUEInfo_p->nonAdaptiveCount++;
                            /* SPR 18510 - end*/
                        }
                    }
                    else
                    { 
                        ulUEContext_p->isPdcchAllocated = NO_CCE_ALLOCATED;
                        ueScheduledInfo_p->flag = UNSET_FLAG;
                        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                        if ((tempULHarqInfo_p->currentRetransCount) >= (ulUEContext_p->maxUlHARQTx -1))
                        {                        
                            resetHarqId( packetSubFrameNum,ulUEContext_p, internalCellIndex, packetSysFrameNum);
                            /* SPR 16916 FIX START */
#ifdef KPI_STATS
                            freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                            freeULHarqProcess(tempULHarqInfo_p);
#endif
                            /* SPR 16916 FIX END */
                        }
                        else
                        {
#ifndef DL_UL_SPLIT
                            tempULHarqInfo_p->currentRetransCount += \
                                                                     ((currentGlobalTickWithDelay - tempPhyTti)/MAX_SUB_FRAME) + 1;
                            ulHarqTimerStart( subFrameNum,
                                    sysFrameNum,
                                    tempUeIndex,
                                    harqIndex,
                                    /* + SPS_TDD_Changes */
                                    SPS_SCHEDULING,
                                    /* - SPS_TDD_Changes */
                                    internalCellIndex,
                                    ulSfFromNackNode);
#else
                            /* SPR 16916 FIX START */
#ifdef KPI_STATS
                            freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                            freeULHarqProcess(tempULHarqInfo_p);
#endif
                            /* SPR 16916 FIX END */
#endif
                        }
                        LTE_MAC_UT_LOG(LOG_WARNING, LOG_PDCCH,
                                "nackQueue Node later than n+3, so discarding the UE for"
                                "NACK  retransmission resources\n");
                    }
                }
            }
            else
            {
                LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "UL NACK of UE ID:%d" \
                        "Having PHY TTI %d,Pending to delete is set",
                        tempUeIndex,tempPhyTti);
            }
            freeMemPool((void *)nackNode_p);
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PDCCH, "nackQueue Node not valid...\n");
        }
    }

    LTE_MAC_UT_LOG (LOG_INFO, "PDCCH",
            "[%s] \n\t nackUEInfo_p->nonAdaptiveCount = %d nackUEInfo_p->adaptiveCount[CC_USER] = %d availableULRBCount_g[%d]\n"
            ,__func__,pdcchInfo_p->nackUEInfo_p->nonAdaptiveCount,pdcchInfo_p->nackUEInfo_p->adaptiveCount[CC_USER], availableULRBCount_g);
    LTE_MAC_UT_LOG (LOG_INFO, LOG_PDCCH, "[%s] Exit",__func__);
    return MAC_SUCCESS;
}
#endif
/* - SPS_TDD_Changes */
/*START : DYN_PDCCH*/
/*********************************************************************************
 * Function Name : calculateCFI
 * Inputs        : currentGlobalTickWithDelay,
 *                 pdcchDLOutputInfo_p,
 *                 pdcchULOutputInfo_p,
 *                 ulDlConfigInSF,
 *                 totalCCERequired_p.
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : currentCFIValue_g   
 * Description   : This function will calculate the CFI value to be used based
 *                 on number of CCEs.
 **********************************************************************************/
/* SPR 15909 fix start */
UInt8 calculateCFI(tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
                          PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                          /* +- SPR 17777 */
#ifdef FDD_CONFIG
                          PdcchULOutputInfo *pdcchULOutputInfo_p,
#endif
                          /* +- SPR 17777 */
                          DLULConfigType ulDlConfigInSF,
                          UInt8 *totalCCERequired_p,
                          InternalCellIndex internalCellIndex)
{
    UInt8 totalMinCCERequired = 0;
    UInt8 index               = 0;

    /*SPR 6925 Optimization Start*/
#ifndef DL_UL_SPLIT
    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->isDynamicPdcchDisabled)
    {
         currentCFIValue_g[internalCellIndex] = valueCFI_g[internalCellIndex];
         return currentCFIValue_g[internalCellIndex];
    }
#endif
    /*SPR 6925 Optimization End*/
    totalMinCCERequired = calculateRequiredCCEs(currentGlobalTickWithDelay,
                                                        pdcchDLOutputInfo_p,
                                                        /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                                        pdcchULOutputInfo_p,
#endif
                                                        /* +- SPR 17777 */
                                                        ulDlConfigInSF,
                                                        internalCellIndex);
    *totalCCERequired_p = totalMinCCERequired;
    for(;index < valueCFI_g[internalCellIndex];index++)
    {
#ifdef FDD_CONFIG 
        if(maxCCEForCFI_g[internalCellIndex][index] > totalMinCCERequired)
#elif TDD_CONFIG
            /** CA-TDD Changes Start **/
        if(maxCCEForCFI_g[internalCellIndex][index][miValueForSubframe_g[internalCellIndex]]> totalMinCCERequired)
            /** CA-TDD Changes Stop **/
#endif
            {
                currentCFIValue_g[internalCellIndex] = index + 1;
                /*Max CFI value is 3*/
                if(currentCFIValue_g[internalCellIndex] > 3)
                {
                    currentCFIValue_g[internalCellIndex] = 3;
                }
                break;
            }
    }
    if (changeCFIInThisTTI_g[internalCellIndex] == TRUE)
    {
        currentCFIValue_g[internalCellIndex] = valueCFI_g[internalCellIndex];
    }
    return currentCFIValue_g[internalCellIndex];
}
/*********************************************************************************
 * Function Name : calculateRequiredCCEs
 * Inputs        : currentGlobalTickWithDelay - current Tick with Delay to PHY
 *                 pdcchDLOutputInfo_p - Pointer to the PDCCH DL output info
 *                    which contatins the CCE and RB allocated info
 *                 pdcchULOutputInfo_p - Pointer to the PDCCH UL output info
 *                    which contatins the CCE and RB allocated info
 *                 ulDlConfigInSF - config type either DL/UL/both 
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : totalMinCCERequired
 * Description   : This function will calculate the minimum number of total CCEs required
 *                 in the system for new and retransmissions in DL/UL
 **********************************************************************************/
                                /* SPR 15909 fix start */
UInt8 calculateRequiredCCEs(tickType_t currentGlobalTickWithDelay,
                               /* SPR 15909 fix end */
                                   PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                                   /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                   PdcchULOutputInfo *pdcchULOutputInfo_p,
#endif
                                   /* +- SPR 17777 */
                                   DLULConfigType ulDlConfigInSF,
                                   InternalCellIndex internalCellIndex)
{
    UInt32  availableCCELocal      = 0;
    UInt8   totalMinCCERequired    = 0;
    SInt32  availableDLRBCount     = 0;
    SInt32  availableULRBCount     = 0;
    UInt8   maxUEsToBeScheduledDL  = 0;
    UInt8   maxUEsToBeScheduledUL  = 0;
    UInt8  totalMinCCERequired_DLReTx    = 0;
    UInt8  totalMinCCERequired_DLNewTx   = 0;
#ifndef DL_UL_SPLIT_TDD
    UInt8  totalMinCCERequired_ULReTx    = 0;
    UInt8  totalMinCCERequired_ULNewTx   = 0;
#else
    UInt8  totalMinCCERequired_ULTx   = 0;
#endif

    #ifdef FDD_CONFIG
    availableCCELocal      = maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex]-1];
    maxUEsToBeScheduledUL  =  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                               initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    #elif TDD_CONFIG
    /** CA-TDD Changes Start **/ 
    availableCCELocal      =  maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex]-1][miValueForSubframe_g[internalCellIndex]];
    /** CA-TDD Changes Stop **/
    /* SPR 10824 Changes Start */
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {
        maxUEsToBeScheduledUL  =  NUM_TWO * cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                               initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    }
    else
    {
        maxUEsToBeScheduledUL  =  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                  initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    }
    /* SPR 10824 Changes End */
    #endif
    availableDLRBCount     =  (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;
    availableULRBCount     =  (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB;
    maxUEsToBeScheduledDL  =  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                               initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;

    switch(ulDlConfigInSF)
	{

		case BOTH_DL_UL:
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
			if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULReTx = calculateCCERequiredInULReTx(currentGlobalTickWithDelay,
						&availableULRBCount,&maxUEsToBeScheduledUL,
                                                internalCellIndex);
				availableCCELocal -=totalMinCCERequired_ULReTx;
			}
			if (availableCCELocal && availableDLRBCount && maxUEsToBeScheduledDL)
			{
				totalMinCCERequired_DLReTx = minimumCCERequiredDLReTx(&availableDLRBCount,
						&maxUEsToBeScheduledDL,
						pdcchDLOutputInfo_p,
                        /* +- SPR 17777 */
#ifdef TDD_CONFIG
						currentGlobalTickWithDelay,
#endif
                        /* +- SPR 17777 */
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_DLReTx;
			}
			if (availableCCELocal && availableDLRBCount && maxUEsToBeScheduledDL)
			{
				totalMinCCERequired_DLNewTx = minimumCCERequiredDLNewTx(&availableDLRBCount,
						&maxUEsToBeScheduledDL,
						pdcchDLOutputInfo_p,
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_DLNewTx;
			}
			if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULNewTx = calculateCCERequiredInULNewTx(&availableULRBCount,
                        /* +- SPR 17777 */
						&maxUEsToBeScheduledUL,
#ifdef FDD_CONFIG
                        pdcchULOutputInfo_p,
#endif
                        /* +- SPR 17777 */
                        /* +- SPR 17777 */
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_ULNewTx;
			}
#else
			if (availableCCELocal && availableDLRBCount && maxUEsToBeScheduledDL)
			{
				totalMinCCERequired_DLReTx = minimumCCERequiredDLReTx(&availableDLRBCount,
						&maxUEsToBeScheduledDL,
						pdcchDLOutputInfo_p,
                        /* +- SPR 17777 */
#ifdef TDD_CONFIG
						currentGlobalTickWithDelay,
#endif
                        /* +- SPR 17777 */
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_DLReTx;
			}
			if (availableCCELocal && availableDLRBCount && maxUEsToBeScheduledDL)
			{
				totalMinCCERequired_DLNewTx = minimumCCERequiredDLNewTx(&availableDLRBCount,
						&maxUEsToBeScheduledDL,
						pdcchDLOutputInfo_p,
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_DLNewTx;
			}
#ifndef DL_UL_SPLIT_TDD
			if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULReTx = calculateCCERequiredInULReTx(currentGlobalTickWithDelay,
						&availableULRBCount,&maxUEsToBeScheduledUL,
                                                internalCellIndex);
				availableCCELocal -=totalMinCCERequired_ULReTx;
			}
			{
				totalMinCCERequired_ULNewTx = calculateCCERequiredInULNewTx(&availableULRBCount,
                        /* +- SPR 17777 */
						&maxUEsToBeScheduledUL,
#ifdef FDD_CONFIG
                        pdcchULOutputInfo_p,
#endif
                        /* +- SPR 17777 */
                        /* +- SPR 17777 */
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_ULNewTx;
			}
#else
            /*In DL UL SPLIT TDD PDCCH has been split between DL and UL threads. So unlike FDD there is no sync
              between UL and DL thread during CFI calculation. Since there is no uplink information available, 
              Uplink CCE requirement is calculated based on following assumption  
              UE no.= maxUEsToBeScheduledUL and Aggregation Level= 2*/

            if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULTx = calculateCCERequiredInULTx();
				availableCCELocal -=totalMinCCERequired_ULTx;
			}

#endif
#endif
			break;
		case ONLY_UL:
#ifndef DL_UL_SPLIT_TDD
			if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULReTx = calculateCCERequiredInULReTx(currentGlobalTickWithDelay,
						&availableULRBCount,&maxUEsToBeScheduledUL,
                                                internalCellIndex);
				availableCCELocal -=totalMinCCERequired_ULReTx;
			}
			if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULNewTx = calculateCCERequiredInULNewTx(&availableULRBCount,
                        /* +- SPR 17777 */
						&maxUEsToBeScheduledUL,
#ifdef FDD_CONFIG
                        pdcchULOutputInfo_p,
#endif
                        /* +- SPR 17777 */
                        /* +- SPR 17777 */
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_ULNewTx;
			}
#else
            /*In DL UL SPLIT TDD PDCCH has been split between DL and UL threads. So unlike FDD there is no sync
              between UL and DL thread during CFI calculation. Since there is no uplink information available, 
              Uplink CCE requirement is calculated based on following assumption  
              UE no.= maxUEsToBeScheduledUL and Aggregation Level= 2*/

            if (availableCCELocal && availableULRBCount && maxUEsToBeScheduledUL)
			{
				totalMinCCERequired_ULTx = calculateCCERequiredInULTx();
				availableCCELocal -=totalMinCCERequired_ULTx;
			}

#endif
			break;
		case ONLY_DL:
			if (availableCCELocal && availableDLRBCount && maxUEsToBeScheduledDL)
			{
				totalMinCCERequired_DLReTx = minimumCCERequiredDLReTx(&availableDLRBCount,
						&maxUEsToBeScheduledDL,
						pdcchDLOutputInfo_p,
                        /* +- SPR 17777 */
#ifdef TDD_CONFIG
						currentGlobalTickWithDelay,
#endif
                        /* +- SPR 17777 */
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_DLReTx;
			}
			if (availableCCELocal && availableDLRBCount && maxUEsToBeScheduledDL)
			{
				totalMinCCERequired_DLNewTx = minimumCCERequiredDLNewTx(&availableDLRBCount,
						&maxUEsToBeScheduledDL,
						pdcchDLOutputInfo_p,
                        internalCellIndex);
				availableCCELocal -=totalMinCCERequired_DLNewTx;
			}  
			break;
		default :
			break;
	}
#ifndef DL_UL_SPLIT_TDD
    totalMinCCERequired = totalMinCCERequired_DLReTx + 
                          totalMinCCERequired_DLNewTx +
                          totalMinCCERequired_ULReTx + 
                          totalMinCCERequired_ULNewTx;
#else
    totalMinCCERequired = totalMinCCERequired_DLReTx + 
                          totalMinCCERequired_DLNewTx +
                          totalMinCCERequired_ULTx; 

#endif
    return totalMinCCERequired;
}
/*********************************************************************************
 * Function Name : minimumCCERequiredDLNewTx
 * Inputs        : availableDLRBCount - count of availables RB's in Dl,
 *                 maxUEsToBeScheduledDL - maximum number UE's can be scheduled in DL,
 *                 pdcchDLOutputInfo_p - Pointer to the PDCCH DL output info
 *                    which contatins the CCE and RB allocated info
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : totalMinCCERequired
 * Description   : This function will calculate the minimum number of CCEs required
 *                 for DL new transmission
 **********************************************************************************/
UInt8 minimumCCERequiredDLNewTx(SInt32* availableDLRBCount,
        UInt8* maxUEsToBeScheduledDL,
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt16 ueIndex                       = 0;
    UInt32 countDLUENewTx                = 0;
    DCIFormat  dciFormat                 = MAX_NUM_DCI_FORMAT;
    UInt32 dciSize                       = 0;
    UInt32 dciSizeCategory               = 0;
    UInt32 localCount                    = 0;
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
    UInt32 aggregationLevel              = 0;
    DLUEContext *dlUEContext_p           = PNULL;
    UInt32 cqiValue                      = 0;
    UInt8 totalMinCCERequired            = 0;
    UInt8 userLocationType               = CC_USER;
    UInt8 arrayIndex                     = 0;
    AggregationPowerOffset *aggregationLevelPowerOffset_p = PNULL;
    DLMIMOInfo  *dlMIMOInfo_p=PNULL;
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* + SPR 11491 Changes */
    UInt8 isPdcchAllocatedFlag = FALSE;
    /* - SPR 11491 Changes */

    countDLUENewTx    = sQueueCount(dlStrategyNewTxQueue_gp[internalCellIndex] );
    CellConfigParams *cellParams_p = \
                                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    dlStrategyTxNode_p = (DLStrategyTxNode*)
        getFirstNode(dlStrategyNewTxQueue_gp[internalCellIndex]);
    while (countDLUENewTx-- && *maxUEsToBeScheduledDL && *availableDLRBCount)
    {
        /*Firstly check the strategynode is not NULL*/
        if(dlStrategyTxNode_p == PNULL)
        {
            dlStrategyTxNode_p = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode_p);
            continue;
        }
        /*Now get the UE context Info*/
        ueIndex =  dlStrategyTxNode_p->ueIndex;
        /*Now check if UE Context is NULL or not*/
        /* Coverity CID 54503 start */
        /* code removed for coverity */
        /* Coverity CID 54503 end */
        /*Check for pending delete flag*/
        if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
        {
            dlStrategyTxNode_p = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode_p);
            continue;
        }
        /*Now get the UE context*/
        dlUEContext_p    = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
        /*Checking if Pdcch has been allocated earlier or not .If allocated 
          then skip allocating CCE for this UE .If not allocated then set the 
          flag isPdcchAllocated in UE Context  to true */
        /* + SPR 11491 Changes */
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            isPdcchAllocatedFlag = dlUEContext_p->isPdcchAllocatedFlag;
        }
        else
        {
            isPdcchAllocatedFlag = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag;
        }
        /* SPR 22252 fix start */
        userLocationType = getUserLocationType(dlUEContext_p,internalCellIndex);
        if ( isPdcchAllocatedFlag)
        {
            dlStrategyTxNode_p = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode_p);
            continue;
        }
        if(((userLocationType == CE_USER)&&
                    (pdcchDLOutputInfo_p->countMsg[CE_USER] ==
                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     initParams_p->dlSchdConfig.maxCellEdgeUEsToBeScheduledDL))&&(((pdcchDLOutputInfo_p->countMsg[CC_USER] +
                             pdcchDLOutputInfo_p->countMsg[CE_USER]) <
                         cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         initParams_p->dlSchdConfig.maxUEsToBeScheduledDL)))
        {
            dlStrategyTxNode_p = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode_p);
            continue; 
        }
        /* - SPR 11491 Changes */
        else if((pdcchDLOutputInfo_p->countMsg[CC_USER] + 
                  pdcchDLOutputInfo_p->countMsg[CE_USER]) >= 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 initParams_p->dlSchdConfig.maxUEsToBeScheduledDL)
        {
            /* SPR 22579 Coverity Changes + */
            return totalMinCCERequired;
            /* SPR 22579 Coverity Changes - */
        }

        /*pdcch allocation For scell changes start*/
        dlMIMOInfo_p=getdlMIMOInfo(dlUEContext_p,internalCellIndex);
        dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
        if((dlMIMOInfo_p == PNULL) ||(dlCQIInfo_p == PNULL))
        {
            return MAC_FAILURE;
        }
        /* SPR 22252 fix end */
        /*pdcch allocation For scell changes end*/

        if( SPS_SCHEDULING == dlUEContext_p->schType )
        {
            userLocationType = CC_USER;
        }

       if (cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable &&
                dlCQIInfo_p->isWideBandReportAvailable)
        {
            if ((TX_MODE_3 == dlMIMOInfo_p->transmissionMode ||
	
                        TX_MODE_4 == dlMIMOInfo_p->transmissionMode ||
                        /* + TM7_8 Changes Start */ 
                        ((TX_MODE_8 == dlMIMOInfo_p->transmissionMode) &&
                         (1 < cellParams_p->numOfTxAnteenas)))&&
                    /* - TM7_8 Changes End */ 
                    (dlCQIInfo_p->widebandCQICodeWordOne >=
                     dlCQIInfo_p->widebandCQICodeWordTwo))
                {
                    cqiValue = dlCQIInfo_p->widebandCQICodeWordTwo;
                 }
		   else
                {
                    cqiValue = dlCQIInfo_p->widebandCQICodeWordOne;
                 }
                  
	    dciFormat = dlStrategyTxNode_p->primaryDCIFormat;

            dciSize = getDCIBitSize( dciFormat,
                    cellParams_p->numOfTxAnteenas,
                    cellParams_p->dlAvailableRBs );

            dciSizeCategory = getDCISizeCategory(dciSize);

            aggregationLevelPowerOffset_p =
                &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                        pdcchAggregationAndPowerOffset[userLocationType].\
                        aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);

            aggregationLevel = aggregationLevelPowerOffset_p->
                aggregationPowerOffsetTuples[localCount].aggregationLevel;

            arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
            totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                  * contentionFactor_g[arrayIndex]);
        }else
        {
            dciFormat = dlStrategyTxNode_p->primaryDCIFormat;
            ueIndex   = dlStrategyTxNode_p->ueIndex;

            aggregationLevel =
                dciFormatToAggrLvlMapping_g[internalCellIndex].\
                dciFormatToAggregationLevel[userLocationType]\
                [dlStrategyTxNode_p->primaryDCIFormat];
            arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
            totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                  * contentionFactor_g[arrayIndex]);
        }
        *availableDLRBCount -= dlStrategyTxNode_p->resourceAllocInput.requiredRB;
        *maxUEsToBeScheduledDL -= 1;
        dlStrategyTxNode_p = (DLStrategyTxNode*)
            getNextNode((const LTE_SNODE *)dlStrategyTxNode_p);
    }
    return totalMinCCERequired;
}
/*********************************************************************************
 * Function Name : minimumCCERequiredDLReTx
 * Inputs        : availableDLRBCount - count of availables RB's in Dl,
 *                 maxUEsToBeScheduledDL - maximum number UE's can be scheduled in DL,
 *                 pdcchDLOutputInfo_p - Pointer to the PDCCH DL output info
 *                    which contatins the CCE and RB allocated info
 *                 currentGlobalTickWithDelay - current TTi with delay to PHY
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : totalMinCCERequired
 * Description   : This function will calculate the minimum number of CCEs required
 *                 for DL retransmission
 **********************************************************************************/
UInt8 minimumCCERequiredDLReTx(SInt32* availableDLRBCount,
                               UInt8* maxUEsToBeScheduledDL,
                               PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                               /* SPR 15909 fix start */
                               /* +- SPR 17777 */
#ifdef TDD_CONFIG
                               tickType_t currentGlobalTickWithDelay,
#endif
                               /* +- SPR 17777 */
                               /* SPR 15909 fix end */
                               InternalCellIndex internalCellIndex)
{
    UInt32 countDLUEReTx                 = 0;
    DLStrategyTxNode *dlStrategyTxNode   = PNULL;
    DLUEContextInfo *dlUEContextInfo_p   = PNULL;
    DLUEContext *dlUEContext_p           = PNULL;
    UInt8 userLocationType               = CC_USER;
    UInt16 ueIndex                       = 0;
    UInt8 totalMinCCERequired            = 0;
    UInt8 arrayIndex                     = 0;
    UInt32 cqiValue                      = 0;
    UInt32 dciSize                       = 0;
    UInt32 dciSizeCategory               = 0;
    UInt32 localCount                    = 0;
    DCIFormat  dciFormat                 = MAX_NUM_DCI_FORMAT;
    UInt32 aggregationLevel              = 0;
    AggregationPowerOffset *aggregationLevelPowerOffset_p = PNULL;
    DLMIMOInfo  *dlMIMOInfo_p=PNULL;
    DLCQIInfo *dlCQIInfo_p = PNULL;
 
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    countDLUEReTx  = sQueueCount(dlStrategyReTxQueue_gp[internalCellIndex] );
    dlStrategyTxNode = (DLStrategyTxNode*)
                    getFirstNode( dlStrategyReTxQueue_gp[internalCellIndex]);        

#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt32 subFrameNum =  currentGlobalTickWithDelay %
                                NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    UInt8 isSpecialSubFrame = checkForSpecialSubframe(subFrameNum, internalCellIndex);
    UInt8 harqProcessId = 255;
    DLHARQProcess harqProcess_p ;
#endif
    /* + SPR 11491 Changes */
    UInt8 isPdcchAllocatedFlag = FALSE;
    /* - SPR 11491 Changes */

    while ( countDLUEReTx-- && *maxUEsToBeScheduledDL && *availableDLRBCount)
    {
        if (PNULL == dlStrategyTxNode)
        {
	        dlStrategyTxNode = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode);
            continue;
        }
        ueIndex            =  dlStrategyTxNode->ueIndex;
        dlUEContextInfo_p  =  &dlUECtxInfoArr_g[ueIndex];
	/* Coverity CID 54504 start */
        /* code removed for coverity */
	/* Coverity CID 54504 end */
        if (dlUEContextInfo_p->pendingDeleteFlag)
        {
	        dlStrategyTxNode = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode);
            continue;
        }

        dlUEContext_p     = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

#ifdef TDD_CONFIG
        harqProcessId = dlStrategyTxNode->harqProcessId;
        harqProcess_p = dlUEContext_p->dlHarqContext_p->harqProcess[harqProcessId];
        if(DL == harqProcess_p.subFrameType)
        {
            if ((isSpecialSubFrame) &&
                    (RE_TX == dlStrategyTxNode->transmissionType ))
            {
                dlStrategyTxNode = (DLStrategyTxNode*)
                    getNextNode((const LTE_SNODE *)dlStrategyTxNode);
                continue;
            }
        }
        else if(SP == harqProcess_p.subFrameType)
        {
            if ((!isSpecialSubFrame) &&
                    (RE_TX == dlStrategyTxNode->transmissionType))
            {
	        dlStrategyTxNode = (DLStrategyTxNode*)
                    getNextNode((const LTE_SNODE *)dlStrategyTxNode);
                continue;
            }
        }
#endif
        /* + SPR 11491 Changes */
        if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
        {
            isPdcchAllocatedFlag = dlUEContext_p->isPdcchAllocatedFlag;
        }
        else
        {
            isPdcchAllocatedFlag = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isPdcchAllocatedFlag;
        }
        if (isPdcchAllocatedFlag)
        /* - SPR 11491 Changes */
        {
	    dlStrategyTxNode = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode);
            continue;
        }
        /*pdcch allocation For scell changes start*/

        dlMIMOInfo_p=getdlMIMOInfo(dlUEContext_p,internalCellIndex);
        dlCQIInfo_p=getdlCQIInfo(dlUEContext_p,internalCellIndex);
        if((dlMIMOInfo_p == PNULL) ||(dlCQIInfo_p == PNULL))
        {
            return MAC_FAILURE;
        }
        userLocationType = getUserLocationType(dlUEContext_p,internalCellIndex);
        /*pdcch allocation For scell changes end*/

        /* SPR 22252 fix start */
        if(((userLocationType == CE_USER)&&
                    (pdcchDLOutputInfo_p->countMsg[CE_USER] ==
                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     initParams_p->dlSchdConfig.maxCellEdgeUEsToBeScheduledDL))&&(((pdcchDLOutputInfo_p->countMsg[CC_USER] +
                             pdcchDLOutputInfo_p->countMsg[CE_USER]) <
                         cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         initParams_p->dlSchdConfig.maxUEsToBeScheduledDL )))
        {
            /* SPR 22579 Coverity Changes + */
            dlStrategyTxNode = (DLStrategyTxNode*)
                getNextNode((const LTE_SNODE *)dlStrategyTxNode);
            /* SPR 22579 Coverity Changes - */
            continue;
        }

        else if ((pdcchDLOutputInfo_p->countMsg[CC_USER] + 
                        pdcchDLOutputInfo_p->countMsg[CE_USER]) >= 
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    initParams_p->dlSchdConfig.maxUEsToBeScheduledDL )
        {
            return totalMinCCERequired;
        }
        /* SPR 22252 fix end */


     if (cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable &&
            dlCQIInfo_p->isWideBandReportAvailable)
        {
           if ((TX_MODE_3 == dlMIMOInfo_p->transmissionMode ||
                    TX_MODE_4 == dlMIMOInfo_p->transmissionMode ||
                    /* + TM7_8 Changes Start */ 
                    ((TX_MODE_8 == dlMIMOInfo_p->transmissionMode) &&
                     (1 < cellParams_p->numOfTxAnteenas)))&&
                     /* - TM7_8 Changes End */ 
                    (dlCQIInfo_p->widebandCQICodeWordOne >=
                     dlCQIInfo_p->widebandCQICodeWordTwo))
                {
                    cqiValue = dlCQIInfo_p->widebandCQICodeWordTwo;
                }else
                {
                    cqiValue = dlCQIInfo_p->widebandCQICodeWordOne;
                }
            	
	      
	     dciFormat = dlStrategyTxNode->primaryDCIFormat;

            dciSize = getDCIBitSize( dciFormat,
                    cellParams_p->numOfTxAnteenas,
                    cellParams_p->dlAvailableRBs );

            dciSizeCategory = getDCISizeCategory(dciSize);

            aggregationLevelPowerOffset_p =
                &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                        pdcchAggregationAndPowerOffset[userLocationType].\
                        aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);

            aggregationLevel = aggregationLevelPowerOffset_p->
                    aggregationPowerOffsetTuples[localCount].aggregationLevel;
            arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
            totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                        ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                         * contentionFactor_g[arrayIndex]);
        }else
        {
            dciFormat = dlStrategyTxNode->primaryDCIFormat;
            ueIndex   = dlStrategyTxNode->ueIndex;

            aggregationLevel = dciFormatToAggrLvlMapping_g[internalCellIndex].\
                               dciFormatToAggregationLevel[userLocationType]\
                               [dlStrategyTxNode->primaryDCIFormat];
            arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
            totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                     ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                     * contentionFactor_g[arrayIndex]);
        }
        *availableDLRBCount -= dlStrategyTxNode->resourceAllocInput.requiredRB;
        *maxUEsToBeScheduledDL -= 1;
        dlStrategyTxNode = (DLStrategyTxNode*)
            getNextNode((const LTE_SNODE *) dlStrategyTxNode);
    }
    return totalMinCCERequired;
} 

#ifndef DL_UL_SPLIT_TDD
/*********************************************************************************
 * Function Name : calculateCCERequiredInULNewTx
 * Inputs        : availableULRBCount - count of availables RB's in UL,
 *                 maxUEsToBeScheduledUL - maximum number UE's can be scheduled in UL,
 *                 pdcchDLOutputInfo_p - Pointer to the PDCCH DL output info
 *                    which contatins the CCE and RB allocated info
 *                 currentGlobalTickWithDelay - current TTi with delay to PHY
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : totalMinCCERequired
 * Description   : This function will calculate the minimum number of CCEs required
 *                 for UL new transmission
 **********************************************************************************/
UInt8 calculateCCERequiredInULNewTx(SInt32* availableULRBCount,
                                    UInt8* maxUEsToBeScheduledUL,
                                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                    PdcchULOutputInfo *pdcchULOutputInfo_p,
#endif
                                    /* +- SPR 17777 */
                                    /* +- SPR 17777 */
                                    InternalCellIndex internalCellIndex)
{
#ifndef DL_UL_SPLIT
    UInt32 countULUETx                 = 0;
#endif
    UInt16 ueIndex                     = 0;
    ULStrategyTxNode *ulStrategyTxNode = PNULL;
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
    ULUEContext* ulUEContext_p         = PNULL;
    UInt32 cqiValue                    = 0;
    UInt32 dciSize                     = 0;
    UInt32 dciSizeCategory             = 0;
    DCIFormat  dciFormat               = MAX_NUM_DCI_FORMAT;
    UInt32 localCount                  = 0;
    UInt32 aggregationLevel            = INVALID_AGGREGATION_LEVEL;
    UInt8 totalMinCCERequired          = 0;
    UInt8 arrayIndex                   = 0;
#ifdef TDD_CONFIG
    UeScheduledInfo *ueScheduledInfo_p = PNULL;
#endif
    AggregationPowerOffset *aggregationLevelPowerOffset_p = PNULL;

    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* SPR 5785 fix start */            
#ifdef DL_UL_SPLIT
#ifdef FDD_CONFIG
    UInt32 availableCCELocal      = maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex]-1];
#endif
#endif
    /* SPR 5785 fix end */            

#ifndef DL_UL_SPLIT
    countULUETx =  sQueueCount(ulStrategyTxQueue_gp[internalCellIndex]);
    ulStrategyTxNode = (ULStrategyTxNode*)
        getFirstNode(ulStrategyTxQueue_gp[internalCellIndex]);
#else
#ifdef CIRC_QUEUE_IMPL
    semWaitDefault(&waitForULStrategyTxInd_g[internalCellIndex]);
    DEQUEUE_UL_STRATEGY_TX_CIRC_Q(ULStrategyTxNode, (void *)&ulStrategyTxNode, internalCellIndex);
#else
    /* +- SPR 17777 */
    recvMsgWithIccQ(ulStrategyTxQueueIdDL_g[internalCellIndex], &ulStrategyTxNode);
    /*CA Changes end JAVED */
#endif
#endif


#ifdef DL_UL_SPLIT
    while (1)
#else
        while ( countULUETx-- && *availableULRBCount && *maxUEsToBeScheduledUL)
#endif
        {
            if(ulStrategyTxNode == PNULL)
            {
#ifndef DL_UL_SPLIT
                ulStrategyTxNode = (ULStrategyTxNode*)
                    getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
#ifdef CIRC_QUEUE_IMPL
                semWaitDefault(&waitForULStrategyTxInd_g[internalCellIndex]);
                DEQUEUE_UL_STRATEGY_TX_CIRC_Q(ULStrategyTxNode, (void *)&ulStrategyTxNode, internalCellIndex);
#else
                /* +- SPR 17777 */
                recvMsgWithIccQ(ulStrategyTxQueueIdDL_g[internalCellIndex], &ulStrategyTxNode);
#endif
#endif
                if(PNULL == ulStrategyTxNode)
                {
                    continue;
                }
            }
#ifdef DL_UL_SPLIT
            if (1 == pushNode(ulStrategyNewTxQueue_gp[internalCellIndex],
                        &(ulStrategyTxNode->ulStrategyTxNodeAnchor)))
            {

                freeMemPool(ulStrategyTxNode);
                return MAC_FAILURE;
            }
#endif
            ueIndex            = ulStrategyTxNode->ueIndex;     
#ifdef DL_UL_SPLIT
            if (ueIndex == INVALID_UE_INDEX)
            {
                break;
            }
#endif
            ulUEContextInfo_p  = &ulUECtxInfoArr_g[ueIndex];
            /* Coverity CID 54502 start */
            /* Code removed for coverity */
            /* Coverity CID 54502 start */
            if (ulUEContextInfo_p->pendingDeleteFlag)
            {
#ifndef DL_UL_SPLIT
                ulStrategyTxNode = (ULStrategyTxNode*)
                    getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
                ulStrategyTxNode = PNULL;
#endif
                continue;
            }
            ulUEContext_p     = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
#ifdef TDD_CONFIG
            /** SPR 14204 Fix : Start **/
            ueScheduledInfo_p =
                ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulStrategyTxNode->ulSubFrameNum];
            /** SPR 14204 Fix : End **/
            if ( ueScheduledInfo_p == PNULL)
            {
#ifndef DL_UL_SPLIT
                ulStrategyTxNode = (ULStrategyTxNode*)
                    getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
                ulStrategyTxNode = PNULL;
#endif
                continue;
            }

#endif
#ifdef TDD_CONFIG
            if (ueScheduledInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
#elif FDD_CONFIG
                if ( pdcchULOutputInfo_p->directIndexingUEInfo[
                        ulStrategyTxNode->ueIndex].flag ==
                        RECEIVED_NACK_RE_TRANS_COMPELETE)
#endif
                {
#ifndef DL_UL_SPLIT
                    ulStrategyTxNode = (ULStrategyTxNode*)
                        getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
                    ulStrategyTxNode = PNULL;
#endif
                    continue;
                }
            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
            switch ( ulUEContext_p->ulresInfo[ulStrategyTxNode->ulHarqProcessId].isPdcchAllocated )
#elif FDD_CONFIG 
            switch ( ulUEContext_p->isPdcchAllocated )
#endif
            /*TDD Config 0 Changes End*/
            {
                case UL_NON_ADAPTIVE_NO_CCE:
                case UL_ADAPTIVE_CCE:
#ifndef DL_UL_SPLIT
                    ulStrategyTxNode = (ULStrategyTxNode*)
                        getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
                    ulStrategyTxNode = PNULL;
#endif
                    continue;
                default:
                    {
                    }
            }
#ifdef FDD_CONFIG
            if (UL_SPS_OCCASION == ulStrategyTxNode->ulGrantRequestType )
            {
#ifndef DL_UL_SPLIT
                ulStrategyTxNode = (ULStrategyTxNode*)
                    getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
                ulStrategyTxNode = PNULL;
#endif
                continue;
            }
#endif

#ifdef DL_UL_SPLIT
            /* SPR 5785 fix start */            
            if ( !(availableCCELocal && *availableULRBCount && *maxUEsToBeScheduledUL) )
            {
                ulStrategyTxNode = PNULL;
                continue;
            }
            /* SPR 5785 fix end */            
#endif

           if (cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable &&
                    ulUEContext_p->isDLWideBandCQIReportAvailable)
            {
               
                if ((ulUEContext_p->dlWidebandCQICodeWordTwo != INVALID_CQI_VALUE)&&
                        (ulUEContext_p->dlWidebandCQICodeWordOne >=
                         ulUEContext_p->dlWidebandCQICodeWordTwo))
                {
                    cqiValue = ulUEContext_p->dlWidebandCQICodeWordTwo;
                }
                else
                {
                    cqiValue = ulUEContext_p->dlWidebandCQICodeWordOne;
                }

      
       	    dciFormat = DCI_FORMAT_0;

                /*Wideband CQI is available, aggregation level will be decided using
                 * DCISize*/
                dciSize =
                    getDCIBitSize( dciFormat,
                            cellParams_p->numOfTxAnteenas,
                            cellParams_p->dlAvailableRBs);

                /*Now find if this DCI size lies in big/small/medium DCI size category.
                 * This result will be used to find the aggregation level*/
                dciSizeCategory = getDCISizeCategory(dciSize);
                /* ICIC changes start*/
                aggregationLevelPowerOffset_p =
                    &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->pdcchAggregationAndPowerOffset \
                            [ulUEContext_p->userLocationType].aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);
                /* ICIC changes end */
                /*The static power control table returned valid values.*/

                aggregationLevel = aggregationLevelPowerOffset_p->
                    aggregationPowerOffsetTuples[localCount].aggregationLevel;
                arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
                totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                    ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                      * contentionFactor_g[arrayIndex]);
            }
            else
            {
                aggregationLevel = dciFormatToAggrLvlMapping_g[internalCellIndex].\
                                   dciFormatToAggregationLevel\
                                   [ulUEContext_p->userLocationType][DCI_FORMAT_0];
                arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
                totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                    ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                      * contentionFactor_g[arrayIndex]);
            }
            *availableULRBCount -= ulStrategyTxNode->requiredRB;
            *maxUEsToBeScheduledUL -= 1;
#ifndef DL_UL_SPLIT
            ulStrategyTxNode = (ULStrategyTxNode*)
                getNextNode((const LTE_SNODE *)ulStrategyTxNode);
#else
            ulStrategyTxNode = PNULL;
#endif

        }
    return totalMinCCERequired;
} 
/*********************************************************************************
 * Function Name : calculateCCERequiredInULReTx
 * Inputs        : currentGlobalTickWithDelay - current Tick with Delay to PHY
 *                 availableULRBCount - count of availables RB's in UL,
 *                 maxUEsToBeScheduledUL - maximum number UE's can be scheduled in UL,
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : totalMinCCERequired
 * Description   : This function will calculate the minimum number of CCEs required
 *                 for UL retransmission
 **********************************************************************************/
				  /* SPR 15909 fix start */
UInt8 calculateCCERequiredInULReTx(tickType_t currentGlobalTickWithDelay,
			           /* SPR 15909 fix end */
                                   SInt32* availableULRBCount,
                                   UInt8* maxUEsToBeScheduledUL,
                                   InternalCellIndex internalCellIndex)
{
#ifndef DL_UL_SPLIT
    UInt16 nackQueueCount                  = 0;
#endif
    UplinkContainer* ulContainerQueue_p    = PNULL;
    UInt8  nackQContainerTick              = 0;
    ContainerNackQueueNode * nackNode_p    = PNULL;
    UInt16 tempUeIndex                     = 0;
    /* SPR 15909 fix start */
    tickType_t tempPhyTti                  = 0;
    /* SPR 15909 fix end */
    UInt8 harqIndex                        = INVALID_HARQ_ID;
    UInt8 arrayIndex                       = 0;
    ULUEContext* ulUEContext_p             = PNULL;
    /* + Coverity_Stack Fix */
    ULHarqInfo *tempULHarqInfo_p = PNULL;
    /* - Coverity_Stack Fix */
    UInt32 cqiValue                        = 0;
    UInt32 dciSize                         = 0;
    UInt32 dciSizeCategory                 = 0;
    DCIFormat  dciFormat                   = MAX_NUM_DCI_FORMAT;
    UInt32 localCount                      = 0;
    UInt32 aggregationLevel                = INVALID_AGGREGATION_LEVEL;
    UInt8 totalMinCCERequired              = 0;
    AggregationPowerOffset *aggregationLevelPowerOffset_p = PNULL;
#ifdef TDD_CONFIG
    UInt8  packetSubFrameNum               = 0;
    UInt8  hiPduSubFrameNum                = 0;
    UInt32 subFrameNum                     = 0;
    UeScheduledInfo *ueScheduledInfo_p     = PNULL;
    /* TDD Config 6 Changes Start */
    UInt16 packetSysFrameNum               = 0;
    /* TDD Config 6 Changes End */
#endif
#ifdef FDD_CONFIG
    UInt8  maxUlHARQTx                     = 0;
#endif

    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

#ifdef FDD_CONFIG
    nackQContainerTick = MODULO_EIGHT(currentGlobalTickWithDelay - 4);
#else
    nackQContainerTick = (currentGlobalTickWithDelay)% getMaxULCrcContainer(internalCellIndex);
#endif
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;

#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
    semWaitDefault(&waitForULNackQueue_g[internalCellIndex]);
    DEQUEUE_NACK_CIRC_Q(&(ulContainerQueue_p->nackCircQueue), 
    ContainerNackQueueNode, (void *)&nackNode_p);
#else
    /* +- SPR 17777 */
    recvMsgWithIccQ(ulContainerQueue_p->ulContainerQIdDL, &nackNode_p);
#endif
#else
    nackQueueCount             = sQueueCount(&(ulContainerQueue_p->nackQueue));
    nackNode_p = (ContainerNackQueueNode *)getFirstNode(&(ulContainerQueue_p->nackQueue));
#endif

#ifdef DL_UL_SPLIT
    while(1)
    {
#else 
    while(nackQueueCount-- && *availableULRBCount && *maxUEsToBeScheduledUL)
    {
#endif
#ifdef FDD_CONFIG
        if (NULL == nackNode_p)
	{ 
#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
		semWaitDefault(&waitForULNackQueue_g[internalCellIndex]);
		DEQUEUE_NACK_CIRC_Q(&(ulContainerQueue_p->nackCircQueue), 
        ContainerNackQueueNode, (void *)&nackNode_p);
#else
        /* +- SPR 17777 */
		recvMsgWithIccQ(ulContainerQueue_p->ulContainerQIdDL, &nackNode_p);
#endif
#else
		nackNode_p = (ContainerNackQueueNode *)
			getNextNode((const LTE_SNODE *)nackNode_p);
#endif
		if(PNULL == nackNode_p)
		{
			continue;
		}
	}
#ifdef DL_UL_SPLIT
        if (1 == pushNode(ulStrategyReTxQueue_gp[internalCellIndex],
                &(nackNode_p->nackNodeAnchor)))
        {
            FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
            return MAC_FAILURE;
        }
#endif
        tempUeIndex =  nackNode_p->ueIdx;
        if( tempUeIndex == INVALID_UE_INDEX)
        {
#ifndef DL_UL_SPLIT
            nackNode_p = (ContainerNackQueueNode *)
                       getNextNode((const LTE_SNODE *)nackNode_p);
            continue;
#else
            break;
#endif
        }
        tempPhyTti         = nackNode_p->ttiCounter;
        harqIndex          = MODULO_EIGHT(tempPhyTti);

        if (!ulUECtxInfoArr_g[tempUeIndex].pendingDeleteFlag)
        {
            ulUEContext_p    = ulUECtxInfoArr_g[tempUeIndex].ulUEContext_p;
            tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
                    /* + SPS_TDD_Changes */
            if((HARQ_PROCESS_RESET == tempULHarqInfo_p->harqStatus)||
               (SPS_SCHEDULING == ulUEContext_p->schType) || (SPS_SCHEDULING == nackNode_p->schUsed))
                    /* - SPS_TDD_Changes */
            {
#ifndef DL_UL_SPLIT
                nackNode_p = (ContainerNackQueueNode *)
                       getNextNode((const LTE_SNODE *)nackNode_p);
#else
            nackNode_p = PNULL;
#endif
                continue;
            }
            if ((currentGlobalTickWithDelay <= (tempPhyTti + FDD_HARQ_OFFSET))&&
                 tempULHarqInfo_p->riLength)
            {

                /* +- SPR 17669*/
                /* + SPS_TDD_Changes */
                maxUlHARQTx = ulUEContext_p->maxUlHARQTx -1;
                /* - SPS_TDD_Changes */
                if (tempULHarqInfo_p->currentRetransCount < maxUlHARQTx)
                {
                   /*CSR 00055881 Start*/                               
                   /* ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType = ADAPTIVE_RE_TX; 
                    if (ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType == \
                            ADAPTIVE_RE_TX)
                   */
                   /*CSR 00055881 End*/                               
                        if (cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable &&
                             ulUEContext_p->isDLWideBandCQIReportAvailable && 
                             (ulUEContext_p->dlWidebandCQICodeWordOne != INVALID_CQI_VALUE))
                        {

                                if ((ulUEContext_p->dlWidebandCQICodeWordTwo != INVALID_CQI_VALUE)&&
                                (ulUEContext_p->dlWidebandCQICodeWordOne >=
                                ulUEContext_p->dlWidebandCQICodeWordTwo))
                                {
                                    cqiValue = ulUEContext_p->dlWidebandCQICodeWordTwo;
                                }
                                else
                                {
                                    cqiValue = ulUEContext_p->dlWidebandCQICodeWordOne;
                                }


			       	dciFormat = DCI_FORMAT_0;

                            /*Wideband CQI is available, aggregation level will be decided using
                            * DCISize*/
                            dciSize =
                            getDCIBitSize( dciFormat,
                                cellParams_p->numOfTxAnteenas,
                                cellParams_p->dlAvailableRBs);

                            /*Now find if this DCI size lies in big/small/medium DCI size category.
                            * This result will be used to find the aggregation level*/
                            dciSizeCategory = getDCISizeCategory(dciSize);
                            /* ICIC changes start*/
                            aggregationLevelPowerOffset_p =
                               &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->pdcchAggregationAndPowerOffset\
                               [ulUEContext_p->userLocationType].aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);
                           /* ICIC changes end */
                           /*The static power control table returned valid values.*/
                           aggregationLevel = aggregationLevelPowerOffset_p->
                           aggregationPowerOffsetTuples[localCount].aggregationLevel;
                           arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
                           totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                               ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                                   * contentionFactor_g[arrayIndex]);
                        }else
                        {
                            aggregationLevel = dciFormatToAggrLvlMapping_g[internalCellIndex].\
                                               dciFormatToAggregationLevel\
                                               [ulUEContext_p->userLocationType][DCI_FORMAT_0];
                            arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
                            totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                                 ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                                     * contentionFactor_g[arrayIndex]);
                        }
                        *availableULRBCount -= tempULHarqInfo_p->riLength;
                        *maxUEsToBeScheduledUL -= 1;
                }
            }
        }
#ifndef DL_UL_SPLIT
        nackNode_p = (ContainerNackQueueNode *)
              getNextNode((const LTE_SNODE *)nackNode_p); 
#else
        nackNode_p = PNULL;
#endif
#else
        if(PNULL != nackNode_p)
        {
            tempUeIndex =  nackNode_p->ueIdx;
            tempPhyTti = nackNode_p->ttiCounter;
            if (!ulUECtxInfoArr_g[tempUeIndex].pendingDeleteFlag)
            {
                ulUEContext_p = ulUECtxInfoArr_g[tempUeIndex].ulUEContext_p;
                /* TDD Config 6 Changes Start */
                GET_SFN_SF_FROM_TTI(tempPhyTti, packetSysFrameNum, packetSubFrameNum)
                harqIndex      = getULHarqIDFromSubFrame (packetSubFrameNum, ulUEContext_p, internalCellIndex, packetSysFrameNum);
                /* TDD Config 6 Changes End */
                if ( INVALID_HARQ_ID == harqIndex )
                {
                    nackNode_p = (ContainerNackQueueNode *)
                        getNextNode((const LTE_SNODE *)nackNode_p);
                    continue;
                }
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
                if(!tempULHarqInfo_p->riLength)
                {
                    nackNode_p = (ContainerNackQueueNode *)
                       getNextNode((const LTE_SNODE *)nackNode_p);
                    continue;
                }else
                {
                    if (! isValidULSubFrameForHIPdu ( packetSubFrameNum, &hiPduSubFrameNum, internalCellIndex ))
                    {
                        nackNode_p = (ContainerNackQueueNode *)
                           getNextNode((const LTE_SNODE *)nackNode_p);
                        continue;
                    }
                    subFrameNum =  currentGlobalTickWithDelay %
                                    NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
                    if (!isValidSubframeNumForDCI_0 (subFrameNum, internalCellIndex))
                    {
                        nackNode_p = (ContainerNackQueueNode *)
                           getNextNode((const LTE_SNODE *)nackNode_p);
                        continue;
                    }
                    /** SPR 14204 Fix : Start **/
                    ueScheduledInfo_p = ulUEContext_p->
                        ulUeScheduledInfoPointerArray[internalCellIndex][nackNode_p->ulSubFrame];
                    /** SPR 14204 Fix : End **/
                    if ( PNULL == ueScheduledInfo_p )
                    {
                        nackNode_p = (ContainerNackQueueNode *)
                           getNextNode((const LTE_SNODE *)nackNode_p);
                        continue;
                    }
        ulUEContext_p->ulresInfo[harqIndex].nextReTransmissionType = ADAPTIVE_RE_TX;
                                            if (cellParams_p->powerControlEnableInfo.pdcchPowOrAggregationEnable &&
                    ulUEContext_p->isDLWideBandCQIReportAvailable)
                    {						    
                        if ((ulUEContext_p->dlWidebandCQICodeWordTwo != INVALID_CQI_VALUE)&&
                            (ulUEContext_p->dlWidebandCQICodeWordOne >=
                            ulUEContext_p->dlWidebandCQICodeWordTwo))
                            {
                                cqiValue = ulUEContext_p->dlWidebandCQICodeWordTwo;
                            }
                            else
                           {
                                cqiValue = ulUEContext_p->dlWidebandCQICodeWordOne;
                            }
                      
                        dciFormat = DCI_FORMAT_0;

                        /*Wideband CQI is available, aggregation level will be decided using
                        * DCISize*/
                        dciSize =
                        getDCIBitSize( dciFormat,
                             cellParams_p->numOfTxAnteenas,
                             cellParams_p->dlAvailableRBs);

                        /*Now find if this DCI size lies in big/small/medium DCI size category.
                        * This result will be used to find the aggregation level*/
                        dciSizeCategory = getDCISizeCategory(dciSize);
                        /* ICIC changes start*/
                        aggregationLevelPowerOffset_p =
                          &((macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->pdcchAggregationAndPowerOffset\
                           [ulUEContext_p->userLocationType].aggregationPowerOffset[dciSizeCategory][cqiValue - 1]);
                        /* ICIC changes end */
                        /*The static power control table returned valid values.*/
                        aggregationLevel = aggregationLevelPowerOffset_p->
                        aggregationPowerOffsetTuples[localCount].aggregationLevel;
                        arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
                        totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                               ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                                   * contentionFactor_g[arrayIndex]);
                        }else
                        {
                            aggregationLevel = dciFormatToAggrLvlMapping_g[internalCellIndex].\
                                               dciFormatToAggregationLevel\
                                               [ulUEContext_p->userLocationType][DCI_FORMAT_0];
                            arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);
                            totalMinCCERequired += aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                                 ( rrmMacCellGlobal_gp[internalCellIndex]->cceAdjustFactor[arrayIndex]
                                     * contentionFactor_g[arrayIndex]);
                        }
                        *availableULRBCount -= tempULHarqInfo_p->riLength;
                        *maxUEsToBeScheduledUL -= 1;
                }
            }
        }    
        nackNode_p = (ContainerNackQueueNode *)
        getNextNode((const LTE_SNODE *)nackNode_p);           
#endif
    }
    return totalMinCCERequired;
}
#endif
/*********************************************************************************
 * Function Name : getArrayIndexFromAggregationLevel
 * Inputs        : aggregationLevel
 * Outputs       : None
 * Returns       : index
 * Description   : This function will return the aggregationLevel index 
 *                 based on aggregationLevel
 **********************************************************************************/
UInt8 getArrayIndexFromAggregationLevel(UInt8 aggregationLevel)
{
    /*SPR 21948 Changes Start */
    /* Changes under spr 21762 have been reverted */
    UInt8 index = 0;
    if (AGGREGATION_LEVEL_ONE == aggregationLevel)
    {
        index = AL1;
    }
    if (AGGREGATION_LEVEL_TWO == aggregationLevel)
    {
        index = AL2;
    }
    if (AGGREGATION_LEVEL_FOUR == aggregationLevel)
    {
        index = AL4;
    }
    if (AGGREGATION_LEVEL_EIGHT == aggregationLevel)
    {
        index = AL8;
    }
    return index;
    /*SPR 21948 Changes End */
}
/*END   : DYN_PDCCH*/

#ifdef DL_UL_SPLIT_TDD
UInt8 calculateCCERequiredInULTx()
{
    UInt8 arrayIndex       =0;
    UInt8 aggregationLevel =AGGREGATION_LEVEL_TWO;
    UInt8 totalMinCCERequired=0;

    arrayIndex = getArrayIndexFromAggregationLevel(aggregationLevel);

    totalMinCCERequired = ulSchdConfig_g.maxUEsToBeScheduledUL 
        *(aggregationLevel/*requiredCCE[aggregationLevel]*/ +
                ( cceAdjustFactor_g[arrayIndex]
                  * contentionFactor_g[arrayIndex]));
    return totalMinCCERequired; 
}

#endif
/* SPR 6856 Changes start */
/******************************************************************************
 * Function Name: allocatePdcchForPDCCHOrderMsg
 * Inputs       : dlUEContext_p -
 *                ueSearchSpaceInfo_p: Info relatd to UE space allocation
 *                dciCCEInfo_p: This is pointer to structure on which Free
 *                             CCE Information is there and once cce is
 *                             allocated, updated information is written
 *                             on the reserved CCE.
 *                dciCCEContainerInfo_p -
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : allocatedCCEIndex_p - CCE index which is allocated 
 *                availableCCE_p - Available CCE in UE/common specific space
 * Returns      : SUCCESS/FAILURE
 * Description  : This function allocates the PDCCH/ CCE resources for 
 *                PDCCH order messages from UE/Common search space
 ******************************************************************************/
MacRetType allocatePdcchForPDCCHOrderMsg(
        DLUEContext *dlUEContext_p, 
        UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p,
        UInt32 *allocatedCCEIndex_p,
        UInt32 *availableCCE_p,
        DciCCEInfo *dciCCEInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex)
{
	MacRetType macRetType = MAC_FAILURE; /* Return value */
	UInt32 aggregationLevel = AGGREGATION_LEVEL_FOR_PDCCH_ORDER_MSG;

	/* First search the CCE resources in UE space */
    macRetType = allocatePDCCHForDCIFormatUESpace(
            dlUEContext_p,
            ueSearchSpaceInfo_p,
            allocatedCCEIndex_p,
            availableCCE_p,
            dciCCEInfo_p,
            aggregationLevel,
            dciCCEContainerInfo_p,
            internalCellIndex);

    /* if CCE not available in UE search space, try allocating in common search space */
	if (MAC_SUCCESS != macRetType)
	{
              macRetType= allocatePDCCHForDCIFormatCommonSpace (
			  	             aggregationLevel,allocatedCCEIndex_p, 
			  	             availableCCE_p, dciCCEInfo_p,
                             internalCellIndex);
	}

	if (MAC_SUCCESS != macRetType)
	{
		LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
				"[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
				__func__,aggregationLevel);
	}

	return macRetType;
}
/* SPR 6856 Changes end */
/*********************************************************************************
 * Function Name :    resetAllocatedCCEs
 * Inputs:            dciCCEContainerInfo_p - pointer to CCE container
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs:           
 * Description:       It reset all the allocated CCE before use. 
 **********************************************************************************/
 /** SPR 13599 Changes Start **/
 void resetAllocatedCCEs(DciCCEContainerInfo *dciCCEContainerInfo_p,
        InternalCellIndex internalCellIndex)
 /** SPR 13599 Changes End **/
{
    UInt8 cceCount=0;
    UInt8 cceNumber=0;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;

    currentCFIValue_g[internalCellIndex] = dciCCEContainerInfo_p->CFIValueForThisContainer; 
    /* + TDD - Reset CCE Changes */
#ifdef TDD_CONFIG
    /** CA-TDD Changes Start **/
    miValueForSubframe_g[internalCellIndex] = dciCCEContainerInfo_p->MIValueForThisContainer; 
    /** CA-TDD Changes Stop **/
#endif
    /* - TDD - Reset CCE Changes */

    if (dciCCEContainerInfo_p->mibData_p)
    {
        msgFree(dciCCEContainerInfo_p->mibData_p);
        dciCCEContainerInfo_p->mibData_p = PNULL; 
    }
    /* + SPR 22354 */ 
    cceCount = dciCCEContainerInfo_p->countOfCCEAllocated;
    while(cceCount--)
    {
        cceNumber = dciCCEContainerInfo_p->arrayCCEAllocated[cceCount];
        tempDciCCEInfo_p = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);

        if ( (tempDciCCEInfo_p->dciPduInfo.aggregationLvl) && (tempDciCCEInfo_p->isCCEFree == 0) )
        {
                mac_reset_cce(tempDciCCEInfo_p->dciPduInfo.aggregationLvl,tempDciCCEInfo_p,
                    internalCellIndex);

            /* Rel 5.3: Coverity 40775 Fix Start */
            tempDciCCEInfo_p->msgType=INVALID_MSG;
            /* Rel 5.3: Coverity 40775 Fix End */
            tempDciCCEInfo_p->dciPduInfo.harqProcessId = INVALID_HARQ_ID;
        }
    }
    /*pdcch for scell changes start*/
     resetExtraCCEsAllocated( dciCCEContainerInfo_p,internalCellIndex);
    /*pdcch for scell changes end*/
    cceCount = dciCCEContainerInfo_p->countCmnChannelMsg;
    while(cceCount--)
    {
        cceNumber = dciCCEContainerInfo_p->dciCCEInfoCmnChannel[cceCount].cceIndex;

        tempDciCCEInfo_p = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);

        if ( (tempDciCCEInfo_p->dciPduInfo.aggregationLvl) && (tempDciCCEInfo_p->isCCEFree == 0) )
        {
            mac_reset_cce(tempDciCCEInfo_p->dciPduInfo.aggregationLvl,tempDciCCEInfo_p,
                    internalCellIndex);
            /* - SPR 22354 */ 
            /* Rel 5.3: Coverity 40775 Fix Start */
            tempDciCCEInfo_p->msgType=INVALID_MSG;
            /* Rel 5.3: Coverity 40775 Fix End */
            tempDciCCEInfo_p->dciPduInfo.harqProcessId = INVALID_HARQ_ID;
        }
    } 
    dciCCEContainerInfo_p->countOfCCEAllocated = 0;
    dciCCEContainerInfo_p->countCmnChannelMsg = 0;
}
/*pdcch for scell changes start*/
/*********************************************************************************
 * Function Name :    resetExtraCCEsAllocated
 * Inputs:            dciCCEContainerInfo_p - pointer to CCE container
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs:           
 * Description:       It reset all the allocated CCE before use. 
 **********************************************************************************/
/* spr 23717 fix + */
 void resetExtraCCEsAllocated( DciCCEContainerInfo *dciCCEContainerInfo_p,
/* spr 23717 fix - */
        InternalCellIndex internalCellIndex)
{
   
    UInt8 cceCount=0;
    UInt8 cceNumber=0;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    cceCount = dciCCEContainerInfo_p->countOfExtraCCEAllocated;
    if(IS_CA_ENABLED())
    {
    while(cceCount--)
    {
        cceNumber = dciCCEContainerInfo_p->arrayExtraCCEAllocated[cceCount];
        tempDciCCEInfo_p = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);

        if ( (tempDciCCEInfo_p->dciPduInfo.aggregationLvl) && (tempDciCCEInfo_p->isCCEFree == CCE_ALLOCATED) )
        {
            mac_reset_cce(tempDciCCEInfo_p->dciPduInfo.aggregationLvl, tempDciCCEInfo_p,
                    internalCellIndex);
            /* Rel 5.3: Coverity 40775 Fix Start */
            tempDciCCEInfo_p->msgType=INVALID_MSG;
            /* Rel 5.3: Coverity 40775 Fix End */
            tempDciCCEInfo_p->dciPduInfo.harqProcessId = INVALID_HARQ_ID;
        }
    }
    dciCCEContainerInfo_p->countOfExtraCCEAllocated = 0;
    }
}
/*pdcch for scell changes end*/

/*pdcch changes start*/
/*********************************************************************************
 * Function Name :    getDLUEContextSearchSpaceInfo
 * Inputs:             - pointer to CCE container
 *                     - subFrameNum
 *                     - internalCellIndex - Cell-Index at MAC
 * Outputs:           
 * Description:       This function will be used to find whether uespecific information 
 *                     needs to fetched from primary or secondary context of cell
 **********************************************************************************/
UESpecificSearchSpaceInfo * getDLUEContextSearchSpaceInfo(DLUEContext *dlUEContext_p,
        UInt32 subFrameNum,InternalCellIndex internalCellIndex)
{
   
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p=PNULL;

    if(internalCellIndex == dlUEContext_p->internalCellIndex)
    {
        ueSearchSpaceInfo_p=&dlUEContext_p->ueSearchSpaceInfo_p[subFrameNum];
    }
    else
    {

        if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
        {
            ueSearchSpaceInfo_p=&dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ueSearchSpaceInfo_p[subFrameNum];
        }
    }

    return(ueSearchSpaceInfo_p);
}
/*********************************************************************************
 * Function Name :    getULUEContextSearchSpaceInfo
 * Inputs:             - pointer to CCE container
 *                     internalCellIndex - Cell-Index at MAC
 * Outputs:          ueSearchSpaceInfo_p-pointer to ue specific search
 *                    space to be considered 
 * Description:       This function will be used to find whether uespecific information 
 *                     needs to fetched from primary or secondary context of cell
 **********************************************************************************/
UESpecificSearchSpaceInfo * getULUEContextSearchSpaceInfo(ULUEContext *ulUEContext_p,
        UInt32 subFrameNum,InternalCellIndex internalCellIndex)
{
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p=PNULL;
    if(internalCellIndex == ulUEContext_p->internalCellIndex)
    {
        ueSearchSpaceInfo_p=&ulUEContext_p->ueSearchSpaceInfo_p[subFrameNum];
    }
    else
    {
        if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX])
        {
            ueSearchSpaceInfo_p=&ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->ueSearchSpaceInfo_p[subFrameNum];
        }
    }
    return(ueSearchSpaceInfo_p);
}

/*pdcch changes end*/
/*********************************************************************************
 * Function Name :    getdlMIMOInfo
 * Inputs:             dlUEContext_p - pointer to DL UE Context
 *                     internalCellIndex - Cell-Index at MAC
 * Outputs:           dlMIMOInfo_p-pointer to MIMO Information to be considered 
 *                     for aggregation level selection
 * Description:       This function will be used to find 
 **********************************************************************************/
/*pdcch allocation For scell changes start*/
DLMIMOInfo *getdlMIMOInfo( DLUEContext *dlUEContext_p,
		InternalCellIndex internalCellIndex)
{

    DLMIMOInfo *dlMIMOInfo_p = PNULL;
    if(internalCellIndex == dlUEContext_p->internalCellIndex)
    {
        dlMIMOInfo_p = &dlUEContext_p->dlMIMOInfo;
    }
    else 
    {

        if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
        {
            if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex==internalCellIndex)
            {

                dlMIMOInfo_p = &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo;
            }
        }

    }
    return dlMIMOInfo_p;

}
/*********************************************************************************
 * Function Name :    getUserLocationType
 * Inputs:            dlUEContext_p - pointer to DL UE Context
 *                    internalCellIndex -
 * Outputs:           userlocationtype
 * Description:       This function will be used to find whether userlocationtype 
 *                     needs to fetched from primary or secondary context of cell
 **********************************************************************************/
 UInt8 getUserLocationType( DLUEContext *dlUEContext_p,
		InternalCellIndex internalCellIndex)
{

    UInt8 userLocationType=0;
    /*kolckwork warning fix */
    if(internalCellIndex == dlUEContext_p->internalCellIndex && dlUEContext_p->userLocationType < MAX_USER_LOCATION)
    {
        userLocationType=dlUEContext_p->userLocationType;
    }
    else 
    {

        if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
        {
            if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex==internalCellIndex &&
                  dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType < MAX_USER_LOCATION)
            {

                userLocationType=dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
            }
        }

    }
    return userLocationType ;

}

/*pdcch allocation For scell changes end*/

/* Cyclomatic_complexity_changes_start */
#ifdef FDD_CONFIG
/*********************************************************************************
 * Function Name :    setCurrentCFIValueInPDCCH
 * Inputs:            cellParams_p
 *                    maxCommonSpaceCCE
 *                    availableCCE_p
 *                    totalCCERequired
 *                    internalCellIndex
 * Outputs:           availableCCE_p
 * Return:            None
 * Description:       This function set currentCFIValue_g depends on dynamic pdcch
 *                    is enabled or not.
 **********************************************************************************/
void setCurrentCFIValueInPDCCH(CellConfigParams *cellParams_p,
        UInt32 maxCommonSpaceCCE,
        UInt32 *availableCCE_p,
        UInt8 totalCCERequired,
        InternalCellIndex  internalCellIndex)
{
    if (!cellParams_p->isDynamicPdcchDisabled)
    {

        while(((maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1] - maxCommonSpaceCCE) 
                    > maxCCEInCommonSpace_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1])&& (currentCFIValue_g[internalCellIndex] < 3))
        {
            currentCFIValue_g[internalCellIndex] = currentCFIValue_g[internalCellIndex] + 1;
        }
        while (((totalCCERequired + (maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1] - maxCommonSpaceCCE)) 
                    > maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1]) && (currentCFIValue_g[internalCellIndex] < 3))
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        /* + SPR 8190 */
        while ((highestCCENumberCommonSpace_g[internalCellIndex] != INVALID_CCE_INDEX)
                && (maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1] < 
                    highestCCENumberCommonSpace_g[internalCellIndex])
                &&(currentCFIValue_g[internalCellIndex] < 3))
        {
            currentCFIValue_g[internalCellIndex]++;
        }
        highestCCENumberCommonSpace_g[internalCellIndex] = 0;
        /* - SPR 8190 */

        if(cellParams_p->dlAvailableRBs == 6 && (currentCFIValue_g[internalCellIndex] < 4))
        {
            currentCFIValue_g[internalCellIndex] += 1;
        }
        *availableCCE_p = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1]\
                       - (maxCCEInCommonSpace_g[internalCellIndex][valueCFI_g[internalCellIndex] - 1] - maxCommonSpaceCCE);
    }
    else
    {
        currentCFIValue_g[internalCellIndex] = valueCFI_g[internalCellIndex];

    }
    /* + PRS_CHANGES */
    if((0 != cellParams_p->prsConfigInfo.prsSubframeCounter) &&
            (10 > cellParams_p->dlAvailableRBs) && (4 == currentCFIValue_g[internalCellIndex]))
    {
        currentCFIValue_g[internalCellIndex] -= 1;
    }

    return;
}
#endif


/* Cyclomatic_complexity_changes_end */

#ifdef TDD_CONFIG
/* TDD Config 0 Changes Start */
/*****************************************************************************
 * Function Name  : putEntryInULSchedulerNewTxList
 * Inputs         : subFrameNum : subFrame Nuber where grant is being given
 *                  ulSubFrameNum : ul subframe number or which grant is 
 *                  being given
 *                  ulStrategyTxNode_p : This is pointer to ULStrategyTxNode
 *                  structure
 *                  ueULContext_p : uplink UE Context.
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion will push the ulStrategyTxNode_p Node in the List
 *                  ulStrategyTdd0SsfTxList_g/ulStrategyTdd0SsfTxListUSfOffset7_g
 *                  based on the ulSubFrameOffSet value.
 *****************************************************************************/
MacRetType putEntryInULSchedulerNewTxList(
        UInt8 subFrameNum,
        UInt8 ulSubFrameNum,
        ULStrategyTxNode *ulStrategyTxNode_p,
        ULUEContext   *ueULContext_p,
        /* CA TDD CHANGES START */
        InternalCellIndex internalCellIndex
        /* CA TDD CHANGES END */
        )
{
    UInt8 ulSubFrameOffSet = 0;
    ULGrantRequestType ulGrantRequestType  = INVALID_GRANT;
    ULHarqInfo   *ulHarqProcess_p          = PNULL;
    LTE_LIST     *ulStrategyTdd0SsfTxList_p = PNULL;
    ulHarqProcess_p = 
        &ueULContext_p->ulresInfo[ulStrategyTxNode_p->ulHarqProcessId];
    ulGrantRequestType = ulStrategyTxNode_p->ulGrantRequestType;

    /* Get the ulSubFrameOffSet and corresponding List */
    GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrameNum,ulSubFrameNum);
    if (ULTX_OFFSET == ulSubFrameOffSet)
    {
        /* CA TDD CHANGES START */
        ulStrategyTdd0SsfTxList_p = &ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex];
    }
    else
    {
        ulStrategyTdd0SsfTxList_p = &ulStrategyTdd0SsfTxList_g[internalCellIndex];
        /* CA TDD CHANGES END */
    }

    /* Check the validity of ulHarqProcess_p and ulStrategyTdd0SsfTxList_p */
    if ((INVALID_HARQ_ID == ulStrategyTxNode_p->ulHarqProcessId) || 
            (PNULL == ulStrategyTdd0SsfTxList_p))
    {
        return MAC_FAILURE;
    }

    /* Updating the reqiredRB in UL Harq Process */
    ulHarqProcess_p->requiredRB = ulStrategyTxNode_p->requiredRB;

    /* Put Entry in ULSchedulerNewTxList */
    listInsertNode(ulStrategyTdd0SsfTxList_p,
            &(ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor));

    /* Updating the scheduled count in UE Context in case of SUCCESS */
    if (NEW_ULGRANT_REQ_MSG == ulGrantRequestType)
    {
        ueULContext_p->scheduledCount++;
    }

    return MAC_SUCCESS;
}
/*****************************************************************************
 *  * Function Name  : pushListNodeInULSchedulerNewTxQueue
 *  * Inputs         : ulStrategyTxNode_p - This is pointer to ULStrategyTxNode
 *  *                  structure
 *  * Outputs        : None
 *  * Returns        : MAC_SUCCESS/MAC_FAILURE
 *  * Description    : This funtion will push the ulStrategyTxNode_p Node in the
 *  *                     ulStrategyTxQueue_g Queue
 ******************************************************************************/
MacRetType pushListNodeInULSchedulerNewTxQueue(
        ULStrategyTxNode *ulStrategyTxNode_p,
        InternalCellIndex internalCellIndex)
{
    /* + coverity 32791, 32792 */
    MacRetType ratval = MAC_SUCCESS;
#ifndef DL_UL_SPLIT
    if (pushNode(ulStrategyTxQueue_gp[internalCellIndex],
                &(ulStrategyTxNode_p->ulStrategyTxNodeAnchor)))
    {
        ratval = MAC_FAILURE;
    }
#else
    ratval = putEntryInULSchedulerNewTxQueueUsingICC(ulStrategyTxNode_p);
#endif

    return ratval;
    /* - coverity 32791, 32792 */
}
/*********************************************************************************
 * Function Name : getPdcchInfoNode 
 * Inputs        : localPdcchInfoMap - Local PdcchInfo* array
 *                 currentGlobalTickWithDelay - Current Global Tick with delay 
 *                 subFrameNum - Current Subframe Number
 *                 sysFrameNum - Current System Frame number
 *                 ulSfFromNode - UL Subframe number for which we need to send grant
 * Outputs       : PdcchInfo* - PdcchInfo pointer coresponding to ulSubframe Number          
 * Description   : It will check the PdcchInfo* node in ulSubframe location If present 
 *                 return the same, if not, it will create a node and initialise all
 *                 values. 
 **********************************************************************************/
PdcchInfo* getPdcchInfoNode(PdcchInfo* localPdcchInfoMap[],
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithDelay,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        UInt8 ulSfFromNode,
        InternalCellIndex internalCellIndex)
{
    PdcchInfo* pdcchInfoNode_p      = PNULL;
    UInt8 pdcchULOutputInfoIndex    = MODULO_TWO(sysFrameNum);
    UInt8 ulSfnFactor               = 0;
    UInt8 ulschOffset               = 0;
    UInt8 rbMapNum                  = 0;
    UInt8 userModulationType        = 0;
    UInt8 userLocationType          = 0;
    /* To Initialize the dynamic allocated PdcchInfo structure element */
    static PdcchInfo initPdcchInfoNode = {0};

    pdcchInfoNode_p = localPdcchInfoMap[ulSfFromNode];
    if ( pdcchInfoNode_p == PNULL )
    {
        GET_MEM_FROM_POOL(PdcchInfo,pdcchInfoNode_p,sizeof(PdcchInfo),PNULL);
        if ( PNULL == pdcchInfoNode_p )
        {
            pdcchInfoNode_p = PNULL;    
        }
        else
        {
            *pdcchInfoNode_p = initPdcchInfoNode;
            ulschOffset = getUlOffset(subFrameNum,ulSfFromNode);
            ulSfnFactor = (subFrameNum + ulschOffset) > MAX_SUB_FRAME ? 1:0;
            pdcchULOutputInfoIndex = MODULO_TWO(pdcchULOutputInfoIndex + ulSfnFactor);
            rbMapNum = (currentGlobalTickWithDelay + ulschOffset)% getMaxULRBMaps(internalCellIndex);
            
            pdcchInfoNode_p->pdcchULOutput_p = &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndex][ulSfFromNode];
            //Reset Variables for Nack Q Processing
            pdcchInfoNode_p->nackUEInfo_p = &pdcchInfoNode_p->pdcchULOutput_p->pdcchULNackUEInfo;
            pdcchInfoNode_p->nackUEInfo_p->adaptiveCount[CC_USER] = 0;
            pdcchInfoNode_p->nackUEInfo_p->adaptiveCount[CE_USER] = 0;
            pdcchInfoNode_p->nackUEInfo_p->nonAdaptiveCount = 0;

            //Reset Variables for Temp UE Nack Processing
            pdcchInfoNode_p->nackTempUEInfo_p = &pdcchInfoNode_p->pdcchULOutput_p->pdcchULNackTempUEInfo;
            pdcchInfoNode_p->nackTempUEInfo_p->adaptiveCount[CC_USER] = 0;
            pdcchInfoNode_p->nackTempUEInfo_p->adaptiveCount[CE_USER] = 0;
            pdcchInfoNode_p->nackTempUEInfo_p->nonAdaptiveCount = 0;

            //Reset Variables for New UE Processing
            pdcchInfoNode_p->pdcchULOutput_p->pdcchULNewUEInfo[CC_USER].count = 0;
            pdcchInfoNode_p->pdcchULOutput_p->pdcchULNewUEInfo[CE_USER].count = 0;
            pdcchInfoNode_p->pdcchULOutput_p->pdcchDLUEInfo.count             = 0;
            /* SPR 18510 - start*/
            //Reset Variables for SPS Nack Q Processing
            pdcchInfoNode_p->nackSpsUEInfo_p = &pdcchInfoNode_p->pdcchULOutput_p->pdcchULNackSpsUEInfo;
            pdcchInfoNode_p->nackSpsUEInfo_p->adaptiveCount[CC_USER] = 0;
            pdcchInfoNode_p->nackSpsUEInfo_p->adaptiveCount[CE_USER] = 0;
            pdcchInfoNode_p->nackSpsUEInfo_p->nonAdaptiveCount = 0;
            /* SPR 18510 - end*/
            /* UL_MU_MIMO_CHG_START */
            for (userLocationType = 0; userLocationType < MAX_USER_LOCATION; userLocationType++)
            {
                for (userModulationType = 0; userModulationType < MAC_MAX_NUM_MOD_SCHEME; userModulationType++)
                {
                    pdcchInfoNode_p->pdcchULOutput_p->pdcchULMUMIMOCandidateInfo
                        [userLocationType][userModulationType].count = 0;
                }
            }
            /* UL_MU_MIMO_CHG_END */

            pdcchInfoNode_p->rbMapNum = rbMapNum;
            localPdcchInfoMap[ulSfFromNode] = pdcchInfoNode_p;
          
            //Mark the PUCCH and RACH resource Before Processing the Nack Queue
            processAdvancePUCCHAndPRACHReports(rbMapNum, currentGlobalTickWithDelay,ulSfFromNode, internalCellIndex);
        }
    }
    return pdcchInfoNode_p;
    
}
/*********************************************************************************
 * Function Name : cleanlocalPdcchInfoMap 
 * Inputs        : localPdcchInfoMap - Local PdcchInfo* array
 * Outputs       :           
 * Description   : It will free all the nodes in the local map 
 **********************************************************************************/
void cleanlocalPdcchInfoMap(PdcchInfo* localPdcchInfoMap[])
{
    PdcchInfo* pdcchInfoNode_p      = PNULL;
    UInt8 index                     = 0;
    for (;index < MAX_SUB_FRAME;index++)
    {
        pdcchInfoNode_p = localPdcchInfoMap[index];
        if (pdcchInfoNode_p)
        {
            freeMemPool((void *)pdcchInfoNode_p);
        }
    }
        
}
/*********************************************************************************
 * Function Name : updateUeScheduleInfo 
 * Inputs        : txNode_p - Strategy Node pointer
 *                 ueScheduledInfo_p - Schedule Info pointer 
 *                 pdcchULNewUEInfo_p - PdcchUlNewInfo Pointer
 *                 countULNewUE - PdcchUlNewInfo array index
 *                 allocatedCCEIndex - Alocated CCE index
 *                 ulHarqProcessId - Harq Process Id
 * Outputs       : 
 * Description   : It will fill the information in ueScheduleInfo and pdcchUlNewueInfo 
 *                 from the strategy node and allocated cee index
 **********************************************************************************/
void updateUeScheduleInfo(ULStrategyTxNode  *txNode_p,
                          UeScheduledInfo *ueScheduledInfo_p,
                          PdcchULNewUEInfo *pdcchULNewUEInfo_p,
                          UInt8 countULNewUE,
                          UInt32 allocatedCCEIndex,
                          UInt8 ulHarqProcessId,
                          /* SPR 11065 Changes Start */
                          UInt8 isNodePresentIn2UlSubframe
                          /* SPR 11065 Changes End */
                          )
{
    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].ueIndex =
         txNode_p->ueIndex; 
    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS = 
        txNode_p->avgMcs;
    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].minDataSize = 
        txNode_p->minDataSize;
    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize = 
        txNode_p->maxDataSize;
    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].scheduleCount = 1;
    /* SPR 11065 Changes Start */
    pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].isNodePresentIn2UlSubframe = isNodePresentIn2UlSubframe;
    /* SPR 11065 Changes Start */

    ueScheduledInfo_p->ulCCEIndex         = allocatedCCEIndex;
    ueScheduledInfo_p->strictRBAllocation = txNode_p->strictAllocRB;

    ueScheduledInfo_p->requiredRB         = 
        getUplinkMaxRBsInPowerOfTwoThreeFive(txNode_p->requiredRB);  

    ueScheduledInfo_p->ulGrantRequestType = txNode_p->ulGrantRequestType;
    ueScheduledInfo_p->ttiCounter         = txNode_p->ttiCounter;
    ueScheduledInfo_p->ulHarqProcessId    = ulHarqProcessId;
    ueScheduledInfo_p->uePosInPdcchULNewUEArray = countULNewUE;
    ueScheduledInfo_p->flag               = NEW_ALLOCATION;

}

/* SPR 11236 Changes Start */
/*********************************************************************************
 * Function Name : updateULOutputInfoArray 
 * Inputs        : pdcchULNewUEInfo_p - PdcchUlNewInfo Pointer
 *                 posInULOutputInfo  - Current position in ul output info array.
 *                 ueScheduledInfo_p  - ueSchedule info pointer for UE.
 *                 ulSubframeNum - uplink subframe number.
 * Outputs       : 
 * Description   : This function does the following:
 *                 a) Copies the content of uloutput info array for this UE
 *                    (on which currently working) into local structure.
 *                 b) Then, Moves each entry to its next index starting from 0
 *                    index to next index till the index in which current UE is
 *                    present. Let say, Current ueindex is present at the index 3.
 *                 c) Copies localULOutputInfo to zeroth index.
 **********************************************************************************/
void updateULOutputInfoArray(PdcchULNewUEInfo *pdcchULNewUEInfo_p,
        UInt8 posInULOutputInfo,
        UeScheduledInfo *ueScheduledInfo_p,
        UInt8 ulSubframeNum,
        /** SPR 14204 Fix : Start **/
        InternalCellIndex internalCellIndex
        /** SPR 14204 Fix : End **/
        )
{
    UInt8 localIndex = 0;
    PdcchULNewUE localULNewUEInfo;

    /* a) */
    localULNewUEInfo = pdcchULNewUEInfo_p->pdcchULNewUEArray[posInULOutputInfo];

    /* b) */
    for (localIndex = posInULOutputInfo; localIndex > NUM_ZERO; localIndex--)
    {
        /** SPR 14204 Fix : Start **/
        copyULOutputInfo(pdcchULNewUEInfo_p, localIndex, (localIndex-NUM_ONE), ulSubframeNum, internalCellIndex);
        /** SPR 14204 Fix : End **/
    }

    /* c) */
    pdcchULNewUEInfo_p->pdcchULNewUEArray[0] = localULNewUEInfo;
    ueScheduledInfo_p->uePosInPdcchULNewUEArray = NUM_ZERO;

    return;
}

/*********************************************************************************
 * Function Name : copyULOutputInfo
 * Inputs        : pdcchULNewUEInfo_p - PdcchUlNewInfo Pointer
 *                 toIndex - Index in which uloutput will be copied.
 *                 fromIndex - from which data will be copied.
 *                 ulSubframeNum - Uplink subframe number.
 * Outputs       : NONE 
 * Description   : This function copies the content of uloutput info index (from index)
 *                 to target index and update the uePosInPdcchULNewUEArray in 
 *                 ueScheduled info.
 **********************************************************************************/
void copyULOutputInfo(PdcchULNewUEInfo *pdcchULNewUEInfo_p,
                      UInt8 toIndex,
                      UInt8 fromIndex,
                      UInt8 ulSubframeNum,
                      /** SPR 14204 Fix : Start **/
                      InternalCellIndex internalCellIndex
                      /** SPR 14204 Fix : End **/
        )
{
    ULUEContext* ulUEContext_p = PNULL;

    pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].ueIndex =
        pdcchULNewUEInfo_p->pdcchULNewUEArray[fromIndex].ueIndex; 
    pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].avgMCS = 
        pdcchULNewUEInfo_p->pdcchULNewUEArray[fromIndex].avgMCS; 
    pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].minDataSize = 
        pdcchULNewUEInfo_p->pdcchULNewUEArray[fromIndex].minDataSize; 
    pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].maxDataSize = 
        pdcchULNewUEInfo_p->pdcchULNewUEArray[fromIndex].maxDataSize; 
    pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].scheduleCount = 
        pdcchULNewUEInfo_p->pdcchULNewUEArray[fromIndex].scheduleCount;
    pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].isNodePresentIn2UlSubframe =
        pdcchULNewUEInfo_p->pdcchULNewUEArray[fromIndex].isNodePresentIn2UlSubframe;

   /* Update the Position in ueScheduledInfo */
    ulUEContext_p = ulUECtxInfoArr_g[pdcchULNewUEInfo_p->pdcchULNewUEArray[toIndex].ueIndex].ulUEContext_p;
    /** SPR 14204 Fix : Start **/
    ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubframeNum]->uePosInPdcchULNewUEArray = toIndex;
    /** SPR 14204 Fix : End **/

    return;
}

/* SPR 11236 Changes End */

/*********************************************************************************
 * Function Name : processUlMUMIMOUe 
 * Inputs        : ueScheduledInfo_p - ueScheduleInfo pointer
 *                 pdcchULOutputInfo_p - pdcchUlOutputInfo pointer
 *                 ulUEContext_p - ulUeContext pointer
 *                 txNode_p - Strategy Node
 *                 rbMapNum - RB Map number
 *                 internalCellIndex - internal cell idx
 * Outputs       :           
 * Description   : Process the UL Mimo UEs 
 **********************************************************************************/
void processUlMUMIMOUe(UeScheduledInfo *ueScheduledInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        ULUEContext * ulUEContext_p,
        ULStrategyTxNode  *txNode_p,
        UInt8 rbMapNum,
        InternalCellIndex internalCellIndex)
{ 
    /* UL_MU_MIMO_CHG_START */
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
    PdcchULMUMIMOCandidateInfo *pdcchULMUMIMOCandidateInfo_p = PNULL;
    PdcchULNewUEInfo *pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo
                              [ulUEContext_p->userLocationType];
    UInt32 countULMUMIMO = 0;
    UInt32 rbsForMaxTBSize = 0;
    UInt8 modScheme = 0;
    UInt8 retVal = FALSE;
    UInt8 countULNewUE      = pdcchULNewUEInfo_p->count;

    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    ueScheduledInfo_p->buddyUeIndex = INVALID_UE_INDEX;
    modScheme = ulUEContext_p->modulationScheme;
    MAC_GET_MOD_TYPE(modScheme);
    pdcchULMUMIMOCandidateInfo_p = &pdcchULOutputInfo_p->pdcchULMUMIMOCandidateInfo[ulUEContext_p->userLocationType][modScheme];
    countULMUMIMO = pdcchULMUMIMOCandidateInfo_p->count;
    ulUEContextInfo_p = &ulUECtxInfoArr_g[txNode_p->ueIndex];

    /* Checking if MU MIMO is to be done or not.
     * If all the criteria for MU MIMO are met then add the ueIndex in
     * PdcchULMUMIMOCandidateInfo[userLocation][modulationType]list and reference Index in UEContext. */

    /* TDD Config 0 Changes Start */
    UL_MU_MIMO_CANDIDATE_SELECTION(ulUEContextInfo_p,
            ulUEContext_p,
            txNode_p->avgMcs,
            txNode_p->ulGrantRequestType,
            txNode_p->ulSubFrameNum,
            retVal,
            internalCellIndex);
    /* TDD Config 0 Changes End */
    if (retVal)
    {
        pdcchULMUMIMOCandidateInfo_p->pdcchULMUMIMOCandidateUE[countULMUMIMO].ueIndex = txNode_p->ueIndex;
        countULMUMIMO++;
        pdcchULMUMIMOCandidateInfo_p->count = countULMUMIMO;
        /*SPR_5875 Start*/
        rbsForMaxTBSize = DETERMINE_RBs_UL(pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS,
                                           pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize,
                                           internalCellIndex);
        rbsForMaxTBSize = getUplinkMaxRBsInPowerOfTwoThreeFive(rbsForMaxTBSize);
        rbMapNode_p->aggregateMaxRBsRequired += rbsForMaxTBSize;
        /*SPR_5875 End*/
        /* The UE is selected as a candidate UE */
        LOG_MAC_MSG(MAC_CAND_SEL_UE_INFO_ID,LOGDEBUG,MAC_PDCCH,
                getCurrentTick(),
                txNode_p->ueIndex,
                countULMUMIMO,rbMapNode_p->aggregateMaxRBsRequired,pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS,
                pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].maxDataSize,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    }
    else
    {
        /* The various parameter values will help deduce the reason for failure of UE as a candidate UE */
        LOG_MAC_MSG(MAC_CAND_FAIL_UE_INFO_ID,LOGDEBUG,MAC_PDCCH,
                getCurrentTick(),
                txNode_p->ueIndex,
                ulUEContextInfo_p->pendingDeleteFlag,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                pdcchULNewUEInfo_p->pdcchULNewUEArray[countULNewUE].avgMCS,
                ueScheduledInfo_p->ulGrantRequestType,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    }

    /* UL_MU_MIMO_CHG_END */


}
/*****************************************************************************
 * Function Name  : checkAvailabilityOfHIResources
 * Inputs         : hiIndex
 *                  nPhichGrp
 *                  nPhichSeq
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function checks if same hi resources are
 *                  available or not If available update phichgrp and phichseq
 *                  in harq info.
 ******************************************************************************/
STATIC  MacRetType checkAvailabilityOfHIResources(
                                  UInt8 hiIndex,
                                  UInt8 iPhich,
                                  ULHarqInfo* tempULHarqInfo_p,
                                  InternalCellIndex internalCellIndex)
{
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;

    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][tempULHarqInfo_p->riStart][tempULHarqInfo_p->nDmrs];
    nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][tempULHarqInfo_p->riStart][tempULHarqInfo_p->nDmrs];
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp]
                & (ONE << nPhichSeq))
    {
        /*Update phich grp and phich seq in harq info incase ndmrs is
         * available*/
        tempULHarqInfo_p->nPhichGrp = nPhichGrp;
        tempULHarqInfo_p->nPhichSeq = nPhichSeq; 
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}
/* TDD Config 0 Changes End */
/* + CA_TDD_HARQ_CHANGES */
/*****************************************************************************
 * Function Name  : calculateDAI
 * Inputs         : currentGlobaTick - Current global tick
 *                  tddValidateDaiVal - Value use to validate DAI
 *                  tddDaiLastScheduled - Last DL tick scheduled
 *                  tempDai - last DAI value
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculate DAI.
 ******************************************************************************/
 /* SPR 15909 fix start */
STATIC void calculateDAI( tickType_t currentGlobaTick,
                   UInt8  tddValidateDaiVal,
                   UInt32 tddDaiLastScheduled,
                   UInt8  *tempDai,
                   /* + SPS_TDD_Changes */
                   UInt8 *tempDaiSps)
                   /* - SPS_TDD_Changes */
/* SPR 15909 fix end */
{

    /* SPR 609 changes start */    
    /* SPR 15909 fix start */
    tickType_t tickRoundoff = 0xffffffffffffffff;
    /* SPR 15909 fix end */
    /* SPR 609 changes start */   
    UInt32 timeDiff = 0;

    if(tddDaiLastScheduled < currentGlobaTick)
    {
        timeDiff = currentGlobaTick - tddDaiLastScheduled;
    }
    else
    {
        /* SPR 609 changes start */    
        timeDiff = currentGlobaTick + tickRoundoff + 1 - tddDaiLastScheduled;
        /* SPR 609 changes end */    
    }

    /* If the last PDSCH transmission is for previous DAI txn, 
     * Reset DAI and Initialize to ONE
     * Else, Increment the DAI value
     */
    /** CA-TDD Changes Start **/
    if(timeDiff > tddValidateDaiVal)
        /** CA-TDD Changes End **/
    {
        *tempDai = 1;
        /* + SPS_TDD_Changes */
        *tempDaiSps = 0;
        /* - SPS_TDD_Changes */
    }
    else
    {
        *tempDai += 1;
    }

}
/* - CA_TDD_HARQ_CHANGES */
#endif
/* SPR 22252 fix start */
/*****************************************************************************
 * Function Name  : cleanUpHarqAndStrategyNode
 * Inputs         : ueIndex
 * 		    dlUEContext_p: DL UE context
 * 		    dlStrategyTxNode: Downlink strategy node
 * 		    isPdcchAllocatedFlag_p: Flag to check if CCE allocation already done 	
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion will be called in failure cases where Harq process and 
 * 	 	    strategy node is freed. For TA/Scell CE messgae, add entry in failure 
 * 	 	    queue.			
 *****************************************************************************/

STATIC  void cleanUpHarqAndStrategyNode( UInt16 ueIndex,
                        DLUEContext *dlUEContext_p,
                        DLStrategyTxNode *dlStrategyTxNode,
                        UInt8 *isPdcchAllocatedFlag_p,
                        InternalCellIndex internalCellIndex)
{
    MacRetType retVal = MAC_FAILURE;
    LTE_MAC_UT_LOG(LOG_WARNING,PDCCH_MGR,
            "PDCCH has been allocated earlier for this UE = %u",ueIndex);
    DLHARQProcess  *harqProcess_p = PNULL;
    /* + SPR 11469 Changes */
    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        harqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess
            [dlStrategyTxNode->harqProcessId];
    }
    else
    {
        harqProcess_p = &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
            ScelldlHarqContext_p->harqProcess[dlStrategyTxNode->harqProcessId];
    }
    /* - SPR 11469 Changes */

    if((DRA_SCHEDULING == dlUEContext_p->schType)&&
            (DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             initParams_p->dlSchdConfig.dlSchedulerStrategy))
    {

        switch(harqProcess_p->dlHARQTBOneInfo.taDrxFlag)
        {
            case TA_MSG:
            /* SPR 9546 Fix Start */
            putEntryInPdcchPdschFailureTAQueue(ueIndex,
                    dlUEContext_p->timingAdvanceValue,
                    internalCellIndex );
            /* SPR 9546 Fix End */
            break;
            default:
            break;
            /* No TA_DRX Confiured. Hence do nothing */
        }

        if( ( (harqProcess_p->isTBOneValid) &&
                    (harqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)) ||
                ( (harqProcess_p->isTBTwoValid) &&  
                  (harqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)))
        {
            putEntryInPendingFailureDlCAMacCEQueue( dlStrategyTxNode->ueIndex,
                    harqProcess_p,
                    internalCellIndex);
        }

    }
    /* ICIC changes start */
    else
    {
        switch(harqProcess_p->dlHARQTBOneInfo.taDrxFlag)
        {
            case TA_MSG:
            /* +coverity-530-CID 24492*/
            if(MAC_FAILURE == putEntryInAdvanceAllocInfoQueue(dlStrategyTxNode,internalCellIndex))
            {
                retVal= MAC_FAILURE;
            }
            /* +coverity-530-CID 24492*/
            /* + SPR 5584/5510 Fix Start */
            /* + Coverity 55034 */
            else
            {
                retVal = MAC_SUCCESS;
            }
            /* - Coverity 55034 */ 
	    break; 	
            /* - SPR 5584/5510 Fix Start */
            default:
            break;
            /* No TA_DRX Confiured. Hence do nothing */
        }
    }
    /* ICIC changes end */
    if ( retVal == MAC_FAILURE)
    {

        freeDLHarqProcess(dlUEContext_p,harqProcess_p,internalCellIndex);

        freeMemPool(dlStrategyTxNode);

        /* + SPR 11491 Changes */
        if (!*isPdcchAllocatedFlag_p)
            /* + SPR 11491 Changes */
        {
            /* SPR 5147 changes start */
            lteWarning("Total number of CC+CE scheduled users greater than max scheduled UE per TTI");
            /* SPR 5147 changes start */
        }
    }
}
/*****************************************************************************
 *
 * Function Name  : updateRBsAndPutEntryinFailureQueue
 * Inputs         : dlStrategyTxNode: Downlink strategy node
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion will be called to update RB is execess pool 
 * 		    in failure cases.
 * *****************************************************************************/

STATIC  void updateRBsAndPutEntryinFailureQueue( DLStrategyTxNode *dlStrategyTxNode,
                        InternalCellIndex internalCellIndex )
{
    UPDATE_EXCESS_POOL_RB(dlStrategyTxNode->resourceAllocInput. 
            resourceRegionAllocated, 
            dlStrategyTxNode->resourceAllocInput.requiredRB, 
            internalCellIndex); 
    if (CE_REGION ==  
            dlStrategyTxNode->resourceAllocInput.resourceRegionAllocated) 
    { 
        excessPoolRb_g[internalCellIndex][CC_CE_REGION] += 
            dlStrategyTxNode->resourceAllocInput.requiredRB; 
    } 
    putEntryInRetransmissionFailureQueue(dlStrategyTxNode,internalCellIndex); 
}
/* SPR 22252 fix start */

/******************************************************************************
 * Function Name: allocatePdcchForDCI1CNotification
 * Inputs       : availableCCE_p - Contains the available maximum CCEs
 *                dciCCEContainerInfo_p -
 *                internalCellIndex - Cell-Index at MAC
 * Outputs      : 
 * Returns      : maxCommonSpaceCCE left after allocation.
 * Description  : This will allocate CCEs for DCI1c notification
 ******************************************************************************/

#ifdef LTE_EMBMS_SUPPORTED
void  allocatePdcchForDCI1CNotification(UInt32 *maxCommonSpaceCCE,
            DciCCEContainerInfo *dciCCEContainerInfo_p,
            UInt32 *availableCCE_p,
            InternalCellIndex internalCellIndex)
{
    UInt32 aggregationLevel = 0;

    UInt32 cceAllocated     = 0;
    UInt32 macRetType       = MAC_FAILURE;
    DciCCEInfo *dciCCEInfo_p = PNULL;

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    aggregationLevel = AGGREGATION_LEVEL_FOR_COMMON_SPACE;

    macRetType = allocatePDCCHForDCIFormatCommonSpace (
            aggregationLevel,&cceAllocated,
            availableCCE_p, dciCCEInfo_p,
            internalCellIndex);

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,PDCCH_MGR,
                "[%s]: CCE allocated successfully cceAllocated [%d]\n",
                __func__,cceAllocated);
        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl =aggregationLevel;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex = cceAllocated;
        dciCCEContainerInfo_p->arrayCCEAllocated \
            [dciCCEContainerInfo_p->countOfCCEAllocated] = cceAllocated;
        dciCCEContainerInfo_p->countOfCCEAllocated++;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti = MRNTI;
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->eMbmsTransPower;

        dciCCEInfo_p[cceAllocated].dciPduInfo.pA = DEFAULT_PA_VALUE;
        dciCCEInfo_p[cceAllocated].msgType = EMBMS_DCI1C_NOTIFICATION;
        /* SSI_FIX start*/
        dciCCEContainerInfo_p->dciCCEInfoCmnChannel[dciCCEContainerInfo_p->countCmnChannelMsg].msgType=\
        EMBMS_DCI1C_NOTIFICATION;
        dciCCEContainerInfo_p->countCmnChannelMsg++;
        /* SSI_FIX End*/
        dciCCEInfo_p[cceAllocated].dciPduInfo.dciFormat = DCI_FORMAT_1C;   
        *maxCommonSpaceCCE = *maxCommonSpaceCCE - aggregationLevel;
    }
}

#endif

