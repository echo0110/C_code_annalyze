/***************************************************************************
 *  FILE NAME  : s_osmem.h
 *
 *  DESCRIPTION: Contains the Memory related macros.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_cli_mem_SEEN__
#define __FILE_sctp_cli_mem_SEEN__

#include <sig_os.h>

sctp_Pvoid_t sctp_client_get_mesg_buffer( sctp_U32bit size );

sctp_return_t sctp_client_free_mesg_buffer( sctp_U8bit **p_p_buffer );

sctp_return_t sctp_client_init_mem_module( sctp_void_t );

sctp_void_t sctp_client_deinit_mem_module( sctp_void_t );

sctp_Pvoid_t sctp_client_get_buffer( sctp_U32bit size );

sctp_return_t sctp_client_free_buffer( sctp_Pvoid_t p_buffer );

#endif /* __FILE_sctp_mem_SEEN__ */
