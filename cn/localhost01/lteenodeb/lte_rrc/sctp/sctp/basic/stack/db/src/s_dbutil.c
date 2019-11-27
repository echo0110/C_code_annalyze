/***************************************************************************
 *  FILE NAME  : s_dbutil.c
 *
 *  DESCRIPTION: Contains the utility functions for Database access.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  20May02    ygahlaut     SPR 4073 4035 Fix
 *  22Nov02    mrajpal      SPR 5085     Address Available Notification
 *  22Nov02    mrajpal      SPR 5086     adaption indication
 *  28Aug03    mrajpal                   Rel 5.0
 *  14Oct03    vibansal                  Changes for mixed
 *                                       IPv4-v6 support
 *  29Apr04    mrajpal                   SPR 6751
 *  09May08    Raza          REL 6.2     SPR 18527
 *  18Feb13    Rohan Pawa   SPR 21107    CSR 24680
 *  10Oct13    Rohan Pawa   SPR 21155    CSR 56067
 *  25Oct13    Pinky Sinha  SPR 21157    CSR 56258
 *  15Sep14   Priyanka Mishra   Bug Id 22     CSR 85223
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "DB"

#include <s_cotyps.h>
#include <s_comac.h>
#include <s_dbbuf.h>
#include <s_dbutil.h>
#include <s_dbtcb.h>
#include <s_dbg.h>
#include <s_uisuif.h>
#include <s_select.h>
#include <s_ostmr.h>
#include <s_osutil.h>
#include <s_stats.h>
#include <s_fsmtx.h>
#include <s_osmem.h>


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_update_src_addr_index
 *
 *    DESCRIPTION:
 *        This functions updates the local address index to next available
 *        address.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_db_update_src_addr_index(
    sctp_tcb_st         *p_ass,
    sctp_U32bit         addr_index )

{
    sctp_U32bit saddr_index, count, count1;
    sctp_remote_addr_param_st *p_peer_addr =
        &p_ass->transmit.remote[addr_index];

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_db_update_src_addr_index" ) );

    /* This function is called after a timeout and hence
     * path_error_count should never be zero
     */
    if( p_peer_addr->path_error_count %  p_peer_addr->path_error_threshold
            == 0 )
    {
        /* We will not consider this Address for
         * Path Verification Probing */
        p_peer_addr->perf_path_verf = SCTP_FALSE;

        saddr_index = p_peer_addr->src_addr_index;

        for( count = 0; count < ( sctp_U32bit )( p_ass->p_ep->naddr - 1 ); count++ )
        {
            saddr_index = ( saddr_index + 1 ) % p_ass->p_ep->naddr;

            if( p_ass->p_ep->this_address[saddr_index].family !=
                    p_peer_addr->that_address.family )
            {
                /*Incompatible Family, can't use*/
                continue;
            }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

            if( SCTP_NULL_TIMER_ID !=
                    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] )
            {
                sctp_U32bit count2;

                /* An asconf in progress, check if any requests for delete */
                for( count2 = 0; count2 < p_ass->cmd_asconf.num_params; count2++ )
                {
                    sctp_asconf_delip_param_st *p_delip =
                        &( p_ass->cmd_asconf.asconf_params[count2].del_ip );

                    if( ( SCTP_ASCONF_PARAM_DEL_IP ==
                            p_ass->cmd_asconf.asconf_params[count2].param_type ) ||
                            ( SCTP_ASCONF_PARAM_ADD_IP ==
                              p_ass->cmd_asconf.asconf_params[count2].param_type ) )
                    {
                        /* Check that if ABORT has come for this address */
                        if( SCTP_SUCCESS == sctp_cmp_addr( &p_delip->ipaddr,
                                                           &p_ass->p_ep->this_address[saddr_index] ) )
                        {
                            /*Don't do anything, */
                            break;
                        }
                    }
                }

                if( count2 < p_ass->cmd_asconf.num_params )
                {
                    continue;
                }
            }

#endif
            /* Check that this source address is not choosen by any
             * ACTIVE Address
             */

            for( count1 = 0; count1 < p_ass->transmit.num_remote_address;
                    count1++ )
            {
                if( ( p_ass->transmit.remote[count1].status
                        == SCTP_COMMUNICATION_UP )
                        && ( p_ass->transmit.remote[count1].src_addr_index
                             == saddr_index ) )
                {
                    /* Source address is used by an active address,
                     * can't choose this
                     */
                    break;
                }
            }

            if( count1 == p_ass->transmit.num_remote_address )
            {
                /* found an address that can be used, update and return */
                p_peer_addr->src_addr_index = saddr_index;

                /* The PATH has been changed.
                 * i.e. if earlier the PATH was b/w Dest Addr = A and Src Addr = X,
                 * now the New Path is b/w Dest Addr = A and Src Addr = Y
                 */
                p_peer_addr->perf_path_verf = SCTP_TRUE;
                return;
            }
        } /* for loop */
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_reset_error_counters
 *
 *    DESCRIPTION:
 *        Resets the destination address error counters.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_db_reset_error_counters(
    sctp_tcb_st     *p_ass,
    sctp_U32bit     addr_index )
{
    sctp_U32bit   sctp_index;

    p_ass->transmit.remote[addr_index].path_error_count = 0;
    p_ass->error_count = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_db_reset_error_counters" ) );

    /* If the remote address is in Inactive state and is confirmed, then
     * set it in active state.
     *
     * NOTE :
     * A misbehaving Peer may fill an "Invalid Address" in the SRC Address
     * of the IP Header. Hence it is necessary to check if this Address has
     * been marked confirmed previously before setting Active state for it.
     *
     * Invalid Address here means a Address sent by Peer in Address list of
     * INIT or INIT-ACK which doesn't belong to the Peer.
     */

    if( p_ass->transmit.remote[addr_index].status == SCTP_COMMUNICATION_LOST &&
            p_ass->transmit.remote[addr_index].is_confirmed == SCTP_TRUE )
    {
        p_ass->transmit.remote[addr_index].status = SCTP_COMMUNICATION_UP;

        if( p_ass->conn_accepted == SCTP_TRUE )
        {
            /*SPR 5085*/
            /* notify service user */
            sctp_ntfy_dst_status_change( p_ass,
                                         &p_ass->transmit.remote[addr_index].that_address,
                                         SCTP_ADDRESS_AVAILABLE, E_NOERROR );
            /*SPR 5085*/
        }

        sctp_index = p_ass->transmit.primary_address_index;

        if( p_ass->transmit.configured_primary_path_index == addr_index ||
                p_ass->transmit.remote[sctp_index].status == SCTP_COMMUNICATION_LOST )
        {
            p_ass->transmit.primary_address_index = addr_index;
            /* Changes For CSR#1-5037386 */
            sctp_move_data_to_primary_addr_from_prev( p_ass, sctp_index, addr_index );
            /* Changes For CSR#1-5037386 Ends */
        }
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_cmp_addr
 *
 *    DESCRIPTION:
 *        Compares the two addresses passed for equality.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS on exact match else SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_cmp_addr(
    sctp_sockaddr_st  *p_a1,
    sctp_sockaddr_st  *p_a2 )
{
    if( p_a1->family != p_a2->family )
    {
        return ( SCTP_FAILURE );
    }

    switch( p_a1->family )
    {
        case SCTP_AF_INET:
            if( p_a1->ip.v4.addr == p_a2->ip.v4.addr )
            {
                return ( SCTP_SUCCESS );
            }

            break;

        case SCTP_AF_INET6:
            if( ( p_a1->ip.v6.addr.s_u6_addr32[0] == p_a2->ip.v6.addr.s_u6_addr32[0] )
                    && ( p_a1->ip.v6.addr.s_u6_addr32[1] == p_a2->ip.v6.addr.s_u6_addr32[1] )
                    && ( p_a1->ip.v6.addr.s_u6_addr32[2] == p_a2->ip.v6.addr.s_u6_addr32[2] )
                    && ( p_a1->ip.v6.addr.s_u6_addr32[3] == p_a2->ip.v6.addr.s_u6_addr32[3] ) )
            {
                /* Write now no check for flow info and scope. */
                return ( SCTP_SUCCESS );
            }

            break;

        default:
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid Address family\n" ) );
            break;
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_addrs_not_present
 *
 *    DESCRIPTION:
 *        Compares the two addresses passed for equality.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS on exact match else SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_db_get_addrs_not_present(
    sctp_tcb_st      *p_ass,
    sctp_U32bit       *p_new_naddr,
    sctp_sockaddr_st  *p_new_list,
    sctp_U32bit       naddr1,
    sctp_sockaddr_st  *p_addr_list1,
    sctp_U32bit       naddr2,
    sctp_sockaddr_st  *p_addr_list2,
    sctp_error_t      *p_err )
{
    sctp_U32bit     counter, counter1;
    sctp_suppaddr_st supp_addr_type;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_db_get_addrs_not_present" ) );

    *p_new_naddr = 0;

    sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr_type );

    for( counter = 0; counter < naddr2; counter++ )
    {
        /*
         * Ignore the address type that the local endpoint does not support.
         */
        if( ( ( p_addr_list2[counter].family == SCTP_AF_INET6 ) && ( supp_addr_type.v6 != SCTP_TRUE ) ) ||
                ( ( p_addr_list2[counter].family == SCTP_AF_INET )  && ( supp_addr_type.v4 != SCTP_TRUE ) ) )
        {
            continue;
        }

        for( counter1 = 0; counter1 < naddr1; counter1++ )
        {
            /*
             * Ignore the address type that the local endpoint does not support.
             */
            if( ( ( p_addr_list1[counter1].family == SCTP_AF_INET6 ) && ( supp_addr_type.v6 != SCTP_TRUE ) ) ||
                    ( ( p_addr_list1[counter1].family == SCTP_AF_INET )  && ( supp_addr_type.v4 != SCTP_TRUE ) ) )
            {
                continue;
            }

            if( sctp_cmp_addr( &p_addr_list2[counter], &p_addr_list1[counter1] )
                    == SCTP_SUCCESS )
            {
                break;
            }
        }

        if( counter1 == naddr1 )
        {
            p_new_list[( *p_new_naddr )] = p_addr_list2[counter];
            ( *p_new_naddr )++;
        }
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_db_get_addrs_not_present: number of addresses found = %u",
                  *p_new_naddr ) );

    if( ( *p_new_naddr ) == 0 )
    {
        *p_err = EINV_ADDRS;

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/*
 * Functions that handle the notification processing.
 * These functions are called to send notifications to the ULP.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_operror
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_OPERROR notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_operror(
    sctp_tcb_st    *p_tcb,
    sctp_U16bit    e_code,
    sctp_U16bit    e_length,
    sctp_Pvoid_t   sys_err_info )
{

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_operror" ) );

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */

    /*
     * Set the Notifications into the notification structure in the
     * association.
     */
    p_tcb->notify.operror.assoc_id = p_tcb->p_ep->ep_id;
    p_tcb->notify.operror.err_code = e_code;
    p_tcb->notify.operror.err_length = e_length;

    /* SPR : 18527 */
    if( sys_err_info != SCTP_NULL )
    {
        /* SPR : 18527 */
        p_tcb->notify.operror.sys_err_info = sys_err_info;
    }

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_OPERROR );

#else
    /* The sctp socket interface is being used */

    /* SPR 4061 */
    /* First check if this notification type is enabled in the endpoint */
    if( SCTP_SOCK_IS_RECVPEERERR_ON( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_operror: SCTPNOTIFY_OPERROR"
                          "/PeerError: No Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_remote_error.sre_type  = SCTP_REMOTE_ERROR;

        /* flags are currently unused */
        p_new->notification.sn_remote_error.sre_flags = 0;

        /* The assoc id :used only in case of UDP style */
        p_new->notification.sn_remote_error.sre_assoc_id = p_tcb->assoc_id;

        /* The particular event that happened */
        p_new->notification.sn_remote_error.sre_error = e_code;

        /*
         * Total length of the notification data: including the notification
         * header.
         */
        e_length = ( e_length > SCTP_MAX_NOTF_DATA ) ? SCTP_MAX_NOTF_DATA : e_length;

        p_new->notification.sn_remote_error.sre_length =
            sizeof( sctp_remote_error_st ) - SCTP_MAX_NOTF_DATA + e_length;

        /* SPR : 18527 */
        if( sys_err_info != SCTP_NULL )
        {
            /* SPR : 18527 */
            /* Error info data */
            __MEMCOPY( p_new->notification.sn_remote_error.sre_data,
                       sys_err_info, e_length );
        }

        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );
        /* Set the Error happened flag for the endpoint:
           This is used in select */
        /* Set the socket as error happend */
        SCTP_SOCK_SET_SK_ERROR( p_tcb->p_ep->flags, SCTP_TRUE );

        /* If the endpoint is selected invoke select processing */
        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }

        /*SPR 5048*/
        SCTP_WAKEUP( p_tcb->p_ep->cond );
        /*SPR 5048*/
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_operror: SCTPNOTIFY_OPERROR"
                      "/Peer Error notificaiton not enabled\n" ) );
    }

#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_dst_status_change
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_DESTSTATCHANGE Notification.
 *    e_code -> this is the error code that is sent to the upper layer when
 *    this notification is give. This can be any valid error code such as
 *    ADDRESS_UNREACHABLE.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_dst_status_change(
    sctp_tcb_st            *p_tcb,
    sctp_sockaddr_st       *p_dst_addr,
    sctp_sock_spc_event_et status,
    sctp_U32bit            e_code )
{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( e_code );
    /*to avoid the warning*/


    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_dst_status_change" ) );

    if( status == SCTP_ADDRESS_AVAILABLE )
    {
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_DEST_ACTIVE, 1 );
    }

    else if( status == SCTP_ADDRESS_UNREACHABLE )
    {
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_DEST_INACTIVE, 1 );
    }

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.dststatchange.assoc_id =  p_tcb->p_ep->ep_id;
    p_tcb->notify.dststatchange.destination_ip_addr = *p_dst_addr;
    p_tcb->notify.dststatchange.status = status;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_DESTSTATCHANGE );
#else
    /* The sctp socket interface is being used */

    /* First check if this notification type is enabled in the endpoint */
    if( SCTP_SOCK_IS_RECVPADDR_ON( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;
        sctp_U32bit            s_err;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_dst_status_change: SCTPNOTIFY_DESTSTATCHANGE"
                          "/PeerAddrChng: No Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_padr_change.spc_type = SCTP_PEER_ADDR_CHANGE;

        /* flags are currently unused */
        p_new->notification.sn_padr_change.spc_flags      = 0;

        /*
         * Total length of the notification data: including the notification
         * header.
         */
        p_new->notification.sn_padr_change.spc_length = sizeof( sctp_paddr_change_st );

        /* The assoc id :used only in case of UDP style */
        p_new->notification.sn_padr_change.spc_assoc_id   = p_tcb->assoc_id;

        SOCKADDR_STORAGE_FAMILY( p_new->notification.sn_padr_change.spc_aaddr ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        if( SCTP_FAILURE ==
                sctp_conv_addr_sctp_to_os( 1, p_dst_addr,
                                           &( p_new->notification.sn_padr_change.spc_aaddr ), p_tcb->transmit.that_port,
                                           ( sctp_error_t * )&s_err ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_ntfy_dst_status_change: couldnot convert address " ) );
        }

        /* The particular event that happened */
        p_new->notification.sn_padr_change.spc_state      = status;

        /* Any appropriate error code */
        p_new->notification.sn_padr_change.spc_error      = e_code;

        SOCKADDR_STORAGE_FAMILY( p_new->peer_addr ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        if( SCTP_FAILURE ==
                sctp_conv_addr_sctp_to_os( 1, p_dst_addr,
                                           &( p_new->peer_addr ), p_tcb->transmit.that_port,
                                           ( sctp_error_t * )&s_err ) )
        {
            /* This address is to be returned in msghdr's msg_name field */
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_ntfy_dst_status_change: couldnot convert address " ) );
        }

        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }

        /*SPR 5048*/
        SCTP_WAKEUP( p_tcb->p_ep->cond );
        /*SPR 5048*/
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_dst_status_change: SCTPNOTIFY_DESTSTATCHANGE"
                      "/Peer addr change notificaiton not enabled\n" ) );
    }

    /* Update the stats */
    if( status == SCTP_ADDRESS_AVAILABLE )
    {
        SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_DEST_INACTIVE, 1 );
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_DEST_ACTIVE, 1 );
    }

    else if( status == SCTP_ADDRESS_UNREACHABLE )
    {
        SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_DEST_ACTIVE, 1 );
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_DEST_INACTIVE, 1 );
    }

#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_endpt_status_change
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_ENDSTATCHANGE notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_endpt_status_change(
    sctp_tcb_st               *p_tcb,
    sctp_sock_sac_event_et    status,
    sctp_endpoint_chg_data_ut *p_status_data,
    sctp_error_t              e_code )
{
    sctp_Boolean_t  delete_assoc = SCTP_FALSE;
#ifdef SCTP_UI_SOCK_API
    sctp_U32bit     info_len;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( e_code );
    /*to avoid the warning*/


    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_endpt_status_change" ) );

    if( status == SCTP_COMMUNICATION_UP )
    {
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_ENDPOINT_UP, 1 );
        SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_ENDPOINT_DOWN, 1 );
    }

    else if( ( status == SCTP_COMMUNICATION_LOST )
             || ( status == SCTP_ENDPOINT_DOWN_RETRIEVABLE )
             || ( status == SCTP_SHUTDOWN_COMPLETE ) )
    {
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_ENDPOINT_DOWN, 1 );
        SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_ENDPOINT_UP, 1 );
    }

#ifndef SCTP_UI_SOCK_API

    /* The Traditional socket interface is enabled */
    if( ( SCTP_SOCK_IS_LISTENER( p_tcb->p_ep->flags ) )
            && !( SCTP_SOCK_IS_CLOSE_CALLED( p_tcb->p_ep->flags ) ) )
    {
        /*don't give indication, just return*/
        return SCTP_SUCCESS;
    }

    p_tcb->notify.endstatchange.assoc_id    = p_tcb->p_ep->ep_id;

    if( ( SCTP_CANT_START_ASSOC == status ) || ( SCTP_SHUTDOWN_COMPLETE == status ) )
    {
        status = SCTP_COMMUNICATION_LOST;
    }

    if( ( SCTP_COMMUNICATION_UP == status ) || ( SCTP_RESTART == status ) )
    {
        SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_FALSE );
        SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_TRUE );
    }

    else if( status != SCTP_ENDPOINT_ADDR_CHNG )
    {
        SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_TRUE );
        SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_FALSE );
    }

    p_tcb->notify.endstatchange.status      = status;
    p_tcb->notify.endstatchange.status_data = *p_status_data;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_ENDSTATCHANGE );

#else

    /*
     * When the status changed to UP then we need to set it
     * available for write.
     */
    switch( status )
    {
        case SCTP_COMMUNICATION_UP:
        case SCTP_RESTART:
            /* Mark the socket as writeable */
            SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_FALSE );
            SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_TRUE );

            /*
             * Start the autoclose timer if the ASTOCLOSE option is
             * enabled for the socket.
             */
            if( SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
            {
                sctp_U32bit timerintrvl = 0;

                /* Calculate the timer interval for autoclose timer */
                timerintrvl = p_tcb->p_ep->aclose_time * 1000;

                SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                            ( "sctp_ntfy_endpt_status_change: COMM_UP: starting "
                              "Autoclose timer" ) );

                /* Start the autoclose timer. */
                sctp_start_timer( p_tcb,
                                  SCTPEVENT_TIMERAUTOCLOSE,
                                  timerintrvl,
                                  0,
                                  &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                                  SCTP_NULL );
            }

            /* Changes For CSR#1-6684004 */
            if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
            {
                /* Check if the socket was selected for error */
                sctp_wakeup_selected( p_tcb->p_ep->ep_id );
            }

            /* Changes For CSR#1-6684004 Ends */

            break;

        case SCTP_COMMUNICATION_LOST:
        case SCTP_SHUTDOWN_COMPLETE:
        case SCTP_CANT_START_ASSOC:

            /* SPR 4074 */
            if( status == SCTP_SHUTDOWN_COMPLETE )
            {
                /* Mark the socket as writeable because */

                SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_FALSE );
                SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_TRUE );
            }

            else
            {
                /* Mark the socket as not writeable */
                SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_TRUE );
                SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_FALSE );
            }

            /*
             * Stop the autoclose timer if the ASTOCLOSE option is
             * enabled for the socket.
             */
            if( SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
            {
                if( SCTP_COMMUNICATION_LOST == status )
                {
                    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                                ( "sctp_ntfy_endpt_status_change: COMM_LOST: stopping"
                                  "Autoclose timer" ) );
                }

                else if( SCTP_SHUTDOWN_COMPLETE == status )
                {
                    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                                ( "sctp_ntfy_endpt_status_change: SHUTDOWN_COMPLETE: "
                                  "stopping Autoclose timer" ) );
                }

                else if( SCTP_CANT_START_ASSOC == status )
                {
                    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                                ( "sctp_ntfy_endpt_status_change: CANT_START_ASSOC: "
                                  "stopping Autoclose timer" ) );
                }

                /* Stop the autoclose timer. */
                sctp_stop_timer(
                    &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                    SCTP_NULL, SCTP_NULL );
            }

            break;

        case SCTP_ENDPOINT_DOWN_RETRIEVABLE:
            {
                sctpbuffer_st *p_buf, *save;
                SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                            ( "endpt_status_change: SCTP_ENDPOINT_DOWN_RETRIEVABLE"
                              "Not in Socket i/f" ) );
                /* Mark the socket as not writeable */
                SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_TRUE );
                SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_FALSE );
                /* SPR 4056 */
                /*
                * here give send fail notification per data and change status
                * to SCTP_COMMUNICATION_LOST bcose in socket interface there
                * is no state SCTP_ENDPOINT_DOWN_RETRIEVABLE
                */


                status = SCTP_COMMUNICATION_LOST;

                p_buf = ( sctpbuffer_st * )
                        lstFirst( &p_tcb->transmit.transmit_queue );

                for( ; ; )
                {
                    if( p_buf == NULL )
                    {
                        break;
                    }

                    save = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );

                    if( p_buf->begin == 0 )
                    {
                        p_buf = save;
                        continue;
                    }

                    lstDelete( &p_tcb->transmit.transmit_queue, ( NODE * )p_buf );
                    sctp_free_transmit_buffer( p_tcb, p_buf );

                    /* also delete , rest of the fragments */
                    if( p_buf->end != 1 )
                    {
                        for( ; ; )
                        {
                            p_buf = save;

                            if( ( p_buf == NULL ) || ( p_buf->end == 1 ) )
                            {
                                break;
                            }

                            /* SPR : 18527 */
                            else
                            {
                                save = ( sctpbuffer_st * )lstNext( ( NODE * ) p_buf );
                            }

                            /* SPR : 18527 */
                        }
                    }

                    /* SPR 21107 changes */
                    if( p_buf == NULL )
                    {
                        break;
                    }

                    /* SPR 21107 changes */

                    /* give a send failure notification */
                    sctp_ntfy_send_failure( p_tcb, p_buf->context,
                                            p_buf->sent, E_NOERROR, sizeof( sctpbuffer_st ),
                                            ( sctp_U8bit * )p_buf );

                    SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, p_buf->stream,
                                                 SCTPSTATS_ASSOC_STREAM_DATA_SENT, 1 );

                    p_buf = save;
                }

                /*Now Delete the association*/
                delete_assoc = SCTP_TRUE;
                /*SPR 4056 */
            }
            break;

        default:
            break;
    }

    if( ( SCTP_SOCK_IS_CLOSE_CALLED( p_tcb->p_ep->flags ) ) &&
            ( ( SCTP_COMMUNICATION_LOST == status ) ||
              ( SCTP_SHUTDOWN_COMPLETE == status ) ) )
    {
        if( SCTP_TRUE == delete_assoc )
        {
            sctp_db_delete_association( p_tcb );
        }

        /* since the close has called, No need to give notification here */
        return ( SCTP_SUCCESS );
    }

    /*
     * First check if this notification type is enabled in the endpoint
     * Note that the notification SCTP_ENDPOINT_DOWN_RETRIEVABLE is allowed
     * only in the HSS sctp interface and not in the socket API interface, So
     * we have to look at the transmit queue if there is any data then give a
     * send fail notification to the user first.
     */

    if( SCTP_SOCK_IS_RECVASSOC_ON( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;
        sctp_sockaddr_st      *p_dst_addr = SCTP_NULL;
        sctp_U32bit            s_err;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_endpt_status_change:SCTPNOTIFY_ENDSTATCHANGE:"
                          " No Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_assoc_change.sac_type   = SCTP_ASSOC_CHANGE;

        /* flags are currently unused */
        p_new->notification.sn_assoc_change.sac_flags  = 0;

        if( SCTPEVENT_MSGABORT == p_tcb->fsm_event )
        {
            /*
             * Total length of the notification data: including the notification
             * header.
             */
            /* Changes for CSR#1-4734801 */
            info_len = ( p_tcb->event.msg_abort.chunk_length < SCTP_MAX_NOTF_DATA ) ?
                       p_tcb->event.msg_abort.chunk_length : SCTP_MAX_NOTF_DATA;
            /* Changes for CSR#1-4734801 Ends */

            /*reset chunk length to 0 as it is no longer required */
            p_tcb->event.msg_abort.chunk_length = 0;
            p_tcb->event.msg_abort.cause_length = 0;

            p_new->notification.sn_assoc_change.sac_length
                = sizeof( sctp_assoc_change_st ) - SCTP_MAX_NOTF_DATA + info_len;

            if( info_len > 0 )
            {
                __MEMCOPY( p_new->notification.sn_assoc_change.sac_info,
                           p_tcb->event.msg_abort.p_chunk, info_len );
            }
        }

        else
        {
            /*
             * Total length of the notification data: including the notification
             * header.
             */
            p_new->notification.sn_assoc_change.sac_length
                = sizeof( sctp_assoc_change_st );
        }

        /* The assoc id :used only in case of UDP style */
        p_new->notification.sn_assoc_change.sac_assoc_id = p_tcb->assoc_id;

        /* The particular event that happens */
        p_new->notification.sn_assoc_change.sac_state = status;
        p_new->notification.sn_assoc_change.sac_error = e_code;

        /* The Number of in streams and out streams for this assoc */
        p_new->notification.sn_assoc_change.sac_outbound_streams
            = p_status_data->endpoint_up.num_out_streams;
        p_new->notification.sn_assoc_change.sac_inbound_streams
            = p_status_data->endpoint_up.num_in_streams;

        p_dst_addr = &( p_tcb->transmit.
                        remote[p_tcb->transmit.primary_address_index].that_address );

        SOCKADDR_STORAGE_FAMILY( p_new->peer_addr ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        if( SCTP_FAILURE ==
                sctp_conv_addr_sctp_to_os( 1, p_dst_addr,
                                           &( p_new->peer_addr ), p_tcb->transmit.that_port,
                                           ( sctp_error_t * )&s_err ) )
        {
            /* This address is to be returned in msghdr's msg_name field */
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_ntfy_endpt_status_change: couldnot convert address " ) );
        }

        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_endpt_status_change: SCTPNOTIFY_ENDSTATCHANGE/ASSOC"
                      " CHANGE:notificaiton not enabled\n" ) );
    }

#endif

    /*SPR 5048*/
    SCTP_WAKEUP( p_tcb->p_ep->cond );

    /*SPR 5048*/
    if( SCTP_TRUE == delete_assoc )
    {
        sctp_db_delete_association( p_tcb );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_congestion
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_CONGESTION notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_congestion(
    sctp_tcb_st        *p_tcb,
    sctp_cong_level_et cong_level )
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_ntfy_congestion: level = %d", cong_level ) );

    switch( cong_level )
    {
        case SCTP_CONGLEVEL_NOCONG:
        case SCTP_CONGLEVEL_LOW:
            SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_TRUE );
            break;

        case SCTP_CONGLEVEL_MED:
            SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_TRUE );
            break;

        case SCTP_CONGLEVEL_MAX:
        default:
            break;
    }

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.congestion.assoc_id   = p_tcb->p_ep->ep_id;
    p_tcb->notify.congestion.cong_level = cong_level;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_CONGESTION );

#else
    /* The sctp socket interface is being used */

    /* Changes For CSR#1-6684004 */
    if( SCTP_SOCK_IS_WRITEABLE( p_tcb->p_ep->flags ) )
    {
        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }
    }

    /* Changes For CSR#1-6684004 Ends */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_ntfy_congestion: SCTPNOTIFY_CONGESTION:notificaiton not "
                  "enabled\n" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_tcb );
    /*to avoid the warning*/



#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_sys_error
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_SYSERROR notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_sys_error(
    sctp_tcb_st    *p_tcb,
    sctp_error_t   err_no )
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_sys_error" ) );

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.syserror.assoc_id = p_tcb->p_ep->ep_id;
    p_tcb->notify.syserror.errorno    = err_no;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_SYSERROR );

#else
    /* The sctp socket interface is being used */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_ntfy_sys_error:SCTPNOTIFY_SYSERROR:notificaiton not enabled\n" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_tcb );
    SCTP_UNUSED_VARIABLE( err_no );
    /*to avoid the warning*/
#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_conn_pending
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_CONNPENDING notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_conn_pending(
    sctp_tcb_st     *p_tcb )
{
    sctp_ep_st    *p_ep;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_conn_pending" ) );
    SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_ENDPOINT_UP, 1 );
    SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_ENDPOINT_DOWN, 1 );

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( p_tcb->p_ep->ep_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_db_get_ep_ptr(%d) failed", p_tcb->p_ep->ep_id ) );

        return ( SCTP_FAILURE );
    }

    /* SPR : 18527 */

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    /* The Association id On which the connection is pending */
    p_tcb->notify.connpending.assoc_id = p_tcb->p_ep->server_ep_id;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_ep, SCTPNOTIFY_CONNPENDING );

#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_ntfy_conn_pending: SCTPNOTIFY_CONNPENDING:No such notification"
                  " in socket APIs and just setting that the server socket is ready for"
                  " read.\n" ) );

    /*
     * In The socket interface we do not have a connection pending notif-
     * cation. The ULP is supposed to do an accept to check whether we have
     * a new connection waiting for the server. So we don't have to queue up
     * anything in this case.
     */

    /*
     * For the select API if a socket is a listener and has some assocs waiting
     * for accept then the sock doesnot block on a read after select.
     * Check if the socket is selected in some select block.
     */
    /* Set the socket as readable */
    SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

    if( SCTP_SOCK_IS_SELECTED( p_ep ) )
    {
        /* Check if the socket was selected for error */
        sctp_wakeup_selected( p_tcb->p_ep->server_ep_id );
    }

#endif

    return ( SCTP_SUCCESS );
}




/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_data_pending
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_DATAPENDING notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_data_pending(
    sctp_tcb_st     *p_tcb,
    sctp_U16bit     stream,
    sctp_U32bit     ssn, /* Stream Sequence number */
    sctp_U32bit     no_notify ) /* no. of data Notifications */
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_data_pending" ) );

    /* Set the socket as readable */
    SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ssn );
    SCTP_UNUSED_VARIABLE( stream );
    /*to avoid the warning*/

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */

    /**** ACCEPT_BUGFIX START ****/
    if( SCTP_SOCK_IS_LISTENER( p_tcb->p_ep->flags ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                    ( "sctp_ntfy_data_pending:server socket, waiting for accept" ) );
        return( SCTP_SUCCESS );
    }

    /**** ACCEPT_BUGFIX END ****/
    /* The Association id for which the data arrived */
    p_tcb->notify.datapending.assoc_id = p_tcb->p_ep->ep_id;

    /* The stream on which the data arived */
    p_tcb->notify.datapending.stream   = stream;

    /* Invoke the generic Notification function. */
    for( ; no_notify > 0; no_notify-- )
    {
        sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_DATAPENDING );
    }

#else

    /*
     * In this we need to queue the data pending indication. This is a special
     * case indication as we shall not give this indication to the ULP. This is
     * just for our internal handling. We have just one Data pending indication
     * structure for one association in one endpoint. So in this case we have
     * to first look for the indication structure for this association in the
     * queue and then increment the data count.
     * Now Note: We cannot club the data indications with the other indications
     * as 1. we dun want to be searching the indications list all the time when
     *       ever we want to read the data. (Decreases perforamace)
     *    2. So! we have a new structure for this at the Endpoint level and this
     *       structure stores the Data related indications.
     *    3. Generally we have just one instance of this structure. But in case
     *       of UDP this will be a list and the number of nodes in this list
     *       will be equal to the number of associations in the UDP endpoint.
     */

    /*
     * Stop the autoclose timer and start it again if AUTOCLOSE option is set
     * for the socket.
     */
    if( SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
    {
        sctp_U32bit timerintrvl = 0;

        /* Stop the autoclose timer. */
        sctp_stop_timer(
            &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
            SCTP_NULL, SCTP_NULL );

        /* Calculate the timer interval for autoclose timer */
        timerintrvl = p_tcb->p_ep->aclose_time * 1000;

        SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                    ( "sctp_ntfy_data_pending: COMM_UP: starting "
                      "Autoclose timer" ) );

        /* Start the autoclose timer. */
        sctp_start_timer( p_tcb,
                          SCTPEVENT_TIMERAUTOCLOSE, timerintrvl, 0,
                          &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                          SCTP_NULL );
    }

    /* add a notification into the datapending queue for the association */
    if( SCTP_FAILURE == sctp_db_add_data_ind( p_tcb, no_notify ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                    ( "sctp_ntfy_data_pending: Error adding data indication" ) );

        return ( SCTP_FAILURE );
    }

    /* Set the socket as readable */
    SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

    /*
     * For the select API if the sock was waiting for read then wake it up.
     * Check if the socket is selected in some select block.
     */
    if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
    {
        /* Check if the socket was selected for error */
        sctp_wakeup_selected( p_tcb->p_ep->ep_id );
    }

#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_send_failure
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_SENDFAILURE notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_send_failure(
    sctp_tcb_st        *p_tcb,
    sctp_U32bit        context,
    sctp_sock_ssf_flag_et flags,
    sctp_error_t       cause_code,/* Cause for failure */
    sctp_U16bit        info_len,  /* Length of the info string */
    sctp_U8bit         *p_info )  /* Optiional data to be sent back */
{
#ifdef SCTP_UI_SOCK_API
    sctpbuffer_st    *p_buf = ( sctpbuffer_st * )p_info;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    SCTP_UNUSED_VARIABLE( info_len );
    SCTP_UNUSED_VARIABLE( p_info );

    /*to avoid the warning*/


    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_send_failure" ) );

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.sendfailure.assoc_id   = p_tcb->p_ep->ep_id;
    p_tcb->notify.sendfailure.context    = context;
    p_tcb->notify.sendfailure.cause_code = cause_code;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_SENDFAILURE );

#else

    /*
     * First check if this notification type is enabled in the endpoint
     */
    if( SCTP_SOCK_IS_RECVSENDFAIL( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;
        sctp_sockaddr_st      *p_dst_addr = SCTP_NULL;
        sctp_U32bit            s_err;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_send_failure: SCTPNOTIFY_SENDFAILURE: No"
                          " Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_send_failed.ssf_type = SCTP_SEND_FAILED;

        /* flags for the unsent data */
        p_new->notification.sn_send_failed.ssf_flags = flags;

        /* The assoc id :used only in case of UDP style */
        p_new->notification.sn_send_failed.ssf_assoc_id = p_tcb->assoc_id;

        /* The protocol error code */
        p_new->notification.sn_send_failed.ssf_error = cause_code;

        if( p_buf != NULL )
        {
            /*
             * Total length of the notification data: including the notification
             * header.
             */
            /* Changes for CSR#1-4734801 */
            info_len = ( p_buf->size < SCTP_MAX_NOTF_DATA ) ?
                       p_buf->size : SCTP_MAX_NOTF_DATA;
            /* Changes for CSR#1-4734801 Ends */

            p_new->notification.sn_send_failed.ssf_info.sinfo_assoc_id =
                p_tcb->assoc_id;
            p_new->notification.sn_send_failed.ssf_info.sinfo_context =
                context;
            p_new->notification.sn_send_failed.ssf_info.sinfo_timetolive =
                p_buf->lifetime;
            p_new->notification.sn_send_failed.ssf_info.sinfo_flags =
                p_buf->unordered ? SCTP_MSG_UNORDERED : 0;
            p_new->notification.sn_send_failed.ssf_info.sinfo_ppid =
                p_buf->ppi;
            p_new->notification.sn_send_failed.ssf_info.sinfo_stream =
                p_buf->stream;
            __MEMCOPY( p_new->notification.sn_send_failed.ssf_data,
                       p_buf->buffer, info_len );
        }

        else
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_ntfy_send_failure: NO Data To be returned\n" ) );
            info_len = 0;
        }


        p_new->notification.sn_send_failed.ssf_length
            = sizeof( sctp_send_failed_st ) - SCTP_MAX_NOTF_DATA + info_len;

        p_dst_addr = &( p_tcb->transmit.
                        remote[p_tcb->transmit.primary_address_index].that_address );

        SOCKADDR_STORAGE_FAMILY( p_new->peer_addr ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        if( SCTP_FAILURE ==
                sctp_conv_addr_sctp_to_os( 1, p_dst_addr,
                                           &( p_new->peer_addr ), p_tcb->transmit.that_port,
                                           ( sctp_error_t * )&s_err ) )
        {
            /* This address is to be returned in msghdr's msg_name field */
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_ntfy_send_failure: couldnot convert address " ) );
        }

        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }
    }

    /*SPR 21155 changes starts*/
    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_send_failure: SCTPNOTIFY_SENDFAILURE:notificaiton "
                      "not enabled\n" ) );
        return ( SCTP_SUCCESS );

    }

    /*SPR 21155 changes ends*/
#endif
    /*SPR 5048*/
    SCTP_WAKEUP( p_tcb->p_ep->cond );
    /*SPR 5048*/

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_partial_data
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_PARTIAL_DATAPENDING notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_partial_data(
    sctp_tcb_st     *p_tcb,
    sctp_U16bit     stream )
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_partial_data" ) );

    SCTP_UNUSED_VARIABLE( stream );


#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.partial_datapending.assoc_id = p_tcb->p_ep->ep_id;
    p_tcb->notify.partial_datapending.stream   = stream;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_PARTIAL_DATAPENDING );
#else

    /* The sctp socket interface is being used */
    if( SCTP_SOCK_IS_RECVPARTDELIVERY( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_send_failure: SCTPNOTIFY_SENDFAILURE: No"
                          " Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_rcv_pdapi_event.pdapi_type = SCTP_PARTIAL_DELIVERY;
        /*flags is currently unused */
        p_new->notification.sn_rcv_pdapi_event.pdapi_flags = 0;
        p_new->notification.sn_rcv_pdapi_event.pdapi_length =
            sizeof( sctp_pdapi_event_st );
        /*we don't send ABORT indication*/
        p_new->notification.sn_rcv_pdapi_event.pdapi_indication = 0;
        p_new->notification.sn_rcv_pdapi_event.pdapi_assoc_id =
            p_tcb->assoc_id;

        /*SPR 5086*/
        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }

        /*SPR 5086*/
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_partial_data: SCTPNOTIFY_PARTIAL_DATAPENDING:"
                      "notificaiton not enabled\n" ) );
    }

#endif
    /*SPR 5048*/
    SCTP_WAKEUP( p_tcb->p_ep->cond );
    /*SPR 5048*/

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_adaption_ind
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_ADAPTION_INDICATION notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_adaption_ind(
    sctp_tcb_st     *p_tcb,
    sctp_U32bit     adaption_bits )
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_adaption_ind" ) );

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.adaption_ind.assoc_id      = p_tcb->p_ep->ep_id;
    p_tcb->notify.adaption_ind.adaption_bits = adaption_bits;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_ADAPTION_IND );
#else

    /* The sctp socket interface is being used */
    if( SCTP_SOCK_IS_RECVADAPTION( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_send_failure: SCTPNOTIFY_SENDFAILURE: No"
                          " Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_adaption_event.sai_type = SCTP_ADAPTION_EVENT;
        /*flags is currently unused */
        p_new->notification.sn_adaption_event.sai_flags = 0;
        p_new->notification.sn_adaption_event.sai_length =
            sizeof( sctp_adaption_event_st );
        /*we don't send ABORT indication*/
        p_new->notification.sn_adaption_event.sai_adaptation_bits = adaption_bits;
        p_new->notification.sn_adaption_event.sai_assoc_id =
            p_tcb->assoc_id;

        /*SPR 5086*/
        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }

        /*SPR 5086*/
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_partial_data: SCTPNOTIFY_ADAPTION_IND:"
                      "notificaiton not enabled\n" ) );
    }

#endif

    /*don;t require to wakeup because endpt_status_chng has allready done */
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_recv_congestion
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_RECVCONGESTION notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_recv_congestion(
    sctp_tcb_st        *p_tcb,
    sctp_cong_level_et cong_level )
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_recv_congestion" ) );


#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.recv_congestion.assoc_id   = p_tcb->p_ep->ep_id;
    p_tcb->notify.recv_congestion.cong_level = cong_level;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_RECVCONGESTION );

#else
    /* The sctp socket interface is being used */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_ntfy_recv_congestion:SCTPNOTIFY_RECVCONGESTION:notificaiton "
                  "not enabled\n" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_tcb );
    SCTP_UNUSED_VARIABLE( cong_level );
    /*to avoid the warning*/


#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_shutdown_event
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_SHUTDOWN notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_shutdown_event(
    sctp_tcb_st    *p_tcb )
{
    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_shutdown_event" ) );

#ifndef SCTP_UI_SOCK_API
    /* The Traditional socket interface is enabled */
    p_tcb->notify.shutdown_event.assoc_id = p_tcb->p_ep->ep_id;

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_SHUTDOWN_EVENT );
#else
    /*
     * First check if this notification type is enabled in the endpoint
     */

    SCTP_SOCK_SET_WR_BLOCKED( p_tcb->p_ep->flags, SCTP_TRUE );
    SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_FALSE );

    if( SCTP_SOCK_IS_RECVSHUTDOWN( p_tcb->p_ep->ulpqueue.ntfy_flags ) )
    {
        /* Queue notification for the endpoint */
        sctp_ulpqueue_node_st *p_new = SCTP_NULL;
        sctp_sockaddr_st      *p_dst_addr = SCTP_NULL;
        sctp_U32bit            s_err;

        if( !( p_new = sctp_db_get_ntfy_node() ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_ntfy_shutdown_event: SCTPNOTIFY_SHUTDOWN: No "
                          "Notification node left\n" ) );

            return ( SCTP_FAILURE );
        }

        /* Got a new node: Create the notification */
        p_new->assoc_id = p_tcb->assoc_id;
        p_new->notification.sn_shutdown_event.sse_type   = SCTP_SHUTDOWN_EVENT;

        /* flags : Currently unused */
        p_new->notification.sn_shutdown_event.sse_flags  = 0;

        /*
         * Total length of the notification data: including the notification
         * header.
         */
        p_new->notification.sn_shutdown_event.sse_length
            = sizeof( sctp_shutdown_event_st );

        /* The assoc id :used only in case of UDP style */
        p_new->notification.sn_shutdown_event.sse_assoc_id = p_tcb->assoc_id;

        p_dst_addr = &( p_tcb->transmit.
                        remote[p_tcb->transmit.primary_address_index].that_address );

        SOCKADDR_STORAGE_FAMILY( p_new->peer_addr ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        if( SCTP_FAILURE ==
                sctp_conv_addr_sctp_to_os( 1, p_dst_addr,
                                           &( p_new->peer_addr ), p_tcb->transmit.that_port,
                                           ( sctp_error_t * )&s_err ) )
        {
            /* This address is to be returned in msghdr's msg_name field */
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_ntfy_shutdown_event: couldnot convert address " ) );
        }

        /*
         * Now queue the notification in the Endpoint notification list.
         */
        sctp_db_ulpqueue_add_ntfy( &( p_tcb->p_ep->ulpqueue ), p_new );

        /* Set the socket as readable */
        SCTP_SOCK_SET_READABLE( p_tcb->p_ep->flags, SCTP_TRUE );

        if( SCTP_SOCK_IS_SELECTED( p_tcb->p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_tcb->p_ep->ep_id );
        }
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_ntfy_shutdown_event: SCTPNOTIFY_SHUTDOWN:notificaiton not "
                      "enabled\n" ) );
    }

#endif
    /*SPR 5048*/
    SCTP_WAKEUP( p_tcb->p_ep->cond );
    /*SPR 5048*/

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *       sctp_ntfy_peer_data_drop
 *
 *    DESCRIPTION:
 *        date chunk drop notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_peer_data_drop(
    sctp_tcb_st    *p_tcb,
    sctp_error_t   *p_ecode,
    sctp_U8bit     *p_buffer,
    sctp_U32bit    buffer_len )
{

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_peer_data_drop" ) );

#ifndef SCTP_UI_SOCK_API
    /*
     * Set the Notifications into the notification structure in the
     * association.
     */
    p_tcb->notify.peer_data_drop.assoc_id = p_tcb->p_ep->ep_id;
    p_tcb->notify.peer_data_drop.ecode    = *p_ecode;
    p_tcb->notify.peer_data_drop.packet_len = buffer_len;
    __MEMCOPY( p_tcb->notify.peer_data_drop.buffer,
               p_buffer, buffer_len );

    /* Invoke the generic Notification function. */
    sctp_generic_su_notification( p_tcb->p_ep, SCTPNOTIFY_PEER_DATA_DROP );
#else
    /* The sctp socket interface is being used */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_ntfy_peer_data_drop:SCTPNOTIFY_PEER_DATA_DROP:"
                  "notificaiton not enabled\n" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_tcb );
    SCTP_UNUSED_VARIABLE( p_ecode );
    SCTP_UNUSED_VARIABLE( p_buffer );
    SCTP_UNUSED_VARIABLE( buffer_len );
    /*to avoid the warning*/


#endif
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_is_af_same
 *
 *    DESCRIPTION:
 *        Checks if the address type passed is same with that of endpoint.
 *
 *    RETURN VALUE:
 *        SCTP_FALSE/SCTP_TRUE
 *************************************************************************/
sctp_Boolean_t
sctp_db_is_af_same(
    sctp_ep_st            *p_ep,
    sctp_U32bit           n_addr,
    sctp_sockaddr_st      addr_list[] )
{
    sctp_U32bit   count = 0;

    for( ; count < n_addr; count++ )
    {
        if( p_ep->family != addr_list[count].family )
        {
            return ( SCTP_FALSE );
        }
    }

    return ( SCTP_TRUE );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_is_af_correct
 *
 *    DESCRIPTION:
 *        Checks if the address type passed is correct in the endpoint.
 *
 *    RETURN VALUE:
 *        SCTP_FALSE/SCTP_TRUE
 *************************************************************************/
sctp_Boolean_t
sctp_db_is_af_correct(
    sctp_ep_st            *p_ep,
    sctp_U32bit           n_addr,
    sctp_sockaddr_st      addr_list[] )
{
    sctp_U32bit   count = 0;

    for( ; count < n_addr; count++ )
    {
        if( p_ep->family == SCTP_AF_INET )
        {
            if( p_ep->family != addr_list[count].family )
            {
                return ( SCTP_FALSE );
            }
        }

        else if( p_ep->family == SCTP_AF_INET6 )
        {
            if( ( addr_list[count].family != SCTP_AF_INET6 ) &&
                    ( addr_list[count].family != SCTP_AF_INET ) )
            {
                return ( SCTP_FALSE );
            }
        }
    }

    return ( SCTP_TRUE );
}
/***************************************************************************
 *    FUNCTION :
 *        sctp_db_is_v4_v6_addr_compatible
 *
 *    DESCRIPTION:
 *        Checks if the address type passed is compatible in the endpoint.
 *
 *    RETURN VALUE:
 *        SCTP_TRUE : SCTP_FALSE
 *************************************************************************/
sctp_Boolean_t
sctp_db_is_v4_v6_addr_compatible(
    sctp_ep_st              *p_ep,
    sctp_U32bit             n_addr,
    sctp_sockaddr_list_st   addr_list,
    sctp_suppaddr_st        *p_peer_support )
{
    sctp_Boolean_t        local_v4_present = SCTP_FALSE,
                          local_v6_present = SCTP_FALSE,
                          peer_v4_present = SCTP_FALSE,
                          peer_v6_present = SCTP_FALSE;

    sctp_U32bit           count = 0;
    /*no type is supported by default*/
    sctp_suppaddr_st      default_support = {SCTP_FALSE, SCTP_FALSE, SCTP_FALSE};

    SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                ( "sctp_db_is_v4_v6_addr_compatible" ) );

    for( count = 0; count < n_addr; count++ )
    {
        if( addr_list[count].family == SCTP_AF_INET6 )
        {
            peer_v6_present = SCTP_TRUE;
        }

        else
        {
            peer_v4_present = SCTP_TRUE;
        }

        if( peer_v4_present && peer_v6_present )
        {
            /*No need to go further*/
            break;
        }
    }

    for( count = 0; count < p_ep->naddr; count++ )
    {
        if( p_ep->this_address[count].family == SCTP_AF_INET6 )
        {
            local_v6_present = SCTP_TRUE;
        }

        else
        {
            local_v4_present = SCTP_TRUE;
        }

        if( local_v4_present && local_v6_present )
        {
            /*No need to go further*/
            break;
        }
    }

    /*peer6ok tells if peer is ok with v6 addresses*/
    /*
     * This is not a concern now. So if local endpoint supports only one
     * of them and the other supports both, then ignore the types and the
     * corresponding address that we don't support.
     *
    */
    /* SPR 6751 */
    /* Check if only one of v4 and v6 is present on local */
    if( ( local_v4_present ^ local_v6_present ) &&
            ( ( !peer_v6_present && local_v6_present ) ||
              ( !peer_v4_present && local_v4_present ) ) )
    {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        /*Bug Id 22 fix starts*/
        /*return (SCTP_FAILURE);*/
        return( SCTP_FALSE );
        /*Bug Id 22 fix ends*/
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        return ( SCTP_FALSE );
#endif
    }

    /* SPR 6751 */

    if( SCTP_NULL == p_peer_support )
    {
        p_peer_support = &default_support;
    }

    /* Fix for SPR 8046*/


    if( ( local_v4_present ^ ( peer_v4_present || p_peer_support->v4 ) ) &&
            ( local_v6_present ^ ( peer_v6_present || p_peer_support->v6 ) ) )
    {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        /*Bug Id 22 fix starts*/
        /*return (SCTP_FAILURE);*/
        return( SCTP_FALSE );
        /*Bug Id 22 fix ends*/
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        return ( SCTP_FALSE );
#endif
    }

    /*End fix*/

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /*Bug Id 22 fix starts*/
    /*return (SCTP_SUCCESS);*/
    return( SCTP_TRUE );
    /*Bug Id 22 fix ends*/
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return ( SCTP_TRUE );
#endif
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_supported_address_types
 *
 *    DESCRIPTION:
 *        Checks if the address type passed is compatible in the endpoint.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 *************************************************************************/
sctp_return_t
sctp_db_get_supported_address_types(
    sctp_ep_st              *p_ep,
    sctp_suppaddr_st        *p_supp_addr )
{
    sctp_U32bit           count = 0;

    SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_supported_address_types" ) );

    p_supp_addr->hname = p_ep->hostname_supported;
    p_supp_addr->v6 = SCTP_FALSE;
    p_supp_addr->v4 = SCTP_FALSE;

    for( count = 0; count < p_ep->naddr; count++ )
    {
        if( p_ep->this_address[count].family == SCTP_AF_INET6 )
        {
            p_supp_addr->v6 = SCTP_TRUE;
        }

        else
        {
            p_supp_addr->v4 = SCTP_TRUE;
        }

        if( p_supp_addr->v4 && p_supp_addr->v6 )
        {
            /*No need to go further*/
            break;
        }
    }

    return ( SCTP_SUCCESS );
}
/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_primary_addr_index
 *
 *    DESCRIPTION:
 *        For the association finds the primary destination address and a
 *    corresponding source address. The two index's returned have the same
 *    type.
 *
 *    RETURN VALUE:
 *        SCTP_FAILURE if there is no source address available corresponding
 *    to the destination addres.
 *************************************************************************/
sctp_return_t
sctp_db_get_primary_addr_index(
    sctp_tcb_st           *p_tcb,
    sctp_U32bit           *p_src_pri, /* primary source address  */
    sctp_U32bit           *p_dst_pri, /* primary destination address */
    sctp_error_t          *p_ecode )
{
    sctp_U32bit      count = 0;
    sctp_sa_family_t af_dst;
    sctp_sa_family_t af_src;

    /* Get the destination primary address for the tcb */
    *p_dst_pri = p_tcb->transmit.primary_address_index;

    /* Get the destnation address family */
    af_dst = p_tcb->transmit.remote[*p_dst_pri].that_address.family;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_primary_addr_index:famliy = %d", af_dst ) );

    /* Find the source address with family af */
    *p_src_pri = p_tcb->transmit.remote[*p_dst_pri].src_addr_index;

    /* Get the source address family. */
    af_src = p_tcb->p_ep->this_address[*p_src_pri].family;

    /* If the families match then return soccess */
    if( af_src == af_dst )
    {
        return ( SCTP_SUCCESS );
    }

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_primary_addr_index:src_address_index not ok." ) );

    /* Else find the first suitable source address */
    /* start spr 21157 */
    for( count = 0; count < p_tcb->p_ep->naddr  &&  count < SCTP_MAX_TRANSPORT_ADDR; count++ )
        /* end spr 21157 */
    {
        if( af_dst == p_tcb->p_ep->this_address[count].family )
        {
            *p_src_pri = count;
            p_tcb->transmit.remote[*p_dst_pri].src_addr_index = count;
            return ( SCTP_SUCCESS );
        }
    }

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_primary_addr_index: Could not find source address with "
                  "the same family as the destination address " ) );

    SCTP_SET_ECODE( p_ecode, E_ADDR_TYPE_MISMATCH );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_saddr_index_with_af
 *
 *    DESCRIPTION:
 *        For the address family specified gets the best suitable source
 *    address index.
 *
 *    RETURN VALUE:
 *        SCTP_FAILURE if there is no source address available
 *        corresponding to the address family.
 *************************************************************************/
sctp_return_t
sctp_db_get_saddr_index_with_af(
    sctp_tcb_st           *p_tcb,
    sctp_U32bit           *p_src_pri, /* primary source address  */
    sctp_sa_family_t      af,         /* Address Family */
    sctp_error_t          *p_ecode )
{
    sctp_U32bit      count = 0;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_saddr_index_with_af:famliy = %d", af ) );

    /* Else find the first suitable source address */
    for( count = 0; count < p_tcb->p_ep->naddr; count++ )
    {
        if( af == p_tcb->p_ep->this_address[count].family )
        {
            *p_src_pri = count;

            return ( SCTP_SUCCESS );
        }
    }

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_primary_addr_index: Could not find source address with "
                  "the same family as the destination address " ) );

    SCTP_SET_ECODE( p_ecode, E_ADDR_TYPE_MISMATCH );

    return ( SCTP_FAILURE );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_v4_v6_saddr_index_with_af
 *
 *    DESCRIPTION:
 *        For the address family specified gets the best suitable source
 *    address index.
 *
 *    RETURN VALUE:
 *        SCTP_FAILURE if there is no source address available
 *        corresponding to the address family.
 *************************************************************************/
sctp_return_t
sctp_db_get_v4_v6_addr_index_with_af(
    sctp_tcb_st           *p_tcb,
    sctp_U32bit           *p_src_pri, /* primary source address  */
    sctp_sa_family_t      af,         /* Address Family */
    sctp_error_t          *p_ecode )
{
    sctp_U32bit      count = 0;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_v4_v6_addr_index_with_af:famliy = %d", af ) );

    /* Else find the first suitable source address */
    for( count = *p_src_pri; count < p_tcb->p_ep->naddr; count++ )
    {
        if( af == p_tcb->p_ep->this_address[count].family )
        {
            *p_src_pri = count;
            return ( SCTP_SUCCESS );
        }
    }

    for( count = 0; count < *p_src_pri; count++ )
    {
        if( af == p_tcb->p_ep->this_address[count].family )
        {
            *p_src_pri = count;
            return ( SCTP_SUCCESS );
        }
    }

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_db_get_primary_addr_index: Could not find source address with "
                  "the same family as the destination address " ) );

    SCTP_SET_ECODE( p_ecode, E_ADDR_TYPE_MISMATCH );

    return ( SCTP_FAILURE );
}

#ifdef SCTP_UI_SOCK_API

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_update_readable_status
 *
 *    DESCRIPTION:
 *        Updates the readable staus for the endpoint.
 *
 *    RETURN VALUE:
 *        void
 *************************************************************************/
void
sctp_db_update_readable_status(
    sctp_ep_st     *p_ep )
{

    SCTP_TRACE( p_ep, SCTP_DBG_TRC, ( "sctp_db_update_readable_status" ) );

    /*
     * If we have some data waiting on the socket ot be read then the socket
     * is readable.
     */
    if( lstCount( &( p_ep->ulpqueue.data_ind_list ) ) > 0 )
    {
        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "sctp_db_update_readable_status: have data: setting readable" ) );

        SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_TRUE );

        /* Changes For CSR#1-6284004 */
        /* If the endpoint is selected invoke select processing */
        if( SCTP_SOCK_IS_SELECTED( p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_ep->ep_id );
        }

        /* Changes For CSR#1-6284004 */
        return;
    }

    /*
     * If we have a notification then the socket is readable.
     */
    if( p_ep->ulpqueue.n_ind > 0 )
    {
        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "sctp_db_update_readable_status: have ntfy: setting readable" ) );

        SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_TRUE );

        /* Changes For CSR#1-6284004 */
        /* If the endpoint is selected invoke select processing */
        if( SCTP_SOCK_IS_SELECTED( p_ep ) )
        {
            /* Check if the socket was selected for error */
            sctp_wakeup_selected( p_ep->ep_id );
        }

        /* Changes For CSR#1-6284004 */
        return;
    }

    /*
     * If endpoint is a server (tcp) then if there is any waiting association
     * then set the ep as readable.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) &&
            ( SCTP_SOCK_STREAM == p_ep->sk_type ) )
    {
        if( p_ep->n_assoc > 0 )
        {
            SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                        ( "sctp_db_update_readable_status: waiting assoc: setting "
                          "readable" ) );

            SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_TRUE );

            /* Changes For CSR#1-6284004 */
            /* If the endpoint is selected invoke select processing */
            if( SCTP_SOCK_IS_SELECTED( p_ep ) )
            {
                /* Check if the socket was selected for error */
                sctp_wakeup_selected( p_ep->ep_id );
            }

            /* Changes For CSR#1-6284004 */
            return;
        }
    }

    SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                ( "sctp_db_update_readable_status: setting !readable" ) );

    SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

    return;
}

#endif /* SCTP_UI_SOCK_API */


#ifdef SCTP_DYNAMIC_IP_SUPPORT
/***************************************************************************
 *    FUNCTION :
 *        sctp_ntfy_local_addr_change
 *
 *    DESCRIPTION:
 *        SCTPNOTIFY_ENDSTATCHANGE notification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_ntfy_local_addr_change(
    sctp_tcb_st             *p_tcb,
    sctp_sockaddr_st        *p_addr,
    sctp_addrconf_status_et status,
    sctp_asconf_response_et ecode )
{
#ifndef SCTP_UI_SOCK_API
    sctp_endpoint_chg_data_ut   data;

    data.endpoint_addr_chng.src_addr = *p_addr;
    data.endpoint_addr_chng.status   = status;
    data.endpoint_addr_chng.response = ecode;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_ntfy_local_addr_change" ) );

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_ntfy_endpt_status_change( p_tcb, SCTP_ENDPOINT_ADDR_CHNG,
                                   &data, ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_ntfy_endpt_status_change( p_tcb, SCTP_ENDPOINT_ADDR_CHNG,
                                   &data, ( sctp_error_t )ecode );
#endif
#else

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_tcb );
    SCTP_UNUSED_VARIABLE( status );
    SCTP_UNUSED_VARIABLE( p_addr );
    SCTP_UNUSED_VARIABLE( ecode );
    /*to avoid the warning*/

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_ntfy_local_addr_change:not supported in socket I/F" ) );
#endif
    return ( SCTP_SUCCESS );
}

#endif
