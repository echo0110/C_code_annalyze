/******************************************************************************
*
*   FILE NAME:
*       rrm_oam_interface.h
*
*   DESCRIPTION:
*       This file contains the rrm's API interface structures towards OAM 
*
*   DATE            AUTHOR      REFERENCE       REASON
*   31 Jan 2012     Aricent     ---------       Initial
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
/*! \file rrm_oam_intf.h
 *  \brief This file contains the the rrm's API interface structures towards OAM
 *  \date January 31, 2012
 *  \author Aricent
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _RRM_OAM_INTF_H
#define _RRM_OAM_INTF_H


/*
 * Header File Include
 */
/*! \headerfile stacklayer.h <>
 */
#include <stacklayer.h>
/*! \headerfile rrm_oam_defines.h <>
 */
#include "rrm_oam_defines.h"
/*! \headerfile rrm_oam_types.h <>
 */
#include "rrm_oam_types.h"
/*
 * Interface structures Starts
 */
/*
 * RRM_OAM_INIT_CONFIG_REQ
 */

/*! \ rrm_oam_log_config_t
 *  \brief This struct contains initial configuration log for the RRM internal modules
 *  \param log_on_off   Specifies to Enable or Disable Logging of the specified module
 *  \param log_level    log level to be set for logging
 */
typedef struct _rrm_oam_log_config_t
{
       rrm_bitmask_t            bitmask; /*^ BITMASK ^*/
       rrm_oam_log_on_off_et   log_on_off; /*^ M, 0, N, 0, 0 ^*/
       /* coverity 94838/94849/94858/94860/35170/35221/35283/94809 fix */
        rrm_oam_log_level_et    log_level;   /*^ M, 0, H, 0, 6 ^*/
}rrm_oam_log_config_t;

/*! \ rrm_oam_module_init_config_t
 *  \brief This struct contains initial configuration for the RRM internal modules
 *  \param module_id Denotes Module Identifier
 *  \param log_config Denotes initial configuration log for RRM internal modules
 */
typedef struct _rrm_oam_module_init_config_t
{
    rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
    rrm_internal_module_id_et       module_id;  /*^ M, 0, N, 0, 0  ^*/
	rrm_oam_log_config_t		log_config; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_module_init_config_t;


/*! \ rrm_oam_init_config_req_t
 *  \brief This struct denotesInitial Configuration Request received from OAM for RRM modules
 *  \param bitmask Bitmask Value
 *  \param init_module_config[RRM_MAX_NUM_INT_MODULES]   Contains initial configuration reqd by RRM internal modules
 */
#define RRM_OAM_MODULE_INIT_CONFIG_PRESENT 0x0001
#define RRM_OAM_MODULE_HEALTH_TIMER_PRESENT 0x0002
#define RRM_OAM_SHM_LOGGING_INFO_PRESENT 0x0004
/* Spr 16211 Changes Start*/
#define RRM_OAM_RIM_CONFIG_INFO_PRESENT  0x0008
/* Spr 16211 Changes End*/
typedef struct _rrm_oam_init_config_req_t
{
	rrm_bitmask_t 			bitmask; /*^ BITMASK ^*/
        rrm_oam_module_init_config_t 	init_module_config[RRM_MAX_NUM_INT_MODULES]; /*^ O, RRM_OAM_MODULE_INIT_CONFIG_PRESENT, OCTET_STRING, FIXED ^*/
        U16                             health_update_timer; /*^ O ,RRM_OAM_MODULE_HEALTH_TIMER_PRESENT ,B ,5,900 ^*/ 
        rrm_oam_shm_log_configuration_t            rrm_shm_log_config; /*^ O, RRM_OAM_SHM_LOGGING_INFO_PRESENT, N, 0, 0 ^*/
/* Spr 16211 Changes Start*/
        rrm_oam_rim_configuration_t     rrm_rim_config;   /*^ O, RRM_OAM_RIM_CONFIG_INFO_PRESENT, N, 0, 0 ^*/                  
/* Spr 16211 Changes End*/
} rrm_oam_init_config_req_t;    /*^ API, RRM_OAM_INIT_CONFIG_REQ ^*/


/*! \ rrm_oam_init_config_resp_t
 *  \brief This struct contains Initial Configuration Request received from OAM for RRM modules
 *  \param response Denotes the response to init config
 *  \param fail_cause Denotes the fail cause
 */
typedef struct _rrm_oam_init_config_resp_t
{
	rrm_return_et	response; /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_init_config_resp_t;    /*^ API, RRM_OAM_INIT_CONFIG_RESP ^*/


/*
 *   RRM_OAM_SET_LOG_LEVEL_REQ
 */

/*! \ rrm_oam_set_log_level_req_t
 *  \brief This struct contains Request from OAM to set the log level of RRM internal module
 *  \param module_id Internal Module ID (In case module Id is zero then log level will be applied to all the RRM modules)
 *  \param log_level log level to be set
 */
typedef struct _rrm_oam_set_log_level_req_t
{
        rrm_internal_module_id_et       module_id;  /*^ M, 0, N, 0, 0  ^*/
        /* coverity 94838/94849/94858/94860/35170/35221/35283/94809 fix */
        rrm_oam_log_level_et            log_level;  /*^ M, 0, H, 0, 6 ^*/
}rrm_oam_set_log_level_req_t;   /*^ API, RRM_OAM_SET_LOG_LEVEL_REQ ^*/


/*! \ rrm_oam_set_log_level_resp_t
 *  \brief This struct denotes Reponse to set log level req
 *  \param response   success/fail
 *  \param fail_cause  fail cause
 */
typedef struct _rrm_oam_set_log_level_resp_t
{
	rrm_return_et	response;  /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause;  /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_set_log_level_resp_t;    /*^ API, RRM_OAM_SET_LOG_LEVEL_RESP ^*/


/*! \ rrm_oam_enable_disable_log_config_t
 *  \brief This  struct denotes log configuration for the enable-disable api
 *  \param bitmask Bitmask Value
 *  \param log_on_off Denotes whether log is on/off
 *  \param log_level Denotes the level of log
 */
#define RRM_OAM_LOG_LEVEL_PRESENT   0x0001
typedef struct _rrm_oam_enable_disable_log_config_t
{
        rrm_bitmask_t                 bitmask;    /*^ BITMASK ^*/
        rrm_oam_log_on_off_et         log_on_off; /*^ M, 0, N, 0, 0 ^*/
        /* coverity 94838/94849/94858/94860/35170/35221/35283/94809 fix */
        rrm_oam_log_level_et          log_level;  /*^ O, RRM_OAM_LOG_LEVEL_PRESENT, H, 0, 6 ^*/
}rrm_oam_enable_disable_log_config_t;


/*! \ rrm_oam_log_enable_disable_req_t
 *  \brief This struct denotes Request from OAM to enable/disable the RRM logging at a specified module or globally for all the RRM modules.
 *  \param module_id Internal Module ID (In case module Id is zero then logging will be enabled/disabled for all the RRM modules)
 *  \param log_config denotes log configuration for the enable-disable api
 */
typedef struct _rrm_oam_log_enable_disable_req_t
{
        rrm_internal_module_id_et             module_id;  /*^ M, 0, N, 0, 0  ^*/ 
	rrm_oam_enable_disable_log_config_t   log_config; /*^ M, 0, N, 0, 0 ^*/
} rrm_oam_log_enable_disable_req_t; /*^ API, RRM_OAM_LOG_ENABLE_DISABLE_REQ ^*/


/*! \ rrm_oam_log_enable_disable_resp_t
 *  \brief This struct denotes Reponse to enable/disable logs
 *  \param response  success/fail
 *  \param fail_cause  fail cause
 */
typedef struct _rrm_oam_log_enable_disable_resp_t
{
	rrm_return_et	response; /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_log_enable_disable_resp_t;    /*^ API, RRM_OAM_LOG_ENABLE_DISABLE_RESP ^*/


/*! \ rrm_oam_shutdown_req_t
 *  \brief This struct denotes shutdown_mode forced/graceful
 *  \param shutdown_mode Denotes the mode of shutdown
 *  \param time_to_shutdown  time to shutdown
 */
typedef struct _rrm_oam_shutdown_req
{
	rrm_oam_shutdown_mode_et shutdown_mode;		    /*^ M, 0, H, 0, 1 ^*/
	U16		         time_to_shutdown;          /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_shutdown_req_t; /*^ API, RRM_OAM_SHUTDOWN_REQ ^*/

/*! \typedef rrm_oam_shutdown_resp_t
 *  \brief This struct denotes Reponse to enable/disable logs
 *  \param response  success/fail
 *  \param fail_cause  fail cause
 */
typedef struct _rrm_oam_shutdown_resp_t
{
	rrm_return_et	response;    /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause;  /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_shutdown_resp_t;    /*^ API, RRM_OAM_SHUTDOWN_RESP ^*/


/*! \ rrm_oam_resume_service_resp_t
 *  \brief This struct denotes Reponse to resume service request
 *  \param response  success/fail
 *  \param fail_cause  fail cause
 */
typedef struct _rrm_oam_resume_service_resp
{
	rrm_return_et	response;   /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_resume_service_resp_t; /*^ API, RRM_OAM_RESUME_SERVICE_RESP ^*/


/* RRM_OAM_CELL_START_REQ */
/*! \ rrm_oam_cell_start_req_t
 *  \brief This struct denotes Request to start a cell
 *  \param global_cell_id     global cell id
 */
typedef struct rrm_oam_cell_start_req
{
	rrm_oam_eutran_global_cell_id_t	global_cell_id;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_start_req_t;/*^ API, RRM_OAM_CELL_START_REQ ^*/


/* RRM_OAM_CELL_START_RESP */
/*! \ rrm_oam_cell_start_resp_t
 *  \brief This stuct contains Response to start a cell
 *  \param global_cell_id     global cell id
 *  \param response  success/failure
 *  \param fail_cause  fail cause
 */
typedef struct _rrm_oam_cell_start_resp_t
{
	rrm_oam_eutran_global_cell_id_t	 global_cell_id; /*^ M, 0, N, 0, 0 ^*/
	rrm_return_et	response;   /*^ M, 0, N, 0, 0 ^*/   
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_start_resp_t;    /*^ API, RRM_OAM_CELL_START_RESP^*/


/* RRM_OAM_CELL_STOP_REQ */
/*! \ rrm_oam_cell_stop_req_t
 *  \brief This struct contains Request to stop a cell
 *  \param global_cell_id     global cell id
 */
typedef struct _rrm_oam_cell_stop_req
{
	rrm_oam_eutran_global_cell_id_t	global_cell_id;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_stop_req_t;/*^ API, RRM_OAM_CELL_STOP_REQ ^*/


/* RRM_OAM_CELL_STOP_RESP */
/*! \ rrm_oam_cell_stop_resp_t
 *  \brief This struct contains Response to stop a cell
 *  \param global_cell_id     global cell id
 *  \param response  success/failure
 *  \param fail_cause  fail cause
 */
typedef struct _rrm_oam_cell_stop_resp_t
{
	rrm_oam_eutran_global_cell_id_t	 global_cell_id; /*^ M, 0, N, 0, 0 ^*/
	rrm_return_et	response; /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_stop_resp_t;    /*^ API, RRM_OAM_CELL_STOP_RESP^*/



/* RRM_OAM_ELL_DELETE_REQCELL_DELETE_REQ*/
/*! \ rrm_oam_cell_delete_req_t
 *  \brief This struct contains Request to delete a cell
 *  \param global_cell_id     global cell id
 */
/* SPR 21369 Start */
#define RRM_OAM_INSTANCE_RESET_FLAG_PRESENT			0x01
typedef struct rrm_oam_cell_delete_req
{
        U8 bitmask; /*^ BITMASK ^*/
/* SPR 21369 End */
	rrm_oam_eutran_global_cell_id_t	global_cell_id;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_delete_req_t;/*^ API, RRM_OAM_CELL_DELETE_REQ ^*/


/* RRM_OAM_CELL_DELETE_RESP */
/*! \ rrm_oam_delete_resp_t
 *  \brief This struct denotes Response to delete a cell
 *  \param global_cell_id     global cell id
 *  \param response response enum
 *  \param fail_cause cause of failure
 */
typedef struct _rrm_oam_delete_resp_t
{
	rrm_oam_eutran_global_cell_id_t	global_cell_id;/*^ M, 0, N, 0, 0 ^*/
	rrm_return_et	response;   /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_delete_resp_t; /*^ API, RRM_OAM_CELL_DELETE_RESP ^*/


/* RRM_OAM_CELL_CONFIG_REQ */
/*! \ rrm_oam_cell_config_req_t 
 *  \brief This struct contains Request to config a cell
 *  \param global_cell_info  Contains global cell information
 *  \param ran_info  denotes RAN info 
 *  \param epc_info  denotes EPC info
 *  \param operator_info  denotes Operator info
 *  \param access_mgmt_params denotes access mgmt parameters info
 *  \param immediate_start_needed denotes whether cell needs to be started immediately or it is just configured
 */
#define RRM_OAM_ACCESS_MGMT_PARAMS_PRESENT	0x0001
typedef struct _rrm_oam_cell_config_req
{
	rrm_bitmask_t           	bitmask;                        /*^ BITMASK ^*/
    rrm_oam_cell_info_t		global_cell_info;		/*^ M, 0, N, 0, 0 ^*/
    rrm_oam_ran_t                   ran_info;			/*^ M, 0, N, 0, 0 ^*/
    rrm_oam_epc_t                   epc_info;			/*^ M, 0, N, 0, 0 ^*/
    rrm_oam_operator_info_t         operator_info;			/*^ M, 0, N, 0, 0 ^*/
    rrm_oam_access_mgmt_params_t    access_mgmt_params;		/*^ O, RRM_OAM_ACCESS_MGMT_PARAMS_PRESENT, N, 0, 0 ^*/
    rrm_bool_et             	immediate_start_needed; 	/*^ M, 0, N, 0, 0 ^*/
    /* SPR 20908 Start */
    rrm_oam_lower_layer_comm_info_t l2_comm_info_params;      /*^ M, 0, N, 0, 0 ^*/
    /* SPR 20908 End */
}rrm_oam_cell_config_req_t; /*^ API, RRM_OAM_CELL_CONFIG_REQ ^*/


/* RRM_OAM_CELL_CONFIG_RESP */
/*! \ rrm_oam_cell_config_resp_t
 *  \brief This struct contains Response to config a cell
 *  \param global_cell_id     global cell id
 *  \param response response enum
 *  \param fail_cause cause of failure
 */
typedef struct _rrm_oam_cell_config_resp_t
{
	rrm_oam_eutran_global_cell_id_t	 global_cell_id;/*^ M, 0, N, 0, 0 ^*/
	rrm_return_et	response;  /*^ M, 0, N, 0, 0 ^*/ 
	rrm_error_et 	fail_cause;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_config_resp_t;    /*^ API, RRM_OAM_CELL_CONFIG_RESP ^*/


/* RRM_OAM_CELL_RECONFIG_REQ */
#define RRM_OAM_CELL_ACCESS_PARAMS_PRESENT	        0x0001
#define RRM_OAM_RAN_INFO_PRESENT	                0x0002
#define RRM_OAM_EPC_INFO_PRESENT	                0x0004
#define RRM_OAM_OPERATOR_INFO_PRESENT	        0x0008
#define RRM_OAM_RRM_ACCESS_MGMT_PARAMS_PRESENT	0x0010
/*! \ rrm_oam_cell_reconfig_req_t
 *  \brief This struct contains Request to config a cell
 *  \param global_cell_info  Contains global cell information
 *  \param ran_info  denotes RAN info
 *  \param epc_info  denotes EPC info
 *  \param operator_info  denotes Operator info
 *  \param access_mgmt_params denotes access mgmt parameters info
 */
typedef struct _rrm_oam_cell_reconfig_req
{
	rrm_bitmask_t           	bitmask; 		/*^ BITMASK ^*/
	rrm_oam_eutran_global_cell_id_t	global_cell_id; 	/*^ M, 0, N, 0, 0 ^*/
	rrm_oam_cell_access_restriction_params_t	cell_access_restriction_params; /*^ O, RRM_OAM_CELL_ACCESS_PARAMS_PRESENT, N, 0, 0 ^*/
        rrm_oam_ran_t                   ran_info;		/*^ O, RRM_OAM_RAN_INFO_PRESENT, N, 0, 0 ^*/
        rrm_oam_epc_t                   epc_info;		/*^ O, RRM_OAM_EPC_INFO_PRESENT, N, 0, 0 ^*/
        rrm_oam_operator_info_t         operator_info;		/*^ O, RRM_OAM_OPERATOR_INFO_PRESENT, N, 0, 0 ^*/
        rrm_oam_access_mgmt_params_t    access_mgmt_params;	/*^ O, RRM_OAM_RRM_ACCESS_MGMT_PARAMS_PRESENT, N, 0, 0 ^*/
}rrm_oam_cell_reconfig_req_t; /*^ API, RRM_OAM_CELL_RECONFIG_REQ ^*/


/* RRM_OAM_CELL_RECONFIG_RESP */
/*! \ rrm_oam_cell_reconfig_resp_t
 *  \brief This struct contains Response to reconfigure a cell
 *  \param global_cell_id     global cell id
 *  \param response response enum
 *  \param fail_cause cause of failure
 */
typedef struct _rrm_oam_cell_reconfig_resp_t
{
	rrm_oam_eutran_global_cell_id_t	global_cell_id;/*^ M, 0, N, 0, 0 ^*/
	rrm_return_et	response;   /*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 	fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_reconfig_resp_t; /*^ API, RRM_OAM_CELL_RECONFIG_RESP ^*/


/* RRM_OAM_ENABLE_DISABLE_RAC_REQ */
/*! \ rrm_oam_rac_enable_disable_req_t
 *  \brief This struct contains Request to enable/disable rac
 *  \param bitmask		to indicate if RAC is to be turned on for one or all cells
 *  \param global_cell_id     global cell id
 *  \param request_type       type of request
 */
#define RRM_OAM_CELL_ID_PRESENT			0x0001
typedef struct _rrm_oam_rac_enable_disable_req_t
{
	rrm_bitmask_t				bitmask; /*^ BITMASK ^*/
	rrm_oam_rac_enable_disable_req_type_et  request_type; /*^ M, 0, N, 0, 0 ^*/
	rrm_oam_eutran_global_cell_id_t		global_cell_id; /*^ O, RRM_OAM_CELL_ID_PRESENT, N, 0, 0 ^*/
}rrm_oam_rac_enable_disable_req_t;	/*^ API, RRM_OAM_RAC_ENABLE_DISABLE_REQ ^*/

/* RRM_OAM_ENABLE_DISABLE_RAC_RESP */
/*! \ rrm_oam_rac_enable_disable_resp_t
 *  \brief This struct contains Response to enable/disable rac requset
 *  \param bitmask
 *  \param global_cell_id     global cell id
 *  \param response           response
 *  \param fail_cause         cause of failure
 */
#define RRM_OAM_CELL_ID_PRESENT			0x0001
typedef struct _rrm_oam_rac_enable_disable_resp_t
{
	rrm_bitmask_t				bitmask; /*^ BITMASK ^*/
	rrm_oam_eutran_global_cell_id_t		global_cell_id; /*^ O, RRM_OAM_CELL_ID_PRESENT, N, 0, 0 ^*/
	rrm_return_et				response;/*^ M, 0, N, 0, 0 ^*/
	rrm_error_et 				fail_cause; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_rac_enable_disable_resp_t; /*^ API, RRM_OAM_RAC_ENABLE_DISABLE_RESP ^*/

/*! \ rrm_oam_cell_context_print_req_t
 *  \brief This struct contains Request to print cell context 
 *  \param global_cell_id     global cell id
 */
typedef struct _rrm_oam_cell_context_print_req
{
    rrm_oam_eutran_global_cell_id_t         global_cell_id; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_context_print_req_t;  /*^ API, RRM_OAM_CELL_CONTEXT_PRINT_REQ ^*/

/*UE Release Req from OAM to RRM */
/*! \ rrm_oam_ue_release_req_t
 *  \brief This struct contains request to release UE
 *  \param ue_index UE index
 */
typedef struct
{
  rrm_oam_eutran_global_cell_id_t     global_cell_id;  /*^ M, 0, N, 0, 0 ^*/
  U16    ue_index; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_ue_release_req_t;  /*^ API, RRM_OAM_UE_RELEASE_REQ ^*/


/* Start API for Soft lock feature */
/* RRM_OAM_CELL_BLOCK_REQ*/
/*! \ rrm_oam_cell_block_req_t
 *  \brief This struct denotes request for block the corresponding cell
 *  \param bitmask Bitmask Value
 *  \param global_cell_id global cell id
 *  \param cell_block_priority block cell priority
 *  \param cell_block_resource_cleanup_timer cell block wait time for low priority (graceful shutdown)
 */
typedef struct _rrm_oam_cell_block_req_t
{
    rrm_bitmask_t                       bitmask ;               /*^ BITMASK ^*/
#define RRM_OAM_CELL_BLOCK_WAIT_TIMER_PRESENT 0x01
    rrm_oam_eutran_global_cell_id_t     global_cell_id;         /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_cell_block_priority_et      cell_block_priority;    /*^ M, 0, H, 0, 1 ^*/
    S16                                 cell_block_resource_cleanup_timer;  /*^ O, RRM_OAM_CELL_BLOCK_WAIT_TIMER_PRESENT, L, 1, 0 ^*/
}rrm_oam_cell_block_req_t; /*^ API, RRM_OAM_CELL_BLOCK_REQ ^*/

/* RRM_OAM_CELL_BLOCK_RESP */
/*! \ rrm_oam_cell_block_resp_t
 *  \brief This struct contains response for block cell request
 *  \param global_cell_id global cell id
 *  \param response response type success/failure
 *  \param fail_cause failure cause
 */
typedef struct _rrm_oam_cell_block_resp_t
{
    rrm_oam_eutran_global_cell_id_t global_cell_id; /*^ M, 0, N, 0, 0 ^*/
    rrm_return_et                   response;       /*^ M, 0, N, 0, 0 ^*/
    rrm_error_et                    fail_cause;     /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_block_resp_t;     /*^ API, RRM_OAM_CELL_BLOCK_RESP ^*/

/* RRM_OAM_CELL_UNBLOCK_CMD */
/*! \ rrm_oam_cell_unblock_cmd_t
 *  \brief This struct contains indication for unblock corresponding cell
 *  \param global_cell_id global cell id
 */
typedef struct _rrm_oam_cell_unblock_cmd_t
{
  rrm_oam_eutran_global_cell_id_t     global_cell_id; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_unblock_cmd_t; /*^ API, RRM_OAM_CELL_UNBLOCK_CMD ^*/

/* RRM_OAM_READY_FOR_CELL_BLOCK_IND */
/*! \ rrm_oam_ready_for_cell_block_ind_t 
 *  \brief This struct contains indication to oam that cell is ready to be blocked
 *  \param global_cell_id global cell id
 */
typedef struct _rrm_oam_ready_for_cell_block_ind_t
{
    rrm_oam_eutran_global_cell_id_t         global_cell_id; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_ready_for_cell_block_ind_t; /*^ API, RRM_OAM_READY_FOR_CELL_BLOCK_IND ^*/
/* End API for Soft lock feature */

/* RRM_OAM_CELL_UPDATE_REQ */
/*! \  rrm_oam_cell_update_req_t
 *  \brief This struct contains request for cell update
 *  \param global_cell_id global cell id
 *  \param pci_value PCI of detected eutran cells
 *  \param updated_plmn_info Denotes updated PLMN information
 *  \param conn_mode_cell_spec_off Denotes connection mode cell specific offset 
 *  \param idle_mode_cell_spec_off Denotes idle mode cell specific offset
 */
#define RRM_OAM_PCI_VALUE_PRESENT         0x01
#define RRM_OAM_UPDATED_PLMN_INFO_PRESENT 0x02
#define RRM_OAM_CM_CELL_SPEC_OFF_PRESENT  0x04
#define RRM_OAM_IM_CELL_SPEC_OFF_PRESENT  0x08
typedef struct _rrm_oam_cell_update_req_t
{
  rrm_bitmask_t                       bitmask; /*^ BITMASK ^*/
  rrm_oam_eutran_global_cell_id_t     global_cell_id; /*^ M, 0, N, 0, 0 ^*/
  U16           		      pci_value;   /*^ O, RRM_OAM_PCI_VALUE_PRESENT, H, 0, 503 ^*/
  rrm_oam_updated_plmn_info_t         updated_plmn_info; /*^ O, RRM_OAM_UPDATED_PLMN_INFO_PRESENT, N, 0, 0 ^*/
  S8                                  conn_mode_cell_spec_off; /*^ O, RRM_OAM_CM_CELL_SPEC_OFF_PRESENT, B, -24, 24 ^*/
  S8                                  idle_mode_cell_spec_off; /*^ O, RRM_OAM_IM_CELL_SPEC_OFF_PRESENT, B, -24, 24 ^*/
}rrm_oam_cell_update_req_t; /*^ API, RRM_OAM_CELL_UPDATE_REQ ^*/

/* RRM_OAM_CELL_UPDATE_RESP */
/*! \ rrm_oam_cell_update_resp_t
 *  \brief This struct denotes response for cell update
 *  \param global_cell_id Global Cell Identifier
 *  \param response  Success/Failure
 *  \param fail_cause Cause of failure
 */
typedef struct _rrm_oam_cell_update_resp_t
{
  rrm_oam_eutran_global_cell_id_t   global_cell_id; /*^ M, 0, N, 0, 0 ^*/
  rrm_return_et                     response;       /*^ M, 0, N, 0, 0 ^*/
  rrm_error_et                      fail_cause;     /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_update_resp_t; /*^ API, RRM_OAM_CELL_UPDATE_RESP ^*/

/* RRM_OAM_GET_VER_ID_RESP */
/*! \ rrm_oam_get_ver_id_resp_t
 *  \brief This struct denotes Response to oam version id
 *  \param response Denotes response type
 *  \param ver_id[RRM_MAX_VER_ID_LEN] Denotes version ID of response
 */
typedef struct _rrm_oam_get_ver_id_resp_t
{
   rrm_return_et      response; /*^ M, 0, N, 0, 0 ^*/
   U8                 ver_id[RRM_MAX_VER_ID_LEN]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_oam_get_ver_id_resp_t;    /*^ API, RRM_OAM_GET_VER_ID_RESP ^*/

/* RRM_OAM_EVENT_NOTIFICATION */
/*! \ rrm_oam_event_notification_t
 *  \brief This struct denotes send event triggered by the RRM. 
 *  \param bitmask Bitmask Value
 *  \param msg_header denotes message header 
 *  \param api_data[RRM_OAM_MAX_EVENT_LEN] denotes array of api data
 */
#define RRM_OAM_API_DATA_PRESENT  0x01
 typedef struct _rrm_oam_event_notification_t
 {
   rrm_bitmask_t             bitmask; /*^ BITMASK ^*/
   rrm_oam_event_header_t    msg_header; /*^ M, 0, N, 0, 0 ^*/
   U8		   	     api_data[RRM_OAM_MAX_EVENT_LEN]; /*^ O, RRM_OAM_API_DATA_PRESENT, OCTET_STRING, FIXED ^*/
 }rrm_oam_event_notification_t; /*^ API, RRM_OAM_EVENT_NOTIFICATION ^*/

/* RRM_OAM_LOAD_CONFIG_REQ */
/*! \ rrm_oam_load_config_req_t
 *  \brief This struct is Sent from OAM to RRM for configuring the action that has to be taken by RRM under different load conditions
 *  \param ncl_load_ind_intrvl Denote NCL Load Indication interval
 *  \param load_rpt_intrvl Indicate the interval at which the load information of the serving cells needs to be sent to OAM 
 *  \param num_enb_cells denotes Number of cells information present in the array
 *  \param serv_enb_cell_info[RRM_MAX_NUM_CELLS] This structure indicate the action that needs to be taken by the serving eNodeB depending upon the  load condition of eNodeB
 *  \param load_chk_periodicity: Indicate the periodicity at which the load level of the eNodeB is checked. 
 *  \param high_actn_repeat_lmt: Limit below the overload limit the actions to be applied repeatedly.
 */
#define RRM_OAM_NCL_LOAD_IND_INTRVL_PRESENT     0x01
#define RRM_OAM_LOAD_RPT_INTRVL_PRESENT         0x02
/* Bug_8718_start */
#define RRM_OAM_LOAD_CHK_PRDCTY_PRESENT         0x04
#define RRM_OAM_HIGH_ACT_RPT_LMT_PRESENT        0x08
/* Bug_8718_end */
/* SPR 20653 Fix Start */
#define RRM_OAM_TNL_LOAD_SUPPORT_FLAG_PRESENT   0x10
#define RRM_OAM_HW_LOAD_SUPPORT_FLAG_PRESENT    0x20
/* SPR 20653 Fix End */
typedef struct _rrm_oam_load_config_req_t
{
  rrm_bitmask_t 	           bitmask; /*^ BITMASK ^*/ 
  U8                           ncl_load_ind_intrvl;  /*^ O, RRM_OAM_NCL_LOAD_IND_INTRVL_PRESENT, H, 0, 10 ^*/
  U16                          load_rpt_intrvl; /*^ O, RRM_OAM_LOAD_RPT_INTRVL_PRESENT, H, 0, 300 ^*/
/*SPR_17664_start*/
  U16		                   num_enb_cells; /*^ M, 0, H, 0, RRM_MAX_NUM_CELLS ^*/
/*SPR_17664_emb*/
  rrm_oam_serving_enb_cell_info_t  serv_enb_cell_info[RRM_MAX_NUM_CELLS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
  /* Bug_8718_start */
  U16                           load_chk_periodicity; /*^ O, RRM_OAM_LOAD_CHK_PRDCTY_PRESENT, B, 5, 600 ^*/
  U8                            high_actn_repeat_lmt; /*^ O, RRM_OAM_HIGH_ACT_RPT_LMT_PRESENT, B, 5, 10 ^*/
  /* Bug_8718_end */
  /* SPR 20653 Fix Start */
  /* coverity_112363 Start */
  rrm_bool_t                      tnl_load_support_flag; /*^ O, RRM_OAM_TNL_LOAD_SUPPORT_FLAG_PRESENT, H, 0, 1 ^*/
  /* coverity_112363 Stop */
  /* coverity_35216 Start */
  rrm_bool_t                      hw_load_support_flag;  /*^ O, RRM_OAM_HW_LOAD_SUPPORT_FLAG_PRESENT, H, 0, 1 ^*/
  /* coverity_35216 Stop */
  /* SPR 20653 Fix End */
}rrm_oam_load_config_req_t; /*^ API, RRM_OAM_LOAD_CONFIG_REQ ^*/


/* RRM_OAM_LOAD_CONFIG_RESP */
/*! \ rrm_oam_load_config_resp_t
 *  \brief This struct is used to send from RRM to OAM after 
 *         successfully updating the values
 *  \param response success/failure
 *  \param fail_cause cause of failure
 */
typedef struct _rrm_oam_load_config_resp_t
{
  rrm_return_et                     response;       /*^ M, 0, N, 0, 0 ^*/
  rrm_error_et                      fail_cause;     /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_load_config_resp_t; /*^ API, RRM_OAM_LOAD_CONFIG_RESP ^*/

/* RRM_OAM_LOAD_REPORT */
/*! \  rrm_oam_load_report_ind_t 
 *  \brief This Struct contains load report of the cell 
 *  \param count For number of cells report present in the report. This value   is zero at present as neighbouring load information is not sent to OAM
 *  \param serv_cell_load_info contains information about the load information for the serving cell
 *  \param neigh_cell_load_info[RRM_MAX_NUM_CELLS] contains information about the load information for neighbouring cells of the serving cell
 */
typedef struct _rrm_oam_load_report_ind_t
{
  rrm_cell_load_info_t        serv_cell_load_info; /*^ M, 0, N, 0, 0 ^*/
/*SPR_17664_start*/
  U8                          count; /*^ M, 0, H, 0, RRM_MAX_NUM_CELLS^*/
/*SPR_17664_end*/
  rrm_cell_load_info_t        neigh_cell_load_info[RRM_MAX_NUM_CELLS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_oam_load_report_ind_t; /*^ API, RRM_OAM_LOAD_REPORT_IND ^*/

/*! \ rrm_oam_cell_ecn_capacity_enhance_req_t 
 *  \brief This struct contains the request for configuration of bitrate for QCI on RRM 
 *  \param bitmask Bitmask Value
 *  \param count To indicate the number of cells
 *  \param ecn_cells[RRM_MAX_NUM_CELLS] Cells for which ECN bitrate has been provided
 */
typedef struct _rrm_oam_cell_ecn_capacity_enhance_req_t
{
    rrm_bitmask_t bitmask; /*^ BITMASK ^*/
/*SPR_17664_start*/
    U8 count; /*^ M, 0, H, 0, RRM_MAX_NUM_CELLS ^*/
/*SPR_17664_end*/
    rrm_ecn_configure_cell_list_t ecn_cells[RRM_MAX_NUM_CELLS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_oam_cell_ecn_capacity_enhance_req_t; /*^ API, RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_REQ ^*/

/*! \ rrm_oam_cell_ecn_capacity_enhance_resp_t
 *  \brief This struct contains the response for  configuration of bitrate for QCI on RRM
 *  \param response success/failure
 *  \param fail_cause cause of failure
 */
typedef struct _rrm_oam_cell_ecn_capacity_enhance_resp_t
{
    rrm_return_et      response;       /*^ M, 0, N, 0, 0 ^*/
    rrm_error_et       fail_cause;     /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_cell_ecn_capacity_enhance_resp_t; /*^ API, RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_RESP ^*/

/* RRM_OAM_EVENT_CONFIG_REQ */
/*! \typedef rrm_oam_event_config_req
 *  \brief   This API is sent from OAM to 
 *           RRM for enabling/disabling notification & logging events
 *  \Params  no_of_items: Count of configuration provided.
 *  \Params  event_config: event configuration such as 
 *           event class(Protocol Eevent, Alarm), action 
 *           to be taken on event class 
 */
typedef struct _rrm_oam_event_config_req_t
{
  U16                     no_of_items; /*^ M, 0, H, 0, 10 ^*/
  rrm_oam_event_config_t  event_config[RRM_OAM_MAX_SUBCLASS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_oam_event_config_req_t;  /*^ API,  RRM_OAM_EVENT_CONFIG_REQ ^*/

/* RRM_OAM_EVENT_CONFIG_RESP */
/*! \typedef rrm_oam_event_config_resp
 *  \brief   This API is sent from RRM to OAM 
 *           in the response of rrm_oam_event_config_req
 */
typedef struct _rrm_oam_event_config_resp_t
{
   rrm_return_et      response;       /*^ M, 0, N, 0, 0 ^*/
   rrm_error_et       fail_cause;     /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_event_config_resp_t; /*^ API,  RRM_OAM_EVENT_CONFIG_RESP ^*/

/* RRM_OAM_KPI_IND*/
/*! \typedef rrm_oam_kpi_ind_t 
 *  \brief   This API is sent from RRM to OAM
 *           in the response of rrm_oam_kpi_ind_t 
 */
typedef struct _rrm_oam_kpi_ind_t
{
  rrm_oam_eutran_global_cell_id_t cell_id; /*^ M, 0, N, 0, 0 ^*/
  rrm_oam_kpi_data_t kpi_data; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_kpi_ind_t;/*^ API, RRM_OAM_KPI_IND ^*/ 
/* RRM_OAM_CONFIG_KPI_REQ */
/*! \typedef rrm_oam_config_kpi_req_t 
 *  \brief   This API is sent from OAM to RRM
 *           in the response of rrm_oam_config_kpi_req_t 
 */
#define RRM_OAM_CONFIG_KPI_DURATION_PRESENT 0x01
typedef struct _rrm_oam_config_kpi_req_t
{
   rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
   rrm_oam_eutran_global_cell_id_t cell_id; /*^ M, 0, N, 0, 0 ^*/
   U16                             duration; /*^ O, RRM_OAM_CONFIG_KPI_DURATION_PRESENT, B, 5, 3600 ^*/
   U8                              periodic_reporting; /*^ M, 0, H, 0, 1 ^*/
   rrm_oam_kpi_t                   kpi_to_report;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_config_kpi_req_t; /*^ API, RRM_OAM_CONFIG_KPI_REQ ^*/

/* RRM_OAM_CONFIG_KPI_RESP*/
/*! \typedef rrm_oam_config_kpi_resp_t 
 *  \brief   This API is sent from OAM to RRM
 *           in the response of rrm_oam_config_kpi_resp_t 
 */
#define RRM_OAM_CONFIG_KPI_CELL_ID_PRESENT 0x01
typedef struct _rrm_oam_config_kpi_resp_t
{
  rrm_bitmask_t bitmask; /*^ BITMASK ^*/
  rrm_oam_eutran_global_cell_id_t cell_id; /*^ O, RRM_OAM_CONFIG_KPI_CELL_ID_PRESENT, N, 0, 0 ^*/
  S32  response;/*^ M, 0, N, 0, 0 ^*/
  S32  fail_cause;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_config_kpi_resp_t;/*^ API, RRM_OAM_CONFIG_KPI_RESP ^*/

/* RRM_OAM_GET_KPI_REQ */
/*! \typedef rrm_oam_get_kpi_req_t 
 *  \brief   This API is sent from OAM to RRM
 *           in the response of rrm_oam_get_kpi_req_t 
 */
typedef struct _rrm_oam_get_kpi_req_t
{
   rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
   rrm_oam_eutran_global_cell_id_t cell_id; /*^ M, 0, N, 0, 0 ^*/
   rrm_oam_bool_t                  reset; /*^ M, 0, H, 0, 1 ^*/
   rrm_oam_kpi_t                   kpi_to_report;/*^ M, 0, N, 0, 0 ^*/
}rrm_oam_get_kpi_req_t; /*^ API, RRM_OAM_GET_KPI_REQ ^*/

/* RRM_OAM_GET_KPI_RESP */
/*! \typedef rrm_oam_get_kpi_resp_t
 *  \brief   This API is sent from RRM to OAM
 *           in the response of rrm_oam_get_kpi_resp_t 
 */
#define RRM_OAM_GET_KPI_CELL_ID_PRESENT 0x01
typedef struct _rrm_oam_get_kpi_resp_t
{
  rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
  rrm_oam_eutran_global_cell_id_t cell_id; /*^ O, RRM_OAM_GET_KPI_CELL_ID_PRESENT, N, 0, 0 ^*/
  S32  response;/*^ M, 0, N, 0, 0 ^*/
  S32  fail_cause;/*^ M, 0, N, 0, 0 ^*/
  rrm_oam_kpi_data_t kpi_data; /*^ M, 0, N, 0, 0 ^*/
}rrm_oam_get_kpi_resp_t;/*^ API, RRM_OAM_GET_KPI_RESP ^*/

/*RRM_OAM_CHK_HEALTH_REQ*/
/*! \typedef rrm_oam_chk_health_req_t
 *  \brief This API is sent from OAM to RRM
 *         to check the status of all threads
 *         of RRM
 */
typedef struct _rrm_oam_chk_health_req_t
{
 rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
}rrm_oam_chk_health_req_t;/*^ API ,RRM_OAM_CHK_HEALTH_REQ  ^*/

/*RRM_OAM_CHK_HEALTH_RESP*/
/*! \typedef rrm_oam_chk_health_resp_t
 *  \brief   This API is sent from RRM to OAM
 *           as response to rrm_oam_chk_health_req_t API
 */
typedef struct _rrm_oam_chk_health_resp_t
{
 rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
}rrm_oam_chk_health_resp_t;/*^ API ,RRM_OAM_CHK_HEALTH_RESP  ^*/

/* Carrier_Aggregation_Start */
/* RRM_OAM_ENB_CONFIG_REQ */
/*! \rrm_oam_enb_config_req_t 
 *  \brief This struct contains the request for configuration information
     related to eNodeB
 *  \param bitmask Bitmask Value
 *  \param enb_type To indicate the eNodeB type
 *  \param cell_stop_needed_for_nmm  To indicate if cell stop is required in
 *  case of NMM
 */
#define RRM_OAM_GLOBAL_ENB_ID_PRESENT               0x0001
#define RRM_OAM_CELL_STOP_NEEDED_FOR_NMM_PRESENT    0x0002
/*SPR 10329:start*/
#define RRM_OAM_GU_GROUP_INFO_PRESENT               0x0004
/*SPR 10329:end*/
/*NR_DC Code Change Start*/
#define RRM_OAM_QCI_FOR_SGNB_ADDITION_PRESENT       0x0100
/*NR_DC Code Change Stop*/
typedef struct _rrm_oam_enb_config_req_t
{
    rrm_bitmask_t           bitmask;                    /*^ BITMASK ^*/
    rrm_global_enb_id_t     enb_id;                     /*^ O, RRM_OAM_GLOBAL_ENB_ID_PRESENT, N, 0, 0 ^*/
    rrm_bool_et             cell_stop_needed_for_nmm;   /*^ O, RRM_OAM_CELL_STOP_NEEDED_FOR_NMM_PRESENT, N, 0, 0 ^*/
    /*SPR 10329:start*/
    rrm_oam_gu_group_info_t gu_group_info;              /*^ O, RRM_OAM_GU_GROUP_INFO_PRESENT, N, 0, 0 ^*/
    /*SPR 10329:end*/
/*NR_DC Code Change Start*/
    U8                      qci_for_sgnb_addition;     
/*^ O, RRM_OAM_QCI_FOR_SGNB_ADDITION_PRESENT, B, 1, 9 ^*/
/*NR_DC Code Change Stop*/
}rrm_oam_enb_config_req_t; /*^ API, RRM_OAM_ENB_CONFIG_REQ ^*/
    
/* RRM_OAM_ENB_CONFIG_RESP */
/*! \rrm_oam_enb_config_resp_t
 *  \brief This struct contains the response for configuration of eNodeB request
 *  \param response success/failure
 *  \param fail_cause cause of failure
 */
/*SPR 10329:start*/
#define RRM_OAM_GU_GROUP_RESPONSE_LIST_PRESENT      0x01 
/*SPR 10329:end*/
typedef struct _rrm_oam_enb_config_resp_t
{
    /*SPR 10329:start*/
    rrm_bitmask_t       bitmask;        /*^ BITMASK ^*/
    /*SPR 10329:end*/
    rrm_return_et       response;       /*^ M, 0, N, 0, 0 ^*/
    rrm_error_et        fail_cause;     /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_gu_group_info_success_failure_list_t   gu_group_response_list;
                                        /*^ O, RRM_OAM_GU_GROUP_RESPONSE_LIST_PRESENT, N, 0, 0 ^*/ 
}rrm_oam_enb_config_resp_t; /*^ API, RRM_OAM_ENB_CONFIG_RESP ^*/

/* Carrier_Aggregation_End */

/* SPR_13117_FIX_START */
/*
 *   RRM_OAM_GET_LOG_LEVEL_REQ
 */

/*! \ rrm_oam_get_log_level_req_t
 *  \brief This struct contains Request from OAM to get the log level of RRM internal module
 *  \param module_id Internal Module ID
 */
typedef struct _rrm_oam_get_log_level_req_t
{
    rrm_bitmask_t                   bitmask;  /*^ BITMASK ^*/
    rrm_internal_module_id_et       module_id;  /*^ M, 0, N, 0, 0  ^*/
}rrm_oam_get_log_level_req_t;   /*^ API, RRM_OAM_GET_LOG_LEVEL_REQ ^*/

#define RRM_OAM_MODULE_LOG_LEVEL_PRESENT 0x0001
typedef struct _rrm_oam_log_level_module_t
{
    rrm_bitmask_t                   bitmask;  /*^ BITMASK ^*/
    rrm_internal_module_id_et       module_id;  /*^ M, 0, B, 21, 23  ^*/
    rrm_error_et                    error_code; /*^ M, 0, N, 0, 0 ^*/
    /* coverity 94838/94849/94858/94860/35170/35221/35283/94809 fix */
    rrm_oam_log_level_et            log_level;  /*^ O, RRM_OAM_MODULE_LOG_LEVEL_PRESENT, H, 0, 6 ^*/
}rrm_oam_log_level_module_t;

/*! \ rrm_oam_get_log_level_resp_t
 *  \brief This struct denotes Reponse to get log level req
 *  \param response   success/fail
 *  \param fail_cause fail cause
 *  \param num_of_modules number of modules we fill in array
 *  \module_id  array of module levels
 */
typedef struct _rrm_oam_get_log_level_resp_t
{
    rrm_bitmask_t                   bitmask;  /*^ BITMASK ^*/
    rrm_return_et                   response;  /*^ M, 0, N, 0, 0 ^*/
    rrm_error_et                    fail_cause;  /*^ M, 0, N, 0, 0 ^*/
    U8                              num_of_modules; /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_log_level_module_t      module_id[RRM_MAX_NUM_INT_MODULES]; /*^ M ,0, OCTET_STRING, VARIABLE ^*/
}rrm_oam_get_log_level_resp_t;                /*^ API, RRM_OAM_GET_LOG_LEVEL_RESP ^*/
/* SPR_13117_FIX_END */
/*spr 22474 start*/
/* This section defines the common structure used across layer-2 modules with OAM */
/* This structure provides the POOL stats */
typedef struct rrm_PoolStatsT {
    /* the size of the buffer allocated in this pool */
    U32 bufSize; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers in this pool */
    U32 numOfBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers allocated from this pool */
    U32 numOfAllocBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Peak of Total number of Buffers allocated from this pool during the system is running */
    U32 numOfPeakBuf; /*^ M, 0, N, 0, 0 ^*/
}rrm_PoolStats;

/* This structure provides the MEM POOL stats */
typedef struct rrm_MemPoolStatsT {
    /* This array is terminated when bufSize is 0 for an entry, other entries are not valid */
    rrm_PoolStats stats[NVARPOOL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_MemPoolStats;



/* RRM_OAM_GET_DEBUG_INFO_REQ */
/*! \typedef rrm_oam_get_debug_info_req_t 
 *  \brief   This API is sent from OAM to RRM
 *           to request for debug stats
 */
typedef struct _rrm_oam_get_debug_info_req_t
{
   U16                   debug_type;/*^ M, 0, N, 0, 0 ^*/
   /*rrm_oam_debug_info_req_et*/
}rrm_oam_get_debug_info_req_t; /*^ API, RRM_OAM_GET_DEBUG_INFO_REQ ^*/

/* RRM_OAM_GET_DEBUG_INFO_RESP */
/*! \typedef rrm_oam_get_debug_info_resp_t
 *  \brief   This API is sent from RRM to OAM
 *           in the response of rrm_oam_get_debug_info_req_t 
 */
#define RRM_OAM_DEBUG_GET_MEM_POOL_PRESENT 0x01
#define RRM_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT 0x02
typedef struct _rrm_oam_get_debug_info_resp_t
{
  rrm_bitmask_t     bitmask; /*^ BITMASK ^*/
  S32  response;/*^ M, 0, N, 0, 0 ^*/
  S32  fail_cause;/*^ O, RRM_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT, N, 0, 0 ^*/
  /* rrm_oam_debug_info_fail_cause_et */
  rrm_MemPoolStats csplMemPoolStats;/*^ O, RRM_OAM_DEBUG_GET_MEM_POOL_PRESENT, N, 0, 0 ^*/
}rrm_oam_get_debug_info_resp_t;/*^ API, RRM_OAM_GET_DEBUG_INFO_RESP ^*/
/*spr 22474 end*/

/* SPR_13117_FIX_END */

#endif /* _RRM_OAM_INTF_H */
