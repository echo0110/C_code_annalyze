/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: tegy.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This File is used to apply the DL Scheduling strategy 
                       to all the UEs which needs to be schedule depending upon 
                       the DL RBs requests. the Request can be for sending TA ,
                       DRX, Retransmission or for the fresh scheduling.  
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLSchedulerStrategy.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.50  2009/12/02 07:16:49  gur18550
 * Optimisation related changes
 *
 * Revision 1.49  2009/12/02 06:36:27  gur18550
 * Bug Solved for tempNewCalculatedRBs
 *
 * Revision 1.48  2009/11/15 12:57:14  gur11912
 * LTE_MAC_UT_LOG(LOG_WARNING) used instead of lteWarning
 *
 * Revision 1.47  2009/11/10 12:45:13  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.46  2009/10/06 06:11:29  gur11912
 * optimization changes
 *
 * Revision 1.45  2009/10/05 08:04:37  gur11912
 * optimization changes
 *
 * Revision 1.44  2009/08/04 15:25:12  gur20052
 * Added more LOGS related to subnband info
 *
 * Revision 1.43  2009/08/01 09:43:49  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.42  2009/08/01 09:42:18  gur20052
 * Added the implementation of TA DRX,now cheking pendingTA/DRX Ack
 *
 * Revision 1.41  2009/07/31 06:32:13  gur15697
 * cleanup Bugfix
 *
 * Revision 1.40  2009/07/30 18:10:14  gur20052
 * UT Defects are fixed related to isAlreadyConsideredFlag
 *
 * Revision 1.39  2009/07/30 14:44:41  gur20052
 * UT Defects are fixed
 *
 * Revision 1.38  2009/07/30 12:28:55  gur20052
 * UT Defects are fixed
 *
 * Revision 1.37  2009/07/29 16:25:30  gur20052
 * UT Defect is fixed
 *
 * Revision 1.36  2009/07/27 06:09:22  gur20052
 * UT Defects are fixed
 *
 * Revision 1.35  2009/07/22 14:18:50  gur20052
 * UT Defect are fixed
 *
 * Revision 1.34  2009/07/20 17:45:04  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.33  2009/07/16 17:36:14  gur20052
 * Added code for Setting up of aperiodicCQIRequested flag in DL UE Context and UT Bug Fix
 *
 * Revision 1.32  2009/07/15 05:59:30  gur20052
 * UT Defects fixed
 *
 * Revision 1.31  2009/07/13 14:06:30  gur20052
 * UT Bugs Fixed
 *
 * Revision 1.30  2009/07/13 12:25:10  gur20052
 * added prefix string macro and removed code in #if 0
 *
 * Revision 1.29  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.28  2009/07/07 13:28:42  gur20052
 * Updated for UT Defects
 *
 * Revision 1.27  2009/07/02 10:57:36  gur20052
 * Enhancement Comments are incorporated
 *
 * Revision 1.26  2009/06/29 15:09:13  gur20052
 * UT Defects are removed
 *
 * Revision 1.25  2009/06/25 06:45:05  gur20052
 * UT Defects are resolved
 *
 * Revision 1.24  2009/06/24 08:10:45  gur20052
 * Reveiw Comments are incorporated
 *
 * Revision 1.23  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.21  2009/06/18 06:44:20  gur20052
 * UT Defects are removed
 *
 * Revision 1.20  2009/06/11 14:52:56  gur20052
 * Removed the Linking Errors and Resource Allocator Node
 *
 * Revision 1.18  2009/06/10 14:26:07  gur20052
 * Removed the Linking errors and updated for createMemPool changes
 *
 * Revision 1.17  2009/06/08 08:21:19  gur20052
 * Updated for the Review comments incorporations
 *
 * Revision 1.16  2009/06/05 16:56:28  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.15  2009/06/05 16:20:23  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.14  2009/06/05 06:33:48  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.13  2009/06/04 07:39:50  gur20052
 * Review Comments are incorporated
 *
 * Revision 1.12  2009/06/02 07:19:08  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.11  2009/05/28 11:33:01  gur20052
 * The changes for the UE contexts are done
 *
 * Revision 1.10  2009/05/26 10:16:04  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.9  2009/05/25 16:20:01  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.8  2009/05/21 17:22:46  gur20052
 * Updated the Basic Framework of UL Strategy
 *
 * Revision 1.7  2009/05/21 06:24:55  gur20052
 * Added code for round robin Scheduler
 *
 * Revision 1.6  2009/05/20 13:02:47  gur20052
 *  Added The new Enums for message Type
 *
 * Revision 1.5  2009/05/20 08:01:45  gur20052
 * incorporated the review comments
 *
 * Revision 1.4  2009/05/19 14:29:31  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.3  2009/05/11 06:18:32  gur20052
 * Removed compilation errors
 *
 * Revision 1.2  2009/04/30 16:27:35  gur20052
 * Added initial version of DL Round Robin Scheduler
 *
 * Revision 1.1  2009/04/30 14:07:40  gur20052
 * Added initial version of  DL Scheduler Strategy
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteTypes.h"
#include "lteMisc.h"
#include <mqueue.h>
#include <string.h>

 /**************************************************************************** 
  * Projects Include
  * *************************************************************************/
#include "lteMacRRMInterface.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacRRMIntfInfo.h"
/* ECN CHANGES START*/
#include "ltePdcpRRMInterface.h"
#include "lteL2RRMInterface.h"
/* ECN CHANGES END*/
#include "ltePdcpTlvUtils.h"
/****************************************************************************
 * API Declaration 
 * **************************************************************************/
#define RRM_MAC_REQ_MESSAGE_API_START 1
#define RRM_MAC_CELL_CONFIG_REQ (RRM_MAC_REQ_MESSAGE_API_START + 0)
#define RRM_MAC_CELL_RECONFIG_REQ (RRM_MAC_REQ_MESSAGE_API_START + 1)
/* Perf stats RRM-MAC start */
#define RRM_MAC_CONFIGURE_PERF_STATS_REQ (RRM_MAC_REQ_MESSAGE_API_START + 7)
/* Perf stats RRM-MAC end */

#define RRM_MAC_CNF_MESSAGE_API_START 100
#define RRM_MAC_CELL_CONFIG_CNF (RRM_MAC_CNF_MESSAGE_API_START + 0)
#define RRM_MAC_SCHEDULER_RECONFIG_CNF (RRM_MAC_CNF_MESSAGE_API_START + 1)
#define RRM_MAC_UE_RECONFIG_CNF (RRM_MAC_CNF_MESSAGE_API_START + 2)

/* Perf stats RRM-MAC start */
#define RRM_MAC_CONFIGURE_PERF_STATS_CNF (RRM_MAC_CNF_MESSAGE_API_START + 10)
/* Perf stats RRM-MAC end */

#define RRM_MAC_IND_MESSAGE_API_START 200
#define RRM_MAC_PERIODIC_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 0)
#define RRM_MAC_LAYER1_MEAS_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 1)
#define RRM_MAC_LAYER2_ICIC_MEAS_REPORT_IND (RRM_MAC_IND_MESSAGE_API_START + 2)

/* Perf stats RRM-MAC start */
#define RRM_MAC_CELL_PERF_STATS_IND (RRM_MAC_IND_MESSAGE_API_START + 4)
/* Perf stats RRM-MAC end */

#define RRM_API_HEADER_SIZE 12
 /*+SPR - 7531*/
/* spr 18983 */
#define MAX_MSG_SIZE 30000
#define MAX_MSG_SZ 30000
/* spr 18983 */
 /*-SPR - 7531*/
#define PDCP_REC_ERROR -1


#define MAX_UTIL_INDEX 100
#define MAX_TA_RANGE 12
#define MAX_SRB 3
#define MAX_CFI 4
#define MAX_CCE_UTIL_INDEX 20

 /***************************************************************************
 * Global Declaration 
 * *************************************************************************/
mqd_t fdSendToMac_g;
mqd_t fdRecvFromMac_g;
/* ECN changes start*/
mqd_t fdSendToPdcp_g;
mqd_t fdRecvFromPdcp_g;
/* ECN changes end*/
mqd_t fdRecvFromRRM_g;
mqd_t fdForReceivingMsg_g;
mqd_t fdForSendingMsg_g;
AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMap;  
DeltaSINRToTPCMapForPUSCH deltaSINRToTPCMapForPUSCH; 
DeltaSINRToTPCMapForPUCCH deltaSINRToTPCMapForPUCCH; 
PdcchAggregationAndPowerOffset pdcchAggregationAndPowerOffset;
CqiToPHICHPower cqiToPHICHPower;
UInt32 duration_g   = 0;
UInt32 periodic_g   = 0;
UInt32 cellbitmap_g = 0;
UInt32 uebitmap_g   = 0;


#define NUM_OF_SINR_TPC_VALUES    255

/***************************************************************************
 * Global Function Declaration 
 * ************************************************************************/
PdcchAggregationAndPowerOffset CCpdcchAggregationAndPowerOffset =
{
        {
            { /* Small DCI Size*/
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {4,3000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  1*/
                { 2,  { {4,3000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  2*/
                { 2,  { {4,3000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  3*/
                { 2,  { {4,3000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  4*/
                { 2,  { {4,3000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  5*/
                { 2,  { {2,3000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  6*/
                { 2,  { {2,3000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  7*/
                { 2,  { {2,3000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  8*/
                { 1,  { {2,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI =  9*/
                { 1,  { {2,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 10*/
                { 1,  { {2,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 11*/
                { 1,  { {1,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 1,  { {1,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 1,  { {1,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 1,  { {1,3000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/
            },
            { /*Medium DCI Size */
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {8,2000}, {4, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},           /* CQI =  1*/
                { 2,  { {8,2000}, {4, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},           /* CQI =  2*/
                { 2,  { {4,2000}, {2, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},       /* CQI =  3*/
                { 2,  { {4,2000}, {2, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},       /* CQI =  4*/
                { 2,  { {4,2000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  5*/
                { 2,  { {4,2000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  6*/
                { 1,  { {2,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  7*/
                { 1,  { {2,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  8*/
                { 1,  { {2,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  9*/
                { 1,  { {2,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 10*/
                { 1,  { {2,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 11*/
                { 1,  { {2,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 1,  { {1,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 1,  { {1,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 1,  { {1,2000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/

            },
            { /*Big DCI Size */
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {8,1000},  {4,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  1*/
                { 2,  { {8,1000},  {4,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  2*/
                { 2,  { {8,1000},  {4,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  3*/
                { 2,  { {8,1000},  {4,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  4*/
                { 2,  { {4,1000},  {2,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  5*/
                { 2,  { {4,1000},  {2,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  6*/
                { 2,  { {4,1000},  {2,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  7*/
                { 2,  { {4,1000},  {2,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  8*/
                { 1,  { {2,1000},  {0xFF,0xFF},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  9*/
                { 1,  { {2,1000},  {0xFF,0xFF},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI = 10*/
                { 1,  { {2,1000},  {0xFF,0xFF},  {0xFF,0xFF}, {0xFF, 0xFF}}},   /* CQI = 11*/
                { 1,  { {2,1000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 1,  { {2,1000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 1,  { {1,1000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 1,  { {1,1000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/
            }
        }

};

PdcchAggregationAndPowerOffset CEpdcchAggregationAndPowerOffset =
{
        {
            { /* Small DCI Size*/
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {4,4000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  1*/
                { 2,  { {4,4000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  2*/
                { 2,  { {4,4000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  3*/
                { 2,  { {4,4000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  4*/
                { 2,  { {4,4000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  5*/
                { 2,  { {2,4000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  6*/
                { 2,  { {2,4000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  7*/
                { 2,  { {2,4000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  8*/
                { 1,  { {2,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI =  9*/
                { 1,  { {2,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 10*/
                { 1,  { {2,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 11*/
                { 1,  { {1,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 1,  { {1,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 1,  { {1,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 1,  { {1,4000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/
            },
            { /*Medium DCI Size */
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {8,5000}, {4, 8000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},           /* CQI =  1*/
                { 2,  { {8,5000}, {4, 8000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},           /* CQI =  2*/
                { 2,  { {4,5000}, {2, 8000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},       /* CQI =  3*/
                { 2,  { {4,5000}, {2, 8000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},       /* CQI =  4*/
                { 2,  { {4,5000}, {2, 8000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  5*/
                { 2,  { {4,5000}, {2, 8000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  6*/
                { 1,  { {2,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  7*/
                { 1,  { {2,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  8*/
                { 1,  { {2,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  9*/
                { 1,  { {2,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 10*/
                { 1,  { {2,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 11*/
                { 1,  { {2,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 1,  { {1,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 1,  { {1,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 1,  { {1,5000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/

            },
            { /*Big DCI Size */
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {8,7000},  {4,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  1*/
                { 2,  { {8,7000},  {4,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  2*/
                { 2,  { {8,7000},  {4,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  3*/
                { 2,  { {8,7000},  {4,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  4*/
                { 2,  { {4,7000},  {2,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  5*/
                { 2,  { {4,7000},  {2,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  6*/
                { 2,  { {4,7000},  {2,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  7*/
                { 2,  { {4,7000},  {2,9000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  8*/
                { 1,  { {2,7000},  {0xFF,0xFF},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  9*/
                { 1,  { {2,7000},  {0xFF,0xFF},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI = 10*/
                { 1,  { {2,7000},  {0xFF,0xFF},  {0xFF,0xFF}, {0xFF, 0xFF}}},   /* CQI = 11*/
                { 1,  { {2,7000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 1,  { {2,7000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 1,  { {1,7000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 1,  { {1,7000}, {0xFF, 0xFF}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/
            }
        }

};
DeltaSINRToTPCMapForPUCCH deltaSINRToTPCMapForPUCCH = 
{
    174, /*pucchSINRTarget*/
    4,   /* Number of tuples*/
    {
  /*startDeltaSINR, endDeltaSINR, TPC*/
        {      -255,    -26,  1},
        {      -25 ,    -10,  1},
        {      -9  ,     10,  0},
        {      11  ,     255, -1}

    }
};

DeltaBLERToTPCMapForPUCCH blerToTPCMapForPUCCH = 
{
    10, /*pucchBLERTarget*/
    4,  /* Number of tuples*/
    {
        /*startDeltaBLER, endDeltaBLER, TPC*/
        {      -4,   7,    -1},
        {       5 ,   6,     1},
        {       6 ,   26,     2},
        {       7 ,   70,    3}
    }
};

DeltaSINRToTPCMapForPUSCH deltaSINRToTPCMapForPUSCH =
{
    4,/* Number of tuples*/
    {
        /*startDeltaSINR, endDeltaSINR, TPC*/
        {      -255,    -26,  1},
        {      -25 ,    -10,  1},
        {      -9  ,     10,  0},
        {      11  ,    255, -1}

    }
};

AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMap = 
{
    30,
    {
        {/* alpha = 0*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/ 
            {{          4,          84,             158},
                {          181,       430,             160},
                {          141,        230,             116}
            }
        },
        {/* alpha = 0.4*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/ 
            {{          5,          86,             178},
                {          81,         130,             160},
                {          11,        203,             246}
            }
        },
        {/* alpha = 0.5*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          6,          80,             178},
                {          61,         130,             160},
                {          131,        300,             144}
            }
        }, 
        {/* alpha = 0.6*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          7,          80,             178},
                {          81,        130,             160},
                {          13,        100,             146}
            }
        }, 
        {/* alpha = 0.7*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          9,          80,             178},
                {          21,        130,             160},
                {          11,        200,             146}
            }
        }, 
        {/* alpha = 0.8*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          10,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }   
        }, 
        {/* alpha = 0.9*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          11,          80,             178},
                {          81,        130,             130},
                {          131,        200,             146}
            }      
        }, 
        {/* alpha = 1*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          12,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }      
        } 
    }
};


typedef struct CellHarqPerfStatsT
{
    UInt32 totalDLPRBUsageforHarq[9]; /*Number of PRBs used in DL per cell per
                                              QCI for HARQ initial transmissions. */
    UInt32 totalDLPRBUsageforHarqRetx;     /*Number of PRBs used in DL per cell for
                                                  HARQ retransmissions. */
} CellHarqPerfStats;

/* Structure Name  : RachSuccFailPerfStats
 *  * Purpose         : Indicates the RACH related Performance Stats  */
typedef struct RachSuccFailPerfStatsT {
    UInt32 totalRcvdMsg3RandomAccessPreamblesCBRA;  /*Number of successfully detected
                                                          RA Msg3 for CBRA */
    UInt32 totalRcvdMsg3RandomAccessPreamblesCFRA; /*Number of successfully completed
                                                          CFRA procedures */
    UInt32 totalFailedRandomAccessPreamblesMsg2CBRA; /*Number of CBRA preambles for which
                                                          no RA Msg2 was sent */
    UInt32 totalFailedRandomAccessPreamblesMsg2CFRA; /*Number of CFRA preambles for which
                                                          no RA Msg2 was sent */
    UInt32 totalAllocatedRandomAccessPreambles;    /*Number of allocated CFRA preambles. */
    UInt32 totalSuccRandomAccessTA[MAX_TA_RANGE];  /*Number of detected successfull random
                                                    access with a TA value in the range
                                                    0..9%, 10..19%, 20..29% and so on. */
    UInt32 totalUnassignedFalseCFRA;  /*The number of detected Contention Free Random Access */
    UInt32 totalUnassignedSumCFRA;
} RachSuccFailPerfStats;


/* Structure Name  : PdcchUtilizationStats
 *  * Purpose         : Contains PDCCH Utilizatin Statistics  */
typedef struct PdcchUtilizationStatsT
{
    UInt32 totalPdcchCceUtil[MAX_CCE_UTIL_INDEX]; /*Number of samples with PDCCH utilization
                                                      in the range 0-5%. */
    UInt32 noOfSubFrameWithCceUtilMoreThanZero; /*Number of subframes with PDCCH CCE
                                                  utilization greater than 0 CCEs.*/
    UInt32 noOfSubFrameWithPdcchCfi[MAX_CFI]; /*Number of subframes with PDCCH CFI set to 1,
                                                    CFI set to 2 and CFI set to 2.*/
    UInt32 totalCceAvail;  /* The total max number of CCEs available in a cell */
} PdcchUtilizationStats;

/* Structure Name  : CellPRBUsagePerfStats
 *  * Purpose         : Contains Cell PRB Usage Statistics  */
typedef struct CellPRBUsagePerfStatsT
{
    UInt32 totalULPRBUsageSRB[MAX_SRB];
    UInt32 totalDLPRBUsageSRB[MAX_SRB];
    UInt32 totalDLPRBUsagePCCH;
    UInt32 totalDLPRBUsageBCCH;
    UInt32 totalDLPRBAvailable;
    UInt32 totalULPRBAvailable;
} CellPRBUsagePerfStats;

/* Structure Name  : CellMacPerfStatsResp
 *  * Purpose         : Contains the statistics for Cell MAC Perf Stats   */
typedef struct CellMacPerfStatsRespT {
    /* SPR 15909 fix start */
    tickType_t durationTTI;      /*Indicates the duration for which PERF STATS
                                    need to be provided to OAM. */
    /* SPR 15909 fix end */
    CellHarqPerfStats cellHarqPerfStats; /*Indicates number of PRBs used for HARQ
                                           initial transmissions or retransmissions */
    RachSuccFailPerfStats rachSuccFailPerfStats; /*structure indicating the RACH related
                                                       Performance Stats */
    PdcchUtilizationStats pdcchUtilizationStats; /*structure indicating PDCCH Utilization Stats */
    UInt32 totalPUCCHResAllocationAttempts; /*Structure indicating PUCCH Utilization Stats */
    CellPRBUsagePerfStats cellPRBUsagePerfStats; /*Structure indicating cell PRB Usage stats */
} CellMacPerfStatsResp;


typedef struct MacCellPerfStatsIndParamsT {
    CellMacPerfStatsResp cellMacPerfStatsResp;  //Contains the statistics for Cell Mac Perf Stats
} MacCellPerfStatsIndParams ;

/***************************************************************************
 * Private Function Declaration 
 * *************************************************************************/
struct RrmMacCellConfigReq *rrmMacCellConfigReq;
struct RrmMacCellReconfigReq *p_rrmMacCellReconfigReq; 
struct RrmMacReconfigSchedulerReq *p_rrmMacReconfigSchedulerReq;
struct RrmMacUeReconfigReq *p_rrmMacUeReconfigReq;
struct RrmMacCnf *rrmMacCnf_p; 
struct RrmMacCellConfigCnf *rrmMacUEReConfigCnf_p;
struct RrmMacReconfigSchedulerCnf *rrmMacReconfigSchedulerCnf_p;
struct RrmMacUEReConfigCnf *rrmMacUeReconfigReq_p;

/*****************************************************************************
 * Function Name  : displayMenu 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function display the list of API which is going 
 *                  for process .  
 *****************************************************************************/
static void displayMenu()
{
    printf("\n 1.   Send RRM_MAC_CELL_CONFIG_REQ \n "); 
    printf("\n 2.   Send RRM_MAC_SCHEDULER_RECONFIG_REQ \n"); 
    printf("\n 3.   Send RRM_MAC_UE_RECONFIG_REQ \n");
    printf("\n 4.   TO RUN ALL API SIMULTANEOUSLY \n ");
    /* ECN changes start*/ 
    printf("\n 5.   Send RRM_PDCP_ECN_CONFIG_PARAM_REQ \n");
    printf("\n 6.   Send RRM_PDCP_UE_DRB_ECN_CE_REQ \n ");
    /* ECN changes end*/ 
    /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
    printf("\n 7.   MAC_CONFIGURE_PERF_STATS_REQ \n"); 
#endif
    printf("\n 8.   DISPLAY MENU \n"); 
    /* Perf stats RRM-MAC end */
}

/*****************************************************************************
 * Function Name  : PrepareMsgHeader 
 * Inputs         : Buffer and API ID 
 * Outputs        : None
 * Returns        : void
 * Description    : This function prepare the header of the respective 
 *                  RRM API.  
 *****************************************************************************/

static void prepareMsgHeader(UInt8 *buffer_p ,UInt16 msg_length, UInt16 apiId)
{

    //UInt8 current = 0;
    UInt16 rrm_module_id = 2;
    UInt16 mac_module_id = 7;
    /* arbitary value of transactionId */
    UInt16 transactionId = 5;
    
    L2RrmMsg *l2RrmMsg_p = PNULL;
    l2RrmMsg_p = (L2RrmMsg *)buffer_p;

    l2RrmMsg_p->header.transactionId = transactionId;
    l2RrmMsg_p->header.srcModuleId = rrm_module_id;
    l2RrmMsg_p->header.destModuleId = mac_module_id;
    l2RrmMsg_p->header.apiId = apiId;
    l2RrmMsg_p->header.length = msg_length; 

    printf("transactionId=%d,apiId=%d\n",transactionId,apiId);
    printf("\n\n**************************************\n\n");

}
/* ECN changes start*/
/*****************************************************************************
 * Function Name  : PreparePdcpMsgHeader 
 * Inputs         : Buffer and API ID 
 * Outputs        : None
 * Returns        : void
 * Description    : This function prepare the header of the respective 
 *                  RRM API.  
 *****************************************************************************/
static void preparePdcpMsgHeader(UInt8 *buffer_p , UInt16 apiId)
{

    UInt8 current = 0;
    UInt16 api_buf_size = 0;
    UInt16 rrm_module_id = 2;
    UInt16 pdcp_module_id = 5;
    /* arbitary value of transactionId */
    UInt16 transactionId = 5;


    buffer_p[current] = (transactionId & 0xFF00)>>8;
    current++;
    buffer_p[current] = (transactionId & 0x00FF);
    current++;

    buffer_p[current] = (rrm_module_id/*RRM_MODULE_ID*/ & 0xFF00)>>8;
    current++;
    buffer_p[current] = (rrm_module_id/*RRM_MODULE_ID*/ & 0x00FF);
    current++;

    buffer_p[current] = (pdcp_module_id/*RRM_MODULE_ID*/ & 0xFF00)>>8;
    current++;
    buffer_p[current] = (pdcp_module_id/*RRM_MODULE_ID*/ & 0x00FF);
    current++;


    buffer_p[current] = (apiId & 0xFF00)>>8;
    current++;
    buffer_p[current] = (apiId & 0x00FF);
    current++;

    api_buf_size = (UInt16)(api_buf_size + L2_RRM_API_HEADER_LEN);

    buffer_p[current] = (UInt8)((api_buf_size & 0xFF00) >> 8);
    current++;
    buffer_p[current] = (UInt8)(api_buf_size & 0x00FF);
    current++;

    printf("transactionId=%d,apiId=%d\n",transactionId,apiId);
    printf("\n\n**************************************\n\n");
}
/*****************************************************************************
 * Function Name  : parseForRrrmMacUeReconfigReq 
 * Inputs         : passsing the buffer structure of RrrmMacUeReconfigReq API 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the parameters of RrrmMacUeReconfigReq
 *                  reading from config file .    
 *****************************************************************************/
UInt16 parseForRrrmMacUeReconfigReq(RrmMacUeReconfigReq *msgBuff_p)
{

    UInt8 line[50] = {0};
    UInt8 *line_p = line;
    size_t len = 0;
    ssize_t read = 0;
    UInt8 temp[100];
UInt32 val=0;
//UInt8 val = 0;
    FILE* configFile_p = PNULL;
    UInt32 index = 0;


    printf(" code coming in parse fun \n");


    configFile_p = fopen("../configFile/rrmUeReconfig.txt","r");

    if(PNULL == configFile_p)
    {
        printf("FILE READ ERROR");
    }


    while ((read = getline((char **)&line_p, &len, configFile_p)) != -1)
    {

        val = 0;

        if (strncmp((const Char8 *)line_p,"#",1) == 0)
        {
            continue;
        }
        else if(strncmp((const Char8 *)line_p,"BITMASK",7) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+8,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->bitmask = val;
                printf(" value of bitmask = %d \n",msgBuff_p->bitmask);
            }    
        }
        else if(strncmp((const Char8 *)line_p,"UE_LOCATION_INFO_NUM_OF_UE",26) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+27,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueLocationInfo.numOfUE = val;
                printf("UE_LOCATION_INFO_NUM_OF_UE = %d \n ",
                        msgBuff_p->ueLocationInfo.numOfUE);
            }
        }
        else if(strncmp((const Char8 *)line_p,
                    "UE_LOCATION_INFO_UE_INDEX",25) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+26,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueLocationInfo.ueLocation[index].ueIndex = val;
                printf("UE_LOCATION_INFO_UE_INDEX = %d \n"
                        ,msgBuff_p->ueLocationInfo.ueLocation[index].ueIndex);
            }    
        }
        else if(strncmp((const Char8 *)line_p,
                    "UE_LOCATION_INFO_UE_LOCATION_TYPE",33) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+34,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueLocationInfo.ueLocation[index].ueLocationType = val;
                printf("UE_LOCATION_INFO_UE_LOCATION_TYPE = %d \n"
                        ,msgBuff_p->ueLocationInfo.ueLocation[index].ueLocationType);
            }    
            ++index;
            if(msgBuff_p->ueLocationInfo.numOfUE <= index)
            {
                break;
            }
        }
        /*UL_MU_MIMO_CHG Start*/
        else if(strncmp((const Char8 *)line_p,"UE_UL_MUMIMO_INFO_NUM_OF_UE",27) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+28,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueUlMUMIMOInfoList.numOfUE = val;
                printf("UE_UL_MUMIMO_INFO_NUM_OF_UE = %d \n ",
                        msgBuff_p->ueUlMUMIMOInfoList.numOfUE);
            }
        }
        else if(strncmp((const Char8 *)line_p,"UE_UL_MUMIMO_INFO_UE_INDEX",26) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+27,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {
                msgBuff_p->ueUlMUMIMOInfoList.ueUlMUMIMOInfo[index].ueIndex = val;
                printf("UE_UL_MUMIMO_INFO_UE_INDEX = %d \n",
                        msgBuff_p->ueUlMUMIMOInfoList.ueUlMUMIMOInfo[index].ueIndex);
            }
        }
        else if(strncmp((const Char8 *)line_p,"UE_UL_MUMIMO_INFO_STATUS",24) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+25,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueUlMUMIMOInfoList.ueUlMUMIMOInfo[index].ulMUMIMOStatus = val;
                printf("UE_UL_MUMIMO_INFO_STATUS = %d \n"
                        ,msgBuff_p->ueUlMUMIMOInfoList.ueUlMUMIMOInfo[index].ulMUMIMOStatus);
            }    
            ++index;
            if(msgBuff_p->ueUlMUMIMOInfoList.numOfUE <= index)
            {
                break;
            }
        }
        /*UL_MU_MIMO_CHG End*/
    }  
    return 0;
}


/*****************************************************************************
 * Function Name  : parseForRrmPdcpEcnConfigReq 
 * Inputs         : passsing the buffer structure of RrrmMacUeReconfigReq API 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the parameters of RrmPdcpEcnConfigReq
 *                  reading from config file .    
 *****************************************************************************/
UInt16 parseForRrmPdcpEcnConfigReq(RrmPdcpEcnConfigParamReq *msgBuff_p)
{

    msgBuff_p->ecnFeatureStatus = ECN_APPLIED_IN_BOTH_UL_DL;
    msgBuff_p->ecnTrigger       = ECN_BOTH_TRIGGERS_APPLIED;
    msgBuff_p->lowWaterMark     = 2;
    msgBuff_p->highWaterMark    = 3;
    
    return 0;
}

/*****************************************************************************
 * Function Name  : parseForRrmPdcpDrbEcnCeReq 
 * Inputs         : passsing the buffer structure of RrrmMacUeReconfigReq API 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the parameters of RrmPdcpEcnConfigReq
 *                  reading from config file .    
 *****************************************************************************/
UInt16 parseForRrmPdcpDrbEcnCeReq(RrmPdcpDrbEcnCeReq *msgBuff_p)
{
    UInt8 i = 0;
    msgBuff_p->ueIndex       =  0  ;
    msgBuff_p->numOfLc       =  1   ;
    for(; i < msgBuff_p->numOfLc ; i++)
    {    
       msgBuff_p->ceLc[i].lcId              =  3  ;
       msgBuff_p->ceLc[i].direction         =  2   ;
       msgBuff_p->ceLc[i].congestionStatus  =  0x01  ;
    }   

/*    UInt8 line[50] = {0};
    UInt8 *line_p = line;
    size_t len = 0;
    ssize_t read = 0;
    UInt8 temp[100];
    UInt8 val = 0;
    FILE* configFile_p = PNULL;
    UInt32 index = 0;
    
    printf(" code coming in parse fun \n");

    configFile_p = fopen("../configFile/rrmUeReconfig.txt","r");

    if(PNULL == configFile_p)
    {
        printf("FILE READ ERROR");
    }


    while ((read = getline((char **)&line_p, &len, configFile_p)) != -1)
    {

        val = 0;

        if (strncmp((const Char8 *)line_p,"#",1) == 0)
        {
            continue;
        }
        else if(strncmp((const Char8 *)line_p,"BITMASK",7) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+8,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->bitmask = val;
                printf(" value of bitmask = %d \n",msgBuff_p->bitmask);
            }    
        }
        else if(strncmp((const Char8 *)line_p,"UE_LOCATION_INFO_NUM_OF_UE",26) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+27,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueLocationInfo.numOfUE = val;
                printf("UE_LOCATION_INFO_NUM_OF_UE = %d \n ",
                        msgBuff_p->ueLocationInfo.numOfUE);
            }
        }
        else if(strncmp((const Char8 *)line_p,
                    "UE_LOCATION_INFO_UE_INDEX",25) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+26,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueLocationInfo.ueLocation[index].ueIndex = val;
                printf("UE_LOCATION_INFO_UE_INDEX = %d \n"
                        ,msgBuff_p->ueLocationInfo.ueLocation[index].ueIndex);
            }    
        }
        else if(strncmp((const Char8 *)line_p,
                    "UE_LOCATION_INFO_UE_LOCATION_TYPE",33) == 0)
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+34,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ueLocationInfo.ueLocation[index].ueLocationType = val;
                printf("UE_LOCATION_INFO_UE_LOCATION_TYPE = %d \n"
                        ,msgBuff_p->ueLocationInfo.ueLocation[index].ueLocationType);
            }    
            ++index;
            if(msgBuff_p->ueLocationInfo.numOfUE <= index)
            {
                break;
            }
        }
    }  */
    return 0;
}



/******************************************************************************
* Function Name  : initSinrToTpcMap
* Inputs         : deltaSINRToTPCMapForPUCCH_p - Pointer to
*                  DeltaSINRToTPCMapForPUCCH populated from OAM interface
* Outputs        : None
* Returns        : Void
* Description    : This function based on the target Sinr , populates the
*                  deltaSinrToPucchTpcMap_g map.
******************************************************************************/
void initSinrToTpcMap(DeltaSINRToTPCMapForPUCCH * deltaSINRToTPCMapForPUCCH_p,
        MacReconfigSchedulerParamsDL *schedulerParams_p)
{
    UInt8 targetSinr = deltaSINRToTPCMapForPUCCH_p->pucchSINRTarget;
    UInt8 count = deltaSINRToTPCMapForPUCCH_p->count;
    /* SPR  3444 changes start */
    UInt16 index = 0;
    /* SPR  3444 changes end */
    SInt16 startIndex = 0;
    SInt16 endIndex = 0;

    while (count)
    {
        count--;
        DeltaSINRToTPCMapForPUCCHInfo * deltaSINRToTPCMap_p = \
            &(deltaSINRToTPCMapForPUCCH_p->\
                  deltaSinrToTPCMapForPUCCHInfo[count]);

        if (deltaSINRToTPCMap_p->startSINR > \
               (NUM_OF_SINR_TPC_VALUES - targetSinr))
        {
            continue;
        }

        if (deltaSINRToTPCMap_p->startSINR < -1*targetSinr)
        {
            startIndex = 0;
        }
        else
        {
            startIndex = deltaSINRToTPCMap_p->startSINR + targetSinr;
        }

        if (deltaSINRToTPCMap_p->endSINR > \
               (NUM_OF_SINR_TPC_VALUES - targetSinr))
        {
            endIndex = NUM_OF_SINR_TPC_VALUES ;
        }
        else
        {
            endIndex = deltaSINRToTPCMap_p->endSINR + targetSinr;
        }

        for (index =  startIndex; index <= endIndex; index++)
        {
#if 0
            //jyotesh
deltaSinrToPucchTpcMap_g[!deltaSinrToPucchTpcMapIndex_g][index] = deltaSINRToTPCMap_p->pucchTPC;
#endif

            schedulerParams_p->deltaSinrToPucchTpcMap[index] = 
                deltaSINRToTPCMap_p->pucchTPC;
        }
    } /* While End */
}

/*****************************************************************************
* Function Name  : initHarqBlerToTPCMap
* Inputs         : deltaBLERToTPCMapForPUCCH_p - Pointer to
*                  DeltaBLERToTPCMapForPUCCH populated from OAM interface
* Outputs        : None
* Returns        : Void
* Description    : This function based on the target Sinr , populates the
*                  deltaHarqBlerToPucchTpcMap_g map.
*****************************************************************************/
void initHarqBlerToTPCMap(
         DeltaBLERToTPCMapForPUCCH * deltaBLERToTPCMapForPUCCH_p,
         MacReconfigSchedulerParamsDL *schedulerParams_p)

{
    UInt8 targetBler = deltaBLERToTPCMapForPUCCH_p->pucchBLERTarget;
    UInt8 count = deltaBLERToTPCMapForPUCCH_p->count;
    /* SPR  3444 changes start */
    UInt16 index = 0;
    /* SPR  3444 changes end */
    SInt16 startIndex = 0;
    SInt16 endIndex = 0;

    while (count)
    {
        count--;
        DeltaBLERToTPCMapForPUCCHInfo * deltaBLERToTPCMap_p =
            &(deltaBLERToTPCMapForPUCCH_p->\
                  deltaBlerToTPCMapForPUCCHInfo[count]);

        if (deltaBLERToTPCMap_p->startBLER > \
                (NUM_OF_BLER_TPC_VALUES - targetBler))
        {
            continue;
        }

        if (deltaBLERToTPCMap_p->startBLER < -1*targetBler)
        {
            startIndex = 0;
        }
        else
        {
            startIndex = deltaBLERToTPCMap_p->startBLER + targetBler;
        }

        if (deltaBLERToTPCMap_p->endBLER > \
               (NUM_OF_BLER_TPC_VALUES - targetBler))
        {
            endIndex = NUM_OF_BLER_TPC_VALUES ;
        }
        else
        {
            endIndex = deltaBLERToTPCMap_p->endBLER + targetBler;
        }

        for (index =  startIndex; index <= endIndex; index++)
        {
#if 0
            deltaHarqBlerToPucchTpcMap_g[index]=deltaBLERToTPCMap_p->pucchTPC;
#endif
            schedulerParams_p->deltaHarqBlerToPucchTpcMap[index] =
                deltaBLERToTPCMap_p->pucchTPC;

        }
    } /* While End */
}




/*****************************************************************************
 * Function Name  : parseForRrmMacReconfigSchedulerReq 
 * Inputs         : passsing the buffer structure of RrmMacReconfigSchedulerReq API 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the parameters of RrmMacReconfigSchedulerReq
 *                  reading from config file .    
 *****************************************************************************/

UInt16 parseForRrmMacReconfigSchedulerReq(RrmMacReconfigSchedulerReq *msgBuff_p)
{

    UChar8 line[50] = {0};
    UChar8 *line_p = line;
    size_t len = 0;
    ssize_t read = 0;
    UInt8 temp[100];
    UInt32 val = 0;
    FILE* configFile_p = PNULL;

    //UInt32 minRbForPlPhrCalc=0;
    printf(" code coming in parse fun \n");

    configFile_p = fopen("../configFile/rrmSchedularReconfig.txt","r");


    if(NULL == configFile_p)
    {
        printf("FILE READ ERROR");
    }


    while ((read = getline((char **)&line_p, &len, configFile_p)) != -1)
    {

        val = 0;

        if (strncmp((const Char8 *)line_p,"#",1) == 0)
        {
            continue;
        }
        else if(strncmp((const Char8 *)line_p,"SCHEDULAR_PARAM_BIT_MAP",23) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+24,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->schedulerParamBitMap = val;
                printf(" value of SCHEDULAR_PARAM_BIT_MAP = %d \n",
                        msgBuff_p->schedulerParamBitMap);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,"TPC_FOR_MSG3",12) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+13,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->tpcForMsg3 = val;
                printf(" value of TPC_FOR_MSG3 = %d \n",msgBuff_p->tpcForMsg3);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,"SRS_SUB_BSND_GROUP_SIZE",23) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+24,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->srsSubBandGroupSize = val;
                printf(" value of SRS_SUB_BSND_GROUP_SIZE = %d \n",
                        msgBuff_p->srsSubBandGroupSize);
            }    
            continue ;
        }


        else if(strncmp((const Char8 *)line_p,
                    "TPC_TRIGGER_WAIT_TIME_FOR_PUSCH_DCI0",36) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+37,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->tpcTriggerWaitTimeForPUSCHDCI0 = val;
                printf(" value of TPC_TRIGGER_WAIT_TIME_FOR_PUSCH_DCI0 = %d \n",
                        msgBuff_p->tpcTriggerWaitTimeForPUSCHDCI0);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,
                    "TPC_TRIGGER_WAIT_TIME_FOR_PUCCH_DCI_1X_2X",41) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+42,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->tpcTriggerWaitTimeForPUCCHDCI1x2x = val;
                printf(" value of TPC_TRIGGER_WAIT_TIME_FOR_PUCCH_DCI_1X_2X = %d \n",
                        msgBuff_p->tpcTriggerWaitTimeForPUCCHDCI1x2x);
            }    
            continue ;
        }
        
        else if(strncmp((const Char8 *)line_p,
                    "TPC_TRIGGER_WAIT_TIME_FOR_PUSCH_DCI_3A",38) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+39,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {  
                msgBuff_p->tpcNumUeThresholdForPUSCHDCI3A = val;
                printf(" value of TPC_TRIGGER_WAIT_TIME_FOR_PUSCH_DCI_3A = %d \n",
                        msgBuff_p->tpcNumUeThresholdForPUSCHDCI3A);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,
                    "TPC_TRIGGER_WAIT_TIME_FOR_PUCCH_DCI_3A",38) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+39,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {  
                msgBuff_p->tpcNumUeThresholdForPUCCHDCI3A = val;
                printf(" value of TPC_TRIGGER_WAIT_TIME_FOR_PUCCH_DCI_3A = %d \n",
                        msgBuff_p->tpcNumUeThresholdForPUCCHDCI3A);
            }    
            continue ;
        }
        
        else if(strncmp((const Char8 *)line_p,
                    "COOL_OFF_PERIOD_FOR_BLER_TRIGGERED_PUCCH_TPC",44) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+45,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->coolOffPeriodForBLERTriggeredPUCCHTPC = val;
                printf(" value of COOL_OFF_PERIOD_FOR_BLER_TRIGGERED_PUCCH_TPC = %d \n",
                        msgBuff_p->coolOffPeriodForBLERTriggeredPUCCHTPC);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,"COOL_OFF_PERIOD_FOR_PUCCH_TPC",29) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+30,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->coolOffPeriodForPUCCHTPC = val;
                printf(" value of COOL_OFF_PERIOD_FOR_PUCCH_TPC = %d \n",
                        msgBuff_p->coolOffPeriodForPUCCHTPC);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,
                    "COOL_OFF_PERIOD_FOR_PUSCH_TPC",29) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+30,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->coolOffPeriodForPUCCHTPC = val;
                printf(" value of COOL_OFF_PERIOD_FOR_PUSCH_TPC = %d \n",
                        msgBuff_p->coolOffPeriodForPUCCHTPC);
            }    
            continue ;
        }
   
    
        else if(strncmp((const Char8 *)line_p,
                    "PDCCH_AGGREGATION_AND_POWER_OFFSET_CE_BIT",41) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+30,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->pdcchAggregationAndPowerOffset.bitmask = val; 
                printf(" value of PDCCH_AGGREGATION_AND_POWER_OFFSET_CE_BIT = %d \n",
                        msgBuff_p->pdcchAggregationAndPowerOffset.bitmask);
            }    
            continue ;
        }

        /*UL_MU_MIMO_CHG_START*/
        else if(strncmp((const Char8 *)line_p,
                    "UPLINK_MULTI_USER_MIMO_TYPE",27) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+28,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->ulMUMIMOType = val;
                printf(" value of UPLINK_MULTI_USER_MIMO_TYPE = %d \n",
                        msgBuff_p->ulMUMIMOType);
            }    
            continue ;
        }
        else if(strncmp((const Char8 *)line_p,
                    "UPLINK_MULTI_USER_MIMO_MCS_THRESHOLD",36) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+37,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {    
                msgBuff_p->mcsThresholdULMUMIMO= val;
                printf(" value of UPLINK_MULTI_USER_MIMO_MCS_THRESHOLD= %d \n",
                        msgBuff_p->mcsThresholdULMUMIMO);
            }    
            continue ;
        }   
        /*UL_MU_MIMO_CHG_END*/
     

        /* +DYNAMIC_ICIC */


        else if(strncmp((const Char8 *)line_p,"PAQPSKINFO",10) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+11,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {
                msgBuff_p->paQpsk = val;
                printf(" value of PAQPSKINFO = %d \n",
                        msgBuff_p->paQpsk);
            }
            continue ;
        }


        else if(strncmp((const Char8 *)line_p,
                    "ATBCONFIG_MIN_MCS_INDEX_FOR_ATB" , 31 ) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+32,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {
                msgBuff_p->atbConfig.minMCSIndexForATB = val;
                printf(" value of ATBCONFIG_MIN_MCS_INDEX_FOR_ATB = %d \n",
                        msgBuff_p->atbConfig.minMCSIndexForATB);
            }
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,
                    "ATBCONFIG_MIN_PRB_VAL_FOR_ATB",29) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+30,read);
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);

            if(val >= 0)
            {
                msgBuff_p->atbConfig.minPRBValForATB = val;
                printf(" value of ATBCONFIG_MIN_PRB_VAL_FOR_ATB = %d \n",
                        msgBuff_p->atbConfig.minPRBValForATB);
            }
            continue ;
        }

        /* -DYNAMIC_ICIC */
    }

  /* reading rest of parameters from global variable */

    msgBuff_p->alphaBasedPathLossToTargetSINRMap = alphaBasedPathLossToTargetSINRMap;
    msgBuff_p->deltaSINRToTPCMapForPUSCH = deltaSINRToTPCMapForPUSCH;
    msgBuff_p->deltaSINRToTPCMapForPUCCH = deltaSINRToTPCMapForPUCCH;
    msgBuff_p->deltaBlerToTPCMapForPUCCH = blerToTPCMapForPUCCH;
  //  msgBuff_p->pdcchAggregationAndPowerOffset = pdcchAggregationAndPowerOffset_g;
    memset(&msgBuff_p->cqiToPHICHPower,0,sizeof(CqiToPHICHPower));
 // msgBuff_p->cqiToPHICHPower=
    msgBuff_p->minRbForPlPhrCalc = 20; /* should verified by vishal */
    /* +DYNAMIC_ICIC */
    msgBuff_p->paQpsk = 5;
    msgBuff_p->atbConfig.minMCSIndexForATB = 17;
    msgBuff_p->atbConfig.minPRBValForATB = 77;
    /* -DYNAMIC_ICIC */
     
#if 0   
    memCpy(&msgBuff_p->pdcchAggregationAndPowerOffset[CC_USER].aggregationPowerOffset,
                    &rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                    aggregationPowerOffsetCCUser,
                    sizeof(AggregationPowerOffset) * NUM_DCI_SIZE_CATEGORY * MAX_CQI_VAL);

#endif     
   
    memcpy(&msgBuff_p->pdcchAggregationAndPowerOffset.aggregationPowerOffsetCCUser,
            &CCpdcchAggregationAndPowerOffset,sizeof(CCpdcchAggregationAndPowerOffset));

    memcpy(&msgBuff_p->pdcchAggregationAndPowerOffset.aggregationPowerOffsetCEUser,
            &CEpdcchAggregationAndPowerOffset,sizeof(CEpdcchAggregationAndPowerOffset));

return 0;
}


/*****************************************************************************
 * Function Name  : parseForCellConfigReq 
 * Inputs         : passsing the buffer structure of RrmMacCellConfigReq API 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the parameters of RrmMacCellConfigReq
 *                  reading from config file .    
 *****************************************************************************/

UInt16  parseForCellConfigReq(RrmMacCellConfigReq *msgBuff_p)
{
    UChar8 line[50] = {0};
    UChar8 *line_p = line;
    size_t len = 0;
    ssize_t read = 0;
    UInt8 temp[100];
    UInt32 val = 0;
    
    FILE* configFile_p = PNULL;
    /* +DYNAMIC ICIC*/
    Char8 *str_v;
    UInt32 valCenter = 0 , valCenter1=0;
    UInt32 DL_flag = 0 , UL_flag=0 ,Periodic_flag = 0 , L1_flag=0 , L2_flag=0 , UL_powerFlag=0;
    UInt32 flag=0;
    UInt32 L1RepReqd=0 , L1RepAvgEnabled = 0 , L1RepPeriodicity = 0 , L2CellEdgePrbUsage = 0 , L2RntpReport = 0 , L2Periodicity = 0  , RntpRepReqd = 0 , RntpThreshold = 0 , RntpMaxNominalEPRE = 0;
    
    /* +DYNAMIC ICIC*/
    
    printf(" code coming in parse fun \n");

    configFile_p = fopen("../configFile/rrmCellConfig.txt","r");

    if(NULL == configFile_p)
    {
        printf("FILE READ ERROR");
    }


    while ((read = getline((char **)&line_p, &len, configFile_p)) != -1)
    {

        val = 0;
        

        if (strncmp((const Char8 *)line_p,"#",1) == 0)
        {
            continue;
        }
    /* +DYNAMIC ICIC*/
        else if(strncmp((const Char8 *)line_p,"BITMASK",7) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+8,read);
            val = atoi((Char8 *)temp);
            DL_flag = val & DL_RESOURCE_INFO_BITMASK;
           
            UL_flag = val & UL_RESOURCE_INFO_BITMASK;
            printf("Val = %d \n",DL_flag);  
            Periodic_flag = val & PERIODIC_REPORT_CONFIG_BITMASK;
            L1_flag = val & L1_REPORT_CONFIG_BITMASK;
            L2_flag = val & ICIC_REPORT_CONFIG_BITMASK;
            UL_powerFlag = val & CONFIG_UL_POWER_BITMASK;

            
            printf(" value of val = %d \n",val);

                
            if(val >= 0)
            {    
                msgBuff_p->bitmask = val;
                printf(" value of bitmask = %d \n",msgBuff_p->bitmask);
                    
                continue ;
            }    
        }
    /* +DYNAMIC ICIC*/

        else if((strncmp((const Char8 *)line_p,"DL_RES_PARTITION_BITMASK",24) == 0) && (DL_flag !=0 ))
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+25,read);

            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.bitmask = val;
                printf("DL_RES_PARTITION_BITMASK = %d \n ",
                        msgBuff_p->dlResourcePartitionInfo.bitmask);
            }    
            continue ;
    
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_CENTRE_REGION",35) == 0) && (DL_flag !=0 ) )
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+36,read);

            val = atoi((Char8 *)temp);
            valCenter = val;
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.numOfCellCenterRegion = val;
                printf(" value of DL_RES_PARTITION_CELL_CENTRE_REGION = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.numOfCellCenterRegion);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_EDGE_REGION",33) == 0) && (DL_flag !=0 ))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+34,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.numOfCellEdgeRegion = val;
                printf(" value of DL_RES_PARTITION_CELL_EDGE_REGION = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.numOfCellEdgeRegion);
            }    
            continue ;
        }

    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"DL_RES_PARTITION_CELL_CENTRE_0_START_RB",39) == 0) && ( DL_flag!=0) )
        {    
            char strDlSrc[100];
            strcpy(strDlSrc,"DL_RES_PARTITION_CELL_CENTRE_");
            char strDlTrgt[100],strDlTrgt1[100];
                     
            int dlCellCentre = 0;            
            while(dlCellCentre<valCenter)
            {
                     strcpy(strDlTrgt , "");
                     strcpy(strDlTrgt,strDlSrc);
                     
                     sprintf(strDlTrgt1 ,"%d" , dlCellCentre);
                    
                     char strDlTrgt2[20]="_START_RB";
                     strcat(strDlTrgt , strDlTrgt1);  
                     strcat(strDlTrgt,strDlTrgt2);
                     
                     if(strncmp((const Char8 *)line_p, strDlTrgt , 39) == 0)
                     {          
                            
                                   strncpy((Char8 *)temp,(const Char8 *)line_p+40,read);
                                   val = atoi((Char8 *)temp);
                                   if(val >= 0)
                                   {    
                                         msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[dlCellCentre].startRB = val;
                                         printf(" value of DL_RES_PARTITION_CELL_CENTRE_%d_START_RB = %d \n",dlCellCentre,
                                         msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[dlCellCentre].startRB);
                                   }    
                 
                     }   
                     else 
                     { flag=1;
                       break;
                     }  
                 
                
                 if ((read = getline((char **)&line_p, &len, configFile_p)) != -1)
                 {
                     
                     strcpy(strDlTrgt , "");
                     strcpy(strDlTrgt,strDlSrc);
                    
                     sprintf(strDlTrgt1 ,"%d" , dlCellCentre);
                     
                     char strDlTrgt2[20]="_NUM_OF_RB";
                     strcat(strDlTrgt , strDlTrgt1);  
                     strcat(strDlTrgt,strDlTrgt2);
                    
                     if(strncmp((const Char8 *)line_p, strDlTrgt ,40) == 0)
                     {          
                            
                                   strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);
                                   val = atoi((Char8 *)temp);
                                   if(val >= 0)
                                   {    
                                         msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[dlCellCentre].numOfRB = val;
                                         printf(" value of DL_RES_PARTITION_CELL_CENTRE_%d_NUM_OF_RB = %d \n",dlCellCentre,
                                         msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[dlCellCentre].numOfRB);
                                   }    
                 
                     }
                  
                
                }
                 read = getline((char **)&line_p, &len, configFile_p);
                 dlCellCentre++;
                 
          }
            if(flag ==1 || (dlCellCentre == valCenter))
           {
                      if((strncmp((const Char8 *)line_p, "DL_RES_PARTITION_CELL_EDGE_START_RB",35) == 0)&& (DL_flag!=0))
                      {
                                           strncpy((Char8 *)temp,(const Char8 *)line_p+36,read);

                                           val = atoi((Char8 *)temp);
                                           if(val >= 0)
                                             {    
                                                    msgBuff_p->dlResourcePartitionInfo.CellEdgeRegion.startRB = val;
                                                    printf(" value of DL_RES_PARTITION_CELL_EDGE_START_RB = %d \n",
                                                            msgBuff_p->dlResourcePartitionInfo.CellEdgeRegion.startRB);
                                              }
                                           flag =0;
                      } 
           }          
            continue ;
        
        }        
        /*
        else if(strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_CENTRE_0_NUM_OF_RB",40) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[0].numOfRB = val;
                printf(" value of DL_RES_PARTITION_CELL_CENTRE_0_NUM_OF_RB = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[0].numOfRB);
            }    
            continue ;
        }
        else if(strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_CENTRE_1_START_RB",39) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+40,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[1].startRB = val;
                printf(" value of DL_RES_PARTITION_CELL_CENTRE_1_START_RB = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[1].startRB);
            }    
            continue ;
        }
        else if(strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_CENTRE_1_NUM_OF_RB",40) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[1].numOfRB = val;
                printf(" value of DL_RES_PARTITION_CELL_CENTRE_1_NUM_OF_RB = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.CellCenterRegion[1].numOfRB);
            }    
            continue ;
        } 
        */
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_EDGE_START_RB",35) == 0)&& (DL_flag!=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+36,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.CellEdgeRegion.startRB = val;
                printf(" value of DL_RES_PARTITION_CELL_EDGE_START_RB = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.CellEdgeRegion.startRB);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "DL_RES_PARTITION_CELL_EDGE_NUM_OF_RB",36) == 0) && (DL_flag!=0) )
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+37,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->dlResourcePartitionInfo.CellEdgeRegion.numOfRB = val;
                printf(" value of DL_RES_PARTITION_CELL_EDGE_NUM_OF_RB = %d \n",
                        msgBuff_p->dlResourcePartitionInfo.CellEdgeRegion.numOfRB);
            }    
            continue ;
        }
        /*  UL Configuration */

    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"UL_RES_PARTITION_BITMASK",24) == 0) && (UL_flag!=0))
        {

            strncpy((Char8 *)temp,(const Char8 *)line_p+25,read);
            printf("entered");
            val = atoi((Char8 *)temp);
            printf(" value of val = %d \n",val);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.bitmask = val;
                printf("UL_RES_PARTITION_BITMASK = %d \n ",
                        msgBuff_p->ulResourcePartitionInfo.bitmask);
            }    
            continue ;
        }
       
     
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_CENTRE_REGION",35) == 0) && (UL_flag !=0) )
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+36,read);

            val = atoi((Char8 *)temp);
            valCenter1=val;
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.numOfCellCenterRegion = val;
                printf(" value of UL_RES_PARTITION_CELL_CENTRE_REGION = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.numOfCellCenterRegion);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_EDGE_REGION",33) == 0) && (UL_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+34,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.numOfCellEdgeRegion = val;
                printf(" value of UL_RES_PARTITION_CELL_EDGE_REGION = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.numOfCellEdgeRegion);
            }    
            continue ;
        }


    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"UL_RES_PARTITION_CELL_CENTRE_0_START_RB",39) == 0)&& (UL_flag !=0) )
        {    
            char strUlSrc[100];
            strcpy(strUlSrc,"UL_RES_PARTITION_CELL_CENTRE_");
            char strUlTrgt[100],strUlTrgt1[100];
            UInt32 flag1 =0;
                     
            int ulCellCentre=0;            
            while(ulCellCentre<valCenter1)
            {
                     strcpy(strUlTrgt , "");
                     strcpy(strUlTrgt,strUlSrc);
                     
                     sprintf(strUlTrgt1 ,"%d" , ulCellCentre);
                    
                     char strUlTrgt2[20]="_START_RB";
                     strcat(strUlTrgt , strUlTrgt1);  
                     strcat(strUlTrgt,strUlTrgt2);
                     
                     if(strncmp((const Char8 *)line_p, strUlTrgt , 39) == 0)
                     {          
                            
                                   strncpy((Char8 *)temp,(const Char8 *)line_p+40,read);
                                   val = atoi((Char8 *)temp);
                                   if(val >= 0)
                                   {    
                                         msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[ulCellCentre].startRB = val;
                                         printf(" value of UL_RES_PARTITION_CELL_CENTRE_%d_START_RB = %d \n",ulCellCentre,
                                         msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[ulCellCentre].startRB);
                                   }    
                 
                     }    
                     else
                     {
                             flag1 =1;
                             break;
                     }
                
                 if ((read = getline((char **)&line_p, &len, configFile_p)) != -1)
                 {
                     
                     strcpy(strUlTrgt , "");
                     strcpy(strUlTrgt,strUlSrc);
                    
                     sprintf(strUlTrgt1 ,"%d" , ulCellCentre);
                     
                     char strUlTrgt2[20]="_NUM_OF_RB";
                     strcat(strUlTrgt , strUlTrgt1);  
                     strcat(strUlTrgt,strUlTrgt2);
                    
                     if(strncmp((const Char8 *)line_p, strUlTrgt ,40) == 0)
                     {          
                            
                                   strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);
                                   val = atoi((Char8 *)temp);
                                   if(val >= 0)
                                   {    
                                         msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[ulCellCentre].numOfRB = val;
                                         printf(" value of UL_RES_PARTITION_CELL_CENTRE_%d_NUM_OF_RB = %d \n",ulCellCentre,
                                         msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[ulCellCentre].numOfRB);
                                   }    
                 
                     }
                }
                 read = getline((char **)&line_p, &len, configFile_p);
                 ulCellCentre++;
                 
          }


            if(flag1 ==1 || (ulCellCentre == valCenter1))
           {
                      if((strncmp((const Char8 *)line_p, "UL_RES_PARTITION_CELL_EDGE_START_RB",35) == 0)&& (UL_flag!=0))
                      {
                                           strncpy((Char8 *)temp,(const Char8 *)line_p+36,read);

                                           val = atoi((Char8 *)temp);
                                           if(val >= 0)
                                             {    
                                                    msgBuff_p->ulResourcePartitionInfo.CellEdgeRegion.startRB = val;
                                                    printf(" value of UL_RES_PARTITION_CELL_EDGE_START_RB = %d \n",
                                                            msgBuff_p->ulResourcePartitionInfo.CellEdgeRegion.startRB);
                                              }
                                           flag1 =0;
                      } 
           }          
        }        
        /*
        else if(strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_CENTRE_0_START_RB",39) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+40,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[0].startRB = val;
                printf(" value of UL_RES_PARTITION_CELL_CENTRE_START_RB = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[0].startRB);
            }    
            continue ;
        }
        else if(strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_CENTRE_0_NUM_OF_RB",40) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[0].numOfRB = val;
                printf(" value of UL_RES_PARTITION_CELL_CENTRE_0_NUM_OF_RB = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[0].numOfRB);
            }    
            continue ;
        }

        else if(strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_CENTRE_1_START_RB",39) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+40,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[1].startRB = val;
                printf(" value of UL_RES_PARTITION_CELL_CENTRE_1_START_RB = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[1].startRB);
            }    
            continue ;
        }
        else if(strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_CENTRE_1_NUM_OF_RB",40) == 0)
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[1].numOfRB = val;
                printf(" value of UL_RES_PARTITION_CELL_CENTRE_1_NUM_OF_RB = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.CellCenterRegion[1].numOfRB);
            }    
            continue ;
        }
        */
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_EDGE_START_RB",35) == 0) && (UL_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+36,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.CellEdgeRegion.startRB = val;
                printf(" value of UL_RES_PARTITION_CELL_EDGE_START_RB = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.CellEdgeRegion.startRB);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,
                    "UL_RES_PARTITION_CELL_EDGE_NUM_OF_RB",36) == 0) && (UL_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+37,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulResourcePartitionInfo.CellEdgeRegion.numOfRB = val;
                printf(" value of UL_RES_PARTITION_CELL_EDGE_NUM_OF_RB = %d \n",
                        msgBuff_p->ulResourcePartitionInfo.CellEdgeRegion.numOfRB);
            }    
            continue ;
        }

        /*   PeriodicReportConfig  */

    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"PERIODIC_REP_BITMASK",20) == 0)  && (Periodic_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+21,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->periodicReportConfig.bitmask = val;
                printf(" value of PERIODIC_REP_BITMASK = %d \n",
                        msgBuff_p->periodicReportConfig.bitmask);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"PERIODIC_REP_REQUIRED",21) == 0) && (Periodic_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+22,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->periodicReportConfig.periodicReportRequired = val;
                printf(" value of PERIODIC_REP_REQUIRED = %d \n",
                        msgBuff_p->periodicReportConfig.periodicReportRequired);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"PERIODIC_REP_PERIODICITY",24) == 0) && (Periodic_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+25,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->periodicReportConfig.periodicity = val;
                    printf(" value of PERIODIC_REP_PERIODICITY = %d \n",
                        msgBuff_p->periodicReportConfig.periodicity);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"PERIODIC_REP_UE_BATCH_SIZE",26) == 0) && (Periodic_flag !=0) )
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+27,read);

            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->periodicReportConfig.ueBatchSize = val;
                printf(" value of PERIODIC_REP_PERIODICITY = %d \n",
                        msgBuff_p->periodicReportConfig.ueBatchSize);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER1_MEAS_REP_BITMASK",23) == 0) && (L1_flag !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+24,read);
            val = atoi((Char8 *)temp);
            L1RepReqd=val & L1_MEAS_REPORT_REQD_BITMASK;
            L1RepPeriodicity=val & L1_MEAS_REPORT_PERIODICITY_BITMASK;
            L1RepAvgEnabled=val & L1_MEAS_REPORT_AVERAGING_ENABLED_BITMASK;
            if(val >= 0)
            {    
                msgBuff_p->layer1MeasReportConfig.bitmask = val;
                printf(" value of LAYER1_MEAS_REP_BITMASK = %d \n",
                         msgBuff_p->layer1MeasReportConfig.bitmask);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER1_MEAS_REP_REQUIRED",24) == 0)&& (L1RepReqd !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+25,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer1MeasReportConfig.layer1MeasReportRequired= val;
                printf(" value of LAYER1_MEAS_REP_REQUIRED = %d \n",
                        msgBuff_p->layer1MeasReportConfig.layer1MeasReportRequired);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER1_MEAS_REP_AVG_ENABLED",27) == 0) && (L1RepAvgEnabled !=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+28,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer1MeasReportConfig.layer1ReportAveragingEnabled = val;
                printf(" value of LAYER1_MEAS_REP_AVG_ENABLED = %d \n",
                        msgBuff_p->layer1MeasReportConfig.layer1ReportAveragingEnabled);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER1_MEAS_REP_PERIODICITY",27) == 0) && (L1RepPeriodicity!=0))
        {
            strncpy((Char8 *)temp,(const Char8 *)line_p+28,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer1MeasReportConfig.periodicity = val;
                printf(" value of LAYER1_MEAS_REP_PERIODICITY = %d \n",
                        msgBuff_p->layer1MeasReportConfig.periodicity);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_BITMASK",19) == 0) && (L2_flag !=0))
                {
            strncpy((Char8 *)temp,(const Char8 *)line_p+20,read);
            val = atoi((Char8 *)temp);
            L2CellEdgePrbUsage=val & CE_PRB_USAGE_REQD_BITMASK;
            L2RntpReport=val & RNTP_CONFIG_BITMASK;
            L2Periodicity=val & L2_ICIC_MEAS_REPORT_PERIODICITY_BITMASK;
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.bitmask = val;
                printf(" value of LAYER2_MEAS_BITMASK = %d \n",
                        msgBuff_p->layer2IcicMeasReportConfig.bitmask);
            }    
            continue ;

         }   
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_CELLEDGE_PRB_USAGE_REQUIRED",39) == 0) && (L2CellEdgePrbUsage !=0))
         {
            strncpy((Char8 *)temp,(const Char8 *)line_p+40,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.cellEdgePrbUsageRequired = val;
                printf(" value of LAYER2_MEAS_CELLEDGE_PRB_USAGE_REQUIRED = %d \n",
                        msgBuff_p->layer2IcicMeasReportConfig.cellEdgePrbUsageRequired);
            }    
            continue ;
         }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_RNTP_REPORT_BITMASK",31) == 0) && (L2RntpReport !=0))
          {
            strncpy((Char8 *)temp,(const Char8 *)line_p+32,read);
            val = atoi((Char8 *)temp);
            
            RntpThreshold = val & RNTP_THRESHOLD_BITMASK;
            RntpMaxNominalEPRE = val & MAX_NOMINAL_EPRE_BITMASK;
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.bitmask = val;
                printf(" value of LAYER2_MEAS_RNTP_REPORT_BITMASK = %d \n",
                        msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.bitmask);
            }    
            continue ;
           }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_RNTP_REPORT_REQUIRED",32)== 0) && (L2RntpReport !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+33,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.rntpReportRequired = val;
                printf(" value of LAYER2_MEAS_RNTP_REPORT_REQUIRED = %d \n",
                        msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.rntpReportRequired);
            }    
            continue ;
             }
        
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_RNTP_REPORT_THRESHOLD",33) == 0) && (RntpThreshold !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+34,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.rntpThreshold = val;
                printf(" value of LAYER2_MEAS_RNTP_REPORT_THRESHOLD = %d \n",
                         msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.rntpThreshold );
            }    
            continue ;
             }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_RNTP_REPORT_MAX_NOMINAL_EPRE",40) == 0) && (RntpMaxNominalEPRE !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+41,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.maxNominalEPRE = val;
                printf(" value of LAYER2_MEAS_RNTP_REPORT_MAX_NOMINAL_EPRE = %d \n",
                        msgBuff_p->layer2IcicMeasReportConfig.rntpReportingConfigInfo.maxNominalEPRE);
            }    
            continue ;
        }
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"LAYER2_MEAS_PERIODICITY",23) == 0) && (L2Periodicity !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+24,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->layer2IcicMeasReportConfig.periodicity = val;
                printf(" value of LAYER2_MEAS_PERIODICITY = %d \n",
                        msgBuff_p->layer2IcicMeasReportConfig.periodicity);
            }    
            continue ;
            }
       
    
    /* +DYNAMIC ICIC*/
       #if 1
        else if((strncmp((const Char8 *)line_p,"UL_POWER_MASK_CELL_CENTER_USER",30) == 0) && (UL_powerFlag !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+31,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulPowerMask.cellCenterUserPowerMask = val;
                printf(" value of UL_POWER_MASK_CELL_CENTER_USER = %d \n",
                         msgBuff_p->ulPowerMask.cellCenterUserPowerMask);
            }    
            continue ;
            }
     
    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"UL_POWER_MASK_CELL_EDGE_USER",28) == 0) && (UL_powerFlag !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+29,read);
            val = atoi((Char8 *)temp);
            if(val >= 0)
            {    
                msgBuff_p->ulPowerMask.cellEdgeUserPowerMask = val;
                printf(" value of UL_POWER_MASK_CELL_EDGE_USER = %d \n",
                        msgBuff_p->ulPowerMask.cellEdgeUserPowerMask);
            }    
            continue ;
            }

    /* +DYNAMIC ICIC*/
        else if((strncmp((const Char8 *)line_p,"UL_POWER_MASK_QCI_DELTA[9]",26) == 0) && (UL_powerFlag !=0))
            {
            strncpy((Char8 *)temp,(const Char8 *)line_p+27,read);
            str_v=strtok((Char8 *)temp,"[,]");
            UInt32 i=0;
            while(str_v!=NULL) 
            {    
                val = atoi((Char8 *)str_v);
                msgBuff_p->ulPowerMask.qciDeltaPowerMask[i] = val;
                printf(" value of UL_POWER_MASK_QCI_DELTA[%d] = %d \n",
                        i ,  msgBuff_p->ulPowerMask.qciDeltaPowerMask[i]);
                str_v=strtok(NULL,"[,]");
                i=i+1;
                
            }    
            continue ;
            }
    #endif
  }      
        return 0;

}


/*****************************************************************************
 * Function Name  : initRRMSimulator 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Description    : This function init the message queue .    
 *****************************************************************************/


void initRRMSimulator()
{

    struct mq_attr attr;
    UInt8 open_flags = 0;
    UInt32 num_byte_to_send;
    UInt8 priority_of_msg;

    attr.mq_maxmsg = 20;
/* +SPR - 7531*/
    attr.mq_msgsize = 9000;
/* -SPR - 7531*/
    attr.mq_flags = 1;

    //open_flags =O_CREAT|O_RDWR|O_NONBLOCK ;
    open_flags =O_RDWR ;


    num_byte_to_send = 10;
    priority_of_msg = 1;



    printf(" going to open mQ \n");

    fdSendToMac_g = mq_open("/RrmToMac0", open_flags,0666,&attr);
 // fdSendToMac_g = mq_open("/MacToRrm", open_flags);
    printf(" initializing MAC send mQ with fd = %d\n",fdSendToMac_g);  
    
    fdRecvFromMac_g = mq_open("/MacToRrm0", open_flags ,0666,&attr);
    printf(" initializing MAC recv mQ with fd = %d\n",fdRecvFromMac_g);  
    //printf(" initializing receive mQ with fd = %d\n",fdRecvFromMac_g);  
    //fdRecvFromMac_g = mq_open("/MacToRrm0", open_flags ,0666,&attr);

    /* ECN changes start*/
    fdSendToPdcp_g = mq_open("/RrmToPdcp0", open_flags,0666,&attr);
    printf(" initializing PDCP send mQ with fd = %d\n",fdSendToPdcp_g);  
    fdRecvFromPdcp_g = mq_open("/PdcpToRrm0", open_flags ,0666,&attr);
    printf(" initializing PDCP receive mQ with fd = %d\n",fdRecvFromPdcp_g);  
    /* ECN changes end*/
    
    // return 0;
}


/*****************************************************************************
 * Function Name  : sendUsingMsgQ 
 * Inputs         : passsing the buffer for  API and length of message
 * Outputs        : None
 * Returns        : Length of message that is processed
 * Description    : This function send parameters of respective API to MAC.    
 *****************************************************************************/
UInt16  sendUsingMsgQ(UInt8 *buffer ,UInt16 length)
{

    SInt8 status;

    printf("\n value of fdSendToMac_g =%d\n",fdSendToMac_g);
    status =  mq_send(fdSendToMac_g,(const char *)buffer,length,0);

    if(status < 0)
        printf("mq_send failed ... status =%d error=%d \n",status,errno);
    else
        printf("mq_send successfull\n");

        return(length);
}
/* ECN changes start*/
/*****************************************************************************
 * Function Name  : sendUsingPdcpMsgQ 
 * Inputs         : passsing the buffer for  API and length of message
 * Outputs        : None
 * Returns        : Length of message that is processed
 * Description    : This function send parameters of respective API to MAC.    
 *****************************************************************************/
UInt16  sendUsingPdcpMsgQ(UInt8 *buffer ,UInt16 length)
{

    SInt8 status;

    printf("\n value of fdSendToPdcp_g =%d\n",fdSendToPdcp_g);
    status =  mq_send(fdSendToPdcp_g,(const char *)buffer,length,0);

    if(status < 0)
        printf("mq_send failed ... status =%d error=%d \n",status,errno);
    else
        printf("mq_send successfull\n");

        return(length);
}
/*****************************************************************************
 * Function Name  : recvUsingPdcpMessageQ 
 * Inputs         : passsing the FD and buffer 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receive the respone/CNF from MAC .     
 *****************************************************************************/
SInt16 recvUsingPdcpMessageQ(mqd_t fdRecv,UInt8 *buffer)
{
    ssize_t retVal = 0;
    /*Receiving data **********/
//  fflush(stdin);
  //  retVal = mq_receive(fdRecv,buffer,MAX_MSG_SZ,0);
    retVal = mq_receive(fdRecv,(char *)buffer,MAX_MSG_SZ,0);

    if(retVal<0)
        printf(" \n error in receiving msg from MAC ... %lu \t %d\n ", retVal,errno);
    else 
        printf(" \n rececive sucessfully ... \n");

    return retVal;

}
/* ECN changes end*/
/*****************************************************************************
 * Function Name  : recvUsingMessageQ 
 * Inputs         : passsing the FD and buffer 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receive the respone/CNF from MAC .     
 *****************************************************************************/
SInt16 recvUsingMessageQ(mqd_t fdRecv,UInt8 *buffer)
{
    ssize_t retVal = 0;
    /*Receiving data **********/
//  fflush(stdin);
  //  retVal = mq_receive(fdRecv,buffer,MAX_MSG_SZ,0);
    retVal = mq_receive(fdRecv,(char *)buffer,MAX_MSG_SZ,0);

    if(retVal<0)
        printf(" \n error in receiving msg from MAC ... %lu \t %d\n ", retVal,errno);
    else 
        printf(" \n rececive sucessfully ... \n");

    return retVal;

}
/*****************************************************************************
 * Function Name  : recvMsgFromPDCP
 * Inputs         : passsing the buffer structure of RrrmMacUeReconfigReq API 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receiving the parameter of respective API .    
 *****************************************************************************/

SInt16 recvMsgFromPDCP(UInt8 *buffer)
{
    SInt16 retVal = 0;
    //  UInt8 *buffer = 0;
    retVal = recvUsingMessageQ(fdRecvFromPdcp_g,buffer);
    return retVal;
} 
/* ECN changes end*/

/*****************************************************************************
 * Function Name  : recvMsgFromMAC
 * Inputs         : passsing the buffer structure of RrrmMacUeReconfigReq API 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receiving the parameter of respective API .    
 *****************************************************************************/

SInt16 recvMsgFromMAC(UInt8 *buffer)
{
    SInt16 retVal = 0;
    //  UInt8 *buffer = 0;
    retVal = recvUsingMessageQ(fdRecvFromMac_g,buffer);
    return retVal;
} 

/*****************************************************************************
 * Function Name  : processPeriodicReportInd
 * Inputs         : passsing the reportInd structure of RrrmMacPeriodicReportsInd API 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receiving the parameter of respective API .    
 *****************************************************************************/
void processPeriodicReportInd(RrmMacPeriodicReportsInd *reportInd)
{ 
    UInt32 count = 0;
    
    for(;count<reportInd->numOfUE; count++)
    {
        /* SPR 5033 changes start*/
       
        printf("\n ueIndex=%d uplinkSINRValue=%d downlinkSINRValueCW0=%d" \ 
                "downlinkSINRValueCW1=%d uplinkBlerValue=%d" \
                "downlinkBlerValueCW0=%d downlinkBlerValueCW1=%d" \
                "uplinkPathLossValue=%d timingAdvanceValue=%d" \
                "pucchSINRValue=%d pucchBlerValue=%d \n" 
#ifdef TTI_BUNDLING
               " uplinkPHR=%d"
#endif
                ,
                reportInd->reportInfo[count].ueIndex,
                reportInd->reportInfo[count].uplinkSINRValue,
                reportInd->reportInfo[count].downlinkSINRValueCW0,
                reportInd->reportInfo[count].downlinkSINRValueCW1,
                reportInd->reportInfo[count].uplinkBlerValue,
                reportInd->reportInfo[count].downlinkBlerValueCW0,
                reportInd->reportInfo[count].downlinkBlerValueCW1,
                reportInd->reportInfo[count].uplinkPathLossValue,
                reportInd->reportInfo[count].timingAdvanceValue,
                reportInd->reportInfo[count].pucchSINRValue,
                reportInd->reportInfo[count].pucchBlerValue
#ifdef TTI_BUNDLING
                ,reportInd->reportInfo[count].uplinkPHR
#endif
                );
        /* SPR 5033 changes end*/
     fflush(stdout);
    }
}
/* +DYNAMIC ICIC */
/*****************************************************************************
 * Function Name  : processLayer1MeasReportInd
 * Inputs         : passsing the reportInd structure of MacRrmLayer1MeasReportInd API 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receiving the parameter of respective API .    
 *****************************************************************************/
void processLayer1MeasReportInd(MacRrmLayer1MeasReportInd *reportInd)
{ 
        int count=0;   
        printf("\n Bitmask=%d \n", reportInd->bitmask);

        if((reportInd->bitmask & THERMAL_NOISE_BITMASK) !=0)
            printf("THERMAL_NOISE %d\n" , reportInd->thermalNoise);

        if((reportInd->bitmask & RS_TX_POWER_BITMASK) !=0)
            printf("RS_TX_POWER %d\n" , reportInd->dlRsTxPower);

        if((reportInd->bitmask & INTERFERENCE_PER_PRB_BITMASK) !=0)
        {
            printf("interferenceValuePerPRB = ");
            while(count<100)
            {
                printf("%d\t",reportInd->interferencePerPRB[count]);
                count++;
            }    
            printf("\n");
        }     
           fflush(stdout);
    
}
/* +DYNAMIC ICIC */
/*****************************************************************************
 * Function Name  : processLayer2IcicMeasReportInd
 * Inputs         : passsing the reportInd structure of MacRrmLayer2IcicMeasReportInd API 
 * Outputs        : None
 * Returns        : Number of byte received.
 * Description    : This function receiving the parameter of respective API .    
 *****************************************************************************/
void processLayer2IcicMeasReportInd(MacRrmLayer2IcicMeasReportInd *reportInd)
{   
        int count=0;   
        
        printf("\n Bitmask=%d\n  ", reportInd->bitmask);
        if((reportInd->bitmask & DL_CE_PRB_USAGE_BITMASK) !=0)
            printf("DL_CE_PRB_USAGE %d\n" , reportInd->dlCEPrbUsage);
     
        if((reportInd->bitmask & UL_CE_PRB_USAGE_BITMASK) !=0)
            printf("UL_CE_PRB_USAGE %d\n" , reportInd->ulCEPrbUsage);

       
        if((reportInd->bitmask & RNTP_VAL_BITMASK) !=0)
        {
            printf("rntpValPerPRB = ");
            while(count<100)
            {
                printf("%d," ,reportInd->rntpValuePerPRB[count]);
                count++;
            }    
            printf("\n");
     
        }     
     fflush(stdout);
    
}

/*****************************************************************************
* Function Name  : processMacCellPerfStatsInd
* Inputs         : None
* Outputs        : None
* Returns        : void
* Description    : This function handles processMacCellPerfStatsInd
*****************************************************************************/

void processMacCellPerfStatsInd(MacCellPerfStatsIndParams* pCellPerfStats)
{
    UInt32 qci = 0;
    printf("\n");

    printf("duration received[%d]\n",pCellPerfStats->cellMacPerfStatsResp.durationTTI);
    if ( cellbitmap_g & 1 )
    {
        printf("PERF_STATS_PRB_USAGE_HARQ\n");
        for ( qci = 0;qci < MAX_QCI;qci++ )
        {
            printf("totalDLPRBUsageforHarq[%d] for Qci[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellHarqPerfStats.totalDLPRBUsageforHarq[qci],(qci+1));
        }
        printf("totalDLPRBUsageforHarqRetx[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellHarqPerfStats.totalDLPRBUsageforHarqRetx);

    } 

    if ( cellbitmap_g & 2 )
    {
        printf("PERF_STATS_RACH_SUCC_FAIL\n");
        printf("totalRcvdMsg3RandomAccessPreamblesCBRA[%d]\n",pCellPerfStats->cellMacPerfStatsResp.rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCBRA);
    }

    if ( cellbitmap_g & 4 )
    {
        printf("PERF_STATS_PDCCH_UTILIZATION\n");
    }

    if ( cellbitmap_g & 8 )
    {
        printf("PERF_STATS_PUCCH_UTILIZATION\n");
    }

    if ( cellbitmap_g & 16 )
    {
        printf("PERF_STATS_CELL_PRB_USAGE\n");
        printf("totalULPRBUsageSRB0[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalULPRBUsageSRB[0]);
        printf("totalDLPRBUsageSRB0[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalDLPRBUsageSRB[0]);
        printf("totalULPRBUsageSRB1[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalULPRBUsageSRB[1]);
        printf("totalDLPRBUsageSRB1[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalDLPRBUsageSRB[1]);
        printf("totalULPRBUsageSRB2[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalULPRBUsageSRB[2]);
        printf("totalDLPRBUsageSRB2[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalDLPRBUsageSRB[2]);
        printf("totalDLPRBUsagePCCH[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalDLPRBUsagePCCH);
        printf("totalDLPRBUsageBCCH[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalDLPRBUsageBCCH);
        printf("totalDLPRBAvailable[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalDLPRBAvailable);
        printf("totalULPRBAvailable[%d]\n",pCellPerfStats->cellMacPerfStatsResp.cellPRBUsagePerfStats.totalULPRBAvailable);
    }
}


/*****************************************************************************
 * Function Name  : processMACMsg 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This is main function of processing MAC messages.  
 *****************************************************************************/
 
void processMACMsg ()
{

    UInt8  buffer[MAX_MSG_SIZE] = {0};
    //UInt16 msgId = 0;
    SInt32 bytesRead     = 0;
    //UInt16 srcModuleId   = 0;
    //UInt16 destModuleId   = 0;
    //UInt16 transactionId   = 0;
    //UInt16 msgLen          = 0;
    /* +- SPR 18268 */
    UInt16 i = 0;
    /* +- SPR 18268 */
    L2RrmMsg *l2RrmMsgCnf_p = PNULL;
    RrmMacReconfigSchedulerCnf *rrmMacReconfigSchedulerCnf_p = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReConfigCnf_p               = PNULL;
    RrmMacCellConfigCnf *rrmMacCellConfigCnf_p               = PNULL;

    /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
    RrmMacConfigPerfStatsCnf *rrmMacConfigPerfStatsCnf_p     = PNULL;
#endif
    /* Perf stats RRM-MAC end */


    bytesRead = recvMsgFromMAC(buffer);

    printf("\n  msg from MAC...bytes [%d]\n",bytesRead);

    if(MAC_RECV_ERROR == bytesRead)
    {
        printf("\n error in receiving msg from msgQ ,MAC_RECV_ERROR =%d \n",MAC_RECV_ERROR);

    }

    if(bytesRead < MAC_API_HEADER_LEN)
    {
        printf("\n error due to bytesRead < MAC_API_HEADER_LEN \n");
    }

   l2RrmMsgCnf_p = (L2RrmMsg *)buffer;

    /*Parse the buffer header***************/
   
    if((MAC_MODULE_ID == l2RrmMsgCnf_p->header.srcModuleId) &&
        (RRM_MODULE_ID == l2RrmMsgCnf_p->header.destModuleId))
    {
        switch(l2RrmMsgCnf_p->header.apiId)
        {

            case RRM_MAC_CELL_CONFIG_CNF:
                {
                    
                    printf(" \n ********************************************* \n");
                    printf("\n RRM_MAC_CELL_CONFIG_CNF received\n");
                    rrmMacCellConfigCnf_p = (RrmMacCellConfigCnf *)(l2RrmMsgCnf_p->data);
                    switch(rrmMacCellConfigCnf_p->responseCode)
                    {
                    case 1 : {          
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "SUCCESS");
                             break;
                             }

                    case 3 : {          
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "PARAMETER_OUT_OF_RANGE");
                             break ;
                             }
                    case 4 : {
                                                    
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "RETRY_AFTER_SOME_TIME");
                             break ;
                             }
                    case 5 : {

                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_NUMBER_OF_CC_REGION");
                             break ;
                      }
                    case 6 : {
                   
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_NUMBER_OF_CE_REGION");
                             break ;
                             }
                    case 7 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_TOTAL_RB_COUNT");
                             break ;
                              }
                    case 8 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "INVALID_CELL_CONFIG_BITMASK");
                             break ;
                             }
                    case 9 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "INVALID_REPORT_REQUIRED_VALUE");
                             break ;
                              }
                    case 10 : { 
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "INVALID_PERIODICITY");
                             break ;
                         }
                    case 11 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "INVALID_UE_BATCH_SIZE");
                             break ;
                         } 
                    case 12 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "BATCH_SIZE_RECONFIG_NOT_ALLOWED");
                             break ;
                              }
                    case 13 :{
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_PERIODIC_REPORT_CONFIG_BITMASK");
                             break ;
                             }
                    case 14 :{
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "PERIODICITY_VALUE_NOT_RECEIVED");
                             break ;
                             }
                    case 15 :{
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "UE_BATCH_SIZE_VALUE_NOT_RECEIVED");
                             break ;
                             }
                    case 16 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "INVALID_CELL_CONFIG_API_LENGTH");
                             break ;
                              }
                    case 17 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "START_RB_NOT_IN_INCREASING_ORDER");
                             break ;
                        
                              }
                    case 18 :{
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "CE_USER_EXIST_CE_REGION_IS_ZERO");
                             break ;
                        
                             }
                    case 19 :{
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "CC_REGION_OVERLAPPING");
                             break ;
                             }
    
                    case 20 :{
                    
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_L1_MEAS_REPORT_CONFIG_BITMASK");
                             break ;
                    
                             }
                    case 21 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_L2_ICIC_MEAS_REPORT_CONFIG_BITMASK");
                             break ;
                    
                              }
                    case 22 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                           "INVALID_RNTP_REPORT_CONFIG_BITMASK" );
                             break ;
                             }
                    case 23 : {
                        
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                            "INVALID_RNTP_THRESHOLD_VALUE"); 
                             break ;
                             }
                    case 24 :{
                       
                    printf("\n respone for cellconfig receive from MAC = %s \n",
                              "INVALID_EPRE_BITMASK "); 
                    //printf("\n respone for cellconfig receive from MAC = %d \n",
                      //        rrmMacCellConfigCnf_p->responseCode);
                             break ;
                            }
                    }
                    
                    printf(" \n ********************************************* \n");
                    break;
                }
            case RRM_MAC_SCHEDULER_RECONFIG_CNF:
                {

                    printf(" \n ********************************************* \n");
                    printf("\n RRM_MAC_SCHEDULER_RECONFIG_CNF received\n");
                    rrmMacReconfigSchedulerCnf_p = (RrmMacReconfigSchedulerCnf *)(l2RrmMsgCnf_p->data);
                    printf("\n msg element receive from MAC = %d \n"
                            ,rrmMacReconfigSchedulerCnf_p->responseCode);
                    printf(" \n ********************************************* \n");
                    break;
                } 
            case RRM_MAC_UE_RECONFIG_CNF:
                {


                    printf(" \n ********************************************* \n");
                    printf("\n RRM_MAC_UE_RECONFIG_CNF received\n");
                    rrmMacUEReConfigCnf_p = (RrmMacUEReConfigCnf *)(l2RrmMsgCnf_p->data);
                    printf("\n responseCode for UeReConfig receive from MAC = %d \n"
                            ,rrmMacUEReConfigCnf_p->responseCode);

                    for(i =0;i<rrmMacUEReConfigCnf_p->ueLocationErrorInfo.numOfUE;i++)
                    {
                        printf("Error Code for UE Index[%d] is %d",
                                rrmMacUEReConfigCnf_p->ueLocationErrorInfo.ueLocationError[i].ueIndex,
                                rrmMacUEReConfigCnf_p->ueLocationErrorInfo.ueLocationError[i].errorCode);
                    }
                    /*UL_MU_MIMO_CHG Start*/
                    for(i =0;i<rrmMacUEReConfigCnf_p->ueULMUMIMOErrorInfo.numOfUE;i++)
                    {
                        printf("MUMIMO Error Code for UE Index[%d] is %d",
                                rrmMacUEReConfigCnf_p->ueULMUMIMOErrorInfo.ueULMUMIMOError[i].ueIndex,
                                rrmMacUEReConfigCnf_p->ueULMUMIMOErrorInfo.ueULMUMIMOError[i].errorCode);
                    }
                    /*UL_MU_MIMO_CHG End*/


                    printf(" \n ********************************************* \n");
                    break;
                }

           /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
           case RRM_MAC_CONFIGURE_PERF_STATS_CNF:
               {
                   printf("\n***********************\n");
                   printf("\n RRM_MAC_CONFIGURE_PERF_STATS_CNF received\n");
                   rrmMacConfigPerfStatsCnf_p = (RrmMacConfigPerfStatsCnf*)(l2RrmMsgCnf_p->data);

                   printf("\n responseCode for RRM_MAC_CONFIGURE_PERF_STATS_CNF receive from MAC = %d \n",
                            rrmMacConfigPerfStatsCnf_p->responseCode);

                   break;
               }
#endif
           /* Perf stats RRM-MAC end */



            case RRM_MAC_PERIODIC_REPORT_IND:
                {
                    printf(" \n ********************************************* \n");
                    printf("\n RRM_MAC_PERIODIC_REPORT_IND received\n");
                    processPeriodicReportInd((RrmMacPeriodicReportsInd *)l2RrmMsgCnf_p->data);
                    break;
                }
            
            case RRM_MAC_LAYER1_MEAS_REPORT_IND:
                {
                    printf(" \n ********************************************* \n");
                    printf("\n RRM_MAC_LAYER1_REPORT_IND received\n");
                    processLayer1MeasReportInd((MacRrmLayer1MeasReportInd *)l2RrmMsgCnf_p->data);
                    break;
                }
                
            case RRM_MAC_LAYER2_ICIC_MEAS_REPORT_IND:
                {
                    printf(" \n ********************************************* \n");
                    printf("\n RRM_MAC_LAYER2_ICIC_REPORT_IND received\n");
                    processLayer2IcicMeasReportInd((MacRrmLayer2IcicMeasReportInd *)l2RrmMsgCnf_p->data);
                    break;
                }

            /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
            case RRM_MAC_CELL_PERF_STATS_IND:
                {
                    char *timestamp = (char *)malloc(sizeof(char) * 16);
                    time_t ltime;
                    ltime=time(NULL);
                    struct tm *tm;
                    tm=localtime(&ltime);

                    printf(" Received RRM_MAC_CELL_PERF_STATS_IND at %04d/%02d/%02d/ %02d-%02d-%02d", tm->tm_year+1900, tm->tm_mon+1, 
                                                    tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

                    processMacCellPerfStatsInd((  MacCellPerfStatsIndParams*)l2RrmMsgCnf_p->data);

                    free(timestamp);
                    break;
                }
#endif
            /* Perf stats RRM-MAC end */
            default:
                {
                    printf("\n  wrong API are coming from MAC \n");
                }

        }

    }

}

/* Ecn changes start*/
/*****************************************************************************
 * Function Name  : processPDCPMsg 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This is main function of processing Pdcp messages.  
 *****************************************************************************/
 
void processPDCPMsg ()
{

    UInt8  buffer[MAX_MSG_SIZE] = {0};
    //UInt16 msgId = 0;
    SInt32 bytesRead     = 0;
    //UInt16 srcModuleId   = 0;
    //UInt16 destModuleId   = 0;
    //UInt16 transactionId   = 0;
    //UInt16 msgLen          = 0;
    L2RrmMsg *msg_p = PNULL;
    //UInt8 i = 0;
    
  //  RrmMacCnf *rrmMacCnf_p = PNULL;
    RrmPdcpEcnConfigParamCnf *rrmPdcpEcnConfigParamCnf_p = PNULL;
    RrmPdcpDrbEcnCeCnf *rrmPdcpDrbEcnCeCnf_p             = PNULL;
    RrmPdcpDrbEcnCeInd *rrmPdcpDrbEcnCeInd_p             = PNULL;

    printf("\n  msg from PDCP... \n");
    bytesRead = recvMsgFromPDCP(buffer);

    //  bytesRead =  mq_receive(fdRecvFromMac_g,(char *)buffer,length,0); 
    msg_p = (L2RrmMsg *) buffer;

    if(PDCP_REC_ERROR == bytesRead)
    {
        printf("\n error in receiving msg from msgQ ,PDCP_REC_ERROR =%d \n",PDCP_REC_ERROR);

    }

    if(bytesRead < L2_RRM_API_HEADER_LEN )
    {
        printf("\n error due to bytesRead < L2_RRM_API_HEADER_LEN \n");
    }

    /*Parse the buffer header***************/

    printf("\n src %d dest %d api %d len %d\n",msg_p->header.srcModuleId, msg_p->header.destModuleId , msg_p->header.apiId ,msg_p->header.length);

    if((PDCP_MODULE_ID == msg_p->header.srcModuleId) && (RRM_MODULE_ID == msg_p->header.destModuleId))
    {
        switch(msg_p->header.apiId)
        {

            case RRM_PDCP_CONFIG_PARAMS_CNF:
                {
                    
                    printf(" \n ********************************************* \n");
                    printf("\n RRM_PDCP_CONFIG_PARAMS_CNF received\n");
                    rrmPdcpEcnConfigParamCnf_p = (RrmPdcpEcnConfigParamCnf *)(msg_p->data);
                    printf("\n respone for from PDCP = %d \n",
                            rrmPdcpEcnConfigParamCnf_p->respCode);
                    printf(" \n ********************************************* \n");
                    break;
                }
            
            case RRM_PDCP_UE_DRB_ECN_CE_CNF:
                {

                    printf(" \n ********************************************* \n");
                    printf("\n RRM_PDCP_UE_DRB_ECN_CE_CNF received\n");
                    rrmPdcpDrbEcnCeCnf_p = (RrmPdcpDrbEcnCeCnf *)(msg_p->data);
                    printf("\n Resp Code %d ueIndex %d numOfLc %d\n",rrmPdcpDrbEcnCeCnf_p->respCode, 
                                rrmPdcpDrbEcnCeCnf_p->ueIndex, rrmPdcpDrbEcnCeCnf_p->numOfLc );
                    printf("\n result from PDCP = %d  direction %d lcId %d\n"
                            ,rrmPdcpDrbEcnCeCnf_p->ceLc[0].result, rrmPdcpDrbEcnCeCnf_p->ceLc[0].direction, rrmPdcpDrbEcnCeCnf_p->ceLc[0].lcId);
                    printf(" \n ********************************************* \n");
                    break;
                } 
            
            case RRM_PDCP_UE_DRB_ECN_CE_IND:
                {

                    printf(" \n ********************************************* \n");
                    printf("\n RRM_PDCP_UE_DRB_ECN_CE_IND received\n");
                    rrmPdcpDrbEcnCeInd_p = (RrmPdcpDrbEcnCeInd *)(msg_p->data);
                    printf("\n ueIndex [%d] lcid [%d] direction [%d] congestionStatus [%d]\n",rrmPdcpDrbEcnCeInd_p->ueIndex, 
                                rrmPdcpDrbEcnCeInd_p->lcId, rrmPdcpDrbEcnCeInd_p->direction, rrmPdcpDrbEcnCeInd_p->congestionStatus );
                    printf(" \n ********************************************* \n");
                    break;
                } 
            default:
                {
                    printf("\n  wrong API are coming from MAC \n");
                }

        }

    }

}


/*****************************************************************************
 * Function Name  : sendRrmMacUeReconfigReq 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function preparing header,parsing the parameter 
 *                  of RrmMacUeReconfigReq and send the parameter of UeReconfig
 *                  API to MAC Inteface .
 * *****************************************************************************/
void sendRrmMacUeReconfigReq()
{
    SInt16 retvalue = 0;
    UInt8  *msg_p = PNULL;
    UInt16 msg_length = 0;
    UInt8 buffer[6144]={0};



    RrmMacUeReconfigReq *p_rrmMacUeReconfigReq;
    msg_p = buffer;


    p_rrmMacUeReconfigReq = (RrmMacUeReconfigReq *)(msg_p + RRM_API_HEADER_SIZE);

//        printf(" \n size of msg_length = %d \n",msg_length);

    parseForRrrmMacUeReconfigReq(p_rrmMacUeReconfigReq);


    msg_length = sizeof(RrmMacUeReconfigReq) + RRM_API_HEADER_SIZE + sizeof(UELocation)*
                    (p_rrmMacUeReconfigReq->ueLocationInfo.numOfUE - 1) ;
    prepareMsgHeader(msg_p,msg_length, RRM_MAC_UE_RECONFIG_REQ );
    
    retvalue = sendUsingMsgQ(msg_p, msg_length);

    printf("========================================\n");
    printf("RRM Send >>: Init Layer Request\n");
    printf("%d bytes of data sent\n ",retvalue);
}

/* ECN changes start*/
/*****************************************************************************
 * Function Name  : sendRrmPdcpEcnConfigParamReq 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function preparing header,parsing the parameter 
 *                  of RrmPdcpEcnConfigParamReq and send the parameter of
 *                  API to PDCP Inteface .
 * *****************************************************************************/
void sendRrmPdcpEcnConfigParamReq()
{
    SInt16 retvalue = 0;
    UInt8  *msg_p = PNULL;
    UInt16 msg_length = 0;
    UInt8 buffer[16]={0};
    
    RrmPdcpEcnConfigParamReq *p_rrmPdcpEcnConfigParamReq;
    msg_p = buffer;

    //preparePdcpMsgHeader(msg_p, RRM_PDCP_CONFIG_PARAMS_REQ );
    *((UInt16 *)msg_p) = 5;
    msg_p += 2;
    *((UInt16 *)msg_p) = 2;
    msg_p += 2;
    *((UInt16 *)msg_p) = 5;
    msg_p += 2;
    *((UInt16 *)msg_p) = 2;
    msg_p += 2;
    *((UInt16 *)msg_p) = 16;

    p_rrmPdcpEcnConfigParamReq = (RrmPdcpEcnConfigParamReq *)(buffer + L2_RRM_API_HEADER_LEN);

//        printf(" \n size of msg_length = %d \n",msg_length);

    parseForRrmPdcpEcnConfigReq(p_rrmPdcpEcnConfigParamReq);
    //parseForRrrmMacUeReconfigReq(p_rrmMacUeReconfigReq);
    msg_length = sizeof(RrmPdcpEcnConfigParamReq) + L2_RRM_API_HEADER_LEN ;

    retvalue = sendUsingPdcpMsgQ(buffer, msg_length);

    printf("========================================\n");
    printf("RRM Send >>: RRM_PDCP_CONFIG_PARAMS_REQ \n");
    printf("%d bytes of data sent\n ",retvalue);
}

/*****************************************************************************
 * Function Name  : sendRrmPdcpEcnDrbUeCnReq 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function preparing header,parsing the parameter 
 *                  of RrmPdcpEcnConfigParamReq and send the parameter of
 *                  API to PDCP Inteface .
 * *****************************************************************************/
void sendRrmPdcpEcnDrbUeCnReq()
{
    SInt16 retvalue = 0;
    UInt8  *msg_p = PNULL;
    UInt16 msg_length = 0;
    UInt8 buffer[512]={0};
    
    RrmPdcpDrbEcnCeReq *p_rrmPdcpDrbEcnCeReq;
    msg_p = buffer;

    //preparePdcpMsgHeader(msg_p, RRM_PDCP_UE_DRB_ECN_CE_REQ );
    *((UInt16 *)msg_p) = 5;
    msg_p += 2;
    *((UInt16 *)msg_p) = 2;
    msg_p += 2;
    *((UInt16 *)msg_p) = 5;
    msg_p += 2;
    *((UInt16 *)msg_p) = 1;
    msg_p += 2;
    *((UInt16 *)msg_p) = 48;

    p_rrmPdcpDrbEcnCeReq = (RrmPdcpDrbEcnCeReq *)(buffer + L2_RRM_API_HEADER_LEN );

//        printf(" \n size of msg_length = %d \n",msg_length);

    parseForRrmPdcpDrbEcnCeReq(p_rrmPdcpDrbEcnCeReq);
    //parseForRrrmMacUeReconfigReq(p_rrmMacUeReconfigReq);
    msg_length = sizeof(RrmPdcpDrbEcnCeReq) + L2_RRM_API_HEADER_LEN ;

    retvalue = sendUsingPdcpMsgQ(buffer, msg_length);

    printf("========================================\n");
    printf("RRM Send >>: RRM_PDCP_UE_DRB_ECN_CE_REQ \n");
    printf("%d bytes of data sent\n ",retvalue);
}
/* ECN changes end*/
/*****************************************************************************
 * Function Name  : sendRrmMacReconfigSchedulerReq 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function preparing header,parsing the parameter 
 *                  of RrmMacReconfigSchedulerReq and send the parameter of UeReconfig
 *                  API to MAC Inteface .
 * *****************************************************************************/
void sendRrmMacReconfigSchedulerReq()
{
    SInt16 retvalue = 0;
    UInt8  *msg_p = NULL;
    UInt16 msg_length = 0;
    UInt8 buffer[5120];
    RrmMacReconfigSchedulerReq *p_rrmMacReconfigSchedulerReq;
    msg_p = buffer;

    msg_length = sizeof(RrmMacReconfigSchedulerReq) + RRM_API_HEADER_SIZE;
    prepareMsgHeader(msg_p, msg_length,RRM_MAC_SCHEDULER_RECONFIG_REQ );

    p_rrmMacReconfigSchedulerReq = (RrmMacReconfigSchedulerReq *)(msg_p + RRM_API_HEADER_SIZE);

    printf(" \n size of msg_length = %d \n",msg_length);

    parseForRrmMacReconfigSchedulerReq(p_rrmMacReconfigSchedulerReq);


    retvalue = sendUsingMsgQ(msg_p, msg_length);

    printf("========================================\n");
    printf("MAC Send >>: Init Layer Request\n");
    printf("%d bytes of data sent\n ",retvalue);
}

/*****************************************************************************
 * Function Name  : sendRrmMacCellConfigReq 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function preparing header,parsing the parameter 
 *                  of RrmMacCellConfigReq and send the parameter of UeReconfig
 *                  API to MAC Inteface .
 * *****************************************************************************/
void sendRrmMacCellConfigReq()
{
    SInt16 retvalue = 0;
    UInt8  *msg_p = NULL;
    UInt16 msg_length = 0;
    UInt8 buffer[1024];
    RrmMacCellConfigReq *p_rrm_cell_config_req;

    msg_p = buffer;
    msg_length = sizeof(RrmMacCellConfigReq) + RRM_API_HEADER_SIZE;

    prepareMsgHeader(msg_p,msg_length, RRM_MAC_CELL_CONFIG_REQ );

    p_rrm_cell_config_req = (RrmMacCellConfigReq *)(msg_p + RRM_API_HEADER_SIZE);

    printf(" \n size of msg_length = %d \n",msg_length);

    parseForCellConfigReq(p_rrm_cell_config_req);


    retvalue = sendUsingMsgQ(msg_p, msg_length);

    printf("========================================\n");
    printf("MAC Send >>: Init Layer Request\n");
    printf("%d bytes of data sent\n ",retvalue);
    printf("========================================\n");

}

/* Perf stats RRM-MAC start */
#ifdef PERF_STATS
/*****************************************************************************
 * Function Name  : sendRrmMacConfigPerfStatsReq
 * Inputs         : None
 * Outputs        : None
 * Returns        : void
 * Description    : This function process RRM_MAC_CONFIGURE_PERF_STATS_REQ
 * *****************************************************************************/

void sendRrmMacConfigPerfStatsReq()
{
    SInt16 retvalue = 0;
    UInt8  *msg_p = NULL;
    UInt16 msg_length = 0;
    UInt8 buffer[1024];
    RrmMacConfigPerfStatsReq *p_rrm_mac_config_perf_stats_req = PNULL;

    msg_p = buffer;
    msg_length = sizeof(RrmMacConfigPerfStatsReq) + RRM_API_HEADER_SIZE;

    prepareMsgHeader(msg_p,msg_length, RRM_MAC_CONFIGURE_PERF_STATS_REQ );

    p_rrm_mac_config_perf_stats_req = (RrmMacConfigPerfStatsReq *)(msg_p + RRM_API_HEADER_SIZE);

    printf(" \n size of msg_length = %d \n",msg_length);

    p_rrm_mac_config_perf_stats_req->duration = duration_g;
    p_rrm_mac_config_perf_stats_req->periodicReporting = periodic_g;
    p_rrm_mac_config_perf_stats_req->uePerfStatsToReportBitMap = uebitmap_g;
    p_rrm_mac_config_perf_stats_req->cellPerfStatsToReportBitMap = cellbitmap_g;

    retvalue = sendUsingMsgQ(msg_p, msg_length);

    printf("========================================\n");
    printf("RRM_MAC_CONFIGURE_PERF_STATS_REQ Send\n");
    printf("%d bytes of data sent\n ",retvalue);
    printf("========================================\n");

}
#endif
/* Perf stats RRM-MAC end */

/*****************************************************************************
 * Function Name  : rxFromSTDIN 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function take input from user ,for processing of API .
 * *****************************************************************************/
void rxFromSTDIN(UInt8 choice)
{

 //   UInt8 choice;
    fflush(stdin);
    fflush(STDIN_FILENO);

 //   printf(" enter your choice \n");
 //   scanf("%d",&choice);
    if(0 == choice)
    {
        printf ("\nNot a valid choice: \n");
        return ;
    } 
    if(choice > 0) 
    {
        switch (choice)
        {
            case 1:
                printf("sending rrmMacCellConfigReq \n ");
                sendRrmMacCellConfigReq();
                printf ("\nRRMSIM Enter Choice[Press 8 to display menu]:");
                break;

            case 2:
                printf("sending rrmMacReconfigSchedulerReq \n ");
                sendRrmMacReconfigSchedulerReq();
                printf ("\n RRMSIM Enter Choice[Press 8 to display menu]:");
                break;

            case 3:
                printf("sending rrmiMacReconfigSchedulerReq \n ");
                sendRrmMacUeReconfigReq();
                printf ("\n RRMSIM Enter Choice[Press 8 to display menu]:");
                break;
            case 4:
                printf("\n sending all API Simultaneously \n");
                printf("sending rrmMacCellConfigReq \n ");
                sendRrmMacCellConfigReq();
                printf("sending rrmMacReconfigSchedulerReq \n ");
                sendRrmMacReconfigSchedulerReq();
                printf("sending rrmiMacReconfigSchedulerReq \n ");
                sendRrmMacUeReconfigReq();
            /* ECN changes start*/   
                printf("sending sendRrmPdcpEcnConfigParamReq \n ");
                sendRrmPdcpEcnConfigParamReq();
                printf("sending sendRrmPdcpEcnDrbUeCnReq \n ");
                sendRrmPdcpEcnDrbUeCnReq();
            /* ECN changes end*/    

                /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
                printf("Sending RRM_MAC_CONFIGURE_PERF_STATS_REQ\n");
                sendRrmMacConfigPerfStatsReq();
                printf ("\n RRMSIM Enter Choice[Press 8 to display menu]:");
#endif
                /* Perf stats RRM-MAC end */

                break;
            /* ECN changes start*/    
            case 5:
                printf("sending sendRrmPdcpEcnConfigParamReq \n ");
                sendRrmPdcpEcnConfigParamReq();
                printf ("\n RRMSIM Enter Choice[Press 8 to display menu]:");
                break;

            case 6:
                printf("sending sendRrmPdcpEcnDrbUeCnReq \n ");
                sendRrmPdcpEcnDrbUeCnReq();
                printf ("\n RRMSIM Enter Choice[Press 8 to display menu]:");
                break;
            /* ECN changes end*/  
            /* Perf stats RRM-MAC start */ 
            case 7:
#ifdef PERF_STATS
                printf(" enter your duration \n");
                scanf("%d",&duration_g);
                printf(" enter your periodic \n");
                scanf("%d",&periodic_g);
                printf(" enter your cellbitmap \n");
                scanf("%d",&cellbitmap_g);
                printf(" enter your uebitmap \n");
                scanf("%d",&uebitmap_g);


               printf("Sending RRM_MAC_CONFIGURE_PERF_STATS_REQ\n");
               sendRrmMacConfigPerfStatsReq();
#endif
               printf ("\n RRMSIM Enter Choice[Press 8 to display menu]:");
               break;
            case 8:
                displayMenu();
                break;
            /* Perf stats RRM-MAC end */
            default:
                printf("Wrong Option selected\n");
                break;
        }      
        return ;
    }
    choice = 0;
}

/*****************************************************************************
 * Function Name  : userInputMenu 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function taking the user input for processing API 
 *                  and processiong the MAC respone message.
 * ***************************************************************************
 */
void userInputMenu()
{
    fd_set readFD;
    UInt32 choice;
    /* ECN changes start*/
    UInt8 highestFD = 0;
    /* ECN changes end*/
        printf("\n user Input Menu"); 

    while(1)
    {

        FD_ZERO(&readFD); 
        FD_SET(STDIN_FILENO,&readFD);
        FD_SET(fdRecvFromMac_g,&readFD); if (fdRecvFromMac_g > highestFD) highestFD = fdRecvFromMac_g;
        /* ECN changes start*/
        FD_SET(fdRecvFromPdcp_g,&readFD);if (fdRecvFromPdcp_g > highestFD) highestFD = fdRecvFromPdcp_g;
        /* ECN changes end*/

/*        if (select(fdRecvFromMac_g + 1 ,&readFD,NULL,NULL,NULL) == -1)
        {
            printf("\n entering in select fun \n");
            continue;
        }
        */    
        if ( 0 != select(highestFD + 1 ,&readFD,NULL,NULL,NULL))  
        {
            printf("\n entering in select fun \n");
            if (FD_ISSET(STDIN_FILENO,&readFD))
            {
                printf("\n reading from file in rxFromSTDIN \n");
                printf(" enter your choice \n");
                scanf("%d",&choice);
                rxFromSTDIN(choice);
                FD_CLR(STDIN_FILENO,&readFD); 
            }
            if (FD_ISSET(fdRecvFromMac_g,&readFD))
            {   
                printf(" \n code coming in FD_ISSET \n");
                processMACMsg() ;
                FD_CLR(fdRecvFromMac_g,&readFD);       
            }
            if (FD_ISSET(fdRecvFromPdcp_g,&readFD))
            {   
                printf(" \n code coming in FD_ISSET \n");
                processPDCPMsg() ;
                FD_CLR(fdRecvFromPdcp_g,&readFD);       
            }
        }   

    }
}


/******************************************************************************
 * Function Name  : main 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : void
 * Description    : This function init RRM Simulator and taking the user input
 *                  for processing API and processiong the MAC respone message.
 * *****************************************************************************/

int main (int argc, char *argv[])
{   
 printf(" \n start RRM SIM testing \n");
    initRRMSimulator();
    displayMenu();
    userInputMenu();
    return 0;
}

