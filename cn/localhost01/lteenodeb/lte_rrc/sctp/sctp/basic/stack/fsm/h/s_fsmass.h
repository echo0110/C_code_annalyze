/***************************************************************************
 *  FILE NAME  : s_fsmass.h
 *
 *  DESCRIPTION: Contains the Functions for the association state machine.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  15Mar04    mrajpal      SPR 6631     Shutdcown gaurd timer
 *  29May17   Neha Gupta    Bug Id 199   CSR  134689
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef       __FILE_sctp_assoc_fsm_SEEN__
#define       __FILE_sctp_assoc_fsm_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    void
    sctp_assoc_state_machine(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    event );

    void
    sctpfsm_assoc_Close_CmdAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_CmdAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgInitack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookW_MsgInitack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgCookieAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_MsgCookieAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_MsgShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgShutAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_MsgShutAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookW_EvtTimer(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_EvtTimer(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookieWE_TimerInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_TimerShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_TimerShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutP_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutR_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_CmdUserData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_EvtTimerData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerHB(
        sctp_tcb_st    *p_ass );

/*Bug Id 199 changes s*/
void 
sctpfsm_assoc_Shut_EvtTimerHB(
    sctp_tcb_st    *p_ass);
/*Bug Id 199 changes e*/

    void
    sctpfsm_assoc_Gener_EvtTimerInac(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerInac(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutP_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Restart_MsgInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_MsgInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgHBAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgCookie(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtNewPmtu(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerPmtu(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_TimerDeAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_TimerLtime(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_CmdConnect(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_MsgStaleErr(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgStaleErr(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgEcne(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgCwr(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerEcn(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_MsgShutCom(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_TimerBundle(
        sctp_tcb_st    *p_ass );

    void
    sctp_assoc_state_machine(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    event );

    void
    sctpfsm_assoc_Close_CmdAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_CmdAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_CmdTerm(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgInitack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookW_MsgInitack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgCookieAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_MsgCookieAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgAbort(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_MsgShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgShutAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_MsgShutAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_EvtTimer(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookW_EvtTimer(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_EvtTimer(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_TimerShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_TimerShut(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutP_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutR_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgSack(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_CmdUserData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_EvtTimerData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerHB(
        sctp_tcb_st    *p_ass );

/*Bug Id 199 changes s*/
void 
sctpfsm_assoc_Shut_EvtTimerHB(
    sctp_tcb_st    *p_ass);
/*Bug Id 199 changes e*/

    void
    sctpfsm_assoc_Estbl_EvtTimerInac(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutP_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutS_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgData(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Restart_MsgInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_MsgInit(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgHBAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgCookie(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_EvtNewPmtu(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtNewPmtu(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_EvtTimerPmtu(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerPmtu(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_TimerDeAck(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_TimerLtime(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Close_CmdConnect(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_CookE_MsgStaleErr(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_MsgStaleErr(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgEcne(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgCwr(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerEcn(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_ShutA_MsgShutCom(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_TimerBundle(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_TimerLinger(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Gener_TimerAutoclose(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerLHbeat(
        sctp_tcb_st    *p_ass );

    sctp_return_t
    sctp_get_idle_addr_index(
        sctp_tcb_st    *p_ass,
        sctp_U32bit    *addr_index,
        sctp_U32bit    *next_idle_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    void
    sctpfsm_assoc_Estbl_CmdAsconf(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_MsgAsconf(
        sctp_tcb_st    *p_ass );

    void
    sctpfsm_assoc_Estbl_EvtTimerAsconf(
        sctp_tcb_st    *p_ass );
    void
    sctpfsm_assoc_Estbl_MsgAsconfAck(
        sctp_tcb_st    *p_ass );

#endif

    void
    sctpfsm_assoc_handle_unreco_chunk_err(
        sctp_tcb_st  *p_ass,
        sctp_U8bit   chunk_type );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_assoc_fsm_SEEN__ */


