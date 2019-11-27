/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_rlf.c,v 1.12 2010/10/13 07:03:37 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :This file contains the UE connection release procedure 
 *                    in case of radio link failure and errors during the 
 *            handover.
 *
 *
 ****************************************************************************/


/****************************************************************************
* Header File Includes
******************************************************************************/

#include "uecc_rlf.h"
#include "rrc_asn_enc_dec.h"
#include "uecc_logger.h"
#include "rrc_intrl_api.h"
#include "uecc_rcrfsm.h"
#include "uecc_mainfsm.h"
#include "uecc_llim.h"
#include "uecc_rrm.h"
#include "uecc_s1ap.h"

#include "uecc_ue_timer_utils.h"
#include "uecc_common.h"
#include "uecc_fsm_engine.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_RLF_UCR_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_radio_link_failure_handling_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

void uecc_fsm_suspend_ue_entity_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
 );

void uecc_fsm_suspend_ue_entity_resp_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);


void uecc_fsm_suspend_ue_entity_ongoing_init_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_radio_link_failure_handling_finish_failure(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_ue_radio_link_failure_handling_finish_success(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_rlf_s1ap_erab_modify_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
     void                *p_api              /* Input API buffer */
);

void uecc_fsm_rlf_s1ap_erab_setup_req_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
);

void uecc_fsm_rlf_s1ap_erab_release_cmd_handler(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
     void                *p_api              /* Input API buffer */
);
        
void uecc_fsm_rlf_rrm_erab_release_ind_handler(
     uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
      void                *p_api              /* Input API buffer */
);

void uecc_fsm_rlf_ue_release_req_handler(
     uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
      void                *p_api              /* Input API buffer */
);     

void uecc_fsm_radio_link_failure_handler (
     uecc_ue_context_t * p_ue_context,/* UECC UE context */
      void *p_api_data               /* Input API buffer */    
) ;


static void uecc_fsm_ho_required_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
);

static void uecc_fsm_csc_delete_all_ue_req_handler(
      uecc_ue_context_t*      p_ue_context,
       void*                   p_api
);

static void uecc_fsm_rrc_re_establish_handler(
     uecc_ue_context_t   *p_ue_context,
      void                *p_api_data
);

static void uecc_fsm_s1ap_reset_msg_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);    

static void uecc_fsm_rlf_ue_ctx_mod_req_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);    


/****************************************************************************
 Private Variables
****************************************************************************/

/**************************************
 * States definition
 **************************************/

static uecc_fsm_handler_record_t uecc_fsm_suspend_ue_entity_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
            uecc_fsm_suspend_ue_entity_ongoing_init_handler            },
    { { UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP  },
            uecc_fsm_suspend_ue_entity_resp_handler                     },
    { { UECC_EV_TIMER_EXPIRY          },
            uecc_fsm_suspend_ue_entity_timer_expiry_handler            },
    { { UECC_LAST_ID                        },  PNULL                   }
};



static uecc_fsm_activity_state_t uecc_suspend_ue_entity_ongoing =
{
    /* .s_name         = */  (S8*)"UE_ENTITY_SUSPEND_ACTIVITY",
    /* .s_id           = */  (S8*)"UE_SUSPEND_A",
    /* .is_need_init   = */  RRC_TRUE,
    /* .p_mht          = */  uecc_fsm_suspend_ue_entity_ongoing_mht
};

/*Pranab */
static uecc_fsm_activity_t uecc_fsm_ue_suspend_activity =
{
    /*  .s_name         = */(S8*)"Radio link failure Handling activity",
    /* .s_id            = */(S8*)"UE_RLF_A",
    /* .p_first_state   = */&uecc_suspend_ue_entity_ongoing
};


static uecc_fsm_activity_t*
    uecc_fsm_ue_radio_link_failure_handling_procedure_activity_list [] =
{
    &uecc_fsm_ue_suspend_activity,
    PNULL
};

static uecc_fsm_handler_record_t radio_link_failure_procedure_mht[] =
{
        { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_rlf_ue_release_req_handler },
        { { UECC_EV_S1AP_ERAB_MODIFY_REQ              },
            uecc_fsm_rlf_s1ap_erab_modify_req_handler  },
        { { UECC_EV_S1AP_ERAB_SETUP_REQ             },
            uecc_fsm_rlf_s1ap_erab_setup_req_handler },
        { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND       },
             uecc_fsm_rlf_s1ap_erab_release_cmd_handler },
        { { UECC_EV_RRM_ERAB_RELEASE_IND           },
             uecc_fsm_rlf_rrm_erab_release_ind_handler },
        { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
             uecc_fsm_radio_link_failure_handler},
        { { UECC_EV_S1AP_RESET_MSG_IND  },
              uecc_fsm_s1ap_reset_msg_ind_handler },
        { { UECC_EV_RRM_HO_REQUIRED },
              uecc_fsm_ho_required_handler },
        { { UECC_EV_CSC_DELETE_ALL_UE_REQ },
              uecc_fsm_csc_delete_all_ue_req_handler                  },
        { { UECC_EV_RRC_RE_ESTABLISH_REQ  },
            uecc_fsm_rrc_re_establish_handler },
        { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ              },
             uecc_fsm_rlf_ue_ctx_mod_req_handler },
        { { UECC_LAST_ID                            },  PNULL           }         

 };      


uecc_fsm_procedure_t uecc_fsm_ue_radio_link_failure_handling_procedure =
{
    /*.s_name           = */(S8*)"RADIO LINK FAILURE HANDLING",
    /*.s_id             = */(S8*)"UE_RLF_P",
    /*.constructor_f    = */
    uecc_fsm_ue_radio_link_failure_handling_procedure_constructor,
    /*.finish_success_f =*/
    uecc_fsm_ue_radio_link_failure_handling_finish_success,
    /* .finish_fail_f    =*/ 
    uecc_fsm_ue_radio_link_failure_handling_finish_failure, 
    /* .activity_list    =*/
    uecc_fsm_ue_radio_link_failure_handling_procedure_activity_list,
    /*.p_mht            =*/ 
    radio_link_failure_procedure_mht
};


/****************************************************************************
 * Function Name  : uecc_fsm_ue_radio_link_failure_handling_procedure_constructor
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the constructor for radio link failure handling
 *                  procedure.
 ****************************************************************************/
void uecc_fsm_ue_radio_link_failure_handling_procedure_constructor(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO,
                "Inside the Constructor for Radio Link Failure"
                " Handling Procedure ");

    /* Set flag used in connection release procedure */ 
    p_ue_context->
    share_data.uecc_rcr_data.rrc_connection_release_not_required_flag =
    RRC_TRUE ;
 
    /*Set flag which is used by CRE procedure*/ 
    p_ue_context->
    share_data.uecc_rcr_data.rlf_detected = RRC_TRUE ;
    
 
    /* Start timer*/
    if ( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_RLF_UE_SUSPEND_GUARD_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                                    (rrc_ue_index_t)p_ue_context->ue_index))
    {
         RRC_UECC_RLF_UCR_FSM_TRACE(RRC_WARNING,
                                   "[UECC_RLF_UE_SUSPEND_GUARD_TIMER]"
                                   "Timer Start Failed");
    }
    else
    {
        RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO ,
            "Radio Link failure Procedure Timer Started");
                 
    }



    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_radio_link_failure_handling_finish_success
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the constructor for radio link failure handling
 *                  procedure.
 ****************************************************************************/
void uecc_fsm_ue_radio_link_failure_handling_finish_success(
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    uecc_event_queue_push_event(
        UECC_EV_RLF_HANDLING_SUCCESS_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
* Function Name  : uecc_fsm_ue_radio_link_failure_handling_finish_failure
* Input          : uecc_ue_context_t   *p_ue_context
*                  void                *p_api
* Output         : None
* Returns        : None
* Description    : This is the constructor for radio link failure handling
*                  procedure.
****************************************************************************/

void uecc_fsm_ue_radio_link_failure_handling_finish_failure (
uecc_ue_context_t * p_ue_context,
void *p_api /* Input API buffer */
  )
{

    RRC_ASSERT (PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);
    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    uecc_event_queue_push_event (UECC_EV_RLF_HANDLING_FAILURE_INT,
                   p_ue_context, PNULL);

    RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);

}
/**************************************************************************
 * Function Name  : uecc_fsm_suspend_ue_entity_ongoing_init_handler
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the constructor for radio link failure handling
 *                  procedure.
 ***************************************************************************/
void uecc_fsm_suspend_ue_entity_ongoing_init_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_uecc_llim_suspend_ue_entity_req_t ue_suspend_req;
    rrc_counter_t lc_count = 0, erab_idx = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO ,
            "Radio Link failure's Activity for Suspending the"
            "UE Entity Started");
    
    memset_wrapper(&ue_suspend_req, RRC_NULL, 
        sizeof(rrc_uecc_llim_suspend_ue_entity_req_t));
    
    ue_suspend_req.transaction_id = 
    uecc_ue_ctx_get_op_transction_id(p_ue_context);
    ue_suspend_req.ue_index=p_ue_context->ue_index ;
    ue_suspend_req.cell_index=p_ue_context->cell_index ;
    
    /* set this flag if suspend ue entity req send to llim
     * due to radio link failure detection */
    ue_suspend_req.rlf_detected_flag = RRC_TRUE;

    if( ue_suspend_req.transaction_id != RRC_NULL)
    {
        RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO ,
            "Transaction ID added"
            " to rrc_uecc_llim_suspend_ue_entity_req_t ");
    }

    if( ue_suspend_req.ue_index != RRC_NULL)
    {
        RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO ,
            "UE index added "
            " to rrc_uecc_llim_suspend_ue_entity_req_t ");
    }

    /* filling the list of LCs */
    for(erab_idx = 0; erab_idx < MAX_ERAB_COUNT; erab_idx ++)
    {
        if(p_ue_context->p_e_rab_list[erab_idx])
        {
            ue_suspend_req.lc_id_list.lc_id_state[lc_count].lc_id =
                p_ue_context->p_e_rab_list[erab_idx]->
                drb_config.logical_channel_identity;
	    /*SPR 15738 Fix Start*/
            ue_suspend_req.lc_id_list.lc_id_state[lc_count].state = RLC_LLIM_FREEZE;
	    /*SPR 15738 Fix Stop*/
            lc_count++;
        }
    }
    ue_suspend_req.lc_id_list.lc_count = (U8)lc_count;

    if(RRC_SUCCESS != uecc_rrc_send_internal_msg(
        RRC_LLIM_MODULE_ID,
        UECC_LLIM_SUSPEND_UE_ENTITY_REQ,
        sizeof(rrc_uecc_llim_suspend_ue_entity_req_t),
        &ue_suspend_req))
    {

        RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO ,
            "[UECC_LLIM_SUSPEND_UE_ENTITY_REQ] message sending failed ");
             

    }
    else
    {
        RRC_UECC_RLF_UCR_FSM_TRACE(RRC_INFO ,
            "[UECC_LLIM_SUSPEND_UE_ENTITY_REQ] message sent successfully ");

    }      

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_suspend_ue_entity_cnf_handler
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_SUSPEND_UE_ENTITY_CNF from LLIM.
 *
 ****************************************************************************/
void uecc_fsm_suspend_ue_entity_resp_handler (
    uecc_ue_context_t   *p_ue_context,      /* UECC UE context */
    void                *p_api              /* Input API buffer */
)
{
    rrc_uecc_llim_suspend_ue_entity_resp_t*  p_llim_ue_suspend_cnf = PNULL ;
     
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_llim_ue_suspend_cnf =
        (rrc_uecc_llim_suspend_ue_entity_resp_t*)p_api ;


  /* Stop RLF timer here  */
   uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                        UECC_RLF_UE_SUSPEND_GUARD_TIMER,
                       &p_ue_context->timers_data);

   
    switch (p_llim_ue_suspend_cnf->response_code )
    {

        case RRC_SUCCESS:
            uecc_event_queue_push_event(
                UECC_EV_RLF_HANDLING_SUCCESS_INT,
                p_ue_context,
                PNULL);

            break ;

        case RRC_FAILURE :

            uecc_event_queue_push_event(
                UECC_EV_RLF_HANDLING_FAILURE_INT,
                p_ue_context,
                PNULL);

            break;

        default :
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                    "[UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP] Invalid Response=%i",
                    p_llim_ue_suspend_cnf->response_code);

            /* SPR 2592 Start */
            uecc_event_queue_push_event(
                UECC_EV_RLF_HANDLING_FAILURE_INT,
                p_ue_context,
                PNULL);
            /* SPR 2592 Stop */
    }



    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_suspend_ue_entity_timer_expiry_handler
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_SUSPEND_UE_ENTITY_CNF from LLIM.
 *
 ****************************************************************************/
void uecc_fsm_suspend_ue_entity_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* ARGUSED */
    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_BRIEF, "RLF Time out.");

    /* Continue failure processing */
    uecc_event_queue_push_event(
        UECC_EV_RLF_HANDLING_FAILURE_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rlf_rrm_erab_release_ind_handler
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when UE Main FSM receives the
 *                  UECC_EV_RRM_ERAB_RELEASE_IND   message
 ****************************************************************************/
void uecc_fsm_rlf_rrm_erab_release_ind_handler (
  uecc_ue_context_t * p_ue_context, /* UECC UE context */
  void *p_api   /* Input API buffer */
  )
{

     RRC_ASSERT (PNULL != p_ue_context);
     RRC_ASSERT (PNULL != p_api);
     RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

     RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
             p_api);

      RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
              p_ue_context->p_gb_context->facility_name,
              RRC_INFO, "[UECC_EV_RRM_ERAB_RELEASE_IND] Discarding "
              "the Request ");

     RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}


/****************************************************************************
* Function Name  : uecc_fsm_rlf_s1ap_erab_modify_req_handler
* Input          : uecc_ue_context_t   *p_ue_context
*                  void                *p_api
* Output         : None
* Returns        : None
* Description    : This action routine is called when UE Main FSM receives the
*                  UECC_EV_S1AP_ERAB_MODIFY_REQ   message
****************************************************************************/
void uecc_fsm_rlf_s1ap_erab_modify_req_handler (
    uecc_ue_context_t * p_ue_context,   /* UECC UE context */
     void *p_api    /* Input API buffer */
)
{

     RRC_ASSERT (PNULL != p_ue_context);
     RRC_ASSERT (PNULL != p_api);
     RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);

     RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
             p_api);


     RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
             p_ue_context->p_gb_context->facility_name,
             RRC_INFO, "[UECC_EV_S1AP_ERAB_MODIFY_REQ] Discarding "
             "the Request ");

     RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);


}



/****************************************************************************
* Function Name  : uecc_fsm_rlf_s1ap_erab_setup_req_handler
* Input          : uecc_ue_context_t   *p_ue_context
*                  void                *p_api
* Output         : None
* Returns        : None
* Description    : This action routine is called when UE Main FSM receives the
*                  UECC_EV_S1AP_ERAB_MODIFY_REQ   message
****************************************************************************/
void uecc_fsm_rlf_s1ap_erab_setup_req_handler (
   uecc_ue_context_t * p_ue_context,    /* UECC UE context */
   void *p_api  /* Input API buffer */
)
{

     RRC_ASSERT (PNULL != p_ue_context);
     RRC_ASSERT (PNULL != p_api);
     RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);
     RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
             p_api);


     RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
             p_ue_context->p_gb_context->facility_name,
             RRC_INFO, "[UECC_EV_S1AP_ERAB_SETUP_REQ ] Discarding "
             "the Request ");

     RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);


}


/****************************************************************************
* Function Name  : uecc_fsm_rlf_s1ap_erab_release_cmd_handler
* Input          : uecc_ue_context_t   *p_ue_context
*                  void                *p_api
* Output         : None
* Returns        : None
* Description    : This action routine is called when UE Main FSM receives the
*                  UECC_EV_S1AP_ERAB_MODIFY_REQ   message
****************************************************************************/
void uecc_fsm_rlf_s1ap_erab_release_cmd_handler (
    uecc_ue_context_t * p_ue_context,   /* UECC UE context */
     void *p_api    /* Input API buffer */
)
{

  RRC_ASSERT (PNULL != p_ue_context);
  RRC_ASSERT (PNULL != p_api);
  RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);
  RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
          p_api);

  RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
          p_ue_context->p_gb_context->facility_name,
          RRC_INFO, "[ UECC_EV_S1AP_ERAB_RELEASE_COMMAND ] Discarding "
          "the Request ");

  RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);


}

/****************************************************************************
* Function Name  : uecc_fsm_rlf_ue_release_req_handler
* Input          : uecc_ue_context_t   *p_ue_context
*                  void                *p_api
* Output         : None
* Returns        : None
* Description    : This action routine is called when UE Main FSM receives the
*                  UECC_EV_S1AP_ERAB_MODIFY_REQ   message
****************************************************************************/
void uecc_fsm_rlf_ue_release_req_handler(
    uecc_ue_context_t * p_ue_context,   /* UECC UE context */
     void *p_api    /* Input API buffer */
)
{
    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);

    uecc_ue_timer_stop(p_ue_context->p_gb_context,
                            UECC_RLF_UE_SUSPEND_GUARD_TIMER,
                           &p_ue_context->timers_data);

    uecc_fsm_finish_active_procedure(p_ue_context);
    uecc_event_queue_push_event (UECC_EV_UE_RELEASE_REQ,
                       p_ue_context,
                       p_api);
 

    RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_radio_link_failure_handler
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RADIO_LINK_FAILURE
 ****************************************************************************/

void uecc_fsm_radio_link_failure_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
)
{

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);

    RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name, RRC_INFO,
            "[UECC_EV_RADIO_LINK_FAILURE ]RADIO LINK FAILURE already in Progress");

    RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);
                                  
}

/****************************************************************************
 * Function Name  : uecc_fsm_s1ap_reset_msg_ind_handler
 * Input          : uecc_ue_context_t   *p_ue_context
 *                  void                *p_api
 * Output         : None
 * Returns        : None
 * Description    : Handling for the event UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_s1ap_reset_msg_ind_handler (
    uecc_ue_context_t * p_ue_context,
     void *p_api)
{

    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);
   
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_RLF_UE_SUSPEND_GUARD_TIMER,
                           &p_ue_context->timers_data);
    uecc_fsm_finish_active_procedure(p_ue_context);

    uecc_event_queue_push_event (UECC_EV_S1AP_RESET_MSG_IND, p_ue_context, p_api);
    RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_rrc_re_establish_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRC_RE_ESTABLISH
 ****************************************************************************/
void uecc_fsm_rrc_re_establish_handler (
      uecc_ue_context_t * p_ue_context,
      void *p_api_data)
{
    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);


    /* Store the Re-establishment msg in UE Context temporarily */
    if(PNULL != p_ue_context->p_curr_proc_data)
    {
        if(RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "[RLF]: MSG DISCARDING. RE-EST MSG ALREADY STORED.");
            return;
        }
        else if(PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    else
    {
        p_ue_context->p_curr_proc_data = 
            rrc_mem_get(sizeof(curr_proc_data_t));
        
        if(PNULL == p_ue_context->p_curr_proc_data)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                    "[RLF]: Memory Allocation Failure of Current Proc Data");
            return;
        }

        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
    }

    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                rrc_mem_get(sizeof(RRCConnectionReestablishmentRequest_r8_IEs))))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                "[RLF]: Memory Allocation Failure of RE-EST MSG");
        return;
    }
    else
    {
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api_data,
                sizeof(RRCConnectionReestablishmentRequest_r8_IEs));

        p_ue_context->p_curr_proc_data->msg_bfr_type = 
            RRC_RE_ESTABLISH_MSG;

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_INFO, 
                "[RLF]: RE-EST MSG successfully stored during RLF");
    }


   RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_csc_delete_all_ue_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handling for the event UECC_EV_CSC_DELETE_ALL_UE_REQ
 ****************************************************************************/
void uecc_fsm_csc_delete_all_ue_req_handler (
     uecc_ue_context_t * p_ue_context,
    void *p_api
)
{

     RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);
 
     uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_RLF_UE_SUSPEND_GUARD_TIMER,
                            &p_ue_context->timers_data);
     uecc_fsm_finish_active_procedure(p_ue_context);                            

     uecc_event_queue_push_event (UECC_EV_CSC_DELETE_ALL_UE_REQ,
                   p_ue_context, p_api);



     RRC_UECC_UT_TRACE_EXIT (p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ho_required_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRM_HO_REQUIRED 
*********************************************************************/
void uecc_fsm_ho_required_handler (
        uecc_ue_context_t * p_ue_context,
        void *p_api_data)
{
     RRC_ASSERT(PNULL != p_ue_context);

     RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);
     RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
             p_api_data);
    
     RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
             p_ue_context->p_gb_context->facility_name,
             RRC_INFO, "[UECC_EV_RRM_HO_REQUIRED  ]"
             "discarding as RLF has been received");

    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api_data)
    {
        rrc_mem_free(p_api_data);
        p_api_data = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

     RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}



/****************************************************************************
 * Function Name  : uecc_fsm_rlf_ue_ctx_mod_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_UE_CONTEXT_MOD_REQ
 *********************************************************************/
void uecc_fsm_rlf_ue_ctx_mod_req_handler(
    uecc_ue_context_t * p_ue_context,
     void *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RLF_UCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_UT_TRACE_ENTER (p_ue_context->p_gb_context);

    RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO, "[UECC_EV_S1AP_UE_CONTEXT_MOD_REQ]" 
            " discarding as RLF has been received");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}




