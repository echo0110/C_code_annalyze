/*******************************************************************************
 * FILE NAME:  s_dbtcb.c
 *
 * DESCRIPTION:
 *   This file is part of Database Module of  SCTP. This file includes
 *   routines related to database operations  of tcb , like locating
 *   associations, adding associations , deleting associations etc.
 *   Database Organisation
 *
 *   ---------------------
 *   Association Table
 *   ---------------------
 *     Database module contains association specific data structures. Data
 *   structure for an association is referred to as Transmission Control
 *   Block (TCB). These blocks are not destroyed and created at runtime At the
 *   time of initialisation maximum number of allowed associations are passed
 *   to database module and database module allocates memory for these many
 *   TCBs. If system allows database module may allocate a flat memory for
 *   maximum  number of TCBs and treat them as Array of TCBs. This array is
 *   called Global Association Table and is shown following picture.
 *   Since size of this array is large, there might be problem in allocating
 *   such large memory chunk in some some systems. These systems might allocate
 *   memory per TCB and arranging them in a linked-link. This linked-list will
 *   still be termed as Global Association Table.
 *   This is a porting effort and by default first option will be implemented.
 *
 *   At the time of adding an association (sctp_db_add_association), buffers
 *   from Global Tx-Buffer table are extraced  and added to association
 *   Tx-freelist and similarly buffers from Global Rx-Buffer table are extraced
 *   and added to association  Rx-freelist respectively. Number of transmit and
 *   receive buffers  moved will be equal  to max_tx_buffers max_sctp_assoc and
 *   max_tx_buffers/max_sctp_assoc respectively.
 *   This scheme reserves maximum number of buffers per association or
 *   transmiting and receiveing SCTP DATA chunks respectively. Whenever
 *   association require a transmit buffer (SCTP DATA chunk to be sent to peer)
 *   it extract it from Tx-freelist and add into its transmit queue. It remains
 *   there till the acknowledgement for  this datagram arrives, after that it
 *   is returned to Tx-freelist. Similarly whenever SCTP receives a DATA chunk
 *   from peer. A receive buffer is extracted from Rx-freelist and put into
 *   message list which is maintained per stream. When SU does a read (invokes
 *   sctp_api_receive, sctp_api_receive_ext) data is copied to buffer passed by
 *   SU  and receive buffer is returned to Rx-freelist.
 *
 *
 *    DATE  NAME     REFERENCE     REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *    10Oct  2000     hbhondwe                 Vxworks port.
 *    14 Feb 2001     Sandee/p Mahajan          PQ SPR for Rel 2.0
 *    31 Aug 2001     Yogesh Gahlaut           Rel 3.0 Changes
 *    26 Feb 2002     Yogesh Gahlaut           SPR 3672 fixes
 *    27Aug02         mrajpal                  SPR 4798, 4800
 *    23Nov02         mrajpal                  SPR 5088
 *    25Nov02         mrajpal                  SPR 5048
 *    25Nov02         mrajpal                  SPR 5093
 *    06Jan03         mrajpal                  SPR 5215
 *    06Jan03         mrajpal                  SPR 5217
 *    10Jan03         mrajpal                  SPR 5216
 *    11Feb03         mrajpal                  SPR 5335
 *    23July03        sharmam                  SPR 6200
 *    22Aug03         mrajpal                  SR 1-736944
 *    10Nov03         mrajpal                  SR 1-954224
 *    14Oct03         vibansal                 Changes for mixed
 *                                             IPv4-v6 support
 *    16Nov04         svakumar                 SPR 7192
 *    19Oct04         Amit Ray   REL 6.0       SCTP/FUNC-CONGCON-4
 *    21Oct04         Amit Ray   REL 6.0       SCTP/FUNC-IG-11
 *    22Oct04         Amit Ray   REL 6.0       SCTP/FUNC-MEM-2
 *    02Nov04         Svakumar   REL 6.0       SCTP/FUNC-CONGCON-5
 *    08Nov04         Svakumar   REL 6.0
 *    09May08         Raza       REL 6.2       SPR 18527
 *    24Jul08         Raza       REL 6.2       CSR 1-5026490, 1-5628824
 *    15july10        Sharad Kumar Yadav      REL 6.2.0 SPR 20526 For CSR# 1-7670333
 *    09Aug10         Sharad Kumar Yadav  SPR-20567 for CSR 1-6725936 Merging
 *                                        (Address parameter not default include hostname address)
 *    25Mar11         Akanksha   SPR 20859     Enhancement for Callback
 *    1May13       Pranav Sharma SPR 21107     CSR 44034
 *    1May13         Pranav      SPR 21127     CSR 40731-GCC 4.7 warning removal
 *    1July13        Rohan Pawa  SPR 21141     CSR 50452
 *    10Oct13        Rohan Pawa  SPR 21155       CSR 56067
 *    25Feb14        Rohan Pawa                CSR 69326
 *    15Sep14   Priyanka Mishra   Bug Id 22     CSR 85223
 *    28Apr14        Rohan Pawa  Bug-Id 3      CSR 77058
 *    01Jul15        Gurpreet   REL 6.2        Bug ID 61 For  CSR 105110
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/

#define SCTP_MODULE "DB"

#include <s_cotyps.h>
#include <s_db.h>
#include <s_dbbuf.h>
#include <s_dbg.h>
#include <s_fsmass.h>
#include <s_fsmmtu.h>
#include <s_fsmcng.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_osmutx.h>
#include <s_uismty.h>
#include <s_uisuif.h>
#include <s_stats.h>
#include <s_dbtcb.h>
#include <s_dbutil.h>
#include <s_select.h>
#include <s_mhproc.h>
#if defined(SCTP_STATS_ENABLED) && defined(SCTP_ASSOC_STATS_RECOVERY_SUPPORT)
#include <s_in.h>
#endif
/* Global structures */

sctp_db_main_st     sctp_db_main;
sctp_U8bit          *sctp_recv_thr_buff;
extern sctp_pool_id_t       g_sctp_timer_mem_poolid[2];
/*Fix Bug-Id 3 starts*/
extern sctp_lock_t     mtu_list_lock;
/*Fix Bug-Id 3 ends*/


/* Forward Declrations of local functions */
void
sctp_db_init_prot_params(
    sctp_U16bit    max_streams_per_assoc );

void
sctp_db_get_prot_params(
    sctp_prot_params_st *p_config,
    sctp_U32bit         server_id,
    sctp_Boolean_t      def_flag );

sctp_return_t
sctp_db_free_assoc_freelists(
    sctp_tcb_st    *p_ass );

void
sctp_db_init_assoc_queues(
    sctp_tcb_st  *p_ass );

sctp_return_t
sctp_db_buffer_mem_alloc(
    sctp_init_options_st   *init_options,
    sctp_error_t           *p_ecode );

sctp_return_t
sctp_db_alloc_buffer(
    sctp_init_options_st     *init_options,
    sctp_error_t             *p_ecode );

/**************************************************************************
 *  FUNCTION :  sctp_db_get_su_id
 *
 *  DESCRIPTION:This function is used to get the SU ID.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_get_su_id(
    sctp_U16bit            application_id,
    sctp_U16bit            *p_su_id )
{
#ifdef SCTP_UI_SU_MESSAGEBASED

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( application_id );
    SCTP_UNUSED_VARIABLE( p_su_id );
    /* SPR 21107 changes end */

    if( ( application_id < sctp_db_main.max_appl )
            && ( sctp_db_main.appl_list[application_id].open == 1 ) )
    {
        *p_su_id = sctp_db_main.appl_list[application_id].su_id;

        return( SCTP_SUCCESS );
    }

    else
    {
        return( SCTP_FAILURE );
    }

#else

    application_id = 0; /*to remove compilation warnings*/
    p_su_id = SCTP_NULL;

    /* SPR 21127 starts */
    SCTP_UNUSED_VARIABLE( application_id );
    SCTP_UNUSED_VARIABLE( p_su_id );
    /* SPR 21127 ends */

    return( SCTP_FAILURE );
#endif

}

/**************************************************************************
 *  FUNCTION :  sctp_db_assoc_rx_buffer_num
 *
 *  DESCRIPTION:
 *    This function will return the number of receive buffers in an assoc.
 *    that can accomodate the data of specified length.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_rx_buffer_num(
    sctp_tcb_st   *p_ass,
    sctp_U32bit   length )
{
    sctp_U32bit buffer_num;

    if( length <= SCTP_MIN_FRAGSIZE )
    {
        buffer_num  =  lstCount( &p_ass->rx_freelist.list_min ) +
                       lstCount( &p_ass->rx_freelist.list_med ) +
                       lstCount( &p_ass->rx_freelist.list_max );

        return  buffer_num;
    }

    if( length <= SCTP_MED_FRAGSIZE )
    {
        buffer_num  =  lstCount( &p_ass->rx_freelist.list_med ) +
                       lstCount( &p_ass->rx_freelist.list_max );

        return buffer_num;

    }

    if( length <= SCTP_MAX_FRAGSIZE )
    {
        buffer_num  =  lstCount( &p_ass->rx_freelist.list_max );

        return buffer_num;
    }

    return 0;

}

/***************************************************************************
 *  FUNCTION :  sctp_db_assoc_tx_buffer_num
 *
 *  DESCRIPTION:
 *    This function will return the total number of tx buffers in an assoc.
 *    that can accomodate data of specified length.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_tx_buffer_num(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    length )
{
    sctp_U32bit buffer_num;

    if( length <= SCTP_MIN_FRAGSIZE )
    {
        buffer_num  =  lstCount( &p_ass->tx_freelist.list_min ) +
                       lstCount( &p_ass->tx_freelist.list_med ) +
                       lstCount( &p_ass->tx_freelist.list_max );

        return  buffer_num;
    }

    if( length <= SCTP_MED_FRAGSIZE )
    {
        buffer_num  =  lstCount( &p_ass->tx_freelist.list_med ) +
                       lstCount( &p_ass->tx_freelist.list_max );

        return buffer_num;
    }

    if( length <= SCTP_MAX_FRAGSIZE )
    {
        buffer_num  =  lstCount( &p_ass->tx_freelist.list_max );

        return buffer_num;
    }

    return 0;
}

/***************************************************************************
 *  FUNCTION :  sctp_db_assoc_tx_buffer_num_recompute
 *
 *  DESCRIPTION:
 *    This function will return the total number and also calculate the
 *    number of max,min and med buffer required in a optimized fashion.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_tx_buffer_num_recompute(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    length,
    sctp_U32bit    data_size,
    sctp_U8bit     *max_buff_req,
    sctp_U8bit    *med_buff_req,
    sctp_U8bit    *min_buff_req )
{
    sctp_U32bit buffer_num = 0;
    sctp_U32bit temp_data_size;

    sctp_U8bit temp_max_no = lstCount( &p_ass->tx_freelist.list_max );
    sctp_U8bit temp_med_no = lstCount( &p_ass->tx_freelist.list_med );
    sctp_U8bit temp_min_no = lstCount( &p_ass->tx_freelist.list_min );

    /*Store the data size to avoid signed manipulation*/
    temp_data_size = data_size;

    *max_buff_req = 0;
    *med_buff_req = 0;
    *min_buff_req = 0;

    while( data_size > 0 )
    {
        /*This is done to keep track that when data_size
         *bcomes -ve as it is an unsigned variable*/

        if( data_size > temp_data_size )
        {
            break;
        }

        if( ( length <= SCTP_MIN_FRAGSIZE ) || ( data_size <= SCTP_MIN_FRAGSIZE ) )
        {
            if( temp_min_no )
            {
                ( *min_buff_req )++;
                buffer_num++;
                data_size -= length;
                temp_min_no--;
                continue;
            }
        }

        if( ( length <= SCTP_MED_FRAGSIZE ) || ( data_size <= SCTP_MED_FRAGSIZE ) )
        {
            if( temp_med_no )
            {
                ( *med_buff_req )++;
                buffer_num++;
                data_size -= length;
                temp_med_no--;
                continue;
            }
        }

        if( ( length <= SCTP_MAX_FRAGSIZE ) || ( data_size <= SCTP_MAX_FRAGSIZE ) )
        {
            if( temp_max_no )
            {
                ( *max_buff_req )++;
                buffer_num++;
                data_size -= length;
                temp_max_no--;
                continue;
            }

            else
            {
                if( temp_med_no )
                {
                    ( *med_buff_req )++;
                    buffer_num++;
                    data_size -= SCTP_MED_FRAGSIZE;
                    temp_med_no--;
                    continue;
                }

                else if( temp_min_no )
                {
                    ( *min_buff_req )++;
                    buffer_num++;
                    data_size -= SCTP_MIN_FRAGSIZE;
                    temp_min_no--;
                    continue;
                }

                /*Buffer can not be allocated any way.Advanced tecnique is failed
                  too*/
                else
                {
                    return 0;
                }
            }
        }

        /*SPR 21141 changes starts*/
        else
        {
            return 0;
        }

        /*SPR 21141 changes ends*/
    }

    return buffer_num;
}


/***************************************************************************
 *  FUNCTION :  sctp_db_assoc_rx_window
 *
 *  DESCRIPTION:
 *    This function will return the receiving window of an associaton.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_rx_window(
    sctp_tcb_st *p_ass )
{
    sctp_U32bit length;

    length = ( lstCount( &p_ass->rx_freelist.list_min ) * SCTP_MIN_FRAGSIZE ) +
             ( lstCount( &p_ass->rx_freelist.list_med ) * SCTP_MED_FRAGSIZE ) +
             ( lstCount( &p_ass->rx_freelist.list_max ) * SCTP_MAX_FRAGSIZE );

    return length;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_assoc_tx_window
 *
 *  DESCRIPTION:
 *    This function will return the transmission window of an associaton.
 *    Transmission window is buffer space available to send the data.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_tx_window(
    sctp_tcb_st *p_ass )
{
    sctp_U32bit length;

    length = ( lstCount( &p_ass->tx_freelist.list_min ) * SCTP_MIN_FRAGSIZE ) +
             ( lstCount( &p_ass->tx_freelist.list_med ) * SCTP_MED_FRAGSIZE ) +
             ( lstCount( &p_ass->tx_freelist.list_max ) * SCTP_MAX_FRAGSIZE );

    return length;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_assoc_max_tx_window
 *
 *  DESCRIPTION:
 *    This function will return the maximum possible transmission window
 *    of an associaton. Transmission window is buffer space available to
 *    send the data.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_max_tx_window(
    sctp_tcb_st     *p_tcb )
{
    return p_tcb->transmit.wnd_size;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_assoc_max_rx_window
 *
 *  DESCRIPTION:
 *    This function will return the maximum possible receiving window
 *    of an associaton. Receving window is buffer space available to
 *    receive data.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_assoc_max_rx_window(
    sctp_tcb_st     *p_tcb )
{
    return p_tcb->receive.wnd_size;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_add_hash
 *
 *  DESCRIPTION:
 *    This funcation inserts an endpoint in the desired hash table.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_add_hash(
    sctp_U32bit    flag,
    sctp_ep_st     *p_ep )
{
    sctp_ep_st    *p_prev, *p_curr;
    sctp_hash_st  *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_add_hash " ) );

    if( SCTP_LISTEN_HASH == flag )
    {
        p_hash = sctp_db_get_lhash_table( p_ep->this_port );
    }

    else if( SCTP_BIND_HASH == flag )
    {
        p_hash = sctp_db_get_bhash_table( p_ep->this_port );
    }

    if( SCTP_NULL != p_hash )
    {
        SCTP_LOCK( p_hash->lock );
        p_prev = SCTP_NULL;
        p_curr = ( sctp_ep_st * )lstFirst( &( p_hash->chain ) );

        if( SCTP_NULL == p_curr )
        {
            /* List is empty insert as first node */
            lstInsert( &( p_hash->chain ), ( NODE * )p_prev, ( NODE * )p_ep );
        }

        else
        {
            /* find the appropriate location */
            for( ; p_curr != SCTP_NULL;
                    p_prev = p_curr, p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
            {
                if( p_curr->this_port >= p_ep->this_port )
                {
                    break;
                }
            }

            /*SPR 5044*/
            /* check that it is not duplicate */
            if( ( p_curr != SCTP_NULL ) &&
                    ( p_curr->ep_id == p_ep->ep_id ) )
            {
                /*already there, jsut return success*/
                SCTP_UNLOCK( p_hash->lock );
                return ( SCTP_SUCCESS );
            }

            /*SPR 5044*/

            lstInsert( &( p_hash->chain ), ( NODE * )p_prev, ( NODE * )p_ep );
        }

        SCTP_SOCK_SET_HASHED( p_ep->flags, 1 );

        SCTP_UNLOCK( p_hash->lock );

        return ( SCTP_SUCCESS );
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_del_hash
 *
 *  DESCRIPTION:
 *    This funcation deletes an endpoint from the desired hash table.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_del_hash(
    sctp_U32bit    flag,
    sctp_ep_st     *p_ep )
{
    sctp_hash_st  *p_hash = SCTP_NULL;
    sctp_ep_st    *p_curr = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_del_hash " ) );

    if( SCTP_LISTEN_HASH == flag )
    {
        p_hash = sctp_db_get_lhash_table( p_ep->this_port );
    }

    else if( SCTP_BIND_HASH == flag )
    {
        p_hash = sctp_db_get_bhash_table( p_ep->this_port );
    }

    if( SCTP_NULL != p_hash )
    {
        SCTP_LOCK( p_hash->lock );
        p_curr = ( sctp_ep_st * )lstFirst( &( p_hash->chain ) );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->ep_id == p_ep->ep_id )
            {
                lstDelete( &( p_hash->chain ), ( NODE * )p_ep );
                SCTP_SOCK_SET_HASHED( p_ep->flags, 0 );

                SCTP_UNLOCK( p_hash->lock );
                return SCTP_SUCCESS;
            }
        }

        SCTP_UNLOCK( p_hash->lock );
    }

    return ( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_add_ass_hash
 *
 *  DESCRIPTION:
 *    This funcation inserts an assoc in the desired hash table.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_add_ass_hash(
    sctp_tcb_verf_st    *p_ass )
{
    sctp_hash_st  *p_hash = SCTP_NULL;
    sctp_tcb_verf_st *p_prev, *p_curr;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_add_ass_hash " ) );

    p_hash = sctp_db_get_ass_hash_table( p_ass->local_tag );

    /*Bug Id 22 fix starts */
    /*if(SCTP_NULL != p_hash)*/
    /*{*/
    /*Bug Id 22 fix ends*/
    SCTP_LOCK( p_hash->lock );
    p_prev = SCTP_NULL;
    p_curr = ( sctp_tcb_verf_st * )lstFirst( &( p_hash->chain ) );

    if( SCTP_NULL == p_curr )
    {
        lstInsert( &( p_hash->chain ), ( NODE * )p_prev, ( NODE * )p_ass );
    }

    else
    {
        /* find the appropriate location */
        for( ; p_curr != SCTP_NULL;
                p_prev = p_curr, p_curr = ( sctp_tcb_verf_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->local_tag >= p_ass->local_tag )
            {
                break;
            }
        }

        lstInsert( &( p_hash->chain ), ( NODE * )p_prev, ( NODE * )p_ass );
    }

    SCTP_UNLOCK( p_hash->lock );
    return ( SCTP_SUCCESS );
    /*Bug Id 22 fix starts*/
    /*}*/
    /*return (SCTP_FAILURE);*/
    /*Bug Id 22 fix ends*/
}


/***************************************************************************
 *  FUNCTION :    sctp_db_del_ass_hash
 *
 *  DESCRIPTION:
 *    This funcation deletes an assoc from the desired hash table.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_del_ass_hash(
    sctp_tcb_verf_st    *p_ass )
{
    sctp_hash_st        *p_hash = SCTP_NULL;
    sctp_tcb_verf_st    *p_curr = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_del_ass_hash " ) );

    p_hash = sctp_db_get_ass_hash_table( p_ass->local_tag );

    if( SCTP_NULL != p_hash )
    {
        SCTP_LOCK( p_hash->lock );
        p_curr = ( sctp_tcb_verf_st * )lstFirst( &( p_hash->chain ) );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_tcb_verf_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->local_tag == p_ass->local_tag )
            {
                lstDelete( &( p_hash->chain ), ( NODE * )p_ass );
                SCTP_UNLOCK( p_hash->lock );
                return SCTP_SUCCESS;
            }
        }

        SCTP_UNLOCK( p_hash->lock );
    }

    return ( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_free_verf_ass_node
 *
 *  DESCRIPTION:
 *    This funcation return free verf_ass node
 *
 *  RETURN VALUE:
 *    sctp_tcb_verf_st *
 ****************************************************************************/
sctp_tcb_verf_st *
sctp_db_get_free_verf_ass_node(
    sctp_U32bit    local_tag )
{
    sctp_U32bit counter, index_found = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_free_verf_ass" ) );

    for( counter = 0; counter < sctp_db_main.max_assoc; counter++ )
    {
        if( ( sctp_db_main.verf_ass_table[counter].flags == SCTP_TRUE )
                && ( sctp_db_main.verf_ass_table[counter].local_tag == local_tag ) )
        {
            return &( sctp_db_main.verf_ass_table[counter] );
        }

        else if( sctp_db_main.verf_ass_table[counter].flags != SCTP_TRUE )
        {
            index_found = counter;
        }
    }

    sctp_db_main.verf_ass_table[index_found].local_tag = local_tag;
    sctp_db_main.verf_ass_table[index_found].flags = SCTP_TRUE;

    return &( sctp_db_main.verf_ass_table[index_found] );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_verf_ass_node
 *
 *  DESCRIPTION:
 *    This funcation return verf_ass node corresponding to local tag
 *
 *  RETURN VALUE:
 *    sctp_tcb_verf_st *
 ****************************************************************************/
sctp_tcb_verf_st *
sctp_db_get_verf_ass_node(
    sctp_U32bit    local_tag )
{
    sctp_U32bit counter = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_verf_ass" ) );

    for( counter = 0; counter < sctp_db_main.max_assoc; counter++ )
    {
        if( ( sctp_db_main.verf_ass_table[counter].flags == SCTP_TRUE )
                && ( sctp_db_main.verf_ass_table[counter].local_tag == local_tag ) )
        {
            return &( sctp_db_main.verf_ass_table[counter] );
        }
    }

    return SCTP_NULL;
}
/***************************************************************************
 *  FUNCTION :    sctp_db_delete_assoc_verf
 *
 *  DESCRIPTION:
 *    This funcation delete verf_ass node corresponding to local tag
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_delete_assoc_verf(
    sctp_tcb_st          *p_tcb )
{
    sctp_tcb_verf_st     *p_verf_ass = SCTP_NULL;
    sctp_verf_list_ref_tcb_st *verf_ref_tcb = SCTP_NULL;
    sctp_U32bit            i_count;


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "In function sctp_db_delete_assoc_verf" ) );
    p_verf_ass = sctp_db_get_verf_ass_node( p_tcb->local_tag );

    if( p_verf_ass == NULL )
    {
        return SCTP_FAILURE;
    }

    /*Extract the first tcb in the list*/
    verf_ref_tcb = ( sctp_verf_list_ref_tcb_st * )lstFirst( &p_verf_ass->assoc_list );

    /*Rotate through the assoc list in case same tag is assigned to multiple
     *tcb
     */
    for( i_count = 0; i_count < p_verf_ass->n_assoc; i_count++ )
    {
        if( verf_ref_tcb->p_assoc_ref == p_tcb )
        {

            lstDelete( &p_verf_ass->assoc_list, ( NODE * )verf_ref_tcb );
            break;
        }

        else
        {
            verf_ref_tcb =
                ( sctp_verf_list_ref_tcb_st * )lstNext( ( NODE * )verf_ref_tcb );

            if( verf_ref_tcb == SCTP_NULL )
            {
                /*One node in the list.Pick Up the first node*/
                verf_ref_tcb = ( sctp_verf_list_ref_tcb_st * )lstFirst(
                                   &p_verf_ass->assoc_list );
            }
        }
    }

    /*Free the pool*/
    sigtran_free_buffer( SIG_SCTP_STACK, SCTP_FALSE,
                         &( g_sctp_timer_mem_poolid[1] ), ( sigtran_buffer_t * )&verf_ref_tcb );



    p_verf_ass->n_assoc--;

    if( p_verf_ass->n_assoc == 0 )
    {
        p_verf_ass->flags = 0;
        sctp_db_del_ass_hash( p_verf_ass );
    }

    return ( SCTP_SUCCESS );
}
/***************************************************************************
 *  FUNCTION :    sctp_db_add_assoc_verf
 *
 *  DESCRIPTION:
 *    This funcation add verf_ass node corresponding to local tag
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_add_assoc_verf(
    sctp_tcb_st          *p_tcb )
{
    sctp_tcb_verf_st     *p_verf_ass = SCTP_NULL;

    /*Take a dummy tcb structure*/
    sctp_verf_list_ref_tcb_st *verf_ref_tcb = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_add_assoc_verf" ) );

    verf_ref_tcb = ( sctp_verf_list_ref_tcb_st * )
                   sigtran_get_buffer( SIG_SCTP_STACK, SCTP_FALSE,
                                       &( g_sctp_timer_mem_poolid[1] ),
                                       sizeof( sctp_verf_list_ref_tcb_st ) );


    if( verf_ref_tcb == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "Could not allocate node for verification tcb" ) );
        return SCTP_FAILURE;
    }

    SCTPLOCK();
    p_verf_ass = sctp_db_get_free_verf_ass_node( p_tcb->local_tag );

    if( p_verf_ass->n_assoc == 0 )
    {
        lstInit( &p_verf_ass->assoc_list );
        sctp_db_add_ass_hash( p_verf_ass );
    }

    /*Store the actual tcb refference*/
    verf_ref_tcb->p_assoc_ref = p_tcb;

    /*Add the verification reffered tcb in the list*/
    lstAdd( &( p_verf_ass )->assoc_list, ( NODE * )verf_ref_tcb );

    /*Store the verification reffered reference for backtracking*/
    p_verf_ass->n_assoc++;
    SCTPUNLOCK();
    return ( SCTP_SUCCESS );
}
/***************************************************************************
 *  FUNCTION :    sctp_db_locate_association_verf
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to
 *    locate an association in the Database Module Data Structure.
 *    It will return a valid association Id.

 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_locate_association_verf(
    sctp_U16bit        that_port,
    sctp_sockaddr_st   *that_address,
    sctp_U16bit        this_port,
    sctp_sockaddr_st   *this_address,
    sctp_U32bit        verf_tag,
    sctp_tcb_st        **pp_ass )
{
    sctp_U32bit  sctp_index;
    sctp_tcb_verf_st   *p_ep;
    sctp_tcb_st  *p_assoc;
    sctp_hash_st *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_locate_association_verf" ) );

    *pp_ass = SCTP_NULL;
    p_hash = sctp_db_get_ass_hash_table( verf_tag );

    if( SCTP_NULL  != p_hash )
    {
        p_ep = ( sctp_tcb_verf_st * )lstFirst( &p_hash->chain );

        for( ; p_ep != SCTP_NULL; p_ep = ( sctp_tcb_verf_st * )lstNext( ( NODE * )p_ep ) )
        {
            sctp_verf_list_ref_tcb_st *verf_ref_tcb = SCTP_NULL;

            if( p_ep->local_tag < verf_tag )
            {
                continue;
            }

            if( p_ep->local_tag > verf_tag )
            {
                break;
            }

            verf_ref_tcb = ( sctp_verf_list_ref_tcb_st * )lstFirst( &p_ep->assoc_list );

            if( p_ep->n_assoc > 1 )
            {
                for( ; verf_ref_tcb != SCTP_NULL;
                        verf_ref_tcb =
                            ( sctp_verf_list_ref_tcb_st * )lstNext( ( NODE * )verf_ref_tcb ) )

                {
                    p_assoc = verf_ref_tcb->p_assoc_ref;

                    if( ( this_port == p_assoc->p_ep->this_port )
                            && ( that_port == p_assoc->transmit.that_port )
                            && ( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_assoc,
                                                                               that_address, &sctp_index ) )
                            && ( SCTP_SUCCESS == sctp_db_validate_local_addr( p_assoc->p_ep,
                                                                              this_address, &sctp_index ) ) )

                    {
                        *pp_ass = p_assoc;
                        return ( SCTP_SUCCESS );
                    }
                }
            }

            else
            {
                /* Change For CSR 1-5026490 Fix */
                if( verf_ref_tcb )
                {
                    /* Change For CSR 1-5026490 Fix */
                    *pp_ass = verf_ref_tcb->p_assoc_ref;
                    return ( SCTP_SUCCESS );
                }
            }
        }
    }

    return SCTP_FAILURE;
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_callback_info
 *
 *  DESCRIPTION:
 *    This funcation return call back function and appl specific data.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_db_get_callback_info(
    sctp_U16bit           appl_id,
    sctp_callback_func_t  *pp_callback,
    sctp_Pvoid_t          *pp_appl_data )
{
#ifndef SCTP_UI_SU_MESSAGEBASED

    /* SCTPLOCK(); */ /*SPR 3672 */
    if( ( appl_id < sctp_db_main.max_appl )
            && ( sctp_db_main.appl_list[appl_id].open == 1 ) )
    {
        *pp_callback  = sctp_db_main.appl_list[appl_id].callback;
        *pp_appl_data = sctp_db_main.appl_list[appl_id].p_appl_data;

        /* SCTPUNLOCK(); */ /* SPR 3672 */
        return ( SCTP_SUCCESS );
    }

#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( appl_id );
    /*to avoid the warning*/

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    *pp_callback  = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    *pp_callback  = ( sctp_callback_func_t )SCTP_NULL;
#endif
    *pp_appl_data = SCTP_NULL;

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_get_dafault_rwnd
 *
 *  DESCRIPTION:
 *    This function will return default receiver window size.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_get_dafault_rwnd(
    void )
{
    sctp_U32bit length;

    length = ( sctp_db_main.min_rx_buffers * SCTP_MIN_FRAGSIZE ) +
             ( sctp_db_main.med_rx_buffers * SCTP_MED_FRAGSIZE ) +
             ( sctp_db_main.max_rx_buffers * SCTP_MAX_FRAGSIZE );

    return length;

}


/***************************************************************************
 *  FUNCTION :    sctp_db_get_number_txbuffers
 *
 *  DESCRIPTION:
 *    This function will return max number of tx buffers.
 *
 *  RETURN VALUE:
 *    sctp_U32bit
 **************************************************************************/
sctp_U32bit
sctp_db_get_number_txbuffers(
    void )

{
    return sctp_db_main.min_tx_buffers * SCTP_MIN_FRAGSIZE
           + sctp_db_main.med_tx_buffers * SCTP_MED_FRAGSIZE
           + sctp_db_main.med_tx_buffers * SCTP_MAX_FRAGSIZE;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_init_prot_params
 *
 *  DESCRIPTION:
 *    This function will inilialise the default protocol parameters
 *    used by SCTP protocol stack. Some of these values are initialised
 *    to values suggested in Protocol specification rest are initialised
 *    with values requested by System management via stack init API.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 **************************************************************************/
void
sctp_db_init_prot_params(
    sctp_U16bit    max_streams_per_assoc )
{

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_init_prot_params " ) );

    sctp_db_main.sctp_def_prot_params.rto_init = SCTP_DEFAULT_RTO_INIT;
    sctp_db_main.sctp_def_prot_params.rto_min  = SCTP_DEFAULT_RTO_MIN;
    sctp_db_main.sctp_def_prot_params.rto_max  = SCTP_DEFAULT_RTO_MAX;
    sctp_db_main.sctp_def_prot_params.max_burst = SCTP_DEFAULT_MAX_BURST;
    sctp_db_main.sctp_def_prot_params.rto_alpha_percent
        = SCTP_DEFAULT_RTO_ALPHA_PERCENT;
    sctp_db_main.sctp_def_prot_params.rto_beta_percent
        = SCTP_DEFAULT_RTO_BETA_PERCENT;
    sctp_db_main.sctp_def_prot_params.valid_cookie_life
        = SCTP_DEFAULT_VALID_COOKIE_LIFE;
    sctp_db_main.sctp_def_prot_params.assoc_max_retrans
        = SCTP_DEFAULT_ASSOC_MAX_RTX;
    sctp_db_main.sctp_def_prot_params.path_max_retrans
        = SCTP_DEFAULT_PATH_MAX_RTX;
    sctp_db_main.sctp_def_prot_params.max_init_retrans
        = SCTP_DEFAULT_MAX_INIT_RTX;
    sctp_db_main.sctp_def_prot_params.max_init_path_retrans
        = SCTP_DEFAULT_MAX_INIT_PATH_RTX;
    sctp_db_main.sctp_def_prot_params.init_timeo
        = SCTP_DEFAULT_INIT_TIMEOUT;
    sctp_db_main.sctp_def_prot_params.num_in_streams = max_streams_per_assoc;
    sctp_db_main.sctp_def_prot_params.num_out_streams = max_streams_per_assoc;

    /* number of in, out streams set at time of init */
    sctp_db_main.init_in_streams = max_streams_per_assoc;
    sctp_db_main.init_out_streams = max_streams_per_assoc;

    sctp_db_main.trx_cong_reg_def.no_cong_lvl_abate
        = SCTP_DEFAULT_NO_CONG_LEVEL_ABATE;
    sctp_db_main.trx_cong_reg_def.no_cong_lvl_onset
        = SCTP_DEFAULT_NO_CONG_LEVEL_ONSET;
    sctp_db_main.trx_cong_reg_def.low_cong_lvl_abate
        = SCTP_DEFAULT_LOW_CONG_LEVEL_ABATE;
    sctp_db_main.trx_cong_reg_def.low_cong_lvl_onset
        = SCTP_DEFAULT_LOW_CONG_LEVEL_ONSET;
    sctp_db_main.trx_cong_reg_def.med_cong_lvl_abate
        = SCTP_DEFAULT_MED_CONG_LEVEL_ABATE;
    sctp_db_main.trx_cong_reg_def.med_cong_lvl_onset
        = SCTP_DEFAULT_MED_CONG_LEVEL_ONSET;
    sctp_db_main.trx_cong_reg_def.high_cong_lvl_abate
        = SCTP_DEFAULT_HIGH_CONG_LEVEL_ABATE;
    sctp_db_main.trx_cong_reg_def.high_cong_lvl_onset
        = SCTP_DEFAULT_HIGH_CONG_LEVEL_ONSET;

    sctp_db_main.rcv_cong_reg_def.no_cong_lvl_abate
        = SCTP_DEFAULT_NO_CONG_LEVEL_ABATE;
    sctp_db_main.rcv_cong_reg_def.no_cong_lvl_onset
        = SCTP_DEFAULT_NO_CONG_LEVEL_ONSET;
    sctp_db_main.rcv_cong_reg_def.low_cong_lvl_abate
        = SCTP_DEFAULT_LOW_CONG_LEVEL_ABATE;
    sctp_db_main.rcv_cong_reg_def.low_cong_lvl_onset
        = SCTP_DEFAULT_LOW_CONG_LEVEL_ONSET;
    sctp_db_main.rcv_cong_reg_def.med_cong_lvl_abate
        = SCTP_DEFAULT_MED_CONG_LEVEL_ABATE;
    sctp_db_main.rcv_cong_reg_def.med_cong_lvl_onset
        = SCTP_DEFAULT_MED_CONG_LEVEL_ONSET;
    sctp_db_main.rcv_cong_reg_def.high_cong_lvl_abate
        = SCTP_DEFAULT_HIGH_CONG_LEVEL_ABATE;
    sctp_db_main.rcv_cong_reg_def.high_cong_lvl_onset
        = SCTP_DEFAULT_HIGH_CONG_LEVEL_ONSET;

    sctp_db_main.sctp_def_prot_params.hb_interval = SCTP_DEFAULT_HB_INTERVAL;
    /*SPR 4798*/
    sctp_db_main.sctp_def_prot_params.bundling_time = SCTP_BUNDLING_TIME;
    /*SPR 4798*/
    sctp_db_main.sctp_def_prot_params.sack_period   = SCTP_DELAYACK_TIME;

    sctp_db_main.sctp_def_prot_params.sack_frequency = SCTP_SACK_FREQUENCY;

    sctp_db_main.sctp_def_prot_params.mtu_size      = SCTP_MAX_MTUSIZE;
    /*SPR 4800*/
    sctp_db_main.eshell_select_time = 100; /*default is 100 ms */
    /*SPR 4800*/
    sctp_db_main.sctp_def_prot_params.adaption_ind =
        SCTP_NULL_ADAPTION_IND;
    sctp_db_main.sctp_def_prot_params.allow_asconf = SCTP_TRUE;
    sctp_db_main.sctp_def_prot_params.want_mapped  = SCTP_FALSE;
    sctp_db_main.sctp_def_prot_params.mtu_size      = SCTP_MAX_MTUSIZE;
    sctp_db_main.sctp_def_prot_params.hb_info_length = SCTP_MIN_HBINFO_LEN;

    /*Rel 6.2.0  change start */
    sctp_db_main.sctp_def_prot_params.hb_max_burst = SCTP_HB_MAX_BURST;
    /*Rel 6.2.0  change end */

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_db_main.sctp_def_prot_params.ip_header_params.tos = IP_DSCP;
    sctp_db_main.sctp_def_prot_params.ip_header_params.ttl = IPHEADER_TTL;
    /*Rel 6.2.0  change start */
#ifdef SCTP_INET6_SUPPORT
    sctp_db_main.sctp_def_prot_params.ip_header_params.hoplimit = IPHEADER_HOPLIMIT;
    sctp_db_main.sctp_def_prot_params.ip_header_params.tclass = IPHEADER_TCLASS;
#endif
    /*Rel 6.2.0  change end */

#endif


    /*remainig params are 0 being static */
    return;

}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_prot_params
 *
 *  DESCRIPTION:
 *    Get initial values of protocol parameters, either from Server
 *    in case of a new socket inheriting parameters from server socket
 *    or protocol default values in case of a new client socket.
 *
 *  RETURN VALUE:
 *   None
 ****************************************************************************/
void
sctp_db_get_prot_params(
    sctp_prot_params_st *p_config,
    sctp_U32bit         server_id,
    sctp_Boolean_t      def_flag )
{
    sctp_prot_params_st *protocol;
    sctp_ep_st *p_ser;

    if( SCTP_FALSE == def_flag )
    {
        p_ser = &sctp_db_main.ep_table[server_id];
        protocol = &p_ser->sk_config;
    }

    else
    {
        SCTPLOCK();
        protocol = &sctp_db_main.sctp_def_prot_params;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_prot_params" ) );

    p_config->rto_init = protocol->rto_init;
    p_config->rto_min = protocol->rto_min;
    p_config->rto_max = protocol->rto_max;
    p_config->rto_alpha_percent = protocol->rto_alpha_percent;
    p_config->rto_beta_percent = protocol->rto_beta_percent;
    p_config->valid_cookie_life = protocol->valid_cookie_life;
    p_config->assoc_max_retrans = protocol->assoc_max_retrans;
    p_config->path_max_retrans = protocol->path_max_retrans;
    p_config->max_init_retrans = protocol->max_init_retrans;
    p_config->max_init_path_retrans = protocol->max_init_path_retrans;
    p_config->init_timeo = protocol->init_timeo;
    p_config->num_in_streams = protocol->num_in_streams;
    p_config->num_out_streams = protocol->num_out_streams;
    p_config->hb_interval = protocol->hb_interval;
    p_config->hb_info_length = protocol->hb_info_length;
    /*SPR 4798*/
    p_config->bundling_time = protocol->bundling_time;
    /*SPR 4798*/
    p_config->sack_period = protocol->sack_period;
    p_config->sack_frequency = protocol->sack_frequency;

    p_config->mtu_size      = protocol->mtu_size;
    p_config->re_use_addr   = protocol->re_use_addr;
    p_config->adaption_ind  = protocol->adaption_ind;
    p_config->dont_fragment = protocol->dont_fragment;
    p_config->allow_asconf  = protocol->allow_asconf;
    p_config->checksum_func = protocol->checksum_func;
    p_config->nat_capable   = protocol->nat_capable;
    p_config->pmtu_capable  = protocol->pmtu_capable;
    p_config->want_mapped   = protocol->want_mapped;
    p_config->maxseg        = protocol->maxseg;
    /* REl 6.2.0 change start */
    p_config->hb_max_burst  = protocol->hb_max_burst;
    /* REl 6.2.0 change end */
#ifdef SCTP_IP_HEADER_PARAMS
    p_config->ip_header_params.tos = protocol->ip_header_params.tos;
    p_config->ip_header_params.ttl = protocol->ip_header_params.ttl;
    /* REl 6.2.0 change start */
#ifdef SCTP_INET6_SUPPORT
    p_config->ip_header_params.hoplimit = protocol->ip_header_params.hoplimit;
    p_config->ip_header_params.tclass   = protocol->ip_header_params.tclass;
#endif
    /* REl 6.2.0 change end */
#endif

#ifdef SCTP_UI_SOCK_API

    /*required only in socket if*/
    if( SCTP_FALSE !=
            ( p_config->sndrcvinfo_valid = protocol->sndrcvinfo_valid ) )
    {
        p_config->def_sndrcvinfo = protocol->def_sndrcvinfo;
    }

#endif

    if( SCTP_TRUE == def_flag )
    {
        SCTPUNLOCK();
    }

    return;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_validate_config_stack_address
 *
 *  DESCRIPTION:
 *    check if addresses can be configured on machine.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_validate_config_stack_address(
    sctp_bindx_flag_et        operation,
    sctp_U32bit               naddr,
    sctp_sockaddr_st          *p_addrlist,
    sctp_error_t              *p_ecode )
{
    sctp_U32bit        counter, counter1;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_validate_config_stack_address" ) );

    if( SCTP_BINDX_ADD_ADDR == operation )
    {
        /**/
        if( ( naddr + sctp_db_main.num_local_addrs ) > SCTP_MAX_TRANSPORT_ADDR )
        {
            *p_ecode = E_NORESOURCES;
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_db_validate_config_stack_address: Fails"
                          "total address list exceed max" ) );
            return SCTP_FAILURE;
        }

        for( counter = 0; counter < naddr; counter++ )
        {
            for( counter1 = 0; counter1 < sctp_db_main.num_local_addrs; counter1++ )
            {
                if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_db_main.addr_list[counter1],
                                                   &p_addrlist[counter] ) )
                {
                    /*Address already present*/
                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                                ( "sctp_db_validate_config_stack_address:"
                                  "Address already present" ) );

                    *p_ecode = EINV_ADDRS;
                    return SCTP_FAILURE;
                }/*if*/
            }
        } /*for naddr*/
    }

    else
    {
        /* check that addresses are part of stack address list*/
        if( naddr == sctp_db_main.num_local_addrs )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_db_validate_config_stack_address:Trying to delete all" ) );

            *p_ecode = EINV_ADDRS;
            return ( SCTP_FAILURE );
        }

        for( counter = 0; counter < naddr; counter++ )
        {
            for( counter1 = 0; counter1 < sctp_db_main.num_local_addrs; counter1++ )
            {
                if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_db_main.addr_list[counter1],
                                                   &p_addrlist[counter] ) )
                {
                    break;
                }
            }

            if( counter1 == sctp_db_main.num_local_addrs )
            {
                *p_ecode = EINV_ADDRS;
                return ( SCTP_FAILURE );
            }

            /* check if this address is in use */
            for( counter1 = 0; counter1 < sctp_db_main.max_ep; counter1++ )
            {
                sctp_ep_st   *p_ep = &sctp_db_main.ep_table[counter1];

                if( 0 != SCTP_SOCK_IS_IN_USE( p_ep->flags ) )
                {
                    sctp_U32bit addr_index;

                    if( SCTP_SUCCESS == sctp_db_validate_local_addr( p_ep,
                                                                     &p_addrlist[counter], &addr_index ) )
                    {
                        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                                    ( "sctp_db_validate_config_stack_address:"
                                      "Address in use" ) );
                        *p_ecode = EINV_ADDRS;

                        return ( SCTP_FAILURE );
                    }
                }
            }/*for max_ep*/
        }/*for naddr*/
    }/*if.. else */

    return SCTP_SUCCESS;
}



/***************************************************************************
 *  FUNCTION :    sctp_db_add_stack_address
 *
 *  DESCRIPTION:
 *    Add the address and change the num_local_addrs count.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_add_stack_address(
    sctp_U32bit               naddr,
    sctp_sockaddr_st          *p_addrlist )
{
    sctp_U32bit        counter;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_add_stack_address" ) );

    for( counter = 0; counter < naddr; counter++ )
    {
        sctp_db_main.addr_list[counter + sctp_db_main.num_local_addrs] =
            p_addrlist[counter];
    }

    sctp_db_main.num_local_addrs += naddr;
    return SCTP_SUCCESS;
}


/***************************************************************************
 *  FUNCTION :    sctp_db_del_stack_address
 *
 *  DESCRIPTION:
 *     Delete the  address and change the num_local_addrs count
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_del_stack_address(
    sctp_U32bit               naddr,
    sctp_sockaddr_st          *p_addrlist )
{
    sctp_U32bit        new_counter, counter = 0;
    sctp_U32bit        del_count = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_del_stack_address" ) );

    for( counter = 0; counter < naddr; counter++ )
    {
        for( new_counter = 0; new_counter < sctp_db_main.num_local_addrs
                ; new_counter++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_db_main.addr_list[new_counter],
                                               &p_addrlist[counter] ) )
            {
                break;
            }
        }

        /*If addresses have been deleted, shift the addresses */
        for( del_count = new_counter ;
                del_count < sctp_db_main.num_local_addrs - 1; del_count++ )
        {
            sctp_db_main.addr_list[del_count] =
                sctp_db_main.addr_list[del_count + 1];
        }

        ( sctp_db_main.num_local_addrs )--;
    }

    return SCTP_SUCCESS;
}



/***************************************************************************
 *  FUNCTION :    sctp_db_validate_bind_address
 *
 *  DESCRIPTION:
 *    check if this local address is present in association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_validate_bind_address(
    sctp_U32bit               naddr,
    sctp_sockaddr_st          addrlist[] )
{
    sctp_U32bit        counter, counter1;
    sctp_sockaddr_st   *p_addr;

    p_addr = ( sctp_sockaddr_st * )&addrlist[0];

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_validate_bind_address" ) );

    SCTPLOCK();

    for( counter = 0; counter < naddr; counter++ )
    {
        for( counter1 = 0; counter1 < sctp_db_main.num_local_addrs; counter1++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_db_main.addr_list[counter1],
                                               &p_addr[counter] ) )
            {
                break;
            }
        }

        if( counter1 == sctp_db_main.num_local_addrs )
        {
            SCTPUNLOCK();
            return ( SCTP_FAILURE );
        }
    }

    SCTPUNLOCK();

    return ( SCTP_SUCCESS );
}

/* SPR 6274 starts */
/***************************************************************************
 *  FUNCTION :    sctp_db_validate_bind_address_ext
 *
 *  DESCRIPTION:
 *    check if this local address is present in association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_validate_bind_address_ext(
    sctp_U32bit               naddr,
    sctp_sockaddr_st          addrlist[] )
{
    sctp_U32bit        counter, counter1;
    sctp_sockaddr_st   *p_addr;

    p_addr = ( sctp_sockaddr_st * )&addrlist[0];

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_validate_bind_address_ext" ) );

    for( counter = 0; counter < naddr; counter++ )
    {
        for( counter1 = 0; counter1 < sctp_db_main.num_local_addrs; counter1++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_db_main.addr_list[counter1],
                                               &p_addr[counter] ) )
            {
                break;
            }
        }

        if( counter1 == sctp_db_main.num_local_addrs )
        {
            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}
/* SPR 6274 Ends */
/***************************************************************************
 *  FUNCTION :   sctp_db_lct_n_updt_assoc_wo_dprt
 *
 *  DESCRIPTION: This function locate assoc with only src ip, dst ip, and src
 *  port. It update assoc with new dest port.
 *
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_lct_n_updt_assoc_wo_dprt(
    sctp_U16bit        that_port,
    sctp_U32bit        naddr,
    sctp_sockaddr_st   that_address[],
    sctp_U16bit        this_port,
    sctp_sockaddr_st   this_address[],
    sctp_tcb_st        **pp_ass )
{
    sctp_U32bit  sctp_index;
    sctp_U32bit  counter;
    sctp_U32bit  num_assoc = 0;
    sctp_ep_st   *p_ep;
    sctp_tcb_st  *p_assoc;
    sctp_hash_st *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_locate_update_association_without_dstport" ) );

    *pp_ass = SCTP_NULL;
    p_hash = sctp_db_get_bhash_table( this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_ep = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_ep != SCTP_NULL; p_ep = ( sctp_ep_st * )lstNext( ( NODE * )p_ep ) )
        {
            if( p_ep->this_port < this_port )
            {
                continue;
            }

            if( p_ep->this_port > this_port )
            {
                break;
            }

            p_assoc = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

            for( ; p_assoc != SCTP_NULL;
                    p_assoc = ( sctp_tcb_st * )lstNext( ( NODE * )p_assoc ) )
            {
                for( counter = 0; counter < naddr; counter++ )
                {
                    if( ( this_port == p_assoc->p_ep->this_port )
                            && ( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_assoc,
                                                                               &that_address[counter], &sctp_index ) )
                            && ( SCTP_SUCCESS == sctp_db_validate_local_addr( p_assoc->p_ep,
                                                                              this_address, &sctp_index ) ) )
                    {
                        if( p_assoc->state == SCTPSTATE_COOKIEWAIT )
                        {
                            num_assoc++;
                            *pp_ass = p_assoc;
                        }

                        break;
                    }
                }
            }
        }
    }

    p_hash = sctp_db_get_lhash_table( this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_ep = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_ep != SCTP_NULL; p_ep = ( sctp_ep_st * )lstNext( ( NODE * )p_ep ) )
        {
            if( p_ep->this_port < this_port )
            {
                continue;
            }

            if( p_ep->this_port > this_port )
            {
                break;
            }

            p_assoc = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

            for( ; p_assoc != SCTP_NULL;
                    p_assoc = ( sctp_tcb_st * )lstNext( ( NODE * )p_assoc ) )
            {
                for( counter = 0; counter < naddr; counter++ )
                {
                    if( ( this_port == p_assoc->p_ep->this_port )
                            && ( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_assoc,
                                                                               that_address, &sctp_index ) )
                            && ( SCTP_SUCCESS == sctp_db_validate_local_addr(
                                     p_assoc->p_ep, this_address, &sctp_index ) ) )
                    {
                        if( p_assoc->state == SCTPSTATE_COOKIEWAIT )
                        {
                            num_assoc++;
                            *pp_ass = p_assoc;
                        }

                        break;
                    }
                }
            }
        }
    }

    if( num_assoc == 1 )
    {
        ( *pp_ass )->transmit.that_port = that_port;

        return SCTP_SUCCESS;
    }

    return( SCTP_FAILURE );
}
/***************************************************************************
 *  FUNCTION :   sctp_db_add_server_created_port
 *
 *  DESCRIPTION: This function will add the port created by the server
 *
 *
 *  RETURN VALUE:None
 ****************************************************************************/
void sctp_db_add_server_created_port( sctp_U16bit  new_port,
                                      sctp_U32bit              naddr,
                                      sctp_sockaddr_st         addr_list[] )
{
    sctp_U32bit    counter;
    sctp_db_main.server_created_addr_info[sctp_db_main.num_server_created_port].naddr = naddr;
    sctp_db_main.server_created_addr_info[sctp_db_main.num_server_created_port].this_port =
        new_port;

    for( counter = 0; counter < naddr; counter++ )
    {
        sctp_db_main.server_created_addr_info[sctp_db_main.num_server_created_port].
        this_address[counter] = addr_list[counter];
    }

    sctp_db_main.num_server_created_port++;

    if( sctp_db_main.num_server_created_port >= SCTP_MAX_SERVER_CREATED_PORT )
    {
        sctp_db_main.num_server_created_port = 0;
    }
}

/***************************************************************************
 *  FUNCTION :   sctp_db_delete_server_created_port
 *
 *  DESCRIPTION: This function will delete the port created by the server
 *
 *
 *  RETURN VALUE:None
 ****************************************************************************/
void sctp_db_delete_server_created_port( sctp_U16bit  new_port,
                                         sctp_U32bit              naddr,
                                         sctp_sockaddr_st         addr_list[] )
{
    sctp_U32bit    counter, counter1;

    for( counter1 = 0; counter1 < SCTP_MAX_SERVER_CREATED_PORT; counter1++ )
    {
        if( sctp_db_main.server_created_addr_info[counter1].this_port
                == new_port )
        {
            for( counter = 0; counter < naddr; counter++ )
            {
                if( SCTP_SUCCESS != sctp_cmp_addr(
                            &sctp_db_main.server_created_addr_info[counter1].this_address[counter],
                            &addr_list[counter] ) )
                {
                    break;
                }
            }

            sctp_db_main.server_created_addr_info[counter1].this_port = 0;
        }
    }
}

/***************************************************************************
 *  FUNCTION :   sctp_db_find_server_created_port
 *
 *  DESCRIPTION: This function will find the port created by the server
 *
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 *
 ****************************************************************************/
sctp_return_t sctp_db_find_server_created_port( sctp_U16bit  new_port,
                                                sctp_U32bit              naddr,
                                                sctp_sockaddr_st         addr_list[] )
{
    sctp_U32bit    counter, counter1;

    for( counter1 = 0; counter1 < SCTP_MAX_SERVER_CREATED_PORT; counter1++ )
    {
        if( sctp_db_main.server_created_addr_info[counter1].this_port
                == new_port )
        {
            for( counter = 0; counter < naddr; counter++ )
            {
                if( SCTP_SUCCESS == sctp_cmp_addr(
                            &sctp_db_main.server_created_addr_info[counter1].this_address[counter],
                            &addr_list[counter] ) )
                {
                    return SCTP_SUCCESS;
                }
            }
        }
    }

    return SCTP_FAILURE;
}
/***************************************************************************
 *  FUNCTION :   sctp_db_locate_ep_created_from_server
 *
 *  DESCRIPTION:
 *    This function will locate and return the End point Id
 *    of SCTP server listening on this sctp port. If no server is
 *    found on this port error is returned.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_locate_ep_created_from_server(
    sctp_U16bit             sctp_port,
    sctp_sockaddr_st        *p_addr,
    sctp_ep_st              **pp_ep )
{
    sctp_U32bit      counter;
    sctp_ep_st       *p_curr;
    sctp_hash_st     *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_locate_server" ) );

    p_hash = sctp_db_get_bhash_table( sctp_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < sctp_port )
            {
                continue;
            }

            if( p_curr->this_port > sctp_port )
            {
                break;
            }

            for( counter = 0; counter < p_curr->naddr; counter++ )
            {
                if( ( SCTP_SUCCESS == sctp_cmp_addr( p_addr,
                                                     &p_curr->this_address[counter] ) ) &&
                        ( p_curr->create_from_server == SCTP_TRUE ) )
                {
                    *pp_ep = p_curr;
                    return ( SCTP_SUCCESS );
                }
            }
        }
    }

    return ( SCTP_FAILURE );
}
/***************************************************************************
 *  FUNCTION : sctp_db_create_duplicate_ep_with_port
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to add an
 *    EndPoint in the Database Module Data Structure. It will
 *    return a non negative ep Id if successful.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_create_duplicate_ep_with_port(
    sctp_ep_st           *p_ep,
    sctp_ep_st           **pp_new_ep,
    sctp_U16bit          port,
    sctp_error_t         *p_ecode )
{
    sctp_U32bit            count;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_create_duplicate_ep_with_port" ) );

    if( sctp_db_addr_bindable( port, p_ep->naddr, p_ep->this_address )
            == SCTP_FAILURE )
    {
        return ( SCTP_FAILURE );
    }

    if( p_ep->l_backlog < 1 )
    {
        return ( SCTP_FAILURE );
    }

    *pp_new_ep = sctp_db_get_free_ep( p_ecode );

    if( SCTP_NULL == *pp_new_ep )
    {
        return ( SCTP_FAILURE );
    }

    p_ep->l_backlog--;
    /* Copy server parameters */
    sctp_db_get_prot_params( &( *pp_new_ep )->sk_config, p_ep->ep_id, SCTP_FALSE );

    ( *pp_new_ep )->n_assoc     = 0;
    ( *pp_new_ep )->last_used_assoc = 0;
    ( *pp_new_ep )->l_backlog   = 1;
    ( *pp_new_ep )->create_from_server = SCTP_TRUE;
    ( *pp_new_ep )->server_ep_id = p_ep->ep_id;
    ( *pp_new_ep )->num_created_ep = 0;

    p_ep->created_ep_id[p_ep->num_created_ep] = ( *pp_new_ep )->ep_id;
    p_ep->num_created_ep++;

    lstInit( &( *pp_new_ep )->assoc_list );

#ifdef SCTP_UI_SOCK_API

    ( *pp_new_ep )->ulpqueue.p_ep  = *pp_new_ep;
    ( *pp_new_ep )->ulpqueue.n_ind = 0;
    ( *pp_new_ep )->ulpqueue.ntfy_flags = p_ep->ulpqueue.ntfy_flags;

    /*  initilize the ulp queue */
    lstInit( &( ( *pp_new_ep )->ulpqueue.ntfy_queue ) );
    lstInit( &( ( *pp_new_ep )->ulpqueue.data_ind_list ) );
#else

#endif /* SCTP_UI_SOCK_API */

    ( *pp_new_ep )->flags = p_ep->flags;
    /* Reset the Listener Flag of New ep */
    SCTP_SOCK_SET_LISTENER( ( *pp_new_ep )->flags, SCTP_FALSE );

    /* Reset the readable flag for the ep */
    SCTP_SOCK_SET_READABLE( ( *pp_new_ep )->flags, SCTP_FALSE );

    /* Reset the writeable flag for the ep */
    SCTP_SOCK_SET_WRITEABLE( ( *pp_new_ep )->flags, SCTP_TRUE );

    ( *pp_new_ep )->this_port = port;
    ( *pp_new_ep )->naddr = p_ep->naddr;

    for( count = 0; count < p_ep->naddr; count++ )
    {
        ( *pp_new_ep )->this_address[count] = p_ep->this_address[count];
    }

    /* Add the New EP to Bind Hash Table */
    sctp_db_add_hash( SCTP_BIND_HASH, *pp_new_ep );

    for( count = 0; count < SCTP_MAX_HOSTNAME_SIZE; count++ )
    {
        ( *pp_new_ep )->hostname[count] = p_ep->hostname[count];
    }

    ( *pp_new_ep )->appl_id = p_ep->appl_id;
    ( *pp_new_ep )->family = p_ep->family;
    ( *pp_new_ep )->sk_type = p_ep->sk_type;
    ( *pp_new_ep )->trc_flags = p_ep->trc_flags;
    ( *pp_new_ep )->so_linger = p_ep->so_linger;
    ( *pp_new_ep )->send_func = p_ep->send_func; /* Callback for Sending Func SPR 20859 */


    /* Commenting following three statements for CSR#1-4859282 */

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_validate_remote_addr
 *
 *  DESCRIPTION:
 *    check if this remote address is present in association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_validate_remote_addr(
    sctp_tcb_st        *p_ass,
    sctp_sockaddr_st   *rem_addr,
    sctp_U32bit        *addr_index )
{
    sctp_U32bit counter;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_validate_remote_addr " ) );

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( &p_ass->transmit.remote[counter].
                                           that_address, rem_addr ) )
        {
            break;
        }
    }

    if( counter == p_ass->transmit.num_remote_address )
    {
        return( SCTP_FAILURE );
    }

    *addr_index = counter;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_validate_local_addr
 *
 *  DESCRIPTION:
 *    check if this local address is present in association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_validate_local_addr(
    sctp_ep_st        *p_ep,
    sctp_sockaddr_st  *local_addr,
    sctp_U32bit       *addr_index )
{
    sctp_U32bit counter;

    SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_validate_local_addr " ) );

    for( counter = 0; counter < p_ep->naddr; counter++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( &p_ep->this_address[counter],
                                           local_addr ) )
        {
            break;
        }
    }

    if( counter == p_ep->naddr )
    {
        return( SCTP_FAILURE );
    }

    *addr_index = counter;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_get_assoc_from_ep
 *
 *  DESCRIPTION:
 *    This Database Access function will return the pointer to the first
 *    association of the endpoint
 *
 *  RETURN VALUE:
 *    Pointer to the first association of the given endpoint or SCTP_NULL
 *
 ****************************************************************************/
sctp_tcb_st *
sctp_db_get_assoc_from_ep(
    sctp_ep_st         *p_ep )
{
    /* SPR : 18527 */
    if( p_ep == SCTP_NULL || 0 == p_ep->assoc_list.count )
        /* SPR : 18527 */
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_db_get_assoc_from_ep failed" ) );
        return ( ( sctp_tcb_st * )SCTP_NULL );
    }

    return ( ( sctp_tcb_st * )lstFirst( &( p_ep->assoc_list ) ) );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_is_tcb_in_ep
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to
 *    locate an association in the Database Module Data Structure.
 *    It will return a valid association Id.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_is_tcb_in_ep(
    sctp_ep_st             *p_ep,
    sctp_U16bit            dport,
    sctp_U32bit            naddr,
    sctp_sockaddr_st       daddr_list[],
    sctp_tcb_st            **pp_tcb,
    sctp_error_t           *p_err )
{
    sctp_U32bit     counter    = 0;
    sctp_U32bit     counter1   = 0;
    sctp_tcb_st     *p_tmp_tcb = SCTP_NULL;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_err );
    /*to avoid the warning*/


    if( 0 == p_ep->assoc_list.count )
    {
        *pp_tcb = SCTP_NULL;

        return ( SCTP_FAILURE );
    }

    p_tmp_tcb = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

    for( ; p_tmp_tcb != SCTP_NULL;
            p_tmp_tcb = ( sctp_tcb_st * )lstNext( ( NODE * )p_tmp_tcb ) )
    {
        if( p_tmp_tcb->transmit.that_port != dport )
        {
            continue;
        }

        for( counter = 0; counter < naddr; counter++ )
        {
            for( counter1 = 0;
                    counter1 < p_tmp_tcb->transmit.num_remote_address;
                    counter1++ )
            {
                if( sctp_cmp_addr( &daddr_list[counter],
                                   &p_tmp_tcb->transmit.remote[counter1].that_address )
                        == SCTP_SUCCESS )
                {
                    *pp_tcb = p_tmp_tcb;

                    return ( SCTP_SUCCESS );
                }
            }
        }
    }

    *pp_tcb = SCTP_NULL;

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_is_listenable
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to
 *    locate an association in the Database Module Data Structure.
 *    It will return a valid association Id.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_is_listenable(
    sctp_ep_st         *p_ep,
    sctp_error_t       *p_err )
{
    sctp_ep_st    *p_curr;
    sctp_hash_st  *p_hash = SCTP_NULL;
    sctp_U32bit   counter, counter1;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_err );
    /*to avoid the warning*/

    p_hash = sctp_db_get_lhash_table( p_ep->this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < p_ep->this_port )
            {
                continue;
            }

            if( p_curr->this_port > p_ep->this_port )
            {
                break;
            }

            /* SPR : 18527 */
            for( counter = 0; counter < SCTP_MAX_TRANSPORT_ADDR && counter < p_ep->naddr; counter++ )
            {
                for( counter1 = 0; counter1 < SCTP_MAX_TRANSPORT_ADDR && counter1 < p_curr->naddr; counter1++ )
                    /* SPR : 18527 */
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr(
                                &p_ep->this_address[counter],
                                &p_curr->this_address[counter1] ) )
                    {
                        return( SCTP_FAILURE );
                    }
                }
            }
        }
    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_addr_bindable
 *
 *  DESCRIPTION:This function will check the address if it is bindable
 *
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_addr_bindable(
    sctp_U16bit              port,
    sctp_U32bit              naddr,
    sctp_sockaddr_st         addr_list[] )
{
    sctp_U32bit    counter, counter1;
    sctp_ep_st     *p_curr;
    sctp_hash_st   *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_addr_bindable " ) );

    p_hash = sctp_db_get_lhash_table( port );

    if( SCTP_NULL != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        /*SPR 5212*/
        for( ; p_curr != SCTP_NULL && p_curr->this_port <= port;
                /*SPR 5212*/
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            /*SPR 5212*/
            if( p_curr->this_port < port )
                /*SPR 5212*/
            {
                continue;
            }

            for( counter = 0; counter < naddr; counter++ )
            {
                for( counter1 = 0; counter1 < p_curr->naddr; counter1++ )
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr( &addr_list[counter],
                                                       &p_curr->this_address[counter1] ) )
                    {
                        /* don't Fail if REUSEADDR is set */
                        if( 0 == SCTP_SOCK_IS_REUSEADDR( p_curr->flags ) )
                        {
                            return ( SCTP_FAILURE );
                        }
                    }
                }
            }
        }
    }

    p_hash = sctp_db_get_bhash_table( port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        /*SPR 5212*/
        for( ; p_curr != SCTP_NULL && p_curr->this_port <= port;
                /*SPR 5212*/
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            /*SPR 5212*/
            if( p_curr->this_port < port )
            {
                continue;
            }

            /*SPR 5212*/

            for( counter = 0; counter < naddr; counter++ )
            {
                for( counter1 = 0; counter1 < p_curr->naddr; counter1++ )
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr( &addr_list[counter],
                                                       &p_curr->this_address[counter1] ) )
                    {
                        if( 0 == SCTP_SOCK_IS_REUSEADDR( p_curr->flags ) )
                        {
                            return ( SCTP_FAILURE );
                        }
                    }
                }
            }
        }
    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_chk_addr_for_delete
 *
 *  DESCRIPTION:
 *    This function will check the address for deletion from list.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_chk_addr_for_delete(
    sctp_ep_st               *p_ep,
    sctp_U16bit              port,
    sctp_U32bit              naddr,
    sctp_sockaddr_st         addr_list[],
    sctp_error_t             *p_err )
{
    sctp_U32bit        counter, counter1, addr_matched;
    sctp_sockaddr_st   *p_addr;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_err );
    SCTP_UNUSED_VARIABLE( port );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_chk_addr_for_delete" ) );

    p_addr = ( sctp_sockaddr_st * )&addr_list[0];

    /*
     * Check whether the list has duplicate addresses in it
     * if yes return error
     */
    for( counter = 0; counter < naddr; counter++ )
    {
        for( counter1 = counter + 1; counter1 < naddr; counter1++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &p_addr[counter1],
                                               &p_addr[counter] ) )
            {
                /* Error List has duplicate addresses */
                return( SCTP_FAILURE );
            }
        }
    }

    /*
     * check whether this deletion is going to delete all the address
     * of the endpoint, not possible
     * return error
     */
    if( naddr >= p_ep->naddr )
    {
        /* Error */
        return( SCTP_FAILURE );
    }

    /*
     * check whether all the addresses are present in the endpoint
     * if not then return error
     */
    addr_matched = 0;

    for( counter = 0; counter < naddr; counter++ )
    {
        for( counter1 = 0; counter1 < p_ep->naddr; counter1++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &p_ep->this_address[counter1],
                                               &p_addr[counter] ) )
            {
                addr_matched++;
                break;
            }
        }
    }

    if( addr_matched != naddr )
    {
        /* Error */
        return( SCTP_FAILURE );
    }

    return SCTP_SUCCESS;
}



/***************************************************************************
 *  FUNCTION :    sctp_db_rem_addr_after_chks
 *
 *  DESCRIPTION:
 *   This function will return the remaining addresses after checks
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_rem_addr_after_chks(
    sctp_ep_st               *p_ep,
    sctp_U16bit              port,
    sctp_U32bit              naddr,
    sctp_sockaddr_st         addr_list[],
    sctp_error_t             *p_err )
{
    sctp_U32bit        counter, counter1, addr_matched, counter2;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_rem_addr_after_chks" ) );

    if( SCTP_FAILURE == sctp_db_chk_addr_for_delete( p_ep, port, naddr,
                                                     addr_list, p_err ) )
    {
        return SCTP_FAILURE;
    }

    /*
     * Now we can delete the addresses
     */
    /* CSR 69326 fix starts */
    if( p_ep->naddr == 0 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "number of addresses are null\n" ) );
        return SCTP_FAILURE;
    }

    /* CSR 69326 fix ends */
    addr_matched = 0;

    for( counter = 0; ( counter < p_ep->naddr )
            && ( p_ep->naddr <= SCTP_MAX_TRANSPORT_ADDR ); counter++ ) /* klockwork fix 6.2.4 */
    {
        for( counter1 = 0; counter1 < naddr; counter1++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &p_ep->this_address[counter],
                                               &addr_list[counter1] ) )
            {
                addr_matched++;
                break;
            }
        }

        if( counter1 < naddr )
        {
            counter2 = counter;

            for( counter1 = counter + 1; ( counter1 < p_ep->naddr )
                    && ( p_ep->naddr <= SCTP_MAX_TRANSPORT_ADDR ); counter1++ ) /* klockwork fix 6.2.4 */

            {
                if( !( SCTP_IS_INADDR_ANY( p_ep->this_address[counter1] ) ) )
                {
                    p_ep->this_address[counter2] = p_ep->this_address[counter1];
                    __MEMSET( &p_ep->this_address[counter1], 0,
                              sizeof( sctp_sockaddr_st ) );
                    counter2++;
                }
            }
        }
    }

    /* Decrement the no of addrs */
    p_ep->naddr -= addr_matched;
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_addr_connectable
 *
 *  DESCRIPTION:
 *    This function checks if address is connectable
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_addr_connectable(
    sctp_ep_st             *p_ep,
    sctp_U16bit            port,
    sctp_U32bit            naddr,
    sctp_sockaddr_st       addr_list[],
    sctp_error_t           *p_err )
{
    sctp_U32bit   counter, counter1;
    sctp_ep_st    *p_curr;
    sctp_tcb_st   *p_tcb = SCTP_NULL;
    sctp_hash_st  *p_hash = SCTP_NULL;
    sctp_suppaddr_st   supp_addr = {SCTP_TRUE, SCTP_TRUE, SCTP_TRUE};

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_addr_connectable" ) );

    /*SPR 5093*/
    for( counter = 0; counter < naddr; counter++ )
    {
        if( SCTP_FAILURE == sctp_chk_addr_unicast( &addr_list[counter] ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_db_addr_connectable : addr not unicast" ) );
            return SCTP_FAILURE;
        }
    }

    /*SPR 5093*/

    /*SPR 5216*/
    /* Check if the destination address is of the correct family */
    /*Assume peer is v6 capable even if user has passed a v4 destination*/
    if( SCTP_FALSE == sctp_db_is_v4_v6_addr_compatible( p_ep, naddr,
                                                        addr_list, &supp_addr ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address family is not right" ) );
        return ( SCTP_FAILURE );
    }

    /*SPR 5216*/

    p_hash = sctp_db_get_bhash_table( p_ep->this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < p_ep->this_port )
            {
                continue;
            }

            if( p_curr->this_port > p_ep->this_port )
            {
                break;
            }

            for( counter = 0; counter < p_ep->naddr; counter++ )
            {
                for( counter1 = 0; counter1 < p_curr->naddr; counter1++ )
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr(
                                &p_ep->this_address[counter],
                                &p_curr->this_address[counter1] ) )
                    {
                        if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_curr, port,
                                                                  naddr, addr_list, &p_tcb, p_err ) )
                        {
                            return ( SCTP_FAILURE );
                        }
                    }
                }
            }
        }
    }

    p_hash = sctp_db_get_lhash_table( p_ep->this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < p_ep->this_port )
            {
                continue;
            }

            if( p_curr->this_port > p_ep->this_port )
            {
                break;
            }

            for( counter = 0; counter < p_ep->naddr; counter++ )
            {
                for( counter1 = 0; counter1 < p_curr->naddr; counter1++ )
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr(
                                &p_ep->this_address[counter],
                                &p_curr->this_address[counter1] ) )
                    {
                        if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_curr, port,
                                                                  naddr, addr_list, &p_tcb, p_err ) )
                        {
                            return ( SCTP_FAILURE );
                        }
                    }
                }
            }
        }
    }

    return( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_locate_association
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to
 *    locate an association in the Database Module Data Structure.
 *    It will return a valid association Id.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_locate_association(
    sctp_U16bit        that_port,
    sctp_sockaddr_st   *that_address,
    sctp_U16bit        this_port,
    sctp_sockaddr_st   *this_address,
    sctp_tcb_st        **pp_ass )
{
    sctp_U32bit  sctp_index;
    sctp_ep_st   *p_ep;
    sctp_tcb_st  *p_assoc;
    sctp_hash_st *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_locate_association" ) );

    *pp_ass = SCTP_NULL;
    p_hash = sctp_db_get_bhash_table( this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_ep = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_ep != SCTP_NULL; p_ep = ( sctp_ep_st * )lstNext( ( NODE * )p_ep ) )
        {
            if( p_ep->this_port < this_port )
            {
                continue;
            }

            if( p_ep->this_port > this_port )
            {
                break;
            }

            p_assoc = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

            for( ; p_assoc != SCTP_NULL;
                    p_assoc = ( sctp_tcb_st * )lstNext( ( NODE * )p_assoc ) )
            {
                if( ( this_port == p_assoc->p_ep->this_port )
                        && ( that_port == p_assoc->transmit.that_port )
                        && ( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_assoc,
                                                                           that_address, &sctp_index ) )
                        && ( SCTP_SUCCESS == sctp_db_validate_local_addr( p_assoc->p_ep,
                                                                          this_address, &sctp_index ) ) )
                {
                    *pp_ass = p_assoc;
                    return ( SCTP_SUCCESS );
                }
            }
        }
    }

    p_hash = sctp_db_get_lhash_table( this_port );

    if( SCTP_NULL  != p_hash )
    {
        p_ep = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_ep != SCTP_NULL; p_ep = ( sctp_ep_st * )lstNext( ( NODE * )p_ep ) )
        {
            if( p_ep->this_port < this_port )
            {
                continue;
            }

            if( p_ep->this_port > this_port )
            {
                break;
            }

            p_assoc = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

            for( ; p_assoc != SCTP_NULL;
                    p_assoc = ( sctp_tcb_st * )lstNext( ( NODE * )p_assoc ) )
            {
                if( ( this_port == p_assoc->p_ep->this_port )
                        && ( that_port == p_assoc->transmit.that_port )
                        && ( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_assoc,
                                                                           that_address, &sctp_index ) )
                        && ( SCTP_SUCCESS == sctp_db_validate_local_addr(
                                 p_assoc->p_ep, this_address, &sctp_index ) ) )
                {
                    *pp_ass = p_assoc;
                    return( SCTP_SUCCESS );
                }
            }
        }
    }

    return( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_locate_server
 *
 *  DESCRIPTION:
 *    This function will locate and return the End point Id
 *    of SCTP server listening on this sctp port. If no server is
 *    found on this port error is returned.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_locate_server(
    sctp_U16bit             sctp_port,
    sctp_sockaddr_st        *p_addr,
    sctp_ep_st              **pp_ep )
{
    sctp_U32bit      counter;
    sctp_ep_st       *p_curr;
    sctp_hash_st     *p_hash = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_locate_server" ) );

    p_hash = sctp_db_get_lhash_table( sctp_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < sctp_port )
            {
                continue;
            }

            if( p_curr->this_port > sctp_port )
            {
                break;
            }

            for( counter = 0; counter < p_curr->naddr; counter++ )
            {
                if( SCTP_SUCCESS == sctp_cmp_addr( p_addr,
                                                   &p_curr->this_address[counter] ) )
                {
                    *pp_ep = p_curr;
                    return ( SCTP_SUCCESS );
                }
            }
        }
    }

    return ( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_is_port_active
 *
 *  DESCRIPTION:
 *    This function will see if the given SCTP port is active
 *    if true it returns success else failure.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_is_port_active(
    sctp_U16bit sctp_port )
{
    sctp_hash_st *p_hash = SCTP_NULL;
    sctp_ep_st   *p_curr;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_is_port_active, port= %d",
                  sctp_port ) );

    p_hash = sctp_db_get_lhash_table( sctp_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < sctp_port )
            {
                continue;
            }

            if( p_curr->this_port > sctp_port )
            {
                break;
            }

            return SCTP_SUCCESS;
        }
    }

    p_hash = sctp_db_get_bhash_table( sctp_port );

    if( SCTP_NULL  != p_hash )
    {
        p_curr = ( sctp_ep_st * )lstFirst( &p_hash->chain );

        for( ; p_curr != SCTP_NULL;
                p_curr = ( sctp_ep_st * )lstNext( ( NODE * )p_curr ) )
        {
            if( p_curr->this_port < sctp_port )
            {
                continue;
            }

            if( p_curr->this_port > sctp_port )
            {
                break;
            }

            return SCTP_SUCCESS;
        }
    }


    return( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_free_sctpport
 *
 *  DESCRIPTION:
 *    This function will locate and return a free SCTP port.
 *    This port is chosen between a minimum and maximum value.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_get_free_sctpport(
    sctp_U16bit *sctp_port )

{
    sctp_U16bit port = SCTPDYNPORT_MIN;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_free_sctpport" ) );

    while( ( SCTPDYNPORT_MAX > port ) &&
            ( SCTP_SUCCESS == sctp_db_is_port_active( port ) ) )
    {
        port++;
    }

    if( SCTPDYNPORT_MAX == port )
    {
        return( SCTP_FAILURE );
    }

    else
    {
        *sctp_port = port;
        return( SCTP_SUCCESS );
    }
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_ep_ptr
 *
 *  DESCRIPTION:
 *    This function returns pointer to end point structure.
 *    End point structure is indexed by ep id passed as
 *    input parameter.
 *
 *  RETURN VALUE:
 *     in case of success pointer to the endpoint
 *     in case of failure NULL
 ****************************************************************************/
sctp_ep_st *
sctp_db_get_ep_ptr(
    sctp_U32bit     ep_id )

{
    sctp_ep_st *p_ep;

    if( ep_id < sctp_db_main.max_ep )
    {
        p_ep = &sctp_db_main.ep_table[ep_id];

        if( 1 == SCTP_SOCK_IS_IN_USE( p_ep->flags ) )
        {
            return( p_ep );
        }
    }

    return( SCTP_NULL );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_association_ptr
 *
 *  DESCRIPTION:
 *    This function returns pointer to end point structure.
 *    End point structure is indexed by ep id passed as
 *    input parameter.
 *
 *  RETURN VALUE:
 *     in case of success   pointer to the endpoint
 *     in case of failure   NULL
 ****************************************************************************/
sctp_tcb_st *
sctp_db_get_association_ptr(
    sctp_U32bit     ep_id,
    sctp_U32bit     assoc_id )
{
    sctp_ep_st   *p_ep;
    sctp_tcb_st  *p_ass;

    p_ep = sctp_db_get_ep_ptr( ep_id );

    if( p_ep != SCTP_NULL )
    {

        for( p_ass = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );
                ( p_ass != SCTP_NULL ) && ( p_ass->assoc_id <= assoc_id );
                p_ass = ( sctp_tcb_st * )lstNext( ( NODE * )p_ass ) )
        {
            if( p_ass->assoc_id == assoc_id )
            {
                break;
            }
        }

        if( p_ass == SCTP_NULL )
        {
            return( SCTP_NULL );
        }

        if( p_ass->assoc_id == assoc_id )
        {
            return( p_ass );
        }
    }

    return ( SCTP_NULL );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_free_assoc_freelists
 *
 *  DESCRIPTION:
 *    This procedure frees buffers from  tx and rx free lists
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_free_assoc_freelists(
    sctp_tcb_st    *p_ass )
{
    sctpbuffer_st *p_buf;
    /*WARNING*/
    sctp_U32bit   count = 0;
    /*WARNING*/

    /* SCTPLOCK();*/ /* SPR 3672 */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_free_assoc_freelists" ) );

    /*Free rx buffers*/
    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_min );

    for( count = 0; p_buf;
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_min ), count++ )
    {
        lstDelete( &p_ass->tx_freelist.list_min, ( NODE * )p_buf );

        if( count < sctp_db_main.min_tx_buffers )
        {
            lstAdd( &sctp_db_main.tx_list.list_min, ( NODE * )p_buf );
        }

        else
        {
            lstAdd( &sctp_db_main.reserve_list.list_min, ( NODE * )p_buf );
        }
    }

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_med );

    for( count = 0; p_buf;
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_med ), count++ )
    {
        lstDelete( &p_ass->tx_freelist.list_med, ( NODE * )p_buf );

        if( count < sctp_db_main.med_tx_buffers )
        {
            lstAdd( &sctp_db_main.tx_list.list_med, ( NODE * )p_buf );
        }

        else
        {
            lstAdd( &sctp_db_main.reserve_list.list_med, ( NODE * )p_buf );
        }
    }

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_max );

    for( count = 0; p_buf;
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_max ), count++ )
    {
        lstDelete( &p_ass->tx_freelist.list_max, ( NODE * )p_buf );

        if( count < sctp_db_main.max_tx_buffers )
        {
            lstAdd( &sctp_db_main.tx_list.list_max, ( NODE * )p_buf );
        }

        else
        {
            lstAdd( &sctp_db_main.reserve_list.list_max, ( NODE * )p_buf );
        }
    }

    /*Free rx buffers*/

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_min );

    for( count = 0; p_buf;
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_min ), count++ )
    {
        lstDelete( &p_ass->rx_freelist.list_min, ( NODE * )p_buf );

        if( count < sctp_db_main.min_rx_buffers )
        {
            lstAdd( &sctp_db_main.rx_list.list_min, ( NODE * )p_buf );
        }

        else
        {
            lstAdd( &sctp_db_main.reserve_list.list_min, ( NODE * )p_buf );
        }
    }

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_med );

    for( count = 0; p_buf;
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_med ), count++ )
    {
        lstDelete( &p_ass->rx_freelist.list_med, ( NODE * )p_buf );

        if( count < sctp_db_main.med_rx_buffers )
        {
            lstAdd( &sctp_db_main.rx_list.list_med, ( NODE * )p_buf );
        }

        else
        {
            lstAdd( &sctp_db_main.reserve_list.list_med, ( NODE * )p_buf );
        }
    }

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_max );

    for( count = 0; p_buf;
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_max ), count++ )
    {
        lstDelete( &p_ass->rx_freelist.list_max, ( NODE * )p_buf );

        if( count < sctp_db_main.max_rx_buffers )
        {
            lstAdd( &sctp_db_main.rx_list.list_max, ( NODE * )p_buf );
        }

        else
        {
            lstAdd( &sctp_db_main.reserve_list.list_max, ( NODE * )p_buf );
        }
    }

    /* SCTPUNLOCK(); */ /* SPR 3672 */

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_get_assoc_freelists
 *
 *  DESCRIPTION:
 *    This procedure get buffer for tx and rx free lists
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_get_assoc_freelists(
    sctp_tcb_st    *p_ass,
    sctp_error_t   *p_ecode )
{
    sctp_U32bit counter;

    sctpbuffer_st *p_buf;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_assoc_freelists" ) );

    /* extract tx buffer from global pool and add it to association freelist */
    lstInit( &p_ass->tx_freelist.list_min );
    lstInit( &p_ass->tx_freelist.list_med );
    lstInit( &p_ass->tx_freelist.list_max );

    for( counter = 0; counter < sctp_db_main.min_tx_buffers; counter++ )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &sctp_db_main.tx_list.list_min );

        if( SCTP_NULL != p_buf )
        {
            lstDelete( &sctp_db_main.tx_list.list_min , ( NODE * ) p_buf );
            lstAdd( &p_ass->tx_freelist.list_min, ( NODE * ) p_buf );
        }

        else
        {
            /*Error init tx free list, cleanup and return error */
            *p_ecode = E_INIT_QUEUES;  /* error init queues */
            return( SCTP_FAILURE );
        }
    }

    for( counter = 0; counter < sctp_db_main.med_tx_buffers; counter++ )
    {

        p_buf = ( sctpbuffer_st * )lstFirst( &sctp_db_main.tx_list.list_med );

        if( SCTP_NULL != p_buf )
        {
            lstDelete( &sctp_db_main.tx_list.list_med , ( NODE * ) p_buf );
            lstAdd( &p_ass->tx_freelist.list_med, ( NODE * )p_buf );
        }

        else
        {
            /* Error init tx free list, cleanup and return error */
            *p_ecode = E_INIT_QUEUES;  /* error init queues */
            return( SCTP_FAILURE );
        }
    }

    for( counter = 0; counter < sctp_db_main.max_tx_buffers; counter++ )
    {

        p_buf = ( sctpbuffer_st * )lstFirst( &sctp_db_main.tx_list.list_max );

        if( SCTP_NULL != p_buf )
        {
            lstDelete( &sctp_db_main.tx_list.list_max, ( NODE * )p_buf );
            lstAdd( &p_ass->tx_freelist.list_max, ( NODE * ) p_buf );
        }

        else
        {
            /*Error init tx free list, cleanup and return error */
            *p_ecode = E_INIT_QUEUES;  /* error init queues */

            return ( SCTP_FAILURE );
        }
    }

    /* similarly extract rx buffer from global pool and add
     * it to association freelist
     */
    lstInit( &p_ass->rx_freelist.list_min );
    lstInit( &p_ass->rx_freelist.list_med );
    lstInit( &p_ass->rx_freelist.list_max );

    for( counter = 0; counter < sctp_db_main.min_rx_buffers; counter++ )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &sctp_db_main.rx_list.list_min );

        if( SCTP_NULL != p_buf )
        {
            lstDelete( &sctp_db_main.rx_list.list_min, ( NODE * ) p_buf );
            lstAdd( &p_ass->rx_freelist.list_min, ( NODE * ) p_buf );
        }

        else
        {
            /*Error init rx free list, cleanup and return error */
            *p_ecode = E_INIT_QUEUES;  /* error init queues */

            return ( SCTP_FAILURE );
        }
    }

    for( counter = 0; counter < sctp_db_main.med_rx_buffers; counter++ )
    {

        p_buf = ( sctpbuffer_st * )lstFirst( &sctp_db_main.rx_list.list_med );

        if( SCTP_NULL != p_buf )
        {
            lstDelete( &sctp_db_main.rx_list.list_med, ( NODE * ) p_buf );
            lstAdd( &p_ass->rx_freelist.list_med, ( NODE * )p_buf );
        }

        else
        {
            /*Error init rx free list, cleanup and return error */
            *p_ecode = E_INIT_QUEUES;  /* error init queues */

            return ( SCTP_FAILURE );
        }
    }

    for( counter = 0; counter < sctp_db_main.max_rx_buffers; counter++ )
    {

        p_buf = ( sctpbuffer_st * )lstFirst( &sctp_db_main.rx_list.list_max );

        if( SCTP_NULL != p_buf )
        {
            lstDelete( &sctp_db_main.rx_list.list_max, ( NODE * )p_buf );
            lstAdd( &p_ass->rx_freelist.list_max, ( NODE * ) p_buf );

        }

        else
        {
            /*Error init rx free list, cleanup and return error */
            *p_ecode = E_INIT_QUEUES;  /* error init queues */

            return ( SCTP_FAILURE );
        }
    }

    p_ass->transmit.wnd_size = sctp_db_main.min_tx_buffers *
                               SCTP_MIN_FRAGSIZE + sctp_db_main.med_tx_buffers * SCTP_MED_FRAGSIZE
                               + sctp_db_main.max_tx_buffers * SCTP_MAX_FRAGSIZE;
    p_ass->receive.wnd_size = sctp_db_main.min_rx_buffers *
                              SCTP_MIN_FRAGSIZE + sctp_db_main.med_rx_buffers * SCTP_MED_FRAGSIZE
                              + sctp_db_main.max_rx_buffers * SCTP_MAX_FRAGSIZE;

    /* Changes For CSR#1-5037386 */
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
    p_ass->transmit.last_txq_tsn = SCTP_INVALID_U32;
    p_ass->transmit.non_resp_thresh = 0;
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS*/
    /* Changes For CSR#1-5037386 Ends */
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_init_assoc_queues
 *
 *  DESCRIPTION:
 *    This function is called only during stack initialisation time to
 *    initialise association linked lists.
 *
 *  RETURN VALUE:
 *    void
 ****************************************************************************/
void
sctp_db_init_assoc_queues(
    sctp_tcb_st  *p_ass )
{
    sctp_U32bit counter;

    lstInit( &p_ass->tx_freelist.list_min );
    lstInit( &p_ass->tx_freelist.list_med );
    lstInit( &p_ass->tx_freelist.list_max );

    lstInit( &p_ass->transmit.transmit_queue );

    lstInit( &p_ass->rx_freelist.list_min );
    lstInit( &p_ass->rx_freelist.list_med );
    lstInit( &p_ass->rx_freelist.list_max );

    for( counter = 0; counter < sctp_db_main.max_streams_per_assoc; counter++ )
    {
        lstInit( &p_ass->receive.stream[counter].receive_queue );
    }

    lstInit( &p_ass->receive.sacklist );
    lstInit( &p_ass->receive.sackfreelist );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_cleanup_queues
 *
 *  DESCRIPTION:
 *    This procedure initialises the following linked lists.
 *   - trannsmit_queue
 *   - receive_queue
 *   - sacklist
 *   - sackfreelist
 *
 *  RETURN VALUE:
 *   None
 ****************************************************************************/
void
sctp_db_cleanup_queues(
    sctp_tcb_st        *p_ass,
    sctp_Boolean_t     clean_tx_queue )
{

    sctp_U32bit  counter;
    sctp_U32bit  id;

    sctpbuffer_st *p_buf;
    /*We can take any sctpbuffer type (max or med also here */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_cleanup_queues" ) );

    /* clear Transmit queue*/
    if( SCTP_TRUE == clean_tx_queue )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

        for( ; p_buf;
                p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue ) )
        {
            lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )p_buf );
            id = p_buf->id;

            if( SCTP_MIN_FRAGSIZE == id )
            {
                lstAdd( &( p_ass->tx_freelist.list_min ), ( NODE * )p_buf );
            }

            else if( SCTP_MED_FRAGSIZE == id )
            {
                lstAdd( &( p_ass->tx_freelist.list_med ), ( NODE * )p_buf );
            }

            else
            {
                lstAdd( &( p_ass->tx_freelist.list_max ), ( NODE * )p_buf );
            }
        }
    }

    /* Clear all Receive queues*/
    for( counter = 0; counter < p_ass->receive.num_in_streams; counter++ )
    {
        p_buf = ( sctpbuffer_st * )lstFirst(
                    &p_ass->receive.stream[counter].receive_queue );

        for( ; p_buf;
                p_buf = ( sctpbuffer_st * )lstFirst(
                            &p_ass->receive.stream[counter].receive_queue ) )
        {
            lstDelete( &p_ass->receive.stream[counter].receive_queue,
                       ( NODE * )p_buf );
            id = p_buf->id;

            if( SCTP_MIN_FRAGSIZE == id )
            {
                lstAdd( &p_ass->rx_freelist.list_min, ( NODE * )p_buf );
            }

            else if( SCTP_MED_FRAGSIZE == id )

            {
                lstAdd( &p_ass->rx_freelist.list_med, ( NODE * )p_buf );
            }

            else
            {
                lstAdd( &p_ass->rx_freelist.list_max, ( NODE * )p_buf );
            }
        }
    }

    /*
     * Initialise the sacklist and sackfreelist, As memory for nodes for this
     *  queue is allocated as an local array,no need to worry for it
     */
    lstInit( &p_ass->receive.sacklist );
    lstInit( &p_ass->receive.sackfreelist );

}


/***************************************************************************
 *  FUNCTION :    sctp_db_clean_association
 *
 *  DESCRIPTION:
 *    Initialise association with default values. No Lists are initialised
 *    here.
 *
 *  RETURN VALUE:
 *   None
 ****************************************************************************/
void
sctp_db_clean_association(
    sctp_tcb_st    *p_ass,
    sctp_Boolean_t restart_flag )
{
    sctp_U32bit counter, counter1;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_clean_association" ) );

    if( SCTP_FALSE == restart_flag )
    {
        p_ass->open = 0;
        p_ass->state  = SCTPSTATE_CLOSED;

        p_ass->restart_option    = SCTP_RESTARTOPTION_REQUE;

        p_ass->transmit.that_port  = 0;
    }

    p_ass->init_retry      = 0;
    p_ass->cookie_retry      = 0;
    p_ass->shutdown_retry    = 0;
    p_ass->shutdown_ack_retry = 0;
    p_ass->error_count       = 0;

    p_ass->local_tag       = 0;
    p_ass->remote_tag      = 0;
    p_ass->total_sent      = 0;

    p_ass->zero_win_prob_on = SCTP_FALSE;
    p_ass->conn_accepted    = SCTP_FALSE;

    /* Stop all timers */
    for( counter = 0; counter < SCTP_ASSOCTIMER_NUM; counter++ )
    {
        sctp_stop_timer( &p_ass->assoc_timer_id[counter], SCTP_NULL, SCTP_NULL );
    }

    /* transmit variables */
    p_ass->transmit.congestion_event_sent   = 0;
    p_ass->transmit.tsn       = 0;
    p_ass->transmit.rwndthere   = 0;
    p_ass->transmit.total_outstanding     = 0;
    p_ass->transmit.num_out_streams = p_ass->config.num_out_streams;
    p_ass->transmit.lowest_path_mtu = SCTP_DEFAULT_MTUSIZE;
    /* Changes For CSR#1-5037386 */
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
    p_ass->transmit.last_txq_tsn = SCTP_INVALID_U32;
    p_ass->transmit.non_resp_thresh = 0;
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS*/
    /* Changes For CSR#1-5037386 Ends */


    /* Init the ssn values for all streams */
    for( counter = 0; counter < sctp_db_main.max_streams_per_assoc; counter++ )
    {
        p_ass->transmit.ssn[counter] = 0;
    }

    /* p_ass->transmit.src_addr_index   = 0; */

    p_ass->transmit.num_remote_address       = 0;
    p_ass->transmit.primary_address_index    = 0;
    p_ass->transmit.configured_primary_path_index = 0;

    for( counter = 0; counter < SCTP_MAX_TRANSPORT_ADDR; counter++ )
    {
        __MEMSET( &( p_ass->transmit.remote[counter].that_address ),
                  0, sizeof( sctp_sockaddr_st ) );

        p_ass->transmit.remote[counter].state = SCTP_TX_STATE_NULL;
        p_ass->transmit.remote[counter].status = SCTP_COMMUNICATION_LOST;
        p_ass->transmit.remote[counter].src_addr_index   = 0;

        for( counter1 = 0; counter1 < SCTP_DESTTIMER_NUM; counter1++ )
        {
            sctp_stop_timer( &p_ass->transmit.remote[counter].dest_timer_id[counter1],
                             SCTP_NULL, SCTP_NULL );
        }

        /* Changes For CSR#1-5037386, 1-5628824 */
        p_ass->transmit.remote[counter].prefered_sack_dest = SCTP_FALSE;
        p_ass->transmit.remote[counter].unsent_packet_count = 0;
        p_ass->transmit.remote[counter].retrans_packet_count = 0;
        /* Changes For CSR#1-5037386, 1-5628824 Ends */
        p_ass->transmit.remote[counter].allow_heartbeat = 1;
        p_ass->transmit.remote[counter].path_mtu = 0;
        p_ass->transmit.remote[counter].path_error_count = 0;
        p_ass->transmit.remote[counter].lowest_tsn = 0;
        p_ass->transmit.remote[counter].num_chunks_await_ack = 0;
        p_ass->transmit.remote[counter].cwnd = 0;
        p_ass->transmit.remote[counter].path_error_threshold
            = p_ass->config.path_max_retrans;
        p_ass->transmit.remote[counter].ssthresh = 0;
        p_ass->transmit.remote[counter].outstanding = 0;
        p_ass->transmit.remote[counter].rto = p_ass->config.rto_init;
        p_ass->transmit.remote[counter].srtt = 0;
        p_ass->transmit.remote[counter].rttvar = 0;
        p_ass->transmit.remote[counter].count = 0;
        /* Start SPR 20526 */
        p_ass->transmit.remote[counter].min_rtt = 0;
        p_ass->transmit.remote[counter].max_rtt = 0;
        p_ass->transmit.remote[counter].last_recorded_rtt = 0;
        /* End SPR 20526 */
    }

    /*  The Receive variables per-stream ... */

    p_ass->receive.first_data_flag = 0;
    p_ass->receive.congestion_event_sent = 0;
    p_ass->receive.overrule_delayack = SCTP_FALSE;
    p_ass->receive.number_duplicate_tsn = 0;
    p_ass->receive.state = SCTP_RX_STATE_NULL;
    p_ass->receive.cumtsn = 0;
    p_ass->receive.num_in_streams = p_ass->config.num_in_streams;

    /* Initialise sackfreelist  **/
    for( counter = 0; counter < SCTP_MAX_NUMSACKFRAGS; counter++ )
        lstAdd( &p_ass->receive.sackfreelist,
                ( NODE * )( &p_ass->receive.sackbuf[counter] ) );

    /***PQT SPR 4218***/
    /* Initialise nextssn for all receive queue */
    for( counter = 0; counter < sctp_db_main.max_streams_per_assoc; counter++ )
    {
        p_ass->receive.stream[counter].nextssn = 0;
        p_ass->receive.stream[counter].nextssn_notify = 0;
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_ass->restrict.asconf  = 0;
    p_ass->restrict.addip   = 0;
    p_ass->restrict.delip   = 0;
    p_ass->restrict.setprim = 0;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_ass->sctp_restrict.asconf  = 0;
    p_ass->sctp_restrict.addip   = 0;
    p_ass->sctp_restrict.delip   = 0;
    p_ass->sctp_restrict.setprim = 0;
#endif
#endif
}


/***************************************************************************
 *  FUNCTION :    sctp_db_get_free_association
 *
 *  DESCRIPTION:
 *    get a free association from association pool.
 *
 *  RETURN VALUE:
 *    in case of success pointer to the endpoint
 *    in case of failure NULL
 ****************************************************************************/
sctp_tcb_st *
sctp_db_get_free_association(
    sctp_error_t  *p_ecode )
{
    sctp_tcb_st   *p_ass;

    SCTPLOCK();
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_free_association" ) );

    if( 0 != lstCount( &sctp_db_main.assoc_freelist ) )
    {
        p_ass = ( sctp_tcb_st * )lstFirst( &sctp_db_main.assoc_freelist );

        if( SCTP_NULL != p_ass )
        {
            lstDelete( &sctp_db_main.assoc_freelist, ( NODE * )p_ass );
            ( p_ass )->open = 1;


            if( SCTP_FAILURE == sctp_db_get_assoc_freelists( p_ass, p_ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "Get Assoc Freelist Failed !!!" ) );
                *p_ecode = E_DB_GET_FREELIST_FAILED;

                /* Add this node back to the free list */
                lstAdd( &sctp_db_main.assoc_freelist, ( NODE * )p_ass );

                SCTPUNLOCK();

                return ( SCTP_NULL );
            }
        }

        SCTPUNLOCK();
        return ( p_ass );
    }

    *p_ecode = E_DB_NO_FREE_ASSOCIATION;

    SCTPUNLOCK();

    return ( SCTP_NULL );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_update_assoc_on_cookie
 *
 *  DESCRIPTION:
 *    Update Association structure based on Cookie message received.
 *
 *  RETURN VALUE:
 *   None
 ****************************************************************************/
void
sctp_db_update_assoc_on_cookie(
    sctp_tcb_st          *p_ass,
    sctp_chunk_cookie_st *cookie )
{
    sctp_U32bit         counter;
    sctp_U32bit         dest_addr_counter;
    sctp_return_t       ret;
    sctp_U32bit         saddr_index;
    sctp_error_t        s_err;
    sctp_suppaddr_st    supp_addr_type;

    p_ass->local_tag = cookie->local_init_parameters.fixed_params.init_tag;
    p_ass->remote_tag = cookie->remote_init_parameters.fixed_params.init_tag;
    p_ass->transmit.tsn = cookie->local_init_parameters.fixed_params.init_tsn;
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    p_ass->transmit.serial_no = p_ass->transmit.tsn;
#endif

    p_ass->transmit.rwndthere
        = cookie->remote_init_parameters.fixed_params.a_rwnd;
    p_ass->transmit.num_out_streams
        = cookie->local_init_parameters.fixed_params.num_out_streams;

    p_ass->transmit.lowest_path_mtu = SCTP_MAX_MTUSIZE;
    saddr_index = 0;

    sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr_type );

    for( counter = 0, dest_addr_counter = 0;
            counter < cookie->remote_init_parameters.num_addrs; counter++ )
    {
        /*
         * Ignore the address type that the local endpoint does not support.
         */
        if( ( ( cookie->remote_init_parameters.addr_list[counter].family == SCTP_AF_INET6 ) &&
                ( supp_addr_type.v6 != SCTP_TRUE ) ) ||
                ( ( cookie->remote_init_parameters.addr_list[counter].family == SCTP_AF_INET ) &&
                  ( supp_addr_type.v4 != SCTP_TRUE ) ) )
        {
            continue;
        }

        p_ass->transmit.remote[dest_addr_counter].that_address
            = cookie->remote_init_parameters.addr_list[counter];

        /* Update the Source Address Index */
        /* logic changed for v4_v6 */
        sctp_db_get_v4_v6_addr_index_with_af( p_ass, &saddr_index,
                                              p_ass->transmit.remote[dest_addr_counter].that_address.family, &s_err );

        p_ass->transmit.remote[dest_addr_counter].src_addr_index = saddr_index;

        saddr_index++;

        if( saddr_index == p_ass->p_ep->naddr )
        {
            saddr_index = 0;
        }

        p_ass->transmit.num_remote_address++;

        p_ass->transmit.remote[dest_addr_counter].rto = p_ass->config.rto_init;
        /* Update last used parameter */
        __GETSYSTIME( &p_ass->transmit.remote[dest_addr_counter].last_used_time );

        ret = sctp_get_pathmtu( p_ass,
                                &p_ass->transmit.remote[dest_addr_counter].that_address,
                                &p_ass->transmit.remote[dest_addr_counter].path_mtu );

        if( SCTP_SUCCESS == ret )
        {
            /* This means path mtu retrieved successfully,
             *  start pmtu rediscover timer
             */

            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERPMTU,
                              SCTP_PMTU_REDISCOVER_TIME,
                              dest_addr_counter,
                              &p_ass->transmit.remote[dest_addr_counter].
                              dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
        }

        else
        {
            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERPMTU,
                              SCTP_PMTU_DISCOVER_FAIL_TIME,
                              dest_addr_counter,
                              &p_ass->transmit.remote[dest_addr_counter].
                              dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
        }


        if( p_ass->transmit.remote[dest_addr_counter].path_mtu
                < p_ass->transmit.lowest_path_mtu )
            p_ass->transmit.lowest_path_mtu
                = p_ass->transmit.remote[dest_addr_counter].path_mtu;

        sctp_slowstart_at_setup( p_ass, dest_addr_counter );

        /*Initially mark all the addresses is the lost state*/
        p_ass->transmit.remote[dest_addr_counter].status
            = SCTP_COMMUNICATION_LOST;

        p_ass->transmit.remote[dest_addr_counter].path_error_threshold
            = p_ass->config.path_max_retrans;

        /*Initially mark the address as Unconfirmed */
        p_ass->transmit.remote[dest_addr_counter].is_confirmed   = SCTP_FALSE;
        p_ass->transmit.remote[dest_addr_counter].perf_path_verf = SCTP_TRUE;

        /*
         * Increment the destination address index.
         */
        dest_addr_counter++;
    }

    /*
     * Set the number of destination addresses equal to the number of
     * addresses that we support.
     */

    p_ass->receive.cumtsn
        = cookie->remote_init_parameters.fixed_params.init_tsn - 1;
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    p_ass->receive.serial_no = p_ass->receive.cumtsn;
#endif
    p_ass->receive.num_in_streams
        = cookie->local_init_parameters.fixed_params.num_in_streams;

    /* update ecn capable parameter */
    p_ass->ecn.ecn_capable = cookie->ecn_capable;

    /* start inactivity timer */
    sctp_start_timer(
        p_ass,
        SCTPEVENT_TIMERINACTIVE,
        p_ass->config.hb_interval
        + p_ass->transmit.remote[p_ass->transmit.primary_address_index].rto,
        0,
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
        SCTP_NULL );

    return;
}

/***************************************************************************
 *  FUNCTION :    sctp_db_add_association
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to add an
 *    association in the Database Module Data Structure. It will
 *    return a non negative association Id if successful.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_add_association(
    sctp_U32bit           ep_id,
    sctp_U16bit           that_port,
    sctp_chunk_cookie_st  *cookie,
    sctp_U32bit           *p_assoc_id,
    sctp_error_t          *p_ecode )
{

    sctp_ep_st  *p_ep;
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_return_t add_result = SCTP_FAILURE;
    sctp_Boolean_t  rollover_occured = SCTP_FALSE;
    sctp_tcb_st *p_ass_next = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_add_association" ) );

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( ep_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_db_get_ep_ptr(%d) failed", ep_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    p_ass = sctp_db_get_free_association( p_ecode );

    if( SCTP_NULL == p_ass )
    {
        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "db_add_association:Couldnot get a free TCB." ) );
        return ( SCTP_FAILURE );
    }

    do
    {
        if( ( SCTP_ROLLOVERGUARD == p_ep->last_used_assoc ) &&
                ( SCTP_FALSE == rollover_occured ) )
        {
            p_ep->last_used_assoc = 0;
            rollover_occured = SCTP_TRUE;
        }

        else
        {
            p_ep->last_used_assoc++;
        }

        p_ass->assoc_id = p_ep->last_used_assoc;

        p_ass_next = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

        while( ( p_ass_next != SCTP_NULL ) && ( p_ass_next->assoc_id < p_ass->assoc_id ) )
        {
            p_ass_next = ( sctp_tcb_st * )lstNext( ( NODE * )p_ass_next );
        }

        if( p_ass_next != SCTP_NULL )
        {
            if( p_ass_next->assoc_id == p_ass->assoc_id )
            {
                add_result = SCTP_FAILURE;
            }

            else
            {
                lstInsert( &p_ep->assoc_list, ( ( NODE * )p_ass_next )->previous,
                           ( NODE * )p_ass );
                add_result = SCTP_SUCCESS;
            }
        }

        else
        {
            lstAdd( &p_ep->assoc_list, ( NODE * )p_ass );
            add_result = SCTP_SUCCESS;
        }
    }
    while( ( SCTP_FAILURE == add_result ) && ( rollover_occured == SCTP_FALSE ) );

    if( rollover_occured == SCTP_TRUE && SCTP_FAILURE == add_result )
    {
        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "db_add_association: Could not assign assoc_id to the TCB." ) );
        lstAdd( &sctp_db_main.assoc_freelist, ( NODE * )p_ass );
        return ( SCTP_FAILURE );
    }

    p_ep->n_assoc++;
    p_ass->p_ep = p_ep;

    /* Copy server parameters */
    sctp_db_get_prot_params( &p_ass->config, p_ep->ep_id, SCTP_FALSE );
    sctp_get_cong_reg_def( SCTP_TRUE, SCTP_FALSE, p_ep->ep_id,
                           &p_ass->trx_cong_reg_def );
    sctp_get_cong_reg_def( SCTP_FALSE, SCTP_FALSE, p_ep->ep_id,
                           &p_ass->rcv_cong_reg_def );

    p_ass->state  = SCTPSTATE_ESTABLISHED;

    p_ass->transmit.that_port = that_port;

    sctp_db_update_assoc_on_cookie( p_ass, cookie );

    /* SPR 6854 */
    p_ass->configured_local_primary_address_index =

        p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].src_addr_index;
    /* SPR 6854 */

    SCTP_INIT_ASSOCSTAT( p_ass );

    *p_assoc_id = p_ass->assoc_id;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_create_tcb
 *
 *  DESCRIPTION:
 *    This Database Access function will create a tcb in the given endpoint
 *
 *  RETURN VALUE:
 *    Pointer to the TCB or SCTP_NULL
 *
 ****************************************************************************/
sctp_tcb_st *
sctp_db_create_tcb(
    sctp_ep_st             *p_ep,
    sctp_U16bit            dport,
    sctp_U32bit            naddr,
    sctp_sockaddr_st       addr_list[],
    sctp_error_t           *p_err )
{
    sctp_U32bit  count;
    sctp_tcb_st  *p_tcb = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_create_tcb" ) );

    p_tcb = sctp_db_get_free_association( p_err );

    if( SCTP_NULL == p_tcb )
    {
        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "db_create_tcb:Couldnot get a free TCB." ) );

        return ( SCTP_NULL );
    }

    p_tcb->p_ep = p_ep;

    lstInsert( &p_ep->assoc_list, lstLast( &p_ep->assoc_list ), ( NODE * )p_tcb );
    p_ep->n_assoc++;

    if( SCTP_ROLLOVERGUARD == p_ep->last_used_assoc )
    {
        p_ep->last_used_assoc = 0;
    }

    else
    {
        p_ep->last_used_assoc++;
    }

    p_tcb->assoc_id = p_ep->last_used_assoc;

    sctp_db_get_prot_params( &p_tcb->config, p_ep->ep_id, SCTP_FALSE );
    sctp_get_cong_reg_def( SCTP_TRUE, SCTP_FALSE, p_ep->ep_id,
                           &p_tcb->trx_cong_reg_def );
    sctp_get_cong_reg_def( SCTP_FALSE, SCTP_FALSE, p_ep->ep_id,
                           &p_tcb->rcv_cong_reg_def );

    p_tcb->transmit.that_port = dport;
    p_tcb->transmit.num_remote_address = naddr;
    p_tcb->transmit.lowest_path_mtu = SCTP_DEFAULT_MTUSIZE;

    for( count = 0; count < naddr; count++ )
    {
        p_tcb->transmit.remote[count].that_address = addr_list[count];
        p_tcb->transmit.remote[count].rto = p_tcb->config.rto_init;
        p_tcb->transmit.remote[count].path_error_threshold =
            p_tcb->config.path_max_retrans;
        /* Changes For CSR#1-5037386 */
        p_tcb->transmit.remote[count].prefered_sack_dest = SCTP_FALSE;
        p_tcb->transmit.remote[count].unsent_packet_count = 0;
        p_tcb->transmit.remote[count].retrans_packet_count = 0;
        /* Changes For CSR#1-5037386 Ends */

        /* Changes for CSR#1-5628824 */
        p_tcb->transmit.remote[count].lowest_tsn = 0;
        p_tcb->transmit.remote[count].num_chunks_await_ack = 0;
        p_tcb->transmit.remote[count].outstanding = 0;
        p_tcb->transmit.remote[count].count = 0;
        /* Changes for CSR#1-5628824 Ends*/
        /* Start SPR 20526 */
        p_tcb->transmit.remote[count].min_rtt = 0;
        p_tcb->transmit.remote[count].max_rtt = 0;
        p_tcb->transmit.remote[count].last_recorded_rtt = 0;
        /* End SPR 20526 */
    }

    p_tcb->transmit.primary_address_index = 0;
    p_tcb->transmit.configured_primary_path_index = 0;
    p_tcb->transmit.fast_recovery_exit = 0;
    /* Changes For CSR#1-5037386, 1-5628824 */
    p_tcb->transmit.default_sack_dest_index = 0;
    /* Changes For CSR#1-5037386, 1-5628824 Ends */

    p_tcb->zero_win_prob_on = SCTP_FALSE;
    p_tcb->conn_accepted    = SCTP_TRUE;

    SCTP_INIT_ASSOCSTAT( p_tcb );

    return( p_tcb );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_delete_association
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to delete
 *    an association in the Database Module Data Structure.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_delete_association(
    sctp_tcb_st    *p_ass )
{
    sctp_ep_st    *p_ep;
    sctp_endpoint_chg_data_ut   state_data;
    sctpbuffer_st  *p_buf = SCTP_NULL;
    sctpbuffer_st  *save = SCTP_NULL;
#if defined(SCTP_STATS_ENABLED) && defined(SCTP_ASSOC_STATS_RECOVERY_SUPPORT)
    sctp_error_t           e_code;
    sctp_U8bit       *p_stats_data;
#endif
    /* M3UA Socket interface problem
      SCTPLOCK();  // SPR 3672  */

    p_ep = p_ass->p_ep;
    SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_delete_association" ) );

    /*SPR 21155 changes starts*/

    if( SCTP_FALSE == SCTP_SOCK_IS_IN_USE( p_ep->flags ) )
    {
        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "In function sctp_db_delete_association socket is not in use" ) );

        return SCTP_SUCCESS;
    }

    /*SPR 21155 changes ends*/

#if defined(SCTP_STATS_ENABLED) && defined(SCTP_ASSOC_STATS_RECOVERY_SUPPORT)
    p_stats_data = ( sctp_U8bit * )__MALLOC( sizeof( sctp_assoc_stats_st ) );

    if( sctp_api_get_stats( SCTP_ASSOC_STATS, p_ep->ep_id, p_stats_data, SCTP_FALSE, &e_code ) ==
            SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "Error in getting Association stats" ) );
    }

    else
    {
        sctp_stk_api_stats_recovery_ind( p_ass->assoc_id,
                                         ( sctp_assoc_stats_st * )p_stats_data );
    }

    __FREE( p_stats_data );
#endif

    __MEMSET( ( sctp_U8bit * )&state_data, 0, sizeof( sctp_endpoint_chg_data_ut ) );

    switch( p_ass->state )
    {
        case SCTPSTATE_ESTABLISHED :
        case SCTPSTATE_SHUTDOWNPEND :
        case SCTPSTATE_SHUTDOWNRCVD :
            {
                if( p_ass->fsm_event == SCTPEVENT_MSGABORT )
                {
                    if( p_ass->event.msg_abort.cause_length != 0 )
                    {
                        state_data.endpoint_down.cause_len = p_ass->event.msg_abort.cause_length;
                        state_data.endpoint_down.cause     = p_ass->event.msg_abort.p_cause;
                    }
                }

                sctp_ntfy_endpt_status_change( p_ass, SCTP_COMMUNICATION_LOST,
                                               &state_data, E_NOERROR );
                break;
            }

        case SCTPSTATE_SHUTDOWNSENT :
        case SCTPSTATE_SHUTDOWNACKSENT :
            {
                {
                    sctp_ntfy_endpt_status_change( p_ass, SCTP_SHUTDOWN_COMPLETE,
                                                   &state_data, E_NOERROR );
                }
                break;
            }

        case SCTPSTATE_COOKIEWAIT :
        case SCTPSTATE_COOKIESENT :
            {
                sctp_ntfy_endpt_status_change( p_ass, SCTP_CANT_START_ASSOC,
                                               &state_data, E_NOERROR );

                for( p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.
                                                          transmit_queue );
                        p_buf != SCTP_NULL; )
                {
                    save = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf );
                    lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )p_buf );
                    sctp_free_transmit_buffer( p_ass, p_buf );
                    sctp_ntfy_send_failure( p_ass, p_buf->context, SCTP_DATA_UNSENT,
                                            E_NOERROR, sizeof( sctpbuffer_st ), ( sctp_U8bit * )p_buf );

                    SCTP_INCR_ASSOC_STREAM_STAT( p_ass,
                                                 p_buf->stream,
                                                 SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL,
                                                 1 );

                    p_buf = save;
                }

                break;
            }
    }

    p_ass->state = SCTPSTATE_CLOSED;

    SCTPLOCK();  /* M3UA Socket interface problem */

    /*SPR 5088*/
#ifdef SCTP_UI_SOCK_API
    /*clear the data indication*/
    sctp_db_clear_data_ind( p_ass, 0 );
    /*do a review of readability status*/
    /*Bug ID 61 changes S*/
    /*sctp_db_update_readable_status(p_ass->p_ep);*/
    /*Bug ID 61 changes E*/
#endif
    /*SPR 5088*/

    /*Changes for CSR No 1-2480893*/
    /* Free the Cookie List */
    p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );

    while( p_buf )
    {
        sctp_free_transmit_buffer( p_ass, p_buf );
        p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );
    }

    /* cleanup all association queues */
    sctp_db_cleanup_queues( p_ass, SCTP_TRUE );

    /** remove from ass_hash table **/
    sctp_db_delete_assoc_verf( p_ass );

    /* return tx and rx buffer to global pool */
    sctp_db_free_assoc_freelists( p_ass );

    /*  clean association data */
    sctp_db_clean_association( p_ass, SCTP_FALSE );
    lstDelete( &p_ep->assoc_list, ( NODE * )p_ass );
    p_ep->n_assoc--;
    /*Bug ID 61 changes S*/
#ifdef SCTP_UI_SOCK_API
    sctp_db_update_readable_status( p_ass->p_ep );
#endif
    /*Bug ID 61 changes E*/


    lstAdd( &sctp_db_main.assoc_freelist, ( NODE * )p_ass );

    SCTPUNLOCK();


    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_get_free_ep
 *
 *  DESCRIPTION:
 *    get a free EndPoint from EndPoint pool.
 *
 *  RETURN VALUE:
 *    pointer to the endpoint or SCTP_NULL
 *
 ****************************************************************************/
sctp_ep_st *
sctp_db_get_free_ep(
    sctp_error_t  *p_ecode )
{
    sctp_ep_st  *p_ep = SCTP_NULL;
    sctp_U32bit counter, sctp_index = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_get_free_ep" ) );

    if( sctp_db_main.num_active_ep < sctp_db_main.max_ep )
    {
        for( counter = 0; counter < sctp_db_main.max_ep; counter++ )
        {
            sctp_index = ( counter + sctp_db_main.last_allocated_ep + 1 ) %
                         sctp_db_main.max_ep;

            if( 0 == SCTP_SOCK_IS_IN_USE( sctp_db_main.ep_table[sctp_index].flags ) )
            {
                sctp_db_main.num_active_ep++;
                SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_ASSOC_REG, 1 );
                sctp_db_main.last_allocated_ep = sctp_index;

                p_ep = &sctp_db_main.ep_table[sctp_index];
                p_ep->ep_id = sctp_index;
                p_ep->server_ep_id = p_ep->ep_id;
                p_ep->send_func = SCTP_NULL; /* Callback for Sending Func SPR 20859 */


                /* Set the INUSE flag */
                SCTP_SOCK_SET_IN_USE( p_ep->flags, SCTP_TRUE );

                /* Set the other not writeable flags */
                SCTP_SOCK_SET_WRITEABLE( p_ep->flags, SCTP_FALSE );
                SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );
                SCTP_SOCK_SET_RD_BLOCKED( p_ep->flags, SCTP_FALSE );
                SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_FALSE );

                /* copy default protocol parameters */
                sctp_db_get_prot_params( &p_ep->sk_config, 0, SCTP_TRUE );
                sctp_get_cong_reg_def( SCTP_TRUE, SCTP_TRUE, 0,
                                       &p_ep->trx_cong_reg_def );
                sctp_get_cong_reg_def( SCTP_FALSE, SCTP_TRUE, 0,
                                       &p_ep->rcv_cong_reg_def );
                /* SPR 20567 Starts */
                p_ep->hostname_supported = SCTP_FALSE;
                /* SPR 20567 Ends */

                break;
            }
        }

        if( counter != sctp_db_main.max_ep )
        {
            return ( p_ep );
        }
    }

    *p_ecode = E_DB_NO_FREE_ENDPOINT;

    return ( SCTP_NULL );
}


/***************************************************************************
 *  FUNCTION :  sctp_db_bind_ep
 *
 *  DESCRIPTION:
 *    This Database Access function will store the local port and local
 *    Address in Endpoint structure.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_bind_ep(
    sctp_U32bit              ep_id,
    sctp_U16bit              port,
    sctp_U32bit              no_addr,
    sctp_sockaddr_st         list_of_addrs[],
    sctp_error_t             *p_ecode )
{
    sctp_U32bit       counter, counter1;
    sctp_ep_st        *p_ep;
    sctp_sockaddr_st  *addrlist;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/



    addrlist = ( sctp_sockaddr_st * ) &list_of_addrs[0];

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_bind_ep" ) );

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( ep_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_db_get_ep_ptr(%d) failed", ep_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    if( ( 0 == ( p_ep->naddr + no_addr ) )
            || ( SCTP_MAX_TRANSPORT_ADDR < ( p_ep->naddr + no_addr ) ) )
    {
        return ( SCTP_FAILURE );
    }

    if( SCTP_SUCCESS ==  sctp_db_find_server_created_port( port,
                                                           no_addr, &list_of_addrs[0] ) )
    {
        return ( SCTP_FAILURE );
    }

    for( counter = 0; counter < no_addr; counter++ )
    {
        /* Changes for SPR-21022 Starts here (Klocwork) */
        for( counter1 = 0; ( counter1 < p_ep->naddr ) && ( counter1 < SCTP_MAX_TRANSPORT_ADDR );
                counter1++ )
            /* Changes for SPR-21022 Ends here (Klocwork) */
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( &p_ep->this_address[counter1],
                                               &addrlist[counter] ) )
            {
                break;
            }
        }

        /* Changes for SPR-21022 Starts here (Klocwork) */
        if( ( p_ep->naddr == counter1 ) && ( counter1 < SCTP_MAX_TRANSPORT_ADDR ) )
            /* Changes for SPR-21022 Ends here (Klocwork) */
        {
            p_ep->this_address[p_ep->naddr] = addrlist[counter];
            p_ep->naddr++;
        }
    }

    p_ep->this_port = port;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :  sctp_db_bind_ep_with_af_check
 *
 *  DESCRIPTION:
 *    This Database Access function will bind the endpoint to the local addrs
 *  of the system which are of the same family as the endpoint. This will only
 *  be called in case there the endpoint is not already bound.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_bind_ep_with_af_check(
    sctp_U32bit          ep_id,
    sctp_U16bit          port,
    sctp_error_t         *p_ecode )
{
    sctp_U32bit       counter = 0;
    sctp_ep_st        *p_ep;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_bind_ep_with_af_check" ) );

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( ep_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_db_get_ep_ptr(%d) failed", ep_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    p_ep->naddr = 0;

    /* if family == INET6 , assign all addr else only v4 addr */
    if( SCTP_AF_INET == p_ep->family )
    {
        /* Copy only the INET addresses */
        for( counter = 0; ( counter < sctp_db_main.num_local_addrs )
                && ( sctp_db_main.num_local_addrs <= SCTP_MAX_TRANSPORT_ADDR ); counter++ ) /* klockwork fix 6.2.4 */
        {
            if( SCTP_AF_INET == sctp_db_main.addr_list[counter].family )
            {
                p_ep->this_address[p_ep->naddr]
                    = sctp_db_main.addr_list[counter];

                p_ep->naddr++;
            }
        }
    }

    else
    {
        for( counter = 0; ( counter < sctp_db_main.num_local_addrs )
                && ( sctp_db_main.num_local_addrs <= SCTP_MAX_TRANSPORT_ADDR ); counter++ ) /* klockwork fix 6.2.4 */
        {
            p_ep->this_address[p_ep->naddr]
                = sctp_db_main.addr_list[counter];

            p_ep->naddr++;
        }
    }

    /* Could we add any address */
    if( !p_ep->naddr )
    {
        return ( SCTP_FAILURE );
    }

    p_ep->this_port = port;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :  sctp_db_create_ep
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to add an
 *    EndPoint in the Database Module Data Structure. It will
 *    return a non negative ep Id if successful.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_create_ep(
    sctp_U32bit          server_id,
    sctp_U32bit          *p_ep_id,
    sctp_error_t         *p_ecode )
{
    sctp_ep_st *p_ep = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_create_ep" ) );

    p_ep = sctp_db_get_free_ep( p_ecode );

    if( SCTP_NULL == p_ep )
    {
        return ( SCTP_FAILURE );
    }

    /* Copy server parameters */
    sctp_db_get_prot_params( &p_ep->sk_config, server_id, SCTP_FALSE );
    sctp_get_cong_reg_def( SCTP_TRUE, SCTP_TRUE, server_id, &p_ep->trx_cong_reg_def );
    sctp_get_cong_reg_def( SCTP_FALSE, SCTP_TRUE, server_id, &p_ep->rcv_cong_reg_def );

    p_ep->n_assoc = 0;
    p_ep->last_used_assoc = 0;
    p_ep->create_from_server = SCTP_FALSE;
    p_ep->server_ep_id = p_ep->ep_id;
    p_ep->num_created_ep = 0;



    lstInit( &p_ep->assoc_list );


#ifdef SCTP_UI_SOCK_API

    /* set the blocking flag for both TCP and UDP Style Sockets */
    SCTP_SOCK_SET_BLOCKING( p_ep->flags, 1 );

    p_ep->ulpqueue.p_ep  = p_ep;
    p_ep->ulpqueue.n_ind = 0;
    p_ep->aclose_time    = 0;

    /*All socket options are OFF for both one to one and one to many style
      sockets.*/
    if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
    {
        /* Set the Notification Flags for UDP style Socket */
        SCTP_SOCK_SET_RECVDATAIO( p_ep->ulpqueue.ntfy_flags, 0 );
        SCTP_SOCK_SET_RECVASSOC( p_ep->ulpqueue.ntfy_flags, 0 );
        SCTP_SOCK_SET_RECVPADDR( p_ep->ulpqueue.ntfy_flags, 0 );
        SCTP_SOCK_SET_RECVPEERERR( p_ep->ulpqueue.ntfy_flags, 0 );
        SCTP_SOCK_SET_RECVSENDFAIL( p_ep->ulpqueue.ntfy_flags, 0 );
        SCTP_SOCK_SET_RECVSHUTDOWN( p_ep->ulpqueue.ntfy_flags, 0 );
    }

    else if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        /* Set the Notification Flags for TCP style Socket */
        SCTP_SOCK_SET_RECVDATAIO( p_ep->ulpqueue.ntfy_flags, 0 );
        /*SCTP_ASSOC_CHANGE event*/
        SCTP_SOCK_SET_RECVASSOC( p_ep->ulpqueue.ntfy_flags, 0 );

        /*SCTP_PEER_ADDR_CHANGE, ignored for one to one sockets*/
        SCTP_SOCK_SET_RECVPADDR( p_ep->ulpqueue.ntfy_flags, 0 );

        /*SCTP_REMOTE_ERROR, ignored for one-to-one sockets*/
        SCTP_SOCK_SET_RECVPEERERR( p_ep->ulpqueue.ntfy_flags, 0 );

        SCTP_SOCK_SET_RECVSENDFAIL( p_ep->ulpqueue.ntfy_flags, 0 );
        SCTP_SOCK_SET_RECVSHUTDOWN( p_ep->ulpqueue.ntfy_flags, 0 );
    }

    /*  initilize the ulp queue */
    lstInit( &( p_ep->ulpqueue.ntfy_queue ) );
    lstInit( &( p_ep->ulpqueue.data_ind_list ) );
#else

#endif

    /* Set the other flags parameter */
    SCTP_SOCK_SET_WRITEABLE( p_ep->flags, SCTP_FALSE );
    SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );
    SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_FALSE );
    SCTP_SOCK_SET_RD_BLOCKED( p_ep->flags, SCTP_FALSE );

    /* Commenting following three statments for CSR#1-4859282 */

    *p_ep_id = p_ep->ep_id;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :  sctp_db_create_duplicate_ep
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to add an
 *    EndPoint in the Database Module Data Structure. It will
 *    return a non negative ep Id if successful.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_create_duplicate_ep(
    sctp_ep_st           *p_ep,
    sctp_tcb_st          *p_tcb,
    sctp_ep_st           **pp_new_ep,
    sctp_error_t         *p_ecode )
{
    sctp_U32bit            count;
    sctp_U32bit            new_assoc_id;

#ifdef SCTP_UI_SOCK_API
    /*SPR 5335*/
    sctp_ulpqueue_node_st  *p_currind;
    sctp_ulpqueue_node_st  *p_nextind;
    sctp_data_indication_st  *p_currdata;
    sctp_data_indication_st  *p_nextdata;
    /*SPR 5335*/
#endif  /* SCTP_UI_SOCK_API */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_create_duplicate_ep" ) );

    *pp_new_ep = sctp_db_get_free_ep( p_ecode );

    if( SCTP_NULL == *pp_new_ep )
    {
        return ( SCTP_FAILURE );
    }

    /* Copy server parameters */
    sctp_db_get_prot_params( &( *pp_new_ep )->sk_config, p_ep->ep_id, SCTP_FALSE );
    sctp_get_cong_reg_def( SCTP_TRUE, SCTP_FALSE, p_ep->ep_id, &( *pp_new_ep )->trx_cong_reg_def );
    sctp_get_cong_reg_def( SCTP_FALSE, SCTP_FALSE, p_ep->ep_id, &( *pp_new_ep )->rcv_cong_reg_def );

    ( *pp_new_ep )->n_assoc     = 0;
    ( *pp_new_ep )->last_used_assoc = 0;
    ( *pp_new_ep )->l_backlog   = 0;
    ( *pp_new_ep )->create_from_server = SCTP_FALSE;
    ( *pp_new_ep )->server_ep_id = ( *pp_new_ep )->ep_id;
    ( *pp_new_ep )->num_created_ep = 0;

    lstInit( &( *pp_new_ep )->assoc_list );

#ifdef SCTP_UI_SOCK_API

    ( *pp_new_ep )->ulpqueue.p_ep  = *pp_new_ep;
    ( *pp_new_ep )->ulpqueue.n_ind = 0;
    ( *pp_new_ep )->ulpqueue.ntfy_flags = p_ep->ulpqueue.ntfy_flags;

    /*  initilize the ulp queue */
    lstInit( &( ( *pp_new_ep )->ulpqueue.ntfy_queue ) );
    lstInit( &( ( *pp_new_ep )->ulpqueue.data_ind_list ) );
#else

#endif /* SCTP_UI_SOCK_API */

    ( *pp_new_ep )->flags = p_ep->flags;
    /* Reset the Listener Flag of New ep */
    SCTP_SOCK_SET_LISTENER( ( *pp_new_ep )->flags, SCTP_FALSE );

    /* Reset the readable flag for the ep */
    SCTP_SOCK_SET_READABLE( ( *pp_new_ep )->flags, SCTP_FALSE );

    /* Reset the writeable flag for the ep */
    SCTP_SOCK_SET_WRITEABLE( ( *pp_new_ep )->flags, SCTP_TRUE );

    ( *pp_new_ep )->this_port = p_ep->this_port;
    ( *pp_new_ep )->naddr = p_ep->naddr;

    for( count = 0; count < p_ep->naddr; count++ )
    {
        ( *pp_new_ep )->this_address[count] = p_ep->this_address[count];
    }

    /* Add the New EP to Bind Hash Table */
    sctp_db_add_hash( SCTP_BIND_HASH, *pp_new_ep );

    for( count = 0; count < SCTP_MAX_HOSTNAME_SIZE; count++ )
    {
        ( *pp_new_ep )->hostname[count] = p_ep->hostname[count];
    }

    ( *pp_new_ep )->appl_id = p_ep->appl_id;
    ( *pp_new_ep )->family = p_ep->family;
    ( *pp_new_ep )->sk_type = p_ep->sk_type;
    ( *pp_new_ep )->trc_flags = p_ep->trc_flags;
    ( *pp_new_ep )->so_linger = p_ep->so_linger;
    ( *pp_new_ep )->send_func = p_ep->send_func; /* Callback for Sending Func SPR 20859 */

    /* copy the tcb in new endpoint */
    lstDelete( &p_ep->assoc_list, ( NODE * )p_tcb );
    p_ep->n_assoc--;

    lstAdd( &( *pp_new_ep )->assoc_list, ( NODE * )p_tcb );
    ( *pp_new_ep )->n_assoc++;
    ( *pp_new_ep )->last_used_assoc++;

    new_assoc_id = ( *pp_new_ep )->last_used_assoc;

#ifdef SCTP_UI_SOCK_API
    /*SPR 5335*/
    p_currind = ( sctp_ulpqueue_node_st * )lstFirst( &( p_ep->ulpqueue.ntfy_queue ) );

    while( p_currind != SCTP_NULL )
    {
        p_nextind = ( sctp_ulpqueue_node_st * )lstNext( ( NODE * )p_currind );

        if( p_currind->assoc_id == p_tcb->assoc_id )
        {
            lstDelete( &p_ep->ulpqueue.ntfy_queue, ( NODE * )p_currind );
            p_ep->ulpqueue.n_ind--;

            lstAdd( &( ( *pp_new_ep )->ulpqueue.ntfy_queue ), ( NODE * )p_currind );
            ( *pp_new_ep )->ulpqueue.n_ind++;

            p_currind->assoc_id = new_assoc_id;
        }

        p_currind = p_nextind;
    }

    p_currdata = ( sctp_data_indication_st * )lstFirst( &( p_ep->ulpqueue.data_ind_list ) );

    while( p_currdata != SCTP_NULL )
    {
        p_nextdata = ( sctp_data_indication_st * )lstNext( ( NODE * )p_currdata );

        if( p_currdata->p_assoc == p_tcb )
        {
            lstDelete( &p_ep->ulpqueue.data_ind_list, ( NODE * )p_currdata );
            lstAdd( &( ( *pp_new_ep )->ulpqueue.data_ind_list ), ( NODE * )p_currdata );
            break;
        }

        p_currdata = p_nextdata;
    }

    sctp_db_update_readable_status( *pp_new_ep );
    /*SPR 5335*/
#endif  /* SCTP_UI_SOCK_API */

    p_tcb->assoc_id = new_assoc_id;
    p_tcb->p_ep = *pp_new_ep;
    /*SPR 5217*/
    sctp_timer_update_assoc_id( p_tcb );
    /*SPR 5217*/

    /* Commenting following three statements for CSR#1-4859282 */

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_delete_ep
 *
 *  DESCRIPTION:
 *    This Database Access function will invoke procedure to delete
 *    an End point in the Database Module Data Structure.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_delete_ep(
    sctp_U32bit      ep_id,
    sctp_tcb_st      *p_ass,
    sctp_Boolean_t   ep_flag )
{
    sctp_ep_st    *p_ep;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ep_flag );
    /*to avoid the warning*/

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( ep_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "sctp_db_get_ep_ptr(%d) failed", ep_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                ( "In function sctp_db_delete_ep" ) );

    if( SCTP_NULL != p_ass )
    {
        sctp_db_delete_association( p_ass );
    }

    /*SPR 5048*/
    SCTP_WAKEUP( p_ep->cond );
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    SCTP_WAKEUP( p_ep->asconf_cond );
#endif
    /*SPR 5048*/


#ifdef SCTP_UI_SOCK_API

    /*SPR 20805 Start*/
    if( ( p_ep->n_assoc == 0 ) &&
            ( SCTP_SOCK_IS_CLOSE_CALLED( p_ep->flags ) ) )
        /*SPR 20805 Stop*/
    {

        sctp_db_ulpqueue_empty( &p_ep->ulpqueue );
#else

    /**** CONNECT_BUGFIX START ****/
    if( ( !SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
            || ( SCTP_SOCK_IS_CLOSE_CALLED( p_ep->flags ) ) )
        /**** CONNECT_BUGFIX END ****/
    {
#endif /* SCTP_UI_SOCK_API */

        if( 1 == SCTP_SOCK_IS_HASHED( p_ep->flags ) )
        {
            if( 1 == SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
            {
                sctp_db_del_hash( SCTP_LISTEN_HASH, p_ep );
            }

            else
            {
                sctp_db_del_hash( SCTP_BIND_HASH, p_ep );
            }
        }

        SCTP_SOCK_SET_LISTENER( p_ep->flags, SCTP_FALSE );
        SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_FALSE );
        SCTP_SOCK_SET_IN_USE( p_ep->flags, SCTP_FALSE );
        p_ep->naddr = 0;
        p_ep->send_func = SCTP_NULL; /* Callback for Sending Func SPR 20859 */

        sctp_db_main.num_active_ep--;

        SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_ASSOC_REG, 1 );
#ifdef SCTP_UI_SOCK_API

        /* If the endpoint is selected invoke select processing */
        if( SCTP_SOCK_IS_SELECTED( p_ep ) )
        {
            sctp_wakeup_selected( ep_id );
        }

#endif
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_validate_appl
 *
 *  DESCRIPTION:
 *    Validates an application.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_validate_appl(
    sctp_U16bit    application_id )
{

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_validate_appl,Id = %d",
                  application_id ) );

    if( ( application_id >= sctp_db_main.max_appl )
            || ( sctp_db_main.appl_list[application_id].open == 0 ) )
    {
        return ( SCTP_FAILURE );
    }

    else
    {
        return( SCTP_SUCCESS );
    }

}


/***************************************************************************
 *  FUNCTION :    sctp_db_register_new_appl
 *
 *  DESCRIPTION:
 *    Register New application
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_register_new_appl(
    sctp_U16bit           *appl_id,
    void                  *callback_func,
    void                  *p_appl_data,
    sctp_error_t          *p_ecode )
{
    sctp_U32bit  counter;
#ifdef SCTP_UI_SU_MESSAGEBASED
    sctp_U16bit            *p_su_id;
    sctp_U32bit            counter1;
#endif

    SCTPLOCK();
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_register_new_appl" ) );

    if( sctp_db_main.num_active_appl < sctp_db_main.max_appl )
    {
        for( counter = 0; counter < sctp_db_main.max_appl; counter++ )
            if( 0 == sctp_db_main.appl_list[counter].open )
            {
                break;
            }

        if( counter != sctp_db_main.max_appl )
        {

#ifdef SCTP_UI_SU_MESSAGEBASED
            p_su_id = ( sctp_U16bit * )callback_func;

            if( ( 0 == *p_su_id ) || ( SCTP_MAX_CLIENT_SUPPORTED <= *p_su_id ) ) /*SU-ID 0 is invalid*/
            {
                *p_ecode = E_INV_SU_ID;
                SCTPUNLOCK();
                return ( SCTP_FAILURE );
            }

            /*Search if su id is already registered*/
            for( counter1 = 0; counter1 < sctp_db_main.max_appl; counter1++ )
            {
                if( sctp_db_main.appl_list[counter1].su_id == *p_su_id )
                {
                    *p_ecode = E_SU_ALREADY_REGISTRED;
                    SCTPUNLOCK();
                    return ( SCTP_FAILURE );
                }
            }

            sctp_db_main.appl_list[counter].su_id = *p_su_id;
#else
            sctp_db_main.appl_list[counter].callback
                = ( sctp_callback_func_t )callback_func;
#endif
            sctp_db_main.appl_list[counter].open = 1;
            sctp_db_main.appl_list[counter].p_appl_data = p_appl_data;
            *appl_id = ( sctp_U16bit )counter;
            sctp_db_main.num_active_appl++;

            SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_APPL_REG, 1 );
            SCTPUNLOCK();
            return ( SCTP_SUCCESS );

        }
    }

    *p_ecode = E_NORESOURCES;
    SCTPUNLOCK();
    return( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_deregister_appl
 *
 *  DESCRIPTION:
 *    Deregister an application.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_deregister_appl(
    sctp_U16bit    appl_id )
{

    sctp_U32bit  counter;
    sctp_ep_st   *ep_table = sctp_db_main.ep_table;
    sctp_tcb_st  *p_curr_tcb = SCTP_NULL;
    sctp_tcb_st  *p_temp     = SCTP_NULL;

    SCTPLOCK();
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_db_deregister_appl, Id = %d",
                  appl_id ) );

    if( SCTP_SUCCESS != sctp_db_validate_appl( appl_id ) )
    {
        SCTPUNLOCK();
        return ( SCTP_FAILURE );
    }

    for( counter = 0; counter < sctp_db_main.max_ep; counter++ )
    {
        if( 1 == SCTP_SOCK_IS_IN_USE( ep_table[counter].flags )
                && ( ep_table[counter].appl_id == appl_id ) )
        {
            SCTP_SOCK_SET_CLOSE_CALLED( ep_table[counter].flags, SCTP_TRUE );
            /* Check for the associations in EndPoint */
            p_curr_tcb = ( sctp_tcb_st * )
                         lstFirst( &( ep_table[counter].assoc_list ) );

            if( SCTP_NULL != p_curr_tcb )
            {
                /* If any association then call state machine function for all
                 */
                while( p_curr_tcb )
                {
                    SCTPUNLOCK(); /* SPR 3672 */
                    p_temp = ( sctp_tcb_st * )lstNext( ( NODE * )p_curr_tcb );
                    p_curr_tcb->event.cmd_abort.cause_length = 0;
                    sctp_assoc_state_machine( p_curr_tcb, SCTPEVENT_CMDABORT );
                    p_curr_tcb = p_temp;
                    SCTPLOCK(); /* SPR 3672 */
                }
            }

            else
            {
                SCTP_LOCK_EP( counter );

                /* if no assoc simply delete the ep */
                sctp_db_delete_ep( counter, p_curr_tcb, SCTP_TRUE );

                SCTP_UNLOCK_EP( counter );
            }
        }
    }

    /* update application structure here  */
    sctp_db_main.appl_list[appl_id].open = 0;

#ifdef SCTP_UI_SU_MESSAGEBASED
    sctp_db_main.appl_list[appl_id].su_id = 0;
#else
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_db_main.appl_list[appl_id].callback = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_db_main.appl_list[appl_id].callback = ( sctp_callback_func_t )SCTP_NULL;
#endif
#endif
    sctp_db_main.appl_list[appl_id].p_appl_data = SCTP_NULL;
    sctp_db_main.num_active_appl--;
    SCTP_DECR_PROT_INTSTATS( SCTPSTATS_NO_APPL_REG, 1 );

    SCTPUNLOCK();
    return( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION :    sctp_db_buffer_mem_alloc
 *
 *  DESCRIPTION:
 *    This is the first function called at the time of db module
 *    initialisation. It performs following functions.
 *
 *    1) Allocate memory for maximum number of associations tx, rx buffers and
 *    initialise this memory.
 *
 *    2) On failure, it frees up the previously allocated memory
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_buffer_mem_alloc(
    sctp_init_options_st   *init_options,
    sctp_error_t           *p_ecode )
{

    /* allocate memory for number of global reserve  buffers */
    /* SR 1-954224 */
    /* SPR 20292 start */
    if( init_options->min_reserve_buffers != 0 )
    {
        sctp_db_main.reserve_mem_pool.min_pool = ( sctp_U8bit * )
                                                 __MALLOC( init_options->min_reserve_buffers *
                                                           ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        /* SR 1-954224 */

        if( SCTP_NULL == sctp_db_main.reserve_mem_pool.min_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /****************/

    /* SR 1-954224 */
    if( init_options->med_reserve_buffers != 0 )
    {
        sctp_db_main.reserve_mem_pool.med_pool = ( sctp_U8bit * )
                                                 __MALLOC( init_options->med_reserve_buffers *
                                                           ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        /* SR 1-954224 */

        if( SCTP_NULL == sctp_db_main.reserve_mem_pool.med_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /****************/

    /* SR 1-954224 */
    if( init_options->max_reserve_buffers != 0 )
    {
        sctp_db_main.reserve_mem_pool.max_pool = ( sctp_U8bit * )
                                                 __MALLOC( init_options->max_reserve_buffers *
                                                           ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        /* SR 1-954224 */

        if( SCTP_NULL == sctp_db_main.reserve_mem_pool.max_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /****************/

    /* SPR-6200 Starts*/

    /* allocate memory for number of mintx buffers */
    if( init_options->MaxPerAssoc.min_tx_buffers != 0 )
    {
        sctp_db_main.tx_mem_pool.min_pool = ( sctp_U8bit * )
                                            __MALLOC( init_options->MaxPerAssoc.min_tx_buffers *
                                                      init_options->max_tcb * ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        /* SPR-6200 ends*/

        if( SCTP_NULL == sctp_db_main.tx_mem_pool.min_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );
            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /****************/

    /* SPR-6200 Starts*/
    if( init_options->MaxPerAssoc.med_tx_buffers != 0 )
    {
        sctp_db_main.tx_mem_pool.med_pool = ( sctp_U8bit * )
                                            __MALLOC( init_options->MaxPerAssoc.med_tx_buffers *
                                                      init_options->max_tcb * ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        /* SPR-6200 ends*/

        if( SCTP_NULL == sctp_db_main.tx_mem_pool.med_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }

    }


    /* SPR-6200 Starts*/

    if( init_options->MaxPerAssoc.max_tx_buffers != 0 )
    {
        sctp_db_main.tx_mem_pool.max_pool = ( sctp_U8bit * )
                                            __MALLOC( init_options->MaxPerAssoc.max_tx_buffers *
                                                      init_options->max_tcb * ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        /* SPR-6200 ends*/

        if( SCTP_NULL == sctp_db_main.tx_mem_pool.max_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }

    }


    /* allocate memory for number of global rx buffers */

    if( init_options->MaxPerAssoc.min_rx_buffers != 0 )
    {
        sctp_db_main.rx_mem_pool.min_pool = ( sctp_U8bit * )
                                            __MALLOC( init_options->MaxPerAssoc.min_rx_buffers *
                                                      init_options->max_tcb * ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );


        if( SCTP_NULL == sctp_db_main.rx_mem_pool.min_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );
            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }


    if( init_options->MaxPerAssoc.med_rx_buffers != 0 )
    {
        sctp_db_main.rx_mem_pool.med_pool = ( sctp_U8bit * )
                                            __MALLOC( init_options->MaxPerAssoc.med_rx_buffers *
                                                      init_options->max_tcb * ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

        if( SCTP_NULL == sctp_db_main.rx_mem_pool.med_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }


    if( init_options->MaxPerAssoc.max_rx_buffers != 0 )
    {
        sctp_db_main.rx_mem_pool.max_pool = ( sctp_U8bit * )
                                            __MALLOC( init_options->MaxPerAssoc.max_rx_buffers *
                                                      init_options->max_tcb * ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );


        if( SCTP_NULL == sctp_db_main.rx_mem_pool.max_pool )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );

            *p_ecode = E_MALLOC_FAILED; /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /* SPR 20292 end */
    /* SR 1-954224 */
    /* initialise reserve Pool memory */
    __MEMSET( sctp_db_main.reserve_mem_pool.min_pool, 0,
              init_options->min_reserve_buffers *
              ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    __MEMSET( sctp_db_main.reserve_mem_pool.med_pool, 0,
              init_options->med_reserve_buffers *
              ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    __MEMSET( sctp_db_main.reserve_mem_pool.max_pool, 0,
              init_options->max_reserve_buffers *
              ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    /* initialise Tx Pool memory */
    __MEMSET( sctp_db_main.tx_mem_pool.min_pool, 0,
              init_options->MaxPerAssoc.min_tx_buffers *
              init_options->max_tcb *
              ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    __MEMSET( sctp_db_main.tx_mem_pool.med_pool, 0,
              init_options->MaxPerAssoc.med_tx_buffers *
              init_options->max_tcb *
              ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    __MEMSET( sctp_db_main.tx_mem_pool.max_pool, 0,
              init_options->MaxPerAssoc.max_tx_buffers *
              init_options->max_tcb *
              ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
    /* SR 1-954224 */
    /* initialise Rx Pool memory */
    __MEMSET( sctp_db_main.rx_mem_pool.min_pool, 0,
              init_options->MaxPerAssoc.min_rx_buffers *
              init_options->max_tcb *
              ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    __MEMSET( sctp_db_main.rx_mem_pool.med_pool, 0,
              init_options->MaxPerAssoc.med_rx_buffers *
              init_options->max_tcb *
              ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

    __MEMSET( sctp_db_main.rx_mem_pool.max_pool, 0,
              init_options->MaxPerAssoc.max_rx_buffers *
              init_options->max_tcb *
              ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );


    return( SCTP_SUCCESS );

}
/***************************************************************************
 *  FUNCTION :    sctp_db_alloc_buffer
 *
 *  DESCRIPTION:
 *    This is the first function called at the time of db module initialisation.
 *    It performs following functions.
 *
 *    1) Allocate memory for buffer structures of different queues.
 *
 *    2) On failure, it frees up the previously allocated memory
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_alloc_buffer(
    sctp_init_options_st     *init_options,
    sctp_error_t             *p_ecode )
{
    /* allocate buffer for reserve_buffer */
    /* SPR 20292 start */
    if( init_options->min_reserve_buffers != 0 )
    {
        sctp_db_main.reserve_buffer.min_buffer = ( sctpbuffer_st * )
                                                 __MALLOC( init_options->min_reserve_buffers * sizeof( sctpbuffer_st ) );

        if( SCTP_NULL == sctp_db_main.reserve_buffer.min_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );

            __FREE( sctp_recv_thr_buff );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    if( init_options->med_reserve_buffers != 0 )
    {
        sctp_db_main.reserve_buffer.med_buffer = ( sctpbuffer_st * )
                                                 __MALLOC( init_options->med_reserve_buffers * sizeof( sctpbuffer_st ) );

        if( SCTP_NULL == sctp_db_main.reserve_buffer.med_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );

            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    if( init_options->max_reserve_buffers != 0 )
    {
        sctp_db_main.reserve_buffer.max_buffer = ( sctpbuffer_st * )
                                                 __MALLOC( init_options->max_reserve_buffers * sizeof( sctpbuffer_st ) );

        if( SCTP_NULL == sctp_db_main.reserve_buffer.max_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );

            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /* allocate for rx_buffer */

    if( init_options->MaxPerAssoc.min_rx_buffers != 0 )
    {
        sctp_db_main.rx_buffer.min_buffer = ( sctpbuffer_st * )
                                            __MALLOC( init_options->MaxPerAssoc.min_rx_buffers *
                                                      sizeof( sctpbuffer_st ) * init_options->max_tcb );

        if( SCTP_NULL == sctp_db_main.rx_buffer.min_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            /*        __FREE(sctp_db_main.p_ulpq);
                    __FREE(sctp_db_main.p_data_ind);

            */
            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );

            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );
            __FREE( sctp_db_main.reserve_buffer.max_buffer );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    if( init_options->MaxPerAssoc.med_rx_buffers != 0 )
    {
        sctp_db_main.rx_buffer.med_buffer = ( sctpbuffer_st * )
                                            __MALLOC( init_options->MaxPerAssoc.med_rx_buffers *
                                                      sizeof( sctpbuffer_st ) * init_options->max_tcb );

        if( SCTP_NULL == sctp_db_main.rx_buffer.med_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );


            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );
            __FREE( sctp_db_main.reserve_buffer.max_buffer );
            __FREE( sctp_db_main.rx_buffer.min_buffer );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }

    }

    if( init_options->MaxPerAssoc.max_rx_buffers != 0 )
    {
        sctp_db_main.rx_buffer.max_buffer = ( sctpbuffer_st * )
                                            __MALLOC( init_options->MaxPerAssoc.max_rx_buffers *
                                                      sizeof( sctpbuffer_st ) * init_options->max_tcb );

        if( SCTP_NULL == sctp_db_main.rx_buffer.max_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );


            __FREE( sctp_recv_thr_buff );


            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );
            __FREE( sctp_db_main.reserve_buffer.max_buffer );

            __FREE( sctp_db_main.rx_buffer.min_buffer );
            __FREE( sctp_db_main.rx_buffer.med_buffer );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /* allocate for tx_buffer */

    if( init_options->MaxPerAssoc.min_tx_buffers != 0 )
    {
        sctp_db_main.tx_buffer.min_buffer = ( sctpbuffer_st * )
                                            __MALLOC( init_options->MaxPerAssoc.min_tx_buffers *
                                                      sizeof( sctpbuffer_st ) * init_options->max_tcb );

        if( SCTP_NULL == sctp_db_main.tx_buffer.min_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );


            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );
            __FREE( sctp_db_main.reserve_buffer.max_buffer );

            __FREE( sctp_db_main.rx_buffer.min_buffer );
            __FREE( sctp_db_main.rx_buffer.med_buffer );
            __FREE( sctp_db_main.rx_buffer.max_buffer );


            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    if( init_options->MaxPerAssoc.med_tx_buffers != 0 )
    {
        sctp_db_main.tx_buffer.med_buffer = ( sctpbuffer_st * )
                                            __MALLOC( init_options->MaxPerAssoc.med_tx_buffers *
                                                      sizeof( sctpbuffer_st ) * init_options->max_tcb );

        if( SCTP_NULL == sctp_db_main.tx_buffer.med_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );



            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );
            __FREE( sctp_db_main.reserve_buffer.max_buffer );

            __FREE( sctp_db_main.rx_buffer.min_buffer );
            __FREE( sctp_db_main.rx_buffer.med_buffer );
            __FREE( sctp_db_main.rx_buffer.max_buffer );

            __FREE( sctp_db_main.tx_buffer.min_buffer );

            *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }

    }

    if( init_options->MaxPerAssoc.max_tx_buffers != 0 )
    {
        sctp_db_main.tx_buffer.max_buffer = ( sctpbuffer_st * )
                                            __MALLOC( init_options->MaxPerAssoc.max_tx_buffers *
                                                      sizeof( sctpbuffer_st ) * init_options->max_tcb );


        if( SCTP_NULL == sctp_db_main.tx_buffer.max_buffer )
        {
            /* Free earlier allocated memory and return error */
            __FREE( sctp_db_main.ep_table );
            __FREE( sctp_db_main.assoc_table );

            __FREE( sctp_db_main.reserve_mem_pool.min_pool );
            __FREE( sctp_db_main.reserve_mem_pool.med_pool );
            __FREE( sctp_db_main.reserve_mem_pool.max_pool );

            __FREE( sctp_db_main.tx_mem_pool.min_pool );
            __FREE( sctp_db_main.tx_mem_pool.med_pool );
            __FREE( sctp_db_main.tx_mem_pool.max_pool );

            __FREE( sctp_db_main.rx_mem_pool.min_pool );
            __FREE( sctp_db_main.rx_mem_pool.med_pool );
            __FREE( sctp_db_main.rx_mem_pool.max_pool );

            __FREE( sctp_db_main.appl_list );
            __FREE( sctp_db_main.addr_list );


            __FREE( sctp_recv_thr_buff );

            __FREE( sctp_db_main.reserve_buffer.min_buffer );
            __FREE( sctp_db_main.reserve_buffer.med_buffer );
            __FREE( sctp_db_main.reserve_buffer.max_buffer );

            __FREE( sctp_db_main.rx_buffer.min_buffer );
            __FREE( sctp_db_main.rx_buffer.med_buffer );
            __FREE( sctp_db_main.rx_buffer.max_buffer );

            __FREE( sctp_db_main.tx_buffer.min_buffer );
            __FREE( sctp_db_main.tx_buffer.med_buffer );

            *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
            return( SCTP_FAILURE );
        }
    }

    /* SPR 20292 ends*/
    return( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :    sctp_init_db_module
 *
 *  DESCRIPTION:
 *    This is the first function called at the time of stack
 *    initialisation. It performs following functions.
 *
 *    1) Allocate memory for maximum number of associations and
 *    initialise this memory chunk.
 *
 *    2) Allocate memory for Transmit and Receive Buffer pool
 *    and arrange them in tx_bufferr_pool and  rx_buffer_pool respectively.
 *
 *    3) Initialise parameter max_appl and max_assoc maintained in
 *    sctp_db_main structure.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_init_db_module(
    sctp_init_options_st   *init_options,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit   counter;
    sctp_ep_st    *p_ep;
    sctp_tcb_st   *p_assoc;

    sctpbuffer_st *p_buf1;
    sctpbuffer_st *p_buf2;
    sctpbuffer_st *p_buf3;

    sctp_U8bit    *p_pool1;
    sctp_U8bit    *p_pool2;
    sctp_U8bit    *p_pool3;

#ifdef SCTP_UI_SOCK_API

    sctp_ulpqueue_node_st     *p_ulp;
    sctp_data_indication_st   *p_data_ind;

    sctp_select_st   *p_select;

#endif /* SCTP_UI_SOCK_API */

    SCTP_INIT_LOCK( sctp_db_main.lock );

    sctp_db_main.ep_table = ( sctp_ep_st * )
                            __MALLOC( init_options->max_endpoint * sizeof( sctp_ep_st ) );

    if( SCTP_NULL == sctp_db_main.ep_table )
    {
        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }

    sctp_db_main.assoc_table = ( sctp_tcb_st * )
                               __MALLOC( init_options->max_tcb * sizeof( sctp_tcb_st ) );

    if( SCTP_NULL == sctp_db_main.assoc_table )
    {
        __FREE( sctp_db_main.ep_table );

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }


#ifdef SCTP_UI_SOCK_API

    sctp_db_main.p_ulpq = ( sctp_ulpqueue_node_st * )
                          __MALLOC( init_options->max_tcb * SCTP_MAX_NTFY_PER_ASSOC
                                    * sizeof( sctp_ulpqueue_node_st ) );

    if( SCTP_NULL == sctp_db_main.p_ulpq )
    {
        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }

    sctp_db_main.p_data_ind = ( sctp_data_indication_st * )
                              __MALLOC( init_options->max_endpoint * SCTP_MAX_LISTEN_BACKLOG
                                        * sizeof( sctp_data_indication_st ) );

    if( SCTP_NULL == sctp_db_main.p_data_ind )
    {
        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );
        __FREE( sctp_db_main.p_ulpq );

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }

    sctp_db_main.p_select = ( sctp_select_st * )__MALLOC( SCTP_MAX_SIMUL_SELECT
                                                          * sizeof( sctp_select_st ) );
    p_select = sctp_db_main.p_select;

    if( SCTP_NULL == p_select )
    {
        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }

#endif /* SCTP_UI_SOCK_API */

    /* Allocate memory to be stored in buffer structures */
    if( SCTP_FAILURE == sctp_db_buffer_mem_alloc( init_options, p_ecode ) )
    {
#ifdef SCTP_UI_SOCK_API
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );
        __FREE( p_select );
#endif /* SCTP_UI_SOCK_API */

        return( SCTP_FAILURE ); /*Previously allocated memory has been freed*/
    }

    /*
     *  Allocate memory for global  buffers used by application and sctp receive
     *  thread
     */

    sctp_recv_thr_buff = ( sctp_U8bit * )__MALLOC( SCTP_MAX_SEND_BUFFERS *
                                                   sizeof( sctpmtubuf_st ) );

    if( SCTP_NULL == sctp_recv_thr_buff )
    {
        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );

#ifdef SCTP_UI_SOCK_API
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );

        __FREE( p_select );
#endif /* SCTP_UI_SOCK_API */

        __FREE( sctp_db_main.reserve_mem_pool.min_pool );
        __FREE( sctp_db_main.reserve_mem_pool.med_pool );
        __FREE( sctp_db_main.reserve_mem_pool.max_pool );

        __FREE( sctp_db_main.tx_mem_pool.min_pool );
        __FREE( sctp_db_main.tx_mem_pool.med_pool );
        __FREE( sctp_db_main.tx_mem_pool.max_pool );

        __FREE( sctp_db_main.rx_mem_pool.min_pool );
        __FREE( sctp_db_main.rx_mem_pool.med_pool );
        __FREE( sctp_db_main.rx_mem_pool.max_pool );

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ... */
        return( SCTP_FAILURE );
    }

    sctp_db_init_mtu_buffers( SCTP_MAX_SEND_BUFFERS );


    /* allocate memory for application structure  */
    sctp_db_main.appl_list = ( sctp_appl_info_st * )
                             __MALLOC( init_options->max_appl * sizeof( sctp_appl_info_st ) );

    if( SCTP_NULL == sctp_db_main.appl_list )
    {
        /* Free earlier allocated memory and return error */

        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );

#ifdef SCTP_UI_SOCK_API
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );

        __FREE( p_select );
#endif /* SCTP_UI_SOCK_API */

        __FREE( sctp_db_main.reserve_mem_pool.min_pool );
        __FREE( sctp_db_main.reserve_mem_pool.med_pool );
        __FREE( sctp_db_main.reserve_mem_pool.max_pool );

        __FREE( sctp_db_main.tx_mem_pool.min_pool );
        __FREE( sctp_db_main.tx_mem_pool.med_pool );
        __FREE( sctp_db_main.tx_mem_pool.max_pool );

        __FREE( sctp_db_main.rx_mem_pool.min_pool );
        __FREE( sctp_db_main.rx_mem_pool.med_pool );
        __FREE( sctp_db_main.rx_mem_pool.max_pool );
        __FREE( sctp_recv_thr_buff );

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ... */
        return( SCTP_FAILURE );
    }

    /* allocate memory for address structure */
    /* CSR 1-2465134 : init_options->no_of_local_addrs is replaced by
     * SCTP_MAX_TRANSPORT_ADDR */
    sctp_db_main.addr_list = ( sctp_sockaddr_st * )
                             __MALLOC( SCTP_MAX_TRANSPORT_ADDR * sizeof( sctp_sockaddr_st ) );

    if( SCTP_NULL == sctp_db_main.addr_list )
    {
        /* Free earlier allocated memory and return error */
        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );

#ifdef SCTP_UI_SOCK_API
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );

        __FREE( p_select );
#endif /* SCTP_UI_SOCK_API */

        __FREE( sctp_db_main.reserve_mem_pool.min_pool );
        __FREE( sctp_db_main.reserve_mem_pool.med_pool );
        __FREE( sctp_db_main.reserve_mem_pool.max_pool );

        __FREE( sctp_db_main.tx_mem_pool.min_pool );
        __FREE( sctp_db_main.tx_mem_pool.med_pool );
        __FREE( sctp_db_main.tx_mem_pool.max_pool );

        __FREE( sctp_db_main.rx_mem_pool.min_pool );
        __FREE( sctp_db_main.rx_mem_pool.med_pool );
        __FREE( sctp_db_main.rx_mem_pool.max_pool );

        __FREE( sctp_db_main.appl_list );
        __FREE( sctp_recv_thr_buff );

        *p_ecode  = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }

    sctp_db_main.verf_ass_table = ( sctp_tcb_verf_st * )
                                  __MALLOC( init_options->max_tcb * sizeof( sctp_tcb_verf_st ) );

    if( SCTP_NULL == sctp_db_main.verf_ass_table )
    {
        /* Free earlier allocated memory and return error */
        __FREE( sctp_db_main.ep_table );
        __FREE( sctp_db_main.assoc_table );

#ifdef SCTP_UI_SOCK_API
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );

        __FREE( p_select );
#endif /* SCTP_UI_SOCK_API */

        __FREE( sctp_db_main.reserve_mem_pool.min_pool );
        __FREE( sctp_db_main.reserve_mem_pool.med_pool );
        __FREE( sctp_db_main.reserve_mem_pool.max_pool );

        __FREE( sctp_db_main.tx_mem_pool.min_pool );
        __FREE( sctp_db_main.tx_mem_pool.med_pool );
        __FREE( sctp_db_main.tx_mem_pool.max_pool );

        __FREE( sctp_db_main.rx_mem_pool.min_pool );
        __FREE( sctp_db_main.rx_mem_pool.med_pool );
        __FREE( sctp_db_main.rx_mem_pool.max_pool );

        __FREE( sctp_db_main.appl_list );
        __FREE( sctp_recv_thr_buff );
        __FREE( sctp_db_main.addr_list );


        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_FAILURE );
    }


    /*Allocate the buffer structures*/
    if( sctp_db_alloc_buffer( init_options, p_ecode ) == SCTP_FAILURE )
    {
#ifdef SCTP_UI_SOCK_API
        __FREE( sctp_db_main.p_ulpq );
        __FREE( sctp_db_main.p_data_ind );
        __FREE( p_select );
#endif /* SCTP_UI_SOCK_API */
        __FREE( sctp_db_main.verf_ass_table );

        return( SCTP_FAILURE ); /*Previously allocated memory has been freed*/
    }


    /* initialise ep table structure */
    __MEMSET( sctp_db_main.ep_table, 0,
              init_options->max_endpoint * sizeof( sctp_ep_st ) );

    /* initialise association table structure */
    __MEMSET( sctp_db_main.assoc_table, 0,
              init_options->max_tcb * sizeof( sctp_tcb_st ) );

    /* initialise verf association table structure */
    __MEMSET( sctp_db_main.verf_ass_table, 0,
              init_options->max_tcb * sizeof( sctp_tcb_verf_st ) );
#ifdef SCTP_UI_SOCK_API

    /* initialise ep table structure */
    __MEMSET( sctp_db_main.p_ulpq, 0,
              init_options->max_tcb * SCTP_MAX_NTFY_PER_ASSOC
              * sizeof( sctp_ulpqueue_node_st ) );

    /* initialise ep table structure */
    __MEMSET( sctp_db_main.p_data_ind, 0,
              init_options->max_endpoint * SCTP_MAX_LISTEN_BACKLOG
              * sizeof( sctp_data_indication_st ) );

    lstInit( &select_db.Select_list );
    lstInit( &select_db.Select_free_list );
    SCTP_INIT_LOCK( select_db.Select_list_lock );

    for( counter = 0; counter < SCTP_MAX_SIMUL_SELECT; counter++ )
    {
        p_select[counter].p_rfds = SCTP_NULL;
        p_select[counter].p_wfds = SCTP_NULL;
        p_select[counter].p_efds = SCTP_NULL;
        p_select[counter].p_timeout = SCTP_NULL;

        SCTP_FD_ZERO( &p_select[counter].n_rfds );
        SCTP_FD_ZERO( &p_select[counter].n_wfds );
        SCTP_FD_ZERO( &p_select[counter].n_efds );

        p_select[counter].nfds = 0;
        p_select[counter].timer_id = SCTP_NULL_TIMER_ID;

        SCTP_INIT_LOCK( p_select[counter].lock );
        SCTP_COND_INIT( p_select[counter].cond );

        lstAdd( &select_db.Select_free_list, ( NODE * )&p_select[counter] );

    }

    /* Initalise the ulpq_freelist */
    lstInit( &sctp_db_main.ulpq_freelist );
    p_ulp = ( sctp_ulpqueue_node_st * )( sctp_db_main.p_ulpq );

    for( counter = 0;
            counter < ( init_options->max_tcb * SCTP_MAX_NTFY_PER_ASSOC ); counter++ )
    {
        lstAdd( &sctp_db_main.ulpq_freelist, ( NODE * )( &p_ulp[counter] ) );
    }

    /* Initalise the data_ind_freelist */
    lstInit( &sctp_db_main.data_ind_freelist );
    p_data_ind = ( sctp_data_indication_st * )( sctp_db_main.p_data_ind );

    for( counter = 0; counter < ( init_options->max_tcb ); counter++ )
    {
        lstAdd( &sctp_db_main.data_ind_freelist, ( NODE * )( &p_data_ind[counter] ) );
    }

#endif  /* SCTP_UI_SOCK_API */

    /* initialise application structure */
    __MEMSET( sctp_db_main.appl_list, 0,
              init_options->max_appl * sizeof( sctp_appl_info_st ) );

    __MEMCOPY( sctp_db_main.addr_list, init_options->list_of_local_addrs,
               init_options->no_of_local_addrs * sizeof( sctp_sockaddr_st ) );

    /* Initialise the default protocol parameters */
    sctp_db_init_prot_params( ( sctp_U16bit )init_options->MaxPerAssoc.streams );
    sctp_db_main.sctp_def_prot_params.nat_capable =
        init_options->capability.nat_capable;

    sctp_db_main.sctp_def_prot_params.pmtu_capable =
        init_options->capability.pmtu_capable;

    sctp_db_main.sctp_def_prot_params.checksum_func =
        init_options->checksum;


    /* initialise max ep, max application and max association parameters */
    sctp_db_main.max_ep = init_options->max_endpoint;
    sctp_db_main.max_appl = init_options->max_appl;
    sctp_db_main.max_assoc = init_options->max_tcb;

    sctp_db_main.min_tx_buffers = init_options->MaxPerAssoc.min_tx_buffers;
    sctp_db_main.med_tx_buffers = init_options->MaxPerAssoc.med_tx_buffers;
    sctp_db_main.max_tx_buffers = init_options->MaxPerAssoc.max_tx_buffers;
    sctp_db_main.min_rx_buffers = init_options->MaxPerAssoc.min_rx_buffers;
    sctp_db_main.med_rx_buffers = init_options->MaxPerAssoc.med_rx_buffers;
    sctp_db_main.max_rx_buffers = init_options->MaxPerAssoc.max_rx_buffers;
    sctp_db_main.min_reserve_buffers = init_options->min_reserve_buffers;
    sctp_db_main.med_reserve_buffers = init_options->med_reserve_buffers;
    sctp_db_main.max_reserve_buffers = init_options->max_reserve_buffers;

    sctp_db_main.max_streams_per_assoc = init_options->MaxPerAssoc.streams;

    sctp_db_main.num_active_appl = 0;
    /* sctp_db_main.num_active_assoc = 0; */
    sctp_db_main.num_active_ep = 0;
    sctp_db_main.last_allocated_ep = 0;

    sctp_db_main.num_local_addrs = init_options->no_of_local_addrs;

    /* Initalise the ep_list */
    lstInit( &sctp_db_main.ep_list );
    p_ep = ( sctp_ep_st * )( sctp_db_main.ep_table );

    for( counter = 0; counter < ( init_options->max_endpoint ); counter++ )
    {
        lstAdd( &sctp_db_main.ep_list, ( NODE * )( &p_ep[counter] ) );
        /* Initilize the lock and Condition variaable */
        SCTP_INIT_LOCK( p_ep[counter].lock );
        SCTP_COND_INIT( p_ep[counter].cond );
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        SCTP_COND_INIT( p_ep[counter].asconf_cond );
#endif
    }

    /* Initalise the assoc_list */
    lstInit( &sctp_db_main.assoc_freelist );
    p_assoc = ( sctp_tcb_st * )( sctp_db_main.assoc_table );

    for( counter = 0; counter < ( init_options->max_tcb ); counter++ )
    {
        lstAdd( &sctp_db_main.assoc_freelist, ( NODE * )( &p_assoc[counter] ) );
    }

    for( counter = 0; counter < SCTP_MAX_VERIFICATION_NODE; counter++ )
    {
        lstInit( &sctp_db_main.ass_hash[counter].chain );
        SCTP_INIT_LOCK( sctp_db_main.ass_hash[counter].lock );
    }

    for( counter = 0; counter < SCTP_MAX_BHTABLE_SIZE; counter++ )
    {
        lstInit( &sctp_db_main.bhash[counter].chain );
        SCTP_INIT_LOCK( sctp_db_main.bhash[counter].lock );
    }

    for( counter = 0; counter < SCTP_MAX_LHTABLE_SIZE; counter++ )
    {
        lstInit( &sctp_db_main.lhash[counter].chain );
        SCTP_INIT_LOCK( sctp_db_main.lhash[counter].lock );
    }

    /* Initalise reserve_buffer list */
    lstInit( &sctp_db_main.reserve_list.list_min );
    lstInit( &sctp_db_main.reserve_list.list_med );
    lstInit( &sctp_db_main.reserve_list.list_max );

    p_buf1 = ( sctpbuffer_st * )( sctp_db_main.reserve_buffer.min_buffer );
    p_buf2 = ( sctpbuffer_st * )( sctp_db_main.reserve_buffer.med_buffer );
    p_buf3 = ( sctpbuffer_st * )( sctp_db_main.reserve_buffer.max_buffer );

    p_pool1 = ( sctp_U8bit * )( sctp_db_main.reserve_mem_pool.min_pool );
    p_pool2 = ( sctp_U8bit * )( sctp_db_main.reserve_mem_pool.med_pool );
    p_pool3 = ( sctp_U8bit * )( sctp_db_main.reserve_mem_pool.max_pool );

    /* SR 1-954224 */
    for( counter = 0; counter < init_options->min_reserve_buffers; counter++ )
    {
        p_buf1[counter].id     = SCTP_MIN_FRAGSIZE;
        p_buf1[counter].buffer = p_pool1 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.reserve_list.list_min, ( NODE * )( &p_buf1[counter] ) );
    }

    for( counter = 0; counter < init_options->med_reserve_buffers; counter++ )
    {
        p_buf2[counter].id     = SCTP_MED_FRAGSIZE;
        p_buf2[counter].buffer = p_pool2 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.reserve_list.list_med, ( NODE * )( &p_buf2[counter] ) );
    }

    for( counter = 0; counter < init_options->max_reserve_buffers; counter++ )
    {
        p_buf3[counter].id     = SCTP_MAX_FRAGSIZE;
        p_buf3[counter].buffer = p_pool3 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );

        lstAdd( &sctp_db_main.reserve_list.list_max, ( NODE * )( &p_buf3[counter] ) );
    }

    /* SR 1-954224 */

    /* Initalise tx_buffer list */
    lstInit( &sctp_db_main.tx_list.list_min );
    lstInit( &sctp_db_main.tx_list.list_med );
    lstInit( &sctp_db_main.tx_list.list_max );

    p_buf1 = ( sctpbuffer_st * )( sctp_db_main.tx_buffer.min_buffer );
    p_buf2 = ( sctpbuffer_st * )( sctp_db_main.tx_buffer.med_buffer );
    p_buf3 = ( sctpbuffer_st * )( sctp_db_main.tx_buffer.max_buffer );

    p_pool1 = ( sctp_U8bit * )( sctp_db_main.tx_mem_pool.min_pool );
    p_pool2 = ( sctp_U8bit * )( sctp_db_main.tx_mem_pool.med_pool );
    p_pool3 = ( sctp_U8bit * )( sctp_db_main.tx_mem_pool.max_pool );

    for( counter = 0;
            counter < ( init_options->MaxPerAssoc.min_tx_buffers *
                        sctp_db_main.max_assoc );
            counter++ )
    {
        p_buf1[counter].id     = SCTP_MIN_FRAGSIZE;
        p_buf1[counter].buffer = p_pool1 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.tx_list.list_min, ( NODE * )( &p_buf1[counter] ) );
    }

    for( counter = 0;
            counter < ( init_options->MaxPerAssoc.med_tx_buffers *
                        sctp_db_main.max_assoc );
            counter++ )
    {
        p_buf2[counter].id     = SCTP_MED_FRAGSIZE;
        p_buf2[counter].buffer = p_pool2 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.tx_list.list_med, ( NODE * )( &p_buf2[counter] ) );
    }

    for( counter = 0;
            counter < ( init_options->MaxPerAssoc.max_tx_buffers *
                        sctp_db_main.max_assoc );
            counter++ )
    {
        p_buf3[counter].id     = SCTP_MAX_FRAGSIZE;
        p_buf3[counter].buffer = p_pool3 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MAX_FRAGSIZE  + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.tx_list.list_max, ( NODE * )( &p_buf3[counter] ) );
    }

    /* Initalise rx_buffer list */
    lstInit( &sctp_db_main.rx_list.list_min );
    lstInit( &sctp_db_main.rx_list.list_med );
    lstInit( &sctp_db_main.rx_list.list_max );

    p_buf1 = ( sctpbuffer_st * )( sctp_db_main.rx_buffer.min_buffer );
    p_buf2 = ( sctpbuffer_st * )( sctp_db_main.rx_buffer.med_buffer );
    p_buf3 = ( sctpbuffer_st * )( sctp_db_main.rx_buffer.max_buffer );

    p_pool1 = ( sctp_U8bit * )( sctp_db_main.rx_mem_pool.min_pool );
    p_pool2 = ( sctp_U8bit * )( sctp_db_main.rx_mem_pool.med_pool );
    p_pool3 = ( sctp_U8bit * )( sctp_db_main.rx_mem_pool.max_pool );

    for( counter = 0;
            counter < ( init_options->MaxPerAssoc.min_rx_buffers *
                        sctp_db_main.max_assoc );
            counter++ )
    {
        p_buf1[counter].id     = SCTP_MIN_FRAGSIZE;
        p_buf1[counter].buffer = p_pool1 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MIN_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.rx_list.list_min, ( NODE * )( &p_buf1[counter] ) );
    }

    for( counter = 0;
            counter < ( init_options->MaxPerAssoc.med_rx_buffers *
                        sctp_db_main.max_assoc );
            counter++ )
    {
        p_buf2[counter].id     = SCTP_MED_FRAGSIZE;
        p_buf2[counter].buffer = p_pool2 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MED_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.rx_list.list_med, ( NODE * )( &p_buf2[counter] ) );
    }

    for( counter = 0;
            counter < ( init_options->MaxPerAssoc.max_rx_buffers *
                        sctp_db_main.max_assoc );
            counter++ )
    {
        p_buf3[counter].id     = SCTP_MAX_FRAGSIZE;
        p_buf3[counter].buffer = p_pool3 + SCTPCHUNKOFFSET_PAYLOAD +
                                 ( counter * ( SCTP_MAX_FRAGSIZE + SCTPCHUNKOFFSET_PAYLOAD ) );
        lstAdd( &sctp_db_main.rx_list.list_max, ( NODE * )( &p_buf3[counter] ) );
    }


    /* clean up all associations */
    for( counter = 0; counter < init_options->max_tcb; counter++ )
    {
        /* Initialise association queues */
        sctp_db_init_assoc_queues( &sctp_db_main.assoc_table[counter] );

        /* initialise association structure with default values */
        sctp_db_clean_association( &sctp_db_main.assoc_table[counter],
                                   SCTP_FALSE );
    }

    if( SCTP_NULL != init_options->hostname )
    {
        sctp_U32bit sctp_index;

        for( sctp_index = 0;
                init_options->hostname[sctp_index] != '\0' && sctp_index < ( SCTP_MAX_HOSTNAME_SIZE - 1 );
                sctp_index++ )
        {
            sctp_db_main.hostname[sctp_index] = init_options->hostname[sctp_index];
        }

        sctp_db_main.hostname[sctp_index] = '\0';
    }

    return( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_deinit_db_module
 *
 *  DESCRIPTION:
 *    This is the  function called at the time of stack
 *    deinitialisation. It performs following functions.
 *
 *    1) DeAllocate memory allocated for  associations, Tx Rx buffers
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_deinit_db_module( void )
{
    sctp_U32bit         counter = 0;
    sctp_ep_st          *p_ep;

    /*Fix Bug-Id 3 starts*/
    SCTP_DESTROY_LOCK( sctp_db_main.lock );

    SCTP_DESTROY_LOCK( mtu_list_lock );
    /*Fix Bug-Id 3 ends*/

    for( counter = 0; counter < SCTP_MAX_BHTABLE_SIZE; counter++ )
    {
        SCTP_DESTROY_LOCK( sctp_db_main.bhash[counter].lock );
    }

    for( counter = 0; counter < SCTP_MAX_LHTABLE_SIZE; counter++ )
    {
        lstInit( &sctp_db_main.lhash[counter].chain );
        SCTP_DESTROY_LOCK( sctp_db_main.lhash[counter].lock );
    }

    for( counter = 0; counter < SCTP_MAX_VERIFICATION_NODE; counter++ )
    {
        lstInit( &sctp_db_main.ass_hash[counter].chain );
        SCTP_DESTROY_LOCK( sctp_db_main.ass_hash[counter].lock );
    }

    p_ep = ( sctp_ep_st * )( sctp_db_main.ep_table );

    for( counter = 0; counter < ( sctp_db_main.max_ep ); counter++ )
    {
        lstAdd( &sctp_db_main.ep_list, ( NODE * )( &p_ep[counter] ) );
        /* Initilize the lock and Condition variaable */
        SCTP_DESTROY_LOCK( p_ep[counter].lock );
        SCTP_COND_DESTROY( p_ep[counter].cond );
#ifdef SCTP_DYNAMIC_IP_SUPPORT
        SCTP_COND_DESTROY( p_ep[counter].asconf_cond );
#endif
    }

#ifdef SCTP_UI_SOCK_API

    SCTP_DESTROY_LOCK( select_db.Select_list_lock );

    for( counter = 0; counter < SCTP_MAX_SIMUL_SELECT; counter++ )
    {
        SCTP_DESTROY_LOCK( sctp_db_main.p_select[counter].lock );
        SCTP_COND_DESTROY( sctp_db_main.p_select[counter].cond );
    }

    __FREE( sctp_db_main.p_ulpq );
    __FREE( sctp_db_main.p_data_ind );
    __FREE( sctp_db_main.p_select );
#endif /* SCTP_UI_SOCK_API */


    __FREE( sctp_db_main.ep_table );
    __FREE( sctp_db_main.assoc_table );

    __FREE( sctp_db_main.tx_mem_pool.min_pool );
    __FREE( sctp_db_main.tx_mem_pool.med_pool );
    __FREE( sctp_db_main.tx_mem_pool.max_pool );

    __FREE( sctp_db_main.rx_mem_pool.min_pool );
    __FREE( sctp_db_main.rx_mem_pool.med_pool );
    __FREE( sctp_db_main.rx_mem_pool.max_pool );

    __FREE( sctp_db_main.appl_list );
    __FREE( sctp_db_main.addr_list );

    __FREE( sctp_db_main.rx_buffer.min_buffer );
    __FREE( sctp_db_main.rx_buffer.med_buffer );
    __FREE( sctp_db_main.rx_buffer.max_buffer );

    __FREE( sctp_db_main.tx_buffer.min_buffer );
    __FREE( sctp_db_main.tx_buffer.med_buffer );
    __FREE( sctp_db_main.tx_buffer.max_buffer );

    __FREE( sctp_recv_thr_buff );

    __FREE( sctp_db_main.reserve_buffer.min_buffer );
    __FREE( sctp_db_main.reserve_buffer.med_buffer );
    __FREE( sctp_db_main.reserve_buffer.max_buffer );

    __FREE( sctp_db_main.reserve_mem_pool.min_pool );
    __FREE( sctp_db_main.reserve_mem_pool.med_pool );
    __FREE( sctp_db_main.reserve_mem_pool.max_pool );
    __FREE( sctp_db_main.verf_ass_table );
    return ( SCTP_SUCCESS );

}
#ifdef SCTP_UI_SOCK_API


/***************************************************************************
 *  FUNCTION :    sctp_db_add_data_ind
 *
 *  DESCRIPTION:
 *     This functions adds n indications to the given assoc. and if the node
 *   for that assoc is not present adds a new node for that assoc.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_add_data_ind(
    sctp_tcb_st     *p_tcb,
    sctp_U32bit     n_ind )
{
    sctp_data_indication_st   *p_curr = SCTP_NULL;
    sctp_data_indication_st   *p_prev = SCTP_NULL;

    SCTPLOCK();

    p_curr = ( sctp_data_indication_st * )
             lstFirst( &( p_tcb->p_ep->ulpqueue.data_ind_list ) );

    for( ; p_curr != SCTP_NULL;
            p_prev = p_curr,
            p_curr = ( sctp_data_indication_st * )lstNext( ( NODE * )p_curr ) )
    {
        /* find the entry for the assoc */
        if( p_curr->p_assoc == p_tcb )
        {
            /* update the parameter */
            p_curr->n_ind += n_ind;

            SCTPUNLOCK();

            return ( SCTP_SUCCESS );
        }
    }

    /* entry for this tcb is not present so add a new node */
    /* first get a new node from the freelist */
    p_curr = ( sctp_data_indication_st * )
             lstFirst( &( sctp_db_main.data_ind_freelist ) );

    if( !p_curr )
    {
        SCTPUNLOCK();

        return ( SCTP_FAILURE );
    }

    lstDelete( &( sctp_db_main.data_ind_freelist ), ( NODE * )p_curr );

    /* set the parameters value */
    p_curr->p_assoc = p_tcb;
    p_curr->n_ind   = n_ind;

    /* Add this node to the list */
    lstInsert( &( p_tcb->p_ep->ulpqueue.data_ind_list ),
               ( NODE * )p_prev, ( NODE * )p_curr );

    SCTPUNLOCK();

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_clear_data_ind
 *
 *  DESCRIPTION:
 *      This functions clears n indications for the given assoc and if 0 is
 *    passed in n_ind then clears all indications and remove that node.
 *
 *  RETURN VALUE:
 *     SCTP_SUCCESS
 *     SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_db_clear_data_ind(
    sctp_tcb_st      *p_tcb,
    sctp_U32bit      n_ind )
{
    sctp_data_indication_st  *p_curr = SCTP_NULL;

    p_curr = ( sctp_data_indication_st * )
             lstFirst( &( p_tcb->p_ep->ulpqueue.data_ind_list ) );

    for( ; p_curr != SCTP_NULL;
            p_curr = ( sctp_data_indication_st * )lstNext( ( NODE * )p_curr ) )
    {
        if( p_curr->p_assoc == p_tcb )
        {
            if( ( 0 != n_ind ) && ( p_curr->n_ind > n_ind ) )
            {
                p_curr->n_ind -= n_ind;
            }

            else
            {
                p_curr->n_ind = 0;
            }

            if( 0 == p_curr->n_ind )
            {
                p_curr->n_ind = 0;
                p_curr->p_assoc = SCTP_NULL;
                lstDelete( &p_tcb->p_ep->ulpqueue.data_ind_list, ( NODE * )p_curr );
                lstAdd( &( sctp_db_main.data_ind_freelist ), ( NODE * )p_curr );
            }

            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( " %u Data Indications Cleared Successfully ", n_ind ) );

            return ( SCTP_SUCCESS );
        }
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "InValid Association " ) );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_check_data_ind
 *
 *  DESCRIPTION:
 *     This function checks for the data indicatons at the given assoc
 *
 *  RETURN VALUE:
 *     SCTP_SUCCESS there is one or more data indications
 *     SCTP_FAILURE there is no data indications on assoc
 *
 ****************************************************************************/
sctp_return_t
sctp_db_check_data_ind(
    sctp_tcb_st      *p_tcb )
{

    sctp_data_indication_st   *p_curr;

    p_curr = ( sctp_data_indication_st * )
             lstFirst( &( p_tcb->p_ep->ulpqueue.data_ind_list ) );

    for( ; p_curr != SCTP_NULL;
            p_curr = ( sctp_data_indication_st * )lstNext( ( NODE * )p_curr ) )
    {
        if( p_curr->p_assoc == p_tcb )
        {
            return ( SCTP_SUCCESS );
        }
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_db_get_ntfy_node
 *
 *  DESCRIPTION:
 *      This function get a free node from the free list.
 *
 *  RETURN VALUE:
 *     pointer to the node  in case of success
 *     SCTP_NULL  in case of failure
 *
 ****************************************************************************/
sctp_ulpqueue_node_st *
sctp_db_get_ntfy_node()
{
    sctp_ulpqueue_node_st  *p_node;

    SCTPLOCK();

    p_node = ( sctp_ulpqueue_node_st * )lstGet( &( sctp_db_main.ulpq_freelist ) );

    if( SCTP_NULL != p_node )
    {
        SCTPUNLOCK();
        return( p_node );
    }

    SCTPUNLOCK();

    return ( SCTP_NULL );
}

/***************************************************************************
 *  FUNCTION :    sctp_db_free_ntfy_node
 *
 *  DESCRIPTION:
 *      This function delete a node from the given list and add it to
 *      free list.
 *
 *  RETURN VALUE:
 *      None
 ****************************************************************************/
void
sctp_db_free_ntfy_node(
    sctp_ulpqueue_node_st *p_node )
{
    SCTPLOCK();

    lstAdd( &( sctp_db_main.ulpq_freelist ), ( NODE * )p_node );

    SCTPUNLOCK();
}
/***************************************************************************
 *  FUNCTION :    sctp_db_ulpqueue_add_ntfy
 *
 *  DESCRIPTION:
 *      This function add a node to the given ulpqueue list
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FALILURE
 ****************************************************************************/
sctp_return_t
sctp_db_ulpqueue_add_ntfy(
    sctp_ulpqueue_st       *p_ulpq,
    sctp_ulpqueue_node_st  *p_ntfy )
{
    sctp_ulpqueue_node_st  *p_prev = SCTP_NULL;
    p_prev = ( sctp_ulpqueue_node_st * )lstLast( &p_ulpq->ntfy_queue );
    lstInsert( &p_ulpq->ntfy_queue, ( NODE * )p_prev, ( NODE * )p_ntfy );
    p_ulpq->n_ind++;
    return SCTP_SUCCESS;
}

/***************************************************************************
 *  FUNCTION :  sctp_db_ulpqueue_rem_ntfy
 *
 *  DESCRIPTION:
 *      This function removes the fisrt node from the given ulpqueue list
 *   and return the pointer to that node.
 *
 *  RETURN VALUE: Pointer to the node
 *
 ****************************************************************************/
sctp_ulpqueue_node_st *
sctp_db_ulpqueue_rem_ntfy(
    sctp_ulpqueue_st   *p_ulpq )
{
    sctp_ulpqueue_node_st  *p_ntfy = SCTP_NULL;
    p_ntfy = ( sctp_ulpqueue_node_st * )lstFirst( &p_ulpq->ntfy_queue );
    lstDelete( &p_ulpq->ntfy_queue, ( NODE * )p_ntfy );
    p_ulpq->n_ind--;
    return( p_ntfy );
}

/***************************************************************************
 *  FUNCTION :  sctp_db_ulpqueue_empty
 *
 *  DESCRIPTION:
 *      This function removes all the nodes from a given ulpqueue list and add
 *   them to the global freelist
 *
 *
 *  RETURN VALUE:
 *      None
 ****************************************************************************/
void
sctp_db_ulpqueue_empty(
    sctp_ulpqueue_st   *p_ulpq )
{
    sctp_ulpqueue_node_st  *p_ntfy = SCTP_NULL;

    while( p_ulpq->n_ind > 0 )
    {
        p_ntfy = sctp_db_ulpqueue_rem_ntfy( p_ulpq );
        sctp_db_free_ntfy_node( p_ntfy );
    }

}

/***************************************************************************
 *  FUNCTION :    sctp_db_ulpqueue_copy
 *
 *  DESCRIPTION:
 *      This extracts all the ntfy nodes related to a given assoc id from the
 *   given source ulpqueue list and add them to the new destination ulpqueue
 *
 *
 *  RETURN VALUE:
 *       None
 ****************************************************************************/
void
sctp_db_ulpqueue_copy(
    sctp_U32bit        assoc_id,
    sctp_ulpqueue_st   *p_srcq,
    sctp_ulpqueue_st   *p_dstq )
{
    sctp_ulpqueue_node_st  *p_ntfy = SCTP_NULL;
    sctp_ulpqueue_node_st  *p_temp = SCTP_NULL;

    p_ntfy = ( sctp_ulpqueue_node_st * )lstFirst( &p_srcq->ntfy_queue );

    while( SCTP_NULL != p_ntfy )
    {
        if( p_ntfy->assoc_id == assoc_id )
        {
            p_temp = ( sctp_ulpqueue_node_st * )lstPrevious( ( NODE * )p_ntfy );
            lstDelete( &p_srcq->ntfy_queue, ( NODE * )p_ntfy );
            p_srcq->n_ind--;
            sctp_db_ulpqueue_add_ntfy( p_dstq, p_ntfy );

            if( p_temp == SCTP_NULL )
            {
                p_ntfy = ( sctp_ulpqueue_node_st * )lstFirst( &p_srcq->ntfy_queue );
            }

            else
            {
                p_ntfy = ( sctp_ulpqueue_node_st * )lstNext( ( NODE * )p_temp );
            }
        }

        else
        {
            p_ntfy = ( sctp_ulpqueue_node_st * )lstNext( ( NODE * )p_ntfy );
        }
    }
}



#endif  /* SCTP_UI_SOCK_API */
/* CSR 69326 fix starts*/
/***************************************************************************
 *  FUNCTION :    sigtran_is_addr_any
 *
 *  DESCRIPTION:
 *                This function will check for any stack address
 *
 *
 *  RETURN VALUE:
 *
 ****************************************************************************/
sigtran_U32bit sigtran_is_addr_any( sctp_sockaddr_st sock_addr )
{
    if( SCTP_AF_INET == sock_addr.family )
    {
        if( !( sock_addr.ip.v4.addr ) )
        {
            return SCTP_TRUE;
        }

        else
        {
            return SCTP_FALSE;
        }
    }

    else
    {
        if( ( !( sock_addr.ip.v6.addr.s_u6_addr32[0] ) ) && ( !( sock_addr.ip.v6.addr.s_u6_addr32[1] ) ) && ( !( sock_addr.ip.v6.addr.s_u6_addr32[2] ) ) && ( !( sock_addr.ip.v6.addr.s_u6_addr32[3] ) ) )
        {
            return SCTP_TRUE;
        }

        else
        {
            return SCTP_FALSE;
        }

    }
}

/* CSR 69326 fix ends*/


