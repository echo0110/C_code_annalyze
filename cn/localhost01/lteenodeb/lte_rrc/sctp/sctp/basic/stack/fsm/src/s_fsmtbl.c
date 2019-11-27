/***************************************************************************
 *  FILE NAME  : s_fsmtbl.c
 *
 *  DESCRIPTION: Contains the Assoc FSM function ptr table.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  23Feb04    mrajpal      SPR 6631     T5- Shutdown Gaurd Timer
 *  21Oct04    Amit Ray     Rel6.0       SCTP/FUNC-IG-13
 *  29May17   Neha Gupta    Bug Id 199   CSR  134689
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "FSM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_fsmass.h>
#include <s_fsmtx.h>
#include <s_fsmrx.h>

/*
 * Association State Machine Function Pointer Table
 */

void
( *sctp_assoc_fsm_ftbl[SCTP_EVENT_NUM][SCTP_STATE_NUM] )(
    sctp_tcb_st  *p_ass )    =
{
    /* 0  CMD-ABORT    */
    {
        sctpfsm_assoc_Close_CmdAbort,   /* CLOSE            State */
        sctpfsm_assoc_Gener_CmdAbort,   /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_CmdAbort,   /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_CmdAbort,   /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_CmdAbort,   /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_CmdAbort,   /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_CmdAbort,   /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_CmdAbort,   /* ESTABLISHED      State */
    },

    /* 1 CMD-TERM */
    {
        sctpfsm_assoc_Close_CmdTerm,   /* CLOSE            State */
        sctpfsm_assoc_Gener_CmdTerm,   /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_CmdTerm,   /* COOKIE-ECHOED    State */
        sctpfsm_assoc_ShutA_CmdTerm,   /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_ShutA_CmdTerm,   /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_ShutA_CmdTerm,   /* SHUTOWN-SENT     State */
        sctpfsm_assoc_ShutA_CmdTerm,   /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_CmdTerm,   /* ESTABLISHED      State */
    },

    /* 2 MSG-INITACK */
    {
        NULL,                             /* CLOSE            State */
        sctpfsm_assoc_CookW_MsgInitack,   /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgInitack,   /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_MsgInitack,   /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_MsgInitack,   /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgInitack,   /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgInitack,   /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_MsgInitack,   /* ESTABLISHED      State */
    },

    /* 3    MSG-COOKIEACK */
    {
        NULL,                             /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgCookieAck, /* COOKIE-WAIT      State */
        sctpfsm_assoc_CookE_MsgCookieAck, /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_MsgCookieAck, /*SHUTOWN-PENDING   State */
        sctpfsm_assoc_Gener_MsgCookieAck, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgCookieAck, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgCookieAck, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_MsgCookieAck, /* ESTABLISHED      State */
    },

    /*    4 MSG-ABORT */
    {
        NULL,                             /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgAbort,     /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgAbort,     /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_MsgAbort,     /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_MsgAbort,     /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgAbort,     /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Estbl_MsgAbort,     /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgAbort,     /* ESTABLISHED      State */
    },

    /* 5     MSG-SHUT */
    {
        NULL,                            /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgShut,     /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgShut,     /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_MsgShut,     /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_MsgShut,     /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_ShutS_MsgShut,     /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgShut,     /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgShut,     /* ESTABLISHED      State */
    },

    /* 6    MSG-SHUTACK */
    {
        NULL,                           /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgShutAck, /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgShutAck, /* COOKIE-ECHOED    State */
        NULL,                           /* SHUTOWN-PENDING  State */
        NULL,                           /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_ShutS_MsgShutAck, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_ShutS_MsgShutAck, /* SHUTOWN-ACK-SENT State */
        NULL,                           /* ESTABLISHED      State */
    },

    /* 7    EVT-TIMEOUT */
    {
        NULL,                         /* CLOSE            State */
        sctpfsm_assoc_CookW_EvtTimer, /* COOKIE-WAIT      State */
        /* For Init Timer */
        sctpfsm_assoc_CookE_EvtTimer, /* COOKIE-ECHOED    State */
        /* For Cookie Timer */
        sctpfsm_assoc_ShutS_EvtTimer, /* SHUTOWN-PENDING  State */
        NULL,                         /* SHUTOWN-RECEIVE  State */
        /* 6631 */
        sctpfsm_assoc_ShutS_EvtTimer, /* SHUTOWN-SENT     State */
        /* For T5 - Shutdown Gaurd Timer */
        /* 6631 */
        NULL,                         /* SHUTOWN-ACK-SENT State */
        NULL,                         /* ESTABLISHED      State */
    },

    /*    8 EVT-TIMERSHUTDN */
    {
        NULL,                            /* CLOSE            State */
        sctpfsm_assoc_CookieWE_TimerInit,/* COOKIE-WAIT      State */
        sctpfsm_assoc_CookieWE_TimerInit,/* COOKIE-ECHOED    State */
        NULL,                            /* SHUTOWN-PENDING  State */
        NULL,                            /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_ShutS_TimerShut,   /* SHUTOWN-SENT     State */
        sctpfsm_assoc_ShutA_TimerShut,   /* SHUTOWN-ACK-SENT State */
        NULL,                            /* ESTABLISHED      State */
    },

    /* 9    MSG_SACK */
    {
        sctpfsm_assoc_Close_MsgSack, /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgSack, /* COOKIE-WAIT      State */
        sctpfsm_assoc_CookE_MsgSack, /* COOKIE-ECHOED    State */
        sctpfsm_assoc_ShutP_MsgSack, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_ShutR_MsgSack, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgSack, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgSack, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgSack, /* ESTABLISHED      State */
    },

    /* 10    CMD-USERDATA */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        NULL,                            /* SHUTOWN-PENDING  State */
        NULL,                            /* SHUTOWN-RECEIVE  State */
        NULL,                            /* SHUTOWN-SENT     State */
        NULL,                            /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_CmdUserData, /* ESTABLISHED      State */
    },

    /* 11 EVT-TIMERDATA */
    {
        NULL,                             /* CLOSE            State */
        NULL,                             /* COOKIE-WAIT      State */
        NULL,                             /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_EvtTimerData, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_EvtTimerData, /* SHUTOWN-RECEIVE  State */
        NULL,                             /* SHUTOWN-SENT     State */
        NULL,                             /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_EvtTimerData, /* ESTABLISHED      State */
    },

    /* 12 EVT-TIMERHB */
    {
        NULL,                           /* CLOSE            State */
        NULL,                           /* COOKIE-WAIT      State */
        NULL,                           /* COOKIE-ECHOED    State */
/*Bug Id 199 changes s*/
        sctpfsm_assoc_Shut_EvtTimerHB,  /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Shut_EvtTimerHB,  /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Shut_EvtTimerHB,  /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Shut_EvtTimerHB,  /* SHUTOWN-ACK-SENT State */
/*Bug Id 199 changes e*/
        sctpfsm_assoc_Estbl_EvtTimerHB, /* ESTABLISHED      State */
    },

    /* 13 EVT-TIMERINACTIVE    */
    {
        NULL,                             /* CLOSE            State */
        NULL,                             /* COOKIE-WAIT      State */
        NULL,                             /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_EvtTimerInac, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_EvtTimerInac, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_EvtTimerInac, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_EvtTimerInac, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_EvtTimerInac, /* ESTABLISHED      State */
    },

    /* 14 MSG-DATA */
    {
        sctpfsm_assoc_Close_MsgData, /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgData, /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgData, /* COOKIE-ECHOED    State */
        sctpfsm_assoc_ShutP_MsgData, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_MsgData, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_ShutS_MsgData, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgData, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgData, /* ESTABLISHED      State */
    },

    /* 15 MSG-INIT */
    {
        NULL,                          /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgInit,   /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgInit,   /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Restart_MsgInit, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Restart_MsgInit, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Restart_MsgInit, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_ShutA_MsgInit,   /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Restart_MsgInit, /* ESTABLISHED      State */
    },

    /* 16 MSG-HEARTBEATACK*/
    {
        NULL,                         /* CLOSE            State */
        NULL,                         /* COOKIE-WAIT      State */
        NULL,                         /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_MsgHBAck, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_MsgHBAck, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgHBAck, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgHBAck, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_MsgHBAck, /* ESTABLISHED      State */
    },


    /* 17 MSG-COOKIE */
    {
        NULL,                          /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgCookie, /* COOKIE-WAIT      State */
        sctpfsm_assoc_Gener_MsgCookie, /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_MsgCookie, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_MsgCookie, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgCookie, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgCookie, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_MsgCookie, /* ESTABLISHED      State */
    },

    /* 18 EVT-NEWPMTU */
    {
        NULL,                           /* CLOSE            State */
        NULL,                           /* COOKIE-WAIT      State */
        NULL,                           /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_EvtNewPmtu, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_EvtNewPmtu, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_EvtNewPmtu, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_EvtNewPmtu, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_EvtNewPmtu, /* ESTABLISHED      State */
    },

    /* 19  EVT-TIMERPMTU */
    {
        NULL,                              /* CLOSE              State */
        NULL,                              /* COOKIE-WAIT        State */
        NULL,                              /* COOKIE-ECHOED      State */
        sctpfsm_assoc_Gener_EvtTimerPmtu,  /* SHUTOWN-PENDING    State */
        sctpfsm_assoc_Gener_EvtTimerPmtu,  /* SHUTOWN-RECEIVE    State */
        sctpfsm_assoc_Gener_EvtTimerPmtu,  /* SHUTOWN-SENT       State */
        sctpfsm_assoc_Gener_EvtTimerPmtu,  /* SHUTOWN-ACK-SENT   State */
        sctpfsm_assoc_Estbl_EvtTimerPmtu,  /* ESTABLISHED        State */
    },

    /*    20 EVT-TIMERDEALYACK */
    {
        NULL,                           /* CLOSE            State */
        NULL,                           /* WAIT             State */
        NULL,                           /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_TimerDeAck, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_TimerDeAck, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_TimerDeAck, /* SHUTOWN-SENT     State */
        NULL,                           /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_TimerDeAck, /* ESTABLISHED      State */
    },

    /* 21EVT-TIMERLIFETIME */
    {
        NULL,                           /* CLOSE            State */
        NULL,                           /* COOKIE-WAIT      State */
        NULL,                           /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_TimerLtime, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_TimerLtime, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_TimerLtime, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Estbl_TimerLtime, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_TimerLtime, /* ESTABLISHED      State */
    },

    /*22    CMD-CONNECT */
    {
        sctpfsm_assoc_Close_CmdConnect, /* CLOSE             State */
        NULL,                           /* COOKIE-WAIT       State */
        NULL,                           /* COOKIE-ECHOED     State */
        NULL,                           /* SHUTOWN-PENDING   State */
        NULL,                           /* SHUTOWN-RECEIVE   State */
        NULL,                           /* SHUTOWN-SENT      State */
        NULL,                           /* SHUTOWN-ACK-SENT  State */
        NULL,                           /* ESTABLISHED       State */
    },

    /* 23    MSG-STALECOOKIE */
    {
        sctpfsm_assoc_Gener_MsgStaleErr, /* CLOSE            State */
        sctpfsm_assoc_Gener_MsgStaleErr, /* COOKIE-WAIT      State */
        sctpfsm_assoc_CookE_MsgStaleErr, /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_MsgStaleErr, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_MsgStaleErr, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_MsgStaleErr, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_MsgStaleErr, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_MsgStaleErr,  /* ESTABLISHED     State */
    },


    /* 24    MSG-ECNE */
    {
        NULL,                        /* CLOSE             State */
        NULL,                        /* COOKIE-WAIT       State */
        NULL,                        /* COOKIE-ECHOED     State */
        sctpfsm_assoc_Estbl_MsgEcne, /* SHUTOWN-PENDING   State */
        sctpfsm_assoc_Estbl_MsgEcne, /* SHUTOWN-RECEIVE   State */
        sctpfsm_assoc_Estbl_MsgEcne, /* SHUTOWN-SENT      State */
        sctpfsm_assoc_Estbl_MsgEcne, /* SHUTOWN-ACK-SENT  State */
        sctpfsm_assoc_Estbl_MsgEcne, /* ESTABLISHED       State */
    },

    /* 25    MSG-CWR */
    {
        NULL,                        /* CLOSE            State */
        NULL,                        /* COOKIE-WAIT      State */
        NULL,                        /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_MsgCwr,  /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_MsgCwr,  /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_MsgCwr,  /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Estbl_MsgCwr,  /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgCwr,  /* ESTABLISHED      State */
    },

    /* 26 TIMER-ECNE */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_EvtTimerEcn, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_EvtTimerEcn, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_EvtTimerEcn, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Estbl_EvtTimerEcn, /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_EvtTimerEcn, /* ESTABLISHED      State */
    },

    /* 27 MSG-SHUT-COMPLETE */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        NULL,                            /* SHUTOWN-PENDING  State */
        NULL,                            /* SHUTOWN-RECEIVE  State */
        NULL,                            /* SHUTOWN-SENT     State */
        sctpfsm_assoc_ShutA_MsgShutCom,  /* SHUTOWN-ACK-SENT State */
        NULL,                            /* ESTABLISHED      State */
    },

    /* 28 EVENT_TIMER-BUNDLE */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_TimerBundle, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_TimerBundle, /* SHUTOWN-RECEIVE  State */
        NULL,                            /* SHUTOWN-SENT     State */
        NULL,                            /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_TimerBundle, /* ESTABLISHED      State */
    },

    /* 29 EVENT_TIMER-LINGER */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Gener_TimerLinger, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Gener_TimerLinger, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Gener_TimerLinger, /* SHUTOWN-SENT     State */
        sctpfsm_assoc_Gener_TimerLinger, /* SHUTOWN-ACK-SENT State */
        NULL,                            /* ESTABLISHED      State */
    },

    /* 30 EVENT_TIMER-AUTOCLOSE */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        NULL,                            /* SHUTOWN-PENDING  State */
        NULL,                            /* SHUTOWN-RECEIVE  State */
        NULL,                            /* SHUTOWN-SENT     State */
        NULL,                            /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Gener_TimerAutoclose, /* ESTABLISHED      State */
    },
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    /* 31 CMD-ASCONF */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_CmdAsconf,   /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_CmdAsconf,   /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_CmdAsconf,   /* SHUTOWN-SENT     State */
        NULL,                            /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_CmdAsconf, /* ESTABLISHED      State */
    },
    /* 32 TIMER_ASCONF */
    {
        NULL,                               /* CLOSE            State */
        NULL,                               /* COOKIE-WAIT      State */
        NULL,                               /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_EvtTimerAsconf, /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_EvtTimerAsconf, /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_EvtTimerAsconf, /* SHUTOWN-SENT     State */
        NULL,                               /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_EvtTimerAsconf, /* ESTABLISHED      State */
    },
    /* 33 MSG-ASCONF */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_MsgAsconf,   /* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_MsgAsconf,   /* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_MsgAsconf,   /* SHUTOWN-SENT     State */
        NULL,                            /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgAsconf,   /* ESTABLISHED      State */
    },
    /* 34 MSG-ASCONFACK */
    {
        NULL,                            /* CLOSE            State */
        NULL,                            /* COOKIE-WAIT      State */
        NULL,                            /* COOKIE-ECHOED    State */
        sctpfsm_assoc_Estbl_MsgAsconfAck,/* SHUTOWN-PENDING  State */
        sctpfsm_assoc_Estbl_MsgAsconfAck,/* SHUTOWN-RECEIVE  State */
        sctpfsm_assoc_Estbl_MsgAsconfAck,/* SHUTOWN-SENT     State */
        NULL,                            /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_MsgAsconfAck,/* ESTABLISHED      State */
    },
#endif
    /* SPR 6854 */
    /* 31, 35 TIMER-LOCALHBEAT */
    {
        NULL,                              /* CLOSE            State */
        NULL,                              /* COOKIE-WAIT      State */
        NULL,                              /* COOKIE-ECHOED    State */
        NULL,                              /* SHUTOWN-PENDING  State */
        NULL,                              /* SHUTOWN-RECEIVE  State */
        NULL,                              /* SHUTOWN-SENT     State */
        NULL,                              /* SHUTOWN-ACK-SENT State */
        sctpfsm_assoc_Estbl_EvtTimerLHbeat,/* ESTABLISHED      State */
    }
    /* SPR 6854 */
};

/*
 * Transmit State Machine Function Pointer Table
 */

void
( *sctp_tx_fsm_ftbl[SCTP_TX_EVENT_NUM][SCTP_TX_STATE_NUM] )(
    sctp_tcb_st   *p_ass,
    sctp_U32bit   addr_index ) =
{
    /* NULL                     AWTAIT_ACK                                  */
    {sctp_txfsm_Gener_Sack,     sctp_txfsm_Gener_Sack,},      /* SACK       */
    {sctp_txfsm_Null_Userdata,  sctp_txfsm_Awack_Userdata,},  /* USER-DATA  */
    {sctp_txfsm_Null_TimerData, sctp_txfsm_Awack_TimerData,}, /* TIMER-DATA */
};


/*
 * Receive State Machine Function Pointer Table
 */

void
( *sctp_rx_fsm_ftbl[SCTP_RX_EVENT_NUM][SCTP_RX_STATE_NUM] )(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index ) =
{
    /*     NULL                SACK_PENDING                                   */
    {sctp_rxfsm_Null_MsgData,  sctp_rxfsm_SackP_MsgData,},   /* MSG-DATA      */
    {NULL,                     sctp_rxfsm_SackP_TimerDeack,},/* TIMER-DELAYACK*/
    {sctp_rxfsm_Null_UserData, sctp_rxfsm_SackP_UserData,},  /* USER-Data     */
};





