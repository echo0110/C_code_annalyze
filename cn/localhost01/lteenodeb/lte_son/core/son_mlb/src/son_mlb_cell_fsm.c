/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_mlb_cell_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains MLB per cell FSM functions
 *
 *******************************************************************************
* Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Dec, 2012        Sajida/Vinod                Initial
 ******************************************************************************/
/* Includes */
#include <son_mlb_cell_fsm.h>
#include <son_mlb_ctxt_mgr.h>
#include <son_utils.h>
#include <rrm_son_composer.h>
#include <son_rrm_composer.h>
#include <son_mlb_cell_fsm.h>
#include <son_anr_cell_fsm.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_anr_table.h>
#include <son_anr_utils.h>
#include <son_mlb_intf.h>

extern const son_8 *p_son_mlb_facility_name;
    /*SPR 22006 Start*/
extern son_global_config_params_t g_config_params;
    /*SPR 22006 End*/

/*SPR 20523 START*/
son_s8 g_actual_db_values[SON_Q_OFFSET_ABSOLUTE_VAL_DB31]=
{
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_24,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_22,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_20,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_18,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_16,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_14,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_12,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_10,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_8,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_6,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_5,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_4,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_3,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_2,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB_1,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB0,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB1,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB2,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB3,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB4,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB5,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB6,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB8,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB10,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB12,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB14,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB16,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB18,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB20,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB22,
    SON_Q_OFFSET_ABSOLUTE_VAL_DB24
};
/*SPR 20523 END*/
/*****************************************************************************
 *  Function Name  : son_mlb_object_id_keyof 
 *  Inputs         : p_ylnode - Pointer to scan data.
 *  Outputs        : None
 *  Returns        : node
 *  Description    : This function is the keyof function for the son_mlb_neigh_cell_info_record_t
 *                   linked list.
 *****************************************************************************/
static const void *
son_mlb_object_id_keyof
(
    const YLNODE *p_ylnode
)
{
    return &(((son_mlb_neigh_cell_info_record_t *)p_ylnode)->
            neigh_cell_info_data.neigh_cell_id);
}       

/*SPR 20523 START*/
/*****************************************************************************
 * Function Name  : son_map_q_offset_value
 * Inputs         : son_q_offset_range_et q_offset_value
 * Outputs        : son_s8 absolute_q_offset_value_db
 * Returns        : son_s8
 * Description    : Maps and returns q_offset_value
 *****************************************************************************/
son_s8
son_map_q_offset_value
(
 son_q_offset_range_et q_offset_value
 )
{
    /*+SPR 22535*/
    son_s8 absolute_q_offset_value_db = SON_NULL;
    /*-SPR 22535*/
    SON_UT_TRACE_ENTER();

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "value received of Q_OFFSET = %d",q_offset_value);
    switch(q_offset_value)
    {
        case SON_Q_OFFSET_RANGE_DB_24 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_24;
        break;

        case SON_Q_OFFSET_RANGE_DB_22 :
        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_22;
        break;
        case SON_Q_OFFSET_RANGE_DB_20 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_20;
        break;

        case SON_Q_OFFSET_RANGE_DB_18 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_18;
        break;

        case SON_Q_OFFSET_RANGE_DB_16 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_16;
        break;

        case SON_Q_OFFSET_RANGE_DB_14 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_14;
        break;

        case SON_Q_OFFSET_RANGE_DB_12 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_12;
        break;

        case SON_Q_OFFSET_RANGE_DB_10 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_10;
        break;

        case SON_Q_OFFSET_RANGE_DB_8 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_8;
        break;

        case SON_Q_OFFSET_RANGE_DB_6 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_6;
        break;

        case SON_Q_OFFSET_RANGE_DB_5 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_5;
        break;

        case SON_Q_OFFSET_RANGE_DB_4 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_4;
        break;

        case SON_Q_OFFSET_RANGE_DB_3 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_3;
        break;

        case SON_Q_OFFSET_RANGE_DB_2 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_2;
        break;

        case SON_Q_OFFSET_RANGE_DB_1 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB_1;
        break;

        case SON_Q_OFFSET_RANGE_DB0 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB0;
        break;

        case SON_Q_OFFSET_RANGE_DB1 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB1;
        break;

        case SON_Q_OFFSET_RANGE_DB2 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB2;
        break;

        case SON_Q_OFFSET_RANGE_DB3 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB3;
        break;

        case SON_Q_OFFSET_RANGE_DB4 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB4;
        break;

        case SON_Q_OFFSET_RANGE_DB5 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB5;
        break;

        case SON_Q_OFFSET_RANGE_DB6 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB6;
        break;

        case SON_Q_OFFSET_RANGE_DB8 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB8;
        break;

        case SON_Q_OFFSET_RANGE_DB10 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB10;
        break;

        case SON_Q_OFFSET_RANGE_DB12 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB12;
        break;

        case SON_Q_OFFSET_RANGE_DB14 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB14;
        break;

        case SON_Q_OFFSET_RANGE_DB16 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB16;
        break;

        case SON_Q_OFFSET_RANGE_DB18 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB18;
        break;

        case SON_Q_OFFSET_RANGE_DB20 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB20;
        break;

        case SON_Q_OFFSET_RANGE_DB22 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB22;
        break;

        case SON_Q_OFFSET_RANGE_DB24 :

        absolute_q_offset_value_db = SON_Q_OFFSET_ABSOLUTE_VAL_DB24;
        break;
        default :

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Invalid value received of Q_OFFSET = %d",q_offset_value);
    }
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_INFO,
            "Q_OFFSET value = %d and Mapped value of Q_OFFSET = %d",q_offset_value, absolute_q_offset_value_db);
    SON_UT_TRACE_EXIT();
    return absolute_q_offset_value_db;
}

/*****************************************************************************
 * Function Name  : son_map_q_offset_value_to_enum
 * Inputs         : son_s8 db_value_new_offset
 * Outputs        : q_offset_enum_value
 * Returns        : son_q_offset_range_et
 * Description    : Maps and returns q_offset_value
 *****************************************************************************/
son_q_offset_range_et
son_map_q_offset_value_to_enum
(
 son_s8 db_value_new_offset
 )
{
    /*+SPR 22535*/
    son_q_offset_range_et q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_24;
    /*-SPR 22535*/
    SON_UT_TRACE_ENTER();

    switch(db_value_new_offset)
    {
        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_24 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_24;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_22 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_22;
        break;
        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_20 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_20;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_18 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_18;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_16 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_16;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_14 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_14;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_12 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_12;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_10 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_10;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_8 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_8;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_6 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_6;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_5 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_5;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_4 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_4;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_3 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_3;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_2 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_2;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB_1 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB_1;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB0 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB0;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB1 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB1;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB2 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB2;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB3 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB3;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB4 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB4;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB5 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB5;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB6 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB6;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB8 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB8;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB10 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB10;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB12 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB12;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB14 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB14;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB16 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB16;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB18 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB18;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB20 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB20;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB22 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB22;
        break;

        case SON_Q_OFFSET_ABSOLUTE_VAL_DB24 :

        q_offset_enum_value = SON_Q_OFFSET_RANGE_DB24;
        break;
        default :

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Invalid value received of Q_OFFSET = %d",db_value_new_offset);
    }
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_INFO,
            "Q_OFFSET value = %d, and Enum value of Q_OFFSET = %d",db_value_new_offset, q_offset_enum_value);
    SON_UT_TRACE_EXIT();
    return q_offset_enum_value;
}

/*****************************************************************************
 * Function Name  : son_reverse_map_db_to_enum
 * Inputs         : son_s8 db_value
 * Outputs        : son_q_offset_range_et enum_value
 * Returns        : son_q_offset_range_et
 * Description    : Maps q_offset_value to enum value
 *****************************************************************************/

son_q_offset_range_et
son_reverse_map_db_to_enum
(
 son_s8 db_value_new_offset,
 son_s8 initial_offset_db_value
 )
{
    son_q_offset_range_et enum_value;
    son_bool_et value_incremented = SON_FALSE;
    son_u32 first = SON_NULL;
    son_u32 last  = SON_Q_OFFSET_ABSOLUTE_VAL_DB30;
    son_u32 middle = SON_NULL;
    SON_UT_TRACE_ENTER();

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "value received of Q_OFFSET(DB) = %d",db_value_new_offset);
    if(db_value_new_offset > initial_offset_db_value)
    {
        value_incremented = SON_TRUE;
    }

    if(g_actual_db_values[SON_Q_OFFSET_ABSOLUTE_VAL_DB30] < db_value_new_offset)
    {
        db_value_new_offset = g_actual_db_values[SON_Q_OFFSET_ABSOLUTE_VAL_DB30];
    }
    else if( db_value_new_offset < g_actual_db_values[SON_NULL])
    {
        db_value_new_offset = g_actual_db_values[SON_NULL];
    }
    else
    {
        middle = (first + last)/SON_Q_OFFSET_ABSOLUTE_VAL_DB2;

        while(first <= last)
        {
            if(db_value_new_offset > g_actual_db_values[middle])
            {
                if(db_value_new_offset < g_actual_db_values[middle + SON_Q_OFFSET_ABSOLUTE_VAL_DB1])
                {
                    if(value_incremented == SON_TRUE)
                    {
                        db_value_new_offset = g_actual_db_values[middle + SON_Q_OFFSET_ABSOLUTE_VAL_DB1];
                    }
                    else
                    {
                        db_value_new_offset = g_actual_db_values[middle];
                    }
                    break;
                }
                first = middle + SON_Q_OFFSET_ABSOLUTE_VAL_DB1;
            }

            else if(db_value_new_offset == g_actual_db_values[middle])
            {
                break;
            }

            else
            {
                if(db_value_new_offset > g_actual_db_values[middle - SON_Q_OFFSET_ABSOLUTE_VAL_DB1])
                {
                    if(value_incremented == SON_TRUE)
                    {
                        db_value_new_offset = g_actual_db_values[middle];
                    }
                    else
                    {
                        db_value_new_offset = g_actual_db_values[middle - SON_Q_OFFSET_ABSOLUTE_VAL_DB1];
                    }
                    break;
                }
                last = middle - SON_Q_OFFSET_ABSOLUTE_VAL_DB1;
            }

            middle = (last + first)/SON_Q_OFFSET_ABSOLUTE_VAL_DB2;
        }
    }

    enum_value = son_map_q_offset_value_to_enum(db_value_new_offset);

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "enum_value of Q_OFFSET = %d",enum_value);
    return enum_value;
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_calculate_mob_change_range
 * Inputs         : mlb_cell_context_t Pointer to MLB Cell context
 *                : son_mlb_neigh_cell_info_t Pointer to Neighbor cell info
 *                : son_mob_param_range_t Pointer to mobility range
 * Outputs        : None
 * Returns        : void
 * Description    : Calculates mobility range
 *****************************************************************************/
void
son_calculate_mob_change_range
(
 mlb_cell_context_t *p_cell_ctxt,
 son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
 son_mob_param_range_t *p_mob_range
 )
{
    son_s8 mapped_q_offset_max;
    son_s8 mapped_initial_cm_cell_offset;
    son_s8 mapped_q_offset_min;
    son_s8 lower_limit_temp = SON_NULL;
    son_s8 upper_limit_temp = SON_NULL;

    SON_UT_TRACE_ENTER();
    mapped_initial_cm_cell_offset = son_map_q_offset_value(p_neigh_cell_info->initial_cm_cell_offset);
    mapped_q_offset_max = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_max);
    mapped_q_offset_min = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_min);

    lower_limit_temp = mapped_initial_cm_cell_offset - SON_Q_OFFSET_ABSOLUTE_VAL_DB10;
    upper_limit_temp = mapped_initial_cm_cell_offset + SON_Q_OFFSET_ABSOLUTE_VAL_DB10;

    if(lower_limit_temp < mapped_q_offset_min)
    {
        lower_limit_temp = mapped_q_offset_min;
    }
    if(upper_limit_temp > mapped_q_offset_max)
    {
        upper_limit_temp = mapped_q_offset_max;
    }

    p_mob_range->upper_limit = upper_limit_temp - mapped_initial_cm_cell_offset;
    p_mob_range->lower_limit = lower_limit_temp - mapped_initial_cm_cell_offset;

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "p_mob_range.upper_limit = %d and p_mob_range.lower_limit = %d",p_mob_range->upper_limit,p_mob_range->lower_limit);

    SON_UT_TRACE_EXIT();
}
/*SPR 20523 END*/

/*****************************************************************************
 * Function Name  : mlb_map_rrm_error_codes
 * Inputs         : son_func_name - Pointer to calling function name
 *                : result    -     SUCESS/FAILURE
 *                : rrm_error_code - Error Code received from RRM
 *                : msg_id      -    API Id of message received from RRM
 * Outputs        : None
 * Returns        : son_error_et
 * Description    : Maps and returns SON error code for the received RRM
 *                : error code
 ****************************************************************************/
static son_error_et
mlb_map_rrm_error_codes
(
    const son_s8 *son_func_name,
 son_return_et result,
 rrm_error_et rrm_error_code,
 rrm_son_message_resp_et msg_id    /*coverity_CID_25758 fix start-end*/
 )
{
    son_error_et ret_val = SON_NO_ERROR;


    SON_UT_TRACE_ENTER();
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_ERROR,
            "%s: Message %d received from RRM with result: %d and error "
            "code: %d",
            son_func_name,
            msg_id,
            result,
            rrm_error_code);

    switch (rrm_error_code)
    {
        case RRM_ERR_INVALID_PARAMS:
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

        default:
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_ERROR,
                    "%s: RRM Error Code %d received in message %d could not "
                    "be mapped",
                    son_func_name,
                    rrm_error_code,
                    msg_id);
            ret_val = SON_ERR_RRM_FAILURE;
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mlb_map_rrm_error_codes */


/*Rel 3.0 Multi Sector code changes START*/
/******************************************************************************
 * Function Name  : son_mlb_populate_source_enbId 
 * Inputs         : p_cell_ctxt - Pointer to mlb cell context
 *                : src_enbid -
 *                : src_cgi1 -
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : function to creta and send Mobility Change ack to SON-X2
 *****************************************************************************/
son_void_t
son_mlb_populate_source_enbId
(
    mlb_cell_context_t *p_cell_ctxt,
    son_global_enb_id_t                 *src_enbid,
    son_intra_rat_global_cell_id_t      *src_cgi1
)
{

    /*poulatting  source ENB ID*/
    SON_MEMCPY(&src_enbid->plmn_id, 
		&p_cell_ctxt->serv_cell_load_info.g_cell_id.primary_plmn_id,
            	sizeof(son_cell_plmn_info_t));

    if (SON_TRUE == p_cell_ctxt->src_csg_id_present /*& CSG FLAG */)
    {
        src_enbid->enb_type = SON_HOME_ENB;
        src_enbid->enb_id[0] = 
            p_cell_ctxt->serv_cell_load_info.g_cell_id.cell_identity[0]; 
        src_enbid->enb_id[1] = 
            p_cell_ctxt->serv_cell_load_info.g_cell_id.cell_identity[1]; 
        src_enbid->enb_id[2] = 
            p_cell_ctxt->serv_cell_load_info.g_cell_id.cell_identity[2]; 
        src_enbid->enb_id[3] = 
            p_cell_ctxt->serv_cell_load_info.g_cell_id.cell_identity[3] & 0xF0;
    }
    else
    {
        src_enbid->enb_type = SON_MACRO_ENB;
        anr_determine_macro_enb_id(src_cgi1, src_enbid);
    }

}/*son_mlb_populate_source_enbId*/

/******************************************************************************
 * Function Name  : son_mlb_populate_target_enbId 
 * Inputs         : p_neigh_cell_info - Pointer to the neighbor context
 *                : trgt_enbid -
 *                : src_cgi2 -
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : function to creta and send Mobility Change ack to SON-X2
 *****************************************************************************/
son_void_t
son_mlb_populate_target_enbId
(
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
    son_global_enb_id_t                 *trgt_enbid,
    son_intra_rat_global_cell_id_t     	*src_cgi2
)
{

    /*poulatting  target ENB ID*/
    SON_MEMCPY(&trgt_enbid->plmn_id, 
	&p_neigh_cell_info->neigh_cell_load_info.g_cell_id.primary_plmn_id,
        sizeof(son_cell_plmn_info_t));

    if (SON_TRUE == p_neigh_cell_info->nbr_csg_id_present /*& CSG FLAG */)
    {
        trgt_enbid->enb_type = SON_HOME_ENB;
        trgt_enbid->enb_id[0] = 
            p_neigh_cell_info->neigh_cell_load_info.g_cell_id.cell_identity[0]; 
        trgt_enbid->enb_id[1] = 
            p_neigh_cell_info->neigh_cell_load_info.g_cell_id.cell_identity[1]; 
        trgt_enbid->enb_id[2] = 
            p_neigh_cell_info->neigh_cell_load_info.g_cell_id.cell_identity[2]; 
        trgt_enbid->enb_id[3] = 
            p_neigh_cell_info->neigh_cell_load_info.g_cell_id.cell_identity[3] 
	    & 0xF0;
    }
    else
    {
        trgt_enbid->enb_type = SON_MACRO_ENB;
        anr_determine_macro_enb_id(src_cgi2, trgt_enbid);
    }

}/*son_mlb_populate_target_enbId*/


/*Rel 3.0 Multi Sector code changes STOP*/


/******************************************************************************
 * Function Name  : son_mlb_send_mobility_change_fail 
 * Inputs         : p_cell_ctxt - Pointer to mlb cell context
 *                : p_neigh_cell_info - Pointer to the neighbor context
 *                : ho_trigger_change_serv - Calculated HO threshold trigger
 *                  change value
 *                : p_mob_range - pointer to the mobility range sent by OAM 
 *                  in centralized mode
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : function to creta and send Mobility Change ack to SON-X2
 *****************************************************************************/
son_void_t
son_mlb_send_mobility_change_fail
(
    mlb_cell_context_t *p_cell_ctxt,
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
    /*SPR 20523 START*/
    /*code deleted*/
    /*SPR 20523 END*/
    son_mob_param_range_t *p_mob_range
)
{
    SON_UT_TRACE_ENTER();
    sonmlb_mobility_change_failure_t fail;
    /* SPR 19015 Fix Start */
    anr_cell_context_t        *p_anr_cell_ctxt = SON_PNULL;
    SON_HASH                  *p_hash = SON_PNULL;
    SON_HASH_NODE             *p_hash_node = SON_PNULL;
    anr_intra_rat_hash_rec_t   hash_data ;
    anr_intra_rat_hash_nr_tuple_t   *p_tpl      = SON_PNULL;
    /* SPR 19015 Fix Stop */


    SON_MEMSET(&fail, 0, sizeof(sonmlb_mobility_change_failure_t));

    SON_MEMCPY(&fail.src_cgi1, &p_cell_ctxt->serv_cell_load_info.g_cell_id, sizeof(son_intra_rat_global_cell_id_t));
    SON_MEMCPY(&fail.src_cgi2,
        &p_neigh_cell_info->neigh_cell_load_info.g_cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    /*populating  range*/
    /*SPR 20523 START*/
    if (SON_PNULL != p_mob_range)
    {
        fail.bitmask |= MOBILITY_PARAM_RANGE_PRESENT;
        SON_MEMCPY(&fail.mob_param_range, p_mob_range, sizeof(son_mob_param_range_t));
    }
    /*SPR 20523 END*/

    /*poulatting  source ENB ID*/
    /* SPR 19015 Fix Start */
    son_anr_get_serving_enb_id(p_cell_ctxt->cell_id,&fail.src_enbid);

    /*poulatting  target ENB ID*/
    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
            &p_cell_ctxt->cell_id);
    /* SPR_20915 Fix - Starts */
    if(SON_PNULL == p_anr_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_ERROR,
            "p_anr_cell_ctxt is NULL");
        SON_UT_TRACE_EXIT();
        return;
    }
    /* SPR_20915 Fix - Ends */

    p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;

    SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

    SON_MEMCPY(&hash_data.neighbor_info.nbr_cgi,&p_neigh_cell_info->neigh_cell_id,
            sizeof(son_intra_rat_global_cell_id_t));

    if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
    {
        p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
            YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_hash_node);

        fail.trgt_enbid = p_tpl->data.enb_id;
    }
    /* SPR 19015 Fix Stop */
    else
    {
        son_create_send_buffer((son_u8 *)&fail, SON_MLB_MODULE_ID,
            SON_MLB_MODULE_ID, SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE,
            sizeof(sonmlb_mobility_change_failure_t));
    }

    SON_UT_TRACE_EXIT();
} /* son_mlb_send_mobility_change_fail */


/******************************************************************************
 * Function Name  : son_mlb_send_mobility_change_ack 
 * Inputs         : p_cell_context -  Pointer to mlb cell context
 *                : p_neigh_cell_info - Pointer to the neighbor context
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : function to creta and send Mobility Change ack to SON-X2
 *****************************************************************************/
son_void_t
son_mlb_send_mobility_change_ack
(
    mlb_cell_context_t *p_cell_ctxt,
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info
)
{
    SON_UT_TRACE_ENTER();
  /*SPR 19030 changes start*/
  anr_cell_context_t        *p_anr_cell_ctxt = SON_PNULL;
  SON_HASH                  *p_hash = SON_PNULL;
  SON_HASH_NODE             *p_hash_node = SON_PNULL;
  anr_intra_rat_hash_rec_t   hash_data ;
  anr_intra_rat_hash_nr_tuple_t   *p_tpl      = SON_PNULL;
  /*SPR 19030 changes end*/

    sonmlb_mobility_change_ack_t ack;

    SON_MEMSET(&ack, 0, sizeof(sonmlb_mobility_change_ack_t));
    SON_MEMCPY(&ack.src_cgi1, &p_cell_ctxt->serv_cell_load_info.g_cell_id, 
                sizeof(son_intra_rat_global_cell_id_t));
    SON_MEMCPY(&ack.src_cgi2, 
               &p_neigh_cell_info->neigh_cell_load_info.g_cell_id,
               sizeof(son_intra_rat_global_cell_id_t));

   /*Rel 3.0 Multi Sector code changes Start*/
    /*poulatting  source ENB ID*/
  /*SPR 19030 changes start*/
  son_anr_get_serving_enb_id(p_cell_ctxt->cell_id,&ack.src_enbid);

    /*poulatting  target ENB ID*/
  p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
      &p_cell_ctxt->cell_id);

  p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;

  SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

  SON_MEMCPY(&hash_data.neighbor_info.nbr_cgi,&p_neigh_cell_info->neigh_cell_id,
      sizeof(son_intra_rat_global_cell_id_t));

  if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
  {
    p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
      YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_hash_node);

    ack.trgt_enbid = p_tpl->data.enb_id;
  }

  /*SPR 19030 changes end*/
  /* SPR 19221: CID 108598 fix start */
  if (!anr_is_nbr_cell_on_serving_enb (&ack.trgt_enbid))
  {
      son_create_send_buffer((son_u8 *)&ack, SON_MLB_MODULE_ID,
          SON_X2_MODULE_ID, SONMLB_MOBILITY_CHANGE_ACK,
          sizeof(sonmlb_mobility_change_ack_t));
  }
  else
  {
      son_create_send_buffer((son_u8 *)&ack, SON_MLB_MODULE_ID,
          SON_MLB_MODULE_ID, SONMLB_MOBILITY_CHANGE_ACK,
          sizeof(sonmlb_mobility_change_ack_t));
  }
  /*Rel 3.0 Multi Sector code changes Stop*/

   SON_UT_TRACE_EXIT();
  /* SPR 19221: CID 108598 fix stop */
} /* son_mlb_send_mobility_change_ack */

/******************************************************************************
 * Function Name  : mlb_cell_calc_serv_ho_threshold 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : p_neigh_cell_info - Neighbor cell info
 *                : ho_trigger_change_serv - Calculated HO trigger change for serv cell
 *                : new_cell_offset_connected - Calculated new cell offset
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : calculate the changed HO range possible for serving.
 *****************************************************************************/
son_return_et
mlb_cell_calc_serv_ho_threshold
(
 mlb_cell_context_t   *p_cell_ctxt,
 son_mlb_neigh_cell_info_t   *p_neigh_cell_info,
 /*SPR 20523 START*/
 son_s8 *p_ho_trigger_change_serv,
 /* CID 118132 Fix Start */
 son_s32  *p_new_cell_offset_connected
 /* CID 118132 Fix End */
 /*SPR 20523 END*/
 )
{
    son_u8 nbr_load, serving_load;
    son_float    diff_load;
    son_s8 ho_threshold_serv_old, ho_threshold_serv_new;
    /* CID 118132 Fix Start */
    son_s8 delta_offset_connected = 0;
    /* CID 118132 Fix End */
    /*SPR 20523 START*/
    son_s8 mapped_q_offset_max;
    son_s8 mapped_q_offset_min;
    son_s8 mapped_initial_cm_cell_offset;
    /*SPR 20523 END*/
	
    /*SPR 20190 Fix Start*/
    /*SPR 20523 START*/
    /*code deleted*/
    /*SPR 20523 END*/
	/*SPR 20190 Fix Stop*/

    /*SPR 18750 Fix Start*/
    if(!(p_neigh_cell_info->neigh_cell_load_info.bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT) ||
            !(p_cell_ctxt->serv_cell_load_info.bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT))
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "nbr/Serv load is not yet received");
        return SON_FAILURE;
    }
    /*SPR 18750 Fix Stop*/

    nbr_load = p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val;
    serving_load = p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val;

    /* SPR 19015 Fix Start */
     
    if((p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask & 
                RRM_SON_CELL_CAP_CLASS_VAL_PRESENT) &&
            (p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask &
             RRM_SON_CELL_CAP_CLASS_VAL_PRESENT))
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
               "nbr_load and serving_load is :%d %d",nbr_load,serving_load);

        nbr_load = ((nbr_load * 
                    p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val)/
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val);

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "RRM_SON_CELL_CAP_CLASS_VAL is present & calculated nbr_load [%d] serving load [%d]"
                "neighbour cell cap class [%d]"
                "serving cell cap class [%d]",
                nbr_load,serving_load,p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val,
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val);
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "RRM_SON_CELL_CAP_CLASS_VAL is not present so calculated nbr_load :%d serving load [%d]",nbr_load,serving_load);
    }
    /* SPR 19015 Fix Stop */

    /*calculate new offset for neighbor cell.threshold load
     * value we read as in percentage from CFG file son_oam.cfg
     * exp : if threshold value is 10 in cfg file the actual value should be 10/100 = 0.1*/
    if ((serving_load > nbr_load))
    {
        diff_load = (serving_load - nbr_load);
    }
    else if ((serving_load < nbr_load))
    {
        diff_load = (nbr_load - serving_load);
    }
    else
    {
        /*return from here*/
        /*SPR 20190 Fix Start*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,"Serving and neighbor load are equal");
        /*SPR 20190 Fix Stop*/
        return SON_FAILURE;
    }

    /*SPR 20523 START*/
    mapped_q_offset_max = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_max);
    mapped_q_offset_min = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_min);
    mapped_initial_cm_cell_offset = son_map_q_offset_value(p_neigh_cell_info->initial_cm_cell_offset);

    /* ∆ = (|Pi –Ps| - Pth,conn) / (1- Pth,conn ) * (OCNmax – OCNmin,step_size) + OCNmin,step_size.
     * here we read load in percentage from cfg file.so need to divide load by 100*/
    delta_offset_connected =
        (son_s8)(((diff_load/100.0 - p_cell_ctxt->eutra_threshold_connected /
                        100.0) / (1 - p_cell_ctxt->eutra_threshold_connected / 100.0)) *
                (mapped_q_offset_max - g_mlb_context.mlb_config_param.eutra_ocn_min_step)+ g_mlb_context.mlb_config_param.eutra_ocn_min_step);

    /*SPR 20190 Fix Start*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_DETAILED,
            "mlb_cell_calc_serv_ho_threshold: diff_load=%f, delta_offset_connected=%u, eutra_threshold_connected=%u",
            diff_load, delta_offset_connected,p_cell_ctxt->eutra_threshold_connected);
    /*SPR 20190 Fix Stop*/

    /*Only +/- 10dB change allowed*/
    if(delta_offset_connected > SON_Q_OFFSET_ABSOLUTE_VAL_DB10)
    {
        delta_offset_connected = SON_Q_OFFSET_ABSOLUTE_VAL_DB10;
    }

    /*if serving and neighbor load difference is greater than threhhold load that means the
     * serving has more available capacity and the handover offset OCN shall be ncreased so that UEs can move to neighbor*/
    if ((serving_load > nbr_load) && (diff_load) > p_cell_ctxt->eutra_threshold_connected)
    {
        /*SPR 15884 Fix Start*/
        /*SPR 20190 Fix Start*/
        *p_new_cell_offset_connected = mapped_initial_cm_cell_offset - delta_offset_connected;
        /*SPR 20190 Fix Stop*/
        /*SPR 15884 Fix Stop*/
    }

    /*if serving and neighbor load difference is less than threhhold load that means the neighbor 
     * has more available capacity and the handover offset OCN shall be decreased so that UEs can move to neighbor*/
    else if ((serving_load < nbr_load) && (diff_load) > p_cell_ctxt->eutra_threshold_connected)
    {
        /*SPR 15884 Fix Start*/
        /*SPR 20190 Fix Start*/
        *p_new_cell_offset_connected = mapped_initial_cm_cell_offset + delta_offset_connected;
        /*SPR 20190 Fix Stop*/
        /*SPR 15884 Fix Stop*/
    }

    else
    {
        /*SPR 20190 Fix Start*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_WARNING,"Serving and neighbor load are equal");
        /*SPR 20190 Fix Stop*/

        /*return from here*/
        return SON_FAILURE;
    }

    /*SPR 20190 Fix Start*/
    if(*p_new_cell_offset_connected < mapped_q_offset_min)
    {
        *p_new_cell_offset_connected = mapped_q_offset_min;
    }
    if (*p_new_cell_offset_connected > mapped_q_offset_max)
    {
        *p_new_cell_offset_connected = mapped_q_offset_max;
    }

    /*SPR 20190 Fix Stop*/
    if (*p_new_cell_offset_connected != mapped_initial_cm_cell_offset)
    {
        /*calculate the HO range change for serving cell.
         * event A3 based HO threshold = hys + off +ocs + ocs - ofn - ocn*/

        /*calculate HO threshold of serving using old OCN*/
        ho_threshold_serv_old = p_neigh_cell_info->offset_attr.hys + p_neigh_cell_info->offset_attr.off + 
            p_neigh_cell_info->offset_attr.ocs + p_neigh_cell_info->offset_attr.ofs -
            p_neigh_cell_info->offset_attr.ofn - mapped_initial_cm_cell_offset;

        /*calculate HO threshold of serving using new calculated OCN*/
        ho_threshold_serv_new = p_neigh_cell_info->offset_attr.hys + p_neigh_cell_info->offset_attr.off + 
            p_neigh_cell_info->offset_attr.ocs + p_neigh_cell_info->offset_attr.ofs -
            p_neigh_cell_info->offset_attr.ofn - *p_new_cell_offset_connected;

        /*calculate HO threshold change of serving*/
        *p_ho_trigger_change_serv = ho_threshold_serv_new - ho_threshold_serv_old;
        /*SPR 20190 Fix Start*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "ho_threshold_serv_old[%d] , ho_threshold_serv_new[%d] , ho_trigger_change_serv[%ld]",
                ho_threshold_serv_old , ho_threshold_serv_new , *p_ho_trigger_change_serv);
        /*SPR 20190 Fix Stop*/

    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,"p_new_cell_offset_connected,mapped_initial_cm_cell_offset are same and = [%d]",*p_new_cell_offset_connected);
        return SON_FAILURE;

    }
    /*SPR 20523 END*/

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* mlb_cell_calc_serv_ho_threshold */


/*Rel 3.0 Multi Sector code changes Start*/
/******************************************************************************
 * Function Name  : mlb_cell_mobility_change_req_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_MOBILITY_CHANGE_REQ and SONMLB_INTERNAL_MOBILITY_CHANGE_REQ..
 *****************************************************************************/
son_buf_retain_status_et
mlb_cell_mobility_change_req_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    mlb_mobility_change_req_t  *p_req = (mlb_mobility_change_req_t *)(p_msg);
    /*SPR 20523 START*/
    /*code deleted*/
    son_mob_param_range_t mobility_range;
    son_s8  new_cell_offset_connected = SON_NULL;
    /* CID 118136 Fix Start */
    son_q_offset_range_et new_cell_offset_connected_enum = SON_Q_OFFSET_RANGE_DB0;
    /* CID 118136 Fix End */
    son_s8 mapped_initial_cm_cell_offset; 
    son_s8 mapped_q_offset_max; 
    son_s8 mapped_q_offset_min;
    son_u8 neighbor_cell_found = SON_FALSE;
    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record 
                                                      = SON_PNULL;
    /*SPR 20523 END*/
    son_mlb_neigh_cell_info_t *p_neigh_cell_info = SON_PNULL;
    son_global_cell_id_t glbl_cell_id = {0};


    SON_UT_TRACE_ENTER();

    /*SPR 20523 START*/
    p_req->proposed_mobility_params = p_req->proposed_mobility_params/2; 
    /*SPR 20523 END*/

    /*SONMLB-- Centralized support start*/
    if (SON_MODE_CENTRALIZED == g_mlb_context.mlb_config_param.mlb_mode)
    {
        /*forward the SONMLB_MOBILITY_CHANGE_REQ message to SON-MIF to 
         *forward to OAM*/
        son_mlb_peer_offset_change_req_t offset_change_req;
        son_size_t offset_change_req_len =
            sizeof(son_mlb_peer_offset_change_req_t);
        SON_MEMSET(&offset_change_req, 0, offset_change_req_len);
        /*copy self(target) cell id*/
        SON_MEMCPY(&offset_change_req.peer_offset_change_req.trgt_cgi,
            &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
        /*copy source cell id*/
        SON_MEMCPY(&offset_change_req.peer_offset_change_req.src_cgi,
            &p_req->src_cgi1,
            sizeof(son_intra_rat_global_cell_id_t));
        /*copy proposed mobility params mobility range*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_INFO,
            "self cell id:[%d],nbr cell ID:[%d],proposed_mobility_params [%d]",
            son_convert_char_cell_id_to_int_cell_id(p_req->src_cgi2.cell_identity),
            son_convert_char_cell_id_to_int_cell_id(p_req->src_cgi1.cell_identity),
            p_req->proposed_mobility_params);

        offset_change_req.peer_offset_change_req.bitmask |= SON_OAM_MLB_PEER_PROPOSED_MOB_PARAMS_PRESENT;
        offset_change_req.peer_offset_change_req.proposed_mobility_params =
            p_req->proposed_mobility_params;
        son_create_send_buffer((son_u8 *)&offset_change_req,
            SON_MLB_MODULE_ID, SON_MIF_MODULE_ID,
            SONMLB_PEER_OFFSET_CHANGE_REQ, offset_change_req_len);
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    /*SONMLB-- Centralized support end*/

    if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
    {
        /*in mobility change req the src_cgi1 will be the serving cell id for us */
        p_son_mlb_neigh_cell_info_record = (son_mlb_neigh_cell_info_record_t *)
                                           list_find_node(&p_cell_ctxt->neigh_cell_info_head,
            &p_req->src_cgi1,
            son_mlb_object_id_keyof,
            son_compare_cell_id);

        if (p_son_mlb_neigh_cell_info_record != SON_PNULL)
        {
            p_neigh_cell_info = &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data;
            neighbor_cell_found = SON_TRUE;
            /*SPR 20523 START*/
            /*code deleted*/
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_INFO,
                    "mlb_x2_cell_mobility_change_req_handler:"
                    "initial cell offset [%d] : new calculated cell offset [%d],p_req->proposed_mobility_params [%d]\n",
                    p_neigh_cell_info->initial_cm_cell_offset,
                    new_cell_offset_connected,p_req->proposed_mobility_params);

            /*update new cell offset:If all other params in Mi-Ms calculation is zero,like hys,OCS,OFS 
             * Then, Proposed HO trigger change = (Mi-Ms)new - (Mi-Ms)old = -OCNnew - (-OCNold)
             * So, OCNnew = OCNold - Proposed HO trigger change
             * */
            mapped_q_offset_max = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_max);
            mapped_q_offset_min = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_min);
            mapped_initial_cm_cell_offset = son_map_q_offset_value(p_neigh_cell_info->initial_cm_cell_offset);

            new_cell_offset_connected = mapped_initial_cm_cell_offset + p_req->proposed_mobility_params;


            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "mlb_x2_cell_mobility_change_req_handler:"
                    "p_req->proposed_mobility_params[%d],new_cell_offset_connected[%d]\n",
                    p_req->proposed_mobility_params,new_cell_offset_connected);

            if((mapped_q_offset_max < new_cell_offset_connected)||(new_cell_offset_connected < mapped_q_offset_min))
            {
                son_calculate_mob_change_range(p_cell_ctxt,p_neigh_cell_info,&mobility_range);
                son_mlb_send_mobility_change_fail(p_cell_ctxt,
                        p_neigh_cell_info,
                        &mobility_range);
            }
            else
            { 
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_BRIEF,
                        "new proposed cell offset[%d]",
                        new_cell_offset_connected);
                /*calculate global cell id of nbr cell*/
                SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
                glbl_cell_id.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
                SON_MEMCPY(&glbl_cell_id.intra_rat_global_cell_id, &p_req->src_cgi1,
                        sizeof(son_intra_rat_global_cell_id_t));

                new_cell_offset_connected_enum = son_reverse_map_db_to_enum(new_cell_offset_connected,mapped_initial_cm_cell_offset);

                /*update anr table cell context*/
                if (SON_SUCCESS ==
                        son_mlb_update_anr_table(p_cell_ctxt->cell_id, glbl_cell_id.intra_rat_global_cell_id,
                            new_cell_offset_connected_enum, SON_TRUE,
                            p_neigh_cell_info->nbr_csg_id_present)) /*coverity_CID_26316 fix start-end*/
                {
                    /*update mlb context with new cell offset*/
                    p_neigh_cell_info->initial_cm_cell_offset = new_cell_offset_connected_enum; /*coverity_CID_26318 fix start-end*/
                    /*send mobility change ack to son-x2*/
                    son_mlb_send_mobility_change_ack(p_cell_ctxt, p_neigh_cell_info);

                    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                            SON_BRIEF,
                            "mlb_x2_cell_mobility_change_req_handler : ANR table updated successfully with cell offset [%d] \n",
                            p_neigh_cell_info->initial_cm_cell_offset);
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                            SON_ERROR,
                            "mlb_x2_cell_mobility_change_req_handler: Cell offset updation failed at ANR"
                            " nbr cell id in req msg[Cell Id: 0x%x]\n",
                            son_convert_char_cell_id_to_int_cell_id(p_req->src_cgi1.cell_identity));
                    son_mlb_send_mobility_change_fail(p_cell_ctxt,
                            p_neigh_cell_info,
                            SON_PNULL);

                }
            }
            /*SPR 20523 END*/
        }
    }

    /*nbr cell id received is not expected ack*/
    if (SON_TRUE != neighbor_cell_found)
    {
            /*SPR 17777 +-*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "Cell 0x%x received in mobility req does not exist ",
                son_convert_char_cell_id_to_int_cell_id(p_req->src_cgi1.cell_identity));
        LTE_GCC_UNUSED_PARAM(evnt_id);
        LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
            /*SPR 17777 +-*/
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_cell_mobility_change_req_handler */
/*Rel 3.0 Multi Sector code changes Stop*/

/******************************************************************************
 * Function Name  : son_mlb_mro_nbr_cell_status
 * Inputs         : serv_cell_id     serving cell id
 *                : nbr_cell_id      nbr cell id
 *                : nbr_cell_status  MLB is enabled or disbaled for a nbr cell
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : function to creta and send SONMLB_NBR_CELL_STATUS_IND  to SON-MRO
 *****************************************************************************/
son_void_t
son_mlb_mro_nbr_cell_status
(
    son_intra_rat_global_cell_id_t serv_cell_id,
    son_intra_rat_global_cell_id_t nbr_cell_id,
    mlb_nbr_cell_state_et nbr_cell_status
)
{
    SON_UT_TRACE_ENTER();
    mlb_nbr_cell_status_ind_t cell_status_ind;

    SON_MEMSET(&cell_status_ind, 0, sizeof(mlb_nbr_cell_status_ind_t));

    SON_MEMCPY(&cell_status_ind.serv_cell_id, &serv_cell_id, sizeof(son_intra_rat_global_cell_id_t));
    SON_MEMCPY(&cell_status_ind.nbr_cell_id, &nbr_cell_id, sizeof(son_intra_rat_global_cell_id_t));

    /*poulatting  proposed nbr HO trigger change params*/
    cell_status_ind.nbr_cell_state = nbr_cell_status;

    son_create_send_buffer((son_u8 *)&cell_status_ind, SON_MLB_MODULE_ID,
        SON_MRO_MODULE_ID, SONMLB_NBR_CELL_STATUS_IND,
        sizeof(mlb_nbr_cell_status_ind_t));

    SON_UT_TRACE_EXIT();
} /* son_mlb_mro_nbr_cell_status */

/******************************************************************************
 * Function Name  : mlb_cell_mobility_change_failure_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_MOBILITY_CHANGE_FAILURE.
 *****************************************************************************/
son_buf_retain_status_et
mlb_cell_mobility_change_failure_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
)
{
    SON_UT_TRACE_ENTER();
    son_u8                           neighbor_cell_found = SON_FALSE;
    son_s8 ho_trigger_change_serv = SON_NULL;
    /*SPR 20523 START*/
    /*code deleted*/
    /* CID 118132 Fix Start */
    son_s32 new_cell_offset_connected = SON_NULL;
    /* CID 118132 Fix End */
    /* CID 118135 Fix Start */
    son_q_offset_range_et new_cell_offset_connected_enum = SON_Q_OFFSET_RANGE_DB0;
    /* CID 118135 Fix End */
    son_s8 mapped_initial_cm_cell_offset; 
    son_s8 mapped_q_offset_max; 
    son_s8 mapped_q_offset_min;
    /*SPR 20523 END*/
    son_s8 ho_trigger_change_nbr = SON_NULL;
    sonmlb_mobility_change_failure_t *p_fail = (sonmlb_mobility_change_failure_t *)(p_msg);
    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL;
    son_mlb_neigh_cell_info_t *p_neigh_cell_info = SON_PNULL;
    son_bool_et send_cell_status_to_mro = SON_FALSE;

    /*SONMLB-- Centralized support start*/
    if (SON_MODE_CENTRALIZED == g_mlb_context.mlb_config_param.mlb_mode)
    {
        /*forward the response message to SON-MIF*/
        son_mlb_offset_change_resp_t offset_change_resp;
        son_size_t offset_change_resp_len =
            sizeof(son_mlb_offset_change_resp_t);
        SON_MEMSET(&offset_change_resp, 0, offset_change_resp_len);
        /*copy serving cell id*/
        SON_MEMCPY(&offset_change_resp.offset_change_resp.srv_cgi,
            &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
        /*copy target cell id*/
        offset_change_resp.offset_change_resp.bitmask |= SON_OAM_MLB_TRGT_CGI_PRESENT;
        SON_MEMCPY(&offset_change_resp.offset_change_resp.trgt_cgi,
            &p_fail->src_cgi2,
            sizeof(son_intra_rat_global_cell_id_t));
        offset_change_resp.offset_change_resp.mlb_status = SON_FAILURE;
        offset_change_resp.offset_change_resp.error_code =
            SON_NO_ERROR;
        /*copy mobility range*/
        if (p_fail->bitmask & MOBILITY_PARAM_RANGE_PRESENT)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "mobility_range present in "
                "centralized mode for nbr cell 0x%x,Mobility Range Upper Limit:[%d]"
                "Mobility Range Lower Limit:[%d]",
                son_convert_char_cell_id_to_int_cell_id(p_fail->src_cgi2.cell_identity),
                p_fail->mob_param_range.upper_limit, p_fail->mob_param_range.lower_limit);

            offset_change_resp.offset_change_resp.bitmask |= SON_OAM_MLB_MOBILITY_RANGE_PRESENT;
            offset_change_resp.offset_change_resp.mob_range =
                p_fail->mob_param_range;
        }

        son_create_send_buffer((son_u8 *)&offset_change_resp,
            SON_MLB_MODULE_ID, SON_MIF_MODULE_ID,
            SONMLB_OFFSET_CHANGE_RESP, offset_change_resp_len);
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    /*SONMLB-- Centralized support end*/

    if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
    {
        /*in mobility change req the src_cgi1 will be the serving cell id for us */
        p_son_mlb_neigh_cell_info_record = (son_mlb_neigh_cell_info_record_t *)
                                           list_find_node(&p_cell_ctxt->neigh_cell_info_head,
            (son_void_t *)&p_fail->src_cgi2,
            son_mlb_object_id_keyof,
            son_compare_cell_id);

        if ((p_son_mlb_neigh_cell_info_record != SON_PNULL) &&
            (SON_TRUE == p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.mob_change_req_sent))
        {
            p_neigh_cell_info = &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data;
            p_neigh_cell_info->mob_change_req_sent = SON_FALSE;
            neighbor_cell_found = SON_TRUE;

            /*SPR 20523 START*/
            mapped_q_offset_max = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_max);
            mapped_q_offset_min = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_min);
            mapped_initial_cm_cell_offset = son_map_q_offset_value(p_neigh_cell_info->initial_cm_cell_offset);
            /*SPR 20523 END*/

            if (p_fail->bitmask & MOBILITY_PARAM_RANGE_PRESENT)
            {
                /*calculate new HO trigger change for nbr and again create and send 
                 * mobility change req to SON-X2*/
                if (SON_SUCCESS ==
                    mlb_cell_calc_serv_ho_threshold(p_cell_ctxt, p_neigh_cell_info, &ho_trigger_change_serv,
                        &new_cell_offset_connected))
                {
                    /*SPR 20523 START*/

                    if(ho_trigger_change_serv < p_fail->mob_param_range.lower_limit)
                    {
                        ho_trigger_change_serv = p_fail->mob_param_range.lower_limit; 
                    }
                    else if(ho_trigger_change_serv > p_fail->mob_param_range.upper_limit)
                    {
                        ho_trigger_change_serv = p_fail->mob_param_range.upper_limit;
                    }

                    ho_trigger_change_nbr = ho_trigger_change_serv;

                    ho_trigger_change_nbr = (SON_Q_OFFSET_ABSOLUTE_VAL_DB_1)*ho_trigger_change_nbr;

                    new_cell_offset_connected = mapped_initial_cm_cell_offset + ho_trigger_change_serv;

                    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                            SON_DETAILED,
                            "mlb_x2_cell_mobility_change_failure_handler: New HO trigger change calulated[%d] .."
                            " for nbr cell id[Cell Id: 0x%x],new cell offset for serving after taking average[%d]\n",
                            ho_trigger_change_nbr,
                            son_convert_char_cell_id_to_int_cell_id(p_fail->src_cgi2.cell_identity),
                            new_cell_offset_connected);
                    if (new_cell_offset_connected >= mapped_q_offset_min &&
                            new_cell_offset_connected <= mapped_q_offset_max &&
                            new_cell_offset_connected != mapped_initial_cm_cell_offset)
                    {
                        new_cell_offset_connected_enum = son_reverse_map_db_to_enum(new_cell_offset_connected,mapped_initial_cm_cell_offset);
                        p_neigh_cell_info->new_cell_offset_connected = new_cell_offset_connected_enum;
                        son_mlb_send_mobility_change_req(p_cell_ctxt, p_neigh_cell_info,
                                ho_trigger_change_nbr);
                    }
                    else
                    {
                        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                                SON_WARNING,
                                "mlb_x2_cell_mobility_change_failure_handler: HO trigger range is out of bound .."
                                " [%d] it should be with in range mapped_q_offset_min [%d],mapped_q_offset_max [%d].nbr cell id[Cell Id: 0x%x]\n",
                                ho_trigger_change_nbr,mapped_q_offset_min,mapped_q_offset_max,
                                son_convert_char_cell_id_to_int_cell_id(p_fail->src_cgi2.cell_identity));
                        send_cell_status_to_mro = SON_TRUE;
                    }

                    /*code deleted*/
                }
                else
                {

                    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                            SON_ERROR,
                            "mlb_x2_cell_mobility_change_failure_handler: Cannot calculate the New CIO offset")
                }
                /*SPR 20523 END*/
            }
            else
            {
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_BRIEF,
                        "mlb_cell_mobility_change_failure_handler: HO trigger range has not received.."
                        " ignoring the msg and no need to update ANR.nbr cell id[Cell Id: 0x%x]\n",
                        son_convert_char_cell_id_to_int_cell_id(p_fail->src_cgi2.cell_identity));
                send_cell_status_to_mro = SON_TRUE;
            }
        }
    }

    /*nbr cell id received is not expected ack*/
    if (SON_TRUE != neighbor_cell_found)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "Cell received in mobility failiure is not the expected one"
                " nbr cell id received [Cell Id: 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(p_fail->src_cgi2.cell_identity));
    }

    /*MLB Enhancement : start*/
    if (SON_TRUE == send_cell_status_to_mro)
    {
        /*send SONMLB_NBR_CELL_STATUS_IND to MRO*/
        son_mlb_mro_nbr_cell_status(p_cell_ctxt->cell_id, p_neigh_cell_info->neigh_cell_id, MLB_NBR_STATE_DISABLED);
    }

    /*MLB Enhancement : End*/
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_cell_mobility_change_failure_handler */

/******************************************************************************
 * Function Name  : mlb_cell_mobility_change_ack_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_MOBILITY_CHANGE_ACK.
 *****************************************************************************/
son_buf_retain_status_et
mlb_cell_mobility_change_ack_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    SON_UT_TRACE_ENTER();
    son_global_cell_id_t           glbl_cell_id;
    son_u8 neighbor_cell_found = SON_FALSE;
    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL;
    son_mlb_neigh_cell_info_t *p_neigh_cell_info = SON_PNULL;

    if (p_cell_ctxt == SON_PNULL)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "p_cell_ctxt is NULL");
    }
    else
    {
        sonmlb_mobility_change_ack_t *p_ack = (sonmlb_mobility_change_ack_t *)(p_msg);

        /*SONMLB-- Centralized support start*/
        if (SON_MODE_CENTRALIZED == g_mlb_context.mlb_config_param.mlb_mode)
        {
            /*forward the response message to SON-MIF*/
            son_mlb_offset_change_resp_t offset_change_resp;
            son_size_t offset_change_resp_len =
                sizeof(son_mlb_offset_change_resp_t);
            SON_MEMSET(&offset_change_resp, 0, offset_change_resp_len);
            /*copy serving cell id*/
            SON_MEMCPY(&offset_change_resp.offset_change_resp.srv_cgi,
                &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
            /*copy target cell id*/
            offset_change_resp.offset_change_resp.bitmask |= SON_OAM_MLB_TRGT_CGI_PRESENT;
            SON_MEMCPY(&offset_change_resp.offset_change_resp.trgt_cgi,
                &p_ack->src_cgi2,
                sizeof(son_intra_rat_global_cell_id_t));
            offset_change_resp.offset_change_resp.mlb_status = SON_SUCCESS;
            offset_change_resp.offset_change_resp.error_code =
                SON_NO_ERROR;
            son_create_send_buffer((son_u8 *)&offset_change_resp,
                SON_MLB_MODULE_ID, SON_MIF_MODULE_ID,
                SONMLB_OFFSET_CHANGE_RESP, offset_change_resp_len);

            SON_UT_TRACE_EXIT();
            return RELEASE_SON_BUFFER;
        }

        /*SONMLB-- Centralized support end*/

        if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
        {
            /*in mobility change req the src_cgi1 will be the serving cell id for us */
            p_son_mlb_neigh_cell_info_record = (son_mlb_neigh_cell_info_record_t *)
                                               list_find_node(&p_cell_ctxt->neigh_cell_info_head,
                (son_void_t *)&p_ack->src_cgi2,
                son_mlb_object_id_keyof,
                son_compare_cell_id);

            if ((p_son_mlb_neigh_cell_info_record != SON_PNULL) &&
                (SON_TRUE == p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.mob_change_req_sent))
            {
                p_neigh_cell_info = &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data;
                /*reset mob_change_req_sent flag*/
                p_neigh_cell_info->mob_change_req_sent = SON_FALSE;
                neighbor_cell_found = SON_TRUE;
                /*calculate global cell id of nbr cell*/
                SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
                glbl_cell_id.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
                SON_MEMCPY(&glbl_cell_id.intra_rat_global_cell_id, &p_ack->src_cgi2,
                    sizeof(son_intra_rat_global_cell_id_t));
                /*update anr table cell context*/
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_BRIEF,
                        "initial cell offset [%d] : new cell offset [%d]",
                        p_neigh_cell_info->initial_cm_cell_offset,
                        p_neigh_cell_info->new_cell_offset_connected);

                if (SON_SUCCESS == son_mlb_update_anr_table(p_cell_ctxt->cell_id, glbl_cell_id.intra_rat_global_cell_id,
                        (son_q_offset_range_et)p_neigh_cell_info->new_cell_offset_connected, SON_TRUE,
                                    p_neigh_cell_info->nbr_csg_id_present)) /*coverity_CID_26317 fix start-end*/
                {
                    /*update mlb context with new cell offset*/
                    p_neigh_cell_info->initial_cm_cell_offset =
                        (son_q_offset_range_et)p_neigh_cell_info->new_cell_offset_connected; /*coverity_CID_26321 start-end*/
                    /*MLB Enhancement : start*/
                    p_neigh_cell_info->num_of_mlb_iteration++;
                    /*MLB Enhancement : End*/
                }
                /*MLB Enhancement : start
                 *send SONMLB_NBR_CELL_STATUS_IND to MRO*/
                son_mlb_mro_nbr_cell_status(p_cell_ctxt->cell_id,
                    p_neigh_cell_info->neigh_cell_id,
                    MLB_NBR_STATE_DISABLED);
                /*MLB Enhancement : End*/
            }
        }

        /*nbr cell id received is not expected ack*/
        if (SON_TRUE != neighbor_cell_found)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_INFO,
                    "Cell 0x%x received in mobility ack is not expected in ack",
                    son_convert_char_cell_id_to_int_cell_id(p_ack->src_cgi2.cell_identity));
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_cell_mobility_change_ack_handler */

/*****************************************************************************
 * Function Name  : mlb_construct_rrm_deregister_req
 * Inputs         : transaction_id   -    transaction identity
 *                : p_cell_id       -     Pointer to Cell_Id
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : Constructs and sends RRM_DEREGISTER_REQ
 ****************************************************************************/
son_return_et
mlb_construct_send_rrm_deregister_req
(
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
    son_rrm_deregister_req_t req = {0};
    son_return_et ret_val = SON_SUCCESS;


    SON_UT_TRACE_ENTER();

    req.transaction_id = transaction_id;
    req.req.deregister_req.object.type = RRM_OBJ_CELL;

    SON_MEMCPY(&(req.req.deregister_req.object.id.object_id.cell_id), p_cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    req.req.deregister_req.object.attr_list_size = 4;

    /* CELL STATE DEREGISTERATION */
    req.req.deregister_req.object.p_attr_lst[0].id = RRM_ATTR_CELL_STATE;
    req.req.deregister_req.object.p_attr_lst[0].trgr_list_size = 0;

    /* Load reporting DEREGISTRATION */
    req.req.deregister_req.object.p_attr_lst[1].id =
        RRM_ATTR_LOAD_REPORT;
    req.req.deregister_req.object.p_attr_lst[1].trgr_list_size = 0;

    /* HO Report DEREGISTRATION */
    req.req.deregister_req.object.p_attr_lst[2].id =
        RRM_ATTR_HO_REPORT;
    req.req.deregister_req.object.p_attr_lst[2].trgr_list_size = 0;

    /* Neighbor Load Report DEREGISTRATION */
    req.req.deregister_req.object.p_attr_lst[3].id =
        RRM_ATTR_NEIGHBOR_LOAD_REPORT;
    req.req.deregister_req.object.p_attr_lst[3].trgr_list_size = 0;

    /*Invoke PUP tool call to send message to RRM*/
    if (RRM_FAILURE == rrm_son_send_rrm_son_deregister_req(&req.req,
                SON_MLB_MODULE_ID, SON_RRM_MODULE_ID,
                transaction_id, SON_NULL))
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "MLB->RRM:RRM_SON_DEREGISTER_REQ send failed"
                " for Cell 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
        ret_val = SON_FAILURE;
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mlb_construct_send_rrm_deregister_req */

/*SONMLB-- Centralized support start*/

/******************************************************************************
 * Function Name  : mlb_fsm_offset_change_req_handler
 * Inputs         : p_cell_context    Pointer to mlb cell context
 *                : p_msg             Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_OFFSET_CHANGE_REQ.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_offset_change_req_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
)
{
    son_mlb_offset_change_req_t *p_req = (son_mlb_offset_change_req_t *)p_msg;
    son_mlb_offset_change_resp_t offset_change_res = {0};
    son_mlb_neigh_cell_info_t neigh_cell_info;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&neigh_cell_info, 0, sizeof(son_mlb_neigh_cell_info_t));
    SON_MEMCPY(&(offset_change_res.offset_change_resp.srv_cgi),
        &p_cell_ctxt->cell_id,
        sizeof(son_intra_rat_global_cell_id_t));
    /* transaction ID needs to be saved.below line has to be changed */
    offset_change_res.transaction_id = p_req->transaction_id;
    if (p_req->offset_change_req.bitmask & SON_OAM_MLB_PROPOSED_MOB_PARAMS_PRESENT)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_INFO,
            "Trgt Cell Id: %d]"
            "proposed_mobility_params: %d",
            son_convert_char_cell_id_to_int_cell_id(p_req->offset_change_req.trgt_cgi.cell_identity),
            p_req->offset_change_req.proposed_mobility_params);

        /*send mobility change req on X2*/
        SON_MEMCPY(&p_cell_ctxt->serv_cell_load_info.g_cell_id, &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
        SON_MEMCPY(&neigh_cell_info.neigh_cell_load_info.g_cell_id, &p_req->offset_change_req.trgt_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        if (p_req->offset_change_req.bitmask & SON_OAM_MLB_TRGT_CSG_ID_PRESENT)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_DETAILED,
                "TRGT CSG ID PRESENT FLAG: %d",
                p_req->offset_change_req.trgt_csg_id_present);

            neigh_cell_info.nbr_csg_id_present = p_req->offset_change_req.trgt_csg_id_present;
        }

/*Rel 3.0 Multi Sector code changes Start*/
      	son_mlb_send_mobility_change_req(p_cell_ctxt,
            		&neigh_cell_info,
            		p_req->offset_change_req.proposed_mobility_params);
/*Rel 3.0 Multi Sector code changes Stop*/
    }
    else
    {
        /*send failure response to SON-MIF*/
        offset_change_res.transaction_id = p_req->transaction_id;
        offset_change_res.offset_change_resp.mlb_status = SON_FAILURE;
        offset_change_res.offset_change_resp.error_code = SON_ERR_INVALID_PARAMS;
        son_create_send_buffer((son_u8 *)&offset_change_res, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SONMLB_OFFSET_CHANGE_RESP,
            sizeof(offset_change_res));
                    /*SPR 17777 +-*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_INFO,
            "Invalid offset change request"
            " Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_req->offset_change_req.srv_cgi.cell_identity));
        LTE_GCC_UNUSED_PARAM(evnt_id);
                    /*SPR 17777 +-*/

        return RELEASE_SON_BUFFER;
    }

    SON_UT_TRACE_EXIT();

    return RELEASE_SON_BUFFER;
} /* mlb_fsm_offset_change_req_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_peer_offset_change_resp_handler
 * Inputs         : p_cell_context    Pointer to mlb cell context
 *                : p_msg             Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_PEER_OFFSET_CHANGE_RESP.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_peer_offset_change_resp_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
)
{
    son_mlb_peer_offset_change_resp_t *p_resp = (son_mlb_peer_offset_change_resp_t *)p_msg;
    son_mlb_neigh_cell_info_t neigh_cell_info;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&neigh_cell_info, 0, sizeof(son_mlb_neigh_cell_info_t));
    /*copy self cell id to g_cell_id*/
    SON_MEMCPY(&p_cell_ctxt->serv_cell_load_info.g_cell_id, &p_cell_ctxt->cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    /*check for serving is macro or femto*/
    if ((g_mlb_context.enb_type == SON_HOME_ENB))
    {
        p_cell_ctxt->src_csg_id_present = SON_TRUE;
    }
    else
    {
        p_cell_ctxt->src_csg_id_present = SON_FALSE;
    }

    if (p_resp->peer_offset_change_resp.mlb_status == SON_SUCCESS)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_INFO,
            "Send MOBILITY CHANGE"
            "ACK to X2 :"
            "[Neighbor Cell 0x%x]"
            "mlb_status: %d",
            son_convert_char_cell_id_to_int_cell_id(p_resp->peer_offset_change_resp.src_cgi.cell_identity),
            p_resp->peer_offset_change_resp.mlb_status);

        /*send mobility change ack on X2*/
        SON_MEMCPY(&neigh_cell_info.neigh_cell_load_info.g_cell_id, &p_resp->peer_offset_change_resp.src_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        if (p_resp->peer_offset_change_resp.bitmask & SON_OAM_MLB_SRC_CSG_ID_PRESENT)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_DETAILED,
                "nbr CSG ID flag [%d] :",
                p_resp->peer_offset_change_resp.src_csg_id_present);

            neigh_cell_info.nbr_csg_id_present = p_resp->peer_offset_change_resp.src_csg_id_present;
        }

        son_mlb_send_mobility_change_ack(p_cell_ctxt, &neigh_cell_info);
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_INFO,
            "Send MOBILITY CHANGE "
            "FAILURE to X2 [NBR Cell 0x%x"
            "mlb_status: %d ",
            son_convert_char_cell_id_to_int_cell_id(p_resp->peer_offset_change_resp.src_cgi.cell_identity),
            p_resp->peer_offset_change_resp.mlb_status);

        if (p_resp->peer_offset_change_resp.bitmask & SON_OAM_MLB_PEER_MOBILITY_RANGE_PRESENT)
        {
                    /*SPR 17777 +-*/
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_DETAILED,
                "mob_range upper_limit: %d "
                "mob_range lower_limit: %d "
                "evnt_id=%d p_cspl_hdr=%p",
                p_resp->peer_offset_change_resp.mob_range.upper_limit,
                p_resp->peer_offset_change_resp.mob_range.lower_limit,
                evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
        }

        /*send mobility change fail on X2*/
        SON_MEMCPY(&neigh_cell_info.neigh_cell_load_info.g_cell_id, &p_resp->peer_offset_change_resp.src_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        if (p_resp->peer_offset_change_resp.bitmask & SON_OAM_MLB_SRC_CSG_ID_PRESENT)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_DETAILED,
                "nbr CSG ID flag [%d]",
                p_resp->peer_offset_change_resp.src_csg_id_present);

            neigh_cell_info.nbr_csg_id_present = p_resp->peer_offset_change_resp.src_csg_id_present;
        }
        /*SPR 20523 START*/
        son_mlb_send_mobility_change_fail(p_cell_ctxt, &neigh_cell_info,
            &p_resp->peer_offset_change_resp.mob_range);
        /*SPR 20523 END*/
    }

    SON_UT_TRACE_EXIT();

    return RELEASE_SON_BUFFER;
} /* mlb_fsm_peer_offset_change_resp_handler */

/*SONMLB-- Centralized support end*/
/* eICIC feature code Starts */
/**********************************************************************************
 * Function Name  : mlb_fsm_eicic_enable_disable_event_handler
 * Inputs         : p_cell_context    Pointer to mlb cell context
 *                : p_msg             Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific RRM_SON_EICIC_ENABLE_DISABLE_IND.
 ************************************************************************************/
son_buf_retain_status_et
mlb_fsm_eicic_enable_disable_event_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
)
{
    rrm_son_eicic_enable_disable_ind_t      *p_ind = SON_PNULL;

    p_ind = (rrm_son_eicic_enable_disable_ind_t *)p_msg;

    SON_UT_TRACE_ENTER();
    
                    /*SPR 17777 +-*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
            "Received eICIC enable and disable indication"
            "Serving Cell id : [ %u ] "
            "Provision_type received : [ %u ] "
            " evnt_id=%d p_cspl_hdr=%p",
            son_convert_char_cell_id_to_int_cell_id(p_ind->cell_id.cell_identity),
            p_ind->eicic_provisioned_type,
            evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    /** Provisioned type received from RRM to trigger the CRE algo for serving 
        or neighboring cells */
    switch(p_ind->eicic_provisioned_type)
    {
        case RRM_SON_EICIC_DEACTIVATED :
        {
            p_cell_ctxt->provision_type = SON_EICIC_DEACTIVATED;
            break;
        }
        case RRM_SON_ACT_AS_AGGRESSOR :
        {
            p_cell_ctxt->provision_type = SON_ACT_AS_AGGRESSOR;
            break;
        }
        case RRM_SON_ACT_AS_VICTIM :
        {
            p_cell_ctxt->provision_type = SON_ACT_AS_VICTIM;
            break;
        }
        case RRM_SON_ACT_AS_AGGRESSOR_AND_VICTIM :
        {
            p_cell_ctxt->provision_type = SON_ACT_AS_AGGRESSOR_AND_VICTIM;
            break;
        }
        default :
            p_cell_ctxt->provision_type = SON_EICIC_DEACTIVATED;
    }
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}/*mlb_fsm_eicic_enable_disable_event_handler */
/* eICIC feature code Ends */
/************************* MLB Cell Specific State Machine*********************/

/*****************************************************************************
 * FSM Entry Point Table
 *****************************************************************************/
mlb_cell_evt_handler_t 
mlb_cell_fsm_table[MLB_CELL_NUM_OF_STATES][SON_MLB_CELL_MAX_EVENT] =
{
    /* State DISABLING */
    {
        mlb_fsm_nack_enable_req_handler,
        /* SONMLB_ENABLE_REQ*/
        mlb_fsm_nack_disable_req_handler,
        /* SONMLB_DISABLE_REQ*/
        mlb_fsm_modify_req_handler,
        /* SONMLB_MODIFY_REQ */
        mlb_fsm_invalid_event_handler,
        /* RRM_REGISTER_RES*/
        mlb_fsm_deregister_res_handler,
        /* RRM_DEREGISTER_RES*/
        mlb_fsm_invalid_event_handler,
        /* RRM_SON_HO_REPORT*/
        mlb_fsm_invalid_event_handler,
        /* RRM_SON_LOAD_REPORT*/
        mlb_fsm_invalid_event_handler,
        /* SON_ANR_DISABLE_IND */
        mlb_fsm_state_change_handler,
        /* RRM_CELL_STATE_CHANGE_IND */
        mlb_fsm_invalid_event_handler,
        /*SON_MOBILITY_CHANGE_REQ*/
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_fsm_invalid_event_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_ACK*/
/*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_invalid_event_handler,
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_invalid_event_handler,
        /*SON_MOBILITY_CHANGE_ACK */
        mlb_fsm_invalid_event_handler,
        /* SONANR_NBR_DELETE_IND*/
        mlb_fsm_invalid_event_handler,
        /* SONMLB_OFFSET_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SONMLB_PEER_OFFSET_CHANGE_RESP*/
/* eICIC feature code Starts */
        mlb_fsm_invalid_event_handler,
        /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /* State DISABLED */
    {
        mlb_fsm_enable_req_handler,
        /* SONMLB_ENABLE_REQ*/
        mlb_fsm_nack_disable_req_handler,
        /* SONMLB_DISABLE_REQ*/
        mlb_fsm_modify_req_handler,
        /* SONMLB_MODIFY_REQ */
        mlb_fsm_invalid_event_handler,
        /* RRM_REGISTER_RES*/
        mlb_fsm_invalid_event_handler,
        /* RRM_DEREGISTER_RES*/
        mlb_fsm_invalid_event_handler,
        /* RRM_SON_HO_REPORT*/
        mlb_fsm_invalid_event_handler,
        /* RRM_SON_LOAD_REPORT*/
        mlb_fsm_invalid_event_handler,
        /* SON_ANR_DISABLE_IND */
        mlb_fsm_invalid_event_handler,
        /* RRM_CELL_STATE_CHANGE_IND */
        mlb_fsm_invalid_event_handler,
        /* SON_MOBILITY_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SON_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_invalid_event_handler,
        /* SON_MOBILITY_CHANGE_ACK*/
        /*Rel 3.0 Multi Sector code changes Start*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_ACK*/
        /*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_invalid_event_handler,
        /* SONANR_NBR_DELETE_IND*/
        mlb_fsm_invalid_event_handler,
        /* SONMLB_OFFSET_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SONMLB_PEER_OFFSET_CHANGE_RESP*/
/* eICIC feature code Starts */
        mlb_fsm_invalid_event_handler,
        /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /* State ENABLING */
    {
        mlb_fsm_nack_enable_req_handler,
        /* SONMLB_ENABLE_REQ*/
        mlb_fsm_nack_disable_req_handler,
        /* SONMLB_DISABLE_REQ*/
        mlb_fsm_modify_req_handler,
        /* SONMLB_MODIFY_REQ */
        mlb_fsm_register_res_handler,
        /* RRM_REGISTER_RES*/
        mlb_fsm_invalid_event_handler,
        /* RRM_DEREGISTER_RES*/
        mlb_fsm_invalid_event_handler,
        /* RRM_SON_HO_REPORT*/
        mlb_fsm_invalid_event_handler,
        /* RRM_SON_LOAD_REPORT*/
        mlb_fsm_invalid_event_handler,
        /* SON_ANR_DISABLE_IND */
        mlb_fsm_invalid_event_handler,
        /* RRM_CELL_STATE_CHANGE_IND */
        mlb_fsm_invalid_event_handler,
        /* SON_MOBILITY_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SON_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_invalid_event_handler,
        /* SON_MOBILITY_CHANGE_ACK*/
 /*Rel 3.0 Multi Sector code changes Start*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_invalid_event_handler,
        /* SON_INTERNAL_MOBILITY_CHANGE_ACK*/
 /*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_invalid_event_handler,
        /* SONANR_NBR_DELETE_IND*/
        mlb_fsm_invalid_event_handler,
        /* SONMLB_OFFSET_CHANGE_REQ*/
        mlb_fsm_invalid_event_handler,
        /* SONMLB_PEER_OFFSET_CHANGE_RESP*/
/* eICIC feature code Starts */
        /* SPR 14397 fix start */
        mlb_fsm_eicic_enable_disable_event_handler,
        /* SPR 14397 fix end */
        /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /* State ENABLED */
    {
        mlb_fsm_nack_enable_req_handler,
        /* SONMLB_ENABLE_REQ*/
        mlb_fsm_disable_req_handler,
        /* SONMLB_DISABLE_REQ*/
        mlb_fsm_modify_req_handler,
        /* SONMLB_MODIFY_REQ */
        mlb_fsm_invalid_event_handler,
        /* RRM_REGISTER_RES*/
        mlb_fsm_invalid_event_handler,
        /* RRM_DEREGISTER_RES*/
        mlb_fsm_ho_report_handler,
        /* RRM_SON_HO_REPORT*/
        mlb_fsm_load_report_handler,
        /* RRM_SON_LOAD_REPORT*/
        mlb_fsm_anr_disable_ind_handler,
        /* SON_ANR_DISABLE_IND */
        mlb_fsm_state_change_handler,
        /* RRM_CELL_STATE_CHANGE_IND */
        mlb_cell_mobility_change_req_handler,
        /*SON_MOBILITY_CHANGE_REQ*/
        mlb_cell_mobility_change_failure_handler,
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_cell_mobility_change_ack_handler,
        /*SON_MOBILITY_CHANGE_ACK */

/*Rel 3.0 Multi Sector code changes Start*/
        mlb_cell_mobility_change_req_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_cell_mobility_change_failure_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_cell_mobility_change_ack_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_fsm_nbr_update_info_handler,
        /* SONANR_NBR_DELETE_IND*/
        mlb_fsm_offset_change_req_handler,
        /*SONMLB_OFFSET_CHANGE_REQ */
        mlb_fsm_peer_offset_change_resp_handler,
        /*SONMLB_PEER_OFFSET_CHANGE_RESP */
/* eICIC feature code Starts */
        mlb_fsm_eicic_enable_disable_event_handler,
        /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    }
};

/*****************************************************************************
 * Function Name  : mlb_cell_process_msg
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_mlb_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the entry point to the es Cell FSM.
 *                  This function takes the API message received,event_id,cell context
 *                  and calls the required event handler function.
 ****************************************************************************/
son_buf_retain_status_et
mlb_cell_process_msg
(
 son_mlb_index_et     evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_mlb_msg
 )
{
    mlb_cell_fsm_state_et  current_state = p_cell_ctxt->current_cell_fsm_state;
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;


    SON_UT_TRACE_ENTER();

    /*Klockwork Fix Starts*/
    if (current_state < MLB_CELL_NUM_OF_STATES && evnt_id < SON_MLB_CELL_MAX_EVENT) /*coverty_CID_21499 start-end*/
    /*Klockwork Fix Ends*/    
    {
        SON_LOG(anr_get_log_mode(), p_son_mlb_facility_name, SON_INFO,
              "### MLB_Cell_FSM: Cell: 0x%x Processing state %s Event %s ###",
               son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
               SON_MLB_CELL_FSM_STATES_NAMES[current_state],
               SON_MLB_CELL_FSM_EVENT_NAMES[evnt_id]);

        ret = (*mlb_cell_fsm_table[current_state][evnt_id])(evnt_id, p_cell_ctxt, p_cspl_hdr, p_mlb_msg);

        SON_LOG(anr_get_log_mode(), p_son_mlb_facility_name, SON_INFO,
              "### MLB_Cell_FSM: Processing Complete ###");
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
              "Error: Unexpected Event or State cell_id: 0x%x State: %d Event: %d",
               son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
               current_state, evnt_id);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_cell_process_msg */

/******************************************************************************
 * Function Name  : mlb_fsm_mlb_enable_req_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        :    son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_ENABLE_REQ.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_enable_req_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t  *p_cell_ctxt,
 STACKAPIHDR        *p_cspl_hdr,
 son_u8               *p_msg
 )
{
    son_mlb_enable_req_t         *p_req = (son_mlb_enable_req_t *)(p_msg);
    anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    son_error_et error_code = SON_NO_ERROR;
    son_feature_state_et state = SON_STATE_UNDEFINED;
    son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;


    SON_UT_TRACE_ENTER();

    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
            &p_cell_ctxt->cell_id);
    if (SON_PNULL != p_anr_cell_ctxt && 
            ((p_anr_cell_ctxt->current_cell_fsm_state == ANR_CELL_STATE_ENABLED) || 
            (p_anr_cell_ctxt->current_cell_fsm_state == ANR_CELL_STATE_ENABLING)))
    {
        if (SON_TRUE == mlb_construct_send_rrm_register_req(
                    p_req->transaction_id,
                    &p_req->enable_req.src_cgi))
        {
            SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MLB_CELL_STATE_ENABLING);
        }
        else
        {
            error_code = SON_ERR_COMPOSER_ERROR;
            SON_LOG(mlb_get_log_mode(), 
                    p_son_mlb_facility_name,
                    SON_ERROR,
                    "Message sending "
                    "failure to RRM evnt_id=%u",evnt_id);
                    /*SPR 17777 +-*/
        }
    }
    else if (SON_PNULL == p_anr_cell_ctxt)
    {
        error_code = SON_ERR_CELL_UNCONFIGURED_AT_ANR;
        cause = SON_ANR_CELL_UNCONFIGURED;
        state = SON_STATE_DISABLED;
    }
    else
    {
        error_code = SON_ERR_ANR_MODE_IS_DISABLED;
        cause = SON_ANR_MODE_DISABLED;
        state = SON_STATE_DISABLED;
    }

    if (SON_NO_ERROR != error_code)
    {
        cell_status_list_data_t *p_list_node = SON_PNULL;
        /* Search for the cell Id in pending response structure. */
        p_list_node = mlb_find_cell_in_pending_res(&(p_cell_ctxt->cell_id));

        /* Presence indicates that this event is triggered due to Global
         * Enable Request or Cell Info Indication and no immediate Enable
         * Response must be sent from cell FSM */
        if (SON_PNULL != p_list_node)
        {
            /* Update Cell in MLB Pending Response List with error
             * code determined above */
            p_list_node->cell_status.error_code = error_code;
            /* Decrement expected_response_count by 1 */
            mlb_decrement_pending_res_expected_count();
            if (SMIF_CELL_INFO_IND == p_cspl_hdr->api)
            {
                mlb_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
                        state, cause);
            }
        }
        /* Cell specific Enable Request is being processed */
        else
        {
            son_mlb_enable_res_t enable_res;
            son_size_t enable_res_len = sizeof(son_mlb_enable_res_t);
            /* Send SONMLB_ENABLE_RES to SONMgmtIfH with faliure */
            SON_MEMSET(&enable_res, 0, enable_res_len);
            enable_res.transaction_id = p_req->transaction_id;
            enable_res.enable_res.result = SON_FAILURE;
            enable_res.enable_res.error_code = SON_NO_ERROR;
            enable_res.enable_res.cell_status_list_size = 1;
            SON_MEMCPY(&enable_res.enable_res.cell_status_list[0].cgi,
                    &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));
            enable_res.enable_res.cell_status_list[0].error_code = error_code;
            son_create_send_buffer((son_u8 *)&enable_res,
                    SON_MLB_MODULE_ID, SON_MIF_MODULE_ID, SONMLB_ENABLE_RES,
                    enable_res_len);
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_enable_req_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_invalid_event_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for logging invalid event it is invoked when
 *                     an unexpected event occurs..
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_invalid_event_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t     *p_cell_ctxt,
 STACKAPIHDR        *p_cspl_hdr,
 son_u8                *p_msg
 )
{    
    SON_UT_TRACE_ENTER();
                    /*SPR 17777 +-*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected Message %s received in "
            "%s state for the cell %x from %d p_msg=%p",
            SON_MLB_CELL_FSM_EVENT_NAMES[evnt_id],
            SON_MLB_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
            p_cspl_hdr->from,
            p_msg);
                    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();

    return RELEASE_SON_BUFFER;
}

/******************************************************************************
 * Function Name  : mlb_construct_send_rrm_register_req 
 * Inputs         : transaction_id -  Transaction Id
 *                : p_cell_id  - Pointer to Cell_Id
 * Outputs        : None
 * Returns        :    son_bool_et
 * Description    : Constructs RRM_REGISTER_REQ
 *****************************************************************************/
son_bool_et 
mlb_construct_send_rrm_register_req
(
 son_u16    transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
    son_rrm_register_req_t reg_req = {0};
    son_bool_et ret_val      = SON_FALSE;
    son_u8 attr_list_size = 0;
    son_u8 attr_list_index = 0;


    SON_UT_TRACE_ENTER();

    reg_req.transaction_id = transaction_id;
    reg_req.req.register_req.object.type = RRM_OBJ_CELL;

    SON_MEMCPY(&(reg_req.req.register_req.object.id.object_id.cell_id), 
            p_cell_id, sizeof(son_intra_rat_global_cell_id_t));

    attr_list_size++;

    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        id = RRM_ATTR_HO_REPORT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list_size = 1;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].id = RRM_TRGR_EVENT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].bitmask = RRM_TRIGGER_TYPE_EVENT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].event.enable = SON_TRUE;

    attr_list_size++;
    attr_list_index++;

    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        id = RRM_ATTR_LOAD_REPORT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list_size = 1;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].id = RRM_TRGR_PERIODIC;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].bitmask = RRM_TRIGGER_TYPE_PERIODIC;
    /*SPR 22006 Start*/
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].periodic.interval = 10;

    attr_list_size++;
    attr_list_index++;

    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        id = RRM_ATTR_CELL_STATE;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list_size = 1;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].id = RRM_TRGR_EVENT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].bitmask = RRM_TRIGGER_TYPE_EVENT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].event.enable = SON_TRUE;

    attr_list_size++;
    attr_list_index++;

    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        id = RRM_ATTR_NEIGHBOR_LOAD_REPORT;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list_size = 1;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].id = RRM_TRGR_PERIODIC;
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].bitmask = RRM_TRIGGER_TYPE_PERIODIC;
    /*SPR 22006 Start*/
    reg_req.req.register_req.object.p_attr_lst[attr_list_index].
        trgr_list[0].periodic.interval = 10;

    reg_req.req.register_req.object.attr_list_size = attr_list_size;

    /*PUP Tool call to create and send REGISTER Message to RRM */
    if (RRM_SUCCESS == rrm_son_send_rrm_son_register_req(&reg_req.req,
                SON_MLB_MODULE_ID, SON_RRM_MODULE_ID,
                transaction_id, SON_NULL))
    {
        ret_val = SON_TRUE;
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), 
                p_son_mlb_facility_name,
                SON_ERROR,
                "MLB->RRM:RRM_SON_REGISTER_REQ send failed for Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mlb_construct_send_rrm_register_req */

/******************************************************************************
 * Function Name  : mlb_fsm_nack_disable_req_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for SONMLB_DISABLE_REQ, when received in
 *                     unexpected state.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_nack_disable_req_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t  *p_cell_ctxt,
 STACKAPIHDR        *p_cspl_hdr,
 son_u8                *p_msg
 )
{
    son_mlb_disable_res_t          disable_resp = {0};
    son_mlb_disable_req_t          *p_req = (son_mlb_disable_req_t *)p_msg;


    disable_resp.disable_res.cell_status_list_size = 1;
    disable_resp.disable_res.cell_status_list[0].error_code = 
        SON_ERR_ALREADY_DISABLED;
    disable_resp.disable_res.result = SON_FAILURE;

    SON_UT_TRACE_ENTER();

    if (MLB_CELL_STATE_DISABLING == p_cell_ctxt->current_cell_fsm_state)
    {
        disable_resp.disable_res.cell_status_list[0].error_code = 
            SON_ERR_DISABLE_IN_PROGRESS;
    }

    if (MLB_CELL_STATE_ENABLING == p_cell_ctxt->current_cell_fsm_state)
    {
        disable_resp.disable_res.cell_status_list[0].error_code = 
            SON_ERR_ENABLE_IN_PROGRESS;
    }

    disable_resp.transaction_id = 
        ((son_mlb_disable_req_t *)p_msg)->transaction_id;
    SON_MEMCPY(&(disable_resp.disable_res.cell_status_list[0].cgi),
            &p_req->disable_req.src_cgi, 
        sizeof(son_intra_rat_global_cell_id_t));

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected Msg %s received in "
            "%s state for cell: %x from %d. SONMLB_DISABLE_RES Msg sent to %d "
            "with Error Code: %d ",
            SON_MLB_CELL_FSM_EVENT_NAMES[evnt_id],
            SON_MLB_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
            p_cspl_hdr->from, p_cspl_hdr->to,
            disable_resp.disable_res.cell_status_list[0].error_code);

    son_create_send_buffer((son_u8 *)&disable_resp, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SONMLB_DISABLE_RES,
            sizeof(son_mlb_disable_res_t));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nack_disable_req_handler */

/*****************************************************************************
 * Function Name  : mlb_fsm_deregister_res_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for the RRM_DEREGISTER_RES. It is invoked
 *                  when mlb state for the cell is MLB_CELL_STATE_DISABLING.
 ****************************************************************************/
son_buf_retain_status_et
mlb_fsm_deregister_res_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8                *p_msg
 )
{
    son_return_et result = SON_FAILURE;
    son_rrm_deregistration_res_t *p_res =
        (son_rrm_deregistration_res_t *)p_msg;
    son_mlb_disable_res_t res;
    son_error_et error_code = SON_NO_ERROR;
    son_size_t disable_res_len = sizeof(son_mlb_disable_res_t);


    SON_UT_TRACE_ENTER();

    result = ((p_res->res.deregistration_resp.result == RRM_SUCCESS) ?
            SON_SUCCESS : SON_FAILURE);
    if (SONANR_DISABLE_IND == p_cell_ctxt->dereg_invoking_api)
    {
        p_cell_ctxt->dereg_invoking_api = SON_PROCEDURE_UNDEFINED;
                    /*SPR 17777 +-*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "mlb_fsm_deregister_res_handler: Deregister "
                "response received for MLB generated RRM_SON_REGISTER_REQ "
                "with result = %u for cell [Cell Id: 0x%x]"
                "evnt_id=%d p_cspl_hdr=%p",
                result, son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/

        /* Change cell FSM state to MLB_CELL_STATE_DISABLED */
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_DISABLED);

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    if (SON_FAILURE == result)
    {
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_ENABLED);
        error_code = mlb_map_rrm_error_codes(
            ( const son_s8 *)__func__,
                result, p_res->res.deregistration_resp.error_code,
                RRM_SON_DEREGISTER_RESP);
    }
    else
    {
        /* Change cell FSM state to MLB_CELL_STATE_DISABLED */
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_DISABLED);
    }

    if (p_res->transaction_id == mlb_get_pending_res_transaction_id())
    {
        cell_status_list_data_t *p_list_node = SON_PNULL;
        /* Search for the cell Id in pending response structure. */
        p_list_node = mlb_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

        /* Presence indicates that cell state is disabling due to Global
         * Disable Request */
        if (SON_PNULL != p_list_node)
        {
            /* Update Cell in MLB Pending Response List with error code
             * determined above */
            p_list_node->cell_status.error_code = error_code;
            /* Decrement expected_response_count by 1 */
            mlb_decrement_pending_res_expected_count();
            /* Check if expected response count == 0 */
            if (0 == mlb_get_pending_res_expected_count())
            {
                SON_MEMSET(&res, 0, disable_res_len);
                res.transaction_id = p_res->transaction_id;
                res.disable_res.error_code = SON_NO_ERROR;

                /* Remove Successful cells from the
                 * MLB_Pending_Response_LIST and create cell array with
                 * entry for each cell for which MLB could not be
                 * enabled/disabled. Last argument is not required
                 * as the cell FSM state is already set above */
                mlb_construct_cell_arr_from_pending_res_list(
                        &res.disable_res.cell_status_list_size,
                        res.disable_res.cell_status_list,
                        MLB_CELL_STATE_UNDEFINED);
                if (0 != res.disable_res.cell_status_list_size)
                {
                    res.disable_res.result = SON_FAILURE;
                }
                else
                {
                    res.disable_res.result = SON_SUCCESS;
                }

                son_create_send_buffer((son_u8 *)&res,
                        SON_MLB_MODULE_ID,
                        SON_MIF_MODULE_ID,
                        SONMLB_DISABLE_RES,
                        disable_res_len);
                /* Set MLB global FSM state to ACTIVE */
                SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state,
                        MLB_STATE_ACTIVE);
                /* Reset MLB_Pending_Response_LIST */
                mlb_reset_pending_resp_list();
                /* Stop the MLB_PENDING_RESPONSE_TIMER if running */
                if (SON_TRUE == son_is_timer_running(
                            g_mlb_context.mlb_prending_res_timer_id))
                {
                son_stop_timer(g_mlb_context.mlb_prending_res_timer_id);
                }
            }
        }
    }
    else
    {
        /* Send SONMLB_DISABLE_RES to SONMgmtIfH */
        SON_MEMSET(&res, 0, disable_res_len);
        res.transaction_id = p_res->transaction_id;
        res.disable_res.result = result;
        res.disable_res.error_code = SON_NO_ERROR;
        if (SON_FAILURE == result)
        {
            res.disable_res.cell_status_list_size = 1;
            SON_MEMCPY(&res.disable_res.cell_status_list[0].cgi,
                    &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));
            res.disable_res.cell_status_list[0].error_code = error_code;
        }

        son_create_send_buffer((son_u8 *)&res,
                SON_MLB_MODULE_ID,
                SON_MIF_MODULE_ID, SONMLB_DISABLE_RES,
                disable_res_len);
    }

    if ((RRM_ERR_OBJECT_NON_EXISTENT ==
                p_res->res.deregistration_resp.error_code) ||
            (RRM_ERR_CELL_UNCONFIGURED == \
             p_res->res.deregistration_resp.error_code))
    {
        mlb_delete_cell_from_context(&p_cell_ctxt->cell_id);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_deregister_res_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_register_res_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for RRM_REGISTER_RES.It is invoked
 *                  when mlb state for the cell is ES_CELL_STATE_ENABLING.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_register_res_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8                *p_msg
 )
{
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
    son_return_et result = SON_FAILURE;
    son_rrm_registration_res_t    *p_res = (son_rrm_registration_res_t *)p_msg;
    son_mlb_enable_res_t res;
    son_size_t enable_res_len = sizeof(son_mlb_enable_res_t);
    son_feature_state_et state = SON_STATE_DISABLED;
    son_feature_state_change_cause_et cause = SON_RRM_REGISTRATION_UNSUCCESSFUL;
    son_error_et error_code = SON_NO_ERROR;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, enable_res_len);
    result = ((p_res->res.registration_resp.result == RRM_SUCCESS) ?
            SON_SUCCESS : SON_FAILURE);
    if (SON_FAILURE == result)
    {
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_DISABLED);
        error_code = mlb_map_rrm_error_codes(
            ( const son_s8 *)__func__,
                result, p_res->res.registration_resp.error_code,
                RRM_SON_REGISTER_RESP);
    }
    else
    {
        /* Change cell FSM state to MLB_CELL_STATE_ENABLED */
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_ENABLED);
        state = SON_STATE_ENABLED;
        cause = SON_RRM_REGISTRATION_SUCCESSFUL;
    }

    if (p_res->transaction_id == mlb_get_pending_res_transaction_id())
    {
        cell_status_list_data_t *p_list_node = SON_PNULL;
        /* Search for the cell Id in pending response structure. */
        p_list_node = mlb_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

        /* Presence indicates that cell state is enabling due to Global
         * Enable Request or Cell Info Indication and no immediate Enable
         * Response must be sent from cell FSM */
        if (SON_PNULL != p_list_node)
        {
            api = mlb_get_pending_res_api_id();

            /* Update Cell in MLB Pending Response List with error code
             * determined above */
            p_list_node->cell_status.error_code = error_code;
            /* Decrement expected_response_count by 1 */
            mlb_decrement_pending_res_expected_count();

            /* Check if expected response count == 0 */
            if (0 == mlb_get_pending_res_expected_count())
            {
                if (SONMLB_ENABLE_RES == api)
                {
                    res.transaction_id = p_res->transaction_id;
                    res.enable_res.error_code = SON_NO_ERROR;

                    /* Remove Successful cells from the
                     * MLB_Pending_Response_LIST and create cell array with
                     * entry for each cell for which MLB could not be
                     * enabled/disabled. Last argument is not required
                     * as the cell FSM state is already set above */
                    mlb_construct_cell_arr_from_pending_res_list(
                            &res.enable_res.cell_status_list_size,
                            res.enable_res.cell_status_list,
                            MLB_CELL_STATE_UNDEFINED);
                    if (0 != res.enable_res.cell_status_list_size)
                    {
                        res.enable_res.result = SON_FAILURE;
                    }
                    else
                    {
                        res.enable_res.result = SON_SUCCESS;
                    }

                    son_create_send_buffer((son_u8 *)&res,
                            SON_MLB_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONMLB_ENABLE_RES,
                            enable_res_len);
                }
                else
                {
                    /* Remove cell from the MLB_Pending_Response_LIST */
                    mlb_delete_cell_from_pending_res(&p_cell_ctxt->cell_id);
                }

                /* Set MLB global FSM state to ACTIVE */
                SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state,
                        MLB_STATE_ACTIVE);
                /* Reset MLB_Pending_Response_LIST */
                mlb_reset_pending_resp_list();
                /* Stop the MLB_PENDING_RESPONSE_TIMER if running */
                if (SON_TRUE == son_is_timer_running(
                            g_mlb_context.mlb_prending_res_timer_id))
                {
                son_stop_timer(g_mlb_context.mlb_prending_res_timer_id);
                }
            }
        }
    }
    else
    {
        /* Send SONMLB_ENABLE_RES to SONMgmtIfH */
        res.transaction_id = p_res->transaction_id;
        res.enable_res.result = result;
        res.enable_res.error_code = SON_NO_ERROR;
        if (SON_FAILURE == result)
        {
            res.enable_res.cell_status_list_size = 1;
            SON_MEMCPY(&res.enable_res.cell_status_list[0].cgi,
                    &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));
            res.enable_res.cell_status_list[0].error_code = error_code;
        }

        son_create_send_buffer((son_u8 *)&res,
                SON_MLB_MODULE_ID,
                SON_MIF_MODULE_ID, SONMLB_ENABLE_RES,
                enable_res_len);
    }

    if (SON_FAILURE == result)
    {
        if ((RRM_ERR_OBJECT_NON_EXISTENT ==
                    p_res->res.registration_resp.error_code) ||
                (RRM_ERR_CELL_UNCONFIGURED == \
                 p_res->res.registration_resp.error_code))
        {
            state = SON_STATE_DELETED;
            cause = SON_RRM_CELL_UNCONFIGURED;
            mlb_delete_cell_from_context(&p_cell_ctxt->cell_id);
        }
    }

    if (SMIF_CELL_INFO_IND == api)
    {
        /* Send feature state change indication is received
         * registration response is for the request triggered due
         * to Cell Info Indication request received by MLB */
        mlb_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
                state, cause);
    }
    /*SPR 17777 +-*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_register_res_handler */

/******************************************************************************
 * Function Name  : son_mlb_find_nbr_node_with_max_avr_rsrp
 * Inputs         : p_cell_context  -  Pointer to mlb cell context
 *                : son_mlb_neigh_cell_info_record_t - **p_p_son_mlb_neigh_cell_info_record
 * Outputs        : None
 * Returns        : son_void_t 
 * Description    : calculate nbr_max_avg_rsrp
 *****************************************************************************/
son_void_t
son_mlb_find_nbr_node_with_max_avr_rsrp
(
 mlb_cell_context_t   *p_cell_ctxt,
    son_mlb_neigh_cell_info_record_t * *p_p_son_mlb_neigh_cell_info_record
 )
{
    SON_UT_TRACE_ENTER();
    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL; 
    son_s16 count = SON_NULL;
    son_u8 temp_rsrp = SON_NULL;
    count = MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head) - 1;
    for (; count >= 0; count--)
    {
        p_son_mlb_neigh_cell_info_record  = (son_mlb_neigh_cell_info_record_t *)
                      ylNth(&p_cell_ctxt->neigh_cell_info_head, count);
        if (p_son_mlb_neigh_cell_info_record == SON_PNULL)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR, "May be mlb database got corrupted");

            SON_UT_TRACE_EXIT();
            return;
        }

        if (p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.avg_rsrp >
                temp_rsrp)
        {
            temp_rsrp = p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.avg_rsrp;
            *p_p_son_mlb_neigh_cell_info_record = p_son_mlb_neigh_cell_info_record;
        }
    }

    SON_UT_TRACE_EXIT();
    return;
} /* son_mlb_find_nbr_node_with_max_avr_rsrp */

/******************************************************************************
 * Function Name  : mlb_fsm_nbr_update_info_handler, 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for ANR_NBR_UPDATE_INFO.It is invoked
 *                  when anr delete the nbr from its NRT table .
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_nbr_update_info_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8                *p_msg
 )
{
    SON_UT_TRACE_ENTER();

    son_u16 count = SON_NULL; 
    son_u16 nbr_index = SON_NULL; 
    son_anr_updated_nrt_info_t *p_son_anr_updated_nrt_info = (son_anr_updated_nrt_info_t *)p_msg;

    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL;
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "SONANR_UPDATED_NRT_INFO received: Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_son_anr_updated_nrt_info->src_cgi.cell_identity));

    if (SON_NRT_OP_DELETE != p_son_anr_updated_nrt_info->nrt_op)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "nrt update info does not having nrt delete option, So ignoring nbr update info");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    count = p_son_anr_updated_nrt_info->nr_list.intra_rat_nr_list_size;
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "Count for neighb cell in cell context before deletion of nbr: %d"
	    "size of nbr list in anr nbr updated info ind : %d", p_cell_ctxt->count_for_neigh_cell, count);

    while (count != SON_NULL)
    {
        if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
        {
            p_son_mlb_neigh_cell_info_record = (son_mlb_neigh_cell_info_record_t *)
                                               list_find_node(&p_cell_ctxt->neigh_cell_info_head,
                (son_void_t *)&p_son_anr_updated_nrt_info->nr_list.intra_rat_nr_list[nbr_index].nbr_cgi,
                son_mlb_object_id_keyof,
                son_compare_cell_id);

            if (p_son_mlb_neigh_cell_info_record != SON_PNULL)
            {
                MLB_NBR_Q_DELETE(&p_cell_ctxt->neigh_cell_info_head, p_son_mlb_neigh_cell_info_record);
                son_mem_free(p_son_mlb_neigh_cell_info_record);
                p_son_mlb_neigh_cell_info_record = SON_PNULL;
                p_cell_ctxt->count_for_neigh_cell--;
            }
        }
        else
        {
            break;
        }

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "Deleted nbr cell id: 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_son_anr_updated_nrt_info->nr_list.intra_rat_nr_list[nbr_index].
                nbr_cgi.cell_identity));

        count--;
        nbr_index++;
    }

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
        "Count for neighb cell in cell context after deletion of neighb : %d", p_cell_ctxt->count_for_neigh_cell);

    if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
    {
        p_son_mlb_neigh_cell_info_record = SON_PNULL;
        son_mlb_find_nbr_node_with_max_avr_rsrp(p_cell_ctxt, &p_son_mlb_neigh_cell_info_record);
        if (p_son_mlb_neigh_cell_info_record == SON_PNULL)
        {
                    /*SPR 17777 +-*/
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "Avg rsrp is not present for neghb cell");

            LTE_GCC_UNUSED_PARAM(evnt_id);
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
            p_cell_ctxt->strongest_nbr_rsrp = SON_NULL;
            return RELEASE_SON_BUFFER;
        }

        p_cell_ctxt->strongest_nbr_rsrp = p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.avg_rsrp;
    }
    else
    {
        p_cell_ctxt->strongest_nbr_rsrp = SON_NULL;
    }

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "updated strongest nbr rsrp : %d", p_cell_ctxt->strongest_nbr_rsrp);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nbr_update_info_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_anr_disable_ind_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for ANR_DISABLE_IND.It is invoked
 *                  when anr state for the cell is ANR_CELL_STATE_DISABLED .
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_anr_disable_ind_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8                *p_msg
 )
{
    son_anr_cell_mode_ind_t *p_anr_mode_ind =
        (son_anr_cell_mode_ind_t *)p_msg;


    SON_UT_TRACE_ENTER();

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "SONANR_DISABLE_IND received: Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_anr_mode_ind->srv_cgi.cell_identity));

    if (ANR_MODE_DISABLED == p_anr_mode_ind->cell_mode)
    {
        mlb_send_feature_state_change_ind(&p_anr_mode_ind->srv_cgi,
                SON_STATE_DISABLED, SON_ANR_MODE_DISABLED);

        if (SON_SUCCESS != mlb_construct_send_rrm_deregister_req(
                    son_generate_txn_id(),
                    &p_anr_mode_ind->srv_cgi))
        {
                    /*SPR 17777 +-*/
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_ERROR,
                    "Failed to send rrm_deregister_req on receiving "
                    "SONANR_DISABLE_IND from ANR for Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
            LTE_GCC_UNUSED_PARAM(evnt_id);
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
            SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MLB_CELL_STATE_DISABLED);
        }
        else
        {
            p_cell_ctxt->dereg_invoking_api = SONANR_DISABLE_IND;
            SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MLB_CELL_STATE_DISABLING);
        }
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "SONANR_DISABLE_IND received with invalid cell mode[%d] for "
                "Cell 0x%x",
                p_anr_mode_ind->cell_mode,
                son_convert_char_cell_id_to_int_cell_id(p_anr_mode_ind->srv_cgi.cell_identity));
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_anr_disable_ind_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_nack_enable_req_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for SONMLB_ENABLE_REQ, when received in
 *                     unexpected state.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_nack_enable_req_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8                *p_msg
 )
{
    son_mlb_enable_res_t  enable_resp = {0};
    son_mlb_enable_req_t  *p_req = (son_mlb_enable_req_t *)p_msg;


    SON_UT_TRACE_ENTER();

    enable_resp.enable_res.cell_status_list_size = 1;
    enable_resp.enable_res.result = SON_FAILURE;
    enable_resp.enable_res.error_code = SON_NO_ERROR;
    enable_resp.enable_res.cell_status_list[0].error_code = 
        SON_ERR_ALREADY_ENABLED;

    if (MLB_CELL_STATE_ENABLING == p_cell_ctxt->current_cell_fsm_state)
    {
        enable_resp.enable_res.cell_status_list[0].error_code = 
            SON_ERR_ENABLE_IN_PROGRESS;
    }

    if (MLB_CELL_STATE_DISABLING == p_cell_ctxt->current_cell_fsm_state)
    {
        enable_resp.enable_res.cell_status_list[0].error_code = 
            SON_ERR_DISABLE_IN_PROGRESS;
    }

    enable_resp.transaction_id = p_req->transaction_id;
    SON_MEMCPY(&(enable_resp.enable_res.cell_status_list[0].cgi), 
            &p_req->enable_req.src_cgi, 
        sizeof(son_intra_rat_global_cell_id_t));

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected Msg %s received for "
            "cell %x in %s state from %d. SONMLB_ENABLE_RES "
            "Msg sent to %d with Error Code: %d ",
            SON_MLB_CELL_FSM_EVENT_NAMES[evnt_id],
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
            SON_MLB_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
            p_cspl_hdr->from, p_cspl_hdr->from,
            enable_resp.enable_res.cell_status_list[0].error_code);

    son_create_send_buffer((son_u8 *)&enable_resp, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SONMLB_ENABLE_RES,
            sizeof(son_mlb_enable_res_t));

    SON_UT_TRACE_EXIT();

    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nack_enable_req_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_disable_req_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_DISABLE_REQ.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_disable_req_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    son_mlb_disable_req_t *p_req = (son_mlb_disable_req_t *)(p_msg);


    SON_UT_TRACE_ENTER();

    if (SON_SUCCESS == mlb_construct_send_rrm_deregister_req(
                p_req->transaction_id,
                &p_req->disable_req.src_cgi))
    {
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_DISABLING);
    }
    else
    {
        /* Send failure to SONMgmtIfH */
        son_mlb_disable_res_t disable_res;
        son_size_t disable_res_len = sizeof(son_mlb_disable_res_t);

        SON_MEMSET(&disable_res, 0, disable_res_len);
        disable_res.transaction_id = p_req->transaction_id;
        disable_res.disable_res.result = SON_FAILURE;
        disable_res.disable_res.error_code = SON_NO_ERROR;
        disable_res.disable_res.cell_status_list_size = 1;
        SON_MEMCPY(&disable_res.disable_res.cell_status_list[0].cgi,
                &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
        disable_res.disable_res.cell_status_list[0].error_code =
            SON_ERR_COMPOSER_ERROR;

        son_create_send_buffer((son_u8 *)&disable_res,
                SON_MLB_MODULE_ID, SON_MIF_MODULE_ID, SONMLB_DISABLE_RES,
                disable_res_len);
    }
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(evnt_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_disable_req_handler */

/******************************************************************************
 * Function Name  : son_mlb_loacl_max_min_offset_set 
 * Inputs         : p_cell_context -  Pointer to mlb cell context
 * Outputs        : None
 * Returns        : None
 * Description    : For setting local context of Qoffset.
 *****************************************************************************/
son_void_t
son_mlb_local_max_min_offset_set
(
 mlb_cell_context_t   *p_cell_ctxt
 )
{
    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL;
    son_s16 count = SON_NULL;


    count = MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head) - 1;
    for (; count >= 0; count--)
    {
        p_son_mlb_neigh_cell_info_record  = (son_mlb_neigh_cell_info_record_t *)
                                            ylNth(&p_cell_ctxt->neigh_cell_info_head, count);
        if (p_son_mlb_neigh_cell_info_record == SON_PNULL)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR, "May be mlb database got corrupted");
            return;
        }

        if (p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.initial_cell_offset >
                p_cell_ctxt->eutra_q_offset_config.q_offset_max)
        {
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.initial_cell_offset = 
                p_cell_ctxt->eutra_q_offset_config.q_offset_max;
        }
        else if (p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.initial_cell_offset <
                p_cell_ctxt->eutra_q_offset_config.q_offset_min)
        {
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.initial_cell_offset = 
                p_cell_ctxt->eutra_q_offset_config.q_offset_min;
        }

        p_son_mlb_neigh_cell_info_record = SON_PNULL;
    }

    return;
} /* son_mlb_local_max_min_offset_set */

/******************************************************************************
 * Function Name  : mlb_fsm_modify_req_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_MODIFY_REQ.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_modify_req_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    son_mlb_modify_req_t *p_req = (son_mlb_modify_req_t *)p_msg;
    son_mlb_modify_res_t modify_res = {0};
    son_bool_et send_modify_resp = SON_FALSE;


    SON_UT_TRACE_ENTER();

    SON_MEMCPY(&(modify_res.modify_attributes_res.srv_cgi),
            &p_cell_ctxt->cell_id,
        sizeof(son_intra_rat_global_cell_id_t));
    modify_res.transaction_id = p_req->transaction_id;

    /*SONMLB-- Centralized support start*/
    if (p_req->modify_attributes_req.bitmask & SON_OAM_MLB_MODE_PRESENT)
    {
        modify_res.modify_attributes_res.result = SON_SUCCESS;
        modify_res.modify_attributes_res.error_code = SON_NO_ERROR;
        send_modify_resp = SON_TRUE;
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "Sending mlb_fsm_modify_resp for cell"
            "Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_req->modify_attributes_req.srv_cgi.cell_identity));
    }

    /*SONMLB-- Centralized support end*/

    if (p_req->modify_attributes_req.bitmask & SON_OAM_MLB_EUTRA_Q_OFFSET_CONFIG_PRESENT)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_DETAILED,
            "Requested Structure:Offset min : %d Offset max: %d ",
            p_req->modify_attributes_req.eutra_q_offset_config.q_offset_min,
            p_req->modify_attributes_req.eutra_q_offset_config.q_offset_max);

        if (((p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MAX_PRESENT) /* max is present in req */
             && (p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MIN_PRESENT) /*min is present in req */
             && (p_req->modify_attributes_req.eutra_q_offset_config.q_offset_min >
                 p_req->modify_attributes_req.eutra_q_offset_config.q_offset_max)) /* and the min value in req > max value in req*/
            ||
            (!(p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MAX_PRESENT) /* max is absent in req */
             && (p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MIN_PRESENT) /* min is present in req */
             && (p_req->modify_attributes_req.eutra_q_offset_config.q_offset_min >
                 p_cell_ctxt->eutra_q_offset_config.q_offset_max)) /*and the min value in req > max value in ctxt */
            ||
            ((p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MAX_PRESENT) /* max is present in req */
             && !(p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MIN_PRESENT) /* min is absent in req */
             && (p_req->modify_attributes_req.eutra_q_offset_config.q_offset_max <
                 p_cell_ctxt->eutra_q_offset_config.q_offset_min)) /* and the max value in req < min value in ctxt */
            )
        {
            modify_res.modify_attributes_res.result = SON_FAILURE;
            modify_res.modify_attributes_res.error_code = SON_ERR_INVALID_PARAMS;
            son_create_send_buffer((son_u8 *)&modify_res, SON_MLB_MODULE_ID,
                    (son_module_id_et)p_cspl_hdr->from, SONMLB_MODIFY_ATTRIBUTES_RES, /*coverity_CID_26320 start-end*/
                    sizeof(modify_res));
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "Invalid modify request, preserving old values"
                    "Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_req->modify_attributes_req.srv_cgi.cell_identity));

            return RELEASE_SON_BUFFER;
        }

        if (p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MIN_PRESENT)
        {
            p_cell_ctxt->eutra_q_offset_config.q_offset_min =
                p_req->modify_attributes_req.eutra_q_offset_config.q_offset_min;
        }
        else
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "Min attributes has not been configured for"
                    "Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_req->modify_attributes_req.srv_cgi.cell_identity));
        }

        if (p_req->modify_attributes_req.eutra_q_offset_config.bitmask &  SON_OAM_MLB_Q_OFF_MAX_PRESENT)
        {
            p_cell_ctxt->eutra_q_offset_config.q_offset_max =
                p_req->modify_attributes_req.eutra_q_offset_config.q_offset_max;
        }
        else
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "Max attributes has not been configured for"
                    "Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_req->modify_attributes_req.srv_cgi.cell_identity));
        }

        son_mlb_local_max_min_offset_set(p_cell_ctxt);
        modify_res.modify_attributes_res.result = SON_SUCCESS;
        modify_res.modify_attributes_res.error_code = SON_NO_ERROR;
        send_modify_resp = SON_TRUE;
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "Sending mlb_fsm_modify_resp for cell"
            "Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_req->modify_attributes_req.srv_cgi.cell_identity));
    }
    else
    {
        if (send_modify_resp == SON_FALSE)
        {
            send_modify_resp = SON_TRUE;
            modify_res.modify_attributes_res.result = SON_FAILURE;
            modify_res.modify_attributes_res.error_code = SON_ERR_INVALID_PARAMS;
        }

                    /*SPR 17777 +-*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_INFO,
            "Attributes has not been configured for"
            "Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_req->modify_attributes_req.srv_cgi.cell_identity));
        LTE_GCC_UNUSED_PARAM(evnt_id);
                    /*SPR 17777 +-*/
    }

    if (send_modify_resp == SON_TRUE)
    {
        son_create_send_buffer((son_u8 *)&modify_res, SON_MLB_MODULE_ID,
                (son_module_id_et)p_cspl_hdr->from, SONMLB_MODIFY_ATTRIBUTES_RES,
                sizeof(modify_res));
    }

    SON_UT_TRACE_EXIT();

    return RELEASE_SON_BUFFER;
} /* mlb_fsm_modify_req_handler */

/******************************************************************************
 * Function Name  : mlb_fsm_state_change_handler
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler to handle RRM_CELL_STATE_CHANGE_IND message.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_state_change_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t  *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    son_cell_state_change_indication_t *p_ind =
        (son_cell_state_change_indication_t *)p_msg;
    son_feature_state_et                state = SON_STATE_UNDEFINED;
    son_feature_state_change_cause_et   cause = 
        SON_STATE_CHANGE_CAUSE_UNDEFINED;


    SON_UT_TRACE_ENTER();

    if (RRM_CELL_DELETED ==  p_ind->cell_state)
    {
        state = SON_STATE_DELETED;
        cause = SON_RRM_CELL_UNCONFIGURED;
        if (SON_TRUE == mlb_delete_cell_from_context
                ((son_intra_rat_global_cell_id_t *)&p_ind->cell_id))
        {
                    /*SPR 17777 +-*/
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_INFO,
                    "Cell context deleted for Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_ind->cell_id.cell_identity));
            LTE_GCC_UNUSED_PARAM(evnt_id);
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
        }
    }
    else if (RRM_CELL_UNOPERATIONAL ==  p_ind->cell_state)
    {
        /* PR 551 Fix Start */
        state = SON_STATE_ENABLED;
        /* PR 551 Fix End */
        cause = SON_RRM_CELL_UNOPERATIONAL;
        /* PR 551 Fix Start */
        SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MLB_CELL_STATE_ENABLED);
        /* PR 551 Fix End */
    }
    /* PR 551 Fix Start */
    else if (RRM_CELL_OPERATIONAL == p_ind->cell_state)
    {
        state = SON_STATE_ENABLED;
        cause = SON_RRM_CELL_OPERATIONAL;
    }
    /* PR 551 Fix End */
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Invalid Cell state[%d] received from RRM",
                p_ind->cell_state);
    }

    if (SON_STATE_UNDEFINED != state)
    {
        mlb_send_feature_state_change_ind(
                (son_intra_rat_global_cell_id_t *)&p_ind->cell_id,
                state, cause);
        if (mlb_get_pending_res_expected_count())
        {
            mlb_delete_cell_from_pending_res
                ((son_intra_rat_global_cell_id_t *)&p_ind->cell_id);

            if (0 == mlb_get_pending_res_expected_count())
            {
                son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
                api = mlb_get_pending_res_api_id();
                mlb_set_context_state(MLB_STATE_ACTIVE);
                if (SONMLB_ENABLE_RES == api)
                {
                    son_mlb_enable_res_t res;
                    son_size_t enable_res_len = sizeof(son_mlb_enable_res_t);
                    SON_MEMSET(&res, 0, enable_res_len);
                    res.transaction_id = mlb_get_pending_res_transaction_id();
                    res.enable_res.error_code = SON_NO_ERROR;

                    /* Remove Successful cells from the
                     * MLB_Pending_Response_LIST and create cell array with
                     * entry for each cell for which MLB could not be
                     * enabled. Last argument is not required
                     * as the cell FSM state is already set above */
                    mlb_construct_cell_arr_from_pending_res_list(
                            &res.enable_res.cell_status_list_size,
                            res.enable_res.cell_status_list,
                            MLB_CELL_STATE_UNDEFINED);
                    if (0 != res.enable_res.cell_status_list_size)
                    {
                        res.enable_res.result = SON_FAILURE;
                    }
                    else
                    {
                        res.enable_res.result = SON_SUCCESS;
                    }

                    son_create_send_buffer((son_u8 *)&res,
                            SON_MLB_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONMLB_ENABLE_RES,
                            enable_res_len);
                }
                else if (SONMLB_DISABLE_RES == api)
                {
                    son_mlb_disable_res_t res;
                    son_size_t disable_res_len = sizeof(son_mlb_disable_res_t);
                    SON_MEMSET(&res, 0, disable_res_len);
                    res.transaction_id = mlb_get_pending_res_transaction_id();
                    res.disable_res.error_code = SON_NO_ERROR;

                    /* Remove Successful cells from the
                     * MLB_Pending_Response_LIST and create cell array with
                     * entry for each cell for which MLB could not be
                     * disabled. Last argument is not required
                     * as the cell FSM state is already set above */
                    mlb_construct_cell_arr_from_pending_res_list(
                            &res.disable_res.cell_status_list_size,
                            res.disable_res.cell_status_list,
                            MLB_CELL_STATE_UNDEFINED);   /*coverity_CID_25755 fix start-end*/
                    if (0 != res.disable_res.cell_status_list_size)
                    {
                        res.disable_res.result = SON_FAILURE;
                    }
                    else
                    {
                        res.disable_res.result = SON_SUCCESS;
                    }

                    son_create_send_buffer((son_u8 *)&res,
                            SON_MLB_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONMLB_DISABLE_RES,
                            disable_res_len);
                }

                mlb_reset_pending_res();
            }
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_state_change_handler */

/******************************************************************************
 * Function Name  : son_mlb_get_avg_rsrp 
 * Inputs         : p_cell_context -  Pointer to MLB cell context
 * Outputs        : None
 * Returns        : Average of rsrp
 * Description    : Calculate the average of rsrp of neigh cell.
 *****************************************************************************/
son_u8
son_mlb_get_avg_rsrp
(
    son_mlb_neigh_cell_info_t *p_cell_ctxt
)
{
    SON_UT_TRACE_ENTER();
    son_u32 count = 0;
    son_u32 total = 0;
    son_u8 rsrp_avg = 0;
    while (count < p_cell_ctxt->ho_count)
    {
        total += p_cell_ctxt->rsrp[count];
        count++;
    }

    /* SPR_20915 Fix - Starts */
    if(0 != count)
    {
        rsrp_avg = total / count;
    }
    /* SPR_20915 Fix - Ends */
    SON_UT_TRACE_EXIT();
    return rsrp_avg;
}

/** eICIC changes start */
/******************************************************************************
 * Function Name  : son_mlb_offset_change_required
 * Inputs         : min_abs_usage
 *                : max_abs_usage
 *                : abs_usage
 * Outputs        : SON_TRUE/SON_FALSE
 * Returns        : son_bool_et
 * Description    : This function shall decides if min_abs_usage_threshold is
 *                  is greater than abs_usage received from RRM then return TRUE.
 *                  else max_abs_usage_threshold is less than abs_usage and 
 *                  greater than min_abs_usage_threshold then return FALSE.
 *                  else max_abs_usage_threshold is less than abs_usage then
 *                  return FALSE.
 *****************************************************************************/
son_bool_et
son_mlb_offset_change_required
(
    son_u8    min_abs_usage_threshold,
    son_u8    max_abs_usage_threshold,
    son_u8    abs_usage
 )
{
    son_bool_et     ret_val = SON_FALSE;
    SON_UT_TRACE_ENTER();
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
       	    SON_INFO,
	    "min_abs_usage_threshold %d "
	    "max_abs_usage_threshold %d "
	    "abs_usage [ %d ]",
	     min_abs_usage_threshold,
	     max_abs_usage_threshold,
	     abs_usage);
    /*+ eICIC review commnents incorporated +*/
    /* SPR 15178 Fix start */
    if(max_abs_usage_threshold >= abs_usage)
    {
        if((max_abs_usage_threshold - min_abs_usage_threshold) < (max_abs_usage_threshold - abs_usage))
        {
            ret_val = SON_FALSE;
        }
        else
        {
            ret_val = SON_TRUE;
        }
    }
    /* SPR 15178 Fix end */
    /*+ eICIC review commnents incorporated +*/
    SON_UT_TRACE_EXIT();
    return ret_val;
}
/** eICIC changes end */

/******************************************************************************
 * Function Name  : son_mlb_calc_of_delta_and_newoffset_idle_mod 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : trgt_cgi      -    Globle cell id for target cell
 *                : p_neigh_cell_info - Pointer to neighbor cell info
 * Outputs        : None
 * Returns        : son_void_t 
 * Description    : It will calculate the delta and new offset for Idle Mode.
 *****************************************************************************/
son_void_t
son_mlb_calc_of_delta_and_newoffset_idle_mod
(
    mlb_cell_context_t *p_cell_ctxt,
    rrm_oam_global_cell_id_t trgt_cgi,
    son_mlb_neigh_cell_info_t  *p_neigh_cell_info
)
{
    SON_UT_TRACE_ENTER();
    son_u8 nbr_load, serving_load;
    son_float    diff_load;
    son_u8 rsrp = 0;
    son_global_cell_id_t glbl_cell_id;
    /*SPR 20523 START*/
    son_s8 mapped_q_offset_max; 
    son_s8 mapped_q_offset_min;
    son_s8 mapped_initial_idle_cell_offset; 
    /* CID 118136 Fix Start */
    son_q_offset_range_et new_cell_offset_enum = SON_Q_OFFSET_RANGE_DB0;
    /* CID 118136 Fix End */
    son_s8 mapped_new_cell_offset = SON_NULL; 
    /*SPR 20523 END*/
    /*coverty_CID_25764 fix start*/
    SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
    /*coverty_CID_25764 fix end*/

    /*extract neighbor cell info*/
    son_mlb_neigh_cell_info_t *p_trgt_cell_ctxt1 =   p_neigh_cell_info;

    /*calcualte avg RSRP of neighbor*/
    rsrp = son_mlb_get_avg_rsrp(p_trgt_cell_ctxt1);

    /*reset handover count*/
    p_neigh_cell_info->ho_count = 0;

    /*SPR 18750 Fix Start*/
    if(!(p_neigh_cell_info->neigh_cell_load_info.bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT) ||
            !(p_cell_ctxt->serv_cell_load_info.bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT))
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "nbr/Serv load is not yet received."
                "Not calling the idle mod algo");
        return;
    }
    /*SPR 18750 Fix Stop*/

    nbr_load = p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val; 
    serving_load = p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val;

    /* SPR 19015 Fix Start */
    if((p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask &
                RRM_SON_CELL_CAP_CLASS_VAL_PRESENT) &&
            (p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask &
             RRM_SON_CELL_CAP_CLASS_VAL_PRESENT))
    {
        nbr_load = ((nbr_load *
                    p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val)/
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val);

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "RRM_SON_CELL_CAP_CLASS_VAL is present & calculated nbr_load [%d] serving load [%d]"
                "neighbour cell cap class [%d]"
                "serving cell cap class [%d]",
                nbr_load,serving_load,p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val,
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val);
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "RRM_SON_CELL_CAP_CLASS_VAL is present & calculated nbr_load :%d  serving load is:%d",nbr_load,serving_load);
    }
    /* SPR 19015 Fix Stop */

    /*calculate new offset for neighbor cell.threshold load
     * value we read as in percentage from CFG file son_oam.cfg
     * exp : if threshold value is e0 in cfg file the actual value should be 10/100 = 0.1*/
    if (serving_load > nbr_load)
    {
        diff_load = (serving_load - nbr_load);
    }
    else if (nbr_load > serving_load)
    {
        diff_load = (nbr_load - serving_load);
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "serving and nbr load are same.Not calling the idle mod algo ");
        return;
    }

    if (rsrp > p_cell_ctxt->eutra_threshold_rsrp)
    {
        /*SPR 20523 START*/
        mapped_q_offset_max = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_max);
        mapped_q_offset_min = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_min);
        mapped_initial_idle_cell_offset = son_map_q_offset_value(p_neigh_cell_info->initial_cell_offset);

        /* ∆ = (|Pi –Ps| - Pth,idle) / (1- Pth,idle ) * (Qoffset,max – Qoffset,min,step_size) + Qoffset,min,step_size.
         * here we read load in percentage from cfg file.so need to divide load by 100*/
        p_cell_ctxt->delta_offset_idle =
            (son_u32)(((diff_load/100.0 - p_cell_ctxt->eutra_threshold_idle /
                            100.0) / (1 - p_cell_ctxt->eutra_threshold_idle / 100.0)) *
                    (mapped_q_offset_max -
                     g_mlb_context.mlb_config_param.eutra_offset_min_step)
                    + g_mlb_context.mlb_config_param.eutra_offset_min_step);


        /* SPR 20447 Fix Start */
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,"son_mlb_calc_of_delta_and_newoffset_idle_mod"
                "delta_offset_idle [%d] diff_load [%f] eutra_threshold_idle[%u]"
                "q_offset_max[%d] eutra_ocn_min_step [%d],rsrp [%d] \n",
                p_cell_ctxt->delta_offset_idle,diff_load,p_cell_ctxt->eutra_threshold_idle,
                p_cell_ctxt->eutra_q_offset_config.q_offset_max,
                g_mlb_context.mlb_config_param.eutra_ocn_min_step,rsrp);
        /* SPR 20447 Fix Stop */

        /*if serving and neighbor load difference is greater than threhhold load that means the 
         * serving has more available capacity and the handover offset OCN shall be ncreased so that UEs can move to neighbor*/
        if ((serving_load > nbr_load) && ((diff_load) > p_cell_ctxt->eutra_threshold_idle))
        {
            /** eICIC changes start */
            /*+ eICIC review commnents incorporated +*/
            if (SON_ACTIVATE_EICIC == p_neigh_cell_info->eicic_scheme) 
            {
                /** Case a) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as ACT_AS_VICTIM
                 *           then SON shall consider the ABS usage threshold received in SON_OAM_CELL_INFO_IND */
                if (p_cell_ctxt->provision_type == SON_ACT_AS_VICTIM)
                {
                    /* bug_13866_start */
                    if (son_mlb_offset_change_required(p_cell_ctxt->min_abs_usage_threshold_victim,
                                p_cell_ctxt->max_abs_usage_threshold_victim,
                                p_cell_ctxt->serv_cell_load_info.abs_status.dl_abs_status)
                            == SON_TRUE)
                    /* bug_13866_end */
                    {
                        mapped_new_cell_offset = 
                            mapped_initial_idle_cell_offset - p_cell_ctxt->delta_offset_idle;
                    }
                    else
                    {
                        mapped_new_cell_offset =
                            mapped_initial_idle_cell_offset;
                    }
                }
                /** Case b) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as ACT_AS_AGGRESSOR
                 *          then SON shall consider the ABS usage threshold received in SON_OAM_CELL_INFO_IND */
                else if (p_cell_ctxt->provision_type == SON_ACT_AS_AGGRESSOR)
                {
                    /* bug_13866_start */
                    if (son_mlb_offset_change_required(p_neigh_cell_info->min_abs_usage_threshold_aggressor,
                                p_neigh_cell_info->max_abs_usage_threshold_aggressor,
                                p_neigh_cell_info->neigh_cell_load_info.abs_status.dl_abs_status)
                            == SON_TRUE)
                    /* bug_13866_end */
                    {
                        mapped_new_cell_offset =
                            mapped_initial_idle_cell_offset - p_cell_ctxt->delta_offset_idle;
                    }
                    else
                    {
                        mapped_new_cell_offset = 
                            mapped_initial_idle_cell_offset;
                    }
                }
                /** Case c) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as 
                 *          ACT_AS_AGGRESSOR_AND_VICTIM then SON shall consider the ABS usage threshold
                 *          received in SON_OAM_CELL_INFO_IND */
                else if (p_cell_ctxt->provision_type == SON_ACT_AS_AGGRESSOR_AND_VICTIM)
                {
                    /* bug_13866_start */
                    if (((SON_TRUE == son_mlb_offset_change_required(p_neigh_cell_info->min_abs_usage_threshold_aggressor,
                                        p_neigh_cell_info->max_abs_usage_threshold_aggressor,
                                        p_neigh_cell_info->neigh_cell_load_info.abs_status.dl_abs_status))) && 
                            ((SON_TRUE == son_mlb_offset_change_required(p_cell_ctxt->min_abs_usage_threshold_victim,
                                                                         p_cell_ctxt->max_abs_usage_threshold_victim,
                                                                         p_cell_ctxt->serv_cell_load_info.abs_status.dl_abs_status))))
                   /* bug_13866_end */
                    {
                        mapped_new_cell_offset =
                            mapped_initial_idle_cell_offset - p_cell_ctxt->delta_offset_idle;
                    }
                    else
                    {
                        mapped_new_cell_offset = 
                            mapped_initial_idle_cell_offset;
                    }
                }
                /** Case d) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as 
                 *          NONE then SON shall consider the ABS usage threshold
                 *          received in SON_OAM_CELL_INFO_IND */
                /* SPR 20447 Fix Start */
                else if (p_cell_ctxt->provision_type == SON_EICIC_DEACTIVATED)
                {
                    mapped_new_cell_offset =
                        mapped_initial_idle_cell_offset - p_cell_ctxt->delta_offset_idle;
                }
                /* SPR 20447 Fix Stop */
            }
            /* SPR 20447 Fix Start */
            else
            {
                mapped_new_cell_offset =
                    mapped_initial_idle_cell_offset + p_cell_ctxt->delta_offset_idle;
            }
            /* SPR 20447 Fix Stop */
        }
        /*if serving and neighbor load difference is less than threhhold load that means the neighbor 
         * has more available capacity and the handover offset OCN shall be decreased so that UEs can move to neighbor*/
        else if ((nbr_load > serving_load) && 
                ((diff_load) > p_cell_ctxt->eutra_threshold_idle) &&
                ((SON_NO_INTF_SCHEME == p_neigh_cell_info->eicic_scheme) ||
                 (SON_ACTIVATE_ICIC == p_neigh_cell_info->eicic_scheme))) 
        {
            mapped_new_cell_offset = 
                mapped_initial_idle_cell_offset + p_cell_ctxt->delta_offset_idle;
        }
        else
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "son_mlb_calc_of_delta_and_newoffset_idle_mod:"
                    "serving and nbr load difference is less than threshold.Not calling the idle mod algo \n");
            return;
        }

        /*SPR 15742 Fix Start*/
        /*Lines deleted*/
        /*SPR 15742 Fix End*/

        /* SPR 20447 Fix Start */
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "new_cell_offset_idle[%d] q_offset_max[%d] q_offset_min[%d] initial_cell_offset [%d]\n",
                p_neigh_cell_info->new_cell_offset_idle,
                p_cell_ctxt->eutra_q_offset_config.q_offset_max,
                p_cell_ctxt->eutra_q_offset_config.q_offset_min,
                p_neigh_cell_info->initial_cell_offset);
        /* SPR 20447 Fix Stop */

        if (mapped_new_cell_offset > mapped_q_offset_max)
        {
            /*take the new offset in initial offset as max offset possible*/
            mapped_new_cell_offset = mapped_q_offset_max;
        }

        /*SPR 15742 Fix Start*/

        if (mapped_new_cell_offset < mapped_q_offset_min)
        {
            /*take the new offset in initial offset as min offset possible*/
            mapped_new_cell_offset = mapped_q_offset_min;
        }
        /*SPR 15742 Fix End*/

        if (mapped_new_cell_offset  >= mapped_q_offset_min &&
                mapped_new_cell_offset <= mapped_q_offset_max &&
                mapped_new_cell_offset != mapped_initial_idle_cell_offset)
        {
            convert_rrm_gl_cell_id_to_son_gl_cell_id(&glbl_cell_id, (rrm_oam_global_cell_id_t *)&trgt_cgi);

            new_cell_offset_enum = son_reverse_map_db_to_enum(mapped_new_cell_offset,mapped_initial_idle_cell_offset);
            p_neigh_cell_info->new_cell_offset_idle = (son_s8)new_cell_offset_enum;

            /*update anr table cell context*/
            if (SON_SUCCESS == son_mlb_update_anr_table(p_cell_ctxt->cell_id, glbl_cell_id.intra_rat_global_cell_id,
                        new_cell_offset_enum, SON_FALSE,
                        p_neigh_cell_info->nbr_csg_id_present)) /*coverity_CID_26319 fix start-end*/
            {
                p_neigh_cell_info->initial_cell_offset = new_cell_offset_enum; /*coverity_CID_26322 fix start-end*/
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_BRIEF,
                        "son_mlb_calc_of_delta_and_newoffset_idle_mod :ANR table updated successfully \n");
            }
            /*SPR 20523 END*/
            else
            {
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_WARNING,
                        "Failed to update ANR table for Cell 0x%x",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
            }
        }
    }

    SON_UT_TRACE_EXIT();
} /* son_mlb_calc_of_delta_and_newoffset_idle_mod */

/******************************************************************************
 * Function Name  : son_mlb_update_anr_table 
 * Inputs         : serv_cell_id  -      Serving cell id
 *                : trgt_cell_id  -      Target cell id
 *                : cell_offset   -      Neighbor cell new calculated offset
 *                : ocn_present   -      Update is for OCN or Qoffset
 *                : nbr_csg_id_present - Neigbor cell CSG ID present flag
 * Outputs        : None
 * Returns        : None 
 * Description    : It will update the ANR table with new offset.
 *****************************************************************************/
son_return_et
son_mlb_update_anr_table
(
    son_intra_rat_global_cell_id_t serv_cell_id,
    son_intra_rat_global_cell_id_t trgt_cell_id,
    son_q_offset_range_et cell_offset,
    son_u8 ocn_present,
    son_u8 nbr_csg_id_present
)
{
    SON_UT_TRACE_ENTER();
    anr_cell_context_t                 *p_anr_cell_ctxt = SON_PNULL;
    son_error_et                       error_code = SON_NO_ERROR;
    /* Coverity ID 72625 Starts */
    son_return_et                       retval = SON_SUCCESS;
    /* Coverity ID 72625 Ends */
    son_anr_updated_nrt_info_t *p_oam_updated_nrt_info = SON_PNULL;
    son_size_t oam_updated_nrt_info_len = sizeof(son_anr_updated_nrt_info_t);
    son_anr_rrm_updated_nrt_info_t *p_rrm_updated_nrt_info = SON_PNULL;
    son_size_t rrm_updated_nrt_info_len = sizeof(son_anr_rrm_updated_nrt_info_t);
    anr_intra_rat_hash_rec_t intra_rat_hash_rec;


    p_oam_updated_nrt_info = (son_anr_updated_nrt_info_t *)son_mem_get(
            oam_updated_nrt_info_len);
    if (SON_PNULL == p_oam_updated_nrt_info)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Memory allocation failed for p_oam_updated_nrt_info");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;  /*coverity_CID_26323 start-end*/
    }

    p_rrm_updated_nrt_info = (son_anr_rrm_updated_nrt_info_t *)son_mem_get(
            rrm_updated_nrt_info_len);
    if (SON_PNULL == p_rrm_updated_nrt_info)
    {
                    /*SPR 17777 +-*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Memory allocation failed for p_rrm_updated_nrt_info");
        LTE_GCC_UNUSED_PARAM(nbr_csg_id_present);
                    /*SPR 17777 +-*/
        son_mem_free(p_oam_updated_nrt_info);
        SON_UT_TRACE_EXIT();
        return SON_FAILURE; /*coverity_CID_25760 start-end*/
    }

    SON_MEMSET(p_oam_updated_nrt_info, 0, oam_updated_nrt_info_len);
    SON_MEMSET(p_rrm_updated_nrt_info, 0, rrm_updated_nrt_info_len);
    SON_MEMSET(&intra_rat_hash_rec, 0, sizeof(anr_intra_rat_hash_rec_t));

    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(&serv_cell_id);

    /*if ocn_present flag is false that means the update is for Qoffset(ranking offset
     * used in cell reselction)*/
    if (p_anr_cell_ctxt != SON_NULL && SON_FALSE == ocn_present)
    {
        intra_rat_hash_rec.neighbor_info.nbr_cgi = trgt_cell_id;
        intra_rat_hash_rec.neighbor_info.cell_specific_offset =
            cell_offset;

        intra_rat_hash_rec.neighbor_info.bitmask |= 
            SON_CELL_OFFSET_PRESENT;
        /*Deleted the CSG ID bitmask settingSPR:9035:*/

        anr_intra_rat_modify_nr(p_anr_cell_ctxt, 
                &intra_rat_hash_rec,
                SON_FALSE, &error_code, SON_PNULL, 
                p_oam_updated_nrt_info, 
                p_rrm_updated_nrt_info, SON_TRUE, SON_FALSE);

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "ANR table updated with Qoffset %u for Cell 0x%x", cell_offset,
                son_convert_char_cell_id_to_int_cell_id(trgt_cell_id.cell_identity));
    }

    /*if ocn_present flag is true that means the update is for OCN(
     * handover offset used in event A3 based handover)*/
    if (p_anr_cell_ctxt != SON_NULL && SON_TRUE == ocn_present)
    {
        intra_rat_hash_rec.neighbor_info.nbr_cgi = trgt_cell_id;
        intra_rat_hash_rec.neighbor_info.cm_cell_specific_offset =
            cell_offset;

        intra_rat_hash_rec.neighbor_info.bitmask |= 
            SON_CM_CELL_OFFSET_PRESENT;
        /*Deleted the CSG ID bitmask settingSPR:9035:*/

        anr_intra_rat_modify_nr(p_anr_cell_ctxt, 
                &intra_rat_hash_rec,
                SON_FALSE, &error_code, SON_PNULL, 
                p_oam_updated_nrt_info, 
                p_rrm_updated_nrt_info, SON_TRUE, SON_FALSE);

        /*print the ANR Table after updating the offset value */
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "ANR table updated with OCN %u for Cell 0x%x", cell_offset,
                son_convert_char_cell_id_to_int_cell_id(trgt_cell_id.cell_identity));
    }

    if (0 != p_oam_updated_nrt_info->nr_list.intra_rat_nr_list_size)
    {
        /* Send information of updated CIO to OAM */
        SON_MEMCPY(&p_oam_updated_nrt_info->src_cgi,
                &serv_cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
        p_oam_updated_nrt_info->nrt_op = SON_NRT_OP_UPDATE;
        anr_send_updated_ncl_to_oam(p_oam_updated_nrt_info);

        /* Send information of updated CIO to RRM */
        SON_MEMCPY(&p_rrm_updated_nrt_info->src_cgi,
                &serv_cell_id,
                sizeof(son_rrm_intra_rat_global_cell_id_t));
        anr_send_updated_ncl_to_rrm(p_rrm_updated_nrt_info,
                son_convert_char_cell_id_to_int_cell_id((p_rrm_updated_nrt_info->src_cgi.
                        cell_identity)));
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_WARNING,
                "intra_rat_nr_list_size is zero");
        retval = SON_FAILURE;
    }
    /* Coverity ID 72625 Starts */
    son_mem_free(p_oam_updated_nrt_info);
    son_mem_free(p_rrm_updated_nrt_info);
    p_oam_updated_nrt_info = SON_PNULL;
    p_rrm_updated_nrt_info = SON_PNULL;
    /* SPR 15935 Fix Stop */
    SON_UT_TRACE_EXIT();
    return retval;    
} /* son_mlb_update_anr_table */

/******************************************************************************
 * Function Name  : son_mlb_calc_of_delta_and_newoffset_nbr_macro 
* Inputs         : p_cell_context  - Pointer to mlb cell context
*                : ho_report     -   Pointer to the rrm_ho_report
*                : p_neigh_cell_info - Pointer to neighbor cell info
 * Outputs        : None
 * Returns        : son_void_t 
 * Description    : It will calculate the delta and new offset for femto priority scenario.
******************************************************************************/
son_void_t
son_mlb_calc_of_delta_and_newoffset_nbr_macro
(
    mlb_cell_context_t *p_cell_ctxt,
    rrm_son_ho_report_t *ho_report,
    son_mlb_neigh_cell_info_t  *p_neigh_cell_info
)
{
    SON_UT_TRACE_ENTER();
    son_u8                             trgt_avg_rsrp1 = 0;
    son_u32                            new_calc_offset = 0; /*coverity_CID_24956 fix start-end*/
    son_global_cell_id_t        glbl_cell_id;
   /*coverty_CID_25765 fix start*/
    SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
   /*coverty_CID_25765 fix end*/

    /*print the ANR Table before updating the offset value */
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "ANR table before update"
            "Current offset value is : %d",
            p_neigh_cell_info->initial_cell_offset);

    son_mlb_neigh_cell_info_t *p_trgt_cell_ctxt1 =  p_neigh_cell_info;

    /*calculate avg RSRP*/
    trgt_avg_rsrp1 = son_mlb_get_avg_rsrp(p_trgt_cell_ctxt1);
    /*reset HO count*/
    p_neigh_cell_info->ho_count = 0;

    if (trgt_avg_rsrp1 > p_cell_ctxt->eutra_threshold_rsrp &&
            trgt_avg_rsrp1 > p_cell_ctxt->strongest_nbr_rsrp)
    {
        /*calculate new offset for neighbor cell.small offset
         * value we read as in percentage from CFG file son_oam.cfg
         * exp : if small offset value is 10 in cfg file the actual value should be 10/100 = 0.1*/
        new_calc_offset =
            (son_u32)(((p_cell_ctxt->small_step_cell_offset *
                        trgt_avg_rsrp1) / 100)  + p_neigh_cell_info->initial_cell_offset); /*coverity_CID_25759 fix start-end*/
        p_cell_ctxt->strongest_nbr_rsrp = trgt_avg_rsrp1;
        p_neigh_cell_info->avg_rsrp = trgt_avg_rsrp1;
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "Offset tuning algo will not be triggered for femto priority scneario"
               );
        return;
    }

    if (new_calc_offset  > p_cell_ctxt->eutra_q_offset_config.q_offset_max)
    {
        new_calc_offset = p_cell_ctxt->eutra_q_offset_config.q_offset_max;
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "Newly calculated offset value is"
                " greater than the configured maximum offset value, hence has been reset to maximum offset value."
                "Newly calculated offset value : %d",
                new_calc_offset);
    }

    /* update the neighbor cell info in cell context */
    if (new_calc_offset  >= p_cell_ctxt->eutra_q_offset_config.q_offset_min  &&
            new_calc_offset != p_neigh_cell_info->initial_cell_offset)
    {
        p_neigh_cell_info->new_cell_offset_idle = (son_u8)new_calc_offset;
        convert_rrm_gl_cell_id_to_son_gl_cell_id(&glbl_cell_id, (rrm_oam_global_cell_id_t *)&ho_report->trgt_cgi);
        /*update anr table cell context*/
        if (SON_SUCCESS == son_mlb_update_anr_table(p_cell_ctxt->cell_id,
                glbl_cell_id.intra_rat_global_cell_id, (son_q_offset_range_et)new_calc_offset, SON_FALSE,
                                        p_neigh_cell_info->nbr_csg_id_present))
        {
            /*take the new offset in initial offset*/
            p_neigh_cell_info->initial_cell_offset = (son_q_offset_range_et)new_calc_offset;

            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "ANR has been updated"
                    "New offset value : %d",
                    p_neigh_cell_info->initial_cell_offset);
        }
        else
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_ERROR,
                    "Error in updating ANR table");
        }
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "Newly calculated offset=%d is either less "
                "than the value of minimum permissible offset or same as the current offset value = %d."
                "Hence, no changes in offset value has been made",
                new_calc_offset,
                p_neigh_cell_info->initial_cell_offset);
    }

    SON_UT_TRACE_EXIT();
} /* son_mlb_calc_of_delta_and_newoffset_nbr_macro */

/******************************************************************************
 * Function Name  : calc_offset_in_idle_mod 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : ho_report    -     Pointer to the rrm_ho_report
 *                : p_neigh_cell_info - Pointer to neighbor cell info
 * Outputs        : None
 * Returns        : son_void_t 
 * Description    : It will calculate the offset in idle mode.
 *****************************************************************************/
son_void_t
calc_offset_in_idle_mod
(
        mlb_cell_context_t   *p_cell_ctxt,
        rrm_son_ho_report_t *ho_report,
        son_mlb_neigh_cell_info_t  *p_neigh_cell_info 
        )
{
    anr_cell_context_t              *p_anr_cell_ctxt = SON_PNULL;
    son_void_t                      *p_nr = SON_PNULL;
    son_error_et                    error_code = SON_NO_ERROR;
    son_rat_type_et                 rat_type = SON_CELL_UNDEFINED;
    anr_intra_rat_hash_nr_tuple_t   *p_eutran_nbr = SON_PNULL;
    son_global_cell_id_t        glbl_cell_id;


    SON_UT_TRACE_ENTER();

    /*fetch cell context from the ANR table*/
    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(&p_cell_ctxt->cell_id);
    if (SON_NULL == p_anr_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Cell is not present in ANR");
        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
    convert_rrm_gl_cell_id_to_son_gl_cell_id(&glbl_cell_id, (rrm_oam_global_cell_id_t *)&ho_report->trgt_cgi);
    /*fetch neighbor cell info from the ANR table using cell context*/
    p_nr = anr_find_nr(p_anr_cell_ctxt, &glbl_cell_id, &error_code, &rat_type);
    if (SON_NULL == p_nr)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "Neighbor Cell is not present in ANR");
        SON_UT_TRACE_EXIT();
        return;
    }

    if (rat_type == SON_EUTRAN_CELL)
    {
        p_eutran_nbr = ((anr_intra_rat_hash_nr_tuple_t *)p_nr);

        if (p_eutran_nbr->data.neighbor_info.bitmask & SON_CELL_OFFSET_PRESENT)
        {
            /*update mlb context for cell spefic offset*/
            p_neigh_cell_info->initial_cell_offset = p_eutran_nbr->data.neighbor_info.cell_specific_offset;
        }

        if (p_eutran_nbr->data.neighbor_info.bitmask & SON_CM_CELL_OFFSET_PRESENT)
        {
            /*update mlb context for cm cell spefic offset*/
            p_neigh_cell_info->initial_cm_cell_offset = p_eutran_nbr->data.neighbor_info.cm_cell_specific_offset;
        }
        /** eICIC changes start */
        if (p_eutran_nbr->data.neighbor_info.bitmask & SON_EICIC_INFO_PRESENT)
        {
            if( SON_CELL_INTF_SCHEME_TYPE_PRESENT & p_eutran_nbr->data.neighbor_info.eicic_info.bitmask)
            {
                p_neigh_cell_info->eicic_scheme = 
                    p_eutran_nbr->data.neighbor_info.eicic_info.interference_scheme;
            }
            else
            {
                p_neigh_cell_info->eicic_scheme = SON_NO_INTF_SCHEME;
            }
             /* bug_13866_start */
            if(p_eutran_nbr->data.neighbor_info.eicic_info.bitmask & SON_CELL_ABS_USAGE_THRESHOLD_PRESENT)
            {
               if(p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask & SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT)
               {
                    p_neigh_cell_info->min_abs_usage_threshold_aggressor =
                                       p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.min_abs_usage_threshold;  
               }
               else
               {
                   p_neigh_cell_info->min_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MIN_ABS_THRESHOLD;
               }    
               if(p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask & SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT)
               {
                    p_neigh_cell_info->max_abs_usage_threshold_aggressor =
                                       p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.max_abs_usage_threshold;  
               }
               else
               {
                   p_neigh_cell_info->max_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MAX_ABS_THRESHOLD;
               }    
            }
            else
            {
                    p_neigh_cell_info->min_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MIN_ABS_THRESHOLD;
                   p_neigh_cell_info->max_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MAX_ABS_THRESHOLD;
            }
                    
         
           /* bug_13866_end */
       }
        /** eICIC changes end */
        /* check for Serving cell is femto cell and Nbr is macro cell.If access
         * mode is open a cell is a macro cell.*/
        if ((g_mlb_context.enb_type ==  SON_HOME_ENB) &&
                /*SPR 19116 Fix Start*/
                (p_eutran_nbr->data.enb_id.enb_type == SON_MACRO_ENB) &&
                /*SPR 19116 Fix Stop*/
                (p_eutran_nbr->data.neighbor_info.access_mode == SON_OPEN))
        {
            /*Serving cell is femto cell and neighbor cell is macro cell this is case for femto priority scenario*/
            son_mlb_calc_of_delta_and_newoffset_nbr_macro(p_cell_ctxt, ho_report, p_neigh_cell_info);
            p_neigh_cell_info->nbr_csg_id_present = SON_FALSE;
        }
        else
        {
            /*Serving cell is macro cell and neighbor cell is macro or femto cell*/
            son_mlb_calc_of_delta_and_newoffset_idle_mod(p_cell_ctxt, ho_report->trgt_cgi, p_neigh_cell_info);
            if ((p_eutran_nbr->data.neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT))
            {
                  p_neigh_cell_info->nbr_csg_id_present = SON_TRUE;
                  SON_MEMCPY(p_neigh_cell_info->csg_identity, p_eutran_nbr->data.neighbor_info.csg_identity,
                              sizeof(p_neigh_cell_info->csg_identity));
            }
        }
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_INFO,
                "Cell in not EUTRAN cell so not"
				" handling the offset calculation ");
    }

    SON_UT_TRACE_EXIT();
} /* calc_offset_in_idle_mod */

/******************************************************************************
 * Function Name  : son_mlb_ho_report_store_and_calc_offset 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : ho_report   -      Pointer to the rrm_ho_report
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : It will store ho report and calculate the offset.
 *****************************************************************************/
son_void_t
son_mlb_ho_report_store_and_calc_offset
(
        mlb_cell_context_t   *p_cell_ctxt,
        rrm_son_ho_report_t *ho_report
        )
{
    SON_UT_TRACE_ENTER();

    son_u8 neighbor_cell_found = SON_FALSE;
    son_u32 rsrp_index = 0;
    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL;
    son_mlb_neigh_cell_info_t *p_neigh_cell_info = SON_PNULL;
    /**********UT FIX********************/
    anr_cell_context_t              *p_anr_cell_ctxt = SON_PNULL;
    son_void_t                      *p_nr = SON_PNULL;
    son_error_et                    error_code = SON_NO_ERROR;
    son_rat_type_et                 rat_type = SON_CELL_UNDEFINED;
    son_global_cell_id_t        glbl_cell_id;

    /*check for count exceed from max */
    if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
    {
        /*in mobility change req the src_cgi1 will be the serving cell id for us */
        p_son_mlb_neigh_cell_info_record = (son_mlb_neigh_cell_info_record_t *)
              list_find_node(&p_cell_ctxt->neigh_cell_info_head,
            (son_void_t *)&ho_report->trgt_cgi.intra_rat_global_cell_id,
                son_mlb_object_id_keyof,
                son_compare_cell_id);

        if (p_son_mlb_neigh_cell_info_record != SON_PNULL)
        {
            p_neigh_cell_info = &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data;
            neighbor_cell_found = SON_TRUE;
            rsrp_index = p_neigh_cell_info->ho_count;
            if (ho_report->target_meas_strength.eutran_cell_strength.bitmask & RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT)
            {
                p_neigh_cell_info->rsrp[rsrp_index] = ho_report->target_meas_strength.eutran_cell_strength.rsrp;
            }
            else
            {
                /*ignore the HO Report and don’t update context*/
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                        "HO report does not have RSRP value "
                        "ignoring the msg,Cell 0x%x,neighbor Cell 0x%x",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), 
                    son_convert_char_cell_id_to_int_cell_id(p_neigh_cell_info->neigh_cell_id.cell_identity));
                return;
            }

            p_neigh_cell_info->ho_count++;
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                    "HO report received for Cell 0x%x "
                    " neighbor Cell 0x%x,HO count %d",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), 
                son_convert_char_cell_id_to_int_cell_id(p_neigh_cell_info->neigh_cell_id.cell_identity),
					p_neigh_cell_info->ho_count);

            /* SPR 22195 Fix Start */
            /* Code Removed */
            /* SPR 22195 Fix Stop */

            /*save the offset values*/
            if (ho_report->bitmask & HO_REPORT_OFFSET_ATTR_PRESENT)
            {
                p_neigh_cell_info->offset_attr.ofs =  ho_report->offset_attr.ofs;
                p_neigh_cell_info->offset_attr.ocs =  ho_report->offset_attr.ocs;
                p_neigh_cell_info->offset_attr.ofn =  ho_report->offset_attr.ofn;
                p_neigh_cell_info->offset_attr.off =  ho_report->offset_attr.off;
                p_neigh_cell_info->offset_attr.hys =  ho_report->offset_attr.hys;
            }

            /*check if ho count has exceeded the max ho count configured*/
            if (p_neigh_cell_info->ho_count >= g_mlb_context.mlb_config_param.max_ho_sample)
            {
                /*call the idle load balancing algo*/
                calc_offset_in_idle_mod(p_cell_ctxt, ho_report, p_neigh_cell_info);
            }
        }
        else
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF, "May be new nbr cell entry come");
        }
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF, "New nbr cell entry come");
    }

    if ((neighbor_cell_found == SON_FALSE) &&
        (MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head) < MAX_NEIGH_CELL))
    {
        /**********UT FIX: check if NBR is present at ANR*******************
         *fetch cell context from the ANR table*/
        p_anr_cell_ctxt = anr_get_cell_context_from_global_context(&p_cell_ctxt->cell_id);
        if (SON_NULL == p_anr_cell_ctxt)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_ERROR,
                    "Cell is not present in ANR");
            SON_UT_TRACE_EXIT();
            return;
        }

        SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
        convert_rrm_gl_cell_id_to_son_gl_cell_id(&glbl_cell_id, (rrm_oam_global_cell_id_t *)&ho_report->trgt_cgi);
        /*fetch neighbor cell info from the ANR table using cell context*/
        p_nr = anr_find_nr(p_anr_cell_ctxt, &glbl_cell_id, &error_code, &rat_type);
        if (SON_NULL == p_nr)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF,
                    "Neighbor Cell is not present in ANR");
            SON_UT_TRACE_EXIT();
            return;
        }

        p_son_mlb_neigh_cell_info_record =
            (son_mlb_neigh_cell_info_record_t *)son_mem_get(sizeof(son_mlb_neigh_cell_info_record_t));
        if (p_son_mlb_neigh_cell_info_record == SON_PNULL)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR, "Memory allocation failed for p_son_mlb_neigh_cell_info_record");
            return;
        }

        SON_MEMSET(p_son_mlb_neigh_cell_info_record, 0, sizeof(son_mlb_neigh_cell_info_record_t));
        SON_MEMCPY(&p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.neigh_cell_id,
            &ho_report->trgt_cgi.intra_rat_global_cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
        /*save the rsrp value*/
        if (ho_report->target_meas_strength.eutran_cell_strength.bitmask & RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT)
        {
            /*increament ho count*/
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.ho_count++;
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.rsrp[0] =
                ho_report->target_meas_strength.eutran_cell_strength.rsrp;
        }

        /* SPR 22195 Fix Start */
        /* Code Removed */
        /* SPR 22195 Fix Stop */

        /*save the offset values*/
        if (ho_report->bitmask & HO_REPORT_OFFSET_ATTR_PRESENT)
        {
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.offset_attr.ofs =  ho_report->offset_attr.ofs;
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.offset_attr.ocs =  ho_report->offset_attr.ocs;
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.offset_attr.ofn =  ho_report->offset_attr.ofn;
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.offset_attr.off =  ho_report->offset_attr.off;
            p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.offset_attr.hys =  ho_report->offset_attr.hys;
        }

        MLB_NBR_Q_ENQUEUE(&p_cell_ctxt->neigh_cell_info_head, p_son_mlb_neigh_cell_info_record);
        /*increament neighbor count*/
        p_cell_ctxt->count_for_neigh_cell++;
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                "HO report received for Cell 0x%x "
                " neighbor Cell 0x%x,HO count %d",
            son_convert_char_cell_id_to_int_cell_id(
                p_cell_ctxt->cell_id.cell_identity),
            son_convert_char_cell_id_to_int_cell_id(p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.neigh_cell_id
                .
                cell_identity), p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.ho_count);
    }

    SON_UT_TRACE_EXIT();
} /* son_mlb_ho_report_store_and_calc_offset */

/*SONMLB-- Centralized support start*/

/*****************************************************************************
 * Function Name  : mlb_send_ho_load_params_info
 * Inputs         : src_cell_id - serving cell Id
 *                  ho_params_flag_on - the send funtion is for HO params or load params
 *                  ho_params - HO Params if ho_params_flag_on is TRUE
 *                  load_params - Load Params if ho_params_flag_on is FALSE
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates and sends the feature state
 *                  change indication message to SONMgmtIfH.
 ****************************************************************************/
son_void_t
mlb_send_ho_load_params_info
(
    son_intra_rat_global_cell_id_t      *p_src_cell_id,
    son_bool_et ho_params_flag_on,
    son_mlb_ho_params_t                 *p_ho_params,
    son_mlb_load_params_t               *p_load_params
)
{
    son_mlb_params_info_t mlb_params_info;
    son_size_t mlb_params_info_len =
        sizeof(son_mlb_params_info_t);


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&mlb_params_info, 0,
        mlb_params_info_len);
    /* Copy the Serving CGI */
    SON_MEMCPY(&mlb_params_info.srv_cgi, p_src_cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    if (SON_TRUE == ho_params_flag_on)
    {
        mlb_params_info.bitmask = SON_OAM_MLB_HO_PARAMS_PRESENT;
        /*Copy HO Params  */
        SON_MEMCPY(&mlb_params_info.ho_params, p_ho_params, sizeof(son_mlb_ho_params_t));
    }
    else
    {
        mlb_params_info.bitmask = SON_OAM_MLB_LOAD_PARAMS_PRESENT;
        /*Copy Load Params  */
        SON_MEMCPY(&mlb_params_info.load_params, p_load_params, sizeof(son_mlb_load_params_t));
    }

    son_create_send_buffer((son_u8 *)&mlb_params_info,
        SON_MLB_MODULE_ID,
        SON_MIF_MODULE_ID,
        SONMLB_HO_LOAD_PRARAMS_INFO,
        mlb_params_info_len);

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "Load Balancing "
        "params sent for Cell 0x%x",
        son_convert_char_cell_id_to_int_cell_id(mlb_params_info.srv_cgi.cell_identity));

    SON_UT_TRACE_EXIT();
} /* mlb_send_ho_load_params_info */

/*SONMLB-- Centralized support end*/

/******************************************************************************
 * Function Name  : mlb_fsm_ho_report_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_RRM_HO_REPORT.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_ho_report_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    SON_UT_TRACE_ENTER();
    rrm_son_ho_report_t *ho_report = (rrm_son_ho_report_t *)p_msg;
    /*SONMLB-- Centralized support start*/
    son_mlb_ho_params_t ho_params;
    SON_MEMSET(&ho_params, 0, sizeof(son_mlb_ho_params_t));
    if (SON_MODE_CENTRALIZED == g_mlb_context.mlb_config_param.mlb_mode)
    {
        if (HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT & ho_report->bitmask)
        {
            if (RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT & ho_report->serving_meas_strength.
                bitmask)
            {
                ho_params.bitmask |= SON_OAM_MLB_SRC_RSRP_PRESENT;
                ho_params.src_rsrp = ho_report->serving_meas_strength.rsrp;
            }
        }

        if (HO_REPORT_OFFSET_ATTR_PRESENT & ho_report->bitmask)
        {
            ho_params.bitmask |= SON_OAM_MLB_CELL_OFSSET_ATTR_PRESENT;
            SON_MEMCPY(&ho_params.offset_attr, &ho_report->offset_attr, sizeof(son_cell_offset_attr_t));
        }

        if (HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT & ho_report->bitmask)
        {
            if (RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT & ho_report->target_meas_strength.
                bitmask)
            {
                if (RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT & ho_report->target_meas_strength.
                    eutran_cell_strength.bitmask)
                {
                    ho_params.bitmask |= SON_OAM_MLB_TARGET_RSRP_PRESENT;
                    SON_MEMCPY(&ho_params.trgt_cgi, &ho_report->trgt_cgi.intra_rat_global_cell_id,
                        sizeof(son_intra_rat_global_cell_id_t));
                    ho_params.trgt_rsrp = ho_report->target_meas_strength.eutran_cell_strength.rsrp;
                }

                /*send HO report info to OAM*/
                mlb_send_ho_load_params_info((son_intra_rat_global_cell_id_t *)&ho_report->src_cgi,
                    SON_TRUE, &ho_params, SON_PNULL);
            }
        }

        return RELEASE_SON_BUFFER;
    }
                    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(evnt_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
    /*SONMLB-- Centralized support end*/
    son_mlb_ho_report_store_and_calc_offset(p_cell_ctxt, ho_report);
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_ho_report_handler */

/*Rel 3.0 Multi Sector code changes START*/
/******************************************************************************
 * Function Name  : son_mlb_send_mobility_change_req 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : p_neigh_cell_info - Pointer to the neighbor context
 *                : ho_trigger_change_nbr - Value of proposed HO trigger change
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : function to creta and send Mobility Change Req to SON-X2
 *****************************************************************************/
son_void_t
son_mlb_send_mobility_change_req
(
    mlb_cell_context_t *p_cell_ctxt,
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
    son_s8 ho_trigger_change_nbr
)
{
    SON_UT_TRACE_ENTER();
    mlb_mobility_change_req_t req;
  /*SPR 19030 changes start*/
  anr_cell_context_t      *p_anr_cell_ctxt = SON_PNULL;
  SON_HASH *p_hash = SON_PNULL;
  SON_HASH_NODE           *p_hash_node = SON_PNULL;
  anr_intra_rat_hash_rec_t        hash_data;
  anr_intra_rat_hash_nr_tuple_t   *p_tpl      = SON_PNULL;
  /*SPR 19030 changes end*/

    SON_MEMSET(&req, 0, sizeof(mlb_mobility_change_req_t));

    SON_MEMCPY(&req.src_cgi1, &p_cell_ctxt->serv_cell_load_info.g_cell_id, sizeof(son_intra_rat_global_cell_id_t));
    SON_MEMCPY(&req.src_cgi2, &p_neigh_cell_info->neigh_cell_load_info.g_cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    /*poulatting  proposed nbr HO trigger change params*/
    /*SPR 20523 START*/
    /*code deleted*/
    req.proposed_mobility_params = (SON_Q_OFFSET_ABSOLUTE_VAL_DB2) * ho_trigger_change_nbr; 
    /*SPR 20523 END*/

    /*poulatting  target ENB ID*/
  /*SPR 19030 changes start*/

  p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
      &p_cell_ctxt->cell_id);

  p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;

  SON_MEMSET(&hash_data, 0, sizeof(anr_intra_rat_hash_rec_t));

  SON_MEMCPY(&hash_data.neighbor_info.nbr_cgi,&p_neigh_cell_info->neigh_cell_id,
      sizeof(son_intra_rat_global_cell_id_t));

  if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
  {
    p_tpl = (anr_intra_rat_hash_nr_tuple_t *)
      YMEMBEROF(anr_intra_rat_hash_nr_tuple_t, anchor, p_hash_node);

    req.trgt_enbid = p_tpl->data.enb_id;
    /* SPR 19221: CID 108599 fix start */
  }

  /*SPR 19030 changes end*/
  /*Rel 3.0 Multi Sector code changes Start*/

  if (!anr_is_nbr_cell_on_serving_enb(&req.trgt_enbid))
  {
      son_create_send_buffer((son_u8 *)&req, SON_MLB_MODULE_ID,
          SON_X2_MODULE_ID, SONMLB_MOBILITY_CHANGE_REQ,
          sizeof(mlb_mobility_change_req_t));
  }
  else
  {
      son_create_send_buffer((son_u8 *)&req, SON_MLB_MODULE_ID,
          SON_MLB_MODULE_ID, SONMLB_INTERNAL_MOBILITY_CHANGE_REQ,
          sizeof(mlb_mobility_change_req_t));
  }

  /*Rel 3.0 Multi Sector code changes Stop*/
  /*set the Mobility change Req flag as true*/
  p_neigh_cell_info->mob_change_req_sent = SON_TRUE;

  SON_UT_TRACE_EXIT();
  /* SPR 19221: CID 108599 fix stop */
} /* son_mlb_send_mobility_change_req */


/*Rel 3.0 Multi Sector code changes  STOP*/


/******************************************************************************
 * Function Name  : son_mlb_calc_delta_and_newoffset_in_connected_mod 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : p_neigh_cell_info   -    Pointer to the neighbor context
 * Outputs        : None
 * Returns        : None 
 * Description    : It will calculate the delta and new offset in connected mode.
 *****************************************************************************/
son_void_t
son_mlb_calc_delta_and_newoffset_in_connected_mod
(
    mlb_cell_context_t   *p_cell_ctxt,
    son_mlb_neigh_cell_info_t           *p_neigh_cell_info
)
{
  SON_UT_TRACE_ENTER();
  son_u8 nbr_load, serving_load;
  son_float diff_load;
  son_s8 ho_threshold_serv_old, ho_threshold_serv_new, ho_trigger_change_serv, ho_trigger_change_nbr;
  /*SPR 20523 START*/
  /*+SPR 22535*/
  son_s8 mapped_q_offset_max = SON_NULL; 
  son_s8 mapped_new_cell_offset_connected = SON_NULL; 
  son_s8 mapped_q_offset_min = SON_NULL;
  son_s8 mapped_initial_cm_cell_offset = SON_NULL; 
  /*-SPR 22535*/
  /*SPR 20523 END*/

  nbr_load = p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val;
  serving_load = p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val;

    /* SPR 19015 Fix Start */
    if((p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask &
                RRM_SON_CELL_CAP_CLASS_VAL_PRESENT) &&
            (p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask &
             RRM_SON_CELL_CAP_CLASS_VAL_PRESENT))
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "nbr_load and serving_load is:%d %d",nbr_load, serving_load);

        nbr_load = ((nbr_load *
                    p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val)/
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val);
        
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "RRM_SON_CELL_CAP_CLASS_VAL is present & calculated nbr_load [%d] serving load [%d]"
                "neighbour cell cap class [%d]"
                "serving cell cap class [%d]",
                nbr_load,serving_load,p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val,
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val);
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_DETAILED,
                "RRM_SON_CELL_CAP_CLASS_VAL is not present so calculated nbr_load :%d serving load [%d]",nbr_load,serving_load);
    }
    /* SPR 19015 Fix Stop */

  /*MLB Enhancement : start*/
  if (serving_load < g_mlb_context.mlb_config_param.mlb_start_threshold)
  {
    /*MLB algo should be triggered*/
    p_cell_ctxt->mlb_start_flag = SON_TRUE;

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "Serving available capacity"
        "[%d] is less than the MLB start threshold[%d]."
        "connected mode algo could be triggered", serving_load,
        g_mlb_context.mlb_config_param.mlb_start_threshold);
  }
  else if (serving_load > g_mlb_context.mlb_config_param.mlb_stop_threshold)
  {
    /*MLB algo should not be triggered*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "Serving available capacity"
        "[%d] is more than the MLB stop threshold[%d]."
        "Not calling the connected mode algo", serving_load,
        g_mlb_context.mlb_config_param.mlb_stop_threshold);
    p_cell_ctxt->mlb_start_flag = SON_FALSE;
    p_neigh_cell_info->num_of_mlb_iteration = SON_NULL;
    return;
  }

  if (p_cell_ctxt->mlb_start_flag == SON_TRUE)
  {
    if (SON_NULL == p_neigh_cell_info->num_of_mlb_iteration)
    {
      p_cell_ctxt->dl_cap_val[p_neigh_cell_info->num_of_mlb_iteration] = serving_load;
    }
    else if ((p_neigh_cell_info->num_of_mlb_iteration < NUM_OF_MLB_ITERATION) &&
        (serving_load <= (p_cell_ctxt->dl_cap_val[p_neigh_cell_info->num_of_mlb_iteration - 1] +
                          g_mlb_context.mlb_config_param.min_capacity_gain)))
    {
      p_cell_ctxt->dl_cap_val[p_neigh_cell_info->num_of_mlb_iteration] = serving_load;
    }
    else
    {
      if (p_neigh_cell_info->num_of_mlb_iteration == NUM_OF_MLB_ITERATION)
      {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_INFO,
            "Serving available capacity is not changing substancially after maximum iteration[%d]."
            "old serving available capacity[%d] and new serving available capacity[%d]."
            "Not calling the connected mode algo",
            p_neigh_cell_info->num_of_mlb_iteration,
            p_cell_ctxt->dl_cap_val[p_neigh_cell_info->num_of_mlb_iteration - 1], serving_load);
        p_cell_ctxt->mlb_start_flag = SON_FALSE;
        p_neigh_cell_info->num_of_mlb_iteration = SON_NULL;
        return;
      }

      SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
          SON_INFO,
          "Old serving available capacity[%d] and new serving available capacity[%d]",
          p_cell_ctxt->dl_cap_val[p_neigh_cell_info->num_of_mlb_iteration - 1], serving_load);
      p_neigh_cell_info->num_of_mlb_iteration = SON_NULL;
      p_cell_ctxt->dl_cap_val[p_neigh_cell_info->num_of_mlb_iteration] = serving_load;
    }
  }
  else
  {
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_INFO,
        "MLB start flag is Off,not calling the connected mode algo."
        "serving available capacity[%d]",
        serving_load);
    return;
  }

  SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
      SON_BRIEF,
      "serving available capacity is[%d] and number of iteration[%d]",
      serving_load, p_neigh_cell_info->num_of_mlb_iteration);

  /*MLB Enhancement : End*/

  /*calculate new offset for neighbor cell.threshold load
   * value we read as in percentage from CFG file son_oam.cfg
   * exp : if threshold value is 10 in cfg file the actual value should be 10/100 = 0.1*/

  if (serving_load > nbr_load)
  {
    diff_load = (serving_load - nbr_load);
  }
  else if (serving_load < nbr_load)
  {
    diff_load = (nbr_load - serving_load);
  }
  else
  {
    /*return from here*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "serving and nbr load are same.Not calling the connected mode algo");
    return;
  }

  /* ∆ = (|Pi –Ps| - Pth,conn) / (1- Pth,conn ) * (OCNmax – OCNmin,step_size) + OCNmin,step_size.
   * here we read load in percentage from cfg file.so need to divide load by 100*/
  /*SPR 20523 START*/
  mapped_q_offset_max = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_max);
  mapped_q_offset_min = son_map_q_offset_value(p_cell_ctxt->eutra_q_offset_config.q_offset_min);
  mapped_initial_cm_cell_offset = son_map_q_offset_value(p_neigh_cell_info->initial_cm_cell_offset);

  p_cell_ctxt->delta_offset_connected =
    (son_u32)(((diff_load/100.0 - p_cell_ctxt->eutra_threshold_connected /
            100.0) / (1 - p_cell_ctxt->eutra_threshold_connected / 100.0)) *
        (mapped_q_offset_max -
         g_mlb_context.mlb_config_param.eutra_ocn_min_step)
        + g_mlb_context.mlb_config_param.eutra_ocn_min_step);

  SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
          SON_DETAILED,"mlb_cell_calc_serv_ho_threshold:"
          "diff_load[%f], p_cell_ctxt->eutra_threshold_connected [%lu]"
          "p_cell_ctxt->eutra_threshold_connected [%lu]"
          "mapped_q_offset_max [%d],g_mlb_context.mlb_config_param.eutra_ocn_min_step[%u],delta_offset_connected [%lu],"
          "p_neigh_cell_info->initial_cm_cell_offset [%u],mapped_initial_cm_cell_offset [%d]",
          diff_load,p_cell_ctxt->eutra_threshold_connected,p_cell_ctxt->eutra_threshold_connected,mapped_q_offset_max,
          g_mlb_context.mlb_config_param.eutra_ocn_min_step,p_cell_ctxt->delta_offset_connected,
          p_neigh_cell_info->initial_cm_cell_offset,mapped_initial_cm_cell_offset);        

  /*if serving and neighbor load difference is greater than threhhold load that means the
   * serving has more available capacity and the handover offset OCN shall be decreased so that UEs can't move to neighbor*/
  if ((serving_load > nbr_load) && ((diff_load) > p_cell_ctxt->eutra_threshold_connected))
  {
    /** eICIC changes start */
    if (SON_ACTIVATE_EICIC == p_neigh_cell_info->eicic_scheme)
    {
      /** Case a) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as ACT_AS_VICTIM
       *           then SON shall consider the ABS usage threshold received in SON_OAM_CELL_INFO_IND */
      if (p_cell_ctxt->provision_type == SON_ACT_AS_VICTIM)
      {
        /* bug_13866_start */
        if (son_mlb_offset_change_required(p_cell_ctxt->min_abs_usage_threshold_victim,
              p_cell_ctxt->max_abs_usage_threshold_victim,
              p_cell_ctxt->serv_cell_load_info.abs_status.dl_abs_status)
            == SON_TRUE)
          /* bug_13866_end */
        {
          /*SPR 14398 Fix start */
          /*SPR 15884 Fix Start*/
          mapped_new_cell_offset_connected = 
            mapped_initial_cm_cell_offset - p_cell_ctxt->delta_offset_connected;
           /*SPR 15884 Fix Stop*/
          /*SPR 14398 Fix end*/
          SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
              SON_BRIEF,
              "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
              "[EICIC activated case serving cell is victim]\n"
              "initial cell offset [%d] : \n"
              "new cell offset [%d]\n"
              "delta offset[%d]\n",
              p_neigh_cell_info->initial_cm_cell_offset,
              p_neigh_cell_info->new_cell_offset_connected,
              p_cell_ctxt->delta_offset_connected);
        }
        else
        {
          mapped_new_cell_offset_connected = 
            mapped_initial_cm_cell_offset; 
          SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
              SON_DETAILED,"eICIC algo : No offset change required\n");
        }
      }
      /** Case b) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as ACT_AS_AGGRESSOR
       *          then SON shall consider the ABS usage threshold received in SON_OAM_CELL_INFO_IND */
      else if (p_cell_ctxt->provision_type == SON_ACT_AS_AGGRESSOR)
      {
        /* bug_13866_start */
        if (son_mlb_offset_change_required(p_neigh_cell_info->min_abs_usage_threshold_aggressor,
              p_neigh_cell_info->max_abs_usage_threshold_aggressor,
              p_neigh_cell_info->neigh_cell_load_info.abs_status.dl_abs_status)
            == SON_TRUE)
          /* bug_13866_end */
        {
          /*SPR 14398 Fix start */
          /*SPR 15884 Fix Start*/
          mapped_new_cell_offset_connected = 
            mapped_initial_cm_cell_offset - p_cell_ctxt->delta_offset_connected;
          /*SPR 15884 Fix Stop*/
          /*SPR 14398 Fix end */
          SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
              SON_BRIEF,
              "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
              "[EICIC activated case serving cell is aggressor]\n"
              "initial cell offset [%d] \n:"
              "new cell offset [%d]\n"
              "delta offset[%d]\n",
              p_neigh_cell_info->initial_cm_cell_offset,
              p_neigh_cell_info->new_cell_offset_connected,
              p_cell_ctxt->delta_offset_connected);
        }
        else
        {
          mapped_new_cell_offset_connected =
            mapped_initial_cm_cell_offset; 
          SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
              SON_DETAILED,"eICIC algo : No offset change required\n");
        }
      }
      /** Case c) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as 
       *          ACT_AS_AGGRESSOR_AND_VICTIM then SON shall consider the ABS usage threshold
       *          received in SON_OAM_CELL_INFO_IND */
      else if (p_cell_ctxt->provision_type == SON_ACT_AS_AGGRESSOR_AND_VICTIM)
      {
        /* bug_13866_start */
        if (((SON_TRUE == son_mlb_offset_change_required(p_neigh_cell_info->min_abs_usage_threshold_aggressor,
                  p_neigh_cell_info->max_abs_usage_threshold_aggressor,
                  p_neigh_cell_info->neigh_cell_load_info.abs_status.dl_abs_status))) && 
            ((SON_TRUE == son_mlb_offset_change_required(p_cell_ctxt->min_abs_usage_threshold_victim,
                                                         p_cell_ctxt->max_abs_usage_threshold_victim,
                                                         p_cell_ctxt->serv_cell_load_info.abs_status.dl_abs_status))))
          /* bug_13866_end */
        {
          /*SPR 14398 Fix start */
          /*SPR 15884 Fix Start*/
          mapped_new_cell_offset_connected =
            mapped_initial_cm_cell_offset - p_cell_ctxt->delta_offset_connected;
          /*SPR 15884 Fix Stop*/
          /*SPR 14398 Fix end */
        }
        else
        {
          mapped_new_cell_offset_connected =
            mapped_initial_cm_cell_offset;
          SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
              SON_DETAILED,"eICIC algo : No offset change required\n");
        }
      }
      /*+ eICIC review commnents incorporated +*/
      /** Case d) if in SON_RRM_EICIC_DISABLE_ENABLE_IND API SON receives provision type as 
       *          NONE then SON shall consider the ABS usage threshold
       *          received in SON_OAM_CELL_INFO_IND */
      else if (p_cell_ctxt->provision_type == SON_EICIC_DEACTIVATED)
      {
        /*+ eICIC review commnents incorporated +*/
        /*SPR 14398 Fix start */
        /*SPR 15884 Fix Start*/
        mapped_new_cell_offset_connected = 
          mapped_initial_cm_cell_offset - p_cell_ctxt->delta_offset_connected;
        /*SPR 15884 Fix Stop*/
        /*SPR 14398 Fix end */
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
            "[EICIC Deactivated case]initial cell offset [%d] : new cell offset [%d]\n",
            p_neigh_cell_info->initial_cm_cell_offset,
            p_neigh_cell_info->new_cell_offset_connected);
        /*- eICIC review commnents incorporated -*/
      }
    }
    else
    {
        mapped_new_cell_offset_connected = 
          mapped_initial_cm_cell_offset - p_cell_ctxt->delta_offset_connected;
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
            SON_BRIEF,
            "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
            "[EICIC Deactivated case]initial cell offset [%d] : new cell offset [%d]\n",
            p_neigh_cell_info->initial_cm_cell_offset,
            p_neigh_cell_info->new_cell_offset_connected);
    }
  }
  /** eICIC changes end */
  /*if serving and neighbor load difference is less than threhhold load that means the neighbor 
   * has more available capacity and the handover offset OCN shall be increased so that UEs can move to neighbor*/
  else if ((serving_load < nbr_load) && (diff_load) > p_cell_ctxt->eutra_threshold_connected)
  {
    /*SPR 14398 Fix start */
    /*SPR 15884 Fix Start*/
    mapped_new_cell_offset_connected = 
      mapped_initial_cm_cell_offset + p_cell_ctxt->delta_offset_connected;
    /*SPR 15884 Fix Stop*/
    /*SPR 14398 Fix end */
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
        "[Non EICIC case]initial cell offset [%d] : new cell offset [%d]\n",
        p_neigh_cell_info->initial_cm_cell_offset,
        p_neigh_cell_info->new_cell_offset_connected);
  }
  else
  {
    /*return from here*/
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
        "serving and nbr load difference is less than threshold."
        "Not calling the connected mode algo \n");
    p_cell_ctxt->mlb_start_flag = SON_FALSE;
    return;
  }

  /*SPR 15742 Fix Start*/
  /*Lines deleted*/
  /*SPR 15742 Fix End*/

  /*if calculated cell offset is greater than max use the max value*/
  if (mapped_new_cell_offset_connected > mapped_q_offset_max)
  {
    mapped_new_cell_offset_connected = mapped_q_offset_max;
  }

  /*SPR 15742 Fix Start*/
  /*if calculated cell offset is lesser than min use the min value*/
  else if (mapped_new_cell_offset_connected < mapped_q_offset_min)
  {
    mapped_new_cell_offset_connected = mapped_q_offset_min;
  }
  /*SPR 15742 Fix End*/
  SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
          SON_DETAILED,"mapped_new_cell_offset_connected [%d]",mapped_new_cell_offset_connected);

  /*calculate the HO range change for serving cell.
   * event A3 based HO threshold = hys + off +ocs + ocs - ofn - ocn*/

  /*calculate HO threshold of serving using old OCN*/
  ho_threshold_serv_old = p_neigh_cell_info->offset_attr.hys + p_neigh_cell_info->offset_attr.off +
    p_neigh_cell_info->offset_attr.ocs + p_neigh_cell_info->offset_attr.ofs -
    p_neigh_cell_info->offset_attr.ofn - mapped_initial_cm_cell_offset;

  /*calculate HO threshold of serving using new calculated OCN*/
  ho_threshold_serv_new = p_neigh_cell_info->offset_attr.hys + p_neigh_cell_info->offset_attr.off + 
    p_neigh_cell_info->offset_attr.ocs + p_neigh_cell_info->offset_attr.ofs -
    p_neigh_cell_info->offset_attr.ofn - mapped_new_cell_offset_connected;

  SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
          SON_DETAILED,"ho_threshold_serv_old [%d],ho_threshold_serv_new [%d]",ho_threshold_serv_old,ho_threshold_serv_new);



  /*calculate HO threshold change of serving*/
  ho_trigger_change_serv = ho_threshold_serv_new - ho_threshold_serv_old;

  SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
          SON_DETAILED,"ho_trigger_change_serv[%d],p_neigh_cell_info->new_cell_offset_connected[%d]",
          ho_trigger_change_serv,p_neigh_cell_info->new_cell_offset_connected);

  /*calculate HO threshold change of nbr.If serving HO threshold,ho_trigger_change_serv, is increased by 2 
   * then the nbr should be decreased by -2*/
  if(ho_trigger_change_serv > SON_Q_OFFSET_ABSOLUTE_VAL_DB10)
  {
      ho_trigger_change_serv = SON_Q_OFFSET_ABSOLUTE_VAL_DB10;
  }    
  else if(ho_trigger_change_serv < SON_Q_OFFSET_ABSOLUTE_VAL_DB_10)
  {
      ho_trigger_change_serv = SON_Q_OFFSET_ABSOLUTE_VAL_DB_10;
  }    

  ho_trigger_change_nbr = (SON_Q_OFFSET_ABSOLUTE_VAL_DB_1) * ho_trigger_change_serv;

  p_neigh_cell_info->new_cell_offset_connected = son_reverse_map_db_to_enum(mapped_initial_cm_cell_offset + ho_trigger_change_serv, mapped_initial_cm_cell_offset);

  if (mapped_new_cell_offset_connected != mapped_initial_cm_cell_offset)
  {
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
        SON_BRIEF,
        "son_mlb_calc_delta_and_newoffset_in_connected_mod:"
        "initial cell offset [%d] : new cell offset [%d],ho_trigger_change_nbr[%d]\n",
        mapped_initial_cm_cell_offset,
        mapped_new_cell_offset_connected, ho_trigger_change_nbr);


    /*Rel 3.0 Multi Sector code changes Start*/
    son_mlb_send_mobility_change_req(p_cell_ctxt, p_neigh_cell_info, ho_trigger_change_nbr);
    /*Rel 3.0 Multi Sector code changes Stop*/
    /*SPR 20523 END*/

    /*MLB Enhancement : start*/
    son_mlb_mro_nbr_cell_status(p_cell_ctxt->cell_id, p_neigh_cell_info->neigh_cell_id, MLB_NBR_STATE_ENABLED);
    /*MLB Enhancement : End*/
  }

  SON_UT_TRACE_EXIT();
} /* son_mlb_calc_delta_and_newoffset_in_connected_mod */

/******************************************************************************
 * Function Name  : mlb_save_load_report 
 * Inputs         : p_cell_context -   Pointer to mlb cell context
 *                : nbr_load_info  -   Neighbor load info rcevived from RRM
 *                : p_neigh_cell_info   -    Pointer to the neighbor context
 * Outputs        : None
 * Returns        : son_u8
 * Description    : saving load report nbr info in mlb context.
 *****************************************************************************/
son_u8
mlb_save_load_report
(
 mlb_cell_context_t   *p_cell_ctxt,
 rrm_son_cell_load_info_t nbr_load_info,
 son_mlb_neigh_cell_info_t *p_neigh_cell_info
 )
{
    SON_UT_TRACE_ENTER();

    anr_cell_context_t              *p_anr_cell_ctxt = SON_PNULL;
    son_void_t                      *p_nr = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t   *p_eutran_nbr = SON_PNULL;
    son_global_cell_id_t        glbl_cell_id;
    son_u8 ret = SON_FALSE;
    son_rat_type_et rat_type = SON_CELL_UNDEFINED;
    son_error_et   error_code = SON_NO_ERROR;

    /*fetch cell context from the ANR table*/
    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(&p_cell_ctxt->cell_id);
    if (SON_NULL == p_anr_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Cell is not present in ANR");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    SON_MEMSET(&glbl_cell_id, 0, sizeof(son_global_cell_id_t));
    glbl_cell_id.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

    SON_MEMCPY(&glbl_cell_id.intra_rat_global_cell_id,
            &nbr_load_info.g_cell_id,
            sizeof(son_intra_rat_global_cell_id_t));

    /*fetch neighbor cell info from the ANR table using cell context*/
    p_nr = anr_find_nr(p_anr_cell_ctxt, &glbl_cell_id, &error_code, &rat_type);
    if (SON_NULL == p_nr)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_BRIEF,
                "Neighbor Cell is not present in ANR");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    if (rat_type == SON_EUTRAN_CELL)
    {
        p_eutran_nbr = ((anr_intra_rat_hash_nr_tuple_t *)p_nr);

        if (p_eutran_nbr->data.neighbor_info.bitmask & SON_CM_CELL_OFFSET_PRESENT)
        {
            /*update mlb context for cm cell spefic offset*/
            p_neigh_cell_info->initial_cm_cell_offset = p_eutran_nbr->data.neighbor_info.cm_cell_specific_offset;
        }

        /*check for neighbor is macro or femto*/
        if ((p_eutran_nbr->data.neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT))
        {
            p_neigh_cell_info->nbr_csg_id_present = SON_TRUE;
            SON_MEMCPY(p_neigh_cell_info->csg_identity, p_eutran_nbr->data.neighbor_info.csg_identity,
                       sizeof(p_neigh_cell_info->csg_identity));
        }
        else
        {
            p_neigh_cell_info->nbr_csg_id_present = SON_FALSE;
        }
        /*+ eICIC review commnents incorporated +*/
        //lines deleted
        /*- eICIC review commnents incorporated -*/
        /** eICIC changes start */
        if (p_eutran_nbr->data.neighbor_info.bitmask & SON_EICIC_INFO_PRESENT)
        {
            /*+ eICIC review commnents incorporated +*/
            if( SON_CELL_INTF_SCHEME_TYPE_PRESENT & p_eutran_nbr->data.neighbor_info.eicic_info.bitmask)
            /*- eICIC review commnents incorporated -*/
            {
                p_neigh_cell_info->eicic_scheme = 
                    p_eutran_nbr->data.neighbor_info.eicic_info.interference_scheme;
            }
            else
            {
                p_neigh_cell_info->eicic_scheme = SON_NO_INTF_SCHEME;
            }
             /* bug_13866_start */
            if(p_eutran_nbr->data.neighbor_info.eicic_info.bitmask & SON_CELL_ABS_USAGE_THRESHOLD_PRESENT)
            {
               if(p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask & SON_CELL_MIN_ABS_USAGE_THRESHOLD_PRESENT)
               {
                    p_neigh_cell_info->min_abs_usage_threshold_aggressor =
                                       p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.min_abs_usage_threshold;  
               }
               else
               {
                   p_neigh_cell_info->min_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MIN_ABS_THRESHOLD;
               }    
               if(p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.bitmask & SON_CELL_MAX_ABS_USAGE_THRESHOLD_PRESENT)
               {
                    p_neigh_cell_info->max_abs_usage_threshold_aggressor =
                                       p_eutran_nbr->data.neighbor_info.eicic_info.abs_usage_threshold.max_abs_usage_threshold;  
               }
               else
               {
                   p_neigh_cell_info->max_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MAX_ABS_THRESHOLD;
               }    
            }
            else
            {
                   p_neigh_cell_info->min_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MIN_ABS_THRESHOLD;
                   p_neigh_cell_info->max_abs_usage_threshold_aggressor = SON_CELL_DEFAULT_MAX_ABS_THRESHOLD;
            }
                    
         
           /* bug_13866_end */
       }


        /*save nbr composite load*/
        if (nbr_load_info.bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT)
        {
            /*save load info received for a nbr in mlb nbr context*/
            SON_MEMCPY(&p_neigh_cell_info->neigh_cell_load_info, &nbr_load_info, sizeof(rrm_son_cell_load_info_t));
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                   "Load report have composite load value "
                   "for nbr cell 0x%x,dl cap val = %d",
                   son_convert_char_cell_id_to_int_cell_id(p_neigh_cell_info->neigh_cell_id.cell_identity),
                    p_neigh_cell_info->neigh_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val);
            ret = SON_TRUE;
        }
        else
        {
            /*ignore the HO Report and don’t update context*/
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                    "Load report does not have composite load value "
                    "ignoring the msg,Cell 0x%x,neighbor Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(
                    p_cell_ctxt->cell_id.cell_identity),
                son_convert_char_cell_id_to_int_cell_id(p_neigh_cell_info->neigh_cell_id.cell_identity));
            ret = SON_FALSE;
    }
        /* bug_13885_start */
        if (nbr_load_info.bitmask & RRM_SON_ABS_STATUS_PRESENT)
        {
            p_neigh_cell_info->neigh_cell_load_info.abs_status.dl_abs_status =
                    nbr_load_info.abs_status.dl_abs_status;
            SON_LOG(mlb_get_log_mode(),p_son_mlb_facility_name, SON_BRIEF,
                    "Load report have DL ABS status"
                    "for nbr Cell 0x%x, DL ABS status %d",
                    son_convert_char_cell_id_to_int_cell_id(p_neigh_cell_info->neigh_cell_id.cell_identity),
                    p_neigh_cell_info->neigh_cell_load_info.abs_status.dl_abs_status);
        }
        /* bug_13885_end */
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_save_load_report */

/******************************************************************************
 * Function Name  : mlb_fsm_load_report_handler 
 * Inputs         : evnt_id  -    event identity
 *                : p_cell_context -  Pointer to mlb cell context
 *                : p_cspl_hdr -   Pointer to STACKAPIHDR structure
 *                : p_msg     -       Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for Cell Specific SONMLB_RRM_LOAD_REPORT.
 *****************************************************************************/
son_buf_retain_status_et
mlb_fsm_load_report_handler
(
 son_mlb_index_et    evnt_id,
 mlb_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
    SON_UT_TRACE_EXIT();
    son_u32 i = 0;
    rrm_son_load_report_ind_t *load_report = (rrm_son_load_report_ind_t *)p_msg;
    son_u8 neighbor_cell_found = SON_FALSE;

    son_mlb_neigh_cell_info_record_t *p_son_mlb_neigh_cell_info_record = SON_PNULL;
    son_mlb_neigh_cell_info_t *p_neigh_cell_info = SON_PNULL;
    /*SONMLB-- Centralized support start*/
    son_mlb_load_params_t load_params;
    son_mlb_ho_params_t ho_params;
    SON_MEMSET(&ho_params, 0, sizeof(son_mlb_ho_params_t));
    SON_MEMSET(&load_params, 0, sizeof(son_mlb_load_params_t));
    /*SONMLB-- Centralized support end*/

    /*storing serving cell cell id present in load report to mlb context*/
    SON_MEMCPY(&p_cell_ctxt->serv_cell_load_info.g_cell_id, &load_report->serv_cell_load_info.g_cell_id,
            sizeof(son_intra_rat_global_cell_id_t));

    /*SONMLB-- Centralized support start*/
    if (SON_MODE_CENTRALIZED == g_mlb_context.mlb_config_param.mlb_mode)
    {
        if (RRM_SON_COMP_AVL_CAP_GRP_PRESENT & load_report->serv_cell_load_info.bitmask)
        {
            load_params.bitmask |= MLB_SRC_CELL_CAP_VAL_PRESENT;
            load_params.src_dl_comp_avl_cap_val = load_report->serv_cell_load_info.
                comp_avl_cap_grp.dl_comp_avl_cap.cap_val;
        }

        load_params.count = load_report->count;
        while (i < load_report->count && load_report->count <= RRM_MAX_NUM_CELLS)
        {
            SON_MEMCPY(&load_params.nbr_cell_load_info[i].trgt_cgi, &load_report->neigh_cell_load_info[i].
                    g_cell_id, sizeof(son_intra_rat_global_cell_id_t));
            if (RRM_SON_COMP_AVL_CAP_GRP_PRESENT & load_report->neigh_cell_load_info[i].
                    bitmask)
            {
                load_params.nbr_cell_load_info[i].bitmask |= MLB_TRGT_CELL_CAP_VAL_PRESENT;
                load_params.nbr_cell_load_info[i].trgt_dl_comp_avl_cap_val =
                    load_report->neigh_cell_load_info[i].comp_avl_cap_grp.dl_comp_avl_cap.cap_val;
            }

            i++;
        }

        /*send Load report info to OAM*/
        mlb_send_ho_load_params_info((son_intra_rat_global_cell_id_t *)&load_report->serv_cell_load_info.
                g_cell_id, SON_FALSE, SON_PNULL, &load_params);
        return RELEASE_SON_BUFFER;
    }

    /*SONMLB-- Centralized support end*/

    /*check for serving is macro or femto*/
    if ((g_mlb_context.enb_type == SON_HOME_ENB))
    {
        p_cell_ctxt->src_csg_id_present = SON_TRUE;
    }
    else
    {
        p_cell_ctxt->src_csg_id_present = SON_FALSE;
    }

    /*save serving hw load*/
    if (load_report->serv_cell_load_info.bitmask & RRM_SON_CELL_LOAD_INFO_HW_LOAD_PRESENT)
    {
        p_cell_ctxt->serv_cell_load_info.hw_load.dl = load_report->serv_cell_load_info.hw_load.dl;
        p_cell_ctxt->serv_cell_load_info.hw_load.ul = load_report->serv_cell_load_info.hw_load.ul;
    }

    /*save serving tnl load*/
    if (load_report->serv_cell_load_info.bitmask & RRM_SON_CELL_LOAD_INFO_S1_TNL_LOAD_PRESENT)
    {
        p_cell_ctxt->serv_cell_load_info.s1_tnl_load.dl = load_report->serv_cell_load_info.s1_tnl_load.dl;
        p_cell_ctxt->serv_cell_load_info.s1_tnl_load.ul = load_report->serv_cell_load_info.s1_tnl_load.ul;
    }

    /*save serving prb usage*/
    if (load_report->serv_cell_load_info.bitmask & RRM_SON_CELL_LOAD_INFO_RRS_PRESENT)
    {
        p_cell_ctxt->serv_cell_load_info.rrs.dl_gbr_prb_usage = load_report->serv_cell_load_info.rrs.dl_gbr_prb_usage;
        p_cell_ctxt->serv_cell_load_info.rrs.ul_gbr_prb_usage = load_report->serv_cell_load_info.rrs.ul_gbr_prb_usage;
        p_cell_ctxt->serv_cell_load_info.rrs.dl_non_gbr_prb_usage =
            load_report->serv_cell_load_info.rrs.dl_non_gbr_prb_usage;
        p_cell_ctxt->serv_cell_load_info.rrs.ul_non_gbr_prb_usage =
            load_report->serv_cell_load_info.rrs.ul_non_gbr_prb_usage;
        p_cell_ctxt->serv_cell_load_info.rrs.dl_total_prb_usage =
            load_report->serv_cell_load_info.rrs.dl_total_prb_usage;
        p_cell_ctxt->serv_cell_load_info.rrs.ul_total_prb_usage =
            load_report->serv_cell_load_info.rrs.ul_total_prb_usage;
    }
    /* bug_13885_start */
    if (load_report->serv_cell_load_info.bitmask & RRM_SON_ABS_STATUS_PRESENT)
    {
        p_cell_ctxt->serv_cell_load_info.abs_status.dl_abs_status =
            load_report->serv_cell_load_info.abs_status.dl_abs_status;
    }
    /* bug_13885_end */

    /*save serving composite load*/
    if (load_report->serv_cell_load_info.bitmask & RRM_SON_COMP_AVL_CAP_GRP_PRESENT)
    {
        /* SPR 19015 Fix Start */
        p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask = SON_NULL;
        p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.bitmask = SON_NULL;
        if (load_report->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask & RRM_SON_CELL_CAP_CLASS_VAL_PRESENT)
        {
        p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val = 
            load_report->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cell_cap_class_val;
            p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.bitmask = RRM_SON_CELL_CAP_CLASS_VAL_PRESENT;
        } 
        /* SPR 19015 Fix Stop */
        p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val = 
            load_report->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val;
        /* SPR 19015 Fix Start */
        if (p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.bitmask & RRM_SON_CELL_CAP_CLASS_VAL_PRESENT)
        {
        p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.cell_cap_class_val = 
            load_report->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.cell_cap_class_val;
            p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.bitmask  = RRM_SON_CELL_CAP_CLASS_VAL_PRESENT;
        }
        /* SPR 19015 Fix Stop */
        p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.cap_val = 
            load_report->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.cap_val;
        /* SPR 19015 Fix Start */
        p_cell_ctxt->serv_cell_load_info.bitmask |= RRM_SON_COMP_AVL_CAP_GRP_PRESENT;
        /* SPR 19015 Fix Stop */

        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                "Load report have composite load value "
                "for serving Cell 0x%x,dl cap val = %d",
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                p_cell_ctxt->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val);
    }
    else
    {
        /*serving cell composite load not present.so ignore load report*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                "Load report does not have composite load value "
                "for serving cell.ignoring the msg,Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
        return RELEASE_SON_BUFFER;
    }

    /*storing neighboring cell load info*/
    while (i < load_report->count && load_report->count <= RRM_MAX_NUM_CELLS)
    {
        if (SON_NULL != MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head))
        {
            /*in mobility change req the src_cgi1 will be the serving cell id for us */
            p_son_mlb_neigh_cell_info_record = (son_mlb_neigh_cell_info_record_t *)
                list_find_node(&p_cell_ctxt->neigh_cell_info_head,
                        (son_void_t *)&load_report->neigh_cell_load_info[i].g_cell_id,
                        son_mlb_object_id_keyof,
                        son_compare_cell_id);

            if (p_son_mlb_neigh_cell_info_record != SON_PNULL)
            {
                p_neigh_cell_info = &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data;
                neighbor_cell_found = SON_TRUE;
                /*save nbr load*/
                if (SON_TRUE ==
                        mlb_save_load_report(p_cell_ctxt, load_report->neigh_cell_load_info[i], p_neigh_cell_info))
                {
                    /*call the OCN offset change algo(applicable for connected UEs)*/
                    son_mlb_calc_delta_and_newoffset_in_connected_mod(p_cell_ctxt, p_neigh_cell_info);
                }
                else
                {
                    /*ignore the LOAD Report and don’t update context*/
                    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_INFO,
                            "Load report does not have composite load value "
                            "ignoring the msg,cell id %x,neighbor Cell 0x%x",
                            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), 
                            son_convert_char_cell_id_to_int_cell_id(p_neigh_cell_info->neigh_cell_id.cell_identity));
                }
            }
            else
            {
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_BRIEF, "May be new nbr cell entry come");
            }
        }
        else
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_BRIEF, "New nbr cell entry come");
        }

        /*if the neighbor cell id is not present.create an entry in mlb neighbor context */
        if ((SON_TRUE != neighbor_cell_found) &&
                (MLB_NBR_Q_GET_COUNT(&p_cell_ctxt->neigh_cell_info_head) < MAX_NEIGH_CELL))
        {
            p_son_mlb_neigh_cell_info_record =
                (son_mlb_neigh_cell_info_record_t *)son_mem_get(sizeof(son_mlb_neigh_cell_info_record_t));
            if (p_son_mlb_neigh_cell_info_record == SON_PNULL)
            {
                    /*SPR 17777 +-*/
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                        SON_ERROR, "Memory allocation failed for p_son_mlb_neigh_cell_info_record");
                LTE_GCC_UNUSED_PARAM(evnt_id);
                LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
                return RELEASE_SON_BUFFER;
            }

            /*fetch index for nbr context.It should be one less than the count_for_neigh_cell */
            SON_MEMSET(p_son_mlb_neigh_cell_info_record, 0, sizeof(son_mlb_neigh_cell_info_record_t));
            SON_MEMCPY(&p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.neigh_cell_id,
                    &load_report->neigh_cell_load_info[i].g_cell_id, sizeof(son_intra_rat_global_cell_id_t));
            /*save nbr load*/
            if (SON_TRUE ==
                    mlb_save_load_report(p_cell_ctxt, load_report->neigh_cell_load_info[i],
                        &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data))
            {
                /*call the OCN offset change algo(applicable for connected UEs)*/
                son_mlb_calc_delta_and_newoffset_in_connected_mod(p_cell_ctxt,
                        &p_son_mlb_neigh_cell_info_record->neigh_cell_info_data);

                MLB_NBR_Q_ENQUEUE(&p_cell_ctxt->neigh_cell_info_head, p_son_mlb_neigh_cell_info_record);
                /*increament neighbor count*/
                p_cell_ctxt->count_for_neigh_cell++;

                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                        "Load report received for Cell 0x%x "
                        " neighbor Cell 0x%x",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), 
                        son_convert_char_cell_id_to_int_cell_id(p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.
                            neigh_cell_id.cell_identity));
            }
            else
            {
                /*ignore the LOAD Report and don’t update context*/
                SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                        "Discard Load report "
                        "cell id 0x%x,neighbor cell id 0x%x",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), 
                        son_convert_char_cell_id_to_int_cell_id(p_son_mlb_neigh_cell_info_record->neigh_cell_info_data.
                            neigh_cell_id.cell_identity));
                /* SPR 15935 Fix Start */
                son_mem_free(p_son_mlb_neigh_cell_info_record);
                p_son_mlb_neigh_cell_info_record = SON_PNULL;
                /* SPR 15935 Fix Stop */
            }
        }

        /*increament count in load report*/
        i++;
        neighbor_cell_found = SON_FALSE;
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_load_report_handler */
