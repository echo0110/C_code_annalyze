/*******************************************************************************
*    FILE NAME:
*    s_uismap.h
*
*    DESCRIPTION:
*    This file is part of System MAnager Interface module. It includes
*    functions that process SM  API's.
*
*    DATE        NAME       REFERENCE       REASON
*    -----------------------------------------------------
*
*    29March 2001     Vakul Garg  -       .Original Creation
*
*
*
*   Copyright (C) 2006 Aricent Inc . All Rights Reserved
*******************************************************************************/
#ifndef _S_UISMAP_H_SEEN_
#define _S_UISMAP_H_SEEN_

#ifdef __cplusplus
extern "C" {
#endif

    sctp_void_t
    sctp_process_sm_api_prim_msg(
        sctp_U8bit           *p_buffer,
        sctp_len_t           nooctets,
        sctp_client_info_st  *from );

#ifdef __cplusplus
}
#endif

#endif /* _S_UISMAP_H_SEEN_ */

