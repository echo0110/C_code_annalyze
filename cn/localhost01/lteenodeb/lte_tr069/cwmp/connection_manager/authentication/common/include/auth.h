/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: auth.h$
 *
******************************************************************************/

/*******************************************************************************
 *
 *           STANDARD LIBRARY FILES
 *
*******************************************************************************/
#include <stdio.h>
#include <lteTypes.h>
/*********************************************************************************
 *                FUNCTION PROTOTYPES
 *
 ********************************************************************************/
int auth_for_digest(char *auth_str, char *auth[2]);
void auth_for_basic(char * buffer, char *auth[2]);

/*  SPR 17968 fix start */
void digest_auth_md5(UInt8* username, UInt8* realm, UInt8* nonce, UInt8* uri,
                     char *response, char *auth[2]);
/*  SPR 17968 fix end */

void user_pwd(char * buffer, char *auth[2]);
void ipr_cwmp_decoder(char * buffer, char *auth[2]);
int isbase64(char c);
int UnBase64(unsigned char *dest, const unsigned char *src, int srclen);
void auth_for_basic(char * buffer, char *auth[2]);
