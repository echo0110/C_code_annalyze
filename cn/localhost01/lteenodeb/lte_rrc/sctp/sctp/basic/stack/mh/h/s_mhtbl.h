/*****************************************************************************
 *    FILE NAME:
 *          s_mhtbl.h
 *
 *    DESCRIPTION:
 *        This file    includes data strucures    exported by message processing
 *        table.
 *
 *    DATE                  NAME        REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef    __FILE_sctp_mesg_tbl_SEEN__
#define    __FILE_sctp_mesg_tbl_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    extern sctp_return_t
    ( *sctp_process_chunk_func_tbl[SCTP_NUM_CHUNK_TYPES] )
    ( sctp_packet_st     *p_sctp,
      sctp_U8bit         *buffer,
      sctp_U32bit        chunk_offset,
      sctp_Boolean_t     last_chunk_flag,
      sctp_tcb_st        *p_ass,
      sctp_error_t       *p_ecode );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_mesg_tbl_SEEN__ */


