/***************************************************************************
 *  FILE NAME  : s_fsmtbl.h
 *
 *  DESCRIPTION: Contains the Assoc FSM function ptr table.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_fsm_tbl_SEEN__
#define __FILE_sctp_fsm_tbl_SEEN__

extern void
( *sctp_assoc_fsm_ftbl[SCTP_EVENT_NUM][SCTP_STATE_NUM] )(
    sctp_tcb_st    *p_ass );


extern void
( *sctp_tx_fsm_ftbl[SCTP_TX_EVENT_NUM][SCTP_TX_STATE_NUM] )(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index );

extern void
( *sctp_rx_fsm_ftbl[SCTP_RX_EVENT_NUM][SCTP_RX_STATE_NUM] )(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index );

#endif /* __FILE_sctp_fsm_tbl_SEEN__ */



