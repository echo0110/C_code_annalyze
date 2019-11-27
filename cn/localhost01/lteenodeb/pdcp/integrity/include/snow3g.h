/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: snow3g.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Functions for snow3g algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: snow3g.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:28:26  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/14 12:38:01  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _SNOW3G_DEF_
#define _SNOW3G_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "in_lteFramework.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*state of snow3g*/
typedef struct Snow3gStateT
{
    /* LFSR */
    UInt32 LFSR_S[16];

    /* FSM */
    UInt32 FSM_R1;
    UInt32 FSM_R2;
    UInt32 FSM_R3;
} Snow3gState;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* Initialization.
 * Input k[4]: Four 32-bit words making up 128-bit key.
 * Input IV[4]: Four 32-bit words making 128-bit initialization variable.
 * Output: All the LFSRs and FSM are initialized for key generation.
 * See Section 4.1.
 */
void snow3g_Initialize(UInt32 k[4], UInt32 IV[4], Snow3gState *st);

/*same as snow3g_Initialize() but takes Uint8 arrays as input*/
void snow3g_Initialize_UInt8(UInt8 u8k[16], UInt8 u8IV[16], Snow3gState *st);

/* Generation of Keystream.
 * input n: number of 32-bit words of keystream.
 * input z: space for the generated keystream, assumes
 * memory is allocated already.
 * output: generated keystream which is filled in z
 * See section 4.2.
 */
void snow3g_GenerateKeystream(UInt32 n, UInt32 *z, Snow3gState *st);
#endif
