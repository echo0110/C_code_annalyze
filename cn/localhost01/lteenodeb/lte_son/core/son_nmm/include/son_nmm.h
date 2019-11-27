/*! \file son_nmm.h
 *  \brief This file contains structures/definitions of NMM module.
 *  \date Feb 01, 2012
 *  \author Ssamal
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_NMM_H_
#define _SON_NMM_H_

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \headerfile son_nmmctl_fsm.h <>
 */
#include <son_nmmctl_fsm.h>

/*! \def NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, new_state)
 *  \brief A macro that is used to set the state of NMM CTRL.
 */

#define NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, new_state) \
    do \
    { \
        p_nmmctl_ctx->current_state = new_state; \
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
            "NMM controller is in %s state", NMMCTL_FSM_STATES_NAMES[new_state]); \
    } \
    while (0)

/*! \def SCAN_TIME_INTERVAL
 *  \brief Macro for scan time interval.
 */
#define SCAN_TIME_INTERVAL 7200000 /* 2hr/120min */

/*! \enum nmm_submodules_et
 *  \brief An enum that specifies the NMM SUBMODULES.
 */

typedef enum
{
    SONNMM_SUBMODULE_NMM,               /*!<  NMM SUBMODULE 0 */
    SONNMM_SUBMODULE_NMMCTL,            /*!<  NMM SUBMODULE 1 */
    SONNNM_SUBMODULE_SMIF,              /*!<  NMM SUBMODULE 2 */
    SONNMM_SUBMODULE_INVALID            /*!<  NMM SUBMODULE 3 */
} nmm_submodules_et;

/*! \  struct nmm_timer_buf_t
 *  \brief Contains the nmm timer data.
 *  \param auto_delete
 *  \param target_submodule
 */

typedef struct _nmm_timer_buf_t
{
    son_bool_et auto_delete;
    nmm_submodules_et target_submodule;
} nmm_timer_buf_t;

/*! \var typedef YLIST nmmctl_scan_queue_t
 *  \brief Represent the list for scan structure
 */

typedef YLIST nmmctl_scan_queue_t;

/*! \  struct nmmctl_ctx_t
 *  \brief  Caintains the global context data for NMM controller.
 *  \param  nmm_scan_config_param  Represents the network scan config parameters
 *  \param  nmmctl_periodic_scan_timer_buf  Defines periodic scan timer data.
 *  \param  nmmctl_scan_resp_timer_buf      Defines scan response timer data.
 *  \param new_scan_req   Whether any new scan request is to be processed from the local queue
 *  \param current_state  NMM_FSM current state
 *  \param scaning_queue  List of NMM controller pending scan requests
 */

typedef struct _nmmctl_ctx_t
{
    nmmctl_set_scan_config_cmd_t nmm_scan_config_param;
    nmm_timer_buf_t nmmctl_periodic_scan_timer_buf;
    nmm_timer_buf_t nmmctl_scan_resp_timer_buf;
    son_bool_et new_scan_req;
    nmmctl_fsm_state_et current_state;
    nmmctl_scan_queue_t scaning_queue;
}nmmctl_ctx_t;

/*! \  struct nmm_gl_ctx_t
 *  \brief   Contains global context data for NMM module
 *  \param *p_nmmctl_ctx           NMMCTL context
 */

typedef struct _nmm_gl_ctx_t
{
    nmmctl_ctx_t                 *p_nmmctl_ctx;
} nmm_gl_ctx_t;

/*! \  struct nmm_start_scan_req_t
 *  \brief Contains the parameters required for starting the scan request.
 *  \param *start_scan_req   It is a variable for structure "start_scan_req_t"
 */

typedef struct  _nmm_start_scan_req_t
{
    start_scan_req_t    *p_start_scan_req;
} nmm_start_scan_req_t;

/*! \  struct nmm_start_scan_res_t
 *  \brief  Contains the parameters require for getting the scan response from NMM.
 *  \param start_scan_res It is a variable for structure "start_scan_res_t"
 */

typedef struct _nmm_start_scan_res_t
{
    start_scan_res_t start_scan_res;
} nmm_start_scan_res_t;

/*! \fn  void son_nmmctl_fsm_process_msg(void *p_api_buf, nmm_gl_ctx_t *p_nmm_gl_ctx)
 *  \brief This function act as entry function for NMM CONTROLLER
 *  \param *p_api_buf Pointer to the API MESSAGE buffer
 *  \param *p_nmm_gl_ctx Pointer to the NMM global context data.
 */

void
son_nmmctl_fsm_process_msg
(
    void *p_api,
    nmm_gl_ctx_t *p_nmm_gl_ctx
);

/*! \fn  void son_nmm_fsm_process_msg(void *p_msg, nmm_gl_ctx_t *p_nmm_gl_ctx)
 *  \brief This function act as entry function for NMM
 *  \param *p_msg Pointer to the API MESSAGE buffer
 *  \param *p_nmm_gl_ctx Pointer to the NMM global context data.
 */

void
son_nmm_fsm_process_msg
(
    void *p_msg
    /*SPR 17777 +-*/
);

/*! \fn void son_nmm_time_out_handler(son_timer_t  timer_id, nmm_timer_buf_t  *p_nmm_timer_buf, nmm_gl_ctx_t  *p_nmm_gl_ctx)
 *  \brief This function handler the event when the timer expires.
 *  \param timer_id  It is a variable for structure "son_timer_t"
 *  \param *p_nmm_timer_buf  It is a variable for structure "nmm_timer_buf_t"
 *  \param *p_nmm_gl_ctx    It is a variable for structure "nmm_gl_ctx_t"
 */

void
son_nmm_time_out_handler
(
    son_timer_t timer_id
    /*SPR 17777 +-*/
);

/*! \fn void son_nmmctl_time_out_handler(son_timer_t  timer_id, nmm_timer_buf_t *p_nmm_timer_buf, nmm_gl_ctx_t  *p_nmm_gl_ctx)
 *  \brief This function handler the event when the timer expires.
 *  \param timer_id  It is a variable for structure "son_timer_t"
 *  \param *p_nmm_timer_buf  It is a variable for structure "nmm_timer_buf_t"
 *  \param *p_nmm_gl_ctx    It is a variable for structure "nmm_gl_ctx_t"
 */

void
son_nmmctl_time_out_handler
(
    son_timer_t timer_id,
    /*SPR 17777 +-*/
    nmm_gl_ctx_t     *p_nmm_gl_ctx
);

/*! \fn void nmmctl_prepare_periodic_scan_data();
 *  \brief This function prepares the periodic  scan info when triggered from OAM.
 */

void *
nmmctl_prepare_periodic_scan_data
(
/* + 17439 */
void
/* - 17439 */
);

/*! \ U8 g_nmm_log_enable;
 *  \brief This stores the information about log enable/disable status.
 */
extern U8 g_nmm_log_enable;

/* SPR-13251 Fix Starts */
/*! \ U8 g_nmm_log_level
 *  \brief This stores the information about log level
 */
extern U8 g_nmm_log_level;
/* SPR-13251 Fix Ends */

/*! \ son_8 p_son_nmm_facility_name;
 *  \brief This stores the info for the son facility name.
 */
extern const son_8 *p_son_nmm_facility_name;

/*! \ S8 *NMMCTL_FSM_STATES_NAMES;
 *  \brief This stores the NMM controller FSM State names.
 */

extern const S8 *NMMCTL_FSM_STATES_NAMES[];

/*! \var typedef S32 (*message)(void *, void *)
 *  \brief Represent the message for scan structure
 */
typedef S32 (*message)
(
    void *,
    void *
);

/*! \var typedef void (*timer)(QTIMER, void *, void *)
 *  \brief Represent the timer.
 */
typedef void (*timer)
(
    QTIMER,
    void *,
    void *
);

/*! \fn void son_nmm_fill_time_stamp(son_nmm_time_stamp_t *p_time_stamp)
 *  \brief This function fills the time stamp.
 *  \param *p_time_stamp is a variable for structure "son_nmm_time_stamp_t"
 */
void
son_nmm_fill_time_stamp
(
    son_nmm_time_stamp_t *p_time_stamp
);

#endif /* ifndef _SON_NMM_H_ */
