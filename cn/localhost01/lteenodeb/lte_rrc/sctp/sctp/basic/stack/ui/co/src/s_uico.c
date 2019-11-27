/******************************************************************************
 *    FILE NAME:  s_uico.c
 *
 *    DESCRIPTION:
 *    This file is part of User Interface module. It includes
 *    functions that are common to SU & SM API's.
 *
 *    DATE    NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    30March 2001     Vakul Garg   -       .Original Creation
 *    12Sep   2001     Yogesh Gahlaut       Rel 3.0 Changes
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include <s_clco.h>
#include <s_uico.h>
#include <s_uisuap.h>
#include <s_uismap.h>
#include <s_clpapi.h>
#include <s_clsmap.h>
#include <s_red.h>


#ifdef SCTP_UI_MESSAGEBASED
/***************************************************************************
 *    FUNCTION :  sctp_process_api_prim_msg
 *
 *    DESCRIPTION:
 *    This function is called from the es to process the API messages
 *    in message based interface.
 *
 *    RETURN VALUE:
 *    None
 *************************************************************************/


sctp_void_t
sctp_process_api_prim_msg(
    sctp_U8bit            *buffer,
    sctp_len_t            nooctets,
    sctp_client_info_st   *from )
{
    sctp_api_id_t    api_id;
    sctp_U16bit      api_version;
    sctp_U32bit      req_len;

    /*
     * check if    length > header
     */
    if( nooctets < SCTP_API_HEADER_LEN )
    {
        /* report    error */
        return;
    }

    /* get the header fields */
    api_id = sctp_buffer_get_UshortAtOffset( buffer, 0 );
    api_version = sctp_buffer_get_UshortAtOffset( buffer, 2 );
    req_len = sctp_buffer_get_UlongAtOffset( buffer, 12 );

    if( api_version != SCTP_API_VERSION_NO )
    {
        return;
    }

    if( req_len != nooctets )
    {
        /* error */
        return;
    }

#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_CLI_RECV_COM_THREAD

    if( sctp_stack_proc_state == SCTP_PROC_STATE_STOP )
    {
        /* If the stack is in STOP state, only redundancy and initialization
         * api can be called
         */
        if( api_id == SCTP_API_INIT_STACK_REQ ||
                ( api_id >= SCTP_API_SET_STACK_RED_STATE_REQ &&
                  api_id <= SCTP_API_GET_STACK_HEALTH_REQ ) )
        {
            sctp_process_sm_api_prim_msg( ( sctp_U8bit * ) &buffer[0],
                                          nooctets, from );
        }

        return;
    }

#endif    /*SCTP_CLI_RECV_COM_THREAD*/
#endif    /*SCTP_RED_SUPPORT*/

#ifdef SCTP_UI_SU_MESSAGEBASED

    /*else the stack is in START state. So process the SU requests*/
    if( ( api_id >= SCTP_MIN_SUREQUST_APIID )
            && ( api_id <=    SCTP_MAX_SUREQUST_APIID ) )
    {
        sctp_process_su_api_prim_msg( ( sctp_U8bit * ) &buffer[0],
                                      nooctets, from );
        return;
    }

#endif
#ifdef SCTP_UI_SM_MESSAGEBASED

    if( ( api_id >= SCTP_MIN_SMREQ_APIID ) && ( api_id <= SCTP_MAX_SMREQ_APIID ) )
    {
        sctp_process_sm_api_prim_msg( ( sctp_U8bit * ) &buffer[0],
                                      nooctets, from );
        return;
    }

#endif

}

/***************************************************************************
 *    FUNCTION :  sctp_ui_build_api_header
 *
 *    DESCRIPTION:
 *     This function builds API header to be sent across to stack code.
 *    API elements are passed to this function and it converts them into
 *    buffer format.
 *    RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_ui_build_api_header(
    sctp_U8bit        *p_buffer,
    sctp_api_id_t     api_id,
    sctp_trans_id_t   transaction_id,
    sctp_src_id_t     src_id,
    sctp_dest_id_t    dest_id )
{
    sctp_U16bit api_version;
    /* we call buffer functions */
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



#endif
