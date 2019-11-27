/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacSRSMgr.c,v $
 *
 ******************************************************************************
 *
 *  File Description : This file contains all the handler functions that are
 *  required by the SRS manager to handle SRS transmissions
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacSRSMgr.c,v $
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
 
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacSRSMgr.h"
#include "lteMacCellConfMgr.h"
#include "lteMacPUSCH.h"

/******************************************************************************
 Private Definitions
 *****************************************************************************/
 
/******************************************************************************
 Private Types
 *****************************************************************************/
 
/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
STATIC  MacRetType checkValidTransOffset(UInt8 transOffset, UInt8 configIndex);
/*SPR 16855 fix start*/
/* +- SPR 17777 */
STATIC  MacRetType insertUESrsNodeInUESubframeMap(UInt16 ueIndex,
                             SRSReportNode **nodeEntry_pp, LTE_LIST *subFrameList_p);
/* +- SPR 17777 */
/*SPR 16855 fix end*/
STATIC  MacRetType getSrsPeriodicityAndOffset(UInt16 srsConfigIndex, UInt16
        *srsPeriodicity_p, UInt16 *srsSubFrameOffset_p);
/*SPR 16855 fix start*/
STATIC  MacRetType updateSrsUESubframeMapForNot2ms(UInt16 ueIndex,SRSConfigInfo *tempsrsConfigInfo, 
                        UInt16  srsPeriodicity, UInt16 srsSubFrameOffset,
                        InternalCellIndex internalCellIndex);
/*SPR 16855 fix end*/
/*SPR 7086 Fix Start*/
STATIC  void determineAndUpdateSrsPosition(UInt8 transmissionComb, InternalCellIndex internalCellIndex);
/*SPR 7086 Fix End*/
#ifdef TDD_CONFIG
STATIC  void processSRSReportNodesForUSubframes(SRSUESubFrameMapList *srsUESubframeMap_p,
                                UInt16 subFrameOffset, UInt8 currSubFrameNum, 
                                ULSchUciRachPoolInfo* ulschUciRachInfo_p,
                                /* SPR 15909 fix start */
                                tickType_t ulTTI, /* MEAS_GAP_CHG */
                                /* SPR 15909 fix end */
                                /*CA Changes start  */
                                InternalCellIndex internalCellIndex);                                        
                                /*CA Changes end  */
                                                              

STATIC MacRetType selectUplinkSRSBandwidthTable(InternalCellIndex internalCellIndex);
STATIC  void updateUESrsMapOffFor2msUsingKsrs(UInt8 srsUESubFrOffFor2ms, 
                UInt8 *ueSrsMapOffEntryFor2ms_p, UInt8 *numUESrsMapOffFor2ms_p);
/*SPR 16855 fix start*/
STATIC   MacRetType updateSrsUESubframeMapFor2ms(UInt16 ueIndex,SRSConfigInfo *srsConfigInfo, 
                    UInt8 *ueSrsMapOffEntryFor2ms_p, UInt8 numUESrsMapOffFor2ms, InternalCellIndex internalCellIndex);
/*SPR 16855 fix end*/
STATIC  void processSRSReportNodesForSSubframes(SRSUESubFrameMapList *srsUESubframeMap_p,
                                UInt16 subFrameOffset, 
                                ULSchUciRachPoolInfo* ulschUciRachInfo_p, 
                                /* SPR 15909 fix start */
                                tickType_t ulTTI,
                                /* SPR 15909 fix end */
                                InternalCellIndex internalCellIndex);
                                /* MEAS_GAP_CHG */
#elif FDD_CONFIG

STATIC  void processSRSReportNodesForUSubframes(SRSUESubFrameMapList *srsUESubframeMap_p,
                                UInt16 subFrameOffset, ULSchUciRachPoolInfo* ulschUciRachInfo_p,
                                PdcchULOutputInfo *pdcchULOutputInfo_p,
                                /* SPR 15909 fix start */
                                tickType_t ulTTI, /* MEAS_GAP_CHG */
                                /* SPR 15909 fix end */
                                /*CA Changes start  */
                                InternalCellIndex internalCellIndex);                                        
                                /*CA Changes end  */
/*SPR 16855 fix start*/
STATIC  MacRetType updateSrsUEShortSubframeMap(UInt16 ueIndex,SRSConfigInfo *tempsrsConfigInfo, 
                        UInt16  srsPeriodicity, UInt16 srsSubFrameOffset,
                        InternalCellIndex internalCellIndex);
STATIC  MacRetType processSRSShortSubframeMap(UInt16 srsUEConfigIndex,
                                UInt16 duration,UInt16 ueIndex,SRSConfigInfo *tempsrsConfigInfo,
                                InternalCellIndex internalCellIndex);
/*SPR 16855 fix end*/
#endif
/******************************************************************************
 * Private Constants
 *****************************************************************************/
 
/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/** HD FDD Changes **/
#ifdef HD_FDD_CONFIG
/*SPR 16855 fix start*/
extern void updateHDFddSemiStaticMap(UInt16 ueIndex, UInt8 reportType);
/*SPR 16855 fix end*/
#endif
/** HD FDD Changes **/

/* SPR 9326 (Bug 542) fix start*/                                
SRSReportNode           *srsReportNodeg_p        = PNULL;
/* SPR 9326 (Bug 542) fix end*/                                
/*****************************************************************************
 * Private Variables 
 *****************************************************************************/
/*CA Changes start  */
UInt8* srsCellConfigMap_gp[MAX_NUM_CELL] = {PNULL};
#ifdef TDD_CONFIG
/*SPR 16855 fix start*/
STATIC SRSUESubFrameMapList srsUESubframeMapFor2ms_g[MAX_NUM_CELL][MAX_NUM_MAPS][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
/*SPR 16855 fix end*/
#elif FDD_CONFIG
/* srsUEShortSubframeMap_g is used for 2ms, 5ms and 10ms periodicity */
/*SPR 16855 fix start*/
STATIC SRSUESubFrameMapList srsUEShortSubframeMap_g[MAX_NUM_CELL][MAX_NUM_MAPS][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
/*SPR 16855 fix end*/
#endif
/*CA Changes end  */

/* In case of FDD, srsUESubframeMapForNot2ms_g is re-used for non-2ms, non-5ms and
 * non-10ms periodicity cases */
/*SPR 16855 fix start*/
STATIC SRSUESubFrameMapList srsUESubframeMapForNot2ms_g[MAX_NUM_CELL][MAX_NUM_MAPS][MAX_SRS_PERIODICITY];
/*SPR 16855 fix end*/
/*FDD_SRS*/
#ifdef FDD_CONFIG

/* The values of this table are filled as per spec 36.213 table 8.2-1 
 The values are for all periodicity*/
STATIC SRSUEConfigTableEntryForNot2ms  srsFddUeConfigTable[] =
{
/* startIndex, endIndex, periodicity, offset */
    {0,         1,          2,           0},
    {2,         6,          5,          -2},
    {7,         16,         10,         -7},
    {17,        36,         20,         -17},
    {37,        76,         40,         -37},
    {77,        156,        80,         -77},
    {157,       316,        160,        -157},
    {317,       636,        320,        -317},
    {637,       1023,       0,            0}
};

/* The values of this table are filled as per spec 36.211 table 5.5.3.3-1
This is a direct indexing array for cell specific srs config index.*/
STATIC CellConfigTableEntry srsFddCellConfigTable[] = 
{
    /* Periodicity,  NumTransOffset, TransOffsetArray*/
    {1,                 1,           {0, 0, 0, 0, 0, 0, 0, 0}},
    {2,                 1,           {0, 0, 0, 0, 0, 0, 0, 0}},
    {2,                 1,           {1, 0, 0, 0, 0, 0, 0, 0}},
    {5,                 1,           {0, 0, 0, 0, 0, 0, 0, 0}},
    {5,                 1,           {1, 0, 0, 0, 0, 0, 0, 0}},
    {5,                 1,           {2, 0, 0, 0, 0, 0, 0, 0}},
    {5,                 1,           {3, 0, 0, 0, 0, 0, 0, 0}},
    {5,                 2,           {0, 1, 0, 0, 0, 0, 0, 0}},
    {5,                 2,           {2, 3, 0, 0, 0, 0, 0, 0}},
    {10,                1,           {0, 0, 0, 0, 0, 0, 0, 0}},
    {10,                1,           {1, 0, 0, 0, 0, 0, 0, 0}},
    {10,                1,           {2, 0, 0, 0, 0, 0, 0, 0}},
    {10,                1,           {3, 0, 0, 0, 0, 0, 0, 0}},
    {10,                7,           {0, 1, 2, 3, 4, 6, 8, 0}},
    {10,                8,           {0, 1, 2, 3, 4, 5, 6, 8}},
};
/*SPR 7086 Fix Start*/
SRSULBandwidthTableEntry *srsFddUplinkSRSBandwidthTable_p[MAX_NUM_CELL] = {PNULL};
/* The values of this table are filled as per table 5.5.3.2-1 in TS 36.211.
The table has entries for SRS UL RBs when 6<=total number of uplink RBs<=40 */
SRSULBandwidthTableEntry srsFddUplinkSRSBandwidthTable6to40
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {36, 1}, {12, 3}, {4, 3}, {4, 1}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {32, 1}, {16, 2}, {8, 2}, {4, 2}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {24, 1}, {4, 6},  {4, 1}, {4, 1}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {20, 1}, {4, 5},  {4, 1}, {4, 1}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {16, 1}, {4, 4},  {4, 1}, {4, 1}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {12, 1}, {4, 3},  {4, 1}, {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {8, 1},  {4, 2},  {4, 1}, {4, 1}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {4, 1},  {4, 1},  {4, 1}, {4, 1}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};

/* The values of this table are filled as per table 5.5.3.2-2 in TS 36.211
The table has entries for SRS UL RBs when 41<=total number of uplink RBs<=60 */
SRSULBandwidthTableEntry srsFddUplinkSRSBandwidthTable41to60
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {48, 1}, {24, 2}, {12, 2}, {4, 3}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {16, 3}, {8, 2},  {4, 2}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {40, 1}, {20, 2}, {4, 5},  {4, 1}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {36, 1}, {12, 3}, {4, 3},  {4, 1}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {32, 1}, {16, 2}, {8, 2},  {4, 2}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {24, 1}, {4, 6},  {4, 1},  {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {20, 1}, {4, 5},  {4, 1},  {4, 1}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {16, 1}, {4, 4},  {4, 1},  {4, 1}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};

/* The values of this table are filled as per table 5.5.3.2-3 in TS 36.211
The table has entries for SRS UL RBs when 61<=total number of uplink RBs<=80 */
SRSULBandwidthTableEntry srsFddUplinkSRSBandwidthTable61to80
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {72, 1}, {24, 3}, {12, 2}, {4, 3}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {64, 1}, {32, 2}, {16, 2}, {4, 4}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {60, 1}, {20, 3}, {4, 5},  {4, 1}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {24, 2}, {12, 2}, {4, 3}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {16, 3}, {8, 2},  {4, 2}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {40, 1}, {20, 2}, {4, 5},  {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {36, 1}, {12, 3}, {4, 3},  {4, 1}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {32, 1}, {16, 2}, {8, 2},  {4, 2}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};

/* The values of this table are filled as per table 5.5.3.2-4 in TS 36.211
The table has entries for SRS UL RBs when 81<=total number of uplink RBs<=100 */
SRSULBandwidthTableEntry srsFddUplinkSRSBandwidthTable81to100
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {96, 1}, {48, 2}, {24, 2}, {4, 6}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {96, 1}, {32, 3}, {16, 2}, {4, 4}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {80, 1}, {40, 2}, {20, 2}, {4, 5}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {72, 1}, {24, 3}, {12, 2}, {4, 3}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {64, 1}, {32, 2}, {16, 2}, {4, 4}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {60, 1}, {20, 3}, {4, 5},  {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {24, 2}, {12, 2}, {4, 3}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {16, 3}, {8, 2},  {4, 2}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};
/*SPR 7086 Fix End*/
#endif
/*FDD_SRS*/

#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
STATIC SRSULBandwidthTableEntry *srsTddUplinkSRSBandwidthTable_p[MAX_NUM_CELL] = {PNULL};
/** CA-TDD Changes End **/

/* The values of this table are filled as per spec 36.213 table 8.2-2 
 The values are for periodicity > 2ms*/
STATIC SRSUEConfigTableEntryForNot2ms  srsTddUeConfigTableForNot2ms[] =
{
/* startIndex, endIndex, periodicity, offset */
    {10,       14,        5,           -10},
    {15,       24,        10,          -15},
    {25,       44,        20,          -25},
    {45,       84,        40,          -45},
    {85,       164,       80,          -85},
    {165,      324,       160,        -165},
    {325,      644,       320,        -325},
    {645,      1023,      0,            0}
};

/* The values of this table are filled as per spec 36.213 table 8.2-2 
The values are for periodicity 2ms. It is a direct indexing array for dedicated
srs config index.*/
STATIC SRSUEConfigTableEntryFor2ms  srsTddConfigTableFor2ms[] =
{
    /*Subframe offsets*/
    {{0,1}},
    {{0,2}},
    {{1,2}},
    {{0,3}},
    {{1,3}},
    {{0,4}},
    {{1,4}},
    {{2,3}},
    {{2,4}},
    {{3,4}}
};

/* The values of this table are filled as per spec 36.211 table 5.5.3.3-2 
This is a direct indexing array for cell specific srs config index.*/
STATIC CellConfigTableEntry srsTddCellConfigTable[] = 
{
    /* Periodicity,  NumTransOffset, TransOffsetArray*/
    {5,                 1,           {1, 0, 0, 0, 0, 0, 0, 0}},
    {5,                 2,           {1, 2, 0, 0, 0, 0, 0, 0}},
    {5,                 2,           {1, 3, 0, 0, 0, 0, 0, 0}},
    {5,                 2,           {1, 4, 0, 0, 0, 0, 0, 0}},
    {5,                 3,           {1, 2, 3, 0, 0, 0, 0, 0}},
    {5,                 3,           {1, 2, 4, 0, 0, 0, 0, 0}},
    {5,                 3,           {1, 3, 4, 0, 0, 0, 0, 0}},
    {5,                 4,           {1, 2, 3, 4, 0, 0, 0, 0}},
    {10,                3,           {1, 2, 6, 0, 0, 0, 0, 0}},
    {10,                3,           {1, 3, 6, 0, 0, 0, 0, 0}},
    {10,                3,           {1, 6, 7, 0, 0, 0, 0, 0}},
    {10,                4,           {1, 2, 6, 8, 0, 0, 0, 0}},
    {10,                4,           {1, 3, 6, 9, 0, 0, 0, 0}},
    {10,                4,           {1, 4, 6, 7, 0, 0, 0, 0}},
};

/* The values of this table are filled as per table 5.5.3.2-1 in TS 36.211.
The table has entries for SRS UL RBs when 6<=total number of uplink RBs<=40 */
STATIC SRSULBandwidthTableEntry srsTddUplinkSRSBandwidthTable6to40
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {36, 1}, {12, 3}, {4, 3}, {4, 1}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {32, 1}, {16, 2}, {8, 2}, {4, 2}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {24, 1}, {4, 6},  {4, 1}, {4, 1}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {20, 1}, {4, 5},  {4, 1}, {4, 1}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {16, 1}, {4, 4},  {4, 1}, {4, 1}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {12, 1}, {4, 3},  {4, 1}, {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {8, 1},  {4, 2},  {4, 1}, {4, 1}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {4, 1},  {4, 1},  {4, 1}, {4, 1}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};

/* The values of this table are filled as per table 5.5.3.2-2 in TS 36.211
The table has entries for SRS UL RBs when 41<=total number of uplink RBs<=60 */
STATIC SRSULBandwidthTableEntry srsTddUplinkSRSBandwidthTable41to60
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {48, 1}, {24, 2}, {12, 2}, {4, 3}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {16, 3}, {8, 2},  {4, 2}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {40, 1}, {20, 2}, {4, 5},  {4, 1}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {36, 1}, {12, 3}, {4, 3},  {4, 1}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {32, 1}, {16, 2}, {8, 2},  {4, 2}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {24, 1}, {4, 6},  {4, 1},  {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {20, 1}, {4, 5},  {4, 1},  {4, 1}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {16, 1}, {4, 4},  {4, 1},  {4, 1}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};

/* The values of this table are filled as per table 5.5.3.2-3 in TS 36.211
The table has entries for SRS UL RBs when 61<=total number of uplink RBs<=80 */
STATIC SRSULBandwidthTableEntry srsTddUplinkSRSBandwidthTable61to80
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {72, 1}, {24, 3}, {12, 2}, {4, 3}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {64, 1}, {32, 2}, {16, 2}, {4, 4}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {60, 1}, {20, 3}, {4, 5},  {4, 1}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {24, 2}, {12, 2}, {4, 3}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {16, 3}, {8, 2},  {4, 2}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {40, 1}, {20, 2}, {4, 5},  {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {36, 1}, {12, 3}, {4, 3},  {4, 1}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {32, 1}, {16, 2}, {8, 2},  {4, 2}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};

/* The values of this table are filled as per table 5.5.3.2-4 in TS 36.211
The table has entries for SRS UL RBs when 81<=total number of uplink RBs<=100 */
STATIC SRSULBandwidthTableEntry srsTddUplinkSRSBandwidthTable81to100
            [MAX_SRS_CELL_BANDWIDTH_CONFIG][MAX_SRS_DEDICATED_BANDWIDTH_CONFIG] =
{
    { {96, 1}, {48, 2}, {24, 2}, {4, 6}},  /*Csrs = 0 & Bsrs = 0, 1, 2, 3*/
    { {96, 1}, {32, 3}, {16, 2}, {4, 4}},  /*Csrs = 1 & Bsrs = 0, 1, 2, 3*/
    { {80, 1}, {40, 2}, {20, 2}, {4, 5}},  /*Csrs = 2 & Bsrs = 0, 1, 2, 3*/
    { {72, 1}, {24, 3}, {12, 2}, {4, 3}},  /*Csrs = 3 & Bsrs = 0, 1, 2, 3*/
    { {64, 1}, {32, 2}, {16, 2}, {4, 4}},  /*Csrs = 4 & Bsrs = 0, 1, 2, 3*/
    { {60, 1}, {20, 3}, {4, 5},  {4, 1}},  /*Csrs = 5 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {24, 2}, {12, 2}, {4, 3}},  /*Csrs = 6 & Bsrs = 0, 1, 2, 3*/
    { {48, 1}, {16, 3}, {8, 2},  {4, 2}},  /*Csrs = 7 & Bsrs = 0, 1, 2, 3*/
};
#endif

/*****************************************************************************
 * Private Functions 
 *****************************************************************************/
 /*****************************************************************************
 * Function Name  : checkValidTransOffset 
 * Inputs         : transOffset - transmission offset calculated
 *                  configIndex - srs cell specific config index
 * Outputs        : None
 * Returns        : SUCCESS / FAILURE
 * Description    : This function checks whether the transmission offset passed 
 *                  belongs to the transmission offset array at the configIndex 
 *                  entry in table srsTddCellConfigTable
 *****************************************************************************/
STATIC  MacRetType checkValidTransOffset(UInt8 transOffset, UInt8 configIndex)
{
    CellConfigTableEntry    *cellConfig_p       = {0};
    UInt8                   count               = 0;
    UInt8                   *transOffset_p      = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);
    
#ifdef TDD_CONFIG
    cellConfig_p = &(srsTddCellConfigTable[configIndex]);
/*FDD_SRS*/
#elif FDD_CONFIG
    cellConfig_p = &(srsFddCellConfigTable[configIndex]);
/*FDD_SRS*/
#endif
    transOffset_p = &(cellConfig_p->transOffsetArray[0]);
    
    for(count = 0; count < (cellConfig_p->numTransOffset); ++count)
    {
        if(transOffset == transOffset_p[count])
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
            return MAC_SUCCESS;
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return MAC_FAILURE;
}
/*SPR 7086 Fix Start*/
 /*****************************************************************************
 * Function Name  : selectUplinkSRSBandwidthTable 
 * Inputs         : internalCellIndex - Cell-Index used at MAc
 * Outputs        : None
 * Returns        : SUCCESS / FAILURE
 * Description    : This function selects the correct uplink SRS bandwidth
 *                  table according to the available uplink RBs configured.
 *****************************************************************************/
STATIC MacRetType selectUplinkSRSBandwidthTable(InternalCellIndex internalCellIndex)
{
    UInt8           ulRBs       = 0;
    
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);
    
    ulRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
              cellParams_p->ulAvailableRBs;

    if((6 <= ulRBs) && (ulRBs <= 40))
    {
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
        srsTddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsTddUplinkSRSBandwidthTable6to40[0][0]);
/** CA-TDD Changes End **/
#elif FDD_CONFIG
        srsFddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsFddUplinkSRSBandwidthTable6to40[0][0]);
#endif
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
        return MAC_SUCCESS;
    }
    else if((41 <= ulRBs) && (ulRBs <= 60))
    {
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
        srsTddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsTddUplinkSRSBandwidthTable41to60[0][0]);
/** CA-TDD Changes End **/
#elif FDD_CONFIG
        srsFddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsFddUplinkSRSBandwidthTable41to60[0][0]);
#endif
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
        return MAC_SUCCESS;
    }
    else if((61 <= ulRBs) && (ulRBs <= 80))
    {
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
        srsTddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsTddUplinkSRSBandwidthTable61to80[0][0]);
/** CA-TDD Changes End **/
#elif FDD_CONFIG
        srsFddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsFddUplinkSRSBandwidthTable61to80[0][0]);
#endif
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
        return MAC_SUCCESS;
    }
    /* +COVERITY 10714 */
    else if((81 <= ulRBs) && (ulRBs <= 100))
    /* -COVERITY 10714 */
    {
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
        srsTddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsTddUplinkSRSBandwidthTable81to100[0][0]);
/** CA-TDD Changes End **/
#elif FDD_CONFIG
        srsFddUplinkSRSBandwidthTable_p[internalCellIndex] = 
            &(srsFddUplinkSRSBandwidthTable81to100[0][0]);
#endif
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
        return MAC_SUCCESS;
    }
    
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR,
                   "[%s] Invalid uplink bandwidth configured ulRBs %d",
                   __func__, ulRBs);
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return MAC_FAILURE;
}

#ifdef TDD_CONFIG
/*SPR 7086 Fix End*/
/*****************************************************************************
 * Function Name  : updateUESrsMapOffFor2msUsingKsrs
 * Inputs         : srsUESubFrOffFor2ms- This is the offset in the 
 *                  srsTddConfigTableFor2ms table entry.
 *                  ueSrsMapOffEntryFor2ms_p- Pointer to the ueSrsMapOffsetsArrayFor2ms 
 *                  maintained by the calling function common for all sfOffset(s) 
 *                  passed above
 *                  numUESrsMapOffFor2ms_p - Pointer to the number of offsets
 *                  in srsMapOffsetsArray
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the srs map offsets for 2 ms using
 *                  sfOffset and Ksrs from table 8.2-3 in TS 36.213 and updates 
 *                  in array.
 *****************************************************************************/
STATIC void updateUESrsMapOffFor2msUsingKsrs(UInt8 srsUESubFrOffFor2ms, 
                UInt8 *ueSrsMapOffEntryFor2ms_p, UInt8 *numUESrsMapOffFor2ms_p)
{
    UInt8  count = (*numUESrsMapOffFor2ms_p);

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    /*Refer to the table 8.2-3 in TS 36.213*/
    switch(srsUESubFrOffFor2ms)
    {
        case 0:
        case 1:
            ueSrsMapOffEntryFor2ms_p[count] = 1;
            ++count;
            ueSrsMapOffEntryFor2ms_p[count] = 6;
            ++count;
        break;
        case 2:
        case 3:
        case 4:
            ueSrsMapOffEntryFor2ms_p[count] = srsUESubFrOffFor2ms;
            ++count;
            ueSrsMapOffEntryFor2ms_p[count] = srsUESubFrOffFor2ms + 5;
            ++count;
        break;
    }
    (*numUESrsMapOffFor2ms_p) = count;
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
}
#endif

/*****************************************************************************
 * Function Name  : insertUESrsNodeInUESubframeMap
 * Inputs         : ueIndex - 
 *                  srsConfigInfo pointer -
 *                  currentUEContext_p - UE context pointer
 *                  nodeEntry_p - Pointer to the node to be inserted
 *                  subFrameList_p - Pointer to subframe list in which the node
 *                                   should be inserted
 * Outputs        : None
 * Returns        : SUCCESS / FAILURE
 * Description    : This function will insert the UE SRS node in UE subframe map 
 *****************************************************************************/
/*SPR 16855 fix start*/
/* +- SPR 17777 */
STATIC  MacRetType insertUESrsNodeInUESubframeMap(UInt16 ueIndex, 
        SRSReportNode **nodeEntry_pp, LTE_LIST *subFrameList_p) 
    /* +- SPR 17777 */
/*SPR 16855 fix end*/
{
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    GET_MEM_FROM_POOL(SRSReportNode,(*nodeEntry_pp),sizeof(SRSReportNode),PNULL);    

    /* + coverity 24556 */
    if (PNULL == *nodeEntry_pp)
    {
        return MAC_FAILURE;
    }
    /* - coverity 24556 */
    else
    {
        /*SPR 16855 fix start*/
        (*nodeEntry_pp)->ueIndex = ueIndex;
        /*SPR 16855 fix end*/
        (*nodeEntry_pp)->nextNode_p = PNULL;

        listInsertNode(subFrameList_p, &(*nodeEntry_pp)->nodeAnchor);

        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
        return MAC_SUCCESS;
    }
}

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : updateSrsUESubframeMapFor2ms
 * Inputs         : ueIndex - Ue Index in ul Ueecontext
 *                  currentUEContext_p - Pointer to UE context
 *                  ueSrsMapOffEntryFor2ms_p - pointer to valid offsets 
 *                  array in srsUESubframeMapFor2ms
 *                  numUESrsMapOffFor2ms - Number of subframe offsets 
 *                  received in the array
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : SUCCESS / FAILURE
 * Description    : This function updates the srsUESubframeMapFor2ms_g for the 
 *                  subframe offsets received in ueSrsMapOffsetEntryFor2ms_p 
 *                  array for 2ms periodicity
 *****************************************************************************/
/*SPR 16855 fix start*/
STATIC MacRetType updateSrsUESubframeMapFor2ms(UInt16 ueIndex,SRSConfigInfo *srsConfigInfo, 
                    UInt8 *ueSrsMapOffEntryFor2ms_p, UInt8 numUESrsMapOffFor2ms, InternalCellIndex internalCellIndex )
/*SPR 16855 fix end*/
{
    UInt8                   count           = 0;
    SRSReportNode           *nodeEntry_p     = PNULL;
    SRSReportNode           *lastNodeEntry_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    /*Add the nodes for initial subframe offsets received in 
      array srsUESubframeMapOff*/
    for(count = 0; count < numUESrsMapOffFor2ms; ++count)
    {
        /*SPR 16855 fix start*/
        /*CA Changes start  */
    /* +- SPR 17777 */
        if( MAC_FAILURE == insertUESrsNodeInUESubframeMap(ueIndex,
                    &nodeEntry_p, &(srsUESubframeMapFor2ms_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][ueSrsMapOffEntryFor2ms_p[count]].
                        srsSubFrameList)))
            /* +- SPR 17777 */
            /*SPR 16855 fix end*/
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in inserting node "
                    "in srsUESubframeMap_g", __func__);
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
            return MAC_FAILURE;
        }
        if(0 == count)
        {   /*First node*/
            /*SPR 16855 fix start*/
            /*CA Changes start  */
            srsConfigInfo->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = nodeEntry_p;
            /*SPR 16855 fix end*/
        }
        else
        {
            /* SPR 9326 (Bug 542) fix start - code removed*/                                
            /*CA Changes end  */
            lastNodeEntry_p->nextNode_p = nodeEntry_p;
            /*Save the current offset in the last node, it will
            be used while deleting*/
            lastNodeEntry_p->nextUESfMapOffset = ueSrsMapOffEntryFor2ms_p[count];
        }
        lastNodeEntry_p = nodeEntry_p;
    }
    
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return MAC_SUCCESS;
}
#endif

/*****************************************************************************
 * Function Name  : getSrsPeriodicityAndOffset 
 * Inputs         : srsConfigIndex - SRS Config Index as received from RRC
 * Outputs        : srsPeriodicity_p - Periodicity os SRS reports
 *                  srsSubFrameOffset_p      - SRS offset    
 * Returns        : None
 * Description    : This function retrieves the srs periodicity and srs subframe
 *                  offset from srsTddUeConfigTableForNot2ms table using 
 *                  srsConfigIndex 
 *****************************************************************************/
STATIC MacRetType getSrsPeriodicityAndOffset(UInt16 srsConfigIndex, UInt16
        *srsPeriodicity_p, UInt16 *srsSubFrameOffset_p)
{
    /* Rel 5.3: Coverity 32366 Fix Start */
    MacRetType resp = MAC_FAILURE;
    /* Rel 5.3: Coverity 32366 Fix End */
    UInt8 index = 0;
#ifdef TDD_CONFIG
    UInt16       sfIndex = 0;
#endif

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    /* Search for the SRS periodicity and subframe offset that falls in the
     * range between SRS config index start and end as in table 
     * srsTddUeConfigTableEntryForNot2ms */
#ifdef TDD_CONFIG
    for (index = 0; index < NUM_OF_ROW_IN_SRS_CONFIG_TABLE_NOT_2MS; index ++)
    {
        /*The table definition will be different for FDD. Rest logic will be same*/
        if (srsConfigIndex >= srsTddUeConfigTableForNot2ms[index].
                startIndex
                && srsConfigIndex <= srsTddUeConfigTableForNot2ms[index].
                endIndex)
        {
            *srsPeriodicity_p  = srsTddUeConfigTableForNot2ms[index].
                periodicity;
            *srsSubFrameOffset_p =  srsTddUeConfigTableForNot2ms[index].
                subFrameOffset + srsConfigIndex;

            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
            resp = MAC_SUCCESS; 
            break;
        }
    }
    /* + coverity 55315 */
    sfIndex = (*srsSubFrameOffset_p) % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    if((0 == sfIndex) || (5 == sfIndex))
    {
        /*To satisfy the Ksrs table 8.2-3 defined in TS 36.213*/
        (*srsSubFrameOffset_p) += 1;
    }
    /* - coverity 55315 */

    /*FDD_SRS*/
#elif FDD_CONFIG
    for (index = 0; index < NUM_OF_ROW_IN_FDD_SRS_CONFIG_TABLE; index ++)
    {
        if (srsConfigIndex >= srsFddUeConfigTable[index].
                startIndex
                && srsConfigIndex <= srsFddUeConfigTable[index].
                endIndex)
        {
            *srsPeriodicity_p  = srsFddUeConfigTable[index].
                periodicity;
            *srsSubFrameOffset_p =  srsFddUeConfigTable[index].
                subFrameOffset + srsConfigIndex;

            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
            resp = MAC_SUCCESS; 
            break;
        }
    }

#endif

    /** base code prob - coverity warning **/
    /* this is used for mod and division in called fn, so should not be 0 */
    if(!(*srsPeriodicity_p))
    {
        resp = MAC_FAILURE; 
    }

    /*FDD_SRS*/

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR,
            "[%s] srsPeriodicity =%d, srsSubFrameOffset =%d Exit.",
            __func__, *srsPeriodicity_p, *srsSubFrameOffset_p);
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return resp;
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : updateSrsUEShortSubframeMap 
 * Inputs         : ueIndex - ue Index in UL UE Context
 *                  ulUEContext_p - Pointer to UE context
 *                  srsPeriodicity - SRS periodicity
 *                  srsSubFrameOffset - First SRS subframe offset
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function updates the srsUEShortSubframeMap for 
 *                  the subframe offsets received in srsUESubframeMapOff array 
 *                  for periodicity less than or equal to 10
 *****************************************************************************/
/*SPR 16855 fix start*/
STATIC  MacRetType updateSrsUEShortSubframeMap(UInt16 ueIndex,SRSConfigInfo *srsConfigInfo, 
                        UInt16  srsPeriodicity, UInt16 srsSubFrameOffset,
                        InternalCellIndex internalCellIndex)
/*SPR 16855 fix end*/
{
    SRSReportNode   *srsReportNode_p = PNULL;
    SRSReportNode   *srsPrevReportNode_p = PNULL;
    UInt16          srsMaxPeriodicity = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME; 
   
    LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Entry.",__func__);
    
    /* Store the head node first */
    /*SPR 16855 fix start*/
    /* +- SPR 17777 */
    if( MAC_FAILURE == insertUESrsNodeInUESubframeMap(ueIndex,
                /*CA Changes start  */
                &srsReportNode_p, &(srsUEShortSubframeMap_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][srsSubFrameOffset].
                /*CA Changes end  */
                    srsSubFrameList)))
    /* +- SPR 17777 */
        /*SPR 16855 fix end*/
    {
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in inserting node in "
                "srsUESubframeMap_g", __func__);
        return MAC_FAILURE;
    }

    srsPrevReportNode_p = srsReportNode_p;
    srsSubFrameOffset += srsPeriodicity;

    /* Store the address of the starting SRS report node in the UL UE
     * context */
    /*SPR 16855 fix start*/
    /*CA Changes start  */
    srsConfigInfo->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = srsReportNode_p;
    /*CA Changes end  */
    /*SPR 16855 fix end*/

    /* Now start inserting rest of the nodes */
    while(srsSubFrameOffset < srsMaxPeriodicity)
    {
        /*SPR 16855 fix start*/
        /* +- SPR 17777 */
        if( MAC_FAILURE == insertUESrsNodeInUESubframeMap(ueIndex,
                /*CA Changes start  */
                &srsReportNode_p, &(srsUEShortSubframeMap_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][srsSubFrameOffset].
                /*CA Changes end  */
                    srsSubFrameList)))
            /* +- SPR 17777 */
            /*SPR 16855 fix end*/
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in inserting node in "
                    "srsUESubframeMap_g", __func__);
            return MAC_FAILURE;
        }
        /* Assign the nextNode_p pointer of previous node to the current
         * report node */
        srsPrevReportNode_p->nextNode_p = srsReportNode_p;
        /*Save the next node map offset, it will be used while deleting*/
        srsPrevReportNode_p->nextUESfMapOffset = srsSubFrameOffset;

        /* Point the prevoius report node to the current node */
        srsPrevReportNode_p = srsReportNode_p; 

        /* Update the subframe offset */
        srsSubFrameOffset += srsPeriodicity;
    }
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR,"[%s] Exit.",__func__);
    return MAC_SUCCESS;
}
#endif
/*****************************************************************************
 * Function Name  : updateSrsUESubframeMapForNot2ms 
 * Inputs         : ueIndex - ue Index in UL UE Context
 *                  ulUEContext_p - Pointer to UE context
 *                  srsPeriodicity - SRS periodicity
 *                  srsSubFrameOffset - First SRS subframe offset
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function updates the srsUESubframeMapForNot2ms_g for 
 *                  the subframe offsets received in srsUESubframeMapOff array 
 *                  for periodicity greater than 2ms
 *****************************************************************************/
/*SPR 16855 fix start*/
STATIC  MacRetType updateSrsUESubframeMapForNot2ms(UInt16 ueIndex,SRSConfigInfo *srsConfigInfo, 
                        UInt16  srsPeriodicity, UInt16 srsSubFrameOffset,
                        InternalCellIndex internalCellIndex)
/*SPR 16855 fix end*/
{
    SRSReportNode   *srsReportNode_p = PNULL;
    SRSReportNode   *srsPrevReportNode_p = PNULL;
    UInt16          srsMaxPeriodicity = MAX_SRS_PERIODICITY;  
    LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Entry.",__func__);

    /* Store the head node first */
   
   
    
    /*SPR 16855 fix start*/
        /* +- SPR 17777 */
    if( MAC_FAILURE == insertUESrsNodeInUESubframeMap(ueIndex,
                /*CA Changes start  */
                &srsReportNode_p, &(srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][srsSubFrameOffset].srsSubFrameList)))
        /* +- SPR 17777 */
                /*CA Changes end  */
        /*SPR 16855 fix end*/
    {
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in inserting node in "
                "srsUESubframeMap_g", __func__);
        return MAC_FAILURE;
    }

    srsPrevReportNode_p = srsReportNode_p;
    srsSubFrameOffset += srsPeriodicity;

    /* Store the address of the starting SRS report node in the UL UE
     * context */
    /*SPR 16855 fix start*/
    /*CA Changes start  */
    srsConfigInfo->srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = srsReportNode_p;
    /*CA Changes end  */
    /*SPR 16855 fix end*/
    /* Now start inserting rest of the nodes */
    while(srsSubFrameOffset < srsMaxPeriodicity)
    {

        
        /*SPR 16855 fix start*/
        /* +- SPR 17777 */
        if( MAC_FAILURE == insertUESrsNodeInUESubframeMap(ueIndex,
                    /*CA Changes start  */
                    &srsReportNode_p, &(srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][srsSubFrameOffset].srsSubFrameList)))
        /* +- SPR 17777 */
                    /*CA Changes end  */
            /*SPR 16855 fix end*/
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in inserting node in "
                    "srsUESubframeMap_g", __func__);
            return MAC_FAILURE;
        }
        

        /* Assign the nextNode_p pointer of previous node to the current
         * report node */
        srsPrevReportNode_p->nextNode_p = srsReportNode_p;
        /*Save the next node map offset, it will be used while deleting*/
        srsPrevReportNode_p->nextUESfMapOffset = srsSubFrameOffset;

        /* Point the prevoius report node to the current node */
        srsPrevReportNode_p = srsReportNode_p; 

        /* Update the subframe offset */
        srsSubFrameOffset += srsPeriodicity;
    }

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR,"[%s] Exit.",__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : processSRSReportNodesForUSubframes
 * Inputs         : srsUESubframeMap_p - Pointer to srsUESubframeMapFor2ms_g or
 *                                          srsUESubframeMapForNot2ms_g
 *                  ueSrsMapOff - subframe offset in subframe map for which 
 *                                  nodes are processed
 *                  currSubFrameNum -
 *                  ulschUciRachInfo_p - pointer to uci rach info
 *                  ulTTI - uplink TTI
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes the SRS report nodes for uplink 
 *                  subframes in case of 2ms periodicity or > 2ms periodicity.
 *****************************************************************************/
#ifdef TDD_CONFIG
STATIC void processSRSReportNodesForUSubframes(SRSUESubFrameMapList *srsUESubframeMap_p,
				                /* TDD Config 0 Changes Start */
                                UInt16 ueSrsMapOff, 
				                UInt8 ulSubFrameNum,
				                /* TDD Config 0 Changes End */
                                ULSchUciRachPoolInfo* ulschUciRachInfo_p,
                                /* SPR 15909 fix start */
                                tickType_t ulTTI, /* MEAS_GAP_CHG */
                                /* SPR 15909 fix end */
                                /*CA Changes start  */
                                InternalCellIndex internalCellIndex)
                                /*CA Changes end  */
#elif FDD_CONFIG
STATIC void processSRSReportNodesForUSubframes(SRSUESubFrameMapList *srsUESubframeMap_p,
                                UInt16 ueSrsMapOff, ULSchUciRachPoolInfo *ulschUciRachInfo_p,
                                PdcchULOutputInfo *pdcchULOutputInfo_p, 
                                /* SPR 15909 fix start */
                                tickType_t ulTTI, /* MEAS_GAP_CHG */
                                /* SPR 15909 fix end */
                                /*CA Changes start  */
                                InternalCellIndex internalCellIndex)
                                /*CA Changes end  */
#endif
{
    UInt16                  ueIndex                     = 0;
    ULUEContextInfo         *uLUEContextInfo_p          = PNULL;
    ULUEContext             *ulUEContext_p              = PNULL;
    SRSReportNode           *srsReportNode_p            = PNULL;
    UInt16                  srsPduIndex                 = 0;
    MACUlConfigPduInfo      pduType                     = MAC_SS_PDU_INVALID; 
    /*FDD_SRS*/
    UInt16                  ulSchUciInfoIndex           = ULSCH_UCI_INVALID_INDEX;
#ifdef TDD_CONFIG
    UeScheduledInfo         *ueScheduled_p              = PNULL;
#endif
    /*FDD_SRS*/
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    srsReportNode_p = (SRSReportNode *)getListNode(
            &(srsUESubframeMap_p[ueSrsMapOff].srsSubFrameList), 0);

    if(LTE_TRUE == ulschUciRachInfo_p->isSRSPresent)
    {
        if(PNULL != srsReportNode_p)
        {

            while(srsReportNode_p)
            {
                pduType = MAC_SS_PDU_INVALID; 
                ueIndex = srsReportNode_p->ueIndex;
                uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];;
                /* If pendingDeleteFlag is true for this UE and SRS node still exists,
                 * delete the node from the MAP and process the next node  */
                if (uLUEContextInfo_p->pendingDeleteFlag)
                {
                    lteWarning("Pending Delete Flag is set For this UE Index [%d]",
                            ueIndex);
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /*Get the pduType stored while processing PUCCH map*/
                ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;

                /* If the UE context is NULL, delete the Node from the list and process 
                 * the next node */
                if (PNULL == ulUEContext_p)
                {
                    lteWarning("No UL UE context created for UE Index [%d]", ueIndex);
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /*SPR 6497 Fix Start*/
                /*If SRS Configuration is in WAITING State, then delete node*/
                if(ulUEContext_p->srsConfigInfo.srsConfigType == SRS_DEDICATED_CONFIG_TYPE_SETUP_WAITING)
                {
                    srsReportNode_p = (SRSReportNode *)getNextListNode(
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /*SPR 6497 Fix End*/


                /* MEAS_GAP_CHG */
                if (TRUE == isTTIInMeasGapPeriod(ulUEContext_p, ulTTI))
                {
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /* MEAS_GAP_CHG */
#ifdef TDD_CONFIG
				/* TDD Config 0 Changes Start */
                /* SPR 14204 FIXES START */
				ueScheduled_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
                /* SPR 14204 FIXES END */
				/* TDD Config 0 Changes End */
                if(PNULL != ueScheduled_p)
                {
                    ulSchUciInfoIndex = ueScheduled_p->ulschUciInfoIndex;

                    if(ULSCH_UCI_INVALID_INDEX != ulSchUciInfoIndex)
                    {
                        pduType = ulschUciRachInfo_p->ulschUciInfo[ulSchUciInfoIndex].
                            pduType;

                        /* Update the nSRS flag with the srs present parameter */
                        ulschUciRachInfo_p->ulschUciInfo[ulSchUciInfoIndex].nSRS = 
                            ulschUciRachInfo_p->isSRSPresent;
                    }
                }
                /*FDD_SRS*/
#elif FDD_CONFIG
                ulSchUciInfoIndex = pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].
                    ulschUciInfoIndex;

                /* SPR 14877 SRS Changes*/
                if(ULSCH_UCI_INVALID_INDEX == ulSchUciInfoIndex)
                {
                    ulSchUciInfoIndex = pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].ulschInfoIndex;
                }
                /* SPR 14877 SRS Changes*/
                if(ULSCH_UCI_INVALID_INDEX != ulSchUciInfoIndex)
                {
                    pduType = ulschUciRachInfo_p->ulschUciInfo[ulSchUciInfoIndex].
                        pduType;

                    /* Update the nSRS flag with the srs present parameter */
                    ulschUciRachInfo_p->ulschUciInfo[ulSchUciInfoIndex].nSRS = 
                        ulschUciRachInfo_p->isSRSPresent;
                }
#endif
                /*FDD_SRS*/
                /*
                   Collision 1 - A UE shall not transmit SRS whenever SRS and PUCCH 
                   format 2/2a/2b transmissions happen to coincide in the same subframe.
                   Collision 2 - A UE shall not transmit SRS whenever SRS and 
                   PUCCH transmissions carrying ACK/NACK/Positive SR coincide in the same 
                   subframe. This is valid when ackNackSRSSimultaneousTrans flag is set to
                   FALSE. If the flag is set to TRUE, then this collsion is invalid and 
                   SRS is not dropped.
                   */
                /*
                   Collisions handling -
                   1. MAC_SS_PDU_ULSCH, MAC_SS_PDU_ULSCH_CQI_RI, MAC_SS_PDU_ULSCH_HARQ, 
                   MAC_SS_PDU_ULSCH_CQI_HARQ_RI - For all these pdu types, control info
                   is multiplexed with data on PUSCH. So, the collision is invalid.
                   2. MAC_SS_PDU_UCI_CQI, MAC_SS_PDU_UCI_CQI_HARQ - Transmitted on PUCCH
                   (format 2/2a/2b).So drop SRS.
                   3. MAC_SS_PDU_UCI_SR - UE may send a positive SR or no SR at all. So,we
                   cannot determine its collision with SRS. Both SR & SRS pdus are sent
                   in this case. So, the collision is invalid.
                   4. MAC_SS_PDU_UCI_HARQ - If ackNackSRSSimultaneousTrans flag is FALSE, 
                   then drop SRS else collision is invalid.
                   5. MAC_SS_PDU_UCI_SR_HARQ - Two combinations are possible; UE will send
                   either HARQ or SR+HARQ. In both the cases, drop SRS if 
                   ackNackSRSSimultaneousTrans flag is FALSE.
                   6. MAC_SS_PDU_UCI_CQI_SR - CQI has a collision with positive SR. So, 
                   UE either sends a positive SR or CQI. In case CQI is transmitted, 
                   SRS will be dropped. In case positive SR is transmitted and the flag
                   ackNackSRSSimultaneousTrans is FALSE, then SRS will be dropped.
                   So, overall, if the ackNackSRSSimultaneousTrans flag is set to FALSE,
                   then drop SRS.
                   7. MAC_SS_PDU_UCI_CQI_SR_HARQ - Two combinations are possible; UE will
                   send either HARQ+CQI or HARQ+SR. For HARQ+CQI, refer to 2. For 
                   HARQ+SR, refer to 5. Overall,drop SRS if ackNackSRSSimultaneousTrans
                   is set to FALSE.
                   */ 

                if((MAC_SS_PDU_UCI_CQI == pduType) || 
                        (MAC_SS_PDU_UCI_CQI_HARQ == pduType))
                {
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /*CA Changes start  */
                if(FALSE == (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->srsCommonSetupInfo.ackNackSRSSimultaneousTrans))
                /*CA Changes end  */
                {
                    if((MAC_SS_PDU_UCI_HARQ == pduType) ||
                            (MAC_SS_PDU_UCI_SR_HARQ == pduType) ||
                            (MAC_SS_PDU_UCI_CQI_SR == pduType) ||
                            (MAC_SS_PDU_UCI_CQI_SR_HARQ == pduType))
                    {
                        srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                                (LTE_LIST_NODE *)srsReportNode_p);
                        continue;
                    }
                }

                /* Update the SRS information in ulSchUciInfo array */
                srsPduIndex = ulschUciRachInfo_p->numSRSPdus;
                ulschUciRachInfo_p->srsPduInfo[srsPduIndex].rnti =
                    ulUEContext_p->crnti;
                ulschUciRachInfo_p->srsPduInfo[srsPduIndex].srsDedicatedSetupInfo_p
                    = &(ulUEContext_p->srsConfigInfo.srsDedicatedSetupInfo); 

                ++(ulschUciRachInfo_p->numSRSPdus);

                if(0 == ((ulUEContext_p->srsConfigInfo).srsDedicatedSetupInfo.duration))
                {   
                    /*Delete the node in case of single transmission*/
                    listDeleteNode(&(srsUESubframeMap_p[ueSrsMapOff].srsSubFrameList), 
                            &(srsReportNode_p->nodeAnchor));
                    freeMemPool((void*)srsReportNode_p);
                    /* + SPR 5584 (COVERITY CID 25536) FIX */
                    srsReportNode_p = PNULL;
                    /* - COVERITY CID 25536 FIX */

                    /*SPR 16855 fix start*/
#ifdef FDD_CONFIG
                    if (ulUEContext_p->srsConfigInfo.srsPeriodicity <= 10 )
#else 
                    if (ulUEContext_p->srsConfigInfo.srsPeriodicity == 2 )
#endif
                    {
                        /*CA Changes start  */
                        (ulUEContext_p->srsConfigInfo).srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex] = PNULL;
                        /*CA Changes end  */
                    }
                    else
                    {
                        /*CA Changes start  */
                        (ulUEContext_p->srsConfigInfo).srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex] = PNULL;
                        /*CA Changes end  */
                    }
                    /*SPR 16855 fix end*/
                }

                /* Process the next node */
                srsReportNode_p = (SRSReportNode *)getNextListNode(               
                        (LTE_LIST_NODE *)srsReportNode_p);
            }
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return;
}
        
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : processSRSReportNodesForSSubframes
 * Inputs         : srsUESubframeMap_p - Pointer to srsUESubframeMapFor2ms_g or
 *                                          srsUESubframeMapForNot2ms_g
 *                  ueSrsMapOff - subframe offset in subframe mao for which 
 *                                  nodes are processed
 *                  ulSubFrameNum - subframe no for which nodes are processed
 *                  currSubFrameNum -
 *                  ulschUciRachInfo_p - pointer to uci rach info
 *                  ulTTI - uplink TTI
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : None 
 * Returns        : None
 * Description    : This function process the SRS report nodes for special subframes 
 *                  in case of 2ms periodicity or not 2ms periodicity.
 *****************************************************************************/
STATIC void processSRSReportNodesForSSubframes(SRSUESubFrameMapList *srsUESubframeMap_p,
                                UInt16 ueSrsMapOff,
                                /* TDD Config 0 Changes Start 
                                 * These variables are not used inside the function
                                UInt8 ulSubFrameNum, 
                                UInt8 currSubFrameNum, */
                                /* TDD Config 0 Changes End */
                                ULSchUciRachPoolInfo* ulschUciRachInfo_p, 
                                /* SPR 15909 fix start */
                                tickType_t ulTTI,
                                /* SPR 15909 fix end */
                                InternalCellIndex internalCellIndex) /* MEAS_GAP_CHG */
{
    UInt16                      ueIndex                     = 0;
    ULUEContextInfo             *uLUEContextInfo_p          = PNULL;
    ULUEContext                 *ulUEContext_p              = PNULL;
    SRSReportNode               *srsReportNode_p            = PNULL;
    UInt16                      srsPduIndex                 = 0;
    /* + SPR 5584 (COVERITY CID 25703) FIX */
    SRSReportNode               *tmpSrsReportNode_p         = PNULL;
    /* - SPR 5584 (COVERITY CID 25703) FIX */


    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    srsReportNode_p = (SRSReportNode *)getListNode(
            &(srsUESubframeMap_p[ueSrsMapOff].srsSubFrameList), 0);

    /*TDD_SRS_Enhancement*/
    if(TRUE == ulschUciRachInfo_p->isSRSPresent)
        /*TDD_SRS_Enhancement*/
    { 
        if(PNULL != srsReportNode_p)
        {
            while(srsReportNode_p)
            {
                ueIndex = srsReportNode_p->ueIndex;
                uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];;
                /* If the UE context is NULL, delete the Node from the list and process 
                 * the next node */
                /*coverity-530-55305*/
                /*code removed*/
                /*coverity-530-55305*/
                /* If pendingDeleteFlag is true for this UE and SRS node still exists,
                 * delete the node from the MAP and process the next node  */
                if (uLUEContextInfo_p->pendingDeleteFlag)
                {
                    lteWarning("Pending Delete Flag is set For this UE Index [%d]",
                            ueIndex);
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;
                /* If the UE context is NULL, delete the Node from the list and process 
                 * the next node */
                if (PNULL == ulUEContext_p)
                {
                    lteWarning("No UL UE context created for UE Index [%d]", ueIndex);
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /*Collsion handling 5 - In upPts, If SRS bandwidth is more than the 
                  total uplink bandwidth, then drop SRS*/

                /* MEAS_GAP_CHG */
                if (TRUE == isTTIInMeasGapPeriod(ulUEContext_p, ulTTI))
                {
                    srsReportNode_p = (SRSReportNode *)getNextListNode(                   
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }
                /* MEAS_GAP_CHG */
                
                if((ulUEContext_p->srsConfigInfo).srsULBandwidthCollision)
                {
                    srsReportNode_p = (SRSReportNode *)getNextListNode(               
                            (LTE_LIST_NODE *)srsReportNode_p);
                    continue;
                }

                /* Update the SRS information in ulSchUciInfo array */
                srsPduIndex = ulschUciRachInfo_p->numSRSPdus;
                ulschUciRachInfo_p->srsPduInfo[srsPduIndex].rnti =
                    ulUEContext_p->crnti;
                ulschUciRachInfo_p->srsPduInfo[srsPduIndex].
                    srsDedicatedSetupInfo_p = &(ulUEContext_p->srsConfigInfo.
                            srsDedicatedSetupInfo); 

                ++(ulschUciRachInfo_p->numSRSPdus);

                /* + SPR 5584 (COVERITY CID 25703) FIX */
                tmpSrsReportNode_p = (SRSReportNode *)getNextListNode(
                               (LTE_LIST_NODE *)srsReportNode_p);
                /* - SPR 5584 (COVERITY CID 25703) FIX */

                if(0 == ((ulUEContext_p->srsConfigInfo).srsDedicatedSetupInfo.duration))
                {
                    /*Delete the SRS node in case of single transmission*/
                    listDeleteNode(&(srsUESubframeMap_p[ueSrsMapOff].srsSubFrameList),
                            &(srsReportNode_p->nodeAnchor));
                    freeMemPool((void*)srsReportNode_p);
                    /*SPR 16855 fix start*/
                    if (ulUEContext_p->srsConfigInfo.srsPeriodicity == 2 )
                    {
                        /*CA Changes start  */
                        (ulUEContext_p->srsConfigInfo).srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex] = PNULL;
                    }
                    else
                    {
                        (ulUEContext_p->srsConfigInfo).srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex] = PNULL;
                        /*CA Changes end  */
                    }
                    /*SPR 16855 fix end*/
                }

                /* Process the next node */
                /* + SPR 5584 (COVERITY CID 25703) FIX */
                srsReportNode_p = tmpSrsReportNode_p;
                /* - SPR 5584 (COVERITY CID 25703) FIX */
            }
        }
    }
/* SPR 1868 changes start */
/* SPR 1868 changes end */

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return;
}
#endif

/*****************************************************************************
 * Public Functions 
 *****************************************************************************/
/*****************************************************************************
 * Function Name  : initCellSpecificSRSConfig 
 * Inputs         : internalCellIndex - Cell-Index used at MAc
 * Outputs        : None
 * Returns        : None
 * Description    : This function will maintain the superset of permitted 
 *                  subframes for all the UEs in which they are allowed to 
 *                  transmit SRS. 
 *****************************************************************************/
void initCellSpecificSRSConfig(InternalCellIndex internalCellIndex)
{
    UInt8           srsSfConfigIndex      = 0;
    UInt8           subFrNum              = 0;
    UInt8           transOffset           = 0;
    UInt8           configPeriod          = 0;

    LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Entry.",__func__);



    /*Always invoked with cellConfig_g.srsCommonSetupInfo set to correct srs 
      cell specific config info*/
    srsSfConfigIndex = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->srsCommonSetupInfo.srsSubframeConfig;
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR,
            "[%s] SRS Cell specific config index received : %d",  
            __func__, srsSfConfigIndex);

#ifdef TDD_CONFIG
    configPeriod = srsTddCellConfigTable[srsSfConfigIndex].configPeriod;
    /*FDD_SRS*/
#elif FDD_CONFIG
    configPeriod = srsFddCellConfigTable[srsSfConfigIndex].configPeriod;
    /*FDD_SRS*/
#endif

    for(subFrNum = 0; subFrNum < NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME; 
            ++subFrNum)
    {
        /*Refer to the formula in section 5.5.3.3 in TS 36.211
          The formula is same for both FDD and TDD. Only new table will be 
          defined for FDD, rest will be the same for cell config*/
        transOffset = subFrNum % configPeriod;
        if(checkValidTransOffset(transOffset, srsSfConfigIndex))
        {
            /*Set the subframe index permitted for SRS transmissions*/
            *(srsCellConfigMap_gp[internalCellIndex] + subFrNum) = LTE_TRUE;
        }
    }

    /*SPR 7086 Fix Start*/
    /*Select the uplinkSRSBandwidth table according to total number of uplink 
      bandwidth RBs*/
    if(MAC_FAILURE == selectUplinkSRSBandwidthTable(internalCellIndex))
    {
        lteWarning("No uplink bandwidth table selected for SRS. So, SRS \
                collision with total uplink bandwidth will not be handled.\n");
    }
    else
    {
        /*In case of cell setup transmissionComb = 0, as UE config has not been received yet*/
        determineAndUpdateSrsPosition(0, internalCellIndex);
    }
    /*SPR 7086 Fix End*/

    LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
}

/*SPR 7086 Fix Start*/
/*****************************************************************************
 * Function Name  : determineAndUpdateSrsPositions
 * Inputs         : transmissionComb
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : 
 * Returns        : None
 * Description    : This function calculates and update the SRS Start
 *                  and SRS End Position the Cell Config Structure
 *****************************************************************************/
STATIC void determineAndUpdateSrsPosition(UInt8 transmissionComb, InternalCellIndex internalCellIndex)
{
    UInt8  ulSRSBandwidth = 0;
    UInt8  numOfSubcarrier = 0;
    UInt16 refSignalSeqLength = 0; /* Stored in terms of Subcarriers*/
    UInt16 srsFreqDomainStart = 0; /*Stored in terms of Subcarriers*/
    SRSULBandwidthTableEntry *temp = PNULL;

    /*Retrieve the uplink bandwidth for SRS*/
    CellConfigParams *cellParams_p = 
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    if(1 == cellParams_p->cyclicPrefix)
    {
        numOfSubcarrier = NUM_OF_SUBCARRIER_NORMAL;
    }
    else
    {
        numOfSubcarrier = NUM_OF_SUBCARRIER_EXTENDED;
    }
    /*Determining SRS Start Subcarrier and SRS End Subcarrier*/
#ifdef TDD_CONFIG /* SPR# 9916 fix */
/** CA-TDD Changes Start **/
    temp = srsTddUplinkSRSBandwidthTable_p[internalCellIndex] + 
        ((cellParams_p->srsCommonSetupInfo.srsBandwidthConfig * MAX_SRS_UE_BANDWIDTH_INDEX) + 0);
/** CA-TDD Changes End **/
#elif FDD_CONFIG
    temp = srsFddUplinkSRSBandwidthTable_p[internalCellIndex] + 
        ((cellParams_p->srsCommonSetupInfo.srsBandwidthConfig * MAX_SRS_UE_BANDWIDTH_INDEX) + 0);
#endif
    ulSRSBandwidth = temp->mSRS;
    refSignalSeqLength = (UInt16)(ulSRSBandwidth*numOfSubcarrier);

    /*Determining K0*/
    /* SPR 18122 Changes Start*/
    srsFreqDomainStart = (UInt16)((((DIVIDE_BY_TWO(cellParams_p->ulAvailableRBs))- 
                    (DIVIDE_BY_TWO(ulSRSBandwidth)))*numOfSubcarrier) + transmissionComb);
    /* SPR 18122 Changes End*/

    /*Updating Start RB and End RB in Cell Congig structure*/
    cellParams_p->srsFreqDomainStartRB = 
        (UInt8)((srsFreqDomainStart + numOfSubcarrier)/numOfSubcarrier);
    cellParams_p->srsFreqDomainEndRB = 
        (UInt8)((srsFreqDomainStart + numOfSubcarrier +
                    refSignalSeqLength -1) /numOfSubcarrier);
}
/*SPR 7086 Fix End*/

/*****************************************************************************
 * Function Name  : initSrsUESubFrameMaps 
 * Inputs         : numOfCells
 * Outputs        : None 
 * Returns        : None
 * Description    : This function Initializes the LIST POINTER Array of 
 *                  srsUESubframeMapFor2ms_g & srsUESubframeMapForNot2ms_g maps
 *****************************************************************************/
 /* CA Changes start */
void initSrsUESubFrameMaps(UInt8  numOfCells)
 /* CA Changes end */
{

    UInt16      subFrameOffset = 0;
    UInt8       cellCount = 0;
    /*SPR 16855 fix start*/
    UInt16      index = 0;
    /*SPR 16855 fix end*/
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    /* Start 128 UE: Memory Changes */
    /* CA Changes start */
    /*Pool for both srsUESubframeMapFor2ms_g & srsUESubframeMapForNot2ms_g maps*/
    for ( cellCount = 0; cellCount < numOfCells; cellCount++)
    {    
        createMemPool(sizeof (SRSReportNode), (cellSpecificParams_g.\
            cellConfigAndInitParams_p[cellCount]->initParams_p->\
            macMaxNumOfUeSupported * MAX_SRS_PERIODICITY/10));
    }
    /* CA Changes end */
    /* End 128 UE: Memory Changes */

    /*Initialize the map for (2ms periodicity for TDD)/(for periodicities <= 10 in case of FDD)*/
    /*SPR 16855 fix start*/
    /*CA Changes start  */
    for(cellCount = 0; cellCount < numOfCells; cellCount++ )
    {
        for(index = 0; index < MAX_NUM_MAPS ; index++ )
        {
            for(subFrameOffset = 0; subFrameOffset < NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
                    subFrameOffset++)
            {
#ifdef TDD_CONFIG
                listInit(&(srsUESubframeMapFor2ms_g[cellCount][index][subFrameOffset].srsSubFrameList));
#else  
                listInit(&(srsUEShortSubframeMap_g[cellCount][index][subFrameOffset].srsSubFrameList));
#endif
        /*FDD_SRS*/ 
            }

        /*FDD_SRS*/ 
            /*Initialize the map for (> 2ms periodicity in case of TDD)/(> 10 perodicities in case of FDD)*/
            for (subFrameOffset = 0; subFrameOffset < MAX_SRS_PERIODICITY; 
                    subFrameOffset++)
            {
                listInit(&(srsUESubframeMapForNot2ms_g[cellCount][index][subFrameOffset].srsSubFrameList));
            }
        }
    }        

    /* Create pool for both shortPeriodicSRSMapInfo_gp and longPeriodicSRSMapInfo_gp */
    createMemPool(sizeof(mapInfo), numOfCells * 2);

    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
        GET_MEM_FROM_POOL(mapInfo, shortPeriodicSRSMapInfo_gp[cellCount], sizeof(mapInfo), PNULL);
        GET_MEM_FROM_POOL(mapInfo, longPeriodicSRSMapInfo_gp[cellCount], sizeof(mapInfo), PNULL);

        shortPeriodicSRSMapInfo_gp[cellCount]->passiveIndex = 0;
        shortPeriodicSRSMapInfo_gp[cellCount]->activeIndex = 1;
        longPeriodicSRSMapInfo_gp[cellCount]->passiveIndex = 0;
        longPeriodicSRSMapInfo_gp[cellCount]->activeIndex =1;
    }
   /*SPR 16855 fix end*/
/*FDD_SRS*/ 

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return;
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processSRSShortSubframeMap 
 * Inputs         : srsUEConfigIndex - srs Config Index
 *                : duration - srs Duration
 *                  ueIndex -
 *                  internalCellIndex - Cell-Index used at MAc
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : SUCCESS or FAILURE
 * Description    : This function will update or insert nodes in the 
 *                : srsUEShortSubframeMap_g according to duration
 *                : with UE nodes at the valid subframe offsets for 2ms,5ms and 
 *                : 10ms periodicities.
 *****************************************************************************/
/*SPR 16855 fix start*/
MacRetType processSRSShortSubframeMap(UInt16 srsUEConfigIndex,UInt16 duration,
                                      UInt16 ueIndex,SRSConfigInfo   *srsConfigInfo,
                                      InternalCellIndex internalCellIndex)
/*SPR 16855 fix end*/
{
    UInt16       srsPeriodicity = 0, srsStartSfOff = 0;
    UInt16       nearestUESrsMapOff = 0;
    UInt32       currSFN = 0, currSF = 0, currUESrsMapOff = 0;
    SInt32       ueSrsMapOffGap = 0;
    SRSReportNode   *srsReportNode_p        = PNULL;
    UInt16          srsMaxPeriodicity = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;  

    if (MAC_FAILURE == getSrsPeriodicityAndOffset(srsUEConfigIndex, 
                &srsPeriodicity, &srsStartSfOff))
    {
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Invalid SRS Config Index "
                "Error getting SRS Periodicity and Offset");
        LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
        return MAC_FAILURE;
    }
    if(0 != duration)
    {  
        /*Indefinite SRS transmissions*/
        /*SPR 16855 fix start*/
        if(MAC_FAILURE == updateSrsUEShortSubframeMap(ueIndex,srsConfigInfo, 
                    srsPeriodicity, srsStartSfOff, internalCellIndex))
            /*SPR 16855 fix end*/
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Error in creating SRS Subframe "
                    "map for UE");
            LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
            return MAC_FAILURE;
        }
    }
    else
    {   
        /*Single SRS transmission - This node will be deleted after it is 
          processed once*/
        getSFAndSFN(&currSFN,&currSF,internalCellIndex);
        /*Take delay into account while calculating the current offset in 
          the subframe map.*/
        currUESrsMapOff = ((1024 * currSFN) + (10 * currSF) + 
                RRC_UE_DELAY_FACTOR) % srsMaxPeriodicity;
        /*Calculate the gap b/w current and start sf offsets*/
        ueSrsMapOffGap = (currUESrsMapOff - srsStartSfOff + 
                srsMaxPeriodicity) % srsPeriodicity;
        /*Subtract the above diff and add the periodicity to current sf 
          offset. This final offset will be a valid sf offset*/        
        nearestUESrsMapOff = (currUESrsMapOff + srsPeriodicity - 
                ueSrsMapOffGap) % srsMaxPeriodicity;

        /*SPR 16855 fix start*/
        /* +- SPR 17777 */
        if(MAC_FAILURE == insertUESrsNodeInUESubframeMap
                (ueIndex,&srsReportNode_p, 
                 /*CA Changes start  */
                 &(srsUEShortSubframeMap_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][nearestUESrsMapOff].srsSubFrameList)))
        /* +- SPR 17777 */
                 /*CA Changes end  */
            /*SPR 16855 fix end*/
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in "
                    "inserting node in srsUESubframeMap_g",
                    __func__);
            LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
            return MAC_FAILURE;
        }
        /*Only one node is added, so update the start offset with the
          nearest offset calculated*/
        srsStartSfOff = nearestUESrsMapOff;
        /*SPR 16855 fix start*/
        /*CA Changes start  */
        srsConfigInfo->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = 
        /*CA Changes end  */
            srsReportNode_p;
        /*SPR 16855 fix end*/
    }
    /*SPR 16855 fix start*/
    srsConfigInfo->srsPeriodicity = srsPeriodicity;
    srsConfigInfo->srsSubFrameOffset = srsStartSfOff;
    /*SPR 16855 fix end*/
    return MAC_SUCCESS;
}
#endif


/*****************************************************************************
 * Function Name  : configureDedicatedSRSConfig 
 * Inputs         : ueIndex - ueIndex in UL UE context
 *                  dedicatedSetupInfo_p - Pointer to SRS config info
 *                  srsUEConfigType - SETUP/RELEASE
 *                  currSFN,currSF,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : SUCCESS / FAILURE
 * Description    : This function configures the dedicated SRS configuration.
 *                  It will update the srsUESubframeMapFor2ms_g or 
 *                  srsUESubframeMapForNot2ms_g according to the periodicity 
 *                  with UE nodes at the valid subframe offsets.
 *****************************************************************************/
/*SPR 16855 fix start*/
MacRetType configureDedicatedSRSConfig(
        UInt16 ueIndex,
        SRSConfigInfo *srsConfigInfo,
        SRSDedicatedSetupInfo   *dedicatedSetupInfo_p,
        UInt8   srsUEConfigType,
        UInt32 currSFN, UInt32 currSF,
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
        /*CA Changes end */
/*SPR 16855 fix end*/
{
#ifdef TDD_CONFIG
    UInt8        ulSRSBandwidth = 0, srsCellBandwidth = 0, srsUEBandwidth = 0;
    UInt8        srsUESubFrOffFor2ms = 0, numUESrsMapOffFor2ms = 0;
    UInt16       minSrsMapOffGap = 0;
    UInt8        ueSrsMapOffArrayFor2ms[2 * NUM_SF_OFFSETS_FOR_SRS_UE_CONFIG_2_MS] = {0};
    UInt8        count                      = 0;
    SRSULBandwidthTableEntry *temp = PNULL;
#endif
    UInt16       nearestUESrsMapOff = 0;
    UInt16       srsPeriodicity = 0, srsStartSfOff = 0, srsUEConfigIndex = 0;
    /*SPR 16855 fix start*/
    /*SPR 16855 fix end*/
    UInt32       currUESrsMapOff = 0;
    SInt32       ueSrsMapOffGap = 0;
    SRSReportNode   *srsReportNode_p        = PNULL;
    UInt16          srsMaxPeriodicity = MAX_SRS_PERIODICITY;  

    LTE_MAC_UT_LOG(LOG_MINOR,SRS_MGR,"[%s] Entry.\n",__func__);

    srsUEConfigIndex = dedicatedSetupInfo_p->srsConfigIndex;
    /*Only SETUP is expected in configure as it is invoked from two functions:
    createMACUEEntity - Here, the config type must be setup
    reconfigureSrsDedicatedConfig - Here, the previous configuration is either 
    deleted first and then this fn is invoked or else this fn is invoked for 
    the first time. In both the cases, the config type should be SETUP*/
    if(SRS_DEDICATED_CONFIG_TYPE_SETUP != srsUEConfigType)
    {
        LOG_MAC_ERROR(MAC_RRC_INF,"Invalid srsUEConfigType %u UE %u\n",
                srsUEConfigType,ueIndex);
        return MAC_FAILURE;
    }

#ifdef TDD_CONFIG
    /*Retrieve the uplink bandwidth for SRS*/
    srsCellBandwidth = (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->srsCommonSetupInfo).srsBandwidthConfig;
    srsUEBandwidth = dedicatedSetupInfo_p->srsBandwidth;
/** CA-TDD Changes Start **/
    temp = srsTddUplinkSRSBandwidthTable_p[internalCellIndex] + ((srsCellBandwidth * 
                MAX_SRS_UE_BANDWIDTH_INDEX) + srsUEBandwidth);
/** CA-TDD Changes End **/
    ulSRSBandwidth = temp->mSRS; 

    /*Update the srsULBandwidthCollision flag on the basis of RBs allocated
      for SRS and total uplink bandwidth. This is used while processing of 
      special subframes */
    if(ulSRSBandwidth > (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                   cellParams_p->ulAvailableRBs))
    {
        /*SPR 16855 fix start*/
        srsConfigInfo->srsULBandwidthCollision = LTE_TRUE; 
        /*SPR 16855 fix end*/
    }
    else
    {
        /*SPR 16855 fix start*/
        srsConfigInfo->srsULBandwidthCollision = LTE_FALSE; 
        /*SPR 16855 fix end*/
    }

    if(srsUEConfigIndex <= 9)
    {  
        /*The periodicity is approx 2ms*/
        for(count = 0; count < NUM_SF_OFFSETS_FOR_SRS_UE_CONFIG_2_MS; ++count)
        {
            srsUESubFrOffFor2ms = srsTddConfigTableFor2ms[srsUEConfigIndex].
                                        subFrameOffsetArray[count];
            /*Update the valid srsUESubframeMap_g map srs offsets in 
            srsMapOffsetArray for every subframe offset retrieved from 
            srsTddConfigTableFor2ms*/
            updateUESrsMapOffFor2msUsingKsrs(srsUESubFrOffFor2ms, 
                 &ueSrsMapOffArrayFor2ms[0], &numUESrsMapOffFor2ms);    
         }
         /*The config index corresponds to 0 & 1 transmission offsets.
         Both these trans offsets map to subframe index 1 & 6. The 
         numUESrsMapOffFor2ms is set to 2 to avoid adding two nodes for
         the same UE at the same offset*/
         if(0 == srsUEConfigIndex)
         {
             numUESrsMapOffFor2ms = 2;
         }
        if(0 != (dedicatedSetupInfo_p->duration))
        {   
            /*Indefinite SRS transmissions*/
            /*SPR 16855 fix start*/
            if (MAC_FAILURE == updateSrsUESubframeMapFor2ms(ueIndex,srsConfigInfo, 
                      &ueSrsMapOffArrayFor2ms[0], numUESrsMapOffFor2ms, internalCellIndex))
                /*SPR 16855 fix end*/
            {
                LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "Error in updating the \
                        srsUESubframeMapFor2ms\n");
                LTE_MAC_UT_LOG(LOG_MINOR,SRS_MGR,"[%s] Exit\n",__func__);
                LOG_MAC_ERROR(MAC_RRC_INF,"updateSrsUESubframeMapFor2ms failed UE %u\n",ueIndex);
                return MAC_FAILURE;
            }
        }
        else
        {   /*Single SRS transmission - This node will be deleted after it is 
              processed once*/
            /*SPR 16855 fix start*/
            /*SPR 16855 fix end*/
            /*The current offset can exceed 9 when delay is added. So, it is necessary to
            take modulo*/
            currUESrsMapOff = (currSF + RRC_UE_DELAY_FACTOR)
                                    % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
            /*Set it to max gap initially*/
            minSrsMapOffGap = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME + 1;
            nearestUESrsMapOff = ueSrsMapOffArrayFor2ms[0];
            
            for(count = 0; count < numUESrsMapOffFor2ms; ++count)
            {
                /*Calculate the gap between current SF and srsMapOffset*/
                ueSrsMapOffGap = ueSrsMapOffArrayFor2ms[count] - currUESrsMapOff;
                if(ueSrsMapOffGap < 0)
                {
                    ueSrsMapOffGap += NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
                }
                else if(0 == ueSrsMapOffGap)
                {
                    /*A valid offset with gap = 0 is not considered*/
                    continue;
                }
                
                /*Update minTicksGap & nearestSrsMapOffset for the first
                  offset and for offsets whose diff is < existing minTicksGap*/
                if(ueSrsMapOffGap < minSrsMapOffGap)
                {
                    nearestUESrsMapOff = ueSrsMapOffArrayFor2ms[count];
                    minSrsMapOffGap = ueSrsMapOffGap;
                }
            }
            ueSrsMapOffArrayFor2ms[0] = nearestUESrsMapOff;
            /*SPR 16855 fix start*/
            if (MAC_FAILURE == updateSrsUESubframeMapFor2ms(ueIndex,srsConfigInfo, 
                      &ueSrsMapOffArrayFor2ms[0], numUESrsMapOffFor2ms, internalCellIndex))
                /*SPR 16855 fix end*/
            {
                LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] Error in "
                        "inserting node in srsUESubframeMap_g\n",
                        __func__);
                LTE_MAC_UT_LOG(LOG_MINOR,SRS_MGR,"[%s] Exit\n",__func__);
                LOG_MAC_ERROR(MAC_RRC_INF,"updateSrsUESubframeMapFor2ms failed UE %u\n",
                        ueIndex);
                return MAC_FAILURE;
            }
        }
        /*SPR 16855 fix start*/
        srsConfigInfo->srsPeriodicity = 2;
        srsConfigInfo->srsSubFrameOffset = ueSrsMapOffArrayFor2ms[0];
        /*SPR 16855 fix end*/
    }
/*FDD_SRS*/
    else if(srsUEConfigIndex < 645)
    {
        /*The periodicity is > 2ms for TDD*/
#elif FDD_CONFIG
    if(srsUEConfigIndex <= 16)
    {
        /*SPR 16855 fix start*/
        LTE_MAC_UT_LOG(LOG_MINOR, PUCCH_MODULE,
                "[%s]***** srsUEConfigIndex < =16 \n processSRSShortSubframeMap is called \n*****",__func__);
        if(MAC_FAILURE == processSRSShortSubframeMap(srsUEConfigIndex,
                    dedicatedSetupInfo_p->duration,ueIndex,srsConfigInfo,
                    internalCellIndex))
            /*SPR 16855 fix end*/
        {
            LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR,
                        "Error in processing of short subframe map\n");
            LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
            LOG_MAC_ERROR(MAC_RRC_INF,"processSRSShortSubframeMap failed UE %u\n",ueIndex);
            return MAC_FAILURE;

        }
        
    }
    else if((16 < srsUEConfigIndex)&&(srsUEConfigIndex < 637))
/*FDD_SRS*/
    {
            
        /*For all periodicities for FDD*/
#endif   
        if (MAC_FAILURE == getSrsPeriodicityAndOffset(srsUEConfigIndex, 
                    &srsPeriodicity, &srsStartSfOff))
        {
            LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Invalid SRS Config Index "
                    "Error getting SRS Periodicity and Offset");
            LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
            LOG_MAC_ERROR(MAC_RRC_INF,"getSrsPeriodicityAndOffset Failed UE %u\n",ueIndex);
            return MAC_FAILURE;
        }

        if(0 != (dedicatedSetupInfo_p->duration))
        {   
            /*Indefinite SRS transmissions*/
            /*SPR 16855 fix start*/
            LTE_MAC_UT_LOG(LOG_MINOR, PUCCH_MODULE,
                    "[%s]***** 16 < srsUEConfigIndex <637 AND duration != 0 \n*****",__func__);
            if(MAC_FAILURE == updateSrsUESubframeMapForNot2ms(ueIndex,
                        srsConfigInfo,srsPeriodicity, srsStartSfOff,
                        internalCellIndex))
                /*SPR 16855 fix end*/
            {
                LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Error in creating SRS Subframe "
                        "map for UE");
                LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
                LOG_MAC_ERROR(MAC_RRC_INF,"updateSrsUESubframeMapForNot2ms failed UE %u\n",ueIndex);
                return MAC_FAILURE;
            }
        }
        else
        {   
            /*Single SRS transmission - This node will be deleted after it is 
              processed once*/
            /*SPR 16855 +-*/
            /*Take delay into account while calculating the current offset in 
              the subframe map.*/
            currUESrsMapOff = ((1024 * currSFN) + (10 * currSF) + 
                        RRC_UE_DELAY_FACTOR) % srsMaxPeriodicity;
            /*Calculate the gap b/w current and start sf offsets*/
            ueSrsMapOffGap = (currUESrsMapOff - srsStartSfOff + 
                        srsMaxPeriodicity) % srsPeriodicity;
            /*Subtract the above diff and add the periodicity to current sf 
              offset. This final offset will be a valid sf offset*/        
            nearestUESrsMapOff = (currUESrsMapOff + srsPeriodicity - 
                        ueSrsMapOffGap) % srsMaxPeriodicity;
            {

                /*SPR 16855 fix start*/
                LTE_MAC_UT_LOG(LOG_MINOR, PUCCH_MODULE,
                        "[%s]***** 16 < srsUEConfigIndex <637 AND duration  = 0 \n*****",__func__);
                /* +- SPR 17777 */
                if(MAC_FAILURE == insertUESrsNodeInUESubframeMap
                        (ueIndex,&srsReportNode_p, 
                         /*CA Changes start  */
                         &(srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][nearestUESrsMapOff].srsSubFrameList)))
                    /* +- SPR 17777 */
                         /*CA Changes end  */
                    /*SPR 16855 fix end*/
                {
                    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Error in "
                            "inserting node in srsUESubframeMap_g",
                            __func__);
                    LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
                    LOG_MAC_ERROR(MAC_RRC_INF,"insertUESrsNodeInUESubframeMap failed UE %u\n",ueIndex);
                    return MAC_FAILURE;
                }
            }
            /*Only one node is added, so update the start offset with the
              nearest offset calculated*/
            srsStartSfOff = nearestUESrsMapOff;
            /*SPR 16855 fix start*/
            /*CA Changes start  */
            srsConfigInfo->srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = 
            /*CA Changes end  */
                srsReportNode_p;
            /*SPR 16855 fix end*/
        }
        /*SPR 16855 fix start*/
        srsConfigInfo->srsPeriodicity = srsPeriodicity;
        srsConfigInfo->srsSubFrameOffset = srsStartSfOff;
        /*SPR 16855 fix end*/

    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Invalid SRS Subframe offset passed. "
                "Value = %d\n", srsStartSfOff);
        LOG_MAC_ERROR(MAC_RRC_INF,"Invalid SRS Subframe offset %u UE %u\n",
                srsStartSfOff,ueIndex);
        return MAC_FAILURE;
    }

    /*The dedicated SRS config is configured successfully here. So, update
    in UE context*/
    /*SPR 16855 fix start*/
    srsConfigInfo->srsDedicatedSetupInfo = 
                                            *dedicatedSetupInfo_p;
    /*SPR 6497 Fix Start*/
    srsConfigInfo->srsConfigType = SRS_DEDICATED_CONFIG_TYPE_SETUP_WAITING;
    /*SPR 6497 Fix End*/
    /*SPR 16855 fix end*/
    LTE_MAC_UT_LOG(LOG_INFO,SRS_MGR,"[%s] Exit",__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : deleteDedicatedSRSConfig 
 * Inputs         : reconfigInfo_p -
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : None
 * Description    : This function is used to delete the UE report nodes from the
 *                  SRS subframe map.
 *****************************************************************************/
/*SPR 16855 fix start*/
/*CA Changes start  */
void deleteDedicatedSRSConfig(SRSConfigInfo *srsConfigInfo, UeContextForReconfig *reconfigInfo_p, InternalCellIndex internalCellIndex)
    /*SPR 16855 fix end*/
{
    UInt16                  srsSubFrameOffset       = 0;     
    UInt16                  srsTmpSubFrameOffset    = 0;     
    UInt16                  periodicity             = 0;
    SRSReportNode           *srsReportNode_p        = PNULL;
    SRSReportNode           *srsTmpReportNode_p     = PNULL;
    SRSUESubFrameMapList    *srsUESubframeMap_p      = PNULL;
    
    LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] Entry.\n", __func__);
    
    /*SPR 16855 fix start*/
    if((srsConfigInfo->srsConfigType) != SRS_DEDICATED_CONFIG_TYPE_SETUP)
        /*SPR 16855 fix end*/
    {   /*Invalid request received*/
        lteWarning("Error in deleting the SRS config as \
                        no SRS is configured for this UE");
        LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
        return;
    }
    /*SPR 16855 fix start*/
    if (reconfigInfo_p)
    {
	    if ((reconfigInfo_p->srsReconfigBitMap & UL_RECONFIG_DELETE_LONG_SRS_NODE) ||
			    (reconfigInfo_p->srsReconfigBitMap & UL_RECONFIG_DELETE_SHORT_SRS_NODE))
	    {
		    srsSubFrameOffset = reconfigInfo_p->srsOffset; 
		    periodicity = reconfigInfo_p->srsPeriodicity; 
	    }
	    else
	    {
		    srsSubFrameOffset = srsConfigInfo->srsSubFrameOffset;
		    periodicity = srsConfigInfo->srsPeriodicity;
		    reconfigInfo_p->srsOffset = srsSubFrameOffset;
		    reconfigInfo_p->srsPeriodicity = periodicity;
	    }
    }
    else
    {
	    srsSubFrameOffset = srsConfigInfo->srsSubFrameOffset;
	    periodicity = srsConfigInfo->srsPeriodicity;
    }
    /* SPR 9326 (Bug 542) fix start*/                                
#ifdef FDD_CONFIG 
    if ( srsConfigInfo->srsPeriodicity <= 10 )
#else
    /* SPR 9326 (Bug 542) fix end*/                                
    if ( periodicity == 2 )
#endif
    {
        /*CA Changes start  */
        srsReportNode_p = srsConfigInfo->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex];
        /*CA Changes end  */
    }
    else
    {
        /*CA Changes start  */
        srsReportNode_p = srsConfigInfo->srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex];
        /*CA Changes end  */
    }
/* SPR 9326 (Bug 542) fix start*/ 
    if ( PNULL != srsReportNodeg_p  )
    {
	srsReportNode_p = srsReportNodeg_p;
    }
 /* SPR 9326 (Bug 542) fix end*/
    /*SPR 16855 fix end*/
    
    if(PNULL != srsReportNode_p)
    {
#ifdef TDD_CONFIG
        /*SPR 16855 fix start*/
        /*CA Changes start  */
        if(periodicity == 2)
        {
            srsUESubframeMap_p = &(srsUESubframeMapFor2ms_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][0]);

        }
        else
        {
            srsUESubframeMap_p = &(srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][0]);
        }
        /*SPR 16855 fix end*/
/* SRS TDD Fix */
/*FDD_SRS*/
#elif FDD_CONFIG
        /*SPR 16855 fix start*/
        if(periodicity <= 10)
        {
            /*CA Changes start  */
            srsUESubframeMap_p = &(srsUEShortSubframeMap_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][0]);
            /*CA Changes end  */
        }
        else
        {
            /*CA Changes start  */
            srsUESubframeMap_p = &(srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex][0]);
            /*CA Changes end  */
        }
        /*SPR 16855 fix end*/

#endif
/*FDD_SRS*/
        
        while(srsReportNode_p)
        {
            srsTmpReportNode_p = srsReportNode_p->nextNode_p;
            srsTmpSubFrameOffset = srsReportNode_p->nextUESfMapOffset;
            /** SPR 15231 Changes Start **/
            if(listCount(&(srsUESubframeMap_p[srsSubFrameOffset].srsSubFrameList)))
            {
                /*SPR 16855 fix start*/
                listDeleteNode(&(srsUESubframeMap_p[srsSubFrameOffset].
                    srsSubFrameList), &(srsReportNode_p->nodeAnchor));
                /*SPR 16855 fix end*/
            }
            /** SPR 15231 Changes End **/
        
            freeMemPool((void*)srsReportNode_p);
            srsReportNode_p = srsTmpReportNode_p;
            srsSubFrameOffset = srsTmpSubFrameOffset;
        }
        /** SPR 15231 Changes Start **/
        /*SPR 16855 fix start*/
#ifdef FDD_CONFIG 
        if ( srsConfigInfo->srsPeriodicity <= 10 )
#else
        if ( periodicity == 2 )
#endif
        {
            srsConfigInfo->srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = PNULL;
        }
        else
        {
            srsConfigInfo->srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex] = PNULL;
        }
        /*SPR 16855 fix endt*/
        /** SPR 15231 Changes End **/
    }
    /*SPR 16855 +-*/
    
    LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] Exit.\n", __func__);
}

/*****************************************************************************
 * Function Name  : reconfigureDedicatedSRSConfig 
 * Inputs         : ueIndex - UE Index
 *                  ulUEContext_p - pointer to the UL UE context
 *                  dedicatedSetupInfo_p - Pointer to dedicated setup info
 *                  newConfigType - New SRS config type
 *                  currSFN,currSF
 *                  reconfigInfo_p - pointer to reconfig UE info changes
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : srsConfigInfo - pointer of SRS configuration info to be updated
 * Returns        : SUCCESS / FAILURE
 * Description    : This function is used to reconfigure dedicated SRS 
 *                  configuration for a UE
 *****************************************************************************/
/*SPR 16855 fix start*/
MacRetType reconfigureDedicatedSRSConfig(UInt16 ueIndex,SRSConfigInfo *srsConfigInfo, 
            SRSDedicatedSetupInfo   *dedicatedSetupInfo_p, UInt8 newConfigType,
            UInt32 currSFN, UInt32 currSF, UeContextForReconfig *reconfigInfo_p,
            /*CA Changes start */
            InternalCellIndex internalCellIndex)
            /*CA Changes end */
/*SPR 16855 fix end*/
{
	LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] Entry.\n", __func__);

	/* If the configType is release. Delete the dedicated SRS configuration */
	if (SRS_DEDICATED_CONFIG_TYPE_RELEASE == newConfigType) 
	{
        /*SPR 16855 fix start*/
        /*CA Changes start  */
		deleteDedicatedSRSConfig(srsConfigInfo,reconfigInfo_p, internalCellIndex);
		reconfigInfo_p->srsReconfigBitMap |= UL_RECONFIG_DELETE_LONG_SRS_NODE;
		memSet(srsConfigInfo, 0, sizeof(SRSConfigInfo));
       /*SPR 16855 fix end*/

		LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] Exit.\n", __func__);
		return MAC_SUCCESS;
	}
	else
	{ 
		/* If the configType is setup and there is no existing dedicated SRS
		 * config list. Create a new one  */
        /*SPR 16855 fix start*/
		UInt32 srsPassiveIndex =0;

#ifdef FDD_CONFIG
        if(srsConfigInfo->srsPeriodicity <= 10)
#else
        if(srsConfigInfo->srsPeriodicity == 2)
#endif
		{
			LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] srsPeriodicity < =10"
					"Dedicated SRS Map\n",__func__);
            /*CA Changes start  */
			srsPassiveIndex = shortPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
            /*CA Changes end  */
		}
		else 
		{
			LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] srsPeriodicity > 10"
					"Dedicated SRS Map\n",__func__);
            /*CA Changes start  */
			srsPassiveIndex = longPeriodicSRSMapInfo_gp[internalCellIndex]->passiveIndex;
            /*CA Changes end  */

		}
		if (PNULL == (srsConfigInfo->srsReportHeadNode_p[srsPassiveIndex]))
		{
			LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] srsReportHeadNode_p == NULL"
					"Dedicated SRS Map\n",__func__);
			if (MAC_FAILURE == configureDedicatedSRSConfig(ueIndex,srsConfigInfo, 
						dedicatedSetupInfo_p, newConfigType, currSFN, currSF,
                        internalCellIndex))
			{
				LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Error re-configuring "
						"Dedicated SRS Map");
				LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
				return MAC_FAILURE;
			}
		}
		else
		{
			/* If the configType is setup and there exists SRS dedicated config
			 * list. Check the major parameters srsConfigIndex and duration from
			 * the previous values. If these are changed we need to delete the
			 * srs dedicated config list and create a new one. */ 
			if (
					(
					 (dedicatedSetupInfo_p->srsConfigIndex) !=
					 (srsConfigInfo->srsDedicatedSetupInfo.srsConfigIndex)
					) || 
					(
					 (dedicatedSetupInfo_p->duration) != 
					 (srsConfigInfo->srsDedicatedSetupInfo.duration)
					)
			   )
            {
                /* SPR 9326 (Bug 542)fix start*/                                
                deleteDedicatedSRSConfig(srsConfigInfo,reconfigInfo_p,
                        internalCellIndex);
                if ( 2 == srsConfigInfo->srsPeriodicity   ) 
                {
                    reconfigInfo_p->srsReconfigBitMap |= UL_RECONFIG_DELETE_SHORT_SRS_NODE;
                    srsReportNodeg_p = srsConfigInfo-> \
                    srsReportHeadNode_p[shortPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex];

                }
                else 
                {
                    reconfigInfo_p->srsReconfigBitMap |= UL_RECONFIG_DELETE_LONG_SRS_NODE;
                    srsReportNodeg_p = srsConfigInfo-> \
                    srsReportHeadNode_p[longPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex];
                }
                /* SPR 9326 (Bug 542)fix end*/                                
                if (MAC_FAILURE == configureDedicatedSRSConfig(ueIndex, 
                    srsConfigInfo, dedicatedSetupInfo_p, newConfigType, 
                    currSFN, currSF,internalCellIndex))
                {
                    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "Error re-configuring "
                            "Dedicated SRS Map");
                    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
                    return MAC_FAILURE;
                }
            }
			else
			{
				/* Or else just update the new parameters received */
				srsConfigInfo->srsConfigType = newConfigType;
				srsConfigInfo->srsDedicatedSetupInfo = 
					*dedicatedSetupInfo_p;
				return MAC_PARTIAL_SUCCESS;
			}
		}

    /*SPR 16855 fix end*/
	}
	LTE_MAC_UT_LOG(LOG_MINOR, SRS_MGR, "[%s] Exit.\n", __func__);
	return MAC_SUCCESS; 
}

/*FDD_SRS*/

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processSrsReports 
 * Inputs         : currSubFrameNum - 
 *                  ulDelay -
 *                  globalTick - global tick in MAC    
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : Void
 * Description    : This function is used to handle SRS transmissions in every 
 *                  Subframes and handle collisions
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void processSrsReports(UInt32 currSubFrameNum, UInt8 ulDelay, tickType_t globalTick, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */    
/*CA Changes end  */
{
    UInt8               ulSubFrameNum            = 0;
    UInt32              ueSrsMapOffForNot2ms     = 0;
    UInt32              ueSrsMapOffForShortMap   = 0;     
    UInt32              containerTick            = 0;
    ULSchUciRachPoolInfo *ulschUciRachInfo_p     = PNULL;
    PdcchULOutputInfo    *tempPdcchULOutput_p    = PNULL;
    /* MEAS_GAP_CHG */
    /* SPR 15909 fix start */
    tickType_t          ttiWithDelay = (globalTick + 4 + ulDelay);
    /* SPR 15909 fix end */
    /* MEAS_GAP_CHG */
    
    /*Calculate subframe offsets in the FDD SRS map*/

    ueSrsMapOffForNot2ms = (globalTick + ulDelay + 4) % MAX_SRS_PERIODICITY; 
    ulSubFrameNum = (currSubFrameNum + ulDelay + 4) % 
        NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    ueSrsMapOffForShortMap = ulSubFrameNum;


    /*Find the ulschUciRachInfo_p and pass the same pointer to the map 
    below*/
    containerTick = (globalTick + ulDelay) % MAX_PDCCH_CONTAINER;
    /*CA Changes start  */
    ulschUciRachInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    ulschUciRachInfo_p->numSRSPdus = 0;
    
    /* Update the SRS present flag from the srsCellConfigMap_g */
    if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + ulSubFrameNum))
    {
        ulschUciRachInfo_p->isSRSPresent = LTE_TRUE;
    }
    else
    {
        ulschUciRachInfo_p->isSRSPresent = LTE_FALSE;
    }
    
    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */

    /*process the SRS report nodes in srsUESubframeMapForNot2ms_g map
    and srsUEShortSubframeMap_g(for less than 10 periodicity)
    for subframes */
    /*SPR 16855 fix start*/
    processSRSReportNodesForUSubframes(&srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex][0],
                    ueSrsMapOffForNot2ms, ulschUciRachInfo_p, 
                    tempPdcchULOutput_p, ttiWithDelay,
                    /*CA Changes start  */
                    internalCellIndex);                                        
                    /*CA Changes end  */
    
    /*CA Changes start  */
    processSRSReportNodesForUSubframes(&srsUEShortSubframeMap_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex][0],
                    ueSrsMapOffForShortMap, ulschUciRachInfo_p, 
                    tempPdcchULOutput_p, ttiWithDelay,
                    internalCellIndex);                                        
    /*CA Changes end  */
    /*SPR 16855 fix end*/
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return;

}
#endif
/*FDD_SRS*/

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : processSrsForUplinkSubframes 
 * Inputs         : ulSubFrameNum - UL subframe number for which SRS transmissions
 *                  will be calculated
 *                  globalTick - global tick in MAC    
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : Void
 * Description    : This function is used to handle SRS transmissions in U
 *                  Subframes and handle collisions
 *****************************************************************************/
/* SPR 15909 fix start */
void processSrsForUplinkSubframes(UInt8 ulSubFrameNum, tickType_t globalTick, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
{
    UInt32              ueSrsMapOffForNot2ms     = 0;
    UInt32              ueSrsMapOffFor2ms        = 0;
    UInt8               deltaSubFrameCount       = 0;
    UInt32              containerTick            = 0;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p        = PNULL;
    UInt32 currSubFrameNum = globalTick % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    /* MEAS_GAP_CHG */
/*  UInt32 ulTTI = 0; TDD Warning Fix */
    /* SPR 15909 fix start */
    tickType_t globalAdvancedTick = 0;
    /* SPR 15909 fix end */
    /* MEAS_GAP_CHG */
    
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);
    
    /* The SRS transmissions will be processed for subframe calculated as 
     * below: */
    if (ulSubFrameNum >= currSubFrameNum)
    {
        deltaSubFrameCount = ulSubFrameNum - currSubFrameNum;
    }
    else
    {
        deltaSubFrameCount = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - 
            currSubFrameNum + ulSubFrameNum;
    }
    
    /* MEAS_GAP_CHG */
    /* SPR 14094 CHANGES START */
    /* globalAdvancedTick = globalTick + ulSubFrameNum; */
    globalAdvancedTick = globalTick + deltaSubFrameCount;
    /* SPR 14094 CHANGES END */

/*  ulTTI = ulSysFrameNum * MAX_SUBFRAME + ulSubFrameNum; TDD Warning Fix */
    /* MEAS_GAP_CHG */
    
    /*Calculate subframe offsets in both maps respectively*/
    ueSrsMapOffForNot2ms = (globalTick + deltaSubFrameCount) % MAX_SRS_PERIODICITY; 
    ueSrsMapOffFor2ms = (globalTick + deltaSubFrameCount) % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    
    /*Find the ulschUciRachInfo_p and pass the same pointer to both the maps 
    below*/
	/* TDD Config 0 Changes Start */
	containerTick = ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER;
	/* TDD Config 0 Changes End */
    /*CA Changes start  */
    ulschUciRachInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    ulschUciRachInfo_p->numSRSPdus = 0;
    
    /* Update the SRS present flag from the srsCellConfigMap_g */
    if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + ulSubFrameNum))
    {
        ulschUciRachInfo_p->isSRSPresent = LTE_TRUE;
    }
    else
    {
        ulschUciRachInfo_p->isSRSPresent = LTE_FALSE;
    }
    
    /*process the SRS report nodes in srsUESubframeMapForNot2ms_g map
    for U subframes*/
    /*SPR 16855 fix start*/
    /*CA Changes start  */
    processSRSReportNodesForUSubframes(&srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex][0],
                    ueSrsMapOffForNot2ms,
                    /* TDD Config 0 Changes Start */
                    ulSubFrameNum, 
                    /* TDD Config 0 Changes End */
                    ulschUciRachInfo_p,
                    globalAdvancedTick,
                    internalCellIndex
                    );
    
    processSRSReportNodesForUSubframes(&srsUESubframeMapFor2ms_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex][0],
                    ueSrsMapOffFor2ms,
                    /* TDD Config 0 Changes Start */
			        ulSubFrameNum, 
                    /* TDD Config 0 Changes End */
                    ulschUciRachInfo_p,
                    globalAdvancedTick,
                    internalCellIndex);
    /*SPR 16855 fix end*/ 
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return;
}

/*****************************************************************************
 * Function Name  : processSrsForSpecialSubframes 
 * Inputs         : ulSubFrameNum - UL subframe number for which SRS transmissions
 *                  will be calculated
 *                  globalTick - global tick in MAC    
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : Void
 * Description    : This function is used to handle SRS transmissions in S
 *                  Subframes and handle collisions
 *****************************************************************************/
/* SPR 15909 fix start */
void processSrsForSpecialSubframes(UInt8 ulSubFrameNum, tickType_t globalTick, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */    
{
    UInt32                  ueSrsMapOffForNot2ms     = 0;
    UInt32                  ueSrsMapOffFor2ms        = 0;
    UInt8                   deltaSubFrameCount       = 0;
    UInt32                  containerTick                   = 0;
    ULSchUciRachPoolInfo*   ulschUciRachInfo_p            = PNULL;
    UInt32 currSubFrameNum = globalTick % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    /* SPR 15909 fix start */
    tickType_t globalAdvancedTick = 0;
    /* SPR 15909 fix end */
    
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);
    
    /* The SRS transmissions will be processed for subframe calculated as 
     * below: */
    if (ulSubFrameNum >= currSubFrameNum)
    {
        deltaSubFrameCount = ulSubFrameNum - currSubFrameNum;
    }
    else
    {
        deltaSubFrameCount = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - 
            currSubFrameNum + ulSubFrameNum;
    }
   
    /* SPR 14094 CHANGES START */
    /* globalAdvancedTick = ulSubFrameNum +globalTick; */
    globalAdvancedTick = deltaSubFrameCount +globalTick;
    /* SPR 14094 CHANGES END */

    /*Calculate subframe offsets in both maps respectively*/
    ueSrsMapOffForNot2ms = (globalTick + deltaSubFrameCount) % MAX_SRS_PERIODICITY; 
    ueSrsMapOffFor2ms = (globalTick + deltaSubFrameCount) % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    
    /*Find the ulschUciRachInfo_p and pass the same pointer to both the maps 
      below*/
    /* TDD Config 0 Changes Start */
    containerTick = ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER;
    /* TDD Config 0 Changes End */
    /*CA Changes start  */
    ulschUciRachInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    ulschUciRachInfo_p->numSRSPdus = 0;
    /* Update the SRS present flag from the srsCellConfigMap_g */
    /* If PRACH resources are allocated for special subframe for preamble
     * format 4, SRS transmission need not be transmitted */
    if( ( LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + 
                    ulSubFrameNum) ) &&
            ( 0 == ulschUciRachInfo_p->isPrachResourceReserved ) )
    {
        ulschUciRachInfo_p->isSRSPresent = LTE_TRUE;
    }
    else
    {
        ulschUciRachInfo_p->isSRSPresent = LTE_FALSE;
    }

    /*process the SRS report nodes in srsUESubframeMapForNot2ms_g map
    for S subframes*/
    /*SPR 16855 fix start*/
    processSRSReportNodesForSSubframes(&srsUESubframeMapForNot2ms_g[internalCellIndex][longPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex][0],
                        ueSrsMapOffForNot2ms,
                        /* TDD Config 0 Changes Start */
                        /* This parameters were not used inside
                         * the function . 
                        ulSubFrameNum, currSubFrameNum, */
                        /* TDD Config 0 Changes End */
                        ulschUciRachInfo_p,
                        globalAdvancedTick,
                        internalCellIndex); /* MEAS_GAP_CHG */
    
    processSRSReportNodesForSSubframes(&srsUESubframeMapFor2ms_g[internalCellIndex][shortPeriodicSRSMapInfo_gp[internalCellIndex]->activeIndex][0],
                        ueSrsMapOffFor2ms,
                        /* TDD Config 0 Changes Start */
                        /* This parameters were not used inside
                         * the function . 
                        ulSubFrameNum, currSubFrameNum, */
                        /* TDD Config 0 Changes End */
                        ulschUciRachInfo_p,
                        globalAdvancedTick,
                        internalCellIndex); /* MEAS_GAP_CHG */
    /*SPR 16855 fix end*/
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
}
#endif

/*****************************************************************************
 * Function Name  : initSRSMgr 
 * Inputs         : InternalCellIndex internalCellIndex
 * Outputs        : None 
 * Returns        : Void
 * Description    : This function takes the memory for the nodes at the time of cell config
 *****************************************************************************/
void initSRSMgr(UInt8 srsConfigType, InternalCellIndex internalCellIndex)
{
    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(UInt8) * NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME, 1);
    }
    GET_MEM_FROM_POOL(UInt8, srsCellConfigMap_gp[internalCellIndex],sizeof(UInt8) * NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME, PNULL);
    memSet(srsCellConfigMap_gp[internalCellIndex], 0, sizeof(UInt8) * NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME);

    if(SRS_COMMON_CONFIG_TYPE_SETUP == srsConfigType)
    {
        initCellSpecificSRSConfig(internalCellIndex);
    }
    return;
}

/*****************************************************************************
 * Function Name  : cleanupSRSMgr 
 * Inputs         : InternalCellIndex for which the cleanup is performed
 * Outputs        : None 
 * Returns        : Void
 * Description    : This function will remove the nodes and its memory from
 *                  SRS subframe maps. This is called at cell stop
 *****************************************************************************/
void cleanupSRSMgr(InternalCellIndex internalCellIndex)
{
    UInt32          count              = 0;
    UInt16          ueSrsMapOff        = 0;
    UInt16          index   =0;
    LTE_LIST        *list_p            = PNULL;
    LTE_LIST_NODE   *listNode_p        = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Entry.", __func__);

    /*SPR 16855 fix start*/
    for(index = 0 ; index<MAX_NUM_MAPS ; index++ )
    {
        for (ueSrsMapOff = 0; ueSrsMapOff < 
                NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME; ++ueSrsMapOff)    
        {
#ifdef TDD_CONFIG
            list_p = &(srsUESubframeMapFor2ms_g[internalCellIndex][index][ueSrsMapOff].srsSubFrameList);
#else
            list_p = &(srsUEShortSubframeMap_g[internalCellIndex][index][ueSrsMapOff].srsSubFrameList);
#endif
            count = listCount(list_p);
            while(count) {
                listNode_p = getListNode(list_p, 0);
                if(listNode_p) {
                    listDeleteNode(list_p, listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }   
            listInit(list_p);
        }

        for(ueSrsMapOff = 0; ueSrsMapOff <
                MAX_SRS_PERIODICITY; ++ueSrsMapOff)
        {
            list_p = &(srsUESubframeMapForNot2ms_g[internalCellIndex][index][ueSrsMapOff].srsSubFrameList);
            count = listCount(list_p);
            while(count) {
                listNode_p = getListNode(list_p, 0);
                if(listNode_p) {
                    listDeleteNode(list_p, listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }   
            listInit(list_p);
        }
    }

    /*SPR 16855 fix end*/
    LTE_MAC_UT_LOG(LOG_INFO, SRS_MGR, "[%s] Exit.", __func__);
    return;
}

/*****************************************************************************
 * Function Name  : deInitSRSMgr 
 * Inputs         : internalCellIndex - Cell-Index used at MAC
 * Outputs        : None 
 * Returns        : Void
 * Description    : This function frees the memory for the nodes at the time of cell delete
 *****************************************************************************/
void deInitSRSMgr(InternalCellIndex internalCellIndex)
{
    if (srsCellConfigMap_gp[internalCellIndex])
    {
        freeMemPool(srsCellConfigMap_gp[internalCellIndex]);
        srsCellConfigMap_gp[internalCellIndex] = PNULL;
    }
    return;
}
/* CA Changes end */

/* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG

/*****************************************************************************
 * Function Name  : updateHDFddSemiStaticMapSRS 
 * Inputs         : 1.  UE-Index / UE Context
 *                     2. SRS Periodicty
 *                     3. SRS offset - informing subframe which will have SRS transmission
 *                     4. oneShot - if SRS is transmitted indefinite or only once by UE
 *                     5. setupRel -
 *
 * Outputs        : Updates the SRS bitmap in HD Table
 * Returns        : None
 * Description    : This function handles the updation of SRS map
 *
*****************************************************************************/
void updateHDFddSemiStaticMapSRS(
    /*SPR 16855 fix start*/
    UInt16 ueIndex,
    /*SPR 16855 fix end*/
    UInt16 srsPeriodicity, /* SRS peridicity */
    /* +- SPR 17777 */
    UInt8 oneShot, /* Whether infinite (non-zero) or one shot (0) SRS from UE */
    UInt8 setupRel) /* Whether SRS is setup or released */
{

      /*SPR 16855 +-*/
      /* if request for deletion of SRS, remove all nodes from the static map */
      if (SRS_DEDICATED_CONFIG_TYPE_RELEASE == setupRel)
      {
          /* Reset all the SRS parameters in static map */
          HD_UE_RESET_REPORT_INFO (ueIndex, HD_UE_STATIC_MAP_SRS);

          return;
      }

      /* Check if the SRS periodicity is less than 4 and add a log that it may impact the HD UE 
       * throughput as most of the subframes will be marked as UL. This should be checked if
       * SRS is periodic and not one shot */
      if((oneShot) && (srsPeriodicity <= HD_UE_MIN_REPORT_PERIODICITY))
      {
            LOG_MAC_MSG(MAC_HD_FDD_MIN_REPORTING, LOGWARNING, MAC_RRC_INF,\
                    getCurrentTick(), \
                    ueIndex,srsPeriodicity,\
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                    FUNCTION_NAME,"SRS_PERIODICITY");
      }

      /* Update the oneShot/indefinite in HD database */
      /* UE Index check not added because it is expected that UE should be in HD database */
      hdUeContextInfo_g.hdUeReportingInfo[ueIndex][HD_UE_STATIC_MAP_SRS].oneShot = oneShot;

      /* Update SRS bitmap in semi-static map */
      updateHDFddSemiStaticMap (
              /*SPR 16855 fix start*/
                ueIndex,
                /*SPR 16855 fix end*/
                HD_UE_STATIC_MAP_SRS);

    return;
}

#endif
/*****************************************************************************
 * Function Name  : reconfigureSRSPassiveContext 
 * Inputs         : ueIndex -
 *                  tempsrsConfigInfo_p -
 *                  uplkUEInfo_p -
 *                  currSFN,currSF,
 *                  reconfigInfo_p -
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : SUCCESS / FAILURE
 * Description    : This function is used to reconfigure dedicated SRS 
 *                  configuration for a UE In passive context after getting 
 *                  response from HP thread to LP thread after reconfiguration of 
 *                  active context
 *****************************************************************************/
 MacRetType reconfigureSRSPassiveContext(
        UInt16               ueIndex,
        SRSConfigInfo        *tempsrsConfigInfo_p, 
        UeContextForUplink   *uplkUEInfo_p,
        UInt32               currSFN, 
        UInt32               currSF, 
        UeContextForReconfig *reconfigInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */

{
    /* + coverity 54494 */
    MacRetType reconfigSRSRetValue = MAC_FAILURE;
    /* - coverity 54494 */

    if((reconfigInfo_p->srsReconfigBitMap & UL_RECONFIG_DELETE_SHORT_SRS_NODE) ||
            (reconfigInfo_p->srsReconfigBitMap & UL_RECONFIG_DELETE_LONG_SRS_NODE))
    {
        /*CA Changes start  */
        deleteDedicatedSRSConfig(tempsrsConfigInfo_p,reconfigInfo_p,internalCellIndex);
        /*CA Changes end  */
        /* SPR 9326 (Bug 542) fix start */
        srsReportNodeg_p = PNULL;
        /* SPR 9326 (Bug 542) fix end */
    }
    if((reconfigInfo_p->srsReconfigBitMap & UL_RECONFIG_ADD_SHORT_SRS_NODE ) ||
            (reconfigInfo_p->srsReconfigBitMap & UL_RECONFIG_ADD_LONG_SRS_NODE) )
    {
        reconfigSRSRetValue = configureDedicatedSRSConfig(ueIndex,tempsrsConfigInfo_p,
                &uplkUEInfo_p->srsDedicatedSetupInfo, uplkUEInfo_p->srsConfigType,
                currSFN, currSF, internalCellIndex);

    }
    
    return reconfigSRSRetValue;
}
