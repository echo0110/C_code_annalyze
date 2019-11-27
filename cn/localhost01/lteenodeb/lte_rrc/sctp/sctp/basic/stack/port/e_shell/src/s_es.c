/****************************************************************************
 *  FILE NAME  : s_es.c
 *
 *  DESCRIPTION: Contains the Encapsulation shell functions.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  02jan02    hbhondwe     SCTP_IPC_TCP ipc changes for tcp
 *  27Aug02    mrajpal                   SPR 4800
 *  30Oct02    mrajpal      SPR 5002     MSG_MAXIOVECLEN limit
 *  30Oct02    mrajpal      SPR 5024     IPC TCP with multiple stack
 *  30Oct02    mrajpal      SPR 5068     V6 header problem
 *  27Nov02    mrajpal      SPR 5110     V4 header problem on intel
 *  12Sep03    Manish Rajpal             Rel 5.0
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *  27Now03    Vivek Bansal              CSPL Changes
 *  28May10    Akanksha     SPR 20414    CSR 1-8614678-htons missing in pmtu msg *  24May10    Rajnesh      SPR 20411    Warning Fixed
 *  25JUL10    Rajnesh      SPR 20539    Memory Corruption Fix
 *  25Mar11    Debasri Sarkar            SPR 20860 changes
 *  10Aug11    Supreet Jain SPR 21004    CSR 5758
 *  1July13    Rohan Pawa   SPR 21141    CSR 50452
*   6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "ESHELL"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_esdef.h>
#include <s_tlif.h>
#include <s_uismty.h>
#include <s_in.h>
#include <s_ostmr.h>
#include <s_osmem.h>
#include <s_err.h>
#include <s_mhproc.h>
#include <s_mmmain.h>
#include <s_db.h>
#include <s_dbutil.h>
#include <s_uico.h>
#include <s_red.h>
#include <s_es.h>
#include <s_os.h>
#include <s_dbg.h>
#include <s_cspl.h>
#include <s_clpapi.h>
#include <s_clsmap.h>
#include <s_fsmmtu.h>
#include <s_dbtcb.h>

#ifdef SCTP_UI_MESSAGEBASED
sctp_client_info_st  sctp_client_list[SCTP_MAX_CLIENT_SUPPORTED];
#ifdef SCTP_RED_SUPPORT
sctp_U32bit g_sctp_rm_dest_id = 0;
#endif
#endif
extern sctp_U16bit g_dummy_sock;
/* ipc related infos */
#ifdef NO_INDICATION_THREAD
extern application_process_sctp_indications();
#endif
extern sig_ipc_mgr_glb_data_st  g_ipc_mgr_data[SIG_MAX_NUM_STACK];

/* This structure has all socket related information */
sctp_eshell_info_t sctp_eshell_info;

static void
sctp_send_msg_v4_iov(
    sctp_U8bit       *buffer,   /* an iovec must be passed here*/
    sctp_U32bit      size,      /* length of scatter array*/
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *to,
    sctp_sockaddr_st *from );

static void
sctp_send_msg_v6_iov(
    sctp_U8bit       *buffer,   /* an iovec must be passed here*/
    sctp_U32bit      size,      /* length of scatter array*/
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *to,
    sctp_sockaddr_st *from );


/* Function Ptr table used to send IPV4 and IPV6 Message */
sctp_sendmessage_fpt sctp_send_message[] =
{
    sctp_send_msg_v4,     /* For sending V4 message */
    sctp_send_msg_v6,     /* For sending V6 message */
    sctp_send_msg_v4_iov, /* For sending V4 scatter buffer */
    sctp_send_msg_v6_iov /* For sending V6 scatter buffer */
};



static sctp_return_t
sctp_es_add_address_inet(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_st       *p_addr_list,
    sctp_error_t           *p_ecode );

static sctp_return_t
sctp_es_del_addr(
    sctp_sockaddr_st       *p_addr,
    sctp_error_t           *p_ecode );

static sctp_return_t
sctp_es_add_address_udp(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_st       *p_addr_list,
    sctp_error_t           *p_ecode );
sctp_return_t sctp_TL_bind(
    sctp_U32bit        socket_id,
    sctp_sockaddr_st   *p_addr,
    sctp_U16bit        port );


#ifdef SCTP_OS_SOLARIS10
/******************************************************************************
**  Function Name       :sctp_TL_bind
**
**    Description       : This functions binds a given socket fd.
**
**    Returns           : SCTP_SUCCESS/SCTP_FAILURE
**
**    Functions called  : system call bind()
**
******************************************************************************/
sctp_return_t sctp_TL_bind(
    sctp_U32bit        socket_id,
    sctp_sockaddr_st   *p_addr,
    sctp_U16bit        port )
{
    sctp_S32bit        ret = SCTP_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_TL_bind" ) );

    if( p_addr->family == SCTP_AF_INET )
    {
        struct sockaddr_in local_addr;

        __MEMSET( &local_addr, 0, sizeof( struct sockaddr_in ) );
        local_addr.sin_family = AF_INET;
        local_addr.sin_port   = htons( port );
        local_addr.sin_addr.s_addr = htonl( p_addr->ip.v4.addr );

        ret = bind( socket_id, ( struct sockaddr * )&local_addr,
                    sizeof( local_addr ) );
    }

    else if( p_addr->family == SCTP_AF_INET6 )
    {
    }

    if( ret < 0 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_TL_bind, fail %s", strerror( errno ) ) );

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}
#endif

/***************************************************************************
 *    FUNCTION :
 *    sctp_eshell_config_address
 *
 *    DESCRIPTION: This function adds or deletes the stack local address
 *    on which data is sent and received.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_eshell_config_address(
    sctp_bindx_flag_et    flag,
    sctp_U32bit           num_local_addrs,
    sctp_sockaddr_st      *p_addr_list,
    sctp_error_t          *p_ecode )
{
    sctp_return_t     ret = SCTP_FAILURE;
    sctp_U32bit       count = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nModify stack address list" ) );

    switch( flag )
    {

        case SCTP_BINDX_ADD_ADDR:
            if( ( SCTP_IS_STACK_INET ) || ( SCTP_IS_STACK_INET6 ) )
            {
                ret = sctp_es_validate_local_addresses( num_local_addrs, p_addr_list,
                                                        p_ecode );

                if( SCTP_SUCCESS == ret )
                {
                    ret = sctp_es_add_address_inet( num_local_addrs, p_addr_list,
                                                    p_ecode );
                }
            }

            else
            {
                ret = sctp_es_add_address_udp( num_local_addrs, p_addr_list,
                                               p_ecode );
            }

            break;

        case SCTP_BINDX_REM_ADDR:
            for( count = 0; count < num_local_addrs; count++ )
            {
                ret = sctp_es_del_addr( &p_addr_list[count], p_ecode );
            }

            break;

        default:
            ret = SCTP_FAILURE;
            break;

    }

    return ret;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_es_add_address_inet
 *
 *    DESCRIPTION:
 *        This function adds the address to the stack in case stack is
 *    running oer raw IP (v4 or v6)
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_es_add_address_inet(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_st       *p_addr_list,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit         counter = 0;
    sctp_U32bit         count_v4 = sctp_eshell_info.receive.num_v4_addrs;
#ifdef SCTP_INET6_SUPPORT
    sctp_U32bit         pre_count_v6 = sctp_eshell_info.receive.num_v6_addrs;
    sctp_U32bit         *p_count_v6 = &sctp_eshell_info.receive.num_v6_addrs;
#endif
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nAdding the IPv4/IPv6 address" ) );

    for( counter = 0; counter < num_local_addrs; counter ++ )
    {
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, ( p_addr_list + counter ) );

        if( SCTP_AF_INET == p_addr_list[counter].family )
        {
            if( ( counter == 0 ) && ( count_v4 == 0 ) )
            {
                if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_RAW,
                                                         IPPROTO_SCTP, SCTP_NULL, 0, SCTP_FALSE,
                                                         &sctp_eshell_info.receive.sock.ipV4 ) )
                {
                    *p_ecode = E_SYSCALL_FAILURE;
                    return ( SCTP_FAILURE );
                }
            }

            /* Only INET addresses  */
            sctp_eshell_info.receive.ipv4_addrs[count_v4] =
                p_addr_list[counter];
            count_v4++;
            continue;
        }

#ifdef SCTP_INET6_SUPPORT

        sctp_eshell_info.receive.ipv6_address[*p_count_v6] =
            p_addr_list[counter];

        if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET6, SOCK_RAW, IPPROTO_SCTP,
                                                 &( p_addr_list[counter] ), 0, SCTP_TRUE,
                                                 &sctp_eshell_info.receive.sock.ipV6[*p_count_v6] ) )
        {
            *p_ecode = EINV_ADDRS;
            ( *p_count_v6 )++;
            break;
        }

        ( *p_count_v6 )++;
#endif
    }

#ifdef SCTP_INET6_SUPPORT

    if( counter != num_local_addrs )
    {
        while( ( *p_count_v6 ) > pre_count_v6 )
        {
            --( *p_count_v6 );
            sctp_TL_socket_close(
                sctp_eshell_info.receive.sock.ipV6[*p_count_v6] );
        }

        return SCTP_FAILURE;
    }

#endif
    /*set the new numbe rof v4 address only if everything is fine*/
    sctp_eshell_info.receive.num_v4_addrs = count_v4;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nAddresses added successfully" ) );
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_es_del_addr
 *
 *    DESCRIPTION:deleted the address from the stack
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_es_del_addr(
    sctp_sockaddr_st       *p_addr,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit         count = 0;
    sctp_sockaddr_st    *p_addr_list  = sctp_eshell_info.receive.ipv4_addrs;
    sctp_U32bit         *p_addr_count = &sctp_eshell_info.receive.num_v4_addrs;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nDeleting IPv4/IPv6 address" ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_addr );

#ifdef SCTP_INET6_SUPPORT

    /*check the address type */
    if( p_addr->family == SCTP_AF_INET6 )
    {
        p_addr_list  = sctp_eshell_info.receive.ipv6_address;
        p_addr_count = &sctp_eshell_info.receive.num_v6_addrs;
    }

#endif

    /*find the address to be deleted */
    for( count = 0; count < *p_addr_count; count++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( p_addr, &p_addr_list[count] ) )
        {
            break;
        }
    }

    if( SCTP_IS_STACK_INET6 )
    {
        sctp_TL_socket_close( sctp_eshell_info.receive.sock.ipV6[count] );
    }

    else if( SCTP_IS_STACK_UDP )
    {
        sctp_TL_socket_close( sctp_eshell_info.receive.udp_socket_id[count] );
    }

    ( *p_addr_count )--;

    /* shift the addresses */
    while( count < *p_addr_count )
    {
        if( SCTP_IS_STACK_INET6 )
        {
            sctp_eshell_info.receive.sock.ipV6[count] =
                sctp_eshell_info.receive.sock.ipV6[count + 1];
        }

        else if( SCTP_IS_STACK_UDP )
        {
            sctp_eshell_info.receive.udp_socket_id[count] =
                sctp_eshell_info.receive.udp_socket_id[count + 1];
        }

        p_addr_list[count] = p_addr_list[count + 1];
        count++;
    }

#ifndef SCTP_INET6_SUPPORT

    if( 0 == *p_addr_count )
    {
        sctp_TL_socket_close( sctp_eshell_info.receive.sock.ipV4 );
    }

#endif
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nAddresses deleted successfully" ) );
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_es_add_address_udp
 *
 *    DESCRIPTION:
 *        This function adds the address to the stack in case stack is
 *    running over udp
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_es_add_address_udp(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_st       *p_addr_list,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit       counter = 0;
    sctp_U32bit       *p_count = &sctp_eshell_info.receive.num_v4_addrs;
    sctp_U32bit       pre_count = sctp_eshell_info.receive.num_v4_addrs;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nAdding UDP addresses" ) );

    for( counter = 0; counter < num_local_addrs; counter ++ )
    {
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, ( p_addr_list + counter ) );

        if( SCTP_AF_INET != p_addr_list[counter].family )
        {
            /* Only INET addresses supported */
            *p_ecode = E_ADDR_TYPE_MISMATCH;
            break;
        }

        if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_DGRAM, 0,
                                                 &p_addr_list[counter], sctp_eshell_info.udp_port, SCTP_TRUE,
                                                 &sctp_eshell_info.receive.udp_socket_id[*p_count] ) )
        {
            *p_ecode = E_SYSCALL_FAILURE;
            break;
        }

        sctp_eshell_info.receive.ipv4_addrs[*p_count] =
            p_addr_list[counter];
        ( *p_count )++;
    }

    if( counter != num_local_addrs )
    {
        while( *p_count > pre_count )
        {
            ( *p_count )--;
            sctp_TL_socket_close(
                sctp_eshell_info.receive.sock.ipV6[*p_count] );
        }

        return SCTP_FAILURE;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nAddresses added successfully" ) );
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_pmtu_msg_v4
 *
 *    DESCRIPTION:
 *        This function Sends Ip message on the address. This is used in case
 *    Path MTU discovery.
 *    The IP V4 Header.
 *    struct ip {
 *        #ifdef _BIT_FIELDS_LTOH
 *        u_char    ip_hl:4,         //header length
 *        ip_v:4;    //version
 *        #else
 *        u_char    ip_v:4,        //version
 *        ip_hl:4;     //header length
 *        #endif
 *        u_char    ip_tos;        //type  of service
 *        short     ip_len;        //total length
 *        u_short   ip_id;         //identification
 *        short     ip_off;        //fragment offset field
 *        #define       IP_DF 0x4000     //dont fragment flag
 *        #define       IP_MF 0x2000     //more  fragments flag
 *        u_char    ip_ttl;        //time  to live
 *        u_char    ip_p;    //protocol
 *        u_short   ip_sum;        //checksum
 *        struct    in_addr ip_src, ip_dst;  //source and dest address
 *    };
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/

void
sctp_send_pmtu_msg_v4(
    sctp_tcb_st      *p_ass,
    sctp_U32bit      size,
    sctp_sockaddr_st *p_to )
{
    sctp_return_t   ret;
    sctp_U8bit      *buffer;
    SCTP_IP4        *ipheader;
    sctp_U32bit      saddr_index, daddr_index;

    /*ICMP structure for ICMP ECHO*/
    struct icmp *icmp_buff;
    sctp_pmtu_node_st *T;

    /*Get memory accprding to size*/
    buffer = ( sctp_U8bit * )__MALLOC( size );

    if( buffer == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "Could not get Memoty for PMTU buffer" ) );
        return;
    }


#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
            || ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )
    {
        return;
    }

#endif
#endif
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_send_pmtu_msg_v4: -> To " ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_to );
    /*SPR 21141 changes starts*/
    __MEMSET( buffer, 0 , size );
    /*SPR 21141 changes ends*/

    /*IP Header Formation*/
    ipheader = ( SCTP_IP4 * ) buffer;

    ipheader->ip_v = IP4_VERSION;

    ipheader->ip_hl  = sizeof( SCTP_IP4 ) >> 2;
    ipheader->ip_tos = IP_DSCP;

    ipheader->ip_len = ( short )size;
    /*SPR Fix 20414 Starts*/

#ifdef SCTP_OS_SOLARIS10
    ipheader->ip_len = htons( ipheader->ip_len );
#endif

    /*SPR Fix 20414 Stops*/


    ipheader->ip_id  = 0;

    /* Fix for CSR 1-2294328.Lynx OS does an additional byte swapping which
     * results in changing the value of off*/
#ifdef SIGTRAN_PORT_LYNX
    ipheader->ip_off = IP_DF;
#else
    ipheader->ip_off = htons( IP_DF );
#endif

    ipheader->ip_ttl = IPHEADER_TTL;

    if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_ass, p_to, &daddr_index ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_send_pmtu_msg_v4: -> Invalid remote address " ) );
        /* SPR_20411_FIX_START -- */
        __FREE( buffer );
        /* SPR_20411_FIX_END -- */
        return;
    }

    saddr_index = p_ass->transmit.remote[daddr_index].src_addr_index;

    ipheader->ip_src.s_addr =
        htonl( p_ass->p_ep->this_address[saddr_index].ip.v4.addr );

    ipheader->ip_p = 1; /*IP is carring ICMP payload*/

    ipheader->ip_dst.s_addr = htonl( p_to->ip.v4.addr );


    /*ICMP  Packet Formation*/

    icmp_buff = ( struct icmp * )( buffer + sizeof( SCTP_IP4 ) );
    icmp_buff->icmp_type = ICMP_ECHO;
    icmp_buff->icmp_code = 0;
    icmp_buff->icmp_id = 0;
    icmp_buff->icmp_seq = 1;


    /*Private information*/

    sctp_buffer_set_UlongAtOffset( buffer + sizeof( SCTP_IP4 ) + sizeof( struct icmp ),                                    0, p_ass->local_tag );

    sctp_buffer_set_UshortAtOffset( buffer + sizeof( SCTP_IP4 ) + sizeof( struct icmp ),
                                    4, p_ass->transmit.that_port );

    sctp_buffer_set_UshortAtOffset( buffer + sizeof( SCTP_IP4 ) + sizeof( struct icmp ),
                                    6, p_ass->p_ep->this_port );


    icmp_buff->icmp_cksum = 0;   /*Set checksum to 0 for computing checksum*/
    icmp_buff->icmp_cksum =
        sctp_calc_icmpcksum( ( sctp_U16bit * )icmp_buff, size - 20 );


    /*Search the pmtu node list*/
    for( T = ( sctp_pmtu_node_st * )lstFirst( &mtu_activelist ); T;
            T = ( sctp_pmtu_node_st * )lstNext( ( NODE * )T ) )
    {
        if( sctp_cmp_addr( &T->addr, p_to ) == SCTP_FAILURE )
        {
            continue;
        }

        /*Store the proposed mtu size*/
        T->proposed_mtu_size = size;
    }

    ret = sctp_TL_send_data(
              sctp_eshell_info.send.ipV4,
              buffer, size, 0, p_to, sctp_eshell_info.udp_port );

    if( ret == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,

                    E_INTERNAL_ERROR, "SCTP Send Failure" );
    }

    /*Free the allocated memory for pmtu buffer*/
    __FREE( buffer );
    return;

}

/***************************************************************************
 *    FUNCTION :
 *        sctp_calc_icmpcksum
 *
 *    DESCRIPTION:
 *        This function is used to calculate the checksum for ICMP packet
 *
 *    RETURN VALUE:
 *        checksum
 **************************************************************************/

sctp_U16bit
sctp_calc_icmpcksum( sctp_U16bit *buffer, sctp_U32bit len )
{
    sctp_U32bit nleft = len;
    sctp_U32bit sum = 0;
    sctp_U16bit *w = buffer;
    sctp_U16bit answer = 0;

    while( nleft > 1 )
    {
        sum += *w++;
        nleft -= 2;
    }

    if( nleft == 1 )
    {
        *( sctp_U8bit * )( &answer ) = *( sctp_U8bit * ) w;
        sum += answer;
    }

    sum = ( sum >> 16 ) + ( sum & 0xffff );
    sum += ( sum >> 16 );
    answer = ~sum;
    return ( answer );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_pmtu_msg_v6
 *
 *    DESCRIPTION:
 *        This function Sends IPv6 message on the address. This is used in
 *        case Path MTU discovery for IP version 6.
 *    The IP V6 Header Structure.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_pmtu_msg_v6(
    sctp_tcb_st       *p_ass,
    sctp_U32bit       size,
    sctp_sockaddr_st  *p_to )
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    SCTP_UNUSED_VARIABLE( size );
    SCTP_UNUSED_VARIABLE( p_to );
    /*to avoid the warning*/

#ifdef SCTP_INET6_SUPPORT
#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
            || ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )

    {
        return;
    }

#endif
#endif
#endif


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_send_pmtu_msg_v6: -> To " ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_to );

    /* In this release we are not supporting PMTU discovery so return from here */
    return;
#endif /* SCTP_INET6_SUPPORT */

    return;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_msg_v4
 *
 *    DESCRIPTION:
 *        This function is used to send a IPV4 datagram.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_msg_v4(
    sctp_U8bit       *buffer,
    sctp_U32bit      size,
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *to,
    sctp_sockaddr_st *from )
{
    sctp_U32bit     counter;
    sctp_return_t   ret;
    SCTP_IP4        *ipheader;
    sctp_U8bit      *ip_buff;
    sctp_U32bit     ip_size;

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_U16bit     src_port;
    sctp_U16bit     dst_port;
    sctp_tcb_st        *p_assoc = SCTP_NULL;
    sctp_U32bit      ep;
    sctp_U32bit      sctp_index;
    sctp_U32bit      dst_port_offset;
    sctp_U32bit      src_port_offset;
#endif

#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
            || ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )
    {
        return;
    }

#endif
#endif
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "send_msg_v4: From -> To " ) );

    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, from );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, to );

    if( SCTP_IS_STACK_INET || SCTP_IS_STACK_INET6 )
    {
        /*
         * The Stack is running over Raw IP sockets
         */

#ifdef SCTP_INET6_SUPPORT
        ip_buff = buffer + ( sizeof( SCTP_IP6 ) - sizeof( SCTP_IP4 ) );
        ip_size = size - ( sizeof( SCTP_IP6 ) - sizeof( SCTP_IP4 ) );
#else
        ip_buff = buffer;
        ip_size = size;
#endif /* SCTP_INET6_SUPPORT */

        ipheader = ( SCTP_IP4 * ) ip_buff;
        /*SPR 21141 changes starts*/
        __MEMSET( ip_buff, 0 , sizeof( SCTP_IP4 ) );
        /*SPR 21141 changes ends*/

        ipheader->ip_v = IP4_VERSION;

        /* This is number of 32 bit words  */
        ipheader->ip_hl = sizeof( SCTP_IP4 ) >> 2;

#ifdef SCTP_IP_HEADER_PARAMS
        dst_port_offset = SCTP_MAX_IP4_HEADER_SIZE + SCTP_OFFSET_DSTPORT;

        dst_port = sctp_buffer_get_UshortAtOffset( ip_buff, dst_port_offset );

        src_port_offset = SCTP_MAX_IP4_HEADER_SIZE + SCTP_OFFSET_SRCPORT ;
        src_port = sctp_buffer_get_UshortAtOffset( ip_buff, src_port_offset );
        sctp_db_locate_association( dst_port,
                                    ( to ),
                                    src_port,
                                    ( from ),
                                    &p_assoc );

        if( p_assoc )
        {

            ipheader->ip_tos = p_assoc->config.ip_header_params.tos;
            ipheader->ip_ttl = p_assoc->config.ip_header_params.ttl;

        }

        else
        {

            for( ep = 0; ep < sctp_db_main.max_ep; ep++ )
            {
                if( ( sctp_db_main.ep_table[ep].this_port == src_port )
                        && ( SCTP_SUCCESS == sctp_db_validate_local_addr( &sctp_db_main.ep_table[ep], from, &sctp_index ) ) )
                {
                    ipheader->ip_tos = sctp_db_main.ep_table[ep].sk_config.ip_header_params.tos;
                    ipheader->ip_ttl = sctp_db_main.ep_table[ep].sk_config.ip_header_params.ttl;
                }
            }
        }

#else

        if( SCTP_IS_ECN_CAPABLE )
        {
            ipheader->ip_tos = IP_DSCP | SCTP_ECT_BIT;
        }

        else
        {
            ipheader->ip_tos = IP_DSCP;
        }

        ipheader->ip_ttl = IPHEADER_TTL;

#endif /*SCTP_IP_HEADER_PARAMS*/

        ipheader->ip_len = ( short )ip_size;

#ifdef SCTP_OS_SOLARIS10
        ipheader->ip_len = htons( ipheader->ip_len );
#endif

        ipheader->ip_id    = 0;


#if defined(SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE)
        ipheader->ip_off = 0;
        /*to avoid the warning*/
        SCTP_UNUSED_VARIABLE( df_flag );
        /*to avoid the warning*/


#else

        if( SCTP_TRUE == df_flag )
        {
            /* Fix for CSR 1-2294328.Lynx OS does an additional byte swapping which
             * results in changing the value of off*/

#ifdef SIGTRAN_PORT_LYNX
            ipheader->ip_off = IP_DF;
#else
            ipheader->ip_off = htons( IP_DF );
#endif
        }

        else
        {
            ipheader->ip_off = 0;
        }

#endif /*SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE)*/

        ipheader->ip_src.s_addr = htonl( from->ip.v4.addr );
        ipheader->ip_p = IPPROTO_SCTP;
        ipheader->ip_dst.s_addr = htonl( to->ip.v4.addr );

        /* Send data on the IPv4 Socket. */
        ret = sctp_TL_send_data(
                  sctp_eshell_info.send.ipV4, ip_buff, ip_size, 0,
                  to, sctp_eshell_info.udp_port );

        if( ret == SCTP_FAILURE )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Failure" );
        }
    }

    else
    {
        /*
         * SCTP Running over UDP tunnled.
         */
        for( counter = 0; counter < sctp_eshell_info.receive.num_v4_addrs;
                counter ++ )
        {
            if( sctp_eshell_info.receive.ipv4_addrs[counter].ip.v4.addr ==
                    from->ip.v4.addr )
            {
                break;
            }
        }

        if( counter == sctp_eshell_info.receive.num_v4_addrs )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Request on unknown Address" );
        }

        else
        {
            /* raw socket option disabled, use udp socket*/
            ret = sctp_TL_send_data(
                      sctp_eshell_info.receive.udp_socket_id[counter],
                      buffer + SCTP_MAX_IP_HEADER_SIZE + sizeof( SCTP_UDPHDR ),
                      size - SCTP_MAX_IP_HEADER_SIZE - sizeof( SCTP_UDPHDR ), 0,
                      to, sctp_eshell_info.udp_port );

            if( ret == SCTP_FAILURE )
            {
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                            SCTPERROR_LEVEL_CRITICAL, 103, "SCTP Send Failure" );
            }
        }
    }

    return;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_msg_v4_iov
 *
 *    DESCRIPTION:
 *        This function is used to send a IPV4 datagram.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_msg_v4_iov(
    sctp_U8bit       *buffer,   /* an iovec must be passed here*/
    sctp_U32bit      size,      /* length of scatter array*/
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *to,
    sctp_sockaddr_st *from )
{
    sctp_U32bit     counter;
    sctp_return_t   ret;
    SCTP_IP4        *ipheader;
    sctp_U32bit     ip_size;

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_U16bit     src_port;
    sctp_U16bit     dst_port;
    sctp_tcb_st        *p_assoc = SCTP_NULL;
    sctp_U8bit      *ip_buff;
#endif

#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN           100/*max possible data chunks in a packet*/
#endif

    sctp_U32bit     msg_maxiovlen = MSG_MAXIOVLEN;

    sctp_iovec_st *p_iovec = ( sctp_iovec_st * )buffer;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "send_msg_v4_iovec: From -> To " ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, from );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, to );

    buffer = ( sctp_U8bit * )p_iovec[0].iov_base;

    /* Bug Id 81 fix s */
    SCTP_UNUSED_VARIABLE( buffer );
    /* Bug Id 81 fix e */
#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
            || ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )

    {
        return;
    }

#endif
#endif
#endif

    /*check that there aren't too many io-vector*/
    msg_maxiovlen += ( SCTP_IS_STACK_INET || SCTP_IS_STACK_INET6 ) ? 0 : 1;

    if( size > msg_maxiovlen )
    {
        /*
         * need to cut down, can't do much, will just copy all the buffers in
         * a single buffer and send it across
         */

        sctp_U8bit *p_buf = ( sctp_U8bit * )( p_iovec[1].iov_base ) + p_iovec[1].iov_len;
        sctp_U32bit i    = msg_maxiovlen - 1;
        sctp_U32bit buf_size = p_iovec[i].iov_len;

        __MEMCOPY( p_buf, p_iovec[i].iov_base, p_iovec[i].iov_len );
        p_iovec[i].iov_base = ( char * )p_buf;
        p_buf += p_iovec[i].iov_len;

        for( i = msg_maxiovlen; i < size; i++ )
        {
            __MEMCOPY( p_buf, p_iovec[i].iov_base, p_iovec[i].iov_len );
            buf_size += p_iovec[i].iov_len;
            p_buf += p_iovec[i].iov_len;
        }

        p_iovec[msg_maxiovlen - 1].iov_len = buf_size;
        size = msg_maxiovlen;
    }

    if( SCTP_IS_STACK_INET || SCTP_IS_STACK_INET6 )
    {
        /*
         * The Stack is running over Raw IP sockets
         */

#ifdef SCTP_INET6_SUPPORT
        ip_size = p_iovec[0].iov_len - ( sizeof( SCTP_IP6 ) - sizeof( SCTP_IP4 ) );
#else
        ip_size = p_iovec[0].iov_len;
#endif

        p_iovec[0].iov_len = sizeof( SCTP_IP4 );

        ipheader = ( SCTP_IP4 * ) p_iovec[0].iov_base;
        /*SPR 21141 changes starts*/

        __MEMSET( ipheader, 0 , p_iovec[0].iov_len );

        /*SPR 21141 changes ends*/

        ipheader->ip_v = IP4_VERSION;

        /* This is number of 32 bit words  */
        ipheader->ip_hl = sizeof( SCTP_IP4 ) >> 2;

        ipheader->ip_len = ( short )ip_size;
#ifdef SCTP_OS_SOLARIS10
        ipheader->ip_len = htons( ipheader->ip_len );
#endif
#ifdef SCTP_IP_HEADER_PARAMS
        ip_buff = ( sctp_U8bit * )p_iovec[1].iov_base ;

        dst_port = sctp_buffer_get_UshortAtOffset( ip_buff,
                                                   SCTP_OFFSET_DSTPORT );

        src_port = sctp_buffer_get_UshortAtOffset( ip_buff,
                                                   SCTP_OFFSET_SRCPORT );


        sctp_db_locate_association( dst_port,
                                    ( to ),
                                    src_port,
                                    ( from ),
                                    &p_assoc );

        if( p_assoc )
        {

            ipheader->ip_tos = p_assoc->config.ip_header_params.tos;
            ipheader->ip_ttl = p_assoc->config.ip_header_params.ttl;

        }

        else
        {
            if( SCTP_IS_ECN_CAPABLE )
            {
                ipheader->ip_tos = IP_DSCP | SCTP_ECT_BIT;
            }

            else
            {
                ipheader->ip_tos = IP_DSCP;
            }

            ipheader->ip_ttl = IPHEADER_TTL;
        }



#else

        if( SCTP_IS_ECN_CAPABLE )
        {
            ipheader->ip_tos = IP_DSCP | SCTP_ECT_BIT;
        }

        else
        {
            ipheader->ip_tos = IP_DSCP;
        }

        ipheader->ip_ttl = IPHEADER_TTL;

#endif /*SCTP_IP_HEADER_PARAMS*/



        ipheader->ip_len = ( short )ip_size;

#ifdef SCTP_OS_SOLARIS10
        ipheader->ip_len = htons( ipheader->ip_len );
#endif
        ipheader->ip_id    = 0;

#if defined(SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE)

        ipheader->ip_off = 0;

        /*to avoid the warning*/
        SCTP_UNUSED_VARIABLE( df_flag );
        /*to avoid the warning*/

#else

        if( SCTP_TRUE == df_flag )
        {

            /* Fix for CSR 1-2294328.Lynx OS does an additional byte swapping which
             * results in changing the value of off*/

#ifdef SIGTRAN_PORT_LYNX
            ipheader->ip_off = IP_DF;
#else
            ipheader->ip_off = htons( IP_DF );
#endif
        }

        else
        {
            ipheader->ip_off = 0;
        }

#endif /*SIGTRAN_PORT_VXWORKS || SIGTRAN_PORT_VX_PNE*/

        ipheader->ip_src.s_addr = htonl( from->ip.v4.addr );
        ipheader->ip_p = IPPROTO_SCTP;
        ipheader->ip_dst.s_addr = htonl( to->ip.v4.addr );


        /* Send data on the IPv4 Socket. */
        ret = sctp_TL_send_iovec(
                  sctp_eshell_info.send.ipV4, p_iovec, size, 0,
                  to, sctp_eshell_info.udp_port );

        if( ret == SCTP_FAILURE )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Failure" );
        }
    }

    else
    {
        /*
         * SCTP Running over UDP tunnled.
         */
        for( counter = 0; counter < sctp_eshell_info.receive.num_v4_addrs;
                counter ++ )
        {
            if( sctp_eshell_info.receive.ipv4_addrs[counter].ip.v4.addr ==
                    from->ip.v4.addr )
            {
                break;
            }
        }

        if( counter == sctp_eshell_info.receive.num_v4_addrs )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Request on unknown Address" );
        }

        else
        {
            /* raw socket option disabled, use udp socket*/
            ret = sctp_TL_send_iovec(
                      sctp_eshell_info.receive.udp_socket_id[counter],
                      &p_iovec[1], size - 1, 0,
                      to, sctp_eshell_info.udp_port );

            if( ret == SCTP_FAILURE )
            {
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                            SCTPERROR_LEVEL_CRITICAL, 103, "SCTP Send Failure" );
            }
        }
    }

    return;
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_send_msg_v6
 *
 *    DESCRIPTION:
 *        This function is used to send an IPv6 datagram.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_msg_v6(
    sctp_U8bit       *buffer,
    sctp_U32bit      size,
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *p_to,
    sctp_sockaddr_st *p_from )
{
#ifdef SCTP_INET6_SUPPORT
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_U32bit   counter = 0;

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_S32bit send_flags = 0;
    sctp_U8bit  *sctp_buffer_offset = SCTP_NULL;
    sctp_U16bit hoplimit = 0xFFFF;
    sctp_U16bit tclass   = 0xFFFF;

    sctp_iovec_st    p_iovec[SCTP_MAX_IP_SCATTER_BUFFERS];
#endif


#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
            || ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )
    {
        return;
    }

#endif
#endif
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "send_msg_v6: From -> To " ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_from );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_to );

#ifdef SCTP_IP_HEADER_PARAMS

    if( SCTP_IS_STACK_UDP )
    {
        sctp_buffer_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_buffer_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    sctp_get_ipv6_params_from_buffer( ( sctp_U8bit * )sctp_buffer_offset,
                                      p_to,
                                      p_from,
                                      &hoplimit,
                                      &tclass );

    /* Valid Range for Hoplimt and Tclass is 0-255.
     * Any other value means that the Kernel's Default value
     * should be used. */
    if( hoplimit <= 255 || tclass <= 255 )
    {
        p_iovec[0].iov_base = ( char * )buffer;
        p_iovec[0].iov_len = size;

        p_iovec[1].iov_base = ( char * )sctp_buffer_offset;
        p_iovec[1].iov_len = size - ( sctp_buffer_offset - buffer );

        /* store the hoplimt and tclass in the send_flags */
        send_flags = hoplimit;
        send_flags <<= 16;
        send_flags = send_flags | tclass;

        for( counter = 0; counter < sctp_eshell_info.receive.num_v6_addrs;
                counter ++ )
        {
            if( sctp_cmp_addr( &sctp_eshell_info.receive.ipv6_address[counter],
                               p_from )  == SCTP_SUCCESS )
            {
                break;
            }
        }

        if( counter == sctp_eshell_info.receive.num_v6_addrs )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Request on unknown Address" );
        }

        else
        {
            /* Send data on the IPv6 Socket. */
            ret = sctp_TL_send_iovec(
                      sctp_eshell_info.receive.sock.ipV6[counter],
                      &p_iovec[1],
                      1,
                      send_flags,  /* Send Flags */
                      p_to,
                      0 ); /* The destination port: Not valid in this case */

            if( SCTP_FAILURE == ret )
            {
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                            E_INTERNAL_ERROR, "SCTP Send Failure" );
            }
        }

    }

    else
#endif   /* #ifdef SCTP_IP_HEADER_PARAMS */
    {
        /*
         * This function is called only to send a IPv6 packet.
         * The stack is running on raw Ipv4 and Ipv6 packets.
         */

        for( counter = 0; counter < sctp_eshell_info.receive.num_v6_addrs;
                counter ++ )
        {
            if( sctp_cmp_addr( &sctp_eshell_info.receive.ipv6_address[counter],
                               p_from )  == SCTP_SUCCESS )
            {
                break;
            }
        }

        if( counter == sctp_eshell_info.receive.num_v6_addrs )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Request on unknown Address" );
        }

        else
        {
            /* Send data on the IPv6 Socket. */
            ret = sctp_TL_send_data(
                      sctp_eshell_info.receive.sock.ipV6[counter],
                      buffer + SCTP_MAX_IP_HEADER_SIZE,
                      size - SCTP_MAX_IP_HEADER_SIZE,
                      0,  /* Send Flags */
                      p_to,
                      0 ); /* The destination port: Not valid in this case */

            if( SCTP_FAILURE == ret )
            {
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                            E_INTERNAL_ERROR, "SCTP Send Failure" );
            }
        }
    }

#else

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( size );
    SCTP_UNUSED_VARIABLE( p_to );
    SCTP_UNUSED_VARIABLE( p_from );
    /*to avoid the warning*/


#endif /* SCTP_INET6_SUPPORT */
    SCTP_UNUSED_VARIABLE( df_flag );

}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_msg_v6_iov
 *
 *    DESCRIPTION:
 *        This function is used to send an IPv6 datagram.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_msg_v6_iov(
    sctp_U8bit       *buffer,
    sctp_U32bit      size,
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *p_to,
    sctp_sockaddr_st *p_from )
{

#ifdef SCTP_INET6_SUPPORT
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_U32bit   counter = 0;
    sctp_S32bit   send_flags = 0;

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_U16bit   hoplimit = 0xFFFF;
    sctp_U16bit   tclass = 0xFFFF;
#endif

#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN           100/*max possible data chunks in a packet*/
#endif

    sctp_U32bit     msg_maxiovlen = MSG_MAXIOVLEN;
    sctp_iovec_st   *p_iovec = ( sctp_iovec_st * )buffer;
#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
            || ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )
    {
        return;
    }

#endif
#endif
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "send_msg_v6_iov: From -> To " ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_from );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_to );

    /*
     * This function is called only to send a IPv6 packet.
     * The stack is running on raw Ipv4 and Ipv6 packets.
     */
    /*check that there aren't too many io-vector*/
    msg_maxiovlen += 1;

    if( size > msg_maxiovlen )
    {
        /*
         * need to cut down, can't do much, will just copy all the buffers in
         * a single buffer and send it across
         */

        sctp_U8bit *p_buf = ( sctp_U8bit * )( p_iovec[1].iov_base ) + p_iovec[1].iov_len;
        sctp_U32bit i    = msg_maxiovlen - 1;
        sctp_U32bit buf_size = p_iovec[i].iov_len;

        __MEMCOPY( p_buf, p_iovec[i].iov_base, p_iovec[i].iov_len );
        p_iovec[i].iov_base = ( char * )p_buf;
        p_buf += p_iovec[i].iov_len;

        for( i = msg_maxiovlen; i < size; i++ )
        {
            __MEMCOPY( p_buf, p_iovec[i].iov_base, p_iovec[i].iov_len );
            buf_size += p_iovec[i].iov_len;
            p_buf += p_iovec[i].iov_len;
        }

        p_iovec[msg_maxiovlen - 1].iov_len = buf_size;
        size = msg_maxiovlen;
    }

    for( counter = 0; counter < sctp_eshell_info.receive.num_v6_addrs;
            counter ++ )
    {
        if( sctp_cmp_addr( &sctp_eshell_info.receive.ipv6_address[counter],
                           p_from )  == SCTP_SUCCESS )
        {
            break;
        }
    }

    if( counter == sctp_eshell_info.receive.num_v6_addrs )
    {
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                    E_INTERNAL_ERROR, "SCTP Send Request on unknown Address" );
    }

    else
    {
#ifdef SCTP_IP_HEADER_PARAMS

        sctp_get_ipv6_params_from_buffer( ( sctp_U8bit * )p_iovec[1].iov_base,
                                          p_to,
                                          p_from,
                                          &hoplimit,
                                          &tclass );

        /* store the hoplimt and tclass in the send_flags */
        send_flags = hoplimit;
        send_flags <<= 16;
        send_flags = send_flags | tclass;
#endif
        /* Send data on the IPv6 Socket. */
        ret = sctp_TL_send_iovec(
                  sctp_eshell_info.receive.sock.ipV6[counter],
                  &p_iovec[1],
                  size - 1,
                  send_flags,  /* Send Flags */
                  p_to,
                  0 ); /* The destination port: Not valid in this case */

        if( SCTP_FAILURE == ret )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "SCTP Send Failure" );
        }
    }

#else
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( size );
    SCTP_UNUSED_VARIABLE( p_to );
    SCTP_UNUSED_VARIABLE( p_from );
    /*to avoid the warning*/


#endif /* SCTP_INET6_SUPPORT */
    SCTP_UNUSED_VARIABLE( df_flag );

}

#ifdef SCTP_INDICATION_THREAD
#ifndef SCTP_NO_QUEUE
#ifndef SCTP_UI_SOCK_API

static sctp_sema_t g_ind_sema;

static void *
sctp_indication_thread( void *A );

/***************************************************************************
 *    FUNCTION :
 *        sctp_signal_indication
 *
 *    DESCRIPTION:
 *        Called by the add notification function.
 *
 *    RETURN VALUE:
 *        void
 **************************************************************************/
void sctp_signal_indication()
{
    __SEMAPOST( &g_ind_sema );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_indication_thread
 *
 *    DESCRIPTION:
 *        This function removes the notifications from the queue.
 *
 *    RETURN VALUE:
 *        void*
 **************************************************************************/
void *
sctp_indication_thread( void *A )
{
    for( ;; )
    {
        __SEMAWAIT( g_ind_sema );
        sctp_remove_notifications_from_queue();
    }

    return NULL;
}
#endif /*SOCK API*/
#endif /*NO_INDICATION*/
#endif /*SCTP_INDICATION_THREAD*/


/***************************************************************************
 *    FUNCTION :
 *        sctp_es_validate_local_addresses
 *
 *    DESCRIPTION:
 *        This function validates the local addresses passed.
 *    This is done by opening a socket and binding it to address passed.
 *    If binding fails, it means that the address is not local address of m/c.
 *
 *    RETURN VALUE:
 *       SCTP_FAILURE/SCTP_SUCCESS
 **************************************************************************/
sctp_return_t
sctp_es_validate_local_addresses(
    sctp_U32bit           num_local_addrs,
    sctp_sockaddr_st      *p_addr_list,
    sctp_error_t          *p_ecode )
{
    sctp_U32bit         count;
    sctp_U32bit         tmp_socket_id;
    sctp_sockaddr_st    *addr_list = p_addr_list;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nValidating local addresses" ) );

    for( count = 0; count < num_local_addrs; count++ )
    {
        if( SCTP_FAILURE == sctp_chk_addr_unicast( &addr_list[count] ) )
        {
            *p_ecode = E_UNSUPPORTED_ADDR;

            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "\nValidation failure::Unsupported address" ) );
            return ( SCTP_FAILURE );
        }

        if( !( SCTP_IS_STACK_INET6 ) )
        {
            if( addr_list[count].family != SCTP_AF_INET )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nValidation failure::UERR_TRC address" ) );

                *p_ecode = E_UNSUPPORTED_ADDR;
                return SCTP_FAILURE;
            }
        }

        /*
         * This function will open the socket and then bind on port specified.
         * This is just to check the validity of the IP address.
         */
        if( SCTP_FAILURE == sctp_TL_open_socket( addr_list[count].family,
                                                 SOCK_RAW, IPPROTO_SCTP, &addr_list[count], 0, SCTP_FALSE,
                                                 &tmp_socket_id ) )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;

            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nValidation failure" ) );
            return ( SCTP_FAILURE );
        }

        /*
         * Subsequently close the same communication channel.
         */
        sctp_TL_socket_close( tmp_socket_id );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Validating local addresses succeeded" ) );
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *    sctp_init_ehsell
 *
 *    DESCRIPTION:
 *    This function will initialise the encapsulating shell. This
 *    includes opening number of UDP/IP/ICMP socket as requetsed. Create
 *    a thread to do a read on all these sockets and also call function
 *    to process timeout.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_init_ehsell(
    sctp_stack_type_et    stack_type,
    sctp_U16bit           standard_udp_port,
    sctp_U32bit           num_local_addrs,
    sctp_sockaddr_list_st local_addr_list,
    sctp_error_t          *p_ecode )
{
    sctp_return_t     ret = SCTP_FAILURE;


    /* SPR_20411_FIX_START -- */

    sig_iovec_ipc_buffer_st   message;
    /* SPR_20411_FIX_END -- */


#ifdef SCTP_INDICATION_THREAD
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE   /*NO QUEUE Overrides the INDICATION FLAG*/
    /* SPR 21004 Starts */
    sigtran_thread_t  th_id;
    /* SPR 21004 Ends */
    sigtran_error_t   sig_ecode;
#endif
#endif
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nsctp_init_ehsell" ) );

    /* SPR_20411_FIX_START -- */

    __MEMSET( &message, 0, sizeof( sig_iovec_ipc_buffer_st ) );
    /* SPR_20411_FIX_END -- */

    sctp_eshell_info.stack_type   = stack_type;

#ifndef SCTP_CLI_RECV_COM_THREAD
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "\nInit IPC Manager called for SIG_SCTP_RECV_STACK" ) );

    if( sig_init_ipc_mgr( SIG_SCTP_RECV_STACK, sctp_get_api_buf_len,
                          sctp_get_api_buf_len, SIGTRAN_NULL ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nInit IPC Manager Failed" ) );
        return SCTP_FAILURE;
    }

#endif

    if( ( SCTP_IS_STACK_INET ) || ( SCTP_IS_STACK_INET6 ) )
    {
        /* Init for the INET stack */
        ret = sctp_init_inet( num_local_addrs, local_addr_list, p_ecode );

        /* If INET init'ted then init INET6 if needed */
        if( ( SCTP_SUCCESS == ret ) && ( SCTP_IS_STACK_INET6 ) )
        {
            ret = sctp_init_inet6( num_local_addrs, local_addr_list, p_ecode );
        }
    }

    else
    {
        /* Init the stack for UDP */
        ret = sctp_init_udp( num_local_addrs, local_addr_list,
                             standard_udp_port, p_ecode );
    }

    if( SCTP_SUCCESS != ret )
    {
        return SCTP_FAILURE;
    }


#ifdef SIGTRAN_LOCAL_TIME_THREAD

    /* Thread to update local time value  */
    if( sctp_eshell_start_localtime_thread( p_ecode ) == SCTP_FAILURE )
    {
        return SCTP_FAILURE;
    }

#else
#ifdef SCTP_CLOCK_THREAD

    /* thread to update local time value  */
    if( sctp_eshell_start_clock_thread( p_ecode ) == SCTP_FAILURE )
    {
        return SCTP_FAILURE;
    }

#endif  /* SCTP_CLOCK_THREAD */
#endif

#ifdef SCTP_INDICATION_THREAD
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE   /*NO QUEUE Overrides the INDICATION FLAG*/

    if( ( 0 != __SEMAINIT( &g_ind_sema, 0, USYNC_THREAD, NULL ) ) ||
            ( sigtran_create_thread( ( sigtran_thread_t * )&th_id,
                                     &sctp_indication_thread, ( void * )NULL,
                                     &sig_ecode ) != SIGTRAN_SUCCESS ) )
    {
        *p_ecode = E_ESHELL_INIT_FAILED;
        return ( SCTP_FAILURE );
    }

#endif
#endif
#endif

#ifndef SCTP_CLI_RECV_COM_THREAD

    /*
     * This is spwaned only if there is no common thread for client, receive and
     * timer expiry. This function just checks for the messages from the lower layer
     * and timer expiry.
     */
    if( sctp_start_receive_timer_thread( p_ecode ) == SCTP_FAILURE )
    {
        return SCTP_FAILURE;
    }

#endif

    /* SPR_20411_FIX_START -- */

    sig_ipc_mgr_send_api( SIG_SCTP_STACK, g_dummy_sock, ( sigtran_void_t * )( &message ), sizeof( sig_iovec_ipc_buffer_st ), SIGTRAN_FALSE );
    /* SPR_20411_FIX_END -- */

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_inet6
 *
 *    DESCRIPTION:
 *        This function will initialise the INET6 TL. This shall create a
 *    send Raw socket and a receive raw socket and a raw socket for ICMP
 *    messages for IP version 6.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_init_inet6(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_list_st  local_addr_list,
    sctp_error_t           *p_ecode )
{
#ifdef SCTP_INET6_SUPPORT
    sctp_U32bit         counter = 0;
    sctp_U32bit         count_v6 = 0;
    sctp_U32bit         sctp_index = 0;
    sctp_sockaddr_st    *addr_list = ( sctp_sockaddr_st * ) local_addr_list;

    /*
     * If the stack is AF_INET6 then create the IPv6 Raw socket. Since NULL is
     * passed in the local address, the open function will not bind on the
     * socket.
     */
    /* CSR 1-6805471 Fix Start*/
#ifndef SCTP_OS_SOLARIS10

    if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET6, SOCK_RAW, IPPROTO_SCTP,
                                             SCTP_NULL, 0, SCTP_FALSE, &sctp_eshell_info.send.ipV6 ) )
    {
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }

#endif
    /* CSR 1-6805471 Fix End*/
    /*
     * Create Raw Socket for Receiving SCTP Messages
     */
    sctp_eshell_info.receive.num_v6_addrs = 0;

    for( counter = 0, count_v6 = 0; counter < num_local_addrs; counter ++ )
    {
        if( SCTP_AF_INET6 != addr_list[counter].family )
        {
            /* Only INET addresses supported */
            continue;
        }

        sctp_eshell_info.receive.ipv6_address[count_v6] =
            local_addr_list[counter];

        if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET6, SOCK_RAW, IPPROTO_SCTP,
                                                 &local_addr_list[counter], 0, SCTP_TRUE,
                                                 &sctp_eshell_info.receive.sock.ipV6[count_v6] ) )
        {
            counter = ( ( counter == 0 ) ? 0 : ( counter - 1 ) );
            *p_ecode = E_SYSCALL_FAILURE;
            break;
        }

        count_v6++;
        sctp_eshell_info.receive.num_v6_addrs++;
    }

    if( counter != num_local_addrs )
    {
        for( sctp_index = 0; sctp_index <= count_v6; sctp_index++ )
        {
            sctp_TL_socket_close( sctp_eshell_info.receive.sock.ipV6[sctp_index] );
        }

        sctp_TL_socket_close( sctp_eshell_info.send.ipV6 );
        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );

#else  /* SCTP_INET6_SUPPORT */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( num_local_addrs );
    SCTP_UNUSED_VARIABLE( local_addr_list );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    return ( SCTP_FAILURE );

#endif /* SCTP_INET6_SUPPORT */
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_inet
 *
 *    DESCRIPTION:
 *        This function will initialise the INET TL. This shall create a
 *    send Raw socket and a receive raw socket and a raw socket for ICMP
 *    messages for IP version 4.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_init_inet(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_list_st  local_addr_list,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit         counter;

    sctp_sockaddr_st    *addr_list = ( sctp_sockaddr_st * ) local_addr_list;
#ifdef SCTP_OS_SOLARIS10
    /* CSR 1-6805471 Fix Start*/
    sctp_sockaddr_list_st  local_addr_list1;
    sctp_sockaddr_st *temp_addr_list = ( sctp_sockaddr_st * ) local_addr_list1;
    /* CSR 1-6805471 Fix End */
#endif


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nInitialising IPv4/v6 addresses" ) );

    /*
     * validate local addresses. validates both v4 and v6.
     */
    if( sctp_es_validate_local_addresses( num_local_addrs,
                                          local_addr_list, p_ecode ) == SCTP_FAILURE )
    {
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    /*
     * Create the send Raw socket for AF_INET
     */
#ifndef SCTP_OS_SOLARIS10

    if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_RAW, IPPROTO_SCTP,
                                             SCTP_NULL, 0, SCTP_FALSE, &sctp_eshell_info.send.ipV4 ) )
    {
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }

#endif

    /*
     * Create Raw Socket for Receiving ICMP Messages, for PMTU
     */
    if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_RAW, IPPROTO_ICMP,
                                             SCTP_NULL, 0, SCTP_TRUE, &sctp_eshell_info.receive.sock.icmpV4 ) )
    {
        sctp_TL_socket_close( sctp_eshell_info.send.ipV4 );
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }

    /*
     * Create Raw Socket for Receiving SCTP Messages
     */
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)

    if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_RAW, IPPROTO_SCTP,
                                             &( addr_list[0] ), 0, SCTP_TRUE, &sctp_eshell_info.receive.sock.ipV4 ) )
#else
    if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_RAW, IPPROTO_SCTP,
                                             SCTP_NULL, 0, SCTP_TRUE, &sctp_eshell_info.receive.sock.ipV4 ) )
#endif
    {
        sctp_TL_socket_close( sctp_eshell_info.send.ipV4 );
        sctp_TL_socket_close( sctp_eshell_info.receive.sock.icmpV4 );
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }

#ifdef SCTP_OS_SOLARIS10
    temp_addr_list->ip.v4.addr = INADDR_ANY;
    /*CSR 1-6805471 Fix Start*/
    temp_addr_list->family = SCTP_AF_INET;
    /*CSR 1-6805471 Fix End*/

    if( sctp_TL_bind( g_ipc_mgr_data[SIG_SCTP_RECV_STACK].ipc_conn_info.
                      conn_info[sctp_eshell_info.receive.sock.ipV4].fd, &( temp_addr_list[0] ), 0 ) != SCTP_SUCCESS )
    {
        sctp_TL_socket_close( g_ipc_mgr_data[SIG_SCTP_RECV_STACK].ipc_conn_info.
                              conn_info[sctp_eshell_info.receive.sock.ipV4].fd );
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }



    sctp_eshell_info.send.ipV4 = sctp_eshell_info.receive.sock.ipV4;



#endif


    /*
     * Update local addresses parameters
     */
    sctp_eshell_info.receive.num_v4_addrs = 0;

    for( counter = 0; counter < num_local_addrs; counter++ )
    {
        if( SCTP_AF_INET == addr_list[counter].family )
        {
            sctp_eshell_info.receive.ipv4_addrs[sctp_eshell_info.receive.
                                                num_v4_addrs++] = addr_list[counter];
        }
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nInitialisation success" ) );
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_udp
 *
 *    DESCRIPTION:
 *        This function will initialise the UDP TL.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_init_udp(
    sctp_U32bit            num_local_addrs,
    sctp_sockaddr_list_st  local_addr_list,
    sctp_port_t            standard_udp_port,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit         counter;
    sctp_U32bit         sctp_index;
    sctp_sockaddr_st    *addr_list = ( sctp_sockaddr_st * ) local_addr_list;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Initialising UDP addresses" ) );

    sctp_eshell_info.receive.num_v4_addrs = num_local_addrs;

    for( counter = 0; counter < num_local_addrs; counter ++ )
    {
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, ( addr_list + counter ) );

        if( SCTP_AF_INET != addr_list[counter].family )
        {
            /* Only INET addresses supported */
            *p_ecode = E_ADDR_TYPE_MISMATCH;
            break;
        }

        sctp_eshell_info.receive.ipv4_addrs[counter] = addr_list[counter];

        /*
         * Open socket for receiving messages.
         */
        if( SCTP_FAILURE == sctp_TL_open_socket( AF_INET, SOCK_DGRAM, 0,
                                                 &addr_list[counter], standard_udp_port, SCTP_TRUE,
                                                 &sctp_eshell_info.receive.udp_socket_id[counter] ) )
        {
            counter = ( ( counter == 0 ) ? 0 : ( counter - 1 ) );
            *p_ecode = E_SYSCALL_FAILURE;
            break;
        }
    }

    if( counter != sctp_eshell_info.receive.num_v4_addrs )
    {
        for( sctp_index = 0; sctp_index <= counter; sctp_index++ )
        {
            sctp_TL_socket_close( sctp_eshell_info.receive.udp_socket_id[sctp_index] );
        }

        return ( SCTP_FAILURE );
    }

    sctp_eshell_info.udp_port = standard_udp_port; /* Standard UDP PORT */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nInitialisation success" ) );
    return ( SCTP_SUCCESS );
}


#ifdef SCTP_UI_MESSAGEBASED
/***************************************************************************
 *    FUNCTION :
 *        sctp_stack_proc_client_msg
 *
 *    DESCRIPTION:This function will process the message from SU/SM.
 *    RETURN VALUE:
 *        void
 **************************************************************************/
sctp_void_t sctp_stack_proc_client_msg(
    sctp_U8bit         *p_message,
    sctp_U32bit         buffer_size )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nProcess message from SU/SM" ) );

    sctp_process_api_prim_msg( ( sctp_U8bit * ) p_message, buffer_size,
                               SCTP_NULL );
    return;
}
#endif

#ifdef SCTP_UI_MESSAGEBASED
/***************************************************************************
 *    FUNCTION :
 *    sctp_init_stack_ipc
 *
 *    DESCRIPTION:
 *    This function will initialise the encapsulating shell. This
 *    includes opening number of UDP/IP/ICMP socket as requetsed.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_init_stack_ipc(
    sctp_U16bit        local_port,
    sctp_sockaddr_st   local_ip_address,
    sctp_port_type_et  port_type,
    sctp_error_t       *p_ecode )
{
    sig_ip_addr_st      local_sig_addr;
    sctp_U16bit         dest_id;
    sigtran_error_t     sig_ecode;
    sctp_U8bit          i_count;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Init Stack IPC" ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, &local_ip_address );

    SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( local_ip_address, local_port,
                                     local_sig_addr );

    if( SCTP_PORT_TYPE_UDP == port_type )
    {
        /*
         * Open the UDP socket for sending the data. Mark this as the default
         * write socket so that the same is used for sending the data on the
         * any new address of the SCTP client. Mark the read flag as false,
         * so that the same is not used for reading the data.
         * Also for the sendig socket specify no local address. So that this
         * function just creates the sockets and does not bind on them.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( SIG_SCTP_STACK, SIG_SOC_MODE_SERVER,
                                       SIG_SOC_TYPE_UDP, SIGTRAN_FALSE, SIGTRAN_TRUE, SIGTRAN_NULL,
                                       SIGTRAN_NULL, ( sig_read_cbk_func_ptr_t )sctp_ipc_recv_data,
                                       &dest_id, &sig_ecode ) )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( SCTP_FAILURE );
        }

        /*
         * Open the socket for receiving the data. Specify the local address for
         * reading the data. Specify that the same socket should be used for
         * reading the messages. Also mention that this socket should not used
         * for default write.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( SIG_SCTP_STACK, SIG_SOC_MODE_SERVER,
                                       SIG_SOC_TYPE_UDP, SIGTRAN_TRUE, SIGTRAN_FALSE, &local_sig_addr,
                                       SIGTRAN_NULL, ( sig_read_cbk_func_ptr_t )sctp_ipc_recv_data,
                                       &dest_id, &sig_ecode ) )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( SCTP_FAILURE );
        }
    }

    else
    {
        /*
         * Open the TCP socket. This is the listening socket. Therefore this
         * socket is never used for sending and receiving.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( SIG_SCTP_STACK, SIG_SOC_MODE_SERVER,
                                       SIG_SOC_TYPE_TCP, SIGTRAN_TRUE, SIGTRAN_TRUE, &local_sig_addr,
                                       SIGTRAN_NULL, ( sig_read_cbk_func_ptr_t )sctp_ipc_recv_data,
                                       &dest_id, &sig_ecode ) )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( SCTP_FAILURE );
        }
    }

    /*Reinitialize all unused application id to -1 to avoid
     * confussion between allocated 0 application id and
     * default initialize application id
     * */

    for( i_count = 0; i_count < SCTP_MAX_CLIENT_SUPPORTED; i_count++ )
    {
        sctp_client_list[i_count].application_id = -1;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nInit Stack IPC success" ) );
    return ( SCTP_SUCCESS );
}
#endif

#ifdef SCTP_UI_MESSAGEBASED
/******************************************************************************
**  Function Name       : sctp_ipc_recv_data
**
**    Description       : This function is responsible for receiving messages
**                        from SU/SM side. This function checks for the first
**                        message,creates a entry in the database. This also
**                        creates a mapping of the user id with the destination
**                        /handle is of the socket manager.
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t    sctp_ipc_recv_data(
    sigtran_U32bit      handle_id,
    sig_sock_mode_et    socket_mode,
    sig_soc_type_et     socket_type,
    sig_sock_fd_t      socket_id,
    sig_q_module_t      *p_from_q_module,
    sigtran_pvoid_t     *p_data,
    sigtran_U32bit      *p_data_len,
    sigtran_error_t     *p_ecode )
{
    sctp_U8bit          *p_message;
    sig_ip_addr_st      from_addr;
    sigtran_U16bit      dest_id;
    sctp_U16bit         mesg_src_id;
    sctp_api_id_t       api_id;
    sctp_U32bit         data_len_to_read;
    sctp_U32bit         connect_addr;
    sctp_U16bit         connect_port;
    sigtran_boolean_t   b_is_new_dest_id;
    sctp_U8bit          *p_data_buffer;
    sig_dest_type_et    dest_type;
    sctp_U8bit          arr_buf_len[4];
    sctp_U8bit          *p_buf_len;
    sctp_U16bit          su_id;
    sctp_U8bit          i_count;
    sctp_U16bit         application_id;
#ifdef SCTP_STRUCT_BASED


    sctp_msg_socket_st          sctp_socket;

    sctp_msg_register_su_st register1;
#endif
#ifdef SCTP_RED_SUPPORT
    /* SPR Fix 20539 Start --- */
    struct sockaddr_in     from_client;
    /* SPR Fix 20539 End --- */

    sctp_U32bit         numoctets;
    sctp_U32bit         event_type;
#else
    sctp_U32bit         api_buf_len;
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "\nFunction to recv data from SU/SM called" ) );

    if( ( SIG_SOC_MODE_SERVER == socket_mode ) && ( SIG_SOC_TYPE_TCP == socket_type ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "\nNew TCP connect request received" ) );
        /*
         * Then accept the connection, do the set destination for it and store the
         * q module for it in the database.
         */

        if( SIGTRAN_FAILURE == sig_accept_cli_conn( SIG_SCTP_STACK, socket_id,
                                                    ( sig_read_cbk_func_ptr_t )sctp_ipc_recv_data, &dest_id,
                                                    &connect_addr, &connect_port, p_ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "\nFailure in accepting new TCP connection" ) );
            return SIGTRAN_FAILURE;
        }

        *p_data = SIGTRAN_NULL;
        return SIGTRAN_SUCCESS;
    }

    p_message = __MESG_MALLOC( SCTP_APIS_MESG_POOL, SCTP_MAX_API_SIZE );

    if( SCTP_NULL == p_message )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nMemory allocation failure" ) );
        *p_ecode = ESIG_MEM_ALLOC_FAILED;
        return SIGTRAN_FAILURE;
    }

    p_data_buffer = p_message;

    /*
     * If the socket type is UDP, then read the message first.
     */
    if( SIG_SOC_TYPE_UDP == socket_type )
    {
        if( SIGTRAN_FAILURE ==
                sig_read( SIG_SCTP_STACK, socket_type, socket_id,
                          SCTP_MAX_API_SIZE, SIG_RECV_ALL, &from_addr, ( sigtran_pvoid_t )p_message,
                          p_data_len, p_ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "\nReading data on UDP socket failed" ) );

            __MESG_FREE( p_data_buffer );
            return SIGTRAN_FAILURE;
        }

        /*
         * The port number of the sending and receiving socket differs
         * by SCTP_SEND_RECV_PORT_OFFSET. Therefore perform rest of
         * the operations using the received port number minus the
         * port number offset.
         */

        from_addr.ip_addr_ut.addr_v4.port -= SCTP_SEND_RECV_PORT_OFFSET;

#ifdef SCTP_RED_SUPPORT
        /*
         * Extract the event type from the received message.
         */

        p_message = sctp_buffer_get_Ulong( p_message, &event_type );

        /*
         * If the event type is client message, then handle it as below. But
         * if the event is anything other than the client message, then there
         * should be a corresponding destination id to it. If absent then ignore
         * the error.
         */
        if( SCTP_EVT_CLIENT_MSG != event_type )
        {
            if( SIGTRAN_FAILURE ==
                    sig_get_dest_id( SIG_SCTP_STACK, socket_type,
                                     SOC_MGR_INVALID_HANDLE, from_addr.ip_addr_ut.addr_v4.addr,
                                     from_addr.ip_addr_ut.addr_v4.port, &dest_id,
                                     &b_is_new_dest_id, p_ecode ) )
            {
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            if( SIGTRAN_TRUE == b_is_new_dest_id )
            {
                /*
                 * Message received from an unknown destination, then ignore it.
                 */
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            /*
             * Get the qmodule corresponding to the destination id.
             */
            if( SIGTRAN_FAILURE ==
                    sig_get_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                                      dest_id, p_from_q_module, p_ecode ) )
            {
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            *p_data = p_data_buffer;
            return SIGTRAN_SUCCESS;
        }

        p_message = sctp_buffer_get_Ulong( p_message, &numoctets );
        p_message = sctp_buffer_get_Buffer( p_message, sizeof( struct sockaddr_in ),
                                            ( sctp_U8bit * )&from_client );
#else

        api_buf_len = sctp_buffer_get_UlongAtOffset( p_message, 12 );

        if( api_buf_len != *p_data_len )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "\nData read is not same as in the API header" ) );

            __MESG_FREE( p_data_buffer );
            return SIGTRAN_FAILURE;
        }

#endif
        /*
         * Extract the API Id from the buffer.
         */
        api_id = sctp_buffer_get_UshortAtOffset( p_message, 0 );

        /*
         * If the API Id is register SU or init stack request from SM,
         * then extract the destination id in the message and create a
         * mapping with the corresponding destination id in socket manager
         * database.
         */
        if( ( SCTP_API_REGISTER_SU_REQ == api_id ) ||
                ( SCTP_API_INIT_STACK_REQ == api_id ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "\nREGISTER SU/INIT STACK Req recvd" ) );

            /*
             * Get the destination id using the IP address and port number.
             */
            if( SIGTRAN_FAILURE ==
                    sig_get_dest_id( SIG_SCTP_STACK, socket_type,
                                     SOC_MGR_INVALID_HANDLE, from_addr.ip_addr_ut.addr_v4.addr,
                                     from_addr.ip_addr_ut.addr_v4.port, &dest_id,
                                     &b_is_new_dest_id, p_ecode ) )
            {
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            if( SIGTRAN_TRUE == b_is_new_dest_id )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nNew SU/SM is getting connected" ) );
                /*
                 * This is the case where if the user is new.
                 */

                if( SCTP_API_REGISTER_SU_REQ == api_id )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nMessage from SU" ) );
                    dest_type = SIG_DEST_TYPE_APP;
                }

                else
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nMessage from SM" ) );
                    dest_type = SIG_DEST_TYPE_SM;
                }

                if( SIGTRAN_FAILURE ==
                        sig_set_destination( SIG_SCTP_STACK, dest_type, dest_id,
                                             SIGTRAN_NULL, SIGTRAN_NULL, p_from_q_module, p_ecode ) )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                                ( "\nSet Destination for dest Id = [%u] failed", dest_id ) );
                    __MESG_FREE( p_data_buffer );
                    return SIGTRAN_FAILURE;
                }

                sig_cspl_set_immediate( sig_get_service_name( *p_from_q_module ),
                                        p_ecode );

                if( SIGTRAN_FAILURE ==
                        sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID, dest_id,
                                          *p_from_q_module, p_ecode ) )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                                ( "\nSet QMODULE for dest Id = [%u] failed", dest_id ) );
                    __MESG_FREE( p_data_buffer );
                    return SIGTRAN_FAILURE;
                }
            }

            else
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nSame user registering again" ) );

                /*
                 * This is the case if the user had gone down and it has come
                 * up again and it is now sending the API to the stack. So in
                 * this case just extract the q module as stored earlier.
                 */
                if( SIGTRAN_FAILURE ==
                        sig_get_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                                          dest_id, p_from_q_module, p_ecode ) )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                                ( "\nQMODULE for the dest id = [%u] not found", dest_id ) );
                    __MESG_FREE( p_data_buffer );
                    return SIGTRAN_FAILURE;
                }
            }

            mesg_src_id = sctp_buffer_get_UshortAtOffset( p_message, 4 );

            /*
             * Set the source id in the database. User will use the same in
             * APIs to send message to the application.
             */
            if( SIGTRAN_FAILURE ==
                    sig_set_in_map( SIG_SCTP_STACK, mesg_src_id, dest_id,  p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nCreating map between src id = [%u], dest id = [%u]",
                              mesg_src_id, dest_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            /*
             * Set the destination id in the global variable. The same can be used
             * when the asynchronous message is to be sent to the RM.
             */
#ifdef SCTP_RED_SUPPORT
            g_sctp_rm_dest_id = dest_id;
#endif

            *p_data = p_data_buffer;

            /*For SU verification*/
            if( SCTP_API_REGISTER_SU_REQ == api_id )
            {

#ifdef SCTP_STRUCT_BASED
                p_message = sctp_buffer_get_Buffer( p_message + 16, sizeof( sctp_msg_register_su_st ),
                                                    ( sctp_U8bit * )&register1 );
                su_id   = register1.su_id;
#else
                su_id = sctp_buffer_get_UshortAtOffset( p_message, 16 );
#endif
                sctp_client_list[su_id].dest_id = dest_id;

            }

            return SIGTRAN_SUCCESS;
        }

        else
        {
            /*
             * This is the where the API could be anything other than the
             * register SU APIs. This could be SM or RM APIs also. If it is
             * any other SU API, then the message should be ignored.
             */

            /*Check if it is socket api.If so check the dest id*/

            if( SCTP_API_SOCKET_REQ == api_id )
            {
                if( SIGTRAN_FAILURE ==
                        sig_get_dest_id( SIG_SCTP_STACK, socket_type,
                                         SOC_MGR_INVALID_HANDLE, from_addr.ip_addr_ut.addr_v4.addr,
                                         from_addr.ip_addr_ut.addr_v4.port, &dest_id,
                                         &b_is_new_dest_id, p_ecode ) )
                {
                    __MESG_FREE( p_data_buffer );
                    return SIGTRAN_FAILURE;
                }

#ifdef SCTP_STRUCT_BASED


                sctp_buffer_get_Buffer( p_message + 16, sizeof( sctp_msg_socket_st ),
                                        ( sctp_U8bit * )&sctp_socket );

                application_id = sctp_socket.appl_id;



#else
                application_id = sctp_buffer_get_UshortAtOffset( p_message, 16 );
#endif

                for( i_count = 0; i_count < SCTP_MAX_CLIENT_SUPPORTED; i_count++ )
                {
                    if( sctp_client_list[i_count].application_id
                            == application_id )
                    {
                        /*Got application id !! index must be the su_id*/
                        if( sctp_client_list[i_count].dest_id == dest_id )
                        {
                            break;
                        }
                    }
                }

                /*Invalid application*/
                if( i_count == SCTP_MAX_CLIENT_SUPPORTED )
                {
                    __MESG_FREE( p_data_buffer );
                    return SIGTRAN_FAILURE;
                }

            }

            mesg_src_id = sctp_buffer_get_UshortAtOffset( p_message, 4 );

            if( SIGTRAN_FAILURE ==
                    sig_get_from_map( SIG_SCTP_STACK, mesg_src_id, &dest_id,
                                      p_ecode ) )
            {
                /*
                 * If there exists no mapping corresponding to the source id in
                 * the api header.
                 */
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nMissing map between src id = [%u], dest id = [%u]",
                              mesg_src_id, dest_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            if( SIGTRAN_FAILURE ==
                    sig_get_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                                      dest_id, p_from_q_module, p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nQMODULE not found for dest_id = [%u]", dest_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            *p_data = p_data_buffer;
            return SIGTRAN_SUCCESS;
        }
    }

    else if( SIG_SOC_TYPE_TCP == socket_type )
    {
        p_buf_len = arr_buf_len;

        if( SIGTRAN_FAILURE ==
                sig_read( SIG_SCTP_STACK, socket_type, socket_id,
                          4, SIG_RECV_ALL, &from_addr, ( sigtran_pvoid_t )p_buf_len,
                          p_data_len, p_ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nRead from nw failed" ) );
            __MESG_FREE( p_data_buffer );
            sig_close_comm_channel( SIG_SCTP_STACK, ( sigtran_U16bit )handle_id );
            return SIGTRAN_FAILURE;
        }

        sig_util_extract_4bytes( &p_buf_len, &data_len_to_read );

        if( SIGTRAN_FAILURE ==
                sig_read( SIG_SCTP_STACK, socket_type, socket_id,
                          data_len_to_read, SIG_RECV_ALL, &from_addr, ( sigtran_pvoid_t )p_message,
                          p_data_len, p_ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nRead from nw failed" ) );
            __MESG_FREE( p_data_buffer );
            sig_close_comm_channel( SIG_SCTP_STACK, ( sigtran_U16bit )handle_id );
            return SIGTRAN_FAILURE;
        }

#ifdef SCTP_RED_SUPPORT
        /*
         * Extract the event type from the received message.
         */

        p_message = sctp_buffer_get_Ulong( p_message, &event_type );

        /*
         * If the event type is client message, then handle it as below. But
         * if the event is anything other than the client message, then there
         * should be a corresponding destination id to it. If absent then ignore
         * the error.
         */
        if( SCTP_EVT_CLIENT_MSG != event_type )
        {
            /*
             * Get the qmodule corresponding to the destination id.
             */
            if( SIGTRAN_FAILURE ==
                    sig_get_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                                      handle_id, p_from_q_module, p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nQMODULE not found for dest_id = [%u]", handle_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            *p_data = p_data_buffer;
            return SIGTRAN_SUCCESS;
        }

        p_message = sctp_buffer_get_Ulong( p_message, &numoctets );
        p_message = sctp_buffer_get_Buffer( p_message, sizeof( struct sockaddr_in ),
                                            ( sctp_U8bit * )&from_client );
#else
        api_buf_len = sctp_buffer_get_UlongAtOffset( p_message, 12 );

        if( api_buf_len != *p_data_len )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "\nData read is not same as in the API header" ) );

            __MESG_FREE( p_data_buffer );
            return SIGTRAN_FAILURE;
        }

#endif
        /*
         * Extract the API Id from the buffer.
         */
        api_id = sctp_buffer_get_UshortAtOffset( p_message, 0 );

        /*
         * If the API Id is register SU or init stack request from SM,
         * then extract the destination id in the message and create a
         * mapping with the corresponding destination id in socket manager
         * database.
         */
        if( ( SCTP_API_REGISTER_SU_REQ == api_id ) ||
                ( SCTP_API_INIT_STACK_REQ == api_id )
#ifdef SCTP_RED_SUPPORT
                || ( ( api_id >= SCTP_API_SET_STACK_RED_STATE_REQ ) &&
                     ( api_id <= SCTP_API_GET_STACK_HEALTH_REQ ) )
#endif
          )
        {
            /*
             * This is the case where if the user is new.
             */
            if( SCTP_API_REGISTER_SU_REQ == api_id )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nMessage from SU" ) );
                dest_type = SIG_DEST_TYPE_APP;
#ifdef SCTP_STRUCT_BASED
                /* SPR 20860 Starts */
                sctp_buffer_get_Buffer( p_message + 16,
                                        sizeof( sctp_msg_register_su_st ), ( sctp_U8bit * )&register1 );
                /* SPR 20860 Ends */
                su_id   = register1.su_id;
#else
                /*For Application verification in TCP style socket*/
                su_id = sctp_buffer_get_UshortAtOffset( p_message, 16 );
#endif

                /*For TCP style socket use handle_id*/
                sctp_client_list[su_id].dest_id = handle_id;

            }

            else
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nMessage from SM" ) );
                dest_type = SIG_DEST_TYPE_SM;
            }

            if( SIGTRAN_FAILURE ==
                    sig_set_destination( SIG_SCTP_STACK, dest_type, ( sigtran_U16bit )handle_id,
                                         SIGTRAN_NULL, SIGTRAN_NULL, p_from_q_module, p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nSet Destination for dest Id = [%u] failed", handle_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            sig_cspl_set_immediate( sig_get_service_name( *p_from_q_module ),
                                    p_ecode );

            if( SIGTRAN_FAILURE ==
                    sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                                      ( sigtran_U16bit )handle_id, *p_from_q_module, p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nSet QMODULE for dest Id = [%u] failed", handle_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            /*
             * Extract the Source Id from the buffer.
             */
            mesg_src_id = sctp_buffer_get_UshortAtOffset( p_message, 4 );

            /*
             * Set the source id in the database. User will use the same in
             * APIs to send message to the application.
             */
            if( SIGTRAN_FAILURE ==
                    sig_set_in_map( SIG_SCTP_STACK, mesg_src_id,
                                    ( sigtran_U16bit )handle_id,  p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nCreating map between src id = [%u], dest id = [%u]",
                              mesg_src_id, handle_id ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }

            /*
             * Set the destination id in the global variable. The same can be used
             * when the asynchronous message is to be sent to the RM.
             */
#ifdef SCTP_RED_SUPPORT
            g_sctp_rm_dest_id = handle_id;
#endif

            *p_data = p_data_buffer;
            return SIGTRAN_SUCCESS;
        }

        else if( SCTP_API_SOCKET_REQ == api_id )
        {
#ifdef SCTP_STRUCT_BASED
            p_message = sctp_buffer_get_Buffer( p_message + 16, sizeof( sctp_msg_socket_st ),
                                                ( sctp_U8bit * )&sctp_socket );


            application_id = sctp_socket.appl_id;





#else
            application_id = sctp_buffer_get_UshortAtOffset( p_message, 16 );
#endif

            for( i_count = 0; i_count < SCTP_MAX_CLIENT_SUPPORTED; i_count++ )
            {
                if( sctp_client_list[i_count].application_id
                        == application_id )
                {
                    /*Got application id !! index must be the su_id*/
                    if( sctp_client_list[i_count].dest_id == handle_id )
                    {
                        break;
                    }
                }
            }

            /*Invalid application*/
            if( i_count == SCTP_MAX_CLIENT_SUPPORTED )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nInvalid Application\n" ) );
                __MESG_FREE( p_data_buffer );
                return SIGTRAN_FAILURE;
            }
        }

        if( SIGTRAN_FAILURE ==
                sig_get_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                                  ( sigtran_U16bit )handle_id, p_from_q_module, p_ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "\nQMODULE not found for dest_id = [%u]", handle_id ) );
            /*
             * If there exists no mapping corresponding to the source id in
             * the api header.
             */
            __MESG_FREE( p_data_buffer );
            return SIGTRAN_FAILURE;
        }

        /*
         * Set the destination id in the global variable. The same can be used
         * when the asynchronous message is to be sent to the RM.
         */
#ifdef SCTP_RED_SUPPORT
        g_sctp_rm_dest_id = handle_id;
#endif
        *p_data = p_data_buffer;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Message recvived successfully" ) );
    return ( SIGTRAN_SUCCESS );
}
#endif /* SCTP_UI_MESSAGEBASED */


#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_RED_SUPPORT
/***************************************************************************
 *    FUNCTION :
 *    sctp_process_events
 *
 *    DESCRIPTION: This function processes Timer events, Client Msgs
 *                 and peer sctp msgs in case of redundancy. This function
 *                 is executed both at active and standby stacks.
 *
 *    RETURN VALUE:
 *    None
 *************************************************************************/
sctp_void_t sctp_process_events(
    sctp_U8bit         *p_buffer,
    sctp_U32bit         buffer_size )
{
    sctp_U32bit             event_type;
    sctp_timer_t            timer_id;
    sctp_U32bit             numoctets;
    sctp_U32bit             ip_header_offset;
    sctp_U32bit             size;
    sctp_sockaddr_st        src_addr;
    sctp_sockaddr_st        dst_addr;
    sctp_Boolean_t          CE_bit;
    sctp_Boolean_t          cookie_res_flag;
    sctp_Boolean_t          ip_header_present;
    sctp_TL_packet_st       *packet;

    /* SPR Fix 20539 Start --- */
    struct sockaddr_in     from_client;
    /* SPR Fix 20539 End --- */

    sctp_U32bit             total_size;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer_size );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Process events from RM" ) );

    p_buffer = sctp_buffer_get_Ulong( p_buffer, &event_type );

    switch( event_type )
    {
        case SCTP_EVT_TIMER:
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nEvent Timer Expiry" ) );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, &total_size );

            p_buffer = sctp_buffer_get_Buffer( p_buffer, sizeof( sctp_timer_t ),
                                               ( sctp_U8bit * ) & ( timer_id ) );

            if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY ) &&
                    ( sctp_stack_proc_state == SCTP_PROC_STATE_STOP ) )
            {
                return;
            }

            sctp_process_timer_expiry( timer_id, ( sctp_timer_buffer_st * )p_buffer );
            break;

        case SCTP_EVT_CLIENT_MSG:
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nEvent Message from SU/SM" ) );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, &numoctets );
            p_buffer = sctp_buffer_get_Buffer( p_buffer, sizeof( struct sockaddr_in ),
                                               ( sctp_U8bit * )&from_client );

            sctp_process_api_prim_msg( p_buffer, numoctets, SCTP_NULL );
            break;

        case SCTP_EVT_PEER_MSG:
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nEvent Message from PEER" ) );
            SCTP_TL_GETBUF( packet );

            if( packet == SCTP_NULL )
            {
                /* SCTP_TL_ADDBUF(packet);*/
                return;
            }

            p_buffer = sctp_buffer_get_Ulong( p_buffer, &total_size );

            p_buffer = sctp_buffer_get_Ulong( p_buffer, &ip_header_offset );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, &size );
            p_buffer = sctp_buffer_unpack_sockaddr( p_buffer, &src_addr );
            p_buffer = sctp_buffer_unpack_sockaddr( p_buffer, &dst_addr );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, ( sctp_U32bit * )&CE_bit );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, ( sctp_U32bit * )&cookie_res_flag );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, ( sctp_U32bit * )&ip_header_present );
            p_buffer = sctp_buffer_get_Buffer( p_buffer, size, &packet->buffer[0] );

            packet->ip_header_present = ip_header_present;

            if( sctp_stack_proc_state == SCTP_PROC_STATE_STOP )
            {
                if( packet != SCTP_NULL )
                {
                    SCTP_TL_ADDBUF( packet );
                }

                return;
            }

            packet->dst_addr = dst_addr;
            packet->src_addr = src_addr;
            packet->CE_bit   = CE_bit;

            sctp_process_TL_datagram( ip_header_offset, packet, size,
                                      cookie_res_flag );
            break;

        case SCTP_EVT_ICMP_MSG:
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nEvent ICMP Message" ) );
            p_buffer = sctp_buffer_get_Ulong( p_buffer, &total_size );

            p_buffer = sctp_buffer_get_Ulong( p_buffer, &size );
            p_buffer = sctp_buffer_unpack_sockaddr( p_buffer, &src_addr );

            if( sctp_stack_proc_state == SCTP_PROC_STATE_STOP )
            {
                return;
            }

            if( src_addr.family == SCTP_AF_INET )
            {
                sctp_process_icmpV4_message( p_buffer, size, &src_addr );
            }

            else if( src_addr.family == SCTP_AF_INET6 )
            {
                sctp_process_icmpV6_message( p_buffer, size, &src_addr );
            }

            break;

        default:
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nReceived Invalid Event Type" ) );
            }
    }
}
#endif
#endif

/******************************************************************************
**  Function Name       : sctp_get_api_buf_len
**
**    Description       : Function returns the length of the API buffer.
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sctp_get_api_buf_len(
    const sigtran_void_t    *p_msg,
    sigtran_U32bit          *p_msg_len
)
{
#ifdef SCTP_RED_SUPPORT
    *p_msg_len = sctp_buffer_get_UlongAtOffset( ( sctp_U8bit * )p_msg, 4 );

#else

    *p_msg_len = sctp_buffer_get_UlongAtOffset( ( sctp_U8bit * )p_msg, APIBUF_LENGTH_OFFSET );

#endif
    return SIGTRAN_SUCCESS;
}


#ifdef SCTP_IP_HEADER_PARAMS
/******************************************************************************
**  Function Name       : sctp_get_ipv6_params_from_buffer
**
**    Description       : This function extracts the values of the Hoplimit
**                        and Traffic class based on the buffer, src &
**                        destination address.
**
**    Returns           : None
**
******************************************************************************/
void sctp_get_ipv6_params_from_buffer( sctp_U8bit *buffer,
                                       sctp_sockaddr_st *p_to,
                                       sctp_sockaddr_st *p_from,
                                       sctp_U16bit      *hoplimit,
                                       sctp_U16bit      *tclass )
{
#ifdef SCTP_INET6_SUPPORT
    sctp_U16bit   dst_port = 0;
    sctp_U16bit   src_port = 0;
    sctp_tcb_st   *p_assoc = SCTP_NULL;
    sctp_U32bit   ep = 0;
    sctp_U32bit   sctp_index = 0;

    *hoplimit = 0xFFFF;
    *tclass = 0xFFFF;

    /* Locate the Association and get the HOPLIMIT/TCLASS */

    dst_port = sctp_buffer_get_UshortAtOffset( buffer, SCTP_OFFSET_DSTPORT );
    src_port = sctp_buffer_get_UshortAtOffset( buffer, SCTP_OFFSET_SRCPORT );

    sctp_db_locate_association( dst_port,
                                ( p_to ),
                                src_port,
                                ( p_from ),
                                &p_assoc );

    if( p_assoc )
    {
        *hoplimit = p_assoc->config.ip_header_params.hoplimit;
        *tclass   = p_assoc->config.ip_header_params.tclass;
    }

    else
    {
        for( ep = 0; ep < sctp_db_main.max_ep; ep++ )
        {
            if( ( sctp_db_main.ep_table[ep].this_port == src_port )
                    && ( SCTP_SUCCESS == sctp_db_validate_local_addr(
                             &sctp_db_main.ep_table[ep],
                             p_from,
                             &sctp_index ) ) )
            {
                *hoplimit = sctp_db_main.ep_table[ep].sk_config.ip_header_params.hoplimit;
                *tclass   = sctp_db_main.ep_table[ep].sk_config.ip_header_params.tclass;
            }
        }
    }

#else  /* SCTP_INET6_SUPPORT */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( p_to );
    SCTP_UNUSED_VARIABLE( p_from );
    SCTP_UNUSED_VARIABLE( hoplimit );
    SCTP_UNUSED_VARIABLE( tclass );
    /*to avoid the warning*/
#endif
}
#endif


