/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_ue_ho_utils.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains utility functions used for measurements 
 *                  and HO.
 *        
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   15 Mar,2013      gur11059    Initial        Other main file had grown large
 *    2 July,2014     gur25767   10498           Inter-freq Daho Support
 ******************************************************************************/

/*****************************************************************************
 * Project includes 
 ******************************************************************************/
#include "rrm_defines.h"
#include "rrm_utils.h"
#include "rrm_ue_context.h"
#include "rrm_ue_ho_utils.h"
#include "rrm_cell_mgr.h"
#include "rrm_ue_mgr.h"


/* SPR 15336 Fix Start */
/* SPR 15674 Start */
/****************************************************************************
 * Function Name  : rrm_validate_and_get_geran_daho_pci
 * Inputs         : rrm_ue_context_t *p_ue_ctxt, U8 *dtm_ho_supported
 * Outputs        : Geran DAHO PCI
 * Returns        : geran DAHO PC
 * Description    : Validates and returns geran DAHO PCI
 ****************************************************************************/
phy_cell_id_info_t
rrm_validate_and_get_geran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt,
        U8               *p_dtm_ho_supported
        )
    /* SPR 15336 Fix End */
{
    /* SPR 15336 Fix Start */
    /* code removed */
    U16 carrier_freq =RRM_ZERO;
    /* SPR 15336 Fix End */
    /* SPR 16406 8SEP start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, carrier_freq, RRM_ZERO, RRM_ZERO, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 16406 8SEP end */
    rrm_ho_params_t*                p_ho_params = &(p_ue_ctxt->ho_params);
    U16                             daho_pci    = INVALID_PHYSICAL_CELL_ID;
    /* Start :SPR 1020 */
    /* Deleted the code related to FGI bit 9 check for GERAN */
    /* End :SPR 1020 */
    rrm_bool_et                     ret_car_freq_supported = RRM_TRUE;
    rrm_daho_cell_config_info_t*    p_daho_cfg =
        rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);

    RRM_UT_TRACE_ENTER();
    /*CCO Fix-21-feb*/
    if(p_daho_cfg != RRM_PNULL)
    {
        /*daho clean up(condition added for presence of daho cell)*/
        if(p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT)
        {
            daho_pci = p_daho_cfg->geran_daho_cell_config_info.pci;
            /* SPR 15336 Fix Start */
            /* Code removed */
            /* SPR 15336 Fix End */

            carrier_freq = p_daho_cfg->geran_daho_cell_config_info.bcch_arfcn;
            /* SPR 15336 Fix Start */
            *p_dtm_ho_supported = p_daho_cfg->geran_daho_cell_config_info.dtm_ho_supported;
            /* SPR 15336 Fix End */
            ret_car_freq_supported = rrm_ue_is_ue_supported_geran_frequency (
                    carrier_freq,
                    p_ue_ctxt);
            /* SPR 15336 Fix Start */
            /* Code removed */
            /* SPR 15336 Fix End */
            /* Start :SPR 1020 */
            if((ret_car_freq_supported == RRM_FALSE))
            {

                RRM_TRACE (
                        g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "GERAN DAHO Freq:[%d] is not supported by [UE:%d]",
                        carrier_freq,
                        p_ue_ctxt->ue_index);
                /* SPR_13721_fix : start */
                return phy_cell_id_info;
                /* SPR_13721_fix : end */
            }
            /* SPR_13721_fix : start */
            if(RRM_FAILURE == rrm_uem_configure_geran_srvcc(p_ue_ctxt,
                        p_ue_ctxt->srvcc_config_info.ue_srvcc_criteria,
                        /*Cov 71239_71238_71237 +*/
                        (rrm_bool_et)p_daho_cfg->geran_daho_cell_config_info.dtm_ho_supported,
                        (rrm_bool_et)p_daho_cfg->geran_daho_cell_config_info.dtm_supported,
                        (rrm_bool_et)p_daho_cfg->geran_daho_cell_config_info.voip_capable))
                         /*Cov 71239_71238_71237 -*/
            {
                /* Select another Target Cell */
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "DAHO GERAN cell with pci = %d cannot be selected"
                        "Need to check next candidate cell for HO",
                        p_daho_cfg->geran_daho_cell_config_info.pci);
            }
            else
            {
                phy_cell_id_info.phy_cell_id = daho_pci;
                phy_cell_id_info.carr_earfcn = carrier_freq;    
                /* End :SPR 1020 */

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "DAHO GERAN cell with pci = %d selected for HO", daho_pci);

                /* return Blind cell id */
                RRM_TRACE (g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "Trying HO or redirection to GERAN Blind cell with PCI [%d]",daho_pci);
                /* SPR 16406 Start */
                p_ho_params->geran_known_cell_selection_list.bitmask |=\
                                                       RRM_UE_BLIND_CELL_HO_TRIGGERED;
                /* SPR 16406 End */
                return phy_cell_id_info;
            }
            /* SPR_13721_fix : end */
            /* return Blind cell id */
            RRM_TRACE (
                    g_uem_log_on_off,
                    p_rrm_uem_facility_name,
                    RRM_WARNING,
                    "Trying HO to GERAN Blind cell with PCI [%u]",daho_pci);
                /* SPR 16406 Start */
                p_ho_params->geran_known_cell_selection_list.bitmask |=\
                                                       RRM_UE_BLIND_CELL_HO_TRIGGERED;
                /* SPR 16406 End */

        }
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
}
/* SPR 15674 End */


/****************************************************************************
 * Function Name  : rrm_validate_and_get_utran_daho_pci
 * Inputs         : rrm_ue_context_t *p_ue_ctxt
 * Outputs        : Utran DAHO PSC
 * Returns        : Utran DAHO PSC
 * Description    : Validates and returns Utran DAHO PSC
 ****************************************************************************/
/* SPR 15674 Start */
    phy_cell_id_info_t
rrm_validate_and_get_utran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt
        )
    /* SPR 15674 End  */
{
    rrm_daho_cell_config_info_t*    p_daho_cfg =
        rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);
    U16 carrier_freq =RRM_ZERO;
    /* SPR 22731 Fix Start */
    rrm_bool_et  ret_car_freq_supported = RRM_FALSE;
    /* SPR 22731 Fix End */
    U16  daho_pci            = INVALID_PHYSICAL_CELL_ID;
    /*CA HARDENING 6.3.0 CHANGES */
    U32                             feature_group_indicators = (U32)-RRM_ONE;
    rrm_ho_params_t*    p_ho_params         = &(p_ue_ctxt->ho_params);
    rrm_bool_et                     fgi_supported = RRM_TRUE;
    /* SPR 15674 Start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t      phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_ZERO, RRM_ZERO, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 15674 End */
    /*SPR_15241_Start*/
    rrm_bool_et                     ue_supported_voip = RRM_FALSE;
    U32    serv_dl_earfcn = rrm_cellm_get_dl_earfcn(p_ue_ctxt->cell_index);
    /* SPR_15241_End */
    /* SPR 22731 Fix Start */
    U8 utran_fdd_count = RRM_ZERO;
    U8 utran_tdd_count = RRM_ZERO;
    rrm_cell_context_t *p_cell_ctx = RRM_PNULL;
    p_cell_ctx = rrm_cellm_get_cell_context(p_ue_ctxt->cell_index);
    /* CID 147596 */
    if(p_cell_ctx == RRM_PNULL)
    {
        RRM_UT_TRACE_EXIT();
        return phy_cell_id_info;
    }
    /* SPR 22731 Fix End */

    RRM_UT_TRACE_ENTER();
    if(p_daho_cfg != RRM_PNULL)
    {
        /*Get scrambling code*/
        /*daho pci in this func is actually PSC configured from UTRAN*/
        /*daho clean up(condition added for presence of daho cell)*/
        if(p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT)
        {
            daho_pci = p_daho_cfg->utran_daho_cell_config_info.pcpich_scrambling_code;
            /*CCO Fix-21-feb*/
            if(RRM_MAX_PSC < daho_pci)
            {
                RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_ERROR,
                        "Incorrect DAHO PSC [%u] configured",daho_pci);
                RRM_UT_TRACE_EXIT();
    /* SPR 15674 Start */
                return phy_cell_id_info;
            }

            carrier_freq =  p_daho_cfg->utran_daho_cell_config_info.uarfcndl;
            /* SPR 22731 Fix Start */
            for(utran_fdd_count = RRM_ZERO;utran_fdd_count < p_cell_ctx->ran_info.mobility_params.
                    idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                    irat_eutran_to_utran_fdd_list.num_irat_eutran_to_utran_fdd_carriers; utran_fdd_count++)
            {
                if (carrier_freq == p_cell_ctx->ran_info.mobility_params.
                        idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                        irat_eutran_to_utran_fdd_list.irat_eutran_to_utran_fdd_carriers[utran_fdd_count].utra_carrier_arfcn)
                {
                    ret_car_freq_supported = rrm_ue_is_ue_supported_utra_fdd_frequency (
                    carrier_freq,
                    p_ue_ctxt);
                    if (RRM_FALSE == ret_car_freq_supported)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                "UTRA FDD Carrier Freq = %d, is not supported",carrier_freq);
                        break;
                    }
                }
            }
            if (utran_fdd_count == p_cell_ctx->ran_info.mobility_params.idle_mode_mobility_params.
                    idle_mode_mobility_irat_utra_params.irat_eutran_to_utran_fdd_list.num_irat_eutran_to_utran_fdd_carriers)
            {
                for(utran_tdd_count = RRM_ZERO;utran_tdd_count < p_cell_ctx->ran_info.mobility_params.
                        idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                        irat_eutran_to_utran_tdd_list.num_irat_eutran_to_utran_tdd_carriers; utran_tdd_count++)
                {
                    if (carrier_freq == p_cell_ctx->ran_info.mobility_params.
                            idle_mode_mobility_params.idle_mode_mobility_irat_utra_params.
                            irat_eutran_to_utran_tdd_list.irat_eutran_to_utran_tdd_carriers[utran_tdd_count].utra_carrier_arfcn)
                    { 
                        ret_car_freq_supported = rrm_ue_is_ue_supported_utra_tdd_frequency (
                                carrier_freq,
                                p_ue_ctxt);
                        if (RRM_FALSE == ret_car_freq_supported)
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                    "UTRA TDD Carrier Freq = %d, is not supported",carrier_freq);
                            break;
                        }
                    }
                }
            }
            /* SPR 22731 Fix End */

            if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
                    p_ue_ctxt->ue_capability_params.rrm_eutra_radio_capability_info.
                    eutra_radio_capability_info_def.bitmask)
            {
                feature_group_indicators = p_ue_ctxt->ue_capability_params.
                    rrm_eutra_radio_capability_info.
                    eutra_radio_capability_info_def.feature_group_indicators;
                if(!(feature_group_indicators &
                            (RRM_ONE <<(RRM_THIRTY_TWO - RRM_EIGHT))))
                {
                    /* FGI doesn't support for UTRAN HO*/
                    fgi_supported = RRM_FALSE;
                }
            }
            if((ret_car_freq_supported == RRM_FALSE) || \
                    (fgi_supported == RRM_FALSE))
            {
                RRM_TRACE (
                        g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "UTRAN DAHO Freq:[%d] or FGI bit:[%d] is not"
                        "supported by [UE:%d]",
                        carrier_freq,
                        RRM_EIGHT,
                        p_ue_ctxt->ue_index);
                RRM_UT_TRACE_EXIT();
                return phy_cell_id_info;
            }
            /*SPR_15241_Start*/
            if(RRM_TRUE == rrm_check_ue_capability_for_voip(p_ue_ctxt,serv_dl_earfcn))
            {
                ue_supported_voip = RRM_TRUE;
            } 
            /*SPR_15241_End*/

            /* SRVCC SUPPORT START */
            if(RRM_FAILURE == rrm_uem_configure_utran_srvcc(p_ue_ctxt, 
                        p_ue_ctxt->srvcc_config_info.ue_srvcc_criteria,
                        p_daho_cfg->utran_daho_cell_config_info.ps_ho_supported,
                        p_daho_cfg->utran_daho_cell_config_info.voip_capable,
                        /*SPR_15241_Start*/
                        ue_supported_voip))
                /*SPR_15241_End*/
            {
                /* Select another Target Cell */
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "DAHO UTRAN cell with pci = %d cannot be selected"
                        "Need to check next candidate cell for HO",
                        p_daho_cfg->utran_daho_cell_config_info.pcpich_scrambling_code);
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "DAHO UTRAN cell with pci = %d selected for HO", daho_pci);

                /* return Blind cell id */
                RRM_TRACE (
                        g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "Trying HO or redirection to UTRAN Blind cell with PSC [%u]",daho_pci);
                /* SPR 16406 Start */
                p_ho_params->utra_known_cell_selection_list.bitmask |=\
                                                      RRM_UE_BLIND_CELL_HO_TRIGGERED;
                /* SPR 16406 End */
                phy_cell_id_info.phy_cell_id = daho_pci;
                phy_cell_id_info.carr_earfcn = carrier_freq;
                return phy_cell_id_info;
            }
            /* SRVCC SUPPORT END */
        }
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
    /* SPR 15674 End  */
}

/****************************************************************************
 * Function Name  : rrm_validate_and_get_eutran_daho_pci
 * Inputs         : rrm_ue_context_t *p_ue_ctxt
 * Outputs        : eUtran DAHO PCI
 * Returns        : eUtran DAHO PCI
 * Description    : Validates and returns eUtran DAHO PCI
 ****************************************************************************/
                /* SPR 15674 Start */
    phy_cell_id_info_t
                /* SPR 15674 End */
rrm_validate_and_get_eutran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt
        )
{
    /*SPR 17852 Start*/
    rf_params_t*  rf_parameters = RRM_PNULL;
    lte_ncl_t *p_ncl_params = rrm_cellm_get_lte_ncl_info(p_ue_ctxt->cell_index);
    /*SPR 17852 End*/
    /* SPR 10498 Fix Start */
    rrm_daho_cell_config_info_t*    p_daho_cfg =
        rrm_cellm_get_daho_cell_config_info_params(p_ue_ctxt->cell_index);
    /*SPR 17852 Start*/
    U16 count = RRM_ZERO;
    U32 carrier_freq = RRM_ZERO;
    U8 requested_cell_id[MAX_CELL_IDENTITY_OCTETS] = {RRM_ZERO};
    U8 existing_cell_id[MAX_CELL_IDENTITY_OCTETS] = {RRM_ZERO};
    /*SPR 17852 End*/
    /* SPR 12897 Start */
	U8                  ret_car_freq_supported = RRM_ZERO;
    /* SPR 12897 End */
    U16  daho_pci            = INVALID_PHYSICAL_CELL_ID;
    /* SPR 15674 Start */
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t      phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_ZERO, RRM_ZERO, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 15674 End */
    rrm_ho_params_t*    p_ho_params         = RRM_PNULL; 

    RRM_UT_TRACE_ENTER();
    p_ho_params = &(p_ue_ctxt->ho_params);
    /*SPR 17852 Start*/
    if((p_daho_cfg != RRM_PNULL) && 
            /*daho clean up(condition added for presence of daho cell)*/
            (p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT))
    {
            rrm_uem_find_eutran_daho_cell(p_ue_ctxt, p_daho_cfg,
                    carrier_freq, &daho_pci);
    }
    /*coverity 25201, code for NULL condition added, aditya, rel 1.3.1*/
    else
    {
        RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, "p_daho_cfg is NULL");
        /* SPR 15674 Start */
        return phy_cell_id_info;
        /* SPR 15674 End */
    }


    if(RRM_PNULL != p_ncl_params) /* Coverity: CID 29570 */
    {
      /* SPR-17852 START */
        if((p_ho_params->blind_timer) && !(qvTimerRunning(p_ho_params->blind_timer)))
        {
      /* SPR-17852 END */
            RRM_MEMCPY(requested_cell_id ,
                    p_daho_cfg->eutran_daho_cell_config_info.cell_identity ,
                    sizeof(requested_cell_id));

            for(count = RRM_ZERO; count<p_ncl_params->num_valid_intra_freq_cell; count++)
            {
                RRM_MEMCPY(existing_cell_id ,
                        p_ncl_params->intra_freq_cells[count].cell_id.cell_identity ,
                        sizeof(existing_cell_id));

                if(!RRM_MEMCMP(requested_cell_id, existing_cell_id, MAX_CELL_IDENTITY_OCTETS))
                {
                    rf_parameters = rrm_cellm_get_rf_params(p_ue_ctxt->cell_index);
                    carrier_freq = rf_parameters->rf_configurations.dl_earfcn;
                    break;
                }
            }


            for(count = RRM_ZERO; count<p_ncl_params->num_valid_inter_freq_cell; count++)
            {
                RRM_MEMCPY(existing_cell_id ,
                        p_ncl_params->inter_freq_cells[count].cell_id.cell_identity ,
                        sizeof(existing_cell_id));

                if(!RRM_MEMCMP(requested_cell_id, existing_cell_id, MAX_CELL_IDENTITY_OCTETS))
                {
                    carrier_freq = p_ncl_params->inter_freq_cells[count].eutra_carrier_arfcn;
                    break;
                }
            }

            ret_car_freq_supported = rrm_ue_is_ue_supported_eutra_frequency (
                    carrier_freq,
                    p_ue_ctxt);

            if(ret_car_freq_supported == RRM_FALSE)
            {

                RRM_TRACE (
                        g_uem_log_on_off,
                        p_rrm_uem_facility_name,
                        RRM_WARNING,
                        "EUTRAN DAHO Freq:[%d] is not supported by [UE:%d]",
                        carrier_freq,
                        p_ue_ctxt->ue_index);
                /* SPR 15674 Start */
                return phy_cell_id_info;
                /* SPR 15674 End */
            }
            RRM_TRACE (
                    g_uem_log_on_off,
                    p_rrm_uem_facility_name,
                    RRM_WARNING,
                    "Trying HO to E-UTRAN Blind cell");
            /* return Blind cell id */
            /* SPR 16406 Start */
            p_ho_params->eutra_known_cell_selection_list.bitmask |=\
                                                                   RRM_UE_BLIND_CELL_HO_TRIGGERED;
            /* SPR 16406 End */
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING, 
                "Empty NCL params");
    }


    /*Spr 17852 End*/

    /* SPR 10498 Fix End */
    /* SPR 15674 Start */
    phy_cell_id_info.phy_cell_id = daho_pci;
    phy_cell_id_info.carr_earfcn = carrier_freq;
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
    /* SPR 15674 End */
}

/* SPR 15674 Start */
/****************************************************************************
 * Function Name  : rrm_get_valid_daho_cell_id 
 * Inputs         : rrm_ue_context_t *p_ue_ctxt
 * Outputs        : DAHO PCI
 * Returns        : DAHO PCI
 * Description    : Find a valid DAHO PCI for handover based on the RAT type
 ****************************************************************************/
    phy_cell_id_info_t
rrm_get_valid_daho_cell_id(
        rrm_ue_context_t *p_ue_context
        )
{
    /* SPR 16406 18sep Start */
    phy_cell_id_info_t phy_cell_id_info = {RRM_ZERO, INVALID_PHYSICAL_CELL_ID, RRM_ZERO, RRM_ZERO, RRM_ZERO, {{{RRM_ZERO}, RRM_ZERO, {RRM_ZERO}}, {RRM_ZERO}}};
    /* SPR 16406 18sep End */
    /* SPR 15336 Fix Start */
    U8  dtm_ho_supp = RRM_ZERO;
    /* SPR 15336 Fix End */

    RRM_UT_TRACE_ENTER();

    switch(p_ue_context->target_cell_type)
    {
            /* SPR 16406 Start */
        case EUTRA_CELL:
            /*get eutran daho cell on if previous ho was
              not triggered on eutran daho cell*/
            if(!(p_ue_context->ho_params.eutra_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED)) 
            {
                phy_cell_id_info = rrm_validate_and_get_eutran_daho_pci(p_ue_context);
            }
            /*SPR_17893_START*/
            if((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id) && 
               !(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED))
            {
                phy_cell_id_info = rrm_validate_and_get_utran_daho_pci(p_ue_context); 
            }
            if((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id) && 
               !(p_ue_context->ho_params.geran_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED))
            {
                phy_cell_id_info = rrm_validate_and_get_geran_daho_pci(p_ue_context, &dtm_ho_supp); 
            }
            /*SPR_17893_END*/
            break;

        case UTRA_CELL:
            /*get utran daho cell on if previous ho was
              not triggered on utran daho cell*/
            if(!(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED))
            {
                phy_cell_id_info = rrm_validate_and_get_utran_daho_pci(p_ue_context); 
            }
            /*SPR_17893_START*/
            if(((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id)) && 
              !(p_ue_context->ho_params.eutra_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED)) 
            {
                phy_cell_id_info = rrm_validate_and_get_eutran_daho_pci(p_ue_context);
            }
            if((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id) && 
               !(p_ue_context->ho_params.geran_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED))
            {
                phy_cell_id_info = rrm_validate_and_get_geran_daho_pci(p_ue_context, &dtm_ho_supp); 
            }
            /*SPR_17893_END*/
            break;

        case GERAN_CELL:
            /*get geran daho cell on if previous ho was
              not triggered on geran daho cell*/
            if(!(p_ue_context->ho_params.geran_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED))
            {
                /* SPR 15336 Fix Start */
                phy_cell_id_info = rrm_validate_and_get_geran_daho_pci(p_ue_context, &dtm_ho_supp); 
                /* SPR 15336 Fix End */
            }
            /*SPR_17893_START*/
            if(((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id)) && 
              !(p_ue_context->ho_params.eutra_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED)) 
            {
                phy_cell_id_info = rrm_validate_and_get_eutran_daho_pci(p_ue_context);
            }
            if((INVALID_PHYSICAL_CELL_ID == phy_cell_id_info.phy_cell_id) && 
               !(p_ue_context->ho_params.utra_known_cell_selection_list.bitmask & 
                        RRM_UE_BLIND_CELL_HO_TRIGGERED))
            {
                phy_cell_id_info = rrm_validate_and_get_utran_daho_pci(p_ue_context); 
            }
            /*SPR_17893_END*/
            break;
            /* SPR 16406 End */

        default :
            phy_cell_id_info.phy_cell_id = INVALID_PHYSICAL_CELL_ID;
            break;
    }
    RRM_UT_TRACE_EXIT();
    return phy_cell_id_info;
}
/* SPR 15674 End */
/* ho restriction : code changes started */
/****************************************************************************
 * Function Name  :  rrm_is_cell_pres_in_serv_eqv_list
 * Inputs         :  plmn_id of CELL.
 * Outputs        :    
 * Returns        :  TRUE if plmn_id is equal to serving plmn_id or present in the equivalent list otherwise FALSE.
 * Description    :  Determine that is plmn_id is presenet in the serving and equivalent list. 
 ****************************************************************************/
rrm_bool_et
rrm_is_cell_pres_in_serv_eqv_list (
    /*SPR 17777 +-*/    
        rrm_ho_restriction_list_t        *p_ho_restriction_list
        )
{
    rrm_bool_et ret_val= RRM_FALSE;
    RRM_UT_TRACE_ENTER();

    if(RRM_PNULL != p_ho_restriction_list)
    {
        ret_val= RRM_TRUE;

    }
    else
    {
        RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_WARNING,
                "p_ho_restriction_list is  NULL");
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  :  rrm_is_cell_pres_in_ho_res_tac
 * Inputs         :  plmn_id of CELL.
 * Outputs        :    
 * Returns        :  TRUE if plmn_id and TAC is present in the forbidden TAC list otherwise FALSE.
 * Description    :  Determine that is plmn_id and TAC is presenet in the forbidden TAC list. 
 ****************************************************************************/
rrm_bool_et
rrm_is_cell_pres_in_ho_res_tac (
        U8                               plmn_tac[],
        rrm_plmn_identity_t              plmn_id,
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        )
{
    rrm_bool_et             ret_val= RRM_FALSE;
    rrm_bool_et             is_plmn_in_forbidden_tac_list = RRM_FALSE;
    rrm_bool_et             is_tac_in_forbidden_tac_list = RRM_FALSE;
    U16                     plmn_list_count=RRM_ZERO;
    U16                     tac_list_count=RRM_ZERO;
    rrm_forbidden_ta_t*     p_forbidden_ta = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    if(RRM_PNULL != p_ho_restriction_list)
    {
        if(p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT)
        {
            for(plmn_list_count= RRM_ZERO; plmn_list_count < p_ho_restriction_list->forbidden_ta_list.count ;plmn_list_count++)
            {
                /*SPR 20589 FIX START*/
                p_forbidden_ta = &(p_ho_restriction_list->forbidden_ta_list.p_forbidden_ta[plmn_list_count]);
                if(RRM_NULL != p_forbidden_ta)
                {
                    /*SPR 20589 FIX END*/
                    if((p_forbidden_ta->plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG) &&
                            (plmn_id.presence_bitmask                       & PLMN_IDENTITY_MCC_PRESENCE_FLAG))
                    {
                        if(!RRM_MEMCMP(&plmn_id.mcc,&p_forbidden_ta->plmn_identity.mcc,MCC_OCTET_SIZE))
                        {
                            if((plmn_id.mnc.count ==p_forbidden_ta->plmn_identity.mnc.count )&&
                                    (!RRM_MEMCMP(&plmn_id.mnc.mnc,&p_forbidden_ta->plmn_identity.mnc.mnc,plmn_id.mnc.count)))
                            {
                                is_plmn_in_forbidden_tac_list = RRM_TRUE;
                            }
                        }
                    }
                    else if((p_forbidden_ta->plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG) ||
                            (plmn_id.presence_bitmask                       & PLMN_IDENTITY_MCC_PRESENCE_FLAG))
                    {
                        /* one PLMN have MCC other one doesnot have */
                    }
                    else
                    {
                        if((plmn_id.mnc.count == p_forbidden_ta->plmn_identity.mnc.count )&&
                                (!RRM_MEMCMP(&plmn_id.mnc.mnc,&p_forbidden_ta->plmn_identity.mnc.mnc,plmn_id.mnc.count)))
                        {
                            is_plmn_in_forbidden_tac_list = RRM_TRUE;
                        }
                    }
                    if(RRM_TRUE == is_plmn_in_forbidden_tac_list)
                    {
                        is_plmn_in_forbidden_tac_list = RRM_FALSE;
                        for(tac_list_count = RRM_ZERO ; tac_list_count < p_forbidden_ta->forbidden_tac.count ; tac_list_count++)
                        {
                            /*SPR 20589 FIX START*/
                            if(!RRM_MEMCMP(plmn_tac,p_forbidden_ta->forbidden_tac.p_forbidden_tac[tac_list_count].tac , MAX_TAC_SIZE))
                                /*SPR 20589 FIX END*/
                            {
                                is_tac_in_forbidden_tac_list = RRM_TRUE;
                                RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                                        "plmn_id and TAC are present in the forbidden tac list");
                                break;

                            }
                        }
                    }
                    if (RRM_TRUE == is_tac_in_forbidden_tac_list )
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                    /*SPR 20589 FIX START*/
                }
                /*SPR 20589 FIX End*/
            }
        }
    }
    else
    {
        RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_WARNING,
                "p_ho_restriction_list is  NULL");
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  :  rrm_is_cell_pres_in_ho_res_lac
 * Inputs         :  plmn_id of CELL.
 * Outputs        :    
 * Returns        :  TRUE if plmn_id/LAC is present in the forbidden LAC list otherwise FALSE.
 * Description    :  Determine that is plmn_id/LAC is presenet in the forbidden LAC list. 
 ****************************************************************************/
rrm_bool_et
rrm_is_cell_pres_in_ho_res_lac (
        U8                               plmn_lac[],
        rrm_plmn_identity_t              plmn_id,
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        )
{
    rrm_bool_et             ret_val= RRM_FALSE;
    rrm_bool_et             is_plmn_in_forbidden_lac_list = RRM_FALSE;
    rrm_bool_et             is_lac_in_forbidden_lac_list = RRM_FALSE;
    U16                     plmn_list_count=RRM_ZERO;
    U16                     lac_list_count=RRM_ZERO;
    rrm_forbidden_la_t*     p_forbidden_lac = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    if(RRM_PNULL != p_ho_restriction_list)
    {
        if(p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT)
        {
            for(plmn_list_count= RRM_ZERO; plmn_list_count < p_ho_restriction_list->forbidden_la_list.count ;plmn_list_count++)
            {
                /*SPR 20589 FIX START*/
                p_forbidden_lac = &(p_ho_restriction_list->forbidden_la_list.p_forbidden_la[plmn_list_count]);
                if(RRM_NULL != p_forbidden_lac)
                {
                    /*SPR 20589 FIX END*/
                    if((p_forbidden_lac->plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG) &&
                            (plmn_id.presence_bitmask                        & PLMN_IDENTITY_MCC_PRESENCE_FLAG))
                    {
                        if(!RRM_MEMCMP(&plmn_id.mcc,&p_forbidden_lac->plmn_identity.mcc,MCC_OCTET_SIZE))
                        {
                            if((plmn_id.mnc.count == p_forbidden_lac->plmn_identity.mnc.count )&&
                                    (!RRM_MEMCMP(&plmn_id.mnc.mnc,&p_forbidden_lac->plmn_identity.mnc.mnc,plmn_id.mnc.count)))
                            {
                                is_plmn_in_forbidden_lac_list = RRM_TRUE;
                            }
                        }
                    }
                    else if((p_forbidden_lac->plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG)||
                            (plmn_id.presence_bitmask                        & PLMN_IDENTITY_MCC_PRESENCE_FLAG))
                    {
                        /* one PLMN have MCC other one doesnot have */
                    }
                    else
                    {
                        if((plmn_id.mnc.count == p_forbidden_lac->plmn_identity.mnc.count )&&
                                (!RRM_MEMCMP(&plmn_id.mnc.mnc,&p_forbidden_lac->plmn_identity.mnc.mnc,plmn_id.mnc.count)))
                        {
                            is_plmn_in_forbidden_lac_list = RRM_TRUE;
                        }
                    }
                    if(RRM_TRUE == is_plmn_in_forbidden_lac_list)
                    {
                        is_plmn_in_forbidden_lac_list = RRM_FALSE;
                        for(lac_list_count = RRM_ZERO ; lac_list_count < p_forbidden_lac->forbidden_lacs_list.count ; lac_list_count++)
                        {
                            /*SPR 20589 FIX START*/
                            if(!RRM_MEMCMP(plmn_lac,p_forbidden_lac->forbidden_lacs_list.p_forbidden_lac[lac_list_count].lac , RRM_OAM_MAX_NUM_LAC_OCTETS))
                                /*SPR 20589 FIX END*/
                            {
                                is_lac_in_forbidden_lac_list = RRM_TRUE;
                                RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                                        "plmn_id and LAC are present in the forbidden LAC list");
                                break;

                            }
                        }
                    }
                    if (RRM_TRUE == is_lac_in_forbidden_lac_list )
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                    /*SPR 20589 FIX START*/
                }
                /*SPR 20589 FIX End*/
            }
        }
    }
    else
    {
        RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_WARNING,
                "p_ho_restriction_list is  NULL");
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  :  rrm_is_eutran_cell_restricted
 * Inputs         :  
 * Outputs        :    
 * Returns        :  TRUE if EUTRAN cell is restricted by ho_restriction_list otherwise FALSE
 * Description    :  Determine that is EUTRAN cell present in ho_restriction_list 
 ****************************************************************************/
rrm_bool_et
rrm_is_eutran_cell_restricted (
        /* SPR 17564 Fix Start */
        rrm_ue_context_t *p_ue_ctxt,
        /* SPR 17564 Fix End */
        rrm_oam_cell_plmn_info_t        euran_plmn_id,
        U8                              tac[],
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        )
{
    rrm_plmn_identity_t  plmn_id= {RRM_ZERO};
    rrm_bool_et ret_val= RRM_FALSE;
    RRM_UT_TRACE_ENTER();

    if(RRM_PNULL != p_ho_restriction_list)
    {
        /* SPR 21251 Start */
        if((p_ho_restriction_list->bitmask & HO_RESTRICTION_EQUIV_PLMN_LIST_PRESENT ) ||
                (p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT) ||
                (p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT) ||
                (p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT) ||
                (p_ho_restriction_list->serving_plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG))
        {

            /* SPR 17564 Fix Start */
            if (RRM_FALSE == rrm_is_ue_special_previledge(p_ue_ctxt))
                /* SPR 17564 Fix End */
            {
                /* SPR 21251 End */
                plmn_id.presence_bitmask |= PLMN_IDENTITY_MCC_PRESENCE_FLAG;
                RRM_MEMCPY(plmn_id.mcc,euran_plmn_id.mcc ,MCC_OCTET_SIZE);
                plmn_id.mnc.count = euran_plmn_id.num_mnc_digit;
                RRM_MEMCPY(plmn_id.mnc.mnc,euran_plmn_id.mnc ,MAX_MNC_OCTET_SIZE);

                /*SPR 17777 +-*/    
                if(RRM_TRUE == rrm_is_cell_pres_in_serv_eqv_list(p_ho_restriction_list))
                {
                    /* Cell is present in the serving cell or equivalent list */
                    if(RRM_TRUE == rrm_is_cell_pres_in_ho_res_tac(tac,plmn_id,p_ho_restriction_list))
                    {
                        /* this plmn_id and TAC combination is restricted */
                        ret_val = RRM_TRUE;
                    }
                }
                else
                {  
                    /* SPR 17564 Fix Start */
                    RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                            "cell is not present in serving equivalent list");
                    /* SPR 17564 Fix End */
                }
            }
            /* SPR 21251 Start */
            else
            {  
                /* SPR 17564 Fix Start */
                RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                        "UE is either EMERGENCY or HIGHPRIORITY");
                /* SPR 17564 Fix End */
            }
        }
        else
        {
            RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_DETAILED,
                    "Invalid bitmask[%d] is set in p_ho_restriction_list",
                    p_ho_restriction_list->bitmask);
        }
        /* SPR 21251 End */
    }
    else
    {
        RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_WARNING,
                "p_ho_restriction_list is NULL");
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  :  rrm_is_inter_rat_cell_restricted
 * Inputs         :  physical cell ID of the neighbour.
 * Outputs        :    
 * Returns        :  TRUE if inter RAT cell is restricted otherwise FALSE
 * Description    :  Determine that is inter RAT cell present in ho_restriction_list 
 ****************************************************************************/
rrm_bool_et
rrm_is_inter_rat_cell_restricted (
        /* SPR 17564 Fix Start */
        rrm_ue_context_t *p_ue_ctxt,
        /* SPR 17564 Fix End */
        plmn_identity_t                 iner_rat_plmn_id,
        U8                              lac[],
        rrm_cell_type_t                 cell_type,
        rrm_ho_restriction_list_t       *p_ho_restriction_list
        )
{
    rrm_plmn_identity_t  plmn_id= {RRM_ZERO};
    rrm_bool_et ret_val= RRM_FALSE;
    RRM_UT_TRACE_ENTER();

    if(RRM_PNULL != p_ho_restriction_list)
    {
        if((!(p_ho_restriction_list->bitmask & HO_RESTRICTION_EQUIV_PLMN_LIST_PRESENT )) &&
                (!(p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT)) &&
                (!(p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT)) &&
                (!(p_ho_restriction_list->bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT)) &&
                (!(p_ho_restriction_list->serving_plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG) ))
        {
            RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                    "Empty ho_restriction_list  bitmask =%d, serving_plmn_identity.presence_bitmask =%d",
                    p_ho_restriction_list->bitmask,
                    p_ho_restriction_list->serving_plmn_identity.presence_bitmask);
        }
        /* SPR 17564 Fix Start */
        else if (RRM_TRUE == rrm_is_ue_special_previledge(p_ue_ctxt))
        {
            RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                    "UE is either EMERGENCY or HIGHPRIORITY");
        /* SPR 17564 Fix End */
            /* for emergency and highpriority, ho_restriction_list will not be applied */
        }
        else if( (p_ho_restriction_list->bitmask &  HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) &&
                ( (UTRA_CELL == cell_type) &&
                  ( (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_ALL) ||
                    (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_UTRAN_TYPE) ||
                    (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN) || 
                    (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_CDMA2000_AND_UTRAN) ) ) )
        {
            ret_val=RRM_TRUE;
            RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                    "UTRAN is restricted as per ho_restriction_list forbidden_inter_rat= %d",
                    p_ho_restriction_list->forbidden_inter_rat);
        }
        else if( (p_ho_restriction_list->bitmask &  HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) &&
                ( ( GERAN_CELL == cell_type) &&
                  ( (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_ALL) ||
                    (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_GEREN_TYPE) ||
                    (p_ho_restriction_list->forbidden_inter_rat == FORBIDDEN_INTER_RAT_GERAN_AND_UTRAN) ) ))
        {
            ret_val=RRM_TRUE;
            RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                    "GREAN is restricted as per ho_restriction_list forbidden_inter_rat= %d",
                    p_ho_restriction_list->forbidden_inter_rat);
        }
        else
        {
            RRM_MEMCPY(&plmn_id, &(iner_rat_plmn_id), sizeof(plmn_identity_t));
            /*SPR 17777 +-*/    
            if(RRM_TRUE == rrm_is_cell_pres_in_serv_eqv_list(p_ho_restriction_list))
            {
                /* Cell is present in the serving cell or equivalent list */
                if(RRM_TRUE == rrm_is_cell_pres_in_ho_res_lac(lac,plmn_id,p_ho_restriction_list))
                {
                    /* this plmn_id and LAC combination is restricated */
                    ret_val = RRM_TRUE;
                }
            }
            else
            {
                /* this plmn_id is not present in the serving and equivalent list */
                ret_val = RRM_TRUE;
            }
        }
    }
    else
    {
        RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_WARNING,
                "p_ho_restriction_list is NULL");
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_is_daho_cell_restricted 
 * Inputs         : rrm_ue_context_t *p_ue_ctxt
 * Outputs        : 
 * Returns        : TRUE if DAHO PCI is restricted by ho_restiction_list otherwise FALSE
 * Description    : Determine is daho cell restricted
 ****************************************************************************/
rrm_bool_et
rrm_is_daho_cell_restricted(
        rrm_ue_context_t *p_ue_context
        )
{
    rrm_bool_et                     ret_val = RRM_FALSE;
    rrm_daho_cell_config_info_t     *p_daho_cfg = RRM_PNULL;
    rrm_plmn_identity_t             plmn_id = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();
    /* SPR:757- Coverity fix start */
    if (p_ue_context != RRM_PNULL )
    {
        /* SPR:757- Coverity fix end */
        p_daho_cfg = rrm_cellm_get_daho_cell_config_info_params(p_ue_context->cell_index);

        /*coverity 29149, rel2.0.1 ,aditya, operator changed to && from ||*/
        if(p_daho_cfg != RRM_PNULL) 
            /* Coverity 19oct End : 54991 */
        {
            switch(p_ue_context->target_cell_type)
            {
                case EUTRA_CELL:
                /* SPR 10498 Fix Start */
                /* SPR 10498 Fix End */
                        if((!(p_ue_context->ho_restriction_list.bitmask & HO_RESTRICTION_EQUIV_PLMN_LIST_PRESENT) ) &&
                                (!(p_ue_context->ho_restriction_list.bitmask & HO_RESTRICTION_FORBIDDEN_TA_LIST_PRESENT)) &&
                                (!(p_ue_context->ho_restriction_list.bitmask & HO_RESTRICTION_FORBIDDEN_LA_LIST_PRESENT)) &&
                                (!(p_ue_context->ho_restriction_list.bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT)) &&
                                (!(p_ue_context->ho_restriction_list.serving_plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG) ))
                        {
                            RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Empty ho_restriction_list  bitmask =%d, serving_plmn_identity.presence_bitmask =%d",
                                    p_ue_context->ho_restriction_list.bitmask,
                                    p_ue_context->ho_restriction_list.serving_plmn_identity.presence_bitmask);
                        }
                    /* SPR 17564 Fix Start */
                    else if (RRM_FALSE == rrm_is_ue_special_previledge(p_ue_context))
                    /* SPR 17564 Fix End */
                        {
                            /* SPR 10498 Fix Start */
                            rf_params_t  *p_rf_parameters = rrm_cellm_get_rf_params(p_ue_context->cell_index);
                            /* Intra-freq DAHO */
                            if(p_ue_context->ho_params.target_earfcn == p_rf_parameters->rf_configurations.dl_earfcn)
                            {
                                if(p_daho_cfg->bitmask & EUTRA_DAHO_CELL_PRESENT)
                                {
                                    plmn_id.presence_bitmask =  p_daho_cfg->eutran_daho_cell_config_info.plmn_id.bitmask;
                                    RRM_MEMCPY(plmn_id.mcc, p_daho_cfg->eutran_daho_cell_config_info.plmn_id.mcc,MCC_OCTET_SIZE);
                                    plmn_id.mnc.count = p_daho_cfg->eutran_daho_cell_config_info.plmn_id.num_mnc_digit;
                                    RRM_MEMCPY(plmn_id.mnc.mnc, p_daho_cfg->eutran_daho_cell_config_info.plmn_id.mnc, MAX_MNC_OCTET_SIZE);
                                }
                            }
                            /* Inter-freq DAHO */
                            else
                            {
                                U16     daho_pci = INVALID_PHYSICAL_CELL_ID;
                                rrm_eutran_inter_freq_daho_cell_config_info_t *p_inter_freq_daho = RRM_PNULL; 
                                p_inter_freq_daho = rrm_uem_search_inter_freq_daho_cell(p_ue_context, p_daho_cfg, 
                                        p_ue_context->ho_params.target_earfcn, &daho_pci);

                                if(RRM_PNULL == p_inter_freq_daho)
                                {
                                    RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,RRM_ERROR,
                                            "DAHO cell not found for Inter-freq = %d", p_ue_context->ho_params.target_earfcn);
                                }
                                else
                                {
                                    plmn_id.presence_bitmask |= RRMCM_MCC_PRESENCE_BITMASK;
                                    RRM_MEMCPY(plmn_id.mcc, p_inter_freq_daho->cell_id.primary_plmn_id.mcc,MCC_OCTET_SIZE);
                                    plmn_id.mnc.count = p_inter_freq_daho->cell_id.primary_plmn_id.num_mnc_digit;
                                    RRM_MEMCPY(plmn_id.mnc.mnc, p_inter_freq_daho->cell_id.primary_plmn_id.mnc, MAX_MNC_OCTET_SIZE);
                                }
                            }
                            /* SPR 10498 Fix End */
                            /*SPR 17777 +-*/    
                            if(RRM_TRUE == rrm_is_cell_pres_in_serv_eqv_list(&p_ue_context->ho_restriction_list))
                            {
                                /* TAC info is not present for EUTRAN DAHO cell */
                            }
                            else
                            {
                                ret_val =RRM_TRUE;
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                        "EUTRAN DAHO cell is restricted");
                            }
                        }
                        else
                        {
                        /* SPR 17564 Fix Start */
                        RRM_TRACE ( g_uem_log_on_off,  p_rrm_uem_facility_name, RRM_BRIEF,
                                "UE is either EMERGENCY or HIGHPRIORITY");
                        /* SPR 17564 Fix End */
                            /* for emergency and highpriority, ho_restriction_list will not be applied */
                        }
                        /* SPR 10498 Fix Start */
                        /* SPR 10498 Fix End */
                    break;

                case UTRA_CELL:
                    if(p_daho_cfg->bitmask & UTRAN_DAHO_CELL_PRESENT)
                    {
                    /* SPR 17564 Fix Start */
                    if (RRM_TRUE == rrm_is_inter_rat_cell_restricted (p_ue_context,
                    /* SPR 17564 Fix End */
                                    p_daho_cfg->utran_daho_cell_config_info.rai.lai.plmn_identity,
                                    p_daho_cfg->utran_daho_cell_config_info.rai.lai.location_area_code,
                                    UTRA_CELL,
                                    &p_ue_context->ho_restriction_list))
                        {
                            ret_val = RRM_TRUE;
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "UTRAN DAHO cell is restricted");
                        }
                    }
                    break;

                case GERAN_CELL:
                    if(p_daho_cfg->bitmask & GERAN_DAHO_CELL_PRESENT)
                    {
                    /* SPR 17564 Fix Start */
                    if (RRM_TRUE == rrm_is_inter_rat_cell_restricted (p_ue_context,
                    /* SPR 17564 Fix End */
                                    p_daho_cfg->geran_daho_cell_config_info.lai.plmn_identity,
                                    p_daho_cfg->geran_daho_cell_config_info.lai.lac,
                                    GERAN_CELL,
                                    &p_ue_context->ho_restriction_list))
                        {
                            ret_val = RRM_TRUE;
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "GERAN  DAHO cell is restricted");
                        }
                    }
                    break;

                default :
                    break;
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "p_daho_cfg is NULL");
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "p_ue_context is NULL");
    }
    /* SPR:757- Coverity fix end */

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* ho restriction : code changes end */
