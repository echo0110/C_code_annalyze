/* REL1.1 CHANGES : STARTS */

/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * SPR-612 Coverity Fix Starts
 * $Id: son_apps_pci.c $
 * SPR-612 Coverity Fix Ends
 *
 ****************************************************************************
 *
 * File Description: Processes the SONApps PCI feature events. It contains
 *                   the definition of various event handlers for SONApps
 *                   PCI feature.
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *
 * SPR-612 Coverity Fix Starts
 * $Log: son_apps_pci.c $
 * SPR-612 Coverity Fix Ends
 * Revision 0.1  2012/05/05  gur25767
 *               2012/07/26  gur27278  Removed compilation warnings
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_apps_fsm.h>
#include <son_apps_pci.h>
#include <son_anr_intf.h>

const S8 *SONAPPS_PCI_FSM_STATES_NAMES[] =
{
	(const S8 *)"SONAPPS_PCI_DISABLED",
	(const S8 *)"SONAPPS_PCI_ENABLED",
	(const S8 *)"SONAPPS_PCI_DISABLE_IN_PROGRESS",
	(const S8 *)"SONAPPS_PCI_ENABLE_IN_PROGRESS",
	(const S8 *)"SONAPPS_PCI_SEL_RESEL_IN_PROGRESS",
};

/*****************************************************************************
 * Function Name  : sonapps_find_error_code
 * Inputs         : p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : error_code - error code corresponding to the current state
 *                  of APPS and PCI feature
 * Description    : This function finds the error code corresponding to the
 *                  current state of APPS and PCI feature.
 ****************************************************************************/
static son_error_et
sonapps_find_error_code
(
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_error_et error_code = SON_NO_ERROR;
	sonapps_pci_status_et pci_fsm_state;

	SON_UT_TRACE_ENTER();

	pci_fsm_state = p_sonapps_gb_context->pci_fsm_state;

	/* If SONAPPS is not in active state */
	if (p_sonapps_gb_context->g_sonapps_state != SONAPP_STATE_ACTIVE)
	{
		error_code = SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;
	}

	/* If SONAPPS is in active state but PCI is not in a vaild for the current
	 * msg */
	else
	{
		switch (pci_fsm_state)
		{
			case SONAPPS_PCI_DISABLED:
				{
					error_code = SON_ERR_PCI_DISABLED;
					break;
				}

			case SONAPPS_PCI_DISABLE_IN_PROGRESS:
				{
					error_code = SON_ERR_DISABLE_IN_PROGRESS;
					break;
				}

			case SONAPPS_PCI_ENABLE_IN_PROGRESS:
				{
					error_code = SON_ERR_ENABLE_IN_PROGRESS;
					break;
				}

			case SONAPPS_PCI_SEL_RESEL_IN_PROGRESS:
				{
					error_code = SON_ERR_PCI_SELECTION_IN_PROGRESS;
					break;
				}

			default:
				error_code = SON_NO_ERROR;
		} /* switch */
	}

	SON_UT_TRACE_EXIT();
	return error_code;
} /* sonapps_find_error_code */

/*****************************************************************************
 * Function Name  : sonapps_select_unused_pci
 * Inputs         : p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 *                  in_use_pci_list - List that contains the indication
 *                  for the PCIs being used, out of the valid PCI values i.e.
 *                  0 - 503.
 *                  Eg. if PCI = 5 is in use, in_use_pci_list[5] = SON_TRUE
 *                  otherwise in_use_pci_list[5] = SON_FALSE
 * Outputs        : None
 * Returns        : selected_pci  - PCI that is not in use
 * Description    : This function selects an unused PCI from the PCI list
 *                  provided by OAM on the basis of In-Use PCI information
 *                  provided by ANR/NMM
 ****************************************************************************/
static son_u16
sonapps_select_unused_pci
(
 son_apps_gb_context_t            *p_sonapps_gb_context,
    /* Spr 17753 Changes Start */
    son_used_pci_info_t              *p_msg
    /* Spr 17753 Changes End */
)
{
    son_u16 index        = SON_NULL;
    son_u16 selected_pci = SONAPPS_INVALID_PCI;
    /* Spr 17753 Changes Start */
    /* sub set of PCI, not conflicting with First degree nbr */
    son_u16 pci_list_for_sel_lev_1[SON_MAX_PCI] = {SON_NULL};
    /* sub set of PCI, not conflicting with both First and second degree nbr */
    son_u16 pci_list_for_sel_lev_2[SON_MAX_PCI] = {SON_NULL};
    son_bool_et pci_in_use =  SON_TRUE;
    son_bool_et is_conflict_w_fd =  SON_FALSE;
    son_bool_et is_conflict_w_sd =  SON_FALSE;
    son_bool_et serving_pci_present_in_received_list =  SON_FALSE;
    son_u16 pci_count = SON_NULL;
    son_u16 pci_count_2 = SON_NULL;
    son_u16 index_1   = SON_NULL;
    son_u8  rand_index = SON_NULL;
    SON_UT_TRACE_ENTER();

    if(SONAPPS_INVALID_PCI != p_msg->served_cell_pci)
    {
        /* PCI Reselection Triggered */
        /* Check if conflict with first degree is present or not */
        for (index = SON_NULL; index < p_msg->num_pci_in_use_w_first_degree; index++)
        {
            if (p_msg->pci_list_level_1[index] == p_msg->served_cell_pci)
            {
                is_conflict_w_fd = SON_TRUE;
                break;
            }
        }
        if(SON_FALSE == is_conflict_w_fd)
        {
            /* Check if conflict with second degree is present or not */
            for (index = SON_NULL; index < SON_MAX_PCI; index++)
            {
                if(SON_TRUE == p_msg->pci_list_level_2[index])
                {
                    if(index == p_msg->served_cell_pci)
                    {
                        is_conflict_w_sd = SON_TRUE;
                        break;
                    }
                }
            }
        }
    }
    /* Creating the List of PCI, non conflicting with first degree neighbors */ 
    SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
            SON_DETAILED, "List of PCIs not conflicting with first degree nbrs");

    for(index = SON_NULL; index < p_sonapps_gb_context->pci_list_size; index++)
    {
        pci_in_use = SON_FALSE;
        for(index_1 = SON_NULL ; index_1<p_msg->num_pci_in_use_w_first_degree; index_1++)
        {
            if(p_sonapps_gb_context->pci_list[index] == p_msg->pci_list_level_1[index_1])
            {
                pci_in_use = SON_TRUE;
                break;
            }

        }
        if(SON_FALSE == pci_in_use)
        {
            /* pci not in use with first degree*/
            pci_list_for_sel_lev_1[pci_count++] = p_sonapps_gb_context->pci_list[index]; 
            SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                    SON_DETAILED, "[PCI]=%u",p_sonapps_gb_context->pci_list[index]);
        }
    }
    if(SONAPPS_INVALID_PCI != p_msg->served_cell_pci)
    {
        SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                SON_DETAILED, "List of free PCIs available for selection");

        /* Search and select the available pci, in second degree list and 
         * create the list of pci which is non conflicting with both the nbrs */

        for(index = SON_NULL; index < pci_count; index++)
        {
            pci_in_use = SON_FALSE;
            for(index_1 = SON_NULL; index_1 < SON_MAX_PCI; index_1++)
            {
                if(p_msg->pci_list_level_2[index_1] == SON_TRUE)
                {
                    if(pci_list_for_sel_lev_1[index] == index_1)
                    {
                        pci_in_use = SON_TRUE;
                        break;
                    }
                }
            }
            if(SON_FALSE == pci_in_use)
            {
                /* Populating the list of free pci */
                pci_list_for_sel_lev_2[pci_count_2++] = pci_list_for_sel_lev_1[index];
                SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                        SON_DETAILED, "[PCI]=%u",pci_list_for_sel_lev_1[index]);
            }
        }
        if(SON_NULL < pci_count_2)
        {
            /* Select random pci from the free pci's */ 
            rand_index = son_anr_get_random_val(SON_NULL,pci_count_2);
            selected_pci = pci_list_for_sel_lev_2[rand_index];    
        }	
    }
    /* No conflict Found */
    if((SON_FALSE == is_conflict_w_fd) && (SON_FALSE == is_conflict_w_sd))
    {
        if(SONAPPS_INVALID_PCI != p_msg->served_cell_pci)
        {
            /* Check if Serving cell pci is present in updated list received from OAM or not */
            for(index = SON_NULL; index < p_sonapps_gb_context->pci_list_size; index++)
            {
                /* First index in pci_list_level_1 is for serving cell PCI */
                if(p_sonapps_gb_context->pci_list[index] == p_msg->served_cell_pci)
                {
                    serving_pci_present_in_received_list = SON_TRUE;
                    break;
                }
            }
            if(serving_pci_present_in_received_list)
            {
                /* No need to reselect PCI as no conflict found and the serving pci is also present in list rcvd from OAM */
                selected_pci = SONAPPS_PCI_RESELECTION_NOT_REQUIRED; 
                SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                        SON_DETAILED, "PCI Reslection not required as No conflict is present"
                        " and seving cell pci is also present in updated list received from OAM");
            }
        }
        else
        {
            /* PCI Selection Case Through NMM */
            if(SON_NULL < pci_count)
            {
                rand_index = son_anr_get_random_val(SON_NULL,pci_count);
                selected_pci = pci_list_for_sel_lev_1[rand_index];    
            }

        }
    }
    else if(is_conflict_w_fd == SON_TRUE)
    {
        if((SON_NULL < pci_count) && (SONAPPS_INVALID_PCI == selected_pci))
        {
            /* no pci is present which non conflicting with both fd and sd nbrs
             * selecting the random pci which is atleast non conflicting with 
             * first degree nbrs */  

            SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                    SON_WARNING, "Selecting non conflicting pci randomly"
                    " from available non conflicting pci's with first degree");

            /* select random pci from available non conflicting pci list */
            rand_index = son_anr_get_random_val(SON_NULL,pci_count);
            selected_pci = pci_list_for_sel_lev_1[rand_index];
        }
        /* If value of selected_pci is not updated from SONAPPS_INVALID_PCI while
         * the selection process, then all the PCIs are in use */
    }
    else if(is_conflict_w_sd == SON_TRUE)
    {
        if(SONAPPS_INVALID_PCI == selected_pci)
        {
            /* Check if updated list is received from OAM or not */
            for(index = SON_NULL; index < p_sonapps_gb_context->pci_list_size; index++)
            {
                /* First index in pci_list_level_1 is for serving cell PCI */
                if(p_sonapps_gb_context->pci_list[index] == p_msg->served_cell_pci)
                {
                    serving_pci_present_in_received_list = SON_TRUE;
                    break;
                }
            }
            if(serving_pci_present_in_received_list)
            {    
                selected_pci = SONAPPS_PCI_RESELECTION_NOT_REQUIRED; 
                SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                        SON_WARNING, "conflict was detected with second degree nbr"
                        " and no free pci available, which is non conflicting"
                        " with both First and Second degree nbrs"
                        " pci reselection not required");
            }
            else
            {
                /* selecting the random pcis from updated list received from OAM */
                if(SON_NULL < pci_count)
                {
                    rand_index = son_anr_get_random_val(SON_NULL,pci_count);
                    selected_pci = pci_list_for_sel_lev_1[rand_index];
                }
            } 
        }
    }
    if(SONAPPS_INVALID_PCI == selected_pci)
    {
        SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
                SON_WARNING, "All the PCIs provided by OAM are in use");
    }
    /* Spr 17753 Changes End */
    SON_UT_TRACE_EXIT();

    return selected_pci;
} /* sonapps_select_unused_pci */

/*****************************************************************************
 * Function Name  : sonapps_pci_send_pci_selection_res
 * Inputs         : p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 *                  pci   - unused PCI(Success case) or SONAPPS_INVALID_PCI
 *                  (Failure case)
 *                  error_code - SON_NO_ERROR (Success case) or
 *                               indicates the cause (Failure case)
 *                  transaction_id - transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends the SONAPPS_PCI_SELECTION_RES to
 *                  SON MgmtIfH
 ****************************************************************************/
static son_void_t
sonapps_pci_send_pci_selection_res
(
 son_apps_gb_context_t   *p_sonapps_gb_context,
 son_u16 pci,
 son_error_et error_code,
 son_u16 transaction_id
 )
{
	sonapps_pci_selection_res_t apps_pci_selection_res = {0};

	SON_UT_TRACE_ENTER();

	/* Populate SONAPPS_PCI_SELECTION_RES message data */
	apps_pci_selection_res.transaction_id = transaction_id;

	/* pci contains the value of unused PCI in case of Success and
	 * SONAPPS_INVALID_PCI in case of Failure */
	apps_pci_selection_res.pci_selection_res.selected_pci = pci;

	if (error_code == SON_NO_ERROR)
	{
		apps_pci_selection_res.pci_selection_res.result = SON_SUCCESS;

		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_INFO,
				"Sending SONAPPS_PCI_SELECTION_RES to OAM with "
				"selected PCI = %d", pci);
	}
	else
	{
		apps_pci_selection_res.pci_selection_res.result = SON_FAILURE;

		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_ERROR,
				"SONAPPS_PCI_SELECTION_REQ failed!!! Sending Failure "
				"response to OAM. Error code = %d", error_code);
	}

	apps_pci_selection_res.pci_selection_res.error_code = error_code;

	/* Send SONAPPS_PCI_SELECTION_RES to SONMgmtIfh */
	son_create_send_buffer((son_u8 *)&apps_pci_selection_res,
			SON_APPS_MODULE_ID, SON_MIF_MODULE_ID, SONAPPS_PCI_SELECTION_RES,
			sizeof(sonapps_pci_selection_res_t));

	SON_UT_TRACE_EXIT();
} /* sonapps_pci_send_pci_selection_res */

/*****************************************************************************
 * Function Name  : sonapps_pci_send_pci_reselection_res
 * Inputs         : p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 *                  pci   - unused PCI or SONAPPS_INVALID_PCI in case failure
 *                  error_code - indicates the cause in case of failure
 *                               SON_NO_ERROR in case of success
 *                  transaction_id - Transaction Id of PCI Re-Selection Request
 *                  src_cgi  - Source Cell ID
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends the SONAPPS_PCI_RESELECTION_RES to
 *                  SON MgmtIfH
 ****************************************************************************/
static son_void_t
sonapps_pci_send_pci_reselection_res
(
 son_apps_gb_context_t   *p_sonapps_gb_context,
 son_u16 pci,
 son_error_et error_code,
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t src_cgi /*SPR-12423-Start-End*/
 )
{
	sonapps_pci_reselection_res_t apps_pci_reselection_res = {0};

	SON_UT_TRACE_ENTER();

	/* Populate SONAPPS_PCI_RESELECTION_RES message data */
	apps_pci_reselection_res.transaction_id = transaction_id;

	/*SPR-12423-Start*/
	SON_MEMCPY(&apps_pci_reselection_res.pci_reselection_res.src_cgi, &src_cgi,
			sizeof(son_intra_rat_global_cell_id_t));
	/*SPR-12423-End*/

	/* pci contains the value of unused PCI in case of Success and
	 * SONAPPS_INVALID_PCI in case of Failure */
	apps_pci_reselection_res.pci_reselection_res.reselected_pci = pci;
	if (error_code == SON_NO_ERROR)
	{
		apps_pci_reselection_res.pci_reselection_res.result = SON_SUCCESS;

		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_INFO,
				"Sending SONAPPS_PCI_RESELECTION_RES to OAM with "
				"re-selected PCI = %d", pci);
	}
	else
	{
		apps_pci_reselection_res.pci_reselection_res.result = SON_FAILURE;

		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_ERROR,
				"SONAPPS_PCI_RESELECTION_REQ failed!!! Sending Failure "
				"response to OAM. Error code = %d", error_code);
	}

	apps_pci_reselection_res.pci_reselection_res.error_code = error_code;

	/* Send SONAPPS_PCI_RESELECTION_RES to SONMgmtIfh */
	son_create_send_buffer((son_u8 *)&apps_pci_reselection_res,
			SON_APPS_MODULE_ID, SON_MIF_MODULE_ID, SONAPPS_PCI_RESELECTION_RES,
			sizeof(sonapps_pci_reselection_res_t));

	SON_UT_TRACE_EXIT();
} /* sonapps_pci_send_pci_reselection_res */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_invalid_event_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  from SONMgmtIfH for PCI feature in invalid state of PCI
 *                  or SONAPPS.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_invalid_event_handler
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
			SON_WARNING, "Message with API Id = [%u] received in "
			"invalid state- APPS state = [%s], PCI state = [%s]",
            		p_cspl_hdr->api,
			SON_APPS_FSM_STATES_NAMES[p_sonapps_gb_context->g_sonapps_state],
			SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
	LTE_GCC_UNUSED_PARAM(p_msg);
                /*SPR 17777 +-*/

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_enable_disable_req_nack
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ
 *                  message received from SONMgmtIfH
 *                  if the message is received in an
 *                  invalid state of PCI or SONAPPS.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_enable_disable_req_nack
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	sonapps_pci_conflict_enable_disable_res_t pci_enable_disable_res    = {0};
	sonapps_pci_conflict_enable_disable_req_t    *p_pci_enable_disable_req =
		(sonapps_pci_conflict_enable_disable_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	pci_enable_disable_res.transaction_id =
		p_pci_enable_disable_req->transaction_id;
	pci_enable_disable_res.pci_enable_disable_res.result = SON_FAILURE;

	pci_enable_disable_res.pci_enable_disable_res.error_code =
		sonapps_find_error_code(p_sonapps_gb_context);

	SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
			SON_WARNING, "Message with API Id = [%u] received in "
			"invalid state- APPS state = [%u], PCI state = [%s]", p_cspl_hdr->api,
			p_sonapps_gb_context->g_sonapps_state,
			SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);

	/* Send SONAPPS_PCI_CONLICT_ENABLE_DISABLE_RES with Failure to SON MgmtIfH */
	son_create_send_buffer((son_u8 *)&pci_enable_disable_res, SON_APPS_MODULE_ID
			, SON_MIF_MODULE_ID, SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES,
			sizeof(sonapps_pci_conflict_enable_disable_res_t));

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_enable_disable_req_nack */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_selection_req_nack
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *                  message received from SONMgmtIfH, if the message is
 *                  received in an invalid state of PCI or SONAPPS.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_selection_req_nack
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_error_et error_code             = SON_NO_ERROR;

	sonapps_pci_selection_req_t *p_sonapps_pci_selection_req =
		(sonapps_pci_selection_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_WARNING,
			"PCI SELECTION REQ recieved in invalid state");
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	error_code = sonapps_find_error_code(p_sonapps_gb_context);

	/* Send SONAPPS_PCI_SELECTION_RES with Failure to SONMgmtIfh */
	sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
			SONAPPS_INVALID_PCI, error_code,
			p_sonapps_pci_selection_req->transaction_id);

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_selection_req_nack */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_reselection_req_nack
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *                  message received from SONMgmtIfH, if the message is
 *                  received in an invalid state of PCI or SONAPPS.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_reselection_req_nack
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	sonapps_pci_reselection_req_t   *p_sonapps_pci_reselection_req =
		(sonapps_pci_reselection_req_t *)p_msg;
	son_error_et error_code = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_BRIEF,
			"PCI RE-SELECTION REQ recieved in invalid state");
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	error_code = sonapps_find_error_code(p_sonapps_gb_context);

	/* Send SONAPPS_PCI_RESELECTION_RES with Failure to SONMgmtIfh */
	sonapps_pci_send_pci_reselection_res(p_sonapps_gb_context,
			SONAPPS_INVALID_PCI, error_code,
			p_sonapps_pci_reselection_req->transaction_id,
			p_sonapps_pci_reselection_req->pci_reselection_req.src_cgi); /*SPR-12423-Start-End*/

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_reselection_req_nack */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_enable_disable_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ
 *                  message received from SONMgmtIfH in
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_enable_disable_req
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_error_et error_code                = SON_NO_ERROR;
	sonapps_pci_conflict_enable_disable_res_t apps_pci_enable_disable_res = {0};
	son_anr_pci_reg_dereg_req_t pci_reg_dereg_req           = {0};

	sonapps_pci_conflict_enable_disable_req_t *p_pci_enable_disable_req =
		(sonapps_pci_conflict_enable_disable_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_DETAILED,
			"PCI ENABLE/DISABLE REQ recieved ");

	/* If PCI enable/disable status is already same as requested by OAM  or
	 * PCI enable/disable request is already being processed */
	switch (p_pci_enable_disable_req->pci_enable_disable_req.pci_mode)
	{
		case SON_PCI_ENABLE:
			{
				if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_ENABLE_IN_PROGRESS)
				{
					error_code = SON_ERR_ENABLE_IN_PROGRESS;
				}
				else if (p_sonapps_gb_context->pci_fsm_state ==
						SONAPPS_PCI_SEL_RESEL_IN_PROGRESS ||
						p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_ENABLED)
				{
					error_code = SON_ERR_ALREADY_ENABLED;
				}

				break;
			}

		case SON_PCI_DISABLE:
			{
				if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_DISABLE_IN_PROGRESS)
				{
					error_code = SON_ERR_DISABLE_IN_PROGRESS;
				}
				else if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_DISABLED)
				{
					error_code = SON_ERR_ALREADY_DISABLED;
				}

				break;
			}
	} /* switch */

	/* If msg is received in an invalid state of PCI */
	if (error_code != SON_NO_ERROR)
	{
                /*SPR 17777 +-*/
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_BRIEF, "SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ failed!! "
				"Sending Failure response to OAM. Error code = %d", error_code);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

		apps_pci_enable_disable_res.transaction_id =
			p_pci_enable_disable_req->transaction_id;
		apps_pci_enable_disable_res.pci_enable_disable_res.result = SON_FAILURE;
		apps_pci_enable_disable_res.pci_enable_disable_res.error_code =
			error_code;

		/* Send SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES with Failure to SON MgmtIfH */
		son_create_send_buffer((son_u8 *)&apps_pci_enable_disable_res,
				SON_APPS_MODULE_ID, SON_MIF_MODULE_ID,
				SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES,
				sizeof(sonapps_pci_conflict_enable_disable_res_t));
	}

	/* Register/De-register with SONANR to receive PCI conflict/confusion
	 * indications */
	else
	{
		/* Update the global context for PCI feature status */
		if (p_pci_enable_disable_req->pci_enable_disable_req.pci_mode ==
				SON_PCI_ENABLE)
		{
			p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_ENABLE_IN_PROGRESS;
		}
		else
		{
			p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_DISABLE_IN_PROGRESS;
		}

		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_INFO, "### PCI_FSM: State changed to %s ###",
				SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);

		/* Populate SONANR_PCI_REG_DEREG_REQ message data */
		pci_reg_dereg_req.transaction_id =
			p_pci_enable_disable_req->transaction_id;
		pci_reg_dereg_req.pci_mode =
			p_pci_enable_disable_req->pci_enable_disable_req.pci_mode;

		/* Send SONANR_PCI_REG_DEREG_REQ to SONANR */
		son_create_send_buffer((son_u8 *)&pci_reg_dereg_req, SON_APPS_MODULE_ID,
				SON_ANR_MODULE_ID, SONANR_PCI_REG_DEREG_REQ,
				sizeof(son_anr_pci_reg_dereg_req_t));

		sonapps_print_global_context(p_sonapps_gb_context,
				p_sonapps_gb_context->pci_log_enable);
	}

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_enable_disable_req */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_reg_dereg_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_REG_DEREG_RES
 *                  message received from SONANR in
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_reg_dereg_res
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	sonapps_pci_conflict_enable_disable_res_t apps_pci_enable_disable_res = {0};
	son_anr_pci_reg_dereg_res_t       *p_pci_reg_dereg_res        =
		(son_anr_pci_reg_dereg_res_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_DETAILED,
			"PCI REG/DEREG RES recieved ");

	/* If PCI Dereg response is received in SONAPPS_PCI_ENABLE_IN_PROGRESS
	 * state */
	if ((SONAPPS_PCI_ENABLE_IN_PROGRESS == p_sonapps_gb_context->pci_fsm_state) &&
			(SON_PCI_DISABLE == p_pci_reg_dereg_res->pci_mode))
	{
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "PCI Deregistration Response received from "
				"ANR in PCI state [%s]. Sending PCI Disable response to OAM",
				SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);
	}

	/* If PCI Registration response is received in
	 * SONAPPS_PCI_DISABLE_IN_PROGRESS state*/
	if ((SONAPPS_PCI_DISABLE_IN_PROGRESS == p_sonapps_gb_context->pci_fsm_state) &&
			(SON_PCI_ENABLE == p_pci_reg_dereg_res->pci_mode))
	{
                /*SPR 17777 +-*/
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "PCI Registration Response received in from "
				"ANR in PCI state [%s]. Sending PCI Enable response to OAM",
				SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
	}
	else
	{
		if (p_pci_reg_dereg_res->result == SON_FAILURE)
		{
			/* Update the PCI feature mode status */
			if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_ENABLE_IN_PROGRESS)
			{
				/* Set the PCI feature to Disabled mode */
				p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_DISABLED;
			}
			else if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_DISABLE_IN_PROGRESS)
			{
				/* Set the PCI feature to Enabled mode */
				p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_ENABLED;
			}
		}
		else
		{
			/* Update the PCI feature mode status */
			if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_ENABLE_IN_PROGRESS)
			{
				p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_ENABLED;
			}
			else if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_DISABLE_IN_PROGRESS)
			{
				if (p_sonapps_gb_context->pci_list != SON_PNULL)
				{
					son_mem_free(p_sonapps_gb_context->pci_list);
					p_sonapps_gb_context->pci_list_size = SON_NULL;
				}

				p_sonapps_gb_context->pci_list = SON_PNULL;
				p_sonapps_gb_context->transaction_id[SONAPPS_PCI] =
					SONAPPS_INVALID_VALUE;
				p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_DISABLED;
			}
		}

		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_INFO, "### PCI_FSM:State changed to %s ###",
				SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);
	}

	/* Populate SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES message data */
	apps_pci_enable_disable_res.transaction_id =
		p_pci_reg_dereg_res->transaction_id;
	apps_pci_enable_disable_res.pci_enable_disable_res.result =
		p_pci_reg_dereg_res->result;
	apps_pci_enable_disable_res.pci_enable_disable_res.error_code =
		p_pci_reg_dereg_res->error_code;

	/* Send SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES to SON MgmtIfH */
	son_create_send_buffer((son_u8 *)&apps_pci_enable_disable_res,
			SON_APPS_MODULE_ID, SON_MIF_MODULE_ID,
			SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES,
			sizeof(sonapps_pci_conflict_enable_disable_res_t));

	sonapps_print_global_context(p_sonapps_gb_context,
			p_sonapps_gb_context->pci_log_enable);

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_reg_dereg_res */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_selection_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *                  message received from SONMgmtIfH in
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_selection_req
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_error_et ret_val = SON_NO_ERROR;
	son_u16 index = SON_NULL;
	son_u16 cnt   = SON_NULL;
	son_bool_et selection_req_valid = SON_FALSE;
	son_bool_et selection_req_meas_bw_valid = SON_FALSE;

	sonapps_pci_selection_req_t *p_sonapps_pci_selection_req =
		(sonapps_pci_selection_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_DETAILED,
			"PCI SELECTION REQ recieved ");

	/* If the PCI list provided by OAM is empty */
	if (p_sonapps_pci_selection_req->pci_selection_req.pci_list_size == SON_NULL)
	{
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "PCI Selection list empty");

		sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
				SONAPPS_INVALID_PCI, SON_ERR_EMPTY_PCI_LIST,
				p_sonapps_pci_selection_req->transaction_id);
	}
	/* If PCI list size is greater than the max no. of valid PCI values */
	else if (p_sonapps_pci_selection_req->pci_selection_req.pci_list_size >
			SON_MAX_PCI)
	{
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "Excess no of elements in PCI Selection list");

		sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
				SONAPPS_INVALID_PCI, SON_ERR_INVALID_PARAMS,
				p_sonapps_pci_selection_req->transaction_id);
	}
	/* If measurement bandwidth list size is greater than the max no. of valid meas bw values */
	else if (p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list_size >
			SON_MAX_MEAS_BW)
	{
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "Excess no of elements in measurement bandwidth list");

		sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
				SONAPPS_INVALID_PCI, SON_ERR_INVALID_PARAMS,
				p_sonapps_pci_selection_req->transaction_id);
	}
	/*SPR-12944-Start*/
	else if(SON_FALSE == p_sonapps_gb_context->nmm_support_present)
	{
		/*NMM support is not present,send the first PCI from PCI list 
		  provided by OAM in PCI selection response*/
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_ERROR, "NMM support is not present"
				"Sending the first PCI provided by OAM in PCI selection response");

		sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
				p_sonapps_pci_selection_req->pci_selection_req.pci_list[0],SON_NO_ERROR,
				p_sonapps_pci_selection_req->transaction_id);
	}
	/*SPR-12944-End*/
	else
	{
		/* Print PCI list received from OAM */
		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_DETAILED,
				"PCIs recieved from OAM");
		for (index = 0; index < p_sonapps_pci_selection_req->
				pci_selection_req.pci_list_size; index++)
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_DETAILED,
					"%d ", p_sonapps_pci_selection_req->
					pci_selection_req.pci_list[index]);

			if (SON_MAX_PCI >
					p_sonapps_pci_selection_req->pci_selection_req.pci_list[index])
			{
				selection_req_valid = SON_TRUE;
			}
		}

		/* Print measurement bw list received from OAM */
		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_DETAILED,
				"Measurement bandwidths recieved from OAM");
		for (index = 0; index < p_sonapps_pci_selection_req->
				pci_selection_req.meas_bw_list_size; index++)
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_DETAILED,
					"%d ", p_sonapps_pci_selection_req->
					pci_selection_req.meas_bw_list[index]);

			if (mbw_6   == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
					mbw_15  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
					mbw_25  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
					mbw_50  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
					mbw_75  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
					mbw_100 == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index])
			{
				selection_req_meas_bw_valid = SON_TRUE;
			}
		}

		/* If All the PCIs received from OAM are invalid */
		if (SON_FALSE == selection_req_valid)
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_ERROR,
					"All PCIs received from OAM are invalid");

			sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
					SONAPPS_INVALID_PCI, SON_ERR_INVALID_PARAMS,
					p_sonapps_pci_selection_req->transaction_id);
		}
		else if (SON_FALSE == selection_req_meas_bw_valid &&
				p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list_size)
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_ERROR,
					"All measurement bandwidth received from OAM are invalid");

			sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
					SONAPPS_INVALID_PCI, SON_ERR_INVALID_MEAS_BW,
					p_sonapps_pci_selection_req->transaction_id);
		}
		else
		{
			/* Store the transaction ID corresponding to PCI feature */
			p_sonapps_gb_context->transaction_id[SONAPPS_PCI] =
				p_sonapps_pci_selection_req->transaction_id;

			/* store earfcn in global context */
			p_sonapps_gb_context->pci_selection_earfcn =
				p_sonapps_pci_selection_req->pci_selection_req.earfcn;

			/* Update PCI List size in global context */
			p_sonapps_gb_context->pci_list_size =
				p_sonapps_pci_selection_req->pci_selection_req.pci_list_size;

			/* Store the list of PCIs received by OAM in the global context */
			p_sonapps_gb_context->pci_list = son_mem_get(
					p_sonapps_pci_selection_req->pci_selection_req.pci_list_size *
					sizeof(son_u16));

			if (SON_PNULL == p_sonapps_gb_context->pci_list)
			{
				SON_UT_TRACE_EXIT();
				/* Release the buffer memory as it is no longer needed. */
				return RELEASE_SON_BUFFER;
			}

			SON_MEMCPY(p_sonapps_gb_context->pci_list,
					p_sonapps_pci_selection_req->pci_selection_req.pci_list,
					p_sonapps_pci_selection_req->pci_selection_req.pci_list_size *
					sizeof(son_u16));

			if (p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list_size)
			{
				/* Update OAM received meas bw data in global context */

				/* Store the list of meas bw received by OAM in the global context */
				p_sonapps_gb_context->meas_bw_list = son_mem_get(
						p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list_size *
						sizeof(son_u8));

				if (SON_PNULL == p_sonapps_gb_context->meas_bw_list)
				{
					son_mem_free(p_sonapps_gb_context->pci_list);
					SON_UT_TRACE_EXIT();
					/* Release the buffer memory as it is no longer needed. */
					return RELEASE_SON_BUFFER;
				}

				for (index = 0; index < p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list_size;
						index++)
				{
					if (mbw_6   == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
							mbw_15  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
							mbw_25  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
							mbw_50  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
							mbw_75  == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index] ||
							mbw_100 == p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index])
					{
						/* Valid meas bandwidth received from OAM */
						p_sonapps_gb_context->meas_bw_list[cnt++] =
							p_sonapps_pci_selection_req->pci_selection_req.meas_bw_list[index];
					}
				}

				/* Update meas bw list size in global context */
				p_sonapps_gb_context->meas_bw_list_size = cnt;
			}
			else
			{
				son_u8 meas_bws[SONAPPS_NO_EARFCN_6] = {mbw_6, mbw_15, mbw_25, mbw_50, mbw_75, mbw_100};
				p_sonapps_gb_context->meas_bw_list = son_mem_get(
						SONAPPS_NO_EARFCN_6 * sizeof(son_u8));

				if (SON_PNULL == p_sonapps_gb_context->meas_bw_list)
				{
					son_mem_free(p_sonapps_gb_context->pci_list);
					SON_UT_TRACE_EXIT();
					/* Release the buffer memory as it is no longer needed. */
					return RELEASE_SON_BUFFER;
				}

				SON_MEMCPY(p_sonapps_gb_context->meas_bw_list,
						meas_bws,
						SONAPPS_NO_EARFCN_6 * sizeof(son_u8));
				/* Update meas bw list size in global context */
				p_sonapps_gb_context->meas_bw_list_size = SONAPPS_NO_EARFCN_6;
			}

			for (index = 0; index < p_sonapps_gb_context->meas_bw_list_size; index++)
			{
				/* Send SONNMM_START_SCAN_REQ to SONNMM */
				ret_val = sonapps_prepare_and_send_nw_scan_req(p_sonapps_gb_context,
						p_sonapps_pci_selection_req, SON_PCI_FEATURE, p_cspl_hdr);

				/* If EARFCN value provided by OAM is invalid */
				if (ret_val == SON_ERR_INVALID_EARFCN)
				{
					sonapps_pci_send_pci_selection_res(p_sonapps_gb_context,
							SONAPPS_INVALID_PCI, SON_ERR_INVALID_EARFCN,
							p_sonapps_pci_selection_req->transaction_id);
					break;
				}
				else if (ret_val == SON_ERR_SYS_MEM_ALLOC_FAILURE)
				{
					SON_LOG(p_sonapps_gb_context->pci_log_enable,
							p_son_apps_facility_name, SON_ERROR,
							"Memory allocation failed for creating scan request structure");
				}
				else if (ret_val == SON_ERR_UNEXPECTED_MSG)
				{
					SON_LOG(p_sonapps_gb_context->pci_log_enable,
							p_son_apps_facility_name, SON_ERROR,
							"Failed to create nw scan message");
				}
				else
					/* If Scan request is sent to SONNMM successfully */
				{
					/* Update the PCI state to inidicate PCI selection in progress */
					p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_SEL_RESEL_IN_PROGRESS;

					SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
							SON_INFO, "### PCI_FSM: State changed to %s ###",
							SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);

					sonapps_print_global_context(p_sonapps_gb_context,
							p_sonapps_gb_context->pci_log_enable);
				}
			}

			if (SON_NULL == p_sonapps_gb_context->num_nw_scan_req_sent_for_pci)
			{
				/* Free the memory of PCI list received by OAM in PCI selection request */
				son_mem_free(p_sonapps_gb_context->pci_list);
				p_sonapps_gb_context->pci_list = SON_PNULL;
				son_mem_free(p_sonapps_gb_context->meas_bw_list);
				p_sonapps_gb_context->meas_bw_list = SON_PNULL;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_selection_req */

/*****************************************************************************
 * Function Name  : sonapps_pci_scan_res_handler
 * Inputs         : p_nmm_start_scan_res - Pointer to the NW scan response
 *                  received from NMM(after the CSPL header)
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONNMM_START_SCAN_RES
 *                  message received from SONNMM in SONAPP_STATE_ACTIVE_state
 *                  for the PCI feature.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_scan_res_handler
(
 son_nmm_start_scan_res_t             *p_nmm_start_scan_res,
 son_apps_gb_context_t                *p_sonapps_gb_context
 )
{
	son_u16 pci_id                 = SON_NULL;
	son_u16 cell_index             = SON_NULL;
	son_u32 count                  = SON_NULL;
	son_u32 no_of_nodes            = SON_NULL;
	son_return_et result                 = SON_FAILURE;
	son_error_et error_code             = SON_NO_ERROR;
	son_u16 unused_pci             = SONAPPS_INVALID_PCI;
	nmm_scan_res_list_t            *p_eutran_scan_res_list   = SON_PNULL;
	start_scan_res_eutran_data_t   *p_start_scan_res_data = SON_PNULL;
    /* Spr 17753 Changes Start */
    son_used_pci_info_t            *p_pci_list = SON_PNULL; 
    /* Spr 17753 Changes End */

	SON_UT_TRACE_ENTER();

	SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
			SON_DETAILED, "NW SCAN RES recieved ");

	/* Get the pointer to the nmm start scan resp list */
	p_eutran_scan_res_list =
		p_nmm_start_scan_res->start_scan_res.p_eutran_scan_res_list;

	result = p_nmm_start_scan_res->start_scan_res.son_resp;
	if ((SON_SUCCESS == result || SON_PARTIAL_SUCCESS == result)
			&& SON_PNULL != p_eutran_scan_res_list)
	{
		no_of_nodes = p_eutran_scan_res_list->count;
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_DETAILED, "Number of nodes in the scan resp "
				"list received from SONNmm = [%u]", no_of_nodes);
		/* Iterate through the response list */
		for (count = 0; count < no_of_nodes; count++)
		{
			/* Get data from the nmm start scan resp list */
			SCAN_RESP_LIST_DEQUEUE(start_scan_res_eutran_data_t, p_eutran_scan_res_list, p_start_scan_res_data);

			/* Checking p_start_scan_res_data pointer to remove klocwork
			 * warning though this pointer will never be null*/
			if (SON_PNULL == p_start_scan_res_data)
			{
				SON_LOG(p_sonapps_gb_context->pci_log_enable,
						p_son_apps_facility_name, SON_WARNING,
						"Failed to read data successfully from NMM scan "
						"response list at node = %d. Selecting the PCI on the "
						"the basis of partial results read!", count + 1);
				break;
			}

			/* Enqueue the dequed data as it is needed by NMM */
			SCAN_RESP_LIST_ENQUEUE(p_eutran_scan_res_list, p_start_scan_res_data);

			/* Loop to read the PCIs in the cell list */
			for (cell_index = 0; cell_index <
					p_start_scan_res_data->scan_res_data.num_of_cells; cell_index++)
			{
				pci_id = p_start_scan_res_data->scan_res_data.cell[cell_index].
					pci_id;

				/* Set the flag at index = PCI, of in-use pci list array, to
				 * indicate that the PCI is in use */
				p_sonapps_gb_context->in_use_pci_list[pci_id] = SON_TRUE;
			}
		}

		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_DETAILED, "Scan resp list received from SONNmm has "
				"been freed.");
	}

	/* If the scan res list received from NMM was empty or in case of Failure,
	 * select a PCI from the list received from OAM */
	else
	{
        /* Spr 17753 Changes Start */
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
            SON_WARNING, "Scan res list received from NMM"
                         " is either empty or scan resp is failure ");
        /* Spr 17753 Changes End */
	}

	if (++p_sonapps_gb_context->num_nw_scan_rsp_rcvd_for_pci ==
			p_sonapps_gb_context->num_nw_scan_req_sent_for_pci)
	{
		/* All NW scan responses are received */

        /* Spr 17753 Changes Start */
        /* populate the list of pci based on nw scan result */
        p_pci_list = (son_used_pci_info_t *)son_mem_get(sizeof(son_used_pci_info_t)); 
        /* + coverity 98778 */
        if( SON_PNULL == p_pci_list )
        {
            return RELEASE_SON_BUFFER;
        }
        /* - coverity 98778 */
        SON_MEMSET(p_pci_list , 0, sizeof(son_used_pci_info_t));

        for (cell_index = 0; cell_index < SON_MAX_PCI; cell_index++)
        {
            if(p_sonapps_gb_context->in_use_pci_list[cell_index] == SON_TRUE)
            {
                p_pci_list-> 
                    pci_list_level_1[p_pci_list->num_pci_in_use_w_first_degree++] = cell_index;
            }
        }
        p_pci_list->served_cell_pci =  SONAPPS_INVALID_PCI;
    /* Spr 17753 Changes End */
		/* Select an unused PCI on the basis of PCI information received
		 * from NMM Scan results */
		unused_pci = sonapps_select_unused_pci(p_sonapps_gb_context,
                                            /* Spr 17753 Changes Start */
                                               p_pci_list);
                                            /* Spr 17753 Changes End */
		/* All PCIs provided by OAM are in use */
		if (unused_pci == SONAPPS_INVALID_PCI)
		{
			error_code = SON_ERR_UNUSED_PCI_NOT_FOUND;
		}
		else
		{
			error_code = SON_NO_ERROR;
		}

		/* Send SONAPPS_PCI_SELECTION_RES response to SON MgmtIfH */
		sonapps_pci_send_pci_selection_res(p_sonapps_gb_context, unused_pci,
				error_code, p_nmm_start_scan_res->transaction_id);

		/* Free the memory of PCI list received by OAM in PCI selection request */
		if (p_sonapps_gb_context->pci_list != SON_PNULL)
		{
			son_mem_free(p_sonapps_gb_context->pci_list);
			p_sonapps_gb_context->pci_list = SON_PNULL;
			p_sonapps_gb_context->pci_list_size = SON_NULL;
		}

		if (p_sonapps_gb_context->meas_bw_list != SON_PNULL)
		{
			son_mem_free(p_sonapps_gb_context->meas_bw_list);
			p_sonapps_gb_context->meas_bw_list = SON_PNULL;
			p_sonapps_gb_context->meas_bw_list_size = SON_NULL;
		}

		p_sonapps_gb_context->num_nw_scan_req_sent_for_pci = SON_NULL;
		p_sonapps_gb_context->num_nw_scan_rsp_rcvd_for_pci = SON_NULL;
		p_sonapps_gb_context->pci_selection_earfcn = SONAPPS_INVALID_VALUE;
		SON_MEMSET(p_sonapps_gb_context->in_use_pci_list, SON_FALSE,
				sizeof(p_sonapps_gb_context->in_use_pci_list));
		/* Update PCI state if aplicable*/
		if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_SEL_RESEL_IN_PROGRESS)
		{
			p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_ENABLED;

			SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
					SON_INFO, "### PCI_FSM: State changed to %s ###",
					SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);
		}

		/* Reset the transaction id for PCI in global context */
		p_sonapps_gb_context->transaction_id[SONAPPS_PCI] = SONAPPS_INVALID_VALUE;

		sonapps_print_global_context(p_sonapps_gb_context,
				p_sonapps_gb_context->pci_log_enable);
        /* Spr 17753 Changes Start */
        /* Free the memory allocated */
        son_mem_free(p_pci_list);
        /* Spr 17753 Changes End */
	}

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_scan_res_handler */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_reselection_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *                  message received from SONMgmtIfH in
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_reselection_req
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_u16 index                 = SON_NULL;
	son_bool_et reselection_req_valid = SON_FALSE;
	son_anr_pci_in_use_list_req_t pci_in_use_list_req = {0};
	sonapps_pci_reselection_req_t   *p_sonapps_pci_reselection_req =
		(sonapps_pci_reselection_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_DETAILED,
			"PCI RE-SELECTION REQ recieved ");

	if (p_sonapps_pci_reselection_req->pci_reselection_req.pci_list_size ==
			SON_NULL)
	{
                /*SPR 17777 +-*/
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_ERROR, "PCI Re-selection list empty");
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

		sonapps_pci_send_pci_reselection_res(p_sonapps_gb_context,
				SONAPPS_INVALID_PCI, SON_ERR_EMPTY_PCI_LIST,
				p_sonapps_pci_reselection_req->transaction_id,
				p_sonapps_pci_reselection_req->pci_reselection_req.src_cgi); /*SPR-12423-Start-End*/
	}
	else if (p_sonapps_pci_reselection_req->pci_reselection_req.pci_list_size >
			SON_MAX_PCI)
	{
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "Excess no of elements in PCI Re-selection "
				"list");

		sonapps_pci_send_pci_reselection_res(p_sonapps_gb_context,
				SONAPPS_INVALID_PCI, SON_ERR_INVALID_PARAMS,
				p_sonapps_pci_reselection_req->transaction_id,
				p_sonapps_pci_reselection_req->pci_reselection_req.src_cgi); /*SPR-12423-Start-End*/
	}
	else
	{
		/* Print the PCI list received from OAM */
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_DETAILED, "PCIs recieved from OAM :");

		for (index = 0; index < p_sonapps_pci_reselection_req->
				pci_reselection_req.pci_list_size; index++)
		{
            /* Spr 17753 Changes Start*/
            /* Code removed */ 
            /* Spr 17753 Changes End*/
			/*Changed to remove compilation warnings*/
			if (SON_MAX_PCI > p_sonapps_pci_reselection_req->pci_reselection_req.pci_list[index])
			{
				reselection_req_valid = SON_TRUE;
			}
		}

		/* If All the PCIs received from OAM are invalid */
		if (SON_FALSE == reselection_req_valid)
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_ERROR,
					"All PCIs received from OAM are invalid");

			sonapps_pci_send_pci_reselection_res(p_sonapps_gb_context,
					SONAPPS_INVALID_PCI, SON_ERR_INVALID_PARAMS,
					p_sonapps_pci_reselection_req->transaction_id,
					p_sonapps_pci_reselection_req->pci_reselection_req.src_cgi); /*SPR-12423-Start-End*/
		}
		else
		{
			if (SON_NULL == sonapps_find_band_id_for_earfcn(p_sonapps_pci_reselection_req->
						pci_reselection_req.earfcn, p_sonapps_gb_context->pci_log_enable))
			{
				/* EARFCN value provided by OAM is invalid */
				sonapps_pci_send_pci_reselection_res(p_sonapps_gb_context,
						SONAPPS_INVALID_PCI, SON_ERR_INVALID_EARFCN,
						p_sonapps_pci_reselection_req->transaction_id,
						p_sonapps_pci_reselection_req->pci_reselection_req.src_cgi); /*SPR-12423-Start-End*/
			}
			else
			{
				/* Populate SONANR_PCI_IN_USE_LIST_REQ structure */
				pci_in_use_list_req.transaction_id =
					p_sonapps_pci_reselection_req->transaction_id;
				pci_in_use_list_req.earfcn =
					p_sonapps_pci_reselection_req->pci_reselection_req.earfcn;
				pci_in_use_list_req.level =  p_sonapps_pci_reselection_req->pci_reselection_req.level;

				/*SPR-12423-Start*/
				SON_MEMCPY(&pci_in_use_list_req.src_cgi, &p_sonapps_pci_reselection_req->pci_reselection_req.src_cgi,
						sizeof(son_intra_rat_global_cell_id_t));
				/*SPR-12423-End*/

				/* Send SONANR_PCI_IN_USE_LIST_REQ to SONANR */
				son_create_send_buffer((son_u8 *)&pci_in_use_list_req,
						SON_APPS_MODULE_ID, SON_ANR_MODULE_ID,
						SONANR_PCI_IN_USE_LIST_REQ,
						sizeof(son_anr_pci_in_use_list_req_t));

				/* Update PCI List size in global context */
				p_sonapps_gb_context->pci_list_size =
					p_sonapps_pci_reselection_req->pci_reselection_req.pci_list_size;

				/* Get Memory */
				p_sonapps_gb_context->pci_list = son_mem_get(
						p_sonapps_gb_context->pci_list_size * sizeof(son_u16));

				if (SON_PNULL == p_sonapps_gb_context->pci_list)
				{
					SON_UT_TRACE_EXIT();
					/* Release the buffer memory as it is no longer needed. */
					return RELEASE_SON_BUFFER;
				}

				/* Update PCI List in global context */
				SON_MEMCPY(p_sonapps_gb_context->pci_list,
						p_sonapps_pci_reselection_req->pci_reselection_req.pci_list,
						p_sonapps_gb_context->pci_list_size * sizeof(son_u16));

				/* Update the PCI state to inidicate PCI re-selection in progress */
				p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_SEL_RESEL_IN_PROGRESS;

				SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
						SON_INFO, "### PCI_FSM: State changed to %s ###",
						SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);

				sonapps_print_global_context(p_sonapps_gb_context,
						p_sonapps_gb_context->pci_log_enable);
			}
		}
	}

	SON_UT_TRACE_EXIT();

	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_reselection_req */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_conflict_confusion_ind
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SONANR_PCI_CONFLICT_CONFUSION_IND message received from
 *                  SONANR in SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_conflict_confusion_ind
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	sonapps_pci_conflict_confusion_ind_t apps_pci_conf_ind;
	son_anr_pci_conflict_confusion_ind_t   *p_anr_pci_conf_ind =
		(son_anr_pci_conflict_confusion_ind_t *)p_msg;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_DETAILED,
			"PCI CONFLICT/CONFUSION IND recieved");
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	SON_MEMSET(&(apps_pci_conf_ind), 0, sizeof(apps_pci_conf_ind));

	/* Populate SONAPPS_PCI_CONFLICT_CONFUSION_IND message data */

	/*SPR-12596-Start*/

	SON_MEMCPY(&(apps_pci_conf_ind.src_cgi), &(p_anr_pci_conf_ind->src_cgi),
			sizeof(apps_pci_conf_ind.cgi_1));

	/*SPR-12596-End*/

	SON_MEMCPY(&(apps_pci_conf_ind.cgi_1), &(p_anr_pci_conf_ind->cgi_1),
			sizeof(apps_pci_conf_ind.cgi_1));
	SON_MEMCPY(&(apps_pci_conf_ind.cgi_2), &(p_anr_pci_conf_ind->cgi_2),
			sizeof(apps_pci_conf_ind.cgi_2));
	apps_pci_conf_ind.pci = p_anr_pci_conf_ind->pci;
	apps_pci_conf_ind.cause = p_anr_pci_conf_ind->cause;

	/* Send SONAPPS_PCI_CONFLICT_CONFUSION_IND to SONMgmtIfh */
	son_create_send_buffer((son_u8 *)&apps_pci_conf_ind, SON_APPS_MODULE_ID,
			SON_MIF_MODULE_ID, SONAPPS_PCI_CONFLICT_CONFUSION_IND,
			sizeof(sonapps_pci_conflict_confusion_ind_t));

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_conflict_confusion_ind */

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_in_use_list_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONANR_PCI_IN_USE_LIST_RES
 *                  message received from SONANR in SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_in_use_list_res
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_s16 unused_pci             = SONAPPS_INVALID_PCI;
	son_error_et error_code             = SON_NO_ERROR;
    /* Spr 17753 Changes Start*/
    son_u8  rand_index = SON_NULL;
    /* Spr 17753 Changes End*/

	son_anr_pci_in_use_list_res_t *p_pci_in_use_list_res =
		(son_anr_pci_in_use_list_res_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_LOG(p_sonapps_gb_context->pci_log_enable,
			p_son_apps_facility_name, SON_DETAILED,
			"PCI IN-USE LIST RES recieved ");

	if ((p_pci_in_use_list_res->result == SON_FAILURE) &&
			(p_pci_in_use_list_res->error_code == SON_ERR_ANR_IS_DISABLED))
	{
                /*SPR 17777 +-*/
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "ANR is disabled,"
				" sending failure to OAM with error code as SON_ERR_ANR_IS_DISABLED");
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
		unused_pci = SONAPPS_INVALID_PCI;
		error_code = SON_ERR_ANR_IS_DISABLED;
	}
	else if ((p_pci_in_use_list_res->result == SON_FAILURE) &&
			(p_pci_in_use_list_res->error_code == SON_ERR_EMPTY_PCI_LIST))
	{
        /* Spr 17753 Changes Start*/
		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_WARNING, "Empty in-use PCI list received from ANR."
                " Selecting the random PCI from list provided by OAM");

        /* Getting random index no by rand function */ 
        if(SON_NULL < p_sonapps_gb_context->pci_list_size)
        {
            rand_index = son_anr_get_random_val(SON_NULL,
                    p_sonapps_gb_context->pci_list_size);

        unused_pci = p_sonapps_gb_context->pci_list[rand_index];
        }
        /* Spr 17753 Changes End*/
		error_code = SON_NO_ERROR;
	}
	else
	{
		/* Select an unused PCI */
		unused_pci = sonapps_select_unused_pci(p_sonapps_gb_context,
        /* Spr 17753 Changes Start*/
            &p_pci_in_use_list_res->in_use_pci_list);
        /* Spr 17753 Changes End*/

		if (unused_pci == SONAPPS_INVALID_PCI)
		{
			error_code = SON_ERR_UNUSED_PCI_NOT_FOUND;
		}
        /* Spr 17753 Changes Start*/
        else if (unused_pci == SONAPPS_PCI_RESELECTION_NOT_REQUIRED)
        {
           error_code = SON_ERR_PCI_RESELECTION_NOT_REQUIRED;
           unused_pci = SONAPPS_INVALID_PCI;
        }
        /* Spr 17753 Changes End*/
		else
		{
			error_code = SON_NO_ERROR;
		}
	}

	/* Send SONAPPS_PCI_RESELECTION_RES to SON MgmtIfH */
	sonapps_pci_send_pci_reselection_res(p_sonapps_gb_context,
			unused_pci, error_code, p_pci_in_use_list_res->transaction_id,
			p_pci_in_use_list_res->src_cgi); /*SPR-12423-Start-End*/

	/* Free the memory of PCI list received by OAM in PCI selection request */
	if (p_sonapps_gb_context->pci_list != SON_PNULL)
	{
		son_mem_free(p_sonapps_gb_context->pci_list);
		p_sonapps_gb_context->pci_list = SON_PNULL;
	}

	p_sonapps_gb_context->pci_list_size = SON_NULL;

	/* Update PCI state if aplicable*/
	if (p_sonapps_gb_context->pci_fsm_state == SONAPPS_PCI_SEL_RESEL_IN_PROGRESS)
	{
		p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_ENABLED;

		SON_LOG(p_sonapps_gb_context->pci_log_enable, p_son_apps_facility_name,
				SON_INFO, "### PCI_FSM: State changed to %s ###",
				SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);
	}

	sonapps_print_global_context(p_sonapps_gb_context,
			p_sonapps_gb_context->pci_log_enable);

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonapps_pci_fsm_in_use_list_res */

/*********** FSM table for PCI feature *****************/

son_apps_pci_fsm_evt_handler_t
son_apps_pci_fsm_table[SONAPPS_PCI_NUM_OF_STATES][SONAPPS_PCI_NUM_OF_EVENTS] =
{
	/* State PCI_DISABLED*/
	{
		sonapps_pci_fsm_enable_disable_req,
		/*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONAPPS_EVENT_PCI_REG_DEREG_RES */
		sonapps_pci_fsm_selection_req_nack,
		/*SONAPPS_EVENT_PCI_SELECTION_REQ */
		sonapps_pci_fsm_reselection_req_nack,
		/*SONAPPS_EVENT_PCI_RESELECTION_REQ */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONANR_EVENT_PCI_CONFLICT_IND */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONANR_EVENT_PCI_IN_USE_LIST_RES */
	},

	/* State PCI_ENABLED */
	{
		sonapps_pci_fsm_enable_disable_req,
		/*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONAPPS_EVENT_PCI_REG_DEREG_RES */
		sonapps_pci_fsm_selection_req,
		/*SONAPPS_EVENT_PCI_SELECTION_REQ */
		sonapps_pci_fsm_reselection_req,
		/*SONAPPS_EVENT_PCI_RESELECTION_REQ */
		sonapps_pci_fsm_conflict_confusion_ind,
		/*SONANR_EVENT_PCI_CONFLICT_IND */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONANR_EVENT_PCI_IN_USE_LIST_RES */
	},

	/* State PCI_DISABLE_IN_PROGRESS */
	{
		sonapps_pci_fsm_enable_disable_req,
		/*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
		sonapps_pci_fsm_reg_dereg_res,
		/*SONAPPS_EVENT_PCI_REG_DEREG_RES */
		sonapps_pci_fsm_selection_req_nack,
		/*SONAPPS_EVENT_PCI_SELECTION_REQ */
		sonapps_pci_fsm_reselection_req_nack,
		/*SONAPPS_EVENT_PCI_RESELECTION_REQ */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONANR_EVENT_PCI_CONFLICT_IND */
		sonapps_pci_fsm_in_use_list_res,
		/*SONANR_EVENT_PCI_IN_USE_LIST_RES */
	},

	/* State PCI_ENABLE_IN_PROGRESS */
	{
		sonapps_pci_fsm_enable_disable_req,
		/*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
		sonapps_pci_fsm_reg_dereg_res,
		/*SONAPPS_EVENT_PCI_REG_DEREG_RES */
		sonapps_pci_fsm_selection_req_nack,
		/*SONAPPS_EVENT_PCI_SELECTION_REQ */
		sonapps_pci_fsm_reselection_req_nack,
		/*SONAPPS_EVENT_PCI_RESELECTION_REQ */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONANR_EVENT_PCI_CONFLICT_IND */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONANR_EVENT_PCI_IN_USE_LIST_RES */
	},

	/* State PCI_SEL_RESEL_IN_PROGRESS */
	{
		sonapps_pci_fsm_enable_disable_req,
		/*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
		sonapps_pci_fsm_invalid_event_handler,
		/*SONAPPS_EVENT_PCI_REG_DEREG_RES */
		sonapps_pci_fsm_selection_req_nack,
		/*SONAPPS_EVENT_PCI_SELECTION_REQ */
		sonapps_pci_fsm_reselection_req_nack,
		/*SONAPPS_EVENT_PCI_RESELECTION_REQ */
		sonapps_pci_fsm_conflict_confusion_ind,
		/*SONANR_EVENT_PCI_CONFLICT_IND */
		sonapps_pci_fsm_in_use_list_res,
		/*SONANR_EVENT_PCI_IN_USE_LIST_RES */
	}
};

/*****************************************************************************
 * Function Name  : sonapps_pci_fsm_event_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to the global context of
 *                  SONAPPS
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for messages received for
 *                  PCI feature in SONAPP_STATE_ACTIVE state and call the
 *                  respective function handlers for various messages received
 *                  depending upon the current state of PCI feature.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_pci_fsm_event_handler
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr,
 son_apps_gb_context_t   *p_sonapps_gb_context
 )
{
	son_buf_retain_status_et buf_retain_status = RELEASE_SON_BUFFER;
	son_u8 idx = APPS_IDX_NOT_AVAILABLE;

	SON_UT_TRACE_ENTER();

	idx = calculate_apps_msg_index(p_cspl_hdr->from, p_cspl_hdr->api,
			p_sonapps_gb_context);

	if (APPS_IDX_NOT_AVAILABLE != idx)
	{
		if ((p_sonapps_gb_context->pci_fsm_state < SONAPPS_PCI_NUM_OF_STATES) &&
				(idx < SONAPPS_PCI_NUM_OF_EVENTS))
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_BRIEF,
					"Msg received in PCI state = [%s] ",
					SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state]);

			/* Invoke appropriate event handler */
			buf_retain_status =
				(*son_apps_pci_fsm_table[p_sonapps_gb_context->pci_fsm_state][idx])
				(p_msg, p_cspl_hdr, p_sonapps_gb_context);
		}
		else
		{
			SON_LOG(p_sonapps_gb_context->pci_log_enable,
					p_son_apps_facility_name, SON_ERROR,
					"Received invalid event [%u] or Msg is received in "
					"invalid state %d", idx,p_sonapps_gb_context->pci_fsm_state);
                                               /* Coverity ID 86933 Ends */
		}
	}
	else
	{
		SON_LOG(p_sonapps_gb_context->pci_log_enable,
				p_son_apps_facility_name, SON_ERROR,
				"Received invalid event [%u]", idx);
	}

	SON_UT_TRACE_EXIT();
	return buf_retain_status;
} /* sonapps_pci_fsm_event_handler */

/* REL1.1 CHANGES : ENDS */
