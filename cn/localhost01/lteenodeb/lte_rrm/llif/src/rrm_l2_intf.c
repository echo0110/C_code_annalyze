/******************************************************************************
*
*   FILE NAME:
*       rrm_l2_intf.c
*    
*   DESCRIPTION:
		This file will be used for handle the L2 messages to RRM.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   23 March 2012   gur26991    ---------       Initial
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#include "rrm_defines.h"
#include "rrm_utils.h"
#include "rrm_l2_intf.h"
#include "rrm_cell_context.h"
#include "rrm_events.h"
#include "rrm_mem_utils.h"
#include "rrm_cell_mgr.h"
/****************************************************************************
 * Function Name  : rrm_l2_get_l2_ue_sinr_ta_resp
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function provide ue specific SINR and TA info recvd from mac    
 ****************************************************************************/
rrm_return_et 
rrm_l2_get_l2_ue_snr_report  (
								U8 *p_api_buf
                            /*SPR 17777 +-*/
                             )
{
	/*LTE_RRM_KLOCWORK_WARN_5_JULY_12_START*/
	rrm_l2_snr_report_t *p_l2_snr_report = RRM_PNULL;
	rrm_return_et        ret_val = RRM_SUCCESS;

	RRM_UT_TRACE_ENTER();
	if(RRM_PNULL == p_api_buf)
	{
		RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,"NULL pointer Exception of p_api_buf");
		ret_val = RRM_FAILURE;
	}
	else
	{	
		p_l2_snr_report = (rrm_l2_snr_report_t *)p_api_buf;
		RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,"SNR REPORT : [UE:%d], averageULSINR [%d]\
										 averageDLSNR[%d], timingAdvanceValue[%d]",
										 p_l2_snr_report->ue_index,
										 p_l2_snr_report->averageULSINR,
										 p_l2_snr_report->averageDLSNR,
										 p_l2_snr_report->timingAdvanceValue);
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
	/*LTE_RRM_KLOCWORK_WARN_5_JULY_12_END*/
}

/****************************************************************************
 * Function Name  : rrm_mac_ue_periodic_stats_report
 * Inputs         : p_api_buf, data_len, p_api_ue_data
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function extract the Ues mac reports 
 ****************************************************************************/
rrm_return_et
rrm_mac_ue_periodic_stats_report(U8 *p_api_buf,
                /*SPR 17777 +-*/
				 rrm_ue_mac_stats_report_t **p_api_ue_data)
{
    rrm_return_et        ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    rrm_ue_mac_stats_report_t *p_ue_stats = RRM_PNULL;
    U8                        *p_incoming_api_start = RRM_PNULL;

    p_incoming_api_start = (U8 *)p_api_buf;

    p_ue_stats = (rrm_ue_mac_stats_report_t *)rrm_mem_get(sizeof(rrm_ue_mac_stats_report_t));
    /*coverity 19427, rel2.0.1, aditya*/
    if(RRM_PNULL != p_ue_stats)
    {
        p_ue_stats->ue_count = *((U32 *)p_incoming_api_start);
        p_incoming_api_start += RRM_FOUR;

        /* SPR: 15922 Start */
        if(p_ue_stats->ue_count == RRM_ZERO)
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "Received Zero number of UEs");
            ret_val = RRM_FAILURE;
            RRM_MEM_FREE(p_ue_stats);

            RRM_UT_TRACE_EXIT();
            return ret_val;
        }
        /* SPR: 15922 End */

        /* + SPR 20078 */
        p_ue_stats->ue_stats = (PeriodicReportInfo *)rrm_mem_get((p_ue_stats->ue_count) *  sizeof(PeriodicReportInfo));
        /* - SPR 20078 */

        /* SPR: 15922 Start */
        if(RRM_PNULL != p_ue_stats->ue_stats)
            /* SPR: 15922 End */
        {
            /*extract the data in to provided data structure*/
            RRM_MEMCPY(p_ue_stats->ue_stats,
                    p_incoming_api_start,
                    /* + SPR 20078 */
                    sizeof(PeriodicReportInfo) * p_ue_stats->ue_count);
                    /* - SPR 20078 */

            *p_api_ue_data = p_ue_stats;
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED, \
                    "Parsing of UE Stats is successful for periodic ue stats report");
        }
        else
        {
            p_api_ue_data = RRM_PNULL;
            /* SPR: 15922 Start */
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,"Memory allocation to p_ue_stats->ue_stats failed");
            ret_val = RRM_FAILURE;
            RRM_MEM_FREE(p_ue_stats);
            /* SPR: 15922 End */
        }

    }
    else
    {
        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_ERROR,
                "Memory allocation to p_ue_stats failed");
        ret_val = RRM_FAILURE;
    }


    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/* Carrier Aggregation start*/
/*klock_works_changes_start*/
rrm_void_t
rrm_fill_mac_scheduler_reconfig_params
(
 CAParams        *p_mac_scheduler_ca_param,
 operator_info_t *operator_info
)
{
    RRM_UT_TRACE_ENTER();

    p_mac_scheduler_ca_param->bitMask = 0x00 ;

    if(operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_LOAD_CALCULATION_WINDOW_SIZE_PRESENT )
    {
        p_mac_scheduler_ca_param->loadCalcualtionWindowSize = 
            operator_info->ca_config.ca_mac_scheduler_params.load_calcualtion_window_size;

        p_mac_scheduler_ca_param->bitMask |= LOAD_CAL_WINDOW_SIZE;

        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : load_calcualtion_window_size [%d]",
                p_mac_scheduler_ca_param->loadCalcualtionWindowSize );
    }
    else
    {

        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : load_calcualtion_window_size is not present");
    }

    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_LOAD_DISPARITY_THRESHOLD_PRESENT )
    {
        p_mac_scheduler_ca_param->loadDisparityThreshold = 
            operator_info->ca_config.ca_mac_scheduler_params.load_disparity_threshold;

        p_mac_scheduler_ca_param->bitMask |= LOAD_DISPARITY_THRESHOLD ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : loadDisparityThreshold [%d]",
                p_mac_scheduler_ca_param->loadDisparityThreshold );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : load_disparity_threshold is not present");
    }

    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_MCS_DIFF_THRESHOLD_PRESENT )
    {
        p_mac_scheduler_ca_param->mcsDiffThreshold = 
            operator_info->ca_config.ca_mac_scheduler_params.mcs_diff_threshold ;

        p_mac_scheduler_ca_param->bitMask |= MCS_DIFF_THRESHOLD ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : mcsDiffThreshold [%d]",
                p_mac_scheduler_ca_param->mcsDiffThreshold );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : mcs_diff_threshold is not present");
    }
    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_PRB_USAGE_WATER_MARK_PRESENT )
    {
        p_mac_scheduler_ca_param->prbUsageWaterMark =
            operator_info->ca_config.ca_mac_scheduler_params.prb_usage_water_mark ;

        p_mac_scheduler_ca_param->bitMask |= PRB_USAGE_WATER_MARK ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : prbUsageWaterMark [%d]",
                p_mac_scheduler_ca_param->prbUsageWaterMark );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : prb_usage_water_mark is not present");
    }

    RRM_UT_TRACE_EXIT();
}
rrm_void_t
rrm_fill_remaining_mac_scheduler_reconfig_params
(
 CAParams   *p_mac_scheduler_ca_param,
 operator_info_t                         *operator_info
)
{
    RRM_UT_TRACE_ENTER();

    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_DL_DEACT_COMMAND_THRESH_PRESENT )
    {
        p_mac_scheduler_ca_param->dlDeactCommandThresh  =
            operator_info->ca_config.ca_mac_scheduler_params.dl_deact_command_thresh ;

        p_mac_scheduler_ca_param->bitMask |= DL_DEACT_COMMON_THREAD ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : dlDeactCommandThresh [%d]",
                p_mac_scheduler_ca_param->dlDeactCommandThresh );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : dl_deact_command_thresh is not present");
    }
    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_DL_DEACT_MCS_LOW_THRESH_PRESENT )
    {
        p_mac_scheduler_ca_param->dlDeactMCSLowThresh =
            operator_info->ca_config.ca_mac_scheduler_params.dl_deact_mcs_low_thresh ;

        p_mac_scheduler_ca_param->bitMask |= DL_DEACT_MCS_LOW_THRESH ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : dlDeactMCSLowThresh [%d]",
                p_mac_scheduler_ca_param->dlDeactMCSLowThresh );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : dl_deact_mcs_low_thresh is not present");
    }
    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_DL_Q_LOAD_UP_THRESH_PRESENT )
    {
        p_mac_scheduler_ca_param->dlQLoadUpThresh =
            operator_info->ca_config.ca_mac_scheduler_params.dl_qload_up_threshold ;

        p_mac_scheduler_ca_param->bitMask |= DL_QLOAD_UP_THRESH ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : dlQLoadUpThresh [%d]",
                p_mac_scheduler_ca_param->dlQLoadUpThresh );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : dl_qload_up_threshold is not present");
    }
    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_DL_Q_LOAD_LOW_THRESH_PRESENT )
    {
        p_mac_scheduler_ca_param->dlQLoadLowThresh =
            operator_info->ca_config.ca_mac_scheduler_params.dl_qload_low_threshold ;

        p_mac_scheduler_ca_param->bitMask |= DL_QLOAD_DOWN_THRESH ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : dlQLoadLowThresh [%d]",
                p_mac_scheduler_ca_param->dlQLoadLowThresh );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : dl_qload_low_threshold is not present");
    }
    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_ACT_DEACT_TRIG_COUNT_THRESH_PRESENT )
    {
        p_mac_scheduler_ca_param->actDeactTriggerCountThresh =
            operator_info->ca_config.ca_mac_scheduler_params.act_deact_trigger_count_threshold ;

        p_mac_scheduler_ca_param->bitMask |= ACT_DEACT_TIG_COUNT_THRESH ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : actDeactTriggerCountThresh [%d]",
                p_mac_scheduler_ca_param->actDeactTriggerCountThresh );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : act_deact_trigger_count_threshold is not present");
    }
    /* SPR 16422 START */
    if( operator_info->ca_config.ca_mac_scheduler_params.bitmask &
            RRM_CQI_VALIDITY_TIMER_PRESENT )
    {
        p_mac_scheduler_ca_param->cqiValidityTimer =
            operator_info->ca_config.ca_mac_scheduler_params.cqi_validity_timer ;

        p_mac_scheduler_ca_param->bitMask |= CQI_VALIDITY_TIMER ; 
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_RECONFIG : cqi_validity_timer [%d]",
                p_mac_scheduler_ca_param->cqiValidityTimer );
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "RRM_SCHEDULER_CA_RECONFIG : cqi_validity_timer is not present");
    }   
    /* SPR 16422 END */
    RRM_UT_TRACE_EXIT();
}

/*klock_works_changes_end*/
/****************************************************************************
 * Function Name  : rrm_fill_carrier_agg_mac_scheduler_reconfig_param 
 * Inputs         : p_mac_scheduler_ca_param,operator_info
 * Outputs        : none
 * Returns        : rrm_void_t 
 * Description    : This function extract the Ues mac reports 
 ****************************************************************************/

rrm_void_t
rrm_fill_carrier_agg_mac_scheduler_reconfig_param
( 
 CAParams         *p_mac_scheduler_ca_param,
 operator_info_t  *operator_info
)
{

    RRM_UT_TRACE_ENTER();

    if (RRM_NULL != p_mac_scheduler_ca_param)
    {
        /*klock_works_changes_start*/

        rrm_fill_mac_scheduler_reconfig_params(p_mac_scheduler_ca_param, operator_info); 

        rrm_fill_remaining_mac_scheduler_reconfig_params(p_mac_scheduler_ca_param, operator_info);

        /*klock_works_changes_end*/ 
    } 
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_start*/
/****************************************************************************
 * Function Name  : rrm_send_mac_scheduler_ca_info 
 * Inputs         : p_cell_context, operator_info
 * Outputs        : none
 * Returns        : rrm_void_t
 * Description    : This function extract the Ues mac reports
 ****************************************************************************/
rrm_void_t
rrm_send_mac_scheduler_ca_info(operator_info_t              *operator_info ,
                               RrmMacReconfigSchedulerReq   *p_out_reconf_schedule_msg
        )
{
    RRM_UT_TRACE_ENTER();
    if( operator_info->bitmask & RRMCM_RMIF_CA_CONFIG_INFO_PRESENT )
    {
        if( operator_info->ca_config.bitmask & 
                RRM_CA_MAC_SCHEDULER_PARAMS_INFO_PRESENT )
        {
            p_out_reconf_schedule_msg->schedulerParamBitMap |= CA_PARAMS_BIT;

            rrm_fill_carrier_agg_mac_scheduler_reconfig_param( &(p_out_reconf_schedule_msg->caParams),
                    operator_info );
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "MAC_SCHEDULER RECONFIG configuration is not presnt");
        }
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "No CA configuration in RRM_SCHEDULER_CA_RECONFIG param is not present");
    }
    RRM_UT_TRACE_EXIT();
}
 /*Klockwork_fix_end*/
/****************************************************************************
 * Function Name  : rrm_send_mac_scheduler_ca_agg_config_param  
 * Inputs         : p_cell_context, operator_info
 * Outputs        : none
 * Returns        : rrm_void_t 
 * Description    : This function extract the Ues mac reports 
 ****************************************************************************/
rrm_return_et 
rrm_send_mac_scheduler_ca_agg_config_param
( 
    rrm_cell_context_t  *p_cell_ctx,
    operator_info_t     *operator_info
)
{

    RrmMacReconfigSchedulerReq             *p_out_reconf_schedule_msg = RRM_PNULL;
    
	RRM_UT_TRACE_ENTER();

    if (RRM_PNULL == operator_info)
    {
		RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_WARNING,
		    "operator_info is NULL on cell_index = %d",p_cell_ctx->cell_index);

        RRM_UT_TRACE_EXIT();
        /* CID : 61037*/
        return RRM_FAILURE;
     }

    /*  Allocating memory for p_out_reconf_schedule_msg*/
    p_out_reconf_schedule_msg = rrm_mem_get(sizeof(RrmMacReconfigSchedulerReq));
    
	if (RRM_PNULL != p_out_reconf_schedule_msg)
    {
    
	RRM_MEMSET(p_out_reconf_schedule_msg, RRM_ZERO, sizeof(RrmMacReconfigSchedulerReq));
    
    p_out_reconf_schedule_msg->schedulerParamBitMap = RRM_ZERO;
    /*Klockwork_fix_start*/ 
    rrm_send_mac_scheduler_ca_info(operator_info ,p_out_reconf_schedule_msg);
    /*Klockwork_fix_end*/

       if (RRM_ZERO != p_out_reconf_schedule_msg->schedulerParamBitMap)
       {
	   l2_reconfig_scheduler_process_msg(p_out_reconf_schedule_msg,
		   RRM_MODULE_ID,
		   p_cell_ctx->ongoing_trans_id,
		   p_cell_ctx->cell_index
		   ); 
       }
       else
       {
	    RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
		       "No Bitmask is set for l2_reconfig_scheduler_process_msg");

       }

       RRM_MEM_FREE(p_out_reconf_schedule_msg);
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                   "Memory allocation failed for p_out_reconf_schedule_msg");
    }

    RRM_UT_TRACE_EXIT();
    return RRM_SUCCESS;
}
/* Carrier Aggregation end */
 /*Cov_fix_start_65033*/
/*Klockwork_fix_2431_start*/
/****************************************************************************
 * Function Name  : rrm_l2_build_rrmcm_rmif_dynamic_icic_info
 * Inputs         : rrm_cell_context_t *p_cell_ctx  : cell context
 * Outputs        : None
 * Returns        : None 
 * Description    : send reconfig schedule param
 ****************************************************************************/
rrm_void_t
rrm_l2_build_rrmcm_rmif_dynamic_icic_info(rrm_cell_context_t           *p_cell_ctx,
                                          RrmMacReconfigSchedulerReq   *p_out_reconf_schedule_msg,                                                                  
                                          rrm_dynamic_icic_info_t      *dynamic_icic_info)
{ 
    U8 					       count = RRM_ZERO;
    U8                         count2 = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    if(RRMCM_RMIF_DYNAMIC_ICIC_INFO_PRESENT & (p_cell_ctx->operator_info).bitmask)
    {
        if(RRMCM_RMIF_TPC_FOR_MSG3_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->tpcForMsg3 = dynamic_icic_info->tpc_for_msg3 ;
            p_out_reconf_schedule_msg->schedulerParamBitMap |= TPC_FOR_MSG3_BIT;

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_TPC_FOR_MSG3_BIT present, tpcForMsg3 = %d", p_out_reconf_schedule_msg->tpcForMsg3 );
        }

        /* setting srsSubBandGroupSize  */                                 
        if(RRMCM_RMIF_SRS_SUBBAND_GRP_SIZE_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->srsSubBandGroupSize = dynamic_icic_info->srs_subband_group_size ;
            p_out_reconf_schedule_msg->schedulerParamBitMap |= SRS_SUBBAND_GRP_SIZE_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_SRS_SUBBAND_GRP_SIZE_BIT present, srsSubBandGroupSize = %d",p_out_reconf_schedule_msg->srsSubBandGroupSize);
        }

        /* setting tpcTriggerWaitTimeForPUSCHDCI0   */
        if(RRMCM_RMIF_TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->tpcTriggerWaitTimeForPUSCHDCI0 = 
                dynamic_icic_info->tpc_trigger_waittime_for_pusch_DCI0 ;
            p_out_reconf_schedule_msg->schedulerParamBitMap |= TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_BIT present, tpcTriggerWaitTimeForPUSCHDCI0 = %d", p_out_reconf_schedule_msg->tpcTriggerWaitTimeForPUSCHDCI0);
        }

        /* settting tpcTriggerWaitTimeForPUCCHDCI1x2x   */ 
        if(RRMCM_RMIF_TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->tpcTriggerWaitTimeForPUCCHDCI1x2x = 
                dynamic_icic_info->tpc_trigger_waittime_for_pucch_DCI1x2x ;
            p_out_reconf_schedule_msg->schedulerParamBitMap |= TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_BIT present, tpcTriggerWaitTimeForPUCCHDCI1x2x = %d", p_out_reconf_schedule_msg->tpcTriggerWaitTimeForPUCCHDCI1x2x);
        }

        /*  setting tpcNumUeThresholdForPUSCHDCI3A  */
        if(RRMCM_RMIF_TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->tpcNumUeThresholdForPUSCHDCI3A= 
                dynamic_icic_info->tpc_num_ue_threshold_for_pusch_DCI3A ;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A present, tpcNumUeThresholdForPUSCHDCI3A =%d", p_out_reconf_schedule_msg->tpcNumUeThresholdForPUSCHDCI3A);
        }

        /*  setting tpcNumUeThresholdForPUCCHDCI3A  */
        if(RRMCM_RMIF_TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->tpcNumUeThresholdForPUCCHDCI3A = 
                dynamic_icic_info->tpc_num_ue_threshold_for_pucch_DCI3A ;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A present,  tpcNumUeThresholdForPUCCHDCI3A = %d", p_out_reconf_schedule_msg->tpcNumUeThresholdForPUCCHDCI3A);
        }

        /*  setting coolOffPeriodForBLERTriggeredPUCCHTPC   */
        if(RRMCM_RMIF_COOL_OFF_PRD_FOR_BLER_BIT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->coolOffPeriodForBLERTriggeredPUCCHTPC = 
                dynamic_icic_info->cooloff_period_for_bler_triggered_pucchTPC ;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= COOL_OFF_PRD_FOR_BLER_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_COOL_OFF_PRD_FOR_BLER_BIT present, coolOffPeriodForBLERTriggeredPUCCHTPC = %d", p_out_reconf_schedule_msg->coolOffPeriodForBLERTriggeredPUCCHTPC);
        }

        /* setting coolOffPeriodForPUCCHTPC */
        if(RRMCM_RMIF_COOL_OFF_PERIOD_FOR_PUCCHTPC_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->coolOffPeriodForPUCCHTPC = 
                dynamic_icic_info->cooloff_period_for_pucchTPC ;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= COOL_OFF_PERIOD_FOR_PUCCH_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_COOL_OFF_PERIOD_FOR_PUCCH_BIT present, coolOffPeriodForPUCCHTPC = %d", p_out_reconf_schedule_msg->coolOffPeriodForPUCCHTPC);
        }

        /* setting coolOffPeriodForPUSCHTPC */
        if(RRMCM_RMIF_COOL_OFF_PERIOD_PUSCHTPC_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->coolOffPeriodForPUSCHTPC = 
                dynamic_icic_info->cooloff_period_for_puschTPC ;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= COOL_OFF_PERIOD_PUSCH_BIT ; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_COOL_OFF_PERIOD_PUSCH_BIT present, coolOffPeriodForPUSCHTPC = %d", p_out_reconf_schedule_msg->coolOffPeriodForPUSCHTPC);
        }

        /* setting alphaBasedPathLossToTargetSINRMap */
        if(RRMCM_RMIF_ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_PRESENT &  dynamic_icic_info->bitmask)
        {

            rrm_memcpy_alpha_pathloss_target_sinr_map(
                    &(p_out_reconf_schedule_msg->alphaBasedPathLossToTargetSINRMap),
                    &(dynamic_icic_info->alpha_pathloss_target_sinr_map));
            p_out_reconf_schedule_msg->schedulerParamBitMap |= ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_BIT; 

        }

        /* setting deltaSINRToTPCMapForPUSCH */
        if(RRMCM_RMIF_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_PRESENT &  dynamic_icic_info->bitmask)
        {
            rrm_memcpy_rrm_delta_sinr_to_tpc_map_pusch(
                    &(p_out_reconf_schedule_msg->deltaSINRToTPCMapForPUSCH),
                    &(dynamic_icic_info->delta_sinr_tpc_map_pusch_info));
            p_out_reconf_schedule_msg->schedulerParamBitMap |= DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_BIT ; 

        }

        /* setting deltaBlerToTPCMapForPUCCH */ 
        if(RRMCM_RMIF_BLER_TO_TPC_MAP_FOR_PUCCH_PRESENT &  dynamic_icic_info->bitmask)
        {
            rrm_memcpy_bler_to_tpc_map_for_pucch(
                    &(p_out_reconf_schedule_msg->deltaBlerToTPCMapForPUCCH),
                    &(dynamic_icic_info->bler_to_tpc_map_for_pucch));
            p_out_reconf_schedule_msg->schedulerParamBitMap |= BLER_TO_TPC_MAP_FOR_PUCCH; 

        }

        /* setting DeltaSINRToTPCMapForPUCCH*/
        if(RRMCM_RMIF_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_PRESENT &  dynamic_icic_info->bitmask)
        {
            rrm_memcpy_delta_sinr_tpc_map_pucch_info(       
                    &(p_out_reconf_schedule_msg->deltaSINRToTPCMapForPUCCH),
                    &(dynamic_icic_info->delta_sinr_tpc_map_pucch_info));
            /* 
               RRM_MEMCPY(&(p_out_reconf_schedule_msg->deltaSINRToTPCMapForPUCCH),
               &(dynamic_icic_info->delta_sinr_tpc_map_pucch_info),
               sizeof(DeltaSINRToTPCMapForPUSCH));
             */
            p_out_reconf_schedule_msg->schedulerParamBitMap |= DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_BIT; 

        }

        /* pdcchAggregationAndPowerOffset */ 
        if(RRMCM_RMIF_PDCCH_AGG_AND_POW_OFFSET_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->pdcchAggregationAndPowerOffset.bitmask |= RRM_ZERO;

            for(count = RRM_ZERO; count < RRM_OAM_NUM_DCI_SIZE_CATEGORY; count++)
            {
                for(count2 = RRM_ZERO; count2 < RRM_OAM_MAX_CQI_VALUE; count2++)
                {
                    rrm_memcpy_rrm_aggregation_power_offset(
                            &(p_out_reconf_schedule_msg->pdcchAggregationAndPowerOffset).
                            aggregationPowerOffsetCCUser[count][count2],
                            &(dynamic_icic_info->pdcch_aggregation_power_offset).aggregation_power_offset_cc_user[count].
                            aggregation_power_offset_user[count2]);
                    rrm_memcpy_rrm_aggregation_power_offset(
                            &(p_out_reconf_schedule_msg->pdcchAggregationAndPowerOffset).
                            aggregationPowerOffsetCEUser[count][count2],
                            &(dynamic_icic_info->pdcch_aggregation_power_offset).aggregation_power_offset_ce_user[count].
                            aggregation_power_offset_user[count2]);

                    /* SPR 21251 +- */

                    /*                                      

                                                            RRM_MEMCPY(&(p_out_reconf_schedule_msg->pdcchAggregationAndPowerOffset).aggregationPowerOffsetCCUser[count][count2],
                                                            &(dynamic_icic_info->pdcch_aggregation_power_offset).aggregation_power_offset_cc_user[count].
                                                            aggregation_power_offset_user[count2],
                                                            (sizeof(rrm_aggregation_power_offset_t)));

                                                            RRM_MEMCPY(&(p_out_reconf_schedule_msg->pdcchAggregationAndPowerOffset).aggregationPowerOffsetCEUser[count][count2],
                                                            &(dynamic_icic_info->pdcch_aggregation_power_offset).aggregation_power_offset_ce_user[count].
                                                            aggregation_power_offset_user[count2],
                                                            (sizeof(rrm_aggregation_power_offset_t)));
                     */
                }
            }
            p_out_reconf_schedule_msg->schedulerParamBitMap |= PDCCH_AGG_AND_POW_OFF_BIT; 

        }

        /* cqiToPHICHPower */
        if(RRMCM_RMIF_CQI_TO_PHICH_POW_PRESENT &  dynamic_icic_info->bitmask)
        {
            rrm_memcpy_rrm_l2_cqi_to_phich_power(
                    &(p_out_reconf_schedule_msg->cqiToPHICHPower),
                    &(dynamic_icic_info->cqi_to_phich_power));
            /*                                    
                                                  RRM_MEMCPY(&(p_out_reconf_schedule_msg->cqiToPHICHPower),
                                                  &(dynamic_icic_info->cqi_to_phich_power),
                                                  sizeof(CqiToPHICHPower));
             */
            p_out_reconf_schedule_msg->schedulerParamBitMap |= CQI_TO_PHICH_POW_BIT; 

        }

        /* minRbForPlPhrCalc */
        if(RRMCM_RMIF_MIN_RB_FOR_PL_PHR_CALC_BIT_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->minRbForPlPhrCalc =
                dynamic_icic_info->min_rb_for_pl_phr_calc;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= MIN_RB_FOR_PL_PHR_CALC_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_MIN_RB_FOR_PL_PHR_CALC_BIT present, minRbForPlPhrCalc = %d",p_out_reconf_schedule_msg->minRbForPlPhrCalc);
        }
        if( RRM_TRUE == p_cell_ctx->sps_on)
        {
            /* spsDlSchInfoPerTti */
            if(RRMCM_RMIF_SPS_DL_SCHEDULING_INFO_PER_TTI_PRESENT &  dynamic_icic_info->bitmask)
            {
                rrm_memcpy_rrm_sps_dl_scheduling_Info_per_tti(
                        &(p_out_reconf_schedule_msg->spsDlSchInfoPerTti),
                        &(dynamic_icic_info->sps_dl_scheduling_Info_per_tti));
                /* 
                   RRM_MEMCPY(&(p_out_reconf_schedule_msg->spsDlSchInfoPerTti),
                   &(dynamic_icic_info->sps_dl_scheduling_Info_per_tti),
                   sizeof(SpsDlSchedulingInfoPerTti));
                 */ 
                p_out_reconf_schedule_msg->schedulerParamBitMap |= SPS_DL_SCHEDULING_INFO_PER_TTI_BIT; 


            }

            /* spsUlSchInfoPerTti */

            if(RRMCM_RMIF_SPS_UL_SCHEDULING_INFO_PER_TTI_PRESENT &  dynamic_icic_info->bitmask)
            {
                rrm_memcpy_rrm_sps_ul_scheduling_Info_per_tti(
                        &(p_out_reconf_schedule_msg->spsUlSchInfoPerTti),
                        &(dynamic_icic_info->sps_ul_scheduling_Info_per_tti));

                RRM_MEMCPY(&(p_out_reconf_schedule_msg->spsUlSchInfoPerTti),
                        &(dynamic_icic_info->sps_ul_scheduling_Info_per_tti),
                        sizeof(SpsUlSchedulingInfoPerTti));

                p_out_reconf_schedule_msg->schedulerParamBitMap |= SPS_UL_SCHEDULING_INFO_PER_TTI_BIT; 
            }
            /* alphaBasedPathLossToTargetSINRMapSps */

            if(RRMCM_RMIF_ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_PRESENT & dynamic_icic_info->bitmask)
            {
                rrm_memcpy_alpha_pathloss_target_sinr_map(
                        &(p_out_reconf_schedule_msg->alphaBasedPathLossToTargetSINRMapSps),
                        &(dynamic_icic_info->alpha_pathloss_target_sinr_map_sps));
                /*                                
                                                  RRM_MEMCPY(&(p_out_reconf_schedule_msg->alphaBasedPathLossToTargetSINRMapSps),
                                                  &(dynamic_icic_info->alpha_pathloss_target_sinr_map_sps),
                                                  sizeof(AlphaBasedPathLossToTargetSINRMap));
                 */
                p_out_reconf_schedule_msg->schedulerParamBitMap |= ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_BIT; 

            }
        }
        /* hdReducedMcsTable */
        if(RRMCM_RMIF_HD_REDUCED_MCS_TABLE_PRESENT &  dynamic_icic_info->bitmask)
        {
            for(count = RRM_ZERO; count < MAX_MCS_VALUES; count++)
            {
                p_out_reconf_schedule_msg->hdReducedMcsTable[count] =   
                    dynamic_icic_info->hd_reduced_mcs_table[count];

                RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "RRM_L2_HD_REDUCED_MCS_TABLE_BIT present,  p_out_reconf_schedule_msg->hdReducedMcsTable[%d] = %d",count, p_out_reconf_schedule_msg->hdReducedMcsTable[count]);
            }
            /*
               RRM_MEMCPY(&(p_out_reconf_schedule_msg->hdReducedMcsTable),
               &(dynamic_icic_info->hd_reduced_mcs_table),
               (sizeof(U8) * MAX_MCS_VALUES));
             */                    

            p_out_reconf_schedule_msg->schedulerParamBitMap |= HD_REDUCED_MCS_TABLE_BIT; 

        }
        /* dynamicCFIExtensionParams */
        if(RRMCM_RMIF_DYNAMIC_CFI_EXTENSION_PARAM_PRESENT &  dynamic_icic_info->bitmask)
        {
            rrm_memcpy_rrm_dynamic_cfi_extension_params(
                    &(p_out_reconf_schedule_msg->dynamicCFIExtensionParams),
                    &(dynamic_icic_info->dynamic_cfi_extension_params));
            /*                            
                                          RRM_MEMCPY(&(p_out_reconf_schedule_msg->dynamicCFIExtensionParams),
                                          &(dynamic_icic_info->dynamic_cfi_extension_params),
                                          sizeof(DynamicCFIExtensionParams));
             */
            p_out_reconf_schedule_msg->schedulerParamBitMap |= DYN_PDCCH_EXTENSION_PARAMS_BIT;    //need to check 
        }

        /* atbConfig */
        if(RRMCM_RMIF_ATB_CONFIG_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->atbConfig.minMCSIndexForATB = 
                (U32)dynamic_icic_info->atb_config.min_mcs_index_for_atb;

            p_out_reconf_schedule_msg->atbConfig.minPRBValForATB = 
                (U32)dynamic_icic_info->atb_config.min_prb_val_for_atb;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= ATB_CONFIG_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_ATB_CONFIG_BIT present, atbConfig.minMCSIndexForATB = %d", p_out_reconf_schedule_msg->atbConfig.minMCSIndexForATB);

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_ATB_CONFIG_BIT present,atbConfig.minPRBValForATB = %d", p_out_reconf_schedule_msg->atbConfig.minPRBValForATB);




        }

        /* ulMUMIMOType */
        if(RRMCM_RMIF_UL_MU_MIMO_TYPE_PRESENT &  dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->ulMUMIMOType = 
                dynamic_icic_info->ul_mu_mimo_type;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= UPLINK_MU_MIMO_TYPE_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "RRM_L2_UPLINK_MU_MIMO_TYPE_BIT present, ulMUMIMOType = %d",p_out_reconf_schedule_msg->ulMUMIMOType);

        }

        /*  mcsThresholdULMUMIMO    */
        if(RRMCM_RMIF_MSC_THRESHOLD_UL_MU_MIMO_PRESENT & dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->mcsThresholdULMUMIMO = 
                dynamic_icic_info->msc_threshold_ul_mu_mimo;

            p_out_reconf_schedule_msg->schedulerParamBitMap |= UPLINK_MU_MIMO_MCS_THRESHOLD_BIT; 

            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "UPLINK_MU_MIMO_MCS_THRESHOLD_BIT present, mcsThresholdULMUMIMO = %d",  p_out_reconf_schedule_msg->mcsThresholdULMUMIMO);
        }
        if(RRMCM_RMIF_PA_QPSK_INFO_PRESENT & dynamic_icic_info->bitmask)
        {
            p_out_reconf_schedule_msg->paQpsk = dynamic_icic_info->pa_qpsk;
            p_out_reconf_schedule_msg->schedulerParamBitMap |= PA_QPSK_BIT; 

        }
    }
    RRM_UT_TRACE_EXIT();
}
/*Cov_fix_end_65033*/
/****************************************************************************
 * Function Name  : rrm_l2_build_rrmcm_rmif_antenna_port
 * Inputs         : rrm_cell_context_t *p_cell_ctx  : cell context
 * Outputs        : None
 * Returns        : None
 * Description    : send reconfig schedule param
 ****************************************************************************/
rrm_void_t
rrm_l2_build_rrmcm_rmif_antenna_port(rrm_cell_context_t             *p_cell_ctx,
                                     RrmMacReconfigSchedulerReq     *p_out_reconf_schedule_msg , 
                                     rrm_mac_beam_forming_info_t    *p_rrm_mac_beam_forming_info)
{
    U8                                         port = RRM_ZERO;
    U32                                        index = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    if( RRM_SUCCESS == rrm_cellm_get_antenna_port( p_cell_ctx->cell_index,&port))
    {
        if( ( ANTENNA_PORTS_COUNT_AN2 == port ) || ( ANTENNA_PORTS_COUNT_AN4 == port ) )
        { 
            if(p_rrm_mac_beam_forming_info->bitmask & RRM_AOA_TO_BEAM_FRMNG_VECTOR_MPPNG_PRESENT)
            {
                p_out_reconf_schedule_msg->schedulerParamBitMap |= AOA_TO_BF_VECTOR_BIT; 
                for(index = RRM_ZERO; index < MAX_NUM_BF_VECTORS_TO_AOA_VAL ; index ++)
                {
                    p_out_reconf_schedule_msg->angleOfArrivalToBeamFormingVectorMapping[index] =
                        p_rrm_mac_beam_forming_info->rrm_aoa_to_beam_frmng_vctr_mppng[index];


                    RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                            "ATOA vector [%d]",p_out_reconf_schedule_msg->angleOfArrivalToBeamFormingVectorMapping[index] );
                }
            }
            else
            {
                RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "ATOA Beamforming vector is not present");
            }


            if(p_rrm_mac_beam_forming_info->bitmask & RRM_SINR_TO_BEAM_FRMNG_CONFIG_PRESENT)
            {
                p_out_reconf_schedule_msg->schedulerParamBitMap |= SINR_TO_BF_VECTOR_BIT;

                for(index = RRM_ZERO; index < MAX_NUM_BF_VECTORS_TO_SINR_VAL ; index ++)
                {
                    p_out_reconf_schedule_msg->sinrToBeamFormingVectorMapping[index].beamFormingVectorForA1 =
                        p_rrm_mac_beam_forming_info->sinr_to_beam_forming_config_t[index].beam_frmng_vector_for_A1;


                    RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                            "SINR verctor [%d]", p_out_reconf_schedule_msg->sinrToBeamFormingVectorMapping[index].beamFormingVectorForA1);
                }
            }
            else
            {
                RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "SINR Beamforming vectoe is not present");
            }
        }
    }
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_2431_end*/
/****************************************************************************
 * Function Name  : rrm_l2_build_send_reconfig_schedule_params
 * Inputs         : rrm_cell_context_t *p_cell_ctx  : cell context 
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : send reconfig schedule param
 ****************************************************************************/

rrm_void_t
rrm_l2_build_send_reconfig_schedule_params
(
    rrm_cell_context_t *p_cell_ctx                            
)
{
    RrmMacReconfigSchedulerReq             *p_out_reconf_schedule_msg = RRM_PNULL;
    rrm_dynamic_icic_info_t                   *dynamic_icic_info = RRM_PNULL;
    /*TM mode 7/8 start*/
    rrm_mac_beam_forming_info_t               *p_rrm_mac_beam_forming_info = RRM_NULL;
    /*TM mode 7/8 end*/

    /*  Allocating memory for rrmcm_rmif_reconf_scheduler_parameters_t */
    p_out_reconf_schedule_msg = rrm_mem_get(sizeof(RrmMacReconfigSchedulerReq));

    RRM_UT_TRACE_ENTER();

    if (RRM_PNULL == p_out_reconf_schedule_msg)
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                "Memory allocation failed for p_out_reconf_schedule_msg");
        RRM_UT_TRACE_EXIT();
        return;
    }
    RRM_MEMSET(p_out_reconf_schedule_msg, RRM_ZERO, sizeof(RrmMacReconfigSchedulerReq));
    p_out_reconf_schedule_msg->schedulerParamBitMap = RRM_ZERO;

    /* Assigning value to rrm_dynamic_icic_info_t */
    /* BUG 9787 Fix Start */
    if(RRMCM_RMIF_DYNAMIC_ICIC_INFO_PRESENT & (p_cell_ctx->operator_info).bitmask) 
    {
        dynamic_icic_info = &((p_cell_ctx->operator_info).dynamic_icic_info);
    }
    /* BUG 9787 Fix End */

    /*getting beamforming info*/
    p_rrm_mac_beam_forming_info =&(p_cell_ctx->operator_info.mac_beam_forming_info);

    /* setting bitmask of RrmMacReconfigSchedulerReq to zero */
    p_out_reconf_schedule_msg->schedulerParamBitMap = RRM_ZERO;

    RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
            "Populating RRM_MAC_RECONFIG_SCHEDULER_REQ");
    /* setting tpcForMsg3   */
    /* BUG 9787 Fix Start */
    /* BUG 9787 Fix End */
    /*Klockwork_fix_2431_start*/
    rrm_l2_build_rrmcm_rmif_dynamic_icic_info(p_cell_ctx, p_out_reconf_schedule_msg,dynamic_icic_info);
    /*Klockwork_fix_2431_end*/
    /* BUG 9787 Fix Start */
    /* BUG 9787 Fix End */
    /*Klockwork_fix_2431_start*/
    rrm_l2_build_rrmcm_rmif_antenna_port(p_cell_ctx,p_out_reconf_schedule_msg,p_rrm_mac_beam_forming_info);
    /*Klockwork_fix_2431_end*/

#ifdef LTE_EMBMS_SUPPORTED
/*SPR 18089:start*/
    if ((MBMS_SIB_13_ON_BROADCAST & 
        p_cell_ctx->rrm_cell_embms_data.current_sibs_on_boradcast)) 
    {
        if (p_cell_ctx->rrm_cell_embms_data.bitmask & 
                RRMCM_RECONFIG_EMBMS_POWER_OFFSET_PRESENT)
        {
            p_out_reconf_schedule_msg->schedulerParamBitMap |= EMBMS_PARAM_BIT;
            p_out_reconf_schedule_msg->eMbmsReservedCellPowerOffset =
                p_cell_ctx->operator_info.mbms_power_info.cell_pwer_offset;
                /*SPR 18468: Fix Start*/
         //   p_cell_ctx->rrm_cell_embms_data.bitmask = RRM_ZERO;
                /*SPR 18468: Fix End*/
        }
        else
        {
            if (RRMCM_RMIF_MBMS_POWER_INFO_PRESENT & p_cell_ctx->operator_info.bitmask)
            {
                p_out_reconf_schedule_msg->
                    schedulerParamBitMap |= EMBMS_PARAM_BIT;
                p_out_reconf_schedule_msg->eMbmsReservedCellPowerOffset =
                    p_cell_ctx->operator_info.mbms_power_info.cell_pwer_offset;
            }
        }
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "cell_pwer_offset [%d] send to MAC",
                p_out_reconf_schedule_msg->eMbmsReservedCellPowerOffset);
    }
/*SPR 18089:end*/
#endif
    if( p_cell_ctx->operator_info.bitmask & RRMCM_RMIF_CA_CONFIG_INFO_PRESENT )
    {
        if( p_cell_ctx->operator_info.ca_config.bitmask & 
                RRM_CA_MAC_SCHEDULER_PARAMS_INFO_PRESENT )
        {
            p_out_reconf_schedule_msg->schedulerParamBitMap |= CA_PARAMS_BIT;

            rrm_fill_carrier_agg_mac_scheduler_reconfig_param( &(p_out_reconf_schedule_msg->caParams),
                    &(p_cell_ctx->operator_info));
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "MAC_SCHEDULER RECONFIG configuration is not presnt");
        }
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "No CA configuration in RRM_SCHEDULER_CA_RECONFIG param is not present");
    }

    if (RRM_ZERO != p_out_reconf_schedule_msg->schedulerParamBitMap)
    {
        l2_reconfig_scheduler_process_msg(p_out_reconf_schedule_msg,
                RRM_MODULE_ID,
                p_cell_ctx->ongoing_trans_id,
                p_cell_ctx->cell_index
                ); 
    }
    else
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "No Bitmask is set for l2_reconfig_scheduler_process_msg");

    }

    RRM_TRACE(g_cellm_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
            "Scheduler Reconfig Bitmap = %d",  p_out_reconf_schedule_msg->schedulerParamBitMap);
    RRM_MEM_FREE(p_out_reconf_schedule_msg);

    RRM_UT_TRACE_EXIT();
}
/** eICIC_PHASE_1_2_CHANGES_START */
/****************************************************************************
 * Function Name  : build_and_send_l2_cell_config_req_for_eicic_reconfig 
 * Inputs         : serving_abs_pattern,usable_abs_pattern,
 *                  cell_index, trans_id
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function sends the RRM_MAC_CELL_CONFIG_REQ to L2
 ****************************************************************************/
rrm_return_et build_and_send_l2_cell_config_req_for_eicic_reconfig
(
   mac_config_data_t            *p_mac_config_data,
   rrm_cell_index_t             cell_index,
   U16                          trans_id
)
{
    rrm_global_context_t        *p_rrm_gbl_ctx  = RRM_PNULL; 
    rrm_return_et 		        ret_val = RRM_FAILURE;
    RrmMacCellConfigReq 	    mac_config_req = {RRM_ZERO};
    rrm_cell_context_t          *p_cell_ctx = RRM_PNULL;
    mac_config_req.bitmask = RRM_ZERO;
    /*EICIC_TDD_CHANGES_START*/
    U8                          abs_size = RRM_ZERO;
    /*EICIC_TDD_CHANGES_END*/
    mac_config_req.bitmask = RRM_ZERO;
    p_rrm_gbl_ctx = (rrm_global_context_t *)qvThisContext();
    //coverity_fix_req
    if(RRM_PNULL != p_rrm_gbl_ctx)
    {    
        p_cell_ctx = p_rrm_gbl_ctx->p_rrm_cell_ctx[cell_index];
        if(RRM_PNULL == p_cell_ctx)
        {
            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                    "Cell context is not found at [%d]",cell_index);
            return RRM_FAILURE;
        }
/*EICIC_TDD_CHANGES_START*/
#ifdef TDD_MODE_FLAG
        abs_size = rrm_find_abs_frame_size(& p_cell_ctx->ran_info.physical_layer_params.physical_layer_param_tdd_frame_structure);
#else
        abs_size = RRM_MAX_PATTERN_BYTE;
#endif
        RRM_MEMSET(mac_config_req.usableAbsPatternServingCellConfig.absPatternServingCell,RRM_ZERO,
                                                     RRM_MAX_PATTERN_BYTE);

        RRM_MEMSET(mac_config_req.absPatternServingCellConfig.absPatternServingCell,RRM_ZERO,
                                                     RRM_MAX_PATTERN_BYTE);
/*EICIC_TDD_CHANGES_END*/
        if( p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_SRV_ABS_RECONFIG_INFO_PRESENT )
        {
            /*EICIC_TDD_CHANGES_START*/
            RRM_MEMSET(p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,RRM_ZERO,
                                             RRM_MAX_PATTERN_BYTE);
            
            RRM_MEMCPY(p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,
                    p_mac_config_data->serving_abs_pattern,
                    abs_size);
            rrm_copy_tdd_bits(abs_size, p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern);
            /*EICIC_TDD_CHANGES_END*/
            RRM_MEMCPY(mac_config_req.absPatternServingCellConfig.absPatternServingCell,
                    p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,
                    MAX_ABS_PATTERN);
            mac_config_req.bitmask |= ABS_PATTERN_CONFIG_BITMASK; 

            /*SPR 14529 start*/
            if ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_EICIC_PROV_MOD_INFO_PRESENT)&&
                    ((p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type != RRM_OAM_ACT_AS_VICTIM)&&
                     (p_cell_ctx->p_new_reconfig_data->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)))
            {
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_TRUE;
                if(( (RRMCM_RMIF_EICIC_INFO_PRESENT & p_cell_ctx->operator_info.bitmask)&&
                            ( p_cell_ctx->operator_info.eicic_info.bitmask & RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT ) &&
                            ( p_cell_ctx->operator_info.eicic_info.eicic_timer_info.bitmask & RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT)) ||
                        ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_REQUIRED_PER_RECONFIG_TO_MAC_PRESENT))
                  )
                {
                    mac_config_req.layer2EicicMeasReportConfig.periodicity =
                        p_cell_ctx->operator_info.eicic_info.eicic_timer_info.mac_eicic_meas_report_periodicity;
                }
                else
                {
                    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                            "eICIC provisioned_type is victim or reconfigured as victim but periodicity is not provided for the cell");
                   /*15032_start*/ 
                   RRM_UT_TRACE_EXIT();
                   return RRM_FAILURE;
                   /*15032_end*/
                }
            }
            else if ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_EICIC_PROV_MOD_INFO_PRESENT)&&
                    ((p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)&&
                     (p_cell_ctx->p_new_reconfig_data->operator_info.eicic_info.eicic_provisioned_type != RRM_OAM_ACT_AS_VICTIM)))
            {
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_FALSE;
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                        "if eicic type is not victim then stop meas_reports");
            }
            else if(p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)
            {
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_TRUE;
                if(( (RRMCM_RMIF_EICIC_INFO_PRESENT & p_cell_ctx->operator_info.bitmask)&&
                            ( p_cell_ctx->operator_info.eicic_info.bitmask & RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT ) &&
                            ( p_cell_ctx->operator_info.eicic_info.eicic_timer_info.bitmask & RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT)) ||
                        ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_REQUIRED_PER_RECONFIG_TO_MAC_PRESENT))
                  )
                {
                    mac_config_req.layer2EicicMeasReportConfig.periodicity =
                        p_cell_ctx->operator_info.eicic_info.eicic_timer_info.mac_eicic_meas_report_periodicity;
                }
                else
                {
                    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                            "eICIC provisioned_type is victim or reconfigured as victim but periodicity is not provided for the cell");
                   /*15032_start*/ 
                   RRM_UT_TRACE_EXIT();
                   return RRM_FAILURE;
                   /*15032_end*/
                }
            }
            /*SPR 14529 end*/
        }

        if( p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_USABLE_ABS_RECONFIG_INFO_PRESENT )
        {
            /*EICIC_TDD_CHANGES_START*/            
            RRM_MEMSET(p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern,RRM_ZERO,
                                            RRM_MAX_PATTERN_BYTE);
            RRM_MEMCPY(p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern,
                    p_mac_config_data->usable_abs_pattern,
                    abs_size);
            rrm_copy_tdd_bits(abs_size, p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern);
            /*EICIC_TDD_CHANGES_END*/
            RRM_MEMCPY(mac_config_req.usableAbsPatternServingCellConfig.absPatternServingCell,
                    p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern,
                    MAX_ABS_PATTERN);
            mac_config_req.bitmask |= USABLE_ABS_PATTERN_CONFIG_BITMASK; 
        
            /*SPR 14529 start*/
            if ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_EICIC_PROV_MOD_INFO_PRESENT)&&
                    ((p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type != RRM_OAM_ACT_AS_VICTIM)&&
                     (p_cell_ctx->p_new_reconfig_data->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)))
            {
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_TRUE;
                if(( (RRMCM_RMIF_EICIC_INFO_PRESENT & p_cell_ctx->operator_info.bitmask)&&
                            ( p_cell_ctx->operator_info.eicic_info.bitmask & RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT ) &&
                            ( p_cell_ctx->operator_info.eicic_info.eicic_timer_info.bitmask & RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT)) ||
                        ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_REQUIRED_PER_RECONFIG_TO_MAC_PRESENT))
                  )
                {
                    mac_config_req.layer2EicicMeasReportConfig.periodicity =
                        p_cell_ctx->operator_info.eicic_info.eicic_timer_info.mac_eicic_meas_report_periodicity;
                }
                else
                {
                    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                            "eICIC provisioned_type is victim or reconfigured as victim but periodicity is not provided for the cell");
                   /*15032_start*/ 
                   RRM_UT_TRACE_EXIT();
                   return RRM_FAILURE;
                   /*15032_end*/
            }
            }
            else if ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_EICIC_PROV_MOD_INFO_PRESENT)&&
                    ((p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)&&
                     (p_cell_ctx->p_new_reconfig_data->operator_info.eicic_info.eicic_provisioned_type != RRM_OAM_ACT_AS_VICTIM)))
            {
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_FALSE;
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                        "if eicic type is not victim then stop meas_reports");
            }
            else if(p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)
            {
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_TRUE;
                if(( (RRMCM_RMIF_EICIC_INFO_PRESENT & p_cell_ctx->operator_info.bitmask)&&
                            ( p_cell_ctx->operator_info.eicic_info.bitmask & RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT ) &&
                            ( p_cell_ctx->operator_info.eicic_info.eicic_timer_info.bitmask & RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT)) ||
                        ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_REQUIRED_PER_RECONFIG_TO_MAC_PRESENT))
                  )
                {
                    mac_config_req.layer2EicicMeasReportConfig.periodicity =
                        p_cell_ctx->operator_info.eicic_info.eicic_timer_info.mac_eicic_meas_report_periodicity;
                }
                else
                {
                    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR,
                            "eICIC provisioned_type is victim or reconfigured as victim but periodicity is not provided for the cell");
                   /*15032_start*/ 
                   RRM_UT_TRACE_EXIT();
                   return RRM_FAILURE;
                   /*15032_end*/
                }
            }
            /*SPR 14529 end*/
        }
        if(p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_REQUIRED_PER_RECONFIG_TO_MAC_PRESENT)
        {   
            mac_config_req.layer2EicicMeasReportConfig.periodicity = p_mac_config_data->mac_eicic_meas_report_periodicity; 
            mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK;
            /*SPR 14529 start*/
            if((p_cell_ctx->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)||
                    ((p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask & RRM_CTXT_EICIC_PROV_MOD_INFO_PRESENT)&&
                     (p_cell_ctx->p_new_reconfig_data->operator_info.eicic_info.eicic_provisioned_type == RRM_OAM_ACT_AS_VICTIM)))
            {
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_TRUE;
            }
            else
            {
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_FALSE;
            }
            /*SPR 14529 end*/
        }
    }

   /* eICIC_PHASE_1_2_CHANGES_END */
    if(RRM_ZERO !=  mac_config_req.bitmask)
    {
        ret_val = rrm_send_mac_config_req (&mac_config_req, RRM_MODULE_ID, trans_id, cell_index);
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_cellm_log_on_off,p_rrm_cellm_facility_name,RRM_ERROR,
                    "Failed to Send to RRM_MAC_CELL_CONFIG_REQ L2");
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/** eICIC_PHASE_1_2_CHANGES_END */

/* DYNAMIC ICIC CHANGES START   */
/****************************************************************************
 * Function Name  : build_and_send_l2_cell_config_req 
 * Inputs         : p_dl_resource_partition_info, p_ul_res_partition_info,
 *                  serving_abs_pattern,usable_abs_pattern,
                    cell_index, trans_id
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function sends the RRM_MAC_CELL_CONFIG_REQ to L2
 ****************************************************************************/
rrm_return_et build_and_send_l2_cell_config_req
(
   rrm_resource_partition_info_t *p_dl_resource_partition_info,
   rrm_resource_partition_info_t *p_ul_res_partition_info,
   /* eICIC_PHASE_1_2_CHANGES_START */
   U8                           *p_serving_abs_pattern,
   U8                           *p_usable_abs_pattern,
   /* eICIC_PHASE_1_2_CHANGES_END */
   rrm_cell_index_t           cell_index,
   U16                        trans_id
)
{

	rrm_return_et 		        ret_val = RRM_FAILURE;
	RrmMacCellConfigReq 	    mac_config_req = {RRM_ZERO};
	
   
 
	mac_config_req.bitmask = RRM_ZERO; 
    if (RRM_PNULL != p_dl_resource_partition_info)
    {
        mac_config_req.bitmask |= DL_RESOURCE_INFO_BITMASK;
        mac_config_req.dlResourcePartitionInfo.bitmask = RRM_ZERO;
        if (p_dl_resource_partition_info->bitmask & 
                    RRMCM_RMIF_RESOURCE_PARTITION_CELL_EDGE_PRESENT)
        {
            mac_config_req.dlResourcePartitionInfo.bitmask |= 
                CE_RESOURCE_INFO_BITMASK;

            mac_config_req.dlResourcePartitionInfo.numOfCellEdgeRegion = 
                     p_dl_resource_partition_info->num_of_cell_edge_region;
            mac_config_req.dlResourcePartitionInfo.CellEdgeRegion.startRB = 
                     p_dl_resource_partition_info->cell_edge_region.start_rb;
            mac_config_req.dlResourcePartitionInfo.CellEdgeRegion.numOfRB = 
                     p_dl_resource_partition_info->cell_edge_region.num_of_rb;
        }
        mac_config_req.dlResourcePartitionInfo.numOfCellCenterRegion = 
                    p_dl_resource_partition_info->num_of_cell_center_region; 
        RRM_MEMCPY(&(mac_config_req.dlResourcePartitionInfo.CellCenterRegion),
                   &(p_dl_resource_partition_info->cell_center_region),
                   sizeof(resource_partition_t) * RRMCM_RMIF_MAX_CC_REGION);
    }

    if (RRM_PNULL != p_ul_res_partition_info)
    {
        
        mac_config_req.bitmask |= UL_RESOURCE_INFO_BITMASK;
        mac_config_req.ulResourcePartitionInfo.bitmask = RRM_ZERO;
        if (p_ul_res_partition_info->bitmask & 
                    RRMCM_RMIF_RESOURCE_PARTITION_CELL_EDGE_PRESENT)
        {
            mac_config_req.ulResourcePartitionInfo.bitmask |= 
                CE_RESOURCE_INFO_BITMASK;

            mac_config_req.ulResourcePartitionInfo.numOfCellEdgeRegion = 
                     p_ul_res_partition_info->num_of_cell_edge_region;
            mac_config_req.ulResourcePartitionInfo.CellEdgeRegion.startRB = 
                     p_ul_res_partition_info->cell_edge_region.start_rb;
            mac_config_req.ulResourcePartitionInfo.CellEdgeRegion.numOfRB = 
                     p_ul_res_partition_info->cell_edge_region.num_of_rb;
        }
        mac_config_req.ulResourcePartitionInfo.numOfCellCenterRegion = 
                    p_ul_res_partition_info->num_of_cell_center_region; 
        RRM_MEMCPY(&(mac_config_req.ulResourcePartitionInfo.CellCenterRegion),
                   &(p_ul_res_partition_info->cell_center_region),
                   sizeof(resource_partition_t) * RRMCM_RMIF_MAX_CC_REGION);
    }
   /*
   ** Multiple cell support, Passing a valid cell_index if multiple cell support
   ** is enabled, otherwise passing 0 in cell_index.
   */
   /* eICIC_PHASE_1_2_CHANGES_START */
    if (RRM_PNULL != p_serving_abs_pattern)
    {
        mac_config_req.bitmask |= ABS_PATTERN_CONFIG_BITMASK;
        
        /* bug_14010_start */  
        RRM_MEMSET(mac_config_req.absPatternServingCellConfig.absPatternServingCell,
                   RRM_ZERO,
                   MAX_ABS_PATTERN);
        /* bug_14010_end */
        
        RRM_MEMCPY(mac_config_req.absPatternServingCellConfig.absPatternServingCell,
                   p_serving_abs_pattern,
                   MAX_ABS_PATTERN);
    }

    if (RRM_PNULL != p_usable_abs_pattern)
    {
        mac_config_req.bitmask |= USABLE_ABS_PATTERN_CONFIG_BITMASK;
        /* bug_14010_start */  
        RRM_MEMSET(mac_config_req.usableAbsPatternServingCellConfig.absPatternServingCell,
                   RRM_ZERO,
                   MAX_ABS_PATTERN);
        /* bug_14010_end */
        RRM_MEMCPY(mac_config_req.usableAbsPatternServingCellConfig.absPatternServingCell,
                   p_usable_abs_pattern,
                   MAX_ABS_PATTERN);
    }

   /* eICIC_PHASE_1_2_CHANGES_END */
    ret_val = rrm_send_mac_config_req (&mac_config_req, RRM_MODULE_ID, trans_id, cell_index);
	if(RRM_FAILURE == ret_val)
	{
		RRM_TRACE(g_cellm_log_on_off,p_rrm_cellm_facility_name,RRM_ERROR,
				"Failed to Send to RRM_MAC_CELL_CONFIG_REQ L2");
	}

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*Klockwork_fix_start*/
/****************************************************************************
 * Function Name  : rrm_request_icic_scheme_sfr
 * Inputs         : pericodic_reporting, p_operator_info, cell_index, trans_id
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function populates the RRM_MAC_CELL_CONFIG_REQ structure
 ****************************************************************************/
rrm_void_t
rrm_request_icic_scheme_sfr(operator_info_t       *p_operator_info ,
                            RrmMacCellConfigReq   *mac_config_req)
{
    rrm_report_config_info_t    *report_config_info =RRM_PNULL;
    rrm_dynamic_icic_info_t     *p_dynamic_icic_info =RRM_PNULL;
    U8                          count = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    
    RRM_ASSERT(RRM_PNULL != p_operator_info);   /* Klocwork_fix_27_aug */

    if(ICIC_SCHEME_SFR == p_operator_info->dynamic_icic_info.icic_scheme_type)
        /*Location update fix in UE RECONFIG*/
    {
        report_config_info = &(p_operator_info->dynamic_icic_info.report_config_info);
        RRM_ASSERT(RRM_PNULL != report_config_info);

        p_dynamic_icic_info = &(p_operator_info->dynamic_icic_info);
        RRM_ASSERT(RRM_PNULL != p_dynamic_icic_info);

        mac_config_req->layer1MeasReportConfig.bitmask = RRM_ZERO;
        if(RRM_L2_l1_MEAS_REPORT_CONFIG_INFO & report_config_info->bitmask)
        {    
            mac_config_req->layer1MeasReportConfig.layer1MeasReportRequired = 
                report_config_info->l1_meas_report_config_info.layer1_meas_report_required;
            mac_config_req->layer1MeasReportConfig.bitmask |= L1_MEAS_REPORT_REQD_BITMASK;

            mac_config_req->layer1MeasReportConfig.layer1ReportAveragingEnabled = 
                report_config_info->l1_meas_report_config_info.layer1_meas_report_avg_enabled;
            mac_config_req->layer1MeasReportConfig.bitmask |= L1_MEAS_REPORT_AVERAGING_ENABLED_BITMASK;

            mac_config_req->layer1MeasReportConfig.periodicity = 
                report_config_info->l1_meas_report_config_info.layer1_meas_report_perodicity;
            mac_config_req->layer1MeasReportConfig.bitmask |= L1_MEAS_REPORT_PERIODICITY_BITMASK;                        

            mac_config_req->bitmask |= L1_REPORT_CONFIG_BITMASK;
        }   
        /* filling layer2IcicMeasReportConfig */
        mac_config_req->layer2IcicMeasReportConfig.bitmask = RRM_ZERO;
        if(RRM_L2_l2_MEAS_REPORT_CONFIG_INFO & report_config_info->bitmask)
        {
            mac_config_req->layer2IcicMeasReportConfig.cellEdgePrbUsageRequired = 
                report_config_info->l2_meas_report_config_info.cell_edge_prb_usage_required;
            mac_config_req->layer2IcicMeasReportConfig.bitmask |= 
                CE_PRB_USAGE_REQD_BITMASK;

            mac_config_req->layer2IcicMeasReportConfig.periodicity = 
                report_config_info->l2_meas_report_config_info.l2_icic_report_perodicity;
            mac_config_req->layer2IcicMeasReportConfig.bitmask |= 
                L2_ICIC_MEAS_REPORT_PERIODICITY_BITMASK;
            mac_config_req->bitmask |= ICIC_REPORT_CONFIG_BITMASK ; 
        }

        mac_config_req->layer2IcicMeasReportConfig.rntpReportingConfigInfo.bitmask = RRM_ZERO;
        if(RRMCM_RMIF_RNTP_REPORT_CONFIG_INFO_PRESENT & p_dynamic_icic_info->bitmask)
        {
            mac_config_req->layer2IcicMeasReportConfig.bitmask |=
                RNTP_CONFIG_BITMASK;
            mac_config_req->layer2IcicMeasReportConfig.rntpReportingConfigInfo.rntpReportRequired = 
                p_dynamic_icic_info->rntp_report_config_info.rntp_report_on_X2_required;

            mac_config_req->layer2IcicMeasReportConfig.rntpReportingConfigInfo.rntpThreshold = 
                p_dynamic_icic_info->rntp_report_config_info.rntp_threshold;

            mac_config_req->layer2IcicMeasReportConfig.rntpReportingConfigInfo.bitmask |= 
                RNTP_THRESHOLD_BITMASK;

            if(RRMCM_RMIF_MAX_NOMINAL_EPRE_PRESENT == p_dynamic_icic_info->rntp_report_config_info.bitmask)
            {
                mac_config_req->layer2IcicMeasReportConfig.rntpReportingConfigInfo.maxNominalEPRE = 
                    p_dynamic_icic_info->rntp_report_config_info.max_nominal_epre;

                mac_config_req->layer2IcicMeasReportConfig.rntpReportingConfigInfo.bitmask |= 
                    MAX_NOMINAL_EPRE_BITMASK;
            }
            mac_config_req->bitmask |= ICIC_REPORT_CONFIG_BITMASK ; 
        }

        /* filling ulPowerMask */
        if(RRMCM_RMIF_UL_POWER_MASK_PRESENT & p_dynamic_icic_info->bitmask)
        {
            mac_config_req->ulPowerMask.cellCenterUserPowerMask =
                p_dynamic_icic_info->ul_power_mask.cell_center_user_power_mask;

            mac_config_req->ulPowerMask.cellEdgeUserPowerMask = 
                p_dynamic_icic_info->ul_power_mask.cell_edge_user_power_mask;

            for (count = 0; count < RRM_MAX_QCI; count++)
            {
                mac_config_req->ulPowerMask.qciDeltaPowerMask[count] =
                    p_dynamic_icic_info->ul_power_mask.qci_delta_power_mask[count];
            }
            mac_config_req->bitmask |= CONFIG_UL_POWER_BITMASK;
        }
    }
    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_request_icic_scheme_dl_resource
 * Inputs         : pericodic_reporting, p_operator_info, cell_index, trans_id
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function populates the RRM_MAC_CELL_CONFIG_REQ structure
 ****************************************************************************/
rrm_void_t
rrm_request_icic_scheme_dl_resource(operator_info_t       *p_operator_info ,
                                    RrmMacCellConfigReq   *mac_config_req)
{

    U8                          count = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    if(RRMCM_RMIF_DL_RESOURCE_PARTITION_INFO_PRESENT & p_operator_info->dynamic_icic_info.bitmask) 
    {
        mac_config_req->dlResourcePartitionInfo.bitmask = RRM_ZERO;

        mac_config_req->dlResourcePartitionInfo.numOfCellCenterRegion = 
            p_operator_info->dynamic_icic_info.dl_resource_partition_info.num_of_cell_center_region;     

        for(count = RRM_ZERO; count < 
                p_operator_info->dynamic_icic_info.dl_resource_partition_info.num_of_cell_center_region; count++)
        {
            mac_config_req->dlResourcePartitionInfo.CellCenterRegion[count].startRB = 
                p_operator_info->dynamic_icic_info.dl_resource_partition_info.cell_center_region[count].start_rb;
            mac_config_req->dlResourcePartitionInfo.CellCenterRegion[count].numOfRB =
                p_operator_info->dynamic_icic_info.dl_resource_partition_info.cell_center_region[count].num_of_rb;
        }

        if(RRMCM_RMIF_RESOURCE_PARTITION_CELL_EDGE_PRESENT & p_operator_info->dynamic_icic_info.dl_resource_partition_info.bitmask)
        {
            mac_config_req->dlResourcePartitionInfo.numOfCellEdgeRegion =
                p_operator_info->dynamic_icic_info.dl_resource_partition_info.num_of_cell_edge_region;

            mac_config_req->dlResourcePartitionInfo.CellEdgeRegion.startRB = 
                p_operator_info->dynamic_icic_info.dl_resource_partition_info.cell_edge_region.start_rb;

            mac_config_req->dlResourcePartitionInfo.CellEdgeRegion.numOfRB =
                p_operator_info->dynamic_icic_info.dl_resource_partition_info.cell_edge_region.num_of_rb;

            mac_config_req->dlResourcePartitionInfo.bitmask |=
                CE_RESOURCE_INFO_BITMASK;
        }
        mac_config_req->bitmask |= DL_RESOURCE_INFO_BITMASK;
    }
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_end*/

/****************************************************************************
 * Function Name  : rrm_request_l2_cell_config_req 
 * Inputs         : pericodic_reporting, p_operator_info, cell_index, trans_id 
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function populates the RRM_MAC_CELL_CONFIG_REQ structure
 ****************************************************************************/
rrm_void_t
rrm_request_l2_cell_config_req
(
    /*SPR 17777 +-*/
    /* eICIC_PHASE_1_2_CHANGES_START */
     rrm_cell_context_t  * p_cell_ctx
    /* eICIC_PHASE_1_2_CHANGES_END */
 )
{
    rrm_return_et 		        ret_val = RRM_FAILURE;
    RrmMacCellConfigReq 	    mac_config_req = {RRM_ZERO};
    /*EICIC_TDD_CHANGES_START*/
    U8                          abs_size = RRM_ZERO;
    /*EICIC_TDD_CHANGES_END*/
    U8                          count = RRM_ZERO;
    /* eICIC_PHASE_1_2_CHANGES_START */
    U8                                ncl_cnt = RRM_ZERO;
    rrm_bool_et                       x2_up_vd_neigh = RRM_FALSE;
    operator_info_t                   *p_operator_info = RRM_PNULL;
    /* eICIC_PHASE_1_2_CHANGES_END */
    RRM_UT_TRACE_ENTER();
    /* Klocwork changes start 22 may */
    RRM_ASSERT(RRM_PNULL != p_cell_ctx);
    p_operator_info = &(p_cell_ctx->operator_info);
    RRM_ASSERT(RRM_PNULL != p_operator_info);
    /* Klocwork changes end 22 may */
    mac_config_req.bitmask = RRM_ZERO; 
    mac_config_req.bitmask |= PERIODIC_REPORT_CONFIG_BITMASK; /*Bitmask set for periodic SINR report */

    mac_config_req.periodicReportConfig.bitmask = RRM_ZERO;
    mac_config_req.periodicReportConfig.bitmask |= PERIODIC_REPORTS_REQUIRED_BITMASK;
    /* SPR 8505  FIX START */
    mac_config_req.periodicReportConfig.periodicReportRequired = 
        p_operator_info->rrm_mac_config.periodic_report_required ;//pericodic_reporting;
    /* SPR 8505  FIX END */

    mac_config_req.periodicReportConfig.bitmask |= PERIODICITY_BITMASK;
    /* SPR 8505  FIX START */
    mac_config_req.periodicReportConfig.periodicity =  p_operator_info->rrm_mac_config.periodic_report_periodicity;
    /* SPR 8505  FIX END */

    mac_config_req.periodicReportConfig.bitmask |= UE_BATCH_SIZE_BITMASK;
    /* SPR 8505  FIX START */
    mac_config_req.periodicReportConfig.ueBatchSize = p_operator_info->rrm_mac_config.periodic_report_ue_batch_size;
    /* SPR 8505  FIX END */
/*EICIC_TDD_CHANGES_START*/
#ifdef TDD_MODE_FLAG
    abs_size = rrm_find_abs_frame_size(& p_cell_ctx->ran_info.physical_layer_params.physical_layer_param_tdd_frame_structure);
#else
    abs_size = RRM_MAX_PATTERN_BYTE;
#endif
/*EICIC_TDD_CHANGES_START*/    
    /* DYNAMIC ICIC CHANGES START */

    if(RRMCM_RMIF_DYNAMIC_ICIC_INFO_PRESENT & p_operator_info->bitmask)
    {
        /*Location update fix in UE RECONFIG*/
        if (RRMCM_RMIF_ICIC_SCHEME_TYPE_PRESENT & p_operator_info->dynamic_icic_info.bitmask)
        { 
            /*Klockwork_fix_start*/
            rrm_request_icic_scheme_sfr(p_operator_info ,&mac_config_req);
            rrm_request_icic_scheme_dl_resource(p_operator_info ,&mac_config_req);
            /*Klockwork_fix_end*/


            if(RRMCM_RMIF_UL_RESOURCE_PARTITION_INFO_PRESENT & p_operator_info->dynamic_icic_info.bitmask) 
            {
                mac_config_req.ulResourcePartitionInfo.bitmask = RRM_ZERO;

                mac_config_req.ulResourcePartitionInfo.numOfCellCenterRegion = 
                    p_operator_info->dynamic_icic_info.ul_resource_partition_info.num_of_cell_center_region;     

                for(count = RRM_ZERO; count < 
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.num_of_cell_center_region; count++)
                {
                    mac_config_req.ulResourcePartitionInfo.CellCenterRegion[count].startRB = 
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.cell_center_region[count].start_rb;
                    mac_config_req.ulResourcePartitionInfo.CellCenterRegion[count].numOfRB =
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.cell_center_region[count].num_of_rb;
                }

                if(RRMCM_RMIF_RESOURCE_PARTITION_CELL_EDGE_PRESENT & 
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.bitmask)
                {
                    mac_config_req.ulResourcePartitionInfo.numOfCellEdgeRegion =
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.num_of_cell_edge_region;

                    mac_config_req.ulResourcePartitionInfo.CellEdgeRegion.startRB = 
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.cell_edge_region.start_rb;

                    mac_config_req.ulResourcePartitionInfo.CellEdgeRegion.numOfRB =
                        p_operator_info->dynamic_icic_info.ul_resource_partition_info.cell_edge_region.num_of_rb;

                    mac_config_req.ulResourcePartitionInfo.bitmask |=
                        CE_RESOURCE_INFO_BITMASK;
                }
                mac_config_req.bitmask |= UL_RESOURCE_INFO_BITMASK;
            }
        }
    }
    /* DYNAMIC ICIC CHANGES END   */
    /* eICIC_PHASE_1_2_CHANGES_START */
    p_cell_ctx->icic_l1_l2_report_info.decision_info_stored.bitmask = RRM_ZERO;
    /* Check if none of the NCL is X2_Allowed , then send the configured 
     ** ABS pattern to L2 as per the eICIC scheme activated 
     */
    if((p_cell_ctx->operator_info.bitmask & RRMCM_RMIF_EICIC_INFO_PRESENT) &&
           ( p_cell_ctx->operator_info.eicic_info.bitmask & RRMCM_RMIF_EICIC_PROVISIONED_TYPE_PRESENT))
    {
        for (ncl_cnt = RRM_ZERO; ncl_cnt < p_cell_ctx->ran_info.ncl_params.lte_ncl.num_valid_intra_freq_cell; ++ncl_cnt)
        {
		/* SPR14157start */
		if ((X2_PROHIBITED_CONNECTED == p_cell_ctx->ran_info.ncl_params.lte_ncl.intra_freq_cells[ncl_cnt].x2_status ) ||
				(X2_ALLOWED_CONNECTED == p_cell_ctx->ran_info.ncl_params.lte_ncl.intra_freq_cells[ncl_cnt].x2_status ) || 
                                (p_cell_ctx->ran_info.ncl_params.lte_ncl.intra_freq_cells[ncl_cnt].x2_link_up_status == RRM_TRUE))
			/* SPR14157end */
            {
                x2_up_vd_neigh = RRM_TRUE;
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
                          " X2 is Up with one of the neighbours. No need to apply operator specific"
                          " ABS patterns (both for aggressor and Victim"); 
                break;
            }
        }
       /*eICIC_Changes_Start*/
       /*Bug 831 Chnages Start*/
	    if((RRM_TRUE == rrm_cellm_is_aggressor_node(p_cell_ctx)))
       /*Bug 831 Chnages End*/
       /*eICIC_Changes_End*/
        {
            /* SPR 14811 start*/
            if((RRMCM_RMIF_ABS_INFO_PRESENT & p_operator_info->eicic_info.bitmask) &&
                    (RRMCM_RMIF_ABS_PATTERN_INFO_PRESENT & p_operator_info->eicic_info.abs_info.bitmask) &&
                    (RRMCM_RMIF_LOW_LOAD_ABS_PATTERN_PRESENT & p_operator_info->eicic_info.abs_info.abs_pattern.bitmask))
            {
                rrm_raise_error_event_notification(RRM_EE_ABS_FOUND_LOW_LOAD);
                /* bug_14010_start */  
                RRM_MEMSET(p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,RRM_ZERO,
                        RRM_MAX_PATTERN_BYTE);
                /* bug_14010_end */
                /*EICIC_TDD_CHANGES_START*/
                RRM_MEMCPY( p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,
                        p_operator_info->eicic_info.abs_info.abs_pattern.low_load_abs_pattern,
                        abs_size);
                rrm_copy_tdd_bits(abs_size , p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern);
                RRM_MEMCPY( mac_config_req.absPatternServingCellConfig.absPatternServingCell,
                        p_cell_ctx->rrm_eicic_cell_info.temp_srv_abs_pattern,
                        MAX_ABS_PATTERN);
                /*EICIC_TDD_CHANGES_END*/
                mac_config_req.bitmask |= ABS_PATTERN_CONFIG_BITMASK;
                p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask |= RRM_CTXT_SRV_ABS_RECONFIG_INFO_PRESENT;
            }
            else
            {
                rrm_raise_error_event_notification(RRM_EE_ABS_NOT_FOUND_LOW_LOAD);
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                        " ABS pattern hasn't been configured by the operator for the aggressor");
            }
        }
        if((RRM_TRUE == rrm_cellm_is_victim_node(p_cell_ctx)) && ( RRM_FALSE == x2_up_vd_neigh ))
        {
            if((RRMCM_RMIF_EICIC_TIMER_INFO_PRESENT & p_operator_info->eicic_info.bitmask) &&
                    (RRMCM_RMIF_MAC_EICIC_MEAS_REPORT_PERIODICITY_PRESENT & p_operator_info->eicic_info.eicic_timer_info.bitmask))
            {
                mac_config_req.layer2EicicMeasReportConfig.absUsageRequired = RRM_TRUE;
                mac_config_req.layer2EicicMeasReportConfig.periodicity = 
                    p_operator_info->eicic_info.eicic_timer_info.mac_eicic_meas_report_periodicity;
                mac_config_req.bitmask |= EICIC_REPORT_CONFIG_BITMASK; 
            }

            if((RRMCM_RMIF_ABS_INFO_PRESENT & p_operator_info->eicic_info.bitmask) &&
                    (RRMCM_RMIF_ABS_PATTERN_INFO_PRESENT & p_operator_info->eicic_info.abs_info.bitmask) &&
                    (RRMCM_RMIF_VICTIM_ABS_PATTERN_PRESENT & p_operator_info->eicic_info.abs_info.abs_pattern.bitmask))
            {
                rrm_raise_error_event_notification(RRM_EE_VICTIM_ABS_FOUND);
                /* bug_14010_start */  
                RRM_MEMSET(p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern,RRM_ZERO,
                        RRM_MAX_PATTERN_BYTE);
                /* bug_14010_end */
                /*EICIC_TDD_CHANGES_START*/
                RRM_MEMCPY( p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern,
                        p_operator_info->eicic_info.abs_info.abs_pattern.victim_abs_pattern,
                        abs_size);
                rrm_copy_tdd_bits(abs_size, p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern);
                RRM_MEMCPY( mac_config_req.usableAbsPatternServingCellConfig.absPatternServingCell,
                        p_cell_ctx->rrm_eicic_cell_info.temp_usable_abs_pattern,
                        MAX_ABS_PATTERN);
                /*EICIC_TDD_CHANGES_END*/
                mac_config_req.bitmask |= USABLE_ABS_PATTERN_CONFIG_BITMASK;
                p_cell_ctx->rrm_eicic_cell_info.reconfig_param_bitmask |= RRM_CTXT_USABLE_ABS_RECONFIG_INFO_PRESENT;
            }
            else
            {
                rrm_raise_error_event_notification(RRM_EE_VICTIM_ABS_NOT_FOUND);
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                        " Victim ABS pattern hasn't been configured by the operator for the Victim");
            }

            /*eicic_subset_pattern_fix_start*/
            if((RRMCM_RMIF_ABS_INFO_PRESENT & p_operator_info->eicic_info.bitmask) &&
                    (RRMCM_RMIF_ABS_PATTERN_INFO_PRESENT & p_operator_info->eicic_info.abs_info.bitmask) &&
                    (RRM_OAM_VICTIM_MEAS_SUBSET_PRESENT & p_operator_info->eicic_info.abs_info.abs_pattern.bitmask))
            {
                rrm_raise_error_event_notification(RRM_EE_VICTIM_MEAS_SUBSET_FOUND);
                RRM_MEMSET(p_cell_ctx->rrm_eicic_cell_info.temp_meas_abs_pattern,RRM_ZERO,
                        RRM_MAX_PATTERN_BYTE);
                /* bug_14010_end */
                /*EICIC_TDD_CHANGES_START*/
                RRM_MEMCPY( p_cell_ctx->rrm_eicic_cell_info.temp_meas_abs_pattern,
                        p_operator_info->eicic_info.abs_info.abs_pattern.victim_meas_subset,
                        abs_size);
                rrm_copy_tdd_bits(abs_size, p_cell_ctx->rrm_eicic_cell_info.temp_meas_abs_pattern);
                /*EICIC_TDD_CHANGES_END*/

            }    
            /*eicic_subset_pattern_fix_end*/ 
            else
            {
                rrm_raise_error_event_notification(RRM_EE_VICTIM_MEAS_SUBSET_NOT_FOUND);
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                        " Victim Meas Subset pattern hasn't been configured by the operator for the Victim");
            }
            /* SPR 14811 end*/
        }
    }
    /* eICIC_PHASE_1_2_CHANGES_END */
    /*
     ** Multiple cell support, Passing a valid cell_index if multiple cell support
     ** is enabled, otherwise passing 0 in cell_index.
     */
    ret_val = rrm_send_mac_config_req (&mac_config_req, RRM_MODULE_ID, 
            p_cell_ctx->ongoing_trans_id, 
            p_cell_ctx->cell_index);
    /* COMP_WARN_1_FEB:compilation warning removal */
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_cellm_log_on_off,p_rrm_cellm_facility_name,RRM_ERROR,
                "Failed to Send to RRM_MAC_CELL_CONFIG_REQ L2");
    }

    RRM_UT_TRACE_EXIT();
}

/* SPR 21251 +- */
/****************************************************************************
 * Function Name  : rrm_send_mac_config_req 
 * Inputs         : RrmMacCellConfigReq *p_mac_config_req
 *                : U16 src_module_id
 *                : U16 transaction_id
 *                : U8  cell_index
 * Outputs        : None
 * Returns        : None
 * Description    : send MAC cell config req
 ****************************************************************************/
rrm_return_et 
rrm_send_mac_config_req
(
 RrmMacCellConfigReq *p_mac_config_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index      /* cell_index */
 )
{
	U16 msg_length     = RRM_ZERO;
	U16 msg_api_length = RRM_ZERO;
	U8 *p_mac_msg = RRM_PNULL;
	U8 *p_msg = RRM_PNULL;
	RRM_UT_TRACE_ENTER();
	msg_length = sizeof(RrmMacCellConfigReq);
	msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

	/* Allocate buffer */
	p_msg = rrm_msg_mem_get(msg_api_length);

	if (p_msg == RRM_PNULL)
	{
		/* Not enough memory */
		return RRM_FAILURE;
	}
	p_mac_msg = p_msg;
	RRM_MEMSET(p_mac_msg, RRM_ZERO, msg_api_length);

	/* Fill CSPL header */
	rrm_construct_api_header(p_mac_msg, RRM_VERSION_ID, src_module_id,
			MAC_MODULE_ID,RRM_MAC_CELL_CONFIG_REQ, msg_api_length);

	p_mac_msg= p_mac_msg + RRM_API_HEADER_SIZE;

	/* Fill interface header */

    /*
    ** Multiple cell support, Passing a valid cell_index if multiple cell support
    ** is enabled, otherwise passing 0 in cell_index.
    */
	/*SPR 20856 Start*/
	rrm_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, RRM_MAC_CELL_CONFIG_REQ, msg_length,cell_index);
	/*SPR 20856 End*/

	p_mac_msg = p_mac_msg + RRM_INTERFACE_API_HEADER_SIZE;

	RRM_MEMCPY(p_mac_msg,p_mac_config_req,sizeof(RrmMacCellConfigReq));

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
            "Src(%u)->Dst(%u):RRM_MAC_CELL_CONFIG_REQ", src_module_id, MAC_MODULE_ID);
	/* Send message to */
	rrm_send_message(p_msg, MAC_MODULE_ID);
	RRM_UT_TRACE_EXIT();
	return RRM_SUCCESS;
}

/****************************************************************************
 * Function Name  : l2_reconfig_scheduler_process_msg 
 * Inputs         : rrm_void_t *l2_reconf_scheduler_req
 *                : U16 src_module_id
 *                : U16 transaction_id
                  : U8  cell_index
 * Outputs        : None
 * Returns        : None
 * Description    : Sends the RRM_MAC_SCHEDULER_RECONFIG_REQ API to L2
 ****************************************************************************/
rrm_return_et 
l2_reconfig_scheduler_process_msg
(
 rrm_void_t  *l2_reconf_scheduler_req,/* Pointer to API specific information. */
 U16         src_module_id,  /* Source module identifier */
 U16         transaction_id, /* Interface transaction identifier */
 U8          cell_index      /* cell index */
 )
{
	U16 msg_length     = RRM_ZERO;
	U16 msg_api_length = RRM_ZERO;
	U8 *p_mac_msg = RRM_PNULL;
	U8 *p_msg = RRM_PNULL;

	msg_length = sizeof(RrmMacReconfigSchedulerReq);
	msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;
	/* Allocate buffer */
	p_msg = rrm_msg_mem_get(msg_api_length);

	if (p_msg == RRM_PNULL)
	{
		/* Not enough memory */
		return RRM_FAILURE;
	}
	p_mac_msg = p_msg;
	RRM_MEMSET(p_mac_msg, RRM_ZERO, msg_api_length);

	/* Fill CSPL header */
	rrm_construct_api_header(p_mac_msg, RRM_VERSION_ID, src_module_id,
			MAC_MODULE_ID,RRM_MAC_SCHEDULER_RECONFIG_REQ, msg_api_length);

	p_mac_msg= p_mac_msg + RRM_API_HEADER_SIZE;
    /*
    ** Fill interface header. 
    ** Multiple cell support, Passing a valid cell_index if multiple cell support
    ** is enabled, otherwise passing 0 in cell_index.
    */
    /*SPR 20856 Start*/
	rrm_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, RRM_MAC_SCHEDULER_RECONFIG_REQ, msg_length, cell_index);
    /*SPR 20856 End*/

	/* Fill MAC_INIT_LAYER_REQ message */
	p_mac_msg = p_mac_msg + RRM_INTERFACE_API_HEADER_SIZE;

	RRM_MEMCPY(p_mac_msg,l2_reconf_scheduler_req,sizeof(RrmMacReconfigSchedulerReq));

        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
        "Src(%u)->Dst(%u):RRM_MAC_SCHEDULER_RECONFIG_REQ", src_module_id, MAC_MODULE_ID);
	/* Send message to */
	rrm_send_message(p_msg, MAC_MODULE_ID);
	return RRM_SUCCESS;
}
/* RACH_OPTIMIZATION_CHANGES_START */
/****************************************************************************
 * Function Name  : l2_perf_stats_req
 * Inputs         : rrm_void_t *l2_perf_stats_req
 *                : U16 src_module_id
 *                : U16 transaction_id
 : U8  cell_index
 * Outputs        : None
 * Returns        : None
 * Description    : Sends the MAC_CONFIGURE_PERF_STATS_REQ API to L2
 ****************************************************************************/
rrm_return_et 
l2_perf_stats_req
(
 rrm_void_t  *l2_perf_stats_req,/* Pointer to API specific information. */
 U16         src_module_id,  /* Source module identifier */
 U16         transaction_id, /* Interface transaction identifier */
 U8          cell_index      /* cell index */
 )
{
    U16 msg_length     = RRM_ZERO;
    U16 msg_api_length = RRM_ZERO;
    U8 *p_mac_msg = RRM_PNULL;
    U8 *p_msg = RRM_PNULL;

    msg_length = sizeof(rrm_mac_config_perf_stats_req_t);
    msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;
    /* Allocate buffer */
    p_msg = rrm_msg_mem_get(msg_api_length);

    if (p_msg == RRM_PNULL)
    {
        /* Not enough memory */
        return RRM_FAILURE;
    }
    p_mac_msg = p_msg;
    RRM_MEMSET(p_mac_msg, RRM_ZERO, msg_api_length);

    /* Fill CSPL header */
    rrm_construct_api_header(p_mac_msg, RRM_VERSION_ID, src_module_id,
            MAC_MODULE_ID,RRM_MAC_CONFIGURE_PERF_STATS_REQ, msg_api_length);

    p_mac_msg= p_mac_msg + RRM_API_HEADER_SIZE;
    /*
     ** Fill interface header. 
     ** Multiple cell support, Passing a valid cell_index if multiple cell support
     ** is enabled, otherwise passing 0 in cell_index.
     */
    /*SPR 20856 Start*/
    rrm_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
            MAC_MODULE_ID, RRM_MAC_CONFIGURE_PERF_STATS_REQ, msg_length, cell_index);
    /*SPR 20856 End*/

    /* Fill MAC_INIT_LAYER_REQ message */
    p_mac_msg = p_mac_msg + RRM_INTERFACE_API_HEADER_SIZE;

    RRM_MEMCPY(p_mac_msg,l2_perf_stats_req,sizeof(rrm_mac_config_perf_stats_req_t));

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
            "Src(%u)->Dst(%u):RRM_MAC_CONFIGURE_PERF_STATS_REQ", src_module_id, MAC_MODULE_ID);
    /* Send message to */
    rrm_send_message(p_msg, MAC_MODULE_ID);
    return RRM_SUCCESS;
}
/* RACH_OPTIMIZATION_CHANGES_END */

/****************************************************************************
 * Function Name  : rrm_send_mac_kpi_req 
 * Inputs         : MacConfigureKpiStatsReqParams *l2_kpi_stats_req
 *                : U16 src_module_id
 *                : U16 transaction_id
 *                : U8  cell_index
 * Outputs        : None
 * Returns        : None
 * Description    : send KPI stat req to L2
 ****************************************************************************/
rrm_return_et 
rrm_send_mac_kpi_req
(
 MacConfigureKpiStatsReqParams *l2_kpi_stats_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index
 )
{
	U16 msg_length     = RRM_ZERO;
	U16 msg_api_length = RRM_ZERO;
	U8 *p_mac_msg = RRM_PNULL;
	U8 *p_msg = RRM_PNULL;

	msg_length = sizeof(MacConfigureKpiStatsReqParams);
	msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

	/* Allocate buffer */
	p_msg = rrm_msg_mem_get(msg_api_length);

	if (p_msg == RRM_PNULL)
	{
		/* Not enough memory */
		return RRM_FAILURE;
	}
	p_mac_msg = p_msg;
	RRM_MEMSET(p_mac_msg, RRM_ZERO, msg_api_length);

	/* Fill CSPL header */
	rrm_construct_api_header(p_mac_msg, RRM_VERSION_ID, src_module_id,
			MAC_MODULE_ID,RRM_MAC_CONFIGURE_KPI_STATS_REQ, msg_api_length);

	p_mac_msg= p_mac_msg + RRM_API_HEADER_SIZE;
    /*
    ** Fill interface header.
    ** Multiple cell support, Passing a valid cell_index if multiple cell support
    ** is enabled, otherwise passing 0 in cell_index.
    */
    /*SPR 20856 Start*/
	rrm_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, RRM_MAC_CONFIGURE_KPI_STATS_REQ , msg_length, cell_index);
    /*SPR 20856 End*/

	/* Fill MAC_INIT_LAYER_REQ message */
	p_mac_msg = p_mac_msg + RRM_INTERFACE_API_HEADER_SIZE;
	RRM_MEMCPY(p_mac_msg,l2_kpi_stats_req,sizeof(MacConfigureKpiStatsReqParams));

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
            "Src(%u)->Dst(%u):RRM_MAC_CONFIGURE_KPI_STATS_REQ", src_module_id, MAC_MODULE_ID);
	/* Send message to */
	rrm_send_message(p_msg, MAC_MODULE_ID);
	return RRM_SUCCESS;
}


/* eICIC_PHASE_1_2_CHANGES_START */
/****************************************************************************
 * Function Name  : build_and_send_l2_ue_reconfig_req_for_victim 
 * Inputs         : cell_index, p_ue_index_list, num_of_ues 
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function builds and sends the RRM_MAC_UE_RECONFIG_REQ to L2
 ****************************************************************************/
rrm_return_et build_and_send_l2_ue_reconfig_req_for_victim 
(
     rrm_cell_index_t     cell_index ,
     rrm_ue_index_t       *p_ue_index_list,
     U32                  num_of_ues,
     U8                   dl_interference_info
)
{
   rrm_return_et 		        ret_val = RRM_FAILURE;
   /* Coverity Fix 99682 Start */
   RrmMacUeReconfigReq          *p_rrm_mac_ue_reconfig_req = RRM_PNULL;
   /* Coverity Fix 99682 End */
   U32                          ue_count = RRM_ZERO;
   
   RRM_UT_TRACE_ENTER();
   /* Coverity Fix 99682 Start */
   p_rrm_mac_ue_reconfig_req = (RrmMacUeReconfigReq *)
                        rrm_mem_get(sizeof(RrmMacUeReconfigReq));
   if( RRM_PNULL ==  p_rrm_mac_ue_reconfig_req)
   {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,"Memory Allocation Failure");
        return ret_val;
   }
   RRM_MEMSET(p_rrm_mac_ue_reconfig_req,RRM_ZERO,sizeof(RrmMacUeReconfigReq));
   /* Coverity Fix 99682 End */
   
   /* Coverity Fix 99682 Start */
   p_rrm_mac_ue_reconfig_req->bitmask = RRM_ZERO;
   p_rrm_mac_ue_reconfig_req->bitmask |= UE_DL_CONTROL_CHANNEL_INTERFERENCE_LIST_RECONFIG_BIT;
  
   p_rrm_mac_ue_reconfig_req->ueSevereDlControlChannelInterferenceInfo.numOfUE = num_of_ues;
   /* Coverity Fix 99682 End */
   for(ue_count = RRM_ZERO ;  ue_count < num_of_ues; ue_count++)
   {
      /* Coverity Fix 99682 Start */
      p_rrm_mac_ue_reconfig_req->ueSevereDlControlChannelInterferenceInfo.
                    SevereDlControlChannelInterferenceInfo[ue_count].
                    ueIndex =  p_ue_index_list[ue_count];
      p_rrm_mac_ue_reconfig_req->ueSevereDlControlChannelInterferenceInfo.
                    SevereDlControlChannelInterferenceInfo[ue_count].
                    SevereDlControlChannelInterference = dl_interference_info;
      /* Coverity Fix 99682 End */
   }
   /* Coverity Fix 99682 Start */
   ret_val = rrm_send_mac_ue_reconfig_req (p_rrm_mac_ue_reconfig_req, RRM_MODULE_ID, RRM_ZERO, cell_index);
   if(RRM_FAILURE == ret_val)
   {
		RRM_TRACE(g_cellm_log_on_off,p_rrm_cellm_facility_name,RRM_ERROR,
				"Failed to Send to RRM_MAC_UE_RECONFIG_REQ L2 ");
   }
   /* Coverity Fix 99682 Start */
   RRM_MEM_FREE(p_rrm_mac_ue_reconfig_req);
   /* Coverity Fix 99682 End */
   RRM_UT_TRACE_EXIT();
   return ret_val;
}
/* eICIC_PHASE_1_2_CHANGES_END */

/****************************************************************************
 * Function Name  : build_and_send_l2_ue_reconfig_req
 * Inputs         : cell_index, p_ue_index_list, num_of_ues, ue_location_update_action
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function builds and sends the RRM_MAC_UE_RECONFIG_REQ to L2
 ****************************************************************************/
rrm_return_et build_and_send_l2_ue_reconfig_req
(
     rrm_cell_index_t     cell_index ,
     rrm_ue_index_t *p_ue_index_list,
     U32             num_of_ues,
     rrm_location_update_action_et ue_location_update_action
)
{
   rrm_return_et                ret_val = RRM_FAILURE;
   /* Coverity Fix 99708 Start */
   RrmMacUeReconfigReq   *p_rrm_mac_ue_reconfig_req = RRM_PNULL;
   /* Coverity Fix 99708 End */
   U32                         ue_count = RRM_ZERO;
   U32                         ue_location = RRM_NULL;

   RRM_UT_TRACE_ENTER();
   
   /* Coverity Fix 99708 Start */
   p_rrm_mac_ue_reconfig_req = (RrmMacUeReconfigReq *)
                        rrm_mem_get(sizeof(RrmMacUeReconfigReq));
   if( RRM_PNULL ==  p_rrm_mac_ue_reconfig_req)
   {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,"Memory Allocation Failure");
        return ret_val;
   }
   RRM_MEMSET(p_rrm_mac_ue_reconfig_req,RRM_ZERO,sizeof(RrmMacUeReconfigReq));
   p_rrm_mac_ue_reconfig_req->bitmask = RRM_ZERO;
   p_rrm_mac_ue_reconfig_req->bitmask |= UE_LOCATION_RECONFIG_BIT;
   /* Coverity Fix 99708 End */
   if(UE_LOCATION_UPDATE_CC_TO_CE == ue_location_update_action)
   {
     ue_location = UE_LOCATION_UPDATE_CC_TO_CE;
   }
   else
   {
     ue_location = UE_LOCATION_UPDATE_CE_TO_CC;
   }
   /* Coverity Fix 99708 Start */
   p_rrm_mac_ue_reconfig_req->ueLocationInfo.numOfUE = num_of_ues;
   /* Coverity Fix 99708 End */
   for(ue_count = RRM_ZERO ;  ue_count < num_of_ues; ue_count++)
   {
      /* Coverity Fix 99708 Start */
      p_rrm_mac_ue_reconfig_req->ueLocationInfo.ueLocation[ue_count].ueIndex =  p_ue_index_list[ue_count];
      p_rrm_mac_ue_reconfig_req->ueLocationInfo.ueLocation[ue_count].ueLocationType =  ue_location;
      /* Coverity Fix 99708 End */
   }
   /* Coverity Fix 99708 Start */
   ret_val = rrm_send_mac_ue_reconfig_req (p_rrm_mac_ue_reconfig_req, RRM_MODULE_ID, RRM_ZERO, cell_index);
   /* Coverity Fix 99708 End */
   if(RRM_FAILURE == ret_val)
   {
        RRM_TRACE(g_cellm_log_on_off,p_rrm_cellm_facility_name,RRM_ERROR,
                "Failed to Send to RRM_MAC_UE_RECONFIG_REQ L2");
   }
   /* Coverity Fix 99708 Start */
   RRM_MEM_FREE(p_rrm_mac_ue_reconfig_req);
   /* Coverity Fix 99708 End */
   RRM_UT_TRACE_EXIT();
   return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_send_mac_ue_reconfig_req 
 * Inputs         : RrmMacUeReconfigReq *p_rrm_mac_ue_reconfig_req
 *                : U16 src_module_id
 *                : U16 transaction_id
 *                : U8  cell_index
 * Outputs        : None
 * Returns        : None
 * Description    : send MAC UE re-config req
 ****************************************************************************/
rrm_return_et 
rrm_send_mac_ue_reconfig_req
(
 RrmMacUeReconfigReq *p_rrm_mac_ue_reconfig_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index      /* cell_index */
 )
{
	U16 msg_length     = RRM_ZERO;
	U16 msg_api_length = RRM_ZERO;
	U8 *p_mac_msg = RRM_PNULL;
	U8 *p_msg = RRM_PNULL;
	RRM_UT_TRACE_ENTER();
	msg_length = sizeof(RrmMacUeReconfigReq);
	msg_api_length = msg_length + RRM_API_HEADER_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

	/* Allocate buffer */
	p_msg = rrm_msg_mem_get(msg_api_length);

	if (p_msg == RRM_PNULL)
	{
		/* Not enough memory */
		return RRM_FAILURE;
	}
	p_mac_msg = p_msg;
	RRM_MEMSET(p_mac_msg, RRM_ZERO, msg_api_length);

	/* Fill CSPL header */
	rrm_construct_api_header(p_mac_msg, RRM_VERSION_ID, src_module_id,
			MAC_MODULE_ID,RRM_MAC_UE_RECONFIG_REQ, msg_api_length);

	p_mac_msg= p_mac_msg + RRM_API_HEADER_SIZE;

	/* Fill interface header */

    /*
    ** Multiple cell support, Passing a valid cell_index if multiple cell support
    ** is enabled, otherwise passing 0 in cell_index.
    */
    /*SPR 20856 Start*/
	rrm_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, RRM_MAC_UE_RECONFIG_REQ, msg_length, cell_index);
    /*SPR 20856 End*/
	p_mac_msg = p_mac_msg + RRM_INTERFACE_API_HEADER_SIZE;
	
    RRM_MEMCPY(p_mac_msg,p_rrm_mac_ue_reconfig_req,sizeof(RrmMacUeReconfigReq));

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
            "Src(%u)->Dst(%u):RRM_MAC_UE_RECONFIG_REQ", src_module_id, MAC_MODULE_ID);
	/* Send message to */
	rrm_send_message(p_msg, MAC_MODULE_ID);
	RRM_UT_TRACE_EXIT();
	return RRM_SUCCESS;
}


/* DYNAMIC ICIC CHANGES END   */

/* uplink mu mimo change start*/

/* Bug_869_Changes_Start */
/* Bug_869_Changes_End */
/* uplink mu mimo change end*/

/* UE Positioning start */
/****************************************************************************
 * Function Name  : rrm_parse_mac_ecid_meas_report_resp
 * Inputs         : p_api_buf, data_len, p_api_meas_report
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function extracts the ecid meas report resp from MAC
 ****************************************************************************/
rrm_return_et
rrm_parse_mac_ecid_meas_report_resp( 
                    U8                                *p_api_buf,
    /*SPR 17777 +-*/
                    MacRrmEcidMeasurementResp  **p_api_ue_data
                    )
{
    rrm_return_et                     ret_val = RRM_SUCCESS;
    MacRrmEcidMeasurementResp  *p_mac_ecid_meas_report_resp = RRM_PNULL;
    U32                               bitmask = RRM_ZERO;
    U8                                index = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    p_mac_ecid_meas_report_resp = (MacRrmEcidMeasurementResp *)p_api_buf;
    SET_UE_INDEX(p_mac_ecid_meas_report_resp->ueIndex);

    bitmask = p_mac_ecid_meas_report_resp->bitmask;


    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF, \
                                "MAC->UEM:MAC_RRM_E_CID_MEAS_REPORT_RESP:[UE:%d] with esmlc_meas =%d, bitmask =%d,response_code=%d", 
                                 p_mac_ecid_meas_report_resp->ueIndex, 
                                 p_mac_ecid_meas_report_resp->eCidMeasId, 
                                 p_mac_ecid_meas_report_resp->bitmask,
                                 p_mac_ecid_meas_report_resp->responseCode);
    if( RRM_SUCCESS == p_mac_ecid_meas_report_resp->responseCode ||
        RRM_PARTIAL_SUCCESS == p_mac_ecid_meas_report_resp->responseCode)
    {
        *p_api_ue_data = (MacRrmEcidMeasurementResp *)rrm_mem_get(sizeof(MacRrmEcidMeasurementResp));

        if(RRM_PNULL != (*p_api_ue_data))
        {
            (*p_api_ue_data)->bitmask = bitmask ; 
            (*p_api_ue_data)->responseCode = p_mac_ecid_meas_report_resp->responseCode;
            (*p_api_ue_data)->ueIndex = p_mac_ecid_meas_report_resp->ueIndex;
            (*p_api_ue_data)->eCidMeasId = p_mac_ecid_meas_report_resp->eCidMeasId;

            if( bitmask & RRM_MAC_TYPE1_ENB_RXTX_REPORT_PRESENT)
            {
                (*p_api_ue_data)->type1EnbRxTxReport.systemFrameNum = 
                        p_mac_ecid_meas_report_resp->type1EnbRxTxReport.systemFrameNum;
                (*p_api_ue_data)->type1EnbRxTxReport.type1EnbRxTxDiffValue = 
                        p_mac_ecid_meas_report_resp->type1EnbRxTxReport.type1EnbRxTxDiffValue;
                
                /* SPR 8065 Fix: Wrong traces */
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF, \
                            "TYPE1_ENB_RXTX is present and values are type1_sfn[%d] and type1_rxtx_diff[%d]", \
                            p_mac_ecid_meas_report_resp->type1EnbRxTxReport.systemFrameNum, \
                            p_mac_ecid_meas_report_resp->type1EnbRxTxReport.type1EnbRxTxDiffValue);

                (*p_api_ue_data)->bitmask |= RRM_MAC_TYPE1_ENB_RXTX_REPORT_PRESENT;
            }
            if( bitmask & RRM_MAC_TYPE2_ENB_RXTX_REPORT_PRESENT)
            {
                (*p_api_ue_data)->type2EnbRxTxReport.systemFrameNum = 
                        p_mac_ecid_meas_report_resp->type2EnbRxTxReport.systemFrameNum;
                (*p_api_ue_data)->type2EnbRxTxReport.type2EnbRxTxDiffValue = 
                        p_mac_ecid_meas_report_resp->type2EnbRxTxReport.type2EnbRxTxDiffValue;
                
                /* SPR 8065 Fix: Wrong traces */
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF, \
                            "TYPE2_ENB_RXTX is present and values are type2_sfn[%d] and type2_rxtx_diff[%d]", \
                            p_mac_ecid_meas_report_resp->type2EnbRxTxReport.systemFrameNum, \
                            p_mac_ecid_meas_report_resp->type2EnbRxTxReport.type2EnbRxTxDiffValue);

                (*p_api_ue_data)->bitmask |= RRM_MAC_TYPE2_ENB_RXTX_REPORT_PRESENT;
            }
            if ( bitmask & RRM_MAC_AOA_REPORT_PRESENT )
            {
                (*p_api_ue_data)->angleOfArrivalReport.systemFrameNum = 
                        p_mac_ecid_meas_report_resp->angleOfArrivalReport.systemFrameNum;
                
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF, \
                            "MAC_AOA is present and value of SFN is AoA_sfn[%d]", \
                            p_mac_ecid_meas_report_resp->angleOfArrivalReport.systemFrameNum);

                for(index=RRM_ZERO; index<MAX_NUM_ANTENNA; index++)
                {
                    (*p_api_ue_data)->angleOfArrivalReport.angleOfArrivalValue[index] = 
                            p_mac_ecid_meas_report_resp->angleOfArrivalReport.angleOfArrivalValue[index];
                    
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF, \
                            "MAC_AOA is present and angleOfArrivalValue[%d] is %d", \
                            index, p_mac_ecid_meas_report_resp->angleOfArrivalReport.angleOfArrivalValue[index]);
                }
                
                (*p_api_ue_data)->bitmask |= RRM_MAC_AOA_REPORT_PRESENT;
            }
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, \
                        "Memory could not be allocated for MAC_RRM_E_CID_MEAS_REPORT_RESP api "); 
            ret_val = RRM_FAILURE;
        }
    }
    else
    {

        *p_api_ue_data = (MacRrmEcidMeasurementResp *)rrm_mem_get(sizeof(MacRrmEcidMeasurementResp));

        if( RRM_PNULL != (*p_api_ue_data) )
        {
            (*p_api_ue_data)->bitmask = bitmask;
            (*p_api_ue_data)->responseCode = p_mac_ecid_meas_report_resp->responseCode;
            (*p_api_ue_data)->ueIndex = p_mac_ecid_meas_report_resp->ueIndex;
            (*p_api_ue_data)->eCidMeasId = p_mac_ecid_meas_report_resp->eCidMeasId;
            
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, \
                "Memory could not be allocated for MAC_RRM_E_CID_MEAS_REPORT_RESP api ");
            ret_val = RRM_FAILURE;
        }
    } 

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* UE Positioning end */

/****************************************************************************
 * Function Name  : rrm_parse_mac_ue_reconfig_cnf
 * Inputs         : p_api_buf, data_len, p_api_ue_data
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function extract the Ues mac reconfig cnf
 ****************************************************************************/
rrm_return_et
rrm_parse_mac_ue_reconfig_cnf( U8                 *p_api_buf,
    /*SPR 17777 +-*/
        RrmMacUEReConfigCnf
        **p_api_ue_data )
{

    rrm_return_et                 ret_val = RRM_SUCCESS;
    RrmMacUEReConfigCnf    *p_mac_ue_cnf = RRM_PNULL;
    U32                           bitmask = RRM_ZERO;
    U32                           response_code = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    p_mac_ue_cnf = (RrmMacUEReConfigCnf *)p_api_buf;

    bitmask = p_mac_ue_cnf->bitmask;

    response_code = p_mac_ue_cnf->responseCode;

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
             "MAC->UEM:MAC_UE_RECONFIG_CNF with response: %d bitmask: %d",
             response_code, bitmask);


    *p_api_ue_data = (RrmMacUEReConfigCnf *)rrm_mem_get(sizeof(RrmMacUEReConfigCnf));
    if( RRM_FAILURE == response_code )
    {
        if( RRM_PNULL != (*p_api_ue_data) )
        {
            (*p_api_ue_data)->bitmask = bitmask ;
            (*p_api_ue_data)->responseCode = response_code;
            if( bitmask & UE_LOCATION_ERROR_INFO_BITMASK)
            {
                (*p_api_ue_data)->ueLocationErrorInfo = p_mac_ue_cnf->ueLocationErrorInfo;

            }
            if( bitmask & UE_MUMIMO_RECONFIG_ERROR_INFO_BITMASK)
            {
                (*p_api_ue_data)->ueULMUMIMOErrorInfo = p_mac_ue_cnf->ueULMUMIMOErrorInfo;
            }
            if( bitmask & UE_NUM_OF_LAYER_ERROR_INFO_BITMASK )
            {
                (*p_api_ue_data)->ueNumOfLayerErrorInfo = p_mac_ue_cnf->ueNumOfLayerErrorInfo;
            }
            if( bitmask & UE_BEAM_FORMING_ALGO_ERROR_INFO_BITMASK )
            {
                (*p_api_ue_data)->ueBeamFormingAlgoErrorInfo = p_mac_ue_cnf->ueBeamFormingAlgoErrorInfo;
            }
            /* Carrier Aggregation start*/
            if( bitmask & UE_CA_CONFIG_ERROR_INFO_BITMASK )
            {
                (*p_api_ue_data)->ueCarrierAggConfigErrorInfo = p_mac_ue_cnf->ueCarrierAggConfigErrorInfo ;
            }

            if( bitmask & UE_CQI_VALIDITY_TIMER_ERROR_INFO_BITMASK )
            {
                (*p_api_ue_data)->ueCqiValidityTimerErrorInfo = p_mac_ue_cnf->ueCqiValidityTimerErrorInfo ;
            }
            /* Carrier Aggregation end */
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, \
                    "Memory could not be allocated for RrmMacUEReConfigCnf api ");
            ret_val = RRM_FAILURE;
        }
    }
    else
    {
        if( RRM_PNULL != (*p_api_ue_data) )
        {
            (*p_api_ue_data)->bitmask = bitmask;
            (*p_api_ue_data)->responseCode = response_code;
        }
        else
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, \
                    "Memory could not be allocated for RrmMacUEReConfigCnf api ");
            ret_val = RRM_FAILURE;
        }
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 21251 +- */

