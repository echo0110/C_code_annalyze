/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2012 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 ****************************************************************************
 *
 *  File Description : UECC RRM UE RECONFIGURATION FSM functions implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/


/****************************************************************************
 * Header File Includes
 ***************************************************************************/
#include "rrc_rrm_intf.h"
#include "uecc_logger.h"
#include "uecc_common.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "uecc_fsm_engine.h"
#include "uecc_erab_delfsm.h"
/* CA start */
#include "uecc_rrm_ue_reconfig_fsm.h"
/* CA end */
#include "rrc_phy_intf.h"
#include "rrc_mac_intf.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
            p_ue_context->ue_index,\
            p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
##__VA_ARGS__)

#define SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     rrc_connection_release_not_required_flag = RRC_TRUE)



/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static uecc_fsm_activity_state_t uecc_fsm_rrm_ue_reconfig_llim_reconfig_ongoing;
static uecc_fsm_activity_state_t uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing;
static uecc_fsm_activity_state_t uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing;


//following to be moved to private variables section
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_fail_type1_actions[];
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_fail_type2_actions[];
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_rrc_reestablish_actions[];
/* coverity_96754 start */
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_fail_type3_actions[];
/* coverity_96754 stop */
/* CA start */
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_rlf_actions[];
/* CA end */

/* GDB fix - compiler warning */
extern rrc_return_et uecc_rrm_build_and_send_ue_reconfig_resp(  uecc_ue_context_t*  p_ue_context,
                                                                U8                  response,
                                                                U32                 fail_cause);


/* GDB fix - compiler warning */
extern rrc_return_et uecc_llim_build_and_send_rrm_ue_reconfig_rrc_connection_reconfiguration( uecc_ue_context_t*  p_uecc_ue_context );


/*SPR_16942_START*/
extern void uecc_update_mac_config_in_ue_context(
        uecc_ue_context_t*  p_ue_context,
        rrm_mac_config_t*   p_mac_config
        );
/*SPR_16942_END*/
/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_process_rrm_ue_reconfig_req_msg 
 * Input          : uecc_ue_context_t*            p_ue_context
 *                  rrc_rrm_ue_reconfig_req_t*    p_msg
 * Output         : None                 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function processes UE Reconfig Request message
 *                  message:
 *                  - check message content;
 *                  - store IEs in UE context;
 ****************************************************************************/
rrc_return_t uecc_fsm_rrm_ue_reconfig_process_rrm_ue_reconfig_req_msg(
        uecc_ue_context_t*            p_ue_context,
        rrc_rrm_ue_reconfig_req_t*    p_msg
        )
{
    rrc_return_t    result = RRC_SUCCESS;
    /*Bug:8353 fix start*/
    rrc_bool_et     found_flag = RRC_TRUE;
    /*Bug:8353 fix stop*/

    rrc_meas_subframe_pattern_pcell_r10_t*  p_meas_subframe_dest; /*dest meas subframe pattern pcell*/
    rrc_meas_subframe_pattern_pcell_r10_t*  p_meas_subframe_src ;

    rrm_ue_reconfig_request_data_t *p_ue_reconfig_req_data = PNULL;
    rrc_phy_physical_config_dedicated_t  *p_phy_config_src = PNULL;
    rrc_phy_physical_config_dedicated_t  *p_phy_config_dest = PNULL;
    rrm_mac_config_t *p_mac_config_src = PNULL;
    rrm_mac_config_t *p_mac_config_dest = PNULL;
    /*SPR_17969_Start*/
    /*Code Removed*/
    /*SPR_17969_End*/
 // rrc_mac_main_config_t *p_mac_main_config_dest = PNULL;
    rrm_mac_main_config_extensions_t  *p_mac_main_config_ext_dest = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_msg);

    p_ue_reconfig_req_data =
        &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data);

    /* Check that message is properly packed by RRM */
    if (!((RRM_UE_RECONFIG_REQ_API_UE_RECONFIG_REQ_PRESENT & p_msg->bitmask) ||
                (RRM_UE_RECONFIG_SCELL_CONFIG_PRESENT & p_msg->bitmask)))
    {
        RRC_TRACE(RRC_INFO,"RRM Message incomplete:"
                "RRM_UE_RECONFIG_REQ_API_UE_RECONFIG_REQ_PRESENT and"
                "RRM_UE_RECONFIG_SCELL_CONFIG_PRESENT not present");
        result = RRC_FAILURE;
    }
    else
    {
        /* Store message data in UE context for future processing */
        /* SPR 14562 Fix start */
        if(RRM_UE_RECONFIG_REQ_API_UE_RECONFIG_REQ_PRESENT
                & p_msg->bitmask)
        {
        /* SPR 14562 Fix end */
            do
            {
                if (RRM_UE_RECONFIG_PHY_CONFIG_DED_PRESENT & p_msg->radio_resource_reconfig.presence_bitmask)
                {
                    p_ue_reconfig_req_data->m.physical_config_dedicated_updated = 1;
                    p_phy_config_src = &(p_msg->radio_resource_reconfig.rrc_rrm_ue_reconfig_physical_config_dedicated);
                    p_phy_config_dest = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.physical_config_dedicated);

                    l3_memcpy_wrapper(p_phy_config_dest, p_phy_config_src, sizeof(rrc_phy_physical_config_dedicated_t));

                    /*Bug:8353 fix start*/
                    /*
                       / *Blank RCR fix start* /
                       if((p_phy_config_dest->bitmask & RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_ANTENNA_INFORMATION_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORTING_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT)||
                       (p_phy_config_dest->bitmask & RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)||
                       (p_phy_config_dest->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)||
                       / *tm7-8 fix* /
                       (p_phy_config_dest->bitmask & RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT) ||
                       (p_phy_config_dest->bitmask &
                       RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT) ||
                       / *tm7-8 fix* /
                       (p_phy_config_dest->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT) ||
                       (p_phy_config_dest->bitmask & RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT))
                       {
                       found_flag = RRC_TRUE;
                       }

                       / *Resetting following optional IEs which is not supported as a part
                     * of Dynamic UE Reconfiguration feature while reconfigurating
                     * Physical dedicated configuration parameters * /


                     if(p_phy_config_dest->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
                     {
                     p_phy_config_dest->bitmask ^= RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT;
                     RRC_TRACE(RRC_WARNING, "UE Reconfig of pucch_configuration_dedicated not supported");
                     }
                     */

                    if(p_phy_config_dest->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:Reconf of pucch_configuration_dedicated is not supported");
                        found_flag = RRC_FALSE;
                        break;
                    }
                    /* eICIC ue reconfig changes start */
                    if(((p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT) && 
                                (p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)) || 
                            ((p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)&&
                             (p_phy_config_dest->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)))
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:In Reconf both R10 and R8/R9 are present");
                        found_flag = RRC_FALSE;
                        result = RRC_FAILURE;
                        break;

                    }
                    /* eICIC code changes start */
                    if((p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT) &&
                            (p_phy_config_dest->cqi_report_config_r10.csi_subframePattern_r10.bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP) &&
                            !(p_phy_config_dest->cqi_report_config_r10.cqi_report_periodic_r10.setup.csi_config_index.bitmask & 
                                RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT))

                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:csi-meassubframepattern present but csi-config-index not present");
                        found_flag = RRC_FALSE;
                        result = RRC_FAILURE;
                        break;
                    }
                    if((p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT) &&
                            !(p_phy_config_dest->cqi_report_config_r10.csi_subframePattern_r10.bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP) &&
                            (p_phy_config_dest->cqi_report_config_r10.cqi_report_periodic_r10.setup.csi_config_index.bitmask & 
                             RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT))

                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:csi-config-index present but csi-meassubframepattern not present");
                        found_flag = RRC_FALSE;
                        result = RRC_FAILURE;
                        break;
                    }
                    if((p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT) &&
                            !(RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT &
                                p_ue_context->physical_config_dedicated.cqi_report_config_r10.cqi_report_periodic_r10.setup.bitmask) &&
                            !(RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT &
                                p_phy_config_dest->cqi_report_config_r10.cqi_report_periodic_r10.setup.bitmask) &&
                            (RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT & 
                             p_phy_config_dest->cqi_report_config_r10.bitmask))
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:ri-ConfigIndex not configured and csi-meassubframepattern present");
                        found_flag = RRC_FALSE;
                        result = RRC_FAILURE;
                        break;
                    }
                    if((p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT) &&
                            (p_phy_config_dest->cqi_report_config_r10.csi_subframePattern_r10.bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP) &&
                            !(RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT & 
                                p_phy_config_dest->cqi_report_config_r10.
                                cqi_report_periodic_r10.setup.csi_config_index.setup.bitmask))
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:ri-ConfigIndex2-r10 not configured and csi-meassubframepattern present");
                        found_flag = RRC_FALSE;
                        result = RRC_FAILURE;
                        break;
                    }
                    /* eICIC code changes stop */
                    /* eICIC ue reconfig changes stop */

                    /*tm7-8 fix*/
                    /*
                       if(p_phy_config_dest->bitmask & RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
                       {
                       p_phy_config_dest->bitmask ^= RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
                       RRC_TRACE(RRC_WARNING, "UE Reconfig of cqi_report_config_v920 not supported");
                       }
                     */
                    /*tm7-8 fix*/

                    /*Blank RCR fix end*/
                }


                if (RRM_UE_RECONFIG_MAC_CONFIG_PRESENT & p_msg->radio_resource_reconfig.presence_bitmask)
                {
                    p_ue_reconfig_req_data->m.mac_config_updated = 1;

                    p_mac_config_src = &(p_msg->radio_resource_reconfig.mac_config);
                    p_mac_config_dest = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.mac_config);
                    l3_memcpy_wrapper(p_mac_config_dest, p_mac_config_src, sizeof(rrm_mac_config_t));

                    /*Bug:8353 fix start*/
                /*SPR_17969_Start*/
                /*Code Removed*/
                /*SPR_17969_End*/
                p_mac_main_config_ext_dest = &(p_mac_config_dest->mac_main_config_extensions);
		
                /*SPR_17969_Start*/
                /*Code Removed*/
                /*SPR_17969_End*/
                /*MAC MAIN CONFIG EXTENTION*/
                    if(RRM_MAC_MAIN_CONFIG_EXT_USER_LOCATION_TYPE_PRESENT & p_mac_main_config_ext_dest->bitmask)
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:Reconf of user_location_type is not supported");
                        found_flag = RRC_FALSE;
                        break;
                    }

                    if(RRM_MAC_MAIN_CONFIG_EXT_UL_MU_MIMO_INFO_PRESENT & p_mac_main_config_ext_dest->bitmask)
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:Reconf of ul_mu_mimo_status is not supported");
                        found_flag = RRC_FALSE;
                        break;
                    }

                    if(RRM_MAC_MAIN_CONFIG_EXT_MIN_UE_POWER_PRESENT & p_mac_main_config_ext_dest->bitmask)
                    {
                        RRC_TRACE(RRC_ERROR,"RRM Message incorrect:Reconf of min_ue_power is not supported");
                        found_flag = RRC_FALSE;
                        break;
                    }
                }
                /* klocwork Fix */
                /* eICIC ue reconfig changes start */
                if(RRM_UE_RECONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT & p_msg->radio_resource_reconfig.presence_bitmask)
                {
                    p_ue_reconfig_req_data->m.meas_subframe_pattern_pcell_updated = 1;
                    p_meas_subframe_src = &(p_msg->radio_resource_reconfig.rrc_meas_subframe_pattern_pcell_r10);
                    p_meas_subframe_dest = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.rrc_meas_subframe_pattern_pcell_r10);

                    l3_memcpy_wrapper(p_meas_subframe_dest, p_meas_subframe_src, sizeof(rrc_meas_subframe_pattern_pcell_r10_t));
                }
                /* eICIC ue reconfig changes stop */
            }while(0);/* klocwork Fix */
        }
        /*Bug:8353 fix stop*/


        if(RRM_UE_RECONFIG_SCELL_CONFIG_PRESENT & p_msg->bitmask)
        {
            result = uecc_rrm_process_scell_config(p_ue_context,
                    &p_msg->scell_config);
        }

        if(RRC_FALSE == found_flag && RRC_FAILURE == result)
        {

            RRC_TRACE(RRC_INFO,"RRM Message incomplete:"
                    "Neither Phy_Dedicated nor MAC Reconfig parameters are present");
            result = RRC_FAILURE;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}



/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_procedure_constructor
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : Constructor for UE Reconfiguration Procedure(eNB Initiated)
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_procedure_constructor(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_return_t    result = RRC_FAILURE;
    U32    error_code = RRC_NO_ERROR;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
    RRC_TRACE(RRC_INFO,"UE Reconfiguration: Transaction Id updated");
#else
    uecc_ue_ctx_set_op_transction_id(p_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/

    /* Allocate memory for the curr_proc_data for UE Reconfig Procedure*/
    p_ue_context->p_curr_proc_data = (curr_proc_data_t*)
        rrc_mem_get(sizeof(curr_proc_data_t));

    if (PNULL == p_ue_context->p_curr_proc_data)
    {
        /* Memory Allocation Failed */
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                "[CURR_PROC_DATA] Memory Alocation failed(UE Reconfig)");
        error_code = RRC_MEMORY_ALLOCATION_FAILURE;
    }
    else
    {
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
        do {
            /* Start timer*/
            if (RRC_SUCCESS != 
                    uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_RRM_UE_RECONFIG_GUARD_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_WARNING,
                        "[UECC_RRM_UE_RECONFIG_GUARD_TIMER]"
                        "Timer Start Failed");
                error_code = RRC_INTERNAL_ERROR;
                break;
            }

            if (RRC_SUCCESS !=
                    uecc_fsm_rrm_ue_reconfig_process_rrm_ue_reconfig_req_msg
                    (p_ue_context, p_api))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_WARNING,
                        "[UECC_RRM_UE_RECONFIG_REQ]"
                        "Not a valid message");
                error_code = RRC_INVALID_RRM_MESSAGE_RECV;
                break;
            }

            RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
                    "UE Reconfiguration Request received from RRM");

            result = RRC_SUCCESS;
            /* Activate the first Activity now*/
        } while(0);
    }
    if (RRC_SUCCESS != result)
    {
        /* stop timer if it has been started */
        if (PNULL != 
                p_ue_context->timers_data.timers_ids[UECC_RRM_UE_RECONFIG_GUARD_TIMER])
        {
            /* Stop UE Reconfiguration procedural timer. */
            uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER, 
                    &p_ue_context->timers_data);
        }

        /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                    p_ue_context, FAILURE, error_code))
        {
            RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
        }

        /* Bug:9110 */
        /* Process failure*/
        /* Clear update configuration IEs flags */
        p_ue_context->m.physical_config_dedicated_updated = RRC_NULL;
        p_ue_context->m.mac_config_updated = RRC_NULL;
       /* eICIC ue reconfig changes start */
        p_ue_context->m.meas_subframe_pattern_pcell_updated = RRC_NULL;
       /* eICIC ue reconfig changes stop */
        uecc_event_queue_push_event(
                UECC_EV_RRM_UE_RECONFIG_FAILURE_INT,
                p_ue_context,
                p_api);
        /* Bug:9110 */
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* eICIC ue reconfig changes start */
/*********************************************************************************************************
 * Function Name  : updateMeassubframepatternPcellUeCtx 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  rrc_meas_subframe_pattern_pcell_r10_t* dest meas subframe pattern pcell
 *                  rrc_meas_subframe_pattern_pcell_r10_t* src meas subframe pattern pcell
 * Output         : None                 
 * Returns        : None
 * Description    : To update Physical Config Dedicated in UE Context
 * ********************************************************************************************************/
void updateMeassubframepatternPcellUeCtx(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        rrc_meas_subframe_pattern_pcell_r10_t*  p_meas_subframe_dest, /*dest meas subframe pattern pcell*/
        rrc_meas_subframe_pattern_pcell_r10_t*  p_meas_subframe_src /*src meas subframe pattern pcell*/
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_meas_subframe_dest);
    RRC_ASSERT(PNULL != p_meas_subframe_src);
    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:p_ue_context=%p",
                    p_ue_context);/*SPR 17777 +-*/
    if(p_meas_subframe_src->bitmask & MEAS_SUBFRAME_PATTERN_PCELL_R10_SETUP_PRESENT)
    {
        l3_memcpy_wrapper(&(p_meas_subframe_dest->meas_subframe_pattern_r10_setup),
                          &(p_meas_subframe_src->meas_subframe_pattern_r10_setup),
                          sizeof(rrc_phy_meas_subframe_pattern_r10_t));
        p_meas_subframe_dest->bitmask |= MEAS_SUBFRAME_PATTERN_PCELL_R10_SETUP_PRESENT;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* eICIC ue reconfig changes stop */


/*spr:8393/8162 fix start*/
/****************************************************************************
 * Function Name  : updatePhyConfigDedInUeCtx
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  rrc_phy_physical_config_dedicated_t* dest PHY config
 *                  rrc_phy_physical_config_dedicated_t* src PHY config
 * Output         : None                 
 * Returns        : None
 * Description    : To update Physical Config Dedicated in UE Context
 * ****************************************************************************/
void updatePhyConfigDedInUeCtx(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        rrc_phy_physical_config_dedicated_t*  p_phy_config_dest, /*dest PHY config*/
        rrc_phy_physical_config_dedicated_t*  p_phy_config_src /*src PHY config*/
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_phy_config_dest);
    RRC_ASSERT(PNULL != p_phy_config_src);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:p_ue_context=%p",
                    p_ue_context);/*SPR 17777 +-*/
    if(p_phy_config_src->bitmask & RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->pdsch_configuration_dedicated), 
                &(p_phy_config_src->pdsch_configuration_dedicated), sizeof(rrc_phy_pdsch_configuration_dedicated_t));
        p_phy_config_dest->bitmask |= RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->pucch_configuration_dedicated), 
                &(p_phy_config_src->pucch_configuration_dedicated), sizeof(rrc_phy_pucch_configuration_dedicated_t));
        p_phy_config_dest->bitmask |= RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->pusch_configuration_dedicated), 
                &(p_phy_config_src->pusch_configuration_dedicated), sizeof(rrc_phy_pusch_configuration_dedicated_t));
        p_phy_config_dest->bitmask |= RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->uplink_power_control_dedicated), 
                &(p_phy_config_src->uplink_power_control_dedicated), sizeof(rrc_phy_uplink_power_control_dedicated_t));
        p_phy_config_dest->bitmask |= RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->tpc_pdcch_config_pucch), 
                &(p_phy_config_src->tpc_pdcch_config_pucch), sizeof(rrc_phy_tpc_pdcch_configuration_t));
        p_phy_config_dest->bitmask |= RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->tpc_pdcch_config_pusch), 
                &(p_phy_config_src->tpc_pdcch_config_pusch), sizeof(rrc_phy_tpc_pdcch_configuration_t));
        p_phy_config_dest->bitmask |= RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->cqi_reporting), 
                &(p_phy_config_src->cqi_reporting), sizeof(rrc_phy_cqi_reporting_t));
        p_phy_config_dest->bitmask |= RRC_PHY_CQI_REPORTING_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->sounding_rs_ul_config_dedicated), 
                &(p_phy_config_src->sounding_rs_ul_config_dedicated), sizeof(rrc_phy_sounding_rs_ul_config_dedicated_t));
        p_phy_config_dest->bitmask |= RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_ANTENNA_INFORMATION_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->antenna_information), 
                &(p_phy_config_src->antenna_information), sizeof(rrc_phy_antenna_information_t));
        p_phy_config_dest->bitmask |= RRC_PHY_ANTENNA_INFORMATION_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->scheduling_request_config), 
                &(p_phy_config_src->scheduling_request_config), sizeof(rrc_phy_scheduling_request_config_t));
        p_phy_config_dest->bitmask |= RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->cqi_report_config_v920), 
                &(p_phy_config_src->cqi_report_config_v920), sizeof(rrc_phy_cqi_report_config_v920_t));
        p_phy_config_dest->bitmask |= RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
    }

    if(p_phy_config_src->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->antenna_info_v920), 
                &(p_phy_config_src->antenna_info_v920), sizeof(rrc_phy_antenna_info_v920_t));
        p_phy_config_dest->bitmask |= RRC_PHY_ANTENNA_INFO_V920_PRESENT;
    }

/*CA physical config updated ut fix*/
    if (p_phy_config_src->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->antenna_info_r10),
                &(p_phy_config_src->antenna_info_r10),
                sizeof(rrc_antenna_info_r10_t));
        p_phy_config_dest->bitmask |= RRC_PHY_ANTENNA_INFO_R10_PRESENT;
    }

    if (p_phy_config_src->bitmask & RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->antenna_info_ul_r10),
                &(p_phy_config_src->antenna_info_ul_r10),
                sizeof(rrc_antenna_info_ul_r10_t));
        p_phy_config_dest->bitmask |= RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT;
    }

    if (p_phy_config_src->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->cqi_report_config_r10),
                &(p_phy_config_src->cqi_report_config_r10),
                sizeof(rrc_cqi_report_config_r10_t));
        p_phy_config_dest->bitmask |= RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT;
    }

    if (p_phy_config_src->bitmask & RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT)
    {
        l3_memcpy_wrapper(&(p_phy_config_dest->phy_physical_config_dedicated_extended),
                &(p_phy_config_src->phy_physical_config_dedicated_extended),
                sizeof(rrc_phy_physical_config_dedicated_extended_t));
        p_phy_config_dest->bitmask |= RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT;
    }
/*CA physical config updated ut fix*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : updateMacConfigInUeCtx
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  rrm_mac_config_t* dest MAC config
 *                  rrm_mac_config_t* src MAC config
 * Output         : None                 
 * Returns        : None
 * Description    : To update MAC configuration in UE Context
 * ****************************************************************************/
void updateMacConfigInUeCtx(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        rrm_mac_config_t*   p_mac_config_dest, /*desc MAC config*/
        rrm_mac_config_t*   p_mac_config_src /*src MAC config*/
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_mac_config_dest);
    RRC_ASSERT(PNULL != p_mac_config_src);
    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:p_ue_context=%p",
                    p_ue_context);/*SPR 17777 +-*/

    /*MAC MAIN CONFIG*/
    if(p_mac_config_src->bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT)
    {
        p_mac_config_dest->bitmask |= RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT;

        if(p_mac_config_src->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT)
        {
            l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config.ul_sch_config), 
                    &(p_mac_config_src->mac_main_config.ul_sch_config), sizeof(rrc_ul_sch_config_t));
            p_mac_config_dest->mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT;
        }

        if(p_mac_config_src->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT)
        {
            l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config.drx_config), 
                    &(p_mac_config_src->mac_main_config.drx_config), sizeof(rrc_drx_config_t));
            p_mac_config_dest->mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT;
        }

        l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config.time_alignment_timer_dedicated), 
                &(p_mac_config_src->mac_main_config.time_alignment_timer_dedicated), sizeof(U8));

        if(p_mac_config_src->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT)
        {
            l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config.phr_config), 
                    &(p_mac_config_src->mac_main_config.phr_config), sizeof(rrc_phr_config_t));
            p_mac_config_dest->mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT;
        }

        if(p_mac_config_src->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT)
        {
            l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config.sr_prohibit_timer_r9), 
                    &(p_mac_config_src->mac_main_config.sr_prohibit_timer_r9), sizeof(U8));
            p_mac_config_dest->mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT;
        }
/*CA mac config updated ut fix*/
        if (p_mac_config_src->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_V1020_PRESENT)
        {
            l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config.mac_main_config_v1020), 
                    &(p_mac_config_src->mac_main_config.mac_main_config_v1020),
                    sizeof(rrc_mac_main_config_v1020_t));
            p_mac_config_dest->mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_V1020_PRESENT;
        }
	/*CA mac config updated ut fix*/
        /* SPR 21958 PUSCH RAT1 Support Start */
        if ( p_mac_config_src->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_UPLINK_RAT1_SUPPORT_INFO_PRESENT )
        {
            p_mac_config_dest->mac_main_config.is_uplink_rat1_supported = p_mac_config_src->mac_main_config.
                is_uplink_rat1_supported;
            p_mac_config_dest->mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_UPLINK_RAT1_SUPPORT_INFO_PRESENT;
        }
        /* SPR 21958 PUSCH RAT1 Support End */
    }

    /*MAC MAIN CONFIG EXTENSIONS*/
    p_mac_config_dest->mac_main_config_extensions.ue_priority = p_mac_config_src->mac_main_config_extensions.ue_priority;
    p_mac_config_dest->mac_main_config_extensions.dl_num_harq_process = p_mac_config_src->mac_main_config_extensions.dl_num_harq_process;
    p_mac_config_dest->mac_main_config_extensions.dl_i_mcs = p_mac_config_src->mac_main_config_extensions.dl_i_mcs;
    p_mac_config_dest->mac_main_config_extensions.dl_max_rb = p_mac_config_src->mac_main_config_extensions.dl_max_rb;
    p_mac_config_dest->mac_main_config_extensions.max_ul_harq_tx = p_mac_config_src->mac_main_config_extensions.max_ul_harq_tx;
    p_mac_config_dest->mac_main_config_extensions.ul_i_mcs = p_mac_config_src->mac_main_config_extensions.ul_i_mcs;
    p_mac_config_dest->mac_main_config_extensions.num_of_layer = p_mac_config_src->mac_main_config_extensions.num_of_layer;
    p_mac_config_dest->mac_main_config_extensions.code_book_index = p_mac_config_src->mac_main_config_extensions.code_book_index;

    if(p_mac_config_src->mac_main_config_extensions.bitmask & RRM_MAC_MAIN_CONFIG_EXT_PC_MAX_PRESENT)
    {
        l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config_extensions.pc_max), 
                &(p_mac_config_src->mac_main_config_extensions.pc_max), sizeof(S8));
        p_mac_config_dest->mac_main_config_extensions.bitmask |= RRM_MAC_MAIN_CONFIG_EXT_PC_MAX_PRESENT;
    }

    if(p_mac_config_src->mac_main_config_extensions.bitmask & RRM_MAC_MAIN_CONFIG_EXT_USER_LOCATION_TYPE_PRESENT)
    {
        l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config_extensions.user_location_type), 
                &(p_mac_config_src->mac_main_config_extensions.user_location_type), sizeof(U8));
        p_mac_config_dest->mac_main_config_extensions.bitmask |= RRM_MAC_MAIN_CONFIG_EXT_USER_LOCATION_TYPE_PRESENT;
    }

    if(p_mac_config_src->mac_main_config_extensions.bitmask & RRM_MAC_MAIN_CONFIG_EXT_UL_MU_MIMO_INFO_PRESENT)
    {
        l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config_extensions.ul_mu_mimo_status), 
                &(p_mac_config_src->mac_main_config_extensions.ul_mu_mimo_status), sizeof(U8));
        p_mac_config_dest->mac_main_config_extensions.bitmask |= RRM_MAC_MAIN_CONFIG_EXT_UL_MU_MIMO_INFO_PRESENT;
    }

    if(p_mac_config_src->mac_main_config_extensions.bitmask & RRM_MAC_MAIN_CONFIG_EXT_MIN_UE_POWER_PRESENT)
    {
        l3_memcpy_wrapper(&(p_mac_config_dest->mac_main_config_extensions.min_ue_power), 
                &(p_mac_config_src->mac_main_config_extensions.min_ue_power), sizeof(S8));
        p_mac_config_dest->mac_main_config_extensions.bitmask |= RRM_MAC_MAIN_CONFIG_EXT_MIN_UE_POWER_PRESENT;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*spr:8393/8162 fix stop*/



/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_procedure_finish_success
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : Success handler for UE UE Reconfiguration procedure
 *                  (eNB Initiated) Calls on procedure's finishing
 * ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
/*SPR_16942_START*/
    /*spr:8393/8162 fix start*/
    rrc_phy_physical_config_dedicated_t*  p_phy_config = PNULL;
    rrm_mac_config_t *p_mac_config = PNULL; 
/*SPR_16942_END*/    

    /* eICIC changes start */
    rrc_meas_subframe_pattern_pcell_r10_t* p_meas_subframe_dest = PNULL; 
    rrc_meas_subframe_pattern_pcell_r10_t* p_meas_subframe_src = PNULL; 
    /* eICIC changes stop */

    rrm_ue_reconfig_request_data_t *p_data = PNULL ;
    U8 scell_index = RRC_NULL ;
    U8 loop = RRC_NULL ;
    /*spr:8393/8162 fix stop*/

    /* eICIC changes start */
    p_meas_subframe_dest = &(p_ue_context->rrc_meas_subframe_pattern_pcell_r10);
    p_meas_subframe_src  = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.rrc_meas_subframe_pattern_pcell_r10);
    /* eICIC changes stop */

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UE Reconfig Procedure - Success Handler");

    /*spr:8393/8162 fix start*/
    if(1 == p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.m.physical_config_dedicated_updated)
    {
/*SPR_16942_START*/
        p_phy_config = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.physical_config_dedicated);
        /*Update PHY_CONFIG_DED in ue_context*/
        updatePhyConfigDedInUeCtx(p_ue_context, &(p_ue_context->physical_config_dedicated), p_phy_config);
/*SPR_16942_END*/    
    }

    if(1 == p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.m.mac_config_updated)
    {
/*SPR_16942_START*/
      p_mac_config = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.mac_config);
/*Code Removed*/       
        /*Update MAC_CONFIG in ue_context*/
       uecc_update_mac_config_in_ue_context(p_ue_context,p_mac_config);
/*SPR_16942_END*/        
    }
    /*spr:8393/8162 fix stop*/
    /* eICIC ue reconfig changes start */
    if(1 == p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.m.meas_subframe_pattern_pcell_updated)
    {
        /*Update MEAS_SUBFRAME_PATTERN_PCELL in ue_context*/
        updateMeassubframepatternPcellUeCtx(p_ue_context, p_meas_subframe_dest, p_meas_subframe_src);
        p_ue_context->m.meas_subframe_pattern_pcell_present = 1;
    }
    /* eICIC ue reconfig changes stop */

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = RRC_NULL;
    p_ue_context->m.mac_config_updated = RRC_NULL;
    /* eICIC ue reconfig changes start */
    p_ue_context->m.meas_subframe_pattern_pcell_updated = RRC_NULL;
    /* eICIC ue reconfig changes stop */
    /* CA start */
    p_data = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data) ;
    /* SPR 20418 FIX START*/
    if((PNULL != p_data) &&
         (1 == p_data->m.sps_config_present))
    {
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_ue_context->p_gb_context,
                p_ue_context,
                RRC_SUCCESS);
    }
    /* SPR 20418 FIX END*/

    if((PNULL != p_data) &&
            (RRC_TRUE == p_data->m.scell_config_updated ))
    {
        if(PNULL == p_ue_context->p_scell_config)
        {
            p_ue_context->p_scell_config =
                rrc_mem_get(sizeof(rrm_scell_config_t));
            /* SES-508 Fix Start */
            if(PNULL == p_ue_context->p_scell_config)
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                    "Memory Allocation Failed to store scell_config");
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
            /* SES-508 Fix End */
        }
        rrc_rrm_update_ue_ctx_scell_config(p_data->
                p_scell_config,p_ue_context);
        /*Bug_12726_fix_start*/
        if (RRC_NULL == 
                p_ue_context->p_scell_config->scell_add_mod_list.count)
        {
            rrc_mem_free(p_ue_context->p_scell_config);
            p_ue_context->p_scell_config = PNULL;
        }
        /*Bug_12726_fix_stop*/
        for(loop = RRC_NULL; loop < p_data->p_scell_config->scell_add_mod_list.count ; loop++)
        {
            scell_index = p_data->p_scell_config->scell_add_mod_list.
                rrc_scell_to_add_mod[loop].scellIndex;

            p_ue_context->scells_operation_list[scell_index].operation_type = CLEAR_FLAG; 
        }
    }

    /* Notify UE Main FSM about failure */
    /* Post event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_RRM_UE_RECONFIG_SUCCESS_INT,
            p_ue_context,
            p_api);

    /* Deallocate memory previously allocated for UE Reconfiguration
     * procedure in the UE Context */
    
    if(PNULL != 
            p_data->p_scell_config)
    {
        p_data->m.scell_config_updated = RRC_FALSE;
        rrc_mem_free(
                p_data->p_scell_config);
        p_data->p_scell_config = PNULL;
    }

    /* Stop Procedure */
    /* Bug 175 */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    /* Bug 175 */
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_procedure_finish_fail
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : Failure handler for UE Reconfiguration procedure
 *                  (ENB Initiated)Calls on procedure's finishing with failure
 ** ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_procedure_finish_fail(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{

/*SPR 20636 Changes Start*/
    uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/*SPR 20636 Changes Start*/
    rrm_ue_reconfig_request_data_t *p_data = PNULL ;
    U8 cell_index = RRC_NULL ;
    U8 loop = RRC_NULL ;
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UE Reconfig Procedure - Failure Handler");

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    /*Bug 767 Fix start*/
    if ( (PNULL != p_ue_context->p_curr_proc_data) &&
          (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd) )
    {
        if ( (UECC_EV_ERB_ONGOING_HO_REQUIRED_INT == failure_ev) &&
                (RRC_HO_REQUIRED_MSG ==  p_ue_context->p_curr_proc_data->msg_bfr_type) )
        {
            p_api = rrc_mem_get(sizeof(rrc_rrm_ho_required_t));
            /* coverity_fix_61965 start */
            if (PNULL == p_api)
            {
                /* Memory Allocation Failed */
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                        "[uecc_fsm_rrm_ue_reconfig_procedure_finish_fail] Memory Alocation failed");
                return;            
            }
            /* coverity_fix_61965 stop */
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */

           l3_memcpy_wrapper(p_api, p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                           sizeof(rrc_rrm_ho_required_t));
       }
    }
    /*Bug 767 Fix stop*/

    /* CA start */
    /* If the dynamic UE reconfig procedure failed, 
     * then the chk_cellid_exist flag that was turned on 
     * at the start of procedure corresponding to the 
     * add operation receievd for that scell is now 
     * turned off due to the failure    
     */
    p_data = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data) ;

    if((PNULL != p_data ) &&
       (RRC_TRUE == p_data->m.scell_config_updated ))
    {
        for(loop = RRC_NULL; loop < p_data->p_scell_config->scell_add_mod_list.count ; loop++)
      {
          cell_index = p_data->p_scell_config->scell_add_mod_list.
                       rrc_scell_to_add_mod[loop].cell_index;

          if((p_ue_context->scells_operation_list[cell_index].operation_type == 0 )  &&
             (p_ue_context->scells_operation_list[cell_index].chk_cellid_exist == RRC_TRUE))
          {
              p_ue_context->scells_operation_list[cell_index].chk_cellid_exist = RRC_FALSE ;
          }    

        }
    } 
   
    /* Freeing the memory allocated to p_scell_config in p_curr_proc_data */
    /*klokwork fix*/
    if((PNULL != p_data) && (PNULL != p_data->p_scell_config))
    {
        p_ue_context->m.scell_config_present = RRC_FALSE;
        rrc_mem_free(p_data->p_scell_config);
        p_data->p_scell_config = PNULL;

    }    
    /*klokwork fix*/

    /* SPR 9084 Fix Start */
    if (UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT != failure_ev)
    {
        /* Notify UE Main FSM about failure */
        /* Post event to UE Main FSM */
        uecc_event_queue_push_event(
            failure_ev,
            p_ue_context,
            p_api);
    }
    /* SPR 9084 Fix End */
/* Bug 175 */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
/* Bug 175 */
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_ue_release_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    :  This is the handler function for the message
 *                   UECC_EV_UE_RELEASE_REQ
 *
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    U32    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
    rrc_s1ap_ue_context_release_command_t *p_ue_context_release_command 
        = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_UE_RELEASE_REQ");

    /* Stop UE Reconfiguration procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER, 
            &p_ue_context->timers_data);

    p_ue_context_release_command = (rrc_s1ap_ue_context_release_command_t*)
        p_api;

    /* Store data in UE context for future processing */
    p_ue_context->share_data.uecc_rcr_data.u.cause =
        p_ue_context_release_command->cause;//check

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message Delivery failure");
    }


  /*UE_Reconfig Crash Fix Start*/
    /* Notify procedure about failure */    
    /*uecc_fsm_finish_activity_failure(
            p_ue_context, 
            (void*)UECC_EV_UE_RELEASE_REQ_INT);*/

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);


    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASE_REQ_INT,
        p_ue_context,
        p_api);
  /*UE_Reconfig Crash Fix End*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_rrm_ue_release_ind_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_rrm_ue_release_ind_handler(
        uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
        void*               p_api          /* Input API buffer */
        )
{
    U32 error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RRM_UE_CONNECTION_RELEASE_IND");

    /* Stop UE Reconfiguration procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER,
            &p_ue_context->timers_data);

    /*check - do we need to send failure to RRM in this case. Ideally it
     * shouldn't be send. It depends on hanlding in RRM */
    /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message Delivery failure");
    }

    /* Notify procedure about failure */
    /* Bug 9110 */

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;

    /* SPR 9084 Fix Start */
    uecc_event_queue_push_event(
                UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT,
                p_ue_context,
                p_api);
    /* SPR 9084 Fix End */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }
    uecc_fsm_finish_active_procedure(p_ue_context);
    /* Bug 9110 */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_erab_setup_req_discard_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description   : This function discards the ERAB Setup Req message
 ******************************************************************************/
void uecc_fsm_rrm_ue_reconfig_erab_setup_req_discard_handler(
        uecc_ue_context_t*  p_ue_context, /* UECC UE context */
        void*               p_api         /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* In this function No action is taken for simply discard
       the received message */

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,
            "[UECC_EV_S1AP_ERAB_SETUP_REQ]" 
            " RRM Initiated UE Reconfiguration Procedure Ongoing"
            " Discarding E-RAB SetUp Req Received from MME");

    //check - do we need to send Erab Setup Response with Fail_Cause to MME

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_erab_modify_req_discard_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This function discards the ERAB Modify Req message
 ******************************************************************************/
void uecc_fsm_rrm_ue_reconfig_erab_modify_req_discard_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* In this function No action is taken for simply discard 
       the received message */

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,
            "[UECC_EV_S1AP_ERAB_MODIFY_REQ]" 
            "RRM Initiated UE Reconfiguration Procedure Ongoing"
            "Discarding E-RAB Modify Req Received from MME");

    //check - do we need to send Erab Setup Response with Fail_Cause to MME

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_erab_rel_cmd_discard_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This function discards the ERAB Release command
 ******************************************************************************/
void uecc_fsm_rrm_ue_reconfig_erab_rel_cmd_discard_handler(
        uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
        void*               p_api          /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* In this function No action is taken for simply discard 
       the received message */

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,
            "[UECC_EV_S1AP_ERAB_RELEASE_COMMAND]" 
            "RRM Initiated UE Reconfiguration Procedure Ongoing"
            "Discarding E-RAB Release Cmd Received from MME");

    //check - do we need to send Erab Setup Response with Fail_Cause to MME

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_erab_rel_ind_discard_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                      UECC_EV_RRM_ERAB_RELEASE_IND
 ******************************************************************************/
void uecc_fsm_rrm_ue_reconfig_erab_rel_ind_discard_handler(
        uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
        void*               p_api          /* Input API buffer */
        )
{
    U32 error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "Received: RRC_RRM_ERAB_RELEASE_IND.");

    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_ind_fail_resp(
                p_api,
                p_ue_context,
                FAILURE,
                error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_RELEASE_IND_CNF] Message Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_s1ap_reset_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_s1ap_reset_handler(
        uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
        void*               p_api          /* Input API buffer */
        )
{
    U32    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_S1AP_RESET_MSG_IND");

    /* Stop UE Reconfiguration procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER, 
            &p_ue_context->timers_data);

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message Delivery failure");
    }

    /* Bug:9110 */
    /* Notify procedure about failure */
    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;

    uecc_event_queue_push_event(
                UECC_EV_S1AP_RESET_MSG_IND_INT,
                p_ue_context,
                p_api);
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    uecc_fsm_finish_active_procedure(p_ue_context);
    /* Bug:9110 */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_csc_delete_all_ue_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ
 *
 ***************************************************************************/
void uecc_fsm_rrm_ue_reconfig_csc_delete_all_ue_req_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_CSC_DELETE_ALL_UE_REQ");

    /* Stop UE Reconfiguration procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER, &p_ue_context->timers_data);

    /* Process failure*/
    /* Bug:9110 */

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    uecc_event_queue_push_event(
                UECC_EV_CSC_CELL_DELETE_REQ_INT,
                p_ue_context,
                p_api);
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    uecc_fsm_finish_active_procedure(p_ue_context);
    /* Bug:9110 */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_init_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_fail_type2_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /* Build and send RRCConnectionReconfiguration */
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_rrm_ue_reconfig_rrc_connection_reconfiguration(
                p_ue_context))
    {
        /* Stop UE Reconfiguration procedural timer. */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER,
                &p_ue_context->timers_data);

        /* unrecoverable error from LLIM: Release UE Context */
        p_fail_actions_list =
            uecc_fsm_ue_reconfig_fail_type2_actions; //check. here we've to rollback
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                p_fail_actions_list);

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_complete_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_complete_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    rrc_return_t  rrc_response = RRC_FAILURE;
    U32    error_code = RRC_NO_ERROR;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE");

    /* Stop UE Reconfiguration procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER, &p_ue_context->timers_data);

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with SUCCESS to RRM */
    rrc_response = uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, SUCCESS, error_code);
       
    if (RRC_SUCCESS != rrc_response)
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }
    else
    {
        /* Call success handler for procedure */
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    uecc_llim_check_and_send_rrc_ue_information(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_init_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the event UECC_EV_STATE_INIT
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_MODIFY_LC_REQ msg to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_modify_lc_req(p_ue_context))
    {
        /* Process failure according to selected failure actions list */
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_MODIFY_LC_REQ] Delivery Failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_resp_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_resp_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    rrc_return_t  result = RRC_FAILURE;

    rrc_uecc_llim_modify_lc_resp_t* p_rrc_uecc_llim_modify_lc_resp = PNULL;
    uecc_fsm_fail_handler_f const* p_fail_actions_list = 
        uecc_fsm_ue_reconfig_fail_type2_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_LLIM_RECONFIGURE_UE_RESP");

    p_rrc_uecc_llim_modify_lc_resp = (rrc_uecc_llim_modify_lc_resp_t*)p_api;

    do{
        /* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
         * received with success or failure or partial success */
        switch(p_rrc_uecc_llim_modify_lc_resp->response_code)
        {
            case SUCCESS:
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
                        "LLIM has modified radio resources successfully");

                /* Switch  state */
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing, PNULL);

                result = RRC_SUCCESS;
                break;

            case L3_FATAL:
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                        "Radio resources modification unsuccessful at LLIM");

                /* Stop UE Reconfiguration procedural timer. */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER,
                        &p_ue_context->timers_data);

                /* unrecoverable error from LLIM: Release UE Context */
                  p_fail_actions_list =
                      uecc_fsm_ue_reconfig_fail_type2_actions;
                p_ue_context->p_curr_proc_data->u.
                   rrm_ue_reconfig_request_data.modify_lc_resp_code = L3_FATAL;

                if(p_rrc_uecc_llim_modify_lc_resp->bitmask & 
                     UECC_LLIM_MODIFY_LC_RESP_FAIL_CAUSE_PRESENT )
                {
                    p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.
                              fail_cause = p_rrc_uecc_llim_modify_lc_resp->fail_cause;
                    p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.
                                      m.fail_cause_updated = 1 ;
                }
                result = RRC_FAILURE;
                break;

            case FAILURE:    
            case PARTIAL_SUCCESS:
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
                        "Radio resources modification unsuccessful at LLIM");

                /* Stop UE Reconfiguration procedural timer. */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER,
                        &p_ue_context->timers_data);
                 p_fail_actions_list =
                     uecc_fsm_ue_reconfig_fail_type3_actions;
                p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.modify_lc_resp_code = 
                    p_rrc_uecc_llim_modify_lc_resp->response_code;

                if(p_rrc_uecc_llim_modify_lc_resp->bitmask &
                       UECC_LLIM_MODIFY_LC_RESP_FAIL_CAUSE_PRESENT )
                {
                    p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.
                         fail_cause = p_rrc_uecc_llim_modify_lc_resp->fail_cause;
                    p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.
                            m.fail_cause_updated = 1 ;
                }
 
                result = RRC_FAILURE;
                break;

            default:
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                        "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] Invalid Response=%u",
                        p_rrc_uecc_llim_modify_lc_resp->response_code);

                /* Stop UE Reconfiguration procedural timer. */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                          UECC_RRM_UE_RECONFIG_GUARD_TIMER,
                        &p_ue_context->timers_data);

                /* Unrecoverable error from LLIM: Release UE Context */
                p_fail_actions_list =
                    uecc_fsm_ue_reconfig_fail_type2_actions;
                result = RRC_FAILURE;
                break;
        }
    } while(0);

    if (RRC_SUCCESS != result)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                p_fail_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  :uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_reestablish_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ over CCCH. 
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_reestablish_req_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
	/* coverity_fix_62125_start */
	U16     len = RRC_NULL;
	/* SPR 16053 Fix Start */
	/* CODE REMOVED */
    	/* SPR 16053 Fix Start */
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);


	RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
			"UECC_EV_RRC_RE_ESTABLISH_REQ");

    	/* SPR 16053 Fix Start */
    	/* CODE REMOVED */
    	/* SPR 16053 Fix Stop */

	/* Store the Re-establish message in the UE Context */
	len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
	if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = rrc_mem_get(len)))
	{
		RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
				"Memory Allocation Failed "
				"to store Reestablishment Req msg");
		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}

	/* storing re_establishment contents into msg_bfr_rcvd */
	l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
			p_api, len);

	p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;


	/* Switch state */
	UECC_FSM_SET_CHILD_STATE(p_ue_context,
			&uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing,
			PNULL);
	
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_62125_stop */



/****************************************************************************
 * Function Name  :uecc_fsm_rrm_ue_reconfig_llim_reconfig_ho_required_ind_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * DESCRIPTION   : This is the handler function for the event 
 *                   UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_ho_required_ind_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    U16     len = RRC_NULL;
    uecc_ue_timer_id_et uecc_ue_timer_id = UECC_UE_TIMER_LAST;
    rrm_ue_reconfig_request_data_t *p_reconfig_data = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    /*
     ** Start the HO PREP TIMER
     ** Change the state to UECC_UE_RECONFIG_LLIM_RECONFIG_ROLLBACK_ONGOING
     */
    if (PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                "p_ue_context->p_curr_proc_data is NULL"
                "hence ignoring");
        /* SPR 24021/SES-452 Fix Start */
        if(PNULL != p_api)
        {
            rrc_mem_free(p_api);
            p_api = PNULL;
        }
        /* SPR 24021/SES-452 Fix End */
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    if (RRC_NO_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type)
    {
        p_reconfig_data = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data);

        if (PNULL != p_reconfig_data)
        {        
            if ((RRC_TRUE == p_reconfig_data->m.scell_config_updated) && 
                (p_reconfig_data->p_scell_config->bitmask & RRM_SCELL_RELEASE_LIST_PRESENT))
            { 
                /* SEND HO FAILURE TO RRM */
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = 
                    s1ap_interaction_with_other_procedure;
                /* Send the HO Failure Message to RRM */
                uecc_rrm_build_and_send_ho_failure(p_ue_context);

            }  
            else
            {
                /* Buffer the HO REQUIRED message in UE Context. */
                len = (U16)sizeof(rrc_rrm_ho_required_t);

                if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                            rrc_mem_get(len))) 
                {
                    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                            "memory allocation failed to store received "
                            "RRC_RRM_HO_REQUIRED");
                    /* SPR 24021/SES-452 Fix Start */
                    if(PNULL != p_api)
                    {
                        rrc_mem_free(p_api);
                        p_api = PNULL;
                    }
                    /* SPR 24021/SES-452 Fix End */
                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return;
                }

                /* storing rrc_rrm_ho_required contents into msg_bfr_rcvd */
                l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                        p_api, len);

                /* store received buffer type */
                p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;


                /*x2_ho_timer*/
                if (HANDOVER_TYPE_INTRA_LTE_X2 ==
                        ((rrc_rrm_ho_required_t*)p_api)->ho_type)
                {
                    p_ue_context->ho_info.s1_or_x2_handover = X2_HO;
                    uecc_ue_timer_id = UECC_UE_X2_HO_PREP_TIMER;
                }
                else if(HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
                {
                    p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO;
                    uecc_ue_timer_id = UECC_UE_X2_HO_PREP_TIMER;
                }
		/*BUG 604 changes start*/
		else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
		{    
			p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
			uecc_ue_timer_id = UECC_UE_X2_HO_PREP_TIMER;
		}    
		/*BUG 604 changes stop*/
		else
                {
                    p_ue_context->ho_info.s1_or_x2_handover = S1_HO;
                    uecc_ue_timer_id = UECC_UE_HO_PREP_TIMER;
                }

                if (RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context,
                            uecc_ue_timer_id,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
                {
                    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                            "[%u] HO Prep Timer start Failure", uecc_ue_timer_id);
                }
                else
                {
                    /* Switch state */
                    UECC_FSM_SET_CHILD_STATE(
                            p_ue_context,
                            &uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing,
                            PNULL);


                }

            }
        }
        
    }    
    else
    {
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);
    }
    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_timer_expiry_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_fail_type1_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "LLIM Reconfiguration Ongoing: UECC_EV_TIMER_EXPIRY");

    p_fail_actions_list = uecc_fsm_ue_reconfig_fail_type1_actions;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* SPR 16053 Fix Start */

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_ongoing_radio_link_failure_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_ongoing_radio_link_failure_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    /* SPR 16053 Fix Start */
    U32    error_code = RRC_RADIO_LINK_FAILURE_TRIGGERED;
    /* SPR 16053 Fix Stop */

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RADIO_LINK_FAILURE_IND");

    /* Stop UE Reconfiguration procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER,
            &p_ue_context->timers_data);

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }

    /* Process failure handler for procedure */
    /* Bug:9110 */

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    uecc_event_queue_push_event(
                UECC_EV_RADIO_LINK_FAILURE_INT,
                p_ue_context,
                p_api);
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    uecc_fsm_finish_active_procedure(p_ue_context);
    /* Bug:9110 */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* SPR 16053 Fix End */
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_timer_expiry_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for timer expiry event
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_fail_type1_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "LLIM Reconfiguration Ongoing: UECC_EV_TIMER_EXPIRY");

    p_fail_actions_list = uecc_fsm_ue_reconfig_fail_type1_actions;
    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ho_required_ind_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * DESCRIPTION   :   This is the handler function for the message
 *                   UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ho_required_ind_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    U16     len = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context); 
    RRC_ASSERT(PNULL != p_ue_context);

    if (RRC_NO_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type)
    {
        /* Buffer the HO REQUIRED message in UE Context. */
        len = (U16)sizeof(rrc_rrm_ho_required_t);
        if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                    rrc_mem_get(len))) 
        {
            RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                    "memory allocation failed to store received "
                    "RRC_RRM_HO_REQUIRED");
            /* SPR 24021/SES-452 Fix Start */
            if(PNULL != p_api)
            {
                rrc_mem_free(p_api);
                p_api = PNULL;
            }
            /* SPR 24021/SES-452 Fix End */

            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
        }

        /* storing rrc_rrm_ho_required contents into msg_bfr_rcvd */
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api, len);

        /* store received buffer type */
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

        /* Start the HO PREP TIMER */
        /*x2_ho_timer*/
        if (HANDOVER_TYPE_INTRA_LTE_X2 ==
                ((rrc_rrm_ho_required_t*)p_api)->ho_type) 
        {
            p_ue_context->ho_info.s1_or_x2_handover = X2_HO; 
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER]X2_HO Prep Timer start Failure");
            }
        }
        else if(HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO; 
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER]INTER_CELL_HO Prep Timer start Failure");
            }
        }
/*BUG 604 changes start*/
        else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO; 
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER]INTRA_CELL_HO Prep Timer start Failure");
            }
        }
/*BUG 604 changes stop*/
        else
        {
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO;
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
        /* check - whether we need to move to ROLLBACK state here
         * Reference is taken from uecc_modfsm.c */
    }
    else
    {
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);
    }
    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_llim_rollback_ongoing_init_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the event
 *                  UECC_EV_STATE_INIT
 ****************************************************************************/
void uecc_fsm_ue_reconfig_llim_rollback_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_fail_type2_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_MODIFY_ROLLBACK_REQ msg to LLIM */
    if (RRC_SUCCESS != 
            uecc_llim_build_and_send_modify_rollback_req(p_ue_context))
    {
        /* Here, the UE context must be released!! */
        /* Process failure according to selected failure actions list */
        p_fail_actions_list = uecc_fsm_ue_reconfig_fail_type2_actions;
        uecc_fsm_start_failure_processing(p_ue_context, p_fail_actions_list);
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_llim_modify_rollback_resp_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the event
 *                  UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_ue_reconfig_llim_modify_rollback_resp_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    
    rrc_return_et rrc_response = RRC_FAILURE;
    U32           error_code   = RRC_NO_ERROR;
    uecc_fsm_event_et event = UECC_EV_LAST;

    rrc_uecc_llim_modify_rollback_resp_t* p_rrc_uecc_llim_modify_rollback_resp = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    

    p_rrc_uecc_llim_modify_rollback_resp = 
        (rrc_uecc_llim_modify_rollback_resp_t*)p_api;

    /* Stop Procedure Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER, 
            &p_ue_context->timers_data);

    switch (p_rrc_uecc_llim_modify_rollback_resp->response)
    {
        case RRC_SUCCESS:
            rrc_response = RRC_SUCCESS;
            if (p_ue_context->p_curr_proc_data->msg_bfr_type != RRC_NO_MSG)
            {
                /* Start the Procedure as per the buffered Message */
                switch(p_ue_context->p_curr_proc_data->msg_bfr_type)
                {
                    /* Post Internal event as per the buffered message type*/
                    case RRC_RE_ESTABLISH_MSG:
                        error_code = RRC_REESTABLISHMENT_TRIGGERED;
                        event = UECC_EV_RRC_RE_ESTABLISH_REQ_INT;
                        break;
                    case RRC_HO_REQUIRED_MSG:
                        error_code = RRC_HANDOVER_TRIGGERED;
                        event = UECC_EV_ERB_ONGOING_HO_REQUIRED_INT;
                        break;
                    case RRC_RLF_MSG:
                        error_code = RRC_RADIO_LINK_FAILURE_TRIGGERED;
                        event = UECC_EV_RADIO_LINK_FAILURE_INT;
                        break;
                    default:
                        error_code = RRC_INTERNAL_ERROR;
                        rrc_response = RRC_FAILURE;
                         RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, 
                            "Unrecognized Message Encountered");
                        break;
                }
            }
            break;
        case RRC_FAILURE:
            switch (p_ue_context->p_curr_proc_data->msg_bfr_type)
            {
                case RRC_HO_REQUIRED_MSG:
                    error_code = RRC_HANDOVER_ROLLBACK_TRIGGERED_FAILURE;

                    /*x2_ho_timer*/
                    /* Stop Procedure Timer */
                    if  ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
/*BUG 604 changes start*/
                             (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                             (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
/*BUG 604 changes stop*/
                    {
                        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                           UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
                        p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
                    }
                    else
                    {
                        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                            UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
                        p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
                    }

                    /* SEND HO FAILURE TO RRM */
                    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                    p_ue_context->ho_info.cause.u.radioNetwork =
                        s1ap_unspecified_2;

                    /* Send the HO Failure Message to RRM */
                    uecc_rrm_build_and_send_ho_failure(p_ue_context);

                    break;
                case RRC_RE_ESTABLISH_MSG:
                    error_code = 
                        RRC_REESTABLISHMENT_TRIGGERED_ROLLBACK_FAILURE;
                    break;
                case RRC_RLF_MSG:
                    error_code =
                        RRC_RADIO_LINK_FAILURE_TRIGGERED_ROLLBACK_FAILURE;
                    break;
                default:
                    error_code = RRC_INTERNAL_ERROR;
                    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_WARNING,
                        "Invalid Error Code Received");
                    break;
            }
            break;
        default:
             /* Error*/
             error_code = RRC_INTERNAL_ERROR;
             RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_BRIEF, 
                "Unrecognized Response Received");
            break;
    }

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }
        
    if (rrc_response == RRC_SUCCESS)
    {
        /*SPR:7756 start*/
        //uecc_fsm_finish_activity_failure(p_ue_context, (void*)event);

        /* Notify procedure about failure with specified event
           to be posted in Main FSM Queue */
   /*SPR 8155 Fix start*/ 
    uecc_event_queue_push_event(
                event,
                p_ue_context,
                p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
   /*SPR 8155 Fix stop*/ 
        /* Deallocate memory previously allocated for UE Reconfiguration
         * procedure in the UE Context */
        if (PNULL != p_ue_context->p_curr_proc_data)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
        }

        /* Stop Procedure */
        uecc_fsm_finish_active_procedure(p_ue_context);

        /*SPR:7756 stop*/
    }
    else
    {
        if(p_ue_context->p_curr_proc_data->msg_bfr_type ==
                RRC_RE_ESTABLISH_MSG)
        {
            /* Send RRC-Connection Reject to UE */
            if (RRC_SUCCESS !=
                    uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                        p_ue_context->p_gb_context, p_ue_context->crnti, otherFailure,
                        p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "Fail to send the RRCConnectionReestablishmentReject to UE");
            }
            else
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
                        "RRCConnectionReestablishmentReject Send to UE Successfully");
            }
            /* enabling flag */
            SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
        }

        /* Bug:9110 */
        /* Notify procedure about failure */
        /* Clear update configuration IEs flags */
        p_ue_context->m.physical_config_dedicated_updated = 0;
        p_ue_context->m.mac_config_updated = 0;
        uecc_event_queue_push_event(
                UECC_EV_RRM_UE_RECONFIG_FAILURE_REL_UE_CTX_INT,
                p_ue_context,
                p_api);
        uecc_fsm_finish_active_procedure(p_ue_context);
        /* Bug:9110 */
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_llim_rollback_ongoing_timer_expiry_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the event
 *                  UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_ue_reconfig_llim_rollback_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_fail_type1_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "LLIM Reconfiguration Ongoing: UECC_EV_TIMER_EXPIRY");

    p_fail_actions_list = uecc_fsm_ue_reconfig_fail_type1_actions;
    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_ue_reconfig_rrc_re_establish_request_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the event
 *                  UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_ue_reconfig_rrc_re_establish_request_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    /* Event handling will be similar in current state
     * as it was there in LLIM Reconf Ongoing state, except
     * here no state transition to llim_rollback will occur
     * as its already there in that state
     */
    U16     len = RRC_NULL;
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_rrc_reestablish_actions;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RRC_RE_ESTABLISH_REQ");

    p_fail_actions_list = uecc_fsm_ue_reconfig_rrc_reestablish_actions;

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);


    /* Store the Re-establish message in the UE Context */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                rrc_mem_get(len)))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed "
                "to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api, len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongonig_ho_required_ind_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRM_HO_REQUIRED                         
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongonig_ho_required_ind_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    U16     len = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    if (RRC_NO_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type)
    {
        len = (U16)sizeof(rrc_rrm_ho_required_t);
        if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                    rrc_mem_get(len)))
        {
            RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL, 
                    "memory allocation failed to store received "
                    "RRC_RRM_HO_REQUIRED");
            /* SPR 24021/SES-452 Fix Start */
            if(PNULL != p_api)
            {
                rrc_mem_free(p_api);
                p_api = PNULL;
            }
            /* SPR 24021/SES-452 Fix End */
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
        }

        /* storing RRC_HO_REQUIRED_MSG contents into msg_bfr_rcvd */
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api,len);

        /* store received buffer type */
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

        /* Start the HO PREP TIMER */
        /*x2_ho_timer*/
        if (HANDOVER_TYPE_INTRA_LTE_X2 ==
                ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = X2_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] X2_HO Prep Timer start Failure");
            }
        }
        else if(HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTER_CELL_HO Prep Timer start Failure");
            }
        }
/*BUG 604 changes start*/
        else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTRA_CELL_HO Prep Timer start Failure");
            }
        }
/*BUG 604 changes stop*/
        else
        {
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
    }
    else
    {
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);
    }
    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* CA start */
/****************************************************************************
 * Function Name  :uecc_fsm_rrm_ue_reconfig_llim_reconfig_radio_link_failure_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                   - Pointer to the UE Context in UECC 
 *                  void*               p_api
 *                    - Pointer to the radio_link_failure_ind 
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND over CCCH. 
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_radio_link_failure_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    U16     len = RRC_NULL;
    
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
        uecc_fsm_ue_reconfig_rlf_actions;

    rrm_ue_reconfig_request_data_t *p_reconfig_data = PNULL;
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_LLIM_RADIO_LINK_FAILURE_IND");

    p_fail_actions_list = uecc_fsm_ue_reconfig_rlf_actions;

    p_reconfig_data = &(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data);

   if((RRC_TRUE == p_reconfig_data->m.scell_config_updated) &&
      (p_reconfig_data->p_scell_config->bitmask & RRM_SCELL_RELEASE_LIST_PRESENT))
    {
        /* Stop UE Reconfiguration procedural timer. */
        uecc_ue_timer_stop(
           p_ue_context->p_gb_context,UECC_RRM_UE_RECONFIG_GUARD_TIMER,
           &p_ue_context->timers_data);
         
         /* Since scell release is successful at MAC and PHY, the Scell release
          * cannot be rolled back , the UE is released*/
        p_fail_actions_list = uecc_fsm_ue_reconfig_rlf_actions;
        uecc_fsm_start_failure_processing(p_ue_context, p_fail_actions_list);
    } 
    else 
    {
        /* Store the Re-establish message in the UE Context */
        len = (U16)sizeof(rrc_mac_radio_link_failure_ind_t);

        if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                    rrc_mem_get(len)))
        {
            RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                    "Memory Allocation Failed "
                    "to store Radio Link Failure Ind msg");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return;
        }

        /* storing re_establishment contents into msg_bfr_rcvd */
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api, len);

        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RLF_MSG;

        /* Switch state */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing,
                PNULL);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_radio_link_failure_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None                 
 * Returns        : None
 * Description    : This is the handler function for the event
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_radio_link_failure_handler(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        void*               p_api           /* Input API buffer */
        )
{
    /* Buffering the RLF message to be handled after completion 
     * of Dynamic UE Reconfiguration Procedure
     */
    U16    len = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_LLIM_RADIO_LINK_FAILURE_IND");

    /* Store the Re-establish message in the UE Context */
    len = (U16)sizeof(rrc_mac_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                rrc_mem_get(len)))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed "
                "to store RRC_MAC_RADIO_LINK_FAILURE_IND msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api, len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RLF_MSG;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* CA end */


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_clear_msg_buffer
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : None
 * DESCRIPTION   : Fail action to clear message buffered in ue context
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_clear_msg_buffer(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT( PNULL != p_ue_context );

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_UE_RECONFIG_RESP");

    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /* Stop Procedure Timer */
        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
/*BUG 604 changes start*/
              (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
              (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
/*BUG 604 changes stop*/
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
        }
        else
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
        }

        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_unspecified_2;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Discard the HO message*/
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL; 
        }
    }
    else
    {
        /* Release any buffered message */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd=PNULL;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* CA start */
/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_rlf_triggered
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 - Pointer to the UE context in UECC 
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : None
 *  DESCRIPTION: Fail action in case of Re-Establishment Triggered. Sending
 *               RRC_RRM_UE_RECONFIG_RESP to RRM with FAILURE
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_rlf_triggered(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break )
{
    U32 error_code = RRC_RADIO_LINK_FAILURE_TRIGGERED;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT( PNULL != p_ue_context );

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_UE_RECONFIG_RESP");

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* CA end */


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_reest_triggered
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : None
 *  DESCRIPTION: Fail action in case of Re-Establishment Triggered. Sending
 *               RRC_RRM_UE_RECONFIG_RESP to RRM with FAILURE
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_reest_triggered(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    U32 error_code = RRC_REESTABLISHMENT_TRIGGERED;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT( PNULL != p_ue_context );

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_UE_RECONFIG_RESP");

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_timer_expiry
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : None
 *  DESCRIPTION: Fail action in case of timer expiry. Sending
 *               RRC_RRM_UE_RECONFIG_RESP to RRM with FAILURE
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_timer_expiry(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    U32 error_code = RRC_TIMER_EXPIRY;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT( PNULL != p_ue_context );

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_UE_RECONFIG_RESP");

    /* Build and send RRC_RRM_UE_RECONFIG_RESP with FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : None
 * DESCRIPTION  : Fail action.
 *
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    U32 error_code = RRC_LLIM_RECONFIG_FAILURE;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT( PNULL != p_ue_context );

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);


    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_UE_RECONFIG_RESP");
    if (RRC_TRUE == p_ue_context->p_curr_proc_data->u.
            rrm_ue_reconfig_request_data.m.fail_cause_updated)
    {
        error_code = p_ue_context->p_curr_proc_data->u.
            rrm_ue_reconfig_request_data.fail_cause;
    }
    /* Build and send RRC_RRM_UE_RECONFIG_RESP with FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_reconfig_resp(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_RECONFIG_RESP] Message delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_send_ue_context_release_req
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : void
 * DESCRIPTION   : Sends the ASN.1 encoded UE Context Release request
 *
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_send_ue_context_release_req
(
 uecc_ue_context_t*  p_ue_context,
 rrc_bool_et*        p_break
 )
{
    s1ap_Cause cause; 
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, "FAILURE: END");
    /* Send release request to MME and
     * don't start UE Connection Release procedure */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.transport = s1ap_unspecified_2;

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context,
                &cause ) )
    {
        RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_last_with_ue_ctx_rel
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : void
 * DESCRIPTION   : Handling of 
 *                 uecc_fsm_ue_reconfig_fail_action_last_with_ue_ctx_rel
 *
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_last_with_ue_ctx_rel(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /* This function relists the UE Context as either:
       - the lower layers are completely out of sync now, OR
       - procedure timer has expired */

    /* Notify procedure about failure */
    uecc_fsm_finish_activity_failure(
            p_ue_context, (void *) UECC_EV_RRM_UE_RECONFIG_FAILURE_REL_UE_CTX_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_reconfig_fail_action_last
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None                
 * RETURNS       : void
 * DESCRIPTION   : Handling of 
 *                 uecc_fsm_ue_reconfig_fail_action_last
 ****************************************************************************/
void uecc_fsm_ue_reconfig_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RRM_UE_RECONFIG_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /* This function relists the UE Context as either:
       - the lower layers are completely out of sync now, OR
       - procedure timer has expired */

    /* Notify procedure about failure */
    uecc_fsm_finish_activity_failure(
            p_ue_context, (void *) UECC_EV_RRM_UE_RECONFIG_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
  Private Variables
 ****************************************************************************/
/* Fail actions */
/* UE Reconfig failure actions lists */

static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_fail_type1_actions[] =
{
    uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_timer_expiry,
    uecc_fsm_ue_reconfig_fail_action_clear_msg_buffer,
    uecc_fsm_ue_reconfig_fail_action_last_with_ue_ctx_rel,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_fail_type2_actions[] =
{
    uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp,
/*Dynamic_ue_reconfig+*/
//    uecc_fsm_ue_reconfig_fail_action_send_ue_context_release_req,
    uecc_fsm_ue_reconfig_fail_action_clear_msg_buffer,
    uecc_fsm_ue_reconfig_fail_action_last_with_ue_ctx_rel,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_fail_type3_actions[] =
{
    uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp,
    uecc_fsm_ue_reconfig_fail_action_clear_msg_buffer,
    uecc_fsm_ue_reconfig_fail_action_last,
    PNULL
};
/* Coverity_fix_94867_start */
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_rrc_reestablish_actions[] =
{
    uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_reest_triggered,
    PNULL
};

/* Coverity_fix_94867_end */
/*CA start*/
static const uecc_fsm_fail_handler_f uecc_fsm_ue_reconfig_rlf_actions[] =
{
    uecc_fsm_ue_reconfig_fail_action_send_rrm_ue_reconf_resp_rlf_triggered,
    uecc_fsm_ue_reconfig_fail_action_clear_msg_buffer,
    uecc_fsm_ue_reconfig_fail_action_last_with_ue_ctx_rel,
    PNULL
};

/*CA end*/



uecc_fsm_handler_record_t
uecc_fsm_ue_rrm_ue_reconfig_procedure_mht[] =
{
    { { UECC_EV_UE_RELEASE_REQ            			},
        uecc_fsm_rrm_ue_reconfig_ue_release_req_handler 				},
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND        		},
        uecc_fsm_rrm_ue_reconfig_rrm_ue_release_ind_handler 				},
    { { UECC_EV_S1AP_RESET_MSG_IND              		},
        uecc_fsm_rrm_ue_reconfig_s1ap_reset_handler 					},
    /* Dynamic_ue_reconfig + */
    /*{ { UECC_EV_S1AP_ERAB_SETUP_REQ              		},
        uecc_fsm_rrm_ue_reconfig_erab_setup_req_discard_handler 			},
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ            		},
        uecc_fsm_rrm_ue_reconfig_erab_modify_req_discard_handler 			},
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND      			},
        uecc_fsm_rrm_ue_reconfig_erab_rel_cmd_discard_handler 				},*/
    /* Dynamic_ue_reconfig - */
    { { UECC_EV_RRM_ERAB_RELEASE_IND          			},
        uecc_fsm_rrm_ue_reconfig_erab_rel_ind_discard_handler 				},
    /*SPR_18241_START*/
    /* Code Removed */
    /*SPR_18241_END*/
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ          			},
        uecc_fsm_rrm_ue_reconfig_csc_delete_all_ue_req_handler 				},
    /* SPR 16053 Fix Start */
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND                     },
        uecc_fsm_rrm_ue_reconfig_ongoing_radio_link_failure_handler                     },
    /* SPR 16053 Fix End */
    { { UECC_LAST_ID                      			}, PNULL		}
};

/***********************************************************/

/* Event Handlers of Activity states */
static uecc_fsm_handler_record_t
uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  			},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_init_handler 		},
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP  			},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_resp_handler    			},
    { { UECC_EV_TIMER_EXPIRY                			},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_timer_expiry_handler 	},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ               		},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_reestablish_req_handler	},
    { { UECC_EV_RRM_HO_REQUIRED                    		},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_ho_required_ind_handler              },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_radio_link_failure_handler  },
    { { UECC_LAST_ID                        			},  PNULL		}
};

static uecc_fsm_handler_record_t
uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      		},
        uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_init_handler   		},
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE   	},
        uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_complete_handler			},
    { { UECC_EV_TIMER_EXPIRY                    		},
        uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_timer_expiry_handler 	},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            		},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_reestablish_req_handler	},
    { { UECC_EV_RRM_HO_REQUIRED                    		},
        uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ho_required_ind_handler          },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_radio_link_failure_handler  },
    { { UECC_LAST_ID                            		},  PNULL		}
};

static uecc_fsm_handler_record_t
uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                             		},
        uecc_fsm_ue_reconfig_llim_rollback_ongoing_init_handler         		},
    { { UECC_EV_LLIM_MODIFY_ROLLBACK_RESP              		},
        uecc_fsm_ue_reconfig_llim_modify_rollback_resp_handler          		},
    { { UECC_EV_TIMER_EXPIRY                           		},
        uecc_fsm_ue_reconfig_llim_rollback_ongoing_timer_expiry_handler 		},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                   		},
        uecc_fsm_ue_reconfig_rrc_re_establish_request_handler           		},
    { { UECC_EV_RRM_HO_REQUIRED                    		},
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongonig_ho_required_ind_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
        uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_radio_link_failure_handler  },
    { { UECC_LAST_ID                            		},  PNULL		}
};

/***********************************************************/

/* Activity states */
static uecc_fsm_activity_state_t
uecc_fsm_rrm_ue_reconfig_llim_reconfig_ongoing =
{
    /* .s_name       =  */ (S8*)"UECC_UE_RECONFIG_LLIM_RECONFIG_ONGOING",
    /* .s_id         =  */ (S8*)"UE_RECONFIG_LLIM",
    /* .is_need_init =  */ RRC_TRUE,
    /* .p_mht        =  */ uecc_fsm_rrm_ue_reconfig_llim_reconfig_ue_ongoing_mht
};

static uecc_fsm_activity_state_t 
uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_UE_RECONFIG_RRC_CONN_RECONFIG_ONGOING",
    /* .s_id           =  */ (S8*)"UE_RECONFIG_RRC",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_rrm_ue_reconfig_rrc_conn_reconfig_ongoing_mht
};

static uecc_fsm_activity_state_t 
uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_UE_RECONFIG_LLIM_RECONFIG_ROLLBACK_ONGOING",
    /* .s_id           =  */ (S8*)"UE_RECONFIG_LLIM_ROLLBACK",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_rrm_ue_reconfig_llim_reconfig_rollback_ongoing_mht
};

/***********************************************************/

/* Activity */
static uecc_fsm_activity_t uecc_fsm_rrm_ue_reconfig_activity =
{
    /* .s_name        = */(S8*)"RRC Connection Reconfiguration",
    /* .s_id          = */(S8*)"UE_RCRECONF_A",
    /* .p_first_state = */&uecc_fsm_rrm_ue_reconfig_llim_reconfig_ongoing
};

/***********************************************************/

uecc_fsm_activity_t* uecc_fsm_rrm_ue_reconfig_procedure_activity_list[] =
{
    &uecc_fsm_rrm_ue_reconfig_activity,
    PNULL
};

/***********************************************************/

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_rrm_ue_reconfig_procedure =
{
    /*.s_name           = */(S8*)"RRM UE Reconfiguration",
    /*.s_id             = */(S8*)"UE_RRM_UE_RECONFIG_P",
    /*.constructor_f    = */uecc_fsm_rrm_ue_reconfig_procedure_constructor,
    /*.finish_success_f = */uecc_fsm_rrm_ue_reconfig_procedure_finish_success,
    /*.finish_fail_f    = */uecc_fsm_rrm_ue_reconfig_procedure_finish_fail,
    /*.activity_list    = */uecc_fsm_rrm_ue_reconfig_procedure_activity_list,
    /*.p_mht            = */uecc_fsm_ue_rrm_ue_reconfig_procedure_mht
};
