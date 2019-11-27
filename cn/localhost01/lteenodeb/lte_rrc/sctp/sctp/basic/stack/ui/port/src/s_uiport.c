/*******************************************************************************
 *    FILE NAME:
 *    s_uiport.c
 *
 *    DESCRIPTION:
 *    sctp ui port related functions
 *
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    Nov 2004    svakumar      -       Original Creation
 *    16Nov04     svakumar                 SPR 7192
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#define SCTP_MODULE "UI"

#include <s_cotyps.h>
#include <s_dbtcb.h>
#include <s_uiport.h>

/* CSR 69326 fix starts*/
#if 0
/***************************************************************************
 *    FUNCTION :
 *       sctp_assign_new_port_for_server
 *
 *    DESCRIPTION:
 *       This function is for Application.If application on server side
 *       wants new port to be used for each new assoc then it returns new
 *       free sctp port if available else return same port.
 *
 *    RETURN VALUE:
 *       sctp port
 *************************************************************************/
sctp_U16bit
sctp_assign_new_port_for_server(
    sctp_U32bit         assoc_id,
    sctp_U32bit         naddr,
    sctp_sockaddr_st    addr_list[],
    sctp_U16bit         old_port )
{
    sctp_return_t    ret = SCTP_FAILURE;
    sctp_U16bit      sctp_port = 0;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( assoc_id );
#ifdef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    SCTP_UNUSED_VARIABLE( naddr );
    SCTP_UNUSED_VARIABLE( addr_list );
#endif
    /*to avoid the warning*/

    /*
     * By default the function returns the same port as passed to this
     * function. If the application desires new port to be allocated, it
     * will have to change the application specific logic as mention below
     * in this function.
     */
    sctp_port = sctp_port;
    ret = ret;
    return old_port;

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */

    /*********** APPLICATION SPECIFIC CODE START *********************/
    /* Application specific logic here:-
     * Get new port for the endpoint. Application may use any other
     * logic to generate a new port.
     */
    ret = sctp_db_get_free_sctpport( &sctp_port );

    if( ret == SCTP_FAILURE )
    {
        return old_port;
    }

    /*********** APPLICATION SPECIFIC CODE END **********************/

    /**** NON APPLICATION specific code. Application is not supposed to
     * change this part.
     */

    /* check IPADDRESS and new PORT is being used by some ep */
    if( sctp_db_addr_bindable( sctp_port, naddr, &addr_list[0] )
            == SCTP_FAILURE )
    {
        return old_port;
    }

    return sctp_port;
#endif
}
#endif

/* CSR 69326 fix ends*/

