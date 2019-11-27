/*! \file rrm_uem_rrmim_intf.h
 *  \brief This file contains Data Structures for interface between RRMIM
 *       and UE Manager
 *  \date March 12, 2012
 *  \author gur21481
*/
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _RRM_UEM_RRMIM_INTF_H_
#define _RRM_UEM_RRMIM_INTF_H_

/*! \headerfile lteTypes.h <>
 */
#include "lteTypes.h"
/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"
/*! \headerfile rrm_oam_intf.h <>
 */
#include "rrm_oam_intf.h"
/*! \enum   rrm_uem_log_level_et
 *  \brief  Different log level enumerations.
 */
typedef enum
{
    /*RRM_UEM_LOG_LEVEL_NONE,*/
    RRM_UEM_LOG_LEVEL_CRITICAL = LTE_LOG_CRITICAL,
    RRM_UEM_LOG_LEVEL_ERROR = LTE_LOG_ERROR,
    RRM_UEM_LOG_LEVEL_WARNING = LTE_LOG_WARNING,
    RRM_UEM_LOG_LEVEL_INFO = LTE_LOG_INFO,
    RRM_UEM_LOG_LEVEL_BRIEF = LTE_LOG_BRIEF,
    RRM_UEM_LOG_LEVEL_DETAILED = LTE_LOG_DETAILED,
    RRM_UEM_LOG_LEVEL_DETAILEDALL = LTE_LOG_DETAILEDALL
} rrm_uem_log_level_et;
#define RRM_UEM_LOG_LEVEL_NONE RRM_UEM_LOG_LEVEL_CRITICAL
/*! \enum rrm_uem_log_on_off_et
 *  \brief log ON OFF for RRM modules.
 */
typedef enum
{
    RRM_UEM_LOG_OFF,
    RRM_UEM_LOG_ON
} rrm_uem_log_on_off_et;

/* MRO changes start */
typedef enum
{
    RRMUEM_RMIF_HO_TOO_EARLY = 0,
    RRMUEM_RMIF_HO_TO_WRONG_CELL,
    RRMUEM_RMIF_HO_TOO_LATE
}rrmuem_rmif_ho_cause_et;
/* MRO changes end */

/* RRM UEM MIF INTERFACE APIS */

/*! \  struct struct rrmuem_rmif_init_ind_t 
 *  \brief Structure RRM UEM init indication
 *  \param module_id RRM module IDs
 */
typedef struct _rrmuem_rmif_init_ind_t
{
    rrm_internal_module_id_et module_id;    /*!< Module Id */
} rrmuem_rmif_init_ind_t;          /*^ API, RRMUEM_RMIF_INIT_IND ^*/

/*! \struct rrmuem_rmif_init_config_req_t 
 *  \brief RRM UEM init config request sructure
 *  \param  bitmask 		Bitmask for Module LOG ON and OFF
 *  \param  transaction_id	Transaction Identifier
 *  \param  log_on_off		Log ON/OFF Flag
 *  \param  log_level		Log Level Identifier
 */
typedef struct _rrmuem_rmif_init_config_req_t
{
#define RRM_UE_LOG_ON_OFF_PRESENT    0x0001
#define RRM_UE_LOG_LEVEL_PRESENT     0x0002
    rrm_bitmask_t		bitmask;		/*!< Bitmask for log on/off param */
	U16			transaction_id;		/*!< Transaction Id */
    rrm_uem_log_on_off_et   log_on_off; 		/*!< Log Enable/DIsable Flag */
    rrm_uem_log_level_et    log_level;   		/*!< Log Level */
} rrmuem_rmif_init_config_req_t;    /*^ API, RRMUEM_RMIF_INIT_CONFIG_REQ ^*/

/*! \struct rrmuem_rmif_init_config_res_t 
 *  \brief structure for RRM UEM init config response
 *  \param  transaction_id 	Transaction Identifier
 *  \param  response		Response(SUCCESS/FAILURE)
 */
typedef struct _rrmuem_rmif_init_config_res_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_bool_et             response;		/*!< UEM Config Response */
} rrmuem_rmif_init_config_res_t;    /*^ API, RRMUEM_RMIF_INIT_CONFIG_RES ^*/

/*! \struct rrmuem_rmif_log_enable_disable_req_t 
 *  \brief this structure used for log disable and enable request
 *  \param  transaction_id	Transaction Identifier
 *  \param  log_on_off		Log ON OFF Flag
 *  \param  log_level		Log Level Identifier
 */
typedef struct _rrmuem_rmif_log_enable_disable_req_t
{
	U16			transaction_id;		/*!< Transaction Id */
        rrm_uem_log_on_off_et   log_on_off; 		/*!< Log Enable/Disable Flag */
        rrm_uem_log_level_et    log_level;  		/*!< Log Level */
} rrmuem_rmif_log_enable_disable_req_t;     /*^ API, RRMUEM_RMIF_LOG_ENABLE_DISABLE_REQ ^*/

/*! \struct rrmuem_rmif_log_enable_disable_res_t 
 *  \brief   This structure contains params for log disable and enable response
 *  \param  transaction_id	Transaction Identifier
 *  \param  response		Response(SUCCESS/FAILURE)
 */
typedef struct _rrmuem_rmif_log_enable_disable_res_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_bool_et             response;		/*!< Response */
} rrmuem_rmif_log_enable_disable_res_t;     /*^ API, RRMUEM_RMIF_LOG_ENABLE_DISABLE_RES ^*/

/*! \struct rrmuem_rmif_set_log_level_req_t 
 *  \brief  This structure used for set the LOG level
 *  \param  transaction_id	Transaction Identifier
 *  \param  log_level		Log Level Identifier
 */
typedef struct _rrmuem_rmif_set_log_level_req_t
{
	U16			transaction_id;		/*!< Transaction Id */
        rrm_uem_log_level_et    log_level;      	/*!< Log Level */
}rrmuem_rmif_set_log_level_req_t;   /*^ API, RRMUEM_RMIF_SET_LOG_LEVEL_REQ ^*/

/*! \struct rrmuem_rmif_set_log_level_res_t 
 *  \brief This struct set log level response
 *  \param transaction_id 	Transaction Identifier
 *  \param response		Response(SUCCESS/FAILURE)
 */
typedef struct _rrmuem_rmif_set_log_level_res_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_bool_et             response;		/*!< Response */
}rrmuem_rmif_set_log_level_res_t;   /*^ API, RRMUEM_RMIF_SET_LOG_LEVEL_RES ^*/

/*! \struct rrmuem_rmif_emergency_call_active_req_t 
 *  \brief  this structure used for emergency call request
 *  \param  transaction_id	Transaction Identifier
 *  \param  cell_index		Cell Index 
 */
typedef struct _rrmuem_rmif_emergency_call_active_req_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_cell_index_t cell_index;		/*!< Cell Index */		
}rrmuem_rmif_emergency_call_active_req_t;   /*^ API, RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_REQ ^*/

/*! \struct rrmuem_rmif_emergency_call_active_res_t 
 *  \brief  this sturcure used emergency call active response
 *  \param  transaction_id		Transaction Identifier
 *  \param  cell_index			Cell Index
 *  \param  emergency_call_ongoing 	flag for emergency call ongoing
 */
typedef struct _rrmuem_rmif_emergency_call_active_res_t
{
	U16			transaction_id;		/*!< Transaction Id */
	rrm_cell_index_t        cell_index;		/*!< Cell Index */		
	rrm_bool_et             emergency_call_ongoing; /*!< Emergency Call Ongoing Flag */
}rrmuem_rmif_emergency_call_active_res_t;   /*^ API, RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES ^*/

/*! \struct rrmuem_rmif_ue_release_req_t
 *  \brief  This struct contains UE release request
 *  \param  ue_index   UE Index
 */
typedef struct _rrmuem_rmif_ue_release_req_t
{
/*SPR_11366_FIX_START */
		rrm_cell_index_t				cell_index;  
		rrm_ue_index_t                  ue_index;
/*SPR_11366_FIX_END */
}rrmuem_rmif_ue_release_req_t;

/*! \struct rrmuem_rmif_meas_results_ecid_r9_t
 *  \brief  This struct contains UE release request
 *  \param  ue_index   UE Index
 */
typedef struct _rrmuem_rmif_meas_results_ecid_r9_t
{
    U16    ue_rxtx_time_diff;
    U8     current_sfn_r9[ECID_CURRENT_SFN_OCTET_SIZE];
}rrmuem_rmif_meas_results_ecid_r9_t;

/*! \struct rrmuem_rmif_meas_results_ind_t
 *  \brief  This structure used for UEM measurement indication.
 *  \param  cell_index			Cell Index	
 *  \param  ue_index			UE Index
 *  \param  eutran_meas_list_size	Eutran measurement list size
 *  \param  eutran_meas_list		Eutran measurement list
 *  \param  utran_meas_list_size	Utran measurement list size
 *  \param  utran_meas			Utran measurement
 */ 
typedef struct _rrmuem_rmif_meas_results_ind_t
{
    rrm_cell_index_t                cell_index;
    rrm_ue_index_t                  ue_index;
    /* SPR 15698 Start */
    rrmc_meas_report_event_et       meas_report_event;
    /* SPR 15698 End */
    U8                              eutran_meas_list_size;
    rrm_son_eutran_meas_results_t   eutran_meas_list[RRM_MAX_CELLS_REPORTED];
    U8                              utran_meas_list_size;
    rrm_son_utran_meas_results_t    utran_meas[RRM_MAX_CELLS_REPORTED];
    U8                              geran_meas_list_size;
    rrm_son_geran_meas_results_t    geran_meas[RRM_MAX_CELLS_REPORTED];
    U8                              cdma_meas_list_size;
    rrm_son_cdma_meas_results_t     cdma_meas[RRM_MAX_CELLS_REPORTED];
    rrmuem_rmif_meas_results_ecid_r9_t meas_results_ecid_r9;
}rrmuem_rmif_meas_results_ind_t;

/*! \struct rrmuem_rmif_meas_config_resp_t
 *  \brief meaurement config response sturcture.
 *  \param  cellindex		Cell Index
 *  \param  transaction_id	Transaction Identifier
 *  \param  response		Response (SUCCESS/FAILURE)
 *  \param  fail_cause 		Fail Cause
 */
typedef struct _rrmuem_rmif_meas_config_resp_t
{
    rrm_cell_index_t cellindex;
    U16 transaction_id;
    U8  response;
    U16 fail_cause;
}rrmuem_rmif_meas_config_resp_t;

/*! \struct rrmuem_rmif_release_all_existing_ue_forcefully_req_t
 *  \brief  all ue release forcefully request
 *  \param  transaction_id	Transaction Identifier
 *  \param  cell_index		Cell Index
 */
typedef struct _rrmuem_rmif_release_all_existing_ue_forcefully_req_t
{
    U16                 transaction_id;     /*!< Transaction Id */
    rrm_cell_index_t    cell_index;			/*!< Cell Index */
}rrmuem_rmif_release_all_existing_ue_forcefully_req_t;
/* UE MEAS CHANGES: ENDS */

/* Soft Lock changes starts */

/*! \struct rrmuem_rmif_no_active_ue_ind_t
 *  \brief  all ue release indication
 *  \param  transaction_id  Transaction Identifier
 *  \param  cell_index      Cell Index
 */

typedef struct _rrmuem_rmif_no_active_ue_ind_t
{
    rrm_cell_index_t    cell_index;         /*!< Cell Index */
}rrmuem_rmif_no_active_ue_ind_t;

/*! \struct rrmuem_rmif_non_emrgncy_active_calls_ho_req_t
 *  \brief  all ue release indication
 *  \param  transaction_id  Transaction Identifier
 *  \param  cell_index      Cell Index
 */
typedef struct _rrmuem_rmif_non_emrgncy_active_calls_ho_req_t
{
    U16                 transaction_id;     /*!< Transaction Id */
    rrm_cell_index_t    cell_index;         /*!< Cell Index */
}rrmuem_rmif_non_emrgncy_active_calls_ho_req_t;   /*^ API, RRMUEM_RMIF_NON_EMRGNCY_ACTIVE_CALLS_HO_REQ ^*/

/*! \struct rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t
 *  \brief  all ue release indication
 *  \param  transaction_id  Transaction Identifier
 *  \param  cell_index      Cell Index
 */
typedef struct _rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t
{
    U16                     transaction_id;       /*!< Transaction Id */
    rrm_cell_index_t        cell_index;           /*!< Cell Index */
    rrm_bool_et             response;             /*!< Response */
}rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t;   /*^ API, RRMUEM_RMIF_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP ^*/

/* Soft Lock changes ends */

#define RRMUEM_RMIF_DST_SON_MOD_ANR_HOR 0x01
#define RRMUEM_RMIF_DST_SON_MOD_MLB_HOR 0x02
/* BLR Team HO status report to SON Changes Start */

/*! \struct  rrmuem_rmif_handover_report_t
 *  \brief   This struct contains params for rrm ue handover 
 * \param    src_cgi   Source cell global identity
 * \param    trgt_cgi  Target cell global identity for which HO is triggered
 * \param    srv_cell_meas_strgth  Serving cell measurement result 
 * \param    trgt_cell_meas_strgth Target cell measurement result
 */
typedef struct _rrmuem_rmif_ho_report_t
{
    rrm_oam_eutran_global_cell_id_t                 src_cgi;
    /*^ M, 0, N, 0, 0 ^*/

    rrm_oam_global_cell_id_t      trgt_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_return_et                  result;
    /*^ M, 0, N, 0, 0 ^*/   /* Result */
    /* MLB Changes start */
    rrm_eutran_meas_strength_t    srv_cell_meas_strgth;
    U8                            rrm_dst_son_mod_ho_rep;
    rrm_trgt_cell_meas_strength_t trgt_cell_meas_strgth;
    /* MLB Changes end */

} rrmuem_rmif_ho_report_t;
/* BLR Team HO status report to SON Changes End */

/*Local Event Handling Feature- Start*/

/* ! \struct rrmuem_rmif_time_stamp_t
 * \brief This struct contains the timestamp
 * \param year 
 * \param month
 * \param day
 * \param hour
 * \param min
 * \param sec
*/
typedef struct _rrmuem_rmif_time_stamp_t
{
  U16   year;
  U16   month;
  U16   day;
  U16   hour;
  U16   min;
  U16   sec;
}rrmuem_rmif_time_stamp_t;

/* ! \struct rrmuem_rmif_event_header_t
 * \brief This struct contains the event header  
 * \param msg_len  message lenghth         
 * \param time_stamp time stamp
 * \param event_id event id
 * \param core_id  core id
 * \param cell_id  cell id
 * \param ue_id    ue id
 * \param msg_direction message direction
 * \param mom_id mom id

*/
typedef struct _rrmuem_rmif_event_header_t
{
    rrmuem_rmif_time_stamp_t  time_stamp;
    rrm_oam_event_class_et    event_type;
    rrm_oam_event_subclass_et event_sub_type;
    U16                       event_id;
}rrmuem_rmif_event_header_t;

/*! \def RRM_MIF_EVENT_DATA_PRESENT
*/
#define RRM_MIF_EVENT_DATA_PRESENT  0x01
/* ! \struct rrmuem_rmif_event_notification_ind_t
 *   \brief This struct contains information about the event notification
 *   \param bitmask   To indicate if the optional fields are present or not 
 *   \param header    contains the complete header
 *   \param buffer[200] data buffer
*/
typedef struct _rrmuem_rmif_event_notification_ind_t
{
    rrm_bitmask_t                bitmask;
    rrmuem_rmif_event_header_t   header;
    U8                           buffer[MAX_BUFFER];
}rrmuem_rmif_event_notification_ind_t;

/* MRO changes start */
/* ! \struct rrmuem_rmif_ho_attempt_ind_t
 *   \brief 
 *   \param src_cgi  Source Cell CGI
 *   \param trgt_cgi Target Cell CGI 
*    \param attempt_count number of attempts
*/
typedef struct _rrmuem_rmif_ho_attempt_ind_t
{
   rrm_oam_eutran_global_cell_id_t src_cgi;
   /*^ M, 0, N, 0, 0 ^*/
   rrm_oam_eutran_global_cell_id_t trgt_cgi;
   /*^ M, 0, N, 0, 0 ^*/
   /* This count will be set to 1 in present release */
   U8                              attempt_count; 
   /*^ M, 0, N, 0, 0 ^*/
}rrmuem_rmif_ho_attempt_ind_t;

/*! \def RRM_MIF_HO_FAILURE_REPORT_RE_EST_CGI_PRESENT
*/
#define RRM_MIF_HO_FAILURE_REPORT_RE_EST_CGI_PRESENT 0x01
/* ! \struct rrmuem_rmif_ho_failure_report_t
 *  \brief This struct contains the handoff failure report params
 *  \param bitmask  To indicate if the optional fields are present or not 
 *  \param src_cgi  Serving CELL CGI
 *  \param trgt_cgi Target CELL CGI
 *  \param restablishment_cgi restablishment cgi
 *  \param ho_cause cause of handoff
*/
typedef struct _rrmuem_rmif_ho_failure_report_t
{
    rrm_bitmask_t                    bitmask;
    /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t  src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_eutran_global_cell_id_t  trgt_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_eutran_global_cell_id_t  restablishment_cgi;
    /*^ O, RRM_MIF_HO_FAILURE_REPORT_RE_EST_CGI_PRESENT, N, 0, 0 ^*/
    rrmuem_rmif_ho_cause_et          ho_cause;
    /*^ M, 0, H, 0, 2 ^*/
}rrmuem_rmif_ho_failure_report_t;
/*^ API, RRMUEM_RMIF_HO_FAILURE_REPORT ^*/
/* MRO changes end */

typedef struct _rrmuem_rmif_event_config_req_t
{
  U16   transaction_id;       /*!< Transaction Id */  
  U16   count;
  rrm_oam_event_config_t  event_config[RRM_OAM_MAX_SUBCLASS];
}rrmuem_rmif_event_config_req_t;

typedef struct _rrmuem_rmif_event_config_resp_t
{
  U16           transaction_id;       /*!< Transaction Id */
  rrm_bool_et   response;             /*!< Response */
}rrmuem_rmif_event_config_resp_t; 

/* RACH_OPTIMIZATION_CHANGES_START */

typedef struct _rrmuem_rmif_ue_rach_report_t
{
    rrm_cell_index_t         cell_index;
    U16                      count; 
    rrm_ue_prach_data_info_t rach_data[MAX_NUMBER_OF_PRACH_SAMPLES];
}rrmuem_rmif_ue_rach_report_t;

/* CA_Stage3_Change: Start */
typedef struct _rrmuem_rcm_cell_del_resp
{
    rrm_cell_index_t         pcell_index;
    rrm_cell_index_t         scell_index;
    rrm_return_et            status;
    /*SPR 13316:start*/
    U16                      trans_id;
    /*SPR 13316:end*/
}rrmuem_rcm_cell_del_resp_t;
/* CA_Stage3_Change: End */
/* BUG_11648_FIX_START */
typedef struct _rrmuem_rcm_cell_stop_broadcast_resp
{
    rrm_cell_index_t         pcell_index;
    rrm_cell_index_t         scell_index;
    rrm_return_et            status;
    /*SPR 13316:start*/
    U16                      trans_id;
    /*SPR 13316:end*/
}rrmuem_rcm_cell_stop_broadcast_resp_t;

/* BUG_11648_FIX_END */

typedef struct _rrmuem_rmif_rach_info_ind_t
{
#define RRM_UE_REPORT_PERIODICITY_PRESENT    0x01
#define RRM_UE_RACH_SAMPLE_COUNT_PRESENT     0x02
#define RRM_UE_NUM_STRONGEST_CELLS_PRESENT   0x04
    rrm_bitmask_t            bitmask;
    rrm_cell_index_t         cell_index;
    U16                      ue_reports_periodicity;
    U16                      rach_sample_count;
    U8                       num_of_strongest_cells;
}rrmuem_rmif_rach_info_ind_t;
/*^ API, RRMUEM_RMIF_RACH_INFO_IND ^*/
/* RACH_OPTIMIZATION_CHANGES_END */
/*SPR-695 Fix Starts*/
/*! \ rrmuem_cm_unset_for_all_ue_ind_t
 *  \brief This struct ids for all ue indication information
 *  \param trans_id transaction ID
 *  \param cell_index cell index
 */
typedef struct _rrmuem_cm_ue_rel_ind_t
{
    U16 trans_id;
    U8 count;
    rrm_cell_index_t cell_index;
}rrmuem_cm_ue_rel_ind_t;
/*SPR-695 Fix Ends*/

/* SPR_13117_FIX_START */
/*! \struct rrmuem_rmif_get_log_level_req_t 
 *  \brief  This structure used for get the LOG level
 *  \param  transaction_id  Transaction Identifier
 *  \param  module_id       Module identifier
 */

typedef struct _rrmuem_rmif_get_log_level_req_t
{
    U16                           transaction_id;     /*!< Transaction Id */
    rrm_internal_module_id_et     module_id;
}rrmuem_rmif_get_log_level_req_t; /*^ API, RRMUEM_RMIF_GET_LOG_LEVEL_REQ ^*/

/*! \struct rrmuem_rmif_get_log_level_resp_t
 *  \brief  This structure used for get the LOG level
 *  \param  transaction_id  Transaction Identifier
 *  \param  module_id       Module identifier
 *  \param  log_level_on_off_on_cell
 *  \param  log_level_for_cellm
 *  \param  log_level_on_off_on_ue
 *  \param  log_level_for_uem
 */

typedef struct _rrmuem_rmif_get_log_level_resp_t
{
    U16    transaction_id;
    U8     module_id;
    U8     log_level_on_off_on_cell;
    U8     log_level_for_cellm;
    U8     log_level_on_off_on_ue;
    U8     log_level_for_uem;
}rrmuem_rmif_get_log_level_resp_t; /*^ API, RRMUEM_RMIF_GET_LOG_LEVEL_RESP ^*/
/* SPR_13117_FIX_END */

#endif /* _RRM_UEM_RRMIM_INTF_H_ */
