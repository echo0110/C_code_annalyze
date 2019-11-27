/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_ct_a.c,v 1.13 2010/10/13 07:03:24 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Capability transfer activity implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_ct_a.c,v $
 * Revision 1.13  2010/10/13 07:03:24  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.9.4.4  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.9.4.3  2010/09/13 13:03:51  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.9.4.2  2010/08/05 08:07:13  gur15697
 * merging round 2
 *
 * Revision 1.9.4.1  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.9  2010/04/07 15:32:59  gur21006
 * warnings removed
 *
 * Revision 1.8  2010/04/01 14:42:21  gur22133
 * *** empty log message ***
 *
 * Revision 1.7  2010/03/29 10:48:42  gur22133
 * sanity_changes
 *
 * Revision 1.6  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.5  2010/03/12 05:50:13  gur22133
 * ut_ue_capability_changes
 *
 * Revision 1.4  2010/02/23 14:11:06  gur22133
 * after_lokesh_ue_cap_code_review_23feb
 *
 * Revision 1.3  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.3  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.4.2  2009/12/08 16:05:18  ukr15916
 * LG_14 fixed
 *
 * Revision 1.1.4.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.6  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.5  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.4.2.3  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.4.2.2  2009/11/20 19:14:40  ukr16032
 * UECC ICS failure processing updated
 *
 * Revision 1.1.2.4.2.1  2009/11/20 15:38:45  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.4  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.3  2009/11/15 12:32:12  ukr15916
 * mme_s1ap_id is stored in uecc context now
 *
 * Revision 1.1.2.2  2009/11/15 10:02:59  ukr15916
 * beautification
 *
 * Revision 1.1.2.1  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_ct_a.h"
#include "uecc_common.h"
#include "uecc_logger.h"
#include "uecc_fsm_engine.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_ed.h"
#include "uecc_rrm.h"
#include "uecc_icsp.h"
/****************************************************************************
 * Local definitions
 ****************************************************************************/

#define RRC_UECC_CT_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        p_ue_context->uecc_fsm.proc_descriptor.p_activity->s_id,\
        ##__VA_ARGS__)

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ct_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_ct_ue_capability_information_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
static void uecc_fsm_ue_capability_rrm_enq_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_rrm_ue_capability_enq_start_timer(
    uecc_ue_context_t   *p_ue_context
);




void uecc_fsm_ue_ct_rrm_ue_capability_enq_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_ct_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);


/****************************************************************************
 Private Variables
****************************************************************************/

/**************************************
 * States definition
 **************************************/
static uecc_fsm_handler_record_t uecc_fsm_ct_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_ct_ongoing_init_handler   },
    { { UECC_EV_RRC_UE_CAPABILITY_INFORMATION   },
            uecc_fsm_ct_ue_capability_information_handler },
    { { UECC_EV_TIMER_EXPIRY              },
            uecc_fsm_ct_timer_expiry_handler },
    { { UECC_LAST_ID                            },  PNULL }
};

static uecc_fsm_activity_state_t uecc_fsm_ct_ongoing_state =
{
   /* .s_name         =*/   (S8*)"UECC_CT_ONGOING",
   /* .s_id           =*/   (S8*)"CT",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_ct_ongoing_mht
};

/**************************************
 * Activity definition
 **************************************/
/* Activity */
uecc_fsm_activity_t uecc_fsm_ue_capability_transfer_activity =
{
   /* .s_name             =*/ (S8*)"UE Capability transfer",
   /* .s_id               =*/ (S8*)"UE_CT_A",
   /* .p_first_state      =*/ &uecc_fsm_ct_ongoing_state
};

/**************************************
 * Failure actions definition
 **************************************/
/**************************************
 * Procedures definition
 **************************************/
/**************************************
 * Activity definition
 **************************************/
/* Activity */
static uecc_fsm_activity_t*
    uecc_fsm_ue_capability_rrm_enq_procedure_activity_list[] =
{
    &uecc_fsm_ue_capability_transfer_activity,
    PNULL
};


static uecc_fsm_handler_record_t
    uecc_fsm_ue_capability_rrm_enq_procedure_mht[] =
{
    { { UECC_EV_TIMER_EXPIRY              },
            uecc_fsm_ue_ct_rrm_ue_capability_enq_timer_expiry_handler },
    { { UECC_LAST_ID                      }, PNULL    }
};


/* RRM initiated UE Capability Enquiry Req in  UECC_UE_CONNECTED state */
uecc_fsm_procedure_t uecc_fsm_ue_capability_rrm_enq_procedure =
{
  /*  .s_name           =*/ (S8*)"UCT due to RRM enq req",
  /*  .s_id             =*/ (S8*)"UE_CT_RRM_ENQ",
  /*  .constructor_f    =*/ uecc_fsm_ue_capability_rrm_enq_procedure_constructor,
  /*  .finish_success_f =*/
                uecc_fsm_ue_capability_rrm_enq_procedure_finish_success,
  /*  .finish_fail_f    = */ uecc_fsm_ue_capability_rrm_enq_procedure_finish_failure,
  /*  .activity_list    = */uecc_fsm_ue_capability_rrm_enq_procedure_activity_list,
  /*  .p_mht            = */ uecc_fsm_ue_capability_rrm_enq_procedure_mht
};

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_capability_enq_start_timer
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Starts UE_CT_RRM_ENQ Procedural timer
 ****************************************************************************/
void uecc_fsm_rrm_ue_capability_enq_start_timer(
    uecc_ue_context_t   *p_ue_context
)
{
    uecc_timer_buffer_t repeated_timer_buf;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ( PNULL != p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER] )
    {
        /* Already started */
        return;
    }


    /* Start the Procedural timer*/
    repeated_timer_buf.type = UECC_TIMER_TYPE_RRC_PROCEDURAL;
    repeated_timer_buf.id = UECC_UE_CAPABILITY_ENQ_TIMER;
    repeated_timer_buf.auto_delete = RRC_TRUE;
    repeated_timer_buf.ue_index = p_ue_context->ue_index;
    repeated_timer_buf.fsm_event_id = UECC_EV_TIMER_EXPIRY;

    

    p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER] = rrc_start_timer(
        p_ue_context->p_gb_context->ue_timers_data.durations[UECC_UE_CAPABILITY_ENQ_TIMER],
        &repeated_timer_buf,
        sizeof(uecc_timer_buffer_t),
        RRC_FALSE);

    if ( PNULL == p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER])
    {
        RRC_UECC_CT_FSM_TRACE(RRC_ERROR,
            "[UECC_RRM_UE_CAPABILITY_ENQ_TIMER] Starting error");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_capability_rrm_enq_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE capability enquiry by RRM procedure
 ****************************************************************************/
void uecc_fsm_ue_capability_rrm_enq_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /*SPR_17893_START*/
    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
		    (p_ue_context->p_gb_context)->facility_name,
		    RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /*SPR_17893_END*/


    /* Start timer */
    uecc_fsm_rrm_ue_capability_enq_start_timer(p_ue_context);
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_capability_rrm_enq_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - api data
 * Outputs        : None
 * Returns        : None
 * Description    : success finish for UE_CT_RRM_ENQ procedure
 ****************************************************************************/
void uecc_fsm_ue_capability_rrm_enq_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_counter_t rat_type_index = 0;
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    RRC_UECC_CT_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    
    RRC_ASSERT(PNULL != p_ue_context);
    uecc_fsm_rrm_ue_capability_enq_stop_timer(p_ue_context);    /* stopping the timer */
    p_ue_context->rrm_ue_capability_enq_ind = RRC_FALSE;

    p_ue_context->rrm_ue_capability_enq_count = 0;

    for(rat_type_index = 0;rat_type_index < MAX_RAT_CAPABILITY;
    rat_type_index++)
    {
        p_ue_context->rrm_ue_capability_enq_list[rat_type_index] = 0;
    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_capability_rrm_enq_procedure_finish_failure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - api data
 * Outputs        : None
 * Returns        : None
 * Description    : failure finish for UE_CT_RRM_ENQ procedure
 ****************************************************************************/
void uecc_fsm_ue_capability_rrm_enq_procedure_finish_failure(
        uecc_ue_context_t*  p_ue_context,
        void*                   p_api           
)
{
    rrc_counter_t rat_type_index = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_CT_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    uecc_fsm_rrm_ue_capability_enq_stop_timer(p_ue_context);    /* stopping the timer */
    if(RRC_SUCCESS != uecc_rrm_build_and_send_failure_ue_capability_info_to_rrm(p_ue_context))
    {
        RRC_UECC_CT_FSM_TRACE(RRC_ERROR,
            "failure message not sent to RRM"); 
    }
    p_ue_context->rrm_ue_capability_enq_ind = RRC_FALSE;
    p_ue_context->rrm_ue_capability_enq_count = 0;

    for(rat_type_index = 0;rat_type_index < MAX_RAT_CAPABILITY;
    rat_type_index++)
    {
        p_ue_context->rrm_ue_capability_enq_list[rat_type_index] = 0;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_ue_capability_enq_stop_timer
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Stop UE_CT_RRM_ENQ Procedural timer
 ****************************************************************************/
void uecc_fsm_rrm_ue_capability_enq_stop_timer(
    uecc_ue_context_t   *p_ue_context
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop procedural timer */
    if (p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER] != PNULL)
    {
        /*SPR_17893_START*/

        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                        (p_ue_context->p_gb_context)->facility_name,
                        RRC_BRIEF,
                        "Stop timer [UECC_RRM_UE_CAPABILITY_ENQ_TIMER]");
        rrc_stop_timer(p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER]);
        p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER] = PNULL;
    }
    else if(p_ue_context->timers_data.timers_ids[UECC_UE_CAPABILITY_ENQ_TIMER] == PNULL)
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_BRIEF,
                "Timer Already Stopped [UECC_RRM_UE_CAPABILITY_ENQ_TIMER]");
        /*SPR_17893_END*/
    }
        

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ct_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_ct_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CT_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /*SPR_17893_START*/
    if ((1 == p_ue_context->m.access_stratum_release) &&
            (RRC_FALSE == p_ue_context->rrm_ue_capability_enq_ind)
#ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        && ( RRC_NULL == p_ue_context->nr_capab_req_flag )
        /* MENB CHANGES - END */
#endif
       )
    {
        RRC_UECC_CT_FSM_TRACE(RRC_BRIEF, "UE Capability received from MME");

        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    else
    {
        /* Enquiry UE capabilities */
        if (RRC_SUCCESS !=     
                    uecc_llim_build_and_send_rrc_ue_capability_enquiry(p_ue_context))
        {
            /* Process failure */
            uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
        }
    }
    /*SPR_17893_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ct_ue_capability_information_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_UE_CAPABILITY_INFORMATION message.
 ****************************************************************************/
void uecc_fsm_ct_ue_capability_information_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    /*SPR_17893_START*/
    if((T_UECapabilityInformation_criticalExtensions_c1 ==
                ((UECapabilityInformation*)p_api)->criticalExtensions.t) &&
            (T_UECapabilityInformation_criticalExtensions_c1_ueCapabilityInformation_r8 ==
             ((UECapabilityInformation*)p_api)->criticalExtensions.u.c1->t))
    {
        if((1 < ((UECapabilityInformation*)p_api)->criticalExtensions.u.c1->
                    u.ueCapabilityInformation_r8->
                    ue_CapabilityRAT_ContainerList.count) ||
                ((PNULL !=
                  ((UECapabilityInformation*)p_api)
                  ->criticalExtensions.u.c1->u.ueCapabilityInformation_r8->
                  ue_CapabilityRAT_ContainerList.head) &&
                 (PNULL !=
                  ((UECapabilityInformation*)p_api)
                  ->criticalExtensions.u.c1->u.ueCapabilityInformation_r8->
                  ue_CapabilityRAT_ContainerList.head->data) &&
                 (RRC_RAT_TYPE_UTRA !=
                  ((UE_CapabilityRAT_Container *)(((UECapabilityInformation*)p_api)
                      ->criticalExtensions.u.c1->u.ueCapabilityInformation_r8->
                      ue_CapabilityRAT_ContainerList.head->data))->rat_Type)))
        {
            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_capability_info_ind(
                        p_ue_context,
                        (UECapabilityInformation*)p_api))
            {
                /* Process failure */
                uecc_fsm_finish_activity_failure(
                        p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return;
            }
        }

        /* sending message to RRM */
        if(RRC_SUCCESS ==
                uecc_rrm_build_and_send_ue_capability_info(p_ue_context,(UECapabilityInformation*)p_api))
        {
            #ifdef ENDC_ENABLED
            /* MENB CHANGES - START */
            if (( RRC_ONE == p_ue_context->endcConfigInfo.enDcR15Supported ) &&
                ( RRC_ONE != p_ue_context->nr_capab_req_flag ))
            {
                p_ue_context->nr_capab_req_flag = RRC_ONE;
                uecc_fsm_ct_ongoing_init_handler(p_ue_context,PNULL);
                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                    (p_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,"Build and send UE Capability Enquiry for NR");
            }
            else
            {
                uecc_fsm_finish_activity_success(p_ue_context, PNULL);
            }
            /* MENB CHANGES - END */
            #else
            uecc_fsm_finish_activity_success(p_ue_context, PNULL);
            #endif
        }
        else
        {
            /* Process failure */
            uecc_fsm_finish_activity_failure(
                    p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
        }
    }
    else
    {
        /* Process failure */
        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    }
    /*SPR_17893_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*****************************************************************************
 * Function Name  : uecc_fsm_ue_ct_rrm_ue_capability_enq_timer_expiry_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles timer expiry 
 ********************************************************************************/
void uecc_fsm_ue_ct_rrm_ue_capability_enq_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_CT_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* BUG: 7882 Start */
/****************************************************************************
 * Function Name  : uecc_fsm_ct_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_ct_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CT_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    if ( RRC_ONE != p_ue_context->nr_capab_req_flag )
    {
        /*SPR_17893_START*/
        if(RRC_FALSE == p_ue_context->rrm_ue_capability_enq_ind)
        {   
            p_ue_context->share_data.p_uecc_rm_data->cause.t =
                T_s1ap_Cause_radioNetwork;
            p_ue_context->share_data.p_uecc_rm_data->cause.u.protocol =
                s1ap_failure_in_radio_interface_procedure;
        }   
        /*SPR_17893_END*/

        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    } 
    else
    {
        /* Start ICS Timer */
        if ( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_CT_FSM_TRACE(RRC_INFO,"Timer for eutra-nr Capability Response is not started.");
            uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
        }

        RRC_UECC_CT_FSM_TRACE(RRC_INFO,"Timer Expired waiting for eutra-nr Capability Response.");
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    /* MENB CHANGES - END */
    #else
    /*SPR_17893_START*/
    if(RRC_FALSE == p_ue_context->rrm_ue_capability_enq_ind)
    {   
        p_ue_context->share_data.p_uecc_rm_data->cause.t =
            T_s1ap_Cause_radioNetwork;
        p_ue_context->share_data.p_uecc_rm_data->cause.u.protocol =
            s1ap_failure_in_radio_interface_procedure;
    }   
    /*SPR_17893_END*/

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    #endif 

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* BUG: 7882 End */

/*SPR_17893_START*/
/****************************************************************************
 * Function Name  : uecc_rrm_ue_capability_enq_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when Main FSM receives the
 *                  UECC_EV_RRM_UE_CAPABILITY_ENQUIRY_REQ message.
 ****************************************************************************/
void uecc_rrm_ue_capability_enq_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    rrc_counter_t rat_type_index = 0;

    /* Start timer */
    uecc_fsm_rrm_ue_capability_enq_start_timer(p_ue_context);

    if (RRC_SUCCESS !=     
            uecc_llim_build_and_send_rrc_ue_capability_enquiry(p_ue_context))
    {
        /* Process failure */
        p_ue_context->share_data.p_uecc_rm_data->cause.t =
            T_s1ap_Cause_radioNetwork;
        p_ue_context->share_data.p_uecc_rm_data->cause.u.protocol =
            s1ap_failure_in_radio_interface_procedure;

        if(RRC_SUCCESS != uecc_rrm_build_and_send_failure_ue_capability_info_to_rrm(p_ue_context))
        {
            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                    (p_ue_context->p_gb_context)->facility_name,
                    RRC_ERROR,"Failed to send ue_capability_info to RRM");
        }

        p_ue_context->rrm_ue_capability_enq_ind = RRC_FALSE;
        p_ue_context->rrm_ue_capability_enq_count = 0;
        p_ue_context->rrm_ue_capability_enq_flag_after_attach = RRC_FALSE;

        for(rat_type_index = 0;rat_type_index < MAX_RAT_CAPABILITY;
                rat_type_index++)
        {
            p_ue_context->rrm_ue_capability_enq_list[rat_type_index] = 0;
        }

    }

    p_ue_context->rrm_ue_capability_enq_flag_after_attach = RRC_TRUE;


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_ue_capability_information_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when uecc recieves ue 
 *                  capability information from llim.
 ****************************************************************************/
void uecc_ue_capability_information_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    rrc_counter_t rat_type_index = 0;
    
    /* SES-508 Fix End */
    if((T_UECapabilityInformation_criticalExtensions_c1 ==
                ((UECapabilityInformation*)p_api)->criticalExtensions.t) &&
            (T_UECapabilityInformation_criticalExtensions_c1_ueCapabilityInformation_r8 ==
             ((UECapabilityInformation*)p_api)->criticalExtensions.u.c1->t))
    {
        if((1 < ((UECapabilityInformation*)p_api)->criticalExtensions.u.c1->
                    u.ueCapabilityInformation_r8->
                    ue_CapabilityRAT_ContainerList.count) ||
                ((PNULL !=
                  ((UECapabilityInformation*)p_api)
                  ->criticalExtensions.u.c1->u.ueCapabilityInformation_r8->
                  ue_CapabilityRAT_ContainerList.head) &&
                 (PNULL !=
                  ((UECapabilityInformation*)p_api)
                  ->criticalExtensions.u.c1->u.ueCapabilityInformation_r8->
                  ue_CapabilityRAT_ContainerList.head->data) &&
                 (RRC_RAT_TYPE_UTRA !=
                  ((UE_CapabilityRAT_Container *)(((UECapabilityInformation*)p_api)
                      ->criticalExtensions.u.c1->u.ueCapabilityInformation_r8->
                      ue_CapabilityRAT_ContainerList.head->data))->rat_Type)))
        {
            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_capability_info_ind(
                        p_ue_context,
                        (UECapabilityInformation*)p_api))
            {
                if(RRC_SUCCESS != uecc_rrm_build_and_send_failure_ue_capability_info_to_rrm(p_ue_context))
                {
                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                            (p_ue_context->p_gb_context)->facility_name,
                            RRC_ERROR,"Failed to send ue_capability_info to RRM");
                }
            }
        }
        /* checking result and sending message to RRM */
        if(RRC_SUCCESS != 
                uecc_rrm_build_and_send_ue_capability_info(p_ue_context,(UECapabilityInformation*)p_api)) 
        {
            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                    (p_ue_context->p_gb_context)->facility_name,
                    RRC_ERROR,"Failed to send ue_capability_info to RRM");

        }

    }
    else
    {
        if(RRC_SUCCESS != uecc_rrm_build_and_send_failure_ue_capability_info_to_rrm(p_ue_context))
        {
            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                    (p_ue_context->p_gb_context)->facility_name,
                    RRC_ERROR,"failure message not sent to RRM"); 
        }
    }
    uecc_fsm_rrm_ue_capability_enq_stop_timer(p_ue_context);    /* stopping the timer */
    p_ue_context->rrm_ue_capability_enq_ind = RRC_FALSE;
    p_ue_context->rrm_ue_capability_enq_flag_after_attach = RRC_FALSE;

    p_ue_context->rrm_ue_capability_enq_count = 0;

    for(rat_type_index = 0;rat_type_index < MAX_RAT_CAPABILITY;
    rat_type_index++)
    {
        p_ue_context->rrm_ue_capability_enq_list[rat_type_index] = 0;
    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_ue_capability_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for 
*                   UECC_UE_CAPABILITY_ENQ_TIMER expiry. 
 ****************************************************************************/
void uecc_ue_capability_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
            (p_ue_context->p_gb_context)->facility_name,
            RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    rrc_counter_t rat_type_index = 0;

    if(RRC_SUCCESS != uecc_rrm_build_and_send_failure_ue_capability_info_to_rrm(p_ue_context))
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_ERROR,"failure message not sent to RRM"); 
    }

    p_ue_context->rrm_ue_capability_enq_ind = RRC_FALSE;
    p_ue_context->rrm_ue_capability_enq_count = 0;
    p_ue_context->rrm_ue_capability_enq_flag_after_attach = RRC_FALSE;

    for(rat_type_index = 0;rat_type_index < MAX_RAT_CAPABILITY;
    rat_type_index++)
    {
        p_ue_context->rrm_ue_capability_enq_list[rat_type_index] = 0;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17893_END*/




