/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_init.c,v 1.4 2010/10/13 07:04:20 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the init function handler for S1AP
 *                     that is registered with CSPL.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_init.c,v $
 * Revision 1.4  2010/10/13 07:04:20  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.2  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.3.4.1  2010/06/24 12:56:36  gur22047
 * internal source and s1ap code
 *
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.8  2009/12/28 10:14:40  gur21006
 * system calls removed
 *
 * Revision 1.1.2.7  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.6  2009/12/27 08:39:04  gur18569
 * removed tabs
 *
 * Revision 1.1.2.5  2009/12/26 12:05:01  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.4  2009/12/23 06:30:26  gur21006
 * Warnings Removed
 *
 * Revision 1.1.2.3  2009/12/01 15:07:24  gur18569
 * integration changes
 *
 * Revision 1.1.2.2  2009/11/29 09:19:42  gur18569
 * changes during sctp intg
 *
 * Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.6  2009/11/22 11:49:14  gur18569
 * ut fixes
 *
 * Revision 1.1.2.5  2009/11/20 14:05:33  gur21006
 * UMR removed
 *
 * Revision 1.1.2.4  2009/11/10 11:41:55  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.3  2009/11/09 09:18:23  gur18569
 * removed compilation warnings
 *
 * Revision 1.1.2.2  2009/11/08 11:05:27  gur20470
 * removed compilation warnings
 *
 * Revision 1.1.2.1  2009/10/23 16:10:17  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "s1ap_global_ctx.h"
#include "s1ap_timers.h"
#include "s1ap_logger.h"
#include "s1ap_handlers.h"
#include "sctp_wrappers.h"
#include "s1ap_oam.h"
#include "s1ap_enb_mgr_fsm.h"

/* These are used for compare function of a search tree*/
#define FIRST_IS_LESS_THAN_SECOND         -1
#define FIRST_IS_GREATER_THAN_SECOND       1
#define FIRST_IS_EQUAL_TO_SECOND           0
#define ERROR_IN_COMPARE                   0


/******************************************************************************
  Private Definitions
 *****************************************************************************/



/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
const void * key_of_socket_desc_search_tree (
        /* Pointer to Tree node */
        const YTNODE * p_node);

S32 compare_socket_desc_search_tree ( 
        /* Pointer to first element being compared */
        const void * p_elem1 , 

        /* Pointer to second element being compared */
        const void * p_elem2);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/*****************************************************************************
 * Function Name  : s1ap_init
 * Inputs         : p_s1ap_init_data - Pointer to S1AP stack initialization 
 *                                     data. Unused
 * Outputs        : None
 * Returns        : Buffer that will be passed in s1ap_process_message
 *                  and in s1ap_process_timer_msg callbacks
 * Description    : This function is CSPL callback. It initialize S1AP entity.
 *****************************************************************************/
void* s1ap_init
(
 /* Pointer to S1AP stack initialization data. Unused */
 void *p_s1ap_init_data
 )
{
    s1ap_gb_context_t *p_s1ap_gl_ctx = PNULL;

    s1ap_timer_buffer_t timer_buf;
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_DETAILEDALL, "Unused variable in s1ap_init function: %p",p_s1ap_init_data);

    RRC_S1AP_TRACE(RRC_DETAILED, "Init.");

    /* Allocate Memory for S1AP global context data structure */
    p_s1ap_gl_ctx = (s1ap_gb_context_t *)rrc_mem_get(sizeof(s1ap_gb_context_t));

    if(PNULL == p_s1ap_gl_ctx)
    {
        RRC_S1AP_TRACE(RRC_FATAL, "s1ap_init: unable to Allocate Memory for "
                "S1AP global context!");
        return PNULL;
    }
    else
    {
        timer_buf.timer_type = S1AP_OAM_INIT_TIMER;
        timer_buf.mme_id = MAX_NUM_MME;
        timer_buf.auto_delete = RRC_FALSE;

        p_s1ap_gl_ctx->oam_init_ind_timer = RRC_NULL; 
        /* Init s1ap DB */
        memset_wrapper((void*)p_s1ap_gl_ctx, RRC_NULL, sizeof(s1ap_gb_context_t));
        p_s1ap_gl_ctx->s1_config_resp_duration = 
            S1AP_TIMER_S1_SETUP_RESP_DEFAULT_DURATION;
        p_s1ap_gl_ctx->s1ap_conn_recovery_duration = 
            S1AP_TIMER_CONN_RECOVERY_DEFAULT_DURATION;

        p_s1ap_gl_ctx->s1_pws_duration = 
            S1AP_TIMER_PWS_DEFAULT_DURATION;

        p_s1ap_gl_ctx->s1_kill_duration = 
            S1AP_TIMER_KILL_DEFAULT_DURATION;
        
        /*Initialize Enb Manager FSM*/
        p_s1ap_gl_ctx->current_enb_mgr_state = ENB_MGR_IDLE;
        
        p_s1ap_gl_ctx->p_pws_info = PNULL;

        p_s1ap_gl_ctx->trans_id = RRC_NULL;         

        p_s1ap_gl_ctx->total_ue_supported = RRC_NULL;

        list_init(&p_s1ap_gl_ctx->reset_context_list);
        
        /*REL 1.2:HANDOVER PROCEDURE START*/
        list_init(&p_s1ap_gl_ctx->ho_ongoing_ue_ctx_list);
        /*REL 1.2:HANDOVER PROCEDURE END*/

	/* SPR 15641 Fix Start */
	list_init(&p_s1ap_gl_ctx->s1ap_kill_queue);
	/* SPR 15641 Fix Stop */
	
        /* SPR 17842 Fix Start */
	#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
        /* Initialise logging parameters to default values */
        p_s1ap_gl_ctx->log_level    = RRC_BRIEF;
        p_s1ap_gl_ctx->log_enabled  = RRC_TRUE;
        #endif
	/* SPR 17842 Fix Stop */
        
        p_s1ap_gl_ctx->s1ap_oam_tx_info.reset_response = RRC_SUCCESS;

        /* set S1AP module state to initial */
        S1AP_SET_FSM_STATE(p_s1ap_gl_ctx,S1AP_INIT);
            
        /* SPR 13432 Fix Start */
        p_s1ap_gl_ctx->last_selected_mme_id = MAX_NUM_MME;
	    p_s1ap_gl_ctx->s1_conn_mode_one_flag = S1AP_FALSE;
        /* SPR 13432 Fix Stop */

        st_init( &(p_s1ap_gl_ctx->sctp_socket_desc_search_tree), 
                &compare_socket_desc_search_tree,
                key_of_socket_desc_search_tree);

        /* Send S1AP INIT to OAM */
        s1ap_process_init_timer(
                p_s1ap_gl_ctx);

        /* Start repeated timer for sending S1AP_OAM_INIT_IND */
        p_s1ap_gl_ctx->oam_init_ind_timer = 
            rrc_start_timer(S1AP_TIMER_OAM_INIT_IND_DEFAULT_DURATION,
                    &timer_buf,
                    sizeof(timer_buf),
                    RRC_TRUE);
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return p_s1ap_gl_ctx;
}

/*bug 115771 begin*/
 /*****************************************************************************
 * Function Name  : s1ap_gb_ctx_reset
 * Inputs         : s1ap_gb_context_t - Pointer to s1ap global context
 * Description    : This function is used to reset global context
 *****************************************************************************/
void s1ap_gb_ctx_reset(s1ap_gb_context_t *p_s1ap_gl_ctx)
 {
    s1ap_timer_buffer_t timer_buf;
    RRC_S1AP_UT_TRACE_ENTER();

    timer_buf.timer_type = S1AP_OAM_INIT_TIMER;
    timer_buf.mme_id = MAX_NUM_MME;
    timer_buf.auto_delete = RRC_FALSE;

    p_s1ap_gl_ctx->oam_init_ind_timer = RRC_NULL; 
    /* Init s1ap DB */
    memset_wrapper((void*)p_s1ap_gl_ctx, RRC_NULL, sizeof(s1ap_gb_context_t));
    p_s1ap_gl_ctx->s1_config_resp_duration = 
        S1AP_TIMER_S1_SETUP_RESP_DEFAULT_DURATION;
    p_s1ap_gl_ctx->s1ap_conn_recovery_duration = 
        S1AP_TIMER_CONN_RECOVERY_DEFAULT_DURATION;

    p_s1ap_gl_ctx->s1_pws_duration = 
        S1AP_TIMER_PWS_DEFAULT_DURATION;

    p_s1ap_gl_ctx->s1_kill_duration = 
        S1AP_TIMER_KILL_DEFAULT_DURATION;
    
    /*Initialize Enb Manager FSM*/
    p_s1ap_gl_ctx->current_enb_mgr_state = ENB_MGR_IDLE;
    
    p_s1ap_gl_ctx->p_pws_info = PNULL;

    p_s1ap_gl_ctx->trans_id = RRC_NULL;         

    p_s1ap_gl_ctx->total_ue_supported = RRC_NULL;

    list_init(&p_s1ap_gl_ctx->reset_context_list);
    
    /*REL 1.2:HANDOVER PROCEDURE START*/
    list_init(&p_s1ap_gl_ctx->ho_ongoing_ue_ctx_list);
    /*REL 1.2:HANDOVER PROCEDURE END*/

	/* SPR 15641 Fix Start */
	list_init(&p_s1ap_gl_ctx->s1ap_kill_queue);
	/* SPR 15641 Fix Stop */

    /* SPR 17842 Fix Start */
	#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
	    /* Initialise logging parameters to default values */
	    p_s1ap_gl_ctx->log_level    = RRC_BRIEF;
	    p_s1ap_gl_ctx->log_enabled  = RRC_TRUE;
	    #endif
	/* SPR 17842 Fix Stop */
    
    p_s1ap_gl_ctx->s1ap_oam_tx_info.reset_response = RRC_SUCCESS;

    /* set S1AP module state to initial */
    S1AP_SET_FSM_STATE(p_s1ap_gl_ctx,S1AP_INIT);
        
    /* SPR 13432 Fix Start */
    p_s1ap_gl_ctx->last_selected_mme_id = MAX_NUM_MME;
    p_s1ap_gl_ctx->s1_conn_mode_one_flag = S1AP_FALSE;
    /* SPR 13432 Fix Stop */

    st_init( &(p_s1ap_gl_ctx->sctp_socket_desc_search_tree), 
            &compare_socket_desc_search_tree,
            key_of_socket_desc_search_tree);

    /* Send S1AP INIT to OAM */
    s1ap_process_init_timer(
            p_s1ap_gl_ctx);

    /* Start repeated timer for sending S1AP_OAM_INIT_IND */
    p_s1ap_gl_ctx->oam_init_ind_timer = 
        rrc_start_timer(S1AP_TIMER_OAM_INIT_IND_DEFAULT_DURATION,
                &timer_buf,
                sizeof(timer_buf),
                RRC_TRUE);


    RRC_S1AP_UT_TRACE_EXIT();
 }
/*bug 115771 end*/
/*****************************************************************************
 * Function Name  : compare_socket_desc_search_tree
 * Inputs         : p_elem1 - Pointer to first element being compared
 *                  p_elem2 - Pointer to second element being compared
 * Outputs        : None
 * Returns        : 0 if both elements are equal.
 *                  1 if first is greater than second.
 *                 -1 if first is less than second.
 * Description    : This function is used by CSPL Tree types to compare 
 *                  elements in a tree. This function compares data of type 
 *                  U32.
 *****************************************************************************/
S32 compare_socket_desc_search_tree
( 
 /* Pointer to first element being compared */
 const void * p_elem1 , 

 /* Pointer to second element being compared */
 const void * p_elem2
 )
{
    if (p_elem1 == PNULL || p_elem2 == PNULL)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s: passed null arg",__FUNCTION__);
        return ERROR_IN_COMPARE;
    }

    if ( * ((const U32 *) p_elem1) == * ((const U32 *) p_elem2))
    {
        return FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((const U32 *) p_elem1) < * ((const U32 *) p_elem2))
    {
        return FIRST_IS_LESS_THAN_SECOND;
    }
    else /*( * ((U32 *) p_elem1) > * ((U32 *) p_elem2))*/
    {
        return FIRST_IS_GREATER_THAN_SECOND;
    }
}


/*****************************************************************************
 * Function Name  : key_of_socket_desc_search_tree
 * Inputs         : p_node - Pointer to tree node.
 * Outputs        : None
 * Returns        : PNULL: Error
 *                  Pointer to key element in this case sctp_sd.
 * Description    : This function returns pointer to key element in a search 
 *                  node.
 *****************************************************************************/
const void * key_of_socket_desc_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 )
{
    return (p_node) ? ( &(((sctp_sd_search_node_t *) p_node)->sctp_sd)) : PNULL;
}


