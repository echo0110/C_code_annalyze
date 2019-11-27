/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: aes_ca.c,v 1.1 2010/06/10 09:31:00 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions to calculate ciphering using AES (EEA1) algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 ****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "scrypt.h"
#include "aes_ca.h"

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

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : aes_eea2
 * Inputs         : UInt8* key - 128 bit Confidentiality Key
 *                  UInt16 bearer - same as lcId
 *                  UInt32 count
 *                  UInt16 direction - 0(downlink) or 1(uplink)
 *		    void *data -  input bit stream.
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
    UInt32 sizeOfP2) 
{
    /* SPR_1349_START_FIX */
    struct ctr_s ctx;
    /* SPR_1349_START_END */
    /* SPR_1205_START_FIX */
    UInt8 buf[COUNTER_SIZE] = {0}; 
    /* SPR_1205_START_END */
    
    direction &= 1;
             
    buf[0] = (UInt8)(count >> 24);
    buf[1] = (UInt8)(count >> 16);
    buf[2] = (UInt8)(count >> 8);
    buf[3] = (UInt8)count;
    /* SPR 1975 changes start */
    buf[4]=(UInt8)(((bearer & 0x1F)<<3) | ((direction & 0x01)<<2));
    /* SPR 1975 changes end */
    buf[5] = buf[6] = buf[7] = 0;

    /* Remaining 64 bits are already set to 0 when initializing buf array*/

    /* Initializes cipher in ctr mode */
    ctr_start(&ctx, key, buf, COUNTER_SIZE);
    ctr_encrypt (&ctx,inBuffP1_p,outBuff_p,sizeOfP1);
    if (sizeOfP2){
        ctr_encrypt (&ctx,inBuffP2_p,&outBuff_p[sizeOfP1],sizeOfP2);
    }
}

