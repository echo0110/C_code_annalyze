/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPRACH.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPRACH.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2009/07/13 12:07:28  gur15697
 * second argument to LTE_MAC_UT_LOG modifed
 *
 * Revision 1.2  2009/07/07 08:55:48  gur15697
 * UT bug fixes
 *
 * Revision 1.1  2009/06/08 08:50:59  gur15697
 * Initial vertion
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacPRACH.h"
#include "lteMacCellConfMgr.h"
#ifdef TDD_CONFIG
#include "lteMacPUCCH.h"
#include "lteMacTDDConfig.h"
#include "lteMacCellConfMgr.h"
#include "lteMacULResourceAllocator.h"
#endif

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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/*
 *For below table reffer 36.211 "Table 5.7.1-2: Frame structure type 1 random  
 *access configuration for preamble format 0-3"
 */
#ifdef FDD_CONFIG
STATIC PRACHConfigInfo prachConfig[MAX_PRACH_CONFIGURATION_INDEX] = 
{
    /*******************************************************
     *Preamble format   SFN        Subframe map             
     ******************************************************/
    {PREAMBLE_FORMAT_0, EVEN, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_0, EVEN, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_0, EVEN, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 0, 1, 0, 0, 1, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}},
    {PREAMBLE_FORMAT_0, ANY,  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0}},
    {PREAMBLE_FORMAT_0, ANY,  {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
    {PREAMBLE_FORMAT_0, ANY,  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
    {PREAMBLE_FORMAT_0, EVEN, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
    {PREAMBLE_FORMAT_1, EVEN, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_1, EVEN, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_1, EVEN, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 0, 1, 0, 0, 1, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}},
    {PREAMBLE_FORMAT_1, ANY,  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0}},
    {PREAMBLE_FORMAT_1, ANY,  {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
    {INVALID_PREAMBLE_FORMAT, INVALID_PRACH_SFN,  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_1, EVEN, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
    {PREAMBLE_FORMAT_2, EVEN, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_2, EVEN, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_2, EVEN, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 0, 1, 0, 0, 1, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}},
    {PREAMBLE_FORMAT_2, ANY,  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0}},
    {PREAMBLE_FORMAT_2, ANY,  {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}},
    {INVALID_PREAMBLE_FORMAT, INVALID_PRACH_SFN,  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_2, EVEN, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
    {PREAMBLE_FORMAT_3, EVEN, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_3, EVEN, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_3, EVEN, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 1, 0, 0, 0, 0, 1, 0, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 0, 1, 0, 0, 1, 0, 0, 1, 0}},
    {PREAMBLE_FORMAT_3,  ANY, {0, 0, 0, 1, 0, 0, 1, 0, 0, 1}},
    {INVALID_PREAMBLE_FORMAT, INVALID_PRACH_SFN,  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {INVALID_PREAMBLE_FORMAT, INVALID_PRACH_SFN,  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {INVALID_PREAMBLE_FORMAT, INVALID_PRACH_SFN,  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {PREAMBLE_FORMAT_3, EVEN, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}}
};
#endif

#ifdef TDD_CONFIG
/* The table returns preamble format from the PRACH Config Index as per table
 * 5.7.1-3, 36.211 */
static PreambleFormat preambleFrmPrachConfig_g[MAX_PRACH_CONFIGURATION_INDEX] =
{
    PREAMBLE_FORMAT_0, /*  0 */
    PREAMBLE_FORMAT_0, /*  1 */
    PREAMBLE_FORMAT_0, /*  2 */
    PREAMBLE_FORMAT_0, /*  3 */
    PREAMBLE_FORMAT_0, /*  4 */
    PREAMBLE_FORMAT_0, /*  5 */
    PREAMBLE_FORMAT_0, /*  6 */
    PREAMBLE_FORMAT_0, /*  7 */
    PREAMBLE_FORMAT_0, /*  8 */
    PREAMBLE_FORMAT_0, /*  9 */
    PREAMBLE_FORMAT_0, /* 10 */
    PREAMBLE_FORMAT_0, /* 11 */
    PREAMBLE_FORMAT_0, /* 12 */
    PREAMBLE_FORMAT_0, /* 13 */
    PREAMBLE_FORMAT_0, /* 14 */
    PREAMBLE_FORMAT_0, /* 15 */
    PREAMBLE_FORMAT_0, /* 16 */
    PREAMBLE_FORMAT_0, /* 17 */
    PREAMBLE_FORMAT_0, /* 18 */
    PREAMBLE_FORMAT_0, /* 19 */
    PREAMBLE_FORMAT_1, /* 20 */
    PREAMBLE_FORMAT_1, /* 21 */
    PREAMBLE_FORMAT_1, /* 22 */
    PREAMBLE_FORMAT_1, /* 23 */
    PREAMBLE_FORMAT_1, /* 24 */
    PREAMBLE_FORMAT_1, /* 25 */
    PREAMBLE_FORMAT_1, /* 26 */
    PREAMBLE_FORMAT_1, /* 27 */
    PREAMBLE_FORMAT_1, /* 28 */
    PREAMBLE_FORMAT_1, /* 29 */
    PREAMBLE_FORMAT_2, /* 30 */
    PREAMBLE_FORMAT_2, /* 31 */
    PREAMBLE_FORMAT_2, /* 32 */
    PREAMBLE_FORMAT_2, /* 33 */
    PREAMBLE_FORMAT_2, /* 34 */
    PREAMBLE_FORMAT_2, /* 35 */
    PREAMBLE_FORMAT_2, /* 36 */
    PREAMBLE_FORMAT_2, /* 37 */
    PREAMBLE_FORMAT_2, /* 38 */
    PREAMBLE_FORMAT_2, /* 39 */
    PREAMBLE_FORMAT_3, /* 40 */
    PREAMBLE_FORMAT_3, /* 41 */
    PREAMBLE_FORMAT_3, /* 42 */
    PREAMBLE_FORMAT_3, /* 43 */
    PREAMBLE_FORMAT_3, /* 44 */
    PREAMBLE_FORMAT_3, /* 45 */
    PREAMBLE_FORMAT_3, /* 46 */
    PREAMBLE_FORMAT_3, /* 47 */
    PREAMBLE_FORMAT_4, /* 48 */
    PREAMBLE_FORMAT_4, /* 49 */
    PREAMBLE_FORMAT_4, /* 50 */
    PREAMBLE_FORMAT_4, /* 51 */
    PREAMBLE_FORMAT_4, /* 52 */
    PREAMBLE_FORMAT_4, /* 53 */
    PREAMBLE_FORMAT_4, /* 54 */
    PREAMBLE_FORMAT_4, /* 55 */
    PREAMBLE_FORMAT_4, /* 56 */
    PREAMBLE_FORMAT_4, /* 57 */
    INVALID_PREAMBLE_FORMAT, /* 58 */
    INVALID_PREAMBLE_FORMAT, /* 59 */
    INVALID_PREAMBLE_FORMAT, /* 60 */
    INVALID_PREAMBLE_FORMAT, /* 61 */
    INVALID_PREAMBLE_FORMAT, /* 62 */
    INVALID_PREAMBLE_FORMAT /* 63 */
};

/******************************************************************************
 * The table returns if the the PRACH CONFIG index received is valid or invalid
 * for TDD CONFIG(0-6). This table is defined as per 5.7.1-4, 36.211 
 *****************************************************************************/
static UInt8 prachConfigValid[MAX_PRACH_CONFIGURATION_INDEX]
                        [UL_DL_CONFIG_MAX] = 
{
    /* 0,    1,    2,    3,    4,    5,    6   */
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 0 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 1 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE } /* 2 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 3 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE } /* 4 */,
    { TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 5 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 6 */,
    { TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 7 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 8 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 9 */,
    { TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE } /* 10 */,
    /* 0,    1,    2,    3,    4,    5,    6   */
    { FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE } /* 11 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 12 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 13 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 14 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 15 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE } /* 16 */,
    { TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE } /* 17 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 18 */,
    { FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE } /* 19 */,
    /* 0,    1,    2,    3,    4,    5,    6   */
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 20 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 21 */,
    { TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE } /* 22 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 23 */,
    { TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE } /* 24 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 25 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 26 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 27 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 28 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 29 */,
    /* 0,    1,    2,    3,    4,    5,    6   */
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 30 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 31 */,
    { TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE } /* 32 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 33 */,
    { TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE } /* 34 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 35 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 36 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 37 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 38 */,
    { TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE } /* 39 */,
    /* 0,    1,    2,    3,    4,    5,    6   */
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 40 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 41 */,
    { TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 42 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 43 */,
    { TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 44 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 45 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 46 */,
    { TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE } /* 47 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 48 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 49 */,
    /* 0,    1,    2,    3,    4,    5,    6   */
    { TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE } /* 50 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 51 */,
    { TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE } /* 52 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 53 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 54 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 55 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 56 */,
    { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE } /* 57 */,
    { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 58 */,
    { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 59 */,
    { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 60 */,
    { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 61 */,
    { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 62 */,
    { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE } /* 63 */

};

/******************************************************************************
 * Table: prachResInfoForConfig0
 * Description: Returns set of frequency in set of valid SFs for TDD Config 0
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U U D S U U U
 *****************************************************************************/
PRACHResInfo prachResInfoForConfig0[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }}, //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 }}, //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }}, //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 }}, //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }}, //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 }}, //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //11

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 }},  //12

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: prachResInfoForConfig1
 * Description: Returns set of frequency in set of valid SFs for TDD Config 1
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U D D S U U D
 *****************************************************************************/
static PRACHResInfo prachResInfoForConfig1[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }}, //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }}, //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //11

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //12

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: prachResInfoForConfig2
 * Description: Returns set of frequency in set of valid SFs for TDD Config 2
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U D D D S U D D
 *****************************************************************************/
PRACHResInfo prachResInfoForConfig2[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //11

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: prachResInfoForConfig3
 * Description: Returns set of frequency in set of valid SFs for TDD Config 3
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U U D D D D D
 *****************************************************************************/
PRACHResInfo prachResInfoForConfig3[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //11

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //12

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2 },{SF_VALID, 1, 2},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 1, 2 },{SF_VALID, 1, 2},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 4, 5 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 5, 6 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 4, 5 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 5, 6 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 4, 5 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 5, 6 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: prachResInfoForConfig4
 * Description: Returns set of frequency in set of valid SFs for TDD Config 4
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U D D D D D D
 *****************************************************************************/
PRACHResInfo prachResInfoForConfig4[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //12

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 4, 5 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 5, 6 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 4, 5 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 5, 6 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 4, 5 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 5, 6 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: prachResInfoForConfig5
 * Description: Returns set of frequency in set of valid SFs for TDD Config 5
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U D D D D D D D
 *****************************************************************************/
PRACHResInfo prachResInfoForConfig5[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //11

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //12

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 4, 5 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 5, 6 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 4, 5 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 5, 6 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: prachResInfoForConfig6
 * Description: Returns set of frequency in set of valid SFs for TDD Config 6 
 *              as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U U D S U U D
 *****************************************************************************/
PRACHResInfo prachResInfoForConfig6[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] =
{
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //0

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //1

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //2

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //3

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //4

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //5

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //6

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //7

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //8

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //9

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //10

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //11
/* + SPR 17439 */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //12
/* - SPR 17439 */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //13

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //14

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //15

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //16

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //17

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 1, 2},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 }},  //18

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1 },{SF_VALID, 0, 1},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_VALID,1 , 2 },{SF_INVALID, 0, 0 }},  //19

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //20

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //21

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //22

    {{SF_INVALID, 1, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //23

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //24

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //25

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //26

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //27

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //28

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //29

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //30

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //31

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //32

    {{SF_INVALID, 1, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //33

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //34

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //35

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //36

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //37

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //38

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //39

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //40

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //41

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //42

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //43

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //44

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //45

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //46

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_VALID, 3, 4 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //47

    /* Preamble 4 PRACH configurations (48 - 57) */
    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //48

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //49

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //50

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //51

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //52

    {{SF_INVALID, 0, 0 }, {SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //53

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 0, 1 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //54

    {{SF_INVALID, 0, 0 }, {SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //55

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 1, 2 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //56

    {{SF_INVALID, 0, 0 }, {SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_VALID, 2, 3 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //57

    /* Invalid PRACH configurations (58 - 63) */
    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //58

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //59

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //60

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //61

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //62

    {{SF_INVALID, 0, 0 }, {SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0},{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 },{SF_INVALID, 0, 0 }},  //63
};

/******************************************************************************
 * Table: IsValidSFNForConfig0
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 0 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U U D S U U U
 *****************************************************************************/
UInt8 IsValidSFNForConfig0[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {1,1,1,1,1,1,1,1,1,1} /* 0 */,       
    {2,2,2,2,2,2,2,2,2,2} /* 1 */,        
    {1,1,1,1,1,1,1,1,1,1} /* 2 */,        
    {0,0,0,0,0,0,0,0,0,0} /* 3 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 4 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 5 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 6 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 7 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 8 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 9 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 10 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 11 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 12 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 13 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 14 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 15 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 16 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 17 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 18 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 19 */,  
    {1,1,1,1,1,1,1,1,1,1} /* 20 */, 
    {2,2,2,2,2,2,2,2,2,2},  
    {1,1,1,1,1,1,1,1,1,1} , 
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {1,1,1,1,1,1,1,1,1,1} ,
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {1,1,1,1,1,1,1,1,1,1} ,
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {1,1,1,1,1,1,1,1,1,1} /* 50 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 52 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

/******************************************************************************
 * Table: IsValidSFNForConfig1
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 0 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U D D S U U D
 *****************************************************************************/
UInt8 IsValidSFNForConfig1[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {1,1,1,1,1,1,1,1,1,1} ,       
    {2,2,2,2,2,2,2,2,2,2},       
    {1,1,1,1,1,1,1,1,1,1},        
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {1,1,1,1,1,1,1,1,1,1} , 
    {2,2,2,2,2,2,2,2,2,2},  
    {1,1,1,1,1,1,1,1,1,1} , 
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {1,1,1,1,1,1,1,1,1,1} ,
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {1,1,1,1,1,1,1,1,1,1} /* 50 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 52 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

/******************************************************************************
 * Table: IsValidSFNForConfig2
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 2 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U D D D S U D D
 *****************************************************************************/
UInt8 IsValidSFNForConfig2[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {1,1,1,1,1,1,1,1,1,1} ,       
    {2,2,2,2,2,2,2,2,2,2},        
    {1,1,1,1,1,1,1,1,1,1},        
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},   
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},   
    {3,3,3,3,3,3,3,3,3,3},   
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {1,1,1,1,1,1,1,1,1,1} /* 50 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 52 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

/******************************************************************************
 * Table: IsValidSFNForConfig3
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 3 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U U D D D D D
 *****************************************************************************/
UInt8 IsValidSFNForConfig3[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {0,0,0,0,0,0,0,0,0,0},  
    {2,2,2,2,2,2,2,2,2,2},        
    {1,1,1,1,1,1,1,1,1,1},        
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0}, 
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {1,1,1,1,1,1,1,1,1,1} , 
    {2,2,2,2,2,2,2,2,2,2},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {3,3,3,3,3,3,3,3,3,3}, 
    {0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,3,3,3,3,3}, 
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 50 */,
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 52 */,
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

/******************************************************************************
 * Table: IsValidSFNForConfig4
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 4 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U D D D D D D
 *****************************************************************************/
UInt8 IsValidSFNForConfig4[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {1,1,1,1,1,1,1,1,1,1} ,       
    {2,2,2,2,2,2,2,2,2,2},        
    {1,1,1,1,1,1,1,1,1,1},        
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {1,1,1,1,1,1,1,1,1,1} , 
    {2,2,2,2,2,2,2,2,2,2},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    {3,3,3,3,3,3,3,3,3,3}, 
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 50 */,
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 52 */,
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

/******************************************************************************
 * Table: IsValidSFNForConfig5
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 5 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U D D D D D D D
 *****************************************************************************/
UInt8 IsValidSFNForConfig5[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {1,1,1,1,1,1,1,1,1,1} ,       
    {2,2,2,2,2,2,2,2,2,2},        
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 50 */,
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 52 */,
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

/******************************************************************************
 * Table: IsValidSFNForConfig6
 * Description: Returns even/odd/all radio frame as per PRACH Config and UL SF 
 *              for TDD Config 6 as per Table 5.7.1-4, 36.211
 *              0 1 2 3 4 5 6 7 8 9
 *              -------------------
 *              D S U U U D S U U D
 *****************************************************************************/
UInt8 IsValidSFNForConfig6[MAX_PRACH_CONFIGURATION_INDEX][MAX_SUB_FRAME] = {
    {1,1,1,1,1,1,1,1,1,1} ,      
    {2,2,2,2,2,2,2,2,2,2},        
    {1,1,1,1,1,1,1,1,1,1},       
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},  
    {3,3,3,3,3,3,3,3,3,3},  
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0}, 
    {0,0,0,0,0,0,0,0,0,0},  
    {1,1,1,1,1,1,1,1,1,1} , 
    {2,2,2,2,2,2,2,2,2,2}, 
    {1,1,1,1,1,1,1,1,1,1} , 
    {0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {1,1,1,1,1,1,1,1,1,1} ,
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1} ,
    {2,2,2,2,2,2,2,2,2,2},
    {3,3,3,3,3,3,3,3,3,3},  
    {0,0,0,0,0,0,0,0,0,0},
    {3,3,3,3,3,3,3,3,3,3}, 
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    /* Preamble 4 PRACH configurations (48 - 57) */
    {1,1,1,1,1,1,1,1,1,1} /* 48 */,  
    {2,2,2,2,2,2,2,2,2,2} /* 49 */,  
    {1,1,1,1,1,1,1,1,1,1} /* 50 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 51 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 52 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 53 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 54 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 55 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 56 */,  
    {0,0,0,0,0,0,0,0,0,0} /* 57 */,  
    /* Invalid PRACH configurations (58 - 63) */
    {3,3,3,3,3,3,3,3,3,3} /* 58 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 59 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 60 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 61 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 62 */,  
    {3,3,3,3,3,3,3,3,3,3} /* 63 */,
};

UInt8 (* IsValidSFN_gp[UL_DL_CONFIG_MAX]) [MAX_SUB_FRAME] =
{
    IsValidSFNForConfig0,
    IsValidSFNForConfig1,
    IsValidSFNForConfig2,
    IsValidSFNForConfig3,
    IsValidSFNForConfig4,
    IsValidSFNForConfig5,
    IsValidSFNForConfig6
};

PRACHResInfo (* prachResForConfig_gp[UL_DL_CONFIG_MAX]) [MAX_SUB_FRAME] =
{
    prachResInfoForConfig0,
    prachResInfoForConfig1,
    prachResInfoForConfig2,
    prachResInfoForConfig3,
    prachResInfoForConfig4,
    prachResInfoForConfig5,
    prachResInfoForConfig6
};
#endif

/*****************************************************************************
 * Function Name  : updateRACHConfigInfo
 * Inputs         : sfn - Target System frame number from which new received 
 *                            values are applicable
 *                  cellConfigReqFlag  - This flag is set if MAC_CONFIG_CELL_REQ
 *                  raRspWindowSize - RA response window size
 *                  macContentionResTimer - Mac contention resolution timer
 *                  prachConfigIndex - PRACH configuration index 
 *                  prachFrequencyOffset - PRACH Frequency offset
 *                  numContnBasedPreambles -                  
 *                  preambleGroupBConfigPresent -
 *                  numOfPreamblesInGroupA -
 *                  messageSizeGroupA -
 *                  preambleInitialReceivedTargetPower -
 *                  maxHarqMsg3Tx -
 *                  rachConfigInfo_p -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will validated the received information and updates in
 *                  global static variable "rachConfigInfo_g".
 *****************************************************************************/
MacRetType updateRACHConfigInfo( UInt32  sfn,
        UInt32  cellConfigReqFlag,
        UInt32  raRspWindowSize,
        UInt32  macContentionResTimer,
        UInt32  prachConfigIndex,
        UInt32  prachFrequencyOffset,
        UInt32  numContnBasedPreambles,
        UInt32  preambleGroupBConfigPresent,
        UInt32  numOfPreamblesInGroupA,
        UInt32  messageSizeGroupA,
        UInt8   preambleInitialReceivedTargetPower,
        UInt8   maxHarqMsg3Tx,
        RACHConfigInfo *rachConfigInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 firstFreePreambleIndex = INVALID_PREAMBLE_IDX;
    UInt32 prachActiveIndex       = 0;
    UInt32 activeIdxRARspWindow   = 0;
    UInt32 activeIdxContentionRes = 0;
    UInt32 buffLenInBytes = 0;
    /* SPR 13250 fix start */
    UInt8  tbSize  = 0;
    /* SPR 13250 fix end */
    UInt8  numRB   = 0;
    UInt8  mcsIndex = 0;
    UInt8  minimumRBReqdForSingleRAR = 0;
    UInt8  maxRBReqdForAnyRAR = 0;
//    UInt8  tpc = 0;

    RACHConfiguration *rachConfig_p = 
        &(rachConfigInfo_p->rachConfiguration);

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;


    LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
            "Entry [%s]\n",__func__);

    if ((raRspWindowSize < RA_RSP_WINDOW_SIZE_2) || 
            ((raRspWindowSize > RA_RSP_WINDOW_SIZE_8) && 
             (RA_RSP_WINDOW_SIZE_10 != raRspWindowSize)))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "[%s]: ERROR Invalid raRspWindowSize [%d]\n",
                __func__,raRspWindowSize);

        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "Exit [%s]\n",__func__);

        return MAC_FAILURE;
    }

    if ((macContentionResTimer < CONTENTION_RES_TIMER_8) ||
            (macContentionResTimer > CONTENTION_RES_TIMER_64) ||
            ((MODULO_EIGHT(macContentionResTimer))))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "[%s]: ERROR Invalid macContentionResTimer [%d]\n",
                __func__,macContentionResTimer);

        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "Exit [%s]\n",__func__);

        return MAC_FAILURE;
    }

    if (prachConfigIndex > MAX_PRACH_CONFIGURATION_INDEX)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "[%s]: ERROR Invalid prachConfigIndex [%d]\n",
                __func__,prachConfigIndex);

        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "Exit [%s]\n",__func__);

        return MAC_FAILURE;
    }

    if (prachFrequencyOffset > MAX_PRACH_FREQUENCY_OFFSET)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "[%s]: ERROR Invalid prachFrequencyOffset [%d]\n",
                __func__,prachFrequencyOffset);

        LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                "Exit [%s]\n",__func__);

        return MAC_FAILURE;
    }
#ifdef TDD_CONFIG
    /* At this point we know the prach config index , UL/DL config index  say tddconfig_g contains the config.
       we will populate the data structure for PRACH here based on the UL/Dl TDD config
     */
#endif
    /*If flag is not set it,s Cell re-config case.
     *sfn is received in cell re-config only.
     */
    if (FALSE == cellConfigReqFlag) 
    {
        /*1)During the CELL RE-CONFIG time save the new received messages
         *  at array index which is not the current active index.
         *2)During the CELL CONFIG time no need to change the active index
         */
        prachActiveIndex       = (cellParams_p->rachConfigInfo.prachActiveIndex)?(0):(1);
        activeIdxRARspWindow   = 
            (rachConfig_p->activeIdxRARspWindow)?(0):(1);
        activeIdxContentionRes = 
            (rachConfig_p->activeIdxContentionRes)?(0):(1);

        if (sfn > 1024)
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                    "[%s]: ERROR Invalid sfn [%d]\n",
                    __func__,sfn);

            LTE_MAC_UT_LOG(LOG_DETAIL,PRACH_MGR,
                    "Exit [%s]\n",__func__);

            return MAC_FAILURE;        
        }

        cellParams_p->rachConfigInfo.sfn = sfn;
        cellParams_p->rachConfigInfo.prachUpdateFlag = TRUE;
        rachConfig_p->raRspWindowUpdateFlag = TRUE;
        rachConfig_p->contentionResUpdateFlag = TRUE;
    }

    cellParams_p->rachConfigInfo.prachConfiguration[prachActiveIndex].prachConfigIndex = 
        prachConfigIndex;
    cellParams_p->rachConfigInfo.prachConfiguration[prachActiveIndex].prachFrequencyOffset = 
        prachFrequencyOffset;
    rachConfig_p->macContentionResTimer[activeIdxContentionRes] = 
        macContentionResTimer;
#ifdef FDD_CONFIG
    /* + SPR 17733 */
    rachConfig_p->msg4MaxHarqRetransmission[activeIdxContentionRes] = 
        (macContentionResTimer/8);
    /* - SPR 17733 */
#endif
    rachConfig_p->raRspWindowSize[activeIdxRARspWindow] = 
        raRspWindowSize;
    rachConfig_p->countOfContentionBasedRAPreambles  = 
        numContnBasedPreambles;

    /*SPR_3305_FIX_START*/
    rachConfig_p->maxHarqMsg3Tx = maxHarqMsg3Tx;
    /*SPR_3305_FIX_END*/
    firstFreePreambleIndex = numContnBasedPreambles;

    /* + Coverity 24507 */
    if(MAC_FAILURE == initContentionFreePreamblePool(firstFreePreambleIndex, internalCellIndex))
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24507 */

    rachConfig_p->preambleGroupBConfigPresent  = 
        preambleGroupBConfigPresent;

    if(preambleGroupBConfigPresent)
    {    

        rachConfig_p->numOfPreamblesInGroupA  = 
            numOfPreamblesInGroupA;

        rachConfig_p->messageSizeGroupA  =
            messageSizeGroupA;

        buffLenInBytes = 
            ((rachConfig_p->messageSizeGroupA)/ 8);

        /* SPR 22845 fix start*/

        determineRBFromTBSizeForGrant
            (buffLenInBytes,
             &tbSize,
             &numRB,
             &mcsIndex);

        /* SPR 22845 fix end*/
        rachConfig_p->mcsIndexForMSG3GroupARACHRequest  = 
            mcsIndex;
        rachConfig_p->numRBReqdForMSG3GroupARACHRequest  = 
            numRB;

        minimumRBReqdForSingleRAR = numRB;
        maxRBReqdForAnyRAR = numRB;

        buffLenInBytes = 
            ((rachConfig_p->messageSizeGroupA +
              PREAMBLE_GROUPB_MSG_SIZE_DELTA) /8);
        tbSize = 0;
        numRB = 0;
        mcsIndex = 0;

        /* SPR 22845 fix start*/

        determineRBFromTBSizeForGrant
            (buffLenInBytes,
             &tbSize,
             &numRB,
             &mcsIndex);
        /* SPR 22845 fix end*/

        rachConfig_p->mcsIndexForMSG3GroupBRACHRequest  = 
            mcsIndex;
        rachConfig_p->numRBReqdForMSG3GroupBRACHRequest  = 
            numRB;

        if(numRB > maxRBReqdForAnyRAR)
        {
            maxRBReqdForAnyRAR = numRB;
        }
        if(numRB < minimumRBReqdForSingleRAR)
        {
            minimumRBReqdForSingleRAR = numRB;
        }
    }
    else
    {
        rachConfig_p->messageSizeGroupA  = 
            DEFAULT_PREAMBLE_GROUPA_MSG_SIZE;

        buffLenInBytes = 
            ((rachConfig_p->messageSizeGroupA)/ 8);

        tbSize = 0;
        numRB = 0;
        mcsIndex = 0;

        /* SPR 22845 fix start*/
        determineRBFromTBSizeForGrant
            (buffLenInBytes,
             &tbSize,
             &numRB,
             &mcsIndex);
        /* SPR 22845 fix end*/

        rachConfig_p->mcsIndexForMSG3GroupARACHRequest  = 
            mcsIndex;
        rachConfig_p->numRBReqdForMSG3GroupARACHRequest  = 
            numRB;

        minimumRBReqdForSingleRAR = numRB;
        maxRBReqdForAnyRAR = numRB;
    }

    if(numContnBasedPreambles < MAC_MAX_NUM_OF_RA_PREAMBLES)
    {

        buffLenInBytes = CON_FREE_HAND_OVER_GRANT_BYTES; 
        tbSize = 0;
        numRB = 0;
        mcsIndex = 0;

        /* SPR 22845 fix start*/

        determineRBFromTBSizeForGrant
            (buffLenInBytes,
             &tbSize,
             &numRB,
             &mcsIndex);
        /* SPR 22845 fix end*/

        rachConfig_p->mcsIndexForConFreeHandOver  = 
            mcsIndex;
        rachConfig_p->numRBReqdForConFreeHandOver  = 
            numRB;

        if(numRB > maxRBReqdForAnyRAR)
        {
            maxRBReqdForAnyRAR = numRB;
        }
        if(numRB < minimumRBReqdForSingleRAR)
        {
            minimumRBReqdForSingleRAR = numRB;
        }

        buffLenInBytes = CON_FREE_PDCCH_ORDER_GRANT_BYTES; 

        tbSize = 0;
        numRB = 0;
        mcsIndex = 0;

        /* SPR 22845 fix start*/

        determineRBFromTBSizeForGrant
            (buffLenInBytes,
             &tbSize,
             &numRB,
             &mcsIndex);
        /* SPR 22845 fix end*/

        rachConfig_p->mcsIndexForConFreePDCCHOrder  = 
            mcsIndex;
        rachConfig_p->numRBReqdForConFreePDCCHOrder  = 
            numRB;

        if(numRB > maxRBReqdForAnyRAR)
        {
            maxRBReqdForAnyRAR = numRB;
        }
        if(numRB < minimumRBReqdForSingleRAR)
        {
            minimumRBReqdForSingleRAR = numRB;
        }
    }

    rachConfig_p->maxRBReqdForAnyRAR = maxRBReqdForAnyRAR;
    rachConfig_p->minimumRBReqdForSingleRAR = minimumRBReqdForSingleRAR;
    rachConfig_p->preambleInitialReceivedTargetPower = \
                                                       preambleInitialReceivedTargetPower;


    LOG_MAC_MSG(PREAMBLE_GROUP_A_CONFIG_INFO,LOGINFO,MAC_RRC_INF, 
            getCurrentTick(),
            rachConfig_p->countOfContentionBasedRAPreambles,
            rachConfig_p->numOfPreamblesInGroupA,
            rachConfig_p->messageSizeGroupA,
            rachConfig_p->mcsIndexForMSG3GroupARACHRequest,
            rachConfig_p->numRBReqdForMSG3GroupARACHRequest,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, 
            FUNCTION_NAME,"PREAMBLE_GROUP_A_CONFIG_INFO");

    LOG_MAC_MSG(PREAMBLE_GROUP_B_CONFIG_INFO,LOGINFO,MAC_RRC_INF, 
            getCurrentTick(),
            rachConfig_p->preambleGroupBConfigPresent,
            rachConfig_p->mcsIndexForMSG3GroupBRACHRequest,
            rachConfig_p->numRBReqdForMSG3GroupBRACHRequest,
            (rachConfig_p->countOfContentionBasedRAPreambles - 
             rachConfig_p->numOfPreamblesInGroupA),
            rachConfig_p->minimumRBReqdForSingleRAR,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, 
            FUNCTION_NAME,"PREAMBLE_GROUP_B_CONFIG_INFO");

    LOG_MAC_MSG(CONTENTION_FREE_PREAMBLE_CONFIG,LOGINFO,MAC_RRC_INF, 
            getCurrentTick(),
            rachConfig_p->mcsIndexForConFreeHandOver,
            rachConfig_p->numRBReqdForConFreeHandOver,
            rachConfig_p->mcsIndexForConFreePDCCHOrder,
            rachConfig_p->numRBReqdForConFreePDCCHOrder,
            rachConfig_p->maxRBReqdForAnyRAR,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, 
            FUNCTION_NAME,"CONTENTION_FREE_PREAMBLE_CONFIG");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : isValidFrameForPRACH
 * Inputs         : subFrame - Sub frame number
 *                  sysFrame - System frame number
 * Outputs        : prachFrequencyOffset_p - physical resources block number
 *                  numPhysicalResources_p - Number of physical resources blocks
 *                                           to be allocated
 *                  internalCellIndex - internal cell index of L2
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will check wether to allocate UL RBs for PRACH
 *                  and returns the physical resources block number and number
 *                  of physical resources to be allocated
 *****************************************************************************/
MacRetType  isValidFrameForPRACH( UInt8  subFrame,
        UInt16 sysFrame,
        UInt32 *prachFrequencyOffset_p,
        UInt32 *numPhysicalResources_p,
        InternalCellIndex internalCellIndex)
{
    PRACHConfiguration *prachConfig_p  = PNULL;
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
        cellParams_p;

    UPDATE_PRACH_CONFIGURATION_ACTIVE_INDEX( cellParams_p->rachConfigInfo, 
            sysFrame );
    prachConfig_p =
        &cellParams_p->rachConfigInfo.prachConfiguration[
        cellParams_p->rachConfigInfo.prachActiveIndex];



#ifdef FDD_CONFIG
    PRACHConfigInfo    *prachConfigInfo_p = 
        &prachConfig[prachConfig_p->prachConfigIndex]; 
    if ((ANY == prachConfigInfo_p->prachSFN) ||
            (IS_EVEN(sysFrame)))
    {
        if (prachConfigInfo_p->subFrame[subFrame])
        {
            *prachFrequencyOffset_p = prachConfig_p->prachFrequencyOffset;
            *numPhysicalResources_p = NUM_PRACH_PHYSICAL_RESOURCES;
        }
        else
        {
            /*no need to allocate PRACH resources in this SFN and SF*/
            return MAC_FAILURE;
        }
    }
    else
    {
        /*no need to allocate PRACH resources in this SFN and SF*/
        return MAC_FAILURE;
    }
#endif

#ifdef TDD_CONFIG
    if (((IsValidSFN_gp[cellParams_p->subFrameAssign][prachConfig_p->prachConfigIndex][subFrame] != SFN_INVALID) &&
                (IsValidSFN_gp[cellParams_p->subFrameAssign][prachConfig_p->prachConfigIndex][subFrame] == SFN_ALL)) ||
            ((IsValidSFN_gp[cellParams_p->subFrameAssign][prachConfig_p->prachConfigIndex][subFrame] == SFN_EVEN)&&
             (IS_EVEN(sysFrame)))||
            ((IsValidSFN_gp[cellParams_p->subFrameAssign][prachConfig_p->prachConfigIndex][subFrame] == SFN_ODD)&&
             (IS_ODD(sysFrame))))
    {
        if( SF_VALID == prachResForConfig_gp[cellParams_p->subFrameAssign]
                [prachConfig_p->prachConfigIndex][subFrame].ValidityFlag )
        {
            *prachFrequencyOffset_p = prachConfig_p->prachFrequencyOffset;
            *numPhysicalResources_p = 
                prachResForConfig_gp[cellParams_p->subFrameAssign]
                [prachConfig_p->prachConfigIndex][subFrame].PRACHResCount * 
                NUM_PRACH_PHYSICAL_RESOURCES ;
        }
        else
        {
            /* PRACH resources are not present in this subframe                */
            return MAC_FAILURE;
        }
    }

    else
    {
        /*no need to allocate PRACH resources in this SFN and SF*/
        return MAC_FAILURE;
    }
#endif
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : preambleFormatFrmPrachConfigIndex
 * Inputs         : prachConfigIndex in case of TDD only
 *                  internalCellIndex - cell index of L2
 * Outputs        : None 
 * Returns        : preamble format corresponding to PRACH Config Index
 * Description    : This function will return the Preamble format corresponding
 *                  to PRACH Config Index
 *****************************************************************************/
 /* +- SPR 17777 */
#ifdef FDD_CONFIG
PreambleFormat preambleFormatFrmPrachConfigIndex(InternalCellIndex internalCellIndex) 
#else
PreambleFormat preambleFormatFrmPrachConfigIndex(UInt8 prachConfigIndex) 
 /* +- SPR 17777 */
#endif 
 /* +- SPR 17777 */
{
#ifdef FDD_CONFIG
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
        cellParams_p;

    PRACHConfiguration *prachConfig_p =
        &cellParams_p->rachConfigInfo.prachConfiguration[
        cellParams_p->rachConfigInfo.prachActiveIndex];
    PRACHConfigInfo    *prachConfigInfo_p = 
        &prachConfig[prachConfig_p->prachConfigIndex];

    return prachConfigInfo_p->preambleFormat;
#elif TDD_CONFIG
    return preambleFrmPrachConfig_g[ prachConfigIndex ];
#endif
}

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : isPrachConfigValid
 * Inputs         : prachConfigIndex, tddConfig
 * Outputs        : None
 * Returns        : TRUE/FALSE
 * Description    : This function will return if Prach Config Index received 
 *                  is valid wrt TDD UL/DL config 0 - 6
 *****************************************************************************/
UInt8 isPrachConfigValid( UInt8 prachConfigIndex, UInt8 tddConfig )
{
    return( prachConfigValid[prachConfigIndex][tddConfig] );
}

/* SPR 11489 fix start */
/*****************************************************************************
 * Function Name  : validateFraWrtResBlks
 * Inputs         : ulResBlocks - UL Resource Blocks received from L3
 *                  subFrameAssign 
 *                  prachConfigIndex
 *                  prachFrequencyOffset
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function validates if the number of RBs required for 
 *                  PRACH can be allocated for PRACH configuration Index for
 *                  all TDD UL/DL config 0 - 6 and returns failure at the time
 *                  of cell setup only.
 *****************************************************************************/
MacRetType validateFraWrtResBlks( UInt8 ulResBlocks, UInt8 subFrameAssign,
    UInt8 prachConfigIndex, UInt8 prachFrequencyOffset )
{
    UInt8 subFrame = 0;
    UInt8 prachResCount = 0;
    UInt8 numRbsReqd = 0;
    MacRetType retVal = MAC_FAILURE;
 
    for( subFrame = 0; subFrame < MAX_SUB_FRAME; subFrame++ )
    {
        if( SF_VALID == prachResForConfig_gp[subFrameAssign]
                 [prachConfigIndex][subFrame].ValidityFlag )
        {
            prachResCount = prachResForConfig_gp[subFrameAssign]
                [prachConfigIndex][subFrame].PRACHResCount ;
            numRbsReqd = NUM_PRACH_PHYSICAL_RESOURCES * prachResCount;

            if( numRbsReqd <= ulResBlocks && 
                    prachFrequencyOffset <= (ulResBlocks - numRbsReqd ) )
            {
                retVal = MAC_SUCCESS;
                break;
            }
            else
            {
                LOG_MAC_MSG( MAC_PRACH_RB_MORE, LOGWARNING, MAC_RRC_INF, 
                        getCurrentTick(), subFrameAssign, numRbsReqd, 
                        ulResBlocks, prachFrequencyOffset, prachConfigIndex, 
                        0,0, __func__, "" );
            }
        }
    }
    return retVal;
}
/* SPR 11489 fix end */

/*****************************************************************************
 * Function Name  : updateUplinkRbMapForPrach
 * Inputs         : prachConfigIndex, NraOffset, rbMapNum, ulSubFrame,
 *                  internalCellIndex
 * Outputs        : 
 * Returns        : Void
 * Description    : This function will update the Ul RB map for PRACH resources
 *                  wrt TDD UL/DL config 0 - 6
 *****************************************************************************/
/* SPR 1540 changes start */
void updateUplinkRbMapForPrach( UInt8 prachConfigIndex,
        UInt32  NraOffset, UInt32 rbMapNum, UInt32 ulSubFrame,
        InternalCellIndex internalCellIndex)
/* SPR 1540 changes end */
{
    UInt8 Fra;
    UInt32 NraPrb;
    UInt32 NulRb = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->ulAvailableRBs;
    UInt8 subFrameAssign = cellSpecificParams_g.cellConfigAndInitParams_p
        [internalCellIndex]->cellParams_p->subFrameAssign;
    /* SPR 1540 changes start */
    /* SPR 4193 chg */
    
    if( TRUE == prachResForConfig_gp[subFrameAssign]
            [prachConfigIndex][ulSubFrame].ValidityFlag )
    {
        /* Allocation shall be done here on this RB as per the no of the PRACH
         * resources required  to be allocated based on the prach config index
         * and the subframe . and where these PRACH resources need to be
         * allocated in the frequency spectrum.
         */
        UInt8 freqResIndx = prachResForConfig_gp[subFrameAssign][
            prachConfigIndex][ulSubFrame].FreqResIndx;

        /* SPR 1540 changes start */
        for(Fra = 0; Fra <= freqResIndx; Fra++)
            /* SPR 1540 changes end */
        {
            /* SPR 4535 changes */
            if (IS_EVEN(Fra))
                /* SPR 4535 changes */
            {
                /* SPR 18122 Changes Start*/
                NraPrb = NraOffset + NUM_PRACH_PHYSICAL_RESOURCES * 
                    floor_wrapper(DIVIDE_BY_TWO(Fra));
                    /* SPR 18122 Changes End*/
            }
            else
            {
                /* SPR 18122 Changes Start*/
                NraPrb = NulRb - NUM_PRACH_PHYSICAL_RESOURCES - NraOffset - 
                    NUM_PRACH_PHYSICAL_RESOURCES * floor_wrapper(DIVIDE_BY_TWO(Fra));
                    /* SPR 18122 Changes End*/
            }

            /* Allocate the resources for this RB Map */
            allocateResourcesUsingRBIndex( NraPrb, 
                    NUM_PRACH_PHYSICAL_RESOURCES, rbMapNum, 
                    internalCellIndex );
        }
    }
    /* SPR 4193 chg */
}
#endif




