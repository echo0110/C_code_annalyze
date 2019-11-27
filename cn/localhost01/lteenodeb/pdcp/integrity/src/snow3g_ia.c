/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: snow3g_ia.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 * File Description : Functions to calculate integrity using snow3g (EIA1) algo 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: snow3g_ia.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:28:26  gur19140
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
#include "snow3g_ia.h"
#include "snow3g.h"
#include "integrity.h"

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

/*****************************************************************************
 * Function Name  : MUL64x 
 * Inputs         : V, c - a 64-bit input.
 * Outputs        : 
 * Returns        : a 64-bit output.
 * Description    : A 64-bit Mulx function. 
 *****************************************************************************/
/* MUL64x.
 * Input V: a 64-bit input.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling
 * function.
 * See section 4.3.2 for details.
 */
static UInt64 MUL64x(UInt64 V, UInt64 c)
{
    if ( V & 0x8000000000000000LL )
    {
        return (V << 1) ^ c;
    }
    else
    {
        return V << 1;
    }
}

/*****************************************************************************
 * Function Name  : MUL64xPOW 
 * Inputs         : V, c - a 64-bit input.
 *                  i - A positive integer.
 * Outputs        : 
 * Returns        : a 64-bit output.
 * Description    : A 64-bit Mulx POW function. 
 *****************************************************************************/
/* MUL64xPOW.
 * Input V: a 64-bit input.
 * Input i: a positive integer.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling
 function.
 * See section 4.3.3 for details.
 */
static UInt64 MUL64xPOW(UInt64 V, UInt8 i, UInt64 c)
{
    if(0==i)
    {
        return V;
    }
    else
    {
        return MUL64x( MUL64xPOW(V,i-1,c) , c);
    }
}

/*****************************************************************************
 * Function Name  : MUL64 
 * Inputs         : V, P, c - a 64-bit input.
 * Outputs        : 
 * Returns        : a 64-bit output.
 * Description    : A 64-bit Mul function. 
 *****************************************************************************/
/* MUL64.
 * Input V: a 64-bit input.
 * Input P: a 64-bit input.
 * Input c: a 64-bit input.
 * Output : a 64-bit output.
 * A 64-bit memory is allocated which is to be freed by the calling
 * function.
 * See section 4.3.4 for details.
 */
static UInt64 MUL64(UInt64 V, UInt64 P, UInt64 c)
{
    UInt64 result=0;
    Int i=0;
    for(i=0;i<64;i++)
    {
        if( ( P>>i ) & 0x1 )
        {
            result ^= MUL64xPOW(V,i,c);
        }
    }
    return result;
}

/*****************************************************************************
 * Function Name  : mask32bit 
 * Inputs         : n - an integer in 1-32.
 * Outputs        : 
 * Returns        : a 32-bit mask.
 * Description    : Prepares a 32 bit mask with required number of 1 bits on
 *                  the MSB side. 
 *****************************************************************************/
/* mask32bit.
 * Input n: an integer in 1-32.
 * Output : a 32 bit mask.
 * Prepares a 32 bit mask with required number of 1 bits on the MSB side.
 */
static UInt32 mask32bit(Int n)
{
    UInt32 mask=0x0;
    if (0==n%32)
    {
        return 0xffffffff;
    }
    while(n--)
    {
        mask = (mask>>1) ^ 0x80000000;
    }
    return mask;
}

/****************************************************************************
 * Function Name  : snow3g_eia_ZCB
 * Inputs         : UInt8* key - 128 bit Integrity Key
 *					UInt16 bearer - same as lcId
 *					UInt32 count
 *					UInt16 dir - direction, 0(downlink) or 1(uplink)
 *					void *data_ZCB - ZCB with data to calc MACI for
 *					UInt32 byte_length - length of data in data_ZCB to calc MACI
 * Outputs        : UInt8 *res_MAC_I - calculated MACI will be placed here
 * Returns        : None
 * Description    : Calculates 32-bit MACI using snow3g (EIA1) algo
 ****************************************************************************/
void snow3g_eia_ZCB(UInt8* key,UInt16 bearer, UInt32 count, UInt16 dir,
        void *data_ZCB, UInt32 byte_length, UInt8 *res_MAC_I)
{
    Snow3gState state;
    UInt32 K[4],IV[4],z[5];
    UInt32 i,D;
    UInt32 MAC_I = 0;
    UInt64 EVAL;
    UInt64 V;
    UInt64 P;
    UInt64 Q;
    UInt64 c;
    UInt64 M_D_2;
    Int rem_bits;
    UInt32 mask = 0;
    UInt64 bit_length=((UInt64)byte_length)*8;
    ZCB_state ZCB_st;

    UInt32 fresh = (bearer&0x1F)<<27; 
    if(msgSize(data_ZCB,PNULL) < byte_length)
    {
        PDCP_LOG(LOG_DUMP,
                "Error in snow3g_eia_ZCB: length of data_ZCB < byte_length");
    }
    Init_ZCB_state(data_ZCB,&ZCB_st);

    dir &= 1; /*only last bit is used*/
    /* Load the Integrity Key for SNOW3G initialization as in section 4.4. */
    for(i=0;i<4;i++)
    {
        K[3-i] = MakeUInt32(key+i*4);
    }
    /* Prepare the Initialization Vector (IV) for SNOW3G initialization as in
       section 4.4. */
    IV[3] = count;
    IV[2] = fresh;
    IV[1] = count ^ ( dir << 31 ) ;
    IV[0] = fresh ^ (dir << 15);
    z[0] = z[1] = z[2] = z[3] = z[4] = 0;

    /* Run SNOW 3G to produce 5 keystream words z_1, z_2, z_3, z_4 and z_5. */
    snow3g_Initialize(K,IV,&state);
    snow3g_GenerateKeystream(5,z,&state);

    P = ((UInt64)z[0] << 32)|(UInt64)z[1];
    Q = ((UInt64)z[2] << 32)|(UInt64)z[3];

    /* Calculation */
    rem_bits = (Int)bit_length % 64;
    D = (UInt32)(bit_length/64 + 1);
    if(rem_bits!=0)
    {
        D++;
    }
    else
    {
        rem_bits = 64;
    }

    EVAL = 0;
    c = 0x1b;
    /* for 0 <= i <= D-3 */
    for (i=0;i<D-2;i++)
    {
        V = EVAL ^ ( (UInt64)GetNextUInt32_ZCB(&ZCB_st) << 32);
        V ^= (UInt64)GetNextUInt32_ZCB(&ZCB_st);
        EVAL = MUL64(V,P,c);
    }
    /* for D-2 */
    mask = mask32bit(rem_bits%32);
    if (rem_bits > 32)
    {
        M_D_2=((UInt64)GetNextUInt32_ZCB(&ZCB_st)<<32);
        M_D_2|=(UInt64)(GetNextUInt32_ZCB(&ZCB_st)&mask);
    }
    else
    {
        M_D_2=((UInt64)GetNextUInt32_ZCB(&ZCB_st)&mask) << 32 ;
    }
    V = EVAL ^ M_D_2;
    EVAL = MUL64(V,P,c);
    /* for D-1 */
    EVAL ^= bit_length;
    /* Multiply by Q */
    EVAL = MUL64(EVAL,Q,c);
    MAC_I = (UInt32)((EVAL >> 32) ^ z[4]);
    for(i=0;i<4;i++)
    {
        res_MAC_I[3-i]=(UInt8)(MAC_I&0xFF);
        MAC_I>>=8;
    }
}
