/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: snow3g_ca.c,v 1.1 2010/06/10 09:33:19 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions to calculate ciphering using snow3g (EEA1) algo 
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
#include "snow3g_ca.h"
#include "snow3g_optimized.h"

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
 * Function Name  : snow3g_eea1
 * Inputs         : UInt8* key - 128 bit Confidentiality Key
 *		    UInt16 bearer - same as lcId
 *		    UInt32 count
 *		    UInt16 dir - direction, 0(downlink) or 1(uplink)
 *		    void *data - length number of bits, input bit stream.
 *		    UInt32 length - Number of bits to be encrypted or decrypted.
 * Outputs        : Output bit stream. Assumes data is suitably memory allocated.
 * Returns        : None
 * Description    : Encrypts/decrypts blocks of data between 1 and 2^32 bits 
 *                  in length as defined in Section 3. The code is as per the 
 *                  specs defined in version 2.1 dated 16 March 2009. 
 *                  http://gsmworld.com/documents/uea2_uia2_d1_v2_1.pdf
 ****************************************************************************/
UInt32 snow3g_eea1( 
	UInt8 *key, 
	UInt32 count, 
	UInt16 bearer, 
	UInt16 dir, 
	UInt8 *inBuffP1_p, 
	UInt8 *inBuffP2_p,
	UInt8 *outBuff_p,
        UInt32 lenOfBuff1,
        UInt32 lenOfBuff2)
{
    UInt32 K[4],IV[4];
    UInt8 bitsInMultiple=1;
    UInt16 buffLen1 = lenOfBuff1;
    UInt32 n = ((lenOfBuff1*8)+(lenOfBuff2*8)+31)/32;
    /*+- SPR 20899*/
    lenOfBuff1 = DIVIDE_BY_FOUR(lenOfBuff1);
    /*+- SPR 20899*/
    bitsInMultiple =lenOfBuff1%4?0:1;
    if(lenOfBuff2){
        if(bitsInMultiple) 
    /*+- SPR 20899*/
            lenOfBuff2 = DIVIDE_BY_FOUR(lenOfBuff1);
    /*+- SPR 20899*/
        else
    /*+- SPR 20899*/
            lenOfBuff2 = DIVIDE_BY_FOUR(lenOfBuff2-1);
    /*+- SPR 20899*/
    }
    UInt16 i = 0;
    UInt32 val=0;
    UInt32 *KS;
    /*Initialisation*/
    /* Load the confidentiality key for SNOW 3G initialization as in section
	3.4. */
    /* SPR_1205_FIX_START */
    for (i=0; i<4; i++)
    {
        K[3-i] = (key[4*i] << 24) ^ (key[4*i+1] << 16) ^ (key[4*i+2] << 8) ^ (key[4*i+3]);
    }
    /* SPR_1205_FIX_END */

    /* Prepare the initialization vector (IV) for SNOW 3G initialization as in
       section 3.4. */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
    IV[1] = IV[3];
    IV[0] = IV[2];
    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS */
    Initialize (K, IV);

    /* SPR_1205_FIX_START */
    KS = (UInt32 *)getMemFromPool(4 * n, PNULL);
    /* SPR_1205_FIX_END */
    /* SPR_1210_FIX_START */
    if (PNULL == KS)
    {
        return CIPHERING_FAILURE;
    }
    /* SPR_1210_FIX_END*/

    GenerateKeystream(n, (UInt32 *)KS);
    
    /* Exclusive-OR the input data with keystream to generate the output bit
	stream */
    /* SPR_1205_FIX_START */
    for (i = 0; i<lenOfBuff1;i++)
    {
        outBuff_p[4*i+0] = (inBuffP1_p[4*i+0])^((UInt8) (KS[i] >> 24) & 0xff);
        outBuff_p[4*i+1] = (inBuffP1_p[4*i+1]) ^((UInt8) (KS[i] >> 16) & 0xff);
        outBuff_p[4*i+2] = (inBuffP1_p[4*i+2]) ^((UInt8) (KS[i] >> 8) & 0xff);
        outBuff_p[4*i+3] = (inBuffP1_p[4*i+3]) ^((UInt8) (KS[i] ) & 0xff);
    }
    if (lenOfBuff2){
        if (1 == bitsInMultiple)
        {
            bitsInMultiple = lenOfBuff2%4;
            for (;val<lenOfBuff2;i++,val++)
            {
                outBuff_p[4*i+0] = (inBuffP2_p[4*val+0])^((UInt8) (KS[i] >> 24) & 0xff);
                outBuff_p[4*i+1] = (inBuffP2_p[4*val+1]) ^((UInt8) (KS[i] >> 16) & 0xff);
                outBuff_p[4*i+2] = (inBuffP2_p[4*val+2]) ^((UInt8) (KS[i] >> 8) & 0xff);
                outBuff_p[4*i+3] = (inBuffP2_p[4*val+3]) ^((UInt8) (KS[i] ) & 0xff);
            }
        }
        else {
            if (3==(4-lenOfBuff1%4))
            {
                UInt8 idx=i;
                outBuff_p[4*i+0] = (inBuffP1_p[4*idx+0])^((UInt8) (KS[i] >> 24) & 0xff);
                outBuff_p[4*i+1] = (inBuffP1_p[4*idx+1]) ^((UInt8) (KS[i] >> 16) & 0xff);
                outBuff_p[4*i+2] = (inBuffP1_p[4*idx+2]) ^((UInt8) (KS[i] >> 8) & 0xff);
                outBuff_p[4*i+3] = (inBuffP2_p[0]) ^((UInt8) (KS[i] ) & 0xff);
                i++;
                for (;val<lenOfBuff2;i++,val++)
                {
                    outBuff_p[4*i+0] = (inBuffP2_p[4*val+1])^((UInt8) (KS[i] >> 24) & 0xff);
                    outBuff_p[4*i+1] = (inBuffP2_p[4*val+2]) ^((UInt8) (KS[i] >> 16) & 0xff);
                    outBuff_p[4*i+2] = (inBuffP2_p[4*val+3]) ^((UInt8) (KS[i] >> 8) & 0xff);
                    outBuff_p[4*i+3] = (inBuffP2_p[4*val+4]) ^((UInt8) (KS[i] ) & 0xff);
                }
            }
            else if (2==(4-lenOfBuff1%4))
            {
                UInt8 idx=i;
                outBuff_p[4*i+0] = (inBuffP1_p[4*idx+0]) ^ ((UInt8)(KS[i] >> 24)
                                                         & 0xff);
                outBuff_p[4*i+1] = (inBuffP1_p[4*idx+1])^((UInt8) (KS[i] >> 16) & 0xff);
                outBuff_p[4*i+2] = (inBuffP2_p[0]) ^((UInt8) (KS[i] >> 8) & 0xff);
                outBuff_p[4*i+3] = (inBuffP2_p[1]) ^((UInt8) (KS[i] ) & 0xff);
                i++;
                for (val=0;val<lenOfBuff2;i++,val++)
                {
                    outBuff_p[4*i+0] = (inBuffP2_p[4*val+2])^((UInt8) (KS[i] >> 24) & 0xff);
                    outBuff_p[4*i+1] = (inBuffP2_p[4*val+3])^((UInt8) (KS[i] >> 16) & 0xff);
                    outBuff_p[4*i+2] = (inBuffP2_p[4*val+4]) ^((UInt8) (KS[i] >> 8) & 0xff);
                    outBuff_p[4*i+3] = (inBuffP2_p[4*val+5]) ^((UInt8) (KS[i] ) & 0xff);
                }
            }
            else if (1==(4-lenOfBuff1%4))
            {
                outBuff_p[4*i+0] = (inBuffP1_p[4*i+0])^((UInt8) (KS[i] >> 24) & 0xff);
                outBuff_p[4*i+1] = (inBuffP2_p[0]) ^((UInt8) (KS[i] >> 16) & 0xff);
                outBuff_p[4*i+2] = (inBuffP2_p[1]) ^((UInt8) (KS[i] >> 8) & 0xff);
                outBuff_p[4*i+3] = (inBuffP2_p[2]) ^((UInt8) (KS[i] ) & 0xff);
                i++;
                for (val=0;val+4<lenOfBuff2;i++,val++)
                {
                    outBuff_p[4*i+0] = (inBuffP2_p[4*val+3])^((UInt8) (KS[i] >> 24) & 0xff);
                    outBuff_p[4*i+1] = (inBuffP2_p[4*val+4]) ^((UInt8) (KS[i] >> 16) & 0xff);
                    outBuff_p[4*i+2] = (inBuffP2_p[4*val+5]) ^((UInt8) (KS[i] >> 8) & 0xff);
                    outBuff_p[4*i+3] = (inBuffP2_p[4*val+6]) ^((UInt8) (KS[i] ) & 0xff);
                }
            }
        } 
    }
    if ((!bitsInMultiple)&&(!lenOfBuff2))
    {
        if (3==(buffLen1-i*4))
        {
            outBuff_p[4*i+0] = (inBuffP1_p[4*i+0]) ^((UInt8) (KS[i] >> 24) & 0xff);
            outBuff_p[4*i+1] = (inBuffP1_p[4*i+1]) ^((UInt8) (KS[i] >> 16) & 0xff);
            outBuff_p[4*i+2] = (inBuffP1_p[4*i+2]) ^((UInt8) (KS[i] >> 8) & 0xff);
        }
        else if (2==(buffLen1-i*4))
        {
            outBuff_p[4*i+0] = (inBuffP1_p[4*i+0]) ^((UInt8) (KS[i] >> 24) & 0xff);
            outBuff_p[4*i+1] = (inBuffP1_p[4*i+1]) ^((UInt8) (KS[i] >> 16) & 0xff);

        }
        else if (1==(buffLen1-i*4))
        {
            outBuff_p[4*i+0] = (inBuffP1_p[4*i+0]) ^((UInt8) (KS[i] >> 24) & 0xff);
        }
    }
    else if ((!bitsInMultiple)&&(lenOfBuff2)&& (i!=n))
    {
       if (3==(4-lenOfBuff2%4))
       {
           outBuff_p[4*i+0] = (inBuffP2_p[4*val+0]) ^((UInt8) (KS[i] >> 24) & 0xff);
           outBuff_p[4*i+1] = (inBuffP2_p[4*val+1]) ^((UInt8) (KS[i] >> 16) & 0xff);
           outBuff_p[4*i+2] = (inBuffP2_p[4*val+2]) ^((UInt8) (KS[i] >> 8) & 0xff);

       }
       else if (2==(4-lenOfBuff2%4))
       {
           outBuff_p[4*i+0] = (inBuffP2_p[4*val+0]) ^((UInt8) (KS[i] >> 24) & 0xff);
           outBuff_p[4*i+1] = (inBuffP2_p[4*val+1]) ^((UInt8) (KS[i] >> 16) & 0xff);
       }
       else if (1==(4-lenOfBuff2%4))
       {
           outBuff_p[4*i+0] = (inBuffP2_p[4*val+0]) ^((UInt8) (KS[i] >> 24) & 0xff);
       }
    }
    /* SPR_1205_FIX_END */
    freeMemPool(KS);
    /* SPR_1210_FIX_START*/
    return CIPHERING_SUCCESS;
    /* SPR_1210_FIX_END*/	
}
