/*****************************************************************************
 * File Name   : oam_hm.c
 *
 * Description : This file contains OAM Health Monitoring Code and
 *               its associated routines.
 *
 * Revision History:
 *
 * Date           Author           Reference       Comments
 * -----          ------           ---------       --------
 * October,2012   Harsh Parmar     RCM ICD         Initial Version
 * September,2013 Neeraj Kumar     OAM Design Doc  Changes for HM 
 * May, 2014      Ankita Jain      Klocwork, coverity fixes
 * June, 2014	  Ekta Jain		   SPR FIX 11480
 * JUly 2014        Shalu SAgar     SPR 12421 FIX
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved 
 *****************************************************************************/
#include<oam_hm.h>
#include<oam_sim_utils.h>
#include<oam_fault_mgmt.h>
#include<oam_rrm_composer.h>
#include<oam_fm_alarm_defs.h>
LOCAL oam_hm_context_t hm_data = {OAM_ZERO};
LOCAL UInt8 retry_counter = OAM_ZERO;
extern oam_prov_req_t oam_prov_req;
/*SPR 11480 FIX START*/
extern oam_bool_et shutdown_flag;
/*SPR 11480 FIX END*/
/*SPR 21369 Start*/
extern UInt8 num_mac_init_cell[OAM_MAX_NUM_L2_INSTANCE];
LOCAL UInt16 tcb_transaction_id= OAM_ZERO;
extern lower_layer_init_info_t l2_comm_info[OAM_MAX_NUM_L2_INSTANCE];
/*SPR 21369 End */

/* SPR 21889 +- */
/******************************************************************************
*   FUNCTION NAME : oam_health_monitoring_init
*   DESCRIPTION   : This function initialises health monitoring
*                   context structure with default values
*   Inputs        : NONE
*   Outputs       : NONE
*   RETURNS       : OAM_SUCCESS/OAM_FAILURE 
******************************************************************************/
/*-----This function initialises health monitoring context structure with default values-----*/
oam_return_et 
oam_health_monitoring_init(
/* + 17439 */
void
)
/* - 17439 */
{
  OAM_FUN_ENTRY_TRACE();

   /*----Initializing common default values----*/
   hm_data.periodic_timer_val = oam_prov_req.hm_config_param.monitoring_interval;
   hm_data.non_responsive_timer_val = oam_prov_req.hm_config_param.nonresponsive_monitoring_interval;
   hm_data.enb_recieved_resp_bitmask = OAM_ZERO;
/*SPR 21369 Start*/
   oam_memset( hm_data.oam_l2_health_ping_resp,OAM_ZERO,OAM_MAX_NUM_L2_INSTANCE*sizeof(oam_l2_health_ping_resp_t));
   hm_data.l3_health_mon_flag=OAM_FALSE;
/*SPR 21369 End*/
   hm_data.retry_count = oam_prov_req.hm_config_param.max_retries;

   OAM_FUN_EXIT_TRACE();
   return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME  : oam_start_hm
*   DESCRIPTION    : This function starts health monitoring 
*                    and sends the request to respective modules
*   Inputs         : NONE
*   Outputs        : NONE
 *   RETURNS        : NONE 
 ******************************************************************************/
void
oam_start_hm(
/* + 17439 */
void
)
/* - 17439 */
{
    OAM_FUN_ENTRY_TRACE();
    /* SPR 22055 FIX START */
    if ((((hm_data.non_responsive_timer_val) * (hm_data.retry_count)) 
            <  hm_data.periodic_timer_val)&& (OAM_ZERO != hm_data.periodic_timer_val))
    /* SPR 22055 FIX END  */    
    {    
        /*---Starting timer---*/
        /*SPR 21369 Start*/
        if(OAM_FALSE==hm_data.l3_health_mon_flag)
        {    
            hm_data.hm_periodic_timer = oam_start_new_timer(OAM_SUB_MODULE_HM,
                    OAM_MODULE_ID,
                    OAM_HM_TIMER_TYPE,
                    hm_data.periodic_timer_val,
                    "HM PERIODIC TIMER", OAM_ONE, (oam_bool_et)OAM_ONE); 

            oam_send_check_health_req_to_all();
            hm_data.l3_health_mon_flag=OAM_TRUE;
        /*SPR 21369 End*/
        }
    }
    /* SPR 22055 FIX START */
    else if (OAM_ZERO == hm_data.periodic_timer_val)
    {
        OAM_LOG(OAM, OAM_INFO, "Health Monitoring is Disabled ");
    }
    /* SPR 22055 FIX END  */
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Health Monitoring not started as value of product" 
                "(nonresponsive_monitoring_interval[%u] * max_retries[%u]) should be "
                "less than monitoring_interval[%u]",
                hm_data.non_responsive_timer_val,
                hm_data.retry_count,
                hm_data.periodic_timer_val);    

    }
    /* SPR 22055 FIX START */
    OAM_FUN_EXIT_TRACE();
    /* SPR 22055 FIX END   */
}    


/******************************************************************************
*   FUNCTION NAME  : oam_hm_timer_expiry
*   DESCRIPTION    : This function will process the periodic timer expiration.
*   Inputs         : p_timer_data: Message Buffer from TIMER
*   Outputs        : NONE
*   RETURNS        : OAM_SUCCESS/OAM_FAILURE 
******************************************************************************/
oam_return_et
    /*SPR 17777 fix*/
oam_hm_timer_expiry(void)
{
    OAM_FUN_ENTRY_TRACE();
/*SPR 21369 Start*/
    if (OAM_ALL_RESP_RECVD == hm_data.enb_recieved_resp_bitmask)
    {
        OAM_LOG(OAM, OAM_DETAILEDALL,"AFTER TIMER EXPIRY ");
        oam_memset(hm_data.oam_l2_health_ping_resp,OAM_ZERO,OAM_MAX_NUM_L2_INSTANCE*sizeof(oam_l2_health_ping_resp_t));
        oam_send_check_health_req_to_all();
        hm_data.enb_recieved_resp_bitmask = OAM_ZERO;
/*SPR 21369 End*/
        hm_data.retry_count = oam_prov_req.hm_config_param.max_retries;
        retry_counter = OAM_ZERO;
    }
   else
   {
       /* OAM BCOM Code Comment Changes Start */
       if (OAM_FAILURE == oam_hm_call_system_state_change())
       {
           OAM_LOG (OAM, OAM_ERROR,
                   "Call to oam_hm_call_system_state_change() failed !!");
       }
       /* OAM BCOM Code Comment Changes End */ 

       /*SPR 20421 FIX START*/
       shutdown_flag = OAM_TRUE;
       /*SPR 20421 FIX END*/
       oam_handle_internal_shutdown();
       /* SPR 21889 +- */
   }
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME  : oam_hm_non_responsive_timer_expiry
*   DESCRIPTION    : This function will process the non responsive 
*                    timer expiration.
*   Inputs         : p_timer_data: Message Buffer from TIMER
*   Outputs        : NONE
*   RETURNS        : OAM_SUCCESS/OAM_FAILURE 
******************************************************************************/
oam_return_et
    /*SPR 17777 fix*/
oam_hm_non_responsive_timer_expiry(void)
{
    OAM_FUN_ENTRY_TRACE();
    retry_counter++;
    /*SPR 21369 Start*/
    rrm_oam_cell_delete_req_t  rrm_oam_cell_delete_req;          
    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
    oam_return_et  ret_val = OAM_SUCCESS;
    oam_bool_et l2_resp_recvd =OAM_TRUE;
    UInt8 l2_instance=OAM_ZERO;
    /*SPR 21478 Start*/
    oam_cell_status_t *p_cell_status = OAM_NULL;
    /*SPR 21478 End*/

    if((OAM_ALL_RESP_RECVD == hm_data.enb_recieved_resp_bitmask)&&(hm_data.retry_count != OAM_ZERO))

    {
        for(l2_instance =OAM_ZERO;(l2_instance < OAM_MAX_NUM_L2_INSTANCE) ;l2_instance++)                
        {     
            if(hm_data.oam_l2_health_ping_resp[l2_instance].l2_send_flag!=OAM_ZERO)
            {
                if(OAM_ALL_RESP_RECVD_FOR_L2 != hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask)
                {
                    OAM_LOG(OAM, OAM_ERROR,"Health Ping response not recieved for l2 Instance %d!!!!",l2_instance);
                    oam_send_hm_req_to_layers();
                    hm_data.hm_non_responsive_timer = oam_start_new_timer(OAM_SUB_MODULE_HM,
                            OAM_MODULE_ID,
                            OAM_HM_NON_RESPONSIVE_TIMER_TYPE,
                            hm_data.non_responsive_timer_val,
                            "HM NON RESPONSIVE TIMER", OAM_ZERO, (oam_bool_et)OAM_ZERO);
                    l2_resp_recvd=OAM_FALSE;
                }
                else
                {
                    OAM_LOG(OAM, OAM_DETAILED,"Health Ping response  recieved for l2 Instance %d bitmask %d!",
                            l2_instance,hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask);
                }    
            }
        }/*SPR 21369 End*/
        if(l2_resp_recvd)
        {
            hm_data.retry_count = oam_prov_req.hm_config_param.max_retries;
            retry_counter = OAM_ZERO;
            return OAM_SUCCESS;
        }
    }
    else if(hm_data.retry_count != OAM_ZERO)
    {
        oam_send_hm_req_to_layers();
        hm_data.hm_non_responsive_timer = oam_start_new_timer(OAM_SUB_MODULE_HM,
                OAM_MODULE_ID,
                OAM_HM_NON_RESPONSIVE_TIMER_TYPE,
                hm_data.non_responsive_timer_val,
                "HM NON RESPONSIVE TIMER", OAM_ZERO, (oam_bool_et)OAM_ZERO);
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,"Health Monitoring :Max Retry reached!!!!");
        /*SPR 21369 Start*/
        /* Code Removed */
        /* SPR 15366 END*/
        if(OAM_ALL_RESP_RECVD != hm_data.enb_recieved_resp_bitmask)
        {   
            oam_handle_internal_shutdown();
            return OAM_SUCCESS;
        }
        else
        {
            for(l2_instance =OAM_ZERO;(l2_instance < OAM_MAX_NUM_L2_INSTANCE);l2_instance++)
            {
                if(hm_data.oam_l2_health_ping_resp[l2_instance].l2_send_flag!=OAM_ZERO)
                {      
                    if(OAM_ALL_RESP_RECVD_FOR_L2 != hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask)
                    {
                        UInt8 cell_indx ; 
                        for(cell_indx=OAM_ZERO;(cell_indx< L2_INSTANCE_INFO(l2_instance).num_cells)&& (L2_INSTANCE_CELL_INFO(l2_instance,cell_indx ).cell_status);cell_indx++)
                        {    
                            tcb_transaction_id=oam_get_new_trans_id();
                            cell_config_idx = oam_get_rrm_cell_config_idx(L2_INSTANCE_CELL_INFO(l2_instance,cell_indx).cell_idx);
                            if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
                                OAM_LOG(OAM, OAM_ERROR, "Invalid cell context[cid=%d]",
                                        L2_INSTANCE_CELL_INFO(l2_instance, cell_indx).cell_idx);
                                OAM_FUN_EXIT_TRACE();
                                continue;
                            }
                            oam_memcpy(&(rrm_oam_cell_delete_req.global_cell_id),&OWN_CELL_CGI(cell_config_idx),
                                    sizeof(rrm_oam_eutran_global_cell_id_t));
                            rrm_oam_cell_delete_req.bitmask=RRM_OAM_INSTANCE_RESET_FLAG_PRESENT;
                            ret_val = oam_rrm_send_rrm_oam_cell_delete_req(
                                    &rrm_oam_cell_delete_req,
                                    OAM_MODULE_ID,
                                    RRM_MODULE_ID,
                                    tcb_transaction_id,
                                    L2_INSTANCE_CELL_INFO(l2_instance, cell_indx).cell_idx);
                            if (OAM_FAILURE == ret_val) {
                                OAM_LOG(OAM, OAM_ERROR,
                                        "Error sending RRM_OAM_CELL_DELETE_REQ to RRM");
                            }
                            else {
                                /*SPR 21478 Start*/
                                L2_INSTANCE_CELL_INFO(l2_instance,cell_config_idx ).cell_status=OAM_WF_INIT_RESP;
                                num_mac_init_cell[l2_instance]--;
                                OAM_LOG(OAM, OAM_INFO, "Sent RRM_OAM_CELL_DELETE_REQ to RRM for Instance = %d , cell index =%d",l2_instance,cell_config_idx);
                                p_cell_status = oam_get_cell_cntxt_status(cell_config_idx);
                                /* CID Fix 115501 +*/
                                if(!p_cell_status) {
                                    OAM_LOG(OAM, OAM_ERROR,
                                            "No cell found for cell context = %d",cell_config_idx);
                                    break;
                                }
                                /* CID Fix 115501 -*/
                                p_cell_status->cell_state |= OAM_CELL_DELETE; 
                                /*SPR 21478 End*/
                            }
                        }
                        if (OAM_SUCCESS == ret_val) 
                        {
                            /* SPR 21815 Start */
                            Char8 hm_alarm_text[OAM_HM_ALARM_TEXT_SIZE] = {OAM_ZERO};
                            oam_memset(hm_alarm_text, OAM_ZERO, OAM_HM_ALARM_TEXT_SIZE);	
                            oam_snprintf(hm_alarm_text, OAM_HM_ALARM_TEXT_SIZE, "Layer2Instance :%d Not Responding Starting Cleanup for it.", l2_instance);
                            OAM_ALARM(HM_RESPONSE_NOT_RECVD_ALARM_ID,
                                    OAM_ZERO,NOTIFY_NEW_ALARM,
                                    PROCESSING_ERROR_ALARM,
                                    SOFTWARE_ERROR,
                                    EXPEDITED_ALARM,
                                    hm_alarm_text, OAM_NULL,
                                    L2_INSTANCE_CELL_INFO(l2_instance, cell_indx).cell_idx);
                            /* SPR 21815 End */
                            oam_memset(&l2_comm_info[l2_instance].mac_comm_info,OAM_ZERO,sizeof(rrm_oam_l2_comm_info_t));
                            oam_memset(&l2_comm_info[l2_instance].rlc_comm_info,OAM_ZERO,sizeof(rrm_oam_l2_comm_info_t));
                            oam_memset(&l2_comm_info[l2_instance].cran_pdcp_comm_info,OAM_ZERO,sizeof(rrm_oam_pdcp_comm_info_t));			
                            oam_memset(&l2_comm_info[l2_instance].pdcp_comm_info,OAM_ZERO,sizeof(rrm_oam_l2_comm_info_t));
                            oam_memset(&l2_comm_info[l2_instance].phy_comm_info,OAM_ZERO,sizeof(rrm_oam_l2_comm_info_t));
                            oam_memset(&hm_data.oam_l2_health_ping_resp[l2_instance],OAM_ZERO,sizeof(oam_l2_health_ping_resp_t));
                            l2_comm_info[l2_instance].init_flag=OAM_ZERO;
                        }
                    }
                    /* SPR 21889 Fix Start */ 
                    oam_trigger_agent_fsm(OAM_NULL, OAM_AGENT_EVENT_RESTART, l2_instance);
                    /* SPR 21889 Fix End */ 
                    if (OAM_ZERO < oam_prov_req.l2_setup_info.num_instances)
                    {
                        oam_prov_req.l2_setup_info.num_instances--;
                    }
                }
            }    
        }    
    }
    if(hm_data.retry_count>OAM_ZERO)
    {    
        hm_data.retry_count--;
    }
    /*SPR 21369 End*/
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
/******************************************************************************
 *  FUNCTION NAME: oam_hm_call_system_state_change
 *  DESCRIPTION:   This function will handles shutdown of oam 
 *  Inputs:        NONE
 *  Outputs:       NONE
 *  RETURNS:       OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et oam_hm_call_system_state_change(
   void
   )
{
    UInt16 trans_id = OAM_ZERO; 
    Char8 hm_alarm_text[OAM_HM_ALARM_TEXT_SIZE] = {OAM_ZERO};
    UInt16 rem_size = OAM_HM_ALARM_TEXT_SIZE;
    UInt16 alarm_text_flag = OAM_FALSE;
    UInt16 hm_resp_count = OAM_ZERO;
    /* Spr 12880 Fix Start */
    UInt8 CellId = OAM_INVALID_CELL_ID;  
    /* Spr 12880 Fix End */
    /* Coverity 61348 */
    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;

    cell_config_idx = oam_get_rrm_cell_config_idx(
            oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_ERROR, "Invalid cell context[cid=%d]",
        oam_prov_req.cell_cntxt.curr_cell_cid);
        return OAM_FAILURE;
    }


    oam_memset(hm_alarm_text, OAM_ZERO, OAM_HM_ALARM_TEXT_SIZE);	
    oam_strncpy(hm_alarm_text, "eNodeB protocol layers : ",
            OAM_HM_ALARM_TEXT_SIZE);
    rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);

   /* Set operational state as disable and inform management interface */   
   oam_prov_req.system_status.op_state = ENODEB_OP_STATE_DISABLED;
   /* Lock the admin state internally for now */
   oam_prov_req.system_status.admin_state = ENODEB_ADMIN_STATE_LOCKED;
   oam_prov_req.system_status.rftx_status = OAM_FALSE;

    OAM_LOG(OAM, OAM_INFO, "OP_State Changed to Disabled and "
            "Admin State Changed to Locked");

    if(OAM_TR069_MODE_ON == oam_prov_req.oam_tr069_req.tr069_mode) {
        oam_inform_value_change_event(OAM_NULL,
                                      OAM_MODULE_ID,
                                      TR069_MODULE_ID);
    }
/* BUG 1080 FIX START */
   if(OAM_FAILURE == oam_admin_state_changed(OAM_MODULE_ID,trans_id))
   {
     OAM_LOG(OAM, OAM_WARNING, "Failure occured during admin state changing");
   }
/* BUG 1080 FIX END */

   /* SPR 12421 CODE REMOVED */
   /*SPR 21369 Start*/
   /*Coverity fix start : 62079 */
   if(!(hm_data.enb_recieved_resp_bitmask & OAM_RRM_RESP_RECVD_MASK ))
   {
       oam_strncat(hm_alarm_text, " RRM", rem_size);
       rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       alarm_text_flag = OAM_TRUE;
       hm_resp_count++;
   }	

#ifdef OAM_SON_ENABLE
   if(!(hm_data.enb_recieved_resp_bitmask & OAM_SON_RESP_RECVD_MASK ))
   {
       if(alarm_text_flag)
       {
           oam_strncat(hm_alarm_text, ",", rem_size);
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       }
       oam_strncat(hm_alarm_text, " SON", rem_size);	
       rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       alarm_text_flag = OAM_TRUE;
       hm_resp_count++;
   }	
#endif

   if(!(hm_data.enb_recieved_resp_bitmask & OAM_RRC_RESP_RECVD_MASK ))
   {
       if(alarm_text_flag)
       {
           oam_strncat(hm_alarm_text, ",", rem_size);
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       }
       oam_strncat(hm_alarm_text, " RRC", rem_size);	
       rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       alarm_text_flag = OAM_TRUE;
       hm_resp_count++;
   }	

   if(!(hm_data.enb_recieved_resp_bitmask & OAM_X2AP_RESP_RECVD_MASK ))
   {
       if(alarm_text_flag)
       {
           oam_strncat(hm_alarm_text, ",", rem_size);
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       }
       oam_strncat(hm_alarm_text, " X2AP", rem_size);	
       rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       alarm_text_flag = OAM_TRUE;
       hm_resp_count++;
   }	

   if(!(hm_data.enb_recieved_resp_bitmask & OAM_S1AP_RESP_RECVD_MASK))
   {
       if(alarm_text_flag)
       {
           oam_strncat(hm_alarm_text, ",", rem_size);
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       }
       oam_strncat(hm_alarm_text, " S1AP", rem_size);	
       rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       alarm_text_flag = OAM_TRUE;
       hm_resp_count++;
   }	
   if(!(hm_data.enb_recieved_resp_bitmask & OAM_EGTPU_RESP_RECVD_MASK))
   {
       if(alarm_text_flag)
       {
           oam_strncat(hm_alarm_text, ",", rem_size);
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       }
       oam_strncat(hm_alarm_text, " EGTPU", rem_size);	
       rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
       hm_resp_count++;
   }
   UInt8 l2_instance;
   for(l2_instance =OAM_ZERO;(l2_instance < OAM_MAX_NUM_L2_INSTANCE) &&
           (hm_data.oam_l2_health_ping_resp[l2_instance].l2_send_flag!=OAM_ZERO);l2_instance++)
   {
       if(!(hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask & OAM_PDCP_RESP_RECVD_MASK))
       {
           if(alarm_text_flag)
           {
               oam_strncat(hm_alarm_text, ",", rem_size);
               rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
           }
           oam_strncat(hm_alarm_text, " PDCP ", rem_size);	
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
           alarm_text_flag = OAM_TRUE;
           hm_resp_count++;
       }	

       if(!(hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask & OAM_MAC_RESP_RECVD_MASK))
       {
           if(alarm_text_flag)
           {
               oam_strncat(hm_alarm_text, ",", rem_size);
               rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
           }
           oam_strncat(hm_alarm_text, " MAC", rem_size);	
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
           alarm_text_flag = OAM_TRUE;
           hm_resp_count++;
       }	

       if(!(hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask & OAM_RLC_RESP_RECVD_MASK))
       {
           if(alarm_text_flag)
           {
               oam_strncat(hm_alarm_text, ",", rem_size);
               rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
           }
           oam_strncat(hm_alarm_text, " RLC", rem_size);	
           rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
           alarm_text_flag = OAM_TRUE;
           hm_resp_count++;
       }	

   } 	
   /*SPR 21369 End*/
   /*Coverity fix end : 62079 */
   oam_strncat(hm_alarm_text, " Not Responding ", rem_size);	
   /*coverity 81822 fix start*/
   //rem_size = OAM_HM_ALARM_TEXT_SIZE - oam_strlen(hm_alarm_text);
   /*coverity 81822 fix end*/
   OAM_ALARM(HM_RESPONSE_NOT_RECVD_ALARM_ID,
           OAM_ZERO,NOTIFY_NEW_ALARM,
           PROCESSING_ERROR_ALARM,
           SOFTWARE_ERROR,
           EXPEDITED_ALARM,
           /* Spr 12880 Fix Start */
           hm_alarm_text, OAM_NULL,CellId);
           /* Spr 12880 Fix End */

   OAM_FUN_EXIT_TRACE ();
   return OAM_SUCCESS;
}
/******************************************************************************
 *  FUNCTION NAME: oam_send_check_health_req_to_rrm
 *  DESCRIPTION:   This function will send check health request to layers
 *  Inputs:        NONE
 *  Outputs:       NONE
 *  RETURNS:       OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_send_check_health_req_to_rrm(
/* + 17439 */
void
)
/* - 17439 */
{
    rrm_oam_chk_health_req_t  rrm_oam_chk_health_req;           /* Pointer to API specific information. */
    UInt16                 transaction_id;  /* Interface transaction identifier */
    transaction_id = oam_get_new_trans_id();

    rrm_oam_chk_health_req.bitmask = OAM_ZERO;
    OAM_FUN_ENTRY_TRACE ();

    oam_rrm_send_rrm_oam_chk_health_req(&rrm_oam_chk_health_req,
                                      OAM_MODULE_ID,
                                      RRM_MODULE_ID,
                                      transaction_id,
                                      oam_prov_req.cell_cntxt.curr_cell_cid);

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}


#ifdef OAM_SON_ENABLE
/******************************************************************************
 *  FUNCTION NAME: oam_send_check_health_req_to_son 
 *  DESCRIPTION:   This function will send check health request to layers
 *  Inputs:        NONE
 *  Outputs:       NONE
 *  RETURNS:       OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_send_check_health_req_to_son(
/* + 17439 */
void
)
/* - 17439 */
{
    OAM_FUN_ENTRY_TRACE ();
    son_oam_chk_health_req_t oam_son_chk_health_req;
    UInt16                 transaction_id;  /* Interface transaction identifier */
    transaction_id = oam_get_new_trans_id();

    oam_son_chk_health_req.bitmask = OAM_ZERO;  

    oam_son_send_son_oam_chk_health_req(&oam_son_chk_health_req,
                                      OAM_MODULE_ID,
                                      SON_MODULE_ID,
                                      transaction_id,
                                      oam_prov_req.cell_cntxt.curr_cell_cid);    
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
#endif

/******************************************************************************
 *  FUNCTION NAME: oam_hm_recieved_resp_bitmask_update
 *  DESCRIPTION:   This function will update bitmask corresponding to hm recieved 
 *                 response bitmask
 *  Inputs:        NONE
 *  Outputs:       NONE
 *  RETURNS:       OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_hm_recieved_resp_bitmask_update
(
    /*SPR 21369 Start*/
   void *p_api_buf
    /*SPR 21369 End*/
)
{
    OAM_FUN_ENTRY_TRACE ();
    /*SPR 21369 Start*/
    UInt16         api_id   = OAM_ZERO;
    api_id   = get_word_from_header((UInt8*)(p_api_buf) + OAM_EXT_API_ID_FROM_HEADER);
    /* SPR 21683 Start */

    UInt8 l2_instance =OAM_ZERO;
	
	oam_return_et  ret_val = OAM_SUCCESS;
    UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));

	
    ret_val=find_l2_instance_id_from_cell_index(cell_index, &l2_instance);
    if(OAM_FAILURE == ret_val)
    {
	    OAM_LOG(OAM, OAM_WARNING,"instance id not exist for received cell_idex: [%d]", cell_index);
    }
    /* SPR 21683 End*/
    if(api_id == RRM_OAM_CHK_HEALTH_RESP)
    {
        hm_data.enb_recieved_resp_bitmask |= OAM_RRM_RESP_RECVD_MASK;  
        OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from RRM [api_id]= %d",api_id);
    }
    else if(api_id == RRC_OAM_PROC_SUPP_RESP)
    {    
        hm_data.enb_recieved_resp_bitmask |= OAM_RRC_RESP_RECVD_MASK; 
        OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from RRC [api_id]= %d",api_id);
    }    
    else if(api_id == S1AP_OAM_PROC_SUPP_RESP)
    {
       hm_data.enb_recieved_resp_bitmask |= OAM_S1AP_RESP_RECVD_MASK;  
       OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from S1AP [api_id]= %d",api_id);
    }   
    else if(api_id == X2AP_OAM_PROC_SUPP_RESP)
    {    
       hm_data.enb_recieved_resp_bitmask  |= OAM_X2AP_RESP_RECVD_MASK;  
       OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from X2AP [api_id]= %d",api_id);
    }  
#ifdef OAM_SON_ENABLE
    else if(api_id == SON_OAM_CHK_HEALTH_RESP)
    {
        hm_data.enb_recieved_resp_bitmask |= OAM_SON_RESP_RECVD_MASK;  
        OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from SON [api_id]= %d",api_id);
    }
#endif
    else if(api_id ==  PR_GTPU_PROC_SUP_RESP)
    {
        hm_data.enb_recieved_resp_bitmask |= OAM_EGTPU_RESP_RECVD_MASK;  
        OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from EGTPU [api_id]= %d",api_id);
    }
    if(OAM_ONE==hm_data.oam_l2_health_ping_resp[l2_instance].l2_send_flag)
    {
        if(api_id == MAC_PROC_SUP_RESP)
        {    
            hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask |= OAM_MAC_RESP_RECVD_MASK; 
            OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from MAC for l2_instance [%d] [api_id]= %d",l2_instance,api_id); 
        }   
        else if(api_id == RLC_PROC_SUP_RESP)
        {    
            hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask |= OAM_RLC_RESP_RECVD_MASK;  
            OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from RLC for l2_instance [%d] [api_id]= %d",l2_instance,api_id);
        }    
        else if(api_id == PDCP_PROC_SUP_RESP)
        {    
            hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask |= OAM_PDCP_RESP_RECVD_MASK;  
            OAM_LOG(OAM,OAM_INFO, "health monitoring recieved response from PDCP for l2_instance[%d] [api_id]= %d",l2_instance,api_id);
        }
    }
    OAM_FUN_EXIT_TRACE();
    /*SPR 21369 End*/
}

/******************************************************************************
 *  FUNCTION NAME: oam_send_hm_req_to_layers 
 *  DESCRIPTION:   function sends check health request tom l2 
 *  Inputs:        NONE 
 *  Outputs:       NONE
 *  RETURNS:       NONE
 *******************************************************************************/
void
oam_send_hm_req_to_layers(
/* + 17439 */
void
)
/* - 17439 */
{
    oam_error_code_et p_error_code;

    UInt16 transaction_id = OAM_ZERO;
    transaction_id = oam_get_new_trans_id();
    /*SPR 21369 Start*/
    if(!(hm_data.enb_recieved_resp_bitmask & OAM_RRM_RESP_RECVD_MASK ))
    {
        OAM_LOG(OAM,OAM_INFO,"HM Response not received from RRM. Retrying %d time",retry_counter);
        oam_send_check_health_req_to_rrm();
    }
#ifdef OAM_SON_ENABLE
    if(!(hm_data.enb_recieved_resp_bitmask & OAM_SON_RESP_RECVD_MASK))
    {
        OAM_LOG(OAM,OAM_INFO,"HM Response not received from SON. Retrying %d time",retry_counter);
        oam_send_check_health_req_to_son();
    }
#endif

    if(!(hm_data.enb_recieved_resp_bitmask & OAM_RRC_RESP_RECVD_MASK))
    {
        OAM_LOG(OAM,OAM_INFO,"HM Response not received from RRC. Retrying %d time",retry_counter);
        oam_send_check_health_req( 
                RRC_OAM_PROC_SUPP_REQ,
                transaction_id,
                RRC_MODULE_ID,
                &p_error_code
                );
    }
    if(!(hm_data.enb_recieved_resp_bitmask & OAM_X2AP_RESP_RECVD_MASK))
    {
        OAM_LOG(OAM,OAM_INFO,"HM Response not received from X2AP. Retrying %d time",retry_counter);
        oam_send_check_health_req( 
                X2AP_OAM_PROC_SUPP_REQ,
                transaction_id,
                RRC_MODULE_ID,
                &p_error_code
                );
    }
    if(!(hm_data.enb_recieved_resp_bitmask & OAM_S1AP_RESP_RECVD_MASK))
    {
        OAM_LOG(OAM,OAM_INFO,"HM Response not received from S1AP. Retrying %d time",retry_counter);
        oam_send_check_health_req( 
                S1AP_OAM_PROC_SUPP_REQ,
                transaction_id,
                RRC_MODULE_ID,
                &p_error_code
                );
    }
    if(!(hm_data.enb_recieved_resp_bitmask & OAM_EGTPU_RESP_RECVD_MASK))
    {
        OAM_LOG(OAM,OAM_INFO,"HM Response not received from EGTPU. Retrying %d time",retry_counter);
        oam_send_check_health_req_for_l2( 
                PR_GTPU_PROC_SUP_REQ,                               
                transaction_id,
                EGTPU_MODULE_ID,
                &p_error_code,
                OAM_ZERO
                );
    }
    UInt8 l2_instance =OAM_ZERO;
    for(l2_instance=OAM_ZERO;(l2_instance<OAM_MAX_NUM_L2_INSTANCE);l2_instance++)
    {
       if(OAM_ZERO!=hm_data.oam_l2_health_ping_resp[l2_instance].l2_send_flag)
       {    
           if(!(hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask & OAM_PDCP_RESP_RECVD_MASK))
           {
               OAM_LOG(OAM,OAM_INFO,"HM Response not received from PDCP. for l2_instance %d Retrying %d time",l2_instance,retry_counter);
               oam_send_check_health_req_for_l2( 
                       PDCP_PROC_SUP_REQ,
                       transaction_id,
                       PDCP_MODULE_ID,
                       &p_error_code,
                       l2_instance
                       );
           }
           if(!(hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask & OAM_MAC_RESP_RECVD_MASK))
           {
               OAM_LOG(OAM,OAM_INFO,"HM Response not received from MAC. for l2_instcane %d Retrying %d time",l2_instance,retry_counter);
               oam_send_check_health_req_for_l2( 
                       MAC_PROC_SUP_REQ,
                       transaction_id,
                       MAC_MODULE_ID,
                       &p_error_code,
                       l2_instance
                       );
           }
           if(!(hm_data.oam_l2_health_ping_resp[l2_instance].l2_response_bitmask & OAM_RLC_RESP_RECVD_MASK))
           {
               OAM_LOG(OAM,OAM_INFO,"HM Response not received from RLC. for l2_instance %d  Retrying %d time",l2_instance,retry_counter);
               oam_send_check_health_req_for_l2( 
                       RLC_PROC_SUP_REQ,
                       transaction_id,
                       RLC_MODULE_ID,
                       &p_error_code,
                       l2_instance
                       );
           }   
       }
    }
    /*SPR 21369 End*/
}
/******************************************************************************
 *  FUNCTION NAME: oam_send_check_health_req 
 *  DESCRIPTION:   function sends check health request tom l2 and l3
 *  Inputs:        api_id : api id which have to be sent 
 *                 des_module : detination module to which message have to be sent
 *                 p_error_code : pointer to error code 
 *  Outputs:       NONE
 *  RETURNS:       OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_send_check_health_req
(
    UInt16 api_id,
    UInt16 transaction_id,
    oam_module_id_t des_module,
    oam_error_code_et *p_error_code
)
{
    oam_length_t msg_length     = OAM_ZERO;
    oam_length_t msg_api_length = OAM_ZERO;
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_check_hlth_msg = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
    {
           OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                                                                  *p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_check_hlth_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_check_hlth_msg, OAM_VERSION_ID, OAM_MODULE_ID,
            des_module, api_id, msg_api_length);


    /* Fill interface header */
    p_check_hlth_msg = p_check_hlth_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_check_hlth_msg,
                                      transaction_id,
                                      OAM_MODULE_ID,
                                      des_module,
                                      api_id,
                                      msg_length,
                                      oam_prov_req.cell_cntxt.curr_cell_cid);

    /* Send message */
    if(OAM_FAILURE == oam_send_message(p_msg, des_module,p_error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending message [des_module id = %d] with error code %d",
                                                                       des_module,*p_error_code);
        /*+-coverity 86977*/
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
/*SPR 21369 Start*/
/******************************************************************************
 *  FUNCTION NAME: oam_send_check_health_req_for_l2 
 *  DESCRIPTION:   function sends check health request to l2 
 *  Inputs:        api_id : api id which have to be sent 
 *                 des_module : detination module to which message have to be sent
 *                 p_error_code : pointer to error code 
 *                 l2_instance :l2 instance for sending HM
 *  Outputs:       NONE
 *  RETURNS:       OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_send_check_health_req_for_l2
(
    UInt16 api_id,
    UInt16 transaction_id,
    oam_module_id_t des_module,
    oam_error_code_et *p_error_code,
    UInt8 l2_instance
)
{
    oam_length_t msg_length     = OAM_ZERO;
    oam_length_t msg_api_length = OAM_ZERO;
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_check_hlth_msg = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
    {
           OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                                                                  *p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_check_hlth_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_check_hlth_msg, OAM_VERSION_ID, OAM_MODULE_ID,
            des_module, api_id, msg_api_length);


    /* Fill interface header */
    p_check_hlth_msg = p_check_hlth_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_check_hlth_msg,
                                      transaction_id,
                                      OAM_MODULE_ID,
                                      des_module,
                                      api_id,
                                      msg_length,
                                      L2_INSTANCE_CELL_INFO(l2_instance,OAM_ZERO).cell_idx);

    /* Send message */
    if(OAM_FAILURE == oam_send_message(p_msg, des_module,p_error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending message [des_module id = %d] with error code %d",
                                                                       des_module,*p_error_code);
        /*+-coverity 86977*/
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}


/******************************************************************************
 *  FUNCTION NAME: oam_send_check_health_req_to_all 
 *  DESCRIPTION:   function sends check health request to all process
 *  Inputs:        NONE
 *  Outputs:       NONE
 *  RETURNS:       NONE
 *******************************************************************************/
void
oam_send_check_health_req_to_all(
/* + 17439 */
void
)
/* - 17439 */
{
    oam_error_code_et p_error_code = NO_ERROR;
    UInt16 transaction_id = OAM_ZERO;
    transaction_id = oam_get_new_trans_id();

    OAM_FUN_ENTRY_TRACE();

        oam_send_check_health_req_to_rrm();
#ifdef OAM_SON_ENABLE
        oam_send_check_health_req_to_son();
#endif
        oam_send_check_health_req(
                RRC_OAM_PROC_SUPP_REQ,
                transaction_id,
                RRC_MODULE_ID,
                &p_error_code
                ); 
        oam_send_check_health_req(
                X2AP_OAM_PROC_SUPP_REQ,
                transaction_id,
                RRC_MODULE_ID,
                &p_error_code
                );

        oam_send_check_health_req( 
                S1AP_OAM_PROC_SUPP_REQ,
                transaction_id,
                RRC_MODULE_ID,
                &p_error_code
                );
        /*SPR 21369 Start*/
        oam_send_check_health_req_for_l2( 
                PR_GTPU_PROC_SUP_REQ,                               
                transaction_id,
                EGTPU_MODULE_ID,
                &p_error_code,
                OAM_ZERO
                );
    UInt8 l2_instance =OAM_ZERO;
    for(l2_instance=OAM_ZERO;l2_instance<OAM_MAX_NUM_L2_INSTANCE;l2_instance++)
    {
        if(PDCP_OAM_INIT_CNF & l2_comm_info[l2_instance].init_flag)
        {    

            oam_send_check_health_req_for_l2( 
                    PDCP_PROC_SUP_REQ,
                    transaction_id,
                    PDCP_MODULE_ID,
                    &p_error_code,
                    l2_instance
                    );
            oam_send_check_health_req_for_l2( 
                    MAC_PROC_SUP_REQ,
                    transaction_id,
                    MAC_MODULE_ID,
                    &p_error_code,
                    l2_instance
                    );
            oam_send_check_health_req_for_l2( 
                    RLC_PROC_SUP_REQ,
                    transaction_id,
                    RLC_MODULE_ID,
                    &p_error_code ,
                    l2_instance
                    );
               hm_data.oam_l2_health_ping_resp[l2_instance].l2_send_flag=OAM_ONE; 
        }
    }
    /*SPR 21369 End*/
      
    hm_data.hm_non_responsive_timer = oam_start_new_timer(OAM_SUB_MODULE_HM,
                                OAM_MODULE_ID,
                                OAM_HM_NON_RESPONSIVE_TIMER_TYPE,
                                hm_data.non_responsive_timer_val,
                                "HM NON RESPONSIVE TIMER", OAM_ZERO, (oam_bool_et)OAM_ZERO);


    OAM_FUN_EXIT_TRACE();
}



