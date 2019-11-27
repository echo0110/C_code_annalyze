/*! \file   son_mif_intf.h
 *  \brief  This file contains the API structures for communicaton
 *          of SONMgmtIfH module with other SON internal modules
 *	    (APPS, NMM, ES, ANR)
 *  \date   January 13, 2012
 *  \author Ritika Chopra
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_MIF_INTF_H_
#define _SON_MIF_INTF_H_

/*
 * Header File Includes
 */

/*! \headerfile son_defines.h <> */
#include <son_defines.h>
/*! \headerfile son_oam_intf.h <> */
#include <son_oam_intf.h>

/*! \  son_oam_cell_info_ind_t smif_cell_info_ind_t;
 */
typedef son_oam_cell_info_ind_t smif_cell_info_ind_t;

/*! \ struct smif_init_config_req_t
 *  \brief  Structure to send SMIF_INIT_CONFIG_REQ msg from SON MgmtIfH to SON internal modules (NMM, APPS, ES, ANR)
 *  \param  transaction_id	Transaction Identifier
 *  \param  log_enable		Specifies whether to enable/disable logging
 *  \param  log_level		Log level
 */
typedef struct _smif_init_config_req_t
{
    U16 transaction_id;
    son_oam_log_on_off_et log_enable;
    son_oam_log_level_et log_level;
    /*SPR-12944-Start*/
    son_bool_et is_nmm_supported;
    /*SPR-12944-End*/
}smif_init_config_req_t;

/*! \ struct smif_init_config_res_t
 *  \brief  Structure to send SMIF_INIT_CONFIG_RES msg from SON internal modules (NMM, APPS, ES, ANR) to SON MgmtIfH
 *  \param  transaction_id      Transaction Identifier
 *  \param  result		Success/Failure
 *  \param  error_code		Cause for failure
 */
typedef struct _smif_init_config_res_t
{
    U16 transaction_id;
    son_return_et result;
    son_error_et error_code;
}smif_init_config_res_t;

/* SPR_19619 start */

/*! \ struct smif_init_config_res_t
 *  \brief  Structure to send SMIF_PEER_CELL_ACTIVATION_RES msg from SON internal module ES to SON MgmtIfH
 *  \param  transaction_id      Transaction Identifier
 *  \param  result		Success/Failure
 *  \param  error_code		Cause for failure
 */
typedef struct _smif_peer_cell_activation_res_t
{
    U16 transaction_id;
    son_return_et result;
    son_error_et error_code;
}smif_peer_cell_activation_res_t;

/* SPR_19619 stop */

/*! \ struct smif_set_nw_scan_config_cmd_t
 *  \brief  Structure to send SMIF_SET_NW_SCAN_CONFIG_CMD msg from SON MgmtIfH to NMM.
 *  \param  transaction_id    Transaction Identifier
 *  \param  force_scan_on_off Force scan ON/OFF
 *  \param  scan_time_interval     Interval between two scan
 */
typedef struct _smif_set_nw_scan_config_cmd_t
{
    U16 transaction_id;
    U8 force_scan_on_off;
    U32 scan_time_interval;
}smif_set_nw_scan_config_cmd_t;

/* REL1.1 CHANGES : STARTS */

/*! \ struct smif_log_enable_disable_cmd_t
 *  \brief  Structure to send SMIF_LOG_ENABLE_DISABLE_CMD msg from SON MgmtIfH to SON internal modules (NMM, APPS, ES, ANR)
 *  \param  log_enable          Specifies whether to enable/disable logging
 *  \param  log_level           Log level to be set
 */
#define SMIF_MODULE_LOG_LEVEL_PRESENT 0x01
typedef struct _smif_log_enable_disable_cmd_t
{
    U32 bitmask;
    U8 log_enable;
    U8 log_level;
    U8 feature_id;
}smif_log_enable_disable_cmd_t;
/* REL1.1 CHANGES : ENDS */

/*! \ struct smif_set_log_level_cmd_t
 *  \brief  Structure to send SMIF_SET_LOG_LEVEL_CMD msg from SON MgmtIfH to SON internal modules (NMM, APPS, ES, ANR)
 *  \param  log_level           Log level to be set
 */
typedef struct _smif_set_log_level_cmd_t
{
    U8 log_level;
}smif_set_log_level_cmd_t;

/*! \ struct smif_shutdown_req_t
 *  \brief  Structure to send SMIF_SHUTDOWN_REQ msg from SON MgmtIfH to SON internal modules (NMM, APPS, ES, ANR)
 *  \param  transaction_id      Transaction Identifier
 */
typedef struct _smif_shutdown_req_t
{
    U16 transaction_id;
}smif_shutdown_req_t;

/*! \ struct smif_shutdown_res_t
 *  \brief  Structure to send SMIF_SHUTDOWN_RES msg
 *          from SON internal modules (NMM, APPS, ES, ANR) to SON MgmtIfH
 *  \param  transaction_id      Transaction Identifier
 *  \param  result              Success/Failure
 *  \param  error_code          Cause for failure
 */
typedef smif_init_config_res_t smif_shutdown_res_t;

/* Rel 3.0 Cell delete support changes Start */

/*! \  son_oam_cell_delete_req_t smif_cell_delete_req_t;
 */
typedef son_oam_cell_delete_req_t smif_cell_delete_req_t;


/* SPR_19619 start */

typedef son_oam_peer_cell_activation_req_t smif_peer_cell_activation_req_t;

/* SPR_19619 stop */
/*! \  son_oam_cell_delete_resp_t smif_cell_delete_resp_t;
 */
typedef son_oam_cell_delete_resp_t smif_cell_delete_resp_t;
/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*! \ struct smif_get_log_level_req_t
 *  \brief  Structure to send SMIF_GET_LOG_LEVEL_REQ msg from SON MgmtIfH to SON internal modules (NMM, APPS, ES, ANR etc.)
 *  \param  transaction_id	Transaction Identifier
 */
typedef struct _smif_get_log_level_req_t
{
    son_u16 transaction_id;
}smif_get_log_level_req_t;

/*! \ struct smif_get_log_level_res_t
 *  \brief  Structure for getting the log level of module in SMIF_GET_LOG_LEVEL_RES message
 *  \param  transaction_id  Transaction Identifier
 *  \param  log_level       Log Level of the module
 */
typedef struct _smif_get_log_level_res_t
{
    son_u16 transaction_id;
    son_u8  log_level;  
    son_module_id_et module_id;
}smif_get_log_level_res_t;
/* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
typedef struct _smif_neighbor_default_config_res_t
{
    son_u16             transaction_id;
    son_oam_anr_neighbor_default_config_resp_t  default_resp;
}smif_neighbor_default_config_res_t;

/* eICIC feature code Ends */

#endif /* _SON_MIF_INTF_H_ */
