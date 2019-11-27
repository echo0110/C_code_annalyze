/*! \file son_nmmctl_fsm.h
 *  \brief This file contains the states events and definitions for NMM controller FSM.
 *  \date January 02, 2012
 *  \author Sandip Samal
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_NMMCTL_H_
#define _SON_NMMCTL_H_

/*! \headerfile son_nmm.h <>
 */
/* #include<son_nmm.h> */

/*! \headerfile son_nmm_apps_intf.h <>
 */
#include <son_nmm_apps_intf.h>

/*! \headerfile son_mif_intf.h <>
 */
#include <son_mif_intf.h>

/*! \headerfile son_oam_intf.h <>
 */

#include <son_oam_intf.h>

/*! \def TRANSACTION_ID_SIZE
 *  \brief Macro signifies the transation ID size.
 */

#define TRANSACTION_ID_SIZE 4

/*! \def EMPTY_LIST
 *  \brief A macro that store a integer value 0 and represents the empty queue.
 */

#define EMPTY_LIST 0

/*! \def PERIODIC_SCAN_OFF
 *  \brief A macro that store a integer value 0 and represents the periodic scan OFF status.
 */
#define PERIODIC_SCAN_OFF 0

/*! \def PERIODIC_SCAN_ON
 *  \brief A macro that store a integer value 1 and represents the periodic scan ON status.
 */
#define PERIODIC_SCAN_ON 1

/*! \def MAX_ALLOWED_SCAN
 *  \brief A macro defines the maximum number of scan allowed.
 */
#define MAX_ALLOWED_SCAN 6

/*! \def GET_PARAMLEN_FROM_CSPL_HDR
 *  \brief A macro specifies "p_running_start_scan->hdr.paramlen", use to obtain the parameter from CSPL header.
 */
#define GET_PARAMLEN_FROM_CSPL_HDR(p_running_start_scan)    (p_running_start_scan->hdr.paramlen)

/*! \def NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx)
 *  \brief A macro specifies "p_nmmctl_ctx->current_state", use to obtain the current state of NMM CTRL .
 */

#define NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx)      (p_nmmctl_ctx->current_state)

/*! \def SCANING_Q_INIT(p_scaning_queue)
 *  \brief A macro specifies "ylInit( p_scaning_queue )" ,use to scan the queue.
 */

#define SCANING_Q_INIT(p_scaning_queue) \
    ylInit( p_scaning_queue )

/*! \def SCANING_Q_GET_COUNT(p_scaning_queue)
 *  \brief A macro specifies "ylCount( p_scaning_queue )" ,use to count the enteries in queue.
 */

#define SCANING_Q_GET_COUNT(p_scaning_queue) \
    ylCount( p_scaning_queue )

/*! \def SCANING_Q_ENQUEUE(p_scaning_queue, p_scaning_record)
 *  \brief A macro specifies "ylPushTail( p_scaning_queue, &(p_scaning_record)->sNode )" ,use to scan the enqueue requests.
 */

#define SCANING_Q_ENQUEUE(p_scaning_queue, p_scaning_record) \
    ylPushTail( p_scaning_queue, &(p_scaning_record)->sNode )

/*! \def SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record)
 *  \brief A macro that specifies "{\
 *   YLNODE *snode = ylPopHead( p_scaning_queue );\
 *       p_scaning_record = YMEMBEROF( nmmctl_scan_record_t, sNode, snode );" ,use to scan the dequeue member.
 */

#define SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record){ \
        YLNODE *snode = ylPopHead( p_scaning_queue ); \
        p_scaning_record = YMEMBEROF( nmmctl_scan_record_t, sNode, snode ); \
}

/*! \enum nmmctl_fsm_state_et
 *  \brief An enum that specifies the NMMCTRL_FSM states.
 */

typedef enum
{
    NMMCTL_STATE_INIT,                       /* NMMCTL_FSM state 0 */
    NMMCTL_STATE_ACTIVE,                     /* NMMCTL_FSM state 1 */
    NMMCTL_STATE_W_PREPARE_RES,              /* NMMCTL_FSM state 2 */
    NMMCTL_STATE_W_SCAN_RES,                 /* NMMCTL_FSM state 3 */
    NMMCTL_STATE_W_COMPLT_RES,               /* NMMCTL_FSM state 4 */
    NMMCTL_STATE_W_STOP_RES,                 /* NMMCTL_FSM state 5 */
    NMMCTL_STATE_W_SHTDWN_RES,               /* NMMCTL_FSM state 6 */
    NMMCTL_MAX_STATE                         /* NMMCTL_FSM state 7 */
}nmmctl_fsm_state_et;

/*! \enum nmmctl_event_code_et
 *  \brief An enum that specifies the event codes
 */

typedef enum
{
    NMMCTL_SMIF_INIT_CONFIG_REQ_EV,          /* NMMCTL EVENT 0 */
    NMMCTL_SMIF_LOG_ENABLE_DISABLE_CMD_EV,   /* NMMCTL EVENT 1 */
    NMMCTL_SMIF_SET_LOG_LEVEL_CMD_EV,        /* NMMCTL EVENT 2 */
    NMMCTL_START_SCAN_REQ_EV,                /* NMMCTL EVENT 3 */
    NMMCTL_PERIODIC_SCAN_REQ_EV,             /* NMMCTL EVENT 4 */
    NMMCTL_SMIF_SET_NW_SCAN_CONFIG_CMD_EV,   /* NMMCTL EVENT 5 */
    NMMCTL_START_SCAN_RES_EV,                /* NMMCTL EVENT 6 */
    NMMCTL_STOP_SCAN_REQ_EV,                 /* NMMCTL EVENT 7 */
    NMMCTL_STOP_SCAN_RES_EV,                 /* NMMCTL EVENT 8 */
    NMMCTL_SCAN_PREPARE_RES_EV,              /* NMMCTL EVENT 9 */
    NMMCTL_SCAN_COMPLETE_RES_EV,             /* NMMCTL EVENT 10 */
    NMMCTL_SMIF_SHUTDOWN_REQ_EV,             /* NMMCTL EVENT 11 */
    /* SPR-13251 Fix Starts */
    NMMCTL_SMIF_GET_LOG_LEVEL_REQ_EV,        /* NMMCTL EVENT 12 */
    /* SPR-13251 Fix Ends */
    NMMCTL_MAX_EVENT                         /* NMMCTL EVENT 13 */
}nmmctl_event_code_et;

/*! \  struct nmmctl_start_scan_req_t
 *  \brief              Contains the parameters of start scan request including the CSPL header
 *  \param  hdr         Variable of type STACKAPIHDR
 *  \param  nmmctl_start_scan_req   variable for son_nmm_start_scan_req_t
 */

typedef struct _nmmctl_start_scan_req_t
{
    STACKAPIHDR hdr;
    son_nmm_start_scan_req_t nmmctl_start_scan_req;
    son_timer_t               scan_timer_id;
} nmmctl_start_scan_req_t;

/*! \  struct nmmctl_periodic_nw_scan_req_t
 *  \brief              Contains the parameters of periodic scan request including the CSPL header
 *  \param  hdr         Variable of type STACKAPIHDR
 *  \param  nmmctl_periodic_nw_scan_req  variable for nmmctl_periodic_nw_scan_req_t
 */

typedef struct _nmmctl_periodic_nw_scan_req_t
{
    STACKAPIHDR hdr;
    son_nmm_periodic_nw_scan_req_t nmmctl_periodic_nw_scan_req;
} nmmctl_periodic_nw_scan_req_t;

/*! \  struct nmmctl_start_scan_res_t
 *  \brief              Contains the parameters of start scan response including the CSPL header
 *  \param  scan_resp_data_valid  Variable defines that the last scan data is valid or not.
 *  \param  start_scan_res_data   Structure containg network scan response data
 */

typedef struct _nmmctl_start_scan_res_t
{
    U8 scan_resp_data_valid;
    start_scan_res_t start_scan_res_data;
} nmmctl_start_scan_res_t;

/*! \  struct nmmctl_stop_scan_req_t
 *  \brief              Contains the parameters of stop scan request including the CSPL header
 *  \param  hdr         Variable of type STACKAPIHDR
 *  \param  nmmctl_stop_scan_req   variable for son_nmm_stop_scan_req_t
 */

typedef struct _nmmctl_stop_scan_req_t
{
    STACKAPIHDR hdr;
    son_nmm_stop_scan_req_t nmmctl_stop_scan_req;
} nmmctl_stop_scan_req_t;

/*! \  struct nmmctl_stop_scan_res_t
 *  \brief              Contains the parameters of stop scan response including the CSPL header
 *  \param  hdr         Variable of type STACKAPIHDR
 *  \param  nmmctl_stop_scan_res   variable for son_nmm_stop_scan_res_t
 */

typedef struct _nmmctl_stop_scan_res_t
{
    STACKAPIHDR hdr;
    son_nmm_stop_scan_res_t nmmctl_stop_scan_res;
} nmmctl_stop_scan_res_t;

/*! \  struct nmmctl_scan_record_t
 *  \brief              Contains the parameters require to scan record
 *  \param  sNode       Variable of type YNODE
 *  \param  scan_data   variable for nmmctl_start_scan_req_t
 */

typedef struct _nmmctl_scan_record_t
{
    YLNODE sNode;
    nmmctl_start_scan_req_t scan_data;
} nmmctl_scan_record_t;

/*! \  struct nmm_start_scan_data_t
 *  \brief              Contains the parameters require to scan record
 *  \param  p_start_scan_req   pointer to network scan data
 */

typedef struct _nmm_start_scan_data_t
{
    start_scan_req_t    *p_start_scan_req;
} nmm_start_scan_data_t;

/*! \ struct nmmctl_set_scan_config_cmd_t
 *  \brief  Structure to send SMIF_SET_NW_SCAN_CONFIG_CMD msg from SON MgmtIfH to NMM.
 *  \param  force_scan Parameter telling a force full scan should be occured or not(TRUE/FALSE)
 *  \param  scan_time_interval     Interval between two scan
 */
typedef struct _nmmctl_set_scan_config_cmd_t
{
    U8 force_scan;
    U32 scan_time_interval;
}nmmctl_set_scan_config_cmd_t;

/*! \typedef son_oam_last_scan_timestamp_ind_t son_nmm_last_scan_timestamp_ind_t
 */
typedef son_oam_last_scan_timestamp_ind_t son_nmm_last_scan_timestamp_ind_t;

/*! \typedef son_oam_time_stamp_t son_nmm_time_stamp_t
 */
typedef son_oam_time_stamp_t son_nmm_time_stamp_t;
#endif /* ifndef _SON_NMMCTL_H_ */
