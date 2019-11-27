/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_apps_common_intf.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: This file contains the definitions common for
*                   SONApps and its features
*
****************************************************************************
* Revision Details
* ----------------
*
* SPR-612 Coverity Fix Starts
* $Log: son_apps_common_intf.c $
* SPR-612 Coverity Fix Ends
* Revision 0.1  2012/17/05 14:50:00  Sonal Jain
*
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_apps_common_intf.h>
#include <son_nmm_apps_intf.h>
#include <son_apps_pci.h>

/****************************************************************************
* Global Variables
****************************************************************************/
extern const son_8 *p_son_apps_facility_name;
extern son_s8 *SON_APPS_FSM_STATES_NAMES[];

/* Band ID to EARFCN mapping table : Band ID, Min EARFCN value, Max EARFCN
 * value */
sonapps_band_earfcn_map_table_t band_earfcn_map_table[SONAPPS_MAX_NO_VALID_BAND_ID] =
{
    {1, 0, 599},
    {2, 600, 1199},
    {3, 1200, 1949},
    {4, 1950, 2399},
    {5, 2400, 2649},
    {6, 2650, 2749},
    {7, 2750, 3449},
    {8, 3450, 3799},
    {9, 3800, 4149},
    {10, 4150, 4749},
    {11, 4750, 4959},
    {12, 5010, 5179},
    {13, 5180, 5279},
    {14, 5280, 5379},
    {17, 5730, 5849},
    {18, 5850, 5999},
    {19, 6000, 6149},
    {20, 6150, 6449},
    {21, 6450, 6599},
    {22, 6600, 7399},
    {23, 7500, 7699},
    {24, 7700, 8039},
    {25, 8040, 8689},
    {26, 8690, 9039},
    {33, 36000, 36199},
    {34, 36200, 36349},
    {35, 36350, 36949},
    {36, 36950, 37549},
    {37, 37550, 37749},
    {38, 37750, 38249},
    {39, 38250, 38649},
    {40, 38650, 39649},
    {41, 39650, 41589},
    {42, 41590, 43589},
    {43, 43590, 45589}
};

/*****************************************************************************
 * Function Name  : sonapps_print_global_context
 * Inputs         : p_sonapps_gb_context - Pointer to global context structure
 *                : log_enable_disable   - Logging status(On/Off) of the calling
 *                  feature
 * Outputs:       : None
 * Returns        : None
 * Description    : This function prints the SONAPPS global context contents
 ****************************************************************************/
son_void_t
sonapps_print_global_context
(
    son_apps_gb_context_t   *p_sonapps_gb_context,
    son_oam_log_on_off_et log_enable_disable
)
{
    son_u16 index = SON_NULL;

    SON_UT_TRACE_ENTER();

    SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_DETAILED,
        "\n************************SONAPPS CONTEXT*********************\n");

    SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_BRIEF,
        "APPS: apps_log_enable = %d g_sonapps_state = %s",
        " PCI: pci_log_enable = %d pci_fsm_state = %s "
        "transaction_id[PCI] = %u pci_list_size = %u",
        p_sonapps_gb_context->apps_log_enable,
        SON_APPS_FSM_STATES_NAMES[p_sonapps_gb_context->g_sonapps_state],
        p_sonapps_gb_context->pci_log_enable,
        SONAPPS_PCI_FSM_STATES_NAMES[p_sonapps_gb_context->pci_fsm_state],
        p_sonapps_gb_context->transaction_id[SONAPPS_PCI],
        p_sonapps_gb_context->pci_list_size);

    if (p_sonapps_gb_context->pci_list == SON_PNULL)
    {
        SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_DETAILED,
            "pci_list = EMPTY");
    }
    else
    {
        for (index = SON_NULL; index < p_sonapps_gb_context->pci_list_size;
             index++)
        {
            SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_DETAILED,
                "pci_list[%d] = %u", index,
                p_sonapps_gb_context->pci_list[index]);
        }
    }

    SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_DETAILED,
        "ACF: acf_log_enable = %d transaction_id[OAM] = %u "
        "transaction_id[RRM] = %u "
        "meas_bandwidth = %u earfcn[OAM] = %u earfcn[RRM] = %u "
        "acf_selection_progress_status[OAM] = %d"
        "acf_selection_progress_status[RRM] = %d"
        "*************************************************************\n",
        p_sonapps_gb_context->acf_log_enable,
        p_sonapps_gb_context->transaction_id[SONAPPS_ACF_OAM],
        p_sonapps_gb_context->transaction_id[SONAPPS_ACF_APPS],
        p_sonapps_gb_context->meas_bandwidth,
        p_sonapps_gb_context->earfcn[ACF_SELECTION_REQ],
        p_sonapps_gb_context->earfcn[ACF_AND_POWER_REQ],
        p_sonapps_gb_context->acf_selection_progress_status[ACF_SELECTION_REQ],
        p_sonapps_gb_context->acf_selection_progress_status[ACF_AND_POWER_REQ]);

    SON_UT_TRACE_EXIT();
} /* sonapps_print_global_context */

/*****************************************************************************
 * Function Name  : sonapps_find_band_id_for_earfcn
 * Inputs         : earfcn - EARFCN for which the Band Id is to be found
 *                : log_enable_disable - Logging status(On/Off) of the calling
 *                  feature
 * Outputs:       : None
 * Returns        : Band Id
 * Description    : This function is used to find the band id corresponding to
 *                  the earfcn provided
 ****************************************************************************/
son_u8
sonapps_find_band_id_for_earfcn
(
    son_u32 earfcn,
    son_oam_log_on_off_et log_enable_disable
)
{
    son_u8 first = SON_NULL;
    son_u8 last  = SON_NULL;
    son_u8 mid   = SON_NULL;

    SON_UT_TRACE_ENTER();

    last = SONAPPS_MAX_NO_VALID_BAND_ID - 1;

    while (first <= last)
    {
        mid = (first + last) / 2;

        /* If EARFCN lies within the range */
        if ((band_earfcn_map_table[mid].max_earfcn_val >= earfcn) &&
            (band_earfcn_map_table[mid].min_earfcn_val <= earfcn))
        {
            SON_UT_TRACE_EXIT();
            /* Return the band Id corresponding to the EARFCN */
            SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_DETAILED,
                "EARFCN : BAND ID  :::  %d : %d", earfcn,
                band_earfcn_map_table[mid].band_id);

            return band_earfcn_map_table[mid].band_id;
        }
        /* If EARFCN lies in the higher ranges */
        else if (band_earfcn_map_table[mid].max_earfcn_val < earfcn)
        {
            first = mid + 1;
        }
        /* If EARFCN lies in the lower ranges */
        else
        {
            last = mid - 1;
        }
    }

    SON_LOG(log_enable_disable, p_son_apps_facility_name, SON_WARNING,
        "Band Id not found. "
        "Invalid EARFCN value [%u] received", earfcn);

    SON_UT_TRACE_EXIT();
    /* EARFCN did not lie in the valid ranges, return 0 */
    return SON_NULL;
} /* sonapps_find_band_id_for_earfcn */

/*****************************************************************************
 * Function Name  : sonapps_prepare_and_send_nw_scan_req
 * Inputs         : p_sonapps_gb_context - Pointer to global context structure
 *                  p_msg - Pointer to the message
 *                  feature_id - Feature identifier
 *                  p_cspl_hdr - Pointer to CSPL header
 * Outputs        : None
 * Returns        : Error code
 * Description    : This function is used to prepare network scan request
 *                  and send SONNMM_START_SCAN_REQ message to NMM.
 ****************************************************************************/
son_error_et
sonapps_prepare_and_send_nw_scan_req
(
    son_apps_gb_context_t   *p_sonapps_gb_context,
    son_void_t              *p_msg,
    son_feature_id_et feature_id,
    STACKAPIHDR             *p_cspl_hdr
)
{
    son_error_et ret_val = SON_NO_ERROR;
    son_u8 index = SON_NULL;
    son_u16 num_earfcn = SON_NULL;
    sonapps_nw_scan_req_t nw_scan_req;

    nw_scan_req.eutran_scan_req.p_start_scan_req = SON_PNULL;

    /* Getting memory for start_scan_req structure */
    nw_scan_req.eutran_scan_req.p_start_scan_req = son_mem_get(sizeof(start_scan_req_t));

    if (SON_PNULL == nw_scan_req.eutran_scan_req.p_start_scan_req)
    {
        /* Memory failure */
        ret_val = SON_ERR_SYS_MEM_ALLOC_FAILURE;
    }
    else
    {
        SON_MEMSET(nw_scan_req.eutran_scan_req.p_start_scan_req, SON_NULL,
            sizeof(start_scan_req_t));

        /* Populating default values in network scan request structure */
        nw_scan_req.eutran_scan_req.p_start_scan_req->rat_type = SON_EUTRAN_CELL;
        nw_scan_req.eutran_scan_req.p_start_scan_req->meas_period = SONAPPS_MEAS_PERIOD;
        nw_scan_req.eutran_scan_req.p_start_scan_req->is_meas_bandwidth_valid = SON_FALSE;
        nw_scan_req.eutran_scan_req.p_start_scan_req->meas_bandwidth = SON_NULL;
        nw_scan_req.eutran_scan_req.p_start_scan_req->retry_count = SONAPPS_RETRY_COUNT;

        /* Populate rest of the network scan request structure on basis
         *  of feature id */
        if (SON_ACF_FEATURE == feature_id)
        {
            /* Typecast message to carrier frequency selection request
             *  structure */
            sonapps_carrier_freq_selection_req_t *p_carrier_freq_selection_req =
                (sonapps_carrier_freq_selection_req_t *)p_msg;

            /* Find frequency band indicator corresponding to EARFCN. Here
             *  condition is that OAM provided EARFCN list for carrier frequency
             *  selection are of same frequency band. So finding band
             *  indicator for first earfcn only */
            nw_scan_req.eutran_scan_req.p_start_scan_req->freq_band_id =
                sonapps_find_band_id_for_earfcn(p_carrier_freq_selection_req->
                    carrier_freq_selection_req.earfcn[0],
                    p_sonapps_gb_context->acf_log_enable);

            /* Check if frequency band indicator is received for EARFCN */
            if (SON_NULL != nw_scan_req.eutran_scan_req.p_start_scan_req->freq_band_id)
            {
                /* Assign the transaction id of carrier frequency selection
                 * request */
                nw_scan_req.eutran_scan_req.transaction_id = p_carrier_freq_selection_req->transaction_id;

                /* Assign originator module id of carrier frequency selection
                 *  request */
                if (SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ == p_cspl_hdr->api)
                {
                    /* When carrier frequency selection request is initiated
                     *  from RRM then originating module id will set to RRM */
                    nw_scan_req.eutran_scan_req.originator_module_id = SON_APPS_MODULE_ID;
                }
                else
                {
                    nw_scan_req.eutran_scan_req.originator_module_id = SON_MIF_MODULE_ID;
                }

                num_earfcn = p_carrier_freq_selection_req->
                             carrier_freq_selection_req.num_earfcn;

                /* If number of earfcn received from OAM is more than
                 *  SON_MAX_NO_EARFCN (100) then set it to the maximum
                 *  supported value */
                if (num_earfcn > SON_MAX_NO_EARFCN)
                {
                    num_earfcn = SON_MAX_NO_EARFCN;
                }
                /* Assign number of EARFCN */
                nw_scan_req.eutran_scan_req.p_start_scan_req->num_earfcn = num_earfcn;
                for (index =0;index < num_earfcn;index ++)
                {
                    /*CSR NO:00070093 changes start here */

                    /* Copy EARFCN list in network scan request structure */
                    nw_scan_req.eutran_scan_req.p_start_scan_req->earfcn_pci_info[index].earfcn=
                        p_carrier_freq_selection_req->carrier_freq_selection_req.
                        earfcn[index];
                }
                /* Populating measurement BW for each EARFCN */
                for (index = 0; index < num_earfcn; index++)
                {
                    nw_scan_req.eutran_scan_req.p_start_scan_req->earfcn_pci_info[index].meas_bandwidth_per_earfcn =
                        p_carrier_freq_selection_req->
                        carrier_freq_selection_req.meas_bandwidth;
                    /* Start: CSR 00053250 */
                    nw_scan_req.eutran_scan_req.
                        p_start_scan_req->earfcn_pci_info[index].num_pci = SON_NULL;
                    SON_MEMSET(nw_scan_req.eutran_scan_req.p_start_scan_req->earfcn_pci_info[index].pci, 
                            SON_NULL,sizeof(nw_scan_req.eutran_scan_req.
                                p_start_scan_req->earfcn_pci_info[index].pci));
                    /*End: CSR 00053250 end */
                    /*CSR NO:00070093 changes ends here */

                }
            }
            else
            {
                /* Band id is not found for earfcn. Invalid earfcn is received
                 *  from OAM */
                ret_val = SON_ERR_INVALID_EARFCN;

                /* Free Memory */
                son_mem_free(nw_scan_req.eutran_scan_req.p_start_scan_req);
                nw_scan_req.eutran_scan_req.p_start_scan_req = SON_PNULL;
            }
        }
        else
        {
            SON_LOG(p_sonapps_gb_context->pci_log_enable,
                p_son_apps_facility_name, SON_DETAILED,
                "Function called for"
                "PCI feature");

            /* Populating network scan request structure for PCI */

            /* Find frequency band indicator corresponding to EARFCN */
            nw_scan_req.eutran_scan_req.p_start_scan_req->freq_band_id =
                sonapps_find_band_id_for_earfcn(
                    p_sonapps_gb_context->pci_selection_earfcn,
                    p_sonapps_gb_context->pci_log_enable);
            /* Check if frequency band indicator is received for EARFCN */
            if (SON_NULL != nw_scan_req.eutran_scan_req.p_start_scan_req->freq_band_id)
            {
                /* Assign the transaction id corresponding to the PCI feature
                 *  index */
                nw_scan_req.eutran_scan_req.transaction_id =
                    p_sonapps_gb_context->transaction_id[SONAPPS_PCI];

                /* Populate measurement bandwidth */

                /* If measurement BW has not been populated yet in the global
                 *  context by ACF feature, scan on all the six allowed
                 *  measurement BW values, i.e. 6, 15, 25, 50, 75, 100 */

                /* Fill Measurement BW */
                if (p_sonapps_gb_context->num_nw_scan_req_sent_for_pci <
                    p_sonapps_gb_context->meas_bw_list_size)
                {
                    /*CSR NO:00070093 changes start here */

                    nw_scan_req.eutran_scan_req.p_start_scan_req->earfcn_pci_info[0].meas_bandwidth_per_earfcn =
                        p_sonapps_gb_context->meas_bw_list[p_sonapps_gb_context->num_nw_scan_req_sent_for_pci];

                    nw_scan_req.eutran_scan_req.p_start_scan_req->num_earfcn = SONAPPS_NO_EARFCN_1;
                    nw_scan_req.eutran_scan_req.p_start_scan_req->earfcn_pci_info[0].earfcn =
                        p_sonapps_gb_context->pci_selection_earfcn;
                    /*CSR NO:00070093 changes end here */
                }
                else
                {
                    SON_LOG(p_sonapps_gb_context->pci_log_enable,
                        p_son_apps_facility_name, SON_DETAILED,
                        "Function called for"
                        "PCI feature more than intended times. Returning failure");
                    ret_val = SON_ERR_UNEXPECTED_MSG;
                    /* Free Memory */
                    son_mem_free(nw_scan_req.eutran_scan_req.p_start_scan_req);
                    nw_scan_req.eutran_scan_req.p_start_scan_req = SON_PNULL;
                }
            }
            else
            {
                /* Band id is not found for earfcn. Invalid earfcn is received
                 *  from OAM */
                ret_val = SON_ERR_INVALID_EARFCN;

                /* Free Memory */
                son_mem_free(nw_scan_req.eutran_scan_req.p_start_scan_req);
                nw_scan_req.eutran_scan_req.p_start_scan_req = SON_PNULL;
            }

            if (SON_NO_ERROR == ret_val)
            {
                p_sonapps_gb_context->num_nw_scan_req_sent_for_pci++;
            }
        }

        /* Send SONNMM_START_SCAN_REQ to NMM only when there is no error
         *  i.e. ret_val = SON_NO_ERROR*/
        if (SON_NO_ERROR == ret_val)
        {
            son_create_send_buffer((son_u8 *)&nw_scan_req,
                SON_APPS_MODULE_ID, SON_NMM_MODULE_ID, SONNMM_START_SCAN_REQ,
                sizeof(sonapps_nw_scan_req_t));
        }
    }

    return ret_val;
} /* sonapps_prepare_and_send_nw_scan_req */
