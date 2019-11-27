/*! \file   son_mif_msg_handler.h
 *  \brief  This file contains the declarations and function prototypes
 *          for SONMgmtIfH module
 *  \date   January 13, 2012\Jan 3 2013
 *  \author Ritika Chopra,mukesh
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_MIF_MSG_HANDLER_H_
#define _SON_MIF_MSG_HANDLER_H_

/*
 * Header File Includes
 */

/*! \headerfile son_defines.h <> */
#include <son_defines.h>

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*
 * Extern definitions
 */

/*
 * Macro Definitions
 */

/*
 * Macros to maintain the status of receipt of messages from other modules
 */

/*! \def    SMIF_SONNMM_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONnmm
 */
#define SMIF_SONNMM_INIT_RECD           0x01   /* 0 0 0 0 0 0 0 1 */

/*! \def    SMIF_SONAPPS_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONapps
 */
#define SMIF_SONAPPS_INIT_RECD          0x02   /* 0 0 0 0 0 0 1 0 */

/*! \def    SMIF_SONANR_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONanr
 */
#define SMIF_SONANR_INIT_RECD          0x04   /* 0 0 0 0 0 1 0 0 */

/*! \def    SMIF_SONES_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONes
 */
#define SMIF_SONES_INIT_RECD          0x08   /* 0 0 0 0 1 0 0 0 */

/*! \def    SMIF_SONX2_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONX2
 */
#define SMIF_SONX2_INIT_RECD          0x10   /* 0 0 0 1 0 0 0 0 */

/*! \def    SMIF_SONMRO_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONMRO
 */
#define SMIF_SONMRO_INIT_RECD          0x20   /* 0 0 1 0 0 0 0 0 */

/*! \def    SMIF_SONMLB_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SONmlb
 */
#define SMIF_SONMLB_INIT_RECD          0x40   /* 0 1 0 0 0 0 0 0 */

/* RACH OPT CHANGES : STARTS */
/*! \def    SMIF_SONRCH_OPT_INIT_RECD
 *  \brief  To set flag on receipt of init indication from SON RACH OPT
 */
#define SMIF_SONRACH_OPT_INIT_RECD     0x80   /* 1 0 0 0 0 0 0 0 */
/* RACH OPT CHANGES : ENDS */

/*! \def    SMIF_SONNMM_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONnmm
 */
#define SMIF_SONNMM_INIT_CONFIG_RESP_RECD     0x01   /* 0 0 0 0 0 0 0 1 */

/*! \def    SMIF_SONAPPS_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONapps
 */
#define SMIF_SONAPPS_INIT_CONFIG_RESP_RECD    0x02   /* 0 0 0 0 0 0 1 0 */

/*! \def    SMIF_SONANR_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONanr
 */
#define SMIF_SONANR_INIT_CONFIG_RESP_RECD    0x04   /* 0 0 0 0 0 1 0 0 */

/*! \def    SMIF_SONES_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONes
 */
#define SMIF_SONES_INIT_CONFIG_RESP_RECD    0x08   /* 0 0 0 0 1 0 0 0 */

/*! \def    SMIF_SONX2_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONX2
 */
#define SMIF_SONX2_INIT_CONFIG_RESP_RECD    0x10   /* 0 0 0 1 0 0 0 0 */

/*! \def    SMIF_SONMRO_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONMRO
 */
#define SMIF_SONMRO_INIT_CONFIG_RESP_RECD    0x20   /* 0 0 1 0 0 0 0 0 */

/*! \def    SMIF_SONMLB_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SONmlb
 */
#define SMIF_SONMLB_INIT_CONFIG_RESP_RECD    0x40   /* 0 1 0 0 0 0 0 0 */

/* RACH OPT CHANGES : STARTS */
/*! \def    SMIF_SONRACH_OPT_INIT_CONFIG_RESP_RECD
 *  \brief  To set flag on receipt of active indication from SON RACH OPT 
 */
#define SMIF_SONRACH_OPT_INIT_CONFIG_RESP_RECD 0x80 /* 1 0 0 0 0 0 0 0 */
/* RACH OPT CHANGES : ENDS */

/*! \def    SMIF_SONNMM_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONnmm
 */
#define SMIF_SONNMM_SHUTDOWN_RECD       0x01   /* 0 0 0 0 0 0 0 1 */

/*! \def    SMIF_SONAPPS_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONapps
 */
#define SMIF_SONAPPS_SHUTDOWN_RECD      0x02   /* 0 0 0 0 0 0 1 0 */

/*! \def    SMIF_SONANR_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONanr
 */
#define SMIF_SONANR_SHUTDOWN_RECD       0x04   /* 0 0 0 0 0 1 0 0 */

/*! \def    SMIF_SONES_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONes
 */
#define SMIF_SONES_SHUTDOWN_RECD       0x08   /* 0 0 0 0 1 0 0 0 */

/*! \def    SMIF_SONX2_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONX2
 */
#define SMIF_SONX2_SHUTDOWN_RECD       0x10   /* 0 0 0 1 0 0 0 0 */

/*! \def    SMIF_SONMRO_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONMRO
 */
#define SMIF_SONMRO_SHUTDOWN_RECD       0x20   /* 0 0 1 0 0 0 0 0 */

/*! \def    SMIF_SONMLB_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONMLB
 */
#define SMIF_SONMLB_SHUTDOWN_RECD       0x40  /* 0 1 0 0 0 0 0 0 */

/* RACH OPT CHANGES : STARTS */
/*! \def    SMIF_SONRACH_OPT_SHUTDOWN_RECD
 *  \brief  To set flag on receipt of shutdown response from SONRACH OPT
 */
#define SMIF_SONRACH_OPT_SHUTDOWN_RECD  0x80   /* 1 0 0 0 0 0 0 0 */
/* RACH OPT CHANGES : ENDS */

/* Rel 3.0 Cell delete support changes Start */
/*! \def    SMIF_SONANR_CELL_DELETE_RESP_RECD
 *  \brief  To set flag on receipt of cell delete resp from SONanr
 */
#define SMIF_SONANR_CELL_DELETE_RESP_RECD   0x01   /* 0 0 0 0 0 0 0 1 */

/*! \def    SMIF_SONES_CELL_DELETE_RESP_RECD
 *  \brief  To set flag on receipt of cell delete resp from SONes
 */
#define SMIF_SONES_CELL_DELETE_RESP_RECD    0x02   /* 0 0 0 0 0 0 1 0 */

/*! \def    SMIF_SONMLB_CELL_DELETE_RESP_RECD
 *  \brief  To set flag on receipt of cell delete resp from SONmlb
 */
#define SMIF_SONMLB_CELL_DELETE_RESP_RECD   0x04   /* 0 0 0 0 0 1 0 0 */

/*! \def    SMIF_SONMRO_CELL_DELETE_RESP_RECD
 *  \brief  To set flag on receipt of cell delete resp from SONmro
 */
#define SMIF_SONMRO_CELL_DELETE_RESP_RECD   0x08   /* 0 0 0 0 1 0 0 0 */

/*! \def    SMIF_SONRACH_OPT_CELL_DELETE_RESP_RECD
 *  \brief  To set flag on receipt of cell delete resp from SON RACH-OPT 
 */
#define SMIF_SONRACH_OPT_CELL_DELETE_RESP_RECD 0x10 /* 0 0 0 1 0 0 0 0 */

/* Rel 3.0 Cell delete support changes Stop */
/* SPR_19619 start */
#define SMIF_SONES_PEER_CELL_ACTIVATION_RESP_RECD    0x20   /* 0 0 0 0 0 0 1 0 */
/* SPR_19619 stop */

/*
 * Enumeration Definitions
 */

/*! \enum   smif_event_et
 *  \brief  Events for SON MgmtIfH FSM
 */
typedef enum
{
    SMIF_EVENT_SONNMM_INIT_IND,
    SMIF_EVENT_SONAPPS_INIT_IND,
    SMIF_EVENT_SONANR_INIT_IND,
    SMIF_EVENT_SONES_INIT_IND,
    SMIF_EVENT_SONX2_INIT_IND,
#ifdef SON_MRO_FEATURE
    SMIF_EVENT_SONMRO_INIT_IND,
#endif
    SMIF_EVENT_SONMLB_INIT_IND,
/* RACH OPT CHANGES : STARTS */
    SMIF_EVENT_SONRACH_OPT_INIT_IND,
/* RACH OPT CHANGES : ENDS */

    SMIF_EVENT_OAM_INIT_CNF,
    SMIF_EVENT_SONNMM_INIT_CONFIG_RESP,
    SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP,
    SMIF_EVENT_SONANR_INIT_CONFIG_RESP,
    SMIF_EVENT_SONES_INIT_CONFIG_RESP,
    SMIF_EVENT_SONX2_INIT_CONFIG_RESP,
#ifdef SON_MRO_FEATURE
    SMIF_EVENT_SONMRO_INIT_CONFIG_RESP,
#endif
    SMIF_EVENT_SONMLB_INIT_CONFIG_RESP,
/* RACH OPT CHANGES : STARTS */
    SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP,
/* RACH OPT CHANGES : ENDS */

    SMIF_EVENT_SHUTDOWN_REQ,
    SMIF_EVENT_SONNMM_SHUTDOWN_RES,
    SMIF_EVENT_SONAPPS_SHUTDOWN_RES,
    SMIF_EVENT_SONANR_SHUTDOWN_RES,
    SMIF_EVENT_SONES_SHUTDOWN_RES,
    SMIF_EVENT_SONX2_SHUTDOWN_RES,
#ifdef SON_MRO_FEATURE
    SMIF_EVENT_SONMRO_SHUTDOWN_RES,
#endif
    SMIF_EVENT_SONMLB_SHUTDOWN_RES,
/* RACH OPT CHANGES : STARTS */
    SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RES,
/* RACH OPT CHANGES : ENDS */

    /* SPR-11286 Fix Starts */
    /* Lines deleted */
    /* SPR-11286 Fix Ends */
    /* Rel 3.0 Cell delete support changes Start */
    SMIF_EVENT_SONANR_CELL_DELETE_RES,
    SMIF_EVENT_SONES_CELL_DELETE_RES,
#ifdef SON_MRO_FEATURE
    SMIF_EVENT_SONMRO_CELL_DELETE_RES,
#endif
    SMIF_EVENT_SONMLB_CELL_DELETE_RES,
    SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES,
    /* Rel 3.0 Cell delete support changes Stop */

    /* SPR-13251 Fix Starts */
    SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES,
    SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES,
    SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES,
    SMIF_EVENT_SONES_GET_LOG_LEVEL_RES,
#ifdef SON_MRO_FEATURE
    SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES,
#endif
    SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES,
    SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES,
    /* SPR-13251 Fix Ends */
    /* SPR_19619 start */
    SMIF_EVENT_SONES_PEER_CELL_ACTIVATION_RES,
    /* SPR_19619 stop */

    SMIF_NUM_OF_EVENTS,
    SMIF_INVALID_EVENT
}smif_event_et;

typedef enum
{
    SMIF_NULL = 0,                   /*!< Start-up state */
    SMIF_STATE_INIT,                 /*!< Initialized state */
    SMIF_STATE_W_FOR_SHUTDOWN_RESP,  /*!< Waiting for Shutdown responses */
    SMIF_STATE_ACTIVE,               /*!< Active state */
    SMIF_NUM_OF_STATES               /*!< Number of states */
}smif_state_et;

/*! \struct son_nw_scan_plmn_data_t
 *  \brief  Structure to store Network scan PLMNs data
 *  \param transaction_id    Request transaction ID
 *  \param num_plmn_id       Number of PLMNs
 *  \param plmn_id           MCCs and MNCs
 */

typedef struct _son_nw_scan_plmn_data_t
{
    son_u16 transaction_id;
    son_u8 num_plmn_id;
    son_cell_plmn_info_t plmn_id[SON_MAX_NUM_PLMN];
}son_nw_scan_plmn_data_t;

/*! \struct son_nw_scan_plmn_node_t
 *  \brief                 Node to store the network scan PLMNs
 *  \param anchor          Link list starting pointer
 *  \param nw_scan_plmn    Structure to store PLMNs info
 */
typedef struct _son_nw_scan_plmn_node_t
{
    SON_LIST_NODE anchor;
    son_nw_scan_plmn_data_t nw_scan_plmn;
}son_nw_scan_plmn_node_t;

/*
 * Types Definitions
 */

/*! \ struct son_mif_gb_context_t
 *  \brief  Structure to store the global context of SON MgmtIfH
 *  \param  init_ind    store the receipt status of init ind messages from
 *          SONapps, SONnmm, SONanr and SONes. Bit-0 for SONnmm,
 *          Bit-1 for SONapps, Bit-3 for SONanr, Bit-4 for SONes
 *          Bit-5 for SONX2, Bit-6 for SONMRO, Bit-7 for SONMLB, Bit-8 for RACH OPT
 *  \param  init_config_resp_status  store the receipt status of init config
 *          resp messages from SONapps, SONnmm, SONanr and SONes. Bit-0 for
 *          SONnmm, Bit-1 for SONapps, Bit-3 for SONanr, Bit-4 for SONes
 *          Bit-5 for SONX2, Bit-6 for SONMRO, Bit-7 for SONMLB, Bit-8 for RACH
OPT
 *  \param  shutdown_status store the receipt status of shutdown resp messages
 *          from SONapps, SONnmm, SONanr and SONes. Bit-0 for SONnmm, Bit-1
 *          for SONapps, Bit-3 for SONanr, Bit-4 for SONes
 *          Bit-5 for SONX2, Bit-6 for SONMRO, Bit-7 for SONMLB, Bit-8 for RACH
OPT
 *  \param  cell_delete_resp_status the receipt status of cell delete resp 
 *          messages from SONapps, SONnmm, SONanr, SONes and SON Rach-opt. 
 *          Bit-0 for SONANR, Bit-1 for SONes, Bit-3 for SONmlb, 
 *          Bit-4 for SONmro, Bit-5 for SON Rach-opt.
 *  \param  current_state The current state of OAM FSM
 *  \param  oam_init_ind_duration Time duration for the timer of sending
 *          SON_OAM_INIT_IND to OAM
 *  \param  oam_init_ind_timer    Timer ID of init indication timer
 *  \param  son_shutdown_timer    Timer ID of shutdown timer
 *  \param  nw_scan_plmn_list     Network scan PLMNs list
 *  \param  son_health_monitoring_timer_prd Timer ID of health monitoring
 *  \param  cell_delete_req_in_process Flag storing if Cell delete request 
 *          is already in progress.
 *  \param  cell_delete_res       Structure to store the cell delete response
 *          to be sent to OAM (this is used to consolidate response from 
 *          all SON modules)
 *  \param smif_log_level         SMIF log level
 *  \param get_log_level_resp     Structure to store the get log level 
 *         response to be sent to OAM (this is used to consolidate response 
 *         from all SON modules)
 *  \param  get_log_level_req_in_process Flag set to 1 if the get log level request handling from OAM is in progress
 *  \param expected_get_log_level_res Total number of expected get log level response messages from SON internal modules
 */
typedef struct _son_mif_gb_context_t
{
    U8              init_ind;
    U8              init_config_resp_status;
    U8              shutdown_status;
    /* Rel 3.0 Cell delete support changes Start */
    U8              cell_delete_resp_status;
    /* Rel 3.0 Cell delete support changes Stop */
    smif_state_et   current_state;
    U16             oam_init_ind_duration;
    U16             oam_init_ind_retry_count;
    son_timer_t     oam_init_ind_timer;
    son_timer_t     son_shutdown_timer;
    SON_LIST        nw_scan_plmn_list;
    son_u16         son_health_monitoring_timer_prd;
    /* Rel 3.0 Cell delete support changes Start */
    U8                      cell_delete_req_in_process;
    son_cell_delete_resp_t  cell_delete_res;
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    U8                              smif_log_level;
    son_oam_get_log_level_resp_t    get_log_level_resp;
    U8                              get_log_level_req_in_process;
    U8                              expected_get_log_level_resp;
    /* SPR-13251 Fix Ends */
} son_mif_gb_context_t;

/*
 * Function Prototypes
 */

/*! \fn void* son_mif_init(void *p_init_param)
 *  \brief  This function initialises the SON MgmtIfH global context
 *  \param p_init_param
 */
void *
son_mif_init
(
    void *p_init_param
);

/*! \fn int son_mif_process_msg (void *p_api, void *p_gl_ctx)
 *  \brief  This is entry point function of the SON MgmtIfH module.
 *          This function receives messages from OAM agent and other
 *          SON modules.
 *  \param  p_api
 *  \param  p_gl_ctx
 */
S32
son_mif_process_msg
(
    void *p_api,
    void *p_gl_ctx
);

/*! \fn void son_mif_process_timer_msg( son_timer_t timer_id,
 *                                      void *p_timer_buf,
 *                                      void *p_gl_ctx);
 *  \brief This function is entry point function for timer expiry
 *         of init cnf msg from OAM
 *  \param timer_id
 *  \param p_timer_buf
 *  \param p_gl_ctx
 */
void
son_mif_process_timer_msg
(
    son_timer_t timer_id,
    void *p_timer_buf,
    void *p_gl_ctx
);

/*! \fn son_return_et son_oam_send_son_oam_init_ind(son_module_id_et  src_module_id,
 *                                                  U16  transaction_id)
 *  \brief  This function constructs and sends SON_OAM_INIT_IND message to OAM
 *  \param  src_module_id       Source module identifier
 *  \param  transaction_id      Interface transaction identifier
 */
son_return_et
son_oam_send_son_oam_init_ind
(
    son_module_id_et src_module_id,
    U16 transaction_id
);

/*! \fn son_return_et son_mif_init_config_res(son_void_t *p_msg,
 *                                      son_mif_gb_context_t *p_smif_gb_context,
 *                                      son_u16 event_id)
 *  \brief This function handles SMIF_INIT_CONFIG_RES from internal modules of
 *         SON and sends response to SON_OAM_INIT_CONFIG_RESP to OAM
 *  \param p_msg
 *  \param p_smif_gb_context
 *  \param event_id
 */
son_return_et
son_mif_init_config_res
(
    son_void_t *p_msg,
    son_mif_gb_context_t *p_smif_gb_context,
    son_u16 event_id
);
/* SPR_19619 start */
/*! \fn son_return_et son_mif_init_config_res(son_void_t *p_msg,
 *                                      son_mif_gb_context_t *p_smif_gb_context,
 *                                      son_u16 event_id)
 *  \brief This function handles SMIF_INIT_CONFIG_RES from internal modules of
 *         SON and sends response to SON_OAM_INIT_CONFIG_RESP to OAM
 *  \param p_msg
 *  \param p_smif_gb_context
 *  \param event_id
 */
son_return_et
son_mif_peer_cell_activation_res
(
    son_void_t *p_msg,
    son_mif_gb_context_t *p_smif_gb_context,
    son_u16 event_id
);

/* SPR_19619 stop */
/*! \fn son_void_t son_mif_oam_init_config_req (
 * son_void_t *p_api,
 * son_mif_gb_context_t   *p_smif_gb_context)
 *  \brief This function handles SON_OAM_INIT_CONFIG_REQ msg
 *  \param p_api
 *  \param p_smif_gb_context
 */
son_void_t
son_mif_oam_init_config_req
(
    son_void_t *p_api,
    son_mif_gb_context_t   *p_smif_gb_context
);

/* SPR_19619 start */

/*! \fn son_void_t son_mif_oam_peer_cell_activation_req (
 *   son_void_t *p_api,
 *   STACKAPIHDR cspl_hdr)
 *  \brief This function handles SON_OAM_PEER_CELL_ACTIVATION_REQ msg
 *  \param p_api
 *  \param cspl_hdr
 */
son_void_t
son_mif_oam_peer_cell_activation_req
(
 son_void_t *p_api,
 STACKAPIHDR cspl_hdr
 );

/* SPR_19619 stop */


son_void_t
son_mif_oam_mlb_enable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
);

son_void_t
son_mif_mlb_enable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
);
son_void_t
son_mif_oam_mlb_disable_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
);
son_void_t
son_mif_mlb_disable_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
);
son_void_t
son_mif_oam_mlb_modify_attributes_req
(
    son_void_t *p_api,
    STACKAPIHDR cspl_hdr
);
son_void_t
son_mif_oam_mlb_modify_atributes_res
(
    son_void_t *p_msg,
    STACKAPIHDR cspl_hdr
);

/* SPR-13251 Fix Starts */
/*! \fn son_void_t son_mif_oam_send_get_log_level_resp(son_transaction_id_t transaction_id, son_return_et result, son_error_et error_code, son_module_id_et module_id, son_u8 log_level)
 *  \brief This function sends SON_OAM_GET_LOG_LEVEL_RESP msg to OAM 
 *  \param transaction_id   Transaction ID for the response
 *  \param result           Result (SUCCESS/FAILURE)
 *  \param error_code       Error Code in case of failure
 *  \param module_id        Module Id
 *  \param log_level        Log Level of the module
 */
son_void_t
son_mif_oam_send_get_log_level_resp
(
    son_transaction_id_t    transaction_id,
    son_return_et           result,
    son_error_et            error_code,
    son_module_id_et        module_id,
    son_u8                  log_level
);
/* SPR-13251 Fix Ends */

#endif /* _SON_MIF_MSG_HANDLER_H__ */
