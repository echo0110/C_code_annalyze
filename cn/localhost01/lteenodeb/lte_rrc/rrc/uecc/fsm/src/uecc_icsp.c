/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_icsp.c,v 1.50 2010/11/16 13:09:56 gur21239 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Initial UE context procedure implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_icsp.c,v $
 * Revision 1.50  2010/11/16 13:09:56  gur21239
 * spr 606 related changes done
 *
 * Revision 1.49  2010/10/19 03:18:53  gur11120
 * SPR 708 fix
 *
 * Revision 1.48  2010/10/13 07:03:31  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.13.4.8  2010/10/05 12:54:11  gur04518
 * Merged SPR 692
 *
 * Revision 1.13.4.7  2010/10/05 12:35:34  gur22047
 * compilation warning removed
 *
 * Revision 1.13.4.6  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.13.4.5  2010/09/13 13:03:57  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.13.4.4  2010/08/10 10:02:34  gur18569
 * incorported review comments for ho
 *
 * Revision 1.13.4.3  2010/08/05 08:07:13  gur15697
 * merging round 2
 *
 * Revision 1.13.4.2  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.13.4.1  2010/06/23 10:37:58  gur18569
 * Added Target Handover related changes
 *
 * Revision 1.13  2010/05/14 09:36:58  gur19827
 * checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 * Revision 1.12  2010/04/07 15:32:59  gur21006
 * warnings removed
 *
 * Revision 1.11  2010/04/05 09:48:24  gur10248
 * cell_delete_changes_05apr
 *
 * Revision 1.10  2010/03/31 18:26:39  gur22133
 * ssit_changes
 *
 * Revision 1.9  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.8  2010/03/12 05:49:56  gur22133
 * ut_ue_capability_changes
 *
 * Revision 1.7  2010/02/24 04:55:07  gur10248
 * comments_dpisposed_cell_del_1_1
 *
 * Revision 1.6  2010/02/23 14:11:20  gur22133
 * after_lokesh_ue_cap_code_review_23feb
 *
 * Revision 1.5  2010/02/22 13:29:47  gur22133
 * after_code_review_22feb
 *
 * Revision 1.4  2010/02/22 05:45:31  gur22133
 * *** empty log message ***
 *
 * Revision 1.3  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.6  2009/12/26 12:09:22  gur21006
 * Warnings removed
 *
 * Revision 1.1.4.5  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.4.4  2009/12/08 16:05:18  ukr15916
 * LG_14 fixed
 *
 * Revision 1.1.4.3  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.4.2  2009/12/07 15:28:44  ukr16032
 * UECC - inspection log items LG_1, LG_2, LG_3, LG_5, LG_8, LG_9, LG_10, LG_12, LG_13, LG_15, LG_16, LG_17 fixed
 *
 * Revision 1.1.4.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.17  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.16  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.15.2.11  2009/11/24 09:54:05  ukr16032
 * UECC FT - new FT  tests added
 *
 * Revision 1.1.2.15.2.10  2009/11/24 08:20:52  ukr18880
 * comment changed
 *
 * Revision 1.1.2.15.2.9  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.15.2.8  2009/11/23 15:34:36  ukr15916
 * nAS_PDUPresent = 0 added
 *
 * Revision 1.1.2.15.2.7  2009/11/23 14:54:06  ukr16032
 * UECC NAS PDU processing on reconfigure added
 *
 * Revision 1.1.2.15.2.6  2009/11/20 19:14:40  ukr16032
 * UECC ICS failure processing updated
 *
 * Revision 1.1.2.15.2.5  2009/11/20 15:38:46  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.15.2.4  2009/11/20 13:17:29  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.15.2.3  2009/11/20 12:57:30  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.15.2.2  2009/11/20 12:16:24  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.15.2.1  2009/11/19 18:56:34  ukr16032
 * UECC RRM updated according to the last interface changes.
 *
 * Revision 1.1.2.15  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.14  2009/11/18 17:06:14  ukr16032
 * UECC NAS processing updated
 * (except sending NAS PDU in non delivery msg).
 * Security capability processing fixed.
 * Transaction_id generetion updated.
 *
 * Revision 1.1.2.13  2009/11/18 11:12:09  ukr15916
 * single function rrc_generate_key() for key generation,algo_id set for Kup_enc
 *
 * Revision 1.1.2.12  2009/11/18 10:02:49  ukr16032
 * UECC IAS procedure implemented
 *
 * Revision 1.1.2.11  2009/11/16 21:47:42  ukr15916
 * Generate integrity/security keys
 *
 * Revision 1.1.2.10  2009/11/16 18:32:53  ukr16032
 * UECC ISA activity implementation started
 *
 * Revision 1.1.2.9  2009/11/16 14:53:07  ukr15916
 * mme_id check moved to uecc_s1ap
 *
*
 * Revision 1.1.2.7  2009/11/16 11:14:23  ukr15916
 * uecc_drb_ctx_t added
 *
 * Revision 1.1.2.6  2009/11/16 10:20:38  ukr15916
 * moved to rrc_s1ap_ue_aggregate_maximum_bitrate_t
 *
 * Revision 1.1.2.5  2009/11/16 09:29:23  ukr15916
 * RRC_MAX_NUM_DRB as erab max num
 *
 * Revision 1.1.2.4  2009/11/15 12:32:12  ukr15916
 * mme_s1ap_id is stored in uecc context now
 *
 * Revision 1.1.2.3  2009/11/15 10:02:59  ukr15916
 * beautification
 *
 * Revision 1.1.2.2  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.1  2009/11/12 14:36:33  ukr16032
 * UECC - Initial UE context setup procedure moved to uecc_icsp.*
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_ue_ctx.h"
#include "uecc_icsp.h"
#include "uecc_common.h"
#include "uecc_logger.h"
#include "uecc_fsm_engine.h"
#include "uecc_rmfsm.h"
#include "uecc_trg_hofsm.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_ct_a.h"
#include "uecc_s1ap.h"
#include "uecc_isa_a.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_asn_enc_dec.h"
#include "uecc_rrm.h"
#include "uecc_mainfsm.h"
#include "uecc_llim.h"
#include "rrc_shared_stats.h"
#include "rrc_logging.h"
#include <lteMisc.h>
/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_ICS_FSM_TRACE(log_level, format, ...) \
    RRC_LOG(rrc_uecc_log_facility_name, log_level, "[UE:%u] [%s] "format, \
            p_ue_context->ue_index,\
            p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id, \
            ##__VA_ARGS__)

 /*Bug_11999_fix_start*/
#define RRC_IS_GBR_BEARER(qci) ((qci >= 1 && qci <= 4) ? 1 :0)
 /*Bug_11999_fix_stop*/

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_initial_context_setup_procedure_constructor(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_initial_context_setup_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_icsp_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_icsp_reset_req_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_icsp_csc_cell_delete_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_ics_rlf_ind_handler(
 uecc_ue_context_t*  p_ue_context,
 void*               p_api
 );
static void uecc_fsm_ics_re_establish_req_handler(
 uecc_ue_context_t*  p_ue_context,
 void*               p_api
);
static void uecc_fsm_icsp_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static rrc_return_t uecc_fsm_icsp_process_initial_ue_ctx_setup_msg(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

static void uecc_fsm_icsp_rrm_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/*SPR_17893_START*/
static rrc_return_t uecc_fsm_icsp_process_and_save_ue_radio_capability(
        uecc_ue_context_t  *p_ue_context,
        UE_CapabilityRAT_ContainerList *p_rat_container_list
);
/*SPR_17893_END*/

static rrc_return_t  uecc_fsm_icsp_process_erab_list(
    s1ap_E_RABToBeSetupListCtxtSUReq *p_e_rab_to_be_setup_list,
    s1ap_E_RABToBeSetupItemCtxtSUReq *p_e_rab_list[],    
    U8                                 *e_rab_list_count,
    uecc_ue_context_t                  *p_ue_context
);

static rrc_return_t  uecc_fsm_icsp_allocate_erab_context(
    s1ap_E_RABToBeSetupItemCtxtSUReq *p_e_rab_list[],    
    U8                                 e_rab_list_count,
    uecc_ue_context_t                  *p_ue_context
); 


/****************************************************************************
 Local Variables
****************************************************************************/

static uecc_fsm_activity_t*
    uecc_fsm_ue_initial_context_setup_procedure_activity_list[] =
{
    &uecc_fsm_initial_security_activation_activity,
    &uecc_fsm_ue_capability_transfer_activity,
    &uecc_fsm_rrc_connection_reconfiguration_activity,
    PNULL
};

static uecc_fsm_handler_record_t
    uecc_fsm_ue_initial_context_setup_procedure_mht[] =
{
    { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_icsp_ue_release_req_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND        },
            uecc_fsm_icsp_reset_req_handler },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND        },
            uecc_fsm_icsp_rrm_ue_release_req_handler },
    { { UECC_EV_TIMER_EXPIRY              },
            uecc_fsm_icsp_timer_expiry_handler },
     { { UECC_EV_CSC_DELETE_ALL_UE_REQ              },
            uecc_fsm_icsp_csc_cell_delete_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND         },
            uecc_fsm_ics_rlf_ind_handler           },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ         },
            uecc_fsm_ics_re_establish_req_handler  },
    { { UECC_LAST_ID                      }, PNULL    }
};


/****************************************************************************
 Global Variables
****************************************************************************/

/* Initial UE context procedure */
uecc_fsm_procedure_t uecc_fsm_ue_initial_context_setup_procedure =
{
   /* .s_name           = */(S8*)"Initial UE Context Setup",
   /* .s_id             = */(S8*)"UE_ICS_P",
   /* .constructor_f    = */
       uecc_fsm_ue_initial_context_setup_procedure_constructor,
   /* .finish_success_f = */
       uecc_fsm_ue_initial_context_setup_procedure_finish_success,
   /* .finish_fail_f    = */
       uecc_fsm_ue_initial_context_setup_procedure_finish_fail,
   /* .activity_list    = */
       uecc_fsm_ue_initial_context_setup_procedure_activity_list,
   /* .p_mht            = */uecc_fsm_ue_initial_context_setup_procedure_mht
};
/****************************************************************************
 * Global Functions Declaration
 ****************************************************************************/
/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/

/****************************************************************************
 * Function Name  : uecc_fsm_ue_initial_context_setup_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for Initial UE Context Setup procedure
 ****************************************************************************/
void uecc_fsm_ue_initial_context_setup_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
	/* coverity_fix_62094_start */
	rrc_return_t    result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

#ifdef RRC_UECC_TR_ID_CONST
	uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
#else
	uecc_ue_ctx_set_op_transction_id(p_ue_context,
			uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
	do {
		p_ue_context->share_data.p_uecc_rm_data =
			(uecc_rm_data_t*)
			rrc_mem_get(sizeof(uecc_rm_data_t));
		if ( p_ue_context->share_data.p_uecc_rm_data  ==  PNULL)
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
					p_ue_context->p_gb_context->facility_name,RRC_FATAL,
					"Memory Allocation Failure!!");
			break;

		}

		/* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

		p_ue_context->share_data.p_uecc_rm_data->cause.t =
			T_s1ap_Cause_radioNetwork;
        	/* SPR 14196 Fix Start */
		p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
			s1ap_unspecified_2;
        	/* SPR 14196 Fix Stop */

		/* Start ICS Timer */
		if ( RRC_SUCCESS != uecc_ue_timer_start(
					p_ue_context->p_gb_context, 
					UECC_UE_ICS_TIMER,
					&p_ue_context->p_gb_context->ue_timers_data,
					&p_ue_context->timers_data,
					p_ue_context->ue_index))
		{
			break;
		}

		p_ue_context->ue_category = 255; /* Initially Set Invalid UE Category */

		if (RRC_SUCCESS !=
				uecc_fsm_icsp_process_initial_ue_ctx_setup_msg(p_ue_context, p_api))
		{
			break;
		}

		RRC_UECC_ICS_FSM_TRACE(RRC_INFO, "eRAB number to be setup [%u]",
				p_ue_context->e_rab_list_count);

		result = RRC_SUCCESS;

	} while (0);

	if (RRC_SUCCESS != result)
	{
		/* Process failure*/
		uecc_fsm_finish_activity_failure(
				p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT);
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_62094_stop */

/****************************************************************************
 * Function Name  : uecc_fsm_ue_initial_context_setup_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for Initial UE Context Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_initial_context_setup_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U8 counter = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_ICS_TIMER, &p_ue_context->timers_data);

    /* finishing UE_CT_RRM_ENQ procedure as success */
    if(p_ue_context->rrm_ue_capability_enq_ind)
    {
        uecc_fsm_ue_capability_rrm_enq_procedure_finish_success(p_ue_context,p_api);
    }
    /* Build and send the RRC_S1AP_INIT_CTX_SETUP_COMP_RESP msg to S1AP */
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_initial_context_setup_response(p_ue_context))
    {
        RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                "[s1ap_InitialContextSetupResponse] Delivery failure");
    }

    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;
    p_ue_context->m.meas_config_updated = 0;
   /* set SRB2 create flag in UE context as one */
   p_ue_context->m.srb2_created = 1;
    /*Qos changes start*/
    p_ue_context->m.ue_agg_max_bit_rate_updated = 0;
    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

    /*Qos changes end*/
   /* update the number of SRB2 in UE context as well in cell context */
   uecc_status_ue_update_num_of_srb2(p_ue_context);

    /* Free NAS PDUs*/
    /* ICS START */

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


    /* ICS START */
    p_ue_context->e_rab_list_count = uecc_fsm_icsp_count_erab_list(
                                         p_ue_context);    
    uecc_status_ue_update_num_of_drb(p_ue_context,(S8)p_ue_context->e_rab_list_count);
    p_ue_context->ue_status.ue_state = ECM_CONNECTED;

    /* Clear the share data in UE Context */
    if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
    {
        rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
        p_ue_context->share_data.p_uecc_rm_data = PNULL;
    }

    /* Free the Current Proc data memory */
    if (p_ue_context->p_curr_proc_data != PNULL)
    {

        /*SPR 21947 Start */
        if(p_ue_context->p_curr_proc_data->msg_bfr_rcvd != PNULL)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        if(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.p_scell_config != PNULL)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.p_scell_config);
            p_ue_context->p_curr_proc_data->u.rrm_ue_reconfig_request_data.p_scell_config = PNULL;
        }
        if(p_ue_context->p_curr_proc_data->p_registered_lai != PNULL)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->p_registered_lai);
            p_ue_context->p_curr_proc_data->p_registered_lai = PNULL;
        }
        /*SPR 21947 Stop */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL; 
    }  

    /*SPR 21947 Start */
    if(p_ue_context->p_registered_lai != PNULL)
    {
        rrc_mem_free(p_ue_context->p_registered_lai);
        p_ue_context->p_registered_lai = PNULL; 
    }
    /*SPR 21947 Stop */

    /* ICS STOP */

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_S1AP_INITIAL_CTX_SETUP_SUCCESS_INT,
            p_ue_context,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_initial_context_setup_procedure_finish_fail
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for Initial UE Context Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_initial_context_setup_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
	/* coverity_fix_54728_start */
/* SPR 20636 Changes Start*/
	uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/* SPR 20636 Changes End*/
	OSCTXT asn1_ctx;   
	U8 counter = 0;
    /* SPR 13614 Fix Start */
    U8 index = RRC_NULL;
    /* SPR 13614 Fix Stop */
	s1ap_Cause s1apCause;
    /* SPR 16152 Fix Start */
    uecc_erab_failed_to_setup_list_t *p_failed_erab_data = PNULL; 
    uecc_rm_erab_setup_info_t        *p_erab_setup_list = PNULL;
    U8 count = RRC_NULL;
    U8 fail_list_count = RRC_NULL;
    /* SPR 16152 Fix Stop */
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	memset_wrapper(&s1apCause, 0, sizeof(s1apCause));

	/* Init ASN context */
	if (RT_OK != rtInitContext(&asn1_ctx))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,RRC_ERROR,
				"[s1ap_InitialContextSetupFailure] "
				"ASN context initialization failed");
		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}


	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context,
			UECC_UE_ICS_TIMER, &p_ue_context->timers_data);

	/* finishing UE_CT_RRM_ENQ procedure as failure */
	if(p_ue_context->rrm_ue_capability_enq_ind)
	{
		uecc_fsm_ue_capability_rrm_enq_procedure_finish_failure(p_ue_context,p_api);
	}
	if (   (UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT == failure_ev)
			|| (UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT == failure_ev)
			|| (UECC_EV_S1AP_INITIAL_CTX_SETUP_RRM_RELEASE_REQ_INT== failure_ev)
			|| (UECC_EV_CSC_CELL_DELETE_REQ_INT == failure_ev)
			|| (UECC_EV_ICS_RADIO_LINK_FAILURE_INT == failure_ev)
			|| (UECC_EV_ICS_REESTABLISH_REQ_INT == failure_ev)
	   )
	{
		/* Send failure response to MME */
		RRC_UECC_ICS_FSM_TRACE(RRC_INFO,
				"FAILURE: Send s1ap_InitialContextSetupFailure");


	    /* SPR 16152 Fix Start */
        if ((PNULL != g_rrc_stats.p_rrc_stats) && (PNULL != p_ue_context->p_curr_proc_data))
        { 
            p_failed_erab_data = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                failed_erab_data;

            p_erab_setup_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data;

            /* Checking whether failure is due to timer expiry as in case of timer expiry ,
               all the erabs are added in the failed to setup list */
            if (UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT != failure_ev)
            {
                for ( count = 0; (count < p_erab_setup_list->count) && (count < MAX_ERAB_COUNT); count++)
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_erab_related_stats.erab_setup_stats.
                            erab_estab_init_fail_stats.erab_estabInitFailNbr_cause,
                            p_ue_context->share_data.p_uecc_rm_data->cause);

                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.
                            erab_setup_stats.erab_estabInitFailNbr_sum,
                            0, DUMMY_RANGE_CHECK_INDEX);
                }
            }

            for ( fail_list_count = 0; (fail_list_count < p_failed_erab_data->count) &&
                    (fail_list_count < MAX_ERAB_COUNT); fail_list_count++)
            {

                /* Update the performance Counter E-RAB Failed List */
                /* SPR 13468 Fix Start */
                if ( p_ue_context->m.erb_setup_resp_fail_cause_present )
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_erab_related_stats.erab_setup_stats.
                            erab_estab_init_fail_stats.erab_estabInitFailNbr_cause,
                            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause);
                }
                else
                {
                    /* SPR 13614 Fix Stop */
                    if ( p_ue_context->erab_ics_data.cause[counter].u.radioNetwork != 
                            s1ap_multiple_E_RAB_ID_instances )
                    {
                        RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                                g_rrc_stats.p_rrc_stats->
                                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                                p_gb_context->offset_uecc_mod_id].
                                rrc_uecc_cell_stats[p_ue_context->cell_index].
                                rrc_erab_related_stats.erab_setup_stats.
                                erab_estab_init_fail_stats.erab_estabInitFailNbr_cause,
                                p_ue_context->erab_ics_data.cause[counter]);
                    }
                    /* SPR 13614 Fix Stop */
                }
                /* SPR 16152 Fix Stop */
                /* SPR 13468 Fix Stop */

                /* SPR 13614 Fix Stop */
                if ( p_ue_context->erab_ics_data.cause[counter].u.radioNetwork != 
                        s1ap_multiple_E_RAB_ID_instances )
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.
                            erab_setup_stats.erab_estabInitFailNbr_sum,
                            0, DUMMY_RANGE_CHECK_INDEX);
                }
                /* SPR 13614 Fix Stop */
            }

            /* SPR 13614 Fix Start */
            if ( p_ue_context->erab_ics_fail_data.count != RRC_NULL )
            {
                for( index = 0; index < p_ue_context->erab_ics_fail_data.count; index++)
                {
                    RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_erab_related_stats.erab_setup_stats.
                            erab_estab_init_fail_stats.erab_estabInitFailNbr_cause,
                            p_ue_context->erab_ics_fail_data.cause[index]);

                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.
                            erab_setup_stats.erab_estabInitFailNbr_sum,
                            0, DUMMY_RANGE_CHECK_INDEX);
                }
            }
            /* SPR 13614 Fix Stop */
        }

		if(RRC_SUCCESS !=
				uecc_s1ap_build_and_send_initial_context_setup_failure(
					p_ue_context,
					uecc_ue_ctx_get_mme_ue_id(p_ue_context),
					p_ue_context->s1ap_ue_id,
					&p_ue_context->share_data.p_uecc_rm_data->cause,
					PNULL))
		{
			RRC_UECC_ICS_FSM_TRACE(RRC_FATAL,
					"[s1ap_InitialContextSetupFailure] delivery failure");
		}
	}

	if (UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT != failure_ev
			&&
			UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT != failure_ev)
	{
		/* Clear the share data in UE Context */
		if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
		{
			rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
			p_ue_context->share_data.p_uecc_rm_data = PNULL;
		}
	}

	/* Free the Current Proc data memory */
	if (p_ue_context->p_curr_proc_data != PNULL)
	{
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL; 
	} 

	/* SPS Start */
	if (p_ue_context->m.is_sps_recv_frm_rrm)
	{
		if (p_ue_context->p_sps_config != PNULL)
		{
			rrc_mem_free(p_ue_context->p_sps_config);
			p_ue_context->p_sps_config = PNULL;
		}

		/* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
		uecc_llim_build_and_send_mac_reconfig_complete_ind(
				p_ue_context->p_gb_context,
				p_ue_context,
				RRC_FAILURE);
	}
	/* SPS Stop */

	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			failure_ev,
			p_ue_context,
			PNULL);
	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_54728_stop */

/****************************************************************************
 * Function Name  : uecc_fsm_icsp_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_icsp_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_ue_context_release_command_t *p_ue_context_release_command =
            (rrc_s1ap_ue_context_release_command_t*)p_api;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Store data in UE context for future processing */
    p_ue_context->share_data.uecc_rcr_data.u.cause =
        p_ue_context_release_command->cause;

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_RELEASE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_icsp_reset_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_icsp_reset_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ICS_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_RESET_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_icsp_csc_cell_delete_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_CSC_DELETE_ALL_UE_REQ
 ****************************************************************************/
void uecc_fsm_icsp_csc_cell_delete_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ICS_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_CSC_CELL_DELETE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_icsp_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_icsp_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ICS_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17893_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_icsp_process_and_save_ue_radio_capability
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  UE_CapabilityRAT_ContainerList *p_rat_container_list - pointer to ue radio capability
 * Outputs        : None
 * Returns        : rrc_return_t
 * Description    : This is the handler function for the processing and saving ue radio capability by MME.
 ****************************************************************************/
static rrc_return_t uecc_fsm_icsp_process_and_save_ue_radio_capability(
        uecc_ue_context_t  *p_ue_context,
        UE_CapabilityRAT_ContainerList *p_rat_container_list
)
{
    OSRTDListNode*      p_node = PNULL;
    rrc_return_et       result = RRC_FAILURE;
    U8 i = 0;
    /* HD-FDD start */
    U8 counter;
    /* HD-FDD stop */
    /* SPR 13502 Fix Start */
    rrc_rrm_ue_capability_ind_t *p_rrc_rrm_ue_capability_ind = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rat_container_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_ue_capability_ind = (rrc_rrm_ue_capability_ind_t *)rrc_mem_get
                                  (sizeof(rrc_rrm_ue_capability_ind_t));
    if (PNULL == p_rrc_rrm_ue_capability_ind)
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_FATAL,
                "Memory Allocation Failure.");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }
    /* SES-508 Fix Start */
    else
    {
        memset_wrapper(p_rrc_rrm_ue_capability_ind, 0, sizeof(rrc_rrm_ue_capability_ind_t));
    }
    /* SES-508 Fix End */
    /*carsh_fix*/

    do
    {
        /*rrc_uecc_set_cell_index_in_rrm_api (p_ue_context->p_gb_context,
                p_rrc_rrm_ue_capability_ind,
                RRC_RRM_UE_CAPABILITY_IND,
                p_ue_context->cell_index);*/

        if( (p_rat_container_list->count < 1) || (p_rat_container_list->count > MAX_RAT_CAPABILITY))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,
                    "uecc_fsm_icsp_process_and_save_ue_radio_capability wrong list count [%u]",
                    p_rat_container_list->count);
            result = RRC_FAILURE;
            break;
        }

        /* storing in the uecc context */
        p_node = p_rat_container_list->head;

        for(i = 0 ; i < p_rat_container_list->count ; i++)
        {
            RRC_ASSERT(PNULL != p_node);
            RRC_ASSERT(PNULL != p_node->data);

            /* 36.300 section 18: The UTRAN capabilities, i.e. the INTER RAT 
             * HANDOVER INFO, include START-CS, START-PS and "predefined 
             * configurations", which are "dynamic" IEs. In order to avoid the
             * START values desynchronisation and the key replaying issue, the 
             * eNB always enquiry the UE UTRAN capabilities at transition from 
             * RRC_IDLE to RRC_CONNECTED and before Handover to UTRAN. The eNB 
             * does not upload the UE UTRAN capabilities to the MME.
             * 
             * 36.331 sec 10.2.2: ue-RadioAccessCapabilityInfo
             * Including E-UTRA, GERAN, and CDMA2000-1xRTT Bandclass radio 
             * access capabilities (separated). UTRA radio access capabilities 
             * are not included.
             *
             * eNB does not provide the UTRA capability to MME and can not use 
             * the START-CS, START-PS and "predefined configurations", received
             * in UTRA capability from MME. So, L3 is ignoring the UTRA 
             * capability received from MME because UTRA capability received 
             * from MME can't be used in any of the procedures.
             */
            /* building rat capability */
            if(RRC_RAT_TYPE_UTRA != ((UE_CapabilityRAT_Container *)(p_node->data))->rat_Type)
            {
                result = uecc_rrm_build_ue_rat_capability(p_ue_context,
                        &p_rrc_rrm_ue_capability_ind->radio_capability_info,p_node->data);
                if(result)
                {
                    if(RRC_FAILURE == uecc_save_ue_rat_capability(p_ue_context,p_node->data))
                    {
                        result = RRC_FAILURE;
                    }
                }
                if(!result)
                {
                    break;
                }
            }
            p_node = p_node->next;
        }

        /* Save the EUTRA UE Category in UE Context */
        p_ue_context->ue_category = p_rrc_rrm_ue_capability_ind->
            radio_capability_info.eutra_radio_capability_info.
            eutra_radio_capability_info_def.ue_category;
       /* 36321_CR0538 start */
        /* Save the EUTRA UE access_stratum_release in UE Context */
        p_ue_context->access_stratum_release = p_rrc_rrm_ue_capability_ind->radio_capability_info.
            eutra_radio_capability_info.eutra_radio_capability_info_def.access_stratum_release;
       /* 36321_CR0538 stop */
        
        if ((p_rrc_rrm_ue_capability_ind->radio_capability_info.\
            eutra_radio_capability_info.eutra_radio_capability_info_def.\
            access_stratum_release == rrm_rel8) &&
           (p_ue_context->configure_integrity_protection.algorithm_id
            == RRC_PDCP_INT_ALG_EIA0))
        {
            RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
              "[uecc_fsm_icsp_process_and_save_ue_radio_capability] EIA0 can't be configured for Rel8 UE.");
            result = RRC_FAILURE;
            break;
        }
        /* HD-FDD start */
        p_ue_context->ue_mode = 0;/* Reset */
        for(counter = 0; counter< p_rrc_rrm_ue_capability_ind->radio_capability_info.eutra_radio_capability_info.
                eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.count; counter++)
        {
            if(p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                    p_csc_init_setup_ind->sib_type_1_Info.freq_band_indicator == 
                    p_rrc_rrm_ue_capability_ind->radio_capability_info.eutra_radio_capability_info.
                    eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.
                    supported_band_eutra[counter].band_eutra)
            {
                if(p_rrc_rrm_ue_capability_ind->radio_capability_info.eutra_radio_capability_info.
                       eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra.
                        supported_band_eutra[counter].half_duplex == 1)
                {
                p_ue_context->ue_mode = 1;
                break;
                }
            }

        }

        if ((DUPLEXING_MODE_FDD == p_ue_context->p_gb_context->
                p_p_csc_context[p_ue_context->cell_index]->p_csc_init_setup_ind->duplexing_mode) &&
                (p_ue_context->ue_mode == 1))
        {
             result = RRC_PARTIAL_SUCCESS;
	     /* Coverity Fix 83394 Start */
	     if ( PNULL != p_rrc_rrm_ue_capability_ind )
	     {
             rrc_mem_free (p_rrc_rrm_ue_capability_ind);
	         p_rrc_rrm_ue_capability_ind = PNULL;
	     }
    	     RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
	     /* Coverity Fix 83394 Stop */
             return result;
        }
        /* HD-FDD stop */
    
    }   while(0);

    if(result)
    {
        p_rrc_rrm_ue_capability_ind->ue_Index = p_ue_context->ue_index;

        result = rrc_rrm_il_send_rrc_rrm_ue_capability_ind(
                p_rrc_rrm_ue_capability_ind,
                RRC_UECC_MODULE_ID,
                RRC_RRM_MODULE_ID,
                uecc_ue_ctx_get_op_transction_id(p_ue_context),
                p_ue_context->cell_index);
    }
    if (PNULL != p_rrc_rrm_ue_capability_ind)
    {
        rrc_mem_free (p_rrc_rrm_ue_capability_ind);
        p_rrc_rrm_ue_capability_ind = PNULL;
    }
    /* SPR 13502 Fix Stop */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return  result;
}
/*SPR_17893_END*/

/****************************************************************************
 * Function Name  : uecc_icsp_process_initial_ue_ctx_setup_msg
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function processes S1AP_INITIAL_CTX_SETUP_IND
 *                  message:
 *                  - check message content;
 *                  - store IEs in UE context;
 ****************************************************************************/
rrc_return_t uecc_fsm_icsp_process_initial_ue_ctx_setup_msg(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_return_t    result = RRC_FAILURE;
    rrc_s1ap_initial_context_setup_request_t *p_msg=
        (rrc_s1ap_initial_context_setup_request_t*)p_api;
    U8 e_rab_list_count = 0;

    s1ap_E_RABToBeSetupItemCtxtSUReq *p_e_rab_list[MAX_ERAB_COUNT] = {PNULL};
    /*SPR_17893_START*/
    UECapabilityInformation             ue_capability_info;
    UERadioAccessCapabilityInformation  ue_radio_access_capability_info;
    /*SPR_17893_END*/
    LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT
                                        *p_event_ciph_integrity_algo = PNULL;
    /* SPR 21428 Fix Start */
    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABToBeSetupListCtxtSUReq_element *p_erab_elem = PNULL;
    s1ap_E_RABToBeSetupListCtxtSUReq *p_e_rab_to_be_setup_list = PNULL;
    /* SPR 21428 Fix Stop */

    s1ap_UEAggregateMaximumBitrate_iE_Extensions    *p_iE_Extensions_list =
        PNULL;
    OSRTDListNode* p_node_ext = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* returning as uecc_global_context is null */
        return RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UE_START_TIME_PERFORMANCE_COUNTER(
          p_ue_context, p_ue_context->ue_stats.ue_timer_stats[RRC_UE_ERAB_SETUP_TIME], 
          RRC_UE_ERAB_SETUP_TIME);

    /* Memory allocated dynamically for curr_proc_data */
    p_ue_context->p_curr_proc_data = (curr_proc_data_t *)rrc_mem_get(sizeof(
                                          curr_proc_data_t));

    if ( PNULL == p_ue_context->p_curr_proc_data )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                "Memory Allocation Failure!!");
        result = RRC_FAILURE;
        return result;

    }
    else
    {
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        /* Set Current Procedure to INITIAL CONTEXT SETUP */
        p_ue_context->p_curr_proc_data->t = UECC_ICS_PROC;
    } 

    
    /* Init ASN1 context: before using p_ue_context->s1ap_asn1_ctx 
     * for copying encryption and integration algorithms */
    if ( RT_OK != rtInitContext(&p_ue_context->s1ap_asn1_ctx))
    {
        /* Drop message */
        RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
                "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] ASN context initialization failed.");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        result = RRC_FAILURE;
        return result;
    }


    /* Check message */
    /* SPR 17146 Fix Start */
    /* Filter the E_RAB List with Failure Conditions */ 
    result = uecc_fsm_icsp_process_erab_list(
        &p_msg->e_rab_to_be_setup_list,
        p_e_rab_list,    
        &e_rab_list_count,
        p_ue_context);
    /* SPR 20007 : CID 107192 fix start */
    if(RRC_FAILURE == result)
    {
        RRC_UECC_ICS_FSM_TRACE(RRC_DETAILED,
                "Failure returned from uecc_fsm_icsp_process_erab_list");
    }
    /* SPR 20007 : CID 107192 fix end */

    /* SPR 17146 Fix Stop */

    /*  CR0860 start */
    if ((CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG &
            p_ue_context->p_gb_context->p_p_csc_context[
            p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
            cell_access_related_info.presence_bitmask) &&
            (p_ue_context->p_gb_context->p_p_csc_context[
             p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
             cell_access_related_info.csg_indication == RRC_FALSE) && 
            ((1 == p_ue_context->m.rel_of_mme_present) &&
                    (RRC_MME_REL10 == p_ue_context->rel_of_mme) ))
    {
        if (!(RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT & p_msg->bitmask))
        {
            RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
                "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] Cell is a hybrid cell and CSG Membership Status IE not present, send failure.");
            result = RRC_FAILURE;
            /*+ SPR 22111 Fix*/
            /* Free ASN Context */
            rtFreeContext(&p_ue_context->s1ap_asn1_ctx);
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            /*- SPR 22111 Fix*/
            return result; 
        }
    }
    /*  CR0860 end */
    if ((CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG &
            p_ue_context->p_gb_context->p_p_csc_context[
            p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
            cell_access_related_info.presence_bitmask) &&
            (p_ue_context->p_gb_context->p_p_csc_context[
             p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
             cell_access_related_info.csg_indication == RRC_TRUE) && 
            (RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT & p_msg->bitmask) && 
            (p_msg->csg_membership_status == s1ap_not_member ))
    {
            RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
                "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] CSG is not member of CSG Cell.");
            result = RRC_FAILURE;
            /*+ SPR 22111 Fix*/
            /* Free ASN Context */
            rtFreeContext(&p_ue_context->s1ap_asn1_ctx);
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            /*- SPR 22111 Fix*/
            return result; 
    }

    /*SPR_17484_START*/
    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
                    p_ue_context->cell_index))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        
        p_ue_context->share_data.p_uecc_rm_data->cause.t =
            T_s1ap_Cause_radioNetwork;
        p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
           s1ap_cell_not_available;
        result = RRC_FAILURE;
        /*+ SPR 22111 Fix*/
        /* Free ASN Context */
        rtFreeContext(&p_ue_context->s1ap_asn1_ctx);
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        /*- SPR 22111 Fix*/
        return result; 
    }
    /*SPR_17484_END*/

    /* Check security capabilities */
    result = uecc_fsm_icsp_check_and_process_security_capability(
            p_ue_context,
            (void*)(&p_msg->ue_security_capabilities));
    if (RRC_SUCCESS != result)
    {
        p_ue_context->share_data.p_uecc_rm_data->cause.t =
            T_s1ap_Cause_radioNetwork;
        /* SPR 14196 Fix Start */
        p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
           s1ap_encryption_and_or_integrity_protection_algorithms_not_supported;
        /* SPR 14196 Fix Stop */
        
        /* SPR 21428 Fix Start */
        p_e_rab_to_be_setup_list = &p_msg->e_rab_to_be_setup_list;
        for ( p_node = p_e_rab_to_be_setup_list->head;
          (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT);
             p_node=p_node->next )
        {
            p_erab_elem=(s1ap_E_RABToBeSetupListCtxtSUReq_element*)p_node->data;
            if ( (p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1 == PNULL))
            {
                continue;
            }
            if ((PNULL != p_ue_context->p_gb_context) && (PNULL != g_rrc_stats.p_rrc_stats))
            {
                RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].
                        rrc_erab_related_stats.erab_setup_stats.
                        erab_estab_init_fail_stats.erab_estabInitFailNbr_cause,
                        p_ue_context->share_data.p_uecc_rm_data->cause);

                RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                        g_rrc_stats.p_rrc_stats->
                        rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                        p_gb_context->offset_uecc_mod_id].
                        rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.
                        erab_setup_stats.erab_estabInitFailNbr_sum,
                        0, DUMMY_RANGE_CHECK_INDEX);
            }
        }
        /* SPR 21428 Fix Stop */
        
        /* Generating Protocol Event
         * LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH*/
        p_event_ciph_integrity_algo = 
            rrc_mem_get(sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT));
        if (p_event_ciph_integrity_algo)
        {
            /* SES-508 Fix Start */
            /* Code Removed */
            /* SES-508 Fix End */
            p_event_ciph_integrity_algo->header.length = 
                sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT);
            p_event_ciph_integrity_algo->header.event_id = 
                LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH;

            if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) && 
                (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
                && (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                        p_csc_init_setup_ind))
            {
                l3_memcpy_wrapper(&p_event_ciph_integrity_algo->header.EVENT_PARAM_CELL_ID, 
                        p_ue_context->p_gb_context->
                        p_p_csc_context[p_ue_context->cell_index]->
                        p_csc_init_setup_ind->sib_type_1_Info.
                        cell_access_related_info.cell_Id, sizeof(U32));
            }
            p_event_ciph_integrity_algo->header.EVENT_PARAM_EUTRANCELL_FROID = 
                p_event_ciph_integrity_algo->header.EVENT_PARAM_CELL_ID;

            if (p_ue_context->m.mme_ue_id_present)
            {
                p_event_ciph_integrity_algo->header.EVENT_PARAM_MMES1APID = p_ue_context->mme_ue_id;
            }
            
            p_event_ciph_integrity_algo->header.EVENT_PARAM_RAC_UE_REF = p_ue_context->s1ap_ue_id;
            
            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_ciph_integrity_algo);
        }

        /*+ SPR 22111 Fix*/
        /* Free ASN Context */
        rtFreeContext(&p_ue_context->s1ap_asn1_ctx);
        /*- SPR 22111 Fix*/
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return result;
    }

    /* Store message data in UE context for future processing */
    p_ue_context->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL =
        p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL;
    p_ue_context->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL =
        p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL;

    #ifdef ENDC_ENABLED
    if ( p_msg->ue_aggregate_maximum_bitrate.m.iE_ExtensionsPresent )
    {
        p_iE_Extensions_list  = &(p_msg->ue_aggregate_maximum_bitrate.iE_Extensions); 
        p_node_ext = p_iE_Extensions_list->head;
        for ( ; PNULL != p_node_ext; p_node_ext = p_node_ext->next)
        {
            s1ap_UEAggregateMaximumBitrate_iE_Extensions_element *p_elem =
                /* coverity_280002_fix_start*/
                (s1ap_UEAggregateMaximumBitrate_iE_Extensions_element*)p_node_ext->data;
            /* coverity_280002_fix_end*/

            RRC_ASSERT( PNULL != p_elem );

            if ( p_elem->extensionValue.t == T261__UEAggregate_MaximumBitrates_ExtIEs_1 ) 
            {
                p_ue_context->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDLExt =
                        p_elem->extensionValue.u._UEAggregate_MaximumBitrates_ExtIEs_1;

                /* SET Ext UE AMBR in UE context to 1 */ 
                p_ue_context->m.ext_ue_agg_max_bit_rate_updated = RRC_ONE;
            }

            if ( p_elem->extensionValue.t == T261__UEAggregate_MaximumBitrates_ExtIEs_2 ) 
            {
                p_ue_context->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateULExt =
                        p_elem->extensionValue.u._UEAggregate_MaximumBitrates_ExtIEs_2;

                /* SET Ext UE AMBR in UE context to 1 */ 
                p_ue_context->m.ext_ue_agg_max_bit_rate_updated = RRC_ONE;
            }
        }
    }
    #endif

    /* SET UE AMBR in UE context to 1 */ 
    p_ue_context->m.ue_agg_max_bit_rate_updated = 1;

    asn1Copy_s1ap_EncryptionAlgorithms(&p_ue_context->s1ap_asn1_ctx,
        &p_msg->ue_security_capabilities.encryptionAlgorithms,
        &p_ue_context->ue_security_capabilities.encryptionAlgorithms);
    asn1Copy_s1ap_IntegrityProtectionAlgorithms(&p_ue_context->s1ap_asn1_ctx,
        &p_msg->ue_security_capabilities.integrityProtectionAlgorithms,
        &p_ue_context->ue_security_capabilities.integrityProtectionAlgorithms);

    asn1Copy_s1ap_SecurityKey(&p_ue_context->s1ap_asn1_ctx,
        &p_msg->security_key,
        &p_ue_context->security_key);

    /*SPR_17893_START*/
    /* UE Radio Capability */
    if (RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_UE_RADIO_CAPABILITY_PRESENT
            & p_msg->bitmask)
    {
        do
        {
            pu_setBuffer(&p_ue_context->s1ap_asn1_ctx, (OSOCTET *)p_msg->ue_radio_capability.data,
                    p_msg->ue_radio_capability.numocts, ASN_RRC_BUF_TYPE);

            if ( RT_OK != asn1PD_UERadioAccessCapabilityInformation(&p_ue_context->s1ap_asn1_ctx, &ue_radio_access_capability_info) )
            {
                /* Drop message */
                RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                        "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] ASN decode failed.");
                result = RRC_FAILURE;
                break;
            }

            /*Bug 9894 fix start*/
            if(T_UERadioAccessCapabilityInformation_criticalExtensions_c1 ==
                    ue_radio_access_capability_info.criticalExtensions.t)
            {

                pe_GetMsgPtr(
                        &p_ue_context->s1ap_asn1_ctx,
                        (OSINT32*)&ue_radio_access_capability_info.criticalExtensions.u.c1->u.ueRadioAccessCapabilityInformation_r8->ue_RadioAccessCapabilityInfo.numocts);


                pu_setBuffer(&p_ue_context->s1ap_asn1_ctx, (OSOCTET *)ue_radio_access_capability_info.criticalExtensions.u.c1->u.ueRadioAccessCapabilityInformation_r8->ue_RadioAccessCapabilityInfo.data,
                        ue_radio_access_capability_info.criticalExtensions.u.c1->u.ueRadioAccessCapabilityInformation_r8->ue_RadioAccessCapabilityInfo.numocts, ASN_RRC_BUF_TYPE);

                if ( RT_OK != asn1PD_UECapabilityInformation(&p_ue_context->s1ap_asn1_ctx, &ue_capability_info) )
                {
                    /* Drop message */
                    RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                            "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] ASN decode failed.");
                    result = RRC_FAILURE;
                    break;
                }
                result = uecc_fsm_icsp_process_and_save_ue_radio_capability(p_ue_context,
                        &ue_capability_info.criticalExtensions.u.c1->
                        u.ueCapabilityInformation_r8->ue_CapabilityRAT_ContainerList);
                /*SPR 21947 Start */
                if(result == RRC_FAILURE)
                {
                   break;
                }
                /*SPR 21947 Stop */
            }
            else
            {
                RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                        "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg]  Type should be"
                        "of T_UERadioAccessCapabilityInformation_criticalExtensions_c1");
                result = RRC_FAILURE;
                break;
            }
            /*Bug 9894 fix stop*/

        }while(0);
        /* HD-FDD start */
        if (RRC_PARTIAL_SUCCESS == result)
        {
            RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                    "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] UE Capability processing failed. "
                    "So failing ICS.");
            result = RRC_FAILURE;
            
            /*SPR 21947 Stop */
            /*+ SPR 22111 Fix*/
            /* Free ASN Context */
            rtFreeContext(&p_ue_context->s1ap_asn1_ctx);
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            /*- SPR 22111 Fix*/
            return result;
        }
        /* HD-FDD stop */

        if (RRC_SUCCESS != result)
        {
            RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                    "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] ASN decode failed for ue capabilities given by MME in initial context.");

            p_ue_context->m.access_stratum_release = 0;
            
            /*+ SPR 22111 Fix*/
            /* Free ASN Context */
            rtFreeContext(&p_ue_context->s1ap_asn1_ctx);
            /*- SPR 22111 Fix*/
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return result;
        }
        else
        {

            RRC_UECC_ICS_FSM_TRACE(RRC_BRIEF,
                    "[uecc_fsm_icsp_process_initial_ue_ctx_setup_msg] ASN decode success for ue capabilities given by MME in initial context.");

            p_ue_context->m.access_stratum_release = 1;
        }
    }
    /*SPR_17893_END*/

    /* Generate integrity/security keys */

    rrc_generate_key(
        K_RRC_INT_ALG_P1,
        (U8)p_ue_context->configure_integrity_protection.algorithm_id,
        &p_ue_context->configure_integrity_protection.key[0],
        &p_ue_context->security_key.data[0]);

    rrc_generate_key(
        K_RRC_ENC_ALG_P1,
        (U8)p_ue_context->configure_ciphering.algorithm_id,
        &p_ue_context->configure_ciphering.key[0],
        &p_ue_context->security_key.data[0]);

    rrc_generate_key(
        K_UP_ENC_ALG_P1,
        (U8)p_ue_context->configure_ciphering_user_plane.algorithm_id,
        &p_ue_context->configure_ciphering_user_plane.key[0],
        &p_ue_context->security_key.data[0]);

        /*SPR_17143_START*/
        RRC_UECC_ICS_FSM_TRACE(RRC_INFO,
                    "SRB Key Generation Inputs during ICS: "
                    "Integrity Algo Id[%u] ciphering algo[%u] "
                    "security_key size[%u] key  - "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                    p_ue_context->configure_integrity_protection.algorithm_id,
                    p_ue_context->configure_ciphering.algorithm_id,
                    p_ue_context->security_key.numbits,
                    p_ue_context->security_key.data[0],
                    p_ue_context->security_key.data[1],
                    p_ue_context->security_key.data[2],
                    p_ue_context->security_key.data[3],
                    p_ue_context->security_key.data[4],
                    p_ue_context->security_key.data[5],
                    p_ue_context->security_key.data[6],
                    p_ue_context->security_key.data[7],
                    p_ue_context->security_key.data[8],
                    p_ue_context->security_key.data[9],
                    p_ue_context->security_key.data[10],
                    p_ue_context->security_key.data[11],
                    p_ue_context->security_key.data[12],
                    p_ue_context->security_key.data[13],
                    p_ue_context->security_key.data[14],
                    p_ue_context->security_key.data[15],
                    p_ue_context->security_key.data[16],
                    p_ue_context->security_key.data[17],
                    p_ue_context->security_key.data[18],
                    p_ue_context->security_key.data[19],
                    p_ue_context->security_key.data[20],
                    p_ue_context->security_key.data[21],
                    p_ue_context->security_key.data[22],
                    p_ue_context->security_key.data[23],
                    p_ue_context->security_key.data[24],
                    p_ue_context->security_key.data[25],
                    p_ue_context->security_key.data[26],
                    p_ue_context->security_key.data[27],
                    p_ue_context->security_key.data[28],
                    p_ue_context->security_key.data[29],
                    p_ue_context->security_key.data[30],
                    p_ue_context->security_key.data[31]);

        RRC_UECC_ICS_FSM_TRACE(RRC_INFO, "SRB Integrity Key Data: "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                p_ue_context->configure_integrity_protection.key[0],
                p_ue_context->configure_integrity_protection.key[1],
                p_ue_context->configure_integrity_protection.key[2],
                p_ue_context->configure_integrity_protection.key[3],
                p_ue_context->configure_integrity_protection.key[4],
                p_ue_context->configure_integrity_protection.key[5],
                p_ue_context->configure_integrity_protection.key[6],
                p_ue_context->configure_integrity_protection.key[7],
                p_ue_context->configure_integrity_protection.key[8],
                p_ue_context->configure_integrity_protection.key[9],
                p_ue_context->configure_integrity_protection.key[10],
                p_ue_context->configure_integrity_protection.key[11],
                p_ue_context->configure_integrity_protection.key[12],
                p_ue_context->configure_integrity_protection.key[13],
                p_ue_context->configure_integrity_protection.key[14],
                p_ue_context->configure_integrity_protection.key[15]);

        RRC_UECC_ICS_FSM_TRACE(RRC_INFO, "SRB Ciphering Key Data: "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                p_ue_context->configure_ciphering.key[0],
                p_ue_context->configure_ciphering.key[1],
                p_ue_context->configure_ciphering.key[2],
                p_ue_context->configure_ciphering.key[3],
                p_ue_context->configure_ciphering.key[4],
                p_ue_context->configure_ciphering.key[5],
                p_ue_context->configure_ciphering.key[6],
                p_ue_context->configure_ciphering.key[7],
                p_ue_context->configure_ciphering.key[8],
                p_ue_context->configure_ciphering.key[9],
                p_ue_context->configure_ciphering.key[10],
                p_ue_context->configure_ciphering.key[11],
                p_ue_context->configure_ciphering.key[12],
                p_ue_context->configure_ciphering.key[13],
                p_ue_context->configure_ciphering.key[14],
                p_ue_context->configure_ciphering.key[15]);

        RRC_UECC_ICS_FSM_TRACE(RRC_INFO, "User Plane Ciphering Key Data: "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                p_ue_context->configure_ciphering_user_plane.key[0],
                p_ue_context->configure_ciphering_user_plane.key[1],
                p_ue_context->configure_ciphering_user_plane.key[2],
                p_ue_context->configure_ciphering_user_plane.key[3],
                p_ue_context->configure_ciphering_user_plane.key[4],
                p_ue_context->configure_ciphering_user_plane.key[5],
                p_ue_context->configure_ciphering_user_plane.key[6],
                p_ue_context->configure_ciphering_user_plane.key[7],
                p_ue_context->configure_ciphering_user_plane.key[8],
                p_ue_context->configure_ciphering_user_plane.key[9],
                p_ue_context->configure_ciphering_user_plane.key[10],
                p_ue_context->configure_ciphering_user_plane.key[11],
                p_ue_context->configure_ciphering_user_plane.key[12],
                p_ue_context->configure_ciphering_user_plane.key[13],
                p_ue_context->configure_ciphering_user_plane.key[14],
                p_ue_context->configure_ciphering_user_plane.key[15]);
        /*SPR_17143_END*/

    /* Store E-RAB configuration*/

    /* SPR 17146 Fix Start */
    /* Code Removed */
    /* SPR 17146 Fix Stop */

    /* Allocate E_RAB's in UE Context Dynamically */
    if ( (e_rab_list_count != 0 ) && (result == RRC_SUCCESS))
    {
        result = uecc_fsm_icsp_allocate_erab_context(
                     p_e_rab_list,    
                     e_rab_list_count,
                     p_ue_context);    
    }
    else
    {
        result = RRC_FAILURE;
    }

    /* Handover restriction list store in UE context and is sent to
       RRM during ERAB setup request */
    if (p_msg->bitmask &
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_HANDOVER_RESTRICTION_LIST_PRESENT)
    {
        p_ue_context->m.ho_restriction_list_present = RRC_TRUE;
        asn1Copy_s1ap_HandoverRestrictionList(
            &p_ue_context->s1ap_asn1_ctx,
            &p_msg->handover_restriction_list,
            &p_ue_context->ics_handover_restriction_list);
    }

    /* cr_761 change */
    
    if(RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_REGISTERED_LAI_PRESENT
            & p_msg->bitmask)
    {
        //Memory Allocation
        p_ue_context->p_registered_lai = (registered_lai_t *)rrc_mem_get(sizeof(registered_lai_t));
        if(PNULL == p_ue_context->p_registered_lai)
        {
                RRC_TRACE(RRC_WARNING,"%s: Memory Allocation failed",__FUNCTION__);
                return RRC_FAILURE;
        }

        if (PNULL != p_ue_context->p_registered_lai)
        {
            p_ue_context->m.registered_lai_present = 1;

            p_ue_context->p_registered_lai->plmn_identity.num = p_msg->registered_lai.pLMNidentity.numocts;

            l3_memcpy_wrapper(p_ue_context->p_registered_lai->plmn_identity.data,
                    p_msg->registered_lai.pLMNidentity.data,
                    p_msg->registered_lai.pLMNidentity.numocts);

            p_ue_context->p_registered_lai->lac.num = p_msg->registered_lai.lAC.numocts;

            l3_memcpy_wrapper(p_ue_context->p_registered_lai->lac.data,
                    p_msg->registered_lai.lAC.data,
                    p_msg->registered_lai.lAC.numocts);
        }
    }
    if (p_msg->bitmask &
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CS_FALLBACK_INDICATOR_PRESENT)
    {
        p_ue_context->cs_fallback_indicator = 
            p_msg->cs_fallback_indicator;
        p_ue_context->m.CsfallbackIndicator = RRC_TRUE;
    }

    if (p_msg->bitmask &
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT)
    {
        p_ue_context->srvcc_operation_possible =    
            p_msg->srvcc_operation_possible;
        p_ue_context->m.SrvccOP = RRC_TRUE;
            
    }

    if (p_msg->bitmask &
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT)
    {
        p_ue_context->csg_membership_status =    
            p_msg->csg_membership_status;
        p_ue_context->m.CsgMembershipStatus = RRC_TRUE;
            
    }
    if (p_msg->bitmask &
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT)
    {
        
        p_ue_context->spid =
            p_msg->subscriber_profile_id_for_rfp;
        p_ue_context->m.SpId = RRC_TRUE;
    }
            

    if (p_msg->bitmask &
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CS_FALLBACK_INDICATOR_PRESENT)
    {
        p_ue_context->cs_fallback_indicator =
            p_msg->cs_fallback_indicator;
        p_ue_context->m.CsfallbackIndicator =
            RRC_TRUE;
    }


    /* UF: store other fields in future releases */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_icsp_check_and_process_security_capability
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*  p_api
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This is the function processes UE security capability IE
 *                  and configures UE ciphering and integrity protection algs
 ****************************************************************************/
rrc_return_t uecc_fsm_icsp_check_and_process_security_capability(
        uecc_ue_context_t*  p_ue_context,
        void*  p_api
)
{
    rrc_return_t            result = RRC_FAILURE;
    U16     security_algorithms = UECC_CIPHERING_EEA0_BIT;
    U8      algo_index  = 0;
    U8      mme_integrity_algo = 0;
    U8      mme_ciphering_algo = 0;
    U8      integrity_algo_flag = 0;
    U8      ciphering_supported = 0;
    rrc_bool_t  ims_emergency_support = RRC_FALSE;
    s1ap_UESecurityCapabilities *p_ue_security_capabilities
                    = (s1ap_UESecurityCapabilities *)p_api;

    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    /*SPR_17484_START*/
    /* Code Removed */
    /*SPR_17484_END*/

    do
    {
        /*
         * Check and select ciphering alg
         * */

        if (0 !=
                rtxTestBit(p_ue_security_capabilities->encryptionAlgorithms.data,
                    p_ue_security_capabilities->encryptionAlgorithms.numbits,0))
        {
            /* 128-EEA1 */
            security_algorithms |= UECC_CIPHERING_EEA1_BIT;
        }

        if (0 !=
                rtxTestBit(p_ue_security_capabilities->encryptionAlgorithms.data,
                    p_ue_security_capabilities->encryptionAlgorithms.numbits,1))
        {
            /* 128-EEA2 */
            security_algorithms |= UECC_CIPHERING_EEA2_BIT;
        }
        for (algo_index = 0; algo_index < p_ue_context->p_gb_context->
                supported_security_algo_per_cell.supported_security_algorithms[p_ue_context->cell_index].
                ciphering_algorithms.num_algorithms; algo_index++)
        {
            mme_ciphering_algo = (U8)(1 << p_ue_context->p_gb_context->
                    supported_security_algo_per_cell.
                    supported_security_algorithms[p_ue_context->cell_index].ciphering_algorithms.
                    algorithms[algo_index]);

            /* Check its presence in MME side */
            if (mme_ciphering_algo & security_algorithms)
            {
                p_ue_context->configure_ciphering_user_plane.algorithm_id =
                    p_ue_context->p_gb_context->supported_security_algo_per_cell.
                    supported_security_algorithms[p_ue_context->cell_index].
                    ciphering_algorithms.algorithms[algo_index];
                p_ue_context->configure_ciphering.algorithm_id =
                    p_ue_context->p_gb_context->supported_security_algo_per_cell.
                    supported_security_algorithms[p_ue_context->cell_index].
                    ciphering_algorithms.algorithms[algo_index];
                RRC_UECC_ICS_FSM_TRACE(RRC_BRIEF,
                        "Ciphering algorithms %d selected",
                        p_ue_context->configure_ciphering.algorithm_id);
                /* Set ciphering supported flag */
                ciphering_supported = 1;
                break;    
            }
        }
        if(!ciphering_supported)
        {
            RRC_UECC_ICS_FSM_TRACE(RRC_BRIEF, 
                    "Ciphering algorithm not matched between oam and mme");
            break;
        }
        /* Check and select integrity protection alg */
        if ( ( 0 == p_ue_context->rcs_establishmentCause ) &&
             ( (RRC_TRUE == !(strncmp_wrapper((const S8 *)(p_ue_context->uecc_fsm.p_current_state->s_name),
                  (const S8 *)("UECC_UE_S1_CONNECTION_SETUP_ONGOING"),
                    strlen_wrapper((const Char8*)p_ue_context->uecc_fsm.p_current_state->s_name))))
                    || (RRC_TRUE == !(strncmp_wrapper((const S8 *)(p_ue_context->uecc_fsm.p_current_state->s_id),
                    (const S8 *)("M_ICS_O"), strlen_wrapper((const Char8*)p_ue_context->uecc_fsm.p_current_state->s_id)))) ) )

        {
            security_algorithms = UECC_INTEGRITY_PROTECTION_EIA0_BIT;
        }
	    else if ( (PNULL != p_ue_context->ho_info.p_ho_info) &&
	            (PNULL != p_ue_context->ho_info.p_ho_info->p_trg_ho_info) &&
		      (1 == p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
		        rrc_s1ap_ho_request.source_to_target_transparent_container.
			 source_enb_to_target_enb_transparent_container.rRC_Container.m.as_ConfigPresent) &&
		       (eia0_v920 == p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
		        rrc_s1ap_ho_request.source_to_target_transparent_container.
			 source_enb_to_target_enb_transparent_container.rRC_Container.as_Config.
			     sourceSecurityAlgorithmConfig.integrityProtAlgorithm) )
        {
            security_algorithms = UECC_INTEGRITY_PROTECTION_EIA0_BIT;
        }
	    else
        {
/* SPR 12893 Fix Start */
            /* Acc to section 15.2 of Spec 33.401 v a20, unauthenticated
             * emergency calls are only allowed if eNodeB supports "NULL
             * Integrity (EIA0)" algo otherwise "authenticated" emergency calls
             * are established. */
            security_algorithms = RRC_NULL;
        }
        if (0 !=
                rtxTestBit(
                    p_ue_security_capabilities->integrityProtectionAlgorithms.data,
                    p_ue_security_capabilities->integrityProtectionAlgorithms.numbits,
                    0))
        {
            /* 128-EIA1 */
            security_algorithms |= UECC_INTEGRITY_PROTECTION_EIA1_BIT;
        }

        if (0 !=
                rtxTestBit(
                    p_ue_security_capabilities->integrityProtectionAlgorithms.data,
                    p_ue_security_capabilities->integrityProtectionAlgorithms.numbits,
                    1))
        {
            /* 128-EIA2 */
            security_algorithms |= UECC_INTEGRITY_PROTECTION_EIA2_BIT;
        }
/* SPR 12893 Fix Stop */


        if ( (0 == security_algorithms) &&
                (0 == p_ue_context->p_gb_context->supported_security_algo_per_cell.
                 supported_security_algorithms[p_ue_context->cell_index].
                 integrity_algorithms.num_algorithms))
        {
            /* MME don't provide valid integrity protection alg- Failure */
            RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                    "MME don't provide valid integrity protection algorithms");
            break;
        }

        for (algo_index = 0; algo_index < p_ue_context->p_gb_context->
                supported_security_algo_per_cell.
                supported_security_algorithms[p_ue_context->cell_index].
                integrity_algorithms.num_algorithms;algo_index++)
        {
            mme_integrity_algo = (U8)(1 << p_ue_context->p_gb_context->
                    supported_security_algo_per_cell.
                    supported_security_algorithms[p_ue_context->cell_index].
                    integrity_algorithms.algorithms[algo_index]);

            /* Check its presence in MME side */
            if(mme_integrity_algo & security_algorithms)
            {
                p_ue_context->configure_integrity_protection.algorithm_id =
                    p_ue_context->p_gb_context->supported_security_algo_per_cell.
                    supported_security_algorithms[p_ue_context->cell_index].
                    integrity_algorithms.algorithms[algo_index];
                RRC_UECC_ICS_FSM_TRACE(RRC_BRIEF,
                        "Integrity protection algorithms %d selected",
                        p_ue_context->configure_integrity_protection.algorithm_id);
                integrity_algo_flag = 1;
                break;
            }
        }
        if (RRC_PDCP_INT_ALG_EIA0 == p_ue_context->configure_integrity_protection.algorithm_id)
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
            {
                if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                        [p_ue_context->cell_index])
                {
                    if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                            [p_ue_context->cell_index]->p_csc_init_setup_ind)
                    {
                        if (SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG & 
                                p_ue_context->p_gb_context->p_p_csc_context[
                                p_ue_context->cell_index]->p_csc_init_setup_ind->
                                sib_type_1_Info.presence_bitmask)
                        {
                            if (SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG &
                                    p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                    cell_index]->p_csc_init_setup_ind->sib_type_1_Info.\
                                    non_critical_extension.presence_bitmask)
                            {
                                if (SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG &
                                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                                        cell_index]->p_csc_init_setup_ind->sib_type_1_Info.\
                                        non_critical_extension.non_critical_extension.presence_bitmask)
                                {
                                    ims_emergency_support = RRC_TRUE;
                                }
                            }
                        }
                    }
                }
            }
            if ( (RRC_TRUE == ims_emergency_support) &&
                    (0 == integrity_algo_flag) )
            {
                /* eNodeB doesnot support EIA0 while provides IMS Emergency Support */
                RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                        "eNodeB doesnot support EIA0 while provides IMS Emergency Support");
                break;
            }
            if ( (RRC_FALSE == ims_emergency_support) &&
                    (1 == integrity_algo_flag) )
            {
                /* eNodeB doesnot IMS Emergency while MME supports EIA0 integrity algo */
                RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                        "eNodeB doesnot provide IMS Emergency Support while supports EIA0 algo");
                break;
            }
            if ( (1 == p_ue_context->m.rel_of_mme_present) &&
                    (RRC_MME_REL8 == p_ue_context->rel_of_mme) )
            {
                /* MME is REL8 so doesnot support EIA0 */
                RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                        "MME is of REL8 so doesnot support EIA0");
                break;
            }
        }

        if( 0 == integrity_algo_flag )
        {
            RRC_UECC_ICS_FSM_TRACE(RRC_WARNING,
                    "Integrity protection algorithms not matching by OAM");
            break;
        }

        result = RRC_SUCCESS;
    }while (0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_icsp_rrm_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_icsp_rrm_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause cause;
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind = PNULL;
    /* SPR 13231 Fix Start */
    U8 counter = 0;
    /* SPR 13231 Fix Stop */


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_connection_release_ind = (rrc_rrm_ue_connection_release_ind_t *)p_api;

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    if (strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                (const S8 *)("UECC_RRM_RESOURCE_ADMISSION_ONGOING")))
    {
        /*If Waiting for SgNBAdditionRequestAck, initiate SgNBReleaseReq without SgNB-X2AP-ID*/
        if (!strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                    (const S8 *)("UECC_X2AP_SGNB_ADDITION_ONGOING")))
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_DC_PREP_TIMER,
                    &p_ue_context->timers_data);

            /* Put all E_RAB's in failed list from ERAB Setup List */
            uecc_fsm_rm_fail_action_mark_all_erab_failed(
                    p_ue_context,
                    RRC_NULL);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                /* Send SgNB RELEASE to SgNB */ 
                if (RRC_SUCCESS != 
                        build_and_send_sgnb_release_request(p_ue_context))
                {
                    RRC_UECC_ICS_FSM_TRACE(RRC_ERROR, 
                            "[%s],Unable to process SgNB Release Request", __FUNCTION__);
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
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->timers_data);
            
            /* Put all E_RAB's in failed list from ERAB Setup List */
            uecc_fsm_rm_fail_action_mark_all_erab_failed(
                    p_ue_context,
                    RRC_NULL);

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
                    RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
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

    if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
            &cause, 
            &p_ue_connection_release_ind->release_cause,
            p_ue_context))
    {
        /* Store data in UE context for future processing */
        if (
        RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT &
            p_ue_connection_release_ind->bitmask)
        {
            /* store in UE context */
            p_ue_context->p_redirected_carrier_info = PNULL;

            p_ue_context->p_redirected_carrier_info =
                (rrm_redirected_carrier_info_t*) 
                rrc_mem_get(sizeof(rrm_redirected_carrier_info_t));

            if (p_ue_context->p_redirected_carrier_info == PNULL)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name,
                        RRC_FATAL,"Memory Allocation Failure!!");
                return;
            }

            l3_memcpy_wrapper(p_ue_context->p_redirected_carrier_info,
                 &p_ue_connection_release_ind->redirected_carrier_info,
                 sizeof(rrm_redirected_carrier_info_t));
            
            p_ue_context->m.redirected_carrier_info_present = 1;
        }

        if (
        RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT &
            p_ue_connection_release_ind->bitmask)
        {
             /* store in UE context */
            p_ue_context->p_idle_mode_mobility_control = PNULL;

            p_ue_context->p_idle_mode_mobility_control =
                (rrm_idle_mode_mobility_control_info_t*)
                rrc_mem_get(sizeof(rrm_idle_mode_mobility_control_info_t));

            if (p_ue_context->p_idle_mode_mobility_control == PNULL)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_FATAL,"Memory Allocation Failure!!");
                return;
            }
         
             l3_memcpy_wrapper(p_ue_context->p_idle_mode_mobility_control,
                &p_ue_connection_release_ind->idle_mode_mobility_control,
                sizeof(rrm_idle_mode_mobility_control_info_t));
                 
             p_ue_context->m.idle_mode_mobility_control_present = 1;
        }

        if (RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT &
            p_ue_connection_release_ind->bitmask)
        {
          /* store in UE context */
          p_ue_context->rrc_connection_release_v890_ies =
            p_ue_connection_release_ind->rrc_connection_release_v890_ies;
          p_ue_context->m.connection_release_v890_ies = 1;
        }

        if (
        (RRC_RRM_UE_CONNECTION_RELEASE_IND_REDIRECTED_CARRIER_INFO_PRESENT |
        RRC_RRM_UE_CONNECTION_RELEASE_IND_IDLE_MODE_MOBILITY_CONTROL_PRESENT |
        RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V890_IES_PRESENT)
            & p_ue_connection_release_ind->bitmask)
        {
             p_ue_context->share_data.uecc_rcr_data.u.cause = cause;
             p_ue_context->share_data.p_uecc_rm_data->cause = cause;

			 /* SPR 13231 Fix Start */
			 for(counter = 0; counter < p_ue_context->erab_ics_data.count; counter++)
			 {
					 p_ue_context->erab_ics_data.cause[counter] = cause;
			 }
			 /* SPR 13231 Fix Stop */


             /* Go to release */
             uecc_fsm_finish_activity_failure(
                     p_ue_context,
                     (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_RRM_RELEASE_REQ_INT);
        }
        else
        {
            /* SPR 23293/SES-490 Fix Start */
            p_ue_context->share_data.uecc_rcr_data.u.cause = cause;
            p_ue_context->share_data.p_uecc_rm_data->cause = cause;

            /* Set the Flag to Ignore Release Procedure towards RRM */
            p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag = RRC_TRUE;

            /* SPR 13231 Fix Start */
            for(counter = 0; counter < p_ue_context->erab_ics_data.count; counter++)
            {     
                p_ue_context->erab_ics_data.cause[counter] = cause;
            }
            /* SPR 13231 Fix Stop */

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING, "RRC_RRM_UE_CONNECTION_RELEASE_IND received"
                    " without Redirect Carrier Info during ICS ongoing");

            /* Go to release */
            uecc_fsm_finish_activity_failure(p_ue_context,
                    (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT);
            /* SPR 23293/SES-490 Fix End */
        }
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING, "[RRC_RRM_UE_CONNECTION_RELEASE_IND] "
                "invalid cause value [%u][%u]",
                p_ue_connection_release_ind->release_cause.type,
                p_ue_connection_release_ind->release_cause.value);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_icsp_process_erab_list
 * Inputs         : s1ap_E_RABToBeSetupListCtxtSUReq - Pointer to ERAB List 
 *                  s1ap_E_RABToBeSetupItemCtxtSUReq - Array of ERAB items
 *                  U8 *e_rab_list_count - Pointer to No of ERABS
 *                  uecc_ue_context_t * - Pointer to Failed List
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the list with abnormal check 
                    condition.                    
 ****************************************************************************/
rrc_return_t  uecc_fsm_icsp_process_erab_list(
    s1ap_E_RABToBeSetupListCtxtSUReq *p_e_rab_to_be_setup_list, /* ERAB List */
    s1ap_E_RABToBeSetupItemCtxtSUReq *p_e_rab_list[],     /* ERAB Item Array */   
    U8                               *e_rab_list_count, /* No of ERAB List */
    uecc_ue_context_t                *p_ue_context         /* UECC UE context */
)
{
    U8 temp_erab_list[MAX_ERAB_COUNT] = {0};
    U8 temp_erab_list_status[MAX_ERAB_COUNT] = {0};
    U8 fail_list_count = 0;    
    U8 index = 0;
    /*SPR 21007 start*/
    U8 counter = 0;
    U8 count = 0;
    /*SPR 21007 stop*/
    U8 erab_counter = 1;
    U8 is_same_failure_cause_recv = RRC_TRUE;
  U8 qci_value = 0;

    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABToBeSetupListCtxtSUReq_element *p_erab_elem = PNULL;
    
    uecc_erab_failed_to_setup_list_t *p_failed_erab_data = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        failed_erab_data;
    
 
    RRC_ASSERT(PNULL != p_e_rab_to_be_setup_list);
    RRC_ASSERT(PNULL != p_failed_erab_data);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->erab_ics_data.count = 0;
    /* SPR 13614 Fix Start */
    memset_wrapper(&p_ue_context->erab_ics_fail_data, 0, sizeof(rrc_uecc_erab_data_ics_t));
    /* SPR 13614 Fix Stop */


    for ( p_node = p_e_rab_to_be_setup_list->head;
          (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT);
             p_node=p_node->next )
    {

        p_erab_elem=(s1ap_E_RABToBeSetupListCtxtSUReq_element*)p_node->data;
        if ( (p_erab_elem == PNULL) || (p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1 == PNULL))
        {
            continue;
        }

       /*SPR_18582_FIX_START*/
        temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID]++;    
          /* code removed */ 
       /*SPR_18582_FIX_END*/
        qci_value = p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RABlevelQoSParameters.qCI;
        if ((PNULL != p_ue_context->p_gb_context) && (PNULL != g_rrc_stats.p_rrc_stats))
        {
            /* SPR 13441 Fix Start */
            if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                    (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
            {
            /* Update the performance Counter E-RAB Initial Attempts*/
            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_erab_related_stats.erab_setup_stats.
                    erab_estabInitAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_erab_related_stats.erab_setup_stats.
                    erab_estabInitAttNbr_sum, RRC_UPDATED_QCI_INDEX(qci_value), 
                    RRC_MAX_QCI_INDEX);
            }
            /* SPR 13441 Fix End */
        }

       /*SPR_18582_FIX_START*/
          /* code removed */ 
       /*SPR_18582_FIX_END*/
        if ((( temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] == 1 ) && 
                (MAX_ERAB_COUNT > index)) && (PNULL != p_ue_context->p_gb_context))
        {/* lipa fix start */
                /* If LGW is not configured and correlation ID is received from 
                 *  MME in InitialContextSetup Request message, Send
                 *  InitialContextsetup failure from L3 */

            if(p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->m.iE_ExtensionsPresent 
                        &&
               RRC_FALSE == p_ue_context->p_gb_context->lgw_configured_status)
            {
       /*SPR_18582_FIX_START*/
                   /* temp_erab_list_status[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] will be set to
                              1 in case erabId already populated in failed list, so below mentioned check is ensuring that
                                 duplicate entries are not populated in failed list */
                if (temp_erab_list_status[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] == 0)
                {
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    cause.t = T_s1ap_Cause_misc ;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    cause.u.radioNetwork = s1ap_om_intervention;
                fail_list_count++;

                /* Copying Data for the ICS Failure Stats */
                p_ue_context->erab_ics_data.count++;
                p_ue_context->erab_ics_data.cause[index] = 
                    p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause;
                index++;

                RRC_UECC_ICS_FSM_TRACE(
                        RRC_ERROR,
                        "E_RAB ID %u = Failed :Correlation ID received from MME when LGW is not configured",
                        p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID);
                temp_erab_list_status[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] = 1;
                }
                else
                {
                    RRC_UECC_ICS_FSM_TRACE(RRC_INFO,
                      "Duplicate occurance, E_RAB ID [%u] already present in failed list",
                       p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID);
                    continue;
                }
        /*SPR_18582_FIX_END*/
            }
            else 
            {
                /* Copying Data for the ICS Stats */
                p_ue_context->erab_ics_data.count++;
                p_ue_context->erab_ics_data.cause[index].t = T_s1ap_Cause_misc;
                p_ue_context->erab_ics_data.cause[index].u.misc = s1ap_unspecified; 
                index++;

                p_e_rab_list[*e_rab_list_count] = p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1;            
                /* ERAB List count incremented and will always less than equal to 16 */
                (*e_rab_list_count)++;
            }
        }
        else if (( temp_erab_list[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] > 1 ) &&
                   (MAX_ERAB_COUNT > index))
        {
	        /* SPR 13614 Fix Start */
    	    p_ue_context->erab_ics_fail_data.cause[p_ue_context->erab_ics_fail_data.count].t =
		         T_s1ap_Cause_radioNetwork;
    	    p_ue_context->erab_ics_fail_data.cause[p_ue_context->erab_ics_fail_data.count].
	        	u.radioNetwork = s1ap_multiple_E_RAB_ID_instances;	    
    	    p_ue_context->erab_ics_fail_data.count++;
            /* SPR 13614 Fix Stop */
            
            /*SPR_18582_FIX_START*/
            if ((temp_erab_list_status[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] ==
                        RRC_NULL) && (index <MAX_ERAB_COUNT) &&
                    (fail_list_count < MAX_ERAB_COUNT))
                /*SPR_18582_FIX_END*/
            {
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    cause.t =  T_s1ap_Cause_radioNetwork;
                p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    cause.u.radioNetwork = s1ap_multiple_E_RAB_ID_instances;
	            /* SPR 13614 Fix Start */
	            /* SPR 13614 Fix Stop */
                /*SPR 21007  start*/
                for(counter = 0; counter < MAX_ERAB_COUNT ; counter++)
                {
                   if(p_e_rab_list[counter]->e_RAB_ID == p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                    e_RAB_ID)
                   {
                       for ( count = counter; count < (*e_rab_list_count); count++)
                       {
                           if (count == ((*e_rab_list_count) - 1))
                           {
                               p_e_rab_list[count] = 0;
                           }
                           else
                           {
                               p_e_rab_list[count] = p_e_rab_list[count + 1];
                           }
                       }   
                       
                       (*e_rab_list_count)--;
                       break;
                   }

                }
                /*SPR 21007 stop*/
                /* Copying Data for the ICS Failure Stats */
                p_ue_context->erab_ics_data.count++;
                p_ue_context->erab_ics_data.cause[index] = 
                    p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause;
                index++;
    	        /* SPR 13614 Fix Start */
                fail_list_count++;
	            /* SPR 13614 Fix Stop */

                RRC_UECC_ICS_FSM_TRACE(
                    RRC_INFO,
                    "E_RAB ID Failed : Mutilpe Instances in ICS Request  : %u",
                    p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID);
                temp_erab_list_status[p_erab_elem->value.u._E_RABToBeSetupItemCtxtSUReqIEs_1->e_RAB_ID] = 1;
            }
    /*SPR_18582_FIX_START*/
            else if (fail_list_count == MAX_ERAB_COUNT)
            {
                RRC_UECC_ICS_FSM_TRACE(
                    RRC_ERROR,
                    "E_RAB ID Failed list count reached to max ");
                break;
            }
    /*SPR_18582_FIX_END*/
        }
    }
    p_failed_erab_data->count = fail_list_count;
    
    /* if all erab are failing with same cause then cause will be send with
      appropriate cause otherwise cause with unspecified will be send */
    if(p_e_rab_to_be_setup_list->count == p_failed_erab_data->count)
    {
        for(erab_counter=1;erab_counter < fail_list_count ; erab_counter++)
        {
            /* Fail cause can be only radionetwork type. */
            if((p_failed_erab_data->erab_failed_to_setup_item[0].cause.t!=
            p_failed_erab_data->erab_failed_to_setup_item[erab_counter].cause.t)
            ||(p_failed_erab_data->erab_failed_to_setup_item[0].cause.u.radioNetwork!=
            p_failed_erab_data->erab_failed_to_setup_item[erab_counter].cause.u.radioNetwork))
            {
                is_same_failure_cause_recv = RRC_FALSE;
                break;
            }
        }
        if(is_same_failure_cause_recv == RRC_TRUE)
        {

            p_ue_context->share_data.p_uecc_rm_data->cause=
                p_failed_erab_data->erab_failed_to_setup_item[0].cause;
        }

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
     
}

/****************************************************************************
 * Function Name  : uecc_fsm_icsp_allocate_erab_context
 * Inputs         : s1ap_E_RABToBeSetupItemCtxtSUReq - Array of ERAB Items
 *                  U8 e_rab_list_count - No of ERABS 
 *                  uecc_ue_context_t * - pointer to UE Context
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function allocates ERAB Ctx in UE Context  
                    
 ****************************************************************************/
rrc_return_t  uecc_fsm_icsp_allocate_erab_context(
    s1ap_E_RABToBeSetupItemCtxtSUReq *p_e_rab_list[],   /* Pointer to ERAB List */  
    U8                                e_rab_list_count, /* No. of ERABS */
    uecc_ue_context_t                 *p_ue_context     /* UECC UE context */
)    
{
    U8 erab_counter = 0;
    U8 e_rab_id_index = 0;
    U8 response = RRC_SUCCESS;
    OSRTDListNode *p_node = PNULL;
    s1ap_E_RABToBeSetupItemCtxtSUReq_iE_Extensions
                    *p_iE_Extensions_list = PNULL;

    uecc_rm_erab_setup_info_t*         p_erab_setup_list = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_info_data;
    /*valgrind_fix_start*/
    /*Bug_12446_fix_start*/
    /*U32 size = RRC_NULL;*/
    /*Bug_12446_fix_stop*/
    /*valgrind_fix_stop*/


    RRC_ASSERT(PNULL != p_e_rab_list);
    RRC_ASSERT(PNULL != p_ue_context);

    /* Allocate Memory for successful E_RAB's */
    for ( erab_counter = 0; erab_counter < e_rab_list_count; erab_counter++ )
    {
   
        /* Will Store At Index e_RAB_ID */
        e_rab_id_index = (U8)p_e_rab_list[erab_counter]->e_RAB_ID;

        /* Dynamic Memory Allocation For ERAB */
        p_ue_context->p_e_rab_list[e_rab_id_index] = (uecc_drb_ctx_t *)
                               rrc_mem_get(sizeof(uecc_drb_ctx_t));

        /* ASSERT Memory Allocation */
        RRC_ASSERT(PNULL != p_ue_context->p_e_rab_list[e_rab_id_index]);
        if (PNULL == p_ue_context->p_e_rab_list[e_rab_id_index])
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name,
                    RRC_FATAL,"Memory Allocation Failure!!");
            response = RRC_FAILURE;
            break;
        }


        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
                    
        /* Fill Info in Share Data */
        /* Put E_RAB ID in Temp E_RAB Setup List */ 
        p_erab_setup_list->erab_info[erab_counter].erab_id =  
            e_rab_id_index;
        /* Set status to RRC_SUCCESS in Temp E_RAB Setup List */
        p_erab_setup_list->erab_info[erab_counter].status = RRC_NO_ERROR;

        /* Initialize E_RAB List in UE Context with parameters received from MME */
        p_ue_context->p_e_rab_list[e_rab_id_index]->e_RAB_ID =
            e_rab_id_index;
        /* SPR 20161 Fix Ftart */
        asn1Copy_s1ap_E_RABLevelQoSParameters(&p_ue_context->s1ap_asn1_ctx,
            &p_e_rab_list[erab_counter]->e_RABlevelQoSParameters,
            &p_ue_context->p_e_rab_list[e_rab_id_index]->e_RABlevelQoSParameters);
        /* SPR 20816 Fix Start */
        if (RRC_SUCCESS != rrc_copy_transportlayer_address(&p_ue_context->
                p_e_rab_list[e_rab_id_index]->transportLayerAddress,
                &p_e_rab_list[erab_counter]->transportLayerAddress,
                p_ue_context))
        {
            response = RRC_FAILURE;
            break;
        }
        /* SPR 20816 Fix End */
        /* SPR 20161 Fix End */
        /*Bug_11999_fix_start*/
        if (!(RRC_IS_GBR_BEARER(p_ue_context->p_e_rab_list[e_rab_id_index]->
                    e_RABlevelQoSParameters.qCI)))
        {
            if (RRC_TRUE == p_ue_context->p_e_rab_list[e_rab_id_index]->
                    e_RABlevelQoSParameters.m.gbrQosInformationPresent)
            {
                p_ue_context->p_e_rab_list[e_rab_id_index]->
                    e_RABlevelQoSParameters.m.gbrQosInformationPresent =
                    RRC_FALSE;
            }

        }
        /*Bug_11999_fix_stop*/

        p_ue_context->p_e_rab_list[e_rab_id_index]->gTP_TEID =
            p_e_rab_list[erab_counter]->gTP_TEID;

        /* Copy NAS Pdu */
        if ( p_e_rab_list[erab_counter]->m.nAS_PDUPresent )
        {

            asn1Copy_s1ap_NAS_PDU(&p_ue_context->s1ap_asn1_ctx,
                &p_e_rab_list[erab_counter]->nAS_PDU,
                &p_ue_context->p_e_rab_list[e_rab_id_index]->nAS_PDU);

            p_ue_context->p_e_rab_list[e_rab_id_index]->m.nAS_PDUPresent = 1;
        }
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
                    s1ap_E_RABToBeSetupItemCtxtSUReq_iE_Extensions_element *p_elem =
                        (s1ap_E_RABToBeSetupItemCtxtSUReq_iE_Extensions_element*)p_node->data;

                    RRC_ASSERT(PNULL!=p_elem);

                    if ( p_elem->extensionValue.t ==  T47__E_RABToBeSetupItemCtxtSUReqExtIEs_1 
                            &&
                            p_elem->extensionValue.u._E_RABToBeSetupItemCtxtSUReqExtIEs_1 != PNULL )

                    {
                        p_ue_context->p_e_rab_list[e_rab_id_index]->correlation_id = 
                            (s1ap_Correlation_ID*)rrc_mem_get(sizeof(s1ap_Correlation_ID));

                        asn1Copy_s1ap_Correlation_ID(&p_ue_context->s1ap_asn1_ctx,
                                p_elem->extensionValue.u._E_RABToBeSetupItemCtxtSUReqExtIEs_1,
                                p_ue_context->p_e_rab_list[e_rab_id_index]->correlation_id);

                        p_ue_context->p_e_rab_list[e_rab_id_index]->m.Lipa_RabPresent = RRC_TRUE;
                    }
                }
            }
            else 
            {
            }
        }/* lipa end */
        RRC_UECC_ICS_FSM_TRACE(
                RRC_INFO,
            "E_RAB Context Successfully Allocated for E_RAB ID : %u ",
            e_rab_id_index);
    }

/*Qos changes START*/        
    if ((response == RRC_SUCCESS) 

            /*&&(is_dflt_bearer_est == RRC_TRUE)*/)
    {
        /* Set success count in Temp E_RAB Setup List */
        p_erab_setup_list->count = e_rab_list_count;

        /* Set the successul erab list count in UE Context */
        p_ue_context->e_rab_list_count = e_rab_list_count;
    }

/*Qos changes END*/        

    return response;

}

/****************************************************************************
 * Function Name  : uecc_fsm_icsp_count_erab_list
 * Inputs         : uecc_ue_context_t * - Pointer to UE Context
 * Outputs        : None
 * Returns        : erab_list_count
 * Description    : This function counts the no. of ERABs present in temporary  
 *                  ERAB Setup List
 ****************************************************************************/
rrc_return_t  uecc_fsm_icsp_count_erab_list(
    uecc_ue_context_t      *p_ue_context  /* UECC UE context */
)    
{
    U8 erab_list_count = 0;
    U8 erab_list_counter = 0;

    RRC_ASSERT(PNULL != p_ue_context);

    /* Count no. of Successful E_RAB's in Temp E_RAB Setup List */
    if(PNULL != p_ue_context->p_curr_proc_data)
    {
        for (erab_list_counter = 0;
                erab_list_counter < p_ue_context->p_curr_proc_data->u.
                erab_setup_request_data.erab_info_data.count;
                erab_list_counter++)
        {
            /* If status is RRC_SUCCESS then it is Successful E_RAB */
            if (RRC_NO_ERROR == p_ue_context->p_curr_proc_data->u.
                    erab_setup_request_data.erab_info_data.erab_info[erab_list_counter]
                    .status)
            {
                erab_list_count++;
            }
        }
    }
    return erab_list_count;
}
/*Pranab */
/****************************************************************************
 * Function Name  : uecc_fsm_ics_rlf_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                   UECC_EV_LLIM_RADIO_LINK_FAILURE_IND
 ************************************************************************/
 void uecc_fsm_ics_rlf_ind_handler(
 uecc_ue_context_t*  p_ue_context,
 void*               p_api
 )

{
    /* SPR 13231 Fix Start */
    U8 counter = 0;
    /* SPR 13231 Fix Stop */

     RRC_ASSERT(PNULL != p_ue_context);
     RRC_ASSERT(PNULL != p_api);
     RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
 
    RRC_UECC_ICS_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in fsm :%p"
            ,p_api);/*SPR 17777 +-*/

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    if (strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                (const S8 *)("UECC_RRM_RESOURCE_ADMISSION_ONGOING")))
    {
        /*If Waiting for SgNBAdditionRequestAck, initiate SgNBReleaseReq without SgNB-X2AP-ID*/
        if (!strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                    (const S8 *)("UECC_X2AP_SGNB_ADDITION_ONGOING")))
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_DC_PREP_TIMER,
                    &p_ue_context->timers_data);

            /* Put all E_RAB's in failed list from ERAB Setup List */
            uecc_fsm_rm_fail_action_mark_all_erab_failed(
                    p_ue_context,
                    RRC_RADIO_LINK_FAILURE_TRIGGERED);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

                /* Send SgNB RELEASE to SgNB */ 
                if (RRC_SUCCESS != 
                        build_and_send_sgnb_release_request(p_ue_context))
                {
                    RRC_UECC_ICS_FSM_TRACE(RRC_ERROR, 
                            "[%s],Unable to process SgNB Release Request", __FUNCTION__);
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
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->timers_data);

            /* Put all E_RAB's in failed list from ERAB Setup List */
            uecc_fsm_rm_fail_action_mark_all_erab_failed(
                    p_ue_context,
                    RRC_RADIO_LINK_FAILURE_TRIGGERED);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

                /* Send SgNBReconfigurationComplete with failure */ 
                if (RRC_SUCCESS != 
                        uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                            p_ue_context
                            ))
                {
                    RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
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

    /* SPR 13231 Fix Start */
    p_ue_context->share_data.p_uecc_rm_data->cause.t =
            T_s1ap_Cause_radioNetwork;
    p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
            s1ap_radio_connection_with_ue_lost;

	for(counter = 0; counter < p_ue_context->erab_ics_data.count; counter++)
	{
       p_ue_context->erab_ics_data.cause[counter].t = 
                                     T_s1ap_Cause_radioNetwork;
       p_ue_context->erab_ics_data.cause[counter].u.radioNetwork =
                                     s1ap_radio_connection_with_ue_lost; 
    }
    /* SPR 13231 Fix Stop */

     uecc_fsm_finish_activity_failure( 
     p_ue_context,
     (void*)UECC_EV_ICS_RADIO_LINK_FAILURE_INT );

     RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
 }
/****************************************************************************
 * Function Name  : uecc_fsm_ics_re_establish_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                   UECC_EV_RRC_RE_ESTABLISH_REQ
 ************************************************************************/
void uecc_fsm_ics_re_establish_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    /* SPR 13231 Fix Start */
    U8 counter = 0;
    /* SPR 13231 Fix Stop */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ICS_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    if (strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                (const S8 *)("UECC_RRM_RESOURCE_ADMISSION_ONGOING")))
    {
        /*If Waiting for SgNBAdditionRequestAck, initiate SgNBReleaseReq without SgNB-X2AP-ID*/
        if (!strcmp_wrapper(p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name,
                    (const S8 *)("UECC_X2AP_SGNB_ADDITION_ONGOING")))
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_DC_PREP_TIMER,
                    &p_ue_context->timers_data);

             /* Put all E_RAB's in failed list from ERAB Setup List */
             uecc_fsm_rm_fail_action_mark_all_erab_failed(
                     p_ue_context,
                     RRC_REESTABLISHMENT_TRIGGERED);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

                /* Send SgNB RELEASE to SgNB */ 
                if (RRC_SUCCESS != 
                        build_and_send_sgnb_release_request(p_ue_context))
                {
                    RRC_UECC_ICS_FSM_TRACE(RRC_ERROR, 
                            "[%s],Unable to process SgNB Release Request", __FUNCTION__);
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
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->timers_data);

             /* Put all E_RAB's in failed list from ERAB Setup List */
             uecc_fsm_rm_fail_action_mark_all_erab_failed(
                     p_ue_context,
                     RRC_REESTABLISHMENT_TRIGGERED);

            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

                /* Send SgNBReconfigurationComplete with failure */ 
                if (RRC_SUCCESS != 
                        uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                            p_ue_context
                            ))
                {
                    RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
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


    if (RRC_SUCCESS !=
           uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
           p_ue_context->p_gb_context, p_ue_context->crnti,
           p_ue_context->reestablishmentCause,
           p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_ICS_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }

    p_ue_context->share_data.p_uecc_rm_data->cause.t =
        T_s1ap_Cause_radioNetwork;
    /* SPR 13231 Fix Start */
    p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
        s1ap_interaction_with_other_procedure;

	for(counter = 0; counter < p_ue_context->erab_ics_data.count; counter++)
	{
       p_ue_context->erab_ics_data.cause[counter].t = 
                                     T_s1ap_Cause_radioNetwork;
       p_ue_context->erab_ics_data.cause[counter].u.radioNetwork =
                                     s1ap_interaction_with_other_procedure; 
    }
    /* SPR 13231 Fix Stop */

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_ICS_REESTABLISH_REQ_INT);

     RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
