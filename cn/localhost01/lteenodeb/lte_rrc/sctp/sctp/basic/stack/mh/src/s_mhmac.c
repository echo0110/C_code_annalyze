/******************************************************************************
 *    FILE NAME:
 *           s_mhmac.c
 *
 *    DESCRIPTION:
 *        This file is part of message Handling module. It includes
 *        MAC related functions.
 *
 *    DATE          NAME            REFERENCE       REASON
 *    -------------------------------------------------------------
 *    02Jun00     Sandeep Mahajan       -           .Original Creation
 *    29Aug01     gsheoran              -           REL 3.0
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "MH"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_ostmr.h>
#include <s_osmem.h>
#include <s_cobuf.h>
#include <s_dbtcb.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_mhmac.h>
#include <s_comd5.h>


sctp_mac_info_st sctp_mac_info;
sctp_timer_t      mac_timer_id;


/***************************************************************************
 *    FUNCTION :
 *       sctp_init_mac_keys
 *
 *    DESCRIPTION:This function initialises the mac keys ,update the switch
 *              over time to current time and starts mac key update timer
 *
 *    RETURN VALUE:
 *       None
 **************************************************************************/
void
sctp_init_mac_keys(
    void )
{
    sctp_U32bit counter;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_init_mac_keys" ) );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        sctp_mac_info.current_mac_key[counter] = __RANDOM() & 0xff;

        sctp_mac_info.old_mac_key[counter]
            = sctp_mac_info.current_mac_key[counter];
    }

    /* update the switch over time to current time */
    __GETSYSTIME( &sctp_mac_info.switch_over_time );

    /* start the mac key update timer */
    sctp_start_timer(
        SCTP_NULL,
        SCTPEVENT_TIMERMAC,
        SCTP_MAC_KEY_UPDATE_TIME,
        0, &mac_timer_id, SCTP_NULL );

    return;
}


/***************************************************************************
 *    FUNCTION :
 *       sctp_process_mac_timeout
 *
 *    DESCRIPTION:This function processes mac timeout.
 *
 *    RETURN VALUE:
 *       None
 **************************************************************************/
void
sctp_process_mac_timeout(
    void )
{
    sctp_U32bit counter;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_mac_timeout" ) );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        sctp_mac_info.old_mac_key[counter]
            = sctp_mac_info.current_mac_key[counter];

        sctp_mac_info.current_mac_key[counter] = __RANDOM() & 0xff;
    }

    /* update the switch over time to current time */
    __GETSYSTIME( &sctp_mac_info.switch_over_time );

    /* start the mac key update timer */
    sctp_start_timer(
        SCTP_NULL,
        SCTPEVENT_TIMERMAC,
        SCTP_MAC_KEY_UPDATE_TIME,
        0, &mac_timer_id, SCTP_NULL );

    return;
}


/***************************************************************************
 *    FUNCTION :
 *       sctp_is_cookie_valid
 *
 *    DESCRIPTION:This function validates the cookie chunk.
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS
 *       SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_is_cookie_valid(
    sctp_chunk_cookie_st  *cookie_chunk,
    sctp_U16bit          cookie_len ) /* SPR 20568 Starts for CSR 1-6658486*/
{
    sctp_U8bit     savedigest[SCTP_KEYSIZE];
    sctp_U8bit     digest[SCTP_KEYSIZE];
    sctp_Boolean_t old_key_flag = SCTP_FALSE;
    sctp_U32bit    counter;
    sctp_U32bit    time_diff;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_is_cookie_valid" ) );


    if( sctp_calculate_time_difference( &cookie_chunk->time_stamp,
                                        &sctp_mac_info.switch_over_time, &time_diff ) == SCTP_FAILURE )
    {
        old_key_flag = SCTP_TRUE;
    }

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        savedigest[counter] =    cookie_chunk->digest[counter];

        if( old_key_flag == SCTP_TRUE )
        {
            cookie_chunk->digest[counter]
                = sctp_mac_info.old_mac_key[counter];
        }

        else
        {
            cookie_chunk->digest[counter]
                = sctp_mac_info.current_mac_key[counter];
        }
    }

    /* SPR 20568 Starts for CSR 1-6658486 */
    md5digest( ( sctp_U8bit * )( cookie_chunk ), cookie_len, &digest[0] );

    /* SPR 20568 Ends for CSR 1-6658486 */
    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        if( digest[counter] != savedigest[counter] )
        {
            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *       sctp_init_chunk_cookie
 *
 *    DESCRIPTION:This function initialises the cookie chunk

 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS
 *       SCTP_FAILURE
 **************************************************************************/
void
sctp_init_chunk_cookie(
    sctp_chunk_cookie_st   *cookie_chunk )
{
    sctp_U32bit counter;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_init_chunk_cookie" ) );

    __MEMSET( cookie_chunk, 0, sizeof( sctp_chunk_cookie_st ) );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        cookie_chunk->digest[counter] = sctp_mac_info.current_mac_key[counter];
    }

    return;
}




