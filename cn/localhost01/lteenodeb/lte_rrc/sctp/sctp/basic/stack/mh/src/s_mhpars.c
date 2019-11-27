/*******************************************************************************
 *    FILE NAME:
 *    s_mhpars.c
 *
 *    DESCRIPTION:
 *    This file is part of message Handling module. It includes
 *    that parse the SCTP messages received from peer.
 *
 *    DATE               NAME        REFERENCE    REASON
 *    -----------------------------------------------------
 *    02June 2000     Sandeep Mahajan   -         .Original Creation
 *    23Sept 2000     Sandeep Mahajan             SPR No 476
 *    23Sept 2000     Sandeep Mahajan             SPR No 480
 *    13Oct  2000     hbhondwe         SPR472     Fix
 *    16Jan  2001     vagarg           SPR947     Fix
 *    16Jan  2001     vagarg           SPR948     Fix
 *    24Sep  2001     gsheoran          -         Rel 3.0
 *    20May  2002     ygahlaut        SPR 4040 4085 Fix
 *    06Jan  2003     mrajpal         SPR 5223    SPR Fix
 *    07JULY 2008    Shikha Kathpal   Rel 6.2.0 RFC Upgrade
 *    29JULY 2008     Raza Mohamed    Rel 6.2.0 RFC Upgrade
 *    24May  2010     Rajnesh         SPR 20411 Warning Fixed
 *    1May13          Pranav Sharma   SPR 21107   CSR 44034
 *    1July13         Rohan Pawa       SPR 21141  CSR 50452
 *    17July14   Pinky Sinha   Bug Id 13    CSR  84956
*     6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "MH"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_cobuf.h>
#include <s_dbtcb.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_mhpars.h>
#include <s_mhproc.h>
#include <s_cocksm.h>


static sctp_U8bit
sctp_convert_chunk_id(
    sctp_U8bit             chunk_id );


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_parameter_pad_bytes
 *
 *    DESCRIPTION: This function is used to get the parameter and add padding
 *
 *    RETURN VALUE:sctp_U16bit
 *
 *************************************************************************/
sctp_U16bit
sctp_get_parameter_pad_bytes(
    sctp_U16bit    parameter_len )
{
    if( parameter_len % 4 != 0 )
    {
        return ( 4 - parameter_len % 4 );
    }

    else
    {
        return 0;
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_static_chk_init_paramid
 *
 *    DESCRIPTION:
 *        This function will check for valid optional parmeter in INIT
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_static_chk_init_paramid(
    sctp_U16bit    parameter_id,
    sctp_error_t   *p_ecode )
{
    sctp_return_t ret = SCTP_FAILURE;

    switch( parameter_id )
    {
            /*
            * TLV's have relevence within the Chunk type,
            * compare with all Init TLV we know .
                */
            /* VALID INIT PARAMS */
        case SCTPVARPAR_IPV4ADDRESS:
        case SCTPVARPAR_IPV6ADDRESS:
        case SCTPVARPAR_COOKIEPRESV:
        case SCTPVARPAR_ECNCAPABLE:
        case SCTPVARPAR_HOSTNAME:
        case SCTPVARPAR_SUPADDRTYPE:
        case SCTPVARPAR_ADAPTIONIND:
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        case SCTP_ASCONF_PARAM_SET_PRIM:
#endif
            *p_ecode = E_NOERROR;
            ret = SCTP_SUCCESS;
            break;

            /* VALID SCTP PARAMS BUT NOT OF INIT */
        case SCTPVARPAR_COOKIE:
        case SCTPVARPAR_UNKNOWNPAR:
        case SCTPVARPAR_HBINFO:
            *p_ecode = E_UNKNOWN_PARAMETER_INIT_RETURN_ERROR;
            break;

        default:
            /* Unrecognized Parameter Received */
            {
                switch( parameter_id >> 14 )
                {
                    case 0:
                        /* stop processing chunk. */
                        *p_ecode = E_UNKNOWN_PARAMETER_ID;
                        break;

                    case 1:
                        /* stop processing chunk and report the unrecognized
                           parameter in an 'Unrecognized Parameter'. */
                        *p_ecode = E_UNKNOWN_PARAMETER_RETURN_ERROR;
                        break;

                    case 2:
                        /* skip this parameter */
                        *p_ecode = E_SKIP_PARAMETER;
                        break;

                    case 3:
                        /* Skip this parameter and continue processing
                         * but report the unrecognized parameter in an
                         * 'Unrecognized Parameter'*/
                        *p_ecode = E_SKIP_PARAMETER_RETURN_ERROR;
                        break;
                }

                break;
            }
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_static_chk_initack_paramid
 *
 *    DESCRIPTION:
 *        This function will check for valid optional parmeter in    INITACK
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_static_chk_initack_paramid(
    sctp_U16bit    parameter_id,
    sctp_error_t   *p_ecode )
{
    sctp_return_t ret = SCTP_FAILURE;

    switch( parameter_id )
    {
            /*
             * TLV's have relevence within the Chunk type,
             * cmpare with all Init TLV we know
             */
        case SCTPVARPAR_IPV4ADDRESS:
        case SCTPVARPAR_IPV6ADDRESS:
        case SCTPVARPAR_ECNCAPABLE:
        case SCTPVARPAR_HOSTNAME:
        case SCTPVARPAR_COOKIE:
        case SCTPVARPAR_UNKNOWNPAR:
        case SCTPVARPAR_ADAPTIONIND:
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        case SCTP_ASCONF_PARAM_SET_PRIM:
#endif
            *p_ecode = E_NOERROR;
            ret = SCTP_SUCCESS;
            break;

            /* VALID SCTP PARAMS BUT NOT OF INIT ACK */
        case SCTPVARPAR_COOKIEPRESV:
        case SCTPVARPAR_SUPADDRTYPE:
        case SCTPVARPAR_HBINFO:
            *p_ecode = E_UNKNOWN_PARAMETER_INITACK_RETURN_ERROR;
            break;

        default:
            switch( parameter_id >> 14 )
            {
                case 0:
                    /* stop processing chunk and drop it */
                    *p_ecode = E_UNKNOWN_PARAMETER_ID;
                    break;

                case 1:
                    /* stop processing chunk and drop it */
                    *p_ecode = E_UNKNOWN_PARAMETER_RETURN_ERROR;
                    break;

                case 2:
                    /* skip this parameter */
                    *p_ecode = E_SKIP_PARAMETER;
                    break;

                case 3:
                    /* skip this parameter but return error */
                    *p_ecode = E_SKIP_PARAMETER_RETURN_ERROR;
                    break;
            }

            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_static_chk_init_paramlen
 *
 *    DESCRIPTION:
 *        This function will validate init optional parameters length.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_static_chk_init_paramlen(
    sctp_U16bit    parameter_id,
    sctp_U16bit    parameter_len )
{
    sctp_return_t ret = SCTP_SUCCESS;

    switch( parameter_id )
    {
        case SCTPVARPAR_IPV4ADDRESS:
            if( parameter_len != SCTPPARAM_LEN_IPV4ADDRESS )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_IPV6ADDRESS:
            if( parameter_len != SCTPPARAM_LEN_IPV6ADDRESS )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_COOKIEPRESV:
            if( parameter_len != SCTPPARAM_LEN_COOKIEPRESV )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_ECNCAPABLE:
            if( parameter_len != SCTPPARAM_LEN_ECNCAPABLE )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_HOSTNAME:
            if( parameter_len > SCTP_MAX_HOSTNAME_SIZE + SCTPPARAMOFFSET_BODY )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_SUPADDRTYPE:
            if( parameter_len
                    > SCTP_MAX_ADDR_TYPES * 2 + SCTPPARAMOFFSET_BODY )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_ADAPTIONIND:
            if( parameter_len != SCTPPARAM_LEN_ADAPTIONIND )
            {
                ret = SCTP_FAILURE;
            }

            break;
#ifdef SCTP_DYNAMIC_IP_SUPPORT

        case SCTP_ASCONF_PARAM_SET_PRIM:
            if( parameter_len < 16 )
            {
                ret = SCTP_FAILURE;
            }

            break;
#endif

        default:
            /* SPR 4085 */
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_static_chk_initack_paramlen
 *
 *    DESCRIPTION:
 *        This function will validate initack optional parameters    length.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_static_chk_initack_paramlen(
    sctp_U16bit    parameter_id,
    sctp_U16bit    parameter_len )
{
    sctp_return_t ret = SCTP_SUCCESS;

    switch( parameter_id )
    {
        case SCTPVARPAR_IPV4ADDRESS:
            if( parameter_len != SCTPPARAM_LEN_IPV4ADDRESS )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_IPV6ADDRESS:
            if( parameter_len != SCTPPARAM_LEN_IPV6ADDRESS )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_UNKNOWNPAR:
            if( parameter_len <  SCTPPARAMOFFSET_BODY )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_ECNCAPABLE:
            if( parameter_len != SCTPPARAM_LEN_ECNCAPABLE )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_HOSTNAME:
            if( parameter_len > SCTP_MAX_HOSTNAME_SIZE + SCTPPARAMOFFSET_BODY )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_COOKIE:
            if( parameter_len <  SCTPPARAMOFFSET_BODY )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPVARPAR_ADAPTIONIND:
            if( parameter_len != SCTPPARAM_LEN_ADAPTIONIND )
            {
                ret = SCTP_FAILURE;
            }

            break;
#ifdef SCTP_DYNAMIC_IP_SUPPORT

        case SCTP_ASCONF_PARAM_SET_PRIM:
            if( parameter_len < 16 )
            {
                ret = SCTP_FAILURE;
            }

            break;
#endif

        default:
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_init_opt_params
 *
 *    DESCRIPTION:
 *        This function will parse INIT chunk for all valid optional
 *        parameters and perform static checks on all optional parameters
 *        In-line functions
 *
 *        sctp_extract_param_id
 *        sctp_static_chk_opt_param_in_init
 *        sctp_extract_param_len
 *        sctp_static_chk_param_len
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_chk_init_opt_params(
    sctp_U8bit          *buffer,
    sctp_U16bit         chunk_offset,
    sctp_U16bit         chunk_len,
    sctp_chunk_init_st  *init,
    sctp_Boolean_t      *unrecognise_param_present,
    sctp_U16bit         *unrecognise_param_len,
    sctp_U8bit          *p_unrecognise_param,
    sctp_U8bit          *p_unresolved_addr,
    sctp_U16bit         *non_init_param_len,
    sctp_U8bit          *p_non_init_params,
    sctp_error_t        *p_ecode )
{
    sctp_U16bit parameter_id;
    sctp_U16bit parameter_len;
    sctp_U16bit parameter_offset;
    sctp_U16bit next_parameter_offset;
    sctp_return_t ret;
    sctp_U32bit offset;
    sctp_sockaddr_st address;
    sctp_U8bit          *p_start;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_unresolved_addr );
    /*to avoid the warning*/



    *p_ecode = E_NOERROR;  /* Initialise error code, Its being used later */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_chk_init_opt_params" ) );

    parameter_offset = chunk_offset + SCTP_MIN_INITCHUNK_LEN;
    next_parameter_offset = parameter_offset;

    for( ; ; )
    {
        if( parameter_offset == ( chunk_len + chunk_offset ) )
        {
            break;
        }

        parameter_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                        parameter_offset + SCTPPARAMOFFSET_LENGTH );

        if( ( parameter_len <  SCTPPARAMOFFSET_BODY ) ||
                ( parameter_len > ( chunk_len - /* Change for CSR 1-4580336 */
                                    ( next_parameter_offset - chunk_offset ) ) ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid parameter length" ) );
            *p_ecode = E_INVALID_PARAMETER_LEN;

            return ( SCTP_FAILURE );
        }

        /*  calculate next parameter offset */
        next_parameter_offset += parameter_len
                                 + sctp_get_parameter_pad_bytes( parameter_len );

        if( next_parameter_offset > chunk_len + chunk_offset )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_chk_init_opt_params: Invalid Chunk length" ) );
            *p_ecode = E_INVALID_CHUNK_SIZE;

            return ( SCTP_FAILURE );
        }

        parameter_id = sctp_buffer_get_UshortAtOffset( buffer,
                                                       parameter_offset );

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_chk_init_opt_params: Received parameter Id:%d",
                      parameter_id ) );

        ret = sctp_static_chk_init_paramid( parameter_id, p_ecode );

        if( ret == SCTP_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, \
                        ( "sctp_chk_init_opt_params: Invalid Optional Parameter,"
                          "parameter Id= %d", parameter_id ) );

            switch( *p_ecode )
            {
                case E_INVALID_PARAMETER_ID:
                case E_UNKNOWN_PARAMETER_ID:
                    /* No Further Processing */
                    return( SCTP_FAILURE );
                    break;

                case E_UNKNOWN_PARAMETER_RETURN_ERROR:
                    /* No Further Processing */
                    *unrecognise_param_present = SCTP_TRUE;
                    parameter_len += sctp_get_parameter_pad_bytes(
                                         parameter_len );

                    if( *unrecognise_param_len + parameter_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_unrecognise_param + *unrecognise_param_len,
                                   buffer + parameter_offset, parameter_len );
                        *unrecognise_param_len += parameter_len;
                    }

                    return ( SCTP_FAILURE );
                    break;

                case E_SKIP_PARAMETER:
                    /*  skip this parameter */
                    break;

                case E_SKIP_PARAMETER_RETURN_ERROR:
                    /*
                     * skip this parameter but copy in unknown parameters
                     * buffer. This will be sent with INIT-ACK.
                     */
                    *unrecognise_param_present = SCTP_TRUE;
                    parameter_len
                    += sctp_get_parameter_pad_bytes( parameter_len );

                    if( *unrecognise_param_len + parameter_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_unrecognise_param + *unrecognise_param_len,
                                   buffer + parameter_offset, parameter_len );

                        *unrecognise_param_len += parameter_len;
                    }

                    break;

                case E_UNKNOWN_PARAMETER_INIT_RETURN_ERROR:
                    /* The Parameter is Valid but it was not expected in INIT chunk. */
                    parameter_len += sctp_get_parameter_pad_bytes(
                                         parameter_len );

                    if( *non_init_param_len + parameter_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_non_init_params + *non_init_param_len,
                                   buffer + parameter_offset, parameter_len );
                        *non_init_param_len += parameter_len;
                    }

                    break;

                default:
                    *p_ecode = E_NOERROR;
                    break;
            }
        }

        else
        {
            ret = sctp_static_chk_init_paramlen( parameter_id, parameter_len );

            if( ret == SCTP_FAILURE )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_chk_init_opt_params: Invalid Optional Parameter"
                              " Length,Len =%d", parameter_len ) );

                *p_ecode = E_INVALID_PARAMETER_LEN;

                return ( SCTP_FAILURE );
            }

            switch( parameter_id )
            {
                case SCTPVARPAR_IPV4ADDRESS:
                    if( init->num_addrs >= SCTP_MAX_TRANSPORT_ADDR )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return ( SCTP_FAILURE );
                    }

                    address.ip.v4.addr = sctp_buffer_get_UlongAtOffset( buffer,
                                                                        parameter_offset + SCTPPARAMOFFSET_BODY );

                    /* Set the Address family to AF_INET */
                    address.family = SCTP_AF_INET;

                    if( sctp_chk_addr_unicast( &address ) == SCTP_FAILURE )
                    {
                        p_start =  p_unresolved_addr;
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV4ADDRESS );
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, 8 );   /* Length */

                        /*  byte ordering only where needed */
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr, address.ip.v4.addr );
                        /* SPR 21141 chnages starts*/
                        SCTP_UNUSED_VARIABLE( p_unresolved_addr );
                        /* SPR 21141 changes ends*/

                        p_unresolved_addr = p_start;
                        *p_ecode = E_UNSUPPORTED_ADDR;


                        /* SPR 21107 changes start */
                        SCTP_UNUSED_VARIABLE( p_unresolved_addr );
                        /* SPR 21107 changes end */

                        return ( SCTP_FAILURE );
                    }

                    /* Copy the address into the chunk structure */
                    init->addr_list[init->num_addrs] = address;
                    init->num_addrs++;
                    break;

                case SCTPVARPAR_ECNCAPABLE:
                    if( init->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return( SCTP_FAILURE );
                    }

                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;

                    init->num_opt_params++;
                    break;

                case SCTPVARPAR_HOSTNAME:
                    if( init->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return( SCTP_FAILURE );
                    }

                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;
                    init->opt_params[init->num_opt_params].param_len
                        = parameter_len;

                    __MEMCOPY( &init->opt_params[init->num_opt_params].param_data.
                               hostname[0], buffer + parameter_offset +
                               SCTPPARAMOFFSET_BODY, parameter_len
                               - SCTPPARAMOFFSET_BODY );

                    init->num_opt_params++;
                    break;


                case SCTPVARPAR_SUPADDRTYPE:
                    if( init->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return( SCTP_FAILURE );
                    }

                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;
                    init->opt_params[init->num_opt_params].param_len
                        = parameter_len;

                    init->opt_params[init->num_opt_params].param_data.
                    supported_addrtypes.v4 = SCTP_FALSE;
                    init->opt_params[init->num_opt_params].param_data.
                    supported_addrtypes.v6 = SCTP_FALSE;
                    init->opt_params[init->num_opt_params].param_data.
                    supported_addrtypes.hname = SCTP_FALSE;

                    offset = SCTPPARAMOFFSET_BODY;

                    for( ; offset < parameter_len; offset += 2 )
                    {
                        sctp_U16bit sup_type;
                        sup_type =
                            sctp_buffer_get_UshortAtOffset( buffer, parameter_offset + offset );

                        switch( sup_type )
                        {
                            case SCTPVARPAR_IPV4ADDRESS:
                                init->opt_params[init->num_opt_params].param_data.
                                supported_addrtypes.v4 = SCTP_TRUE;
                                break;

                            case SCTPVARPAR_IPV6ADDRESS:
                                init->opt_params[init->num_opt_params].param_data.
                                supported_addrtypes.v6 = SCTP_TRUE;
                                break;

                            case SCTPVARPAR_HOSTNAME:
                                init->opt_params[init->num_opt_params].param_data.
                                supported_addrtypes.hname = SCTP_TRUE;
                                break;

                            default:
                                /*CSR No 1712735*/
                                p_start =  p_unresolved_addr;
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_SUPADDRTYPE );
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, 0 );  /* Length place holder*/
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, sup_type );
                                sctp_buffer_set_UshortAtOffset( p_start, SCTPPARAMOFFSET_LENGTH,
                                                                ( sctp_U16bit )( p_unresolved_addr - p_start ) );
                                p_unresolved_addr = p_start;
                                *p_ecode = E_INVALID_ADDR;
                                return ( SCTP_FAILURE );

                        }
                    }

                    init->num_opt_params++;
                    break;

                case SCTPVARPAR_COOKIEPRESV:
                    if( init->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return ( SCTP_FAILURE );
                    }

                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;
                    init->opt_params[init->num_opt_params].
                    param_data.cookie_preservative
                        = sctp_buffer_get_UlongAtOffset(
                              buffer, parameter_offset + SCTPPARAMOFFSET_BODY );
                    init->num_opt_params++;

                    break;

                case SCTPVARPAR_IPV6ADDRESS:
                    if( init->num_addrs >= SCTP_MAX_TRANSPORT_ADDR )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return ( SCTP_FAILURE );
                    }

                    /* Extract the IPv6 address from the buffer */
                    address.ip.v6.addr.s_u6_addr32[0] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY );

                    address.ip.v6.addr.s_u6_addr32[1] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY + 4 );

                    address.ip.v6.addr.s_u6_addr32[2] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY + 8 );

                    address.ip.v6.addr.s_u6_addr32[3] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY + 12 );

                    /* Set the flow info and the scope id. */
                    address.ip.v6.flow_info = 0;
                    address.ip.v6.scope_id  = SCTP_IPV6_SCOPE_LINK_LOCAL;

                    /* Set the Address family to AF_INET */
                    address.family = SCTP_AF_INET6;

                    /*
                     * Check That the IPV6 address is a unicast address. i.e. not
                     * a broadcast or unicast address.
                     */
                    if( sctp_chk_addr_unicast( &address ) == SCTP_FAILURE )
                    {
                        /* SPR_20411_FIX_START -- */

                        p_start =  p_unresolved_addr;
                        /* Bug Id 81 fix s */
                        SCTP_UNUSED_VARIABLE( p_start );
                        /* Bug Id 81 fix e */
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV6ADDRESS );
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPPARAM_LEN_IPV6ADDRESS );  /* Length */
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[0] );
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[1] );
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[2] );
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[3] );

                        p_unresolved_addr = p_start;
                        /* SPR_20411_FIX_END -- */

                        *p_ecode = E_UNSUPPORTED_ADDR;

                        return ( SCTP_FAILURE );
                    }


                    /* Copy the address into the chunk structure */
                    init->addr_list[init->num_addrs] = address;
                    init->num_addrs++;
                    break;

                case SCTPVARPAR_ADAPTIONIND:
                    if( init->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return( SCTP_FAILURE );
                    }

                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;
                    init->opt_params[init->num_opt_params].param_len
                        = parameter_len;
                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;
                    init->opt_params[init->num_opt_params].
                    param_data.adaption_ind =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY );
                    init->num_opt_params++;
                    break;
#ifdef SCTP_DYNAMIC_IP_SUPPORT

                case SCTP_ASCONF_PARAM_SET_PRIM:
                    if( init->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return( SCTP_FAILURE );
                    }

                    init->opt_params[init->num_opt_params].param_type
                        = parameter_id;

                    /*read the IP address. ignore correlation id */
                    if( SCTP_NULL == sctp_parse_chk_addr_param(
                                buffer + parameter_offset + SCTPPARAMOFFSET_BODY + 4,
                                ( sctp_U16bit )( parameter_len - ( SCTPPARAMOFFSET_BODY + 4 ) ),
                                &init->opt_params[init->num_opt_params].
                                param_data.pref_primaddr, p_ecode ) )
                    {
                        if( *p_ecode == E_UNSUPPORTED_ADDR )
                        {
                            /* SPR_20411_FIX_START -- */
                            p_start =  p_unresolved_addr;

                            if( init->opt_params[init->num_opt_params].param_data.pref_primaddr.family == SCTP_AF_INET )
                            {
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV4ADDRESS );
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, 8 );   /* Length */

                                /*  byte ordering only where needed */
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr, init->opt_params[init->num_opt_params].param_data.pref_primaddr.ip.v4.addr );
                            }

                            else
                            {
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV6ADDRESS );
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPPARAM_LEN_IPV6ADDRESS );  /* Length */
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           init->opt_params[init->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[0] );
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           init->opt_params[init->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[1] );
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           init->opt_params[init->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[2] );
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           init->opt_params[init->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[3] );
                            }

                            p_unresolved_addr = p_start;
                            /* SPR_20411_FIX_END -- */


                            return SCTP_FAILURE;
                        }
                    }

                    init->num_opt_params++;
                    break;
#endif

                default:
                    return SCTP_FAILURE;

            }
        }

        parameter_offset = next_parameter_offset;
    } /* For Loop */

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_initack_opt_params
 *
 *    DESCRIPTION:
 *        This function will parse INITACK chunk for all valid optional
 *        parameters and perform static checks on all optional parameters
 *        In-line functions
 *
 *        sctp_extract_param_id
 *        sctp_static_chk_opt_param_in_init
 *        sctp_extract_param_len
 *        sctp_static_chk_param_len
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_chk_initack_opt_params(
    sctp_U8bit             *buffer,
    sctp_U16bit            chunk_offset,
    sctp_U16bit            chunk_len,
    sctp_chunk_initack_st  *initack,
    sctp_Boolean_t         *unrecognise_param_present,
    sctp_U16bit            *unrecognise_param_len,
    sctp_U8bit             *p_unrecognise_param,
    sctp_U8bit             *p_unresolved_addr,
    sctp_error_t           *p_ecode )
{
    sctp_U16bit parameter_id;
    /* SPR 20573 Starts for CSR 1-7088501  */
    sctp_U16bit parameter_len = 0;
    sctp_U16bit parameter_offset = 0;
    sctp_U16bit next_parameter_offset = 0;
    sctp_U16bit parameter_len_temp = 0;
    /* SPR 20573 Ends for CSR 1-7088501 */
    sctp_U16bit cookie_len;
    sctp_return_t    ret;
    sctp_Boolean_t   cookie_found;
    sctp_sockaddr_st address;
    sctp_U8bit             *p_start;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_unresolved_addr );
    /*to avoid the warning*/



    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_chk_initack_opt_params" ) );

    cookie_found = SCTP_FALSE;
    parameter_offset = chunk_offset + SCTP_MIN_INITACKCHUNK_LEN;
    next_parameter_offset = parameter_offset;

    for( ; ; )
    {
        if( parameter_offset == chunk_len + chunk_offset )
        {
            break;
        }

        parameter_len = sctp_buffer_get_UshortAtOffset(
                            buffer, parameter_offset + SCTPPARAMOFFSET_LENGTH );

        if( ( parameter_len <  SCTPPARAMOFFSET_BODY ) ||
                ( parameter_len > ( chunk_len - /* Change for CSR 1-4580336 */
                                    ( next_parameter_offset - chunk_offset ) ) ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid parameter length" ) );
            *p_ecode = E_INVALID_PARAMETER_LEN;

            return ( SCTP_FAILURE );
        }

        /*  calculate next parameter offset */
        /* SPR 20573 Starts for CSR 1-7088501  */
        parameter_len_temp = sctp_get_parameter_pad_bytes( parameter_len );
        next_parameter_offset += ( sctp_U16bit )parameter_len;
        next_parameter_offset += ( sctp_U16bit )parameter_len_temp;
        /* SPR 20573 Ends for CSR 1-7088501 */

        if( next_parameter_offset > chunk_len + chunk_offset )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid Chunk length" ) );
            *p_ecode = E_INVALID_CHUNK_SIZE;

            return ( SCTP_FAILURE );
        }

        parameter_id = sctp_buffer_get_UshortAtOffset( buffer, parameter_offset );

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_chk_initack_opt_params:Received parameter Id:%d",
                      parameter_id ) );

        ret = sctp_static_chk_initack_paramid( parameter_id, p_ecode );

        if( ret == SCTP_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_chk_initack_opt_params:Invalid Optional Parameter,"
                          "parameter Id= %d", parameter_id ) );

            switch( *p_ecode )
            {
                case E_UNKNOWN_PARAMETER_ID:
                    /*  return failure from here */

                    return ( SCTP_FAILURE );
                    break;

                case E_UNKNOWN_PARAMETER_RETURN_ERROR:
                    /*
                     * Return Failure, copy unknown parameters.
                     */
                    *unrecognise_param_present = SCTP_TRUE;
                    parameter_len +=
                        sctp_get_parameter_pad_bytes( parameter_len );

                    if( *unrecognise_param_len + parameter_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_unrecognise_param + *unrecognise_param_len,
                                   buffer + parameter_offset, parameter_len );
                        *unrecognise_param_len += parameter_len;
                    }

                    return( SCTP_FAILURE );
                    break;

                case E_SKIP_PARAMETER:
                    /*  skip this parameter */
                    break;

                case E_UNKNOWN_PARAMETER_INITACK_RETURN_ERROR:
                case E_SKIP_PARAMETER_RETURN_ERROR:
                    /*
                    * skip this parameter but copy in unknown parameters
                    * buffer
                    */
                    *unrecognise_param_present = SCTP_TRUE;
                    parameter_len +=
                        sctp_get_parameter_pad_bytes( parameter_len );

                    if( *unrecognise_param_len + parameter_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_unrecognise_param + *unrecognise_param_len,
                                   buffer + parameter_offset, parameter_len );

                        *unrecognise_param_len += parameter_len;
                    }

                    break;

                default:
                    break;
            }
        }

        else
        {
            ret = sctp_static_chk_initack_paramlen( parameter_id, parameter_len );

            if( ret == SCTP_FAILURE )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_chk_initack_opt_params:Invalid Optional Parameter Length"
                              ",Len :%d", parameter_len ) );

                *p_ecode = E_INVALID_PARAMETER_LEN;

                return ( SCTP_FAILURE );
            }

            switch( parameter_id )
            {
                case SCTPVARPAR_IPV4ADDRESS:
                    if( initack->num_addrs >= SCTP_MAX_TRANSPORT_ADDR )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return ( SCTP_FAILURE );
                    }

                    address.ip.v4.addr = sctp_buffer_get_UlongAtOffset( buffer,
                                                                        parameter_offset + SCTPPARAMOFFSET_BODY );

                    /* Set the Address family to AF_INET */
                    address.family = SCTP_AF_INET;

                    if( sctp_chk_addr_unicast( &address ) == SCTP_FAILURE )
                    {
                        p_start =  p_unresolved_addr;
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV4ADDRESS );
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, 8 );   /* Length */
                        /*  byte ordering only where needed */
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr, address.ip.v4.addr );
                        /* SPR 21141 chnages starts*/
                        SCTP_UNUSED_VARIABLE( p_unresolved_addr );
                        /* SPR 21141 changes ends*/
                        p_unresolved_addr = p_start;
                        *p_ecode = E_UNSUPPORTED_ADDR;

                        /* SPR 21107 changes start */
                        SCTP_UNUSED_VARIABLE( p_unresolved_addr );
                        /* SPR 21107 changes end */

                        return ( SCTP_FAILURE );
                    }

                    /* Copy the address into the chunk structure */
                    initack->addr_list[initack->num_addrs] = address;
                    initack->num_addrs++;

                    break;

                case SCTPVARPAR_ECNCAPABLE:
                    if( initack->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return ( SCTP_FAILURE );
                    }

                    initack->opt_params[initack->num_opt_params].param_type
                        = parameter_id;
                    initack->num_opt_params++;

                    break;

                case SCTPVARPAR_HOSTNAME:
                    if( initack->num_opt_params >= SCTP_MAX_OPTIONAL_PARAMS )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return( SCTP_FAILURE );
                    }

                    initack->opt_params[initack->num_opt_params].param_type =
                        parameter_id;
                    initack->opt_params[initack->num_opt_params].param_len =
                        parameter_len;

                    __MEMCOPY( &initack->opt_params[initack->num_opt_params].
                               param_data.hostname[0],
                               buffer + parameter_offset + SCTPPARAMOFFSET_BODY,
                               parameter_len - SCTPPARAMOFFSET_BODY );

                    initack->num_opt_params++;
                    break;

                case SCTPVARPAR_IPV6ADDRESS:
                    if( initack->num_addrs >= SCTP_MAX_TRANSPORT_ADDR )
                    {
                        *p_ecode = E_INVALID_NUM_OPTIONAL_PARAMS;

                        return ( SCTP_FAILURE );
                    }

                    /* Extract the IPv6 address from the buffer */
                    address.ip.v6.addr.s_u6_addr32[0] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY );

                    address.ip.v6.addr.s_u6_addr32[1] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY + 4 );

                    address.ip.v6.addr.s_u6_addr32[2] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY + 8 );

                    address.ip.v6.addr.s_u6_addr32[3] =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY + 12 );

                    /* Set the flow info and the scope id. */
                    address.ip.v6.flow_info = 0;
                    address.ip.v6.scope_id  = SCTP_IPV6_SCOPE_LINK_LOCAL;

                    /* Set the Address family to AF_INET */
                    address.family = SCTP_AF_INET6;

                    /*
                     * Check That the IPV6 address is a unicast address. i.e. not
                     * a broadcast or unicast address.
                     */
                    if( sctp_chk_addr_unicast( &address ) == SCTP_FAILURE )
                    {
                        /* SPR_20411_FIX_START -- */
                        p_start =  p_unresolved_addr;
                        /* Bug Id 81 fix s */
                        SCTP_UNUSED_VARIABLE( p_start );
                        /* Bug Id 81 fix e */
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV6ADDRESS );
                        p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPPARAM_LEN_IPV6ADDRESS );  /* Length */
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[0] );
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[1] );
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[2] );
                        p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                   address.ip.v6.addr.s_u6_addr32[3] );
                        p_unresolved_addr = p_start;

                        /* SPR_20411_FIX_END -- */
                        *p_ecode = E_UNSUPPORTED_ADDR;

                        return ( SCTP_FAILURE );
                    }

                    /* Copy the address into the chunk structure */
                    initack->addr_list[initack->num_addrs] = address;
                    initack->num_addrs++;

                    break;

                case SCTPVARPAR_UNKNOWNPAR:
                    break;

                case SCTPVARPAR_COOKIE:
                    cookie_found = SCTP_TRUE;
                    cookie_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                                 parameter_offset + SCTPPARAMOFFSET_LENGTH );

                    /*  Cookie Length  minus the header length */
                    initack->cookie_len = cookie_len - SCTPPARAMOFFSET_BODY;
                    initack->cookie = buffer + parameter_offset
                                      + SCTPPARAMOFFSET_BODY;

                    break;

                case SCTPVARPAR_ADAPTIONIND:
                    initack->opt_params[initack->num_opt_params].param_type
                        = parameter_id;
                    initack->opt_params[initack->num_opt_params].
                    param_data.adaption_ind =
                        sctp_buffer_get_UlongAtOffset( buffer,
                                                       parameter_offset + SCTPPARAMOFFSET_BODY );
                    initack->num_opt_params++;
                    break;
#ifdef SCTP_DYNAMIC_IP_SUPPORT

                case SCTP_ASCONF_PARAM_SET_PRIM:
                    initack->opt_params[initack->num_opt_params].param_type
                        = parameter_id;

                    /*read the IP address. ignore correlation id */
                    if( SCTP_NULL == sctp_parse_chk_addr_param(
                                buffer + parameter_offset + SCTPPARAMOFFSET_BODY + 4,
                                ( sctp_U16bit )( parameter_len - ( SCTPPARAMOFFSET_BODY + 4 ) ),
                                &initack->opt_params[initack->num_opt_params].
                                param_data.pref_primaddr, p_ecode ) )
                    {
                        if( *p_ecode == E_UNSUPPORTED_ADDR )
                        {
                            /* SPR_20411_FIX_START -- */
                            p_start =  p_unresolved_addr;

                            if( initack->opt_params[initack->num_opt_params].param_data.pref_primaddr.family == SCTP_AF_INET )
                            {
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV4ADDRESS );
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, 8 );   /* Length */

                                /*  byte ordering only where needed */
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr, initack->opt_params[initack->num_opt_params].param_data.pref_primaddr.ip.v4.addr );
                            }

                            else
                            {
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPVARPAR_IPV6ADDRESS );
                                p_unresolved_addr = sctp_buffer_set_Ushort( p_unresolved_addr, SCTPPARAM_LEN_IPV6ADDRESS );  /* Length */
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           initack->opt_params[initack->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[0] );
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           initack->opt_params[initack->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[1] );
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           initack->opt_params[initack->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[2] );
                                p_unresolved_addr = sctp_buffer_set_Ulong( p_unresolved_addr,
                                                                           initack->opt_params[initack->num_opt_params].param_data.pref_primaddr.ip.v6.addr.s_u6_addr32[3] );
                            }

                            p_unresolved_addr = p_start;
                            /* SPR_20411_FIX_END -- */
                        }

                        return SCTP_FAILURE;
                    }

                    initack->num_opt_params++;
                    break;
#endif
            }
        }

        parameter_offset = next_parameter_offset;
    } /* For Loop */

    if( cookie_found == SCTP_FALSE )
    {
        *p_ecode = E_MISSING_PARAMS;

        return( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_init_fixed_params
 *
 *    DESCRIPTION:
 *         Checks fixed parameters in INIT Chunks
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_chk_init_fixed_params(
    sctp_U8bit          *buffer,
    sctp_U32bit         chunk_offset,
    sctp_chunk_init_st  *init_chunk,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit init_tag;
    sctp_U32bit a_rwnd;
    sctp_U32bit init_tsn;
    sctp_U16bit num_out_streams;
    sctp_U16bit num_in_streams;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_chk_init_fixed_params " ) );

    /* extract init tag */
    init_tag = sctp_buffer_get_UlongAtOffset( buffer,
                                              chunk_offset + SCTPOFFSET_INIT_INITTAG );

    a_rwnd = sctp_buffer_get_UlongAtOffset( buffer,
                                            chunk_offset + SCTPOFFSET_INIT_ARWND );

    num_out_streams = sctp_buffer_get_UshortAtOffset( buffer,
                                                      chunk_offset + SCTPOFFSET_INIT_NUMOS );

    num_in_streams = sctp_buffer_get_UshortAtOffset( buffer,
                                                     chunk_offset + SCTPOFFSET_INIT_NUMIS );

    init_tsn = sctp_buffer_get_UlongAtOffset( buffer,
                                              chunk_offset + SCTPOFFSET_INIT_INITTSN );

    init_chunk->fixed_params.init_tag = init_tag;
    init_chunk->fixed_params.a_rwnd = a_rwnd;
    init_chunk->fixed_params.num_in_streams = num_in_streams;
    init_chunk->fixed_params.num_out_streams = num_out_streams;
    init_chunk->fixed_params.init_tsn = init_tsn;

    /* SPR 4040 */
    if( ( init_tag == 0 ) || ( a_rwnd < 1500 ) ||
            ( num_out_streams == 0 ) || ( num_in_streams == 0 ) )
    {
        *p_ecode = E_INVALID_FIXED_PARAMETERS;

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_init
 *
 *    DESCRIPTION:
 *        This function will parse INIT chunk and perform static
 *        checks on all mandatory and optional parameters.
 *
 *
 *        sctp_extract_initiate_tag_
 *        sctp_static_chk_initiate_tag
 *        sctp_extract_arwnd
 *        sctp_extract_out_streams
 *        sctp_static_chk_out_streams
 *        sctp_extract_maximum_in_strems
 *        sctp_static_chk_maximum_in_strems
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_init(
    sctp_U8bit          *buffer,
    sctp_U32bit         chunk_offset,
    sctp_chunk_init_st  *init_chunk,
    sctp_Boolean_t      *unrecognise_param_present,
    sctp_U16bit         *unrecognise_param_len,
    sctp_U8bit          *p_unrecognise_param,
    sctp_U8bit          *p_unresolved_addr,
    sctp_U16bit         *non_init_params_len,
    sctp_U8bit          *p_non_init_params,
    sctp_error_t        *p_ecode )
{
    sctp_U16bit   chunk_len;
    sctp_return_t ret;
    sctp_U32bit init_tag;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_init" ) );

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    chunk_len += ( sctp_U16bit )sctp_get_chunk_padd_bytes( SCTPCHUNK_INIT,
                                                           chunk_len );

    /*CSR No:1-1690052*/
    if( ( chunk_len >= SCTP_MIN_CHUNK_LEN + 4 ) &&
            ( chunk_len < SCTP_MIN_INITCHUNK_LEN ) )
    {
        init_tag = sctp_buffer_get_UlongAtOffset( buffer,
                                                  chunk_offset + SCTPOFFSET_INIT_INITTAG );
        init_chunk->fixed_params.init_tag = init_tag;
    }

    /*CSR No:1-1690052*/


    if( chunk_len < SCTP_MIN_INITCHUNK_LEN )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_init: Missing Mandatory parameters " ) );
        *p_ecode = E_INVALID_CHUNK_SIZE;

        return ( SCTP_FAILURE );
    }

    ret = sctp_chk_init_fixed_params(
              buffer,
              chunk_offset,
              init_chunk,
              p_ecode );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_init:Invalid fixed Paramameters " ) );

        return ( SCTP_FAILURE );
    }

    if( chunk_len == SCTP_MIN_INITCHUNK_LEN )
    {
        /* No optional parameters, return success */

        return ( SCTP_SUCCESS );
    }

    /* make sure we can read first parameter header */
    if( chunk_len >= SCTP_MIN_INITCHUNK_LEN + SCTPPARAMOFFSET_BODY )
    {
        /* Parse optional parameters */
        ret = sctp_chk_init_opt_params(
                  buffer,
                  ( sctp_U16bit )chunk_offset,
                  chunk_len,
                  init_chunk,
                  unrecognise_param_present,
                  unrecognise_param_len,
                  p_unrecognise_param,
                  p_unresolved_addr,
                  non_init_params_len,
                  p_non_init_params,
                  p_ecode );

        if( ret == SCTP_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_parse_chk_chunk_init: Invalid Optional paramameters " ) );

            return ( SCTP_FAILURE );
        }

        else
        {
            return ( SCTP_SUCCESS );
        }
    }

    *p_ecode = E_INVALID_CHUNK_SIZE;

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_initack
 *
 *    DESCRIPTION:
 *        This function will parse INITACK chunk and perform static
 *        checks on all mandatory and optional parameters.
 *        In-line functions
 *
 *        sctp_extract_initiate_tag_
 *        sctp_static_chk_initiate_tag
 *        sctp_extract_arwnd
 *        sctp_extract_out_streams
 *        sctp_static_chk_out_streams
 *        sctp_extract_maximum_in_strems
 *        sctp_static_chk_maximum_in_strems
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_initack(
    sctp_U8bit             *buffer,
    sctp_U32bit            chunk_offset,
    sctp_chunk_initack_st  *initack_chunk,
    sctp_Boolean_t         *unrecognise_param_present,
    sctp_U16bit            *unrecognise_param_len,
    sctp_U8bit             *p_unrecognise_param,
    sctp_U8bit             *p_unresolved_addr,
    sctp_error_t           *p_ecode )
{
    sctp_U16bit   chunk_len;
    sctp_return_t ret;
    sctp_U32bit init_tag;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_initack " ) );

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    chunk_len += ( sctp_U16bit )sctp_get_chunk_padd_bytes( SCTPCHUNK_INITACK,
                                                           chunk_len );

    /*CSR No 1-1690052*/
    if( ( chunk_len >= SCTP_MIN_CHUNK_LEN + 4 ) &&
            ( chunk_len <= SCTP_MIN_INITACKCHUNK_LEN ) ) /*SPR NO 20358*/
    {
        init_tag = sctp_buffer_get_UlongAtOffset( buffer,
                                                  chunk_offset + SCTPOFFSET_INIT_INITTAG );
        initack_chunk->fixed_params.init_tag = init_tag;
    }

    /* CSR No. 1-1690052*/

    /* Make sure we can read all fixed parameters */
    if( chunk_len < SCTP_MIN_INITACKCHUNK_LEN )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_initack: Missing Mandatory parameters" ) );
        *p_ecode = E_INVALID_CHUNK_SIZE;

        return ( SCTP_FAILURE );
    }

    if( chunk_len == SCTP_MIN_INITACKCHUNK_LEN )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_initack:Cookie missing " ) );
        *p_ecode = E_MISSING_PARAMS;

        return ( SCTP_FAILURE );
    }

    ret = sctp_chk_init_fixed_params(
              buffer,
              chunk_offset,
              ( sctp_chunk_init_st * )initack_chunk,
              p_ecode );

    if( ret != SCTP_SUCCESS )
    {

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_initack: Invalid fixed Paramameters" ) );

        return ( SCTP_FAILURE );
    }

    ret = sctp_chk_initack_opt_params(
              buffer,
              ( sctp_U16bit )chunk_offset ,
              chunk_len,
              initack_chunk,
              unrecognise_param_present,
              unrecognise_param_len,
              p_unrecognise_param,
              p_unresolved_addr,
              p_ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_initack: Invalid Optional paramameters" ) );

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_error_code
 *
 *    DESCRIPTION:
 *        Checks for the error codes
 *
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_parse_chk_error_code(
    sctp_U8bit                  *buffer,
    sctp_U32bit                 chunk_offset,
    sctp_U16bit                 chunk_len,
    sctp_U32bit                 *num_errors,
    sctp_error_cause_st         *error_codes,
    sctp_error_t                *p_ecode )
{
    sctp_U32bit    num_missing_params;
    sctp_U16bit    param_offset;
    sctp_U16bit    cause_id;
    sctp_U16bit    cause_len;
    sctp_U32bit    cause_offset;
    sctp_U32bit    next_cause_offset;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_error_code" ) );

    cause_offset = chunk_offset + SCTPCHUNKOFFSET_BODY;
    cause_id     = sctp_buffer_get_UshortAtOffset( buffer, cause_offset );
    cause_len    = sctp_buffer_get_UshortAtOffset( buffer,
                                                   cause_offset + SCTPERROROFFSET_LENGTH );

    for( ;  ; )
    {
        next_cause_offset = cause_offset + cause_len;

        /* This check is necessary,so we do not read pass buffer boundry */
        if( next_cause_offset > chunk_len + chunk_offset )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_parse_chk_error_code: Invalid Chunk length " ) );

            *p_ecode = E_INVALID_CHUNK_SIZE;

            return ( SCTP_FAILURE );
        }

        error_codes[*num_errors].cause_code = cause_id;
        error_codes[*num_errors].cause_len = cause_len;

        switch( cause_id )
        {
            case SCTPERR_INVALIDSTREAM:
                if( cause_len != SCTPERRORLEN_INVALIDSTREAM )
                {
                    return ( SCTP_FAILURE );
                }

                else
                {
                    error_codes[*num_errors].cause_info.invalid_stream.
                    stream_id = sctp_buffer_get_UshortAtOffset( buffer,
                                                                cause_offset + SCTPERROROFFSET_BODY );
                }

                break;

            case SCTPERR_MISSINGPARAM:
                num_missing_params
                    = sctp_buffer_get_UlongAtOffset( buffer, cause_offset
                                                     + SCTPERROROFFSET_BODY );

                error_codes[*num_errors].cause_info.missing_params.
                num_missing_params = num_missing_params;

                if( cause_len != ( num_missing_params * 2 ) + 8 )
                {
                    return ( SCTP_FAILURE );
                }

                if( num_missing_params > SCTP_MAX_INIT_TLVPARMS )
                {
                    return ( SCTP_FAILURE );
                }

                param_offset = ( sctp_U16bit )( cause_offset + SCTPERROROFFSET_BODY
                                                + SCTPERRORLEN_NUM_MISSING_PARAM );

                for( ; num_missing_params > 0; num_missing_params-- )
                {
                    error_codes[*num_errors].
                    cause_info.missing_params.param_id[num_missing_params - 1]
                        = sctp_buffer_get_UshortAtOffset( buffer, param_offset );

                    param_offset += SCTPERRORLEN_MISSING_PARAM_TYPE;
                }

                break;

            case SCTPERR_STALECOOKIE:
                if( cause_len != SCTPERRORLEN_STALECOOKIE )
                {
                    return ( SCTP_FAILURE );
                }

                error_codes[*num_errors].cause_info.stale_cookie_error.time_value
                    = sctp_buffer_get_UlongAtOffset( buffer,
                                                     cause_offset + SCTPERROROFFSET_BODY );
                break;

            case SCTPERR_NORESOURCE:
                if( cause_len != SCTPERRORLEN_NORESOURCE )
                {
                    return ( SCTP_FAILURE );
                }

                break;

            case SCTPERR_UNRESOLVEADDR:
                /* only copy the pointer and not the entire buffer */
                error_codes[*num_errors].cause_info.unresolve_addr.addr
                    = buffer + cause_offset + SCTPERROROFFSET_BODY;

                if( cause_len % 4 != 0 )
                {
                    next_cause_offset += 4 - cause_len % 4;
                }

                break;

            case SCTPERR_UNRECOCHUNK:
                /* only copy the pointer and not the entire buffer */
                error_codes[*num_errors].cause_info.unrecognize_chunk_type.chunk
                    = buffer + cause_offset + SCTPERROROFFSET_BODY;

                /* add padd bytes to next offset here */
                if( cause_len % 4 != 0 )
                {
                    next_cause_offset += 4 - cause_len % 4;
                }

                break;

            case SCTPERR_INVMANDPARAM:
                if( cause_len != SCTPERRORLEN_INVMANDPARAM )
                {
                    return( SCTP_FAILURE );
                }

                break;

            case SCTPERR_UNRECOPARAMS:
                /* only copy the pointer and not the entire buffer */
                error_codes[*num_errors].cause_info.unrecognize_parameters.
                parameters = buffer + cause_offset + SCTPERROROFFSET_BODY;

                if( cause_len % 4 != 0 )
                {
                    next_cause_offset += 4 - cause_len % 4;
                }

                break;

            case SCTPERR_NOUSERDATA:
                if( cause_len != SCTPERRORLEN_NOUSERDATA )
                {
                    return ( SCTP_FAILURE );
                }

                else
                {
                    error_codes[*num_errors].cause_info.no_user_data.tsn =
                        sctp_buffer_get_UlongAtOffset( buffer, cause_offset +
                                                       SCTPERROROFFSET_BODY );
                }

                break;

            case SCTPERR_COOKIERECVSHUTDOWN:
                if( cause_len != SCTPERRORLEN_COOKIERECVSHUTDOWN )
                {
                    return ( SCTP_FAILURE );
                }

                break;

            case SCTPERR_ILLEGALASCONFACK:
                if( cause_len != 4 )
                {
                    return ( SCTP_FAILURE );
                }

                break;

            case SCTPERR_RESTARTNEWADDRS:
                break;

            case SCTPERR_USER_INITIATED_ABORT:
                if( cause_len > 4 )
                {
                    /* only copy the pointer and not the entire buffer */
                    error_codes[*num_errors].cause_info.user_initiated_abort.cause =
                        buffer + cause_offset + SCTPERROROFFSET_BODY;
                    /* Substact the Type and Length Feild length from the cause_len */
                    error_codes[*num_errors].cause_len  = cause_len - 4;
                }

                else
                {
                    error_codes[*num_errors].cause_info.user_initiated_abort.cause = SCTP_NULL;
                    error_codes[*num_errors].cause_len = 0;
                }

                break;

            case SCTPERR_PROTOCOL_VIOLATION:
                break;

            default:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_parse_chk_error_code: Invalid Error cause Id "
                              "= %d", cause_id ) );
                break;
        } /* Switch */

        ( *num_errors )++;

        /* if more than maximum error codes per chunk , drop rest */
        if( *num_errors >= SCTP_MAX_ERROR_CODES )
        {
            return ( SCTP_SUCCESS );
        }

        if( next_cause_offset % 4 != 0 )  /*Added to fix SPR947, SPR948*/
        {
            next_cause_offset += ( 4 - ( next_cause_offset % 4 ) );
        }

        if( next_cause_offset == chunk_len + chunk_offset )
        {
            break;
        }

        /* Make Sure we can read next parameter header */


        /* changed to fix SPR947, SPR948*/
        if( next_cause_offset > chunk_len + chunk_offset )
        {
            *p_ecode = E_INVALID_CHUNK_SIZE;

            return ( SCTP_FAILURE );
        }

        cause_offset = next_cause_offset;
        cause_id = sctp_buffer_get_UshortAtOffset( buffer, cause_offset );
        cause_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                    cause_offset + SCTPERROROFFSET_LENGTH );

    } /* For Loop */

    return ( SCTP_SUCCESS );
}


/***************************************************************************
*    FUNCTION :
*        sctp_parse_chk_chunk_abort
*
*    DESCRIPTION:
*        This function parses the abort chunk.
*        Id. If valid number of padding bits will be returned.
*
*    RETURN VALUE:
*        SCTP_SUCCESS
*        SCTP_FALILURE
*
**************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_abort(
    sctp_U8bit            *buffer,
    sctp_U32bit           chunk_offset,
    sctp_U32bit           *num_errors,
    sctp_error_cause_st   *error_codes,
    sctp_error_t          *p_ecode )
{
    sctp_U16bit chunk_len;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_abort" ) );

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    if( chunk_len == SCTPCHUNKOFFSET_BODY )
    {
        return ( SCTP_SUCCESS );
    }

    if( chunk_len < SCTPCHUNKOFFSET_BODY + 4 )
    {
        return ( SCTP_FAILURE );
    }


    /*Invalid Error cause will be ignored in ABORT chunk
     *and processing will be continued
     */
    sctp_parse_chk_error_code( buffer,
                               chunk_offset,
                               chunk_len,
                               num_errors,
                               error_codes,
                               p_ecode );

    return SCTP_SUCCESS;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_error
 *
 *    DESCRIPTION:
 *        This functions parses the error chunk
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_error(
    sctp_U8bit             *buffer,
    sctp_U32bit            chunk_offset,
    sctp_U32bit            *num_errors,
    sctp_error_cause_st    *error_codes,
    sctp_error_t           *p_ecode )
{
    sctp_U16bit chunk_len;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_error" ) );

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    if( chunk_len < ( SCTPCHUNKOFFSET_BODY + 4 ) )
    {
        return ( SCTP_FAILURE );
    }

    return  sctp_parse_chk_error_code( buffer,
                                       chunk_offset,
                                       chunk_len,
                                       num_errors,
                                       error_codes,
                                       p_ecode );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_sack
 *
 *    DESCRIPTION:
 *        This function will parse SACK chunk and perform static
 *        checks where applicable.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_sack(
    sctp_U8bit          *buffer,
    sctp_U32bit         chunk_offset,
    sctp_chunk_sack_st  *sack_chunk,
    sctp_error_t        *p_ecode )
{
    sctp_U16bit chunk_len, num_fragments, num_duplicate_tsn;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_parse_chk_chunk_sack" ) );

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    num_fragments = sctp_buffer_get_UshortAtOffset( buffer,
                                                    chunk_offset + SCTPOFFSET_SACK_NUMFRAGMENTS );

    num_duplicate_tsn = sctp_buffer_get_UshortAtOffset( buffer,
                                                        chunk_offset + SCTPOFFSET_SACK_DUPTSNS );

    if( chunk_len != SCTP_MIN_SACKCHUNK_LEN
            + ( num_fragments + num_duplicate_tsn ) * 4 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_sack: Invalid Chunk length,"
                      "chunk Len = %d", chunk_len ) );
        *p_ecode = E_INVALID_CHUNK_LEN;

        return ( SCTP_FAILURE );
    }

    /*
     * check on duplicate tsn fields dropped, We may still process SACK with
     * an Invalid Duplicate TSN field.
     */

    /* sack  chunk  */
    sack_chunk->cumtsn  = sctp_buffer_get_UlongAtOffset( buffer,
                                                         chunk_offset + SCTPOFFSET_SACK_CUMTSN );

    sack_chunk->arwnd = sctp_buffer_get_UlongAtOffset( buffer,
                                                       chunk_offset + SCTPOFFSET_SACK_ARWND );

    sack_chunk->numfrags = num_fragments;

    sack_chunk->frags
        = buffer + chunk_offset + SCTP_MIN_SACKCHUNK_LEN;

    sack_chunk->num_dupicate_tsn = num_duplicate_tsn;

    sack_chunk->dupicate_tsn
        = buffer + chunk_offset + SCTP_MIN_SACKCHUNK_LEN + 2 * num_fragments;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_heartbeatack
 *
 *    DESCRIPTION:
 *        This function will parse HEARTBEAT-ACK chunk and perform static
 *        checks where applicable. HeartbeatAck is expected to have
 *        3 Heartbeat Info types  for which it was sent.
 *        1. current time,
 *        2. 64-bit Random nonce
 *        3. destination address
 *
 *    RETURN VALUE:
 *        SCTP_FAILURE
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_heartbeatack(
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_tcb_st        *p_assoc,
    sctp_chunk_hb_st   *heartbeatack_chunk,
    sctp_error_t       *p_ecode )
{
    sctp_U16bit hb_info_len, hb_info_type;
    sctp_U32bit hb_info_offset;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_heartbeatack" ) );

    /* calculate heartbeat info 'Length' offset  */
    hb_info_offset = chunk_offset + SCTPCHUNKOFFSET_BODY + SCTPPARAMOFFSET_LENGTH;

    hb_info_len = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset );

    if( p_assoc->config.hb_info_length != hb_info_len )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB Info len,"
                      "Info len= %d", hb_info_len ) );

        *p_ecode = E_INVALID_HBINFO_LEN;

        return( SCTP_FAILURE );
    }

    /* calculate heartbeat info 'Value' offset */
    hb_info_offset += 2;

    hb_info_type = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset );

    /* first heartbeat info element should be timestamp */
    if( hb_info_type != SCTP_HBINFO_TYPE_TIME )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB Info,Info Id = %d",
                      hb_info_type ) );
        *p_ecode = E_INVALID_HBINFO_TYPE;

        return( SCTP_FAILURE );
    }

    /* extract heatbeat info length parameter */
    hb_info_len  = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset + 2 );

    if( hb_info_len != SCTP_HBINFO_LEN_TIME )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB Info len,"
                      "Info len= %d", hb_info_len ) );

        *p_ecode = E_INVALID_HBINFO_LEN;

        return( SCTP_FAILURE );
    }

    /* fill in data */
    heartbeatack_chunk->time_stamp.tv_sec
        = sctp_buffer_get_UlongAtOffset( buffer , hb_info_offset + 4 );

    heartbeatack_chunk->time_stamp.tv_usec
        = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 8 );

    hb_info_offset += hb_info_len;
    hb_info_type = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset );

    /* Second heartbeat info element should be destnation Address */
    if( ( hb_info_type != SCTP_HBINFO_TYPE_ADDRV4 ) &&
            ( hb_info_type != SCTP_HBINFO_TYPE_ADDRV6 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB Info,"
                      "Info Id = %d", hb_info_type ) );

        *p_ecode = E_INVALID_HBINFO_TYPE;

        return ( SCTP_FAILURE );
    }

    hb_info_len
        = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset + 2 );

    if( ( hb_info_len != SCTP_HBINFO_LEN_ADDRV4 ) &&
            ( hb_info_len != SCTP_HBINFO_LEN_ADDRV6 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB Info len,"
                      "Info len= %d", hb_info_len ) );

        *p_ecode = E_INVALID_HBINFO_LEN;

        return ( SCTP_FAILURE );
    }

    if( SCTP_HBINFO_TYPE_ADDRV4 == hb_info_type )
    {
        heartbeatack_chunk->dest_addr.family = SCTP_AF_INET;

        heartbeatack_chunk->dest_addr.ip.v4.addr
            = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 4 );
    }

    else
    {
        heartbeatack_chunk->dest_addr.family = SCTP_AF_INET6;

        heartbeatack_chunk->dest_addr.ip.v6.addr.s_u6_addr32[0]
            = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 4 );
        heartbeatack_chunk->dest_addr.ip.v6.addr.s_u6_addr32[1]
            = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 8 );
        heartbeatack_chunk->dest_addr.ip.v6.addr.s_u6_addr32[2]
            = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 12 );
        heartbeatack_chunk->dest_addr.ip.v6.addr.s_u6_addr32[3]
            = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 16 );
    }

    /* Third heartbeat info element should be 64-bit random nonce. */
    hb_info_offset += hb_info_len;
    hb_info_type = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset );

    if( hb_info_type != SCTP_HBINFO_TYPE_NONCE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB Info,"
                      "Info Id = %d, Expected was SCTP_HBINFO_TYPE_NONCE", hb_info_type ) );

        *p_ecode = E_INVALID_HBINFO_TYPE;

        return ( SCTP_FAILURE );
    }

    hb_info_len = sctp_buffer_get_UshortAtOffset( buffer, hb_info_offset + 2 );

    if( hb_info_len != SCTP_HBINFO_LEN_NONCE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_chunk_heartbeatack: Invalid HB 64-bit Nonce len,"
                      "Info len= %d, expected Info len = %d", hb_info_len, SCTP_HBINFO_LEN_NONCE ) );

        *p_ecode = E_INVALID_HBINFO_LEN;

        return ( SCTP_FAILURE );
    }

    heartbeatack_chunk->nonce[0] = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 4 );
    heartbeatack_chunk->nonce[1] = sctp_buffer_get_UlongAtOffset( buffer, hb_info_offset + 8 );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_chunk_padd_bytes
 *
 *    DESCRIPTION:
 *        This functions checks if padding is valid for this chunk
 *        Id. If valid number of padding bits will be returned.
 *
 *    RETURN VALUE:
 *        sctp_U32bit
 *
 **************************************************************************/
/* SPR 20283 */
sctp_U32bit
sctp_get_chunk_padd_bytes(
    sctp_U8bit    chunk_id,
    sctp_U16bit    chunk_len )
{
    sctp_U32bit pad_bytes = 0;

    if( ( chunk_id == SCTPCHUNK_SACK )
            || ( chunk_id == SCTPCHUNK_COOKIEACK )
            || ( chunk_id == SCTPCHUNK_SHUTDOWN )
            || ( chunk_id == SCTPCHUNK_SHUTDOWNACK )
            || ( chunk_id == SCTPCHUNK_SHUTDOWNCOMP ) )
    {
        return ( pad_bytes );
    }

    if( chunk_len % 4 != 0 )
    {
        pad_bytes = 4 - chunk_len % 4;
    }

    return ( pad_bytes );
}

/* SPR 20283 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_static_chk_chunk_len
 *
 *    DESCRIPTION:
 *        Perform static check on chunk length passed. If it is not
 *        of valid chunk length, an error is returned.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *
 **************************************************************************/
sctp_return_t
sctp_static_chk_chunk_len(
    sctp_U8bit      chunk_id,
    sctp_U16bit     chunk_len,
    sctp_error_t    *p_ecode )
{
    sctp_return_t ret = SCTP_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_static_chk_chunk_len: Received chunk Id:%d: len = %d",
                  chunk_id, chunk_len ) );

    switch( chunk_id )
    {
        case SCTPCHUNK_DATA:
            if( chunk_len < SCTP_MIN_DATACHUNK_LEN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_SACK:
            if( chunk_len < SCTP_MIN_SACKCHUNK_LEN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_HEARTBEATACK:
            if( chunk_len < SCTP_MIN_HBACKCHUNK_LEN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_SHUTDOWN:
            if( chunk_len != SCTP_CHUNK_LEN_SHUTDOWN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_SHUTDOWNACK:
            if( chunk_len != SCTP_CHUNK_LEN_SHUTDOWNACK )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_SHUTDOWNCOMP:
            if( chunk_len != SCTP_CHUNK_LEN_SHUTDOWNCOMP )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_COOKIEACK:
            if( chunk_len != SCTP_CHUNK_LEN_COOKIENACK )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_ECNE:
            if( chunk_len != SCTP_CHUNK_LEN_ECNE )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_CWR:
            if( chunk_len != SCTP_CHUNK_LEN_CWR )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_INITACK:
            if( chunk_len < SCTP_MIN_INITACKCHUNK_LEN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_INIT:
            if( chunk_len < SCTP_MIN_INITCHUNK_LEN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_HEARTBEAT:
            if( chunk_len < SCTP_MIN_HBCHUNK_LEN )
            {
                ret = SCTP_FAILURE;
            }

            break;

        case SCTPCHUNK_COOKIE:
        case SCTPCHUNK_ERROR:
        case SCTPCHUNK_ABORT:
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        case SCTPCHUNK_TYPE_ASCONFACK:
#endif
            if( chunk_len < SCTPCHUNKOFFSET_BODY )
            {
                ret = SCTP_FAILURE;
            }

            break;

#ifdef SCTP_DYNAMIC_IP_SUPPORT

        case SCTPCHUNK_TYPE_ASCONF:
            if( chunk_len < SCTPCHUNKOFFSET_BODY + 12 )
            {
                ret = SCTP_FAILURE;
            }

            break;
#endif

        default:
            break;
    }

    if( ret == SCTP_FAILURE )
    {
        *p_ecode = E_INVALID_CHUNK_SIZE;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_static_chk_chunk_id
 *
 *    DESCRIPTION:
 *        Perform static check on chunk Id passed. If it is not one
 *        of valid chunk Id'd an error is returned.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_static_chk_chunk_id(
    sctp_U8bit      chunk_id,
    sctp_error_t    *p_ecode )
{
    sctp_return_t ret;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_static_chk_chunk_id: Received chunk Id:%d", chunk_id ) );

    switch( chunk_id )
    {
        case SCTPCHUNK_DATA:
        case SCTPCHUNK_INIT:
        case SCTPCHUNK_INITACK:
        case SCTPCHUNK_SACK:
        case SCTPCHUNK_HEARTBEAT:
        case SCTPCHUNK_HEARTBEATACK:
        case SCTPCHUNK_ABORT:
        case SCTPCHUNK_SHUTDOWN:
        case SCTPCHUNK_SHUTDOWNACK:
        case SCTPCHUNK_ERROR:
        case SCTPCHUNK_COOKIE:
        case SCTPCHUNK_COOKIEACK:
        case SCTPCHUNK_ECNE:
        case SCTPCHUNK_CWR:
        case SCTPCHUNK_SHUTDOWNCOMP:
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        case SCTPCHUNK_TYPE_ASCONF:
        case SCTPCHUNK_TYPE_ASCONFACK:
#endif
            ret = SCTP_SUCCESS;
            break;

        default:
            switch( chunk_id >> 6 )
            {
                case 0:
                    /* stop processing chunk and drop it */
                    *p_ecode = E_UNKNOWN_CHUNK_ID;
                    break;

                case 1:
                    /* stop processing chunk and drop it , also return error*/
                    *p_ecode = E_UNKNOWN_CHUNK_RETURN_ERROR;
                    break;

                case 2:
                    /* skip this parameter */
                    *p_ecode = E_SKIP_CHUNK;
                    break;

                case 3:
                    /* skip this parameter but return error */
                    *p_ecode = E_SKIP_CHUNK_RETURN_ERROR;
                    break;
            }

            ret = SCTP_FAILURE;
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_valid_chunk_list
 *
 *    DESCRIPTION:
 *        This function initialises the valid chunk list array.
 *        This can be called with initialisation value of TRUE or
 *        FLASE.
 *
 *    RETURN VALUE:
 *        None
 *
 **************************************************************************/
void
sctp_init_valid_chunk_list(
    sctp_U8bit         *valid_chunks_list,
    sctp_Boolean_t     init_value )
{
    __MEMSET( valid_chunks_list, init_value, SCTP_NUM_CHUNK_TYPES );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_TLdata_chunks_and_len
 *
 *    DESCRIPTION:
 *        This function will scan the TL datagram for valid chunks.
 *        It will validate if chunk id is valid, there are number of
 *        octets present in the datagram as indicated in the chunk length
 *        field. If successful it will return number of chunks present in
 *        the datagram.
 *
 *    In-Line functions
 *        sctp-extract_chunk_id     - extract chunk id from the datagram.
 *        sctp_static_chk_chunk_id  - Is this a valid chunk Id.
 *        sctp_extract_chunk_len    - extract chunk id from the datagram.
 *        sctp_static_chk_chunk_len - Is chunk length valid for this chunk Id.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_chk_TLdata_chunks_and_len(
    sctp_U32bit           size,
    sctp_U8bit            *buffer,
    sctp_U32bit           *num_chunks,
    sctp_chunk_info_st    *chunk_id_list,
    sctp_Boolean_t        *unrecognise_chunk_present,
    sctp_U16bit           *unrecognise_chunk_len,
    sctp_U8bit            *p_unrecognise_chunk,
    sctp_error_t          *p_ecode )
{
    sctp_U8bit  chunk_id;
    sctp_U16bit chunk_len;
    sctp_U16bit chunk_offset;
    sctp_U16bit optional_pad_bytes;
    sctp_U16bit next_chunk_offset = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_chk_TLdata_chunks_and_len:Size=%u", size ) );

    *num_chunks = 0;
    chunk_offset = SCTP_OFFSET_FIRSTCHUNK;
    /* Bug Id 81 fix s */
    /*next_chunk_offset is an unused variable*/
    /*next_chunk_offset = chunk_offset; */
    /* Bug Id 81 fix e */

    for( ; ; )
    {
        if( chunk_offset == size )
        {
            break;
        }

        chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                    chunk_offset + SCTPCHUNKOFFSET_LENGTH );

        if( chunk_len <  SCTPCHUNKOFFSET_BODY )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid chunk length" ) );
            *p_ecode = E_INVALID_CHUNK_LEN;

            return ( SCTP_FAILURE );
        }

        chunk_id = sctp_buffer_get_UcharAtOffset( buffer, chunk_offset );
        optional_pad_bytes = ( sctp_U16bit )sctp_get_chunk_padd_bytes(
                                 chunk_id, chunk_len );

        next_chunk_offset = chunk_offset + chunk_len + optional_pad_bytes;

        if( next_chunk_offset > size )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_chk_TLdata_chunks_and_len: Invalid TL datagram size" ) );

            *p_ecode = E_INVALID_TL_SIZE;

            return( SCTP_FAILURE );
        }

        if( sctp_static_chk_chunk_id( chunk_id, p_ecode ) == SCTP_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_chk_TLdata_chunks_and_len: Invalid chunk,chunk Id= %d",
                          chunk_id ) );

            switch( *p_ecode )
            {
                case E_UNKNOWN_CHUNK_ID:
                    /*  return failure from here, packet to be dropped */
                    return( SCTP_FAILURE );
                    break;

                case E_UNKNOWN_CHUNK_RETURN_ERROR:
                    /* return failure and copy unknown parameters,
                       packet to be dropped */
                    *unrecognise_chunk_present = SCTP_TRUE;
                    chunk_len += ( sctp_U16bit )sctp_get_chunk_padd_bytes(
                                     chunk_id, chunk_len );

                    if( *unrecognise_chunk_len + chunk_len
                            < SCTP_MAX_UNKNOWN_CHUNK_LENGTH )
                    {
                        /*Bug Id 81 fix s*/
                        if( chunk_len <  SCTPCHUNKOFFSET_BODY )
                        {
                            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid chunk length" ) );
                            *p_ecode = E_INVALID_CHUNK_LEN;

                            return ( SCTP_FAILURE );
                        }

                        /*Bug Id 81 fix e*/

                        __MEMCOPY( p_unrecognise_chunk + *unrecognise_chunk_len,
                                   buffer + chunk_offset, chunk_len );

                        *unrecognise_chunk_len += chunk_len;
                    }

                    return ( SCTP_FAILURE );
                    break;

                case E_SKIP_CHUNK:
                    /*  skip this parameter */
                    break;

                case E_SKIP_CHUNK_RETURN_ERROR:
                    /* skip this parameter but copy in unknown
                       parameters buffer */
                    *unrecognise_chunk_present = SCTP_TRUE;
                    chunk_len += ( sctp_U16bit )
                                 sctp_get_chunk_padd_bytes( chunk_id, chunk_len );

                    if( *unrecognise_chunk_len + chunk_len
                            < SCTP_MAX_UNKNOWN_CHUNK_LENGTH )
                    {
                        /*Bug Id 81 fix s*/
                        if( chunk_len <  SCTPCHUNKOFFSET_BODY )
                        {
                            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid chunk length" ) );
                            *p_ecode = E_INVALID_CHUNK_LEN;

                            return ( SCTP_FAILURE );
                        }

                        /*Bug Id 81 fix e*/

                        __MEMCOPY( p_unrecognise_chunk + *unrecognise_chunk_len,
                                   buffer + chunk_offset, chunk_len );
                        *unrecognise_chunk_len += chunk_len;
                    }

                    break;

                default:
                    break;
            }
        }

        else
        {
            if( sctp_static_chk_chunk_len( chunk_id, chunk_len, p_ecode )
                    == SCTP_FAILURE )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_chk_TLdata_chunks_and_len: Invalid Chunk length %d",
                              chunk_len ) );

                return ( SCTP_FAILURE );
            }

            else
            {
                /*
                 * For extended chunks, chunk id can not be same as one used
                 * in the draft. So use our own defined chunk ids that range
                 * from 0 - NUM_SUPPORTED_CHUNKS.Id that's > SCTP_MAX_CHUNKID
                 * needs to be changed in stack
                 * This is reuired for supporting any extension draft
                 */
                if( chunk_id > SCTP_MAX_CHUNKID )
                {
                    /*chunk id is a valid one, */
                    chunk_id = sctp_convert_chunk_id( chunk_id );
                }

                chunk_id_list[*num_chunks].chunk_id = chunk_id;
                chunk_id_list[*num_chunks].chunk_offset = chunk_offset;
                ( *num_chunks )++;
            }
        }

        if( *num_chunks == SCTP_MAX_CHUNKS_PERDATAGRAM )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "sctp_chk_TLdata_chunks_and_len: Number of Chunks In Packet"
                          " Higher than limit !!" ) );

            break;
        }

        chunk_offset = next_chunk_offset;
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_convert_chunk_id
 *
 *    DESCRIPTION:
 *        This function returns new chunk id for extension chunks in case
 *         of sctp DYNAMIC_IP_SUPPORT.
 *
 *    RETURN VALUE:
 *       sctp_U8bit
 *
 **************************************************************************/
sctp_U8bit
sctp_convert_chunk_id(
    sctp_U8bit             chunk_id )
{

    sctp_U8bit new_id = 0;

    switch( chunk_id )
    {
            /*Only extension chunks*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        case SCTPCHUNK_TYPE_ASCONF:
            new_id = SCTPCHUNK_ASCONF;
            break;

        case SCTPCHUNK_TYPE_ASCONFACK:
            new_id = SCTPCHUNK_ASCONFACK;
            break;
#endif
    }

    return new_id;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_chunk_combination_valid
 *
 *    DESCRIPTION:
 *        This function reads all the chunks (indicated by num_of_chunks)
 *        present in chunk_id_list. It will check if the type of chunks
 *        present in the datagram form a valid combination.
 *        like DATA and INIT chunk do not form a valid combination and if
 *        they come in a single datagram, they should not be processed.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *
 **************************************************************************/
sctp_return_t
sctp_chk_chunk_combination_valid(
    sctp_U32bit             num_of_chunks,
    sctp_chunk_info_st      *chunk_id_list,
    sctp_error_t            *p_ecode )
{
    sctp_U32bit sctp_index;
    sctp_U8bit  chunk_id;
    sctp_U8bit  valid_chunks_list[SCTP_NUM_CHUNK_TYPES];

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_chk_chunk_combination_valid" ) );

    sctp_init_valid_chunk_list( &valid_chunks_list[0], SCTP_TRUE );

    for( sctp_index = 0; sctp_index < num_of_chunks; sctp_index++ )
    {
        /* chunk Id's should be valid when this is called */

        chunk_id = chunk_id_list[sctp_index].chunk_id;

        if( chunk_id > SCTP_MAX_CHUNKID )
        {
            *p_ecode = E_INVALID_CHUNKID;
            continue;
        }

        if( *( &valid_chunks_list[0] + chunk_id ) == SCTP_FALSE )
        {
            *p_ecode = E_INVALID_CHUNK_COMBINATION;
            return ( SCTP_FAILURE );
        }

        /* update valid chunk structure */
        switch( chunk_id )
        {
            case SCTPCHUNK_INIT:
                /* This should be the only chunk, nothing else expected */

                sctp_init_valid_chunk_list( &valid_chunks_list[0], SCTP_FALSE );
                break;

            case SCTPCHUNK_INITACK:
                /* This should be the only chunk, nothing else expected */
                sctp_init_valid_chunk_list( &valid_chunks_list[0], SCTP_FALSE );
                break;

            case SCTPCHUNK_SHUTDOWNCOMP:
                /* This should be the only chunk, nothing else expected */
                sctp_init_valid_chunk_list( &valid_chunks_list[0], SCTP_FALSE );
                break;

            case SCTPCHUNK_COOKIE:
                /* This should be the first chunk and may have data with it */
                sctp_init_valid_chunk_list( &valid_chunks_list[0],  SCTP_FALSE );
                *( &valid_chunks_list[0] + SCTPCHUNK_DATA ) = SCTP_TRUE;
                *( &valid_chunks_list[0] + SCTPCHUNK_ABORT ) = SCTP_TRUE;
                *( &valid_chunks_list[0] + SCTPCHUNK_ERROR ) = SCTP_TRUE;
                break;

            case SCTPCHUNK_COOKIEACK:
                /* This should be the first chunk and may have data and/or Error with it */
                sctp_init_valid_chunk_list( &valid_chunks_list[0],  SCTP_FALSE );
                *( &valid_chunks_list[0] + SCTPCHUNK_DATA ) = SCTP_TRUE;
                *( &valid_chunks_list[0] + SCTPCHUNK_SACK ) = SCTP_TRUE;
                *( &valid_chunks_list[0] + SCTPCHUNK_ABORT ) = SCTP_TRUE;
                *( &valid_chunks_list[0] + SCTPCHUNK_HEARTBEAT ) = SCTP_TRUE;
                *( &valid_chunks_list[0] + SCTPCHUNK_ERROR ) = SCTP_TRUE;
                break;

            case SCTPCHUNK_DATA:
                /* Only Data chunk may come after this chunk */
                sctp_init_valid_chunk_list( &valid_chunks_list[0],  SCTP_FALSE );
                *( &valid_chunks_list[0] + SCTPCHUNK_DATA ) = SCTP_TRUE;
                break;

            case SCTPCHUNK_ABORT:
                *( &valid_chunks_list[0] + SCTPCHUNK_INIT ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_INITACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIE )  = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIEACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_SHUTDOWNCOMP ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_DATA ) = SCTP_FALSE;
                break;

            case SCTPCHUNK_SHUTDOWN:
            case SCTPCHUNK_SHUTDOWNACK:
                *( &valid_chunks_list[0] + SCTPCHUNK_DATA ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_INIT )    = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_INITACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIE )  = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIEACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_SHUTDOWNCOMP ) = SCTP_FALSE;
                break;

            case SCTPCHUNK_ERROR:
            case SCTPCHUNK_SACK:
            case SCTPCHUNK_HEARTBEAT:
            case SCTPCHUNK_HEARTBEATACK:
            case SCTPCHUNK_ECNE:
            case SCTPCHUNK_CWR:
                *( &valid_chunks_list[0] + SCTPCHUNK_INIT )    = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_INITACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIE )  = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIEACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_SHUTDOWNCOMP ) = SCTP_FALSE;
                break;

#ifdef SCTP_DYNAMIC_IP_SUPPORT

            case SCTPCHUNK_ASCONF:
            case SCTPCHUNK_ASCONFACK:
                *( &valid_chunks_list[0] + SCTPCHUNK_INIT )    = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_INITACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIE )  = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_COOKIEACK ) = SCTP_FALSE;
                *( &valid_chunks_list[0] + SCTPCHUNK_SHUTDOWNCOMP ) = SCTP_FALSE;
                /*two asconf, asconf-ack can not be bundled */
                *( &valid_chunks_list[0] + chunk_id ) = SCTP_FALSE;
                break;

#endif

            default:
                *p_ecode = E_INVALID_CHUNKID;
        }

    } /* For Loop */

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *      sctp_parse_packet_header
 *
 *    DESCRIPTION:
 *       This functions performs checking on the SCTP datagram length.
         Also checks to disallow the usage port number 0

 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS
 *       SCTP_FAILURE
 *
 **************************************************************************/
sctp_return_t
sctp_parse_packet_header(
    sctp_U8bit          *buffer,
    sctp_U32bit         size,
    sctp_packet_st      *p_packet,
    sctp_error_t        *ecode )
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ecode );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_parse_packet_header :size= %u", size ) );

    /* Check if data received is of minimum length expected. */
    if( size < SCTP_MIN_TLDATAGRAM_SIZE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_packet_header: Invalid TL datagram Size" ) );

        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_parse_packet_header: Incoming TL data has "
                      "unexpected length" ) );

        return ( SCTP_FAILURE );
    }

    p_packet->dst_port = sctp_buffer_get_UshortAtOffset( buffer,
                                                         SCTP_OFFSET_DSTPORT );
    p_packet->v_tag    = sctp_buffer_get_UlongAtOffset( buffer,
                                                        SCTP_OFFSET_VERTAG );
    p_packet->src_port = sctp_buffer_get_UshortAtOffset( buffer,
                                                         SCTP_OFFSET_SRCPORT );

    /* Add check to disallow port number 0 being used */
    if( ( p_packet->src_port == 0 ) || ( p_packet->dst_port == 0 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_parse_chk_TL_datagram: Zero Port Number being used in SCTP" ) );

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *       sctp_parse_chk_TL_datagram
 *
 *    DESCRIPTION:
 *       This functions is called from main module. This performs
 *       static checking on the SCTP datagram. One SCTP datagram can
 *       have multiple chunks in it.Checks are performed on all
 *       the chunks. Also only some of the chunk combinations are valid.
 *       Check is performed to see if the incoming chunk sequence is valid.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS
 *       SCTP_FAILURE
 *
 **************************************************************************/
sctp_return_t
sctp_parse_chk_TL_datagram(
    sctp_U8bit          *buffer,
    sctp_U32bit         size,
    sctp_tcb_st         *p_ass,
    sctp_packet_st      *p_packet,
    sctp_U32bit         *num_of_chunks,
    sctp_chunk_info_st  *chunk_id_list,
    sctp_Boolean_t      *unrecognise_chunk_present,
    sctp_U16bit         *unrecognise_chunk_len,
    sctp_U8bit          *p_unrecognise_chunk,
    sctp_Boolean_t      resolver_flag,
    sctp_error_t        *ecode )
{
    sctp_U32bit   checksum;
    sctp_U32bit   computed;
    sctp_checksum_et cksm_func;
    sctp_return_t ret;

    /* validate checksum */
    if( p_ass )
    {
        cksm_func = p_ass->config.checksum_func;
    }

    else
    {
        sctp_ep_st   *p_ep = SCTP_NULL;
        sctp_U8bit chunk_id =
            sctp_buffer_get_UcharAtOffset( buffer, SCTP_OFFSET_FIRSTCHUNK );

        cksm_func = sctp_db_main.sctp_def_prot_params.checksum_func;

        if( ( SCTPCHUNK_INIT == chunk_id ) ||
                ( SCTPCHUNK_COOKIE  == chunk_id ) )
        {
            /*Bug Id 13 fix starts */
            /*sctp_db_locate_server(p_packet->dst_port,
              &p_packet->p_tl_packet->dst_addr, &p_ep);*/
            ret = sctp_db_locate_server( p_packet->dst_port,
                                         &p_packet->p_tl_packet->dst_addr, &p_ep );

            if( ret != SCTP_SUCCESS )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_parse_chk_TL_datagram: No Server, message dropped" ) );
                return SCTP_FAILURE;
            }

            /*Bug Id 13 fix ends */


            if( SCTP_NULL != p_ep )
            {
                cksm_func = p_ep->sk_config.checksum_func;
            }
        }
    }

    /* validate checksum */
    checksum = sctp_buffer_get_UlongAtOffset( buffer, SCTP_OFFSET_CKSUM );
    sctp_buffer_set_UlongAtOffset( buffer, SCTP_OFFSET_CKSUM, 0UL );
    computed = sctp_compute_cksm[cksm_func]( buffer, size );

    p_packet->checksum = cksm_func;

    if( ( computed != checksum ) && ( resolver_flag == SCTP_FALSE ) )
    {
        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_CHK_SUM_ERR_RECV, 1 );
        /* Invalid packet - discard */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_parse_chk_TL_datagram: Checksum failed" ) );

        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_parse_chk_TL_datagram: checksum failed" ) );

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_parse_chk_TL_datagram:size= %u", size ) );

    ret = sctp_chk_TLdata_chunks_and_len( size,
                                          buffer,
                                          num_of_chunks,
                                          &chunk_id_list[0],
                                          unrecognise_chunk_present,
                                          unrecognise_chunk_len,
                                          p_unrecognise_chunk,
                                          ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATACHUNKS_RECV_ERROR, 1 );

        switch( *ecode )
        {
            case E_UNKNOWN_CHUNK_ID:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_parse_chk_TL_datagram: some of chunks may be "
                              "dropped !!!" ) );
                break;

            case E_UNKNOWN_CHUNK_RETURN_ERROR:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_parse_chk_TL_datagram: Send Error, Unknown chunk"
                              " type received !!!" ) );
                break;

            default:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC || SCTP_ERR_TRC, \
                            ( "sctp_parse_chk_TL_datagram: InValid TL Datagram" ) );

                return ( SCTP_FAILURE );
                break;
        }
    }

    ret = sctp_chk_chunk_combination_valid( *num_of_chunks,
                                            &chunk_id_list[0], ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC || SCTP_ERR_TRC,
                    ( "sctp_parse_chk_TL_datagram: InValid Chunk Combination " ) );

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


#ifdef SCTP_DYNAMIC_IP_SUPPORT

static sctp_U8bit *
sctp_parse_chk_asconf_param(
    sctp_U8bit              *p_buf,
    sctp_U16bit             *p_chunk_len,
    sctp_asconf_param_ut    *p_asconf_param,
    sctp_U32bit             *unrecognise_param_len,
    sctp_U8bit              *p_unrecognise_param,
    sctp_error_t            *p_ecode );

/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_addr_param
 *
 *    DESCRIPTION:
 *       This function parses the address parameters.
 *        checks where applicable.
 *
 *    RETURN VALUE:
 *        pointer to next parameter
 *        NULL
 *
 **************************************************************************/
sctp_U8bit *
sctp_parse_chk_addr_param(
    sctp_U8bit            *p_buf,
    sctp_U16bit           chunk_len,
    sctp_sockaddr_st      *p_addr,
    sctp_error_t          *p_ecode )
{
    sctp_U16bit param_type, param_len;

    /*read the address parameter */
    p_buf = sctp_buffer_get_Ushort( p_buf, &param_type );
    p_buf = sctp_buffer_get_Ushort( p_buf, &param_len );

    if( chunk_len < param_len )
    {
        *p_ecode = E_INVALID_CHUNK_SIZE;
        return SCTP_NULL;
    }

    switch( param_type )
    {

        case SCTPVARPAR_IPV4ADDRESS:
            if( param_len != SCTPPARAM_LEN_IPV4ADDRESS )
            {
                *p_ecode = E_INVALID_PARAMETER_LEN;
                return SCTP_NULL;
            }

            p_buf = sctp_buffer_get_Ulong( p_buf, &( p_addr->ip.v4.addr ) );

            /* Set the Address family to AF_INET */
            p_addr->family = SCTP_AF_INET;

            if( sctp_chk_addr_unicast( p_addr ) == SCTP_FAILURE )
            {
                *p_ecode = E_UNSUPPORTED_ADDR;

                return ( SCTP_NULL );
            }

            break;

        case SCTPVARPAR_IPV6ADDRESS:
            if( param_len != SCTPPARAM_LEN_IPV6ADDRESS )
            {
                *p_ecode = E_INVALID_PARAMETER_LEN;
                return ( SCTP_NULL );
            }

            /* Extract the IPv6 p_msg->src_addr from the buffer */
            p_buf = sctp_buffer_get_Ulong( p_buf,
                                           ( sctp_U32bit * ) & ( p_addr->ip.v6.addr.s_u6_addr32[0] ) );

            p_buf = sctp_buffer_get_Ulong( p_buf,
                                           ( sctp_U32bit * ) & ( p_addr->ip.v6.addr.s_u6_addr32[1] ) );

            p_buf = sctp_buffer_get_Ulong( p_buf,
                                           ( sctp_U32bit * ) & ( p_addr->ip.v6.addr.s_u6_addr32[2] ) );

            p_buf = sctp_buffer_get_Ulong( p_buf,
                                           ( sctp_U32bit * ) & ( p_addr->ip.v6.addr.s_u6_addr32[3] ) );

            /* Set the flow info and the scope id. */
            p_addr->ip.v6.flow_info = 0;
            p_addr->ip.v6.scope_id  = SCTP_IPV6_SCOPE_LINK_LOCAL;

            /* Set the p_msg->src_addr family to AF_INET */
            p_addr->family = SCTP_AF_INET6;

            /*
            * Check That the IPV6 p_msg->src_addr is a unicast p_addr-> i.e. not
            * a broadcast or unicast p_addr->
            */
            if( sctp_chk_addr_unicast( p_addr ) == SCTP_FAILURE )
            {
                *p_ecode = E_UNSUPPORTED_ADDR;

                return ( SCTP_NULL );
            }

            break;

        case SCTPVARPAR_HOSTNAME:
            p_buf += ( param_len - SCTPPARAMOFFSET_BODY ) +
                     sctp_get_parameter_pad_bytes( param_len );
            break;

        default:
            *p_ecode = E_INVALID_ADDR;
            return SCTP_NULL;
    }

    return p_buf;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_asconf
 *
 *    DESCRIPTION:
 *         This function parses asconf chunk.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_asconf(
    sctp_U8bit            *buffer,
    sctp_U32bit           chunk_offset,
    sctp_addr_conf_st     *p_msg,
    sctp_error_t          *p_ecode )
{
    sctp_U16bit chunk_len;
    sctp_U32bit sctp_index = 0;
    sctp_U8bit  *p_buf = buffer + chunk_offset + SCTPCHUNKOFFSET_LENGTH;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_asconf" ) );

    p_buf = sctp_buffer_get_Ushort( p_buf, &chunk_len );
    chunk_len -= SCTPCHUNKOFFSET_BODY;

    /*already checked for serial number and ipv4 addr size*/
    p_buf = sctp_buffer_get_Ulong( p_buf, &( p_msg->serial_no ) );
    chunk_len -= 4;
    /*read the address parameter */
    p_buf = sctp_parse_chk_addr_param( p_buf, chunk_len,
                                       &p_msg->src_addr, p_ecode );

    if( !p_buf )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "invalid address param" ) );
        return SCTP_FAILURE;
    }

    chunk_len -= ( p_msg->src_addr.family == SCTP_AF_INET ) ? 8 : 20;

    /* now read the asconf parameters */
    while( chunk_len )
    {
        p_buf = sctp_parse_chk_asconf_param( p_buf, &chunk_len,
                                             &p_msg->asconf_params[sctp_index], &p_msg->unreco_param_len,
                                             p_msg->p_unreco_params, p_ecode );

        if( SCTP_NULL == p_buf )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "invalid asconf param" ) );
            return SCTP_FAILURE;
        }

        sctp_index++;
    }

    p_msg->num_params = sctp_index;

    if( sctp_index == 0 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "missing asconf param" ) );
        *p_ecode = E_MISSING_PARAMS;
        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_asconf_param
 *
 *    DESCRIPTION:
 *        This function will parse asconf param and perform static
 *        checks where applicable.
 *
 *    RETURN VALUE:
 *        pointer to next parameter
 *        NULL
 *
 **************************************************************************/
sctp_U8bit *
sctp_parse_chk_asconf_param(
    sctp_U8bit              *p_buf,
    sctp_U16bit             *p_chunk_len,
    sctp_asconf_param_ut    *p_asconf_param,
    sctp_U32bit             *unrecognise_param_len,
    sctp_U8bit              *p_unrecognise_param,
    sctp_error_t            *p_ecode )
{
    sctp_U16bit chunk_len = *p_chunk_len;
    sctp_U16bit param_type,  param_len;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_asconf_param" ) );

    if( chunk_len < SCTPPARAMOFFSET_BODY )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "invalid length" ) );
        /*end of chunk, missing ASConf parameter*/
        *p_ecode = E_INVALID_CHUNK_SIZE;
        return( SCTP_NULL );
    }

    p_buf = sctp_buffer_get_Ushort( p_buf, &param_type );
    p_buf = sctp_buffer_get_Ushort( p_buf, &param_len );

    if( chunk_len < param_len )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "invalid length" ) );
        /*end of chunk, missing ASConf parameter*/
        *p_ecode = E_INVALID_CHUNK_SIZE;
        return( SCTP_NULL );
    }

    switch( param_type )
    {
        case SCTP_ASCONF_PARAM_ADD_IP:
        case SCTP_ASCONF_PARAM_DEL_IP:
        case SCTP_ASCONF_PARAM_SET_PRIM:
            {
                sctp_asconf_addip_param_st  *p_param =
                    &( p_asconf_param->add_ip );

                /*there has to be a minimum of 4 + 4 + 8 bytes*/
                if( param_len < 16 )
                {
                    *p_ecode = E_INVALID_PARAMETER_LEN;
                    return SCTP_NULL;
                }

                p_buf = sctp_buffer_get_Ulong( p_buf, &( p_param->corr_id ) );

                /*read the IP address*/
                p_buf = sctp_parse_chk_addr_param( p_buf, param_len,
                                                   &( p_param->ipaddr ), p_ecode );

                if( !p_buf )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "chk_addr_param failed" ) );
                    return SCTP_NULL;
                }
            }
            break;

        default:
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "unknown parameter %u",
                                                   param_type ) );

            switch( param_type >> 14 )
            {
                case 0:
                    /* stop processing chunk and drop it */
                    *p_ecode = E_UNKNOWN_PARAMETER_ID;
                    return SCTP_NULL;
                    break;

                case 1:
                    /* stop processing chunk and drop it */
                    param_len += sctp_get_parameter_pad_bytes(
                                     param_len );

                    if( *unrecognise_param_len + param_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_unrecognise_param + *unrecognise_param_len,
                                   p_buf - SCTPPARAMOFFSET_BODY, param_len );
                        *unrecognise_param_len += param_len;
                    }

                    *p_ecode = E_UNKNOWN_PARAMETER_RETURN_ERROR;
                    return SCTP_NULL;
                    break;

                case 2:
                    /* skip this parameter */
                    *p_ecode = E_SKIP_PARAMETER;
                    break;

                case 3:
                    param_len += sctp_get_parameter_pad_bytes(
                                     param_len );

                    if( *unrecognise_param_len + param_len
                            < SCTP_MAX_OPT_PARAMS_TOTAL_SIZE )
                    {
                        __MEMCOPY( p_unrecognise_param + *unrecognise_param_len,
                                   p_buf - SCTPPARAMOFFSET_BODY, param_len );
                        *unrecognise_param_len += param_len;
                    }

                    *p_ecode = E_SKIP_PARAMETER_RETURN_ERROR;
                    break;
            }

            p_buf += ( param_len - SCTPPARAMOFFSET_BODY );
            break;
    }

    p_asconf_param->param_type = param_type;
    chunk_len   -= param_len;
    *p_chunk_len = chunk_len;
    return p_buf;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_parse_chk_chunk_asconfack
 *
 *    DESCRIPTION:
 *        This function will parse asconf ack chunk and perform static
 *        checks where applicable.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_parse_chk_chunk_asconfack(
    sctp_U8bit            *buffer,
    sctp_U32bit           chunk_offset,
    sctp_addr_conf_ack_st *p_msg,
    sctp_error_t          *p_ecode )
{
    sctp_U16bit chunk_len;
    sctp_U32bit sctp_index = 0;
    sctp_U16bit param_type,  param_len;
    sctp_U8bit  *p_buf = buffer + chunk_offset + SCTPCHUNKOFFSET_LENGTH;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_parse_chk_chunk_asconfack" ) );

    p_buf = sctp_buffer_get_Ushort( p_buf, &chunk_len );
    chunk_len -= SCTPCHUNKOFFSET_BODY;

    /*already checked for serial number and ipv4 addr size*/
    p_buf = sctp_buffer_get_Ulong( p_buf, &( p_msg->serial_no ) );
    chunk_len -= SCTPPARAMOFFSET_BODY;

    /*now read the asconfack parameters*/
    while( chunk_len )
    {
        if( chunk_len < SCTPPARAMOFFSET_BODY )
        {
            /*end of chunk, missing ASConf parameter*/
            *p_ecode = E_INVALID_CHUNK_SIZE;
            return( SCTP_FAILURE );
        }

        p_buf = sctp_buffer_get_Ushort( p_buf, &param_type );
        p_buf = sctp_buffer_get_Ushort( p_buf, &param_len );

        /*SPR 5223*/
        if( chunk_len < ( param_len + sctp_get_parameter_pad_bytes( param_len ) ) )
            /*SPR 5223*/
        {
            /*end of chunk, missing ASConf parameter*/
            *p_ecode = E_INVALID_CHUNK_SIZE;
            return( SCTP_FAILURE );
        }

        if( SCTP_MAX_PARAMS_PER_ASCONF == sctp_index )
        {
            /*already too many parameter, can't process further*/
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_parse_chk_chunk_asconfack:too many params,"
                          " malacious peer : otherwise not possible" ) );
            break;
        }

        switch( param_type )
        {
            case SCTP_ASCONF_PARAM_ERR_CAUSE:
                {
                    sctp_U16bit tmpU16;

                    if( param_len < 8 )
                    {
                        *p_ecode = E_INVALID_PARAMETER_LEN;
                        return SCTP_FAILURE;
                    }

                    p_buf = sctp_buffer_get_Ulong( p_buf,
                                                   &( p_msg->asconf_params[sctp_index].corr_id ) );
                    p_buf = sctp_buffer_get_Ushort( p_buf,
                                                    &( p_msg->asconf_params[sctp_index].err_cause.cause_code ) );

                    p_buf = sctp_buffer_get_Ushort( p_buf, &tmpU16 );

                    /*SPR 5223*/
                    if( ( tmpU16 + SCTPPARAMOFFSET_BODY + 4 ) !=  param_len )
                    {
                        *p_ecode = E_INVALID_PARAMETER_LEN;
                        return SCTP_FAILURE;
                    }

                    /*SPR 5223*/
                    p_msg->asconf_params[sctp_index].err_cause.cause_len = tmpU16;
                    p_buf += ( tmpU16 - SCTPPARAMOFFSET_BODY );
                }
                break;

            case SCTP_ASCONF_PARAM_SUCCESS_REPORT:
                if( param_len != 8 )
                {
                    *p_ecode = E_INVALID_PARAMETER_LEN;
                    return SCTP_FAILURE;
                }

                p_buf = sctp_buffer_get_Ulong( p_buf,
                                               &( p_msg->asconf_params[sctp_index].corr_id ) );
                break;

            default:
                /*unknown parameter in ack, just skip*/
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "unknown parameter %u",
                                                       param_type ) );

                if( 0 == param_len )
                {
                    *p_ecode = E_INVALID_PARAMETER_LEN;
                    return SCTP_FAILURE;
                }

                p_buf += ( param_len - SCTPPARAMOFFSET_BODY );
                param_type = 0; /*unknown param*/
                break;
        }

        /*SPR 5223*/
        p_buf += sctp_get_parameter_pad_bytes( param_len );

        if( 0 != param_type )
        {
            p_msg->asconf_params[sctp_index++].param_type = param_type;
        }

        chunk_len -= ( param_len + sctp_get_parameter_pad_bytes( param_len ) );
        /*SPR 5223*/
    }

    p_msg->num_params = sctp_index;
    return ( SCTP_SUCCESS );
}


#endif
