/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2TBSize.c,v 1.1.1.1.16.1 2010/09/21 15:53:27 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :This file defines the functionality for calculating
 *                    the Transport block size for the given MCS Index and 
 *                    number of Resource Block.
 *                     
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2TBSize.c,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:27  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5  2009/09/16 11:46:14  gur19413
 * optimization changes
 *
 * Revision 1.4  2009/09/03 14:25:06  gur20439
 * ltePanic put in macro LTE_MAC_UT_LOG.
 *
 * Revision 1.3  2009/07/14 14:50:03  gur20439
 * rrc interface code.
 *
 * Revision 1.2  2009/07/12 08:08:47  gur15697
 * tbSize changed to tbSize >> 3
 *
 * Revision 1.1  2009/03/30 10:20:34  gur19836
 * Initial Version
 *
 * Revision 1.13  2009/03/25 07:18:41  gur20439
 * LTE_LOG macro called modified as per the new macro.
 *
 * Revision 1.12  2009/02/24 14:01:40  gur11912
 * Optimization Changes
 *
 * Revision 1.11  2009/02/09 07:02:58  gur11912
 * TBS Index used instead of mcsIndex in determineRBs
 *
 * Revision 1.10  2009/01/25 11:48:20  gur20052
 * Added during Regression tests
 *
 * Revision 1.9  2008/12/23 07:48:56  gur20052
 * Phase 2 review comments incorporated
 *
 * Revision 1.8  2008/12/19 06:56:44  gur18569
 * review comments incorporated for phase2
 *
 * Revision 1.7  2008/12/11 14:17:06  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.6  2008/12/08 12:59:11  gur12905
 * Modified during phase-2 merge
 *
 * Revision 1.5  2008/12/05 13:45:40  gur18569
 * tb look up table updated
 *
 * Revision 1.4  2008/10/22 12:30:03  gur11912
 * fixed for performance changes
 *
 * Revision 1.3  2008/09/27 15:16:16  gur11912
 * UT Defect Fixed
 *
 * Revision 1.2  2008/09/19 07:11:38  gur11912
 * review comments incorporated
 *
 * Revision 1.1  2008/09/04 06:22:14  gur11912
 * Initial Version
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2TBSize.h"
#include "lteLayer2RRCInterface.h"
/****************************************************************************
 Private Definitions
 ****************************************************************************/

/****************************************************************************
 Private Types
 ****************************************************************************/
UInt8 TBSArr[MAX_MCS_INDEX] = {0,1,2,3,4,5,6,7,8,9,9,10,11,12,13,14,15,15,16,
                               17,18,19,20,21,22,23,24,25,26};


UInt32 TBSizeArr[MAX_IBS_INDEX][MAX_RESOURCE_BLOCK] =
{
     {    16,    32,    56,    88,    120,    152,    176,    208,    224,    256,
        288,    328,    344,    376,    392,    424,    456,    488,    504,    536,
        568,    600,    616,    648,    680,    712,    744,    776,    776,    808,
        840,    872,    904,    936,    968,    1000,    1032,    1032,    1064,    1096,
        1128,    1160,    1192,    1224,    1256,    1256,    1288,    1320,    1352,    1384,
        1416,    1416,    1480,    1480,    1544,    1544,    1608,    1608,    1608,    1672,
        1672,    1736,    1736,    1800,    1800,    1800,    1864,    1864,    1928,    1928,
        1992,    1992,    2024,    2088,    2088,    2088,    2152,    2152,    2216,    2216,
        2280,    2280,    2280,    2344,    2344,    2408,    2408,    2472,    2472,    2536,
        2536,    2536,    2600,    2600,    2664,    2664,    2728,    2728,    2728,    2792,
        2792,    2856,    2856,    2856,    2984,    2984,    2984,    2984,    2984,    3112
         },
     {    24,    56,    88,    144,    176,    208,    224,    256,    328,    344,
        376,    424,    456,    488,    520,    568,    600,    632,    680,    712,
        744,    776,    808,    872,    904,    936,    968,    1000,    1032,    1064,
        1128,    1160,    1192,    1224,    1256,    1288,    1352,    1384,    1416,    1416,
        1480,    1544,    1544,    1608,    1608,    1672,    1736,    1736,    1800,    1800,
        1864,    1864,    1928,    1992,    1992,    2024,    2088,    2088,    2152,    2152,
        2216,    2280,    2280,    2344,    2344,    2408,    2472,    2472,    2536,    2536,
        2600,    2600,    2664,    2728,    2728,    2792,    2792,    2856,    2856,    2856,
        2984,    2984,    2984,    3112,    3112,    3112,    3240,    3240,    3240,    3240,
        3368,    3368,    3368,    3496,    3496,    3496,    3496,    3624,    3624,    3624,
        3752,    3752,    3752,    3752,    3880,    3880,    3880,    4008,    4008,    4008
         },
     {    32,    72,    144,    176,    208,    256,    296,    328,    376,    424,
        472,    520,    568,    616,    648,    696,    744,    776,    840,    872,
        936,    968,    1000,    1064,    1096,    1160,    1192,    1256,    1288,    1320,
        1384,    1416,    1480,    1544,    1544,    1608,    1672,    1672,    1736,    1800,
        1800,    1864,    1928,    1992,    2024,    2088,    2088,    2152,    2216,    2216,
        2280,    2344,    2344,    2408,    2472,    2536,    2536,    2600,    2664,    2664,
        2728,    2792,    2856,    2856,    2856,    2984,    2984,    3112,    3112,    3112,
        3240,    3240,    3240,    3368,    3368,    3368,    3496,    3496,    3496,    3624,
        3624,    3624,    3752,    3752,    3880,    3880,    3880,    4008,    4008,    4008,
        4136,    4136,    4136,    4264,    4264,    4264,    4392,    4392,    4392,    4584,
        4584,    4584,    4584,    4584,    4776,    4776,    4776,    4776,    4968,    4968
        },
    {    40,    104,    176,    208,    256,    328,    392,    440,    504,    568,
        616,    680,    744,    808,    872,    904,    968,    1032,    1096,    1160,
        1224,    1256,    1320,    1384,    1416,    1480,    1544,    1608,    1672,    1736,
        1800,    1864,    1928,    1992,    2024,    2088,    2152,    2216,    2280,    2344,
        2408,    2472,    2536,    2536,    2600,    2664,    2728,    2792,    2856,    2856,
        2984,    2984,    3112,    3112,    3240,    3240,    3368,    3368,    3496,    3496,
        3624,    3624,    3624,    3752,    3752,    3880,    3880,    4008,    4008,    4136,
        4136,    4264,    4264,    4392,    4392,    4392,    4584,    4584,    4584,    4776,
        4776,    4776,    4776,    4968,    4968,    4968,    5160,    5160,    5160,    5352,
        5352,    5352,    5352,    5544,    5544,    5544,    5736,    5736,    5736,    5736,
        5992,    5992,    5992,    5992,    6200,    6200,    6200,    6200,    6456,    6456
         },

    {    56,    120,    208,    256,    328,    408,    488,    552,    632,    696,
        776,    840,    904,    1000,    1064,    1128,    1192,    1288,    1352,    1416,
        1480,    1544,    1608,    1736,    1800,    1864,    1928,    1992,    2088,    2152,
        2216,    2280,    2344,    2408,    2472,    2600,    2664,    2728,    2792,    2856,
        2984,    2984,    3112,    3112,    3240,    3240,    3368,    3496,    3496,    3624,
        3624,    3752,    3752,    3880,    4008,    4008,    4136,    4136,    4264,    4264,
        4392,    4392,    4584,    4584,    4584,    4776,    4776,    4968,    4968,    4968,
        5160,    5160,    5160,    5352,    5352,    5544,    5544,    5544,    5736,    5736,
        5736,    5992,    5992,    5992,    5992,    6200,    6200,    6200,    6456,    6456,
        6456,    6456,    6712,    6712,    6712,    6968,    6968,    6968,    6968,    7224,
        7224,    7224,    7480,    7480,    7480,    7480,    7736,    7736,    7736,    7992
        },
    {    72,    144,    224,    328,    424,    504,    600,    680,    776,    872,
        968,    1032,    1128,    1224,    1320,    1384,    1480,    1544,    1672,    1736,
        1864,    1928,    2024,    2088,    2216,    2280,    2344,    2472,    2536,    2664,
        2728,    2792,    2856,    2984,    3112,    3112,    3240,    3368,    3496,    3496,
        3624,    3752,    3752,    3880,    4008,    4008,    4136,    4264,    4392,    4392,
        4584,    4584,    4776,    4776,    4776,    4968,    4968,    5160,    5160,    5352,
        5352,    5544,    5544,    5736,    5736,    5736,    5992,    5992,    5992,    6200,
        6200,    6200,    6456,    6456,    6712,    6712,    6712,    6968,    6968,    6968,
        7224,    7224,    7224,    7480,    7480,    7480,    7736,    7736,    7736,    7992,
        7992,    7992,    8248,    8248,    8248,    8504,    8504,    8760,    8760,    8760,
        8760,    9144,    9144,    9144,    9144,    9528,    9528,    9528,    9528,    9528
       },

    {    328,    176,    256,    392,    504,    600,    712,    808,    936,    1032,
        1128,    1224,    1352,    1480,    1544,    1672,    1736,    1864,    1992,    2088,
        2216,    2280,    2408,    2472,    2600,    2728,    2792,    2984,    2984,    3112,
        3240,    3368,    3496,    3496,    3624,    3752,    3880,    4008,    4136,    4136,
        4264,    4392,    4584,    4584,    4776,    4776,    4968,    4968,    5160,    5160,
        5352,    5352,    5544,    5736,    5736,    5992,    5992,    5992,    6200,    6200,
        6456,    6456,    6456,    6712,    6712,    6968,    6968,    6968,    7224,    7224,
        7480,    7480,    7736,    7736,    7736,    7992,    7992,    8248,    8248,    8248,
        8504,    8504,    8760,    8760,    8760,    9144,    9144,    9144,    9144,    9528,
        9528,    9528,    9528,    9912,    9912,    9912,    10296,    10296,    10296,    10296,
        10680,    10680,    10680,    10680,    11064,    11064,    11064,    11448,    11448,    11448
        },
    {    104,    224,    328,    472,    584,    712,    840,    968,    1096,    1224,
        1320,    1480,    1608,    1672,    1800,    1928,    2088,    2216,    2344,    2472,
        2536,    2664,    2792,    2984,    3112,    3240,    3368,    3368,    3496,    3624,
        3752,    3880,    4008,    4136,    4264,    4392,    4584,    4584,    4776,    4968,
        4968,    5160,    5352,    5352,    5544,    5736,    5736,    5992,    5992,    6200,
        6200,    6456,    6456,    6712,    6712,    6712,    6968,    6968,    7224,    7224,
        7480,    7480,    7736,    7736,    7992,    7992,    8248,    8248,    8504,    8504,
        8760,    8760,    8760,    9144,    9144,    9144,    9528,    9528,    9528,    9912,
        9912,    9912,    10296,    10296,    10296,    10680,    10680,    10680,    11064,    11064,
        11064,    11448,    11448,    11448,    11448,    11832,    11832,    11832,    12216,    12216,
        12216,    12576,    12576,    12576,    12960,    12960,    12960,    12960,    13536,    13536
        },
    {    120,    256,    392,    536,    680,    808,    968,    1096,    1256,    1384,
        1544,    1672,    1800,    1928,    2088,    2216,    2344,    2536,    2664,    2792,
        2984,    3112,    3240,    3368,    3496,    3624,    3752,    3880,    4008,    4264,
        4392,    4584,    4584,    4776,    4968,    4968,    5160,    5352,    5544,    5544,
        5736,    5992,    5992,    6200,    6200,    6456,    6456,    6712,    6968,    6968,
        7224,    7224,    7480,    7480,    7736,    7736,    7992,    7992,    8248,    8504,
        8504,    8760,    8760,    9144,    9144,    9144,    9528,    9528,    9528,    9912,
        9912,    9912,    10296,    10296,    10680,    10680,    10680,    11064,    11064,    11064,
        11448,    11448,    11448,    11832,    11832,    12216,    12216,    12216,    12576,    12576,
        12576,    12960,    12960,    12960,    13536,    13536,    13536,    13536,    14112,    14112,
        14112,    14112,    14688,    14688,    14688,    14688,    15264,    15264,    15264,    15264
        },

    {    136,    296,    456,    616,    776,    936,    1096,    1256,    1416,    1544,
        1736,    1864,    2024,    2216,    2344,    2536,    2664,    2856,    2984,    3112,
        3368,    3496,    3624,    3752,    4008,    4136,    4264,    4392,    4584,    4776,
        4968,    5160,    5160,    5352,    5544,    5736,    5736,    5992,    6200,    6200,
        6456,    6712,    6712,    6968,    6968,    7224,    7480,    7480,    7736,    7992,
        7992,    8248,    8248,    8504,    8760,    8760,    9144,    9144,    9144,    9528,
        9528,    9912,    9912,    10296,    10296,    10296,    10680,    10680,    11064,    11064,
        11064,    11448,    11448,    11832,    11832,    11832,    12216,    12216,    12576,    12576,
        12960,    12960,    12960,    13536,    13536,    13536,    13536,    14112,    14112,    14112,
        14112,    14688,    14688,    14688,    15264,    15264,    15264,    15264,    15840,    15840,
        15840,    16416,    16416,    16416,    16416,    16992,    16992,    16992,    16992,    17568
        },
    {    144,    328,    504,    680,    872,    1032,    1224,    1384,    1544,    1736,
        1928,    2088,    2280,    2472,    2664,    2792,    2984,    3112,    3368,    3496,
        3752,    3880,    4008,    4264,    4392,    4584,    4776,    4968,    5160,    5352,
        5544,    5736,    5736,    5992,    6200,    6200,    6456,    6712,    6712,    6968,
        7224,    7480,    7480,    7736,    7992,    7992,    8248,    8504,    8504,    8760,
        9144,    9144,    9144,    9528,    9528,    9912,    9912,    10296,    10296,    10680,
        10680,    11064,    11064,    11448,    11448,    11448,    11832,    11832,    12216,    12216,
        12576,    12576,    12960,    12960,    12960,    13536,    13536,    13536,    14112,    14112,
        14112,    14688,    14688,    14688,    14688,    15264,    15264,    15264,    15840,    15840,
        15840,    16416,    16416,    16416,    16992,    16992,    16992,    16992,    17568,    17568,
        17568,    18336,    18336,    18336,    18336,    18336,    19080,    19080,    19080,    19080 
        },
    {    176,    376,    584,    776,    1000,    1192,    1384,    1608,    1800,    2024,
        2216,    2408,    2600,    2792,    2984,    3240,    3496,    3624,    3880,    4008,
        4264,    4392,    4584,    4776,    4968,    5352,    5544,    5736,    5992,    5992,
        6200,    6456,    6712,    6968,    6968,    7224,    7480,    7736,    7736,    7992,
        8248,    8504,    8760,    8760,    9144,    9144,    9528,    9528,    9912,    9912,
        10296,    10680,    10680,    11064,    11064,    11448,    11448,    11832,    11832,    12216,
        12216,    12576,    12576,    12960,    12960,    13536,    13536,    13536,    14112,    14112,
        14112,    14688,    14688,    14688,    15264,    15264,    15840,    15840,    15840,    16416,
        16416,    16416,    16992,    16992,    16992,    17568,    17568,    17568,    18336,    18336,
        18336,    18336,    19080,    19080,    19080,    19080,    19848,    19848,    19848,    19848,
        20616,    20616,    20616,    21384,    21384,    21384,    21384,    22152,    22152,    22152
        },
    {    208,    440,    680,    904,    1128,    1352,    1608,    1800,    2024,    2280,
        2472,    2728,    2984,    3240,    3368,    3624,    3880,    4136,    4392,    4584,
        4776,    4968,    5352,    5544,    5736,    5992,    6200,    6456,    6712,    6712,
        6968,    7224,    7480,    7736,    7992,    8248,    8504,    8760,    8760,    9144,
        9528,    9528,    9912,    9912,    10296,    10680,    10680,    11064,    11064,    11448,
        11832,    11832,    12216,    12216,    12576,    12576,    12960,    12960,    13536,    13536,
        14112,    14112,    14112,    14688,    14688,    15264,    15264,    15264,    15840,    15840,
        16416,    16416,    16416,    16992,    16992,    17568,    17568,    17568,    18336,    18336,
        18336,    19080,    19080,    19080,    19080,    19848,    19848,    19848,    20616,    20616,
        20616,    21384,    21384,    21384,    21384,    22152,    22152,    22152,    22920,    22920,
        22920,    23688,    23688,    23688,    23688,    24496,    24496,    24496,    24496,    25456
        },
    {    224,    488,    744,    1000,    1256,    1544,    1800,    2024,    2280,    2536,
        2856,    3112,    3368,    3624,    3880,    4136,    4392,    4584,    4968,    5160,
        5352,    5736,    5992,    6200,    6456,    6712,    6968,    7224,    7480,    7736,
        7992,    8248,    8504,    8760,    9144,    9144,    9528,    9912,    9912,    10296,
        10680,    10680,    11064,    11448,    11448,    11832,    12216,    12216,    12576,    12960,
        12960,    13536,    13536,    14112,    14112,    14688,    14688,    14688,    15264,    15264,
        15840,    15840,    16416,    16416,    16992,    16992,    16992,    17568,    17568,    18336,
        18336,    18336,    19080,    19080,    19080,    19848,    19848,    19848,    20616,    20616,
        20616,    21384,    21384,    21384,    22152,    22152,    22152,    22920,    22920,    22920,
        23688,    23688,    23688,    24496,    24496,    24496,    25456,    25456,    25456,    25456,
        26416,    26416,    26416,    26416,    27376,    27376,    27376,    27376,    28336,    28336
        },
        {
        256,    552,    840,    1128,    1416,    1736,    1992,    2280,    2600,    2856,
        3112,    3496,    3752,    4008,    4264,    4584,    4968,    5160,    5544,    5736,
        5992,    6200,    6456,    6968,    7224,    7480,    7736,    7992,    8248,    8504,
        8760,    9144,    9528,    9912,    9912,    10296,    10680,    11064,    11064,    11448,
        11832,    12216,    12216,    12576,    12960,    12960,    13536,    13536,    14112,    14112,
        14688,    14688,    15264,    15264,    15840,    15840,    16416,    16416,    16992,    16992,
        17568,    17568,    18336,    18336,    18336,    19080,    19080,    19848,    19848,    19848,
        20616,    20616,    20616,    21384,    21384,    22152,    22152,    22152,    22920,    22920,
        22920,    23688,    23688,    24496,    24496,    24496,    25456,    25456,    25456,    25456,
        26416,    26416,    26416,    27376,    27376,    27376,    28336,    28336,    28336,    28336,
        29296,    29296,    29296,    29296,    30576,    30576,    30576,    30576,    31704,    31704
        
    },
        {
            280,    600,    904,    1224,    1544,    1800,    2152,    2472,    2728,    3112,
                3368,    3624,    4008,    4264,    4584,    4968,    5160,    5544,    5736,    6200,
                6456,    6712,    6968,    7224,    7736,    7992,    8248,    8504,    8760,    9144,
                9528,    9912,    10296,    10296,    10680,    11064,    11448,    11832,    11832,    12216,
                12576,    12960,    12960,    13536,    13536,    14112,    14688,    14688,    15264,    15264,
        15840,    15840,    16416,    16416,    16992,    16992,    17568,    17568,    18336,    18336,
        18336,    19080,    19080,    19848,    19848,    20616,    20616,    20616,    21384,    21384,
        22152,    22152,    22152,    22920,    22920,    23688,    23688,    23688,    24496,    24496,
        24496,    25456,    25456,    25456,    26416,    26416,    26416,    27376,    27376,    27376,
        28336,    28336,    28336,    29296,    29296,    29296,    29296,    30576,    30576,    30576,
        30576,    31704,    31704,    31704,    31704,    32856,    32856,    32856,    34008,    34008
        
    },
        {
        328,    632,    968,    1288,    1608,    1928,    2280,    2600,    2984,    3240,
        3624,    3880,    4264,    4584,    4968,    5160,    5544,    5992,    6200,    6456,
        6712,    7224,    7480,    7736,    7992,    8504,    8760,    9144,    9528,    9912,
        9912,    10296,    10680,    11064,    11448,    11832,    12216,    12216,    12576,    12960,
        13536,    13536,    14112,    14112,    14688,    14688,    15264,    15840,    15840,    16416,
        16416,    16992,    16992,    17568,    17568,    18336,    18336,    19080,    19080,    19848,
        19848,    19848,    20616,    20616,    21384,    21384,    22152,    22152,    22152,    22920,
        22920,    23688,    23688,    24496,    24496,    24496,    25456,    25456,    25456,    26416,
        26416,    26416,    27376,    27376,    27376,    28336,    28336,    28336,    29296,    29296,
        29296,    30576,    30576,    30576,    30576,    31704,    31704,    31704,    31704,    32856,
        32856,    32856,    34008,    34008,    34008,    34008,    35160,    35160,    35160,    35160
        
    },
        {
        336,    696,    1064,    1416,    1800,    2152,    2536,    2856,    3240,    3624,
        4008,    4392,    4776,    5160,    5352,    5736,    6200,    6456,    6712,    7224,
        7480,    7992,    8248,    8760,    9144,    9528,    9912,    10296,    10296,    10680,
        11064,    11448,    11832,    12216,    12576,    12960,    13536,    13536,    14112,    14688,
        14688,    15264,    15264,    15840,    16416,    16416,    16992,    17568,    17568,    18336,
        18336,    19080,    19080,    19848,    19848,    20616,    20616,    20616,    21384,    21384,
        22152,    22152,    22920,    22920,    23688,    23688,    24496,    24496,    24496,    25456,
        25456,    26416,    26416,    26416,    27376,    27376,    27376,    28336,    28336,    29296,
        29296,    29296,    30576,    30576,    30576,    30576,    31704,    31704,    31704,    32856,
        32856,    32856,    34008,    34008,    34008,    35160,    35160,    35160,    35160,    36696,
        36696,    36696,    36696,    37888,    37888,    37888,    39232,    39232,    39232,    39232
    },
        {
        376,    776,    1160,    1544,    1992,    2344,    2792,    3112,    3624,    4008,
        4392,    4776,    5160,    5544,    5992,    6200,    6712,    7224,    7480,    7992,
        8248,    8760,    9144,    9528,    9912,    10296,    10680,    11064,    11448,    11832,
        12216,    12576,    12960,    13536,    14112,    14112,    14688,    15264,    15264,    15840,
        16416,    16416,    16992,    17568,    17568,    18336,    18336,    19080,    19080,    19848,
        19848,    20616,    21384,    21384,    22152,    22152,    22920,    22920,    23688,    23688,
        24496,    24496,    24496,    25456,    25456,    26416,    26416,    27376,    27376,    27376,
        28336,    28336,    29296,    29296,    29296,    30576,    30576,    30576,    31704,    31704,
        31704,    32856,    32856,    32856,    34008,    34008,    34008,    35160,    35160,    35160,
        36696,    36696,    36696,    37888,    37888,    37888,    37888,    39232,    39232,    39232,
        40576,    40576,    40576,    40576,    42368,    42368,    42368,    42368,    43816,    43816
        
    },
        {
        408,    840,    1288,    1736,    2152,    2600,    2984,    3496,    3880,    4264,
        4776,    5160,    5544,    5992,    6456,    6968,    7224,    7736,    8248,    8504,
        9144,    9528,    9912,    10296,    10680,    11064,    11448,    12216,    12576,    12960,
        13536,    13536,    14112,    14688,    15264,    15264,    15840,    16416,    16992,    16992,
        17568,    18336,    18336,    19080,    19080,    19848,    20616,    20616,    21384,    21384,
        22152,    22152,    22920,    22920,    23688,    24496,    24496,    25456,    25456,    25456,
        26416,    26416,    27376,    27376,    28336,    28336,    29296,    29296,    29296,    30576,
        30576,    30576,    31704,    31704,    32856,    32856,    32856,    34008,    34008,    34008,
        35160,    35160,    35160,    36696,    36696,    36696,    37888,    37888,    37888,    39232,
        39232,    39232,    40576,    40576,    40576,    40576,    42368,    42368,    42368,    43816,
        43816,    43816,    43816,    45352,    45352,    45352,    46888,    46888,    46888,    46888
    },
        {
        440,    904,    1384,    1864,    2344,    2792,    3240,    3752,    4136,    4584,
        5160,    5544,    5992,    6456,    6968,    7480,    7992,    8248,    8760,    9144,
        9912,    10296,    10680,    11064,    11448,    12216,    12576,    12960,    13536,    14112,
        14688,    14688,    15264,    15840,    16416,    16992,    16992,    17568,    18336,    18336,
        19080,    19848,    19848,    20616,    20616,    21384,    22152,    22152,    22920,    22920,
        23688,    24496,    24496,    25456,    25456,    26416,    26416,    27376,    27376,    28336,
        28336,    29296,    29296,    29296,    30576,    30576,    31704,    31704,    31704,    32856,
        32856,    34008,    34008,    34008,    35160,    35160,    35160,    36696,    36696,    36696,
        37888,    37888,    39232,    39232,    39232,    40576,    40576,    40576,    42368,    42368,
        42368,    42368,    43816,    43816,    43816,    45352,    45352,    45352,    46888,    46888,
        46888,    46888,    48936,    48936,    48936,    48936,    48936,    51024,    51024,    51024
    },
        {
        488,    1000,    1480,    1992,    2472,    2984,    3496,    4008,    4584,    4968,
        5544,    5992,    6456,    6968,    7480,    7992,    8504,    9144,    9528,    9912,
        10680,    11064,    11448,    12216,    12576,    12960,    13536,    14112,    14688,    15264,
        15840,    15840,    16416,    16992,    17568,    18336,    18336,    19080,    19848,    19848,
        20616,    21384,    21384,    22152,    22920,    22920,    23688,    24496,    24496,    25456,
        25456,    26416,    26416,    27376,    27376,    28336,    28336,    29296,    29296,    30576,
        30576,    31704,    31704,    31704,    32856,    32856,    34008,    34008,    35160,    35160,
        35160,    36696,    36696,    36696,    37888,    37888,    39232,    39232,    39232,    40576,
        40576,    40576,    42368,    42368,    42368,    43816,    43816,    43816,    45352,    45352,
        45352,    46888,    46888,    46888,    46888,    48936,    48936,    48936,    48936,    51024,
        51024,    51024,    51024,    52752,    52752,    52752,    52752,    55056,    55056,    55056
    },
        {
        520,    1064,    1608,    2152,    2664,    3240,    3752,    4264,    4776,    5352,
        5992,    6456,    6968,    7480,    7992,    8504,    9144,    9528,    10296,    10680,
        11448,    11832,    12576,    12960,    13536,    14112,    14688,    15264,    15840,    16416,
        16992,    16992,    17568,    18336,    19080,    19080,    19848,    20616,    21384,    21384,
        22152,    22920,    22920,    23688,    24496,    24496,    25456,    25456,    26416,    27376,
        27376,    28336,    28336,    29296,    29296,    30576,    30576,    31704,    31704,    32856,
        32856,    34008,    34008,    34008,    35160,    35160,    36696,    36696,    36696,    37888,
        37888,    39232,    39232,    40576,    40576,    40576,    42368,    42368,    42368,    43816,
        43816,    43816,    45352,    45352,    45352,    46888,    46888,    46888,    48936,    48936,
        48936,    48936,    51024,    51024,    51024,    51024,    52752,    52752,    52752,    55056,
        55056,    55056,    55056,    57336,    57336,    57336,    57336,    59256,    59256,    59256
    },
        {
        552,    1128,    1736,    2280,    2856,    3496,    4008,    4584,    5160,    5736,
        6200,    6968,    7480,    7992,    8504,    9144,    9912,    10296,    11064,    11448,
        12216,    12576,    12960,    13536,    14112,    14688,    15264,    15840,    16416,    16992,
        17568,    18336,    19080,    19848,    19848,    20616,    21384,    22152,    22152,    22920,
        23688,    24496,    24496,    25456,    25456,    26416,    27376,    27376,    28336,    28336,
        29296,    29296,    30576,    30576,    31704,    31704,    32856,    32856,    34008,    34008,
        35160,    35160,    36696,    36696,    37888,    37888,    37888,    39232,    39232,    40576,
        40576,    40576,    42368,    42368,    43816,    43816,    43816,    45352,    45352,    45352,
        46888,    46888,    46888,    48936,    48936,    48936,    51024,    51024,    51024,    51024,
        52752,    52752,    52752,    55056,    55056,    55056,    55056,    57336,    57336,    57336,
        57336,    59256,    59256,    59256,    59256,    61664,    61664,    61664,    61664,    63776
    },
        {
        584,    1192,    1800,    2408,    2984,    3624,    4264,    4968,    5544,    5992,
        6712,    7224,    7992,    8504,    9144,    9912,    10296,    11064,    11448,    12216,
        12960,    13536,    14112,    14688,    15264,    15840,    16416,    16992,    17568,    18336,
        19080,    19848,    19848,    20616,    21384,    22152,    22920,    22920,    23688,    24496,
        25456,    25456,    26416,    26416,    27376,    28336,    28336,    29296,    29296,    30576,
        31704,    31704,    32856,    32856,    34008,    34008,    35160,    35160,    36696,    36696,
        36696,    37888,    37888,    39232,    39232,    40576,    40576,    42368,    42368,    42368,
        43816,    43816,    45352,    45352,    45352,    46888,    46888,    46888,    48936,    48936,
        48936,    51024,    51024,    51024,    52752,    52752,    52752,    52752,    55056,    55056,
        55056,    57336,    57336,    57336,    57336,    59256,    59256,    59256,    61664,    61664,
        61664,    61664,    63776,    63776,    63776,    63776,    66592,    66592,    66592,    66592
    },
        {
        616,    1256,    1864,    2536,    3112,    3752,    4392,    5160,    5736,    6200,
        6968,    7480,    8248,    8760,    9528,    10296,    10680,    11448,    12216,    12576,
        13536,    14112,    14688,    15264,    15840,    16416,    16992,    17568,    18336,    19080,
        19848,    20616,    20616,    21384,    22152,    22920,    23688,    24496,    24496,    25456,
        26416,    26416,    27376,    28336,    28336,    29296,    29296,    30576,    31704,    31704,
        32856,    32856,    34008,    34008,    35160,    35160,    36696,    36696,    37888,    37888,
        39232,    39232,    40576,    40576,    40576,    42368,    42368,    43816,    43816,    43816,
        45352,    45352,    46888,    46888,    46888,    48936,    48936,    48936,    51024,    51024,
        51024,    52752,    52752,    52752,    55056,    55056,    55056,    55056,    57336,    57336,
        57336,    59256,    59256,    59256,    61664,    61664,    61664,    61664,    63776,    63776,
        63776,    63776,    66592,    66592,    66592,    66592,    68808,    68808,    68808,    71112
    },
        {
        712,    1480,    2216,    2984,    3752,    4392,    5160,    5992,    6712,    7480,
        8248,    8760,    9528,    10296,    11064,    11832,    12576,    13536,    14112,    14688,
        15264,    16416,    16992,    17568,    18336,    19080,    19848,    20616,    21384,    22152,
        22920,    23688,    24496,    25456,    25456,    26416,    27376,    28336,    29296,    29296,
        30576,    30576,    31704,    32856,    32856,    34008,    35160,    35160,    36696,    36696,
        37888,    37888,    39232,    40576,    40576,    40576,    42368,    42368,    43816,    43816,
        45352,    45352,    46888,    46888,    48936,    48936,    48936,    51024,    51024,    52752,
        52752,    52752,    55056,    55056,    55056,    55056,    57336,    57336,    57336,    59256,
        59256,    59256,    61664,    61664,    61664,    63776,    63776,    63776,    66592,    66592,
        66592,    68808,    68808,    68808,    71112,    71112,    71112,    73712,    73712,    75376,
        75376,    75376,    75376,    75376,    75376,    75376,    75376,    75376,    75376,    75376
        
    }
};
 /****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : determineTBSize 
 * Inputs         : mcsIndex - modulation coding scheme index
 *                  nbRB - number of resource block assigned
 * Outputs        : Transport Block Size
 * Returns        : UInt32  
 * Variables      : 
 * Description    : This function returns the transport block size according to
                    mcsIndex and number of RB.
 *****************************************************************************/
UInt32 determineTBSize(SInt8 mcsIndex, UInt8 nbRB)
{
    SInt8 ibsIndex = -1;
    UInt32 tbSize = 0;
#ifdef UT_TESTING
    if(nbRB == 0)
    {
        ltePanic("determineTBSize --> Number Of Resource Blocks Invalid\n");    
    }
    if((mcsIndex >= 0) && (mcsIndex < MAX_MCS_INDEX))
    {
        ibsIndex = TBSArr[mcsIndex];

        if((ibsIndex >= 0) && (ibsIndex < MAX_IBS_INDEX))
        {
            tbSize = TBSizeArr[ibsIndex][nbRB - 1];
        }
    }
#else
    ibsIndex = TBSArr[mcsIndex];
    tbSize = TBSizeArr[ibsIndex][nbRB - 1];
#endif
    return (tbSize>>3);
}