/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: snow3g_optimized.h,v 1.1 2010/06/10 09:47:27 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions for snow3g algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: snow3g_optimized.h,v $
 * Revision 1.1  2010/06/10 09:47:27  gur21010
 * ciphering changes added for sync interface
 *
 *
 *
 ****************************************************************************/
#ifndef _SNOW3G_OPTIMIZED_DEF_
#define _SNOW3G_OPTIMIZED_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include <lteTypes.h>
#include <lteLog.h>
#include <lteMemPool.h>
#include <lteMsgPool.h>
#include <lteMisc.h>
#include <ltePdcpCmnTools.h>

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
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
void Initialize(UInt32 k[4], UInt32 IV[4]);

/* Generation of Keystream.
 * input n: number of 32-bit words of keystream.
 * input z: space for the generated keystream, assumes
 * memory is allocated already.
 * output: generated keystream which is filled in z
 * See section 4.2.
 */
void GenerateKeystream(UInt32 n, UInt32 *z);

#endif
