/***************************************************************************
 *  FILE NAME:
 *      s_coad32.c
 *
 *  DESCRIPTION:
 *      Contains porting function related to Adler checksum module
 *
 *
 *  DATE     NAME              REF#   REASON
 *  -------  ----------------  -----  -------------------------------------
 *  27Mar09  sigtran team      -----  Original
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **************************************************************************/
#include <s_cotyps.h>
#include <s_cocksm.h>
#include <arAdler_api.h>

/****************************************************************************
 *    Function:       compute_adler32_cksm
 *
 *    Description:
 *                   This function returns Adler checksum value for
 *                   given input (char *) buffer.
 *    Returns:       checksum
 ****************************************************************************/
unsigned int compute_adler32_cksm( unsigned char *buf, unsigned int len )
{
    sctp_error_t         ecode;
    /*windows fix*/
    /* CSR 1-8128593 fix starts*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
    return arAdler_compute_chcksum( buf, len, ( UINT4 * )&ecode );
#else
    return arAdler_compute_chcksum( buf, len, &ecode );
#endif
    /* CSR 1-8128593 fix ends*/
    /*windows fix*/

}

/****************************************************************************
 *    Function:         sctp_compute_adler32_iovec                               *
 *    Description:
 *                   This function returns Adler checksum value for
 *                   given input iovec structure.
 *    Returns:       checksum
 ****************************************************************************/
unsigned int
sctp_compute_adler32_iovec(
    sctp_iovec_st           *p_iovec,
    unsigned int           iov_len )
{
    sctp_error_t         ecode;
    /*windows fix*/
    /* CSR 1-8128593 fix starts*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
    return arAdler_compute_chcksum_iovec( ( IOVEC * ) p_iovec, iov_len, ( UINT4 * )&ecode );
#else
    return arAdler_compute_chcksum_iovec( p_iovec, iov_len, &ecode );
#endif
    /* CSR 1-8128593 fix ends*/
    /*windows fix*/

}

