/***************************************************************************
 *  FILE NAME:
 *      s_cocrc32.c
 *
 *  DESCRIPTION:
 *      Contains porting function related to CRC checksum module
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
#include <arCRC_api.h>

/****************************************************************************
 *    Function:      generate_crc32c
 *
 *    Description:
 *                   This function returns CRC checksum value for
 *                   given input (char *) buffer.
 *    Returns:       Checksum
 ****************************************************************************/
unsigned int
generate_crc32c( unsigned char *buffer, unsigned int length )
{
    sctp_error_t  ecode;
    /* CSR 1-8128593 fix starts*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
    return arCRC_compute_chcksum( buffer, length, ( UINT4 * )&ecode );
#else
    return arCRC_compute_chcksum( buffer, length, &ecode );
#endif
    /* CSR 1-8128593 fix ends*/
}

/****************************************************************************
 *    Function:        sctp_compute_crc32_iovec
 *
 *    Description:
 *                   This function returns CRC checksum value for
 *                   given input iovec structure.
 *    Returns:       Checksum
 ****************************************************************************/
unsigned int
sctp_compute_crc32_iovec(
    sctp_iovec_st           *p_iovec,
    unsigned int           iov_len )
{
    sctp_error_t  ecode;
    /* CSR 1-8128593 fix starts*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
    return arCRC_compute_chcksum_iovec( ( IOVEC * )p_iovec, iov_len, ( UINT4 * )&ecode );
#else
    return arCRC_compute_chcksum_iovec( p_iovec, iov_len, &ecode );
#endif
    /* CSR 1-8128593 fix ends*/
}

