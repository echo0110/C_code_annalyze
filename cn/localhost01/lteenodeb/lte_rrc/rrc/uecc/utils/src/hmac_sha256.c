/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: hmac_sha256.c,v 1.3 2010/03/24 09:50:44 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : For HMAC-SHA-256 Algo
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: hmac_sha256.c,v $
 * Revision 1.3  2010/03/24 09:50:44  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.3  2009/12/26 12:10:00  gur21006
 * Warnings removed
 *
 * Revision 1.1.6.2  2009/12/05 20:54:21  ukr15916
 * HMAC-SHA256 refactored
 *
 * Revision 1.1.6.1  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.4  2009/11/24 11:32:30  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.3.2.2  2009/11/24 08:29:05  ukr15916
 * Reviewed
 *
 *
 ****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "hmac_sha256.h"

/******************************************************************************
 * Private defines
 *****************************************************************************/

#define SHA_256_RIGHT_ROTATE(val,bit_num)\
    (((val)>>(bit_num))|((val)<<((sizeof(val)*8)-(bit_num))))

#define U32_TO_BYTES(x,bytes)     \
{                                 \
    *(bytes) = (U8)((x)>>24);     \
    *((bytes)+1) = (U8)((x)>>16); \
    *((bytes)+2) = (U8)((x)>> 8); \
    *((bytes)+3) = (U8)(x);       \
}

#define BYTES_TO_U32(bytes,x)                        \
{                                                    \
    *(x)=((U32)*((bytes)+3))|((U32)*((bytes)+2)<<8)| \
        ((U32)*((bytes)+1)<<16)|((U32)*(bytes)<<24); \
}

#define INNER_PADDING_VAL 0x36
#define OUTER_PADDING_VAL 0x5c

/******************************************************************************
 * Local functions declaration
 *****************************************************************************/

static void sha_256_tr(sha_256_state *state,const U8 *msg,U32 chunk_num);

/******************************************************************************
 * Functions implementation
 *****************************************************************************/

/* SHA-256 functions */

/******************************************************************************
*   FUNCTION NAME: sha_256_tr
*
*   INPUT:         sha_256_state *state 
*                  const U8 *msg
*                  U32 chunk_num
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs SHA-256 transform according to
*                  SHA-256 pseudocode at
*                  http://en.wikipedia.org/wiki/SHA_hash_functions#SHA-2
*
******************************************************************************/
static void sha_256_tr(sha_256_state *state,const U8 *msg,U32 chunk_num)
{
    U32 w[64];
    U32 t1=RRC_NULL;
    U32 t2=RRC_NULL;
    U32 chunk=RRC_NULL;
    U32 i=RRC_NULL;

    U32 a=RRC_NULL;
    U32 b=RRC_NULL;
    U32 c=RRC_NULL;
    U32 d=RRC_NULL;
    U32 e=RRC_NULL;
    U32 f=RRC_NULL;
    U32 g=RRC_NULL;
    U32 h=RRC_NULL;

    static U32 k[64] =
    {
        0x428a2f98, 0x71374491,
        0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1,
        0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01,
        0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe,
        0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786,
        0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa,
        0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d,
        0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147,
        0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138,
        0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb,
        0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624,
        0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08,
        0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a,
        0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f,
        0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb,
        0xbef9a3f7, 0xc67178f2
    };

    RRC_ASSERT(PNULL!=state);
    RRC_ASSERT(PNULL!=msg);

    for (chunk=0;chunk<chunk_num;chunk++)
    {
        for (i=0;i<16;i++)
        {
            BYTES_TO_U32(msg+(chunk*SHA_256_CHUNK_LEN)+i*sizeof(U32),&w[i]);
        }

        for (i=16;i<64;i++)
        {
            w[i]=w[i-16]+
                ( SHA_256_RIGHT_ROTATE(w[i-15],7) ^
                    SHA_256_RIGHT_ROTATE(w[i-15],18) ^
                    (w[i-15] >> 3) ) /*s0*/+
                w[i-7]+
                ( SHA_256_RIGHT_ROTATE(w[i-2],17) ^
                    SHA_256_RIGHT_ROTATE(w[i-2],19) ^
                    (w[i-2] >> 10) ) /*s1*/;
        }

        a=state->h[0];
        b=state->h[1];
        c=state->h[2];
        d=state->h[3];
        e=state->h[4];
        f=state->h[5];
        g=state->h[6];
        h=state->h[7];

        for (i=0;i<64;i++)
        {
            t2=( SHA_256_RIGHT_ROTATE(a,2) ^ SHA_256_RIGHT_ROTATE(a,13) ^
                    SHA_256_RIGHT_ROTATE(a,22) ) /*s0*/+
                ((a&b)^(a&c)^(b&c)) /*maj*/;

            t1=h+
                ( SHA_256_RIGHT_ROTATE(e,6) ^ SHA_256_RIGHT_ROTATE(e,11) ^
                    SHA_256_RIGHT_ROTATE(e,25) ) /*s1*/+
                ((e&f)^(~e&g)) /*ch*/+
                k[i]+w[i];

            h=g;
            g=f;
            f=e;
            e=d+t1;
            d=c;
            c=b;
            b=a;
            a=t1+t2;
        }

        state->h[0]+=a;
        state->h[1]+=b;
        state->h[2]+=c;
        state->h[3]+=d;
        state->h[4]+=e;
        state->h[5]+=f;
        state->h[6]+=g;
        state->h[7]+=h;
    }
}

/******************************************************************************
*   FUNCTION NAME: sha_256_algo
*
*   INPUT:         const U8 *msg
*                  U32 length 
*                  U8 *digest
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function implements all SHA-256 algo functionality
*
******************************************************************************/
void sha_256_algo(const U8 *msg,U32 length,U8 *digest)
{
    sha_256_state state;

    RRC_ASSERT(PNULL!=msg);
    RRC_ASSERT(PNULL!=digest);

    sha_256_initialize(&state);
    sha_256_update_state(&state,msg,length);
    sha_256_finish(&state,digest);
}

/******************************************************************************
*   FUNCTION NAME: sha_256_initialize
*
*   INPUT:         sha_256_state *state
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs SHA-256 initialization
*
******************************************************************************/
void sha_256_initialize(sha_256_state *state)
{
    static U32 h0[8] =
    {
        0x6a09e667, 0xbb67ae85,
        0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c,
        0x1f83d9ab, 0x5be0cd19
    };

    RRC_ASSERT(PNULL!=state);

    RRC_ASSERT(sizeof(state->h)==sizeof(h0));
    l3_memcpy_wrapper(state->h,h0,sizeof(state->h));
    state->length = state->total_length = 0;
}

/******************************************************************************
*   FUNCTION NAME: sha_256_update_state
*
*   INPUT:         sha_256_state *state
*                  U8 *msg
*                  U32 length
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs SHA-256 state update
*
******************************************************************************/
void sha_256_update_state(sha_256_state *state,const U8 *msg,U32 length)
{
    U32 chunk_num=RRC_NULL;
    U32 rem_length=RRC_NULL;
    U32 tmp_length=RRC_NULL;

    RRC_ASSERT(PNULL!=state);
    RRC_ASSERT(PNULL!=msg);

    tmp_length=SHA_256_CHUNK_LEN - state->length;
    if (tmp_length<=length)
    {
        rem_length=tmp_length;
    }
    else
    {
        rem_length=length;
    }

    l3_memcpy_wrapper(state->chunk + state->length, msg, rem_length);

    if (state->length + length >= SHA_256_CHUNK_LEN)
    {
        chunk_num = (length - rem_length) / SHA_256_CHUNK_LEN;

        sha_256_tr(state,state->chunk,1);
        sha_256_tr(state,msg+rem_length,chunk_num);

        state->length = (length - rem_length) % SHA_256_CHUNK_LEN;

        l3_memcpy_wrapper(state->chunk,msg+rem_length+chunk_num*SHA_256_CHUNK_LEN,
            state->length);

        state->total_length += (chunk_num+1)*SHA_256_CHUNK_LEN;
    }
    else
    {
        state->length += length;
    }
}

/******************************************************************************
*   FUNCTION NAME: sha_256_finish
*
*   INPUT:         sha_256_state *state
*                  U8 *digest
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs SHA-256 finish iteration
*
******************************************************************************/
void sha_256_finish(sha_256_state *state,U8 *digest)
{
    U32 chunk_num=1;
    U32 pm_length=RRC_NULL;
    U32 length_val=RRC_NULL;
    U32 i=RRC_NULL;

    RRC_ASSERT(PNULL!=state);
    RRC_ASSERT(PNULL!=digest);

    if ((SHA_256_CHUNK_LEN-8)<=(state->length % SHA_256_CHUNK_LEN))
    {
        chunk_num=2;
    }

    pm_length = chunk_num*SHA_256_CHUNK_LEN;

    memset_wrapper(state->chunk + state->length,0,pm_length - state->length);
    state->chunk[state->length] = (1<<7);
    length_val = (state->total_length + state->length)*8;
    U32_TO_BYTES(length_val,state->chunk + pm_length - 4);

    sha_256_tr(state,state->chunk,chunk_num);

    for (i=0;i<8;i++)
    {
        U32_TO_BYTES(state->h[i],digest+i*sizeof(U32));
    }
}

/* HMAC part */

/******************************************************************************
*   FUNCTION NAME: hmac_sha_256_initialize
*
*   INPUT:         hmac_sha_256_state *state
*                  U8 *key 
*                  U32 key_length
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs HMAC-SHA-256 initialization
*
******************************************************************************/
void hmac_sha_256_initialize
(
    hmac_sha_256_state *state,
    U8 *key,
    U32 key_length
)
{
    U32 cur_length=SHA_256_CHUNK_LEN;
    U8 *cur_key=key;
    U8 key_temp[SHA_256_DIGEST_LEN];
    U32 i=RRC_NULL;

    RRC_ASSERT(PNULL!=state);
    RRC_ASSERT(PNULL!=key);

    if (SHA_256_CHUNK_LEN!=key_length)
    {
        if (SHA_256_CHUNK_LEN>key_length)
        {
            cur_length = key_length;
        }
        else
        {
            cur_length=SHA_256_DIGEST_LEN;
            cur_key = key_temp;
            sha_256_algo(key, key_length, cur_key);
        }
        /* klockwork Fix */
        if( cur_length < SHA_256_CHUNK_LEN)
        {
            memset_wrapper(state->inner_padding + cur_length, INNER_PADDING_VAL,
                SHA_256_CHUNK_LEN - cur_length);
            memset_wrapper(state->outer_padding + cur_length, OUTER_PADDING_VAL,
             SHA_256_CHUNK_LEN - cur_length);
        }
        /* klockwork Fix */
    }

    for (i=0;i<cur_length;i++)
    {
        state->inner_padding[i] = cur_key[i] ^ INNER_PADDING_VAL;
        state->outer_padding[i] = cur_key[i] ^ OUTER_PADDING_VAL;
    }

    sha_256_initialize(&state->state_inner);
    sha_256_update_state(&state->state_inner,state->inner_padding,
        SHA_256_CHUNK_LEN);

    sha_256_initialize(&state->state_outer);
    sha_256_update_state(&state->state_outer,state->outer_padding,
        SHA_256_CHUNK_LEN);

    l3_memcpy_wrapper(&state->state_inner_reinit,&state->state_inner,
        sizeof(sha_256_state));
    l3_memcpy_wrapper(&state->state_outer_reinit,&state->state_outer,
        sizeof(sha_256_state));
}

/******************************************************************************
*   FUNCTION NAME: hmac_sha_256_reinitialize
*
*   INPUT:         hmac_sha_256_state *state
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs HMAC-SHA-256 reinitialization
*
******************************************************************************/
void hmac_sha_256_reinitialize(hmac_sha_256_state *state)
{
    RRC_ASSERT(PNULL!=state);

    l3_memcpy_wrapper(&state->state_inner,&state->state_inner_reinit,
        sizeof(sha_256_state));
    l3_memcpy_wrapper(&state->state_outer,&state->state_outer_reinit,
        sizeof(sha_256_state));
}

/******************************************************************************
*   FUNCTION NAME: hmac_sha_256_update_state
*
*   INPUT:         hmac_sha_256_state *state
*                  U8 *msg 
*                  U32 msg_length
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs HMAC-SHA-256 state update
*
******************************************************************************/
void hmac_sha_256_update_state
(
    hmac_sha_256_state *state,
    U8 *msg,
    U32 msg_length
)
{
    RRC_ASSERT(PNULL!=state);
    RRC_ASSERT(PNULL!=msg);

    sha_256_update_state(&state->state_inner,msg,msg_length);
}

/******************************************************************************
*   FUNCTION NAME: hmac_sha_256_finish
*
*   INPUT:         hmac_sha_256_state *state
*                  U8 *mac
*                  U32 mac_length
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function performs HMAC-SHA-256 finish iteration
*
******************************************************************************/
void hmac_sha_256_finish
(
    hmac_sha_256_state *state,
    U8 *mac,
    U32 mac_length
)
{
    U8 digest_inner[SHA_256_DIGEST_LEN];
    /* CID 112742 Fix Start */
    U8 mac_buf[SHA_256_DIGEST_LEN] = {0};
    /* CID 112742 Fix End */ 
    RRC_ASSERT(PNULL!=state);
    RRC_ASSERT(PNULL!=mac);

    sha_256_finish(&state->state_inner,digest_inner);
    sha_256_update_state(&state->state_outer,digest_inner,SHA_256_DIGEST_LEN);
    sha_256_finish(&state->state_outer,mac_buf);
    l3_memcpy_wrapper(mac,mac_buf,mac_length);
}

/******************************************************************************
*   FUNCTION NAME: hmac_sha_256_algo
*
*   INPUT:         U8 *key
*                  U32 key_length
*                  U8 *msg
*                  U32 msg_length
*                  U8 *mac
*                  U32 mac_length
*
*
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function implements all HMAC-SHA-256 algo functionality
*
******************************************************************************/
void hmac_sha_256_algo
(
    U8 *key,
    U32 key_length,
    U8 *msg,
    U32 msg_length,
    U8 *mac,
    U32 mac_length
)
{
    hmac_sha_256_state state;

    RRC_ASSERT(PNULL!=key);
    RRC_ASSERT(PNULL!=msg);
    RRC_ASSERT(PNULL!=mac);

    hmac_sha_256_initialize(&state,key,key_length);
    hmac_sha_256_update_state(&state,msg,msg_length);
    hmac_sha_256_finish(&state,mac,mac_length);
}
