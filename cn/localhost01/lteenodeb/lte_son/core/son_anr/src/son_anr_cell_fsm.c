/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_anr_cell_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains ANR per cell FSM functions
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *  DATE            AUTHOR              REFERENCE       REASON
 *  Feb, 2012       Komal/Shekhar                       Initial
 *  May, 2012       Komal/Shekhar/Shilpi                Release 1.1 Changes
 *  Aug, 2012       Komal/Shekhar                       GERAN support added
 *  June,2013       Rashi/Swetank                       CDMA support added
 *  May, 2014       Shilpi                              Coverity Fixes  
 *  July, 2014      Shilpi                              SPR 12483 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_anr_cell_fsm.h>
#include <son_anr_ctxt_mgr.h>
#include <son_utils.h>
#include <rrm_son_composer.h>
#include <son_rrm_composer.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_anr_utils.h>
#include <son_mro_ctxt_mgr.h>
#include <son_mlb_ctxt_mgr.h>
#include <son_rach_opt_ctxt_mgr.h>
#include <son_anr_table.h> /*SPR-8950*/ 

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern const son_8 *p_son_anr_facility_name;
extern son_global_config_params_t g_config_params; /*Added for SPR-9759*/
#define SON_MAX_U8_VAL  255

/*****************************************************************************
 * Function Name  : anr_map_son_cdma_band_class
 * Inputs         : 
 *                : band_class    -  SON CDMA2000 band_class  
 * Outputs        : None
 * Returns        : rrm_son_cdma_band_class_et - CDMA2000 band class at RRM
 * Description    : This function maps and returns RRM CDMA2000 band class for
 *                  the received SON CDMA2000 band class
 ****************************************************************************/
static rrm_son_cdma_band_class_et
anr_map_son_cdma_band_class
(
/*SPR 17777 +-*/ 
 son_cdma_band_class_et  band_class
 )
{
	rrm_son_cdma_band_class_et  ret_val = RRM_SON_BAND_CLASS_BC_0;


	SON_UT_TRACE_ENTER();

	switch (band_class)
	{
		case BC0:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_0;
				break;
			}

		case BC1:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_1;
				break;
			}

		case BC2:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_2;
				break;
			}

		case BC3:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_3;
				break;
			}

		case BC4:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_4;
				break;
			}

		case BC5:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_5;
				break;
			}

		case BC6:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_6;
				break;
			}

		case BC7:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_7;
				break;
			}

		case BC8:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_8;
				break;
			}

		case BC9:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_9;
				break;
			}

		case BC10:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_10;
				break;
			}

		case BC11:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_11;
				break;
			}

		case BC12:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_12;
				break;
			}

		case BC13:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_13;
				break;
			}

		case BC14:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_14;
				break;
			}

		case BC15:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_15;
				break;
			}

		case BC16:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_16;
				break;
			}

		case BC17:
			{
				ret_val = RRM_SON_BAND_CLASS_BC_17;
				break;
			}

		case BC18_V9A0:
			{
				ret_val = RRM_SON_BAND_CLASS_BC18_V9A0;
				break;
			}

		case BC19_V9A0:
			{
				ret_val = RRM_SON_BAND_CLASS_BC19_V9A0;
				break;
			}

		case BC20_V9A0:
			{
				ret_val = RRM_SON_BAND_CLASS_BC20_V9A0;
				break;
			}

		case BC21_V9A0:
			{
				ret_val = RRM_SON_BAND_CLASS_BC21_V9A0;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"SON cdma band class %d received could not "
						"be mapped",
						band_class);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_son_cdma_band_class */

/*****************************************************************************
 * Function Name  : anr_map_son_cdma_type
 * Inputs         : 
 *                : cdma_type     -  SON CDMA type
 * Outputs        : None
 * Returns        : rrm_son_cdma2000_type_et - CDMA2000 band class at RRM
 * Description    : This function maps and returns RRM CDMA type for the 
 *                  received SON cdma type
 ****************************************************************************/
static rrm_son_cdma2000_type_et
anr_map_son_cdma_type
(
/*SPR 17777 +-*/ 
 son_cdma2000_type_et    cdma_type
 )
{
	rrm_son_cdma2000_type_et    ret_val = R_TYPE_1X_RTT;


	SON_UT_TRACE_ENTER();

	switch (cdma_type)
	{
		case TYPE_1X_RTT:
			{
				ret_val = R_TYPE_1X_RTT;
				break;
			}

		case TYPE_HRPD:
			{
				ret_val = R_TYPE_HRPD;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"SON cdma type %d received could not "
						"be mapped",
						cdma_type);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_son_cdma_type */

/***************************************************************************************
 * Function Name  : anr_map_rrm_return_code
 * Inputs         : p_son_func_name   - Pointer to calling function name
 *                : result          - Return Code received from RRM
 * Outputs        : None
 * Returns        : son_return_et   - Returns SON_SUCCESS if result value is RRM_SUCCESS 
 *                                    returns SON_FAILURE if result is RRM_FAILURE and 
 *                                    returns SON_PARTIAL_SUCCESS if result is RRM_PARTIAL_SUCCESS
 * Description    : This function maps and returns SON result code for the received RRM
 *                : return code
 ****************************************************************************************/
static son_return_et
anr_map_rrm_return_code
(
 const son_8     *p_son_func_name,
 rrm_return_et   result
 )
{
	son_return_et ret_val = SON_FAILURE;




	SON_UT_TRACE_ENTER();

	if (p_son_func_name == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"calling function name is NULL"
				);
		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	switch (result)
	{
		case RRM_SUCCESS:
			{
				ret_val = SON_SUCCESS;
				break;
			}

		case RRM_FAILURE:
			{
				ret_val = SON_FAILURE;
				break;
			}

		case RRM_PARTIAL_SUCCESS:
			{
				ret_val = SON_PARTIAL_SUCCESS;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						" RRM return code %d received could not "
						"be mapped",
						result);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_rrm_return_code */

/*****************************************************************************
 * Function Name  : anr_map_son_offset_frequency
 * Inputs         : p_son_func_name        - Pointer to calling function name
 *                : son_offset_frequency - SON meas bandwidth received in request
 * Outputs        : None
 * Returns        : rrm_q_offsetrange_et - Enum describing cell specific offset 
 *                                         frequency for RRM
 * Description    : This function maps and returns RRM cell specific offset 
 *                  frequency for the received SON offset frequency
 ****************************************************************************/
/* Coverity Fix: 19148 */
static rrm_q_offsetrange_et
anr_map_son_offset_frequency
(
 const son_8             *p_son_func_name,
 son_q_offset_range_et   son_offset_frequency
 )
{
	/* Coverity Fix: 26826 */
	rrm_q_offsetrange_et ret_val = RRM_Q_OFFSET_RANGE_DB_24;




	SON_UT_TRACE_ENTER();

	if (p_son_func_name == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" calling function name is NULL"
				);
		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	switch (son_offset_frequency)
	{
		case SON_Q_OFFSET_RANGE_DB_24:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_24;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_22:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_22;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_20:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_20;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_18:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_18;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_16:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_16;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_14:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_14;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_12:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_12;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_10:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_10;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_8:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_8;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_6:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_6;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_5:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_5;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_4:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_4;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_3:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_3;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_2:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_2;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB_1:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB_1;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB0:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB0;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB1:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB1;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB2:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB2;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB3:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB3;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB4:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB4;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB5:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB5;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB6:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB6;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB8:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB8;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB10:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB10;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB12:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB12;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB14:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB14;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB16:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB16;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB18:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB18;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB20:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB20;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB22:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB22;
				break;
			}

		case SON_Q_OFFSET_RANGE_DB24:
			{
				ret_val = RRM_Q_OFFSET_RANGE_DB24;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						" SON offset frequency %d received could not "
						"be mapped",
						son_offset_frequency);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_son_offset_frequency */

/*****************************************************************************
 * Function Name  : anr_map_son_meas_bandwidth
 * Inputs         : p_son_func_name       - Pointer to calling function name
 *                : son_meas_bandwidth  - SON meas bandwidth received in req
 * Outputs        : None
 * Returns        : rrm_allowed_meas_bandwidth_et - An enum that specifies type 
 *                                                  allowed bandwidth for RRM
 * Description    : This function maps and returns RRM meas bandwidth for the 
 *                  received SON meas bandwidth
 ****************************************************************************/
/* Coverity Fix: 19151 */
static rrm_allowed_meas_bandwidth_et
anr_map_son_meas_bandwidth
(
 const son_8 *p_son_func_name,
 son_allowed_meas_bandwidth_et son_meas_bandwidth
 )
{
	/* Coverity Fix: 26825 */
	rrm_allowed_meas_bandwidth_et ret_val = RRM_ALWD_BW_MBW_6;




	SON_UT_TRACE_ENTER();

	if (p_son_func_name == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" calling function name is NULL"
				);
		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	switch (son_meas_bandwidth)
	{
		case SON_ALWD_BW_MBW_6:
			{
				ret_val = RRM_ALWD_BW_MBW_6;
				break;
			}

		case SON_ALWD_BW_MBW_15:
			{
				ret_val = RRM_ALWD_BW_MBW_15;
				break;
			}

		case SON_ALWD_BW_MBW_25:
			{
				ret_val = RRM_ALWD_BW_MBW_25;
				break;
			}

		case SON_ALWD_BW_MBW_50:
			{
				ret_val = RRM_ALWD_BW_MBW_50;
				break;
			}

		case SON_ALWD_BW_MBW_75:
			{
				ret_val = RRM_ALWD_BW_MBW_75;
				break;
			}

		case SON_ALWD_BW_MBW_100:
			{
				ret_val = RRM_ALWD_BW_MBW_100;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						" SON meas bandwidth %d received could not "
						"be mapped",
						son_meas_bandwidth);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_son_meas_bandwidth */

/*****************************************************************************
 * Function Name  : anr_map_rrm_band_ind
 * Inputs         : p_son_func_name  - Pointer to calling function name
 *                : rrm_band_ind   - Geran nbr band_ind received from RRM
 * Outputs        : None
 * Returns        : son_geran_band_indicator_et - An enum that specifies bands 
 *                                                for GERAN carrier frequency
 * Description    : This function maps and returns SON band ind for the received
 *                  RRM band ind
 ****************************************************************************/
/* Coverity Fix: 19154 */
static son_geran_band_indicator_et
anr_map_rrm_band_ind
(
 const son_8 *p_son_func_name,
 rrm_son_geran_band_indicator_et rrm_band_ind
 )
{
	/* Coverity Fix: 26828 */
	son_geran_band_indicator_et ret_val = SON_GERAN_DCS_1800;




	SON_UT_TRACE_ENTER();

	if (p_son_func_name == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" calling function name is NULL"
				);
		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	switch (rrm_band_ind)
	{
		case RRM_SON_GERAN_DCS_1800:
			{
				ret_val = SON_GERAN_DCS_1800;
				break;
			}

		case RRM_SON_GERAN_PCS_1900:
			{
				ret_val = SON_GERAN_PCS_1900;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						" RRM Band ind for Geran nbr %d received could not "
						"be mapped",
						rrm_band_ind);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_rrm_band_ind */

/*****************************************************************************
 * Function Name  : anr_map_rrm_error_codes
 * Inputs         : p_son_func_name   - Pointer to calling function name
 *                : result          - Result received by SON
 *                : rrm_error_code  - Error Code received from RRM
 *                : msg_id          - API Id of message received from RRM
 * Outputs        : None
 * Returns        : son_error_et    - Returns SON error code
 * Description    : This function maps and returns SON error code for the
 *                  received RRM error code
 ****************************************************************************/
son_error_et
anr_map_rrm_error_codes
(
 const son_8 *p_son_func_name,
 son_return_et result,
 rrm_error_et rrm_error_code,
 rrm_son_message_resp_et msg_id
 )
{
	son_error_et ret_val = SON_NO_ERROR;




	SON_UT_TRACE_ENTER();

	if (p_son_func_name == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" calling function name is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_ERR_RRM_FAILURE;
	}

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_INFO,
			" Message %d received from RRM with result: %d and error code: %d",
			msg_id,
			result,
			rrm_error_code);

	switch (rrm_error_code)
	{
		case RRM_ERR_INVALID_PARAMS:
			/*Rel 1.2 changes*/
		case RRM_ERR_MANDATORY_PARAMS_ABSENT:
			{
				ret_val = SON_ERR_INVALID_PARAMS;
				break;
			}

		case RRM_ERR_UNEXPECTED_MSG:
		case RRM_ERR_CELL_ALREADY_REGISTER_FAILURE:
		case RRM_ERR_CELL_ALREADY_DEREGISTER_FAILURE:
		case RRM_ERR_EVENT_NOT_POSSIBLE:
		case RRM_ERR_CELL_SET_ATTR_FAILURE:
		case RRM_ERR_CELL_UNCONFIGURED:
		case RRM_ERR_OBJECT_NON_EXISTENT:
			/*Rel 1.2 changes*/
		case RRM_ERR_MAX_INTER_FREQS_CONFIGURED:
		case RRM_ERR_MAX_UTRA_FDD_FREQS_CONFIGURED:
		case RRM_ERR_MAX_UTRA_TDD_FREQS_CONFIGURED:
		case RRM_ERR_MAX_GERAN_FREQS_CONFIGURED:
		case RRM_ERR_NO_UTRAN_FREQ_CONFIGURED:
		case RRM_ERR_NO_GERAN_FREQ_CONFIGURED:
		case RRM_ERR_MAX_MEAS_OBJ_CREATED:
		case RRM_ERR_CELL_RECONFIG_ONGOING:
		case RRM_ERR_CELL_START_ONGOING:
		case RRM_ERR_CELL_DELETION_ONGOING:
		case RRM_ERR_CELL_STOP_ONGOING:
		case RRM_ERR_CELL_UNOPERATIONAL: 
			{
				ret_val = SON_ERR_RRM_FAILURE;
				break;
			}

		case RRM_ERR_EMERGENCY_CALL_ONGOING:
			{
				ret_val = SON_ERR_EMERGENCY_CALL_ONGOING;
				break;
			}

		case RRM_ERR_CELL_ALREADY_SWITCHED_ON:
			{
				ret_val = SON_ERR_ES_CELL_SWITCHED_ON;
				break;
			}

		case RRM_ERR_CELL_ALREADY_SWITCHED_OFF:
			{
				ret_val = SON_ERR_ES_CELL_SWITCHED_OFF;
				break;
			}

		case RRM_ERR_DEREGISTERATION_FAILURE:
			{
				ret_val = SON_ERR_DEREGISTERATION_FAILURE;
				break;
			}

		case RRM_ERR_REGISTERATION_FAILURE:
			{
				ret_val = SON_ERR_REGISTERATION_FAILURE;
				break;
			}
			/* SPR-612 Coverity Fix Starts */
			/* Deleted the commented line */
			/* SPR-612 Coverity Fix Ends */
		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						" RRM Error Code %d received in message %d could not "
						"be mapped",
						rrm_error_code,
						msg_id);
				ret_val = SON_ERR_RRM_FAILURE;
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_map_rrm_error_codes */

/*****************************************************************************
 * Function Name  : anr_construct_send_rrm_register_req
 * Inputs         : transaction_id     - transaction identity   
 *                : p_cell_id          - Pointer to Cell_Id
 *                : ue_cnt             - Number of UEs to configure for UE 
 *                                       Measurement Reporting
 *                : reporting_interval - Interval at which RRM shall send
 *                                       the UE Measurements for ANR
 * Outputs        : None
 * Returns        : son_return_et      - Returns SON result code
 * Description    : This function constructs and sends RRM_REGISTER_REQ
 ****************************************************************************/
static son_return_et
anr_construct_send_rrm_register_req
(
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_u8 ue_cnt,
 /* SPR 20653 Fix Start */
 son_u8 reporting_interval,
 son_u8 ue_throughput_dl_anr_threshold,
 son_u8 ue_throughput_ul_anr_threshold
 /* SPR 20653 Fix End */
 )
{
	son_rrm_register_req_t req = {0};
	son_return_et ret_val = SON_SUCCESS;


	SON_UT_TRACE_ENTER();

	if (p_cell_id == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to cell id is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	req.transaction_id = transaction_id;
	req.req.register_req.object.type = RRM_OBJ_CELL;

	SON_MEMCPY(&(req.req.register_req.object.id.object_id.cell_id), p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
    /* SPR 20653 Fix Start */
	req.req.register_req.object.attr_list_size = SON_NULL;
    /* SPR 20653 Fix End */

	/* CELL STATE REGISTERATION */
	req.req.register_req.object.p_attr_lst[0].id = RRM_ATTR_CELL_STATE;
	req.req.register_req.object.p_attr_lst[0].trgr_list_size = 1;
	req.req.register_req.object.p_attr_lst[0].trgr_list[0].id =
		RRM_TRGR_EVENT;
	req.req.register_req.object.p_attr_lst[0].trgr_list[0].bitmask |=
		RRM_TRIGGER_TYPE_EVENT;
	req.req.register_req.object.p_attr_lst[0].trgr_list[0].
		event.enable = SON_TRUE;
    /* SPR 20653 Fix Start */
	req.req.register_req.object.attr_list_size++;
    /* SPR 20653 Fix End */

	/* Number of UEs for UE Measurement REGISTRATION */
	req.req.register_req.object.p_attr_lst[1].id = RRM_ATTR_MEAS_UE_COUNT;
	req.req.register_req.object.p_attr_lst[1].value = ue_cnt; 
    /* SPR 20653 Fix Start */
	req.req.register_req.object.attr_list_size++;
    /* SPR 20653 Fix End */

	/* Measurement reporting interval REGISTRATION */
	req.req.register_req.object.p_attr_lst[2].id = 
		RRM_ATTR_MEAS_REPORTING_INTERVAL;
	req.req.register_req.object.p_attr_lst[2].trgr_list_size = 1;
	req.req.register_req.object.p_attr_lst[2].trgr_list[0].id = 
		RRM_TRGR_PERIODIC;
	req.req.register_req.object.p_attr_lst[2].trgr_list[0].bitmask |=
		RRM_TRIGGER_TYPE_PERIODIC;
	req.req.register_req.object.p_attr_lst[2].trgr_list[0].periodic.interval =
		reporting_interval * 60;
    /* SPR 20653 Fix Start */
	req.req.register_req.object.attr_list_size++;
    /* SPR 20653 Fix End */

	/* HO Report REGISTRATION */
	req.req.register_req.object.p_attr_lst[3].id = RRM_ATTR_HO_REPORT;
	req.req.register_req.object.p_attr_lst[3].trgr_list_size = 1;
	req.req.register_req.object.p_attr_lst[3].trgr_list[0].id =
		RRM_TRGR_EVENT;
	req.req.register_req.object.p_attr_lst[3].trgr_list[0].bitmask |=
		RRM_TRIGGER_TYPE_EVENT;
	req.req.register_req.object.p_attr_lst[3].trgr_list[0].
		event.enable = SON_TRUE;
    /* SPR 20653 Fix Start */
	req.req.register_req.object.attr_list_size++;
    /* SPR 20653 Fix End */

    /* SPR 20653 Fix Start */
    /* ANR Attribute REGISTRATION */
	req.req.register_req.object.p_attr_lst[4].id = RRM_ATTR_UE_THROUGHPUT_DL_ANR_THRESHOLD;
	req.req.register_req.object.p_attr_lst[4].value = ue_throughput_dl_anr_threshold; 
	req.req.register_req.object.attr_list_size++;

	req.req.register_req.object.p_attr_lst[5].id = RRM_ATTR_UE_THROUGHPUT_UL_ANR_THRESHOLD;
	req.req.register_req.object.p_attr_lst[5].value = ue_throughput_ul_anr_threshold; 
	req.req.register_req.object.attr_list_size++;
    /* SPR 20653 Fix End */
	/*Invoke PUP tool call to send message to RRM*/
	if (RRM_FAILURE == rrm_son_send_rrm_son_register_req(&req.req,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				transaction_id,
				SON_NULL))
	{
		ret_val = SON_FAILURE;
	}
	else
	{
        /* SPR 20653 Fix Start */
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_INFO,
				"Message %u send from %u to"
				" %u for cell [Cell Id: 0x%x], UE count = %u meas interval = %u,"
                " ue_throughput_dl_anr_threshold = %u, ue_throughput_ul_anr_threshold = %u",
				RRM_SON_REGISTER_REQ,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity),
				(son_u16)ue_cnt,
				(son_u16)reporting_interval,
                (son_u16)ue_throughput_dl_anr_threshold,
                (son_u16)ue_throughput_ul_anr_threshold);
        /* SPR 20653 Fix End */
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_construct_send_rrm_register_req */

/*****************************************************************************
 * Function Name  : anr_construct_send_rrm_deregister_req
 * Inputs         : transaction_id  - transaction identity
 *                : p_cell_id       - Pointer to Cell_Id
 * Outputs        : None
 * Returns        : son_return_et   - Returns SON result code
 * Description    : This function constructs and sends RRM_DEREGISTER_REQ
 ****************************************************************************/
son_return_et
anr_construct_send_rrm_deregister_req
(
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	son_rrm_deregister_req_t req = {0};
	son_return_et ret_val = SON_SUCCESS;


	if (p_cell_id == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to cell id is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_UT_TRACE_ENTER();

	req.transaction_id = transaction_id;
	req.req.deregister_req.object.type = RRM_OBJ_CELL;

	SON_MEMCPY(&(req.req.deregister_req.object.id.object_id.cell_id), p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	req.req.deregister_req.object.attr_list_size = 4;

	/* CELL STATE DEREGISTERATION */
	req.req.deregister_req.object.p_attr_lst[0].id = RRM_ATTR_CELL_STATE;
	req.req.deregister_req.object.p_attr_lst[0].trgr_list_size = 0;

	/* Number of UEs for UE Measurement DEREGISTRATION */
	req.req.deregister_req.object.p_attr_lst[1].id = RRM_ATTR_MEAS_UE_COUNT;
	req.req.deregister_req.object.p_attr_lst[1].trgr_list_size = 0;

	/* Measurement reporting interval DEREGISTRATION */
	req.req.deregister_req.object.p_attr_lst[2].id =
		RRM_ATTR_MEAS_REPORTING_INTERVAL;
	req.req.deregister_req.object.p_attr_lst[2].trgr_list_size = 0;

	/* HO Report DEREGISTRATION */
	req.req.deregister_req.object.p_attr_lst[3].id =
		RRM_ATTR_HO_REPORT;
	req.req.deregister_req.object.p_attr_lst[3].trgr_list_size = 0;

	/*Invoke PUP tool call to send message to RRM*/
	if (RRM_FAILURE == rrm_son_send_rrm_son_deregister_req(&req.req,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				transaction_id,
				SON_NULL))
	{
		ret_val = SON_FAILURE;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_INFO,
				"Message %u send from %u to"
				" %u for cell [Cell Id: 0x%x]",
				RRM_SON_DEREGISTER_REQ,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_construct_send_rrm_deregister_req */

/***************************************************************************************
 * Function Name  : anr_fsm_anr_enable_req
 * Inputs         : p_cspl_hdr  - Pointer to cspl header
 *                : p_cell_ctxt - Pointer to cell context
 *                : evnt_id     - Event id
 *                : p_msg       - Message to be send
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is Event Handler for SONANR_ENABLE_REQ.It is invoked when
 *                : current_cell_fsm_state is ANR_CELL_STATE_DISABLED and
 *                  ANR_CELL_STATE_DISABLING
 **************************************************************************************/
static son_buf_retain_status_et
anr_fsm_anr_enable_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t  *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_enable_req_t     *p_req     = (son_anr_enable_req_t *)(p_msg);
	son_u16                  reporting_interval = SON_ANR_DEFAULT_REPORTING_INTERVAL;
	son_u8                   ue_cnt = SON_ANR_DEFAULT_UE_COUNT; 
    /* SPR 20653 Fix Start */
    son_u8                   ue_throughput_dl_anr_threshold = SON_DEFAULT_UE_THROUGHPUT_DL_ANR_THRESHOLD; 
    son_u8                   ue_throughput_ul_anr_threshold = SON_DEFAULT_UE_THROUGHPUT_UL_ANR_THRESHOLD;
    /* SPR 20653 Fix End */


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL)
	{
/*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
                "Pointer to Cell Context is NULL evnt_id %u p_cspl_hdr %p"
				,evnt_id,p_cspl_hdr);
/*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	if (p_req->enable_req.bitmask & SON_REPORTNG_INTERVAL_PRESENT)
	{
		reporting_interval = p_req->enable_req.reporting_interval;
	}

	if (p_req->enable_req.bitmask & SON_UE_CNT_PRESENT)
	{
		ue_cnt = p_req->enable_req.ue_cnt;
	}
    /* SPR 20653 Fix Start */
	if (p_req->enable_req.bitmask & SON_UE_THROUGHPUT_DL_ANR_THRESHOLD_PRESENT)
	{
		ue_throughput_dl_anr_threshold = p_req->enable_req.ue_throughput_dl_anr_threshold;
	}
	if (p_req->enable_req.bitmask & SON_UE_THROUGHPUT_UL_ANR_THRESHOLD_PRESENT)
	{
		ue_throughput_ul_anr_threshold = p_req->enable_req.ue_throughput_ul_anr_threshold;
	}
    /* SPR 20653 Fix End */

	if (SON_SUCCESS == anr_construct_send_rrm_register_req(
				p_req->transaction_id,
				&p_cell_ctxt->cell_id,
				ue_cnt,
                /* SPR 20653 Fix Start */
				reporting_interval,
                ue_throughput_dl_anr_threshold,
                ue_throughput_ul_anr_threshold))
        /* SPR 20653 Fix End */
	{
		SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ANR_CELL_STATE_ENABLING);

		if (SON_LIMITED_MODE_PRESENT & p_req->enable_req.bitmask)
		{
			p_cell_ctxt->limited_mode =
				p_req->enable_req.limited_mode;
		}

		if ((SON_REMOVAL_CRITERIA_PRESENT & p_req->enable_req.bitmask) &&
				p_req->enable_req.removal_criteria.bitmask)
		{
			SON_MEMCPY(&p_cell_ctxt->removal_criteria,
					&p_req->enable_req.removal_criteria,
					sizeof(p_cell_ctxt->removal_criteria));
		}

		if ((SON_PCI_CONFUSION_CFG_PRESENT & p_req->enable_req.bitmask) &&
				p_req->enable_req.pci_confusion_cfg.bitmask)
		{
			SON_MEMCPY(&p_cell_ctxt->pci_confusion_cfg,
					&p_req->enable_req.pci_confusion_cfg,
					sizeof(p_cell_ctxt->pci_confusion_cfg));
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Register Request could not be sent"
				" for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_anr_enable_req */

/*****************************************************************************
 * Function Name  : anr_populate_meas_config_req_cdma_freq_info
 * Inputs         : p_in_cf_info  - Pointer to the CDMA2000 neighbor carrier freq 
 *                  info received from OAM
 * Outputs        : p_out_cf_info - Pointer to the CDMA2000 neighbor carrier freq 
 *                  info  to be sent to RRM
 * Returns        : None
 * Description    : This function populates the CDMA nbr carrier frq info to 
 *                  be send to RRM from the info received in msg from OAM  
 ****************************************************************************/
static son_void_t
anr_populate_meas_config_req_cdma_freq_info
(
 const son_cdma_carrier_freq_info_t *p_in_cf_info,
 rrm_son_cdma_carrier_freq_info_t   *p_out_cf_info
 )
{
	SON_UT_TRACE_ENTER(); 

	if (p_in_cf_info == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to CDMA2000 carrier frequency info is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	if ((p_in_cf_info->cdma_type == TYPE_1X_RTT) ||
			(p_in_cf_info->cdma_type == TYPE_HRPD))
	{
		/* Coverity Fix: 24314 */
		p_out_cf_info->cdma_type = anr_map_son_cdma_type(
/*SPR 17777 +-*/ 
				p_in_cf_info->cdma_type
				);
	}

	/* Coverity Fix: 20892 */
	if (p_in_cf_info->band_class <= BC21_V9A0)
	{
		/* Coverity Fix: 30291, 30292 */
		p_out_cf_info->band_class = anr_map_son_cdma_band_class(
/*SPR 17777 +-*/ 
				p_in_cf_info->band_class
				);
	}

	if (p_in_cf_info->arfcn <= 2047)
	{
		p_out_cf_info->arfcn = p_in_cf_info->arfcn;
	}

	if (p_in_cf_info->threshX_high <= 63)
	{
		p_out_cf_info->threshX_high = p_in_cf_info->threshX_high;
	}

	if (p_in_cf_info->threshX_low <= 63)
	{
		p_out_cf_info->threshX_low = p_in_cf_info->threshX_low;
	}

	if ((p_in_cf_info->bitmask & SON_CDMA_FREQ_OFFSET_FREQ_PRESENT) &&
			((p_in_cf_info->offset_freq >= -15) && (p_in_cf_info->offset_freq <= 15)))
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_FREQ_OFFSET_FREQ_PRESENT;
		p_out_cf_info->offset_freq = p_in_cf_info->offset_freq;
	}

	if ((p_in_cf_info->bitmask & SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT) &&
			(p_in_cf_info->srch_wnd_size <= 15))
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT;
		p_out_cf_info->srch_wnd_size = p_in_cf_info->srch_wnd_size;
	}

	if ((p_in_cf_info->bitmask & SON_CDMA_FREQ_CELL_RESELECTION_PRIORITY_PRESENT) &&
			(p_in_cf_info->cell_reselection_priority <= 7))
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_RESELECTION_PRIORITY_PRESENT;
		p_out_cf_info->cell_reselection_priority = p_in_cf_info->cell_reselection_priority;
	}

	SON_UT_TRACE_EXIT();
} /* anr_populate_meas_config_req_cdma_freq_info */

/*****************************************************************************
 * Function Name  : anr_populate_meas_config_req_geran_freq_info
 * Inputs         : p_in_cf_info  - Pointer to the GERAN neighbor carrier freq 
 *                  info received from OAM
 * Outputs        : p_out_cf_info - Pointer to the GERAN neighbor carrier freq 
 *                  info  to be sent to RRM
 * Returns        : son_void_t    - None
 * Description    : This function populates the GERAN nbr carrier frq info to 
 *                  be send to RRM from the info received in msg from OAM
 ****************************************************************************/
static son_void_t
anr_populate_meas_config_req_geran_freq_info
(
 const son_geran_carrier_freq_info_t *p_in_cf_info,
 rrm_son_geran_carrier_freq_info_t   *p_out_cf_info
 )
{
	SON_UT_TRACE_ENTER();

	if (p_in_cf_info == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to GERAN carrier frequency info is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMCPY(&p_out_cf_info->geran_car_freqs, &p_in_cf_info->geran_car_freqs,
			sizeof(rrm_son_geran_carrier_freqs_t));
	p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT;

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT)
	{
		p_out_cf_info->cell_reselection_priority = p_in_cf_info->cell_reselection_priority;
		p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT;
	}

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_NCC_PERMITTED_PRESENT)
	{
		p_out_cf_info->ncc_permitted    = p_in_cf_info->ncc_permitted;
		p_out_cf_info->bitmask |=  RRM_SON_GERAN_CFI_NCC_PERMITTED_PRESENT;
	}

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT)
	{
		p_out_cf_info->q_rx_lev_min     = p_in_cf_info->q_rx_lev_min;
		p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT;
	}

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT)
	{
		p_out_cf_info->p_max_geran = p_in_cf_info->p_max_geran;
		p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT;
	}

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_THRESH_HIGH_PRESENT)
	{
		p_out_cf_info->threshX_high  = p_in_cf_info->threshX_high;
		p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_THRESHX_HIGH_PRESENT;
	}

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_THRESH_LOW_PRESENT)
	{
		p_out_cf_info->threshX_low  = p_in_cf_info->threshX_low;
		p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_THRESHX_LOW_PRESENT;
	}

	if (p_in_cf_info->bitmask & SON_GERAN_CFI_OFFSET_FREQ_PRESENT)
	{
		p_out_cf_info->offset_frequency = p_in_cf_info->offset_frequency;
		p_out_cf_info->bitmask |= RRM_SON_GERAN_CFI_OFFSET_FREQ_PRESENT;
	}

	SON_UT_TRACE_EXIT();
} /* anr_populate_meas_config_req_geran_freq_info */

/*****************************************************************************
 * Function Name  : populate_cdma_hrpd_info
 * Inputs         : p_in_cf_info  - Pointer to the CDMA neighbor cells 
 *                  HRPD info
 * Output         : p_out_cf_info - Pointer to the CDMA neighbor cells
 *                  HRPD info to be sent to RRM
 * Returns        : son_void_t    - None
 * Description    : This function is used to populate the CDMA neighbor cells 
 *                  hrpd information which is used by RRM to get the relavent
 *                  UE mesurement reports 
 ****************************************************************************/
static son_void_t
populate_cdma_hrpd_info
(
 const son_cdma_hrpd_info_t *p_in_cf_info,
 rrm_son_cdma_hrpd_info_t   *p_out_cf_info
 )
{
	SON_UT_TRACE_ENTER();
	son_u8  idx = 0;

	if (p_in_cf_info == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to CDMA2000 HRPD info is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	if (p_in_cf_info->bitmask & SON_CDMA_HRPD_REG_PARAMS_PRESENT)
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_HRPD_REG_PARAMS_PRESENT;

		/*Populating son_pre_reg_hrpd_info_t structure
		 * Coverity Fix 30300 */
		p_out_cf_info->hrpd_reg_params.pre_reg_allowed =
			((p_in_cf_info->hrpd_reg_params.pre_reg_allowed == SON_TRUE) ?
			 RRM_TRUE : RRM_FALSE);

		if (p_in_cf_info->hrpd_reg_params.bitmask & SON_HRPD_PRE_REG_ZONE_ID_PRESENT)
		{
			p_out_cf_info->hrpd_reg_params.bitmask |= RRM_SON_HRPD_PRE_REG_ZONE_ID_PRESENT;
			p_out_cf_info->hrpd_reg_params.pre_reg_zone_id = p_in_cf_info->hrpd_reg_params.pre_reg_zone_id;
		}

		if (p_in_cf_info->hrpd_reg_params.bitmask & SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT)
		{
			p_out_cf_info->hrpd_reg_params.bitmask |= RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT;

			p_out_cf_info->hrpd_reg_params.scndry_pre_reg_zone_id_list_size
				= p_in_cf_info->hrpd_reg_params.scndry_pre_reg_zone_id_list_size;

			for (idx = 0;
					idx < p_in_cf_info->hrpd_reg_params.scndry_pre_reg_zone_id_list_size;
					idx++)
			{
				p_out_cf_info->hrpd_reg_params.scndry_pre_reg_zone_id_list[idx] =
					p_in_cf_info->hrpd_reg_params.scndry_pre_reg_zone_id_list[idx];
			}
		}
	}

	if ((p_in_cf_info->bitmask & SON_CDMA_HRPD_RESELECTION_PRESENT) &&
			(p_in_cf_info->t_reselection <= 7))
	{
		p_out_cf_info->t_reselection = p_in_cf_info->t_reselection;
	}

	if (p_in_cf_info->bitmask & SON_CDMA_HRPD_RESELECTION_SF_PRESENT)
	{
		/* Bug Fix: 791 */
		p_out_cf_info->bitmask |= RRM_SON_CDMA_HRPD_RESELECTION_SF_PRESENT;
		SON_MEMCPY(&p_out_cf_info->t_reselection_sf,
				&p_in_cf_info->t_reselection_sf,
				sizeof(rrm_son_speed_state_scale_factors_t));
	}

	SON_UT_TRACE_EXIT();
} /* populate_cdma_hrpd_info */

/*****************************************************************************
 * Function Name  : populate_cdma_xrtt_info
 * Inputs         : p_in_cf_info  - Pointer to the CDMA neighbor cells 
 *                  XRTT info received from OAM
 * Output         : p_out_cf_info - Pointer to the CDMA neighbor cells
 *                  XRTT info to be sent to RRM
 * Returns        : son_void_t    - None
 * Description    : This function is used to populate the CDMA neighbor cells 
 *                  xrtt information which is used by RRM to get the relavent
 *                  UE mesurement reports
 ****************************************************************************/
static son_void_t
populate_cdma_xrtt_info
(
 const son_cdma_1xrtt_info_t *p_in_cf_info,
 rrm_son_cdma_1xrtt_info_t   *p_out_cf_info
 )
{
	SON_UT_TRACE_ENTER();    

	if (p_in_cf_info == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to CDMA2000 1XRTT info is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	if (SON_CDMA_1XRTT_REG_PARAMS_PRESENT & p_in_cf_info->bitmask)
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_1XRTT_REG_PARAMS_PRESENT;

		SON_MEMCPY(&p_out_cf_info->xrtt_reg_params,
				&p_in_cf_info->xrtt_reg_params,
				sizeof(rrm_son_cdma_csfb_reg_param_1XRTT_t));
	}

	if (SON_CDMA_1XRTT_LONG_CODE_PRESENT & p_in_cf_info->bitmask)
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_1XRTT_LONG_CODE_PRESENT; 

		SON_MEMCPY(&p_out_cf_info->cdma_1xrtt_long_code,
				&p_in_cf_info->cdma_1xrtt_long_code,
				RRM_SON_MAX_LONG_CODE_1XRTT_SIZE);
	}

	if (p_in_cf_info->t_reselection <= 7)
	{
		p_out_cf_info->t_reselection = p_in_cf_info->t_reselection;
	}

	if (p_in_cf_info->bitmask & SON_CDMA_1XRTT_RESELECTION_SF_PRESENT)
	{
		p_out_cf_info->bitmask |= RRM_SON_CDMA_1XRTT_RESELECTION_SF_PRESENT;

		SON_MEMCPY(&p_out_cf_info->t_reselection_sf,
				&p_in_cf_info->t_reselection_sf,
				sizeof(rrm_son_speed_state_scale_factors_t));
	}

	SON_UT_TRACE_EXIT();
} /* populate_cdma_xrtt_info */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_cdma_meas_config_list
 * Inputs         : p_in_config_list  - Pointer to the CDMA neighbor cells 
 *                  freq info list received from OAM
 * Output         : p_out_config_list - Pointer to the CDMA neighbor cells
 *                  freq info list to be sent to RRM
 * Returns        : son_void_t        - None
 * Description    : This function is used to populate the CDMA neighbor cells 
 *                  freq information list in the measurement config request message
 *                  which is used by RRM to get the relavent UE mesurement 
 *                  reports
 ****************************************************************************/
static son_void_t
anr_populate_rrm_cdma_meas_config_list
(
 const son_meas_config_cdma_t *p_in_config_list,
 rrm_son_meas_config_cdma_t *p_out_config_list
 )
{
	SON_UT_TRACE_ENTER();
	son_u8 idx = 0;

	if (p_in_config_list == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to measurement configuration info for CDMA2000 is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	/*Checking the list size for avoiding the klocwork's "buffer overflow" warning */
	if (p_in_config_list->cdma_freq_info_list_size
			<= SON_MAX_CONFIG_OBJECT)
	{
		p_out_config_list->cdma_freq_info_list_size
			= p_in_config_list->cdma_freq_info_list_size;

		for (idx = 0; idx < p_in_config_list->cdma_freq_info_list_size; idx++)
		{
			anr_populate_meas_config_req_cdma_freq_info(
					&p_in_config_list->cdma_freq_info_list[idx],
					&p_out_config_list->cdma_freq_info_list[idx]);
		}           
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Failed to "
				"populate cdma carrier freq info in Meas config "
				"request to be sent to RRM due to received list size "
				" > Max CDMA list size %u", SON_MAX_CONFIG_OBJECT);
	}

	SON_UT_TRACE_EXIT();     
} /* anr_populate_rrm_cdma_meas_config_list */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_geran_meas_config_list
 * Inputs         : p_in_config_list  - Pointer to the GERAN neighbor cells 
 *                  freq info list received from OAM
 * Output         : p_out_config_list - Pointer to the GERAN neighbor cells
 *                  freq info list to be sent to RR
 * Returns        : son_void_t        - None
 * Description    : This function is used to populate the GERAN neighbor cells 
 *                  freq information list in the measurement config request message
 *                  which is used by RRM to get the relavent UE mesurement 
 *                  reports
 ****************************************************************************/
static son_void_t
anr_populate_rrm_geran_meas_config_list
(
 const son_meas_config_geran_t *p_in_config_list,
 rrm_son_meas_config_geran_t *p_out_config_list
 )
{
	SON_UT_TRACE_ENTER();
	son_u8 idx = 0;

	if (p_in_config_list == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to measurement configuration info for GERAN is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	/*Checking the list size for avoiding the klocwork's "buffer overflow" warning */
	if (p_in_config_list->geran_freq_info_list_size
			<= SON_MAX_CONFIG_OBJECT)
	{
		p_out_config_list->geran_freq_info_list_size
			= p_in_config_list->geran_freq_info_list_size;

		for (idx = 0; idx < p_in_config_list->geran_freq_info_list_size; idx++)
		{
			anr_populate_meas_config_req_geran_freq_info(
					&p_in_config_list->geran_freq_info_list[idx],
					&p_out_config_list->geran_freq_info_list[idx]);
		}           
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Failed to "
				"populate geran carrier freq info in Meas config "
				"request to be sent to RRM due to received list size "
				" > Max GERAN list size %u", SON_MAX_CONFIG_OBJECT);
	}


	SON_UT_TRACE_EXIT();     
} /* anr_populate_rrm_geran_meas_config_list */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_utran_meas_config_list
 * Inputs         : p_in_config_list   - Pointer to the UTRAN neighbor cells 
 *                  info list received from OAM
 * Output         : p_out_config_list  - Pointer to the UTRAN neighbor cells 
 *                  info list to be sent to RRM
 * Returns        : son_void_t         - None
 * Description    : This function is used to populate the UTRAN neighbor cells 
 *                  information list in the measurement config request message
 *                  which is used by RRM to get the relavent UE mesurement 
 *                  reports
 ****************************************************************************/
static son_void_t
anr_populate_rrm_utran_meas_config_list
(
 const son_meas_config_utran_t *p_in_config_list,
 rrm_son_meas_config_utran_t *p_out_config_list
 )
{
    SON_UT_TRACE_ENTER();
    son_u8 idx = 0;

    if (p_in_config_list == SON_PNULL)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                " Pointer to measurement configuration info for UTRAN is NULL"
               );
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Check the list size against maximum value to avoid Buffer overflow */

    /*SPR-14071 - Start*/
    if ((SON_NULL != p_in_config_list->utran_fdd_list_size) &&
            (SON_MAX_CONFIG_OBJECT >= p_in_config_list->utran_fdd_list_size))
        /*SPR-14071 - End*/    
    {
        p_out_config_list->utran_fdd_list_size = 
            p_in_config_list->utran_fdd_list_size;
        for (idx = 0; idx < p_in_config_list->utran_fdd_list_size; idx++)
        {
            p_out_config_list->utran_fdd_list[idx].uarfcn = 
                p_in_config_list->utran_fdd_list[idx].uarfcn;

            if (SON_MC_UTRAN_FDD_OFFSET_FREQ_PRESENT &
                    p_in_config_list->utran_fdd_list[idx].bitmask) 
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_OFFSET_FREQUENCY_PRESENT; 
                p_out_config_list->utran_fdd_list[idx].offset_frequency = 
                    p_in_config_list->utran_fdd_list[idx].offset_frequency;
            }

            if (p_in_config_list->utran_fdd_list[idx].bitmask &
                    SON_MC_UTRAN_FDD_CELL_RESELECTION_PRIORITY_PRESENT)
            {
                p_out_config_list->utran_fdd_list[idx].
                    cell_reselection_priority = p_in_config_list->
                    utran_fdd_list[idx].cell_reselection_priority;
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_CELL_RESELECTION_PRIORITY_PRESENT;
            }

            if (SON_MC_UTRAN_FDD_THRESH_HIGH_PRESENT &
                    p_in_config_list->utran_fdd_list[idx].bitmask)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_THRESHOLD_HIGH_PRESENT;
                p_out_config_list->utran_fdd_list[idx].threshX_high = 
                    p_in_config_list->utran_fdd_list[idx].threshX_high;
            }

            if (SON_MC_UTRAN_FDD_THRESH_LOW_PRESENT &
                    p_in_config_list->utran_fdd_list[idx].bitmask)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_THRESHOLD_LOW_PRESENT; 
                p_out_config_list->utran_fdd_list[idx].threshX_low = 
                    p_in_config_list->utran_fdd_list[idx].threshX_low;
            }

            if (SON_MC_UTRAN_FDD_Q_RX_LEV_MIN_PRESENT &
                    p_in_config_list->utran_fdd_list[idx].bitmask)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_Q_RX_LEV_MIN_PRESENT;
                p_out_config_list->utran_fdd_list[idx].q_rx_lev_min = 
                    p_in_config_list->utran_fdd_list[idx].q_rx_lev_min;
            }

            if (SON_MC_UTRAN_FDD_P_MAX_PRESENT &
                    p_in_config_list->utran_fdd_list[idx].bitmask)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_P_MAX_PRESENT;
                p_out_config_list->utran_fdd_list[idx].p_max = 
                    p_in_config_list->utran_fdd_list[idx].p_max;
            }

            if (SON_MC_UTRAN_FDD_Q_QUAL_MIN_PRESENT &
                    p_in_config_list->utran_fdd_list[idx].bitmask)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_Q_QUAL_MIN_PRESENT;
                p_out_config_list->utran_fdd_list[idx].q_qual_min = 
                    p_in_config_list->utran_fdd_list[idx].q_qual_min;
            }

            if (p_in_config_list->utran_fdd_list[idx].bitmask &
                    SON_MC_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT)
            {
                p_out_config_list->utran_fdd_list[idx].
                    csg_allowed_reporting_cell_list_size = 
                    p_in_config_list->utran_fdd_list[idx].
                    num_csg_allowed_reporting_cell;
                SON_MEMCPY(p_out_config_list->utran_fdd_list[idx].
                        csg_allowed_reporting_cell_list,
                        p_in_config_list->utran_fdd_list[idx].
                        csg_allowed_reporting_cell_list,
                        sizeof(p_out_config_list->utran_fdd_list[idx].
                            csg_allowed_reporting_cell_list));
                p_out_config_list->utran_fdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT;
            }

            if (p_in_config_list->utran_fdd_list[idx].bitmask & SON_MC_UTRAN_THRESHX_HIGH_Q_R9)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= RRM_SON_UTRAN_THRESHX_HIGH_Q_R9;
                p_out_config_list->utran_fdd_list[idx].threshx_highq_r9 =
                    p_in_config_list->utran_fdd_list[idx].threshx_highq_r9;
            }

            if (p_in_config_list->utran_fdd_list[idx].bitmask & SON_MC_UTRAN_THRESHX_LOW_Q_R9)
            {
                p_out_config_list->utran_fdd_list[idx].bitmask |= RRM_SON_UTRAN_THRESHX_LOW_Q_R9;
                p_out_config_list->utran_fdd_list[idx].threshx_lowq_r9 =
                    p_in_config_list->utran_fdd_list[idx].threshx_lowq_r9;
            }
        }
    }
    /*SPR 17379 Fix Start*/
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,
                "utran_fdd_list_size received from OAM is either zero "
                "or greater than SON_MAX_CONFIG_OBJECT ");
    }
    /*SPR 17379 Fix End*/

    /*SPR 17379 Fix Start - else If changed to If*/
    /*SPR-14071 - Start*/
    if ((SON_NULL != p_in_config_list->utran_tdd_list_size) &&
            (SON_MAX_CONFIG_OBJECT >= p_in_config_list->utran_tdd_list_size))
        /*SPR-14071 - End*/    
        /*SPR 17379 Fix End*/
    {
        p_out_config_list->utran_tdd_list_size = 
            p_in_config_list->utran_tdd_list_size;

        for (idx = 0; idx < p_in_config_list->utran_tdd_list_size; idx++)
        {
            p_out_config_list->utran_tdd_list[idx].uarfcn = 
                p_in_config_list->utran_tdd_list[idx].uarfcn;

            if (SON_MC_UTRAN_TDD_OFFSET_FREQ_PRESENT &
                    p_in_config_list->utran_tdd_list[idx].bitmask) 
            {
                p_out_config_list->utran_tdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_TDD_OFFSET_FREQUENCY_PRESENT;

                p_out_config_list->utran_tdd_list[idx].offset_frequency = 
                    p_in_config_list->utran_tdd_list[idx].offset_frequency;
            }

            if (p_in_config_list->utran_tdd_list[idx].bitmask &
                    SON_MC_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT)
            {
                p_out_config_list->utran_tdd_list[idx].
                    cell_reselection_priority = p_in_config_list->
                    utran_tdd_list[idx].cell_reselection_priority;

                p_out_config_list->utran_tdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT;
            }

            if (SON_MC_UTRAN_TDD_THRESH_HIGH_PRESENT &
                    p_in_config_list->utran_tdd_list[idx].bitmask)
            {
                p_out_config_list->utran_tdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_TDD_THRESHOLD_HIGH_PRESENT;

                p_out_config_list->utran_tdd_list[idx].threshX_high = 
                    p_in_config_list->utran_tdd_list[idx].threshX_high;
            }

            if (SON_MC_UTRAN_TDD_THRESH_LOW_PRESENT &
                    p_in_config_list->utran_tdd_list[idx].bitmask)
            {
                p_out_config_list->utran_tdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_TDD_THRESHOLD_LOW_PRESENT;

                p_out_config_list->utran_tdd_list[idx].threshX_low = 
                    p_in_config_list->utran_tdd_list[idx].threshX_low;
            }

            if (SON_MC_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT &
                    p_in_config_list->utran_tdd_list[idx].bitmask)
            {
                p_out_config_list->utran_tdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT;

                p_out_config_list->utran_tdd_list[idx].q_rx_lev_min = 
                    p_in_config_list->utran_tdd_list[idx].q_rx_lev_min;
            }

            if (SON_MC_UTRAN_TDD_P_MAX_PRESENT &
                    p_in_config_list->utran_tdd_list[idx].bitmask)
            {
                p_out_config_list->utran_tdd_list[idx].bitmask |= 
                    RRM_SON_UTRAN_TDD_P_MAX_PRESENT;

                p_out_config_list->utran_tdd_list[idx].p_max = 
                    p_in_config_list->utran_tdd_list[idx].p_max;
            }
        }
    }
    /*SPR 17379 Fix Start*/
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,
                "utran_tdd_list_size received from OAM is either zero "
                "or greater than SON_MAX_CONFIG_OBJECT ");
    }
    /*SPR 17379 Fix End*/

    SON_UT_TRACE_EXIT();
} /* anr_populate_rrm_utran_meas_config_list */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_eutran_meas_config_list
 * Inputs         : p_in_config_list    - Pointer to the EUTRAN neighbor cells 
 *                                        info list received from OAM
 *                  list_size           - Size of EUTRAN neighbor cells info 
 *                                        list received from OAM
 *                  p_cell_ctxt         - Pointer to ANR cell context
 * Outputs        : p_out_config_list   - Pointer to the EUTRAN neighbor cells 
 *                                        info list to be sent to RRM
 * Returns        : son_void_t          - None
 * Description    : This function is used to populate the EUTRAN neighbor cells 
 *                  information list in the measurement config request message 
 *                  which is used by RRM to get the relavent UE mesurement 
 *                  reports
 ****************************************************************************/
static son_void_t
anr_populate_rrm_eutran_meas_config_list
(
 const son_meas_config_eutran_t      *p_in_config_list,
 const son_u8                        list_size,
 const anr_cell_context_t            *p_cell_ctxt,
 rrm_son_meas_config_eutran_t        *p_out_config_list
 )
{
	son_u8 index = 0;


	SON_UT_TRACE_ENTER();

	if (p_in_config_list == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to measurement configuration info for EUTRAN or Cell Context is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	for (; index < list_size; index++)
	{
		p_out_config_list[index].earfcn = p_in_config_list[index].earfcn;

		if (SON_MC_EUTRAN_ALLOW_MEAS_BW_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_MEAS_BANDWIDTH_PRESENT;
			/* Coverity Fix: 19151 */
			p_out_config_list[index].meas_bandwidth = 
				anr_map_son_meas_bandwidth(
						__func__,
						p_in_config_list[index].meas_bandwidth_for_earfcn
						);
		}

		if (SON_MC_EUTRAN_PRESENCE_ANTENNA_PORT_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_PRESENCE_ANTENNAE_PORT_PRESENT;
			p_out_config_list[index].presence_antenna_port = 
				p_in_config_list[index].presence_antenna_port;
		}

		if (SON_MC_EUTRAN_NBR_CELL_CONFIG_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_NEIGHBOR_CELL_CONFIG_PRESENT;
			p_out_config_list[index].neighbor_cell_config = 
				p_in_config_list[index].neighbor_cell_config;
		}

		if (SON_MC_EUTRAN_OFFSET_FREQ_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_OFFSET_FREQUENCY_PRESENT;
			/* Coverity Fix: 19148 */
			p_out_config_list[index].offset_frequency = 
				anr_map_son_offset_frequency(
						__func__,
						p_in_config_list[index].offset_frequency
						);
		}

		if (SON_MC_EUTRAN_Q_RX_LEV_MIN_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_Q_RX_LEV_MIN_PRESENT;
			p_out_config_list[index].q_rx_lev_min = 
				p_in_config_list[index].q_rx_lev_min;
		}

		if (SON_MC_EUTRAN_P_MAX_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |= RRM_SON_EUTRAN_P_MAX_PRESENT;
			p_out_config_list[index].p_max = p_in_config_list[index].p_max;
		}

		if (SON_MC_EUTRAN_RESELECTION_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_RESELECTION_PRESENT;
			p_out_config_list[index].t_reselection = 
				p_in_config_list[index].t_reselection;
		}

		if (SON_MC_EUTRAN_RESELECTION_SF_PRESENT &  p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_RESELECTION_SF_PRESENT;
			SON_MEMCPY(&p_out_config_list[index].t_reselection_sf,
					&p_in_config_list[index].t_reselection_sf, 
					sizeof(rrm_son_speed_state_scale_factors_t));
		}

		if (SON_MC_EUTRAN_CELL_THRESH_HIGH_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_THRESHOLD_HIGH_PRESENT;
			p_out_config_list[index].threshX_high = 
				p_in_config_list[index].threshX_high;
		}

		if (SON_MC_EUTRAN_CELL_THRESH_LOW_PRESENT & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_THRESHOLD_LOW_PRESENT;
			p_out_config_list[index].threshX_low =
				p_in_config_list[index].threshX_low;
		}

		if (SON_MC_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT &
				p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |=
				RRM_SON_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT;
			p_out_config_list[index].cell_reselection_priority = 
				p_in_config_list[index].cell_reselection_priority;
		}

		if (SON_MC_EUTRAN_Q_QUAL_MIN_R9 & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |= RRM_SON_EUTRAN_Q_QUAL_MIN_R9;
			p_out_config_list[index].q_qualmin_r9 = p_in_config_list[index].q_qualmin_r9;
		}

		if (SON_MC_EUTRAN_THRESHX_HIGH_Q_R9 & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |= RRM_SON_EUTRAN_THRESHX_HIGH_Q_R9;
			p_out_config_list[index].threshx_highq_r9 = p_in_config_list[index].threshx_highq_r9;
		}

		if (SON_MC_EUTRAN_THRESHX_LOW_Q_R9 & p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |= RRM_SON_EUTRAN_THRESHX_LOW_Q_R9;
			p_out_config_list[index].threshx_lowq_r9 = p_in_config_list[index].threshx_lowq_r9;
		}

		if (SON_MC_EUTRAN_CM_OFFSET_FREQ_PRESENT &  p_in_config_list[index].bitmask)
		{
			p_out_config_list[index].bitmask |= RRM_SON_EUTRAN_CM_OFFSET_FREQUENCY_PRESENT;
			p_out_config_list[index].cm_offset_frequency =
				(rrm_q_offsetrange_et)p_in_config_list[index].cm_offset_frequency;
		}
	}

	SON_UT_TRACE_EXIT();
} /* anr_populate_rrm_eutran_meas_config_list */

/*****************************************************************************
 * Function Name  : anr_fsm_meas_config_req
 * Inputs         : p_cspl_hdr     - Pointer to CSPl header
 *                  p_cell_ctxt    - Pointer to ANR cell context
 *                  evnt_id        - Unique Identifier of the message
 *                  p_msg          - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MEAS_CONFIG_REQ. It is invoked when
 *                  the anr state of a cell is ANR_CELL_STATE_ENABLED. This 
 *                  function is used to parse the meas config req received from 
 *                  OAM and populate the meas config req for RRM which it uses
 *                  to get the UE mesurement reports
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_meas_config_req
(
 STACKAPIHDR         *p_cspl_hdr,
 anr_cell_context_t  *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8              *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	son_anr_meas_config_req_t *p_req = (son_anr_meas_config_req_t *)p_msg;
	son_rrm_meas_config_req_t *p_rrm_req = SON_PNULL;
	/*SPR-10230-Start*/
	son_u8      index = 0;
	son_u16     temp_open_pci_end = 0;
	son_u16     temp_closed_pci_end = 0;
	son_u16     temp_hybrid_pci_end = 0;
	/*SPR-10230-End*/

	if (p_msg == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
    /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to measurement configuration request"
                "or Pointer to Cell Context is NULL evnt_id %u p_cspl_hdr %p"
				,evnt_id,p_cspl_hdr);
    /*SPR 17777 +-*/ 

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	/*SPR-10230-Start*/

	if(p_req->config_req.eutran_config_list_size)
	{
		for (; index < p_req->config_req.eutran_config_list_size; index++)
		{
			temp_open_pci_end = 0;
			temp_closed_pci_end = 0;
			temp_hybrid_pci_end = 0;

			/*SPR-10426-Start*/
			if(p_req->config_req.eutran_config_list[index].bitmask & SON_MC_INTER_FREQ_OPEN_PCI_RANGE_PRESENT)
			{
				/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
				temp_open_pci_end = son_determine_pci_range_end_value(p_req->config_req.eutran_config_list[index].inter_freq_open_pci_range.pci_range);
				if((temp_open_pci_end == 0)||
						((p_req->config_req.eutran_config_list[index].inter_freq_open_pci_range.pci_start + temp_open_pci_end -1) > 503))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"Invalid pci range received. open_pci_start[%d], open_pci_end[%d]",
                            p_req->config_req.eutran_config_list[index].inter_freq_open_pci_range.pci_start,
                            temp_open_pci_end);

					SON_UT_TRACE_EXIT();
					return RELEASE_SON_BUFFER;
				}
			}

			if(p_req->config_req.eutran_config_list[index].bitmask & SON_MC_INTER_FREQ_CLOSED_PCI_RANGE_PRESENT)
			{
				/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
				temp_closed_pci_end = son_determine_pci_range_end_value(p_req->config_req.eutran_config_list[index].inter_freq_closed_pci_range.pci_range);
				if((temp_closed_pci_end == 0)||
						((p_req->config_req.eutran_config_list[index].inter_freq_closed_pci_range.pci_start + temp_closed_pci_end -1) > 503))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"Invalid pci range received. closed_pci_start[%d], closed_pci_end[%d]",
                            p_req->config_req.eutran_config_list[index].inter_freq_closed_pci_range.pci_start,
                            temp_closed_pci_end);

					SON_UT_TRACE_EXIT();
					return RELEASE_SON_BUFFER;
				}
			}

			if(p_req->config_req.eutran_config_list[index].bitmask & SON_MC_INTER_FREQ_HYBRID_PCI_RANGE_PRESENT)
			{
				/*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
				temp_hybrid_pci_end = son_determine_pci_range_end_value(p_req->config_req.eutran_config_list[index].inter_freq_hybrid_pci_range.pci_range);
				if((temp_hybrid_pci_end == 0)||
						((p_req->config_req.eutran_config_list[index].inter_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1) > 503))
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"Invalid pci range received. hybrid_pci_start[%d], hybrid_pci_end[%d]",
                            p_req->config_req.eutran_config_list[index].inter_freq_hybrid_pci_range.pci_start,
                            temp_hybrid_pci_end);

					SON_UT_TRACE_EXIT();
					return RELEASE_SON_BUFFER;
				}
			}
			/*SPR-10426-End*/
		}
	}

	/*SPR-10230-End*/

	if (!p_req->config_req.eutran_config_list_size &&
			!p_req->config_req.other_rat_config_list.utran_config_list.utran_fdd_list_size &&
			!p_req->config_req.other_rat_config_list.utran_config_list.utran_tdd_list_size &&
			!p_req->config_req.other_rat_config_list.geran_config_list.geran_freq_info_list_size &&
			!p_req->config_req.other_rat_config_list.cdma_config_list.cdma_freq_info_list_size &&
			!(p_req->config_req.bitmask & SON_ANR_MEAS_GAP_CONFIG_PRESENT) &&
			!(p_req->config_req.other_rat_config_list.utran_config_list.bitmask) &&
			!(p_req->config_req.other_rat_config_list.geran_config_list.bitmask) &&
			!(p_req->config_req.other_rat_config_list.cdma_config_list.bitmask))
	{
		son_anr_meas_config_res_t oam_res = {0};
		oam_res.transaction_id = p_req->transaction_id;
		oam_res.config_res.result = SON_FAILURE;
		oam_res.config_res.error_code = SON_ERR_INVALID_PARAMS;

		SON_MEMCPY(&oam_res.config_res.src_cgi,
				&p_req->config_req.src_cgi,
				sizeof(son_rrm_intra_rat_global_cell_id_t));

		/* Coverity Fix: 19134, 19153 */
		son_create_send_buffer((son_u8 *)&oam_res, SON_ANR_MODULE_ID,
				SON_MIF_MODULE_ID, SONANR_MEAS_CONFIG_RES,
				sizeof(son_anr_meas_config_res_t));

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Empty Measurement Configuration "
				"Request received from MIF for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_req->config_req.src_cgi.cell_identity));
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_rrm_req = (son_rrm_meas_config_req_t *)son_mem_get(sizeof(son_rrm_meas_config_req_t));
	if (SON_PNULL == p_rrm_req)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_rrm_req, 0, sizeof(son_rrm_meas_config_req_t));

	p_rrm_req->transaction_id = p_req->transaction_id;

	SON_MEMCPY(&p_rrm_req->config_req.cell_id, &p_req->config_req.src_cgi, 
			sizeof(son_intra_rat_global_cell_id_t)); 

	if(p_req->config_req.bitmask & SON_ANR_MEAS_GAP_CONFIG_PRESENT)
	{
		p_rrm_req->config_req.bitmask |= RRM_SON_MEAS_GAP_CONFIG_PRESENT;
		if(p_req->config_req.meas_gap_config.bitmask & SON_ANR_EUTRAN_GAP_CONFIG_PRESENT)
		{
			p_rrm_req->config_req.meas_gap_config.bitmask |= RRM_SON_EUTRAN_GAP_CONFIG_PRESENT;

			if(p_req->config_req.meas_gap_config.eutran_gap_config.bitmask & SON_ANR_SETUP_GAP_CONFIG_PRESENT)
			{
				p_rrm_req->config_req.meas_gap_config.eutran_gap_config.bitmask |= RRM_SON_SETUP_GAP_CONFIG_PRESENT;
				if(p_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP0_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP0_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.gap_offset_gp0 =
						p_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.gap_offset_gp0; 
				}
				if(p_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP1_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP1_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.gap_offset_gp1 =
						p_req->config_req.meas_gap_config.eutran_gap_config.setup_gap_config.gap_offset_gp1; 
				}
			}
		}

		if(p_req->config_req.meas_gap_config.bitmask & SON_ANR_UTRAN_GAP_CONFIG_PRESENT)
		{
			p_rrm_req->config_req.meas_gap_config.bitmask |= RRM_SON_UTRAN_GAP_CONFIG_PRESENT;
			if(p_req->config_req.meas_gap_config.utran_gap_config.bitmask & SON_ANR_SETUP_GAP_CONFIG_PRESENT)
			{
				p_rrm_req->config_req.meas_gap_config.utran_gap_config.bitmask |= RRM_SON_SETUP_GAP_CONFIG_PRESENT;
				if(p_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP0_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP0_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.gap_offset_gp0 =
						p_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.gap_offset_gp0; 
				}
				if(p_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP1_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP1_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.gap_offset_gp1 =
						p_req->config_req.meas_gap_config.utran_gap_config.setup_gap_config.gap_offset_gp1; 
				}
			}
		}
		if(p_req->config_req.meas_gap_config.bitmask & SON_ANR_GERAN_GAP_CONFIG_PRESENT)
		{
			p_rrm_req->config_req.meas_gap_config.bitmask |= RRM_SON_GERAN_GAP_CONFIG_PRESENT;
			if(p_req->config_req.meas_gap_config.geran_gap_config.bitmask & SON_ANR_SETUP_GAP_CONFIG_PRESENT)
			{
				p_rrm_req->config_req.meas_gap_config.geran_gap_config.bitmask |= RRM_SON_SETUP_GAP_CONFIG_PRESENT;
				if(p_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP0_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP0_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.gap_offset_gp0 =
						p_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.gap_offset_gp0; 
				}
				if(p_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP1_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP1_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.gap_offset_gp1 =
						p_req->config_req.meas_gap_config.geran_gap_config.setup_gap_config.gap_offset_gp1; 
				}
			}
		}
		if(p_req->config_req.meas_gap_config.bitmask & SON_ANR_CDMA2000_GAP_CONFIG_PRESENT)
		{
			p_rrm_req->config_req.meas_gap_config.bitmask |= RRM_SON_CDMA2000_GAP_CONFIG_PRESENT;
			if(p_req->config_req.meas_gap_config.cdma2000_gap_config.bitmask & SON_ANR_SETUP_GAP_CONFIG_PRESENT)
			{
				p_rrm_req->config_req.meas_gap_config.cdma2000_gap_config.bitmask |= RRM_SON_SETUP_GAP_CONFIG_PRESENT;
				if(p_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP0_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP0_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.gap_offset_gp0 =
						p_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.gap_offset_gp0; 
				}
				if(p_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.bitmask & SON_ANR_GAP_OFFSET_GP1_PRESENT)
				{
					p_rrm_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.bitmask |= RRM_SON_GAP_OFFSET_GP1_PRESENT; 
					p_rrm_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.gap_offset_gp1 =
						p_req->config_req.meas_gap_config.cdma2000_gap_config.setup_gap_config.gap_offset_gp1; 
				}
			}
		}
	}
	if (p_req->config_req.eutran_config_list_size)
	{
		p_rrm_req->config_req.eutran_config_list_size = 
			p_req->config_req.eutran_config_list_size;
		anr_populate_rrm_eutran_meas_config_list(
				p_req->config_req.eutran_config_list, 
				p_req->config_req.eutran_config_list_size,
				p_cell_ctxt,
				p_rrm_req->config_req.eutran_config_list);
	}

	if (p_req->config_req.other_rat_config_list.utran_config_list.utran_fdd_list_size ||
			p_req->config_req.other_rat_config_list.utran_config_list.utran_tdd_list_size)
	{
		anr_populate_rrm_utran_meas_config_list(
				&p_req->config_req.other_rat_config_list.utran_config_list,
				&p_rrm_req->config_req.inter_rat_meas_config.utran_config_list);
	}

	if (SON_UTRAN_RESELECTION_PRESENT & 
			p_req->config_req.other_rat_config_list.utran_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.bitmask 
			|= RRM_SON_UTRAN_RESELECTION_PRESENT;
		p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.t_reselection = 
			p_req->config_req.other_rat_config_list.utran_config_list.t_reselection;
	}

	if (SON_UTRAN_RESELECTION_SF_PRESENT & 
			p_req->config_req.other_rat_config_list.utran_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.bitmask 
			|= RRM_SON_UTRAN_RESELECTION_SF_PRESENT;
		SON_MEMCPY(&p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.t_reselection_sf,
				&p_req->config_req.other_rat_config_list.utran_config_list.t_reselection_sf,
				sizeof(rrm_son_speed_state_scale_factors_t));
	}

	if (p_req->config_req.other_rat_config_list.geran_config_list.geran_freq_info_list_size)
	{
		anr_populate_rrm_geran_meas_config_list(
				&p_req->config_req.other_rat_config_list.geran_config_list,
				&p_rrm_req->config_req.inter_rat_meas_config.geran_config_list );         
	}

	if (SON_MC_GERAN_RESELECTION_PRESENT & 
			p_req->config_req.other_rat_config_list.geran_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.geran_config_list.bitmask 
			|= RRM_SON_MC_GERAN_T_RESELECTION_PRESENT;
		p_rrm_req->config_req.inter_rat_meas_config.geran_config_list.t_reselection 
			= p_req->config_req.other_rat_config_list.geran_config_list.t_reselection;
	}

	if (SON_MC_GERAN_RESELECTION_SF_PRESENT &
			p_req->config_req.other_rat_config_list.geran_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.geran_config_list.bitmask
			|= RRM_SON_MC_GERAN_T_RESELECTION_SF_PRESENT;
		SON_MEMCPY(&p_rrm_req->config_req.inter_rat_meas_config.geran_config_list.t_reselection_sf,
				&p_req->config_req.other_rat_config_list.geran_config_list.t_reselection_sf, 
				sizeof(rrm_son_speed_state_scale_factors_t));
	}

	if (p_req->config_req.other_rat_config_list.cdma_config_list.cdma_freq_info_list_size)
	{
		anr_populate_rrm_cdma_meas_config_list(
				&p_req->config_req.other_rat_config_list.cdma_config_list,
				&p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list);
	}

	if (SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |= 
			RRM_SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT;

		SON_MEMCPY(&p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.sys_time_info,
				&p_req->config_req.other_rat_config_list.cdma_config_list.sys_time_info,
				sizeof(son_cdma_sys_time_info_t));
	}

	if (SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |= 
			RRM_SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT;

		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.srch_wnd_size =  
			p_req->config_req.other_rat_config_list.cdma_config_list.srch_wnd_size;
	}

	if (SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |= 
			RRM_SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT;

		populate_cdma_hrpd_info(&p_req->config_req.other_rat_config_list.cdma_config_list.hrpd_info,
				&p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.hrpd_info);
	}

	if (SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |= 
			RRM_SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT;

		populate_cdma_xrtt_info(&p_req->config_req.other_rat_config_list.cdma_config_list.cdma_1xrtt_info,
				&p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.cdma_1xrtt_info);
	}

	if (SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |=
			RRM_SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT;

		/* Coverity Fix: 30301 */
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.csfb_support_for_dual_Rx_Ues_r9 = 
			((p_req->config_req.other_rat_config_list.cdma_config_list.csfb_support_for_dual_Rx_Ues_r9 == SON_TRUE) ?
			 RRM_TRUE : RRM_FALSE);
	}

	if (SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |=
			RRM_SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT;

		SON_MEMCPY(&p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.csfb_registration_param_1xrtt_r9,
				&p_req->config_req.other_rat_config_list.cdma_config_list.csfb_registration_param_1xrtt_r9,
				sizeof(son_cdma_csfb_reg_param_1xrtt_r9_t));
	}

	if (SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT & 
			p_req->config_req.other_rat_config_list.cdma_config_list.bitmask)
	{
		p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.bitmask |=
			RRM_SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT;

		SON_MEMCPY(&p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.ac_barring_config_1xrtt_r9,
				&p_req->config_req.other_rat_config_list.cdma_config_list.ac_barring_config_1xrtt_r9,
				sizeof(rrm_son_ac_barring_config_1xrtt_t));
	}

	/*Invoke PUP tool call to send message to RRM*/
	if (RRM_FAILURE == rrm_son_send_rrm_son_meas_config_req(
				&p_rrm_req->config_req,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				p_req->transaction_id,
				SON_NULL))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Meas config request could not"
				" be sent to RRM for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id((p_req->config_req.src_cgi.cell_identity)));
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"Message %u send from %u to"
				" %u for cell [Cell Id: 0x%x]",
				RRM_SON_MEAS_CONFIG_REQ,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id((p_req->config_req.src_cgi.cell_identity)));

		/*SPR-9574-Start*/
		anr_insert_in_inter_freq_list(
				p_req->config_req.eutran_config_list,
				p_req->config_req.eutran_config_list_size);
		/*SPR-9574-End*/
	}

	if (SON_PNULL !=  p_rrm_req)
	{
		son_mem_free(p_rrm_req);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_meas_config_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_meas_config_req
 * Inputs         : p_cspl_hdr     - Pointer to CSPl header
 *                  p_cell_ctxt    - Pointer to ANR cell context
 *                  evnt_id        - Unique Identifier of the message
 *                  p_msg          - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MEAS_CONFIG_REQ message received 
 *                  in invalid state (cell is in state other than 
 *                  ANR_CELL_STATE_ENABLED). This function sends a negative 
 *                  response to OAM with error cause as SON_ERR_UNEXPECTED_MSG.
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_meas_config_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_meas_config_req_t *p_req = (son_anr_meas_config_req_t *)p_msg;
	son_anr_meas_config_res_t oam_res = {0};


	SON_UT_TRACE_ENTER();

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to CSPL Header or Pointer to Cell Context is NULL"
				);

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	oam_res.transaction_id = p_req->transaction_id;
	oam_res.config_res.result = SON_FAILURE;
	oam_res.config_res.error_code = SON_ERR_UNEXPECTED_MSG;
	SON_MEMCPY(&oam_res.config_res.src_cgi,
			&p_req->config_req.src_cgi,
			sizeof(son_rrm_intra_rat_global_cell_id_t));

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	/* Coverity Fix: 19156,19140 */
	son_create_send_buffer((son_u8 *)&oam_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_MEAS_CONFIG_RES,
			sizeof(son_anr_meas_config_res_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_meas_config_req */

/*****************************************************************************
 * Function Name  : anr_fsm_meas_config_res
 * Inputs         : p_cspl_hdr    - Pointer to CSPl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Unique Identifier of the message
 *                  p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for RRM_MEAS_CONFIG_RES. It is invoked when
 *                  the anr state of a cell is ANR_CELL_STATE_ENABLED. This 
 *                  function is used to parse the meas config res received from
 *                  RRM and populate the meas config res for OAM
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_meas_config_res
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_rrm_meas_config_res_t *p_res  =
		(son_rrm_meas_config_res_t *)p_msg;
	son_buf_retain_status_et        ret_val = RELEASE_SON_BUFFER;
	/* Coverity Fix: 17261,26820,26822,26827 */
	son_return_et result = SON_SUCCESS;
	son_anr_meas_config_res_t oam_res = {0};


	SON_UT_TRACE_ENTER();

	/* Search the received transaction Id in the cell context's transaction
	 * Id list. If found (the config responses is received
	 * for ANR generated meas config request) then don't send any message to
	 * OAM, delete the transaction Id from the list and return */

    /*SPR 17777 +-*/ 
	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_BRIEF,
			"Meas config response received for"
			" Cell [Cell Id: 0x%x] ANR generated meas config request "
			"message with Trans "
			"Id %u evnt_id %u",
            son_convert_char_cell_id_to_int_cell_id(p_res->config_res.meas_config_res.cell_id.cell_identity),
			p_res->transaction_id,evnt_id);
    /*SPR 17777 +-*/ 

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to CSPL Header or Pointer to Cell Context is NULL"
				);
		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	if (anr_find_txn_id_list(&p_cell_ctxt->txn_id_list, p_res->transaction_id))
	{
		anr_delete_from_txn_id_list(&p_cell_ctxt->txn_id_list, p_res->transaction_id);
		SON_UT_TRACE_EXIT();
		return ret_val;
	}

	/* Coverity Fix: 17261,17262,26824 */ 
	result = anr_map_rrm_return_code(
			__func__,
			p_res->config_res.meas_config_res.result
			);

	if (SON_SUCCESS == result)
	{
		SON_MEMCPY(&oam_res, p_res, sizeof(son_anr_meas_config_res_t));
	}
	else
	{
		oam_res.transaction_id = p_res->transaction_id;
		SON_MEMCPY(&oam_res.config_res.src_cgi, 
				&p_res->config_res.meas_config_res.cell_id,
				sizeof(son_intra_rat_global_cell_id_t));

		/* Coverity Fix: 19135,19152,26823 */ 
		oam_res.config_res.result = result;
		oam_res.config_res.error_code = anr_map_rrm_error_codes(
				__func__,
				result,
				p_res->config_res.meas_config_res.error_code,
				RRM_SON_MEAS_CONFIG_RESP);

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Error: %d: Meas configuration"
				" request with RRM failed for cell [Cell Id: 0x%x].",
				p_res->config_res.meas_config_res.error_code,
				son_convert_char_cell_id_to_int_cell_id(p_res->config_res.meas_config_res.cell_id.cell_identity));
	}

	son_create_send_buffer((son_u8 *)&oam_res,
			SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONANR_MEAS_CONFIG_RES,
			sizeof(oam_res));

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_fsm_meas_config_res */

/*****************************************************************************
 * Function Name  : anr_construct_set_attribute_req
 * Inputs         : transaction_id  - Transaction Id
 *                  p_cell_id       - Pointer to Cell_Id
 *                : attr_id         - Attribute ID which needs to be modified
 *                : attr_val        - New Value for Attribute
 * Outputs        : None
 * Returns        : son_void_t      - None
 * Description    : This function constructs RRM_SET_ATTRIBUTE_REQ request
 ****************************************************************************/
 /* SPR 20653 Fix Start */
static son_void_t
anr_construct_set_attribute_req
(
 son_u16                         transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_modify_attribute_t          *mod_attr
 )
{
	son_rrm_set_attribute_value_req_t set_att_req = {0};


	SON_UT_TRACE_ENTER();

	if (p_cell_id == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to Cell Id is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	set_att_req.transaction_id = transaction_id;
	set_att_req.req.object.type = RRM_OBJ_CELL;
	SON_MEMCPY(&(set_att_req.req.object.id.object_id.cell_id), p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	set_att_req.req.object.attr_list_size = SON_NULL;
    set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list_size = SON_NULL;
	if (SON_MODIFY_UE_COUNT_PRESENT & mod_attr->bitmask)
	{
        set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list_size++;
        set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].value = mod_attr->ue_cnt;
	    set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].id = RRM_ATTR_MEAS_UE_COUNT;
        set_att_req.req.object.attr_list_size++;
	}
	if (SON_MODIFY_REPORT_INTERVAL_PRESENT & mod_attr->bitmask)
	{
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list_size++;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].id = RRM_TRGR_PERIODIC;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].bitmask |= RRM_TRIGGER_TYPE_PERIODIC;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].periodic.interval = (mod_attr->reporting_interval) * 60;
	    set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].id = RRM_ATTR_MEAS_REPORTING_INTERVAL;
        set_att_req.req.object.attr_list_size++;
	}
	if (SON_MODIFY_DL_THRESHOLD_PRESENT & mod_attr->bitmask)
	{
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list_size++;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].value = mod_attr->ue_throughput_dl_anr_threshold;
	    set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].id = RRM_ATTR_UE_THROUGHPUT_DL_ANR_THRESHOLD;
        set_att_req.req.object.attr_list_size++;
	}
	if (SON_MODIFY_UL_THRESHOLD_PRESENT & mod_attr->bitmask)
	{
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list_size++;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].value = mod_attr->ue_throughput_ul_anr_threshold;
	    set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].id = RRM_ATTR_UE_THROUGHPUT_UL_ANR_THRESHOLD;
        set_att_req.req.object.attr_list_size++;
	}

	/*PUP Tool call to create and send SET ATTRIBUTE REQ Message to RRM */
	if (RRM_FAILURE == rrm_son_send_rrm_son_set_attribute_value_req(
				&set_att_req.req,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				transaction_id,
				SON_NULL))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Set Attribute could not"
				" be sent to RRM for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id((p_cell_id->cell_identity)));
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"Message %u send from %u to"
				" %u for cell [Cell Id: 0x%x] with New value = %d",
				RRM_SON_SET_ATTRIBUTE_REQ,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity),
				set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].value);
	} 

	SON_UT_TRACE_EXIT();
} /* anr_construct_set_attribute_req */
 /* SPR 20653 Fix End */

/*****************************************************************************
 * Function Name  : anr_fsm_set_attribute_value_res_handler
 * Inputs         : p_cspl_hdr        - Pointer to CSPL header
 *                  p_cell_ctxt       - Pointer to ANR cell context
 *                  evnt_id           - API Id
 *                  p_msg             - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is event Handler for SON_RRM_SET_ATTRIBUTE_RES.
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_set_attribute_value_res_handler
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_rrm_set_attribute_res_t *p_res  = (son_rrm_set_attribute_res_t *)p_msg;
	son_generic_response_with_cell_id_t modify_res = {0};


	SON_UT_TRACE_ENTER();

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				" Pointer to Cell context or CSPL header is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}
    /* SPR 20653 Fix Start */
    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
            "Attribute id %d received in set attribute response "
            "from: %d for API: %s for cell [Cell Id: 0x%x]", p_res->res.id, p_cspl_hdr->from,
            SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
            son_convert_char_cell_id_to_int_cell_id(p_res->res.set_attr_resp.id.object_id.cell_id.cell_identity));
    /* SPR 20653 Fix End */

	son_procedure_code_et api  = SON_PROCEDURE_UNDEFINED;

	if (RRM_ATTR_MEAS_UE_COUNT == p_res->res.id)
	{
		api = SONANR_MODIFY_UE_COUNT_RES;
	}
	else if (RRM_ATTR_MEAS_REPORTING_INTERVAL == p_res->res.id)
	{
		api = SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES;
	}
    /* SPR 20653 Fix Start */
	else
	{
		api = SONANR_MODIFY_ATTRIBUTE_RES;
	}
    /* SPR 20653 Fix End */

	modify_res.transaction_id = p_res->transaction_id;
	SON_MEMCPY(&modify_res.generic_res.cell_id,
			&p_res->res.set_attr_resp.id.object_id.cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	if (RRM_FAILURE == p_res->res.set_attr_resp.result)
	{
		modify_res.generic_res.result = SON_FAILURE;

		/* Coverity Fix: 17266,19136,19133 */
		modify_res.generic_res.error_code = anr_map_rrm_error_codes(
				__func__,
				modify_res.generic_res.result,
				p_res->res.set_attr_resp.error_code,
				(rrm_son_message_resp_et)p_cspl_hdr->api
				);

		if (RRM_ERR_OBJECT_NON_EXISTENT == p_res->res.set_attr_resp.error_code
				|| RRM_ERR_CELL_UNCONFIGURED == 
				p_res->res.set_attr_resp.error_code)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"Modifying Attribute failed as Cell not configured, cell "
					"[Cell Id: 0x%x] deleted from ANR Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_res->res.set_attr_resp.id.object_id.cell_id.
						cell_identity));

			anr_delete_cell_from_context(((son_intra_rat_global_cell_id_t *)&
						p_res->res.set_attr_resp.id.object_id.cell_id));
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_DETAILED,
				"Message %d received from RRM with result: %d and error code: "
				"%d ", p_cspl_hdr->api,
				p_res->res.set_attr_resp.result,
				p_res->res.set_attr_resp.error_code);
		modify_res.generic_res.error_code =  SON_NO_ERROR;
		modify_res.generic_res.result = SON_SUCCESS;
	}

	son_create_send_buffer((son_u8 *)&modify_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, api,
			sizeof(modify_res));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_set_attribute_value_res_handler */

/******************************************************************************
 * Function Name  : anr_find_pci_in_inter_rat_nrt
 * Inputs         : p_hash          - Pointer to the inter RAT NRT
 *                  data            - UARFCN of the cell in case of UTRAN cell 
 *                  p_detected_cell - Structure storing information of the 
 *                                   neighbor cell found
 *                  p_oam_update_notification_for_update_op - Structure storing
 *                                   updates done in NRT to be sent to OAM
 * Output         : None
 * Returns        : son_error_et    - SON_NO_ERROR If PCI of neighbor cell found
 *                                    in NRT, Error Code Otherwise
 * Description    : Function to find the cell with given PCI and earfcn in the 
 *                  inter RAT NRT 
 *****************************************************************************/
static son_error_et
anr_find_pci_in_inter_rat_nrt
(
 anr_cell_context_t *p_cell_ctxt,
 son_u32                         data,
 rrm_son_utran_neighbor_cell_t   *p_detected_cell,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op
 )
{
	anr_inter_rat_hash_nr_tuple_t   *p_inter_tpl    = SON_PNULL;
	son_error_et                    error_code      = SON_ERR_ANR_NR_NOT_FOUND;
	son_u16                         pci             = 0;
	son_bool_et                     is_rscp_present = SON_FALSE;
	son_bool_et                     is_ecno_present = SON_FALSE;
	son_bool_et                     is_rscp_ecno_updated = SON_FALSE;
	son_inter_rat_neighbor_info_t   inter_rat_nr    = {0};
	SON_HASH *p_hash = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt 
			|| SON_PNULL == p_detected_cell
			|| SON_PNULL == p_oam_update_notification_for_update_op)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"No Inter-RAT neighbor with "
				"matching PCI could be found, p_cell_ctxt or p_detected_cell"
				" or p_oam_update_notification_for_update_op is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	p_hash = &p_cell_ctxt->inter_rat_neighbor_table;

	p_inter_tpl  = anr_get_first_nr_from_inter_rat_hash_table(p_hash);

	while (SON_PNULL != p_inter_tpl)
	{
		if (ANR_UTRAN_NEIGHBOR == p_inter_tpl->data.t)
		{
			/* Check for uarfcn and PCI */
			if ((SON_UTRAN_UARFCN_PRESENT & 
						p_inter_tpl->data.u.utran_neighbor_info.bitmask) &&
					(son_u16)data == p_inter_tpl->data.u.utran_neighbor_info.
					uarfcn &&
					(SON_UTRAN_PCI_PRESENT & 
					 p_inter_tpl->data.u.utran_neighbor_info.bitmask) &&
					!SON_MEMCMP(&p_inter_tpl->data.u.utran_neighbor_info.pci,
						&p_detected_cell->pci,
						sizeof(p_inter_tpl->data.u.utran_neighbor_info.pci)))
			{
				if (SON_PCI_FDD_PRESENT & 
						p_inter_tpl->data.u.utran_neighbor_info.pci.bitmask)
				{   
					pci = p_inter_tpl->data.u.utran_neighbor_info.pci.pci_fdd;
				}
				else if (SON_PCI_TDD_PRESENT &
						p_inter_tpl->data.u.utran_neighbor_info.pci.
						bitmask)
				{
					pci = p_inter_tpl->data.u.utran_neighbor_info.pci.pci_tdd;
				}

				/* Coverity Fix: 19132 */
				if (SON_UTRAN_RSCP_PRESENT & p_inter_tpl->data.u.utran_neighbor_info.bitmask)
				{
					is_rscp_present = SON_TRUE;
				}

				/* Coverity Fix: 19131 */
				if (SON_UTRAN_ECNO_PRESENT & p_inter_tpl->data.u.utran_neighbor_info.bitmask)
				{                    
					is_ecno_present = SON_TRUE;
				}

				error_code =  SON_NO_ERROR;

				/* Check if RSCP or ECNO is present in the tupple and if 
				 * present compare their values to find if the received
				 * neighbor is same as already stored neighbor */
				if (!is_rscp_present || p_detected_cell->rscp >
						p_inter_tpl->data.u.utran_neighbor_info.rscp)
				{
					/* Update is required in NRT */
					p_inter_tpl->data.u.utran_neighbor_info.rscp = 
						p_detected_cell->rscp;
					is_rscp_ecno_updated = SON_TRUE;
					p_inter_tpl->data.u.utran_neighbor_info.bitmask |= SON_UTRAN_RSCP_PRESENT;
				}

				if (!is_ecno_present || p_detected_cell->ecno >
						p_inter_tpl->data.u.utran_neighbor_info.ecno)
				{
					/* Update is required in NRT */
					p_inter_tpl->data.u.utran_neighbor_info.ecno = 
						p_detected_cell->ecno;
					is_rscp_ecno_updated = SON_TRUE;
					p_inter_tpl->data.u.utran_neighbor_info.bitmask |= SON_UTRAN_ECNO_PRESENT;
				}

				if (SON_TRUE == is_rscp_ecno_updated)
				{
					p_oam_update_notification_for_update_op->nrt_op = SON_NRT_OP_UPDATE;
					/* Update notification strucutre towards OAM */
					anr_convert_inter_rat_hash_rec_to_inter_rat_nr(
							&p_inter_tpl->data,
							&inter_rat_nr);

					anr_construct_oam_updated_ncl(
							&p_oam_update_notification_for_update_op->
							nr_list,
							&inter_rat_nr,
							SON_TRUE);
				}

				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_BRIEF,
						"UTRAN Neighbor Cell [Cell Id: 0x%x] has a matching PCI "
						"[%d]",
						son_convert_char_cell_id_to_int_cell_id(p_inter_tpl->data.u.utran_neighbor_info.
							nbr_cgi.cell_identity),
						pci);

				son_time(&p_inter_tpl->data.last_activity_ts);

				SON_UT_TRACE_EXIT();
				return error_code;
			}
		}
		else
		{
			/* Add code for other RATs in future releases */
		}

		p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
				p_hash, &p_inter_tpl->anchor);

        /* Coverity 80370 Fix Start */
        if (SON_PNULL != p_inter_tpl)
        {
            /* Coverity fix 88476 */
            /* Coverity fix 88476 */
        }
        /* Coverity 80370 Fix Start */
	}

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_BRIEF,
			"No Inter-RAT neighbor with "
			"matching PCI could be found");

	SON_UT_TRACE_EXIT();
	return error_code;
} /* anr_find_pci_in_inter_rat_nrt */

/******************************************************************************
 * Function Name  : anr_find_pci_in_intra_rat_nrt
 * Inputs         : p_cell_ctxt     - Pointer to cell context
 *                  earfcn          - EARFCN of the cell   
 *                  p_detected_cell - Structure storing information of the 
 *                                    neighbor cell found
 * Outputs        : p_oam_update_notification_for_update_op - Structure storing 
 *                                   updates done in NRT to be sent to OAM
 * Returns        : son_error_et    - SON_NO_ERROR If PCI of neighbor cell 
 *                                    found in NRT, Error Code Otherwise
 * Description    : Function to find the cell with given PCI and earfcn in the 
 *                  intra RAT NRT 
 *****************************************************************************/
static son_error_et
anr_find_pci_in_intra_rat_nrt
(
 anr_cell_context_t              *p_cell_ctxt,
 son_u32                         earfcn,
 rrm_son_eutran_neighbor_cell_t  *p_detected_cell,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op
 )
{
	/* Coverity CID-55065 Fix Starts */
	anr_intra_rat_hash_nr_tuple_t     *p_intra_tpl            = SON_PNULL;
	son_error_et    error_code = SON_ERR_ANR_NR_NOT_FOUND;
	son_bool_et is_rsrp_rsrq_updated = SON_FALSE;
	SON_HASH                        *p_hash = SON_PNULL;
	anr_pci_sus_conf_node_t         *p_pci_sus_conf_node      = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt
			|| SON_PNULL == p_detected_cell
			|| SON_PNULL == p_oam_update_notification_for_update_op)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"No Intra-RAT neighbor with "
				" matching PCI could be found, p_cell_ctxt or p_detected_cell"
				" or p_update_notification is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	p_hash = &p_cell_ctxt->intra_rat_neighbor_table;

	p_pci_sus_conf_node = anr_find_in_sus_pci_ctxt_list(p_cell_ctxt,
			p_detected_cell->pci,
			earfcn);
	if (SON_PNULL != p_pci_sus_conf_node)
	{
		if (0 ==  p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_thx_val)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"Serving cell"
					"[Cell Id:0x%x] has the PCI[%d] and EARFCN [%d] marked as"
					"[state:%d] but is having the ask_cgi_thx_val set to zero so CGI can not"
					"be asked", son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
					p_detected_cell->pci, earfcn, p_pci_sus_conf_node->pci_sus_conf_data.state);
			error_code = SON_NO_ERROR;
		}
		else
		{
			error_code = SON_ERR_ANR_NR_NOT_FOUND;
			if (0 !=  p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_counter)
			{
				p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_counter--;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
						"For serinvg cell [cell id:0x%x] "
						"The Ask CGI counter for PCI [%d] for EARFCN [%d] ,is already zero"
						"hence Ask cgi counter can not be decremented",
						son_convert_char_cell_id_to_int_cell_id(
							p_cell_ctxt->cell_id.cell_identity), p_detected_cell->pci, earfcn);
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"For the serving cell "
					"[Cell Id: 0x%x] has PCI [%d] for EARFCN[%d], marked as "
					"PCI Confusion -suspected,ECGI can be requested again",
					son_convert_char_cell_id_to_int_cell_id(
						p_cell_ctxt->cell_id.cell_identity), p_detected_cell->pci, earfcn);
		}

		return error_code;
	}

	p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);
	while (SON_PNULL != p_intra_tpl)
	{
		if ((p_intra_tpl->data.neighbor_info.bitmask & SON_EARFCN_PRESENT) &&
				earfcn == p_intra_tpl->data.neighbor_info.earfcn &&
				(p_intra_tpl->data.neighbor_info.bitmask & SON_PCI_PRESENT) &&
				p_detected_cell->pci == p_intra_tpl->data.neighbor_info.pci)
		{
			son_bool_et is_rsrp_present = SON_FALSE;
			son_bool_et is_rsrq_present = SON_FALSE;
			son_time(&p_intra_tpl->data.last_activity_ts);

			/* Matching PCI Found
			 * Coverity Fix: 24311 */
			if (SON_RSRP_PRESENT & p_intra_tpl->data.neighbor_info.bitmask)
			{
				is_rsrp_present = SON_TRUE;
			}

			/* Coverity Fix: 24312 */
			if (SON_RSRQ_PRESENT & p_intra_tpl->data.neighbor_info.bitmask)
			{
				is_rsrq_present = SON_TRUE;
			}

			error_code =  SON_NO_ERROR;

			/* Check if RSRP or RSRQ is present in the tupple and if present
			 * compare their values to find if the received neighbor is same
			 * as already stored neighbor */
			if (!is_rsrp_present || p_detected_cell->rsrp >
					p_intra_tpl->data.neighbor_info.rsrp)
			{
				/* Update is required in NRT */
				p_intra_tpl->data.neighbor_info.rsrp = p_detected_cell->rsrp;
				is_rsrp_rsrq_updated = SON_TRUE;
				p_intra_tpl->data.neighbor_info.bitmask |= SON_RSRP_PRESENT;
			}

			if (!is_rsrq_present || p_detected_cell->rsrq >
					p_intra_tpl->data.neighbor_info.rsrq)
			{
				/* Update is required in NRT */
				p_intra_tpl->data.neighbor_info.rsrq = p_detected_cell->rsrq;
				is_rsrp_rsrq_updated = SON_TRUE;
				p_intra_tpl->data.neighbor_info.bitmask |= SON_RSRQ_PRESENT;
			}

			if (SON_TRUE == is_rsrp_rsrq_updated)
			{
				p_oam_update_notification_for_update_op->nrt_op = SON_NRT_OP_UPDATE;
				/* Update notification structure towards OAM */
				anr_construct_oam_updated_ncl(
						&p_oam_update_notification_for_update_op->nr_list,
						&p_intra_tpl->data.neighbor_info,
						SON_FALSE);
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF,
					"EUTRAN neighbor cell "
					"[Cell Id: 0x%x] has matching PCI [%d]",
					son_convert_char_cell_id_to_int_cell_id(p_intra_tpl->data.neighbor_info.nbr_cgi.
						cell_identity),
					p_detected_cell->pci);

			SON_UT_TRACE_EXIT();
			return error_code;
		}

		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				p_hash, &p_intra_tpl->anchor);
            /* Coverity 79120 Fix Start */
        if (SON_PNULL != p_intra_tpl)
        {
            /* Coverity fix 88475 */
            /* Coverity fix 88475 */
        }
        /* Coverity 79120 Fix End */
	}

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_BRIEF,
			"No Intra-RAT neighbor with "
			" matching PCI could be found");

	SON_UT_TRACE_EXIT();
	return error_code;
	/* Coverity CID-55065 Fix Ends */
} /* anr_find_pci_in_intra_rat_nrt */

/******************************************************************************
 * Function Name  : anr_add_utran_cell_to_inter_rat_nrt
 * Inputs         : p_cell_context      - Pointer to ANR cell context
 *                  p_detected_cell     - Structure storing information of the
 *                                        neighbor cell to be added in NRT
 *                  p_rec               - Pointer to intra rat hash record
 *                  p_oam_notif         - Pointer to the notification structure 
 *                                        to be sent to OAM if neighbor is added
 *                                        in NRT
 *                  p_oam_notif_update  - Pointer to the notification structure 
 *                                        to be sent to OAM if neighbor is updated
 *                                        in NRT.
 *                  p_rrm_notif         - Pointer to message for rrm notification
 *                  uarfcn              - UARFCN of the cell
 * Outputs        : p_rec               - Pointer to the inter RAT NRT record 
 *                                        inserted
 * Returns        : son_error_et        - Error Code if error occurs, otherwise 
 *                                        SON_NO_ERROR
 * Description    : Function to add a UTRAN cell to inter RAT NRT.
 *****************************************************************************/
static son_error_et
anr_add_utran_cell_to_inter_rat_nrt
(
 anr_cell_context_t                  *p_cell_ctxt,
 const rrm_son_utran_neighbor_cell_t *p_detected_cell,
 const son_u16                       uarfcn,
 anr_inter_rat_hash_rec_t            *p_rec,
 son_anr_updated_nrt_info_t			*p_oam_notif,
 son_anr_updated_nrt_info_t			*p_oam_notif_update,
 son_anr_rrm_updated_nrt_info_t		*p_rrm_notif
 )
{
	son_error_et error_code = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_detected_cell ||
			SON_PNULL == p_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"UTRAN neighbor cell "
				"could not be added to NRT, p_hash or p_detected_cell or p_rec"
				" is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	SON_MEMSET(p_rec, 0, sizeof(anr_inter_rat_hash_rec_t));
	p_rec->t = ANR_UTRAN_NEIGHBOR;

	SON_MEMCPY(&p_rec->u.utran_neighbor_info.nbr_cgi,
			&p_detected_cell->cgi,
			sizeof(son_utran_geran_global_cell_id_t));

	p_rec->u.utran_neighbor_info.bitmask |= 
		SON_UTRAN_UARFCN_PRESENT;
	p_rec->u.utran_neighbor_info.uarfcn = uarfcn;

	if (RRM_SON_UTRAN_NC_PLMN_PRESENT &
			p_detected_cell->bitmask)
	{
		if (MAX_PLMN_LIST >= p_detected_cell->num_plmn_id)
		{
			p_rec->u.utran_neighbor_info.bitmask |= 
				SON_UTRAN_PLMN_ID_PRESENT;
			p_rec->u.utran_neighbor_info.num_plmn_id = 
				p_detected_cell->num_plmn_id;
			SON_MEMCPY(p_rec->u.utran_neighbor_info.plmn_id,
					p_detected_cell->plmn_id,
					sizeof(p_rec->u.utran_neighbor_info.plmn_id));
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"UTRAN neighbor "
					"cell plmn Id could not be added to NRT. Number of "
					"PLMN Id %u > Maximum number of PLMN Ids (6)", 
					(son_u16)(p_detected_cell->num_plmn_id)); 
		}
	}

	if (RRM_SON_UTRAN_NC_LAC_PRESENT &
			p_detected_cell->bitmask)
	{
		p_rec->u.utran_neighbor_info.bitmask |= 
			SON_UTRAN_LAC_PRESENT;
		SON_MEMCPY(p_rec->u.utran_neighbor_info.lac,
				p_detected_cell->lac,
				SON_LAC_OCTET_SIZE);
	}

	if (RRM_SON_UTRAN_NC_RAC_PRESENT &
			p_detected_cell->bitmask)
	{
		p_rec->u.utran_neighbor_info.bitmask |= 
			SON_UTRAN_RAC_PRESENT;
		p_rec->u.utran_neighbor_info.rac = p_detected_cell->rac;
	}

	if (RRM_SON_UTRAN_NC_CSG_PRESENT &
			p_detected_cell->bitmask)
	{
		p_rec->u.utran_neighbor_info.bitmask |= 
			SON_UTRAN_CSG_ID_PRESENT;
		SON_MEMCPY(p_rec->u.utran_neighbor_info.csg_identity,
				p_detected_cell->csg_identity,
				SON_CSG_ID_OCTET_SIZE);
	}
	else
	{
		p_rec->u.utran_neighbor_info.bitmask |=  SON_UTRAN_ACCESS_MODE_PRESENT;
		p_rec->u.utran_neighbor_info.access_mode = SON_OPEN; 
	}

	p_rec->u.utran_neighbor_info.bitmask |=
		SON_UTRAN_PCI_PRESENT;
	if (RRM_PCI_FDD_PRESENT & p_detected_cell->pci.bitmask)
	{
		p_rec->u.utran_neighbor_info.pci.bitmask |= 
			SON_PCI_FDD_PRESENT;
		p_rec->u.utran_neighbor_info.pci.pci_fdd = 
			p_detected_cell->pci.pci_fdd;
	}
	else if (RRM_PCI_TDD_PRESENT &
			p_detected_cell->pci.bitmask)
	{
		p_rec->u.utran_neighbor_info.pci.bitmask |= 
			SON_PCI_TDD_PRESENT;
		p_rec->u.utran_neighbor_info.pci.pci_tdd = 
			p_detected_cell->pci.pci_tdd;
	}

	p_rec->u.utran_neighbor_info.bitmask |= SON_UTRAN_RSCP_PRESENT;
	p_rec->u.utran_neighbor_info.rscp = p_detected_cell->rscp;

	p_rec->u.utran_neighbor_info.bitmask |= SON_UTRAN_ECNO_PRESENT;
	p_rec->u.utran_neighbor_info.ecno = p_detected_cell->ecno;

	p_rec->u.utran_neighbor_info.bitmask |= SON_UTRAN_ENABLE_NR_STATUS_PRESENT;
	p_rec->u.utran_neighbor_info.nr_status = SON_NR_UNLOCKED;

	p_rec->u.utran_neighbor_info.bitmask |= SON_UTRAN_ENABLE_HO_STATUS_PRESENT;
	p_rec->u.utran_neighbor_info.ho_status = SON_HO_ALLOWED;

	anr_inter_rat_insert_nr(
			p_cell_ctxt,
			p_rec,
			SON_TRUE,
			&error_code,
			p_oam_notif,
			p_rrm_notif);

	/*SPR-9759-Start*/
	if(error_code == SON_NO_ERROR)
	{
		/*The utran cell has been successfully added to NRT.If its PCI value is present in utran_pci_requested_cgi_list
		  then remove it from there as its CGI need not to be requested now*/

		anr_delete_pci_from_utran_pci_requested_cgi_list(&p_cell_ctxt->utran_pci_requested_cgi_list,
				p_detected_cell->pci,
                		/*SPR 18360 Fix Start*/
                		uarfcn);
                		/*SPR 18360 Fix Stop*/
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"number of nodes in utran_pci_requested_cgi_list are %d",
				list_count(&p_cell_ctxt->utran_pci_requested_cgi_list));
	}
	/*SPR-9759-End*/

	if (SON_ERR_ANR_DUPLICATE_NR == error_code)
	{
		p_rec->u.utran_neighbor_info.bitmask &=
			~(SON_UTRAN_ENABLE_NR_STATUS_PRESENT | SON_UTRAN_ENABLE_HO_STATUS_PRESENT);
		anr_inter_rat_modify_nr(p_cell_ctxt,
				p_rec,
				SON_TRUE,
				&error_code,
				p_oam_notif_update,
				p_rrm_notif);
	}

	SON_UT_TRACE_EXIT();
	return error_code;
} /* anr_add_utran_cell_to_inter_rat_nrt */

/******************************************************************************
 * Function Name  : anr_find_geran_pci_in_inter_rat
 * Inputs         : p_cell_ctxt     - Pointer to cell context
 *                  p_detected_cell - Structure storing information of the 
 *                                    neighbor cell found
 *                  p_oam_update_notification_for_update_op - Structure storing
 *                                    updates done in NRT to be sent to OAM
 * Output         : None
 * Returns        : son_error_et    - SON_NO_ERROR If PCI of neighbor cell found
 *                                    in NRT, Error Code Otherwise
 * Description    : Function to find the cell with given PCI and earfcn in the 
 *                  inter RAT NRT 
 *****************************************************************************/
static son_error_et
anr_find_geran_pci_in_inter_rat
(
 anr_cell_context_t              *p_cell_ctxt,
 rrm_son_geran_neighbor_cell_t   *p_detected_cell,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op
 )
{
	anr_inter_rat_hash_nr_tuple_t   *p_inter_tpl    = SON_PNULL;
	son_geran_neighbor_info_t       *p_geran_data   = SON_PNULL;
   /*SPR 15393 Fix Start*/
    son_error_et                    error_code      = SON_NO_ERROR;
    son_bool_et       is_any_geran_nbr_cfgrd_on_given_pci  = SON_FALSE;
    anr_inter_rat_hash_rec_t       *p_first_geran_nbr_hash_rec   = SON_PNULL;
   /*SPR 15393 Fix Stop*/
	son_inter_rat_neighbor_info_t   inter_rat_nr    = {0};
	SON_HASH                        *p_hash = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt 
			|| SON_PNULL == p_detected_cell
			|| SON_PNULL == p_oam_update_notification_for_update_op)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"No Inter-RAT neighbor with "
				"matching PCI could be found, p_cell_ctxt or p_detected_cell"
				" or p_oam_update_notification_for_update_op is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	p_hash = &p_cell_ctxt->inter_rat_neighbor_table;
	p_inter_tpl  = anr_get_first_nr_from_inter_rat_hash_table(p_hash);
	while (SON_PNULL != p_inter_tpl)
	{
		if (ANR_GERAN_NEIGHBOR == p_inter_tpl->data.t)
		{
			p_geran_data = &p_inter_tpl->data.u.geran_neighbor_info;
			if (p_detected_cell->arfcn == p_geran_data->arfcn &&
					p_detected_cell->band_ind == (rrm_son_geran_band_indicator_et) p_geran_data->band_ind &&
					!SON_MEMCMP(&p_geran_data->pci, &p_detected_cell->pci,
						sizeof(son_geran_physical_cell_id_t)))
			{
                /*SPR 15393 Fix Start*/
                is_any_geran_nbr_cfgrd_on_given_pci = SON_TRUE;

                /*Get the address of first matching geran neighbor in the table*/
                if(SON_PNULL == p_first_geran_nbr_hash_rec)
                {
                    p_first_geran_nbr_hash_rec = &p_inter_tpl->data;
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_BRIEF, "anr_find_geran_pci_in_inter_rat: "
                            "First found GERAN Nbr Cell is [Cell Id: 0x%x] on  matching"
                            "PCI",                    
                            son_convert_const_char_cell_id_to_int_cell_id(
                            p_first_geran_nbr_hash_rec->u.geran_neighbor_info.nbr_cgi.cell_identity));
                }

                /*Check if the GERAN neighbor is not having RAC,
                  Try for the ask CGI Procedure  */ 
                if(!(p_geran_data->bitmask & SON_GERAN_RAC_PRESENT))
                {

                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_BRIEF, "anr_find_geran_pci_in_inter_rat: "
                            "GERAN Nbr [Cell Id: 0x%x] of matching"
                            " PCI(NCC [%d],BSCC [%d]) is not having "
                            "RAC ",
                            son_convert_const_char_cell_id_to_int_cell_id(p_geran_data->nbr_cgi.cell_identity),
                            p_geran_data->pci.ncc, p_geran_data->pci.bscc);
                    error_code      = SON_ERR_ANR_NR_NOT_FOUND;
                    break;
                } 
                /*SPR 15393 Fix Stop*/
			}
		}

		p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
				p_hash, &p_inter_tpl->anchor);      
	}
    /*SPR 15393 Fix Start*/
    if(SON_TRUE == is_any_geran_nbr_cfgrd_on_given_pci)
    {
        /*If all the GERAN neighbors with the matching PCI
          are having the RAC available,do the updation for the 
          first GERAN neighbor found in the iteration*/
        if((SON_NO_ERROR == error_code) && 
                (SON_PNULL != p_first_geran_nbr_hash_rec))
        {
            son_geran_neighbor_info_t       *p_first_geran_nbr  = SON_PNULL;
            p_first_geran_nbr = &p_first_geran_nbr_hash_rec->u.geran_neighbor_info;

            if (!(p_first_geran_nbr->bitmask & SON_GERAN_RSSI_PRESENT) ||
                    p_detected_cell->rssi > p_first_geran_nbr->rssi)
            {
                /*Update the nbr with incoming rssi value*/
                p_first_geran_nbr->rssi = p_detected_cell->rssi;
                p_first_geran_nbr->bitmask |= SON_GERAN_RSSI_PRESENT;

                anr_convert_inter_rat_hash_rec_to_inter_rat_nr(
                        p_first_geran_nbr_hash_rec, &inter_rat_nr);

                p_oam_update_notification_for_update_op->nrt_op = SON_NRT_OP_UPDATE;

                /*Populate the notification msg to OAM for updation in
                 * GERAN nbr*/
                anr_construct_oam_updated_ncl(
                        &p_oam_update_notification_for_update_op->
                        nr_list, &inter_rat_nr, SON_TRUE);
            }

            son_time(&p_first_geran_nbr_hash_rec->last_activity_ts);
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, "anr_find_geran_pci_in_inter_rat:All GERAN nbrs  "
                    "of matching pci are having RAC. "
                    "GERAN Neighbor Cell [Cell Id: 0x%x] Time stamp Updated ",
                    son_convert_const_char_cell_id_to_int_cell_id(
                        p_first_geran_nbr_hash_rec->u.geran_neighbor_info.nbr_cgi.cell_identity));
        }
    }
    else
    {
        error_code  = SON_ERR_ANR_NR_NOT_FOUND;
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF,
                "anr_find_geran_pci_in_inter_rat_nrt: No GERAN neighbor with "
                "matching PCI could be found");
    }
    /*SPR 15393 Fix Stop*/

    SON_UT_TRACE_EXIT();
	return error_code;
} /* anr_find_geran_pci_in_inter_rat */

/******************************************************************************
 * Function Name  : anr_add_geran_cell_to_inter_rat_nrt
 * Inputs         : p_cell_context     - Pointer to ANR cell context
 *                  p_detected_cell    - Structure storing information of the
 *                                       neighbor cell to be added in NRT
 *                  p_rrm_notif        - Pointer to message for rrm notification
 *                  p_oam_notif        - Pointer to message for oam notification
 *                : band_ind           - Band Indicator value
 * Outputs        : p_rec              - Pointer to the inter RAT NRT record inserted
 * Returns        : son_error_et       - Error Code if error occurs otherwise SON_NO_ERROR
 * Description    : Function to add a GERAN cell to inter RAT NRT.
 *****************************************************************************/
static son_error_et
anr_add_geran_cell_to_inter_rat_nrt
(
 anr_cell_context_t                    *p_cell_ctxt,
 const rrm_son_geran_neighbor_cell_t   *p_detected_cell,
 anr_inter_rat_hash_rec_t              *p_rec,
 son_anr_updated_nrt_info_t			  *p_oam_notif,
 son_anr_updated_nrt_info_t			  *p_oam_notif_update,
 son_anr_rrm_updated_nrt_info_t		  *p_rrm_notif
 )
{
	son_error_et error_code = SON_NO_ERROR;
    /* Spr 18615 Changes Start */
    anr_geran_key_t    key  = {{SON_NULL},SON_NULL};
    /* Spr 18615 Changes End */

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_detected_cell ||
			SON_PNULL == p_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"GERAN neighbor cell "
				"could not be added to NRT, p_hash or p_detected_cell or p_rec"
				" is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	SON_MEMSET(p_rec, 0, sizeof(anr_inter_rat_hash_rec_t));
	p_rec->t = ANR_GERAN_NEIGHBOR;

	SON_MEMCPY(&p_rec->u.geran_neighbor_info.nbr_cgi,
			&p_detected_cell->cgi, sizeof(son_utran_geran_global_cell_id_t));

	p_rec->u.geran_neighbor_info.rssi    = p_detected_cell->rssi;

	p_rec->u.geran_neighbor_info.arfcn    = p_detected_cell->arfcn;

	/* Coverity Fix: 19154 */
	p_rec->u.geran_neighbor_info.band_ind = anr_map_rrm_band_ind(
			__func__,
			p_detected_cell->band_ind
			);

	SON_MEMCPY(&p_rec->u.geran_neighbor_info.pci,
			&p_detected_cell->pci, sizeof(son_geran_physical_cell_id_t));

	if (RRM_SON_GERAN_MR_IND_RAC_PRESENT & p_detected_cell->bitmask)
	{
		p_rec->u.geran_neighbor_info.bitmask |= SON_GERAN_RAC_PRESENT;
		p_rec->u.geran_neighbor_info.rac = p_detected_cell->rac;
	}

	p_rec->u.geran_neighbor_info.nr_status = SON_NR_UNLOCKED;
	p_rec->u.geran_neighbor_info.ho_status = SON_HO_ALLOWED;

	p_rec->u.geran_neighbor_info.bitmask |= SON_GERAN_RSSI_PRESENT | SON_GERAN_PCI_PRESENT |
		SON_GERAN_ARFCN_BAND_IND_PRESENT | SON_GERAN_ENABLE_NR_STATUS_PRESENT |
		SON_GERAN_ENABLE_HO_STATUS_PRESENT;

	anr_inter_rat_insert_nr(p_cell_ctxt,
			p_rec, SON_TRUE, &error_code, p_oam_notif, p_rrm_notif);

	/*SPR-9759-Start*/
	if(error_code == SON_NO_ERROR)
	{
       /* Spr 18615 Changes Start */
       SON_MEMCPY(&key.pci,&p_detected_cell->pci,sizeof(rrm_son_geran_physical_cell_id_t));
       key.arfcn = p_detected_cell->arfcn; 
       /* Spr 18615 Changes End */
		/*The geran cell has been successfully added to NRT.If its PCI value is present in geran_pci_requested_cgi_list
		  then remove it from there as its CGI need not to be requested now*/
		anr_delete_pci_from_geran_pci_requested_cgi_list(&p_cell_ctxt->geran_pci_requested_cgi_list,
       /* Spr 18615 Changes Start */
                                                        &key);
       /* Spr 18615 Changes End */
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"number of nodes in geran_pci_requested_cgi_list are %d",
				list_count(&p_cell_ctxt->geran_pci_requested_cgi_list));
	}
	/*SPR-9759-End*/

	if (SON_ERR_ANR_DUPLICATE_NR == error_code)
	{
		p_rec->u.geran_neighbor_info.bitmask &=
			~(SON_GERAN_ENABLE_NR_STATUS_PRESENT | SON_GERAN_ENABLE_HO_STATUS_PRESENT);

		anr_inter_rat_modify_nr(p_cell_ctxt,
				p_rec,
				SON_TRUE,
				&error_code,
				p_oam_notif_update,
				p_rrm_notif);
	}

	SON_UT_TRACE_EXIT();
	return error_code;
} /* anr_add_geran_cell_to_inter_rat_nrt */

/******************************************************************************
 * Function Name  : anr_add_cdma_cell_to_inter_rat_nrt
 * Inputs         : p_cell_context     - Pointer to ANR cell context
 *                  p_detected_cell    - Structure storing information of the
 *                                       neighbor cell to be added in NRT
 *                  p_rrm_notif        - Pointer to message for rrm notification
 *                  p_oam_notif        - Pointer to message for oam notification
 *                : band_ind           - Band Indicator value
 * Outputs        : p_rec              - Pointer to the inter RAT NRT record inserted
 * Returns        : son_error_et       - Error Code if error occurs otherwise SON_NO_ERROR
 * Description    : Function to add a CDMA cell to inter RAT NRT.
 *****************************************************************************/
static son_error_et
anr_add_cdma_cell_to_inter_rat_nrt
(
 anr_cell_context_t                    *p_cell_ctxt,
 const rrm_son_cdma_neighbor_cell_t   *p_detected_cell,
 anr_inter_rat_hash_rec_t              *p_rec,
 son_anr_updated_nrt_info_t			  *p_oam_notif,
 son_anr_updated_nrt_info_t			  *p_oam_notif_update,
 son_anr_rrm_updated_nrt_info_t		  *p_rrm_notif
 )
{
	son_error_et error_code = SON_NO_ERROR;

     	/*SPR 18360 Fix Start*/
    	anr_eutran_cdma_key_t     key = {0};
    	/*SPR 18360 Fix Stop*/

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt || SON_PNULL == p_detected_cell ||
			SON_PNULL == p_rec)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"CDMA neighbor cell "
				"could not be added to NRT, p_hash or p_detected_cell or p_rec"
				" is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	SON_MEMSET(p_rec, 0, sizeof(anr_inter_rat_hash_rec_t));
	p_rec->t = ANR_CDMA_NEIGHBOR;

	SON_MEMCPY(&p_rec->u.cdma_neighbor_info.nbr_cgi,
			&p_detected_cell->cgi, sizeof(son_cdma_global_cell_id_t));

	p_rec->u.cdma_neighbor_info.arfcn    = p_detected_cell->arfcn;

	/* Coverity Fix: 24316, 30288, 30289, 30383, 30348 */
	p_rec->u.cdma_neighbor_info.band_class = (son_cdma_band_class_et)p_detected_cell->band_class;

	p_rec->u.cdma_neighbor_info.pci = p_detected_cell->pci;
	p_rec->u.cdma_neighbor_info.broadcast_status  = SON_FALSE;
	p_rec->u.cdma_neighbor_info.nr_status = SON_NR_UNLOCKED;
	p_rec->u.cdma_neighbor_info.ho_status = SON_HO_ALLOWED;
	p_rec->u.cdma_neighbor_info.pilot_strength = p_detected_cell->pilot_strength;

	if (RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT & p_detected_cell->bitmask)
	{
		p_rec->u.cdma_neighbor_info.bitmask |= SON_CDMA_PILOT_PN_PHASE_PRESENT;
		p_rec->u.cdma_neighbor_info.pilot_pn_phase = p_detected_cell->pilot_pn_phase;
	}

	p_rec->u.cdma_neighbor_info.bitmask |= SON_CDMA_PCI_PRESENT | SON_CDMA_BAND_CLASS_PRESENT |
		SON_CDMA_BAND_CLASS_ARFCN_PRESENT | SON_CDMA_ENABLE_NR_STATUS_PRESENT |
		SON_CDMA_ENABLE_HO_STATUS_PRESENT | SON_CDMA_BROADCAST_MODE_PRESENT |
		SON_CDMA_PILOT_STRENGTH_PRESENT;

	anr_inter_rat_insert_nr(p_cell_ctxt,
			p_rec, SON_TRUE, &error_code, p_oam_notif, p_rrm_notif);

	/*SPR-9759-Start*/
	if(error_code == SON_NO_ERROR)
	{
		/*The cdma cell has been successfully added to NRT.If its PCI value is present in cdma_pci_requested_cgi_list
		  then remove it from there as its CGI need not to be requested now*/
        	/*SPR 18360 Fix Start*/
        	key.t                       = CDMA_KEY;
        	key.key.cdma_key.pci             = p_detected_cell->pci;
        	key.key.cdma_key.band_class      = (son_cdma_band_class_et)p_detected_cell->band_class;
        	key.key.cdma_key.arfcn           = p_detected_cell->arfcn;
        	/*SPR 18360 Fix Stop*/
		anr_delete_pci_from_eutran_cdma_pci_requested_cgi_list(&p_cell_ctxt->cdma_pci_requested_cgi_list,
                		/*SPR 18360 Fix Start*/
                		&key);
                		/*SPR 18360 Fix Stop*/
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"number of nodes in cdma_pci_requested_cgi_list are %d",
				list_count(&p_cell_ctxt->cdma_pci_requested_cgi_list));
	}
	/*SPR-9759-End*/

	if (SON_ERR_ANR_DUPLICATE_NR == error_code)
	{
		p_rec->u.cdma_neighbor_info.bitmask &= ~(SON_CDMA_ENABLE_NR_STATUS_PRESENT |
				SON_CDMA_ENABLE_HO_STATUS_PRESENT |
				SON_CDMA_BROADCAST_MODE_PRESENT);

		anr_inter_rat_modify_nr(p_cell_ctxt,
				p_rec,
				SON_TRUE,
				&error_code,
				p_oam_notif_update,
				p_rrm_notif);
	}

	SON_UT_TRACE_EXIT();
	return error_code;
} /* anr_add_cdma_cell_to_inter_rat_nrt */

/*SPR-9759-Start*/

/******************************************************************************
 * Function Name  : anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  p_detected_cell        - Detected GERAN cell
 *                : ue_index               - Index of the UE from which meas
 *                                           results have come
 * Outputs        : 
 * Returns        : son_bool_et              - Returns SON_TRUE on success else 
 *                                           SON_FALSE
 * Description    : Function to check whether CGI request has to be sent for a PCI 
 *                  or not
 *****************************************************************************/
/* SPR 15698 Start */
static son_bool_et
anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell
(
 anr_cell_context_t              *p_cell_ctxt,
 rrm_son_geran_neighbor_cell_t    *p_detected_cell,
 son_u16                          ue_index,
 rrmc_meas_report_event_et        meas_report_event
 )
{
    anr_geran_pci_value_node_t        *p_pci_list_node = SON_PNULL;
    ue_index_and_counter_list_node_t  *p_ue_index_counter_list_node = SON_PNULL;
    son_bool_et                        ret_val = SON_FALSE;
    /* Spr 18615 Changes Start */
    anr_geran_key_t                    key = {{SON_NULL},SON_NULL};
    /* Spr 18615 Changes End */

    SON_UT_TRACE_ENTER();

    if (p_cell_ctxt == SON_PNULL || p_detected_cell == SON_PNULL)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell: "
                "either p_cell_ctxt or p_detected_cell received is NULL");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    /* Spr 18615 Changes Start */
    SON_MEMCPY(&key.pci,&p_detected_cell->pci,sizeof(rrm_son_geran_physical_cell_id_t));
    key.arfcn = p_detected_cell->arfcn; 
    /* Spr 18615 Changes End */
    /*Finding the PCI value in GERAN PCI value list*/
    p_pci_list_node = anr_find_record_in_geran_pci_value_list(
            &p_cell_ctxt->geran_pci_requested_cgi_list,
            /* Spr 18615 Changes Start */
            &key);
            /* Spr 18615 Changes End */

    /*If PCI is present*/
    if(p_pci_list_node != SON_PNULL)
    {
        /// Check whether an Empty CGI report is received. Since, we don't need to configure the
        /// same PCI for CGI if strongest cell report is received for the same PCI.
        if(meas_report_event == MEAS_REP_EVENT_FOR_CGI)
        {
            p_ue_index_counter_list_node = anr_find_record_in_ue_index_and_counter_list(
                    &p_pci_list_node->geran_pci_value.ue_index_and_counter_list,
                    ue_index);

            /*If UE Index is present*/
            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                if(p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter
                        < g_config_params.anr_config_params.anr_max_num_cgi_request)
                {
                    /// Refresh Timer Set Absolute time for CGI Gaurd Timer.
                    son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                    /* SPR 15831 Start */
                    p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS/MSECS_IN_SEC);
                    /* SPR 15831 End */

                    p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                    ret_val = SON_TRUE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
                else
                {
                    /*Deleting ue_index and counter value node */
                    list_delete_node(&p_pci_list_node->geran_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                    son_mem_free(p_ue_index_counter_list_node);
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell:"
                            "number of nodes in ue_index_and_counter list are %d",
                            list_count(&p_pci_list_node->geran_pci_value.ue_index_and_counter_list));
                    ret_val = SON_FALSE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
            }
            /*If UE Index is not present*/
            else
            {
                /// We don't need to configure the CGI report for same PCI on different UE.
                ret_val = SON_FALSE;
            }
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "Ignore Strongest cell reports received for PCI( NCC [%d] and BSCC [%d]) and CGI is already configured for the same PCI.",
                    p_detected_cell->pci.ncc, p_detected_cell->pci.bscc);

            ret_val = SON_FALSE;
        }
    }
    /*If PCI is not present*/
    else
    {
        p_pci_list_node = (anr_geran_pci_value_node_t *)son_mem_get
            (sizeof(anr_geran_pci_value_node_t));

        if(p_pci_list_node != SON_PNULL)
        {
            /*SPR 15935 Fix Start*/
            /*Getting memory for ue_index and counter node*/
            p_ue_index_counter_list_node = (ue_index_and_counter_list_node_t *)son_mem_get
                (sizeof(ue_index_and_counter_list_node_t));

            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                SON_MEMSET(p_pci_list_node, 0x00, sizeof(anr_geran_pci_value_node_t));
                /*Inserting the PCI value*/
                /* Spr 18615 Changes Start */
                SON_MEMCPY(&p_pci_list_node->geran_pci_value.key.pci,
                        &p_detected_cell->pci,
                        sizeof(rrm_son_geran_physical_cell_id_t));
                p_pci_list_node->geran_pci_value.key.arfcn = p_detected_cell->arfcn;
                /* Spr 18615 Changes End */

                /// Set Absolute time for CGI Gaurd Timer.
                son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                /* SPR 15831 Start */
                p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS / MSECS_IN_SEC);
                /* SPR 15831 End */

                list_insert_node(&p_cell_ctxt->geran_pci_requested_cgi_list, &p_pci_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell:"
                        "number of nodes in geran_pci_requested_cgi_list are %d",
                        list_count(&p_cell_ctxt->geran_pci_requested_cgi_list));

                SON_MEMSET(p_ue_index_counter_list_node, 0x00, sizeof(ue_index_and_counter_list_node_t));
                /*Inserting the ue_index and counter values*/
                p_ue_index_counter_list_node->ue_index_and_counter_values.ue_index = ue_index;
                p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                list_insert_node(&p_pci_list_node->geran_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell:"
                        "number of nodes in ue_index_and_counter list are %d",
                        list_count(&p_pci_list_node->geran_pci_value.ue_index_and_counter_list));
                ret_val = SON_TRUE;
            }
            /*SPR 15935 Fix Stop*/
            else
            {
                son_mem_free(p_pci_list_node);
                /*SPR 15935 Fix Start*/
                p_pci_list_node = SON_PNULL;
                /*SPR 15935 Fix Stop*/
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell: "
                        "Memory allocation failure for ue_index and counter list");
            }

        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell: "
                    "Memory allocation failure for PCI list");
        }
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
}

/*SPR-9759-End*/
/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_process_geran_meas_results
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  list_size              - Number of GERAN cells in the meas 
 *                                           results
 *                  p_meas_list            - Pointer to the GERAN freq info in the 
 *                                           meas results
 * Outputs        : p_oam_update_notification_for_add_op - Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is added in NRT
 *                  p_oam_update_notification_for_update_op - Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is updated in NRT
 *                  p_rrm_req              - Pointer to the meas config request
 *                                           structure to be sent to RRM if 
 *                                           cells found with no CGI information.
 * Returns        : son_void_t             - None
 * Description    : Function to process the measurement information of all the 
 *                  GERAN cells received in RRM_MEAS_RESULT_IND message.
 *****************************************************************************/
/* SPR 15698 Start */
static son_void_t
anr_process_geran_meas_results
(
 anr_cell_context_t              *p_cell_ctxt,
 son_u8                          list_size,
 rrm_son_geran_meas_results_t    *p_meas_list,
 rrmc_meas_report_event_et        meas_report_event,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_add_op,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op,
 son_anr_rrm_updated_nrt_info_t	*p_rrm_notif,
 son_rrm_meas_config_req_t       *p_rrm_req,
 son_u16                          ue_index  /*Last parameter ue_index added for SPR-9759*/
 )
{
    son_u8 outer_idx     = 0;
    son_u8 inner_idx     = 0;
    son_u8 nbr_list_size = 0;
    rrm_son_geran_neighbor_cell_t     *p_nbr_info                  = SON_PNULL;
    rrm_son_geran_carrier_freq_info_t *p_geran_freq_info_list      = SON_PNULL;
    son_u8                            *p_geran_freq_info_list_size = SON_PNULL;
    son_error_et                      ret_val                      = SON_NO_ERROR;
    son_global_cell_id_t     gl_cell_id;
    son_bool_et              is_cgi_to_be_requested = SON_FALSE; /*Added for SPR 9759*/

    anr_inter_rat_hash_rec_t rec = {0};


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_cell_ctxt
            || SON_PNULL == p_meas_list 
            || SON_PNULL == p_oam_update_notification_for_add_op
            || SON_PNULL == p_oam_update_notification_for_update_op
            || SON_PNULL == p_rrm_req)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_process_geran_meas_results: Could not process utran "
                "meas results, p_cell_ctxt or  p_meas_list or "
                "p_update_notification or p_rrm_req is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&gl_cell_id, 0, sizeof(son_global_cell_id_t));
    gl_cell_id.bitmask |= SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT; 
    gl_cell_id.inter_rat_global_cell_id.bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT; 

    p_geran_freq_info_list = p_rrm_req->config_req.inter_rat_meas_config.
        geran_config_list.geran_freq_info_list;

    p_geran_freq_info_list_size = &p_rrm_req->config_req.inter_rat_meas_config.
        geran_config_list.geran_freq_info_list_size;

    /* Outer loop to traverse all ARFCNs */
    for (outer_idx = 0; outer_idx < list_size; outer_idx++)
    {
        nbr_list_size = p_meas_list[outer_idx].neighbor_cell_list_size;
        p_nbr_info    = p_meas_list[outer_idx].neighbor_cell_list;

        /*Inner Loop to traverse all GERAN nbrs of a particular ARFCN*/
        for (inner_idx = 0;  inner_idx < nbr_list_size; inner_idx++)
        {
            is_cgi_to_be_requested = SON_FALSE; /*Added for SPR-9759*/
            if (RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT & p_nbr_info[inner_idx].bitmask)
            {
                /* Coverity Fix: 19331 */
                anr_add_geran_cell_to_inter_rat_nrt(
                        p_cell_ctxt, &p_nbr_info[inner_idx],
                        &rec,
                        p_oam_update_notification_for_add_op,
                        p_oam_update_notification_for_update_op,
                        p_rrm_notif);
            }
            else
            {
                ret_val = anr_find_geran_pci_in_inter_rat(
                        p_cell_ctxt,
                        &p_nbr_info[inner_idx],
                        p_oam_update_notification_for_update_op);

                if (SON_ERR_ANR_NR_NOT_FOUND == ret_val )
                {
                    /*SPR-9759-Start*/
                    is_cgi_to_be_requested = anr_check_if_is_cgi_required_is_to_be_true_for_geran_cell(
                            p_cell_ctxt,
                            &p_nbr_info[inner_idx],
                            ue_index,
                            meas_report_event);
                    /*SPR-9759-End*/
                    if(is_cgi_to_be_requested) /*If clause added for SPR-9759*/
                    {
                        if (*p_geran_freq_info_list_size < MAX_SON_MEAS_CONFIG_OBJECT)
                        {
                            p_geran_freq_info_list[*p_geran_freq_info_list_size].
                                bitmask |= RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT;

                            p_geran_freq_info_list[*p_geran_freq_info_list_size].
                                geran_car_freqs.starting_arfcn = 
                                p_nbr_info[inner_idx].arfcn;

                            p_geran_freq_info_list[*p_geran_freq_info_list_size].
                                geran_car_freqs.band_ind = 
                                p_nbr_info[inner_idx].band_ind;

                            p_geran_freq_info_list[*p_geran_freq_info_list_size].bitmask |=
                                RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT;

                            SON_MEMCPY(&p_geran_freq_info_list[*p_geran_freq_info_list_size].
                                    cells_for_which_to_report_cgi[0],
                                    &p_nbr_info[inner_idx].pci, sizeof(rrm_son_geran_physical_cell_id_t));

                            p_geran_freq_info_list[*p_geran_freq_info_list_size].
                                cells_for_which_to_report_cgi_size++;

                            p_geran_freq_info_list[*p_geran_freq_info_list_size].bitmask |=
                                RRM_SON_GERAN_CFI_MEAS_OBJ_ID_PRESENT;

                            p_geran_freq_info_list[*p_geran_freq_info_list_size].meas_obj_id
                                = p_meas_list[outer_idx].meas_obj_id;

                            *p_geran_freq_info_list_size = *p_geran_freq_info_list_size + 1;
                        }
                        else
                        {
                            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                    SON_DETAILED,
                                    "anr_process_geran_meas_results: Geran Meas Config "
                                    "Object list size is [%d] >= MAX_SON_MEAS_CONFIG_OBJECT",
                                    *p_geran_freq_info_list_size); 
                        }
                    }
                }
            }/*end of else..if CGI is not present*/
        }/*inner for loop*/
    }/*outer for loop*/

    SON_UT_TRACE_EXIT();
    return;
} /* anr_process_geran_meas_results */

/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_find_cdma_pci_in_inter_rat
 * Inputs         : p_cell_ctxt     - Pointer to cell context
 *                  p_detected_cell - Structure storing information of the 
 *                                    neighbor cell found
 *                  p_oam_update_notification_for_update_op - Structure storing
 *                                    updates done in NRT to be sent to OAM
 * Output         : son_error_et    - Error Code if error occurs otherwise 
 *                                    SON_NO_ERROR 
 * Returns        : SON_NO_ERROR    - If PCI of neighbor cell found in NRT,
 *                                    Error Code Otherwise
 * Description    : Function to find the cell with given PCI and earfcn in the 
 *                  inter RAT NRT 
 *****************************************************************************/
static son_error_et
anr_find_cdma_pci_in_inter_rat
(
 anr_cell_context_t              *p_cell_ctxt,
 rrm_son_cdma_neighbor_cell_t   *p_detected_cell,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op
 )
{
	/* Coverity CID-55064 Fix Starts */
	anr_inter_rat_hash_nr_tuple_t   *p_inter_tpl    = SON_PNULL;
	son_cdma_neighbor_info_t        *p_cdma_data    = SON_PNULL;
	son_inter_rat_neighbor_info_t   inter_rat_nr            = {0};
	SON_HASH                        *p_hash                 = SON_PNULL;
	son_bool_et                     is_power_values_updated = SON_FALSE; 


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt 
			|| SON_PNULL == p_detected_cell
			|| SON_PNULL == p_oam_update_notification_for_update_op)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"No Inter-RAT neighbor with "
				"matching PCI could be found, p_cell_ctxt or p_detected_cell"
				" or p_oam_update_notification_for_update_op is NULL");
		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS;
	}

	p_hash = &p_cell_ctxt->inter_rat_neighbor_table;
	p_inter_tpl  = anr_get_first_nr_from_inter_rat_hash_table(p_hash);

	while (SON_PNULL != p_inter_tpl)
	{
		p_cdma_data = &p_inter_tpl->data.u.cdma_neighbor_info;

		/* Coverity Fix: 30290 */
		if ((p_detected_cell->arfcn == p_cdma_data->arfcn) &&
				(p_detected_cell->band_class == anr_map_son_cdma_band_class(
											     p_cdma_data->band_class)) &&
				(p_detected_cell->pci == p_cdma_data->pci))
		{
			if (!(SON_CDMA_PILOT_PN_PHASE_PRESENT & p_cdma_data->bitmask) ||
					(p_detected_cell->pilot_pn_phase > p_cdma_data->pilot_pn_phase))
			{
				p_cdma_data->bitmask |= SON_CDMA_PILOT_PN_PHASE_PRESENT;
				p_cdma_data->pilot_pn_phase = p_detected_cell->pilot_pn_phase;
				is_power_values_updated = SON_TRUE; 
			}

			if (!(SON_CDMA_PILOT_STRENGTH_PRESENT & p_cdma_data->bitmask) ||
					(p_detected_cell->pilot_strength > p_cdma_data->pilot_strength))
			{
				p_cdma_data->bitmask |= SON_CDMA_PILOT_STRENGTH_PRESENT;
				p_cdma_data->pilot_strength = p_detected_cell->pilot_strength;
				is_power_values_updated = SON_TRUE; 
			}

			if (SON_TRUE == is_power_values_updated)
			{
				p_oam_update_notification_for_update_op->nrt_op = SON_NRT_OP_UPDATE;

				anr_convert_inter_rat_hash_rec_to_inter_rat_nr(
						&p_inter_tpl->data, &inter_rat_nr);

				/*Populate the notification msg to OAM for updation in 
				 * CDMA nbr*/
				anr_construct_oam_updated_ncl(
						&p_oam_update_notification_for_update_op->
						nr_list, &inter_rat_nr, SON_TRUE);
			}

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF, 
					"CDMA Neighbor Cell has a matching PCI [%d]",
					p_cdma_data->pci);

			anr_print_cdma_cell_id(&p_cdma_data->nbr_cgi);

			son_time(&p_inter_tpl->data.last_activity_ts);

			SON_UT_TRACE_EXIT();
			return SON_NO_ERROR;
		}

		is_power_values_updated = SON_FALSE;

		p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
				p_hash, &p_inter_tpl->anchor);      
	}

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_BRIEF,
			"No Inter-RAT neighbor with "
			"matching PCI could be found");

	SON_UT_TRACE_EXIT();
	return SON_ERR_ANR_NR_NOT_FOUND;
	/* Coverity CID-55064 Fix Ends */
} /* anr_find_cdma_pci_in_inter_rat */

/*SPR-9759-Start*/

/******************************************************************************
 * Function Name  : anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  p_detected_cell        - Detected CDMA cell
 *                : ue_index               - Index of the UE from which meas
 *                                           results have come
 * Outputs        : 
 * Returns        : son_bool_et              - Returns SON_TRUE on success else 
 *                                           SON_FALSE
 * Description    : Function to check whether CGI request has to be sent for a PCI 
 *                  or not
 *****************************************************************************/
/* SPR 15698 Start */ 
static son_bool_et
anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell
(
 anr_cell_context_t              *p_cell_ctxt,
 rrm_son_cdma_neighbor_cell_t    *p_detected_cell,
 son_u16                          ue_index,
 rrmc_meas_report_event_et        meas_report_event
 )
{
    anr_eutran_cdma_pci_value_node_t  *p_pci_list_node = SON_PNULL;
    ue_index_and_counter_list_node_t  *p_ue_index_counter_list_node = SON_PNULL;
    son_bool_et                        ret_val = SON_FALSE;

    /*SPR 18360 Fix Start*/
    anr_eutran_cdma_key_t     key = {0};
    /*SPR 18360 Fix Stop*/
    SON_UT_TRACE_ENTER();

    if (p_cell_ctxt == SON_PNULL || p_detected_cell == SON_PNULL)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell: "
                "either p_cell_ctxt or p_detected_cell received is NULL");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    /*Finding the PCI value in CDMA PCI value list*/
    /*SPR 18360 Fix Start*/
    /*Finding the PCI-EARFCN node in PCI-EARFCN list*/
    key.t                       = CDMA_KEY;
    key.key.cdma_key.pci             = p_detected_cell->pci;
    key.key.cdma_key.band_class      = (son_cdma_band_class_et)p_detected_cell->band_class;
    key.key.cdma_key.arfcn           = p_detected_cell->arfcn;
    /*SPR 18360 Fix Stop*/
    p_pci_list_node = anr_find_record_in_eutran_cdma_pci_value_list(
            &p_cell_ctxt->cdma_pci_requested_cgi_list,
            /*SPR 18360 Fix Start*/
            /*Lines Deleted*/
            &key);
            /*SPR 18360 Fix Stop*/

    /*If PCI is present*/
    if(p_pci_list_node != SON_PNULL)
    {
        /// Check whether an Empty CGI report is received. Since, we don't need to configure the
        /// same PCI for CGI if strongest cell report is received for the same PCI.
        if(meas_report_event == MEAS_REP_EVENT_FOR_CGI)
        {
            p_ue_index_counter_list_node = anr_find_record_in_ue_index_and_counter_list(
                    &p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list,
                    ue_index);

            /*If UE Index is present*/
            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                if(p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter
                        < g_config_params.anr_config_params.anr_max_num_cgi_request)
                {
                    /// Refresh Timer Set Absolute time for CGI Gaurd Timer.
                    son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                    /* SPR 15831 Start */
                    p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS / MSECS_IN_SEC);
                    /* SPR 15831 End */

                    p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                    ret_val = SON_TRUE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
                else
                {
                    /*Deleting ue_index and counter value node */
                    list_delete_node(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                    son_mem_free(p_ue_index_counter_list_node);
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell:"
                            "number of nodes in ue_index_and_counter list are %d",
                            list_count(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list));
                    ret_val = SON_FALSE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
            }
            /*If UE Index is not present*/
            else
            {
                /// We don't want to configure the CGI for the same PCI event though Strongest Cell
                /// reports came from the different UE.
                ///
                ret_val = SON_FALSE;
            }
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "Ignore Strongest cell reports received for PCI = %d and CGI is already configured for the same PCI.",
                    p_detected_cell->pci);

            ret_val = SON_FALSE;
        }
    }
    /*If PCI is not present*/
    else
    {
        p_pci_list_node = (anr_eutran_cdma_pci_value_node_t *)son_mem_get
            (sizeof(anr_eutran_cdma_pci_value_node_t));

        if(p_pci_list_node != SON_PNULL)
        {
            /*SPR 15935 Fix Start*/
            /*Getting memory for ue_index and counter node*/
            p_ue_index_counter_list_node = (ue_index_and_counter_list_node_t *)son_mem_get
                (sizeof(ue_index_and_counter_list_node_t));

            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                SON_MEMSET(p_pci_list_node, 0x00, sizeof(anr_eutran_cdma_pci_value_node_t));
                /*Inserting the PCI value*/
                /*SPR 18360 Fix Start*/
                p_pci_list_node->eutran_cdma_pci_value.key  = key;
                /*SPR 18360 Fix Stop*/

                /// Set Absolute time for CGI Gaurd Timer.
                son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                /* SPR 15831 Start */
                p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS/MSECS_IN_SEC);
                /* SPR 15831 End */

                list_insert_node(&p_cell_ctxt->cdma_pci_requested_cgi_list, &p_pci_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell:"
                        "number of nodes in cdma_pci_requested_cgi_list are %d",
                        list_count(&p_cell_ctxt->cdma_pci_requested_cgi_list));

                SON_MEMSET(p_ue_index_counter_list_node, 0x00, sizeof(ue_index_and_counter_list_node_t));
                /*Inserting the ue_index and counter values*/
                p_ue_index_counter_list_node->ue_index_and_counter_values.ue_index = ue_index;
                p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                list_insert_node(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell:"
                        "number of nodes in ue_index_and_counter list are %d",
                        list_count(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list));
                ret_val = SON_TRUE;
            }
            /*SPR 15935 Fix Stop*/
            else
            {
                son_mem_free(p_pci_list_node);
                /*SPR 15935 Fix Start*/
                p_pci_list_node = SON_PNULL;
                /*SPR 15935 Fix Stop*/
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell: "
                        "Memory allocation failure for ue_index and counter list");
            }

        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell: "
                    "Memory allocation failure for PCI list");
        }
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  p_detected_cell        - Detected UTRAN cell
 *                : ue_index               - Index of the UE from which meas
 *                                           results have come
 * Outputs        : 
 * Returns        : son_bool_et              - Returns SON_TRUE on success else 
 *                                           SON_FALSE
 * Description    : Function to check whether CGI request has to be sent for a PCI 
 *                  or not
 *****************************************************************************/
/* SPR 15698 Start */ 
static son_bool_et
anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell
(
 anr_cell_context_t              *p_cell_ctxt,
 rrm_son_utran_neighbor_cell_t   *p_detected_cell,
 son_u16                          ue_index,
 rrmc_meas_report_event_et        meas_report_event,
 /*SPR 18360 Fix Start*/
 son_u16                          uarfcn
 /*SPR 18360 Fix Stop*/
 )
{
    anr_utran_pci_value_node_t  *p_pci_list_node = SON_PNULL;
    ue_index_and_counter_list_node_t  *p_ue_index_counter_list_node = SON_PNULL;
    son_bool_et                        ret_val = SON_FALSE;

    /*SPR 18360 Fix Start*/
    anr_utran_key_t  key;
    /*SPR 18360 Fix Stop*/
    SON_UT_TRACE_ENTER();

    if (p_cell_ctxt == SON_PNULL || p_detected_cell == SON_PNULL)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell: "
                "either p_cell_ctxt or p_detected_cell received is NULL");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    /*SPR 18360 Fix Start*/
    /*Finding the PCI-UARFCN value in PCI-UTRAN PCI value list*/
    SON_MEMSET(&key,0,sizeof(anr_utran_key_t));
    key.pci     = p_detected_cell->pci;
    key.uarfcn  = uarfcn;
    /*SPR 18360 Fix Stop*/
    /*Finding the PCI value in UTRAN PCI value list*/
    p_pci_list_node = anr_find_record_in_utran_pci_value_list(
            &p_cell_ctxt->utran_pci_requested_cgi_list,
            /*SPR 18360 Fix Start*/
            /*Lines Deleted*/
            &key);
            /*SPR 18360 Fix Stop*/

    /*If PCI is present*/
    if(p_pci_list_node != SON_PNULL)
    {
        /// Check whether an Empty CGI report is received. Since, we don't need to configure the
        /// same PCI for CGI if strongest cell report is received for the same PCI.
        if(meas_report_event == MEAS_REP_EVENT_FOR_CGI)
        {
            p_ue_index_counter_list_node = anr_find_record_in_ue_index_and_counter_list(
                    &p_pci_list_node->utran_pci_value.ue_index_and_counter_list,
                    ue_index);

            /*If UE Index is present*/
            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                if(p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter
                        < g_config_params.anr_config_params.anr_max_num_cgi_request)
                {
                    /// Refresh Timer Set Absolute time for CGI Gaurd Timer.
                    son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                    /* SPR 15831 Start */
                    p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS/MSECS_IN_SEC);
                    /* SPR 15831 End */

                    p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                    ret_val = SON_TRUE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
                else
                {
                    /*Deleting ue_index and counter value node */
                    list_delete_node(&p_pci_list_node->utran_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                    son_mem_free(p_ue_index_counter_list_node);
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell:"
                            "number of nodes in ue_index_and_counter list are %d",
                            list_count(&p_pci_list_node->utran_pci_value.ue_index_and_counter_list));
                    ret_val = SON_FALSE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
            }
            /*If UE Index is not present*/
            else
            {
                /// We don't need to configure the CGI report for same PCI on different UE.
                ret_val = SON_FALSE;
            }
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "Ignore Strongest cell reports received for PCI([PCI_FDD=%d] [PCI_TDD=%d]) and CGI is already configured for the same PCI.",
                    p_detected_cell->pci.pci_fdd, p_detected_cell->pci.pci_tdd);

            ret_val = SON_FALSE;
        }
    }
    /*If PCI is not present*/
    else
    {
        p_pci_list_node = (anr_utran_pci_value_node_t *)son_mem_get
            (sizeof(anr_utran_pci_value_node_t));

        if(p_pci_list_node != SON_PNULL)
        {
            /*SPR 15935 Fix Start*/
            /*Getting memory for ue_index and counter node*/
            p_ue_index_counter_list_node = (ue_index_and_counter_list_node_t *)son_mem_get
                (sizeof(ue_index_and_counter_list_node_t));

            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                SON_MEMSET(p_pci_list_node, 0x00, sizeof(anr_utran_pci_value_node_t));
                /*Inserting the PCI value*/
                
                /*SPR 18360 Fix Start*/
                p_pci_list_node->utran_pci_value.key = key;
                /*SPR 18360 Fix Stop*/
	
                /// Set Absolute time for CGI Gaurd Timer.
                son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                /* SPR 15831 Start */
                p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS/MSECS_IN_SEC);
                /* SPR 15831 End */

                list_insert_node(&p_cell_ctxt->utran_pci_requested_cgi_list, &p_pci_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell:"
                        "number of nodes in utran_pci_requested_cgi_list are %d",
                        list_count(&p_cell_ctxt->utran_pci_requested_cgi_list));

                SON_MEMSET(p_ue_index_counter_list_node, 0x00, sizeof(ue_index_and_counter_list_node_t));
                /*Inserting the ue_index and counter values*/
                p_ue_index_counter_list_node->ue_index_and_counter_values.ue_index = ue_index;
                p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                list_insert_node(&p_pci_list_node->utran_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell:"
                        "number of nodes in ue_index_and_counter list are %d",
                        list_count(&p_pci_list_node->utran_pci_value.ue_index_and_counter_list));
                ret_val = SON_TRUE;
            }
            /*SPR 15935 Fix Stop*/
            else
            {
                son_mem_free(p_pci_list_node);
                /*SPR 15935 Fix Start*/
                p_pci_list_node = SON_PNULL;
                /*SPR 15935 Fix Stop*/
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell: "
                        "Memory allocation failure for ue_index and counter list");
            }

        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell: "
                    "Memory allocation failure for PCI list");
        }
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
}

/*SPR-9759-End*/
/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_process_cdma_meas_results
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  list_size              - Number of CDMA2000 cells in the meas 
 *                                           results
 *                  p_meas_list            - Pointer to the CDMA2000 freq info in the 
 *                                           meas results
 * Outputs        : p_oam_update_notification_for_add_op - Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is added in NRT
 *                  p_oam_update_notification_for_update_op - Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is updated in NRT
 *                  p_rrm_req              - Pointer to the meas config request
 *                                           structure to be sent to RRM if 
 *                                           cells found with no CGI information.
 * Returns        : son_void_et            - None
 * Description    : Function to process the measurement information of all the 
 *                  CDMA cells received in RRM_MEAS_RESULT_IND message.
 *****************************************************************************/
/* SPR 15698 Start */ 
static son_void_t
anr_process_cdma_meas_results
(
 anr_cell_context_t              *p_cell_ctxt,
 son_u8                          list_size,
 rrm_son_cdma_meas_results_t    *p_meas_list,
 rrmc_meas_report_event_et        meas_report_event,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_add_op,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op,
 son_anr_rrm_updated_nrt_info_t	*p_rrm_notif,
 son_rrm_meas_config_req_t       *p_rrm_req,
 son_u16                          ue_index  /*Last parameter ue_index added for SPR-9759*/
 )
{
    son_u8 outer_idx     = 0;
    son_u8 inner_idx     = 0;
    son_u8 nbr_list_size = 0;
    son_error_et                      error_code                   = SON_NO_ERROR;
    rrm_son_cdma_neighbor_cell_t     *p_nbr_info                  = SON_PNULL;
    rrm_son_cdma_carrier_freq_info_t *p_cdma_freq_info_list      = SON_PNULL;
    son_u8                            *p_cdma_freq_info_list_size = SON_PNULL;
    son_error_et                      ret_val                      = SON_NO_ERROR;
    son_global_cell_id_t     gl_cell_id;
    son_bool_et              is_cgi_to_be_requested = SON_FALSE; /*Added for SPR 9759*/

    anr_inter_rat_hash_rec_t rec = {0};


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_cell_ctxt
            || SON_PNULL == p_meas_list 
            || SON_PNULL == p_oam_update_notification_for_add_op
            || SON_PNULL == p_oam_update_notification_for_update_op
            || SON_PNULL == p_rrm_req)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_process_cdma_meas_results: Could not process cdma "
                "meas results, p_cell_ctxt or  p_meas_list or "
                "p_update_notification or p_rrm_req is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&gl_cell_id, 0, sizeof(son_global_cell_id_t));
    gl_cell_id.bitmask |= SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT; 
    gl_cell_id.inter_rat_global_cell_id.bitmask |= SON_CDMA_GLOBAL_CELL_ID_PRESENT; 

    p_cdma_freq_info_list = p_rrm_req->config_req.inter_rat_meas_config.
        cdma_config_list.cdma_freq_info_list;

    p_cdma_freq_info_list_size = &p_rrm_req->config_req.inter_rat_meas_config.
        cdma_config_list.cdma_freq_info_list_size;

    /* Outer loop to traverse all ARFCNs */
    for (outer_idx = 0; outer_idx < list_size; outer_idx++)
    {
        nbr_list_size = p_meas_list[outer_idx].neighbor_cell_list_size;
        p_nbr_info    = p_meas_list[outer_idx].neighbor_cell_list;

        /*Inner Loop to traverse all CDMA nbrs of a particular ARFCN*/
        for (inner_idx = 0; inner_idx < nbr_list_size; inner_idx++)
        {
            is_cgi_to_be_requested = SON_FALSE; /*Added for SPR-9759*/
            if (p_nbr_info[inner_idx].bitmask & RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT)
            {
                error_code = anr_add_cdma_cell_to_inter_rat_nrt(
                        p_cell_ctxt, &p_nbr_info[inner_idx],
                        &rec,
                        p_oam_update_notification_for_add_op,
                        p_oam_update_notification_for_update_op,
                        p_rrm_notif);

                if (error_code != SON_NO_ERROR)
                {
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_process_cdma_meas_results: Cannot add CDMA "
                            "NBR to NRT error code:%d", error_code);
                    anr_print_cdma_cell_id((son_cdma_global_cell_id_t *)&p_nbr_info->cgi);
                }
            }
            else
            {
                ret_val = anr_find_cdma_pci_in_inter_rat(
                        p_cell_ctxt,
                        &p_nbr_info[inner_idx],
                        p_oam_update_notification_for_update_op);

                if (SON_ERR_ANR_NR_NOT_FOUND == ret_val)
                {
                    /*SPR-9759-Start*/
                    is_cgi_to_be_requested = anr_check_if_is_cgi_required_is_to_be_true_for_cdma_cell(
                            p_cell_ctxt,
                            &p_nbr_info[inner_idx],
                            ue_index,
                            meas_report_event);
                    /*SPR-9759-End*/
                    if(is_cgi_to_be_requested) /*If clause added for SPR-9759*/
                    {
                        if (*p_cdma_freq_info_list_size < MAX_SON_MEAS_CONFIG_OBJECT)
                        {
                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].arfcn =
                                p_nbr_info[inner_idx].arfcn;

                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].band_class =
                                p_nbr_info[inner_idx].band_class;

                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].bitmask |=
                                RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT;

                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].
                                cells_for_which_to_report_cgi[0] =
                                p_nbr_info[inner_idx].pci;

                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].
                                cells_for_which_to_report_cgi_size++;

                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].meas_obj_id
                                = p_meas_list[outer_idx].meas_obj_id;

                            p_cdma_freq_info_list[*p_cdma_freq_info_list_size].bitmask |=
                                RRM_SON_CDMA_CFI_MEAS_OBJ_ID_PRESENT;

                            *p_cdma_freq_info_list_size = *p_cdma_freq_info_list_size + 1;
                        }
                        else
                        {
                            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                    SON_DETAILED,
                                    "anr_process_cdma_meas_results: Geran Meas Config "
                                    "Object list size is [%d] >= MAX_SON_MEAS_CONFIG_OBJECT",
                                    *p_cdma_freq_info_list_size);
                        }
                    }
                }
            }
        }/*inner for loop*/
    }/*outer for loop*/

    SON_UT_TRACE_EXIT(); 
    return;
} /* anr_process_cdma_meas_results */

/* SPR 15698 End */

/**********************************************************************************
 * Function Name  : anr_process_utran_meas_results
 * Inputs         : p_cell_ctxt            - Pointer to ANR cell context
 *                  list_size              - Number of UTRAN cells in the meas 
 *                                           results
 *                  p_meas_list            - Pointer to the UTRAN cells in the 
 *                                           meas results
 * Outputs        : p_oam_update_notification_for_add_op -  Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is added in NRT
 *                  p_oam_update_notification_for_update_op - Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is updated in NRT
 *                  p_rrm_req              - Pointer to the meas config request
 *                                         - structure to be sent to RRM if 
 *                                           cells found with no CGI information.
 *                  p_rrm_notif            - Pointer to message for rrm notification
 *                  p_rrm_req              - Pointer to message for rrm request
 * Returns        : son_void_t             - None
 * Description    : Function to process the measurement information of all the 
 *                  UTRAN cells received in RRM_MEAS_RESULT_IND message.
 ***********************************************************************************/
/* SPR 15698 Start */ 
static son_void_t
anr_process_utran_meas_results
(
 anr_cell_context_t              *p_cell_ctxt,
 son_u8                          list_size,
 rrm_son_utran_meas_results_t    *p_meas_list,
 rrmc_meas_report_event_et        meas_report_event,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_add_op,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op,
 son_anr_rrm_updated_nrt_info_t	*p_rrm_notif,
 son_rrm_meas_config_req_t       *p_rrm_req,
 son_u16                          ue_index  /*Last parameter ue_index added for SPR-9759*/
 )
{
    son_u8  idx1 = 0, idx2 = 0;
    rrm_son_utran_neighbor_cell_t   *p_detected_cell = SON_PNULL;
    son_error_et ret_val = SON_NO_ERROR;
    anr_inter_rat_hash_rec_t rec = {0};
    son_bool_et is_cgi_required = SON_FALSE;
    son_global_cell_id_t    gl_cell_id;
    son_bool_et             is_cgi_to_be_requested = SON_FALSE; /*Added for SPR 9759*/


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell_ctxt
            || SON_PNULL == p_meas_list 
            || SON_PNULL == p_oam_update_notification_for_add_op
            || SON_PNULL == p_oam_update_notification_for_update_op
            || SON_PNULL == p_rrm_req)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_process_utran_meas_results: Could not process utran "
                "meas results, p_cell_ctxt or  p_meas_list or "
                "p_update_notification or p_rrm_req is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&gl_cell_id, 0, sizeof(son_global_cell_id_t));
    gl_cell_id.bitmask |= SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT; 
    gl_cell_id.inter_rat_global_cell_id.bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT; 

    /* Outer for loop (executed the number of uarfcn values found in meas
     * results times) */
    for (idx1 = 0; idx1 < list_size; idx1++)
    {
        /* Inner for loop (executed the number of neighbor found for each
         * uarfcn) */
        for (idx2 = 0; idx2 < p_meas_list[idx1].neighbor_cell_list_size; idx2++)
        {
            p_detected_cell = &p_meas_list[idx1].neighbor_cell_list[idx2];

            is_cgi_to_be_requested = SON_FALSE; /*Added for SPR-9759*/

            /* Check if CGI is present, Add detected cell to NRT */
            if (RRM_SON_UTRAN_NC_CGI_PRESENT & p_detected_cell->bitmask)
            {
                /* Coverity Fix: 19332 */
                anr_add_utran_cell_to_inter_rat_nrt(
                        p_cell_ctxt, p_detected_cell,
                        p_meas_list[idx1].uarfcn, &rec,
                        p_oam_update_notification_for_add_op,
                        p_oam_update_notification_for_update_op,
                        p_rrm_notif);
            }
            /* Add PCI to cells_for_which_to_report_cgi_list */
            else
            {
                ret_val = anr_find_pci_in_inter_rat_nrt(
                        p_cell_ctxt,
                        p_meas_list[idx1].uarfcn,
                        p_detected_cell,
                        p_oam_update_notification_for_update_op);

                if (ret_val == SON_ERR_ANR_NR_NOT_FOUND)
                {
                    /*SPR-9759-Start*/
                    is_cgi_to_be_requested = anr_check_if_is_cgi_required_is_to_be_true_for_utran_cell(
                            p_cell_ctxt,
                            p_detected_cell,
                            ue_index,
                            meas_report_event,
                            /*SPR 18360 Fix Start*/
                            p_meas_list[idx1].uarfcn);
                    	    /*SPR 18360 Fix Stop*/
                    /*SPR-9759-End*/
                    if(is_cgi_to_be_requested) /*if clause added for SPR-9759*/
                    {
                        is_cgi_required = SON_TRUE;

                        /* Insert the PCI in the list of PCIs for which CGI and
                         * other attributes has to be fetched using meas config
                         * req */
                        if (RRM_PCI_FDD_PRESENT & p_detected_cell->pci.bitmask)
                        {
                            p_rrm_req->config_req.inter_rat_meas_config.
                                utran_config_list.utran_fdd_list[
                                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                                utran_fdd_list_size].cells_for_which_to_report_cgi[
                                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                                utran_fdd_list[p_rrm_req->config_req.inter_rat_meas_config.
                                utran_config_list.utran_fdd_list_size].
                                cells_for_which_to_report_cgi_size] =
                                p_detected_cell->pci.pci_fdd;

                            p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                                utran_fdd_list[p_rrm_req->config_req.inter_rat_meas_config.
                                utran_config_list.utran_fdd_list_size].
                                cells_for_which_to_report_cgi_size++;
                        }
                        else if (RRM_PCI_TDD_PRESENT &
                                p_detected_cell->pci.bitmask)
                        {
                            p_rrm_req->config_req.inter_rat_meas_config.
                                utran_config_list.utran_tdd_list[
                                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                                utran_tdd_list_size].cells_for_which_to_report_cgi[
                                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                                utran_tdd_list[p_rrm_req->config_req.inter_rat_meas_config.
                                utran_config_list.utran_tdd_list_size].
                                cells_for_which_to_report_cgi_size] =
                                p_detected_cell->pci.pci_tdd;

                            p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                                utran_tdd_list[p_rrm_req->config_req.inter_rat_meas_config.
                                utran_config_list.utran_tdd_list_size].
                                cells_for_which_to_report_cgi_size++;
                        }
                    }
                }
            }
        } /* End of inner for loop */

        /* Check the flag to ensure that the utran config list size is
         * increased only once for each uarfcn value found in UE
         * measurement results */
        if (SON_TRUE == is_cgi_required)
        {
            if (RRM_PCI_FDD_PRESENT & p_detected_cell->pci.bitmask)
            {
                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_fdd_list[
                    p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                    utran_fdd_list_size].
                    uarfcn = p_meas_list[idx1].uarfcn;

                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_fdd_list[
                    p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                    utran_fdd_list_size].
                    bitmask |= RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT;

                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_fdd_list_size++;
            }
            else if (RRM_PCI_TDD_PRESENT & p_detected_cell->pci.bitmask)
            {
                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_tdd_list[
                    p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                    utran_tdd_list_size].
                    uarfcn = p_meas_list[idx1].uarfcn;

                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_tdd_list[
                    p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.
                    utran_tdd_list_size].
                    bitmask |= RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT;

                p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_tdd_list_size++;
            }

            is_cgi_required = SON_FALSE;
        }
    } 

    SON_UT_TRACE_EXIT();
    return;
} /* anr_process_utran_meas_results */

/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_add_cell_to_intra_rat_nrt
 * Inputs         : p_cell_ctxt     - Pointer to ANR cell context
 *                  p_detected_cell - Pointer to detected cell
 *                  earfcn          - Earfcn value
 *                  p_rec           - Pointer to the inter RAT NRT record inserted
 *                  p_oam_update_notification_for_add - Pointer to the notification
 *                                    structure to be sent to OAM if neighbor is
 *                                    added in NRT
 *                  p_oam_update_notification_for_update - Pointer to the notific-
 *                                    ation structure to be sent to OAM if 
 *                                    neighbor is updated in NRT
 *                  p_rrm_update_notification - Pointer to the notification
 *                                    structure to be sent to RRM
 * Outputs        : None
 * Returns        : son_error_et     - Error Code if error occurs otherwise SON_NO_ERROR
 * Description    : Function to add a EUTRAN cell to intra RAT NRT.
 *****************************************************************************/
static son_error_et
anr_add_cell_to_intra_rat_nrt
(
 anr_cell_context_t                      *p_cell_ctxt,
 const rrm_son_eutran_neighbor_cell_t    *p_detected_cell, 
 const son_u32                           earfcn,
 anr_intra_rat_hash_rec_t                *p_rec,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_add,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update,
 son_anr_rrm_updated_nrt_info_t  *p_rrm_update_notification,
 son_bool_et                     *p_is_notif_reqd
 )
{
	son_error_et error_code = SON_NO_ERROR;
	anr_intra_rat_hash_rec_t  *p_hash_rec = SON_PNULL;
	anr_pci_sus_conf_node_t   *p_pci_sus_conf_node = SON_PNULL;
    	/*SPR 18360 Fix Start*/
    	anr_eutran_cdma_key_t     key = {0};
    	/*SPR 18360 Fix Stop*/


	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_cell_ctxt
			|| SON_PNULL == p_detected_cell)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_detected_cell is NULL"
				);

		SON_UT_TRACE_EXIT();
		return SON_ERR_INVALID_PARAMS; 
	}

	SON_MEMSET(p_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
	SON_MEMCPY(&p_rec->neighbor_info.nbr_cgi,
			&p_detected_cell->cgi,
			sizeof(son_intra_rat_global_cell_id_t));


	p_rec->neighbor_info.bitmask |= SON_EARFCN_PRESENT;
	p_rec->neighbor_info.earfcn = earfcn;

	anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor(p_rec);

	if (RRM_SON_EUTRAN_NC_PLMN_PRESENT &
			p_detected_cell->bitmask
			&& (MAX_PLMN_LIST >= p_detected_cell->num_plmn_id))
	{
		p_rec->neighbor_info.bitmask |= SON_PLMN_ID_PRESENT;
		p_rec->neighbor_info.num_plmn_id =
			p_detected_cell->num_plmn_id;
		SON_MEMCPY(p_rec->neighbor_info.plmn_id,
				p_detected_cell->plmn_id,
				sizeof(p_rec->neighbor_info.plmn_id));
	}
	else
	{
		/* SPR-612 Coverity Fix Starts */
		if (!(RRM_SON_EUTRAN_NC_PLMN_PRESENT & p_detected_cell->bitmask))
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"PLMN Id is not present"
					" for the cell [Cell Id: 0x%x]", 
					son_convert_char_cell_id_to_int_cell_id(
						(son_u8 *)p_detected_cell->cgi.cell_identity));
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_add_cell_to_intra_rat_nrt: Number of PLMN Ids[%u] "
					"is invalid for the cell [Cell Id: 0x%x]",
					p_detected_cell->num_plmn_id,
					son_convert_char_cell_id_to_int_cell_id(
						(son_u8 *)p_detected_cell->cgi.cell_identity));
		}
		/* SPR-612 Coverity Fix Ends */
	}

	if (RRM_SON_EUTRAN_NC_TAC_PRESENT &
			p_detected_cell->bitmask)
	{
		p_rec->neighbor_info.bitmask |= SON_TAC_PRESENT;
		SON_MEMCPY(p_rec->neighbor_info.tac,
				p_detected_cell->tac,
				SON_TAC_OCTET_SIZE);
	}

	if (RRM_SON_EUTRAN_NC_CSG_PRESENT &
			p_detected_cell->bitmask)
	{
		p_rec->neighbor_info.bitmask |= SON_INTRA_CSG_ID_PRESENT;
		SON_MEMCPY(p_rec->neighbor_info.csg_identity,
				p_detected_cell->csg_identity,
				SON_CSG_ID_OCTET_SIZE);
    }/*SPR 15799 Fix Start*/
    else
    {
        son_global_enb_id_t             enb_id;
        son_error_et                    err              = SON_NO_ERROR;
        son_nbr_enb_hash_tuple_t        *p_enb_ctxt      = SON_PNULL;
        /*Check for FEMTO eNB*/
        SON_MEMSET(&enb_id, 0, sizeof(enb_id));
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                "anr_add_cell_to_intra_rat_nrt: "
                "Searching for eNB context,assuming the neighbor belongs to the HOME eNB ");
        enb_id.enb_type = SON_HOME_ENB;
        /*SPR 15899 Fix Start*/
        SON_MEMCPY(&enb_id.plmn_id,
                        &p_rec->neighbor_info.nbr_cgi.plmn_id,
                        sizeof(son_cell_plmn_info_t));
        /*SPR 15899 Fix Stop*/

        enb_id.enb_id[0] = p_rec->neighbor_info.nbr_cgi.cell_identity[0];
        enb_id.enb_id[1] = p_rec->neighbor_info.nbr_cgi.cell_identity[1];
        enb_id.enb_id[2] = p_rec->neighbor_info.nbr_cgi.cell_identity[2];
        enb_id.enb_id[3] = p_rec->neighbor_info.nbr_cgi.cell_identity[3] & 0xF0;
        p_enb_ctxt   =  x2_find_nbr_enb(x2_get_enb_nrt(),
                 &enb_id,&err);
        if(SON_PNULL != p_enb_ctxt)
        {
            p_rec->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
            p_rec->neighbor_info.access_mode = SON_OPEN_FEMTO; 
        } 
        else
        {
            /*SPR 15899 Fix Start*/
            err  = SON_NO_ERROR;
            /*SPR 15899 Fix Stop*/
            SON_MEMSET(&enb_id, 0, sizeof(enb_id));
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_DETAILED,
                    "son_anr_set_x2_status_for_new_neighbor: "
                    "Searching for eNB Context, assuming the neighbor belongs to the MACRO eNB ");
            enb_id.enb_type = SON_MACRO_ENB;
            /*SPR 15899 Fix Start*/
            SON_MEMCPY(&enb_id.plmn_id,
                            &p_rec->neighbor_info.nbr_cgi.plmn_id,
                            sizeof(son_cell_plmn_info_t));
            /*SPR 15899 Fix Stop*/
            anr_determine_macro_enb_id(&p_rec->neighbor_info.nbr_cgi,&enb_id);
            p_enb_ctxt   =  x2_find_nbr_enb(x2_get_enb_nrt(),
                    &enb_id,&err);

            if(SON_PNULL != p_enb_ctxt)
            {
                p_rec->neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
                p_rec->neighbor_info.access_mode = SON_OPEN; 
            }
	}
    }/*SPR 15799 Fix Stop*/

	/*SPR-10230-Else loop of setting access_mode removed as it will be set on basis of pci range*/

	p_rec->neighbor_info.bitmask |= SON_PCI_PRESENT;
	p_rec->neighbor_info.pci = p_detected_cell->pci;

	p_rec->neighbor_info.bitmask |= SON_RSRP_PRESENT;
	p_rec->neighbor_info.rsrp = p_detected_cell->rsrp;

	p_rec->neighbor_info.bitmask |= SON_RSRQ_PRESENT;
	p_rec->neighbor_info.rsrq = p_detected_cell->rsrq;

	p_rec->neighbor_info.bitmask |= SON_ENABLE_NR_STATUS_PRESENT;
	p_rec->neighbor_info.nr_status = SON_NR_UNLOCKED;

	p_rec->neighbor_info.bitmask |= SON_ENABLE_HO_STATUS_PRESENT;
	p_rec->neighbor_info.ho_status = SON_HO_ALLOWED;

	/*SPR 15438 - Lines deleted*/
	/*SPR-9759-Start*/
	
	/*SPR 15438 Comment changed and if condition deleted*/

	/*CGI has been reported.If its PCI value is present in eutran_pci_requested_cgi_list,
	  then remove it from there as its CGI need not to be requested now*/
        /*SPR 18360 Fix Start*/
        key.t = EUTRAN_KEY; 
        key.key.eutran_key.pci    = p_detected_cell->pci;
        key.key.eutran_key.earfcn = earfcn;  
        anr_delete_pci_from_eutran_cdma_pci_requested_cgi_list(&p_cell_ctxt->eutran_pci_requested_cgi_list,                                                                                       &key); 
        /*SPR 18360 Fix Stop*/
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"number of nodes in eutran_pci_requested_cgi_list are %d",
				list_count(&p_cell_ctxt->eutran_pci_requested_cgi_list));
	

	/*SPR-9759-End*/

	/*SPR 15438 Lines deleted*/

	/*SPR 15438 - Start*/
    	anr_intra_rat_find_nr(&p_cell_ctxt->intra_rat_neighbor_table,
       	    &p_rec->neighbor_info.nbr_cgi,
            &error_code);
    	/*SPR 15438 - End*/

	if (SON_NO_ERROR == error_code) /*SPR 15438 if condition modified*/
        {
        /*SPR 15438 Lines deleted*/
		p_rec->neighbor_info.bitmask &= ~(SON_ENABLE_NR_STATUS_PRESENT | SON_ENABLE_HO_STATUS_PRESENT);
		p_hash_rec =  anr_intra_rat_modify_nr(
				p_cell_ctxt,
				p_rec,
				SON_FALSE,
				&error_code,
				p_oam_update_notification_for_add,
				p_oam_update_notification_for_update,
				p_rrm_update_notification,
				SON_TRUE,
				SON_FALSE);

		if (SON_PNULL != p_hash_rec &&
				((p_hash_rec->neighbor_info.earfcn != p_rec->neighbor_info.earfcn) ||
				 (p_hash_rec->neighbor_info.pci != p_rec->neighbor_info.pci) ||
				 /*SPR 16523 fix start*/
				 (p_hash_rec->neighbor_info.tac != p_rec->neighbor_info.tac)))
			         /*SPR 16523 fix stop*/
		{
			*p_is_notif_reqd = SON_TRUE;    
		}
		else if (SON_NO_ERROR == error_code)
		{
			SON_HASH_NODE *p_node = SON_PNULL;
			anr_intra_rat_hash_nr_tuple_t *p_tpl = SON_PNULL;

			/*1. Get the Suspected PCI context*/
			p_pci_sus_conf_node = anr_find_in_sus_pci_ctxt_list(p_cell_ctxt,
					p_detected_cell->pci, earfcn);
			/*2. Get the neighbor relation*/
			p_node  = hash_find(&p_cell_ctxt->intra_rat_neighbor_table,
					(son_void_t *)p_rec);
			p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
				YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);

			/* Coverity Fix: 23468 */
			if (SON_PNULL != p_pci_sus_conf_node && SON_PNULL != p_tpl)
			{
				/*3. Decrement the counter for the received cgi */
				if (0 !=  p_tpl->data.received_cgi_counter)
				{
					p_tpl->data.received_cgi_counter--;
				}

				/*4. Check the condition to determine if the suspected PCI 
				 * node can be deleted or not*/
				if (1 ==  p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count  &&
						0 ==  p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_counter &&
						0 ==  p_tpl->data.received_cgi_counter)
				{
					/*This confirms that there is only one nbr 
					 * and that too is in SUSPECTED state.Hence unmark the
					 * suspected pci field of the neighbor, send the notification to OAM.*/
					if (SON_TRUE == p_tpl->data.neighbor_info.suspected_pci &&
							SON_SUSPECT_PCI_PRESENT & p_tpl->data.neighbor_info.bitmask)
					{
						p_tpl->data.neighbor_info.suspected_pci = SON_FALSE;

						/*prepare the suspect PCI indication for cause UNSUSPECTED   
						 * Report the suspected  PCI indication */

						/* Klockwork 2.1.3 */
						if ((p_oam_update_notification_for_update->nr_list.intra_rat_nr_list_size > 0) &&
								(p_oam_update_notification_for_update->nr_list.intra_rat_nr_list_size <= 
								 SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS) &&
								(!SON_MEMCMP(&p_oam_update_notification_for_update->nr_list.intra_rat_nr_list[
									     p_oam_update_notification_for_update->nr_list.intra_rat_nr_list_size - 1].nbr_cgi,
									     &p_tpl->data.neighbor_info.nbr_cgi, sizeof(son_intra_rat_global_cell_id_t))))
						{
							p_oam_update_notification_for_update->nr_list.intra_rat_nr_list[
								p_oam_update_notification_for_update->nr_list.intra_rat_nr_list_size - 1].
								suspected_pci = SON_FALSE;
						}
						else
						{
							anr_construct_oam_updated_ncl(&p_oam_update_notification_for_update->nr_list,
									&p_tpl->data, SON_FALSE);
						}
					}

					/* delete the suspected PCI node */
					anr_delete_pci_ctxt_from_sus_pci_ctxt_list(p_cell_ctxt,
							p_tpl->data.neighbor_info.pci, p_tpl->data.neighbor_info.earfcn );
				}
				else
				{
					if (((0 ==  p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_counter) &&
								(0 != p_tpl->data.received_cgi_counter)) ||
							((0 != p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_counter) &&
							 (0 == p_tpl->data.received_cgi_counter)) ||
							((0 ==  p_pci_sus_conf_node->pci_sus_conf_data.ask_cgi_counter) &&
							 (0 == p_tpl->data.received_cgi_counter) &&
							 (p_pci_sus_conf_node->pci_sus_conf_data.nbr_cell_count > 1)))
					{
						/*Reset the asked-received counter - Thresholds*/   
						anr_reset_sus_pci_count(p_cell_ctxt, p_pci_sus_conf_node);
					}
				}
			}
		}
	}
	/*SPR 15438 - Start*/
    	/*Report the detected neighbors to OAM*/
    	else if(SON_ERR_ANR_NR_NOT_FOUND == error_code)
    	{
        	if (p_oam_update_notification_for_add)
        	{
                /*SPR 15799 Fix Start*/ 
                son_anr_set_x2_status_for_new_neighbor(p_cell_ctxt,p_rec,SON_PNULL);
                /*SPR 15799 Fix Stop*/
            		p_oam_update_notification_for_add->nrt_op = SON_NRT_OP_ADD;
            		anr_construct_oam_updated_ncl(&p_oam_update_notification_for_add->nr_list,
                    		&p_rec->neighbor_info,
                    		SON_FALSE);

            		error_code = SON_NO_ERROR;
        	}
    	}
    /*SPR 15438 - End*/

	SON_UT_TRACE_EXIT();
	return error_code;
} /* anr_add_cell_to_intra_rat_nrt */

/*SPR-9759-Start*/

/******************************************************************************
 * Function Name  : anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  p_detected_cell        - Detected EUTRAN cell
 *                : ue_index               - Index of the UE from which meas
 *                                           results have come
 * Outputs        : 
 * Returns        : son_bool_et              - Returns SON_TRUE on success else 
 *                                           SON_FALSE
 * Description    : Function to check whether CGI request has to be sent for a PCI 
 *                  or not
 *****************************************************************************/
/* SPR 15698 Start */ 
static son_bool_et
anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell
(
 anr_cell_context_t              *p_cell_ctxt,
 rrm_son_eutran_neighbor_cell_t  *p_detected_cell,
 son_u16                          ue_index,
 rrmc_meas_report_event_et        meas_report_event,
 /*SPR 18360 Fix Start*/
 son_u32                          earfcn
 /*SPR 18360 Fix Stop*/
 )
{
    anr_eutran_cdma_pci_value_node_t  *p_pci_list_node = SON_PNULL;
    ue_index_and_counter_list_node_t  *p_ue_index_counter_list_node = SON_PNULL;
    son_bool_et                        ret_val = SON_FALSE;
    /*SPR 18360 Fix Start*/
    anr_eutran_cdma_key_t                  key = {0};
    /*SPR 18360 Fix Stop*/

    SON_UT_TRACE_ENTER();

    if (p_cell_ctxt == SON_PNULL || p_detected_cell == SON_PNULL)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell: "
                "either p_cell_ctxt or p_detected_cell received is NULL");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    /*SPR 18360 Fix Start*/
    /*Finding the PCI-EARFCN node in PCI-EARFCN list*/
    key.t                       = EUTRAN_KEY;
    key.key.eutran_key.pci      = p_detected_cell->pci;
    key.key.eutran_key.earfcn   = earfcn;
    /*SPR 18360 Fix Stop*/
    /*Finding the PCI value in EUTRAN PCI value list*/
    p_pci_list_node = anr_find_record_in_eutran_cdma_pci_value_list(
            &p_cell_ctxt->eutran_pci_requested_cgi_list,
            /*SPR 18360 Fix Start*/
            &key);
            /*SPR 18360 Fix Stop*/

    /*If PCI is present*/
    if(p_pci_list_node != SON_PNULL)
    {
        /// Check whether an Empty CGI report is received. Since, we don't need to configure the
        /// same PCI for CGI if strongest cell report is received for the same PCI.
        if(meas_report_event == MEAS_REP_EVENT_FOR_CGI)
        {
            p_ue_index_counter_list_node = anr_find_record_in_ue_index_and_counter_list(
                    &p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list,
                    ue_index);

            /*If UE Index is present*/
            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                if(p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter
                        < g_config_params.anr_config_params.anr_max_num_cgi_request)
                {
                    /// Refresh Timer Set Absolute time for CGI Gaurd Timer.
                    son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                    /* SPR 15831 Start */
                    p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS/MSECS_IN_SEC);
                    /* SPR 15831 End */

                    p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                    ret_val = SON_TRUE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
                else
                {
                    /*Deleting ue_index and counter value node */
                    list_delete_node(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                    son_mem_free(p_ue_index_counter_list_node);
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell:"
                            "number of nodes in ue_index_and_counter list are %d",
                            list_count(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list));
                    ret_val = SON_FALSE;
                    SON_UT_TRACE_EXIT();
                    return ret_val;
                }
            }
            /*If UE Index is not present*/
            else
            {
                /// We don't want to configure the CGI for the same PCI event though Strongest Cell
                /// reports came from the different UE.
                ret_val = SON_FALSE;
            }
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "Ignore Strongest cell reports received for PCI = %d and CGI is already configured for the same PCI.",
                    p_detected_cell->pci);

            ret_val = SON_FALSE;
        }
    }
    /*If PCI is not present*/
    else
    {
        p_pci_list_node = (anr_eutran_cdma_pci_value_node_t *)son_mem_get
            (sizeof(anr_eutran_cdma_pci_value_node_t));

        if(p_pci_list_node != SON_PNULL)
        {
            /*SPR 15935 Fix Start*/
            /*Getting memory for ue_index and counter node*/
            p_ue_index_counter_list_node = (ue_index_and_counter_list_node_t *)son_mem_get
                (sizeof(ue_index_and_counter_list_node_t));

            if(p_ue_index_counter_list_node != SON_PNULL)
            {
                SON_MEMSET(p_pci_list_node, 0x00, sizeof(anr_eutran_cdma_pci_value_node_t));
                /*Inserting the PCI value*/
                /*SPR 18360 Fix Start*/
                p_pci_list_node->eutran_cdma_pci_value.key  = key;
		/*SPR 18360 Fix Stop*/
                /// Set Absolute time for CGI Gaurd Timer.
                son_time(&p_pci_list_node->absolute_cgi_gaurd_time_in_secs);

                /* SPR 15831 Start */
                p_pci_list_node->absolute_cgi_gaurd_time_in_secs += (CGI_GAURD_TIMEOUT_IN_MSECS/MSECS_IN_SEC);
                /* SPR 15831 End */

                list_insert_node(&p_cell_ctxt->eutran_pci_requested_cgi_list, &p_pci_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell:"
                        "number of nodes in eutran_pci_requested_cgi_list are %d",
                        list_count(&p_cell_ctxt->eutran_pci_requested_cgi_list));

                SON_MEMSET(p_ue_index_counter_list_node, 0x00, sizeof(ue_index_and_counter_list_node_t));
                /*Inserting the ue_index and counter values*/
                p_ue_index_counter_list_node->ue_index_and_counter_values.ue_index = ue_index;
                p_ue_index_counter_list_node->ue_index_and_counter_values.cgi_request_counter++;
                list_insert_node(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list, &p_ue_index_counter_list_node->anchor);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell:"
                        "number of nodes in ue_index_and_counter list are %d",
                        list_count(&p_pci_list_node->eutran_cdma_pci_value.ue_index_and_counter_list));
                ret_val = SON_TRUE;
            }
            /*SPR 15935 Fix Stop*/
            else
            {
                son_mem_free(p_pci_list_node);
                /*SPR 15935 Fix Start*/
                p_pci_list_node = SON_PNULL;
                /*SPR 15935 Fix Stop*/
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell: "
                        "Memory allocation failure for ue_index and counter list");
            }

        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell: "
                    "Memory allocation failure for PCI list");
        }
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
}

/*SPR-9759-End*/
/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_process_eutran_meas_results
 * Inputs         : p_cell_context         - Pointer to ANR cell context
 *                  list_size              - Number of EUTRAN cells in the meas 
 *                                           results
 *                  p_meas_list            - Pointer to the EUTRAN cells in the 
 *                                           meas results
 * Outputs        : p_oam_update_notification_for_add_op - Pointer to the 
 *                                           notification structure to be sent 
 *                                           to OAM if neighbor is added in NRT
 *                  p_oam_update_notification_for_update_op - Pointer to the
 *                                           notification structure to be sent
 *                                           to OAM if neighbor is updated in NRT
 *                  p_rrm_update_notification - Pointer to the notification
 *                                           structure to be sent to RRM 
 *                  p_rrm_req                - Pointer to the meas config request
 *                                           structure to be sent to RRM if 
 *                                           cells found with no CGI information.
 * Returns        : son_bool_et              - Returns SON_TRUE on success else 
 *                                           SON_FALSE
 * Description    : Function to process the measurement information of all the 
 *                  EUTRAN cells received in RRM_MEAS_RESULT_IND message.
 *****************************************************************************/
/* SPR 15698 Start */ 
static son_bool_et
anr_process_eutran_meas_results
(
 anr_cell_context_t              *p_cell_ctxt,
 son_u8                          list_size,
 rrm_son_eutran_meas_results_t   *p_meas_list,
 rrmc_meas_report_event_et       meas_report_event,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_add_op,
 son_anr_updated_nrt_info_t      *p_oam_update_notification_for_update_op,
 son_anr_rrm_updated_nrt_info_t  *p_rrm_update_notification,
 son_rrm_meas_config_req_t       *p_rrm_req,
 son_u16                          ue_index /*Last parameter ue_index added for SPR-9759*/
 )
{
    son_u8  idx1 = 0, idx2 = 0;
    rrm_son_eutran_neighbor_cell_t  *p_detected_cell = SON_PNULL;
    son_error_et ret_val = SON_NO_ERROR;
    anr_intra_rat_hash_rec_t rec;
    son_bool_et is_cgi_required = SON_FALSE;
    son_error_et error_code = SON_NO_ERROR;
    son_oam_anr_limited_mode_pci_info_t *p_limited_mode_pci_info = SON_PNULL;       
    son_anr_limited_mode_pci_info_t limited_mode_pci;
    son_global_cell_id_t     gl_cell_id;
    son_bool_et                                 ret = SON_FALSE;
    son_bool_et             is_x2_notif_reqd  = SON_FALSE;
    son_bool_et             is_cgi_to_be_requested = SON_FALSE; /*Added for SPR 9759*/


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_cell_ctxt
            || SON_PNULL == p_meas_list 
            || SON_PNULL == p_oam_update_notification_for_add_op
            || SON_PNULL == p_oam_update_notification_for_update_op 
            || SON_PNULL == p_rrm_update_notification
            || SON_PNULL == p_rrm_req)
    {
/* coverity_85478 start */
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_process_eutran_meas_results: Could not process meas "
                "results for EUTRAN cells, p_cell_ctxt or p_meas_list or "
                " p_update_notification or p_rrm_req is NULL");
/* coverity_85478 stop */
        SON_UT_TRACE_EXIT();
        return ret;
    }

    /*Coverity ID 72636 Starts */
    p_limited_mode_pci_info = (son_oam_anr_limited_mode_pci_info_t *)
        son_mem_get(sizeof(son_oam_anr_limited_mode_pci_info_t));
    if (SON_PNULL == p_limited_mode_pci_info)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_process_eutran_meas_results:"
                "Memory allocation for son_oam_anr_limited_mode_pci_info_t fails");
        SON_UT_TRACE_EXIT();
        return ret;
    }
    /*Coverity ID 72636 Ends */

    SON_MEMSET(&gl_cell_id, 0, sizeof(son_global_cell_id_t));
    gl_cell_id.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT; 

    limited_mode_pci.p_anr_limited_mode_pci_info_t = SON_PNULL;

    SON_MEMSET(p_limited_mode_pci_info, 0,
            sizeof(son_oam_anr_limited_mode_pci_info_t));

    SON_MEMCPY(&p_limited_mode_pci_info->src_cgi,
            &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));

    p_limited_mode_pci_info->pci_earfcn_list_size = list_size;

    /* Outer for loop (executed the number of earfcn values found in meas
     * results times) */
    for (idx1 = 0; idx1 < list_size; idx1++)
    {
        p_limited_mode_pci_info->pci_earfcn_list[idx1].earfcn = p_meas_list[idx1].earfcn;

        p_limited_mode_pci_info->pci_earfcn_list[idx1].pci_list_size = 
            p_meas_list[idx1].neighbor_cell_list_size;

        /* Inner for loop (executed the number of neighbor found for each 
         * earfcn times) */
        for (idx2 = 0; 
                idx2 < p_meas_list[idx1].neighbor_cell_list_size; 
                idx2++)
        {
            p_limited_mode_pci_info->pci_earfcn_list[idx1].pci_list[idx2]
                = p_meas_list[idx1].neighbor_cell_list[idx2].pci;

            p_detected_cell = &p_meas_list[idx1].neighbor_cell_list[idx2];

            is_x2_notif_reqd = SON_FALSE;
            is_cgi_to_be_requested = SON_FALSE; /*Added for SPR-9759*/
            /* Check if CGI is present, Add/Modify detected cell to NRT */
            if (RRM_SON_EUTRAN_NC_CGI_PRESENT & p_detected_cell->bitmask)
            {
                error_code =  anr_add_cell_to_intra_rat_nrt(p_cell_ctxt,
                        p_detected_cell,
                        p_meas_list[idx1].earfcn, &rec,
                        p_oam_update_notification_for_add_op,
                        p_oam_update_notification_for_update_op,
                        p_rrm_update_notification, &is_x2_notif_reqd);
                if (SON_NO_ERROR == error_code)
                {
                    if (SON_TRUE ==  is_x2_notif_reqd)
                    {
                        ret = SON_TRUE;
                    }
		
		    /*SPR 15438 - Lines deleted*/
                }
            }
            /* If CGI is not present lookup NRT to see if PCI is present or is suspected */
            else
            {
                ret_val = anr_find_pci_in_intra_rat_nrt(
                        p_cell_ctxt,
                        p_meas_list[idx1].earfcn,
                        p_detected_cell,
                        p_oam_update_notification_for_update_op);

                if (ret_val == SON_ERR_ANR_NR_NOT_FOUND)
                {
                    /*SPR-9759-Start*/
                    is_cgi_to_be_requested = anr_check_if_is_cgi_required_is_to_be_true_for_eutran_cell(
                            p_cell_ctxt,
                            p_detected_cell,
                            ue_index,
                            meas_report_event,
                            /*SPR 18360 Fix Start*/
                            p_meas_list[idx1].earfcn);
                            /*SPR 18360 Fix Stop*/
                    /*SPR-9759-End*/
                    if(is_cgi_to_be_requested)/*if clause added for SPR-9759*/
                    {
                        is_cgi_required = SON_TRUE;

                        /* Insert the PCI in the list of PCIs for which CGI and
                         * other attributes has to be fetched using meas config
                         * req */
                        rrm_son_meas_config_eutran_t *p_meas_config =
                            &(p_rrm_req->config_req.eutran_config_list[
                                    p_rrm_req->config_req.eutran_config_list_size]);
                        p_meas_config->cells_for_which_to_report_cgi[
                            p_meas_config->cells_for_which_to_report_cgi_size] =
                            p_detected_cell->pci;
                        p_meas_config->cells_for_which_to_report_cgi_size++;
                    }
                }
            }
        } /* End of inner for loop */

        /* Check the flag to ensure that the eutran config list size is 
         * increased only once for each earfcn value found in UE
         * measurement results */
        if (SON_TRUE == is_cgi_required)
        {
            p_rrm_req->config_req.eutran_config_list[
                p_rrm_req->config_req.eutran_config_list_size].earfcn = 
                p_meas_list[idx1].earfcn;
            p_rrm_req->config_req.eutran_config_list[
                p_rrm_req->config_req.eutran_config_list_size].bitmask |=
                RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT;
            p_rrm_req->config_req.eutran_config_list_size++;
            is_cgi_required = SON_FALSE;
        }
    }  

    if (SON_TRUE == p_cell_ctxt->limited_mode &&
            p_limited_mode_pci_info->pci_earfcn_list_size)
    {
        /* Klockwork 2.1.3 */
        limited_mode_pci.p_anr_limited_mode_pci_info_t = son_mem_get(sizeof(son_oam_anr_limited_mode_pci_info_t));
        if(SON_PNULL == limited_mode_pci.p_anr_limited_mode_pci_info_t)
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_process_eutran_meas_results: Memory Allocation Failure");
        }
        else
        {
            SON_MEMCPY(limited_mode_pci.p_anr_limited_mode_pci_info_t,
                    p_limited_mode_pci_info,
                    sizeof(son_oam_anr_limited_mode_pci_info_t));
            son_create_send_buffer((son_u8 *)&limited_mode_pci,
                    SON_ANR_MODULE_ID, SON_MIF_MODULE_ID, 
                    SONANR_LIMITED_MODE_PCI_INFO,
                    sizeof(son_anr_limited_mode_pci_info_t));
            /*coverity 72637 fix start*/
            son_mem_free(limited_mode_pci.p_anr_limited_mode_pci_info_t);
            limited_mode_pci.p_anr_limited_mode_pci_info_t = SON_PNULL;/*coverity 94822 fix*/
            /*coverity 72637 fix end*/
        }
    }

    if (SON_PNULL != p_limited_mode_pci_info)
    {
        son_mem_free(p_limited_mode_pci_info);
    }
    /* SPR 15935 Fix Start */
    if(SON_PNULL != limited_mode_pci.p_anr_limited_mode_pci_info_t)
    {
        son_mem_free(limited_mode_pci.p_anr_limited_mode_pci_info_t);
        limited_mode_pci.p_anr_limited_mode_pci_info_t = SON_PNULL;
    }
    /* SPR 15935 Fix Stop */

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_eutran_meas_results */

/* SPR 15698 End */

/******************************************************************************
 * Function Name  : anr_fsm_ue_meas_results_ind
 * Inputs         : p_cspl_hdr      - Pointer to CSPL header 
 *                  p_cell_context  - Pointer to ANR cell context
 *                  evnt_id         - API Id
 *                  p_msg           - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler to handle RRM_MEAS_RESULT_IND message.
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_ue_meas_results_ind
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_rrm_meas_results_ind_t *p_meas_results = (son_rrm_meas_results_ind_t *)p_msg;
	son_anr_updated_nrt_info_t *p_oam_update_notification_for_add_op = SON_PNULL;
	son_anr_updated_nrt_info_t *p_oam_update_notification_for_update_op = SON_PNULL;
	son_anr_rrm_updated_nrt_info_t *p_rrm_update_notification = SON_PNULL;
	son_rrm_meas_config_req_t *p_rrm_req = SON_PNULL;

	son_x2_nrt_update_ind_t   nrt_upd_ind;
	son_bool_et               ret = SON_FALSE;
	son_u16                   ue_index = p_meas_results->ue_index; /*Added for SPR-9759*/


	SON_UT_TRACE_ENTER();

	if (!p_meas_results->eutran_meas_list_size && 
			!p_meas_results->utran_meas_list_size &&
			!p_meas_results->geran_meas_list_size &&
			!p_meas_results->cdma_meas_list_size)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Empty Measurement Result"
				" Indication received from RRM for cell [Cell Id: 0x%x] evnt_id %u",
				son_convert_char_cell_id_to_int_cell_id(p_meas_results->cell_id.cell_identity)
                ,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER; 
	}

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_oam_update_notification_for_add_op = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_oam_update_notification_for_add_op)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation Failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER; 
	}

	p_oam_update_notification_for_update_op = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_oam_update_notification_for_update_op)
	{
		if (SON_PNULL != p_oam_update_notification_for_add_op)
		{
			son_mem_free(p_oam_update_notification_for_add_op);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation Failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_rrm_update_notification = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_rrm_update_notification)
	{
		if (SON_PNULL != p_oam_update_notification_for_add_op)
		{
			son_mem_free(p_oam_update_notification_for_add_op);
		}

		if (SON_PNULL != p_oam_update_notification_for_update_op)
		{
			son_mem_free(p_oam_update_notification_for_update_op);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation Failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_rrm_req = (son_rrm_meas_config_req_t *)son_mem_get(sizeof(son_rrm_meas_config_req_t));
	if (SON_PNULL == p_rrm_req)
	{
		if (SON_PNULL != p_oam_update_notification_for_add_op)
		{
			son_mem_free(p_oam_update_notification_for_add_op);
		}

		if (SON_PNULL != p_oam_update_notification_for_update_op)
		{
			son_mem_free(p_oam_update_notification_for_update_op);
		}

		if (SON_PNULL != p_rrm_update_notification)
		{
			son_mem_free(p_rrm_update_notification);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_fsm_ue_meas_results_ind: Memory Allocation Failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}
    /*SPR 14925 Fix Start */
    /*
	p_rrm_req  = (son_rrm_meas_config_req_t *)son_mem_get(sizeof(son_rrm_meas_config_req_t));
	if (SON_PNULL == p_rrm_req)
	{
		if (SON_PNULL != p_oam_update_notification_for_add_op)
		{
			son_mem_free(p_oam_update_notification_for_add_op);
		}

		if (SON_PNULL != p_oam_update_notification_for_update_op)
		{
			son_mem_free(p_oam_update_notification_for_update_op);
		}

		if (SON_PNULL != p_rrm_update_notification)
		{
			son_mem_free(p_rrm_update_notification);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation Failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}
    */
    /*SPR 14925 Fix Ends */

	SON_MEMSET(p_oam_update_notification_for_add_op, 0, 
			sizeof(son_anr_updated_nrt_info_t));

	SON_MEMSET(p_oam_update_notification_for_update_op, 0, 
			sizeof(son_anr_updated_nrt_info_t));

	SON_MEMSET(p_rrm_req, 0, sizeof(son_rrm_meas_config_req_t));

	SON_MEMSET(p_rrm_update_notification, 0,
			sizeof(son_anr_rrm_updated_nrt_info_t));

	SON_MEMSET(&nrt_upd_ind, 0, sizeof(son_x2_nrt_update_ind_t));

	if (p_meas_results->eutran_meas_list_size)
	{
		ret  =   anr_process_eutran_meas_results(p_cell_ctxt,
				p_meas_results->eutran_meas_list_size,
				p_meas_results->eutran_meas_list,
                p_meas_results->meas_report_event,
				p_oam_update_notification_for_add_op,
				p_oam_update_notification_for_update_op,
				p_rrm_update_notification,
				p_rrm_req,
				ue_index); /*Last parameter ue_index added for SPR-9759*/
	}

	if (p_meas_results->utran_meas_list_size)
	{
		anr_process_utran_meas_results(p_cell_ctxt,
				p_meas_results->utran_meas_list_size,
				p_meas_results->utran_meas,
                p_meas_results->meas_report_event,
				p_oam_update_notification_for_add_op,
				p_oam_update_notification_for_update_op,
				p_rrm_update_notification,
				p_rrm_req,
				ue_index); /*Last parameter ue_index added for SPR-9759*/
	}

	if (p_meas_results->geran_meas_list_size)
	{
		anr_process_geran_meas_results(p_cell_ctxt,     
				p_meas_results->geran_meas_list_size,
				p_meas_results->geran_meas,
                p_meas_results->meas_report_event,
				p_oam_update_notification_for_add_op,
				p_oam_update_notification_for_update_op,
				p_rrm_update_notification,
				p_rrm_req,
				ue_index); /*Last parameter ue_index added for SPR-9759*/                          
	}

	if (p_meas_results->cdma_meas_list_size)
	{
		anr_process_cdma_meas_results(p_cell_ctxt,
				p_meas_results->cdma_meas_list_size,
				p_meas_results->cdma_meas,
                p_meas_results->meas_report_event,
				p_oam_update_notification_for_add_op,
				p_oam_update_notification_for_update_op,
				p_rrm_update_notification,
				p_rrm_req,
				ue_index); /*Last parameter ue_index added for SPR-9759*/
	}

	if (SON_TRUE == ret)
	{
		nrt_upd_ind.is_local_update = SON_FALSE;
		anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
				&p_cell_ctxt->cell_id,
				SON_NRT_OP_UPDATE);
	}

	if (SON_TRUE != p_cell_ctxt->limited_mode)
	{
		/* Check if meas config request is to be sent to RRM to fetch CGI and 
		 * other attributes of a neighbor cell with given PCI */
		if (0 < p_rrm_req->config_req.eutran_config_list_size 
				|| 0 < p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_fdd_list_size
				|| 0 < p_rrm_req->config_req.inter_rat_meas_config.utran_config_list.utran_tdd_list_size
				|| 0 < p_rrm_req->config_req.inter_rat_meas_config.geran_config_list.geran_freq_info_list_size
				|| 0 < p_rrm_req->config_req.inter_rat_meas_config.cdma_config_list.cdma_freq_info_list_size)
		{
			/* Store the generated transaction Id in the cell context 
			 * transaction Id list which will used to map the corresponding
			 * response messages */
			p_rrm_req->transaction_id = son_generate_txn_id();
			anr_insert_in_txn_id_list(&p_cell_ctxt->txn_id_list, p_rrm_req->transaction_id);

			SON_MEMCPY(&p_rrm_req->config_req.cell_id,
					&p_cell_ctxt->cell_id,
					sizeof(son_rrm_intra_rat_global_cell_id_t));
			p_rrm_req->config_req.ue_index = p_meas_results->ue_index; 
			p_rrm_req->config_req.bitmask |= RRM_SON_UE_INDEX_PRESENT;
			/*Invoke PUP tool call to send message to RRM*/
			if (RRM_FAILURE != rrm_son_send_rrm_son_meas_config_req(
						&p_rrm_req->config_req,
						p_cspl_hdr->to,
						SON_RRM_MODULE_ID,
						p_rrm_req->transaction_id,
						SON_NULL))
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_BRIEF,
						"Message %u send from %u "
						"to %u for cell [Cell Id: 0x%x]",
						RRM_SON_MEAS_CONFIG_REQ,
						SON_ANR_MODULE_ID,
						SON_RRM_MODULE_ID,
						son_convert_char_cell_id_to_int_cell_id((p_rrm_req->config_req.cell_id.
								cell_identity)));
			}
			else
			{
				anr_delete_from_txn_id_list(&p_cell_ctxt->txn_id_list, p_rrm_req->transaction_id);
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"Meas config request "
						"could not be sent to RRM for cell [Cell Id: 0x%x]",
						son_convert_char_cell_id_to_int_cell_id((p_rrm_req->config_req.cell_id.
								cell_identity)));
			}
		}
	}

	/* Send update notification to OAM with operation performed as ADD if add 
	 * operation is done in NRT */
	SON_MEMCPY(&p_oam_update_notification_for_add_op->src_cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMCPY(&p_oam_update_notification_for_update_op->src_cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	anr_send_updated_ncl_to_oam(p_oam_update_notification_for_add_op);
	anr_send_updated_ncl_to_oam(p_oam_update_notification_for_update_op);

    /* Spr 17753 Changes Start */
     anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
    /* Spr 17753 Changes End */
	/* Send update notification to RRM if intra RAT neighbor is added in NRT */
	SON_MEMCPY(&p_rrm_update_notification->src_cgi, 
			&p_cell_ctxt->cell_id,
			sizeof(son_rrm_intra_rat_global_cell_id_t));

	anr_send_updated_ncl_to_rrm(p_rrm_update_notification,
			son_convert_char_cell_id_to_int_cell_id((p_rrm_update_notification->src_cgi.
					cell_identity)));

	if (SON_PNULL !=  p_oam_update_notification_for_add_op)
	{
		son_mem_free(p_oam_update_notification_for_add_op);
	}

	if (SON_PNULL != p_oam_update_notification_for_update_op)
	{
		son_mem_free(p_oam_update_notification_for_update_op);
	}

	if (SON_PNULL != p_rrm_update_notification)
	{
		son_mem_free(p_rrm_update_notification);
	}

	if (SON_PNULL != p_rrm_req)
	{
		son_mem_free(p_rrm_req);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_ue_meas_results_ind */

/******************************************************************************
 * Function Name  : anr_fsm_modify_ue_cnt_req
 * Inputs         : p_cspl_hdr        - Pointer to CSPL header 
 *                  p_cell_context    - Pointer to ANR cell context
 *                  evnt_id           - API Id
 *                  p_msg             - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MODIFY_UE_COUNT_REQ
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_modify_ue_cnt_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_modify_ue_count_req_t *p_req =
		(son_anr_modify_ue_count_req_t *)(p_msg);
    /* SPR 20653 Fix Start */
    son_modify_attribute_t mod_attr; 
    /* coverity_112987 Start */
    SON_MEMSET(&mod_attr, 0, sizeof(son_modify_attribute_t));
    /* coverity_112987 Stop */
    /* SPR 20653 Fix End */


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}
    mod_attr.ue_cnt = p_req->modify_ue_cnt_req.ue_cnt;
    mod_attr.bitmask = SON_MODIFY_UE_COUNT_PRESENT; 
	anr_construct_set_attribute_req(p_req->transaction_id,
			&p_req->modify_ue_cnt_req.src_cgi,
            &mod_attr);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_modify_ue_cnt_req */

/******************************************************************************
 * Function Name  : anr_fsm_modify_removal_attributes_req
 * Inputs         : p_cspl_hdr       - Pointer to CSPL header 
 *                  p_cell_context   - Pointer to ANR cell context
 *                  evnt_id          - API Id
 *                  p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_modify_removal_attributes_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_modify_removal_attributes_req_t *p_req =
		(son_anr_modify_removal_attributes_req_t *)(p_msg);
	son_anr_modify_removal_attributes_res_t     res;
	son_u32 bitmask = 
		p_req->modify_removal_attributes_req.removal_criteria.bitmask;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&res, 0, sizeof(son_anr_modify_removal_attributes_res_t));

	/* Filling the response structure */
	res.transaction_id = p_req->transaction_id;

	SON_MEMCPY(&res.modify_removal_attributes_res.src_cgi,
			&p_req->modify_removal_attributes_req.src_cgi,
			sizeof(res.modify_removal_attributes_res.src_cgi));

	if (p_req->modify_removal_attributes_req.removal_criteria.bitmask)
	{
		res.modify_removal_attributes_res.error_code = SON_NO_ERROR;
		res.modify_removal_attributes_res.result = SON_SUCCESS;

		if (bitmask & SON_NO_ACTIVITY_INTERVAL_PRESENT)
		{
			p_cell_ctxt->removal_criteria.no_activity_interval = 
				p_req->modify_removal_attributes_req.removal_criteria.
				no_activity_interval;
			p_cell_ctxt->removal_criteria.bitmask |= SON_NO_ACTIVITY_INTERVAL_PRESENT;
		}

		if (bitmask & SON_FAILED_HO_THRESHOLD_PRESENT)
		{
			p_cell_ctxt->removal_criteria.handover_failure_threshold = 
				p_req->modify_removal_attributes_req.removal_criteria.
				handover_failure_threshold;
			p_cell_ctxt->removal_criteria.bitmask |= SON_FAILED_HO_THRESHOLD_PRESENT;
		}

		if (bitmask & SON_TIMEOUT_NO_NR_NEIGHBORS)
		{
			p_cell_ctxt->removal_criteria.timeout_no_nr_neighbors = 
				p_req->modify_removal_attributes_req.removal_criteria.
				timeout_no_nr_neighbors;
			p_cell_ctxt->removal_criteria.bitmask |= SON_TIMEOUT_NO_NR_NEIGHBORS;
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"None of the removal attributes are set "
				"for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_req->modify_removal_attributes_req.src_cgi.cell_identity));

		res.modify_removal_attributes_res.error_code = SON_ERR_INVALID_PARAMS;
		res.modify_removal_attributes_res.result = SON_FAILURE;
	}

	/* Coverity Fix: 19157, 19142 */
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES,
			sizeof(res));

	anr_print_cell_context(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_modify_removal_attributes_req */

/******************************************************************************
 * Function Name  : anr_fsm_modify_pci_confusion_cfg_req
 * Inputs         : p_cspl_hdr       - Pointer to CSPL header 
 *                  p_cell_context   - Pointer to ANR cell context
 *                  evnt_id          - API Id
 *                  p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MODIFY_PCI_CONFUSION_CFG_REQ
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_modify_pci_confusion_cfg_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_modify_pci_confusion_cfg_req_t  *p_req =
		(son_anr_modify_pci_confusion_cfg_req_t *)(p_msg);
	son_anr_modify_pci_confusion_cfg_res_t  res;
	son_u32 bitmask = p_req->modify_pci_confusion_cfg_req.
		pci_confusion_cfg.bitmask; 


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&res, 0, sizeof(son_anr_modify_pci_confusion_cfg_res_t));

	/* Filling the response structure */
	res.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&res.modify_pci_confusion_cfg_res.src_cgi,
			&p_req->modify_pci_confusion_cfg_req.src_cgi,
			sizeof(res.modify_pci_confusion_cfg_res.src_cgi));

	if (p_req->modify_pci_confusion_cfg_req.pci_confusion_cfg.bitmask)
	{
		res.modify_pci_confusion_cfg_res.error_code = SON_NO_ERROR;
		res.modify_pci_confusion_cfg_res.result = SON_SUCCESS;

		if (bitmask & SON_SUSPECT_PCI_THRESHOLD_PRESENT)
		{
			p_cell_ctxt->pci_confusion_cfg.suspect_pci_threshold = 
				p_req->modify_pci_confusion_cfg_req.pci_confusion_cfg.
				suspect_pci_threshold;
			p_cell_ctxt->pci_confusion_cfg.bitmask |= SON_SUSPECT_PCI_THRESHOLD_PRESENT;
		}

		if (bitmask & SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT)
		{
			p_cell_ctxt->pci_confusion_cfg.clear_suspect_pci_threshold = 
				p_req->modify_pci_confusion_cfg_req.pci_confusion_cfg.
				clear_suspect_pci_threshold;
			p_cell_ctxt->pci_confusion_cfg.bitmask |= SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT;
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"None of the removal attributes are set "
				"for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_req->modify_pci_confusion_cfg_req.src_cgi.cell_identity));

		res.modify_pci_confusion_cfg_res.error_code = SON_ERR_INVALID_PARAMS;
		res.modify_pci_confusion_cfg_res.result = SON_FAILURE;
	}

	/* Coverity Fix: 19157,19141 */
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_MODIFY_PCI_CONFUSION_CFG_RES,
			sizeof(res));

	anr_print_cell_context(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_modify_pci_confusion_cfg_req */

/******************************************************************************
 * Function Name  : anr_fsm_nack_modify_req
 * Inputs         : p_cspl_hdr       - Pointer to CSPL header
 *                  p_cell_context   - Pointer to ANR cell context
 *                  evnt_id          - API Id
 *                  p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MODIFY_UE_COUNT_REQ and 
 *                  SONANR_EVENT_MODIFY_MEAS_REPORTING_INTERVAL_REQ message.
 *                  It is invoked when message is received in unexpected 
 *                  ANR cell FSM state.
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_modify_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_generic_response_with_cell_id_t modify_resp;
	son_error_et error_code = SON_NO_ERROR;
	son_procedure_code_et api_id = SON_PROCEDURE_UNDEFINED;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ANR_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ANR_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ANR_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"Default Case Hit !!!");
				break;
			}
	} /* switch */

	modify_resp.transaction_id = *(son_u16 *)p_msg;
	if (IDX_SONANR_MODIFY_UE_COUNT_REQ == evnt_id)
	{
		SON_MEMCPY(&(modify_resp.generic_res.cell_id),
				&((son_anr_modify_ue_count_req_t *)p_msg)->
				modify_ue_cnt_req.src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
		api_id = SONANR_MODIFY_UE_COUNT_RES;
	}
	else if (IDX_SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ == evnt_id)
	{
		SON_MEMCPY(&(modify_resp.generic_res.cell_id),
				&((son_anr_modify_meas_reporting_interval_req_t *)p_msg)
				->modify_interval_req.src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
		api_id = SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES;
	}
	else if (IDX_SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ == evnt_id)
	{
		SON_MEMCPY(&(modify_resp.generic_res.cell_id),
				&((son_anr_modify_removal_attributes_req_t *)p_msg)
				->modify_removal_attributes_req.src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
		api_id = SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES;
	}
	else if (IDX_SONANR_MODIFY_PCI_CONFUSION_CFG_REQ == evnt_id)
	{
		SON_MEMCPY(&(modify_resp.generic_res.cell_id),
				&((son_anr_modify_pci_confusion_cfg_req_t *)p_msg)
				->modify_pci_confusion_cfg_req.src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
		api_id = SONANR_MODIFY_PCI_CONFUSION_CFG_RES;
	}
    /* SPR 20653 Fix Start */
	else if (IDX_SONANR_MODIFY_ATTRIBUTE_REQ == evnt_id)
	{
		SON_MEMCPY(&(modify_resp.generic_res.cell_id),
				&((son_anr_modify_attribute_req_t *)p_msg)
				->modify_attribute_req.src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
		api_id = SONANR_MODIFY_ATTRIBUTE_RES;
	}
    /* SPR 20653 Fix End */

	modify_resp.generic_res.result = SON_FAILURE;
	modify_resp.generic_res.error_code = error_code;

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	/* Coverity Fix: 19138 */
	son_create_send_buffer((son_u8 *)&modify_resp,
			SON_ANR_MODULE_ID, SON_MIF_MODULE_ID,
			api_id,
			sizeof(son_generic_response_with_cell_id_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_modify_req */

/******************************************************************************
 * Function Name  : anr_fsm_modify_meas_reporting_interval_req
 * Inputs         : p_cspl_hdr        - Pointer to CSPL header 
 *                  p_cell_context    - Pointer to ANR cell context
 *                  evnt_id           - API Id
 *                  p_msg             - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for 
 *                  SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_modify_meas_reporting_interval_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_modify_meas_reporting_interval_req_t *p_req =
		(son_anr_modify_meas_reporting_interval_req_t *)(p_msg);
    /* SPR 20653 Fix Start */
    son_modify_attribute_t mod_attr; 
    /* coverity_112986 Start */
    SON_MEMSET(&mod_attr, 0, sizeof(son_modify_attribute_t));
    /* coverity_112986 Stop */
    /* SPR 20653 Fix End */

	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

    mod_attr.reporting_interval = p_req->modify_interval_req.reporting_interval;
    mod_attr.bitmask = SON_MODIFY_REPORT_INTERVAL_PRESENT; 
	anr_construct_set_attribute_req(p_req->transaction_id,
			&p_req->modify_interval_req.src_cgi,
            &mod_attr);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_modify_meas_reporting_interval_req */

/* SPR 20653 Fix Start */
/******************************************************************************
 * Function Name  : anr_fsm_modify_attribute_req
 * Inputs         : p_cspl_hdr        - Pointer to CSPL header 
 *                  p_cell_context    - Pointer to ANR cell context
 *                  evnt_id           - API Id
 *                  p_msg             - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_MODIFY_ATTRIBUTE_REQ
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_modify_attribute_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_modify_attribute_req_t *p_req =
		(son_anr_modify_attribute_req_t *)(p_msg);
	son_u32 bitmask = p_req->modify_attribute_req.
		anr_attribute.bitmask; 
    son_modify_attribute_t mod_attr;
	son_anr_modify_attribute_res_t  res;

	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL evnt_id %u"
				,evnt_id);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&res, 0, sizeof(son_anr_modify_attribute_res_t));
/* coverity_112985 Start */
    SON_MEMSET(&mod_attr, 0, sizeof(son_modify_attribute_t));
/* coverity_112985 Stop */

	/* Filling the response structure */
	res.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&res.modify_attribute_res.src_cgi,
			&p_req->modify_attribute_req.src_cgi,
			sizeof(res.modify_attribute_res.src_cgi));

    mod_attr.ue_throughput_dl_anr_threshold = p_req->modify_attribute_req.anr_attribute.ue_throughput_dl_anr_threshold;
    mod_attr.bitmask |= SON_MODIFY_DL_THRESHOLD_PRESENT; 
    mod_attr.ue_throughput_ul_anr_threshold = p_req->modify_attribute_req.anr_attribute.ue_throughput_ul_anr_threshold;
    mod_attr.bitmask |= SON_MODIFY_UL_THRESHOLD_PRESENT; 
	if (bitmask)
	{
        if (bitmask & SON_MODIFY_DL_ANR_THRESHOLD_PRESENT)
		{
            anr_construct_set_attribute_req(p_req->transaction_id,
                    &p_req->modify_attribute_req.src_cgi,
                    &mod_attr);
		}
        if (bitmask & SON_MODIFY_UL_ANR_THRESHOLD_PRESENT)
		{
            anr_construct_set_attribute_req(p_req->transaction_id,
                    &p_req->modify_attribute_req.src_cgi,
                    &mod_attr);
		}
    }
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"None of the modify attributes are set "
				"for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_req->modify_attribute_req.src_cgi.cell_identity));

		res.modify_attribute_res.error_code = SON_ERR_INVALID_PARAMS;
		res.modify_attribute_res.result = SON_FAILURE;
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_MODIFY_ATTRIBUTE_RES,
			sizeof(res));
	}


	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_modify_attribute_req */
/* SPR 20653 Fix End */
/*****************************************************************************
 *  Function Name  : anr_fsm_nack_enable_disable_limited_mode_req
 *  Inputs         : p_cspl_hdr     - Pointer to cspl header
 *                   p_cell_ctxt    - Pointer to ANR cell context
 *                   evnt_id        - Event id
 *                   p_msg          - Pointer to the message
 *  Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 *  Description    : This function is invoked when 
 *                   SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ message is received 
 *                   in incorrect state.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_enable_disable_limited_mode_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_enable_disable_limited_mode_req_t    *p_req  =
		(son_anr_enable_disable_limited_mode_req_t *)(p_msg);
	son_anr_enable_disable_limited_mode_res_t                res;
	son_error_et error_code = SON_NO_ERROR;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&res, 0, sizeof(son_anr_enable_disable_limited_mode_res_t));

	SON_MEMCPY(&res.enable_disable_limited_mode_res.src_cgi,
			&p_req->enable_disable_limited_mode_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ANR_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ANR_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ANR_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"Default Case Hit !!!");
				break;
			}
	} /* switch */

	res.transaction_id = p_req->transaction_id;
	res.enable_disable_limited_mode_res.result  = SON_FAILURE;
	res.enable_disable_limited_mode_res.error_code = error_code;

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	/* Coverity Fix: 19157,19139 */
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_ENABLE_DISABLE_LIMITED_MODE_RES,
			sizeof(res));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_enable_disable_limited_mode_req */

/******************************************************************************
 * Function Name  : anr_fsm_enable_disable_limited_mode_req
 * Inputs         : p_cspl_hdr        Pointer to CSPL header 
 *                  p_cell_context    Pointer to ANR cell context
 *                  evnt_id           API Id
 *                  p_msg             Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_enable_disable_limited_mode_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_enable_disable_limited_mode_req_t  *p_req =
		(son_anr_enable_disable_limited_mode_req_t *)(p_msg);
	son_anr_enable_disable_limited_mode_res_t  res;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt or p_cspl_hdr is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&res, 0, sizeof(son_anr_enable_disable_limited_mode_res_t));

	/* Filling the response structure */
	res.transaction_id = p_req->transaction_id;
	res.enable_disable_limited_mode_res.error_code = SON_NO_ERROR;
	res.enable_disable_limited_mode_res.result = SON_SUCCESS;

	SON_MEMCPY(&res.enable_disable_limited_mode_res.src_cgi,
			&p_req->enable_disable_limited_mode_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	p_cell_ctxt->limited_mode = 
		p_req->enable_disable_limited_mode_req.limited_mode;

	/* Coverity Fix: 19139 */
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_ENABLE_DISABLE_LIMITED_MODE_RES,
			sizeof(res));

	anr_print_cell_context(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_enable_disable_limited_mode_req */

/*****************************************************************************
 * Function Name  : anr_process_rrm_register_deregister_res
 * Inputs         : msg_id  - API ID of response to be sent to SMIF
 *                  tid     - Transaction Id
 *                  p_res   - Pointer to register or de-register response
 *                :
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This is the function to handle register_res or deregister_res
 *
 ****************************************************************************/
/* Coverity Fix: 19155, 17265, 17263, 19158, 17258, 17260 */
static son_buf_retain_status_et
anr_process_rrm_register_deregister_res
(
 son_procedure_code_et msg_id,
 son_u16 tid,
 rrm_son_registration_based_res_t *p_res
 )
{
	son_anr_enable_res_t            res         = {0};
	son_u16                        pndg_rs_tid        = 0;
	son_bool_et                  send_res         = SON_TRUE;
	son_intra_rat_global_cell_id_t      *p_rcvd_cell_id = 
		(son_intra_rat_global_cell_id_t *)&p_res->id.object_id.cell_id;
	son_return_et                   result = SON_SUCCESS;
	son_error_et                    error_code = SON_NO_ERROR;
	rrm_son_message_resp_et         rrm_response_id = RRM_SON_RESP_MAX_API;
	son_u8                         pend_res_cnt = 0;
	son_x2_nrt_update_ind_t         nrt_upd_ind      = {0};
	son_anr_feature_state_change_ind_t state_change_ind;
	son_procedure_code_et api = anr_get_pending_res_api_id();


	SON_UT_TRACE_ENTER();

	if (p_res == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_res is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&state_change_ind, 0, sizeof(son_anr_feature_state_change_ind_t));

	if (SONANR_ENABLE_RES == msg_id)
	{
		rrm_response_id = RRM_SON_REGISTER_RESP;
	}
	else
	{
		rrm_response_id = RRM_SON_DEREGISTER_RESP;
	}

	result = ((p_res->result == RRM_SUCCESS) ? SON_SUCCESS : SON_FAILURE);
	if (SON_FAILURE == result)
	{
		error_code = anr_map_rrm_error_codes(__func__,
				result,
				p_res->error_code,
				rrm_response_id);
	}

	pndg_rs_tid = anr_get_pending_res_transaction_id();

	if (pndg_rs_tid != SON_UNDEFINED)
	{
		if (pndg_rs_tid != tid)
		{
			anr_delete_cell_from_pending_res(p_rcvd_cell_id);
		}
		else
		{
			if (SMIF_CELL_INFO_IND == api)
			{
				/* Copy the CGI */
				SON_MEMCPY(&state_change_ind.srv_cgi,
						p_rcvd_cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				/* Set the Module ID */
				state_change_ind.module_id = SON_ANR_MODULE_ID;
			}

			if (SON_SUCCESS == result)
			{
				if (SMIF_CELL_INFO_IND == api)
				{
					/* Set State to Enabled */
					state_change_ind.state = SON_STATE_ENABLED;

					/* Update cause on the basis of error code */
					state_change_ind.cause = SON_RRM_REGISTRATION_SUCCESSFUL;
				}

				anr_delete_cell_from_pending_res(p_rcvd_cell_id);
				if (SONANR_ENABLE_RES == msg_id &&
						SMIF_CELL_INFO_IND == anr_get_pending_res_api_id())
				{
					anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
							p_rcvd_cell_id, SON_NRT_OP_ADD);
				}
			}
			else
			{
				if (SMIF_CELL_INFO_IND == api)
				{
					/* Set State to Disabled */
					state_change_ind.state = SON_STATE_DISABLED;

					/* Update cause on the basis of error code */
					state_change_ind.cause = SON_RRM_REGISTRATION_UNSUCCESSFUL;

					anr_delete_cell_from_pending_res(p_rcvd_cell_id);
					if(SONANR_ENABLE_RES == msg_id &&
							!(RRM_ERR_OBJECT_NON_EXISTENT == p_res->error_code ||
								RRM_ERR_CELL_UNCONFIGURED == p_res->error_code))
					{
						anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
								p_rcvd_cell_id, SON_NRT_OP_ADD);
					}
				}
				else if (SON_TRUE == anr_update_cell_in_pending_res(p_rcvd_cell_id,
							error_code))
				{
					anr_decrement_pending_res_expected_count();
					anr_set_pending_res_error_rcvd(SON_TRUE);
				}
			}

			send_res = SON_FALSE;
			anr_set_pending_res_ts_with_current_timestamp();
		}

		if (!(pend_res_cnt = anr_get_pending_res_expected_count()) ||
				(pend_res_cnt &&
				 anr_get_pending_res_ts_diff_from_curr_ts() >
				 ANR_TIMEOUT_FOR_PENDING_RES))
		{
			anr_pending_res_timer_handler();
			anr_reset_pending_res();
			anr_set_context_state(ANR_STATE_ACTIVE);
		}
	}

	/*Coverity Fix: 17269 */
	if (RRM_FAILURE == p_res->result &&
			(RRM_ERR_OBJECT_NON_EXISTENT == p_res->error_code || 
			 RRM_ERR_CELL_UNCONFIGURED == p_res->error_code)
			&& msg_id == SONANR_ENABLE_RES)
	{
		if ((SMIF_CELL_INFO_IND == api) && (pndg_rs_tid == tid))
		{
			/* Set state to Deleted */
			state_change_ind.state = SON_STATE_DELETED;
			state_change_ind.cause = SON_RRM_CELL_UNCONFIGURED;        
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Registration failed, cell [Cell Id: 0x%x] deleted from ANR"
				" Cell Context List",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));

		anr_delete_cell_from_context(p_rcvd_cell_id);
	}

	if (send_res == SON_TRUE)
	{
		res.transaction_id = tid;
		if (error_code != SON_NO_ERROR)
		{
			res.enable_res.cell_status_list_size = 1;
			SON_MEMCPY(&res.enable_res.cell_status_list[0].cgi,
					p_rcvd_cell_id,
					sizeof(son_intra_rat_global_cell_id_t));
			res.enable_res.cell_status_list[0].error_code = error_code;
		}
		else
		{
			res.enable_res.cell_status_list[0].error_code =
				error_code;
		}

		res.enable_res.result = result;
		son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
				SON_MIF_MODULE_ID, msg_id,
				sizeof(res));
	}

	if (SON_ANR_MODULE_ID == state_change_ind.module_id)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"Sending cell state change indication for cell"
				"[Cell Id: %d]",
				son_convert_char_cell_id_to_int_cell_id(state_change_ind.srv_cgi.cell_identity));

		son_create_send_buffer((son_u8 *)&state_change_ind,
				SON_ANR_MODULE_ID,
				SON_MIF_MODULE_ID, 
				SONANR_FEATURE_STATE_CHANGE_IND,
				sizeof(state_change_ind));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_process_rrm_register_deregister_res */

/*****************************************************************************
 * Function Name  : anr_send_anr_disable_ind_to_mro_mlb
 * Inputs         : p_cell_ctxt   -  Pointer to ANR cell context
 * Outputs        : None
 * Returns        : son_void_t    -  None
 * Description    : This funciton is used to send SONANR_DISABLE_IND
 *                  to MRO and MLB whenever ANR mode is disabled for a cell at 
 *                  run time.
 ****************************************************************************/
static son_void_t
anr_send_anr_disable_ind_to_mro_mlb
(
 anr_cell_context_t *p_cell_ctxt
 )
{
	son_anr_cell_mode_ind_t anr_cell_mode_ind;
	son_size_t anr_cell_mode_ind_size = sizeof(son_anr_cell_mode_ind_t);


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(&anr_cell_mode_ind, 0, anr_cell_mode_ind_size);
	SON_MEMCPY(&anr_cell_mode_ind.srv_cgi, 
			&p_cell_ctxt->cell_id, 
			sizeof(son_intra_rat_global_cell_id_t));
	anr_cell_mode_ind.cell_mode = ANR_MODE_DISABLED;
#ifdef SON_MRO_FEATURE
	if (MRO_CELL_STATE_ENABLED == mro_get_cell_mode(&p_cell_ctxt->cell_id))
	{
		son_create_send_buffer((son_u8 *)&anr_cell_mode_ind, SON_ANR_MODULE_ID,
				SON_MRO_MODULE_ID, SONANR_DISABLE_IND,
				anr_cell_mode_ind_size);
	}

#endif
	if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->cell_id))
	{
		son_create_send_buffer((son_u8 *)&anr_cell_mode_ind, SON_ANR_MODULE_ID,
				SON_MLB_MODULE_ID, SONANR_DISABLE_IND,
				anr_cell_mode_ind_size);
	}
	if (RACH_OPT_CELL_STATE_ENABLED == rach_opt_get_cell_mode(
				&p_cell_ctxt->cell_id))
	{
		son_create_send_buffer((son_u8 *)&anr_cell_mode_ind, SON_ANR_MODULE_ID,
				SON_RACH_OPT_MODULE_ID, SONANR_DISABLE_IND,
				anr_cell_mode_ind_size);
	}

	SON_UT_TRACE_EXIT();
} /* anr_send_anr_disable_ind_to_mro_mlb */

/*****************************************************************************
 * Function Name  : anr_fsm_register_res
 * Inputs         : p_cspl_hdr     - Pointer to cspl header
 *                  p_cell_ctxt    - Pointer to ANR cell context
 *                : evnt_id        - Event id
 *                : p_msg          - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for RRM_REGISTER_RES.It is invoked when
 *                : anr state for a cell is ANR_CELL_STATE_ENABLING.
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_register_res
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_rrm_registration_res_t     *p_res     = (son_rrm_registration_res_t *)p_msg;
	son_buf_retain_status_et     ret_val = RELEASE_SON_BUFFER;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	/* Coverity Fix: 17264 */
	if (p_res->res.registration_resp.result == RRM_SUCCESS)
	{
		SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ANR_CELL_STATE_ENABLED);
		/* Rel 3.0 Multi Sector code changes Start */
		if (SON_TRUE ==
				anr_get_auto_maintain_local_cell_as_nbr_status())
		{
			anr_add_other_serving_cells_as_nbr_autonomously(
					p_cell_ctxt);
			anr_print_cell_context(p_cell_ctxt);
		}
		/* Rel 3.0 Multi Sector code changes Stop */
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Error: %d: Registration "
				"with RRM failed for cell [Cell Id: 0x%x].",
				p_res->res.registration_resp.error_code,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));

		SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ANR_CELL_STATE_DISABLED);
	}

	ret_val = anr_process_rrm_register_deregister_res(SONANR_ENABLE_RES,
			p_res->transaction_id,
			&p_res->res.registration_resp);
	SON_UT_TRACE_EXIT();

	return ret_val;
} /* anr_fsm_register_res */

/*****************************************************************************
 * Function Name  : anr_fsm_deregister_res
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for the RRM_DEREGISTER_RES. It is invoked
 *                  when the anr state of the cell is ANR_CELL_STATE_DISABLING.
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_deregister_res
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_rrm_deregistration_res_t     *p_res     = 
		(son_rrm_deregistration_res_t *)p_msg;
	son_buf_retain_status_et         ret_val = RELEASE_SON_BUFFER;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL evnt-id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	/* Coverity Fix: 17259 */
	if (p_res->res.deregistration_resp.result == RRM_SUCCESS)
	{
		SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ANR_CELL_STATE_DISABLED);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Error: %d: DeRegistration "
				"with RRM failed for cell [Cell Id: 0x%x].",
				p_res->res.deregistration_resp.error_code,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));

		anr_cell_fsm_state_et new_state = ANR_CELL_STATE_DISABLED;
		if (ANR_CELL_STATE_ENABLED == p_cell_ctxt->previous_cell_fsm_state)
		{
			new_state = ANR_CELL_STATE_ENABLED;
		}

		SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				new_state);
	}

	/* Indicate MRO and MLB when ANR mode is disabled at run time */
	if (ANR_CELL_STATE_DISABLED == p_cell_ctxt->current_cell_fsm_state)
	{
		anr_send_anr_disable_ind_to_mro_mlb(p_cell_ctxt);
	}

	ret_val = anr_process_rrm_register_deregister_res(SONANR_DISABLE_RES,
			p_res->transaction_id,
			&p_res->res.deregistration_resp);

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* anr_fsm_deregister_res */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_anr_enable_req
 * Inputs         : p_cspl_hdr       - Pointer to cs;pl header
 *                  p_cell_ctxt      - Pointer to ANR cell context
 *                : evnt_id          - Event id
 *                : p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    :  This function is negative event handler for
 *                  SONANR_ENABLE_REQ
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_anr_enable_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_enable_res_t  res = {0};
	son_anr_enable_req_t  *p_req = (son_anr_enable_req_t  *)p_msg;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	res.enable_res.cell_status_list_size = 1;
	res.enable_res.result = SON_FAILURE;

	SON_MEMCPY(&res.enable_res.cell_status_list[0].cgi,
			&p_req->enable_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	res.enable_res.cell_status_list[0].error_code = SON_ERR_ALREADY_ENABLED;

	if (ANR_CELL_STATE_ENABLING == p_cell_ctxt->current_cell_fsm_state)
	{
		res.enable_res.cell_status_list[0].error_code = 
			SON_ERR_ENABLE_IN_PROGRESS;
	}

	res.transaction_id = p_req->transaction_id;

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, 
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	/* Coverity Fix: 19157, 19129 */
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_ENABLE_RES,
			sizeof(res));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_anr_enable_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_anr_disable_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This is a negative event handler for
 *                  SONANR_DISABLE_REQ
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_anr_disable_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_disable_res_t  res = {0};
	son_anr_disable_req_t  *p_req = (son_anr_disable_req_t *)p_msg;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	res.disable_res.cell_status_list_size = 1;
	res.disable_res.result = SON_FAILURE;

	SON_MEMCPY(&res.disable_res.cell_status_list[0].cgi,
			&p_req->disable_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	res.disable_res.cell_status_list[0].error_code = SON_ERR_ALREADY_DISABLED;

	if (ANR_CELL_STATE_DISABLING == p_cell_ctxt->current_cell_fsm_state)
	{
		res.disable_res.cell_status_list[0].error_code = 
			SON_ERR_DISABLE_IN_PROGRESS;
	}

	res.transaction_id = p_req->transaction_id;

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]."
			"Received message %d, from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	/* Coverity Fix: 19128,19157 */
	son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_DISABLE_RES,
			sizeof(res));
	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_anr_disable_req */

/*****************************************************************************
 * Function Name  : anr_fsm_anr_disable_req
 * Inputs         : p_cspl_hdr   - Pointer to cspl header
 *                  p_cell_ctxt  - Pointer to ANR cell context
 *                  evnt_id      - Event id
 *                : p_msg        - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler for SONANR_DISABLE_REQ.It is invoked when
 *                : the anr state of a cell is ANR_CELL_STATE_ENABLED and
 *                  ANR_CELL_STATE_ENABLING
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_anr_disable_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_disable_req_t   *p_req      = (son_anr_disable_req_t *)(p_msg);


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL evnt_id: [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	if (SON_SUCCESS == anr_construct_send_rrm_deregister_req(
				p_req->transaction_id,
				&p_cell_ctxt->cell_id ))
	{
		SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ANR_CELL_STATE_DISABLING);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"DeRegister Request could not be sent"
				" for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* anr_fsm_anr_disable_req */

/*****************************************************************************
 * Function Name  : anr_process_intra_rat_neighbors
 * Inputs         : p_cell_ctxt            - Pointer to Cell Context
 *                : intra_rat_nr_list_size - Size of Intra Rat Neighbor List
 *                : p_intra_rat_nr_list    - Pointer to Intra Rat Neighbor List
 *                : p_trgt_cell_status_list_size -  Pointer to size of add_upd_status List
 *                : p_trgt_cell_status_list      -   Pointer to add_upd_status List
 *                : p_notification         - Pointer to notification message
 *                : p_fun                  - Pointer to the insert/modify function
 *                : evnt_id                - Event name
 * Outputs        : 
 * Returns        : son_return_et          - Returns SON_TRUE on success or SON_FAILURE
 * Description    : This function processes the Intra-RAT Neighbors
 ******************************************************************************/
static
son_return_et
anr_process_intra_rat_neighbors
(
 anr_cell_context_t        	      *p_cell_ctxt,
 son_u8                            intra_rat_nr_list_size,
 son_intra_rat_neighbor_info_t 	  *p_intra_rat_nr_list,
 son_u16                           *p_trgt_cell_status_list_size,
 son_nbr_cell_status_t	          *p_trgt_cell_status_list,
 son_anr_updated_nrt_info_t        *p_oam_notif_upd,
 son_anr_rrm_updated_nrt_info_t    *p_notification,
 anr_intra_rat_hash_rec_t *(*p_fun )
 (
  anr_cell_context_t *,
  anr_intra_rat_hash_rec_t *,
  son_bool_et,
  son_error_et *,
  son_anr_updated_nrt_info_t *,
  son_anr_updated_nrt_info_t *,
  son_anr_rrm_updated_nrt_info_t *,
  son_bool_et,
  son_bool_et
 ),
 son_anr_index_et    evnt_id
)
{
	son_u16                         idx                     = 0;
	anr_intra_rat_hash_rec_t        intra_rat_hash_rec;
	anr_intra_rat_hash_rec_t        *p_inserted_modified_nr = SON_PNULL;
	son_error_et                    error_code              = SON_NO_ERROR;
	son_bool_et                     is_upd_ind_rqd  = SON_FALSE;
	son_global_cell_id_t            cell_id;
	son_x2_nrt_update_ind_t         nrt_upd_ind      = {0};   


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_intra_rat_nr_list == SON_PNULL || 
			p_trgt_cell_status_list_size == SON_PNULL || 
			p_trgt_cell_status_list == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_intra_rat_nr_list, p_trgt_cell_status_list_size or p_trgt_cell_status_list is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	/* Processing of Inra RAT Neighbors */
	for (idx = 0; idx < intra_rat_nr_list_size; idx++)
	{
		SON_MEMSET(&cell_id, 0, sizeof(cell_id));
		SON_MEMSET(&intra_rat_hash_rec, 0, sizeof(intra_rat_hash_rec));
		list_init(&intra_rat_hash_rec.anr_nr_nbr_list);
		p_inserted_modified_nr = SON_PNULL;

		anr_convert_intra_rat_nr_to_intra_rat_hash_rec(&p_intra_rat_nr_list[idx],
				&intra_rat_hash_rec);
		anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor(&intra_rat_hash_rec);

		if (SON_ENABLE_X2_STATUS_PRESENT & intra_rat_hash_rec.neighbor_info.bitmask)
		{
			error_code = SON_ERR_INVALID_PARAMS; 
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
					"Intra RAT Neighbor cell [Cell Id: 0x%x]"
					"is containing X2 status field but it is READ ONLY for OAM",
					son_convert_char_cell_id_to_int_cell_id(intra_rat_hash_rec.neighbor_info.nbr_cgi.cell_identity));
		}
		else
		{
			error_code = SON_NO_ERROR; 
			p_inserted_modified_nr = p_fun(p_cell_ctxt,
					&intra_rat_hash_rec, SON_FALSE, &error_code,
					SON_PNULL, p_oam_notif_upd, p_notification, SON_TRUE, SON_TRUE);
		}

		if (SON_PNULL != p_inserted_modified_nr)
		{
			if (IDX_SONANR_NR_ADD_REQ == evnt_id)
			{
				is_upd_ind_rqd = SON_TRUE;
		    /* Spr 17753 Changes Start */
		    anr_check_and_insert_second_degree_nbr_data(p_inserted_modified_nr,p_cell_ctxt);
		    /* Spr 17753 Changes End */
			}

			if (IDX_SONANR_NR_UPDATE_REQ == evnt_id &&
					((intra_rat_hash_rec.neighbor_info.earfcn != p_inserted_modified_nr->neighbor_info.earfcn) ||
					 (intra_rat_hash_rec.neighbor_info.pci != p_inserted_modified_nr->neighbor_info.pci) ||
					 /*SPR 16523 fix start*/
					 (intra_rat_hash_rec.neighbor_info.tac != p_inserted_modified_nr->neighbor_info.tac ) ))
				/*SPR 16523 fix stop*/
			{
				is_upd_ind_rqd = SON_TRUE;
			}
		}   
		else
		{
			/*if old and new NR are not same in value.
			 * This is the error code retured during modification, when there is
			 * no update effect on the old NR*/
			if (SON_NO_ERROR != error_code)
			{
				p_trgt_cell_status_list[*p_trgt_cell_status_list_size].cgi.bitmask
					|= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
				p_trgt_cell_status_list[*p_trgt_cell_status_list_size].error_code = error_code;

				SON_MEMCPY( &p_trgt_cell_status_list[(*p_trgt_cell_status_list_size)++].cgi.
						intra_rat_global_cell_id,
						&intra_rat_hash_rec.neighbor_info.nbr_cgi,
						sizeof(son_intra_rat_global_cell_id_t));
			}
		}
	}

	if (SON_TRUE == is_upd_ind_rqd)
	{
		/*ENB CONFIG CHANGE REQ SPR:START*/
		nrt_upd_ind.is_local_update = SON_FALSE;
		anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
				&p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
		/*ENB CONFIG CHANGE REQ SPR:START*/
	}

	if (*p_trgt_cell_status_list_size)
	{
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* anr_process_intra_rat_neighbors */

/*****************************************************************************
 * Function Name  : anr_process_inter_rat_neighbors
 * Inputs         : p_cell_ctxt            - Pointer to Cell Context
 *                : inter_rat_nr_list_size - Size of Intra Rat Neighbor List
 *                : p_inter_rat_nr_list    - Pointer to Inter Rat Neighbor List
 *                : p_trgt_cell_status_list_size - Pointer to size of add_upd_status List
 *                : p_trgt_cell_status_list      - Pointer to add_upd_status List
 *                : p_notification         - Pointer to notification message
 *                : p_fun                  - Pointer to the insert/modify function
 *                : evnt_id                - Event name 
 * Outputs        :
 * Returns        : son_return_et          - Returns SON_TRUE on success or SON_FAILURE 
 * Description    : This function processes the Inter-RAT Neighbors
 ******************************************************************************/
static
son_return_et
anr_process_inter_rat_neighbors
(
 anr_cell_context_t              *p_cell_ctxt,
 son_u8                          inter_rat_nr_list_size,
 son_inter_rat_neighbor_info_t   *p_inter_rat_nr_list,
 son_u16                         *p_trgt_cell_status_list_size,
 son_nbr_cell_status_t            *p_trgt_cell_status_list,
 son_anr_rrm_updated_nrt_info_t  *p_notification,
 anr_inter_rat_hash_rec_t *(*p_fun )
 (
  anr_cell_context_t *,
  anr_inter_rat_hash_rec_t *,
  son_bool_et,
  son_error_et *,
  son_anr_updated_nrt_info_t *,
  son_anr_rrm_updated_nrt_info_t *
 ),
 son_anr_index_et    evnt_id
 )
{
	son_u16                         idx                     = 0;
	anr_inter_rat_hash_rec_t        nr_to_be_inserted_modified;
	son_error_et                    error_code              = SON_NO_ERROR;
	son_global_cell_id_t            cell_id;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_inter_rat_nr_list == SON_PNULL || 
			p_trgt_cell_status_list_size == SON_PNULL || 
			p_trgt_cell_status_list == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_inter_rat_nr_list, p_trgt_cell_status_list_size"
                "or p_trgt_cell_status_list is NULL evnt_id [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_MEMSET(&nr_to_be_inserted_modified, 0, sizeof(anr_inter_rat_hash_rec_t));

	/* Processing of Inter RAT Neighbors */
	for (idx = 0; idx < inter_rat_nr_list_size; idx++)
	{
		error_code = SON_NO_ERROR;
		SON_MEMSET(&cell_id, 0, sizeof(cell_id));
		SON_MEMSET(&nr_to_be_inserted_modified, 0, sizeof(nr_to_be_inserted_modified));
		anr_convert_inter_rat_nr_to_inter_rat_hash_rec(&p_inter_rat_nr_list[idx],
				&nr_to_be_inserted_modified);

		/*Removal of p_inserted_modified_nr as coverity fix*/
		p_fun(p_cell_ctxt,
				&nr_to_be_inserted_modified, SON_FALSE, &error_code, SON_PNULL, p_notification);

		/*This is the error code returned
		 * when no updation is required*/
		if (SON_NO_ERROR != error_code)
		{
			p_trgt_cell_status_list[(*p_trgt_cell_status_list_size)].error_code = error_code;
			anr_extract_global_cell_id_from_inter_rat_hash_rec(
					&nr_to_be_inserted_modified,
					&p_trgt_cell_status_list[(*p_trgt_cell_status_list_size)++].cgi);
		}
	}

	if (*p_trgt_cell_status_list_size)
	{
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* anr_process_inter_rat_neighbors */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_nr_add_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_ADD_REQ message
 *                  is received in incorrect state.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_nr_add_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_add_req_t                *p_add_req  =
		(son_anr_nr_add_req_t *)(p_msg);
	son_anr_nr_add_res_t                *p_res = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
        /* SPR 15935 Fix Start */
        /*Code deleted*/
        /* SPR 15935 Fix Stop */
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

    /* Coverity ID 72556 Starts */
	p_res = (son_anr_nr_add_res_t *)son_mem_get(sizeof(son_anr_nr_add_res_t));
    /* Coverity ID 72556 Ends */
	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Response mem allocation failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_add_res_t));

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	p_res->transaction_id = p_add_req->transaction_id;
	p_res->add_res.result  = SON_FAILURE;
	p_res->add_res.error_code = SON_ERR_UNEXPECTED_MSG;
	SON_MEMCPY(&p_res->add_res.src_cgi,
			&p_add_req->add_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Coverity Fix: 19143,19979 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_ADD_RES,
			sizeof(son_anr_nr_add_res_t));

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_nr_add_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nr_add_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_ADD_REQ message
 *                  is received.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nr_add_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_add_req_t                 *p_add_req     =
		(son_anr_nr_add_req_t *)(p_msg);
	son_anr_nr_add_res_t                  *p_res = SON_PNULL;
	son_anr_rrm_updated_nrt_info_t      *p_notification_msg = SON_PNULL;
	/* Coverity Fix: 11372 */
	son_anr_updated_nrt_info_t          *p_oam_notif_upd = SON_PNULL;
	son_oam_log_on_off_et               log_mode        = anr_get_log_mode();
	son_return_et                       result = SON_SUCCESS;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_add_res_t *)son_mem_get(sizeof(son_anr_nr_add_res_t));
	if (SON_PNULL == p_res)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_notification_msg)
	{
		SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
				"Memory allocation failure" );
		if (p_res != SON_PNULL)
		{
			son_mem_free(p_res);
		}

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_oam_notif_upd = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_oam_notif_upd)
	{
		SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
				"Memory allocation failure" );
		if (p_res != SON_PNULL)
		{
			son_mem_free(p_res);
		}

		if (p_notification_msg != SON_PNULL)
		{
			son_mem_free(p_notification_msg);
		}

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_add_res_t));
	p_res->transaction_id = p_add_req->transaction_id;
	p_res->add_res.error_code = SON_NO_ERROR;

	SON_MEMCPY(&p_res->add_res.src_cgi,
			&p_add_req->add_req.src_cgi, sizeof(son_intra_rat_global_cell_id_t));

	if (0 == p_add_req->add_req.nr_list.intra_rat_nr_list_size &&
			0 == p_add_req->add_req.nr_list.inter_rat_nr_list_size)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"Empty Neighbors List "
				"NR_list_size is 0 for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_add_req->add_req.src_cgi.cell_identity));

		p_res->add_res.result = SON_FAILURE;
		p_res->add_res.error_code = SON_ERR_INVALID_PARAMS;
	}
	else
	{
		SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
		/* Coverity Fix: 11372 */
		SON_MEMSET(p_oam_notif_upd, 0, sizeof(son_anr_updated_nrt_info_t));

		SON_MEMCPY(&p_notification_msg->src_cgi, &p_add_req->add_req.src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));

		/* Coverity Fix: 11372 */
		SON_MEMCPY(&p_oam_notif_upd->src_cgi,
				&p_add_req->add_req.src_cgi, sizeof(son_intra_rat_global_cell_id_t));

		if (p_add_req->add_req.nr_list.intra_rat_nr_list_size)
		{   
			if (SON_SUCCESS != anr_process_intra_rat_neighbors( p_cell_ctxt,
						p_add_req->add_req.nr_list.intra_rat_nr_list_size,
						p_add_req->add_req.nr_list.intra_rat_nr_list,
						&p_res->add_res.trgt_cell_status_list_size,
						p_res->add_res.trgt_cell_status_list,
						p_oam_notif_upd,
						p_notification_msg,
						anr_intra_rat_insert_nr,
						evnt_id))
			{
				result = SON_FAILURE;
			}
		}

		if (p_add_req->add_req.nr_list.inter_rat_nr_list_size)
		{
			if (SON_SUCCESS != anr_process_inter_rat_neighbors( p_cell_ctxt,
						p_add_req->add_req.nr_list.inter_rat_nr_list_size,
						p_add_req->add_req.nr_list.inter_rat_nr_list,
						&p_res->add_res.trgt_cell_status_list_size,
						p_res->add_res.trgt_cell_status_list,
						p_notification_msg,
						anr_inter_rat_insert_nr,
						evnt_id))
			{
				result = SON_FAILURE;
			}
		}

		p_res->add_res.result = result;      

		anr_send_updated_ncl_to_rrm(p_notification_msg,
				son_convert_char_cell_id_to_int_cell_id(p_add_req->add_req.src_cgi.cell_identity));
    /* Spr 17753 Changes Start */
        anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
    /* Spr 17753 Changes End */
	}

	anr_send_updated_ncl_to_oam(p_oam_notif_upd);

	if (p_res->add_res.trgt_cell_status_list_size &&
			p_res->add_res.trgt_cell_status_list_size < (
				p_add_req->add_req.nr_list.inter_rat_nr_list_size +
				p_add_req->add_req.nr_list.intra_rat_nr_list_size))
	{
		p_res->add_res.result = SON_PARTIAL_SUCCESS;
	}

	/* Coverity Fix: 19979,19143 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_ADD_RES,
			sizeof(son_anr_nr_add_res_t));

	if (SON_PNULL != p_notification_msg)
	{
		son_mem_free(p_notification_msg);
	}

	if (SON_PNULL != p_oam_notif_upd)
	{
		son_mem_free(p_oam_notif_upd);
	}

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	anr_print_cell_context(p_cell_ctxt);
	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nr_add_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_nr_delete_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_DELETE_REQ message
 *                  is received in incorrect state.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_nr_delete_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_delete_req_t                *p_req  =
		(son_anr_nr_delete_req_t *)(p_msg);
	son_anr_nr_delete_res_t                *p_res  = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL ||
			p_cspl_hdr  == SON_PNULL ||
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_delete_res_t *)son_mem_get(sizeof(son_anr_nr_delete_res_t));
	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation failure");

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_delete_res_t));

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	p_res->transaction_id = p_req->transaction_id;
	p_res->del_res.result  = SON_FAILURE;
	p_res->del_res.error_code = SON_ERR_UNEXPECTED_MSG;

	SON_MEMCPY(&p_res->del_res.src_cgi,
			&p_req->del_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Coverity Fix: 19145 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_DELETE_RES,
			sizeof(son_anr_nr_delete_res_t));

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_nr_delete_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nr_delete_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg          Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_DELETE_REQ message
 *                  is received.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nr_delete_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_u8                            idx                    = 0;
	son_return_et                     ret                 = SON_SUCCESS;
	son_error_et                     error                 = SON_NO_ERROR;
	son_anr_nr_delete_req_t         *p_delete_req =
		(son_anr_nr_delete_req_t *)(p_msg);
	son_anr_rrm_updated_nrt_info_t  *p_notification_msg = SON_PNULL;
	son_anr_updated_nrt_info_t      *p_delete_notif     = SON_PNULL;
	son_anr_nr_delete_res_t          *p_res = SON_PNULL;
	son_global_cell_id_t            *p_global_cell_id      = SON_PNULL;
	anr_intra_rat_hash_rec_t        intra_rat_hash_rec;
	anr_inter_rat_hash_rec_t        inter_rat_hash_rec;
	son_void_t                      *p_hash_rec = SON_PNULL;
	son_x2_nrt_update_ind_t         nrt_upd_ind      = {0};
	son_bool_et                     is_upd_ind_rqd = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL evnt_id [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res  = (son_anr_nr_delete_res_t *)
		son_mem_get(sizeof(son_anr_nr_delete_res_t));

	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_notification_msg)
	{
		if (SON_PNULL != p_res)
		{
			son_mem_free(p_res);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_delete_notif = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));    

	if (SON_PNULL == p_delete_notif)
	{
		if (SON_PNULL != p_res)
		{
			son_mem_free(p_res);
		}

		if (SON_PNULL != p_notification_msg)
		{
			son_mem_free(p_notification_msg);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_delete_res_t));
	SON_MEMSET(&intra_rat_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
	SON_MEMSET(&inter_rat_hash_rec, 0, sizeof(anr_inter_rat_hash_rec_t));

	/* Filling the response structure */
	p_res->transaction_id = p_delete_req->transaction_id;
	p_res->del_res.error_code = SON_NO_ERROR;
	p_res->del_res.result = SON_SUCCESS;

	SON_MEMCPY(&p_res->del_res.src_cgi,
			&p_delete_req->del_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
	SON_MEMSET(p_delete_notif, 0, sizeof(son_anr_updated_nrt_info_t));

	SON_MEMCPY(&p_notification_msg->src_cgi,
			&p_delete_req->del_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMCPY(&p_delete_notif->src_cgi,
			&p_delete_req->del_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	if (0 == p_delete_req->del_req.trgt_cgi_list_size)
	{
		p_res->del_res.error_code = SON_ERR_INVALID_PARAMS;
		p_res->del_res.result = SON_FAILURE;
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Empty Neighbors List - Target List size is 0");
	}
	else
	{
		for (idx = 0; idx < p_delete_req->del_req.trgt_cgi_list_size; idx++)
		{
			p_global_cell_id = &p_delete_req->del_req.trgt_cgi_list[idx];
			p_hash_rec       = SON_PNULL;

			if (SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT & p_global_cell_id->bitmask)
			{
				p_hash_rec = &intra_rat_hash_rec;
			}
			else if (SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT & p_global_cell_id->bitmask)
			{
				p_hash_rec = &inter_rat_hash_rec;
			}

			ret = anr_del_nr(p_cell_ctxt,
					p_global_cell_id,
					SON_FALSE,
					&error,
					p_hash_rec,
					p_delete_notif,
					p_notification_msg);

			if (SON_FAILURE == ret)
			{ 
				p_res->del_res.result = SON_FAILURE;
				p_res->del_res.trgt_cell_status_list[p_res->del_res.trgt_cell_status_list_size].error_code = error;
				SON_MEMCPY(&p_res->del_res.trgt_cell_status_list
						[p_res->del_res.trgt_cell_status_list_size++].cgi,
						p_global_cell_id, sizeof(son_global_cell_id_t));
			}
			else
			{
				if (SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT ==  p_global_cell_id->bitmask)
				{
					is_upd_ind_rqd = SON_TRUE;
				}
			}
		}

		if (p_res->del_res.trgt_cell_status_list_size &&
				(p_res->del_res.trgt_cell_status_list_size <
				 p_delete_req->del_req.trgt_cgi_list_size))
		{
			p_res->del_res.result = SON_PARTIAL_SUCCESS;
		}

		anr_send_updated_ncl_to_rrm(p_notification_msg,
				son_convert_char_cell_id_to_int_cell_id(p_delete_req->del_req.src_cgi.cell_identity));
    /* Spr 17753 Changes Start */
        anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
    /* Spr 17753 Changes End */
	}

	/* Coverity Fix: 19145 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_DELETE_RES,
			sizeof(son_anr_nr_delete_res_t));

	/* Send updated NRT info to MLB for neighbor deletion */
	if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->cell_id))
	{
		anr_send_updated_ncl_to_mlb(p_delete_notif);
	}

	if (SON_TRUE == is_upd_ind_rqd)
	{  
		nrt_upd_ind.is_local_update = SON_FALSE;
		anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
				&p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
	}

	anr_print_cell_context(p_cell_ctxt);

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	if (SON_PNULL != p_notification_msg)
	{
		son_mem_free(p_notification_msg);
	}

	if (SON_PNULL != p_delete_notif)
	{
		son_mem_free(p_delete_notif);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nr_delete_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_nr_delete_from_remove_list_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                  p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when 
 *                   SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ message is received 
 *                   in incorrect state.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_nr_delete_from_remove_list_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_delete_from_remove_list_req_t    *p_req  =
		(son_anr_nr_delete_from_remove_list_req_t *)(p_msg);
	son_anr_nr_delete_from_remove_list_res_t                *p_res = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_delete_from_remove_list_res_t * )
		son_mem_get(sizeof(son_anr_nr_delete_from_remove_list_res_t));

	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_delete_from_remove_list_res_t));

	SON_MEMCPY(&p_res->delete_from_remove_list_res.src_cgi,
			&p_req->delete_from_remove_list_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ANR_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ANR_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ANR_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		default:
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
						SON_ERROR,
						"Default Case Hit !!!");
				break;
			}
	} /* switch */

	p_res->transaction_id = p_req->transaction_id;
	p_res->delete_from_remove_list_res.result  = SON_FAILURE;
	p_res->delete_from_remove_list_res.error_code = error_code;

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	/* Coverity Fix: 19144 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_DELETE_FROM_REMOVE_LIST_RES,
			sizeof(son_anr_nr_delete_from_remove_list_res_t));

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_nr_delete_from_remove_list_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nr_delete_from_remove_list_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                  p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 *  Description    : This function is invoked when 
 *                   SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ message is received.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nr_delete_from_remove_list_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_u8                                      idx             = 0;
	son_anr_nr_delete_from_remove_list_req_t    *p_delete_req   =
		(son_anr_nr_delete_from_remove_list_req_t *)(p_msg);
	son_anr_nr_delete_from_remove_list_res_t    *p_res = SON_PNULL;
	son_global_cell_id_t            *p_global_cell_id           = SON_PNULL;
	son_bool_et                      ret_val                    = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL evnt_id [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_delete_from_remove_list_res_t *)
		son_mem_get(sizeof(son_anr_nr_delete_from_remove_list_res_t));

	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Mem Allocaton failure");

		SON_UT_TRACE_EXIT(); 
		return RELEASE_SON_BUFFER; 
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_delete_from_remove_list_res_t));

	/* Filling the response structure */
	p_res->transaction_id = p_delete_req->transaction_id;
	p_res->delete_from_remove_list_res.error_code = SON_NO_ERROR;
	p_res->delete_from_remove_list_res.result = SON_SUCCESS;

	SON_MEMCPY(&p_res->delete_from_remove_list_res.src_cgi,
			&p_delete_req->delete_from_remove_list_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	if (0 == p_delete_req->delete_from_remove_list_req.trgt_cgi_list_size)
	{
		p_res->delete_from_remove_list_res.error_code = SON_ERR_INVALID_PARAMS;
		p_res->delete_from_remove_list_res.result = SON_FAILURE;
	}

	for (idx = 0; idx < p_delete_req->delete_from_remove_list_req.trgt_cgi_list_size; idx++)
	{
		p_global_cell_id = &p_delete_req->delete_from_remove_list_req.trgt_cgi_list[idx];

		ret_val = anr_delete_cell_from_remove_list(
				&p_cell_ctxt->nr_remove_list,
				p_global_cell_id);

		if (SON_TRUE == ret_val)
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF,
					"NR deleted from Remove List"
					" for Cell [Cell Id: 0x%x]",
					son_convert_char_cell_id_to_int_cell_id(p_delete_req->
						delete_from_remove_list_req.src_cgi.cell_identity));
		}
		else
		{
			p_res->delete_from_remove_list_res.result = SON_FAILURE;
			p_res->delete_from_remove_list_res.error_code = SON_ERR_ANR_NR_NOT_FOUND;

			p_res->delete_from_remove_list_res.trgt_cell_status_list
				[p_res->delete_from_remove_list_res.trgt_cell_status_list_size].error_code
				= SON_ERR_ANR_NR_NOT_FOUND;

			SON_MEMCPY(&p_res->delete_from_remove_list_res.trgt_cell_status_list
					[p_res->delete_from_remove_list_res.trgt_cell_status_list_size++].cgi,
					p_global_cell_id, sizeof(son_global_cell_id_t));

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF,
					"NR delete from Remove List"
					"failed for Cell [Cell Id: 0x%x]", 
					son_convert_char_cell_id_to_int_cell_id(p_delete_req->
						delete_from_remove_list_req.src_cgi.cell_identity));
		}
	}

	/* Coverity Fix: 19144 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_DELETE_FROM_REMOVE_LIST_RES,
			sizeof(son_anr_nr_delete_from_remove_list_res_t));

	anr_print_cell_context(p_cell_ctxt);

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nr_delete_from_remove_list_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_nr_update_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_UPDATE_REQ message
 *                  is received in incorrect state.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_nr_update_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_update_req_t                *p_upd_req  =
		(son_anr_nr_update_req_t *)(p_msg);
	son_anr_nr_update_res_t                *p_res = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL ||
			p_cspl_hdr  == SON_PNULL ||
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = ( son_anr_nr_update_res_t *)son_mem_get(sizeof(son_anr_nr_update_res_t));
	if (SON_PNULL  == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Allocation failure ");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_update_res_t));

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	p_res->transaction_id = p_upd_req->transaction_id; 
	p_res->upd_res.result  = SON_FAILURE;
	p_res->upd_res.error_code = SON_ERR_UNEXPECTED_MSG;
	SON_MEMCPY(&p_res->upd_res.src_cgi,
			&p_upd_req->upd_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Coverity Fix: 19147 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_UPDATE_RES,
			sizeof(son_anr_nr_update_res_t));

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_nr_update_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nr_update_req
 * Inputs         : p_cspl_hdr     - Pointer to cspl header
 *                  p_cell_ctxt    - Pointer to ANR cell context
 *                  evnt_id        - Event id
 *                : p_msg          - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_UPDATE_REQ message
 *                  is received.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nr_update_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_update_req_t         *p_update_req =
		(son_anr_nr_update_req_t *)(p_msg);
	son_anr_rrm_updated_nrt_info_t  *p_notification_msg = SON_PNULL;
	/* Coverity Fix: 11378 */
	son_anr_updated_nrt_info_t      *p_oam_notif_upd = SON_PNULL;
	son_anr_nr_update_res_t            *p_res = SON_PNULL;
	son_oam_log_on_off_et           log_mode = anr_get_log_mode();
	son_return_et                   result = SON_SUCCESS;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_update_res_t *)
		son_mem_get(sizeof(son_anr_nr_update_res_t));
	if (SON_PNULL == p_res)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"Mem Alloc failure");
		SON_UT_TRACE_EXIT(); 
		return RELEASE_SON_BUFFER;
	}

	p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
	if (SON_PNULL == p_notification_msg)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"Mem Alloc failure ");

		if (SON_PNULL != p_res)
		{
			son_mem_free(p_res);    
		}

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_oam_notif_upd = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_oam_notif_upd)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"Mem Alloc failure ");

		if (SON_PNULL != p_res)
		{
			son_mem_free(p_res);    
		}

		if (SON_PNULL != p_notification_msg)
		{
			son_mem_free(p_notification_msg);    
		}

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_update_res_t));
	p_res->transaction_id = p_update_req->transaction_id;
	p_res->upd_res.error_code =  SON_NO_ERROR;

	SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
	SON_MEMSET(p_oam_notif_upd, 0, sizeof(son_anr_updated_nrt_info_t));
	SON_MEMCPY(&p_res->upd_res.src_cgi,
			&p_update_req->upd_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMCPY(&p_notification_msg->src_cgi,
			&p_update_req->upd_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMCPY(&p_oam_notif_upd->src_cgi, &p_update_req->upd_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	if (0 == p_update_req->upd_req.nr_list.intra_rat_nr_list_size &&
			0 == p_update_req->upd_req.nr_list.inter_rat_nr_list_size)
	{
		SON_LOG(log_mode, p_son_anr_facility_name,
				SON_ERROR,
				"Empty Neighbors List "
				"NR_list_size is 0 for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_update_req->upd_req.src_cgi.cell_identity));

		result = SON_FAILURE;
		p_res->upd_res.error_code = SON_ERR_INVALID_PARAMS;
	}
	else
	{
		if (p_update_req->upd_req.nr_list.intra_rat_nr_list_size)
		{   
			if (SON_FAILURE == anr_process_intra_rat_neighbors( p_cell_ctxt,
						p_update_req->upd_req.nr_list.intra_rat_nr_list_size,
						p_update_req->upd_req.nr_list.intra_rat_nr_list,
						&p_res->upd_res.trgt_cell_status_list_size,
						p_res->upd_res.trgt_cell_status_list,
						p_oam_notif_upd,
						p_notification_msg,
						anr_intra_rat_modify_nr,
						evnt_id))
			{
				result = SON_FAILURE;
			}
		}

		if (p_update_req->upd_req.nr_list.inter_rat_nr_list_size)
		{
			if (SON_FAILURE == anr_process_inter_rat_neighbors( p_cell_ctxt,
						p_update_req->upd_req.nr_list.inter_rat_nr_list_size,
						p_update_req->upd_req.nr_list.inter_rat_nr_list,
						&p_res->upd_res.trgt_cell_status_list_size,
						p_res->upd_res.trgt_cell_status_list,
						p_notification_msg,
						anr_inter_rat_modify_nr,
						evnt_id))
			{
				result = SON_FAILURE;
			}
		}
	}

	p_res->upd_res.result = result;

	if (p_res->upd_res.trgt_cell_status_list_size &&
			p_res->upd_res.trgt_cell_status_list_size < (
				p_update_req->upd_req.nr_list.inter_rat_nr_list_size +
				p_update_req->upd_req.nr_list.intra_rat_nr_list_size))
	{
		p_res->upd_res.result = SON_PARTIAL_SUCCESS;
	}

	anr_send_updated_ncl_to_rrm(p_notification_msg,
			son_convert_char_cell_id_to_int_cell_id(p_update_req->upd_req.src_cgi.cell_identity));

	anr_send_updated_ncl_to_oam(p_oam_notif_upd);

    /* Spr 17753 Changes Start */
    anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
    /* Spr 17753 Changes End */
	/* Coverity Fix: 19147 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_UPDATE_RES,
			sizeof(son_anr_nr_update_res_t));

	anr_print_cell_context(p_cell_ctxt);

	if (SON_PNULL != p_notification_msg)
	{
		son_mem_free(p_notification_msg);
	}

	if (SON_PNULL != p_oam_notif_upd)
	{
		son_mem_free(p_oam_notif_upd);
	}

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nr_update_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nack_nr_retrieve_req
 * Inputs         : p_cspl_hdr    - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id       - Event id
 *                : p_msg         - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_RETRIEVE_REQ message
 *                  is received in incorrect state.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_nr_retrieve_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_retrieve_req_t                *p_req  =
		(son_anr_nr_retrieve_req_t *)(p_msg);
	son_anr_nr_retrieve_res_t                *p_res = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || 
			p_cspl_hdr  == SON_PNULL || 
			p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cell_ctxt, p_cspl_hdr or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_retrieve_res_t *)son_mem_get(sizeof(son_anr_nr_retrieve_res_t));
	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory alloc failure ");

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_retrieve_res_t));

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from);

	p_res->transaction_id = p_req->transaction_id;
	p_res->rtrv_res.result  = SON_FAILURE;
	p_res->rtrv_res.error_code = SON_ERR_UNEXPECTED_MSG;
	SON_MEMCPY(&p_res->rtrv_res.src_cgi,
			&p_req->rtrv_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Coverity Fix: 19146 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_RETRIEVE_RES,
			sizeof(son_anr_nr_retrieve_res_t));

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nack_nr_retrieve_req */

/*****************************************************************************
 * Function Name  : anr_verify_nr_record
 * Inputs         : nr_status      - NR status of the neighbour relation
 *                : ho_status      - HO status of the neighbour relation
 *                : x2_status      - X2 status of the neighbour relation
 *                : p_req          - Pointer to nr retrieve req
 * Returns        : son_bool_et    - Returns SON_SUCCESS when successful 
 *                                   otherwise SON_FAILURE
 * Description    : This function is invoked form 
 ******************************************************************************/
static son_bool_et
anr_verify_nr_record
( 
 son_nr_status_et                         nr_status,
 son_ho_status_et                         ho_status,   
 son_x2_status_et                         x2_status,
 son_anr_nr_retrieve_req_t                 *p_req
 )
{
	son_bool_et   is_valid = SON_FALSE;


	SON_UT_TRACE_ENTER();

	if (p_req == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_req is NULL"
				);
		SON_UT_TRACE_EXIT();
		return is_valid;
	}

	if (SON_RETRIEVE_NR_STATUS_PRESENT == p_req->rtrv_req.bitmask)
	{
		if (nr_status == p_req->rtrv_req.nr_status)
		{
			is_valid  = SON_TRUE;
		}
	}
	else if (SON_RETRIEVE_HO_STATUS_PRESENT == p_req->rtrv_req.bitmask)
	{
		if (ho_status == p_req->rtrv_req.ho_status)
		{
			is_valid  = SON_TRUE;
		}
	}
	else if (SON_RETRIEVE_X2_STATUS_PRESENT == p_req->rtrv_req.bitmask)
	{
		if (SON_X2_ALLOWED == p_req->rtrv_req.x2_status)
		{
			if (x2_status == SON_X2_ALLOWED_CONNECTED ||
					x2_status == SON_X2_ALLOWED_DISCONNECTED)
			{
				is_valid  = SON_TRUE;
			}
		}
		else if (SON_X2_PROHIBITED == p_req->rtrv_req.x2_status)
		{
			if (x2_status == SON_X2_PROHIBITED_CONNECTED ||
					x2_status == SON_X2_PROHIBITED_DISCONNECTED)
			{
				is_valid  = SON_TRUE;
			}
		}
	}
	else if ((SON_RETRIEVE_HO_STATUS_PRESENT | SON_RETRIEVE_NR_STATUS_PRESENT) ==
			p_req->rtrv_req.bitmask)
	{
		if (nr_status == p_req->rtrv_req.nr_status &&
				ho_status == p_req->rtrv_req.ho_status)
		{
			is_valid  = SON_TRUE;
		}
	}
	else if ((SON_RETRIEVE_X2_STATUS_PRESENT | SON_RETRIEVE_NR_STATUS_PRESENT) ==
			p_req->rtrv_req.bitmask)
	{
		if (SON_X2_ALLOWED == p_req->rtrv_req.x2_status)
		{
			if ((x2_status == SON_X2_ALLOWED_CONNECTED ||
						x2_status == SON_X2_ALLOWED_DISCONNECTED) &&
					nr_status == p_req->rtrv_req.nr_status)
			{
				is_valid  = SON_TRUE;
			}
		}
		else if (SON_X2_PROHIBITED == p_req->rtrv_req.x2_status)
		{
			if ((x2_status == SON_X2_PROHIBITED_CONNECTED ||
						x2_status == SON_X2_PROHIBITED_DISCONNECTED) &&
					nr_status == p_req->rtrv_req.nr_status)
			{
				is_valid  = SON_TRUE;
			}
		}
	}
	else if (!p_req->rtrv_req.bitmask)
	{
		is_valid  = SON_TRUE;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Invalid bitmask[0x%x] combination "
				"in retrieve request for cell [Cell Id: 0x%x]",
                p_req->rtrv_req.bitmask,
				son_convert_char_cell_id_to_int_cell_id(p_req->rtrv_req.src_cgi.cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return is_valid;
} /* anr_verify_nr_record */

/*****************************************************************************
 * Function Name  : anr_retrieve_intra_rat_nbrs
 * Inputs         : p_intra_rat_neighbor_table - Pointer to intra rat hash table
 *                  p_req        - Pointer to ANR cell context request
 * Outputs        : p_res        - Pointer to ANR cell context response
 * Returns        : son_void_t   - None
 * Description    : Function to retrieve intra rat nbr. 
 ******************************************************************************/
static 
son_void_t
anr_retrieve_intra_rat_nbrs
(
 SON_HASH *p_intra_rat_neighbor_table,
 son_anr_nr_retrieve_req_t *p_req,
 son_anr_nr_retrieve_res_t *p_res

 )
{
	anr_intra_rat_hash_nr_tuple_t     *p_intra_tpl = SON_PNULL;  
	son_neighbor_info_t               *p_nbr_info  = &p_res->rtrv_res.nr_list;


	SON_UT_TRACE_ENTER();

	if (p_req == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_req is NULL"
				);
		SON_UT_TRACE_EXIT();
	}

	p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(
			p_intra_rat_neighbor_table );

	while (SON_PNULL != p_intra_tpl)
	{   
		if (anr_verify_nr_record( p_intra_tpl->data.neighbor_info.nr_status, 
					p_intra_tpl->data.neighbor_info.ho_status,
					p_intra_tpl->data.neighbor_info.x2_status, 
					p_req))
		{                
			SON_MEMCPY(&p_nbr_info->intra_rat_nr_list
					[p_nbr_info->intra_rat_nr_list_size++],
					&p_intra_tpl->data, 
					sizeof(son_intra_rat_neighbor_info_t));                 
		}

		p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
				p_intra_rat_neighbor_table, &p_intra_tpl->anchor);
	}

	SON_UT_TRACE_EXIT();
} /* anr_retrieve_intra_rat_nbrs */

/*****************************************************************************
 * Function Name  : anr_retrieve_inter_rat_nbrs
 * Inputs         : p_inter_rat_neighbor_table  - Pointer to intra rat hash table
 *                  p_req       - Pointer to ANR cell context request
 * Outputs        : p_res       - Pointer to cell context response
 * Returns        : son_void_t  - None
 * Description    : Function to retrieve inter rat nbr 
 ******************************************************************************/
static 
son_void_t
anr_retrieve_inter_rat_nbrs
(
 SON_HASH   *p_inter_rat_neighbor_table,
 son_anr_nr_retrieve_req_t *p_req,
 son_anr_nr_retrieve_res_t *p_res
 )
{
	anr_inter_rat_hash_nr_tuple_t      *p_inter_tpl     = SON_PNULL;
	son_bool_et                        isvalid          = SON_FALSE;
	son_void_t                         *p_nr_from       = SON_PNULL;
	son_void_t                         *p_nr_to         = SON_PNULL;
	son_u16                            length           = 0;
	son_u16                            mask_value       = 0;
	son_neighbor_info_t *p_nbr_info =  &p_res->rtrv_res.nr_list;


	SON_UT_TRACE_ENTER();

	if (p_req == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_req is NULL"
				);
		SON_UT_TRACE_EXIT();
	}

	p_inter_tpl =  anr_get_first_nr_from_inter_rat_hash_table(
			p_inter_rat_neighbor_table);

	while (SON_PNULL != p_inter_tpl)
	{
		switch (p_inter_tpl->data.t)
		{
			case ANR_UTRAN_NEIGHBOR:
				{
					isvalid  = anr_verify_nr_record( p_inter_tpl->data.u.utran_neighbor_info.nr_status,
							p_inter_tpl->data.u.utran_neighbor_info.ho_status,
							SON_X2_UNDEFINED, p_req);

					p_nr_from  =  &p_inter_tpl->data.u.utran_neighbor_info;

					p_nr_to    =  &p_nbr_info->inter_rat_nr_list
						[p_nbr_info->inter_rat_nr_list_size].
						utran_neighbor;

					length     =  sizeof(son_utran_neighbor_info_t);
					mask_value = SON_INTER_UTRAN_NEIGHBOR;                        
					break;
				}

			case ANR_GERAN_NEIGHBOR:
				{
					isvalid  = anr_verify_nr_record( p_inter_tpl->data.u.geran_neighbor_info.nr_status,
							p_inter_tpl->data.u.geran_neighbor_info.ho_status,
							SON_X2_UNDEFINED, p_req);

					p_nr_from  =  &p_inter_tpl->data.u.geran_neighbor_info;

					p_nr_to    =  &p_nbr_info->inter_rat_nr_list
						[p_nbr_info->inter_rat_nr_list_size].
						geran_neighbor;

					length     =  sizeof(son_geran_neighbor_info_t);
					mask_value = SON_INTER_GERAN_NEIGHBOR; 
					break;
				}

				/*start:gur32802*/
			case ANR_CDMA_NEIGHBOR:
				{
					isvalid = anr_verify_nr_record( p_inter_tpl->data.u.cdma_neighbor_info.nr_status,
							p_inter_tpl->data.u.cdma_neighbor_info.ho_status,
							SON_X2_UNDEFINED, p_req);
					p_nr_from  =  &p_inter_tpl->data.u.cdma_neighbor_info;
					p_nr_to    =  &p_nbr_info->inter_rat_nr_list
						[p_nbr_info->inter_rat_nr_list_size].cdma_neighbor;

					length     =  sizeof(son_cdma_neighbor_info_t);
					mask_value = SON_INTER_CDMA_NEIGHBOR;
					break;
				}

				/*end:gur32802*/
			default:
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_ERROR,
							"Invalid t [0x%x] value in"
							" Inter-RAT neighbor cell", p_inter_tpl->data.t);
					break; 
				}
		} /* switch */

		if (isvalid)
		{
			SON_MEMCPY(p_nr_to, p_nr_from, length);
			p_nbr_info->inter_rat_nr_list
				[p_nbr_info->inter_rat_nr_list_size].bitmask |= mask_value;

			p_nbr_info->inter_rat_nr_list_size++; 
		}

		p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
				p_inter_rat_neighbor_table, &p_inter_tpl->anchor );
	}

	SON_UT_TRACE_EXIT();
} /* anr_retrieve_inter_rat_nbrs */

/*****************************************************************************
 * Function Name  : anr_process_retrieve_req
 * Inputs         : p_cell_ctxt      - Pointer to ANR cell context
 *                : p_req            - Pointer to the retrieve request
 * Outputs        : p_res            - Pointer to the retrieve response
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked from anr_fsm_nr_retrieve_req
 ******************************************************************************/
static son_return_et
anr_process_retrieve_req
(
 anr_cell_context_t *p_cell_ctxt,
 son_anr_nr_retrieve_req_t *p_req,
 son_anr_nr_retrieve_res_t *p_res
 )
{
	son_error_et                      err_code                = SON_NO_ERROR;
	son_void_t                        *p_hash_rec             = SON_PNULL;
	son_rat_type_et                   rat_type                = SON_EUTRAN_CELL;
	son_void_t                        *p_nr_from              = SON_PNULL;
	son_void_t                        *p_nr_to                = SON_PNULL;
	son_u16                           length                  = 0;


	SON_UT_TRACE_ENTER();
	if (p_cell_ctxt == SON_PNULL || p_req == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_req or p_cell_ctxt is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	p_res->transaction_id = p_req->transaction_id;
	p_res->rtrv_res.result     =  SON_FAILURE;
	p_res->rtrv_res.error_code =  SON_ERR_ANR_NR_NOT_FOUND;

	SON_MEMCPY(&p_res->rtrv_res.src_cgi,
			&p_req->rtrv_req.src_cgi, sizeof(son_intra_rat_global_cell_id_t));

	if (p_req->rtrv_req.bitmask == SON_RETRIEVE_TRGT_CGI_PRESENT)
	{
		p_hash_rec = anr_find_nr( p_cell_ctxt, &p_req->rtrv_req.trgt_cgi,
				&err_code, &rat_type);
		if (p_hash_rec)
		{
			p_res->rtrv_res.result     =  SON_SUCCESS;
			p_res->rtrv_res.error_code =  SON_NO_ERROR;
			switch (rat_type)
			{
				case SON_EUTRAN_CELL:
					{
						length = sizeof(son_intra_rat_neighbor_info_t);
						p_nr_from  = 
							&(((anr_intra_rat_hash_nr_tuple_t *)p_hash_rec)->data);

						p_nr_to = &p_res->rtrv_res.nr_list.
							intra_rat_nr_list[p_res->rtrv_res.nr_list.
							intra_rat_nr_list_size];

						p_res->rtrv_res.nr_list.intra_rat_nr_list_size++;           
						break;
					}

				case SON_UTRAN_CELL: 
					{
						length = sizeof(son_utran_neighbor_info_t);
						p_nr_from  = &((anr_inter_rat_hash_nr_tuple_t *)p_hash_rec)->data
							.u.utran_neighbor_info;
						p_nr_to    = &p_res->rtrv_res.nr_list.
							inter_rat_nr_list[p_res->rtrv_res.nr_list.
							inter_rat_nr_list_size].utran_neighbor;

						p_res->rtrv_res.nr_list.inter_rat_nr_list[p_res->rtrv_res.nr_list. 
							inter_rat_nr_list_size].bitmask         =
							SON_INTER_UTRAN_NEIGHBOR;

						p_res->rtrv_res.nr_list.inter_rat_nr_list_size++;

						break;
					}

				case SON_GERAN_CELL:
					{
						length = sizeof(son_geran_neighbor_info_t);
						p_nr_from  = &((anr_inter_rat_hash_nr_tuple_t *)p_hash_rec)->data
							.u.geran_neighbor_info;

						p_nr_to = &p_res->rtrv_res.nr_list.
							inter_rat_nr_list[p_res->rtrv_res.nr_list.
							inter_rat_nr_list_size].geran_neighbor;
						p_res->rtrv_res.nr_list.inter_rat_nr_list[p_res->rtrv_res.nr_list. 
							inter_rat_nr_list_size].bitmask         =
							SON_INTER_GERAN_NEIGHBOR;

						p_res->rtrv_res.nr_list.inter_rat_nr_list_size++;
						break;
					}

					/*start:gur32802*/
				case SON_CDMA_CELL:
					{
						length = sizeof(son_cdma_neighbor_info_t);
						p_nr_from  = &((anr_inter_rat_hash_nr_tuple_t *)p_hash_rec)->data
							.u.cdma_neighbor_info;

						p_nr_to = &p_res->rtrv_res.nr_list.inter_rat_nr_list[p_res->rtrv_res
							.nr_list.inter_rat_nr_list_size].cdma_neighbor;

						p_res->rtrv_res.nr_list.inter_rat_nr_list[p_res->rtrv_res.nr_list.
							inter_rat_nr_list_size].bitmask = SON_INTER_CDMA_NEIGHBOR;

						p_res->rtrv_res.nr_list.inter_rat_nr_list_size++;
						break;
					}

					/*end:gur32802*/
				default:
					{
						SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
								SON_ERROR,
								"Invalid RatType [%d]", rat_type);

						break;
					}
			} /* switch */

			SON_MEMCPY(p_nr_to, p_nr_from, length);
		}   
		else
		{
			p_res->rtrv_res.error_code = err_code;                   
		}   
	}
	else
	{
		anr_retrieve_intra_rat_nbrs(&p_cell_ctxt->intra_rat_neighbor_table,
				p_req, p_res);

		anr_retrieve_inter_rat_nbrs(&p_cell_ctxt->inter_rat_neighbor_table,
				p_req, p_res);
	}

	if (p_res->rtrv_res.nr_list.intra_rat_nr_list_size ||
			p_res->rtrv_res.nr_list.inter_rat_nr_list_size)
	{
		p_res->rtrv_res.result     =  SON_SUCCESS;
		p_res->rtrv_res.error_code =  SON_NO_ERROR;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"No NRs found for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id(p_req->rtrv_req.src_cgi.cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* anr_process_retrieve_req */

/*****************************************************************************
 * Function Name  : anr_fsm_nr_retrieve_req
 * Inputs         : p_cspl_hdr      - Pointer to cspl header
 *                  p_cell_ctxt     - Pointer to ANR cell context
 *                  evnt_id         - Event id
 *                : p_msg           - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is invoked when SONANR_NR_RETRIEVE_REQ message
 *                  is received.
 ******************************************************************************/
static son_buf_retain_status_et
anr_fsm_nr_retrieve_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	son_anr_nr_retrieve_res_t     *p_res                    = SON_PNULL;
	son_anr_nr_retrieve_req_t     *p_req =
		(son_anr_nr_retrieve_req_t *)(p_msg);


	SON_UT_TRACE_ENTER();
	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr, p_cell_ctxt or p_msg is NULL evnt_id [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	p_res = (son_anr_nr_retrieve_res_t *)
		son_mem_get(sizeof(son_anr_nr_retrieve_res_t));
	if (SON_PNULL == p_res)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory Alloc failure");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER; 
	}

	SON_MEMSET(p_res, 0, sizeof(son_anr_nr_retrieve_res_t));

	anr_process_retrieve_req(p_cell_ctxt, p_req, p_res);

	/* Coverity Fix: 19127,19979 */
	son_create_send_buffer((son_u8 *)p_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_NR_RETRIEVE_RES,
			sizeof(son_anr_nr_retrieve_res_t));

	if (SON_PNULL != p_res)
	{
		son_mem_free(p_res);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_nr_retrieve_req */

/*****************************************************************************
 * Function Name   : anr_fsm_invalid_event_handler
 * Inputs         : p_cspl_hdr       - Pointer to cspl header
 *                  p_cell_ctxt      - Pointer to ANR cell context
 *                  evnt_id          - Event id
 *                : p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : This function is an invalid event handler.It is invoked
 *                  when an unexpected event occurs.
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_invalid_event_handler
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8 *p_msg
 )
{
	SON_UT_TRACE_ENTER();

        /*SPR 17777 +-*/ 
	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
			"Invalid Event[%s] occured "
			"in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d p_msg [%u] ",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api, p_cspl_hdr->from,*p_msg);
        /*SPR 17777 +-*/ 

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/******************************************************************************
 * Function Name  : anr_fsm_cell_state_change_handler
 * Inputs         : p_cspl_hdr        - Pointer to cspl header
 *                  p_cell_context    - Pointer to ANR cell context
 *                  evnt_id           - Event id
 *                : p_msg             - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler to handle RRM_CELL_STATE_CHANGE_IND message.
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_cell_state_change_handler
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t   *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8              *p_msg
 )
{
	son_cell_state_change_indication_t *p_ind =
		(son_cell_state_change_indication_t *)p_msg;
	son_anr_feature_state_change_ind_t state_change_ind;
	son_x2_nrt_update_ind_t nrt_upd_ind   = {0};   

	son_u8 pend_res_cnt = 0;


	SON_UT_TRACE_ENTER();
	if (p_cell_ctxt == SON_PNULL || p_cspl_hdr == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr, p_cell_ctxt or p_msg is NULL evnt_id [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	SON_MEMSET(&state_change_ind, 0, sizeof(son_anr_feature_state_change_ind_t));

	if (RRM_CELL_DELETED ==  p_ind->cell_state)
	{
		if (anr_get_pending_res_expected_count())
		{
			anr_delete_cell_from_pending_res
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id);

			if (!(pend_res_cnt = anr_get_pending_res_expected_count()) ||
					(pend_res_cnt &&
					 anr_get_pending_res_ts_diff_from_curr_ts() >
					 ANR_TIMEOUT_FOR_PENDING_RES))
			{
				anr_pending_res_timer_handler();
				anr_reset_pending_res();
				anr_set_context_state(ANR_STATE_ACTIVE);
			}
		}
		else
		{
			/* Copy the CGI */
			SON_MEMCPY(&state_change_ind.srv_cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));

			/* Set the Module ID */
			state_change_ind.module_id = SON_ANR_MODULE_ID;

			/* Set state to Deleted */
			state_change_ind.state = SON_STATE_DELETED;
			state_change_ind.cause = SON_RRM_CELL_UNCONFIGURED;

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF,
					"Sending cell state change indication for cell"
					"[Cell Id: %d]",
					son_convert_char_cell_id_to_int_cell_id(state_change_ind.srv_cgi.cell_identity));

			son_create_send_buffer((son_u8 *)&state_change_ind,
					SON_ANR_MODULE_ID,
					SON_MIF_MODULE_ID,
					SONANR_FEATURE_STATE_CHANGE_IND,
					sizeof(state_change_ind));
		}

		if (SON_TRUE == anr_delete_cell_from_context
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id))
		{
			/* Rel 3.0 Multi Sector code changes Start */
			anr_delete_serving_cell_as_nbr_autonomously
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id);
			/* Rel 3.0 Multi Sector code changes Stop */
			anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
					(son_intra_rat_global_cell_id_t *)&p_ind->cell_id,
					SON_NRT_OP_DELETE);
		}
		else
		{
			SON_MEMCPY(&state_change_ind.srv_cgi,
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));

			/* Set the Module ID */
			state_change_ind.module_id = SON_ANR_MODULE_ID;

			/* Set state to Deleted */
			state_change_ind.state = SON_STATE_DELETED;
			state_change_ind.cause = SON_RRM_CELL_UNCONFIGURED;

			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_BRIEF,
					"Sending cell state change indication for cell"
					"[Cell Id: %d]",
					*((son_u32 *)&state_change_ind.srv_cgi));

			son_create_send_buffer((son_u8 *)&state_change_ind,
					SON_ANR_MODULE_ID,
					SON_MIF_MODULE_ID,
					SONANR_FEATURE_STATE_CHANGE_IND,
					sizeof(state_change_ind));
		}
	}
	else if (RRM_CELL_UNOPERATIONAL ==  p_ind->cell_state)
	{
		if (anr_get_pending_res_expected_count())
		{
			anr_delete_cell_from_pending_res
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id);
		}
		/* SPR-12483 Fix Starts */
		anr_delete_serving_cell_as_nbr_autonomously(
				(son_intra_rat_global_cell_id_t *)&p_ind->cell_id);
		/* SPR-12483 Fix Ends */
	}
	/* SPR-12483 Fix Starts */
	else if (RRM_CELL_OPERATIONAL ==  p_ind->cell_state)
	{
		anr_add_serving_cell_as_nbr_autonomously(
				(son_intra_rat_global_cell_id_t *)&p_ind->cell_id);
	}
	/* SPR-12483 Fix Ends */

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_cell_state_change_handler */

/******************************************************************************
 * Function Name  : anr_configure_suspect_pci
 * Inputs         : p_cell_context    Pointer to ANR cell context
 *                : p_eutran_nbr      Pointer to the NR tuple
 *                : p_oam_notif       Pointer to message for oam notification
 * Outputs        : None
 * Returns        :
 * Description    : Function to process HO Reports for UTRAN Neighbors
 *****************************************************************************/
static  son_bool_et
anr_configure_suspect_pci
(
 anr_cell_context_t   *p_cell_ctxt,
 anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr,
 son_anr_updated_nrt_info_t    *p_oam_notif
 )
{
	anr_pci_sus_conf_node_t *p_pci_sus_conf_node = SON_PNULL;
	son_u8                   clear_sus_pci_thresh = 0;
	son_bool_et               ret                 = SON_FALSE;


	SON_UT_TRACE_ENTER();
	if (p_cell_ctxt == SON_PNULL || p_eutran_nbr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr or p_eutran_nbr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	p_pci_sus_conf_node = anr_insert_in_sus_pci_ctxt_list(p_cell_ctxt,
			p_eutran_nbr->data.neighbor_info.pci,
			p_eutran_nbr->data.neighbor_info.earfcn,
			SON_PCI_SUSPECTED, 1);

	if (SON_PNULL != p_pci_sus_conf_node)
	{
		/*1. Set the PCI as suspected for the neighbor*/
		p_eutran_nbr->data.neighbor_info.suspected_pci = SON_TRUE;
		p_eutran_nbr->data.neighbor_info.bitmask |= SON_SUSPECT_PCI_PRESENT;

		/*2. Send the OAM notification for the modifcation of SUSPECT PCI field*/
		anr_construct_oam_updated_ncl(&p_oam_notif->nr_list,
				&p_eutran_nbr->data, SON_FALSE);

		/*3. Set the ho_failed_for_pci_conflict to 0 for the neighbor*/
		p_eutran_nbr->data.ho_failed_for_pci_conflict = 0;

		/*4. Set the received_cgi_counter field for the neighbor*/
		if (SON_CLEAR_SUSPECT_PCI_THRESHOLD_PRESENT & p_cell_ctxt->pci_confusion_cfg.bitmask)
		{
			clear_sus_pci_thresh = p_cell_ctxt->pci_confusion_cfg.clear_suspect_pci_threshold;
		}

		p_eutran_nbr->data.received_cgi_counter = clear_sus_pci_thresh;
		ret = SON_TRUE;
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_configure_suspect_pci */

/******************************************************************************
 * Function Name  : anr_process_eutran_ho_report
 * Inputs         : p_cell_ctxt   - Pointer to ANR cell context
 *                : p_eutran_nbr	 - Pointer to the NR tuple
 *                : p_ind			 - Pointer to HO Report
 * Outputs        : None
 * Returns        : son_bool_et      - returns SON_TRUE if success otherwise
 *                                     SON_FALSE
 * Description    : Function to process SUCCESS/FAILURE HO Reports for EUTRAN Neighbors
 *****************************************************************************/
static son_bool_et
anr_process_eutran_ho_report
(
 anr_cell_context_t   *p_cell_ctxt,
 anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr,
 son_rrm_ho_report_t *p_ind
 )
{
	son_bool_et ret = SON_TRUE;
	son_return_et result; 


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_eutran_nbr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr or p_eutran_nbr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}

	if (SON_TRUE == is_intra_rat_nbr_notifiable_to_rrm(&p_eutran_nbr->data.neighbor_info) &&
			SON_HO_ALLOWED == p_eutran_nbr->data.neighbor_info.ho_status)
	{
		if (SON_MAX_U8_VAL > p_eutran_nbr->data.neighbor_info.handover_stats.handovers_attempted)
		{
			p_eutran_nbr->data.neighbor_info.handover_stats.handovers_attempted++;
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
					"Neighbor Cell"
					" [Cell Id: 0x%x], Max Ho Successful count reached! Counter not incremented",
					son_convert_char_cell_id_to_int_cell_id(p_eutran_nbr->data.neighbor_info.nbr_cgi.cell_identity));
		}

		/* Coverity Fix: 24823 */
		result = ((p_ind->result == RRM_SUCCESS) ? SON_SUCCESS : SON_FAILURE);
		if (SON_SUCCESS == result)
		{
			if (SON_MAX_U8_VAL > p_eutran_nbr->data.neighbor_info.handover_stats.handovers_succesful)
			{
				p_eutran_nbr->data.neighbor_info.handover_stats.handovers_succesful++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Successful count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_eutran_nbr->data.neighbor_info.nbr_cgi.cell_identity));
			}

			p_eutran_nbr->data.ho_failed_for_nr_removal = 0;
		}
		else
		{
			if (SON_MAX_U8_VAL > p_eutran_nbr->data.neighbor_info.handover_stats.handovers_failed)
			{
				p_eutran_nbr->data.neighbor_info.handover_stats.handovers_failed++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_eutran_nbr->data.neighbor_info.nbr_cgi.cell_identity));
			}

			/*Handover failed for NR removal*/
			if (SON_MAX_U8_VAL > p_eutran_nbr->data.ho_failed_for_nr_removal)
			{
				p_eutran_nbr->data.ho_failed_for_nr_removal++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed NR removal count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_eutran_nbr->data.neighbor_info.nbr_cgi.cell_identity));
			}
		}

		son_time(&p_eutran_nbr->data.last_activity_ts);

		determine_the_suspecion_deletion_of_nr(p_eutran_nbr, p_cell_ctxt);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_process_eutran_ho_report */


/*SPR-8950-Start*/

/******************************************************************************
 * Function Name  : anr_process_utran_ho_report
 * Inputs         : p_cell_ctxt   - Pointer to ANR cell context
 *                : p_utran_nbr  - Pointer to the NR tuple
 *                : p_ind            - Pointer to HO Report
 * Outputs        : None
 * Returns        : son_bool_et      - returns SON_TRUE if success otherwise
 *                                     SON_FALSE
 * Description    : Function to process SUCCESS/FAILURE HO Reports for UTRAN Neighbors
 *****************************************************************************/
static son_bool_et
anr_process_utran_ho_report
(
 anr_cell_context_t   *p_cell_ctxt,
 anr_inter_rat_hash_nr_tuple_t *p_utran_nbr,
 son_rrm_ho_report_t *p_ind
 )
{
	son_bool_et ret = SON_TRUE;
	son_return_et result;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_utran_nbr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr or p_utran_nbr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}
	if (SON_TRUE == is_utran_nbr_notifiable_to_rrm(&p_utran_nbr->data.u.utran_neighbor_info) &&
			SON_HO_ALLOWED == p_utran_nbr->data.u.utran_neighbor_info.ho_status)
	{
		if (SON_MAX_U8_VAL > p_utran_nbr->data.u.utran_neighbor_info.handover_stats.handovers_attempted)
		{
			p_utran_nbr->data.u.utran_neighbor_info.handover_stats.handovers_attempted++;
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
					"Neighbor Cell"
					" [Cell Id: 0x%x], Max Ho Successful count reached! Counter not incremented",
					son_convert_char_cell_id_to_int_cell_id(p_utran_nbr->data.u.utran_neighbor_info.nbr_cgi.cell_identity));
		}

		result = ((p_ind->result == RRM_SUCCESS) ? SON_SUCCESS : SON_FAILURE);
		if (SON_SUCCESS == result)
		{
			if (SON_MAX_U8_VAL > p_utran_nbr->data.u.utran_neighbor_info.handover_stats.handovers_succesful)
			{
				p_utran_nbr->data.u.utran_neighbor_info.handover_stats.handovers_succesful++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Successful count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_utran_nbr->data.u.utran_neighbor_info.nbr_cgi.cell_identity));
			}

			p_utran_nbr->data.ho_failed_for_nr_removal = 0;
		}
		else
		{
			if (SON_MAX_U8_VAL > p_utran_nbr->data.u.utran_neighbor_info.handover_stats.handovers_failed)
			{
				p_utran_nbr->data.u.utran_neighbor_info.handover_stats.handovers_failed++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_utran_nbr->data.u.utran_neighbor_info.nbr_cgi.cell_identity));
			}
			/*Handover failed for NR removal*/
			if (SON_MAX_U8_VAL > p_utran_nbr->data.ho_failed_for_nr_removal)
			{
				p_utran_nbr->data.ho_failed_for_nr_removal++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed NR removal count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_utran_nbr->data.u.utran_neighbor_info.nbr_cgi.cell_identity));
			}
		}

		son_time(&p_utran_nbr->data.last_activity_ts);

		delete_nr_as_failed_ho_exceeded_threshold(p_utran_nbr, p_cell_ctxt);

	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_process_utran_ho_report */

/* SPR 12542/SES-483 Fix Start */
/******************************************************************************
 * Function Name  : delete_geran_nr_as_failed_ho_exceeded_threshold
 * Inputs         : p_geran_nbr    - Pointer to hash record
 *                  p_cell_ctxt  - Pointer to ANR cell context
 * Outputs        : None
 * Returns        : son_void_t      - None
 * Description    : Deletes the neighbor if failed HO exceeds threshold
 *****************************************************************************/
son_void_t
delete_geran_nr_as_failed_ho_exceeded_threshold
(
 anr_inter_rat_hash_nr_tuple_t *p_geran_nbr,
 anr_cell_context_t   *p_cell_ctxt
 )
{
	if (p_geran_nbr == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_geran_nbr or p_cell_ctxt is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	anr_inter_rat_hash_rec_t         hash_rec;
	son_anr_rrm_updated_nrt_info_t  *p_rrm_notification = SON_PNULL;
	son_anr_updated_nrt_info_t      *p_delete_oam_notif = SON_PNULL;
	son_inter_rat_global_cell_id_t  *p_nr_cell_id = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_rrm_notification = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_rrm_notification)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_delete_oam_notif = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_delete_oam_notif)
	{
		if (SON_PNULL != p_rrm_notification)
		{
			son_mem_free(p_rrm_notification);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}
	p_nr_cell_id = (son_inter_rat_global_cell_id_t *)
		son_mem_get(sizeof(son_inter_rat_global_cell_id_t));

	if (SON_PNULL == p_nr_cell_id)
	{
		if (SON_PNULL != p_rrm_notification)
		{
			son_mem_free(p_rrm_notification);
		}

		if (SON_PNULL != p_delete_oam_notif)
		{
			son_mem_free(p_delete_oam_notif);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(&hash_rec, 0, sizeof(anr_inter_rat_hash_rec_t));
	SON_MEMCPY(&hash_rec.u.geran_neighbor_info.nbr_cgi,
			&p_geran_nbr->data.u.geran_neighbor_info.nbr_cgi,
			sizeof(son_utran_geran_global_cell_id_t));

	SON_MEMSET(p_rrm_notification, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
	SON_MEMCPY(&p_rrm_notification->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_delete_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));
	SON_MEMCPY(&p_delete_oam_notif->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_nr_cell_id, 0, sizeof(son_inter_rat_global_cell_id_t));
	p_nr_cell_id->bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;
	SON_MEMCPY(&p_nr_cell_id->utran_geran_global_cell_id, &p_geran_nbr->data.u.geran_neighbor_info.nbr_cgi ,
			sizeof(son_utran_geran_global_cell_id_t));

	/* Autonomous neighbor removal if Failed HO > Threshold */
	if (SON_NR_UNLOCKED == p_geran_nbr->data.u.geran_neighbor_info.nr_status &&
			(SON_FAILED_HO_THRESHOLD_PRESENT & p_cell_ctxt->removal_criteria.bitmask) &&
			p_cell_ctxt->removal_criteria.handover_failure_threshold &&
			(p_cell_ctxt->removal_criteria.handover_failure_threshold <=
			 p_geran_nbr->data.ho_failed_for_nr_removal))
	{
		son_error_et error_code = SON_NO_ERROR;
		anr_inter_rat_del_nr(p_cell_ctxt,
				p_nr_cell_id,
				SON_TRUE,
				&error_code,
				&hash_rec,
				p_delete_oam_notif,
				p_rrm_notification);
	}

	/*Send the msg to OAM*/
	anr_send_updated_ncl_to_oam(p_delete_oam_notif);

	/*Send the msg to MLB*/
	if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->cell_id))
	{
		anr_send_updated_ncl_to_mlb(p_delete_oam_notif);
	}

	/*Send the msg to RRM*/
	anr_send_updated_ncl_to_rrm(p_rrm_notification,
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));

	if (SON_PNULL != p_rrm_notification)
	{
		son_mem_free(p_rrm_notification);
	}

	if (SON_PNULL != p_delete_oam_notif)
	{
		son_mem_free(p_delete_oam_notif);
	}

	if (SON_PNULL != p_nr_cell_id)
	{
		son_mem_free(p_nr_cell_id);
	}

	SON_UT_TRACE_EXIT();

} /* delete_geran_nr_as_failed_ho_exceeded_threshold  */

/******************************************************************************
 * Function Name  : anr_process_geran_ho_report
 * Inputs         : p_cell_ctxt   - Pointer to ANR cell context
 *                : p_geran_nbr  - Pointer to the NR tuple
 *                : p_ind            - Pointer to HO Report
 * Outputs        : None
 * Returns        : son_bool_et      - returns SON_TRUE if success otherwise
 *                                     SON_FALSE
 * Description    : Function to process SUCCESS/FAILURE HO Reports for GERAN Neighbors
 *****************************************************************************/
static son_bool_et
anr_process_geran_ho_report
(
 anr_cell_context_t   *p_cell_ctxt,
 anr_inter_rat_hash_nr_tuple_t *p_geran_nbr,
 son_rrm_ho_report_t *p_ind
 )
{
	son_bool_et ret = SON_TRUE;
	son_return_et result;


	SON_UT_TRACE_ENTER();

	if (p_cell_ctxt == SON_PNULL || p_geran_nbr == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr or p_geran_nbr is NULL"
				);
		SON_UT_TRACE_EXIT();
		return SON_FALSE;
	}
	if (SON_TRUE == is_geran_nbr_notifiable_to_rrm(&p_geran_nbr->data.u.geran_neighbor_info) &&
			SON_HO_ALLOWED == p_geran_nbr->data.u.geran_neighbor_info.ho_status)
	{
		if (SON_MAX_U8_VAL > p_geran_nbr->data.u.geran_neighbor_info.handover_stats.handovers_attempted)
		{
			p_geran_nbr->data.u.geran_neighbor_info.handover_stats.handovers_attempted++;
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
					"Neighbor Cell"
					" [Cell Id: 0x%x], Max Ho Successful count reached! Counter not incremented",
					son_convert_char_cell_id_to_int_cell_id(p_geran_nbr->data.u.geran_neighbor_info.nbr_cgi.cell_identity));
		}

		result = ((p_ind->result == RRM_SUCCESS) ? SON_SUCCESS : SON_FAILURE);
		if (SON_SUCCESS == result)
		{
			if (SON_MAX_U8_VAL > p_geran_nbr->data.u.geran_neighbor_info.handover_stats.handovers_succesful)
			{
				p_geran_nbr->data.u.geran_neighbor_info.handover_stats.handovers_succesful++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Successful count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_geran_nbr->data.u.geran_neighbor_info.nbr_cgi.cell_identity));
			}

			p_geran_nbr->data.ho_failed_for_nr_removal = 0;
		}
		else
		{
			if (SON_MAX_U8_VAL > p_geran_nbr->data.u.geran_neighbor_info.handover_stats.handovers_failed)
			{
				p_geran_nbr->data.u.geran_neighbor_info.handover_stats.handovers_failed++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_geran_nbr->data.u.geran_neighbor_info.nbr_cgi.cell_identity));
			}
			/*Handover failed for NR removal*/
			if (SON_MAX_U8_VAL > p_geran_nbr->data.ho_failed_for_nr_removal)
			{
				p_geran_nbr->data.ho_failed_for_nr_removal++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed NR removal count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_geran_nbr->data.u.geran_neighbor_info.nbr_cgi.cell_identity));
			}
		}

		son_time(&p_geran_nbr->data.last_activity_ts);

		delete_geran_nr_as_failed_ho_exceeded_threshold(p_geran_nbr, p_cell_ctxt);

	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_process_geran_ho_report */

/* SPR 12542/SES-483 Fix End */

/******************************************************************************
 * Function Name  : delete_nr_as_failed_ho_exceeded_threshold
 * Inputs         : p_utran_nbr    - Pointer to hash record
 *                  p_cell_ctxt  - Pointer to ANR cell context
 * Outputs        : None
 * Returns        : son_void_t      - None
 * Description    : Deletes the neighbor if failed HO exceeds threshold
 *****************************************************************************/
son_void_t
delete_nr_as_failed_ho_exceeded_threshold
(
 anr_inter_rat_hash_nr_tuple_t *p_utran_nbr,
 anr_cell_context_t   *p_cell_ctxt
 )
{
	if (p_utran_nbr == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_utran_nbr or p_cell_ctxt is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	anr_inter_rat_hash_rec_t         hash_rec;
	son_anr_rrm_updated_nrt_info_t  *p_rrm_notification = SON_PNULL;
	son_anr_updated_nrt_info_t      *p_delete_oam_notif = SON_PNULL;
	son_inter_rat_global_cell_id_t  *p_nr_cell_id = SON_PNULL;

	SON_UT_TRACE_ENTER();

	p_rrm_notification = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_rrm_notification)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_delete_oam_notif = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_delete_oam_notif)
	{
		if (SON_PNULL != p_rrm_notification)
		{
			son_mem_free(p_rrm_notification);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}
	p_nr_cell_id = (son_inter_rat_global_cell_id_t *)
		son_mem_get(sizeof(son_inter_rat_global_cell_id_t));

	if (SON_PNULL == p_nr_cell_id)
	{
		if (SON_PNULL != p_rrm_notification)
		{
			son_mem_free(p_rrm_notification);
		}

		if (SON_PNULL != p_delete_oam_notif)
		{
			son_mem_free(p_delete_oam_notif);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	SON_MEMSET(&hash_rec, 0, sizeof(anr_inter_rat_hash_rec_t));
	SON_MEMCPY(&hash_rec.u.utran_neighbor_info.nbr_cgi,
			&p_utran_nbr->data.u.utran_neighbor_info.nbr_cgi,
			sizeof(son_utran_geran_global_cell_id_t));

	SON_MEMSET(p_rrm_notification, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
	SON_MEMCPY(&p_rrm_notification->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_delete_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));
	SON_MEMCPY(&p_delete_oam_notif->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_nr_cell_id, 0, sizeof(son_inter_rat_global_cell_id_t));
	p_nr_cell_id->bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;
	SON_MEMCPY(&p_nr_cell_id->utran_geran_global_cell_id, &p_utran_nbr->data.u.utran_neighbor_info.nbr_cgi ,
			sizeof(son_utran_geran_global_cell_id_t));

	/* Autonomous neighbor removal if Failed HO > Threshold */
	if (SON_NR_UNLOCKED == p_utran_nbr->data.u.utran_neighbor_info.nr_status &&
			(SON_FAILED_HO_THRESHOLD_PRESENT & p_cell_ctxt->removal_criteria.bitmask) &&
			p_cell_ctxt->removal_criteria.handover_failure_threshold &&
			(p_cell_ctxt->removal_criteria.handover_failure_threshold <=
			 p_utran_nbr->data.ho_failed_for_nr_removal))
	{
		son_error_et error_code = SON_NO_ERROR;
		anr_inter_rat_del_nr(p_cell_ctxt,
				p_nr_cell_id,
				SON_TRUE,
				&error_code,
				&hash_rec,
				p_delete_oam_notif,
				p_rrm_notification);
	}

	/*Send the msg to OAM*/
	anr_send_updated_ncl_to_oam(p_delete_oam_notif);

	/*Send the msg to MLB*/
	if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->cell_id))
	{
		anr_send_updated_ncl_to_mlb(p_delete_oam_notif);
	}

	/*Send the msg to RRM*/
	anr_send_updated_ncl_to_rrm(p_rrm_notification,
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));

	if (SON_PNULL != p_rrm_notification)
	{
		son_mem_free(p_rrm_notification);
	}

	if (SON_PNULL != p_delete_oam_notif)
	{
		son_mem_free(p_delete_oam_notif);
	}

	if (SON_PNULL != p_nr_cell_id)
	{
		son_mem_free(p_nr_cell_id);
	}

	SON_UT_TRACE_EXIT();

} /* delete_nr_as_failed_ho_exceeded_threshold  */

/*SPR-8950-End*/

/******************************************************************************
 * Function Name  : convert_rrm_inter_rat_cell_id_to_son_inter_rat_cell_id
 * Inputs         : p_rrm_cell_id   -  Pointer to inter rat cell id for son
 * Outputs        : p_son_cell_id   - Pointer to inter rat cell id for rrm
 * Returns        : son_void_t      - None
 * Description    : This function converts the global cell id for UTRAN/GERAN for RRM
 *                  to SON.
 *****************************************************************************/
son_void_t
convert_rrm_inter_rat_cell_id_to_son_inter_rat_cell_id
(
 son_inter_rat_global_cell_id_t   *p_son_cell_id,
 rrm_inter_rat_global_cell_id_t   *p_rrm_cell_id 
 )
{
	SON_UT_TRACE_ENTER();
	if (p_rrm_cell_id == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_rrm_cell_id is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	if (RRM_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT ==  p_rrm_cell_id->bitmask)
	{
		p_son_cell_id->bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;
		SON_MEMCPY(&p_son_cell_id->utran_geran_global_cell_id,
				&p_rrm_cell_id->utran_geran_global_cell_id,
				sizeof(son_utran_geran_global_cell_id_t));
	}
	else
	{
		/* future rel */    
	}

	SON_UT_TRACE_EXIT();
} /* convert_rrm_inter_rat_cell_id_to_son_inter_rat_cell_id */

/******************************************************************************
 * Function Name  : convert_rrm_gl_cell_id_to_son_gl_cell_id
 * Inputs         : p_rrm_gl_cell_id  - Pointer to global cell id for son
 * Output         : p_son_gl_cell_id  - Pointer to global cell id for rrm
 * Returns        : son_void_t        - None
 * Description    : This function converts the global cell id structure from RRM
 *                  to SON
 *****************************************************************************/
son_void_t
convert_rrm_gl_cell_id_to_son_gl_cell_id
(
 son_global_cell_id_t        *p_son_gl_cell_id,
 rrm_oam_global_cell_id_t    *p_rrm_gl_cell_id
 )
{
	SON_UT_TRACE_ENTER();
	if (p_rrm_gl_cell_id == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_rrm_gl_cell_id is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	if (RRM_INTRA_RAT_GLOBAL_CELL_ID_PRESENT == p_rrm_gl_cell_id->bitmask)
	{
		p_son_gl_cell_id->bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

		SON_MEMCPY(&p_son_gl_cell_id->intra_rat_global_cell_id,
				&p_rrm_gl_cell_id->intra_rat_global_cell_id,
				sizeof(son_intra_rat_global_cell_id_t));
	}
	else if (RRM_INTER_RAT_GLOBAL_CELL_ID_PRESENT == p_rrm_gl_cell_id->bitmask)
	{
		p_son_gl_cell_id->bitmask |= SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;
		convert_rrm_inter_rat_cell_id_to_son_inter_rat_cell_id(
				&p_son_gl_cell_id->inter_rat_global_cell_id,
				&p_rrm_gl_cell_id->inter_rat_global_cell_id); 
	}

	SON_UT_TRACE_EXIT();
} /* convert_rrm_gl_cell_id_to_son_gl_cell_id */

/******************************************************************************
 * Function Name  : determine_the_suspecion_deletion_of_nr
 * Inputs         : p_eutran_nbr    - Pointer to hash record
 *                  p_cell_ctxt  - Pointer to ANR cell context
 * Outputs        : None
 * Returns        : son_void_t      - None
 * Description    : Determine the Suspecision or the deletion of the neighbor
 *****************************************************************************/
son_void_t
determine_the_suspecion_deletion_of_nr
(
 anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr,
 anr_cell_context_t   *p_cell_ctxt
 )
{
	/* Coverity CID-55063 Fix Starts */
	if (p_eutran_nbr == SON_PNULL || p_cell_ctxt == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_eutran_nbr or p_cell_ctxt is NULL"
				);
		SON_UT_TRACE_EXIT();
		return;
	}

	anr_pci_sus_conf_node_t  *p_pci_sus_conf_node = SON_PNULL;

	/* Coverity Fix: 24965,24963 */
	son_anr_updated_nrt_info_t *p_sus_pci_oam_notif = SON_PNULL;
	anr_intra_rat_hash_rec_t   hash_rec;

	/* Coverity Fix: 24965, 24966 */
	son_anr_rrm_updated_nrt_info_t  *p_rrm_notification = SON_PNULL;

	/* Coverity Fix: 24965 */
	son_anr_updated_nrt_info_t      *p_delete_oam_notif = SON_PNULL;
	son_x2_nrt_update_ind_t   nrt_upd_ind = {0};

	SON_UT_TRACE_ENTER(); 

	p_sus_pci_oam_notif = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_sus_pci_oam_notif)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_rrm_notification = (son_anr_rrm_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));

	if (SON_PNULL == p_rrm_notification)
	{
		if (SON_PNULL != p_sus_pci_oam_notif)
		{
			son_mem_free(p_sus_pci_oam_notif);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	}

	p_delete_oam_notif = (son_anr_updated_nrt_info_t *)
		son_mem_get(sizeof(son_anr_updated_nrt_info_t));

	if (SON_PNULL == p_delete_oam_notif)
	{
		if (SON_PNULL != p_sus_pci_oam_notif)
		{
			son_mem_free(p_sus_pci_oam_notif);
		}

		if (SON_PNULL != p_rrm_notification)
		{
			son_mem_free(p_rrm_notification);
		}

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation failure");
		SON_UT_TRACE_EXIT();
		return;
	} 

	SON_MEMSET(p_sus_pci_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));
	/* Coverity Fix: 24965 */
	SON_MEMCPY(&p_sus_pci_oam_notif->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	p_sus_pci_oam_notif->nrt_op = SON_NRT_OP_UPDATE;

	SON_MEMSET(&hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
	SON_MEMCPY(&hash_rec.neighbor_info.nbr_cgi,
			&p_eutran_nbr->data.neighbor_info.nbr_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_rrm_notification, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
	SON_MEMCPY(&p_rrm_notification->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	SON_MEMSET(p_delete_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));
	SON_MEMCPY(&p_delete_oam_notif->src_cgi, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t)); 

	/*Get the suspect PCI node*/
	p_pci_sus_conf_node = anr_find_in_sus_pci_ctxt_list(
			p_cell_ctxt, p_eutran_nbr->data.neighbor_info.pci,
			p_eutran_nbr->data.neighbor_info.earfcn);

	/* Autonomous neighbor removal if Failed HO > Threshold */
	if (SON_NR_UNLOCKED == p_eutran_nbr->data.neighbor_info.nr_status &&
			(SON_FAILED_HO_THRESHOLD_PRESENT & p_cell_ctxt->removal_criteria.bitmask) &&
			p_cell_ctxt->removal_criteria.handover_failure_threshold &&
			(p_cell_ctxt->removal_criteria.handover_failure_threshold <=
			 p_eutran_nbr->data.ho_failed_for_nr_removal))
	{
		son_error_et error_code = SON_NO_ERROR;
		anr_intra_rat_del_nr(p_cell_ctxt,
				&p_eutran_nbr->data.neighbor_info.nbr_cgi,
				SON_TRUE,
				&error_code,
				&hash_rec,
				p_delete_oam_notif,
				p_rrm_notification,
				SON_TRUE);

		if (SON_NO_ERROR == error_code)
		{
			anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
					&p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
		}
	}

	else if (SON_PNULL == p_pci_sus_conf_node &&
			(SON_SUSPECT_PCI_THRESHOLD_PRESENT & p_cell_ctxt->pci_confusion_cfg.bitmask) &&
			p_cell_ctxt->pci_confusion_cfg.suspect_pci_threshold &&
			(p_cell_ctxt->pci_confusion_cfg.suspect_pci_threshold <=
			 p_eutran_nbr->data.ho_failed_for_pci_conflict))
	{
		anr_configure_suspect_pci(p_cell_ctxt, p_eutran_nbr, p_sus_pci_oam_notif);

		/*Report the suspect pci filed change to OAM*/
		anr_send_updated_ncl_to_oam(p_sus_pci_oam_notif);
	}

	/*Send the msg to OAM*/
	anr_send_updated_ncl_to_oam(p_delete_oam_notif);

	/*Send the msg to MLB*/
	if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->cell_id))
	{
		anr_send_updated_ncl_to_mlb(p_delete_oam_notif);
	}

	/*Send the msg to RRM*/
	anr_send_updated_ncl_to_rrm(p_rrm_notification,
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));

	if (SON_PNULL != p_sus_pci_oam_notif)
	{
		son_mem_free(p_sus_pci_oam_notif);
	}

	if (SON_PNULL != p_rrm_notification)
	{
		son_mem_free(p_rrm_notification);
	}

	if (SON_PNULL != p_delete_oam_notif)
	{
		son_mem_free(p_delete_oam_notif);
	}

    /* Spr 17753 Changes Start */
      anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
    /* Spr 17753 Changes End */
	SON_UT_TRACE_EXIT();
	/* Coverity CID-55063 Fix Ends */
} /* determine_the_suspecion_deletion_of_nr */

/******************************************************************************
 * Function Name  : anr_fsm_rrm_ho_failure_report_handler
 * Inputs         : p_cspl_hdr       - Pointer to cspl header
 *                  p_cell_ctxt   - Pointer to ANR cell context
 *                  evnt_id          - Event id
 *                  p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler to handle RRM SON HO failure report message.
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_rrm_ho_failure_report_handler
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t   *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8              *p_msg
 )
{
	son_rrm_ho_failure_report_t         *p_ind                   = SON_PNULL;
	SON_HASH_NODE                       *p_node                  = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t     *p_tpl                     = SON_PNULL;
	anr_intra_rat_hash_rec_t          hash_rec;
	anr_pci_sus_conf_node_t  *p_pci_sus_conf_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL || p_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr, p_cell_ctxt or p_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
        /* SPR_20915 Fix - Starts */
        return RELEASE_SON_BUFFER; 
        /* SPR_20915 Fix - Ends */
	}

	/* Klockwork 2.1.3 */
	p_ind = (son_rrm_ho_failure_report_t *)p_msg;
	SON_MEMSET(&hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));
	if (SON_PNULL != p_ind)
	{
		SON_MEMCPY(&hash_rec.neighbor_info.nbr_cgi, &p_ind->trgt_cgi,
				sizeof(son_intra_rat_global_cell_id_t));

		if (SON_PNULL != (p_node = hash_find(&p_cell_ctxt->intra_rat_neighbor_table,
						(son_void_t *)&hash_rec)))
		{
			p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
				YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);

			p_pci_sus_conf_node = anr_find_in_sus_pci_ctxt_list(
					p_cell_ctxt, p_tpl->data.neighbor_info.pci,
					p_tpl->data.neighbor_info.earfcn); 

			/*Handover failed*/
			if (SON_MAX_U8_VAL > p_tpl->data.neighbor_info.handover_stats.handovers_failed)
			{
				p_tpl->data.neighbor_info.handover_stats.handovers_failed++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_tpl->data.neighbor_info.nbr_cgi.cell_identity));
			}

			/*Handover failed for NR removal*/
			if (SON_MAX_U8_VAL > p_tpl->data.ho_failed_for_nr_removal)
			{
				p_tpl->data.ho_failed_for_nr_removal++;
			}
			else
			{
				SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
						"Neighbor Cell"
						" [Cell Id: 0x%x], Max Ho Failed NR removal count reached! Counter not incremented",
						son_convert_char_cell_id_to_int_cell_id(p_tpl->data.neighbor_info.nbr_cgi.cell_identity));
			}

			if (SON_PNULL == p_pci_sus_conf_node &&
					p_ind->ho_cause == RRM_HO_TOO_EARLY && 
					IDX_RRM_HO_FAILURE_REPORT == evnt_id &&
					SON_HO_ALLOWED == p_tpl->data.neighbor_info.ho_status &&
					SON_TRUE == is_intra_rat_nbr_notifiable_to_rrm(&p_tpl->data.neighbor_info))
			{
				if (SON_MAX_U8_VAL > p_tpl->data.ho_failed_for_pci_conflict)
				{
					p_tpl->data.ho_failed_for_pci_conflict++;
				}
				else
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
							"Neighbor Cell"
							" [Cell Id: 0x%x], Max Ho Failed PCI Confusion count reached! Counter not incremented",
							son_convert_char_cell_id_to_int_cell_id(p_tpl->data.neighbor_info.nbr_cgi.cell_identity));
				}
			}

			determine_the_suspecion_deletion_of_nr(p_tpl, p_cell_ctxt);
			/*This tpl can get deleted*/
            p_tpl = SON_PNULL;
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
					"Neighbor is not identified");
		}
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"failure_report contains NULL pointer ");
	}
	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER; 
} /* anr_fsm_rrm_ho_failure_report_handler */

/******************************************************************************
 * Function Name  : anr_fsm_rrm_ho_report_handler
 * Inputs         : p_cspl_hdr       - Pointer to cspl header
 *                  p_cell_context   - Pointer to ANR cell context
 *                  evnt_id          - Event id
 *                : p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler to handle RRM_SON_HO_REPORT message.
 *****************************************************************************/
static son_buf_retain_status_et
anr_fsm_rrm_ho_report_handler
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t   *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8              *p_msg
 )
{
	son_rrm_ho_report_t *p_ind = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;
	son_rat_type_et rat_type = SON_EUTRAN_CELL;
	son_void_t	*p_nr = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr = SON_PNULL;
	anr_inter_rat_hash_nr_tuple_t *p_utran_nbr = SON_PNULL; /*SPR-8950*/
    /* SPR 12542/SES-483 Fix Start */
	anr_inter_rat_hash_nr_tuple_t *p_geran_nbr = SON_PNULL;
    /* SPR 12542/SES-483 Fix End */
	son_global_cell_id_t glbl_cell_id;


	SON_UT_TRACE_ENTER();

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr, p_cell_ctxt or p_msg is NULL evnt_id [%u]"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	/* Klockwork 2.1.3 */
	p_ind = (son_rrm_ho_report_t *)p_msg;
	SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
	convert_rrm_gl_cell_id_to_son_gl_cell_id(&glbl_cell_id, &p_ind->trgt_cgi);
	p_nr = anr_find_nr(p_cell_ctxt, (son_void_t *)&glbl_cell_id, &error_code, &rat_type);

	/*Klockwork fix*/
	if (SON_NO_ERROR == error_code && SON_PNULL != p_nr)
	{
		switch (rat_type)
		{
			case SON_EUTRAN_CELL:
				{
					p_eutran_nbr = ((anr_intra_rat_hash_nr_tuple_t *)p_nr);
					anr_process_eutran_ho_report(p_cell_ctxt,
							p_eutran_nbr, p_ind);
					/*neighbor could be deleted as well*/
                p_eutran_nbr = SON_PNULL;
					break;
				}
				/*SPR-8950-Start*/
			case SON_UTRAN_CELL:
				{
					p_utran_nbr = ((anr_inter_rat_hash_nr_tuple_t *)p_nr);
					anr_process_utran_ho_report(p_cell_ctxt,
							p_utran_nbr, p_ind);
					/*neighbor could be deleted as well*/
                p_utran_nbr = SON_PNULL;
					break;
				}
				/*SPR-8950-End*/
                /* SPR 12542/SES-483 Fix Start */
			case SON_GERAN_CELL:
				{
					p_geran_nbr = ((anr_inter_rat_hash_nr_tuple_t *)p_nr);
					anr_process_geran_ho_report(p_cell_ctxt,
							p_geran_nbr, p_ind);
					/*neighbor could be deleted as well*/
                    p_geran_nbr = SON_PNULL;
					break;
				}
                /* SPR 12542/SES-483 Fix End */

			default:
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
							"INTER RAT Handover not available");
					break;
				}
		} /* switch */
	}

	anr_print_cell_context(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_rrm_ho_report_handler */

/******************************************************************************
 * Function Name  : anr_fsm_rrm_ho_attempts_ind_handler
 * Inputs         : p_cspl_hdr       - Pointer to cspl header
 *                  p_cell_context   - Pointer to ANR cell context
 *                  evnt_id          - Event id
 *                : p_msg            - Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain  the buffer
 * Description    : Event Handler to handle RRM_SON_HO_REPORT message.
 *****************************************************************************/
static
son_buf_retain_status_et
anr_fsm_rrm_ho_attempts_ind_handler
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t   *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8              *p_msg
 )
{
	son_rrm_ho_attempt_ind_t *p_ho_attempts = (son_rrm_ho_attempt_ind_t *)p_msg;
	anr_intra_rat_hash_rec_t  hash_rec;
	SON_HASH_NODE                     *p_node                  = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t     *p_tpl                   = SON_PNULL;


	SON_UT_TRACE_ENTER(); 

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL || p_msg == SON_PNULL)
	{
        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr, p_cell_ctxt or p_msg is NULL evnt_id %u"
				,evnt_id);
        /*SPR 17777 +-*/ 
		SON_UT_TRACE_EXIT();
        /* SPR_20915 Fix - Starts */
        return RELEASE_SON_BUFFER;
        /* SPR_20915 Fix - Ends */
	}

	SON_MEMSET(&hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));

	SON_MEMCPY(&hash_rec.neighbor_info.nbr_cgi, &p_ho_attempts->trgt_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	p_node = hash_find(&p_cell_ctxt->intra_rat_neighbor_table, &hash_rec);
	p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
		YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_node);
	/*If the neighbor cell was send to RRM and was HO Allowed neighbor cell*/ 
	if (SON_PNULL != p_node &&
			SON_TRUE == is_intra_rat_nbr_notifiable_to_rrm(&p_tpl->data.neighbor_info) &&
			SON_HO_ALLOWED == p_tpl->data.neighbor_info.ho_status)
	{
		/*Set the time stamp*/
		son_time(&p_tpl->data.last_activity_ts);

		determine_the_suspecion_deletion_of_nr(p_tpl, p_cell_ctxt);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF, 
				"The neighbor cell is either not available "
				"or was not send to RRM or was not HO Allowed");
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* anr_fsm_rrm_ho_attempts_ind_handler */

/*****************************************************************************
 * Function Name  : anr_fsm_meas_config_obj_remove_req
 * Inputs         : p_cspl_hdr      Pointer to CSPl header
 *                  p_cell_ctxt     Pointer to ANR cell context
 *                  evnt_id         Unique Identifier of the message
 *                  p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ. It is invoked when
 *                  the anr state of a cell is ANR_CELL_STATE_ENABLED. This
 *                  function is used to parse the meas object removal req received from
 *                  OAM and populate the meas object removal req for RRM which it uses
 *                  to delete the frequencies
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_meas_config_obj_remove_req
(
 STACKAPIHDR         *p_cspl_hdr,
 anr_cell_context_t  *p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8              *p_msg
 )
{
	son_anr_meas_config_obj_remove_req_t *p_req = (son_anr_meas_config_obj_remove_req_t *)p_msg;
	son_rrm_meas_config_obj_remove_req_t rrm_req = {0};
	son_error_et                         error_code = SON_NO_ERROR;
	son_u8 index;

	if (!p_req->remove_req.earfcn_remove_list_size &&
			!p_req->remove_req.uarfcn_fdd_remove_list_size &&
			!p_req->remove_req.uarfcn_tdd_remove_list_size &&
			!p_req->remove_req.geran_remove_list_size)
	{
        /*SPR 17777 +-*/ 
		error_code = SON_ERR_INVALID_PARAMS;
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Empty Measurement Object Removal "
				"Request received from MIF for cell [Cell Id: 0x%x] evnt_id [%u] p_cspl_hdr [%p]",
				son_convert_char_cell_id_to_int_cell_id(p_req->remove_req.src_cgi.cell_identity),
                evnt_id,p_cspl_hdr);
        /*SPR 17777 +-*/ 
	}
	else if(SON_PNULL != p_cell_ctxt->p_meas_config_obj_remove)
	{
		error_code = SON_ERR_UNEXPECTED_MSG_IN_ACTIVE_STATE;
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
				"MEAS OBJ REMOVE REQ already in progress"
				" and sent to RRM ");
	}
	if(SON_NO_ERROR != error_code)
	{
		son_anr_meas_config_obj_remove_res_t oam_res = {0};
		oam_res.transaction_id = p_req->transaction_id;
		oam_res.remove_res.result = SON_FAILURE;
		oam_res.remove_res.error_code = error_code;

		SON_MEMCPY(&oam_res.remove_res.src_cgi,
				&p_req->remove_req.src_cgi,
				sizeof(son_rrm_intra_rat_global_cell_id_t));

		son_create_send_buffer((son_u8 *)&oam_res, SON_ANR_MODULE_ID,
				SON_MIF_MODULE_ID, SONANR_MEAS_CONFIG_OBJ_REMOVE_RES,
				sizeof(son_anr_meas_config_obj_remove_res_t));

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}


	rrm_req.transaction_id = p_req->transaction_id;

	SON_MEMCPY(&rrm_req.remove_req.cell_id, &p_req->remove_req.src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Store the frequencies to delete the nbrs on receipt of Success resp from RRM */
	p_cell_ctxt->p_meas_config_obj_remove = (son_rrm_meas_config_obj_remove_req_t *)
		son_mem_get(sizeof(son_rrm_meas_config_obj_remove_req_t));

	if(SON_PNULL == p_cell_ctxt->p_meas_config_obj_remove)
	{

		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Memory allocation for son_rrm_meas_config_obj_remove_req_t fails");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	if (p_req->remove_req.earfcn_remove_list_size)
	{
		rrm_req.remove_req.earfcn_remove_list_size =
			p_req->remove_req.earfcn_remove_list_size;

		for (index = 0; index < p_req->remove_req.earfcn_remove_list_size; index++)
		{
			rrm_req.remove_req.earfcn_remove_list[index] =
				p_req->remove_req.earfcn_remove_list[index];
		}
	}

	if (p_req->remove_req.uarfcn_fdd_remove_list_size)
	{
		rrm_req.remove_req.uarfcn_fdd_remove_list_size =
			p_req->remove_req.uarfcn_fdd_remove_list_size;

		for (index = 0; index < p_req->remove_req.uarfcn_fdd_remove_list_size; index++)
		{
			rrm_req.remove_req.uarfcn_fdd_remove_list[index] =
				p_req->remove_req.uarfcn_fdd_remove_list[index];
		}
	}

	if (p_req->remove_req.uarfcn_tdd_remove_list_size)
	{
		rrm_req.remove_req.uarfcn_tdd_remove_list_size =
			p_req->remove_req.uarfcn_tdd_remove_list_size;

		for (index = 0; index < p_req->remove_req.uarfcn_tdd_remove_list_size; index++)
		{
			rrm_req.remove_req.uarfcn_tdd_remove_list[index] =
				p_req->remove_req.uarfcn_tdd_remove_list[index];
		}
	}

	if (p_req->remove_req.geran_remove_list_size)
	{
		rrm_req.remove_req.geran_remove_list_size =
			p_req->remove_req.geran_remove_list_size;

		for (index = 0; index < p_req->remove_req.geran_remove_list_size; index++)
		{
			SON_MEMCPY(&rrm_req.remove_req.geran_remove_list,
					&p_req->remove_req.geran_remove_list,
					sizeof(son_geran_carrier_freqs_t));
		}
	}

	SON_MEMCPY(p_cell_ctxt->p_meas_config_obj_remove, &rrm_req,
			sizeof(son_rrm_meas_config_obj_remove_req_t));

	/*Invoke PUP tool call to send message to RRM*/
	if (RRM_FAILURE == rrm_son_send_rrm_son_meas_config_obj_remove_req(
				&rrm_req.remove_req,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				p_req->transaction_id,
				SON_NULL))
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Meas object removal request could not"
				" be sent to RRM for cell [Cell Id: 0x%x]",
				son_convert_char_cell_id_to_int_cell_id((p_req->remove_req.src_cgi.cell_identity)));

		son_mem_free(p_cell_ctxt->p_meas_config_obj_remove);
		p_cell_ctxt->p_meas_config_obj_remove = SON_PNULL;
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"Message %u send from %u to"
				" %u for cell [Cell Id: 0x%x]",
				RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ,
				SON_ANR_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id((p_req->remove_req.src_cgi.cell_identity)));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : anr_fsm_nack_meas_config_obj_remove_req
 * Inputs         : p_cspl_hdr      Pointer to CSPl header
 *                  p_cell_ctxt     Pointer to ANR cell context
 *                  evnt_id         Unique Identifier of the message
 *                  p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ message received
 *                  in invalid state (cell is in state other than
 *                  ANR_CELL_STATE_ENABLED). This function sends a negative
 *                  response to OAM with error cause as SON_ERR_UNEXPECTED_MSG.
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_nack_meas_config_obj_remove_req
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *  p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8* p_msg
 )
{
	son_anr_meas_config_obj_remove_req_t *p_req = (son_anr_meas_config_obj_remove_req_t *)p_msg;
	son_anr_meas_config_obj_remove_res_t oam_res = {0};
	SON_UT_TRACE_ENTER();

	oam_res.transaction_id = p_req->transaction_id;
	oam_res.remove_res.result = SON_FAILURE;
	oam_res.remove_res.error_code = SON_ERR_UNEXPECTED_MSG;
	SON_MEMCPY(&oam_res.remove_res.src_cgi,
			&p_req->remove_req.src_cgi,
			sizeof(son_rrm_intra_rat_global_cell_id_t));

	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
			SON_ERROR,
			"Event %s "
			"has occurred in %s state for the cell [Cell Id: 0x%x]"
			"Received Message %d from %d",
			SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->api,p_cspl_hdr->from);

	son_create_send_buffer((son_u8 *)&oam_res, SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID, SONANR_MEAS_CONFIG_OBJ_REMOVE_RES,
			sizeof(son_anr_meas_config_obj_remove_res_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : anr_validate_and_populate_following_arfcn
 * Inputs         : arfcn,band_ind,p_geran_carrier_freq_arr,geran_carrier_freq_arr_size
 * Outputs        : None
 * Returns        : son_u16
 * Description    : This function validates and inserts the arfcn and band_ind in to the list of GERAN carrier frequencies whom   neighbors are to be deleted from the NRT
 ******************************************************************************/
son_u16
anr_validate_and_populate_following_arfcn
(
 son_u16                            arfcn,
 son_geran_band_indicator_et        band_ind,
 anr_geran_meas_obj_remove_data_t   *p_geran_carrier_freq_arr,
 son_u16                            geran_carrier_freq_arr_size
 )
{
	son_u16         idx     = 0;
	son_bool_et     is_duplicate = SON_FALSE;

	for (; idx < geran_carrier_freq_arr_size; idx++)
	{
		if ((p_geran_carrier_freq_arr[idx].arfcn == arfcn) &&
				(p_geran_carrier_freq_arr[idx].band_ind == band_ind))
		{
			is_duplicate = SON_TRUE;
			break;
		}
	}

	if (SON_FALSE == is_duplicate)
	{
		p_geran_carrier_freq_arr[geran_carrier_freq_arr_size].arfcn = arfcn;
		p_geran_carrier_freq_arr[geran_carrier_freq_arr_size].band_ind = band_ind;
		geran_carrier_freq_arr_size ++;
	}

	return geran_carrier_freq_arr_size;
}

/*****************************************************************************
 * Function Name  : anr_populate_geran_carrier_freqs
 * Inputs         : p_in_geran_carrier_freq_arr,geran_carrier_freq_list_size,p_out_geran_carrier_freq_arr
 * Outputs        : None
 * Returns        : son_u8
 * Description    : This function populates the GERAN carrier frequencies whom neighbors are to be deleted from the NRT 
 ******************************************************************************/
son_u8
anr_populate_geran_carrier_freqs
(
 rrm_son_geran_carrier_freqs_t      *p_in_geran_carrier_freq_arr,
 son_u16                            geran_carrier_freq_list_size,
 anr_geran_meas_obj_remove_data_t   *p_out_geran_carrier_freq_arr
 )
{
	son_u8  idx1, idx2, idx3; 
	son_u8  geran_arr_size = 0;
	/* Coverity CID-61361, 61362, 61363 Fix Starts */
	son_geran_band_indicator_et band_ind = SON_GERAN_DCS_1800;
	/* Coverity CID-61361, 61362, 61363 Fix Ends */

	SON_UT_TRACE_ENTER();

	for (idx1 = 0; idx1 < geran_carrier_freq_list_size; idx1++)
	{
		/* Coverity CID-61361, 61362, 61363 Fix Starts */
		band_ind = anr_map_rrm_band_ind(
				__func__, p_in_geran_carrier_freq_arr[idx1].band_ind);
		/* Coverity CID-61361, 61362, 61363 Fix Ends */

		geran_arr_size = anr_validate_and_populate_following_arfcn(
				p_in_geran_carrier_freq_arr[idx1].starting_arfcn,
				/* Coverity CID-61363 Fix Starts */
				band_ind,
				/* Coverity CID-61363 Fix Ends */
				p_out_geran_carrier_freq_arr,
				geran_arr_size);

		/* Copying the following arfcns into the array */ 
		if (SON_MC_GERAN_EXP_ARFCN_SET_PRESENT == 
				p_in_geran_carrier_freq_arr->following_arfcns.bitmask)
		{
			for (idx2 = 0; idx2 < p_in_geran_carrier_freq_arr->following_arfcns.exp_arfcn_list_size;
					idx2 ++)
			{
				geran_arr_size = anr_validate_and_populate_following_arfcn(
						p_in_geran_carrier_freq_arr[idx1].following_arfcns.exp_arfcn_list[idx2],
						/* Coverity CID-61362 Fix Starts */
						band_ind,
						/* Coverity CID-61362 Fix Ends */
						p_out_geran_carrier_freq_arr,
						geran_arr_size);
			} 
		}
		else if (SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT == 
				p_in_geran_carrier_freq_arr->following_arfcns.bitmask)
		{
			for (idx2 = 0; idx2 < p_in_geran_carrier_freq_arr->following_arfcns.num_of_following_arfcn;
					idx2++)
			{
				son_u16 arfcn;
				arfcn = (p_in_geran_carrier_freq_arr[idx1].starting_arfcn +
						(idx2 * p_in_geran_carrier_freq_arr->following_arfcns.arfcn_spacing)) %
					MAX_NUM_OF_ALLOWED_GERAN_FREQ;

				geran_arr_size = anr_validate_and_populate_following_arfcn(
						arfcn,
						/* Coverity CID-61362 Fix Starts */
						band_ind,
						/* Coverity CID-61362 Fix Ends */
						p_out_geran_carrier_freq_arr,
						geran_arr_size);
			} 
		}
		else if (SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT == 
				p_in_geran_carrier_freq_arr->following_arfcns.bitmask)
		{
			for (idx2 = 1; idx2 <= p_in_geran_carrier_freq_arr->following_arfcns.arfcn_bmp_list_size;
					idx2++)
			{
				for (idx3 = 0; idx3 < 8; idx3++)
				{
					if (p_in_geran_carrier_freq_arr->following_arfcns.arfcn_bmp_list[idx2] & (2^idx3))
					{
						son_u16 arfcn;
						arfcn = (p_in_geran_carrier_freq_arr[idx1].starting_arfcn +
								(idx2 * (idx3+1))) % MAX_NUM_OF_ALLOWED_GERAN_FREQ;

						geran_arr_size = anr_validate_and_populate_following_arfcn(
								arfcn,
								/* Coverity CID-61361 Fix Starts */
								band_ind,
								/* Coverity CID-61361 Fix Ends */
								p_out_geran_carrier_freq_arr,
								geran_arr_size);
					}
				}
			} 
		}
		else
		{
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"Meas object removal"
					" request for GERAN contains invalid bitmask[0x%x] following ARFCNS",
                    p_in_geran_carrier_freq_arr->following_arfcns.bitmask);
		}
	}

	SON_UT_TRACE_EXIT();
	return geran_arr_size; 
}

/*****************************************************************************
 * Function Name  : anr_delete_nbr_for_meas_obj_remove
 * Inputs         : p_cell_ctxt - Pointer to Cell context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function deletes the nbrs on frequencies requested by
 *                  OAM for Meas Obj Removal
 *****************************************************************************/
son_void_t
anr_delete_nbr_for_meas_obj_remove
(
 anr_cell_context_t *  p_cell_ctxt
 )
{
	son_u8                              index                   = SON_NULL;
	anr_intra_rat_hash_nr_tuple_t       *p_intra_tpl            = SON_PNULL;
	anr_inter_rat_hash_nr_tuple_t       *p_inter_tpl            = SON_PNULL;
	SON_HASH                            *p_hash                 = SON_PNULL;
	anr_intra_rat_hash_rec_t            *p_intra_rat_hash_rec   = SON_PNULL;
	anr_inter_rat_hash_rec_t            *p_inter_rat_hash_rec   = SON_PNULL;
	son_error_et                        error                   = SON_NO_ERROR;
	son_return_et                       del_status              = SON_SUCCESS;
	son_bool_et                         is_upd_ind_rqd          = SON_FALSE;
	anr_intra_rat_hash_rec_t            intra_rat_hash_rec;
	anr_inter_rat_hash_rec_t            inter_rat_hash_rec;
	son_x2_nrt_update_ind_t             nrt_upd_ind = {0};
	son_inter_rat_global_cell_id_t      inter_rat_global_cell_id;
	anr_geran_meas_obj_remove_data_t    *p_geran_meas_obj_remove_data = SON_PNULL;
	son_u8                              geran_meas_obj_remove_data_size;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&intra_rat_hash_rec,0,sizeof(anr_intra_rat_hash_rec_t));
	SON_MEMSET(&inter_rat_hash_rec,0,sizeof(anr_inter_rat_hash_rec_t));

	/* Removing EUTRAN Neighbors from the NRT */
	for(index = SON_NULL; index < p_cell_ctxt->p_meas_config_obj_remove->remove_req.earfcn_remove_list_size;
			index++)
	{
		p_hash = &p_cell_ctxt->intra_rat_neighbor_table;
		p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(p_hash);

		while (SON_PNULL != p_intra_tpl)
		{
			if ((p_intra_tpl->data.neighbor_info.bitmask & SON_EARFCN_PRESENT) &&
					p_intra_tpl->data.neighbor_info.earfcn ==
					p_cell_ctxt->p_meas_config_obj_remove->remove_req.earfcn_remove_list[index])
			{
				/* Remove the nbr */
				p_intra_rat_hash_rec = &intra_rat_hash_rec;

				del_status = anr_intra_rat_del_nr(
						p_cell_ctxt,
						&p_intra_tpl->data.neighbor_info.nbr_cgi,
						SON_FALSE,
						&error, p_intra_rat_hash_rec, SON_PNULL, SON_PNULL, SON_TRUE);

				if( SON_SUCCESS == del_status )
				{
					is_upd_ind_rqd = SON_TRUE;
				}
			}

			p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
					p_hash, &p_intra_tpl->anchor);
		}
	}

	/* Removing UTRAN FDD Neighbors from the NRT */
	p_inter_rat_hash_rec = &inter_rat_hash_rec;

	for(index = SON_NULL; index < p_cell_ctxt->p_meas_config_obj_remove->remove_req.uarfcn_fdd_remove_list_size;
			index++)
	{
		p_hash = &p_cell_ctxt->inter_rat_neighbor_table;
		p_inter_tpl  = anr_get_first_nr_from_inter_rat_hash_table(p_hash);

		while (SON_PNULL != p_inter_tpl)
		{
			if ((p_inter_tpl->data.u.utran_neighbor_info.bitmask & SON_UTRAN_UARFCN_PRESENT) &&
					(p_inter_tpl->data.u.utran_neighbor_info.uarfcn ==
					 p_cell_ctxt->p_meas_config_obj_remove->remove_req.uarfcn_fdd_remove_list[index]) &&
					(SON_PCI_FDD_PRESENT == p_inter_tpl->data.u.utran_neighbor_info.pci.bitmask))
			{
				/* Remove the nbr */
				SON_MEMSET(&inter_rat_hash_rec,0,sizeof(anr_inter_rat_hash_rec_t));
				SON_MEMSET(&inter_rat_global_cell_id,0,sizeof(son_inter_rat_global_cell_id_t));
				inter_rat_global_cell_id.bitmask = SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

				SON_MEMCPY(&inter_rat_global_cell_id.utran_geran_global_cell_id,
						&p_inter_tpl->data.u.utran_neighbor_info.nbr_cgi,
						sizeof(son_utran_geran_global_cell_id_t));

				del_status = anr_inter_rat_del_nr(
						p_cell_ctxt,
						&inter_rat_global_cell_id,
						SON_FALSE,
						&error, p_inter_rat_hash_rec, SON_PNULL, SON_PNULL);

				if( SON_SUCCESS == del_status )
				{
					is_upd_ind_rqd = SON_TRUE;
				}
			}

			p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
					p_hash, &p_inter_tpl->anchor);
		}
	}

	/* Removing UTRAN TDD Neighbors from the NRT */
	for(index = SON_NULL; index < p_cell_ctxt->p_meas_config_obj_remove->remove_req.uarfcn_tdd_remove_list_size;
			index++)
	{
		p_hash = &p_cell_ctxt->inter_rat_neighbor_table;
		p_inter_tpl  = anr_get_first_nr_from_inter_rat_hash_table(p_hash);

		while (SON_PNULL != p_inter_tpl)
		{
			if ((p_inter_tpl->data.u.utran_neighbor_info.bitmask & SON_UTRAN_UARFCN_PRESENT) &&
					(p_inter_tpl->data.u.utran_neighbor_info.uarfcn ==
					 p_cell_ctxt->p_meas_config_obj_remove->remove_req.uarfcn_tdd_remove_list[index]) &&
					(SON_PCI_TDD_PRESENT == p_inter_tpl->data.u.utran_neighbor_info.pci.bitmask))
			{
				/* Remove the nbr */
				SON_MEMSET(&inter_rat_hash_rec,0,sizeof(anr_inter_rat_hash_rec_t));
				SON_MEMSET(&inter_rat_global_cell_id,0,sizeof(son_inter_rat_global_cell_id_t));
				inter_rat_global_cell_id.bitmask = SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

				SON_MEMCPY(&inter_rat_global_cell_id.utran_geran_global_cell_id,
						&p_inter_tpl->data.u.utran_neighbor_info.nbr_cgi,
						sizeof(son_utran_geran_global_cell_id_t));

				del_status = anr_inter_rat_del_nr(
						p_cell_ctxt,
						&inter_rat_global_cell_id,
						SON_FALSE,
						&error, p_inter_rat_hash_rec, SON_PNULL, SON_PNULL);

				if( SON_SUCCESS == del_status )
				{
					is_upd_ind_rqd = SON_TRUE;
				}
			}

			p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
					p_hash, &p_inter_tpl->anchor);
		}
	}

	/* Removing GERAN Neighbors from the NRT */
	if (p_cell_ctxt->p_meas_config_obj_remove->remove_req.geran_remove_list_size)
	{
		p_geran_meas_obj_remove_data = (anr_geran_meas_obj_remove_data_t *)
			son_mem_get((sizeof(anr_geran_meas_obj_remove_data_t)) * 
					MAX_NUM_OF_ALLOWED_GERAN_FREQ * MAX_NUM_OF_GERAN_BAND_INDICATOR);

		if (SON_PNULL != p_geran_meas_obj_remove_data)
		{
			/* Populate GERAN measurement frequency delete list */
			geran_meas_obj_remove_data_size = anr_populate_geran_carrier_freqs(
					p_cell_ctxt->p_meas_config_obj_remove->remove_req.geran_remove_list,
					p_cell_ctxt->p_meas_config_obj_remove->remove_req.geran_remove_list_size,
					p_geran_meas_obj_remove_data);

			for (index = 0; index < geran_meas_obj_remove_data_size; index ++)
			{
				p_hash = &p_cell_ctxt->inter_rat_neighbor_table;
				p_inter_tpl  = anr_get_first_nr_from_inter_rat_hash_table(p_hash);

				while (SON_PNULL != p_inter_tpl)
				{
					if ((p_inter_tpl->data.u.geran_neighbor_info.bitmask & SON_GERAN_ARFCN_BAND_IND_PRESENT) &&
							(p_inter_tpl->data.u.geran_neighbor_info.band_ind == 
							 p_geran_meas_obj_remove_data[index].band_ind) &&
							(p_inter_tpl->data.u.geran_neighbor_info.arfcn ==
							 p_geran_meas_obj_remove_data[index].arfcn))
					{
						/* Remove the nbr */
						SON_MEMSET(&inter_rat_hash_rec,0,sizeof(anr_inter_rat_hash_rec_t));
						SON_MEMSET(&inter_rat_global_cell_id,0,sizeof(son_inter_rat_global_cell_id_t));
						inter_rat_global_cell_id.bitmask = SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

						SON_MEMCPY(&inter_rat_global_cell_id.utran_geran_global_cell_id,
								&p_inter_tpl->data.u.utran_neighbor_info.nbr_cgi,
								sizeof(son_utran_geran_global_cell_id_t));

						del_status = anr_inter_rat_del_nr(
								p_cell_ctxt,
								&inter_rat_global_cell_id,
								SON_FALSE,
								&error, p_inter_rat_hash_rec, SON_PNULL, SON_PNULL);

						if( SON_SUCCESS == del_status )
						{
							is_upd_ind_rqd = SON_TRUE;
						}
					}

					p_inter_tpl = anr_get_next_nr_from_inter_rat_hash_table(
							p_hash, &p_inter_tpl->anchor);
				}
			}
		}
	}

	if (SON_PNULL != p_geran_meas_obj_remove_data)
	{
		son_mem_free(p_geran_meas_obj_remove_data);
	}

	if(SON_TRUE == is_upd_ind_rqd)
	{
		anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
				&p_cell_ctxt->cell_id,SON_NRT_OP_UPDATE);
	}

	anr_print_cell_context(p_cell_ctxt);

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_fsm_meas_config_obj_remove_res
 * Inputs         : p_cspl_hdr      Pointer to CSPl header
 *                  p_cell_ctxt     Pointer to ANR cell context
 *                  evnt_id         Unique Identifier of the message
 *                  p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for RRM_MEAS_CONFIG_OBJ_REMOVE_RES. It is invoked when
 *                  the anr state of a cell is ANR_CELL_STATE_ENABLED. This
 *                  function is used to parse the meas config res received from
 *                  RRM and populate the meas config res for OAM
 ****************************************************************************/
static son_buf_retain_status_et
anr_fsm_meas_config_obj_remove_res
(
 STACKAPIHDR *p_cspl_hdr,
 anr_cell_context_t *  p_cell_ctxt,
 son_anr_index_et    evnt_id,
 son_u8* p_msg
 )
{
	son_rrm_meas_config_obj_remove_res_t *p_res  =
		(son_rrm_meas_config_obj_remove_res_t *)p_msg;
	son_buf_retain_status_et        ret_val = RELEASE_SON_BUFFER;
	son_anr_meas_config_obj_remove_res_t oam_res = {0};
	son_return_et result = SON_FAILURE;

	SON_UT_TRACE_ENTER();

	result = ((p_res->remove_res.meas_config_obj_remove_res.result == RRM_SUCCESS) ? SON_SUCCESS : SON_FAILURE);
	if (SON_SUCCESS == result)
	{
		SON_MEMCPY(&oam_res, p_res, sizeof(son_anr_meas_config_obj_remove_res_t));

		anr_delete_nbr_for_meas_obj_remove(p_cell_ctxt);
	}
	else
	{
		oam_res.transaction_id = p_res->transaction_id;
		SON_MEMCPY(&oam_res.remove_res.src_cgi,
				&p_res->remove_res.meas_config_obj_remove_res.cell_id,
				sizeof(son_intra_rat_global_cell_id_t));

		oam_res.remove_res.result = result;
		oam_res.remove_res.error_code = anr_map_rrm_error_codes(
				__func__,
				result,
				p_res->remove_res.meas_config_obj_remove_res.error_code,
				RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP);

        /*SPR 17777 +-*/ 
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"Error: %d: Meas object removal"
				" request with RRM failed for cell [Cell Id: 0x%x].evnt_id [%u] p_cspl_hdr [%p]",
				p_res->remove_res.meas_config_obj_remove_res.error_code,
				son_convert_char_cell_id_to_int_cell_id(p_res->remove_res.meas_config_obj_remove_res.cell_id.cell_identity)
                ,evnt_id,p_cspl_hdr);
        /*SPR 17777 +-*/ 

	}

	/* Free the memory of the req (frequencies) stored */
	son_mem_free(p_cell_ctxt->p_meas_config_obj_remove);
	p_cell_ctxt->p_meas_config_obj_remove = SON_PNULL;

	son_create_send_buffer((son_u8 *)&oam_res,
			SON_ANR_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONANR_MEAS_CONFIG_OBJ_REMOVE_RES,
			sizeof(oam_res));

	SON_UT_TRACE_EXIT();
	return ret_val;
}





/************************* ANR Cell Specific State Machine*********************/
typedef son_buf_retain_status_et  (*anr_fsm_evt_handler_t)
	(
	 STACKAPIHDR *p_cspl_hdr,
	 anr_cell_context_t  *p_cell_ctxt,
	 son_anr_index_et evnt_id,
	 son_u8 *p_msg
	);

	/************************************************************************
	 * Static per Cell ANR FSM
	 ***********************************************************************/
	static anr_fsm_evt_handler_t
    /* SPR 20653 Fix Start */
	anr_fsm_table[ANR_CELL_STATE_MAX][IDX_SONANR_MAX] =
    /* SPR 20653 Fix End */
{
	/* State ANR_CELL_STATE_ENABLING */
	{
		anr_fsm_nack_anr_enable_req,         /* SONANR_EVENT_ENABLE_REQ */
		anr_fsm_anr_disable_req,             /* SONANR_EVENT_DISABLE_REQ */
		anr_fsm_nack_nr_add_req,             /* SONANR_EVENT_NR_ADD_REQ */
		anr_fsm_nack_nr_delete_req,          /* SONANR_EVENT_NR_DELETE_REQ */
		anr_fsm_nack_nr_update_req,          /* SONANR_EVENT_NR_UPDATE_REQ */
		anr_fsm_nack_nr_retrieve_req,        /* SONANR_EVENT_NR_RETRIEVE_REQ */
		anr_fsm_register_res,                /* RRM_EVENT_REGISTER_RES */
		anr_fsm_invalid_event_handler,       /* RRM_EVENT_DEREGISTER_RES */
		anr_fsm_cell_state_change_handler,   /* RRM_EVENT_CELL_STATE_CHANGE_IND */
		anr_fsm_nack_meas_config_req,        /* SONANR_EVENT_MEAS_CONFIG_REQ */
		anr_fsm_invalid_event_handler,       /* SONANR_EVENT_MEAS_CONFIG_RES */
		anr_fsm_nack_modify_req,             /* SONANR_EVENT_MODIFY_UE_COUNT_REQ*/
		anr_fsm_nack_modify_req,             
		/* SONANR_EVENT_MODIFY_MEAS_REPORTING_INTERVAL_REQ */
		anr_fsm_invalid_event_handler,       
		/* RRM_EVENT_SETT_ATTRIBUTE_VALUE_RES */
		anr_fsm_invalid_event_handler,       /* RRM_EVENT_MEAS_RESULT_IND */
		anr_fsm_nack_nr_delete_from_remove_list_req,
		/* SONANR_EVENT_NR_DELETE_FROM_REMOVE_LIST_REQ */
		anr_fsm_nack_modify_req,            
		/* SONANR_EVENT_MODIFY_REMOVAL_ATTRIBUTES_REQ */
		anr_fsm_nack_modify_req,             
		/* SONANR_EVENT_MODIFY_PCI_CONFUSION_CFG_REQ */
		anr_fsm_nack_enable_disable_limited_mode_req,   
		/* SONANR_EVENT_ENABLE_DISABLE_LIMITED_MODE_REQ */
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_REPORT */
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_ATTEMPTS*/
		anr_fsm_invalid_event_handler, /*IDX_RRM_HO_FAILURE_REPORT*/
		anr_fsm_invalid_event_handler, /*IDX_X2_HO_FALIURE_REPORT*/
		anr_fsm_nack_meas_config_obj_remove_req,        /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_REQ */
		anr_fsm_invalid_event_handler,               /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_RES */  
        /* SPR 20653 Fix Start */
		anr_fsm_nack_modify_req             /* SONANR_EVENT_MODIFY_ATTRIBUTE_REQ */
        /* SPR 20653 Fix End */
	},
	/* State ANR_CELL_STATE_ENABLED */
	{
		anr_fsm_nack_anr_enable_req,          /* SONANR_EVENT_ENABLE_REQ */
		anr_fsm_anr_disable_req,              /* SONANR_EVENT_DISABLE_REQ */
		anr_fsm_nr_add_req,                   /* SONANR_EVENT_NR_ADD_REQ */
		anr_fsm_nr_delete_req,                /* SONANR_EVENT_NR_DELETE_REQ */
		anr_fsm_nr_update_req,                /* SONANR_EVENT_NR_UPDATE_REQ */
		anr_fsm_nr_retrieve_req,              /* SONANR_EVENT_NR_RETRIEVE_REQ */
		anr_fsm_invalid_event_handler,        /* RRM_EVENT_REGISTER_RES */
		anr_fsm_invalid_event_handler,        /* RRM_EVENT_DEREGISTER_RES */
		anr_fsm_cell_state_change_handler,    /* RRM_EVENT_CELL_STATE_CHANGE_IND */
		anr_fsm_meas_config_req,              /* SONANR_EVENT_MEAS_CONFIG_REQ */ 
		anr_fsm_meas_config_res,              /* SONANR_EVENT_MEAS_CONFIG_RES */
		anr_fsm_modify_ue_cnt_req,            /* SONANR_EVENT_MODIFY_UE_COUNT_REQ*/
		anr_fsm_modify_meas_reporting_interval_req,
		/* SONANR_EVENT_MODIFY_MEAS_REPORTING_INTERVAL_REQ */
		anr_fsm_set_attribute_value_res_handler,
		/* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES */
		anr_fsm_ue_meas_results_ind,          /* RRM_EVENT_MEAS_RESULT_IND */
		anr_fsm_nr_delete_from_remove_list_req,         
		/* SONANR_EVENT_NR_DELETE_FROM_REMOVE_LIST_REQ */
		anr_fsm_modify_removal_attributes_req,
		/* SONANR_EVENT_MODIFY_REMOVAL_ATTRIBUTES_REQ*/
		anr_fsm_modify_pci_confusion_cfg_req,   
		/* SONANR_EVENT_MODIFY_PCI_CONFUSION_CFG_REQ*/
		anr_fsm_enable_disable_limited_mode_req, 
		/* SONANR_EVENT_ENABLE_DISABLE_LIMITED_MODE_REQ */
		anr_fsm_rrm_ho_report_handler, /* IDX_RRM_HO_REPORT */
		anr_fsm_rrm_ho_attempts_ind_handler, /* IDX_RRM_HO_ATTEMPTS*/
		anr_fsm_rrm_ho_failure_report_handler, /*IDX_RRM_HO_FAILURE_REPORT*/
		anr_fsm_rrm_ho_failure_report_handler, /*IDX_X2_HO_FALIURE_REPORT*/
		anr_fsm_meas_config_obj_remove_req,          /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_REQ */
		anr_fsm_meas_config_obj_remove_res,          /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_RES */ 
        /* SPR 20653 Fix Start */
		anr_fsm_modify_attribute_req /* SONANR_EVENT_MODIFY_ATTRIBUTE_REQ */
        /* SPR 20653 Fix Start */
	},
	/* State ANR_CELL_STATE_DISABLING */
	{
		anr_fsm_anr_enable_req,                /* SONANR_EVENT_ENABLE_REQ */
		anr_fsm_nack_anr_disable_req,          /* SONANR_EVENT_DISABLE_REQ */
		anr_fsm_nack_nr_add_req,               /* SONANR_EVENT_NR_ADD_REQ */
		anr_fsm_nack_nr_delete_req,            /* SONANR_EVENT_NR_DELETE_REQ */
		anr_fsm_nack_nr_update_req,            /* SONANR_EVENT_NR_UPDATE_REQ */
		anr_fsm_nack_nr_retrieve_req,          /* SONANR_EVENT_NR_RETRIEVE_REQ */
		anr_fsm_invalid_event_handler,         /* RRM_EVENT_REGISTER_RES */
		anr_fsm_deregister_res,                /* RRM_EVENT_DEREGISTER_RES */
		anr_fsm_cell_state_change_handler,     /* RRM_EVENT_CELL_STATE_CHANGE_IND */
		anr_fsm_nack_meas_config_req,          /* SONANR_EVENT_MEAS_CONFIG_REQ */
		anr_fsm_invalid_event_handler,         /* SONANR_EVENT_MEAS_CONFIG_RES */
		anr_fsm_nack_modify_req,               /* SONANR_EVENT_MODIFY_UE_COUNT_REQ*/
		anr_fsm_nack_modify_req,      
		/* SONANR_EVENT_MODIFY_MEAS_REPORTING_INTERVAL_REQ */
		anr_fsm_invalid_event_handler,         
		/* RRM_EVENT_SETT_ATTRIBUTE_VALUE_RES */
		anr_fsm_invalid_event_handler,         /* RRM_EVENT_MEAS_RESULT_IND */
		anr_fsm_nack_nr_delete_from_remove_list_req,
		/* SONANR_EVENT_NR_DELETE_FROM_REMOVE_LIST_REQ */
		anr_fsm_nack_modify_req,            
		/* SONANR_EVENT_MODIFY_REMOVAL_ATTRIBUTES_REQ*/
		anr_fsm_nack_modify_req,  
		/* SONANR_EVENT_MODIFY_PCI_CONFUSION_CFG_REQ*/
		anr_fsm_nack_enable_disable_limited_mode_req, 
		/* SONANR_EVENT_ENABLE_DISABLE_LIMITED_MODE_REQ */
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_REPORT */
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_ATTEMPTS*/
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_FAILURE_REPORT */
		anr_fsm_invalid_event_handler, /*IDX_X2_HO_FALIURE_REPORT*/
		anr_fsm_nack_meas_config_obj_remove_req,        /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_REQ */
		anr_fsm_invalid_event_handler,               /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_RES */   
        /* SPR 20653 Fix Start */
		anr_fsm_nack_modify_req         /* SONANR_EVENT_MODIFY_ATTRIBUTE_REQ */
        /* SPR 20653 Fix Start */
	},
	/* State ANR_CELL_STATE_DISABLED */
	{
		anr_fsm_anr_enable_req,                 /* SONANR_EVENT_ENABLE_REQ */
		anr_fsm_nack_anr_disable_req,           /* SONANR_EVENT_DISABLE_REQ */
		anr_fsm_nack_nr_add_req,                /* SONANR_EVENT_NR_ADD_REQ */
		anr_fsm_nack_nr_delete_req,             /* SONANR_EVENT_NR_DELETE_REQ */
		anr_fsm_nack_nr_update_req,             /* SONANR_EVENT_NR_UPDATE_REQ */
		anr_fsm_nack_nr_retrieve_req,           /* SONANR_EVENT_NR_RETRIEVE_REQ */
		anr_fsm_invalid_event_handler,          /* RRM_EVENT_REGISTER_RES */
		anr_fsm_invalid_event_handler,          /* RRM_EVENT_DEREGISTER_RES */
		anr_fsm_cell_state_change_handler,      /* RRM_EVENT_CELL_STATE_CHANGE_IND */
		anr_fsm_nack_meas_config_req,           /* SONANR_EVENT_MEAS_CONFIG_REQ */
		anr_fsm_invalid_event_handler,          /* SONANR_EVENT_MEAS_CONFIG_RES */
		anr_fsm_nack_modify_req,                /* SONANR_EVENT_MODIFY_UE_COUNT_REQ*/
		anr_fsm_nack_modify_req,                
		/* SONANR_EVENT_MODIFY_MEAS_REPORTING_INTERVAL_REQ */
		anr_fsm_invalid_event_handler,          
		/* RRM_EVENT_SETT_ATTRIBUTE_VALUE_RES */
		anr_fsm_invalid_event_handler,          /* RRM_EVENT_MEAS_RESULT_IND */
		anr_fsm_nack_nr_delete_from_remove_list_req,
		/* SONANR_EVENT_NR_DELETE_FROM_REMOVE_LIST_REQ */
		anr_fsm_nack_modify_req,  
		/* SONANR_EVENT_MODIFY_REMOVAL_ATTRIBUTES_REQ*/
		anr_fsm_nack_modify_req,  
		/* SONANR_EVENT_MODIFY_PCI_CONFUSION_CFG_REQ*/
		anr_fsm_nack_enable_disable_limited_mode_req, 
		/* SONANR_EVENT_ENABLE_DISABLE_LIMITED_MODE_REQ */
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_REPORT */
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_ATTEMPTS*/
		anr_fsm_invalid_event_handler, /* IDX_RRM_HO_FAILURE_REPORT*/
		anr_fsm_invalid_event_handler, /* IDX_X2_HO_FALIURE_REPORT*/
		anr_fsm_nack_meas_config_obj_remove_req,        /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_REQ */
		anr_fsm_invalid_event_handler,               /* SONANR_EVENT_MEAS_CONFIG_OBJ_REMOVE_RES */
        /* SPR 20653 Fix Start */
		anr_fsm_nack_modify_req /* SONANR_EVENT_MODIFY_ATTRIBUTE_REQ */
        /* SPR 20653 Fix Start */
	}
};

/*****************************************************************************
 * Function Name  : anr_cell_process_msg
 * Inputs         : p_cspl_hdr         - Pointer to CSPL Header
 *                  evnt_id            - event identity
 *                  p_cell_ctxt        - Pointer to cell context
 *                  p_anr_msg          -  Pointer to incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - RELEASE_SON_BUFFER when encapsulating 
 *                                             shell should release the buffer or 
 *                                             RETAIN_SON_BUFFER when Encapsulating 
 *                                             shell should retain the buffer
 * Description    : This is the entry point to the ANR Cell FSM.
 *                  This function takes the API message received,event_id,
 *                  cell context
 *                  and calls the required event handler function.
 ****************************************************************************/
son_buf_retain_status_et
anr_cell_process_msg
(
 STACKAPIHDR         *p_cspl_hdr,
 son_anr_index_et    evnt_id,
 anr_cell_context_t  *p_cell_ctxt,
 son_u8              *p_anr_msg
 )
{
	/*Coverity Fix:55174:Start*/
    /* SPR 20653 Fix Start */
	anr_cell_fsm_state_et  curr_cell_fsm_state = ANR_CELL_STATE_MAX; 
    /* SPR 20653 Fix End */
	/*Coverity Fix:55174:Stop*/
	son_buf_retain_status_et ret = RELEASE_SON_BUFFER;


	SON_UT_TRACE_ENTER();

	if (p_cspl_hdr == SON_PNULL || p_cell_ctxt == SON_PNULL || p_anr_msg == SON_PNULL)
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"p_cspl_hdr, p_cell_ctxt or p_anr_msg is NULL"
				);
		SON_UT_TRACE_EXIT();
		/*Coverity Fix:41586,41587:Start*/
		return RELEASE_SON_BUFFER;
		/*Coverity Fix:41586,41587:Stop*/
	}

	/*Coverity Fix:55174:Start*/
	curr_cell_fsm_state = p_cell_ctxt->current_cell_fsm_state;
	/*Coverity Fix:55174:Stop*/

	/* Coverity Fix: 17306 */
    /* SPR 20653 Fix Start */
	if ((curr_cell_fsm_state < ANR_CELL_STATE_MAX) &&
			(evnt_id < IDX_SONANR_MAX))
        /* SPR 20653 Fix End */
	{
		ret = (*anr_fsm_table[curr_cell_fsm_state][evnt_id])
			(p_cspl_hdr, p_cell_ctxt, evnt_id, p_anr_msg);
        
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
              "cell_id: 0x%x OldState: %s Event: %s NewState: %s",
               son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
               SON_ANR_CELL_FSM_STATES_NAMES[curr_cell_fsm_state], 
               SON_ANR_CELL_FSM_EVENT_NAMES[evnt_id],
               SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state]);
	}
	else
	{
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR, 
				"Error: Unexpected Event or State cell_id: 0x%x State: %d Event: %d",
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                curr_cell_fsm_state, evnt_id); 
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_cell_process_msg */
/* Spr 17753 Changes Start */
/*****************************************************************************
 * Function Name  : anr_check_and_insert_second_degree_nbr_data
 * Inputs         : p_intra_rat_hash_rec,         - Pointer to CSPL Header
 *                  p_cell_ctxt          -  Pointer to incoming message
 * Outputs        : None
 * Returns        : son_void_t - None 
 * Description    : This function copy the second degree nrbr from the list  
 ****************************************************************************/
son_void_t
anr_check_and_insert_second_degree_nbr_data
(
 anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec,
 anr_cell_context_t       *p_cell_ctxt
 )
{
    anr_second_degree_nr_list_node_t * p_nbr_list_node = SON_PNULL;
    SON_LIST *p_list    = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_list = &p_cell_ctxt->second_degree_nbr_list;

    /*Finding the nbr CGI value in second degree nbr list*/
    p_nbr_list_node = anr_find_record_in_second_degree_nbr_list
                            (p_list,&p_intra_rat_hash_rec->neighbor_info.nbr_cgi);
    
    /*If CGI is present*/
    if(p_nbr_list_node != SON_PNULL)
    {   
        SON_MEMCPY(&p_intra_rat_hash_rec->anr_nr_nbr_list,
                   &p_nbr_list_node->nr_nbr_data.anr_nr_nbr_list,
                   sizeof(SON_LIST));

        /* Delete node from Link List */
        list_delete_node(p_list, &p_nbr_list_node->nr_nbr_anchor);
        son_mem_free(p_nbr_list_node);

        /* List exhausted stop the nr add timer */
        if(0 == list_count(p_list) &&(SON_PNULL != p_cell_ctxt->son_nr_add_timer_id))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, 
                    SON_DETAILED,
                    "anr_check_and_insert_second_degree_nbr_data:"
                    " second degree list exhausted stopping the nr_add_timer");

            son_stop_timer(p_cell_ctxt->son_nr_add_timer_id);
            p_cell_ctxt->son_nr_add_timer_id = SON_PNULL;
        }
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, 
                SON_BRIEF,
                "anr_check_and_insert_second_degree_nbr_data:"
                " neighbor not found in second degree list\n");
    }

    SON_UT_TRACE_EXIT();
} /* anr_check_and_insert_second_degree_nbr_data */
/* Spr 17753 Changes End */
