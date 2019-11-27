/******************************************************************************
 *    FILE NAME:
 *        s_mhtbl.c
 *
 *    DESCRIPTION:
 *        This file is part of message Handling module. It includes
 *        sctp process chunk function pointer table.
 *
 *    DATE               NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000     Sandeep Mahajan     -           .Original Creation
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "MH"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_mhproc.h>

/****************************************************************************
 *  Function:     sctp_process_chunk_func_tbl
 *
 *  Description: Function pointer table for processing different types of
 *       sctp chunks(data chunk, cookie chunk ,init chunk etc....)
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 ****************************************************************************/

sctp_return_t
( *sctp_process_chunk_func_tbl[SCTP_NUM_CHUNK_TYPES] )
( sctp_packet_st     *p_sctp,
  sctp_U8bit         *buffer,
  sctp_U32bit        chunk_offset,
  sctp_Boolean_t     last_chunk_flag,
  sctp_tcb_st        *p_assoc,
  sctp_error_t       *p_ecode ) =
{
    sctp_process_chunk_data,        /* Data     chunk */
    sctp_process_chunk_init,        /* Init     chunk */
    sctp_process_chunk_initack,     /* Initack  chunk */
    sctp_process_chunk_sack,        /* Sack     chunk */
    sctp_process_chunk_heartbeat,   /* HB       chunk */
    sctp_process_chunk_heartbeatack,/* HBAck    chunk */
    sctp_process_chunk_abort,       /* Abort    chunk */
    sctp_process_chunk_shutdown,    /* Shut     chunk */
    sctp_process_chunk_shutdownack, /* ShutAck  chunk */
    sctp_process_chunk_error,       /* Error    chunk */
    sctp_process_chunk_cookie,      /* cookie   chunk */
    sctp_process_chunk_cookieack,   /* cookieack chunk */
    sctp_process_chunk_ecne,        /* ecne     chunk */
    sctp_process_chunk_cwr,         /* cwr      chunk */
    sctp_process_chunk_shutdowncomplete, /* Shutdown complete Chunk */
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_process_chunk_asconf,
    sctp_process_chunk_asconfack,
#endif
};






