/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: 
 *
 ******************************************************************************
 *
 *  File Description : This file defines the functionality for calculating
 *                    
 *                    
 *
 ******************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteTree.h"
#include "lteTypes.h"
#ifdef TDD_CONFIG
#include "lteMacTypes.h"
#include "lteMacPHICH.h"
#include "lteMacPUSCH.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacTDDConfig.h"
#include "lteMacCellConfMgr.h"
#include "lteMacReceiver.h"
#include "lteMacTddMgr.h"
/*TDD Config 0 Changes Start*/
#include "lteMacULResourceInfoMgr.h"
extern ULResourceAllocationInfo ulRAInfo_g[2];
/* This index specifies the updated RRM information */
extern UInt8  ulRAInfoIndex_g[MAX_NUM_CELL];
/*TDD Config 0 Changes End*/

void GetMem(void);

/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
  Private Definitions
 *****************************************************************************/
 /*CA TDD CHANGES START*/
ULHarqTimerEntityList ulHarqTimerEntityList_g[MAX_NUM_CELL][MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
extern  ULHarqTimerEntityForMsg3List ulHarqTimerEntityForMsg3List_g[MAX_NUM_CELL][MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
/* CA TDD Changes End */
/* msg3_retx_tdd_support_end */
/******************************************************************************
  Private Types
 *****************************************************************************/
 /* DL -> represents " Downlink Frame "
    UL -> represents " Uplink Frame "
    SP -> represents " Special Subframe "
    NA -> represents "  Not Applicable for this subframe"

    { SubFrame UL or DL or SP , Union ( if DL send DCI-0 for which U subFrame,
       if UL for HI pdu subFrameNum, AdvancedRBMap Num, Prevhistory)
*/
  
/* TDD Config 0 6 Changes Start */

/*
** This Map is used to get Harq ID for Config 6 corresponds to
** UL subframe Number. In Config 6, the same Harq ID will be allocated
** on same UL subframe Number after 60 subframe.
** For e.g: Harq ID 0 is first allocated to UL subframe 7 and
** Harq ID 0 will be allocated to DL subframe number 67.
*/
STATIC UInt8 ulHarqMapTddConfig6_g[] =
{ /*  D   S   U   U   U   D   S   U   U   D  */
  /* 00, 01, 02, 03, 04, 05, 06, 07, 08, 09, */
     NA,  NA, 3,  4,  5, NA, NA,  0,  1, NA,
  /* 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, */
     NA,  NA, 2,  3,  4, NA, NA,  5,  0, NA,
  /* 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, */
     NA,  NA, 1,  2,  3, NA, NA,  4,  5, NA,
  /* 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, */
     NA,  NA, 0,  1,  2, NA, NA,  3,  4, NA,
  /* 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, */
     NA,  NA, 5,  0,  1, NA, NA,  2,  3, NA,
  /* 50, 51, 52, 53, 54, 55, 56, 57, 58, 59  */
     NA,  NA, 4,  5,  0, NA, NA,  1,  2, NA
};
/*
** This Map is used to get Harq ID for Config 0 corresponds to
** UL subframe Number. In Config 7, the same Harq ID will be allocated
** on same UL subframe Number after 70 subframe.
** For e.g: Harq ID 0 is first allocated to UL subframe 7 and
** Harq ID 0 will be allocated to DL subframe number 77.
*/
STATIC UInt8 ulHarqMapTddConfig0_g[] =
{ /*  D   S   U   U   U   D   S   U   U   U  */
  /* 00, 01, 02, 03, 04, 05, 06, 07, 08, 09, */
     NA,  NA, 5,  6,  0, NA, NA,  1,  2, 3,
  /* 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, */
     NA,  NA, 4,  5,  6, NA, NA,  0,  1, 2,
  /* 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, */
     NA,  NA, 3,  4,  5, NA, NA,  6,  0, 1,
  /* 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, */
     NA,  NA, 2,  3,  4, NA, NA,  5,  6, 0,
  /* 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, */
     NA,  NA, 1,  2,  3, NA, NA,  4,  5, 6,
  /* 50, 51, 52, 53, 54, 55, 56, 57, 58, 59  */
     NA,  NA, 0,  1,  2, NA, NA,  3,  4, 5,
  /* 60, 61, 62, 63, 64, 65, 66, 67, 68, 69  */
     NA,  NA, 6,  0,  1, NA, NA,  2,  3, 4
};

/*   Refer TS: 36213 Section 8.3: UE ACK/NACK Procedure Table 8.3-1.
______________________________________________________________________________
| TDD UL/DL                                                                  |
| Configuration           DL subframe number i                               |
|                  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |                 |
-----------------------------------------------------------------------------
|   0              | 7 | 4 |   |   |   | 7 | 4 |   |   |   |                 |
|   1              |   | 4 |   |   | 6 |   | 4 |   |   | 6 |                 |
|   2              |   |   |   | 6 |   |   |   |   | 6 |   |                 |
|   3              | 6 |   |   |   |   |   |   |   | 6 | 6 |                 |
|   4              |   |   |   |   |   |   |   |   | 6 | 6 |                 |
|   5              |   |   |   |   |   |   |   |   | 6 |   |                 |
|   6              | 6 | 4 |   |   |   | 7 | 4 |   |   | 6 |                 |
|__________________|___|___|___|___|___|___|___|___|___|___|_________________|
*/

/* This macro is used find out UL subframe number from PHICH*/
STATIC UInt8 ulSfNumFrmPhich_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME]=
{
    /* Config 0 */
    /* 0(D) 1(S) 2(U) 3(U) 4(U) 5(D) 6(S) 7(U) 8(U) 9(U) */
    {  7,   4,   NA,  NA,  NA,  7,   4,   NA,  NA,  NA  },
    /* Config 1 */
    /* 0(D) 1(S) 2(U) 3(U) 4(D) 5(D) 6(S) 7(U) 8(U) 9(D) */
    {  NA,  4,   NA,  NA,  6,   NA,  4,   NA,  NA,  6   },
    /* Config 2 */
    /* 0(D) 1(S) 2(U) 3(D) 4(D) 5(D) 6(S) 7(U) 8(D) 9(D) */
    {  NA,  NA,  NA,  6,   NA,  NA,  NA,  NA,  6,   NA  },
    /* Config 3 */
    /* 0(D) 1(S) 2(U) 3(U) 4(U) 5(D) 6(S) 7(D) 8(D) 9(D) */
    {  6,   NA,  NA,  NA,  NA,  NA,  NA,  NA,  6,   6   },
    /* Config 4 */
    /* 0(D) 1(S) 2(U) 3(U) 4(D) 5(D) 6(S) 7(D) 8(D) 9(D) */
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  6,   6   },
    /* Config 5 */
    /* 0(D) 1(S) 2(U) 3(D) 4(D) 5(D) 6(S) 7(D) 8(D) 9(D) */
    {  NA,  NA,  NA,  NA,  NA,  NA,  NA,  NA,  6,   NA  },
    /* Config 6 */
    /* 0(D) 1(S) 2(U) 3(U) 4(U) 5(D) 6(S) 7(U) 8(U) 9(D) */
    {  6,   4,   NA,  NA,  NA,  7,   4,   NA,  NA,  6   }
};

/* TDD Config 6 Changes End */

/* TDD Config 0 Changes Start */

/************************************************************************************* 
 * Table contain the mapping from the current PUSCH UL Subframe to the next PUSCH UL
 * Subframe offset and the previous UL subframe offset coresponding to the current UL subframe.
 * 1st element represents the Previous PUSCH UL Subframe Offset
 * 2nd element represents the Next PUSCH UL Subframe Offset
 * Example: let say the DCI0 tick   = 1
 *                      UL Sf tick  = 8
 *                      Hi tick     = 15
 *                      UL Retx tick= 19
 *
 *       So here UL SF 8 is map to next offset 11 means 8+11 = 19th tick.
 *************************************************************************************/
STATIC PuschTransmissionSf ulSubFrameForPuschTransmission_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME]=
{
    /* Config 0 */
    /* 0(D)    1(S)    2(U)    3(U)    4(U)    5(D)    6(S)    7(U)    8(U)    9(U) */
    {  {NA,NA},{NA,NA},{13,11},{11,11},{11,13},{NA,NA},{NA,NA},{13,11},{11,11},{11,13} },
    /* Config 1 */
    /* 0(D)    1(S)    2(U)    3(U)    4(D)    5(D)    6(S)    7(U)    8(U)    9(D) */
    {  {NA,NA},{NA,NA},{10,10},{10,10},{NA,NA},{NA,NA},{NA,NA},{10,10},{10,10},{NA,NA} },
    /* Config 2 */
    /* 0(D)    1(S)    2(U)    3(D)    4(D)    5(D)    6(S)    7(U)    8(D)    9(D) */
    {  {NA,NA},{NA,NA},{10,10},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{10,10},{NA,NA},{NA,NA} },
    /* Config 3 */
    /* 0(D)    1(S)    2(U)    3(U)    4(U)    5(D)    6(S)    7(D)    8(D)    9(D) */
    {  {NA,NA},{NA,NA},{10,10},{10,10},{10,10},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{NA,NA} },
    /* Config 4 */
    /* 0(D)    1(S)    2(U)    3(U)    4(D)    5(D)    6(S)    7(D)    8(D)    9(D) */
    {  {NA,NA},{NA,NA},{10,10},{10,10},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{NA,NA} },
    /* Config 5 */
    /* 0(D)    1(S)    2(U)    3(D)    4(D)    5(D)    6(S)    7(D)    8(D)    9(D) */
    {  {NA,NA},{NA,NA},{10,10},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{NA,NA},{NA,NA} },
    /* Config 6 */
    /* 0(D)    1(S)    2(U)    3(U)    4(U)    5(D)    6(S)    7(U)    8(U)    9(D) */
    {  {NA,NA},{NA,NA},{14,11},{11,11},{11,13},{NA,NA},{NA,NA},{13,11},{11,14},{NA,NA} }
};
/************************************************************************************* 
 * Table contain the mapping from the PUSCH UL Subframe to the offset  
 * from DCI0 DL Subframe(Coresponding to UL SF) to HI DL Subframe.
 *************************************************************************************/
STATIC UInt8 ulSubframeToRTTMap_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME] =
{
    /* Config 0 */
    /* 0(D) 1(S) 2(U) 3(U) 4(U) 5(D) 6(S) 7(U) 8(U) 9(U) */
    {  NA,   NA,  10,  14,  10,  NA,  NA,  10, 14, 10  },
    /* Config 1 */
    /* 0(D) 1(S) 2(U) 3(U) 4(D) 5(D) 6(S) 7(U) 8(U) 9(D) */
    {  NA,  NA,   10,  10,  NA,  NA,  NA,   10,  10, NA  },
    /* Config 2 */
    /* 0(D) 1(S) 2(U) 3(D) 4(D) 5(D) 6(S) 7(U) 8(D) 9(D) */
    {  NA,  NA,   10,  NA,   NA,  NA,  NA, 10,  NA,   NA  },
    /* Config 3 */
    /* 0(D) 1(S) 2(U) 3(U) 4(U) 5(D) 6(S) 7(D) 8(D) 9(D) */
    {   NA,  NA,  10,  10,  10,  NA,  NA,  NA,  NA,   NA   },
    /* Config 4 */
    /* 0(D) 1(S) 2(U) 3(U) 4(D) 5(D) 6(S) 7(D) 8(D) 9(D) */
    {  NA,  NA,  10,  10,  NA,  NA,  NA,  NA,  NA,   NA   },
    /* Config 5 */
    /* 0(D) 1(S) 2(U) 3(D) 4(D) 5(D) 6(S) 7(D) 8(D) 9(D) */
    {  NA,  NA,   10,  NA,  NA,  NA,  NA,  NA,  NA,   NA  },
    /* Config 6 */
    /* 0(D) 1(S) 2(U) 3(U) 4(U) 5(D) 6(S) 7(U) 8(U) 9(D) */
    {   NA,  NA,  11,  13,  11,  NA,  NA,  11,  14,  NA   }
  
};
/* TDD Config 0 Changes End */
  
/* Refer Section 36211 Section 4.2 Frame Structure type 2

          Uplink-downlink configurations.
_____________________________________________________________________________
|Uplink-downlink  Downlink-to-Uplink           Subframe number               |
|configuration    Switch-point                                               |
                   periodicity         0   1   2   3   4   5   6   7   8   9 |
|____________________________________________________________________________|
|   0               5 ms               D   S   U   U   U   D   S   U   U   U |
|   1               5 ms               D   S   U   U   D   D   S   U   U   D |
|   2               5 ms               D   S   U   D   D   D   S   U   D   D |
|   3              10 ms               D   S   U   U   U   D   D   D   D   D |
|   4              10 ms               D   S   U   U   D   D   D   D   D   D |
|   5              10 ms               D   S   U   D   D   D   D   D   D   D |
|   6               5 ms               D   S   U   U   U   D   S   U   U   D |
|____________________________________________________________________________|


   Refer TS: 36213 Section 8.3 Table 8-2.

______________________________________________________________________________
| TDD UL/DL                                                                  |
| Configuration           DL subframe number n                               |  
|                  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |                 |
-----------------------------------------------------------------------------
|   0              | 4 | 6 |   |   |   | 4 | 6 |   |   |   |                 |
|   1              |   | 6 |   |   | 4 |   | 6 |   |   | 4 |                 |
|   2              |   |   |   | 4 |   |   |   |   | 4 |   |                 | 
|   3              | 4 |   |   |   |   |   |   |   | 4 | 4 |                 | 
|   4              |   |   |   |   |   |   |   |   | 4 | 4 |                 |
|   5              |   |   |   |   |   |   |   |   | 4 |   |                 |
|   6              | 7 | 7 |   |   |   | 7 | 7 |   |   | 5 |                 |
|__________________|___|___|___|___|___|___|___|___|___|___|_________________|


Refer TS: 36213 Section 9.1.2 PHICH Assignment Procedure (Table 9.1.2-1)
______________________________________________________________________________
|                                                                            |
| TDD UL/DL                                                                  |
| Configuration               UL subframe index n                            |
|              | 0 | 1 |  2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |                    |
|----------------------------------------------------------------------------|
|  0           |   |   |  4 | 7 | 6 |   |   | 4 | 7 | 6 |                    |
|  1           |   |   |  4 | 6 |   |   |   |   | 4 | 6 |                    |  
|  2           |   |   |  6 |   |   |   |   |   | 6 |   |                    |
|  3           |   |   |  6 | 6 | 6 |   |   |   |   |   |                    |
|  4           |   |   |  6 | 6 |   |   |   |   |   |   |                    |
|  5           |   |   |  6 |   |   |   |   |   |   |   |                    | 
|  6           |   |   |  4 | 6 | 6 |   |   |   | 4 | 7 |                    |
|____________________________________________________________________________|

Refer TS: 36213 Section 8 Physical uplink shared channel related procedures
and Section 7 Physical downlink shared channel related procedures.
-----------------------------------------------------------------------------
|           Number of synchronous UL/DL HARQ processes for TDD               |  
|                                                                            |
|   TDD UL/DL configuration     Number of HARQ processes                     |
|                                 for normal HARQ operation                  |
____________________________________UL_____DL_________________________________
|        0                          7       4                                |
|        1                          4       7                                |
|        2                          2      10                                |
|        3                          3       9                                |
|        4                          2      12                                |
|        5                          1      15                                |
|        6                          6       6                                |
|----------------------------------------------------------------------------|
*/
  
STATIC UlGrantHIPduSubFrameInfo 
       ulGrantPhichTableInfo_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME] =
{     
    /* UL/DL Configuration 0 */
    /*TDD Config 0 Changes Start*/ 
    /*                 0(DL)                        1(SP)                 2(UL)    */            
    { {DL, {4}, {0}, {0, 0, 0} }, {SP, {6}, {0}, {0, 0, 0} }, {UL, {4}, {0}, {0, 0, 0} },
        /*           3(UL)                        4(UL)                 5(DL)    */ 
        {UL, {7}, {0}, {0, 0, 0} }, {UL, {6}, {0}, {0, 0, 0} }, {DL, {4}, {0}, {0, 0, 0} },
        /*           6(SP)                        7(UL)                  8(UL)    */  
        {SP, {6}, {0}, {0, 0, 0} }, {UL, {4}, {0}, {0, 0, 0} }, {UL, {7}, {0}, {0, 0, 0} },
        /*          9(UL)      */
        {UL, {6}, {0}, {0, 0, 0} } 
    },
    /*TDD Config 0 Changes End*/ 

    /* UL/DL Configuration 1(DCI-0 (1,4,6,9))

                  0(DL)                        1(SP)                          2(UL)    */            
    { {DL, {NA}, {NA}, {0, 0, 0} }, {SP, {6}, {16}, {0, 0, 0} }, {UL, {4}, {10}, {6, 6, 16} },
        /*        3(UL)                        4(DL)                          5(DL)    */ 
        {UL,  {6}, {10}, {4, 9, 14} }, {DL, {4}, {14}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} },
        /*        6(SP)                        7(UL)                          8(UL)    */  
        {SP,  {6}, {16}, {0, 0, 0} }, {UL, {4}, {10}, {6, 1, 16} }, {UL, {6}, {10}, {4, 4, 14} },
        /*        9(DL)      */
        {DL,  {4}, {14}, {0, 0, 0} }
    },

    /* UL/DL Configuration 2 (DCI-0 (3,8), HI-> 7, 2)

                  0(DL)                        1(SP)                           2(UL)    */            
    { {DL, {NA}, {NA}, {0, 0, 0} }, {SP, {NA}, {NA}, {0, 0, 0} }, {UL, {6}, {10}, {4, 8, 14} },
        /*       3(DL)                         4(DL)                           5(DL)    */ 
        {DL, {4}, {14}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} },
        /*        6(SP)                        7(UL)                            8(DL)    */  
        {SP, {NA}, {NA}, {0, 0, 0} }, {UL, {6}, {10}, {4, 3, 14} }, {DL, {4}, {14}, {0, 0, 0} },
        /*          9(DL)      */
        {DL, {NA}, {0}, {0, 0, 0} } 
    },
    /* UL/DL Configuration 3(DCI-0, 0,8,9) UL-2,3,4
                     0(DL)                        1(SP)                     2(UL)    */            
    { {DL, {4}, {14}, {0, 0, 0} }, {SP, {NA}, {NA}, {0, 0, 0} }, {UL, {6}, {10}, {4, 8, 14} },
        /*           3(UL)                        4(UL)                     5(DL)    */ 
        {UL, {6}, {10}, {4, 9, 14} }, {UL, {6}, {10}, {4, 0, 14} }, {DL, {NA}, {NA}, {0, 0, 0} },
        /*           6(DL)                        7(DL)                     8(DL)    */  
        {DL, {NA}, {NA}, {0, 0, 0} },{DL, {NA}, {NA}, {0, 0, 0} }, {DL, {4}, {14}, {0, 0, 0} },
        /*          9(DL)      */
        {DL, {4}, {14}, {0, 0, 0} }
    },



    /* UL/DL Configuration 4 (DCI-0-> 8,9) UL-> 2,3
                  0(DL)                        1(SP)                         2(UL)    */            
    { {DL, {NA}, {NA}, {0, 0, 0} }, {SP, {NA}, {NA}, {0, 0, 0} }, {UL, {6},{10}, {4, 8, 14} },
        /*        3(UL)                        4(DL)                         5(DL)    */ 
        {UL, {6}, {10}, {4, 9, 14} }, {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {NA},{NA}, {0, 0, 0} },
        /*        6(DL)                        7(DL)                         8(DL)    */  
        {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {4},{14}, {0, 0, 0} },
        /*          9(DL)      */
        {DL, {4}, {14}, {0, 0, 0} }
    },

    /* UL/DL Configuration 5 (DCI-0 ->8), (UL -> 2)
                   0(DL)                        1(SP)                         2(UL)    */            
    { {DL, {NA}, {NA}, {0, 0, 0} }, {SP, {NA}, {NA}, {0, 0, 0} }, {UL, {6}, {10}, {4, 8, 14} },
        /*         3(DL)                        4(DL)                         5(DL)    */ 
        {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} },
        /*        6(DL)                         7(DL)                         8(DL)    */  
        {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {NA}, {NA}, {0, 0, 0} }, {DL, {4}, {14}, {0, 0, 0} },
        /*          9(DL)      */
        {DL, {NA}, {NA}, {0, 0, 0} }

    },

    /* UL/DL Configuration 6 (DCI-0 ->0,1,5,6,9) UL->2,3,4,7,8
                 0(DL)                        1(SP)                 2(UL)    */            
    { {DL, {7}, {18}, {0, 0, 0} }, {SP, {7}, {21}, {0, 0, 0} }, {UL, {4}, {11}, {7, 5, 18} },
        /*       3(UL)                        4(UL)                 5(DL)    */ 
        {UL, {6}, {11}, {7, 6, 18} }, {UL, {6}, {13}, {5, 9, 18} }, {DL, {7}, {18}, {0, 0, 0} },
        /*        6(SP)                        7(UL)                 8(UL)    */  
        {SP, {7}, {18}, {0, 0, 0} }, {UL, {4}, {11}, {7, 0, 18} }, {UL, {7}, {14}, {7, 1, 21} },
        /*          9(DL)      */
        {DL, {5}, {18}, {0, 0, 0} }

    }
};

/*
 * Refer:- TS: 36.213 Section 10.1 Table 10.1-1: Downlink association set index
 *
 */
STATIC DLAckNackSubFrameNumInfo dlAckNackSubFrameNumInfo[TOTAL_NUM_UL_DL_CONFIG]
                                                           [MAX_SUB_FRAME] =
{

  /* Configuratoin 0 */
  /*  0(D)    1(S)  2(U)   3(U)   4(U)   5(D)  6(S)   7(U)  8(U) 9(D)*/
  {   {4},    {6},  {0},   {0},   {0},   {4},   {6},  {0},  {0}, {0}},
  /* Configuratoin 1 */
  /* 0(D)    1(S)   2(U)   3(U)   4(D)   5(D)   6(S)   7(U)  8(U) 9(D)*/
  {   {7},   {6},   {0},   {0},   {4},   {7},   {6},   {0},  {0},  {4}},
  /* Configuratoin 2 */
  /* 0(D)    1(S)   2(U)   3(D)   4(D)   5(D)   6(S)   7(U) 8(D)   9(D)*/
  {   {7},   {6},   {0},   {4},   {8},   {7},   {6},   {0},  {4},  {8}},
  /* Configuratoin 3 */
  /* 0(D)    1(S)   2(U)   3(U)   4(U)   5(D)   6(D)   7(D)    8(D)  9(D)*/
  {   {4},   {11},   {0},   {0},   {0},   {7},   {6},   {6},   {5},  {5}},
  /* Configuratoin 4 */
  /*   0(D)    1(S)    2(U)    3(U)  4(D)  5(D)    6(D)   7(D)  8(D)   9(D)*/
  {   {12},    {11},   {0},   {0},   {8},  {7},    {7},   {6},   {5},   {4} },
  /* Configuratoin 5 */
  /*   0(D)    1(S)   2(U)  3(D)   4(D)   5(D)   6(S)   7(U)   8(U)   9(D)*/
  {   {12},    {11},   {0},   {9},   {8},   {7},   {6},   {5},   {4},   {13}},
  /* Configuratoin 6 */
  /*  0(D)    1(S)   2(U)   3(U)   4(U)   5(D)   6(S)   7(U)    8(U)   9(D)*/
  {   {7},    {7},   {0},   {0},   {0},   {7},   {7},    {0},   {0},   {5}},
};

STATIC MaxContainerForCRCHandling maxContainerForCRCHandling_g
                                             [TOTAL_NUM_UL_DL_CONFIG] =
{
/*  Max UL Container   Max UL RB Maps */
    {7              ,          24 },
    {14             ,          20 },
    {13             ,          18 },
    {10             ,          18 },
    {9              ,          18 },
    {8              ,          17 },
    {15             ,          25 }
};

STATIC MaxHarqProcessNumULDL maxULDLHarqProcessNum_g
                                         [TOTAL_NUM_UL_DL_CONFIG] =
{
/*Max DL harq             Max UL Harq Process */
    {4             ,            7 },
    {7             ,            4 },
    {10            ,            2 },
    {9             ,            3 },
    {12            ,            2 },
    {15            ,            1 },
    {6             ,            6 }
};
/** CA-TDD Changes Start **/
TddConfigInfo tddConfigInfo_g[MAX_NUM_CELL];
TddTimerConvMapInfo tddTimerConvMap_g[MAX_NUM_CELL];
/** CA-TDD Changes End **/

/* This structure stores the values of container index and DCI0 sub frame 
 * on which UL CONFIG message needs to be prepared for Msg3. Here for each
 * D or S a DCI0 SF is stored on which ul grant is to send for U on which 
 * Msg3 is to be recievd for that D or S (Which is used to send RAR).
 * Here id RAR is sent on t then U should be atleast t+6 */
STATIC SfForRachULConfig sfForRachULConfig_g[TOTAL_NUM_UL_DL_CONFIG]
                                                [MAX_SUB_FRAME] =
{
    /* TDD Config 0 Changes Start */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {   {1, 0}, {0, 0}, {NA, NA}, {NA, NA}, {NA, NA}, {1, 0}, 
        /*  6(S)       7(U)      8(U)      9(U)   */
        {0, 0}, {NA, NA}, {NA, NA}, {NA, NA} },
    /* TDD Config 0 Changes End */
    /*UL/DL Configuration 1 */
    /*  0(D)       1(S)      2(U)      3(U)      4(D)      5(D)  */
    {   {1, 0}, {0, 0}, {NA, NA}, {NA, NA}, {2, 0}, {1, 0},
        /*  6(S)       7(U)      8(U)      9(D)   */
        {0, 0}, {NA, NA}, {NA, NA}, {2, 1} },
    /*UL/DL Configuration 2 */
    /*  0(D)       1(S)      2(U)      3(D)      4(D)      5(D)  */
    {   {3, 0}, {2, 0}, {NA, NA}, {5, 0}, {4, 0}, {3, 0},
        /*  6(S)       7(U)      8(D)      9(D)   */
        {2, 0}, {NA, NA}, {5, 1}, {4, 1} },
    /*UL/DL Configuration 3 */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {   {8, 0}, {7, 0}, {NA, NA}, {NA, NA}, {NA, NA}, {3, 0}, 
        /*  6(D)  7(D)    8(D)    9(D)   */
        {2, 0}, {2, 0}, {2, 1}, {9, 1} },
    /*UL/DL Configuration 4 */
    /*  0(D)       1(S)      2(U)      3(U)   4(D)      5(D)  */
    {   {8, 0}, {7, 0}, {NA, NA}, {NA, NA}, {4, 0}, {3, 0}, 
        /*  6(D)   7(D)  8(D)  9(D)   */
        {2, 0}, {2, 0}, {10, 1}, {9, 1} },
    /*UL/DL Configuration 5 */
    /*  0(D)       1(S)      2(U)    3(D)   4(D)   5(D)  */
    {   {8, 0}, {7, 0}, {NA, NA}, {5, 0}, {4, 0}, {3, 0}, 
        /*  6(D)   7(D)  8(D)    9(D)   */
        {2, 0}, {11, 1}, {10, 1}, {9, 1} },
    /* TDD Config 6 Changes Start */
    /*UL/DL Configuration 6 */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {   {0, 0}, {0, 0}, {NA, NA}, {NA, NA}, {NA, NA}, {0, 0}, 
        /*  6(S)       7(U)      8(U)      9(D)   */
        {0, 0}, {NA, NA}, {NA, NA}, {1, 1} }
    /* TDD Config 6 Changes End */
};



/* SPR 5333 changes start */
STATIC UInt8 numOfPhichInSFPerUE_g[TOTAL_NUM_UL_DL_CONFIG]
						[MAX_SUB_FRAME] = 

{
    /*Config 0 needs to be updated later */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    2,         1,        NA,       NA,       NA,        2,
        /*  6(S)       7(U)      8(U)      9(U)   */
             1,         NA,       NA,       NA   },
    /*UL/DL Configuration 1 */
    /*  0(D)       1(S)      2(U)      3(U)      4(D)      5(D)  */
    {    0,         1,        NA,       NA,       1,        0,
        /*  6(S)       7(U)      8(U)      9(D)   */
             1,         NA,       NA,       1 },
    /*UL/DL Configuration 2 */
    /*  0(D)       1(S)      2(U)      3(D)      4(D)      5(D)  */
    {    0,          0,         NA,       1,         0,          0,
        /*  6(S)       7(U)      8(D)      9(D)   */
             0,          NA,        1,         0},
    /*UL/DL Configuration 3 */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    1,          0,         NA,        NA,        NA,        0, 
        /*  6(D)  7(D)    8(D)    9(D)   */
             0,      0,      1,       1,  },
    /*UL/DL Configuration 4 */
    /*  0(D)       1(S)      2(U)      3(U)   4(D)      5(D)  */
    {    0,          0,         NA,        NA,     0,         0,  
        /*  6(D)   7(D)  8(D)  9(D)   */
             0,       0,    1,     1 },
    /*UL/DL Configuration 5 */
    /*  0(D)       1(S)      2(U)    3(D)   4(D)   5(D)  */
    {    0,          0,         NA,      0,      0,      0,  
        /*  6(D)   7(D)  8(D)    9(D)   */
             0,      0,     1,        0   },
    /* UL/DL Configuration 6  */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    1,          1,         NA,        NA,        NA,        1, 
        /*  6(S)       7(U)      8(U)      9(D)   */
             1,          NA,        NA,        1 }
};
/* SPR 5333 changes end */

/* + SPS_TDD_Changes */
#define NA_SF 128
static SInt8 subFrameOffsetForSps[TOTAL_NUM_UL_DL_CONFIG]
          [MAX_SUB_FRAME]=
{
    /* UL/DL Configuration 0*/
    /*  0(D)       1(S)      2(U)    3(U)   4(U)   5(D)  */
    {   NA_SF,        NA_SF,       0,      0,    0,    NA_SF,
        /*  6(S)      7(U)      8(U)    9(U)   */
        NA_SF,       0,        0,      0
    },
    /* UL/DL Configuration 1*/
    /*  0(D)         1(S)       2(U)       3(U)         4(D)       5(D)  */
    {   NA_SF,       NA_SF,       1,       -1,         NA_SF,       NA_SF,  
        /*   6(S)       7(U)       8(U)      9(D)   */
            NA_SF,        1,       -1,       NA_SF   
    },
    /* UL/DL Configuration 2*/
    /*  0(D)         1(S)          2(U)       3(D)          4(D)          5(D)  */
    {    NA_SF,       NA_SF,       5,        NA_SF,        NA_SF,        NA_SF,
        /*  6(S)          7(U)        8(D)       9(D)   */
            NA_SF,         -5,       NA_SF,      NA_SF
    },
    /* UL/DL Configuration 3*/
    /*   0(D)          1(S)       2(U)      3(U)      4(U)      5(D)  */
    {    NA_SF,       NA_SF,        1 ,      1,        -2,       NA_SF,       
        /*  6(D)         7(D)          8(D)       9(D)   */
            NA_SF,       NA_SF,       NA_SF,     NA_SF
    },
    /* UL/DL Configuration 4*/
    /*  0(D)           1(S)       2(U)     3(U)    4(D)       5(D)  */
    {    NA_SF,       NA_SF,      1,       -1,    NA_SF,     NA_SF,  
        /*   6(D)       7(D)          8(D)        9(D)   */
            NA_SF,       NA_SF,       NA_SF,     NA_SF
    },
    /* UL/DL Configuration 5*/
    /*  0(D)           1(S)      2(U)       3(D)       4(D)     5(D)  */
    {   NA_SF,        NA_SF,       0,      NA_SF,    NA_SF,    NA_SF,       
        /*  6(D)         7(D)          8(D)      9(D)   */
            NA_SF,       NA_SF,       NA_SF,     NA_SF  
    },
    /* UL/DL Configuration 6*/
    /*  0(D)       1(S)      2(U)    3(U)   4(U)   5(D)  */
    {   NA_SF,        NA_SF,        0,      0,      0,    NA_SF,
        /*  6(S)      7(U)      8(U)    9(D)   */
        NA_SF,        0,        0,     NA_SF
    }
};
/* - SPS_TDD_Changes */


#ifdef LTE_EMBMS_SUPPORTED
STATIC UInt8 embmsValidTddDlSF_g[TOTAL_NUM_UL_DL_CONFIG]
                        [MAX_SUB_FRAME] =

{
    /*Config 0 needs to be updated later */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    0,         0,        NA,       NA,       NA,        0,
        /*  6(S)       7(U)      8(U)      9(U)   */
             0,         NA,       NA,       NA   },
    /*UL/DL Configuration 1 */
    /*  0(D)       1(S)      2(U)      3(U)      4(D)      5(D)  */
    {    0,         0,        NA,       NA,       1,        0,
        /*  6(S)       7(U)      8(U)      9(D)   */
             0,         NA,       NA,       1 },
    /*UL/DL Configuration 2 */
    /*  0(D)       1(S)      2(U)      3(D)      4(D)      5(D)  */
    {    0,          0,         NA,       1,         1,          0,
        /*  6(S)       7(U)      8(D)      9(D)   */
             0,          NA,        1,         1},
    /*UL/DL Configuration 3 */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    0,          0,         NA,        NA,        NA,        0,
        /*  6(D)  7(D)    8(D)    9(D)   */
             0,      1,      1,       1,  },
    /*UL/DL Configuration 4 */
    /*  0(D)       1(S)      2(U)      3(U)   4(D)      5(D)  */
    {    0,          0,         NA,        NA,     1,         0,
        /*  6(D)   7(D)  8(D)  9(D)   */
             0,       1,    1,     1 },
    /*UL/DL Configuration 5 */
    /*  0(D)       1(S)      2(U)    3(D)   4(D)   5(D)  */
    {    0,          0,         NA,      1,      1,      0,
        /*  6(D)   7(D)  8(D)    9(D)   */
             0,      1,     1,        1   },
    /* UL/DL Configuration 6  */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    0,          0,         NA,        NA,        NA,        0,
        /*  6(S)       7(U)      8(U)      9(D)   */
             0,          NA,        NA,        1 }
};
#endif

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* This function is used by Strategy and Scheduler to check 
 * the free Harq id is availiable or not
 */
/*  TDD REL 3.1.1 */
/* TDD Config 0, 6 Changes Start */
STATIC  UInt8 checkForFreeULHarqProcess(
        ULUEContext *ulUEContext_p, 
        UInt8 subFrameNum,
        InternalCellIndex cellIndex,
        UInt8 ulSubFrameNum,
        /* SPR 11257 Changes Start*/
        /* +- SPR 17777 */
        /*SPR 15909 fix start*/
        tickType_t currentGlobalTTITickCount);
        /*SPR 15909 fix end*/
        /* SPR 11257 Changes End*/
/* TDD Config 0, 6 Changes End */
/* SPR 11110 fix start */
        /*SPR 15909 fix start*/
STATIC MacRetType freeAllNodesForIndexFromULHarqTimerList( tickType_t recvTTI,
        UInt8  index,
        UInt8  prevULSubFrame,
        UInt8  hiDLSF,
        UInt8  freeHarqOnly,
        InternalCellIndex cellIndex );
        /*SPR 15909 fix end*/
/* SPR 11110 fix end */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
*****************************************************************************/

/* TDD Config 6 Changes Start */
/*
** This points to configured TDD configuration entries in ulSfNumFrmPhich_g.
** This will be populated in initMacTDDConfig().
*/

/* CA TDD Changes Start */
UInt8 *ulSfNumFrmPhichInfo_gp[MAX_NUM_CELL] = {PNULL};
/* CA TDD Changes End */
/* TDD Config 6 Changes End */
/* TDD Config 0 Changes Start*/
/* This map contain the Retx UL Subframe coresponding 
 * Current Subframe no*/
/* CA TDD CHANGES START */
STATIC PuschTransmissionSf *ulSubFrameForPuschTransmission_gp[MAX_NUM_CELL] = {PNULL};
STATIC UInt8 *ulSubframeToRTTMap_gp[MAX_NUM_CELL] = {PNULL};
/* CA TDD CHANGES END */
/* TDD Config 0 Changes End */

/*
 * This sturcture UlGrantHIPduSubFrameInfo is used for following functionalities:-
 * 1. check current subframe is UL or DL or SP
 * 2. DCI-0 send for UL Subframe
 * 3. Resource Map Number
 * 4. HI Pdu Subframe Number
 * 5. Downlin ACK/NACK
 */
/** CA-TDD Changes Start **/
/* + SPS_TDD_Changes */
UlGrantHIPduSubFrameInfo *ulGrantPhichTableInfo_p[MAX_NUM_CELL] = {PNULL};
/* - SPS_TDD_Changes */

/*
 *  This static global variable maxDLHarqProcess_g is used for 
 *  getting the maximum downlink harq process supported correspondence to
 *  UL/DL configuration.
 */
/** CA-TDD Changes Start **/
STATIC UInt8 maxDLHarqProcess_g[MAX_NUM_CELL] = {0};
/** CA-TDD Changes Stop **/

/*
 * Using the appropriate Container there are different number of 
 * UL Containter to store:- 
 * 1. CRC-ACK 
 * 2. CRC-NACK
 * 3. CRC-INAVALID
 * It depends upon the UL/DL Configuration to select the number
 * of UL Container.
 * This maxULContainer_g variable is used for get the Max. number
 * of UL Container correspondece to the UL/DL configuration.
 *
 */
STATIC UInt8 maxULContainer_g[MAX_NUM_CELL]   = {0};
/** CA-TDD Changes End **/

/*
 * We are reserving the resources for uplink
 * data in nth tick for (n + k)th tick.
 * So, we are maintaing different number 
 * of Uplink Resource Maps.
 *
 * This maxULRBMaps_g is used to get the 
 * value of supported the number of 
 * Resouces Maps.
 *
 */
/** CA-TDD Changes Start **/
STATIC UInt8 maxULRBMaps_g[MAX_NUM_CELL]      = {0};

/*
 * This currentConfigIndex_g variable is used 
 * for only this file to  store the 
 * current UL/DL configuration.
 *
 */

#ifdef MAC_AUT_TEST
UInt8 currentConfigIndex_g[MAX_NUM_CELL] = {0};
#else
STATIC UInt8 currentConfigIndex_g[MAX_NUM_CELL] = {0};
#endif

/** CA-TDD Changes End **/

/*
 * This DLAckNackSubFrameNumInfo structure is used to
 * store the information about downlink ack/nack subframe.
 *
 */
/** CA-TDD Changes Start **/
STATIC DLAckNackSubFrameNumInfo *dlAckNackSubFrameNumInfo_p[MAX_NUM_CELL] = {PNULL};

/* This pointer is used to point the array corresponding to the current UL/DL 
 * Configuration used in sturcture sfForRachULConfig_g.
 * */
STATIC SfForRachULConfig *sfForRachULConfig_p[MAX_NUM_CELL] = {PNULL};
/** CA-TDD Changes End **/

/* SPR 5333 changes start */
STATIC UInt8 *numOfPhichInSFPerUE_p[MAX_NUM_CELL] = {PNULL}; 

void tdd_config(void);
void tdd_config_1(void);
void tdd_config_2(void);
void get_mem(void);


#ifdef MAC_AUT_TEST

void get_mem(void)
{
UInt8 temp2[2];
DLAckNackSubFrameNumInfo temp3[2];
DLAckNackSubFrameNumInfo temp1[2];
UlGrantHIPduSubFrameInfo temp[2];
SfForRachULConfig temp5[2];
ulGrantPhichTableInfo_p[0]=&temp[0];
ulGrantPhichTableInfo_p[1]=&temp[1];
dlAckNackSubFrameNumInfo_p[0]=&temp1[0];
numOfPhichInSFPerUE_p[0]=&temp2[0];
numOfPhichInSFPerUE_p[1]=&temp2[1];
dlAckNackSubFrameNumInfo_p[0]=&temp3[0];
dlAckNackSubFrameNumInfo_p[1]=&temp3[1];
sfForRachULConfig_p[0]=&temp5[0];
sfForRachULConfig_p[1]=&temp5[1];
maxULContainer_g[0]=1;
maxULRBMaps_g[0]=1;
maxULRBMaps_g[1]=1;
}
#endif 
/* SPR 5333 changes end */
/* SPR 5333 changes start */
/** CA-TDD Changes End **/
 UInt8 miValueForSubframe(
				 UInt8 subframe,
                 InternalCellIndex internalCellIndex
			       )
{
        /** CA-TDD Changes Start **/
	UInt8 miValue = numOfPhichInSFPerUE_p[internalCellIndex][subframe];
        /** CA-TDD Changes End **/
	if( NA == miValue )
	{
	    ltePanic("\nUplink Subframe passed to fetch mi Value for PHICH\n");
	}
	return miValue;

}
/* SPR 5333 changes end */
void GetMem(void);
#ifdef MAC_AUT_TEST
void GetMem(void)
{
        UInt8 temp[100];
	UInt8 temp1[10];
	UInt8 temp2[10];
        InternalCellIndex internalCellIndex=0;
        UInt8 subframe=0;
	numOfPhichInSFPerUE_p[internalCellIndex] = temp;
	ulGrantPhichTableInfo_p[internalCellIndex] = temp1;
 	dlAckNackSubFrameNumInfo_p[internalCellIndex] = temp2;
	maxULContainer_g[0]= 10;   
	maxULRBMaps_g[0] = 10;
}
#endif

/* EICIC - TDD +*/
extern UInt16 uplinkToGrantMap_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME];
/* EICIC - TDD -*/
/*SPR 19221 +*/
#ifdef CIRC_QUEUE_IMPL
/*CA Changes start  */
extern sem_t waitForULNackForTempUE_g[MAX_NUM_CELL];
/*CA Changes end  */
#endif
/*SPR 19221 -*/

/* TDD Config 6 Changes Start */
/*****************************************************************************
 * Function Name  : getUlSfOnPhichProcessing
 * Inputs         : subframeNum  - DL subframe number.
                    InternalCellIndex internalCellIndex
 * Returns        : UL subframe number for which PHICH is being processed.
 * Description    : This function returns UL subframe number for which PHICH
 *                  is being processed.
 *
 *****************************************************************************/

UInt8 getUlSfOnPhichProcessing(UInt8 subframeNum,InternalCellIndex internalCellIndex)
{
    MAC_ASSERT((subframeNum < MAX_SUB_FRAME) &&
               (DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame));
    /* CA TDD Changes Start */
    SInt8 ulSubframeNum = subframeNum - ulSfNumFrmPhichInfo_gp[internalCellIndex][subframeNum];
    /* CA TDD Changes End */

    /*
    ** Checks whether calculated ulSubframeNum is less than 0,
    ** if yes, it means ulSubframeNum for which PHICH is being
    ** processed belongs to previous system frame Number.
    */

    if (ulSubframeNum < 0)
    {
        ulSubframeNum = ulSubframeNum + MAX_SUB_FRAME;
    }

    return ((UInt8)ulSubframeNum);
}
/* TDD Config 6 Changes End */

/*****************************************************************************
 * Function Name  : initMacTDDConfig 
 * Inputs         : None
 * Outputs        : Initialize all static global variables.
 * Variables      : None
 * Description    : This function will intialize the static global
 *                  variable correspoding to the UL/DL configuration.
 *****************************************************************************/
/** CA-TDD Changes Start **/
 void initMacTDDConfig(InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);

    currentConfigIndex_g[internalCellIndex]    = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign;
    UInt8 currentConfigIndex =  currentConfigIndex_g[internalCellIndex];
    ulGrantPhichTableInfo_p[internalCellIndex] = ulGrantPhichTableInfo_g[currentConfigIndex];

    maxDLHarqProcess_g[internalCellIndex]      = maxULDLHarqProcessNum_g[currentConfigIndex].
                                                            maxDLHarqProcess;
    maxULContainer_g[internalCellIndex]   = maxContainerForCRCHandling_g[currentConfigIndex].
                                                            maxCrcContainer;
    maxULRBMaps_g[internalCellIndex]      = maxContainerForCRCHandling_g[currentConfigIndex].
                                                    maxULResouceAllocatorMap;
   dlAckNackSubFrameNumInfo_p[internalCellIndex] = dlAckNackSubFrameNumInfo[currentConfigIndex];                                                    

   sfForRachULConfig_p[internalCellIndex] = sfForRachULConfig_g[currentConfigIndex];
    /* TDD Config 6 Changes Start */
    /* CA TDD Changes Start */
    ulSfNumFrmPhichInfo_gp[internalCellIndex] = ulSfNumFrmPhich_g[currentConfigIndex];

   initElArgs(ulGrantPhichTableInfo_p[internalCellIndex], 
             dlAckNackSubFrameNumInfo_p[internalCellIndex],internalCellIndex,ulSfNumFrmPhichInfo_gp[internalCellIndex]);
    /* CA TDD Changes End */
    /*TDD Config 0 Changes Start*/
   /* CA TDD CHANGES START */
    ulSubFrameForPuschTransmission_gp[internalCellIndex] = ulSubFrameForPuschTransmission_g[currentConfigIndex];
    ulSubframeToRTTMap_gp[internalCellIndex] = ulSubframeToRTTMap_g[currentConfigIndex];
    /* CA TDD CHANGES END */
    /*TDD Config 0 Changes End*/


    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit. ",__func__);
}
/** CA-TDD Changes End **/
/*****************************************************************************
 * Function Name  : getULGrantSubframeNum
 * Inputs         : subframe - This is present/current Subframe 
 *                  onlyDownlinkUeSchFlag - This flag value should be 0 or 1
 *                  0 means this subframe is used for send DCI-0/DL Data.
 *                  1 means this subframe is used for only send DL data
 *
 * Returns        : UL subrame number
 * Description    : This function will return the exact UL subframe number 
 *                  where the UL data or Downlink Ack/nack will come. 
 
 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getULGrantSubframeNum ( UInt8 subframeNum, 
                                     UInt8 onlyDownlinkUeSchFlag,
                                     InternalCellIndex internalCellIndex
                                   )
{    
  LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);
  MAC_ASSERT( subframeNum < MAX_SUB_FRAME );

  UInt8 dlAckNackSubFrameNumber = 0;
  UInt8 ulGrantSubFrameNum      = 0;
  if ( onlyDownlinkUeSchFlag)
  {  
      MAC_ASSERT( subframeNum < MAX_SUB_FRAME && 
                  (DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame
                   || SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame)
                ); 
      dlAckNackSubFrameNumber = 
               dlAckNackSubFrameNumInfo_p[internalCellIndex][subframeNum].dlAckNackSubFrameNumber;
      dlAckNackSubFrameNumber += subframeNum;
      if ( dlAckNackSubFrameNumber >= MAX_SUB_FRAME)
      {
           dlAckNackSubFrameNumber = (dlAckNackSubFrameNumber % MAX_SUB_FRAME);
      }
      return (dlAckNackSubFrameNumber); 
  }
  else
  {
      MAC_ASSERT( ((SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame) ||
                  (DL == (ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame))) &&
                  (NA != (ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].
                  ulGrantPhichInfo.ulGrantSubFrameNum)));

      LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"SubFrameNo[%d] K [%d]\n", 
              subframeNum, ulGrantPhichTableInfo_p[subframeNum].ulGrantPhichInfo.
              ulGrantSubFrameNum);
/* SPR 609 changes start */      
      ulGrantSubFrameNum = 
            ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulGrantPhichInfo.ulGrantSubFrameNum;
   /** CA-TDD Changes End **/
/* SPR 609 changes end */      
      ulGrantSubFrameNum += subframeNum;
      if ( ulGrantSubFrameNum >= MAX_SUB_FRAME)
      {
           ulGrantSubFrameNum -= MAX_SUB_FRAME;
      }
      return (ulGrantSubFrameNum);
  }              
}
/*****************************************************************************
 * Function Name  : getUlSfnFactor
 * Inputs         : subframeNum - This is current subframe number
 *
 * Returns        : This function will return the 0 or 1. 
 * Description    : This function will check the UL grant SF with current SF
                   return the result
 *****************************************************************************/
/** CA-TDD Changes Start **/
/* +- SPR 17777 */
 UInt8 getUlSfnFactor(UInt8 subframeNum,
			   /*TDD Config 0 Changes Start*/
			    UInt8 ulSubframeNum)
			   /*TDD Config 0 Changes End*/
{
    if (subframeNum > MAX_SUB_FRAME || ulSubframeNum > MAX_SUB_FRAME)
    {
        lteWarning("In function [%s], Invalid SF[%d] and U-SF[%d] received\n",\
				__func__, subframeNum, ulSubframeNum);
    }

    /* If the current UL grant is 
     * next System Frame then return 1 
     * else 0.
     */

    /*TDD Config 0 Changes Start*/
    if ( ulSubframeNum < subframeNum)
    /*TDD Config 0 Changes End*/
    {
        return 1;  
    }
    else
    {
        return 0;
    }

}
/*****************************************************************************
 * Function Name  : getExactULSubFrame
 * Inputs         : subframeNum - This is present/current subframe number
 *
 * Returns        :UL Grant SF
 * Description    :The function will return the UL Grant Subframe

 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getExactULSubFrame( UInt8 subframeNum,InternalCellIndex internalCellIndex)
{
     return (ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].
                    ulGrantPhichInfo.ulGrantSubFrameNum);

}
/*****************************************************************************
 * Function Name  : getHiPduSubframeNum 
 * Inputs         : subframeNum - This is present/current subframe number
 *
 * Returns        : DL/SP subframe number
 * Description    : This function will calculate the subframe number 
 *                  where we can send HI pdu.

 *****************************************************************************/
 UInt8 getHiPduSubframeNum( UInt8 subframeNum,InternalCellIndex internalCellIndex )
{
  LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);

  MAC_ASSERT( UL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame 
              && NA != ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulGrantPhichInfo.
              ulPHICHSubFrameNum);
 
  LTE_MAC_UT_LOG( LOG_INFO,TDD_CONFIGURATION, "SubFrameNo[%d] K [%d]\n", 
         subframeNum, ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulGrantPhichInfo.
          ulPHICHSubFrameNum);
                 
  LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit. ",__func__);
   
  return (ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].
                ulGrantPhichInfo.ulPHICHSubFrameNum);
/** CA-TDD Changes End **/
}

/* CA TDD Removed Unused getMaxULHarq Function */

/*****************************************************************************
 * Function Name  : getMaxDLHarq 
 * Inputs         : None
 * Returns        : Number of DL Harq Processes Supported
 * Description    : This function will return the max. DL harq Processes.

 *****************************************************************************/
 UInt8 getMaxDLHarq(InternalCellIndex internalCellIndex)
{
    return maxDLHarqProcess_g[internalCellIndex];
}

/*****************************************************************************
 * Function Name  : getMaxULCrcContainer 
 * Inputs         : None
 * Returns        : Number of UL Container Supported
 * Description    : This function will return the max. UL Container.

 *****************************************************************************/
 UInt8 getMaxULCrcContainer(InternalCellIndex internalCellIndex)
{
    return maxULContainer_g[internalCellIndex];
}
/** CA-TDD Changes End **/

/*****************************************************************************
 * Function Name  : getMaxULRBMaps 
 * Inputs         : None
 * Returns        : Number of UL Resource Maps
 * Description    : This function will return the max. UL Resouce Maps. 

 *****************************************************************************/
  /** CA-TDD Changes Start **/
 UInt8 getMaxULRBMaps(InternalCellIndex internalCellIndex)
{
   return maxULRBMaps_g[internalCellIndex];
   /** CA-TDD Changes End **/
}

/*****************************************************************************
 * Function Name  : getNextRBMapNumForNonAdaptive
 * Inputs         : subframeNum - This is present/current subframe number. 
 *
 * Returns        : UL Resource Map number
 * Description    : This function will return the resource map number
 *                   for non adaptive retransmission.
 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getNextRBMapNumForNonAdaptive( UInt8 subframeNum,InternalCellIndex internalCellIndex )
{
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);

    MAC_ASSERT( ((SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame) ||
                (DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame) )&&
                ( NA != ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].rbMapNum.
                      rbMapNumForNonAdaptive) );
 
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"subFrameNo[%d]\
                                         AdvanceRBMapNum [%d]\n",
            subframeNum, ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].
            rbMapNum.rbMapNumForNonAdaptive );

    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit. ",__func__);
    return ( ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].rbMapNum.
             rbMapNumForNonAdaptive);
}


/*****************************************************************************
 * Function Name  : getAdvanceReserveRBMap
 * Inputs         : recvdULDataSubframe
 *
 * Returns        : RB Map Number
 * Description    : This function will return the Advance Reserve Map Number.

 *****************************************************************************/
 UInt8 getAdvanceReserveRBMap( UInt32 recvdULDataSubframe,InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);
    MAC_ASSERT( (recvdULDataSubframe < MAX_SUB_FRAME)
             &&(UL == ulGrantPhichTableInfo_p[internalCellIndex][recvdULDataSubframe].ulDlSpFrame)
            && (NA != ulGrantPhichTableInfo_p[internalCellIndex][recvdULDataSubframe].rbMapNum.
            alreadyReserveRBNum));

    LTE_MAC_UT_LOG( LOG_INFO,TDD_CONFIGURATION,
                    "subFrameNo[%d] AlreadyRBMapNo. [%d]\n",
                     recvdULDataSubframe,
                     ulGrantPhichTableInfo_p[internalCellIndex][recvdULDataSubframe].
                     rbMapNum.alreadyReserveRBNum
                  );     
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit. ",__func__);

    return (ulGrantPhichTableInfo_p[internalCellIndex][recvdULDataSubframe].
               rbMapNum.alreadyReserveRBNum);
/** CA-TDD Changes End **/
}

/*****************************************************************************
 * Function Name  : isValidSfForSIMsg
 * Inputs         : current subframe
 *
 * Returns        : TRUE or FALSE 
 * Description    : This function will return the valid subframe for SI message.

 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 isValidSfForSIMsg ( UInt8 subframe,InternalCellIndex internalCellIndex)
{
    if ( DL == ulGrantPhichTableInfo_p[internalCellIndex][subframe].ulDlSpFrame )
/** CA-TDD Changes End **/
    {
        return TRUE;
    }
    else
    {
       return FALSE;
    }

}

/*****************************************************************************
 * Function Name  : checkForSpecialSubframe
 * Inputs         : current subframe
 *
 * Returns        : TRUE or FALSE 
 * Description    : This function will return TRUE if current subframe is 
                    special subframe.
 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8  checkForSpecialSubframe ( UInt8 subframe,InternalCellIndex internalCellIndex )
{
    if ( SP == ulGrantPhichTableInfo_p[internalCellIndex][subframe].ulDlSpFrame )
/** CA-TDD Changes End **/
    {
        return TRUE;
    }
    else
    {
       return FALSE;
    }
}

/*****************************************************************************
 * Function Name  : UInt8 getFreeULHarqId
 * Inputs         : ulUEContext_p
 *                  subFrame
 *                  ulSubFrame
 *                  sysFrameNum
 * Returns        : Free Harq Process Id.
 * Description    : This is the wrapper for function for checking the
 *                  free UL harq process Id.

 *****************************************************************************/
/*  TDD REL 3.1.1 */
/* TDD Config 6 Changes Start */
 UInt8 getFreeULHarqId(
        ULUEContext *ulUEContext_p, 
        UInt8 subFrame, 
        InternalCellIndex internalCellIndex,
        UInt8 ulSubFrame,
        /* SPR 11257 Changes Start*/
        /* +- SPR 17777 */
        /*SPR 15909 fix start*/
        tickType_t currentGlobalTTITickCount
        /*SPR 15909 fix end*/
        /* SPR 11257 Changes End*/
        )
{
 /* +- SPR 17777 */
    return checkForFreeULHarqProcess(ulUEContext_p,subFrame, internalCellIndex,ulSubFrame,currentGlobalTTITickCount);
}
/*  TDD REL 3.1.1 */

/* SPR 3114 Start */
/*****************************************************************************
 * Function Name  : checkForFreeHarqProcessForMsg3
 * Inputs         : ueIndex - The UE Index for which the HARQ process is required.
 *                  sysFrameNum - System Frame Num.
 *                  subFrameNum - subframe Num.
 *                  ulSubFrameNum - UL subframe number
 * Returns        : return Free Harq Process ID. 
 * Description    : This function checks for any free Harq Process within a
 *                  UE Context.
 *****************************************************************************/
/** CA-TDD Changes Start **/
STATIC  UInt8 checkForFreeULHarqProcessForMsg3( ULUEContext *ulUEContext_p,
                    InternalCellIndex internalCellIndex,                    
                    /* TDD Config 0,6 Changes Start */
                    /* SPR 11257 Changes Start*/
                    UInt8 subFrameNum,
                    UInt8 ulSubFrameNum,
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTTI)
/* SPR 15909 fix end */
                    /* SPR 11257 Changes End*/
                    /* TDD Config 0,6 Changes End */
{
    ULHarqInfo  *ulHarqInfo_p    = PNULL;
    UInt8 hpId = 0;
    /* TDD Config 0 Changes Start */
    UInt32 ulSubFrameOffSet = 0;
    /* Get the ulSubFrameOffSet from current subframe num */
    GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrameNum,ulSubFrameNum)
    /* TDD Config 0 Changes End */

    ulHarqInfo_p = &ulUEContext_p->ulresInfo[0];
    switch(currentConfigIndex_g[internalCellIndex])
/** CA-TDD Changes End **/
    {

        case UL_DL_CONFIG_0:
        {
            /* TDD Config 0,6 Changes Start */
            /* SPR 11257 Changes Start*/
            hpId = ulHarqMapTddConfig0_g[(currentGlobalTTI + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG0_SIZE];
            /* SPR 11257 Changes End*/
            if (NA == hpId)
            {
                return BUSY_ALL_HARQ;
            }
            
            if(HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
            /* TDD Config 6 Changes End */
        }
        break;

        case UL_DL_CONFIG_1:
        {
            if ((HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
               )
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
        }
        break;
        case UL_DL_CONFIG_2:
        {
            if ((HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
               )
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
        }
        break;
        case UL_DL_CONFIG_3:
        {
            if ((HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
               )
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
        }
        break;
        case UL_DL_CONFIG_4:
        {
            if ((HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
                    ||(HARQ_PROCESS_FREE == ulHarqInfo_p[++hpId].harqStatus)
               )
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
        }
        break;
        case UL_DL_CONFIG_5:
        {
            if (HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
        }
        break;

        case UL_DL_CONFIG_6:
        {
            /* TDD Config 6 Changes Start */
            /* SPR 11257 Changes Start*/
            hpId = ulHarqMapTddConfig6_g[(currentGlobalTTI + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG6_SIZE];
            /* SPR 11257 Changes End*/
            if (NA == hpId)
            {
                return BUSY_ALL_HARQ;
            }
            
            if(HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
            {
                return hpId;
            }
            else
            {
                return BUSY_ALL_HARQ;
            }
            /* TDD Config 6 Changes End */
        }
        break;

        default:
        {
            ltePanic("Invalid Configuration Recieved\n");

        }
    }
    return BUSY_ALL_HARQ;
}
/* SPR 3114 End */
/*****************************************************************************
 * Function Name  : checkForFreeHarqProcess
 * Inputs         : ueIndex - The UE Index for which the HARQ process is required.
 *                  subFrameNum - dl subframe number.
 *                  ulSubFrameNum - uplink subframe Number.
 *                  sysFrameNum - system Frame Number.
 * Returns        : return Free Harq Process ID. 
 * Description    : This function checks for any free Harq Process within a
 *                  UE Context.
 *****************************************************************************/
/*  TDD REL 3.1.1 */
/** CA-TDD Changes Start **/
/* TDD Config 0 6 Changes Start */
STATIC  UInt8 checkForFreeULHarqProcess( 
                    ULUEContext *ulUEContext_p, 
                    UInt8 subFrame,
                    InternalCellIndex internalCellIndex,
                    UInt8 ulSubFrameNum,
                    /* SPR 11257 Changes Start*/
                    /* +- SPR 17777 */
                    /*SPR 15909 fix start*/
                    tickType_t currentGlobalTTITickCount)
/*SPR 15909 fix end*/
/* SPR 11257 Changes End*/
/* TDD Config 0 6 Changes End */
/** CA-TDD Changes End **/
/*  TDD REL 3.1.1 */
{

    ULHarqInfo  *ulHarqInfo_p    = PNULL;
    UInt8 hpId = 0;
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);

    MAC_ASSERT( ulUEContext_p );

    /* TDD Config 0 Changes Start */
    UInt32 ulSubFrameOffSet = 0;
    /* Get the ulSubFrameOffSet from current subframe num */
    GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrame,ulSubFrameNum)
    /* TDD Config 0 Changes End */

    ulHarqInfo_p = &ulUEContext_p->ulresInfo[0];
    /*
       TDD UL/DL configuration      Max. Number of HARQ processes
       0                               7
       1                               4
       2                               2
       3                               3
       4                               2
       5                               1
       6                               6
     */
    /*  TDD REL 3.1.1 */
/** CA-TDD Changes Start **/
    switch(currentConfigIndex_g[internalCellIndex])
/** CA-TDD Changes End **/
    {

        case UL_DL_CONFIG_0:
            {
                /* TDD Config 0 Changes Start */
                /* SPR 11257 Changes Start*/
                hpId = ulHarqMapTddConfig0_g[((currentGlobalTTITickCount + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG0_SIZE)];
                /* SPR 11257 Changes End*/
                if (NA == hpId)
                {
                    return BUSY_ALL_HARQ;
                }

                /* TDD Config 0 Changes End */
            }
            break;

        case UL_DL_CONFIG_1:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_7 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_8 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                else if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 2;
                }
                else  if(SUB_FRAME_3 == ulSubFrameNum)
                {
                    hpId = 3;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_2:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_7 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_3:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_4 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                else if(SUB_FRAME_3 == ulSubFrameNum)
                {
                    hpId = 2;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }

            }
            break;
        case UL_DL_CONFIG_4:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_3 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
                    
            }
            break;
        case UL_DL_CONFIG_5:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }

            }
            break;

        case UL_DL_CONFIG_6:
            {
                /* TDD Config 0 6 Changes Start */
                /* SPR 11257 Changes Start*/
                hpId = ulHarqMapTddConfig6_g[((currentGlobalTTITickCount + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG6_SIZE)];
                /* SPR 11257 Changes End*/
                if (NA == hpId)
                {
                    return BUSY_ALL_HARQ;
                }
 
                /* TDD Config 0 6 Changes End */
            }
            break;

        default:
            {
                ltePanic("Invalid Configuration Recieved\n");
                
            }
           
    }
    /*540 cyclomatic changes start */
    if(HARQ_PROCESS_FREE == ulHarqInfo_p[hpId].harqStatus)
    {
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"UlHarqID[%d]\n",
                hpId);
        return hpId;
    }
    else
    {
    return BUSY_ALL_HARQ;
    }
    /*540 cyclomatic changes end */
    /*  TDD REL 3.1.1 */
}
 UInt8 getULHarqId( UInt8 subFrame,
        UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    /* TDD Config 0 Changes Start */
    UInt8 ulSubFrameNum = 0;
    UInt32 ulSubFrameOffSet = 0;
    /* TDD Config 0 Changes End */
    UInt8 hpId = 0;
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);

    /* TDD Config 0 Changes Start */
    ulSubFrameNum = (subFrame + getExactULSubFrame(subFrame,internalCellIndex))% MAX_SUBFRAME; 
    /* TDD Config 0 Changes End */

    /*
       TDD UL/DL configuration      Max. Number of HARQ processes
       0                               7
       1                               4
       2                               2
       3                               3
       4                               2
       5                               1
       6                               6
     */
    /*  TDD REL 3.1.1 */
    switch(currentConfigIndex_g[internalCellIndex])
    {
        case UL_DL_CONFIG_0:
            {
                /* TDD Config 0 Changes Start */
                /* Get the ulSubFrameOffSet from current subframe num */
                GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrame,ulSubFrameNum)
                /* SPR 11257 Changes Start*/
                hpId = ulHarqMapTddConfig0_g[(((getCurrentTick() + delayToPhy) + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG0_SIZE)];
                /* SPR 11257 Changes End*/
                if (NA == hpId)
                {
                    return BUSY_ALL_HARQ;
                }

                /* TDD Config 0 Changes End */
            }
            break;

        case UL_DL_CONFIG_1:
            {
                if(1 == subFrame)
                {
                    hpId = 0;
                }
                else if(4 == subFrame)
                {
                    hpId = 1;
                }
                else if(6 == subFrame)
                {
                    hpId = 2;
                }
                else  if(9 == subFrame)
                {
                    hpId = 3;
                }
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_2:
            {
                /* if (!ulHarqInfo_p[hpId].riLength
                   ||!ulHarqInfo_p[++hpId].riLength
                   )*/
                if(3 == subFrame)
                {
                    hpId = 0;
                }
                else if(8 == subFrame)
                {
                    hpId = 1;
                }
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_3:
            {
                /* if (!ulHarqInfo_p[hpId].riLength
                   ||!ulHarqInfo_p[++hpId].riLength
                   ||!ulHarqInfo_p[++hpId].riLength*
                   )*/
                if(0 == subFrame)
                {
                    hpId = 0;
                }
                else if(8 == subFrame)
                {
                    hpId = 1;
                }
                else if(9 == subFrame)
                {
                    hpId = 2;
                }
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_4:
            {
                /*   if (!ulHarqInfo_p[hpId].riLength
                     ||!ulHarqInfo_p[++hpId].riLength
                     )*/
                if(8 == subFrame)
                {
                    hpId = 0;
                }
                else if(9 == subFrame)
                {
                    hpId = 1;
                }
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_5:
            {
              /*  if (!ulHarqInfo_p[hpId].riLength)*/
                if(8 == subFrame)
                {
                    hpId = 0;
                }
                else
                {
                    return BUSY_ALL_HARQ;
                }
            }
            break;
        case UL_DL_CONFIG_6:
            {
                /* TDD Config 0 6 Changes Start */
                /* Get the ulSubFrameOffSet from current subframe num */
                GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrame,ulSubFrameNum)
                /* SPR 11257 Changes Start*/
                hpId = ulHarqMapTddConfig6_g[(((getCurrentTick() + delayToPhy) + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG6_SIZE)];
                /* SPR 11257 Changes End*/
                if (NA == hpId)
                {
                    return BUSY_ALL_HARQ;
                }
 
                /* TDD Config 0 6 Changes End */
            }
            break;
       }
       return hpId;
}
/* - SPS_TDD_CHANGES */

/*****************************************************************************
 * Function Name  : setHarqBusy
 * Inputs         : subframeNum - This is UL subframe number.
 *                  harqId - UL harq Process Id.
 *                  ulUEContext_p - Pointer of UL UE Context
 *                  sysFrameNum - This is UL system Frame Number.
 *
 * Returns        : MAC_FAILURE or MAC_SUCCESS
 * Description    : This function will set the Harq is busy for this subframe.

 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 setHarqBusy( UInt8 subframeNum, 
                          UInt16 harqId, 
                          ULUEContext *ulUEContext_p,
                          InternalCellIndex internalCellIndex,
                          UInt16 sysFrameNum
                        )
/** CA-TDD Changes End **/
{
    /* TDD Config 6 Changes Start */
    UInt8 index = MODULO_TWO(sysFrameNum);

    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);
    
    LTE_MAC_UT_LOG(LOG_CRITICAL,TDD_CONFIGURATION,"subFrame[%d], harqId [%d]\
                   Valid [%d]",
                   subframeNum,
                   harqId,
                   ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum]);
    MAC_ASSERT( (subframeNum < MAX_SUB_FRAME)
/** CA-TDD Changes Start **/
                && (UL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame));
/** CA-TDD Changes End **/
   
    if ( INVALID_HARQ == harqId )
    {
        /* +- SPR 17777 */
         ltePanic("Invalid Harq Recieved UL HARQ ID [%d] internalCellIndex [%u]\n", harqId,internalCellIndex);
         /* coverity 56654 28JUNE2014 */
         return MAC_FAILURE;
         /* coverity 56654 28JUNE2014 */

    } 
    if (INVALID_HARQ == ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum])
    {
        ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum] = harqId;
        /* CRC_ULSCH_IND SPR 8722 Fix Start */
        ulUEContext_p->ulresInfo[harqId].crcUlschIndFlag = 0;
        /* CRC_ULSCH_IND SPR 8722 Fix End */
    }
    else
    {
        ltePanic("Harq Id is already busy for this subframe [%d] HpId [%d]\n",
            subframeNum,
            ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum]);
        return MAC_FAILURE;
    }
    /* TDD Config 6 Changes End */
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : resetHarqId
 * Inputs         : subframeNum - This is UL subframe number.
 *                  ulUEContext_p - Pointer of UL UE Context
 *                  sysFrameNum - This is UL system Frame Number.
 *
 * Returns        : None
 * Description    : This function will reset the UL Harq process Id for this 
 *                  subframe.

 *****************************************************************************/
/** CA-TDD Changes Start **/
 void resetHarqId( UInt8 subframeNum, 
                         ULUEContext *ulUEContext_p,
                         InternalCellIndex internalCellIndex, 
                         UInt16 sysFrameNum
                         )
/** CA-TDD Changes End **/
{
    /* TDD Config 6 Changes Start */
    UInt8 index = MODULO_TWO(sysFrameNum);

    MAC_ASSERT( (subframeNum < MAX_SUB_FRAME) &&
/** CA-TDD Changes Start **/
                (UL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame));                   
/** CA-TDD Changes End **/
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
    if (INVALID_HARQ == 
           ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum])
    {
        /*  TDD REL 3.1.1 */
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit",__func__);
       /*  TDD REL 3.1.1 */
    }
    else
    {
        ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum] =
                                                              INVALID_HARQ;
    }

    return;
    /* TDD Config 6 Changes End */
}

/*****************************************************************************
 * Function Name  : getULHarqIDFromSubFrame
 * Inputs         : subframeNum   - This is UL subframe number.
 *                  ulUEContext_p - Pointer of UL UE Context.
 *                  sysFrameNum   - This is UL System Frame Number.
 *
 * Returns        : INVALID_HARQ or Valid UL Harq Process Id
 * Description    : This function will return the invalid Harq ID or valid 
 *                  UL Harq Process ID.

 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getULHarqIDFromSubFrame ( UInt8 subframeNum,
                                       ULUEContext *ulUEContext_p,
                                       InternalCellIndex internalCellIndex,
                                       UInt16 sysFrameNum
                                     )
/** CA-TDD Changes End **/
{
    /* TDD Config 6 Changes Start */
    UInt8 index = MODULO_TWO(sysFrameNum);

    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);

    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"subFrame[%d], ULharqID [%d]\n",
            subframeNum,
            ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum]);
    MAC_ASSERT( (subframeNum < MAX_SUBFRAME) && 
/** CA-TDD Changes Start **/
                (UL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame));                   
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
/** CA-TDD Changes End **/
    if (INVALID_HARQ == ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][ subframeNum])
    {
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit",__func__);
        return INVALID_HARQ;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit. ",__func__);
        return (ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][subframeNum]);
    }
    /* TDD Config 6 Changes End */
}

/* msg3_retx_tdd_support_start */
/*****************************************************************************
 * Function Name  : getULHarqIDForMsg3Tdd
 * Inputs         : subframeNum  - This is present/current subframe number.
 *                  sysFrameNum - This represnt the current SFN
 *                  ulSubFrameNum - This reprsent the ulSubFrameNum for which 
                    grant is being given in current subframe number

 *
 * Returns        : INVALID_HARQ or Valid UL Harq Process Id
 * Description    : This function will return the invalid Harq ID or valid 
 *                  UL Harq Process ID.

 *****************************************************************************/
 UInt8 getULHarqIDForMsg3Tdd (
/* TDD Config 0 Chnages Strat */
    /* SPR 11257 Changes Start*/
    UInt8 subFrameNum,
    UInt8 ulSubFrameNum,
    /* SPR 15909 fix start */
    tickType_t globalTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex)
    /* SPR 11257 Changes End*/
/* TDD Config 0 Changes End */
{
    UInt8 hpId = 0;
    /* TDD Config 0 Changes Start */
    UInt32 ulSubFrameOffSet = 0;
    /* Get the ulSubFrameOffSet from current subframe num */
    GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrameNum,ulSubFrameNum)
    /* TDD Config 0 Changes End */
    
    switch(currentConfigIndex_g[internalCellIndex])
    {
        case UL_DL_CONFIG_0:
            {
                /* TDD Config 0 Changes Start */
                /* SPR 11257 Changes Start*/
                hpId = ulHarqMapTddConfig0_g[(globalTick + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG0_SIZE];
                /* SPR 11257 Changes End*/
                if (NA == hpId)
                {  
                    return BUSY_ALL_HARQ;
                }
                return hpId;
                /* TDD Config 0 Changes End */
            }
            break;

        case UL_DL_CONFIG_1:
            {
                /* TDD Config 0 Changes Start */
                if(SUB_FRAME_7 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_8 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                else if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 2;
                }
                else  if(SUB_FRAME_3 == ulSubFrameNum)
                {
                    hpId = 3;
                }
                else
                {
                    return BUSY_ALL_HARQ;
                }
                /* TDD Config 0 Changes end */
                return hpId;
            }
            break;
        case UL_DL_CONFIG_2:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_7 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
                return hpId; 
            }
            break;
        case UL_DL_CONFIG_3:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_4 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                else if(SUB_FRAME_3 == ulSubFrameNum)
                {
                    hpId = 2;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
                return hpId; 

            }
            break;
        case UL_DL_CONFIG_4:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                else if(SUB_FRAME_3 == ulSubFrameNum)
                {
                    hpId = 1;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
                return hpId; 

            }
            break;
        case UL_DL_CONFIG_5:
            {
                /* TDD Config 0 Changes start */
                if(SUB_FRAME_2 == ulSubFrameNum)
                {
                    hpId = 0;
                }
                /* TDD Config 0 Changes end */
                else
                {
                    return BUSY_ALL_HARQ;
                }
                return hpId; 

            }
            break;

        case UL_DL_CONFIG_6:
            {
                /* TDD Config 0 6 Changes Start */
                /* SPR 11257 Changes Start*/
                hpId = ulHarqMapTddConfig6_g[(globalTick + ulSubFrameOffSet) % MAX_HARQ_MAP_CONFIG6_SIZE];
                /* SPR 11257 Changes End*/
                if (NA == hpId)
                {
                    return BUSY_ALL_HARQ;
                }
                return hpId; 
                /* TDD Config 0 6 Changes End */
            }
            break;

        default:
            {
                ltePanic("Invalid Configuration Recieved\n");

            }
    }
    return BUSY_ALL_HARQ;
}

/* msg3_retx_tdd_support_end */
/*****************************************************************************
 * Function Name  : setCCEIndexInULContex 
 * Inputs         : ueIndex - It is the index of UE.
 *                  subframeNum  - This is present/current subframe number.
 *                  cceIndex - Value of CCE Index.
 *
 * Returns        : MAC_FAILURE or MAC_SUCCESS
 * Description    : Assume that previous function check that UE Context is exit
 *                  and pending delete flag is not set.
 *                  This function will insert the CCE Index in the container.
 *
*****************************************************************************/
 MacRetType setCCEIndexInULContex( UInt16 ueIndex, 
                                         UInt8 subframeNum, 
                                         UInt8 cceIndex,
                                         /** SPR 14204 Fix : Start **/
                                         InternalCellIndex internalCellIndex
                                         /** SPR 14204 Fix : End **/
                                         /* SPR 19679: TDD HARQ Multiplexing
                                          * Changes Start */
                                         ,UInt8 isTBOneValid
                                         ,UInt8 isTBTwoValid
                                         /* SPR 19679: TDD HARQ Multiplexing
                                          * Changes End */
                                       )
{
    UeScheduledInfo *ueShedInfo_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Entry. ",__func__);
    
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"ueIndex [%d] subframeNum[%d],\
                                               cceIndex[%d]\n",
                                               ueIndex, subframeNum, cceIndex);

    MAC_ASSERT( subframeNum < MAX_SUB_FRAME);

    /*SPR 1024 changes start*/
    if(MAX_UE_SUPPORTED > ueIndex)
    {    
        /*SPR 1024 changes end*/
        /** SPR 14204 Fix : Start **/
        ueShedInfo_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->
            dlUeScheduledInfoPointerArray[internalCellIndex][subframeNum]; 
        /** SPR 14204 Fix : End **/
        /*SPR 1024 changes start*/
    }
    else
    {
        return MAC_FAILURE;
    }    
    /*SPR 1024 changes end*/

    if ( PNULL == ueShedInfo_p)
    {
       ltePanic(" ueShedInfo_p is Null\n");
    }
    
    ueShedInfo_p->dlCceIndex[subframeNum] = cceIndex;
    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */

    if (isTBOneValid)
    {
        ueShedInfo_p->harqSizeForMuxMOneInMIMO[subframeNum]++;
    }    
    if (isTBTwoValid)
    {
        ueShedInfo_p->harqSizeForMuxMOneInMIMO[subframeNum]++;
    }    
    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
     if (ueShedInfo_p->ueInsertedInPdcchDlUeInfo)
     {
         ueShedInfo_p->ueInsertedInPdcchDlUeInfo++;
         LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"ueInsertdFlag [%d] [%s] Entry. ",
                            ueShedInfo_p->ueInsertedInPdcchDlUeInfo,__func__);
         return MAC_FAILURE;
     }
     else
     {
        ueShedInfo_p->ueInsertedInPdcchDlUeInfo = 1;
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"ueInsertdFlag [%d] [%s] Entry. ",
                            ueShedInfo_p->ueInsertedInPdcchDlUeInfo,__func__);
        return MAC_SUCCESS;
     }
}

/*****************************************************************************
 * Function Name  : isValidSubframeNumForDCI_0
 * Inputs         : subframeNum  - This is present/current subframe number.
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will check that it is valid subframe
 *                  to send DCI-0.

 *****************************************************************************/
/** CA-TDD Changes Start **/
 MacRetType isValidSubframeNumForDCI_0 ( UInt8 subframeNum, InternalCellIndex internalCellIndex)
{  
   MAC_ASSERT( subframeNum < MAX_SUB_FRAME);

   if  (DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame
        || SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame )                    
/** CA-TDD Changes End **/
   {
      return MAC_SUCCESS;
   }
   else
   {
      return MAC_FAILURE;
   }
}

/*****************************************************************************
 * Function Name  : isValidULSubFrameForHIPdu
 * Inputs         : subframeNum  - This is present/current subframe number.
 *                  *hiPduSubFrameNum - to fill the HI Pdu subframe number 
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will fill the HI Pdu subframe number.

 *****************************************************************************/
/** CA-TDD Changes Start **/
 MacRetType isValidULSubFrameForHIPdu ( UInt8 subframeNum, 
                                              UInt8 *hiPduSubFrameNum,
                                              InternalCellIndex internalCellIndex 
                                            )
{  
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] subframeNum [%d] Entry. ",
                    __func__, subframeNum);
    MAC_ASSERT( subframeNum < MAX_SUB_FRAME);

    if (UL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame)
    {
        *hiPduSubFrameNum = ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].
                           ulGrantPhichInfo.ulPHICHSubFrameNum;
/** CA-TDD Changes End **/
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"hiPduSubFrameNum [%d]\n",
                                   *hiPduSubFrameNum);
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}    

/*****************************************************************************
 * Function Name  : ulHarqTimerStart 
 * Inputs         : sysFrameNum - Current System Frame number
 *                  subframe -    Current Subframe Number
 *                  ueIndex  - Identifer of the UE 
 *                  harqProcessId - Send DL data for this harq Id.
 *                  ulSubFrame - UL Subframe w.r.t the DL subframe
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will start the ulHarqTimer and 
 *                  store the context in particular sturctures.

 *****************************************************************************/
 MacRetType ulHarqTimerStart( UInt8 subframe, 
                                    UInt16 sysFrameNum,
                                    UInt16 ueIndex,
                                    UInt8 harqProcessId,
                                    /* + SPS_TDD_Changes */
                                    UInt8 schUsed,
                                    /* - SPS_TDD_Changes */
                                    /* CA Code Change */
                                    InternalCellIndex internalCellIndex,
                                    /* CA Code Change */
                                    /*TDD Config 0 Changes Start*/
                                    UInt8 ulSubFrame
                                    /*TDD Config 0 changes End*/
                                  )
{
    if ( ((subframe > MAX_SUB_FRAME) || (ulSubFrame > MAX_SUB_FRAME)) 
			|| (sysFrameNum > MAX_SYS_FRAME_NUM))
    {
        lteWarning("\nIn function : [%s] , Invalid Subframe or SystemFrame Number\n \
				SF[%d] SFN[%d] U-SF[%d]\n", __func__, subframe, sysFrameNum, ulSubFrame);
        return MAC_FAILURE;
    }

    UInt8 ulHarqEntityTimerArrIndex = 0;
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode*       ulharqTimerNode_p       = PNULL;
    ULUEContext* ulUEContext_p         = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL; 
    UInt8 index       = 0;
    UInt8 ulSfnFactor = 0;

    LTE_MAC_UT_LOG(LOG_INFO, TDD_CONFIGURATION,
            "[%s] sysFrameNum[%d] subframe[%d] harqProcessId[%d] Entry",
            __func__, sysFrameNum, subframe, harqProcessId);
    
    /*
       In this function we do not impose an extra check on the harqProcessId for its
       value as it is being assumed that the corresponding value has been received
       from the checkFreeDLHarqProcess ( ).
       */
    ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
    ulSfnFactor = getUlSfnFactor(subframe,
			/*TDD Config 0 Changes Start*/
			 ulSubFrame);
			/*TDD Config 0 Changes End*/

    /*toggle the container index based on the ulSfnFactor */
    /*TDD Config 0 Changes Start*/
    index       = MODULO_TWO( sysFrameNum + ulSfnFactor);
    
     LTE_MAC_UT_LOG( LOG_MINOR, TDD_CONFIGURATION,
     " hPId[%d] sf[%d] ueIdx[%d] ulSubFrame[%d] indexSfn[%d] sysFrameNum[%d]\n",
                harqProcessId,  subframe, ueIndex, ulSubFrame, index, sysFrameNum);
    /*CA TDD CHANGES START*/  
    ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[internalCellIndex][index][ulSubFrame];
    /*CA TDD CHANGES END*/
    /*TDD Config 0 Changes End*/

    /*
        NOTE:- The value of ulHarqEntityTimerList_p->count was initialised to 0
             (invalid value) and hence we have to increase it after it is used as
              array index, on the first occasion on the other occasion it just gives
             the proper array index.
    */

    ulHarqEntityTimerArrIndex = ulHarqEntityTimerList_p->count;

    ulharqTimerNode_p = \
        &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqEntityTimerArrIndex]);

    ulharqTimerNode_p->ueIndex = ueIndex;
    ulharqTimerNode_p->harqProcessId = harqProcessId;
    /* + SPS_TDD_Changes */
    ulharqTimerNode_p->schUsed = schUsed;
    /* - SPS_TDD_Changes */
    ulharqTimerNode_p->isValid = UL_HARQ_TIMER_NODE_VALID;

    /*TDD Config 0 Changes Start*/
    ulUEContext_p->ulHarqTimerNodeArrrayIndex[index][ulSubFrame] =
                                           ulHarqEntityTimerArrIndex;
    
    LOG_MAC_MSG(MAC_TDD_UL_HARQ_TIMER_START_ID,LOGBRIEF,MAC_UL_HARQ,\
        getCurrentTick(),ueIndex,harqProcessId,ulSubFrame,\
        0,0,0.0,0.0,\
        __func__,"UL_HARQ_TIMER_START");
    /*TDD Config 0 Changes End*/

    ulHarqEntityTimerList_p->count++;

    LTE_MAC_UT_LOG(LOG_INFO, TDD_CONFIGURATION,"[%s] Exit",__func__);
    return MAC_SUCCESS;

}
/* msg3_retx_tdd_support_start */
/*****************************************************************************
 * Function Name  : expireAllNodeFromULHarqTimerForMsg3List 
 * Inputs         : recvTTI - current tick
 *                  sfn     - current sfn
 *                  prevULSubFrame - previous ul subframe 
 *                  ulSfnFactor - used for container
 *
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will expire the UL harq timer node if it
 *                  present then check that if it is first or last 
 *                  retransmission then push into ul container otherwise
 *                  just increment the counter.
 *****************************************************************************/
MacRetType expireAllNodeFromULHarqTimerForMsg3List(
        UInt8 ulHarqTimerSysFrameIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        RbMapNode *rbMapNode_p,
        UInt8 freeHarqOnly,
        InternalCellIndex internalCellIndex)
{
    UInt16 ulHarqEntityTimerArrCount = 0;        
    ULHarqTimerEntityForMsg3List* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerForMsg3Node* ulharqTimerNode_p = PNULL;
    UInt16 ueIndex = INVALID_UE_ID;
    ULHarqInfo* ulHarqProcess_p = PNULL;
    UInt32 tickDiff = 0;
    TempCRNTICtx * tempCRNTICtx_p = PNULL;
    ContainerNackQueueNode *nackNode_p = PNULL;
    UplinkContainer *currentContainer_p = PNULL;
    UInt8 frameNumber = recvTTI % MAX_SUB_FRAME;
 /* SPR 20592 Fix Start */
    UInt8 ulContainerIndex = 0;
 /* SPR 20592 Fix End */
    /* CA TDD Changes Start */
    ulHarqEntityTimerList_p = &ulHarqTimerEntityForMsg3List_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];    
    /* CA TDD Changes End */
    ulHarqEntityTimerArrCount = ulHarqEntityTimerList_p->count;

    if (!ulHarqEntityTimerArrCount)
    {
        return MAC_FAILURE;
    }
    while (ulHarqEntityTimerArrCount--)
    {
        ulharqTimerNode_p = 
            &(ulHarqEntityTimerList_p->ulHarqForMsg3TimerNodeArr[ulHarqEntityTimerArrCount]);    

        if (ulharqTimerNode_p->isValid)
        {
            ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID; 
            ueIndex = ulharqTimerNode_p->ueIndex;
            tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if (PNULL != tempCRNTICtx_p)
            {
                ulHarqProcess_p = &(tempCRNTICtx_p->ulHarq);
                if(ulharqTimerNode_p->ulHarqExpiredSubframe != frameNumber)
                {

                    LOG_MAC_MSG(MAC_EXPIRY_SFMISMATCH_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                            currentGlobalTick,\
                            ulharqTimerNode_p->ulHarqExpiredSubframe,\
                            frameNumber,\
                            ulharqTimerNode_p->ueIndex,\
                            ulharqTimerNode_p->harqProcessId,\
                            ulharqTimerNode_p->ulHarqExpiredTTI,\
                            0.0,0.0,__func__,"EXPIRY_SF_MISMATCH For MSG3");

                    continue;
                }

                /* Get the Tick diff to know about Tick Jump */
                tickDiff = recvTTI - ulharqTimerNode_p->ulHarqExpiredTTI;

                if(!freeHarqOnly)
                {
                    if(tickDiff == DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY)
                    {
                        if( HARQ_PROCESS_FREE != ulHarqProcess_p->harqStatus)
                        {
                            /* SPR 20592 Fix Start */
			    ulContainerIndex = (currentGlobalTick) % getMaxULCrcContainer(internalCellIndex); 
                            currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
                            /* SPR 20592 Fix End */
                            GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p,
                                    ContainerNackQueueNode);
                            /*+coverity-530-CID 25164 */
                            if(PNULL == nackNode_p)
                            {
                                continue;
                            }
                            /*+coverity-530-CID 25164 */
                            nackNode_p->ueIdx = ueIndex;
                            nackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;
                            nackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                            nackNode_p->ulSubFrame = (nackNode_p->ttiCounter + 
                                    getnextUlSfOffsetFromCurrentUlSf((nackNode_p->ttiCounter % MAX_SUB_FRAME),
                                    /* CA TDD CHANGES START */
                                    internalCellIndex
                                    /* CA TDD CHANGES END */
                                    )) % MAX_SUB_FRAME;

                            /*TDD Config 0 Changes Start*/
                            rbMapNode_p->totalRetxRb[CC_USER] +=
                                ulHarqProcess_p->riLength;
                            /*TDD Config 0 Changes End*/

#ifndef DL_UL_SPLIT
                            /*Cov-25408 fix start*/
                            if(pushNode(&(currentContainer_p->nackQueueForTempUE),&(nackNode_p->nackNodeAnchor)))
                            {
                                freeMemPool(nackNode_p);
                                return MAC_FAILURE;
                            }
                            /*Cov-25408 fix end*/
#else
#ifdef CIRC_QUEUE_IMPL
                            ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(currentContainer_p->nackCircQueueForTempUE),
                                    ContainerNackQueueNode,
                                    &(nackNode_p->nackNodeAnchor));
                            semPostDefault(&waitForULNackForTempUE_g);
#else
                            putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerNackQueueForTempUEIdUL, (nackNode_p));
#endif
#endif
                            /*TDD Config 0 Changes Start*/
                            /* SPR 5798 Fix Start */
                            rbMapNode_p->nackQueueCount++;
                            /* SPR 5798 Fix End */
                            /*TDD Config 0 Changes End*/

                        }
                    }
                }
                /*
                   We now fill the array ulHarqTimerNodeIndex with an invalid entry.
                 */
                tempCRNTICtx_p->ulHarqTimerNodeArrrayIndex = \
                                                             INVALID_COUNT;
                ulharqTimerNode_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;

                ulharqTimerNode_p->ueIndex = INVALID_UE_ID;

                ulharqTimerNode_p->harqProcessId = MAX_HARQ_PROCESS_NB;
            }
            else
            {
                continue;
            }
        }
    }
    /* SPR 11547 Fix Start*/
    ulHarqEntityTimerList_p->count=0;
    /* SPR 11547 Fix End*/
#ifdef DL_UL_SPLIT
    UInt32 nackQContainerTick = MODULO_EIGHT(currentGlobalTick + PHY_DL_DELAY - FDD_HARQ_OFFSET);
    UplinkContainer* ulContainerQueue_p = &ulContainerPool_gp[nackQContainerTick];
    ContainerNackQueueNode *delimiterNackNode_p ;
    GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(delimiterNackNode_p ,ContainerNackQueueNode);

    delimiterNackNode_p->ueIdx = INVALID_UE_ID; 
    delimiterNackNode_p->ttiCounter = 0;
    
#ifdef CIRC_QUEUE_IMPL
    ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(ulContainerQueue_p->nackCircQueueForTempUE),
            ContainerNackQueueNode,
            &(delimiterNackNode_p->nackNodeAnchor));
    semPostDefault(&waitForULNackForTempUE_g);
#else
    putEntryInULContainerNackQueueUsingICC(ulContainerQueue_p->ulContainerNackQueueForTempUEIdUL,delimiterNackNode_p);
#endif
#endif

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : expireAllNodeFromULHarqTimerList 
 * Inputs         : recvTTI - current tick
 *                  sfn     - current sfn
 *                  prevULSubFrame - previous ul subframe 
 *                  hiDLSF - used for container
 *                  cellIndex   
 *
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function will expire the UL harq timer node if it
 *                  present then check that if it is first or last 
 *                  retransmission then push into ul container otherwise
 *                  just increment the counter.
 *****************************************************************************/
/* SPR 15909 fix start */
MacRetType expireAllNodeFromULHarqTimerList( tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,
        UInt8  prevULSubFrame,
        UInt8  hiDLSF,
        UInt8 freeHarqOnly,
        /* SPR 11110 fix start */
        UInt8 allHarqFree,
        InternalCellIndex cellIndex )
{
    UInt8 index = 0;
    UInt8 ulSfnFactor = (hiDLSF > prevULSubFrame)?0:1;

    if( LTE_TRUE == allHarqFree )
    {
        for( index = 0; index < MAX_UL_HARQ_TIMER; index++ )
        {
            freeAllNodesForIndexFromULHarqTimerList( recvTTI,
                    index, prevULSubFrame, hiDLSF, freeHarqOnly, cellIndex );
        }
    }
    else
    {
        index = MODULO_TWO(sfn);

        /* Going to reteriving the exact index*/
        index = MODULO_TWO((index + ulSfnFactor));

        freeAllNodesForIndexFromULHarqTimerList( recvTTI,
                index, prevULSubFrame, hiDLSF, freeHarqOnly, cellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : freeAllNodesForIndexFromULHarqTimerList
 * Inputs         : recvTTI - current tick
 *                  sfn     - current sfn
 *                  prevULSubFrame - previous ul subframe
 *                  ulSfnFactor - used for container
 *
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will expire the UL harq timer node if it
 *                  present then check that if it is first or last
 *                  retransmission then push into ul container otherwise
 *                  just increment the counter.
 *****************************************************************************/
 /* SPR 15909 fix start */
MacRetType freeAllNodesForIndexFromULHarqTimerList( tickType_t recvTTI,
        UInt8  index,
        UInt8  prevULSubFrame,
        UInt8  hiDLSF,
        UInt8 freeHarqOnly,
        InternalCellIndex cellIndex )
/*SPR 15909 fix end*/
/* SPR 11110 fix end */
{
    MAC_ASSERT( prevULSubFrame < MAX_SUB_FRAME);
    UInt16 ulHarqEntityTimerArrCount = 0;
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerEntityList* ulHarqEntityTimerListAccess_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;
    ULUEContextInfo* ulUeContextInfo_p = PNULL;
    ULUEContext*   ulUeContext_p = PNULL;
    ULHarqInfo* ulHarqProcess_p = PNULL;
    ContainerNackQueueNode *nackNode_p = PNULL;
    UplinkContainer *currentContainer_p = PNULL;
    UInt16 ueIndex = INVALID_UE_ID;
    UInt8 tempHarqID = INVALID_HARQ_ID;
    UInt8 ulContainerIndex = 0;
    /* + SPS_TDD_Changes */
    UInt8 maxUlHARQTx = 0;
    /* - SPS_TDD_Changes */
    /*TDD Config 0 Changes Start*/
    UInt8 rbMapNum                  = (recvTTI + getUlSubframe(prevULSubFrame,hiDLSF,cellIndex))% getMaxULRBMaps(cellIndex);
    RbMapNode *rbMapNode_p = &rbMapNodeArr_gp[cellIndex][rbMapNum];
    if(FALSE == rbMapNode_p->advanceAllocFlag)
    {
        *rbMapNode_p = (ulRAInfo_gp[cellIndex] + 
                ulRAInfoIndex_g[cellIndex])->rbMapInitInfo;
        /* We have allocated PRACH Resource ony but we need to
         ** reserve advance PUCCH resources for this rbMap, thats why this value is set,
         ** which means, no need to do init now.
         **/
        rbMapNode_p->advanceAllocFlag = TDD_ONLY_INIT_DONE;
    }
    /*TDD Config 0 Changes End*/

    /*CA TDD CHANGES START*/
    ulHarqEntityTimerListAccess_p = &ulHarqTimerEntityList_g[cellIndex][index][0];
    /*CA TDD CHANGES END*/

    LTE_MAC_UT_LOG(LOG_INFO, TDD_CONFIGURATION,"[%s] Entry",__func__);
    /* msg3_retx_tdd_support_start */
    /* SPR 15909 fix start */
    tickType_t ulTTI = recvTTI - getHiPduSubframeNum(prevULSubFrame,cellIndex);
    /* SPR 15909 fix end */
    expireAllNodeFromULHarqTimerForMsg3List( index,ulTTI,
            /* SPR 11110 fix start */
            /* +- SPR 17777 */
            recvTTI, rbMapNode_p, freeHarqOnly,cellIndex );
    /* SPR 11110 fix end */
    /* msg3_retx_tdd_support_end */

    ulHarqEntityTimerList_p = &ulHarqEntityTimerListAccess_p[prevULSubFrame];
    ulHarqEntityTimerArrCount = ulHarqEntityTimerList_p->count;

    if (!ulHarqEntityTimerArrCount)
    {
        /*
           As there is no node in this Index of Array hence nothing is to be deleted
           remember the valid value of an array starts form zero and the above
           variable dlHarqEntityTimerList_p->count represents the array index.
         */
        LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] Exit",__func__);
        return MAC_SUCCESS;
    }
    /** SPR 14759 Changes Start **/
    while ((ulHarqEntityTimerArrCount <= MAX_UL_UE_SCHEDULED ) && (ulHarqEntityTimerArrCount--) )
    /** SPR 14759 Changes End **/
    {
        ulharqTimerNode_p = 
            &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqEntityTimerArrCount]);

        if (ulharqTimerNode_p->isValid)
        {
            ueIndex = ulharqTimerNode_p->ueIndex;
            ulUeContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            /*
               We ascertain whether th eUE under consideration is already on the
               process of deletion from the RRC
             */
            if (!ulUeContextInfo_p->pendingDeleteFlag)
            {
                ulUeContext_p = ulUeContextInfo_p->ulUEContext_p;
                ulharqTimerNode_p->isValid = UL_HARQ_TIMER_NODE_INVALID;
                tempHarqID = ulharqTimerNode_p->harqProcessId;
                /*
                   We now fill the array dlHarqTimerNodeIndex with an invalid entry.
                 */
                ulHarqProcess_p = &(ulUeContext_p->ulresInfo[tempHarqID]);
                
                if(!freeHarqOnly)
                {
                    ulContainerIndex = (recvTTI)% getMaxULCrcContainer(cellIndex); 

                    currentContainer_p = ulContainerPool_gp[cellIndex] + ulContainerIndex; 

                    nackNode_p = (ContainerNackQueueNode *)
                        getMemFromPool(sizeof(ContainerNackQueueNode), PNULL);
                    if (nackNode_p)
                    {
                        nackNode_p->ueIdx = ueIndex;
                        nackNode_p->ttiCounter = recvTTI - getHiPduSubframeNum(prevULSubFrame, cellIndex);
                        nackNode_p->ulSubFrame = (nackNode_p->ttiCounter + 
                                getnextUlSfOffsetFromCurrentUlSf(prevULSubFrame, cellIndex)) 
                            % MAX_SUB_FRAME;
                        nackNode_p->ulHarqProcessId = tempHarqID;
                        /* + SPS_TDD_Changes */
                        nackNode_p->schUsed =  ulharqTimerNode_p->schUsed;
                        if(SPS_SCHEDULING == nackNode_p->schUsed)
                        {
                            maxUlHARQTx = ulUeContext_p->spsUlInfo.maxSpsUlHARQTx -1;
                            /* coverity 64942 28June2014 */
                            if ( pushNode(&(currentContainer_p->nackQueue),
                                        &(nackNode_p->nackNodeAnchor)) )
                            {
                                freeMemPool(nackNode_p);
                                return MAC_FAILURE;
                            }
                            /* + SPS_TDD_Changes */
                            ulHarqProcess_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                            /* - SPS_TDD_Changes */
                            /* coverity 64942 28June2014 */
                        }
                        else
                        {
                            /* - SPS_TDD_Changes */
                            maxUlHARQTx = ulUeContext_p->maxUlHARQTx - 1 ;
                            /* coverity 64942 +-*/
                            if (pushNode(&(currentContainer_p->nackQueue),
                                    &(nackNode_p->nackNodeAnchor)))
                            {
                                freeMemPool(nackNode_p);
                                return MAC_FAILURE;
                            }
                            /* coverity 64942 +-*/
                            /* + SPS_TDD_Changes */
                            ulHarqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                            /* - SPS_TDD_Changes */
                        }

                        /* Perform adaptive re-transmission in case of timer 
                         * expiry to again send the DCi PDU */
                        /* + SPS_TDD */		
                        //          ulHarqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        /* - SPS_TDD */		

                        if ( ulHarqProcess_p->currentRetransCount 
                                /* + SPS_TDD */
                                < (maxUlHARQTx))
                            /* - SPS_TDD */
                        {
                            /* ICIC changes start */
                            /*TDD Config 0 Changes Start*/
                            rbMapNode_p->totalRetxRb[ulUeContext_p->userLocationType] +=
                                ulUeContext_p->ulresInfo[ulharqTimerNode_p->harqProcessId].riLength;
                            if (ulUeContext_p->userLocationType == CE_USER)
                            {
                                rbMapNode_p->totalRetxRb[CC_USER] +=
                                    ulUeContext_p->ulresInfo[ulharqTimerNode_p->harqProcessId].riLength;
                            }
                            /*TDD Config 0 Changes End*/
                        }
                        /* SPR 8188 Changes Starts */
                        /*TDD Config 0 Changes Start*/
                        rbMapNode_p->nackQueueCount++;
                        /*TDD Config 0 Changes End*/
                        /* SPR 8188 Changes Ends */

                        LOG_MAC_MSG(MAC_EXPIRE_UL_HARQ_TIMER_NODE_ID,LOGINFO,MAC_UL_HARQ,
                                recvTTI,ueIndex,nackNode_p->ttiCounter,tempHarqID,
                                0,0,0.0,0.0,__func__,"EXPIRE_UL_HARQ_TIMER_NODE");
                    }
                    else
                    {
                        ltePanic("getMemFromPool Failed");
                    }
                }
                else
                {
                    LOG_MAC_MSG( MAC_DELETE_ON_UL_HARQ_TIMER_EXPIRY, LOGDEBUG,
                            MAC_UL_HARQ, getCurrentTick(), __LINE__, ueIndex,
                            tempHarqID, prevULSubFrame, 0, 0,0,
                            __func__,"" );

                    /* SPR 11110 fix start */
                    /* Reset HARQ Procees Id in PuschSubFrameMAp */
                    ulUeContext_p->ulHarqProcessIdToPuschSubFrameMAp[index][prevULSubFrame] =
                        INVALID_HARQ;
                    /* SPR 11110 fix end */
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                    freeULHarqProcess( ulHarqProcess_p,ulUeContext_p->ueIndex,cellIndex);
#else
                    freeULHarqProcess( ulHarqProcess_p);
#endif
                    /* SPR 16916 FIX END */

                }
                /* SPR 5147 changes start */
                ulharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                ulharqTimerNode_p->ueIndex       = INVALID_UE_ID;
                /* SPR 5147 changes end */
            }
        }
    } 

    ulHarqEntityTimerList_p->count = 0;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : ulHarqTimerStop 
 * Inputs         : ulUEContext_p - Pointer of UL UeContext
 *                  recvTTI - Current TTI
 *                  subframe - Current Subframe number
 *                  ulSubFrame - Current UL Subframe
 *
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will stop the harq timer and delete 
 *                  the context & decrement the counter.
 *****************************************************************************/
 MacRetType ulHarqTimerStop(
        ULUEContext* ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t  recvTTI, 
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        /*TDD Config 0 Changes Start*/ 
        UInt8   ulSubframe
        /*TDD Config 0 Changes end*/
        )
{
    if(ulSubframe > MAX_SUB_FRAME)
    {	
        lteWarning("\nIn function : [%s] , Invalid Subframe Number, U-SF [%d] \n",\
                __func__, ulSubframe);
        return MAC_FAILURE;
    }
  
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;
    SInt8 ulHarqTimerNodeArrIndex = -1;
    UInt32 sysFrameNum    = (MODULO_ONEZEROTWOFOUR(recvTTI/MAX_SUB_FRAME));
    UInt8  index = 0;
    
    index = MODULO_TWO(sysFrameNum);
    
    /*TDD Config 0 Changes Start*/
    LTE_MAC_UT_LOG( LOG_INFO, TDD_CONFIGURATION,
            "\n %s indexSfn [%d] sysFrameNumber [%d] ulSubframe[%d] sysFrameNum[%d]\n",
            __func__,index, sysFrameNum, ulSubframe, sysFrameNum);

    /*CA TDD CHANGES START*/
    ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[internalCellIndex][index][ulSubframe];
    /*CA TDD CHANGES END*/
    /*TDD Config 0 Changes End*/

    if (!ulHarqEntityTimerList_p->count)
    {
        /*
           The count is Zero symbolysing that there is no nodes avaliable in the
           DL HARQ TIMER List Array.
           */
        return MAC_FAILURE;
    }

    /*TDD Config 0 Changes Start*/
    ulHarqTimerNodeArrIndex = ulUEContext_p->ulHarqTimerNodeArrrayIndex[index][ulSubframe];
    /*TDD Config 0 Changes End*/

    if (ulHarqTimerNodeArrIndex == UL_INVALID_COUNT)
    {
        return MAC_FAILURE;
    }
    ulharqTimerNode_p = &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqTimerNodeArrIndex]);
    if (ulharqTimerNode_p->isValid)
    {
        ulharqTimerNode_p->isValid = UL_HARQ_TIMER_NODE_INVALID;
        /* + SPS_TDD_Changes */
        ulharqTimerNode_p->schUsed= DRA_SCHEDULING;
        /* - SPS_TDD_Changes */
        /*
           We now fill the array dlHarqTimerNodeIndex with an invalid entry.
           */
        /*TDD Config 0 Changes Start*/
        ulUEContext_p->ulHarqTimerNodeArrrayIndex[index][ulSubframe] = UL_INVALID_COUNT;
        LTE_MAC_UT_LOG( LOG_INFO, TDD_CONFIGURATION, 
                "\nUL harq timer stop  ulSubframe[%d]\n", ulSubframe);
        /*TDD Config 0 Changes End*/
    }
    else
    {
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : getContainerIdxForDlTimer 
 * Inputs         : sysFrameNumber - current System frame number
 *                  subFrameNumber - current subframe number
 *                  sfnFactor - this factor is used for calcualting 
 *                              the conaitner index
 *                  *ackNackContainerIdx - to fill current container index.
 *                  *hpIdContainerIdx    - to fill previous container index. 
 *
 * Returns        : None
 * Description    : This function will return the current Container Index
 *                  & previous container index.

 *****************************************************************************/
 void getContainerIdxForDlTimer ( UInt16 sysFrameNumber,
                                        UInt8  subFrameNumber,
                                        UInt8  sfnFactor,
                                        UInt8 *ackNackContainerIdx,
                                        UInt8 *hpIdContainerIdx
                                        )
{
    MAC_ASSERT( ( subFrameNumber < MAX_SUB_FRAME)
                && (sysFrameNumber < MAX_SFN_VALUE));
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(subFrameNumber)
    UInt8 indexSfn = 0;   
    /* calculate the ackNack container idx using
       the sfnFactor and system frame number.
    */
    if ( sysFrameNumber > 1)
    {
        indexSfn = MODULO_TWO( (sysFrameNumber - sfnFactor));
    }
    else
    {
        indexSfn = MODULO_TWO ((MAX_SFN +  sysFrameNumber - sfnFactor));
    }

    (*ackNackContainerIdx) = indexSfn;

    (*hpIdContainerIdx) = MODULO_TWO(sysFrameNumber);
    
}

/*****************************************************************************
 * Function Name  : deleteEntryFromTimerList
 * Inputs         : dlUEContext_p  - Pointer to the DL UE Context.
 *                  containerIndex - 
 *                  currentULSubFrame -
 *
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This funciton will delete the entry from dl harq timer.

 *****************************************************************************/
 MacRetType deleteEntryFromTimerList(
                                 DLUEContext *dlUEContext_p,
                                 UInt8 containerIndex,
                                 UInt8 currentULSubFrame,
                                 InternalCellIndex internalCellIndex
                               )
{
    MAC_ASSERT( ( currentULSubFrame < MAX_SUB_FRAME)
                && (containerIndex < 2 ));
    TDDHarqTimerExpiryListNode * node_p = PNULL;
    /* SPR 6495 fix start */
	/* SPR 20636 Changes Start*/
    if( PNULL == dlUEContext_p )
    {
        LOG_MAC_MSG( MAC_UNABLE_STOP_DL_HARQ_TIMER, LOGWARNING ,MAC_DL_HARQ,
                getCurrentTick(), (ADDR)dlUEContext_p, containerIndex, 
                currentULSubFrame, 0, 0, 0,0, __func__, "NullPtr" );
        return MAC_FAILURE;
    }
	/* SPR 20636 Changes End*/
    /* SPR 6495 fix end */

    node_p = dlUEContext_p->
        tddHarqTimerExpiryNode[containerIndex][currentULSubFrame];
	/* SPR 20636 Changes Start*/
    if (!node_p)
    {
        LOG_MAC_MSG( MAC_UNABLE_STOP_DL_HARQ_TIMER, LOGWARNING ,MAC_DL_HARQ,
                getCurrentTick(), (ADDR)dlUEContext_p, containerIndex, 
                currentULSubFrame, 0, 0, 0,0, __func__, "ExpNodeNull" );
        return MAC_FAILURE;
    }
	/* SPR 20636 Changes End*/
    
    /* SPR 11230 fix start */
    /* In case error is received for the node, HARQ timer shall not be 
     * stopped */
	/* SPR 20636 Changes Start*/
    if( LTE_TRUE == node_p->isErrForNode )
    {
        /* Even if the error is there in one DL CONFIG, NACK will be
         * considered for all the packets corresponding to all D/S 
         * subframes and re-transmitions shall be performed for all
         * D/S subframes on timer expiry. */
        LOG_MAC_MSG( MAC_UNABLE_STOP_DL_HARQ_TIMER, LOGWARNING ,MAC_DL_HARQ,
                getCurrentTick(), (ADDR)dlUEContext_p, containerIndex, 
                currentULSubFrame, node_p->isErrForNode, 0, 0,0, __func__,
                "ErrRecvd" );
        return MAC_FAILURE;
    }
	/* SPR 20636 Changes End*/
    /* SPR 11230 fix end */

    /** CA-TDD Changes Start **/
    listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [currentULSubFrame].dlExpiryList_p),
                     &(node_p->nodeAnchor)) ;
    /** CA-TDD Changes End **/
    freeMemPool((void *)node_p);
    dlUEContext_p->
    tddHarqTimerExpiryNode[containerIndex][currentULSubFrame] = PNULL;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : getContainerIdxForTimerExpiry 
 * Inputs         : dlAckNackSubFrame - UL subframe frame number
 *                  sfn               - current system frame number
 *                  *containerIndex   - to fill the container index.
 *
 * Returns        : None
 * Description    : This function will fill the container index.

*****************************************************************************/
 void getContainerIdxForTimerExpiry( UInt8 dlAckNackSubFrame,
                                           UInt8 sfn,
                                           UInt8 *containerIndex
                                         )
{
    MAC_ASSERT( (dlAckNackSubFrame < MAX_SUB_FRAME)
               &&( sfn < MAX_SFN_VALUE ));
    UInt8 expectedIndexFactor = 0;
    /* Check whether the ack/nack is expired in the SFN in which
     * ack/nack is received or next System Frame.
     * Expected index factor will  be 0 if the ack/nack is 
     * expired in the system frame number else it will
     * be 1.
     */  
    expectedIndexFactor = (dlAckNackSubFrame + DL_HARQ_EXPIRY_TIME)/MAX_SUB_FRAME;
    /* Based on the expected index factor obtain the ack/nack container index.
     */ 
    *containerIndex     = MODULO_TWO(((MODULO_TWO((sfn)) + expectedIndexFactor)));

}

/* SPR 3114 Start */
/*****************************************************************************
 * Function Name  : getULHarqProcessIdForCurrentSubframeForMsg3
 * Inputs         : subframeNum - Current subframe number
 *                  *ulHarqProcessId - to fill the UL harq process id.
 *                  *ulUEContext_p - pointer to ul ue context.
 *                  sysFrameNum      - DL System Frame Number.
 * Returns        : None 
 * Description    : This function first check that any harq process id is 
 *                  busy for this Subframe. If it is free then check that 
 *                  any harq process id is free. If Harq Process Id is free
 *                  then fill this harq Process Id.

*****************************************************************************/
 void getULHarqProcessIdForCurrentSubframeForMsg3  ( UInt8  subframeNum,
                                                    UInt8  *ulHarqProcessId,   
                                                    ULUEContext *ulUEContext_p,
                                                    InternalCellIndex internalCellIndex,
                                                    UInt16 sysFrameNum,
                                                    /* TDD Config 6 Changes Start */
                                                    /* SPR 11257 Changes Start*/
                                                    UInt8 ulSubframeNum,
                                                    /*SPR 15909 fix start*/
                                                    tickType_t currentGlobalTTI
                                                    /*SPR 15909 fix end*/
                                                    /* SPR 11257 Changes End*/
                                                    /* TDD Config 6 Changes End */
                                                  )
                         

{
    /* First we calculate the UL Subframe(In which Subframe UL Data is coming) 
     * correspondence to this subframe then check any harq Process ID is 
     * already busy for this subframe.
     * INVALID_HARQ denotes that Harq Process Id is free for 
     * that UL subframe. 
     */
    /* TDD Config 6 Changes Start */
    UInt16 ulSysFrameNum = 0;
    if (ulSubframeNum > subframeNum)
    {
        ulSysFrameNum = sysFrameNum;
    }
    else
    {
        ulSysFrameNum = (sysFrameNum + 1) % MAX_SFN_VALUE;
    }

/** CA-TDD Changes Start:05 **/
    if ( INVALID_HARQ == getULHarqIDFromSubFrame (ulSubframeNum,
                ulUEContext_p,
                internalCellIndex,
                ulSysFrameNum) )
    {
        /* getFreeULHarqIdForMsg3 func returns the free harq process Id
         * if any otherwise it return BUSY_ALL_HARQ 255*/
        *ulHarqProcessId = checkForFreeULHarqProcessForMsg3(
                ulUEContext_p, 
                internalCellIndex,
                /* SPR 11257 Changes Start*/
                subframeNum,
                ulSubframeNum,
                currentGlobalTTI);
                /* SPR 11257 Changes End*/
    }
/** CA-TDD Changes End:05 **/
    /* TDD Config 6 Changes End */
}
/* SPR 3114 End */

/*****************************************************************************
 * Function Name  : resetHarqAndUpdateDeltaBsr
 * Inputs         : recvTTI - packet received TTI
 *                  *ulUEContext_p - pointer to ul ue context.
 * Returns        : Update Delta Bsr and return the reset HarqProcess ID
 * Description    : This function first check that any harq process id is
 *                  busy for this Subframe then stop the harq process id
 *                  and update detla bsr. 

*****************************************************************************/

/* SPR 15909 fix start */
 UInt8 resetHarqAndUpdateDeltaBsr( tickType_t recvTTI,
                                        /* SPR 15909 fix end */
                                        ULUEContext *ulUEContext_p,
                                        /* CA Changes Start */
                                        InternalCellIndex internalCellIndex
                                        /* CA Changes Start */
                                      )
{
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] recvTTI [%d] Entry. ",
            __func__, recvTTI);
    UInt8 subframeNum = 0;
    /* TDD Config 6 Changes Start */
    UInt16 sysFrameNum = 0;
    UInt8 ulHarqProcessId = 0; 

    GET_SFN_SF_FROM_TTI(recvTTI, sysFrameNum, subframeNum)
    ulHarqProcessId = 
        getULHarqIDFromSubFrame(
                (subframeNum), 
                ulUEContext_p,
                internalCellIndex,
                sysFrameNum);
    /* TDD Config 6 Changes End */
    /*coverity-530-32558*/
    /* SPR 8416 changes start */
    if ( INVALID_HARQ_ID == ulHarqProcessId )
    {
        return INVALID_HARQ_ID;
    }
    if( MAC_FAILURE == ulHarqTimerStop( ulUEContext_p, recvTTI,internalCellIndex,subframeNum) )
    {
        LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
               getCurrentTick(), __LINE__, ulUEContext_p->ueIndex, recvTTI,
               subframeNum, ulHarqProcessId, 0,0, __func__, "");
    }
    /* SPR 8416 changes end */
    /* SPR 3114 End */
    LTE_MAC_UT_LOG(LOG_INFO,TDD_CONFIGURATION,"[%s] recvTTI [%d] Exit. ",
            __func__, recvTTI);
    return ulHarqProcessId;               
}

/*****************************************************************************
 * Function Name  : getSubframeType
 * Inputs         : subframe - The UE Index for which the HARQ process is required.
 * Returns        : return Subframe Type - DL,UL,SP.
 * Description    : This function retrive the Subframe Type from structure
 *                  ulGrantPhichTableInfo_g for current UL-DL config and
 *                  input subframe number and return the same
 *****************************************************************************/
/* + Coverity 96932 */
 ULDLSPFrame getSubframeType( UInt8 subframe, 
        /* - Coverity 96932 */
        /* CA Changes Start */
        InternalCellIndex internalCellIndex
        /* CA Changes End */
        )
{
    return ulGrantPhichTableInfo_g[cellSpecificParams_g.cellConfigAndInitParams_p\
    [internalCellIndex]->cellParams_p->subFrameAssign][subframe].ulDlSpFrame;
}

/* EICIC - TDD +*/

/*****************************************************************************
 * Function Name  : getUplinkToGrantSubframeNumber
 * Inputs         : subframe
 *                  internalCellIndex
 * Returns        : Uplink grant 
 * Description    : This function provides uplink grant wrt to TDD Config and SF
 *****************************************************************************/
 UInt16 getUplinkToGrantSubframeNumber( UInt8 subframe, 
        InternalCellIndex internalCellIndex
        )
{
    UInt8 tddConfig = 0;
    tddConfig = getTddConfig(internalCellIndex);
	if (TOTAL_NUM_UL_DL_CONFIG <= tddConfig)
	{
	    return 0xFF;
	}
    return uplinkToGrantMap_g[tddConfig][subframe];

}
/* EICIC - TDD -*/

/*****************************************************************************
 * Function Name  : getDlAckNackOffset
 * Inputs         : subframeNum - This is subframe on which dl has been scheduled
 *
 * Returns        : offset in ticks from given dl subframe, where ack nack will
 *                  be received
 * Description    : This function wil return the downlink ACK/NACK SF.

 *****************************************************************************/
/** CA-TDD Changes Start:05 **/
 UInt8 getDlAckNackOffset( UInt8 subframeNum,InternalCellIndex internalCellIndex)
{
    /* Assumed that calling func will ensure that subframe is for DL */
    return dlAckNackSubFrameNumInfo_p[internalCellIndex][subframeNum].dlAckNackSubFrameNumber;
}
/** CA-TDD Changes End:05 **/

/*****************************************************************************
 * Function Name  : initTddConfigInfo
 * Inputs         : subFrameAssign,splSubFramePatterns 
 * Returns        : None 
 * Description    : Stores the number of schedulable D frames and U frames

 *****************************************************************************/
 void initTddConfigInfo(UInt8 subFrameAssign, 
        UInt8 splSubFramePatterns,
        InternalCellIndex internalCellIndex)
{

/* SPR 5333 changes start */
   /** CA-TDD Changes Start **/
   numOfPhichInSFPerUE_p[internalCellIndex] = numOfPhichInSFPerUE_g[subFrameAssign]; 
   /** CA-TDD Changes End **/
/* SPR 5333 changes end */
    switch(subFrameAssign)
    {
        case UL_DL_CONFIG_0:
        {
            /* TDD Config 0 Changes Start */
            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 6;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                    SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 2;

    /*+ coverity 24640 */
                STATIC TddTimerConvMap tempTddDlTimerConvMap =
    /*- coverity 24640 */
                {
                    2,
                    {
                        {{0, 5, 0, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 4, 9, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 8, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 2, 7, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 6, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 5, 0, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 4, 9, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 8, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 2, 7, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 6, 0, 0, 0, 0, 0, 0, 0}}
                    }
                };

                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap;
            }
            else
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 4;

    /*+ coverity 24640 */
                STATIC TddTimerConvMap tempTddDlTimerConvMap =
    /*- coverity 24640 */
                {
                    4,
                    {
                        {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}},   
                        {{0, 4, 5, 9, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 8, 9, 0, 0, 0, 0, 0}},
                        {{0, 2, 3, 7, 8, 0, 0, 0, 0, 0}},
                        {{0, 1, 2, 6, 7, 0, 0, 0, 0, 0}},
                        {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}},
                        {{0, 4, 5, 9, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 8, 9, 0, 0, 0, 0, 0}},
                        {{0, 2, 3, 7, 8, 0, 0, 0, 0, 0}},
                        {{0, 1, 2, 6, 7, 0, 0, 0, 0, 0}}
                    }
                };

                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
            } 

    /*+ coverity 24640 */
            STATIC TddTimerConvMap tempTddUlTimerConvMap =
    /*- coverity 24640 */
            {
                6,
                {
                    {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}},   
                    {{0, 4, 5, 9, 0, 0, 0, 0, 0, 0}},
                    {{0, 3, 4, 8, 9, 0, 0, 0, 0, 0}},
                    {{0, 2, 3, 7, 8, 0, 0, 0, 0, 0}},
                    {{0, 1, 2, 6, 7, 0, 0, 0, 0, 0}},
                    {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}},
                    {{0, 4, 5, 9, 0, 0, 0, 0, 0, 0}},
                    {{0, 3, 4, 8, 9, 0, 0, 0, 0, 0}},
                    {{0, 2, 3, 7, 8, 0, 0, 0, 0, 0}},
                    {{0, 1, 2, 6, 7, 0, 0, 0, 0, 0}}
                }
            };
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
            /* TDD Config 0 Changes End */
        }
        break;

        case UL_DL_CONFIG_1:
        {
            /** CA-TDD Changes Start **/
            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 4;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                   SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 4;
                TddTimerConvMap tempTddDlTimerConvMap =
            {
                    4,
                    {
                        {{0, 4, 5, 9, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 8, 0, 0, 0, 0, 0, 0}},
                        {{0, 2, 3, 7, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 2, 6, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}},
                        {{0, 4, 5, 9, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 8, 0, 0, 0, 0, 0, 0}},
                        {{0, 2, 3, 7, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 2, 6, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}}
                    }
                };

                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap;
            /** CA-TDD Changes End **/
            }
            else
            {
                /** CA-TDD Changes Start **/
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 6;

                TddTimerConvMap tempTddDlTimerConvMap =
                {
                    6,
                    {
                        {{0, 1, 4, 5, 6, 9, 0, 0, 0, 0}},   
                        {{0, 3, 4, 5, 8, 9, 0, 0, 0, 0}},
                        {{0, 2, 3, 4, 7, 8, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 4, 5, 6, 9, 0, 0, 0, 0}},
                        {{0, 3, 4, 5, 8, 9, 0, 0, 0, 0}},
                        {{0, 2, 3, 4, 7, 8, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 0, 0, 0, 0}}
                    }
                };

                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
                /** CA-TDD Changes End **/
            } 

            TddTimerConvMap tempTddUlTimerConvMap =
            {
                4,
                {
                    {{0, 1, 4, 6, 0, 0, 0, 0, 0, 0}},   
                    {{0, 3, 5, 8, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 4, 7, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 3, 6, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 5, 7, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 4, 6, 0, 0, 0, 0, 0, 0}},
                    {{0, 3, 5, 8, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 4, 7, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 3, 6, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 5, 7, 0, 0, 0, 0, 0, 0}}
                }
            };
            /** CA-TDD Changes End **/
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
            /** CA-TDD Changes End **/
        }
        break;

        case UL_DL_CONFIG_2:
        {
            /** CA-TDD Changes Start **/
            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 2;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                    SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 6;
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 2 */
                {
                    6,
                    {
                        {{0, 3, 4, 5, 8, 9, 0, 0, 0, 0}},   
                        {{0, 2, 3, 4, 7, 8, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 4, 5, 6, 9, 0, 0, 0, 0}},
                        {{0, 3, 4, 5, 8, 9, 0, 0, 0, 0}},   
                        {{0, 2, 3, 4, 7, 8, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 4, 5, 6, 9, 0, 0, 0, 0}}
                    }
                };
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
            /** CA-TDD Changes End **/

            }
            else
            {
                /** CA-TDD Changes Start **/
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 8;
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 2 */
                {
                    8,
                    {
                        {{0, 1, 3, 4, 5, 6, 8, 9, 0, 0}},   
                        {{0, 2, 3, 4, 5, 7, 8, 9, 0, 0}},
                        {{0, 1, 2, 3, 4, 6, 7, 8, 0, 0}},
                        {{0, 1, 2, 3, 5, 6, 7, 8, 0, 0}},
                        {{0, 1, 2, 4, 5, 6, 7, 9, 0, 0}},
                        {{0, 1, 3, 4, 5, 6, 8, 9, 0, 0}},   
                        {{0, 2, 3, 4, 5, 7, 8, 9, 0, 0}},
                        {{0, 1, 2, 3, 4, 6, 7, 8, 0, 0}},
                        {{0, 1, 2, 3, 5, 6, 7, 8, 0, 0}},
                        {{0, 1, 2, 4, 5, 6, 7, 9, 0, 0}}
                    }
                };
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
                /** CA-TDD Changes End **/
            } 
            TddTimerConvMap tempTddUlTimerConvMap =
            {
                2,
                {
                    {{0, 3, 0, 0, 0, 0, 0, 0, 0, 0}},   
                    {{0, 2, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 5, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 4, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 3, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 5, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 4, 0, 0, 0, 0, 0, 0, 0, 0}}
                }
            };
            /** CA-TDD Changes Start **/
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
            /** CA-TDD Changes End **/
        }
        break;

        case UL_DL_CONFIG_3:
        {
            /** CA-TDD Changes Start **/
            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 3;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                   SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 6;
            /** CA-TDD Changes End **/
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 3 */
            {
                    6,  
                    {
                        {{0, 5, 6, 7, 8, 9, 0, 0, 0, 0}},
                        {{0, 4, 5, 6, 7, 8, 0, 0, 0, 0}},
                        {{0, 3, 4, 5, 6, 7, 0, 0, 0, 0}},
                        {{0, 2, 3, 4, 5, 6, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 9, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 8, 9, 0, 0, 0, 0}},
                        {{0, 1, 2, 7, 8, 9, 0, 0, 0, 0}},
                        {{0, 1, 6, 7, 8, 9, 0, 0, 0, 0}}
                    }
                };
                /** CA-TDD Changes Start **/
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
            }
            else
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 7;
                /** CA-TDD Changes End **/
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 3 */
                {
                    7,  
                    {
                        {{0, 1, 5, 6, 7, 8, 9, 0, 0, 0}},
                        {{0, 4, 5, 6, 7, 8, 9, 0, 0, 0}},
                        {{0, 3, 4, 5, 6, 7, 8, 0, 0, 0}},
                        {{0, 2, 3, 4, 5, 6, 7, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 9, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 8, 9, 0, 0, 0}},
                        {{0, 1, 2, 3, 7, 8, 9, 0, 0, 0}},
                        {{0, 1, 2, 6, 7, 8, 9, 0, 0, 0}}
                    }
                };
                /** CA-TDD Changes Start**/
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
                /** CA-TDD Changes End **/
            } 
            TddTimerConvMap tempTddUlTimerConvMap =
            {
                3,
                {
                    {{0, 8, 9, 0, 0, 0, 0, 0, 0, 0}},   
                    {{0, 7, 8, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 6, 7, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 5, 6, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 4, 5, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 3, 4, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 3, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 2, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 2, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 5, 0, 0, 0, 0, 0, 0, 0}}
                }
            };
            /** CA-TDD Changes Start**/
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
            /** CA-TDD Changes End **/
        }
        break;

        case UL_DL_CONFIG_4:
        {
            /** CA-TDD Changes Start **/
            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 2;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                    SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 7;
            /** CA-TDD Changes End **/
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 4 */
                {
                    7,  
                    {
                        {{0, 4, 5, 6, 7, 8, 9, 0, 0, 0}},
                        {{0, 3, 4, 5, 6, 7, 8, 0, 0, 0}},
                        {{0, 2, 3, 4, 5, 6, 7, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 9, 0, 0, 0}},
                        {{0, 1, 2, 3, 4, 8, 9, 0, 0, 0}},
                        {{0, 1, 2, 3, 7, 8, 9, 0, 0, 0}},
                        {{0, 1, 2, 6, 7, 8, 9, 0, 0, 0}},
                        {{0, 1, 5, 6, 7, 8, 9, 0, 0, 0}}
                    }
                };
                /** CA-TDD Changes Start **/
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
            }
            else
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 8;
                /** CA-TDD Changes End **/
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 4 */
                {
                    8,  
                    {
                        {{0, 1, 4, 5, 6, 7, 8, 9, 0, 0}},
                        {{0, 3, 4, 5, 6, 7, 8, 9, 0, 0}},
                        {{0, 2, 3, 4, 5, 6, 7, 8, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 9, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 8, 9, 0, 0}},
                        {{0, 1, 2, 3, 4, 7, 8, 9, 0, 0}},
                        {{0, 1, 2, 3, 6, 7, 8, 9, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 8, 9, 0, 0}}
                    }
                };
                /** CA-TDD Changes Start **/
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
                /** CA-TDD Changes End **/
            } 
            TddTimerConvMap tempTddUlTimerConvMap =
            {
                2,
                {
                    {{0, 8, 0, 0, 0, 0, 0, 0, 0, 0}},   
                    {{0, 7, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 6, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 5, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 4, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 3, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 2, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 9, 0, 0, 0, 0, 0, 0, 0, 0}}
                }
            };
            /** CA-TDD Changes Start **/
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
        }
        break;

        case UL_DL_CONFIG_5:
        {
            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 1;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                   SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 8;
            /** CA-TDD Changes End **/
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 5 */
                {
                    8,  
                    {
                        {{0, 3, 4, 5, 6, 7, 8, 9, 0, 0}},
                        {{0, 2, 3, 4, 5, 6, 7, 8, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 9, 0, 0}},
                        {{0, 1, 2, 3, 4, 5, 8, 9, 0, 0}},
                        {{0, 1, 2, 3, 4, 7, 8, 9, 0, 0}},
                        {{0, 1, 2, 3, 6, 7, 8, 9, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 8, 9, 0, 0}},
                        {{0, 1, 4, 5, 6, 7, 8, 9, 0, 0}}
                    }
                };
                /** CA-TDD Changes Start **/
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
            }
            else
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 9;
                /** CA-TDD Changes End **/
                TddTimerConvMap tempTddDlTimerConvMap =
                    /* CONFIG 5 */
                {
                    9,  
                    {
                        {{0, 1, 3, 4, 5, 6, 7, 8, 9, 0}},
                        {{0, 2, 3, 4, 5, 6, 7, 8, 9, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 7, 9, 0}},
                        {{0, 1, 2, 3, 4, 5, 6, 8, 9, 0}},
                        {{0, 1, 2, 3, 4, 5, 7, 8, 9, 0}},
                        {{0, 1, 2, 3, 4, 6, 7, 8, 9, 0}},
                        {{0, 1, 2, 3, 5, 6, 7, 8, 9, 0}},
                        {{0, 1, 2, 4, 5, 6, 7, 8, 9, 0}}
                    }
                };
                /** CA-TDD Changes Start **/
                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
                /** CA-TDD Changes End **/
            } 
            TddTimerConvMap tempTddUlTimerConvMap =
            {
                1,
                {
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},   
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
                    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
                }
            };
            /** CA-TDD Changes Start **/
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
            /** CA-TDD Changes End **/
        }
        break;

        case UL_DL_CONFIG_6:
        /* TDD Config 6 Changes Start */
        {

            tddConfigInfo_g[internalCellIndex].noOfUlSchedulable = 5;
            if (SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                    SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns)
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 3;

    /*+ coverity 24640 */
                STATIC TddTimerConvMap tempTddDlTimerConvMap =
    /*- coverity 24640 */
            {
                    3,
                    {
                        {{0, 5, 9, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 4, 8, 9, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 7, 8, 0, 0, 0, 0, 0, 0}},
                        {{0, 2, 6, 7, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 5, 6, 0, 0, 0, 0, 0, 0}},
                        {{0, 4, 5, 0, 0, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 9, 0, 0, 0, 0, 0, 0}},
                        {{0, 2, 3, 8, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 2, 7, 0, 0, 0, 0, 0, 0}},
                        {{0, 1, 6, 0, 0, 0, 0, 0, 0, 0}}
                    }
                };

                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap;
            }
            else
            {
                tddConfigInfo_g[internalCellIndex].noOfDlSchedulable = 5;

    /*+ coverity 24640 */
                STATIC TddTimerConvMap tempTddDlTimerConvMap =
    /*- coverity 24640 */
                {
                    5,
                    {
                        {{0, 1, 5, 6, 9, 0, 0, 0, 0, 0}},   
                        {{0, 4, 5, 8, 9, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 7, 8, 9, 0, 0, 0, 0}},
                        {{0, 2, 3, 6, 7, 8, 0, 0, 0, 0}},
                        {{0, 1, 2, 5, 6, 7, 0, 0, 0, 0}},
                        {{0, 1, 4, 5, 6, 0, 0, 0, 0, 0}},
                        {{0, 3, 4, 5, 9, 0, 0, 0, 0, 0}},
                        {{0, 2, 3, 4, 8, 9, 0, 0, 0, 0}},
                        {{0, 1, 2, 3, 7, 8, 0, 0, 0, 0}},
                        {{0, 1, 2, 6, 7, 0, 0, 0, 0, 0}}
                    }
                };

                tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap = tempTddDlTimerConvMap; 
            } 

    /*+ coverity 24640 */
            STATIC TddTimerConvMap tempTddUlTimerConvMap =
    /*- coverity 24640 */
            {
                5,
                {
                    {{0, 1, 5, 6, 9, 0, 0, 0, 0, 0}},   
                    {{0, 4, 5, 8, 9, 0, 0, 0, 0, 0}},
                    {{0, 3, 4, 7, 8, 9, 0, 0, 0, 0}},
                    {{0, 2, 3, 6, 7, 8, 0, 0, 0, 0}},
                    {{0, 1, 2, 5, 6, 7, 0, 0, 0, 0}},
                    {{0, 1, 4, 5, 6, 0, 0, 0, 0, 0}},
                    {{0, 3, 4, 5, 9, 0, 0, 0, 0, 0}},
                    {{0, 2, 3, 4, 8, 9, 0, 0, 0, 0}},
                    {{0, 1, 2, 3, 7, 8, 0, 0, 0, 0}},
                    {{0, 1, 2, 6, 7, 0, 0, 0, 0, 0}}
                }
            };
            tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap = tempTddUlTimerConvMap;
        }
        /* TDD Config 6 Changes End */
        break;
        default:
            lteWarning("Not supported tdd config: invalid");
            break;
    }
}

/*****************************************************************************
 * Function Name  : isValidSfForDCI_0
 * Inputs         : subframeNum  - This is present/current subframe number.
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will check that it is valid subframe
 *                  to send DCI-0.
 *
 *****************************************************************************/
/** CA-TDD Changes Start **/
 MacRetType isValidSfForDCI_0 ( UInt8 subframeNum, InternalCellIndex internalCellIndex)
{
    MAC_ASSERT( subframeNum < MAX_SUB_FRAME);

    if  ((DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame
            || SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame )
            && NA != ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulGrantPhichInfo.\
                ulGrantSubFrameNum)
/** CA-TDD Changes End **/
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}

/*****************************************************************************
 * Function Name  : getSfForRachULConfig
 * Inputs         : subframeNum  - This is present/current subframe number.
 *                  index - pointer to the varialble where index needs to be
 *                          stored.
 * Returns        : sub frame number on which RACH UL CONFIG message needs to
 *                  be stored ( SF where DCI0 for Msg3 is to be sent).
 * Description    : This function will return the index and sub frame number 
 *                  where DCI 0 for the Msg3 corresponding to the RAR sent in
 *                  the subframeNum is to be sent.
 *****************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getSfForRachULConfig (UInt8 subframeNum,
                                   UInt8 *index,InternalCellIndex internalCellIndex)
{
    MAC_ASSERT ((DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame)
            || (SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame))

    UInt8 sfForMsg3 = sfForRachULConfig_p[internalCellIndex][subframeNum].ulGrantSfForMsg3;
    
    sfForMsg3 += subframeNum;
    if (sfForMsg3 >= MAX_SUB_FRAME)
    {
        sfForMsg3 %= MAX_SUB_FRAME;
    }
    *index = sfForRachULConfig_p[internalCellIndex][subframeNum].containerIndex;
/** CA-TDD Changes End **/
    return (sfForMsg3);
}

/*****************************************************************************
 * Function Name  : getDlAckNackSfnFactor
 * Inputs         : subframeNum - This is current subframe number
 *
 * Returns        : This function will return the 0 or 1. 
 * Description    : It calculates the SFN Factor based on the UL sub frame on 
 *                  which DL Ack NAck is to be recieved.
 *****************************************************************************/
/** CA-TDD Changes Start:05 **/
 UInt8 getDlAckNackSfnFactor(UInt8 subframeNum,InternalCellIndex internalCellIndex)
{
    MAC_ASSERT( subframeNum < MAX_SUB_FRAME );
    UInt8 dlAckNackSubFrameNumber = 
               dlAckNackSubFrameNumInfo_p[internalCellIndex][subframeNum].dlAckNackSubFrameNumber;
/** CA-TDD Changes End:05 **/

    /* If the current UL grant is 
     * next System Frame then return 1 
     * else 0.
     */

    dlAckNackSubFrameNumber += subframeNum;
    if (( dlAckNackSubFrameNumber / MAX_SUB_FRAME) == 1)
    {
        return 1;  
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************
 * Function Name  : getULSfForMsg3
 * Inputs         : subframeNum  - This is present/current subframe number.
 * Returns        : sub frame number on which resources for Msg3 needs to be 
 *                  reserved ( UL SF where Msg3 is to be recieved).
 * Description    : This function UL SF where Msg3 is to be recieved.
 *****************************************************************************/
 UInt8 getULSfForMsg3 (UInt8 subframeNum, InternalCellIndex internalCellIndex)
{
    /** CA-TDD Changes Start **/
    MAC_ASSERT ((DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame)
            || (SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame))
    UInt8 sfForULGrant = sfForRachULConfig_p[internalCellIndex][subframeNum].ulGrantSfForMsg3;
    
    sfForULGrant += subframeNum;
    if (sfForULGrant >= MAX_SUB_FRAME)
    {
        sfForULGrant %= MAX_SUB_FRAME;
    }
    return (sfForRachULConfig_p[internalCellIndex][subframeNum].ulGrantSfForMsg3 +
                ulGrantPhichTableInfo_p[internalCellIndex][sfForULGrant].ulGrantPhichInfo.\
                ulGrantSubFrameNum);
    /** CA-TDD Changes End **/
}
/*TDD SB_CQI*/
/*****************************************************************************
 * Function Name  : getULSfForUpdatingBandwidthPart
 * Inputs         : subframeNum  - This is present/current subframe number.
 * Returns        : UL SubFrame number 
 * Description    : This function will return the UL Grant subframe number on valid DCI0 
 ******************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getULSfForUpdatingBandwidthPart(UInt8 subframeNum,InternalCellIndex internalCellIndex)
/** CA-TDD Changes End **/
{
	UInt8 sfForULGrant = 0;

	sfForULGrant += DL_ADVANCE_TICK + MAX_DELAY_TDD;
	subframeNum  += DL_ADVANCE_TICK + MAX_DELAY_TDD;
	do
	{
		/* Inrement of two is corresponding to 1 for DL ADVANCE and 1 for DELAY  */
		if(subframeNum >= MAX_SUB_FRAME)
		{
			subframeNum %= MAX_SUB_FRAME;
		}
		if(isValidSfForDCI_0(subframeNum, internalCellIndex))
		{
                        /** CA-TDD Changes Start **/
			return(sfForULGrant+ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].\
                                            ulGrantPhichInfo.ulGrantSubFrameNum);
                        /** CA-TDD Changes End **/
		}
		sfForULGrant += MAX_DELAY_TDD;
		subframeNum += MAX_DELAY_TDD;
	}while(1);
}
/*TDD SB_CQI*/
/*****************************************************************************
 * Function Name  : getTickForPUCCHCalForMsg3 
 * Inputs         : subframeNum  - This is present/current subframe number.
 * Returns        : Tick on which PUCCH is called for Msg3.  
 * Description    : This function  gets the tick where PUCCH is called for Msg3
 *****************************************************************************/
    /** CA-TDD Changes Start **/
 UInt8 getTickForPUCCHCalForMsg3 (UInt8 subframeNum,InternalCellIndex internalCellIndex)
{
    MAC_ASSERT ((DL == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame)
            || (SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame))
    return(sfForRachULConfig_p[internalCellIndex][subframeNum].ulGrantSfForMsg3);
    /** CA-TDD Changes End **/
}
#ifdef MAC_AUT_TEST
/*****************************************************************************
 * Function Name  : tdd_config() 
 * Inputs         : 
 * Returns        :   
 * Description    : This is Aut function
 *****************************************************************************/
tdd_config()
{
InternalCellIndex internalCellIndex=0;
currentConfigIndex_g[internalCellIndex]=8;
}
/*****************************************************************************
 * Function Name  : tdd_config_1() 
 * Inputs         : 
 * Returns        :   
 * Description    : This is Aut function
 *****************************************************************************/
tdd_config_1()
{
InternalCellIndex internalCellIndex=0;
currentConfigIndex_g[internalCellIndex]=7;
}
/*****************************************************************************
 * Function Name  : tdd_config_2() 
 * Inputs         : 
 * Returns        :   
 * Description    : This is Aut function
 *****************************************************************************/
tdd_config_2()
{
InternalCellIndex internalCellIndex=0;
currentConfigIndex_g[internalCellIndex]=6;
}

#endif
/*****************************************************************************
 * Function Name  : getTddConfig
 * Inputs         : None
 * Returns        : return TDD UL-DL Configuration.
 * Description    : This function retrives the UL-DL configuration of the
 *    system
 ******************************************************************************/
/** CA-TDD Changes Start **/
 UInt8 getTddConfig (InternalCellIndex internalCellIndex)
{
        return currentConfigIndex_g[internalCellIndex];
/** CA-TDD Changes End **/
}

/*****************************************************************************
 * Function Name  : tddConvertTimerValForUlGrant
 * Inputs         : timerVal- Value of timer in FDD mode.
 *                  globalTTI- currentGlobalTick
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : This function shall convert the expiry tick such that the 
 *                  expiry tick shall be a UL grant sub frame in TDD mode.
 *****************************************************************************/
    /** CA-TDD Changes Start **/
/* SPR 15909 fix start */
 tickType_t tddConvertTimerValForUlGrant (
        UInt16 timerVal,
        tickType_t globalTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    /* SPR 10794 changes start */
    UInt8   sf = globalTTI % MAX_SUB_FRAME;
    /* SPR 10794 changes end */
    /* SPR 15909 fix start */
    tickType_t expTTI = 0;
    /* SPR 15909 fix end */
    UInt8 numUlGrantSf = 0;
    UInt16  newTimerVal = 0;

    numUlGrantSf = tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap.numPDCCHSF;
    
    if (timerVal < numUlGrantSf)
    {
       /* SPR 10794 changes start */
        expTTI = globalTTI + tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap.
           tddSFToTimerMap[sf].timerValue[timerVal];
    }
    else
    {
        newTimerVal = ((timerVal / numUlGrantSf) * MAX_SUB_FRAME) +
           tddTimerConvMap_g[internalCellIndex].tddUlTimerConvMap.\
             tddSFToTimerMap[sf].timerValue[(timerVal % numUlGrantSf)];
        /* SPR 10794 changes end */
    /** CA-TDD Changes End **/

        expTTI = globalTTI + newTimerVal;
    }

    return expTTI;
}

/****************************************************************************
 *Function Name  : tddConvertTimerValForDlGrant 
 *Inputs         : timerVal,globalTTI
 *Outputs        : 
 *Returns        : expTTI 
 *Description    : This function will send the TTI for DL Grant
 ****************************************************************************/
     /** CA-TDD Changes Start **/
/* SPR 15909 fix start */
 tickType_t tddConvertTimerValForDlGrant(
        UInt16  timerVal, 
        tickType_t globalTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    UInt8   sf = globalTTI % 10;
    UInt8   numPDCCHSF = 0;
    UInt16  newTimerVal = 0;
    /* SPR 15909 fix start */
    tickType_t  expTTI = 0;
    /* SPR 15909 fix end */

    numPDCCHSF = tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap.numPDCCHSF;
    
    if (timerVal < numPDCCHSF)
    {
        expTTI = globalTTI + tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap.
            tddSFToTimerMap[sf].timerValue[timerVal];
    }
    else
    {
        newTimerVal = ((timerVal / numPDCCHSF) * 10) + 
            (tddTimerConvMap_g[internalCellIndex].tddDlTimerConvMap.\
            tddSFToTimerMap[sf].timerValue[timerVal % numPDCCHSF]);
    /** CA-TDD Changes End **/

        expTTI = globalTTI + newTimerVal;  
    }
    return expTTI;
}

/* + SPS_TDD_Changes */
/****************************************************************************
 *Function Name  : getSubFrameOffsetForSpsTDD 
 *Inputs         : subFrameNum 
 *Outputs        : subframeOffset
 *Description    : This function will return the subFrame offset corresponding to
                   current TDD Config
 ****************************************************************************/
 SInt8 getSubFrameOffsetForSpsTDD (UInt8 subFrameNum,
					InternalCellIndex internalCellIndex)
{
    return(subFrameOffsetForSps[cellSpecificParams_g.\
		    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign][subFrameNum]);
}
/* - SPS_TDD_Changes */
/*TDD Config 0 Changes Start*/
/****************************************************************************
 *Function Name  : getUlOffset 
 *Inputs         : hiSf - Current DL Sf
 ulSf - Ul Sf coresponding to current DL SF
 *Outputs        : 
 *Returns        : UlOffset 
 *Description    : This function will calculate the offset for next UL SF
 coresponding to current DL SF
 ****************************************************************************/
 UInt8 getUlOffset(UInt8 hiSf,UInt8 ulSf)
{
    return (hiSf < ulSf) ? (ulSf - hiSf):(ulSf - hiSf + MAX_SUB_FRAME);    
}
/****************************************************************************
 *Function Name  : getnextUlSfOffsetFromCurrentUlSf
 *Inputs         : ulSf - Current UL SF 
 *Outputs        : 
 *Returns        : Ul Offset, internalCellIndex 
 *Description    : This function will calculate the Next UL offset 
 coresponding to current UL SF
 ****************************************************************************/
UInt8 getnextUlSfOffsetFromCurrentUlSf(UInt8 ulSf,
                                    /* CA TDD CHANGES START */
                                    InternalCellIndex internalCellIndex
                                    /* CA TDD CHANGES END */
                                    )
{
    return ulSubFrameForPuschTransmission_gp[internalCellIndex][ulSf].nextUlSfOffset;    
}
/****************************************************************************
 *Function Name  : getprevUlSfOffsetFromCurrentUlSf
 *Inputs         : ulSf - Current UL SF,
 *                 internalCellIndex - internalCellIndex used at MAC.
 *Outputs        : 
 *Returns        : Ul Offset 
 *Description    : This function will calculate the Previous UL offset 
 coresponding to current UL SF
 ****************************************************************************/
 UInt8 getprevUlSfOffsetFromCurrentUlSf(UInt8 ulSf,
                            /* CA TDD CHANGES START */
                            InternalCellIndex internalCellIndex
                            /* CA TDD CHANGES END */
                            )
{
    return ulSubFrameForPuschTransmission_gp[internalCellIndex][ulSf].prevUlSfOffset;    
}
/****************************************************************************
 *Function Name  : getUlSubframe 
 *Inputs         : prevUlSfOffset - Previous Ul Subframe coresponding to current
                   InternalCellIndex internalCellIndex
 DL Subframe(HI).
 *Outputs        : 
 *Returns        : offset - Offset to next UL SF from current DL SF. 
 *Description    : This function will calculate the offset for next UL SF
 coresponding to current DL SF
 ****************************************************************************/
 UInt8 getUlSubframe(UInt8 prevUlSfOffset, UInt8 hiSf,InternalCellIndex internalCellIndex)
{
    UInt8 offset = 0;
    if (UL_DL_CONFIG_0 == currentConfigIndex_g[internalCellIndex])
    {
        if ( prevUlSfOffset == 3 || prevUlSfOffset == 8)
        {
            offset = getExactULSubFrame(hiSf,internalCellIndex);    
        }
        else
        {
            offset = 7;
        }
    }
    else 
    {
        offset = getExactULSubFrame(hiSf,internalCellIndex);
    }
    return offset;
}
/*UL_MU_MIMO_CHG_START*/
/****************************************************************************
 *Function Name  : roundTripTime 
 *Inputs         : ulSubframenum - UL Sub Frame number,
 *                 internalCellIndex - cell index used at MAC
 *Outputs        : 
 *Returns        : RTT
 *Description    : This function will calculate the round trip time from the 
 current DL(DCI0 Tick) SF to next DL(HI Tick) Sf.
 ****************************************************************************/
 UInt8 roundTripTime(UInt8 ulSubframenum, 
            /* CA TDD CHANGES START */
            InternalCellIndex internalCellIndex
            /* CA TDD CHANGES END */
            )
{
    /* CA TDD CHANGES START */
    return ulSubframeToRTTMap_gp[internalCellIndex][ulSubframenum];
    /* CA TDD CHANGES END */
}
/*UL_MU_MIMO_CHG_END*/
/*TDD Config 0 Changes End*/
/* TDD Config 0 Changes Start */
/*****************************************************************************
 * Function Name  : isTddConfig0SsubFrame
 * Inputs         : subframeNum  - This is subframe number.
 * Returns        : LTE_TRUE/LTE_FALSE
 * Description    : This function will check that it is tdd config 0 and 
 *                  Special subframe
 *
 *****************************************************************************/
 UInt8 isTddConfig0SsubFrame(UInt8 subframeNum,InternalCellIndex internalCellIndex)
{
    if ((UL_DL_CONFIG_0 == currentConfigIndex_g[internalCellIndex]) &&
            (SP == ulGrantPhichTableInfo_p[internalCellIndex][subframeNum].ulDlSpFrame))
    {
        return LTE_TRUE;
    }
    return LTE_FALSE;
}
/* SPR 11115 Fix Start */
/*****************************************************************************
 * Function Name  : getNumTicksPerSecond
 * Inputs         : sfDirection  - This is denotes the DL or UL sf (DL/SP 
 *                  means DL .
 * Returns        : numOfTicksPerSecond - Number of DL or UL sf in a second.
 *                  here DL or SP implies DL and UL --> UL
 * Description    : This function will return the number of DL or UL sf in a
 *                  second
 *
 *****************************************************************************/
 UInt16 getNumTicksPerSecond(ULDLSPFrame sfDirection, InternalCellIndex internalCellIndex)
{
    UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
    if (UL == sfDirection)
    {
        numOfTicksPerSecond = (NUM_TICKS_PER_SECOND / MAX_SUB_FRAME ) *
                               tddConfigInfo_g[internalCellIndex].noOfUlSchedulable;
    }
    else if (DL == sfDirection) 
    {
        numOfTicksPerSecond = (NUM_TICKS_PER_SECOND / MAX_SUB_FRAME ) *
                               tddConfigInfo_g[internalCellIndex].noOfDlSchedulable;
    }
    return numOfTicksPerSecond;
}
/* SPR 11115 Fix End */
/* Cyclomatic_complexity_changes_start */

/*****************************************************************************
 * Function Name  : setSubFrameTypeForDlHarq
 * Inputs         : isSplSubFrame,harqProcess_p
 *
 * Returns        : None
 * Description    : This function update subFrameType in Harq Process context
 *
 ******************************************************************************/
void setSubFrameTypeForDlHarq(UInt8           isSplSubFrame,
                              DLHARQProcess   *harqProcess_p
                             )
{
    if (!isSplSubFrame)
    {
        harqProcess_p->subFrameType = DL;
    }
    else
    {
        harqProcess_p->subFrameType = SP;
    }
}
/* Cyclomatic_complexity_changes_end */
#ifdef LTE_EMBMS_SUPPORTED
/****************************************************************************
 *Function Name  : getNextDLOccasion
 *Inputs         : sib13sf - Current subFrame
                   internalCellIndex - Current CellIndex
 *Outputs        :
 *Returns        : offset - Offset to current/next DL SF.
 *Description    : This function will calculate the offset for current/next
  DL SF
 ****************************************************************************/
UInt8 getNextDLOccasion(UInt8 sib13sf, InternalCellIndex internalCellIndex)
{
  UInt8 dlSubFrameNum  = 0;
  UInt8 sfOffset=0;
  for(sfOffset=0;sfOffset<=9;sfOffset++)
  {
      dlSubFrameNum  = (sib13sf+sfOffset)%MAX_SUBFRAME_VAL;
      if(1 == embmsValidTddDlSF_g[currentConfigIndex_g[internalCellIndex]][dlSubFrameNum])
      { 
         return dlSubFrameNum;
      }
  }
  return dlSubFrameNum;
}
#endif
/* + SPS_TDD_Changes */
/*****************************************************************************
 * Function Name  : getPrevSubFrameforDCI0
 * Inputs         : subframe - The UE Index for which the HARQ process is required.
 * Returns        : return Subframe Type - DL,UL,SP.
 * Description    : This function retrive the Subframe Type from structure
 *                  ulGrantPhichTableInfo_g for current UL-DL config and
 *                  input subframe number and return the same
 ******************************************************************************/
 UInt8 getPrevSubFrameforDCI0( UInt8 ulSubframe,InternalCellIndex internalCellIndex)
{
        return ulGrantPhichTableInfo_g[cellSpecificParams_g.\
          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign][ulSubframe].prevRecordForUL.prevSubFrameforDCI_O_From_currentSubFrame;
}
/* - SPS_TDD_Changes */
#endif /*TDD_CONFIG*/
