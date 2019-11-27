/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: confidentiality.h,v 1.1 2010/06/10 09:42:44 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Confidentiality functions for encrypted/decrypted data. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
  *
 ****************************************************************************/

#ifndef _CONFIDENTIALITY_DEF_
#define _CONFIDENTIALITY_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "ltePdcpLteDefs.h"

#include <lteTypes.h>
#include <lteLog.h>
#include <lteMemPool.h>
#include <lteMsgPool.h>
#include <lteMisc.h>
#include <ltePdcpCmnTools.h>

#include "ltePdcpLteDefs.h"

/* SPR 15342 fix start */
/* SPR 15342 fix end */
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define CIPHER_MULTIPLE 4
#define SINGLE_SEGMENT  1

/* SPR_1205_FIX_START*/
#define NUM_BITS_IN_BYTES 8
/* SPR_1205_FIX_END*/

/* SPR_1210_FIX_START*/
#define CIPHERING_SUCCESS 1
#define CIPHERING_FAILURE 0
/* SPR_1210_FIX_END*/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : sendToCiphering
 * Inputs         : UInt16 AlgorithmId (ALGO_EEA1/ALGO_EEA2)
 *		    UInt16 lcId
 *		    UInt32 count
 *		    UInt16 direction - 0(downlink) or 1(uplink)
 *		    UInt8 *key_p - confidentiality key
 *		    void *data_p - Data buffer to perform deciphering.
 *                  UInt32 size - length of data in bytes.
 * Outputs        : UInt8 *data_p - encrypted data will be placed here
 * Returns        : SUCCESS/FAILURE
 * Description    : Encrypts/decrypts blocks of data.
 ****************************************************************************/
UInt32 sendToCiphering(
	UInt16 AlgorithmId, 
	UInt16 lcId, 
	UInt32 count,
        UInt16 direction, 
	UInt8 *key_p, 
 /* + SPR 18103 */
	UInt8 *inBuffP1_p,
 /* - SPR 18103 */
	UInt8 *inBuffP2_p,
	UInt8 *outBuff_p,
	UInt32 sizeOfP1,
	UInt32 sizeOfP2
    );

/****************************************************************************
 * Function Name  : sendToDeciphering
 * Inputs         : UInt16 AlgorithmId (ALGO_EEA1/ALGO_EEA2)
 *		    UInt16 lcId
 *		    UInt32 count
 *		    UInt16 direction - 0(downlink) or 1(uplink)
 *		    UInt8 *key_p - confidentiality key
 *		    void *data_p - Data buffer to perform deciphering.
 *                  UInt32 size - length of data in bytes.
 * Outputs        : UInt8 *data_p - decrypted data will be placed here
 * Returns        : SUCCESS/FAILURE
 * Description    : Encrypts/decrypts blocks of data.
 ****************************************************************************/
UInt32 sendToDeciphering(
	UInt16 AlgorithmId, 
	UInt16 lcId, 
	UInt32 count,
        UInt16 direction,
	UInt8 *key_p,
 /* + SPR 18103 */
	UInt8 *inBuffP1_p,
 /* - SPR 18103 */
	UInt8 *inBuffP2_p,
	UInt8 *outBuff_p,
	UInt32 sizeOfP1,
	UInt32 sizeOfP2
    );
#endif
