/******************************************************************************
 *    FILE NAME:
 *      s_dbbuf.c
 *
 *    DESCRIPTION:
 *      This file is part of Database Module ofSCTP. This has routines
 *      that manipulates Trannsmit and receive buffers.
 *
 *      Transmit (TxB) and Receive (RxB) Buffer    Organisation
 *      ------------------------------------------------------
 *      Parameter max_tx_buffers and max_rx_buffers are passed to database
 *      module at the time of module initialisation (As were passed by System
 *      management to stack in API sctp_api_init_stack).Database module
 *      allocates memory for these many transmit and receive buffers,
 *      Size of transmit and receive buffer is a hash define and decided
 *      at compile time. Arrange these buffers in Global transmit and receive
 *      freepools respectively.These freepools are maintained as linked-list
 *      and are called Global Tx-Buffer Table and Global Rx-Buffer Table.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -           .Original Creation
 *    25Sept 2001     Yogesh Gahlaut   -           Rel 3.0 Changes
 *      Oct  2002     Manish Rajpal   -             Runtime buffers resize
 *      02Nov04       Svakumar      REL 6.0         SCTP/FUNC-CONGCON-5
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#define SCTP_MODULE "DB"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_db.h>
#include <s_dbtcb.h>
#include <s_dbg.h>
#include <s_dbbuf.h>
#include <s_osmem.h>
#include <s_stats.h>

/***************************************************************************
 *    FUNCTION :        sctp_get_receive_buffer
 *
 *    DESCRIPTION:
 *        Get a buffer from Rx-freelist.
 *
 *    RETURN VALUE:
 *        sctpbuffer_st *
 ***************************************************************************/
sctpbuffer_st *
sctp_get_receive_buffer(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    size )
{

    sctpbuffer_st *p_buf;

    if( size <= SCTP_MIN_FRAGSIZE )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_min );

        if( p_buf != SCTP_NULL )
        {
            lstDelete( &p_ass->rx_freelist.list_min, ( NODE * )p_buf );
            return p_buf;
        }
    }

    if( size <= SCTP_MED_FRAGSIZE )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_med );

        if( p_buf != SCTP_NULL )
        {
            lstDelete( &p_ass->rx_freelist.list_med, ( NODE * )p_buf );
#ifdef SCTP_STATS_ENABLED

            if( size <= SCTP_MIN_FRAGSIZE )
            {
                SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_R_MED_INSTD_MIN, 1 );
            }

#endif
            return p_buf;
        }
    }

    if( size <= SCTP_MAX_FRAGSIZE )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->rx_freelist.list_max );

        if( p_buf != SCTP_NULL )
        {
#ifdef SCTP_STATS_ENABLED

            if( size <= SCTP_MIN_FRAGSIZE )
            {
                SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_R_MAX_INSTD_MIN, 1 );
            }

            else if( size <= SCTP_MED_FRAGSIZE )
            {
                SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_R_MAX_INSTD_MED, 1 );
            }

#endif
            lstDelete( &p_ass->rx_freelist.list_max, ( NODE * )p_buf );
            return p_buf;
        }
    }

    return NULL;

}

/**************************************************************************
 *    FUNCTION :   sctp_free_receive_buffer
 *
 *    DESCRIPTION:
 *        This function returns buffer passed to the Rx-freelist per
 *        association
 *
 *    RETURN VALUE:
 *        None
 ***************************************************************************/
void
sctp_free_receive_buffer(
    sctp_tcb_st      *p_ass,
    sctpbuffer_st    *p_buf )
{

    if( p_buf == SCTP_NULL )
    {
        return;
    }

    switch( p_buf->id )
    {
        case SCTP_MIN_FRAGSIZE:
            {
                lstAdd( &p_ass->rx_freelist.list_min, ( NODE * )p_buf );
                break;
            }

        case SCTP_MED_FRAGSIZE:
            {
                lstAdd( &p_ass->rx_freelist.list_med, ( NODE * )p_buf );
                break;
            }

        case SCTP_MAX_FRAGSIZE:
            {
                lstAdd( &p_ass->rx_freelist.list_max, ( NODE * )p_buf );
                break;
            }

        default:
            {
                break;
            }

    }

    return;
}


/**************************************************************************
 *    FUNCTION :    sctp_user_free_receive_buffer
 *
 *    DESCRIPTION:
 *        This function returns buffer passed to the Rx-freelist.
 *
 *    RETURN VALUE:
 *        None
 ***************************************************************************/
void
sctp_user_free_receive_buffer(
    sctp_tcb_st          *p_ass,
    sctpbuffer_st        *p_buf )

{
    sctp_buffer_list_st  *p_free_list;

    if( p_ass )
    {
        sctp_free_receive_buffer( p_ass, p_buf );
        return;
    }

    if( p_buf == SCTP_NULL )
    {
        return;
    }


    p_free_list = &( sctp_db_main.rx_list );

    switch( p_buf->id )
    {
        case SCTP_MIN_FRAGSIZE:
            {
                lstAdd( &( p_free_list->list_min ), ( NODE * )p_buf );
                break;
            }

        case SCTP_MED_FRAGSIZE:
            {
                lstAdd( &( p_free_list->list_med ), ( NODE * )p_buf );
                break;
            }

        case SCTP_MAX_FRAGSIZE:
            {
                lstAdd( &( p_free_list->list_max ), ( NODE * )p_buf );
                break;
            }

        default:
            break;
    }
}


/**************************************************************************
 *    FUNCTION :   sctp_get_transmit_buffer
 *
 *    DESCRIPTION:
 *        Get a buffer from Tx-freelist.
 *
 *    RETURN VALUE:
 *        sctobuffer_t *
 ***************************************************************************/
sctpbuffer_st *
sctp_get_transmit_buffer(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    size )
{

    sctpbuffer_st *p_buf = SCTP_NULL;

    if( size <= SCTP_MIN_FRAGSIZE )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_min );

        if( p_buf != SCTP_NULL )
        {
            lstDelete( &p_ass->tx_freelist.list_min, ( NODE * )p_buf );
            p_buf->ref_count = 1;

            /* SPR 20573 Starts for CSR 1-7088501 */
            p_buf->retrans_flag = SCTP_RETRANSFLAG_NONE;
            p_buf->sent = 0;
            p_buf->dest_addr_index = 0;
            /* SPR 20573 Ends for CSR 1-7088501 */

            return p_buf;
        }
    }

    if( size <= SCTP_MED_FRAGSIZE )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_med );

        if( p_buf != SCTP_NULL )
        {
            lstDelete( &p_ass->tx_freelist.list_med, ( NODE * )p_buf );
            p_buf->ref_count = 1;

            /* SPR 20573 Starts for CSR 1-7088501 */
            p_buf->retrans_flag = SCTP_RETRANSFLAG_NONE;
            p_buf->sent = 0;
            p_buf->dest_addr_index = 0;
            /* SPR 20573 Ends for CSR 1-7088501 */

#ifdef SCTP_STATS_ENABLED

            if( size <= SCTP_MIN_FRAGSIZE )
            {
                SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_T_MED_INSTD_MIN, 1 );
            }

#endif
            return p_buf;
        }
    }

    if( size <= SCTP_MAX_FRAGSIZE )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->tx_freelist.list_max );

        if( p_buf != SCTP_NULL )
        {
            lstDelete( &p_ass->tx_freelist.list_max, ( NODE * )p_buf );
            p_buf->ref_count = 1;
            /* SPR 20573 Starts for CSR 1-7088501 */
            p_buf->retrans_flag = SCTP_RETRANSFLAG_NONE;
            p_buf->sent = 0;
            p_buf->dest_addr_index = 0;
            /* SPR 20573 Ends for CSR 1-7088501 */


#ifdef SCTP_STATS_ENABLED

            if( size <= SCTP_MIN_FRAGSIZE )
            {
                SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_T_MAX_INSTD_MIN, 1 );
            }

            else if( size <= SCTP_MED_FRAGSIZE )
            {
                SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_T_MAX_INSTD_MED, 1 );
            }

#endif
            return p_buf;
        }
    }

    return SCTP_NULL;
}


/**************************************************************************
 *    FUNCTION :    sctp_free_transmit_buffer
 *
 *    DESCRIPTION:
 *        This function returns buffer passed to the Tx-freelist per
 *        association
 *
 *    RETURN VALUE:
 *        None
 ***************************************************************************/
void
sctp_free_transmit_buffer(
    sctp_tcb_st          *p_ass,
    sctpbuffer_st        *p_buf )

{

    if( p_buf == SCTP_NULL || --p_buf->ref_count > 0 )
    {
        return;
    }

    if( p_ass != SCTP_NULL )
    {
        /* Changes For CSR#1-5037386 */
        if( ( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )
                && ( p_ass->transmit.remote[p_buf->dest_addr_index].retrans_packet_count > 0 ) )
        {
            p_ass->transmit.remote[p_buf->dest_addr_index].retrans_packet_count--;
        }

        if( ( p_buf->sent == 0 )
                && ( p_ass->transmit.remote[p_buf->dest_addr_index].unsent_packet_count > 0 ) )
        {
            p_ass->transmit.remote[p_buf->dest_addr_index].unsent_packet_count--;
        }

        /* Changes For CSR#1-5037386 Ends */
        p_ass->total_sent--;

        switch( p_buf->id )
        {
            case SCTP_MIN_FRAGSIZE:
                {
                    lstAdd( &p_ass->tx_freelist.list_min, ( NODE * )p_buf );
                    break;
                }

            case SCTP_MED_FRAGSIZE:
                {
                    lstAdd( &p_ass->tx_freelist.list_med, ( NODE * )p_buf );
                    break;
                }

            case SCTP_MAX_FRAGSIZE:
                {
                    lstAdd( &p_ass->tx_freelist.list_max, ( NODE * )p_buf );
                    break;
                }

            default:
                {
                    break;
                }
        }
    }

    return;
}


/**************************************************************************
 *    FUNCTION :    sctp_user_free_transmit_buffer
 *
 *    DESCRIPTION:
 *        This function returns buffer passed to the Tx-freelist.
 *
 *    RETURN VALUE:
 *        None
 ***************************************************************************/
void
sctp_user_free_transmit_buffer(
    sctp_tcb_st          *p_ass,
    sctpbuffer_st        *p_buf )

{
    sctp_buffer_list_st  *p_free_list;

    if( p_ass )
    {
        sctp_free_transmit_buffer( p_ass, p_buf );
        return;
    }

    if( p_buf == SCTP_NULL || --p_buf->ref_count > 0 )
    {
        return;
    }

    p_free_list = &( sctp_db_main.tx_list );

    switch( p_buf->id )
    {
        case SCTP_MIN_FRAGSIZE:
            {

                lstAdd( &( p_free_list->list_min ), ( NODE * )p_buf );
                break;
            }

        case SCTP_MED_FRAGSIZE:
            {
                lstAdd( &( p_free_list->list_med ), ( NODE * )p_buf );
                break;
            }

        case SCTP_MAX_FRAGSIZE:
            {
                lstAdd( &( p_free_list->list_max ), ( NODE * )p_buf );
                break;
            }

        default:
            break;
    }
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_db_add_buf_to_list
 *
 *    DESCRIPTION:
 *        This functions adds transmit buff to tx_freelist of the assoc/tcb.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_add_buf_to_list(
    sctp_buffer_list_st *p_list,
    sctp_U32bit         num_max_buf,
    sctp_U32bit         num_med_buf,
    sctp_U32bit         num_min_buf )

{
    sctpbuffer_st *p_buf;
    sctp_U32bit cnt = 0;


    /* get the first available node from reserve free list,
     * delete it from that pool, add it to the free list of assoc.
     */
    for( cnt = 0; cnt < num_max_buf; cnt++ )
    {
        p_buf =
            ( sctpbuffer_st * )lstFirst( &sctp_db_main.reserve_list.list_max );

        lstDelete( &sctp_db_main.reserve_list.list_max, ( NODE * )p_buf );

        lstAdd( &( p_list->list_max ), ( NODE * )p_buf );
    }

    for( cnt = 0; cnt < num_med_buf; cnt++ )
    {
        p_buf =
            ( sctpbuffer_st * )lstFirst( &sctp_db_main.reserve_list.list_med );

        lstDelete( &sctp_db_main.reserve_list.list_med, ( NODE * )p_buf );

        lstAdd( &( p_list->list_med ), ( NODE * )p_buf );
    }

    for( cnt = 0; cnt < num_min_buf; cnt++ )
    {
        p_buf =
            ( sctpbuffer_st * )lstFirst( &sctp_db_main.reserve_list.list_min );

        lstDelete( &sctp_db_main.reserve_list.list_min, ( NODE * )p_buf );

        lstAdd( &( p_list->list_min ), ( NODE * )p_buf );
    }

    return SCTP_SUCCESS;
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_db_del_buf_from_list
 *
 *    DESCRIPTION:
 *        This functions deletes transmit buff from tx_freelist of the
 *        assoc/tcb
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_del_buf_from_list(
    sctp_buffer_list_st *p_list,
    sctp_U32bit         num_max_buf,
    sctp_U32bit         num_med_buf,
    sctp_U32bit         num_min_buf )

{
    sctpbuffer_st *p_buf;
    sctp_U32bit cnt = 0;


    /* get the first available node from reserve free list,
     * delete it from that pool, add it to the free list of assoc.
     */
    for( cnt = 0; cnt < num_max_buf; cnt++ )
    {
        p_buf =
            ( sctpbuffer_st * )lstFirst( &p_list->list_max );

        lstDelete( &( p_list->list_max ), ( NODE * )p_buf );

        lstAdd( &( sctp_db_main.reserve_list.list_max ), ( NODE * )p_buf );
    }

    for( cnt = 0; cnt < num_med_buf; cnt++ )
    {
        p_buf =
            ( sctpbuffer_st * )lstFirst( &p_list->list_med );

        lstDelete( &( p_list->list_med ), ( NODE * )p_buf );

        lstAdd( &( sctp_db_main.reserve_list.list_med ), ( NODE * )p_buf );
    }

    for( cnt = 0; cnt < num_min_buf; cnt++ )
    {
        p_buf =
            ( sctpbuffer_st * )lstFirst( &p_list->list_min );

        lstDelete( &( p_list->list_min ), ( NODE * )p_buf );

        lstAdd( &( sctp_db_main.reserve_list.list_min ), ( NODE * )p_buf );
    }

    return SCTP_SUCCESS;
}

/***************************************************************************
**
** FUNCTION NAME:       sctp_db_verify_cong_reg_def
**
** DESCRIPTION  :       This function checks the congestion region
**                      definition. This function also checks
**                      the congestion region definition for  NCR, LCR, MCR
**                      and HCR as per the following rules.
**                      Value should lie in between 0 - 100.
**                      Min_onset_x <= onset_x <=Max_onset_x,
**                      (Not valid for NCR) Abate_x > onset_x-1,
**                      (Not valid for NCR)
**
** RETURNS      :       SCTP_SUCCESS
 *                      SCTP_FALILURE
**
***************************************************************************/
sctp_return_t
sctp_db_verify_cong_reg_def(
    sctp_cong_reg_def_st cong_reg_def,
    sctp_error_t        *p_ecode )
{

    if( ( cong_reg_def.no_cong_lvl_abate != 0 ) ||
            ( 0 != cong_reg_def.no_cong_lvl_onset ) )
    {
        *p_ecode = E_DB_INV_CONG_LVL_VALUE;
        return SCTP_FAILURE;
    }

    if( ( cong_reg_def.low_cong_lvl_abate > 100 ) ||
            ( cong_reg_def.low_cong_lvl_onset > 100 ) ||
            ( cong_reg_def.med_cong_lvl_abate > 100 ) ||
            ( cong_reg_def.med_cong_lvl_onset > 100 ) ||
            ( cong_reg_def.high_cong_lvl_abate > 100 ) ||
            ( cong_reg_def.high_cong_lvl_onset > 100 ) )
    {
        *p_ecode = E_DB_INV_CONG_LVL_VALUE;
        return SCTP_FAILURE;
    }

    if( ( cong_reg_def.high_cong_lvl_onset <= cong_reg_def.high_cong_lvl_abate ) ||
            ( cong_reg_def.high_cong_lvl_abate <= cong_reg_def.med_cong_lvl_onset ) ||

            ( cong_reg_def.med_cong_lvl_onset <= cong_reg_def.med_cong_lvl_abate ) ||
            ( cong_reg_def.med_cong_lvl_abate <= cong_reg_def.low_cong_lvl_onset ) ||

            ( cong_reg_def.low_cong_lvl_onset <= cong_reg_def.low_cong_lvl_abate ) ||
            ( cong_reg_def.low_cong_lvl_abate <= cong_reg_def.no_cong_lvl_onset ) )
    {
        *p_ecode = E_DB_INV_CONG_LVL_VALUE;
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/***************************************************************************
**
** FUNCTION NAME:       sctp_get_cong_reg_def
**
** DESCRIPTION  :       This function gets congestion region definitions
**
** RETURNS      :       None
**
***************************************************************************/
void    sctp_get_cong_reg_def(
    sctp_Boolean_t        is_transmit,
    sctp_Boolean_t        def_flag,
    sctp_U32bit           server_id,
    sctp_cong_reg_def_st  *p_cong_reg )
{
    sctp_cong_reg_def_st  *cong_reg;
    sctp_ep_st            *p_ser;

    /* Get the congestion region definition */
    if( is_transmit == SCTP_TRUE )
    {
        if( SCTP_FALSE == def_flag )
        {
            p_ser = &sctp_db_main.ep_table[server_id];
            cong_reg = &p_ser->trx_cong_reg_def;
        }

        else
        {
            SCTPLOCK();
            cong_reg = &sctp_db_main.trx_cong_reg_def;
        }

        __MEMCOPY( ( sctp_U8bit * )p_cong_reg,
                   ( sctp_U8bit * )cong_reg, sizeof( sctp_cong_reg_def_st ) );
    }

    else
    {
        if( SCTP_FALSE == def_flag )
        {
            p_ser = &sctp_db_main.ep_table[server_id];
            cong_reg = &p_ser->rcv_cong_reg_def;
        }

        else
        {
            SCTPLOCK();
            cong_reg = &sctp_db_main.rcv_cong_reg_def;
        }

        __MEMCOPY( ( sctp_U8bit * )p_cong_reg,
                   ( sctp_U8bit * )cong_reg, sizeof( sctp_cong_reg_def_st ) );
    }

    if( SCTP_TRUE == def_flag )
    {
        SCTPUNLOCK();
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_set_tx_buffers
 *
 *    DESCRIPTION:
 *        This functions sets the number of tx buffers.This is assumed that
 *    n_bytes is a multiple of SCTP_MIN_FRAGSIZE
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_set_tx_buffers(
    sctp_tcb_st         *p_assoc,
    sctp_U32bit         n_bytes,
    sctp_error_t        *p_ecode )
{
    sctp_S32bit  n_delta;
    sctp_U32bit  max_buffer_req, med_buffer_req, min_buffer_req;
    sctp_U32bit  max_free_count, med_free_count, min_free_count;
    sctp_U32bit  max_used_count = 0, med_used_count = 0, min_used_count = 0;
    sctp_U32bit  n_now;
    sctp_U32bit  max_per, med_per, min_per;
    sctpbuffer_st *p_buf;

    if( n_bytes % SCTP_MIN_FRAGSIZE )
    {
        *p_ecode = EINV_PARAMS;
        return ( SCTP_FAILURE );
    }

    /*find the number of busy buffers for each size*/
    p_buf = ( sctpbuffer_st * )lstFirst( &p_assoc->transmit.transmit_queue );

    for( ; p_buf; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * )p_buf ) )
    {
        switch( p_buf->id )
        {
            case SCTP_MIN_FRAGSIZE:
                min_used_count++;
                break;

            case SCTP_MED_FRAGSIZE:
                med_used_count++;
                break;

            case SCTP_MAX_FRAGSIZE:
                max_used_count++;
                break;
        }
    }


    /*get the free buffers count*/
    min_free_count = lstCount( &p_assoc->tx_freelist.list_min );
    med_free_count = lstCount( &p_assoc->tx_freelist.list_med );
    max_free_count = lstCount( &p_assoc->tx_freelist.list_max );

    /*number of bytes right now*/
    n_now = ( ( min_free_count + min_used_count ) * SCTP_MIN_FRAGSIZE ) +
            ( ( med_free_count + med_used_count ) * SCTP_MED_FRAGSIZE ) +
            ( ( max_free_count + max_used_count ) * SCTP_MAX_FRAGSIZE );

    /*the delta we need to provide, can be negative */
    n_delta = n_bytes - n_now;

    /*has to be an absolute value*/
    if( n_delta < 0 )
    {
        /*got to delete*/
        n_delta = -n_delta;
    }

    med_per = sctp_db_main.med_tx_buffers * 100 / ( sctp_db_main.min_tx_buffers
                                                    + sctp_db_main.med_tx_buffers + sctp_db_main.max_tx_buffers );
    max_per = sctp_db_main.max_tx_buffers * 100 / ( sctp_db_main.min_tx_buffers
                                                    + sctp_db_main.med_tx_buffers + sctp_db_main.max_tx_buffers );
    min_per = 100 - ( med_per + max_per );

    /*the buffers of maximum size required to add/delete*/
    max_buffer_req = ( n_delta ) /
                     ( ( SCTP_MIN_FRAGSIZE * min_per ) / max_per
                       + ( SCTP_MED_FRAGSIZE * med_per ) / max_per + SCTP_MAX_FRAGSIZE );

    /*check if we need some more med and min size buffers*/
    n_delta = n_delta - max_buffer_req * SCTP_MAX_FRAGSIZE;

    /* n_delta can also be zero here */
    med_buffer_req = ( n_delta ) /
                     ( ( SCTP_MIN_FRAGSIZE * min_per ) / med_per
                       + SCTP_MED_FRAGSIZE );

    n_delta = n_delta - med_buffer_req * SCTP_MED_FRAGSIZE;

    min_buffer_req = n_delta / SCTP_MIN_FRAGSIZE;
    /* n_delta% SCTP_MIN_FRAG_SIZE must be zero now, checked above */

    if( n_bytes > n_now )
    {
        sctp_U32bit cnt_max, cnt_med, cnt_min;
        cnt_max = lstCount( &( sctp_db_main.reserve_list.list_max ) );
        cnt_med = lstCount( &( sctp_db_main.reserve_list.list_med ) );
        cnt_min = lstCount( &( sctp_db_main.reserve_list.list_min ) );

        /* got to add buffers */
        /* first check that there are enough buffers in reserved pool*/
        if( ( max_buffer_req > cnt_max ) ||
                ( med_buffer_req > cnt_med ) ||
                ( min_buffer_req > cnt_min ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*add the buffers */

        sctp_db_add_buf_to_list( &( p_assoc->tx_freelist ), max_buffer_req,
                                 med_buffer_req, min_buffer_req );
    }

    else
    {
        /* delete buffers */
        /* check that buffers can be deleted from free list */
        if( ( max_buffer_req > ( sctp_U32bit )lstCount(
                    &( p_assoc->tx_freelist.list_max ) ) ) ||
                ( med_buffer_req > ( sctp_U32bit )lstCount(
                      &( p_assoc->tx_freelist.list_med ) ) ) ||
                ( min_buffer_req > ( sctp_U32bit )lstCount(
                      &( p_assoc->tx_freelist.list_min ) ) ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*
         * now check that total buffers quantity should not go below
         * init time default
         */
        if( ( ( ( min_free_count + min_used_count ) - min_buffer_req )
                < sctp_db_main.min_tx_buffers ) ||
                ( ( ( med_free_count + med_used_count ) - med_buffer_req )
                  < sctp_db_main.med_tx_buffers ) ||
                ( ( ( max_free_count + max_used_count ) - max_buffer_req )
                  < sctp_db_main.max_tx_buffers ) )
        {
            /* can not delete these many buffers*/
            *p_ecode = E_BUFFER_TOO_SMALL;
            return ( SCTP_FAILURE );
        }

        sctp_db_del_buf_from_list( &( p_assoc->tx_freelist ), max_buffer_req,
                                   med_buffer_req, min_buffer_req );
    }

    /*set the new window size*/
    p_assoc->transmit.wnd_size = n_bytes;
    return ( SCTP_SUCCESS );
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_db_set_rx_buffers
 *
 *    DESCRIPTION:
 *        This functions sets the number of rx buffers.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_set_rx_buffers(
    sctp_tcb_st         *p_assoc,
    sctp_U32bit         n_bytes,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit  stream = 0;
    sctp_S32bit  n_delta;
    sctp_U32bit  max_buffer_req, med_buffer_req, min_buffer_req;
    sctp_U32bit  max_free_count, med_free_count, min_free_count;
    sctp_U32bit  max_used_count = 0, med_used_count = 0, min_used_count = 0;
    sctp_U32bit  n_now;
    sctp_U32bit  max_per, med_per, min_per;
    sctpbuffer_st *p_buf;

    if( n_bytes % SCTP_MIN_FRAGSIZE )
    {
        *p_ecode = EINV_PARAMS;
        return ( SCTP_FAILURE );
    }

    for( stream = 0; stream < p_assoc->receive.num_in_streams; stream ++ )
    {
        p_buf = ( sctpbuffer_st * )
                lstFirst( &p_assoc->receive.stream[stream].receive_queue );

        for( ; p_buf; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * )p_buf ) )
        {
            switch( p_buf->id )
            {
                case SCTP_MIN_FRAGSIZE:
                    min_used_count++;
                    break;

                case SCTP_MED_FRAGSIZE:
                    med_used_count++;
                    break;

                case SCTP_MAX_FRAGSIZE:
                    max_used_count++;
                    break;
            }
        }
    }


    /*For min*/
    min_free_count = lstCount( &p_assoc->rx_freelist.list_min );
    med_free_count = lstCount( &p_assoc->rx_freelist.list_med );
    max_free_count = lstCount( &p_assoc->rx_freelist.list_max );

    n_now = ( ( min_free_count + min_used_count ) * SCTP_MIN_FRAGSIZE ) +
            ( ( med_free_count + med_used_count ) * SCTP_MED_FRAGSIZE ) +
            ( ( max_free_count + max_used_count ) * SCTP_MAX_FRAGSIZE );

    n_delta = n_bytes - n_now;

    if( n_delta < 0 )
    {
        /*got to delete*/
        n_delta = -n_delta;
    }

    med_per = sctp_db_main.med_rx_buffers * 100 / ( sctp_db_main.min_rx_buffers
                                                    + sctp_db_main.med_rx_buffers + sctp_db_main.max_rx_buffers );
    max_per = sctp_db_main.max_rx_buffers * 100 / ( sctp_db_main.min_rx_buffers
                                                    + sctp_db_main.med_rx_buffers + sctp_db_main.max_rx_buffers );
    min_per = 100 - ( med_per + max_per );

    /*the buffers of maximum size required to add/delete*/
    max_buffer_req = ( n_delta ) /
                     ( ( SCTP_MIN_FRAGSIZE * min_per ) / max_per
                       + ( SCTP_MED_FRAGSIZE * med_per ) / max_per + SCTP_MAX_FRAGSIZE );

    /*check if we need some more med and min size buffers*/
    n_delta = n_delta - max_buffer_req * SCTP_MAX_FRAGSIZE;

    /* n_delta can also be zero here */
    med_buffer_req = ( n_delta ) /
                     ( ( SCTP_MIN_FRAGSIZE * min_per ) / med_per
                       + SCTP_MED_FRAGSIZE );

    n_delta = n_delta - med_buffer_req * SCTP_MED_FRAGSIZE;

    min_buffer_req = n_delta / SCTP_MIN_FRAGSIZE;


    if( n_bytes > n_now )
    {
        /* add buffers */
        /* first chck that there are enough buffers in reserved pool*/
        if( ( max_buffer_req > ( sctp_U32bit )lstCount(
                    &sctp_db_main.reserve_list.list_max ) ) ||
                ( med_buffer_req > ( sctp_U32bit )lstCount(
                      &sctp_db_main.reserve_list.list_med ) ) ||
                ( min_buffer_req > ( sctp_U32bit )lstCount(
                      &sctp_db_main.reserve_list.list_min ) ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*add the buffers */

        sctp_db_add_buf_to_list( &( p_assoc->rx_freelist ), max_buffer_req,
                                 med_buffer_req, min_buffer_req );
    }

    else
    {
        /* delete buffers */
        /* check that buffers can be deleted from free list */
        if( ( max_buffer_req > ( sctp_U32bit )lstCount(
                    &( p_assoc->rx_freelist.list_max ) ) ) ||
                ( med_buffer_req > ( sctp_U32bit )lstCount(
                      &( p_assoc->rx_freelist.list_med ) ) ) ||
                ( min_buffer_req > ( sctp_U32bit )lstCount(
                      &( p_assoc->rx_freelist.list_min ) ) ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*
         * now check that total buffers quantity should not go below
         * init time default
         */
        if( ( ( ( min_free_count + min_used_count ) - min_buffer_req )
                < sctp_db_main.min_rx_buffers ) ||
                ( ( ( med_free_count + med_used_count ) - med_buffer_req )
                  < sctp_db_main.med_rx_buffers ) ||
                ( ( ( max_free_count + max_used_count ) - max_buffer_req )
                  < sctp_db_main.max_rx_buffers ) )
        {
            /* can not delete these many buffers*/
            *p_ecode = E_BUFFER_TOO_SMALL;
            return ( SCTP_FAILURE );
        }

        sctp_db_del_buf_from_list( &( p_assoc->rx_freelist ), max_buffer_req,
                                   med_buffer_req, min_buffer_req );
    }

    /*set the new window size*/
    p_assoc->receive.wnd_size = n_bytes;
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_set_tx_frags
 *
 *    DESCRIPTION:
 *        This functions sets the number of tx frag.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_set_tx_frags(
    sctp_tcb_st         *p_assoc,
    sctp_count_frags_st  count_frags,
    sctp_Boolean_t       is_increment,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit  max_free_count, med_free_count, min_free_count;
    sctp_U32bit  max_used_count = 0, med_used_count = 0, min_used_count = 0;
    sctp_U32bit  n_now;
    sctpbuffer_st *p_buf;

    /*find the number of busy buffers for each size*/
    p_buf = ( sctpbuffer_st * )lstFirst( &p_assoc->transmit.transmit_queue );

    for( ; p_buf; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * )p_buf ) )
    {
        switch( p_buf->id )
        {
            case SCTP_MIN_FRAGSIZE:
                min_used_count++;
                break;

            case SCTP_MED_FRAGSIZE:
                med_used_count++;
                break;

            case SCTP_MAX_FRAGSIZE:
                max_used_count++;
                break;
        }
    }

    /*get the free buffers count*/
    min_free_count = lstCount( &p_assoc->tx_freelist.list_min );
    med_free_count = lstCount( &p_assoc->tx_freelist.list_med );
    max_free_count = lstCount( &p_assoc->tx_freelist.list_max );

    /*number of bytes right now*/
    n_now = ( ( min_free_count + min_used_count ) * SCTP_MIN_FRAGSIZE ) +
            ( ( med_free_count + med_used_count ) * SCTP_MED_FRAGSIZE ) +
            ( ( max_free_count + max_used_count ) * SCTP_MAX_FRAGSIZE );

    if( is_increment )
    {
        sctp_U32bit cnt_max, cnt_med, cnt_min;
        cnt_max = lstCount( &( sctp_db_main.reserve_list.list_max ) );
        cnt_med = lstCount( &( sctp_db_main.reserve_list.list_med ) );
        cnt_min = lstCount( &( sctp_db_main.reserve_list.list_min ) );

        /* got to add buffers */
        /* first check that there are enough buffers in reserved pool*/
        if( ( count_frags.max_num_frag > cnt_max ) ||
                ( count_frags.med_num_frag > cnt_med ) ||
                ( count_frags.min_num_frag > cnt_min ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*add the buffers */

        sctp_db_add_buf_to_list( &( p_assoc->tx_freelist ), count_frags.max_num_frag,
                                 count_frags.med_num_frag, count_frags.min_num_frag );
        /*set the new window size*/
        p_assoc->transmit.wnd_size = n_now +
                                     count_frags.min_num_frag * SCTP_MIN_FRAGSIZE +
                                     count_frags.med_num_frag * SCTP_MED_FRAGSIZE +
                                     count_frags.max_num_frag * SCTP_MAX_FRAGSIZE;
    }

    else
    {
        /* delete buffers */
        /* check that buffers can be deleted from free list */
        if( ( count_frags.max_num_frag > ( sctp_U32bit )lstCount(
                    &( p_assoc->tx_freelist.list_max ) ) ) ||
                ( count_frags.med_num_frag > ( sctp_U32bit )lstCount(
                      &( p_assoc->tx_freelist.list_med ) ) ) ||
                ( count_frags.min_num_frag > ( sctp_U32bit )lstCount(
                      &( p_assoc->tx_freelist.list_min ) ) ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*
         * now check that total buffers quantity should not go below
         * init time default
         */
        if( ( ( ( min_free_count + min_used_count ) - count_frags.min_num_frag )
                < sctp_db_main.min_tx_buffers ) ||
                ( ( ( med_free_count + med_used_count ) - count_frags.med_num_frag )
                  < sctp_db_main.med_tx_buffers ) ||
                ( ( ( max_free_count + max_used_count ) - count_frags.max_num_frag )
                  < sctp_db_main.max_tx_buffers ) )
        {
            /* can not delete these many buffers*/
            *p_ecode = E_BUFFER_TOO_SMALL;
            return ( SCTP_FAILURE );
        }

        sctp_db_del_buf_from_list( &( p_assoc->tx_freelist ), count_frags.max_num_frag,
                                   count_frags.med_num_frag, count_frags.min_num_frag );
        /*set the new window size*/
        p_assoc->transmit.wnd_size = n_now -
                                     count_frags.min_num_frag * SCTP_MIN_FRAGSIZE -
                                     count_frags.med_num_frag * SCTP_MED_FRAGSIZE -
                                     count_frags.max_num_frag * SCTP_MAX_FRAGSIZE;
    }

    return ( SCTP_SUCCESS );
}
/***************************************************************************
 *    FUNCTION :
 *        sctp_db_set_rx_frags
 *
 *    DESCRIPTION:
 *        This functions sets the number of rx frags.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_set_rx_frags(
    sctp_tcb_st         *p_assoc,
    sctp_count_frags_st  count_frags,
    sctp_Boolean_t       is_increment,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit  stream = 0;
    sctp_U32bit  max_free_count, med_free_count, min_free_count;
    sctp_U32bit  max_used_count = 0, med_used_count = 0, min_used_count = 0;
    sctp_U32bit  n_now;
    sctpbuffer_st *p_buf;

    for( stream = 0; stream < p_assoc->receive.num_in_streams; stream ++ )
    {
        p_buf = ( sctpbuffer_st * )
                lstFirst( &p_assoc->receive.stream[stream].receive_queue );

        for( ; p_buf; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * )p_buf ) )
        {
            switch( p_buf->id )
            {
                case SCTP_MIN_FRAGSIZE:
                    min_used_count++;
                    break;

                case SCTP_MED_FRAGSIZE:
                    med_used_count++;
                    break;

                case SCTP_MAX_FRAGSIZE:
                    max_used_count++;
                    break;
            }
        }
    }


    /*For min*/
    min_free_count = lstCount( &p_assoc->rx_freelist.list_min );
    med_free_count = lstCount( &p_assoc->rx_freelist.list_med );
    max_free_count = lstCount( &p_assoc->rx_freelist.list_max );

    n_now = ( ( min_free_count + min_used_count ) * SCTP_MIN_FRAGSIZE ) +
            ( ( med_free_count + med_used_count ) * SCTP_MED_FRAGSIZE ) +
            ( ( max_free_count + max_used_count ) * SCTP_MAX_FRAGSIZE );

    if( is_increment )
    {
        /* add buffers */
        /* first chck that there are enough buffers in reserved pool*/
        if( ( count_frags.max_num_frag > ( sctp_U32bit )lstCount(
                    &sctp_db_main.reserve_list.list_max ) ) ||
                ( count_frags.med_num_frag > ( sctp_U32bit )lstCount(
                      &sctp_db_main.reserve_list.list_med ) ) ||
                ( count_frags.min_num_frag > ( sctp_U32bit )lstCount(
                      &sctp_db_main.reserve_list.list_min ) ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*add the buffers */

        sctp_db_add_buf_to_list( &( p_assoc->rx_freelist ), count_frags.max_num_frag,
                                 count_frags.med_num_frag, count_frags.min_num_frag );
        /*set the new window size*/
        p_assoc->receive.wnd_size = n_now +
                                    count_frags.min_num_frag * SCTP_MIN_FRAGSIZE +
                                    count_frags.med_num_frag * SCTP_MED_FRAGSIZE +
                                    count_frags.max_num_frag * SCTP_MAX_FRAGSIZE;
    }

    else
    {
        /* delete buffers */
        /* check that buffers can be deleted from free list */
        if( ( count_frags.max_num_frag > ( sctp_U32bit )lstCount(
                    &( p_assoc->rx_freelist.list_max ) ) ) ||
                ( count_frags.med_num_frag > ( sctp_U32bit )lstCount(
                      &( p_assoc->rx_freelist.list_med ) ) ) ||
                ( count_frags.min_num_frag > ( sctp_U32bit )lstCount(
                      &( p_assoc->rx_freelist.list_min ) ) ) )
        {
            *p_ecode = E_NORESOURCES;
            return ( SCTP_FAILURE );
        }

        /*
         * now check that total buffers quantity should not go below
         * init time default
         */
        if( ( ( ( min_free_count + min_used_count ) - count_frags.min_num_frag )
                < sctp_db_main.min_rx_buffers ) ||
                ( ( ( med_free_count + med_used_count ) - count_frags.med_num_frag )
                  < sctp_db_main.med_rx_buffers ) ||
                ( ( ( max_free_count + max_used_count ) - count_frags.max_num_frag )
                  < sctp_db_main.max_rx_buffers ) )
        {
            /* can not delete these many buffers*/
            *p_ecode = E_BUFFER_TOO_SMALL;
            return ( SCTP_FAILURE );
        }

        sctp_db_del_buf_from_list( &( p_assoc->rx_freelist ), count_frags.max_num_frag,
                                   count_frags.med_num_frag, count_frags.min_num_frag );
        /*set the new window size*/
        p_assoc->receive.wnd_size = n_now -
                                    count_frags.min_num_frag * SCTP_MIN_FRAGSIZE -
                                    count_frags.med_num_frag * SCTP_MED_FRAGSIZE -
                                    count_frags.max_num_frag * SCTP_MAX_FRAGSIZE;
    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_txrx_frags
 *
 *    DESCRIPTION:
 *        This functions gets the number of tx,rx frags.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_get_txrx_frags(
    sctp_tcb_st         *p_assoc,
    sctp_count_frags_st  *tx_num_frags,
    sctp_count_frags_st  *rx_num_frags )
{
    tx_num_frags->min_num_frag = lstCount( &( p_assoc->tx_freelist.list_min ) );
    tx_num_frags->med_num_frag = lstCount( &( p_assoc->tx_freelist.list_med ) );
    tx_num_frags->max_num_frag = lstCount( &( p_assoc->tx_freelist.list_max ) );

    rx_num_frags->min_num_frag = lstCount( &( p_assoc->rx_freelist.list_min ) );
    rx_num_frags->med_num_frag = lstCount( &( p_assoc->rx_freelist.list_med ) );
    rx_num_frags->max_num_frag = lstCount( &( p_assoc->rx_freelist.list_max ) );

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_res_txrx_frags
 *
 *    DESCRIPTION:
 *        This functions gets the reserved number of tx,rx frags.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_db_get_res_txrx_frags(
    sctp_count_frags_st  *tx_num_frags,
    sctp_count_frags_st  *rx_num_frags )
{
    tx_num_frags->min_num_frag = lstCount( &( sctp_db_main.reserve_list.list_min ) );
    tx_num_frags->med_num_frag = lstCount( &( sctp_db_main.reserve_list.list_med ) );
    tx_num_frags->max_num_frag = lstCount( &( sctp_db_main.reserve_list.list_max ) );

    rx_num_frags = rx_num_frags;
    return ( SCTP_SUCCESS );
}

sctp_lock_t     mtu_list_lock;
static LIST     free_mtu_list;

/***************************************************************************
 *    FUNCTION :
 *        sctp_db_get_mtu_buffer
 *
 *    DESCRIPTION:
 *        This functions gets a MTU buffer from freelist.
 *
 *    RETURN VALUE:
 *        sctpmtubuf_st
 **************************************************************************/
sctpmtubuf_st *
sctp_db_get_mtu_buffer()
{
    sctpmtubuf_st *p_buf;

    SCTP_LOCK( mtu_list_lock );

    p_buf = ( sctpmtubuf_st * )lstFirst( &free_mtu_list );

    if( SCTP_NULL != p_buf )
    {
        lstDelete( &free_mtu_list, ( NODE * )p_buf );
    }

    SCTP_UNLOCK( mtu_list_lock );

    return ( p_buf );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_free_mtu_buffer
 *
 *    DESCRIPTION:
 *        This functions adds a MTU buffer to freelist.
 *
 *    RETURN VALUE:
 *       None
 **************************************************************************/
void
sctp_db_free_mtu_buffer(
    sctpmtubuf_st  *p_buf )
{
    SCTP_LOCK( mtu_list_lock );

    lstAdd( &free_mtu_list, ( NODE * )p_buf );

    SCTP_UNLOCK( mtu_list_lock );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_db_init_mtu_buffers
 *
 *    DESCRIPTION:
 *        This functions initializes the list and lock.
 *
 *    RETURN VALUE:
 *       None
 **************************************************************************/
void
sctp_db_init_mtu_buffers( sctp_U32bit max_count )
{
    sctpmtubuf_st *p_mtu_buffers = ( sctpmtubuf_st * )sctp_recv_thr_buff;
    sctp_U32bit count;

    SCTP_INIT_LOCK( mtu_list_lock );

    lstInit( &free_mtu_list );

    for( count = 0; count < max_count; count++ )
    {
        lstAdd( &free_mtu_list, ( NODE * )&p_mtu_buffers[count] );
    }
}








