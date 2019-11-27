/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: hmac_sha256.h,v 1.3 2010/03/24 09:50:44 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : For HMAC-SHA-256 Algo
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: hmac_sha256.h,v $
 * Revision 1.3  2010/03/24 09:50:44  gur18569
 * Merged files from Rel 1.0 support branch
 *
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.3  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.6.2  2009/12/05 20:54:21  ukr15916
 * HMAC-SHA256 refactored
 *
 * Revision 1.1.6.1  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.3  2009/11/24 11:32:30  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.2.2.1  2009/11/24 08:29:05  ukr15916
 * Reviewed
 *
 *
 ****************************************************************************/

#ifndef HMAC_SHA256_H
#define HMAC_SHA256_H

/****************************************************************************
 * Inculdes
 ****************************************************************************/

#include "rrc_common_utils.h"

/****************************************************************************
 * Public defines
 ****************************************************************************/

#define SHA_256_DIGEST_LEN 32
#define SHA_256_CHUNK_LEN  64

/****************************************************************************
 * Public types definitions
 ****************************************************************************/

typedef struct
{
    U32 length;
    U32 total_length;
    U8  chunk[2*SHA_256_CHUNK_LEN];
    U32 h[8];
} sha_256_state;

typedef struct
{
    U8 inner_padding[SHA_256_CHUNK_LEN];
    U8 outer_padding[SHA_256_CHUNK_LEN];

    sha_256_state state_inner;
    sha_256_state state_outer;

    /* for reinit */
    sha_256_state state_inner_reinit;
    sha_256_state state_outer_reinit;
} hmac_sha_256_state;

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

void sha_256_algo(const U8 *msg,U32 length,U8 *digest);
void sha_256_initialize(sha_256_state *state);
void sha_256_update_state(sha_256_state *state,const U8 *msg,U32 length);
void sha_256_finish(sha_256_state *state,U8 *digest);

void hmac_sha_256_initialize
(
    hmac_sha_256_state *state,
    U8 *key,
    U32 key_length
);

void hmac_sha_256_reinitialize(hmac_sha_256_state *state);

void hmac_sha_256_update_state
(
    hmac_sha_256_state *state,
    U8 *msg,
    U32 msg_length
);

void hmac_sha_256_finish
(
    hmac_sha_256_state *state,
    U8 *mac,
    U32 mac_length
);

void hmac_sha_256_algo
(
    U8 *key,
    U32 key_length,
    U8 *msg,
    U32 msg_length,
    U8 *mac,
    U32 mac_length
);

#endif /* HMAC_SHA256_H */
