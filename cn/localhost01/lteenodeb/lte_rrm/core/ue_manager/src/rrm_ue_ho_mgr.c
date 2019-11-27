/* vim: ts=4:et:ai:ci:sw=4
*/
/*! \file rrm_ue_ho_mgr.c
 *  \brief This file contains Handover processing.
 *  copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 ***************************************************************************
*Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*   25 Apr 2014    gur31824      SPR 10729      RRM is not releasing the UE
*                                               when RRC sends
*                                               RRC_RRM_LOCAL_ERROR_IND to
*                                               RRM for UE with single rabs
*   28 May 2014    gur27356     SPR 11385       RRM trigger unexpected HO towards
*                                               UMTS on receiving A3, if PCI not in neighbour list.
*   6  June 2014   gur26515     SPR 10730       Added code changes for SPR 10730
*   19 June 2014   gur29831     SPR 12019       Added code changes for SPR 12019 
*   19 June,2014     gur29831   12019           Added fixes for SPR 12019 
*   19 June,2014     gur27356   12035           Measurement report A4 in cell Block Scenario. 
*   30 June,2014     gur27356   11679           CSFB Target Cell selection to take Registered LAC into account.
*    2 July,2014     gur25767   10498           Inter-freq Daho Support
*   14 July,2014     gur27356   12599           RRM not triggering DAHO HO towards GERAN cell.
*    2 July,2014     gur25767   10498           Inter-freq Daho Support
*   12 Aug,2014     gur21481    12496           RRM not triggering HO towards A5 reported cell 
*                                               once HO cancel received from L3 for A3 HO
*   18Aug,2014       gur27356   12963           After PCI Confusion Reported, HO failure for neighbor 2
*   27Aug,2014       gur29831   13344           DAHO cell is selected for ho 

***************************************************************************/

/*! \Header files included for UEM releated */
#include "rrm_defines.h"
#include "rrm_ue_context.h"
#include "rrm_utils.h"
#include "rrm_types.h"
#include "rrm_cell_context.h"
#include "rrc_rrm_intf.h"
#include "rrm_rrc_ue_intf.h"
#include "rrm_cell_mgr.h"
#include "rrm_ue_mgr.h"
#include "rrm_ue_ho_utils.h"
#include "rrm_ue_fsm.h"
/* warnings removal: header file 'rrm_mif_fsm.h' contains prototype
   declaration of 'check_block_cell_is_ongoing()' */
#include "rrm_mif_fsm.h"

/* warnings removal: header file 'rrm_mif_db.h' contains prototype
   declaration of 'compare_global_cell_id()' */
#include "rrm_mif_db.h"
#include "s1ap_rrm_composer.h"
#include "rrm_events.h"

/* SPR 20652 Fix Start */
/* Code Removed */
/* SPR 20652 Fix End */
extern rrm_uem_global_proc_t g_uem_gbl_proc;
extern rrm_return_et
rrm_ue_mgr_timer_event_handler (
        rrm_ue_states_et            ue_state, /*!< FSM states for UE   */
        rrm_ue_events_et            ue_event, /*!< current event for FSM */
        rrm_ue_global_context_t *p_g_rrm_ue_ctx /*!< Global UE Context */
        );

/* SPR 15539 start */
/* code removed */
/* SPR 15539 end */
extern rrm_bool_et
rrm_get_cell_specific_info (
    U8  meas_id,         /* IN  MeasID obtained in meas res ind */
    U16 phy_cell_id,    /* IN  physical cell ID obtained in meas results ind */
    rrm_cell_meas_data_t*  p_meas_data, /* OUT Parameters that are configured for this PCI  */
    rrm_ue_context_t*      p_rrm_ue_context,
    /* Fix for CSR 58972 start*/
        rrm_bool_et *p_pci_confusion_detected,
    /* Fix for CSR 58972 end*/
        /*SPR 15525 Fix Start*/   
        rrm_bool_et *p_pci_blacklisted
        /*SPR 15525 Fix End*/

);
/* Inter RAT changes start */
/* SRVCC SUPPORT START - Delete */
/****************************************************************************
 *** Function Name  : rrm_ue_validate_eutran_cell
 *** Inputs         : RRM global context
 ***                  Physical Cell Id
 *** Outputs        : None
 *** Returns        : RRM_SUCCESS/RRM_FAILURE
 *** Description    : Validates the given PCI against the HO restriction list
 ***                  and OAM configurations
 ******************************************************************************/
/* SPR 15674 Start */
rrm_return_et
rrm_ue_validate_eutran_cell(rrm_ue_context_t* p_ue_context, phy_cell_id_info_t *p_phy_cell_id_info);
/* SPR 15674 End */

/****************************************************************************
 **** Function Name  : rrm_cellm_get_redirected_carrier_freq
 **** Inputs         : UE Context
 ****                  Physical Cell Id
 **** Outputs        : None
 **** Returns        : Carrier Frequency corresponding to the given PCI
 **** Description    : returns the Carrier Frequency of the given PCI
 *******************************************************************************/
U16 rrm_cellm_get_redirected_carrier_freq(
                rrm_ue_context_t*   p_ue_context,       
                U16                 phy_cell_id); 

/****************************************************************************
 **** Function Name  : rrm_ue_start_csfb_blind_timer
 **** Inputs         : UE Context
 **** Outputs        : None
 **** Returns        : QTIMER
 **** Description    : This function is invoked when ERAB_SETUP/UE_CTX_MOD 
 *                     request comes with CSFB indicator. This function starts 
 *                     the blind timer CSFB.
 *******************************************************************************/
rrm_timer_t rrm_ue_start_csfb_blind_timer(rrm_ue_context_t *p_ue_context);

/* Inter RAT changes end */

/****************************************************************************
 * Function Name  : rrm_time_diff
 * Inputs         : time stamp information for time1 and time2 
 * Outputs        : NONE
 * Returns        : difference in seconds
 * Description    : Calculate the time difference in seconds for the inputs
 ****************************************************************************/
signed long 
rrm_time_diff (
    const RRM_TIME_T* ptm_one, 
    const RRM_TIME_T* ptm_two)
{
    /* SPR-21497 START */
    S16 rem = 0;
    rem = ((ptm_one->sec/RRM_MEGA) - (ptm_two->sec/RRM_MEGA));

    if(rem > 1)
    {
        return UERRM_MAX_TIME_FOR_UE_IN_CELL * RRM_SEC_TO_MILLI_SEC_CONV;
    }
    else
    {
        return ((rem * RRM_MEGA * RRM_SEC_TO_MILLI_SEC_CONV) +
                (((ptm_one->sec)%RRM_MEGA)*RRM_SEC_TO_MILLI_SEC_CONV+ptm_one->msec) -
                (((ptm_two->sec)%RRM_MEGA)*RRM_SEC_TO_MILLI_SEC_CONV+ptm_two->msec));
    }
    /* SPR-21497 END */
}

/****************************************************************************
 * Function Name  : rrm_get_system_time
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : Returns the system time in the defined structure
 * Description    : Gets the sys time and returns in RRM_TIME_T format
 ****************************************************************************/
RRM_TIME_T rrm_get_system_time(void)
{
    /* SPR 16504 Fix start */
    RRM_TIME_T      curtime = {0};
    /* SPR 16504 Fix stop */
    struct timeb    systime;

    RRM_FTIME(&systime);

    curtime.sec     = (unsigned int)systime.time;
    curtime.msec    = systime.millitm;

    return curtime;
}

/****************************************************************************
 * Function Name  : uerrm_get_ue_elapsed_time
 * Inputs         : UE context information
 * Outputs        : NONE
 * Returns        : Returns the time difference between ue start time and
 *                  current system time
 * Description    : Gets the diff between sys time and ue start time
 ****************************************************************************/
/* CSR 55938: code changes start */
signed long uerrm_get_ue_elapsed_time (rrm_ue_context_t* p_ue_context)
{
    RRM_TIME_T t2;
    t2 = rrm_get_system_time();
    return ((rrm_time_diff(&t2,&p_ue_context->ue_start_time))/RRM_SEC_TO_MILLI_SEC_CONV);
}
/* CSR 55938: code changes end */

/* CSR-58333-fix start*/
/****************************************************************************
 * Function Name  : rrm_ue_start_timer
 * Inputs         : 
 *      1. duration  : Duration in millisec.
 *      2.p_data     : Pointer to the data to be saved with timer and
 *                     it's not null pointer.
 *      3.size       : size of data.
 *      4.bool       : is it repeatable timer.
 * Outputs        : 
 * Returns        : timer struct 
 * Variables      : 
 * Description    : This function starts the timer.
 ****************************************************************************/
rrm_timer_t
rrm_ue_start_timer 
(
 /* Duration (in milliseconds) of the timer to be started */
 U32         timer_duration,
 /* Timer data that will be attached to timer */
 void        *p_timer_data,
 U16         timer_data_size,    /* Size of timer data */
 rrm_bool_t  is_repeated         /* Indicates whether timer is repeated */
 )
{
    QTIME qtime;
    void *p_timer_buf = RRM_PNULL;
    rrm_timer_t timer = RRM_PNULL;
    /* SPR 15822 Fix Start */
    rrm_ue_timer_buf_t *timer_buff = (rrm_ue_timer_buf_t *)p_timer_data;
    /* SPR 15822 Fix End */

    RRM_ASSERT(RRM_ZERO != timer_duration);
    /* SPR 16504 Fix start */
    p_timer_buf = (rrm_ue_timer_buf_t *)rrm_mem_get(timer_data_size);
    /* SPR 16504 Fix stop */

    /*Cov 94801 +-*/

    if(p_timer_buf == RRM_PNULL)
    {
        /*raising error event - TIMER_START_FAIL*/
        rrm_raise_error_event_notification(RRM_EE_TIMER_START_FAIL);
        return RRM_PNULL;
    }

    RRM_MEMCPY(p_timer_buf, p_timer_data, timer_data_size);

    qtime.s = timer_duration / 1000;
    qtime.us = (timer_duration % 1000) * 1000;

    timer = qvTimerStart(&qtime, p_timer_buf, is_repeated);

    /* SPR 15822 Fix Start */
    RRM_TRACE(RRM_TRUE, p_rrm_utils_facility_name, RRM_BRIEF, 
            "Timer Started: Timer [%p], TimerType [%u], TimerDuration [%u]msec", 
            timer, timer_buff->timer_type, timer_duration);
    /* SPR 15822 Fix End */

    if(timer == RRM_PNULL)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_uem_facility_name, RRM_ERROR, "rrm_ue_start_timer: Timer Start failed");
        /* Release the memory allocated for storing the timer data */
        RRM_MEM_FREE(p_timer_buf);

        /*raising error event - TIMER_START_FAIL*/
        rrm_raise_error_event_notification(RRM_EE_TIMER_START_FAIL);
        /* Coverity 75020 Fix Start */
        /* Coverity 75020 Fix End */
    }

    return timer;
}
/* CSR-58333-fix end*/

/****************************************************************************
 * Function Name  : rrm_ue_get_timer_data
 * Inputs         : 
 *      1.p_timerId : Pointer to Timer Identifier.
 * Outputs        : 
 * Returns        : 
 * Variables      : 
 * Description    : This function retrieves the data strored for the timer
 ****************************************************************************/
/* CSR-58333-fix start*/
void* rrm_ue_get_timer_data(rrm_timer_t  p_timerId)
/* CSR-58333-fix end*/
{
    void    *p_timerData = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_timerData = qvTimerData(p_timerId);

    RRM_ASSERT (RRM_PNULL != p_timerData);

    RRM_UT_TRACE_EXIT();

    return(p_timerData);
}

/****************************************************************************
 * Function Name  :rrm_ue_stop_timer
 * Inputs         : 
 *      1.p_timerId : Pointer to Timer Identifier.
 * Outputs        : 
 * Returns        : 
 * Variables      : 
 * Description    : This function stops the running timer
 ****************************************************************************/
/* CSR-58333-fix start*/
void rrm_ue_stop_timer (rrm_timer_t   p_timerId)
{
    void    *p_timerData = RRM_PNULL;
    U8 flag = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    if (RRM_PNULL != p_timerId)
    {
    /* SPR-17852 START */
    if(qvTimerRunning(p_timerId))
    {
        p_timerData = rrm_ue_get_timer_data(p_timerId);
            RRM_TRACE(g_cellm_log_on_off, 
                    p_rrm_cellm_facility_name, 
                    RRM_DETAILED, 
                    "%s: Stoping Timer: Id(%p),Type(%d)","UE", 
                    p_timerId, ((rrm_ue_timer_buf_t*)p_timerData)->timer_type);
            if(p_timerData != RRM_PNULL)
            {
                RRM_MEM_FREE(p_timerData);
            }
            qvTimerStop(p_timerId, RRM_PNULL);
            flag = RRM_ONE;
        }
    else
    {
      RRM_TRACE(RRM_TRUE, p_rrm_uem_facility_name, RRM_ERROR, "timer already expired or not running!");
    }
  }
  else
  {
    RRM_TRACE(RRM_TRUE, p_rrm_uem_facility_name, RRM_ERROR, "NULL timer passed!");
  }
  /* SPR-17852 END */
    if (!flag) {
        RRM_TRACE(g_cellm_log_on_off, 
                p_rrm_cellm_facility_name, 
                RRM_ERROR, 
                "%s: Stoping Timer: Id(%p) Invalid TimerID/Timer not RUNNING", 
                "UE:",p_timerId);
    }

    RRM_UT_TRACE_EXIT();
}
/* CSR-58333-fix end*/

/*SPR 17777 +-*/


/****************************************************************************
 * Function Name  : rrm_ue_sort_meas_result_decending
 * Inputs         : UE Context
 * Outputs        : NONE 
 * Returns        :  NONE
 * Description    : Sorts the cell in the desending order based on 
 *                  signal strength/quality.
 ****************************************************************************/
void rrm_ue_sort_meas_result_decending (
        cell_selection_priority_list_t	*p_cell_list
)
{
    U16 i = RRM_ZERO;
    U16 j = RRM_ZERO;

    /* SPR 15232 start */
    rrm_rpt_cell_info_t temp_cell_info = {RRM_ZERO};
    /* SPR 15232 end */

    RRM_UT_TRACE_ENTER ();

    /* SPR 15232 start */
    p_cell_list->next_target_cell_idx = RRM_ZERO;
    /* SPR 15232 end */
    if (p_cell_list->count) {
		for (i=RRM_ZERO;i<(p_cell_list->count-1);i++) {
			for (j=i+RRM_ONE;j<p_cell_list->count;j++) {
                /* SPR 15232 start */
                if (p_cell_list->reported_neighbor_cell_info[i].priority < p_cell_list->reported_neighbor_cell_info[j].priority) {
                    temp_cell_info =  p_cell_list->reported_neighbor_cell_info[i];
                    p_cell_list->reported_neighbor_cell_info[i] = p_cell_list->reported_neighbor_cell_info[j];
                    p_cell_list->reported_neighbor_cell_info[j] = temp_cell_info;
                    /* SPR 15232 end */
                }
            }
        }
    }
    RRM_UT_TRACE_EXIT();
}

/*BLR:SPID CHANGES START*/

/****************************************************************************
 * Function Name  : rrm_get_freq_priotity_list_spid_based
 * Inputs         : Subscriber profile id
 * Outputs        : friquency priority_list
 * Returns        :  Returns pointer to frequency priority list 
 * Description    : Retrieves assigned new frequncy priority based on spid list.
 ****************************************************************************/
rrm_freq_priority_list_t *rrm_get_freq_priotity_list_spid_based (
    rrm_ue_context_t*          p_ue_ctxt,
    rrm_cell_index_t           cell_index
)
{
    U16                        spid;
    rrm_priority_cfg_t *p_priority_cfg = RRM_PNULL;
    rrm_freq_priority_list_t *p_freq_priority = RRM_PNULL;
    
    RRM_UT_TRACE_ENTER();
    /* SPR 22553 FIXED START*/
    spid = p_ue_ctxt->rrm_sub_profile_id_info.sub_profile_id;
    /* SPR 22553 FIXED END*/
    p_freq_priority = rrm_cellm_get_freq_priority_params(cell_index);
    if(p_freq_priority == RRM_PNULL)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,\
                        "\n failed frequency priority params");
        return RRM_PNULL;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,\
                        "\n Successfully received frequency priority params ");
    }
	if(spid)
	{
    	p_priority_cfg = rrm_cellm_get_priority_cfg_params(cell_index);
    	if(p_priority_cfg == RRM_PNULL)
    	{
        	RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,\
                        "\n failed to get priority config params ");
        	return RRM_PNULL;
    	}
    	else
    	{
        	RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,\
                        "\n Successfully received priority config params ");
    	}

    	if(RRM_FAILURE == rrm_cellm_assign_new_priorities(p_freq_priority,
                                        p_priority_cfg, p_ue_ctxt))
    	{
        	RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,\
                        "\n failed to assign new priorities to frequencies ");
        	return RRM_PNULL;
    	}
    	else
    	{
        	RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,\
                    "\n Successfully assigned new priorities to frequencies");
    	}
	}
    RRM_UT_TRACE_EXIT();
    return p_freq_priority;
 
}

/****************************************************************************
 * Function Name  : rrm_get_priority_for_eutran_frequency
 * Inputs         : earfcn 
 * Outputs        : priority
 * Returns        : returns priority to the requested frequency  
 * Description    : Searches through list gives back priority 
 ****************************************************************************/
U16 rrm_get_priority_for_eutran_frequency (
        U16     earfcn,
        rrm_eutran_freq_priority_list_info_t  *p_freq_priority)
{
	U16 index;
    for(index=RRM_ZERO;index<p_freq_priority->count; index++)
    {
        if(earfcn == p_freq_priority->eutran_freq_priority_info[index].\
				carrier_freq)
        {
            return p_freq_priority->eutran_freq_priority_info[index].\
                       cell_reselection_priority;
        }
    }
    return INVALID_PRIORITY;
}

/****************************************************************************
 * Function Name  : rrm_get_priority_for_utran_frequency
 * Inputs         : uarfcn 
 * Outputs        : priority
 * Returns        : returns priority to the requested frequency  
 * Description    : Searches through list gives back priority 
 ****************************************************************************/
U16 rrm_get_priority_for_utran_frequency (
        U16     uarfcn,
        rrm_utran_freq_priority_list_info_t  *p_freq_priority)
{
	U16 index;
    for(index=RRM_ZERO;index<p_freq_priority->count; index++)
    {
        if(uarfcn == p_freq_priority->utran_freq_priority_info[index].\
				carrier_freq)
        {
            return p_freq_priority->utran_freq_priority_info[index].\
                       cell_reselection_priority;
        }
    }
    return INVALID_PRIORITY;
}
/****************************************************************************
 * Function Name  : rrm_get_priority_for_geran_frequency
 * Inputs         : uarfcn 
 * Outputs        : priority
 * Returns        : returns priority to the requested frequency  
 * Description    : Searches through list gives back priority 
 ****************************************************************************/
U16 rrm_get_priority_for_geran_frequency (
        U16     arfcn,
        rrm_geran_freq_priority_list_info_t  *p_freq_priority)
{
	U16 index;
    for(index=RRM_ZERO;index<p_freq_priority->count; index++)
    {
        if(arfcn == p_freq_priority->geran_freq_priority_info[index].\
				carrier_freq.starting_arfcn)
        {
            return p_freq_priority->geran_freq_priority_info[index].\
                       cell_reselection_priority;
        }
    }
    return INVALID_PRIORITY;
}
/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_ue_prioritize_intra_rat_meas_result
 * Inputs         : UE Context
 * Outputs        : NONE 
 * Returns        :  NONE
 * Description    : Sorts the cell in the desending order based on 
 *                  configured frequency priority.
 ****************************************************************************/
rrm_void_t rrm_ue_prioritize_intra_rat_meas_result (
        rrm_ue_context_t* p_ue_ctxt
)
{
    U8               i   = RRM_ZERO;
    rrm_ho_params_t*             p_ho_params = RRM_PNULL;
    rrm_cell_index_t             cell_index = p_ue_ctxt->cell_index;
    rrm_freq_priority_list_t     *p_freq_priority = RRM_PNULL;
    U8                           priority;

    /* CSR 00055769  : inter-freq HO fix  code changes start */
    RRM_UT_TRACE_ENTER();
    /* CSR 00055769  : inter-freq HO fix  code changes end */

	p_ho_params = (rrm_ho_params_t*)&p_ue_ctxt->ho_params;

    if(RRM_PNULL ==(p_freq_priority = rrm_get_freq_priotity_list_spid_based(
                    p_ue_ctxt,
                    cell_index)))
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,\
                "\n frequency priority structure received is NULL");
    }
    else
    {
		for (i = RRM_ZERO;i< p_ho_params->eutra_known_cell_selection_list.count; i++) 

        {
            priority = rrm_get_priority_for_eutran_frequency(
					p_ho_params->eutra_known_cell_selection_list.
					reported_neighbor_cell_info[i].target_earfcn,
                    &(p_freq_priority->eutran_freq_priority_list_info));
            if(priority != INVALID_PRIORITY)
            { 
                /* SPR 15232 start */
				p_ho_params->eutra_known_cell_selection_list.reported_neighbor_cell_info[i].priority = priority;
                /* SPR 15232 end */
            }
        }
		for (i = RRM_ZERO;i< p_ho_params->eutra_unknown_cell_selection_list.count; i++) 
		{
			priority = rrm_get_priority_for_eutran_frequency(
					p_ho_params->eutra_unknown_cell_selection_list.
					reported_neighbor_cell_info[i].target_earfcn,
					&(p_freq_priority->eutran_freq_priority_list_info));
			if(priority != INVALID_PRIORITY)
			{ 
				p_ho_params->eutra_unknown_cell_selection_list.reported_neighbor_cell_info[i].priority = priority;
    }
		}
		for (i = RRM_ZERO;i< p_ho_params->eutra_confused_cell_selection_list.count; i++) 
		{
			priority = rrm_get_priority_for_eutran_frequency(
					p_ho_params->eutra_confused_cell_selection_list.
					reported_neighbor_cell_info[i].target_earfcn,
					&(p_freq_priority->eutran_freq_priority_list_info));
			if(priority != INVALID_PRIORITY)
			{ 
                            /* SPR 16406 21AUG Start */
				p_ho_params->eutra_confused_cell_selection_list.reported_neighbor_cell_info[i].priority = priority;
                            /* SPR 16406 21AUG End */
			}
		}

	}

    /* CSR 00055769  : inter-freq HO code changes start */
    RRM_UT_TRACE_EXIT();
    /* CSR 00055769  : inter-freq HO code changes end */
}
/* SPR 16406 End */

/* SPR 16406 Start */
/* Code Removed */
/* SPR 16406 End */

/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_prioritize_geran_meas_results 
 * Inputs         : p_cell_selection_list, p_ncl_params 
 * Outputs        : NONE 
 * Returns        : NONE
 * Description    : Sorts the geran cell in the desending order based on 
 *                  configured frequency priority.
 ****************************************************************************/
rrm_void_t
rrm_prioritize_geran_meas_results
(
    rrm_ue_context_t *p_ue_context,
    cell_selection_priority_list_t *p_cell_selection_list
)
{
    RRM_UT_TRACE_ENTER();
    rrm_counter_t index = RRM_ZERO;
    rrm_counter_t count = RRM_ZERO;
    U8               priority = RRM_ZERO;
    U16				carrier_freq = RRM_ZERO;
    ncl_params_t    *p_ncl_params        = RRM_PNULL;
    p_ncl_params     = rrm_cellm_get_ncl_info(p_ue_context->cell_index);
    rrm_freq_priority_list_t *p_freq_priority = RRM_PNULL;
    if(RRM_PNULL ==(p_freq_priority = rrm_get_freq_priotity_list_spid_based(
                    p_ue_context,
                    p_ue_context->cell_index)))
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,\
                "\n frequency priority structure received is NULL");
    }
    else
    {
        for(index=RRM_ZERO;index<p_cell_selection_list->count;index++) 
        {
            for(count = RRM_ZERO; count<p_ncl_params->inter_rat_ncl.\
                    num_valid_geran_cell;count++)
            {
                /* SPR 15674 Start */
                /* SPR 15232 start */
                if((p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id == \
                            /* SPR 15232 end */
                            p_ncl_params->inter_rat_ncl.geran_freq_cells[count].pci) &&
                        (p_ncl_params->inter_rat_ncl.geran_freq_cells[count].bcch_arfcn == 
                         p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn) )
                    /* SPR 15674 End */
                {
                    carrier_freq = p_ncl_params->inter_rat_ncl.\
                                   geran_freq_cells[count].bcch_arfcn;
                    priority = rrm_get_priority_for_geran_frequency(
                            carrier_freq,
                            &(p_freq_priority->geran_freq_priority_list_info)); 
                    break;
                }
            }
            if(priority != INVALID_PRIORITY)
            {
                /* SPR 15232 start */
                p_cell_selection_list->reported_neighbor_cell_info[index].priority = priority;
                /* SPR 15232 end */
                RRM_TRACE ( 
                        g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Priority:[%d], carrier_freq:[%d], pci[%d]",
                        priority, carrier_freq, \
                        /* SPR 15232 start */
                        p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id);
                /* SPR 15232 end */
            }
        }
        if(p_cell_selection_list->count)
        {
            rrm_ue_sort_meas_result_decending(p_cell_selection_list);
        }
    }
    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_prioritize_utran_meas_results 
 * Inputs         : p_cell_selection_list, p_ncl_params 
 * Outputs        : NONE 
 * Returns        : NONE
 * Description    : Sorts the utran cell in the desending order based on 
 *                  configured frequency priority.
 ****************************************************************************/
rrm_void_t
rrm_prioritize_utra_meas_results
(
    rrm_ue_context_t *p_ue_context,
    cell_selection_priority_list_t *p_cell_selection_list
)
{
    RRM_UT_TRACE_ENTER();
    rrm_counter_t index = RRM_ZERO;
    rrm_counter_t count = RRM_ZERO;
    U8               priority = RRM_ZERO;
    U16				carrier_freq = RRM_ZERO;
    ncl_params_t    *p_ncl_params        = RRM_PNULL;
    p_ncl_params     = rrm_cellm_get_ncl_info(p_ue_context->cell_index);
    rrm_freq_priority_list_t *p_freq_priority = RRM_PNULL;
    if(RRM_PNULL ==(p_freq_priority = rrm_get_freq_priotity_list_spid_based(
                    p_ue_context,
                    p_ue_context->cell_index)))
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,\
                "\n frequency priority structure received is NULL");
    }
    else
    {

        for(index=RRM_ZERO;index<p_cell_selection_list->count;index++) 
        {
            for(count = RRM_ZERO; count<p_ncl_params->inter_rat_ncl.\
                    num_valid_utran_freq_cell;count++)
            {
                /* SPR 15674 Start */
                /* SPR 15232 start */
                if((p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id == \
                            /* SPR 15232 end */
                            p_ncl_params->inter_rat_ncl.utran_freq_cells[count].pci) &&
                        (p_ncl_params->inter_rat_ncl.utran_freq_cells[count].uarfcndl == 
                         p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn) )
                    /* SPR 15674 End */
                {
                    carrier_freq = p_ncl_params->inter_rat_ncl.\
                                   utran_freq_cells[count].uarfcndl;
                    priority = rrm_get_priority_for_utran_frequency(
                            carrier_freq,
                            &(p_freq_priority->utran_freq_priority_list_info)); 
                    break;
                }
            }
            if(priority != INVALID_PRIORITY)
            {
                /* SPR 15232 start */
                p_cell_selection_list->reported_neighbor_cell_info[index].priority = priority;
                /* SPR 15232 end */
                RRM_TRACE ( 
                        g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Priority:[%d], carrier_freq:[%d], pci[%d]",
                        priority, carrier_freq, \
                        /* SPR 15232 start */
                        p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id);
                /* SPR 15232 end */
            }
        }

        /* Now sort the list based on the Freq-priority */
        if(p_cell_selection_list->count)
        {
            rrm_ue_sort_meas_result_decending(p_cell_selection_list);
        }
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_ue_prioritize_inter_rat_meas_result
 * Inputs         : UE Context
 * Outputs        : NONE 
 * Returns        :  NONE
 * Description    : Sorts the cell in the desending order based on 
 *                  signal strength/quality.
 ****************************************************************************/
rrm_void_t rrm_ue_prioritize_inter_rat_meas_result (
        rrm_ue_context_t* p_ue_ctxt
        )
{
    rrm_ho_params_t* p_ho_params = RRM_PNULL;

    RRM_UT_TRACE_ENTER ();

    p_ho_params = (rrm_ho_params_t*)&p_ue_ctxt->ho_params;
    /* UTRAN Cell Sorting */
    rrm_prioritize_utra_meas_results(p_ue_ctxt,&(p_ho_params->utra_known_cell_selection_list));
    rrm_prioritize_utra_meas_results(p_ue_ctxt,&(p_ho_params->utra_unknown_cell_selection_list));
    rrm_prioritize_utra_meas_results(p_ue_ctxt,&(p_ho_params->utra_confused_cell_selection_list));

    /* GERAN Cell Sorting */
    rrm_prioritize_geran_meas_results(p_ue_ctxt, &(p_ho_params->geran_known_cell_selection_list));
    rrm_prioritize_geran_meas_results(p_ue_ctxt,&(p_ho_params->geran_unknown_cell_selection_list));
    rrm_prioritize_geran_meas_results(p_ue_ctxt,&(p_ho_params->geran_confused_cell_selection_list));

    RRM_UT_TRACE_EXIT();
}
/* SPR 16406 End */

/* SPR 15674 Start */
/****************************************************************************
 * Function Name  : rrm_get_phy_cell_id_from_sorted_eutran_list
 * Inputs         : UE Context
 * Outputs        : phy_cell_id 
 * Returns        : Returns stongest E-UTRA neighbour cell ID based on 
 * 					freq-priority 
 * Description    : Retrieves the strongest cell which can be chosen for HO.
 ****************************************************************************/
phy_cell_id_info_t rrm_get_phy_cell_id_from_sorted_eutran_list (
        rrm_ue_context_t *p_ue_ctxt,
        /*csg start*/
        rrm_bool_et *csg_membership_status_presence, /*outgoing param - csg membership presence flag for ncl*/
        rrm_bool_et *csg_id_presence /*outgoing param - csg membership presence flag for ncl*/
        /*csg end*/
        )
{
    U16					daho_pci 			= INVALID_PHYSICAL_CELL_ID;
    U32					daho_freq 			= RRM_ZERO;
    rrm_ho_params_t* 	p_ho_params 		= RRM_PNULL;
    /* SPR 18757 Fix Start */
    /* Code Removed */ 
    /* SPR 18757 Fix End */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t  phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 12897 Start */
    U8                  ret_car_freq_supported = RRM_ZERO;
    /* SPR 12897 End */
    /* SPR 10498 Fix Start */
    /* SPR 10498 Fix End */

    p_ho_params	= &(p_ue_ctxt->ho_params);


    /* SPR 16406 Start */
    U8 last_triggered_idx =  p_ho_params->eutra_known_cell_selection_list.\
                             next_target_cell_idx;
    /* SPR 16406 End */
    RRM_UT_TRACE_ENTER ();
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context(
            p_ue_ctxt->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,RRM_WARNING,
                "Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return phy_cell_id_info;
    }
    rrm_daho_cell_config_info_t*    p_daho_cfg = \
                                                 rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);
    /* SPR 18757 Fix Start */
     U32 carrier_freq       = RRM_ZERO;
     U8 valid_freqs         = RRM_ZERO;
     U8  freq_count         = RRM_ZERO;
     idle_mode_mobility_params_t   *p_idle_mode_mobility_params = RRM_PNULL;
     p_idle_mode_mobility_params = rrm_cellm_get_idle_mode_params\
                                  (p_ue_ctxt->cell_index);
     if(RRM_PNULL != p_idle_mode_mobility_params)
    {
        valid_freqs =  p_idle_mode_mobility_params->num_valid_inter_freq_list;
    }

    /* SPR 18757 Fix End */
    if((p_daho_cfg != RRM_PNULL) &&
            /*daho clean up(condition added for presence of daho cell)*/
            (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT) )
    {
        daho_pci = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
        daho_freq = p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn;
    }
    /* SPR 18757 Fix Start */

    else if((p_daho_cfg != RRM_PNULL) &&
		    (p_daho_cfg->bitmask & EUTRA_INTER_FREQ_DAHO_CELL_PRESENT))
    {
	    for(freq_count=RRM_ZERO; freq_count < valid_freqs; freq_count++)
	    {
		    carrier_freq = p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
			    eutra_carrier_arfcn;

		    if(RRM_PNULL != rrm_uem_search_inter_freq_daho_cell(p_ue_ctxt, p_daho_cfg, 
					    carrier_freq, &daho_pci))
		    {
			    break;
		    }
	    }
	    daho_pci = p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.
		    rrm_eutran_inter_freq_daho_cell_config_info->phy_cell_id;	
	    daho_freq = carrier_freq;
	    RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
			    "daho_pci = %d daho_freq = %d", daho_pci, daho_freq);
    }

    /* SPR 18757 Fix End */
    /* SPR 15232 start */
   /* SPR 16406 Start */
    if(p_ho_params->eutra_known_cell_selection_list.count > RRM_ZERO)
    {
        if(last_triggered_idx < p_ho_params->eutra_known_cell_selection_list.count)
        {
            p_ho_params->eutra_known_cell_selection_list.next_target_cell_idx++;
            if((p_ho_params->eutra_known_cell_selection_list.
                        reported_neighbor_cell_info[last_triggered_idx].
                        phy_cell_id  == daho_pci) && (daho_freq == p_ho_params->
                            eutra_known_cell_selection_list.
                            reported_neighbor_cell_info[last_triggered_idx].target_earfcn))
            {
                p_ho_params->eutra_known_cell_selection_list.bitmask |= RRM_UE_BLIND_CELL_HO_TRIGGERED;   
            }
            /*csg start*/
            /* CID_97168 start */
            *csg_membership_status_presence = 
                (rrm_bool_et)p_ho_params->eutra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].csg_membership_status_presence ;
            /* CID_97168 stop */
	        /* CID 97158 Start */
            *csg_id_presence =
                (rrm_bool_et)p_ho_params->eutra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].csg_id_presence;
	        /* CID 97158 End */
            /*csg end*/
            phy_cell_id_info.phy_cell_id = p_ho_params->eutra_known_cell_selection_list.\
                                           reported_neighbor_cell_info[last_triggered_idx].phy_cell_id;
            phy_cell_id_info.carr_earfcn = p_ho_params->eutra_known_cell_selection_list.\
                                           reported_neighbor_cell_info[last_triggered_idx].target_earfcn;
            RRM_UT_TRACE_EXIT();
            return phy_cell_id_info;
        }
    }
    if(!(p_ho_params->eutra_known_cell_selection_list.bitmask & \
                RRM_UE_BLIND_CELL_HO_TRIGGERED) && (p_daho_cfg != RRM_PNULL) &&
            /*daho clean up(condition added for presence of daho cell)*/
	    /* SPR 18757 Fix Start */
            ((p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT) ||
		(p_daho_cfg->bitmask & EUTRA_INTER_FREQ_DAHO_CELL_PRESENT)) &&
	    /* SPR 18757 Fix End */
            (RRM_TRUE == p_ho_params->a2_event_received)) /* A2 is received */
        /* SPR 15232 end */
    {
        /* SPR-11385-fix start */
        /* Suppose in meas report, if reported PCI is not in the neighbore list and cell_selection count
           is still zero. So handover triggered on DAHO cell */
        /* SPR-17852 START */
        if((( p_ho_params->blind_timer) && !(qvTimerRunning( p_ho_params->blind_timer))) || (RRM_ZERO == p_ho_params->eutra_known_cell_selection_list.count))
        {
        /* SPR-17852 END */
            /* SPR-11385-fix end */
	    /* SPR 18757 Fix Start */
            /* Code Removed */ 
            ret_car_freq_supported = rrm_ue_is_ue_supported_eutra_frequency (
                    daho_freq,      
                    p_ue_ctxt);

	    /* SPR 18757 Fix End */
            /* SPR 12897 Start */
            if(ret_car_freq_supported == RRM_ZERO)
                /* SPR 12897 End */
            {

                RRM_TRACE ( 
                        g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "EUTRAN DAHO Freq:[%d] is not supported by [UE:%d]",
                        carrier_freq,
                        p_ue_ctxt->ue_index);
                phy_cell_id_info.phy_cell_id = INVALID_PHYSICAL_CELL_ID; 
                phy_cell_id_info.carr_earfcn = RRM_ZERO;
                /* SPR 15232 start */
                RRM_UT_TRACE_EXIT();
                /* SPR 15232 end */
                return phy_cell_id_info;
            }
            RRM_TRACE ( 
                    g_uem_log_on_off, 
                    p_rrm_uem_facility_name,
                    RRM_WARNING,
                    "Trying HO to E-UTRAN Blind cell");
            /* return Blind cell id */
            p_ho_params->eutra_known_cell_selection_list.bitmask |= \
                                                         RRM_UE_BLIND_CELL_HO_TRIGGERED;
            phy_cell_id_info.phy_cell_id = daho_pci;
	    /* SPR 19075 Fix Start */ 
            phy_cell_id_info.carr_earfcn = daho_freq;
            /* SPR 19075 Fix End */
            /* SPR 15232 start */
            RRM_UT_TRACE_EXIT();
            /* SPR 15232 end */
            return phy_cell_id_info;
        }
    }
    /* SPR 16406 End */
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
}
/* SPR 15674 End */

/* SRVCC SUPPORT START */

/****************************************************************************
 * Function Name  : rrm_uem_configure_utran_srvcc
 * Inputs         : UE Context
 *                  SRVCC criteria as per UE capabilities
 *                  PS HO supported or not by Target cell
 *                  Target cell is voip-capable or not
 * Outputs        : SRVCC indication for HO Required
 * Returns        : RRM_SUCCESS - If the target cell can be selected for HO
 *                  RRM_FAILURE - If the target cell can NOT be selected for HO
 * Description    : Checks the UTRAN target cell capailities for HO(SRVCC/Normal HO).
 *                  Also, identifies the type of HO: CS, PS
 ****************************************************************************/
rrm_return_et
rrm_uem_configure_utran_srvcc(
    rrm_ue_context_t            *p_ue_ctxt,
    rrm_srvcc_ue_criteria_et    ue_srvcc_criteria,
    rrm_bool_et                 ps_ho_supported,
    rrm_bool_et                 voip_capable,
        /*SPR_15241_Start*/
    rrm_bool_et                 ue_support_voip
        /*SPR_15241_End*/
    )
{
    rrm_return_et ret_val = RRM_SUCCESS;

    RRM_UT_TRACE_ENTER ();

    if(RRM_UEM_SRVCC_HO_INDICATION_PRESENT & p_ue_ctxt->srvcc_config_info.bitmask)
    {
        p_ue_ctxt->srvcc_config_info.bitmask ^=
            RRM_UEM_SRVCC_HO_INDICATION_PRESENT;
    }

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
        "Target UTRAN cell PS HO Supported = %d, Voip-capable = %d",
        ps_ho_supported, voip_capable);

    switch(ue_srvcc_criteria)
    {
        case RRM_UE_SRVCC_CS_PS_TYPE:
            if(RRM_TRUE == ps_ho_supported)
            {
                /* Target Cell is Voip-incapable */
                /*SPR_15241_Start*/
                if((RRM_TRUE == voip_capable) &&
                        (RRM_TRUE == ue_support_voip))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "No need for SRVCC HO as UTRAN Target cell is Voip-capable");
                }
                /* Target Cell is Voip-incapable */
                else
                {
                    p_ue_ctxt->srvcc_config_info.bitmask |=
                        RRM_UEM_SRVCC_HO_INDICATION_PRESENT;
                    p_ue_ctxt->srvcc_config_info.srvcc_ho_indication =
                        RRM_SRVCC_CS_PS_TYPE;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "SRVCC HO required with type : CS_PS_TYPE");
                }
                /*SPR_15241_End*/
            }
            /* PS HO not supported by Cell */
            else
            {
                /* Target Cell is Voip-incapable */
                if(RRM_FALSE == voip_capable)
                {
                    p_ue_ctxt->srvcc_config_info.bitmask |=
                        RRM_UEM_SRVCC_HO_INDICATION_PRESENT;
                    p_ue_ctxt->srvcc_config_info.srvcc_ho_indication =
                        RRM_SRVCC_CS_ONLY_TYPE;
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "SRVCC HO required with type : CS_ONLY_TYPE");
                }
                /* Target Cell is Voip-capable */
                else
                {
                    /* Select another target cell */
                    ret_val = RRM_FAILURE;
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                            "Cell incorrectly configured as voip capable when PS HO is not supported!");
                }
            }
            break;

        case RRM_UE_SRVCC_OP_NOT_POSSIBLE:
            /* Voip-incapable cells cannot be seleted if SRVCC is not supported
               and voice bearer is established */
            if(RRM_FALSE == voip_capable)
            {
                /* Select another target cell */
                ret_val = RRM_FAILURE;
                /*bug 10051 Fix-Start*/
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                    "SRVCC op not possible. Need to select another target cell");
                /*bug 10051 Fix-End*/
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "PS-HO to be done");
            }
            break;

        case RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED:
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "SRVCC HO Ind not to be sent");

            /* For HO without SRVCC, Target Utran cell needs to support PS HO */
            if(RRM_TRUE == ps_ho_supported)
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "Utran cell supports PS HO");
            }
            else
            {
                /* Select another target cell */
                ret_val = RRM_FAILURE;
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                        "Utran cell does not spport PS HO, need to select another target cell");
            }
            break;
        default:
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                    "Incorrect value set for ue_srvcc_criteria = %d", ue_srvcc_criteria);
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_uem_check_voice_bearer_established
 * Inputs         : UE Context
 * Outputs        : None
 * Returns        : RRM_SUCCESS - If a voice bearer is established
 *                  RRM_FAILURE - If no voice bearer is established
 * Description    : Checks whether any voice bearer is established with the UE
 ****************************************************************************/
rrm_bool_et
rrm_uem_check_voice_bearer_established(
    rrm_ue_context_t            *p_ue_ctxt
    )
{
    U8                      erab_count = RRM_ZERO;
    U8                      index = RRM_ZERO;
    U8                      qci = RRM_ZERO;
    U8                      qci_count = RRM_ZERO;
    rrm_bool_et             voice_bearer_exists = RRM_FALSE;
    rrm_cell_context_t      *p_cell_context = RRM_PNULL;

    RRM_UT_TRACE_ENTER ();

    erab_count = p_ue_ctxt->drb_configured.num_of_list;

    if (erab_count > MAX_ERAB_COUNT)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                "Number of DRB is [%d] which is more than [%d]", erab_count, MAX_ERAB_COUNT);
        erab_count = MAX_ERAB_COUNT;
    }

    p_cell_context = rrm_cellm_get_cell_context(
            p_ue_ctxt->cell_index);
    /*CID 65790:start*/
    if (RRM_PNULL != p_cell_context)
    {
        for(index = RRM_ZERO ; (index < erab_count) &&
                (index < RRM_MAX_NUM_DRB_PER_UE); index++ )
        {
            qci = p_ue_ctxt->drb_configured.erab_item[index].
                erab_level_qos_params.qci;

            for(qci_count = RRM_ZERO; qci_count < RRM_MAX_QCI; qci_count++)
            {
                /* If the bearer with the QCI is a voice bearer */
                if((RRMCM_RMIF_SRVCC_OP_REQUIRED_PRESENT &
                            p_cell_context->epc_info.epc_params.qos_config_params[qci_count].bitmask) &&
                        (RRM_TRUE == p_cell_context->epc_info.epc_params.
                         qos_config_params[qci_count].srvcc_op_required) &&
                        (qci == p_cell_context->epc_info.epc_params.
                         qos_config_params[qci_count].qci))
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "Voice bearer exists for QCI = %d ERAB ID = %d", qci,
                            p_ue_ctxt->drb_configured.erab_item[index].erab_id);
                    voice_bearer_exists = RRM_TRUE;
                    break;
                }
            }
            if(RRM_TRUE == voice_bearer_exists)
            {
                /* No need to check further ERABs for the existence of voice bearers */
                break;
            }
        }
    }
    /*CID 65790:end*/
    if(RRM_FALSE == voice_bearer_exists)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
            "Voice bearer is NOT established for [UE:%d]", p_ue_ctxt->ue_index);
    }
    RRM_UT_TRACE_EXIT();
    return voice_bearer_exists;
}

/****************************************************************************
 * Function Name  : rrm_uem_check_srvcc_fgi
 * Inputs         : UE Context
 * Outputs        : None
 * Returns        : RRM_SUCCESS - If FGI bits required for SRVCC HO are set
 *                  RRM_FAILURE - If FGI bits required for SRVCC HO are NOT set
 * Description    : Checks if UE has capabilities for SRVCC HO based on FGI bits
 ****************************************************************************/
rrm_bool_et
rrm_uem_check_srvcc_fgi(
        rrm_ue_context_t            *p_ue_ctxt,
        /* SPR_13721_fix : start */
        rrm_cell_type_t             target_type
        /* SPR_13721_fix : end */
    )
{
U32             feature_group_indicators = (U32)-RRM_ONE;
    rrm_bool_et srvcc_fgi_supported = RRM_FALSE;

    RRM_UT_TRACE_ENTER ();

        feature_group_indicators =(U32)( p_ue_ctxt->ue_capability_params.
            rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.feature_group_indicators);

    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        /* SPR_13721_fix : start */
        switch (target_type)
        {
            case UTRA_CELL:
                {
        /* For UTRAN SRVCC HO FGI bits 8 and 27 need to be set */
        if((feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_EIGHT)))
                && (feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_TWENTY_SEVEN))))
        {
            srvcc_fgi_supported = RRM_TRUE;
        }
        else
        {
            srvcc_fgi_supported = RRM_FALSE;
        }
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "UTRAN srvcc_fgi_supported = %d", srvcc_fgi_supported);
                    break;
                }
            case GERAN_CELL:
                { 
                    /* For GERAN SRVCC to supported by UE, FGI bits 9 & 23 shall be true as per 36.331 v10.11.0 (Annex. B1) */
                    if((feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_NINE)))
                            && (feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_TWENTY_THREE))))
                    {
                        srvcc_fgi_supported = RRM_TRUE;
                    }
                    else
                    {
                        srvcc_fgi_supported = RRM_FALSE;
                    }
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "GERAN srvcc_fgi_supported = %d", srvcc_fgi_supported);
                    break;
                }
            default:
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "Invalid rat_type received.");
        }
        /* SPR_13721_fix : end */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
            "FGI bits not present in UE context");
    }

    RRM_UT_TRACE_EXIT();
    return srvcc_fgi_supported;
}

/* SRVCC SUPPORT END */

/*SPR_15241_Start*/
/****************************************************************************
 * Function Name  : rrm_check_ue_capability_for_voip
 * Inputs         : UE Context
 * Outputs        : None
 * Returns        : TRUE/FALSE
 * Description    :
 ****************************************************************************/
    rrm_bool_et
rrm_check_ue_capability_for_voip(
        rrm_ue_context_t *p_ue_ctxt,
        U32              serv_dl_earfcn
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    rrm_ue_eutra_capability_v9a0_ies_t         *p_rrm_ue_eutra_capability_v9a0_ies = RRM_PNULL;
    U8                  serv_freq_band;
    /* Start fix for coverity defect ID: 83047 */
    U8                  serv_fdd_tdd_bitmask = RRM_ZERO;
    /* End fix for coverity defect ID: 83047 */
    RRM_UT_TRACE_ENTER ();

    if (p_ue_ctxt->ue_capability_params.bitmask & RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT)
    {
        if(p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.bitmask & RRM_UE_EUTRA_CAPABILITY_V920_IES_PRESENT)
        {
            if (p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.ue_eutra_capability_v920_ies.bitmask & 
                    RRM_UE_EUTRA_CAPABILITY_V940_IES_PRESENT)
            {
                if (p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.ue_eutra_capability_v920_ies.ue_eutra_capability_v940_ies.\
                        bitmask & RRC_UE_EUTRA_CAPABILITY_V9a0_IES_PRESENT)
                {
                    p_rrm_ue_eutra_capability_v9a0_ies = &(p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.\
                            ue_eutra_capability_v920_ies.ue_eutra_capability_v940_ies.ue_eutra_capability_v9a0_ies);

                    if (p_rrm_ue_eutra_capability_v9a0_ies->bitmask & RRM_UE_EUTRA_CAPABILITY_V9c0_IES_PRESENT )
                    {
                        if (p_rrm_ue_eutra_capability_v9a0_ies->ue_eutra_capability_v9c0_ies.bitmask & 
                                RRM_UE_EUTRA_CAPABILITY_IRAT_PARAMS_UTRA_v9c0_PRESENT)
                        {
                            /* Start fix for Coverity_ID : 83045 */
                            serv_freq_band = rrm_ue_is_ue_supported_eutra_frequency(serv_dl_earfcn, p_ue_ctxt);
                            if (RRM_ZERO != serv_freq_band)
                            {
                                rrm_uem_is_eutra_freq_fdd_or_tdd(&serv_fdd_tdd_bitmask,serv_freq_band,serv_dl_earfcn);
                                RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,"serv_fdd_tdd_bitmask %d",serv_fdd_tdd_bitmask);
                                if (serv_fdd_tdd_bitmask & RRM_EUTRA_FDD_PRESENT)
                                {
                                    if ((p_rrm_ue_eutra_capability_v9a0_ies->ue_eutra_capability_v9c0_ies.inter_rat_parameters_utra_v9c0.bitmask & 
                                                RRM_VOICEOVERPS_HS_UTRA_FDD_R9_PRESENT) && 
                                            (p_rrm_ue_eutra_capability_v9a0_ies->ue_eutra_capability_v9c0_ies.inter_rat_parameters_utra_v9c0.\
                                             voice_over_ps_hs_utra_fdd_r9 == RRM_VOICEOVERPS_HS_UTRA_FDD_R9_SUPPORTED))
                                    {
                                        RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,"UE supported the VOIP in FDD mode");
                                        ret_val = RRM_TRUE;
                                    }
                                }
                                else if( serv_fdd_tdd_bitmask & RRM_EUTRA_TDD_PRESENT)
                                {
                                    if ((p_rrm_ue_eutra_capability_v9a0_ies->ue_eutra_capability_v9c0_ies.inter_rat_parameters_utra_v9c0.bitmask &
                                                RRM_VOICEOVERPS_HS_UTRA_TDD_R9_PRESENT) &&
                                            (p_rrm_ue_eutra_capability_v9a0_ies->ue_eutra_capability_v9c0_ies.inter_rat_parameters_utra_v9c0.\
                                             voice_over_ps_hs_utra_tdd128_r9 == RRM_VOICEOVERPS_HS_UTRA_TDD_R9_SUPPORTED))
                                    {
                                        RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,"UE supported the VOIP in TDD mode");
                                        ret_val = RRM_TRUE;
                                    }

                                }
                            }
                            else 
                            {
                                RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,RRM_ERROR,"Should never come here.");
                                ret_val = RRM_FALSE;
                            }
                            /* End fix for Coverity_ID : 83045 */
                        }
                    }
                }
            }
        }
    }
    /* Start fix for Coverity_ID : 83049 */
    RRM_UT_TRACE_EXIT ();
    /* End fix for Coverity_ID : 83049 */
    return ret_val;
}

/*SPR_15241_End*/

/* SPR 15674 Start */
/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_get_phy_cell_id_from_sorted_utran_list
 * Inputs         : UE Context
 * Outputs        : phy_cell_id 
 * Returns        : Returns stongest neighbour cell ID based on freq-priority 
 * Description    : Retrieves the strongest cell which can be chosen for HO.
 ****************************************************************************/
phy_cell_id_info_t  rrm_get_phy_cell_id_from_sorted_utran_list (
        rrm_ue_context_t *p_ue_ctxt,
        /*csg start*/
        rrm_bool_et *csg_membership_status_presence, /*outgoing param - csg membership presence flag for ncl*/
        rrm_bool_et *csg_id_presence /*outgoing param - csg membership presence flag for ncl*/
        /*csg end*/
        )
{
    rrm_bool_et						irat_utra_forbidden = RRM_FALSE;
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t              phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    rrm_bool_et						validation_required = RRM_TRUE;
    rrm_ho_params_t* 				p_ho_params	= &(p_ue_ctxt->ho_params);
    U16								daho_pci 	= INVALID_PHYSICAL_CELL_ID;
    U16					daho_freq 			= RRM_ZERO;
    /* SPR_14564_fix: start */
    /* Line deleted */
    /* SPR_14564_fix: end */
    /* SRVCC SUPPORT START */
    rrm_srvcc_ue_criteria_et        ue_srvcc_criteria = RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED;
    utran_freq_cells_t              *p_utra_cell  = RRM_PNULL;
    /* SRVCC SUPPORT END */
    /*SPR_15241_Start, Coverity Defect_ID:83048*/
    rrm_bool_et                     ue_support_voip = RRM_FALSE;
    /*SPR_15241_End, Coverity Defect_ID:83048*/

    rrm_daho_cell_config_info_t*    p_daho_cfg = \
                                                 rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);

    rrm_ho_restriction_list_t	*p_ho_restriction_list = \
                                                         &(p_ue_ctxt->ho_restriction_list);

    U8 last_triggered_idx = p_ho_params->utra_known_cell_selection_list.\
                            next_target_cell_idx;

    if((p_daho_cfg != RRM_PNULL) &&
            /*daho clean up(condition added for presence of daho cell)*/
            (p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT))
    {
        daho_pci = p_daho_cfg->utran_daho_cell_config_info.pci;
        daho_freq = p_daho_cfg->utran_daho_cell_config_info.uarfcndl;
    }

    RRM_UT_TRACE_ENTER ();
    /* SPR 17564 Fix Start */
    if ( (RRM_FALSE == rrm_is_ue_special_previledge(p_ue_ctxt)) &&
    /* SPR 17564 Fix End */
            (p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) )
    {
        if((p_ho_restriction_list->forbidden_inter_rat == \
                    FORBIDDEN_INTER_RAT_UTRAN_TYPE) || \
                (p_ho_restriction_list->forbidden_inter_rat == \
                 FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN) || \
                (p_ho_restriction_list->forbidden_inter_rat == \
                 FORBIDDEN_INTER_RAT_ALL))
        {
            irat_utra_forbidden = RRM_TRUE;
        }
    }
    if(p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD)
    {
        /* SPR_14216_start */
        if(p_ue_ctxt->ue_context_mod_req.csf_indicator == RRM_CSFB_HIGH_PRIORITY)
            /* SPR_14216_stop */
        {
            /* HO Restriction check is not needed */
            RRM_TRACE (g_uem_log_on_off, 
                    p_rrm_uem_facility_name,
                    RRM_DETAILED, 
                    "CSFB is HIGH PRIORITY, skipping HO restriction check");
            irat_utra_forbidden = RRM_FALSE;
            /* SPR 15406 Changes Starts */ 
            /* As per 36.413 v a60i, in case of High Priority CSFB Indicator, 
               eNB shall consider that no roaming area nor access restriction applies to UE.
               P.S: Validation is Still Required */ 
            /* validation is not required as CSFB is of High priority */
            // validation_required = RRM_FALSE; 
            /* SPR 15406 Changes Ends */ 
        }
    }
    if(irat_utra_forbidden == RRM_TRUE)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,
                "Inter RAT HO to UTRAN is forbidden");
        return phy_cell_id_info;
    }

    /* SRVCC SUPPORT START */
    /* If B2 is reported or blind timer expires */
    /* SPR-17852 START */
    if(((MEAS_REP_EVENT_B2 == p_ho_params->last_reported_meas_event) &&
                (!(p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD))) ||
            ((p_ue_ctxt->ho_params.blind_timer) && !(qvTimerRunning(p_ue_ctxt->ho_params.blind_timer))))
    {
    /* SPR-17852 END */
        /* If voice bearer is established and UE supports SRVCC */
        if(RRM_TRUE == rrm_uem_check_voice_bearer_established(p_ue_ctxt))
        {
            /* Check for SRVCC possibility in info recvd in ERAB Setup Req and UE Capability Ind */
            if(RRM_FALSE == p_ue_ctxt->srvcc_config_info.srvcc_ho_op_possible)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                        "SRVCC Op not possible as UE and MME are not capable of SRVCC");
                ue_srvcc_criteria = RRM_UE_SRVCC_OP_NOT_POSSIBLE;
            }
            /* SPR_13721_fix : start */
            else if(RRM_TRUE == rrm_uem_check_srvcc_fgi(p_ue_ctxt,UTRA_CELL))
                /* SPR_13721_fix : end */
            {
                ue_srvcc_criteria = RRM_UE_SRVCC_CS_PS_TYPE;
            }
            else
            {
                ue_srvcc_criteria = RRM_UE_SRVCC_OP_NOT_POSSIBLE;
            }
        }
        /* If voice bearer is NOT established, No need to include srvcc indication */
        else
        {
            ue_srvcc_criteria = RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED;
        }
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "UE SRVCC criteria = %d", ue_srvcc_criteria);

    }
    /* Store UE criteria for HO */
    p_ue_ctxt->srvcc_config_info.ue_srvcc_criteria = ue_srvcc_criteria;
    /* SRVCC SUPPORT END */

    /* SPR_15282_Fix_Start */
    /* SPR_14564_fix: start */
    p_ue_ctxt->cgi_reporting_triggered_for_meas_event = RRM_CGI_REP_CAUSE_MAX;
    /* SPR_14564_fix: end */
    /* SPR_15282_Fix_End */

    if(p_ho_params->utra_known_cell_selection_list.next_target_cell_idx <
            p_ho_params->utra_known_cell_selection_list.count)
    {
        if(validation_required == RRM_TRUE)
        {
            /* SPR_14564_fix: start */
            p_ue_ctxt->rrm_cgi_triggered_for_rat_type = NO_RAT_CONFIGURED;
            /* SPR_14564_fix: end */
            /* Before selecting the strongest cell validation is required */
            for(; last_triggered_idx < p_ho_params->utra_known_cell_selection_list.count; \
                    last_triggered_idx++)
            {
                /* SRVCC SUPPORT START */
                /* SPR 15406 Changes Starts */
                /*SPR_15241_Start*/
                if(RRM_PNULL != (p_utra_cell = rrm_ue_validate_utran_cell(p_ue_ctxt,\
                                p_ho_params->utra_known_cell_selection_list.\
                                reported_neighbor_cell_info[last_triggered_idx].phy_cell_id,&ue_support_voip))) 
                    /*SPR_15241_End*/
                    /* SPR 15406 Changes Ends */
                {
                    /*SPR_15241_Start*/
                    if(RRM_FAILURE == rrm_uem_configure_utran_srvcc(p_ue_ctxt, ue_srvcc_criteria,
                                (rrm_bool_et)p_utra_cell->ps_ho_supported, (rrm_bool_et)p_utra_cell->voip_capable,ue_support_voip))
                        /*SPR_15241_End*/
                    {
                        /* Select another Target Cell */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Target UTRAN cell with pci = %d cannot be selected"
                                "Need to check next candidate cell for HO",
                                p_utra_cell->pcpich_scrambling_code);
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Target UTRAN cell with pci = %d selected for HO",
                                /* SPR 15232 start */
                                p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].phy_cell_id);
                        /* SPR 15232 end */
                        break;
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Need to check next candidate cell for HO");
                    /* SRVCC SUPPORT END */
                }
            }

                }
        p_ho_params->utra_known_cell_selection_list.next_target_cell_idx = last_triggered_idx+RRM_ONE;
        if(last_triggered_idx <  p_ho_params->utra_known_cell_selection_list.count)
        {

            /* SPR 15232 start */
            if((p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].phy_cell_id == \
                        /* SPR 15232 end */
                        daho_pci) && (daho_freq == p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].target_earfcn))
            {
                p_ho_params->utra_known_cell_selection_list.bitmask |= \
                                                       RRM_UE_BLIND_CELL_HO_TRIGGERED;
            }
            /**csg start*/
            *csg_membership_status_presence = 
            /* CID_97168 start */
                /* SPR 15232 start */
                (rrm_bool_et)p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].csg_membership_status_presence;
            /* CID_97168 End */
	        /* CID 97158 Start */
            *csg_id_presence =
                (rrm_bool_et)p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].csg_id_presence;
	        /* CID 97158 End */
            /**csg end*/
            phy_cell_id_info.phy_cell_id = p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].phy_cell_id;
            phy_cell_id_info.carr_earfcn = p_ho_params->utra_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx].target_earfcn;
            RRM_UT_TRACE_EXIT();
            return phy_cell_id_info; 
            /* SPR 15232 end */
        }
    }
    /* SPR 13860 Fix Start */
    if(!(p_ho_params->utra_known_cell_selection_list.bitmask & \
                RRM_UE_BLIND_CELL_HO_TRIGGERED) && (INVALID_PHYSICAL_CELL_ID != daho_pci))
        /* SPR 13860 Fix End */
        /* SRVCC SUPPORT START */
    {
        phy_cell_id_info = rrm_validate_and_get_utran_daho_pci(p_ue_ctxt);
        if((phy_cell_id_info.phy_cell_id == daho_pci) && (daho_freq == phy_cell_id_info.carr_earfcn))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Valdation SUCCESS for DAHO UTRAN cell with pci = %d earfcn = %d", daho_pci, daho_freq);
            RRM_UT_TRACE_EXIT();
            return phy_cell_id_info;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Valdation Failed! DAHO UTRAN cell with pci = %d earfcn = %d cannot be selected", daho_pci, daho_freq);
        }
        /* SRVCC SUPPORT END */
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info; 
}
/* SPR 16406 End */
/* SPR 15674 End */
/* SPR_13721_fix : start */    
/****************************************************************************
 * Function Name  : rrm_uem_configure_geran_srvcc
 * Inputs         : UE Context
 *                  SRVCC criteria as per UE capabilities
 *                  DTM supported by target cell or not
 *                  DTM-HO supported by target cell or not
 *                  Target cell is voip-capable or not
 * 
 * Outputs        : SRVCC indication for HO Required
 * Returns        : RRM_SUCCESS - If the target cell can be selected for HO
 *                  RRM_FAILURE - If the target cell can NOT be selected for HO
 * Description    : Checks the GERAN target cell capailities for HO(SRVCC/Normal HO).
 *                  Also, identifies the type of HO: CS, PS
 *****************************************************************************/
rrm_return_et
rrm_uem_configure_geran_srvcc(
        rrm_ue_context_t            *p_ue_ctxt,
        rrm_srvcc_ue_criteria_et    ue_srvcc_criteria,
        rrm_bool_et                 dtm_ho_supported,
        rrm_bool_et                 dtm_supported,
        rrm_bool_et                 voip_capable
        )
{
    rrm_return_et ret_val = RRM_SUCCESS;
    U32 dtm_bitmask_present = RRM_FALSE;   /*0 if dtm not present else 1*/

    RRM_UT_TRACE_ENTER ();

    if(RRM_UEM_SRVCC_HO_INDICATION_PRESENT & p_ue_ctxt->srvcc_config_info.bitmask)
    {
        p_ue_ctxt->srvcc_config_info.bitmask ^=
            RRM_UEM_SRVCC_HO_INDICATION_PRESENT;
    }

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Target GERAN cell DTM HO Supported = %d, Voip-capable = %d, DTM Supported = %d",
            dtm_ho_supported, voip_capable,dtm_supported);

    if(p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.ue_eutra_capability_v920_ies.
            inter_rat_parameters_geran_v920.bitmask & RRM_UE_IRAT_PARAMS_GERAN_V920_DTM_R9_PRESENT)
    {
        dtm_bitmask_present = RRM_TRUE;
    }

    switch(ue_srvcc_criteria)
    {
        case RRM_UE_SRVCC_CS_PS_TYPE:
            {
                if((RRM_TRUE == dtm_ho_supported) && 
                        (RRM_TRUE == dtm_supported) && 
                        (dtm_bitmask_present == RRM_TRUE)) /*need to check dtm criteria for ue context but not clear how*/
                {
                    /* Target Cell is Voip-incapable */
                    if(RRM_FALSE == voip_capable)
                    {
                        p_ue_ctxt->srvcc_config_info.bitmask |=
                            RRM_UEM_SRVCC_HO_INDICATION_PRESENT;
                        p_ue_ctxt->srvcc_config_info.srvcc_ho_indication =
                            RRM_SRVCC_CS_PS_TYPE;

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "SRVCC HO required with type : CS_PS_TYPE");
                    }
                    /* Target Cell is Voip-capable */
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "No need for SRVCC HO as GERAN Target cell is Voip-capable");
                    }
                }
                /* PS HO not supported by Cell */
                else
                {
                    /* Target Cell is Voip-incapable */
                    if(RRM_FALSE == voip_capable)
                    {
                        p_ue_ctxt->srvcc_config_info.bitmask |=
                            RRM_UEM_SRVCC_HO_INDICATION_PRESENT;
                        p_ue_ctxt->srvcc_config_info.srvcc_ho_indication =
                            RRM_SRVCC_CS_ONLY_TYPE;
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "SRVCC HO required with type : CS_ONLY_TYPE");
                    }
                    /* Target Cell is Voip-capable */
                    else
                    {   
                        /* Select another target cell */
                        ret_val = RRM_FAILURE;
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "Cell incorrectly configured as voip capable when PS HO is not supported!");
                    }
                }
                break;
            }
        case RRM_UE_SRVCC_OP_NOT_POSSIBLE:
            {
                /* Voip-incapable cells cannot be seleted if SRVCC is not supported
                 *                and voice bearer is established */
                if(RRM_FALSE == voip_capable)
                {
                    /* Select another target cell */
                    ret_val = RRM_FAILURE;
                    /*bug 10051 Fix-Start*/
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "SRVCC op not possible. Need to select another target cell");
                    /*bug 10051 Fix-End*/
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "PS-HO to be done");
                }
                break;
            }
        case RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED:
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "SRVCC HO Ind not to be sent");
                /* For HO without SRVCC, Target Utran cell needs to support PS HO */
                /*SPR 19299 START*/
                if((p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD)&&
                        (p_ue_ctxt->ue_context_mod_req.csf_indicator != RRM_CSFB_HIGH_PRIORITY))
                {
                    if(RRM_TRUE == dtm_supported )
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Geran cell supports PS HO");
                }
                else
                {
                    /* Select another target cell */
                    ret_val = RRM_FAILURE;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Geran cell does not spport PS HO, need to select another target cell");
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Low Priority CSFB is not enabled on UE ,So,No need to check the dtm_supported.");
                }
                /*SPR 19299 END*/
                break;
            }
        default:
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Incorrect value set for ue_srvcc_criteria = %d", ue_srvcc_criteria);
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR_13721_fix : end */

/* SPR 16406 Start */
/* SPR 15674 Start */
/****************************************************************************
 * Function Name  : rrm_get_phy_cell_id_from_sorted_geran_list
 * Inputs         : UE Context
 * Outputs        : phy_cell_id 
 * Returns        : Returns stongest neighbour cell ID based on freq-priority 
 * Description    : Retrieves the strongest cell which can be chosen for HO.
 ****************************************************************************/
phy_cell_id_info_t rrm_get_phy_cell_id_from_sorted_geran_list (
        rrm_ue_context_t *p_ue_ctxt
        )
{
    rrm_bool_et						irat_geran_forbidden = RRM_FALSE;
    rrm_ho_params_t* 				p_ho_params	= &(p_ue_ctxt->ho_params);
    /* SPR 19364 start */
    /* Code removed */
    /* SPR 19364 end */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t              phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    rrm_bool_et						ret_car_freq_supported = RRM_TRUE;
    /* SPR_13721_fix : start */
    rrm_srvcc_ue_criteria_et        ue_srvcc_criteria = RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED;
    geran_freq_ncl_t                *p_geran_cell     = RRM_PNULL;
    /* SPR_13721_fix : end */
    /* SPR 15336 Fix Start */
    U8        dtm_ho_supp = RRM_ZERO;
    /* SPR 15336 Fix End */
    /* Start :SPR 1020 */
    /* Deleted the code related to FGI bit 9 check for GERAN */
    /* End :SPR 1020 */
    rrm_daho_cell_config_info_t*    p_daho_cfg = \
                                                 rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);

    rrm_ho_restriction_list_t	*p_ho_restriction_list = \
                                                         &(p_ue_ctxt->ho_restriction_list);

    U8 last_triggered_idx = p_ho_params->geran_known_cell_selection_list.\
                            next_target_cell_idx;
    /* SPR 19364 start */
    geran_freq_ncl_t        *p_temp_geran_cell          = RRM_PNULL;
    U8                      save_index                  = RRM_ZERO;
    /* SPR 19364 end */

    /* SPR 19364 start */
    /* Code removed */
    /* SPR 19364 end */
    RRM_UT_TRACE_ENTER ();

    /* ho restriction : code changes start */
    /* SPR 17564 Fix Start */
    if ( (RRM_FALSE == rrm_is_ue_special_previledge(p_ue_ctxt)) &&
    /* SPR 17564 Fix End */
            (p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) )
    {
        if((p_ho_restriction_list->forbidden_inter_rat == \
                    FORBIDDEN_INTER_RAT_GEREN_TYPE) || \
                (p_ho_restriction_list->forbidden_inter_rat == \
                 FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN) || \
                (p_ho_restriction_list->forbidden_inter_rat == \
                 FORBIDDEN_INTER_RAT_ALL))
        {
            irat_geran_forbidden = RRM_TRUE;
        }
    }
    if(p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD)
    {
        /* SPR_14216_start */
        if(p_ue_ctxt->ue_context_mod_req.csf_indicator == RRM_CSFB_HIGH_PRIORITY)
            /* SPR_14216_stop */
        {
            /* HO Restriction check is not needed */
            RRM_TRACE (g_uem_log_on_off, 
                    p_rrm_uem_facility_name,
                    RRM_DETAILED, 
                    "CSFB is HIGH PRIORITY, skipping HO restriction check");
            irat_geran_forbidden = RRM_FALSE;
            /* SPR 15406 Changes Starts */ 
            /* As per 36.413 v a60i, in case of High Priority CSFB Indicator, 
               eNB shall consider that no roaming area nor access restriction applies to UE.
               P.S: Validation is Still Required */ 
            /* validation is not required as CSFB is of High priority */
            /* SPR 15406 Changes Ends */ 
        }
    }
    if(irat_geran_forbidden == RRM_TRUE)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,
                "Inter RAT HO to GERAN is forbidden");
        return phy_cell_id_info;
    }
    /* SPR_13721_fix : start */
    if(((MEAS_REP_EVENT_B2 == p_ho_params->last_reported_meas_event) &&
                (!(p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD))) ||
            (p_ue_ctxt->ho_params.blind_timer_expired_flag))
    {
        /* If voice bearer is established and UE supports SRVCC */
        if(RRM_TRUE == rrm_uem_check_voice_bearer_established(p_ue_ctxt))
        {
            /* Check for SRVCC possibility in info recvd in ERAB Setup Req and UE Capability Ind */
            if(RRM_FALSE == p_ue_ctxt->srvcc_config_info.srvcc_ho_op_possible)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                        "SRVCC Op not possible as UE and MME are not capable of SRVCC");
                ue_srvcc_criteria = RRM_UE_SRVCC_OP_NOT_POSSIBLE;
            }
            else if(RRM_TRUE == rrm_uem_check_srvcc_fgi(p_ue_ctxt, GERAN_CELL))
            {
                ue_srvcc_criteria = RRM_UE_SRVCC_CS_PS_TYPE;
            }
            else
            {
                ue_srvcc_criteria = RRM_UE_SRVCC_OP_NOT_POSSIBLE;
            }
        }
        /* If voice bearer is NOT established, No need to include srvcc indication */
        else
        {
            ue_srvcc_criteria = RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED;
        }
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "GERAN UE SRVCC criteria = %d", ue_srvcc_criteria);
    }
    /* Store UE criteria for HO */
    p_ue_ctxt->srvcc_config_info.ue_srvcc_criteria = ue_srvcc_criteria;

    /* SPR_13721_fix : end */
    if(p_ho_params->geran_known_cell_selection_list.next_target_cell_idx <
            p_ho_params->geran_known_cell_selection_list.count)
    {
        /* Before selecting the strongest cell validation is required */
        for(; last_triggered_idx < p_ho_params->geran_known_cell_selection_list.count; \
                    last_triggered_idx++)
            {
                /* SPR_13721_fix : start */
                if(RRM_PNULL != (p_geran_cell = rrm_ue_validate_geran_cell(p_ue_ctxt,\
                            /* SPR 15232 start */
                        p_ho_params->geran_known_cell_selection_list.reported_neighbor_cell_info[last_triggered_idx],
                            /* SPR 15232 end */
                            /* SPR 15336 Fix Start */
                            &dtm_ho_supp))
                     /* SPR_13721_fix : end */
                        /* SPR 15336 Fix End */
                  )
                {
                     /* SPR_13721_fix : start */
                /* SPR 19364 start */
                if (RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED == ue_srvcc_criteria)
                {
                    /* Check if PS handover allowed by the eNB */
                    if (RRM_TRUE == rrm_cellm_get_ps_ho_flag(p_ue_ctxt->cell_index))
                    {
                        /* Check if target support PS handover */
                        if (RRM_TRUE == p_geran_cell->dtm_ho_supported)
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Target GERAN cell with pci = %d selected for PS Handover",
                                    p_geran_cell->pci);
                            break;
                        }
                        else
                        {
                           
                              /* Check if target is DTM supported */
                            if ((RRM_TRUE == p_geran_cell->dtm_supported) )
                            {
                                p_temp_geran_cell = p_geran_cell;
                                save_index = last_triggered_idx;
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                        "Target GERAN cell with pci = %d selected for CCO",
                                        p_geran_cell->pci);
                            }
                        }
                    }
                    else
                    {
                        /* Check if it is a CSFB case */
                        if (p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD) 
                        {
                            /* Check if target is DTM supported */
                            if (RRM_TRUE == p_geran_cell->dtm_supported)
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                        "Target GERAN cell with pci = %d selected for CSFB",
                                        p_geran_cell->pci);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    /* SPR 19364 end */
			if(RRM_FAILURE == rrm_uem_configure_geran_srvcc(p_ue_ctxt, 
							ue_srvcc_criteria,
                            /*Cov 94859_94791_94808+*/
							(rrm_bool_et)p_geran_cell->dtm_ho_supported,
							(rrm_bool_et)p_geran_cell->dtm_supported, 
							(rrm_bool_et)p_geran_cell->voip_capable))
                             /*Cov 94859_94791_94808 -*/
				{
					/*select another target cell*/
					RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
							"Target GERAN cell with pci = %d cannot be selected"
							"Need to check next candidate cell for HO",
							p_geran_cell->pci);
				}
				else
                    {
					RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
							"Target GERAN cell with pci = %d selected for HO",
							p_geran_cell->pci);
                        break;
                    }
                     /* SPR_13721_fix : end */
                }
            /* SPR 19364 start */
            }
            /* SPR 19364 end */
                else
                {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Need to check next candidate cell for HO");
                }
            /* SPR 19364 start */
	    p_geran_cell = RRM_PNULL;
            /* SPR 19364 end */
            }

        /* SPR 19364 start */
        if (RRM_PNULL != p_temp_geran_cell)
        {
            last_triggered_idx = save_index;
            p_geran_cell = p_temp_geran_cell;
        }
        /* SPR 19364 end */
        p_ho_params->geran_known_cell_selection_list.next_target_cell_idx = last_triggered_idx+RRM_ONE;
        if(last_triggered_idx <  p_ho_params->geran_known_cell_selection_list.count)
        {
            /* SPR 19364 start */
            /* Code removed */
            /* SPR 19364 end */
            /* SPR 19364 start */
            phy_cell_id_info.phy_cell_id = p_geran_cell->pci;;
            phy_cell_id_info.carr_earfcn = p_geran_cell->bcch_arfcn;
            if (p_geran_cell->dtm_ho_supported)
            {
                phy_cell_id_info.bitmask |= RRM_CELL_DTM_HO_SUPPORTED;
            }
            if (p_geran_cell->dtm_supported)
            {
                phy_cell_id_info.bitmask |= RRM_CELL_DTM_SUPPORTED;
            }
            /* SPR 19364 end */
            RRM_UT_TRACE_EXIT();
            return phy_cell_id_info; 
        }
    }
    if(!(p_ho_params->geran_known_cell_selection_list.bitmask & \
                RRM_UE_BLIND_CELL_HO_TRIGGERED) &&
            /*daho clean up(condition added for presence of daho cell)*/
            (RRM_PNULL != p_daho_cfg) && (p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT)) /* Coverity : CID 21185 Added NULL check for p_daho_cfg */
    {
      /* SPR-17852 START */
        /* Freq-Priority Fix changes start */
        if(((p_ue_ctxt->ho_params.blind_timer) && !(qvTimerRunning(p_ue_ctxt->ho_params.blind_timer))) || 
                (p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_BLIND_TIMER_EXPIRED))
            /* Freq-Priority Fix changes end */
        {
          /* SPR-17852 END */

            /* SPR 19364 start */
            /* Code removed */
            /* SPR 19364 end */

            ret_car_freq_supported = rrm_ue_is_ue_supported_geran_frequency (
                    /* SPR 19364 start */
                    p_daho_cfg->geran_daho_cell_config_info.bcch_arfcn,
                    /* SPR 19364 end */
                    p_ue_ctxt);
            /* Start :SPR 1020 */
            if((ret_car_freq_supported == RRM_FALSE))
            {
                /* SPR 19364 start */
                RRM_TRACE ( 
                        g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "GERAN DAHO Freq:[%d] is not supported by [UE:%d]",
                        p_daho_cfg->geran_daho_cell_config_info.bcch_arfcn,
                        p_ue_ctxt->ue_index);
                /* SPR 19364 end */
            }
            else
            {
                /* SPR 19364 start */
                phy_cell_id_info.phy_cell_id = p_daho_cfg->geran_daho_cell_config_info.pci; 
                phy_cell_id_info.carr_earfcn = p_daho_cfg->geran_daho_cell_config_info.bcch_arfcn;
                if (p_daho_cfg->geran_daho_cell_config_info.dtm_ho_supported)
                {
                    phy_cell_id_info.bitmask |= RRM_CELL_DTM_HO_SUPPORTED;
                }
                if (p_daho_cfg->geran_daho_cell_config_info.dtm_supported)
                {
                    phy_cell_id_info.bitmask |= RRM_CELL_DTM_SUPPORTED;
                }
                /* SPR 19364 end */
                /* End :SPR 1020 */
                /* return Blind cell id */
                RRM_TRACE ( 
                        g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "Trying HO to GERAN Blind cell");
                p_ho_params->geran_known_cell_selection_list.bitmask |= \
                                                        RRM_UE_BLIND_CELL_HO_TRIGGERED;
            }
        }
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info; 
}
/* SPR 16406 End */
/* SPR 15674 End */

/* SPR 10498 Fix Start */
rrm_eutran_inter_freq_daho_cell_config_info_t * 
rrm_uem_search_inter_freq_daho_cell(
    rrm_ue_context_t                *p_ue_ctxt,
	rrm_daho_cell_config_info_t     *p_daho_cfg,
    U16                             earfcn,
    U16                             *p_daho_pci
)
{
    rrm_eutran_inter_freq_daho_cell_config_info_t *p_inter_freq_daho = RRM_PNULL;
    U8  index = RRM_ZERO;
    rf_params_t  *rf_parameters = rrm_cellm_get_rf_params(p_ue_ctxt->cell_index);

    RRM_UT_TRACE_ENTER();

    if(((earfcn != RRM_ZERO) &&
                earfcn != rf_parameters->rf_configurations.dl_earfcn)
            && (p_daho_cfg->bitmask & EUTRA_INTER_FREQ_DAHO_CELL_PRESENT))
    {
        for(index = RRM_ZERO;
                (index < p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.count)
                && (index < MAX_NO_INTER_FREQ); index++)
        {
            if(earfcn == p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.
                    rrm_eutran_inter_freq_daho_cell_config_info[index].earfcn)
            {
                *p_daho_pci = p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.
                    rrm_eutran_inter_freq_daho_cell_config_info[index].phy_cell_id;

                RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                        "DAHO cell pci = %d for Inter-freq = %d",
                        *p_daho_pci, earfcn);
                p_inter_freq_daho = &(p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.
                    rrm_eutran_inter_freq_daho_cell_config_info[index]);
                break;
            }
        }

    }
    RRM_UT_TRACE_EXIT();
    return p_inter_freq_daho;
}

rrm_void_t
rrm_uem_find_eutran_daho_cell(
    rrm_ue_context_t                *p_ue_ctxt,
	rrm_daho_cell_config_info_t     *p_daho_cfg,
    U32                             earfcn,
    U16                             *p_daho_pci
)
{            
    rf_params_t                 *rf_parameters = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    rf_parameters = rrm_cellm_get_rf_params(p_ue_ctxt->cell_index);
    if((RRM_ZERO != earfcn) && (earfcn != rf_parameters->rf_configurations.dl_earfcn))
    {
        if(RRM_PNULL != rrm_uem_search_inter_freq_daho_cell(p_ue_ctxt, p_daho_cfg, earfcn, p_daho_pci))
        {
            RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                    "DAHO cell found for Inter-freq = %u" 
                    " Inter-Freq PCI: [%u]",earfcn, *p_daho_pci);
        }
        else if(p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT)
        {
            *p_daho_pci = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
            RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                    "DAHO cell not found for Inter-freq, trying for Intra-freq Daho cell."
                    " Intra-freq PCI: [%u]", *p_daho_pci);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name, RRM_ERROR,
                    "DAHO cell not present for Intra-freq or Intra-freq cells.");
            *p_daho_pci = INVALID_PHYSICAL_CELL_ID;
        }
    }
    else if(p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT)
    {
        *p_daho_pci = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
        RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                "Intra-freq Daho cell found."
                " Intra-freq PCI: [%u]", *p_daho_pci);
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name, RRM_ERROR,
                "DAHO cell not present for Intra-freq or Intra-freq cells.");
        *p_daho_pci = INVALID_PHYSICAL_CELL_ID;
    }

    RRM_UT_TRACE_EXIT();
}
/* SPR 10498 Fix End */

/* SPR 15674 Start */
/* Rel10_CRs 408 start */
/****************************************************************************
 * Function Name  : rrm_validate_csg_id_from_selected_phy_cell_id
 * Inputs         : UE Context
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Validate csg id from selected phy cell id.
 ****************************************************************************/
    phy_cell_id_info_t
rrm_validate_csg_id_from_selected_phy_cell_id (
        rrm_cell_type_t   target_cell_type,
        rrm_bool_et   *csg_membership_status_presence,
        rrm_bool_et *csg_id_presence,
        rrm_ue_context_t *p_ue_ctxt
        )
{
    U8                          intra_count = RRM_ZERO;              
    U8                          inter_count = RRM_ZERO;              
    U8                          intra_phycell_id_matched_flag = RRM_FALSE;              
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t          phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR_15636_Fix: Start */
    /* Code deleted */
    /* SPR_15636_Fix: End */
    lte_ncl_t                   *p_ncl_params = RRM_PNULL;
    /* Start :SPR 8928 */
    U16					         daho_pci = INVALID_PHYSICAL_CELL_ID;
    U16                         daho_freq = RRM_ZERO;
    rrm_daho_cell_config_info_t* p_daho_cfg = RRM_PNULL;
    /* End :SPR 8928 */

    RRM_UT_TRACE_ENTER();

    rrm_cell_context_t *p_cell_ctxt = rrm_cellm_get_cell_context(p_ue_ctxt->cell_index); 
    /* SPR_15636_Fix: Start */
    /* Code deleted */
    /* SPR_15636_Fix: End */

    /* Coverity 65791 Fix Start */
    if(RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,
                "rrm_cellm_get_cell_context return Null");
        return phy_cell_id_info;
    }
    /* Coverity 65791 Fix End */
    
    p_ncl_params = rrm_cellm_get_ncl_params(p_ue_ctxt->cell_index);

    /* Start :SPR 8928 */
    p_daho_cfg = rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);
    if((p_daho_cfg != RRM_PNULL) &&
            (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT) )
    {
        daho_pci = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
        daho_freq = p_cell_ctxt->ran_info.rf_params.rf_configurations.dl_earfcn;
    }
    /* End :SPR 8928 */
    phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_eutran_list(
            p_ue_ctxt, csg_membership_status_presence,
            csg_id_presence);

    while (INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
    {
        /* Start :SPR 8928 */
        if ((phy_cell_id_info.phy_cell_id) == daho_pci && (daho_freq == phy_cell_id_info.carr_earfcn))
        {
            RRM_UT_TRACE_EXIT();
            return phy_cell_id_info;
        }
        /* End :SPR 8928 */

        /* Reset this flag to false */
        intra_phycell_id_matched_flag = RRM_FALSE;
        if(p_ncl_params != RRM_PNULL && target_cell_type == EUTRA_CELL)
        {
            for(intra_count = RRM_ZERO; ((intra_count < p_ncl_params->num_valid_intra_freq_cell) 
                        && (intra_count < MAX_INTRA_FREQ_CELLS)); intra_count++)
            {
                if((phy_cell_id_info.phy_cell_id == p_ncl_params->intra_freq_cells[intra_count].phy_cell_id) &&
                        (phy_cell_id_info.carr_earfcn == p_cell_ctxt->ran_info.rf_params.rf_configurations.dl_earfcn))
                {
                    if((p_ncl_params->intra_freq_cells[intra_count].bitmask & RRMCM_RMIF_INTRA_FREQ_CELL_ACCESS_MODE_PRESENT) &&
                            ((RRM_OAM_ACCESS_MODE_CLOSE == p_ncl_params->intra_freq_cells[intra_count].cell_access_mode) ||
                             (RRM_OAM_ACCESS_MODE_HYBRID == p_ncl_params->intra_freq_cells[intra_count].cell_access_mode)))
                    /* SPR_15636_Fix: Start */
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,\
                                "PCI selected [PCI: %d] has Access MODE [%d]",
                                phy_cell_id_info.phy_cell_id,
                                p_ncl_params->intra_freq_cells[intra_count].cell_access_mode);

                        RRM_UT_TRACE_EXIT();
                        return phy_cell_id_info;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,\
                                "PCI selected [PCI: %d] has Open Access MODE",
                                phy_cell_id_info.phy_cell_id);
                        RRM_UT_TRACE_EXIT();
                        return phy_cell_id_info;
                    }
                    /* SPR_15636_Fix: End */
                    /* SPR: 15922 : Start */
                    /* phycell_id found in intra freq cells */
                    /* SPR: 15922 : End */
                }
            }
            /* No need to search phycell_id in inter freq cells if phycell_id found in intra freq cells */
            if (!intra_phycell_id_matched_flag)
            {
                for(inter_count = 0; ((inter_count < p_ncl_params->num_valid_inter_freq_cell) 
                            && (inter_count < MAX_INTER_FREQ_CELLS)); inter_count++)
                {
                    if((phy_cell_id_info.phy_cell_id == p_ncl_params->inter_freq_cells[inter_count].phy_cell_id) &&
                            (phy_cell_id_info.carr_earfcn == p_ncl_params->inter_freq_cells[inter_count].eutra_carrier_arfcn))
                    {
                        if((p_ncl_params->inter_freq_cells[inter_count].bitmask & RRMCM_RMIF_CELL_ACCESS_MODE_PRESENT) &&
                                ((RRM_OAM_ACCESS_MODE_CLOSE == p_ncl_params->inter_freq_cells[inter_count].cell_access_mode) ||
                                 (RRM_OAM_ACCESS_MODE_HYBRID == p_ncl_params->inter_freq_cells[inter_count].cell_access_mode)))
                        /* SPR_15636_Fix: Start */
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,\
                                    "PCI selected [PCI: %d] has Access MODE [%d]",
                                    phy_cell_id_info.phy_cell_id,
                                    p_ncl_params->inter_freq_cells[inter_count].cell_access_mode);

                            RRM_UT_TRACE_EXIT();
                            return phy_cell_id_info;
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,\
                                    "PCI selected [PCI: %d] has Open Access MODE",
                                    phy_cell_id_info.phy_cell_id);

                            RRM_UT_TRACE_EXIT();
                            return phy_cell_id_info;
                        }
                        /* SPR_15636_Fix: End */
                        break;
                    }
                }
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,\
                    "NCL params are not available for cell_index[%d] [UE:%d] for RAT [%d]",
                    p_ue_ctxt->cell_index,p_ue_ctxt->ue_index, target_cell_type);
            break;
        }

        phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_eutran_list(
                p_ue_ctxt, csg_membership_status_presence,
                csg_id_presence);
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
}
/* SPR 15674 End */
/* Rel10_CRs 408 end */
/* SPR 15674 Start */
/* Rel10_CRs 408 end */
/****************************************************************************
 * Function Name  : rrm_ue_get_strongest_phy_cell_id_for_ho
 * Inputs         : UE Context
 * Outputs        : NONE 
 * Returns        : Returns stongest neighbour cell ID 
 * 		    (based on SPID and freq-priority list) in the list that
 *                  is currently Active, otherwise INVALID_PHYSICAL_CELL_ID
 * Description    : Retrieves the strongest cell which can be chosen for HO.
 ****************************************************************************/

phy_cell_id_info_t rrm_ue_get_strongest_phy_cell_id_for_ho (
        rrm_ue_context_t *p_ue_ctxt,
        /*csg start*/
        rrm_bool_et   *csg_membership_status_presence, /*outgoing param*/
        rrm_bool_et *csg_id_presence /*outgoing param*/ 
        /*csg end*/

        )
{
    /*BLR:SPID CHANGES START*/
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    U8                          i = RRM_ZERO;
    /* SPR 20653 Fix Start */
    rrm_oam_rat_priority_info_et    get_eutran_priority = RRM_OAM_RAT_PRIORITY_LEAST;
    rrm_oam_rat_priority_info_et    get_utran_priority = RRM_OAM_RAT_PRIORITY_LEAST;
    rrm_oam_rat_priority_info_et    get_geran_priority = RRM_OAM_RAT_PRIORITY_LEAST;
    rrm_oam_rat_priority_info_et    get_cdma_priority  = RRM_OAM_RAT_PRIORITY_LEAST;
    /* SPR 20653 Fix End */
    rrm_ho_params_t*            p_ho_params = &(p_ue_ctxt->ho_params);
    rrm_cell_index_t            cell_index = p_ue_ctxt->cell_index;
    rrm_oam_spid_table_t        *spid_table_params = RRM_PNULL;
    /* SPR_14092_fix: start */
    rrm_bool_et                 is_spid_present = RRM_FALSE;
    /* SPR_14092_fix: end */


    RRM_UT_TRACE_ENTER ();


    spid_table_params = rrm_cellm_get_spid_table_params(cell_index);
    if(RRM_PNULL != spid_table_params && RRM_ZERO != spid_table_params->spid_count)
    {
        /* SPR_14564_fix: start */
        for(i =RRM_ZERO; i<spid_table_params->spid_count ; i++)
            /* SPR_14564_fix: end */
        {
                /* SPR 20653 Fix Start */

            /* SPR 22553 FIXED START*/
            if(spid_table_params->spid_config[i].spid == p_ue_ctxt->rrm_sub_profile_id_info.sub_profile_id)
            {
            /* SPR 22553 FIXED END*/
                get_rat_priority(&spid_table_params->spid_config[i].rrm_rat_priority_list, 
                        &get_eutran_priority, &get_utran_priority, &get_geran_priority, &get_cdma_priority);
                /* SPR 20653 Fix End */

                /* SPR_14092_fix: start */
                is_spid_present = RRM_TRUE;
                /* SPR_14092_fix: end */
                break;
            }
        }

    }


    /* Special handling when CSFB flag is received in UE_CTX_MOD 
     * request */
    /* SPR 22553 FIXED START*/
    if((p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD) &&\
            /* SPR_14092_fix: start */
            (is_spid_present == RRM_TRUE)
            &&
            (p_ue_ctxt->rrm_sub_profile_id_info.sub_profile_id != RRM_ZERO))
    /* SPR 22553 FIXED END*/
        /* SPR_14092_fix: end */
    {
        /* E-UTRAN shall not be considered in this case even though it has 
         * high priority */
        /* SPR 20653 Fix Start */
        if(RRM_OAM_RAT_PRIORITY_HIGH == get_utran_priority)
            /* SPR 20653 Fix End */
        { 
            phy_cell_id_info =rrm_get_phy_cell_id_from_sorted_utran_list(
                    p_ue_ctxt, csg_membership_status_presence,
                    csg_id_presence);
            if(phy_cell_id_info.phy_cell_id == INVALID_PHYSICAL_CELL_ID)
            {
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_ERROR,
                        "There are no valid UTRAN cells for CSFB,"
                        " trying for GERAN");
                phy_cell_id_info =rrm_get_phy_cell_id_from_sorted_geran_list(
                        p_ue_ctxt);
                if(phy_cell_id_info.phy_cell_id == INVALID_PHYSICAL_CELL_ID)
                {
                    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                            RRM_ERROR,
                            "There are no valid GERAN cells for CSFB.");
                }     
                else
                {
                    /* bug 982 */
                    p_ue_ctxt->target_cell_type = GERAN_CELL;
                    /* bug 982 */
                    return phy_cell_id_info;
                }  
            }
            else
            {
                /* bug 982 */
                p_ue_ctxt->target_cell_type = UTRA_CELL;
                /* bug 982 */
                return phy_cell_id_info;
            }  
        }
        /* SPR 20653 Fix Start */
        else if(RRM_OAM_RAT_PRIORITY_HIGH == get_geran_priority )
        /* SPR 20653 Fix End */
        {
            phy_cell_id_info =rrm_get_phy_cell_id_from_sorted_geran_list(
                    p_ue_ctxt);
            if(phy_cell_id_info.phy_cell_id == INVALID_PHYSICAL_CELL_ID)
            {
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_ERROR,
                        "There are no valid GERAN cells for CSFB,"
                        " trying for UTRAN cells");
                phy_cell_id_info =rrm_get_phy_cell_id_from_sorted_utran_list(
                        p_ue_ctxt, csg_membership_status_presence, 
                        csg_id_presence);
                if(phy_cell_id_info.phy_cell_id == INVALID_PHYSICAL_CELL_ID)
                {
                    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                            RRM_ERROR,
                            "There are no valid GERAN cells for CSFB.");
                }     
                else
                {
                    /* bug 982 */
                    p_ue_ctxt->target_cell_type = UTRA_CELL;
                    /* bug 982 */
                    return phy_cell_id_info;
                }  
            }
            else
            {
                /* bug 982 */
                p_ue_ctxt->target_cell_type = GERAN_CELL;
                /* bug 982 */
                return phy_cell_id_info;
            }
        }
        else
        {
            /* SPR 22553 FIXED START*/
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_ERROR,
                    "Invalid SPID value:[%d]",p_ue_ctxt->rrm_sub_profile_id_info.sub_profile_id);
            /* SPR 22553 FIXED END*/
        }
    }

    /* SPR_14092_fix: start */
    /* Code Deleted */
    /* SPR_14092_fix: end */
    else
    {
        /*BLR:SPID CHANGES START*/
        /* Get the latest report received and then try HO to that cell */
        switch(p_ho_params->last_reported_meas_event)
        {
            /* SPR_14092_fix: start */
            /* Code Deleted */
            /* SPR_14092_fix: end */
		/* SPR 16587 Start */
            case MEAS_REP_EVENT_A1:
            case MEAS_REP_EVENT_A2:
            case MEAS_REP_EVENT_A3:
            case MEAS_REP_EVENT_A4:
            case MEAS_REP_EVENT_A5:
		/* SPR 16587 End */
                {
                    /* Get the PCI from the E-UTRAN cell list based on 
                     * freq-priority */
                    /* CSR 00055769  : inter-freq HO code changes start */
                    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "TRY HO on EUTRAN cell listed based on freq-priority for [UE:%d]",
                            p_ue_ctxt->ue_index);
                    /* CSR 00055769  : inter-freq HO code changes end */

                    /* SPR 16406 Start */
                    /* Rel10_CRs 408 start */

                    if(RRM_EUTRAN_KNOWN_CELL_LIST == p_ue_ctxt->rrm_cell_selection_info.current_cell_list)
                    {
                    phy_cell_id_info = rrm_validate_csg_id_from_selected_phy_cell_id (
                            /* SPR 21446 start */
                            EUTRA_CELL,
                            /* SPR 21446 end */
                            csg_membership_status_presence,
                            csg_id_presence,
                            p_ue_ctxt);
                        if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                        {

                            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "PCI selected form eutran_known_cell_selection_list[%u]for [UE:%d]",
                                    phy_cell_id_info.phy_cell_id, p_ue_ctxt->ue_index);
                            /* SPR 314 start */
                            p_ue_ctxt->target_cell_type = EUTRA_CELL;
                            /* SPR 314 end */

                        }
                       
                    }
                    /* SPR-17852 START */
                    /* SPR DAHO HO START */
                    /*SPR_17893_START*/
                    if((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id))
                    {
                        if((p_ue_ctxt->ho_params.blind_timer) && !(qvTimerRunning(p_ue_ctxt->ho_params.blind_timer)))
                        {  
                            if(RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT & p_ue_ctxt->ue_capability_params.bitmask)
                            {
                                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                        "UTRAN UE RADIO CAPABILITY is present for [UE:%d]",
                                        p_ue_ctxt->ue_index);
                                phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_utran_list(
                                        p_ue_ctxt, csg_membership_status_presence,
                                        csg_id_presence);
                                if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                                {
                                    p_ue_ctxt->target_cell_type = UTRA_CELL;
                                }
                                else
                                {
                                    phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_geran_list(
                                            p_ue_ctxt);
                                    if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                                    {
                                        p_ue_ctxt->target_cell_type = GERAN_CELL;
                                    }

                                }
                            }
                            else
                            {
                                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                        "UTRAN UE RADIO CAPABILITY is not present for [UE:%d],Checking GERAN first",
                                        p_ue_ctxt->ue_index);
                                phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_geran_list(
                                        p_ue_ctxt);
                                if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                                {
                                    p_ue_ctxt->target_cell_type = GERAN_CELL;
                                }
                                else
                    {
                        phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_utran_list(
                                p_ue_ctxt, csg_membership_status_presence,
                                csg_id_presence);
                        if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                        {
                            p_ue_ctxt->target_cell_type = UTRA_CELL;
                        }
                                }
                            }
                        }
                        else
                        {
                            if ((RRM_UTRAN_KNOWN_CELL_LIST == 
                                        p_ue_ctxt->rrm_cell_selection_info.current_cell_list) 
                                    && 
                                    (p_ho_params->utra_known_cell_selection_list.count > RRM_ZERO)) 
                            {
                                phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_utran_list(
                                        p_ue_ctxt, csg_membership_status_presence,
                                        csg_id_presence);

                                if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                                {
                                    p_ue_ctxt->target_cell_type = UTRA_CELL;
                                }
                            }
                            else if((RRM_GERAN_KNOWN_CELL_LIST == 
                                 p_ue_ctxt->rrm_cell_selection_info.current_cell_list) &&  
                                  (p_ho_params->geran_known_cell_selection_list.count > RRM_ZERO))
                            {
                            phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_geran_list(
                                    p_ue_ctxt);

                            if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                            {
                            p_ue_ctxt->target_cell_type = GERAN_CELL;
                        }
                    }
                    }
                    }
                    /*SPR_17893_END*/
                    /* SPR DAHO HO END */

                    /* Rel10_CRs 408 end */
                    break;
                }
                
            case MEAS_REP_EVENT_B1:
            case MEAS_REP_EVENT_B2:
                /* SPR_14564_fix: start */
            case MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON: 
                /* SPR_14564_fix: end */
                {
                    /* Get the inter RAT cells based on freq-priority */
                    if ((RRM_UTRAN_KNOWN_CELL_LIST == 
                        p_ue_ctxt->rrm_cell_selection_info.current_cell_list) 
                        && 
                        (p_ho_params->utra_known_cell_selection_list.count > RRM_ZERO)) 
                    {
                        phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_utran_list(
                                p_ue_ctxt, csg_membership_status_presence,
                                csg_id_presence);
                        if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                        {
                        /* SPR 314 start */
                        p_ue_ctxt->target_cell_type = UTRA_CELL;
                        /* SPR 314 end */
                    }
                    }
                    else if(p_ho_params->geran_known_cell_selection_list.count > RRM_ZERO)
                    {
                        p_ue_ctxt->rrm_cell_selection_info.current_cell_list = RRM_GERAN_KNOWN_CELL_LIST;
                        phy_cell_id_info = rrm_get_phy_cell_id_from_sorted_geran_list(
                                p_ue_ctxt);
                        if(INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
                        {
                            /* SPR 314 start */
                            p_ue_ctxt->target_cell_type = GERAN_CELL;
                            /* SPR 314 end */
                        }
    /* SPR 17108 FIX START*/
                        /*break;*/
    /* SPR 17108 FIX END*/
                    }
             }
    /* SPR 17108 FIX START*/
            break;
    /* SPR 17108 FIX END*/
            default:
                phy_cell_id_info.phy_cell_id = INVALID_PHYSICAL_CELL_ID;
                break;
        }
        /* Log an error if the cell id is invalid */
        if(phy_cell_id_info.phy_cell_id == INVALID_PHYSICAL_CELL_ID)
        {
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_ERROR,
                    "There are no valid Target cells for HO");
        }
    }
/* SPR 16406 End */
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
}
/* SPR 15674 End */


/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_ue_update_neighbor_list
 * Inputs         : rrm_ue_context_t
 *                  entry/exit condition of the ue
 *                  Reported Event (A1-A5)
 *                  Physical_cell_id of the neighbour
 *                  Ms value for the neighbour cell
 * Outputs        : None
 * Returns        : None 
 * Description    : Adds/Removes the neighbour information 
 ****************************************************************************/
rrm_void_t
rrm_ue_update_neighbor_list (
        rrm_ue_context_t *p_ue_context,
    /*SPR 17777 +-*/
        U16  phy_cell_id, /* This array will contain the physical cell ids */
        /* SPR 15674 Start */
        rrm_cell_meas_data_t  trg_meas_data, /* This is the RSRP or RSRQ value corresponding to physical cell id */
        /* SPR 15674 End */
        /*csg start*/
        rrm_son_eutran_neighbor_cell_t *p_rrm_son_eutran_neighbor_cell,
        /*csg end*/
        /* Fix for CSR 58972 start*/
        /* SPR 15232 start */
        rrm_bool_et *p_pci_confusion_detected,
        /* SPR 16406 3Aug Start */
        rrm_bool_et unknown_pci_detected,
        /* SPR 16406 3Aug End */
        /* SPR 15232 end */
        /* Fix for CSR 58972 end*/
        /* SPR 15232 start */
        U8 event_type
        /* SPR 15232 end */
)
{
    rrm_ho_params_t*                    p_ho_params = RRM_PNULL;
    cell_selection_priority_list_t*     p_known_cell_selection = RRM_PNULL;
    cell_selection_priority_list_t*     p_unknown_cell_selection = RRM_PNULL;
    cell_selection_priority_list_t*     p_confused_cell_selection = RRM_PNULL;
    cell_selection_priority_list_t*     p_cell_selection_node = RRM_PNULL;
    rrm_bool_et  unknown_cell_found = RRM_FALSE;
    /* SPR 15674 Start */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, phy_cell_id, trg_meas_data.carr_earfcn, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 15674 End */


    U16     index           = RRM_ZERO;
    U8      del_mod_flg = RRM_ZERO;
    U16     idx         = RRM_ZERO;
    U8      add_flg     = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    p_ho_params = (rrm_ho_params_t*)&p_ue_context->ho_params;

    p_known_cell_selection = &(p_ho_params->eutra_known_cell_selection_list);
    p_unknown_cell_selection = &(p_ho_params->eutra_unknown_cell_selection_list);
    p_confused_cell_selection = &(p_ho_params->eutra_confused_cell_selection_list);

/* SPR 16406 3Aug Start */
    if(!unknown_pci_detected)
    {
    /* SPR 15674 Start */
    if(RRM_FAILURE == rrm_ue_validate_eutran_cell(p_ue_context, &phy_cell_id_info))
        /* SPR 15674 End */
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
               RRM_INFO,
                "Reported eutran cell pci[%u] EARFCN [%d] is not found in NCL",
               phy_cell_id, phy_cell_id_info.carr_earfcn);
        unknown_cell_found = RRM_TRUE;
    }
    }
/* SPR 16406 3Aug End */
    /* CSR 00055769  : inter-freq HO code changes start */
    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
            RRM_BRIEF,
            "p_eutra_known_cell_selection->count =%u " 
            "p_eutra_unknown_cell_selection->count =%u "
            "p_eutra_confused_cell_selection->count =%u for [UE:%u] ",
            p_known_cell_selection->count,
            p_unknown_cell_selection->count,
            p_confused_cell_selection->count,
            p_ue_context->ue_index);
    /* CSR 00055769  : inter-freq HO  code changes end */
    if(*p_pci_confusion_detected)
    {
        /* SPR 16406 8SEP start */
        if (RRM_TRUE == rrm_is_anr_allowed_on_eutran_freq(p_ue_context, p_ue_context->carrier_earfcn))
        {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_BRIEF, "Eutran cell[%u] reported on EARFCN [%u] is confused",
                    phy_cell_id, p_ue_context->carrier_earfcn);
        p_cell_selection_node = p_confused_cell_selection;  
    }
        else
        {
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_WARNING,
                    "CGI reporting not supported on Eutran cell[%u]"
                    " reported on EARFCN [%u] for confused cell, hence ignoring",
                    phy_cell_id, p_ue_context->carrier_earfcn);
        }
        /* SPR 16406 8SEP end */
    }
/* SPR 16406 3Aug Start */
    else if(unknown_pci_detected || unknown_cell_found)
    {
        /* SPR 16406 8SEP start */
        if (RRM_TRUE == rrm_is_anr_allowed_on_eutran_freq(p_ue_context, p_ue_context->carrier_earfcn))
        {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_BRIEF, "Eutran cell[%u] reported on EARFCN [%u] is unknown",
                    phy_cell_id, p_ue_context->carrier_earfcn);
        p_cell_selection_node = p_unknown_cell_selection;  
    }
        else
        {
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_WARNING,
                    "CGI reporting not supported on Eutran cell[%u]"
                    " reported on EARFCN [%u] for unknown cell, hence ignoring",
                    phy_cell_id, p_ue_context->carrier_earfcn);
        }

        /* SPR 16406 8SEP end */
    }
/* SPR 16406 3Aug End */
    else 
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_BRIEF, "eutran cell[%u] reported is known ",
                phy_cell_id);
        p_cell_selection_node = p_known_cell_selection;  
    }


    if(p_cell_selection_node)
    {
        /* SPR 16406 18sep Start */
        p_cell_selection_node->ranking_done = RRM_FALSE;
        for (index = RRM_ZERO;index<(p_cell_selection_node->count);index++) 
        {
           /* SPR 15674 Start */
            if (p_cell_selection_node->reported_neighbor_cell_info[index].phy_cell_id == phy_cell_id
                    && p_cell_selection_node->reported_neighbor_cell_info[index].target_earfcn == trg_meas_data.carr_earfcn)
            {
                /* SPR 15674 End */
                RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Physical Cell (%d) is modified from [UE:%d]"
                        " with RSRX(%d)",
                        phy_cell_id,
                        p_ue_context->ue_index,
                        /* SPR 15674 Start */
                        trg_meas_data.meas_result);                
                /* SPR 15674 End */
                if ((!(unknown_pci_detected || unknown_cell_found)) && (!(*p_pci_confusion_detected)))
                {
                    /* It is a known cell, no need to increment for unknown and
                     * confused cell because they should be handled as known
                     * once CGI is known */
                    p_cell_selection_node->reported_neighbor_cell_info[index].reported_count++;
                }
                p_cell_selection_node->reported_neighbor_cell_info[index].rsrx_value = trg_meas_data.meas_result;
                /* SPR-13322-fix start */
                p_cell_selection_node->reported_neighbor_cell_info[index].phy_cell_id = phy_cell_id;
                /* SPR-13322-fix end */
                /* SPR 15674 Start */
                p_cell_selection_node->reported_neighbor_cell_info[index].target_earfcn = trg_meas_data.carr_earfcn;
                /* SPR 15674 End */
                p_cell_selection_node->reported_neighbor_cell_info[index].reason_event_type |= event_type;
                /*csg start*/
                if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT)
                {
                    p_cell_selection_node->reported_neighbor_cell_info[index].csg_membership_status = p_rrm_son_eutran_neighbor_cell->csg_membershipStatus;                   
                    p_cell_selection_node->reported_neighbor_cell_info[index].csg_membership_status_presence = RRM_TRUE;
                }
                else
                {
                    p_cell_selection_node->reported_neighbor_cell_info[index].csg_membership_status = RRM_NOT_MEMBER; 
                    p_cell_selection_node->reported_neighbor_cell_info[index].csg_membership_status_presence = RRM_FALSE;
                }

                if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_PRESENT)
                {
                    p_cell_selection_node->reported_neighbor_cell_info[index].csg_id_presence = RRM_TRUE; 
                }
                else
                {
                    p_cell_selection_node->reported_neighbor_cell_info[index].csg_id_presence = RRM_FALSE;
                }

                del_mod_flg = RRM_ONE;
                break;
            }
        }
        if (!del_mod_flg)
        {
            idx = p_cell_selection_node->count;
            if (idx < MAX_NEIGH_CELL) 
            {
                p_cell_selection_node->reported_neighbor_cell_info[idx].phy_cell_id = phy_cell_id;
                p_cell_selection_node->reported_neighbor_cell_info[idx].rsrx_value = trg_meas_data.meas_result; 
                /* SPR-13322-fix start */
                p_cell_selection_node->reported_neighbor_cell_info[idx].phy_cell_id = phy_cell_id;
                /* SPR-13322-fix start */
                /* SPR 15674 Start */
                p_cell_selection_node->reported_neighbor_cell_info[idx].target_earfcn = phy_cell_id_info.carr_earfcn;
                /* SPR 15674 End */
                p_cell_selection_node->count++;
                /*csg start*/

                p_cell_selection_node->reported_neighbor_cell_info[idx].reason_event_type |= event_type;

                if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT)
                {
                    p_cell_selection_node->reported_neighbor_cell_info[idx].csg_membership_status = p_rrm_son_eutran_neighbor_cell->csg_membershipStatus;
                    p_cell_selection_node->reported_neighbor_cell_info[idx].csg_membership_status_presence = RRM_TRUE;
                }
                else
                {
                    p_cell_selection_node->reported_neighbor_cell_info[idx].csg_membership_status = RRM_NOT_MEMBER;
                    p_cell_selection_node->reported_neighbor_cell_info[idx].csg_membership_status_presence = RRM_FALSE;
                }

                if(p_rrm_son_eutran_neighbor_cell->bitmask & RRM_SON_EUTRAN_NC_CSG_PRESENT)
                {
                    p_cell_selection_node->reported_neighbor_cell_info[idx].csg_id_presence = RRM_TRUE;
                }
                else
                {
                    p_cell_selection_node->reported_neighbor_cell_info[idx].csg_id_presence = RRM_FALSE;
                    /* SPR 15232 end */
                }
                /*csg end*/
                add_flg = RRM_ONE;
                p_cell_selection_node->reported_neighbor_cell_info[idx].reported_count = RRM_ONE;
            }
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "Physical Cell [%u] Updated in cell selection list" 
                    "for [UE:%u] with RSRX(%u) : %s",
                    phy_cell_id,
                    p_ue_context->ue_index,
                    /* SPR 15674 Start */
                    trg_meas_data.meas_result,
                    /* SPR 15674 End */
                    add_flg?"SUCCESS":"FAILED");                
        }
                /* SPR 16406 18sep End */

    }
    /* CSR 00055769  : inter-freq HO code changes start */
    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
            RRM_INFO,
            "Status after update for PCI [%d], EARFCN[%d] p_eutra_known_cell_selection->count =%u " 
            "p_eutra_unknown_cell_selection->count =%u "
            "p_eutra_confused_cell_selection->count =%u",
            phy_cell_id_info.phy_cell_id, phy_cell_id_info.carr_earfcn,
            p_known_cell_selection->count,
            p_unknown_cell_selection->count,
            p_confused_cell_selection->count);
    /* CSR 00055769  : inter-freq HO  code changes end */

    RRM_UT_TRACE_EXIT();
}
/* SPR 16406 End */


/* Lipa Sipto Changes */
/****************************************************************************
 * Function Name  : rrm_ue_handle_lipa_bearer_release
 * Inputs         : rrm_ue_context_t  *p_ue_context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Release ue/lipa bearers before proceeding with HO Required.
 ****************************************************************************/
rrm_return_et
rrm_ue_handle_lipa_bearer_release(rrm_ue_context_t  *p_ue_context, rrm_ue_lipa_release_et  *p_is_rab_ue_released)
{
    rrm_return_et                            ret_val = RRM_SUCCESS;
    S8                                       erab_index = RRM_ZERO;
    U8                                       lipa_rab_index = RRM_ZERO;
    rrm_bool_et                              lipa_sipto_erab_release = RRM_FALSE;
    rrm_ue_erab_setup_item_list_t            *p_erab_setup_item_success_list = RRM_PNULL;
    rrm_bool_et                              erb_release_already_processed = RRM_FALSE;
    U8                                       success_count = RRM_ZERO;
    U8                                       total_rab = RRM_ZERO;
    rrc_rrm_erb_release_ind_t                rrc_rrm_erb_release_ind;

    RRM_UT_TRACE_ENTER();

    RRM_MEMSET(&rrc_rrm_erb_release_ind, RRM_ZERO,
            sizeof(rrc_rrm_erb_release_ind_t));


    rrc_rrm_erb_release_ind.ue_index = p_ue_context->ue_index;


    total_rab = p_ue_context->drb_configured.num_of_list;

		p_erab_setup_item_success_list =
			p_ue_context->current_procedure.p_erb_success_list;

	for (erab_index = RRM_ZERO;
           erab_index < p_ue_context->drb_configured.num_of_list;
              erab_index++)
	{
		if (p_ue_context->drb_configured.erab_item[erab_index].bitmask &
				RRM_UEM_ERAB_TO_BE_SETUP_LIPA_RAB_IND_PRESENT_FLAG)
		{
			if (RRM_TRUE == p_ue_context->drb_configured.erab_item[erab_index].lipa_rab_ind)
			{
				if(RRM_PNULL != p_erab_setup_item_success_list)
				{
					erb_release_already_processed = RRM_FALSE;
					for(success_count = RRM_ZERO; 
							(success_count < p_erab_setup_item_success_list->num_of_list) 
                            /* Coverity 21oct Start : 55002 */
                            && (success_count < RRM_MAX_NUM_DRB_PER_UE);
                            /* Coverity 21oct End : 55002 */
							success_count++)
					{
						if(p_erab_setup_item_success_list->erab_item[success_count].erab_id == 
								p_ue_context->drb_configured.erab_item[erab_index].erab_id)
						{
							erb_release_already_processed = RRM_TRUE;
                            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                                    "[UE:%d]: erb setup/modify/release process is ongoing", p_ue_context->ue_index);

						}
					}

					if(RRM_TRUE == erb_release_already_processed)
					{
						continue;
					}
				}

                rrc_rrm_erb_release_ind.erab_to_be_released_item_list.\
                  erab_to_be_release_item[lipa_rab_index].erab_id = 
					p_ue_context->drb_configured.erab_item[erab_index].erab_id;

                rrc_rrm_erb_release_ind.erab_to_be_released_item_list.
                    erab_to_be_release_item[lipa_rab_index].cause.type = RADIO_NETWORK_LAYER_TYPE;
                rrc_rrm_erb_release_ind.erab_to_be_released_item_list.
                    erab_to_be_release_item[lipa_rab_index].cause.value = UNSPECIFIED;

				lipa_rab_index++;
				lipa_sipto_erab_release = RRM_TRUE;
			}
		}
	}

	if(RRM_TRUE == lipa_sipto_erab_release)
	{
        if(total_rab == lipa_rab_index)
        {
            if(RRM_SUCCESS == rrm_build_and_send_ue_connection_release_ind(
                                 p_ue_context->ue_index,
                                 RADIO_NETWORK_LAYER_TYPE,
                                 UNSPECIFIED,
                                 RRM_ZERO,
                                 RRM_PNULL,
                                 /* SPR_10729_fix */
                                 p_ue_context->cell_index))
            {
		        *p_is_rab_ue_released = UE_LIPA_UE_RELEASE;
                /* Bug 16121 Fix Start */
                if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                {
                    RRM_TRACE(g_uem_log_on_off,
                            p_rrm_uem_facility_name,
                            RRM_ERROR,
                            "Failed to clean up ue resources for the [UE:%d]",
                            p_ue_context->ue_index);
                    ret_val = RRM_FAILURE;
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off,
                            p_rrm_uem_facility_name,
                            RRM_DETAILED,
                            "successfully cleaned up resources for the [UE:%d]",
                            p_ue_context->ue_index);
                    ret_val = RRM_FAILURE;
                }
                /* Bug 16121 Fix End */

                ret_val = RRM_SUCCESS;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "unable to send Ue Release Req");
                ret_val = RRM_FAILURE;
            }
        }
        else 
        {
            rrc_rrm_erb_release_ind.erab_to_be_released_item_list.count = lipa_rab_index;
            if(RRM_SUCCESS == rrm_build_and_send_lipa_erb_release_ind(
                                  &rrc_rrm_erb_release_ind, p_ue_context->cell_index))
            {
		    *p_is_rab_ue_released = UE_LIPA_RAB_RELEASE;

                ret_val = RRM_SUCCESS;
            }
            else
            {

                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "unable to send Bearer Release Ind");
                ret_val = RRM_FAILURE;
            }
        }
	}

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Lipa Sipto Changes end*/

/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_remove_eutran_cell_selection_entries 
 * Inputs         : p_ue_context_t
 * Outputs        : None
 * Returns        : None 
 * Description    : Delete all eutran entries in cell selection list except A3 
 ****************************************************************************/
rrm_void_t 
rrm_remove_eutran_cell_selection_entries
(
    rrm_ue_context_t *p_ue_context
)
{
    U32 cell_selection_index = RRM_ZERO;
    U32 cell_selection_next_index = RRM_ZERO;
    U32 cell_selection_count = RRM_ZERO;
    rrm_cell_context_t *p_cell_context = RRM_PNULL;
    /*SPR 17527 START*/
    U16                  pci         = RRM_ZERO;
    rf_params_t         *p_rf_params = RRM_PNULL; 
    U16                  earfcn      = RRM_ZERO;
    /*SPR 17527 END*/
    RRM_UT_TRACE_ENTER(); 

    p_cell_context = rrm_cellm_get_cell_context (p_ue_context->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        return;
    }

    /*SPR 17527 START*/
    p_rf_params = rrm_cellm_get_rf_params(p_ue_context->cell_index);
    cell_selection_count = p_ue_context->ho_params.eutra_unknown_cell_selection_list.count;

    for(cell_selection_index= RRM_ZERO;cell_selection_index<cell_selection_count;cell_selection_index++)
    {
        earfcn  = p_ue_context->ho_params.eutra_unknown_cell_selection_list.
            reported_neighbor_cell_info[cell_selection_index].target_earfcn;
        pci = p_ue_context->ho_params.eutra_unknown_cell_selection_list.
            reported_neighbor_cell_info[cell_selection_index].phy_cell_id; 

            if(p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                    reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A3_PRESENT)
            {
            if(p_rf_params->rf_configurations.dl_earfcn != earfcn)
            {
                rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci,earfcn);
            }
        }
        else if(p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A4_PRESENT)
        {
            rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci,earfcn);
        }
        else if(p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A5_PRESENT)
        {
            rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci,earfcn);
        }
    }
    cell_selection_count = p_ue_context->ho_params.eutra_confused_cell_selection_list.count;

    for(cell_selection_index= RRM_ZERO;cell_selection_index<cell_selection_count;cell_selection_index++)
    {
        earfcn  = p_ue_context->ho_params.eutra_confused_cell_selection_list.
            reported_neighbor_cell_info[cell_selection_index].target_earfcn;
        pci = p_ue_context->ho_params.eutra_confused_cell_selection_list.
            reported_neighbor_cell_info[cell_selection_index].phy_cell_id; 
            if(p_ue_context->ho_params.eutra_confused_cell_selection_list.
                    reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A3_PRESENT)
            {
            if(p_rf_params->rf_configurations.dl_earfcn != earfcn)
            {
                rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci,earfcn);
            }
        }
        else if(p_ue_context->ho_params.eutra_confused_cell_selection_list.
                reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A4_PRESENT)
        {
            rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci,earfcn);
        }
        else if(p_ue_context->ho_params.eutra_confused_cell_selection_list.
                reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A5_PRESENT)
        {
            rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci,earfcn);
        }

    }
    /*SPR 17527 END*/
    if(get_delayed_a3_configure(p_ue_context->cell_index))
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "A3 is delayed so restting all entries in eutran cell selection list");
        p_ue_context->ho_params.eutra_known_cell_selection_list.count=RRM_ZERO;
        p_ue_context->ho_params.eutra_known_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ue_context->ho_params.eutra_unknown_cell_selection_list.count=RRM_ZERO;
        p_ue_context->ho_params.eutra_unknown_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ue_context->ho_params.eutra_confused_cell_selection_list.count=RRM_ZERO;
        p_ue_context->ho_params.eutra_confused_cell_selection_list.next_target_cell_idx=RRM_ZERO;
    }
    else
    {    
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "A3 is not delayed");
        cell_selection_count = p_ue_context->ho_params.eutra_known_cell_selection_list.count;
        for(cell_selection_index= RRM_ZERO;cell_selection_index<cell_selection_count;cell_selection_index++)
        {
            if((p_ue_context->ho_params.eutra_known_cell_selection_list.
                        reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A3_PRESENT) &&
                    (p_ue_context->ho_params.eutra_known_cell_selection_list.
                     reported_neighbor_cell_info[cell_selection_index].target_earfcn ==
                     p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn ))

            {
                /* Here we are skipping A3 intra freq cells because A1 will not be exit criteria for them */
                continue;
            } 
            else
            {
                cell_selection_next_index = cell_selection_index + RRM_ONE;

                for (;cell_selection_next_index<(p_ue_context->ho_params.eutra_known_cell_selection_list.count) && (cell_selection_next_index < MAX_NEIGH_CELL);cell_selection_next_index++) 
                {
                    RRM_MEMCPY(&(p_ue_context->ho_params.eutra_known_cell_selection_list.reported_neighbor_cell_info[cell_selection_next_index - RRM_ONE]),
                            &(p_ue_context->ho_params.eutra_known_cell_selection_list.reported_neighbor_cell_info[cell_selection_next_index]),
                            sizeof(rrm_rpt_cell_info_t));
                }
                p_ue_context->ho_params.eutra_known_cell_selection_list.count--; 
                RRM_MEMSET(&(p_ue_context->ho_params.eutra_known_cell_selection_list.reported_neighbor_cell_info
                            [p_ue_context->ho_params.eutra_known_cell_selection_list.count]),
                        RRM_ZERO,sizeof(rrm_rpt_cell_info_t));
            }

        }

        cell_selection_count = p_ue_context->ho_params.eutra_unknown_cell_selection_list.count;
        for(cell_selection_index = RRM_ZERO;cell_selection_index <cell_selection_count;cell_selection_index++)
        {
	    /* SPR 18459 Fix Start */
            if((p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                    reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A3_PRESENT) &&
                    (p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                     reported_neighbor_cell_info[cell_selection_index].target_earfcn ==
                     p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn ))
	    /* SPR 18459 Fix End */

            {
                continue;
            } 
            else
            {
                cell_selection_next_index = cell_selection_index + RRM_ONE;
                for (;cell_selection_next_index <(p_ue_context->ho_params.eutra_unknown_cell_selection_list.count) && (cell_selection_next_index< MAX_NEIGH_CELL);cell_selection_next_index++) 
                {
                    RRM_MEMCPY(&(p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info[cell_selection_next_index- RRM_ONE]),
                            &(p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info[cell_selection_next_index]),
                            sizeof(rrm_rpt_cell_info_t));
                }
                p_ue_context->ho_params.eutra_unknown_cell_selection_list.count--; 
                RRM_MEMSET(&(p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info
                            [p_ue_context->ho_params.eutra_unknown_cell_selection_list.count]),
                        RRM_ZERO,sizeof(rrm_rpt_cell_info_t));
            }

        }

        cell_selection_count = p_ue_context->ho_params.eutra_confused_cell_selection_list.count;
        for(cell_selection_index = RRM_ZERO;cell_selection_index<cell_selection_count;cell_selection_index++)
        {
            /*SPR 17527 START*/
            if((p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                        reported_neighbor_cell_info[cell_selection_index].reason_event_type & MEAS_EVENT_A3_PRESENT) &&
                    (p_ue_context->ho_params.eutra_unknown_cell_selection_list.
                     reported_neighbor_cell_info[cell_selection_index].target_earfcn ==
                     p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn ))
                /*SPR 17527 END*/
            {
                continue;
            } 
            else
            {
                cell_selection_next_index = cell_selection_index + RRM_ONE;
                for (;cell_selection_next_index<(p_ue_context->ho_params.eutra_confused_cell_selection_list.count) && (cell_selection_next_index< MAX_NEIGH_CELL);cell_selection_next_index++) 
                {
                    RRM_MEMCPY(&(p_ue_context->ho_params.eutra_confused_cell_selection_list.
                                reported_neighbor_cell_info[cell_selection_next_index - RRM_ONE]),
                            &(p_ue_context->ho_params.eutra_confused_cell_selection_list.
                                reported_neighbor_cell_info[cell_selection_next_index]),
                            sizeof(rrm_rpt_cell_info_t));
                }
                p_ue_context->ho_params.eutra_confused_cell_selection_list.count--; 
                RRM_MEMSET(&(p_ue_context->ho_params.eutra_confused_cell_selection_list.reported_neighbor_cell_info
                            [p_ue_context->ho_params.eutra_confused_cell_selection_list.count]),
                        RRM_ZERO,sizeof(rrm_rpt_cell_info_t));
            }

        }
    }

    RRM_UT_TRACE_EXIT(); 
}
/* SPR 16406 End */



/* SPR 16406 3Aug Start */
/****************************************************************************
 * Function Name  : rrm_uem_validate_cgi_record_in_cell_selection_list 
 * Inputs         : p_ue_context
 * Outputs        : None
 * Returns        : RRM_TRUE/RRM_FALSE 
 * Description    : This function checks whether CGI triggered was for A3 event or not 
 ****************************************************************************/
rrm_bool_et
rrm_uem_validate_cgi_record_in_cell_selection_list
(
    rrm_ue_context_t *p_ue_context
)
{
    rrm_bool_et ret_val = RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    if(p_ue_context->ho_params.eutra_unknown_cell_selection_list.count)
    {
        if((p_ue_context->pci_for_which_cgi_required == 
            p_ue_context->ho_params.eutra_unknown_cell_selection_list.
            reported_neighbor_cell_info[RRM_ZERO].phy_cell_id)&&
            (p_ue_context->cgi_trigerred_for_earfcn == 
            p_ue_context->ho_params.eutra_unknown_cell_selection_list.
            reported_neighbor_cell_info[RRM_ZERO].target_earfcn))
        {
            ret_val = RRM_TRUE;
        }
    }
    if(p_ue_context->ho_params.eutra_confused_cell_selection_list.count)
    {
        if((p_ue_context->pci_for_which_cgi_required == 
            p_ue_context->ho_params.eutra_confused_cell_selection_list.
            reported_neighbor_cell_info[RRM_ZERO].phy_cell_id)&&
            (p_ue_context->cgi_trigerred_for_earfcn == 
            p_ue_context->ho_params.eutra_confused_cell_selection_list.
            reported_neighbor_cell_info[RRM_ZERO].target_earfcn))
        {
            ret_val = RRM_TRUE;
        }
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 16406 3Aug End */

/****************************************************************************
 * Function Name  : rrm_ue_handle_meas_event_a1
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the A1 meas  report message received from RRC.
 ****************************************************************************/
rrm_return_et
rrm_ue_handle_meas_event_a1 (
        rrm_ue_global_context_t *p_g_rrm_ue_ctx
)
{
    rrm_ue_context_t*            p_ue_context = RRM_PNULL;
    rrm_return_et                ret_val      = RRM_FAILURE;
    rrm_ho_params_t*             p_ho_params  = RRM_PNULL;
    rrm_cell_context_t*          p_cell_ctxt  = RRM_PNULL; 

    rrm_ue_measurment_results_ind_t*   p_proc_data = RRM_PNULL;
    rrm_cell_meas_data_t         serv_cell_data;

    U8                           transaction_id = RRM_ZERO;
    rrm_ue_procedure_record_t           *p_rrm_ue_procedure_record = RRM_PNULL;

    /* SPR:15970 Fix Start - 6/8/15 */
    rrm_bool_et is_ho_cancelled = RRM_TRUE;
    /* SPR 16406 3Aug Start */
    rrm_bool_et is_cgi_for_A3_intra = RRM_FALSE;
    /* SPR 16406 3Aug End */
    /* SPR:15970 Fix End - 6/8/15 */

    RRM_UT_TRACE_ENTER();

    if ((RRM_PNULL != p_g_rrm_ue_ctx) && 
            RRM_PNULL != p_g_rrm_ue_ctx->p_ue_context) {
        p_ue_context = p_g_rrm_ue_ctx->p_ue_context;
        transaction_id = p_g_rrm_ue_ctx->trans_id;

        p_ue_context = p_g_rrm_ue_ctx->p_ue_context;
        p_proc_data = (rrm_ue_measurment_results_ind_t *)
            (p_ue_context->current_procedure.p_proc_data);

        p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context->cell_index);
        /*CID 65793:start*/
        if (RRM_PNULL != p_cell_ctxt)
        {
            p_ho_params = (rrm_ho_params_t*)&p_ue_context->ho_params;
        /* SPR 16406 Start */
        rrm_remove_eutran_cell_selection_entries(p_ue_context);        

        p_ho_params->utra_known_cell_selection_list.count=RRM_ZERO;
        p_ho_params->utra_known_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ho_params->utra_unknown_cell_selection_list.count=RRM_ZERO;
        p_ho_params->utra_unknown_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ho_params->utra_confused_cell_selection_list.count=RRM_ZERO;
        p_ho_params->utra_confused_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ho_params->geran_known_cell_selection_list.count=RRM_ZERO;
        p_ho_params->geran_known_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ho_params->geran_unknown_cell_selection_list.count=RRM_ZERO;
        p_ho_params->geran_unknown_cell_selection_list.next_target_cell_idx=RRM_ZERO;
        p_ho_params->geran_confused_cell_selection_list.count=RRM_ZERO;
        p_ho_params->geran_confused_cell_selection_list.next_target_cell_idx=RRM_ZERO;
            /*Bug 656 Fix END */
        /* SPR 15232 start */
        p_ho_params->eutra_known_cell_selection_list.bitmask &=  ~(RRM_UE_BLIND_CELL_HO_TRIGGERED);
        p_ho_params->utra_known_cell_selection_list.bitmask &=  ~(RRM_UE_BLIND_CELL_HO_TRIGGERED);
        p_ho_params->geran_known_cell_selection_list.bitmask &=  ~(RRM_UE_BLIND_CELL_HO_TRIGGERED);
        /* SPR 16406 End */
        p_ho_params->ho_retry_count = RRM_ZERO;
        p_ho_params->a2_event_received = RRM_FALSE;
        /* SPR 15232 end */

        p_proc_data = (rrm_ue_measurment_results_ind_t *)
            (p_ue_context->current_procedure.p_proc_data);
        /* SPR 17600 Fix Start */
        /* Code Removed */
        /* SPR 17600 Fix Stop */

            RRM_MEMSET (&serv_cell_data,RRM_ZERO,sizeof(rrm_cell_meas_data_t));

            if (p_ue_context->trigger_quantity == RRM_RRC_RSRP ) {
                serv_cell_data.meas_result = \
                                             p_proc_data->meas_result_serv_cell.rsrp_result;
                serv_cell_data.thresh1 = \
                                         p_cell_ctxt->ran_info.\
                                         connected_mode_mobility_params.\
                                         common_params_for_eutra.a1_threshold_rsrp;
            } else {
                serv_cell_data.meas_result = \
                                             p_proc_data->meas_result_serv_cell.rsrq_result;
                serv_cell_data.thresh1 = \
                                         p_cell_ctxt->ran_info.\
                                         connected_mode_mobility_params.\
                                         common_params_for_eutra.a1_threshold_rsrq;
            }        
            serv_cell_data.hysteresis = p_cell_ctxt->ran_info.\
                                        connected_mode_mobility_params.\
                                        common_params_for_eutra.hysteresis;

            p_ue_context->ho_params.last_reported_meas_event = MEAS_REP_EVENT_A1;
        /*SPR_17893_START*/
        if(!(p_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD))
        {    
           p_ue_context->is_ho_reqd_on_recv_utan_cap = RRM_FALSE;
        }   
        /*SPR_17893_END*/
            /* UE strength is better cancel if any HO is ongoing */
            if (RRM_UE_STATE_HO_ONGOING == p_ue_context->ue_state) {
                if(p_ue_context->ho_params.bitmask & \
                        RRM_UE_CSFB_RECD)
                {
                    /* If CSFB is received then ignore the A1 report */
                    RRM_TRACE(g_uem_log_on_off,
                            p_g_rrm_uem_facility_name,
                            RRM_ERROR,
                            "CS-Fallback is on-going ignoring A1 report");
                    return RRM_FAILURE;
                }

            /* SPR:15970 Fix Start - 6/8/15 */
            if ((p_cell_ctxt->ran_info.rf_params.rf_configurations.dl_earfcn == p_ho_params->target_earfcn) && /* HO is Intra Freq and due to A3 event */
                (p_ho_params->last_ho_trigger_event == MEAS_REP_EVENT_A3)
               )
            {
                is_ho_cancelled = RRM_FALSE;
            }

            if (is_ho_cancelled == RRM_TRUE)
            {
                /* Cancel the previous HO Required message */
                ret_val = rrm_build_and_send_ho_cancel_req(
                        p_ue_context,
                        p_ue_context->ue_index,
                        transaction_id); 
                /* Force Lock / Grace lock changes Start*/
                if ( ret_val == RRM_SUCCESS )
                {
                    p_ue_context->ue_ho_current_state =  RRM_HO_STATE_NULL;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Changed the UE HO state to RRM_HO_STATE_NULL as HO_CANCEL sent \n");
                }
            }
            /* SPR 16406 3Aug Start */
            else
            {
                if((RRM_TRUE == p_ue_context->is_cgi_process_in_progress) &&
                        (p_ue_context->cdrx_status == RRM_UE_CDRX_APPLIED))
                {
                    is_cgi_for_A3_intra = rrm_uem_validate_cgi_record_in_cell_selection_list(p_ue_context);
                    if(RRM_TRUE == is_cgi_for_A3_intra)
                    {
                        /* No need to remove CDRX status as CGI was triggered for A3 intra frequency cell */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "No need to remove CDRX as CGI was triggered for A3 intra freq cell :"
                                "PCI[%u], Earfcn[%u]\n",
                                p_ue_context->pci_for_which_cgi_required,
                                p_ue_context->cgi_trigerred_for_earfcn);
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "removing CDRX as CGI was triggered earlier for Non A3 intra freq cell :"
                                "PCI[%u], Earfcn[%u]\n",
                                p_ue_context->pci_for_which_cgi_required,
                                p_ue_context->cgi_trigerred_for_earfcn);

                        rrm_uem_send_cdrx_removal(p_ue_context);
                    }
                }
            }
            /* SPR 16406 3Aug End */
            /* SPR:15970 Fix End - 6/8/15 */
            }

            /** if queue not empty then dequeue the events */
            if ((p_ue_context->current_procedure.state == RRM_UE_LIPA_SIPTO_HO_ONGOING) &&
                    rrm_get_queue_count(&p_ue_context->procedure_queue))
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, "[UE:%d], dequeue "
                        "LIPA_HO_REQUIRED message",p_ue_context->ue_index);
                do
                {
                    /** Dequeue the events from event queue and call message handler */
                /* SPR 15849 Start */
                p_rrm_ue_procedure_record = rrm_request_dequeue(p_ue_context);
                /* SPR 15849 End */
                    if(RRM_PNULL != p_rrm_ue_procedure_record)
                    {    
                        if(RRC_RRM_HO_REQUIRED ==
                                p_rrm_ue_procedure_record->proc_queue.api_id)
                        {    
                            RRM_MEM_FREE(p_rrm_ue_procedure_record->proc_queue.p_proc_data);
                            p_rrm_ue_procedure_record->proc_queue.p_proc_data = RRM_PNULL;
                            RRM_MEM_FREE(p_rrm_ue_procedure_record);
                        p_rrm_ue_procedure_record = RRM_PNULL;
                            break;
                        }
                        else
                        {
                            /**Enqueue the erab setup req data */
                            rrm_request_enqueue(&p_ue_context->procedure_queue,
                                    p_rrm_ue_procedure_record);
                        }
                    }
                }while((RRM_PNULL != p_rrm_ue_procedure_record)/* Coverity_ID : 53573 */ && 
                        (RRC_RRM_HO_REQUIRED != p_rrm_ue_procedure_record->proc_queue.api_id) && \
                        rrm_get_queue_count(&p_ue_context->procedure_queue));
            }

            /* Should we need to clean up any resources */
            /* Updation of the UE context table for the neighbors */
        /* SPR-17852 START */
        if((p_ho_params->blind_timer)) {
        /* SPR-17852 END */
            rrm_ue_stop_timer (p_ho_params->blind_timer);
            p_ho_params->blind_timer = RRM_PNULL;
        }
            /* SPR 16041 start */
        /* Spr 18748 Fix Start */    
        if(RRM_TRUE == rrm_check_if_sec_meas_configured(p_ue_context))
            {
        /* Spr 18748 Fix End */    
                /* SPR 16041 end */
                ret_val = rrm_build_and_send_meas_config_req(p_ue_context, 
                        MEAS_TYPE_HO);
                RRM_TRACE(g_uem_log_on_off,
                        p_g_rrm_uem_facility_name,
                        RRM_BRIEF,
                        "rrm_build_and_send_meas_config_req :%s",
                        ret_val? "SUCCESS":"FAILURE");
                /* SPR 16041 start */
            }
            /* SPR 16041 end */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off,
                    p_g_rrm_uem_facility_name,
                    RRM_ERROR,
                    "Cell context[%d] not found", p_ue_context->cell_index);
        }
    }
        /*CID 65793:end*/

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_ue_handle_meas_event_a2
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the A2 meas  report message received from RRC.
 ****************************************************************************/
rrm_return_et
rrm_ue_handle_meas_event_a2 (
        rrm_ue_global_context_t *p_g_rrm_ue_ctx
)
{
    rrm_ue_context_t*            p_ue_context = RRM_PNULL;
    rrm_return_et                ret_val      = RRM_FAILURE;

	/* SPR 16406 21AUG Start */
	/* Code removed */
	/* SPR 16406 21AUG End */
    //rrm_ue_measurment_results_ind_t*   p_proc_data = RRM_PNULL;
    //rrm_common_params_for_eutra_t *p_common_params_for_eutra  = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    if ((RRM_PNULL != p_g_rrm_ue_ctx) && 
            RRM_PNULL != p_g_rrm_ue_ctx->p_ue_context) {
        p_ue_context = p_g_rrm_ue_ctx->p_ue_context;
        	/* SPR 16406 21AUG Start */
	/* Code removed */
	/* SPR 16406 21AUG End */
    /* SPR 21251 +- */
        /*p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                (p_ue_context->cell_index);*/
      
	/* SPR 16406 21AUG Start */
	/* Code removed */
	/* SPR 16406 21AUG End */
    /* SPR 21251 +- */
       /* p_proc_data = (rrm_ue_measurment_results_ind_t *)
            p_g_rrm_ue_ctx->p_ue_context->current_procedure.p_proc_data;*/
    /* SPR 21251 +- */
		p_ue_context->ho_params.last_reported_meas_event = MEAS_REP_EVENT_A2;
        /* SPR 15232 start */
        p_ue_context->ho_params.a2_event_received = RRM_TRUE;
        /* SPR 15232 end */

        /* SPR 16041 start */
        /*SPR_17893_START*/
        if ((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state) || (RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES == p_ue_context->ue_state))
            {
        /*SPR_17893_END*/
            /* Spr 18748 Fix Start */
            if(RRM_FALSE == rrm_check_if_sec_meas_configured(p_ue_context))
            /* Spr 18748 Fix End */
                {
            /* SPR 16041 end */
                    /* Start the blind timer if the same was not started */
                    /* Trigger Measurement configuration for A3, A4 for intra frequency  */
                    /* Trigger Measurement configuration for A5 for inter frequency  */
                    ret_val = rrm_build_and_send_meas_config_req(
                            p_ue_context,MEAS_TYPE_HO);
                    RRM_TRACE(g_uem_log_on_off,
                            p_g_rrm_uem_facility_name,
                            RRM_BRIEF,
                            "rrm_build_and_send_meas_config_req:%s",
                            ret_val?"SUCCESS":"FAILED");

                /* SPR 16406 21AUG Start */
                    /* SPR 16041 start */
                if (RRM_FAILURE == ret_val)
                    {
                        p_ue_context->a2_trigger_quantity_bitmask = RRM_ZERO;
                    }
                    /* SPR 16041 end */
                /* SPR 16406 21AUG End */
                }
            }
            /* SPR 16041 start */
            else
            {
                p_ue_context->a2_trigger_quantity_bitmask = RRM_ZERO;
                RRM_TRACE(g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_BRIEF,
                        "Ignoring A2 as [UE:%u] is in state [%u]",
                        p_ue_context->ue_index,
                        p_ue_context->ue_state);
            }
            /* SPR 16041 end */
        }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/*start LTE RRM Mobility-13*/

/* SPR 15674 Start */
/* code removed */
/* SPR 15674 End */
/* SPR 15674 Start */
/****************************************************************************
 * Function Name  : rrm_match_with_utran_daho_pci
 * Inputs         : reported utran PCI in meas report
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Returns fail if reported utran PCI is same as
 *                  utran DAHO PCI
 ****************************************************************************/
    rrm_return_et
rrm_match_with_utran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt,
        U16 pci_reported_utran
        )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    U16 daho_freq = RRM_ZERO;
    U16 daho_pci    = INVALID_PHYSICAL_CELL_ID;
    rrm_daho_cell_config_info_t*    p_daho_cfg =
        rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);
    if((p_daho_cfg != RRM_PNULL) &&
            /*daho clean up(condition added for presence of daho cell)*/
            (p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT))
    {
        daho_pci = p_daho_cfg->utran_daho_cell_config_info.pci;
        daho_freq = p_daho_cfg->utran_daho_cell_config_info.uarfcndl;
        if((daho_pci == pci_reported_utran) &&
                daho_freq == p_ue_ctxt->carrier_earfcn)
        {
            /* Bug 9800 Fix Start */
            RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                    "DAHO PCI = %d reported", pci_reported_utran);		
            /* Bug 9800 Fix End */
            ret_val = RRM_FAILURE;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 15674 End */
/* SPR 15674 Start*/
/****************************************************************************
 * Function Name  : rrm_match_with_eutran_daho_pci
 * Inputs         : reported eutran PCI in meas report 
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Returns fail if reported eutran PCI is same as 
 *                  eutran DAHO PCI
 ****************************************************************************/
    rrm_return_et
rrm_match_with_eutran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt,
        U16 reported_eutran_pci,
        U16 reported_eutran_freq
        )
{
    RRM_UT_TRACE_ENTER();
    rrm_return_et ret_val = RRM_SUCCESS;
    U16 daho_pci    = INVALID_PHYSICAL_CELL_ID;
    U16 daho_freq    = RRM_ZERO;
    rrm_daho_cell_config_info_t*    p_daho_cfg =
        rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);
    rrm_cell_context_t *p_cell_context = 
        rrm_cellm_get_cell_context (p_ue_ctxt->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        return RRM_FAILURE;
    }
    if((p_daho_cfg != RRM_PNULL) &&
            /*daho clean up(condition added for presence of daho cell)*/
            (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT))
    {
        daho_pci = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
        daho_freq = p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn;
        if(daho_pci == reported_eutran_pci && 
                reported_eutran_freq == daho_freq)
        {
            ret_val = RRM_FAILURE;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 15674 End */

/* CSR_97730_fix: start */
/****************************************************************************
 * Function Name  : rrm_config_ue_for_cgi_reporting_for_new_detected_pci
 * Inputs         : p_ue_context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Configures UE for CGI reporting if new PCI reported in measurement results.
 ****************************************************************************/
/* SPR_15259_fix: start */
/* SPR 16406 Start */
    rrm_return_et
rrm_config_ue_for_cgi_reporting_for_new_detected_pci(
        rrm_ue_context_t            *p_ue_context,
        rrm_ue_rat_et               rrm_cell_rat_type,
        rrm_cgi_trigger_cause_et    cgi_trigger_cause
        )
    /* SPR 16406 End */
    /* SPR_15259_fix: end */
{
    rrm_return_et         ret_val = RRM_SUCCESS;
    /* SPR 16053 start */
    /* Code deleted */
    /* SPR 16053 end */

    RRM_UT_TRACE_ENTER();

    /* SPR 16053 start */
    /* Code deleted */
    /* SPR 16053 end */
    /* SPR_15259_fix: start */
    if(RRM_TRUE == rrm_is_ue_supporting_autonomous_gap_for_ho(p_ue_context, rrm_cell_rat_type))
        /* SPR_15259_fix: end */
    {
        /* SPR_14564_fix: start */
        if (RRM_SUCCESS == rrm_build_and_send_meas_config_req_for_cgi(
                    p_ue_context,
                    /*SPR 17777 +-*/
                    rrm_cell_rat_type))
            /* SPR_14564_fix: end */
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "Meas config sent to RRC");

             /* SPR 16406 Start */
            /* SPR_15282_Fix_Start */
           
            p_ue_context->cgi_reporting_triggered_for_meas_event = cgi_trigger_cause;
            /* SPR 16406 End */
            /* SPR_15282_Fix_End */
            /* SPR 16053 start */
            /* Code deleted */
            /* SPR 16053 end */
        }
        else
        {
            /* SPR 16406 8SEP start */
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "Meas config request for CGI cannot be sent to RRC");
            /* SPR 16406 8SEP end */
            /* SPR 16406 8SEP start */
            /* Code removed */
            /* SPR 16406 8SEP end */
            ret_val = RRM_FAILURE;
            /*No need to update the phy_cell_id, because normal HO will process in this case*/

        }
    }
    else
    {
        /* SPR_15771_Fix: Start */
        /* In case CDRX already not configured */
        if((p_ue_context->cdrx_status == RRM_UE_CDRX_DISABLED) ||
           (p_ue_context->cdrx_status == RRM_UE_CDRX_TO_BE_APPLIED))
        {
	    /* In case CDRX is getting configured for the first time */
            p_ue_context->cdrx_status = RRM_UE_CDRX_TO_BE_APPLIED;

            if (RRM_SUCCESS == rrm_build_and_send_ue_reconfig_req(p_ue_context))
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "UE reconfig request sent to [UE:%d]",
                        p_ue_context->ue_index);

                p_ue_context->is_cgi_process_in_progress = RRM_TRUE;
                /* SPR 16406 Start */
                p_ue_context->cgi_reporting_triggered_for_meas_event = cgi_trigger_cause;
                /* SPR 16406 End */
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "Failed to send UE reconfig request, [UE:%d]",
                        p_ue_context->ue_index);

                p_ue_context->pci_for_which_cgi_required = INVALID_PHYSICAL_CELL_ID;
                ret_val = RRM_FAILURE;
                /*No need to update the phy_cell_id, because normal HO will process in this case*/
            }
        }
        /* In case CDRX already configured */
        /* SPR 15843 Start */
        /// Check to ensure that CGI is only configured if SRX is Applied
        else if(p_ue_context->cdrx_status == RRM_UE_CDRX_APPLIED)
        {
        /* SPR 15843 End */
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,RRM_DETAILED,
                    "CDRX already configured for CGI reporting.");
            /* SPR_14564_fix: start */
            if ((RRM_SUCCESS == rrm_build_and_send_meas_config_req_for_cgi(
                            p_ue_context,
                            /*SPR 17777 +-*/
                            rrm_cell_rat_type)))
            /* SPR_14564_fix: end */
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Meas config sent to RRC");
                /* SPR 16053 start */
                /* Code deleted */
                /* SPR 16053 end */
              /* SPR 16406 Start */
                p_ue_context->cgi_reporting_triggered_for_meas_event = cgi_trigger_cause;
                /* SPR 16406 End */
                /* SPR 16053 start */
                /* Code deleted */
                /* SPR 16053 end */
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED,
                        "Meas config request not sent to RRC");

                /*SPR_15852_FIX:Start*/
                /*If CDRX is applied on UE and no cgi report is pending release the CDRX on UE*/
                if (RRM_FALSE == rrm_ue_is_cgi_report_pending(p_ue_context))
                {
                    p_ue_context->cdrx_status = RRM_UE_CDRX_TO_BE_REMOVED;
                    if (RRM_FAILURE == rrm_build_and_send_ue_reconfig_req(p_ue_context))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                                "Failed to send UE reconfig request to remove CDRX, [UE:%d]",
                                p_ue_context->ue_index);
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                "UE reconfig request sent to [UE:%d] to remove CDRX ",
                                p_ue_context->ue_index);
                    }
                }
                /* SPR 16406 8SEP start */
                /* Code removed */
                /* SPR 16406 8SEP end */
                ret_val = RRM_FAILURE;
            }
	    /* SPR_15771_Fix: End */
        }
        /* SPR 16068 start */
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                    "CGI cannot be triggered as DRX is in remove state for [UE:%d]",
                    p_ue_context->ue_index);
            p_ue_context->pci_for_which_cgi_required = INVALID_PHYSICAL_CELL_ID;
            /* SPR 15674 Start */
            p_ue_context->cgi_trigerred_for_earfcn = RRM_ZERO;
            /* SPR 15674 End */
            ret_val = RRM_FAILURE;
        }
        /* SPR 16068 end */
    }

    /* SPR 16053 start */
    /* Code deleted */
    /* SPR 16053 end */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* CSR_97730_fix: end */
/* SPR 15232 start */
S32 rrm_q_offset_map(rrm_q_offsetrange_et offset)
{
    S32 ret_val = RRM_ZERO;

    switch (offset)
    {
        case RRM_Q_OFFSET_RANGE_DB_24:
            ret_val = -24;
            break;

        case RRM_Q_OFFSET_RANGE_DB_22:
            ret_val = -22;
            break;

        case RRM_Q_OFFSET_RANGE_DB_20:
            ret_val = -20;
            break;

        case RRM_Q_OFFSET_RANGE_DB_18:
            ret_val = -18;
            break;

        case RRM_Q_OFFSET_RANGE_DB_16:
            ret_val = -16;
            break;

        case RRM_Q_OFFSET_RANGE_DB_14:
            ret_val = -14;
            break;

        case RRM_Q_OFFSET_RANGE_DB_12:
            ret_val = -12;
            break;

        case RRM_Q_OFFSET_RANGE_DB_10:
            ret_val = -10;
            break;

        case RRM_Q_OFFSET_RANGE_DB_8:
            ret_val = -8;
            break;

        case RRM_Q_OFFSET_RANGE_DB_6:
            ret_val = -6;
            break;

        case RRM_Q_OFFSET_RANGE_DB_5:
            ret_val = -5;
            break;

        case RRM_Q_OFFSET_RANGE_DB_4:
            ret_val = -4;
            break;

        case RRM_Q_OFFSET_RANGE_DB_3:
            ret_val = -3;
            break;

        case RRM_Q_OFFSET_RANGE_DB_2:
            ret_val = -2;
            break;

        case RRM_Q_OFFSET_RANGE_DB_1:
            ret_val = -1;
            break;

        case RRM_Q_OFFSET_RANGE_DB0:
            ret_val = 0;
            break;

        case RRM_Q_OFFSET_RANGE_DB1:
            ret_val = 1;
            break;

        case RRM_Q_OFFSET_RANGE_DB2:
            ret_val = 2;
            break;

        case RRM_Q_OFFSET_RANGE_DB3:
            ret_val = 3;
            break;

        case RRM_Q_OFFSET_RANGE_DB4:
            ret_val = 4;
            break;

        case RRM_Q_OFFSET_RANGE_DB5:
            ret_val = 5;
            break;

        case RRM_Q_OFFSET_RANGE_DB6:
            ret_val = 6;
            break;

        case RRM_Q_OFFSET_RANGE_DB8:
            ret_val = 7;
            break;

        case RRM_Q_OFFSET_RANGE_DB10:
            ret_val = 10;
            break;

        case RRM_Q_OFFSET_RANGE_DB12:
            ret_val = 12;
            break;

        case RRM_Q_OFFSET_RANGE_DB14:
            ret_val = 14;
            break;

        case RRM_Q_OFFSET_RANGE_DB16:
            ret_val = 16;
            break;

        case RRM_Q_OFFSET_RANGE_DB18:
            ret_val = 18;
            break;

        case RRM_Q_OFFSET_RANGE_DB20:
            ret_val = 20;
            break;

        case RRM_Q_OFFSET_RANGE_DB22:
            ret_val = 22;
            break;

        case RRM_Q_OFFSET_RANGE_DB24:
            ret_val = 24;
            break;

        default:
            break;
    }

    return ret_val;
}
/* SPR 15232 end */
/*end LTE RRM Mobility-13*/

/* SPR:15970 Fix Start */
/* Code Removed - Function rrm_check_enter_exit_a3_condition() */
/* SPR:15970 Fix End */


/* SPR:15970 Fix Start - 6/8/15 */
/* Function moved upward in this file just for compilation purpose */
/*SPR 15792 Fix Start */
/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_remove_pci_from_cell_selection_list
 * Inputs         : p_ue_context, pci_for_which_cgi_required, carrier_earfcn 
 * Outputs        : 
 * Returns        : void 
 * Description    : Deletes the PCI from the cell selection list
 ****************************************************************************/
rrm_void_t rrm_remove_pci_from_cell_selection_list(

        rrm_ue_context_t *p_ue_context,
        cell_selection_priority_list_t *p_cell_selection,
        U16               pci_for_which_cgi_required,
        U16               carrier_earfcn
    )
{
	rrm_bool_et                         is_node_to_be_removed_found = RRM_FALSE;
	U32                                 i = RRM_ZERO;

	RRM_UT_TRACE_ENTER();


	for (i = RRM_ZERO;i<(p_cell_selection->count);i++) 
	{
		if (p_cell_selection->reported_neighbor_cell_info[i].phy_cell_id == pci_for_which_cgi_required
				&& p_cell_selection->reported_neighbor_cell_info[i].target_earfcn == carrier_earfcn)
		{
			RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
					RRM_DETAILED,
					"Physical Cell (%d) and Carrier(%d) Freq Matched on list with count size(%d) ",
					pci_for_which_cgi_required,carrier_earfcn,(p_cell_selection->count));

            rrm_delete_entry_in_ho_cgi_pending_list(p_ue_context, pci_for_which_cgi_required,carrier_earfcn);    
           
            /* SPR 16406 18sep Start */
                        /* SPR 17660 Fix Start */
                        /* Code Deleted */
			is_node_to_be_removed_found = RRM_TRUE; 
                        /* Code Deleted */
                        /* SPR 17660 Fix End */
            /* SPR 16406 18sep End */
			break;
		}
	}

	if(RRM_TRUE == is_node_to_be_removed_found)
	{
		U32 j = i + RRM_ONE;

		/* SPR:15970 Fix Start - 6/8/15 */
		for (;j<(p_cell_selection->count) && (j < MAX_NEIGH_CELL);j++) 
		{
			RRM_MEMCPY(&p_cell_selection->reported_neighbor_cell_info[j - RRM_ONE],
					&p_cell_selection->reported_neighbor_cell_info[j],
					sizeof(rrm_rpt_cell_info_t));
		}
		p_cell_selection->count--; 
		RRM_MEMSET(&p_cell_selection->reported_neighbor_cell_info[p_cell_selection->count],
				RRM_ZERO,sizeof(rrm_rpt_cell_info_t));
		/* SPR:15970 Fix End - 6/8/15 */
        /* SPR 16406 8SEP start */
        if (RRM_ZERO != p_cell_selection->next_target_cell_idx)
        {
            /* Cell at this index has shifted to previous index because of deletion */
            p_cell_selection->next_target_cell_idx = p_cell_selection->next_target_cell_idx - RRM_ONE;
        }
        /* SPR 16406 8SEP end */
	}

	RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
			RRM_DETAILED,
			"After Deletion cell selection list count = [%d] ",
			(p_cell_selection->count));

	RRM_UT_TRACE_EXIT();
	return;
}
/* SPR 16406 End */
/*SPR 15792 Fix Stop */
/* SPR:15970 Fix End - 6/8/15 */

/* SPR:15970 Fix Start - 6/8/15 */
/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : updateDeletedPciAfterExitInMr
 * Inputs         : rrm_ue_context_t* p_ue_context - UE Global Context
                    rrm_meas_report_event_et event_type - Enum of Meas Event Type
 * Outputs        : None
 * Returns        : None
 * Description    : Delete PCI for those Exit Condition Meet in latest MR
 ****************************************************************************/
rrm_void_t updateDeletedPciAfterExitInMr
(
 rrm_ue_context_t* p_ue_context, 
 U8 event_type, 
 cell_selection_priority_list_t *p_cell_selection
 )
{
    rrm_ue_measurment_results_ind_t*    p_proc_data = RRM_PNULL;
    meas_report_info_node_t*            p_meas_report_info = RRM_PNULL;
    YLNODE *    p_node = RRM_PNULL;
    U8      idx1 = RRM_ZERO;
    U8      idx2 = RRM_ZERO;
    U8      is_pci_found = RRM_FALSE;
    U8      is_meas_id_found = RRM_FALSE;
    U16     earfcn_of_received_mr = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    

    p_proc_data = (rrm_ue_measurment_results_ind_t *)
        p_ue_context->current_procedure.p_proc_data;

    if (p_cell_selection == RRM_PNULL || p_proc_data == RRM_PNULL)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED, "NULL Pointer Detected");
        RRM_UT_TRACE_EXIT();
        return;
    }

    /* Get Earfcn corresponding to the MeadId recevied in MR */
    p_node = ylFirst(&(p_ue_context->meas_report_list));

    while (p_node)
    {
        p_meas_report_info = (meas_report_info_node_t*) p_node;
        if (p_meas_report_info->meas_id == p_proc_data->meas_id) 
        {
            earfcn_of_received_mr = p_meas_report_info->carrier_earfcn;
            is_meas_id_found = RRM_TRUE;
            break;
        }
        p_node = ylNext(p_node);
    }

    if (is_meas_id_found == RRM_FALSE) /* If MeasId Not Configured then no need to handle any deletion. Raise Condition */
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Measurement Id:%d Not Configured", p_proc_data->meas_id);
        return;
    }

    /* Search for every Cell Selection List for the received event type.
     * If corresponding PCI+EARFCN not part of the latest received MR, We consider that PCI for Exit Condition.
     * And delete Exit Condition PCI for corresponding event type from Cell Selection List. 
     */


    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "MeasId:%d, Earfcn:%d, EventType:%d "
            "MR Eutran Count:%d, Cell Selection Count=%d",
            p_proc_data->meas_id, earfcn_of_received_mr, event_type,
            p_proc_data->eutran_count, p_cell_selection->count);

    
    while(idx1 < p_cell_selection->count)
    {
        is_pci_found = RRM_FALSE;
        if (p_cell_selection->reported_neighbor_cell_info[idx1].reason_event_type & event_type)
        {
/* coverity_87536 start */
            for (idx2 = 0; idx2 < p_proc_data->eutran_count && idx2 < RRM_MAX_CELLS_REPORTED ; idx2++)
/* coverity_87536 stop */
            {
                if ((p_proc_data->eutran_neighbor_list[idx2].pci == 
                            p_cell_selection->reported_neighbor_cell_info[idx1].phy_cell_id) &&
                        (earfcn_of_received_mr == 
                         p_cell_selection->reported_neighbor_cell_info[idx1].target_earfcn)
                   )
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "PCI:%d Received in MR Present in Cell Selection Count", 
                            p_proc_data->eutran_neighbor_list[idx2].pci);

                    is_pci_found = RRM_TRUE;
                    break;
                }
            }

            if (is_pci_found == RRM_FALSE)
            {
                p_cell_selection->reported_neighbor_cell_info[idx1].reason_event_type ^= event_type;

                if (p_cell_selection->reported_neighbor_cell_info[idx1].reason_event_type == MEAS_EVENT_NONE_PRESENT)
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "PCI:%d is Not Present in Cell Selection Count. Deletion Initiated", 
                            p_cell_selection->reported_neighbor_cell_info[idx1].phy_cell_id);

                    rrm_remove_pci_from_cell_selection_list(p_ue_context, 
                            p_cell_selection,
                            p_cell_selection->reported_neighbor_cell_info[idx1].phy_cell_id, 
                            earfcn_of_received_mr);

                    /*We don't need to increase the idx1 by 1 here since an item is 
                      shifted from idx1+1 to idx1 so no need to increase the 
                      index in this case */
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "PCI:%d Received in MR Not Present in Cell Selection Count, Event Type:%d", 
                            p_cell_selection->reported_neighbor_cell_info[idx1].phy_cell_id, 
                            p_cell_selection->reported_neighbor_cell_info[idx1].reason_event_type);

                    idx1++;
                }
            }
            else
            {  
                idx1++;
            }
        }
        else
        {
            idx1++;
        }
    }

    RRM_UT_TRACE_EXIT();
    return;
}
/* SPR 16406 End */
/* SPR:15970 Fix End - 6/8/15 */

/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_uem_get_event_type 
 * Inputs         : report_event 
 * Outputs        : NONE
 * Returns        : event_type 
 * Description    : returns the event type from given report_event 
 ****************************************************************************/
U8
rrm_uem_get_event_type
(
    rrmc_meas_report_event_et report_event
)
{
    U8 event_type = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    switch(report_event)
    {
        case MEAS_REP_EVENT_A3:
            event_type = MEAS_EVENT_A3_PRESENT; 
            break;

        case MEAS_REP_EVENT_A4:
            event_type = MEAS_EVENT_A4_PRESENT; 
            break;

        case MEAS_REP_EVENT_A5:
            event_type = MEAS_EVENT_A5_PRESENT; 
            break;

        default:
            event_type = MEAS_EVENT_NONE_PRESENT; 
    }
    RRM_UT_TRACE_EXIT();
    return event_type;
}

/****************************************************************************
 * Function Name  : is_pci_present_in_cell_selection_list 
 * Inputs         : cell_selection_priority_list_t cell_selection_list,
 *                  phy_cell_is, Earfcn
 * Outputs        : NONE
 * Returns        : RRM_TRUE/RRM_FALSE 
 * Description    : Checks if given PCI+Earfcn combination is present 
 *                  in cell selection list or not 
 ****************************************************************************/
rrm_bool_et
is_pci_present_in_cell_selection_list
(
 /*Cov 88197 +*/
    cell_selection_priority_list_t *p_cell_selection_list,
    U16 phy_cell_id,
    U16 Earfcn
)
{
    rrm_bool_et is_pci_present = RRM_FALSE;
    rrm_counter_t index = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    if(p_cell_selection_list->count)
    {
        for(index = RRM_ZERO; index< p_cell_selection_list->count; index++)
        {
            if((phy_cell_id == p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) 
                    && (Earfcn == p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
                /*Cov 88197 -*/
            {
                RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_DETAILED, "PCI[%u] , Earfcn[%u] is present in cell selection list",
                        phy_cell_id, Earfcn);
                is_pci_present = RRM_TRUE;
            }
        }
    }

    RRM_UT_TRACE_EXIT();
    return is_pci_present;
}
/****************************************************************************
 * Function Name  : rrm_uem_handle_intraRAT_meas_event 
 * Inputs         : rrm_global_context_t *p_rrm_glb_ctxt RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the A3 meas  report message received from RRC.
 ****************************************************************************/
    rrm_return_et
rrm_uem_handle_intraRAT_meas_event(
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        )
{
    rrm_ue_context_t*            p_ue_context = RRM_PNULL;
    rrm_return_et                ret_val      = RRM_FAILURE;
    rrm_ho_params_t*             p_ho_params  = RRM_PNULL;
    rrm_cell_context_t*          p_cell_ctxt  = RRM_PNULL; 
    U16                          num_eutra_ngbrs = RRM_ZERO;
    U8                           i               = RRM_ZERO;
    rrm_bool_et                  pci_confusion_detected = RRM_FALSE;
	/* SPR 16406 3Aug Start */
    rrm_bool_et                  unknown_pci_detected = RRM_FALSE;
/* SPR 16406 3Aug End  */
    rrm_bool_et                  pci_blacklisted = RRM_FALSE;
    /*SPR 16596 START*/
    rrm_bool_et is_cell_block_triggered = RRM_FALSE;
    /*SPR 16596 END*/
    rrm_bool_et is_ho_cancelled = RRM_TRUE;

    rrm_ue_measurment_results_ind_t*   p_proc_data = RRM_PNULL;
    rrm_cell_meas_data_t           serv_cell_data;
    rrm_cell_meas_data_t           tgt_cell_data;
    U8                             transaction_id = RRM_ZERO;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;
    rrm_best_cell_for_ue_node_t *p_best_cell    = RRM_NULL;

    RRM_UT_TRACE_ENTER();

    if ((RRM_PNULL != p_rrm_glb_ctxt) && 
            RRM_PNULL != p_rrm_glb_ctxt->p_ue_context) {

        p_ue_context = p_rrm_glb_ctxt->p_ue_context;
		/* SPR 16406 8SEP start */
		p_ue_context->ho_params.last_reported_meas_event =
            (rrmc_meas_report_event_et)p_ue_context->meas_rep_event;/*Cov 93546 +-*/
		/* SPR 16406 8SEP end */

        if (RRM_CARRIER_REDIRECT_ON_UE == p_rrm_glb_ctxt->p_ue_context->action_triggered)
        {
            RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_INFO,
                    "Previous action on UE was = %d, new action = %d",
                    p_rrm_glb_ctxt->p_ue_context->action_triggered,
                    RRM_HO_ACTN_ON_UE);
            p_rrm_glb_ctxt->p_ue_context->action_triggered = RRM_HO_ACTN_ON_UE;
            /* SPR-17852 START */
            if(p_ue_context->carrier_redirect_ue)
            {
                /* SPR-17852 END */
                RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                        RRM_BRIEF, "Stop carrier_redirect_ue timer");
                rrm_ue_stop_timer(p_ue_context->carrier_redirect_ue);
                p_ue_context->carrier_redirect_ue = RRM_PNULL;
                while(RRM_PNULL != (p_best_cell = (rrm_best_cell_for_ue_node_t *)ylPopHead(&p_ue_context->best_cell_for_ue)))
                {
                    RRM_MEM_FREE(p_best_cell);
                }
            }
        }

        transaction_id = p_rrm_glb_ctxt->trans_id;
        p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context->cell_index);
        /*Coverity 94842 Fix Start*/
        if(RRM_PNULL == p_cell_ctxt) 
        {
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,RRM_WARNING,
                    "Cell Context not found");
            RRM_UT_TRACE_EXIT();
            return RRM_FAILURE;
        }
        /*Coverity 94842 Fix End*/
        p_ho_params = (rrm_ho_params_t*)&p_ue_context->ho_params;

                /* SPR 16406 24Sep Start */
                /* Code removed */
                /* SPR 16406 24Sep End */

        p_proc_data = (rrm_ue_measurment_results_ind_t *)
            p_rrm_glb_ctxt->p_ue_context->current_procedure.p_proc_data;
        num_eutra_ngbrs = p_proc_data->eutran_count;
        RRM_MEMSET (&serv_cell_data,RRM_ZERO,sizeof(rrm_cell_meas_data_t));

        if (p_ue_context->trigger_quantity == RRM_RRC_RSRP ) {
            serv_cell_data.meas_result = \
                                         p_proc_data->meas_result_serv_cell.rsrp_result;
        }
        else {
            serv_cell_data.meas_result = \
                                         p_proc_data->meas_result_serv_cell.rsrq_result;

        }
        serv_cell_data.offset = \
                                p_cell_ctxt->ran_info.\
                                connected_mode_mobility_params.\
                                common_params_for_eutra.a3_offset;

        serv_cell_data.hysteresis = p_cell_ctxt->ran_info.\
                                    connected_mode_mobility_params.\
                                    common_params_for_eutra.hysteresis;
        serv_cell_data.freq_specific_offset = RRM_Q_OFFSET_RANGE_DB0;
        serv_cell_data.cell_specific_offset = RRM_Q_OFFSET_RANGE_DB0;

        /* calculate event_type from p_ue_context->report_event */
		/* SPR 16406 8SEP start */
		U8 event_type =
            rrm_uem_get_event_type((rrmc_meas_report_event_et)p_ue_context->meas_rep_event);/*Cov 93547 +-*/
		/* SPR 16406 8SEP end */

        updateDeletedPciAfterExitInMr(p_ue_context, event_type, &(p_ue_context->ho_params.eutra_known_cell_selection_list));
        updateDeletedPciAfterExitInMr(p_ue_context, event_type, &(p_ue_context->ho_params.eutra_unknown_cell_selection_list));
        updateDeletedPciAfterExitInMr(p_ue_context, event_type, &(p_ue_context->ho_params.eutra_confused_cell_selection_list));

        /*SPR 16364 START*/
        /*SPR 16596 START*/
        if (RRM_PNULL != g_uem_gbl_proc.rrm_timer_id[RRM_UE_CELL_BLOCKED_W_FOR_A4_REPORT_TIMER])
        {
            if (RRM_FALSE != qvTimerRunning(g_uem_gbl_proc.rrm_timer_id[RRM_UE_CELL_BLOCKED_W_FOR_A4_REPORT_TIMER]))
            {	    
                is_cell_block_triggered =  RRM_TRUE;
            }
        }
        else
        {

            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "Couldn't stop RRM_UE_CELL_BLOCKED_W_FOR_A4_REPORT_TIMER timer because g_uem_gbl_proc.rrm_timer_id is NULL");
        }
        /*SPR 16596 END*/
        /*SPR 16364 END*/
        /*SPR 16596 START*/
		/* SPR 16406 8SEP start */
        if ((num_eutra_ngbrs) && ( 
					((MEAS_REP_EVENT_A4 == p_ue_context->meas_rep_event) 
                     && ((p_cell_ctxt->ran_info.rf_params.rf_configurations.dl_earfcn 
                         == p_ue_context->carrier_earfcn)||(is_cell_block_triggered == RRM_TRUE)))
					|| (p_ue_context->meas_rep_event == MEAS_REP_EVENT_A3)
					|| (p_ue_context->meas_rep_event == MEAS_REP_EVENT_A5))
           )
		/* SPR 16406 8SEP end */
        /*SPR 16596 END*/     
        {
            p_ue_context->rrm_cgi_triggered_for_rat_type = NO_RAT_CONFIGURED; 
			/* SPR 16406 24Sep Start */
			p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_EUTRAN_KNOWN_CELL_LIST;
			/* SPR 16406 24Sep End */
            for (i=RRM_ZERO; ((i < num_eutra_ngbrs) && (i < RRM_MAX_CELLS_REPORTED));i++) 
	    {
                /* SPR 16406 21AUG Start */
                unknown_pci_detected = RRM_FALSE;
                /* SPR 16406 21AUG End */
                /* SPR 16406 8SEP start */
                pci_confusion_detected = RRM_FALSE;
                pci_blacklisted = RRM_FALSE;
                /* SPR 16406 8SEP end */

		    RRM_MEMSET (&tgt_cell_data,RRM_ZERO,sizeof(rrm_cell_meas_data_t));

		    if (!rrm_get_cell_specific_info (p_proc_data->meas_id,
					    p_proc_data->eutran_neighbor_list[i].pci,
					    &tgt_cell_data,
					    p_ue_context,
					    &pci_confusion_detected,
					    &pci_blacklisted))
		    {
		/* SPR 16406 8SEP start */
			    RRM_TRACE(g_uem_log_on_off,
					    p_rrm_uem_facility_name,
					    RRM_INFO,
					    " PCI %d not found in NCL",
					    p_proc_data->eutran_neighbor_list[i].pci);
		/* SPR 16406 8SEP end */
                    /* SPR 16406 3Aug Start */
			   unknown_pci_detected = RRM_TRUE;
                    /* SPR 16406 3Aug End */

		    }

		    else if ((pci_blacklisted == RRM_TRUE) && (RRM_FALSE == pci_confusion_detected))
		    {
                    /* SPR 16406 8SEP start */
			    RRM_TRACE(g_uem_log_on_off,
					    p_rrm_uem_facility_name,
					    RRM_BRIEF,
					    "PCI[%u] reported in EVENT[%d] is Blacklisted ",
					    p_proc_data->eutran_neighbor_list[i].pci,
                            p_ue_context->meas_rep_event);
                    /* SPR 16406 8SEP end */
			    continue;
		    }

		    /*If daho pci is reported do not store it,because
		      during ho DAHO pci will be considered in the end*/
		    if(RRM_FAILURE == rrm_match_with_eutran_daho_pci(
					    p_ue_context,
					    p_proc_data->eutran_neighbor_list[i].pci,
					    tgt_cell_data.carr_earfcn))
		    {
                    /* SPR 16406 8SEP start */
			    RRM_TRACE(g_uem_log_on_off,
					    p_rrm_uem_facility_name,
					    RRM_INFO,
					    " Daho PCI[%u] reported in EVENT[%u]",
					    p_proc_data->eutran_neighbor_list[i].pci,
                            p_ue_context->meas_rep_event);
                    /* SPR 16406 8SEP end */
			    continue;
		    }

		    p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
						(p_ue_context->cell_index);

		    if ((rrm_report_quantity_et)p_common_params_for_eutra->report_quantity == RRM_RRC_BOTH) 
		    {
			    if(RRM_RRC_RSRQ == p_ue_context->trigger_quantity)
			    {
                        /*SPR 17529 Fix Start*/
                        if(p_common_params_for_eutra->bitmask & RRMCM_RMIF_MIN_THRESHOLD_RSRQ_PRESENT)
                        {
                            if(p_common_params_for_eutra->min_threshold_rsrq <= p_proc_data->eutran_neighbor_list[i].rsrq)
                            {
				    tgt_cell_data.meas_result = \
								p_proc_data->eutran_neighbor_list[i].rsrq;
                                RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                                        "incase of rsrp and rsrq both configured rsrq value [ %d ] "
                                        "is greater than and equal minimum thrshold rsrq [ %d ]",
                                        tgt_cell_data.meas_result, p_common_params_for_eutra->min_threshold_rsrq);
                            }
                            else
                            {
                                continue;
                            }
                        }
                        /*SPR 17529 Fix End*/
			    }
			    else
			    {
				    tgt_cell_data.meas_result = 
					    p_proc_data->eutran_neighbor_list[i].rsrp;
			    }

		    } 
		    else 
		    {           
			    if(p_ue_context->trigger_quantity == RRM_RRC_RSRP)
			    {
				    tgt_cell_data.meas_result = \
								p_proc_data->eutran_neighbor_list[i].rsrp;
			    }
			    else 
			    {
                        /*SPR 17529 Fix Start*/
                        if(p_common_params_for_eutra->bitmask & RRMCM_RMIF_MIN_THRESHOLD_RSRQ_PRESENT)
                        {
                            if(p_common_params_for_eutra->min_threshold_rsrq <= p_proc_data->eutran_neighbor_list[i].rsrq)
                            {
				    tgt_cell_data.meas_result = \
								p_proc_data->eutran_neighbor_list[i].rsrq;
                                RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_DETAILED,
                                        "incase of only rsrq configured rsrq value [ %d ] "
                                        "is greater than and equal minimum thrshold rsrq [ %d ]",
                                        tgt_cell_data.meas_result, p_common_params_for_eutra->min_threshold_rsrq);
                            }
                            else
                            {
                                continue;
                            }
                        }
                        /*SPR 17529 Fix End*/
			    }
		    }  

		    if(RRM_UE_STATE_HO_ONGOING == p_ue_context->ue_state)
            {
                    /* SPR 16406 8SEP start */
                    /*Cov 88197 + */
                    if((RRM_TRUE == is_pci_present_in_cell_selection_list(
                            &p_ue_context->ho_params.eutra_known_cell_selection_list,
                                    p_ue_context->ho_params.target_phys_cell_id,
                                    p_ue_context->ho_params.target_earfcn))
                            ||

                            
                            (RRM_TRUE == is_pci_present_in_cell_selection_list(
                                                                               &p_ue_context->ho_params.eutra_unknown_cell_selection_list,
                                                                               p_ue_context->ho_params.target_phys_cell_id,
                                                                               p_ue_context->ho_params.target_earfcn))
                            ||
                            (RRM_TRUE == is_pci_present_in_cell_selection_list(
                                                                               &p_ue_context->ho_params.eutra_confused_cell_selection_list,
                                                                               p_ue_context->ho_params.target_phys_cell_id,
                                                                               p_ue_context->ho_params.target_earfcn)))
                        /*Cov 88197 - */
                        /* SPR 16406 8SEP end */
                {
                    is_ho_cancelled = RRM_FALSE;
                }
            }
		    else
		    {
			    is_ho_cancelled = RRM_FALSE;
		    }

                /* SPR 16406 8SEP start */
		    rrm_ue_update_neighbor_list (p_rrm_glb_ctxt->p_ue_context,
                    /*SPR 17777 +-*/
				    p_proc_data->eutran_neighbor_list[i].pci,
				    tgt_cell_data,
				    &(p_proc_data->eutran_neighbor_list[i]),
				    &pci_confusion_detected,
                        /* SPR 16406 3Aug Start */
				    unknown_pci_detected,
                        /* SPR 16406 3Aug End */
				    /* Fix for CSR 58972 end*/
				    event_type 
				    );
                /* SPR 16406 8SEP end */

	    }

            if((p_ue_context->ho_params.eutra_known_cell_selection_list.count) ||
                    (p_ue_context->ho_params.eutra_unknown_cell_selection_list.count) ||
                    (p_ue_context->ho_params.eutra_confused_cell_selection_list.count))
            {
                /* SPR 22553 FIXED START*/
                if(p_ue_context->rrm_sub_profile_id_info.sub_profile_id)
                /* SPR 22553 FIXED END*/    
                {
                    rrm_ue_prioritize_intra_rat_meas_result(p_ue_context);
                }
            }

            ret_val = rrm_uem_search_and_select_pci_for_ho(p_ue_context, transaction_id);

            /* SPR 16406 21AUG Start */
            /* Comments removed */
            /* SPR 16406 21AUG End */
            /* SPR 16406 21AUG Start */
            /* SPR 16406 8SEP start */
            if ((p_ue_context->ho_params.eutra_known_cell_selection_list.count) ||
                    (p_ue_context->meas_rep_event == MEAS_REP_EVENT_A4) ||
                    (p_ue_context->meas_rep_event == MEAS_REP_EVENT_A5))
                /* SPR 16406 8SEP end */
                /* SPR 16406 21AUG End */
            {
              /* SPR-17852 START */
                if ((p_ho_params->blind_timer)) {
                  /* SPR-17852 END */
                    rrm_ue_stop_timer (p_ho_params->blind_timer);
                    p_ho_params->blind_timer = RRM_PNULL;
                }
            }
        }

        if ((is_ho_cancelled == RRM_TRUE) &&
                (RRM_UE_STATE_HO_ONGOING == p_ue_context->ue_state)
           )
        {
            /* Cancel the previous HO Required message */
            ret_val = rrm_build_and_send_ho_cancel_req(
                    p_ue_context,
                    p_ue_context->ue_index,
                    transaction_id); 
            if ( ret_val == RRM_SUCCESS )
            {
                p_ue_context->ue_ho_current_state =  RRM_HO_STATE_NULL;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "HO Cancelled! Due to exit criteria received in A3 event from UE. "
                        "Changed the UE HO state to RRM_HO_STATE_NULL as HO_CANCEL sent");
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "HO Cancelation Failed");
            }

            /* SPR 16406 21AUG Start */
            /* Code Removed */
            /* SPR 16406 21AUG End */
        }
        /* SPR 16406 18sep Start */
        /*SPR 17527 START*/
        /*CODE DELETED*/
        /*SPR 17527 END*/
        /* SPR 16406 18sep End */
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/* SPR 16406 8SEP start */
/******************************************************************************
 * Function Name  : rrm_insert_in_ho_cgi_pending_list 
 * Inputs         : p_ue_context
 * Outputs        : NONE
 * Returns        : NONE 
 * Description    : insert new node in ho cgi pending list 
 ******************************************************************************/
/* SPR 16406 18sep Start */
rrm_void_t 
rrm_insert_in_ho_cgi_pending_list
(
 /* SPR 16406 10SEP start */
    rrm_ue_context_t    *p_ue_context,
    U16                 pci_for_which_cgi_required,
     rrm_oam_eutran_global_cell_id_t *p_cgi,                                
    U16                 carr_earfcn
 )
/* SPR 16406 18sep End */
{
    ho_pending_cgi_node_t *p_new_cgi_pending_node = RRM_PNULL;
    YLIST *p_ho_pending_cgi_list        = RRM_PNULL;   

    RRM_UT_TRACE_ENTER();

    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index); 
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off,
                p_rrm_uem_facility_name,
                RRM_ERROR,
                "cell context not found for [UE:%u]", p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return;
    }


    /* Assigning to actual list kept in cell context */
    p_ho_pending_cgi_list = &p_cell_context->rrm_cgi_pending_list.
        rrm_ho_pending_cgi_list[p_cell_context->cell_index].ho_pending_cgi_list;


    p_new_cgi_pending_node = (ho_pending_cgi_node_t *)rrm_mem_get(sizeof(ho_pending_cgi_node_t));
    if(RRM_PNULL == p_new_cgi_pending_node)
    {
        RRM_TRACE(g_uem_log_on_off,
                p_rrm_uem_facility_name,
                RRM_ERROR,
                "Memory Allocation failed ");
        RRM_UT_TRACE_EXIT();
        return;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO, \
                "Adding new node[PCI:%u and EARFCN:%u] in ho cgi pending list for [UE:%u]",
                pci_for_which_cgi_required,
                carr_earfcn,
                p_ue_context->ue_index);

/* SPR 16406 18sep Start */
	p_new_cgi_pending_node->ho_pending_cgi_node_data.bitmask = RRM_ZERO; 
        p_new_cgi_pending_node->ho_pending_cgi_node_data.ue_idx = p_ue_context->ue_index;
        p_new_cgi_pending_node->ho_pending_cgi_node_data.pci = pci_for_which_cgi_required;
        p_new_cgi_pending_node->ho_pending_cgi_node_data.earfcn = carr_earfcn;


        if(RRM_PNULL != p_cgi)
	{
		RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, \
				"Adding CGI for cell : [PCI:%u and EARFCN:%u] in ho cgi pending list for ",pci_for_which_cgi_required,carr_earfcn);

		p_new_cgi_pending_node->ho_pending_cgi_node_data.bitmask |= RRM_HO_PENDING_NODE_CGI_PRESENT; 

		RRM_MEMCPY(&(p_new_cgi_pending_node->ho_pending_cgi_node_data.cgi),
				(p_cgi),
				sizeof(rrm_oam_eutran_global_cell_id_t));
	}
/* SPR 16406 18sep End */


        ylPushTail(p_ho_pending_cgi_list, &p_new_cgi_pending_node->node);        
    }

    RRM_UT_TRACE_EXIT();
}
/* SPR 16406 8SEP end */
/******************************************************************************
 ** Function Name  : rrm_delete_entry_in_ho_cgi_pending_list 
 ** Inputs         : p_ue_context, pci and earfcn
 ** Outputs        : NONE
 ** Returns        : NONE 
 ** Description    : delete entries in ho cgi pending list for given pci+earfcn combination
 *******************************************************************************/
rrm_void_t
    rrm_delete_entry_in_ho_cgi_pending_list
(
 rrm_ue_context_t  *p_ue_context,
 U16  pci,
 U16  earfcn
 )
{
    ho_pending_cgi_node_t *p_temp_cgi_pending_node = RRM_PNULL;
    ho_pending_cgi_node_t *p_next_cgi_pending_node = RRM_PNULL;
    U16 cgi_node_count = RRM_ZERO;
    YLIST ho_pending_cgi_list;

    RRM_UT_TRACE_ENTER();

    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off,
                p_rrm_uem_facility_name,
                RRM_ERROR,
                "p_cell_context not found for [UE:%u]", p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return;
    }

    ylInit(&ho_pending_cgi_list);

    /* Assigning to actual list kept in cell context */
    ho_pending_cgi_list = p_cell_context->rrm_cgi_pending_list.
        rrm_ho_pending_cgi_list[p_cell_context->cell_index].ho_pending_cgi_list;

    cgi_node_count = ylCount(&ho_pending_cgi_list);

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED, \
            "present count in ho cgi pending list :%u ",cgi_node_count);

    p_temp_cgi_pending_node = (ho_pending_cgi_node_t *)ylFirst(&ho_pending_cgi_list);
    while(p_temp_cgi_pending_node)
    {
        p_next_cgi_pending_node = (ho_pending_cgi_node_t *)ylNext(&p_temp_cgi_pending_node->node);
        if((p_temp_cgi_pending_node->ho_pending_cgi_node_data.pci == pci ) &&
                (p_temp_cgi_pending_node->ho_pending_cgi_node_data.ue_idx == p_ue_context->ue_index ) &&
                (p_temp_cgi_pending_node->ho_pending_cgi_node_data.earfcn == earfcn ))
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO, \
                    "deleting entry in ho cgi pending list [PCI:%u and EARFCN:%u] for [UE:%u] ",
                    pci, earfcn, p_ue_context->ue_index);
            /* SPR 16406 8SEP start */
            /* Code removed */
            /* SPR 16406 8SEP end */
            ylDelete(&ho_pending_cgi_list , (YLNODE *)p_temp_cgi_pending_node);
            RRM_MEM_FREE(p_temp_cgi_pending_node);
            break;
        }
        p_temp_cgi_pending_node = p_next_cgi_pending_node;
    }
    p_cell_context->rrm_cgi_pending_list.
        rrm_ho_pending_cgi_list[p_cell_context->cell_index].ho_pending_cgi_list
        = ho_pending_cgi_list;
    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : rrm_uem_search_and_select_pci_for_ho 
 * Inputs         : p_ue_context
 * Outputs        : NONE
 * Returns        : SUCCESS/FAILURE
 * Description    : search through Cell selection list for HO and take appropriate action 
 ******************************************************************************/
rrm_return_et 
rrm_uem_search_and_select_pci_for_ho 
(
    rrm_ue_context_t *p_ue_context,
    U16 transaction_id
)
{
    rrm_return_et ret_val = RRM_FAILURE;
    cell_selection_priority_list_t *p_cell_selection_list = RRM_PNULL;
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */

    RRM_UT_TRACE_ENTER();


    if(RRM_INVALID_CELL_LIST_TYPE != p_ue_context->rrm_cell_selection_info.current_cell_list)
    {
        p_cell_selection_list = rrm_uem_get_active_cell_selection_list(p_ue_context);
        if(RRM_PNULL == p_cell_selection_list)
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                    "Invalid cell selection list is present ");
            RRM_UT_TRACE_EXIT();
            return ret_val;
        }
    }
    rrm_sort_and_rank_meas_reported_cell(p_ue_context);

    switch(p_ue_context->rrm_cell_selection_info.current_cell_list)     
    {

        case RRM_EUTRAN_KNOWN_CELL_LIST:
        case RRM_UTRAN_KNOWN_CELL_LIST:
        case RRM_GERAN_KNOWN_CELL_LIST:
            {
					/*SPR_17893_START*/ 
					while(((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state) ||
								(RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES == p_ue_context->ue_state)) &&
							/*SPR_17893_END*/ 
                        (p_cell_selection_list->next_target_cell_idx != p_cell_selection_list->count))
                {
                    if((p_ue_context->rrm_csg_membership.rrm_csg_membership_status == RRM_MEMBER) ||
                    /* SPR 17564 Fix Start */
                          (RRM_TRUE == rrm_is_ue_special_previledge(p_ue_context)))
                    /* SPR 17564 Fix End */
                    {
                        /*As ue is csg member try to ho it to a closed cell*/
                        phy_cell_id_info = rrm_get_best_pci_for_ho (p_ue_context,
                                RRM_OAM_ACCESS_MODE_CLOSE);
                             
                        /* SPR 14642 changes start */
                            /* Shall check for Hybrid cells for a MEMBER UE if no PCI found for Closed ACCESS Mode*/
                        if ((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id) 
                            /* SPR 17564 Fix Start */
                             && (RRM_TRUE == rrm_is_ue_special_previledge(p_ue_context)))
                            /* SPR 17564 Fix End */
                        {
                            phy_cell_id_info = rrm_get_best_pci_for_ho(p_ue_context,
                                    RRM_OAM_ACCESS_MODE_HYBRID);
                        }
                            /* Shall check for Open cells for a MEMBER UE if no PCI found for Closed/Hybrid ACCESS Mode*/
                        if ((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id) 
                            /* SPR 17564 Fix Start */
                             && (RRM_TRUE == rrm_is_ue_special_previledge(p_ue_context)))
                            /* SPR 17564 Fix End */
                        {
                            phy_cell_id_info = rrm_get_best_pci_for_ho(p_ue_context,
                                    RRM_OAM_ACCESS_MODE_OPEN);
                        }
                         /*SPR 14642 changes end */
                    }
                    else if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
                            == RRM_NOT_MEMBER)
                    {
                        /*As ue is csg member try to ho it to a closed cell*/
                        phy_cell_id_info = rrm_get_best_pci_for_ho(p_ue_context,
                                RRM_OAM_ACCESS_MODE_OPEN);
                    }
                    /* This is the case where timer is not configured 
                     * hence start the HO here itself */
			/* SPR 16406 3Aug Start */
                    if (INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id ) {
          					/*SPR_17893_START*/ 
							if((UTRA_CELL == p_ue_context->target_cell_type) &&
									!(RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT & p_ue_context->ue_capability_params.bitmask))
							{    
								if((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state))
								{
									rrm_populate_and_send_utran_cap_enq_req(p_ue_context);
									rrm_ue_set_state(p_ue_context,RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES);
									rrm_set_utran_capability_enq_info(p_ue_context,RRM_TRUE,RRM_FALSE);
								}
								/*Move Back one index,if the valid id is found*/
								if((RRM_ZERO < p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx) && 
										(p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx <= 
										 p_ue_context->ho_params.utra_known_cell_selection_list.count))
								{
									p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx--; 
								}
								/*Undo the bitmask,to check if DAHO cell is selected */
								if(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED)
								{
									p_ue_context->ho_params.utra_known_cell_selection_list.bitmask &= ~RRM_UE_BLIND_CELL_HO_TRIGGERED;  
								}    

								p_ue_context->is_ho_reqd_on_recv_utan_cap  = RRM_TRUE;
								RRM_UT_TRACE_EXIT();
								return RRM_SUCCESS;
							}
							else
							{    
							/*SPR 17893 END*/

                        /* Trigger HO Required message */
                        ret_val = rrm_build_and_send_ho_required (phy_cell_id_info,
                                p_ue_context,
                                transaction_id);    
                        // coverity_fix 88199 +-
                        RRM_TRACE(g_cellm_log_on_off,
                                p_g_rrm_uem_facility_name, 
						RRM_BRIEF, 
						"%s: Build & send HO Required (%s)", 
						"UE:", ret_val?"SUCCESS":"FAILED");    
							/*SPR 17893 START*/
							}
							/*SPR 17893 END*/

                        if (RRM_SUCCESS == ret_val) {
                            /* SPR 16406 8SEP start */
                            /* Code removed */
                            /* SPR 16406 8SEP end */
                            RRM_UT_TRACE_EXIT();
                            return ret_val;
                        }
                        else
                        {
                            continue;
                        }
                        /* coverity_fix 88199 +- */
                    }
			else
			{
				RRM_TRACE(g_cellm_log_on_off,
                                p_g_rrm_uem_facility_name, 
						RRM_BRIEF, 
						"Invalid physical cell ID returned"); 
				RRM_UT_TRACE_EXIT();
				return ret_val;
			}
			/* SPR 16406 3Aug End */
                }
                if((RRM_ZERO == p_cell_selection_list->count) ||
                        (p_cell_selection_list->next_target_cell_idx == p_cell_selection_list->count) )
                {
                    switch(p_ue_context->rrm_cell_selection_info.current_cell_list)
                    {
                        case RRM_EUTRAN_KNOWN_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_EUTRAN_CONFUSED_CELL_LIST;
                            break;
                        case RRM_UTRAN_KNOWN_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_UTRAN_CONFUSED_CELL_LIST;
                            break;
                        case RRM_GERAN_KNOWN_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_GERAN_CONFUSED_CELL_LIST;
                            break;
                        default:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_INVALID_CELL_LIST_TYPE;
                            break;

                    } 
                    /* SPR 16406 8SEP start */
                    ret_val = rrm_uem_search_and_select_pci_for_ho(p_ue_context, transaction_id);
                    /* SPR 16406 8SEP end */
                }

            }
            break;

        case RRM_EUTRAN_CONFUSED_CELL_LIST:
        case RRM_UTRAN_CONFUSED_CELL_LIST:
        case RRM_GERAN_CONFUSED_CELL_LIST:
            {
				   /*SPR 17893 START*/
					while(((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state)
						|| (RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES == p_ue_context->ue_state))
			        /*SPR 17893 END*/
                        && (p_cell_selection_list->count > RRM_ZERO)
                     ) 
                {
                    /* Search for PCI in confused cell selection list for CGI configuration that will be used for HO then */
                    ret_val = rrm_handle_pci_in_confused_cell_selection_list(p_ue_context, p_cell_selection_list);
                    if(RRM_SUCCESS == ret_val)
                    {
                        /* If new PCI found then trigger CGI reporting for the PCI */
                        ret_val = rrm_config_ue_for_cgi_reporting_for_new_detected_pci(
                                p_ue_context, EUTRAN_RAT,RRM_CGI_REP_CAUSE_PCI_CONFUSION);
                        if(RRM_SUCCESS == ret_val)
                        {
                            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                    "CGI reporing has been triggerd if Autonomous Gap is supported, "
                                    "UE Reconfig req. will not be sent to UE if CDRX already configured,"
                                    "else UE reconfig req triggered for CDRX configuration. PCI [%u]",
                                    p_ue_context->pci_for_which_cgi_required );
                            p_ue_context->rrm_cgi_triggered_for_rat_type = EUTRAN_RAT;
                            RRM_UT_TRACE_EXIT();
                            return ret_val;
                        }
                        else
                        {
                            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_ERROR,
                                    "CGI reporing has Failed for the confused cell in cell selection list");
                            /* Delete this pci entry in cell selection list as HO attempt has failed */
                            rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                    p_cell_selection_list,
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn);
                            continue;

                        }
                    }
                    else
                    {
                        /* SPR 16406 8SEP start */
                        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
					    "Either no cell is available for CGI or CGI process has already been triggered earlier for same confused cell");
                        /* SPR 16406 8SEP end */
                        /* SPR 16406 18sep Start */
                        ret_val = RRM_SUCCESS;
                        /* SPR 16406 18sep End */
                        break;
                    }
                }
                if((RRM_ZERO == p_cell_selection_list->count) ||
                        (p_cell_selection_list->next_target_cell_idx == p_cell_selection_list->count) )
                {
                    switch(p_ue_context->rrm_cell_selection_info.current_cell_list)
                    {
                        case RRM_EUTRAN_CONFUSED_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_EUTRAN_UNKNOWN_CELL_LIST;
                            break;
                        case RRM_UTRAN_CONFUSED_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_UTRAN_UNKNOWN_CELL_LIST;
                            break;
                        case RRM_GERAN_CONFUSED_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_GERAN_UNKNOWN_CELL_LIST;
                            break;
                        default:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_INVALID_CELL_LIST_TYPE;
                            break;
                    } 
                    /* SPR 16406 8SEP start */
                    ret_val = rrm_uem_search_and_select_pci_for_ho(p_ue_context, transaction_id);
                    /* SPR 16406 8SEP end */
                }
            }
            break;

        case RRM_EUTRAN_UNKNOWN_CELL_LIST:
        case RRM_UTRAN_UNKNOWN_CELL_LIST:
        case RRM_GERAN_UNKNOWN_CELL_LIST:
            {
				    /*SPR 17893 START*/
					while(((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state)
							|| (RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES == p_ue_context->ue_state)) 
							/*SPR 17893 END*/
                        && (p_cell_selection_list->count > RRM_ZERO))
                {
                    if(RRM_SUCCESS == rrm_handle_pci_in_unknown_cell_selection_list(p_ue_context))
                    {
                        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                "Unknown cell selection is done for either CGI or HO");
                        RRM_UT_TRACE_EXIT();
                        return RRM_SUCCESS;
                    }
                    else
                    {
                        /* SPR 16406 8SEP start */
                        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                                "Either no cell is available for CGI or CGI process has already been triggered earlier for same unknown cell");
                        /* SPR 16406 8SEP end */
                        RRM_UT_TRACE_EXIT();
                        return RRM_FAILURE;

                    }
                }
                if((RRM_ZERO == p_cell_selection_list->count) ||
                        (p_cell_selection_list->next_target_cell_idx == p_cell_selection_list->count) )
                {
                    switch(p_ue_context->rrm_cell_selection_info.current_cell_list)
                    {
                        case RRM_EUTRAN_UNKNOWN_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_UTRAN_KNOWN_CELL_LIST;
                            break;
                        case RRM_UTRAN_UNKNOWN_CELL_LIST:
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_GERAN_KNOWN_CELL_LIST;
                            break;
                        default:
						    /*SPR 17893 START*/
							    if((p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx < 
										 p_ue_context->ho_params.utra_known_cell_selection_list.count))
                                {
                                    p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_UTRAN_KNOWN_CELL_LIST;
                                }
                                else
                                {
						     /*SPR 17893 END*/
                            p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_INVALID_CELL_LIST_TYPE;
						    /*SPR 17893 START*/
                                }
						    /*SPR 17893 END*/
                            break;
                    }                 
                    /* SPR 16406 8SEP start */
                    ret_val = rrm_uem_search_and_select_pci_for_ho(p_ue_context, transaction_id);
                    /* SPR 16406 8SEP end */
                }
            }
            break;
        default:
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                    "Invalid current cell_selection_list"
                    "No PCI is available for HO in cell selection lists for [UE:%u]",
                    p_ue_context->ue_index);

    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/******************************************************************************
 * Function Name  : rrm_uem_get_active_cell_selection_list 
 * Inputs         : p_ue_context
 * Outputs        : NONE
 * Returns        : cell selection list for HO attempt  
 * Description    : This function is used to get current active HO cell selection list 
 ******************************************************************************/
cell_selection_priority_list_t* rrm_uem_get_active_cell_selection_list
(
 rrm_ue_context_t *p_ue_context
)
{

    /*SPR 17529 Fix Start */
    cell_selection_priority_list_t *p_out_cell_selection_list = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    /*SPR 17529 Fix Start */

    switch(p_ue_context->rrm_cell_selection_info.current_cell_list)
    {
        case RRM_EUTRAN_KNOWN_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.eutra_known_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_EUTRAN_CELL;
            break;
        case RRM_EUTRAN_UNKNOWN_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.eutra_unknown_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_EUTRAN_CELL;
            break;
        case RRM_EUTRAN_CONFUSED_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.eutra_confused_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_EUTRAN_CELL;
            break;
        case RRM_UTRAN_KNOWN_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.utra_known_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_UTRAN_CELL;
            break;
        case RRM_UTRAN_UNKNOWN_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.utra_unknown_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_UTRAN_CELL;
            break;
        case RRM_UTRAN_CONFUSED_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.utra_confused_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_UTRAN_CELL;
            break;
        case RRM_GERAN_KNOWN_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.geran_known_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_GERAN_CELL;
            break;
        case RRM_GERAN_UNKNOWN_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.geran_unknown_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_GERAN_CELL;
            break;
        case RRM_GERAN_CONFUSED_CELL_LIST: 
            p_out_cell_selection_list = &(p_ue_context->ho_params.geran_confused_cell_selection_list);        
            p_out_cell_selection_list->rat_type = RRM_GERAN_CELL;
            break;
        default:
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
            "Invalid Cell Selection list: %u", 
            p_ue_context->rrm_cell_selection_info.current_cell_list);
    }

    RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
            "active cell selection List for HO: %u", 
            p_ue_context->rrm_cell_selection_info.current_cell_list);

    RRM_UT_TRACE_EXIT();
    return p_out_cell_selection_list;
            /*SPR 17529 Fix End */
}
/* SPR 16406 21AUG Start */
/***********************************************************************************************
 * Function Name  : rrm_get_report_id_for_configured_cgi 
 * Inputs         : p_ue_context
 * Outputs        : NONE
 * Returns        : report_id corresponding to CGIReport
 * Description    : Returns the report_id which had been used for CGI report 
 ***********************************************************************************************/
U8
rrm_get_report_id_for_configured_cgi
(
 rrm_ue_context_t *p_ue_context	
 )
{
    U8 report_id = RRM_ZERO;
    meas_report_info_node_t *p_meas_report_info = RRM_PNULL;
    YLNODE *p_node = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_node = ylFirst(&(p_ue_context->meas_report_list));

    while (p_node) {
        p_meas_report_info = (meas_report_info_node_t*) p_node;
        if (p_meas_report_info->reporting_event == MEAS_REP_EVENT_FOR_CGI) 
        {
            report_id = p_meas_report_info->report_config_id; 
            break;
        }
        p_node = ylNext(p_node);
    }
    RRM_TRACE(g_uem_log_on_off,
            p_rrm_uem_facility_name,
            RRM_BRIEF,
            "report_id configured for [UE:%u] is %u", 
            p_ue_context->ue_index, report_id);

    RRM_UT_TRACE_EXIT();
    return report_id;
}
/* SPR 16406 21AUG End */

/***********************************************************************************************
 * Function Name  : rrm_get_meas_id_for_configured_cgi 
 * Inputs         : p_ue_context
 * Outputs        : NONE
 * Returns        : meas_id corresponding to CGIReport
 * Description    : Returns the meas_id which had been used for CGI report 
 ***********************************************************************************************/
U8
rrm_get_meas_id_for_configured_cgi
(
	rrm_ue_context_t *p_ue_context	
)
{
    U8 meas_id = RRM_ZERO;
    meas_report_info_node_t *p_meas_report_info = RRM_PNULL;
    YLNODE *p_node = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_node = ylFirst(&(p_ue_context->meas_report_list));

    while (p_node) {
        p_meas_report_info = (meas_report_info_node_t*) p_node;
        if (p_meas_report_info->reporting_event == MEAS_REP_EVENT_FOR_CGI) 
        {
            meas_id = p_meas_report_info->meas_id; 
            break;
        }
        p_node = ylNext(p_node);
    }
    RRM_TRACE(g_uem_log_on_off,
            p_rrm_uem_facility_name,
            RRM_ERROR,
            "meas_id configured for [UE:%u] is %u", 
            p_ue_context->ue_index, meas_id);

    RRM_UT_TRACE_EXIT();
    return meas_id;
}
/***********************************************************************************************
 * Function Name  : rrm_handle_pci_in_unknown_cell_selection_list 
 * Inputs         : p_ue_context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Search the appropriate cell in unknown cell selection list for CGI 
 ***********************************************************************************************/
rrm_return_et
rrm_handle_pci_in_unknown_cell_selection_list
(
  rrm_ue_context_t *p_ue_context
)
{
    ncl_params_t  *p_ncl_params   = RRM_PNULL;
    rrm_cell_context_t  *p_cell_context = RRM_PNULL;
    rrm_return_et ret_val = RRM_FAILURE;
    rrm_counter_t index = RRM_ZERO; 
    rrm_counter_t neighbor_index = RRM_ZERO; 
    rrm_bool_et ho_triggered = RRM_FALSE;
    rrm_bool_et  cgi_entry_found = RRM_FALSE;
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    cell_selection_priority_list_t *p_unknown_cell_list = 
        &p_ue_context->ho_params.eutra_unknown_cell_selection_list;
    U16 transaction_id = rrm_generate_txn_id();
    /* SPR 16406 8SEP start */
    /* Code removed */
    /* SPR 16406 8SEP end */

    RRM_UT_TRACE_ENTER();
    p_ncl_params = rrm_cellm_get_ncl_info(p_ue_context->cell_index);
    p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off,
                p_rrm_uem_facility_name,
                RRM_ERROR,
                "p_cell_context not found for [UE:%u]", p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return ret_val;
    }

    /* First need to check if this cell is already present in 
       NRT or not.if found, then perform ho on the same else go for the CGI */
    if(RRMCM_RMIF_LTE_NCL_PRESENT & p_ncl_params->bitmask)
    {
        for(index = RRM_ZERO;index < p_unknown_cell_list->count; index++)  
        {
            if(p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn == 
                    p_unknown_cell_list->reported_neighbor_cell_info[index].target_earfcn) 
            {
                for(neighbor_index = RRM_ZERO;neighbor_index< p_ncl_params->lte_ncl.num_valid_intra_freq_cell;neighbor_index++)
                {
                    if(p_unknown_cell_list->reported_neighbor_cell_info[index].phy_cell_id == 
                            p_ncl_params->lte_ncl.intra_freq_cells[neighbor_index].phy_cell_id)
                    {
                        phy_cell_id_info.phy_cell_id = p_unknown_cell_list->reported_neighbor_cell_info[index].phy_cell_id;
                        phy_cell_id_info.carr_earfcn = p_unknown_cell_list->reported_neighbor_cell_info[index].target_earfcn;
                        /* Trigger HO for this PCI */
                        ret_val = rrm_build_and_send_ho_required (
                                phy_cell_id_info,
                                p_ue_context,
                                transaction_id);
                        if(RRM_SUCCESS == ret_val)
                        {
                            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                    "Ho Triggered for PCI[%u] + Earfcn[%u] for [UE:%u]",
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn,
                                    p_ue_context->ue_index);
                            ho_triggered = RRM_TRUE;
                        }
                        else
                        {
                            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                    "Ho Trigger Failed for PCI[%u] + Earfcn[%u] for [UE:%u]"
                                    "So removing entry from unknown cell selection list",
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn,
                                    p_ue_context->ue_index);
                            /* Delete this pci entry in cell selection list as HO attempt has failed */
                            rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                    p_unknown_cell_list,
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn);
                        }

                    }

                }
            }
            else
            {
                for(neighbor_index = RRM_ZERO;neighbor_index< p_ncl_params->lte_ncl.num_valid_inter_freq_cell;neighbor_index++)
                {
                    if((p_unknown_cell_list->reported_neighbor_cell_info[index].phy_cell_id == 
                                p_ncl_params->lte_ncl.inter_freq_cells[neighbor_index].phy_cell_id) &&
                            (p_unknown_cell_list->reported_neighbor_cell_info[index].target_earfcn == 
                             p_ncl_params->lte_ncl.inter_freq_cells[neighbor_index].eutra_carrier_arfcn))
                    {
                        phy_cell_id_info.phy_cell_id = p_unknown_cell_list->reported_neighbor_cell_info[index].phy_cell_id;
                        phy_cell_id_info.carr_earfcn = p_unknown_cell_list->reported_neighbor_cell_info[index].target_earfcn;
                        /* Trigger HO for this PCI */
                        ret_val = rrm_build_and_send_ho_required (
                                phy_cell_id_info,
                                p_ue_context,
                                transaction_id);
                        if(RRM_SUCCESS == ret_val)
                        {
                            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                    "Ho Triggered for PCI[%u] + Earfcn[%u] for [UE:%u]",
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn,
                                    p_ue_context->ue_index);
                            ho_triggered = RRM_TRUE;
                        }
                        else
                        {
                            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                    "Ho Trigger Failed for PCI[%u] + Earfcn[%u] for [UE:%u]"
                                    "So removing entry from unknown cell selection list",
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn,
                                    p_ue_context->ue_index);
                            /* Delete this pci entry in cell selection list as HO attempt has failed */
                            rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                    p_unknown_cell_list,
                                    phy_cell_id_info.phy_cell_id,
                                    phy_cell_id_info.carr_earfcn);
                        }

                    }
                }
            }
        }
    }

    /* To perform HO CGI configuration for unknown PCI and store the same in HO CGI pending list */
    if(RRM_FALSE == ho_triggered)
    {
        if(p_ue_context->is_cgi_process_in_progress)
        {
            /* SPR 16406 8SEP start */
            /* Code removed */
            /* SPR 16406 8SEP end */
            if((p_unknown_cell_list->reported_neighbor_cell_info[RRM_ZERO].phy_cell_id ==
                        p_ue_context->pci_for_which_cgi_required) &&(p_ue_context->cgi_trigerred_for_earfcn == 
                            p_unknown_cell_list->reported_neighbor_cell_info[RRM_ZERO].target_earfcn) )

            {
                cgi_entry_found = RRM_TRUE;
                /* Already CGI process is ongoing so ignore this one */
                /* SPR 16406 18sep Start */
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                        "CGI has been triggered already for this Unknown PCI[%u] + Earfcn[%u] for [UE:%u]",
                        p_ue_context->pci_for_which_cgi_required,
                        p_ue_context->cgi_trigerred_for_earfcn,
                        p_ue_context->ue_index);
                /* SPR 16406 18sep End */
                /* SPR 16406 8SEP start */
                /* Code removed */
                /* SPR 16406 8SEP end */
                /* SPR 16406 8SEP start */
                RRM_UT_TRACE_EXIT();
                return ret_val;
                /* SPR 16406 8SEP end */
            }
            /* SPR 16406 8SEP start */
            /* Code removed */
            /* SPR 16406 8SEP end */
            if(RRM_FALSE == cgi_entry_found)
            {
                /* SPR 16406 8SEP start */
                /* Code removed */
                /* SPR 16406 8SEP end */
                /* SPR 16406 8SEP start */
                /* SPR 18178 Start */
                /*Code Removed*/
                /* SPR 18178 End*/
                /* SPR 16406 8SEP end */

                p_ue_context->pci_for_which_cgi_required = 
                    p_unknown_cell_list->reported_neighbor_cell_info[RRM_ZERO].phy_cell_id;
                p_ue_context->cgi_trigerred_for_earfcn = p_unknown_cell_list->
                    reported_neighbor_cell_info[RRM_ZERO].target_earfcn;

                /* SPR 16406 8SEP start */
                phy_cell_id_info.phy_cell_id = p_ue_context->pci_for_which_cgi_required;
                phy_cell_id_info.carr_earfcn = p_ue_context->cgi_trigerred_for_earfcn;
                /* SPR 16406 8SEP end */
                if(RRM_SUCCESS == rrm_config_ue_for_cgi_reporting_for_new_detected_pci
                        (p_ue_context, EUTRAN_RAT ,RRM_CGI_REP_CAUSE_NEW_PCI))
                {
                    /* SPR 16406 21AUG Start */
                    RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                            "CGI reporing has been triggerd if Autonomous Gap is supported, "
                            "UE Reconfig req. will not be sent to UE if CDRX already configured,"
                            "else UE reconfig req triggered for CDRX configuration. PCI+Earfcn[%u + %u]",
                            p_ue_context->pci_for_which_cgi_required, p_ue_context->cgi_trigerred_for_earfcn );
                    /* SPR 16406 21AUG End */
                    p_ue_context->rrm_cgi_triggered_for_rat_type = EUTRAN_RAT;
                    /* SPR 16406 8SEP start */
                    /* Code removed */
                    /* SPR 16406 8SEP end */
                    /* SPR 16406 8SEP start */
                    /* SPR 16406 18sep start */
                    rrm_insert_in_ho_cgi_pending_list(p_ue_context,
                            phy_cell_id_info.phy_cell_id,
                            RRM_PNULL,
                            phy_cell_id_info.carr_earfcn);
                    /* SPR 16406 18sep End */
                    /* SPR 16406 8SEP end */
                    RRM_UT_TRACE_EXIT();
                    return RRM_SUCCESS;
                }
                else
                {
                    RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_ERROR,
                            "CGI reporting is Failed for pci:%u !!!",
                            p_ue_context->pci_for_which_cgi_required);
                    /* Delete this pci entry in cell selection list as HO attempt has failed */
                    rrm_remove_pci_from_cell_selection_list(p_ue_context,
                            p_unknown_cell_list,
                            phy_cell_id_info.phy_cell_id,
                            phy_cell_id_info.carr_earfcn);
                }
            }
        }
        else
        {
            if(CELL_ALREADY_REGISTERD_FOR_ANR & p_cell_context->cell_already_register_anr)
            {    
                while(p_unknown_cell_list->count)
                {
                    /* consider cell at first index in cell selection list as it will be the best among all */
                    p_ue_context->pci_for_which_cgi_required = 
                        p_unknown_cell_list->reported_neighbor_cell_info[RRM_ZERO].phy_cell_id;
                    p_ue_context->cgi_trigerred_for_earfcn = 
                        p_unknown_cell_list->reported_neighbor_cell_info[RRM_ZERO].target_earfcn;
                    /* SPR 16406 8SEP start */
                    phy_cell_id_info.phy_cell_id = p_ue_context->pci_for_which_cgi_required;
                    phy_cell_id_info.carr_earfcn = p_ue_context->cgi_trigerred_for_earfcn;
                    /* SPR 16406 8SEP end */

                    if(RRM_SUCCESS == rrm_config_ue_for_cgi_reporting_for_new_detected_pci
                            (p_ue_context, EUTRAN_RAT ,RRM_CGI_REP_CAUSE_NEW_PCI))
                    {
                        /* SPR 16406 21AUG Start */
                        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                                "CGI reporing has been triggerd if Autonomous Gap is supported, "
                                "UE Reconfig req. will not be sent to UE if CDRX already configured,"
                                "else UE reconfig req triggered for CDRX configuration. PCI [%u] + Earfcn[%u]",
                                p_ue_context->pci_for_which_cgi_required,p_ue_context->cgi_trigerred_for_earfcn);
                        /* SPR 16406 21AUG End */
                        p_ue_context->rrm_cgi_triggered_for_rat_type = EUTRAN_RAT;
                        /* SPR 16406 8SEP start */
                        /* SPR 16406 18sep start */
                        rrm_insert_in_ho_cgi_pending_list(p_ue_context,
                                phy_cell_id_info.phy_cell_id,
                                RRM_PNULL,
                                phy_cell_id_info.carr_earfcn);
                        /* SPR 16406 18sep End */
                        /* SPR 16406 8SEP end */
                        RRM_UT_TRACE_EXIT();
                        return RRM_SUCCESS;
                    }
                    else
                    {
                        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_ERROR,
                                "CGI reporting is Failed for pci:%u !!!",
                                p_ue_context->pci_for_which_cgi_required);
                        /* Delete this pci entry in cell selection list as HO attempt has failed */
                        rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                p_unknown_cell_list,
                                phy_cell_id_info.phy_cell_id,
                                phy_cell_id_info.carr_earfcn);
                        continue;
                    }
                }

            }
            else
            {
                RRM_TRACE(g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_BRIEF,
                        "ANR feature is currently DISABLED");
            }
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/***********************************************************************************************
 * Function Name  : rrm_handle_pci_in_confused_cell_selection_list 
 * Inputs         : p_ue_context
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Search the appropriate cell in confused cell selection list for CGI 
 ***********************************************************************************************/
rrm_return_et
rrm_handle_pci_in_confused_cell_selection_list
(
  rrm_ue_context_t *p_ue_context,
  cell_selection_priority_list_t *p_confused_cell_list
)
{

    rrm_cell_context_t *p_cell_context = RRM_PNULL;
    rrm_return_et ret_val = RRM_FAILURE;
    /* SPR 16406 18sep Start */
    /* Code removed */
    /* SPR 16406 18sep End */
    rrm_bool_et  cgi_entry_found = RRM_FALSE;
    /* SPR 16406 8SEP start */
    /* Code removed */
    /* SPR 16406 8SEP end */
    RRM_UT_TRACE_ENTER();

    p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off,
                p_rrm_uem_facility_name,
                RRM_ERROR,
                "p_cell_context not found for [UE:%u]", p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return ret_val;
    }

    /* SPR 16406 21AUG Start */
    /* Code removed */
    /* SPR 16406 21AUG End */
    if(p_ue_context->is_cgi_process_in_progress)
    {
        /* SPR 16406 18sep Start */
        /* Code removed */
        /* SPR 16406 18sep End */
        /* SPR 16406 8SEP start */
        if((p_confused_cell_list->reported_neighbor_cell_info[RRM_ZERO].phy_cell_id ==
                    p_ue_context->pci_for_which_cgi_required) &&(p_ue_context->cgi_trigerred_for_earfcn == 
                        p_confused_cell_list->reported_neighbor_cell_info[RRM_ZERO].target_earfcn) )
        {
            /* SPR 16406 8SEP end */
            /* Already CGI process is ongoing so ignore this one */
            cgi_entry_found = RRM_TRUE;
            /* SPR 16406 18sep Start */
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "CGI Process is already in progress for Confused PCI[%u], Earfcn[%u]",
                    p_ue_context->pci_for_which_cgi_required,
                    p_ue_context->cgi_trigerred_for_earfcn);
            /* SPR 16406 18sep End */

            /* SPR 16406 8SEP start */
            RRM_UT_TRACE_EXIT();
            return ret_val;
            /* SPR 16406 8SEP end */
        }
        /* SPR 16406 18sep Start */
        /* Code removed */
        /* SPR 16406 18sep End */
        if(RRM_FALSE == cgi_entry_found)
        {
            /* SPR 16406 8SEP start */
            /* Code removed */
            /* SPR 16406 8SEP end */
            /* SPR 16406 8SEP start */
                /* SPR 18178 Start */
                /*Code Removed*/
                /* SPR 18178 End*/
            /* SPR 16406 8SEP end */
            p_ue_context->pci_for_which_cgi_required = 
                p_confused_cell_list->reported_neighbor_cell_info[RRM_ZERO].phy_cell_id;
            p_ue_context->cgi_trigerred_for_earfcn = p_confused_cell_list->
                reported_neighbor_cell_info[RRM_ZERO].target_earfcn;
        }
    }
    else
    {
        p_ue_context->pci_for_which_cgi_required = 
            p_confused_cell_list->reported_neighbor_cell_info[RRM_ZERO].phy_cell_id;
        p_ue_context->cgi_trigerred_for_earfcn = p_confused_cell_list->
            reported_neighbor_cell_info[RRM_ZERO].target_earfcn;
    }
    /* SPR 16406 21AUG Start */
    /* Code removed */
    /* SPR 16406 21AUG End */
    /* Coverity_fix_93542_start */
    ret_val = RRM_SUCCESS;
    /* Coverity_fix_93542_end */
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 16406 End */
/* Bug_944_start */
/****************************************************************************
 * Function Name  : rrm_find_first_less_load_rprtd_cell
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the A4 meas  report message received from RRC.
 ****************************************************************************/
U16
rrm_find_first_less_load_rprtd_cell (
    U8                    meas_id,
    U16                   phy_cell_id,
    rrm_ue_context_t      *p_rrm_ue_context)
{
    rrm_bool_et phy_cell_found_in_ncl                   = RRM_FALSE;
    U8          valid_cells                             = RRM_ZERO;
    U8 load_threshold                                   = RRM_ZERO;
    U8 tgt_cell_capacity                                = RRM_ZERO;
    U8 tgt_cell_capacity_ul                             = RRM_ZERO;
    U8 tgt_cell_capacity_dl                             = RRM_ZERO;
    U8 cell_count                                       = RRM_ZERO;
    rrm_bool_et  is_meas_id_found                       = RRM_FALSE;
    U16 rrm_prfed_arfcn_for_redirect                    = RRM_ZERO;
    rrm_best_cell_for_ue_node_t *p_temp                 = RRM_PNULL;
    rrm_cell_context_t    *p_cell_ctxt                  = RRM_PNULL;
    meas_report_info_node_t
        *p_meas_report_info                             = RRM_PNULL;
    lte_ncl_t    *p_lte_ncl                             = RRM_PNULL;
    YLNODE *p_node                                      = RRM_PNULL;
    rrm_best_cell_for_ue_node_t *p_best_cell_node       = RRM_PNULL;
    rrm_bool_et  is_new_arfcn_added_inlist              = RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_rrm_ue_context);

    p_cell_ctxt = p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_rrm_ue_context->cell_index];

    RRM_ASSERT (RRM_PNULL != p_cell_ctxt);
    p_node = ylFirst(&(p_rrm_ue_context->meas_report_list));

    while (p_node) 
    {
        p_meas_report_info = (meas_report_info_node_t*) p_node;
        if (p_meas_report_info->meas_id == meas_id) 
        {
            is_meas_id_found = RRM_TRUE;
            break;
        }
        p_node = ylNext(p_node);
    }
    if (is_meas_id_found != RRM_TRUE)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "measurement id:%d not yet configured",meas_id);
        /* coverity 57595 */
        RRM_UT_TRACE_EXIT();
        return RRM_ZERO;
    }
    /* Check if the neighbour info is currently present in the cell neighbour list */
    p_lte_ncl = rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
    if(p_lte_ncl != RRM_PNULL)
    {
        valid_cells = p_lte_ncl-> num_valid_inter_freq_cell;


        for(cell_count=RRM_ZERO; ((cell_count < valid_cells) && (cell_count < MAX_INTER_FREQ_CELLS))
                ; cell_count++)
        {
            if(p_lte_ncl->inter_freq_cells\
                    [cell_count].blacklisted == RRM_FALSE)
            {
                if ((p_rrm_ue_context->carrier_earfcn == p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn) &&
                   (phy_cell_id == p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id))
                {
                    phy_cell_found_in_ncl = RRM_TRUE;
                    break;

                }

            }
        }
    }

    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "p_lte_ncl is NULL");
    }
    if (RRM_TRUE == phy_cell_found_in_ncl)
    {
        load_threshold = rrm_find_load_threshold (p_cell_ctxt);    
        if(p_lte_ncl->inter_freq_cells[cell_count].resrc_report.bitmask &
                RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT)
        {
            tgt_cell_capacity_ul =  p_lte_ncl->inter_freq_cells[cell_count].
                resrc_report.comp_avl_cap_grp.ul.cell_cap_val;
            tgt_cell_capacity_dl =  p_lte_ncl->inter_freq_cells[cell_count].
                resrc_report.comp_avl_cap_grp.dl.cell_cap_val;
        }
        else
        {
            tgt_cell_capacity_ul = RRM_MAX_AVLB_CAPCTY;
            tgt_cell_capacity_dl = RRM_MAX_AVLB_CAPCTY;
        }

        tgt_cell_capacity = RRM_GET_MIN(tgt_cell_capacity_dl,tgt_cell_capacity_ul);
        if (tgt_cell_capacity > load_threshold)
        {
            rrm_prfed_arfcn_for_redirect = p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn;
            RRM_UT_TRACE_EXIT();
            return rrm_prfed_arfcn_for_redirect;
        }

        /*
           If freq is loaded save it in the queue in ascending order 
         */
        p_best_cell_node = (rrm_best_cell_for_ue_node_t*)rrm_mem_get(sizeof(rrm_best_cell_for_ue_node_t));
        if(RRM_PNULL != p_best_cell_node)
        {
            if (RRM_ZERO != ylCount(&p_rrm_ue_context->best_cell_for_ue))
            {
                p_temp = (rrm_best_cell_for_ue_node_t *)ylFirst(&p_rrm_ue_context->best_cell_for_ue);
                while (RRM_PNULL != p_temp)
                {
                    if (tgt_cell_capacity < p_temp->tgt_cell_capacity)
                    {
                        RRM_MEMSET(p_best_cell_node,RRM_ZERO,sizeof(rrm_best_cell_for_ue_node_t));
                        p_best_cell_node->tgt_cell_capacity = tgt_cell_capacity;
                        p_best_cell_node->earfcn = p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn;
                        p_best_cell_node->meas_ind_rcvd = RRM_TRUE;
                        p_best_cell_node->pci_found = phy_cell_found_in_ncl;
                        ylInsertBefore(&p_rrm_ue_context->best_cell_for_ue,(YLNODE *)(&p_temp->p_node),
                                (YLNODE *)(&p_best_cell_node->p_node));
                        is_new_arfcn_added_inlist = RRM_TRUE;
                        break;
                    }
                    p_temp = (rrm_best_cell_for_ue_node_t *)(ylNext(&p_temp->p_node));
                }
                if(RRM_FALSE == is_new_arfcn_added_inlist)
                {
                    RRM_MEMSET(p_best_cell_node,RRM_ZERO,sizeof(rrm_best_cell_for_ue_node_t));
                    p_best_cell_node->tgt_cell_capacity = tgt_cell_capacity;
                    p_best_cell_node->earfcn = p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn;
                    p_best_cell_node->meas_ind_rcvd = RRM_TRUE;
                    p_best_cell_node->pci_found = phy_cell_found_in_ncl;
                    ylPushHead(&p_rrm_ue_context->best_cell_for_ue,(YLNODE *)(&p_best_cell_node->p_node));
                }

            }
            else
            {
                /* List was empty so put node on head */
                RRM_MEMSET(p_best_cell_node,RRM_ZERO,sizeof(rrm_best_cell_for_ue_node_t));
                p_best_cell_node->tgt_cell_capacity = tgt_cell_capacity;
                p_best_cell_node->earfcn = p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn;
                p_best_cell_node->meas_ind_rcvd = RRM_TRUE;
                p_best_cell_node->pci_found = phy_cell_found_in_ncl;
                ylPushHead(&p_rrm_ue_context->best_cell_for_ue,(YLNODE *)(&p_best_cell_node->p_node));
            }
        }
        else
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "Memory allocation to p_best_cell_node failed");
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "PCI [%d] not found in NCL for freq[%u]",
                phy_cell_id, p_rrm_ue_context->carrier_earfcn);
    }
    RRM_UT_TRACE_EXIT();
    return RRM_ZERO;
}
/* Bug_944_end */
/* SPR 16406 Start */
/* Code Removed */
/* SPR 16406 End */

/* SPR 16406 Start */
/* Inter RAT changes start */
/****************************************************************************
 * Function Name  : rrm_ue_handle_inter_RAT_meas_event 
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the B1/B2 meas report message received from RRC.
 ****************************************************************************/
rrm_return_et
rrm_ue_handle_inter_RAT_meas_event(
		rrm_ue_global_context_t *p_g_rrm_ue_ctx)
{
    rrm_ue_context_t*            p_ue_context = RRM_PNULL;
    rrm_return_et                ret_val      = RRM_SUCCESS;
    rrm_ho_params_t*             p_ho_params  = RRM_PNULL;
    /* SPR 15674 Start */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t           phy_cell_id_info  = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};     
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 15674 End */
    U16                          num_utra_cells = RRM_ZERO;
    U8                          num_geran_cells = RRM_ZERO;
    U8                           i               = RRM_ZERO;
    U8                           ps_ho_flag = RRM_ZERO;

    rrm_ue_measurment_results_ind_t*	p_proc_data = RRM_PNULL;
    cell_selection_priority_list_t*    	p_utra_cell_list = RRM_PNULL;
    cell_selection_priority_list_t*    	p_geran_cell_list = RRM_PNULL;
    U8                             		transaction_id = RRM_ZERO;
    rrm_cell_context_t *p_cell_ctx = RRM_PNULL;
    rrm_bool_et  is_daho_present = RRM_FALSE;
    /* SPR-17852 START */
    /* code deleted  */ 
    /* SPR-17852 END */
    rrm_best_cell_for_ue_node_t *p_best_cell    = RRM_NULL;
    /*csg start*/
    /*coverity 23054:removing "target_cell_typ":simerjit,rel 1.3*/

    /*coverity 23056:removing "ue_mbrshp_status_trget_cell":simerjit,rel 1.3*/
    /*csg end*/

    /*start csfb tgt selection*/
    rrm_ho_or_release_et rrm_decison;
    /*end csfb tgt selection*/

    /* SPR 19364 start */
    /* Code removed */
    /* SPR 19364 end */

    /* Start fix for Bug 369 */
    U16 geran_pci = INVALID_PHYSICAL_CELL_ID;
    /* End fix for Bug 369 */

    RRM_UT_TRACE_ENTER();

    p_ue_context = p_g_rrm_ue_ctx->p_ue_context;
    p_ho_params = (rrm_ho_params_t*)&p_ue_context->ho_params;

    /* SPR-17852 START */
    /* code deleted */
    /* SPR-17852 END */
    p_proc_data = (rrm_ue_measurment_results_ind_t *)
        p_g_rrm_ue_ctx->p_ue_context->current_procedure.p_proc_data;



    /*Start: BUG 974*/
    if (RRM_CARRIER_REDIRECT_ON_UE == p_g_rrm_ue_ctx->p_ue_context->action_triggered)
    {
        RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_DETAILED,
                "Previous action on UE was = %d, new action = %d",
                p_g_rrm_ue_ctx->p_ue_context->action_triggered,
                RRM_HO_ACTN_ON_UE);
        p_g_rrm_ue_ctx->p_ue_context->action_triggered = RRM_HO_ACTN_ON_UE;
        /* SPR-17852 START */
        if(p_ue_context->carrier_redirect_ue)
        {
            /* SPR-17852 END */
            /* Stop carrier_redirect_ue Timer */
            RRM_TRACE (g_uem_log_on_off, p_g_rrm_uem_facility_name,
                    RRM_DETAILED, "Stop carrier_redirect_ue timer");
            /* CSR-58333-fix start*/
            rrm_ue_stop_timer(p_ue_context->carrier_redirect_ue);
            /* CSR-58333-fix end*/
            p_ue_context->carrier_redirect_ue = RRM_PNULL;
            while(RRM_PNULL != (p_best_cell = (rrm_best_cell_for_ue_node_t *)ylPopHead(&p_ue_context->best_cell_for_ue)))
            {
                RRM_MEM_FREE(p_best_cell);
            }
        }
    }
    /*End: BUG 974*/

    num_utra_cells = p_proc_data->utran_count;	
    num_geran_cells = p_proc_data->geran_count;	
    p_utra_cell_list = &(p_ho_params->utra_known_cell_selection_list);
    p_geran_cell_list = &(p_ho_params->geran_known_cell_selection_list);
    transaction_id = p_g_rrm_ue_ctx->trans_id;
    /*SPR 22571 Fix Start*/
    /*Code Removed*/
    /*SPR 22571 Fix Stop*/

    /* Bug 735 Fix START */
    /*CID 65792:start*/
    /*CID 65792:end*/
    /* Bug 735 Fix END */
    if(p_ho_params->csfb_blind_timer)
    {
        /* If CSFB blind timer is running then stop it as we got the inter RAT
         * MR */
        rrm_ue_stop_timer(p_ho_params->csfb_blind_timer);
        p_ho_params->csfb_blind_timer = RRM_PNULL;
    }

    /* Process the B1/B2 event */
    /*SPR_17893_START*/
    if(RRM_ZERO < num_utra_cells )
    {
        RRM_MEMSET(p_utra_cell_list,RRM_ZERO,
                sizeof(cell_selection_priority_list_t));
    }

    if(RRM_ZERO <num_geran_cells )
    {
        RRM_MEMSET(p_geran_cell_list,RRM_ZERO,
                sizeof(cell_selection_priority_list_t));
    }
    /*SPR_17893_END*/

    /* SPR 16406 8SEP start */
    p_ue_context->ho_params.last_reported_meas_event = \
                                                       /*Cov 93548 +*/
                                                       (rrmc_meas_report_event_et)p_ue_context->meas_rep_event;
    /*Cov 93548 -*/
    /* SPR 16406 8SEP end */

    ps_ho_flag = rrm_cellm_get_ps_ho_flag(p_ue_context->cell_index);

    /* SPR 15598 start */
    if((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state) &&
            /* SPR 15598 end */
            (p_ue_context->cdma2000_capabilities.is_cdma2000_present) &&
            ((RRM_TRUE == p_ue_context->cdma2000_meas_id_received) ||
             (RRM_IRAT_PRIORITY_CDMA2000 == get_next_priority_rat(RRM_ZERO, p_ue_context))))
    {
        /* For Release with Redirection to CDMA2000 Cell */
        if(RRM_FALSE == ps_ho_flag)
        {
            /* Stop all the timers running */
            rrm_ue_stop_ho_timers(p_ue_context);

            /* Bug 16121 Fix Start */
            if(RRM_SUCCESS ==  rrm_build_and_send_ue_connection_release_ind(
                        p_ue_context->ue_index,
                        RADIO_NETWORK_LAYER_TYPE,
                        CS_FALLBACK_TRIGGERED,
                        p_g_rrm_ue_ctx->trans_id,
                        p_ue_context,
                        p_ue_context->cell_index))
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "ue connection release ind sent for [UE:%d]",p_ue_context->ue_index);
                if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                {
                    RRM_TRACE(g_uem_log_on_off,
                            p_rrm_uem_facility_name,
                            RRM_DETAILED,
                            "ue_connection_release_ind sent, \
                            Failed to clean up ue resources");
                    ret_val = RRM_FAILURE;
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "Failed to send release connection ind for [UE:%d]",
                        p_ue_context->ue_index);
                ret_val = RRM_FAILURE;
            }
            /* Bug 16121 Fix End */

        }
        else if(RRM_TRUE == ps_ho_flag)
        {
            /* Handover Called to CDMA2000 */
            /* SPR 15674 Start */
            ret_val = rrm_build_and_send_ho_required (
                    phy_cell_id_info,
                    p_ue_context,
                    transaction_id);
            /* SPR 15674 End */
        }
    }
    else
    {
        p_cell_ctx = rrm_cellm_get_cell_context(p_ue_context->cell_index);
        /*CID 65792:start*/
        if (RRM_PNULL != p_cell_ctx)
        {
            if (p_cell_ctx->operator_info.bitmask & RRMCM_RMIF_DAHO_CELL_LIST)
            {
                is_daho_present = RRM_TRUE;
            }
        }
        /*CID 65792:end*/
        /*Cov_fix_start_55371*/
        if(RRM_ZERO < num_utra_cells || (RRM_ZERO < num_geran_cells) ||
                (is_daho_present))
        {
            /* SPR 17108 FIX START*/
            p_ue_context->rrm_cgi_triggered_for_rat_type = NO_RAT_CONFIGURED;
            if(RRM_ZERO < num_utra_cells )
            {
                p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_UTRAN_KNOWN_CELL_LIST;
            }
            if(RRM_ZERO < num_geran_cells)
            {
                p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_GERAN_KNOWN_CELL_LIST;
            }
            /* SPR 17108 FIX END*/
            /* Coverity 21oct Start : 54997 */
            for (i=0;(i<num_utra_cells) && (i < RRM_MAX_CELLS_REPORTED);i++)
                /* Coverity 21oct End : 54997 */
            {
                if(p_proc_data->utran_neighbor_list[i].pci.bitmask & \
                        RRM_PCI_FDD_PRESENT)
                {
                    /*start LTE RRM Mobility-13*/
                    /*If daho pci is reported do not store it,because
                      during ho DAHO pci will be considered in the end*/
                    /*end LTE RRM Mobility-13*/
                    if(RRM_FAILURE == rrm_match_with_utran_daho_pci(
                                p_ue_context,
                                p_proc_data->utran_neighbor_list[i].pci.pci_fdd))
                    {
                        continue;
                    }
                    /*end LTE RRM Mobility-13*/
                    /* SPR 21614 start */
                    if (RRM_TRUE != rrm_is_update_required_in_cell_select(p_cell_ctx, &p_proc_data->utran_neighbor_list[i].pci))
                    {
                        continue;
                    }
                    /* SPR 21614 end */


                    /* Bug 9800 Fix Start */
                    /* SPR 15232 start */
                    p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].phy_cell_id = p_proc_data->\
                                                                                                         /* SPR 15232 end */
                                                                                                         utran_neighbor_list[i].pci.pci_fdd;
                    /* SPR 16406 24Sep Start */
                    p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].reported_count = RRM_ONE; 
                    /* SPR 16406 24Sep End */
                    /* SPR 15674 Start */
                    p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].target_earfcn = 
                        p_ue_context->carrier_earfcn;
                    /* SPR 15674 End */
                    /* Fill the Meas Result based on Trigger Quantity  */
                    /* CID 21686 FIX */
                    if ((quantity_config_utra_meas_fdd_et)p_ue_context->trigger_quantity == 0 ) {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].rsrx_value = \
                                                                                                            /* SPR 15232 end */
                                                                                                            p_proc_data->utran_neighbor_list[i].rscp;
                    }	 
                    else
                    {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].rsrx_value = \
                                                                                                            /* SPR 15232 end */
                                                                                                            p_proc_data->utran_neighbor_list[i].ecno;
                    }
                    /*csg start*/
                    if(p_proc_data->utran_neighbor_list[i].bitmask & RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT)
                    {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].csg_membership_status = 
                            /* SPR 15232 end */
                            p_proc_data->utran_neighbor_list[i].csg_membershipStatus;
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].csg_membership_status_presence = RRM_TRUE;
                        /* SPR 15232 end */
                    }
                    else
                    {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].csg_membership_status = RRM_NOT_MEMBER;
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].csg_membership_status_presence = RRM_FALSE;
                        /* SPR 15232 end */
                    }

                    if(p_proc_data->utran_neighbor_list[i].bitmask & RRM_SON_UTRAN_NC_CSG_PRESENT)
                    {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].csg_id_presence = RRM_TRUE;
                        /* SPR 15232 end */
                    }
                    else
                    {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].csg_id_presence = RRM_FALSE;
                        /* SPR 15232 end */
                    }
                    /*csg end*/
                    p_utra_cell_list->count++;
                }
                /* Start fix for bug 715 */
                if(p_proc_data->utran_neighbor_list[i].pci.bitmask & \
                        RRM_PCI_TDD_PRESENT)
                {
                    /*start LTE RRM Mobility-13*/
                    /*If daho pci is reported do not store it,because
                      during ho DAHO pci will be considered in the end*/
                    if(RRM_FAILURE == rrm_match_with_utran_daho_pci(
                                p_ue_context,
                                p_proc_data->utran_neighbor_list[i].pci.pci_tdd))
                    {
                        continue;
                    }
                    /*end LTE RRM Mobility-13*/
                    /* SPR 15232 start */
                    p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].phy_cell_id = p_proc_data->\
                                                                                                         /* SPR 15232 end */
                                                                                                         utran_neighbor_list[i].pci.pci_tdd;
                    /* SPR 15674 Start */
                    p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].target_earfcn = 
                        p_ue_context->carrier_earfcn;
                    /* SPR 15674 End */
                    /* Fill the Meas Result based on Trigger Quantity  */
                    /* Coverity_ID : 21686 */
                    if ((quantity_config_utra_meas_tdd_et)p_ue_context->trigger_quantity == 0 /* RRM_RRC_PCCPCH_RSCP */)
                    {
                        /* SPR 15232 start */
                        p_utra_cell_list->reported_neighbor_cell_info[p_utra_cell_list->count].rsrx_value = \
                                                                                                            /* SPR 15232 end */
                                                                                                            p_proc_data->utran_neighbor_list[i].rscp;
                    }
                    /* Bug 9800 Fix End */
                    p_utra_cell_list->count++;
                }
                /* End fix for bug 715 */
            }
            /* GERAN-HO changes start */
            if(num_geran_cells)
            {
                RRM_MEMSET (p_geran_cell_list,0,
                        sizeof(cell_selection_priority_list_t));
                for (i=0;((i<num_geran_cells) && (i < RRM_MAX_CELLS_REPORTED));i++)  /* Coverity: CID 54997 */
                {
                    /* Start fix for Bug 369 */
                    geran_pci = \
                                rrm_ue_get_geran_pci(
                                        p_ue_context,
                                        p_proc_data->geran_neighbor_list[i].pci.ncc,
                                        p_proc_data->geran_neighbor_list[i].pci.bscc);
                    /*SPR 17777 +-*/
                    /* SPR 15232 start */
                    p_geran_cell_list->reported_neighbor_cell_info[i].phy_cell_id = geran_pci;
                    /* SPR 15232 end */
                    /* SPR 16406 24Sep Start */
                    p_geran_cell_list->reported_neighbor_cell_info[i].reported_count = RRM_ONE;
                    /* SPR 16406 24Sep End */
                    /* SPR 15674 Start */
                    p_geran_cell_list->reported_neighbor_cell_info[i].target_earfcn = 
                        p_ue_context->carrier_earfcn;
                    /* SPR 15674 End */
                    /* End fix for Bug 369 */
                    /* Fill the Meas Result based on Trigger Quantity  */
                    /* Coverity: CID 21686: This may be correct in future if  
                     **   another enumuration added in quantity_config_utra_meas_geran_et
                     */
                    if (p_ue_context->trigger_quantity == 0 /*RRM_RRC_RSSI*/) {
                        /* SPR 15232 start */
                        p_geran_cell_list->reported_neighbor_cell_info[i].rsrx_value = p_proc_data->geran_neighbor_list[i].rssi;
                        /* SPR 15232 end */
                    }	 
                    p_geran_cell_list->count++;
                }
            } 
            /*BLR:SPID CHANGES START*/ 
            if( p_utra_cell_list->count || p_geran_cell_list->count)
                /* GERAN-HO changes end */
            {
                /* SPR 22553 FIXED START*/
                if(p_ue_context->rrm_sub_profile_id_info.sub_profile_id)
                /* SPR 22553 FIXED END*/    
                {
                    rrm_ue_prioritize_inter_rat_meas_result(p_ue_context);
                }
            }
            /*BLR:SPID CHANGES END*/
        }
        else /* CDMA2000 Leg. - if no UTRAN/GERAN Cells (including DAHO Cells) are present */
        {
            /* HO to HRPD Cell, if B1 is received & no UTRAN/GERAN Cell is present */
            /* SPR 15598 start */
            if(RRM_UE_STATE_ACTIVE == p_ue_context->ue_state)
            {
                /* SPR 15598 end */
                if(p_ue_context->cdma2000_capabilities.is_cdma2000_present)
                {
                    ret_val = rrm_build_and_send_cdm2000_ho_required (
                            p_ue_context,
                            transaction_id);
                    if (RRM_SUCCESS == ret_val)
                    {
                        UEM_FSM_SET_STATE(p_ue_context, RRM_UE_STATE_HO_ONGOING);
                        /*SPR 22571 Fix Start*/
                        RRM_TRACE(g_cellm_log_on_off,
                                p_g_rrm_uem_facility_name,
                                RRM_INFO,
                                "Triggering HO on CDMA2000 Cell");
                    }
                }
                else
                {
                    RRM_TRACE(g_cellm_log_on_off,
                            p_g_rrm_uem_facility_name,
                            RRM_INFO,
                            "No Better cell found");
                    /*SPR 22571 Fix Stop*/
                    p_ue_context->ue_state = RRM_UE_STATE_ACTIVE;
                }
                RRM_UT_TRACE_EXIT();
                return ret_val; /* Intentionaly returning as for CDMA2000 no need
                                   to proceed further. Those are the cases of CCO/RWR */
                /* SPR 15598 start */
            }
            /* SPR 15598 end */
        }
        /*csg start*/
        /* SPR 15598 start */
        /*SPR_17893_START*/
        if((RRM_UE_STATE_ACTIVE ==  p_ue_context->ue_state) ||
                (RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES == p_ue_context->ue_state))
        {
            /*SPR_17893_END*/
            /* SPR 15598 end */
            /* SPR 15674 Start */
            if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
                    == RRM_MEMBER)
            {
                /*As ue is csg member try to ho it to a closed cell*/
                phy_cell_id_info = rrm_get_best_suitable_cell_for_ho(p_ue_context,
                        RRM_OAM_ACCESS_MODE_CLOSE);
            }
            else if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
                    == RRM_NOT_MEMBER)
            {
                /*As ue is csg member try to ho it to a closed cell*/
                phy_cell_id_info = rrm_get_best_suitable_cell_for_ho(p_ue_context,
                        RRM_OAM_ACCESS_MODE_OPEN);
            }
            /*csg end*/

            /*CCO Changes begin*/
            /* SPR_16063_Fix: Start */
            /* In case where CSFB was received from MME & B1 was configured & before B1 measurement report received from
             * UE A2 is received from UE side, then RRM configures B2 event too, So in case B2 event is received from UE first
             * then code will go into this leg regardless which event was received from UE, coz in case B2 was received before B1
             * then RRM shall trigger HO towards the reported Inter-rat cell. This will also avoid dependency on PS_HO_ENABLED
             * flag that should be applicable for CSFB only. */
            /* SPR 16406 8SEP start */
            if((p_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD) && 
                    (MEAS_REP_EVENT_B1 == p_ue_context->meas_rep_event))
                /* SPR 16406 8SEP end */
                /* SPR_16063_Fix: End */    
            {
                /* SPR 19364 start */
                /* Code removed */
                /* SPR 19364 end */
                if(RRM_TRUE == ps_ho_flag)
                {
                    /* SPR 13860 Fix Start */
                    if(phy_cell_id_info.phy_cell_id == INVALID_PHYSICAL_CELL_ID)
                    {
                        rrm_decison = RRM_DO_RELEASE_REDIRECT;
                    }
                    else
                    {
                        rrm_decison = RRM_DO_HO;
                    }
                    /* SPR 13860 Fix End */
                }
                /* SPR 19364 start */
                else if ((INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id) &&
                        (RRM_TRUE == rrm_ue_check_for_cco(p_ue_context, &phy_cell_id_info)))
                {
                    /* SPR 19364 end */
                    rrm_decison = RRM_DO_HO;
                }
                else
                {
                    rrm_decison = RRM_DO_RELEASE_REDIRECT;
                }
            }
            else
            {
                rrm_decison = RRM_DO_HO;
            }
            /*CCO Changes end*/
            if(RRM_DO_HO == rrm_decison)
            {
                /* SPR 16406 8SEP start */
                /* Code removed */
                /* SPR 16406 8SEP end */

                if(phy_cell_id_info.phy_cell_id != INVALID_PHYSICAL_CELL_ID)
                {

                    /*SPR_17893_START*/
                    if((UTRA_CELL == p_ue_context->target_cell_type) &&
                            !(RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT & p_ue_context->ue_capability_params.bitmask))
                    {    
                        if((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state))
                        {
                            rrm_populate_and_send_utran_cap_enq_req(p_ue_context);
                            rrm_ue_set_state(p_ue_context,RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES);
                            rrm_set_utran_capability_enq_info(p_ue_context,RRM_TRUE,RRM_FALSE);
                        }
                        /*Move Back one index,if the valid id is found*/
                        if((RRM_ZERO < p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx) && 
                                (p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx <= 
                                 p_ue_context->ho_params.utra_known_cell_selection_list.count))
                        {
                            p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx--; 
                        }
                        /*Undo the bitmask,to check if DAHO cell is selected */
                        if(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED)
                        {
                            p_ue_context->ho_params.utra_known_cell_selection_list.bitmask &= ~RRM_UE_BLIND_CELL_HO_TRIGGERED;  
                        }    

                        p_ue_context->is_ho_reqd_on_recv_utan_cap  = RRM_TRUE;
                        RRM_TRACE(g_cellm_log_on_off,p_g_rrm_uem_facility_name,
                                RRM_DETAILED,"[UE:%d],has been asked for UTRAN Capability "
                                "No HO is to be triggred ",p_ue_context->ue_index);
                    }
                    else
                    {    
                        /*SPR_17893_END*/
                        ret_val = rrm_build_and_send_ho_required (
                                phy_cell_id_info,
                                p_ue_context,
                                transaction_id);    
                        if (RRM_SUCCESS == ret_val) {
                            /* SPR 16406 8SEP start */
                            /* Code removed */
                            /* SPR 16406 8SEP end */
                            RRM_TRACE(g_cellm_log_on_off,
                                    p_g_rrm_uem_facility_name, 
                                    RRM_DETAILED, 
                                    "HO triggered towards physicall cell id:[%d] "
                                    "for [UE:%d]", phy_cell_id_info.phy_cell_id,
                                    p_ue_context->ue_index); 
                        }
                        /*SPR_17893_START*/
                        else if(RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES != p_ue_context->ue_state)
                        {    
                            /*SPR_17893_END*/
                            UEM_FSM_SET_STATE(p_ue_context, RRM_UE_STATE_ACTIVE);
                        }
                        /*SPR_17893_START*/
                    }
                    /*SPR_17893_END*/
                }
                else
                {
                    /* There are no valid cells reported from UE */
                    ret_val = RRM_FAILURE;
                }
            }/*end of if RRM_DO_HO*/
            else if(RRM_DO_RELEASE_REDIRECT == rrm_decison)
            {
                p_ho_params->target_phys_cell_id = phy_cell_id_info.phy_cell_id;
                p_ho_params->target_earfcn = phy_cell_id_info.carr_earfcn;
                /* SPR 15674 End */
                if(RRM_FAILURE == rrm_build_and_send_ue_connection_release_ind(
                            p_ue_context->ue_index,
                            RADIO_NETWORK_LAYER_TYPE,
                            CS_FALLBACK_TRIGGERED,
                            p_g_rrm_ue_ctx->trans_id,
                            p_ue_context,
                            /* SPR_10729_fix */
                            p_ue_context->cell_index))
                {
                    RRM_TRACE(g_uem_log_on_off,
                            p_g_rrm_uem_facility_name,
                            RRM_ERROR,
                            "[UE:%d]: Failure Received While Sending"
                            " ue_connection_release_ind",
                            p_ue_context->ue_index);
                    ret_val = RRM_FAILURE;
                }
                else
                {
                    if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_g_rrm_uem_facility_name,
                                RRM_DETAILED,
                                "ue_connection_release_ind sent, \
                                Failed to clean up ue resources");
                        ret_val = RRM_FAILURE;

                    }
                }
            }
            /* SPR 15598 start */
        }
        /* SPR 15598 end */
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
    /*Coverity_fix_end_55371*/
}
/* SPR 16406 End */
/* Inter RAT changes end */

/* SPR_15712_Fix: Start */
/* SPR 16406 8SEP start */
/****************************************************************************
 * Function Name  : rrm_check_cgi_of_eutran_reported_cell 
 * Inputs         : p_ue_context,pci_for_which_cgi_required,carrier_earfcn
 * Outputs        : p_ho_cell_info
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : RRM_CGI_REP_CAUSE_KNOWN_PCI->PCI is known to eNB
 *                  RRM_CGI_REP_CAUSE_NEW_PCI->PCI is not found on eNB
 *                  RRM_CGI_REP_CAUSE_PCI_CONFUSION->PCI is confused on eNB
 ****************************************************************************/
/*SPR 17527 START*/
rrm_cgi_trigger_cause_et rrm_check_cgi_of_eutran_reported_cell(
        rrm_ue_context_t *p_ue_context,
        U16               pci_for_which_cgi_required,
        U16               carrier_earfcn,
        rrm_oam_eutran_global_cell_id_t *p_rcvd_cgi,
        phy_cell_id_info_t *p_ho_cell_info,
        rrm_bool_et    *p_cgi_present_in_unknwn_list_flag)
/*SPR 17527 END*/
{
    /*Coverity 93537_9355 Fix Start*/
    rrm_cgi_trigger_cause_et        ret_val            = RRM_CGI_REP_CAUSE_NEW_PCI;
    /*Coverity 93537_93559 Fix End*/
    lte_ncl_t    *p_lte_ncl       = RRM_PNULL;
    U8          cell_count        = RRM_ZERO;
    rf_params_t  *p_rf_params = RRM_PNULL; 
    U16                 j                   = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    /*SPR 17527 START*/
    *p_cgi_present_in_unknwn_list_flag = RRM_FALSE;
    /*SPR 17527 END*/


    p_lte_ncl = rrm_cellm_get_lte_ncl_info(p_ue_context->cell_index);

    p_ho_cell_info->phy_cell_id = INVALID_PHYSICAL_CELL_ID;
    p_ho_cell_info->carr_earfcn = RRM_ZERO;
    p_ho_cell_info->is_cell_blk_list = RRM_FALSE;
    p_ho_cell_info->is_cell_confused = RRM_FALSE;

    if(RRM_PNULL != p_lte_ncl)
    {
        p_rf_params = rrm_cellm_get_rf_params(p_ue_context->cell_index); 
        /* Intra Frequency */
        if (carrier_earfcn == 
                p_rf_params->rf_configurations.dl_earfcn)
        {
            for(cell_count=RRM_ZERO; cell_count < p_lte_ncl->num_valid_intra_freq_cell; cell_count++)
            {
                if(pci_for_which_cgi_required == p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id)
                {
                    if  (RRM_SUCCESS == compare_global_cell_id(p_rcvd_cgi, &(p_lte_ncl->intra_freq_cells[cell_count].cell_id)))
                    {
                        /* Check if blacklist cell's CGI is reported then HO shall not be triggered */
                        if(RRM_ONE == p_lte_ncl->intra_freq_cells[cell_count].blacklisted)
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Intra frequency Neighbor cell with PCI= %d, is Blacklisted",pci_for_which_cgi_required);
                            p_ho_cell_info->is_cell_blk_list = RRM_TRUE;
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Intra frequency Neighbor cell with PCI= %d, is White listed",pci_for_which_cgi_required);

                        }
                        p_ho_cell_info->phy_cell_id = pci_for_which_cgi_required;
                        p_ho_cell_info->carr_earfcn = carrier_earfcn;
                        ret_val = RRM_CGI_REP_CAUSE_KNOWN_PCI;
                        /* PCI is known now check that if it is confused PCI or not */
                        /* SPR 16406 8SEP start */
                        for (j = RRM_ZERO; j < p_ue_context->ho_params.eutra_confused_cell_selection_list.count; j++)
                            /* SPR 16406 8SEP end */
                        {
                            if (p_ue_context->ho_params.eutra_confused_cell_selection_list.reported_neighbor_cell_info[j].phy_cell_id == pci_for_which_cgi_required)
                            {
                                ret_val = RRM_CGI_REP_CAUSE_PCI_CONFUSION;
                                p_ho_cell_info->is_cell_confused = RRM_TRUE;
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                        "Intra frequency Neighbor cell with PCI= %d, is confused", pci_for_which_cgi_required);
                                /* SPR 16406 18sep Start */
                                break;
                                /* SPR 16406 18sep End */
                            }
                        }
                        break;
                    }
                }
            }
        }
        /* Inter Frequency */
        else
        {
            for(cell_count=RRM_ZERO; cell_count < p_lte_ncl-> num_valid_inter_freq_cell; cell_count++)
            {
                if((pci_for_which_cgi_required == p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id) && 
                        (carrier_earfcn == p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn))
                {
                    if  (RRM_SUCCESS == compare_global_cell_id(p_rcvd_cgi, &(p_lte_ncl->inter_freq_cells[cell_count].cell_id)))
                    {
                        /* Check if blacklist cell's CGI is reported then HO shall not be triggered */
                        if(RRM_ONE == p_lte_ncl->inter_freq_cells[cell_count].blacklisted)
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Inter frequency Neighbor cell with PCI= %d, is Blacklisted",pci_for_which_cgi_required);
                            p_ho_cell_info->is_cell_blk_list = RRM_TRUE;
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Inter frequency Neighbor cell with PCI= %d, is White listed",pci_for_which_cgi_required);

                        }
                        p_ho_cell_info->phy_cell_id = pci_for_which_cgi_required;
                        p_ho_cell_info->carr_earfcn = carrier_earfcn;
                        ret_val = RRM_CGI_REP_CAUSE_KNOWN_PCI;
                        /* SPR 16406 8SEP start */
                        for (j = RRM_ZERO; j < p_ue_context->ho_params.eutra_confused_cell_selection_list.count; j++)
                            /* SPR 16406 8SEP end */
                        {
                            if (p_ue_context->ho_params.eutra_confused_cell_selection_list.reported_neighbor_cell_info[j].phy_cell_id == pci_for_which_cgi_required)
                            {
                                ret_val = RRM_CGI_REP_CAUSE_PCI_CONFUSION;
                                p_ho_cell_info->is_cell_confused = RRM_TRUE;
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                        "Inter frequency Neighbor cell with PCI= %d, is confused", pci_for_which_cgi_required);
                                /* SPR 16406 18sep Start */
                                break;
                                /* SPR 16406 18sep End */
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "EUTRA NCL in Empty!")
    }
    /*SPR 17527 START*/
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
            "p_ue_context->ho_params.eutra_unknown_cell_selection_list.count = %d",p_ue_context->ho_params.eutra_unknown_cell_selection_list.count);
    if(ret_val == RRM_CGI_REP_CAUSE_NEW_PCI)
    {   
        /* CID 99445 Fix Start */
        for (j = RRM_ZERO; j < p_ue_context->ho_params.eutra_unknown_cell_selection_list.count; j++)
        /* CID 99445 Fix End */
        {
            if ((p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info[j].phy_cell_id == pci_for_which_cgi_required)&&
                /*SPR 18133 Fix Start*/
                    (carrier_earfcn == p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info[j].target_earfcn))
                /*SPR 18133 Fix Stop*/
            {
                *p_cgi_present_in_unknwn_list_flag = RRM_TRUE;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "SAC p_cgi_present_in_unknwn_list_flag == TRUE");
            }
        }
        /*SPR 18133 Fix Start*/
        if(RRM_FALSE == *p_cgi_present_in_unknwn_list_flag)
        {
            for (j = RRM_ZERO; j < p_ue_context->ho_params.eutra_confused_cell_selection_list.count; j++)
            {
                if ((p_ue_context->ho_params.eutra_confused_cell_selection_list.reported_neighbor_cell_info[j].phy_cell_id == pci_for_which_cgi_required)&&
                        (carrier_earfcn == p_ue_context->ho_params.eutra_confused_cell_selection_list.reported_neighbor_cell_info[j].target_earfcn))
                {
                    *p_cgi_present_in_unknwn_list_flag = RRM_TRUE;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "SAC p_cgi_present_in_unknwn_list_flag == TRUE for Confused List");
                }
            }
        }
        /*SPR 18133 Fix Stop*/
    }
    /*SPR 17527 END*/
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 16406 8SEP end */
/* SPR_15712_Fix: End */
/* SPR 16406 Start */
/****************************************************************************
 * Function Name  : rrm_fill_data_in_eutran_son_meas_results_ind 
 * Inputs         : p_ue_context, p_cell_selection_priority_list
 * Outputs        : p_rrm_son_meas_results_ind 
 * Returns        : NONE 
 * Description    : Update p_rrm_son_meas_results_ind by taking info from current
 *                  cell_selection_list 
 ****************************************************************************/
rrm_void_t
rrm_fill_data_in_eutran_son_meas_results_ind
(
    rrm_ue_context_t *p_ue_context,
    cell_selection_priority_list_t *p_cell_selection_list,
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind    
)
{
    rrm_counter_t index = RRM_ZERO; 
    RRM_UT_TRACE_ENTER();
    if(p_cell_selection_list->count)
    {
        for(index=RRM_ZERO; index<p_cell_selection_list->count;index++)
        {
            if((p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id 
                        == p_ue_context->pci_for_which_cgi_required) && 
                    (p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn
                     == p_ue_context->carrier_earfcn))
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                        "CGI process triggered for PCI[%u] Earcfn[%u]",
                        p_ue_context->pci_for_which_cgi_required,
                        p_ue_context->carrier_earfcn);
                p_rrm_son_meas_results_ind->ue_index = p_ue_context->ue_index;

                if(p_cell_selection_list->reported_neighbor_cell_info[index].reason_event_type & MEAS_EVENT_A3_PRESENT)
                {
                    p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_A3;
                }
                else if (p_cell_selection_list->reported_neighbor_cell_info[index].reason_event_type & MEAS_EVENT_A4_PRESENT)
                {
                    p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_A4;
                }
                else if (p_cell_selection_list->reported_neighbor_cell_info[index].reason_event_type & MEAS_EVENT_A5_PRESENT)
                {
                    p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_A5;
                }
                else
                {
                    p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_NONE;
                }

                p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].earfcn = 
                    p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn;
                p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].neighbor_cell_list_size = RRM_ONE; 
                p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].
                    neighbor_cell_list[RRM_ZERO].pci = p_cell_selection_list->
                    reported_neighbor_cell_info[index].phy_cell_id;
                /* SPR 16406 Start */
                /* Code deleted */
                /* SPR 16406 end */

            }
        }
    }

    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_fill_data_in_utran_son_meas_results_ind 
 * Inputs         : p_ue_context, p_cell_selection_priority_list
 * Outputs        : p_rrm_son_meas_results_ind 
 * Returns        : NONE 
 * Description    : Update p_rrm_son_meas_results_ind by taking info from current
 *                  cell_selection_list 
 ****************************************************************************/
rrm_void_t
rrm_fill_data_in_utran_son_meas_results_ind
(
    rrm_ue_context_t *p_ue_context,
    cell_selection_priority_list_t *p_cell_selection_list,
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind    
)
{
    rrm_counter_t index = RRM_ZERO; 
    RRM_UT_TRACE_ENTER();
    if(p_cell_selection_list->count)
    {
        for(index=RRM_ZERO; index<p_cell_selection_list->count;index++)
        {
            if((p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id 
                        == p_ue_context->pci_for_which_cgi_required) && 
                    (p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn
                     == p_ue_context->carrier_earfcn))
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                        "CGI process triggered for PCI[%u] Earcfn[%u]",
                        p_ue_context->pci_for_which_cgi_required,p_ue_context->carrier_earfcn);
                p_rrm_son_meas_results_ind->ue_index = p_ue_context->ue_index;
                p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_NONE;

                p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].uarfcn = 
                    p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn;
                p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].neighbor_cell_list_size = RRM_ONE; 
            #ifndef TDD_MODE_FLAG
                p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].
                    neighbor_cell_list[RRM_ZERO].pci.pci_fdd = p_cell_selection_list->
                    reported_neighbor_cell_info[index].phy_cell_id;
            #else 
                p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].
                    neighbor_cell_list[RRM_ZERO].pci.pci_tdd = p_cell_selection_list->
                    reported_neighbor_cell_info[index].phy_cell_id;
            #endif
                /* SPR 16406 Start */
                /* Code removed */
                /* SPR 16406 end */
            }
        }
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_fill_data_in_geran_son_meas_results_ind 
 * Inputs         : p_ue_context, p_cell_selection_priority_list
 * Outputs        : p_rrm_son_meas_results_ind 
 * Returns        : NONE 
 * Description    : Update p_rrm_son_meas_results_ind by taking info from current
 *                  cell_selection_list 
 ****************************************************************************/
rrm_void_t
rrm_fill_data_in_geran_son_meas_results_ind
(
    rrm_ue_context_t *p_ue_context,
    cell_selection_priority_list_t *p_cell_selection_list,
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind    
)
{
    rrm_counter_t index = RRM_ZERO; 
    RRM_UT_TRACE_ENTER();
    if(p_cell_selection_list->count)
    {
        for(index=RRM_ZERO; index<p_cell_selection_list->count;index++)
        {
            if((p_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id 
                        == p_ue_context->pci_for_which_cgi_required) && 
                    (p_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn
                     == p_ue_context->carrier_earfcn))
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                        "CGI process triggered for PCI[%u] Earcfn[%u]",
                        p_ue_context->pci_for_which_cgi_required ,
                        p_ue_context->carrier_earfcn);
                p_rrm_son_meas_results_ind->ue_index = p_ue_context->ue_index;
                p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_NONE;

                p_rrm_son_meas_results_ind->geran_meas[RRM_ZERO].neighbor_cell_list_size = RRM_ONE; 
            }
        }
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_uem_search_pci_in_cell_selection_list_for_cgi 
 * Inputs         : p_ue_context 
 * Outputs        : NONE 
 * Returns        : pointer to cell_selection_priority_list_t type structure 
 * Description    : this function returns the pointer of cell selection list 
 *                  in which CGI processed cell is present
 ****************************************************************************/
cell_selection_priority_list_t *rrm_uem_search_pci_in_cell_selection_list_for_cgi
(
 rrm_ue_context_t *p_ue_context
 )
{

    rrm_counter_t index = RRM_ZERO;
    cell_selection_priority_list_t *p_confused_cell_selection_list = RRM_PNULL;
    cell_selection_priority_list_t *p_unknown_cell_selection_list = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_confused_cell_selection_list = &p_ue_context->ho_params.eutra_confused_cell_selection_list;
    p_unknown_cell_selection_list = &p_ue_context->ho_params.eutra_unknown_cell_selection_list;

    for(index = RRM_ZERO;index < (p_confused_cell_selection_list->count);index++)
    { 
        if((p_ue_context->pci_for_which_cgi_required == 
                    p_confused_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) &&
                (p_ue_context->carrier_earfcn == 
                 p_confused_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "CGI process triggered for PCI[%u] Earcfn[%u] from Eutra confused cell list",
                    p_ue_context->pci_for_which_cgi_required ,
                    p_ue_context->carrier_earfcn);
            RRM_UT_TRACE_EXIT();
            return p_confused_cell_selection_list;
        }
    }

    for(index = RRM_ZERO;index < (p_unknown_cell_selection_list->count);index++)
    { 
        if((p_ue_context->pci_for_which_cgi_required == 
                    p_unknown_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) &&
                (p_ue_context->carrier_earfcn == 
                 p_unknown_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "CGI process triggered for PCI[%u] Earcfn[%u] from Eutra unknown cell list",
                    p_ue_context->pci_for_which_cgi_required ,
                    p_ue_context->carrier_earfcn);
            RRM_UT_TRACE_EXIT();
            return p_unknown_cell_selection_list;
        }
    }

    p_confused_cell_selection_list = &p_ue_context->ho_params.utra_confused_cell_selection_list;
        p_unknown_cell_selection_list = &p_ue_context->ho_params.utra_unknown_cell_selection_list;

        for(index = RRM_ZERO;index < (p_confused_cell_selection_list->count);index++)
        { 
            if((p_ue_context->pci_for_which_cgi_required == 
                        p_confused_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) &&
                    (p_ue_context->carrier_earfcn == 
                     p_confused_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                        "CGI process triggered for PCI[%u] Earcfn[%u] from Utra confused cell list",
                        p_ue_context->pci_for_which_cgi_required ,
                        p_ue_context->carrier_earfcn);
                RRM_UT_TRACE_EXIT();
                return p_confused_cell_selection_list;
            }
        }

    for(index = RRM_ZERO;index < (p_unknown_cell_selection_list->count);index++)
    { 
        if((p_ue_context->pci_for_which_cgi_required == 
                    p_unknown_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) &&
                (p_ue_context->carrier_earfcn == 
                 p_unknown_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "CGI process triggered for PCI[%u] Earcfn[%u] from Utra unknown cell list",
                    p_ue_context->pci_for_which_cgi_required ,
                    p_ue_context->carrier_earfcn);
            RRM_UT_TRACE_EXIT();
            return p_unknown_cell_selection_list;
        }
    }

    p_confused_cell_selection_list = &p_ue_context->ho_params.geran_confused_cell_selection_list;
        p_unknown_cell_selection_list = &p_ue_context->ho_params.geran_unknown_cell_selection_list;

        for(index = RRM_ZERO;index < (p_confused_cell_selection_list->count);index++)
        { 
            if((p_ue_context->pci_for_which_cgi_required == 
                        p_confused_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) &&
                    (p_ue_context->carrier_earfcn == 
                     p_confused_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                        "CGI process triggered for PCI[%u] Earcfn[%u] from Geran confused cell list",
                        p_ue_context->pci_for_which_cgi_required ,
                        p_ue_context->carrier_earfcn);
                RRM_UT_TRACE_EXIT();
                return p_confused_cell_selection_list;
            }
        }
    for(index = RRM_ZERO;index < (p_unknown_cell_selection_list->count);index++)
    { 
        if((p_ue_context->pci_for_which_cgi_required == 
                    p_unknown_cell_selection_list->reported_neighbor_cell_info[index].phy_cell_id) &&
                (p_ue_context->carrier_earfcn == 
                 p_unknown_cell_selection_list->reported_neighbor_cell_info[index].target_earfcn))
        {
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                    "CGI process triggered for PCI[%u] Earcfn[%u] from Geran unknown cell list",
                    p_ue_context->pci_for_which_cgi_required ,
                    p_ue_context->carrier_earfcn);
            RRM_UT_TRACE_EXIT();
            return p_unknown_cell_selection_list;
        }
    }

    RRM_UT_TRACE_EXIT();
    return RRM_PNULL;
}


/****************************************************************************
 * Function Name  : rrm_update_meas_pointer_from_selection_list 
 * Inputs         : p_ue_context 
 * Outputs        : p_rrm_son_meas_results_ind 
 * Returns        : RRM_SUCCESS/RRM_FAILURE 
 * Description    : Update p_rrm_son_meas_results_ind by taking info from current
 *                  cell_selection_list 
 ****************************************************************************/
rrm_return_et
rrm_update_meas_pointer_from_selection_list
(
    rrm_ue_context_t *p_ue_context,
    rrm_son_meas_results_ind_t *p_rrm_son_meas_results_ind
)
{
    rrm_return_et ret_val = RRM_FAILURE;
    cell_selection_priority_list_t *p_cell_selection_list = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    /* Currently this function will only support eutra RAT */
    p_cell_selection_list = rrm_uem_search_pci_in_cell_selection_list_for_cgi(p_ue_context);

    if(RRM_PNULL == p_cell_selection_list)
    {
        RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                "No cell selection list is present for cgi procedure triggered");
        RRM_UT_TRACE_EXIT();
        return ret_val;
    }

    switch(p_ue_context->rrm_cell_selection_info.current_cell_list)
    {
        case RRM_EUTRAN_KNOWN_CELL_LIST: 
        case RRM_EUTRAN_UNKNOWN_CELL_LIST: 
        case RRM_EUTRAN_CONFUSED_CELL_LIST: 
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                        "Either Eutran unknown or confused cell selection list" 
                        "is selected for cgi procedure triggered");
                p_rrm_son_meas_results_ind->eutran_meas_list_size = RRM_ONE;        
                p_rrm_son_meas_results_ind->utran_meas_list_size = RRM_ZERO;        
                p_rrm_son_meas_results_ind->geran_meas_list_size = RRM_ZERO;        
                p_rrm_son_meas_results_ind->cdma_meas_list_size = RRM_ZERO;        
                /* SPR 16406 21AUG Start */
                    rrm_fill_data_in_eutran_son_meas_results_ind(p_ue_context, 
                            p_cell_selection_list,p_rrm_son_meas_results_ind); 
                /* SPR 16406 21AUG End */
            }
            ret_val = RRM_SUCCESS;
            break;

        case RRM_UTRAN_KNOWN_CELL_LIST: 
        case RRM_UTRAN_UNKNOWN_CELL_LIST: 
        case RRM_UTRAN_CONFUSED_CELL_LIST: 
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                        "Either Utran unknown or confused cell selection list" 
                        "is selected for cgi procedure triggered");
                p_rrm_son_meas_results_ind->eutran_meas_list_size = RRM_ZERO;        
                p_rrm_son_meas_results_ind->utran_meas_list_size = RRM_ONE;        
                p_rrm_son_meas_results_ind->geran_meas_list_size = RRM_ZERO;        
                p_rrm_son_meas_results_ind->cdma_meas_list_size = RRM_ZERO;        

                rrm_fill_data_in_utran_son_meas_results_ind(p_ue_context, p_cell_selection_list,p_rrm_son_meas_results_ind); 
            }
            ret_val = RRM_SUCCESS;
            break;

        case RRM_GERAN_KNOWN_CELL_LIST: 
        case RRM_GERAN_UNKNOWN_CELL_LIST: 
        case RRM_GERAN_CONFUSED_CELL_LIST: 
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                        "Either Geran unknown or confused cell selection list" 
                        "is selected for cgi procedure triggered");
                p_rrm_son_meas_results_ind->eutran_meas_list_size = RRM_ZERO;        
                p_rrm_son_meas_results_ind->utran_meas_list_size = RRM_ZERO;        
                p_rrm_son_meas_results_ind->geran_meas_list_size = RRM_ONE;        
                p_rrm_son_meas_results_ind->cdma_meas_list_size = RRM_ZERO;        

                rrm_fill_data_in_geran_son_meas_results_ind(p_ue_context, p_cell_selection_list,p_rrm_son_meas_results_ind); 
            }
            ret_val = RRM_SUCCESS;
            break;
        default:
            RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_WARNING,
                    "The current cell list is invalid for CGI procedure"
                    "no updation in rrm_son_meas_results_ind required");

    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 16406 End */
/* SPR-13322-fix start */
/****************************************************************************
 * Function Name  : rrm_ue_handle_meas_results_ind
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the meas_results_ind message received from RRC.
 ****************************************************************************/
rrm_return_et
rrm_ue_handle_meas_results_ind_for_ho (
        rrm_ue_global_context_t *p_g_rrm_ue_ctx
        )
{
    rrm_return_et        ret_val           = RRM_FAILURE;
    rrm_ue_context_t*    p_ue_context      = RRM_PNULL;

    rrmc_meas_report_event_et          meas_rep_event = MEAS_REP_EVENT_NONE;
    rrm_ue_measurment_results_ind_t*   p_rrm_parsed_meas_res_ind = RRM_PNULL;
    /* SPR 16406 8SEP start */
    /* Code removed */
    /* SPR 16406 8SEP end */
    /* SPR 16406 24Sep Start */
    /* Code removed */
    /* SPR 16406 24Sep End */

    RRM_UT_TRACE_ENTER();

    if ((RRM_PNULL != p_g_rrm_ue_ctx) && 
            RRM_PNULL != p_g_rrm_ue_ctx->p_ue_context) 
    {
        // coverity_fix 88203 +-
        /* SPR 16406 8SEP start */
        /* Code removed */
        /* SPR 16406 8SEP end */
        p_ue_context = p_g_rrm_ue_ctx->p_ue_context ;
        p_rrm_parsed_meas_res_ind = (rrm_ue_measurment_results_ind_t*)
            p_g_rrm_ue_ctx->p_ue_context->current_procedure.p_proc_data;
        /* SPR 16406 8SEP start */
        /* Code removed */
        /* SPR 16406 8SEP end */
        /* Soft Lock start - Global variable removal */
        rrm_cell_context_t    *p_cell_ctxt = RRM_PNULL;
        p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context->cell_index);
        if (RRM_PNULL == p_cell_ctxt)
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                    "Cell Context not found");
            return RRM_FAILURE;
        }
        /* Soft Lock end - Global variable removal */
        /* We have the UE index and Meas ID values parsed in UE context */
        /* Find out the event info corresponding to this UE index and Meas ID */
        /* SPR_15712_Fix: Start */
        meas_rep_event = rrm_ue_get_configured_meas_event (
                p_rrm_parsed_meas_res_ind->meas_id,
                p_ue_context);

        /* SPR 16406 8SEP start */
        /* Code removed */
        /* SPR 16406 8SEP end */

        /* SPR 21623 Changes Start */
        if (meas_rep_event != MEAS_REP_EVENT_NONE)
        {
            /* SPR 16406 8SEP end */

                 /* Store the results of serving cell of the UE in ue context */
            p_g_rrm_ue_ctx->p_ue_context->meas_result_serv_cell.rsrp_result =
                p_rrm_parsed_meas_res_ind->meas_result_serv_cell.rsrp_result;

            p_g_rrm_ue_ctx->p_ue_context->meas_result_serv_cell.rsrq_result =
                p_rrm_parsed_meas_res_ind->meas_result_serv_cell.rsrq_result;

        }
        /* SPR 21623 Changes End */
        switch (meas_rep_event) {
            case MEAS_REP_EVENT_A1:
                    if((!p_cell_ctxt->administrative_procedure_ongoing) && 
                            (RRM_HO_ACTN_ON_UE != p_ue_context->action_triggered))
                    {
                        /* SPR 16068 start */
                        /* Code deleted */
                        /* SPR 16068 end */
                        ret_val = rrm_ue_handle_meas_event_a1 (p_g_rrm_ue_ctx);
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                                "Ignoring HO cancel as this is forceful handover");
                        RRM_UT_TRACE_EXIT();
                        return RRM_SUCCESS;
                    }
                break;
            case MEAS_REP_EVENT_A2:
                    if(!p_cell_ctxt->administrative_procedure_ongoing &&
                       (p_ue_context->current_procedure.state !=
                                      RRM_UE_LIPA_SIPTO_HO_ONGOING))
                    {
                        /* SPR 16068 start */
                        /* Code deleted */
                        /* SPR 16068 end */
                        /*SPR Fix 15664 Start*/
                        p_ue_context->is_meas_gap_configured = RRM_TRUE;
                        /*SPR Fix 15664 End*/
                        ret_val = rrm_ue_handle_meas_event_a2 (p_g_rrm_ue_ctx);
                    }
                break;
                /* SPR 16406 Start */
            case MEAS_REP_EVENT_A3:
            case MEAS_REP_EVENT_A4:
            case MEAS_REP_EVENT_A5:
            /* SPR 15598 start */
                {
                    if (p_ue_context->current_procedure.state != RRM_UE_LIPA_SIPTO_HO_ONGOING)
                    {
                        ret_val = rrm_uem_handle_intraRAT_meas_event(p_g_rrm_ue_ctx);
                    }  
                }
                break;
                /* SPR 16406 End */
            /* Inter RAT changes start */
            case MEAS_REP_EVENT_B1:
            case MEAS_REP_EVENT_B2:
            /* SPR_14564_fix: start */
            case MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON:
                {
                    if(p_ue_context->current_procedure.state !=
                            RRM_UE_LIPA_SIPTO_HO_ONGOING)
                    {
                        ret_val = rrm_ue_handle_inter_RAT_meas_event(p_g_rrm_ue_ctx);

                    }
                }
                break;
            case MEAS_REP_EVENT_FOR_CGI:
                {
                    ret_val = rrm_handle_cgi_measurement(p_ue_context, p_rrm_parsed_meas_res_ind);
                }
                break;
            /* SPR 16406 8SEP end */
            case MEAS_REP_EVENT_NONE:
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "Required Processing done");
                ret_val = RRM_SUCCESS;
                break;
            /* CSR 58774 Fix End */ 
            default:
                RRM_TRACE(g_uem_log_on_off, 
                        p_g_rrm_uem_facility_name, 
                        RRM_WARNING, 
                        "Meas Event (%u) received for [UE:%d]",
                        meas_rep_event,
                        p_g_rrm_ue_ctx->p_ue_context->ue_index);
                break;
        }
    }    
    RRM_UT_TRACE_EXIT();

    return ret_val;
}
/* SPR-13322-fix end*/
/* SPR 17784 Start */
/****************************************************************************
 * Function Name  : rrm_ue_process_cgi_timer_expiry
 * Inputs         : rrm_global_context_t *p_rrm_glb_ctxt RRM Global Context.
 * Outputs        : None
 * Returns        : None
 * Description    : Handles CGI timer expiry event for unknown PCI or confused 
 *                  PCI
 ****************************************************************************/
rrm_return_et 
rrm_ue_process_cgi_timer_expiry(rrm_ue_global_context_t *p_rrm_glb_ctxt)
{
  rrm_return_et       ret_val        = RRM_SUCCESS;
  rrm_ue_context_t*   p_ue_context   = RRM_PNULL;
  U8                  transaction_id = p_rrm_glb_ctxt->trans_id;
  rrm_ue_timer_buf_t *p_timer_buf    = RRM_PNULL;

  p_ue_context = (rrm_ue_context_t*)p_rrm_glb_ctxt->p_ue_context;

  RRM_UT_TRACE_ENTER();

  if (p_ue_context->ho_params.cgi_timer) 
  {
    p_timer_buf = (rrm_ue_timer_buf_t *) qvTimerData(p_ue_context->ho_params.cgi_timer);
    p_ue_context->ho_params.cgi_timer = RRM_PNULL;
    /* 
     ** Report CGI timer expired, find another PCI for CGI 
     ** otherwise remove the DRX
     */

    rrm_remove_previous_cell_for_ho(p_ue_context,
        p_timer_buf->phy_cell_id,
        p_timer_buf->earfcn);

    rrm_search_and_remove_phy(p_ue_context,
        p_timer_buf->phy_cell_id,
        p_timer_buf->earfcn);

    ret_val = rrm_uem_search_and_select_pci_for_ho(p_ue_context,transaction_id);

    if(RRM_FAILURE == ret_val)
    {
      if(RRM_TRUE == p_ue_context->is_cgi_process_in_progress)
      {
        RRM_TRACE(g_cellm_log_on_off,
            p_g_rrm_uem_facility_name, 
            RRM_DETAILED, 
            " CGI process ongoing while CGI timer expired: clean up the meas config "); 

        if(RRM_UE_CDRX_APPLIED == p_ue_context->cdrx_status)
        {
          p_ue_context->cdrx_status = RRM_UE_CDRX_TO_BE_REMOVED;
          /* Apply DRX again as CGIs have been reported */
          if (RRM_FAILURE == rrm_build_and_send_ue_reconfig_req(p_ue_context))
          {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                "Failed to send UE reconfig request to remove CDRX, [UE:%d]",
                p_ue_context->ue_index);
          }
          else
          {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "UE reconfig request sent to [UE:%d] to remove CDRX ",
                p_ue_context->ue_index);
          }
        }
      }
    }
    else
      RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
          "Report CGI is triggered return_val = %d",ret_val);
  }
  RRM_UT_TRACE_EXIT();
  return ret_val;
}
/* SPR 17784 End */
 
/****************************************************************************
 * Function Name  : rrm_ue_process_blind_timer_expiry
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : None
 * Description    : Handles Blind timer expiry event for blind(DAHO)
 ****************************************************************************/
void rrm_ue_process_blind_timer_expiry (rrm_ue_global_context_t *p_g_rrm_ue_ctx)
{

    rrm_return_et       ret_val        = RRM_SUCCESS;
    rrm_ue_context_t*   p_ue_context   = RRM_PNULL;
    /* SPR-17852 START */
    /* code deleted */
    /* SPR-17852 END */
    /* SPR 15674 Start */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t  phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 15674 End */
    U8                  transaction_id = p_g_rrm_ue_ctx->trans_id;

    p_ue_context = (rrm_ue_context_t*)p_g_rrm_ue_ctx->p_ue_context;
    /* SPR-17852 START */
    /* code deleted */
    /* SPR-17852 END */

    RRM_UT_TRACE_ENTER();

    p_ue_context->meas_rep_event = MEAS_REP_EVENT_NONE;

    /* SPR-17852 START */
    /* code deleted */
    /* SPR-17852 END */
    /* Start :BUG 983 */
    /* 
     ** Delay timer expired, Trigger HO to DAHO so reset the 
     ** last measurements reports if any.
     */
    /* SPR 15232 start */
    /* Code deleted */
    /* SPR 15232 end */

    /* SPR 16406 Start */
    p_ue_context->rrm_cell_selection_info.current_cell_list = RRM_EUTRAN_KNOWN_CELL_LIST;
    /* SPR 16406 End */

    if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
            == RRM_MEMBER)
    {
        /* Fix for CSR 58972 start*/
        /*As ue is csg member try to ho it to a closed cell*/
        /* SPR 15674 Start */
        phy_cell_id_info = rrm_get_best_pci_for_ho(p_ue_context,
                RRM_OAM_ACCESS_MODE_CLOSE);
        /* Fix for CSR 58972 end*/
    }
    else if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
            == RRM_NOT_MEMBER)
    {
        /* Fix for CSR 58972 start*/
        /*As ue is csg member try to ho it to a closed cell*/
        phy_cell_id_info = rrm_get_best_pci_for_ho(p_ue_context,
                RRM_OAM_ACCESS_MODE_OPEN);
        /* Fix for CSR 58972 end*/
    }
    /*csg end*/
    if (INVALID_PHYSICAL_CELL_ID != phy_cell_id_info.phy_cell_id)
    {
        /* SPR 16406 8SEP start */
        /* Code removed */
        /* SPR 16406 8SEP end */
        /* Trigger Handover */
        /*SPR_17893_START*/
        if((UTRA_CELL == p_ue_context->target_cell_type) &&
                !(RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT & p_ue_context->ue_capability_params.bitmask))
        {    
            if((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state))
            {
                rrm_populate_and_send_utran_cap_enq_req(p_ue_context);
                rrm_ue_set_state(p_ue_context,RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES);
                rrm_set_utran_capability_enq_info(p_ue_context,RRM_TRUE,RRM_FALSE);
            }
            /*Move Back one index,if the valid id is found*/
            if((RRM_ZERO < p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx) && 
                    (p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx <= 
                     p_ue_context->ho_params.utra_known_cell_selection_list.count))
            {
                p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx--;
            }
            /*Undo the bitmask,to check if DAHO cell is selected */
            if(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED)
            {
                p_ue_context->ho_params.utra_known_cell_selection_list.bitmask &= ~RRM_UE_BLIND_CELL_HO_TRIGGERED;  
            }    

            p_ue_context->is_ho_reqd_on_recv_utan_cap  = RRM_TRUE;
        }
        else
        {
            /*SPR_17893_END*/
            ret_val = rrm_build_and_send_ho_required (phy_cell_id_info,
                    p_ue_context,
                    transaction_id);    
            /* SPR 16406 8SEP start */
            /* Code removed */
            /* SPR 16406 8SEP end */
            RRM_TRACE(g_cellm_log_on_off,
                    p_g_rrm_uem_facility_name, 
                    RRM_DETAILED, 
                    "%s: Build & send HO Required (%s)", 
                    "UE:", ret_val?"SUCCESS":"FAILED");    
            /*SPR_17893_START*/
        }   
        /*SPR_17893_END*/
    }
    /* SPR 15853 Start */
    else 
    {
        /*SPR 22571 Fix Start*/
        RRM_TRACE(g_cellm_log_on_off, 
                p_g_rrm_uem_facility_name, 
                RRM_INFO, 
                "Blind Timer expired, but cell not found");    
        /*SPR 22571 Fix Stop*/

        /*SPR_17893_START*/
        if(RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES != p_ue_context->ue_state)
        {
            /*SPR_17893_END*/
            UEM_FSM_SET_STATE (p_ue_context, RRM_UE_STATE_ACTIVE);
            /*SPR_17893_START*/
        }   
        /*SPR_17893_END*/

        if(RRM_TRUE == p_ue_context->is_cgi_process_in_progress)
        {
            RRM_TRACE(g_cellm_log_on_off,
                    p_g_rrm_uem_facility_name, 
                    RRM_DETAILED,
                    " CGI process ongoing while blind timer expired: clean up the meas config ");

            if(RRM_UE_CDRX_APPLIED == p_ue_context->cdrx_status)
            {
                p_ue_context->cdrx_status = RRM_UE_CDRX_TO_BE_REMOVED;
                /* Apply DRX again as CGIs have been reported */
                if (RRM_FAILURE == rrm_build_and_send_ue_reconfig_req(p_ue_context))
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                            "Failed to send UE reconfig request to remove CDRX, [UE:%d]",
                            p_ue_context->ue_index);
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "UE reconfig request sent to [UE:%d] to remove CDRX ",
                            p_ue_context->ue_index);
                }
            }
            else if(RRM_UE_CDRX_DISABLED == p_ue_context->cdrx_status)
            {
                /* For Autonomous Gap */
                /* SPR 16406 8SEP start */
                p_ue_context->meas_rep_event = MEAS_REP_EVENT_FOR_CGI; 
                /* SPR 16406 8SEP end */
                ret_val = rrm_build_and_send_meas_config_req(p_ue_context,MEAS_TYPE_HO);
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "Report config remove list sent for [UE:%d]",
                            p_ue_context->ue_index);
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "Failure in sending report config remove list sent for [UE:%d]",
                            p_ue_context->ue_index);

                }
                /* SPR 16053 start */
                /* Code deleted */
                /* SPR 16053 end */
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "No Action required during blind timer expiry for [UE:%d] with cdrx status[%d]",
                        p_ue_context->ue_index, p_ue_context->cdrx_status);

            }
        }
        /* SPR 15853 End */
    }

    /* SPR-17852 START */
    /* code deleted */
    /* SPR-17852 END */
    RRM_UT_TRACE_EXIT();
}


/* SPR 16263 Fix Start */
    rrm_void_t
populate_rrm_oam_eutran_global_cell_id(cell_global_id_eutra_t*          p_glb_cell_id,
        rrm_oam_eutran_global_cell_id_t* rrm_oam_eutran_global_cell_id)
{

    RRM_UT_TRACE_ENTER();

    RRM_MEMSET(rrm_oam_eutran_global_cell_id, RRM_ZERO, sizeof(rrm_oam_eutran_global_cell_id_t));

    /// Copy PLMN Id data
    RRM_MEMCPY(rrm_oam_eutran_global_cell_id->primary_plmn_id.mcc, p_glb_cell_id->plmn_identity.mcc, sizeof(p_glb_cell_id->plmn_identity.mcc));
    rrm_oam_eutran_global_cell_id->primary_plmn_id.num_mnc_digit = p_glb_cell_id->plmn_identity.mnc.count;
    RRM_MEMCPY(rrm_oam_eutran_global_cell_id->primary_plmn_id.mnc, p_glb_cell_id->plmn_identity.mnc.mnc, sizeof(p_glb_cell_id->plmn_identity.mnc.mnc));

    /// Copy Cell Id
    RRM_MEMCPY(rrm_oam_eutran_global_cell_id->cell_identity, p_glb_cell_id->cell_identity, sizeof(p_glb_cell_id->cell_identity));


    RRM_UT_TRACE_EXIT();
    return;
}
/* SPR 16263 Fix End */


/* For Soft Lock Forcefully HO every UE Start Function Definition */
/****************************************************************************
 * Function Name  : rrm_get_pci_from_ncl_gcid
 * Inputs         : global cell id and cell context
 * Outputs        : NONE
 * Returns        : PCI for correspondig global cell id
 * Description    : This function map global cell id to PCI in NCL List
 ****************************************************************************/
/* SPR 15674 Start */
phy_cell_id_info_t rrm_get_pci_from_ncl_gcid 
(
    cell_global_id_eutra_t *p_glb_cell_id,
    rrm_cell_context_t *p_cell_ctx
)
/* SPR 15674 End  */
{
    U8 num_valid_ran_list = RRM_ZERO;
    U8 cell_list = RRM_ZERO;
    /* SPR 15674 Start */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}}; 
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 15674 End */

    /* SPR 16263 Fix Start */
    rrm_oam_eutran_global_cell_id_t rrm_oam_eutran_global_cell_id;
    /* SPR 16263 Fix End */

    rrm_return_et ret_val = RRM_SUCCESS;
    if (RRM_PNULL != p_cell_ctx)
    {
        /* List of Intra freq EUTRAN Cell  */
        num_valid_ran_list = p_cell_ctx->ran_info.ncl_params.lte_ncl.num_valid_intra_freq_cell;
        for (cell_list = RRM_ZERO; cell_list < num_valid_ran_list; ++cell_list)
        {
            /* SPR 16263 Fix Start */
            populate_rrm_oam_eutran_global_cell_id(p_glb_cell_id, &rrm_oam_eutran_global_cell_id);
            ret_val = compare_global_cell_id (&rrm_oam_eutran_global_cell_id,
            /* SPR 16263 Fix End */
                    &(p_cell_ctx->ran_info.\
                        ncl_params.lte_ncl.\
                        intra_freq_cells[cell_list].\
                        cell_id));
            if (RRM_SUCCESS == ret_val)
            {
                /* SPR 15674 Start */
                phy_cell_id_info.carr_earfcn = p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn;
                phy_cell_id_info.phy_cell_id = p_cell_ctx->ran_info.ncl_params.lte_ncl.
                    intra_freq_cells[cell_list].phy_cell_id;
                return phy_cell_id_info;
                /* SPR 15674 End */
            }
        }
        /* List of Inter freq EUTRAN Cell */
        num_valid_ran_list = p_cell_ctx->ran_info.ncl_params.lte_ncl.num_valid_inter_freq_cell;
        for (cell_list = RRM_ZERO; cell_list < num_valid_ran_list; ++cell_list)
        {
            /* SPR 16263 Fix Start */
            populate_rrm_oam_eutran_global_cell_id(p_glb_cell_id, &rrm_oam_eutran_global_cell_id);
            ret_val = compare_global_cell_id (&rrm_oam_eutran_global_cell_id,
                    /* SPR 16263 Fix End */
                    &(p_cell_ctx->ran_info.\
                        ncl_params.lte_ncl.\
                        inter_freq_cells[cell_list].\
                        cell_id));
            if (RRM_SUCCESS == ret_val)
            {
                /* SPR 15674 Start */
                phy_cell_id_info.carr_earfcn = p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn;
                phy_cell_id_info.phy_cell_id = p_cell_ctx->ran_info.ncl_params.lte_ncl.
                    intra_freq_cells[cell_list].phy_cell_id;
                return phy_cell_id_info;
                /* SPR 15674 End */
            }
        }
    }
    /* warnings removal: return statement is added for the case when 
       function does not return from inside if() block */
    /* SPR 15674 Start */
    return phy_cell_id_info;
    /* SPR 15674 End */
}

/*SPR 16363 START*/
/****************************************************************************
 * Function Name  : rrm_build_and_send_ho_required_for_daho
 * Inputs         : UE Context
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/FAILURE
 * Description    : This function trigger ho to the DAHO cell.
 ****************************************************************************/
rrm_return_et
rrm_build_and_send_ho_required_for_daho
(
 rrm_ue_context_t *p_ue_context,
 U16 transaction_id
 )
{
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
	phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}}; 
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    rrm_return_et ret_val = RRM_SUCCESS;
    rrm_daho_cell_config_info_t  *p_daho_cfg = RRM_NULL;
    rrm_bool_et                   trigger_ho = RRM_FALSE;
    /*SPR 19075 Start*/
    U8 eutran_inter_freq_count = RRM_ZERO;
    /*SPR 19075 End*/
    RRM_UT_TRACE_ENTER();
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "UEM: triggering the HO for DAHO cell [%d]", p_ue_context->ue_index);

    p_daho_cfg = rrm_cellm_get_daho_cell_config_info_params(p_ue_context->cell_index);
    if(p_daho_cfg != RRM_PNULL)
    {
	    if(p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT)
	    {
		    phy_cell_id_info.phy_cell_id = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
		    /* SPR 19075 Start */
		    phy_cell_id_info.carr_earfcn = p_g_rrm_cell_ctx->p_rrm_cell_ctx[p_ue_context->cell_index]->ran_info.rf_params.rf_configurations.dl_earfcn;
		    /* SPR 19075 End */
		    trigger_ho = RRM_TRUE;
		    p_ue_context->target_cell_type = EUTRA_CELL;
		    /* SPR 19075 Start */
		    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
				    "UEM: INTRA FREQ EUTRA DAHO CELL configured");
		    /* SPR 19075 End */
	    }
	    /*SPR 19075 Start*/
	    else if(p_daho_cfg->bitmask & EUTRA_INTER_FREQ_DAHO_CELL_PRESENT)
	    {
		    for(eutran_inter_freq_count = RRM_ZERO; eutran_inter_freq_count < p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.count; eutran_inter_freq_count++)
		    {
			    if(rrm_ue_is_ue_supported_eutra_frequency(p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.rrm_eutran_inter_freq_daho_cell_config_info[eutran_inter_freq_count].earfcn, p_ue_context))
			    {
				    phy_cell_id_info.phy_cell_id = p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.rrm_eutran_inter_freq_daho_cell_config_info[eutran_inter_freq_count].phy_cell_id;
				    phy_cell_id_info.carr_earfcn = p_daho_cfg->eutran_inter_freq_daho_cells_config_info_list.rrm_eutran_inter_freq_daho_cell_config_info[eutran_inter_freq_count].earfcn;
				    ret_val = RRM_SUCCESS;
				    break;
			    }
			    else
			    {
				    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
						    "UEM: NO INTER FREQ DAHO SUPPORTED BY UE");
				    ret_val = RRM_FAILURE;
			    }

		    }
		    if(ret_val == RRM_FAILURE)
		    {
			return ret_val;
		    }
		    trigger_ho = RRM_TRUE;
		    p_ue_context->target_cell_type = EUTRA_CELL;
		    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
				    "UEM: INTER FREQ EUTRA DAHO CELL configured");
	    }
	    /*SPR 19075 End*/
	    else if(p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT)
	    {
		    phy_cell_id_info.phy_cell_id = p_daho_cfg->utran_daho_cell_config_info.pci;
		    phy_cell_id_info.carr_earfcn = p_daho_cfg->utran_daho_cell_config_info.uarfcndl;
		    trigger_ho = RRM_TRUE;
		    p_ue_context->target_cell_type = UTRA_CELL;
		    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
				    "UEM: UTRA_CELL configured");
	    }
	    else if(p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT)
	    {
		    phy_cell_id_info.phy_cell_id = p_daho_cfg->geran_daho_cell_config_info.pci;
		    phy_cell_id_info.carr_earfcn = p_daho_cfg->geran_daho_cell_config_info.bcch_arfcn;
		    trigger_ho = RRM_TRUE;
		    p_ue_context->target_cell_type = GERAN_CELL;
		    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
				    "UEM: GERAN_CELL configured");
	    }
	    else
	    {
		    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
				    "UEM: No DAHO cell is configured by OAM.So no need to trigger HO on DAHO cell");
	    }

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "UEM: daho freq [%d] and pci [%d] ",phy_cell_id_info.carr_earfcn,phy_cell_id_info.phy_cell_id);

        if(trigger_ho == RRM_TRUE && phy_cell_id_info.phy_cell_id != INVALID_PHYSICAL_CELL_ID )
        {
            ret_val = rrm_build_and_send_ho_required (phy_cell_id_info,
                    p_ue_context,
                    transaction_id);
            RRM_TRACE(g_cellm_log_on_off,
                    p_rrm_cellm_facility_name,
                    RRM_BRIEF,
                    "%s: Build & send HO Required (%s)",
                    "UE:", ret_val?"SUCCESS":"FAILED");
        }
        /* SPR 16406 8SEP start */
        /* Code removed */
        /* SPR 16406 8SEP end */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "UEM: No DAHO cell is configured by OAM");
        ret_val = RRM_FAILURE;
    }


    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*SPR 16363 END*/

/*  SPR-20656 START */
/* code deleted */
/*  SPR-20656 END */
/* For Soft Lock Forcefully HO every UE End Function Definition */

/* Inter RAT changes start */
/****************************************************************************
 ** Function Name  : rrm_ue_process_csfb_blind_timer_expiry
 ** Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 ** Outputs        : None
 ** Returns        : None
 ** Description    : Handles CSFB Blind timer expiry event 
 *****************************************************************************/
void rrm_ue_process_csfb_blind_timer_expiry 
		(rrm_ue_global_context_t *p_g_rrm_ue_ctx)
{
	
    rrm_ue_context_t*   p_ue_context   = RRM_PNULL;
    rrm_ho_params_t*    p_ho_params    = RRM_PNULL;
    /* SPR 15674 Start */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 15674 End  */
	rrm_return_et		ret_val = RRM_SUCCESS;

    U8 ps_ho_flag = RRM_FALSE;

    p_ue_context = (rrm_ue_context_t*)p_g_rrm_ue_ctx->p_ue_context;
    p_ho_params  = (rrm_ho_params_t*)&p_ue_context->ho_params;

    RRM_UT_TRACE_ENTER();

	p_ue_context->ho_params.csfb_blind_timer = RRM_PNULL;
	/* CSFB  Timer expired */
	/* Trigger UE Release with Re-direction */
    /*csg start*/

    if((RRM_FALSE == p_ue_context->cdma2000_meas_id_received) &&
        (CDMA2000_CELL != rrm_get_hghst_prorty_inter_rat_basd_on_spid(p_ue_context)) &&
        p_ue_context->cdma2000_capabilities.is_utran_geran_present)
    {
        if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
                == RRM_MEMBER)
        {
            /*As ue is csg member try to ho it to a closed cell*/
            /* SPR 15674 Start */
            phy_cell_id_info = rrm_get_best_suitable_cell_for_ho(p_ue_context,
                    RRM_OAM_ACCESS_MODE_CLOSE);
        }
        else if(p_ue_context->rrm_csg_membership.rrm_csg_membership_status
                == RRM_NOT_MEMBER)
        {
            /*As ue is csg member try to ho it to a closed cell*/
            phy_cell_id_info = rrm_get_best_suitable_cell_for_ho(p_ue_context,
                    RRM_OAM_ACCESS_MODE_OPEN);
        }
        /*csg end*/
        if(p_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD)
        {
            ps_ho_flag = rrm_cellm_get_ps_ho_flag(p_ue_context->cell_index);
            /* Trigger PS-HO */
            if((phy_cell_id_info.phy_cell_id != INVALID_PHYSICAL_CELL_ID) && (RRM_TRUE == ps_ho_flag))
            {
                /*SPR_17893_START*/
                if((UTRA_CELL == p_ue_context->target_cell_type) &&
                        !(RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT & p_ue_context->ue_capability_params.bitmask))
                {    
                    if((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state))
                    {
                        rrm_populate_and_send_utran_cap_enq_req(p_ue_context);
                        rrm_ue_set_state(p_ue_context,RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES);
                        rrm_set_utran_capability_enq_info(p_ue_context,RRM_TRUE,RRM_FALSE);
                        /*Move Back one index,if the valid id is found*/
                        if((RRM_ZERO < p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx) && 
                                (p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx <= 
                                 p_ue_context->ho_params.utra_known_cell_selection_list.count))
                        {
                            p_ue_context->ho_params.utra_known_cell_selection_list.next_target_cell_idx--;
                        }
                        /*Undo the bitmask,to check if DAHO cell is selected */
                        if(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & RRM_UE_BLIND_CELL_HO_TRIGGERED)
                        {
                            p_ue_context->ho_params.utra_known_cell_selection_list.bitmask &= ~RRM_UE_BLIND_CELL_HO_TRIGGERED;  
                        }    

                        p_ue_context->is_ho_reqd_on_recv_utan_cap  = RRM_TRUE;
                    }
                    else if(RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES == p_ue_context->ue_state)
                    {
                        if(RRM_SUCCESS ==  rrm_build_and_send_ue_connection_release_ind(
                                    p_ue_context->ue_index,
                                    RADIO_NETWORK_LAYER_TYPE,
                                    CS_FALLBACK_TRIGGERED,
                                    p_g_rrm_ue_ctx->trans_id,
                                    p_ue_context,
                                    p_ue_context->cell_index))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "ue connection release ind sent for [UE:%d]",p_ue_context->ue_index);
                            if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                            {
                                RRM_TRACE(g_uem_log_on_off,
                                        p_rrm_uem_facility_name,
                                        RRM_ERROR,
                                        "ue_connection_release_ind sent, \
                                        Failed to clean up ue resources");
                                ret_val = RRM_FAILURE;
                            }
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                    "Failed to send release connection ind for [UE:%d]",
                                    p_ue_context->ue_index);
                            ret_val = RRM_FAILURE;
                        }
                    }
                }
                else
                {
                    /*SPR_17893_END*/

                    ret_val = rrm_build_and_send_ho_required (
                            phy_cell_id_info,
                            p_ue_context,
                            p_g_rrm_ue_ctx->trans_id); 
                    if (RRM_SUCCESS == ret_val) 
                    {
                        /* SPR 16406 8SEP start */
                        /* Code removed */
                        /* SPR 16406 8SEP end */
                        RRM_TRACE(g_cellm_log_on_off,
                                p_rrm_cellm_facility_name, 
                                RRM_DETAILED, 
                                "HO triggered towards physicall cell id:[%d]"
                                "for [UE:%d]", phy_cell_id_info.phy_cell_id,
                                p_ue_context->ue_index);    
                        /* SPR 15674 End  */
                    }
                    else
                    {
                        /* Connection Release with Redirection */
                        /* Bug 16121 Fix Start */
                        if(RRM_SUCCESS ==  rrm_build_and_send_ue_connection_release_ind(
                                    p_ue_context->ue_index,
                                    RADIO_NETWORK_LAYER_TYPE,
                                    CS_FALLBACK_TRIGGERED,
                                    p_g_rrm_ue_ctx->trans_id,
                                    p_ue_context,
                                    /* SPR_10729_fix */
                                    p_ue_context->cell_index))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "ue connection release ind sent for [UE:%d]",p_ue_context->ue_index);
                            if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                            {
                                RRM_TRACE(g_uem_log_on_off,
                                        p_rrm_uem_facility_name,
                                        RRM_DETAILED,
                                        "ue_connection_release_ind sent, \
                                        Failed to clean up ue resources");
                                ret_val = RRM_FAILURE;
                            }
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                    "Failed to send release connection ind for [UE:%d]",
                                    p_ue_context->ue_index);
                            ret_val = RRM_FAILURE;
                        }
                        /* Bug 16121 Fix End */

                    }
              /*SPR_17893_START*/
                }
               /*SPR_17893_END*/
            }	
            else 
            {
                /*spr_20912_changes_start*/
                p_ue_context->ho_params.target_phys_cell_id = phy_cell_id_info.phy_cell_id;
                /*spr_20912_changes_end*/
                /* Connection Release with Redirection */
                /* Bug 16121 Fix Start */
                    if(RRM_SUCCESS ==  rrm_build_and_send_ue_connection_release_ind(
                                p_ue_context->ue_index,
                                RADIO_NETWORK_LAYER_TYPE,
                                CS_FALLBACK_TRIGGERED,
                                p_g_rrm_ue_ctx->trans_id,
                                p_ue_context,
                                /* SPR_10729_fix */
                                p_ue_context->cell_index))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "ue connection release ind sent for [UE:%d]",p_ue_context->ue_index);
                        if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                        {
                            RRM_TRACE(g_uem_log_on_off,
                                    p_rrm_uem_facility_name,
                                    RRM_DETAILED,
                                    "ue_connection_release_ind sent, \
                                    Failed to clean up ue resources");
                            ret_val = RRM_FAILURE;
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "Failed to send release connection ind for [UE:%d]",
                                p_ue_context->ue_index);
                        ret_val = RRM_FAILURE;
                    }
                    /* Bug 16121 Fix End */

            }
        }
    }
    else if(p_ue_context->cdma2000_capabilities.is_cdma2000_present)
    {
        ps_ho_flag = rrm_cellm_get_ps_ho_flag(p_ue_context->cell_index);

        /* Check for ps_ho flag configured from OAM */
        if(RRM_TRUE == ps_ho_flag)
        {
            ret_val = rrm_build_and_send_daho_cdma2000_ho_required (
                    p_ue_context,
                    p_g_rrm_ue_ctx->trans_id); 
            if (RRM_SUCCESS == ret_val) 
            {
                /* SPR 15674 Start */
                p_ho_params->target_phys_cell_id = phy_cell_id_info.phy_cell_id;
                p_ho_params->target_earfcn = phy_cell_id_info.carr_earfcn;
                /* SPR 15674 End  */
                RRM_TRACE(g_cellm_log_on_off,
                        p_rrm_cellm_facility_name, 
                        RRM_DETAILED, 
                        "HO triggered for [UE:%d]",
                        p_ue_context->ue_index);    
            }
            else
            {
                /* Connection Release with Redirection */
                /* Bug 16121 Fix Start */
                if(RRM_SUCCESS ==  rrm_build_and_send_ue_connection_release_ind(
                            p_ue_context->ue_index,
                            RADIO_NETWORK_LAYER_TYPE,
                            CS_FALLBACK_TRIGGERED,
                            p_g_rrm_ue_ctx->trans_id,
                            p_ue_context,
                            /* SPR_10729_fix */
                            p_ue_context->cell_index))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "ue connection release ind sent for [UE:%d]",p_ue_context->ue_index);
                    if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_rrm_uem_facility_name,
                                RRM_DETAILED,
                                "ue_connection_release_ind sent, \
                                Failed to clean up ue resources");
                        ret_val = RRM_FAILURE;
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Failed to send release connection ind for [UE:%d]",
                            p_ue_context->ue_index);
                    ret_val = RRM_FAILURE;
                }
                /* Bug 16121 Fix End */

            }
        }
        else 
        {
            /* Bug 4505 begin */
            p_ue_context->ho_params.target_phys_cell_id = phy_cell_id_info.phy_cell_id;
            /* Bug 4505 end */
            /* Connection Release with Redirection */
            /* Bug 16121 Fix Start*/
                if(RRM_SUCCESS ==  rrm_build_and_send_ue_connection_release_ind(
                            p_ue_context->ue_index,
                            RADIO_NETWORK_LAYER_TYPE,
                            CS_FALLBACK_TRIGGERED,
                            p_g_rrm_ue_ctx->trans_id,
                            p_ue_context,
                            /* SPR_10729_fix */
                            p_ue_context->cell_index))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "ue connection release ind sent for [UE:%d]",p_ue_context->ue_index);
                    /* Valgrind fixes start */
                    /* Freq-priority fix changes start */	
                    p_ue_context->ho_params.bitmask &= ~(RRM_UE_CSFB_BLIND_TIMER_EXPIRED);
                    /* Freq-priority fix changes end */	
                    /* Valgrind fixes end */
                    if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_context))
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_rrm_uem_facility_name,
                                RRM_DETAILED,
                                "ue_connection_release_ind sent, \
                                Failed to clean up ue resources");
                        ret_val = RRM_FAILURE;
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Failed to send release connection ind for [UE:%d]",
                            p_ue_context->ue_index);
                    ret_val = RRM_FAILURE;
                }
                /* Bug 16121 Fix End */

        }
        RRM_UT_TRACE_EXIT();
    }
    RRM_UT_TRACE_EXIT();
}
/* Inter RAT changes end */

/* SPR 13445 Fix Start */
/****************************************************************************
 * Function Name  : rrm_try_daho_on_csg_timer_expiry
 * Inputs         : rrm_ue_context_t *p_ue_cntxt
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function try to select DAHO cell for HO if CSG_TIMER 
 *                  expires. If HO Required fails then return FAILURE & Release
 *                  UE.
 ****************************************************************************/
rrm_return_et rrm_try_daho_on_csg_timer_expiry( rrm_ue_context_t *p_ue_cntxt )
{

    rrm_return_et      ret_val = RRM_FAILURE;
    U8                 rat_type = RRM_ZERO;
    U8                 index_rat = RRM_ZERO;
    U16                daho_pci = INVALID_PHYSICAL_CELL_ID;
    /* SPR 15674 Start */
    U16                 daho_freq = RRM_NULL;
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, daho_pci, daho_freq, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    /* SPR 15674 End */
    U16                transaction_id = RRM_ZERO;
    rrm_daho_cell_config_info_t  *p_daho_cfg = RRM_NULL;
    rrm_cell_context_t    *p_cell_ctx = RRM_NULL;

    rrm_bool_et is_eutran_spprted_by_spid = RRM_FALSE;
    rrm_bool_et is_utran_spprted_by_spid = RRM_FALSE;
    rrm_bool_et is_geran_supported_by_spid = RRM_FALSE;
    rrm_bool_et is_cdma2000_supported_by_spid = RRM_FALSE;
    rrm_bool_et spid_found = RRM_FALSE;

    RRM_UT_TRACE_ENTER(); 

    p_cell_ctx =  rrm_cellm_get_cell_context(p_ue_cntxt->cell_index);
    if (RRM_PNULL == p_cell_ctx)
    {
	RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
		"Cell Context not found");
	return ret_val;
    }

    /* extract daho configuration */
    p_daho_cfg = rrm_cellm_get_daho_cell_config_info_params(p_ue_cntxt->cell_index);

    /* Get RAT Priority on the basis of SPID if present else from CFG */
    rrm_get_rat_priority_from_spid(p_ue_cntxt,
	    &is_eutran_spprted_by_spid,
	    &is_utran_spprted_by_spid ,
	    &is_geran_supported_by_spid,
	    &is_cdma2000_supported_by_spid,
	    &spid_found);

    if(RRM_TRUE == spid_found)
    {
	p_ue_cntxt->spid_found = RRM_TRUE;
    }
    
    /* Get PCI of the highest Priority RAT by SPID */
    while(index_rat < RRM_IRAT_TYPE) 
    {
	switch(rat_type = get_next_priority_rat(index_rat, p_ue_cntxt))
	{
	    case RRM_IRAT_PRIORITY_UTRAN:
		{
		    if((p_daho_cfg != RRM_PNULL) &&
			    /*daho clean up(condition added for presence of daho cell)*/
			    (p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT) )
		    {
    /* SPR 15674 Start */
    //U16                 daho_freq = RRM_NULL;
    //phy_cell_id_info_t phy_cell_id_info = {daho_pci, daho_freq};
    /* SPR 15674 End */
                        p_ue_cntxt->target_cell_type = UTRA_CELL;
		    }
		    else
		    {
                /* SPR 14336 Fix- Corrected Trace */
			RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
				"UTRA DAHO not present.");
		    }
		}
		break;
	    case RRM_IRAT_PRIORITY_EUTRAN:
		{
            /* SPR 14336 Fix Start */
            if(p_daho_cfg != RRM_PNULL) 
                /*daho clean up(condition added for presence of daho cell)*/
		    {
                if (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT) 
                {
                    /* SPR 15674 Start */
                    daho_pci = p_daho_cfg->eutran_daho_cell_config_info.phy_cell_id;
                    daho_freq = p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn; 
                    /* SPR 15674 End */
                    p_ue_cntxt->target_cell_type = EUTRA_CELL;
                }
                else if(p_daho_cfg->bitmask & EUTRA_INTER_FREQ_DAHO_CELL_PRESENT)
                {
                    if(RRM_PNULL == rrm_uem_search_inter_freq_daho_cell(p_ue_cntxt, p_daho_cfg, 
                                 p_ue_cntxt->ho_params.target_earfcn, &daho_pci))
                    {
                        RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_ERROR,
                                "DAHO cell not found for Inter-freq = %d", p_ue_cntxt->ho_params.target_earfcn);
                    }
                    else
                    {
                        p_ue_cntxt->target_cell_type = EUTRA_CELL;
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        " EUTRA DAHO cfg not present.");
                }
            }
            /* SPR 14336 Fix End */
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        " DAHO cfg not present.");
            }
        }
		break;
	    case RRM_IRAT_PRIORITY_GERAN:
		{
		    if((p_daho_cfg != RRM_PNULL) &&
			    /*daho clean up(condition added for presence of daho cell)*/
			    (p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT) )
		    {
                    /* SPR 15674 Start */
                    daho_pci = p_daho_cfg->geran_daho_cell_config_info.pci;
                    daho_freq = p_daho_cfg->geran_daho_cell_config_info.bcch_arfcn;            
                    /* SPR 15674 End */
                        p_ue_cntxt->target_cell_type = GERAN_CELL;
		    }
		    else
		    {
			RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
				"GERAN DAHO not present.");
		    }
		}
		break;
	    case RRM_IRAT_PRIORITY_CDMA2000:
		{   
		    /* DAHO of CDMA not supported currently. */
		}
		break;
	    default:
		break;
	}
        if(INVALID_PHYSICAL_CELL_ID != daho_pci)
        {
	    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
		    "DAHO PCI: [%d] of RAT TYPE [%d] target_cell_type [%d].", daho_pci, rat_type,
                    p_ue_cntxt->target_cell_type);
            break;
        }
        else
        {
            index_rat++;
        }
    }

    /* If PCI invalid then (No DAHOs configured) then return Failure */
    if(INVALID_PHYSICAL_CELL_ID == daho_pci)
    {
	ret_val = RRM_FAILURE;
    }
    /* If PCI valid then send HO Required to that DAHO PCI */
    else
    {
        /* SPR 15674 Start */
        phy_cell_id_info.phy_cell_id  = daho_pci;
        phy_cell_id_info.carr_earfcn  = daho_freq;
        transaction_id = rrm_generate_txn_id();
        ret_val = rrm_build_and_send_ho_required (phy_cell_id_info,
                p_ue_cntxt,
                transaction_id);   
        /* SPR 16406 8SEP start */
        /* Code removed */
        /* SPR 16406 8SEP end */
        /* SPR 15674 End */
	RRM_TRACE(g_cellm_log_on_off,
		p_rrm_cellm_facility_name, 
		RRM_BRIEF, 
		"%s: Build & send HO Required (%s)", 
		"UE:", ret_val?"SUCCESS":"FAILED");	
    }

    /* Return Success if Ho required sent out successfully else Failure */
    RRM_UT_TRACE_ENTER();
    return ret_val;
}
/* SPR 13445 Fix End */

/****************************************************************************
 * Function Name  : rrm_ue_process_timer_msg
 * Inputs         : Timer ID, Timer Buffer and context
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function facilitates ue_manager to trigger the event
 *                  TIMER expiry with respect to the UE.
 ****************************************************************************/
void rrm_ue_process_timer_msg (QTIMER timer_id, void *p_buf, void *p_context)
{
    /* CSR_00040799_CHANGES_START */
    rrm_ue_index_t          ue_index     = RRM_INVALID_UE_ID;
    /* CSR_00040799_CHANGES_END */
    rrm_cell_index_t        cell_index   = RRM_ZERO;
    rrm_return_et           ret_val      = RRM_SUCCESS;
    rrm_bool_et             return_flag  = RRM_TRUE;  
    /* SPR 20652 Fix Start */
    /* Code Removed */ 
    /* SPR-20656 START */
    /*Code Removed*/
    /* SPR-20656 END */
    rrm_ue_context_t       *p_ue_ctx     = RRM_PNULL;
    /* SPR 20652 Fix End */
    rrm_ue_timer_buf_t     *p_timer_buf  = RRM_PNULL;
    /* SPR 21527 Start */
    U16 num_active_ue                     = RRM_NULL;
    /* SPR 21527 End */
    /* Soft Lock start - Global variable removal */
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    /* coverity : CID 31014 */
    /* SPR 17784 Start */ 
    rrm_ue_timer_buf_t     ecid_meas_timer_buff =
    {RRM_UE_DELAY_HO_TIMER,RRM_ZERO,RRM_ZERO,RRM_PNULL,RRM_ZERO,RRM_ZERO,RRM_ZERO,RRM_ZERO};
    /* SPR 17784 End */ 
    S32                    ue_started_meas_time_in_ms = RRM_ZERO;
    U32                    esmlc_meas_id = RRM_ZERO;
    rrm_cause_t            cause = {RRM_ZERO};
    rrm_ue_context_t       *p_ue_cntxt = RRM_PNULL;
    /* RACH_OPTIMIZATION_CHANGES_START */
    rrm_ue_global_context_t *p_g_rrm_ue_ctx = RRM_PNULL;
    /* RACH_OPTIMIZATION_CHANGES_END */
    rrm_bool_et         send_esmlc_meas_failure_ind = RRM_FALSE;
    rrm_bool_et         send_esmlc_meas_report = RRM_FALSE;
    /* Bug_944_start */
    U16 rrm_prfed_arfcn_for_redirect              = RRM_ZERO;
    rrm_bool_et is_redirect_of_ue_success               = RRM_TRUE;
    rrm_daho_cell_config_info_t         *p_daho_cell  = RRM_PNULL;
    rrm_best_cell_for_ue_node_t         *p_best_cell_temp = RRM_PNULL;
    rrm_bool_et rrm_need_find_freq_frm_idle_mode        = RRM_TRUE;
    /* Bug_944_end */
    /* Soft Lock end - Global variable removal */

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_buf);
    RRM_ASSERT (RRM_PNULL != p_context);

    p_timer_buf   = (rrm_ue_timer_buf_t *)p_buf;
    /* SPR_10729_fix start */
    cell_index = p_timer_buf->cell_index;
    p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        return;
    }
    /* SPR_10729_fix end */

    /* RACH_OPTIMIZATION_CHANGES_START */
    p_g_rrm_ue_ctx = (rrm_ue_global_context_t *)p_context;
    /* RACH_OPTIMIZATION_CHANGES_END */

    RRM_ASSERT (RRM_PNULL != p_timer_buf);

    /* SPR 15822 Fix Start */
    RRM_TRACE(g_uem_log_on_off,
            p_rrm_uem_facility_name, 
            RRM_BRIEF,
            "Timer expired: Timer [%p] TimeType [%u]",
            timer_id, p_timer_buf->timer_type);
    /* SPR 15822 Fix End */

    /* Timer expiry proessing for Cell Block HO Req */
    if (RRM_UE_CELL_BLOCKED_W_FOR_HO_TIMER == p_timer_buf->timer_type)
    {

        /* SPR 21527 Start */
        num_active_ue = rrm_cellm_get_num_active_ue(cell_index);
        /* SPR 21527 End */
        if (num_active_ue)
        {
            /* Soft Lock start - Global variable removal */
            p_cell_ctxt->administrative_procedure_ongoing = RRM_FALSE;
            /* Soft Lock end - Global variable removal */
            ret_val = rrm_uem_send_rmif_non_emrgncy_active_calls_ho_resp (p_timer_buf->trans_id,
                    p_timer_buf->cell_index,
                    RRM_FALSE);
        }
        else
        {
            /* Soft Lock start - Global variable removal */
            p_cell_ctxt->administrative_procedure_ongoing = RRM_FALSE;
            /* Soft Lock end - Global variable removal */
            ret_val = rrm_uem_send_rmif_non_emrgncy_active_calls_ho_resp (p_timer_buf->trans_id,
                    p_timer_buf->cell_index,
                    RRM_TRUE);
        }
        if (RRM_SUCCESS == ret_val)
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "UEM Successfully sent non_emrgncy_active_calls_ho_resp to MIF");
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                    "UEM Failed to Send non_emrgncy_active_calls_ho_resp to MIF");
        }
    }
    /* SPR-20656 START */
    /* code deleted */
    /* SPR-20656 END */
    /* Bug_944_start */
    else if (RRM_UE_W_FOR_MLB_REDIRECT_MEAS_IND == p_timer_buf->timer_type)
    {
        ue_index      = p_timer_buf->ue_index;
        cell_index    = p_timer_buf->cell_index;
        RRM_TRACE (g_uem_log_on_off,
                p_g_rrm_uem_facility_name,
                RRM_BRIEF,
                "MLB_REDIRECT timer(%p) expired for [UE:%d] on Cell_Idx (%d)",
                timer_id,
                ue_index,
                cell_index);
        /* SPR 20652 Fix Start */
        p_ue_cntxt = rrm_ue_find_context (ue_index, cell_index);
        if (RRM_PNULL == p_ue_cntxt)
        {
            RRM_TRACE (g_uem_log_on_off,
                    p_g_rrm_uem_facility_name,
                    RRM_ERROR,
                    "Unable to find context for [UE:%d] and Cell_Idx (%d)",
                    ue_index,
                    cell_index);
            RRM_MEM_FREE(p_timer_buf);
            RRM_UT_TRACE_EXIT();
            return;
        }
        /* SPR 20652 Fix End */
        /*Start: BUG 974*/
        if(RRM_CARRIER_REDIRECT_ON_UE != p_ue_cntxt->action_triggered)
        {
            RRM_MEM_FREE(p_timer_buf);
            RRM_UT_TRACE_EXIT();
            return;
        }
        /*End: BUG 974*/
        p_best_cell_temp = (rrm_best_cell_for_ue_node_t *)ylFirst(&p_ue_cntxt->best_cell_for_ue);
        if (RRM_PNULL == p_best_cell_temp)
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                    "No Measurement is received");
            p_daho_cell = rrm_cellm_get_daho_cell_config_info_params(cell_index);
            /* SPR 14336 Fix - Bitmask check corrected for inter frequency */
            if((RRM_PNULL != p_daho_cell) && (EUTRA_INTER_FREQ_DAHO_CELL_PRESENT & p_daho_cell->bitmask))
            {
                rrm_prfed_arfcn_for_redirect = rrm_find_daho_cell_freq (p_daho_cell, p_cell_ctxt);
                /* (rrm_prfed_arfcn_for_redirect == RRM_ZERO) indicates DAHO cells is not found */
                if (RRM_ZERO != rrm_prfed_arfcn_for_redirect)
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "DAHO Cell is configured: Redirect [UE:%d] to DAHO CELL freq [%u]",p_ue_cntxt->ue_index,
                            rrm_prfed_arfcn_for_redirect);
                    if (RRM_SUCCESS != (ret_val = rrm_build_and_send_ue_connection_release_ind_carrier_redir
                                (p_ue_cntxt->ue_index,
                                 p_ue_cntxt->cell_index,
                                 rrm_prfed_arfcn_for_redirect, RADIO_NETWORK_LAYER_TYPE,
                                 REDUCE_LOAD_IN_SERVING_CELL,
                                 RRM_ZERO)))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                                "Failed to send Carrier redirection UE connection release indication \
                                for [UE:%d]",p_ue_cntxt->ue_index);
                        is_redirect_of_ue_success = RRM_FALSE;
                    }
                    rrm_need_find_freq_frm_idle_mode = RRM_FALSE;
                }
                /* If DAHO cell freq not found then find freq from idle mode params */
                else
                {
                    rrm_need_find_freq_frm_idle_mode = RRM_TRUE;
                }    
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "DAHO Cell is not configured");
                rrm_need_find_freq_frm_idle_mode = RRM_TRUE;
            }
        }
        else
        {
            if (RRMCM_RMIF_LTE_NCL_PRESENT & p_cell_ctxt->ran_info.ncl_params.bitmask)
            {
                rrm_prfed_arfcn_for_redirect = rrm_sort_freq_of_neighbour_cell_on_load(p_ue_cntxt, p_cell_ctxt);

                /* (rrm_prfed_arfcn_for_redirect == RRM_ZERO) indicates all reported cells are in high load */
                if (rrm_prfed_arfcn_for_redirect != RRM_ZERO)
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "Redirect [UE:%d] to freq [%u]", p_ue_cntxt->ue_index, rrm_prfed_arfcn_for_redirect);
                    if (RRM_SUCCESS != (ret_val = rrm_build_and_send_ue_connection_release_ind_carrier_redir
                                (p_ue_cntxt->ue_index,
                                 p_ue_cntxt->cell_index,
                                 rrm_prfed_arfcn_for_redirect, RADIO_NETWORK_LAYER_TYPE,
                                 REDUCE_LOAD_IN_SERVING_CELL,
                                 RRM_ZERO)))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                                "Failed to send Carrier redirection UE connection release indication "
                                "for [UE:%d]",p_ue_cntxt->ue_index);
                        is_redirect_of_ue_success = RRM_FALSE;
                    }
                    rrm_need_find_freq_frm_idle_mode = RRM_FALSE;
                }
                /* Redirect the UE on DAHO Frequency */
                else
                {
                    p_daho_cell = rrm_cellm_get_daho_cell_config_info_params(p_ue_cntxt->cell_index);
                    /* SPR 14336 Fix - Bitmask check corrected for inter frequency */
                    if((RRM_PNULL != p_daho_cell) && (EUTRA_INTER_FREQ_DAHO_CELL_PRESENT & p_daho_cell->bitmask))
                    {
                        rrm_prfed_arfcn_for_redirect = rrm_find_daho_cell_freq (p_daho_cell, p_cell_ctxt);
                        /* (rrm_prfed_arfcn_for_redirect == RRM_ZERO) indicates DAHO cells is not found */
                        if (RRM_ZERO != rrm_prfed_arfcn_for_redirect)
                        {
                            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                    "DAHO Cell is configured: Redirect [UE:%d] to DAHO CELL freq [%u]",p_ue_cntxt->ue_index,
                                    rrm_prfed_arfcn_for_redirect);
                            if (RRM_SUCCESS != (ret_val = rrm_build_and_send_ue_connection_release_ind_carrier_redir
                                        (p_ue_cntxt->ue_index,
                                         p_ue_cntxt->cell_index,
                                         rrm_prfed_arfcn_for_redirect, RADIO_NETWORK_LAYER_TYPE,
                                         REDUCE_LOAD_IN_SERVING_CELL,
                                         RRM_ZERO)))
                            {
                                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                                        "Failed to send Carrier redirection UE connection release indication "
                                        "for [UE:%d]",p_ue_cntxt->ue_index);
                                is_redirect_of_ue_success = RRM_FALSE;
                            }
                            rrm_need_find_freq_frm_idle_mode = RRM_FALSE;
                        }
                        /* If DAHO cell freq not found then find freq from idle mode params */
                        else
                        {
                            rrm_need_find_freq_frm_idle_mode = RRM_TRUE;
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                "DAHO Cell is not configured for [UE:%d]",p_ue_cntxt->ue_index);
                        rrm_need_find_freq_frm_idle_mode = RRM_TRUE;
                    }
                }
            }

        }
        if (RRM_TRUE == rrm_need_find_freq_frm_idle_mode)
        {
            rrm_prfed_arfcn_for_redirect = rrm_fnd_highst_frq_frm_idle_mode_prms(p_ue_cntxt, p_cell_ctxt);
            if (RRM_ZERO != rrm_prfed_arfcn_for_redirect)
            {
                if (RRM_SUCCESS != (ret_val = rrm_build_and_send_ue_connection_release_ind_carrier_redir
                            (p_ue_cntxt->ue_index,
                             p_ue_cntxt->cell_index,
                             rrm_prfed_arfcn_for_redirect, RADIO_NETWORK_LAYER_TYPE,
                             REDUCE_LOAD_IN_SERVING_CELL,
                             RRM_ZERO)))
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                            "Failed to send Carrier redirection UE connection release indication"
                            " for [UE:%d]",p_ue_cntxt->ue_index);
                    is_redirect_of_ue_success = RRM_FALSE;
                }

            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                        "Best frequency for UE redirection is not found for [UE:%d]",p_ue_cntxt->ue_index);
                is_redirect_of_ue_success = RRM_FALSE;
            }
        }
        if (RRM_TRUE != is_redirect_of_ue_success)
        {
            /*release the ue. No inter freq cell found for this UE */
            if (RRM_SUCCESS == (ret_val = rrm_build_and_send_ue_connection_release_ind
                        (p_ue_cntxt->ue_index,
                         RADIO_NETWORK_LAYER_TYPE,
                         REDUCE_LOAD_IN_SERVING_CELL, 
                         RRM_ONE,
                         RRM_PNULL,
                         /* SPR_10729_fix */
                         cell_index)))
            {    
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "ue_connection_release_ind sent ");
                if(RRM_FAILURE == rrm_ue_resource_clean_up(p_ue_cntxt))
                {
                    RRM_TRACE(g_uem_log_on_off,
                            p_g_rrm_uem_facility_name,
                            RRM_ERROR,
                            "Failed to clean up ue resources for the [UE:%d]",
                            ue_index);
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off,
                            p_g_rrm_uem_facility_name,
                            RRM_DETAILED,
                            "successfully cleaned up resources for the [UE:%d]",
                            ue_index);
                }
            }

        }

    }
    /* Bug_944_end */
    /* Ue positioing code changes start */
    else if (p_timer_buf->timer_type >= RRM_UE_ECID_MEAS_TIMER_1 &&
            p_timer_buf->timer_type <= RRM_UE_ECID_MEAS_TIMER_15)
    {
        ue_index      = p_timer_buf->ue_index;
        cell_index    = p_timer_buf->cell_index;
        /* SPR 20652 Fix Start */
        SET_CELL_AND_UE_INDEX(cell_index, ue_index);
        p_ue_cntxt = rrm_ue_find_context (ue_index, cell_index);
        if (NULL == p_ue_cntxt) 
        {
            RRM_TRACE (g_cellm_log_on_off,
                    p_rrm_cellm_facility_name,
                    RRM_ERROR,
                    "Unable to find context for [UE:%d] and Cell_Idx (%d)",
                    ue_index,
                    cell_index);
            RRM_MEM_FREE(p_timer_buf);
            return;
        }
        else
        {
            s1ap_rrm_lppa_ecid_meas_report_t   *p_lppa_ecid_meas_report=RRM_PNULL;
            U32                          timer_duration = RRM_ZERO;
            esmlc_meas_id = p_timer_buf->timer_type- RRM_UE_ECID_MEAS_TIMER_1;
            cause.type    = RRM_ECID_RADIO_NW_LAYER_CAUSE;
            cause.value   = RRM_RNL_REQUESTED_ITEM_TEMPORARILLY_NOT_AVAILABLE;
            /* SPR 20652 Fix End */

            RRM_TRACE (g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED, 
                    "For UE positioning timer, timer type = %d and esmlc meas id = %d", p_timer_buf->timer_type, esmlc_meas_id);

            /* SPR 21496 Fix Start */ 
            if(p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_req_received )
            {
                if( p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].timer_expire_count < RRM_TEN)
                {
                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].timer_expire_count++;
                }
                if (p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status & RRM_ECID_MAC_MEAS_REQD)
                {
                    if(RRM_SUCCESS == check_if_requested_quantifier_is_received(p_ue_cntxt, esmlc_meas_id) &&
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_report_sent == RRM_FALSE)
                    {
                        send_esmlc_meas_report = RRM_TRUE;
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                " Send ESMLC meas reports ue_mac_meas_status=%d for e_smlc_ue_meas_id=%d  [UE:%d]",
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status,
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].e_smlc_ue_meas_id,
                                p_ue_cntxt->ue_index);
                    }
                    else if((p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].timer_expire_count > RRM_ONE ) && 
                            (p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_report_sent == RRM_FALSE))
                    {
                        send_esmlc_meas_failure_ind = RRM_TRUE;
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                                "Either MAC or UE reports (ue_mac_meas_status=%d) is  not received properly for e_smlc_ue_meas_id=%d  [UE:%d]",
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status,
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].e_smlc_ue_meas_id,
                                p_ue_cntxt->ue_index);

                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                "ecid_meas_report_sent=%d ue_mac_meas_status=%d for e_smlc_ue_meas_id=%d  [UE:%d]",
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_report_sent,
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status,
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].e_smlc_ue_meas_id,
                                p_ue_cntxt->ue_index);
                        if(RRM_LPPA_ONDEMAND == p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].report_type &&
                                p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_report_sent == RRM_TRUE)
                        {
                            RRM_TRACE (g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED, 
                                    "ONDEMAND ECID=%d ECID report sent so clear the ECID_INDEX=%d data of [UE:%d]", 
                                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].e_smlc_ue_meas_id,
                                    esmlc_meas_id,
                                    ecid_meas_timer_buff.ue_index);
                            rrm_ue_delete_meas_id_info_for_ecid_meas(p_ue_cntxt, esmlc_meas_id);
                            rrm_clean_ue_ecid_meas_related_info(p_ue_cntxt, esmlc_meas_id);
                        }
                        else
                        {
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_report_sent = RRM_FALSE;
                            if(RRM_LPPA_ONDEMAND == p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].report_type)
                            {
                                /* Coverity_ID : 55182 */
                                timer_duration = rrm_get_meas_priodicity_in_ms(RRM_LPPA_MS1024);
                            }
                            else
                            {
                                /* coverity : CID 31013 */
                                timer_duration = rrm_get_meas_priodicity_in_ms(p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].report_interval);
                            }
                            RRM_MEMSET (&ecid_meas_timer_buff,RRM_ZERO,sizeof(rrm_ue_timer_buf_t));

                            ecid_meas_timer_buff.timer_type = rrm_get_ecid_meas_timer_type(esmlc_meas_id);
                            ecid_meas_timer_buff.ue_index = p_ue_cntxt->ue_index;
                            ecid_meas_timer_buff.cell_index = p_ue_cntxt->cell_index;
                            /* CSR-58333-fix start*/
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_pos_timer =
                                rrm_ue_start_timer(
                                        timer_duration,
                                        &ecid_meas_timer_buff,
                                        sizeof(rrm_ue_timer_buf_t),
                                        RRM_FALSE);
                            /* CSR-58333-fix end*/
                            RRM_TRACE (g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED, 
                                    "ECID=%d Timer=%d started for ue_mac_meas_status =%d next measurement results of [UE:%d]", 
                                    esmlc_meas_id,
                                    timer_duration,
                                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status,
                                    ecid_meas_timer_buff.ue_index);
                            if(!(p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status & RRM_ECID_UE_MEAS_REQD) &&
                                    (p_ue_cntxt->is_mac_req_initiated == RRM_FALSE))
                            {
                                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                        "Send MAC ecid meas request when only TA2/AOA is required for [UE:%d]"
                                        "otherwise MAC ecid meas request will be sent from meas_results_ind",
                                        p_ue_cntxt->ue_index);

                                if(RRM_FAILURE == rrm_build_and_send_mac_ecid_meas_report_req(p_ue_cntxt,ue_started_meas_time_in_ms)) 
                                {

                                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                            "Failure obtained for sending Mac meas report request for [UE:%d]",
                                            p_ue_cntxt->ue_index);

                                    ret_val = RRM_FAILURE;
                                }
                                else
                                {
                                    p_ue_cntxt->is_mac_req_initiated = RRM_TRUE;
                                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                            "MAC meas request again  sent successfully to MAC for [UE:%d]",
                                            p_ue_cntxt->ue_index);
                                }
                            }
                            else
                            {
                                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                                        "MAC meas request was already sent for [UE:%d]",
                                        p_ue_cntxt->ue_index);
                            }
                        }
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                            "ESMLC meas req for UE meas ue_mac_meas_status=%d for e_smlc_ue_meas_id=%d  [UE:%d]",
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status,
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].e_smlc_ue_meas_id,
                            p_ue_cntxt->ue_index);
                    if(RRM_SUCCESS == check_if_requested_quantifier_is_received(p_ue_cntxt, esmlc_meas_id))
                    {
                        send_esmlc_meas_report =RRM_TRUE;
                    }
                    else
                    {
                        send_esmlc_meas_failure_ind = RRM_TRUE;
                    }

                }
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                        "send_esmlc_meas_report =%d , send_esmlc_meas_failure_ind=%d", 
                        send_esmlc_meas_report, send_esmlc_meas_failure_ind);
                if(send_esmlc_meas_report == RRM_TRUE)
                {
                    p_lppa_ecid_meas_report  = (s1ap_rrm_lppa_ecid_meas_report_t *)
                        rrm_mem_get(sizeof(s1ap_rrm_lppa_ecid_meas_report_t));
                    if (RRM_PNULL == p_lppa_ecid_meas_report)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,"Failed to Allocate Memory");
                        /* Coverity_ID : 53252 */
                        ret_val = RRM_FAILURE;
                    }
                    else
                    {
                        if(RRM_SUCCESS == check_if_requested_quantifier_is_received(p_ue_cntxt, esmlc_meas_id))
                        {
                            /* Send meas report */
                            ret_val = rrm_ue_build_and_send_combined_ecid_meas_report(p_ue_cntxt, 
                                    p_lppa_ecid_meas_report, 
                                    esmlc_meas_id);

                            if(RRM_SUCCESS == ret_val)
                            {
                                RRM_TRACE (g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, 
                                        "Combined meas report sent successfully for esmlc_meas_index[%d] for [UE:%d]", 
                                        esmlc_meas_id, ue_index);

                                /* Start timer */
                                if(RRM_LPPA_ONDEMAND == p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].report_type)
                                {
                                    rrm_ue_delete_meas_id_info_for_ecid_meas(p_ue_cntxt, esmlc_meas_id);
                                    rrm_clean_ue_ecid_meas_related_info(p_ue_cntxt, esmlc_meas_id);
                                }
                                else
                                {
                                    /* Coverity_ID : 55182 */
                                    timer_duration = rrm_get_meas_priodicity_in_ms(
                                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].report_interval);
                                    RRM_MEMSET (&ecid_meas_timer_buff,0,sizeof(rrm_ue_timer_buf_t));

                                    ecid_meas_timer_buff.timer_type = rrm_get_ecid_meas_timer_type(esmlc_meas_id);
                                    ecid_meas_timer_buff.ue_index = p_ue_cntxt->ue_index;
                                    ecid_meas_timer_buff.cell_index = p_ue_cntxt->cell_index;
                                    /* CSR-58333-fix start*/
                                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_pos_timer =
                                        rrm_ue_start_timer(
                                                timer_duration ,
                                                &ecid_meas_timer_buff,
                                                sizeof(rrm_ue_timer_buf_t),
                                                RRM_FALSE);
                                    /* CSR-58333-fix end*/
                                    /* Re-setting the bitmasks for RECVD statuses */
                                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status =
                                        p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status & ~(RRM_ECID_UE_MEAS_RECVD);
                                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status =
                                        p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status & ~(RRM_ECID_MAC_MEAS_RECVD);
                                }
                            }
                            else
                            {
                                RRM_TRACE (g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR, 
                                        "Combined meas report sending unsuccessful for esmlc_meas_index[%d] for [UE:%d]", 
                                        esmlc_meas_id, ue_index);
                                send_esmlc_meas_failure_ind = RRM_TRUE;
                            }
                        }
                        else
                        {
                            send_esmlc_meas_failure_ind = RRM_TRUE;
                        }
                        RRM_MEM_FREE(p_lppa_ecid_meas_report);
                    }
                    p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ecid_meas_report_sent = RRM_FALSE;
                }
                if(send_esmlc_meas_failure_ind == RRM_TRUE)
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                            "ECID Measurement reports unsuccessful for [UE:%d]",
                            p_ue_cntxt->ue_index);

                    ret_val = rrm_build_and_send_ecid_meas_failure_indication(p_ue_cntxt->ue_index,
                            p_ue_cntxt->cell_index,
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].mme_id,
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].routing_id,
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].lppa_trans_id,
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].e_smlc_ue_meas_id,
                            p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].enb_ue_meas_id,cause);

                    if(ret_val == RRM_SUCCESS)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                                "Sent ECID Measurement failure indication successfully for [UE:%d]",
                                p_ue_cntxt->ue_index);
                    }

                    if((p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].report_type == RRM_LPPA_PERIODIC) &&
                            (p_ue_cntxt->p_ue_positioing_data[esmlc_meas_id].ue_mac_meas_status & RRM_ECID_UE_MEAS_REQD))
                    {
                        rrm_ue_meas_config_remove_for_ecid(p_ue_cntxt,
                                esmlc_meas_id,
                                RRM_FALSE);
                        rrm_clean_ue_ecid_meas_related_info(p_ue_cntxt,esmlc_meas_id);
                    }
                    else
                    {
                        rrm_ue_delete_meas_id_info_for_ecid_meas(p_ue_cntxt,esmlc_meas_id);
                        rrm_clean_ue_ecid_meas_related_info(p_ue_cntxt,esmlc_meas_id);
                    }
                }
            }
            /* SPR 21496 Fix End */ 
            else
            {
                RRM_TRACE (g_cellm_log_on_off,
                        p_rrm_cellm_facility_name,
                        RRM_ERROR,
                        "ecid_meas_req_received FLAG is FALSE during timer processing for [UE:%d] and Cell_Idx (%d)",
                        ue_index,
                        cell_index);
            }
        }
    }
    /* Ue positioing code changes end */
    /* RACH_OPTIMIZATION_CHANGES_START */
    else if (RRM_UE_RACH_REPORT_TIMER == p_timer_buf->timer_type)
    {
        /*SPR 20439 Fix Start */
        if (p_g_rrm_ue_ctx->rrm_uem_rach_info_data.is_ue_rach_report_active == RRM_TRUE)
        {
            RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_DETAILED,"As timer \
                    for UE RACH report has has expired, so providing stored data to SON");
            post_stored_data_to_son(p_g_rrm_ue_ctx,p_timer_buf->cell_index);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_DETAILED,"As timer \
                    for UE RACH report has expired, and UE RACH report is not active, \
                    so not sending data to SON and setting report as ACTIVE \
                    to start collecting RACH data for current time duration");
            p_g_rrm_ue_ctx->rrm_uem_rach_info_data.is_ue_rach_report_active = RRM_TRUE;

        }
        /*SPR 20439 Fix End */
    }
    /* RACH_OPTIMIZATION_CHANGES_END */
    /*spr 7996 start*/
    else if(RRM_UE_CSG_GUARD_TIMER == p_timer_buf->timer_type)
    {
        /* SPR 20652 Fix Start */
        p_ue_cntxt  = rrm_ue_find_context (p_timer_buf->ue_index, p_timer_buf->cell_index);
        if (RRM_PNULL != p_ue_cntxt)
        {
            /* SPR 13445 Fix Start */
            /* Try doing HO to DAHO cells configured by Operator */
            ret_val = rrm_try_daho_on_csg_timer_expiry( p_ue_cntxt );
            /* If HO required to DAHO fails then release UE anyhow */
            if(ret_val == RRM_FAILURE)
            {
                ret_val=rrm_release_ue(p_ue_cntxt);
                /* SPR 20652 Fix End */
                /* Coverity 74385 Fix Start */
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                            "Failed in rrm_release_ue");
                }
                /* Coverity 74385 Fix End */
            }
            /* Else if HO triggered successfully then do not release UE as it will 
               be released eventually when HO is successful */
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO, "[UE:%d]CELL_INDEX[%d]"
                        "Successfully sent HO Reqd. to DAHO when UE's CSG membership changed on RRM_UE_CSG_GUARD_TIMER expiry."
                        ,p_timer_buf->ue_index, p_timer_buf->cell_index);
            }
            /* SPR 13445 Fix End */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO, "[UE:%d]CELL_INDEX[%d]"
                    " failed to find ue context, either ue handed over or released anyway",p_timer_buf->ue_index, p_timer_buf->cell_index);
        }
    }
    /*spr 7996 end*/
    else
    {
        ue_index      = p_timer_buf->ue_index;
        cell_index    = p_timer_buf->cell_index;
        /* BUG_876_CHANGES_START*/
        RRM_TRACE (g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
                "Timer(0x%p)(type:%d) expired for [UE:%d] and Cell_Idx(%d)",
                timer_id, p_timer_buf->timer_type, ue_index, cell_index);
        /* BUG_876_CHANGES_END*/

        /* SPR 20652 Fix Start */
        p_ue_ctx = rrm_ue_find_context (ue_index, cell_index);
        if (NULL == p_ue_ctx) {
            RRM_TRACE (g_cellm_log_on_off,
                    p_rrm_cellm_facility_name,
                    RRM_ERROR,
                    "Unable to find context for [UE:%d] and Cell_Idx (%d)",
                    ue_index,
                    cell_index);
            return;
        }
        else {
            SET_CELL_AND_UE_INDEX(cell_index, ue_index);
            RRM_TRACE (g_cellm_log_on_off,
                    p_rrm_cellm_facility_name,
                    RRM_WARNING,
                    "Timer(0x%p)(type:%d) expired for [UE:%d] and Cell_Idx(%d)",
                    timer_id,
                    p_timer_buf->timer_type,
                    ue_index,
                    cell_index);
            /* SPR 17784 Start */ 
            p_ue_ctx->ue_event = RRM_UE_TIMER_EXPIRY_EVENT;
            /* SPR 17784 End */ 

            switch (p_timer_buf->timer_type) {
                case RRM_UE_BLIND_HO_TIMER:
                    /* UT Defect eNB_RRM_MC_TC_358_1 fix starts */
                    /* SPR-17852 START */
                    /* code deleted */
                    /* SPR-17852 END */
                    /* UT Defect eNB_RRM_MC_TC_358_1 fix ends */
                    break;
                case RRM_UE_CSF_BLIND_TIMER:
                    p_ue_ctx->ho_params.bitmask |= \
                                                   RRM_UE_CSFB_BLIND_TIMER_EXPIRED;
                    break;
                    /* SPR 17784 Start */ 
                case RRM_UE_CGI_TIMER:
                    p_ue_ctx->ue_event = RRM_UE_CGI_TIMER_EXPIRY_EVENT;
                    break;
                    /* SPR 17784 End */

                default:
                    RRM_TRACE (g_cellm_log_on_off,
                            p_rrm_cellm_facility_name,
                            RRM_ERROR,
                            "Unknown Timer Type (%d) for Timer-Id (0x%p)",
                            p_timer_buf->timer_type,
                            timer_id);
                    return_flag = RRM_FALSE;
                    break;
            }
        }
        if(RRM_TRUE == return_flag)
        {
            /* CSR_00040799_CHANGES_START */
            if (p_timer_buf->ue_index != RRM_INVALID_UE_ID)
                /* CSR_00040799_CHANGES_END */
            {
                rrm_ue_global_context_t     g_rrm_ue_ctx;
                RRM_MEMSET (&g_rrm_ue_ctx,RRM_ZERO,sizeof(rrm_ue_global_context_t));

                g_rrm_ue_ctx.p_ue_context = p_ue_ctx;
                /* SPR 17784 Start */ 
                /*Code Moved above */
                /* SPR 17784 End */ 

                p_ue_ctx->current_procedure.p_proc_data = RRM_PNULL;
                /** Free the memory before enter into the UE FSM */
                if ( RRM_UE_MAX_STATE > p_ue_ctx->ue_state ) 
                {
                    /************ UEM FSM TRIGERRED **************************/
                    /* SPR 17784 Start */ 
                    ret_val = rrm_ue_mgr_timer_event_handler (
                            p_ue_ctx->ue_state,
                            p_ue_ctx->ue_event,
                            &g_rrm_ue_ctx);
                    /* SPR 17784 End */ 
                    /*********** UEM FSM *************************************/
                    if(RRM_FAILURE == ret_val)
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_g_rrm_uem_facility_name, 
                                RRM_ERROR,
                                "Error processing the message for [UE:%d], "
                                "Event[%d] in State[%d]",
                                p_ue_ctx->ue_index,
                                p_ue_ctx->ue_event,
                                p_ue_ctx->ue_state);
                        /* SPR 20652 Fix End */
                    }
                }
            }
        }
    }

    RRM_UT_TRACE_EXIT();
    return;
}

/****************************************************************************
 * Function Name  : rrm_ue_handle_ue_timer_expiry
 * Inputs         : rrm_global_context_t *p_g_rrm_ue_ctx RRM Global Context.
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Handles the timer expiry event for blind(DAHO) and DELAY .
 ****************************************************************************/
rrm_return_et
rrm_ue_handle_ue_timer_expiry (rrm_ue_global_context_t *p_g_rrm_ue_ctx)
{
    rrm_return_et       ret_val        = RRM_SUCCESS;
    rrm_ue_context_t*   p_ue_context   = RRM_PNULL;
    rrm_ho_params_t*    p_ho_params    = RRM_PNULL;


    p_ue_context = (rrm_ue_context_t*)p_g_rrm_ue_ctx->p_ue_context;
    p_ho_params  = (rrm_ho_params_t*)&p_ue_context->ho_params;

    RRM_UT_TRACE_ENTER();

    /* SPR-17852 START */
    if ((p_ho_params->blind_timer) &&
            !(qvTimerRunning(p_ho_params->blind_timer))) 
    {
    /* SPR-17852 END */
        rrm_ue_process_blind_timer_expiry(p_g_rrm_ue_ctx);
    }
    /* Inter RAT changes start */
    if((p_ue_context->ho_params.csfb_blind_timer) && \
            (p_ue_context->ho_params.bitmask & RRM_UE_CSFB_BLIND_TIMER_EXPIRED))
    {
        rrm_ue_process_csfb_blind_timer_expiry(p_g_rrm_ue_ctx);
    }
    /* Inter RAT changes end */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/* Inter RAT changes start */
/****************************************************************************
 ** Function Name  : rrm_ue_validate_utran_cell
 ** Inputs         : RRM global context
 **					 Physical Cell Id
 ** Outputs        : Pointer to cell having given PCI
 ** Returns        : RRM_SUCCESS/RRM_FAILURE
 ** Description    : Validates the given PCI against the HO restriction list
 **					 and OAM configurations
 *****************************************************************************/
/* SRVCC SUPPORT START */
/* SPR_15241_Start */
utran_freq_cells_t *
rrm_ue_validate_utran_cell(rrm_ue_context_t* p_ue_context, U16 pci, rrm_bool_et *p_ue_support_voip)
{
    /* SPR_15241_End */
/* SRVCC SUPPORT END */
	U16				irat_count;
    /* SPR-11679-fix start */
    /* INVALID_PHYSICAL_CELL_ID used as a default value for the count */
	U16				valid_irat_count = INVALID_PHYSICAL_CELL_ID;
    /* SPR-11679-fix end */
	U16				phy_cell_id = INVALID_PHYSICAL_CELL_ID;
		
	utran_freq_cells_t* 		p_utra_cell  = RRM_PNULL;

	inter_rat_ncl_t			   *p_ncl_params	 = \
			rrm_cellm_get_inter_rat_ncl_info(p_ue_context->cell_index);


    /* SPR_15241_Start */
    U32    serv_dl_earfcn = rrm_cellm_get_dl_earfcn(p_ue_context->cell_index);
    /* SPR_15241_End */


    RRM_UT_TRACE_ENTER();

	/* check for OAM configuration */
	for(irat_count = RRM_ZERO; \
			irat_count < p_ncl_params->num_valid_utran_freq_cell;
			irat_count++)
	{
		phy_cell_id = p_ncl_params->utran_freq_cells[irat_count].pcpich_scrambling_code;
		if(pci == phy_cell_id) 
		{
            /* SPR-11679-fix start */
            /*
            ** RRM check whether the target cell supports the same Location Area Code.
            ** If yes, then select the same neighbore infomation.
            ** Otherwise select the other valid neighbore. 
            */ 
            if (INVALID_PHYSICAL_CELL_ID == valid_irat_count)
            {
                valid_irat_count = irat_count;
            }

            if (RRM_ZERO == RRM_MEMCMP(p_ue_context->ue_context_mod_req.registered_lai.lac.data,
                        p_ncl_params->utran_freq_cells[irat_count].rai.lai.lac, RRM_OAM_MAX_NUM_LAC_OCTETS))
            {
			p_utra_cell = &(p_ncl_params->utran_freq_cells[irat_count]);
                valid_irat_count = INVALID_PHYSICAL_CELL_ID;
            /* SPR_15241_Start */
            if(RRM_TRUE == rrm_check_ue_capability_for_voip(p_ue_context, serv_dl_earfcn))
            {
                *p_ue_support_voip = RRM_TRUE;
            }
			break;
            /* SPR_15241_End */
		} 
	}
    }

    if (valid_irat_count != INVALID_PHYSICAL_CELL_ID)
    {

        p_utra_cell = &(p_ncl_params->utran_freq_cells[valid_irat_count]);
    }
    /* SPR-11679-fix end */
	if(p_utra_cell == RRM_PNULL)
	{
		/* This PCI is not configured */
        RRM_TRACE (g_uem_log_on_off, 
					p_g_rrm_uem_facility_name,
					RRM_ERROR, 
					"PCI [%d] not configured",pci);
	}
    
	RRM_UT_TRACE_EXIT();
    /* SRVCC SUPPORT START */
    return p_utra_cell;
    /* SRVCC SUPPORT END */
}
/* Inter RAT changes start */
/* SPR 15674 Start */
/****************************************************************************
 ** Function Name  : rrm_ue_validate_eutran_cell
 ** Inputs         : RRM global context
 **					 Physical Cell Id
 ** Outputs        : None
 ** Returns        : RRM_SUCCESS/RRM_FAILURE
 ** Description    : Validates the given PCI against 
 **					 OAM configurations
 *****************************************************************************/
    rrm_return_et
rrm_ue_validate_eutran_cell(rrm_ue_context_t* p_ue_context,	phy_cell_id_info_t *p_phy_cell_id_info)
{
    rrm_return_et	ret_val = RRM_SUCCESS;
    U16				count;
    U16				phy_cell_id = INVALID_PHYSICAL_CELL_ID;

    intra_freq_cells_t*         p_intra_freq_cells = RRM_PNULL;
    inter_freq_cells_t*         p_inter_freq_cells = RRM_PNULL;

    lte_ncl_t			   *p_ncl_params	 = \
                                               rrm_cellm_get_lte_ncl_info(p_ue_context->cell_index);



    RRM_UT_TRACE_ENTER();

    if(RRM_PNULL != p_ncl_params) /* Coverity: CID 29569 */
    {
        /* check for OAM configuration */
        for(count = RRM_ZERO; \
                count < p_ncl_params->num_valid_intra_freq_cell;
                count++)
        {
            phy_cell_id = p_ncl_params->intra_freq_cells[count].phy_cell_id;
            if(p_phy_cell_id_info->phy_cell_id == phy_cell_id)
            {
                p_intra_freq_cells = &(p_ncl_params->intra_freq_cells[count]);
                p_phy_cell_id_info->carr_earfcn = p_ue_context->carrier_earfcn;
                break;
            } 
        }
        if(p_intra_freq_cells == RRM_PNULL)
        { 
            for(count = RRM_ZERO; \
                    count < p_ncl_params->num_valid_inter_freq_cell;
                    count++)
            {
                phy_cell_id = p_ncl_params->inter_freq_cells[count].phy_cell_id;
                if((p_phy_cell_id_info->phy_cell_id == phy_cell_id)&&
                        (p_phy_cell_id_info->carr_earfcn == p_ncl_params->inter_freq_cells[count].eutra_carrier_arfcn))
                {
                    p_inter_freq_cells = &(p_ncl_params->inter_freq_cells[count]);
                    p_phy_cell_id_info->carr_earfcn= p_ncl_params->inter_freq_cells[count].eutra_carrier_arfcn;
                    break;
                }
            }
            if(p_inter_freq_cells == RRM_PNULL)
            {
                /* This PCI is not configured */
                RRM_TRACE (g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_ERROR, 
                        "PCI [%d] not configured in the intra/inter frequeny neighbors ",p_phy_cell_id_info->phy_cell_id);
                return RRM_FAILURE;
            }
        }
    }
    else
    {
        RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, 
                "NCL params empty");
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 15674 End */

/* SPR 15336 Fix Start */
/****************************************************************************
 ** Function Name  : rrm_ue_validate_geran_cell
 ** Inputs         : RRM global context
 **					 Physical Cell Id
 ** Outputs        : None
 ** Returns        : RRM_SUCCESS/RRM_FAILURE
 ** Description    : Validates the given PCI against the HO restriction list
 **					 and OAM configurations
 *****************************************************************************/
/* SPR 15674 Start */
/* SPR_13721_fix : start */ 
geran_freq_ncl_t *
/* SPR_13721_fix : end */
rrm_ue_validate_geran_cell(rrm_ue_context_t* p_ue_context, rrm_rpt_cell_info_t rrm_rpt_cell_info, U8 *p_dtm_ho_supp)
    /* SPR 15674 End */
    /* SPR 15336 Fix End */
{
    /* SPR_13721_fix : start */
    /*Code removed*/
    /* SPR_13721_fix : end */

	U16				irat_count;
	U16				phy_cell_id = INVALID_PHYSICAL_CELL_ID;
		
	geran_freq_ncl_t* 		p_geran_cell  = RRM_PNULL;
	rrm_ho_restriction_list_t*	p_ho_restriction_list = RRM_PNULL;

	inter_rat_ncl_t			   *p_ncl_params	 = \
			rrm_cellm_get_inter_rat_ncl_info(p_ue_context->cell_index);



	p_ho_restriction_list = &(p_ue_context->ho_restriction_list);
    RRM_UT_TRACE_ENTER();

    /* SPR 15336 Fix Start */
    *p_dtm_ho_supp = RRM_ZERO;
    /* SPR 15336 Fix End */
	/* check for OAM configuration */
	for(irat_count = RRM_ZERO; \
			irat_count < p_ncl_params->num_valid_geran_cell;
			irat_count++)
	{
		phy_cell_id = p_ncl_params->geran_freq_cells[irat_count].pci;
        /* SPR 15674 Start */
        if((rrm_rpt_cell_info.phy_cell_id == phy_cell_id) && 
                (rrm_rpt_cell_info.target_earfcn == p_ncl_params->geran_freq_cells[irat_count].bcch_arfcn))
        {
            p_geran_cell = &(p_ncl_params->geran_freq_cells[irat_count]);
            /* ho_restriction : code changes start */
            /* SPR 17564 Fix Start */
            if(RRM_FALSE == rrm_is_inter_rat_cell_restricted(p_ue_context,
            /* SPR 17564 Fix End */
                        p_geran_cell->lai.plmn_identity,
                        p_geran_cell->lai.lac,
                        GERAN_CELL,
                        p_ho_restriction_list) )
            {
                RRM_TRACE (g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_BRIEF, 
                        "selected GERAN PCI [%d] is not forbidden",rrm_rpt_cell_info.phy_cell_id);
                /* SPR 15336 Fix Start */
                if(p_geran_cell->bitmask & RRM_GERAN_FREQ_CELL_DTM_HO_SUPPORTED_PRESENT)
                {
                    *p_dtm_ho_supp = p_geran_cell->dtm_ho_supported;
                }
                /* SPR 15336 Fix End */

                /* SPR_13721_fix : start */
                /*Code removed*/
                /* SPR_13721_fix : end */
                break;
            }
            else
            {
                RRM_TRACE (g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_ERROR, 
                        "GERAN PCI [%d] is forbidden",rrm_rpt_cell_info.phy_cell_id);
            }
        } 
    }
    /* ho_restriction : code changes end */
    if(p_geran_cell == RRM_PNULL)
    {
        /* This PCI is not configured */
        RRM_TRACE (g_uem_log_on_off, 
                p_rrm_uem_facility_name,
                RRM_ERROR, 
                "GERAN PCI [%d] not configured",rrm_rpt_cell_info.phy_cell_id);
    }

    /* SPR 15674 End */

	RRM_UT_TRACE_EXIT();
    /* SPR_13721_fix : start */
    return p_geran_cell;
    /* SPR_13721_fix : end */
}
/****************************************************************************
 *** Function Name  : rrm_cellm_get_redirected_carrier_freq
 *** Inputs         : UE Context
 ***                  Physical Cell Id
 *** Outputs        : None
 *** Returns        : Carrier Frequency corresponding to the given PCI
 *** Description    : returns the Carrier Frequency of the given PCI
 ******************************************************************************/
U16 rrm_cellm_get_redirected_carrier_freq(
				rrm_ue_context_t*	p_ue_context,
				U16					phy_cell_id)
{
	U16			redirected_carrier_freq = RRM_ZERO;
	U16			count;
	inter_rat_ncl_t			   *p_ncl_params	 = \
			rrm_cellm_get_inter_rat_ncl_info(p_ue_context->cell_index);
	/* Get the Carrier Freq for this PCI */
	for(count = RRM_ZERO; count<p_ncl_params->num_valid_utran_freq_cell;count++)
	{
		if(phy_cell_id == \
						p_ncl_params->utran_freq_cells[count].pci)
		{
			redirected_carrier_freq = \
					p_ncl_params->utran_freq_cells[count].uarfcndl;
			break;
		}
	}
	return redirected_carrier_freq;
}
/****************************************************************************
 *** Function Name  : rrm_ue_start_csfb_blind_timer
 *** Inputs         : UE Context
 *** Outputs        : None
 *** Returns        : QTIMER
 *** Description    : This function is invoked when ERAB_SETUP/UE_CTX_MOD 
                      request comes with CSFB indicator. This function starts 
                      the blind timer CSFB.
 ******************************************************************************/
rrm_timer_t rrm_ue_start_csfb_blind_timer(rrm_ue_context_t *p_ue_context)
{
	rrm_ue_timer_buf_t 		timer_buff;
	rrm_cell_context_t		*p_cell_ctxt = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
	p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context->cell_index);
	
    if(p_ue_context->ho_params.blind_timer)
	{
		/* Stop HO Blind Timer */
        /* CSR-58333-fix start*/
        rrm_ue_stop_timer(p_ue_context->ho_params.blind_timer);
        /* CSR-58333-fix end*/
		p_ue_context->ho_params.blind_timer = RRM_PNULL;
	}
	RRM_MEMSET (&timer_buff,RRM_ZERO,sizeof(rrm_ue_timer_buf_t));
	timer_buff.timer_type = RRM_UE_CSF_BLIND_TIMER;
	timer_buff.ue_index = p_ue_context->ue_index;
    /* Bug_12469_Fix: Start */
    timer_buff.cell_index = p_ue_context->cell_index;
    /* Bug_12469_Fix: End */

    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name,RRM_DETAILED, "CSFB Timer duration:[%d]",
        	get_csfb_blind_timer_duration(p_ue_context->cell_index)); 

    /*SPR 11274 start*/
    /*CID 65785:start*/
    if (RRM_PNULL != p_cell_ctxt)
    {
        if(!(p_cell_ctxt->ho_parameters.csfb_blind_timer_duration))
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name,RRM_ERROR,
                    " Not starting the timer, ZERO value configured for CSFB Blind Timer for "
                    "[UE:%d]",p_ue_context->ue_index);
        }
        else
        {
            /* CSR-58333-fix start*/
            p_ue_context->ho_params.csfb_blind_timer =\
                                                      rrm_ue_start_timer((p_cell_ctxt->ho_parameters.csfb_blind_timer_duration) * RRM_SEC_TO_MILLI_SEC_CONV , 
                                                              &timer_buff, 
                                                              sizeof(rrm_ue_timer_buf_t),
                                                              RRM_FALSE);
            /* CSR-58333-fix end*/
        }
        /*SPR 11274 end*/
    }                                               
    /*CID 65785:end*/
    RRM_UT_TRACE_EXIT();
    return  p_ue_context->ho_params.csfb_blind_timer;
}
/* Freq-Priority Fix changes start */
/****************************************************************************
 *** Function Name  : rrm_ue_stop_ho_timers
 *** Inputs         : UE Context
 *** Outputs        : None
 *** Returns        : None
 *** Description    : This function stops all the timers started as part of HO.
 ******************************************************************************/
rrm_void_t rrm_ue_stop_ho_timers(rrm_ue_context_t	*p_ue_context)
{

    RRM_UT_TRACE_ENTER();
    /* SPR-17852 START */
    if(p_ue_context->ho_params.blind_timer)
        {
            /* Stop HO Blind Timer */
            RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED, "Stopping Blind Timer"); 
            /* CSR-58333-fix start*/
            rrm_ue_stop_timer(p_ue_context->ho_params.blind_timer);
            /* CSR-58333-fix end*/
            p_ue_context->ho_params.blind_timer = RRM_PNULL;
      /* SPR-17852 END */
    }
    /* LTE_RRM_MALL, CID 11112 */
	if(p_ue_context->ho_params.csfb_blind_timer)
	{
		/* Stop HO Delay Timer */
        RRM_TRACE (g_uem_log_on_off, p_g_rrm_uem_facility_name,
        		RRM_DETAILED, "CSFB Blind Timer"); 
        /* CSR-58333-fix start*/
        rrm_ue_stop_timer(p_ue_context->ho_params.csfb_blind_timer);
        /* CSR-58333-fix end*/
		p_ue_context->ho_params.csfb_blind_timer = RRM_PNULL;
	}

	RRM_UT_TRACE_EXIT();
}
/* Freq-Priority Fix changes end */
/* Inter RAT changes end */
/****************************************************************************
 *** Function Name  : rrm_ue_get_geran_pci
 *** Inputs         : UE Context
 ***                : NCC
 ***                : BSCC
 *** Outputs        : None
 *** Returns        : Physical cell id of a geran cell
 *** Description    : This function returns the configured PCI of geran cell
                      based on the NCC and BSCC.
 ******************************************************************************/
    /*SPR 17777 +-*/
U16 rrm_ue_get_geran_pci(rrm_ue_context_t *p_ue_ctx,U8 ncc, U8 bscc)
{
    ncl_params_t          *p_ncl_params   = RRM_PNULL;
    U8                    geran_ncl_count = RRM_ZERO;
    geran_freq_ncl_t      *p_geran_cell   = RRM_PNULL;
    U8                    index           = RRM_ZERO;
    U8                    bsic            = RRM_ZERO;
    U8                    local_ncc            = ncc;
    U8                    local_bscc            = bscc;

    RRM_UT_TRACE_ENTER();
    bsic = (local_ncc << RRM_THREE) | local_bscc;
    p_ncl_params = rrm_cellm_get_ncl_info(p_ue_ctx->cell_index);
    geran_ncl_count = p_ncl_params->inter_rat_ncl.num_valid_geran_cell;

    if(geran_ncl_count == RRM_ZERO)
    {
        RRM_TRACE (g_uem_log_on_off, p_g_rrm_uem_facility_name,
                RRM_ERROR, "GERAN neighbor cells are not configured"); 
        return INVALID_PHYSICAL_CELL_ID;
    }
    p_geran_cell = p_ncl_params->inter_rat_ncl.geran_freq_cells;
    
    for(index = RRM_ZERO; index < geran_ncl_count; index++)
    {
        /* SPR 15674 Start */
        if((bsic == p_geran_cell[index].pci) && 
                (p_ue_ctx->carrier_earfcn == p_geran_cell[index].bcch_arfcn))
        {
            return p_geran_cell[index].pci;
        }
        /* SPR 15674 End */
    }
    RRM_UT_TRACE_EXIT();
    return INVALID_PHYSICAL_CELL_ID;
}

/*SPR-604 Fix Starts*/
/****************************************************************************
 * Function Name  : uerrm_add_ue_history_e_utran_cell_information
 * Inputs         : UE context information
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : Updates the UE context with the history information
 ****************************************************************************/
void uerrm_add_ue_history_e_utran_cell_information
(
 rrm_ue_context_t *p_ue_context
 )
{
    U8         count        = 0;
    signed int elapsed_time = 0;
    U8         loop         = 0;
    U8         num_mnc_digit= 0;

    epc_params_t                           *p_epc_params = RRM_PNULL;
    common_eutran_params_t       *p_common_eutran_params = RRM_PNULL;
    rrm_cell_context_t                      *p_cell_ctxt = RRM_PNULL;

    last_visited_e_utran_cell_information_t  eutra_cell;

    RRM_UT_TRACE_ENTER();
    if (NULL ==  p_ue_context)
    {
        RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,
                "UE Context is NULL");
        return;
    }
    rrm_cell_index_t  cell_index = p_ue_context->cell_index;
    RRM_MEMSET(&eutra_cell, 0, sizeof(last_visited_e_utran_cell_information_t));
    p_epc_params = rrm_cellm_get_erb_service_profile(cell_index);
    p_common_eutran_params = rrm_cellm_get_common_eutran_params(cell_index);
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context->cell_index);

    RRM_MEMCPY(&eutra_cell.global_cell_id.cell_identity,
            p_cell_ctxt->global_cell_id.cell_identity,
            sizeof(eutra_cell.global_cell_id.cell_identity));

    eutra_cell.global_cell_id.plmn_identity.presence_bitmask =
        PLMN_IDENTITY_MCC_PRESENCE_FLAG;
/* SPR 10730 Fix Start */
    RRM_MEMCPY(&eutra_cell.global_cell_id.plmn_identity.mcc,
            &p_epc_params->general_epc_params.plmn_list[0].plmn_id.mcc,
            sizeof(eutra_cell.global_cell_id.plmn_identity.mcc));

    num_mnc_digit = p_epc_params->general_epc_params.plmn_list[0].plmn_id.num_mnc_digit;

    if (num_mnc_digit > MAX_MNC_OCTET_SIZE) {
        RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_ERROR,
                "[UE:%d] Invalid MNC Digit count (%d),"
                "Should be less than %d, Dropping the remaining digits",
                p_ue_context->ue_index,
                num_mnc_digit,MAX_MNC_OCTET_SIZE);
        num_mnc_digit = MAX_MNC_OCTET_SIZE;
    }

    for( loop =0;loop< num_mnc_digit;
            loop++)
    {
        eutra_cell.global_cell_id.plmn_identity.mnc.mnc[loop] =\
                                                               p_epc_params->general_epc_params.plmn_list[0].plmn_id.mnc[loop];
    }
/* SPR 10730 Fix End */

    eutra_cell.global_cell_id.plmn_identity.mnc.count = num_mnc_digit;
    RRM_MEMCPY (&eutra_cell.global_cell_id.cell_identity,
            &p_common_eutran_params->cell_identity,
            MAX_CELL_IDENTITY_OCTETS);

    eutra_cell.cell_type.cell_size = EUTRAN_CELL_SIZE_SMALL;
    elapsed_time = uerrm_get_ue_elapsed_time (p_ue_context);

    /* SPR-21497 START */
    if(elapsed_time < RRM_ZERO )
    {
        elapsed_time = RRM_ZERO;
        eutra_cell.time_ue_stayed_in_cell = RRM_ZERO;
    }
    else
    {
    if (elapsed_time > UERRM_MAX_TIME_FOR_UE_IN_CELL)
    {
        eutra_cell.time_ue_stayed_in_cell=(U16)UERRM_MAX_TIME_FOR_UE_IN_CELL;
    }
    else
    {
        eutra_cell.time_ue_stayed_in_cell = (U16)elapsed_time;
    }
    }
    /* SPR-21497 END */

    RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
            RRM_DETAILED,
            "Calculated Elapsed time for [UE:%d] is %d (actual=%d)\n"
            "Number of Eutra cell count (%d)",
            p_ue_context->ue_index,
            eutra_cell.time_ue_stayed_in_cell,
            elapsed_time,
            p_ue_context->ue_history.last_visited_cell_list.count);

    count = p_ue_context->ue_history.last_visited_cell_list.count;
    if (count == MAX_NO_OF_CELLS_UE_HISTORY)
    {
        /* Shift the ue history so that last entry is available */
        RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                RRM_INFO,
                "[UE:%d] history has reached max (%d), dropping first",
                p_ue_context->ue_index,
                MAX_NO_OF_CELLS_UE_HISTORY);

        /* Coverity ID 73596 Fix Start */
        /* SPR 16763 Fix Start */
        RRM_MEMMOVE (&(p_ue_context->ue_history.last_visited_cell_list.\
                    last_visited_cell_info[RRM_ONE]),
                &(p_ue_context->ue_history.last_visited_cell_list.\
                    last_visited_cell_info[RRM_ZERO]),
                ((MAX_NO_OF_CELLS_UE_HISTORY - RRM_ONE) *
                 sizeof(last_visited_cell_info_t)));
        /* End fix for Coverity_ID : 73596 */
        /* SPR 16763 Fix Stop */
        /* Coverity ID 73596 Fix End */
        /* Adjust the count to point to correct location */
    }
    /* SPR 16763 Fix Start */
    else
    {
        RRM_MEMMOVE (&(p_ue_context->ue_history.last_visited_cell_list.\
                    last_visited_cell_info[RRM_ONE]),
                &(p_ue_context->ue_history.last_visited_cell_list.\
                    last_visited_cell_info[RRM_ZERO]),
                (count * sizeof(last_visited_cell_info_t)));
    }
    p_ue_context->ue_history.last_visited_cell_list.\
        last_visited_cell_info[RRM_ZERO].bitmask = \
        LAST_VISITED_CELL_INFO_E_UTRAN_CELL_INFORMATION_PRESENT;
    RRM_MEMCPY(&(p_ue_context->ue_history.last_visited_cell_list.\
                last_visited_cell_info[RRM_ZERO].last_visited_e_utran_cell_information),\
            &eutra_cell, sizeof(last_visited_e_utran_cell_information_t));
    /* SPR 16763 Fix Stop */
    if (p_ue_context->ue_history.last_visited_cell_list.count != MAX_NO_OF_CELLS_UE_HISTORY)
    {
        p_ue_context->ue_history.last_visited_cell_list.count++;
    }
    /*Bug:270*/

    RRM_UT_TRACE_EXIT();
    return;
}

/*SPR-604 Fix Ends*/
/* SPR 16406 18sep Start */
/* Code removed */
/* SPR 16406 18sep End */

/****************************************************************************
 * Function Name  : rrm_is_all_cgi_requied_cells_list_is_empty
 * Inputs         : UE context information
 * Outputs        : NONE
 * Returns        : RRM_TRUE incase it is empty
 *                  RRM_FALSE incase it is not empty 
 * Description    : This function checks if all the list which contain cells
 * for which we need to configure CGI is empty or not depending upon which we
 * can remove DRX from UE made for IRAT then sorting algorithm automatically
 * gets triggered
 ****************************************************************************/
rrm_bool_et rrm_is_all_cgi_requied_cells_list_is_empty(
	rrm_ue_context_t *p_ue_context)
{
    RRM_UT_TRACE_ENTER();
    if (RRM_ZERO != p_ue_context->ho_params.eutra_unknown_cell_selection_list.count)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "EUTRAN unknown list has %u cells for [UE:%u]", 
                p_ue_context->ho_params.eutra_unknown_cell_selection_list.count,
                p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return RRM_FALSE;
    }
    if (RRM_ZERO != p_ue_context->ho_params.eutra_confused_cell_selection_list.count)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "EUTRAN confused list has %u cells for [UE:%u]", 
                p_ue_context->ho_params.eutra_confused_cell_selection_list.count,
                p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return RRM_FALSE;
    }
    if (RRM_ZERO != p_ue_context->ho_params.utra_unknown_cell_selection_list.count)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "UTRAN unknown list has %u cells for [UE:%u]", 
                p_ue_context->ho_params.utra_unknown_cell_selection_list.count,
                p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return RRM_FALSE;
    }
    if (RRM_ZERO != p_ue_context->ho_params.utra_confused_cell_selection_list.count)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "UTRAN confused list has %u cells for [UE:%u]", 
                p_ue_context->ho_params.utra_confused_cell_selection_list.count,
                p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return RRM_FALSE;
    }
    if (RRM_ZERO != p_ue_context->ho_params.geran_unknown_cell_selection_list.count)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "GERAN unknown list has %u cells for [UE:%u]", 
                p_ue_context->ho_params.geran_unknown_cell_selection_list.count,
                p_ue_context->ue_index);
        RRM_UT_TRACE_EXIT();
        return RRM_FALSE;
    }
    if (RRM_ZERO != p_ue_context->ho_params.geran_confused_cell_selection_list.count)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "GERAN confused list has %u cells for [UE:%u]", 
                p_ue_context->ho_params.geran_confused_cell_selection_list.count,
                p_ue_context->ue_index);
                RRM_UT_TRACE_EXIT();
                return RRM_FALSE;
    }

    RRM_UT_TRACE_EXIT();
    return RRM_TRUE;
}
/*CID 93895 Start*/
/****************************************************************************
 * Function Name  : rrm_handle_cgi_measurement
 * Inputs         : UE context information
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS
 * Description    : This function handles CGI measurement triggered due to
 *                  measurement event
 ****************************************************************************/
rrm_return_et
rrm_handle_cgi_measurement(
        rrm_ue_context_t *p_ue_context,
        rrm_ue_measurment_results_ind_t *p_rrm_parsed_meas_res_ind
        )
{
    cell_selection_priority_list_t          *p_cell_selection_list = RRM_PNULL;
    /* SPR 15674 Start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t                      phy_cell_id_info  = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_FALSE, RRM_FALSE, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 15674 End */
    /*coverity 93560 fix start*/
    rrm_son_meas_results_ind_t              *p_rrm_son_meas_results_ind = RRM_PNULL;
    /*coverity 93560 fix end*/
    /*SPR 17527 START*/
    rrm_bool_et cgi_present_in_unknwn_list_flag = RRM_FALSE;
    /*SPR 17527 END*/
    /*SPR 17943 START*/
    U8          j = RRM_ZERO;
    /*SPR 17943 END*/

    /* SPR 15674 End */
    /* SPR 16406 18sep End */
    U16                                     ura_pci                 = RRM_ZERO;
    rrm_return_et                           ret_val                 = RRM_SUCCESS;
    rrm_ho_params_t*                   p_ho_params               = RRM_PNULL;
    rrm_cell_context_t    *p_cell_ctxt = RRM_PNULL;
    /* SPR 15232 start */
    p_ho_params = (rrm_ho_params_t*)&p_ue_context->ho_params;
    /* SPR 15232 end */

    RRM_UT_TRACE_ENTER();
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /* SPR 16406 18sep Start */
    /* Code Removed */
    /* SPR 16406 18sep End */
    /*SPR 17943 START*/
    /* SPR 17784 Start */
  if ((p_cell_ctxt->ho_parameters.cgi_timer_duration) &&
      (p_ho_params->cgi_timer))
    {
    RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
        RRM_DETAILED, "Stopping CGI Timer [%p] , CGI is reported",
        p_ue_context->ho_params.cgi_timer); 
    rrm_ue_stop_timer(p_ue_context->ho_params.cgi_timer);
    p_ho_params->cgi_timer = RRM_PNULL;
    }
    /* SPR 17784 End */
    /*SPR 17943 END*/
    if (RRM_SON_EUTRAN_NC_CGI_PRESENT & p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].bitmask)
        /* SPR 15232 end */
    {
        /*SPR 17943 START*/
        /*code deleted*/    
        /*SPR 17943 END*/
        /* SPR 16406 18sep Start */
        /* Code Removed */
        /* SPR 16406 18sep End */
        /*SPR 17527 START*/
        switch(rrm_check_cgi_of_eutran_reported_cell(p_ue_context,
                    p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].pci,
                    p_ue_context->carrier_earfcn,
                    &p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].cgi,
                    &phy_cell_id_info,&cgi_present_in_unknwn_list_flag))
            /*SPR 17527 END*/
        {
            case RRM_CGI_REP_CAUSE_NEW_PCI:
                {/*SPR 15792 Fix Start */    
                    if (CELL_ALREADY_REGISTERD_FOR_ANR & p_cell_ctxt->cell_already_register_anr)
                        /*SPR 15792 Fix Stop */
                    {
                        /*coverity 93560 fix start*/
                        p_rrm_son_meas_results_ind = (rrm_son_meas_results_ind_t *)rrm_mem_get(sizeof(rrm_son_meas_results_ind_t));
                        if(RRM_PNULL == p_rrm_son_meas_results_ind)
                        {
                            RRM_TRACE(g_uem_log_on_off,
                                    p_rrm_uem_facility_name,
                                    RRM_ERROR,
                                    "Memory Allocation failed ");
                            RRM_UT_TRACE_EXIT();
                            return RRM_FAILURE;
                        }
                        /* SPR 18275 Fix Start*/
                        RRM_MEMSET(p_rrm_son_meas_results_ind, RRM_ZERO,sizeof(rrm_son_meas_results_ind_t));
                        /* SPR 18275 Fix End*/ 
                        /* SPR 16406 Start */
                        /* SPR 16406 21AUG End */
                        /* EUTRAN Scenario of CGI reported for new PCI */
                        /* CGI & TAC are mandatorily present in the CGI report in meas results ind */
                        /* Copy CGI reported by UE in rrm_son_meas_results_ind */
                        /*coverity 93560 fix start*/
                        /* SPR 18275 Fix Start*/
                        /* code deleted */
                        /* SPR 18275 Fix End */
                        /* Copy CGI reported by UE in rrm_son_meas_results_ind */
                        /*coverity 93560 fix start*/
                        /*SPR 17527 START*/
                        /*code deleted*/
                        /*SPR 17527 END*/
                        /*SPR 17527 START*/
                        /* Coverity_Fix 96615 ++ */
                        p_rrm_son_meas_results_ind->utran_meas_list_size = RRM_ZERO;        
                        p_rrm_son_meas_results_ind->geran_meas_list_size = RRM_ZERO;        
                        p_rrm_son_meas_results_ind->cdma_meas_list_size = RRM_ZERO;        
                        p_rrm_son_meas_results_ind->ue_index = p_ue_context->ue_index;
                        p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_NONE; 
                        p_rrm_son_meas_results_ind->eutran_meas_list_size = RRM_ONE;        
                        p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].earfcn = p_ue_context->carrier_earfcn; 
                        p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].neighbor_cell_list_size = RRM_ONE;
                        p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].neighbor_cell_list[RRM_ZERO].bitmask = RRM_ZERO;        
                        p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].neighbor_cell_list[RRM_ZERO].bitmask |= RRM_SON_EUTRAN_NC_CGI_PRESENT;        
                        p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].neighbor_cell_list[RRM_ZERO].bitmask |= RRM_SON_EUTRAN_NC_TAC_PRESENT;             
                        /* Coverity_Fix 96615 -- */
                        /*SPR 17527 END*/

                        RRM_MEMCPY(&(p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].
                                    neighbor_cell_list[RRM_ZERO].cgi), 
                                &(p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].cgi),
                                sizeof(rrm_oam_eutran_global_cell_id_t));
                        /* Copy the TAC reported by UE in rrm_son_meas_results_ind */
                        RRM_MEMCPY(&(p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].
                                    neighbor_cell_list[RRM_ZERO].tac), 
                                &(p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].tac),
                                MAX_TAC_SIZE);

                        /* SPR 16406 21AUG Start */
                        p_rrm_son_meas_results_ind->eutran_meas_list[RRM_ZERO].neighbor_cell_list[RRM_ZERO].pci = 
                            p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].pci;
                        /* SPR 16406 21AUG End */
                        rrm_ue_populate_and_send_rrmif_meas_results_ind(p_ue_context, p_rrm_son_meas_results_ind);
                        /* SPR 16406 End */

                        /*SPR 15792 Fix Start */ 
                        /*SPR 17527 START*/
                        if ((cgi_present_in_unknwn_list_flag == RRM_TRUE)&&(RRM_SUCCESS !=
                                    rrm_check_if_node_already_exits_for_ho(p_ue_context,
                                        p_ue_context->carrier_earfcn,
                                        p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].pci)))
                            /*SPR 17527 END*/
                        {
                            /* SPR 16406 18sep Start */
                            rrm_insert_in_ho_cgi_pending_list(p_ue_context,
                                    p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].pci,
                                    &p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].cgi,
                                    p_ue_context->carrier_earfcn);
                            /* SPR 16406 18sep End */

                        }
                        /*coverity 94869 fix start*/ 
                        RRM_MEM_FREE(p_rrm_son_meas_results_ind);
                        /*coverity 94869 fix end*/
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "The ANR is DISABLED and cgi trigger cause  is RRM_CGI_REP_CAUSE_NEW_PCI ");
                    }
                    /*SPR 15792 Fix Stop */ 
                }
                break;

            case RRM_CGI_REP_CAUSE_PCI_CONFUSION:
                {
                    /* SPR_15712_Fix: Start */
                    /* Check the CGI if matching with the existing Cells present in NCL */
                    if (RRM_FALSE == phy_cell_id_info.is_cell_blk_list)
                    {
                        /* SPR_15282_Fix_End */
                        /* SPR 16053 start */
                        /* Code deleted */
                        /* SPR 16053 end */
                        /* SPR 16406 18sep Start */
                        /* Code Removed */
                        /* SPR 16406 18sep End */
                        /* SPR 16836 Fix Start */
                        p_ue_context->target_cell_type = EUTRA_CELL;
                        /* SPR 16836 Fix Stop */
                        /* SPR 16406 18sep Start */
                        RRM_MEMCPY(&(phy_cell_id_info.cgi),
                                &(p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].cgi),
                                sizeof(rrm_oam_eutran_global_cell_id_t));

                        phy_cell_id_info.bitmask = RRM_PHY_CELL_ID_INFO_CGI_PRESENT; 
                        /* SPR 16406 18sep End */
                        /* SPR 15674 Start */
                        ret_val = rrm_build_and_send_ho_required(phy_cell_id_info, p_ue_context, rrm_generate_txn_id());
                        /* SPR 15674 End */

                        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_uem_facility_name,
                                RRM_BRIEF, "[UE:%u] Build & send HO Required (%s)",
                                p_ue_context->ue_index, ret_val?"SUCCESS":"FAILED");
                        /*SPR 17943 START*/
                        if(ret_val == RRM_SUCCESS)
                        {
                            RRM_UT_TRACE_EXIT();
                            return RRM_SUCCESS;
                        }
                        else
                        {
                            RRM_UT_TRACE_EXIT();
                            return RRM_FAILURE;
                        }
                        /*SPR 17943 END*/

                    }
                    else
                    {
                        RRM_TRACE(g_cellm_log_on_off,
                                p_g_rrm_uem_facility_name, 
                                RRM_WARNING, "Confused PCI [%u] is blacklisted hence dropping it", phy_cell_id_info.phy_cell_id); 
                        /*SPR 17527 START*/
                        rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                &p_ue_context->ho_params.eutra_confused_cell_selection_list,
                                phy_cell_id_info.phy_cell_id,
                                phy_cell_id_info.carr_earfcn);

                        if(RRM_FAILURE == rrm_uem_search_and_select_pci_for_ho(p_ue_context, rrm_generate_txn_id()))
                        {
                            if (RRM_UE_CDRX_DISABLED == p_ue_context->cdrx_status)
                            {
                                rrm_remove_previous_pending_measurement(p_ue_context);
                            }
                            else
                            {
                                rrm_uem_send_cdrx_removal(p_ue_context);
                            }
                        }
                        /*SPR 17527 END*/
                    }
                    /* SPR_15712_Fix: End */
                }
                break;

            case RRM_CGI_REP_CAUSE_KNOWN_PCI:
                {
                    /* SPR 16406 3Aug End */
                    if (RRM_FALSE == phy_cell_id_info.is_cell_blk_list)
                    {
                        p_ue_context->target_cell_type = EUTRA_CELL;
                        /*SPR 17943 START*/
                    for (j = RRM_ZERO; j < p_ue_context->ho_params.eutra_unknown_cell_selection_list.count; j++)
                    {
                        if ((p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info[j].phy_cell_id == 
                                    p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].pci)&&(p_ue_context->carrier_earfcn == 
                                        p_ue_context->ho_params.eutra_unknown_cell_selection_list.reported_neighbor_cell_info[j].target_earfcn))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "Cell found in the cell selection list");
                            ret_val = rrm_build_and_send_ho_required(phy_cell_id_info, p_ue_context, rrm_generate_txn_id());
                            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_uem_facility_name,
                                    RRM_BRIEF, "[UE:%u] Build & send HO Required (%s)",
                                    p_ue_context->ue_index, ret_val?"SUCCESS":"FAILED");
                            if(ret_val == RRM_SUCCESS)
                            {
                                RRM_UT_TRACE_EXIT();  
                                return RRM_SUCCESS;
                            }
                            else
                            {
                                rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                        &p_ue_context->ho_params.eutra_known_cell_selection_list,
                                        phy_cell_id_info.phy_cell_id,
                                        phy_cell_id_info.carr_earfcn);

                                if(RRM_FAILURE == rrm_uem_search_and_select_pci_for_ho(p_ue_context, rrm_generate_txn_id()))
                                {
                                    if (RRM_UE_CDRX_DISABLED == p_ue_context->cdrx_status)
                                    {
                                        rrm_remove_previous_pending_measurement(p_ue_context);
                                    }
                                    else
                                    {
                                        rrm_uem_send_cdrx_removal(p_ue_context);
                                    }
                                }
                                RRM_UT_TRACE_EXIT();  
                                return RRM_FAILURE;
                            }    
                        }
                    }
                    /*SPR 17943 END*/

                    }
                    else
                    {
                        RRM_TRACE(g_cellm_log_on_off,
                                p_g_rrm_uem_facility_name, 
                                RRM_WARNING, "Known PCI [%u] is blacklisted hence dropping it", phy_cell_id_info.phy_cell_id); 
                        /*SPR 17527 START*/
                        rrm_remove_pci_from_cell_selection_list(p_ue_context,
                                &p_ue_context->ho_params.eutra_known_cell_selection_list,
                                phy_cell_id_info.phy_cell_id,
                                phy_cell_id_info.carr_earfcn);

                        if(RRM_FAILURE == rrm_uem_search_and_select_pci_for_ho(p_ue_context, rrm_generate_txn_id()))
                        {
                            if (RRM_UE_CDRX_DISABLED == p_ue_context->cdrx_status)
                            {
                                rrm_remove_previous_pending_measurement(p_ue_context);
                            }
                            else
                            {
                                rrm_uem_send_cdrx_removal(p_ue_context);
                            }
                        }
                        /*SPR 17527 END*/
                    }
                }
                break;

            default:
                break;
        }
    }
    /* UTRAN Scenario of CGI reported for new PCI*/
    else if (RRM_SON_UTRAN_NC_CGI_PRESENT & p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].bitmask)
    {
        if (p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci.bitmask & RRM_PCI_FDD_PRESENT)
        {
            ura_pci = p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci.pci_fdd;
        }
        else if (p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci.bitmask & RRM_PCI_TDD_PRESENT)
        {
            ura_pci = p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci.pci_tdd;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                    "PCI not found in the CGI measurement report for [UE:%u]. Dropping the CGI event",
                    p_ue_context->ue_index);
            RRM_UT_TRACE_EXIT();
            return RRM_FAILURE; 
        }

        /*SPR 17943 START*/
        /*code deleted*/    
        /*SPR 17943 END*/

        /*SPR 17527 START*/
        switch (rrm_check_cgi_of_utran_reported_cell(p_ue_context,
                    ura_pci,
                    p_ue_context->carrier_earfcn,
                    &phy_cell_id_info,&cgi_present_in_unknwn_list_flag))
            /*SPR 17527 END*/
        {
            case RRM_CGI_REP_CAUSE_NEW_PCI:
                {
                    /*coverity 93560 fix start*/
                    p_rrm_son_meas_results_ind = (rrm_son_meas_results_ind_t *)rrm_mem_get(sizeof(rrm_son_meas_results_ind_t));
                    if(RRM_PNULL == p_rrm_son_meas_results_ind)
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_rrm_uem_facility_name,
                                RRM_ERROR,
                                "Memory Allocation failed ");
                        RRM_UT_TRACE_EXIT();
                        return RRM_FAILURE;
                    }
                    /* SPR 16406 Start */
                    /* UTRAN Scenario of CGI reported for new PCI */
                    /* CGI & TAC are mandatorily present in the CGI report in meas results ind */
                    /* Copy CGI reported by UE in rrm_son_meas_results_ind */
                    /*coverity 93560 fix start*/
                    /*SPR 17527 START*/
                    /*code deleted*/
                    /*SPR 17527 END*/
                    /*SPR 17527 START*/
                    /* Coverity_Fix 96615 ++ */
                    p_rrm_son_meas_results_ind->eutran_meas_list_size = RRM_ZERO;        
                    p_rrm_son_meas_results_ind->utran_meas_list_size = RRM_ONE;        
                    p_rrm_son_meas_results_ind->geran_meas_list_size = RRM_ZERO;        
                    p_rrm_son_meas_results_ind->cdma_meas_list_size = RRM_ZERO;        
                    p_rrm_son_meas_results_ind->ue_index = p_ue_context->ue_index;
                    p_rrm_son_meas_results_ind->meas_report_event = MEAS_REP_EVENT_NONE; 
                    p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].neighbor_cell_list_size = RRM_ONE;

                    p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].uarfcn = p_ue_context->carrier_earfcn; 
                    p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].neighbor_cell_list[RRM_ZERO].pci = 
                        p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci;

                    p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].neighbor_cell_list[RRM_ZERO].bitmask = RRM_ZERO;
                    p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].neighbor_cell_list[RRM_ZERO].bitmask |= 
                        RRM_SON_EUTRAN_NC_CGI_PRESENT;        
                    /* Coverity_Fix 96615 -- */
                    /*SPR 17527 END*/

                    RRM_MEMCPY(&(p_rrm_son_meas_results_ind->utran_meas[RRM_ZERO].
                                neighbor_cell_list[RRM_ZERO].cgi),
                            &(p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].cgi),
                            sizeof(rrm_son_utran_geran_global_cell_id_t));
                    /* Not copying other params as they are optional as per 36.331.
                     * Need to copy & send to SON if required */
                    rrm_ue_populate_and_send_rrmif_meas_results_ind(p_ue_context, p_rrm_son_meas_results_ind);
                    /*coverity 93560 fix end*/
                    /*SPR 17527 START*/
                    if ((cgi_present_in_unknwn_list_flag == RRM_TRUE)&&(RRM_SUCCESS !=
                                rrm_check_if_node_already_exits_for_ho(p_ue_context,
                                    p_ue_context->carrier_earfcn,
                                    p_rrm_parsed_meas_res_ind->eutran_neighbor_list[RRM_ZERO].pci)))
                        /*SPR 17527 END*/
                    {
                        /* SPR 16406 18sep Start */
                        rrm_insert_in_ho_cgi_pending_list(p_ue_context,
                                p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci.pci_fdd,
                                RRM_PNULL,
                                p_ue_context->carrier_earfcn);
                        /* SPR 16406 18sep End */

                    }

                    /*coverity 93560 fix start*/ 
                    RRM_MEM_FREE(p_rrm_son_meas_results_ind);
                    /*coverity 93560 fix end*/
                }
                break;

            case RRM_CGI_REP_CAUSE_KNOWN_PCI:
                {
                    /* SPR 16406 18sep Start */
                    /* Code Removed */
                    /* SPR 16406 18sep End  */
                    /*SPR_17893_START*/
                    if(!(RRM_UE_UTRA_RADIO_CAPABILITY_PRESENT & p_ue_context->ue_capability_params.bitmask))
                    {
                        if(RRM_TRUE == rrm_check_if_utran_ho_is_possible(p_ue_context))
                        {
                            if((RRM_UE_STATE_ACTIVE == p_ue_context->ue_state)) 
                            {
                                if(RRM_TRUE == rrm_populate_and_send_utran_cap_enq_req(p_ue_context))
                                {
                                    rrm_ue_set_state(p_ue_context,RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES);
                                    rrm_set_utran_capability_enq_info(p_ue_context,RRM_TRUE,RRM_FALSE);
                                    p_ue_context->is_ho_reqd_on_recv_utan_cap  = RRM_TRUE;
                                }
                            }
                        }
                    }
                    else
                    {    
                    /*SPR_17893_END*/
                    p_ue_context->target_cell_type = UTRA_CELL;
                    /*SPR 17943 START*/
                    for (j = RRM_ZERO; j < p_ue_context->ho_params.utra_unknown_cell_selection_list.count; j++)
                    {
                        if ((p_ue_context->ho_params.utra_unknown_cell_selection_list.reported_neighbor_cell_info[j].phy_cell_id == 
                                    p_rrm_parsed_meas_res_ind->utran_neighbor_list[RRM_ZERO].pci.pci_fdd)&&(p_ue_context->carrier_earfcn == 
                                        p_ue_context->ho_params.utra_unknown_cell_selection_list.reported_neighbor_cell_info[j].target_earfcn))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "Cell found in the cell selection list");
                            ret_val = rrm_build_and_send_ho_required (phy_cell_id_info, p_ue_context, rrm_generate_txn_id());    
                            RRM_TRACE(g_cellm_log_on_off, p_g_rrm_uem_facility_name,
                                    RRM_BRIEF, "[UE:%u] Build & send HO Required (%s)",
                                    p_ue_context->ue_index, ret_val?"SUCCESS":"FAILED");
                            if(ret_val == RRM_SUCCESS)
                            {
                                RRM_UT_TRACE_EXIT();  
                                return RRM_SUCCESS;
                            }
                            else
                            {
                                RRM_UT_TRACE_EXIT();  
                                return RRM_FAILURE;
                            } 
                        }
                    }
                    /*SPR 17943 END*/
              /*SPR_17893_START*/
               }
              /*SPR_17893_END*/
                }
                break;

            default:
                break;
        }
        /* SPR_15282_Fix_Start */
        /* SPR 16406 End */
    }
    else
    {
        /* SPR 15509 start */
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Empty CGI report found");

        /* SPR 16406 Start */
        /*Coverity_Fix 88202 ++ */
        p_cell_selection_list = rrm_uem_get_active_cell_selection_list(p_ue_context);
        if(RRM_NULL != p_cell_selection_list)       
        {
            rrm_remove_pci_from_cell_selection_list(p_ue_context,
                    p_cell_selection_list,
                    p_ue_context->pci_for_which_cgi_required,
                    p_ue_context->carrier_earfcn);
        }
        /*Coverity_Fix 88202 -- */
        /* SPR 16406 End */

        /*SPR 17819 Fix Start*/
        if(RRM_UE_STATE_HO_ONGOING != p_ue_context->ue_state)
        {
            /* + coverity 98789 */
            if( RRM_SUCCESS == rrm_uem_search_and_select_pci_for_ho(
                        p_ue_context,rrm_generate_txn_id() ) )
            {
                RRM_TRACE(RRM_TRUE, p_rrm_event_facility_name, RRM_DETAILED,
                        "Report CGI is triggered return_val = %d",ret_val);
            }
            /* - coverity 98789 */
        }
        /*SPR 17819 Fix End*/

        /* SPR 15509 end */
        /*SPR 17527 START*/
        /*code deleted*/
        /*SPR 17527 END*/
    }


    /*SPR 17819 Fix Start*/
    /*SPR_17893_START*/
    if((RRM_UE_STATE_HO_ONGOING != p_ue_context->ue_state) && (RRM_UE_STATE_WAIT_FOR_CAP_ENQ_RES != p_ue_context->ue_state))
    {
    /*SPR_17893_END*/

        /*SPR 17527 START*/
        if (RRM_TRUE == rrm_is_all_cgi_requied_cells_list_is_empty(p_ue_context))
        { 
            if ((RRM_UE_CDRX_APPLIED == p_ue_context->cdrx_status))
            {
                p_ue_context->cdrx_status = RRM_UE_CDRX_TO_BE_REMOVED;

                /* SPR 12480 Start */
                /* Apply DRX again as CGIs have been reported */
                if (RRM_FAILURE == rrm_build_and_send_ue_reconfig_req(p_ue_context))
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                            "Failed to send UE reconfig request to remove CDRX, [UE:%d]",
                            p_ue_context->ue_index);
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_INFO,
                            "UE reconfig request sent to [UE:%d] to remove CDRX ",
                            p_ue_context->ue_index);
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                        "Not removing DRX for [UE:%u] as other cells needs it",
                        p_ue_context->ue_index);
                if( RRM_TRUE == rrm_uem_check_is_cgi_config_pending(p_ue_context))
                {
                    p_ue_context->meas_rep_event = MEAS_REP_EVENT_FOR_CGI;
                    ret_val = rrm_build_and_send_meas_config_req(p_ue_context,MEAS_TYPE_HO);
                    /*CID 97160 Start*/
                    if(ret_val == RRM_FAILURE)
                        RRM_TRACE(g_cellm_log_on_off, p_g_rrm_uem_facility_name,
                                RRM_DETAILEDALL,"rrm_build_and_send_meas_config_req return failure");
                    /*CID 97160 End*/
                }
            }

        }
        /*SPR 17527 END*/
    }
    /*SPR 17819 Fix End*/

    RRM_UT_TRACE_EXIT();
    return RRM_SUCCESS;
}

/*CID 93895 End*/
/****************************************************************************
 * Function Name  : rrm_check_if_node_already_exits_for_ho
 * Inputs         : UE context, Carrier EARFCN, PCI for which CGI is received
 * Outputs        : NONE
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks that if the PCI is already exist in
 *                  the handover pending list or not. if present then it will return success
 *                  else it will return failure
 ****************************************************************************/
rrm_return_et
rrm_check_if_node_already_exits_for_ho(
        rrm_ue_context_t *p_ue_context,
        U16 carrier_earfcn,
        U16 pci)
{
    ho_pending_cgi_node_t *p_new_cgi_pending_node   = RRM_PNULL;
    rrm_cell_context_t *p_cell_context              = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    /*Coverity 94777 Fix Start*/
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,RRM_WARNING,
                "Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*Coverity 94777 Fix End*/

    p_new_cgi_pending_node = (ho_pending_cgi_node_t*)ylFirst(&p_cell_context->rrm_cgi_pending_list.
        rrm_ho_pending_cgi_list[p_cell_context->cell_index].ho_pending_cgi_list);
    while (RRM_PNULL != p_new_cgi_pending_node)
    {
        if ((p_new_cgi_pending_node->ho_pending_cgi_node_data.ue_idx == p_ue_context->ue_index) &&
                (p_new_cgi_pending_node->ho_pending_cgi_node_data.pci == pci) &&
                (p_new_cgi_pending_node->ho_pending_cgi_node_data.earfcn == carrier_earfcn))
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                    "Cell already present for [UE:%u] for PCI[%u], EARFCN[%u] in handover pending list",
                    p_ue_context->ue_index, pci, carrier_earfcn);
            RRM_UT_TRACE_EXIT();
	    /* SPR 16406 24Sep Start */
            return RRM_SUCCESS;
	    /* SPR 16406 24Sep End */
        }
        p_new_cgi_pending_node = (ho_pending_cgi_node_t*)ylNext(&p_new_cgi_pending_node->node);
    } 
    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
            "Cell not present for [UE:%u] for PCI[%u], EARFCN[%u] in handover pending list",
            p_ue_context->ue_index, pci, carrier_earfcn);
    RRM_UT_TRACE_EXIT();
    return RRM_FAILURE;
}

/****************************************************************************
 * Function Name  : rrm_check_if_node_already_exits_for_ho
 * Inputs         : UE context, Carrier EARFCN, PCI for which CGI is received
 *                  CGI
 * Outputs        : Physical cell information
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function checks that if the PCI is already exist in
 *                  the handover pending list or not. if present then it will return success
 *                  else it will return failure
 ****************************************************************************/
/*SPR 17527 START*/
rrm_cgi_trigger_cause_et
rrm_check_cgi_of_utran_reported_cell(
        rrm_ue_context_t *p_ue_context,
        U16 ura_pci,
        U16 carrier_earfcn,
		phy_cell_id_info_t  *p_ho_cell_info,
		rrm_bool_et *p_cgi_present_in_unknwn_list_flag)
/*SPR 17527 END*/
{
    rrm_cgi_trigger_cause_et ret_val        = RRM_CGI_REP_CAUSE_NEW_PCI;
    ncl_params_t          *p_ncl_params   = RRM_PNULL;
    U16                     cell_count      = RRM_ZERO;
    /*SPR 17527 START*/
    U16                     utran_count      = RRM_ZERO;
    /*SPR 17527 END*/

    RRM_UT_TRACE_ENTER();

    /*SPR 17527 START*/
    *p_cgi_present_in_unknwn_list_flag = RRM_FALSE;
    /*SPR 17527 END*/
    p_ho_cell_info->phy_cell_id = INVALID_PHYSICAL_CELL_ID;
    p_ho_cell_info->carr_earfcn = RRM_ZERO;
    p_ho_cell_info->is_cell_blk_list = RRM_FALSE;
    p_ho_cell_info->is_cell_confused = RRM_FALSE;
    p_ncl_params = rrm_cellm_get_ncl_info(p_ue_context->cell_index);

    if(RRM_PNULL != p_ncl_params)
    {
        for(cell_count = RRM_ZERO; cell_count < p_ncl_params->inter_rat_ncl.num_valid_utran_freq_cell; cell_count++)
        {
            if ((p_ncl_params->inter_rat_ncl.utran_freq_cells[cell_count].pcpich_scrambling_code == ura_pci) &&
                    (p_ncl_params->inter_rat_ncl.utran_freq_cells[cell_count].uarfcndl))
            {
                p_ho_cell_info->phy_cell_id = INVALID_PHYSICAL_CELL_ID;
                p_ho_cell_info->carr_earfcn = RRM_ZERO;
                ret_val = RRM_CGI_REP_CAUSE_KNOWN_PCI;
                if (RRM_TRUE == p_ncl_params->inter_rat_ncl.utran_freq_cells[cell_count].blacklisted)
                {
                    p_ho_cell_info->is_cell_blk_list = RRM_TRUE;
                }
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                "Cell not found in Inter-RAT NCL for [UE:%u] for PCI[%u], EARFCN[%u] ",
                p_ue_context->ue_index, ura_pci, carrier_earfcn);
    }
    /*SPR 17527 START*/
    if (ret_val == RRM_CGI_REP_CAUSE_NEW_PCI)
    {
        for (utran_count = RRM_ZERO; utran_count < p_ue_context->ho_params.utra_unknown_cell_selection_list.count; utran_count++)
        {
            if ((p_ue_context->ho_params.utra_unknown_cell_selection_list.reported_neighbor_cell_info[utran_count].phy_cell_id == ura_pci)&&
		    /*SPR 18133 Fix Start*/
                    (carrier_earfcn == p_ue_context->ho_params.utra_unknown_cell_selection_list.reported_neighbor_cell_info[utran_count].target_earfcn))
		    /*SPR 18133 Fix Stop*/
            {
                *p_cgi_present_in_unknwn_list_flag = RRM_TRUE;
            }
        }
    }
    /*SPR 17527 END*/

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_remove_previous_cell_for_ho
 * Inputs         : UE context, Carrier EARFCN, PCI for which CGI is received
 *                  CGI
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function removes the cell from the HO pending list as
 *                  cell is no longer a best cell for doing handover
 ****************************************************************************/
rrm_return_et
rrm_remove_previous_cell_for_ho(rrm_ue_context_t *p_ue_context,
                        U16 pci,
                        U16 carrier_earfcn)
{
    ho_pending_cgi_node_t *p_new_cgi_pending_node   = RRM_PNULL;
    rrm_cell_context_t *p_cell_context              = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index); 
    /*Coverity 94815 Fix Start*/
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,RRM_WARNING,
                "Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }
    /*Coverity 94815 Fix End*/

    p_new_cgi_pending_node = (ho_pending_cgi_node_t*)ylFirst(&p_cell_context->rrm_cgi_pending_list.
        rrm_ho_pending_cgi_list[p_cell_context->cell_index].ho_pending_cgi_list);
    while (RRM_PNULL != p_new_cgi_pending_node)
    {
        if ((p_new_cgi_pending_node->ho_pending_cgi_node_data.ue_idx == p_ue_context->ue_index) &&
                (p_new_cgi_pending_node->ho_pending_cgi_node_data.pci == pci) &&
                (p_new_cgi_pending_node->ho_pending_cgi_node_data.earfcn == carrier_earfcn))
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
                    "Cell already present for [UE:%u] for PCI[%u], EARFCN[%u] in handover pending list, removing it",
                    p_ue_context->ue_index, pci, carrier_earfcn);
            ylDelete(&p_cell_context->rrm_cgi_pending_list.rrm_ho_pending_cgi_list[p_cell_context->cell_index].ho_pending_cgi_list,
                    &p_new_cgi_pending_node->node);
            RRM_UT_TRACE_EXIT();
            return RRM_SUCCESS;
        }
        p_new_cgi_pending_node = (ho_pending_cgi_node_t*)ylNext(&p_new_cgi_pending_node->node);
    } 
    RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_BRIEF,
            "Cannot find cell for removal for [UE:%u] for PCI[%u], EARFCN[%u] in handover pending list",
            p_ue_context->ue_index, pci, carrier_earfcn);
    RRM_UT_TRACE_EXIT();
    return RRM_FAILURE;
}
/* SPR 16406 8SEP end */

/*SPR_17893_START*/
rrm_bool_et rrm_check_if_utran_ho_is_possible(rrm_ue_context_t*   p_ue_context)
{
	rrm_bool_et    retval	= RRM_TRUE;
	U16	           daho_pci     = INVALID_PHYSICAL_CELL_ID;
	U16		   daho_freq     = RRM_ZERO;
	rrm_ho_restriction_list_t	  *p_ho_restriction_list         = RRM_PNULL;
	rrm_daho_cell_config_info_t   *p_daho_cfg                    = RRM_PNULL;
	U32                           feature_group_indicators       = (U32)-RRM_ONE;
	rrm_bool_et 		fgi_supported                    = RRM_FALSE;
	rrm_bool_et                 irat_utra_forbidden              = RRM_FALSE;
    /* SPR 22731 Fix Start */
    U8 utran_fdd_count = RRM_ZERO;
    U8 utran_tdd_count = RRM_ZERO;
    rrm_cell_context_t  *p_cell_ctx = RRM_PNULL;
    p_cell_ctx = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    /* SPR 22731 Fix End */

	RRM_UT_TRACE_ENTER();

	p_ho_restriction_list = &(p_ue_context->ho_restriction_list);
	p_daho_cfg            = rrm_cellm_get_daho_cell_config_info_params(p_ue_context->cell_index);

	if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
			p_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
			eutra_radio_capability_info_def.bitmask)
	{
		feature_group_indicators = p_ue_context->ue_capability_params.
			rrm_eutra_radio_capability_info.
			eutra_radio_capability_info_def.feature_group_indicators;
		if(feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_EIGHT)))
		{
			fgi_supported = RRM_TRUE;
		}
	}

	if(RRM_FALSE == fgi_supported)
	{
		RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,
				RRM_DETAILED,"FGI 8 is not supported");
		RRM_UT_TRACE_EXIT();
		return RRM_FALSE; 
	}

	if((p_daho_cfg != RRM_PNULL) &&
			(p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT))
	{
		daho_pci = p_daho_cfg->utran_daho_cell_config_info.pci;
		daho_freq = p_daho_cfg->utran_daho_cell_config_info.uarfcndl;
	}

	if ( (RRM_FALSE == rrm_is_ue_special_previledge(p_ue_context)) &&
			(p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) )
	{
		if((p_ho_restriction_list->forbidden_inter_rat == \
					FORBIDDEN_INTER_RAT_UTRAN_TYPE) || \
				(p_ho_restriction_list->forbidden_inter_rat == \
				 FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN) || \
				(p_ho_restriction_list->forbidden_inter_rat == \
				 FORBIDDEN_INTER_RAT_ALL))
		{
			irat_utra_forbidden = RRM_TRUE;
		}
	}
	if(p_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD)
	{
		if(p_ue_context->ue_context_mod_req.csf_indicator == RRM_CSFB_HIGH_PRIORITY)
		{
			RRM_TRACE (g_uem_log_on_off, 
					p_rrm_uem_facility_name,
					RRM_DETAILED, 
					"CSFB is HIGH PRIORITY");
			irat_utra_forbidden = RRM_FALSE;
		}
	}
	if(irat_utra_forbidden == RRM_TRUE)
	{
		retval = RRM_FALSE;
		RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
				RRM_DETAILED,
				"Inter RAT HO to UTRAN is forbidden");
	}
	else 
	{
		if((RRM_ZERO == p_ue_context->ho_params.utra_known_cell_selection_list.count) &&
				(RRM_ZERO == p_ue_context->ho_params.utra_known_cell_selection_list.count) &&
				(RRM_ZERO == p_ue_context->ho_params.utra_known_cell_selection_list.count))
		{
			if((INVALID_PHYSICAL_CELL_ID == daho_pci) || (RRM_ZERO == daho_freq))
			{
				retval = RRM_FALSE;
				RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
						RRM_DETAILED,
						"No utran cell in cell selection list as well as no utran daho cell present");
			}
			else
			{
                /* SPR 22731 Fix Start */
                if(RRM_PNULL != p_cell_ctx)
                {
                    for(utran_fdd_count = RRM_ZERO; utran_fdd_count < p_cell_ctx->ran_info.mobility_params.
                            idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                            irat_eutran_to_utran_fdd_list.num_irat_eutran_to_utran_fdd_carriers; utran_fdd_count++)
                    {
                        if (daho_freq == p_cell_ctx->ran_info.mobility_params.
                                idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                                irat_eutran_to_utran_fdd_list.irat_eutran_to_utran_fdd_carriers[utran_fdd_count].utra_carrier_arfcn)
                        { 
                            if(RRM_FALSE  == rrm_ue_is_ue_supported_utra_fdd_frequency(daho_freq,p_ue_context))
				{
					retval = RRM_FALSE;
                                RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,RRM_INFO,
                                        "UTRAN FDD DAHO Freq:[%d] is not supported",daho_freq);
                                break;
                            }
                        }
                    }
                    if(utran_fdd_count == p_cell_ctx->ran_info.mobility_params.idle_mode_mobility_params.
                            idle_mode_mobility_irat_utra_params.irat_eutran_to_utran_fdd_list.num_irat_eutran_to_utran_fdd_carriers)
                    {
                        for(utran_tdd_count = RRM_ZERO; utran_tdd_count < p_cell_ctx->ran_info.mobility_params.
                                idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                                irat_eutran_to_utran_tdd_list.num_irat_eutran_to_utran_tdd_carriers; utran_tdd_count++)
                        {
                            if (daho_freq == p_cell_ctx->ran_info.mobility_params.
                                    idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                                    irat_eutran_to_utran_tdd_list.irat_eutran_to_utran_tdd_carriers[utran_tdd_count].utra_carrier_arfcn)
                            { 
                                if(RRM_FALSE  == rrm_ue_is_ue_supported_utra_tdd_frequency(daho_freq,p_ue_context))
                                {
                                    retval = RRM_FALSE;
                                    RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,RRM_INFO,
                                            "UTRAN TDD DAHO Freq:[%d] is not supported",daho_freq);
                                    break;
                                }
                            }
                        }
                    }
				}
                /* SPR 22731 Fix End */
			}
		}
	}
	RRM_UT_TRACE_EXIT();
	return retval;
}
rrm_bool_et rrm_determine_if_utran_ho_is_forbidden_or_not_allowed(rrm_ue_context_t*   p_ue_context)
{
    rrm_bool_et                    retval	                     = RRM_FALSE;
    rrm_ho_restriction_list_t	  *p_ho_restriction_list         = RRM_PNULL;
    U32                           feature_group_indicators       = (U32)-RRM_ONE;
    rrm_bool_et 		          fgi_supported                  = RRM_FALSE;
    rrm_bool_et                   irat_utra_forbidden            = RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    p_ho_restriction_list = &(p_ue_context->ho_restriction_list);

    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        feature_group_indicators = p_ue_context->ue_capability_params.
            rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.feature_group_indicators;
        if(feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_EIGHT)))
        {
            fgi_supported = RRM_TRUE;
        }
    }

    if(RRM_FALSE == fgi_supported)
    {
        RRM_TRACE (g_uem_log_on_off,p_rrm_uem_facility_name,
                RRM_DETAILED,"FGI 8 is not supported");
        retval = RRM_TRUE;
    }
    else
    {    
        if ( (RRM_FALSE == rrm_is_ue_special_previledge(p_ue_context)) &&
                (p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) )
        {
            if((p_ho_restriction_list->forbidden_inter_rat == \
                        FORBIDDEN_INTER_RAT_UTRAN_TYPE) || \
                    (p_ho_restriction_list->forbidden_inter_rat == \
                     FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN) || \
                    (p_ho_restriction_list->forbidden_inter_rat == \
                     FORBIDDEN_INTER_RAT_ALL))
            {
                irat_utra_forbidden = RRM_TRUE;
            }
        }
        if(p_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD)
        {
            if(p_ue_context->ue_context_mod_req.csf_indicator == RRM_CSFB_HIGH_PRIORITY)
            {
                RRM_TRACE (g_uem_log_on_off, 
                        p_rrm_uem_facility_name,
                        RRM_DETAILED, 
                        "CSFB is HIGH PRIORITY");
                irat_utra_forbidden = RRM_FALSE;
            }
        }
        if(irat_utra_forbidden == RRM_TRUE)
        {
            retval = RRM_TRUE;
            RRM_TRACE ( g_uem_log_on_off, p_rrm_uem_facility_name,
                    RRM_DETAILED,
                    "Inter RAT HO to UTRAN is forbidden");
        }
    } 	
    RRM_UT_TRACE_EXIT();
    return retval;
}
/*SPR_17893_END*/
