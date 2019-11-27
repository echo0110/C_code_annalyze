/*****************************************************************************
 * File Name   : oam_fault_mgmt_fsm.c
 *
 * Description : This file contains OAM Fault Management FSM Handler and
 *               its associated routines.
 * Revision History:
 * Date         Author     Reference             Comments
 * -----        ------     ---------             --------
 * March,2012   Vishwa     OAM Design Document   Initial Version
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

/****************************************************************************
 *   Includes
 ***************************************************************************/

//#include<stdio.h>
#include <oam_types.h>
#include <oam_proto.h>
#include<oam_fault_mgmt.h>
#include<oam_config_mgmt.h>
#include<oam_sim_utils.h>

/****************************************************************************
 *  * Global Variables 
 *   ****************************************************************************/
extern oam_prov_req_t oam_prov_req;

/*Strings for various states*/
SInt8 * OAM_FM_STATE[]=
{
    (SInt8 *)"OAM_FM_STATE_UNACK_UNCLEAR",  // The alarm is not acknowledged.
    (SInt8 *)"OAM_FM_STATE_ACK_UNCLEAR",    // The alarm acknowledged but not cleared.
    (SInt8 *)"OAM_FM_STATE_UNACK_CLEAR"    // The alarm not reccieved acknowledged.
};

/*Strings for various events*/
SInt8 * OAM_FM_EVENT[]=
{
    (SInt8 *)"OAM_FM_EVENT_ALARM_ACKNOWLEDGED",  // The alarm is not acknowledged.
    (SInt8 *)"OAM_FM_EVENT_ALARM_CLEARED",       // The alarm has been cleared.
    (SInt8 *)"OAM_FM_EVENT_ALARM_CHANGED",       // The alarm has beeln changed or modified.
    (SInt8 *)"OAM_FM_EVENT_NEW_ALARM_RAISED"     // Notification of a new alarm that is raised.
};


/************************************************************************************
Functions of FSM table 
*************************************************************************************/

 /****************************************************************************
  * Function Name  : oam_fm_state_1_alarm_ack
  * Inputs         : p_alarm_info :Pointer to input api buffer
  *                  p_fm_cntxt :Pointer to FM FSM context
  *                  curr_alarm_idx :Current alarm index                      
  *                  p_err :Pointer to error code
  * Outputs        : NONE
  * Returns        : OAM_SUCCESS/OAM_FAILURE
  * Description    : function to handle state 1 alarm acknowledged.
  *****************************************************************************/
  oam_return_et
  oam_fm_state_1_alarm_ack(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,              /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)               /*Pointer to error code*/
{
    oam_return_et retVal = OAM_SUCCESS;
    OAM_FUN_ENTRY_TRACE();

    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_DETAILEDALL,"unused parameter %p and %p", p_err, p_alarm_info);

    if(curr_alarm_idx >= OAM_ZERO) {
        p_fm_cntxt->current_alarm_list[curr_alarm_idx].alarm_state =
                                                OAM_FM_STATE_ACK_UNCLEAR;
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
 /****************************************************************************
  * Function Name  : oam_fm_state_1_alarm_cleared
  * Inputs         : p_alarm_info :Pointer to input api buffer
  *                  p_fm_cntxt :Pointer to FM FSM context
  *                  curr_alarm_idx :Current alarm index
  *                  p_err :Pointer to error code
  * Outputs        : NONE
  * Returns        : OAM_SUCCESS/OAM_FAILURE
  * Description    : function to handle state 1 alarm cleared.
  *****************************************************************************/
  oam_return_et
  oam_fm_state_1_alarm_cleared(
    oam_alarm_info_t      *p_alarm_info,                                /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                                /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                     /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    oam_handle_cleared_alarm(p_alarm_info);
    
    /* SPR 14630 Fix Start */

    /* Cleared Alarm not raised to HeMS as Ack not received only removes from Current Alarm DB */
    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_INFO,"Cleared Alarm [%d] not raised to HeMS" 
                " as Alarm currunt State is OAM_FM_STATE_UNACK_UNCLEAR, unused parameter %p",
                 p_alarm_info->alarm_id, p_err);
/* SPR 14630 Fix End */

    if(curr_alarm_idx >= OAM_ZERO) {
        p_fm_cntxt->current_alarm_list[curr_alarm_idx].alarm_state=OAM_FM_STATE_UNACK_CLEAR;
        if(p_fm_cntxt->current_alarm_list[curr_alarm_idx].perceived_severity == MAJOR)
        {
            oam_maintain_system_state(OAM_CHANGE_SYSTEM_STATE_FOR_CLEAR_MAJOR_ALARM);
        }
        else if(p_fm_cntxt->current_alarm_list[curr_alarm_idx].perceived_severity == CRITICAL)
        {
            oam_maintain_system_state(OAM_CHANGE_SYSTEM_STATE_FOR_CLEAR_CRITICAL_ALARM);
        }
    }
  
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

  /****************************************************************************
  *Function Name  : oam_fm_state1_alarm_changed
  *Inputs         : p_alarm_info :Pointer to input api buffer
  *                 p_fm_cntxt :Pointer to FM FSM context
  *                 curr_alarm_idx :Current alarm index
  *                 p_err :Pointer to error code
  *Outputs        : NONE
  *Returns        : OAM_SUCCESS/OAM_FAILURE
  *Description    : function to handle state 1 alarm changed.
  *****************************************************************************/
  oam_return_et
  oam_fm_state1_alarm_changed(
    oam_alarm_info_t       *p_alarm_info,                               /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                               /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                    /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    OAM_LOG(OAM,OAM_DETAILEDALL,"unused parameter %p, %d and %p", p_fm_cntxt, curr_alarm_idx, p_err);
    OAM_LOG(OAM, OAM_INFO, "inside oam_fm_state_1_alarm_changed ");
    /* SPR 17777 fix */
    oam_handle_raised_alarm(p_alarm_info);
    oam_fm_populate_n_send_data_to_mangement_interface(p_alarm_info);

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

  /****************************************************************************
  *Function Name  : oam_fm_state_1_alarm_raised
  *Inputs         : p_alarm_info :Pointer to input api buffer
  *                 p_fm_cntxt :Pointer to FM FSM context
  *                 curr_alarm_idx :Current alarm index
  *                 p_err :Pointer to error code
  *Outputs        : NONE
  *Returns        : OAM_SUCCESS/OAM_FAILURE
  *Description    : function to handle state 1 alarm raised.
  *****************************************************************************/
  oam_return_et
  oam_fm_state1_alarm_raised(
    oam_alarm_info_t      *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                             /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                  /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    OAM_LOG(OAM, OAM_INFO, "inside oam_fm_state_1_alarm_raised ");

    oam_handle_raised_alarm(p_alarm_info);
    oam_fm_populate_n_send_data_to_mangement_interface(p_alarm_info);
    /* SPR 14630 Fix Start */
    curr_alarm_idx = oam_fm_fetch_curr_alarm_idx (p_alarm_info->alarm_id,p_alarm_info->CellId);
    if (oam_prov_req.system_status.tr069_configured != OAM_ONE)
    {            
        oam_fm_state_1_alarm_ack(p_alarm_info,p_fm_cntxt,curr_alarm_idx,p_err);
    }
    /* SPR 14630 Fix End */


    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}


/****************************************************************************
  *Function Name  : oam_fm_state_2_alarm_cleared
  *Inputs         : p_alarm_info :Pointer to input api buffer
  *                 p_fm_cntxt :Pointer to FM FSM context
  *                 curr_alarm_idx :Current alarm index
  *                 p_err :Pointer to error code
  *Outputs        : NONE
  *Returns        : OAM_SUCCESS/OAM_FAILURE
  *Description    : function to handle state 2 alarm cleared.
  *******************************************************************************/
  oam_return_et
  oam_fm_state_2_alarm_cleared(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,              /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)               /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    /* sending data to management interface */
    oam_handle_cleared_alarm(p_alarm_info);
    oam_fm_populate_n_send_data_to_mangement_interface(p_alarm_info);

/* SPR 14630 Fix Start */
    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_INFO,"Cleared Alarm [%d] raised to HeMS Successfully, unused parameter %p and %p",
             p_alarm_info->alarm_id, p_err, p_fm_cntxt);
 
/* SPR 14630 Fix End */

    if(curr_alarm_idx >= OAM_ZERO) {
        oam_memset(&oam_prov_req.fm_cntxt.current_alarm_list[curr_alarm_idx],0,
                sizeof(oam_prov_req.fm_cntxt.current_alarm_list[curr_alarm_idx]));
        current_alarm_list_roll_over(curr_alarm_idx);
    }
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;

}


/****************************************************************************
  *Function Name  : oam_fm_state_2_alarm_changed
  *Inputs         : p_alarm_info :Pointer to input api buffer
  *                 p_fm_cntxt :Pointer to FM FSM context
  *                 curr_alarm_idx :Current alarm index
  *                 p_err :Pointer to error code
  *Outputs        : NONE 
  *Returns        : OAM_SUCCESS/OAM_FAILURE
  *Description    : function to handle state 2 alarm changed.
  *******************************************************************************/
  oam_return_et
  oam_fm_state_2_alarm_changed(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                             /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                  /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();

    /* SPR 17777 fix */
    /* sending data to management interface */
    oam_handle_raised_alarm(p_alarm_info);
    oam_fm_populate_n_send_data_to_mangement_interface(p_alarm_info);
    if(curr_alarm_idx >= OAM_ZERO) {
        p_fm_cntxt->current_alarm_list[curr_alarm_idx].alarm_state=OAM_FM_STATE_UNACK_UNCLEAR;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;

}


  /****************************************************************************
  *Function Name  : oam_fm_state_3_alarm_ack
  *Inputs         : p_alarm_info :Pointer to input api buffer
  *                 p_fm_cntxt :Pointer to FM FSM context
  *                 curr_alarm_idx :Current alarm index
  *                 p_err :Pointer to error code
  *Outputs        : NONE
  *Returns        : OAM_SUCCESS/OAM_FAILURE
  *Description    : function to handle state 3 alarm acknowledged.
  *********************************************************************************/
  oam_return_et
  oam_fm_state_3_alarm_ack(
    oam_alarm_info_t      *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                             /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                  /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %p, %p, %d and %p", p_alarm_info, p_fm_cntxt, curr_alarm_idx, p_err);
    OAM_LOG(OAM, OAM_INFO, "oam_fm_state_3_alarm_ack");

    /* SPR 17777 fix */
    /* DO Nothing As Alarm already Cleared from  Current DB */
/* SPR 14630 Fix Start */
/* Code Removed */
/* SPR 14630 Fix End */

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

  /****************************************************************************
  *Function Name  : oam_fm_state_3_alarm_raised
  *Inputs         : p_alarm_info :Pointer to input api buffer
  *                 p_fm_cntxt :Pointer to FM FSM context
  *                 curr_alarm_idx :Current alarm index
  *                 p_err :Pointer to error code
  *Outputs        : NONE
  *Returns        : OAM_SUCCESS/OAM_FAILURE
  *Description    : Function to handle state 3 alarm raised.
  **********************************************************************************/
  oam_return_et
  oam_fm_state_3_alarm_raised(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                             /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                  /*Pointer to error code*/
{
    OAM_FUN_ENTRY_TRACE();
    OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %p", p_err);
    OAM_LOG(OAM, OAM_INFO, "inside oam_fm_state_3_alarm_raised ");
    /* SPR 17777 fix */
    oam_handle_raised_alarm(p_alarm_info); 
    oam_fm_populate_n_send_data_to_mangement_interface(p_alarm_info);

    if(curr_alarm_idx >= OAM_ZERO) {
        p_fm_cntxt->current_alarm_list[curr_alarm_idx].alarm_state=OAM_FM_STATE_UNACK_UNCLEAR;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

  /****************************************************************************
  * Function Name  : oam_fm_ignore_event
  * Inputs         : p_alarm_info :Pointer to input api buffer
  *                  p_fm_cntxt :Pointer to FM FSM context
  *                  curr_alarm_idx :Current alarm index
  *                  p_err :Pointer to error code
  * Outputs        : None
  * Returns        : OAM_SUCCESS/OAM_FAILURE
  * Description    : This function handles all the state and event cases
  *                  where no action is required
  *****************************************************************************/
  oam_return_et
  oam_fm_ignore_event(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                             /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err)                                  /*Pointer to error code*/
{
    oam_return_et  return_value = OAM_FAILURE;

    OAM_FUN_ENTRY_TRACE();

    /*No handling required for this state and event combination*/
    /* coverity 41126 */
    if(curr_alarm_idx >= OAM_ZERO)
    {
    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILED, "Event %d in state %d ignored, unused parameter %p and %p",
            p_fm_cntxt->current_alarm_list[curr_alarm_idx].alarm_state,
            p_fm_cntxt->current_alarm_list[curr_alarm_idx].event_type,
            p_err, p_alarm_info);
    }
    OAM_FUN_EXIT_TRACE();

    return return_value;
}



/****************************************************************************
 * Two dimensional functional pointer for FSM
 *****************************************************************************/

typedef oam_return_et (*oam_fm_func_ptr)(oam_alarm_info_t    *p_alarm_info,
                                            oam_fm_cntxt_t *p_fm_cntxt,
                                            int curr_alarm_idx,
                                            oam_error_code_et *p_err);

/****************************************************************************
* Array Name     : oam_fm_fsm_tbl
* Inputs         : OAM_FM_MAX_STATES : Current OAM FM State
*                  OAM_FM_MAX_EVENTS : Current OAM FM Event
* Outputs        : None
* Returns        : None
* Description    : Definition of two dimensional array of functional pointer
*                  for OAM FM FSM
****************************************************************************/


oam_fm_func_ptr oam_fm_fsm_tbl[OAM_FM_MAX_STATES][OAM_FM_MAX_EVENTS] =
{
    /* OAM_FM_STATE_UNACK_UNCLEAR */         
    {
        oam_fm_state_1_alarm_ack,        /* Alarm gets acknowledged*/ 
        oam_fm_state_1_alarm_cleared,    // Entry added to His List Alarm is deleted from current alarm list SPR 14630 comment edited 
        oam_fm_state1_alarm_changed,
        oam_fm_state1_alarm_raised      //Event New Alarm Raised is ignored in this state
    },

    /* OAM_FM_STATE_ACK_UNCLEAR */             //OAM_FM_EVENT_ALARM_CLEARED
    {
        oam_fm_ignore_event,
        oam_fm_state_2_alarm_cleared,          //Entry added to His List,Mgmt I/f notified of clear event,Alarm is deleted from current alarm list
        oam_fm_state_2_alarm_changed,
        oam_fm_ignore_event
    },

    /* OAM_FM_STATE_UNACK_CLEAR */                  //OAM_FM_EVENT_ALARM _CHANGED
    {
        oam_fm_state_3_alarm_ack,             //Do nothing Spr 14630 comment edited
        oam_fm_ignore_event,
        oam_fm_ignore_event,
        oam_fm_state_3_alarm_raised          //Entry added to His List,Mgmt I/f notified of new alarm raised event,State changed to OAM_FM_STATE_UNACK_UNCLEAR

    }
};



/****************************************************************************
 * Function Name  : oam_fm_fsm_process_event
 * Inputs         : p_alarm_info :Pointer to input api buffer
 *                  p_fm_cntxt :Pointer to FM FSM context
 *                  curr_alarm_idx:Current alarm index
 *                  current_event :Current event
 *                  p_err :Pointer to error code 
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : Entry function for OAM FM FSM
 ****************************************************************************/

oam_return_et
oam_fm_fsm_process_event(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,                             /*Pointer to FM FSM context*/
    /* SPR 17777 fix code removed */
    oam_fm_events_et    current_event,
    oam_error_code_et  *p_err)                                   /*Pointer to error code*/
{
    /* coverity 35632 */
    oam_fm_states_et    current_state=OAM_FM_STATE_UNACK_UNCLEAR;
    oam_return_et return_value = OAM_SUCCESS;
    int  current_alarm_index=OAM_ZERO;

    OAM_FUN_ENTRY_TRACE();

    /*Get the state and event from context struct*/

    /* SPR 14630 Fix Start */
    current_alarm_index=oam_fm_fetch_curr_alarm_idx(p_alarm_info->alarm_id,p_alarm_info->CellId);
    /* SPR 14630 Fix End */
    if((current_alarm_index== -1)&&((p_alarm_info->criticality)==CLEARED))
    {
      
        OAM_LOG(OAM, OAM_DETAILED, "Alarm trying to clear is not raised");
        return OAM_SUCCESS;
    }    
    else if((current_alarm_index== -1)&&((p_alarm_info->criticality)!=CLEARED))
      {
         current_state= OAM_FM_STATE_UNACK_UNCLEAR;
      }
    else
    {
        current_state=p_fm_cntxt->current_alarm_list[current_alarm_index].alarm_state;          
    }

    /*Check range of passed event*/
    if ((current_state < OAM_FM_MAX_STATES)
            && (current_event < OAM_FM_MAX_EVENTS))
    {
        OAM_LOG(OAM, OAM_INFO, "### OAM_FM_FSM: Processing State %s Event %s ###",
                OAM_FM_STATE[current_state], OAM_FM_EVENT[current_event]);
        /*Checking the return value*/
        if(OAM_SUCCESS == oam_fm_fsm_tbl[current_state][current_event](p_alarm_info,
                    p_fm_cntxt,
                    current_alarm_index,
                    p_err))
        {
            return_value = OAM_SUCCESS;
        }
        else
        {
            OAM_LOG(OAM, OAM_INFO, "Not processed Event:%d, State:%d. oam_fm_fsm_table failed",
                    current_event,current_state);
            return_value = OAM_FAILURE;
        }
        /* SPR 14630 Fix Start */
        current_alarm_index=oam_fm_fetch_curr_alarm_idx(p_alarm_info->alarm_id,p_alarm_info->CellId);
        /* SPR 14630 Fix End */
        if((current_alarm_index== -1)&&((p_alarm_info->criticality)==CLEARED))
        {

            OAM_LOG(OAM, OAM_INFO, "Alarm trying to clear is not raised");
        }    
        else if((current_alarm_index== -1)&&((p_alarm_info->criticality)!=CLEARED))
        {
            OAM_LOG(OAM, OAM_INFO, 
                    "### OAM_FM_FSM: State change: %s -> %s Event: %s ###",
                    OAM_FM_STATE[current_state], 
                    OAM_FM_STATE[OAM_FM_STATE_UNACK_UNCLEAR], 
                    OAM_FM_EVENT[current_event]);
        }
        else
        {
            OAM_LOG(OAM, OAM_INFO, 
                    "### OAM_FM_FSM: State change: %s -> %s Event: %s ###",
                    OAM_FM_STATE[current_state], 
                    OAM_FM_STATE[p_fm_cntxt->current_alarm_list[current_alarm_index].alarm_state], 
                    OAM_FM_EVENT[current_event]);
        }
    }
    else
    {
        OAM_LOG(OAM, OAM_INFO, "Unexpected Event:%d, State:%d",current_event,current_state);
        return_value = OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return return_value;
}


/****************************************************************************
 * Function Name  : current_alarm_list_roll_over
 * Inputs         : alarm_idx :Alarm index
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : to roll over current alarm list.
 ****************************************************************************/


oam_return_et
current_alarm_list_roll_over(
    int alarm_idx
    )
{
    int i=alarm_idx;
    OAM_FUN_ENTRY_TRACE();

    if(alarm_idx==oam_prov_req.fm_cntxt.CurrentAlarmNumberOfEntries)
    {
        oam_prov_req.fm_cntxt.Current_List_Index=OAM_ZERO;
        return OAM_SUCCESS;
    }
    for (;i < oam_prov_req.fm_cntxt.Current_List_Index;i++)
    {
        oam_prov_req.fm_cntxt.current_alarm_list[i] = oam_prov_req.fm_cntxt.current_alarm_list[i + 1];
    }
    oam_prov_req.fm_cntxt.Current_List_Index -= 1;

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}


