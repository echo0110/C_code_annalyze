/***************************************************************************
 *  FILE NAME:
 *      s_comd5.c
 *
 *  DESCRIPTION:
 *      Contains porting function related to md5digest module
 *
 *
 *  DATE     NAME              REF#   REASON
 *  -------  ----------------  -----  -------------------------------------
 *  27Mar09  sigtran team      -----  Original
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **************************************************************************/
#include <s_comd5.h>
#include <arMD5_api.h>

/****************************************************************************
 *    Function:       md5digest
 *
 *    Description:
 *                   This function returns md5digest Output for given
 *                   input (char *) buffer.
 *    Returns:       None
 ****************************************************************************/
void md5digest(
    unsigned char *buffer,
    unsigned long length,
    unsigned char *result )
{
    unArCryptoHash Hash_ctx;

    arMD5_start( &Hash_ctx );
    arMD5_update( &Hash_ctx, buffer, length );
    arMD5_finish( &Hash_ctx, result );
}

