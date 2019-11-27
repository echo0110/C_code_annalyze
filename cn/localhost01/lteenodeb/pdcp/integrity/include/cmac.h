/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: cmac.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Functions for aes cmac algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: cmac.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:28:26  gur19140
 * *** empty log message ***
 *
 * Revision 1.3  2009/05/28 16:26:48  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/14 12:38:01  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _CMAC_AES_DEF
#define _CMAC_AES_DEF


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rijndael.h"
#include "in_lteFramework.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define CMAC_BLOCK_SIZE 16

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* cmac state */
typedef struct
{
    UInt8			txt_cbc[CMAC_BLOCK_SIZE];
    UInt32          txt_cnt;
    rijndael_state	aes;         /* AES encryption context */
} cmac_state;

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
 * Function Name  : cmac_init
 * Inputs         : UInt8 *key - encryption key
 *					UInt32 key_len - key length (bytes)
 * Outputs        : cmac_state *ctx - CMAC context
 * Returns        : None
 * Description    : Init CMAC encryption context based on key
 ****************************************************************************/
/* +- SPR 17777 */
void cmac_init( UInt8 *key,cmac_state *ctx );
/* +- SPR 17777 */

/****************************************************************************
 * Function Name  : cmac_data
 * Inputs         : UInt8 *buf - data block to encrypt
 *					UInt32 len - length of data block (bytes)
 *					cmac_state *ctx - CMAC context
 * Outputs        : None
 * Returns        : None
 * Description    : Perform CMAC data encryption
 ****************************************************************************/
void cmac_data( UInt8 *buf, UInt32 len, cmac_state *ctx );

/****************************************************************************
 * Function Name  : cmac_end
 * Inputs         : UInt32 res_len - number of bytes to receive. <=16
 *					cmac_state *ctx - CMAC context
 * Outputs        : UInt8 *res - result of encryption
 * Returns        : None
 * Description    : Finish CMAC encryption
 ****************************************************************************/
void cmac_end( UInt8 *res, Int res_len, cmac_state *ctx );

#endif
