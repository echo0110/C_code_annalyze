/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: cmac.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Functions for aes cmac algo 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: cmac.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:28:26  gur19140
 * *** empty log message ***
 *
 * Revision 1.3  2009/05/28 16:26:52  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/14 12:38:05  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "cmac.h"

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
static const UInt8 c_xor[4] = { 0x00, 0x87, 0x0e, 0x89 };

/****************************************************************************
 * Functions implementation 
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
void cmac_init( UInt8 *key, cmac_state *ctx )
    /* +- SPR 17777 */
{
    memSet(ctx, 0, sizeof(cmac_state));
    rijndaelSetupEncrypt(&ctx->aes, key, RIJNDAEL_KEYBITS);
}

/****************************************************************************
 * Function Name  : cmac_data
 * Inputs         : UInt8 *buf - data block to encrypt
 *					UInt32 len - length of data block (bytes)
 *					cmac_state *ctx - CMAC context
 * Outputs        : None
 * Returns        : None
 * Description    : Perform CMAC data encryption
 ****************************************************************************/
void cmac_data( UInt8 *buf, UInt32 len, cmac_state *ctx )
{   
    UInt32 cnt = 0;
    UInt32 b_pos = ctx->txt_cnt;
    Int i=0;

    if(0 == len)
    {
        return;
    }

    while((cnt < len) && (b_pos < CMAC_BLOCK_SIZE))
    {
        ctx->txt_cbc[b_pos++] ^= buf[cnt++];
    }

    while(cnt + CMAC_BLOCK_SIZE <= len)
    {
        rijndaelEncrypt(&ctx->aes, RIJNDAEL_NROUNDS(RIJNDAEL_KEYBITS), 
                         ctx->txt_cbc, ctx->txt_cbc);
        for(i=0; i<CMAC_BLOCK_SIZE; i++)
        {
            ctx->txt_cbc[i]^=buf[cnt++];
        }
    }

    while(cnt < len)
    {
        if(b_pos == CMAC_BLOCK_SIZE)
        {
            rijndaelEncrypt(&ctx->aes, RIJNDAEL_NROUNDS(RIJNDAEL_KEYBITS), 
                             ctx->txt_cbc, ctx->txt_cbc);
            b_pos = 0;
        }
        ctx->txt_cbc[b_pos++] ^= buf[cnt++];
    }

    ctx->txt_cnt = b_pos;
}

/****************************************************************************
 * Function Name  : gf_mulx
 * Inputs         : UInt8 pad[] - Pointer to pad array
 * Outputs        : None
 * Returns        : None
 * Description    : Perform pad
 ****************************************************************************/
static void gf_mulx(UInt8 pad[CMAC_BLOCK_SIZE])
{
    Int i=0;
    Int t = pad[0] >> 7;

    for(i = 0; i < CMAC_BLOCK_SIZE - 1; i++)
    {
        pad[i] = (pad[i] << 1) | (pad[i + 1] >> 7);
    }
    pad[CMAC_BLOCK_SIZE - 1] = (pad[CMAC_BLOCK_SIZE - 1] << 1) ^ c_xor[t];
}

/****************************************************************************
 * Function Name  : gf_mulx2
 * Inputs         : UInt8 pad[] - Pointer to pad array
 * Outputs        : None
 * Returns        : None
 * Description    : Perform pad
 ****************************************************************************/
static void gf_mulx2(UInt8 pad[CMAC_BLOCK_SIZE])
{
    Int i=0;
    Int t = pad[0] >> 6;

    for(i = 0; i < CMAC_BLOCK_SIZE - 1; i++)
    {
        pad[i] = (pad[i] << 2) | (pad[i + 1] >> 6);
    }
    pad[CMAC_BLOCK_SIZE - 2] ^= (t >> 1);
    pad[CMAC_BLOCK_SIZE - 1] = (pad[CMAC_BLOCK_SIZE - 1] << 2) ^ c_xor[t];
}

/****************************************************************************
 * Function Name  : cmac_end
 * Inputs         : UInt32 res_len - number of bytes to receive. <=16
 *					cmac_state *ctx - CMAC context
 * Outputs        : UInt8 *res - result of encryption
 * Returns        : None
 * Description    : Finish CMAC encryption
 ****************************************************************************/
void cmac_end( UInt8 *res, Int res_len, cmac_state *ctx )
{   
    UInt8 pad[CMAC_BLOCK_SIZE];
    Int	i=0;

    memSet(pad, 0, sizeof(pad));
    rijndaelEncrypt(&ctx->aes, RIJNDAEL_NROUNDS(RIJNDAEL_KEYBITS), pad, pad);
    if(ctx->txt_cnt < CMAC_BLOCK_SIZE)
    {
        ctx->txt_cbc[ctx->txt_cnt] ^= 0x80;
        gf_mulx2(pad);
    }
    else
    {
        gf_mulx(pad);
    }

    for(i=0; i<CMAC_BLOCK_SIZE; i++)
    {
        pad[i]^=ctx->txt_cbc[i];
    }

    rijndaelEncrypt(&ctx->aes, RIJNDAEL_NROUNDS(RIJNDAEL_KEYBITS), pad, pad);

    for(i = 0; i < res_len; i++)
    {
        res[i] = pad[i];
    }
}
