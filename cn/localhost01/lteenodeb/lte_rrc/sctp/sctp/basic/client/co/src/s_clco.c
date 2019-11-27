/*****************************************************************************
 *  FILE NAME:   s_clco.c
 *
 *  DESCRIPTION:
 *   This file contains the functions which are common to both SU and SM clients
 *  of the SCTP stack.
 *
 *  DATE              NAME           REFERENCE    REASON
 *  -----------------------------------------------------
 *
 *  23March 2001     Vakul Garg      -           .Original Creation
 *  12Sep   2001     Yogesh Gahlaut  -            Rel 3.0 changes
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

/*  any restriction on send API, max buffer sise.. there should be .. more than
 *  4500 bytes not allowed
 */

#include <s_clco.h>

/***************************************************************************
 *    FUNCTION :
 *    sctp_build_api_header
 *
 *    DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *    API elements are passed to this function and it convert them into
 *    buffer format.
 *    RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_build_api_header(
    sctp_U8bit      *p_buffer,
    sctp_api_id_t   api_id,
    sctp_trans_id_t transaction_id,
    sctp_src_id_t   src_id,
    sctp_dest_id_t  dest_id )
{
    sctp_U16bit api_version;
    /* we call buffer functions    */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, api_id );

    api_version = SCTP_API_VERSION_NO;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, api_version );

    p_buffer = sctp_buffer_set_Ushort( p_buffer, src_id );
    p_buffer = sctp_buffer_set_Ushort( p_buffer, dest_id );

    p_buffer = sctp_buffer_set_Ulong( p_buffer, transaction_id );

    /* put a place holder for length field */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, 0 );

    return p_buffer;

}
