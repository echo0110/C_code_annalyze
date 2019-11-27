/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: snow3g_ca.h,v 1.1 2010/06/10 09:45:29 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions to calculate ciphering using snow3g (EEA1) algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: snow3g_ca.h,v $
 * Revision 1.1  2010/06/10 09:45:29  gur21010
 * ciphering changes added for sync interface
 *
 *
 *
 *
 ****************************************************************************/

#ifndef _SNOW3G_CA_DEF_
#define _SNOW3G_CA_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "confidentiality.h"

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

/****************************************************************************
 * Function Name  : snow3g_eea1
 * Inputs         : UInt8* key - 128 bit Confidentiality Key
 *		    UInt16 bearer - same as lcId
 *		    UInt32 count
 *		    UInt16 dir - direction, 0(downlink) or 1(uplink)
 *		    void *data - length number of bits, input bit stream.
 *		    UInt32 length - Number of bits to be encrypted or decrypted.
 * Outputs        : Output bit stream. Assumes data is suitably memory allocated.
 * Returns        : SUCCESS/FAILURE
 * Description    : Encrypts/decrypts blocks of data between 1 and 20000 bits 
 *                  in length as defined in Section 3.
 ****************************************************************************/
UInt32 snow3g_eea1( 
	UInt8 *key, 
	UInt32 count, 
	UInt16 bearer, 
	UInt16 dir, 
	UInt8 *inBuffP1_p, 
	UInt8 *inBuffP2_p, 
	UInt8 *outBuff_p, 
	UInt32 sizeOfP1,
	UInt32 sizeOfP2 );

#endif
