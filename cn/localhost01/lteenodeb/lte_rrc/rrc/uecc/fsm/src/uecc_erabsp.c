/******************************************************************************
*
*   FILE NAME:
*       uecc_erabsp.c
*
*   DESCRIPTION:
*       UECC ERAB SETUP procedure implementation.
*
*
*   DATE            AUTHOR      REFERENCE       REASON
*   28 May 2010     Ajay       ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/


/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_erabsp.h"
#include "uecc_common.h"
#include "uecc_logger.h"
#include "uecc_fsm_engine.h"
#include "uecc_erabsp_rmfsm.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_ct_a.h"
#include "uecc_s1ap.h"
#include "uecc_isa_a.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_asn_enc_dec.h"
#include "uecc_rrm.h"
#include "rrc_shared_stats.h"
#include "uecc_llim.h"



/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_ERAB_SETUP_FSM_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_uecc_log_facility_name, log_level, "[UE:%u] [%s] "format, \
            p_ue_context->ue_index,\
            p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id, \
            ##__VA_ARGS__)



/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_erab_setup_procedure_constructor(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_erab_setup_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_erab_setup_procedure_finish_fail(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erabsp_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,     /* UECC UE context */
        void*               p_api             /* Input API buffer */
);

static rrc_return_t uecc_fsm_erabsp_process_erab_setup_msg(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erabsp_rrm_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
        void*               p_api                 /* Input API buffer */
);

static void uecc_fsm_erabsp_ho_prep_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/*SPR 18758 Fix Start*/
/*Code Deleted*/
/*SPR 18758 Fix Stop*/
static void uecc_fsm_erabsp_rrm_erab_release_ind_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

static rrc_return_t uecc_fsm_erab_setup_process_erab_list(
    s1ap_E_RABToBeSetupListBearerSUReq *p_e_rab_to_be_setup_list,
    s1ap_E_RABToBeSetupItemBearerSUReq *p_e_rab_list[],    
    U8                                 *p_e_rab_list_count,
    uecc_ue_context_t                  *p_ue_context    
);

static rrc_return_t  uecc_fsm_erab_setup_allocate_erab_context(
    s1ap_E_RABToBeSetupItemBearerSUReq *p_e_rab_list[],    
    U8                                 e_rab_list_count,
    uecc_ue_context_t               *p_ue_context
);    

void uecc_fsm_erabsp_append_success_erab_to_fail_list(
    uecc_ue_context_t*  p_ue_context,    /* UECC UE Context */
    s1ap_Cause*         p_cause         /* S1AP Cause */
);

/*SPR_16942_START*/
extern void updatePhyConfigDedInUeCtx(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        rrc_phy_physical_config_dedicated_t*  p_phy_config_dest, /*dest PHY config*/
        rrc_phy_physical_config_dedicated_t*  p_phy_config_src /*src PHY config*/
        );
extern void uecc_update_mac_config_in_ue_context(
        uecc_ue_context_t*  p_ue_context,
        rrm_mac_config_t*   p_mac_config
        );
/*SPR_16942_END*/


/****************************************************************************
 Local Variables
****************************************************************************/

static uecc_fsm_activity_t*
    uecc_fsm_ue_erab_setup_procedure_activity_list[] =
{
    &uecc_fsm_erab_setup_rrc_connection_reconfiguration_activity,
    PNULL
};

static uecc_fsm_handler_record_t
    uecc_fsm_ue_erab_setup_procedure_mht[] =
{
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND        },
            uecc_fsm_erabsp_rrm_ue_release_req_handler },
    { { UECC_EV_HO_PREP_TIMER_EXPIRY },
            uecc_fsm_erabsp_ho_prep_timer_expiry_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
            uecc_fsm_erabsp_csc_cell_delete_req_handler },
    /*SPR 18758 Fix Start*/
    /* Code Deleted */
    /*SPR 18758 Fix Stop*/
    { { UECC_EV_RRM_ERAB_RELEASE_IND },
            uecc_fsm_erabsp_rrm_erab_release_ind_handler },
    /*SPR_18241_START*/
    /* Code Removed */
    /*SPR_18241_END*/
    { { UECC_LAST_ID                      }, PNULL    }
};


/****************************************************************************
 Global Variables
****************************************************************************/

/* ERAB SETUP procedure */
uecc_fsm_procedure_t uecc_fsm_ue_erab_setup_procedure =
{
   /* .s_name           = */(S8*)"E-RAB Setup",
   /* .s_id             = */(S8*)"UE_ERABSETUP_P",
   /* .constructor_f    = */
       uecc_fsm_ue_erab_setup_procedure_constructor,
   /* .finish_success_f = */
       uecc_fsm_ue_erab_setup_procedure_finish_success,
   /* .finish_fail_f    = */
       uecc_fsm_ue_erab_setup_procedure_finish_fail,
   /* .activity_list    = */
       uecc_fsm_ue_erab_setup_procedure_activity_list,
   /* .p_mht            = */uecc_fsm_ue_erab_setup_procedure_mht
};


/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_setup_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for ERAB Setup procedure
 ****************************************************************************/
void uecc_fsm_ue_erab_setup_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,    /* UECC UE Context */
    void*               p_api            /* Input API Buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name, 
            RRC_INFO,"ERAB SETUP: Transaction Id updated");
#else
    uecc_ue_ctx_set_op_transction_id(p_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
    /* Start Procedural Timer */
    if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_ERAB_SETUP_GUARD_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
    {
        RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_ERROR,
               "ERAB SETUP GUARD TIMER IS NOT Started");
        /* Stop Procedure */
        uecc_fsm_finish_active_procedure(p_ue_context);
    }
    else
    {
    
        do{
            /* Memory allocated dynamically for curr_proc_data */
            p_ue_context->p_curr_proc_data = (curr_proc_data_t *)rrc_mem_get(sizeof(
                        curr_proc_data_t));
            if ( PNULL == p_ue_context->p_curr_proc_data )
            {
                RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_FATAL,
                        "Memory Allocation failed for Current PROC data.");
                break;
            }
            else
            {
                /* SES-508 Fix Start */
                /* Code Removed */
                /* SES-508 Fix End */

                /* Set Current Procedure to ERAB SETUP */
                p_ue_context->p_curr_proc_data->t = UECC_ERAB_SETUP_PROC;
            }            

            /* Clear the share data in UE Context */
            memset_wrapper(&p_ue_context->share_data,
                    0,
                    sizeof(uecc_ue_share_data_t));


            /* Process ERAB SETUP REQUEST received from MME */ 
            if (RRC_SUCCESS !=
                    uecc_fsm_erabsp_process_erab_setup_msg(p_ue_context, 
                        p_api))
            {

                /* Stop timer */
                uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                        UECC_ERAB_SETUP_GUARD_TIMER,
                        &p_ue_context->timers_data);

                /* Send ASN.1 Encoded ERAB SETUP Response to MME */
                if(RRC_SUCCESS !=
                        uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
                {
                    RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_FATAL,
                            "[s1ap_E_RABSetup] delivery failure");
                }

                /* Process failure*/
                uecc_fsm_finish_activity_failure(
                        p_ue_context,
                        (void*)UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT);

                break;
            }
        } while (0);

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_setup_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for ERAB Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_erab_setup_procedure_finish_success(
         uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
         void*               p_api                  /* Input API buffer */
)
{

    U8 counter = 0;
    U8 erab_count = 0;
    uecc_rm_erab_setup_op_data_t*     p_erab_op_params  = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_op_params;

/*SPR_16942_START*/
    rrm_mac_config_t *p_mac_config = PNULL;
    rrc_phy_physical_config_dedicated_t  *p_phy_config = PNULL;
    rrm_meas_config_t           *p_meas_config = PNULL;
/*SPR_16942_END*/    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


   /* UT bug fix - TC UECC_ERAB_SETUP_62 */
   /* Send event to UE Main FSM */
   /* uecc_event_queue_push_event(
        UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT,
        p_ue_context,
        PNULL);
   */
 

/*SPR_16942_START*/
    if(p_erab_op_params->m.physical_config_dedicated_present)
    {
        p_phy_config =  &(p_erab_op_params->physical_config_dedicated);
        updatePhyConfigDedInUeCtx(p_ue_context,&(p_ue_context->physical_config_dedicated), p_phy_config);
    }

    if(p_erab_op_params->m.meas_config_present)
    {
        p_meas_config = &(p_erab_op_params->meas_config);

        if (RRC_SUCCESS !=  uecc_rrc_update_CurrMeasConfigList(
                    p_meas_config,
                    &p_ue_context->curr_meas_config,
                    p_ue_context->p_gb_context))
        {
            RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_WARNING,
                    "[%s] Failure to update CurrMeasConfigList !!",
                    __FUNCTION__);
        }
    }
    if(p_erab_op_params->m.proximity_config_present)
    {
        p_ue_context->proximity_config = p_erab_op_params->proximity_config;
        p_ue_context->m.proximity_config_present = 1;
    }
    /* Update MAC Config If Present */
    if (p_erab_op_params->m.mac_config_present)
    {    
        p_mac_config = &(p_erab_op_params->mac_config);
        uecc_update_mac_config_in_ue_context(p_ue_context,p_mac_config);
    }
/*SPR_16942_END*/
    /* Update UE Context */
    /* Reset Variables */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;
    /* SPR 11103 Fix Start */
    p_ue_context->m.meas_config_updated = 0;
    /* SPR 11103 Fix Stop */
    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

    /* ARP changes begin */
    if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        p_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing = RRC_FALSE;
    }
    /* ARP changes end */


    /* Update Active ERAB List Count in UE Context */

    erab_count = uecc_fsm_icsp_count_erab_list(p_ue_context);
    p_ue_context->e_rab_list_count = (U8)(p_ue_context->e_rab_list_count +
                                            erab_count); 

    uecc_status_ue_update_num_of_drb(p_ue_context, (S8)erab_count);

    /* Free NAS PDU from ERAB List in UE Context */
    if (0 < p_ue_context->e_rab_list_count)
    {
        for (counter = 0; counter < MAX_ERAB_COUNT; counter++ )
        {
            if (p_ue_context->p_e_rab_list[counter] != PNULL)
            {
                if (p_ue_context->p_e_rab_list[counter]->m.nAS_PDUPresent) 
                {
                    asn1Free_s1ap_NAS_PDU(&p_ue_context->s1ap_asn1_ctx,
                                     &p_ue_context->p_e_rab_list[counter]->nAS_PDU);
                    p_ue_context->p_e_rab_list[counter]->m.nAS_PDUPresent = 0;
                }
            }
        }
    }

    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_ue_context->m.is_sps_setup_or_rel_recv)
        {
            if (p_ue_context->p_sps_config != PNULL)
            {
                rrc_mem_free(p_ue_context->p_sps_config);
                p_ue_context->m.sps_config_present = 0;
                p_ue_context->p_sps_config = PNULL;
            }
        }
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_ue_context->p_gb_context,
                p_ue_context,
                RRC_SUCCESS);

    }

    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.is_sps_recv_frm_rrm = 0;
    p_ue_context->m.is_sps_setup_or_rel_recv = 0;

    /* SPS Stop */


    /* Clear the share data in UE Context */
    memset_wrapper(&p_ue_context->share_data,
               0,
               sizeof(uecc_ue_share_data_t));

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT,
            p_ue_context,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_setup_procedure_finish_fail
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for ERAB Setup procedure
                    Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_erab_setup_procedure_finish_fail(
         uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
         void*               p_api                  /* Input API buffer */
)
{
/*SPR 20636 Changes Start*/
    uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/*SPR 20636 Changes End*/
    s1ap_Cause cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ( UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT == failure_ev || 
         UECC_EV_S1AP_ERAB_SETUP_FAILURE_REL_UE_CTX_INT == failure_ev )
    {

        /* Fill Cause */
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason; 

        /* Now build and send UE CONTEXT RELEASE REQ to MME */
        if(RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
             p_ue_context,
             &cause))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR, "[%s]Unable to send UE CONTEXT RELEASE REQ to MME",
                    __FUNCTION__);
        }
    }
    p_ue_context->m.ue_agg_max_bit_rate_updated = 0;

    /* ARP changes begin */
    /*SPR 7106  Fix Start */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            is_erab_del_ongoing == RRC_TRUE)
        {
             p_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing = RRC_FALSE;
	}
    }
    /* SPR 7106 Fix Stop */
    /* ARP changes end */
     
    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_ue_context->p_sps_config != PNULL)
        {
            rrc_mem_free(p_ue_context->p_sps_config);
            p_ue_context->p_sps_config = PNULL;
            p_ue_context->m.sps_config_present = 0;
        }
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_ue_context->p_gb_context,
                p_ue_context,
                RRC_FAILURE);
    }
    /* SPS Stop */

    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        failure_ev,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_erabsp_process_erab_setup_msg
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function processes S1AP_ERAB_SETUP_IND
 *                  message:
 *                  - check message content;
 *                  - store IEs in UE context temp data i.e. erab_setup_data_t
 ****************************************************************************/
rrc_return_t uecc_fsm_erabsp_process_erab_setup_msg(
    uecc_ue_context_t*  p_ue_context,    /* UECC UE Context */
    void*               p_api            /* Input API Buffer */
)
{
    rrc_return_t    result = RRC_SUCCESS;
    rrc_s1ap_erab_setup_request_t    *p_msg = 
         (rrc_s1ap_erab_setup_request_t *)p_api;
    U8 e_rab_list_count = 0;

    s1ap_E_RABToBeSetupItemBearerSUReq *p_e_rab_list[MAX_ERAB_COUNT] = {PNULL};        
     
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UE_START_TIME_PERFORMANCE_COUNTER(
           p_ue_context, p_ue_context->ue_stats.ue_timer_stats[RRC_UE_ERAB_SETUP_TIME], 
           RRC_UE_ERAB_SETUP_TIME);

    do
    {
        /* Store UE AMBR in UE context if present for future processing */
        if (p_msg->bitmask &
            RRC_S1AP_ERAB_SETUP_REQUEST_UE_AGGREGATE_MAXIMUM_BITRATE_PRESENT)
        {
            p_ue_context->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL =
                p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL;
            p_ue_context->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL =
                p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL;
            p_ue_context->m.ue_agg_max_bit_rate_updated = 1;
        }
   
        /* Filter the E_RAB List with Failure Conditions */ 
        result = uecc_fsm_erab_setup_process_erab_list(
            &p_msg->e_rab_to_be_setup_list,
            p_e_rab_list,    
            &e_rab_list_count,
            p_ue_context);

        if ((e_rab_list_count != 0) && (result == RRC_SUCCESS))
        {
            /* Allocate E_RAB's in UE Context Dynamically */
            result = uecc_fsm_erab_setup_allocate_erab_context(
                         p_e_rab_list,    
                         e_rab_list_count,
                         p_ue_context);    
        }
        else
        {
           result = RRC_FAILURE;
        }

    }while(0);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return result;

}


/****************************************************************************
 * Function Name  : uecc_fsm_erabsp_ho_prep_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_HO_PREP_TIMER_EXPIRY
 ****************************************************************************/

static void uecc_fsm_erabsp_ho_prep_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Check if HO Msg is present in UE Context */ 
    if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /* FREE MSG IF ANY MSG PRESENT IN UE CONTEXT */
        if (  PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd )
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_tS1relocprep_expiry;

        /* SEND HO FAILURE TO RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);
    }
    else
    {
        RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_INFO,
            "[UECC_EV_HO_PREP_TIMER_EXPIRY]: Invalid Event Received;Unused variable p_api=%p"
            ,p_api);/*SPR 17777 +-*/
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}





/****************************************************************************
 * Function Name  : uecc_fsm_erabsp_csc_cell_delete_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_CSC_DELETE_ALL_UE_REQ
 ****************************************************************************/
void uecc_fsm_erabsp_csc_cell_delete_req_handler(
    uecc_ue_context_t*  p_ue_context,    /* UECC UE Context */
    void*               p_api            /* Input API Buffer */
)
{

    U8 id_count = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
        UECC_ERAB_SETUP_GUARD_TIMER,
        &p_ue_context->timers_data);


    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_CSC_CELL_DELETE_REQ_INT,
        p_ue_context,
        p_api);

    /* Clear E_RAB Context */
    for ( id_count = 0; id_count < MAX_ERAB_COUNT; id_count++)
    {
        if (p_ue_context->p_e_rab_list[id_count] != PNULL)
        {
            RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_INFO,
                "ERAB ID : %u context deleted from UE Context",
                p_ue_context->p_e_rab_list[id_count]->e_RAB_ID);
            rrc_mem_free(p_ue_context->p_e_rab_list[id_count]);
            p_ue_context->p_e_rab_list[id_count] = PNULL;
        }
    }

    p_ue_context->e_rab_list_count = 0;

    /* Free the data for current procedure in the UE Context */
    if (p_ue_context->p_curr_proc_data != PNULL)
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);  
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erabsp_rrm_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_erabsp_rrm_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,    /* UECC UE Context */
    void*               p_api            /* Input API Buffer */
)
{
    s1ap_Cause cause;
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_connection_release_ind = (rrc_rrm_ue_connection_release_ind_t *)p_api;

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    if (strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                (const S8 *)("UECC_RRM_ERB_SETUP_RESOURCE_ADMISSION_ONGOING")))
    {
        /*If Waiting for SgNBAdditionRequestAck, initiate SgNBReleaseReq without SgNB-X2AP-ID*/
        if (!strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                    (const S8 *)("UECC_X2AP_SGNB_ADDITION_ONGOING")))
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_DC_PREP_TIMER,
                    &p_ue_context->timers_data);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                /* Send SgNB RELEASE to SgNB */ 
                if (RRC_SUCCESS != 
                        build_and_send_sgnb_release_request(p_ue_context))
                {
                    RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_ERROR, 
                            "[%s],Failed to send SgNB Release Request", __FUNCTION__);
                }

                /* Reset SgNBAddRequired Flag in UE_CTX*/
                p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
            }
        }
        else
        {
            /*if SgNB Addition Ack is already received
             initiate SgNBReconfigurationComplete with failure*/
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_ERAB_SETUP_GUARD_TIMER,
                    &p_ue_context->timers_data);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                /* Send SgNBReconfigurationComplete with failure */ 
                if (RRC_SUCCESS != 
                        uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                            p_ue_context
                            ))
                {
                    RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_ERROR, 
                            "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                            " failed!", __FUNCTION__);
                }

                /* Reset SgNBAddRequired Flag in UE_CTX*/
                p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
            }
        }
    }
/*NR_DC Code Change Stop*/
#endif
    /* Map cause recv from RRM to S1AP Cause */
    if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
            &cause, &p_ue_connection_release_ind->release_cause,p_ue_context))
    {

#ifndef ENDC_ENABLED
/*NR_DC Code Change Start*/
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_SETUP_GUARD_TIMER,
            &p_ue_context->timers_data);
/*NR_DC Code Change Stop*/
#endif

        /* Append all success erabs to failed list */
        uecc_fsm_erabsp_append_success_erab_to_fail_list(
            p_ue_context,
            &cause);

        /* Send ASN.1 Encoded ERAB SETUP Response to MME */
        if(RRC_SUCCESS !=
           uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
        {
            RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_FATAL,
                "[s1ap_E_RABSetup] delivery failure");
        }


        /* Send event to UE Main FSM */
        uecc_event_queue_push_event(
            UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT,
            p_ue_context,
            p_api);
 
        /* Free memory for curr_proc_data_t */
        if ( PNULL != p_ue_context->p_curr_proc_data )
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data);
            p_ue_context->p_curr_proc_data = PNULL;
        }

        /* Stop Procedure */
        uecc_fsm_finish_active_procedure(p_ue_context);
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name ,
                RRC_WARNING, "[RRC_RRM_UE_CONNECTION_RELEASE_IND] "
                "invalid cause value [%u][%u]",
                p_ue_connection_release_ind->release_cause.type,
                p_ue_connection_release_ind->release_cause.value);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/*SPR 18758 Fix Start*/
/*Code Deleted*/
/*SPR 18758 Fix Start*/
/*****************************************************************************
 * Function Name  : uecc_fsm_erabsp_rrm_erab_release_ind_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RRC_RRM_ERAB_RELEASE_IND msg
 ********************************************************************************/
void uecc_fsm_erabsp_rrm_erab_release_ind_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U32 error_code;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_INFO,
        "Received: RRC_RRM_ERAB_RELEASE_IND.");

    /* Send Failure Here */
    
    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);

    if (RRC_SUCCESS !=
            uecc_rrm_build_and_send_erb_release_ind_fail_resp(
                p_api,
                p_ue_context,
                FAILURE,
                error_code))
    {
        RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_RELEASE_IND_CNF] Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_erabsp_append_success_erab_to_fail_list
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  s1ap_Cause*   - pointer to S1AP Cause 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This is the function fills the temporary failure list
 ****************************************************************************/
void uecc_fsm_erabsp_append_success_erab_to_fail_list(
    uecc_ue_context_t*  p_ue_context,    /* UECC UE Context */
    s1ap_Cause*         p_cause          /* S1AP Cause */
)
{
    U8 fail_list_counter     = 0;
    U8 succ_list_count       = 0;
    U8 succ_list_counter     = 0;  

    uecc_rm_erab_setup_info_t*        p_erab_setup_list = &p_ue_context->
        p_curr_proc_data->u.erab_setup_request_data.erab_info_data;
    uecc_erab_failed_to_setup_list_t* p_erab_fail_list = &p_ue_context->
        p_curr_proc_data->u.erab_setup_request_data.failed_erab_data;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    fail_list_counter = p_erab_fail_list->count;

    succ_list_count = p_erab_setup_list->count;

    for (succ_list_counter = 0; 
         succ_list_counter < succ_list_count && 
         succ_list_counter < MAX_ERAB_COUNT;
         succ_list_counter++)
    {
        if (RRC_NO_ERROR ==  p_erab_setup_list->erab_info[succ_list_counter].
            status)
        {
         
            p_erab_fail_list->erab_failed_to_setup_item[fail_list_counter].
                e_RAB_ID = p_erab_setup_list->erab_info[succ_list_counter].
                           erab_id;
 
            p_erab_setup_list->erab_info[succ_list_counter].status = 
                RRC_ERROR_CODE_LAST;
     
            l3_memcpy_wrapper(&p_erab_fail_list->erab_failed_to_setup_item
                       [fail_list_counter].cause, 
                       p_cause,
                       sizeof(s1ap_Cause));

            /* Delete Failed ERAB from UE CONTEXT */
            uecc_fsm_rm_erab_setup_deallocate_erab(
                    p_ue_context,           
                    p_erab_setup_list->erab_info[succ_list_counter].erab_id);

            fail_list_counter++;
        }    
    }
  
    /* Copy Fail Count to fail List */
    p_erab_fail_list->count = fail_list_counter;


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}




/****************************************************************************
 * Function Name  : uecc_fsm_erab_setup_process_erab_list
 * Inputs         : s1ap_E_RABToBeSetupListBearerSUReq - Pointer to ERAB List 
 *                  s1ap_E_RABToBeSetupItemBearerSUReq - Array of ERAB items
 *                  U8 *e_rab_list_count - Pointer to No of ERABS
 *                  uecc_erab_failed_to_setup_list_t * - Pointer to failed list
 *                  uecc_ue_context_t * - Pointer to Failed List
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This is the process the ERAB List received from MME.               
 ****************************************************************************/
rrc_return_t  uecc_fsm_erab_setup_process_erab_list(
    s1ap_E_RABToBeSetupListBearerSUReq *p_e_rab_to_be_setup_list, /* ERAB List */
    s1ap_E_RABToBeSetupItemBearerSUReq *p_e_rab_list[],           /* ERAB Item */
    U8                                 *e_rab_list_count,         /* No of ERAB's */
    uecc_ue_context_t                  *p_ue_context              /* UECC UE Context */
)
{
    S8 temp_erab_list[MAX_ERAB_COUNT] = {0};
    U8 fail_list_count = 0;
    U8 qci_value = 0;

    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABToBeSetupListBearerSUReq_element *p_erab_elem = PNULL;

    uecc_erab_failed_to_setup_list_t   *p_failed_erab_data =    
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        failed_erab_data;
     
    RRC_ASSERT(PNULL != p_e_rab_to_be_setup_list);
    RRC_ASSERT(PNULL != p_failed_erab_data);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Find the mutiple occurences of ERAB IDs in temp_erab_list array */
    for (p_node = p_e_rab_to_be_setup_list->head;
         (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT);
         p_node=p_node->next )
    {
        p_erab_elem=(s1ap_E_RABToBeSetupListBearerSUReq_element*)p_node->data;
        if ((p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1 == PNULL ))
        {
            continue;
        }
        
        if (p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID > MAX_SUPPORTED_E_RAB_ID)
        {
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                cause.t = T_s1ap_Cause_radioNetwork;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                cause.u.radioNetwork = s1ap_unknown_E_RAB_ID;
            fail_list_count++;

            RRC_UECC_ERAB_SETUP_FSM_TRACE(
                    RRC_ERROR,
                    "INVALID E_RAB ID : %u",
                    p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
        }
        else if ( temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID] == 0 )
        {
            temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID] = 1;
        }
        else if ( temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID] == 1 )
        {
            temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID] = -1;
            /* Multiple E_RAB ID found put in fail list with cause value */
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t 
                =  T_s1ap_Cause_radioNetwork;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.u.
                radioNetwork = s1ap_multiple_E_RAB_ID_instances;
        
            fail_list_count++;
            RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_ERROR,
                         "Mutiple E_RAB ID Found : %u",
                         p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
        }
    }
    
    /* Filter ERAB List for QCI and ACTIVE ERABS */
    for (p_node = p_e_rab_to_be_setup_list->head;
         (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT);
         p_node=p_node->next )
    {
        p_erab_elem=(s1ap_E_RABToBeSetupListBearerSUReq_element*)p_node->data;
        if ((p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1 == PNULL ))
        {
            continue;
        }

        qci_value = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RABlevelQoSParameters.qCI;
  
        if ((PNULL != p_ue_context->p_gb_context) && (PNULL != g_rrc_stats.p_rrc_stats))
        {
            /* SPR 13441 Fix Start */
            if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                    (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
            {
            /* Update the performance counter for Erab setup addtional attempts */
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                    rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.
                    erab_setup_stats.
                    erab_estabAddAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_erab_related_stats.erab_setup_stats.
                    erab_estabAddAttNbr_sum, RRC_UPDATED_QCI_INDEX(qci_value), 
                    RRC_MAX_QCI_INDEX);
            }
            /* SPR 13441 Fix End */
        }

        if (p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID > MAX_SUPPORTED_E_RAB_ID)
        {
            /* Invalid E_RAB ID's have already been
             * populated in failed
             * to setup list in previous
             * for loop
             */
            continue;
        }


        if (temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID] == 1)
        {
            /* Check if request is came for existing ID */
            if ( PNULL != p_ue_context->p_e_rab_list[p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->
                          e_RAB_ID] ) 
            {
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t 
                    =  T_s1ap_Cause_radioNetwork;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.u.
                    radioNetwork = s1ap_multiple_E_RAB_ID_instances ;

                fail_list_count++;
                RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_ERROR,
                    "E_RAB ID Already Found : %u",
                    p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);

                continue; 
            }
                /*lipa fix start */ 
            else if(PNULL != p_ue_context->p_gb_context)
            { 
                   if( p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->m.iE_ExtensionsPresent  && 
                       RRC_FALSE == p_ue_context->p_gb_context->lgw_configured_status )
                   {
                     p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                     e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID ;
                     p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t 
                      = T_s1ap_Cause_misc; 
                     p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.u.
                     misc = s1ap_om_intervention;
        
                    fail_list_count++;
                    RRC_UECC_ERAB_SETUP_FSM_TRACE(
                        RRC_ERROR,
                        "ERAB  = %u  setup failed as Correlation ID received from MME when LGW is not configured",
                        p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID );
                 continue;
                   }
             }
                /*lipa fix end */ 

            p_e_rab_list[*e_rab_list_count] = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1;            
            /* Value of e_rab_list_count will not exceed 16 */
            (*e_rab_list_count)++;
        }
    }
    p_failed_erab_data->count = fail_list_count;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_erab_setup_allocate_erab_context
 * Inputs         : s1ap_E_RABToBeSetupItemBearerSUReq - Array of ERAB Items
 *                  U8 e_rab_list_count - No of ERABS 
 *                  uecc_ue_context_t * - pointer to UE Context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function allocates ERAB's info in UE Context
                    
 ****************************************************************************/
rrc_return_t  uecc_fsm_erab_setup_allocate_erab_context(
    s1ap_E_RABToBeSetupItemBearerSUReq *p_e_rab_list[],    /* ERAB Item Array */    
    U8                                 e_rab_list_count,   /* No of ERAB's */
    uecc_ue_context_t                  *p_ue_context       /* UECC UE Context */
)    
{
    U8 erab_counter = 0;
    U8 e_rab_id_index = 0;
    U8 success_erab_count = 0;
    rrc_return_t response = RRC_SUCCESS;

    OSRTDListNode *p_node = PNULL;
    s1ap_E_RABToBeSetupItemBearerSUReq_iE_Extensions
                    *p_iE_Extensions_list = PNULL;
    RRC_ASSERT(PNULL != p_e_rab_list);
    RRC_ASSERT(PNULL != p_ue_context);

    /* Allocate ERAB's Dynamically in p_ue_context->e_rab_list context */
    for ( erab_counter = 0; erab_counter < e_rab_list_count; erab_counter++ )
    {
    
        /* Will Store At Index e_RAB_ID */
        e_rab_id_index = (U8)(p_e_rab_list[erab_counter]->e_RAB_ID);

        /* Dynamic Memory Allocation For ERAB */
        p_ue_context->p_e_rab_list[e_rab_id_index] = (uecc_drb_ctx_t *) 
                             rrc_mem_get(sizeof(uecc_drb_ctx_t));

        /* If Memory not allocated put in failed list with cause */
        RRC_ASSERT(PNULL != p_ue_context->p_e_rab_list[e_rab_id_index]);
        if (PNULL == p_ue_context->p_e_rab_list[e_rab_id_index])
        {
            RRC_UECC_ERAB_SETUP_FSM_TRACE(RRC_FATAL,
               "Memory Allocation failed for ERAB's.");
            response = RRC_FAILURE;
            break;
        }

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        /* Fill Temp ERAB Setup List in Share Data */
        p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            erab_info_data.erab_info[erab_counter].erab_id =  
                e_rab_id_index;

        p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            erab_info_data.erab_info[erab_counter].status = RRC_NO_ERROR;

        /* Fill ERAB Params received from MME in ERAB Context in UE Context */ 
        p_ue_context->p_e_rab_list[e_rab_id_index]->e_RAB_ID =
            e_rab_id_index;

        asn1Copy_s1ap_E_RABLevelQoSParameters(&p_ue_context->s1ap_asn1_ctx,
            &p_e_rab_list[erab_counter]->e_RABlevelQoSParameters,
            &p_ue_context->p_e_rab_list[e_rab_id_index]->e_RABlevelQoSParameters);

        /*Bug_12446_fix_start*/
        /*asn1Copy_s1ap_TransportLayerAddress(&p_ue_context->s1ap_asn1_ctx,
            &p_e_rab_list[erab_counter]->transportLayerAddress,
            &p_ue_context->p_e_rab_list[e_rab_id_index]->transportLayerAddress);*/

        if (RRC_SUCCESS != rrc_copy_transportlayer_address(&p_ue_context->
                p_e_rab_list[e_rab_id_index]->transportLayerAddress,
                &p_e_rab_list[erab_counter]->transportLayerAddress,
                 p_ue_context))
        {
            response = RRC_FAILURE;
            break;
        }
        /*Bug_12446_fix_stop*/

        p_ue_context->p_e_rab_list[e_rab_id_index]->gTP_TEID =
            p_e_rab_list[erab_counter]->gTP_TEID;

        asn1Copy_s1ap_NAS_PDU(&p_ue_context->s1ap_asn1_ctx,
             &p_e_rab_list[erab_counter]->nAS_PDU,
             &p_ue_context->p_e_rab_list[e_rab_id_index]->nAS_PDU);

        p_ue_context->p_e_rab_list[e_rab_id_index]->m.nAS_PDUPresent = 1;
        /* lipa start */
        /* Copy Correlation ID */
        if ( p_e_rab_list[erab_counter]->m.iE_ExtensionsPresent )
        {

            if( p_ue_context->p_gb_context->lgw_configured_status )
            {
                p_iE_Extensions_list  = &(p_e_rab_list[erab_counter]->iE_Extensions); 
                p_node = p_iE_Extensions_list->head;
                for ( ; PNULL != p_node; p_node = p_node->next)
                {
                    s1ap_E_RABToBeSetupItemBearerSUReq_iE_Extensions_element *p_elem =
                        (s1ap_E_RABToBeSetupItemBearerSUReq_iE_Extensions_element*)p_node->data;

                    RRC_ASSERT(PNULL!=p_elem);

                    if ( p_elem->extensionValue.t == T30__E_RABToBeSetupItemBearerSUReqExtIEs_1 
                            &&
                            p_elem->extensionValue.u._E_RABToBeSetupItemBearerSUReqExtIEs_1 != PNULL )

                    {
                        p_ue_context->p_e_rab_list[e_rab_id_index]->correlation_id = 
                            (s1ap_Correlation_ID*)rrc_mem_get(sizeof(s1ap_Correlation_ID));

                        asn1Copy_s1ap_Correlation_ID(&p_ue_context->s1ap_asn1_ctx,
                                p_elem->extensionValue.u._E_RABToBeSetupItemBearerSUReqExtIEs_1,
                                p_ue_context->p_e_rab_list[e_rab_id_index]->correlation_id);

                        p_ue_context->p_e_rab_list[e_rab_id_index]->m.Lipa_RabPresent = RRC_TRUE;
                    }
                }
            }
            else 
            {
                /* handled before in 
                 uecc_fsm_erab_setup_process_erab_list */

            }
        }/* lipa end */

        RRC_UECC_ERAB_SETUP_FSM_TRACE(
                RRC_INFO,
                "E_RAB Context Successfully Allocated for E_RAB ID : %u ",
                e_rab_id_index);

        success_erab_count++;

    }

    /* Update ERAB Success Counts in Share Data */ 
    p_ue_context->p_curr_proc_data->u.erab_setup_request_data.erab_info_data.
        count = success_erab_count;

    if ( success_erab_count == 0 )
    {
        response = RRC_FAILURE;
    }
    
    return response;
}
