/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: snow3g.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Functions for snow3g algo 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: snow3g.c,v $
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
#include "snow3g.h"

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

/* Rijndael S-box SR */
static UInt8 SR[256] = {
    0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
    0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
    0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
    0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
    0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
    0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
    0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
    0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
    0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
    0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
    0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
    0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
    0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
    0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
    0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
    0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};
/* S-box SQ */
static UInt8 SQ[256] = {
    0x25,0x24,0x73,0x67,0xD7,0xAE,0x5C,0x30,0xA4,0xEE,0x6E,0xCB,0x7D,0xB5,0x82,0xDB,
    0xE4,0x8E,0x48,0x49,0x4F,0x5D,0x6A,0x78,0x70,0x88,0xE8,0x5F,0x5E,0x84,0x65,0xE2,
    0xD8,0xE9,0xCC,0xED,0x40,0x2F,0x11,0x28,0x57,0xD2,0xAC,0xE3,0x4A,0x15,0x1B,0xB9,
    0xB2,0x80,0x85,0xA6,0x2E,0x02,0x47,0x29,0x07,0x4B,0x0E,0xC1,0x51,0xAA,0x89,0xD4,
    0xCA,0x01,0x46,0xB3,0xEF,0xDD,0x44,0x7B,0xC2,0x7F,0xBE,0xC3,0x9F,0x20,0x4C,0x64,
    0x83,0xA2,0x68,0x42,0x13,0xB4,0x41,0xCD,0xBA,0xC6,0xBB,0x6D,0x4D,0x71,0x21,0xF4,
    0x8D,0xB0,0xE5,0x93,0xFE,0x8F,0xE6,0xCF,0x43,0x45,0x31,0x22,0x37,0x36,0x96,0xFA,
    0xBC,0x0F,0x08,0x52,0x1D,0x55,0x1A,0xC5,0x4E,0x23,0x69,0x7A,0x92,0xFF,0x5B,0x5A,
    0xEB,0x9A,0x1C,0xA9,0xD1,0x7E,0x0D,0xFC,0x50,0x8A,0xB6,0x62,0xF5,0x0A,0xF8,0xDC,
    0x03,0x3C,0x0C,0x39,0xF1,0xB8,0xF3,0x3D,0xF2,0xD5,0x97,0x66,0x81,0x32,0xA0,0x00,
    0x06,0xCE,0xF6,0xEA,0xB7,0x17,0xF7,0x8C,0x79,0xD6,0xA7,0xBF,0x8B,0x3F,0x1F,0x53,
    0x63,0x75,0x35,0x2C,0x60,0xFD,0x27,0xD3,0x94,0xA5,0x7C,0xA1,0x05,0x58,0x2D,0xBD,
    0xD9,0xC7,0xAF,0x6B,0x54,0x0B,0xE0,0x38,0x04,0xC8,0x9D,0xE7,0x14,0xB1,0x87,0x9C,
    0xDF,0x6F,0xF9,0xDA,0x2A,0xC4,0x59,0x16,0x74,0x91,0xAB,0x26,0x61,0x76,0x34,0x2B,
    0xAD,0x99,0xFB,0x72,0xEC,0x33,0x12,0xDE,0x98,0x3B,0xC0,0x9B,0x3E,0x18,0x10,0x3A,
    0x56,0xE1,0x77,0xC9,0x1E,0x9E,0x95,0xA3,0x90,0x19,0xA8,0x6C,0x09,0xD0,0xF0,0x86
};

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : MULx 
 * Inputs         : V, c - An 8-bit input
 * Outputs        : 
 * Returns        : An 8-bit Output
 * Description    : Mulx function 
 *****************************************************************************/
/* MULx.
 * Input V: an 8-bit input.
 * Input c: an 8-bit input.
 * Output : an 8-bit output.
 * See section 3.1.1 for details.
 */

static UInt8 MULx(UInt8 V, UInt8 c)
{
    if( 0 != (V&0x80) )
    {
        return ( (V << 1) ^ c);
    }
    else
    {
        return ( V << 1);
    }
}

/*****************************************************************************
 * Function Name  : MULxPOW 
 * Inputs         : V, c - An 8-bit input
 *                  i - A positive integer
 * Outputs        : 
 * Returns        : An 8-bit Output
 * Description    : Mulx with a power function 
 *****************************************************************************/
/* MULxPOW.
 * Input V: an 8-bit input.
 * Input i: a positive integer.
 * Input c: an 8-bit input.
 * Output : an 8-bit output.
 * See section 3.1.2 for details.
 */
static UInt8 MULxPOW(UInt8 V, UInt8 i, UInt8 c)
{
    if(0==i)
    {
        return V;
    }
    else
    {
        return MULx( MULxPOW( V, i-1, c ), c);
    }
}

/*****************************************************************************
 * Function Name  : MULalpha 
 * Inputs         : c - An 8-bit input
 * Outputs        : 
 * Returns        : An 32-bit Output
 * Description    : Function provides MUL alpha functionality 
 *****************************************************************************/
/* The function MUL alpha.
 * Input c: 8-bit input.
 * Output : 32-bit output.
 * See section 3.4.2 for details.
 */
static UInt32 MULalpha(UInt8 c)
{
    return ( ( ((UInt32)MULxPOW(c, 23, 0xa9)) << 24 ) |
            ( ((UInt32)MULxPOW(c, 245, 0xa9)) << 16 ) |
            ( ((UInt32)MULxPOW(c, 48, 0xa9)) << 8 ) |
            ( ((UInt32)MULxPOW(c, 239, 0xa9)) ) ) ;
}

/*****************************************************************************
 * Function Name  : DIValpha 
 * Inputs         : c - An 8-bit input
 * Outputs        : 
 * Returns        : An 32-bit Output
 * Description    : Function provides DIV alpha functionality 
 *****************************************************************************/
/* The function DIV alpha.
 * Input c: 8-bit input.
 * Output : 32-bit output.
 * See section 3.4.3 for details.
 */
static UInt32 DIValpha(UInt8 c)
{
    return ( ( ((UInt32)MULxPOW(c, 16, 0xa9)) << 24 ) |
            ( ((UInt32)MULxPOW(c, 39, 0xa9)) << 16 ) |
            ( ((UInt32)MULxPOW(c, 6, 0xa9)) << 8 ) |
            ( ((UInt32)MULxPOW(c, 64, 0xa9)) ) ) ;
}

/*****************************************************************************
 * Function Name  : S1 
 * Inputs         : w - An 32-bit input
 * Outputs        : 
 * Returns        : An 32-bit Output of S1 box
 * Description    : The 32x32-bit S-Box S1 
 *****************************************************************************/
/* The 32x32-bit S-Box S1
 * Input: a 32-bit input.
 * Output: a 32-bit output of S1 box.
 * See section 3.3.1.
 */
static UInt32 S1(UInt32 w)
{
    UInt8 r0=0, r1=0, r2=0, r3=0;
    UInt8 srw0 = SR[ (UInt8)((w >> 24) & 0xff) ];
    UInt8 srw1 = SR[ (UInt8)((w >> 16) & 0xff) ];
    UInt8 srw2 = SR[ (UInt8)((w >> 8) & 0xff) ];
    UInt8 srw3 = SR[ (UInt8)((w) & 0xff) ];
    r0 = ( ( MULx( srw0 , 0x1b) ) ^
            ( srw1 ) ^
            ( srw2 ) ^
            ( (MULx( srw3, 0x1b)) ^ srw3 )
         );
    r1 = ( ( ( MULx( srw0 , 0x1b) ) ^ srw0 ) ^
            ( MULx(srw1, 0x1b) ) ^
            ( srw2 ) ^
            ( srw3 )
         );
    r2 = ( ( srw0 ) ^
            ( ( MULx( srw1 , 0x1b) ) ^ srw1 ) ^
            ( MULx(srw2, 0x1b) ) ^
            ( srw3 )
         );
    r3 = ( ( srw0 ) ^
            ( srw1 ) ^
            ( ( MULx( srw2 , 0x1b) ) ^ srw2 ) ^
            ( MULx( srw3, 0x1b) )
         );
    return ( ( ((UInt32)r0) << 24 ) | ( ((UInt32)r1) << 16 ) | 
            ( ((UInt32)r2) << 8 ) | ( ((UInt32)r3) ) );
}

/*****************************************************************************
 * Function Name  : S2 
 * Inputs         : w - An 32-bit input
 * Outputs        : 
 * Returns        : An 32-bit Output of S2 box
 * Description    : The 32x32-bit S-Box S2 
 *****************************************************************************/
/* The 32x32-bit S-Box S2
 * Input: a 32-bit input.
 * Output: a 32-bit output of S2 box.
 * See section 3.3.2.
 */
static UInt32 S2(UInt32 w)
{
    UInt8 r0=0, r1=0, r2=0, r3=0;
    UInt8 sqw0 = SQ[ (UInt8)((w >> 24) & 0xff) ];
    UInt8 sqw1 = SQ[ (UInt8)((w >> 16) & 0xff) ];
    UInt8 sqw2 = SQ[ (UInt8)((w >> 8) & 0xff) ];
    UInt8 sqw3 = SQ[ (UInt8)((w) & 0xff) ];
    r0 = ( ( MULx( sqw0 , 0x69) ) ^
            ( sqw1 ) ^
            ( sqw2 ) ^
            ( (MULx( sqw3, 0x69)) ^ sqw3 )
         );
    r1 = ( ( ( MULx( sqw0 , 0x69) ) ^ sqw0 ) ^
            ( MULx(sqw1, 0x69) ) ^
            ( sqw2 ) ^
            ( sqw3 )
         );
    r2 = ( ( sqw0 ) ^
            ( ( MULx( sqw1 , 0x69) ) ^ sqw1 ) ^
            ( MULx(sqw2, 0x69) ) ^
            ( sqw3 )
         );
    r3 = ( ( sqw0 ) ^
            ( sqw1 ) ^
            ( ( MULx( sqw2 , 0x69) ) ^ sqw2 ) ^
            ( MULx( sqw3, 0x69) )
         );
    return ( ( ((UInt32)r0) << 24 ) | ( ((UInt32)r1) << 16 ) |
           ( ((UInt32)r2) << 8 ) | ( ((UInt32)r3) ) );
}

/*****************************************************************************
 * Function Name  : ClockLFSRInitializationMode 
 * Inputs         : F - a 32-bit word comes from output of FSM
 * Outputs        : 
 * Returns        : None
 * Description    : Clocking LFSR in initialization mode. LFSR Registers S0 
 *                  to S15 are updated as the LFSR receives a single clock. 
 *****************************************************************************/
/* Clocking LFSR in initialization mode.
 * LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
 * Input F: a 32-bit word comes from output of FSM.
 * See section 3.4.4.
 */
static void ClockLFSRInitializationMode(UInt32 F, Snow3gState *st)
{
    Int i = 0;	
    UInt32 v = ( ( (st->LFSR_S[0] << 8) & 0xffffff00 ) ^
            ( MULalpha( (UInt8)((st->LFSR_S[0]>>24) & 0xff) ) ) ^
            ( st->LFSR_S[2] ) ^
            ( (st->LFSR_S[11] >> 8) & 0x00ffffff ) ^
            ( DIValpha( (UInt8)( ( st->LFSR_S[11]) & 0xff ) ) ) ^
            ( F )
            );
    for(i=0;i<15;i++)
    {
        st->LFSR_S[i]=st->LFSR_S[i+1];
    }
    st->LFSR_S[15] = v;
}

/*****************************************************************************
 * Function Name  : ClockLFSRKeyStreamMode 
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : Clocking LFSR in keystream mode. LFSR Registers S0 
 *                  to S15 are updated as the LFSR receives a single clock. 
 *****************************************************************************/
/* Clocking LFSR in keystream mode.
 * LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
 * See section 3.4.5.
 */
static void ClockLFSRKeyStreamMode(Snow3gState *st)
{
    Int i=0;	
    UInt32 v = ( ( (st->LFSR_S[0] << 8) & 0xffffff00 ) ^
            ( MULalpha( (UInt8)((st->LFSR_S[0]>>24) & 0xff) ) ) ^
            ( st->LFSR_S[2] ) ^
            ( (st->LFSR_S[11] >> 8) & 0x00ffffff ) ^
            ( DIValpha( (UInt8)( ( st->LFSR_S[11]) & 0xff ) ) )
            );
    for(i=0;i<15;i++)
    {
        st->LFSR_S[i]=st->LFSR_S[i+1];
    }
    st->LFSR_S[15] = v;
}

/*****************************************************************************
 * Function Name  : ClockFSM 
 * Inputs         : 
 * Outputs        : 
 * Returns        : A 32 bit word F.
 * Description    : Produces a 32-bit word F. Updates FSM registers R1, R2, R3. 
 *****************************************************************************/
/* Clocking FSM.
 * Produces a 32-bit word F.
 * Updates FSM registers R1, R2, R3.
 * See Section 3.4.6.
 */
static UInt32 ClockFSM(Snow3gState *st)
{
    UInt32 F = ( ( st->LFSR_S[15] + st->FSM_R1 ) & 0xffffffff ) ^ st->FSM_R2 ;
    UInt32 r = ( st->FSM_R2 + ( st->FSM_R3 ^ st->LFSR_S[5] ) ) & 0xffffffff ;
    st->FSM_R3 = S2(st->FSM_R2);
    st->FSM_R2 = S1(st->FSM_R1);
    st->FSM_R1 = r;
    return F;
}

/*****************************************************************************
 * Function Name  : snow3g_Initialize 
 * Inputs         : k[4]- Four 32-bit words making up 128-bit key.
 *                  IV[4]- 4 32-bit words making 128-bit init variable.
 * Outputs        : 
 * Returns        : All the LFSRs and FSM are initialized for key generation.
 * Description    : Initialization of snow 3g. 
 *****************************************************************************/
/* Initialization.
 * Input k[4]: Four 32-bit words making up 128-bit key.
 * Input IV[4]: Four 32-bit words making 128-bit initialization variable.
 * Output: All the LFSRs and FSM are initialized for key generation.
 * See Section 4.1.
 */
void snow3g_Initialize(UInt32 k[4], UInt32 IV[4], Snow3gState *st)
{
    UInt8 i=0;
    UInt32 F = 0x0;
    st->LFSR_S[15] = k[3] ^ IV[0];
    st->LFSR_S[14] = k[2];
    st->LFSR_S[13] = k[1];
    st->LFSR_S[12] = k[0] ^ IV[1];
    st->LFSR_S[11] = k[3] ^ 0xffffffff;
    st->LFSR_S[10] = k[2] ^ 0xffffffff ^ IV[2];
    st->LFSR_S[9] = k[1] ^ 0xffffffff ^ IV[3];
    st->LFSR_S[8] = k[0] ^ 0xffffffff;
    st->LFSR_S[7] = k[3];
    st->LFSR_S[6] = k[2];
    st->LFSR_S[5] = k[1];
    st->LFSR_S[4] = k[0];
    st->LFSR_S[3] = k[3] ^ 0xffffffff;
    st->LFSR_S[2] = k[2] ^ 0xffffffff;
    st->LFSR_S[1] = k[1] ^ 0xffffffff;
    st->LFSR_S[0] = k[0] ^ 0xffffffff;
    st->FSM_R1 = 0x0;
    st->FSM_R2 = 0x0;
    st->FSM_R3 = 0x0;
    for(i=0;i<32;i++)
    {
        F = ClockFSM(st);
        ClockLFSRInitializationMode(F,st);
    }
}

/*****************************************************************************
 * Function Name  : snow3g_Initialize_UInt8 
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : same as snow3g_Initialize() but takes UInt8 arrays as input. 
 *****************************************************************************/
/*same as snow3g_Initialize() but takes Uint8 arrays as input*/
void snow3g_Initialize_UInt8(UInt8 u8k[16], UInt8 u8IV[16], Snow3gState *st)
{
    UInt32 k[4];
    UInt32 IV[4];
    Int n=0;

    for(n=0;n<4;n++)
    {
        k[n]=MakeUInt32(u8k+n*4);
    }

    for(n=0;n<4;n++)
    {
        IV[n]=MakeUInt32(u8IV+n*4);
    }

    snow3g_Initialize(k,IV,st);
}

/*****************************************************************************
 * Function Name  : snow3g_GenerateKeystream 
 * Inputs         : n - number of 32-bit words of keystream.
 *                  ks - space for the generated keystream, assumes memory
 *                       is allocated already.
 * Outputs        : 
 * Returns        : None
 * Description    : Generates a keystream and fill and returns. 
 *****************************************************************************/
/* Generation of Keystream.
 * input n: number of 32-bit words of keystream.
 * input z: space for the generated keystream, assumes
 * memory is allocated already.
 * output: generated keystream which is filled in z
 * See section 4.2.
 */
void snow3g_GenerateKeystream(UInt32 n, UInt32 *ks, Snow3gState *st)
{
    UInt32 t = 0;
    UInt32 F = 0x0;
    ClockFSM(st); /* Clock FSM once. Discard the output. */
    ClockLFSRKeyStreamMode(st); /* Clock LFSR in keystream mode once. */
    for(t=0;t<n;t++)
    {
        F = ClockFSM(st); /* STEP 1 */
        ks[t] = F ^ st->LFSR_S[0]; /* STEP 2 */
        /* Note that ks[t] corresponds to z_{t+1} in section 4.2 */
        ClockLFSRKeyStreamMode(st); /* STEP 3 */
    }
}
