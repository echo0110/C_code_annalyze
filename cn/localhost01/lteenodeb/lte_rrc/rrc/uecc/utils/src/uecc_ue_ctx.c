/******************************************************************************
*
*   FILE NAME:
*       uecc_ue_ctx.c
*
*   DESCRIPTION:
*       This is the UECC UE context data implementation file.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   20 Jul 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "uecc_ue_ctx.h"
#include "uecc_x2ap.h"
#include "rrc_common_utils.h"
#include "rrc_common_md.h"
#include "uecc_logger.h"
#include "uecc_statistic.h"
#include "rrc_msg_mgmt.h" 
#include "uecc_global_ctx.h"
#include "uecc_common.h"
#include "uecc_fsm_engine.h"
/* CR 90 changes start*/
#include "rrc_shared_stats.h"
/* CR 90 changes stop*/
#include <lteMisc.h>
/******************************************************************************
 * Private variable definitions
 *****************************************************************************/

/*BUG 604 changes start*/
/*Macro not needed*/
/*#define UECC_UE_CTX_GET_UE_INDEX_FROM_S1AP_ID(s1ap_ue_id) \
    ((s1ap_ue_id) & 0xFFFF)*/
     /*BUG 604 changes stop*/

#define UECC_UE_CTX_GET_UE_INDEX_FROM_X2AP_ID(old_enb_ue_x2ap_id) \
    ((old_enb_ue_x2ap_id) & 0xFFFF)

/******************************************************************************
 * Local Functions declarations
 *****************************************************************************/
uecc_meas_object_to_add_mod_t* uecc_queue_pop_node_from_meas_object_list(
        uecc_meas_object_to_add_mod_list_t * p_list 
);
uecc_report_config_to_add_mod_t* uecc_queue_pop_node_from_report_config_list(
        uecc_report_config_to_add_mod_list_t * p_list 
);
UE_Radio_Capability_RAT_Container_t* uecc_queue_pop_node_from_ue_capability_list(
        UE_Radio_Capability_RAT_Container_list_t * p_list 
);
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
OSUINT32 s1apToX2apMapRadioNetwork[MAX_RADIO_NETWORK_CAUSE] = 
{
    x2ap_unspecified_2,
    x2ap_tx2relocoverall_expiry,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_partial_handover,
    x2ap_unspecified_2,
    x2ap_ho_target_not_allowed,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_cell_not_available,
    x2ap_unspecified_2,
    x2ap_no_radio_resources_available_in_target_cell,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_handover_desirable_for_radio_reasons,
    x2ap_time_critical_handover,
    x2ap_resource_optimisation_handover,
    x2ap_reduce_load_in_serving_cell,
    x2ap_user_inactivity,
    x2ap_radio_connection_with_UE_lost,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_invalid_QoS_combination,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_multiple_E_RAB_ID_instances,
    x2ap_encryption_and_or_integrity_protection_algorithms_not_supported,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_unspecified_2,
    x2ap_not_supported_QCI_value       
};
OSUINT32 s1apToX2apMapMisc[MAX_MISC_CAUSE] = 
{
    x2ap_control_processing_overload,
    x2ap_not_enough_user_plane_processing_resources,
    x2ap_hardware_failure,
    x2ap_om_intervention,
    x2ap_unspecified,
    x2ap_unspecified
}
/*NR_DC Code Change Stop*/;
#endif
/******************************************************************************
 * Function implementation
 *****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_db_init
*
*   INPUT:	   uecc_gb_context_t*  p_uecc_gb_context
*   
*   OUTPUT:        None
*
*   RETURNS:       RRC_SUCCESS or RRC_FAILURE
*
*   DESCRIPTION:   This function initializes UE contexts DB.
*
******************************************************************************/
rrc_return_t uecc_ue_ctx_db_init(
   uecc_gb_context_t*  p_uecc_gb_context  /* UECC global context */
)
{
    rrc_return_t result = RRC_FAILURE;
    uecc_ue_context_t** p_contexts_db = PNULL; /* Pointer on UE contexts DB */
    rrc_ue_index_t* p_not_allocated_ue_ids = PNULL; /*UE IDs ready to allocate*/
    rrc_ue_index_t ue_index = RRC_NULL;

    p_contexts_db = (uecc_ue_context_t**)
        rrc_mem_get(sizeof(uecc_ue_context_t*)*(p_uecc_gb_context->total_ue_supported));
    if (PNULL == p_contexts_db)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->facility_name,
                RRC_BRIEF,"[UE_DB] Initialization failed (contexts_db)");
    }
    else
    {
        p_not_allocated_ue_ids = (rrc_ue_index_t*)
            rrc_mem_get(sizeof(rrc_ue_index_t)*(p_uecc_gb_context->total_ue_supported));
        if (PNULL == p_not_allocated_ue_ids)
        {
            rrc_mem_free(p_contexts_db);
/* Coverity ID 81581 Fix Start*/
           /* p_contexts_db = PNULL; */
/* Coverity ID 81581 Fix End*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->facility_name,
                    RRC_BRIEF,"[UE_DB] Initialization failed (not_allocated_ue_ids)");
        }
        else
        {
            memset_wrapper( p_contexts_db, 0,
                sizeof(uecc_ue_context_t*)*(p_uecc_gb_context->total_ue_supported) );

           p_uecc_gb_context->ue_db.ue_contexts = p_contexts_db;

            for (ue_index=0; ue_index<(p_uecc_gb_context->
                        total_ue_supported); ue_index++)
            {
                p_not_allocated_ue_ids[ue_index] = ue_index;
            }
            p_uecc_gb_context->ue_db.not_allocated_ue_ids_count =
                p_uecc_gb_context->total_ue_supported;

            p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur = 0;
            p_uecc_gb_context->ue_db.not_allocated_ue_ids =
                p_not_allocated_ue_ids;

            list_init(&p_uecc_gb_context->ue_context_info_list);

            RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->facility_name,
                    RRC_BRIEF, "[UE_DB] Initialized");

            result=RRC_SUCCESS;
        }
    }

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_db_free
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*   
*   OUTPUT:        none
*
*   RETURNS:       none.
*
*   DESCRIPTION:   This function free all UE contexts.
*
******************************************************************************/
void uecc_ue_ctx_db_free(
    uecc_gb_context_t*  p_uecc_gb_context  /* UECC global context */
)
{
    U16 ctx_index = RRC_NULL;
    U16 ue_index = RRC_NULL;

    if ( PNULL == p_uecc_gb_context->ue_db.ue_contexts )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->facility_name,
                RRC_BRIEF, "[UE_DB] Not initialized");
        return;
    }

    for (ctx_index = 0;
         ctx_index < (p_uecc_gb_context->total_ue_supported);
         ctx_index++ )
    {
        if ( PNULL != p_uecc_gb_context->ue_db.ue_contexts[ctx_index])
        {  
            ue_index = p_uecc_gb_context->ue_db.ue_contexts[ctx_index]->
                ue_index;

            uecc_ue_ctx_free(p_uecc_gb_context, ue_index);
        }
    }

    rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts);
    rrc_mem_free(p_uecc_gb_context->ue_db.not_allocated_ue_ids);

    p_uecc_gb_context->ue_db.ue_contexts = PNULL;
    p_uecc_gb_context->ue_db.not_allocated_ue_ids = PNULL;
    p_uecc_gb_context->ue_db.not_allocated_ue_ids_count = 0;


    p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur = 0;

    RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->facility_name,
            RRC_BRIEF, "[UE_DB] Cleaned up");
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_allocate
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  rrc_module_id_t     uecc_module_id    
*   
*   OUTPUT:        none
*
*   RETURNS:       UE index or MAX_NUM_SUPPORTED_UE_PER_UECC in case of error.
*
*   DESCRIPTION:   This function allocates and initializes UE context.
*
******************************************************************************/
rrc_ue_index_t uecc_ue_ctx_allocate(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_module_id_t     uecc_module_id     /* Module Id for UECC Instance */
)
{
    uecc_ue_context_t*  p_ue_ctx = PNULL;
    U16                 ue_id = RRC_NULL;
    /* SPR:7018 start */
    rrc_ue_index_t      ue_index = p_uecc_gb_context->total_ue_supported;
    /* SPR:7018 stop */
    

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts );


    if (p_uecc_gb_context->ue_db.not_allocated_ue_ids_count > 0)
    {
        /*Get free UE context*/
        ue_id = p_uecc_gb_context->ue_db.not_allocated_ue_ids[
            p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur];

        RRC_ASSERT(PNULL == p_uecc_gb_context->ue_db.ue_contexts[ue_id]);

        /*allocate context */
        p_ue_ctx=(uecc_ue_context_t*)rrc_mem_get(sizeof(uecc_ue_context_t));
        if (PNULL == p_ue_ctx)
        {
            return MAX_NUM_SUPPORTED_UE_PER_UECC;
        }

        p_uecc_gb_context->ue_db.not_allocated_ue_ids_count--;
        p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur++;
        if (p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur >=
            p_uecc_gb_context->total_ue_supported)
        {
            p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur = 0;
        }

        memset_wrapper( p_ue_ctx, 0, sizeof(uecc_ue_context_t) );

        p_uecc_gb_context->ue_db.ue_contexts[ue_id] = p_ue_ctx;
 
        ue_index = (U16)(uecc_module_id - RRC_UECC_1_MODULE_ID);
        ue_index = (U16)((ue_index << 0x0D) | ue_id);
  
        /* SPR 19273 Fix Start */
        SET_UE_INDEX(ue_index);
        /* SPR 19273 Fix End */

        RRC_UECC_TRACE(p_uecc_gb_context->context_index,p_uecc_gb_context->facility_name,
                RRC_BRIEF,"[UE_INDEX:%u] context allocated", ue_index);
    }
    return ue_index;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_init
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  rrc_ue_index_t      ue_index
*                  rrc_rnti_t          rnti
*                  rrc_cell_index_t    cell_index
*   
*   OUTPUT:        None
*
*   RETURNS:       None
*
*   DESCRIPTION:   This function initializes UE context.
*
******************************************************************************/
void uecc_ue_ctx_init(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_ue_index_t      ue_index,           /* UE index */
    rrc_rnti_t          rnti,               /* UE rnti */
    rrc_cell_index_t    cell_index          /* Cell Index */
)
{
    uecc_ue_context_t*  p_ue_ctx = PNULL;
    list_t                          *p_ue_reconfig_queue;
    RRC_ASSERT(PNULL != p_uecc_gb_context);

    p_ue_ctx = uecc_ue_ctx_get(p_uecc_gb_context, ue_index);

    /* p_ue_ctx should not be PNULL here */
    if(PNULL == p_ue_ctx)
    {
        return;
    }
    /* BUG_11901 start */
    /* BUG 12722 Fix Start */
    /* BUG 12722 Fix Stop */
    /* BUG_11901 stop */
    /* SPR 20818 Fix Start */
    p_ue_ctx->p_registered_lai = PNULL ;
    /* SPR 20818 Fix End */
    p_ue_ctx->nh_ncc_pair.used_flag=RRC_TRUE;
    p_ue_ctx->rrm_ue_capability_enq_ind = RRC_FALSE;
    p_ue_ctx->is_meas_transaction_id_valid = RRC_FALSE;
    p_ue_ctx->crnti = rnti;
    p_ue_ctx->ue_index = ue_index;
    /* TS 3GPP TS 36.413 V8.5.1: 8.6.2.1 Initial UE Message */
    p_ue_ctx->s1ap_ue_id = ue_index;
    p_ue_ctx->cell_index = cell_index;

    p_ue_ctx->wait_time =
        UECC_LTE_RRC_CONNECTION_REJECT_WAIT_TIME_FIELD_DEFAULT_VALUE;

    /* 0 will denote that extended wait time will not be sent */ 
    p_ue_ctx->extended_wait_time = 0;

    /* store pointers to global context ue context */
    p_ue_ctx->p_gb_context = p_uecc_gb_context;

    /*Initialize X2ap id's */
    p_ue_ctx->x2ap_context.old_enb_ue_x2ap_id = INVALID_X2AP_UE_ID;
    p_ue_ctx->x2ap_context.new_enb_ue_x2ap_id = INVALID_X2AP_UE_ID;
    p_ue_ctx->x2ap_context.m.new_enb_ue_x2ap_id_present = 0;
    /* SPR 16750 Fix Start */
    p_ue_ctx->x2ap_context.m.error_indication_ongoing = 0;
    /* SPR 16750 Fix Stop */

    /* SPR 17391 Fix Start */
    p_ue_ctx->m.rrc_connection_count_updated = RRC_FALSE;
    /* SPR 17391 Fix Stop */

    /*Initialize CDMA2000 info*/
    p_ue_ctx->cdma2000_info.w_f_1xrtt_msg = RRC_TRUE;
    p_ue_ctx->cdma2000_info.w_f_hrpd_msg = RRC_TRUE;

    /* Bug 817 Fix Start */
    p_ue_ctx->rrc_trg_ho_fwd_tnl_info.
        drb_tunnel_info_count = RRC_NULL;
    /* Bug 817 Fix Stop */


    /* Init timer_id */
    uecc_ue_timers_data_init(p_uecc_gb_context, &p_ue_ctx->timers_data);

    /*SPR_16416_START*/
    uecc_event_queue_init(&(p_ue_ctx->p_ul_nas_transport_queue));
    /*SPR_16416_END*/

    /*SPR_17976_START*/
    uecc_event_queue_init(&(p_ue_ctx->p_tenb_cre_event_queue));
    /*SPR_17976_END*/

    /*spr_22003_changes_start*/
    /*By default value for this counter flag has been set as TRUE as it is not
     * to be incremented in case UE is getting released at target due to HO
     * cancel. More conditions may be added later. Scenarios where this counter
     * needs to be incremented is defined in 3GPP 32.425
     */
    p_ue_ctx->is_erab_relSuccNbr_qci_counter_pegging_reqd = RRC_TRUE;
    /*spr_22003_changes_end*/

    /* Init S1AP ASN context */
    if (RT_OK != rtInitContext(&p_ue_ctx->s1ap_asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_ERROR, "[UE_INDEX:%u] ASN context initialization failed", 
                ue_index);
    }

    uecc_status_ue_init(p_ue_ctx);
    uecc_fsm_init_engine(p_ue_ctx);

    p_ue_reconfig_queue = &p_ue_ctx->ue_reconfig_queue;

    RECONFIGURATION_Q_INIT(p_ue_reconfig_queue);

    /* Free S1AP ASN context */
    rtFreeContext(&p_ue_ctx->s1ap_asn1_ctx);
    #ifdef ENDC_ENABLED
        /* Setting MeNB-X2AP-ID and SgNB-X2AP-ID to INVALID_UE_INDEX */
        p_ue_ctx->x2ap_context.menb_ue_x2ap_id = RRC_INVALID_UE_INDEX;
        p_ue_ctx->x2ap_context.sgnb_ue_x2ap_id = RRC_INVALID_UE_INDEX;
        p_ue_ctx->partial_sgnb_rab_release = RRC_FALSE;
        p_ue_ctx->rel_proc_fail = RRC_FALSE;
        p_ue_ctx->x2apUecontextReleased = RRC_FALSE;
        p_ue_ctx->sgnbInitiatedSgnbReleaseFlag = RRC_FALSE;
        p_ue_ctx->release_type = INVALID_REL;
        p_ue_ctx->nr_capab_req_flag = 0;
        p_ue_ctx->msg_type = 0xFF;
        memset_wrapper( &(p_ue_ctx->endcConfigInfo), 0, sizeof(RrcEndcConfigInfo_t));
        memset_wrapper( &(p_ue_ctx->sgnb_cause), 0, sizeof(x2ap_Cause));
    #endif
}

/* TRACE_fix */
/*SPR 15896 Fix Start*/
/******************************************************************************
*   FUNCTION NAME: stop_trace_and_send_file_transfer_start_ind
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  U16 ue_id
*   OUTPUT:        none
*   RETURNS:       none.
*   DESCRIPTION:   This function closes the trace file and sends trace file
*                  transfer start ind message to OAM.
******************************************************************************/
void stop_trace_and_send_file_transfer_start_ind(
        uecc_gb_context_t*  p_uecc_gb_context,
        U16 ue_id)
{
	U8                  *p_msg = PNULL;
	rrc_uecc_oamh_trace_file_transfer_start_ind_t *p_out = PNULL;
	uecc_ue_context_t   *p_ue_context = PNULL;
	/*SPR 15875 Fix Start*/
	trace_id_info_t *p_trace_info = PNULL ;
	U8 mcc[4] = {RRC_NULL};
	U8 mnc[4] = {RRC_NULL};
	U8 trace_id[7] = {RRC_NULL};
	/*SPR 15875 Fix Stop*/	


	if ( p_uecc_gb_context->ue_db.ue_contexts[ue_id]->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE)
	{
		if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_cell_trace_activation_info) && 
				(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_cell_trace_activation_info->fp != PNULL))
		{
			p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[ue_id];
			if ((PNULL != p_ue_context) && (PNULL != p_ue_context->p_ue_cell_trace_activation_info))
			{
				/*SPR 15875 Fix Start*/
    	    	                p_trace_info = &(p_ue_context->p_ue_cell_trace_activation_info->trace_id_info);
				/*Extracting TRACE ID*/
				//		  {
				snprintf_wrapper((Char8*)trace_id,7,(const Char8*)"%02X%02X%02X",
						p_trace_info->cell_eutran_trace_id.trace_reference[3],
						p_trace_info->cell_eutran_trace_id.trace_reference[4],
						p_trace_info->cell_eutran_trace_id.trace_reference[5]);
			/*Extracting MCC*/
			snprintf_wrapper((Char8*)mcc,4,(const Char8*)"%d%d%d",
					(p_trace_info->cell_eutran_trace_id.trace_reference[0] & 0x0F),
					((p_trace_info->cell_eutran_trace_id.trace_reference[0] & 0xF0)>>4),
					(p_trace_info->cell_eutran_trace_id.trace_reference[1] & 0x0F));
			/*Extracting MNC*/
			/*Check if the higher nibble contains filler digit*/
			/*If present , discard the filler digit('F')*/
			if(0xF0 == (p_trace_info->cell_eutran_trace_id.trace_reference[1] & 0xF0))
			{
				snprintf_wrapper((Char8*)mnc,4,(const Char8*)"%d%d",
						(p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0x0F),
						((p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0xF0)>>4));
			}
			else	
			{
				snprintf_wrapper((Char8*)mnc,4,(const Char8*)"%d%d%d",
						((p_trace_info->cell_eutran_trace_id.trace_reference[1] & 0xF0)>>4),
						(p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0x0F),
						((p_trace_info->cell_eutran_trace_id.trace_reference[2] & 0xF0)>>4));
			}
			P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n<traceSessionRef>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(),GEN_CELL,"\n<MCC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,mcc);
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"</MCC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"\n<MNC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,mnc);
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"</MNC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"\n<TRACE_ID>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,trace_id);
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"</TRACE_ID>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"\n</traceSessionRef>");

			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_CELL,"\n</traceRecSession> \n </traceCollecFile>");
			/*SPR 15875 Fix Stop*/	

			/* allocate memory to send trace_file_transfer_start_ind */
			p_msg = uecc_alloc_intrl_msg(RRC_OAMH_MODULE_ID,
					UECC_OAMH_TRACE_FILE_TRANSFER_START_IND,
					sizeof(rrc_uecc_oamh_trace_file_transfer_start_ind_t));

			if (PNULL == p_msg)
			{
				/*out of memory*/
				RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name,
						RRC_FATAL,"[OAMH] Low memory for UECC_OAMH_TRACE_FILE_TRANSFER_START_IND ");
				return;
			}
			/* Point to message */
			p_out = (rrc_uecc_oamh_trace_file_transfer_start_ind_t*)(p_msg +
					RRC_API_HEADER_SIZE);
			/* cell_index */
			p_out->cell_index = p_uecc_gb_context->ue_db.ue_contexts[ue_id]->cell_index;
			/* ue index */
			p_out->ue_index = ue_id;

			/* e_utran_trace_id: trace recording session reference */
			l3_memcpy_wrapper(&(p_out->e_utran_trace_id.trace_recording_session_reference),
					p_ue_context->p_ue_cell_trace_activation_info->trace_id_info.
					cell_eutran_trace_id.trace_recording_session_reference,
					sizeof(p_out->e_utran_trace_id.trace_recording_session_reference));

			/* length of file */
			p_out->trace_file.len_file_name =(U16)(strlen_wrapper((const Char8 *)(p_ue_context->p_ue_cell_trace_activation_info->trace_file)));

			/* trace file name */
			l3_memcpy_wrapper(p_out->trace_file.file_name,
					p_ue_context->p_ue_cell_trace_activation_info->trace_file,
					(U16)(p_out->trace_file.len_file_name + 1)); /* +1 for Null Character */

			/* Trace Collection IP Addr */
			l3_memcpy_wrapper(&(p_out->trace_collection_entity),
					&(p_ue_context->p_ue_cell_trace_activation_info->trace_collection_entity_info),
					(U16)(sizeof(p_out->trace_collection_entity)));

			/* Send Message */
			rrc_send_message(p_msg, RRC_OAMH_MODULE_ID);

			/* Deallocate memory whic was used to store trace control and
			 * configuration parameters in Ue Context */
			rrc_fclose(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_cell_trace_activation_info->fp);
			p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_cell_trace_activation_info->fp = PNULL;
			rrc_mem_free(p_ue_context->p_ue_cell_trace_activation_info);
			p_ue_context->p_ue_cell_trace_activation_info = PNULL;
			}
		}
		p_uecc_gb_context->ue_db.ue_contexts[ue_id]->m.traceActivated &= (0xFF^RRC_TRACE_ACTIVATION_CELL_INITIATE);
	}

/*Checking for presence of Subscriber initiated Trace*/

	if ( p_uecc_gb_context->ue_db.ue_contexts[ue_id]->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE)
	{
		if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_trace_activation_info) && 
				(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_trace_activation_info->fp != PNULL))
		{
	
			p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[ue_id];
			if ((PNULL != p_ue_context) && (PNULL != p_ue_context->p_ue_trace_activation_info))
			{
				p_trace_info = &(p_ue_context->p_ue_trace_activation_info->trace_id_info);
				/*Extracting TRACE ID*/
				snprintf_wrapper((Char8*)trace_id,7,(const Char8*)"%02X%02X%02X",
						p_trace_info->eutran_trace_id.data[3],
						p_trace_info->eutran_trace_id.data[4],
						p_trace_info->eutran_trace_id.data[5]);


			/*Extracting MCC*/
			snprintf_wrapper((Char8*)mcc,4,(const Char8*)"%d%d%d",
					(p_trace_info->eutran_trace_id.data[0] & 0x0F),
					((p_trace_info->eutran_trace_id.data[0] & 0xF0)>>4),
					(p_trace_info->eutran_trace_id.data[1] & 0x0F));
			/*Extracting MNC*/
			/*Check if the higher nibble contains filler digit*/
			/*If present , discard the filler digit('F')*/
			if(0xF0 == (p_trace_info->eutran_trace_id.data[1] & 0xF0))
			{
				snprintf_wrapper((Char8*)mnc,4,(const Char8*)"%d%d",
						(p_trace_info->eutran_trace_id.data[2] & 0x0F),
						((p_trace_info->eutran_trace_id.data[2] & 0xF0)>>4));
			}
			else	
			{
				snprintf_wrapper((Char8*)mnc,4,(const Char8*)"%d%d%d",
						((p_trace_info->eutran_trace_id.data[1] & 0xF0)>>4),
						(p_trace_info->eutran_trace_id.data[2] & 0x0F),
						((p_trace_info->eutran_trace_id.data[2] & 0xF0)>>4));
			}
			P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME,"\n<traceSessionRef>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(),GEN_MME,"\n<MCC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,mcc);
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"</MCC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"\n<MNC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,mnc);
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"</MNC>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"\n<TRACE_ID>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,trace_id);
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"</TRACE_ID>");
			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"\n</traceSessionRef>");

			P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), GEN_MME,"\n</traceRecSession> \n </traceCollecFile>");
	         /*SPR 15875 Fix Stop*/	

                /* allocate memory to send trace_file_transfer_start_ind */
                p_msg = uecc_alloc_intrl_msg(RRC_OAMH_MODULE_ID,
                        UECC_OAMH_TRACE_FILE_TRANSFER_START_IND,
                        sizeof(rrc_uecc_oamh_trace_file_transfer_start_ind_t));

                if (PNULL == p_msg)
                {
                    /*out of memory*/
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name,
                            RRC_FATAL,"[OAMH] Low memory for UECC_OAMH_TRACE_FILE_TRANSFER_START_IND ");
                    return;
                }
                /* Point to message */
                p_out = (rrc_uecc_oamh_trace_file_transfer_start_ind_t*)(p_msg +
                        RRC_API_HEADER_SIZE);
                /* cell_index */
                p_out->cell_index = p_uecc_gb_context->ue_db.ue_contexts[ue_id]->cell_index;
                
                /* ue index */
                p_out->ue_index = p_uecc_gb_context->ue_db.ue_contexts[ue_id]->ue_index;

                /* e_utran_trace_id: trace recording session reference */
                l3_memcpy_wrapper(&(p_out->e_utran_trace_id.trace_recording_session_reference),
                        p_ue_context->p_ue_trace_activation_info->trace_id_info.
					eutran_trace_id.data+6,
                        sizeof(p_out->e_utran_trace_id.trace_recording_session_reference));

                /* length of file */
                p_out->trace_file.len_file_name =(U16)(strlen_wrapper((const Char8 *)(p_ue_context->p_ue_trace_activation_info->trace_file)));

                /* trace file name */
                l3_memcpy_wrapper(p_out->trace_file.file_name,
                        p_ue_context->p_ue_trace_activation_info->trace_file,
                        (U16)(p_out->trace_file.len_file_name + 1)); /* +1 for Null Character */

                /* Trace Collection IP Addr */
                l3_memcpy_wrapper(&(p_out->trace_collection_entity),
                        &(p_ue_context->p_ue_trace_activation_info->trace_collection_entity_info),
                        (U16)(sizeof(p_out->trace_collection_entity)));

                /* Send Message */
                rrc_send_message(p_msg, RRC_OAMH_MODULE_ID);

                /* Deallocate memory whic was used to store trace control and
                 * configuration parameters in Ue Context */
			rrc_fclose(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_trace_activation_info->fp);
			p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ue_trace_activation_info->fp = PNULL;
                rrc_mem_free(p_ue_context->p_ue_trace_activation_info);
                p_ue_context->p_ue_trace_activation_info = PNULL;
            }
        }
		p_uecc_gb_context->ue_db.ue_contexts[ue_id]->m.traceActivated &= (0xFF^RRC_TRACE_ACTIVATION_MME_INITIATE);
    }
}
/*SPR 15896 Fix Stop*/
/* TRACE_fix */

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_free
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  rrc_ue_index_t ue_index
*   
*   OUTPUT:        none
*
*   RETURNS:       none.
*
*   DESCRIPTION:   This function frees UE context.
*
******************************************************************************/
void uecc_ue_ctx_free(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_ue_index_t ue_index  /* UE index  */
)
{
    U16 erb_idx = RRC_NULL;
    U16 ue_id = 0;
    /* Bug 817 Fix Start */
    U8 drb_count = 0;
    uecc_ue_context_t *p_ue_context = PNULL;
    /* Bug 817 Fix Stop */

    uecc_meas_object_to_add_mod_t    *p_meas_object_node = PNULL;
    uecc_report_config_to_add_mod_t    *p_report_config_node = PNULL;
    UE_Radio_Capability_RAT_Container_t    *p_ue_capability_node = PNULL;
    
    YLIST                           *p_ue_reconfig_queue = PNULL;
    YLNODE                          *snode = PNULL;
    
    /* CR 90 changes start*/
    U8 qci_value = 0;
    /* CR 90 changes stop*/

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts );

    ue_id = RRC_GET_UE_INDEX(ue_index);

    if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id])
    {
    	/* Bug 817 Fix Start */
    	p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[ue_id];
    	/* Bug 817 Fix Stop */

    	/* SPR 17391 Fix Start */
        if ( (PNULL != g_rrc_stats.p_rrc_stats) && (RRC_NULL < 
                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                     rrc_uecc_instance_stats[p_ue_context->p_gb_context->
                      offset_uecc_mod_id].rrc_uecc_cell_stats[p_ue_context->cell_index].
                       rrc_connection_related_stats.rrcConnectionNumber) &&
	    		(RRC_TRUE == p_ue_context->m.rrc_connection_count_updated)) 
        {
            RRC_UPDATE_PERFORMANCE_COUNTER_WITH_DECREMENT (p_ue_context->p_gb_context,
                      g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                      rrc_uecc_instance_stats[p_ue_context->
                      p_gb_context->offset_uecc_mod_id].rrc_uecc_cell_stats[p_ue_context->
                      cell_index].rrc_connection_related_stats.
                      rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);

	        p_ue_context->m.rrc_connection_count_updated = RRC_FALSE;
	        /* SPR 17391 Fix Stop */
        }
        /* SPR 11748 Fix Stop */

        /* TRACE_fix */
        stop_trace_and_send_file_transfer_start_ind(p_uecc_gb_context, ue_id);
        /* TRACE_fix */

        /* Free handover related context data */
        uecc_fsm_free_trg_ho_context(p_uecc_gb_context->
                ue_db.ue_contexts[ue_id]);

        /*SPR_16416_START*/
        /* Delete nodes stored in UL NAS Transport queue */
        uecc_delete_ul_nas_transport_queue(p_uecc_gb_context->ue_db.ue_contexts[ue_id],
                &(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_ul_nas_transport_queue));
        /*SPR_16416_END*/
        /*SPR_17976_START*/
        /* Deallocate memory previously allocated for
           RRCConnectionReestablishmentReq */
        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_reestab_ue_identity)
        {
            rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_reestab_ue_identity);
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_reestab_ue_identity = PNULL;
        }
        /* Delete nodes stored in TeNB re-establishment queue */
        uecc_delete_tenb_cre_queue(p_uecc_gb_context->ue_db.ue_contexts[ue_id],
                &(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_tenb_cre_event_queue));
        rrc_delete_ho_ue_identity_node(p_uecc_gb_context->ue_db.ue_contexts[ue_id]);
        /*SPR_17976_END*/

        /* Bug 817 Fix Start */
        /* Free Forwarding tunnel information */
        if (p_ue_context->rrc_trg_ho_fwd_tnl_info.
                drb_tunnel_info_count > RRC_NULL)
        {
            for (drb_count = 0; drb_count < MAX_ERAB_COUNT; drb_count++)
            {
                if (PNULL != p_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[drb_count])
                {
                    rrc_mem_free (p_ue_context->rrc_trg_ho_fwd_tnl_info.
                            p_uecc_drb_fwd_tnl_list[drb_count]);
                    p_ue_context->rrc_trg_ho_fwd_tnl_info.
                        p_uecc_drb_fwd_tnl_list[drb_count] = PNULL;
                }
            }
            p_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count = 0;
        }
        /* Bug 817 Fix Stop */
        /*SPR_19387_START*/
        if (p_ue_context->p_sps_config != PNULL)
        {
                rrc_mem_free(p_ue_context->p_sps_config);
                p_ue_context->p_sps_config = RRC_PNULL;
                p_ue_context->m.sps_config_present = RRC_NULL;
        }
        /*SPR_19387_END*/
        /* Stop all procedural timer */
        uecc_ue_timers_stop_all(
                p_uecc_gb_context, 
                &p_uecc_gb_context->ue_db.ue_contexts[ue_id]->timers_data);

        /* Free S1AP ASN context */
        rtFreeContext(&p_uecc_gb_context->
            ue_db.ue_contexts[ue_id]->s1ap_asn1_ctx);

        /* update statistic */
        uecc_status_ue_release(p_uecc_gb_context->ue_db.ue_contexts[ue_id]);
        if (PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            if (PNULL != p_uecc_gb_context->p_p_csc_context[p_uecc_gb_context->
                    ue_db.ue_contexts[ue_id]->cell_index])
            {
                if (PNULL != p_uecc_gb_context->p_p_csc_context[p_uecc_gb_context->
                        ue_db.ue_contexts[ue_id]->cell_index]->p_rrc_uecc_statistic)
                {
                    uecc_status_cell_update_num_of_ue(
                            p_uecc_gb_context, 
                            p_uecc_gb_context->p_p_csc_context[p_uecc_gb_context->ue_db.ue_contexts[ue_id]->
                            cell_index]->p_rrc_uecc_statistic, RRC_FALSE);
                }
            }
        }
        /* Free the memory allocated to the E-RABs, if not freed already */
        for (erb_idx = 0; erb_idx < MAX_ERAB_COUNT; erb_idx++)
        {

            if (
                    p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx]
                    != PNULL)
            {
                /* CR 90 changes start*/
                qci_value = p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx]->
                    e_RABlevelQoSParameters.qCI;
                
                /* SPR Fix 8377 */
                /* SPR 11748 Fix Start */
                if (PNULL != g_rrc_stats.p_rrc_stats)
                {
                    /* SPR 13441 Fix Start */
                    if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                            (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
                    {
                        /*spr_22003_changes_start*/
                        if(RRC_TRUE == p_uecc_gb_context->ue_db.ue_contexts[ue_id]->is_erab_relSuccNbr_qci_counter_pegging_reqd)
                        {
                            RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_gb_context,
                                    g_rrc_stats.p_rrc_stats->
                                    rrc_uecc_stats.rrc_uecc_instance_stats[
                                    p_uecc_gb_context->offset_uecc_mod_id].
                                    rrc_uecc_cell_stats[p_uecc_gb_context->ue_db.
                                    ue_contexts[ue_id]->cell_index].
                                    rrc_erab_related_stats.erab_release_stats.
                                    erab_relSuccNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                            RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_gb_context,
                                    g_rrc_stats.p_rrc_stats->
                                    rrc_uecc_stats.rrc_uecc_instance_stats[
                                    p_uecc_gb_context->offset_uecc_mod_id].
                                    rrc_uecc_cell_stats[p_uecc_gb_context->ue_db.
                                    ue_contexts[ue_id]->cell_index].rrc_erab_related_stats.
                                    erab_release_stats.erab_relSuccNbr_sum, 
                                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
                        }
                        /*spr_22003_changes_end*/
                    }
                    /* SPR 13441 Fix End */
                }
                /* SPR 11748 Fix Stop */
                /* SPR Fix 8377 */
                /* CR 90 changes stop */
                /*SPR 18778 FIX START */ 
                /*Code deleted*/
                /*SPR 18778 FIX START */
                /* SPR 20816 Fix Start */ 
                if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx]->
                                          transportLayerAddress.data)
                {
                    rrc_mem_free ((char *)(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx]->
                                          transportLayerAddress.data));
                    p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx]->
                                          transportLayerAddress.data = PNULL;
                }
                /* SPR 20816 Fix End */ 
                rrc_mem_free(
                        p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx]);
                p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_e_rab_list[erb_idx] = PNULL;
            }
        }

        if (p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_meas_config
                != PNULL)
        {
            rrc_mem_free(
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_meas_config);
            /* BUG_764_FIX Start */
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_meas_config = PNULL;
            /* BUG_764_FIX Stop */
        }

        if (
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_redirected_carrier_info != PNULL)
        {
            rrc_mem_free(
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_redirected_carrier_info);
            /* BUG_764_FIX Start */
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_redirected_carrier_info = PNULL;
            /* BUG_764_FIX Stop */
        }

        if (
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_idle_mode_mobility_control != PNULL)
        {
            rrc_mem_free(
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_idle_mode_mobility_control);
            /* BUG_764_FIX Start */
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_idle_mode_mobility_control = PNULL;
            /* BUG_764_FIX Stop */
        }
        
        /*valgrind_fix start*/
        if(PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_curr_proc_data)
        {
            rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_curr_proc_data);
        }
        /*valgrind_fix stop*/

        /*SPR_18241_START*/
        if(PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_temp_ue_ctx_mod_proc_data)
        {
            rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_temp_ue_ctx_mod_proc_data);
        }
        /*SPR_18241_END*/

        while ( PNULL!=(p_meas_object_node = uecc_queue_pop_node_from_meas_object_list
            (&(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->curr_meas_config.curr_meas_object_to_add_mod_list))))
        {
            /* Free event */
            rrc_mem_free(p_meas_object_node);
            p_meas_object_node = PNULL;
        }
        
        while ( PNULL!=(p_report_config_node = uecc_queue_pop_node_from_report_config_list
            (&(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->curr_meas_config.curr_report_config_to_add_mod_list))))
        {
            /* Free event */
            rrc_mem_free(p_report_config_node);
            p_report_config_node = PNULL;
        }

        while ( PNULL!=(p_ue_capability_node = uecc_queue_pop_node_from_ue_capability_list
            (&(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->ue_radio_capability))))
        {
            /* SPR 13502 Fix Start */
            if (PNULL != p_ue_capability_node->p_rat_string)
            {
                rrc_mem_free(p_ue_capability_node->p_rat_string);
                p_ue_capability_node->p_rat_string = PNULL;
            }
            /* SPR 13502 Fix Stop */

            /* Free event */
            rrc_mem_free(p_ue_capability_node);
            p_ue_capability_node = PNULL;
        }
//cr_761
        if( p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_registered_lai
                != PNULL)
        {
            rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_registered_lai);
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_registered_lai = PNULL;
        }

//cr_220

        if( p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_rlf_timers_and_constants_r9
                != PNULL)
        {
            rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_rlf_timers_and_constants_r9);
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_rlf_timers_and_constants_r9 = PNULL;
        }
        
        p_ue_reconfig_queue = &p_uecc_gb_context->ue_db.ue_contexts[ue_id]->ue_reconfig_queue;
        
        while (RECONFIGURATION_Q_GET_COUNT(p_ue_reconfig_queue))
        {
            RECONFIGURATION_Q_DEQUEUE(p_ue_reconfig_queue, snode);
            rrc_mem_free(snode);
            snode = PNULL;
        }
        
        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_scell_config)
        {
           rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_scell_config);
           p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_scell_config = PNULL;
        }
        /*valgrind_fix_start*/
        if (p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_sps_config != PNULL)
        {    
            rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_sps_config);
            p_uecc_gb_context->ue_db.ue_contexts[ue_id]->p_sps_config = PNULL;
        }    
        /*valgrind_fix_stop*/

        rrc_mem_free(p_uecc_gb_context->ue_db.ue_contexts[ue_id]);
        p_uecc_gb_context->ue_db.ue_contexts[ue_id] = PNULL;

        RRC_ASSERT(p_uecc_gb_context->ue_db.not_allocated_ue_ids_count <
            p_uecc_gb_context->total_ue_supported);


        p_uecc_gb_context->ue_db.not_allocated_ue_ids[
            (p_uecc_gb_context->ue_db.not_allocated_ue_ids_cur +
            p_uecc_gb_context->ue_db.not_allocated_ue_ids_count) %
            (p_uecc_gb_context->total_ue_supported)] = ue_id;

        p_uecc_gb_context->ue_db.not_allocated_ue_ids_count++;

        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_BRIEF,
                "UE context deallocated [UE_INDEX:%u]", ue_index);
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  rrc_ue_index_t ue_index   
*   
*   OUTPUT:        None
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t.
*
*   DESCRIPTION:   This function gets UE context.
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_ue_index_t ue_index  /* UE index  */
)
{
    U16 ue_id = 0;
    uecc_ue_context_t* p_ue_context = NULL;
    
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    ue_id = RRC_GET_UE_INDEX(ue_index);

    if (ue_id >= (p_uecc_gb_context->total_ue_supported))
    {
        return PNULL;
    }

    p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[ue_id];
    if(NULL == p_ue_context)
    {
        return p_ue_context;
    }
    
    SET_CELL_AND_UE_INDEX(p_ue_context->cell_index, p_ue_context->ue_index);
    return p_ue_context;
}
/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_by_crnti
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  rrc_rnti_t    crnti  
*   
*   OUTPUT:        None
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t.
*
*   DESCRIPTION:   This function gets UE context.
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_crnti(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_rnti_t    crnti  /* crnti in RRCConnectionReestablishmentRequest */
)
{
    U16     index_counter = RRC_NULL;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context->ue_db.ue_contexts);
    
    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {

        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
        {
            if (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->crnti ==
                           crnti)
            {
                break;
            }
        }


    }
    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_by_s1ap_id
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  U32                 s1ap_id

*   
*   OUTPUT:        None
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t or PNULL.
*
*   DESCRIPTION:   This function gets UE context by the value of S1AP id.
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_s1ap_id(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U32                 s1ap_id             /* S1AP id */
)
{
    //rrc_ue_index_t    ue_index = RRC_NULL;
    U16               index_counter = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

     /*BUG 604 changes start*/
    //ue_index = (rrc_ue_index_t)UECC_UE_CTX_GET_UE_INDEX_FROM_S1AP_ID(s1ap_id);
    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
        {
            if(p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                    s1ap_ue_id == s1ap_id)
            {
                break;
            }
        }
    }
    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
     /*BUG 604 changes stop*/
}


/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_by_mme_s1ap_id
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  U32                 s1ap_id   
*   
*   OUTPUT:        None
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t or PNULL.
*
*   DESCRIPTION:   This function gets UE context by the value of MME S1AP id.
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_mme_s1ap_id(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U32                 s1ap_id             /* S1AP id */
)
{
    rrc_counter_t ue_index = 0;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    for (ue_index =0; ue_index < 
            p_uecc_gb_context->total_ue_supported;
            ue_index++)
    {
        if ((p_uecc_gb_context->ue_db.ue_contexts[ue_index])&&
                (p_uecc_gb_context->ue_db.
                 ue_contexts[ue_index]->m.mme_ue_id_present)
            && ((p_uecc_gb_context->ue_db.
                    ue_contexts[ue_index]->mme_ue_id ==
                    s1ap_id)))
            {
                return p_uecc_gb_context->
                    ue_db.ue_contexts[ue_index];
            }
    }

    return PNULL;
}
/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_set_meas_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context,
*                  rrc_transaction_id_t    value  
*   
*   OUTPUT:        none
*
*   RETURNS:       none.
*
*   DESCRIPTION:   Set Current operation Transaction id value.
*
******************************************************************************/
void uecc_ue_ctx_set_meas_transction_id(
    uecc_ue_context_t       *p_ue_context,       /* UECC UE context */
    rrc_transaction_id_t    value
)
{
    p_ue_context->meas_transaction_id = value;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_meas_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context,
*   
*   OUTPUT:        none
*
*   RETURNS:       rrc_transaction_id_t.
*
*   DESCRIPTION:   Get Current operation Transaction id value.
*
******************************************************************************/
rrc_transaction_id_t uecc_ue_ctx_get_meas_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
)
{
    return p_ue_context->meas_transaction_id;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_next_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context
*   
*   OUTPUT:        none
*
*   RETURNS:       rrc_transaction_id_t.
*
*   DESCRIPTION:   Get next UECC Transaction id value.
*
******************************************************************************/
rrc_transaction_id_t uecc_ue_ctx_get_next_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
)
{
    /* SPR 16087 Start */
    ++p_ue_context->transaction_id;
    if(RRC_S1U_END_MARKER_RESERVED_TRANSACTION_ID == p_ue_context->transaction_id)
        ++p_ue_context->transaction_id;
    return p_ue_context->transaction_id;
    /* SPR 16087 Stop */
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_set_op_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context
*                  rrc_transaction_id_t    value   
*   
*   OUTPUT:        none
*
*   RETURNS:       none.
*
*   DESCRIPTION:   Set Current operation Transaction id value.
*
******************************************************************************/
void uecc_ue_ctx_set_op_transction_id(
    uecc_ue_context_t       *p_ue_context,       /* UECC UE context */
    rrc_transaction_id_t    value
)
{
    p_ue_context->op_transaction_id = value;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_op_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context
*   
*   OUTPUT:        none
*
*   RETURNS:       rrc_transaction_id_t.
*
*   DESCRIPTION:   Get Current operation Transaction id value.
*
******************************************************************************/
rrc_transaction_id_t uecc_ue_ctx_get_op_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
)
{
    return p_ue_context->op_transaction_id;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_mme_ue_id
*
*   INPUT:         uecc_ue_context_t*  p_ue_context
*   
*   OUTPUT:        none
*
*   RETURNS:       U32
*
*   DESCRIPTION:   Get Current MME_UE_ID or 0 if not present.
*
******************************************************************************/
U32 uecc_ue_ctx_get_mme_ue_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
)
{
    if (0 != p_ue_context->m.mme_ue_id_present)
    {
        return p_ue_context->mme_ue_id;
    }
    return 0;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_look_for_transport_addr
*
*   INPUT:         uecc_ue_context_t       *p_ue_context
*                  U8                      ip_address_octet_size 
*                  U8                      *ip_address
*   
*   OUTPUT:        none
*
*   RETURNS:       RRC_TRUE/RRC_FALSE
*
*   DESCRIPTION:   Looks for first occurrence of Transport layer address in
*                  DRBs contexts list within selected UE context.
*
******************************************************************************/
rrc_bool_t uecc_ue_ctx_look_for_transport_addr(
    uecc_ue_context_t*  p_ue_context,       /* UECC UE context */
    U8                  ip_address_octet_size,
    U8                  *ip_address
)
{
    rrc_bool_t result = RRC_FALSE;
    rrc_counter_t   drb_index = RRC_NULL;
    s1ap_TransportLayerAddress *p_drb_addr = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(0 < ip_address_octet_size);
    RRC_ASSERT(PNULL != ip_address);

    while (drb_index < MAX_ERAB_COUNT)
    {
        if (PNULL == p_ue_context->p_e_rab_list[drb_index])
        {
            drb_index++;
            continue;
        }
        p_drb_addr = &p_ue_context->p_e_rab_list[drb_index]->transportLayerAddress;
        /* Compare addresses */
        if ( (p_drb_addr->numbits == (U32)ip_address_octet_size*8) &&
             (0 == memcmp_wrapper(
                     p_drb_addr->data, ip_address, ip_address_octet_size)))
        {
            /* Transport layer address has been found  */
            result = RRC_TRUE;
            break;
        }
        drb_index++;
    }

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_queue_pop_node_from_meas_object_list
*
*   INPUT:         uecc_meas_object_to_add_mod_list_t * p_list
*   
*   OUTPUT:        none 
*
*
*   RETURNS:       Pointer of uecc_meas_object_to_add_mod_t.
*
*   DESCRIPTION:   This function pops a node from uecc_meas_object_to_add_mod_list_t.
*       
******************************************************************************/
uecc_meas_object_to_add_mod_t* uecc_queue_pop_node_from_meas_object_list(
        uecc_meas_object_to_add_mod_list_t * p_list     
)
{
    uecc_meas_object_to_add_mod_t    *node_p = PNULL;

    node_p = (uecc_meas_object_to_add_mod_t*) ylPopHead (p_list);

    return node_p;
}

/******************************************************************************
*   FUNCTION NAME: uecc_queue_pop_node_from_report_config_list
*
*   INPUT:         uecc_report_config_to_add_mod_list_t * p_list
*   
*   OUTPUT:        none
*
*   RETURNS:       Pointer of uecc_report_config_to_add_mod_t.
*
*   DESCRIPTION:   This function pops a node from uecc_report_config_to_add_mod_list_t.
*
******************************************************************************/
uecc_report_config_to_add_mod_t* uecc_queue_pop_node_from_report_config_list(
        uecc_report_config_to_add_mod_list_t * p_list     
)
{
    uecc_report_config_to_add_mod_t    *node_p = PNULL;

    node_p = (uecc_report_config_to_add_mod_t*) ylPopHead (p_list);

    return node_p;
}

/******************************************************************************
*   FUNCTION NAME: uecc_queue_pop_node_from_ue_capability_list
*
*   INPUT:         UE_Radio_Capability_RAT_Container_list_t * p_list
*   
*   OUTPUT:        none
*
*   RETURNS:       Pointer of UE_Radio_Capability_RAT_Container_t.
*
*   DESCRIPTION:   This function pops a node from UE_Radio_Capability_RAT_Container_list_t.
*
******************************************************************************/

UE_Radio_Capability_RAT_Container_t* uecc_queue_pop_node_from_ue_capability_list(
        UE_Radio_Capability_RAT_Container_list_t * p_list 
)
{
    UE_Radio_Capability_RAT_Container_t    *node_p = PNULL;

    node_p = (UE_Radio_Capability_RAT_Container_t*) ylPopHead (p_list);

    return node_p;
}

/*SPR 5253 Start*/
/*x2ap-start*/
/*SPR_16618_START*/
/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_by_x2ap_id
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context 
*                  U8                  peer_enodeb_id
*                  U16                 enb_ue_x2ap_id
*                  U32                 x2ap_id_type
*   
*   OUTPUT:        none
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t or PNULL.
*
*   DESCRIPTION:   This function gets UE context by the value of X2AP id.
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2ap_id(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
)
/*SPR_16618_END*/
{
    U16               index_counter = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
        {
            /*SPR_16618_START*/
            if ((OLD_X2AP_UE_ID == x2ap_id_type) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                x2ap_context.old_enb_ue_x2ap_id == enb_ue_x2ap_id) &&
                (peer_enodeb_id == 
                p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                x2ap_context.peer_enodeb_id))
            {
                break;
            }
            else if ((NEW_X2AP_UE_ID == x2ap_id_type) &&
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                 x2ap_context.m.new_enb_ue_x2ap_id_present) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                x2ap_context.new_enb_ue_x2ap_id == enb_ue_x2ap_id) &&
                (peer_enodeb_id == 
                p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                x2ap_context.peer_enodeb_id))
            /*SPR_16618_END*/
            {
                break;
            }
        }
    }

    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
}
/*x2ap-end*/
/*SPR 5253 Stop*/

/******************************************************************************
*   FUNCTION NAME: uecc_x2ap_ueid_allocate
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  rrc_ue_index_t      ue_idx 
*   
*   OUTPUT:        none
*
*   RETURNS:       X2AP_UEID or INVALID_X2AP_UE_ID case of error.
*
*   DESCRIPTION:   This function allocates x2ap_ueid .
*
******************************************************************************/
U16 uecc_x2ap_ueid_allocate(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    rrc_ue_index_t      ue_idx           /* UE Idex */
)
{
    U16                 x2ap_ueid = INVALID_X2AP_UE_ID;
    U8                  uecc_module_id = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );

    if (p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_count > 0)
    {
        /*Get unused X2AP_UEID*/
        x2ap_ueid = p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids[
            p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_cur];

        p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_count--;
        p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_cur++;
        if (p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_cur >=
            MAX_NUM_SUPPORTED_X2AP_UEID)
        {
            p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_cur = 0;
        }
        uecc_module_id = (U8)RRC_GET_UECC_MODULE_INDEX(ue_idx);

        x2ap_ueid = (U16)((uecc_module_id << 0x09) | x2ap_ueid);

        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_INFO, "[X2AP_UEID:%u] allocated", x2ap_ueid);
    }
    return x2ap_ueid;
}

/******************************************************************************
*   FUNCTION NAME: uecc_x2ap_ueid_free
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context
*                  U16  x2ap_ueid
*   
*   OUTPUT:        none
*
*   RETURNS:       none.
*
*   DESCRIPTION:   This function frees X2AP_UEID.
*
******************************************************************************/
void uecc_x2ap_id_free(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U16    x2ap_ueid                        /* X2AP_UEID  */
)
{
    RRC_ASSERT( PNULL != p_uecc_gb_context );

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO, "[X2AP_UEID:%u] released", x2ap_ueid);

    x2ap_ueid = ((x2ap_ueid) & (0x1FF));

    p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids[
        (p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_cur +
         p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_count) %
        MAX_NUM_SUPPORTED_X2AP_UEID] = x2ap_ueid;

    p_uecc_gb_context->x2ap_ctx.not_allocated_ue_ids_count++;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_set_counter_check_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context
*                  rrc_transaction_id_t    value  
*   
*   OUTPUT:        none
*
*   RETURNS:       none.
*
*   DESCRIPTION:   Set Current operation Transaction id value.
*
******************************************************************************/
void uecc_ue_ctx_set_counter_check_transction_id(
    uecc_ue_context_t       *p_ue_context,       /* UECC UE context */
    rrc_transaction_id_t    value
)
{
    p_ue_context->counter_check_info.ue_counter_check_trans_id = value;
}

/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_counter_check_transction_id
*
*   INPUT:         uecc_ue_context_t       *p_ue_context
*   
*   OUTPUT:        none
*
*   RETURNS:       rrc_transaction_id_t.
*
*   DESCRIPTION:   Get Current operation Transaction id value.
*
******************************************************************************/
rrc_transaction_id_t uecc_ue_ctx_get_counter_check_transction_id(
    uecc_ue_context_t*  p_ue_context       /* UECC UE context */
)
{
    return p_ue_context->counter_check_info.ue_counter_check_trans_id;
}

/*BUG 604 changes start*/

/*SPR_17792_start*/
/********************************************************************************
*	FUNCTION NAME: 	uecc_ue_ctx_get_by_x2_id_intra_cell_ho_cancel
*
*  	INPUT:         uecc_gb_context_t*  p_uecc_gb_context 
*                      U16                 enb_ue_x2ap_id
*                      U32                 x2ap_id_type
*
* 	OUTPUT:        none
*
* 	RETURNS:       Pointer on UE context data uecc_ue_context_t or PNULL.
* 
*       DESCRIPTION:   This function gets UE context by the value of X2AP id.
*                   	in case of processing HO_CANCEL .
*
*********************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2_id_intra_cell_ho_cancel(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
)
{
    U16               index_counter = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter]) &&
            (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
            ho_info.p_ho_info) && (PNULL != p_uecc_gb_context->ue_db.
            ue_contexts[index_counter]->ho_info.p_ho_info->p_trg_ho_info))
	{
            if ((OLD_X2AP_UE_ID == x2ap_id_type) &&
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.old_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
            else if ((NEW_X2AP_UE_ID == x2ap_id_type) &&
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.new_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
        }
    }

    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
}
	

/*SPR_17792_end*/


/*SPR_16618_START*/
/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_by_x2ap_id_ho_cancel
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context 
*                  U8                  peer_enodeb_id
*                  U16                 enb_ue_x2ap_id
*                  U32                 x2ap_id_type
*   
*   OUTPUT:        none
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t or PNULL.
*
*   DESCRIPTION:   This function gets UE context by the value of X2AP id.
*                  in case of processing HO_CANCEL .
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2ap_id_ho_cancel(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
)
/*SPR_16618_END*/
{
    U16               index_counter = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        /*SPR_16618_START*/
        if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter]) &&
            (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
            ho_info.p_ho_info) && (PNULL != p_uecc_gb_context->ue_db.
            ue_contexts[index_counter]->ho_info.p_ho_info->p_trg_ho_info) &&
            (peer_enodeb_id == 
            p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
            x2ap_context.peer_enodeb_id))
        /*SPR_16618_END*/
        {
            if ((OLD_X2AP_UE_ID == x2ap_id_type) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.old_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
            else if ((NEW_X2AP_UE_ID == x2ap_id_type) &&
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.new_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
        }
    }

    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
}

/*SPR_17792_start*/
/*******************************************************************************
*	 FUNCTION NAME:	uecc_ue_ctx_get_by_x2_id_src_intra_cell_ho
*
*	 INPUT:	   uecc_gb_context_t*  p_uecc_gb_context 
*                  U16                 enb_ue_x2ap_id
*                  U32                 x2ap_id_type
*             
*	OUTPUT:     none
*
*	RETURNS:    Pointer on UE context data uecc_ue_context_t or PNULL.
*
*
*     DESCRIPTION:  This function gets UE context by the value of X2AP id.
*                   in case of processing X2 Messages at source eNodeB.
*
********************************************************************************/

uecc_ue_context_t* uecc_ue_ctx_get_by_x2_id_src_intra_cell_ho(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
)

{
    U16               index_counter = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    for (index_counter = 0;
            index_counter < p_uecc_gb_context->total_ue_supported;
            index_counter++)
    {
        if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter]) &&
                (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                 ho_info.p_ho_info) && (PNULL != p_uecc_gb_context->ue_db.
                     ue_contexts[index_counter]->ho_info.p_ho_info->p_src_ho_info))
        {
            if ((OLD_X2AP_UE_ID == x2ap_id_type) &&
                    (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.old_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
            if ((NEW_X2AP_UE_ID == x2ap_id_type) &&
                    (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.m.new_enb_ue_x2ap_id_present) &&
                    (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.new_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
        }
    }

    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
}

/*SPR_17792_end*/
/*SPR_16618_START*/
/******************************************************************************
*   FUNCTION NAME: uecc_ue_ctx_get_by_x2ap_id_src_enb
*
*   INPUT:         uecc_gb_context_t*  p_uecc_gb_context 
*                  U8                  peer_enodeb_id
*                  U16                 enb_ue_x2ap_id
*                  U32                 x2ap_id_type
*   
*   OUTPUT:        none
*
*   RETURNS:       Pointer on UE context data uecc_ue_context_t or PNULL.
*
*   DESCRIPTION:   This function gets UE context by the value of X2AP id.
*                  in case of processing X2 Messages at source eNodeB.
*
******************************************************************************/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2ap_id_src_enb(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
)
/*SPR_16618_END*/
{
    U16               index_counter = RRC_NULL;

    RRC_ASSERT( PNULL != p_uecc_gb_context );
    RRC_ASSERT( PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        /*SPR_16618_START*/
        if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter]) &&
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
                (
                 (MENB_X2AP_UE_ID == x2ap_id_type) ||
                 (SGNB_X2AP_UE_ID == x2ap_id_type) ||
                 (
                  (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                  ho_info.p_ho_info) && (PNULL != p_uecc_gb_context->ue_db.
                      ue_contexts[index_counter]->ho_info.p_ho_info->p_src_ho_info) 
                 )
                ) &&
/*NR_DC Code Change Stop*/
#else
            (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
            ho_info.p_ho_info) && (PNULL != p_uecc_gb_context->ue_db.
            ue_contexts[index_counter]->ho_info.p_ho_info->p_src_ho_info) &&
#endif
                (peer_enodeb_id == 
                 p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                 x2ap_context.peer_enodeb_id))
        /*SPR_16618_END*/
        {
            if ((OLD_X2AP_UE_ID == x2ap_id_type) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.old_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
            /* SPR 16750 Fix Start */
            if ((NEW_X2AP_UE_ID == x2ap_id_type) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                   x2ap_context.m.new_enb_ue_x2ap_id_present) &&
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.new_enb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
            /* SPR 16750 Fix Stop */
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/;
            if ((MENB_X2AP_UE_ID == x2ap_id_type) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.menb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
            if ((SGNB_X2AP_UE_ID == x2ap_id_type) && 
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                   x2ap_context.m.sgnb_ue_x2ap_id_present) &&
                (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     x2ap_context.sgnb_ue_x2ap_id == enb_ue_x2ap_id))
            {
                break;
            }
/*NR_DC Code Change Stop*/;
#endif
        }
    }

    if (index_counter >= p_uecc_gb_context->total_ue_supported)
    {
        return PNULL;
    }

    return p_uecc_gb_context->ue_db.ue_contexts[index_counter];
}
/*BUG 604 changes stop*/

/*Bug_12446_fix_start*/
/******************************************************************************
*   FUNCTION NAME: rrc_copy_transportlayer_address 
*
*   INPUT        : s1ap_TransportLayerAddress *p_trg - Transport Layer address
*                : to be stored in target
*                : s1ap_TransportLayerAddress *p_src - Transport Layer address
                 : uecc_ue_context_t          *p_ue_context - Pointer to the ue context
*                : of the source
*   
*   OUTPUT       : None
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
*   DESCRIPTION  : This function copies the Trasport Layer Address from source
*                : to target.
*
******************************************************************************/
rrc_return_t rrc_copy_transportlayer_address(
        s1ap_TransportLayerAddress *p_trg,        /* Target */
        s1ap_TransportLayerAddress *p_src,        /* Source */
        uecc_ue_context_t          *p_ue_context /* UECC UE context */
)
{
    U32 size = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);                    

    RRC_ASSERT( PNULL != p_trg );
    RRC_ASSERT( PNULL != p_src);
    RRC_ASSERT( PNULL != p_ue_context);

    if ((PNULL == p_trg) ||
            (PNULL == p_src))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name,
                RRC_FATAL,"Invalid p_trg or p_src");
        return RRC_FAILURE;
    }

    p_trg->numbits
        = p_src->numbits;

    if ((p_trg->numbits)%8)
    {
        size = (p_trg->numbits/8)+1;
    }
    else
    {
        size = p_trg->numbits/8;
    }

    p_trg->data
        = (U8 *)rrc_mem_get(size);

    if (PNULL == p_trg->data)
    {     
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name,
                RRC_FATAL,"Memory Allocation Failure!!");
        return RRC_FAILURE;
    }     
    l3_memcpy_wrapper((void*)( p_trg->data),
            (const void *)( p_src->data),
            size);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);                    

    return RRC_SUCCESS;
}
/*Bug_12446_fix_stop*/

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/;
/******************************************************************************
*   FUNCTION NAME: fill_sgnb_release_cause
*
*   INPUT:         x2ap_Cause *p_x2apCause
*                  s1ap_Cause *p_s1apCause

*
*   OUTPUT:        None
*
*   RETURNS:       Void
*
*   DESCRIPTION:   This function fill the sgnb release cause
*
******************************************************************************/
void fill_sgnb_release_cause(x2ap_Cause *p_x2apCause ,s1ap_Cause *p_s1apCause)
{
    if(RRC_NULL != p_s1apCause)
    {
        switch(p_s1apCause->t)
        {
            case T_s1ap_Cause_radioNetwork:
            {
                p_x2apCause->t = T_x2ap_Cause_radioNetwork;
                p_x2apCause->u.radioNetwork = s1apToX2apMapRadioNetwork[p_s1apCause->u.radioNetwork];
                break;
            }
            case T_s1ap_Cause_transport:
            {
                p_x2apCause->t = T_x2ap_Cause_transport;
                p_x2apCause->u.transport = p_s1apCause->u.transport;
                break;
            }
            case T_s1ap_Cause_nas:
            {
                p_x2apCause->t = T_x2ap_Cause_misc;
                p_x2apCause->u.misc = x2ap_unspecified;

                break;
            }
            case T_s1ap_Cause_protocol:
            {
                p_x2apCause->t = T_x2ap_Cause_protocol;
                if(s1ap_abstract_syntax_error_falsely_constructed_message == p_s1apCause->u.protocol)
                {
                    p_x2apCause->u.protocol = x2ap_unspecified_1;
                }
                else if(s1ap_unspecified_1 == p_s1apCause->u.protocol)
                {
                    p_x2apCause->u.protocol = x2ap_abstract_syntax_error_falsely_constructed_message;
                }
                else
                {
                    p_x2apCause->u.protocol = p_s1apCause->u.protocol;
                }

                break;
            }
            case T_s1ap_Cause_misc:
            {
                p_x2apCause->t = T_x2ap_Cause_misc;

                p_x2apCause->u.misc = s1apToX2apMapMisc[p_s1apCause->u.misc];
                break;
            }
            default:
            {
                p_x2apCause->t = T_x2ap_Cause_misc;
                p_x2apCause->u.misc = x2ap_unspecified;

                break;
            }
        }
    }
    else
    {
        p_x2apCause->t = T_x2ap_Cause_misc;
        p_x2apCause->u.misc = x2ap_unspecified;
    }

}
/****************************************************************************
 * Function Name  : fill_sgnb_cause_for_scgFailureInformationNR_failure_type 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  UInt32             - scg_failure_type
 * Outputs        : None
 * Returns        : None
 * Description    : Fills correct fail cause for SgNB Release based on scg_fail_type
 *                  in received in SCGFailureInformationNR message.
 ****************************************************************************/
void fill_sgnb_cause_for_scgFailureInformationNR_failure_type(
    uecc_ue_context_t*  p_ue_context,
    UInt32              *p_scg_failure_type
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    if (RRC_ONE != p_ue_context->m.sgnbCausePresent)
    {
        if (PNULL != p_scg_failure_type)
        {
            switch(*p_scg_failure_type)
            {
                case 0:
                {
                    /*t310_Expiry*/
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;
                    break;
                }
                case 1:
                {
                    /*randomAccessProblem_1*/
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_failure_in_the_radio_interface_procedure;
                    break;
                }
                case 2:
                {
                    /*rlc_MaxNumRetx_1*/
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;
                    break;
                }
                default:
                {
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;
                    break;
                }
            }
        }
        else
        {
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;
        }

        p_ue_context->m.sgnbCausePresent = RRC_ONE;
    }
}
/*NR_DC Code Change Stop*/;
#endif
