/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: aes_ca.h,v 1.1 2010/06/10 09:41:45 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions to calculate cipherign using AES (EEA2) 
 *                    algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 ****************************************************************************/

#ifndef _AES_CA_DEF_
#define _AES_CA_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include <lteLog.h>
#include <lteMemPool.h>
#include <lteMsgPool.h>
#include <lteMisc.h>
#include <ltePdcpCmnTools.h>

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define COUNTER_SIZE 16
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


/****************************************************************************
 * Function Name  : aes_eea2
 * Inputs         : UInt8* key - 128 bit Confidentiality Key
 *		    UInt32 count
 *		    UInt16 bearer - same as lcId
 *		    UInt16 direction - 0(downlink) or 1(uplink)
 *		    UInt8 *data - input bit stream.
 *                  UInt32 size - length of data in bytes.
 * Outputs        : Output bit stream. Assumes data is suitably memory allocated.
 * Returns        : void
 * Description    : Encrypts/Decrypts blocks of data.
 ****************************************************************************/
void aes_eea2(
     UInt8 *key,
     UInt32 count,
     UInt16 bearer, 
     UInt16 direction,
     UInt8 *inBuffP1_p,
     UInt8 *inBuffP2_p,
     UInt8 *outBuff_p,
     UInt32 sizeOfP1,
     UInt32 sizeOfP2);

#endif