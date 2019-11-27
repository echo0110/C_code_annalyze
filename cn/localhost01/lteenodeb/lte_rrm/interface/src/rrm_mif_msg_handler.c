/****************************************************************************
 *
 *  ARICENT GROUP
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_mif_msg_handler.c v0.1 2012/01/30 10:50:45 $
 ****************************************************************************
 *
 *  File Description :
 *      This file is the main entry point handler for RRM MgmtIfH Module. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrm_mif_msg_handler.c  $
 * Revision 0.2  2012/01/31 09:50:42 gur29990
 * Initial code
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *
 *   27 May,2014     gur29831    SPR-11366       UE Rel Fix.
 *   20 Jun,2014     gur34951    SPR 10329       Enhancement of GU GROUP ID
 *   13 June ,2014     gur29831    SPR-11838     Plmn id fix.
 *   18 June ,2014     gur29831   SPR-11963      rrm crash fixed.
 *   28 July,2014    gur30784    SPR-12796       cell state change ind for Rach
 ****************************************************************************/

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "rrm_stats.h"

/*! \headerfile rrm_mif_fsm.h <>
 */
#include <rrm_mif_fsm.h>
/*! \headerfile rrm_mif_db.h <>
 */
#include <rrm_mif_db.h>
/*! \headerfile rrm_mif_msg_handler.h <>
 */
#include "rrm_mif_msg_handler.h"
/*! \headerfile rrm_son_parser.h <>
 */
#include "rrm_son_parser.h"
/*! \headerfile rrm_son_composer.h <>
 */
#include "rrm_son_composer.h"
#include "rrm_utils.h"
#include "rrm_mem_utils.h"
#include "son_rrm_intf.h"
#include "son_rrm_parser.h"
/* Event handler header file */
#include "rrm_event_defines.h"
#include "rrm_oam_platform_parser.h"
/* Variable storing the SMIF name for logging */
S8 *p_g_rrm_mif_facility_name = (S8 *)"RRM_MIF";
/* Global variable storing the status (Enabled/Disabled) of RRM MIF logging */
/* coverity id 16530 , ,rel 2.0.1 */
rrm_oam_log_on_off_et g_mif_log_on_off = RRM_OAM_LOG_ON;

/* SPS related changes start */
rrm_bool_et is_sps_activated = RRM_FALSE;
/* SPS related changes end */

/*! \fn mif_raise_event_to_fsm 
 *  \brief  Function to pass messages from OAM 
 *  \param  p_mif_context store the global Ctxt
 *  \param  p_gl_ctxt store the global Ctxt
 *  \param  ret_val   store the return value
 */
rrm_return_et
rrm_son_send_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
  son_apps_rrm_carrier_freq_and_dl_power_params_req_t  *p_son_apps_rrm_carrier_freq_and_dl_power_params_req,           /* Pointer to API specific information. */
  U16                 src_module_id,  /* Source module identifier */
  U16                 dest_module_id,  /* Destination module identifier */
  U16                 transaction_id  /* Interface transaction identifier */
);

rrm_return_et
rrm_parse_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
  son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp,
  U8  *p_src,
  S32 length_left,
  S32 *p_length_read
);
/*SPR 10329:start*/
rrm_void_t 
build_and_send_son_enb_config_update_ind_to_cellm
(
    rrm_son_enb_config_update_ind_t *p_in_enb_config_update_ind

);

/*SPR 10329:end*/
static void 
mif_raise_event_to_fsm (rrm_mif_context       *p_mif_context,
        rrm_mif_gb_context_t  *p_gl_ctxt,  
        YLNODE                *p_ret_val_node);

/*! \fn mif_validate_stack_api_hdr 
 *  \brief  Function to validate STACKAPI header 
 *  \param  p_api     store the API Context
 *  \param  api_hdr   store the Header
 */
static void 
mif_validate_stack_api_hdr(void              *p_api, 
        STACKAPIHDR *stack_api_hdr);

/*! \fn mif_validate_api_hdr 
 *  \brief  Function to validate header 
 *  \param  p_api     store the API Context
 *  \param  api_hdr   store the Header
 */
static void 
mif_validate_api_hdr(void                 *p_api, 
        interface_api_header *api_hdr);

/*! \fn mif_oam_message_parser 
 *  \brief  Function to pass messages from OAM 
 *  \param  p_gl_ctxt store the global Ctxt
 *  \param  p_api     store the API Context
 *  \param  api_hdr   store the Header
 *  \param  ret_val   store the return value
 */
static rrm_mif_context* 
mif_oam_message_parser(rrm_mif_gb_context_t   *p_gl_ctxt, 
        void                   *p_api,
        interface_api_header   *api_hdr, 
        mif_return_value       *ret_val);

/*! \fn mif_son_message_parser 
 *  \brief  Function to pass messages from SON 
 *  \param  p_gl_ctxt store the global Ctxt
 *  \param  p_api     store the API Context
 *  \param  api_hdr   store the Header
 *  \param  ret_val   store the return value
 */
static rrm_mif_context* 
mif_son_message_parser (rrm_mif_gb_context_t   *p_gl_ctxt, 
        void                   *p_api,
        interface_api_header   *api_hdr, 
        mif_return_value       *ret_val);

/*! \fn mif_uem_message_parser 
 *  \brief  Function to pass messages from UEM 
 *  \param  p_gl_ctxt store the global Ctxt
 *  \param  p_api     store the API Context
 *  \param  p_stack_api_hdr   store the Header
 *  \param  ret_val   store the return value
 */
static rrm_mif_context* 
mif_uem_message_parser (rrm_mif_gb_context_t   *p_gl_ctxt, 
        void                   *p_api,
        STACKAPIHDR            *p_stack_api_hdr,
        mif_return_value       *ret_val);

/*! \fn mif_cellm_message_parser 
 *  \brief  Function to pass messages from CELLM 
 *  \param  p_gl_ctxt store the global Ctxt
 *  \param  p_api     store the API Context
 *  \param  p_api_hdr   store the Header
 *  \param  ret_val   store the return value
 */
static rrm_mif_context* 
mif_cellm_message_parser (rrm_mif_gb_context_t   *p_gl_ctxt, 
        void                   *p_api,
        STACKAPIHDR            *p_stack_api_hdr,
        mif_return_value       *ret_val);

/*! \fn mif_message_parser 
 *  \brief  Function to pass messages  
 *  \param  p_gl_ctxt store the global Ctxt
 *  \param  p_api     store the API Context
 *  \param  api_hdr   store the Interface Header
 *  \param  stack_api_hdr   store the STACKAPI Header
 *  \param  ret_val   store the return value
 */
static rrm_mif_context* 
mif_message_parser(rrm_mif_gb_context_t   *p_gl_ctxt, 
        void                   *p_api,
        interface_api_header   *api_hdr, 
        STACKAPIHDR            *p_stack_api_hdr,
        mif_return_value       *ret_val);
const S8 * MIF_SUPER_STATE []=
{
    (const S8 *)"RMIF_STATE_IDLE",                                    /*!< Idle state */
    (const S8 *)"RMIF_STATE_INIT",                                    /*!< Initialized state */
    (const S8 *)"RMIF_STATE_INIT_CONFIG_ONGOING",                     /*!< Init Config Ongoing */
    (const S8 *)"RMIF_STATE_ACTIVE",                                  /*!< Active state */
    (const S8 *)"RMIF_STATE_SHUTDOWN_ONGOING",                        /*!< Shutdown in progress */
    (const S8 *)"RMIF_STATE_RESUME_ONGOING"                          /*!< Resume in progress */

};

/* function prototype of event raise function */
void rrm_raise_event_cell_setup_reject(rrm_cell_setup_fail_cause_et fail_cause);

rrm_return_et parse_and_store_rrm_oam_cfg (U8 *fileName, rrm_mif_gb_context_t *p_gl_ctxt);


/* SPR_13117_FIX_START */
/************************************************************************
 * Function Name  : rrm_mif_oam_process_get_log_level_req
 * Inputs         : pointer to the message received
 *                  pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RRM_OAM_GET_LOG_LEVEL_REQ msg
 *************************************************************************/
static void rrm_mif_oam_process_get_log_level_req (
        void                   *p_msg,
        rrm_mif_gb_context_t   *p_gl_ctxt,
        U16                    trans_id)
{
    S32 length_read    = RRM_ZERO;
    rrm_oam_get_log_level_req_t    p_in_message = {RRM_ZERO};
    rrm_oam_get_log_level_resp_t   p_out_message = {RRM_ZERO};
    rrm_return_et                  ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_msg);
    RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

    if (p_gl_ctxt->current_req_for_all_modules == RRM_FALSE)
    {
        if (RMIF_STATE_ACTIVE == p_gl_ctxt->current_state)
        {
            if (RRM_SUCCESS != rrm_parse_rrm_oam_get_log_level_req(
              &p_in_message,
              (U8*)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
              rrm_get_api_buf_size(p_msg) -
              RRM_INTERFACE_API_HEADER_SIZE,
              (void *)&length_read))
             
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                        "OAM->MIF:RRM_OAM_GET_LOG_LEVEL_REQ parsing failed");
            }
            else
            {
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
                        "OAM->MIF:RRM_OAM_GET_LOG_LEVEL_REQ successfully parsed");
                if (RRM_OAM_MIF_MODULE_ID == p_in_message.module_id)
                {
                    p_out_message.fail_cause = RRM_NO_ERROR;
                    p_out_message.num_of_modules = RRM_ONE;
                    p_out_message.module_id[RRM_ZERO].module_id = p_in_message.module_id;
                    if (RRM_OAM_LOG_OFF == p_gl_ctxt->log_enabled)
                    {
                        p_out_message.response = RRM_FAILURE;
                        p_out_message.module_id[RRM_ZERO].error_code = RRM_ERR_LOGGING_DISABLED;
                        p_out_message.module_id[RRM_ZERO].bitmask = RRM_OAM_ZERO;
                    }
                    else
                    {
                        p_out_message.response = RRM_SUCCESS;
                        p_out_message.module_id[RRM_ZERO].error_code = RRM_NO_ERROR;
                        p_out_message.module_id[RRM_ZERO].bitmask |= RRM_OAM_MODULE_LOG_LEVEL_PRESENT;
                        p_out_message.module_id[RRM_ZERO].log_level = p_gl_ctxt->log_level;
                    }
                    ret_val = rrm_oam_send_rrm_oam_get_log_level_resp(
                      (void *)&p_out_message,
                      RRM_MIF_MODULE_ID,
                      RRM_OAM_MODULE_ID,
                      trans_id,
                      RRM_ZERO);
                     
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_get_log_level_resp to OAM");
                    if(RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                    }
                }
                else if ((RRM_ALL_MODULE_ID == p_in_message.module_id) || (RRM_OAM_CM_MODULE_ID == p_in_message.module_id) || (RRM_OAM_UEM_MODULE_ID == p_in_message.module_id))
                {
                    rrm_build_and_send_get_log_level_req_to_cellm_uem(p_in_message.module_id,trans_id);
                    p_gl_ctxt->current_req_for_all_modules = RRM_TRUE;
                }
                else
                {
                    p_out_message.response = RRM_FAILURE;
                    p_out_message.fail_cause = RRM_ERR_INVALID_PARAMS;
                    p_out_message.num_of_modules = RRM_ZERO;
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name,
                            RRM_ERROR, "Invalid module id[%d] is received.", p_in_message.module_id);
                    
                       ret_val = rrm_oam_send_rrm_oam_get_log_level_resp(
                       (void *)&p_out_message,
                       RRM_OAM_MODULE_ID,
                       RRM_MIF_MODULE_ID,
                       trans_id,
                       RRM_ZERO);
                     
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_get_log_level_resp to OAM");
                    if(RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                    }
                }
            }
        }
        else
        {
            p_out_message.response = RRM_FAILURE;
            p_out_message.fail_cause = RRM_ERR_UNEXPECTED_MSG;
            p_out_message.num_of_modules = RRM_ZERO;
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name,
                    RRM_ERROR, "Request recived in invalid state. current state = [%d]",p_gl_ctxt->current_state);
            
               ret_val = rrm_oam_send_rrm_oam_get_log_level_resp(
               (void *)&p_out_message,
               RRM_MIF_MODULE_ID,
               RRM_OAM_MODULE_ID,
               trans_id,
               RRM_ZERO);
             
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_get_log_level_resp to OAM");
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
            }
        }
    }
    else
    {
        p_out_message.response = RRM_FAILURE;
        p_out_message.fail_cause = RRM_ERR_REQ_ALREADY_IN_PROGRESS;
        p_out_message.num_of_modules = RRM_ZERO;
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name,
                RRM_ERROR, "Resend same API, get_log_level api is proceesing");
        
           ret_val = rrm_oam_send_rrm_oam_get_log_level_resp(
           (void *)&p_out_message,
           RRM_MIF_MODULE_ID,
           RRM_OAM_MODULE_ID,
           trans_id,
           RRM_ZERO);
         
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_get_log_level_resp to OAM");
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
        }
    }
    RRM_UT_TRACE_EXIT();
}
/* SPR_13117_FIX_END */

/************************************************************************
 * Function Name  : rrm_mif_oam_process_set_log_level_req 
 * Inputs         : pointer to the message received
 *                  pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RRM_OAM_SET_LOG_LEVEL_REQ msg
 *************************************************************************/
static void rrm_mif_oam_process_set_log_level_req (
        void                   *p_msg, 
        rrm_mif_gb_context_t   *p_gl_ctxt,
        U16                    trans_id)
{
  S32 length_read    = RRM_ZERO;

  /* coverity id 16526 , ,rel 2.0.1 */
  rrm_oam_set_log_level_req_t    in_message = {RRM_ALL_MODULE_ID,RRM_OAM_NONE};
  /* coverity id 16527 , ,rel 2.0.1 */
  rrm_oam_set_log_level_resp_t   out_message = {RRM_INDEX_ERR,RRM_NO_ERROR};
  rrm_return_et                  ret_val = RRM_SUCCESS;

  RRM_UT_TRACE_ENTER();

  RRM_ASSERT(RRM_PNULL != p_msg);
  RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

  if (RRM_SUCCESS != rrm_parse_rrm_oam_set_log_level_req(
	(void *)&in_message,
	(U8*)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
	rrm_get_api_buf_size(p_msg) - 
	RRM_INTERFACE_API_HEADER_SIZE,
	(void *)&length_read))
  {
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
              "OAM->MIF:RRM_OAM_SET_LOG_LEVEL_REQ parsing failed");
    ret_val = RRM_FAILURE;
  }
  /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/
  else 
  /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
  {
      RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
              "OAM->MIF:RRM_OAM_SET_LOG_LEVEL_REQ successfully parsed");
    p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;

    /*LTE_RRM_KLOCWORK_WARN_24_JULY_START*/
    /* SPR_6488 FIX START */
    if ( RRM_ALL_MODULE_ID == in_message.module_id )/*LTE_RRM_KLOCWORK_WARN_24_JULY_END*/
    /* SPR_6488 FIX END */
    {
        /* Send request to all modules */
        p_gl_ctxt->current_req_for_all_modules = RRM_TRUE;
        p_gl_ctxt->module_resp = RRM_ZERO;
        /* SPR_13117_FIX_START */
        p_gl_ctxt->log_level = in_message.log_level;
        /* SPR_13117_FIX_END */
        /* SPR_6488 FIX START */
        /* Change the log level at MIF as per incoming request */
        if ((U32)(RRM_TRUE) == (U32)(g_mif_log_on_off))
        {   
            SET_MODULE_LOG_LEVEL(in_message.log_level);
        }

        /* Change the log level at CM and UEM as per incoming request */
        build_and_send_set_log_level_req(/*SPR 17777 +-*/ 
                &in_message,
                trans_id);
        /* SPR_6488 FIX END */
    }
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/
    else if (RRM_OAM_MIF_MODULE_ID == in_message.module_id)
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
    {
        /* SPR_13117_FIX_START */
        p_gl_ctxt->log_level = in_message.log_level;
        /* SPR_13117_FIX_END */
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                RRM_DETAILED, "rrm_oam_set_log_level_req_t for MIF");

        if ((U32)(RRM_TRUE) == (U32)(g_mif_log_on_off))
        {   
            SET_MODULE_LOG_LEVEL(in_message.log_level);
        }

        /* SPR_6488 FIX START */
        out_message.response = RRM_SUCCESS;
        out_message.fail_cause = RRM_NO_ERROR;
        if(RRM_FAILURE == rrm_oam_send_rrm_oam_set_log_level_resp((void *)&out_message,
                    RRM_MIF_MODULE_ID,
                    RRM_OAM_MODULE_ID,
                    trans_id,
                    RRM_ZERO))
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                    "Msg return_src_id_from_tcb Failed");
        }
        /* SPR_6488 FIX END */
    }
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/ 
    else if ((RRM_OAM_CM_MODULE_ID == in_message.module_id)  ||
	     (RRM_OAM_UEM_MODULE_ID == in_message.module_id)) 
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                RRM_DETAILED, "build_and_send_set_log_level_req for Cellm/UEM");
        build_and_send_set_log_level_req(/*SPR 17777 +-*/
                &in_message,
                trans_id);
    }
    else
    {
           /* SPR_6488 FIX START */
           RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
   	                 RRM_BRIEF, "Invalid Module Id[%d] recieved ", in_message.module_id);
           ret_val = RRM_FAILURE;
           /* SPR_6488 FIX END */
    }
  }

  if (RRM_FAILURE == ret_val)
  {
	  out_message.response = RRM_FAILURE;
      /* SPR_6488 FIX START */
	  out_message.fail_cause = RRM_ERR_INVALID_PARAMS;
      /* SPR_6488 FIX END */
	  p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
	  /*_start CID 11474*/

	  if(RRM_FAILURE==rrm_oam_send_rrm_oam_set_log_level_resp((void *)&out_message,
				  RRM_MIF_MODULE_ID,
                  RRM_OAM_MODULE_ID,
				  trans_id,
                  RRM_ZERO))
	  { RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
			  "Msg return_src_id_from_tcb Failed");    
	  }

	  /*_END*/
  }
  RRM_UT_TRACE_EXIT();
}

/************************************************************************
 * Function Name  : rrm_mif_oam_process_get_ver_id_req
 * Inputs         : transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RRM_OAM_GET_VER_ID_REQ msg
 *************************************************************************/
static void rrm_mif_oam_process_get_ver_id_req(U16  trans_id)
{
    /* coverity id 21801 , ,rel 2.0.1 */
    rrm_oam_get_ver_id_resp_t   out_message = {RRM_INDEX_ERR,{RRM_ZERO}};

    RRM_UT_TRACE_ENTER();
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED,
                "filling get version id response message");

    out_message.response = RRM_SUCCESS;
    /*coverity 19992, rel2.0.1, , strcpy changed to strncpy*/
    RRM_STRNCPY(out_message.ver_id,RRM_VER_ID, sizeof(out_message.ver_id));
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED,
              "Version Id [%s]", out_message.ver_id);

    /*send get version id resp message to oam */	
    rrm_oam_send_rrm_oam_get_ver_id_resp((void *)&out_message,
                                         RRM_MIF_MODULE_ID,
                                         RRM_OAM_MODULE_ID,
                                         trans_id,
                                         RRM_ZERO);
    RRM_UT_TRACE_EXIT();
}

static void
build_and_send_tnl_discovery_resp_to_son
(
    rrmcm_rmif_son_tnl_discovery_resp_t *p_tnl_discovery_resp,
    U16 tr_id
)
{
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_tnl_discovery_resp);

    rrm_son_tnl_discovery_res_t *p_rrm_son_tnl_discovery_res = {RRM_PNULL};
    p_rrm_son_tnl_discovery_res = (rrm_son_tnl_discovery_res_t *)rrm_mem_get(sizeof(rrm_son_tnl_discovery_res_t)); 
    if ( p_rrm_son_tnl_discovery_res == RRM_PNULL)
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_rrm_son_tnl_discovery_res failed" );
        RRM_UT_TRACE_EXIT();
        return ;
    }    
    /*  Coverity, CID 16296 */
    /*  end */

    rrm_memcpy_rrm_son_tnl_discovery_res((p_rrm_son_tnl_discovery_res),
                             &(p_tnl_discovery_resp->tnl_discovery_resp));
    
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
            RRM_BRIEF,
            " sending TNL discovery to son module");

    /*Sending msg to the SON module*/
    rrm_son_send_rrm_son_tnl_discovery_res(p_rrm_son_tnl_discovery_res,
            RRM_MIF_MODULE_ID,
            RRM_SON_ANR_MODULE_ID,
            tr_id,
            RRM_ZERO);
    
    RRM_MEM_FREE(p_rrm_son_tnl_discovery_res);
    RRM_UT_TRACE_EXIT();
}        

/*START OF UE RELEASE FROM OAM TO RRM */
/****************************************************************************
 * Function Name  : rrm_mif_oam_process_ue_release_req
 * Inputs         : 
 *                  -p_msg - It's not null pointer.
 *                  -tr_id - Transaction Id.
 * Outputs        : None
 * Returns        : void
 * Description    : This function handles ue release req from OAM
 ****************************************************************************/
static void rrm_mif_oam_process_ue_release_req(void *p_msg)/*SPR 17777 +-*/
{
	S32  length_read = RRM_ZERO;
	rrm_ue_index_t  ue_index=RRM_ZERO;
	/*SPR_11366_FIX_START*/
	rrm_cell_index_t cell_index            = RRM_ZERO;
	/*SPR_11366_FIX_END*/

	RRM_UT_TRACE_ENTER();
	rrm_oam_ue_release_req_t rrm_oam_ue_release_req;
    rrm_memset_rrm_oam_ue_release_req(&rrm_oam_ue_release_req,RRM_ZERO);
	if (RRM_SUCCESS != rrm_parse_rrm_oam_ue_release_req(
				(void *)&rrm_oam_ue_release_req,
				(U8*)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
				rrm_get_api_buf_size(p_msg) -
				RRM_INTERFACE_API_HEADER_SIZE,
				(void *)&length_read))
	{
		RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
				"Msg rrm_parse_rrm_oam_ue_release_req"
				"Parsing Failed");
	}
	else
	{   
		/*SPR_11366_FIX_START*/
		cell_index = rrm_get_cell_id_from_header((U8*)p_msg + RRM_API_HEADER_SIZE);
		ue_index=rrm_oam_ue_release_req.ue_index;
		RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF,
						"Ue Release req for cell index : %d , [UE:%d] "
						, cell_index , ue_index);
		build_and_send_ue_release_req(ue_index ,/*SPR 17777 +-*/ 
	            cell_index);
		/*SPR_11366_FIX_END*/
	}
	RRM_UT_TRACE_EXIT();
}

/* END OF UE RELEASE FROM OAM TO RRM */

/****************************************************************************
 * Function Name  : rrm_oam_fill_event_notification_ind_msg
 * Inputs         :
 *                  p_event_notif_to_oam:  Notification msg to OAM
 *                  p_event_ind: Evnet notification from event handler
 * Returns        : void
 * Description    : This function fills the event notification message.
 ****************************************************************************/
void rrm_oam_fill_event_notification_ind_msg( 
        rrm_oam_event_notification_t     *p_event_notif_to_oam,  
        rrmuem_rmif_event_notification_ind_t   *p_event_ind)
{
  U8 payload_count = RRM_ZERO;
  
  RRM_UT_TRACE_ENTER();

  RRM_ASSERT (RRM_PNULL != p_event_notif_to_oam);
  RRM_ASSERT (RRM_PNULL != p_event_ind);

  /* Filling Header */ 
  p_event_notif_to_oam->msg_header.time_stamp.year = p_event_ind->header.time_stamp.year;
  p_event_notif_to_oam->msg_header.time_stamp.month = p_event_ind->header.time_stamp.month;
  p_event_notif_to_oam->msg_header.time_stamp.day = p_event_ind->header.time_stamp.day;
  p_event_notif_to_oam->msg_header.time_stamp.hour = p_event_ind->header.time_stamp.hour;
  p_event_notif_to_oam->msg_header.time_stamp.min = p_event_ind->header.time_stamp.min;
  p_event_notif_to_oam->msg_header.time_stamp.sec = p_event_ind->header.time_stamp.sec;

  p_event_notif_to_oam->msg_header.event_type = p_event_ind->header.event_type;
  p_event_notif_to_oam->msg_header.event_sub_type = p_event_ind->header.event_sub_type;
  p_event_notif_to_oam->msg_header.event_id = p_event_ind->header.event_id;  
  
  /* Filling Payload*/
  if(RRM_MIF_EVENT_DATA_PRESENT == (p_event_ind->bitmask & RRM_MIF_EVENT_DATA_PRESENT))
  {
     RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
               RRM_DETAILED, "RRM_MIF_EVENT_DATA_PRESENT");

    p_event_notif_to_oam->bitmask |= RRM_OAM_API_DATA_PRESENT;
    for (payload_count=RRM_ZERO; ((payload_count<RRM_OAM_MAX_EVENT_LEN)&& 
        (payload_count<MAX_BUFFER)); ++payload_count) 
    {
      p_event_notif_to_oam->api_data[payload_count]= 
                        p_event_ind->buffer[payload_count];
    }
  }
 
   RRM_UT_TRACE_EXIT();
}

/*********************************************************************
 * Function Name  : rrm_mif_oam_process_log_enable_req 
 * Inputs         : pointer to the message received
 *                  pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RRM_OAM_LOG_ENABLE_REQ msg
 ***********************************************************************/
static void rrm_mif_oam_process_log_enable_req(
        void                   *p_api,
        rrm_mif_gb_context_t   *p_gl_ctxt,
        U16                     trans_id)
{
  S32           length_read = RRM_ZERO;
  /* coverity id 16524 , ,rel 2.0.1 */
  /* coverity : CID 31033 */
  rrm_oam_log_enable_disable_req_t   in_message = {RRM_ALL_MODULE_ID,{RRM_ZERO}};
  /* coverity id 16525 , ,rel 2.0.1 */
  rrm_oam_log_enable_disable_resp_t  out_message = {RRM_INDEX_ERR,RRM_NO_ERROR};
  rrm_return_et                      ret_val = RRM_SUCCESS;

  RRM_UT_TRACE_ENTER();

  RRM_ASSERT(RRM_PNULL != p_api);
  RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

  if (RRM_SUCCESS != rrm_parse_rrm_oam_log_enable_disable_req(
	(void *)&in_message,
	(U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
	rrm_get_api_buf_size(p_api) - 
	RRM_INTERFACE_API_HEADER_SIZE,
	(void *)&length_read))
  {
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
              "OAM->MIF:RRM_OAM_LOG_ENABLE_DISABLE_REQ parsing failed");
    ret_val = RRM_FAILURE;
  }
  /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/ 
  else
  /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
  {
      RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
              "OAM->MIF:RRM_OAM_LOG_ENABLE_DISABLE_REQ successfully parsed");
    p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;

    /*LTE_RRM_KLOCWORK_WARN_24_JULY_START*/
    /* SPR_6488 FIX START */
    if ( RRM_ALL_MODULE_ID == in_message.module_id ) /*LTE_RRM_KLOCWORK_WARN_24_JULY_END*/
    /* SPR_6488 FIX START */
    {
        /* Log Enable Disable Request to be sent to all modules */
        p_gl_ctxt->current_req_for_all_modules = RRM_TRUE;
        p_gl_ctxt->module_resp = RRM_ZERO;

        /* SPR_6488 FIX START */
        /* Enable / DIsable log level request at MIF as per incoming request */
        rrm_mif_log_enable_disable_cmd(p_gl_ctxt, 
                in_message.log_config.log_on_off, 
                in_message.log_config.log_level);

        /* Enable / DIsable log level request at CM and UEM as per incoming request */
        build_and_send_log_enable_disable_req(/*SPR 17777 +-*/
                &in_message,
                trans_id);
        /* SPR_6488 FIX END */
    }
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/
    else if (RRM_OAM_MIF_MODULE_ID == in_message.module_id)
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                "rrm_oam_log_enable_disable_req_t for MIF");
        rrm_mif_log_enable_disable_cmd(p_gl_ctxt, 
                in_message.log_config.log_on_off, 
                in_message.log_config.log_level);

        /* SPR_6488 FIX START */
        out_message.response = RRM_SUCCESS;
        out_message.fail_cause = RRM_NO_ERROR;
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                "Sending log_enable_disable_resp to OAM");
        ret_val = rrm_oam_send_rrm_oam_log_enable_disable_resp((void *)&out_message,
                RRM_MIF_MODULE_ID,
                RRM_OAM_MODULE_ID,
                trans_id,
                RRM_ZERO);
        /* SPR_6488 FIX END */
    }
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/
    else if ((RRM_OAM_CM_MODULE_ID == in_message.module_id)  ||
	         (RRM_OAM_UEM_MODULE_ID == in_message.module_id)) 
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
    {
      RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                "build_and_send_log_enable_disable_req for Cellm/UEM");
      build_and_send_log_enable_disable_req(/*SPR 17777 +-*/
        	                            &in_message,
	                                    trans_id);
    }
    else
    {
        /* SPR_6488 FIX START */
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
   	                 RRM_BRIEF, "Invalid Module Id[%d] recieved ", in_message.module_id);
        ret_val = RRM_FAILURE;
        /* SPR_6488 FIX END */
    }
  }

  if (RRM_FAILURE == ret_val)
  {
    out_message.response = RRM_FAILURE;
    out_message.fail_cause = RRM_ERR_INVALID_PARAMS;
    p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_START*/
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
              "Sending log_enable_disable_resp to OAM");
    ret_val = rrm_oam_send_rrm_oam_log_enable_disable_resp((void *)&out_message,
                                                           RRM_MIF_MODULE_ID,
                                                           RRM_OAM_MODULE_ID,
                                                           trans_id,
                                                           RRM_ZERO);
    /*Coverity ID 78685 Fix Start*/
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "Send Failed log_enable_disable_resp");
    }
    /*Coverity ID 78685 Fix End*/
    /*LTE_RRM_KLOCWORK_WARN_6_JUL_END*/
  }


  RRM_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : rrm_mif_process_son_nmm_prepare_request
 * Inputs         : 
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes an NNM PREPARE request
 *                  received from eNB SON. 
 ****************************************************************************/
static void rrm_mif_process_son_nmm_prepare_request(rrm_mif_gb_context_t *p_gl_ctxt, 
                                                    U16                   transaction_id,
                                                    U16                   source)
{
    rrm_error_et ret_val = RRM_NO_ERROR;
    /* coverity id 16529 rel 2.0.1 */
    rrm_son_nmm_prepare_res_t out_nmm_prepare_resp_to_son = {{RRM_INDEX_ERR,RRM_NO_ERROR}};
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_gl_ctxt);
    /* checking cell_stop_flag if true then cell stop
       required otherwise cell stop not required */
    /*SPR_15766 FIX START*/
    if (RRM_CELL_STARTED == p_gl_ctxt->cell_state)
        /*SPR_15766 FIX END*/
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "cell_stop_flag is true");

        /* Sending cell stop to all cell */
        ret_val = rrm_mif_send_event_to_all_cells(MIF_FSM_EVENT_CELL_STOP_REQ,
                RRM_PNULL,
                p_gl_ctxt,
                transaction_id,
                source);

        if (ret_val != RRM_NO_ERROR)
        {
            if (ret_val == RRM_ERR_CELL_UNCONFIGURED)
            {
                out_nmm_prepare_resp_to_son.prepare_res.result = RRM_SUCCESS;
                out_nmm_prepare_resp_to_son.prepare_res.error_code = RRM_NO_ERROR;
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "no cell configured");
#ifdef RRM_UT_FLAG
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_S);
#endif
            }
            else
            {
                out_nmm_prepare_resp_to_son.prepare_res.result = RRM_FAILURE;
                out_nmm_prepare_resp_to_son.prepare_res.error_code = ret_val;
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                        "error in sending cell stop event to all cell with fail cause[%d]",
                        out_nmm_prepare_resp_to_son.prepare_res.error_code);
#ifdef RRM_UT_FLAG
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_F);
#endif
            }

            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF,
                    "sending nmm prep response");
            rrm_son_send_rrm_son_nmm_prepare_res(&out_nmm_prepare_resp_to_son,
                    RRM_MIF_MODULE_ID,
                    RRM_SON_NMM_MODULE_ID,
                    transaction_id,
                    RRM_ZERO);
        }
    }
    /*SPR_15766 FIX START*/
    else if (RRM_CELL_STOPPED == p_gl_ctxt->cell_state
            || RRM_CELL_INIT == p_gl_ctxt->cell_state
            )
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF,
                "cell is not started sending nmm prep success resp to nmm module");
        out_nmm_prepare_resp_to_son.prepare_res.result = RRM_SUCCESS;
        out_nmm_prepare_resp_to_son.prepare_res.error_code = RRM_NO_ERROR;

#ifdef RRM_UT_FLAG
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_S);
#endif
        rrm_son_send_rrm_son_nmm_prepare_res(&out_nmm_prepare_resp_to_son,
                RRM_MIF_MODULE_ID,
                RRM_SON_NMM_MODULE_ID,
                transaction_id,
                RRM_ZERO);
    }
    else
    {
        out_nmm_prepare_resp_to_son.prepare_res.result = RRM_FAILURE;
        if(RRM_CELL_START_ONGOING == p_gl_ctxt->cell_state )
        {
            out_nmm_prepare_resp_to_son.prepare_res.error_code = RRM_ERR_CELL_START_ONGOING;
        }
        else if(RRM_CELL_STOP_ONGOING == p_gl_ctxt->cell_state )
        {
            out_nmm_prepare_resp_to_son.prepare_res.error_code = RRM_ERR_CELL_STOP_ONGOING;
        }
        else
        {
            out_nmm_prepare_resp_to_son.prepare_res.error_code = RRM_ERR_INTERNAL_FAILURE;
        }
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                "nmm_prepare failed,  cell_state[%d].", p_gl_ctxt->cell_state);
#ifdef RRM_UT_FLAG
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_S);
#endif
        rrm_son_send_rrm_son_nmm_prepare_res(&out_nmm_prepare_resp_to_son,
                RRM_MIF_MODULE_ID,
                RRM_SON_NMM_MODULE_ID,
                transaction_id,
                RRM_ZERO);
    }
    /*SPR_15766 FIX END*/

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_process_son_nmm_comp_request
 * Inputs         :
 *
 *
 * Outputs        : None
 * Returns        : void
 * Description    : This function processes an NNM COMPLETE request 
 *                  received from eNB SON.
 ****************************************************************************/
static void rrm_mif_process_son_nmm_comp_request(rrm_mif_gb_context_t  *p_gl_ctxt,
                                                 U16                    transaction_id, 
                                                 U16                    source)
{
    rrm_error_et ret_val = RRM_NO_ERROR;
    /* coverity id 16528 ,rel 2.0.1 */
    rrm_son_nmm_complete_res_t out_nmm_complete_resp_to_son = {{RRM_INDEX_ERR,RRM_NO_ERROR}};
    RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

    RRM_UT_TRACE_ENTER();
    /*SPR_15766 FIX START*/
    if (RRM_CELL_STOPPED == p_gl_ctxt->cell_state)
        /*SPR_15766 FIX END*/
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "cell_stop_flag is true");

        /* Sending cell start to all cell */
        ret_val =  rrm_mif_send_event_to_all_cells(MIF_FSM_EVENT_CELL_START_REQ,
                RRM_PNULL,
                p_gl_ctxt,
                transaction_id,
                source);

        if (RRM_NO_ERROR != ret_val)
        {
            if (RRM_ERR_CELL_UNCONFIGURED == ret_val)
            {
                out_nmm_complete_resp_to_son.complete_res.result = RRM_SUCCESS;
                out_nmm_complete_resp_to_son.complete_res.error_code = RRM_NO_ERROR;
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "no cell configured");
#ifdef RRM_UT_FLAG
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_complete_resp_S);
#endif
            }
            else
            {
                out_nmm_complete_resp_to_son.complete_res.result = RRM_FAILURE;
                out_nmm_complete_resp_to_son.complete_res.error_code = ret_val;
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                        "error in sending cell start event to all cell with fail cause[%d]",
                        out_nmm_complete_resp_to_son.complete_res.error_code);
#ifdef RRM_UT_FLAG
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_complete_resp_F);
#endif
            }

            rrm_son_send_rrm_son_nmm_complete_res(&out_nmm_complete_resp_to_son,
                    RRM_MIF_MODULE_ID,
                    RRM_SON_NMM_MODULE_ID,
                    transaction_id,
                    RRM_ZERO);
        }
    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "cell_stop_flag is false & sending nmm complete success response");
        out_nmm_complete_resp_to_son.complete_res.result = RRM_SUCCESS;
        out_nmm_complete_resp_to_son.complete_res.error_code = RRM_NO_ERROR;
#ifdef RRM_UT_FLAG
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_complete_resp_S);
#endif
        rrm_son_send_rrm_son_nmm_complete_res(&out_nmm_complete_resp_to_son,
                RRM_MIF_MODULE_ID,
                RRM_SON_NMM_MODULE_ID,
                transaction_id,
                RRM_ZERO);
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_process_init_ind_timer_expiry
 * Inputs         : p_gl_ctxt : global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes an init indication timer expiry
 *                  event. It sends an init indication message to OAM and
 *                  restarts the timer.
 ****************************************************************************/
static void rrm_mif_process_init_ind_timer_expiry
(
 rrm_mif_gb_context_t*  p_gl_ctxt
 )
{
    QTIMER                       mif_timer_id = RRM_PNULL;
  /* coverity id 16510 , ,rel 2.0.1 */
    mif_timer_buf                o_mif_timer_buf = {MIF_TIME_TO_SHUTDOWN_TIMER,RRM_ZERO,NULL_FSM,RRM_ZERO,RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT( RRM_PNULL != p_gl_ctxt );

    /* Reset the running timer id */
    p_gl_ctxt->timer_id[MIF_INIT_INDICATION_TIMER] = RRM_PNULL;

    /* Send another init indication to OAM */
    build_and_send_init_ind_to_oam();

    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "OAM Init Sent Successfully");

    /* Restart the INIT_IND timer */

    o_mif_timer_buf.timer_type = MIF_INIT_INDICATION_TIMER;

    RRM_ASSERT (MIF_INIT_INDICATION_TIMER <= MIF_MAX_NUM_TIMER_TYPE);
    mif_timer_id = mif_start_timer(MIF_INIT_INDICATION_TIMER,
            (void *)&o_mif_timer_buf, 
            sizeof(mif_timer_buf),
            p_gl_ctxt);

    /* Save the timer id in global context */
    p_gl_ctxt->timer_id[MIF_INIT_INDICATION_TIMER] = mif_timer_id; 

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_send_event_to_particular_cells
 * Inputs         : event_id 
 *                  p_msg_info : incoming (parsed) message buffer
 *                  p_gl_ctxt  : global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function loops through the saved mif contexts
 *                  and sends the requested event to all active cells.
 ****************************************************************************/
rrm_error_et rrm_mif_send_event_to_particular_cells
(
 U16                   event_id, 
 U8                   *p_msg_info, 
 rrm_mif_gb_context_t *p_gl_ctxt,
 U16                   transaction_id,
 U16                   source,
 U8                    store_mif_ctxt[RRM_MAX_NUM_CELLS]
 )
{
    U8                                     mif_ctxt_cnt     = RRM_ZERO;
    U8                                     mif_ctxt_id      = RRM_ZERO;
    U8                                     match_ctxt_id = RRM_ZERO; 
    YLNODE                                *p_node           = RRM_PNULL;
    mif_return_value                      *p_mif_return     = RRM_PNULL;
    rrm_mif_context                       *p_mif_context    = RRM_PNULL;
    rrm_error_et                           return_value     = RRM_ERR_CELL_UNCONFIGURED;
    rrm_return_et                          ret_val          = RRM_FAILURE;
    /*ECN start*/
    rrm_oam_cell_ecn_capacity_enhance_req_t *p_in_cell_ecn_capa_enhance_req=RRM_PNULL; 
    rrm_ecn_configure_cell_list_t           *p_ecn_for_cell = RRM_PNULL;
    rrm_plat_cell_load_t                    *p_cell_load = RRM_PNULL;
    platform_rrm_load_ind_t                 *p_plat_oam_load = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (p_gl_ctxt != RRM_PNULL);
    switch (event_id) 
    {
        case MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ:

            p_in_cell_ecn_capa_enhance_req=
                (rrm_oam_cell_ecn_capacity_enhance_req_t *)p_msg_info; 
            break;
        case MIF_FSM_EVENT_CELL_PLATFORM_IND:
            p_plat_oam_load = (platform_rrm_load_ind_t *)p_msg_info;
            break;
        default:
            break;
    }
    /* Loop through all valid cells and trigger routine for
       each one them them */
    for (mif_ctxt_cnt = RRM_ZERO; mif_ctxt_cnt < MAX_NUM_MIF_CTXT; mif_ctxt_cnt++)
    {
        if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
        {
            mif_ctxt_id = p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
            for(match_ctxt_id = RRM_ZERO; match_ctxt_id < RRM_MAX_NUM_CELLS; match_ctxt_id++)
            {
                /* SPR 10869 fix start*/
                if (store_mif_ctxt[match_ctxt_id]== mif_ctxt_id)
                    /* SPR 10869 fix end */
                {

                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                            "Processing for mif_ctxt_id: %d, "
                            "cell_index: %d",
                            mif_ctxt_id, 
                            p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index);

                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
                    if (p_mif_context != RRM_PNULL)
                    {
                        /* mif context found in pool */
                        ret_val = allocate_and_store_tcb_info_in_mif_context(
                                p_mif_context,
                                /* SPR_19279_START */
                                RRM_NULL,
                                RRM_ZERO,
                                /* SPR_19279_END */
                                transaction_id,
                                source);
                        if(event_id == MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ)
                        {
                            p_ecn_for_cell= (rrm_ecn_configure_cell_list_t *)
                                rrm_mem_get(sizeof(rrm_ecn_configure_cell_list_t));
                            if ( p_ecn_for_cell == RRM_PNULL)
                            {
                                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                        "Memory allocation to p_ecn_for_cell failed" );
                                /* BUG_11576_FIX_START */
                                /*SPR 15993 Fix Start*/
                                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, 
                                        p_mif_context->currently_processed_api_trans_d);
                                /*SPR 15993 Fix End*/
                                if(RRM_FAILURE != ret_val)
                                {
                                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                            "TCB De-Allocated successfully");
                                }
                                /* BUG_11576_FIX_END */
                                RRM_UT_TRACE_EXIT();
                                /*Coverity fix 58744 start*/
                                return RRM_ERR_SYS_MEM_ALLOC_FAILURE;
                                /*Coverity fix 58744 end*/
                            }                            
                            /* SPR 10869 fix start*/    
                            RRM_MEMCPY(p_ecn_for_cell,
                                    &(p_in_cell_ecn_capa_enhance_req->ecn_cells[match_ctxt_id]),
                                    sizeof(rrm_ecn_configure_cell_list_t));
                            /* SPR 10869 fix end */
                        }
                        else if(event_id == MIF_FSM_EVENT_CELL_PLATFORM_IND)
                        {
                            p_cell_load = (rrm_plat_cell_load_t *)rrm_mem_get(sizeof(rrm_plat_cell_load_t));
                            if ( p_cell_load == RRM_PNULL)
                            {
                                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                        "Memory allocation to p_cell_load failed" );
                                /* BUG_11576_FIX_START */
                                /*SPR 15993 Fix Start*/
                                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                                        p_mif_context->currently_processed_api_trans_d);
                                /*SPR 15993 Fix End*/
                                if(RRM_FAILURE != ret_val)
                                {
                                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                            "TCB De-Allocated successfully");
                                }
                                /* BUG_11576_FIX_END */
                                RRM_UT_TRACE_EXIT();
                                /*Coverity fix 58744 start*/
                                return RRM_ERR_SYS_MEM_ALLOC_FAILURE;
                                /*Coverity fix 58744 end*/
                            }

                            RRM_MEMCPY(p_cell_load,&p_plat_oam_load->cell_load, sizeof(rrm_plat_cell_load_t));
                        }
                        if (RRM_SUCCESS == ret_val)
                        {
                            /* Trigger MIF FSM for this cell */
                            p_node = (YLNODE *)rrm_mem_get(sizeof(mif_return_value));
                            if ( p_node == RRM_PNULL)
                            {
                                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                        "Memory allocation to p_node failed" );
                                RRM_UT_TRACE_EXIT();
                                /* COVERITY : RESOURCE LEAK : CID : 54401,54402 fix start */
                                if( RRM_NULL != p_ecn_for_cell )
                                    RRM_MEM_FREE(p_ecn_for_cell);
                                if( RRM_NULL != p_cell_load )
                                    RRM_MEM_FREE(p_cell_load);
                                /* COVERITY : RESOURCE LEAK : CID : 54401,54002 fix end*/
                                /*SPR 15993 Fix Start*/
                                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                                        p_mif_context->currently_processed_api_trans_d);
                                /*SPR 15993 Fix End*/
                                if(RRM_FAILURE != ret_val)
                                {
                                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                            "TCB De-Allocated successfully");
                                }
                                /* BUG_11576_FIX_END */
                                return RRM_ERR_SYS_MEM_ALLOC_FAILURE; /* Coverity: CID 58744 */
                            }

                            p_mif_return = (mif_return_value *)p_node;
                            p_mif_return->fsm_index = MIF_FSM;
                            p_mif_return->event_id  = event_id;
                            if(event_id == MIF_FSM_EVENT_CELL_PLATFORM_IND)
                            {
                                /*spr_22250_changes_start*/
                                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                                        p_mif_context->currently_processed_api_trans_d);
                                /*spr_22250_changes_end*/
                                /* CID 118138 start */
                                if(RRM_FAILURE != ret_val)
                                {
                                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                            "TCB De-Allocated successfully");
                                }
                                /* CID 118138 end */

                                p_mif_return->p_msg_info =(U8 *)p_cell_load;
                            }
                            else
                            {
                                p_mif_return->p_msg_info =(U8 *)p_ecn_for_cell;
                            }
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                                    "Calling mif_raise_event_to_fsm for particular cells");
                            mif_raise_event_to_fsm (p_mif_context, p_gl_ctxt, p_node);
                            return_value = RRM_NO_ERROR;
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                                    "TCB cant be allocated");
                            return_value = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
                            /* COVERITY : RESOURCE LEAK : CID : 54401,54002 fix start */
                            if( RRM_NULL != p_ecn_for_cell )
                                RRM_MEM_FREE(p_ecn_for_cell);
                            if( RRM_NULL != p_cell_load )
                                RRM_MEM_FREE(p_cell_load);
                            /* COVERITY : RESOURCE LEAK : CID : 54401,54002 fix end*/
                            break;
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, 
                                "MIF Context find failed for mif_ctxt_id = %d", mif_ctxt_id);
                        /* Try next cell */
                    }            
                    break; /*start search for another MIF ctxt*/
                } 
            }
        }
    } /* end for loop */

    RRM_UT_TRACE_EXIT();
    return return_value;
}

/****************************************************************************
 * Function Name  : rrm_mif_send_event_to_all_cells
 * Inputs         : event_id 
 *                  p_msg_info : incoming (parsed) message buffer
 *                  p_gl_ctxt  : global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function loops through the saved mif contexts
 *                  and sends the requested event to all active cells.
 ****************************************************************************/
rrm_error_et rrm_mif_send_event_to_all_cells
(
 U16                   event_id, 
 U8                   *p_msg_info, 
 rrm_mif_gb_context_t *p_gl_ctxt,
 U16                   transaction_id,
 U16                   source 
 )
{
    U8                                     mif_ctxt_cnt     = RRM_ZERO;
    U8                                     mif_ctxt_id      = RRM_ZERO;
    YLNODE                                *p_node           = RRM_PNULL;

    mif_return_value                      *p_mif_return     = RRM_PNULL;
    rrm_mif_context                       *p_mif_context    = RRM_PNULL;
    rrm_error_et                           return_value     = RRM_ERR_CELL_UNCONFIGURED;
    rrm_return_et                          ret_val          = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (p_gl_ctxt != RRM_PNULL);

    /* Loop through all valid cells and trigger routine for
       each one them them */
    for (mif_ctxt_cnt = RRM_ZERO; mif_ctxt_cnt < MAX_NUM_MIF_CTXT; mif_ctxt_cnt++)
    {
        if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
        {
            mif_ctxt_id = p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Processing for mif_ctxt_id: %d, "
                    "cell_index: %d",
                    mif_ctxt_id, 
                    p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index);

            p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
            if (p_mif_context != RRM_PNULL)
            {
                /* mif context found in pool */
                ret_val = allocate_and_store_tcb_info_in_mif_context(
                        p_mif_context,
                        /* SPR_19279_START */
                        RRM_NULL,
                        RRM_ZERO,
                        /* SPR_19279_END */
                        transaction_id,
                        source);
                if (RRM_SUCCESS == ret_val)
                {
                    /* Trigger MIF FSM for this cell */
                    p_node = (YLNODE *)rrm_mem_get(sizeof(mif_return_value));
                    if ( p_node == RRM_PNULL)
                    {
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                "Memory allocation to p_node failed" );
                        /* BUG_11576_FIX_START */
                        /*SPR 15993 Fix Start*/
                        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                                            p_mif_context->currently_processed_api_trans_d);
                        /*SPR 15993 Fix End*/
                        if(RRM_FAILURE != ret_val)
                        {
                            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                                    "TCB De-Allocated successfully");
                        }
                        /* BUG_11576_FIX_END */
                        RRM_UT_TRACE_EXIT();
                        return RRM_ERR_SYS_MEM_ALLOC_FAILURE; /* Coverity: CID 58745 */
                    }

                    p_mif_return = (mif_return_value *)p_node;
                    /* SPR 11781 start */
                    switch(event_id)
                    {
                        case MIF_FSM_EVENT_CELL_SHUTDOWN_REQ:
                            {
                                p_mif_return->p_msg_info = (U8 *)rrm_mem_get(sizeof(rrm_oam_shutdown_req_t));
                                if (p_mif_return->p_msg_info == RRM_PNULL)
                                {
                                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                            "Memory allocation to p_mif_return->p_msg_info failed" );
                                    RRM_UT_TRACE_EXIT();
                                    return RRM_ERR_SYS_MEM_ALLOC_FAILURE; /* Coverity: CID 58745 */
                                }
                                RRM_MEMCPY(p_mif_return->p_msg_info,p_msg_info,
                                        sizeof(rrm_oam_shutdown_req_t));
                            }
                            break;

                        case MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ:
                            {
                                p_mif_return->p_msg_info = (U8 *)rrm_mem_get(sizeof(rrm_oam_rac_enable_disable_req_t));
                                if (p_mif_return->p_msg_info == RRM_PNULL)
                                {
                                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                            "Memory allocation to p_mif_return->p_msg_info failed" );
                                    RRM_UT_TRACE_EXIT();
                                    return RRM_ERR_SYS_MEM_ALLOC_FAILURE; /* Coverity: CID 58745 */
                                }
                                RRM_MEMCPY(p_mif_return->p_msg_info,p_msg_info,
                                        sizeof(rrm_oam_rac_enable_disable_req_t));
                            }
                            break;

                        /* Coverity 109455 + */
                        case MIF_FSM_EVENT_CELL_START_REQ:
                        case MIF_FSM_EVENT_CELL_STOP_REQ:
                        case MIF_FSM_EVENT_CELL_DELETE_REQ:
                        case MIF_FSM_EVENT_CELL_RESUME_REQ:
                        case MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY:
                        continue;

                        default:
                            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                             "Default case");
                        continue;
                        /* Coverity 109455 - */
                    }
                    /* SPR 11781 end */

                    p_mif_return->fsm_index = MIF_FSM;
                    p_mif_return->event_id  = event_id;

                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "DEBUG: Calling mif_raise_event_"
                            "to_fsm");
                    mif_raise_event_to_fsm (p_mif_context, p_gl_ctxt, p_node);
                    return_value = RRM_NO_ERROR;
                }
                else
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                    RRM_UT_TRACE_EXIT();
                    return RRM_ERR_SYS_MEM_ALLOC_FAILURE;
                }
            }
            else
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "MIF Context find failed for "
                        "mif_ctxt_id = %d", mif_ctxt_id);
                /* Try next cell */
            }
        }
        else
        {
            /* This mif context id is invalid. Continue looping */
        }
    } /* end for loop */

    /* SPR 14088 Fix start */
    if(RRM_NO_ERROR == return_value)
    {
        /* SPR 11781 start */
        if (RRM_PNULL != p_msg_info)
        {
            RRM_MEM_FREE(p_msg_info);
        }
        /* SPR 11781 end */
    }
    /* SPR 14088 Fix end */
    RRM_UT_TRACE_EXIT();
    return return_value;
}

/****************************************************************************
 * Function Name  : rrm_mif_process_shutdown_timer_expiry
 * Inputs         : p_gl_ctxt : global context
 *                  p_timer_buf : timer data
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes an init indication timer expiry
 *                  event. It sends an init indication message to OAM and
 *                  restarts the timer.
 ****************************************************************************/
static void rrm_mif_process_shutdown_timer_expiry
(
 rrm_mif_gb_context_t *p_gl_ctxt,
 mif_timer_buf        *p_timer_buf
 )
{
    rrm_error_et return_value = RRM_NO_ERROR;
   /* coverity id 16523 , ,rel 2.0.1 */
    rrm_oam_shutdown_resp_t  cell_shutdown_resp_to_oam = {RRM_INDEX_ERR,RRM_NO_ERROR};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (p_gl_ctxt != NULL);
    RRM_ASSERT (p_timer_buf != NULL);

    /* Loop through all valid cells and trigger the shutdown routine for
       each one them them */
    return_value = rrm_mif_send_event_to_all_cells(
            MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY,
            RRM_PNULL,
            p_gl_ctxt,
            p_timer_buf->transaction_id,
            p_timer_buf->source);

    if (return_value != RRM_NO_ERROR)
    {
        cell_shutdown_resp_to_oam.response = RRM_FAILURE;
        cell_shutdown_resp_to_oam.fail_cause = return_value;

        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY Response[%d] ",
                cell_shutdown_resp_to_oam.fail_cause);

        /* COMPOSER send the message to OAM AGENT */
	/*coverity 21233, rel 2.0.1, */
        if(RRM_SUCCESS == rrm_oam_send_rrm_oam_shutdown_resp(
                (void *)&cell_shutdown_resp_to_oam,
                RRM_MIF_MODULE_ID,
                RRM_OAM_MODULE_ID,
                p_timer_buf->transaction_id,
                RRM_ZERO))
	{
	            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
		                    "Shutdown response sent to OAM\n");
	}
	else
	{
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
	                "Error sending message to OAM\n");
	}

        /* Change MIF state to ACTIVE */
        p_gl_ctxt->current_state = RMIF_STATE_ACTIVE; 

    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_process_switchoff_timer_expiry 
 * Inputs         : p_gl_ctxt
 * Outputs        : None 
 * Returns        : None
 * Description    : 
 ****************************************************************************/
static void rrm_mif_process_switchoff_timer_expiry
(
 rrm_mif_gb_context_t *p_gl_ctxt,
 mif_timer_buf        *p_timer_buf
 )
{
    rrm_mif_context    *p_mif_context = RRM_PNULL;
   /* rrm_son_cell_switch_off_res_t cell_switch_off_resp_to_son = {0};*/
    YLNODE                   *p_ret_val_node = RRM_PNULL;
    mif_return_value           *p_return_value = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (p_gl_ctxt != NULL);
    RRM_ASSERT (p_timer_buf != NULL);

    p_mif_context = rrm_find_mif_context(p_gl_ctxt, p_timer_buf->mif_ctxt_id);

    if (RRM_PNULL != p_mif_context)
    { 
        p_ret_val_node = (YLNODE *)rrm_mem_get(sizeof(mif_return_value));
        if (p_ret_val_node == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_ret_val_node failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        }
        /*  Coverity, CID 16297 */
        /*  end */
        p_return_value = (mif_return_value *)p_ret_val_node; 
        p_return_value->fsm_index = MIF_FSM;
        p_return_value->event_id = MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY;
        p_return_value->p_msg_info = RRM_PNULL;
        mif_raise_event_to_fsm (p_mif_context, p_gl_ctxt,
                (YLNODE *)p_return_value);
    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found in received "
                "RRMCM_RMIF_CELL_CONFIG_RESP ");
    }

    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_check_for_log_enable_disable_resp
 * Inputs         : p_gl_ctxt
 * Outputs        : None 
 * Returns        : None
 * Description    : 
 ****************************************************************************/
static void rrm_check_for_log_enable_disable_resp(rrm_mif_gb_context_t  *p_gl_ctxt,
        rrm_error_et fail_cause, /* Coverity_ID : 16508 */
        U16 transaction_id)
{
  /* coverity id 16517 , ,rel 2.0.1 */
    rrm_oam_log_enable_disable_resp_t  out_message = {RRM_INDEX_ERR,RRM_NO_ERROR};
    rrm_bool_et                        response_received = RRM_FALSE;
    rrm_return_et                      ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (p_gl_ctxt != RRM_PNULL);

    if ((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD) &&
            (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD))
    {
        out_message.response = RRM_SUCCESS;
        out_message.fail_cause = RRM_NO_ERROR;
        response_received = RRM_TRUE;
        /* SPR_13117_FIX_START */
        p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
        /* SPR_13117_FIX_END */
    }
    else if (((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD) &&
                (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD)) ||
            ((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD) &&
             (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD)) ||
            ((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD) &&
             (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD)))

    {
        out_message.response = RRM_FAILURE;
        out_message.fail_cause = fail_cause;
        response_received = RRM_TRUE;
        /* SPR_13117_FIX_START */
        p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
        /* SPR_13117_FIX_END */
    }
    else
    {
        /* Response from one of the modules awaited */
    }

    if (RRM_TRUE == response_received)
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                "Sending log_enable_disable_resp [%d] to OAM", out_message.response);
        ret_val = rrm_oam_send_rrm_oam_log_enable_disable_resp(
                (void *)&out_message,
                RRM_MIF_MODULE_ID,
                RRM_OAM_MODULE_ID,
                transaction_id,
                RRM_ZERO);
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                    "Send Failed log_enable_disable_resp");
        }
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_check_for_set_log_level_resp
 * Inputs         : p_gl_ctxt
 * Outputs        : None
 * Returns        : None
 * Description    :
 ****************************************************************************/
static void rrm_check_for_set_log_level_resp(rrm_mif_gb_context_t  *p_gl_ctxt,
        /* coverity : CID 16508*/
        rrm_error_et fail_cause,
        U16 transaction_id)
{
  /* coverity id 16521 , ,rel 2.0.1 */
    rrm_oam_set_log_level_resp_t  out_message = {RRM_INDEX_ERR,RRM_NO_ERROR};
    rrm_bool_et                   response_received = RRM_FALSE;
    rrm_return_et                 ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (p_gl_ctxt != RRM_PNULL);

    if ((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD) &&
            (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD))
    {
        out_message.response = RRM_SUCCESS;
  /* coverity id 16508 , ,rel 2.0.1 */
        out_message.fail_cause = RRM_NO_ERROR;
        response_received = RRM_TRUE;
        /* SPR_13117_FIX_START */
        p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
        /* SPR_13117_FIX_END */
    }
    else if (((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD) &&
                (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD)) ||
            ((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD) &&
             (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD)) ||
            ((p_gl_ctxt->module_resp & RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD) &&
             (p_gl_ctxt->module_resp & RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD)))

    {
        out_message.response = RRM_FAILURE;
        out_message.fail_cause = fail_cause;
        response_received = RRM_TRUE;
        /* SPR_13117_FIX_START */
        p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
        /* SPR_13117_FIX_END */
    }
    else
    {
        /* Response from one of the modules awaited */
    }

    if (RRM_TRUE == response_received)
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
        ret_val = rrm_oam_send_rrm_oam_set_log_level_resp (
                (void *)&out_message,
                RRM_MIF_MODULE_ID,
                RRM_OAM_MODULE_ID,
                transaction_id,
                RRM_ZERO);
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                    RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
        }
    }

    RRM_UT_TRACE_EXIT();
}

rrm_bool_et rrm_mif_verify_ld_action_bits
(
    rrm_oam_load_def_t *p_actn
)
{
    rrm_bool_et ret_val = RRM_TRUE;
    RRM_UT_TRACE_ENTER();
    RRM_ASSERT (RRM_PNULL != p_actn);

    if (p_actn->action <= RRM_MAX_ACTION_VALUE)
    {
        if ((p_actn->action > RRM_ONE) && ((p_actn->action % RRM_TWO) == RRM_ONE )) 
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING,
                    "Load Config Req should contains either NONE ACTION or Any Other action");
            ret_val = RRM_FALSE;
        }
    }
    else
    {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                    "Load Config action [0x%x] is exceeded max action value [0x%x]",
                    p_actn->action, RRM_MAX_ACTION_VALUE);
            ret_val = RRM_FALSE;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/************************************************************************
 * Function Name  : rrm_mif_oam_process_load_config_req 
 * Inputs         : pointer to the message received
 *                  pointer to the SMIF global context
 *                  transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RRM_OAM_LOAD_CONFIG_REQ msg
 *************************************************************************/
static void rrm_mif_oam_process_load_config_req (
        void                   *p_msg, 
        rrm_mif_gb_context_t   *p_gl_ctxt,
        U16                    trans_id)
{
    U16 count = RRM_ZERO;
    S32 length_read    = RRM_ZERO;

  rrm_mif_context               *p_mif_context[RRM_MAX_NUM_CELLS];
  rrm_oam_load_config_req_t 	*in_message = RRM_PNULL;
  /* coverity id 23128 , ,rel 2.0.1 */
  rrm_oam_load_config_resp_t 	out_message = {RRM_INDEX_ERR,RRM_NO_ERROR};
  rrm_return_et                 ret_val = RRM_SUCCESS;
  rrm_bool_et                   actn_bit = RRM_TRUE;
  U8                            resrc_ld_cnt = RRM_NULL;
  U8                            mif_ctxt_cnt = RRM_NULL;
  U8                            mif_ctxt_id = RRM_NULL;
  U8				cell_count = RRM_ZERO;
  rrm_error_et                  fail_cause = RRM_NO_ERROR;

  RRM_UT_TRACE_ENTER();

  RRM_ASSERT(RRM_PNULL != p_msg);
  RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

  for(cell_count = RRM_ZERO; cell_count < RRM_MAX_NUM_CELLS; cell_count++)
  {
      p_mif_context[cell_count] = RRM_PNULL;
  }

  in_message = (rrm_oam_load_config_req_t *) rrm_mem_get(sizeof(rrm_oam_load_config_req_t));
  if(RRM_PNULL == in_message)
  {
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
              "Memory allocation failed for in_message");
    return;
  }
  else
  {
      RRM_MEMSET(in_message, RRM_ZERO, sizeof(rrm_oam_load_config_req_t));
      if (RRM_SUCCESS != rrm_parse_rrm_oam_load_config_req(
                  (void *)in_message,
                  (U8*)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
                  rrm_get_api_buf_size(p_msg) - 
                  RRM_INTERFACE_API_HEADER_SIZE,
                  (void *)&length_read))
      {
          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                  "OAM->MIF:RRM_OAM_LOAD_CONFIG_REQ parsing failed");
          fail_cause = RRM_ERR_INTERNAL_FAILURE;
          ret_val = RRM_FAILURE;
      }
      else 
      {
          RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
                  "OAM->MIF:RRM_OAM_LOAD_CONFIG_REQ successfully parsed");
          if(in_message->ncl_load_ind_intrvl != RRM_ZERO &&
                  in_message->ncl_load_ind_intrvl != RRM_ONE &&
                  in_message->ncl_load_ind_intrvl != RRM_TWO &&
                  in_message->ncl_load_ind_intrvl != RRM_FIVE &&
                  in_message->ncl_load_ind_intrvl != RRM_TEN)
          {
              fail_cause = RRM_ERR_INVALID_PARAMS;
              ret_val = RRM_FAILURE;
          }

          if (RRM_FAILURE != ret_val)
          {
              for(cell_count = RRM_ZERO; (cell_count < in_message->num_enb_cells) &&
                      (cell_count < RRM_MAX_NUM_CELLS);
                      cell_count++)
              {
                  if(((in_message->serv_enb_cell_info[cell_count].bitmask & 
                                  RRM_OAM_OVER_LOAD_LVL_ACT_PRESENT) || 
                              (in_message->serv_enb_cell_info[cell_count].bitmask & 
                               RRM_OAM_HIGH_LOAD_LVL_ACT_PRESENT) || 
                              (in_message->serv_enb_cell_info[cell_count].bitmask & 
                               RRM_OAM_MID_LOAD_LVL_ACT_PRESENT)) && 
                          (in_message->serv_enb_cell_info[cell_count].bitmask &
                           RRM_OAM_RESRC_SPEC_PRESENT))
                  {
                      fail_cause = RRM_ERR_INVALID_PARAMS;
                      ret_val = RRM_FAILURE;
                      RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                              "Load Config Req should contain either Resrc specific or Generic");
                      break;
                  }

                  if (in_message->serv_enb_cell_info[cell_count].bitmask & RRM_OAM_OVER_LOAD_LVL_ACT_PRESENT) 
                  {
                      /*CSR:70202 start*/
                      actn_bit = rrm_mif_verify_ld_action_bits (&in_message->serv_enb_cell_info[cell_count].over_load_lvl_act);
                      /*CSR:70202 end*/
                      if (RRM_FALSE == actn_bit)
                      {
                          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                  "RRM MLB Load Bit Verification is failed for Comp Over Load");
                          break;
                      }

                  }
                  if (in_message->serv_enb_cell_info[cell_count].bitmask &
                          RRM_OAM_HIGH_LOAD_LVL_ACT_PRESENT)
                  {
                      actn_bit = rrm_mif_verify_ld_action_bits (&in_message->serv_enb_cell_info[cell_count].high_load_lvl_act);
                      if (RRM_FALSE == actn_bit)
                      {
                          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                  "RRM MLB Load Bit Verification is failed for Comp High Load");
                          break;
                      }
                  }
                  if (in_message->serv_enb_cell_info[cell_count].bitmask &
                          RRM_OAM_MID_LOAD_LVL_ACT_PRESENT)
                  {
                      actn_bit = rrm_mif_verify_ld_action_bits (&in_message->serv_enb_cell_info[cell_count].mid_load_lvl_act);
                      if (RRM_FALSE == actn_bit)
                      {
                          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                  "RRM MLB Load Bit Verification is failed for Comp Mid Load");
                          break;
                      }
                  }
                  if (in_message->serv_enb_cell_info[cell_count].bitmask &
                          RRM_OAM_RESRC_SPEC_PRESENT)
                  {
                      for (resrc_ld_cnt = RRM_NULL; resrc_ld_cnt < in_message->serv_enb_cell_info[cell_count].resrc_spec.count; ++resrc_ld_cnt)
                      {
                          if (in_message->serv_enb_cell_info[cell_count].resrc_spec.resrc_info[resrc_ld_cnt].resrc_type
                                  >= RRM_CELLM_MAX_RESRC)
                          {
                              RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                      "Invalid resource type[%u] received",
                                      in_message->serv_enb_cell_info[cell_count].resrc_spec.resrc_info[resrc_ld_cnt].resrc_type); 
                              fail_cause = RRM_ERR_INVALID_PARAMS;
                              ret_val = RRM_FAILURE;
                              break;
                          }
                          if (in_message->serv_enb_cell_info[cell_count].resrc_spec.resrc_info[resrc_ld_cnt].bitmask &
                                  RRM_OAM_ACTION_OVERLD_PRESENT)
                          {
                              /*CSR:70202 start*/
                               actn_bit = rrm_mif_verify_ld_action_bits (&in_message->serv_enb_cell_info[cell_count].\
                                          resrc_spec.resrc_info[resrc_ld_cnt].overload);
                              /*CSR:70202 end*/
                              if (RRM_FALSE == actn_bit)
                              {
                                  RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "RRM MLB Load Bit Verification is failed for Resrc Spec Over Load");
                                  break;
                              }
                          }
                          if (in_message->serv_enb_cell_info[cell_count].resrc_spec.resrc_info[resrc_ld_cnt].bitmask &
                                  RRM_OAM_ACTION_HIGHLD_PRESENT)
                          {
                              actn_bit = rrm_mif_verify_ld_action_bits (&in_message->serv_enb_cell_info[cell_count].\
                                      resrc_spec.resrc_info[resrc_ld_cnt].highload);
                              if (RRM_FALSE == actn_bit)
                              {
                                  RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "RRM MLB Load Bit Verification is failed for Resrc Spec High Load");
                                  break;
                              }
                          }
                          if (in_message->serv_enb_cell_info[cell_count].resrc_spec.resrc_info[resrc_ld_cnt].bitmask &
                                  RRM_OAM_ACTION_MIDLD_PRESENT)
                          {
                              actn_bit = rrm_mif_verify_ld_action_bits (&in_message->serv_enb_cell_info[cell_count].\
                                      resrc_spec.resrc_info[resrc_ld_cnt].midload);
                              if (RRM_FALSE == actn_bit)
                              {
                                  RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "RRM MLB Load Bit Verification is failed for Resrc Spec Mid Load");
                                  break;
                              }
                          }
                      }
                      if (RRM_FALSE == actn_bit)
                      {
                          break;
                      } 
                  }
              }
              if (RRM_FALSE == actn_bit)
              {
                  fail_cause = RRM_ERR_INVALID_PARAMS;
                  ret_val = RRM_FAILURE;
                  RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                          "Load Config Req has invalid parameter");
              }
          }

          if (RRM_FAILURE != ret_val)
          {
              for(cell_count = RRM_ZERO; (cell_count < in_message->num_enb_cells) && (cell_count < RRM_MAX_NUM_CELLS);
                      cell_count++)
              {
                  ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt,
                          &(in_message->serv_enb_cell_info[cell_count].global_cell_id),
                          &mif_ctxt_cnt,
                          &mif_ctxt_id);
                  if (RRM_FAILURE == ret_val)
                  {
                      RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Mif Context not found for cell at loc [%u]", cell_count);
                      fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                      break;
                  }
                  else
                  {
                      p_mif_context[count] = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);

                      if (RRM_PNULL == p_mif_context[count])
                      {
                          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                                  RRM_ERROR, "MIF Context not found for cell at loc[%u] RRM_OAM_LOAD_CONFIG_REQ", mif_ctxt_id);
                          fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                          ret_val = RRM_FAILURE;
                          break;
                      }
                  }
                  count++;
                  mif_ctxt_id = RRM_ZERO;
                  mif_ctxt_cnt = RRM_ZERO;
              }
          }

          if (RRM_FAILURE != ret_val)
          {
              ret_val = build_and_send_load_config_req_to_cellm(/*SPR 17777 +-*/
                      p_mif_context,
                      count,
                      (rrm_oam_load_config_req_t *)(in_message), 
                      trans_id, &fail_cause);
              if (RRM_FAILURE == ret_val)
              {
                  fail_cause = RRM_ERR_MANDATORY_PARAMS_ABSENT; 
              }
          } 
      }


      if (RRM_FAILURE == ret_val)
      {
          out_message.response = RRM_FAILURE;
          out_message.fail_cause = fail_cause;

          rrm_oam_send_rrm_oam_load_config_resp((void *)&out_message,
                  RRM_MIF_MODULE_ID,
                  RRM_OAM_MODULE_ID,
                  trans_id,
                  RRM_ZERO);
      }
      RRM_MEM_FREE(in_message);
   }

  RRM_UT_TRACE_EXIT();
}
/* health monitoring changes start*/
/************************************************************************
 * Function Name  : rrm_mif_oam_process_chk_health_req
 * Inputs         : pointer to the message received
 *                  transaction id
 * Returns        : Void
 * Description    : This function handles RRM check health Req from OAM
 *                  msg received from OAM 
 *************************************************************************/
rrm_void_t rrm_mif_oam_process_chk_health_req(void    *p_msg,
                                          rrm_mif_gb_context_t   *p_gl_ctxt,
                                          U16      tr_id)
{
  rrm_oam_chk_health_req_t     chk_health_req  = {RRM_ZERO};   
  rrm_oam_chk_health_resp_t    chk_health_resp = {RRM_ZERO};
  S32                          length_read     = RRM_ZERO;
  RRM_UT_TRACE_ENTER();

  if (RRM_SUCCESS != rrm_parse_rrm_oam_chk_health_req(
                            (void *)&chk_health_req,
                            (U8 *)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            rrm_get_api_buf_size(p_msg) - RRM_INTERFACE_API_HEADER_SIZE,
                            (void *)&length_read))
  {
     RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                "Msg check health req parsing failed");
  }
  else
  {   
      if(RRM_SUCCESS == rrm_oam_check_thread_status(p_gl_ctxt->health_timer))
      {
          /* Sending Response to OAM */
          rrm_oam_send_rrm_oam_chk_health_resp(&chk_health_resp,
                                             RRM_MIF_MODULE_ID,
                                             RRM_OAM_MODULE_ID,
                                             tr_id,
                                             RRM_ZERO);
          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Success Chk response sent to OAM");
        
      }
      else
      {
          RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Health of RRM is not good ,not sending response to OAM");
      }
  }
  RRM_UT_TRACE_EXIT();
}
/* health monitoring changes end */
/************************************************************************
 * Function Name  : rrm_mif_oam_process_event_config_req
 * Inputs         : pointer to the message received
 *                  transaction id
 * Returns        : Void
 * Description    : This function handles RRM OAM Event Config Req 
 *                  msg received from OAM 
 *************************************************************************/
void rrm_mif_oam_process_event_config_req(void    *p_msg,
                                          U16      tr_id)
{
  S32   length_read  = RRM_ZERO;
  U8    event_index  = RRM_ZERO;
  rrm_return_et  ret_val = RRM_SUCCESS;
 
  rrm_oam_event_config_req_t       event_config_req = {RRM_ZERO};   
  /* coverity id 29722 , ,rel 2.0.1 */
  rrm_oam_event_config_resp_t      event_config_resp = {RRM_INDEX_ERR,RRM_NO_ERROR};
  rrmuem_rmif_event_config_req_t   event_config_req_to_uem = {RRM_ZERO};

  RRM_UT_TRACE_ENTER();

  if (RRM_SUCCESS != rrm_parse_rrm_oam_event_config_req(
                            (void *)&event_config_req,
                            (U8*)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            rrm_get_api_buf_size(p_msg) - RRM_INTERFACE_API_HEADER_SIZE,
                            (void *)&length_read))
  {
     RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                "Msg event config req parsing failed");
     ret_val = RRM_FAILURE;
  }
  else
  {
    event_config_req_to_uem.transaction_id = tr_id;
    event_config_req_to_uem.count = event_config_req.no_of_items; 
    for (event_index=RRM_ZERO; 
        ((event_index<event_config_req_to_uem.count)&&(event_index<RRM_OAM_MAX_SUBCLASS));
         ++event_index) 
     {
       event_config_req_to_uem.event_config[event_index].event_class = 
            event_config_req.event_config[event_index].event_class;
       event_config_req_to_uem.event_config[event_index].event_sub_class = 
            event_config_req.event_config[event_index].event_sub_class;
       event_config_req_to_uem.event_config[event_index].action_reqd = 
            event_config_req.event_config[event_index].action_reqd;
     }/*for loop ends here*/

    /*Sending msg to UEM*/
    rrm_mif_send_internal_msg(RRM_OAM_UEM_MODULE_ID,
                              RRMUEM_RMIF_EVENT_CONFIG_REQ,
                              sizeof(rrmuem_rmif_event_config_req_t),
                              (void *)&event_config_req_to_uem);
  }

  if (RRM_FAILURE == ret_val)
  {
     event_config_resp.response = RRM_FAILURE;
     event_config_resp.fail_cause = RRM_ERR_INVALID_PARAMS; 

     /* Sending msg to OAM */
     rrm_oam_send_rrm_oam_event_config_resp(&event_config_resp, 
                                            RRM_MIF_MODULE_ID, 
                                            RRM_OAM_MODULE_ID,
                                            tr_id,
                                            RRM_ZERO);
  }

  RRM_UT_TRACE_EXIT();
}

/* Carrier_Aggregation_Start */
/************************************************************************
 * Function Name  : rrm_mif_oam_process_enb_config_req
 * Inputs         : pointer to the message received
 *                  transaction id
 * Returns        : Void
 * Description    : This function handles RRM OAM eNodeB Config Req 
 *                  msg received from OAM 
 *************************************************************************/
void rrm_mif_oam_process_enb_config_req (void    *p_msg,
                                         U16      trans_id)
{
    S32   length_read                                   = RRM_ZERO;
    rrm_return_et  ret_val                              = RRM_SUCCESS;
    rrm_error_et   fail_cause                           = RRM_NO_ERROR;
    rrm_oam_enb_config_req_t    enb_config_req          = {RRM_ZERO};
    /*SPR 10329:fix start*/
    /*Coverity 65861 Fix Start*/
    rrm_oam_enb_config_resp_t   *p_enb_config_resp         = RRM_PNULL;
    p_enb_config_resp = (rrm_oam_enb_config_resp_t*)rrm_mem_get(
		    sizeof(rrm_oam_enb_config_resp_t));
    if (p_enb_config_resp == RRM_PNULL)
    {

	    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_ERROR, 
			    "Memory allocation to p_enb_config_resp failed");
	    rrm_abort();
    }
    RRM_MEMSET(p_enb_config_resp, RRM_ZERO, sizeof(rrm_oam_enb_config_resp_t));


    /*Coverity 65861 Fix End*/
    /*SPR 10329:fix end*/
    rrmcm_rmif_enb_config_req_t enb_config_req_to_cellm = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_msg);
    if (RRM_SUCCESS != rrm_parse_rrm_oam_enb_config_req(
                (void *)&enb_config_req,
                (U8*)p_msg + RRM_FULL_INTERFACE_HEADERS_SIZE,
                rrm_get_api_buf_size(p_msg) - 
                RRM_INTERFACE_API_HEADER_SIZE,
                (void *)&length_read))
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "OAM->MIF:RRM_OAM_ENB_CONFIG_REQ parsing failed");
        fail_cause = RRM_ERR_INTERNAL_FAILURE;
        ret_val = RRM_FAILURE;
    }
    else
    {
      RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
              "OAM->MIF:RRM_OAM_ENB_CONFIG_REQ successfully parsed");
        /*SPR 10329:start*/
        if (RRM_ZERO == enb_config_req.bitmask)
        /*SPR 10329:end*/
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, 
                    "Bitmask is not set for enb_type in enb config request");
            fail_cause = RRM_ERR_UNEXPECTED_MSG;
            ret_val = RRM_FAILURE;
        }
        else
        {
            /*SPR 10329:start*/
            if(RRM_OAM_GLOBAL_ENB_ID_PRESENT & enb_config_req.bitmask)
            {
                /* Copying global enb id */
                enb_config_req_to_cellm.bitmask |= RRMCM_RMIF_GLOBAL_ENB_ID_PRESENT;
                RRM_MEMCPY(&enb_config_req_to_cellm.enb_id,&enb_config_req.enb_id,
                        sizeof(rrm_global_enb_id_t));
            }
            /* Copying GU Group Info */
            if(RRM_OAM_GU_GROUP_INFO_PRESENT & enb_config_req.bitmask)
            {
                enb_config_req_to_cellm.bitmask |= RRMCM_RMIF_GU_GROUP_INFO_PRESENT;
                RRM_MEMCPY(&enb_config_req_to_cellm.gu_group_info, (rrm_gu_group_info_t *)&enb_config_req.gu_group_info,
                        sizeof (rrm_gu_group_info_t));
            }
            else
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF,
                        "GU Group Info is not present in enb_config_request");
            }
            /*SPR 10329:end*/

            enb_config_req_to_cellm.transaction_id = trans_id;
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
            if (RRM_OAM_QCI_FOR_SGNB_ADDITION_PRESENT & enb_config_req.bitmask)
            {
                enb_config_req_to_cellm.bitmask |= 
                    RRMCM_RMIF_QCI_FOR_SGNB_ADDITION_PRESENT; 

                enb_config_req_to_cellm.qci_for_sgnb_addition = 
                    enb_config_req.qci_for_sgnb_addition;

                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED,
                        "EN_DC: qci_for_sgnb_addition[%d] ", 
                        enb_config_req_to_cellm.qci_for_sgnb_addition);
            }
/*NR_DC Code Change Stop*/
#endif
        }
        
        

    }
    if (RRM_SUCCESS == ret_val)
    {
            /* Sending enb config request to RRMCM*/
            rrm_mif_send_internal_msg(RRM_CM_MODULE_ID,
                    RRMCM_RMIF_ENB_CONFIG_REQ,
                    sizeof(rrmcm_rmif_enb_config_req_t),
                    (void *)&enb_config_req_to_cellm);
    }
    else
    {
	    /*Coverity 65861 Fix Start*/
	    p_enb_config_resp->response = RRM_FAILURE;
	    p_enb_config_resp->fail_cause = fail_cause;
	    /* Sending msg to OAM */
	    rrm_oam_send_rrm_oam_enb_config_resp((void *)p_enb_config_resp,
			    RRM_MIF_MODULE_ID,
			    RRM_OAM_MODULE_ID,
			    trans_id,
			    RRM_ZERO);
    }
    RRM_MEM_FREE(p_enb_config_resp);
    /*Coverity 65861 Fix End*/
    RRM_UT_TRACE_EXIT();
}
/* Carrier_Aggregation_End */

/****************************************************************************
 * Function Name  : mif_message_parser
 * Inputs         :
 *      1. p_api  is not null pointer
 *      2. apIHdr is not null pointer
 *      3. p_gl_ctxt is not null pointer
 *      4. mif_return_value is not null pointer
 * Outputs        : None
 * Returns        : p_mif_context pointer
 * Variables      : None
 * Description    :
 ****************************************************************************/
static rrm_mif_context* mif_message_parser
(
 rrm_mif_gb_context_t   *p_gl_ctxt, void *p_api,
 interface_api_header   *api_hdr, 
 STACKAPIHDR            *p_stack_api_hdr,
 mif_return_value       *ret_val
 )
{
    rrm_mif_context     *p_mif_context = RRM_PNULL;
    U16                 upper_byte_source = RRM_ZERO;
    U8                  *p_source = RRM_PNULL;
    U16                 source = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != api_hdr);
    RRM_ASSERT (RRM_PNULL != ret_val);

    p_source = (U8 *)&p_stack_api_hdr->from;

    /*spr 6839 start*/
    if(RRM_SUCCESS == is_big_endian())  /* Coverity: CID 41397 */
    {
        upper_byte_source = (U16)p_source[RRM_ZERO];
    }
    else
    {
        upper_byte_source = (U16)p_source[RRM_ONE];     
    }
    /*spr 6839 end*/

    if (upper_byte_source == RRM_SON_MODULE_ID)
    {
        source = upper_byte_source;
    }
    else
    {
        source = p_stack_api_hdr->from;
    }
    switch(source)
    {
        case RRM_OAM_MODULE_ID:
        case RRM_PLATFORM_MODULE_ID: 
            p_mif_context = mif_oam_message_parser(p_gl_ctxt, p_api,
                    api_hdr,ret_val);
            break;
        case RRM_SON_MODULE_ID:
            p_mif_context = mif_son_message_parser(p_gl_ctxt, p_api,
                    api_hdr,ret_val);
            break;

        case RRM_CM_MODULE_ID:
            p_mif_context = mif_cellm_message_parser(p_gl_ctxt, p_api,
                    p_stack_api_hdr,ret_val);
            break;

        case RRM_UEM_MODULE_ID:
            p_mif_context = mif_uem_message_parser(p_gl_ctxt, p_api,
                    p_stack_api_hdr,ret_val);
            break;
        default:
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                    "mif_message_parser: Module Id(0x%x) Received is wrong. ",
                    api_hdr->source);
            break;
    }
    RRM_UT_TRACE_EXIT();
    return p_mif_context;
}

/****************************************************************************
 * Function Name  : mif_cellm_message_parser
 * Inputs         : 
 *      1. p_api  is not null pointer
 *      2. api_hdr is not a null pointer.
 *      3. p_gl_ctxt is not null pointer
 *      4. mif_return_value is not null pointer
 * Outputs        : mif_return_value
 * Returns        : p_mif_context pointer
 * Variables      : None
 * Description    : This function parses all the incoming API's from CELLM. 
 ****************************************************************************/

static rrm_mif_context* mif_cellm_message_parser
(
 rrm_mif_gb_context_t *p_gl_ctxt, 
 void                 *p_api,
 STACKAPIHDR          *api_hdr, 
 mif_return_value     *returnValue
 )
{
    rrm_mif_context                       *p_mif_context = RRM_PNULL;
    U8                                     mif_ctxt_cnt = RRM_ZERO;
    U8                                     mif_ctxt_id = RRM_ZERO;
    rrm_return_et                          ret_val = RRM_FAILURE;
    rrm_cell_index_t                       rcvdCellIndex = RRM_ZERO;
    rrmcm_rmif_generic_resp_t                *p_generic_resp = RRM_PNULL;
    rrmcm_rmif_cell_state_change_ind_t       *p_cell_state_change_ind 
        = RRM_PNULL;
    rrmcm_rmif_cell_active_ue_report_ind_t   *p_cell_active_ue_report_ind 
        = RRM_PNULL;
    rrm_ue_count_threshold_reached_ind_t     *p_ue_cnt_threshhold_ind 
        = RRM_PNULL;
    rrmcm_rmif_cell_config_resp              *p_cell_config_resp 
        = RRM_PNULL;
    rrmcm_rmif_rac_enable_disable_res_t      *p_rac_enable_disable_resp 
        = RRM_PNULL;
    rrm_bool_et                               check_failed = RRM_FALSE;
    rrmcm_rmif_get_kpi_resp_t                *p_get_kpi_resp = RRM_PNULL;
    rrmcm_kpi_ind_t                          in_cell_kpi_ind = {RRM_ZERO};
    rrmcm_rmif_rach_l2_report_t              in_rach_l2_report = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != api_hdr);
    RRM_ASSERT (RRM_PNULL != returnValue);

    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
            RRM_DETAILED, 
            "mif_cellm_message_parser: Message(%d) Received from CELLM", 
            api_hdr->api);

    switch(api_hdr->api)
    {
       /* Responses received from CellM for SON Interface  START*/
        case (U16)RRMCM_RMIF_CELL_REGISTER_RESP:
        case (U16)RRMCM_RMIF_CELL_DEREGISTER_RESP:
        case (U16)RRMCM_RMIF_CELL_SET_ATTRIBUTE_RESP:
        case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ANR_IND:
        case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ES_IND:
            /* MLB Changes start */
        case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MLB_IND:
            /* MLB Changes end */
        case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MRO_IND:
        case (U16)RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ANR_IND:
        case (U16)RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ANR_IND:
        case (U16)RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ES_IND:
        case (U16)RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ES_IND:
        /* UE MEAS CHANGES : STARTS */  
        case (U16)RRMCM_RMIF_MEAS_CONFIG_RESP:
        case (U16)RRMCM_RMIF_MEAS_CONFIG_OBJ_REMOVE_RESP:
        /*bug_12796_changes_start*/
        case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_RACH_OPTI:
        /*bug_12796_changes_end*/
        /* UE MEAS CHANGES : ENDS */  
            { 
                switch(api_hdr->api)
                {
                    case (U16)RRMCM_RMIF_CELL_REGISTER_RESP:
                    case (U16)RRMCM_RMIF_CELL_DEREGISTER_RESP:
                    case (U16)RRMCM_RMIF_CELL_SET_ATTRIBUTE_RESP:
                    /* UE MEAS CHANGES : STARTS */  
                    case (U16)RRMCM_RMIF_MEAS_CONFIG_RESP:
                    case (U16)RRMCM_RMIF_MEAS_CONFIG_OBJ_REMOVE_RESP:
                    /* UE MEAS CHANGES : ENDS */  
                        {
                            p_generic_resp =  (rrmcm_rmif_generic_resp_t *)
                                                 rrm_mem_get(sizeof(rrmcm_rmif_generic_resp_t));
                                    if (p_generic_resp == RRM_PNULL)
                                    {
                                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                                "Memory allocation to p_generic_resp failed" );
                                        RRM_UT_TRACE_EXIT();
                                        return RRM_NULL;
                                    }
    
                            rrm_memcpy_rrmcm_rmif_generic_resp(p_generic_resp,
                                                   (rrmcm_rmif_generic_resp_t *)
                                                   (p_api + RRM_API_HEADER_SIZE));
                                    
                            rcvdCellIndex = p_generic_resp->cellindex;
                            break;
                        }

                    case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ANR_IND:
                    case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ES_IND:
                    case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MLB_IND:
                    case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MRO_IND:
                    /*bug_12796_changes_start*/
                    case (U16)RRMCM_RMIF_CELL_STATE_CHANGE_FOR_RACH_OPTI:
                    /*bug_12796_changes_end*/
                        {
                            p_cell_state_change_ind =  
                                (rrmcm_rmif_cell_state_change_ind_t *)
                                rrm_mem_get(sizeof(rrmcm_rmif_cell_state_change_ind_t));
                            if (p_cell_state_change_ind == RRM_PNULL)
                            {
                                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                        "Memory allocation to p_cell_state_change_ind failed" );
                                RRM_UT_TRACE_EXIT();
                                return RRM_NULL;
                            }

                            rrm_memcpy_rrmcm_rmif_cell_state_change_ind(
                                                       p_cell_state_change_ind,
                                          (rrmcm_rmif_cell_state_change_ind_t *)
                                                  (p_api + RRM_API_HEADER_SIZE));
                            rcvdCellIndex = p_cell_state_change_ind->cellindex;
                            break;
                        }

                    case (U16)RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ANR_IND:
                    case (U16)RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ES_IND:
                        {
                            p_cell_active_ue_report_ind = 
                                (rrmcm_rmif_cell_active_ue_report_ind_t *)
                                rrm_mem_get(sizeof(rrmcm_rmif_cell_active_ue_report_ind_t));
                            if (p_cell_active_ue_report_ind == RRM_PNULL)
                            {
                                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                        "Memory allocation to p_cell_active_ue_report_ind failed" );
                                RRM_UT_TRACE_EXIT();
                                return RRM_NULL;
                            }

                            rrm_memcpy_rrmcm_rmif_cell_active_ue_report_ind(
                                                  p_cell_active_ue_report_ind,
                                    (rrmcm_rmif_cell_active_ue_report_ind_t *)
                                    (p_api + RRM_API_HEADER_SIZE));
                            rcvdCellIndex = p_cell_active_ue_report_ind->cellindex;
                            break;
                        }

                    case (U16)RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ANR_IND:
                    case (U16)RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ES_IND:
                        {
                            p_ue_cnt_threshhold_ind = 
                                (rrm_ue_count_threshold_reached_ind_t *)
                                rrm_mem_get(sizeof(rrm_ue_count_threshold_reached_ind_t));
                            if (p_ue_cnt_threshhold_ind == RRM_PNULL)
                            {
                                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                        "Memory allocation to p_ue_cnt_threshhold_ind failed" );
                                RRM_UT_TRACE_EXIT();
                                return RRM_NULL;
                            }

                            rrm_memcpy_rrm_ue_count_threshold_reached_ind(
                                                    p_ue_cnt_threshhold_ind,
                                    (rrm_ue_count_threshold_reached_ind_t *)
                                    (p_api + RRM_API_HEADER_SIZE));
                            rcvdCellIndex = p_ue_cnt_threshhold_ind->cellindex;
                            break;
                        }
                }
                ret_val = RRM_FAILURE;

                for (mif_ctxt_cnt = RRM_ZERO;  
                     mif_ctxt_cnt < MAX_NUM_MIF_CTXT; 
                     mif_ctxt_cnt++)
                {
                    if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
                    {
                        if (p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index == 
                                rcvdCellIndex)
                        {
                            mif_ctxt_id = 
                                p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_DETAILED, 
                                    "Mif Context %d matched with stored Info", 
                                    mif_ctxt_id);
                            ret_val = RRM_SUCCESS;
                            break;
                        }
                    }
                }
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                            RRM_ERROR, 
                            "Incorrect Mif Context for Cell Id %d ", 
                            rcvdCellIndex);
                    check_failed = RRM_TRUE;
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
                    if (p_mif_context != RRM_PNULL)
                    {
                        switch(api_hdr->api)
                        {
                            case RRMCM_RMIF_CELL_REGISTER_RESP:
                            case RRMCM_RMIF_CELL_DEREGISTER_RESP:
                            case RRMCM_RMIF_CELL_SET_ATTRIBUTE_RESP:
                                {
                                    ret_val = validate_trans_id_rcvd_in_resp(
                                            p_mif_context, p_generic_resp->transaction_id);
                                    if (RRM_FAILURE == ret_val)
                                    {
                                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                                RRM_WARNING, 
                                                "Transaction Id mismatched from CellM ");
                                        p_generic_resp->response = RRM_FAILURE;
                                        p_generic_resp->fail_cause = RRM_ERR_TRANS_ID_UNMATCHED; /* Coverity: CID 55842 */
                                    }
                                    build_and_send_registration_based_resp_to_son(
                                            p_gl_ctxt,
                                            p_mif_context,
                                            p_generic_resp,
                                            api_hdr->api);
                                    RRM_MEM_FREE(p_generic_resp);
                                    break;
                                }
                            /* UE MEAS CHANGES : STARTS */  
                            case RRMCM_RMIF_MEAS_CONFIG_RESP:
                                {
                                    ret_val = validate_trans_id_rcvd_in_resp(
                                            p_mif_context, p_generic_resp->transaction_id);
#ifdef RRM_UT_FLAG        
                                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_S);
#endif        
                                    if (RRM_FAILURE == ret_val)
                                    {
                                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                                RRM_WARNING, 
                                                "Transaction Id mismatched from CellM ");
                                        p_generic_resp->response = RRM_FAILURE;
                                        p_generic_resp->fail_cause = RRM_ERR_TRANS_ID_UNMATCHED; /* Coverity: CID 55842 */
#ifdef RRM_UT_FLAG        
                                        RRM_DECR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_S);
                                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F);
                                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F_trans_id_unmatched);
#endif        
                                    }
                                    build_and_send_non_registration_based_resp_to_son(
                                            p_gl_ctxt,
                                            p_mif_context,
                                            p_generic_resp,
                                            RRM_SON_MEAS_CONFIG_RESP);
                                            
                                    RRM_MEM_FREE(p_generic_resp);
                                    break;
                                }
                            case RRMCM_RMIF_MEAS_CONFIG_OBJ_REMOVE_RESP:
                                {
                                    ret_val = validate_trans_id_rcvd_in_resp(
                                            p_mif_context, p_generic_resp->transaction_id);
                                    if (RRM_FAILURE == ret_val)
                                    {
                                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                                                RRM_WARNING,
                                                "Transaction Id mismatched from CellM ");
                                        p_generic_resp->response = RRM_FAILURE;
                                        p_generic_resp->fail_cause = RRM_ERR_TRANS_ID_UNMATCHED;
                                    }
                                    build_and_send_non_registration_based_resp_to_son(
                                            p_gl_ctxt,
                                            p_mif_context,
                                            p_generic_resp,
                                            RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP);

                                    RRM_MEM_FREE(p_generic_resp);
                                    break;
                                }
                            /* UE MEAS CHANGES : ENDS */  

                            case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ANR_IND:
                            case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ES_IND:
                                /* MLB Changes start */
                            case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MLB_IND:
                                /* MLB Changes end */
                            case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MRO_IND:
                            /* RACH_OPTIMIZATION_CHANGES_START */
                            case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_RACH_OPTI:
                            /* RACH_OPTIMIZATION_CHANGES_END */
                                {
                                    build_and_send_cell_state_change_ind_to_son(
                                            p_gl_ctxt,
                                            p_mif_context,
                                            p_cell_state_change_ind,
                                            api_hdr->api);
                                    RRM_MEM_FREE(p_cell_state_change_ind);
                                    break;
                                }

                            case RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ANR_IND:
                            case RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ES_IND:
                                {
                                    build_and_send_active_ue_count_ind_to_son(
                                            p_gl_ctxt,
                                            p_mif_context,
                                            p_cell_active_ue_report_ind,
                                            api_hdr->api);
                                    RRM_MEM_FREE(p_cell_active_ue_report_ind);
                                    break;
                                }

                            case RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ANR_IND :
                            case RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ES_IND :
                                {
                                    build_and_send_active_threashold_reached_indto_son(
                                            p_gl_ctxt,
                                            p_mif_context,
                                            p_ue_cnt_threshhold_ind,
                                            api_hdr->api);
                                    RRM_MEM_FREE(p_ue_cnt_threshhold_ind);
                                    break;
                                }
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found ");
                        check_failed = RRM_TRUE;
                    }
                }
                /*Cov_fix_63805_63806_63807*/
                if (RRM_TRUE == check_failed)
                {
                    switch(api_hdr->api)
                    {
                        case (U16)RRMCM_RMIF_CELL_REGISTER_RESP:
                        case (U16)RRMCM_RMIF_CELL_DEREGISTER_RESP:
                        case (U16)RRMCM_RMIF_CELL_SET_ATTRIBUTE_RESP:
                            {
                              /*Cov_fix_63805*/
                              if(p_generic_resp != RRM_PNULL)
                              {
                                build_and_send_registration_based_resp_to_son(
                                        p_gl_ctxt,
                                        RRM_PNULL, /* To indicate error */
                                        p_generic_resp,
                                        api_hdr->api);
                                RRM_MEM_FREE(p_generic_resp);
                              }
                            /*Cov_fix_63805*/
                            /*Cov_fix_63827*/   
                            break; 
                            /*Cov_fix_63827*/
                            }
                            /* UE MEAS CHANGES : STARTS */
                        case (U16)RRMCM_RMIF_MEAS_CONFIG_RESP:
                            {
                                /*Cov_fix_63805*/
                                if(p_generic_resp != RRM_PNULL)
                            {
                                p_generic_resp->response = RRM_FAILURE;
                                p_generic_resp->fail_cause = RRM_ERR_OBJECT_NON_EXISTENT;

#ifdef RRM_UT_FLAG        
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F);
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F_obj_non_exist);
#endif        
                                build_and_send_non_registration_based_resp_to_son(
                                        p_gl_ctxt,
                                        RRM_PNULL,  /* To indicate error */
                                        p_generic_resp,
                                        RRM_SON_MEAS_CONFIG_RESP);

                                RRM_MEM_FREE(p_generic_resp);
                                }
                                /*Cov_fix_63805*/
                                break;
                            }
                            /* UE MEAS CHANGES : ENDS */

                        case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ANR_IND:
                        case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ES_IND:
                            /* MLB Changes start */
                        case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MLB_IND:
                            /* MLB Changes end */
			/*CID 63805 Start*/ 
			case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_RACH_OPTI:
			/*CID 63805 End*/
                        case RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MRO_IND:
                            {
                                RRM_MEM_FREE(p_cell_state_change_ind);
                                break;
                            }
                        case RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ANR_IND:
                        case RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ES_IND:
                            {
                                RRM_MEM_FREE(p_cell_active_ue_report_ind);
                                break;
                            }
                        case RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ANR_IND :
                        case RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ES_IND :
                            {
                                RRM_MEM_FREE(p_ue_cnt_threshhold_ind);
                                break;
                            }
                        default : 
                            /*cov_fix_63804*/
                            if(RRM_PNULL != p_generic_resp)
                                RRM_MEM_FREE(p_generic_resp);
                            break ;
                            /*cov_fix_63804*/
                    }
                    p_mif_context = RRM_PNULL;
                    return p_mif_context;
                }
                break;
            }
            /* Responses received for SON Interface  ENDS*/


            /* Responses received from CELLM  STARTS*/
        case RRMCM_RMIF_CELL_RECONFIG_RESP:
        case RRMCM_RMIF_CELL_DELETE_RESP:
        case RRMCM_RMIF_CELL_START_RESP:
        case RRMCM_RMIF_CELL_STOP_RESP:
        case RRMCM_RMIF_CELL_UPDATE_RESP:
        case RRMCM_RMIF_CELL_STOP_ADMISSION_RESP:
        case RRMCM_RMIF_CELL_START_ADMISSION_RESP:
        /* ECN start*/
        case RRMCM_RMIF_CELL_ECN_CAPACITY_RESP:
        /*ECN end*/
        case RRMCM_RMIF_CONFIG_KPI_RESP:
            { 
                p_generic_resp =  (rrmcm_rmif_generic_resp_t *)
                    rrm_mem_get(sizeof(rrmcm_rmif_generic_resp_t));
                
                if ( p_generic_resp == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                        "Memory allocation to p_generic_resp failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_NULL;
                }    
                rrm_memcpy_rrmcm_rmif_generic_resp(
                                       p_generic_resp,
                        (rrmcm_rmif_generic_resp_t *)
                        (p_api + RRM_API_HEADER_SIZE));

                ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt, 
                        p_generic_resp->cellindex,
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context for Cell Id %d ", 
                            p_generic_resp->cellindex);
                    check_failed = RRM_TRUE;
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        ret_val = validate_trans_id_rcvd_in_resp(
                                p_mif_context, p_generic_resp->transaction_id);
                        if (RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Transaction Id mismatched from CellM for OAM msg");
                            p_generic_resp->response = RRM_FAILURE;
                            p_generic_resp->fail_cause = RRM_ERR_TRANS_ID_UNMATCHED;
                        }
                        returnValue->fsm_index = MIF_FSM;
                        switch(api_hdr->api)
                        {
                            case RRMCM_RMIF_CELL_START_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_CELL_START_RESP;
                                break;

                            case RRMCM_RMIF_CELL_STOP_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_CELL_STOP_RESP;
                                break;

                            case RRMCM_RMIF_CELL_STOP_ADMISSION_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_STOP_ADMISSION_RESP;
                                break;

                            case RRMCM_RMIF_CELL_START_ADMISSION_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_START_ADMISSION_RESP;
                                break;

                            case RRMCM_RMIF_CELL_RECONFIG_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_CELL_RECONFIG_RESP;
                                /* SPR_19279_START */
                                if( (RRM_NULL != p_mif_context->p_tcb) && 
                                /* SPR 19977 Fix Start */
                                    (RRM_NULL != p_mif_context->p_tcb->p_data)&&
                                   (RRM_ZERO!=((rrm_oam_eutran_global_cell_id_t *)(p_mif_context->p_tcb->p_data))->primary_plmn_id.num_mnc_digit) )
                                {   
                                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                                            &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id),
                                            ((rrm_oam_eutran_global_cell_id_t *)(p_mif_context->p_tcb->p_data)));

                                    /* SPR 19977 Fix End */

                                }  
                                /* SPR_19279_END */
                                break;

                            case RRMCM_RMIF_CELL_UPDATE_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_CELL_UPDATE_RESP;
                                break; 

                            case RRMCM_RMIF_CELL_DELETE_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_CELL_DELETE_RESP;
                                break;
                            /* ECN start*/
                            case RRMCM_RMIF_CELL_ECN_CAPACITY_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP; 
                                break;
                            /* ECN end*/
                            case RRMCM_RMIF_CONFIG_KPI_RESP:
                                returnValue->event_id = MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP; 
                                break;
                            default:
                                break;

                        }
                        returnValue->p_msg_info =(U8 *) p_generic_resp;
                        /*RRM_HEX_DUMP("RRMCM_RMIF_CELL_GENERIC_RESP",
                          p_api,api_hdr->param_len,p_mif_context);
                          rrm_print_generic_response(p_generic_resp);*/
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found in received "
                                "RRMCM_RMIF_CELL_GENERIC_RESP ");
                        check_failed = RRM_TRUE;
                    }
                }
                if (RRM_TRUE == check_failed)
                {
                    RRM_MEM_FREE(p_generic_resp);
                    p_mif_context = RRM_PNULL;
                    RRM_UT_TRACE_EXIT();
                    return p_mif_context;
                }
                break;
            }
        case RRMCM_RMIF_GET_KPI_RESP:
            {
                p_get_kpi_resp = (rrmcm_rmif_get_kpi_resp_t *)rrm_mem_get(sizeof(rrmcm_rmif_get_kpi_resp_t));
                
                if ( p_get_kpi_resp == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_get_kpi_resp failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
                RRM_MEMCPY(p_get_kpi_resp,
                        (rrmcm_rmif_get_kpi_resp_t *)
                        (p_api + RRM_API_HEADER_SIZE),
                        sizeof(rrmcm_rmif_get_kpi_resp_t));
                ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt, 
                        p_get_kpi_resp->cell_index,
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context for cellId:[%d]",
                    p_get_kpi_resp->cell_index);
                    check_failed = RRM_TRUE;
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        returnValue->fsm_index = MIF_FSM;
                        returnValue->event_id = MIF_FSM_EVENT_CELL_GET_KPI_RESP;
                        returnValue->p_msg_info =(U8 *) p_get_kpi_resp;
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found in received "
                                "RRMCM_RMIF_CELL_GET_KPI_RESP");
                        check_failed = RRM_TRUE;
                    }
                }
                if (RRM_TRUE == check_failed)
                {
                    RRM_MEM_FREE(p_get_kpi_resp);
                    p_mif_context = RRM_PNULL;
                    RRM_UT_TRACE_EXIT();
                    return p_mif_context;
                }
                break;
            }
        case RRMCM_RMIF_KPI_IND:
            {
                RRM_MEMCPY(&(in_cell_kpi_ind),
                        (rrmcm_kpi_ind_t *)
                        (p_api + RRM_API_HEADER_SIZE),
                        sizeof(rrmcm_kpi_ind_t));
                ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt, 
                        in_cell_kpi_ind.cell_index,
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context for cellId[%d]",
                    in_cell_kpi_ind.cell_index);
                    check_failed = RRM_TRUE;
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        /*build and send kpi ind to OAM*/
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "MIF Context found in received "
                                "RRMCM_RMIF_CELL_KPI_IND");
                        build_and_send_kpi_ind_to_oam(p_gl_ctxt, p_mif_context, &in_cell_kpi_ind);
                        check_failed = RRM_TRUE;
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found in received "
                                "RRMCM_RMIF_CELL_KPI_IND");
                        check_failed = RRM_TRUE;
                    }
                }
                if (RRM_TRUE == check_failed)
                {
                    p_mif_context = RRM_PNULL;
                    RRM_UT_TRACE_EXIT();
                    return p_mif_context;
                }
                /* Coverity: CID 54930 - break removed */
            }
        case RRMCM_RMIF_CELL_CONFIG_RESP:
            { 
                p_cell_config_resp =  (rrmcm_rmif_cell_config_resp *)
                    rrm_mem_get(sizeof(rrmcm_rmif_cell_config_resp));
                if ( p_cell_config_resp == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_cell_config_resp failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                rrm_memcpy_rrmcm_rmif_cell_config_resp(
                                    p_cell_config_resp,
                        (rrmcm_rmif_cell_config_resp *)
                        (p_api + RRM_API_HEADER_SIZE));

                ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                        &(p_cell_config_resp->global_cell_id),
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                    check_failed = RRM_TRUE;
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        returnValue->fsm_index = MIF_FSM;
                        returnValue->event_id = MIF_FSM_EVENT_CELL_CONFIG_RESP;
                        returnValue->p_msg_info =(U8 *) p_cell_config_resp;
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found in received "
                                "RRMCM_RMIF_CELL_CONFIG_RESP ");
                        check_failed = RRM_TRUE;
                    }
                }
                if (RRM_TRUE == check_failed)
                {
                    RRM_MEM_FREE(p_cell_config_resp);
                    p_mif_context = RRM_PNULL;
                    return p_mif_context;
                }
                break;
            }
        case RRMCM_RMIF_RAC_ENABLE_DISABLE_RES:
            { 
                /* This memory will get free in mif_raise_event_to_fsm */
                p_rac_enable_disable_resp =  
                    (rrmcm_rmif_rac_enable_disable_res_t *)
                    rrm_mem_get(sizeof(rrmcm_rmif_rac_enable_disable_res_t));
                if ( p_rac_enable_disable_resp == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_rac_enable_disable_resp failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                rrm_memcpy_rrmcm_rmif_rac_enable_disable_res(
                                     p_rac_enable_disable_resp,
                        (rrmcm_rmif_rac_enable_disable_res_t *)
                        (p_api + RRM_API_HEADER_SIZE));

                for (mif_ctxt_cnt = RRM_ZERO; 
                        mif_ctxt_cnt < MAX_NUM_MIF_CTXT; 
                        mif_ctxt_cnt++)
                {
                    if (RRM_TRUE == 
                            p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
                    {
                        if(p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index ==
                                p_rac_enable_disable_resp->cell_index)
                        {
                            mif_ctxt_id = 
                                p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Mif Context %d matched "
                                    "with stored Info", mif_ctxt_id);
                            ret_val = RRM_SUCCESS;
                            break;
                        }
                    }
                }
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context for Cell Id %d", 
                            p_rac_enable_disable_resp->cell_index);
                    /* Coverity 54387 fix start */
                    RRM_MEM_FREE(p_rac_enable_disable_resp);
                    /* Coverity 54387 fix end */
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, 
                            mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        returnValue->fsm_index = MIF_FSM;
                        returnValue->event_id =
                            MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP;
                        returnValue->p_msg_info = 
                            (U8 *)p_rac_enable_disable_resp;
                    }
                    else
                    {
                        /* COVERITY : RESOURCE LEAK : CID : 54387 fix start */
                        if( RRM_NULL != p_rac_enable_disable_resp )
                            RRM_MEM_FREE( p_rac_enable_disable_resp );
                        /* COVERITY : RESOURCE LEAK : CID : 54387 fix end */
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found "
                                "in received RRMUEM_RMIF_RAC_ENABLE_DISABLE_RES");
                    }
                }
                break;
            }
            
        case RRMCM_RMIF_TNL_DISCOVERY_FROM_ANR_RES :
            {
                rrmcm_rmif_son_tnl_discovery_resp_t   *p_out_tnl_discovery_resp = RRM_PNULL;
                p_out_tnl_discovery_resp = (rrmcm_rmif_son_tnl_discovery_resp_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Response received for TNL DISCOVERY ");

                build_and_send_tnl_discovery_resp_to_son(
                        p_out_tnl_discovery_resp,
                        p_out_tnl_discovery_resp->transaction_id);
                break;
            }
            /* RACH_OPTIMIZATION_CHANGES_START */
        case RRMCM_RMIF_RACH_L2_REPORT:
            {
                RRM_MEMCPY(&(in_rach_l2_report),
                        (rrmcm_rmif_rach_l2_report_t *)
                        (p_api + RRM_API_HEADER_SIZE),
                        sizeof(rrmcm_rmif_rach_l2_report_t));
                ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt, 
                        in_rach_l2_report.cell_index,
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                            "Incorrect Mif Context for cellId[%d]",
                            in_rach_l2_report.cell_index);
                    check_failed = RRM_TRUE;
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        /*build and send kpi ind to SON*/
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
                                "MIF Context found is received in RRMCM_RMIF_RACH_L2_REPORT");
                        build_and_send_rach_report_to_son
                            (p_gl_ctxt, p_mif_context, &in_rach_l2_report);
                        /* Coverity 19oct : 54930 -delete */
                        //check_failed = RRM_TRUE;
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                "MIF Context not found in received "
                                "RRMCM_RMIF_RACH_L2_REPORT");
                        check_failed = RRM_TRUE;
                    }
                }
                if (RRM_TRUE == check_failed)
                {
                    p_mif_context = RRM_PNULL;
                    RRM_UT_TRACE_EXIT();
                    return p_mif_context;
                }
                break;
            }
            /* RACH_OPTIMIZATION_CHANGES_END */

            /* Responses received from CELLM  ENDS*/

            default:
                /* SPR 12193 Fix start */
                RRM_TRACE(g_mif_log_on_off, 
                          p_rrm_mif_facility_name, 
                          RRM_DETAILED,
                          "CELLM API [%d] Received is wrong. ", 
                          api_hdr->api);
                /* SPR 12193 Fix end */
    }
    RRM_UT_TRACE_EXIT();
    return p_mif_context;
}

/****************************************************************************
 * Function Name  : rrm_mif_populate_eutran_nbr_list
 * Inputs         : utran_count - No. of EUTRAN elements in the list
 *                  p_ue_utran_nbr_list - Pointer to EUtran list received from
 *                  UEM
 * Outputs        : p_ue_meas_results_ind -Pointer to the EUtran list to be sent
 *                  to SON
 * Returns        : None
 * Description    : Populates the meas_results_ind message to be sent to SON
 ****************************************************************************/
rrm_void_t
rrm_mif_populate_eutran_nbr_list
(
        U8                              eutran_count,
        rrm_son_eutran_neighbor_cell_t  *p_ue_eutran_nbr_list,
        rrm_son_eutran_neighbor_cell_t  *p_proc_eutran_nbr_list
        )
{
    U8  eutran_index = RRM_NULL;

    for(eutran_index = RRM_ZERO; eutran_index < eutran_count; eutran_index++)
    {
        p_ue_eutran_nbr_list[eutran_index].bitmask =
            p_proc_eutran_nbr_list->bitmask;

        p_ue_eutran_nbr_list[eutran_index].pci =
            p_proc_eutran_nbr_list->pci;

        if(RRM_SON_EUTRAN_NC_CGI_PRESENT &
                p_proc_eutran_nbr_list->bitmask)
        {
            rrm_memcpy_rrm_oam_eutran_global_cell_id(
                    &p_ue_eutran_nbr_list[eutran_index].cgi,
                    &p_proc_eutran_nbr_list->cgi);
        }
        if(RRM_SON_EUTRAN_NC_PLMN_PRESENT &
                p_proc_eutran_nbr_list->bitmask)
        {
            p_ue_eutran_nbr_list[eutran_index].num_plmn_id =
                p_proc_eutran_nbr_list->num_plmn_id;

            rrm_memcpy_plmn_id(
                    p_ue_eutran_nbr_list[eutran_index].plmn_id,
                    p_proc_eutran_nbr_list->plmn_id);
        }
        if(RRM_SON_EUTRAN_NC_TAC_PRESENT &
                p_proc_eutran_nbr_list->bitmask)
        {
            rrm_memcpy_tac(
                    p_ue_eutran_nbr_list[eutran_index].tac,
                    p_proc_eutran_nbr_list->tac);
        }
        if(RRM_SON_EUTRAN_NC_CSG_PRESENT &
                p_proc_eutran_nbr_list->bitmask)
        {
            RRM_MEMCPY(p_ue_eutran_nbr_list[eutran_index].csg_identity,
                    p_proc_eutran_nbr_list->csg_identity,
                    MAX_CELL_IDENTITY_OCTETS);
        }
        if(RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT &
                p_proc_eutran_nbr_list->bitmask)
        {
            p_ue_eutran_nbr_list[eutran_index].csg_membershipStatus=
                p_proc_eutran_nbr_list->csg_membershipStatus;
        }

        p_ue_eutran_nbr_list[eutran_index].rsrp =
            p_proc_eutran_nbr_list->rsrp;

        p_ue_eutran_nbr_list[eutran_index].rsrq =
            p_proc_eutran_nbr_list->rsrq;
       
        p_proc_eutran_nbr_list++;
    }
}

/****************************************************************************
 * Function Name  : rrm_mif_populate_utran_nbr_list
 * Inputs         : utran_count - No. of UTRAN elements in the list
 *                  p_ue_utran_nbr_list - Pointer to Utran list received from
 *                  UEM
 * Outputs        : p_ue_meas_results_ind - Pointer to the Utran list to be sent
 *                  to SOM
 * Returns        : None
 * Description    : Populates the meas_results_ind message to be sent to SON
 ****************************************************************************/
rrm_void_t
rrm_mif_populate_utran_nbr_list(
        U8                             utran_count,
        rrm_son_utran_neighbor_cell_t  *p_ue_utran_nbr_list,
        rrm_son_utran_neighbor_cell_t  *p_proc_utran_nbr_list
        )
{
    U8 utran_index = RRM_NULL;

    for(utran_index = RRM_ZERO; utran_index < utran_count; utran_index++)
    {
        p_ue_utran_nbr_list[utran_index].bitmask =
            p_proc_utran_nbr_list->bitmask;

        rrm_memcpy_rrm_son_utran_physical_cell_id(
                    &p_ue_utran_nbr_list[utran_index].pci,
                              &p_proc_utran_nbr_list->pci);

        if(RRM_SON_UTRAN_NC_CGI_PRESENT &
                p_proc_utran_nbr_list->bitmask)
        {
            rrm_memcpy_rrm_son_utran_geran_global_cell_id(
                    &p_ue_utran_nbr_list[utran_index].cgi,
                    &p_proc_utran_nbr_list->cgi);
        }
        if(RRM_SON_UTRAN_NC_LAC_PRESENT &
                p_proc_utran_nbr_list->bitmask)
        {
            rrm_memcpy_lac(
                    p_ue_utran_nbr_list[utran_index].lac,
                    p_proc_utran_nbr_list->lac,
                    MAX_LAC_SIZE);
        }
        if(RRM_SON_UTRAN_NC_RAC_PRESENT &
                p_proc_utran_nbr_list->bitmask)
        {
            p_ue_utran_nbr_list[utran_index].rac =
                p_proc_utran_nbr_list->rac;
        }
        if(RRM_SON_UTRAN_NC_PLMN_PRESENT &
                p_proc_utran_nbr_list->bitmask)
        {
            p_ue_utran_nbr_list[utran_index].num_plmn_id =
                p_proc_utran_nbr_list->num_plmn_id;

            rrm_memcpy_plmn_id(
                    p_ue_utran_nbr_list[utran_index].plmn_id,
                    p_proc_utran_nbr_list->plmn_id);
        }
        if(RRM_SON_UTRAN_NC_CSG_PRESENT &
                p_proc_utran_nbr_list->bitmask)
        {
            RRM_MEMCPY(p_ue_utran_nbr_list[utran_index].csg_identity,
                    p_proc_utran_nbr_list->csg_identity,
                    MAX_CELL_IDENTITY_OCTETS);
        }
        if(RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT &
                p_proc_utran_nbr_list->bitmask)
        {
            p_ue_utran_nbr_list[utran_index].csg_membershipStatus =
                p_proc_utran_nbr_list->csg_membershipStatus;
        }
        p_ue_utran_nbr_list[utran_index].rscp =
            p_proc_utran_nbr_list->rscp;

        p_ue_utran_nbr_list[utran_index].ecno =
            p_proc_utran_nbr_list->ecno;

        p_proc_utran_nbr_list++;
    }
}

/****************************************************************************
 * Function Name  : rrm_mif_populate_geran_nbr_list
 * Inputs         : utran_count - No. of GERAN elements in the list
 *                  p_ue_geran_nbr_list - Pointer to GERAN list received from
 *                  UEM
 * Outputs        : p_ue_meas_results_ind - Pointer to the GERAN list to be sent
 *                  to SON
 * Returns        : None
 * Description    : Populates the meas_results_ind message to be sent to SON
 ****************************************************************************/
rrm_void_t
rrm_mif_populate_geran_nbr_list(
        U8                             geran_count,
        rrm_son_geran_neighbor_cell_t  *p_ue_geran_nbr_list,
        rrm_son_geran_neighbor_cell_t  *p_proc_geran_nbr_list
        )
{
    U8 geran_index = RRM_NULL;

    for(geran_index = RRM_ZERO; geran_index < geran_count; geran_index++)
    {
        p_ue_geran_nbr_list[geran_index].bitmask =
            p_proc_geran_nbr_list->bitmask;

        p_ue_geran_nbr_list[geran_index].arfcn = 
            p_proc_geran_nbr_list->arfcn;

        p_ue_geran_nbr_list[geran_index].band_ind =
            p_proc_geran_nbr_list->band_ind;

        rrm_memcpy_rrm_son_geran_physical_cell_id(
                &p_ue_geran_nbr_list[geran_index].pci,
                &p_proc_geran_nbr_list->pci);

        if(RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT &
                p_proc_geran_nbr_list->bitmask)
        {
            rrm_memcpy_rrm_son_utran_geran_global_cell_id(
                    &p_ue_geran_nbr_list[geran_index].cgi,
                    &p_proc_geran_nbr_list->cgi);
        }
        if(RRM_SON_GERAN_MR_IND_RAC_PRESENT &
                p_proc_geran_nbr_list->bitmask)
        {
            p_ue_geran_nbr_list[geran_index].rac =
                        p_proc_geran_nbr_list->rac;
        }
        p_ue_geran_nbr_list[geran_index].rssi =
            p_proc_geran_nbr_list->rssi;

        p_proc_geran_nbr_list++;
    }
}


/****************************************************************************
 * Function Name  : rrm_mif_populate_cdma_nbr_list
 * Inputs         : utran_count - No. of CDMA elements in the list
 *                  p_ue_cdma_nbr_list - Pointer to CDMA list received from
 *                  UEM
 * Outputs        : p_ue_meas_results_ind - Pointer to the CDMA list to be sent
 *                  to SON
 * Returns        : None
 * Description    : Populates the meas_results_ind message to be sent to SON
 ****************************************************************************/
rrm_void_t
rrm_mif_populate_cdma_nbr_list(
        U8                             cdma_count,
        rrm_son_cdma_neighbor_cell_t  *p_ue_cdma_nbr_list,
        rrm_son_cdma_neighbor_cell_t  *p_proc_cdma_nbr_list
        )
{
    U8 cdma_index = RRM_NULL;

    for(cdma_index = RRM_ZERO; cdma_index < cdma_count; cdma_index++)
    {
        p_ue_cdma_nbr_list[cdma_index].bitmask =
            p_proc_cdma_nbr_list->bitmask;

        p_ue_cdma_nbr_list[cdma_index].arfcn = 
            p_proc_cdma_nbr_list->arfcn;

        p_ue_cdma_nbr_list[cdma_index].band_class =
            p_proc_cdma_nbr_list->band_class;

        RRM_MEMCPY(&p_ue_cdma_nbr_list[cdma_index].pci,
                &p_proc_cdma_nbr_list->pci,
                sizeof(U16));

        if(RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT &
                 p_proc_cdma_nbr_list->bitmask)
        {
             p_ue_cdma_nbr_list[cdma_index].bitmask |=
                     RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT;

            p_ue_cdma_nbr_list[cdma_index].pilot_pn_phase =
                     p_proc_cdma_nbr_list->pilot_pn_phase;
        }

        p_ue_cdma_nbr_list[cdma_index].pilot_strength =
                 p_proc_cdma_nbr_list->pilot_strength;


        if(RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT &
                p_proc_cdma_nbr_list->bitmask)
        {
            p_ue_cdma_nbr_list[cdma_index].bitmask |=
                     RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT;

            if( RRM_SON_CGI_1X_RTT_PRESENT & 
                    p_proc_cdma_nbr_list->cgi.bitmask)
            {
            
		p_ue_cdma_nbr_list[cdma_index].cgi.bitmask |=
                     RRM_SON_CGI_1X_RTT_PRESENT;

                RRM_MEMCPY(
                    &p_ue_cdma_nbr_list[cdma_index].cgi.cell_global_id_1X_RTT,
                    &p_proc_cdma_nbr_list->cgi.cell_global_id_1X_RTT,
                    RRM_SON_CELL_GLOBAL_ID_1X_RTT_SIZE * sizeof(U8));
            }
            if( RRM_SON_CGI_HRPD_PRESENT & 
                    p_proc_cdma_nbr_list->cgi.bitmask)
            {
		p_ue_cdma_nbr_list[cdma_index].cgi.bitmask |=
                     RRM_SON_CGI_HRPD_PRESENT;

                RRM_MEMCPY(
                    &p_ue_cdma_nbr_list[cdma_index].cgi.cell_global_id_HRPD,
                    &p_proc_cdma_nbr_list->cgi.cell_global_id_HRPD,
                    RRM_SON_CELL_GLOBAL_ID_HRPD_SIZE * sizeof(U8));
            }

        }

        p_proc_cdma_nbr_list++;
    }
}


/****************************************************************************
 * Function Name  : mif_uem_message_parser
 * Inputs         : 
 *      1. p_api  is not null pointer
 *      2. api_hdr is not a null pointer.
 *      3. p_gl_ctxt is not null pointer
 *      4. mif_return_value is not null pointer
 * Outputs        : mif_return_value
 * Returns        : p_mif_context pointer
 * Variables      : None
 * Description    : This function parses all the incoming API's from UEM. 
 ****************************************************************************/

static rrm_mif_context* mif_uem_message_parser(rrm_mif_gb_context_t *p_gl_ctxt, void *p_api,
        STACKAPIHDR *api_hdr, mif_return_value *returnValue)
{
    rrm_mif_context                         *p_mif_context = RRM_PNULL;
    U8                                       mif_ctxt_cnt = RRM_ZERO;
    U8                                       mif_ctxt_id = RRM_ZERO;
    U8                                       index = RRM_ZERO;
    rrm_return_et                            ret_val = RRM_FAILURE;
    rrmuem_rmif_emergency_call_active_res_t *p_emerg_call_active_resp 
        = RRM_PNULL;
    rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t  *p_non_emerg_active_call_ho_resp = RRM_PNULL;

    /* UE MEAS CHANGES : STARTS */
    U16                                      eutra_list_count = RRM_ZERO;
    U16                                      utra_list_count = RRM_ZERO;
    U16                                      geran_list_count = RRM_ZERO;
    U16                                      cdma_list_count = RRM_ZERO;
    rrmuem_rmif_meas_results_ind_t          *p_in_message = RRM_PNULL;
    rrm_son_meas_results_ind_t              *p_out_message = RRM_PNULL;
    rrmuem_rmif_meas_config_resp_t          *p_meas_in_message = RRM_PNULL;
    rrm_son_meas_config_res_t                meas_out_message;
    /* UE MEAS CHANGES : ENDS */
    rrm_son_ho_report_t                out_handover_report;
    rrmuem_rmif_ho_report_t           *p_in_handover_report;
    rrmuem_rmif_no_active_ue_ind_t    *p_no_active_ue_ind = RRM_PNULL;
    /* MRO changes Start */
    rrmuem_rmif_ho_attempt_ind_t      *p_in_handover_attempt_ind = RRM_PNULL;
    rrm_son_ho_attempt_ind_t           out_handover_attempt_ind;
    rrm_son_ho_failure_report_t        out_handover_failure_report = {RRM_ZERO};
    rrmuem_rmif_ho_failure_report_t   *p_in_handover_failure_report = RRM_PNULL;
    /* MRO changes ends */
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrmuem_rmif_ue_rach_report_t      *p_in_rach_report = RRM_PNULL;
   //    rrm_son_rach_ue_info_ind_t        out_ue_rach_report;
    rrm_son_rach_ue_info_ind_t        *p_out_ue_rach_report = RRM_PNULL;
    U16                               rach_count = RRM_ZERO;
    U8                                strong_cell_count = RRM_ZERO;
    /* RACH_OPTIMIZATION_CHANGES_END */

    RRM_UT_TRACE_ENTER();
        
    p_out_message  = (rrm_son_meas_results_ind_t *)\
                      rrm_mem_get(sizeof(rrm_son_meas_results_ind_t));

    /* SPR: 15922 Fix Start */
    if(RRM_PNULL == p_out_message)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR , "mif_uem_message_parser: "
                "could not allocate memory for p_out_message ");
        return RRM_PNULL;
    }

    p_out_ue_rach_report  = (rrm_son_rach_ue_info_ind_t *)\
                      rrm_mem_get(sizeof(rrm_son_rach_ue_info_ind_t ));

    if(RRM_PNULL == p_out_ue_rach_report)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR , "mif_uem_message_parser: "
                "could not allocate memory for p_out_ue_rach_report ");
        RRM_MEM_FREE(p_out_message);
        return RRM_PNULL;
    }
    /* SPR: 15922 Fix End */
   
    if( (RRM_PNULL == p_out_ue_rach_report) || (RRM_PNULL == p_out_message ) )
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR , "mif_uem_message_parser: "
            "could not allocate memory for p_out_ue_rach_report/p_out_message ");
        /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix start */
        if( RRM_NULL != p_out_message)
            RRM_MEM_FREE(p_out_message);
        if( RRM_NULL != p_out_ue_rach_report )
            RRM_MEM_FREE( p_out_ue_rach_report );
        /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix end*/

        return RRM_PNULL;
    }
                      
    RRM_MEMSET(  p_out_ue_rach_report , RRM_ZERO, sizeof(rrm_son_rach_ue_info_ind_t));

    RRM_ASSERT (RRM_PNULL != p_out_message);
    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != returnValue);

    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "mif_uem_message_parser: "
            "Message Received from UEM");

    switch(api_hdr->api)
    {

        /* Responses received from UEM  STARTS*/

        case RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES:
            { 
                /* This memory will get free in mif_raise_event_to_fsm */
                p_emerg_call_active_resp =  
                    (rrmuem_rmif_emergency_call_active_res_t *)
                    rrm_mem_get(sizeof(rrmuem_rmif_emergency_call_active_res_t));

                if ( p_emerg_call_active_resp == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_emerg_call_active_resp failed" );
                    /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix start */
                    if( RRM_NULL != p_out_message)
                        RRM_MEM_FREE(p_out_message);
                    if( RRM_NULL != p_out_ue_rach_report )
                        RRM_MEM_FREE( p_out_ue_rach_report );
                    /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix end */
                    /* SPR: 15922 Fix Start */
                    break;
                    /* SPR: 15922 Fix End */
                }
                rrm_memcpy_rrmuem_rmif_emergency_call_active_res(p_emerg_call_active_resp,
                        (rrmuem_rmif_emergency_call_active_res_t *)(p_api + RRM_API_HEADER_SIZE));

                ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt, 
                        p_emerg_call_active_resp->cell_index,
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                            RRM_ERROR, 
                            "Incorrect Mif Context for Cell Id %d ", p_emerg_call_active_resp->cell_index);
                    /*COVERITY : RESOURCE LEAK : CID : 54398 fix start */
                    if( RRM_NULL != p_emerg_call_active_resp )
                        RRM_MEM_FREE( p_emerg_call_active_resp );
                    /* COVERITY : RESOURCE LEAK : CID : 54398 fix end */
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                    if (RRM_PNULL != p_mif_context)
                    {
                        returnValue->fsm_index = MIF_FSM;
                        returnValue->event_id = MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP;
                        returnValue->p_msg_info = (U8 *)p_emerg_call_active_resp;
                        //RRM_HEX_DUMP("RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES",p_api,api_hdr->msg_size,p_mif_context);
                        //rrm_print_emergency_call_active_resp(p_emerg_call_active_resp);
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, 
                                "MIF Context not found in received RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES ");
                        /*COVERITY : RESOURCE LEAK : CID : 54398 fix start */
                        if( RRM_NULL != p_emerg_call_active_resp )
                            RRM_MEM_FREE( p_emerg_call_active_resp );
                        /* COVERITY : RESOURCE LEAK : CID : 54398 fix end */
                    }
                }
                break;
            }
        /* UE MEAS CHANGES : STARTS */   
        case RRMUEM_RMIF_MEAS_CONFIG_RES:
            {
                rrm_memset_rrm_oam_cell_plmn_info((rrm_oam_cell_plmn_info_t *)&meas_out_message, RRM_ZERO);
                p_meas_in_message = (rrmuem_rmif_meas_config_resp_t *)
                    ((U8*)p_api + RRM_API_HEADER_SIZE);

                ret_val = copy_global_cell_id_from_ctxt(
                        p_gl_ctxt,
                        &(meas_out_message.meas_config_res.cell_id),
                        p_meas_in_message->cellindex);

                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                            RRM_ERROR, "Incorrect Mif Context for Cell %d ",
                            p_meas_in_message->cellindex);
                }
                else
                {
                  /* coverity id 16505 , ,rel 2.0.1 */
                    meas_out_message.meas_config_res.result =(rrm_return_et)(
                        p_meas_in_message->response);
                   /* coverity id 16504 , ,rel 2.0.1 */
                    meas_out_message.meas_config_res.error_code =
                        (rrm_error_et)(p_meas_in_message->fail_cause);

                    /* Send Meas Config Resp to SON */
                    rrm_son_send_rrm_son_meas_config_res(
                            &meas_out_message,
                            RRM_MIF_MODULE_ID,
                            RRM_SON_ANR_MODULE_ID,
                            p_meas_in_message->transaction_id,
                            p_meas_in_message->cellindex);

                }
                break;
            }
        case RRMUEM_RMIF_MEAS_RESULTS_IND:
            {
                
                rrm_memset_rrm_son_meas_results_ind(p_out_message, RRM_ZERO);
                p_in_message = (rrmuem_rmif_meas_results_ind_t *)
                                  ((U8*)p_api + RRM_API_HEADER_SIZE);
                                  
                ret_val = copy_global_cell_id_from_ctxt(
                                  p_gl_ctxt, 
                                  &(p_out_message->cell_id),
                                  p_in_message->cell_index);
                              
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                        RRM_ERROR, "Incorrect Mif Context for Cell %d ",
                        p_in_message->cell_index);
                }
                else
                {
                    /* Fill EUTRA NBRS */
                    p_out_message->ue_index = p_in_message->ue_index;
                    /* SPR 15698 Start */
                    /// Set Meas_Report_Event
                    /*coverity 86445 fix start*/
                    /* SPR-21979 START */
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, " meas_report_event = %u", p_in_message->meas_report_event);
                    /* SPR-21979 END */
                    /*coverity 86445 fix end*/
                    p_out_message->meas_report_event =  p_in_message->meas_report_event;
                    /* SPR 15698 End */
                    p_out_message->eutran_meas_list_size = p_in_message->
                                                 eutran_meas_list_size;
                    for(index=RRM_ZERO; index < p_out_message->eutran_meas_list_size; 
                          index++)                             
                    {
                        p_out_message->eutran_meas_list[index].earfcn =
                            p_in_message->eutran_meas_list[index].earfcn;
                           
                        eutra_list_count = p_in_message->eutran_meas_list
                            [index].neighbor_cell_list_size;

                        p_out_message->eutran_meas_list[index].
                            neighbor_cell_list_size = eutra_list_count;

                        rrm_mif_populate_eutran_nbr_list
                            (
                                eutra_list_count,
                                p_out_message->eutran_meas_list[index].
                                neighbor_cell_list,
                                p_in_message->eutran_meas_list[index].
                                neighbor_cell_list
                            );
                    }

                    /* Fill UTRA NBRS */
                    p_out_message->utran_meas_list_size  = p_in_message->
                                                 utran_meas_list_size;
                    for(index=RRM_ZERO; index < p_out_message->utran_meas_list_size; 
                          index++)                             
                    {     
                        utra_list_count = p_in_message->utran_meas[index].
                            neighbor_cell_list_size;
                            
                        p_out_message->utran_meas[index].uarfcn =
                            p_in_message->utran_meas[index].uarfcn;

                        p_out_message->utran_meas[index].neighbor_cell_list_size =
                            utra_list_count;

                        rrm_mif_populate_utran_nbr_list
                           (
                               utra_list_count,
                               p_out_message->utran_meas[index].neighbor_cell_list,
                               p_in_message->utran_meas[index].
                               neighbor_cell_list
                           );
                            
                    }
                    
                    /* Fill GERAN NBRS */
                    p_out_message->geran_meas_list_size  = p_in_message->
                                                 geran_meas_list_size;
                    for(index=RRM_ZERO; index < p_out_message->geran_meas_list_size; 
                          index++)                             
                    {     
                        if(RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT &
                            p_in_message->geran_meas[index].bitmask)
                        {
                            p_out_message->geran_meas[index].bitmask |=
                                RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT;

                            p_out_message->geran_meas[index].meas_obj_id =
                                p_in_message->geran_meas[index].meas_obj_id;
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                "meas_obj_id absent in meas results ind");
                        }
                        geran_list_count = p_in_message->geran_meas[index].
                            neighbor_cell_list_size;
                            
                        p_out_message->geran_meas[index].neighbor_cell_list_size =
                            geran_list_count;

                        rrm_mif_populate_geran_nbr_list
                           (
                               geran_list_count,
                               p_out_message->geran_meas[index].neighbor_cell_list,
                               p_in_message->geran_meas[index].
                               neighbor_cell_list
                           );
                            
                    }

                    /* Fill CDMA NBRS */
                    p_out_message->cdma_meas_list_size  = p_in_message->
                                                 cdma_meas_list_size;
                    
                    for(index=RRM_ZERO; index < p_out_message->cdma_meas_list_size; 
                          index++)                             
                    {     
                        if(RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT &
                            p_in_message->cdma_meas[index].bitmask)
                        {
                            p_out_message->cdma_meas[index].bitmask |=
                                RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT;

                            p_out_message->cdma_meas[index].meas_obj_id =
                                p_in_message->cdma_meas[index].meas_obj_id;
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                "meas_obj_id absent in meas results ind");
                        }
                        cdma_list_count = p_in_message->cdma_meas[index].
                            neighbor_cell_list_size;
                            
                        p_out_message->cdma_meas[index].neighbor_cell_list_size =
                            cdma_list_count;

                        rrm_mif_populate_cdma_nbr_list
                           (
                               cdma_list_count,
                               p_out_message->cdma_meas[index].neighbor_cell_list,
                               p_in_message->cdma_meas[index].neighbor_cell_list
                           );
                            
                    }
                    /* SPR 8878 fix: ecid_r9 mapping removed */

                    /* Send Meas Result Ind to SON */
                    rrm_son_send_rrm_son_meas_results_ind(
                            p_out_message,
                            RRM_MIF_MODULE_ID,
                            RRM_SON_ANR_MODULE_ID,
                            RRM_ZERO,
                            RRM_ZERO);
                }
                break;
            }
       /* LTE_RRM_MALL, CID 11122 */
       case RRMUEM_RMIF_HO_REPORT:
           {
               RRM_MEMSET(&out_handover_report,RRM_ZERO, sizeof(rrm_son_ho_report_t));
               p_in_handover_report = (rrmuem_rmif_ho_report_t *)
                                  ((U8*)p_api + RRM_API_HEADER_SIZE);

               /* MLB Changes start */
               RRM_MEMCPY(&out_handover_report.src_cgi, &p_in_handover_report->src_cgi, sizeof(out_handover_report.src_cgi));
               RRM_MEMCPY(&out_handover_report.trgt_cgi, &p_in_handover_report->trgt_cgi, sizeof(out_handover_report.trgt_cgi));
               /* Fill HO result as success to send the HO report in case of HO success to MLB and ANR */
               out_handover_report.result = p_in_handover_report->result; 
               
               if (p_in_handover_report->srv_cell_meas_strgth.bitmask & RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT)
               {
                 if (p_in_handover_report->srv_cell_meas_strgth.offset_attr.bitmask & RRM_OFFSET_ATTR_OFC_PRESENT)
                 {
                   out_handover_report.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OFC_PRESENT;
                   out_handover_report.offset_attr.ofs = p_in_handover_report->srv_cell_meas_strgth.offset_attr.ofs;
                 }
                 if (p_in_handover_report->srv_cell_meas_strgth.offset_attr.bitmask & RRM_OFFSET_ATTR_OCS_PRESENT)
                 {
                   out_handover_report.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OCS_PRESENT;
                   out_handover_report.offset_attr.ocs = p_in_handover_report->srv_cell_meas_strgth.offset_attr.ocs;
                 }
                 if (p_in_handover_report->srv_cell_meas_strgth.offset_attr.bitmask & RRM_OFFSET_ATTR_OFN_PRESENT)
                 {
                   out_handover_report.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OFN_PRESENT;
                   out_handover_report.offset_attr.ofn = p_in_handover_report->srv_cell_meas_strgth.offset_attr.ofn;
                 }
                 if (p_in_handover_report->srv_cell_meas_strgth.offset_attr.bitmask & RRM_OFFSET_ATTR_OFF_PRESENT)
                 {
                   out_handover_report.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OFF_PRESENT;
                   out_handover_report.offset_attr.off = p_in_handover_report->srv_cell_meas_strgth.offset_attr.off;
                 }
                 if (p_in_handover_report->srv_cell_meas_strgth.offset_attr.bitmask & RRM_OFFSET_ATTR_HYS_PRESENT)
                 {
                   out_handover_report.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_HYS_PRESENT;
                   out_handover_report.offset_attr.hys = p_in_handover_report->srv_cell_meas_strgth.offset_attr.hys;
                 }
               }

               out_handover_report.bitmask |= HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT;
               if (p_in_handover_report->srv_cell_meas_strgth.bitmask & RRM_EUTRAN_STRENGTH_RSRP_PRESENT)
               {
                    out_handover_report.serving_meas_strength.bitmask |=RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT;
                    out_handover_report.serving_meas_strength.rsrp  =p_in_handover_report->srv_cell_meas_strgth.rsrp;
               }
               if (p_in_handover_report->srv_cell_meas_strgth.bitmask & RRM_EUTRAN_STRENGTH_RSRQ_PRESENT)
               {
                    out_handover_report.serving_meas_strength.bitmask |=RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT;
                    out_handover_report.serving_meas_strength.rsrq  =p_in_handover_report->srv_cell_meas_strgth.rsrq;
               }
 
               switch(p_in_handover_report->trgt_cell_meas_strgth.choice)
               {
                 case RRM_EUTRAN_TRGT_CELL_STRENGTH:
                   {
                     out_handover_report.bitmask |= HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT;
                     out_handover_report.target_meas_strength.bitmask |= RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT;
                     if (RRM_EUTRAN_STRENGTH_RSRP_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.bitmask)
                     {
                       out_handover_report.target_meas_strength.eutran_cell_strength.bitmask |= RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT;
                       out_handover_report.target_meas_strength.eutran_cell_strength.rsrp = 
                         p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.rsrp;
                     }
                     if (RRM_EUTRAN_STRENGTH_RSRQ_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.bitmask)
                     {
                       out_handover_report.target_meas_strength.eutran_cell_strength.bitmask |= RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT;
                       out_handover_report.target_meas_strength.eutran_cell_strength.rsrq = 
                         p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.rsrq;
                     }
                     if (RRM_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.bitmask)
                     {
                       out_handover_report.target_meas_strength.eutran_cell_strength.bitmask |= RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT;
                       if (RRM_OFFSET_ATTR_OFC_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.bitmask)
                       {
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OFC_PRESENT;
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.ofs = 
                           p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.ofs;
                       }
                       if (RRM_OFFSET_ATTR_OCS_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.bitmask)
                       {
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OCS_PRESENT;
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.ocs = 
                           p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.ocs;
                       }
                       if (RRM_OFFSET_ATTR_OFN_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.bitmask)
                       {
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OFN_PRESENT;
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.ofn = 
                           p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.ofn;
                       }
                       if (RRM_OFFSET_ATTR_OFF_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.bitmask)
                       {
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_OFF_PRESENT;
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.off = 
                           p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.off;
                       }
                       if (RRM_OFFSET_ATTR_HYS_PRESENT & p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.bitmask)
                       {
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.bitmask |= RRM_SON_OFFSET_ATTR_HYS_PRESENT;
                         out_handover_report.target_meas_strength.eutran_cell_strength.offset_attr.hys = 
                           p_in_handover_report->trgt_cell_meas_strgth.u.eutran_cell_strength.offset_attr.hys;
                       }
                     }
                   }
                   break;

                 default:
                   break;
               }
               /* MLB Changes end */
                if (p_in_handover_report->rrm_dst_son_mod_ho_rep & RRMUEM_RMIF_DST_SON_MOD_MLB_HOR)
                {
                    rrm_son_send_rrm_son_ho_report(
                            &out_handover_report,
                            RRM_MIF_MODULE_ID,
                            RRM_SON_MLB_MODULE_ID,
                            RRM_ZERO,
                            RRM_ZERO);
                }
                if (p_in_handover_report->rrm_dst_son_mod_ho_rep & RRMUEM_RMIF_DST_SON_MOD_ANR_HOR)
                {
                    rrm_son_send_rrm_son_ho_report(
                            &out_handover_report,
                            RRM_MIF_MODULE_ID,
                            RRM_SON_ANR_MODULE_ID,
                            RRM_ZERO,
                            RRM_ZERO);
                }
           /* Fix for Coverity, LTE_RRM_MALL:CID:11122 [Prasant] */
               break;
           }             
        case RRMUEM_RMIF_NO_ACTIVE_UE_IND:
           {
               /*changes start*/
               p_no_active_ue_ind =
                   (rrmuem_rmif_no_active_ue_ind_t *)rrm_mem_get(sizeof(rrmuem_rmif_no_active_ue_ind_t));

               if ( p_no_active_ue_ind == RRM_PNULL)
               {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_no_active_ue_ind failed" );

                    /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix start */
                    if( RRM_NULL != p_out_message )
                        RRM_MEM_FREE( p_out_message );
                    if( RRM_NULL != p_out_ue_rach_report )
                        RRM_MEM_FREE( p_out_ue_rach_report );
                    /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix end   */
                    /* SPR: 15922 Fix Start */
                    break;
                    /* SPR: 15922 Fix End */
               }

               rrm_memcpy_rrmuem_rmif_no_active_ue_ind(p_no_active_ue_ind,
                       (rrmuem_rmif_no_active_ue_ind_t *)(p_api + RRM_API_HEADER_SIZE));

               ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
                       p_no_active_ue_ind->cell_index,
                       &mif_ctxt_cnt,
                       &mif_ctxt_id);
               if (RRM_FAILURE == ret_val)
               {
                   RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                           "Incorrect Mif Context for Cell Id %d ", p_no_active_ue_ind->cell_index);
                    /* COVERITY : RESOURCE LEAK : CID : 54399 fix start */
                    if( RRM_NULL != p_no_active_ue_ind )
                        RRM_MEM_FREE(p_no_active_ue_ind);
                    /* COVERITY : RESOURCE LEAK : CID : 54399 fix start */
               }
               else
               {
                   p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
                   if (RRM_PNULL != p_mif_context)
                   {
                       returnValue->fsm_index = MIF_FSM;
                       returnValue->event_id = MIF_FSM_EVENT_NO_ACTIVE_UE_IND;
                       returnValue->p_msg_info = (U8 *)p_no_active_ue_ind;
                   }
                   else
                   {
                       RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                               "MIF Context not found in received RRMUEM_RMIF_NO_ACTIVE_UE_IND");
                        /* COVERITY : RESOURCE LEAK : CID : 54399 fix start */
                        if( RRM_NULL != p_no_active_ue_ind )
                            RRM_MEM_FREE(p_no_active_ue_ind);
                        /* COVERITY : RESOURCE LEAK : CID : 54399 fix start */
                   }
               }
               break;
           }
           /*changes end*/

        case RRMUEM_RMIF_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP:
            {
                /* This memory will get free in mif_raise_event_to_fsm */
                p_non_emerg_active_call_ho_resp = 
                (rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *)rrm_mem_get(sizeof(rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t));
               if ( p_non_emerg_active_call_ho_resp == RRM_PNULL)
               {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_non_emerg_active_call_ho_resp failed" );
                    /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix start */
                    if( RRM_NULL != p_out_message)
                        RRM_MEM_FREE ( p_out_message );
                    if( RRM_NULL != p_out_ue_rach_report )
                        RRM_MEM_FREE( p_out_ue_rach_report );
                    /* COVERITY : RESOURCE LEAK : CID : 54396,54397 fix start */
                    /* SPR: 15922 Fix Start */
                    break;
                    /* SPR: 15922 Fix End */
               }
 
                rrm_memcpy_rrmuem_rmif_non_emrgncy_active_calls_ho_resp(
                                           p_non_emerg_active_call_ho_resp,
                        (rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *)(p_api + RRM_API_HEADER_SIZE));

                ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
                        p_non_emerg_active_call_ho_resp->cell_index,
                        &mif_ctxt_cnt,
                        &mif_ctxt_id);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                            "Incorrect Mif Context for Cell Id %d ", p_non_emerg_active_call_ho_resp->cell_index);
                    /* COVERITY : RESOURCE LEAK : CID : 54400 fix start */
                    if( RRM_NULL != p_non_emerg_active_call_ho_resp )
                        RRM_MEM_FREE(p_non_emerg_active_call_ho_resp);
                    /* COVERITY : RESOURCE LEAK : CID : 54400 fix end */
                }
                else
                {
                    p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
                    if (RRM_PNULL != p_mif_context)
                    {
                        returnValue->fsm_index = MIF_FSM;
                        returnValue->event_id = MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP;
                        returnValue->p_msg_info = (U8 *)p_non_emerg_active_call_ho_resp;
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                          "MIF Context not found in received RRMUEM_RMIF_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP");
                        /* COVERITY : RESOURCE LEAK : CID : 54400 fix start */
                        if( RRM_NULL != p_non_emerg_active_call_ho_resp )
                            RRM_MEM_FREE(p_non_emerg_active_call_ho_resp);
                        /* COVERITY : RESOURCE LEAK : CID : 54400 fix end */
                    }
                }
                break;
            }
            
        /* UE MEAS CHANGES : ENDS */   

       /* MRO changes Start */
       case RRMUEM_RMIF_HO_ATTEMPT_IND:
           {
               RRM_MEMSET(&out_handover_attempt_ind, RRM_ZERO, sizeof(rrm_son_ho_attempt_ind_t));

               p_in_handover_attempt_ind = (rrmuem_rmif_ho_attempt_ind_t *)
                                           ((U8*)p_api + RRM_API_HEADER_SIZE);

               RRM_MEMCPY(&(out_handover_attempt_ind), p_in_handover_attempt_ind,
                           sizeof(rrm_son_ho_attempt_ind_t));

               rrm_son_send_rrm_son_ho_attempt_ind(
                            &out_handover_attempt_ind,
                            RRM_MIF_MODULE_ID,
                            RRM_SON_MRO_MODULE_ID,
                            RRM_ZERO,
                            RRM_ZERO);
               break;
           }

       case RRMUEM_RMIF_HO_FAILURE_REPORT:
           {
               RRM_MEMSET(&out_handover_failure_report, RRM_ZERO, sizeof(rrm_son_ho_failure_report_t));

               p_in_handover_failure_report = (rrmuem_rmif_ho_failure_report_t *)
                                           ((U8*)p_api + RRM_API_HEADER_SIZE);

               RRM_MEMCPY(&(out_handover_failure_report), p_in_handover_failure_report,
                           sizeof(rrm_son_ho_failure_report_t));

               rrm_son_send_rrm_son_ho_failure_report(
                       &out_handover_failure_report,
                       RRM_MIF_MODULE_ID,
                       RRM_SON_MRO_MODULE_ID,
                       RRM_ZERO,
                       RRM_ZERO);

               break;
           }
           /* MRO changes end */
           /* RACH_OPTIMIZATION_CHANGES_START */ 
       case RRMUEM_RMIF_RACH_REPORT:
           {
               p_in_rach_report = (rrmuem_rmif_ue_rach_report_t *)
                   ((U8*)p_api + RRM_API_HEADER_SIZE);

               ret_val = copy_global_cell_id_from_ctxt(
                       p_gl_ctxt,
                       &(p_out_ue_rach_report->cell_id),
                       p_in_rach_report->cell_index);

               if (RRM_FAILURE == ret_val)
               {
                   RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                           RRM_ERROR, "Incorrect Mif Context for Cell %d ",
                           p_in_rach_report->cell_index);
               }
               else
               {
                   p_out_ue_rach_report->count = p_in_rach_report->count;
                   for(rach_count = RRM_ZERO; rach_count < 
                           p_in_rach_report->count; rach_count++)
                   {
                       p_out_ue_rach_report->rach_data[rach_count].cause = 
                           p_in_rach_report->rach_data[rach_count].cause;
                       p_out_ue_rach_report->rach_data[rach_count].number_of_preambles_sent =
                           p_in_rach_report->rach_data[rach_count].number_of_preambles_sent;
                       p_out_ue_rach_report->rach_data[rach_count].contention_detected = 
                           p_in_rach_report->rach_data[rach_count].contention_detected;
                       p_out_ue_rach_report->rach_data[rach_count].ue_location = 
                           p_in_rach_report->rach_data[rach_count].ue_location;
                       p_out_ue_rach_report->rach_data[rach_count].strong_cell_info_count = 
                           p_in_rach_report->rach_data[rach_count].strong_cell_info_count;

                       for(strong_cell_count = RRM_ZERO; strong_cell_count <
                               p_out_ue_rach_report->rach_data[rach_count].
                               strong_cell_info_count; strong_cell_count++)
                       {
                           p_out_ue_rach_report->rach_data[rach_count].
                               strong_cell_info[strong_cell_count].pci = 
                               p_in_rach_report->rach_data[rach_count].
                               strong_cell_info[strong_cell_count].pci;

                           p_out_ue_rach_report->rach_data[rach_count].
                               strong_cell_info[strong_cell_count].rsrp =
                               p_in_rach_report->rach_data[rach_count].
                               strong_cell_info[strong_cell_count].rsrp;
                       }
                   }

                   /* Send UE RACH REPORT to SON */
                    rrm_son_send_rrm_son_rach_ue_info_ind(   
                       p_out_ue_rach_report ,
                      RRM_MIF_MODULE_ID,
                      RRM_SON_RACH_OPT_MODULE_ID,
                      RRM_ZERO,
                      RRM_ZERO);

               }
               break;
           }
           /* RACH_OPTIMIZATION_CHANGES_END */
        /* Responses received from UEM  ENDS*/

        default:
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                    RRM_WARNING, 
                    "UEM API [%d] Received is wrong. ", api_hdr->api);
    }
    RRM_MEM_FREE(p_out_message);
    RRM_MEM_FREE(p_out_ue_rach_report);
    RRM_UT_TRACE_EXIT();

    return p_mif_context;
}


/****************************************************************************
 * Function Name  : mif_son_message_parser
 * Inputs         : 
 *      1. p_api  is not null pointer
 *      2. api_hdr is not a null pointer.
 *      3. p_gl_ctxt is not null pointer
 *      4. mif_return_value is not null pointer
 * Outputs        : mif_return_value
 * Returns        : p_mif_context pointer
 * Variables      : None
 * Description    : This function parses all the incoming API's from SON. 
 ****************************************************************************/

static rrm_mif_context* mif_son_message_parser(rrm_mif_gb_context_t *p_gl_ctxt, void *p_api,
        interface_api_header *api_hdr, mif_return_value *returnValue)
{
    rrm_mif_context                         *p_mif_context = RRM_PNULL;
    S32                                    length_read = RRM_NULL;
    U8                                     mif_ctxt_cnt = RRM_ZERO;
    U8                                     mif_ctxt_id = RRM_ZERO;
    rrm_return_et                          ret_val = RRM_FAILURE;

    rrm_son_register_req_t                  *p_in_cell_register_req_msg = RRM_PNULL;
    rrm_son_registration_res_t               son_register_resp_to_son;
    rrm_son_deregister_req_t                *p_in_cell_deregister_req_msg = RRM_PNULL;
    rrm_son_deregistration_res_t             son_deregister_resp_to_son;
    rrm_son_cell_switch_on_req_t            *p_in_cell_switch_on_req_msg = RRM_PNULL;
    rrm_son_cell_switch_on_res_t             son_cell_switch_on_resp_to_son;
    rrm_son_cell_switch_off_req_t           *p_in_cell_switch_off_req_msg = RRM_PNULL;
    rrm_son_cell_switch_off_res_t            son_cell_switch_off_resp_to_son;
    rrm_son_set_attribute_value_req_t       *p_in_cell_set_attr_req_msg = RRM_PNULL;
    rrm_son_set_attr_res_t                   son_set_attr_resp_to_son;

    /* TNL DISCOVERY : START */
    rrm_son_tnl_discovery_req_t             *p_in_tnl_discovery_req_msg = RRM_PNULL;
    rrm_son_tnl_discovery_res_t              son_tnl_discovery_res_to_son;
    /* TNL DISCOVERY : ENDS */

    /* Spr 16142 Fix Start */
    son_rrm_stop_tnl_discovery_ind_t        *p_in_stop_tnl_discovery_ind_msg = RRM_PNULL;
    /* Spr 16142 Fix End */

    /* UE MEAS CHANGES : STARTS */
    rrm_son_meas_config_req_t                *p_in_meas_config_req = RRM_PNULL;
    rrm_son_meas_config_res_t                son_meas_config_resp;

    rrm_son_meas_config_obj_remove_req_t     *p_in_meas_config_obj_remove_req = RRM_PNULL;
    rrm_son_meas_config_obj_remove_res_t     son_meas_config_obj_remove_resp;

    /* UE MEAS CHANGES : ENDS */
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrm_son_rach_config_req_t                   *p_in_rach_info_req_msg = RRM_PNULL;
    rrm_son_rach_config_resp_t                  rach_info_response_to_son;
    /* RACH_OPTIMIZATION_CHANGES_END */
    /*SPR 10329:start*/
   rrm_son_enb_config_update_ind_t             *p_in_enb_config_update_ind = RRM_PNULL;
    /*SPR 10329:end*/
            
    rrm_bool_et                              check_failed = RRM_FALSE;
    rrm_error_et                             fail_cause = RRM_NO_ERROR;

    /*coverity 14183, rel2.0.1, , typecasted to remove enum warning*/
    U16 api_id = (U16)api_hdr->api_id;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != returnValue);

    rrm_memset_rrm_son_registration_res(&son_register_resp_to_son, RRM_ZERO);
    rrm_memset_rrm_son_deregistration_res(&son_deregister_resp_to_son, RRM_ZERO);
    rrm_memset_rrm_son_cell_switch_on_res(&son_cell_switch_on_resp_to_son, RRM_ZERO);
    rrm_memset_rrm_son_cell_switch_off_res(&son_cell_switch_off_resp_to_son, RRM_ZERO);
    rrm_memset_rrm_son_set_attr_res(&son_set_attr_resp_to_son, RRM_ZERO);
    rrm_memset_rrm_son_meas_config_res(&son_meas_config_resp, RRM_ZERO);
    rrm_memset_rrm_son_tnl_discovery_res(&son_tnl_discovery_res_to_son, RRM_ZERO);   
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrm_memset_rrm_son_rach_info_res(&rach_info_response_to_son, RRM_ZERO);
    /* RACH_OPTIMIZATION_CHANGES_END */

    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
            "mif_son_message_parser: Message Received from SON interface");

    switch(api_id)
    {
        case (U16)RRM_SON_REGISTER_REQ :  /* Coverity: CID 30758 */
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_req);
#endif        
                p_in_cell_register_req_msg = (rrm_son_register_req_t *)rrm_mem_get(sizeof(rrm_son_register_req_t));

                if ( p_in_cell_register_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_register_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
                if(RRM_SUCCESS !=  rrm_parse_rrm_son_register_req( /* to be generated using pup_tool*/
                            p_in_cell_register_req_msg ,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Register message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_register_req_msg->register_req.object.id.object_id.cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                      api_hdr->transaction_id,
                                      api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_register_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            /*SPR 17777 +-*/
                            build_and_send_son_cell_register_req_to_cellm(
                                    p_mif_context,
                                    p_in_cell_register_req_msg,
                                    api_hdr->source);
                            //RRM_HEX_DUMP("RRM_SON_REGISTER_REQ ",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_register_req(p_in_cell_register_req_msg);
                            RRM_MEM_FREE(p_in_cell_register_req_msg);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_REGISTER_REQ  ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            /*rrm_son*/
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                                 &(son_register_resp_to_son.registration_resp.id.object_id.cell_id),
                            &(p_in_cell_register_req_msg->register_req.object.id.object_id.cell_id));
                    son_register_resp_to_son.registration_resp.result = RRM_FAILURE;
                    son_register_resp_to_son.registration_resp.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Registeration Resp with fail_cause(%d) to SON",
                              fail_cause);
                    rrm_son_send_rrm_son_registration_res(
                            &son_register_resp_to_son,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source, 
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_register_req_msg);
                    p_mif_context = RRM_PNULL;
                }  
                p_mif_context = RRM_PNULL;
                break;
            }

        case (U16)RRM_SON_DEREGISTER_REQ: /* Coverity: CID 30758 */
            { 
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_req);
#endif        
                p_in_cell_deregister_req_msg =  (rrm_son_deregister_req_t *)rrm_mem_get(sizeof(rrm_son_deregister_req_t));
                if ( p_in_cell_deregister_req_msg== RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_deregister_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_son_deregister_req( /* to be generated using pup_tool*/
                            p_in_cell_deregister_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Deregister message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_deregister_req_msg->deregister_req.object.id.object_id.cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                      api_hdr->transaction_id,
                                      api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_deregister_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            /*SPR 17777 +-*/
                            build_and_send_son_cell_deregister_req_to_cellm(
                                    p_mif_context,
                                    p_in_cell_deregister_req_msg,
                                    api_hdr->source);
                            //RRM_HEX_DUMP("RRM_SON_DEREGISTER_REQ ",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_deregister_req(p_in_cell_deregister_req_msg);
                            RRM_MEM_FREE(p_in_cell_deregister_req_msg);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_DEREGISTER_REQ  ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.deregister_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                                 &(son_deregister_resp_to_son.deregistration_resp.id.object_id.cell_id),
                            &(p_in_cell_deregister_req_msg->deregister_req.object.id.object_id.cell_id));
                    son_deregister_resp_to_son.deregistration_resp.result = RRM_FAILURE;
                    son_deregister_resp_to_son.deregistration_resp.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending De-Registeration Resp with fail_cause(%d) to SON",
                              fail_cause);
                    rrm_son_send_rrm_son_deregistration_res(
                            &son_deregister_resp_to_son,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source, 
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_deregister_req_msg);
                    p_mif_context = RRM_PNULL;
                }  
                p_mif_context = RRM_PNULL;
                break;
            }

        case (U16)RRM_SON_SET_ATTRIBUTE_REQ: /* Coverity: CID 30758 */
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_req);
#endif        
                p_in_cell_set_attr_req_msg =  (rrm_son_set_attribute_value_req_t *)
                    rrm_mem_get(sizeof(rrm_son_set_attribute_value_req_t));
                if ( p_in_cell_set_attr_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_set_attr_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_son_set_attribute_value_req( /* to be generated using pup_tool*/
                            p_in_cell_set_attr_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                            "Bad Cell Set Attributes message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                }
                else
                {
                    /* SPR 20653 Fix Start */
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_set_attr_req_msg->object.id.object_id.cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                    /* SPR 20653 Fix End */
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_resp_F_inv_param);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                      api_hdr->transaction_id,
                                      api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
#ifdef RRM_UT_FLAG        
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_resp_F);
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_resp_F_cell_unonfig);
#endif        
                            }
                            else
                            {
                                /* Storing attribute Id in MIF Ctxt. Used in case of response*/
                                /* SPR 20653 Fix Start */
                                /* Code Removed */
                                /* SPR 20653 Fix End */
                                build_and_send_son_cell_set_attributes_req_to_cellm( 
                                        /*SPR 17777 +-*/
                                        p_mif_context,
                                        p_in_cell_set_attr_req_msg,
                                        api_hdr->source);
                                //RRM_HEX_DUMP("RRM_SON_SET_ATTRIBUTE_REQ ",p_api,api_hdr->msg_size,p_mif_context);
                                //rrm_print_cell_set_attributes_req(p_in_cell_set_attr_req_msg);
                                RRM_MEM_FREE(p_in_cell_set_attr_req_msg);
                            }
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_SET_ATTRIBUTE_REQ  ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.set_attribute_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    /* SPR 20653 Fix Start */
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                 &(son_set_attr_resp_to_son.set_attr_resp.id.object_id.cell_id),
                            &(p_in_cell_set_attr_req_msg->object.id.object_id.cell_id));
                    son_set_attr_resp_to_son.set_attr_resp.result = RRM_FAILURE;
                    son_set_attr_resp_to_son.set_attr_resp.error_code = fail_cause;
                    son_set_attr_resp_to_son.id = p_in_cell_set_attr_req_msg->object.p_attr_lst[p_in_cell_set_attr_req_msg->object.attr_list_size].id;
                    /* SPR 20653 Fix End */
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Set Attribute Resp with fail_cause(%d) to SON",
                              fail_cause);
                    rrm_son_send_rrm_son_set_attr_res(
                            &son_set_attr_resp_to_son,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source, 
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_set_attr_req_msg);
                    p_mif_context = RRM_PNULL;
                }  
                p_mif_context = RRM_PNULL;
                break;
            }

        case (U16)RRM_SON_CELL_SWITCH_ON_REQ: /* Coverity: CID 30758 */
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_req);
#endif        
                p_in_cell_switch_on_req_msg =  (rrm_son_cell_switch_on_req_t *)rrm_mem_get(sizeof(rrm_son_cell_switch_on_req_t));
                if ( p_in_cell_switch_on_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_switch_on_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_son_cell_switch_on_req( /* to be generated using pup_tool*/
                            p_in_cell_switch_on_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Switch On message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_switch_on_req_msg->cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                      api_hdr->transaction_id,
                                      api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_switch_on_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_SWITCH_ON_REQ;
                            returnValue->p_msg_info = (U8 *)p_in_cell_switch_on_req_msg;
                            //RRM_HEX_DUMP("RRM_SON_CELL_SWITCH_ON_REQ",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_switch_on_req(p_in_cell_switch_on_req_msg);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_CELL_SWITCH_ON_REQ ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                            &(son_cell_switch_on_resp_to_son.cell_switchon_res.cell_id),
                            &(p_in_cell_switch_on_req_msg->cell_id));
                    son_cell_switch_on_resp_to_son.cell_switchon_res.result = RRM_FAILURE;
                    son_cell_switch_on_resp_to_son.cell_switchon_res.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Switch On Resp with fail_cause(%d) to SON",
                              fail_cause);
                    rrm_son_send_rrm_son_cell_switch_on_res(
                            &son_cell_switch_on_resp_to_son,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source, 
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_switch_on_req_msg);
                    p_mif_context = RRM_PNULL;
                }  
                break;
            }

        case (U16)RRM_SON_CELL_SWITCH_OFF_REQ: /* Coverity: CID 30758 */
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_req);
#endif        
                p_in_cell_switch_off_req_msg =  (rrm_son_cell_switch_off_req_t *)rrm_mem_get(sizeof(rrm_son_cell_switch_off_req_t));
                if ( p_in_cell_switch_off_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_switch_off_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_son_cell_switch_off_req( /* to be generated using pup_tool*/
                            p_in_cell_switch_off_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Switch Off message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_switch_off_req_msg->cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                      api_hdr->transaction_id,
                                      api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_switch_off_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ;
                            returnValue->p_msg_info = (U8 *)p_in_cell_switch_off_req_msg;
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_CELL_SWITCH_OFF_REQ ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                            &(son_cell_switch_off_resp_to_son.cell_switchoff_res.cell_id),
                            &(p_in_cell_switch_off_req_msg->cell_id));
                    son_cell_switch_off_resp_to_son.cell_switchoff_res.result = RRM_FAILURE;
                    son_cell_switch_off_resp_to_son.cell_switchoff_res.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Switch Off Resp with fail_cause(%d) to SON",
                              fail_cause);
                    rrm_son_send_rrm_son_cell_switch_off_res(
                            &son_cell_switch_off_resp_to_son,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source, 
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_switch_off_req_msg);
                    p_mif_context = RRM_PNULL;
                }  
                break;
            }
            
        /* UE MEAS CHANGES : STARTS */    
        case (U16)RRM_SON_MEAS_CONFIG_REQ: /* Coverity: CID 30758 */
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_req);
#endif        
                p_in_meas_config_req =  (rrm_son_meas_config_req_t *)
                    rrm_mem_get(sizeof(rrm_son_meas_config_req_t));
                    
                if ( p_in_meas_config_req == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_meas_config_req failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                /* to be generated using pup_tool*/
                if(RRM_SUCCESS !=  rrm_parse_rrm_son_meas_config_req( 
                            p_in_meas_config_req,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                        RRM_ERROR, "Bad UE Meas Config Req message - ignore.");

                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_meas_config_req->cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                        RRM_ERROR, "Incorrect Mif Context ");
                        
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F_cell_unconfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, 
                                                                mif_ctxt_id);
                        if (RRM_PNULL != p_mif_context)
                        {
                           ret_val = allocate_and_store_tcb_info_in_mif_context(
                                            p_mif_context,
                                            /* SPR_19279_START */
                                            RRM_NULL,
                                            RRM_ZERO,
                                            /* SPR_19279_END */
                                            api_hdr->transaction_id,
                                            api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, 
                                          p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "TCB cant be allocated");

                                RRM_MEM_FREE(p_in_meas_config_req);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            /*SPR 17777 +-*/
                            build_and_send_son_ue_meas_config_req_to_cellm(
                                    p_mif_context,
                                    p_in_meas_config_req);
                            /*SPR 17777 +-*/
                            //RRM_HEX_DUMP("RRM_SON_REGISTER_REQ ",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_register_req(p_in_cell_register_req_msg);
                            RRM_MEM_FREE(p_in_meas_config_req);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_MEAS_CONFIG_REQ  ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.meas_config_resp_F_cell_unconfig);
#endif        
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                            &(son_meas_config_resp.meas_config_res.cell_id),
                            &(p_in_meas_config_req->cell_id));
                    son_meas_config_resp.meas_config_res.result = RRM_FAILURE;
                    son_meas_config_resp.meas_config_res.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                          RRM_BRIEF,"Failure: Sending MEAS CONFIG RESP with"
                          " fail_cause(%d) to SON", fail_cause);
                    
                    /* Send Message to SON  */
                    rrm_son_send_rrm_son_meas_config_res(
                            &son_meas_config_resp,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source, 
                            (U16)(rrm_get_word_from_header(
                                &(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                                rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));

                    RRM_MEM_FREE(p_in_meas_config_req);

                    p_mif_context = RRM_PNULL;
                } 
                
                break;
            }
        case (U16)RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ: /* Coverity: CID 30758 */
            {
                p_in_meas_config_obj_remove_req =  (rrm_son_meas_config_obj_remove_req_t *)
                    rrm_mem_get(sizeof(rrm_son_meas_config_obj_remove_req_t));

                if ( p_in_meas_config_obj_remove_req == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_meas_config_obj_remove_req failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_son_meas_config_obj_remove_req(
                            p_in_meas_config_obj_remove_req,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                        RRM_ERROR, "Bad UE Meas Config Obj Remove Req message - ignore.");

                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt,
                            &(p_in_meas_config_obj_remove_req->cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                        RRM_ERROR, "Incorrect Mif Context ");

                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt,
                                                                mif_ctxt_id);
                        if (RRM_PNULL != p_mif_context)
                        {
                           ret_val = allocate_and_store_tcb_info_in_mif_context(
                                            p_mif_context,
                                            /* SPR_19279_START */
                                            RRM_NULL,
                                            RRM_ZERO,
                                            /* SPR_19279_END */
                                            api_hdr->transaction_id,
                                            api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off,
                                          p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "TCB cant be allocated");

                                RRM_MEM_FREE(p_in_meas_config_obj_remove_req);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            /*SPR 17777 +-*/
                            build_and_send_son_ue_meas_config_obj_remove_req_to_cellm(
                                    p_mif_context,
                                    p_in_meas_config_obj_remove_req);
                            /*SPR 17777 +-*/
                            RRM_MEM_FREE(p_in_meas_config_obj_remove_req);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                                    RRM_ERROR,
                                    "MIF Context not found in received RRM_SON_MEAS_CONFIG_REQ  ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        }
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    RRM_MEMCPY(&(son_meas_config_obj_remove_resp.meas_config_obj_remove_res.cell_id),
                            &(p_in_meas_config_obj_remove_req->cell_id),
                            sizeof(rrm_oam_eutran_global_cell_id_t));
                    son_meas_config_obj_remove_resp.meas_config_obj_remove_res.result = RRM_FAILURE;
                    son_meas_config_obj_remove_resp.meas_config_obj_remove_res.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                          RRM_BRIEF,"Failure: Sending MEAS CONFIG OBJ REMOVE RESP with"
                          " fail_cause(%d) to SON", fail_cause);

                    /* Send Message to SON  */
                    rrm_son_send_rrm_son_meas_config_obj_remove_res(
                            &son_meas_config_obj_remove_resp,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source,
                            (U16)(rrm_get_word_from_header(
                                &(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                                rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));

                    RRM_MEM_FREE(p_in_meas_config_obj_remove_req);

                    p_mif_context = RRM_PNULL;
                }

                break;
            }
        /* UE MEAS CHANGES : ENDS */    

            /* TNL DISCOVERY : START */
        case (U16)RRM_SON_TNL_DISCOVERY_REQ: /* Coverity: CID 30758 */
            {
                p_in_tnl_discovery_req_msg = (rrm_son_tnl_discovery_req_t *)
                    rrm_mem_get(sizeof(rrm_son_tnl_discovery_req_t));

                if ( p_in_tnl_discovery_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_tnl_discovery_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_son_tnl_discovery_req(
                            p_in_tnl_discovery_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                            RRM_ERROR, "Bad TNL discovery request message - ignore.");

                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                }
                else
                {
                    build_and_send_son_tnl_discovery_to_cellm(
                            api_hdr->transaction_id,
                            p_in_tnl_discovery_req_msg);
                    /*SPR 17777 +-*/
                    RRM_MEM_FREE(p_in_tnl_discovery_req_msg);
                }
                if(RRM_TRUE == check_failed)
                {
                    son_tnl_discovery_res_to_son.result = RRM_FAILURE;
                    son_tnl_discovery_res_to_son.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                            RRM_BRIEF,"Failure: Sending TNL DISCOVERY RESP with"
                            " fail_cause(%d) to SON", fail_cause);

                    /* Send Message to SON  */
                    rrm_son_send_rrm_son_tnl_discovery_res(
                            &son_tnl_discovery_res_to_son,
                            RRM_MIF_MODULE_ID,
                            api_hdr->source,
                            (U16)(rrm_get_word_from_header(
                                                           &(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));

                    RRM_MEM_FREE(p_in_tnl_discovery_req_msg);
                }    
                break;
            }  

            /* Spr 16142 Fix Start */
        case (U16)SON_RRM_STOP_TNL_DISCOVERY_IND:
            {
                p_in_stop_tnl_discovery_ind_msg = (son_rrm_stop_tnl_discovery_ind_t *)
                    rrm_mem_get(sizeof(son_rrm_stop_tnl_discovery_ind_t));

                if ( p_in_stop_tnl_discovery_ind_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
                            "Memory allocation to p_in_stop_tnl_discovery_ind_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_son_rrm_stop_tnl_discovery_ind(
                            p_in_stop_tnl_discovery_ind_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name,
                            RRM_ERROR, "Bad Msg : STOP TNL discovery Ind - ignore.");

                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                    /*coverity 87531 fix start*/
                    RRM_MEM_FREE(p_in_stop_tnl_discovery_ind_msg);
                    /*coverity 87531 fix end*/
                }
                else
                {
                    build_and_send_son_stop_tnl_discovery_to_cellm(
                            p_in_stop_tnl_discovery_ind_msg);

                    RRM_MEM_FREE(p_in_stop_tnl_discovery_ind_msg);
                }
                break;
            }
            /* Spr 16142 Fix End */

            /* UPDATED NRT INFO CHANGES START */ 
        case (U16)SON_ANR_RRM_UPDATED_NRT_INFO: /* Coverity: CID 30758 */
            {
                son_anr_rrm_updated_nrt_info_t* p_in_updated_nrt_info_req_msg;
                p_in_updated_nrt_info_req_msg =  (son_anr_rrm_updated_nrt_info_t*)rrm_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
                if ( p_in_updated_nrt_info_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_updated_nrt_info_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_son_anr_rrm_updated_nrt_info( /* to be generated using pup_tool*/
                            p_in_updated_nrt_info_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    /* Coverity 74709 Fix Start */
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                            RRM_ERROR, "Bad SON RRM Updated NRT Info message -"
                            "ignore fail cause[%d]",fail_cause);
                    /* Coverity 74709 Fix End */
                    RRM_MEM_FREE(p_in_updated_nrt_info_req_msg);
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            (rrm_oam_eutran_global_cell_id_t*)&(p_in_updated_nrt_info_req_msg->src_cgi),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                       /* Coverity 74709 Fix Start */
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                "Incorrect Mif Context fail cause[%d]",
                                fail_cause);
                       /* Coverity 74709 Fix End */
                        RRM_MEM_FREE(p_in_updated_nrt_info_req_msg);
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            /*SPR 17777 +-*/
                            build_and_send_son_updated_nrt_info_to_cellm(
                                    p_mif_context,
                                    p_in_updated_nrt_info_req_msg,
                                    api_hdr->source);
                            //RRM_HEX_DUMP("SON_ANR_RRM_UPDATED_NRT_INFO ", p_api, api_hdr->msg_size, p_mif_context);
                            //rrm_print_updated_nrt_info(p_in_updated_nrt_info_req_msg);
                            RRM_MEM_FREE(p_in_updated_nrt_info_req_msg);
                        }
                        else
                        {
                           /* Coverity 74709 Fix Start */
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received"
                                    "SON_ANR_RRM_UPDATED_NRT_INFO fail"
                                    "cause[%d]",fail_cause);
                           /* Coverity 74709 Fix End */
                            RRM_MEM_FREE(p_in_updated_nrt_info_req_msg);
                        }
                    }
                }
                break;
            }
            /* UPDATED NRT INFO CHANGES END */ 

            /* UPDATED TTT IND CHANGES START */
        case (U16)SON_RRM_TTT_UPDATE_IND: /* Coverity: CID 30758 */
            {
                son_rrm_ttt_update_ind_t* p_in_son_rrm_ttt_update_ind;
                p_in_son_rrm_ttt_update_ind = (son_rrm_ttt_update_ind_t *)rrm_mem_get(sizeof(son_rrm_ttt_update_ind_t));
                if(p_in_son_rrm_ttt_update_ind != RRM_PNULL)
				{
					if(RRM_SUCCESS != rrm_parse_son_rrm_ttt_update_ind( /* to be generated using pup_tool*/
											p_in_son_rrm_ttt_update_ind,
											(U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
											api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
											&length_read))
					{
                      /* Coverity 74709 Fix Start */
							fail_cause = RRM_ERR_INVALID_PARAMS;
							RRM_TRACE(g_mif_log_on_off,
                                    p_g_rrm_mif_facility_name, RRM_ERROR, "Bad"
                                    "SON_RRM_TTT_UPDATE_IND Info message -"
                                    "ignore. fail cause[%d]",fail_cause);
                      /* Coverity 74709 Fix End - Removed check_failed*/
					}
					else
                    {
                        ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt,
                                (rrm_oam_eutran_global_cell_id_t*)&(p_in_son_rrm_ttt_update_ind->srv_cgi),
                                &mif_ctxt_cnt,
                                &mif_ctxt_id);

                        if (RRM_FAILURE == ret_val)
                        {
                            /* Coverity 74709 Fix Start */
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                    "Incorrect Mif Context fail cause[%d]",
                                    fail_cause);
                            /* Coverity 74709 Fix End */
                        }
                        else
                        {
                            p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                            if (RRM_PNULL != p_mif_context)
                            {
                                if(RRM_SUCCESS == build_and_send_son_ttt_update_ind_to_cellm(p_gl_ctxt,
                                            p_mif_context,
                                            p_in_son_rrm_ttt_update_ind,
                                            api_hdr->source))
                                {
                                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, 
                                            "SON_RRM_TTT_UPDATE_IND msg sent to CELLM ");
                                }
                                else
                                {
                                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                            "Failed to send SON_RRM_TTT_UPDATE_IND msg to CELLM");
                                }
                            }
                            else
                            {
                               /* Coverity 74709 Fix Start */
                                fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_ERROR, 
                                        "MIF Context not found in received "
                                        "SON_RRM_TTT_UPDATE_IND fail"
                                        "cause[%d]",fail_cause);
                               /* Coverity 74709 Fix End */
                            }

                        }
                    }
						RRM_MEM_FREE(p_in_son_rrm_ttt_update_ind);
				}
				else
				{
					RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
								RRM_ERROR, 
								"SON_RRM_TTT_UPDATE_IND memory allocation failure occured ");
                   /* Coverity 74709 Fix Start */
                   /* Coverity 74709 Fix End */
					return RRM_PNULL;
				}
				break;
            }

            /* RACH_OPTIMIZATION_CHANGES_START */
        case (U16)RRM_SON_RACH_CONFIG_REQ : /* Coverity: CID 30758 */
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.rach_info_req);
#endif        
                p_in_rach_info_req_msg = (rrm_son_rach_config_req_t *)
                    rrm_mem_get(sizeof(rrm_son_rach_config_req_t));


                if ( p_in_rach_info_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_rach_info_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
                if(RRM_SUCCESS !=  rrm_parse_rrm_son_rach_config_req( 
                                 p_in_rach_info_req_msg ,
                                 (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                                 api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                                 &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                            RRM_ERROR, "Bad Cell Register message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_RACH_CONFIG_FAILURE;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.rach_info_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_rach_info_req_msg->cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);

                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.rach_info_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            if((p_in_rach_info_req_msg->bitmask & UE_REPORT_PERIODICITY_PRESENT) ||
                                    (p_in_rach_info_req_msg->bitmask & RACH_SAMPLE_COUNT_PRESENT))
                            {
                                build_and_send_rach_info_req_to_uem(
                                        p_mif_context,
                                        p_in_rach_info_req_msg);
                                        /*SPR 17777 +-*/
                            }

                            if(p_in_rach_info_req_msg->bitmask & L2_REPORT_PERIODICITY_PRESENT)
                            {
                                build_and_send_rach_info_req_to_cellm(p_mif_context,
                                        p_in_rach_info_req_msg);
                                        /*SPR 17777 +-*/
                            }

                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received RRM_SON_RACH_CONFIG_REQ  ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.rach_info_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.register_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }
                rrm_memcpy_rrm_oam_eutran_global_cell_id(
                        &(rach_info_response_to_son.cell_id),
                        &(p_in_rach_info_req_msg->cell_id));
                if(RRM_TRUE == check_failed)
                {
                    rach_info_response_to_son.result = RRM_FAILURE;
                    rach_info_response_to_son.error_code = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                            RRM_BRIEF, 
                            "Failure: Sending Rach Info Resp with fail_cause(%d) to SON",
                            fail_cause);
                }
                else
                {

                    rach_info_response_to_son.result = RRM_SUCCESS;
                    rach_info_response_to_son.error_code = RRM_NO_ERROR;
                }
                rrm_son_send_rrm_son_rach_config_resp(
                        &rach_info_response_to_son,
                        RRM_MIF_MODULE_ID,
                        api_hdr->source, 
                        (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                        rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));

                RRM_MEM_FREE(p_in_rach_info_req_msg);
                p_mif_context = RRM_PNULL;
                /* Coverity 74054 Fix Start */
                /* Coverity 74054 Fix End */

                break;
            }
            /* RACH_OPTIMIZATION_CHANGES_END */

        /*SPR 10329:start*/
       case (U16)RRM_SON_ENB_CONFIG_UPDATE_IND : 
            {
                p_in_enb_config_update_ind = (rrm_son_enb_config_update_ind_t *)
                    rrm_mem_get(sizeof(rrm_son_enb_config_update_ind_t));


                if ( p_in_enb_config_update_ind != RRM_PNULL)
                {
                    /*10329_START*/ 
                    if(RRM_SUCCESS != rrm_parse_rrm_son_enb_config_update_ind(
                                /*10329_END*/ 
                                p_in_enb_config_update_ind,
                                (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                                api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                                &length_read)) 
                    {
                        /* Coverity 74709 Fix Start */
                        fail_cause = RRM_ERR_INVALID_PARAMS;
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                                "Bad SON_RRM_ENB_CONFIG_UPDATE_IND message-"
                                "ignored :fail cause[%d]",fail_cause);
                        /* Coverity 74709 Fix End */


                        RRM_MEM_FREE(p_in_enb_config_update_ind);
                    }
                    else
                    { 
                        if(p_gl_ctxt->current_state == RMIF_STATE_ACTIVE)
                        {
                            build_and_send_son_enb_config_update_ind_to_cellm( p_in_enb_config_update_ind);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                                    "MIF is in %d STATE : SON_RRM_ENB_CONFIG_UPDATE_IND message -ignored",
                                    p_gl_ctxt->current_state);
                        }

                        RRM_MEM_FREE(p_in_enb_config_update_ind);     
                    }

                }
                else
                {
                    /* Coverity 74709 Fix Start */
                    fail_cause = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_enb_config_update_ind failed"
                            " fail cause[%d]",fail_cause);
                    /* Coverity 74709 Fix End */

                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
                break;

            }
        /*SPR 10329:end*/

        default:
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                    RRM_WARNING, 
                    "SON API [%d] Received is wrong. ", api_hdr->api_id);
    }
    RRM_UT_TRACE_EXIT();
    return p_mif_context;
}

/*spr 22474 start*/
/****************************************************************************
 **  * Function Name  : rrm_prepare_memPool_stats
 **  * Inputs         : pointer to buffer
 **  * Outputs        : prepares the mempool stats for OAM
 **  * Returns        : None
 **  * Description    : This function updates the stats of mempool
 **  ****************************************************************************/

void
rrm_prepare_memPool_stats
(
    rrm_MemPoolStats *csplMemPoolStats_p
)
{
    UInt32 allocBufMemPool = 0, nbufMemPool = 0, peakBufMemPool = 0;
    UInt32 poolIndex = 0, bufSize = 0, validPool = 0;

    /* Get the mempool stats */
    for( poolIndex = 0,bufSize = MINBUF; poolIndex < NVARMEMPOOL; poolIndex++, bufSize *= 2)
    {
        allocBufMemPool = nbufMemPool = peakBufMemPool = 0;
        nbufMemPool = getMemPoolStatsInShm(poolIndex, &allocBufMemPool, &peakBufMemPool );
        /* Send only pools which has non zero total buffers */
        if (nbufMemPool)
        {
            csplMemPoolStats_p->stats[validPool].bufSize = bufSize;
            csplMemPoolStats_p->stats[validPool].numOfBuf = nbufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfAllocBuf = allocBufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfPeakBuf = peakBufMemPool;
            validPool++;
        }
    }
    /* Set the end marker as bufSize to 0 so that OAM is aware where the status ends in array */
    if (validPool < NVARMEMPOOL)
    {
        csplMemPoolStats_p->stats[validPool].bufSize = 0;
    }
    return;
}

/*spr 22474 end*/

/****************************************************************************
 * Function Name  : mif_oam_message_parser
 * Inputs         : 
 *      1. p_api  is not null pointer
 *      2. api_hdr is not a null pointer.
 *      3. p_gl_ctxt is not null pointer
 *      4. mif_return_value is not null pointer
 * Outputs        : mif_return_value
 * Returns        : p_mif_context pointer
 * Variables      : None
 * Description    : This function parses all the incoming API's from OAM. In this
 *                  function NULL check is not applied on p_mif_context as we are 
 *                  expecting the NULL value in case OAM requests for that Cells
 *                  whose Cell Context is not created yet.
 ****************************************************************************/

static rrm_mif_context* mif_oam_message_parser(rrm_mif_gb_context_t *p_gl_ctxt, void *p_api,
        interface_api_header *api_hdr, mif_return_value *returnValue)
{
    rrm_mif_context                       *p_mif_context = RRM_PNULL;
    U8                                     mifContextCount = RRM_ZERO;
    S32                                    length_read = RRM_NULL;
    U8                                     mif_ctxt_cnt = RRM_ZERO;
    U8                                     mif_ctxt_id = RRM_ZERO;
    U8                                     mifCtxtCount = RRM_ZERO;
    U8                                     cell_count = RRM_ZERO;
    rrm_return_et                          ret_val = RRM_FAILURE;
    rrm_oam_cell_config_req_t             *p_in_cell_config_req_msg = RRM_PNULL;
    rrm_oam_cell_config_resp_t             cell_config_resp_to_oam;
    rrm_oam_cell_reconfig_req_t           *p_in_cell_reconfig_req_msg = RRM_PNULL;
    rrm_oam_cell_reconfig_resp_t           cell_reconfig_resp_to_oam;
    rrm_oam_cell_delete_req_t             *p_in_cell_delete_req_msg = RRM_PNULL;
    rrm_oam_delete_resp_t                  cell_delete_resp_to_oam;
    rrm_oam_shutdown_req_t                *p_in_cell_shutdown_req_msg = RRM_PNULL;
  /* coverity id 16522 , ,rel 2.0.1 */
    rrm_oam_shutdown_resp_t                cell_shutdown_resp_to_oam = {RRM_INDEX_ERR,RRM_NO_ERROR};
    rrm_oam_cell_start_req_t              *p_in_cell_start_req_msg = RRM_PNULL;
    rrm_oam_cell_start_resp_t              cell_start_resp_to_oam;
    rrm_oam_cell_stop_req_t               *p_in_cell_stop_req_msg = RRM_PNULL;
    rrm_oam_cell_stop_resp_t              cell_stop_resp_to_oam;
    rrm_oam_rac_enable_disable_req_t      *p_rac_enable_disable_req_msg = RRM_PNULL;
    rrm_oam_rac_enable_disable_resp_t      rac_enable_disable_resp_to_oam = {RRM_ZERO};
  /* coverity id 16519 , ,rel 2.0.1 */
    rrm_oam_resume_service_resp_t          resume_service_resp_to_oam = {RRM_INDEX_ERR,RRM_NO_ERROR};
    rrm_oam_cell_context_print_req_t      *p_in_cell_context_print_req_msg = RRM_PNULL;
    rrm_oam_cell_block_req_t              *p_in_block_cell_req_msg = RRM_PNULL;
    rrm_oam_cell_block_resp_t              block_cell_resp_to_oam;
    rrm_bool_et                            check_failed = RRM_FALSE;
    rrm_error_et                           fail_cause = RRM_NO_ERROR;
    rrm_oam_cell_unblock_cmd_t            *p_rrm_oam_unblock_cell_cmd=RRM_PNULL;
    /*Cell update start*/
    rrm_oam_cell_update_req_t             *p_rrm_oam_cell_update_req = RRM_PNULL;
    rrm_oam_cell_update_resp_t             rrm_oam_cell_update_resp;
    /*Cell update end*/
    U8                                     store_mif_ctxt[RRM_MAX_NUM_CELLS] = {RRM_ZERO}; 
    /*ECN start*/
    /*SPR 14132 start*/
     U32                                   dl_sinr_threshold_low_mark;
     U32                                   dl_sinr_threshold_high_mark;
     U8                                    abs_usage_high_threshold;
     U8                                    abs_usage_low_threshold;
    /*SPR 14132 end*/
    rrm_oam_cell_ecn_capacity_enhance_req_t  *p_in_cell_ecn_capa_enhance_req = RRM_PNULL;
    /* coverity id 29721 , ,rel 2.0.1 */
    rrm_oam_cell_ecn_capacity_enhance_resp_t p_out_cell_ecn_capa_enhance_resp={RRM_INDEX_ERR,RRM_NO_ERROR};
    /*ECN_end*/
    platform_rrm_load_ind_t                 *p_in_platform_ind = RRM_PNULL;
    rrm_oam_get_kpi_req_t                   *p_rrm_oam_get_kpi_req = RRM_PNULL;
    rrm_oam_get_kpi_resp_t                  rrm_oam_get_kpi_resp = {RRM_ZERO};
    rrm_oam_config_kpi_req_t                *p_in_oam_config_kpi_req = RRM_PNULL;
    rrm_oam_config_kpi_resp_t               rrm_oam_config_kpi_resp= {RRM_ZERO};
    U8                                      index = RRM_ZERO;
    /* SPR 20653 Fix Start */
    /* Code Removed */
    /* SPR 20653 Fix End */
    /* cell_index_from_api_header_start */
    rrm_cell_index_t                        cell_index = RRM_INVALID_CELL_ID;
    /* cell_index_from_api_header_end */

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != api_hdr);
    RRM_ASSERT (RRM_PNULL != returnValue);

    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
            "mif_oam_message_parser: Message Received from OAM interface");

    switch(api_hdr->api_id)
    {
        case RRM_OAM_CELL_CONFIG_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_req);
#endif        

                /* This memory will get free in mif_raise_event_to_fsm */
                p_in_cell_config_req_msg = (rrm_oam_cell_config_req_t *)rrm_mem_get(
                        sizeof(rrm_oam_cell_config_req_t));

                if ( p_in_cell_config_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_config_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }   
                if (RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_config_req( /* to be generated using pup_tool*/
                            p_in_cell_config_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            (S32)(api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE),
                            &length_read))
                {
                    /* Raise Event - RRM_EVENT_LOCAL_CELL_SETUP_REJECT START */
                    rrm_raise_event_cell_setup_reject(RRM_EVENT_CELL_SETUP_REJ_INVALID_PARAMS);
                    /* Raise Event - RRM_EVENT_LOCAL_CELL_SETUP_REJECT END*/

                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad  Cell Config message - ignore.");
                    /* Cant send Config Response to OAM as Global Id is not present*/
                    RRM_UT_TRACE_EXIT();
                    /* COVERITY : RESOURCE LEAK : CID : 54388 fix start */
                    RRM_MEM_FREE(p_in_cell_config_req_msg);
                    /* COVERITY : RESOURCE LEAK : CID : 54388 fix end*/
                    return p_mif_context;
                }
                /* cell_index_from_api_header_start */
                cell_index = rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE);
                if ((RRM_INVALID_CELL_ID == cell_index) || (cell_index >= RRM_MAX_NUM_CELLS))
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "cell_config_req:Cell index allocation failed :%d",
                            cell_index);
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F_indx_alloc_failed);
#endif        
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_CELL_ID;
                }    
                /* cell_index_from_api_header_end */
                /* Check if This request is received for same global cell Id or not */
                if (check_failed == RRM_FALSE)
                {
                    for (mifContextCount = RRM_ZERO; mifContextCount < MAX_NUM_MIF_CTXT; mifContextCount++)
                    {
                        if (RRM_TRUE == p_gl_ctxt->map_info[mifContextCount].is_info_valid)
                        {
                            ret_val = compare_global_cell_id(&(p_in_cell_config_req_msg->global_cell_info.eutran_global_cell_id),
                                    &(p_gl_ctxt->map_info[mifContextCount].global_cell_id)
                                    );

                            if(RRM_SUCCESS == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_WARNING, "This request is received for duplicate global cell Id found at Idx[%d]", mifContextCount);
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_CELL_ALREADY_CONFIGURED;
#ifdef RRM_UT_FLAG        
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F);
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F_cell_already_config);
#endif        
                                break;
                            }
                            /* SPR 11002 local fix*/
                            if (p_gl_ctxt->map_info[mifContextCount].cell_index == cell_index)
                            {
                                /* This index is already allocated */
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_WARNING, "Cell_index:%d is already configured ",cell_index);
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_CELL_ALREADY_CONFIGURED;
                            }
                        }
                    }
                }

                if (check_failed == RRM_FALSE)
                {
                    /* SPR 20653 Fix Start */
                    /* Code Removed */
                    /* SPR 20653 Fix End */

                    mifContextCount = rrm_num_pool_block(p_gl_ctxt);
                    if(mifContextCount < MAX_NUM_MIF_CTXT)
                    {
                        returnValue->fsm_index = MIF_FSM;
                        returnValue->event_id = MIF_FSM_EVENT_CELL_CONFIG_REQUEST;
                        returnValue->p_msg_info = (U8 *)p_in_cell_config_req_msg;

                        p_mif_context = rrm_alloc_mif_context(p_gl_ctxt);
                        RRM_ASSERT (RRM_PNULL != p_mif_context);

                        ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                /* SPR_19279_START */
                                RRM_NULL,
                                RRM_ZERO,
                                /* SPR_19279_END */
                                api_hdr->transaction_id,
                                api_hdr->source);
                        if (RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, "mif_oam_message_parser: Allocation of TCB Failed");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F_sys_mem_alloc);
#endif        
                        }
                        else
                        {
                            /* cell_index_from_api_header_start */
                            p_mif_context->cell_index = cell_index;
                            /* cell_index_from_api_header_end */
                            /* eICIC_PHASE_1_2_CHANGES_START */
                            if((p_in_cell_config_req_msg->operator_info.bitmask & RRM_OAM_EICIC_INFO_PRESENT) &&
                                    (p_in_cell_config_req_msg->operator_info.eicic_info.bitmask & RRM_OAM_EICIC_PROVISIONED_TYPE_PRESENT))
                            {
                                p_mif_context->eicic_prov_type = p_in_cell_config_req_msg->operator_info.eicic_info.eicic_provisioned_type ;
                            }
                            else
                            {
                                p_mif_context->eicic_prov_type = RRM_OAM_EICIC_DEACTIVATED; 
                            }
                            /* eICIC_PHASE_1_2_CHANGES_END */
                            for(mifCtxtCount = RRM_ZERO; mifCtxtCount < MAX_NUM_MIF_CTXT; mifCtxtCount++)
                            {
                                if (RRM_FALSE == p_gl_ctxt->map_info[mifCtxtCount].is_info_valid)
                                {
                                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                                            &(p_gl_ctxt->map_info[mifCtxtCount].global_cell_id),
                                            &(p_in_cell_config_req_msg->global_cell_info.eutran_global_cell_id));

                                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                            RRM_DETAILED, "Allocation MIF Id[%d], Idx[%d]", p_mif_context->mif_ctxt_id, mifCtxtCount);
                                    p_gl_ctxt->map_info[mifCtxtCount].is_info_valid = RRM_TRUE;
                                    p_gl_ctxt->map_info[mifCtxtCount].mif_ctxt_id = p_mif_context->mif_ctxt_id;
                                    /* SPR 10809 fix start*/
                                    p_gl_ctxt->map_info[mifCtxtCount].cell_index = cell_index;
                                    /* SPR 10809 fix end */
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, "mif_oam_message_parser: Max Number of Context Reached");
                        check_failed = RRM_TRUE; 
                        fail_cause = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F_sys_mem_alloc);
#endif        
                    }

                    /* SPR 20653 Fix Start */
                    /* Code removed */
                    /* SPR 20653 Fix End */
                }						
                /*SPR 14132 start*/
                if( RRM_FALSE == check_failed)
                {
                  if(!(RRM_OAM_DL_SINR_THRESHOLD_LOW_MARK_PRESENT & p_in_cell_config_req_msg->operator_info.eicic_info.bitmask))
                  {
                     dl_sinr_threshold_low_mark = RRM_DL_SINR_THERSHOLD_DEFAULT_LOW_VALUE;    
                  }
                  else
                  {
                    dl_sinr_threshold_low_mark = p_in_cell_config_req_msg->operator_info.eicic_info.dl_sinr_threshold_low_mark; 
                  }    
                  if(!(RRM_OAM_DL_SINR_THRESHOLD_HIGH_MARK_PRESENT & p_in_cell_config_req_msg->operator_info.eicic_info.bitmask))
                  {
                     dl_sinr_threshold_high_mark = RRM_DL_SINR_THERSHOLD_DEFAULT_HIGH_VALUE;   
                  }
                  else
                  {
                     dl_sinr_threshold_high_mark =  p_in_cell_config_req_msg->operator_info.eicic_info.dl_sinr_threshold_high_mark; 
                  }    
                  if(!(RRM_OAM_ABS_USAGE_HIGH_THRESHOLD_PRESENT & p_in_cell_config_req_msg->operator_info.eicic_info.abs_info.bitmask))
                  {
                      abs_usage_high_threshold = RRM_ABS_USAGE_HIGH_THRESHOLD_DEFAULT_VALUE; 
                  }
                  else
                  {
                      abs_usage_high_threshold = p_in_cell_config_req_msg->operator_info.eicic_info.abs_info.abs_usage_high_threshold; 
                  }
                  if(!(RRM_OAM_ABS_USAGE_LOW_THRESHOLD_PRESENT & p_in_cell_config_req_msg->operator_info.eicic_info.abs_info.bitmask))
                  {
                      abs_usage_low_threshold = RRM_ABS_USAGE_LOW_THRESHOLD_DEFAULT_VALUE;
                  }    
                  else
                  {
                      abs_usage_low_threshold = p_in_cell_config_req_msg->operator_info.eicic_info.abs_info.abs_usage_low_threshold; 
                  }    
                  if( (abs_usage_high_threshold <= abs_usage_low_threshold)||(dl_sinr_threshold_high_mark <= dl_sinr_threshold_low_mark))
                  {
                     check_failed = RRM_TRUE;
                     fail_cause = RRM_ERR_INVALID_PARAMS; 
                  }    
                }    
                /*SPR 14132 end*/
                /*review comments changes start*/ 
                if( RRM_FALSE == check_failed)
                {
                  if(RRM_OAM_EICIC_PROVISIONED_TYPE_PRESENT & p_in_cell_config_req_msg->operator_info.eicic_info.bitmask)
                  {
                    if(RRM_OAM_ACT_AS_AGGRESSOR_AND_VICTIM == p_in_cell_config_req_msg->operator_info.eicic_info.eicic_provisioned_type)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, " provision type RRM_OAM_ACT_AS_AGGRESSOR_AND_VICTIM is not supported in current release");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_INVALID_PARAMS;
                    }
                  }
                }  
                /*review comments changes end*/
                rrm_memset_rrm_oam_cell_config_resp(&cell_config_resp_to_oam, RRM_ZERO);
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                            &(cell_config_resp_to_oam.global_cell_id),
                            &(p_in_cell_config_req_msg->global_cell_info.eutran_global_cell_id));
                    cell_config_resp_to_oam.response = RRM_FAILURE;
                    cell_config_resp_to_oam.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                            RRM_BRIEF, 
                            "Failure: Sending Cell Config Resp with fail_cause(%d) to OAM",
                            fail_cause);
                    rrm_oam_send_rrm_oam_cell_config_resp(
                            &cell_config_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_config_req_msg);
                    p_mif_context = RRM_PNULL;
                   /* Coverity 74778 Fix Start */
                   /* Coverity 74778 Fix End */
                }  
                break;
            }

        case RRM_OAM_SHUTDOWN_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_shutdown_req);
#endif        
                p_in_cell_shutdown_req_msg =  (rrm_oam_shutdown_req_t *)rrm_mem_get(sizeof(rrm_oam_shutdown_req_t));
                if ( p_in_cell_shutdown_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_shutdown_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_shutdown_req( /* to be generated using pup_tool*/
                            p_in_cell_shutdown_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Shutdown message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_shutdown_resp_F_inv_param);
#endif        
                }
                else
                {
                    /* Loop through all valid cells and trigger the shutdown routine for
                       each one them them */
                    fail_cause = rrm_mif_send_event_to_all_cells(MIF_FSM_EVENT_CELL_SHUTDOWN_REQ,
                            (U8 *)p_in_cell_shutdown_req_msg,
                            p_gl_ctxt,
                            api_hdr->transaction_id,
                            api_hdr->source);
                    if (fail_cause != RRM_NO_ERROR) 
                    {
                        check_failed = RRM_TRUE;
                    }

                }

                if(RRM_TRUE == check_failed)
                {
                    cell_shutdown_resp_to_oam.response = RRM_FAILURE;
                    cell_shutdown_resp_to_oam.fail_cause = fail_cause;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_shutdown_resp_F);
#endif        
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Cell Shutdown Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    if ( RRM_FAILURE == rrm_oam_send_rrm_oam_shutdown_resp(
                            &cell_shutdown_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE)))
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Failed to send rrm_oam_send_rrm_oam_shutdown_resp ");
                    }
                    RRM_MEM_FREE(p_in_cell_shutdown_req_msg);

                    /* Change state to ACTIVE */
                    p_gl_ctxt->current_state  = RMIF_STATE_ACTIVE;
                }

                /* Set p_mif_context to null to indicate that no further processing is required from this event */  
                p_mif_context = RRM_PNULL;
                /* Coverity 74778 Fix Start */
                /* Coverity 74778 Fix End */
                break;
            }
        case RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_REQ:
            {
                p_in_cell_ecn_capa_enhance_req=  (rrm_oam_cell_ecn_capacity_enhance_req_t *)
                    rrm_mem_get(sizeof(rrm_oam_cell_ecn_capacity_enhance_req_t));
                if ( p_in_cell_ecn_capa_enhance_req == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_ecn_capa_enhance_req failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
 
                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_ecn_capacity_enhance_req( /* to be generated using pup_tool*/
                            p_in_cell_ecn_capa_enhance_req,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad ECN_capacity message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                }
                else
                {
                    for(cell_count=RRM_ZERO;cell_count<p_in_cell_ecn_capa_enhance_req->count;cell_count++)
                    {
                        /*fetch mif context for each global_cell_id */
                        ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                                &(p_in_cell_ecn_capa_enhance_req->ecn_cells[cell_count].global_cell_id),
                                &mif_ctxt_cnt,
                                &mif_ctxt_id);
                        if (RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            break;/*cell is not configured*/
                        }
                        else
                        {
                            store_mif_ctxt[cell_count]=mif_ctxt_id;
                        }
                    }
                    /*Send event for particular cell not all*/
                     if (RRM_FALSE == check_failed) /* Coverity: CID 14182 */
                    {
                        fail_cause = rrm_mif_send_event_to_particular_cells(MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ,
                                (U8*)p_in_cell_ecn_capa_enhance_req,
                                p_gl_ctxt,
                                api_hdr->transaction_id,
                                api_hdr->source,
                                store_mif_ctxt);

                        /* SPR: 15922 Fix Start */
                        if (fail_cause != RRM_NO_ERROR)
                        {
                            check_failed = RRM_TRUE;
                        }
                        else
                        {
                            RRM_MEM_FREE(p_in_cell_ecn_capa_enhance_req);
                        }
                        /* SPR: 15922 Fix End */
                    }
                    p_mif_context = RRM_PNULL;
                }

                if(RRM_TRUE == check_failed)
                {
                    p_out_cell_ecn_capa_enhance_resp.response = RRM_FAILURE;
                    p_out_cell_ecn_capa_enhance_resp.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Cell ECN Capacity Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    rrm_oam_send_rrm_oam_cell_ecn_capacity_enhance_resp(
                            (void *)(&p_out_cell_ecn_capa_enhance_resp),
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            RRM_ZERO,
                            RRM_ZERO);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }  
                /* COVERITY : RESOURCE LEAK : CID : 54393 fix start */
                if( RRM_NULL != p_in_cell_ecn_capa_enhance_req )
                    RRM_MEM_FREE( p_in_cell_ecn_capa_enhance_req ); 
                /* COVERITY : RESOURCE LEAK : CID : 54393 fix end */ 
                break;
            }
        /*ECN end*/
        case RRM_OAM_CELL_RECONFIG_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_req);
#endif        
                p_in_cell_reconfig_req_msg =  (rrm_oam_cell_reconfig_req_t *)
                    rrm_mem_get(sizeof(rrm_oam_cell_reconfig_req_t));
                if ( p_in_cell_reconfig_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_reconfig_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
 
                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_reconfig_req( /* to be generated using pup_tool*/
                            p_in_cell_reconfig_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Reconf message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_resp_F_inv_param);
#endif        
                }
                else
                {
                    /*SPR_19279_START*/
                    /* Code Removed */
                    cell_index = rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE);
                    if ((RRM_INVALID_CELL_ID == cell_index) || (cell_index >= RRM_MAX_NUM_CELLS))
                    {
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                "cell_config_req:Cell index allocation failed :%d",
                                cell_index);
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_config_resp_F_indx_alloc_failed);
#endif        
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_INVALID_CELL_ID;
                    }    
                    for (mif_ctxt_cnt = RRM_ZERO; mif_ctxt_cnt < MAX_NUM_MIF_CTXT; mif_ctxt_cnt++)
                    {
                        if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
                        {
                            if (p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index == cell_index)
                            {
                                mif_ctxt_id = p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_DETAILED, "Cell_index:%d matched for reconfiguration ",cell_index);
                                ret_val = RRM_SUCCESS;
                                break;
                            }
                        } 
                    }
                    /*SPR_19279_END*/
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_resp_F_cell_unconfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      (void *)(&(p_in_cell_reconfig_req_msg->global_cell_id)),
                                      sizeof(rrm_oam_eutran_global_cell_id_t),
                                      /* SPR_19279_END */
                                    api_hdr->transaction_id,
                                    api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_reconfig_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            /*SPR_19279_START*/
                                    /* SPR 20653 Fix Start */
                            /* Code removed */
                                    /* SPR 20653 Fix End */
                            /*SPR_19279_END*/
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_RECONFIG_REQ;
                            returnValue->p_msg_info = (U8 *)p_in_cell_reconfig_req_msg;
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, 
                                    "MIF Context not found in received CELL_RECONFIG_REQUEST ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_reconfig_resp_F_cell_unconfig);
#endif        
                        }
                    }
                }

                rrm_memset_rrm_oam_cell_reconfig_resp(&cell_reconfig_resp_to_oam, RRM_ZERO);
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                              &(cell_reconfig_resp_to_oam.global_cell_id),
                            &(p_in_cell_reconfig_req_msg->global_cell_id));
                    cell_reconfig_resp_to_oam.response = RRM_FAILURE;
                    cell_reconfig_resp_to_oam.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Cell Reconf Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    rrm_oam_send_rrm_oam_cell_reconfig_resp(
                            &cell_reconfig_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_reconfig_req_msg);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }  
                break;
            }

        case RRM_OAM_CELL_DELETE_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_req);
#endif        
                p_in_cell_delete_req_msg =  (rrm_oam_cell_delete_req_t *)rrm_mem_get(sizeof(rrm_oam_cell_delete_req_t));
                if ( p_in_cell_delete_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_delete_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
 
                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_delete_req( /* to be generated using pup_tool*/
                            p_in_cell_delete_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Delete message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_delete_req_msg->global_cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                    api_hdr->transaction_id,
                                    api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_delete_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_DELETE_REQ;
                            returnValue->p_msg_info = (U8 *)p_in_cell_delete_req_msg;
                            //RRM_HEX_DUMP("CELL_DELETE_REQUEST",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_delete_req(p_in_cell_delete_req_msg);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, "MIF Context not found in received CELL_DELETE_REQUEST ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                           RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_resp_F);
                           RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_delete_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }

                rrm_memset_rrm_oam_delete_resp(&cell_delete_resp_to_oam, RRM_ZERO);
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                              &(cell_delete_resp_to_oam.global_cell_id),
                            &(p_in_cell_delete_req_msg->global_cell_id));
                    cell_delete_resp_to_oam.response = RRM_FAILURE;
                    cell_delete_resp_to_oam.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Cell Delete Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    rrm_oam_send_rrm_oam_delete_resp(
                            &cell_delete_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_delete_req_msg);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }  
                break;
            }

        case RRM_OAM_CELL_START_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_req);
#endif        
                p_in_cell_start_req_msg =  (rrm_oam_cell_start_req_t *)rrm_mem_get(sizeof(rrm_oam_cell_start_req_t));
                if ( p_in_cell_start_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_start_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_start_req( /* to be generated using pup_tool*/
                            p_in_cell_start_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Start message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_start_req_msg->global_cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                    api_hdr->transaction_id,
                                    api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_start_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_START_REQ;
                            returnValue->p_msg_info = (U8 *)p_in_cell_start_req_msg;
                            //RRM_HEX_DUMP("CELL_START_REQUEST",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_start_req(p_in_cell_start_req_msg);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, "MIF Context not found in received CELL_START_REQUEST ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_start_resp_F_cell_unonfig); 
#endif        
                        }
                    }
                }

                rrm_memset_rrm_oam_cell_start_resp(&cell_start_resp_to_oam, RRM_ZERO);
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                              &(cell_start_resp_to_oam.global_cell_id),
                            &(p_in_cell_start_req_msg->global_cell_id));
                    cell_start_resp_to_oam.response = RRM_FAILURE;
                    cell_start_resp_to_oam.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Cell Start Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    rrm_oam_send_rrm_oam_cell_start_resp(
                            &cell_start_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_start_req_msg);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }  
                break;
            }

        case RRM_OAM_CELL_STOP_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_req);
#endif        
                p_in_cell_stop_req_msg =  (rrm_oam_cell_stop_req_t *)rrm_mem_get(sizeof(rrm_oam_cell_stop_req_t));
                if ( p_in_cell_stop_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_stop_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_stop_req( /* to be generated using pup_tool*/
                            p_in_cell_stop_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Stop message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_resp_F_inv_param);
#endif        
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_cell_stop_req_msg->global_cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_resp_F);
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_resp_F_cell_unonfig);
#endif        
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                    api_hdr->transaction_id,
                                    api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_in_cell_stop_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_STOP_REQ;
                            returnValue->p_msg_info =(U8 *) p_in_cell_stop_req_msg;
                            RRM_ASSERT (RRM_PNULL != (returnValue->p_msg_info));
                            //RRM_HEX_DUMP("CELL_STOP_REQUEST",p_api,api_hdr->msg_size,p_mif_context);
                            //rrm_print_cell_stop_req(p_in_cell_stop_req_msg);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                    RRM_ERROR, "MIF Context not found in received CELL_STOP_REQUEST ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_stop_resp_F_cell_unonfig);
#endif        
                        }
                    }
                }

                rrm_memset_rrm_oam_cell_stop_resp(&cell_stop_resp_to_oam, RRM_ZERO);
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                              &(cell_stop_resp_to_oam.global_cell_id),
                            &(p_in_cell_stop_req_msg->global_cell_id));
                    cell_stop_resp_to_oam.response = RRM_FAILURE;
                    cell_stop_resp_to_oam.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending Cell Stop Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    rrm_oam_send_rrm_oam_cell_stop_resp(
                            &cell_stop_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_in_cell_stop_req_msg);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }  
                break;
            }

        case RRM_OAM_RAC_ENABLE_DISABLE_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_req);
#endif        
                /* Reset the global response bit */
                p_gl_ctxt->rac_response_sent_to_oam = RRM_FALSE;

                p_rac_enable_disable_req_msg =  (rrm_oam_rac_enable_disable_req_t*) 
                    rrm_mem_get(sizeof(rrm_oam_rac_enable_disable_req_t));
                if ( p_rac_enable_disable_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_rac_enable_disable_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                /* Parse the RRM_OAM_RAC_ENABLE_DISABLE_REQ API */
                if(RRM_SUCCESS != rrm_parse_rrm_oam_rac_enable_disable_req( /* to be generated using pup_tool*/
                            p_rac_enable_disable_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    /* Parsing failed */
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad RAC enable/disable message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_resp_F_inv_param);
#endif        
                }
                else
                {
                    /* Parsing successful */
                    /* If the RRM_OAM_RAC_ENABLE_DISABLE_REQ API contains global cell id then 
                       we need to fetch the mif context id for the global cell id */
                    if (RRM_TRUE == (RMIF_GLOBAL_CELL_ID_PRESENT(p_rac_enable_disable_req_msg)))
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, "Global Cell-Id Present");
                        ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                                &(p_rac_enable_disable_req_msg->global_cell_id),
                                &mif_ctxt_cnt,
                                &mif_ctxt_id);
                        if (RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_resp_F);
                            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_resp_F_cell_unconfig);
#endif        
                        }
                        else
                        {
                            p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                            if (RRM_PNULL != p_mif_context)
                            {
                                ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                          /* SPR_19279_START */
                                          RRM_NULL,
                                          RRM_ZERO,
                                          /* SPR_19279_END */
                                        api_hdr->transaction_id,
                                        api_hdr->source);
                                if(RRM_FAILURE == ret_val)
                                {
                                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                    RRM_MEM_FREE(p_in_cell_start_req_msg);
                                    p_mif_context = RRM_PNULL;
                                    /* SPR 19838 : CID 11102 Fix Start */
                                    RRM_MEM_FREE(p_rac_enable_disable_req_msg);
                                    /* SPR 19838 : CID 11102 Fix End */
                                    return p_mif_context;
                                }
                                returnValue->fsm_index = MIF_FSM;
                                returnValue->event_id = MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ;
                                returnValue->p_msg_info = (U8 *)p_rac_enable_disable_req_msg;
                            }
                            else
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "MIF Context not found in received \
                                        RRM_OAM_RAC_ENABLE_DISABLE_REQ ");
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_CELL_UNCONFIGURED;
#ifdef RRM_UT_FLAG        
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_resp_F);
                                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.rac_enbl_disbl_resp_F_cell_unconfig);
#endif        
                            }
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, "Global Cell-Id Not Present");
                        /* Loop through all valid cells and send the rac enable/disable request
                           to all of them.*/
                        fail_cause = rrm_mif_send_event_to_all_cells(MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ,
                                (U8*)p_rac_enable_disable_req_msg,
                                p_gl_ctxt,
                                api_hdr->transaction_id,
                                api_hdr->source);
                        if (fail_cause != RRM_NO_ERROR)
                        {
                            check_failed = RRM_TRUE;
                        }
						/*SPR 11963 fix */
                    }
                }
                if(RRM_TRUE == check_failed)
                {
                    rrm_memcpy_rrm_oam_eutran_global_cell_id(
                           &(rac_enable_disable_resp_to_oam.global_cell_id),
                            &(p_rac_enable_disable_req_msg->global_cell_id));
                    rac_enable_disable_resp_to_oam.response = RRM_FAILURE;
                    rac_enable_disable_resp_to_oam.fail_cause = fail_cause;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                              RRM_BRIEF, 
                              "Failure: Sending RAC En/Dis Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    rrm_oam_send_rrm_oam_rac_enable_disable_resp(
                            &rac_enable_disable_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_MEM_FREE(p_rac_enable_disable_req_msg);
                    p_mif_context = RRM_PNULL;
                    p_gl_ctxt->rac_response_sent_to_oam = RRM_TRUE;
                }  
                break;
            }

        case RRM_OAM_RESUME_SERVICE_REQ:
            {
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.resume_srvc_req);
#endif        
                /* Loop through all valid cells and trigger the shutdown routine for
                   each one them them */
                fail_cause = rrm_mif_send_event_to_all_cells(MIF_FSM_EVENT_CELL_RESUME_REQ,
                        RRM_PNULL,
                        p_gl_ctxt,
                        api_hdr->transaction_id,
                        api_hdr->source);
                if(fail_cause != RRM_NO_ERROR)
                {
                    resume_service_resp_to_oam.response = RRM_FAILURE;
                    resume_service_resp_to_oam.fail_cause = fail_cause;
#ifdef RRM_UT_FLAG        
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.resume_srvc_resp_F);
#endif        
                    if(RRM_FAILURE == rrm_oam_send_rrm_oam_resume_service_resp(
                            &resume_service_resp_to_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE)))
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, 
                              "Failed to send resume service resp to OAM");
                    }

                    /* Change state to ACTIVE */
                    p_gl_ctxt->current_state  = RMIF_STATE_ACTIVE;
                }

                /* Set p_mif_context to null to indicate that no further processing is required from this event */
                p_mif_context = RRM_PNULL;

                break;
            }

        case RRM_OAM_CELL_CONTEXT_PRINT_REQ: 
            {
                p_in_cell_context_print_req_msg =  (rrm_oam_cell_context_print_req_t *)rrm_mem_get(sizeof(rrm_oam_cell_context_print_req_t));
                if ( p_in_cell_context_print_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_cell_context_print_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_context_print_req( /* to be generated using pup_tool*/
                            p_in_cell_context_print_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                            "Bad Cell print msg - Parsing Failed --Message ignored.");
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt,
                            &(p_in_cell_context_print_req_msg->global_cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                                "Mif Context not found for Global Cell Id requested");
                    }
                    else
                    { 
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
                        if (RRM_PNULL != p_mif_context)
                        {
                            build_and_send_context_print_req(p_mif_context->cell_index);
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,"MIF Context not found in received CELL_PRINT_REQ");
                        }
                    }	
                }
                p_mif_context = RRM_PNULL;
                /* COVERITY : RESOURCE LEAK : CID : 54390 fix start */
                if( RRM_NULL != p_in_cell_context_print_req_msg )
                    RRM_MEM_FREE(p_in_cell_context_print_req_msg);
                /* COVERITY : RESOURCE LEAK : CID : 54390 fix start */
                break;
            }
        case RRM_OAM_CELL_BLOCK_REQ:
            {

                p_in_block_cell_req_msg =  (rrm_oam_cell_block_req_t *)rrm_mem_get(sizeof(rrm_oam_cell_block_req_t));
                if ( p_in_block_cell_req_msg == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_block_cell_req_msg failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if (RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_block_req ( /* to be generated using pup_tool*/
                            p_in_block_cell_req_msg,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Block Cell Req message - ignore.");
                    /*Cov_fix_54389*/
                    if(RRM_PNULL != p_in_block_cell_req_msg)
                    {
                       RRM_MEM_FREE(p_in_block_cell_req_msg);
                    }
                    /*Cov_fix_54389*/
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt (p_gl_ctxt,
                                    &(p_in_block_cell_req_msg->global_cell_id),
                                    &mif_ctxt_cnt,
                                    &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context (p_gl_ctxt, mif_ctxt_id);
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context (p_mif_context,
                                                        /* SPR_19279_START */
                                                        RRM_NULL,
                                                        RRM_ZERO,
                                                        /* SPR_19279_END */
                                                        api_hdr->transaction_id,
                                                        api_hdr->source);
                            if (RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                            "TCB cant be allocated");
                                RRM_MEM_FREE (p_in_block_cell_req_msg);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            else
                            {
                                returnValue->fsm_index = MIF_FSM;
                                returnValue->event_id =  MIF_FSM_EVENT_BLOCK_CELL_REQ;
                                returnValue->p_msg_info = (U8 *)p_in_block_cell_req_msg;                                
                            }
                        }
                        else
                        {
                            RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                   "MIF Context not found in received BLOCK_CELL_REQUEST");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        }
                    }
                    rrm_memset_rrm_oam_cell_block_resp(&block_cell_resp_to_oam, RRM_ZERO);
                    if (RRM_TRUE == check_failed)
                    {
                        rrm_memcpy_rrm_oam_eutran_global_cell_id(
                            &(block_cell_resp_to_oam.global_cell_id),
                            &(p_in_block_cell_req_msg->global_cell_id));
                        block_cell_resp_to_oam.response = RRM_FAILURE;
                        block_cell_resp_to_oam.fail_cause = fail_cause;
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF,
                            "Failure: Sending Block Cell Resp with fail_cause(%d) to OAM", fail_cause);
                        /* Coverity : CID 11473 Rel 2.1.3 */
                        if(RRM_FAILURE == rrm_oam_send_rrm_oam_cell_block_resp(
                                                        &block_cell_resp_to_oam, 
                                                        RRM_MIF_MODULE_ID, 
                                                        RRM_OAM_MODULE_ID,
                                                        (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                                                        rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE)))
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF,
                                "Failed to send Block Cell response failure");
                        }
                        RRM_MEM_FREE(p_in_block_cell_req_msg);
                        p_mif_context = RRM_PNULL;
                        /* Coverity 74778 Fix Start */
                        /* Coverity 74778 Fix End */
                    }
                }
                break;
            }
            /*29220: RRM_OAM_CELL_UNBLOCK_CMD */
            case RRM_OAM_CELL_UNBLOCK_CMD:
            {
                p_rrm_oam_unblock_cell_cmd = (rrm_oam_cell_unblock_cmd_t *)
                                    rrm_mem_get(sizeof(rrm_oam_cell_unblock_cmd_t));
                if ( p_rrm_oam_unblock_cell_cmd == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_rrm_oam_unblock_cell_cmd failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }
                
                if (RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_unblock_cmd (
                                                p_rrm_oam_unblock_cell_cmd,
                                                (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                                                api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                                                &length_read))
                {
                    /* Coverity 74795 Fix Start */
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                    RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                   RRM_ERROR, 
                                   "Bad UnBlock Cell IND message - ignored fail"
                                   "cause[%d].",fail_cause);
                    /* Coverity 74795 Fix End */
                    /* Coverity 54391 fix start */
                    RRM_MEM_FREE (p_rrm_oam_unblock_cell_cmd);
                    /* Coverity 54391 fix end */
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt (p_gl_ctxt,
                                       &(p_rrm_oam_unblock_cell_cmd->global_cell_id),
                                       &mif_ctxt_cnt,
                                       &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                       /* Coverity 74795 Fix Start */
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                   RRM_ERROR, "Incorrect Mif Context fail"
                                   "cause[%d] ",fail_cause);
                       /* Coverity 74795 Fix End */
                        check_failed = RRM_TRUE;
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context (p_gl_ctxt, mif_ctxt_id);
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                                        /* SPR_19279_START */
                                                        RRM_NULL,
                                                        RRM_ZERO,
                                                        /* SPR_19279_END */
                                                        api_hdr->transaction_id,
                                                        api_hdr->source);
                            if (RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "TCB cant be allocated");
                                RRM_MEM_FREE (p_rrm_oam_unblock_cell_cmd);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }
                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_UNBLOCK_CELL_CMD;
                            returnValue->p_msg_info = (U8 *)p_rrm_oam_unblock_cell_cmd;
                        }
                        else
                        {
                           /* Coverity 74795 Fix Start */
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                    "MIF Context not found in received"
                                   " UNBLOCK_CELL_CMD fail cause[%d]",fail_cause);
                            check_failed = RRM_TRUE;
                        }

                    }
                    if (RRM_TRUE == check_failed)
                    {
                        RRM_TRACE (g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                "Failed to handle Cell unblock command");
                        /* Coverity fix 54391 */
                        RRM_MEM_FREE (p_rrm_oam_unblock_cell_cmd);
                    }
                                    
                }
                
                break;
            }

            /*Cell update start*/
            case RRM_OAM_CELL_UPDATE_REQ:
            {
                p_rrm_oam_cell_update_req = (rrm_oam_cell_update_req_t *)rrm_mem_get(
                                                     sizeof(rrm_oam_cell_update_req_t));
                if ( p_rrm_oam_cell_update_req == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_rrm_oam_cell_update_req failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_cell_update_req( /* to be generated using pup_tool*/
                                    p_rrm_oam_cell_update_req,
                                    (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                                    api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                                    &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                               RRM_ERROR, "Bad Cell Update message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                }
                else
                {
                    ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt,
                               &(p_rrm_oam_cell_update_req->global_cell_id),
                               &mif_ctxt_cnt,
                               &mif_ctxt_id);
                    if (RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_ERROR, "Incorrect Mif Context ");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                    }
                    else
                    {
                        p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);
                        if (RRM_PNULL != p_mif_context)
                        {
                            ret_val = allocate_and_store_tcb_info_in_mif_context(
                                      p_mif_context,
                                      /* SPR_19279_START */
                                      RRM_NULL,
                                      RRM_ZERO,
                                      /* SPR_19279_END */
                                      api_hdr->transaction_id,
                                      api_hdr->source);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                                RRM_ERROR, "TCB cant be allocated");
                                RRM_MEM_FREE(p_rrm_oam_cell_update_req);
                                p_mif_context = RRM_PNULL;
                                return p_mif_context;
                            }

                            returnValue->fsm_index = MIF_FSM;
                            returnValue->event_id = MIF_FSM_EVENT_CELL_UPDATE_REQ;
                            returnValue->p_msg_info = (U8 *)p_rrm_oam_cell_update_req;
                        }
                        else
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                                    RRM_ERROR,
                                    "MIF Context not found in received CELL_UPDATE_REQUEST ");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        }
                    }
                }

                RRM_MEMSET(&rrm_oam_cell_update_resp, RRM_ZERO, 
                                       sizeof(rrm_oam_cell_update_resp_t));

                if(RRM_TRUE == check_failed)
                {
                    RRM_MEMCPY(&(rrm_oam_cell_update_resp.global_cell_id),
                              &(p_rrm_oam_cell_update_req->global_cell_id),
                              sizeof(rrm_oam_eutran_global_cell_id_t));
                    rrm_oam_cell_update_resp.response = RRM_FAILURE;
                    rrm_oam_cell_update_resp.fail_cause = fail_cause;
                    rrm_oam_send_rrm_oam_cell_update_resp(
                            &rrm_oam_cell_update_resp,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(
                            &(((U8 *)p_api)[RRM_API_HEADER_SIZE]))), 
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name,
                              RRM_BRIEF,
                              "Failure: Sending Cell Update Resp with fail_cause(%d) to OAM",
                              fail_cause);
                    RRM_MEM_FREE(p_in_cell_reconfig_req_msg);
                    /* COVERITY : RESOURCE LEAK : CID : 54395 fix start */
                    if( RRM_NULL != p_rrm_oam_cell_update_req )
                        RRM_MEM_FREE(p_rrm_oam_cell_update_req);
                    /* COVERITY : RESOURCE LEAK : CID : 54395 fix end */
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }
                break;
            }
            /*Cell update end*/
            /*Platform indication*/
        case RRM_PLATFORM_LOAD_IND:
            {
                p_in_platform_ind = (platform_rrm_load_ind_t *)rrm_mem_get(sizeof(platform_rrm_load_ind_t));
                if ( p_in_platform_ind == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_in_platform_ind failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_platform_rrm_load_ind( /* to be generated using pup_tool*/
                            p_in_platform_ind,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            (S32)(api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE),
                            &length_read))
                {
                    /* Coverity 74795 Fix Start */
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                            "Bad Platform Message - ignore. fail"
                            "cause[%d]",fail_cause);
                    /* Coverity 74795 Fix End */
                    check_failed = RRM_TRUE;
                }
                else
                {
                    for(cell_count= RRM_ZERO;cell_count <p_in_platform_ind->num_of_cell; cell_count++)
                    {
                        ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                            &(p_in_platform_ind->cell_load[cell_count].global_cell_id),
                            &mif_ctxt_cnt,
                            &mif_ctxt_id);
                        if (RRM_FAILURE == ret_val)
                        {
                           /* Coverity 74795 Fix Start */
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                    "Incorrect Mif Context fail cause[%d]",
                                    fail_cause);
                           /* Coverity 74795 Fix End */
                            check_failed = RRM_TRUE;
                            break; //NO need to fetch check further mif context
                        }
                        else
                        {
                           /*Storing Mif context id*/
                           store_mif_ctxt[cell_count]= mif_ctxt_id; 
                        }
                    }
                }
                /*Send event for particular cell not all*/
                if (RRM_FALSE == check_failed)  /* Coverity: CID 14182 */
                {
                    /* Loop through all valid cells and trigger the shutdown routine for
                       each one them them */
                    fail_cause = rrm_mif_send_event_to_particular_cells(MIF_FSM_EVENT_CELL_PLATFORM_IND,
                            (U8 *)p_in_platform_ind,
                            p_gl_ctxt,
                            api_hdr->transaction_id,
                            api_hdr->source,
                            store_mif_ctxt);
                    if (fail_cause != RRM_NO_ERROR) 
                    {
                        /* No fail cause shall be send to OAM as this is an indication */
                        /* Coverity 74778 Fix Start */
                            RRM_TRACE(g_mif_log_on_off,
                                    p_g_rrm_mif_facility_name, RRM_ERROR,
                                    "Failed in"
                                    "rrm_mif_send_event_to_particular_cells fail "
                                    "cause [%d]",fail_cause);
                        /* Coverity 74778 Fix End */
                    }
                    /*Coverity 54392 Fix Start */
                    RRM_MEM_FREE(p_in_platform_ind);
                    /*Coverity 54392 Fix End */
                    /* SPR: 15922 Fix End */
                    p_mif_context = RRM_PNULL;
                }
                /* SPR: 15922 Fix Start */
                if(RRM_TRUE == check_failed)
                {
                    RRM_MEM_FREE(p_in_platform_ind);
                    check_failed = RRM_FALSE;
                }
                /* SPR: 15922 Fix End */
                break;
            }
        case RRM_OAM_CONFIG_KPI_REQ: 
            {
                p_in_oam_config_kpi_req = (rrm_oam_config_kpi_req_t *)
                    rrm_mem_get(sizeof(rrm_oam_config_kpi_req_t));

                if(RRM_PNULL != p_in_oam_config_kpi_req)  /* Coverity: CID 30737 */
                {
                rrm_oam_config_kpi_resp.bitmask = RRM_ZERO;

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_config_kpi_req( /* to be generated using pup_tool*/
                            p_in_oam_config_kpi_req,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell KPI"
                            "config req message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                    
                }
                else
                {
                    /* BUG_857 start*/
                    /*currently RRM is supporting only 34 bits. Need to remove this for loop in future */
                    if ((0xFC & p_in_oam_config_kpi_req->kpi_to_report.bitmap[RRM_FOUR]) != RRM_ZERO)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Bit set for index above than bit 34");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_INVALID_PARAMS;
                    }
                    else
                    {
                        for(index = RRM_FIVE;index<RRM_MAX_KPI;index++)
                        {
                    /* BUG_857 end*/        
                        if(RRM_ZERO != p_in_oam_config_kpi_req->kpi_to_report.bitmap[index])
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bit set for index above than bit 34");
                                
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_INVALID_PARAMS;
                                break;
                        }
                    }
                    }

                    if (RRM_FALSE == check_failed)
                    {
                        /*validation madatory field is present or not*/
                        if (RRM_TRUE == p_in_oam_config_kpi_req->periodic_reporting)
                        {
                            for(index = RRM_ZERO;index<RRM_MAX_KPI;index++)
                            {
                                /*In config_kpi_req for enable timer value or any of bitmap must be present in config kpi req*/
                                if(RRM_ZERO != p_in_oam_config_kpi_req->kpi_to_report.bitmap[index] ||
                                        p_in_oam_config_kpi_req->bitmask & RRM_OAM_CONFIG_KPI_DURATION_PRESENT) 
                                {
                                    check_failed = RRM_FALSE;
                                    fail_cause = RRM_NO_ERROR;
                                    break;
                                }
                                else
                                {
                                    check_failed = RRM_TRUE;
                                    fail_cause = RRM_ERR_MANDATORY_PARAMS_ABSENT;
                                }
                            }
                        }
                        /* SPR 22543 Fix +- */
                    }
                    if (RRM_FALSE == check_failed)
                    {
                        ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                                &(p_in_oam_config_kpi_req->cell_id),
                                &mif_ctxt_cnt,
                                &mif_ctxt_id);
                        if (RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Cell is unconfigured");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        }
                        else
                        {
                            p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                            if (RRM_PNULL != p_mif_context)
                            {
                                ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                          /* SPR_19279_START */
                                          RRM_NULL,
                                          RRM_ZERO,
                                          /* SPR_19279_END */
                                        api_hdr->transaction_id,
                                        api_hdr->source);
                                if(RRM_FAILURE == ret_val)
                                {
                                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                    check_failed = RRM_TRUE;
                                    fail_cause = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
                                    p_mif_context = RRM_PNULL;
                                }
                                else
                                {
                                    returnValue->fsm_index = MIF_FSM;
                                    returnValue->event_id = MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ;
                                    returnValue->p_msg_info =(U8 *) p_in_oam_config_kpi_req;
                                    RRM_ASSERT (RRM_PNULL != (returnValue->p_msg_info));
                                }
                            }
                            else
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_ERROR, "MIF Context not found for received RRM_OAM_CONFIG_KPI_REQ");
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            }
                        }
                    }
                    rrm_oam_config_kpi_resp.bitmask |= RRM_OAM_CONFIG_KPI_CELL_ID_PRESENT;
                }
                if(RRM_TRUE == check_failed)
                {
                    if (rrm_oam_config_kpi_resp.bitmask & RRM_OAM_CONFIG_KPI_CELL_ID_PRESENT)
                    {
                        RRM_MEMCPY(&(rrm_oam_config_kpi_resp.cell_id),
                                &(p_in_oam_config_kpi_req->cell_id),
                                sizeof(rrm_oam_eutran_global_cell_id_t));
                    }
                    rrm_oam_config_kpi_resp.response = RRM_FAILURE;
                    rrm_oam_config_kpi_resp.fail_cause = fail_cause;
                    ret_val = rrm_oam_send_rrm_oam_config_kpi_resp(
                            (void *)(&rrm_oam_config_kpi_resp),
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    /* Coverity ID 76359 Fix Start*/
                    if( ret_val == RRM_FAILURE)
                    {    
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, 
                                "Failure: Sending Cell Config KPI Resp with fail_cause(%d) to OAM",
                                fail_cause);
                    }
                    else
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, 
                                "Success: Sending Cell Config KPI Resp with fail_cause(%d) to OAM",
                                fail_cause);
                    }
                    /* Coverity ID 76359 Fix End*/
                    RRM_MEM_FREE(p_in_oam_config_kpi_req);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }
                }
                else  /* Coverity: CID 30737 */
                {
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation failed" );
                        RRM_UT_TRACE_EXIT();
                        return RRM_PNULL;

                }
                break;
            }
        case RRM_OAM_GET_KPI_REQ: 
            {
                p_rrm_oam_get_kpi_req = (rrm_oam_get_kpi_req_t *)
                    rrm_mem_get(sizeof(rrm_oam_get_kpi_req_t));
                if ( p_rrm_oam_get_kpi_req == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_rrm_oam_get_kpi_req failed" );
                    RRM_UT_TRACE_EXIT();
                    return RRM_PNULL;
                }

                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_get_kpi_req( /* to be generated using pup_tool*/
                            p_rrm_oam_get_kpi_req,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Get Kpi message - ignore.");
                    check_failed = RRM_TRUE;
                    fail_cause = RRM_ERR_INVALID_PARAMS;
                }
                else
                {
                    /* BUG_857 start*/
                    /*currently RRM is supporting only 34 bits. Need to remove this for loop in future */
                    /*CA STAGE 2*/
                    if ((0xFC & p_rrm_oam_get_kpi_req->kpi_to_report.bitmap[RRM_FOUR]) != RRM_ZERO)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Bit set for index above than bit 34");
                        check_failed = RRM_TRUE;
                        fail_cause = RRM_ERR_INVALID_PARAMS;
                    }
                    else
                    {
                        for(index = RRM_FIVE;index<RRM_MAX_KPI;index++)
                        {
                            if(RRM_ZERO != p_rrm_oam_get_kpi_req->kpi_to_report.bitmap[index])
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Bit set above than index five");
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_INVALID_PARAMS;
                                break;
                            }
                        }
                    }

                    if(RRM_FALSE == check_failed)
                    {
                        for(index = RRM_ZERO;index<RRM_MAX_KPI;index++)
                        {
                            if(RRM_ZERO != p_rrm_oam_get_kpi_req->kpi_to_report.bitmap[index])  
                            {
                                check_failed = RRM_FALSE;
                                fail_cause = RRM_NO_ERROR;
                                break;
                            }
                            else
                            {
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_MANDATORY_PARAMS_ABSENT;
                            }
                        }
                    }
                    if(RRM_FALSE == check_failed)
                    {
                        ret_val = fetch_mif_ctxt_id_from_global_ctxt(p_gl_ctxt, 
                                &(p_rrm_oam_get_kpi_req->cell_id),
                                &mif_ctxt_cnt,
                                &mif_ctxt_id);
                        if (RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Cell is unconfigured");
                            check_failed = RRM_TRUE;
                            fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                        }
                        else
                        {
                            p_mif_context = rrm_find_mif_context(p_gl_ctxt, mif_ctxt_id);  
                            if (RRM_PNULL != p_mif_context)
                            {
                                ret_val = allocate_and_store_tcb_info_in_mif_context(p_mif_context,
                                          /* SPR_19279_START */
                                          RRM_NULL,
                                          RRM_ZERO,
                                          /* SPR_19279_END */
                                        api_hdr->transaction_id,
                                        api_hdr->source);
                                if(RRM_FAILURE == ret_val)
                                {
                                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "TCB cant be allocated");
                                    RRM_MEM_FREE(p_rrm_oam_get_kpi_req);
                                    p_mif_context = RRM_PNULL;
                                    return p_mif_context;
                                }
                                returnValue->fsm_index = MIF_FSM;
                                returnValue->event_id = MIF_FSM_EVENT_CELL_GET_KPI_REQ;
                                returnValue->p_msg_info =(U8 *) p_rrm_oam_get_kpi_req;
                                RRM_ASSERT (RRM_PNULL != (returnValue->p_msg_info));
                            }
                            else
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                        RRM_ERROR, "MIF Context not found for received RRM_OAM_GET_KPI_REQ");
                                check_failed = RRM_TRUE;
                                fail_cause = RRM_ERR_CELL_UNCONFIGURED;
                            }
                        }
                    }
                    rrm_oam_get_kpi_resp.bitmask |= RRM_OAM_GET_KPI_CELL_ID_PRESENT;
                }
                if(RRM_TRUE == check_failed)
                {
                    if (rrm_oam_get_kpi_resp.bitmask & RRM_OAM_GET_KPI_CELL_ID_PRESENT)
                    {
                        RRM_MEMCPY(&(rrm_oam_get_kpi_resp.cell_id),
                                &(p_rrm_oam_get_kpi_req->cell_id),
                                sizeof(rrm_oam_eutran_global_cell_id_t));
                    }
                    rrm_oam_get_kpi_resp.response = RRM_FAILURE;
                    rrm_oam_get_kpi_resp.fail_cause = fail_cause;
                    ret_val = rrm_oam_send_rrm_oam_get_kpi_resp(
                            (void *)(&rrm_oam_get_kpi_resp),
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(&(((U8 *)p_api)[RRM_API_HEADER_SIZE]))),
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                    /* Coverity 74626 Fix Start */
                    if(RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                                RRM_ERROR, 
                                "Failed in rrm_oam_send_rrm_oam_get_kpi_resp");
                    }
                    /* Coverity 74626 Fix End */
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, 
                            RRM_ERROR, 
                            "Failure: Sending get Config KPI Resp with fail_cause(%d) to OAM",
                            fail_cause);
                    RRM_MEM_FREE(p_rrm_oam_get_kpi_req);
                    p_mif_context = RRM_PNULL;
                    /* Coverity 74778 Fix Start */
                    /* Coverity 74778 Fix End */
                }  
                break;
            }

		/*spr 22474 start*/
        case RRM_OAM_GET_DEBUG_INFO_REQ: 
            {
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF, 
                        "RRM_OAM_GET_DEBUG_INFO_REQ received from RRM");
                rrm_oam_get_debug_info_req_t *p_rrm_oam_get_debug_info_req = RRM_PNULL;
                rrm_oam_get_debug_info_resp_t *p_rrm_oam_get_debug_info_resp = RRM_PNULL;
                rrm_MemPoolStats rrm_csplMemPoolStats;
                RRM_MEMSET(&rrm_csplMemPoolStats, RRM_ZERO, sizeof(rrm_MemPoolStats));
                p_rrm_oam_get_debug_info_req = (rrm_oam_get_debug_info_req_t *)
                    rrm_mem_get(sizeof(rrm_oam_get_debug_info_req_t));
                if (p_rrm_oam_get_debug_info_req == RRM_PNULL)
                {

                    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF, 
                            "Memory allocation to p_rrm_oam_get_debug_info_req failed");
                    rrm_abort();
                }
                RRM_MEMSET(p_rrm_oam_get_debug_info_req, RRM_ZERO, sizeof(rrm_oam_get_debug_info_req_t));
                p_rrm_oam_get_debug_info_resp = (rrm_oam_get_debug_info_resp_t *)
                    rrm_mem_get(sizeof(rrm_oam_get_debug_info_resp_t));
                if (p_rrm_oam_get_debug_info_resp == RRM_PNULL)
                {

                    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF, 
                            "Memory allocation to p_rrm_oam_get_debug_info_resp failed");
                    /*+CID 147574 */
                    RRM_MEM_FREE(p_rrm_oam_get_debug_info_req);
                    p_mif_context = RRM_PNULL;
                    /*-CID 147574 */
                    rrm_abort();
                }
                RRM_MEMSET(p_rrm_oam_get_debug_info_resp, RRM_ZERO, sizeof(rrm_oam_get_debug_info_resp_t));
                if(RRM_SUCCESS !=  rrm_parse_rrm_oam_get_debug_info_req( 
                            p_rrm_oam_get_debug_info_req,
                            (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                            api_hdr->msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                            &length_read))
                {
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Bad Cell Get Debug Info message - ignore.");
                    /*+-CID 77146 */
                    /*+-CID 74795 */
                }
                else
                {
                    if(p_rrm_oam_get_debug_info_req->debug_type == RRM_GET_MEM_POOL_STATS)
                    {
                        p_rrm_oam_get_debug_info_resp->response = RRM_SUCCESS;
                        p_rrm_oam_get_debug_info_resp->bitmask |= RRM_OAM_DEBUG_GET_MEM_POOL_PRESENT;
                        rrm_prepare_memPool_stats(&rrm_csplMemPoolStats);
                        RRM_MEMCPY(&p_rrm_oam_get_debug_info_resp->csplMemPoolStats,&rrm_csplMemPoolStats,sizeof(rrm_MemPoolStats));
                    }
                    else
                    {
                        p_rrm_oam_get_debug_info_resp->response = RRM_FAILURE;
                        p_rrm_oam_get_debug_info_resp->bitmask |= RRM_OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT;
                        p_rrm_oam_get_debug_info_resp->fail_cause = RRM_WRONG_DEBUG_TYPE;
                    }
                    rrm_oam_send_rrm_oam_get_debug_info_resp(
                            p_rrm_oam_get_debug_info_resp,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            (U16)(rrm_get_word_from_header(
                                    &(((U8 *)p_api)[RRM_API_HEADER_SIZE]))), 
                            rrm_get_cell_id_from_header((U8*)p_api + RRM_API_HEADER_SIZE));
                }  
                /*+CID 147574 */
                RRM_MEM_FREE(p_rrm_oam_get_debug_info_resp);
                RRM_MEM_FREE(p_rrm_oam_get_debug_info_req);
                p_mif_context = RRM_PNULL;
                /*-CID 147574 */
                break;
            }
            /*spr 22474 end*/
        default:
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING," OAM API [%d] Received is wrong. ", api_hdr->api_id);
    }
    RRM_UT_TRACE_EXIT();
    return p_mif_context;
}

/****************************************************************************
 * Function Name  : mif_validate_api_hdr
 * Inputs         : 
 *      1. p_api is not null pointer
 *      2. apIHdr is not null pointer
 * Outputs        : 
 * Returns        : Void
 * Variables      : 
 * Description    : This function validate the Interface API Header.
 ****************************************************************************/
static void mif_validate_api_hdr(void *p_api, interface_api_header *api_hdr)
{
    U16                       upper_byte_source = RRM_ZERO;
    U8                        *p_source = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != api_hdr);

    api_hdr->transaction_id = rrm_get_word_from_header(&(((U8 *)p_api)[EXT_MSG_TRANSACTION_ID_OFFSET]));
    api_hdr->source = rrm_get_word_from_header(&(((U8 *)p_api)[EXT_MSG_SRC_MODULE_ID_OFFSET]));
    api_hdr->destination = rrm_get_word_from_header(&(((U8 *)p_api)[EXT_MSG_DEST_MODULE_ID_OFFSET]));
    api_hdr->api_id = rrm_get_word_from_header(&(((U8 *)p_api)[EXT_MSG_API_OFFSET]));
    api_hdr->msg_size = rrm_get_word_from_header(&(((U8 *)p_api)[EXT_MSG_BUF_LEN_OFFSET]));

    p_source = (U8 *)&(api_hdr->source);

    /*spr 6839 start*/
    if(RRM_SUCCESS == is_big_endian()) /* Coverity: CID 41398 */
    {
        upper_byte_source = (U16)p_source[RRM_ZERO];
    }
    else
    {
        upper_byte_source = (U16)p_source[RRM_ONE];     
    }
    /*spr 6839 end*/
    
    if (RRM_ZERO == api_hdr->msg_size)
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "Api Buffer Received with zero len");
    }
    else if(!((api_hdr->source == RRM_OAM_MODULE_ID)    ||
                (upper_byte_source == RRM_SON_MODULE_ID)))
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "mif_validate_api_hdr: API buffer received with invalid Source[%d or %d] ",
                api_hdr->source, upper_byte_source);
    }
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
            "Interface Header Received with PI = %d, Msg Size = %d, Trans Id = %d, Source = %d, Dest = %d", 
            api_hdr->api_id, api_hdr->msg_size, api_hdr->transaction_id, api_hdr->source, api_hdr->destination);
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mif_validate_stack_api_hdr
 * Inputs         :
 *      1. p_api is not null pointer
 *      2. apIHdr is not null pointer
 * Outputs        :
 * Returns        : Void
 * Variables      :
 * Description    : This function validate the Interface API Header.
 ****************************************************************************/
static void mif_validate_stack_api_hdr(void *p_api, STACKAPIHDR *stack_api_hdr)
{
    U16                       api_len = RRM_ZERO;
    U16                       upper_byte_source = RRM_ZERO;
    U16                       lower_byte_source = RRM_ZERO;
    U8                        *p_source = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != stack_api_hdr);

    api_len = rrm_get_word_from_header(&(((U8 *)p_api)[STACK_API_PARAM_LEN_OFFSET]));
    stack_api_hdr->from = rrm_get_word_from_header(&(((U8 *)p_api)[STACK_API_SRC_MODULE_ID_OFFSET]));
    stack_api_hdr->to = rrm_get_word_from_header(&(((U8 *)p_api)[STACK_API_DEST_MODULE_ID_OFFSET]));
    stack_api_hdr->api = rrm_get_word_from_header(&(((U8 *)p_api)[STACK_API_API_OFFSET]));
    stack_api_hdr->paramlen = rrm_get_word_from_header(&(((U8 *)p_api)[STACK_API_PARAM_LEN_OFFSET]));
    stack_api_hdr->payloadlen = rrm_get_word_from_header(&(((U8 *)p_api)[STACK_API_PAYLOAD_LEN_OFFSET]));

    p_source = (U8 *)&stack_api_hdr->from;

    /*spr 6839 start*/
    if(RRM_SUCCESS == is_big_endian()) /* Coverity: CID 41399 */
    {
        upper_byte_source = (U16)p_source[RRM_ZERO];
        lower_byte_source = (U16)p_source[RRM_ONE];
    }
    else
    {
        lower_byte_source = (U16)p_source[RRM_ZERO];
        upper_byte_source = (U16)p_source[RRM_ONE];
    }
    /*spr 6839 end*/
    /* SPR 21251 +-*/
    if (RRM_ZERO == api_len)
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "CSPL Api Buffer Received with zero len");
    }
    else if(!((stack_api_hdr->from == RRM_OAM_MODULE_ID)    ||
                (stack_api_hdr->from == RRM_CM_MODULE_ID)    ||
                (upper_byte_source == RRM_SON_MODULE_ID)    ||
                (stack_api_hdr->from == RRM_UEM_MODULE_ID)))
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, 
                "mif_validate_stack_api_hdr: API buffer received with invalid Source[%d or %d] ",
                stack_api_hdr->from, upper_byte_source);
    }
    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, 
            "CSPL Buf Rcvd API(%d),From(0x%x),To(0x%x), ParamLen(%d), PayloadSize(%d) upper_byte_source (%d) lower_byte_source (%d)", 
            stack_api_hdr->api, stack_api_hdr->from, stack_api_hdr->to, 
            stack_api_hdr->paramlen, stack_api_hdr->payloadlen, upper_byte_source, lower_byte_source);

    RRM_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : mif_raise_event_to_fsm
 * Inputs         : 
 *      1. rrm_mif_context  is not null pointer
 *      2. p_gl_ctxt is not null pointer
 *      3. mif_return_value is not null pointer
 * Outputs        : None
 * Returns        : None
 * Variables      : None
 * Description    : This function shall raise the event to related FSM.
 *                  We are not using p_api as rvalue so there is no NULL
 *                  check applied on it while entering the function.Also
 *                  No NULL check is done on p_api = p_return_value->p_msg_info
 *                  as this value is expected to be NULL as well as NON NULL
 *                  and in case of NON NULL value we are taking the correct
 *                  action.
 ****************************************************************************/

static void mif_raise_event_to_fsm (rrm_mif_context        *p_mif_context,
        rrm_mif_gb_context_t  *p_gl_ctxt,  
        YLNODE                *p_ret_val_node)
{
    mif_process_main_func           process_main_func = RRM_PNULL;
    mif_return_value               *p_return_value = RRM_PNULL;
    YLIST                       ret_val_list;
   /* coverity id 16509 , ,rel 2.0.1 */
    mif_fsm_type                   src_fsm = NULL_FSM;
    void                        *p_api = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
    RRM_ASSERT (RRM_PNULL != p_ret_val_node); 

    ylInit(&ret_val_list);
    ylPushHead (&ret_val_list,p_ret_val_node);

    while (ylEmpty(&ret_val_list)!= RRM_TRUE)
    {
        p_ret_val_node = ylPopHead(&ret_val_list);
        /*Coverity 61370 Fix Start*/
        if(RRM_NULL != p_ret_val_node)
        {
            p_return_value = (mif_return_value *)p_ret_val_node;
            p_api = p_return_value->p_msg_info;
            src_fsm = p_return_value->src_fsm;
            RRM_ASSERT (p_return_value->fsm_index < MAX_FSM_TYPE);
            process_main_func = (mif_process_main_func)
                (p_gl_ctxt->fsm_process_func[p_return_value->fsm_index]);
            process_main_func(p_gl_ctxt, 
                    p_mif_context,
                    p_return_value->event_id, 
                    p_api, src_fsm);
            /* SPR 11781 start */
            if (p_api != RRM_PNULL)
                /* SPR 11781 end */
            {
                /* This is the incoming message received after parsing */
                RRM_MEM_FREE(p_api);
            }
            RRM_MEM_FREE(p_ret_val_node);
        }
        /*Coverity 61370 Fix End*/
    }

    RRM_UT_TRACE_EXIT();
}



/****************************************************************************
 * Function Name  : rrm_mif_process_msg 
 * Inputs         : p_api is not null pointer
 *                  p_gl_ctxt is not null pointer
 * Outputs        : 
 * Returns        : integer
 * Variables      : 
 * Description    : This is entry point function of the RRM MIF module. It 
 *                  extract the event from the incoming message and raised the 
 *                : appropiate event handler to the respective procedural FSM.
 ****************************************************************************/
S32 rrm_mif_process_msg (void *p_api, void *p_context)
{
    interface_api_header        api_hdr = {RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO};
    rrm_mif_gb_context_t     *p_gl_ctxt = RRM_PNULL;
    rrm_mif_context            *p_mif_context = RRM_PNULL;
    YLNODE                   *p_ret_val_node = RRM_PNULL;
    mif_return_value           *p_return_value = RRM_PNULL;
    rrm_bool_et               return_flag = RRM_FALSE;
    STACKAPIHDR               stack_api_hdr = {RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO};
    /*SPR 10329:start*/
    rrmcm_rmif_enb_config_resp_t                       *p_config_response = RRM_PNULL;
    /*SPR 10329:end*/
    rrmcm_rmif_generic_non_cell_specific_api_resp_t    *genericResp_p = RRM_PNULL;
    rrmcm_rmif_generic_non_cell_specific_api_resp_t    *genericRespSet_p = RRM_PNULL;
  /* coverity id 16516 , ,rel 2.0.1 */
    rrm_oam_log_enable_disable_resp_t                   out_message = {RRM_INDEX_ERR,RRM_NO_ERROR};
  /* coverity id 16520 , ,rel 2.0.1 */
    rrm_oam_set_log_level_resp_t                        out_message_set_log = {RRM_INDEX_ERR,RRM_NO_ERROR};
    rrm_oam_init_config_req_t                           in_message = {RRM_ZERO};
    S32                                                 length_read = RRM_ZERO;
  /* coverity id 16515 , ,rel 2.0.1 */
    rrm_oam_init_config_resp_t                          out_init_config_rep   = {RRM_INDEX_ERR,RRM_NO_ERROR};
    rrm_oam_event_notification_t                  event_notification_ind_to_oam = {RRM_ZERO};
    rrm_return_et                      ret_val = RRM_SUCCESS;
    U16                       upper_byte_source;
    U8                        *p_source = RRM_PNULL;
    /*SPR 10329:fix start*/
    /*Coverity 65862 Fix Start */
    rrm_oam_enb_config_resp_t  *p_enb_config_resp      = RRM_PNULL;      
    /*Coverity 65862 Fix End */
    /*SPR 10329:fix end*/
    /* SPR_13117_FIX_START */
    rrm_oam_get_log_level_resp_t               p_out_message = {RRM_ZERO};
    /* SPR_13117_FIX_END */
    /*SPR 15993 Fix Start*/
    rrmuem_rmif_emergency_call_active_res_t    *p_emer_call_active_res = RRM_PNULL; 
    U16                                        src_id = RRM_ZERO;
    /*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_api);
    RRM_ASSERT (RRM_PNULL != p_context);

    p_gl_ctxt = (rrm_mif_gb_context_t *)p_context;

    mif_validate_stack_api_hdr(p_api, &stack_api_hdr);

    p_source = (U8 *)(&stack_api_hdr.from);

    /*spr 6839 start*/
    if(RRM_SUCCESS == is_big_endian()) /* Coverity: CID 41400 */
    {
        upper_byte_source = (U16)p_source[RRM_ZERO];
    }
    else
    {
        upper_byte_source = (U16)p_source[RRM_ONE];
    }
    /*spr 6839 end*/
    rrm_oam_set_status_for_health(RRM_ZERO);
#ifndef RRM_IN_MEM_LOGGER
    rrm_oam_set_status_for_health(RRM_ONE);
#endif
    if((stack_api_hdr.from == (U16)RRM_OAM_MODULE_ID)     ||
            (upper_byte_source == RRM_SON_MODULE_ID)
      )
    {
        mif_validate_api_hdr(((U8 *)p_api) + RRM_API_HEADER_SIZE, &api_hdr);
    }

    if (p_gl_ctxt->current_state == RMIF_STATE_ACTIVE)
    {
        if ((U16)RRM_OAM_MODULE_ID == stack_api_hdr.from)
        {
            switch (api_hdr.api_id)
            {
                case RRM_OAM_LOG_ENABLE_DISABLE_REQ:
                    {
                        rrm_mif_oam_process_log_enable_req(p_api, p_gl_ctxt, api_hdr.transaction_id);
                        return_flag = RRM_TRUE;
                        break;
                    }
                case RRM_OAM_SET_LOG_LEVEL_REQ:
                    {
                        rrm_mif_oam_process_set_log_level_req (p_api, p_gl_ctxt, api_hdr.transaction_id);
                        return_flag = RRM_TRUE;
                        break;
                    }
                    /* SPR_13117_FIX_START */
                case RRM_OAM_GET_LOG_LEVEL_REQ:
                    {
                        rrm_mif_oam_process_get_log_level_req (p_api, p_gl_ctxt, api_hdr.transaction_id);
                        return_flag = RRM_TRUE;
                        break;
                    }

                    /* SPR_13117_FIX_END */

                case RRM_OAM_SHUTDOWN_REQ:
                    {
                        /* Change state to shutdown in progress */
                        p_gl_ctxt->current_state  = RMIF_STATE_SHUTDOWN_ONGOING;
                        break;
                    }
                case RRM_OAM_RESUME_SERVICE_REQ:
                    {
                        /* Change state to resume in progress */
                        p_gl_ctxt->current_state  = RMIF_STATE_RESUME_ONGOING;
                        break;
                    }
              /* START UE RELEASE */
                case RRM_OAM_UE_RELEASE_REQ:
                    {
                  

                       rrm_mif_oam_process_ue_release_req(p_api);/*SPR 17777+-*/
                       return_flag = RRM_TRUE;
                       break;

                    }
                 /* END OF  UE RELEASE */
                /*START OF GET VERION ID */
		case RRM_OAM_GET_VER_ID_REQ:
                    {
                        rrm_mif_oam_process_get_ver_id_req(api_hdr.transaction_id);
                        return_flag = RRM_TRUE;
                        break;
                    }
                /*END OF GET VERION ID */
                /*START OF LOAD CONFIG REQ*/
                case RRM_OAM_LOAD_CONFIG_REQ:
                    {
                        rrm_mif_oam_process_load_config_req (p_api, p_gl_ctxt, api_hdr.transaction_id);
                        return_flag = RRM_TRUE;
                        break;
                    }
                /*END OF LOAD CONFIG REQ */  
                /* health monitoring changes start*/
                case RRM_OAM_CHK_HEALTH_REQ:
                     {
                        rrm_mif_oam_process_chk_health_req(p_api, p_gl_ctxt,api_hdr.transaction_id);
                        return_flag = RRM_TRUE;
                        break;
                     }
                /* health monitoring changes end */
                /*Event Config Req Start*/
                case RRM_OAM_EVENT_CONFIG_REQ:
                 {
                   rrm_mif_oam_process_event_config_req(p_api, api_hdr.transaction_id); 
                   return_flag = RRM_TRUE;
                   break;
                 }
                /*Event Config Req End*/
                 /* Carrier_Aggregation_Start */
                case RRM_OAM_ENB_CONFIG_REQ:
                 {
                     rrm_mif_oam_process_enb_config_req (p_api, api_hdr.transaction_id);
                     return_flag = RRM_TRUE;
                     break;
                 }
                 /* Carrier_Aggregation_End */
            } 
        }
        else if (((U16)RRM_SON_NMM_MODULE_ID == stack_api_hdr.from)||
                 ((U16)RRM_SON_APPS_MODULE_ID == stack_api_hdr.from))
        {
            switch (api_hdr.api_id)
            {
                case (U16)RRM_SON_NMM_PREPARE_REQ:
                    {
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_req);
#endif        
                        rrm_mif_process_son_nmm_prepare_request(p_gl_ctxt, api_hdr.transaction_id, api_hdr.source);
                        return_flag = RRM_TRUE;
                        break;
                    }
                case (U16)RRM_SON_NMM_COMPLETE_REQ:
                    {
#ifdef RRM_UT_FLAG        
                        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_complete_req);
#endif        
                        rrm_mif_process_son_nmm_comp_request(p_gl_ctxt, api_hdr.transaction_id, api_hdr.source);
                        return_flag = RRM_TRUE;
                        break;
                    }
            }
        }
        else if (((U16)RRM_UEM_MODULE_ID == stack_api_hdr.from) ||
                ((U16)RRM_CM_MODULE_ID == stack_api_hdr.from))
        {
            switch (stack_api_hdr.api)
            {
                case (U16)RRMCM_RMIF_LOG_ENABLE_DISABLE_RESP:
                    {
                        genericResp_p = (rrmcm_rmif_generic_non_cell_specific_api_resp_t*)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Response received for LOG ENABLE/DISABLE %d ", genericResp_p->response);
                        if (RRM_TRUE == p_gl_ctxt->current_req_for_all_modules)
                        {
                            if (RRM_SUCCESS == genericResp_p->response)
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD;
                            }
                            else
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD;
                            }
                            rrm_check_for_log_enable_disable_resp(p_gl_ctxt, 
                                    genericResp_p->fail_cause, 
                                    genericResp_p->transaction_id);
                        }
                        else
                        {
                            if (RRM_SUCCESS == genericResp_p->response)
                            {
                                out_message.response = RRM_SUCCESS;
                                out_message.fail_cause = RRM_NO_ERROR;
                            }
                            else
                            {
                                out_message.response = RRM_FAILURE;
                               /* coverity id 16502 , ,rel 2.0.1 */
                                out_message.fail_cause = (rrm_error_et)(genericResp_p->fail_cause);
                            }
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Sending log_enable_disable_resp to OAM");
                            ret_val = rrm_oam_send_rrm_oam_log_enable_disable_resp(
                                    (void *)&out_message,
                                    RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    genericResp_p->transaction_id,
                                    RRM_ZERO);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Send Failed log_enable_disable_resp to OAM");
                            }
                        }
                        return_flag = RRM_TRUE;
                        break;
                    }
                case (U16)RRMCM_RMIF_SET_LOG_LEVEL_RESP:
                    {
                        genericRespSet_p = (rrmcm_rmif_generic_non_cell_specific_api_resp_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Response received for SET LOG LEVEL %d ", genericRespSet_p->response);
                        if (RRM_TRUE == p_gl_ctxt->current_req_for_all_modules)
                        {
                            if (RRM_SUCCESS == genericRespSet_p->response)
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD;
                            }
                            else
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD;
                            }
                            rrm_check_for_set_log_level_resp(p_gl_ctxt,
                                    genericRespSet_p->fail_cause,
                                    genericRespSet_p->transaction_id);
                        }
                        else
                        {
                            if (RRM_SUCCESS == genericRespSet_p->response)
                            {
                                out_message_set_log.response = RRM_SUCCESS;
                                out_message_set_log.fail_cause = RRM_NO_ERROR;
                            }
                            else
                            {
                                out_message_set_log.response = RRM_FAILURE;
                                out_message_set_log.fail_cause = genericRespSet_p->fail_cause;
                            }
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                            ret_val = rrm_oam_send_rrm_oam_set_log_level_resp (
                                    (void *)&out_message_set_log,
                                    RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    genericRespSet_p->transaction_id,
                                    RRM_ZERO);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                            }
                        }
                        return_flag = RRM_TRUE;
                        break;
                    }
                    /* SPR_13117_FIX_START */
                    case (U16)RRMUEM_RMIF_GET_LOG_LEVEL_RESP:
                    {
                        rrmuem_rmif_get_log_level_resp_t *p_resp = RRM_PNULL;
                        p_resp = (rrmuem_rmif_get_log_level_resp_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        rrm_build_and_get_log_level_resp(&p_out_message,p_resp,p_gl_ctxt);

                        ret_val = rrm_oam_send_rrm_oam_get_log_level_resp(
                                (void *)&p_out_message,
                                RRM_MIF_MODULE_ID,
                                RRM_OAM_MODULE_ID,
                                p_resp->transaction_id,
                                RRM_ZERO);
                        
                        p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_get_log_level_resp to OAM");
                        if(RRM_FAILURE == ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                        }

                        return_flag = RRM_TRUE;
                        break;
                    }
                 /* SPR_13117_FIX_END */
                case (U16)RRMUEM_RMIF_SET_LOG_LEVEL_RES:
                    {
                        rrmuem_rmif_set_log_level_res_t *respSet_p = RRM_PNULL;
                        respSet_p = (rrmuem_rmif_set_log_level_res_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Response received for LOG ENABLE/DISABLE %d ", respSet_p->response);
                        if (RRM_TRUE == p_gl_ctxt->current_req_for_all_modules)
                        {
                            if (RRM_TRUE == respSet_p->response) /* 14185 rel1.3*/
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD;
                            }
                            else
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD;
                            }
                            rrm_check_for_set_log_level_resp(p_gl_ctxt,
                                    RRM_ERR_LOG_ENABLE_FAILURE,
                                    respSet_p->transaction_id);
                        }
                        else
                        {
                            if (RRM_TRUE == respSet_p->response) /* 14186 rel1.3*/
                            {
                                out_message_set_log.response = RRM_SUCCESS;
                                /*This param is not required now*/ 
                                //out_message_set_log.fail_cause = RRM_NO_ERROR;
                            }
                            else
                            {
                                out_message_set_log.response = RRM_FAILURE;
                                /*This param is not required now*/
                                //out_message_set_log.fail_cause = respSet_p->fail_cause;
                            }
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Sending rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                            ret_val = rrm_oam_send_rrm_oam_set_log_level_resp(
                                    (void *)&out_message_set_log,
                                    RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    respSet_p->transaction_id,
                                    RRM_ZERO);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Send Failed rrm_oam_send_rrm_oam_set_log_level_resp to OAM");
                            }
                        }
                        return_flag = RRM_TRUE;
                        break;
                    }
                case (U16)RRMUEM_RMIF_LOG_ENABLE_DISABLE_RES:
                    {
                        rrmuem_rmif_log_enable_disable_res_t  *resp_p = RRM_PNULL;
                        resp_p = (rrmuem_rmif_log_enable_disable_res_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Response received for LOG ENABLE/DISABLE %d ", resp_p->response);
                        if ((U32)RRM_TRUE == (U32)p_gl_ctxt->current_req_for_all_modules)
                        {
                            if (RRM_TRUE == resp_p->response) /* 14186 */
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD;
                            }
                            else
                            {
                                p_gl_ctxt->module_resp |= RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD;
                            }
                            rrm_check_for_log_enable_disable_resp(p_gl_ctxt, 
                                    RRM_ERR_LOG_ENABLE_FAILURE,
                                    resp_p->transaction_id);
                        }
                        else
                        {
                            if (RRM_TRUE == resp_p->response) /* Coverity: CID 14186 */
                            {
                                out_message.response = RRM_SUCCESS;
                                out_message.fail_cause = RRM_NO_ERROR;
                            }
                            else
                            {
                                out_message.response = RRM_FAILURE;
                                /*This param is not required now*/
                                //out_message.fail_cause = resp_p->fail_cause;
                            }
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Sending log_enable_disable_resp to OAM");
                            ret_val = rrm_oam_send_rrm_oam_log_enable_disable_resp(
                                    (void *)&out_message,
                                    RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    resp_p->transaction_id,
                                    RRM_ZERO);
                            if(RRM_FAILURE == ret_val)
                            {
                                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Send Failed log_enable_disable_resp to OAM");
                            }
                        }
                        return_flag = RRM_TRUE;
                        break;
                    }
                    case RRMUEM_RMIF_EVENT_NOTIFICATION_IND:
		    {
                        rrmuem_rmif_event_notification_ind_t *event_ind = RRM_PNULL;
                        event_ind = (rrmuem_rmif_event_notification_ind_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                     
                        /* populating event notification msg */
                        rrm_oam_fill_event_notification_ind_msg(&event_notification_ind_to_oam, event_ind);                       

                        /* Sending msg to oam module */
                        rrm_oam_send_rrm_oam_event_notification(&event_notification_ind_to_oam, 
                                                                RRM_MIF_MODULE_ID, 
                                                                RRM_OAM_MODULE_ID,
                                                                RRM_ZERO,
                                                                RRM_ZERO);   

                        return_flag = RRM_TRUE;
                        break;
                    }
                    case (U16)RRMCM_RMIF_LOAD_CONFIG_RESP:
                    {
                        rrmcm_rmif_generic_non_cell_specific_api_resp_t *resp_p = RRM_PNULL;
                        resp_p = (rrmcm_rmif_generic_non_cell_specific_api_resp_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
                                "CellM->MIF:RRMCM_RMIF_LOAD_CONFIG_RESP response %d", resp_p->response);
                        if (RRM_FAILURE == resp_p->response)
                        {
                            out_message.response = RRM_FAILURE;
			    /* Coverity 23127, typecast U16 to rrm_error_et in rel 1.3:  */
                            out_message.fail_cause = (rrm_error_et)resp_p->fail_cause;
                        }
                        else
                        {
                                out_message.response = RRM_SUCCESS;
                                out_message.fail_cause = RRM_NO_ERROR;
                        }

                        ret_val = rrm_oam_send_rrm_oam_load_config_resp(
                                      (void *)&out_message,
                                       RRM_MIF_MODULE_ID,
                                       RRM_OAM_MODULE_ID,
                                       resp_p->transaction_id,
                                       RRM_ZERO);
                       if(RRM_FAILURE == ret_val)
                       {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR,
                                          "Send Failed load_config_resp to OAM");
                       }

                        return_flag = RRM_TRUE;
                        break;
                    }

                    case (U16)RRMUEM_RMIF_EVENT_CONFIG_RESP:
                    {
                     /* coverity id 29720, ,rel 2.0.1 */
                      rrm_oam_event_config_resp_t       event_config_resp = {RRM_INDEX_ERR,RRM_NO_ERROR};
                      rrmuem_rmif_event_config_resp_t  *p_event_config_resp = RRM_PNULL;
                      p_event_config_resp = (rrmuem_rmif_event_config_resp_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);

                      if (RRM_FALSE == p_event_config_resp->response)
                       {
                         event_config_resp.response = RRM_FAILURE;                            
                         event_config_resp.fail_cause = RRM_ERR_INTERNAL_FAILURE;
                       }
                      else
                       {
                         event_config_resp.response = RRM_SUCCESS;
                         event_config_resp.fail_cause = RRM_NO_ERROR;
                       }

                      /*Sending response to OAM*/
                      rrm_oam_send_rrm_oam_event_config_resp(&event_config_resp,
                                                             RRM_MIF_MODULE_ID,
                                                             RRM_OAM_MODULE_ID,
                                                             p_event_config_resp->transaction_id,
                                                             RRM_ZERO);
                      return_flag = RRM_TRUE;
                      break;
                    }
                /* Carrier_Aggregation_Start */
                    case (U16)RRMCM_RMIF_ENB_CONFIG_RESP:
		    {
			    /*Coverity 65862 Fix Start */
			    p_enb_config_resp = (rrm_oam_enb_config_resp_t *)rrm_mem_get(
					    sizeof(rrm_oam_enb_config_resp_t));
			    if (p_enb_config_resp == RRM_PNULL)
			    {

				    RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_BRIEF, 
						    "Memory allocation to p_enb_config_resp failed");
				    rrm_abort();
			    }
			    RRM_MEMSET(p_enb_config_resp, RRM_ZERO, sizeof(rrm_oam_enb_config_resp_t));
			    /*Coverity 65862 Fix End */

			    /*SPR 10329:start*/
			    p_config_response = (rrmcm_rmif_enb_config_resp_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                RRM_TRACE(g_cellm_log_on_off, p_g_rrm_cellm_facility_name, RRM_INFO,
                        "CellM->MIF:RRMCM_RMIF_ENB_CONFIG_RESP response %d", p_config_response->response);
			    /* Coverity_ID_Fix : 60873 */
			    if (RRM_FAILURE == p_config_response->response)
				    /*SPR 10329:end*/
			    {
				    /*Coverity 65862 Fix Start */
				    p_enb_config_resp->response = RRM_FAILURE;
				    /*Coverity 65862 Fix End */
			    }
			    else if(RRM_PARTIAL_SUCCESS == p_config_response->response)
			    {
				    /*Coverity 65862 Fix Start */
				    p_enb_config_resp->response = RRM_PARTIAL_SUCCESS;
				    /*Coverity 65862 Fix End */
			    }
			    else
			    {
				    /*Coverity 65862 Fix Start */
				    p_enb_config_resp->response = RRM_SUCCESS;
				    /*Coverity 65862 Fix End */
			    }
			    /*SPR 10329:start*/
			    /*Coverity 65862 Fix Start */
			    p_enb_config_resp->fail_cause = p_config_response->fail_cause;
			    if((p_enb_config_resp->response == RRM_FAILURE) || (p_enb_config_resp->response == RRM_PARTIAL_SUCCESS))
				    /*Coverity 65862 Fix End */
			    {
				    /*SPR 10329:fix start*/
				    if (p_config_response->bitmask == RRM_GU_GROUP_RESPONSE_LIST_PRESENT)
				    {
					    /*Coverity 65862 Fix Start */
					    p_enb_config_resp->bitmask |= RRM_OAM_GU_GROUP_RESPONSE_LIST_PRESENT;  
					    RRM_MEMCPY(&(p_enb_config_resp->gu_group_response_list),&(p_config_response->gu_group_response_list),
							    /*Coverity 65862 Fix End */
							    sizeof(rrm_gu_group_info_success_failure_list_t));
				    }
				    /*SPR 10329:fix end*/
			    }
			    /*SPR 10329:end*/

			    /*Coverity 65862 Fix Start */
			    ret_val = rrm_oam_send_rrm_oam_enb_config_resp(
					    (void *)p_enb_config_resp,
					    /*Coverity 65862 Fix End */
					    RRM_MIF_MODULE_ID,
					    RRM_OAM_MODULE_ID,
					    /*SPR 10329:start*/
					    p_config_response->transaction_id,
					    /*SPR 10329:end*/
					    RRM_ZERO);
			    if(RRM_FAILURE == ret_val)
			    {
				    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED,
						    "Send Failed enb_config_resp to OAM");
			    }
			    /*Coverity 65862 Fix Start */
			    RRM_MEM_FREE(p_enb_config_resp);
			    /*Coverity 65862 Fix End */

			    return_flag = RRM_TRUE;
			    break;
		    }
                /* Carrier_Aggregation_End */

                default:
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Api(%d) rcvd frm Module(%d) for MIF FSM", 
                                stack_api_hdr.api, stack_api_hdr.from);
                        break;
                    }
            }
        }
        if (return_flag == RRM_TRUE)
        {
            RRM_MSG_MEM_FREE(p_api);
            return RRM_FAILURE;
        }

        p_ret_val_node = (YLNODE *)rrm_mem_get(sizeof(mif_return_value));
        if ( p_ret_val_node == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_ret_val_node failed" );
            RRM_UT_TRACE_EXIT();
            return RRM_FAILURE;
        }
        
        p_return_value = (mif_return_value *)p_ret_val_node;
        RRM_ASSERT (RRM_PNULL != p_return_value);
        p_return_value->fsm_index  = NULL_FSM;
        p_return_value->event_id   = RRM_ZERO;
        p_return_value->p_msg_info = RRM_PNULL;
        p_mif_context = mif_message_parser (p_gl_ctxt, p_api, &api_hdr, &stack_api_hdr,
                p_return_value);
        if (p_mif_context != RRM_PNULL && p_return_value->fsm_index != NULL_FSM)
        {
            /* SPR 21251 +- */
            mif_raise_event_to_fsm (p_mif_context, p_gl_ctxt,
                    p_ret_val_node);
            return_flag = RRM_FALSE;
        }
        else
        {
            RRM_MSG_MEM_FREE(p_api);
            RRM_MEM_FREE(p_ret_val_node);
            return_flag = RRM_TRUE;
        }           
    }
    else if (p_gl_ctxt->current_state == RMIF_STATE_SHUTDOWN_ONGOING)
    {
        /* Coverity: CID 14184 */
        if ( (((U16)RRM_CM_MODULE_ID == stack_api_hdr.from) &&
                    ((U16)RRMCM_RMIF_CELL_STOP_ADMISSION_RESP == stack_api_hdr.api)) ||
                (((U16)RRM_UEM_MODULE_ID == stack_api_hdr.from) && 
                 ((U16)RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES == stack_api_hdr.api)) ||
                (((U16)RRM_CM_MODULE_ID == stack_api_hdr.from) &&
                 ((U16)RRMCM_RMIF_CELL_DELETE_RESP == stack_api_hdr.api)) 
           )
        {
            p_ret_val_node = (YLNODE *)rrm_mem_get(sizeof(mif_return_value));
        if ( p_ret_val_node == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_ret_val_node failed" );
            RRM_UT_TRACE_EXIT();
            return RRM_FAILURE;
        }

            p_return_value = (mif_return_value *)p_ret_val_node;
            RRM_ASSERT (RRM_PNULL != p_return_value);
            p_return_value->fsm_index  = NULL_FSM;
            p_return_value->event_id   = RRM_ZERO;
            p_return_value->p_msg_info = RRM_PNULL;
            p_mif_context = mif_message_parser (p_gl_ctxt, p_api, &api_hdr, &stack_api_hdr,
                    p_return_value);
            if (p_mif_context != RRM_PNULL && p_return_value->fsm_index != NULL_FSM)
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "DEBUG: Calling mif_raise_event_to_fsm");
                mif_raise_event_to_fsm (p_mif_context, p_gl_ctxt,
                        p_ret_val_node);
                /*If shutdown is ongoing and for all cells no emerg call is ongoing for all the Cell. send delete to all cells*/
                if((RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state) &&  
                    ((U16)RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES == stack_api_hdr.api)) /* Coverity: CID 14184 */
                {
                    /*SPR 15993 Fix Start*/
                    p_emer_call_active_res = (rrmuem_rmif_emergency_call_active_res_t*)((U8 *)p_api + RRM_API_HEADER_SIZE);
                    src_id = return_src_id_from_tcb(p_mif_context,
                                    p_emer_call_active_res->transaction_id);
                    if ((RRM_ZERO == p_gl_ctxt->bs_cellmap) && (RRM_OAM_MODULE_ID == src_id))
                    /*SPR 15993 Fix End*/
                    {
                        rrm_mif_send_event_to_all_cells(MIF_FSM_EVENT_CELL_DELETE_REQ,
                                RRM_PNULL,
                                p_gl_ctxt,
                                p_mif_context->currently_processed_api_trans_d,
                                RRM_OAM_MODULE_ID);
                    /*SPR 15993 Fix Start*/
                        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, 
                                       p_emer_call_active_res->transaction_id);

                        if(RRM_FAILURE != ret_val)
                        {
                            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                                             "TCB De-Allocated successfully");
                        }
                    /*SPR 15993 Fix End*/
                    }
                }
                return_flag = RRM_FALSE;
            }
            else
            {
                RRM_MSG_MEM_FREE(p_api);
                RRM_MEM_FREE(p_ret_val_node);
                return_flag = RRM_TRUE;
            }
        }
        else if((U16)RRM_OAM_MODULE_ID == stack_api_hdr.from && (U16)RRM_OAM_CHK_HEALTH_REQ == stack_api_hdr.api)
        {
            rrm_mif_oam_process_chk_health_req(p_api, p_gl_ctxt,api_hdr.transaction_id);
            return_flag = RRM_TRUE;  
        }
        else
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, 
                    "Unexpected message [%d] from modue [%d] in state \
                    SHUTDOWN_ONGOING", stack_api_hdr.api, stack_api_hdr.from);
            return_flag = RRM_FALSE;
        }
    }
    else if (p_gl_ctxt->current_state == RMIF_STATE_RESUME_ONGOING)
    {
        if (((U16)RRM_CM_MODULE_ID == stack_api_hdr.from) &&
                ((U16)RRMCM_RMIF_CELL_START_ADMISSION_RESP == stack_api_hdr.api))
        {
            p_ret_val_node = (YLNODE *)rrm_mem_get(sizeof(mif_return_value));
            if ( p_ret_val_node == RRM_PNULL)
            {
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                        "Memory allocation to p_ret_val_node failed" );
                RRM_UT_TRACE_EXIT();
                return RRM_FAILURE;
            }

            p_return_value = (mif_return_value *)p_ret_val_node;
            RRM_ASSERT (RRM_PNULL != p_return_value);
            p_return_value->fsm_index  = NULL_FSM;
            p_return_value->event_id   = RRM_ZERO;
            p_return_value->p_msg_info = RRM_PNULL;
            p_mif_context = mif_message_parser (p_gl_ctxt, p_api, &api_hdr, &stack_api_hdr,
                    p_return_value);
            if (p_mif_context != RRM_PNULL && p_return_value->fsm_index != NULL_FSM)
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "DEBUG: Calling mif_raise_event_to_fsm");
                mif_raise_event_to_fsm (p_mif_context, p_gl_ctxt,
                        p_ret_val_node);
                return_flag = RRM_FALSE;
            }
            else
            {
                RRM_MSG_MEM_FREE(p_api);
                RRM_MEM_FREE(p_ret_val_node);
                return_flag = RRM_TRUE;
            }
        }
        else if((U16)RRM_OAM_MODULE_ID == stack_api_hdr.from && (U16)RRM_OAM_CHK_HEALTH_REQ == stack_api_hdr.api)
        {
            rrm_mif_oam_process_chk_health_req(p_api, p_gl_ctxt,api_hdr.transaction_id);
            return_flag = RRM_TRUE;  
        }
        else
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, "Unexpected message [%d] from module [%d] in state RESUME_ONGOING", 
                    stack_api_hdr.api, stack_api_hdr.from);
            return_flag = RRM_FALSE;
        }
    }
    else if (p_gl_ctxt->current_state == RMIF_STATE_IDLE)
    {
        if (((U16)RRM_SON_MODULE_ID == upper_byte_source) ||
                ((U16)RRM_OAM_MODULE_ID == stack_api_hdr.from))
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, "Ignoring API Id = %d, From = %d in State = %d", 
                    stack_api_hdr.api, stack_api_hdr.from, p_gl_ctxt->current_state);
            /* p_api will be freed because return_flag = FALSE */
            return_flag = RRM_FALSE;
        }
        else if (((U16)RRM_CM_MODULE_ID == stack_api_hdr.from) ||
                ((U16)RRM_UEM_MODULE_ID == stack_api_hdr.from) )
        {
            /* Process Init Ind from CellM and UeM here */
            switch(stack_api_hdr.api)
            {
                case (U16)RRMCM_RMIF_INIT_IND:
                    {
                        /*SPR 17777 +-*/
                        rrm_mif_rrmcm_init_ind_handler(p_gl_ctxt);
                        break;
                    }

                case (U16)RRMUEM_RMIF_INIT_IND:
                    {
                        rrm_mif_rrmuem_init_ind_handler(p_gl_ctxt);
                        /*SPR 17777 +-*/
                        break;
                    }

                case RRMUEM_RMIF_EVENT_NOTIFICATION_IND:
                    {
                        rrmuem_rmif_event_notification_ind_t *p_event_ind = RRM_PNULL;
                        p_event_ind = (rrmuem_rmif_event_notification_ind_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);

                        /* populating event notification msg */
                        rrm_oam_fill_event_notification_ind_msg(&event_notification_ind_to_oam, p_event_ind);

                        /* Sending msg to oam module */
                        rrm_oam_send_rrm_oam_event_notification(&event_notification_ind_to_oam, 
                                                                 RRM_MIF_MODULE_ID, 
                                                                 RRM_OAM_MODULE_ID,
                                                                 RRM_ZERO,
                                                                 RRM_ZERO);
                        break;
                    }

                default:
                    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Unknown Api(%d) received from Module(%d)", 
                            stack_api_hdr.api, stack_api_hdr.from);
                    break;
            }
        }
    }
    else if (p_gl_ctxt->current_state == RMIF_STATE_INIT) 
    {
        if (((U16)RRM_OAM_MODULE_ID == stack_api_hdr.from) &&
                ((U16)RRM_OAM_INIT_CONFIG_REQ == stack_api_hdr.api))
        {
#ifdef RRM_UT_FLAG        
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_req);
#endif        
            /* Stop the init indication timer */
            if (p_gl_ctxt->timer_id[MIF_INIT_INDICATION_TIMER] != RRM_PNULL)
            {
                mif_stop_timer(p_gl_ctxt->timer_id[MIF_INIT_INDICATION_TIMER]);
                p_gl_ctxt->timer_id[MIF_INIT_INDICATION_TIMER] = RRM_PNULL; 
            }
            if (RRM_SUCCESS != rrm_parse_rrm_oam_init_config_req(
                        (void *)&in_message,
                        (U8*)p_api + RRM_FULL_INTERFACE_HEADERS_SIZE,
                        api_hdr.msg_size - RRM_INTERFACE_API_HEADER_SIZE,
                        (void *)&length_read))
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_ERROR, "Parsing rrm_parse_rrm_oam_init_config_req Failed");
                out_init_config_rep.response = RRM_FAILURE;
                /* coverity : CID 16507*/
                out_init_config_rep.fail_cause = RRM_ERR_INIT_CONFIG_FAILURE;
           
#ifdef RRM_UT_FLAG        
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_resp_F);
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_resp_F_cell_parse);
#endif        
                /* COMPOSER send the message to OAM AGENT */ 
                ret_val = rrm_oam_send_rrm_oam_init_config_resp(
                        (void *)&out_init_config_rep,
                        RRM_MIF_MODULE_ID,
                        RRM_OAM_MODULE_ID,
                        api_hdr.transaction_id,
                        RRM_ZERO);
                /* Coverity ID 77218 Fix Start*/
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "rrm_oam_send_rrm_oam_init_config_resp failed.");
                }
                /* Coverity ID 77218 Fix End*/
                RRM_MSG_MEM_FREE(p_api);
                return RRM_FAILURE;
            }
            /*health monitoring changes start */
            if (in_message.bitmask & RRM_OAM_MODULE_HEALTH_TIMER_PRESENT)
            {
                p_gl_ctxt->health_timer = in_message.health_update_timer;
            }
            /*health monitoring changes end */
            //RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Calling print for init_config_request....");
            //rrm_oam_print_rrm_oam_init_config_req(&in_message, 0, 0);
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Calling build_and_send_init_config_request....");
            build_and_send_init_config_request (p_gl_ctxt, &in_message, api_hdr.transaction_id);
        }
        else if (((U16)RRM_UEM_MODULE_ID==stack_api_hdr.from)&&
                 ((U16)RRMUEM_RMIF_EVENT_NOTIFICATION_IND==stack_api_hdr.api))
	{
	     rrmuem_rmif_event_notification_ind_t *event_ind = RRM_PNULL;
	     event_ind = (rrmuem_rmif_event_notification_ind_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);

	     /* populating event notification msg */
	     rrm_oam_fill_event_notification_ind_msg(&event_notification_ind_to_oam, event_ind);

	     /* Sending msg to oam module */
	     rrm_oam_send_rrm_oam_event_notification(&event_notification_ind_to_oam, 
                                                 RRM_MIF_MODULE_ID, 
                                                 RRM_OAM_MODULE_ID,
                                                 RRM_ZERO,
                                                 RRM_ZERO);

	     return_flag = RRM_TRUE;
	}
    /*SPR 10329:start*/
    
    else if(((U16)RRM_SON_MODULE_ID == stack_api_hdr.from)&&
            ((U16)RRM_SON_ENB_CONFIG_UPDATE_IND == stack_api_hdr.api))
    {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF,
                "RRM_SON_ENB_CONFIG_UPDATE_IND message [%d] from modue [%d] in state \
                SHUTDOWN_ONGOING", stack_api_hdr.api, stack_api_hdr.from);
    }
    /*SPR 10329:end*/

        else
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, "Ignoring API Id = %d, From = %d in State = %d", 
                    stack_api_hdr.api, stack_api_hdr.from, p_gl_ctxt->current_state);
            /* p_api will be freed because return_flag = FALSE */
            return_flag = RRM_FALSE;
        }
    }
    else if (p_gl_ctxt->current_state == RMIF_STATE_INIT_CONFIG_ONGOING)
    {
        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "State = %d", RMIF_STATE_INIT_CONFIG_ONGOING);
        if (((U16)RRM_OAM_MODULE_ID == stack_api_hdr.from)||
                (RRM_SON_MODULE_ID == upper_byte_source))
        {
            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, "Ignoring API Id = %d, From = %d in State = %d", 
                    stack_api_hdr.api, stack_api_hdr.from, p_gl_ctxt->current_state);
            /* p_api will be freed because return_flag = FALSE */
            return_flag = RRM_FALSE;
        }
        else if( ((U16)RRM_CM_MODULE_ID == stack_api_hdr.from) ||
                ((U16)RRM_UEM_MODULE_ID == stack_api_hdr.from) )
        {
            /* Process Init Config Response from CellM and UeM here */
            switch(stack_api_hdr.api)
            {
                case (U16)RRMCM_RMIF_INIT_CONFIG_RESP:
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Received RRMCM_RMIF_INIT_CONFIG_RESP");
                        rrm_mif_rrmcm_init_config_resp_handler(p_gl_ctxt,
                                (U8*)p_api + RRM_API_HEADER_SIZE);
                        break;
                    }

                case (U16)RRMUEM_RMIF_INIT_CONFIG_RES:
                    {
                        RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_DETAILED, "Received RRMUEM_RMIF_INIT_CONFIG_RES");
                        rrm_mif_rrmuem_init_config_resp_handler(p_gl_ctxt,
                                (U8*)p_api + RRM_API_HEADER_SIZE);
                        break;
                    }

                case (U16)RRMUEM_RMIF_EVENT_NOTIFICATION_IND:
                    {
                        rrmuem_rmif_event_notification_ind_t *event_ind = RRM_PNULL;
                        event_ind = (rrmuem_rmif_event_notification_ind_t *)((U8 *)p_api + RRM_API_HEADER_SIZE);
                        /* populating event notification msg */
                        rrm_oam_fill_event_notification_ind_msg(&event_notification_ind_to_oam, event_ind);
                        /* Sending msg to oam module */
                        rrm_oam_send_rrm_oam_event_notification(&event_notification_ind_to_oam, 
                                                                RRM_MIF_MODULE_ID, 
                                                                RRM_OAM_MODULE_ID,
                                                                RRM_ZERO,
                                                                RRM_ZERO
                                                                );
                        break;
                    }
            }
        }
    }
    if(RRM_FALSE == return_flag)
    {
        RRM_MSG_MEM_FREE(p_api);
    }
    RRM_UT_TRACE_EXIT();
    return (RRM_FAILURE);
}


/****************************************************************************
 * Function Name  : mifProcessTimerMsg
 * Inputs         : 
 *      1. timer_id : Timer ID return by cspl
 *      2. p_buf    is not null pointer
 *      3. p_context is not null pointer
 * Outputs        : 
 * Returns        : void
 * Variables      : 
 * Description    : This the entry function for the timer expiries.No NULL check
 *                : is done on p_api = p_return_value->p_msg_info as it can return
 *                : with a NULL as well as NON NULL value. NULL in case of
 *                : Timer Expiry Events.
 ****************************************************************************/
void mifProcessTimerMsg(QTIMER timer_id, void *p_buf, void *p_context)
{
    rrm_mif_gb_context_t         *p_gl_ctxt = RRM_PNULL;
    mif_timer_buf                *p_timer_buf = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_buf);
    RRM_ASSERT (RRM_PNULL != p_context);

    p_gl_ctxt  = (rrm_mif_gb_context_t *)p_context;
    p_timer_buf      = (mif_timer_buf *)p_buf;

    /* SPR 15822 Fix Start */
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, 
            "Timer Expired: Timer (%p), Type(%d), SuperState:%s[%d]", 
            timer_id, p_timer_buf->timer_type, 
            MIF_SUPER_STATE[p_gl_ctxt->current_state],
            p_gl_ctxt->current_state);
    /* SPR 15822 Fix End */

    switch (p_gl_ctxt->current_state)
    {
        case RMIF_STATE_INIT:
            {
                switch (p_timer_buf->timer_type)
                {
                    case MIF_INIT_INDICATION_TIMER:
                        {
                            rrm_mif_process_init_ind_timer_expiry(p_gl_ctxt);
                            break;
                        }
                    default:
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Unexpected event in this state");
                            break;
                        }
                }
                break;
            }
        case RMIF_STATE_SHUTDOWN_ONGOING:
            {
                switch (p_timer_buf->timer_type)
                {
                    case MIF_TIME_TO_SHUTDOWN_TIMER:
                        {
                            rrm_mif_process_shutdown_timer_expiry(p_gl_ctxt, p_timer_buf); 
                            break;
                        }
                    default:
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Unexpected event in this state");
                            break;
                        }
                }
                break;
            }
        case RMIF_STATE_ACTIVE:
            {
                switch (p_timer_buf->timer_type)
                {
                    case MIF_TIME_TO_SHUTDOWN_TIMER:
                    case MIF_BLOCK_CELL_TIMER:
                        {
                            rrm_mif_process_switchoff_timer_expiry(p_gl_ctxt, p_timer_buf); 
                            break;
                        }
                    default:
                        {
                            RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Unexpected event in this state");
                            break;
                        }
                }
                break;
            }
        default:
            {
                RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_WARNING, "Unexpected event in this state");
                break;
            }
    }

    /* Free the timer buffer */
    RRM_MEM_FREE(p_timer_buf);

    RRM_UT_TRACE_EXIT();
    return;
}

static const unsigned long RrmMifDestinationList[] =
{
    RRM_CM_MODULE_ID,
    RRM_UEM_MODULE_ID,
    RRM_ZERO
};


/****************************************************************************
 * Function Name  : rrm_mif_init
 * Inputs         : Initial context data for the module
 * Outputs        : None
 * Returns        : Context of the module
 * Description    : Initialize and create context for the module at the time of
 *                  initialization
 ****************************************************************************/

void* rrm_mif_init
(
 /* Pointer to stack initialization data. Unused */
 void *p_initData
 )
{
    U8               index = RRM_ZERO;
    rrm_mif_gb_context_t     *p_gl_ctxt = RRM_PNULL;


    p_gl_ctxt = (rrm_mif_gb_context_t *)rrm_mem_get(sizeof(rrm_mif_gb_context_t));

    if ( p_gl_ctxt == RRM_PNULL)
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_gl_ctxt failed;Unused variable p_initData=%p",p_initData );/*SPR 17777 +-*/
        /*SPR 17777 +-*/
        RRM_UT_TRACE_EXIT();
        return RRM_PNULL;
    }
    RRM_MEMSET(p_gl_ctxt, RRM_ZERO, sizeof(rrm_mif_gb_context_t));
    RRM_INIT_LOG(RRM_PNULL);
    /* Initialise the MIF Global Context */
    /*SPR_15766 FIX START*/
    p_gl_ctxt->cell_state = RRM_CELL_INIT;
    /*SPR_15766 FIX END*/
    p_gl_ctxt->init_ind = RRM_ZERO;
    p_gl_ctxt->init_config_ind = RRM_ZERO;
    p_gl_ctxt->current_req_for_all_modules = RRM_FALSE;
    p_gl_ctxt->module_resp = RRM_ZERO;
    p_gl_ctxt->current_state = RMIF_STATE_IDLE;
    p_gl_ctxt->fsm_process_func[MIF_FSM] = (void *)rrm_mif_fsm_process_event;
    p_gl_ctxt->log_level = RRM_OAM_DETAILED;
  /* coverity id 16514 , ,rel 2.0.1 */
    p_gl_ctxt->log_enabled = RRM_OAM_LOG_ON;
    /* MLB Changes start */
    /* MLB Changes end */
    /* CSR 00051025 : code changes start */
    p_gl_ctxt->is_mif_ctxt_pool_created = RRM_FALSE;
    p_gl_ctxt->bs_cellmap = RRM_ZERO;
    p_gl_ctxt->mif_ctxt_pool.bufsize = RRM_ZERO;
    p_gl_ctxt->mif_ctxt_pool.min = RRM_ZERO;
    p_gl_ctxt->mif_ctxt_pool.max = RRM_ZERO;
    p_gl_ctxt->mif_ctxt_pool.step = RRM_ZERO;
    p_gl_ctxt->mif_ctxt_pool.pool = RRM_PNULL;
    p_gl_ctxt->rac_response_sent_to_oam = RRM_FALSE;
   /* CSR 00051025 : code changes end */
    p_gl_ctxt->health_timer = RRM_DEFINED_DEFAULT_VALUE_FOR_HEALTH_TIMER;


    /*STATIC ICIC START*/
  /* coverity id 16512 , ,rel 2.0.1 */
    p_gl_ctxt->cell_resp_flag = RRM_TRUE;
    /*STATIC ICIC END*/   
    /*Setting Log level*/
    SET_MODULE_LOG_LEVEL(RRM_OAM_BRIEF);
    
    for (index = RRM_ZERO; index < MIF_MAX_NUM_TIMER_TYPE; index++)
    {
        RRM_ASSERT(index <= MIF_MAX_NUM_TIMER_TYPE);
        p_gl_ctxt->timer_val[index] = MIF_DEFAULT_TIMER_VAL;
        p_gl_ctxt->timer_id[index] = RRM_PNULL;
    }
    for (index = RRM_ZERO; index < MAX_NUM_MIF_CTXT; index++)
    {
        RRM_ASSERT(index <= MAX_NUM_MIF_CTXT);
        p_gl_ctxt->map_info[index].mif_ctxt_id = RRM_OUT_OF_RANGE;
        p_gl_ctxt->map_info[index].is_info_valid = RRM_FALSE;
        rrm_memset_rrm_oam_eutran_global_cell_id(&p_gl_ctxt->map_info[index].global_cell_id, RRM_ZERO);
    }

    /* Initialising structure rrm_rrc_params_from_cfg_t */
		/* Initialize the link list to store the populated data */
    /* SPR 20653 Fix Start */
    /* Code Removed */
    /* SPR 20653 Fix End */

    RRM_TRACE(g_mif_log_on_off, p_g_rrm_mif_facility_name, RRM_BRIEF, 
            "MIF Intialized");

    return p_gl_ctxt;
}


const   QMANIFEST CellMifEntity =
{
    "RRM_MIF_MODULE_ID",
    RRM_MIF_MODULE_ID,

    { RRM_ZERO, rrm_mif_init },

    { rrm_mif_process_msg, mifProcessTimerMsg },

    { RRM_ZERO, RRM_ZERO, RRM_ZERO },

    RrmMifDestinationList
};

/* SPR 20653 Fix Start */
/* Code removed */
/* SPR 20653 Fix End */


/*SPR 10329:start*/
/****************************************************************************
 * Function Name  : build_and_send_son_enb_config_update_ind_to_cellm
 * Inputs         : p_in_enb_config_update_ind 
 * Outputs        : None
 * Returns        : void
 * Description    : This function builds and sends enb_config_update_ind
 *                  to Cell Manager
 ****************************************************************************/
rrm_void_t
build_and_send_son_enb_config_update_ind_to_cellm
(
    rrm_son_enb_config_update_ind_t *p_in_enb_config_update_ind

)
{
    RRM_UT_TRACE_ENTER();

    rrmcm_rmif_enb_config_update_ind_t  *p_out_enb_config_update_ind;
    p_out_enb_config_update_ind = rrm_mem_get(sizeof(rrmcm_rmif_enb_config_update_ind_t));
    /*CID 65857,65771:start*/
    if(RRM_PNULL != p_out_enb_config_update_ind) 
    {
        /*10329_START*/
        p_out_enb_config_update_ind->bitmask = 0x00;
        /*10329_END*/

        if ( p_out_enb_config_update_ind == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_out_enb_config_update_ind failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        }

        RRM_MEMCPY(&(p_out_enb_config_update_ind->nbr_enb_id ), &(p_in_enb_config_update_ind->nbr_enb_id),
                sizeof(rrm_global_enb_id_t));

        if(RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT & p_in_enb_config_update_ind->bitmask )
        {
            p_out_enb_config_update_ind->bitmask |= RRMCM_RMIF_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
            RRM_MEMCPY(&(p_out_enb_config_update_ind->add_gu_id ), &(p_in_enb_config_update_ind->add_gu_id),
                    sizeof(rrmcm_mif_gu_group_id_list_t));
        }

        if(RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & p_in_enb_config_update_ind->bitmask)
        {
            p_out_enb_config_update_ind->bitmask |= RRMCM_RMIF_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;
            RRM_MEMCPY(&(p_out_enb_config_update_ind->delete_gu_id), &(p_in_enb_config_update_ind->delete_gu_id),
                    sizeof(rrmcm_mif_gu_group_id_list_t));
        }
        /*Sending ENB CONFIG UPDATE IND to CellM */
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,"Sending  ENB CONFIG UPDATE IND  to %d",
                RRM_OAM_CM_MODULE_ID);

        rrm_mif_send_internal_msg(RRM_OAM_CM_MODULE_ID,
                RRMCM_RMIF_ENB_CONFIG_UPDATE_IND,
                sizeof(rrmcm_rmif_enb_config_update_ind_t),
                p_out_enb_config_update_ind);

        RRM_MEM_FREE(p_out_enb_config_update_ind);
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
    }
    /*CID 65857,65771:end*/
    RRM_UT_TRACE_EXIT(); 
}

/*SPR 10329:end*/
