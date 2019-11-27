/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: scrypt.h,v 1.1 2010/06/10 09:49:24 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions for AES algorithm 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: scrypt.h,v $
 * Revision 1.1  2010/06/10 09:49:24  gur21010
 * ciphering changes added for sync interface
 *
 *
 *
 ****************************************************************************/

#ifndef __ARICENT_SCRYPT__
#define __ARICENT_SCRYPT__

#include <lteTypes.h>
#include <lteLog.h>
#include <lteMemPool.h>
#include <lteMsgPool.h>
#include <lteMisc.h>
#include <ltePdcpCmnTools.h>

/** Constants ***************************************************************/
#define SCRYPT_DHRSA_MAXBYTES	1024

/** Type Definitions ********************************************************/
union key_u {
	struct aes {
		UInt32 rounds;
		UInt8 subkey[240];
	} aes;
};

struct blockcipher_s {
	UInt32 blocksize, keysize;
	void (*setup)( union key_u *, const UInt8 * );
	void (*encrypt)( const union key_u *, const UInt8 *, UInt8 * );
	void (*decrypt)( const union key_u *, const UInt8 *, UInt8 * );
};

struct __ctr_s {
	UInt32 n;
	UInt8 keystream[64];

	UInt32 countersize;
	UInt8 counter[64];
};

struct ctr_s {
	union key_u key;

	struct __ctr_s state;
};

/** Constant Global Data ****************************************************/
extern const struct blockcipher_s 
		blockcipher_aes128;

/** Public API **************************************************************/
void aes128_setup( union key_u *, const UInt8 * );
#define aes128_encrypt __aes_encrypt
#define aes128_decrypt __aes_decrypt

void __aes_encrypt( const union key_u *, const UInt8 *, UInt8 * );
void __aes_decrypt( const union key_u *, const UInt8 *, UInt8 * );

void ctr_start(struct ctr_s *, 
      const UInt8 *, 
      const UInt8 *, 
      UInt32 );
#define ctr_encrypt __ctr
#define ctr_decrypt __ctr

void __ctr( struct ctr_s *,  const UInt8 *, UInt8 *, UInt32 );

/** Private API *************************************************************/

void __core_ctr_start( const union key_u *U, 
        struct __ctr_s *Q, 
		const UInt8 *salt, 
        UInt32 countersize );
void __core_ctr( const union key_u *U, 
        struct __ctr_s *Q,  
		const UInt8 *input, 
        UInt8 *output, UInt32 L );

#endif
