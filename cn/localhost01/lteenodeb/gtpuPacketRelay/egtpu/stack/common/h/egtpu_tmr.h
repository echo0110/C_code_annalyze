/***********************************************************************
**  FUNCTION:
**      Definitions for GTP-U timer related types and prototypes
************************************************************************
**
**  FILE NAME:
**      egtpu_tmr.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                    	Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#ifndef _EGTPU_TMR_DECL_
#define _EGTPU_TMR_DECL_

/* timer flags */
#define EGTPU_TIMER_OFF                0
#define EGTPU_TIMER_ON                 1
#define EGTPU_FWD_ADDR_FLAG            0xFF
/* timer type definitions */
#define EGTPU_ECHO_RSP_TMR             0

/* Macros for Timer(s) default retries */
#define EGTPU_ECHO_RSP_TMR_MAX_RETRIES 4

/* Macros for Timer(s) default duration */
#define EGTPU_ECHO_RSP_TMR_DURATION    60

/* size of data buffer that is provided to the timer server
   in START_TMR req and that shall be returned in TMR_EXP
   API to identify the peer ctxt rec */
    /* SPR 20430 Changes Start */
#ifdef __x86_64__
#define EGTPU_ECHO_TMR_BUFFER_SIZE     8
#else
    /* SPR 20430 Changes End */
#define EGTPU_ECHO_TMR_BUFFER_SIZE     4
    /* SPR 20430 Changes Start */
#endif
    /* SPR 20430 Changes End */

/** Function prototype for timer related functions **/
egtpu_return_t egtpu_get_timer_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_set_timer_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_get_max_retries_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_set_max_retries_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_modify_reordering_ie_req (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info);
#endif
