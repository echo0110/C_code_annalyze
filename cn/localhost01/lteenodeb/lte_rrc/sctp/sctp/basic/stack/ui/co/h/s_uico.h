/***************************************************************************
 *  FILE NAME:   s_uico.h
 *
 *  DESCRIPTION:
 *   This module has all functions common to both SU and SM UI.
 *
 *    DATE         NAME              REF#    REASON
 *    -------      ----------------  -----   --------------------------------
 *    30Mar01      Vakul Garg        -----   Original
 *    12Sep01      Yogesh Gahlaut    -----   Rel 3.0 Changes
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **************************************************************************/
#ifndef     __FILE_sctp_uico_SEEN__
#define     __FILE_sctp_uico_SEEN__

#ifdef  __cplusplus
extern "C" {
#endif

#include <s_clco.h>


#ifdef SCTP_UI_MESSAGEBASED
    extern sctp_void_t
    sctp_process_api_prim_msg(
        sctp_U8bit           *buffer,
        sctp_len_t           nooctets,
        sctp_client_info_st  *from );

    extern sctp_U8bit *
    sctp_ui_build_api_header(
        sctp_U8bit        *p_buffer,
        sctp_api_id_t     api_id,
        sctp_trans_id_t   transaction_id,
        sctp_src_id_t     src_id,
        sctp_dest_id_t    dest_id );


#endif

#ifdef  __cplusplus
}
#endif

#endif /* __FILE_sctp_uico_SEEN__  */


