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
 *  File Description : This file contains Frame Structure 2 (TDD) related information.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log:
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "phySimulatorApiHandler.h"
#include "lteLayer2TDDMgr.h"
#include "lteFapiMacPhyInterface.h"


/******************************************************************************
 * Project Includes
 *****************************************************************************/
/******************************************************************************
  Private Definitions
 *****************************************************************************/
UInt8 dlAckNackValue_g[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK];


/******************************************************************************
  Private Types
 *****************************************************************************/
/* index storing the value of 'k' in which UL is scheduled at received UL grant at n */
UInt8 ulSfForDCI0OrHiReceived_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME] =
                               {
                                   {4, 6, 0, 0, 0, 4, 6, 0, 0, 0},
                                   {0, 6, 0, 0, 4, 0, 6, 0, 0, 4},
                                   {0, 0, 0, 4, 0, 0, 0, 0, 4, 0},
                                   {4, 0, 0, 0, 0, 0, 0, 0, 4, 4},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 4, 4},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 4, 0},
                                   {7, 7, 0, 0, 0, 7, 7, 0, 0, 5}
                               };

/* index storing the value of 'k' in which DL ack nack are sent for the data received at n */
UInt8 ulSfForDlAckNack_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME] =
                               {
                                   {4, 6, 0, 0, 0, 4, 6, 0, 0, 0},
                                   {7, 6, 0, 0, 4, 7, 6, 0, 0, 4},
                                   {7, 6, 0, 4, 8, 7, 6, 0, 4, 8},
                                   {4, 11, 0, 0, 0, 7, 6, 6, 5, 5},
                                   {12, 11, 0, 0, 8, 7, 7, 6, 5, 4},
                                   {12, 11, 0, 9, 8, 7, 6, 5, 4, 13},
                                   {7, 7, 0, 0, 0, 7, 7, 0, 0, 5}
                               };

/* Index storing the value of kPhich for the UL data send */
/* used for timer expiry*/
UInt8 dlSfForUlAckNack_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME] =
                               {
                                   {0,0,4,7,6,0,0,4,7,6},
                                   {0,0,4,6,0,0,0,4,6,0},
                                   {0,0,6,0,0,0,0,6,0,0},
                                   {0,0,6,6,6,0,0,0,0,0},
                                   {0,0,6,6,0,0,0,0,0,0},
                                   {0,0,6,0,0,0,0,0,0,0},
                                   {0,0,4,6,6,0,0,4,7,0}
                               };


const char UlDlConfig_gc[7][MAX_SUBFRAME] = {
    {'D',   'S',    'U',    'U',    'U',    'D',    'S',    'U',    'U',    'U'}, // 0
    {'D',   'S',    'U',    'U',    'D',    'D',    'S',    'U',    'U',    'D'}, // 1
    {'D',   'S',    'U',    'D',    'D',    'D',    'S',    'U',    'D',    'D'}, // 2
    {'D',   'S',    'U',    'U',    'U',    'D',    'D',    'D',    'D',    'D'}, // 3
    {'D',   'S',    'U',    'U',    'D',    'D',    'D',    'D',    'D',    'D'}, // 4
    {'D',   'S',    'U',    'D',    'D',    'D',    'D',    'D',    'D',    'D'}, // 5
    {'D',   'S',    'U',    'U',    'U',    'D',    'S',    'U',    'U',    'D'}  // 6
};
/*SPR 13884 changes start */
/* + CA_TDD_HARQ_CHANGES */
#ifdef TDD_CONFIG
const UInt8 getM_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME] = {
        {0,0,1,0,1,0,0,1,0,1}, /* DL UL Config 0 */
        {0,0,2,1,0,0,0,2,1,0}, /* DL UL Config 1 */
        {0,0,4,0,0,0,0,4,0,0}, /* DL UL Config 2 */
        {0,0,3,2,2,0,0,0,0,0}, /* DL UL Config 3 */
        {0,0,4,4,0,0,0,0,0,0}, /* DL UL Config 4 */
        {0,0,9,0,0,0,0,0,0,0}, /* DL UL Config 5 */
        {0,0,1,1,1,0,0,1,1,0}  /* DL UL Config 6 */
};
#endif
/* - CA_TDD_HARQ_CHANGES */

/*SPR 13884 changes start */
UInt32 tddAckNackval_g[8] = {
    FAPI_NACK,
    FAPI_ACK,
    /*CLPC_CHG*/
    FAPI_ACK_OR_NACK,
    FAPI_DTX,
    FAPI_ACK_OR_DTX,
    FAPI_NACK_OR_DTX,
    FAPI_ACK_OR_NACK_OR_DTX,
    /*CLPC_CHG*/
    INVALID_ACK_NACK
};
/*SPR 13884 changes end */
	

/* RachResLocationQuad: RACH resource location quadruple */
/* +    SPR 11297 changes start */
const RachResLocationQuad RachResLocationQuad_gc[MAX_UL_DL_CONFIG][MAX_PRACH_CONFIGS][MAX_TDD_RACH_DENSITY] = {
        {   // UL/DL Config 0
	    /*TDD Config 0 And 6 Changes Start*/
/*  0   */  {{0,1,0,2}},
/*  1   */  {{0,2,0,2}},
/*  2   */  {{0,1,1,2}},
/*  3   */  {{0,0,0,2}},
/*  4   */  {{0,0,1,2}},
/*  5   */  {{0,0,0,1}},
/*  6   */  {{0,0,0,2}, {0,0,1,2}},
/*  7   */  {{0,0,0,1}, {0,0,1,1}},
/*  8   */  {{0,0,0,0}, {0,0,1,0}},
/*  9   */  {{0,0,0,1}, {0,0,0,2}, {0,0,1,2}},
/*  10  */  {{0,0,0,0}, {0,0,1,0}, {0,0,1,1}},
/*  11  */  {{NA}},
/*  12  */  {{0,0,0,1}, {0,0,0,2}, {0,0,1,1}, {0,0,1,2}},
/*  13  */  {{0,0,0,0}, {0,0,0,2}, {0,0,1,0}, {0,0,1,2}},
/*  14  */  {{0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1}},
/*  15  */  {{0,0,0,0}, {0,0,0,1}, {0,0,0,2}, {0,0,1,1}, {0,0,1,2}},
/*  16  */  {{0,0,0,1}, {0,0,0,2}, {0,0,1,0}, {0,0,1,1}, {0,0,1,2}},
/*  17  */  {{0,0,0,0}, {0,0,0,1}, {0,0,0,2}, {0,0,1,0}, {0,0,1,2}},
/*  18  */  {{0,0,0,0}, {0,0,0,1}, {0,0,0,2}, {0,0,1,0}, {0,0,1,1}, {0,0,1,2}},
/*  19  */  {{NA}},
/*  20  */  {{0,1,0,1}},
/*  21  */  {{0,2,0,1}},
/*  22  */  {{0,1,1,1}},
/*  23  */  {{0,0,0,1}},
/*  24  */  {{0,0,1,1}},
/*  25  */  {{0,0,0,1}, {0,0,1,1}},
/*  26  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}},
/*  27  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}, {1,0,1,1}},
/*  28  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}, {2,0,0,1}},
/*  29  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}, {1,0,1,1}, {2,0,0,1}, {2,0,1,1}},
/*  30  */  {{0,1,0,1}},
/*  31  */  {{0,2,0,1}},
/*  32  */  {{0,1,1,1}},
/*  33  */  {{0,0,0,1}},
/*  34  */  {{0,0,1,1}},
/*  35  */  {{0,0,0,1}, {0,0,1,1}},
/*  36  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}},
/*  37  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}, {1,0,1,1}},
/*  38  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}, {2,0,0,1}},
/*  39  */  {{0,0,0,1}, {0,0,1,1}, {1,0,0,1}, {1,0,1,1}, {2,0,0,1}, {2,0,1,1}},
/*  40  */  {{0,1,0,0}},
/*  41  */  {{0,2,0,0}},
/*  42  */  {{0,1,1,0}},
/*  43  */  {{0,0,0,0}},
/*  44  */  {{0,0,1,0}},
/*  45  */  {{0,0,0,0}, {0,0,1,0}},
/*  46  */  {{0,0,0,0}, {0,0,1,0}, {1,0,0,0}},
/*  47  */  {{0,0,0,0}, {0,0,1,0}, {1,0,0,0}, {1,0,1,0}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{0,1,1,SSF}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{0,0,1,SSF}},
/*  53  */  {{0,0,0,SSF}, {0,0,1,SSF}},
/*  54  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}},
/*  56  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}, {2,0,1,SSF}}
	    /*TDD Config 0 And 6 Changes End*/
        },
        {   // UL/DL Config 1
/*  0   */  {{0,1,0,1}},
/*  1   */  {{0,2,0,1}},
/*  2   */  {{0,1,1,1}},
/*  3   */  {{0,0,0,1}},
/*  4   */  {{0,0,1,1}},
/*  5   */  {{0,0,0,0}},
/*  6   */  {{0,0,0,1}, {0,0,1,1}},
/*  7   */  {{0,0,0,0}, {0,0,1,0}},
/*  8   */  {{NA}},
/*  9   */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,1}},
/*  10  */  {{0,0,0,1}, {0,0,1,0},  {0,0,1,1}},
/*  11  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0}},
/*  12  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0},  {0,0,1,1}},
/*  13  */  {{NA}},
/*  14  */  {{NA}},
/*  15  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0},  {0,0,1,1},  {1,0,0,1}},
/*  16  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0},  {0,0,1,1},  {1,0,1,1}},
/*  17  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0},  {0,0,1,1},  {1,0,0,0}},
/*  18  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0},  {0,0,1,1},  {1,0,0,1},  {1,0,1,1}},
/*  19  */  {{0,0,0,0}, {0,0,0,1},  {0,0,1,0},  {0,0,1,1},  {1,0,0,0},  {1,0,1,0}},
/*  20  */  {{0,1,0,0}},
/*  21  */  {{0,2,0,0}},
/*  22  */  {{0,1,1,0}},
/*  23  */  {{0,0,0,0}},
/*  24  */  {{0,0,1,0}},
/*  25  */  {{0,0,0,0}, {0,0,1,0}},
/*  26  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0}},
/*  27  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0}},
/*  28  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,0,0}},
/*  29  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,0,0},  {2,0,1,0}},
/*  30  */  {{0,1,0,0}},
/*  31  */  {{0,2,0,0}},
/*  32  */  {{0,1,1,0}},
/*  33  */  {{0,0,0,0}},
/*  34  */  {{0,0,1,0}},
/*  35  */  {{0,0,0,0}, {0,0,1,0}},
/*  36  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0}},
/*  37  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0}},
/*  38  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,0,0}},
/*  39  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,0,0},  {2,0,1,0}},
/*  40  */  {{NA}},
/*  41  */  {{NA}},
/*  42  */  {{NA}},
/*  43  */  {{NA}},
/*  44  */  {{NA}},
/*  45  */  {{NA}},
/*  46  */  {{NA}},
/*  47  */  {{NA}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{0,1,1,SSF}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{0,0,1,SSF}},
/*  53  */  {{0,0,0,SSF}, {0,0,1,SSF}},
/*  54  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}},
/*  56  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}, {2,0,1,SSF}}
        },
        {   // UL/DL Config 2
/*  0   */  {{0,1,0,0}},
/*  1   */  {{0,2,0,0}},
/*  2   */  {{0,1,1,0}},
/*  3   */  {{0,0,0,0}},
/*  4   */  {{0,0,1,0}},
/*  5   */  {{NA}},
/*  6   */  {{0,0,0,0}, {0,0,1,0}},
/*  7   */  {{NA}},
/*  8   */  {{NA}},
/*  9   */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0}},
/*  10  */  {{0,0,0,0}, {0,0,1,0},  {1,0,1,0}},
/*  11  */  {{NA}},
/*  12  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0}},
/*  13  */  {{NA}},
/*  14  */  {{NA}},
/*  15  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,0,0}},
/*  16  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,1,0}},
/*  17  */  {{NA}},
/*  18  */  {{0,0,0,0}, {0,0,1,0},  {1,0,0,0},  {1,0,1,0},  {2,0,0,0},  {2,0,1,0}},
/*  19  */  {{NA}},
/*  20  */  {{NA}},
/*  21  */  {{NA}},
/*  22  */  {{NA}},
/*  23  */  {{NA}},
/*  24  */  {{NA}},
/*  25  */  {{NA}},
/*  26  */  {{NA}},
/*  27  */  {{NA}},
/*  28  */  {{NA}},
/*  29  */  {{NA}},
/*  30  */  {{NA}},
/*  31  */  {{NA}},
/*  32  */  {{NA}},
/*  33  */  {{NA}},
/*  34  */  {{NA}},
/*  35  */  {{NA}},
/*  36  */  {{NA}},
/*  37  */  {{NA}},
/*  38  */  {{NA}},
/*  39  */  {{NA}},
/*  40  */  {{NA}},
/*  41  */  {{NA}},
/*  42  */  {{NA}},
/*  43  */  {{NA}},
/*  44  */  {{NA}},
/*  45  */  {{NA}},
/*  46  */  {{NA}},
/*  47  */  {{NA}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{0,1,1,SSF}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{0,0,1,SSF}},
/*  53  */  {{0,0,0,SSF}, {0,0,1,SSF}},
/*  54  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}},
/*  56  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}, {2,0,1,SSF}}
        },
        {   // UL/DL Config 3
/*  0   */  {{0,1,0,2}},
/*  1   */  {{0,2,0,2}},
/*  2   */  {{0,1,0,1}},
/*  3   */  {{0,0,0,2}},
/*  4   */  {{0,0,0,1}},
/*  5   */  {{0,0,0,0}},
/*  6   */  {{0,0,0,1}, {0,0,0,2}},
/*  7   */  {{0,0,0,0}, {0,0,0,2}},
/*  8   */  {{0,0,0,0}, {0,0,0,1}},
/*  9   */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2}},
/*  10  */  {{NA}},
/*  11  */  {{NA}},
/*  12  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,2}},
/*  13  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,1}},
/*  14  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,0}},
/*  15  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,1},  {1,0,0,2}},
/*  16  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,0},  {1,0,0,2}},
/*  17  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,0},  {1,0,0,1}},
/*  18  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {1,0,0,0},  {1,0,0,1},  {1,0,0,2}},
/*  19  */  {{NA}},
/*  20  */  {{0,1,0,1}},
/*  21  */  {{0,2,0,1}},
/*  22  */  {{NA}},
/*  23  */  {{0,0,0,1}},
/*  24  */  {{NA}},
/*  25  */  {{0,0,0,1},{1,0,0,1}},
/*  26  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1}},
/*  27  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1},{3,0,0,1}},
/*  28  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1},{3,0,0,1},{4,0,0,1}},
/*  29  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1},{4,0,0,1},{5,0,0,1}},
/*  30  */  {{0,1,0,1}},
/*  31  */  {{0,2,0,1}},
/*  32  */  {{NA}},
/*  33  */  {{0,0,0,1}},
/*  34  */  {{NA}},
/*  35  */  {{0,0,0,1},{1,0,0,1}},
/*  36  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1}},
/*  37  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1},{3,0,0,1}},
/*  38  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1},{3,0,0,1},{4,0,0,1}},
/*  39  */  {{0,0,0,1},{1,0,0,1},{2,0,0,1},{3,0,0,1},{4,0,0,1},{5,0,0,1}},
/*  40  */  {{0,1,0,0}},
/*  41  */  {{0,2,0,0}},
/*  42  */  {{NA}},
/*  43  */  {{0,0,0,0}},
/*  44  */  {{NA}},
/*  45  */  {{0,0,0,0},{1,0,0,0}},
/*  46  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0}},
/*  47  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{NA}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{NA}},
/*  53  */  {{0,0,0,SSF}, {1,0,0,SSF}},
/*  54  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}},
/*  56  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}, {4,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}, {4,0,0,SSF}, {5,0,0,SSF}}
        },
        {   // UL/DL Config 4
/*  0   */  {{0,1,0,1}},
/*  1   */  {{0,2,0,1}},
/*  2   */  {{0,1,0,0}},
/*  3   */  {{0,0,0,1}},
/*  4   */  {{0,0,0,0}},
/*  5   */  {{NA}},
/*  6   */  {{0,0,0,0}, {0,0,0,1}},
/*  7   */  {{NA}},
/*  8   */  {{NA}},
/*  9   */  {{0,0,0,0}, {0,0,0,1},  {1,0,0,1}},
/*  10  */  {{0,0,0,0}, {0,0,0,1},  {1,0,0,0}},
/*  11  */  {{NA}},
/*  12  */  {{0,0,0,0}, {0,0,0,1},  {1,0,0,0},  {1,0,0,1}},
/*  13  */  {{NA}},
/*  14  */  {{NA}},
/*  15  */  {{0,0,0,0}, {0,0,0,1},  {1,0,0,0},  {1,0,0,1},  {2,0,0,1}},
/*  16  */  {{0,0,0,0}, {0,0,0,1},  {1,0,0,0},  {1,0,0,1},  {2,0,0,0}},
/*  17  */  {{NA}},
/*  18  */  {{0,0,0,0}, {0,0,0,1},  {1,0,0,0},  {1,0,0,1},  {2,0,0,0},  {2,0,0,1}},
/*  19  */  {{NA}},
/*  20  */  {{0,1,0,0}},
/*  21  */  {{0,2,0,0}},
/*  22  */  {{NA}},
/*  23  */  {{0,0,0,0}},
/*  24  */  {{NA}},
/*  25  */  {{0,0,0,0},{1,0,0,0}},
/*  26  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0}},
/*  27  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0}},
/*  28  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0},{4,0,0,0}},
/*  29  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0},{4,0,0,0},{5,0,0,0}},
/*  30  */  {{0,1,0,0}},
/*  31  */  {{0,2,0,0}},
/*  32  */  {{NA}},
/*  33  */  {{0,0,0,0}},
/*  34  */  {{NA}},
/*  35  */  {{0,0,0,0},{1,0,0,0}},
/*  36  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0}},
/*  37  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0}},
/*  38  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0},{4,0,0,0}},
/*  39  */  {{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0},{4,0,0,0},{5,0,0,0}},
/*  40  */  {{NA}},
/*  41  */  {{NA}},
/*  42  */  {{NA}},
/*  43  */  {{NA}},
/*  44  */  {{NA}},
/*  45  */  {{NA}},
/*  46  */  {{NA}},
/*  47  */  {{NA}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{NA}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{NA}},
/*  53  */  {{0,0,0,SSF}, {1,0,0,SSF}},
/*  54  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}},
/*  56  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}, {4,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}, {4,0,0,SSF}, {5,0,0,SSF}}
        },
        {   // UL/DL Config 5
/*  0   */  {{0,1,0,0}},
/*  1   */  {{0,2,0,0}},
/*  2   */  {{NA}},
/*  3   */  {{0,0,0,0}},
/*  4   */  {{NA}},
/*  5   */  {{NA}},
/*  6   */  {{0,0,0,0}, {1,0,0,0}},
/*  7   */  {{NA}},
/*  8   */  {{NA}},
/*  9   */  {{0,0,0,0}, {1,0,0,0},  {2,0,0,0}},
/*  10  */  {{NA}},
/*  11  */  {{NA}},
/*  12  */  {{0,0,0,0}, {1,0,0,0},  {2,0,0,0},  {3,0,0,0}},
/*  13  */  {{NA}},
/*  14  */  {{NA}},
/*  15  */  {{0,0,0,0}, {1,0,0,0},  {2,0,0,0},  {3,0,0,0},  {4,0,0,0}},
/*  16  */  {{NA}},
/*  17  */  {{NA}},
/*  18  */  {{0,0,0,0}, {1,0,0,0},  {2,0,0,0},  {3,0,0,0},  {4,0,0,0},  {5,0,0,0}},
/*  19  */  {{NA}},
/*  20  */  {{NA}},
/*  21  */  {{NA}},
/*  22  */  {{NA}},
/*  23  */  {{NA}},
/*  24  */  {{NA}},
/*  25  */  {{NA}},
/*  26  */  {{NA}},
/*  27  */  {{NA}},
/*  28  */  {{NA}},
/*  29  */  {{NA}},
/*  30  */  {{NA}},
/*  31  */  {{NA}},
/*  32  */  {{NA}},
/*  33  */  {{NA}},
/*  34  */  {{NA}},
/*  35  */  {{NA}},
/*  36  */  {{NA}},
/*  37  */  {{NA}},
/*  38  */  {{NA}},
/*  39  */  {{NA}},
/*  40  */  {{NA}},
/*  41  */  {{NA}},
/*  42  */  {{NA}},
/*  43  */  {{NA}},
/*  44  */  {{NA}},
/*  45  */  {{NA}},
/*  46  */  {{NA}},
/*  47  */  {{NA}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{NA}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{NA}},
/*  53  */  {{0,0,0,SSF}, {1,0,0,SSF}},
/*  54  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}},
/*  56  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}, {4,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {1,0,0,SSF}, {2,0,0,SSF}, {3,0,0,SSF}, {4,0,0,SSF}, {5,0,0,SSF}}
        },
        {   // UL/DL Config 6
/*TDD Config 0 And 6 Changes Start*/
/*  0   */  {{0,1,0,2}},
/*  1   */  {{0,2,0,2}},
/*  2   */  {{0,1,1,1}},
/*  3   */  {{0,0,0,2}},
/*  4   */  {{0,0,1,1}},
/*  5   */  {{0,0,0,1}},
/*  6   */  {{0,0,0,2}, {0,0,1,1}},
/*  7   */  {{0,0,0,1}, {0,0,1,0}},
/*  8   */  {{0,0,0,0}, {0,0,1,1}},
/*  9   */  {{0,0,0,1}, {0,0,0,2},  {0,0,1,1}},
/*  10  */  {{0,0,0,0}, {0,0,0,2},  {0,0,1,0}},
/*  11  */  {{0,0,0,1}, {0,0,1,0},  {0,0,1,1}},
/*  12  */  {{0,0,0,1}, {0,0,0,2},  {0,0,1,0},  {0,0,1,1}},
/*  13  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {0,0,1,1}},
/*  14  */  {{0,0,0,0}, {0,0,0,2},  {0,0,1,0},  {0,0,1,1}},
/*  15  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {0,0,1,0},
            {0,0,1,1}},
/*  16  */  {{NA}},
/*  17  */  {{NA}},
/*  18  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {0,0,1,0},
            {0,0,1,1},  {1,0,0,2}},
/*  19  */  {{0,0,0,0}, {0,0,0,1},  {0,0,0,2},  {0,0,1,0},
            {0,0,1,1},  {1,0,1,1}},
/*  20  */  {{0,1,0,1}},
/*  21  */  {{0,2,0,1}},
/*  22  */  {{0,1,1,0}},
/*  23  */  {{0,0,0,1}},
/*  24  */  {{0,0,1,0}},
/*  25  */  {{0,0,0,1}, {0,0,1,0}},
/*  26  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1}},
/*  27  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1},  {1,0,1,0}},
/*  28  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1},  {1,0,1,0},
            {2,0,0,1}},
/*  29  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1},  {1,0,1,0},
            {2,0,0,1},  {2,0,1,0}},

/*  30  */  {{0,1,0,1}},
/*  31  */  {{0,2,0,1}},
/*  32  */  {{0,1,1,0}},
/*  33  */  {{0,0,0,1}},
/*  34  */  {{0,0,1,0}},
/*  35  */  {{0,0,0,1}, {0,0,1,0}},
/*  36  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1}},
/*  37  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1},  {1,0,1,0}},
/*  38  */  {{0,0,0,1}, {0,0,1,0},   {1,0,0,1},  {1,0,1,0},
            {2,0,0,1}},

/*  39  */   {{0,0,0,1}, {0,0,1,0},   {1,0,0,1},  {1,0,1,0},
             {2,0,0,1},  {2,0,1,0}},
/*  40  */  {{0,1,0,0}},
/*  41  */  {{0,2,0,0}},
/*  42  */  {{NA}},
/*  43  */  {{0,0,0,0}},
/*  44  */  {{NA}},
/*  45  */  {{0,0,0,0},  {1,0,0,0}},
/*  46  */  {{0,0,0,0},  {1,0,0,0},   {2,0,0,0}},
/*  47  */  {{0,0,0,0},  {1,0,0,0},   {2,0,0,0},  {3,0,0,0}},
/*  48  */  {{0,1,0,SSF}},
/*  49  */  {{0,2,0,SSF}},
/*  50  */  {{0,1,1,SSF}},
/*  51  */  {{0,0,0,SSF}},
/*  52  */  {{0,0,1,SSF}},
/*  53  */  {{0,0,0,SSF}, {0,0,1,SSF}},
/*  54  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}},
/*  55  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}},
/*  56  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}},
/*  57  */  {{0,0,0,SSF}, {0,0,1,SSF}, {1,0,0,SSF}, {1,0,1,SSF}, {2,0,0,SSF}, {2,0,1,SSF}}
        }
};
/* -    SPR 11297 changes end */
/*TDD Config 0 And 6 Changes End*/

/* SPR 2984 Changes */
/* This structure stores the values of container index and DCI0 sub frame
 * on which UL CONFIG message needs to be prepared for Msg3. Here for each
 * D or S a DCI0 SF is stored on which ul grant is to send for U on which
 * Msg3 is to be recievd for that D or S (Which is used to send RAR).
 * Here id RAR is sent on t then U should be atleast t+6 */
#ifdef TDD_CONFIG
static SfForRachULConfig sfForRachULConfig_g[MAX_UL_DL_CONFIG]
                                                [MAX_SUBFRAME] =
{
    /* TDD Config 0 And 6 Changes Start*/
    /*UL/DL Configuration 0 */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {   {1, 0}, {0, 0}, {NA, NA}, {NA, NA}, {NA, NA}, {1, 0},
        /*  6(S)       7(U)      8(U)      9(U)   */
        {0, 0}, {NA, NA}, {NA, NA}, {NA, NA} },
    /*TDD Config 0 And 6 Changes End*/
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
    /*TDD Config 0 And 6 Changes Start*/
    /*UL/DL Configuration 6 */
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {   {0, 0}, {0, 0}, {NA, NA}, {NA, NA}, {NA, NA}, {0, 0},
        /*  6(S)       7(U)      8(U)      9(D)   */
        {0, 0}, {NA, NA}, {NA, NA}, {1, 1} }
    /*TDD Config 0 And 6 Changes End*/
};
/* + SPS_TDD_Changes */
#define NA_SF 128
static SInt8 subFrameOffsetForSps[MAX_UL_DL_CONFIG]
                                 [MAX_SUBFRAME] =
{
    /* UL/DL Configuration 0*/
    /*  0(D)       1(S)      2(U)    3(U)   4(U)   5(D)  */
    {   NA_SF,        NA_SF,       0,      0,    0,    NA_SF,
        /*  6(S)      7(U)      8(U)    9(U)   */
        NA_SF,       0,        0,      0
    },
    /* UL/DL Configuration 1*/
    /*  0(D)       1(S)      2(U)      3(U)      4(D)      5(D)  */
    {   NA_SF,       NA_SF,       1,       -1,         NA_SF,       NA_SF,
        /*  6(S)       7(U)       8(U)      9(D)   */
        NA_SF,        1,       -1,       NA_SF
    },
    /* UL/DL Configuration 2*/
    /*  0(D)       1(S)      2(U)      3(D)      4(D)      5(D)  */
    {    NA_SF,       NA_SF,       5,        NA_SF,        NA_SF,        NA_SF,
        /*  6(S)       7(U)      8(D)      9(D)   */
        NA_SF,         -5,       NA_SF,      NA_SF
    },
    /* UL/DL Configuration 3*/
    /*  0(D)       1(S)      2(U)      3(U)      4(U)      5(D)  */
    {    NA_SF,       NA_SF,        1 ,      1,        -2,       NA_SF,
        /*  6(D)      7(D)      8(D)    9(D)   */
        NA_SF,       NA_SF,       NA_SF,     NA_SF
    },
    /* UL/DL Configuration 4*/
    /*  0(D)       1(S)      2(U)      3(U)   4(D)      5(D)  */
    {    NA_SF,       NA_SF,       1,         -1,    NA_SF,     NA_SF,
        /*  6(D)      7(D)      8(D)     9(D)   */
        NA_SF,       NA_SF,       NA_SF,     NA_SF
    },
    /* UL/DL Configuration 5*/
    /*  0(D)       1(S)      2(U)    3(D)   4(D)   5(D)  */
    {   NA_SF,        NA_SF,       0,      NA_SF,    NA_SF,    NA_SF,
        /*  6(D)      7(D)      8(D)    9(D)   */
        NA_SF,       NA_SF,       NA_SF,     NA_SF
    },
    /* UL/DL Configuration 6*/
    /*  0(D)       1(S)      2(U)    3(U)   4(U)   5(D)  */
    {   NA_SF,        NA_SF,        0,      0,      0,    NA_SF,
        /*  6(S)      7(U)      8(U)    9(D)   */
        NA_SF,        0,        0,     NA_SF
    }
};

/*
This structure is used in future
its only store the previous 
DL Subframe in which DCI-0 is valid
and store the subframe number,
advanced reserved rb map number.
*/
typedef struct PrevRecordForULT
{
   UInt8 prevSubFrameforDCI_O_From_currentSubFrame;
   UInt8 exactPrevSubFrameNum;
   UInt8 advanceReservedRBMapNumFromPrevSubFrame;
}PrevRecordForUL;


/*
This union is used to get the
rbMapNum for reserved in advance in Non Adaptive
case or get the value for already Reserve the 
RB Map.
*/
typedef union RBMapNumT
{
    UInt8 rbMapNumForNonAdaptive;
    UInt8 alreadyReserveRBNum;
}RBMapNum;

/*
This union is used to get the valid
UL & DL subframe for UL Grant and 
HI pdu.
*/
typedef union UlGrantHIPduInfoT
{
    UInt8 ulGrantSubFrameNum;
    UInt8 ulPHICHSubFrameNum;
}UlGrantPhichInfo;

typedef enum ULDLSPFrameT
{
    UL = 0, /* Uplink Subframe */
    DL,     /* Downlink Subframe */
    SP,     /* Special Subframe */
    /*NA = 255*/ /* Not Applicable*/
}ULDLSPFrame;

/*
  This is the main Map or (hash table)
  to get the value of valid DL or UL subframe
  or getting the value of valid UL or DL subframe
  corresponding to the UL/DL Configuration.
  This sturcture is also store the resource block
  map number and advance reserve rb map number.
*/
typedef struct UlGrantHIPduSubFrameInfoT 
{
    ULDLSPFrame ulDlSpFrame;
    UlGrantPhichInfo ulGrantPhichInfo;
    RBMapNum         rbMapNum;
    PrevRecordForUL  prevRecordForUL;
}UlGrantHIPduSubFrameInfo;

UlGrantHIPduSubFrameInfo 
       ulGrantPhichTableInfo_sim_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME] =
{     
    /* UL/DL Configuration 0
                     0(DL)                        1(SP)                 2(UL)    */            
    { {DL, {4}, {0}, {0, 0, 0} }, {SP, {6}, {0}, {0, 0, 0} }, {UL, {4}, {0}, {0, 0, 0} },
        /*           3(UL)                        4(UL)                 5(DL)    */ 
        {UL, {7}, {0}, {0, 0, 0} }, {UL, {6}, {0}, {0, 0, 0} }, {DL, {4}, {0}, {0, 0, 0} },
        /*           6(SP)                        7(UL)                  8(UL)    */  
        {SP, {6}, {0}, {0, 0, 0} }, {UL, {4}, {0}, {0, 0, 0} }, {UL, {7}, {0}, {0, 0, 0} },
        /*          9(UL)      */
        {UL, {7}, {0}, {0, 0, 0} } 
    },

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
/* - SPS_TDD_Changes */

/* CA TDD Changes Start */
extern UInt8 msgSfDlConfigErr_g[MAX_NUM_CELLS];
/* CA TDD Changes End */

#endif
/* SPR 2984 Changes */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
const RachResLocationQuad *getRachLocationSet(UInt8 cellIndex);
/******************************************************************************
 * Private Constants
 *****************************************************************************/
/* CA TDD Changes Start */
UInt8 tddUlDlConfig_g[MAX_NUM_CELLS] = {0};
/* CA TDD Changes End */
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* + CA_TDD_HARQ_CHANGES */
//AckNackMode ackNackMode_g = BUNDLING;
/* - CA_TDD_HARQ_CHANGES */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
extern UInt32 signalling_ack_nack_g[MAX_NUM_CELLS];
extern UInt32 sig_cnt[MAX_NUM_CELLS];
extern UInt8 tb1_ack_nack_value_array [MAX_NUM_CELLS]
[ACK_NACK_ARRAY_SIZE][ACK_NACK_ARRAY_SIZE];
extern UInt8 tb2_ack_nack_value_array [MAX_NUM_CELLS]
[ACK_NACK_ARRAY_SIZE][ACK_NACK_ARRAY_SIZE];
/* + CA_TDD_HARQ_CHANGES */
#ifdef TDD_CONFIG
static void buildCATDDUEAckNACK(
        ueContextForUplink *ueContextInfo_p,
#ifdef CLPC_TDD_UNSURE
        DLUEContext *dlUEContext_p,
#endif
        FAPI_harqIndQueueNode_st * harqNode_p,
        UInt8 sendSF, 
        DaiTrackingRegister *daiTrackingInfo_p,
        UInt8 isDaiConsistent,
        UInt8 isSCell,
        UInt8 currentSF,
        UInt8 qIndex,
        UInt8 tcCellIndex,
        UInt8 rxCellIndex
        );
#endif
/* - CA_TDD_HARQ_CHANGES */
/****************************************************************************
 * Function Name  :  UpdateSfnSf
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : 
 *
 ****************************************************************************/

inline void UpdateSfnSf(UInt16 *sfn_p, UInt8 *sf_p, UInt16 subframeCount)
{
    *sf_p += subframeCount;
    if (*sf_p >= MAX_SUBFRAME)
    {
        *sfn_p +=  (*sf_p / MAX_SUBFRAME);
        *sf_p %= MAX_SUBFRAME;
        if (*sfn_p >= MAX_SFN )
        {
            (*sfn_p) -= MAX_SFN;
        }
    }
}

/****************************************************************************
 * Function Name  :  UpdateTick
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : 
 *
 ****************************************************************************/

inline void UpdateTick(UInt32 *tick_p, UInt32 subframeCount)
{
    *tick_p += subframeCount;
#if 0
    if (*tick_p >= MAX_SUB_FRAME_PER_RADIO_FRAME)
    {
        *tick_p -= MAX_SUB_FRAME_PER_RADIO_FRAME;
    }
#endif
}

#ifdef TDD_CONFIG
/****************************************************************************
 * Function Name  : calQIdxSFandSFN 
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : This calculation is for HI PDU and DCI Format 0 only 
 *
 ****************************************************************************/

inline void calQIdxSFandSFN( UInt8 enbSF,
                            UInt16 enbSFN,
                            UInt8 delay,
                            Dci0HiPdu *dci0HiPdu
                            ,UInt8 cellIndex
                           )
{
    UInt8  hiDci0QIndex  = 0;
    UInt16 hiDci0SendSFN = 0;
    UInt8  hiDci0SendSF  = 0;
    UInt8  dciQIndex     = 0;
    UInt16 dciSendSFN    = 0;
    UInt8  dciSendSF     = 0;
    /* CA TDD Changes Start */
    tddUlDlConfig_g[cellIndex] = cellConfigUeSim_g[cellIndex].subFrameAssignment;
    /* CA TDD Changes End */

    /* RPT:This calculation is for HI PDU and DCI Format 0 only, it's better to calculate
       corresponding send SF, SFN and queueIndex once here instead of in while loop */
    //hiDci0QIndex = enbSF + ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g][enbSF] + PEER_MAC_DELAY - delay;
    /* CA TDD Changes Start */
    hiDci0QIndex = enbSF + ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g[cellIndex]][enbSF] + PEER_MAC_DELAY;

    hiDci0QIndex = ((enbSFN * MAX_SUBFRAME) + hiDci0QIndex) % MAX_UL_ARRAY;

    hiDci0SendSF = enbSF;
    hiDci0SendSFN = enbSFN;
    UpdateSfnSf(&hiDci0SendSFN, &hiDci0SendSF,
            ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g[cellIndex]][enbSF]);
    /* CA TDD Changes End */

#if 1
    /* RPT:Take care of PEER MAC DELAY*/
    /* For other DCI Formats other than DCI 0*/
    dciSendSF = enbSF;
    dciSendSFN = enbSFN;
    /* CA TDD Changes Start */
    UpdateSfnSf(&dciSendSFN, &dciSendSF,
            ulSfForDlAckNack_g[tddUlDlConfig_g[cellIndex]][enbSF]);
    /* CA TDD Changes End */
    //dciQIndex = (dciSendSFN * MAX_SUBFRAME + dciSendSF + PEER_MAC_DELAY - delay) % MAX_UL_ARRAY;
    dciQIndex = (dciSendSFN * MAX_SUBFRAME + dciSendSF + PEER_MAC_DELAY) % MAX_UL_ARRAY;
    dci0HiPdu->dciQIdx = dciQIndex;
    dci0HiPdu->dci0SFN = dciSendSFN;
    dci0HiPdu->dci0SF  = dciSendSF;
#endif
    dci0HiPdu->hiQIdx  = hiDci0QIndex;
    dci0HiPdu->hiSFN   = hiDci0SendSFN;
    dci0HiPdu->hiSF    = hiDci0SendSF;



}
/* SPR 2984 Changes */
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
/* CA TDD Changes Start */
inline UInt8 getSfForMsg3 (UInt8 subframeNum,UInt8 cellIndex)
{
#if 0
    MAC_ASSERT ((DL == ulGrantPhichTableInfo_p[subframeNum].ulDlSpFrame)
            || (SP == ulGrantPhichTableInfo_p[subframeNum].ulDlSpFrame))
#endif
    UInt8 sfForMsg3 = sfForRachULConfig_g[tddUlDlConfig_g[cellIndex]][subframeNum].ulGrantSfForMsg3;
    /* CA TDD Changes End */
    
    sfForMsg3 += subframeNum;
    if (sfForMsg3 >= MAX_SUBFRAME)
    {
        sfForMsg3 %= MAX_SUBFRAME;
    }
    return (sfForMsg3);
}

/****************************************************************************
 * Function Name  : getMsg3SFandQIndex 
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : 
 ****************************************************************************/
inline UInt8 getMsg3SFandQIndex( UInt16 *sendSFN,
                                 UInt8  *sendSF,
                                 /* CA TDD Changes Start */
                                 UInt8 cellIndex
                                 /* CA TDD Changes End */
                                 )
{
    UInt16 enbSFN = *sendSFN;
    UInt8  enbSF  = *sendSF;
    UInt8  qIndex = 0;

    /* CA TDD Changes Start */
    enbSF = getSfForMsg3(enbSF,cellIndex);
    /* CA TDD Changes End */

    if (enbSF < *sendSF)
    {
        enbSFN++;
        if (enbSFN >= MAX_SFN)
        {
            enbSFN = 0;
        }
    }
    /* SPR 13535 fix start */
    /* + SPR_14589 */
    if ( tddUlDlConfig_g[cellIndex] == 4 && (*sendSF == 8))
    /* - SPR_14589 */
    {
        /* Only case in case RAR is received on DL 8,SFN should be
         * incremented. In case of DL 9, increment in SFN is
         * already taken care */

        enbSFN++;
    }

    /* + SPR_14589 */
    if ( tddUlDlConfig_g[cellIndex] == 5 && ((*sendSF == 7) || (*sendSF == 8)) ) 
    /* - SPR_14589 */
    {
        /* SPR 13535 fix end */
        /* Only case in case RAR is received on DL 7 and 8,SFN should be
         * incremented. In case of DL 9, increment in SFN is
         * already taken care */
        enbSFN++;
    }

    UpdateSfnSf(&enbSFN, &enbSF, ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g[cellIndex]][enbSF]);
    qIndex = (enbSFN * MAX_SUBFRAME + enbSF + PEER_MAC_DELAY) % MAX_UL_ARRAY;

    (*sendSFN) = enbSFN; 
    (*sendSF)  = enbSF;

    return qIndex;
}
/* SPR 2984 Changes */

/****************************************************************************
 * Function Name  : calPdschQIdxSFNAndSF 
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : 
 *
 ****************************************************************************/

inline UInt8 calPdschQIdxSFNAndSF( UInt16 *sendSFN,
                                   UInt8  *sendSF,
                                   UInt8   delay,
                                   /* CA TDD Changes Start */
                                   UInt8   cellIndex
                                   /* CA TDD Changes End */
                                 )
{
    UInt16 enbSFN = *sendSFN;
    UInt8  enbSF  = *sendSF;
    UInt8  qIndex = 0;

    /* CA TDD Changes Start */
    UpdateSfnSf(&enbSFN, &enbSF, ulSfForDlAckNack_g[tddUlDlConfig_g[cellIndex]][enbSF]);
    /* CA TDD Changes End */

    //qIndex = (enbSFN * MAX_SUBFRAME + enbSF - delay + PEER_MAC_DELAY) % MAX_UL_ARRAY;
    qIndex = (enbSFN * MAX_SUBFRAME + enbSF + PEER_MAC_DELAY) % MAX_UL_ARRAY;

    (*sendSFN) = enbSFN; 
    (*sendSF)  = enbSF;

    return qIndex;
}

/****************************************************************************
 * Function Name  :
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 *
 ****************************************************************************/
#if 0
inline calcualteDispatcherQIdx()
{
  /* qIndex represent the Queue Index correspondence the SubFrame */
    qIndex = ((SFN_g * MAX_SUBFRAME) + SF_g + PEER_MAC_DELAY ) % MAX_UL_ARRAY;

}
#endif
/****************************************************************************
 * Function Name  :
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 *
 ****************************************************************************/

inline UInt8 validateTick( UInt16 enbSFN,
                          UInt8  enbSF,
                          UInt16 currentSFN,
                          UInt8  currentSF
                        )
{
     UInt8 delay = 0;
    /* RPT:MAC is working "MAC_ADVANCE_TTI" Tick Ahead so validate the tick by subtracting MAC_ADVANCE_TTI subframes*/
    if (enbSF >= MAC_ADVANCE_TTI)
    {
        if (( delay = validateSFNandSFAndgetDelay(enbSFN, enbSF - MAC_ADVANCE_TTI, currentSFN, currentSF)) == 0xFF )
        {
           /*
            fprintf(stderr,"C_SFN[%d] C_SF[%d] recvSFN[%d] recvSF[%d]\n",
                    currentSFN, currentSF, enbSFN, enbSF);*/
            return  0xFF;
        }
        return delay;
    }
    else if (enbSFN != 0)
    {
        if (( delay = validateSFNandSFAndgetDelay(enbSFN - 1, enbSF + MAX_SUBFRAME - MAC_ADVANCE_TTI, currentSFN, currentSF)) == 0xFF )
        {
           /* fprintf(stderr,"C_SFN[%d] C_SF[%d] recvSFN[%d] recvSF[%d]\n",
                    currentSFN, currentSF, enbSFN, enbSF);*/
            return  0xFF;
        }
        return delay;
    }
    else
    {
        if (( delay = validateSFNandSFAndgetDelay(MAX_SFN - 1, enbSF + MAX_SUBFRAME - MAC_ADVANCE_TTI, currentSFN, currentSF)) == 0xFF )
        {
            /*fprintf(stderr,"C_SFN[%d] C_SF[%d] recvSFN[%d] recvSF[%d]\n",
                    currentSFN, currentSF, enbSFN, enbSF);*/
            return  0xFF;
        }
        return delay;
    }


}
/****************************************************************************
 * Function Name  : checkAndUpdateDaiConsistency
 * Inputs         : ulContext_p - UE Context for UL,
 *                  enbSF - SF sent by eNodeB with the DCI Format, i.e.
 *                          when the DCI format was sent
 *                  receivedDai - DAI value received in the DCI format PDU
 * Outputs        : Updated daiTrackingRegister in the UL UE context
 * Returns        : TRUE if the received DAI is equal to the total number
 *                  of DL transmissions received so far
 * Description    : Buffers the received DAI count in the UE context and
 *                  compares it to the total count of total DL transmissions
 *                  expected in this group of DL subframes. Should be called
 *                  when a non-zero DCI format PDU is received
 *
 ****************************************************************************/

UInt8 checkAndUpdateDaiConsistency( ueContextForUplink *ulContext_p,
                                    UInt16 enbSFN,
                                    UInt8 enbSF,
                                    UInt8 receivedDai,
                                    UInt16 dciFormat,
                                    /* + CA_TDD_HARQ_CHANGES */
                                    UInt8 rxCellIndex
                                    /* - CA_TDD_HARQ_CHANGES */
                                  )
{
    /*UInt8 daiTrackIndex = daiTrackIndexForSource_gp[enbSF];
      assert(daiTrackIndex != NA);*/
    UInt8 daiTrackIndex = enbSF;
    DaiTrackingRegister *daiTrackingInfo_p = NULL;
    UInt8 isConsistent = TRUE;
    UInt8 tempDai = 0;

    /* + CA_TDD_HARQ_CHANGES */
    daiTrackingInfo_p = &ulContext_p->daiTrackingRegister[rxCellIndex][enbSFN & 1][daiTrackIndex];
    /* - CA_TDD_HARQ_CHANGES */

    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    if ( receivedDai != 0xFF )
    {
        /* SPR 19679: TDD HARQ Multiplexing Changes End*/
    if (FAPI_DL_DCI_FORMAT_0_FOR_DAI != dciFormat)
    {
        // Increment DAI counter for this reception to check later with
        // DCI format 0's DAI to ensure that the correct number of DL
        // transmissions have been received by the UE.
        /* For MIMO Case dci format DCI 2 and 2A may be received
         * along with DCI 0
         */
        daiTrackingInfo_p->daiCount++;
    }
    tempDai = daiTrackingInfo_p->daiCount;
    tempDai = tempDai & 0x03;	
    tempDai = tempDai ? tempDai : 0x04;
    daiTrackingInfo_p->daiValue = tempDai;

    if(tempDai != receivedDai)
    {
        isConsistent = FALSE;
        /*  SPR 5089 changes */
        /*    fprintf(stderr,"IsConsisten is FALSE DAI_Count = [%d] DL_DAI = [%d] UL_DAI = [%d]\n",
              daiTrackingInfo_p->daiCount, tempDai, receivedDai);
         */
        /*  SPR 5089 changes */
    }
        /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
        else
        {
            /* SPR 19679: TDD HARQ Multiplexing Changes End*/
    /* + SPR_14151 */
    daiTrackingInfo_p->daiValue = receivedDai; 
    /* - SPR_14151 */
            /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
            isConsistent = TRUE;
        }
    }
    daiTrackingInfo_p->vuldai++;
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
    return TRUE;
#if 0    
    /* SPR 8081 fix start */
    return isConsistent;
    /* SPR 8081 fix end */
#endif    
}

/* + CA_TDD_HARQ_CHANGES */            
/****************************************************************************
 * Function Name  : buildCATDDUEAckNACK
 * Inputs         : ulContext_p - uplink context of UE,
 *                  dlUEContext_p - downlink context of UE
 *                  harqNode_p - Harq Indication Node
 *                  sendSF - Sf at which harq needs to be sent
 *                  daiTrackingInfo_p - DAI information pointer
 *                  isDaiConsistent - eNB recieved DAi & maintained DAi same or not
 *                  isSCell - whether scell or pcell
 *                  currentSF - 
 *                  qIndex - Index at which node is inserted
 *                  txCellIndex - cellIndex on which UE is attached
 *                  rxCellIndex - cell Index for which processing is done
 * Outputs        : Harq Indication node is filled with harq Buffer.
 * Description    : It fills the harq buffer of harq Indication node according 
 *                  to various TDD DL/UL config considering Harq will be scheduled
 *                  on PUCCH.
 *
 ****************************************************************************/

#ifdef TDD_CONFIG
void buildCATDDUEAckNACK(
        ueContextForUplink *ueContextInfo_p,
#ifdef CLPC_TDD_UNSURE
        DLUEContext *dlUEContext_p,
#endif
        FAPI_harqIndQueueNode_st * harqNode_p,
        UInt8 sendSF, 
        DaiTrackingRegister *daiTrackingInfo_p,
        UInt8 isDaiConsistent,
        UInt8 isSCell,
        UInt8 currentSF,
        UInt8 qIndex,
        UInt8 txCellIndex,
        UInt8 rxCellIndex
        )
{
    UInt8 M = 0;
    UInt8 j = 0;
    /* + SPR 13930 Changes */
    UInt8 tempHarq = FAPI_ACK; 
    /* - SPR 13930 Changes */
    /* + SPR 14110 Changes */
    UInt8 ackNackFieldTB1 = FAPI_ACK;
    UInt8 ackNackFieldTB2 = FAPI_ACK;
    /* - SPR 14110 Changes */
    tddChannelSelectionHarqInfo_st *harqChannelSelectionInfo_p = &(harqNode_p->harqPduInfo.channelSelectionHarqBuffer[0]);
    M = getM_g[tddUlDlConfig_g[rxCellIndex]][sendSF];
    if(1 < M)
    {
        j = daiTrackingInfo_p->daiValue - 1;
        j = isSCell ? (j + M)  : j;

        harqChannelSelectionInfo_p = &(harqNode_p->harqPduInfo.channelSelectionHarqBuffer[j]);
    }

    if(daiTrackingInfo_p->isTB1Valid)
    {
    /* + SPR 14110 Changes */
    //    if( sig_cnt[rxCellIndex] > signalling_ack_nack_g[rxCellIndex]) 
    /* - SPR 14110 Changes */
        {
#ifndef CLPC_TDD_UNSURE
            /* + SPR 14341 Changes */
            if(isSCell)
            {
                tempHarq &= (isDaiConsistent & tb1_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->scellSysNum][ueContextInfo_p->scellAckNackIndex]);
                ackNackFieldTB1 = (isDaiConsistent & tb1_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->scellSysNum][ueContextInfo_p->scellAckNackIndex]);
            }
            else
            {
                tempHarq &= (isDaiConsistent & tb1_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                ackNackFieldTB1 = (isDaiConsistent & tb1_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
            }
            /* - SPR 14341 Changes */
#else
            /*CLPC_CHG*/
            if (isDaiConsistent)
            {
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
                tempHarq &= (dlUEContext_p->
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
                                [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                ackNackFieldTB1 = (dlUEContext_p->tpcAckNackContext.tb1_ack_nack_perUe_array
                                [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
            }
            /*CLPC_CHG*/
#endif
        }
/* + SPR 14110 Changes */
#if 0        
        else        
        {
            sig_cnt[rxCellIndex]++;
        }
#endif        
/* - SPR 14110 Changes */
    }
    if(daiTrackingInfo_p->isTB2Valid)
    {
/* + SPR 14110 Changes */
//        if( sig_cnt[rxCellIndex] > signalling_ack_nack_g[rxCellIndex])
/* - SPR 14110 Changes */
        {
#ifndef CLPC_TDD_UNSURE
            /* + SPR 14341 Changes */
            if(isSCell)
            {
                tempHarq &= (isDaiConsistent & tb2_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->scellSysNum][ueContextInfo_p->scellAckNackIndex]);
                ackNackFieldTB2 = (isDaiConsistent & tb2_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->scellSysNum][ueContextInfo_p->scellAckNackIndex]);
            }
            else
            {
                tempHarq &= (isDaiConsistent & tb2_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                ackNackFieldTB2 = (isDaiConsistent & tb2_ack_nack_value_array[rxCellIndex]
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
            }
            /* - SPR 14341 Changes */
#else
            /*CLPC_CHG*/
            if (isDaiConsistent)
            {
                tempHarq &= (dlUEContext_p->tpcAckNackContext.tb2_ack_nack_perUe_array
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                ackNackFieldTB2 = (dlUEContext_p->tpcAckNackContext.tb2_ack_nack_perUe_array
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
            }
            /*CLPC_CHG*/
#endif
        }
/* + SPR 14110 Changes */
#if 0        
        else        
        {
            sig_cnt[rxCellIndex]++;
        }
#endif        
/* - SPR 14110 Changes */
    }

    /* + SPR 14176 Changes */
     harqChannelSelectionInfo_p->scheduledSF = currentSF;
    /* - SPR 14176 Changes */
    if (1 == M)
    {
        fapiSendHarqIndication(ueContextInfo_p->crnti,qIndex,&ackNackFieldTB1,&ackNackFieldTB2,harqNode_p,txCellIndex,rxCellIndex);
        daiTrackingInfo_p->ackNackFieldTB1 = ackNackFieldTB1;
        daiTrackingInfo_p->ackNackFieldTB2 = ackNackFieldTB2;
    }
    else if(1 < M)
    {
        /* + SPR 14176 Changes */
//        harqChannelSelectionInfo_p->scheduledSF = currentSF;
        /* - SPR 14176 Changes */

        harqChannelSelectionInfo_p->value0 = tempHarq;
        daiTrackingInfo_p->ackNackFieldTB1 = daiTrackingInfo_p->ackNackFieldTB2 = tempHarq;

        if (2 == M)
        {
            harqNode_p->isScheduledOnPUSCH = TRUE;
        }
    }

    if( TRUE == msgSfDlConfigErr_g[rxCellIndex] ) 
    {
        harqChannelSelectionInfo_p->value0 = FAPI_NACK;
        msgSfDlConfigErr_g[rxCellIndex] = FALSE;
    }
}
#endif
/* - CA_TDD_HARQ_CHANGES */

/****************************************************************************
 * Function Name  :  buildDlAckNack
 * Inputs         :  msgIdArr, qIndex, sendSFN, sendSF, dai
 * Outputs        :
 * Returns        :
 * Description    : This function will build Ack/Nack for Dl data recvd
 *
 ****************************************************************************/
void buildDlAckNack( ueContextForUplink *ueContextInfo_p,
                     UInt8 qIndex,
                     UInt16 sendSFN, 
                     UInt8 sendSF, 
                     UInt8 dai,
                     UInt8 isDaiConsistent,
                     UInt8 dciFormat,
                     UInt8 isTB1Valid,
                     UInt8 isTB2Valid,
                     /* + CA_TDD_HARQ_CHANGES */            
                     UInt8 isSCell,
                     UInt8 currentSF,
                     UInt8 txCellIndex,
                     UInt8 rxCellIndex
                     /* - CA_TDD_HARQ_CHANGES */
                   )
{
    /* TTD Code */
    static UInt8 ackNackIndex = 0;
    static UInt8 sysNum = 0;
    //check already node exist then just update the tb ack nack value
    FAPI_harqIndQueueNode_st *harqNode_p = PNULL;
    UInt8 harqContInd = sendSFN & 0x01;
    /* for temp. added */
    UInt16 crnti = ueContextInfo_p->crnti;
    UInt32 sendSfnSf = 0;
    FAPI_tddBundlingHarqInfo *harqBundleInfo_p = NULL;
    FAPI_tddSpecialBundlingHarqInfo_st *harqSpBundleInfo_p = NULL;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    tddMultiplexingHarqInfo_st *harqMultiplexInfo_p = NULL;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    DaiTrackingRegister *daiTrackingInfo_p = NULL;
#ifdef CLPC_TDD_UNSURE
    /*CLPC_CHG*/
    DLUEContext *dlUEContext_p = PNULL;
    dlUEContext_p = dlUECtxInfoArr_g[ueContextInfo_p->ueIdx].dlUEContext_p;
    if(dlUEContext_p == PNULL)
    {
        fprintf(stderr,"DL Context is NULL\n");
        return FAPI_NACK;
    }
    /*CLPC_CHG*/
#else
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    
    DLUEContext *dlUEContext_p = PNULL;
    dlUEContext_p = dlUECtxInfoArr_g[ueContextInfo_p->ueIdx].dlUEContext_p;
    if(dlUEContext_p == PNULL)
    {
        fprintf(stderr,"DL Context is NULL\n");
    }
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif

    daiTrackingInfo_p = &ueContextInfo_p->daiTrackingRegister[rxCellIndex][harqContInd][sendSF];

    /* TB validation is done for all DL Assignment */
    
    daiTrackingInfo_p->isTB1Valid |= isTB1Valid;
    daiTrackingInfo_p->isTB2Valid |= isTB2Valid;
    
    harqNode_p = ( FAPI_harqIndQueueNode_st *)
                    ueContextInfo_p->harqNode_p[harqContInd][sendSF];
#if 0
    fprintf(stderr, "\n ## SFN [%d] SF[%d] sendSFN[%d] sendSF[%d] index[%d] dciFormat[%d] harqNode_p[%x] dai[%d]\n", 
                    SFN_g, SF_g, sendSFN, sendSF, harqContInd, dciFormat, harqNode_p, dai);
#endif                    
    if ((NULL == harqNode_p) && (8 != dciFormat))// if ( harqNode_p = uecontext....ref == PNULL)
    {
        harqNode_p = ( FAPI_harqIndQueueNode_st *) getMemFromPool (
                sizeof (FAPI_harqIndQueueNode_st), PNULL );
        if ( PNULL == harqNode_p )
        {
            fprintf(stderr," in function: fapiSendFddHarqIndication mem alloc"
                    "failed while sending harq indication for crnti = %d\n", 
                    crnti);
            return;
        }
        ueContextInfo_p->harqNode_p[harqContInd][sendSF] = (void *)harqNode_p;
        harqNode_p->sfnsf = GENERATE_SUBFRAME_SFNSF( sendSFN,sendSF);
        /* TBD : Handle value need some more understanding 
         * Currently value ZERO for UCI and ONE for ULSCH
         */
        harqNode_p->harqPduInfo.handle = 1; 
        harqNode_p->harqPduInfo.rnti = crnti;
        harqNode_p->harqPduInfo.mode = ueContextInfo_p->dlAckNackMode;
        harqNode_p->harqPduInfo.mode = 0;
    /* SPR 8081 fix start */
        harqNode_p->harqPduInfo.numOfAckNack = 0;
    /* SPR 8081 fix end */
        /* + SPR 14192 Changes */ 
        harqNode_p->isScheduledOnPUSCH = 0;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
        harqNode_p->isScheduledOnPUSCHForMux = 0;
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
        /* - SPR 14192 Changes */ 

        /* + SPR 14341 Changes */
        if(isSCell)
        {
            ueContextInfo_p->scellAckNackIndex++;
            if (ueContextInfo_p->scellAckNackIndex >= ACK_NACK_ARRAY_SIZE)
            {
                ueContextInfo_p->scellAckNackIndex = 0;
                ueContextInfo_p->scellSysNum  = ueContextInfo_p->scellSysNum + 1;
                if (ueContextInfo_p->scellSysNum == ACK_NACK_ARRAY_SIZE)
                {
                    ueContextInfo_p->scellSysNum = 0;
                }
            }
        }
        /* - SPR 14341 Changes */
        else
        { 
   	        /* SPR 7964/8081 changes start */
            ueContextInfo_p->ackNackIndex++; 
            /* 128ue_tdd_support start*/
            if (ueContextInfo_p->ackNackIndex >= ACK_NACK_ARRAY_SIZE)
            /* 128ue_tdd_support end*/
            {
                ueContextInfo_p->ackNackIndex = 0;
                ueContextInfo_p->sysNum  = ueContextInfo_p->sysNum + 1;
                if (ueContextInfo_p->sysNum == ACK_NACK_ARRAY_SIZE)
                {
                    ueContextInfo_p->sysNum = 0;
                }
            }
        	/* SPR 7964/8081 changes start */
        }
        
        if (BUNDLING == ueContextInfo_p->ackNackMode)
        {
            harqBundleInfo_p = &(harqNode_p->harqPduInfo.harqBuffer);

    /* SPR 8081 fix start */
            /*
            harqBundleInfo_p->value0 = TRUE;
            harqBundleInfo_p->value1 = TRUE;
            */
    /* SPR 8081 fix end */
            /*SPR 13884 changes start */
            harqBundleInfo_p->value0 = INVALID_ACK_NACK;
            harqBundleInfo_p->value1 = INVALID_ACK_NACK;
           /*SPR 13884 changes end */
            /* Check which TB is valid and update Ack Nack based on DAI 
             * consistency
             * isDaiConsistent would be TRUE if DAI information received so far is
             * correct the Emulator did not miss a transmission
             */
            if(daiTrackingInfo_p->isTB1Valid)
            {
    /* SPR 8081 fix start */
            /*SPR 13884 changes start */
                harqNode_p->harqPduInfo.numOfAckNack++;
           /*SPR 13884 changes end */
    /* SPR 8081 fix end */
                if( sig_cnt[txCellIndex] > signalling_ack_nack_g[txCellIndex])
                {
#ifndef CLPC_TDD_UNSURE
                    harqBundleInfo_p->value0 = (isDaiConsistent
                         & tb1_ack_nack_value_array[txCellIndex]
						 [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                       
#else
                    /*CLPC_CHG*/
                    if (isDaiConsistent)
                    {
                    harqBundleInfo_p->value0 = (dlUEContext_p->tpcAckNackContext.tb1_ack_nack_perUe_array
                            [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                    }
                    else
                    {
                        harqBundleInfo_p->value0 = isDaiConsistent;
                    }
                    /*CLPC_CHG*/
#endif

                }
                else        
                {
                    harqBundleInfo_p->value0 = isDaiConsistent;
                    sig_cnt[txCellIndex]++;
                }
            }
            if(daiTrackingInfo_p->isTB2Valid)
            {
    /* SPR 8081 fix start */
            /*SPR 13884 changes start */
                harqNode_p->harqPduInfo.numOfAckNack++;
           /*SPR 13884 changes end */
    /* SPR 8081 fix end */
#ifndef CLPC_TDD_UNSURE                
                harqBundleInfo_p->value1 = (isDaiConsistent
                   & tb2_ack_nack_value_array[txCellIndex][ueContextInfo_p->sysNum]
				   [ueContextInfo_p->ackNackIndex]);
#else
                /*CLPC_CHG*/
                if (isDaiConsistent)
                {
                    harqBundleInfo_p->value1 = (dlUEContext_p->tpcAckNackContext.tb2_ack_nack_perUe_array
                            [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
                }
                else
                {
                    harqBundleInfo_p->value1 = isDaiConsistent;
                }
                fprintf(stderr,"harqBundleInfo_p->value1=%d\n",harqBundleInfo_p->value1);
                /*CLPC_CHG*/
#endif

            }

            daiTrackingInfo_p->ackNackFieldTB1 = harqBundleInfo_p->value0;
            daiTrackingInfo_p->ackNackFieldTB2 = harqBundleInfo_p->value1;

            /* CA TDD Changes Start */
            if( TRUE == msgSfDlConfigErr_g[txCellIndex] )
            {
                harqBundleInfo_p->value0 = FAPI_NACK;
                harqBundleInfo_p->value1 = FAPI_NACK;
                msgSfDlConfigErr_g[txCellIndex] = FALSE;
            }
            /* CA TDD Changes End */
        }
            /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
        else if (MULTIPLEXING == ueContextInfo_p->ackNackMode)
        {
            UInt8 M = 0;
            UInt8 j = 0;
            UInt8 index = 0;
            /*initialising the value0 and value1 of multiplexingHarqBuffer*/
            for (index =0 ; index < 4; index++)
            {
                harqNode_p->harqPduInfo.multiplexingHarqBuffer[index].value0
                    = FAPI_DTX; /*for TB1*/
                harqNode_p->harqPduInfo.multiplexingHarqBuffer[index].value1 
                    = FAPI_DTX; /*for TB2*/
                harqNode_p->harqPduInfo.multiplexingHarqBuffer[index].
                    scheduledSF = MAX_SUBFRAME;
                harqNode_p->harqPduInfo.multiplexingHarqBuffer[index].spsOcc = FALSE;
                harqNode_p->harqPduInfo.multiplexingHarqBuffer[index].daiIndexValue = 20;
            }
            M = getM_g[tddUlDlConfig_g[txCellIndex]][sendSF]; /*calculating the value of M */
            if(1 < M)
            {
                /*filling of numOfAckNack  in case M > 1 */
                harqNode_p->harqPduInfo.numOfAckNack++;
            }
            else 
            {
#if 1            
                if ( daiTrackingInfo_p->isTB1Valid )
                    harqNode_p->harqPduInfo.numOfAckNack++;
                if ( daiTrackingInfo_p->isTB2Valid )
                    harqNode_p->harqPduInfo.numOfAckNack++;
#endif

#if 0                
                /*filling of numOfAckNack  in case of SISO for M = 1 */
                if ((1 == dlUEContext_p->transmissionMode) || (2 == dlUEContext_p->transmissionMode) || (6 == dlUEContext_p->transmissionMode)|| (7 == dlUEContext_p->transmissionMode) || (8 == dlUEContext_p->transmissionMode))
                {
                    harqNode_p->harqPduInfo.numOfAckNack = 1;
                }
                /*filling of numOfAckNack  in case of MIMO for M = 1 */
                else if ((3 == dlUEContext_p->transmissionMode) || (4 == dlUEContext_p->transmissionMode))
                {
                    harqNode_p->harqPduInfo.numOfAckNack = 2;
                }
#endif                
            }
            harqMultiplexInfo_p = 
                &(harqNode_p->harqPduInfo.multiplexingHarqBuffer[0]);
            harqNode_p->harqPduInfo.mode = ueContextInfo_p->ackNackMode; 
            if (1 < M)
            {
                j = daiTrackingInfo_p->vuldai - 1;
                harqMultiplexInfo_p = 
                    &(harqNode_p->harqPduInfo.multiplexingHarqBuffer[j]);
                harqMultiplexInfo_p->daiIndexValue = daiTrackingInfo_p->daiValue;
            }
            if ( 0xFF == dai )
            {
                harqMultiplexInfo_p->spsOcc = TRUE;
            }
            if(daiTrackingInfo_p->isTB1Valid)
            {
#ifndef CLPC_TDD_UNSURE
                harqMultiplexInfo_p->value0 = (isDaiConsistent & 
                        tb1_ack_nack_value_array[txCellIndex]
                        [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);
#else
                if (isDaiConsistent)
                {
                    harqMultiplexInfo_p->value0 = 
                        (dlUEContext_p->tpcAckNackContext.
                         tb1_ack_nack_perUe_array[ueContextInfo_p->sysNum]
                         [ueContextInfo_p->ackNackIndex]);
                }
#endif
            }
            else
            {
                harqMultiplexInfo_p->value0 = FAPI_ACK;
            }
            /*fill the values of harqMultiplexInfo_p if TB2 is valid*/
           
            if(daiTrackingInfo_p->isTB2Valid)
            {
#ifndef CLPC_TDD_UNSURE
                harqMultiplexInfo_p->value1 = 
                    (isDaiConsistent & 
                     tb2_ack_nack_value_array[txCellIndex]
                     [ueContextInfo_p->sysNum]
                     [ueContextInfo_p->ackNackIndex]);
#else
                if (isDaiConsistent)
                {
                    harqMultiplexInfo_p->value1 
                        = (dlUEContext_p->tpcAckNackContext.
                                tb2_ack_nack_perUe_array[ueContextInfo_p->sysNum]
                                [ueContextInfo_p->ackNackIndex]);
                }
#endif
            }
            else
            {
                harqMultiplexInfo_p->value1 = FAPI_ACK;
            }

            harqNode_p->harqPduInfo.multiplexingHarqBuffer[j].scheduledSF 
                = currentSF;
            daiTrackingInfo_p->ackNackFieldTB1 = harqMultiplexInfo_p->value0;
            daiTrackingInfo_p->ackNackFieldTB2 = harqMultiplexInfo_p->value1; 
           
            if( TRUE == msgSfDlConfigErr_g[txCellIndex] )
            {
                harqMultiplexInfo_p->value0 = FAPI_NACK;
                harqMultiplexInfo_p->value1 = FAPI_NACK;
                msgSfDlConfigErr_g[txCellIndex] = FALSE;
            }
        }
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
        /* + CA_TDD_HARQ_CHANGES */
        else if(CHANNEL_SELECTION == ueContextInfo_p->ackNackMode)
        {
            UInt8 M = 0;
            M = getM_g[tddUlDlConfig_g[txCellIndex]][sendSF];

            harqNode_p->harqPduInfo.numOfAckNack = (M * 2);
            harqNode_p->harqPduInfo.mode = CHANNEL_SELECTION;
            /* + SPR_14151 */
            UInt8 i = 0;
            /* + SPR_14278 */
            if (M >= 2)
            /* - SPR_14278 */
            {    
                for (i = 0; i< harqNode_p->harqPduInfo.numOfAckNack; i++)
                {    
                    harqNode_p->harqPduInfo.channelSelectionHarqBuffer[i].value0 = 1; 
            /* + SPR_14278 */
                    harqNode_p->harqPduInfo.channelSelectionHarqBuffer[i].scheduledSF = MAX_SUBFRAME; 
            /* - SPR_14278 */
                }
            }
#if 0            
            else
            {
                harqNode_p->harqPduInfo.channelSelectionHarqBuffer[0].value0 = 3;
                harqNode_p->harqPduInfo.channelSelectionHarqBuffer[1].value0 = 3;
                harqNode_p->harqPduInfo.channelSelectionHarqBuffer[2].value0 = 3;
                harqNode_p->harqPduInfo.channelSelectionHarqBuffer[3].value0 = 3;
            }                
#endif            
            /* - SPR_14151 */
            buildCATDDUEAckNACK(ueContextInfo_p,
#ifdef CLPC_TDD_UNSURE
                    dlUEContext_p,
#endif
                    harqNode_p, sendSF, daiTrackingInfo_p, isDaiConsistent, 
                    isSCell, currentSF, qIndex, txCellIndex, rxCellIndex);

        }
        /* - CA_TDD_HARQ_CHANGES */
        else
        {
            harqBundleInfo_p = (FAPI_tddSpecialBundlingHarqInfo_st *)
                                    &(harqNode_p->harqPduInfo.harqBuffer);
            ltePanic("2. not supported yet\n");
            /* Support Need to added */
        }
#ifdef UE_SIM_ENABLE_PRINTF_LOGS
        fprintf(stderr,"harq node pushed into harq queue at index = %d"
                " for rnti = %d, sfnsf = %d\n",
                qIndex, harqNode_p->harqPduInfo.rnti, harqNode_p->sfnsf);
#endif
        pushNode(&(harqQueue[txCellIndex][qIndex]), &(harqNode_p->harqNodeAnchor));
    }
    else if((NULL != harqNode_p))
    {
        if (BUNDLING == ueContextInfo_p->ackNackMode)
        {
            harqBundleInfo_p = &(harqNode_p->harqPduInfo.harqBuffer);
            /*SPR 13884 changes start */
      //      harqNode_p->harqPduInfo.numOfAckNack = daiTrackingInfo_p->daiValue;

            /*SPR 13884 changes end */
            if(daiTrackingInfo_p->isTB1Valid)
            {
                if( sig_cnt[txCellIndex] > signalling_ack_nack_g[txCellIndex])
                {
                    /*SPR 13884 changes start */
                    if(harqBundleInfo_p->value0 == INVALID_ACK_NACK)
                    {
                       harqNode_p->harqPduInfo.numOfAckNack++;
                    }
                    /*SPR 13884 changes end */
#ifndef CLPC_TDD_UNSURE
                    harqBundleInfo_p->value0 = (isDaiConsistent
                            & harqBundleInfo_p->value0);
#else
                    /*CLPC_CHG*/
                    if (isDaiConsistent)
                    {
                        harqBundleInfo_p->value0 = harqBundleInfo_p->value0;
                    }
                    else
                    {
                        harqBundleInfo_p->value0 = isDaiConsistent;
                    }
                    fprintf(stderr,"harqBundleInfo_p->value0=%d\n",harqBundleInfo_p->value0);
                    /*CLPC_CHG*/
#endif
                }
                else        
                {
                    harqBundleInfo_p->value0 = isDaiConsistent;
                    sig_cnt[txCellIndex]++;
                }
            }
            if(daiTrackingInfo_p->isTB2Valid)
            {
                    /*SPR 13884 changes start */
                if(harqBundleInfo_p->value1 == INVALID_ACK_NACK)
                {
                   harqNode_p->harqPduInfo.numOfAckNack++;
                }
                    /*SPR 13884 changes end */
#ifndef CLPC_TDD_UNSURE
                harqBundleInfo_p->value1 = (isDaiConsistent
                        & harqBundleInfo_p->value1);
#else
                /*CLPC_CHG*/
                if (isDaiConsistent)
                {
                    harqBundleInfo_p->value1 = harqBundleInfo_p->value1;
                }
                else
                {
                    harqBundleInfo_p->value1 = isDaiConsistent;
                }
                fprintf(stderr,"harqBundleInfo_p->value1=%d\n",harqBundleInfo_p->value1);
                /*CLPC_CHG*/
#endif                

            }
/* SPR 7964 changes start */
#if 0
            ueContextInfo_p->ackNackIndex++; 
            if (ueContextInfo_p->ackNackIndex == ACK_NACK_ARRAY_SIZE)
            {
                ueContextInfo_p->ackNackIndex = 0;
                ueContextInfo_p->sysNum  = ueContextInfo_p->sysNum + 1;
                if (ueContextInfo_p->sysNum == ACK_NACK_ARRAY_SIZE)
                {
                    ueContextInfo_p->sysNum = 0;
                }
            }
#endif
/* SPR 7964 changes end */
            daiTrackingInfo_p->ackNackFieldTB1 = harqBundleInfo_p->value0;
            daiTrackingInfo_p->ackNackFieldTB2 = harqBundleInfo_p->value1;
            
            /* CA TDD Changes Start */
            if( TRUE == msgSfDlConfigErr_g[txCellIndex] )
            {
                harqBundleInfo_p->value0 = FAPI_NACK;
                harqBundleInfo_p->value1 = FAPI_NACK;
                msgSfDlConfigErr_g[txCellIndex] = FALSE;
            }
            /* CA TDD Changes End */
        }
            /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
        else if (MULTIPLEXING == ueContextInfo_p->ackNackMode)
        {
            UInt8 M = 0;
            UInt8 j = 0;
            UInt8 index = 0;

            M = getM_g[tddUlDlConfig_g[txCellIndex]][sendSF];/*calculating the value of M */
            if(1 < M)
            {
                /*filling of numOfAckNack  in case M > 1 */
                harqNode_p->harqPduInfo.numOfAckNack++;
            }
            else 
            {
                /*filling of numOfAckNack  in case of SISO for M = 1 */ 
                if ((1 == dlUEContext_p->transmissionMode) || (2 == dlUEContext_p->transmissionMode) || (6 == dlUEContext_p->transmissionMode)|| (7 == dlUEContext_p->transmissionMode) || (8 == dlUEContext_p->transmissionMode))
                {
                    harqNode_p->harqPduInfo.numOfAckNack = 1;
                }
                /*filling of numOfAckNack  in case of MIMO for M = 1 */
                else if ((3 == dlUEContext_p->transmissionMode) || (4 == dlUEContext_p->transmissionMode))
                {
                    harqNode_p->harqPduInfo.numOfAckNack = 2;
                }
            }
            harqMultiplexInfo_p = 
                &(harqNode_p->harqPduInfo.multiplexingHarqBuffer[0]);
            harqNode_p->harqPduInfo.mode = ueContextInfo_p->ackNackMode;
            /*initialising the multiplexingHarqBuffer array elements*/
            if (1 < M)
            {
                j = daiTrackingInfo_p->vuldai - 1;
                harqMultiplexInfo_p = 
                    &(harqNode_p->harqPduInfo.multiplexingHarqBuffer[j]);
                harqMultiplexInfo_p->daiIndexValue = daiTrackingInfo_p->daiValue;
            }
            if ( 0xFF == dai )
            {
                harqMultiplexInfo_p->spsOcc = TRUE;
            }
            if(daiTrackingInfo_p->isTB1Valid)
            {
#ifndef CLPC_TDD_UNSURE
                  harqMultiplexInfo_p->value0 = (isDaiConsistent & tb1_ack_nack_value_array[txCellIndex]
                  [ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]);


#else
                if (isDaiConsistent)
                {
                    harqMultiplexInfo_p->value0 = harqMultiplexInfo_p->value0;
                }
                else
                {
                    harqMultiplexInfo_p->value0 = isDaiConsistent;
                }

#endif
            }
            else
            {
                harqMultiplexInfo_p->value0 = FAPI_ACK;
            }

            if(daiTrackingInfo_p->isTB2Valid)
            {
#ifndef CLPC_TDD_UNSURE
                harqMultiplexInfo_p->value1 = (isDaiConsistent & 
                           tb2_ack_nack_value_array[txCellIndex][ueContextInfo_p->sysNum][ueContextInfo_p->ackNackIndex]  );
#else
                if (isDaiConsistent)
                {
                    harqMultiplexInfo_p->value1 = harqMultiplexInfo_p->value1;
                }
                else
                {
                    harqMultiplexInfo_p->value1 = isDaiConsistent;
        }
#endif
            }
            else
            {
                harqMultiplexInfo_p->value1 = FAPI_ACK;
            }
            harqNode_p->harqPduInfo.multiplexingHarqBuffer[j].scheduledSF 
                = currentSF;
            daiTrackingInfo_p->ackNackFieldTB1 = harqMultiplexInfo_p->value0;
            daiTrackingInfo_p->ackNackFieldTB2 = harqMultiplexInfo_p->value1; 

            if( TRUE == msgSfDlConfigErr_g[txCellIndex] )
            {
                harqMultiplexInfo_p->value0 = FAPI_NACK;
                harqMultiplexInfo_p->value1 = FAPI_NACK;
                msgSfDlConfigErr_g[txCellIndex] = FALSE;
            }
        }
            /* SPR 19679: TDD HARQ Multiplexing Changes End*/
        /* + CA_TDD_HARQ_CHANGES */
        else if(CHANNEL_SELECTION == ueContextInfo_p->ackNackMode)
        {
            /* + SPR 14341 Changes */
            if(isSCell)
            {
                ueContextInfo_p->scellAckNackIndex++;
                if (ueContextInfo_p->scellAckNackIndex >= ACK_NACK_ARRAY_SIZE)
                {
                    ueContextInfo_p->scellAckNackIndex = 0;
                    ueContextInfo_p->scellSysNum  = ueContextInfo_p->scellSysNum + 1;
                    if (ueContextInfo_p->scellSysNum == ACK_NACK_ARRAY_SIZE)
                    {
                        ueContextInfo_p->scellSysNum = 0;
                    }
                }
            }
            /* - SPR 14341 Changes */
            else
            {
                /* SPR 7964/8081 changes start */
                ueContextInfo_p->ackNackIndex++;
                /* 128ue_tdd_support start*/
                if (ueContextInfo_p->ackNackIndex >= ACK_NACK_ARRAY_SIZE)
                /* 128ue_tdd_support end*/
                {
                    ueContextInfo_p->ackNackIndex = 0;
                    ueContextInfo_p->sysNum  = ueContextInfo_p->sysNum + 1;
                    if (ueContextInfo_p->sysNum == ACK_NACK_ARRAY_SIZE)
                    {
                        ueContextInfo_p->sysNum = 0;
                    }
                }
                /* SPR 7964/8081 changes start */
            }

            buildCATDDUEAckNACK(ueContextInfo_p,
#ifdef CLPC_TDD_UNSURE
                    dlUEContext_p,
#endif
                    harqNode_p, sendSF, daiTrackingInfo_p, isDaiConsistent, 
                    isSCell, currentSF, qIndex, txCellIndex, rxCellIndex);

        }
        /* - CA_TDD_HARQ_CHANGES */
        else
        {
            /* Support Need to added */
        }

    }
    else if(8 == dciFormat)
    {
	    daiTrackingInfo_p->daiUpdated = FALSE;
    }
}

/****************************************************************************
 * Function Name  :  
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : 
 *
 ****************************************************************************/
void updateDaiAndBuildDlAckNack(ueContextForUplink *ulContext_p,
                                    UInt16 enbSFN,
                                    UInt8 enbSF,
                                    UInt8 receivedDai,
                                    UInt16 dciFormat,
                                    UInt8 qIndex,
                                    UInt8 isTB1Valid,
                                    UInt8 isTB2Valid,
                                    /* + CA_TDD_HARQ_CHANGES */
                                    UInt8 currentSF,
                                    UInt8 rxCellIndex
                                    /* - CA_TDD_HARQ_CHANGES */
                                    )
{
    UInt8 isConsistent = FALSE;
    /* + CA_TDD_HARQ_CHANGES */
    UInt8 isSCell = FALSE;
    UInt8 txCellIndex = 0;

    if (ulContext_p)
    {
        GET_RNTI_TO_CELL_INDEX(ulContext_p->crnti,txCellIndex)
        if (txCellIndex != rxCellIndex)
        {
            isSCell = TRUE;
        }
    }
    /* - CA_TDD_HARQ_CHANGES */
    /* + SPS_TDD_Changes */
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    /* SPR 20036 Fix ++ */
#if 0    
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
    if (0xFF == receivedDai)
    {
        isConsistent = TRUE;
    }
    else
    {
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
#endif  
    /* SPR 20036 Fix -- */
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
        /* - SPS_TDD_Changes */

        isConsistent = checkAndUpdateDaiConsistency( ulContext_p, 
                enbSFN,
                enbSF,
                receivedDai,
                dciFormat,
                /* + CA_TDD_HARQ_CHANGES */
                rxCellIndex
                /* - CA_TDD_HARQ_CHANGES */
                );
        /* + SPS_TDD_Changes */
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    /* SPR 20036 Fix ++ */
#if 0
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
    }
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
#endif
    /* SPR 20036 Fix -- */
    /* SPR 19679: TDD HARQ Multiplexing Changes End*/
    /* - SPS_TDD_Changes */
    buildDlAckNack( ulContext_p, 
            qIndex,
            enbSFN, 
            enbSF, 
            receivedDai,
            isConsistent,
            dciFormat,
            isTB1Valid,
            isTB2Valid,
            /* + CA_TDD_HARQ_CHANGES */
            isSCell,
            currentSF,
            txCellIndex,
            rxCellIndex
            /* - CA_TDD_HARQ_CHANGES */
            );
}

/****************************************************************************
 * Function Name  :  
 * Inputs         :  
 * Outputs        :
 * Returns        :
 * Description    : 
 *
 ****************************************************************************/
void calcNextSRILocation(ueContextForUplink *ue_p, UInt16 *sendSFN_p, UInt8 *sendSF_p, UInt8 cellIndex)
{
    const UInt16 sfn = *sendSFN_p;
    const UInt8 sf = *sendSF_p;
    UInt32 lastSFCount = MAX_SUBFRAME * ue_p->lastSRISchedule.sfn + ue_p->lastSRISchedule.sf;
    UInt32 curSFCount = MAX_SUBFRAME * sfn + sf;

    // For debugging purposes only
    // struct timespec start = {0, 0};
    // struct timespec end = {0, 0};

    //assert(ue_p->lastSRISchedule.isValid == TRUE);

    // For debugging purposes only
    // clock_gettime(CLOCK_REALTIME, &start);
    if (ue_p->lastSRISchedule.sfn == sfn && ue_p->lastSRISchedule.sf == sf)
    {
        *sendSFN_p = sfn;
        *sendSF_p = sf;

        return;
    }
  if (lastSFCount < curSFCount)
    {
        for (;lastSFCount < curSFCount; lastSFCount += ue_p->srPeriodicity)
        {}
        for (;
            /* CA TDD Changes Start */
            !IS_UL_SUBFRAME(lastSFCount % MAX_SUBFRAME, cellIndex);
            /* CA TDD Changes End */
            lastSFCount += ue_p->srPeriodicity)
        {
            if (lastSFCount >= (MAX_SFN * MAX_SUBFRAME))
            {
                lastSFCount -= MAX_SFN * MAX_SUBFRAME;
            }
        }
    }
    else    // lastSFCount == curSFCount is being handled at the beginning of the func
    {
        for (; lastSFCount <= (MAX_SFN * MAX_SUBFRAME); lastSFCount += ue_p->srPeriodicity)
        {}
        lastSFCount -= MAX_SFN * MAX_SUBFRAME;  // Roll back
        for (; lastSFCount < curSFCount; lastSFCount += ue_p->srPeriodicity)
        {}
        for (;
            /* CA TDD Changes Start */
            !IS_UL_SUBFRAME(lastSFCount % MAX_SUBFRAME, cellIndex);
            /* CA TDD Changes End */
            lastSFCount += ue_p->srPeriodicity)
        {
            if (lastSFCount >= (MAX_SFN * MAX_SUBFRAME))
            {
                lastSFCount -= MAX_SFN * MAX_SUBFRAME;
            }
        }
    }

    /* If the next calculated SRI opportunity coicides with the
     * current tick it is incremented by another Periodicity
     * interval to ensure that the SRI reaches eNB well in advance
     * (considering a minimum delay (including PHY processing and
     * transmission) of 2. If periodicity is 1 then we advance it
     * by 2 becuase we're processing data 2 ticks in advance */
    if (lastSFCount == curSFCount)
    {
        if (ue_p->srPeriodicity == 1)
        {
            lastSFCount += 2;
        }
        else
        {
            lastSFCount += ue_p->srPeriodicity;
        }
        if (lastSFCount >= (MAX_SFN * MAX_SUBFRAME))
        {
            lastSFCount -= MAX_SFN * MAX_SUBFRAME;
        }
    }

    ue_p->lastSRISchedule.sfn = lastSFCount / MAX_SUBFRAME;
    *sendSF_p = ue_p->lastSRISchedule.sf = lastSFCount % MAX_SUBFRAME;

    if (ue_p->lastSRISchedule.sfn == MAX_SFN)
    {
        ue_p->lastSRISchedule.sfn = 0;
    }
    *sendSFN_p = ue_p->lastSRISchedule.sfn;    
#if 0
    fprintf(stdout, "Next SR opportunity available at SFN.SF = %d.%d, cur SFN.SF = %d.%d\n",
    ue_p->lastSRISchedule.sfn, ue_p->lastSRISchedule.sf, sfn, sf);
#endif
    // For debugging purposes only
    // clock_gettime(CLOCK_REALTIME, &end);
    // printf("SRI Next Calc processing time: %lu s, %lu nsec\n",
    //  (unsigned long int)(end.tv_sec - start.tv_sec), end.tv_nsec - start.tv_nsec);
}

/****************************************************************************
 * Function Name  :  checkAvaibilityOfHarqProcess
 * Inputs         :  ueContextInfo_p
 * Outputs        :
 * Returns        :  harqProcessId if FREE or -1 if all processes are busy
 * Description    : This function will chek the avaibility of Harq process
 *
 ****************************************************************************/
SInt8 checkAvailabilityOfHarqProcess(ueContextForUplink *ueContextInfo_p)
{
    SInt8 harqProcessId = -1;
    SInt8 i = 0;
    ULHARQEntity  *harqEntity = ueContextInfo_p->ulHarqEntity_p;

    for (i = 0; i < harqEntity->numHARQProcess; i++)
    {
        if (HARQ_PROCESS_FREE == harqEntity->ulHarqProcess[i].harqStatus)
        {
            harqProcessId = i;
            return harqProcessId;
        }
    }
    return harqProcessId;
}

/****************************************************************************
 * Function Name  :  calcFirstSRILocation 
 * Inputs         :  ueContextInfo_p
 * Outputs        :
 * Returns        :  harqProcessId if FREE or -1 if all processes are busy
 * Description    : This function will chek the avaibility of Harq process
 *
 ****************************************************************************/

void calcFirstSRILocation(ueContextForUplink *ue_p, UInt16 *sendSFN_p, UInt8 *sendSF_p, UInt8 cellIndex)
{
    UInt16 sfn = SFN_g[cellIndex];
    UInt8  sf = SF_g[cellIndex];
    UInt8 srPeriodicity     = 0;
    UInt8 srSubframeOffset = 0;     // N_OFFSET_SR

    // Refer Table 10.1-5, 3GPP TS 36.213
    if (ue_p->srConfigurationIndex <= 4)    // Range 0-4
    {
        srPeriodicity = 5;
        srSubframeOffset = ue_p->srConfigurationIndex;
    }
    else if (ue_p->srConfigurationIndex >= 5 && ue_p->srConfigurationIndex <= 14)
    {
        srPeriodicity = 10;
        srSubframeOffset = ue_p->srConfigurationIndex - 5;
    }
    else if (ue_p->srConfigurationIndex >= 15 && ue_p->srConfigurationIndex <= 34)
    {
        srPeriodicity = 20;
        srSubframeOffset = ue_p->srConfigurationIndex - 15;
    }
    else if (ue_p->srConfigurationIndex >= 35 && ue_p->srConfigurationIndex <= 74)
    {
        srPeriodicity = 40;
        srSubframeOffset = ue_p->srConfigurationIndex - 35;
    }
    else if (ue_p->srConfigurationIndex >= 75 && ue_p->srConfigurationIndex <= 154)
    {
        srPeriodicity = 80;
        srSubframeOffset = ue_p->srConfigurationIndex - 75;
    }
    else if (ue_p->srConfigurationIndex >= 155 && ue_p->srConfigurationIndex <= 156)
    {
        srPeriodicity = 2;
        srSubframeOffset = ue_p->srConfigurationIndex - 155;
    }
    else if (ue_p->srConfigurationIndex == 157)
    {
        srPeriodicity = 1;
        srSubframeOffset = ue_p->srConfigurationIndex - 157;    // is 0 :-)
    }
    else
    {
        ltePanic("Invalid value SR config index (%d) in UE (CRNTI: %d)",
            ue_p->srConfigurationIndex, ue_p->crnti);
    }

    // 10 x nf + ceil(ns/2) - NoffsetSr mod SRperiodicity == 0 should hold true if SRI is to be sent
    // nf = SFN, ns = slot number (= 2 x SF)

    while (TRUE)
    {
        /* CA TDD Changes Start */
        if (IS_UL_SUBFRAME(sf, cellIndex)
        /* CA TDD Changes End */
            && ((MAX_SUBFRAME * sfn + sf - srSubframeOffset) % srPeriodicity == 0))
        {
            break;  // We found a suitable SFN.SF for sending an SRI
        }

        sf++;
        if (sf == MAX_SUBFRAME)
        {
            sfn++;
            sf = 0;
        }
        if (sfn == MAX_SFN)
        {
            sfn = 0;
        }
    }

    // sfn.sf is the subframe when the SRI can be sent
    ue_p->lastSRISchedule.sfn = sfn;
    ue_p->lastSRISchedule.sf = sf;
    ue_p->lastSRISchedule.isValid = TRUE;
    ue_p->srPeriodicity = srPeriodicity;
    ue_p->srSubframeOffset = srSubframeOffset;

    fprintf(stdout, "SR opportunity available at SFN.SF = %d.%d\n", sfn, sf);

    if (sendSFN_p != NULL)
    {
        *sendSFN_p = sfn;
    }
    if (sendSF_p != NULL)
    {
        *sendSF_p = sf;
    }
}

/*****************************************************************************
 * Function Name  : getNextRachOpportunity
 * Inputs         :
 * Outputs        :
 * Returns        : MAX_SUCCESS if a Rach opportunity was found, MAX_FAILURE
 *                  otherwise
 * Description    : Finds the next available RACH opportunity in TDD mode.
 *                  Refer TS 36.211, section 5.7.1.  Does not take into
 *                  account fRA (freq resource index)
 *****************************************************************************/
MacRetType getNextRachOpportunity(UInt16 *sfn_p, UInt8 *sf_p, RachResLocationQuad *rachLoc_p, UInt8 cellIndex)
{
    MacRetType result = MAC_FAILURE;

    /* CA TDD Changes Start */
    tddUlDlConfig_g[cellIndex] = cellConfigUeSim_g[cellIndex].subFrameAssignment;
    /* CA TDD Changes End */
    const RachResLocationQuad *rachLocList_p = getRachLocationSet(cellIndex);
    const TddRachConfigInfo *rachConfigInfo_p = &TddRachConfigInfo_gc[cellConfigUeSim_g[cellIndex].prachConfigIndex];

    if (NULL != rachLocList_p)
    {
        UInt8 quad;
        UInt16 sfn = SFN_g[cellIndex];
        UInt8 sf = SF_g[cellIndex];
        UInt32 tick = 0;
        UInt32 eligibleTick = 0;
        UInt8 eligibleSF = 0;
        tick = sfn * MAX_SUBFRAME + sf;
        eligibleTick = sfn * MAX_SUBFRAME;
        
        do
        {
            for (quad = 0; quad < rachConfigInfo_p->prachDensity; quad++)
            {
                /* SPR 11297 changes start */
                if( SSF != rachLocList_p[quad].t2ra )
                {
                    /* SPR 11463 chnages start */
                    eligibleSF = (MAX_HALF_SUBFRAME * rachLocList_p[quad].t1ra +
                            SF_UL_BASE + rachLocList_p[quad].t2ra);
                    if( 1 == rachConfigInfo_p->preambleFormat ||
                            2 == rachConfigInfo_p->preambleFormat )
                    {
                        /* Preamble format 1 and 2 takes 2 SFs for RACH to reach
                         * eNB, hence adding 1 to eligibleSF */
                        eligibleSF += 1;
                    }
                    else if( 3 == rachConfigInfo_p->preambleFormat )
                    {
                        /* Preamble format 3 takes 3 SFs for RACH to reach
                         * eNB, hence adding 2 to eligibleSF */
                        eligibleSF += 2;
                    }
                    /* SPR 11463 chnages end */
                }
                else
                {
                    eligibleSF = MAX_HALF_SUBFRAME * rachLocList_p[quad].t1ra +
                        SF_S_BASE;
                }
                /* SPR 11297 changes end */
                /* SPR 4843 changes start */
                if(eligibleSF <= sf)
                {
                    UpdateTick(&eligibleTick, MAX_SUBFRAME);    // Add 10
                }
                /* SPR 4843 changes end */

                UpdateTick(&eligibleTick, eligibleSF);      // Add eligibleSF

                if ((((eligibleTick / MAX_SUBFRAME) & 0x01) + 1) != rachLocList_p[quad].t0ra)
                {
                    UpdateTick(&eligibleTick, MAX_SUBFRAME);    // Add 10
                }

                if (eligibleTick >= tick)
                {
                    *sfn_p = (eligibleTick / MAX_SUBFRAME)%(MAX_SFN_VALUE);
                    *sf_p = eligibleTick % MAX_SUBFRAME;
                    *rachLoc_p = rachLocList_p[quad];
                    result = MAC_SUCCESS;
                    break;
                }
            }
            eligibleTick += MAX_SUBFRAME;
        }
        while (eligibleTick < tick);
    }
    return result;
}


/*****************************************************************************
 * Function Name  : initDlAckNackValue
 * Inputs         : dlNackPercent:gives the percentage if dlNack's need to set
 * Outputs        :
 * Returns        :
 * Description    : This function initializes dlAckNackValue_g with given
 *                  percentage of NACK
 *****************************************************************************/
void initDlAckNackValue(UInt32 noOfDlNacks)
{
#if 1
    UInt8 length = MAX_NO_TB * MAX_RADIO_FRAME_FOR_DL_ACK_NACK;

    switch (noOfDlNacks)
    {
        case ZERO_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] =
                {
                    {1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case ONE_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {1,0,1,1,1,1,1,1},
                    {1,1,1,1,1,0,1,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case TWO_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {1,1,0,1,1,1,0,1},
                    {0,1,1,1,0,1,1,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;
        case THREE_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {1,1,1,0,1,0,1,0},
                    {0,1,1,1,0,1,0,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case FOUR_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {1,0,1,0,1,0,1,0},
                    {0,1,0,1,0,1,0,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case FIVE_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {1,0,0,0,1,1,0,0},
                    {0,0,1,1,0,0,0,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case SIX_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {0,0,0,1,0,0,0,1},
                    {1,0,0,0,1,0,0,0}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case SEVEN_OF_EIGHT_DL_NACK:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {1,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,1}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        case ALL_DL_NACKS:
            {
                UInt8 dlAckNackValue_l[MAX_NO_TB][MAX_RADIO_FRAME_FOR_DL_ACK_NACK] = {
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}
                };
                memcpy (dlAckNackValue_g, dlAckNackValue_l, length);
            }
            break;

        default:
            ltePanic("Invalid input for number of DL Nack's",__func__);
            break;
    }
    #endif
}


/*****************************************************************************
 * Function Name  : getRachLocationArray
 * Inputs         : None
 * Outputs        : None
 * Returns        : Array (pointer) of quadruples that contain information
 *                  about RACH Location. NULL if none is found
 * Variables      : Uses globals tddUlDlConfig_g & cellConfig_g
 * Description    : Using the current UL/DL config and configured
 *                  prachConfigIndex returns an array containing possible RACH
 *                  location quadruples.
 *****************************************************************************/
const RachResLocationQuad *getRachLocationSet(UInt8 cellIndex)
{
    /* CA TDD Changes Start */
    const RachResLocationQuad *set_p = RachResLocationQuad_gc[tddUlDlConfig_g[cellIndex]][cellConfigUeSim_g[cellIndex].prachConfigIndex];
    /* CA TDD Changes End */

    return (NA == set_p->fra) ? NULL : set_p;
}

/*TDD Config 0 and 6 Changes Start*/
/*****************************************************************************
 * Function Name  : getUlSubFrameForHIPdu
 * Inputs         : enbSF
 * 		    iPhich
 * Outputs        : None
 * Returns        : Returns the UL Subframe offset for HI Pdu 
 * Variables      : ulSfForDCI0OrHiReceived_g 
 * Description    : This function returns the UL Subframe offset for HI Pdu
 * 		    based on the iPhich value.
 *****************************************************************************/
/* CA TDD Changes Start */
UInt8 getUlSubFrameForHIPdu(UInt8 enbSF,UInt8 iPhich,UInt8 cellIndex)
{
    UInt8 offset = 0;
    if ((tddUlDlConfig_g[cellIndex] == 0) && ((iPhich == 1) || (IS_SPL_SUBFRAME(enbSF, cellIndex))))
    {
	/* For TDD Config 0, set offset = 7, when, either iPHICH = 1, when 
 	 * PUSCH transmission associated to this HI was received in SF 4 or 9
 	 * or enbSF is a special subframe.
 	 */
        offset = 7;
    }
    else
    {
        offset = ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g[cellIndex]][enbSF];
    }
    return offset;
}

/*****************************************************************************
 * Function Name  : getUlSubFrameForDCI0Pdu
 * Inputs         : enbSF
 *                  ulIndex
 * Outputs        : None
 * Returns        : Returns the UL Subframe offset for HI Pdu
 * Variables      : ulSfForDCI0OrHiReceived_g
 * Description    : This function returns the UL Subframe offset for DCI Pdu
 *                  based on the ulIndex value.
 *****************************************************************************/
UInt8 getUlSubFrameForDCI0Pdu(UInt8 enbSF,UInt8 ulIndex,UInt8 cellIndex)
{
    UInt8 offset = 0;
    if ((tddUlDlConfig_g[cellIndex] == 0) && (ulIndex == 1))
    {
        offset = 7;
    }
    else
    {
        offset = ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g[cellIndex]][enbSF];
    }
    return offset;
}
/* CA TDD Changes End */
/*TDD Config 0 and 6 Changes End*/
/* + SPS_TDD_Changes */
/****************************************************************************
 *Function Name  : getSubFrameOffsetForSpsTDD
 *Inputs         : subFrameNum
 *Outputs        : subframeOffset
 *Description    : This function will return the subFrame offset corresponding to
 current TDD Config
 ****************************************************************************/
inline SInt8 getSubFrameOffsetForSpsTDD (UInt8 subFrameNum, UInt8 cellIndex)
{
    return(subFrameOffsetForSps[cellConfigUeSim_g[cellIndex].subFrameAssignment][subFrameNum]);
}

/*****************************************************************************
 * Function Name  : getSubframeType
 * Inputs         : subframe - The UE Index for which the HARQ process is required.
 * Returns        : return Subframe Type - DL,UL,SP.
 * Description    : This function retrive the Subframe Type from structure
 *                  ulGrantPhichTableInfo_g for current UL-DL config and
 *                  input subframe number and return the same
 *****************************************************************************/
inline ULDLSPFrame getPrevSubFrameforDCI0( UInt8 ulSubframe, UInt8 cellIndex)
{
    return ulGrantPhichTableInfo_sim_g[cellConfigUeSim_g[cellIndex].subFrameAssignment][ulSubframe].prevRecordForUL.prevSubFrameforDCI_O_From_currentSubFrame;
}

/* - SPS_TDD_Changes */
#endif

