/*******************************************************************************
 *    FILE NAME:   s_uiport.h
 *
 *    DESCRIPTION:
 *    This file contains the portable parts of the UI
 *    dir such as
 *        side sctp_assign_new_port_for_server.
 *
 *
 *    DATE      NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    Nov 2004 Satyendra    -       Original Creation
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __FILE_sctp_uiport_SEEN__
#define __FILE_sctp_uiport_SEEN__

#ifdef __cplusplus
extern "C" {
#endif
    extern sctp_U16bit
    sctp_assign_new_port_for_server(
        sctp_U32bit        assoc_id,
        sctp_U32bit        naddr,
        sctp_sockaddr_st   addr_list[],
        sctp_U16bit old_port );

#endif

#ifdef __cplusplus
}
#endif

