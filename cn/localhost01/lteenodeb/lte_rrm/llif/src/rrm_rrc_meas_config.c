/* vim: ts=4:et:ai:ci:sw=4
*/
/*****************************************************************************
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_ue_rrc_meas_config.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains the functions for measurement configuration
 *                 filling
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   16 July 2012   bgh13142      Initial
 *   17 Apr  2013   gur29220      SPR 5883       Measurement gap pattern gp1 
 *                                               is not getting configured
 *   17 Apr 2013    gur29220      SPR 5882       B2 not configured when A2
 *                                               measurement is received by RRM
 *   25 Apr 2014    gur31824      SPR 10729      RRM is not releasing the UE 
 *                                               when RRC sends 
 *                                               RRC_RRM_LOCAL_ERROR_IND to 
 *                                               RRM for UE with single rabs
 *   30 May 2014    gur34951      SPR 8004       ECID Enhancement done for inter
 *                                               Frequency Cells
 *   16 June 2014    gur29831     SPR 11873      Fix for pci confusion
 *   16 Jun 2014    gur25767      SPR 11760      Corrected the logic for prioritizing
 *                                               RAT type on the basis of
 *                                               supported rats per qci and SPID
 *   19 Jun,2014    gur27356      SPR 12035      Measurement report A4 in cell Block Scenario.
 *   24 June 2014   gur21481      SPR 12076      CA validation for same freq present 
 *                                               in NCL is added
 *    2 July,2014   gur25767      SPR 10498      Inter-freq Daho Support
 *   8 July 2014    gur21481      SPR 12080      Filled cell_to_add_mod for CA measurement
 *   10 July 2014   gur29831      SPR 12273      Fix for spr 12273
 *    2 July,2014   gur25767      SPR 10498      Inter-freq Daho Support
 *   12 July,2014   gur30784      SPR 12541      SPID related fix
 *   18 July,2014   gur25767      SPR 10349      SPID related fix for CSFB
 *   24 Jul,2014    gur32417      SPR 12689      PCI conflict with serving issue changes 
 *   25 Jul,2014    gur21481      SPR 12798      bitmask for meas object was not set 
 *   04 Aug,2014    gur21481      SPR 12873      RRM is not triggering HO
 *                                               Cancel once A1 measurement
 *                                               result received & Ho process is ongoing
 *   12 Aug,2014    gur21481      SPR 12841     RRM not updating Meas configuration while 
 *                                              sending meas configuration request after 
 *                                              getting A2 after adding one cell by ANR 
 *   27 Aug,2014   gur21481      SPR 13331,13317 SPR Fixed
 *   29 Aug,2014   gur27356      SPR 13400      RRM is not sending "meas_config_req" for
 *                                              CDMA HRPD/CDMA 1xRTT in proper way after sending A2 report two times 
 ******************************************************************************/
/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "l3_api_parser.h"
#include "l3_api_composer.h"
#include "rrc_rrm_intf.h"
#include "rrm_utils.h"
#include "rrm_defines.h"
#include "rrm_ue_context.h"
#include "rrm_ue_mgr.h"
#include "rrm_ue_ho_utils.h"
#include "rrm_ue_fsm.h"
#include "rrm_rrc_ue_intf.h"
#include "rrm_cell_context.h"
#include "rrm_cell_mgr.h"
#include "rrm_events.h"

//#include "rrm_rrc_meas_config.h"
//
/****************************************************************************
 * Static function declarations
 ****************************************************************************/

/******************************************************************************
 **  FUNCTION NAME: fill_inter_freq_meas_object_info
 **  INPUT        : p_rrm_ue_context,p_idle_mode_mobility_params,freq_count
 **  OUTPUT       : p_meas_object_to_add_mod
 **  DESCRIPTION  : Fills the eutran configuration in meas config list with values
 **                 received from OAM to be sent to RRC.
 **  RETURNS      : None
 *******************************************************************************/
static rrm_void_t
fill_inter_freq_meas_object_info(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                           freq_count
        );



rrm_void_t
fill_report_config_for_event_B1(
        report_config_to_add_mod_t      *p_report_config_to_add_mod,
        rrm_irat_params_t               *p_rrm_irat_params,
        /*SPR  17777 +-*/
        U8                               utran_trigger_quantity);


/* HO Oscillation START */
/* SPR 15804 Fix Start */
rrm_void_t
rrm_ho_osc_prevention (
        rrm_ue_context_t          *p_rrm_ue_context,
        rrm_oam_eutran_global_cell_id_t *p_cgi_selected_cell,
        /*CID 112446 Fix Start*/
        rrm_q_offsetrange_et *cell_individual_offset
        /*CID 112446 Fix Stop*/
        );
/* SPR 15804 Fix Stop */
/* HO Oscillation END */

/* START: CSR 00057390*/
rrm_bool_et
rrm_fill_new_added_meas_object
(
 rrm_ue_context_t                *p_rrm_ue_context,
 rrm_meas_config_t               *p_meas_config,
 meas_object_to_add_mod_list_t   *p_new_added_meas_object
 );

/* Fix for CSR 58972 start*/
/* Function rrm_is_meas_object_configured_for_frequency prototype deleted*/
/* Fix for CSR 58972 end*/

rrm_bool_et
rrm_is_meas_event_configured
(
 rrm_ue_context_t       *p_ue_context,
 U8                     meas_object_id,
 rrmc_meas_report_event_et meas_event
 );
/* END: CSR 00057390*/
/*SPR 14326 Start*/
static rrm_void_t
fill_measurement_gap_info(
        rrm_meas_config_t   *p_meas_config,
        rrm_ue_context_t    *p_rrm_ue_context);
/*SPR 14326 End */
/*SPR 21365 fix start*/
U8 rrm_count_measurements_for_ho_configured
( rrm_ue_context_t *p_rrm_ue_context
);
/*SPR 21365 fix stop*/
/******************************************************************************
 *   FUNCTION NAME: rrm_uem_allocate_free_id
 *   INPUT        : p_rrm_ue_context,type
 *   OUTPUT       : none
 *   DESCRIPTION  : returns free id available
 *   RETURNS      : U8, new id available
 ******************************************************************************/
/* Fix for CSR 58972 start*/
U8 rrm_uem_allocate_free_id
/* Fix for CSR 58972 end*/
(
 rrm_ue_context_t            *p_rrm_ue_context,
 rrmc_meas_id_type_et         type
 )

{
    int new_id = MAX_MEAS_OBJECT_ID+RRM_ONE, i;

    switch (type)
    {
        case MEAS_CONFIG:
        {
            for (i = HO_MEAS_START_ID; i<=MAX_MEAS_OBJECT_ID;i++)  /* UE_POS: Code corrected for ID to be in the range 13-32 */
            {
                if (p_rrm_ue_context->meas_object_id_list[i - HO_MEAS_START_ID] == RRM_ZERO )
                {
                    p_rrm_ue_context->meas_object_id_list[i - HO_MEAS_START_ID] = RRM_ONE;
                    /* SPR 17424 Fix Start */
                    new_id =i + RRM_ONE;
                    /* SPR 17424 Fix End */
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Returned Meas_config_object id:%d",new_id);
                    break;
                }
            }
            break;
        }
        case REP_CONFIG:
        {
            for (i = HO_MEAS_START_ID; i<=MAX_MEAS_OBJECT_ID;i++)  /* UE_POS: Code corrected for ID to be in the range 13-32 */
            {
                if (p_rrm_ue_context->report_config_id_list[i - HO_MEAS_START_ID] == RRM_ZERO )
                {
                    p_rrm_ue_context->report_config_id_list[i - HO_MEAS_START_ID] = RRM_ONE;

                    new_id =i;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Returned Report_config_object id:%d",new_id);
                    break;
                }
            }
            break;
        }
        case MEAS_REPORT:
        {
            for (i = HO_MEAS_START_ID; i<=MAX_MEAS_OBJECT_ID;i++)    /* UE_POS: Code corrected for ID to be in the range 13-32 */
            {
                if (p_rrm_ue_context->meas_report_id_list[i - HO_MEAS_START_ID] == RRM_ZERO )
                {
                    p_rrm_ue_context->meas_report_id_list[i - HO_MEAS_START_ID] = RRM_ONE;
                    new_id =i;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Returned Meas_id id:%d",new_id);
                    break;
                }
            }
            break;
        }

    }
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Returning type %d and id %d",type,new_id);
    return new_id;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_uem_deallocate_free_id
 *   INPUT        : meas_type,used_id
 *   OUTPUT       : p_rrm_ue_context
 *   DESCRIPTION  : deletes the id from the list of allocated ids
 *   RETURNS      : none
 ******************************************************************************/

rrm_void_t
    rrm_uem_deallocate_used_id
(
 rrm_ue_context_t           *p_rrm_ue_context,
 rrmc_meas_id_type_et        meas_type,
 U8                          used_id
 )
{
    RRM_UT_TRACE_ENTER();

    switch (meas_type)
    {
        case MEAS_CONFIG:
            /*SPR 21365 fix start*/
            if ((used_id <= MAX_MEAS_OBJECT_ID) &&
            /*SPR 21365 fix stop*/
                (used_id >= (HO_MEAS_START_ID + 1)) &&
                p_rrm_ue_context->meas_object_id_list[used_id - HO_MEAS_START_ID - 1 ] == 1)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Deallocating the meas object id : %d\n",used_id);
            p_rrm_ue_context->meas_object_id_list[used_id - HO_MEAS_START_ID - 1] =0;
        }
        else
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "The meas object id %d is not yet allocated\n",used_id);
        break;

        case REP_CONFIG:
            /*SPR 21365 fix start*/
            if ((used_id <= MAX_MEAS_OBJECT_ID) &&
            /*SPR 21365 fix stop*/
                (used_id >= HO_MEAS_START_ID) &&
                p_rrm_ue_context->report_config_id_list[used_id - HO_MEAS_START_ID] == 1)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Deallocating the report config id : %d\n",used_id);
            p_rrm_ue_context->report_config_id_list[used_id - HO_MEAS_START_ID] =0;
        }
        else
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "The report config id %d is not yet allocated\n",used_id);
        break;

        case MEAS_REPORT:
            /*SPR 21365 fix start*/
            if ((used_id <= MAX_MEAS_OBJECT_ID) &&
            /*SPR 21365 fix stop*/
                (used_id >= HO_MEAS_START_ID) &&
                p_rrm_ue_context->meas_report_id_list[used_id - HO_MEAS_START_ID] == 1)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Deallocating the measurement id : %d\n",used_id);
            p_rrm_ue_context->meas_report_id_list[used_id - HO_MEAS_START_ID] =0;
        }
        else
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "The meas report id %d is not yet allocated\n",used_id);
        break;

        default :
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Unknown meas type received\n");
        break;
    }

    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_start*/
/******************************************************************************
 *  FUNCTION NAME: fill_meas_intra_freq_neigh_non_blacklist_cell
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_object_eutra
 *  DESCRIPTION  : Fills the list of neighbour cells and black listed cells
 *                 configured for The carrier frequency from the NRT list
 *                 available at OAM
 *  RETURNS      : None
 ******************************************************************************/
    U16
fill_meas_intra_freq_neigh_non_blacklist_cell(rrm_ue_context_t     *p_rrm_ue_context ,
        lte_ncl_t            *p_lte_ncl ,
        meas_object_eutra_t  *p_meas_object_eutra ,
        U8                   *neigh_count , 
        U8                   cell_count ,
        rrm_cell_index_t     cell_index)
{
    /*SPR 13331:start*/
    rf_params_t         *p_rf_params         = RRM_PNULL;
    YLNODE   *p_node = RRM_PNULL;
    meas_report_info_node_t   *p_meas_report_node = RRM_PNULL;
    rrm_bool_et pci_found = RRM_FALSE;
    U8 cell_to_add_cell_index = RRM_ZERO;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mod = RRM_PNULL;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node = RRM_PNULL;
    /*SPR 13331:end*/
    U16        phys_cell_id     = RRM_NULL;
    RRM_UT_TRACE_ENTER();
    if(p_lte_ncl->intra_freq_cells\
            [cell_count].blacklisted == RRM_FALSE)
    {
        /* SPR 17564 Fix Start */
        if(RRM_TRUE == rrm_is_eutran_cell_restricted(p_rrm_ue_context,
                    /* SPR 17564 Fix End */
                    /* ho_restriction at meas config */ 
                    p_lte_ncl->intra_freq_cells[cell_count].cell_id.primary_plmn_id,
                    p_lte_ncl->intra_freq_cells[cell_count].tac,
                    &p_rrm_ue_context->ho_restriction_list))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "Intra frequency Neighbor cell = %d, is restricted",
                    p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id);
            /* Bug_12689_fix: Start */
        }
        else if(RRM_TRUE == p_lte_ncl->intra_freq_cells[cell_count].is_pci_conflict)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "Intra frequency Neighbor cell = %d, has PCI conflict with Serving Cell.",
                    p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id);
            /* Bug_12689_fix: End */
        }
        else
        {
            /*SPR 13331:start*/
            p_rf_params = rrm_cellm_get_rf_params(cell_index);
            p_node = ylFirst(&(p_rrm_ue_context->meas_report_list));
            while(p_node)
            {

                p_meas_report_node = (meas_report_info_node_t*)p_node;
                if(p_meas_report_node->carrier_earfcn == p_rf_params->rf_configurations.dl_earfcn)
                {
                    break;
                }
                p_node = ylNext(p_node);
            }
            if (RRM_PNULL == p_node)
            {
                phys_cell_id = p_lte_ncl->intra_freq_cells
                    [cell_count].phy_cell_id;
                p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].\
                    cell_index =  (*neigh_count)+RRM_ONE;

                p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].\
                    phys_cell_id = p_lte_ncl-> intra_freq_cells[cell_count].phy_cell_id;

                p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].\
                    cell_individual_offset = p_lte_ncl->intra_freq_cells[cell_count].cio;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Intra frequency Neighbor cell = %d, is added to list",phys_cell_id);

                (*neigh_count)++;
            }
            else
            {
                p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) ylFirst(&(p_meas_report_node->cells_to_add_mod_list));
                while (p_cell_to_add_mode_node)
                {
                    if (p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id == p_cell_to_add_mode_node->phy_cell_id)
                    {
                        pci_found = RRM_TRUE;
                        break;
                    }
                    cell_to_add_cell_index = p_cell_to_add_mode_node->cell_index;
                    p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
                }
                if (RRM_FALSE == pci_found)
                {
                    p_cell_to_add_mod = (rrm_meas_obj_cell_to_add_info_t *)rrm_mem_get(sizeof(rrm_meas_obj_cell_to_add_info_t));
                    if (RRM_PNULL != p_cell_to_add_mod)
                    {
                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].cell_index = 
                            cell_to_add_cell_index + RRM_ONE; 
                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].phys_cell_id =
                            p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id; 
                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].cell_individual_offset = 
                            p_lte_ncl->intra_freq_cells[cell_count].cio;
                        p_meas_object_eutra->cells_to_add_mod_list.count++ ;
                        p_cell_to_add_mod->cell_index = cell_to_add_cell_index + RRM_ONE; 
                        p_cell_to_add_mod->phy_cell_id = p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id; 
                        ylPushTail(&(p_meas_report_node->cells_to_add_mod_list),&( p_cell_to_add_mod->node));
                        (*neigh_count)++;
                    }

                }
            }
            /*SPR 13331:end*/
        }

        /* HO Oscillation START */
         /* SPR 15804 Fix Start */
        rrm_ho_osc_prevention(p_rrm_ue_context, 
                &p_lte_ncl->intra_freq_cells[cell_count].cell_id, 
                /*cov 88559 +*/
        /*CID 112446 Fix Start*/
                (rrm_q_offsetrange_et *)(&p_lte_ncl->intra_freq_cells[cell_count].cio));
        /*CID 112446 Fix Stop*/
         /* SPR 15804 Fix Stop */
        /*cov 88559*/
        /* HO Oscillation END */

    }

    RRM_UT_TRACE_EXIT();
    return phys_cell_id ;
}
/*Klockwork_fix_end*/
/******************************************************************************
 *  FUNCTION NAME: fill_meas_intra_freq_neigh_black_cell_list
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_object_eutra
 *  DESCRIPTION  : Fills the list of neighbour cells and black listed cells
 *                 configured for The carrier frequency from the NRT list 
 *                 available at OAM      
 *  RETURNS      : None
 ******************************************************************************/
/*SPR 16879 CHANGES START*/
    static rrm_void_t
fill_meas_intra_freq_neigh_black_cell_list(
        rrm_ue_context_t           *p_rrm_ue_context,
        meas_object_eutra_t        *p_meas_object_eutra
        )
{
    RRM_UT_TRACE_ENTER();
    U8         valid_cells      = RRM_ZERO;
    U8         black_count      = RRM_ZERO;
    U8         neigh_count      = RRM_ZERO;
    U8         cell_count       = RRM_ZERO;
    U16        phys_cell_id     = RRM_NULL;
    rrm_cell_index_t           cell_index =p_rrm_ue_context->cell_index; 

    lte_ncl_t    *p_lte_ncl       = RRM_PNULL;
    p_lte_ncl                   = rrm_cellm_get_lte_ncl_info(cell_index);
    /* SPR-20552 START */
        /*CID 112446 Fix Start*/
    rrm_q_offsetrange_et cell_individual_offset = RRM_Q_OFFSET_RANGE_DB_24;
        /*CID 112446 Fix Stop*/
    /* SPR-20552 END */

    if(RRM_PNULL != p_lte_ncl)
    {
        valid_cells            =  p_lte_ncl-> num_valid_intra_freq_cell;
    }


    for(cell_count=RRM_ZERO; cell_count < valid_cells; cell_count++)
    {
        if(p_lte_ncl->intra_freq_cells\
                [cell_count].blacklisted == RRM_FALSE)
        {
            /* ho_restriction at meas config */ 
            /* SPR 17564 Fix Start */
            if(RRM_TRUE == rrm_is_eutran_cell_restricted(p_rrm_ue_context,
                        /* SPR 17564 Fix End */
                        p_lte_ncl->intra_freq_cells[cell_count].cell_id.primary_plmn_id,
                        p_lte_ncl->intra_freq_cells[cell_count].tac,
                        &p_rrm_ue_context->ho_restriction_list))
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "Intra frequency Neighbor cell = %d, is restricted",p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id);
            }
            else
            {
                phys_cell_id = p_lte_ncl->intra_freq_cells
                    [cell_count].phy_cell_id;
                p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[neigh_count].\
                    cell_index =  neigh_count+RRM_ONE;

                p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[neigh_count].\
                    phys_cell_id = p_lte_ncl-> intra_freq_cells[cell_count].phy_cell_id;

                /* SPR-20552 START */
                    cell_individual_offset = p_lte_ncl->intra_freq_cells[cell_count].cio;
                    rrm_ho_osc_prevention(p_rrm_ue_context, 
                            &p_lte_ncl->intra_freq_cells[cell_count].cell_id, 
                            &cell_individual_offset);
                p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[neigh_count].\
                        cell_individual_offset = cell_individual_offset;
                /* SPR-20552 END */
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Intra frequency Neighbor cell = %d, is added to list",phys_cell_id);

                neigh_count++;
            }
            /* SPR-20552 START */
            /* code deleted */
            /* SPR-20552 END */


        }
        else if(p_lte_ncl->intra_freq_cells\
                [cell_count].blacklisted == RRM_TRUE )
        {
            p_meas_object_eutra->black_cells_to_add_mod_list.\
                black_cells_to_add_mod[black_count].cell_index = \
                black_count +RRM_ONE;

            p_meas_object_eutra->black_cells_to_add_mod_list.\
                black_cells_to_add_mod[black_count].phys_cell_id_range.\
                presence_bitmask = RRM_ZERO;

            p_meas_object_eutra->black_cells_to_add_mod_list.\
                black_cells_to_add_mod[black_count].phys_cell_id_range.\
                start = p_lte_ncl->intra_freq_cells[cell_count]. phy_cell_id;

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "Intra frequency Neighbor PCI= %d, is added to black cell list",phys_cell_id);
            black_count++;
        }

    }
    if (neigh_count != RRM_ZERO)
    {
        p_meas_object_eutra->bitmask |= MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT;
        p_meas_object_eutra->cells_to_add_mod_list.count       = neigh_count;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "Total Number of Intra frequency Neighbor cells = %d",neigh_count);
    }
    if (black_count != RRM_ZERO)
    {
        p_meas_object_eutra->bitmask |=MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT;
        p_meas_object_eutra->black_cells_to_add_mod_list.count = black_count;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "Total Number of Intra frequency Neighbor black cells = %d",black_count);
    }

    RRM_UT_TRACE_EXIT();
}
/*SPR 16879 CHANGES END*/

/******************************************************************************
 *  FUNCTION NAME: fill_eutran_meas_object_to_add_mod_list_for_ho
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config_object
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with 
 *                 values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_eutran_meas_object_to_add_mod_list_for_ho(
        meas_object_to_add_mod_t   *p_meas_config_object,
        rrm_ue_context_t           *p_rrm_ue_context,
        rrm_cell_index_t           cell_index
        )
{
    RRM_UT_TRACE_ENTER();

    meas_object_eutra_t   *p_meas_object_eutra = RRM_PNULL;
    /* SPR14140start */
    /* eICIC_PHASE_1_2_CHANGES_START */
    rrm_cell_context_t      *p_cell_ctxt = RRM_PNULL;
    /*eICIC Bug 816 Changes Start*/   
    /* SPR 19620 Fix start */
    #ifndef TDD_MODE_FLAG
    lte_ncl_t    *p_lte_ncl = rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
    U8		  valid_intra_freq_count = RRM_ZERO;
    U8		  count = RRM_ZERO;
    #endif
    /* SPR 19620 Fix stop */
    /*eICIC Bug 816 Changes End*/
    U8      abs_count   = RRM_ZERO;
    /* eICIC_PHASE_1_2_CHANGES_END */
    /* SPR14140end */
    idle_mode_mobility_params_t *p_idle_mode_mobility_params = RRM_PNULL;
    intra_freq_params_t   *p_idle_mode_mobility_intra_freq_params = RRM_PNULL;
    rrm_sib_type_3_info_t *p_sib_3_info        = RRM_PNULL;
    rf_params_t           *p_rf_params         = RRM_PNULL;
    /*EICIC_TDD_CHANGES_START*/
#ifdef TDD_MODE_FLAG
    U8               abs_size = RRM_ZERO;
#endif
    /*EICIC_TDD_CHANGES_END*/
    p_idle_mode_mobility_params =  rrm_cellm_get_idle_mode_params(cell_index);
    p_idle_mode_mobility_intra_freq_params = &(p_idle_mode_mobility_params->\
            idle_mode_mobility_intra_freq_params);

    p_sib_3_info                           = rrm_cellm_get_sib3_info(cell_index);
    p_meas_object_eutra                    = &(p_meas_config_object->\
            meas_object.meas_object_eutra);
    p_rf_params                            = rrm_cellm_get_rf_params(cell_index);



    p_meas_config_object->meas_object.bitmask |= MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;

    p_meas_config_object->meas_object.meas_object_eutra.bitmask = RRM_ZERO;
    p_meas_config_object->meas_object.meas_object_eutra.carrier_freq = p_rf_params->
        rf_configurations.dl_earfcn;
    p_meas_config_object->meas_object.meas_object_eutra.allowed_meas_bandwidth = 
        p_sib_3_info->intra_freq_reselection_info.measurement_bandwidth;

    p_meas_config_object->meas_object.meas_object_eutra.presence_antenna_port1 = 
        p_sib_3_info->intra_freq_reselection_info.presence_antenna_port1;

    p_meas_config_object->meas_object.meas_object_eutra.neigh_cell_config = 
        p_idle_mode_mobility_intra_freq_params->neigh_cell_config;

    p_meas_config_object->meas_object.meas_object_eutra.offset_freq = 
        RRM_OAM_Q_OFFESET_RANGE_DB0;
    /*p_idle_mode_mobility_intra_freq_params->q_rx_lev_min_offset;*/

    /* SPR14140start */
    /* eICIC_PHASE_1_2_CHANGES_START */
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        return;
    }
/*eICIC Bug 831 code Changes Start*/
    if((rrm_is_set_fgi_bit(p_rrm_ue_context, RRM_HUNDRED_FIFTEEN) && 
        (RRM_REL10 <= p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
                                    eutra_radio_capability_info_def.access_stratum_release)) &&
            /*SPR 20779 Fix Start*/
	(((MEAS_REP_EVENT_PERIODICAL == p_rrm_ue_context->meas_rep_event) && (RRM_TRUE == rrm_cellm_is_victim_node(p_cell_ctxt))) ||
            /*SPR 20779 Fix Stop*/
	  (RRM_TRUE == rrm_cellm_is_aggressor_node(p_cell_ctxt)) ))
/*eICIC Bug 831 code Changes End*/

    {

        p_meas_config_object->meas_object.meas_object_eutra.bitmask = RRM_ZERO; 
        p_meas_config_object->meas_object.meas_object_eutra.bitmask |= MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_PRESENT;
        p_meas_config_object->meas_object.meas_object_eutra.
            meas_subframe_pattern_config_neigh_r10.bitmask = RRM_ZERO;
        p_meas_config_object->meas_object.meas_object_eutra.
            meas_subframe_pattern_config_neigh_r10.bitmask |= 
            MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_SETUP_PRESENT;

        p_meas_config_object->meas_object.meas_object_eutra.
            meas_subframe_pattern_config_neigh_r10.
            meas_subframe_pattern_config_neigh_r10_setup.bitmask = RRM_ZERO;

#ifndef TDD_MODE_FLAG
        p_meas_config_object->meas_object.meas_object_eutra.
            meas_subframe_pattern_config_neigh_r10.
            meas_subframe_pattern_config_neigh_r10_setup.
            meas_subframe_pattern_neigh_r10.bitmask |= 
            RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10 ;

        for(abs_count = RRM_ZERO;abs_count < MAX_SUBFRAME_PATTERN_FDD;abs_count++)
        {
            p_meas_config_object->meas_object.meas_object_eutra.
                meas_subframe_pattern_config_neigh_r10.
                meas_subframe_pattern_config_neigh_r10_setup.
                meas_subframe_pattern_neigh_r10.subframe_pattern_fdd_r10.data[abs_count] = ~(p_cell_ctxt->rrm_eicic_cell_info.
                        serving_meas_abs_pattern_applied[abs_count]);
        }

        /*eICIC Bug 816 Changes Start*/
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                "eICIC:Number of valid intra freq neighbours = [%d]",valid_intra_freq_count);
        if(RRM_PNULL != p_lte_ncl)
        {
            valid_intra_freq_count = p_lte_ncl->num_valid_intra_freq_cell;
        }
        for(count = RRM_ZERO; count < valid_intra_freq_count; count++)
        {
            p_meas_config_object->meas_object.meas_object_eutra.
                meas_subframe_pattern_config_neigh_r10.
                meas_subframe_pattern_config_neigh_r10_setup.bitmask |=
                MEAS_SUBFRAME_CELL_LIST_R10_PRESENT;


            p_meas_config_object->meas_object.meas_object_eutra.
                meas_subframe_pattern_config_neigh_r10.
                meas_subframe_pattern_config_neigh_r10_setup.
                meas_subframe_cell_list_r10.meas_subframe_cells[count].start = p_lte_ncl->intra_freq_cells[count].phy_cell_id;

            p_meas_config_object->meas_object.meas_object_eutra.
                meas_subframe_pattern_config_neigh_r10.
                meas_subframe_pattern_config_neigh_r10_setup.
                meas_subframe_cell_list_r10.meas_subframe_cells[count].presence_bitmask = 0;


        }
        if(MEAS_SUBFRAME_CELL_LIST_R10_PRESENT & p_meas_config_object->meas_object.meas_object_eutra.
                meas_subframe_pattern_config_neigh_r10.
                meas_subframe_pattern_config_neigh_r10_setup.bitmask)
        {
            p_meas_config_object->meas_object.meas_object_eutra.
                meas_subframe_pattern_config_neigh_r10.
                meas_subframe_pattern_config_neigh_r10_setup.
                meas_subframe_cell_list_r10.count = count;
        }

        /*eICIC Bug 816 Changes End*/

#else 
        p_meas_config_object->meas_object.meas_object_eutra.
            meas_subframe_pattern_config_neigh_r10.
            meas_subframe_pattern_config_neigh_r10_setup.
            meas_subframe_pattern_neigh_r10.bitmask |= 
            RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10 ;

        /*CID:83013 start*/
        abs_size = rrm_find_abs_frame_size(& p_cell_ctxt->ran_info.physical_layer_params.physical_layer_param_tdd_frame_structure);
        U8 serving_meas_abs_pattern_applied[abs_size];
        RRM_MEMSET(&serving_meas_abs_pattern_applied,RRM_ZERO, abs_size);
        /*CID:83013 end*/

        switch(abs_size)
        {
            case RRM_MAX_ABS_PATTERN_BYTE_TDD_0 :

            for(abs_count = RRM_ZERO;abs_count < abs_size;abs_count++)
            {
                p_meas_config_object->meas_object.meas_object_eutra.
                    meas_subframe_pattern_config_neigh_r10.
                    meas_subframe_pattern_config_neigh_r10_setup.
                    meas_subframe_pattern_neigh_r10.
                    subframe_pattern_tdd_r10.subframe_config0_r10[abs_count] = ~(p_cell_ctxt->rrm_eicic_cell_info.
                            serving_meas_abs_pattern_applied[abs_count]);
            }
            rrm_copy_tdd_bits(abs_size,p_meas_config_object->meas_object.meas_object_eutra.
                    meas_subframe_pattern_config_neigh_r10.
                    meas_subframe_pattern_config_neigh_r10_setup.
                    meas_subframe_pattern_neigh_r10.
                    subframe_pattern_tdd_r10.subframe_config0_r10);
            break;

            case RRM_MAX_ABS_PATTERN_BYTE_TDD_1_5 :
            for(abs_count = RRM_ZERO;abs_count < abs_size;abs_count++)
            {
                /*CID:83016 start*/
                serving_meas_abs_pattern_applied[abs_count]= ~(p_cell_ctxt->rrm_eicic_cell_info.
                        serving_meas_abs_pattern_applied[abs_count]);

            }

            RRM_MEMSET(p_meas_config_object->meas_object.meas_object_eutra.meas_subframe_pattern_config_neigh_r10.
                    meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_pattern_neigh_r10.
                    subframe_pattern_tdd_r10.subframe_config1_5_r10,RRM_ZERO,abs_size);

            rrm_copy_tdd_bits_from_src_abs_to_dest(abs_size,serving_meas_abs_pattern_applied,
                    p_meas_config_object->meas_object.meas_object_eutra.meas_subframe_pattern_config_neigh_r10.
                    meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_pattern_neigh_r10.
                    subframe_pattern_tdd_r10.subframe_config1_5_r10);
            /*CID:83016 end*/
            break;

            case RRM_MAX_ABS_PATTERN_BYTE_TDD_6 :
            for(abs_count = RRM_ZERO;abs_count < abs_size;abs_count++)
            {
                /*CID:83013 start*/
                serving_meas_abs_pattern_applied[abs_count]= ~(p_cell_ctxt->rrm_eicic_cell_info.
                        serving_meas_abs_pattern_applied[abs_count]);
            }

            RRM_MEMSET(p_meas_config_object->meas_object.meas_object_eutra.meas_subframe_pattern_config_neigh_r10.
                    meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_pattern_neigh_r10.
                    subframe_pattern_tdd_r10.subframe_config6_r10,RRM_ZERO,abs_size);
            rrm_copy_tdd_bits_from_src_abs_to_dest(abs_size,serving_meas_abs_pattern_applied,
                    p_meas_config_object->meas_object.meas_object_eutra.meas_subframe_pattern_config_neigh_r10.
                    meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_pattern_neigh_r10.
                    subframe_pattern_tdd_r10.subframe_config6_r10);
            /*CID:83013 end*/
            break;

            default :
            RRM_TRACE(g_uem_log_on_off,p_g_rrm_uem_facility_name,RRM_ERROR,
                    "Invalid TDD configuration [abs_frame_size=%d] has been given by OAM", abs_size);
        }
#endif        
    }
    /* eICIC_PHASE_1_2_CHANGES_END */
    /* SPR14140end */
    /*SPR 16879 CHANGES START*/
    fill_meas_intra_freq_neigh_black_cell_list(p_rrm_ue_context, p_meas_object_eutra);
    /*SPR 16879 CHANGES END*/
    RRM_UT_TRACE_EXIT();
}



/*spr_12273_fix*/
/****************************************************************************
 * Function Name  : rrm_check_if_new_meas_object_is_req_intra_freq 
 * Inputs         : p_ue_context,
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Checks if any cells are deleted from ncl after meas object creation 
 ****************************************************************************/
rrm_bool_et
rrm_check_if_new_meas_object_is_req_intra_freq
(
 /*SPR 17777 +-*/
 meas_report_info_node_t   *p_meas_report_node,
 cell_index_list_t         *cells_to_remove_list,
 meas_object_to_add_mod_t   *p_meas_config_object,
 /*SPR 13317:start*/
 rrm_cell_index_t           pcell_index
 /*SPR 13317:end*/

 )
{
    rrm_bool_et                  ret_val             = RRM_FALSE;
    lte_ncl_t              *p_ncl_list            = RRM_PNULL;
    U8         valid_cells      = RRM_ZERO;
    U8         ncl_cell_count       = RRM_ZERO;
    rrm_bool_et        pci_found= RRM_FALSE;
    U8 count = RRM_ZERO;
    /* SPR:12849 Start */ 
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node = RRM_PNULL;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node_to_del = RRM_PNULL;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mod = RRM_PNULL;
    rrm_cell_context_t *p_cell_ctx = RRM_PNULL;
    /* SPR:12849 End */ 
    U8 cell_index = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    /*Fill from NCL list*/
    /*SPR 13317:start*/
    p_ncl_list = rrm_cellm_get_lte_ncl_info(pcell_index);
    p_cell_ctx = rrm_cellm_get_cell_context(pcell_index); 
    /*SPR 13317:end*/
    if((RRM_PNULL != p_ncl_list) && (RRM_ZERO < p_ncl_list->num_valid_intra_freq_cell) && (RRM_PNULL != p_cell_ctx))
    {
        valid_cells  =  p_ncl_list->num_valid_intra_freq_cell;
        /*Traverse through the meas object node and compare the pci present in ncl*/
        /* SPR:12849 Start */ 
        p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) ylFirst(&(p_meas_report_node->cells_to_add_mod_list));
        while (p_cell_to_add_mode_node)
        {
            pci_found = RRM_FALSE; 
            for(ncl_cell_count = RRM_ZERO; (ncl_cell_count < valid_cells) && 
                    (ncl_cell_count < MAX_INTRA_FREQ_CELLS) ; ncl_cell_count++)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "pci meas mode[%d] nrt pci[%d]",p_cell_to_add_mode_node->phy_cell_id,
                        p_ncl_list->intra_freq_cells[ncl_cell_count].phy_cell_id);
                if ((p_cell_to_add_mode_node->phy_cell_id == p_ncl_list->intra_freq_cells[ncl_cell_count].phy_cell_id) &&
                        (p_meas_report_node->carrier_earfcn == p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn)) 
                {
                    pci_found = RRM_TRUE;
                    break;
                }
            }
            if(RRM_FALSE == pci_found)
            {
                /*Case where pci is deleted from ncl , here the cell index needs to be filled in cell remove list*/
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "For meas id[%d],cell index[%d] pci[%d] is deleted from ncl.",
                        p_meas_report_node->meas_id, p_cell_to_add_mode_node->cell_index,
                        p_cell_to_add_mode_node->phy_cell_id);

                /* SPR:12849 Start */ 
                cells_to_remove_list->cell_index[cells_to_remove_list->count] = p_cell_to_add_mode_node->cell_index;
                /*Delete the node from the list*/
                p_cell_to_add_mode_node_to_del = p_cell_to_add_mode_node;
                /* SPR:12849 End */ 
                cells_to_remove_list->count++;
                /*CID 65855:start*/
                ret_val = RRM_TRUE;
                /*CID 65855:end*/
                p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
                ylDelete(&(p_meas_report_node->cells_to_add_mod_list),(YLNODE *)p_cell_to_add_mode_node_to_del);
                RRM_MEM_FREE(p_cell_to_add_mode_node_to_del);
            }
            else
            {
                p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
            }
            /* UT Fix start*/
            if( RRM_NULL == p_cell_to_add_mode_node )
                break;
            /* UT Fix end*/

        }
        /*SPR:12841 start*/
        for(ncl_cell_count = RRM_ZERO; (ncl_cell_count < valid_cells) && 
                (ncl_cell_count < MAX_INTRA_FREQ_CELLS) ; ncl_cell_count++)
        {
            pci_found = RRM_FALSE;
            p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) ylFirst(&(p_meas_report_node->cells_to_add_mod_list));
            while (p_cell_to_add_mode_node)
            {
                if (p_ncl_list->intra_freq_cells[ncl_cell_count].phy_cell_id == p_cell_to_add_mode_node->phy_cell_id)
                {
                    pci_found = RRM_TRUE;
                    break;
                }
                cell_index = p_cell_to_add_mode_node->cell_index;
                p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
            }
            if (RRM_FALSE == pci_found)
            {
                p_cell_to_add_mod = (rrm_meas_obj_cell_to_add_info_t *)rrm_mem_get(sizeof(rrm_meas_obj_cell_to_add_info_t));
                if (RRM_PNULL != p_cell_to_add_mod)
                {
                    count = p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.count;
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[count].cell_index = 
                        cell_index + RRM_ONE; 
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[count].phys_cell_id =
                        p_ncl_list->intra_freq_cells[ncl_cell_count].phy_cell_id; 
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[count].cell_individual_offset = 
                        p_ncl_list->intra_freq_cells[ncl_cell_count].cio;
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.count++ ;
                    p_cell_to_add_mod->cell_index = cell_index + RRM_ONE; 
                    p_cell_to_add_mod->phy_cell_id = p_ncl_list->intra_freq_cells[ncl_cell_count].phy_cell_id; 
                    ylPushTail(&(p_meas_report_node->cells_to_add_mod_list),&( p_cell_to_add_mod->node));
                    ret_val = RRM_TRUE;
                }

            }
        }
        /*SPR:12841 end*/
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Either lte ncl list is null or valid intra freq cells are zero"
                );

    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* End CSR: 00058590 */
/*spr_12273_fix*/
/******************************************************************************
 *  FUNCTION NAME: fill_eutran_meas_object_for_cells_to_remove_list
 *  INPUT        : cells_to_remove_list 
 *  OUTPUT       : p_meas_config_object
 *  DESCRIPTION  : Copies the cells to remove list into the meas object 
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_eutran_meas_object_for_cells_to_remove_list(
        meas_object_to_add_mod_t   *p_meas_config_object,
        cell_index_list_t         *p_cells_to_remove_list
        )
{

    /*spr_12273_fix_start */
    U8    valid_count          = RRM_ZERO;
    U8    present_remove_count = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    present_remove_count = p_meas_config_object->meas_object.meas_object_eutra.cells_to_remove_list.count;

    if (RRM_ZERO != present_remove_count)
    {
        p_meas_config_object->meas_object.meas_object_eutra.bitmask |= MEAS_OBJECT_EUTRA_CELLS_TO_REMOVE_LIST_PRESENT;
    }
    for(valid_count =  present_remove_count ; valid_count < ( present_remove_count + p_cells_to_remove_list->count ) ; valid_count++)
    {
        p_meas_config_object->meas_object.meas_object_eutra.cells_to_remove_list.cell_index[valid_count] = p_cells_to_remove_list->cell_index[valid_count];

    }

    p_meas_config_object->meas_object.meas_object_eutra.cells_to_remove_list.count = valid_count;

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Cells to remove list count %d", valid_count );

    /*spr_12273_fix_end */

    RRM_UT_TRACE_EXIT();

    return;
}

/******************************************************************************
 *  FUNCTION NAME: fill_intra_freq_meas_config_object
 *  INPUT        : p_rrm_ue_context, index,eutran_supported
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the measurement object for intra frequency  eutran in 
 *                 meas config list with values confiugred from OAM while cell 
 *                 configuration.
 *  RETURNS      : None
 ******************************************************************************/

static rrm_void_t
fill_intra_freq_meas_config_object(
        rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        U8                           *index,
        /*start supported_rat*/
        rrm_bool_et                  eutran_supported,
        /*end supported_rat*/
        rrm_cell_index_t           cell_index
        )
{
    U8       meas_object_id      = RRM_ZERO;
    /* SPR 17424 Fix +- */

    RRM_UT_TRACE_ENTER();
    /* Start fix for SPR 8391:csg proximity */
    /* 
     ** This is intra freq case so do not configure measurements on the carrier 
     ** freq received in proximity ind if it is not same as serving cell freq.
     */
    rf_params_t         *p_rf_params         = RRM_PNULL;
    rrm_proximity_ind_t *p_rrm_proximity_ind = RRM_PNULL;
    /* SPR 17424 Fix +- */
    /* bug_13863_start */
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context(cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        return ;
    }
    /* bug_13863_end */

    /*spr_12273_fix_start*/
    cell_index_list_t     cells_to_remove_list;

    RRM_MEMSET(&cells_to_remove_list,RRM_ZERO,sizeof(cell_index_list_t) );
    /*spr_12273_fix_end*/

    p_rf_params = rrm_cellm_get_rf_params(cell_index);

    if(RRC_RRM_PROXIMITY_IND_EVENT == p_rrm_ue_context->ue_event)
    {
        p_rrm_proximity_ind = (rrm_proximity_ind_t *)
            p_rrm_ue_context->current_procedure.p_proc_data;
        /* Start fix for SPR 962 */
        if (!(p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Eutra Carrier freq is not received in proximity ind, so return from here"); 
            return;
        }
        else if ((p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT) && 
                (p_rrm_proximity_ind->carrier_Freq.carrier_freq_eutra != p_rf_params->rf_configurations.dl_earfcn))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Carrier freq received in proximity ind is different then serving cell = %d", 
                    p_rrm_proximity_ind->carrier_Freq.carrier_freq_eutra);
            return;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Eutra intra cell carrier freq is received in proximity ind"); 
        }
        /* End fix for SPR 962 */
    }
    /* End fix for SPR 8391 */

    p_meas_config->meas_object_to_add_mod_list.count = RRM_ONE;
    /* SPR 17424 Fix Start */
    meas_object_id = HO_MEAS_START_ID;
    /* SPR 17424 Fix End */

    /* SPR 15608 Start */
    p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
    /* SPR 15608 End */


    if(RRM_TRUE == eutran_supported)
    {    
        /* SPR 17424 Fix Start */
        p_meas_config-> meas_object_to_add_mod_list.meas_object_to_add_mod[*index].\
            meas_object_id = meas_object_id;
        /* SPR 17424 Fix End */
        fill_eutran_meas_object_to_add_mod_list_for_ho(&(p_meas_config->
                    meas_object_to_add_mod_list.meas_object_to_add_mod[*index]),
                p_rrm_ue_context,cell_index);

        /* SPR 17424 Fix +- */

    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                "eUtran rat is not supported by supported_rat param and spid for [UE:%d]", p_rrm_ue_context->ue_index);
    }

    /* Klocwork changes end 22 may */
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_start*/
/******************************************************************************
 *  FUNCTION NAME: fill_meas_inter_freq_neigh_black_list
 *  INPUT        : p_rrm_ue_context,carrier_freq
 *  OUTPUT       : p_meas_object_eutra
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with
 *                 values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_meas_inter_freq_neigh_black_list(lte_ncl_t               *p_lte_ncl ,
        rrm_ue_context_t        *p_rrm_ue_context , 
        meas_object_eutra_t     *p_meas_object_eutra ,
        U8                      cell_count ,
        U8                      *black_count ,
        U8                      *neigh_count)
{
    U8 index = RRM_ZERO;
    /* coverity_fix start- 67392 */
    rrm_bool_et pci_already_added = RRM_FALSE;
    /* coverity_fix end - 67392*/
    YLNODE   *p_node = RRM_PNULL;
    meas_report_info_node_t   *p_meas_report_node = RRM_PNULL;
    rrm_bool_et pci_found = RRM_FALSE;
    U8 cell_to_add_cell_index = RRM_ZERO;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mod = RRM_PNULL;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    if(p_lte_ncl->inter_freq_cells[cell_count].blacklisted == RRM_FALSE)
    {
        /* ho_restriction at meas config */ 
        /* SPR 17564 Fix Start */
        if(RRM_TRUE == rrm_is_eutran_cell_restricted(p_rrm_ue_context,
                    /* SPR 17564 Fix End */
                    p_lte_ncl->inter_freq_cells[cell_count].cell_id.primary_plmn_id,
                    p_lte_ncl->inter_freq_cells[cell_count].tac,
                    &p_rrm_ue_context->ho_restriction_list))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "Inter frequency Neighbor cell = %d, is restricted",p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id);
        }
        else
        {
            /*SPR 13331:start*/
            p_node = ylFirst(&(p_rrm_ue_context->meas_report_list));
            while(p_node)
            {

                p_meas_report_node = (meas_report_info_node_t*)p_node;
                if(p_meas_report_node->carrier_earfcn == p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn)
                {
                    break;
                }
                p_node = ylNext(p_node);
            }
            if (RRM_PNULL == p_node)
            {
                for(index = RRM_ZERO;index< (*neigh_count); index++)
                {
                    if ( p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id ==
                            p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[index].phys_cell_id)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Inter frequency pci [%d] present in cells to add mode,",
                                p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id);
                        pci_already_added = RRM_TRUE;
                    }
                }
                if (RRM_FALSE == pci_already_added)
                {
                    p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].\
                        cell_index =  (*neigh_count)+RRM_ONE;

                    p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].\
                        phys_cell_id = p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id;

                    p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].\
                        cell_individual_offset = p_lte_ncl->inter_freq_cells[cell_count].cio;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Intra frequency Neighbor cell = %d, is added to list",
                            p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id);
                    (*neigh_count)++;
                }
            }
            else
            {
                p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) ylFirst(&(p_meas_report_node->cells_to_add_mod_list));
                while (p_cell_to_add_mode_node)
                {
                    if (p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id == p_cell_to_add_mode_node->phy_cell_id)
                    {
                        pci_found = RRM_TRUE;
                        break;
                    }
                    cell_to_add_cell_index = p_cell_to_add_mode_node->cell_index;
                    p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
                }
                if (RRM_FALSE == pci_found)
                {
                    p_cell_to_add_mod = (rrm_meas_obj_cell_to_add_info_t *)rrm_mem_get(sizeof(rrm_meas_obj_cell_to_add_info_t));
                    if (RRM_PNULL != p_cell_to_add_mod)
                    {
                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].cell_index = 
                            cell_to_add_cell_index + RRM_ONE; 
                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].phys_cell_id =
                            p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id; 
                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[(*neigh_count)].cell_individual_offset = 
                            p_lte_ncl->inter_freq_cells[cell_count].cio;
                        p_meas_object_eutra->cells_to_add_mod_list.count++ ;
                        p_cell_to_add_mod->cell_index = cell_to_add_cell_index + RRM_ONE; 
                        p_cell_to_add_mod->phy_cell_id = p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id; 
                        ylPushTail(&(p_meas_report_node->cells_to_add_mod_list),&( p_cell_to_add_mod->node));
                        (*neigh_count)++;
                    }

                }
            }
            /*SPR 13331:end*/
        }

        /* HO Oscillation START */
        /* SPR 15804 Fix Start */
        rrm_ho_osc_prevention(p_rrm_ue_context, 
                &p_lte_ncl->inter_freq_cells[cell_count].cell_id, 
                /*cov 88523 +*/
        /*CID 112446 Fix Start*/
                (rrm_q_offsetrange_et *)(&p_lte_ncl->inter_freq_cells[cell_count].cio));
        /*CID 112446 Fix Stop*/
        /* SPR 15804 Fix Stop */
        /*cov 88523 -*/
        /* HO Oscillation END */

    }
    else if(p_lte_ncl->inter_freq_cells\
            [cell_count].blacklisted == RRM_TRUE )
    {
        p_meas_object_eutra->black_cells_to_add_mod_list.\
            black_cells_to_add_mod[(*black_count)].cell_index = \
            (*black_count) + RRM_ONE;

        p_meas_object_eutra->black_cells_to_add_mod_list.\
            black_cells_to_add_mod[(*black_count)].phys_cell_id_range.\
            presence_bitmask = RRM_ZERO;

        p_meas_object_eutra->black_cells_to_add_mod_list.\
            black_cells_to_add_mod[(*black_count)].phys_cell_id_range.\
            start = p_lte_ncl->
            inter_freq_cells[cell_count]. phy_cell_id;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Inter frequency Neighbor black cell = %d, is added to list",p_lte_ncl->
                inter_freq_cells[cell_count]. phy_cell_id);
        (*black_count)++;
    }
    //coverity_fix_req
    RRM_MEM_FREE(p_cell_to_add_mod);
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_end*/

/******************************************************************************
 *  FUNCTION NAME: fill_meas_inter_freq_neigh_black_cell_list
 *  INPUT        : p_rrm_ue_context,carrier_freq
 *  OUTPUT       : p_meas_object_eutra
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with 
 *                 values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
/*SPR 16879 CHANGES START*/

    static rrm_void_t
fill_meas_inter_freq_neigh_black_cell_list(
        meas_object_eutra_t       *p_meas_object_eutra,
        rrm_ue_context_t            *p_rrm_ue_context,
        U16                        carrier_freq
        )
{
    RRM_UT_TRACE_ENTER();
    U8         valid_cells      = RRM_ZERO;
    U8         black_count      = RRM_ZERO;
    U8         neigh_count      = RRM_ZERO;
    U16        neigh_carrier_freq     = RRM_ZERO;
    U8         cell_count       = RRM_ZERO;
    U16        phys_cell_id      = RRM_NULL;
    lte_ncl_t    *p_lte_ncl       = RRM_PNULL;
    p_lte_ncl              = rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
    /* SPR-20552 START */
        /*CID 112446 Fix Start*/
    rrm_q_offsetrange_et cell_individual_offset = RRM_Q_OFFSET_RANGE_DB_24;
        /*CID 112446 Fix Stop*/
    /* SPR-20552 END */

    if(RRM_PNULL != p_lte_ncl) /* Coverity: CID 29578 */
    {
        valid_cells            =  p_lte_ncl-> num_valid_inter_freq_cell;


        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "For Inter frequency %d Neighbor cells and black cells",carrier_freq);

        for(cell_count=RRM_ZERO; cell_count < valid_cells; cell_count++)
        {
            neigh_carrier_freq = p_lte_ncl->
                inter_freq_cells[cell_count].eutra_carrier_arfcn;
            if ( neigh_carrier_freq == carrier_freq)
            {
                if(p_lte_ncl->inter_freq_cells[cell_count].blacklisted == RRM_FALSE)
                {
                    /* ho_restriction at meas config */ 
                    /* SPR 17564 Fix Start */
                    if(RRM_TRUE == rrm_is_eutran_cell_restricted(p_rrm_ue_context,
                                /* SPR 17564 Fix End */
                                p_lte_ncl->inter_freq_cells[cell_count].cell_id.primary_plmn_id,
                                p_lte_ncl->inter_freq_cells[cell_count].tac,
                                &p_rrm_ue_context->ho_restriction_list))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Inter frequency Neighbor cell = %d, is restricted",p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id);
                    }
                    else
                    {
                        phys_cell_id = p_lte_ncl->inter_freq_cells
                            [cell_count].phy_cell_id;

                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[neigh_count].\
                            cell_index =  neigh_count +RRM_ONE;

                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[neigh_count].\
                            phys_cell_id = p_lte_ncl->
                            inter_freq_cells[cell_count].phy_cell_id;

                        /* SPR-20552 START */
                            cell_individual_offset = p_lte_ncl->inter_freq_cells[cell_count].cio;
                            rrm_ho_osc_prevention(p_rrm_ue_context, 
                                    &p_lte_ncl->inter_freq_cells[cell_count].cell_id, 
                                    &cell_individual_offset);

                        p_meas_object_eutra->cells_to_add_mod_list.cells_to_add_mod[neigh_count].\
                                cell_individual_offset = cell_individual_offset;
                        /* SPR-20552 END */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Inter frequency Neighbor cell = %d, is added to list",phys_cell_id);

                        neigh_count++;
                    }

                    /* SPR-20552 START */
                    /* code deleted */
                    /* SPR-20552 END */

                }
                else if(p_lte_ncl->inter_freq_cells\
                        [cell_count].blacklisted == RRM_TRUE )
                {
                    p_meas_object_eutra->black_cells_to_add_mod_list.\
                        black_cells_to_add_mod[black_count].cell_index = \
                        black_count + RRM_ONE;

                    p_meas_object_eutra->black_cells_to_add_mod_list.\
                        black_cells_to_add_mod[black_count].phys_cell_id_range.\
                        presence_bitmask = RRM_ZERO;

                    p_meas_object_eutra->black_cells_to_add_mod_list.\
                        black_cells_to_add_mod[black_count].phys_cell_id_range.\
                        start = p_lte_ncl->
                        inter_freq_cells[cell_count]. phy_cell_id;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Inter frequency Neighbor black cell = %d, is added to list",p_lte_ncl->
                            inter_freq_cells[cell_count]. phy_cell_id);
                    black_count++;
                }

            }
        }
        if (neigh_count != RRM_ZERO)
        {
            p_meas_object_eutra->bitmask |= MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT;
            p_meas_object_eutra->cells_to_add_mod_list.count       = neigh_count;
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Total Number of Inter frequency Neighbor cells = %d configured for Inter freq = %d",neigh_count, carrier_freq);
        }
        if (black_count != RRM_ZERO)
        {
            p_meas_object_eutra->bitmask |=MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT;
            p_meas_object_eutra->black_cells_to_add_mod_list.count = black_count;
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Total Number of Inter frequency Neighbor black cells = %d configured for Inter freq = %d",neigh_count, carrier_freq);
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "NCL list is empty");
    }
    RRM_UT_TRACE_EXIT();
}

/* SPR 15776 Fix Start */
S32 rrm_offset_map(rrm_q_offsetrange_et offset)
{
    S32 ret_val = RRM_ZERO;

    switch (offset)
    {
        case RRM_Q_OFFSET_RANGE_DB_24:
            ret_val = RRM_MINUS_TWENTY_FOUR;
            break;

        case RRM_Q_OFFSET_RANGE_DB_22:
            ret_val = RRM_MINUS_TWENTY_TWO;
            break;

        case RRM_Q_OFFSET_RANGE_DB_20:
            ret_val = RRM_MINUS_TWENTY;
            break;

        case RRM_Q_OFFSET_RANGE_DB_18:
            ret_val = RRM_MINUS_EIGHTEEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB_16:
            ret_val = RRM_MINUS_SIXTEEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB_14:
            ret_val = RRM_MINUS_FOURTEEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB_12:
            ret_val = RRM_MINUS_TWELVE;
            break;

        case RRM_Q_OFFSET_RANGE_DB_10:
            ret_val = RRM_MINUS_TEN;
            break;

	case RRM_Q_OFFSET_RANGE_DB_8:
            ret_val = RRM_MINUS_EIGHT;
            break;

        case RRM_Q_OFFSET_RANGE_DB_6:
            ret_val = RRM_MINUS_SIX;
            break;

        case RRM_Q_OFFSET_RANGE_DB_5:
            ret_val = RRM_MINUS_FIVE;
            break;

        case RRM_Q_OFFSET_RANGE_DB_4:
            ret_val = RRM_MINUS_FOUR;
            break;

        case RRM_Q_OFFSET_RANGE_DB_3:
            ret_val = RRM_MINUS_THREE;
            break;

        case RRM_Q_OFFSET_RANGE_DB_2:
            ret_val = RRM_MINUS_TWO;
            break;

        case RRM_Q_OFFSET_RANGE_DB_1:
            ret_val = RRM_MINUS_ONE;
            break;

        case RRM_Q_OFFSET_RANGE_DB0:
            ret_val = RRM_ZERO;
            break;

        case RRM_Q_OFFSET_RANGE_DB1:
            ret_val = RRM_ONE;
            break;

        case RRM_Q_OFFSET_RANGE_DB2:
            ret_val = RRM_TWO;
            break;

        case RRM_Q_OFFSET_RANGE_DB3:
            ret_val = RRM_THREE;
            break;

        case RRM_Q_OFFSET_RANGE_DB4:
            ret_val = RRM_FOUR;
            break;

        case RRM_Q_OFFSET_RANGE_DB5:
            ret_val = RRM_FIVE;
            break;

        case RRM_Q_OFFSET_RANGE_DB6:
            ret_val = RRM_SIX;
            break;

        case RRM_Q_OFFSET_RANGE_DB8:
            ret_val = RRM_EIGHT;
            break;

        case RRM_Q_OFFSET_RANGE_DB10:
            ret_val = RRM_TEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB12:
            ret_val = RRM_TWELVE;
            break;

        case RRM_Q_OFFSET_RANGE_DB14:
            ret_val = RRM_FOURTEEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB16:
            ret_val = RRM_SIXTEEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB18:
            ret_val = RRM_EIGHTEEN;
            break;

        case RRM_Q_OFFSET_RANGE_DB20:
            ret_val = RRM_TWENTY;
            break;

        case RRM_Q_OFFSET_RANGE_DB22:
            ret_val = RRM_TWENTY_TWO;
            break;

        case RRM_Q_OFFSET_RANGE_DB24:
            ret_val = RRM_TWENTY_FOUR;
            break;

	default:
	    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
	              "INVALID VALUE RECEIVED!! DEFAULT VALUE RRM_ZERO ASSIGNED");
            break;
    }
    return ret_val;
}

/* SPR-20552 START */
/******************************************************************************
 *  FUNCTION NAME: rrm_new_cio
 *  INPUT        : cio
 *  OUTPUT       : None
 *  DESCRIPTION  : Finds the cio value from enum set corresponding to calculated value.
 *  RETURNS      : ret_val
 ******************************************************************************/
        /*CID 112446 Fix Start*/
rrm_q_offsetrange_et rrm_new_cio(S32 cio)
{
    rrm_q_offsetrange_et ret_val = RRM_Q_OFFSET_RANGE_DB_24;
        /*CID 112446 Fix Stop*/
/*CID 112446 Fix Stop*/

    if((RRM_TWENTY_FOUR < cio) || (RRM_MINUS_TWENTY_FOUR > cio))
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "INVALID VALUE RECEIVED for cio = %d!! DEFAULT VALUE RRM_Q_OFFSET_RANGE_DB_24ASSIGNED", cio);
    }
    else
    {
        if(RRM_MINUS_TWENTY_FOUR == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_24;
        }
        else if((RRM_MINUS_TWENTY_FOUR < cio) && (RRM_MINUS_TWENTY_TWO >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_22;
        }
        else if((RRM_MINUS_TWENTY_TWO < cio) && (RRM_MINUS_TWENTY >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_20;
        }
        else if((RRM_MINUS_TWENTY < cio) && (RRM_MINUS_EIGHTEEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_18;
        }
        else if((RRM_MINUS_EIGHTEEN < cio) && (RRM_MINUS_SIXTEEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_16;
        }
        else if((RRM_MINUS_SIXTEEN < cio) && (RRM_MINUS_FOURTEEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_14;
        }
        else if((RRM_MINUS_FOURTEEN < cio) && (RRM_MINUS_TWELVE >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_12;
        }
        else if((RRM_MINUS_TWELVE < cio) && (RRM_MINUS_TEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_10;
        }
        else if((RRM_MINUS_TEN < cio) && (RRM_MINUS_EIGHT >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_8;
        }
        else if((RRM_MINUS_EIGHT < cio) && (RRM_MINUS_SIX >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_6;
        }
        else if(RRM_MINUS_FIVE == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_5;
        }
        else if(RRM_MINUS_FOUR == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_4;
        }
        else if(RRM_MINUS_THREE == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_3;
        }
        else if(RRM_MINUS_TWO == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_2;
        }
        else if(RRM_MINUS_ONE == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB_1;
        }
        else if(RRM_ZERO == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB0;
        }
        else if(RRM_ONE == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB1;
        }
        else if(RRM_TWO == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB2;
        }
        else if(RRM_THREE == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB3;
        }
        else if(RRM_FOUR == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB4;
        }
        else if(RRM_FIVE == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB5;
        }
        else if(RRM_SIX == cio)
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB6;
        }
        else if((RRM_SIX < cio) && (RRM_EIGHT >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB8;
        }
        else if((RRM_EIGHT < cio) && (RRM_TEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB10;
        }
        else if((RRM_TEN < cio) && (RRM_TWELVE >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB12;
        }
        else if((RRM_TWELVE < cio) && (RRM_FOURTEEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB14;
        }
        else if((RRM_FOURTEEN < cio) && (RRM_SIXTEEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB16;
        }
        else if((RRM_SIXTEEN < cio) && (RRM_EIGHTEEN >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB18;
        }
        else if((RRM_EIGHTEEN < cio) && (RRM_TWENTY >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB20;
        }
        else if((RRM_TWENTY < cio) && (RRM_TWENTY_TWO >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB22;
        }
        else if((RRM_TWENTY_TWO < cio) && (RRM_TWENTY_FOUR >= cio))
        {
            ret_val = RRM_Q_OFFSET_RANGE_DB24;
        }
    }
    return ret_val;
}
/* SPR-20552 END */

/* HO Oscillation START */
/******************************************************************************
 *  FUNCTION NAME: rrm_ho_osc_prevention 
 *  INPUT        : p_rrm_ue_context,p_cgi_selected_cell,cell_individual_offset
 *  OUTPUT       : none 
 *  DESCRIPTION  : Prevents HO Oscillation. This function returns an offset for 
 *                 cell_individual_offset by which the cio shuold be decreased
 *                 on detection of an HO Oscilltion loop on the selected cell.
 *  RETURNS      : rrm_q_offsetrange_et : Cell Individual Offset 
 ******************************************************************************/

/* SPR 15804 Fix Start */
rrm_void_t
rrm_ho_osc_prevention (
        rrm_ue_context_t          *p_rrm_ue_context,
        rrm_oam_eutran_global_cell_id_t *p_cgi_selected_cell,
        /*CID 112446 Fix Start*/
        rrm_q_offsetrange_et *cell_individual_offset
        /*CID 112446 Fix Stop*/
        )
/* SPR 15804 Fix Stop */
{
    RRM_UT_TRACE_ENTER();
    U8 index = RRM_ZERO;
    last_visited_cell_list_t *p_last_visited_cell_list = RRM_PNULL;
    rrm_cell_context_t  *p_cell_context         = RRM_PNULL;
    cell_global_id_eutra_t *p_global_cell_id    = RRM_PNULL;
    /* SPR 15776 Fix Start */
    /* SPR-20552 START */
    U8	cio_step_cfg       			= RRM_ZERO;
    S8  cio                 		= rrm_offset_map(*cell_individual_offset);
    /* SPR-20552 END */
    /* SPR 15776 Fix Stop */

    if(RRM_TRUE == p_rrm_ue_context->ho_params.osc_loop_detected)
    {
        p_last_visited_cell_list = &p_rrm_ue_context->ue_history.last_visited_cell_list;
        p_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
        /*CID 112424 Fix Start*/
        if (RRM_PNULL == p_cell_context)
        /*CID 112424 Fix Stop*/
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "p_cell_context is NULL for cell_index=%u",p_rrm_ue_context->cell_index);
            RRM_UT_TRACE_EXIT();
            return;
        }

        /* SPR-20552 START */
        cio_step_cfg = p_cell_context->operator_info.ho_oscillation_cfg.ho_oscillation_cio_step;

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "HO Oscillation Prevention : cio_offset read from cfg = %d\
                Number of cells in loop = %d",cio_step_cfg, p_rrm_ue_context->ho_params.osc_loop_count);
        /*Looping on the ue_history cell list on which the Oscillation 
          loop is detected at the time of HO Admission */
        /* SPR-20552 END */

        for(index = RRM_ZERO; index < p_rrm_ue_context->ho_params.osc_loop_count; index++)
        {
            if( p_last_visited_cell_list->last_visited_cell_info[index].bitmask & LAST_VISITED_CELL_INFO_E_UTRAN_CELL_INFORMATION_PRESENT )
            {
                p_global_cell_id = &p_last_visited_cell_list->last_visited_cell_info[index].last_visited_e_utran_cell_information.global_cell_id;
            }
            else if( p_last_visited_cell_list->last_visited_cell_info[index].bitmask & LAST_VISITED_CELL_INFO_UTRAN_CELL_INFORMATION_PRESENT )
            {
                /* SPR-20552 START */
                continue;
                /* SPR-20552 END */
            }
            /* Coverity 61367 Fix Start */
            if (PNULL != p_global_cell_id)
            {
                if(RRM_SUCCESS == rrm_compare_global_cell_id(p_global_cell_id, p_cgi_selected_cell))
                {
                    /* SPR-20552 START */
                    /* Decreasing the cio by step value read from cfg file on the cell for which loop is detected*/
                    /* code deleted */
                    if(RRM_ZERO != cio_step_cfg)
                    {
                        cio = cio - cio_step_cfg;
                        *cell_individual_offset = rrm_new_cio(cio);
                        cio                		= rrm_offset_map(*cell_individual_offset);

                        /* SPR-20552 END */

                        /* SPR 15804 Fix Start */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Preventing HO Oscillation, \
                                Setting cell_individual_offset = %d", cio);
                        /* SPR 15804 Fix Stop */
                        /* SPR-20552 START */
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "ho_oscillation_cio_step is set zero so, not preventing HO Oscillation");
                    }
                    /* SPR-20552 END */
                    break;

                }        
            }
            /* Coverity 61367 Fix Start */
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
            }
        }
    }
    /*CID 65829:end*/
    RRM_UT_TRACE_EXIT();
   /* SPR 15804 Fix Start */
   // Code Deleted;
   /* SPR 15804 Fix Stop */
}
/* HO Oscillation END */

#ifndef __EUTRA__FREQUENCY_MAPPING__
#define __EUTRA__FREQUENCY_MAPPING__

typedef struct _eutra_freq_band_mapping_info
{
    U8               freq_band;
    U32             lower_freq;
    U32             upper_freq;
} eutra_freq_band_mapping_info_t;

eutra_freq_band_mapping_info_t eutra_dl_freq_band_mapping[] =
{
    {  1  ,  0     ,  599   },
    {  2  ,  600   ,  1199  },
    {  3  ,  1200  ,  1949  },
    {  4  ,  1950  ,  2399  },
    {  5  ,  2400  ,  2649  },
    {  6  ,  2650  ,  2749  },
    {  7  ,  2750  ,  3449  },
    {  8  ,  3450  ,  3799  },
    {  9  ,  3800  ,  4149  },
    {  10  ,  4150  ,  4749  },
    {  11  ,  4750  ,  4949  },
    {  12  ,  5010  ,  5179  },
    {  13  ,  5180  ,  5279  },
    {  14  ,  5280  ,  5379  },
    {  17  ,  5730  ,  5849  },
    {  18  ,  5850  ,  5999  },
    {  19  ,  6000  ,  6149  },
    {  20  ,  6150  ,  6449  },
    {  21  ,  6450  ,  6599  },
    /* SPR 14326 Fix Start */
    {  25  ,  8040  ,  8689  },
    /* SPR 14326 Fix End */
    {  33  ,  36000  ,  36199  },
    {  34  ,  36200  ,  36349  },
    {  35  ,  36350  ,  36949  },
    {  36  ,  36950  ,  37549  },
    {  37  ,  37550  ,  37749  },
    {  38  ,  37750  ,  38249  },
    {  39  ,  38250  ,  38649  },
    /*CSR 70228 Fix Start */
    {  40  ,  38650  ,  39649  },
    {  41  ,  39650  ,  41589  },
    {  42  ,  41590  ,  43589  },
    {  43  ,  43590  ,  45589  },
    {  44  ,  45590  ,  46589  }
   /*CSR 70228 Fix End */
    /* LAA Rel13 Code + */
    /* LAA Rel13 Code - */
};

eutra_freq_band_mapping_info_t eutra_ul_freq_band_mapping[] =
{
    { 1 , 18000 , 18599 },
    { 2 , 18600 , 19199 },
    { 3 , 19200 , 19949 },
    { 4 , 19950 , 20399 },
    { 5 , 20400 , 20649 },
    { 6 , 20650 , 20749 },
    { 7 , 20750 , 21449 },
    { 8 , 21450 , 21799 },
    { 9 , 21800 , 22149 },
    { 10 , 22150 , 22749 },
    { 11 , 22750 , 22949 },
    { 12 , 23010 , 23179 },
    { 13 , 23180 , 23279 },
    { 14 , 23280 , 23379 },
    { 17 , 23730 , 23849 },
    { 18 , 23850 , 23999 },
    { 19 , 24000 , 24149 },
    { 20 , 24150 , 24449 },
    { 21 , 24450 , 24599 },
    { 33 , 36000 , 36199 },
    { 34 , 36200 , 36349 },
    { 35 , 36350 , 36949 },
    { 36 , 36950 , 37549 },
    { 37 , 37550 , 37749 },
    { 38 , 37750 , 38249 },
    { 39 , 38250 , 38649 },
    /*CSR 70228 Fix Start */
    { 40 , 38650 , 39649 },
    { 41 , 39650 , 41589 },
    { 42 , 41590 , 43589 },
    { 43 , 43590 , 45589 },
    { 44 , 45590 , 46589 }
    /*CSR 70228 Fix End */
    /* LAA Rel13 Code + */
    /* LAA Rel13 Code - */
};
/* BLR Inter RAT changes Start */

typedef struct _utra_freq_band_mapping_info
{
    U8               freq_band;
    U16             lower_freq;
    U16             upper_freq;
} utra_freq_band_mapping_info_t;

utra_freq_band_mapping_info_t utra_freq_band_mapping[] =
{
    { 1 , 18000 , 18599 },
    { 2 , 18600 , 19199 },
    { 3 , 19200 , 19949 },
    { 4 , 19950 , 20399 },
    { 5 , 20400 , 20649 },
    { 6 , 20650 , 20749 },
    { 7 , 20750 , 21449 },
    { 8 , 21450 , 21799 },
    { 9 , 21800 , 22149 },
    { 10 , 22150 , 22749 },
    { 11 , 22750 , 22949 },
    { 12 , 23010 , 23179 },
    { 13 , 23180 , 23279 },
    { 14 , 23280 , 23379 },
    { 17 , 23730 , 23849 },
    { 18 , 23850 , 23999 },
    { 19 , 24000 , 24149 },
    { 20 , 24150 , 24449 },
    { 21 , 24450 , 24599 },
    { 33 , 36000 , 36199 },
    { 34 , 36200 , 36349 },
    { 35 , 36350 , 36949 },
    { 36 , 36950 , 37549 },
    { 37 , 37550 , 37749 },
    { 38 , 37750 , 38249 },
    { 39 , 38250 , 38649 },
    { 40 , 38650 , 39649 }
};
/******************************************************************************
 *  FUNCTION NAME  : rrm_ue_get_freq_specific_band
 *  INPUT          : frequency 
 *  OUTPUT         : none
 *  DESCRIPTION    : Verifies if the UE supports frequency
 *  RETURNS        : Returns freuency band corresponding to the frequency if 
 *                   supported otherwise returns 0
 ******************************************************************************/
    U8
rrm_ue_get_freq_specific_band (
        U16 frequency
        )
{
    U8               freq_band     = RRM_ZERO;
    U32              num_elems     = RRM_ZERO;             
    U32              counter       = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(utra_freq_band_mapping)/
        sizeof(utra_freq_band_mapping_info_t);
    for(counter  = RRM_ZERO; counter < num_elems ; counter++) 
    {
        if ((frequency >= utra_freq_band_mapping[counter].lower_freq) &&
                (frequency <= utra_freq_band_mapping[counter].upper_freq)) 
        {
            freq_band = utra_freq_band_mapping[counter].freq_band;
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
    return freq_band;
}

/* BLR Inter RAT changes End */


/******************************************************************************
 *  FUNCTION NAME: rrm_ue_get_dl_freq_specific_band
 *  INPUT        : frequency
 *  OUTPUT       : none
 *  DESCRIPTION  : Verifies if the UE supports the downlink frequency and returns band
 *  RETURNS      : Returns freuency band corresponding to the frequency if supported
 *                 otherwise 0
 ******************************************************************************/
    U8
rrm_ue_get_dl_freq_specific_band (
        U32 frequency
        )
{
    /* SPR 14326 Fix Start */
    U8               freq_band = RRM_INVALID_RETURN;
    /* SPR 14326 Fix End */
    U32             num_elems = RRM_ZERO;             
    U32 i=RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(eutra_dl_freq_band_mapping)/
        sizeof(eutra_freq_band_mapping_info_t);
    for(i = RRM_ZERO;i<num_elems;i++) {
        if ((frequency>=eutra_dl_freq_band_mapping[i].lower_freq) &&
                (frequency<=eutra_dl_freq_band_mapping[i].upper_freq)) {
            freq_band =eutra_dl_freq_band_mapping[i].freq_band;
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
    return freq_band;
}

/******************************************************************************
 *  FUNCTION NAME: rrm_ue_get_ul_freq_specific_band
 *  INPUT        : frequency
 *  OUTPUT       : none
 *  DESCRIPTION  : Verifies if the UE supports the uplink frequency and returns band
 *  RETURNS      : Returns freuency band corresponding to the frequency if supported
 *                 otherwise 0
 ******************************************************************************/
    U8
rrm_ue_get_ul_freq_specific_band (
        U16 frequency
        )
{
    U8               freq_band = RRM_ZERO;
    U32             num_elems = RRM_ZERO;             
    U32 i=RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(eutra_ul_freq_band_mapping)/
        sizeof(eutra_freq_band_mapping_info_t);
    for(i = RRM_ZERO;i<num_elems;i++) {
        if ((frequency>=eutra_ul_freq_band_mapping[i].lower_freq) &&
                (frequency<=eutra_ul_freq_band_mapping[i].upper_freq)) {
            freq_band =eutra_ul_freq_band_mapping[i].freq_band;
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
    return freq_band;
}

/******************************************************************************
 *  FUNCTION NAME: rrm_ue_is_ue_supported_eutra_frequency
 *  INPUT        : p_ue_context,frequency
 *  OUTPUT       : none
 *  DESCRIPTION  : Verifies if the UE supports the frequency band 
 *  RETURNS      : Returns Freq_band of frequnecy if UE supports otherwise RRM_ZERO
 ******************************************************************************/
/* SPR 12897 Start */
U8
/* SPR 12897 End */
rrm_ue_is_ue_supported_eutra_frequency (
        U32 frequency,
        rrm_ue_context_t* p_ue_context
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    U8               freq_band = RRM_ZERO;
    /*SPR 524 fix start*/
    rrm_ue_supported_band_list_eutra_t* p_ue_supported_eutra_bands = RRM_PNULL;
    /*SPR 524 fix end*/
    U16  i = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    /*SPR 19961 Changes Start*/
    if (RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT & 
            p_ue_context->ue_capability_params.bitmask) {
    /*SPR 19961 Changes end*/
        /*SPR 524 fix start*/
        p_ue_supported_eutra_bands = (rrm_ue_supported_band_list_eutra_t*)&(p_ue_context->\
                ue_capability_params.rrm_eutra_radio_capability_info.\
                eutra_radio_capability_info_def.rf_parameters.supported_band_list_eutra);
        /*SPR 524 fix end*/

        for(i=0;i<p_ue_supported_eutra_bands->count;i++) {
            freq_band = rrm_ue_get_dl_freq_specific_band (frequency);
	    /*SPR 19075 Start*/
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Checking Frequency(%u), Frequency band (%u), UE Supported Bands (%u)",frequency,freq_band,
                            p_ue_supported_eutra_bands->supported_band_eutra[i].band_eutra);
            /*SPR 19075 end*/
            if (p_ue_supported_eutra_bands->supported_band_eutra[i].band_eutra == freq_band) {
                ret_val = RRM_TRUE;
                break;
            }
        }
    }
    if (RRM_TRUE != ret_val)
    {
        /* SPR 12897 Start */
	/* SPR 19075 Fix Start */
        freq_band = RRM_ZERO;
        /* SPR 19075 Fix End */
        /* SPR 12897 End */
	/*SPR 19075 End*/
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "Frequency(%u) is not Supported by UE",frequency);
    }

    RRM_UT_TRACE_EXIT();
    /* SPR 12897 Start */
    /* SPR 18757 Fix Start */
    /* SPR 19075 Fix Start */
    return freq_band;
    /* SPR 19075 Fix End */
    /* SPR 18757 Fix End */
    /* SPR 12897 End */
}
#endif /* __EUTRA__FREQUENCY_MAPPING__ */

#ifndef __UTRA__FREQUENCY_MAPPING__
#define __UTRA__FREQUENCY_MAPPING__
typedef struct _utran_dl_freq_band_mapping {

    unsigned char band;
    short              dl_offset;
    float               dl_low;
    float               dl_high;
}utran_dl_freq_band_mapping_t;

static utran_dl_freq_band_mapping_t utra_dl_freq_band_info[] =
{
    /* CSR 56128: code changes start */
    /*Acording to table specified in 25.101 Table 5.1: UARFCN definition (general)*/
    {           RRM_UE_UTRA_FDD_BANDI,           0              ,2112.4  ,               2167.6   },
    {           RRM_UE_UTRA_FDD_BANDII,           0              ,1932.4  ,               1987.6   },
    {           RRM_UE_UTRA_FDD_BANDIII,           1575,     1807.4   ,               1877.6   },
    {           RRM_UE_UTRA_FDD_BANDIV,           1805,     2112.4   ,               2152.6   },
    {           RRM_UE_UTRA_FDD_BANDV,           0              ,871.4    ,               891.6     },
    {           RRM_UE_UTRA_FDD_BANDVI,           0              ,877.4    ,               882.6     },
    {           RRM_UE_UTRA_FDD_BANDVII,           2175,     2622.4   ,               2687.6   },
    {           RRM_UE_UTRA_FDD_BANDVIII,           340         ,927.4    ,               957.6    },
    {           RRM_UE_UTRA_FDD_BANDIX,           0              ,1847.4  ,        1877.4   },
    {           RRM_UE_UTRA_FDD_BANDX,          1490,     2112.4   ,               2167.6    },
    {           RRM_UE_UTRA_FDD_BANDXI,          736         ,1478.4  ,        1493.4   },
    {           RRM_UE_UTRA_FDD_BANDXII,          -37          ,730.4    ,        743.6     },
    {           RRM_UE_UTRA_FDD_BANDXIII,          -55          ,748.4    ,        753.6     },
    {           RRM_UE_UTRA_FDD_BANDXIV,          -63          ,760.4    ,        765.6     },
    {           RRM_UE_UTRA_FDD_BANDXIX_8A0,          735         ,877.4    ,        887.6     },
    {           RRM_UE_UTRA_FDD_BANDXX_8A0,            -109,      793.4,        818.6 },  
    {           RRM_UE_UTRA_FDD_BANDXXI_8A0,          1326,     1498.4        ,               1508.4   }
    /* CSR 56128: code changes start */
};



typedef struct geran_dl_freq_band_mapping
{
    U8                band_integer_map;
    U8                band_name[10];
    U32               dl_low_freq;
    U32               dl_high_freq;
    U32               arfcn_dl_low1;
    U32               arfcn_dl_high1;
    U32        arfcn_dl_low2;
    U32        arfcn_dl_high2;
}geran_dl_freq_band_mapping_t;

static geran_dl_freq_band_mapping_t gera_freq_band_info[] =
{
    {0           ,"GSM_450",   460.4,            467.6  ,       259,      293, -1, -1},
    {1           ,"GSM_480",   488.0,            496.0  ,       306,      340, -1, -1},
    {2           ,"GSM_710",   728.0,            746.0  ,       0, 0, 0, 0},
    {3           ,"GSM_750",   777.0,            792.0  ,       438,      511, -1, -1},
    {4           ,"GSM_810",   851.0,            866.0  ,       350,      425, -1, -1},
    {5           ,"GSM_850",   869.0,            894.0  ,       128,      251, -1, -1},
    {6           ,"GSM_900_P", 935.0,            960.0  ,       1,        124, -1, -1},
    {7           ,"GSM_900_E", 925.0,            960.0  ,       0,        124, 975, 1023},
    {8           ,"GSM_900_R", 921.0,            960.0  ,       0,        124, 955, 1023},
    {9           ,"GSM_1800",  1805.0,           1880.0 ,       512,      885, -1, -1},
    {10          ,"GSM_1900",  1930.0,           1990.0 ,       512,      810, -1, -1}
};
/*
http://www.rfcafe.com/references/electrical/gsm-specs.htm
http://niviuk.free.fr/gsm_band.php
http://wireless.agilent.com/rfcomms/refdocs/gsm/gen_band_selection.html
GSM-710 Band Channel Number is Dynamic, that's why not listed here.
*/
/* Start fix for bug 715 */
typedef struct _utran_dl_freq_band_tdd128_mapping
{
    S8                name;
    /* CID 14194*/
    U32                band_integer_map;
    U32               freq_low;
    U32               freq_high;
    U32               uarfcn_ul_dl_low;
    U32               uarfcn_ul_dl_high;
}utran_dl_freq_band_tdd128_mapping_t;

static utran_dl_freq_band_tdd128_mapping_t utra_freq_band_tdd128_info[] =
{
    {'a',           0,           1900,          1920,        9504,         9596},
    {'a',           0,           2010,          2025,        10054,        10121},
    {'b',           1,           1850,          1910,        9254,         9546},
    {'b',           1,           1930,          1990,        9654,         9946},
    {'c',           2,           1910,          1930,        9554,         9646},
    {'d',           3,           2570,          2620,        12854,        13096},
    {'e',           4,           2300,          2400,        11504,        11996},
    {'f',           5,           1880,          1920,        9404,         9596}
};

typedef struct _utran_dl_freq_band_tdd384_mapping
{
    S8                name;
    /* CID 14194*/
    U32                band_integer_map;
    U32               freq_low;
    U32               freq_high;
    U32               uarfcn_ul_dl_low;
    U32               uarfcn_ul_dl_high;
}utran_dl_freq_band_tdd384_mapping_t;

static utran_dl_freq_band_tdd384_mapping_t utra_freq_band_tdd384_info[] =
{
    {'a',           0,           1900,          1920,        9512,         9588},
    {'a',           0,           2010,          2025,        10062,        10113},
    {'b',           1,           1850,          1910,        9262,         9538},
    {'b',           1,           1930,          1990,        9662,         9938},
    {'c',           2,           1910,          1930,        9562,         9638},
    {'d',           3,           2570,          2620,        12862,        13088}
};

typedef struct _utran_dl_freq_band_tdd768_mapping
{
    S8                name;
    /* CID 14194*/
    U32                band_integer_map;
    U32               freq_low;
    U32               freq_high;
    U32               uarfcn_ul_dl_low;
    U32               uarfcn_ul_dl_high;
}utran_dl_freq_band_tdd768_mapping_t;

static utran_dl_freq_band_tdd768_mapping_t utra_freq_band_tdd768_info[] =
{
    {'a',           0,           1900,          1920,        9512,         9588},
    {'a',           0,           2010,          2025,        10062,        10113},
    {'b',           1,           1850,          1910,        9262,         9538},
    {'b',           1,           1930,          1990,        9662,         9938},
    {'c',           2,           1910,          1930,        9562,         9638},
    {'d',           3,           2570,          2620,        12874,        13076}
};




/******************************************************************************
 * *  FUNCTION NAME  : rrm_ue_get_utra_tdd128_freq_band
 * *  INPUT          : utran_freq
 * *  OUTPUT         :
 * *  DESCRIPTION    : Verifies if the UE supports UTRAN frequencies
 * *  RETURNS        : Returns frequency band corresponding to the frequency if 
 * *                   supported otherwise returns 0
 * ******************************************************************************/
/* CID 14194*/
U32 rrm_ue_get_utra_tdd128_freq_band (U32 utran_freq)
{
    /* CID 14194*/
    U32             freq_band     = RRM_INVALID_RETURN;
    U8             i             = RRM_ZERO;
    U32            lower_bound   = RRM_ZERO;
    U32            upper_bound   = RRM_ZERO;
    U8             num_elems     = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(utra_freq_band_tdd128_info)/sizeof(utran_dl_freq_band_tdd128_mapping_t);

    for (i = RRM_ZERO; i < num_elems; i++) 
    {
        lower_bound = utra_freq_band_tdd128_info[i].uarfcn_ul_dl_low;
        upper_bound = utra_freq_band_tdd128_info[i].uarfcn_ul_dl_high;
        if ((utran_freq>=lower_bound) &&
                (utran_freq<=upper_bound)) 
        {
            freq_band = utra_freq_band_tdd128_info[i].band_integer_map;
            break;
        } 
    }

    RRM_UT_TRACE_EXIT();

    return freq_band;
}

/******************************************************************************
 * *  FUNCTION NAME  : rrm_ue_get_utra_tdd384_freq_band
 * *  INPUT          : utran_freq
 * *  OUTPUT         : none
 * *  DESCRIPTION    : Verifies if the UE supports UTRAN frequencies
 * *  RETURNS        : Returns frequency band corresponding to the frequency if 
 * *                   supported otherwise returns 0
 * ******************************************************************************/
/* CID 14194*/
U32 rrm_ue_get_utra_tdd384_freq_band (U32 utran_freq)
{
    /* CID 14194*/
    U32             freq_band     = RRM_INVALID_RETURN;
    U8             i             = RRM_ZERO;
    U32            lower_bound   = RRM_ZERO;
    U32            upper_bound   = RRM_ZERO;
    U8             num_elems     = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(utra_freq_band_tdd384_info)/sizeof(utran_dl_freq_band_tdd384_mapping_t);

    for (i = RRM_ZERO; i < num_elems; i++) 
    {
        lower_bound = utra_freq_band_tdd384_info[i].uarfcn_ul_dl_low;
        upper_bound = utra_freq_band_tdd384_info[i].uarfcn_ul_dl_high;
        if ((utran_freq>=lower_bound) &&
                (utran_freq<=upper_bound)) 
        {
            freq_band = utra_freq_band_tdd384_info[i].band_integer_map;
            break;
        } 
    }

    RRM_UT_TRACE_EXIT();

    return freq_band;
}
/******************************************************************************
 * *  FUNCTION NAME  : rrm_ue_get_utra_tdd768_freq_band
 * *  INPUT          : utran_freq
 * *  OUTPUT         : none
 * *  DESCRIPTION    : Verifies if the UE supports UTRAN frequencies
 * *  RETURNS        : Returns frequency band corresponding to the frequency if 
 * *                   supported otherwise returns 0
 * ******************************************************************************/
/* CID 14194*/
U32 rrm_ue_get_utra_tdd768_freq_band (U32 utran_freq)
{
    /* CID 14194*/
    U32             freq_band     = RRM_INVALID_RETURN;
    U8             i             = RRM_ZERO;
    U32            lower_bound   = RRM_ZERO;
    U32            upper_bound   = RRM_ZERO;
    U8             num_elems     = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(utra_freq_band_tdd768_info)/sizeof(utran_dl_freq_band_tdd768_mapping_t);

    for (i = RRM_ZERO; i < num_elems; i++) 
    {
        lower_bound = utra_freq_band_tdd768_info[i].uarfcn_ul_dl_low;
        upper_bound = utra_freq_band_tdd768_info[i].uarfcn_ul_dl_high;
        if ((utran_freq>=lower_bound) &&
                (utran_freq<=upper_bound)) 
        {
            freq_band = utra_freq_band_tdd768_info[i].band_integer_map;
            break;
        } 
    }

    RRM_UT_TRACE_EXIT();

    return freq_band;
}
/* End fix for bug 715 */
/******************************************************************************
 * *  FUNCTION NAME  : rrm_ue_get_utra_freq_band
 * *  INPUT          : utran_freq 
 * *  OUTPUT         : none
 * *  DESCRIPTION    : Verifies if the UE supports UTRAN frequencies
 * *  RETURNS        : Returns frequency band corresponding to the frequency if 
 * *                   supported otherwise returns 0
 * ******************************************************************************/
/* CID 14194*/
U32 rrm_ue_get_utra_freq_band (U32 utran_freq)
{
    /* CID 14194*/
    U32             freq_band     = RRM_INVALID_RETURN;
    U32            i             = RRM_ZERO;
    U32            lower_bound   = RRM_ZERO;
    U32            upper_bound   = RRM_ZERO;
    U32            num_elems     = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(utra_dl_freq_band_info)/sizeof(utran_dl_freq_band_mapping_t);

    for (i = RRM_ZERO; i < num_elems; i++) 
    {
        lower_bound = RRM_FIVE * (utra_dl_freq_band_info[i].dl_low - utra_dl_freq_band_info[i].dl_offset);
        upper_bound = RRM_FIVE * (utra_dl_freq_band_info[i].dl_high - utra_dl_freq_band_info[i].dl_offset);
        if ((utran_freq>=lower_bound) &&
                (utran_freq<=upper_bound)) 
        {
            freq_band = utra_dl_freq_band_info[i].band;
            break;
        } 
    }

    RRM_UT_TRACE_EXIT();

    return freq_band;
}
/* SPR 22731 Fix Start */
/******************************************************************************
 * *  FUNCTION NAME: rrm_ue_is_ue_supported_utra_fdd_frequency
 * *  INPUT        : frequency, p_ue_context
 * *  OUTPUT       :
 * *  DESCRIPTION  : Verifies if the UE supports the defined UTRAN frequency band 
 * *  RETURNS      : Returns TRUE if UE supports otherwise FALSE
 * ******************************************************************************/
    rrm_bool_et
rrm_ue_is_ue_supported_utra_fdd_frequency (
        U16 frequency,
        rrm_ue_context_t* p_ue_context
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    /* CID 14194*/
    U32               freq_band = RRM_ZERO;
    rrm_ue_inter_rat_Parameters_t *p_ue_inter_rat_param = RRM_PNULL;
    U8                              i = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    /*SPR_17893_START*/
    if (RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT & 
            p_ue_context->ue_capability_params.bitmask) 
    /*SPR_17893_END*/
    {
        p_ue_inter_rat_param =(rrm_ue_inter_rat_Parameters_t*) &(p_ue_context->\
                ue_capability_params.rrm_eutra_radio_capability_info.\
                eutra_radio_capability_info_def.inter_rat_parameters);
        /* Start fix for bug 715 */
        if ((RRM_INTER_RAT_UTRA_FDD_PRESENT & p_ue_inter_rat_param->bitmask) 
                && (p_ue_inter_rat_param->utra_fdd.count)) 
        {
            freq_band = rrm_ue_get_utra_freq_band (frequency);
            if(freq_band != RRM_INVALID_RETURN)
            {

                for(i=RRM_ZERO;i<p_ue_inter_rat_param->utra_fdd.count;i++) 
                {
                    if ((freq_band == 
                                /* CID 14194*/
                                (U32)p_ue_inter_rat_param->utra_fdd.supported_band_utra_fdd[i])) /* Coverity: CID 14194 */

                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            } 
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "UTRAN supported FDD band list not present in UE capability");
        }
        /* End fix for bug 715 */
    }
    RRM_TRACE(g_uem_log_on_off, 
            p_rrm_uem_facility_name, 
            RRM_INFO,
            "UTRAN FDD Frequency(%u) band(%d) is %s",
            frequency,
            /* CSR 56128: code changes start */
            freq_band+RRM_ONE,
            /* CSR 56128: code changes end */
            ret_val?"SUPPORTED":"NOT SUPPORTED");
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/******************************************************************************
 * *  FUNCTION NAME: rrm_ue_is_ue_supported_utra_tdd_frequency
 * *  INPUT        : frequency, p_ue_context
 * *  OUTPUT       :
 * *  DESCRIPTION  : Verifies if the UE supports the defined UTRAN frequency band 
 * *  RETURNS      : Returns TRUE if UE supports otherwise FALSE
 * ******************************************************************************/
    rrm_bool_et
rrm_ue_is_ue_supported_utra_tdd_frequency (
        U16 frequency,
        rrm_ue_context_t* p_ue_context
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    /* CID 14194*/
    U32               freq_band = RRM_ZERO;
    rrm_ue_inter_rat_Parameters_t *p_ue_inter_rat_param = RRM_PNULL;
    U8                              i = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    /*SPR_17893_START*/
    if (RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT & 
            p_ue_context->ue_capability_params.bitmask) 
    /*SPR_17893_END*/
    {
        p_ue_inter_rat_param =(rrm_ue_inter_rat_Parameters_t*) &(p_ue_context->\
                ue_capability_params.rrm_eutra_radio_capability_info.\
                eutra_radio_capability_info_def.inter_rat_parameters);
        /* Start fix for bug 715 */
        if ((RRM_INTER_RAT_UTRA_TDD_128_PRESENT & p_ue_inter_rat_param->bitmask) 
                && (p_ue_inter_rat_param->utra_tdd128.count)) 
        {
            freq_band = rrm_ue_get_utra_tdd128_freq_band(frequency);
            if(freq_band != RRM_INVALID_RETURN)
            {
                for(i = RRM_ZERO; i < p_ue_inter_rat_param->utra_tdd128.count; i++) 
                {
                    /* CID 14194*/
                    if ((freq_band == 
                                (U32)p_ue_inter_rat_param->utra_tdd128.supported_band_utra_tdd128[i]))  /* Coverity: CID 14194 */
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            } 
        }
        else if ((RRM_INTER_RAT_UTRA_TDD_384_PRESENT & p_ue_inter_rat_param->bitmask) 
                && (p_ue_inter_rat_param->utra_tdd384.count)) 
        {
            freq_band = rrm_ue_get_utra_tdd384_freq_band(frequency);
            if(freq_band != RRM_INVALID_RETURN)
            {
                for(i = RRM_ZERO; i < p_ue_inter_rat_param->utra_tdd384.count; i++) 
                {
                    /* CID 14194*/
                    if ((freq_band == 
                                (U32)p_ue_inter_rat_param->utra_tdd384.supported_band_utra_tdd384[i])) /* Coverity: CID 14194 */
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            } 
        }
        else if ((RRM_INTER_RAT_UTRA_TDD_768_PRESENT & p_ue_inter_rat_param->bitmask) 
                && (p_ue_inter_rat_param->utra_tdd768.count)) 
        {
            /* CID 14194*/
            freq_band = rrm_ue_get_utra_tdd768_freq_band(frequency);
            if(freq_band != RRM_INVALID_RETURN)
            {
                for(i = RRM_ZERO; i < p_ue_inter_rat_param->utra_tdd768.count; i++) 
                {
                    /* Coverity: CID 14194 */
                    if (freq_band == 
                            (U32)p_ue_inter_rat_param->utra_tdd768.supported_band_utra_tdd768[i]) 
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            } 
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "UTRAN supported TDD band list not present in UE capability");
        }
        /* End fix for bug 715 */
    }
    RRM_TRACE(g_uem_log_on_off, 
            p_rrm_uem_facility_name, 
            RRM_INFO,
            "UTRAN TDD Frequency(%u) band(%d) is %s",
            frequency,
            /* CSR 56128: code changes start */
            freq_band+RRM_ONE,
            /* CSR 56128: code changes end */
            ret_val?"SUPPORTED":"NOT SUPPORTED");
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 22731 Fix End */
#endif /* __UTRA__FREQUENCY_MAPPING__ */
/******************************************************************************
 * *  FUNCTION NAME  : rrm_ue_get_geran_freq_band
 * *  INPUT          : geran_freq
 * *  OUTPUT         : none
 * *  DESCRIPTION    : Verifies if the UE supports GERAN frequencies
 * *  RETURNS        : Returns frequency band corresponding to the frequency if
 * *                   supported otherwise returns 0
 * ******************************************************************************/
U8 rrm_ue_get_geran_freq_band (U32 geran_freq)
{
    U8             freq_band     = RRM_INVALID_RETURN;
    U8             i             = RRM_ZERO;
    U32            lower_bound   = RRM_ZERO;
    U32            upper_bound   = RRM_ZERO;
    U8             num_elems     = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    num_elems = sizeof(gera_freq_band_info)/sizeof(geran_dl_freq_band_mapping_t);

    for (i = RRM_ZERO; i < num_elems; i++)
    {
        lower_bound = gera_freq_band_info[i].arfcn_dl_low1;
        upper_bound = gera_freq_band_info[i].arfcn_dl_high1;
        if ((geran_freq>=lower_bound) &&
                (geran_freq<=upper_bound))
        {
            freq_band = gera_freq_band_info[i].band_integer_map;
            break;
        }

        if(((S32)gera_freq_band_info[i].arfcn_dl_low2 != RRM_MINUS_ONE) && ((S32)gera_freq_band_info[i].arfcn_dl_high2 != RRM_MINUS_ONE))
        {
            lower_bound = gera_freq_band_info[i].arfcn_dl_low2;
            upper_bound = gera_freq_band_info[i].arfcn_dl_high2;
            if ((geran_freq>=lower_bound) &&
                    (geran_freq<=upper_bound))
            {
                freq_band = gera_freq_band_info[i].band_integer_map;
                break;
            }
        }


    }

    RRM_UT_TRACE_EXIT();

    return freq_band;
}

/******************************************************************************
 * *  FUNCTION NAME: rrm_ue_is_ue_supported_geran_frequency
 * *  INPUT        : p_ue_context,frequency
 * *  OUTPUT       : none
 * *  DESCRIPTION  : Verifies if the UE supports the defined GERAN frequency band 
 * *  RETURNS      : Returns TRUE if UE supports otherwise FALSE
 * ******************************************************************************/
    rrm_bool_et
rrm_ue_is_ue_supported_geran_frequency (
        U16 frequency,
        rrm_ue_context_t* p_ue_context
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    U8               freq_band = RRM_ZERO;
    rrm_ue_inter_rat_Parameters_t *p_ue_inter_rat_param = RRM_PNULL;
    U8                              i = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    /*SPR_17893_START*/
    if (RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT & 
            p_ue_context->ue_capability_params.bitmask) 
    /*SPR_17893_END*/
    {
        p_ue_inter_rat_param =(rrm_ue_inter_rat_Parameters_t*) &(p_ue_context->\
                ue_capability_params.rrm_eutra_radio_capability_info.\
                eutra_radio_capability_info_def.inter_rat_parameters);
        if ((RRM_INTER_RAT_GERAN_PRESENT & p_ue_inter_rat_param->bitmask) 
                && (p_ue_inter_rat_param->geran.count)) 
        {
            freq_band = rrm_ue_get_geran_freq_band(frequency);
            if(freq_band != RRM_INVALID_RETURN)
            {
                for(i = RRM_ZERO; i < p_ue_inter_rat_param->geran.count; i++) 
                {
                    if ((freq_band == 
                                p_ue_inter_rat_param->geran.supported_band_geran[i])) 
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            } 
        }    
    }
    return ret_val;
}
/******************************************************************************
 * *  FUNCTION NAME: rrm_ue_is_ue_supported_cdma2000_frequency
 * *  INPUT        : typetype,band_class,p_ue_context  
 * *  OUTPUT       : none
 * *  DESCRIPTION  : Verifies if the UE supports the defined CDMA frequency band 
 * *  RETURNS      : Returns TRUE if UE supports otherwise FALSE
 * ******************************************************************************/
    rrm_bool_et
rrm_ue_is_ue_supported_cdma2000_frequency (
        U8 type,
        U8 band_class,
        rrm_ue_context_t* p_ue_context
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    U32 band_count = RRM_ZERO;
    rrm_ue_inter_rat_Parameters_t *p_ue_inter_rat_param = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    /*SPR_17893_START*/
    if (RRM_UE_EUTRA_RADIO_CAPABILITY_PRESENT & 
            p_ue_context->ue_capability_params.bitmask)
    /*SPR_17893_END*/
    {
        p_ue_inter_rat_param = (rrm_ue_inter_rat_Parameters_t*) &(p_ue_context->\
                ue_capability_params.rrm_eutra_radio_capability_info.\
                eutra_radio_capability_info_def.inter_rat_parameters);


        if (RRM_OAM_NCL_TYPE_HRPD == type)
        {
            if ((RRM_INTER_RAT_CDMA2000_HRPD_PRESENT & p_ue_inter_rat_param->bitmask) && \
                    (p_ue_inter_rat_param->cdma2000_hrpd.supported_band_list_hrpd.count))
            {
                for(band_count = RRM_ZERO; band_count < p_ue_inter_rat_param-> \
                        cdma2000_hrpd.supported_band_list_hrpd.count; \
                        band_count++)
                {
                    if ((band_class ==
                                p_ue_inter_rat_param->cdma2000_hrpd. \
                                supported_band_list_hrpd.band_class_cdma2000[band_count]))
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            }
        }
        else if(RRM_OAM_NCL_TYPE_1XRTT  == type)
        {
            if((RRM_INTER_RAT_CDMA2000_1XRTT_PRESENT & p_ue_inter_rat_param->bitmask) &&
                    (p_ue_inter_rat_param->cdma2000_1x_rtt.supported_band_list_1xrtt.count))
            {
                for(band_count = RRM_ZERO; \
                        band_count < p_ue_inter_rat_param->cdma2000_1x_rtt. \
                        supported_band_list_1xrtt.count; \
                        band_count++)
                {
                    if ((band_class ==
                                p_ue_inter_rat_param->cdma2000_1x_rtt. \
                                supported_band_list_1xrtt.band_class_cdma2000[band_count]))
                    {
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
            }
        }
    }
    /* coverity 25202, Added the trace for failure case, aditya, rel1.3.1 */
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED, 
                "ue_capability_params not present");
        ret_val = RRM_FALSE;
    }
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING, 
            "CDMA2000 %s Band Class [%d] not supported by UE", (RRM_OAM_NCL_TYPE_1XRTT  == type) ? "1XRTT" : "HRPD", band_class);
    return ret_val;
}

/*SPR 21169 Fix Start*/
/******************************************************************************
 *  FUNCTION NAME: fill_inter_freq_meas_object_data
 *  INPUT        : p_rrm_ue_context,p_idle_mode_mobility_params,freq_count
 *  OUTPUT       : p_meas_object_to_add_mod
 *   DESCRIPTION : Fills the eutran configuration in meas config list with values
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_inter_freq_meas_object_data(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                           freq_count
        )
{
    RRM_UT_TRACE_ENTER();
    meas_object_eutra_t        *p_meas_object_eutra = RRM_PNULL;


    p_meas_object_eutra = &(p_meas_object_to_add_mod->meas_object.meas_object_eutra);

    /*SPR_15852_fix: code removed*/
    /*SPR 21464 Changes Start*/
    if((RRM_ZERO != p_meas_object_to_add_mod->meas_object_id) &&
            (p_meas_object_to_add_mod->meas_object_id <= MAX_MEAS_OBJECT_ID))
    /*SPR 21464 Changes End*/
    {
        /*SPR_15852_fix: start*/
        p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;
        /*SPR_15852_fix: end*/

        p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq = 
            p_idle_mode_mobility_params->\
            idle_mode_mobility_inter_freq_params[freq_count].
            eutra_carrier_arfcn;

        p_meas_object_eutra->allowed_meas_bandwidth = 
            p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
            inter_freq_params_op_specific.\
            allowed_meas_bandwidth;

        p_meas_object_eutra->presence_antenna_port1 = 
            p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count]. 
            inter_freq_params_op_specific.presence_antenna_port1;

        p_meas_object_eutra->neigh_cell_config = 
            p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
            neigh_cell_config;

        /* OFFSET FREQ CHANGES START */
        p_meas_object_eutra->offset_freq = 
            p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
            cm_offset_freq;
        /* OFFSET FREQ CHANGES END */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Maximum Measurement object Id reached for UE = %d", p_rrm_ue_context->ue_index);

    }
    RRM_UT_TRACE_EXIT();
}


/******************************************************************************
 *  FUNCTION NAME: fill_inter_freq_meas_object_info
 *  INPUT        : p_rrm_ue_context,p_idle_mode_mobility_params,freq_count
 *  OUTPUT       : p_meas_object_to_add_mod
 *   DESCRIPTION : Fills the eutran configuration in meas config list with values
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_inter_freq_meas_object_info(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                           freq_count
        )
{
    RRM_UT_TRACE_ENTER();
    U8                          meas_object_id     = RRM_ZERO;
    meas_object_eutra_t        *p_meas_object_eutra = RRM_PNULL;

    p_meas_object_eutra = &(p_meas_object_to_add_mod->meas_object.meas_object_eutra);

    /* SPR 21612 start */
    /*SPR_15852_fix: code removed*/
    if (MEAS_OBJECT_TO_ADD_EUTRA_PRESENT & p_meas_object_to_add_mod->meas_object.bitmask)
    {
        meas_object_id = p_meas_object_to_add_mod->meas_object_id;
    }
    else
    {
        meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);
    }
    /* SPR 21612 end */
/*SPR 21464 Changes Start*/
    if((RRM_ZERO != meas_object_id) &&
            (meas_object_id <= MAX_MEAS_OBJECT_ID))
/*SPR 21464 Changes End*/
    {
        /*SPR_15852_fix: start*/
        p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;
        /*SPR_15852_fix: end*/

        p_meas_object_to_add_mod->meas_object_id = meas_object_id;

        /*SPR 21464 Changes Start*/
        /*Trace moved*/
        /*SPR 21464 Changes End*/
        fill_inter_freq_meas_object_data(p_meas_object_to_add_mod,p_rrm_ue_context,p_idle_mode_mobility_params,freq_count);
        /*SPR 21464 Changes Start*/
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Measurement object is created for Inter freq = %d , meas_object_id %d",
                p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq,meas_object_id);
        /*SPR 21464 Changes End*/

        fill_meas_inter_freq_neigh_black_cell_list(p_meas_object_eutra,p_rrm_ue_context, 
                p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
                eutra_carrier_arfcn); 
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Maximum Measurement object Id reached for UE = %d", p_rrm_ue_context->ue_index);

    }
    RRM_UT_TRACE_EXIT();
}
/*SPR 21169 Fix Stop*/

/******************************************************************************
 *  FUNCTION NAME: fill_inter_freq_meas_object_info_ncl 
 *  INPUT        : p_rrm_ue_context,p_idle_mode_inter_freq
 *  OUTPUT       : p_meas_object_to_add_mod
 *   DESCRIPTION : Fills the eutran configuration in meas config list with values 
 *                 received from NCL to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_inter_freq_meas_object_info_ncl(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        inter_freq_params_t         *p_idle_mode_inter_freq
        )
{
    RRM_UT_TRACE_ENTER();
    U8                          meas_object_id     = RRM_ZERO;
    meas_object_eutra_t        *p_meas_object_eutra = RRM_PNULL;
    //RRM_MEMSET (p_meas_object_to_add_mod,0, sizeof(meas_object_to_add_mod_t)); JVR COMMENT

    p_meas_object_eutra = &(p_meas_object_to_add_mod->meas_object.meas_object_eutra);

    meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);
    if(meas_object_id <= MAX_MEAS_OBJECT_ID)
    {
        p_meas_object_to_add_mod->meas_object_id = meas_object_id;

        p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq = 
            p_idle_mode_inter_freq->eutra_carrier_arfcn;

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Measurement object is created for Inter freq = %d",
                p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq);
        p_meas_object_eutra->allowed_meas_bandwidth = p_idle_mode_inter_freq->inter_freq_params_op_specific.allowed_meas_bandwidth; 
        p_meas_object_eutra->presence_antenna_port1 = p_idle_mode_inter_freq->inter_freq_params_op_specific.presence_antenna_port1; 
        p_meas_object_eutra->neigh_cell_config = p_idle_mode_inter_freq->neigh_cell_config; 
        p_meas_object_eutra->offset_freq = p_idle_mode_inter_freq->cm_offset_freq; 

        //fill_meas_inter_freq_neigh_black_cell_list(p_meas_object_eutra,p_rrm_ue_context,eutra_carrier_arfcn); 
        p_meas_object_to_add_mod->meas_object.bitmask |= MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Measurement object Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: rrm_inter_cell_valid_for_selection 
 *  INPUT        : ue_csg_status, p_int_freq_cell
 *  OUTPUT       : none
 *  DESCRIPTION  : Verify that the target cell is suitable for UE or not
 *  RETURNS      : RRM_TRUE
 RRRM_FALSEE
 ******************************************************************************/

rrm_bool_et rrm_inter_cell_valid_for_selection(
        rrm_csg_membership_status_et ue_csg_status,
        inter_freq_cells_t *p_int_freq_cell,
        rrm_bool_et        is_meas_for_masa
        )
{
    rrm_bool_et ret_val = RRM_FALSE;
    RRM_UT_TRACE_ENTER();
    /* Fix for CSR 58972 start*/
    /*Broadcast status is removed*/
    if (RRM_NOT_MEMBER == ue_csg_status && 
            ((RRM_OAM_ACCESS_MODE_OPEN == p_int_freq_cell->cell_access_mode) ||
             (RRM_OAM_ACCESS_MODE_HYBRID == p_int_freq_cell->cell_access_mode) || 
             (RRM_OAM_ACCESS_MODE_OPEN_FEMTO == p_int_freq_cell->cell_access_mode)) &&
            (RRM_FALSE == p_int_freq_cell->blacklisted))
        /* Fix for CSR 58972 end*/
    {
        ret_val = RRM_TRUE;
    }
    else if (RRM_MEMBER == ue_csg_status && 
            (RRM_OAM_ACCESS_MODE_CLOSE == p_int_freq_cell->cell_access_mode ||
             RRM_OAM_ACCESS_MODE_HYBRID == p_int_freq_cell->cell_access_mode) && 
            (RRM_FALSE == p_int_freq_cell->blacklisted))
    {
        ret_val = RRM_TRUE;
    }
    /*CA stage3: start*/
    if (is_meas_for_masa)
    {
        if(X2_LOCALLY_CONNECTED_CELL == p_int_freq_cell->x2_status)
        {
            ret_val = RRM_TRUE;
        }
        else
        {
            ret_val = RRM_FALSE;
        }
    }
    /*CA stage3: end*/
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
            "Ue csg status[%d] cell access mode[%d] broadcast status[%d] blacklist[%d] is_meas_for_masa[%d] x2_status[%d] cell %s",
            ue_csg_status, p_int_freq_cell->cell_access_mode,p_int_freq_cell->broadcast_status,p_int_freq_cell->blacklisted,
            is_meas_for_masa,p_int_freq_cell->x2_status,
            (RRM_TRUE== ret_val ? "valid":"not valid"));

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/******************************************************************************
 *  FUNCTION NAME: fill_inter_freq_meas_config_object_frm_ncl 
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with values 
 *                 received from NCL to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
/* START: CSR 00057390*/
static rrm_return_et
fill_inter_freq_meas_config_object_frm_ncl(
        rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        meas_object_to_add_mod_list_t  *p_new_added_meas_object,
        /* END: CSR 00057390*/
        rrm_bool_et is_meas_for_masa
        )
{
    rrm_return_et          ret_val                = RRM_FAILURE;
    U8                     valid_inter_freqs_cell = RRM_ZERO;
    U8                     inter_freq_cell_count  = RRM_ZERO;
    U32                    carrier_freq           = RRM_ZERO ;
    lte_ncl_t              *p_ncl_list            = RRM_PNULL;
    rrm_bool_et            ret_car_freq_supported = RRM_FALSE;
    U8                     object_count           = p_meas_config->meas_object_to_add_mod_list.count;
    meas_object_to_add_mod_t *p_meas_object_to_add_mod = RRM_PNULL;
    idle_mode_mobility_params_t   *p_idle_mode_mobility_params = RRM_PNULL;
    U8    valid_inter_freq_index = RRM_OUT_OF_RANGE;
    U8    max_avail_rs = RRM_ZERO; 
    U8    idle_inter_list = RRM_ZERO;
    /* START: CSR 00057390*/
    meas_report_info_node_t  *p_meas_object_config = RRM_PNULL;
    U8    new_meas_obj_index = RRM_ZERO;
    /* END: CSR 00057390*/
    /* Bug_944_start */
    rrm_cell_context_t *p_cell_context                  = RRM_PNULL;
    U8 load_threshold                                   = RRM_ZERO;
    U16 no_of_cell_in_less_load                         = RRM_ZERO;
    U16 rrm_eutra_freq_load_value                       = RRM_ZERO;
    U16 toal_cell_on_particular_freq                    = RRM_ZERO;
    U16 freq_with_less_load                             = RRM_ZERO;
    /* Bug_944_end */
    /*SPR 12080:start*/
    U8   cell_to_add_mod = RRM_ZERO;
    /*SPR 12080:end*/
    rrm_bool_t   cell_index_found = RRM_FALSE;
    /* SPR:12849 Start */
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node = RRM_PNULL;
    U8 cell_index = RRM_ZERO;
    /* SPR:12849 End */
    /* SPR 14326 Fix Start */
    rrm_bool_et    send_measgap = RRM_FALSE;
    /* SPR 14326 Fix End */


    RRM_UT_TRACE_ENTER();
    /* Bug_944_start */
    p_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    if (RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell context is NULL at cell_index [%d]",p_rrm_ue_context->cell_index);
        ret_val = RRM_FAILURE;
        RRM_UT_TRACE_EXIT();
        return ret_val;

    };
    load_threshold = rrm_find_load_threshold (p_cell_context);
    /* Bug_944_end */
    p_idle_mode_mobility_params = rrm_cellm_get_idle_mode_params\
                                  (p_rrm_ue_context->cell_index);
    /*Fill from NCL list*/
    p_ncl_list = rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);

    if(RRM_PNULL != p_ncl_list)  /* Coverity: CID 29577 */
    {
        valid_inter_freqs_cell            =  p_ncl_list->num_valid_inter_freq_cell;
        for(idle_inter_list = RRM_ZERO; idle_inter_list < p_idle_mode_mobility_params->num_valid_inter_freq_list; idle_inter_list++)
        {
            /* Bug_944_start */
            /*Initalizing old avail resource for next frequency*/
            cell_to_add_mod = RRM_ZERO;
            max_avail_rs = RRM_ZERO;
            no_of_cell_in_less_load = RRM_ZERO;
            rrm_eutra_freq_load_value    = RRM_ZERO;
            toal_cell_on_particular_freq = RRM_ZERO;
            /* Bug_944_end */
            /* carrier freq supported by UE */
            /*CID 66796:start*/
            ret_car_freq_supported = (rrm_bool_et)rrm_ue_is_ue_supported_eutra_frequency (
                    p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[idle_inter_list].eutra_carrier_arfcn,
                    p_rrm_ue_context);
            /*CID 66796:end*/
            if (!ret_car_freq_supported)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                        "Carrier Freq = %d, is not supported by [UE:%d]",
                        carrier_freq,
                        p_rrm_ue_context->ue_index);
                continue;
            }
            for(inter_freq_cell_count = RRM_ZERO; inter_freq_cell_count < valid_inter_freqs_cell; inter_freq_cell_count++)
            {
                valid_inter_freq_index = RRM_OUT_OF_RANGE;
                cell_index_found = RRM_FALSE;
                cell_index = RRM_ZERO;
                if(p_ncl_list->inter_freq_cells[inter_freq_cell_count].eutra_carrier_arfcn ==
                        p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[idle_inter_list].eutra_carrier_arfcn)
                {
                    toal_cell_on_particular_freq++;
                    if(p_ncl_list->inter_freq_cells[inter_freq_cell_count].resrc_report.bitmask & RRM_COMP_AVL_CAP_PERIODIC_REPORT_PRESENT)	
                    {
                        max_avail_rs = RRM_GET_MAX(p_ncl_list->inter_freq_cells[inter_freq_cell_count].resrc_report.comp_avl_cap_grp.dl.cell_cap_val,
                                p_ncl_list->inter_freq_cells[inter_freq_cell_count].resrc_report.comp_avl_cap_grp.ul.cell_cap_val);
                        /* Bug_944_start */    
                    }
                    else
                    {
                        max_avail_rs = RRM_MAX_AVLB_CAPCTY;
                    }
                    if (max_avail_rs > load_threshold)
                    {
                        valid_inter_freq_index = inter_freq_cell_count;
                        no_of_cell_in_less_load++;
                    }
                }
                /*SPR 12076:start*/
                if (RRM_OUT_OF_RANGE == valid_inter_freq_index)
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                            "Either earfcn[%d] not found or max_avail_rs[%d] is less than load_threshold[%d]",
                            p_ncl_list->inter_freq_cells[inter_freq_cell_count].eutra_carrier_arfcn,
                            max_avail_rs, load_threshold);
                    continue;
                }
                else
                {
                    if (is_meas_for_masa)
                    {
                        /*If frequency satisfy ca criteria then only select it*/
                        if(RRM_TRUE == rrm_check_freq_already_configured_for_ca(
                                    &(p_ncl_list->inter_freq_cells[inter_freq_cell_count]), 
                                    p_rrm_ue_context))
                        {
                            continue;
                        }
                    }
                }
                /*SPR 12076:end*/
                if ((toal_cell_on_particular_freq > RRM_ZERO) && (no_of_cell_in_less_load > RRM_ZERO))
                {
                    /* Multiply to Hundred to solve fractional value as data type is unsigned int */
                    rrm_eutra_freq_load_value = RRM_HUNDRED * (no_of_cell_in_less_load/toal_cell_on_particular_freq);
                    if (rrm_eutra_freq_load_value > freq_with_less_load)
                    {
                        freq_with_less_load = rrm_eutra_freq_load_value;
                    }

                }
                if(RRM_ZERO != rrm_eutra_freq_load_value) /* idle mode freq found in ncl list*/
                { 
                    /* Bug_944_end */
                    /*chk broadcasring is true. cell is not blacklisted. Cell access mode */
                    if(RRM_TRUE == rrm_inter_cell_valid_for_selection(p_rrm_ue_context->rrm_csg_membership.rrm_csg_membership_status,
                                &p_ncl_list->inter_freq_cells[valid_inter_freq_index], is_meas_for_masa))
                    {
                        /*Check the usage of ue can be fulfilled by cell or not. if yes select the cell*/
                        if(RRM_TRUE == rrm_uem_is_cell_capable(p_rrm_ue_context, &p_ncl_list->inter_freq_cells[valid_inter_freq_index]))
                        {
                            carrier_freq = p_ncl_list->inter_freq_cells[valid_inter_freq_index].eutra_carrier_arfcn;
                            p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[object_count]);
                            /* START: CSR 00057390*/
                            if( RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                                        carrier_freq, RRM_ZERO, RRM_IRAT_PRIORITY_EUTRAN,&p_meas_object_config))
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                        " Inter-freq meas_object=%d is already configured for carrier_freq =%d for  [UE:%d]",
                                        p_meas_object_config->meas_object_id,carrier_freq,p_rrm_ue_context->ue_index);
                                if(RRM_TRUE ==  rrm_is_meas_event_configured(p_rrm_ue_context,p_meas_object_config->meas_object_id,MEAS_REP_EVENT_A4))
                                {
                                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                            "No Need to add meas object as A4 event is already configured for carrier freq = %d for [UE:%d]",
                                            carrier_freq,p_rrm_ue_context->ue_index);
                                    ret_val =  RRM_SUCCESS;
                                }
                                else
                                {
                                    if (!(p_meas_object_to_add_mod->meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT))
                                    {
                                        p_meas_object_to_add_mod->meas_object.bitmask |= MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;
                                        p_meas_object_to_add_mod->meas_object_id =  p_meas_object_config->meas_object_id;
                                        p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq = carrier_freq;
                                        ret_val =  RRM_SUCCESS;
                                    }
                                    /*SPR 12080:start*/ 
                                    if (RRM_TRUE == is_meas_for_masa)
                                    {
                                        /*SPR 12849:start*/
                                        p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) 
                                            ylFirst(&(p_meas_object_config->cells_to_add_mod_list));
                                        while (p_cell_to_add_mode_node)
                                        {
                                            if (p_ncl_list->inter_freq_cells[valid_inter_freq_index].phy_cell_id == 
                                                    p_cell_to_add_mode_node->phy_cell_id)
                                            {
                                                cell_index_found = RRM_TRUE;
                                                break; 
                                            }
                                            if (p_cell_to_add_mode_node->cell_index > cell_index)
                                            {
                                                cell_index = p_cell_to_add_mode_node->cell_index;
                                            }
                                            p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
                                        }
                                        if(RRM_TRUE == cell_index_found)
                                        {
                                            p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                                cells_to_add_mod[cell_to_add_mod].cell_index = p_cell_to_add_mode_node->cell_index; 
                                        }
                                        else
                                        {
                                            p_cell_to_add_mode_node = RRM_PNULL;
                                            /*Find cell index and insert the node in list*/
                                            p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                                cells_to_add_mod[cell_to_add_mod].cell_index = cell_index + RRM_ONE; 
                                            p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)
                                                rrm_mem_get(sizeof(rrm_meas_obj_cell_to_add_info_t));
                                            /*Add the cell index in meas_report_mode_t struct*/
                                            if (RRM_PNULL != p_cell_to_add_mode_node)
                                            {
                                                p_cell_to_add_mode_node->cell_index = cell_index + RRM_ONE;
                                                p_cell_to_add_mode_node->phy_cell_id =  
                                                    p_ncl_list->inter_freq_cells[valid_inter_freq_index].phy_cell_id;
                                                ylPushTail(&(p_meas_object_config->cells_to_add_mod_list),
                                                        &(p_cell_to_add_mode_node->node));
                                            }
                                        }
                                        /*SPR 12849:end*/
                                        p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                            cells_to_add_mod[cell_to_add_mod].phys_cell_id = 
                                            p_ncl_list->inter_freq_cells[valid_inter_freq_index].phy_cell_id;
                                        p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                            cells_to_add_mod[cell_to_add_mod].cell_individual_offset = 
                                            p_ncl_list->inter_freq_cells[valid_inter_freq_index].cio;
                                        cell_to_add_mod++;
                                    }
                                    /*SPR 12080:end*/ 
                                }
                            }
                            else
                            {
                                /* END: CSR 00057390*/
                                if (!(p_meas_object_to_add_mod->meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT))
                                {
                                    fill_inter_freq_meas_object_info_ncl(p_meas_object_to_add_mod,
                                            p_rrm_ue_context, &(p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[idle_inter_list]));
                                    /*If load for any of frequency is less. send success*/
                                    /* START: CSR 00057390*/
                                }
                                /*SPR 12080:start*/ 
                                if (RRM_TRUE == is_meas_for_masa)
                                {
                                    /*SPR 12849:Start */
                                    if (RRM_ZERO != cell_to_add_mod)
                                    {
                                        p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                            cells_to_add_mod[cell_to_add_mod].cell_index = 
                                            p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                            cells_to_add_mod[cell_to_add_mod - RRM_ONE].cell_index + RRM_ONE; 
                                    }
                                    else
                                    {
                                        p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                            cells_to_add_mod[cell_to_add_mod].cell_index = RRM_ONE;
                                    }
                                    /*SPR 12849:end */
                                    p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                        cells_to_add_mod[cell_to_add_mod].phys_cell_id = 
                                        p_ncl_list->inter_freq_cells[valid_inter_freq_index].phy_cell_id;
                                    p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.
                                        cells_to_add_mod[cell_to_add_mod].cell_individual_offset = 
                                        p_ncl_list->inter_freq_cells[valid_inter_freq_index].cio;

                                    cell_to_add_mod++;
                                }
                                /*SPR 12080:end*/ 
                                /* END: CSR 00057390*/
                                ret_val = RRM_SUCCESS;
                            }
                            /* START: CSR 00057390*/
                        }
                        /* END: CSR 00057390*/
                    }
                }
            }
            /*SPR 12080:start*/
            if (cell_to_add_mod != RRM_ZERO)
            {
                p_meas_object_to_add_mod->meas_object.meas_object_eutra.bitmask |= MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT;
                p_meas_object_to_add_mod->meas_object.meas_object_eutra.cells_to_add_mod_list.count = cell_to_add_mod;
            }
            if ((p_meas_object_to_add_mod != RRM_PNULL) && (p_meas_object_to_add_mod->meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT) &&
                    /*SPR_15852_fix: start*/
                    (p_meas_object_to_add_mod->meas_object_id <= MAX_MEAS_OBJECT_ID))
                /*SPR_15852_fix: end*/
            {
                object_count++;
                RRM_MEMCPY(&p_new_added_meas_object->meas_object_to_add_mod[new_meas_obj_index],
                        p_meas_object_to_add_mod,sizeof(meas_object_to_add_mod_t));
                p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[object_count]);

                /* SPR 14326 Fix Start */
                if( RRM_FALSE == send_measgap )
                {
                    send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_context->ran_info.rf_params
                            .rf_configurations.dl_earfcn ,
                            p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[idle_inter_list].
                            eutra_carrier_arfcn,
                            p_rrm_ue_context,
                            INTER_FREQ_BAND_LIST,
                            EUTRA_FREQ ,
                            RRM_PNULL
                            );
                }
                /* SPR 14326 Fix End */

                /*SPR 12798:start*/
                if (RRM_PNULL == p_meas_object_config) 
                    /*SPR 12798:end*/
                {
                    p_new_added_meas_object->count++;
                    new_meas_obj_index++;
                }
                if(RRM_TRUE == RRM_UE_MEAS_OBJECT_COUNT(object_count))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                            "eight meas object is configured");
                    break; 
                }

            }
            /*SPR 12080:start*/

        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "NCL list is empty");
    }
    if (RRM_ZERO != object_count)
    {
        /*set the bitmask for UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT*/
        p_meas_config->bitmask  |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
        p_meas_config->meas_object_to_add_mod_list.count = object_count;
        /* SPR 14326 Fix Start */
        if( RRM_TRUE == send_measgap )
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "HO: Send MEAS GAP for UTRA FDD");
            p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "HO: NO need to send MEAS GAP for UTRA FDD");
            p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
        }
        /* SPR 14326 Fix End */

        ret_val = RRM_SUCCESS;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}


/*spr_12273_fix*/
/****************************************************************************
 * Function Name  : rrm_check_if_new_meas_object_is_req_inter_freq 
 * Inputs         : p_ue_context,
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Checks if any cells are deleted from ncl after meas object creation 
 ****************************************************************************/
/*SPR 21169 Fix Start*/
rrm_void_t
/*SPR 21169 Fix Stop*/
    rrm_check_if_new_meas_object_is_req_inter_freq
(
 rrm_ue_context_t       *p_ue_context,
 meas_report_info_node_t   *p_meas_report_node,
 cell_index_list_t         *cells_to_remove_list,
 meas_object_to_add_mod_t   *p_meas_config_object
 )
{
    lte_ncl_t              *p_ncl_list            = RRM_PNULL;
    U8         valid_cells      = RRM_ZERO;
    U8         ncl_cell_count       = RRM_ZERO;
    rrm_bool_et        pci_found= RRM_FALSE;
    U8 count = RRM_ZERO;
    /* SPR:12849 Start */ 
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node = RRM_PNULL;
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mode_node_to_del = RRM_PNULL;
    /* SPR:12849 End */ 
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mod = RRM_PNULL;
    U8 cell_index = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    /*Fill from NCL list*/
    p_ncl_list = rrm_cellm_get_lte_ncl_info(p_ue_context->cell_index);
    /*SPR 21169 Fix Start*/
    if((RRM_PNULL != p_ncl_list) && (RRM_ZERO < p_ncl_list->num_valid_inter_freq_cell))
    {
        valid_cells  =  p_ncl_list->num_valid_inter_freq_cell;
        /*Traverse through the meas object node and compare the pci present in ncl*/
        p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) ylFirst(&(p_meas_report_node->cells_to_add_mod_list));
        while (p_cell_to_add_mode_node)
        {
            pci_found = RRM_FALSE; 
            //outer meas object loop 
          /*SPR 21150 Fix Start*/
            for(ncl_cell_count = RRM_ZERO; (ncl_cell_count < valid_cells) && 
                    (RRM_PNULL != p_cell_to_add_mode_node) && (ncl_cell_count < MAX_INTER_FREQ_CELLS ) ; ncl_cell_count++)
          /*SPR 21150 Fix End*/
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "pci meas mode[%d] nrt pci[%d]",p_cell_to_add_mode_node->phy_cell_id, 
                        p_ncl_list->inter_freq_cells[ncl_cell_count].phy_cell_id );
                if ((p_cell_to_add_mode_node->phy_cell_id == p_ncl_list->inter_freq_cells[ncl_cell_count].phy_cell_id) &&
                        (p_meas_report_node->carrier_earfcn == p_ncl_list->inter_freq_cells[ncl_cell_count].eutra_carrier_arfcn))
                {
#ifdef LTE_EMBMS_SUPPORTED
                    /*COV_START*/
                     /*SPR 18984:Fix Start*/
                   /*if ( RRM_PNULL != p_cell_to_add_mode_node )
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }*/
                    /*SPR 18984:Fix End*/
                    /*COV_STOP*/
#endif
                    pci_found = RRM_TRUE;
                    break;
                }
                }
            if(pci_found == RRM_FALSE)
                {

                    /*Case where pci is deleted from ncl , here the cell index needs to be filled in cell remove list*/

                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                            "For meas id %d with cell index %d , pci %d is deleted from ncl.",
                            p_meas_report_node->meas_id, p_cell_to_add_mode_node->cell_index,
                            /*Coverity:65853 Start*/
                            p_cell_to_add_mode_node->phy_cell_id);
                    /*Coverity:65853 End*/

                    cells_to_remove_list->cell_index[cells_to_remove_list->count] = p_cell_to_add_mode_node->cell_index; 
                    /*Delete the node from the list*/
                    p_cell_to_add_mode_node_to_del = p_cell_to_add_mode_node;
                    /* SPR:12849 End */ 
                    cells_to_remove_list->count++;
                    p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
                    ylDelete(&(p_meas_report_node->cells_to_add_mod_list),(YLNODE *)p_cell_to_add_mode_node_to_del);
                    RRM_MEM_FREE(p_cell_to_add_mode_node_to_del);
                }
                else
                {
                    p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
                }
        }
        /*SPR 21169 Fix Stop*/
        /*SPR:12841 start*/
        for(ncl_cell_count = RRM_ZERO; (ncl_cell_count < valid_cells) && 
                (ncl_cell_count < MAX_INTER_FREQ_CELLS ) ; ncl_cell_count++)
        {
            pci_found = RRM_FALSE;
            p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *) ylFirst(&(p_meas_report_node->cells_to_add_mod_list));
            while (p_cell_to_add_mode_node)
            {
                if ((p_ncl_list->inter_freq_cells[ncl_cell_count].phy_cell_id == p_cell_to_add_mode_node->phy_cell_id) &&
                        (p_ncl_list->inter_freq_cells[ncl_cell_count].eutra_carrier_arfcn == p_meas_report_node->carrier_earfcn))
                {
                    pci_found = RRM_TRUE;
                    break;
                }
                cell_index = p_cell_to_add_mode_node->cell_index;
                p_cell_to_add_mode_node = (rrm_meas_obj_cell_to_add_info_t *)ylNext((YLNODE *)p_cell_to_add_mode_node);
            }
            if ((RRM_FALSE == pci_found) && 
                    (p_meas_report_node->carrier_earfcn == p_ncl_list->inter_freq_cells[ncl_cell_count].eutra_carrier_arfcn))
            {
                p_cell_to_add_mod = (rrm_meas_obj_cell_to_add_info_t *)rrm_mem_get(sizeof(rrm_meas_obj_cell_to_add_info_t));
                if (RRM_PNULL != p_cell_to_add_mod)
                {
                    count = p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.count;
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[count].cell_index = 
                        cell_index + RRM_ONE; 
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[count].phys_cell_id =
                        p_ncl_list->inter_freq_cells[ncl_cell_count].phy_cell_id; 
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[count].cell_individual_offset = 
                        p_ncl_list->inter_freq_cells[ncl_cell_count].cio;
                    p_meas_config_object->meas_object.meas_object_eutra.cells_to_add_mod_list.count++;
                    p_cell_to_add_mod->cell_index = cell_index + RRM_ONE; 
                    p_cell_to_add_mod->phy_cell_id = p_ncl_list->inter_freq_cells[ncl_cell_count].phy_cell_id; 
                    ylPushTail(&(p_meas_report_node->cells_to_add_mod_list),&( p_cell_to_add_mod->node));
                }
            }
        }
        /*SPR:12841 end*/
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Either lte ncl list is null or valid inter freq cells are zero"
                );

    }
    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: fill_inter_freq_meas_config_object
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *   DESCRIPTION : Fills the eutran configuration in meas config list with values 
 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
/* Fix for CSR 58972 start*/
rrm_void_t
/* Fix for CSR 58972 end*/
fill_inter_freq_meas_config_object(
        rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        /* SPR_14564_fix: start */
        rrm_ue_meas_cause_et        rrm_inter_freq_meas
        /* SPR_14564_fix: end */
        )
{
    RRM_UT_TRACE_ENTER();

    U8 valid_freqs         = RRM_ZERO;
    //U8 meas_object_id     = RRM_ZERO;
    U8 initial_index       = RRM_ZERO; 
    U8  freq_count         = RRM_ZERO;
    U8  carrier_freq_configured = RRM_ZERO;
    U32 carrier_freq       = RRM_ZERO;
    U8  counter            = RRM_ZERO;
    idle_mode_mobility_params_t   *p_idle_mode_mobility_params = RRM_PNULL;
    rrm_bool_et                   ret_car_freq_supported       = RRM_FALSE;
    rrm_bool_et  freq_cells_exist = RRM_FALSE;
    lte_ncl_t    *p_lte_ncl       = RRM_PNULL;
    U8 valid_cells         = RRM_ZERO;
    U8   object_count       = p_meas_config->meas_object_to_add_mod_list.count;
    /* START: CSR 00057390*/
    /* Coverity fix 88485 */
    meas_report_info_node_t  *p_meas_object_config = RRM_PNULL;
    /* Coverity fix 88485 */
    /* END: CSR 00057390*/
    /* SPR 14326 Fix Start */
    rrm_bool_et    send_measgap = RRM_FALSE;
    rrm_cell_context_t *p_cell_context                  = RRM_PNULL;
    /* SPR 14326 Fix End */
/*SPR 21365 fix start*/
    U8 meas_config_count  =  rrm_count_measurements_for_ho_configured(p_rrm_ue_context) + object_count;
    /*SPR 21365 fix stop*/

    /*spr_12273_fix_start*/
    YLNODE *p_node = RRM_PNULL;
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;
    cell_index_list_t     cells_to_remove_list;

    RRM_MEMSET(&cells_to_remove_list,RRM_ZERO,sizeof(cell_index_list_t) );

    /*spr_12273_fix_end*/
    p_idle_mode_mobility_params = rrm_cellm_get_idle_mode_params\
                                  (p_rrm_ue_context->cell_index);
    meas_object_to_add_mod_t  *p_meas_object_to_add_mod = RRM_PNULL;
    rf_params_t         *p_rf_params         = RRM_PNULL;
    rrm_proximity_ind_t *p_rrm_proximity_ind = RRM_PNULL;

    p_rf_params = rrm_cellm_get_rf_params(p_rrm_ue_context->cell_index);

    /* SPR_14564_fix: start */
    if(RRM_PNULL != p_idle_mode_mobility_params)
    {
        valid_freqs =  p_idle_mode_mobility_params->num_valid_inter_freq_list;
    }
    /* SPR_14564_fix: end */
    /* SPR 14326 Fix Start */
    p_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    /* SPR 14326 Fix End */

    p_lte_ncl              =  rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
    /* SPR_14564_fix: start */
    rrm_bool_et rrm_is_inter_freq_ncl_empty = RRM_FALSE;
    /*CID:88257 Fix Start*/
    if (RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "p_cell_context is NULL for cell_index[%d]", p_rrm_ue_context->cell_index);
        return;
    }
    /*CID:88257 Fix End*/
    if(RRM_PNULL != p_lte_ncl) /* Coverity: CID 30922 */
    {
        valid_cells            =  p_lte_ncl->num_valid_inter_freq_cell;

        if(RRM_ZERO == valid_cells)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Inter Freq. cell list is empty");
            /* In case if there is no cell present in Inter Freq List of LTE then we will configure all the 
             * Idle mode Inter Frequencies on UE. If any cell present in LTE NCL then we'll create the meas 
             * object for that frequency only.
             */
            rrm_is_inter_freq_ncl_empty = RRM_TRUE;
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "LTE NCL list is empty");
        /* In case if there is no cell present in Inter Freq List of LTE then we will configure all the 
         * Idle mode Inter Frequencies on UE. If any cell present in LTE NCL then we'll create the meas 
         * object for that frequency only.
         */
        rrm_is_inter_freq_ncl_empty = RRM_TRUE;
    }
    /* SPR_14564_fix: end */



    /* Start fix for SPR 8391:csg proximity */
    /* 
     ** This is inter freq case so configure measurements on the carrier freq
     ** received in proximity ind if it is configured in ncl.
     */


    if(RRC_RRM_PROXIMITY_IND_EVENT == p_rrm_ue_context->ue_event)
    {
        p_rrm_proximity_ind = (rrm_proximity_ind_t *)
            p_rrm_ue_context->current_procedure.p_proc_data;
        /* Start fix for SPR 962 */
        if (!(p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Eutra Carrier freq is not received in proximity ind"); 
            return;
        }
        else if ((p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT) && 
                (p_rrm_proximity_ind->carrier_Freq.carrier_freq_eutra == p_rf_params->rf_configurations.dl_earfcn))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Carrier freq received in proximity ind is same as serving cell = %d", 
                    p_rrm_proximity_ind->carrier_Freq.carrier_freq_eutra);
            return;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Eutra inter cell carrier freq is received in proximity ind"); 
        }
        /* End fix for SPR 962 */
    }
    /* End fix for SPR 8391 */
    for(freq_count=RRM_ZERO; freq_count < valid_freqs; freq_count++)
    {
        carrier_freq_configured = RRM_ZERO;
        carrier_freq = p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
            eutra_carrier_arfcn;

        /* Start fix for SPR 8391:csg proximity */
        if ((p_rrm_proximity_ind != RRM_PNULL) &&
                (p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_EUTRA_PRESENT) &&
                (p_rrm_proximity_ind->carrier_Freq.carrier_freq_eutra != carrier_freq))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "Proximity case: carrier freq [%d] does not match , check next", carrier_freq);
            continue; 
        }
        /* End fix for SPR 8391 */

        /* Bug 678 Start*/
        /* SPR_14564_fix: start */
        /* In case Inter Freq. list is NULL or not present then RRM shall configure all the Idle mode Inter Freq.
         * in meas config. Otherwise only the cells configured in NCL will be configured in meas config req.*/
        if((RRM_FALSE == rrm_is_inter_freq_ncl_empty) && (RRM_PNULL != p_lte_ncl))
        {
            freq_cells_exist = RRM_FALSE;
            for( counter = RRM_ZERO; counter < valid_cells; counter++)
            {
                if(carrier_freq == p_lte_ncl->inter_freq_cells[counter].eutra_carrier_arfcn)
                {
                    freq_cells_exist = RRM_TRUE;
                    break;
                }
            }
            if(RRM_FALSE == freq_cells_exist)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "Not creating meas object for EUTRA frequency[%d] since no cells exists in ncl for this frequency",
                        carrier_freq);
                continue;
            }
        }
        if((RRM_INTER_FREQ_FOR_CGI == rrm_inter_freq_meas) &&
                (carrier_freq != p_rrm_ue_context->inter_freq_earfcn_for_cgi))
        {
            continue;
        }
        /* SPR_14564_fix: end */
        /* Bug 678 End*/
        for (counter = initial_index; counter < object_count; counter++)
        {        
            p_meas_object_to_add_mod = &(p_meas_config->\
                    meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);               
            if ( p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq
                    == carrier_freq )
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "Freq %d already configured  on UE", carrier_freq);
                carrier_freq_configured = RRM_TRUE;
                break;
            }
        }


        if (carrier_freq_configured != RRM_TRUE)
        {

            /* carrier freq supported by UE */
            /*CID 66797:start*/
            ret_car_freq_supported = (rrm_bool_et)rrm_ue_is_ue_supported_eutra_frequency (
                    carrier_freq,
                    p_rrm_ue_context);
            /*CID 66797:end*/
            if (!ret_car_freq_supported)
            {
                continue;
            }
            /*SPR 21365 fix start*/
            if(meas_config_count >= (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE))
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "Maximum allowed MeasConfiguration Reached ");
                break; 
            }    
            /*SPR 21365 fix stop*/         
            p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[counter]); 
            /* START: CSR 00057390*/
            if( RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                        carrier_freq,
                        RRM_ZERO,
                        RRM_IRAT_PRIORITY_EUTRAN,&p_meas_object_config))
            {
                /*spr_12273_fix_start */
                /*SPR 13317:start*/
                /*Code deleted*/
                /*SPR 13317:end*/
                p_node = ylFirst(&(p_rrm_ue_context->meas_report_list));
                while (p_node)
                {
                    p_meas_report_node = (meas_report_info_node_t*)p_node;
                    if(carrier_freq == p_meas_report_node->carrier_earfcn)
                    {
                        /*SPR 21169 Fix Start*/
                        rrm_check_if_new_meas_object_is_req_inter_freq(p_rrm_ue_context,p_meas_report_node,&cells_to_remove_list, 
                                &(p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter]));
                            if (RRM_ZERO != cells_to_remove_list.count)
                            {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,"cell to remove list is added for inter freq");
                            fill_eutran_meas_object_for_cells_to_remove_list(
                                    &(p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter]),
                                        &cells_to_remove_list);
                            }
                            p_meas_object_to_add_mod->meas_object_id = p_meas_report_node->meas_object_id; 
                            /*SPR 21464 Changes Start*/
                            /*code removed*/
                            /*SPR 21464 Changes End*/

                        fill_inter_freq_meas_object_data(p_meas_object_to_add_mod,p_rrm_ue_context,
                                p_idle_mode_mobility_params,freq_count);

                        fill_meas_inter_freq_neigh_black_cell_list(&(p_meas_object_to_add_mod->meas_object.meas_object_eutra),
                                p_rrm_ue_context,
                                p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].eutra_carrier_arfcn);

                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    " Inter-freq meas_object is already configured for carrier_freq =%d for  UE = %d",
                                    carrier_freq,p_rrm_ue_context->ue_index);
                            p_meas_object_to_add_mod->meas_object.bitmask
                                = MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;
                        /*SPR 21169 Fix Stop*/
                    }
                    /* SPR 21598 start */
                    /* Code removed */
                    /* SPR 21598 end */
                    p_node = ylNext(p_node);
                }
            }
            /* Coverity Fix 86167 Start */
            else
            {
                /*SPR 17259 +-*/

                fill_inter_freq_meas_object_info(p_meas_object_to_add_mod,
                        p_rrm_ue_context, p_idle_mode_mobility_params, freq_count);
                /*SPR_15852_fix: start*/
                /*SPR 21365 fix start*/
                /* Lines Deleted */
                /*SPR 21365 fix stop*/
                /*SPR_15852_fix: end*/
                /*SPR 17259 +-*/
            }
            /* END: CSR 00057390*/
            /*spr_12273_fix_end*/
            /*SPR 21365 fix start*/
            if (MEAS_OBJECT_TO_ADD_EUTRA_PRESENT & p_meas_object_to_add_mod->meas_object.bitmask)
            {
                object_count++;
                meas_config_count++;
            }
            /*SPR 21365 fix stop*/

            /* SPR 14326 Fix Start */
            if( RRM_FALSE == send_measgap )
            {
                send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_context->ran_info.rf_params
                        .rf_configurations.dl_earfcn ,
                        carrier_freq,
                        p_rrm_ue_context,
                        INTER_FREQ_BAND_LIST,
                        EUTRA_FREQ,
                        RRM_PNULL
                        );
            }

        } 
    }    
    if (object_count > p_meas_config->meas_object_to_add_mod_list.count)
    {
        /*SPR 17199 Start*/
        p_meas_config->bitmask  |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
        /*SPR 17199 End*/
        p_meas_config->meas_object_to_add_mod_list.count = object_count;
        /* SPR 14326 Fix Start */

        if( RRM_TRUE == send_measgap )
        {
            /*spr_22361_changes_start*/
            if((RRM_INTER_FREQ_FOR_SCELL == rrm_inter_freq_meas) && (p_rrm_ue_context->ca_data.is_meas_gap_enabled_for_ca))
            {
                if(RRM_ZERO != p_rrm_ue_context->meas_gap_config.meas_gap_config_type)
                {
                        p_meas_config->bitmask |= UE_ADM_MEAS_GAP_CONFIG_PRESENT;
                        p_meas_config->meas_gap_config.bitmask |= RRM_ZERO;
                        p_meas_config->meas_gap_config.meas_gap_config_type = RRM_ZERO;
                        p_rrm_ue_context->meas_gap_sent_status = RRM_UNSET;
                        p_rrm_ue_context->ca_data.is_meas_gap_enabled_for_ca = RRM_FALSE;
                }
            }
            else
            {
                /*spr_22361_changes_end*/
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "Send MEAS GAP for EUTRA INTER FREQ");
                p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
                fill_measurement_gap_info(p_meas_config, p_rrm_ue_context);
                /*spr_22361_changes_start*/
            }
            /*spr_22361_changes_end*/
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "No need to Send MEAS GAP for EUTRA INTER FREQ");
            p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
        }
        /* SPR 14326 Fix End */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "NCL list is empty");
    }
    /* Coverity Fix 87167 End */
    
    RRM_UT_TRACE_EXIT();
}
/* Inter RAT BLR Changes Start */

/* Start fix for bug 715 */
/******************************************************************************
 *  FUNCTION NAME: fill_meas_inter_rat_freq_tdd_neigh_cell_list
 *  INPUT        : p_meas_object_utra,carrier_freq
 *  OUTPUT       : p_rrm_ue_context
 *  DESCRIPTION  : Fills the utran configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/

    static rrm_return_et
fill_meas_inter_rat_freq_tdd_neigh_cell_list(
        meas_object_utra_t       *p_meas_object_utra,
        rrm_ue_context_t          *p_rrm_ue_context,
        U16                        carrier_freq
        )
{
    RRM_UT_TRACE_ENTER();

    /* SPR_14564_fix: start */
    /*bug 820 start*/
    rrm_return_et                ret_val =RRM_SUCCESS;
    /*bug 820 end*/
    /* SPR_14564_fix: end */

    U8         valid_cells                = RRM_ZERO;
    U8         neigh_count                = RRM_ZERO;
    U16        neigh_carrier_freq         = RRM_ZERO;
    U8         cell_count                 = RRM_ZERO;
    U16        phys_cell_id               = RRM_NULL;
    inter_rat_ncl_t    *p_inter_rat_ncl               = RRM_PNULL;
    cells_to_add_mod_list_utra_tdd_t    
        *p_cells_to_add_mod_list_utra_tdd         = RRM_PNULL;

    p_inter_rat_ncl              = rrm_cellm_get_inter_rat_ncl_info\
                                   (p_rrm_ue_context->cell_index);

    p_cells_to_add_mod_list_utra_tdd = &(p_meas_object_utra->
            cells_to_add_mod_list.cells_to_add_mod_list_utra_tdd);

    valid_cells            =  p_inter_rat_ncl->num_valid_utran_freq_cell;

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "For UTRAN frequency %d Neighbor cells",carrier_freq);
     /*SPR 21365 fix start*/
    for(cell_count=RRM_ZERO; ((cell_count < valid_cells) && (neigh_count < MAX_CELL_MEAS)) ; cell_count++)
    /*SPR 21365 fix stop*/
    {
        /* We are considering DL carrier freq only. */
        neigh_carrier_freq = p_inter_rat_ncl->
            utran_freq_cells[cell_count].uarfcndl;
        if ( neigh_carrier_freq == carrier_freq)
        {
            /* ho_restriction at meas config */ 
            /* SPR 17564 Fix Start */
            if(RRM_TRUE == rrm_is_inter_rat_cell_restricted(p_rrm_ue_context,
                        /* SPR 17564 Fix End */
                        p_inter_rat_ncl->utran_freq_cells[cell_count].rai.lai.plmn_identity,
                        p_inter_rat_ncl->utran_freq_cells[cell_count].rai.lai.lac,
                        UTRA_CELL,
                        &p_rrm_ue_context->ho_restriction_list))
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "Inter RAT UTRAN cell = %d, is restricted",p_inter_rat_ncl->utran_freq_cells[cell_count].pci);
            }
            else
            {
                /*spr 6991 start*/
                phys_cell_id = p_inter_rat_ncl->utran_freq_cells
                    [cell_count].pcpich_scrambling_code;
                /*spr 6991 end*/

                p_cells_to_add_mod_list_utra_tdd->cells_to_add_mod_utra_tdd[neigh_count].\
                    phys_cell_id = phys_cell_id;

                p_cells_to_add_mod_list_utra_tdd->cells_to_add_mod_utra_tdd[neigh_count].\
                    cell_index =  neigh_count +RRM_ONE;


                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "UTRAN frequency Neighbor cell = %d, is added to list",phys_cell_id);
                neigh_count++;
            }

            /*SPR 21365 fix start*/
            /*Lines Deleted*/
            /*SPR 21365 fix stop*/
        }
    }
    if (neigh_count != RRM_ZERO)
    {
        /* SPR_14564_fix: start */
        /* Lines removed */
        /* SPR_14564_fix: end */

        p_cells_to_add_mod_list_utra_tdd->count = neigh_count;

        p_meas_object_utra->bitmask |= MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_PRESENT; 
        p_meas_object_utra->cells_to_add_mod_list.bitmask |= 
            MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT;

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Total Number of UTRAN frequency Neighbor cells = %d"
                "configured are = %d",neigh_count, carrier_freq);
    }
    RRM_UT_TRACE_EXIT();
    /*bug 820 start*/
    return ret_val;
    /*bug 820 end*/
}
/* End fix for bug 715 */

/******************************************************************************
 *  FUNCTION NAME: fill_meas_inter_rat_freq_neigh_cell_list
 *  INPUT        : p_rrm_ue_context, carrier_freq
 *  OUTPUT       : p_meas_object_utra
 *  DESCRIPTION  : Fills the utran configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/

    static rrm_return_et
fill_meas_inter_rat_freq_neigh_cell_list(
        meas_object_utra_t       *p_meas_object_utra,
        rrm_ue_context_t          *p_rrm_ue_context,
        U16                        carrier_freq
        )
{
    /* SPR 22310 CID 54953 Fix Start */
    RRM_UT_TRACE_ENTER();
    /* SPR_14564_fix: start */
    /*bug 820 start*/
    rrm_return_et   ret_val = RRM_SUCCESS;	
    /*bug 820 end*/
    /* SPR_14564_fix: end */
    U8         valid_cells                = RRM_ZERO;
    U8         neigh_count                = RRM_ZERO;
    U16        neigh_carrier_freq         = RRM_ZERO;
    U8         cell_count                 = RRM_ZERO;
    U16        phys_cell_id               = RRM_NULL;
    /*SPR 21365 fix start*/
    /*Lines Deleted*/
    /*SPR 21365 fix stop*/
    inter_rat_ncl_t    *p_inter_rat_ncl               = RRM_PNULL;
    cells_to_add_mod_list_utra_fdd_t    
        *p_cells_to_add_mod_list_utra_fdd         = RRM_PNULL;

    /*SPR 21365 fix start*/
    /*Lines Deleted*/
    /*SPR 21365 fix stop*/

    p_inter_rat_ncl              = rrm_cellm_get_inter_rat_ncl_info\
                                   (p_rrm_ue_context->cell_index);

    p_cells_to_add_mod_list_utra_fdd = &(p_meas_object_utra->
            cells_to_add_mod_list.cells_to_add_mod_list_utra_fdd);    

    valid_cells            =  p_inter_rat_ncl->num_valid_utran_freq_cell;

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "For UTRAN frequency %d Neighbor cells",carrier_freq);
    /*SPR 21365 fix start*/
    for(cell_count=0; ((cell_count < valid_cells) && (neigh_count < MAX_CELL_MEAS)) ; cell_count++)
        /*SPR 21365 fix stop*/
    {
        /* We are considering UL carrier freq only. */
        neigh_carrier_freq = p_inter_rat_ncl->
            utran_freq_cells[cell_count].uarfcndl;
        if ( neigh_carrier_freq == carrier_freq)
        {
            /* ho_restriction at meas config */ 
            /* SPR 17564 Fix Start */
            /*CID 54953 Start*/
            if(RRM_TRUE == rrm_is_inter_rat_cell_restricted(p_rrm_ue_context,
                        /* SPR 17564 Fix End */
                        /*CID 54953 End*/
                        p_inter_rat_ncl->utran_freq_cells[cell_count].rai.lai.plmn_identity,
                        p_inter_rat_ncl->utran_freq_cells[cell_count].rai.lai.lac,
                        UTRA_CELL,
                        &p_rrm_ue_context->ho_restriction_list))
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Inter RAT UTRAN cell = %d, is restricted",p_inter_rat_ncl->utran_freq_cells[cell_count].pci);
            }
            else
            {
                phys_cell_id = p_inter_rat_ncl->utran_freq_cells
                    [cell_count].pcpich_scrambling_code;

                p_cells_to_add_mod_list_utra_fdd->cells_to_add_mod_utra_fdd[neigh_count].\
                    phys_cell_id = phys_cell_id;

                p_cells_to_add_mod_list_utra_fdd->cells_to_add_mod_utra_fdd[neigh_count].\
                    cell_index =  neigh_count +RRM_ONE;


                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "UTRAN frequency Neighbor cell = %d, is added to list",phys_cell_id);

                neigh_count++;
            }
            /*SPR 21365 fix start*/
            /*Lines Deleted*/
            /*SPR 21365 fix stop*/
        }
    }
    if (neigh_count != RRM_ZERO)
    {
        /* SPR_14564_fix: start */
        /* Code removed */
        /* SPR_14564_fix: end */
        p_cells_to_add_mod_list_utra_fdd->count = neigh_count;

        p_meas_object_utra->bitmask |= MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_PRESENT; 
        p_meas_object_utra->cells_to_add_mod_list.bitmask |= 
            MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT;

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Total Number of UTRAN frequency Neighbor cells = %d"
                "configured are = %d",neigh_count, carrier_freq);
    }
    /*SPR 21365 fix start*/
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                "No neighbor cell found on EARFCN %u", carrier_freq);
    }
    /*SPR 21365 fix stop*/
    /*CID 65833:end*/
    RRM_UT_TRACE_EXIT();
    /*bug 820 start*/
    return ret_val;
    /*bug 820 end*/
    /* SPR 22310 CID 54953 Fix End */
}

/* Start fix for bug 715 */
/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_utran_fdd_freq_meas_object_info
 *  DESCRIPTION  : Fills the utran FDD configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_return_et
fill_inter_rat_utran_fdd_freq_meas_object_info(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                           freq_count
        )
{
    RRM_UT_TRACE_ENTER();
    U8                           meas_object_id     = RRM_ZERO;
    meas_object_utra_t          *p_meas_object_utra = RRM_PNULL;
    irat_eutran_utran_fdd_list_t 
        *p_irat_eutran_utran_fdd_list = RRM_PNULL;
    /*csg proximity start*/
    rrm_proximity_ind_t *p_rrm_proximity_ind = RRM_NULL;
    /*csg proximity end*/
    /* Rel10 CR-835 start */
    /* Rel10 CR-835 end */
    /*bug 820 start*/
    rrm_return_et                ret_val = RRM_FAILURE;
    /*bug 820 end*/

    /* Rel10 CR-835 start */
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

    }
    /* Rel10 CR-835 end */

    p_meas_object_utra = &(p_meas_object_to_add_mod->meas_object.meas_object_utra);

    /* Rel10 CR-835 start */
    /*Start:Bug 762*/
    if (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_TWO))
        /*End:Bug 762*/
    {
        /* Rel10 CR-835 end */
        /* SPR_15852_fix: code removed */
        meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);

        if(meas_object_id <= MAX_MEAS_OBJECT_ID)
        {
            /* SPR_15852_fix: start */
            p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_UTRA_PRESENT;
            /* SPR_15852_fix: end */
            p_meas_object_to_add_mod->meas_object_id = meas_object_id;

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Measurement object %d is created for Inter RAT freq = %d",
                    meas_object_id,
                    p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq);

            if( p_idle_mode_mobility_params->idle_mode_mobility_irat_utra_params.bitmask & 
                    RRMCM_RMIF_IRAT_EUTRAN_UTRAN_FDD_LIST_PRESENT)
            {
                p_irat_eutran_utran_fdd_list = &(p_idle_mode_mobility_params->\
                        idle_mode_mobility_irat_utra_params.irat_eutran_to_utran_fdd_list);

                /*csg proximity start*/
                /*configure measurement on the carrier freq received in
                  proximity ind*/
                if(RRC_RRM_PROXIMITY_IND_EVENT == p_rrm_ue_context->ue_event)
                {
                    p_rrm_proximity_ind = (rrm_proximity_ind_t *)
                        p_rrm_ue_context->current_procedure.p_proc_data;
                    p_meas_object_utra->carrier_freq =
                        p_rrm_proximity_ind->carrier_Freq.carrier_freq_utra;
                }
                else
                {
                    p_meas_object_utra->carrier_freq = p_irat_eutran_utran_fdd_list->
                        irat_eutran_to_utran_fdd_carriers[freq_count].utra_carrier_arfcn;
                }
                /*csg proximity end*/
                /* Have to discuss whether it should be OAM configured parameter or not.So, for 
                 * time being it is hard coded to 0 */
                /* OFFSET FREQ CHANGES START */
                p_meas_object_utra->offset_freq  = p_irat_eutran_utran_fdd_list->
                    irat_eutran_to_utran_fdd_carriers[freq_count].offset_freq;
                /* OFFSET FREQ CHANGES END */
                /*bug 820 start*/
                ret_val =  fill_meas_inter_rat_freq_neigh_cell_list(p_meas_object_utra,p_rrm_ue_context, 
                        p_meas_object_utra->carrier_freq); 
                /*bug 820 end*/
            }
            else
            {
                /*SPR 21365 fix start*/
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "Inter RAT eutra to utra FDD list is not present ");
                /*SPR 21365 fix stop*/
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Maximum Measurement object Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
        }
        /* Rel10 CR-835 start */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "FGI bit is not set for UTRAN Measurement for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    /* Rel10 CR-835 end */
    RRM_UT_TRACE_EXIT();
    /*bug 820 start*/
    return ret_val;
    /*bug 820 end*/
}

/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_utran_tdd_freq_meas_object_info
 *  DESCRIPTION  : Fills the utran tdd configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_return_et
fill_inter_rat_utran_tdd_freq_meas_object_info(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                           freq_count
        )
{
    RRM_UT_TRACE_ENTER();
    U8                           meas_object_id     = RRM_ZERO;
    meas_object_utra_t          *p_meas_object_utra = RRM_PNULL;
    irat_eutran_utran_tdd_list_t 
        *p_irat_eutran_utran_tdd_list = RRM_PNULL;

    /* Rel10 CR-835 start */
    /* Rel10 CR-835 end */
    /*bug 820 start*/
    rrm_return_et                ret_val =RRM_FAILURE;
    /*bug 820 end*/

    /* Rel10 CR-835 start */
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

    }
    /* Rel10 CR-835 end */
    p_meas_object_utra = &(p_meas_object_to_add_mod->meas_object.meas_object_utra);

    /* Rel10 CR-835 start */
    /*Start:Bug 762*/
    if (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_TWO))
        /*End:Bug 762*/
    {
        /* Rel10 CR-835 end */
        /* SPR_15852_fix: code removed */
        meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);

        if(meas_object_id <= MAX_MEAS_OBJECT_ID)
        {
            p_meas_object_to_add_mod->meas_object_id = meas_object_id;
            /* SPR_15852_fix: start */
            p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_UTRA_PRESENT;
            ret_val = RRM_SUCCESS;
            /* SPR_15852_fix: end */

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Measurement object %d is created for Inter RAT freq = %d",
                    meas_object_id,
                    p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq);

            if( p_idle_mode_mobility_params->idle_mode_mobility_irat_utra_params.bitmask & 
                    RRMCM_RMIF_IRAT_EUTRAN_UTRAN_TDD_LIST_PRESENT)
            {
                p_irat_eutran_utran_tdd_list = &(p_idle_mode_mobility_params->\
                        idle_mode_mobility_irat_utra_params.irat_eutran_to_utran_tdd_list);

                p_meas_object_utra->carrier_freq = p_irat_eutran_utran_tdd_list->
                    irat_eutran_to_utran_tdd_carriers[freq_count].utra_carrier_arfcn;
                /* 
                 * Have to discuss whether it should be OAM configured parameter or not.So, for 
                 * time being it is hard coded to 0 
                 */
                p_meas_object_utra->offset_freq  = get_irat_offset(p_rrm_ue_context->cell_index);
                /*bug 820 start*/
                ret_val = fill_meas_inter_rat_freq_tdd_neigh_cell_list(p_meas_object_utra,p_rrm_ue_context, 
                        p_meas_object_utra->carrier_freq); 
                /*bug 820 end*/
                /*SPR 21365 fix start*/
                if(RRM_FAILURE == ret_val)
                {
                    rrm_uem_deallocate_used_id(p_rrm_ue_context,MEAS_CONFIG,meas_object_id);
                    p_meas_object_to_add_mod->meas_object.bitmask = RRM_ZERO;
                }
                /*SPR 21365 fix stop*/  

            }
            else
            {
                /*SPR 21365 fix start*/
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "Inter RAT eutra to utra TDD list is not present ");
                /*SPR 21365 fix stop*/
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Maximum Measurement object Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
        }
        /* Rel10 CR-835 start */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "FGI bit is not set for UTRAN Measurement for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    /* Rel10 CR-835 end */
    RRM_UT_TRACE_EXIT();
    /*bug 820 start*/
    return ret_val;
    /*bug 820 end*/
}
/* End fix for bug 715 */

/******************************************************************************
 *  FUNCTION NAME: fill_meas_inter_rat_cdma2000_freq_neigh_cell_list
 *  INPUT        : p_rrm_ue_context,freq_count,meas_object_id
 *  OUTPUT       : p_meas_object_cdma2000
 *  DESCRIPTION  : Fills the utran configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/

    static rrm_void_t
fill_meas_inter_rat_cdma2000_freq_neigh_cell_list(
        meas_object_cdma2000_t    *p_meas_object_cdma2000,
        rrm_ue_context_t          *p_rrm_ue_context,
        U8                         freq_count,
        U8                         meas_object_id
        )
{
    U8 cell_count      = RRM_ZERO;
    U16 carrier_freq   = RRM_ZERO;
    U8 cdma2000_type   = RRM_ZERO;
    U8 band_class      = RRM_ZERO;
    U16 phys_cell_id   = RRM_ZERO;
    U8 neigh_count     = RRM_ZERO;
    U8 valid_cells     = RRM_ZERO;
    //U8 neigh_count_1xrtt = RRM_ZERO;
    //U8 neigh_count_hrpd = RRM_ZERO;
    U8 neigh_count_1xrtt = RRM_ONE;
    U8 neigh_count_hrpd = RRM_ONE;
    rrm_bool_et      PCI_FILLED_INDEX = RRM_FALSE;
    U8 ncl_count_hrpd = RRM_ZERO;
    U8 ncl_count_1xrtt = RRM_ZERO;
    U8 bc_1xrtt_count = RRM_ZERO;
    U8 bc_hrpd_count = RRM_ZERO;
    U8 valid_ncl_index = RRM_ZERO;
    U8 valid_bc_1xrtt_index = RRM_ZERO;
    U8 valid_bc_hrpd_index = RRM_ZERO;
    U8 xrtt_meas_object_count = RRM_ZERO;
    U8 hrpd_meas_object_count = RRM_ZERO;
    /*SPR 21365 fix start*/
    /*Lines Deleted*/
    /*SPR 21365 fix stop*/


        inter_rat_ncl_t    *p_inter_rat_ncl               = RRM_PNULL;
        rrmcm_rmif_cdma2000_freq_cells_t  *p_cdma2000_freq_cells = RRM_PNULL;
        cells_to_add_mod_list_cdma2000_list_t  *p_cells_to_add_mod_cdma2000_list = RRM_PNULL;
        idle_mode_mobility_params_t       *p_irat_cdma2000 = RRM_PNULL;

        p_irat_cdma2000 = rrm_cellm_get_idle_mode_params(p_rrm_ue_context->cell_index);

        RRM_UT_TRACE_ENTER();


        if (RRM_PNULL == p_rrm_ue_context->p_meas_obj_cdma2000_info)
        {
          p_rrm_ue_context->p_meas_obj_cdma2000_info = (rrm_ue_meas_obj_cdma2000_info_t*)rrm_mem_get(sizeof(rrm_ue_meas_obj_cdma2000_info_t));
          if (RRM_PNULL == p_rrm_ue_context->p_meas_obj_cdma2000_info)
          {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Failed to allocate memory CDMA neighbour frequency %d Neighbor cells for saving in UE context",
                p_meas_object_cdma2000->carrier_freq_cdma2000.arfcn);
            RRM_UT_TRACE_EXIT();
            return;
          }
        }
        xrtt_meas_object_count = p_rrm_ue_context->p_meas_obj_cdma2000_info->count_meas_object_1xrtt;
        hrpd_meas_object_count = p_rrm_ue_context->p_meas_obj_cdma2000_info->count_meas_object_hrpd;


        p_cells_to_add_mod_cdma2000_list = &p_meas_object_cdma2000-> \
                                           cells_to_add_mod_list;

        carrier_freq = p_meas_object_cdma2000->carrier_freq_cdma2000.arfcn;
        band_class = p_meas_object_cdma2000->carrier_freq_cdma2000.band_class;
        cdma2000_type = p_meas_object_cdma2000->cdma2000_type;

        p_inter_rat_ncl = rrm_cellm_get_inter_rat_ncl_info\
                          (p_rrm_ue_context->cell_index);

        p_cdma2000_freq_cells = p_inter_rat_ncl->cdma2000_freq_cells;
        valid_cells           =  p_inter_rat_ncl->num_valid_cdma2000_freq_cells;

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "For CDMA frequency %d Neighbor cells",p_meas_object_cdma2000->carrier_freq_cdma2000.arfcn);






        for(cell_count=RRM_ZERO; (cell_count < valid_cells); cell_count++)
        {

            if(RRM_ZERO == RRM_MEMCMP(p_cdma2000_freq_cells[cell_count].CID,
                        p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.cdma2000_cell_param.cell_id_hrpd.cdma2000_hrpd_cell_id,
                        RRM_CDMA2000_HRPD_CELL_ID_LENGTH))
            {
                continue;
            }

            if(RRM_ZERO == RRM_MEMCMP(p_cdma2000_freq_cells[cell_count].CID,
                        p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.cdma2000_cell_param.cell_id_1xrtt.cdma2000_1xrtt_cell_id,
                        RRMCM_RMIF_CDMA2000_1XRTT_CELL_ID_LENGTH))
            {
                continue;

            }


            PCI_FILLED_INDEX = RRM_FALSE;
            if (p_cdma2000_freq_cells[cell_count].type == cdma2000_type)
            {
                if ((p_cdma2000_freq_cells[cell_count].arfcn == carrier_freq) && 
                        (p_cdma2000_freq_cells[cell_count].band_class == band_class))
                {
                    if (p_cdma2000_freq_cells[cell_count].band_class == band_class)
                    {
                        phys_cell_id = p_cdma2000_freq_cells[cell_count].pn_offset;

                        /* to fill ARFCN, BC, PCI */
                        PCI_FILLED_INDEX = RRM_TRUE;

                        p_cells_to_add_mod_cdma2000_list-> \
                            cells_to_add_mod_cdma2000[neigh_count].\
                            phys_cell_id = phys_cell_id;

                        p_cells_to_add_mod_cdma2000_list-> \
                            cells_to_add_mod_cdma2000[neigh_count].\
                            cell_index =  neigh_count +RRM_ONE;

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "CDMA2000 frequency Neighbor cell = %d, is added to list",phys_cell_id);

                        neigh_count++;
		    /*SPR 21365 fix start*/
                    /*Lines Deleted*/
		    /*SPR 21365 fix stop*/
                    }
                }
            }
            /* If PCI is filled, then store BC, ARFCN, PCI in UE Context */
            if(PCI_FILLED_INDEX)
            {
                if(p_cdma2000_freq_cells[cell_count].type == RRM_OAM_NCL_TYPE_1XRTT)
                {
                    /* Saving Mease Object's parameters in the UE context before neighbour cell's params */

                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].type = \
                        p_cdma2000_freq_cells[freq_count].type;
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].arfcn =
                        p_cdma2000_freq_cells[freq_count].arfcn;
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].pn_offset = 
                        p_cdma2000_freq_cells[freq_count].pn_offset;
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].band_class =
                        p_cdma2000_freq_cells[freq_count].band_class;

                    RRM_MEMCPY(&(p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                                meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                                cdma2000_ncl_info_list[RRM_ZERO].CID),
                            &(p_cdma2000_freq_cells[freq_count].CID),
                            RRMCM_RMIF_CDMA2000_1XRTT_CELL_ID_LENGTH);

                    /* End of Saving Mease Object's parameters in the UE context before neighbour cell's params */
                    /* Klocwork_fix_start : cdma2000_ncl_info_list */
                    if (neigh_count_1xrtt < RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS)
                    {
                        /* Klocwork_fix_end : cdma2000_ncl_info_list */
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_1xrtt].type =
                            p_cdma2000_freq_cells[cell_count].type;
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_1xrtt].arfcn =
                            p_cdma2000_freq_cells[cell_count].arfcn;
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_1xrtt].pn_offset = 
                            p_cdma2000_freq_cells[cell_count].pn_offset;
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_1xrtt].band_class =
                            p_cdma2000_freq_cells[cell_count].band_class;

                        RRM_MEMCPY(&(p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                                    meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                                    cdma2000_ncl_info_list[neigh_count_1xrtt].CID),
                                &(p_cdma2000_freq_cells[cell_count].CID),
                                RRMCM_RMIF_CDMA2000_1XRTT_CELL_ID_LENGTH);
                        /* Klocwork_fix_start : cdma2000_ncl_info_list */
                    }
                    /* Klocwork_fix_end : cdma2000_ncl_info_list */

                    neigh_count_1xrtt++;
                }
                if(p_cdma2000_freq_cells[cell_count].type == RRM_OAM_NCL_TYPE_HRPD)
                {
                    /* Saving Mease Object's parameters in the UE context before neighbour cell's params */
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].type = \
                        p_cdma2000_freq_cells[freq_count].type;
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].arfcn =
                        p_cdma2000_freq_cells[freq_count].arfcn;
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].pn_offset = 
                        p_cdma2000_freq_cells[freq_count].pn_offset;
                    p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                        meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                        cdma2000_ncl_info_list[RRM_ZERO].band_class =
                        p_cdma2000_freq_cells[freq_count].band_class;

                    RRM_MEMCPY(&(p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                                meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                                cdma2000_ncl_info_list[RRM_ZERO].CID),
                            &(p_cdma2000_freq_cells[freq_count].CID),
                            RRM_CDMA2000_HRPD_CELL_ID_LENGTH);

                    /* End of Saving Mease Object's parameters in the UE context before neighbour cell's params */
                    /* Klocwork_fix_start : cdma2000_ncl_info_list */
                    if (neigh_count_hrpd < RRMCM_RMIF_NUM_CDMA2000_FREQ_CELLS)
                    {
                        /* Klocwork_fix_end : cdma2000_ncl_info_list */

                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_hrpd].type =
                            p_cdma2000_freq_cells[cell_count].type;
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_hrpd].arfcn =
                            p_cdma2000_freq_cells[cell_count].arfcn;
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_hrpd].pn_offset = 
                            p_cdma2000_freq_cells[cell_count].pn_offset;
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                            cdma2000_ncl_info_list[neigh_count_hrpd].band_class =
                            p_cdma2000_freq_cells[cell_count].band_class;

                        RRM_MEMCPY(&(p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                                    meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                                    cdma2000_ncl_info_list[neigh_count_hrpd].CID),
                                &(p_cdma2000_freq_cells[cell_count].CID),
                                RRM_CDMA2000_HRPD_CELL_ID_LENGTH);
                        /* Klocwork_fix_start : cdma2000_ncl_info_list */
                    }
                    /* Klocwork_fix_end : cdma2000_ncl_info_list */

                    neigh_count_hrpd++;
                }
            }
        }



        /* Meas Object count, Meas object id, Priority to be filled */
        if(p_meas_object_cdma2000->cdma2000_type == RRM_OAM_NCL_TYPE_HRPD)
        {
            p_rrm_ue_context->p_meas_obj_cdma2000_info->
                meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count]. \
                meas_obj_id = meas_object_id; 
            p_rrm_ue_context->p_meas_obj_cdma2000_info->
                meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count]. \
                ncl_count = neigh_count_hrpd;
            p_rrm_ue_context->p_meas_obj_cdma2000_info->count_meas_object_hrpd++;
        }
        else if(p_meas_object_cdma2000->cdma2000_type == RRM_OAM_NCL_TYPE_1XRTT)
        {
            p_rrm_ue_context->p_meas_obj_cdma2000_info->
                meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count]. \
                meas_obj_id = meas_object_id;
            p_rrm_ue_context->p_meas_obj_cdma2000_info->
                meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count]. \
                ncl_count = neigh_count_1xrtt;
            p_rrm_ue_context->p_meas_obj_cdma2000_info->count_meas_object_1xrtt++;
        }


        ncl_count_hrpd = p_rrm_ue_context->p_meas_obj_cdma2000_info->
            meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].ncl_count;
        ncl_count_1xrtt = p_rrm_ue_context->p_meas_obj_cdma2000_info->
            meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].ncl_count;

        p_irat_cdma2000 = rrm_cellm_get_idle_mode_params(p_rrm_ue_context->cell_index);

        bc_1xrtt_count = p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.
            cell_reselection_params_1xrtt.band_class_list.count;
        bc_hrpd_count = p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.
            cell_reselection_params_hrpd.band_class_list.count;

        for(valid_ncl_index = RRM_ZERO; 
                valid_ncl_index < ncl_count_1xrtt;
                valid_ncl_index++)
        {
            if(RRM_OAM_NCL_TYPE_1XRTT == p_rrm_ue_context->p_meas_obj_cdma2000_info->
                    meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count]. \
                    cdma2000_ncl_info_list[valid_ncl_index].type)
            {
                for(valid_bc_1xrtt_index = RRM_ZERO;
                        valid_bc_1xrtt_index < bc_1xrtt_count;
                        valid_bc_1xrtt_index++)
                {
                    if((p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                                meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                                cdma2000_ncl_info_list[valid_ncl_index].band_class) == 
                            (p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.
                             cell_reselection_params_1xrtt.band_class_list.
                             band_class_info_cdma2000[valid_bc_1xrtt_index].
                             band_class))
                    {
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_1xrtt[xrtt_meas_object_count].
                            cdma2000_ncl_info_list[valid_ncl_index].cell_reselection_priority = 
                            p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.
                            cell_reselection_params_1xrtt.band_class_list.
                            band_class_info_cdma2000[valid_bc_1xrtt_index].
                            cell_reselection_priority;
                    }
                }
            }
        }



        for(valid_ncl_index = RRM_ZERO;
                valid_ncl_index < ncl_count_hrpd;
                valid_ncl_index++)
        {
            if(RRM_OAM_NCL_TYPE_HRPD == p_rrm_ue_context->p_meas_obj_cdma2000_info->
                    meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count]. \
                    cdma2000_ncl_info_list[valid_ncl_index].type)
            {
                for(valid_bc_hrpd_index = RRM_ZERO;
                        valid_bc_hrpd_index < bc_hrpd_count;
                        valid_bc_hrpd_index++)
                {
                    if((p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                                meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                                cdma2000_ncl_info_list[valid_ncl_index].
                                band_class) == (p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.
                                    cell_reselection_params_hrpd.band_class_list.
                                    band_class_info_cdma2000[valid_bc_hrpd_index].band_class))
                    {
                        p_rrm_ue_context->p_meas_obj_cdma2000_info-> \
                            meas_obj_cdma2000_cell_info_hrpd[hrpd_meas_object_count].
                            cdma2000_ncl_info_list[valid_ncl_index].
                            cell_reselection_priority = p_irat_cdma2000->idle_mode_mobility_irat_cdma_params.
                            cell_reselection_params_hrpd.band_class_list.
                            band_class_info_cdma2000[valid_bc_hrpd_index].
                            cell_reselection_priority;
                    }
                }
            }
        }
        if ((neigh_count != RRM_ZERO) && ((neigh_count_hrpd != RRM_ZERO) || (neigh_count_1xrtt != RRM_ZERO)))
        {
            p_cells_to_add_mod_cdma2000_list->count = neigh_count;

            p_meas_object_cdma2000->bitmask |= MEAS_OBJECT_CDMA2000_CELLS_TO_ADD_MOD_LIST_PRESENT; 

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Total Number of CDMA2000 frequency Neighbor cells = %d"
                    "configured are = %d",neigh_count, carrier_freq);
        }
    /*SPR 21365 fix start*/
        else
        {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "CDMA2000 cells to add mod list is not configured as neighbour count is %u"
                    "neigh_count_hrpd %u neigh_count_1xrtt %u",
                    neigh_count,neigh_count_hrpd,neigh_count_1xrtt);
        }
    /*Lines Deleted*/
    /*SPR 21365 fix stop*/
    RRM_UT_TRACE_EXIT();
}


/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_cdma2000_freq_meas_object_info
 *  INPUT        : p_cdma2000_freq_cells,p_rrm_ue_context,p_idle_mode_mobility_params
 freq_count
 *  OUTPUT       : p_meas_object_to_add_mod
 *  DESCRIPTION  : Fills the cdma2000 configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
 /*SPR 21365 fix start*/
static rrm_return_et
/*SPR 21365 fix stop*/
fill_inter_rat_cdma2000_freq_meas_object_info(
        meas_object_to_add_mod_t           *p_meas_object_to_add_mod,
        rrm_ue_context_t                   *p_rrm_ue_context,
        rrmcm_rmif_cdma2000_freq_cells_t   *p_cdma2000_freq_cells,
        idle_mode_mobility_params_t        *p_idle_mode_mobility_params,
        U8                                  freq_count
        )
{
    RRM_UT_TRACE_ENTER();
    U8                           meas_object_id     = RRM_ZERO;
    meas_object_cdma2000_t       *p_meas_object_cdma2000 = RRM_PNULL;
    /* Rel10 CR-835 start */
    /* Rel10 CR-835 end */
    /*SPR 21365 fix start*/
    rrm_return_et        ret_val = RRM_FAILURE;
    /*SPR 21365 fix stop*/

    p_meas_object_cdma2000 = &(p_meas_object_to_add_mod->meas_object.meas_object_cdma2000);

    /* Rel10 CR-835 start */
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

    }
    /*Start:Bug 762*/
    if (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_FOUR) ||( rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_SIX))) 
        /*End:Bug 762*/
    {
        /* Rel10 CR-835 end */
        /* SPR_15852_fix: code removed */
        meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);

        if(meas_object_id <= MAX_MEAS_OBJECT_ID)
        {
            p_meas_object_to_add_mod->meas_object_id = meas_object_id;
            /* SPR_15852_fix: start */
            p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT;
            /* SPR_15852_fix: end */

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Measurement object %d is created for Inter RAT freq = %d",
                    meas_object_id,
                    p_meas_object_to_add_mod->meas_object.meas_object_cdma2000.carrier_freq_cdma2000.arfcn);

            p_meas_object_cdma2000->carrier_freq_cdma2000.arfcn = p_cdma2000_freq_cells[freq_count].arfcn;
            p_meas_object_cdma2000->carrier_freq_cdma2000.band_class = p_cdma2000_freq_cells[freq_count].band_class;
            if (RRM_OAM_NCL_TYPE_1XRTT == p_cdma2000_freq_cells[freq_count].type)
            {
                p_meas_object_cdma2000->cdma2000_type = RRM_RRC_TYPE1_XRTT;
            }
            else if (RRM_OAM_NCL_TYPE_HRPD == p_cdma2000_freq_cells[freq_count].type)
            {
                p_meas_object_cdma2000->cdma2000_type = RRM_RRC_TYPE_HRPD;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Undefined cell type = %d", p_meas_object_cdma2000->cdma2000_type);
            }

            /* Have to discuss whether it should be OAM configured parameter or not.So, for 
             * time being it is hard coded to 0 */
            p_meas_object_cdma2000->offset_freq  = get_irat_offset(p_rrm_ue_context->cell_index);

            p_meas_object_cdma2000->search_window_size = p_idle_mode_mobility_params-> \
                                                         idle_mode_mobility_irat_cdma_params. \
                                                         search_window_size;


            fill_meas_inter_rat_cdma2000_freq_neigh_cell_list(
                    p_meas_object_cdma2000, 
                    p_rrm_ue_context,
                    freq_count,
                    meas_object_id);
            /*SPR 21365 fix start*/
    	    ret_val = RRM_SUCCESS;
	        /*SPR 21365 fix stop*/

        }
        else
        {
            /*SPR 21365 fix start*/
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "Maximum Measurement object Id reached");
            /*SPR 21365 fix stop*/
        }
        /* Rel10 CR-835 start */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "FGI bit is not set for CDMA Measurement for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    /* Rel10 CR-835 end */
    RRM_UT_TRACE_EXIT();
    /*SPR 21365 fix start*/
    return ret_val;
    /*SPR 21365 fix stop*/

}


/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_geran_freq_meas_object_info
 *  INPUT        : p_rrm_ue_context,p_idle_mode_mobility_params,freq_count
 *  OUTPUT       : p_meas_object_to_add_mod
 *  DESCRIPTION  : Fills the geran configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
 /*SPR 21365 fix start*/
static rrm_return_et
/*SPR 21365 fix stop*/
fill_inter_rat_geran_freq_meas_object_info(
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                           freq_count
        )
{
    RRM_UT_TRACE_ENTER();
    U8                           meas_object_id     = RRM_ZERO;
    U8                           count = RRM_ZERO;
    meas_object_geran_t          *p_meas_object_geran = RRM_PNULL;
    irat_eutran_geran_list_t 
        *p_irat_eutran_geran_list = RRM_PNULL;
    rrm_son_geran_carrier_freqs_t *p_geran_carr_freqs =RRM_PNULL;    
    /* Rel10 CR-835 start */
    /* Rel10 CR-835 end */
    /*SPR 21365 fix start*/
    rrm_return_et        ret_val = RRM_FAILURE;
    /*SPR 21365 fix stop*/


    /* Rel10 CR-835 start */
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

    }
    /* Rel10 CR-835 end */
    p_meas_object_geran = &(p_meas_object_to_add_mod->meas_object.meas_object_geran);

    /* Rel10 CR-835 start */
    /*Start:Bug 762*/
    if (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_THREE))
        /*End:Bug 762*/
    {
        /* SPR_15852_fix: code removed */
        meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);

        if(meas_object_id <= MAX_MEAS_OBJECT_ID)
        {
            p_meas_object_to_add_mod->meas_object_id = meas_object_id;
            /* SPR_15852_fix: start */
            p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_GERAN_PRESENT;
            /* SPR_15852_fix: end */

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Measurement object %d is created for Inter RAT freq = %d",
                    meas_object_id,
                    p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq);

            if( p_idle_mode_mobility_params->idle_mode_mobility_irat_geran_params.bitmask & 
                    RRMCM_RMIF_IRAT_EUTRAN_GERAN_FREQ_LIST_PRESENT)
            {
                p_irat_eutran_geran_list = &(p_idle_mode_mobility_params->
                        idle_mode_mobility_irat_geran_params.irat_eutran_to_geran_list);
                p_geran_carr_freqs = &(p_irat_eutran_geran_list->
                        irat_eutran_to_geran_carriers[freq_count].geran_car_freqs);
                p_meas_object_geran->carrier_freqs.starting_arfcn = \
                                                                    p_geran_carr_freqs->starting_arfcn;
                if(p_geran_carr_freqs->band_ind == RRM_SON_GERAN_DCS_1800)
                {
                    p_meas_object_geran->carrier_freqs.band_indicator = RRM_RRC_DCS1800;
                } 
                else
                {
                    p_meas_object_geran->carrier_freqs.band_indicator = RRM_RRC_PCS1900;
                }               
                if(p_geran_carr_freqs->following_arfcns.bitmask &
                        RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT) 
                {
                    p_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask |=\
                                                                                           GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG;
                    p_meas_object_geran->carrier_freqs.following_arfcns.\
                        explicit_list_of_arfcns.count = p_geran_carr_freqs->following_arfcns.exp_arfcn_list_size;
                    for(count=RRM_ZERO;count<p_geran_carr_freqs->following_arfcns.exp_arfcn_list_size;count++)
                    {                  
                        p_meas_object_geran->carrier_freqs.following_arfcns.\
                            explicit_list_of_arfcns.data[count] =\
                            p_geran_carr_freqs->following_arfcns.exp_arfcn_list[count];
                    }               
                }
                if(p_geran_carr_freqs->following_arfcns.bitmask &
                        RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT) 
                {
                    p_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask |=\
                                                                                           GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG;
                    p_meas_object_geran->carrier_freqs.following_arfcns.\
                        equally_spaced_arfcns.arfcn_spacing = p_geran_carr_freqs->following_arfcns.arfcn_spacing;
                    p_meas_object_geran->carrier_freqs.following_arfcns.\
                        equally_spaced_arfcns.num_of_following_arfcns =\
                        p_geran_carr_freqs->following_arfcns.num_of_following_arfcn;
                }
                if(p_geran_carr_freqs->following_arfcns.bitmask &
                        RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT) 
                {
                    p_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask |=\
                                                                                           GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG;
                    p_meas_object_geran->carrier_freqs.following_arfcns.\
                        var_bitmap_of_arfcns.count = 
                        p_geran_carr_freqs->following_arfcns.arfcn_bmp_list_size;
                    for(count=RRM_ZERO;count<p_geran_carr_freqs->following_arfcns.arfcn_bmp_list_size;count++)
                    {                  
                        p_meas_object_geran->carrier_freqs.following_arfcns.\
                            var_bitmap_of_arfcns.data[count] =\
                            p_geran_carr_freqs->following_arfcns.arfcn_bmp_list[count];
                    }               
                }
                /* OFFSET FREQ CHANGES START */
                p_meas_object_geran->offset_freq  = p_irat_eutran_geran_list->
                    irat_eutran_to_geran_carriers[freq_count].offset_freq;
                /* OFFSET FREQ CHANGES END */
                p_meas_object_geran->ncc_Permitted  = p_irat_eutran_geran_list->
                    irat_eutran_to_geran_carriers[freq_count].ncc_permitted;
		        /*SPR 21365 fix start*/
        		ret_val = RRM_SUCCESS;
		        /*SPR 21365 fix stop*/
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "Inter RAT eutra to GERAN list is not present ");
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Maximum Measurement object Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
        }
        /* Rel10 CR-835 start */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "FGI bit is not set for GERAN Measurement for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    /* Rel10 CR-835 end */
    RRM_UT_TRACE_EXIT();
    /*SPR 21365 fix start*/
    return ret_val;
    /*SPR 21365 fix stop*/
}

/*Klockwork_fix_2431_start*/
/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_freq_utra_meas_config_object
 *  INPUT        : p_rrm_ue_context,utran_supported
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the utran configuration in meas config list with values
 *                 received from OAM to be sent to RRC.
 *
 *  RETURNS      :      None
 ******************************************************************************/
static rrm_void_t
fill_inter_rat_freq_utra_meas_config_object(rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        rrm_bool_et                  utran_supported,
        U8                           *object_count,
        /*SPR 21365 fix start*/
        /* Lines Deleted */
        /*SPR 21365 fix stop*/
        /*SPR 14326 Start*/
        rrm_bool_et *send_measgap,
        rrm_cell_context_t    *p_cell_ctx
        /*SPR 14326 End*/
        )
{
    rrm_return_et ret_val;
    U8     utran_neighbour_count                               = RRM_NULL;
    rrm_proximity_ind_t *p_rrm_proximity_ind = RRM_PNULL;
    irat_eutran_utran_fdd_list_t  *p_irat_eutran_utran_fdd_list= RRM_PNULL;
    U8     valid_freqs                                         = RRM_ZERO;
    U8     carrier_freq_configured                             = RRM_ZERO;
    U16    carrier_freq                                        = RRM_ZERO;
    rrm_bool_et                   freq_cells_exist             = RRM_FALSE;
    ncl_params_t    *p_ncl_params            = RRM_PNULL;
    U8     counter                                             = RRM_ZERO;
    U8     freq_count                                          = RRM_ZERO;
    meas_object_to_add_mod_t      *p_meas_object_to_add_mod    = RRM_PNULL;
    meas_report_info_node_t  *p_meas_object_config = RRM_PNULL;
    irat_eutran_utran_tdd_list_t  *p_irat_eutran_utran_tdd_list= RRM_PNULL;
    rrm_bool_et                   ret_car_freq_supported       = RRM_FALSE;
    /*SPR 14564 Start*/
    rrm_bool_et rrm_is_utra_restricted = RRM_FALSE;
    rrm_bool_et rrm_is_utra_ncl_empty = RRM_FALSE;
    /*SPR 14564 End*/
    /*SPR 21365 fix start*/
    U8    meas_config_count                     = RRM_ZERO;
    /*SPR 21365 fix stop*/


    RRM_UT_TRACE_ENTER();

    p_ncl_params                = rrm_cellm_get_ncl_info(p_rrm_ue_context->cell_index);
    utran_neighbour_count       = p_ncl_params->inter_rat_ncl.num_valid_utran_freq_cell;
    /*SPR 21365 fix start*/
     meas_config_count = rrm_count_measurements_for_ho_configured(p_rrm_ue_context) + 
                            p_meas_config->meas_object_to_add_mod_list.count;
    /*SPR 21365 fix stop*/

    if(RRC_RRM_PROXIMITY_IND_EVENT == p_rrm_ue_context->ue_event)
    {
        p_rrm_proximity_ind = (rrm_proximity_ind_t *)
            p_rrm_ue_context->current_procedure.p_proc_data;
        if (!(p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "utra carrier freq is not received in proximity ind, so return from here"); 
            return;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "utra cell carrier freq is received in proximity ind"); 
        }
    }
    /* End fix for SPR 962 */
    /*Start:Bug 762*/
    if(rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_EIGHT) && 
            rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_TWO))
        /*End:Bug 762*/
    { 
        /*bug 820 start*/
        /* SPR_14564_fix: start */
		/*SPR 21687 Start*/
        if(is_ue_restricted_for_given_rat(p_rrm_ue_context, RRM_UTRAN_CELL))
		{
			rrm_is_utra_restricted = RRM_TRUE;
		}
		/*SPR 21687 End*/
        /*bug 820 end*/
        if (utran_neighbour_count >=RRM_ONE )
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "UTRAN neighbors present. ");
        }
        else 
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "UTRAN neighbor is not configured " 
                    "will configure Idle Mode UTRA frequencies.");
            rrm_is_utra_ncl_empty = RRM_TRUE;
            /* SPR 14239 Fix Start */
            if(p_rrm_ue_context->ue_context_mod_req.bitmask & RRM_UEM_CTX_CSF_INDICATOR_PRESENT)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "UTRA NCL is not present in the CSFB scenario for [UE:%d]", p_rrm_ue_context->ue_index);
                rrm_is_utra_restricted = RRM_TRUE;
            }
            /* SPR 14239 Fix End */ 
        }	      

        if(( p_idle_mode_mobility_params->bitmask & RRMCM_RMIF_IDLE_MODE_MOBILITY_IRAT_UTRA_PRESENT) && 
                ( p_idle_mode_mobility_params->idle_mode_mobility_irat_utra_params.bitmask & 
                  RRMCM_RMIF_IRAT_EUTRAN_UTRAN_FDD_LIST_PRESENT ) &&
                RRM_FALSE == rrm_is_utra_restricted)
        {
            /* SPR_14564_fix: end */
            p_irat_eutran_utran_fdd_list = &(p_idle_mode_mobility_params->\
                    idle_mode_mobility_irat_utra_params.irat_eutran_to_utran_fdd_list);

            valid_freqs = p_irat_eutran_utran_fdd_list->num_irat_eutran_to_utran_fdd_carriers;

            for( freq_count = RRM_ZERO; freq_count < valid_freqs ; freq_count++)
            {  
                carrier_freq_configured = RRM_ZERO;
                carrier_freq = p_irat_eutran_utran_fdd_list->\
                               irat_eutran_to_utran_fdd_carriers[freq_count].utra_carrier_arfcn;

                /* Start fix for SPR 962 */
                if ((p_rrm_proximity_ind != RRM_PNULL) &&
                        (p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT) &&
                        (p_rrm_proximity_ind->carrier_Freq.carrier_freq_utra != carrier_freq))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "Proximity case: utran carrier freq [%d] does not match , check next", carrier_freq);
                    continue; 
                }
                /* End fix for SPR 962 */

                /* Bug 678 Start*/
                /* SPR_14564_fix: start */
                if(RRM_FALSE == rrm_is_utra_ncl_empty)
                {
                    /* Bug 678 Start*/
                    /* Not configuring meas object if no cell exist in ncl for that frequency */
                    freq_cells_exist = RRM_FALSE;
                    for( counter = RRM_ZERO; counter < utran_neighbour_count; counter++)
                    {
                        /*SPR_17893_START*/
                        /*SPR 21761 Start*/
                        if((carrier_freq == p_ncl_params->inter_rat_ncl.utran_freq_cells[counter].uarfcndl)) 
                        {
                        /*SPR 21761 End*/
                            /*SPR_17893_END*/
                            freq_cells_exist = RRM_TRUE;
                            break;
                        }
                    }
                    if(RRM_FALSE == freq_cells_exist)
                    {
                        /*SPR_17893_START*/
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Not creating meas object for UTRA frequency[%d] since no cells exists in \
                                ncl for this frequency or ps ho is not supported", carrier_freq);
                        /*SPR_17893_END*/
                        continue;
                    }
                }
                /* Bug 678 End*/
                /* SPR_14564_fix: end */
                /* Now check the freq is configured just before */
                for( counter = RRM_ZERO ; counter < (*object_count); counter++)
                {
                    p_meas_object_to_add_mod = &(p_meas_config->\
                            meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);               

                    if( p_meas_object_to_add_mod->meas_object.bitmask & 
                            MEAS_OBJECT_TO_ADD_UTRA_PRESENT )
                    {
                        if ( p_meas_object_to_add_mod->meas_object.meas_object_utra.carrier_freq
                                == carrier_freq )
                        {
                            carrier_freq_configured = RRM_ONE;
                            break;
                        }
                    }
                }
                if( carrier_freq_configured != RRM_ONE)
                {
                    /* Checking whether the freq which is not configured is supported by the UE */
                    /* SPR 22731 Fix Start */
                    ret_car_freq_supported = rrm_ue_is_ue_supported_utra_fdd_frequency (
                            carrier_freq,
                            p_rrm_ue_context);
                    /* SPR 22731 Fix End */
                    if (!ret_car_freq_supported)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Inter RAT Carrier Freq = %d, is not supported by [UE:%d]",
                                carrier_freq,
                                p_rrm_ue_context->ue_index);
                        continue;
                    }
                    /*SPR 21365 fix start*/
                    /* SPR 22310 CID 71447 Fix Start */
                    if(meas_config_count >= (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                "Maximum allowed MeasConfiguration Reached ");
                        break;
                    }
                    /* SPR 22310 CID 71447 Fix Start */
                    /*SPR 21365 fix stop */



                    p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                            meas_object_to_add_mod[counter]); 
                    /* START: CSR 00057390*/
                    if(RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                                carrier_freq,
                                RRM_ZERO,
                                RRM_IRAT_PRIORITY_UTRAN,&p_meas_object_config))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "UTRAN meas_object is already configured for carrier_freq = %d for [UE:%d]",
                                carrier_freq,p_rrm_ue_context->ue_index);
                                /*SPR 21365 fix start*/
                            ret_val = fill_meas_inter_rat_freq_neigh_cell_list(
                                    &p_meas_object_to_add_mod->meas_object.meas_object_utra,p_rrm_ue_context,
                                    carrier_freq);
                            if(RRM_SUCCESS == ret_val)
                            {
                        p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_UTRA_PRESENT;
                                p_meas_object_to_add_mod->meas_object_id =p_meas_object_config->meas_object_id;
                        p_meas_object_to_add_mod->meas_object.meas_object_utra.carrier_freq = carrier_freq;
                            }
                            /*SPR 21365 fix stop*/

                    }
                    else
                    {
                        /* bug 820 start */
                        ret_val = fill_inter_rat_utran_fdd_freq_meas_object_info(p_meas_object_to_add_mod,
                                p_rrm_ue_context, p_idle_mode_mobility_params, freq_count);
                        /* bug 820 end */
                    }
                    /* END: CSR 00057390*/

                    /*bug 820 start*/
                    if(RRM_SUCCESS == ret_val )
                    {
                        /*SPR 21365 fix start*/
                        meas_config_count++;
                        /*SPR 21365 fix stop*/

                        (*object_count)++;
                        /* SPR 14326 Fix Start */
                /* SPR_17797_FIX_START */
                        if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                        {
                            *send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_ctx->ran_info.rf_params
                                    .rf_configurations.dl_earfcn ,
                                    carrier_freq,
                                    p_rrm_ue_context,
                                    INTER_RAT_BAND_LIST,
                                    UTRAN_FDD_FREQ,
                                    RRM_PNULL
                                    );
                        }
                        /* SPR 14326 Fix End */

                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "No meas object configured for carrier_freq= %d",carrier_freq);
                    }
                    /*bug 820 end*/

                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Inter RAT - Carrier freq is already configured:%d",carrier_freq);
                }
            }

            if ((*object_count) > p_meas_config->meas_object_to_add_mod_list.count)
            {
                /* SPR 15260 start */
                /*spr_22241_changes_start*/
                p_meas_config->bitmask  |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
                /*spr_22241_changes_end*/
                /* SPR 15260 end */
                p_meas_config->meas_object_to_add_mod_list.count = (*object_count);
                /* SPR 14326 Fix Start */
                if( RRM_TRUE == *send_measgap )
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: Send MEAS GAP for UTRA FDD");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
                    fill_measurement_gap_info(p_meas_config, p_rrm_ue_context);
                }
                /* SPR_17797_FIX_START */
                else if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: NO need to send MEAS GAP for UTRA FDD");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
                }
                /* SPR 14326 Fix End */

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "Inter RAT UTRAN FDD measuremnt objects configured");
            }
        }
        /* Start fix for bug 715 */
        /* SPR_14564_fix: start */
        if(( p_idle_mode_mobility_params->bitmask & RRMCM_RMIF_IDLE_MODE_MOBILITY_IRAT_UTRA_PRESENT) && 
                ( p_idle_mode_mobility_params->idle_mode_mobility_irat_utra_params.bitmask & 
                  RRMCM_RMIF_IRAT_EUTRAN_UTRAN_TDD_LIST_PRESENT) &&
                (RRM_FALSE == rrm_is_utra_restricted))
            /* SPR_14564_fix: end */
        {
            p_irat_eutran_utran_tdd_list = &(p_idle_mode_mobility_params->\
                    idle_mode_mobility_irat_utra_params.irat_eutran_to_utran_tdd_list);

            valid_freqs = p_irat_eutran_utran_tdd_list->num_irat_eutran_to_utran_tdd_carriers;

            for( freq_count = RRM_ZERO; freq_count < valid_freqs ; freq_count++)
            {  
                carrier_freq_configured = RRM_ZERO;
                carrier_freq = p_irat_eutran_utran_tdd_list->\
                               irat_eutran_to_utran_tdd_carriers[freq_count].utra_carrier_arfcn;

                /* Start fix for SPR 962 */
                if ((p_rrm_proximity_ind != RRM_PNULL) &&
                        (p_rrm_proximity_ind->carrier_Freq.bitmask & RRM_UE_PROXIMITY_ARFCN_VALUE_UTRA_PRESENT) &&
                        (p_rrm_proximity_ind->carrier_Freq.carrier_freq_utra != carrier_freq))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "Proximity case: utran carrier freq [%d] does not match , check next", carrier_freq);
                    continue; 
                }
                /* End fix for SPR 962 */

                /* SPR_14564_fix: start */
                if(RRM_FALSE == rrm_is_utra_ncl_empty)
                {
                    /* Not configuring meas object if no cell exist in ncl for that frequency */
                    freq_cells_exist = RRM_FALSE;
                    for( counter = RRM_ZERO; counter < utran_neighbour_count; counter++)
                    {
                        if((carrier_freq == p_ncl_params->inter_rat_ncl.utran_freq_cells[counter].uarfcndl) &&
                                (p_ncl_params->inter_rat_ncl.utran_freq_cells[counter].pci <= RRM_MAX_PCI_UTRAN_TDD))
                        {
                            freq_cells_exist = RRM_TRUE;
                            break;
                        }
                    }
                    if(RRM_FALSE == freq_cells_exist)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Not creating meas object for UTRA frequency[%d] since no cells exists in \
                                ncl for this frequency", carrier_freq);
                        continue;
                    }
                }
                /* SPR_14564_fix: end */                        
                /* Now check the freq is configured just before */
                for( counter = RRM_ZERO ; counter < (*object_count); counter++)
                {
                    p_meas_object_to_add_mod = &(p_meas_config->\
                            meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);               

                    if( p_meas_object_to_add_mod->meas_object.bitmask & 
                            MEAS_OBJECT_TO_ADD_UTRA_PRESENT )
                    {
                        if ( p_meas_object_to_add_mod->meas_object.meas_object_utra.carrier_freq
                                == carrier_freq )
                        {
                            carrier_freq_configured = RRM_ONE;
                            break;
                        }
                    }
                }
                if( carrier_freq_configured != RRM_ONE)
                {
                    /* Checking whether the TDD freq which is not configured is supported by the UE */
                    /* SPR 22731 Fix Start */
                    ret_car_freq_supported = rrm_ue_is_ue_supported_utra_tdd_frequency (
                            carrier_freq,
                            p_rrm_ue_context);
                    /* SPR 22731 Fix End */
                    if (!ret_car_freq_supported)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Inter RAT TDD Carrier Freq = %d, is not supported by [UE:%d]",
                                carrier_freq,
                                p_rrm_ue_context->ue_index);
                        continue;
                    }

                    /*start supported_rat*/
                    if(RRM_FALSE == utran_supported)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "[UE:%d],utran rat is not supported by supported_rat param "
                                "and SPID",
                                p_rrm_ue_context->ue_index);
                        continue;
                    }
                    /*end supported_rat*/
                    /*SPR 21365 fix start*/
                    if(meas_config_count >= (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                "Maximum allowed MeasConfiguration Reached ");
                        break;
                    }
                    /*SPR 21365 fix stop*/        

                    p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                            meas_object_to_add_mod[counter]);
                    /* START: CSR 00057390*/
                    if(RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                                carrier_freq,
                                RRM_ZERO,
                                RRM_IRAT_PRIORITY_UTRAN,&p_meas_object_config))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "UTRAN meas_object is already configured for carrier_freq = %d for [UE:%d]",
                                carrier_freq,p_rrm_ue_context->ue_index);
                                /*SPR 21365 fix start*/
                                ret_val = fill_meas_inter_rat_freq_tdd_neigh_cell_list(
                                        &p_meas_object_to_add_mod->meas_object.meas_object_utra,p_rrm_ue_context,
                                        carrier_freq);
                                if(RRM_SUCCESS == ret_val)
                                {
                        p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_UTRA_PRESENT;
                        p_meas_object_to_add_mod->meas_object_id = p_meas_object_config->meas_object_id;
                        p_meas_object_to_add_mod->meas_object.meas_object_utra.carrier_freq = carrier_freq;
                                }
                                /*SPR 21365 fix stop*/

                    }
                    else
                    {
                        /*bug 820 start*/
                        ret_val = fill_inter_rat_utran_tdd_freq_meas_object_info(p_meas_object_to_add_mod,
                                p_rrm_ue_context, p_idle_mode_mobility_params, freq_count);
                        /*bug 820 end*/
                    }
                    /* END: CSR 00057390*/
                    /*bug 820 start*/
                    if(RRM_SUCCESS == ret_val )
                    {
                        /*SPR 21365 fix start*/
                        meas_config_count++;
                        /*SPR 21365 fix stop*/

                        (*object_count)++;
                        /* SPR 14326 Fix Start */
                /* SPR_17797_FIX_START */
                        if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                        {
                            *send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_ctx->ran_info.rf_params
                                    .rf_configurations.dl_earfcn ,
                                    carrier_freq,
                                    p_rrm_ue_context,
                                    INTER_RAT_BAND_LIST,
                                    UTRAN_TDD_FREQ,
                                    RRM_PNULL
                                    );
                        }
                        /* SPR 14326 Fix End */
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "No meas object configured for carrier_freq= %d",carrier_freq);
                    }
                    /*bug 820 end*/

                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Inter RAT - Carrier freq is already configured:%d",carrier_freq);
                }
            }

            if ((*object_count) > p_meas_config->meas_object_to_add_mod_list.count)
            {
                /* SPR_14564_fix: start */
                p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
                /* SPR_14564_fix: end */
                p_meas_config->meas_object_to_add_mod_list.count = (*object_count);

                /* SPR 14326 Fix Start */
                if( RRM_TRUE == *send_measgap )
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: Send MEAS GAP for UTRA TDD");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
                    fill_measurement_gap_info(p_meas_config, p_rrm_ue_context);
                }
                /* SPR_17797_FIX_START */
                else if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO:NO need to send MEAS GAP for UTRA TDD");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
                }
                /* SPR 14326 Fix End */
                /* SPR_14564_fix: start */
                /* Lines deleted */
                /* SPR_14564_fix: end */

            }
        }
        /* End fix for bug 715 */
        /* Klocwork changes start 22 may */
        /* Klocwork changes end 22 may */
    }
    else 
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "UTRAN Measurement Object is not configured " 
                "As the neighbour list configured have 0 UTRAN neighbours");
    }	      

    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_freq_geran_meas_config_object
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the geran  configuration in meas config list with values
 *                 received from OAM to be sent to RRC.
 *
 *  RETURNS      :      None
 ******************************************************************************/
static rrm_void_t
fill_inter_rat_freq_geran_meas_config_object(rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                          *object_count,
        /*SPR 21365 fix start*/
        /* Lines Deleted */
        /*SPR 21365 fix stop*/
        /*SPR 14326 Start*/
        rrm_bool_et *send_measgap,
        rrm_cell_context_t    *p_cell_ctx
        /*SPR 14326 End*/
        )
{
    U8     geran_neighbour_count                               = RRM_NULL;
    U8     valid_freqs                                         = RRM_ZERO;
    U8     carrier_freq_configured                             = RRM_ZERO;
    U16    carrier_freq                                        = RRM_ZERO;
    irat_eutran_geran_list_t      *p_irat_eutran_geran_list    = RRM_PNULL;
    rrm_bool_et                   freq_cells_exist             = RRM_FALSE;
    ncl_params_t    *p_ncl_params                              = RRM_PNULL ;
    U8     counter                                             = RRM_ZERO;
    U8     freq_count                                          = RRM_ZERO;
    meas_object_to_add_mod_t      *p_meas_object_to_add_mod   = RRM_PNULL;
    meas_report_info_node_t  *p_meas_object_config;   
    rrm_bool_et                   ret_geran_car_freq_supported = RRM_FALSE;
    /* END: CSR 00057390*/
    U8                     band_ind = RRM_ZERO;
    /* END: CSR 00057390*/
    /* SPR 14241 FIX START */
    rrm_son_geran_carrier_freqs_t *p_geran_carr_freqs          = RRM_PNULL;    
    meas_object_geran_t          *p_meas_object_geran          = RRM_PNULL;
    U8                           count                         = RRM_ZERO;
    /* SPR 14241 FIX END */
    /*SPR 14564 Start*/
    rrm_bool_et rrm_is_geran_restricted = RRM_FALSE;
    rrm_bool_et rrm_is_geran_ncl_empty = RRM_FALSE;
    /*SPR 14564 End*/
    /*SPR 21365 fix start*/
    U8    meas_config_count                     = RRM_ZERO;
    rrm_return_et ret_val;
    /*SPR 21365 fix stop*/

    RRM_UT_TRACE_ENTER();

    p_ncl_params                = rrm_cellm_get_ncl_info(p_rrm_ue_context->cell_index);     
    geran_neighbour_count       = p_ncl_params->inter_rat_ncl.num_valid_geran_cell;
    /*SPR 21365 fix start*/
     meas_config_count = rrm_count_measurements_for_ho_configured(p_rrm_ue_context) + 
                            p_meas_config->meas_object_to_add_mod_list.count;
    /*SPR 21365 fix stop*/

    if(rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_NINE) && 
            rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_THREE))
        /*End:Bug 762*/
    { 
		/*SPR 21687 Start*/
        if(is_ue_restricted_for_given_rat(p_rrm_ue_context, RRM_GERAN_CELL))
		{
			rrm_is_geran_restricted = RRM_TRUE;
		}
		/*SPR 21687 End*/
        /*bug 820 end*/	          
        if (geran_neighbour_count >=RRM_ONE )
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "GERAN neighbor is present in geran NCL [UE:%d]", p_rrm_ue_context->ue_index);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "GERAN neighbor is not configured " 
                    "will configure Idle Mode GERAN frequencies.");
            rrm_is_geran_ncl_empty = RRM_TRUE;
            /* SPR 14239 Fix Start */
            if(p_rrm_ue_context->ue_context_mod_req.bitmask & RRM_UEM_CTX_CSF_INDICATOR_PRESENT)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "GERAN NCL is not present in the CSFB scenario for [UE:%d]", p_rrm_ue_context->ue_index);
                rrm_is_geran_restricted = RRM_TRUE;
            }
            /* SPR 14239 Fix End */ 
        }
        if(( p_idle_mode_mobility_params->bitmask & RRMCM_RMIF_IDLE_MODE_MOBILITY_IRAT_GERAN_PRESENT) && 
                ( p_idle_mode_mobility_params->idle_mode_mobility_irat_geran_params.bitmask & 
                  RRMCM_RMIF_IRAT_EUTRAN_GERAN_FREQ_LIST_PRESENT ) &&
                (RRM_FALSE == rrm_is_geran_restricted))
            /* SPR_14564_fix: end */
        {
            p_irat_eutran_geran_list = &(p_idle_mode_mobility_params->\
                    idle_mode_mobility_irat_geran_params.irat_eutran_to_geran_list);

            valid_freqs = p_irat_eutran_geran_list->num_irat_eutran_to_geran_carriers;

            for( freq_count = RRM_ZERO; freq_count < valid_freqs ; freq_count++)
            {  
                carrier_freq_configured = RRM_ZERO;
                /* START: CSR 00057390*/
                carrier_freq = p_irat_eutran_geran_list->irat_eutran_to_geran_carriers[freq_count].geran_car_freqs.starting_arfcn;
                band_ind =  p_irat_eutran_geran_list->irat_eutran_to_geran_carriers[freq_count].geran_car_freqs.band_ind; 
                /* END: CSR 00057390*/

                /* Start :BUG 991 */
                /* SPR_14564_fix: start */
                if(RRM_FALSE == rrm_is_geran_ncl_empty )
                {
                    /* Not configuring meas object if no cell exist in ncl for that frequency */
                    freq_cells_exist = RRM_FALSE;
                    for( counter = RRM_ZERO; counter < geran_neighbour_count; counter++)
                    {
                        /* SPR-16447 START */
                        if(band_ind == p_ncl_params->inter_rat_ncl.geran_freq_cells[counter].band_indicator)
                        {
                            /* Spr 18401 Changes Start */
                            freq_cells_exist = check_geran_carrier_freq(p_irat_eutran_geran_list->
                                    irat_eutran_to_geran_carriers[freq_count].geran_car_freqs.following_arfcns,
                                    p_irat_eutran_geran_list->irat_eutran_to_geran_carriers[freq_count].
                                    geran_car_freqs.starting_arfcn,    
                                    p_ncl_params->inter_rat_ncl.geran_freq_cells[counter].bcch_arfcn);
                            /* Spr 18401 Changes End */

                            if(freq_cells_exist == RRM_TRUE)
                            {
                                break;
                            }
                        }
                    }
                    if(RRM_FALSE == freq_cells_exist)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Not creating meas object for GERAN frequency with starting arfcn[%d] since no cells exists in \
                                ncl for this frequency and following arfcns", carrier_freq);
                        continue;
                    }
                    /* SPR-16447 END */
                }
                /* SPR_14564_fix: end */

                /* End :BUG 991 */

                /* Now check the freq is configured just before */
                for( counter = RRM_ZERO ; counter < (*object_count); counter++)
                {
                    p_meas_object_to_add_mod = &(p_meas_config->\
                            meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);               

                    if( p_meas_object_to_add_mod->meas_object.bitmask & 
                            MEAS_OBJECT_TO_ADD_GERAN_PRESENT )
                    {
                        if ( p_meas_object_to_add_mod->meas_object.
                                meas_object_geran.carrier_freqs.starting_arfcn ==
                                p_irat_eutran_geran_list->irat_eutran_to_geran_carriers\
                                [freq_count].geran_car_freqs.starting_arfcn && 
                                p_meas_object_to_add_mod->meas_object.
                                meas_object_geran.carrier_freqs.band_indicator ==
                                p_irat_eutran_geran_list->irat_eutran_to_geran_carriers\
                                [freq_count].geran_car_freqs.band_ind)
                        {
                            carrier_freq_configured = RRM_ONE;
                            break;
                        }
                    }
                }
                if( carrier_freq_configured != RRM_ONE)
                {
                    /* Checking whether the freq which is not configured is supported by the UE */
                    ret_geran_car_freq_supported = rrm_ue_is_ue_supported_geran_frequency (
                            carrier_freq,
                            p_rrm_ue_context);
                    if (!ret_geran_car_freq_supported)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "Inter RAT GERAN Carrier Freqs , is not supported by [UE:%d]",
                                p_rrm_ue_context->ue_index);
                        continue;
                    }
                    /*SPR 21365 fix start*/
                    if(meas_config_count >= (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                "Maximum allowed MeasConfiguration Reached ");
                        break;
                    }
                    /*SPR 21365 fix stop*/       

                    /* START: CSR 00057390*/
                    p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                            meas_object_to_add_mod[counter]); 
                    if( RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                                carrier_freq,
                                band_ind,
                                RRM_IRAT_PRIORITY_GERAN,&p_meas_object_config))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                " GERAN meas_object is already configured for carrier_freq =%d  band_ind =%d for  [UE:%d]",
                                carrier_freq,band_ind,p_rrm_ue_context->ue_index);
                        /*SPR 14241 FIX START */
                        p_geran_carr_freqs = &(p_irat_eutran_geran_list->
                                irat_eutran_to_geran_carriers[freq_count].geran_car_freqs);
                        p_meas_object_geran = &(p_meas_object_to_add_mod->meas_object.meas_object_geran);
                        if(p_geran_carr_freqs->following_arfcns.bitmask &
                                RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT) 
                        {
                            p_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask |=\
                                                                                                   GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG;
                            p_meas_object_geran->carrier_freqs.following_arfcns.\
                                explicit_list_of_arfcns.count = p_geran_carr_freqs->following_arfcns.exp_arfcn_list_size;
                            for(count=RRM_ZERO;count<p_geran_carr_freqs->following_arfcns.exp_arfcn_list_size;count++)
                            {                  
                                p_meas_object_geran->carrier_freqs.following_arfcns.\
                                    explicit_list_of_arfcns.data[count] =\
                                    p_geran_carr_freqs->following_arfcns.exp_arfcn_list[count];
                            }               
                        }
                        else if(p_geran_carr_freqs->following_arfcns.bitmask &
                                RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT) 
                        {
                            p_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask |=\
                                                                                                   GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG;
                            p_meas_object_geran->carrier_freqs.following_arfcns.\
                                equally_spaced_arfcns.arfcn_spacing = p_geran_carr_freqs->following_arfcns.arfcn_spacing;
                            p_meas_object_geran->carrier_freqs.following_arfcns.\
                                equally_spaced_arfcns.num_of_following_arfcns =\
                                p_geran_carr_freqs->following_arfcns.num_of_following_arfcn;
                        }
                        else if(p_geran_carr_freqs->following_arfcns.bitmask &
                                RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT) 
                        {
                            p_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask |=\
                                                                                                   GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG;
                            p_meas_object_geran->carrier_freqs.following_arfcns.\
                                var_bitmap_of_arfcns.count = 
                                p_geran_carr_freqs->following_arfcns.arfcn_bmp_list_size;
                            for(count=RRM_ZERO;count<p_geran_carr_freqs->following_arfcns.arfcn_bmp_list_size;count++)
                            {                  
                                p_meas_object_geran->carrier_freqs.following_arfcns.\
                                    var_bitmap_of_arfcns.data[count] =\
                                    p_geran_carr_freqs->following_arfcns.arfcn_bmp_list[count];
                            }               
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                    "Invalid configuration received[RRM_SON_MC_GERAN_EXP_ARFCN_SET flag not present].");
                            continue;
                        }
                        /*SPR 14241 FIX END */

                        p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_GERAN_PRESENT;
                        p_meas_object_to_add_mod->meas_object_id = p_meas_object_config->meas_object_id;
                        p_meas_object_to_add_mod->meas_object.meas_object_geran.carrier_freqs.starting_arfcn = carrier_freq;
                        p_meas_object_to_add_mod->meas_object.meas_object_geran.carrier_freqs.band_indicator = band_ind;
                        /* SPR_15852_fix: start */
                        (*object_count)++;
                        /* CID 115528 +- */
                        /* SPR_15852_fix: end*/
                    } 
                    else
                    {

                        /*SPR 21365 fix start*/
                        ret_val = fill_inter_rat_geran_freq_meas_object_info(p_meas_object_to_add_mod,
                                p_rrm_ue_context, p_idle_mode_mobility_params, freq_count);
                        if(RRM_SUCCESS == ret_val)
                        {
                            /*SPR 21365 fix stop*/

                        /* SPR_15852_fix: start */
                        if ((MEAS_OBJECT_TO_ADD_GERAN_PRESENT & p_meas_object_to_add_mod->meas_object.bitmask) &&
                                (p_meas_object_to_add_mod->meas_object_id <= MAX_MEAS_OBJECT_ID))
                        {
                                /*SPR 21365 fix start*/
                                /* Line Deleted */
                                meas_config_count++;
                                /*SPR 21365 fix start*/
                            (*object_count)++;
                        }
                        /* SPR_15852_fix: end*/
                    }
                    /* SPR_15852_fix: code removed */
                    /* END: CSR 00057390*/
                    /* SPR 14326 Fix Start */
                /* SPR_17797_FIX_START */
                    if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                    {
                        *send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_ctx->ran_info.rf_params
                                .rf_configurations.dl_earfcn ,
                                carrier_freq,
                                p_rrm_ue_context,
                                INTER_RAT_BAND_LIST,
                                GERAN_FREQ,
                                RRM_PNULL
                                );
                    }
                    /* SPR 14326 Fix End */
                    /* SPR_15852_fix: code removed */
                        /*SPR 21365 fix start*/
                    }
                        /*SPR 21365 fix stop*/

                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,\
                            "GERAN Carrier freq and Band indicator is already configured");
                }
            }

            if ((*object_count) > p_meas_config->meas_object_to_add_mod_list.count)
            {
                /* SPR_14564_fix: start */
                p_meas_config->bitmask  |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
                /* SPR_14564_fix: end */
                p_meas_config->meas_object_to_add_mod_list.count = (*object_count);
                /* SPR 14326 Fix Start */
                if( RRM_TRUE == *send_measgap )
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: Send MEAS GAP for GERAN");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
                    fill_measurement_gap_info(p_meas_config, p_rrm_ue_context);
                }
                /* SPR_17797_FIX_START */
                else if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: NO need to send MEAS GAP for GERAN");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
                }
                /* SPR 14326 Fix End */
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                        "Inter RAT GERAN meas Obj not configured");
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Inter RAT GERAN is not configured");
        }
    }
    /* SPR_14564_fix: start */
    /* Lines removed */
    /* SPR_14564_fix: end */
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_freq_cdma_meas_config_object
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the geran  configuration in meas config list with values
 *                 received from OAM to be sent to RRC.
 *
 *  RETURNS      :      None
 ******************************************************************************/
static rrm_void_t
fill_inter_rat_freq_cdma_meas_config_object(rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        idle_mode_mobility_params_t *p_idle_mode_mobility_params,
        U8                          *object_count,
        /*SPR 21365 fix start*/
        /* Lines Deleted */
        /*SPR 21365 fix stop*/
        /*SPR 14326 Start*/
        rrm_bool_et *send_measgap,
        rrm_cell_context_t    *p_cell_ctx,
        rrm_cdma_meas_gap_config_t *cdma_meas_gap_config
        /*SPR 14326 End*/
        )
{
    rrmcm_rmif_cdma2000_freq_cells_t   *p_cdma2000_freq_cells = RRM_PNULL;
    U8     cdma2000_neighbour_count                            = RRM_NULL;
    U8     cdma2000_type                                       = RRM_NULL;
    U8     cdma2000_band_class                                 = RRM_NULL;
    U8     freq_count                                          = RRM_ZERO;
    ncl_params_t    *p_ncl_params                              = RRM_PNULL ;
    rrm_bool_et                   ret_cdma2000_car_freq_supported = RRM_FALSE;
    meas_object_to_add_mod_t      *p_meas_object_to_add_mod    = RRM_PNULL;
    U8     carrier_freq_configured                             = RRM_ZERO;
    U16    carrier_freq                                        = RRM_ZERO;
    rrm_bool_et                   freq_cells_exist             = RRM_FALSE;
    meas_report_info_node_t  *p_meas_object_config = RRM_PNULL;
    U8     counter                                             = RRM_ZERO;
    /*SPR 21365 fix start*/
    U8    meas_config_count                     = RRM_ZERO;
    rrm_return_et        ret_val = RRM_FAILURE;
    /*SPR 21365 fix stop*/



    p_ncl_params                = rrm_cellm_get_ncl_info(p_rrm_ue_context->cell_index);

    cdma2000_neighbour_count    = p_ncl_params->inter_rat_ncl.num_valid_cdma2000_freq_cells;
    /*SPR 21365 fix start*/
     meas_config_count = rrm_count_measurements_for_ho_configured(p_rrm_ue_context) + 
                            p_meas_config->meas_object_to_add_mod_list.count;
    /*SPR 21365 fix stop*/

    if((rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_FOUR)) &&
            p_rrm_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD)
        /*End:Bug 762*/
    {           

        /*bug 820 start*/
        if( (p_rrm_ue_context->ho_restriction_list.bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) &&
                ( (p_rrm_ue_context->ho_restriction_list.forbidden_inter_rat == FORBIDDEN_INTER_RAT_ALL) ||
                  (p_rrm_ue_context->ho_restriction_list.forbidden_inter_rat == FORBIDDEN_INTER_RAT_CDMA2000_TYPE) ||
                  (p_rrm_ue_context->ho_restriction_list.forbidden_inter_rat == FORBIDDEN_INTER_RAT_CDMA2000_AND_UTRAN) 
                )
          )
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "CDMA is restricted for [UE:%d]", p_rrm_ue_context->ue_index);
        }
        /*bug 820 end*/
        else  if (cdma2000_neighbour_count >= RRM_ONE )
        {
            p_cdma2000_freq_cells = p_ncl_params->inter_rat_ncl.cdma2000_freq_cells;

            for( freq_count = RRM_ZERO; freq_count < cdma2000_neighbour_count; freq_count++)
            {   
                carrier_freq_configured = RRM_ZERO;
                cdma2000_type = p_cdma2000_freq_cells[freq_count].type;

                if(RRM_OAM_NCL_TYPE_1XRTT == cdma2000_type)
                {
                    if(RRM_ZERO == RRM_MEMCMP(p_cdma2000_freq_cells[freq_count].CID,
                                p_idle_mode_mobility_params->idle_mode_mobility_irat_cdma_params. \
                                cdma2000_cell_param.cell_id_1xrtt.cdma2000_1xrtt_cell_id,
                                RRMCM_RMIF_CDMA2000_1XRTT_CELL_ID_LENGTH))
                    {
                        continue;

                    }

                    carrier_freq = p_cdma2000_freq_cells[freq_count].arfcn;
                    cdma2000_band_class = p_cdma2000_freq_cells[freq_count].band_class;

                    /* Not configuring meas object if no cell exist in ncl for that frequency */
                    freq_cells_exist = RRM_FALSE;
                    /*Spr 10305 start */
                    if((p_cdma2000_freq_cells[freq_count].presence_bitmask & RRMCM_RMIF_CDMA_HO_STATUS_PRESENT) &&
                            (p_cdma2000_freq_cells[freq_count].ho_status == HO_PROHIBITED))
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Carrier Frequency [%d], Band Class [%u] of  CDMA is Prohibited",
                                carrier_freq, cdma2000_band_class);
                    }
                    else
                    {
                        freq_cells_exist = RRM_TRUE;
                    }
                    /*Spr 10305 end */

                    if(RRM_FALSE == freq_cells_exist)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                "Not creating meas object for CDMA frequency[%d] Band Class [%u] since no cells exists in ncl for this frequency",
                                carrier_freq, cdma2000_band_class);
                        continue;
                    }

                    /* Now check the freq is configured just before */
                    for( counter = RRM_ZERO ; counter < (*object_count); counter++)
                    {
                        p_meas_object_to_add_mod = &(p_meas_config->\
                                meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);               

                        if( p_meas_object_to_add_mod->meas_object.bitmask & 
                                MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT )
                        {
                            if ( (p_meas_object_to_add_mod->meas_object.
                                        meas_object_cdma2000.carrier_freq_cdma2000.arfcn ==
                                        carrier_freq) &&
                                    (p_meas_object_to_add_mod->meas_object.
                                     meas_object_cdma2000.carrier_freq_cdma2000.band_class ==
                                     cdma2000_band_class))
                            {
                                carrier_freq_configured = RRM_ONE;
                                break;
                            }
                        }
                    }
                    if( carrier_freq_configured != RRM_ONE)
                    {
                        /* Checking whether the freq which is not configured is supported by the UE */
                        /* need to know the CDMA2000 supported Band information*/
                        ret_cdma2000_car_freq_supported = rrm_ue_is_ue_supported_cdma2000_frequency (
                                cdma2000_type,
                                cdma2000_band_class,
                                p_rrm_ue_context);
                        if (!ret_cdma2000_car_freq_supported)
                        {
                            continue;
                        }
                        /*SPR 21365 fix start*/
                        if(meas_config_count >= (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                    "Maximum allowed MeasConfiguration Reached ");
                            break;
                        }
                        /*SPR 21365 fix stop*/      
                        /* START: CSR 00057390*/
                        p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                                meas_object_to_add_mod[counter]); 
                        if( RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                                    carrier_freq,
                                    cdma2000_band_class,
                                    RRM_IRAT_PRIORITY_CDMA2000,&p_meas_object_config))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    " CDMA2000 meas_object is already configured for carrier_freq =%d  band_ind =%d for  [UE:%d]",
                                    carrier_freq,cdma2000_band_class,p_rrm_ue_context->ue_index);
                            p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT;
                            p_meas_object_to_add_mod->meas_object_id = p_meas_object_config->meas_object_id;
                            p_meas_object_to_add_mod->meas_object.meas_object_cdma2000.carrier_freq_cdma2000.arfcn = carrier_freq;
                            p_meas_object_to_add_mod->meas_object.meas_object_cdma2000.carrier_freq_cdma2000.band_class = cdma2000_band_class;
                            /*SPR 21365 fix start*/
                            ret_val = RRM_SUCCESS;            
                            /*SPR 21365 fix stop*/
                        } 
                        else
                        {        /*SPR 21365 fix start*/
                               ret_val = fill_inter_rat_cdma2000_freq_meas_object_info(
                                /*SPR 21365 fix stop*/
                                    p_meas_object_to_add_mod,
                                    p_rrm_ue_context, 
                                    p_cdma2000_freq_cells, 
                                    p_idle_mode_mobility_params,
                                    freq_count);

                                /*SPR 21365 fix start*/
                            /* SPR_15852_fix: start */
                            if (ret_val && (MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT & p_meas_object_to_add_mod->meas_object.bitmask)&&
                                    (p_meas_object_to_add_mod->meas_object_id <= MAX_MEAS_OBJECT_ID))
                            {
                                /*coverity 93937 fix start*/
                                (*object_count)++;
                                 meas_config_count++;
                                /*SPR 21365 fix stop*/
                                /*coverity 93937 fix end*/
                            }
                            /* SPR_15852_fix: end */
                        }
                        /*SPR 21365 fix start*/
                        if(RRM_SUCCESS == ret_val)
                        {
                            /*SPR 21365 fix stop*/
                        /* END: CSR 00057390*/
                        /* SPR_15852_fix: code removed */
                        /* Coverity Fix 88253 Start */
                        /* SPR 14326 Fix Start */
                /* SPR_17797_FIX_START */
                        if (!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                        {
                            cdma_meas_gap_config->band_class = cdma2000_band_class;
                            cdma_meas_gap_config->cdma_type  = RRM_ZERO;
                            *send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_ctx->ran_info.rf_params
                                    .rf_configurations.dl_earfcn ,
                                    carrier_freq,
                                    p_rrm_ue_context,
                                    INTER_RAT_BAND_LIST,
                                    CDMA_FREQ,
                                    cdma_meas_gap_config
                                    );
                        }
                        /* Coverity Fix 88253 End */
                        /* SPR 14326 Fix End */

                            /*SPR 21365 fix start*/
                        }  
                        /*SPR 21365 fix stop*/

                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,\
                                "CDMA2000 Carrier freq and Band indicator is already configured");
                    }
                }
            }
            if ((*object_count) > p_meas_config->meas_object_to_add_mod_list.count)
            {
                p_meas_config->bitmask  = UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
                p_meas_config->meas_object_to_add_mod_list.count = (*object_count);
                /* SPR 14326 Fix Start */
                if( RRM_TRUE == *send_measgap )
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: Send MEAS GAP for CDMA 1XRTT");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
                    fill_measurement_gap_info(p_meas_config, p_rrm_ue_context);
                }
                /* SPR_17797_FIX_START */
                else if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "HO: NO need to send MEAS GAP for CDMA 1XRTT");
                    p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
                }
                /* SPR 14326 Fix End */
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "Inter RAT CDMA2000 cell is not configured");
        }

    }
    /* For EUTRA RRC_CONNECTED to CDMA2000 handover */    

    /*Start:Bug 762*/ 
    if(rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_SIX) &&
            ((p_rrm_ue_context->cdma2000_capabilities.is_ecsfb_conc_ho_present) || 
             (!(p_rrm_ue_context->ho_params.bitmask & RRM_UE_CSFB_RECD)))) 
        /*End:Bug 762*/
        /* Not configuring HRPD report in the case of CSFB and only 1xRTT eCSFB */
    {           
        if (cdma2000_neighbour_count >= RRM_ONE )
        {
            /* start spr 9064 */
            if( (p_rrm_ue_context->ho_restriction_list.bitmask & HO_RESTRICTION_FORBIDDEN_INTER_RAT_PRESENT ) &&
                    ( (p_rrm_ue_context->ho_restriction_list.forbidden_inter_rat == FORBIDDEN_INTER_RAT_ALL) ||
                      (p_rrm_ue_context->ho_restriction_list.forbidden_inter_rat == FORBIDDEN_INTER_RAT_CDMA2000_TYPE) ||
                      (p_rrm_ue_context->ho_restriction_list.forbidden_inter_rat == FORBIDDEN_INTER_RAT_CDMA2000_AND_UTRAN) 
                    )
              )
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "CDMA is restricted for [UE:%d]", p_rrm_ue_context->ue_index);
            }
            else 
            {
                /* end spr 9064 */
                p_cdma2000_freq_cells = p_ncl_params->inter_rat_ncl.cdma2000_freq_cells;

                for( freq_count = RRM_ZERO; freq_count < cdma2000_neighbour_count; freq_count++)
                {   
                    carrier_freq_configured = RRM_ZERO;
                    cdma2000_type = p_cdma2000_freq_cells[freq_count].type;

                    if(RRM_OAM_NCL_TYPE_HRPD == cdma2000_type)
                    {
                        if(RRM_ZERO == RRM_MEMCMP(p_cdma2000_freq_cells[freq_count].CID,
                                    p_idle_mode_mobility_params->idle_mode_mobility_irat_cdma_params. \
                                    cdma2000_cell_param.cell_id_hrpd.cdma2000_hrpd_cell_id,
                                    RRM_CDMA2000_HRPD_CELL_ID_LENGTH))
                        {
                            continue;
                        }

                        carrier_freq = p_cdma2000_freq_cells[freq_count].arfcn;
                        cdma2000_band_class = p_cdma2000_freq_cells[freq_count].band_class;


                        /* Not configuring meas object if no cell exist in ncl for that frequency */
                        freq_cells_exist = RRM_FALSE;
                        /*Spr 10305 start */
                        if((p_cdma2000_freq_cells[freq_count].presence_bitmask & RRMCM_RMIF_CDMA_HO_STATUS_PRESENT) &&
                                (p_cdma2000_freq_cells[freq_count].ho_status == HO_PROHIBITED))
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "This carrier Frequency  CDMA is Prohibited" );
                        }
                        else
                        {
                            freq_cells_exist = RRM_TRUE;
                        }

                        if(RRM_FALSE == freq_cells_exist)
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                    "Not creating meas object for CDMA frequency[%d] since no cells exists in \
                                    ncl for this frequency", freq_count);
                            continue;
                        }

                        /*Spr 10305 end */

                        /* Now check the freq is configured just before */
                        for( counter = RRM_ZERO ; counter < (*object_count); counter++)
                        {
                            p_meas_object_to_add_mod = &(p_meas_config->\
                                    meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);               

                            if( p_meas_object_to_add_mod->meas_object.bitmask & 
                                    MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT )
                            {
                                if ( (p_meas_object_to_add_mod->meas_object.
                                            meas_object_cdma2000.carrier_freq_cdma2000.arfcn ==
                                            carrier_freq) &&
                                        (p_meas_object_to_add_mod->meas_object.
                                         meas_object_cdma2000.carrier_freq_cdma2000.band_class ==
                                         cdma2000_band_class))
                                {
                                    carrier_freq_configured = RRM_ONE;
                                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                            " CDMA2000 carrier_freq =%d  band_ind =%d is already configured for [UE:%d]",
                                            carrier_freq,cdma2000_band_class,p_rrm_ue_context->ue_index);
                                    break;
                                }
                            }
                        }

                        if( carrier_freq_configured != RRM_ONE)
                        {
                            /* Checking whether the freq which is not configured is supported by the UE */
                            /* need to know the CDMA2000 supported Band information*/
                            ret_cdma2000_car_freq_supported = rrm_ue_is_ue_supported_cdma2000_frequency (
                                    cdma2000_type,
                                    cdma2000_band_class,
                                    p_rrm_ue_context);
                            if (!ret_cdma2000_car_freq_supported)
                            {
                                continue;
                            }
			    /*SPR 21365 fix start*/
			    /* SPR 22310 CID 88253 Fix Start */
			    if(meas_config_count >= (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE))
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                            "Maximum allowed MeasConfiguration Reached ");
                                break;
                            }
			    /* SPR 22310 CID 88253 Fix End */
			    /*SPR 21365 fix stop*/             


                            /* START: CSR 00057390*/
                            p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                                    meas_object_to_add_mod[counter]); 
                            if( RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                                        carrier_freq,
                                        cdma2000_band_class,
                                        RRM_IRAT_PRIORITY_CDMA2000,&p_meas_object_config))
                            {
                                p_meas_object_to_add_mod->meas_object.bitmask = MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT;
                                p_meas_object_to_add_mod->meas_object_id = p_meas_object_config->meas_object_id;
                                p_meas_object_to_add_mod->meas_object.meas_object_cdma2000.carrier_freq_cdma2000.arfcn = carrier_freq;
                                p_meas_object_to_add_mod->meas_object.meas_object_cdma2000.carrier_freq_cdma2000.band_class = cdma2000_band_class;
                                /*SPR 21365 fix start*/
                                ret_val = RRM_SUCCESS;            
                                /*SPR 21365 fix stop*/
                            } 
                            else
                            {
                                /*SPR 21365 fix start*/
                                ret_val =  fill_inter_rat_cdma2000_freq_meas_object_info(
                                        /*SPR 21365 fix stop*/
                                        p_meas_object_to_add_mod,
                                        p_rrm_ue_context, 
                                        p_cdma2000_freq_cells, 
                                        p_idle_mode_mobility_params,
                                        freq_count);
                                /* SPR_15852_fix: start */
                                /*SPR 21365 fix start*/
                                if ( ret_val && (MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT & p_meas_object_to_add_mod->meas_object.bitmask)&&
                                        (p_meas_object_to_add_mod->meas_object_id <= MAX_MEAS_OBJECT_ID))
                                /*SPR 21365 fix stop*/
                                {
                                     meas_config_count++;
                                    (*object_count)++;
                                }
                                /* SPR_15852_fix: end */
                            }
                            /*SPR 21365 fix start*/
                            if(RRM_SUCCESS == ret_val)
                            {
                            /*SPR 21365 fix stop*/

                            /* Coverity Fix 88253 Start */
                            /* SPR 14326 Fix Start */
                /* SPR_17797_FIX_START */
                            if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                            {
                                cdma_meas_gap_config->band_class = cdma2000_band_class;
                                cdma_meas_gap_config->cdma_type  = RRM_ONE;
                                *send_measgap = rrm_check_send_meas_gap_is_enabled(p_cell_ctx->ran_info.rf_params
                                        .rf_configurations.dl_earfcn ,
                                        carrier_freq,
                                        p_rrm_ue_context,
                                        INTER_RAT_BAND_LIST,
                                        CDMA_FREQ,
                                        cdma_meas_gap_config
                                        );
                            }
                            /* SPR 14326 Fix End */
                            /* Coverity Fix 88253 End */
                            /* END: CSR 00057390*/
                            /* SPR_15852_fix: code removed */
                            //*SPR 21365 fix start*/
                            } 
                            /*SPR 21365 fix stop*/

                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,\
                                    "CDMA2000 Carrier freq and Band indicator is already configured");
                        }
                    }
                }

                if ((*object_count) > p_meas_config->meas_object_to_add_mod_list.count)
                {
                    p_meas_config->bitmask  = UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
                    p_meas_config->meas_object_to_add_mod_list.count = (*object_count);
                    /* SPR 14326 Fix Start */
                    if( RRM_TRUE == *send_measgap )
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "HO: Send MEAS GAP for CDMA HRPD");
                        p_rrm_ue_context->send_measgap_in_meas_config = RRM_TRUE;
                        fill_measurement_gap_info(p_meas_config, p_rrm_ue_context);
                    }
                /* SPR_17797_FIX_START */
                    else if(!(p_meas_config->bitmask & UE_ADM_MEAS_GAP_CONFIG_PRESENT))
                /* SPR_17797_FIX_END */
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "HO: NO need to send MEAS GAP for CDMA HRPD");
                        p_rrm_ue_context->send_measgap_in_meas_config = RRM_FALSE;
                    }
                    /* SPR 14326 Fix End */
                }
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "No CDMA2000 cell is configured");
        }
    }
    RRM_UT_TRACE_EXIT();
}

/* SPR-16447 START */
/******************************************************************************
 *  FUNCTION NAME: check_geran_carrier_freq 
 *  INPUT        : geran_car_freqs, bcch_arfcn
 *  OUTPUT       : arfcn comparision results, true/false
 *  DESCRIPTION  : Compares the geran_carrier_frequencies with genran frequencies 
 in neighbor list before configuring meas object. 
 *
 *  RETURNS      :      None
 ******************************************************************************/
/* Spr 18401 Changes Start */
rrm_bool_et
check_geran_carrier_freq
(
 rrm_son_geran_following_arfcns_t  following_arfcns,
 U16         starting_arfcn,
 U16         bcch_arfcn
 )
{
    rrm_bool_et freq_cells_exist = RRM_FALSE;
    U16 f_arfcn = RRM_ZERO;
    U8 index = RRM_ZERO,
       loop = RRM_ZERO,
       bitmap = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    if(starting_arfcn == bcch_arfcn)
    {
        freq_cells_exist = RRM_TRUE;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "arfcn = %d,present in geran freq info list",bcch_arfcn);
    }
    else if(following_arfcns.bitmask & RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT)
    {
        for(index=RRM_ZERO; index<(following_arfcns.exp_arfcn_list_size); index++)
        {
            if(following_arfcns.exp_arfcn_list[index] == bcch_arfcn)
            {
                freq_cells_exist = RRM_TRUE;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "arfcn = %d, found in explicitly listed ARFCN"
                        " present in geran freq info for starting arfcn = %d",
                        bcch_arfcn,starting_arfcn);

                break;
            }
        }
    }
    else if(following_arfcns.bitmask & RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT)
    {
        for(index=RRM_ONE; index<=(following_arfcns.num_of_following_arfcn); index++)
        {
            f_arfcn = ((starting_arfcn + (following_arfcns.arfcn_spacing * index)) % RRM_FARFCN1024);
            if(f_arfcn == bcch_arfcn)
            {
                freq_cells_exist = RRM_TRUE;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "arfcn = %d, found in equally spaced ARFCN list"
                        " present in geran freq info for starting arfcn = %d",
                        bcch_arfcn,starting_arfcn);
                break;
            }
        }
    }
    else if(following_arfcns.bitmask & RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT)
    {
        for(index=RRM_ZERO; index<(following_arfcns.arfcn_bmp_list_size); index++)
        {
            bitmap = following_arfcns.arfcn_bmp_list[index];
            for(loop = RRM_ZERO; loop < RRM_ARFCN_SPACING; loop++)
            {
                if(bitmap & (RRM_ONE << loop))
                {
                    f_arfcn = ((starting_arfcn + ((loop + RRM_ONE)*(index + RRM_ONE))) % RRM_FARFCN1024);
                    if(f_arfcn == bcch_arfcn)
                    {
                        freq_cells_exist = RRM_TRUE;
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                                "arfcn = %d, found in variable bitmap of ARFCN"
                                " present in geran freq info for starting arfcn = %d",
                                bcch_arfcn,starting_arfcn);
                        break;
                    }
                }
            }
        }
    }
    RRM_UT_TRACE_EXIT();
    return freq_cells_exist;
}
/* Spr 18401 Changes End */



/* SPR-16447 END */
/*Klockwork_fix_2431_end*/
/******************************************************************************
 *  FUNCTION NAME: fill_inter_rat_freq_meas_config_object
 *  INPUT        : p_rrm_ue_context,utran_supported,geran_supported,cdma2000_supported
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the utran configuration in meas config list with values 
 *                 received from OAM to be sent to RRC.
 *
 *  RETURNS      :      None
 ******************************************************************************/
static rrm_void_t
fill_inter_rat_freq_meas_config_object(
        rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        /*start supported_rat*/
        rrm_bool_et                 utran_supported,
        rrm_bool_et                 geran_supported,
        rrm_bool_et                 cdma2000_supported
        /*start supported_rat*/
        )
{
    RRM_UT_TRACE_ENTER();

    /*bug 820 start*/
    /*bug 820 end*/
    
                /*SPR 21365 fix start*/
                /* Line Deleted */
                /*SPR 21365 fix stop*/
    idle_mode_mobility_params_t   *p_idle_mode_mobility_params = RRM_PNULL;
    /* Start fix for bug 715 */
    /* End fix for bug 715 */
    U8                            object_count                 = RRM_ZERO;
    rrm_cell_context_t    *p_cell_ctx = RRM_PNULL;
    /* SPR 14326 Fix Start */
    rrm_bool_et    send_measgap = RRM_FALSE;
    rrm_cdma_meas_gap_config_t cdma_meas_gap_config = {RRM_ZERO};
    /* SPR 14326 Fix End */

    p_cell_ctx = rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    /*CID 65834:start*/
    if (RRM_PNULL != p_cell_ctx)
    {
                /*SPR 21365 fix start*/
                /* Line Deleted */
                /*SPR 21365 fix stop*/

        if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
                p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
                eutra_radio_capability_info_def.bitmask)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

        }


        p_idle_mode_mobility_params = rrm_cellm_get_idle_mode_params\
                                      (p_rrm_ue_context->cell_index);
        object_count = p_meas_config->meas_object_to_add_mod_list.count;

        /* For EUTRA RRC_CONNECTED to UTRA CELL_DCH  CS handover */ 
        if(RRM_TRUE == utran_supported)
        {
            /* Start fix for SPR 962 */
            /* 
             ** This is UTRA case so configure measurements on the carrier 
             ** freq received in proximity ind if it is UTRA cell freq.
             */
            /*Klockwork_fix_2431_start*/
            /*SPR 14326 START*/
                /*SPR 21365 fix start*/
            fill_inter_rat_freq_utra_meas_config_object(p_meas_config ,p_rrm_ue_context ,
                    p_idle_mode_mobility_params ,utran_supported,&object_count ,
                    &send_measgap,p_cell_ctx);
                /*SPR 21365 fix stop*/
            /*SPR 14326 END */
            /*Klockwork_fix_2431_end*/  

        }

        if ((RRM_TRUE == geran_supported) &&
                /* Start fix for SPR 962 */
                (p_rrm_ue_context->proximity_ind == RRM_FALSE))
            /* End fix for SPR 962 */
        {
            /* For EUTRA RRC_CONNECTED to GERAN GSM_Dedicated handover */    
            /*Start:Bug 762*/
            /*Klockwork_fix_2431_start*/
            /*SPR 14326 START*/
                /*SPR 21365 fix start*/
            fill_inter_rat_freq_geran_meas_config_object(p_meas_config ,p_rrm_ue_context ,
                    p_idle_mode_mobility_params ,&object_count,
                    &send_measgap,p_cell_ctx);
                /*SPR 21365 fix stop*/
            /*SPR 14326 END*/
            /*Klockwork_fix_2431_ekta_end*/
        }


        /* For EUTRA RRC_CONNECTED to CDMA2000 handover */    
        if ((RRM_TRUE == cdma2000_supported) &&
                /* Start fix for SPR 962 */
                (p_rrm_ue_context->proximity_ind == RRM_FALSE))
            /* End fix for SPR 962 */
        {
            /*Start:Bug 762*/
            /*Klockwork_fix_2431_start*/
            /*SPR 14326 START*/
                /*SPR 21365 fix start*/
            fill_inter_rat_freq_cdma_meas_config_object(p_meas_config ,p_rrm_ue_context ,
                    p_idle_mode_mobility_params ,&object_count,
                    &send_measgap,p_cell_ctx,&cdma_meas_gap_config);
                /*SPR 21365 fix stop*/

            /*SPR 14326 END*/
            /*Klockwork_fix_2431_end*/
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctx is null");
    }
    /*CID 65834:end*/
    RRM_UT_TRACE_EXIT();
}

/* SPR 15260 start */
/* Inter RAT BLR chnages End */
/******************************************************************************
 *  FUNCTION NAME: fill_measurement_gap_info
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the measurement gap information info for meas_config
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_measurement_gap_info(
        rrm_meas_config_t   *p_meas_config,
        rrm_ue_context_t    *p_rrm_ue_context)
{
    RRM_UT_TRACE_ENTER();
    rrm_meas_config_info_t *p_cell_meas_config_info =RRM_NULL;
    U8 gap_offset_type = RRM_ZERO;

    /* COMP_WARN_1_FEB:compilation warning removal */
    p_cell_meas_config_info = (rrm_meas_config_info_t*)rrm_cellm_get_meas_config_info(p_rrm_ue_context->cell_index);
    if(p_cell_meas_config_info->bitmask & RRMCM_RMIF_MEAS_GAP_CONFIG_PRESENT &&
            (p_rrm_ue_context->meas_gap_config.meas_gap_added & MEAS_GAP_ADDED_FOR_PRS) == RRM_FALSE) 
    {
        p_meas_config->meas_gap_config.bitmask = RRM_ZERO;
        p_rrm_ue_context->meas_gap_config.meas_gap_added |= MEAS_GAP_ADDED_FOR_NON_PRS;

        /*SPR 6491 Fix Start */
        /* 0 - for release, 1 - for setup */
        p_meas_config->meas_gap_config.meas_gap_config_type = RRM_ONE;
        /*SPR 6491 Fix End */

        /*SPR_17047_START*/
        /*Code Removed*/
        p_rrm_ue_context->meas_gap_sent_status = RRM_SET;
        /*SPR_17047_END*/

        /* Set Meas Gap status in UE Context */
        //p_rrm_ue_context->meas_gap_config.meas_gap_config_type = RRM_ONE;
        p_meas_config->bitmask |= UE_ADM_MEAS_GAP_CONFIG_PRESENT;

        p_meas_config->meas_gap_config.bitmask |= 
            MEAS_GAP_CONFIG_SETUP_CONFIG_INFO_PRESENT;
        p_meas_config->meas_gap_config.setup_config_info.bitmask = RRM_ZERO;

        /* SPR 5883 starts */
        gap_offset_type = p_cell_meas_config_info->meas_gap_config.eutran_gap_offset_type;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "meas gap type  [%d]\n ", gap_offset_type );

        if (RRM_MEAS_GAP_TYPE_0 == gap_offset_type)
        {
            /* SPR 15713 fix start */
            p_meas_config->meas_gap_config.setup_config_info.bitmask = 
                MEAS_GAP_CONFIG_GP0_PRESENT;
            /* SPR 15713 fix end */
            /* for limiting UE meas */
            p_rrm_ue_context->meas_gap_type = RRM_MEAS_GAP_TYPE_0;
            /*SPR 15713 Fix Start*/
            /*Bug 497 Fix Start */
            /*p_rrm_ue_context->meas_gap_offset= rrm_ue_assign_gap_offset(
              p_rrm_ue_context, RRM_MEAS_GAP_TYPE_0);*/
            /*Bug 497 Fix End */
            /*SPR 15713 Fix End*/
            p_rrm_ue_context->meas_gap_config.meas_gap_type = RRM_MEAS_GAP_TYPE_0;
            p_rrm_ue_context->meas_gap_config.meas_gap_offset = p_rrm_ue_context->meas_gap_offset;

            p_meas_config->meas_gap_config.setup_config_info.gp0 = 
                p_rrm_ue_context->meas_gap_offset;
            /*SPR 15713 Fix Start*/
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "meas_gap:%d",p_rrm_ue_context->meas_gap_offset);
            /*SPR 15713 Fix End*/

        }
        else if (RRM_MEAS_GAP_TYPE_1 == gap_offset_type)
        {
            /* SPR 15713 fix start */
            p_meas_config->meas_gap_config.setup_config_info.bitmask = 
                MEAS_GAP_CONFIG_GP1_PRESENT;
            /* SPR 15713 fix end */
            /* for limiting UE meas */
            p_rrm_ue_context->meas_gap_type = RRM_MEAS_GAP_TYPE_1;
            /*Bug 497 Fix Start */
            /*SPR 15713 Fix Start*/
            /*Bug 497 Fix Start */
            /*p_rrm_ue_context->meas_gap_offset = rrm_ue_assign_gap_offset(
              p_rrm_ue_context, RRM_MEAS_GAP_TYPE_1);*/
            /*Bug 497 Fix End */
            /*SPR 15713 Fix End*/
            p_rrm_ue_context->meas_gap_config.meas_gap_type = RRM_MEAS_GAP_TYPE_1;
            p_rrm_ue_context->meas_gap_config.meas_gap_offset = p_rrm_ue_context->meas_gap_offset;
            p_meas_config->meas_gap_config.setup_config_info.gp1 =
                p_rrm_ue_context->meas_gap_offset;
            /*SPR 15713 Fix Start*/
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "meas_gap:%d",p_rrm_ue_context->meas_gap_offset);
            /*SPR 15713 Fix End*/


        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Invalid measurement gaps type[%d] configuration", gap_offset_type);
        }
        /* SPR 5883 ends */
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Not Add meas gap p_cell_meas_config_info->bitmask =%d, p_rrm_ue_context->meas_gap_config.meas_gap_added=%d",
                p_cell_meas_config_info->bitmask,p_rrm_ue_context->meas_gap_config.meas_gap_added);
    }
    RRM_UT_TRACE_EXIT();
}
/* SPR 15260 end */




/******************************************************************************
 *  FUNCTION NAME: fill_pre_registration_info_hrpd
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the fill pre registration info hrpd configuration info
 *                 for meas_config
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_pre_registration_info_hrpd(
        rrm_meas_config_t   *p_meas_config,
        rrm_ue_context_t    *p_rrm_ue_context)
{
    idle_mode_mobility_params_t   *p_idle_mode_mobility_params = RRM_PNULL;
    rrmcm_rmif_pre_reg_info_hrpd_t  *p_pre_reg_info_hrpd = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_idle_mode_mobility_params = \
                                  rrm_cellm_get_idle_mode_params(p_rrm_ue_context->cell_index);

    p_pre_reg_info_hrpd = &p_idle_mode_mobility_params-> \
                          idle_mode_mobility_irat_cdma_params. \
                          pre_reg_info_hrpd;

    if(p_idle_mode_mobility_params->idle_mode_mobility_irat_cdma_params. \
            presence_bitmask & RRMCM_RMIF_PRE_REG_INFO_HRPD_PRESENCE_FLAG)
    {
        p_meas_config->pre_registration_info_hrpd. \
            pre_registration_allowed = p_pre_reg_info_hrpd->pre_reg_allowed;

        if(p_pre_reg_info_hrpd->presence_bitmask &
                RRMCM_RMIF_PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG)
        {
            p_meas_config->pre_registration_info_hrpd. \
                pre_registration_zone_id = p_pre_reg_info_hrpd->pre_reg_zone_id;
            p_meas_config->pre_registration_info_hrpd. \
                bitmask |= PRE_REGISTRATION_ZONE_ID_PRESENT;
        }

        if(p_pre_reg_info_hrpd->presence_bitmask &
                RRMCM_RMIF_PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG)
        {
            p_meas_config->pre_registration_info_hrpd.\
                secondary_pre_registration_zone_id_list.count = \
                p_pre_reg_info_hrpd->secondary_list.count;

            RRM_MEMCPY(p_meas_config->pre_registration_info_hrpd. \
                    secondary_pre_registration_zone_id_list. \
                    pre_registration_zone_id_hrpd, 
                    p_pre_reg_info_hrpd->secondary_list. \
                    pre_reg_zone_id,
                    p_pre_reg_info_hrpd->secondary_list.count);

            p_meas_config->pre_registration_info_hrpd. \
                bitmask |= PRE_REGISTRATION_SECONDARY_ZONE_ID_LIST_PRESENT;
        }
        p_meas_config->bitmask |= UE_ADM_MEAS_PRE_REGISTRATION_INFO_PRESENT;
    }
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: fill_quantity_config_info
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran  quantity configuration info for meas_config
 *  RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_quantity_config_info(
        rrm_meas_config_t   *p_meas_config,
        rrm_ue_context_t    *p_rrm_ue_context
        )
{
    RRM_UT_TRACE_ENTER();

    rrm_common_params_for_eutra_t
        *p_common_params_for_eutra  = RRM_PNULL;

    rrm_irat_params_t        *p_rrm_irat_params          = RRM_PNULL;

    U8                        counter                    = RRM_ZERO;
    U8    quantity_config_for_utra_flag                       = RRM_ZERO; 
    U8    quantity_config_for_eutra_flag                       = RRM_ZERO; 
    U8    quantity_config_for_geran_flag                       = RRM_ZERO; 
    U8    quantity_config_for_cdma2000_flag                    = RRM_ZERO; 

    /* BLR Inter RAT changes start */
    /* Filling the qauntity config only if UTRAN meas object are created */
    for( counter = RRM_ZERO; counter < p_meas_config->meas_object_to_add_mod_list.count ; counter++)
    {
        if( p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT &&
                quantity_config_for_eutra_flag != RRM_ONE)
        { 
            p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                        (p_rrm_ue_context->cell_index);
            p_meas_config->quantity_config.bitmask |= QUANTITY_CONFIG_EUTRA_PRESENT;

            p_meas_config->quantity_config.quantity_config_eutra.filter_coefficient_rsrp =
                p_common_params_for_eutra->filter_coefficient_rsrp;
            /* Need to be check for limiting UE meas */
            p_meas_config->quantity_config.quantity_config_eutra.filter_coefficient_rsrq =
                p_common_params_for_eutra->filter_coefficient_rsrq;
            quantity_config_for_eutra_flag = RRM_ONE;
        }		

        else if( p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_UTRA_PRESENT &&
                quantity_config_for_utra_flag != RRM_ONE)
        {

            p_rrm_irat_params = rrm_cellm_get_connected_mode_irat_params\
                                (p_rrm_ue_context->cell_index);
            p_meas_config->quantity_config.bitmask |= QUANTITY_CONFIG_UTRA_PRESENT;
            p_meas_config->quantity_config.quantity_config_utra.
                meas_quantity_utra_fdd = p_rrm_irat_params->meas_quantity_utra_fdd;
            /* Start fix for bug 715 */
            /* Since it is a manditory parameter, and only supported value for TDD is RSCP 
             * so initializing with default value */
            /* SPR 16880: start */
            p_meas_config->quantity_config.quantity_config_utra.
                meas_quantity_utra_tdd = p_rrm_irat_params->meas_quantity_utra_tdd;
            /* SPR 16880: end */
            /* End fix for bug 715 */
            p_meas_config->quantity_config.quantity_config_utra.
                filter_coefficient = p_rrm_irat_params->filter_coefficient_utra;
            quantity_config_for_utra_flag = RRM_ONE;
        }

        else if( p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_GERAN_PRESENT && 
                quantity_config_for_geran_flag != RRM_ONE)
        {

            p_rrm_irat_params = rrm_cellm_get_connected_mode_irat_params\
                                (p_rrm_ue_context->cell_index);
            p_meas_config->quantity_config.bitmask |= QUANTITY_CONFIG_GERAN_PRESENT;
            p_meas_config->quantity_config.quantity_config_geran.
                meas_quantity_geran = RRM_RRC_RSSI;
            p_meas_config->quantity_config.quantity_config_geran.
                filter_coefficient = p_rrm_irat_params->filter_coefficient_geran;

            quantity_config_for_geran_flag = RRM_ONE;
        }
        else if(p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT &&
                quantity_config_for_cdma2000_flag != RRM_ONE)
        {
            /*  
             ** Coverity, Assigning p_common_params_for_eutra insted of assigning p_rrm_irat_params as it
             ** is not used in this condition, Aditya Rel1.3.
             */
            p_meas_config->quantity_config.bitmask |= QUANTITY_CONFIG_CDMA2000_PRESENT;
            /* SPR 16880: start */
            /* Code removed */
            /* SPR 16880: end */
            /* SPR 16880: start */
            p_rrm_irat_params = rrm_cellm_get_connected_mode_irat_params(p_rrm_ue_context->cell_index);
            p_meas_config->quantity_config.quantity_config_cdma2000.
                meas_quantity_cdma2000 = p_rrm_irat_params->meas_quantity_cdma2000;
            /* SPR 16880: end */

            quantity_config_for_cdma2000_flag = RRM_ONE;
        }

        else if( quantity_config_for_utra_flag == RRM_ONE && quantity_config_for_geran_flag == RRM_ONE &&
                quantity_config_for_eutra_flag == RRM_ONE && quantity_config_for_cdma2000_flag == RRM_ONE)
        {
            break;             
        }            
    }
    if( quantity_config_for_utra_flag == RRM_ONE || quantity_config_for_geran_flag == RRM_ONE ||
            quantity_config_for_eutra_flag == RRM_ONE || quantity_config_for_cdma2000_flag == RRM_ONE)
    {
        p_meas_config->bitmask |= UE_ADM_MEAS_QUANTITY_CONFIG_PRESENT;
    }

    /* BLR Inter RAT changes end */
    RRM_UT_TRACE_EXIT();

}
/*Klockwork_fix_start*/
/******************************************************************************
 *  FUNCTION NAME: store_meas_report_info_eutra
 *  INPUT        : p_rrm_ue_context,meas_id
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t
store_meas_report_info_eutra(/*SPR 17777 +-*/
        meas_report_info_node_t                    * p_meas_report_info ,
        report_config_to_add_mod_report_config_t   *p_report_config ,
        U8                                         *is_report_object_found )

{ 
    RRM_UT_TRACE_ENTER();
    report_config_eutra_trigger_type_event_eventid_t  *p_event_id = RRM_PNULL;
    U16                 event_id_bitmask   = RRM_ZERO;
    p_meas_report_info->trigger_quantity = (rrmc_trigger_quantity_et) p_report_config->report_config_eutra.trigger_quantity;   

    p_event_id = &(p_report_config->report_config_eutra.trigger_type.event.event_id); 

    event_id_bitmask = p_event_id->bitmask;

    if(event_id_bitmask & REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A1_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_A1;
        if(p_event_id->event_a1.a1_threshold.bitmask 
                & THRESHOLD_EUTRA_RSRP_PRESENT)
        {
            p_meas_report_info->thresh1 = p_event_id->event_a1.a1_threshold.threshold_rsrp;
        }
        else
        {
            p_meas_report_info->thresh1 = p_event_id->event_a1.a1_threshold.threshold_rsrq;
        } 
        *is_report_object_found = RRM_TRUE;
    }
    else if(event_id_bitmask & REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A2_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_A2;

        if(p_event_id->event_a2.a2_threshold.bitmask 
                & THRESHOLD_EUTRA_RSRP_PRESENT)
        {
            p_meas_report_info->thresh1 = p_event_id->event_a2.a2_threshold.threshold_rsrp;
        }
        else
        { 
            p_meas_report_info->thresh1 = p_event_id->event_a2.a2_threshold.threshold_rsrq;
        }
        *is_report_object_found = RRM_TRUE;
    }
    else if(event_id_bitmask & REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A3_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_A3;
        p_meas_report_info->offset          = p_event_id->event_a3.a3_offset;
        *is_report_object_found = RRM_TRUE;
    }
    else if(event_id_bitmask & REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A4_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_A4;
        if(p_event_id->event_a4.a4_threshold.bitmask 
                & THRESHOLD_EUTRA_RSRP_PRESENT)
        {
            p_meas_report_info->thresh1 = p_event_id->event_a4.a4_threshold.threshold_rsrp;
        }
        else
        {
            p_meas_report_info->thresh1 = p_event_id->event_a4.a4_threshold.threshold_rsrq;
        } 
        *is_report_object_found = RRM_TRUE;
    }
    else if(event_id_bitmask & REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_A5;
        if(p_event_id->event_a5.a5_threshold1.bitmask 
                & THRESHOLD_EUTRA_RSRP_PRESENT)
        {
            p_meas_report_info->thresh1 = p_event_id->event_a5.a5_threshold1.threshold_rsrp;
            p_meas_report_info->thresh2 = p_event_id->event_a5.a5_threshold2.threshold_rsrp;
        }
        else
        {
            p_meas_report_info->thresh1 = p_event_id->event_a5.a5_threshold1.threshold_rsrq;
            p_meas_report_info->thresh2 = p_event_id->event_a5.a5_threshold2.threshold_rsrq;
        }
        *is_report_object_found = RRM_TRUE;
    }
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: store_meas_report_info_interrat
 *  INPUT        : p_rrm_ue_context,meas_id
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
store_meas_report_info_interrat(rrm_meas_config_t                          *p_meas_config ,
        meas_report_info_node_t                    *p_meas_report_info ,
        report_config_to_add_mod_report_config_t   *p_report_config ,
        U8                                         *is_report_object_found)
{

    report_config_interrat_trigger_type_event_eventid_t  *p_interrat_event_id = RRM_PNULL;
    U16                 event_id_bitmask   = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    p_interrat_event_id = &(p_report_config->report_config_interrat.trigger_type.event.event_id); 

    event_id_bitmask = p_interrat_event_id->bitmask;

    if(event_id_bitmask & REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_B2;

        if( p_interrat_event_id->event_b2.bitmask & 
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT)
        {
			    /*SPR_17893_START*/
			    p_meas_report_info->rat_type = RRM_IRAT_PRIORITY_UTRAN; 
			    /*SPR_17893_END*/
            if(p_interrat_event_id->event_b2.b2_threshold_utra.bitmask 
                    & THRESHOLD_UTRA_RSCP_PRESENT)
            {
                p_meas_report_info->thresh_rscp = 
                    p_interrat_event_id->event_b2.b2_threshold_utra.threshold_rscp;
                /* Coverity_ID : 16535 */
                p_meas_report_info->trigger_quantity =    RRMC_RSRP; /* RRM_RRC_CPICH_RSCP */
            }
            else
            {
                p_meas_report_info->thresh_ecn0 = 
                    p_interrat_event_id->event_b2.b2_threshold_utra.threshold_ecn0;
                /* Coverity_ID : 16534 */
                p_meas_report_info->trigger_quantity =   RRMC_RSRQ; /* RRM_RRC_CPICH_ECN0 */
            } 
            *is_report_object_found = RRM_TRUE;
        }
        if ( p_interrat_event_id->event_b2.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT)
        {
            p_meas_report_info->thresh_rssi = p_interrat_event_id->event_b2.\
                                              b2_threshold_geran;

            p_meas_report_info->trigger_quantity = RRMC_RSRP;
            *is_report_object_found = RRM_TRUE;
        }	      

        if ( p_interrat_event_id->event_b2.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_CDMA2000_PRESENT)
        {
            p_meas_report_info->thresh_rssi = p_interrat_event_id->event_b2.\
                                              b2_threshold_cdma2000;

            /* Coverity_ID : 31035 remove code after disscussion with Prashant and CP */
            *is_report_object_found = RRM_ONE;
        }
        /* Meas_Gap changes Start */
        if (p_meas_config->meas_gap_config.setup_config_info.bitmask &
                MEAS_GAP_CONFIG_GP0_PRESENT)
        {
            p_meas_config->meas_gap_config.setup_config_info.bitmask  =
                MEAS_GAP_CONFIG_GP1_PRESENT;
        }
        else
        {
            p_meas_config->meas_gap_config.setup_config_info.bitmask  =
                MEAS_GAP_CONFIG_GP0_PRESENT;
        }
        /* Meas_Gap changes End */

    }
    /* SPR_14564_fix: start */
    else if((p_report_config->report_config_interrat.trigger_type.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_PERIODICAL_PRESENT) &&
            (RRM_INTERRAT_REPORT_STRONGEST_CELLS_FOR_SON ==
             p_report_config->report_config_interrat.trigger_type.periodical.purpose))
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON;
        *is_report_object_found = RRM_TRUE;
    }
    /* SPR_14564_fix: end */
    else if(event_id_bitmask & REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_PRESENT)
    {
        p_meas_report_info->reporting_event = MEAS_REP_EVENT_B1;

        if( p_interrat_event_id->event_b1.bitmask & 
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT)
        {
			/*SPR_17893_START*/
			p_meas_report_info->rat_type = RRM_IRAT_PRIORITY_UTRAN; 
		    /*SPR_17893_END*/
            if(p_interrat_event_id->event_b1.b1_threshold_utra.bitmask 
                    & THRESHOLD_UTRA_RSCP_PRESENT)
            {
                p_meas_report_info->thresh_rscp = 
                    p_interrat_event_id->event_b1.b1_threshold_utra.threshold_rscp;
                /* Coverity_ID : 16535 */
                p_meas_report_info->trigger_quantity = RRMC_RSRP; /* RRM_RRC_CPICH_RSCP */
            }
            else
            {
                p_meas_report_info->thresh_ecn0 = 
                    p_interrat_event_id->event_b1.b1_threshold_utra.threshold_ecn0;
                /* Coverity_ID : 16534 */
                p_meas_report_info->trigger_quantity = RRMC_RSRQ;/* RRM_RRC_CPICH_ECN0 */
            } 
            *is_report_object_found = RRM_TRUE;
        }
        if ( p_interrat_event_id->event_b1.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT)
        {
            p_meas_report_info->thresh_rssi = p_interrat_event_id->event_b1.\
                                              b1_threshold_geran;

            p_meas_report_info->trigger_quantity = RRMC_RSRP;
            *is_report_object_found = RRM_TRUE;

        }
        if( p_interrat_event_id->event_b1.bitmask &
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT)
        {
            /* Coverity_ID : 30250 remove code after disscussion with Prashant and CP */
            *is_report_object_found = 1;

        }

        /* Meas_Gap changes Start */
        p_meas_config->meas_gap_config.setup_config_info.bitmask  =
            MEAS_GAP_CONFIG_GP0_PRESENT;
        /* Meas_Gap changes End */
    }

/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
    else if (event_id_bitmask & REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_NR_PRESENT)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "event_id_bitmask[%d]\n",event_id_bitmask);
        p_meas_report_info->reporting_event  = MEAS_REP_EVENT_B1_NR;
        p_meas_report_info->is_meas_report_pending = RRM_TRUE;
        if( p_interrat_event_id->event_b1_nr.b1_threshold_nr_r15.bitmask & 
                    THRESHOLD_NR_R15_RSRP_PRESENT)
        {
            p_meas_report_info->b1_nr_threshold_rsrp_r15 = p_interrat_event_id->event_b1_nr.\
                                                    b1_threshold_nr_r15.threshold_rsrp;
        }
        if( p_interrat_event_id->event_b1_nr.b1_threshold_nr_r15.bitmask & 
                    THRESHOLD_NR_R15_RSRQ_PRESENT)
        {
            p_meas_report_info->b1_nr_threshold_rsrq_r15 = p_interrat_event_id->event_b1_nr.\
                                                    b1_threshold_nr_r15.threshold_rsrq;
        }
        if( p_interrat_event_id->event_b1_nr.b1_threshold_nr_r15.bitmask & 
                    THRESHOLD_NR_R15_SINR_PRESENT)
        {
            p_meas_report_info->b1_nr_threshold_sinr_r15 = p_interrat_event_id->event_b1_nr.\
                                                    b1_threshold_nr_r15.threshold_sinr;
        }
        p_meas_report_info->trigger_quantity = RRMC_RSRP; /* RRM_RRC_CPICH_RSCP */
        *is_report_object_found = RRM_TRUE;

        /* TODO : Need to check if we have to store parameters for Meas Gap as well */
    }
#endif
/* ENDC_MEAS_CHANGES_END */
    
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: store_meas_report_info
 *  INPUT        : p_rrm_ue_context,meas_id
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
store_meas_report_eutra(rrm_meas_config_t          *p_meas_config ,
        meas_report_info_node_t    *p_meas_report_info ,
        U8                         counter)
{
    U8                  pci_count = RRM_ZERO;
    /* SPR:12849 Start */
    rrm_meas_obj_cell_to_add_info_t *p_cell_to_add_mod = RRM_PNULL;
    /* SPR:12849 End */
    RRM_UT_TRACE_ENTER();

    if (p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].\
            meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT )
    {			
        p_meas_report_info->carrier_earfcn = p_meas_config->meas_object_to_add_mod_list.\
                                             meas_object_to_add_mod[counter].meas_object.\
                                             meas_object_eutra.carrier_freq;

        /*spr_12273_fix*/

        for(pci_count = RRM_ZERO; pci_count < p_meas_config->meas_object_to_add_mod_list.\
                meas_object_to_add_mod[counter].meas_object.\
                meas_object_eutra.cells_to_add_mod_list.count;
                pci_count++)
        {
            /* SPR:12849 Start */
            p_cell_to_add_mod = (rrm_meas_obj_cell_to_add_info_t *)rrm_mem_get(sizeof(rrm_meas_obj_cell_to_add_info_t));
            if (p_cell_to_add_mod == RRM_PNULL)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "Memory Allocation Failure");
                return;
            }
            p_cell_to_add_mod->phy_cell_id =  
                p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].meas_object.\
                meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[pci_count].phys_cell_id;

            /* spr_12273_fix_start*/
            p_cell_to_add_mod->cell_index =  
                p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].meas_object.\
                meas_object_eutra.cells_to_add_mod_list.cells_to_add_mod[pci_count].cell_index;
            /* spr_12273_fix_end*/
            ylPushTail(&(p_meas_report_info->cells_to_add_mod_list),&( p_cell_to_add_mod->node));
            /* SPR:12849 End */

        }
    }
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: store_meas_report_info
 *  INPUT        : p_rrm_ue_context,meas_id
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
store_meas_report_utra_geran_cdma(rrm_meas_config_t          *p_meas_config ,
        meas_report_info_node_t    *p_meas_report_info ,
        U8                         counter)
{

    RRM_UT_TRACE_ENTER();
    if (p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].\
            meas_object.bitmask & MEAS_OBJECT_TO_ADD_UTRA_PRESENT )
    {
        p_meas_report_info->carrier_earfcn = p_meas_config->meas_object_to_add_mod_list.\
                                             meas_object_to_add_mod[counter].meas_object.\
                                             meas_object_utra.carrier_freq;
    }			
    else if (p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].\
            meas_object.bitmask & MEAS_OBJECT_TO_ADD_GERAN_PRESENT )
    {
        p_meas_report_info->carrier_earfcn = p_meas_config->meas_object_to_add_mod_list.\
                                             meas_object_to_add_mod[counter].meas_object.\
                                             meas_object_geran.carrier_freqs.starting_arfcn;
        p_meas_report_info->band_indicator = p_meas_config->meas_object_to_add_mod_list.\
                                             meas_object_to_add_mod[counter].meas_object.\
                                             meas_object_geran.carrier_freqs.band_indicator;
    }	
    /*+ Coverity CID : 21180 +*/
    /*
       else if (p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].\
       meas_object.bitmask & MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT )
       */
    else if (p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].\
            meas_object.bitmask & MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT )
        /*- Coverity CID : 21180 -*/

    {
        p_meas_report_info->carrier_earfcn = p_meas_config->meas_object_to_add_mod_list.\
                                             meas_object_to_add_mod[counter].meas_object.\
                                             meas_object_cdma2000.carrier_freq_cdma2000.arfcn;
        p_meas_report_info->band_indicator = p_meas_config->meas_object_to_add_mod_list.\
                                             meas_object_to_add_mod[counter].meas_object.\
                                             meas_object_cdma2000.carrier_freq_cdma2000.band_class;
    }	
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_end*/


/******************************************************************************
 *  FUNCTION NAME: store_meas_report_info
 *  INPUT        : p_rrm_ue_context,meas_id
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the 
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t
store_meas_report_info(
        rrm_meas_config_t          *p_meas_config,
        rrm_ue_context_t           *p_rrm_ue_context,
        U8                          meas_id,
        rrm_meas_func_ptr          meas_func_ptr,
        meas_type_et               meas_type
        ) 
{
    RRM_UT_TRACE_ENTER();
    U8                  meas_ids_count     = RRM_ZERO;
    U8                  report_conf_count  = RRM_ZERO;
    U8                  meas_object_count  = RRM_ZERO;
    U8                  counter            = RRM_ZERO;
    U8                  report_config_id   = RRM_ZERO;
    U8                  meas_object_id     = RRM_ZERO;
    U8                  is_report_object_found = RRM_ZERO;

    report_config_to_add_mod_report_config_t    *p_report_config = RRM_PNULL;

    meas_report_info_node_t *p_meas_report_info = RRM_PNULL;
    p_meas_report_info       =(meas_report_info_node_t*) rrm_mem_get(sizeof(meas_report_info_node_t));
    if (p_meas_report_info == RRM_PNULL)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "Memory Allocation Failure");
        return;
    }

    RRM_MEMSET(p_meas_report_info, RRM_ZERO, sizeof(meas_report_info_node_t));
    /*SPR_17893_START*/
    /* SPR 20653 Fix Start */
    p_meas_report_info->rat_type = RRM_IRAT_PRIORITY_MAX; 
    /* SPR 20653 Fix End */
    /*SPR_17893_END*/

    meas_ids_count =  p_meas_config->meas_id_to_add_mod_list.count;

    report_conf_count = p_meas_config->report_config_to_add_mod_list.
        count;

    meas_object_count = p_meas_config->meas_object_to_add_mod_list.
        count;
    /* SPR-12035-fix start */
    p_meas_report_info->meas_type = meas_type;
    /* SPR-12035-fix end */
    /*meas_type provided as argument 
      p_meas_report_info->meas_type = MEAS_TYPE_HO; */
    p_meas_report_info->meas_type = meas_type;     
    for (counter =RRM_ZERO;counter<meas_ids_count;counter ++)
    {
        if (p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[counter].\
                meas_id == meas_id)
        {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "start storing for meas_id [%d]",meas_id);
            meas_object_id = p_meas_config->meas_id_to_add_mod_list.\
                             meas_id_to_add_mod[counter].meas_object_id;
            report_config_id = p_meas_config->meas_id_to_add_mod_list.\
                               meas_id_to_add_mod[counter].report_config_id;
            p_meas_report_info->meas_object_id = meas_object_id;
            p_meas_report_info->report_config_id = report_config_id;
            p_meas_report_info->meas_status = RRM_MEAS_CONFIG_REQ_SENT;
            p_meas_report_info->meas_id = meas_id;  
            p_meas_report_info->register_meas_func_type= meas_func_ptr;  

            break;
        }
    }

    for (counter = RRM_ZERO;counter <report_conf_count; counter ++)
    {
        if (p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[counter].
                report_config_id == report_config_id)
        {
            /* BLR Inter RAT changes start */
            p_report_config = &(p_meas_config->report_config_to_add_mod_list.\
                    report_config_to_add_mod[counter].report_config);

            if( p_report_config->bitmask  & REPORT_CONFIG_EUTRA_PRESENT )
            { 

                /*Klockwork_fix_start*/
                /*SPR 17777 +-*/
                store_meas_report_info_eutra(p_meas_report_info ,p_report_config ,&is_report_object_found);
                /*Klockwork_fix_end*/
                /* coverity : CID 16539*/
            }
            /* BLR Inter RAT handover changes start */
            else if( p_report_config->bitmask  & REPORT_CONFIG_INTERRAT_PRESENT )
            {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                " store_meas_report_info_interrat \n");
                /*Klockwork_fix_start*/
                store_meas_report_info_interrat(p_meas_config ,p_meas_report_info ,p_report_config ,&is_report_object_found);
                /*Klockwork_fix_end*/
            }
            /* BLR Inter RAT handover changes end */
        }
    }
    /* SPR14165start*/
    if (is_report_object_found || (meas_type == MEAS_TYPE_FOR_VICTIM)) {
        ylPushTail(&(p_rrm_ue_context->meas_report_list),&(p_meas_report_info->sNode));
        /* SPR:12849 Start */ 
        ylInit(&(p_meas_report_info->cells_to_add_mod_list));
        /* SPR:12849 End */ 

        for (counter = RRM_ZERO;counter <meas_object_count; counter ++)
        {
            if (p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[counter].	    
                    meas_object_id == meas_object_id )
            {
                /*Klockwork_fix_start*/
                store_meas_report_eutra(p_meas_config ,p_meas_report_info ,counter);
                store_meas_report_utra_geran_cdma(p_meas_config ,p_meas_report_info ,counter);
                /*Klockwork_fix_end*/
            }		
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                " Report_config_id = %d present in Measurement Id %d mapping to " 
                "Measurement_object_id = %d is not configured ",
                report_config_id, meas_id, meas_object_id);
        /* COVERITY : RESOURCE LEAK : CID : 54406 fix start */
        if( RRM_NULL != p_meas_report_info )
            RRM_MEM_FREE(p_meas_report_info);
        /* COVERITY : RESOURCE LEAK : CID : 54406 fix end*/
    }

    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: store_meas_report_info_for_ue_pos
 *  INPUT        : p_rrm_ue_context,meas_id
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the UE positioning meas info into  the UE' meas list 
 *  RETURNS      : None
 ******************************************************************************/
static rrm_void_t
store_meas_report_info_for_ue_pos(
        /*SPR_8004:fix_start*/
        rrm_meas_config_t      *p_meas_config,
        /*SPR_8004:fix_end*/
        rrm_ue_context_t           *p_rrm_ue_context,
        rrm_bool_et                 is_req_for_rach_info,
        rrm_meas_func_ptr          meas_func_ptr
        ) 
{
    RRM_UT_TRACE_ENTER();

    /*SPR_8004:fix_start*/
    U8 counter  = RRM_ZERO;
    U8 meas_ids_count = RRM_ZERO;
    /*SPR_8004:fix_end*/
    meas_report_info_node_t *p_meas_report_info = RRM_PNULL;

    /*SPR_8004:fix_start*/
    if(RRM_PNULL != p_meas_config && RRM_PNULL != p_rrm_ue_context)
    {
        /*cov fix start 62972*/
        meas_ids_count =  p_meas_config->meas_id_to_add_mod_list.count;
        /*cov fix end 62972*/
        for(counter =RRM_ZERO;counter<meas_ids_count;counter ++)
        {
            p_meas_report_info = (meas_report_info_node_t*) rrm_mem_get(sizeof(meas_report_info_node_t));
            if (p_meas_report_info != RRM_PNULL)
            {
                RRM_MEMSET(p_meas_report_info, RRM_ZERO, sizeof(meas_report_info_node_t));
                if(RRM_FALSE == is_req_for_rach_info)
                {
                    p_meas_report_info->meas_type = MEAS_TYPE_ECID;
                }
                else
                {
                    p_meas_report_info->meas_type = MEAS_TYPE_RACH;
                }
                /*Cov_fix_start_64540*/
                p_meas_report_info->meas_object_id = p_meas_config->meas_id_to_add_mod_list.\
                                                     meas_id_to_add_mod[counter].meas_object_id;
                p_meas_report_info->report_config_id = p_meas_config->meas_id_to_add_mod_list.\
                                                       meas_id_to_add_mod[counter].report_config_id;
                p_meas_report_info->meas_status = RRM_MEAS_CONFIG_REQ_SENT;
                p_meas_report_info->meas_id = p_meas_config->meas_id_to_add_mod_list.\
                                              meas_id_to_add_mod[counter].meas_id;
                p_meas_report_info->register_meas_func_type = meas_func_ptr;
                /*Cov_fix_end_64540*/
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "MEAS_ID=%d request for ECID stored in the [UE:%d] list", p_meas_config->meas_id_to_add_mod_list.\
                        meas_id_to_add_mod[counter].meas_id, p_rrm_ue_context->ue_index);

                ylPushTail(&(p_rrm_ue_context->meas_report_list),&(p_meas_report_info->sNode));

            }   

            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "Memory Allocation Failure");
                /* COVERITY : RESOURCE LEAK : CID : 54407 fix start */
                if( RRM_NULL != p_meas_report_info )
                    RRM_MEM_FREE( p_meas_report_info );
                /* COVERITY : RESOURCE LEAK : CID : 54407 fix end */
                return;
            }

        }
    }
    /*SPR_8004:fix_end*/
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "Either p_meas_id_to_add_mod or p_rrm_ue_context is NULL");
    }

    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_start*/
/******************************************************************************
 *  FUNCTION NAME: fill_meas_report_ids_info_a5
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t
fill_meas_report_ids_info_a5(rrm_meas_config_t                *p_meas_config ,
        rrm_ue_context_t                 *p_rrm_ue_context ,
        U8                               rep_index ,
        U8                               *index ,
        /* SPR 15261 start - last parameter added*/
        U8                               meas_object_starting_index) 
/* SPR 15261 end */

{
    U8                 meas_index             = RRM_ZERO;
    U8                 meas_id                = RRM_ZERO;
    U8                 report_config_id       = RRM_ZERO;
    rrm_meas_func_ptr  meas_func_ptr          = RRM_PNULL;
    meas_type_et       meas_type              = MEAS_TYPE_INVALID;
    /* SPR 15261 start */
    /*Line deleted*/
    /* SPR 15261 end */
    U8  meas_object_count      = p_meas_config->meas_object_to_add_mod_list.count;
    U8  meas_object_id         = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    /* SPR 10498 Fix Start */
    /* Store target_earfcn to be used for selecting EUTRAN DAHO - intra or inter-freq
       while blind timer expiry */
    p_rrm_ue_context->ho_params.target_earfcn = p_meas_config->meas_object_to_add_mod_list.
        meas_object_to_add_mod[meas_index].meas_object.meas_object_eutra.carrier_freq;
    /* SPR 10498 Fix End */

    for( meas_index = meas_object_starting_index; meas_index < meas_object_count;meas_index ++)
    {
        if ( p_meas_config->meas_object_to_add_mod_list.\
                meas_object_to_add_mod[meas_index].meas_object.bitmask & 
                MEAS_OBJECT_TO_ADD_EUTRA_PRESENT ) 
        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;
                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;
                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Maximum Measurement Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                /* COVERITY : RESOURCE LEAK : CID : 63263 fix start */
                /* COVERITY : RESOURCE LEAK : CID : 63263 fix end */
                return;
            }
        }
    }
}
/******************************************************************************
 *  FUNCTION NAME: fill_meas_report_ids_info_b1
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_meas_report_ids_info_b1(rrm_meas_config_t          *p_meas_config,
        rrm_ue_context_t           *p_rrm_ue_context,
        U8                         rep_index ,
        U8                         *index)
{
    U8                 meas_index             = RRM_ZERO;
    U8                 meas_id                = RRM_ZERO;
    U8                 report_config_id       = RRM_ZERO;
    rrm_meas_func_ptr  meas_func_ptr          = RRM_PNULL;
    meas_type_et       meas_type              = MEAS_TYPE_INVALID;
    U8                 meas_object_id         = RRM_ZERO;
    U8                 valid_meas_object_id   = RRM_ZERO; 
    U8  meas_object_count      = p_meas_config->meas_object_to_add_mod_list.count;

    RRM_UT_TRACE_ENTER();

    for( meas_index =RRM_ZERO ; meas_index < meas_object_count;meas_index ++)
    {
        if ( (p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[meas_index].meas_object.bitmask & 
                    MEAS_OBJECT_TO_ADD_UTRA_PRESENT)  && 
                (p_meas_config->report_config_to_add_mod_list. \
                 report_config_to_add_mod[rep_index].report_config.report_config_interrat. \
                 trigger_type.event.event_id.event_b1.bitmask &
                 REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT))
        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;

                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;
                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
            }
        }
        else if( (p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[meas_index].meas_object.bitmask &
                    MEAS_OBJECT_TO_ADD_GERAN_PRESENT) && 
                (p_meas_config->report_config_to_add_mod_list. \
                 report_config_to_add_mod[rep_index].report_config.report_config_interrat. \
                 trigger_type.event.event_id.event_b1.bitmask &
                 REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT) )
        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;

                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;
                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
            }    
        }
        /*CDMA2000 changes*/
        else if( (p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[meas_index].meas_object.bitmask &
                    MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT) &&
                (p_meas_config->report_config_to_add_mod_list. \
                 report_config_to_add_mod[rep_index].report_config.report_config_interrat. \
                 trigger_type.event.event_id.event_b1.bitmask &
                 REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT))

        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;
                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;

                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);

                for(valid_meas_object_id = RRM_ZERO; 
                        valid_meas_object_id < p_rrm_ue_context-> \
                        p_meas_obj_cdma2000_info-> \
                        count_meas_object_hrpd;
                        valid_meas_object_id++)
                {
                    if(meas_object_id == p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_hrpd[valid_meas_object_id].meas_obj_id) 
                    {
                        p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_hrpd[valid_meas_object_id]. \
                            meas_id = meas_id;
                        break;
                    }
                }

                for(valid_meas_object_id = RRM_ZERO; 
                        valid_meas_object_id < p_rrm_ue_context-> \
                        p_meas_obj_cdma2000_info-> \
                        count_meas_object_1xrtt;
                        valid_meas_object_id++)
                {
                    if(meas_object_id == p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_1xrtt[valid_meas_object_id].meas_obj_id)
                    {
                        p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_1xrtt[valid_meas_object_id]. \
                            meas_id = meas_id;
                        break;
                    }
                }
            }
        }
        /*CDMA2000 changes*/
    }
}
/******************************************************************************
 *  FUNCTION NAME: fill_meas_report_ids_info_b2
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_meas_report_ids_info_b2(rrm_meas_config_t          *p_meas_config,
        rrm_ue_context_t           *p_rrm_ue_context,
        U8                         rep_index ,
        U8                         *index )
{
    U8                 meas_index             = RRM_ZERO;
    U8                 meas_id                = RRM_ZERO;
    U8                 report_config_id       = RRM_ZERO;
    rrm_meas_func_ptr  meas_func_ptr          = RRM_PNULL;
    meas_type_et       meas_type              = MEAS_TYPE_INVALID;
    U8                 meas_object_id         = RRM_ZERO;
    U8                 valid_meas_object_id   = RRM_ZERO;
    U8  meas_object_count      = p_meas_config->meas_object_to_add_mod_list.count;

    RRM_UT_TRACE_ENTER();

    for( meas_index = RRM_ZERO; meas_index < meas_object_count;meas_index ++)
    {
        if ( (p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[meas_index].meas_object.bitmask & 
                    MEAS_OBJECT_TO_ADD_UTRA_PRESENT)  && 
                (p_meas_config->report_config_to_add_mod_list. \
                 report_config_to_add_mod[rep_index].report_config.report_config_interrat. \
                 trigger_type.event.event_id.event_b2.bitmask &
                 REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT))
        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;
                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;

                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
            }
        }
        else if( (p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[meas_index].meas_object.bitmask &
                    MEAS_OBJECT_TO_ADD_GERAN_PRESENT) && 
                (p_meas_config->report_config_to_add_mod_list. \
                 report_config_to_add_mod[rep_index].report_config.report_config_interrat. \
                 trigger_type.event.event_id.event_b2.bitmask &
                 REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT) )
        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;
                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;

                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
            }    
        }
        /*CDMA2000 changes*/
        else if( (p_meas_config->meas_object_to_add_mod_list.\
                    meas_object_to_add_mod[meas_index].meas_object.bitmask &
                    MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT) &&
                (p_meas_config->report_config_to_add_mod_list. \
                 report_config_to_add_mod[rep_index].report_config.report_config_interrat. \
                 trigger_type.event.event_id.event_b2.bitmask &
                 REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_CDMA2000_PRESENT))

        {
            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
            if(meas_id <= MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_id = meas_id;
                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                 meas_object_to_add_mod[meas_index].meas_object_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    meas_object_id = meas_object_id ;

                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                   report_config_to_add_mod[rep_index].report_config_id;

                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                    report_config_id = report_config_id;

                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                        meas_id, meas_object_id, report_config_id);

                (*index)++;

                p_meas_config->meas_id_to_add_mod_list.count++;
                /*SPR 16764 START*/
                meas_func_ptr = rrm_uem_meas_results_ind;
                /*SPR 16764 END*/
                meas_type = MEAS_TYPE_HO;

                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);

                for(valid_meas_object_id = RRM_ZERO; 
                        valid_meas_object_id < p_rrm_ue_context-> \
                        p_meas_obj_cdma2000_info-> \
                        count_meas_object_hrpd;
                        valid_meas_object_id++)
                {
                    if(meas_object_id == p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_hrpd[valid_meas_object_id].meas_obj_id) 
                    {
                        p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_hrpd[valid_meas_object_id]. \
                            meas_id = meas_id;
                        break;
                    }
                }

                for(valid_meas_object_id = RRM_ZERO; 
                        valid_meas_object_id < p_rrm_ue_context-> \
                        p_meas_obj_cdma2000_info-> \
                        count_meas_object_1xrtt;
                        valid_meas_object_id++)
                {
                    if(meas_object_id == p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_1xrtt[valid_meas_object_id].meas_obj_id)
                    {
                        p_rrm_ue_context->p_meas_obj_cdma2000_info->
                            meas_obj_cdma2000_cell_info_1xrtt[valid_meas_object_id]. \
                            meas_id = meas_id;
                        break;
                    }
                }
            }
        }
        /*CDMA2000 changes*/
    }
}
/******************************************************************************
 *  FUNCTION NAME: fill_meas_report_ids_info_a4
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_meas_report_ids_info_a4(rrm_meas_config_t              *p_meas_config,
        rrm_ue_context_t               *p_rrm_ue_context,
        rrm_cell_index_t               cell_index,
        U8                             rep_index ,
        U8                             *index )
{
    U8                 meas_index             = RRM_ZERO;
    U8                 meas_id                = RRM_ZERO;
    U8                 meas_object_starting_index  = RRM_ZERO; 
    U8                 report_config_id       = RRM_ZERO;
    rrm_meas_func_ptr  meas_func_ptr          = RRM_PNULL;
    meas_type_et       meas_type              = MEAS_TYPE_INVALID;
    U8                 meas_object_id         = RRM_ZERO;
    U8  meas_object_count      = p_meas_config->meas_object_to_add_mod_list.count;
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);

    RRM_UT_TRACE_ENTER();
    /* CID 65831:start*/
    if (RRM_PNULL != p_cell_ctxt)
    {
        /* SPR14165start */
        /* SPR14165end */
        /* SPR_14564_fix: start */
        /* code deleted */
        /* SPR_14564_fix: end */
        meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
        if(meas_id <= MAX_MEAS_OBJECT_ID)
        {
            p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                meas_id = meas_id;

            meas_object_id =  HO_MEAS_START_ID;
            p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                meas_object_id = meas_object_id;

            report_config_id = p_meas_config->report_config_to_add_mod_list.\
                               report_config_to_add_mod[rep_index].report_config_id;

            p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                report_config_id = report_config_id;

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                    meas_id, meas_object_id, report_config_id);
            (*index)++;
            p_meas_config->meas_id_to_add_mod_list.count++;
            /*SPR 16764 START*/
            meas_func_ptr = rrm_uem_meas_results_ind;
            /*SPR 16764 END*/
            meas_type = MEAS_TYPE_HO;
            store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Maximum Measurement Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
        }
        /* SPR_14564_fix: start */
        /* code deleted */
        /* SPR_14564_fix: end */
        if((RRM_TRUE == p_cell_ctxt->administrative_procedure_ongoing))
        {
            /* SPR_14564_fix: start */
            meas_object_starting_index = RRM_ONE;
            /* SPR_14564_fix: end */
            for( meas_index = meas_object_starting_index; meas_index < meas_object_count;meas_index ++)
            {
                if ( p_meas_config->meas_object_to_add_mod_list.\
                        meas_object_to_add_mod[meas_index].meas_object.bitmask & 
                        MEAS_OBJECT_TO_ADD_EUTRA_PRESENT ) 
                {
                    meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
                    if(meas_id <= MAX_MEAS_OBJECT_ID)
                    {
                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                            meas_id = meas_id;
                        meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                         meas_object_to_add_mod[meas_index].meas_object_id;

                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                            meas_object_id = meas_object_id ;

                        report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                           report_config_to_add_mod[rep_index].report_config_id;

                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[(*index)].
                            report_config_id = report_config_id;

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                                meas_id, meas_object_id, report_config_id);

                        (*index)++;

                        p_meas_config->meas_id_to_add_mod_list.count++;
                        /*SPR 16764 START*/
                        meas_func_ptr = rrm_uem_meas_results_ind;
                        /*SPR 16764 END*/
                        meas_type = MEAS_TYPE_HO;

                        store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Maximum Measurement Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                        /* COVERITY : RESOURCE LEAK : CID : 63263 fix start */
                        /* COVERITY : RESOURCE LEAK : CID : 63263 fix end */
                        return;
                    }
                }
            }
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
        RRM_UT_TRACE_EXIT();
    }
    /* CID 65831:end*/
}
/*Klockwork_fix_end*/
/******************************************************************************
 *  FUNCTION NAME: fill_meas_report_ids_info
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran meas ids by mapping meas config ids to the 
 *                 corresponding reporting config ids to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_meas_report_ids_info(
        rrm_meas_config_t          *p_meas_config,
        rrm_ue_context_t           *p_rrm_ue_context,
        rrm_cell_index_t           cell_index, 
        rrm_bool_et                is_meas_for_masa
        )
{
    RRM_UT_TRACE_ENTER();

    rrm_meas_func_ptr         meas_func_ptr = RRM_PNULL;
    meas_type_et       meas_type = MEAS_TYPE_INVALID;

    U8  meas_id                = RRM_ZERO;
    U8  rep_index              = RRM_ZERO;
    U8  index                  = RRM_ZERO;
    U8  report_config_id       = RRM_ZERO;
    U8  meas_object_id         = RRM_ZERO;
    /*CA HARDENING CHANGES 6.3.0*/
    U8  reporting_config_count = p_meas_config->report_config_to_add_mod_list.count;
    /* SPR_14564_fix: start */
    /* code deleted */
    /* SPR_14564_fix: end */
    //U8  intra_freq_neighbour_count = RRM_NULL;
    /* SPR 17258 Start*/
    rrm_bool_et copy_new_added_meas_object= RRM_FALSE;
    meas_object_to_add_mod_list_t      *new_added_meas_object_list = RRM_PNULL;
    /* SPR 17258 End*/

    /* SPR_14564_fix: start */
    /* code deleted */
    /* SPR_14564_fix: end */
    //intra_freq_neighbour_count =  p_ncl_params->lte_ncl.num_valid_intra_freq_cell;
    report_config_eutra_t    *p_report_config_eutra          = RRM_PNULL;

    /* SPR 15261 start */
    U8  meas_index                  = RRM_ZERO;
    U32   feature_group_indicators  = (U32)-RRM_ONE;
    U8  meas_object_starting_index = RRM_ZERO;
    U8  meas_object_count      = p_meas_config->meas_object_to_add_mod_list.count;
    /* SPR 15261 end */

    /* BLR Inter RAT Handover changes start */

    report_config_interrat_t    *p_report_config_interrat          = RRM_PNULL;

    /* SPR 15261 start */
    rf_params_t         *p_rf_params         = RRM_PNULL;
    p_rf_params = rrm_cellm_get_rf_params(p_rrm_ue_context->cell_index);
    if(RRM_PNULL == p_rf_params)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "p_rf_params not found");
        RRM_UT_TRACE_EXIT();
        return;
    }
    /* SPR 15261 end */

    /* SPR_14165start */
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        RRM_UT_TRACE_EXIT();
        return;
    }
    /* SPR14165end */
    /* START: CSR 00057390*/

    /* SPR 15261 start */
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

        feature_group_indicators = p_rrm_ue_context->ue_capability_params.
            rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.feature_group_indicators;
    }
    /* SPR 15261 end */

    if(reporting_config_count > RRM_ZERO)
    {
        /*SPR 17258 Start*/
        new_added_meas_object_list = (meas_object_to_add_mod_list_t *)rrm_mem_get(sizeof(meas_object_to_add_mod_list_t));
        if(new_added_meas_object_list != RRM_PNULL)
        {
            RRM_MEMSET(new_added_meas_object_list, RRM_ZERO, sizeof(meas_object_to_add_mod_list_t));
            copy_new_added_meas_object=
                rrm_fill_new_added_meas_object(p_rrm_ue_context,p_meas_config,new_added_meas_object_list);

            /*SPR 17258 End*/

            for (rep_index = RRM_ZERO;rep_index <reporting_config_count;rep_index ++)
            {

                p_report_config_eutra  = &(p_meas_config->report_config_to_add_mod_list.
                        report_config_to_add_mod[rep_index].report_config.
                        report_config_eutra);

                /* BLR Inter RAT Handover changes */
                p_report_config_interrat  = &(p_meas_config->report_config_to_add_mod_list.
                        report_config_to_add_mod[rep_index].report_config.
                        report_config_interrat);

                /*  p_event                = (report_config_eutra_trigger_type_event_t*)
                    &(p_report_config_eutra->trigger_type.event);
                    if (p_event->event_id.bitmask &
                    REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT )*/


                if (p_report_config_eutra->trigger_type.event.event_id.bitmask &
                        REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT)
                {

                    /* SPR-22308 START */
                    meas_object_starting_index = RRM_ONE;
                    /* SPR-22308 END */

                    /*Klockwork_fix_start*/
                    /* SPR 15261 start - last parameter added*/
                    fill_meas_report_ids_info_a5(p_meas_config ,p_rrm_ue_context ,
                            rep_index ,&index ,meas_object_starting_index);
                    /* SPR 15261 end */
                    /*Klockwork_fix_end*/
                }
                else if ( p_report_config_interrat->trigger_type.event.event_id.bitmask &
                        REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_PRESENT) 
                {
                    /*Klockwork_fix_start*/
                    fill_meas_report_ids_info_b1(p_meas_config ,p_rrm_ue_context ,
                            rep_index ,&index);
                    /*Klockwork_fix_end*/
                }
                /* SPR_14564_fix: start */
                else if ((p_report_config_interrat->trigger_type.bitmask &
                            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_PERIODICAL_PRESENT) &&
                        (RRM_INTERRAT_REPORT_STRONGEST_CELLS_FOR_SON == 
                         p_report_config_interrat->trigger_type.periodical.purpose))
                {
                    for( meas_index = RRM_ZERO; meas_index < meas_object_count;meas_index ++)
                    {
                        if ( (p_meas_config->meas_object_to_add_mod_list.\
                                    meas_object_to_add_mod[meas_index].meas_object.bitmask &
                                    MEAS_OBJECT_TO_ADD_UTRA_PRESENT))
                        {
                            meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
                            if(meas_id <= MAX_MEAS_OBJECT_ID)
                            {
                                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                    meas_id = meas_id;
                                meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                                 meas_object_to_add_mod[meas_index].meas_object_id;

                                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                    meas_object_id = meas_object_id ;

                                report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                                   report_config_to_add_mod[rep_index].report_config_id;

                                p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                    report_config_id = report_config_id;

                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                        "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                                        meas_id, meas_object_id, report_config_id);

                                index++;

                                p_meas_config->meas_id_to_add_mod_list.count++;

                                store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
                            }
                        }
                    }
                }
                /* SPR_14564_fix: end */
                else if ( p_report_config_interrat->trigger_type.event.event_id.bitmask &
                        REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_PRESENT) 
                {
                    /*Klockwork_fix_start*/
                    fill_meas_report_ids_info_b2(p_meas_config ,p_rrm_ue_context ,
                            rep_index ,&index);
                    /*Klockwork_fix_end*/

                }
                /* Start :SPR 8720 */
                else if ((p_report_config_eutra->trigger_type.event.event_id.bitmask &
                            REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A4_PRESENT))
                {
                    /*Klockwork_fix_start*/
                    fill_meas_report_ids_info_a4(p_meas_config ,p_rrm_ue_context ,cell_index,
                            rep_index ,&index);
                    /*Klockwork_fix_end*/
                }
                /* End :SPR 8720 */
                /* SPR 15261 start */
                else if ((p_report_config_eutra->trigger_type.event.event_id.bitmask &
                            REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A3_PRESENT))
                {
                    for( meas_index = RRM_ZERO; meas_index < meas_object_count;meas_index ++)
                    {
                        /*SPR 16406 21AUG Start*/

                        if ( p_meas_config->meas_object_to_add_mod_list.\
                                meas_object_to_add_mod[meas_index].meas_object.bitmask &
                                MEAS_OBJECT_TO_ADD_EUTRA_PRESENT)
                        {  
                            if(p_meas_config->meas_object_to_add_mod_list.\
                                    meas_object_to_add_mod[meas_index].meas_object.\
                                    meas_object_eutra.carrier_freq !=
                                    p_rf_params->rf_configurations.dl_earfcn)
                            {
                                /*SPR 16406 21AUG End */

                                if(!(feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_FOURTEEN))))
                                {
                                    meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
                                    if(meas_id <= MAX_MEAS_OBJECT_ID)
                                    {
                                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                            meas_id = meas_id;
                                        meas_object_id = p_meas_config->meas_object_to_add_mod_list.\
                                                         meas_object_to_add_mod[meas_index].meas_object_id;

                                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                            meas_object_id = meas_object_id ;

                                        report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                                           report_config_to_add_mod[rep_index].report_config_id;

                                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                            report_config_id = report_config_id;

                                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                                "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                                                meas_id, meas_object_id, report_config_id);

                                        index++;

                                        p_meas_config->meas_id_to_add_mod_list.count++;

                                        meas_type = MEAS_TYPE_HO;
                                        /*SPR 16764 START*/
                                        meas_func_ptr = rrm_uem_meas_results_ind;
                                        /*SPR 16764 END*/

                                        store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id ,meas_func_ptr ,meas_type);
                                    }
                                    else
                                    {
                                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                                "Maximum Measurement Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                                        /* SPR_15852_fix: code removed */
                                    }
                                }
                            }

                            else
                            {
                                meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
                                if(meas_id <= MAX_MEAS_OBJECT_ID)
                                {
                                    p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                        meas_id = meas_id;

                                    meas_object_id =  HO_MEAS_START_ID;
                                    p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                        meas_object_id = meas_object_id;

                                    report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                                       report_config_to_add_mod[rep_index].report_config_id;

                                    p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                                        report_config_id = report_config_id;

                                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                            "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                                            meas_id, meas_object_id, report_config_id);
                                    index++;
                                    p_meas_config->meas_id_to_add_mod_list.count++;
                                    meas_type = MEAS_TYPE_HO;
                                    /*SPR 16764 START*/
                                    meas_func_ptr = rrm_uem_meas_results_ind;
                                    /*SPR 16764 END*/
                                    store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id ,meas_func_ptr ,meas_type);
                                }
                                else
                                {
                                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                            "Maximum Measurement Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                                }
                            }
                            /*SPR 16406 21AUG Start*/
                        }
                        /* SPR 16406 21AUG Start */
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "meas_object not present for [UE:%d]", p_rrm_ue_context->ue_index);
                        }
                        /*SPR 16406 21AUG End */
                    }

                }
                /* SPR 15261 end */
                else
                {
                    meas_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_REPORT);
                    if(meas_id <= MAX_MEAS_OBJECT_ID)
                    {
                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                            meas_id = meas_id;
                        /*Removed hard coding meas_object_id = HO_MEAS_START_ID; */
                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                            meas_object_id = p_meas_config->meas_object_to_add_mod_list.
                            meas_object_to_add_mod[RRM_ZERO].meas_object_id;
                        report_config_id = p_meas_config->report_config_to_add_mod_list.\
                                           report_config_to_add_mod[rep_index].report_config_id;

                        p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].
                            report_config_id = report_config_id;
                        /*SPR 21464 Changes Start*/
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                                meas_id, p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[index].meas_object_id, report_config_id);
                        /*SPR 21464 Changes End*/
                        index++;
                        p_meas_config->meas_id_to_add_mod_list.count++;
                        if (is_meas_for_masa)
                        {
                            meas_func_ptr = rrm_uem_masa_meas_config_ind;
                            meas_type = MEAS_TYPE_CA;
                        }
                        /* SPR14165start */
                        else if(p_rrm_ue_context->rrm_eicic_meas_event_ongoing)
                        {

                            meas_func_ptr = rrm_uem_eicic_meas_config_ind;
                            meas_type = MEAS_TYPE_FOR_VICTIM;
                        }
                        /* SPR14165end */
                        else
                        {
                            /*SPR 16764 START*/
                            meas_func_ptr = rrm_uem_meas_results_ind;
                            /*SPR 16764 END*/
                            meas_type = MEAS_TYPE_HO;
                        }
                        store_meas_report_info(p_meas_config,p_rrm_ue_context, meas_id,meas_func_ptr, meas_type);
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Maximum Measurement Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }
                }
            }
            /*SPR 17258 Start*/
            if(copy_new_added_meas_object == RRM_TRUE)
            {
                /* SPR_15852_fix: start */
                if(new_added_meas_object_list->count > RRM_ZERO && new_added_meas_object_list->count <= MAX_MEAS_OBJECT_ID)
                    /* SPR_15852_fix: end */
                {
                    p_meas_config->meas_object_to_add_mod_list.count= new_added_meas_object_list->count;
                    RRM_MEMCPY(&p_meas_config->meas_object_to_add_mod_list,new_added_meas_object_list,sizeof(meas_object_to_add_mod_list_t));
                }
            }
            else
            {
                p_meas_config->bitmask &= (~UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT);
                p_meas_config->meas_object_to_add_mod_list.count= RRM_ZERO;
            }
            RRM_MEM_FREE(new_added_meas_object_list);
        }
        else
        {
            p_meas_config->bitmask &= (~UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT);
            p_meas_config->meas_object_to_add_mod_list.count= RRM_ZERO;
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "memory allocation failure for [UE:%d]", p_rrm_ue_context->ue_index);
            return;
        }

        /*SPR 17258 END*/
    }
    else
    {
        p_meas_config->bitmask &= (~UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT);
        p_meas_config->meas_object_to_add_mod_list.count= RRM_ZERO;
    }
    /* END: CSR 00057390*/
    if (p_meas_config->meas_id_to_add_mod_list.count >=RRM_ONE)
    {
        p_meas_config->bitmask |= UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT;
    }
    RRM_UT_TRACE_EXIT();
}
/* SPR_14564_fix: start */
/******************************************************************************
 * FUNCTION NAME : rrm_fill_report_config_for_StrongestCellsForSON 
 * INPUT         : p_rrm_irat_params,eutran_trigger_quantity,trigger_quantity
 * OUTPUT        : p_report_config_to_add_mod
 * DESCRIPTIONi  : Fills the reproting configuration information for B2 event
 *               : as part of measurement to be sent to RRC
 * RETURNS       : None
 ******************************************************************************/
    rrm_void_t
rrm_fill_report_config_for_StrongestCellsForSON(
        report_config_to_add_mod_t    *p_report_config_to_add_mod
        )
{
    RRM_UT_TRACE_ENTER();

    p_report_config_to_add_mod->report_config.bitmask |= REPORT_CONFIG_INTERRAT_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        trigger_type.bitmask |= REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_PERIODICAL_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        trigger_type.periodical.purpose = RRM_INTERRAT_REPORT_STRONGEST_CELLS_FOR_SON;

    /* In case purpose is set to reportStrongestCellsForSON only value 1 applies Ref. 36.331 */
    p_report_config_to_add_mod->report_config.report_config_interrat.
        max_report_cells = RRM_ONE;

    /* 240 ms time for report interval, so that UE gets sufficient time to scan the nearby n/w */
    p_report_config_to_add_mod->report_config.report_config_interrat.
        report_interval = RRM_RRC_REPORT_INTERVAL_MS_240;

    /*In case purpose is set to reportCGI or reportStrongestCellsForSON only value 1 applies Ref. 36.331 */
    p_report_config_to_add_mod->report_config.report_config_interrat.
        report_amount = RRM_RRC_REPORT_AMOUNT_1;

    RRM_UT_TRACE_EXIT();
}
/* SPR_14564_fix: end */
/* BLR Inter RAT changes start */
/******************************************************************************
 *  FUNCTION NAME : fill_report_config_for_event_B2
 *  INPUT         : p_rrm_irat_params,eutran_trigger_quantity,trigger_quantity
 *  OUTPUT        : p_report_config_to_add_mod
 *  DESCRIPTIONi  : Fills the reproting configuration information for B2 event 
 *                  as part of measurement to be sent to RRC
 *  RETURNS       : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_for_event_B2(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_irat_params_t              *p_rrm_irat_params,
        rrm_trigger_quantity_et         eutran_trigger_quantity,
        U8                              trigger_quantity,
        U8 eutra_thresh_rsrx)
{
    RRM_UT_TRACE_ENTER();

    report_config_interrat_trigger_type_event_t *p_event            = RRM_PNULL;

    p_event = &( p_report_config_to_add_mod->report_config.report_config_interrat.
            trigger_type.event);

    p_report_config_to_add_mod->report_config.
        bitmask  |= REPORT_CONFIG_INTERRAT_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        trigger_type.bitmask  |= REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_PRESENT;

    p_event->event_id.bitmask |=
        REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_PRESENT;

    p_event->hysteresis = p_rrm_irat_params->hysteresis;

    p_event->time_to_trigger = p_rrm_irat_params->time_to_trigger;

    if( eutran_trigger_quantity == RRM_RRC_RSRP )
    {
        p_event->event_id.event_b2.b2_threshold_eutra.bitmask = THRESHOLD_EUTRA_RSRP_PRESENT;
        p_event->event_id.event_b2.b2_threshold_eutra.threshold_rsrp = eutra_thresh_rsrx; 
    }
    //For limiting UE measurement
    else if (eutran_trigger_quantity == RRM_RRC_RSRQ ) 
    {
        p_event->event_id.event_b2.b2_threshold_eutra.bitmask = THRESHOLD_EUTRA_RSRQ_PRESENT;
        p_event->event_id.event_b2.b2_threshold_eutra.threshold_rsrq = eutra_thresh_rsrx; 
    }
    if( trigger_quantity == RRM_ZERO )
    {
        p_event->event_id.event_b2.bitmask  = 
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT;
        p_event->event_id.event_b2.b2_threshold_utra.bitmask =\
                                                              THRESHOLD_UTRA_RSCP_PRESENT;
        p_event->event_id.event_b2.b2_threshold_utra.threshold_rscp = 
            p_rrm_irat_params->b2_threshold_2utra_rscp; 
    }
    //For limiting UE measurement
    else if ( trigger_quantity == RRM_ONE ) 
    {
        p_event->event_id.event_b2.bitmask  = 
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT;
        p_event->event_id.event_b2.b2_threshold_utra.bitmask = THRESHOLD_UTRA_ECN0_PRESENT;
        p_event->event_id.event_b2.b2_threshold_utra.threshold_ecn0 = 
            p_rrm_irat_params->b2_threshold_2utra_ecn0; 
    }
    else if (trigger_quantity == RRM_TWO)
    {
        p_event->event_id.event_b2.bitmask |=
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT;
        p_event->event_id.event_b2.b2_threshold_geran =
            p_rrm_irat_params->b2_threshold_2geran;         
    }
    else if(trigger_quantity == RRM_THREE)
    {
        p_event->event_id.event_b2.bitmask |=
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_CDMA2000_PRESENT;
        p_event->event_id.event_b2.b2_threshold_cdma2000 =
            p_rrm_irat_params->b2_threshold_cdma2000;
    }

    p_report_config_to_add_mod->report_config.report_config_interrat.
        max_report_cells = p_rrm_irat_params->max_report_cells;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        report_interval =  p_rrm_irat_params->report_interval;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        report_amount = p_rrm_irat_params->report_amount;

    RRM_UT_TRACE_EXIT();
}
/* BLR Inter RAT changes end */
/******************************************************************************
 *  FUNCTION NAME: fill_report_config_for_event_A5
 *  INPUT        : p_rrm_common_params_for_eutra,trigger_quantity
 *  OUTPUT       : p_report_config_to_add_mod
 *  DESCRIPTION  : Fills the reproting configuration information for A5 event as 
 *                 part of measurement to be sent to RRC
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_for_event_A5(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_common_params_for_eutra_t *p_rrm_common_params_for_eutra,
        rrm_trigger_quantity_et        trigger_quantity)
{
    RRM_UT_TRACE_ENTER();

    report_config_eutra_trigger_type_event_t *p_event            = RRM_PNULL;
    //RRM_MEMSET(p_report_config_to_add_mod, 0 , sizeof (report_config_to_add_mod_t)); JVR COMMENT


    p_event = &( p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.event);

    p_report_config_to_add_mod->report_config.
        bitmask  |= REPORT_CONFIG_EUTRA_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        trigger_type.bitmask  |= REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT;

    p_event->event_id.bitmask |=
        REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT;

    if (trigger_quantity == RRM_RRC_RSRP )
    {
        p_event->event_id.event_a5.a5_threshold1.bitmask = \
                                                           THRESHOLD_EUTRA_RSRP_PRESENT;

        p_event->event_id.event_a5.a5_threshold2.bitmask = \
                                                           THRESHOLD_EUTRA_RSRP_PRESENT;

        p_event->event_id.event_a5.a5_threshold1.threshold_rsrp = p_rrm_common_params_for_eutra->a5_threshold_1rsrp;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRP;

        p_event->event_id.event_a5.a5_threshold2.
            threshold_rsrp       = p_rrm_common_params_for_eutra->a5_threshold_2rsrp;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                "A5 Report is configured for RSRP Th_1[%d] Th_2[%d]",
                p_event->event_id.event_a5.a5_threshold1.threshold_rsrp,
                p_event->event_id.event_a5.a5_threshold2.threshold_rsrp);
    }
    else if (trigger_quantity == RRM_RRC_RSRQ )
    {
        p_event->event_id.event_a5.a5_threshold1.bitmask =\
                                                          THRESHOLD_EUTRA_RSRQ_PRESENT;

        p_event->event_id.event_a5.a5_threshold2.bitmask =\
                                                          THRESHOLD_EUTRA_RSRQ_PRESENT;

        p_event->event_id.event_a5.a5_threshold1.threshold_rsrq =
            p_rrm_common_params_for_eutra->a5_threshold_1rsrq;

        p_event->event_id.event_a5.a5_threshold2.threshold_rsrq =
            p_rrm_common_params_for_eutra->a5_threshold_2rsrq;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRQ;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                "A5 Report is configured for RSRQ Th_1[%d] Th_2[%d]",
                p_event->event_id.event_a5.a5_threshold1.threshold_rsrq,
                p_event->event_id.event_a5.a5_threshold2.threshold_rsrq);
    }
    p_event->hysteresis = p_rrm_common_params_for_eutra->hysteresis;

    p_event->time_to_trigger = p_rrm_common_params_for_eutra->time_to_trigger;


    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_quantity = p_rrm_common_params_for_eutra->report_quantity;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        max_report_cells = p_rrm_common_params_for_eutra->max_report_cells;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_interval =  p_rrm_common_params_for_eutra->report_interval;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_amount = p_rrm_common_params_for_eutra->report_amount;

    RRM_UT_TRACE_EXIT();
}

/* eICIC_PHASE_1_2_CHANGES_START */
/******************************************************************************
 *  FUNCTION NAME: rrm_fill_report_config_for_victim
 *  INPUT        : p_rrm_common_params_for_eutra,trigger_quantity,
 p_rrm_ue_context,fill_meas_from_ncl
 *  OUTPUT       : p_report_config_to_add_mod
 *  DESCRIPTION  : Fills the reproting configuration information for A4 event as 
 *                 part of  measurement to be sent to RRC
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
rrm_fill_report_config_for_victim(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_common_params_for_eutra_t *p_rrm_common_params_for_eutra,
        rrm_trigger_quantity_et        trigger_quantity,
        rrm_ue_context_t              *p_rrm_ue_context)
    /*SPR 17777 +-*/
{
    RRM_UT_TRACE_ENTER();

    /* SPR14165start */
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    if (RRM_PNULL != p_cell_ctxt)
    {
        p_report_config_to_add_mod->report_config.
            bitmask  |= REPORT_CONFIG_EUTRA_PRESENT;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.bitmask  |= REPORT_CONFIG_EUTRA_TRIGGER_TYPE_PERIODICAL_PRESENT;

        /* SPR 14247 start */
        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = trigger_quantity;
        /* SPR 14247 end */

        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "meas report for victim is configured for RSRQ");

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.periodical.purpose =  RRM_RRC_REPORT_STRONGEST_CELLS;


        /* SPR14165end */
        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_quantity = RRM_RRC_SAME_AS_TRIGGER_QUANTITY;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            max_report_cells = p_rrm_common_params_for_eutra->max_report_cells;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_interval =  p_rrm_common_params_for_eutra->report_interval;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_amount = p_rrm_common_params_for_eutra->report_amount;
    }

    RRM_UT_TRACE_EXIT();
}
/* eICIC_PHASE_1_2_CHANGES_END */
/******************************************************************************
 *  FUNCTION NAME: fill_report_config_for_event_A4
 *  INPUT        : p_rrm_common_params_for_eutra,trigger_quantity,p_sl_ho_param
 p_rrm_ue_context,fill_meas_from_ncl
 *  OUTPUT       : p_report_config_to_add_mod
 *  DESCRIPTION  : Fills the reproting configuration information for A4 event as 
 *                 part of  measurement to be sent to RRC
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_for_event_A4(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_common_params_for_eutra_t *p_rrm_common_params_for_eutra,
        rrm_trigger_quantity_et        trigger_quantity,
        rrm_block_cell_params_ho_t    *p_sl_ho_param,
        rrm_ue_context_t              *p_rrm_ue_context,
        rrm_bool_et 		   fill_meas_from_ncl)
{
    RRM_UT_TRACE_ENTER();

    report_config_eutra_trigger_type_event_t *p_event            = RRM_PNULL;
    //RRM_MEMSET(p_report_config_to_add_mod, 0 , sizeof (report_config_to_add_mod_t)); JVR COMMENT

    /* Soft Lock start - Global variable removal */
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    if (RRM_PNULL != p_cell_ctxt)
    {

        p_event = &( p_report_config_to_add_mod->report_config.report_config_eutra.
                trigger_type.event);

        p_report_config_to_add_mod->report_config.
            bitmask  |= REPORT_CONFIG_EUTRA_PRESENT;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.bitmask  |= REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT;

        p_event->event_id.bitmask |=
            REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A4_PRESENT;

        if (trigger_quantity == RRM_RRC_RSRP )
        {
            p_event->event_id.event_a4.a4_threshold.bitmask = \
                                                              THRESHOLD_EUTRA_RSRP_PRESENT;
            if(RRM_TRUE == fill_meas_from_ncl)
            {
                p_event->event_id.event_a4.a4_threshold.threshold_rsrp = \
                                                                         p_rrm_common_params_for_eutra->a4_threshold_rsrp;
            }
            else if (p_cell_ctxt->administrative_procedure_ongoing)
            {
                p_event->event_id.event_a4.a4_threshold.threshold_rsrp = \
                                                                         p_sl_ho_param->a4_threshold_rsrp_for_block;
            }
            else
            {
                p_event->event_id.event_a4.a4_threshold.threshold_rsrp = \
                                                                         p_rrm_common_params_for_eutra->a4_threshold_rsrp;
            }

            p_report_config_to_add_mod->report_config.report_config_eutra.\
                trigger_quantity = RRM_RRC_RSRP;

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "A4 Report is configured for RSRP Threshold [%d]", 
                    p_event->event_id.event_a4.a4_threshold.threshold_rsrp);
        }
        /* For limiting UE measurement */
        else if (trigger_quantity == RRM_RRC_RSRQ )
        {
            p_event->event_id.event_a4.a4_threshold.bitmask =\
                                                             THRESHOLD_EUTRA_RSRQ_PRESENT;
            if(RRM_TRUE == fill_meas_from_ncl)
            {
                p_event->event_id.event_a4.a4_threshold.threshold_rsrq =
                    p_rrm_common_params_for_eutra->a4_threshold_rsrq;
            }
            else if (p_cell_ctxt->administrative_procedure_ongoing)
            {
                p_event->event_id.event_a4.a4_threshold.threshold_rsrq =
                    p_sl_ho_param->a4_threshold_rsrq_for_block;
            }
            else
            {
                p_event->event_id.event_a4.a4_threshold.threshold_rsrq =
                    p_rrm_common_params_for_eutra->a4_threshold_rsrq;
            }

            p_report_config_to_add_mod->report_config.report_config_eutra.
                trigger_quantity = RRM_RRC_RSRQ;

            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                    "A4 Report is configured for RSRQ Threshold [%d]", 
                    p_event->event_id.event_a4.a4_threshold.threshold_rsrq);
        }
        /* For limiting UE measurement end */


        p_event->hysteresis = p_rrm_common_params_for_eutra->hysteresis;

        p_event->time_to_trigger = p_rrm_common_params_for_eutra->time_to_trigger;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_quantity = p_rrm_common_params_for_eutra->report_quantity;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            max_report_cells = p_rrm_common_params_for_eutra->max_report_cells;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_interval =  p_rrm_common_params_for_eutra->report_interval;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_amount = p_rrm_common_params_for_eutra->report_amount;
    }

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: fill_report_config_for_event_A3
 *  INPUT        : p_rrm_common_params_for_eutra,trigger_quantity
 *  OUTPUT       : p_report_config_to_add_mod
 *  DESCRIPTION  : Fills the reproting configuration information for A3 event as 
 *                 part of measurement to be sent to RRC 
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_for_event_A3(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_common_params_for_eutra_t *p_rrm_common_params_for_eutra,
        rrm_trigger_quantity_et        trigger_quantity)
{
    RRM_UT_TRACE_ENTER();

    report_config_eutra_trigger_type_event_t *p_event            = RRM_PNULL;
    //RRM_MEMSET(p_report_config_to_add_mod, 0 , sizeof (report_config_to_add_mod_t)); JVR COMMENT


    p_event = &( p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.event);

    p_report_config_to_add_mod->report_config.
        bitmask  |= REPORT_CONFIG_EUTRA_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        trigger_type.bitmask  |= REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT;

    p_event->event_id.bitmask |=
        REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A3_PRESENT;

    p_event->event_id.event_a3.a3_offset=
        p_rrm_common_params_for_eutra->a3_offset;

    p_event->event_id.event_a3.report_on_leave=
        p_rrm_common_params_for_eutra->report_on_leave;

    p_event->hysteresis = p_rrm_common_params_for_eutra->hysteresis;

    p_event->time_to_trigger = p_rrm_common_params_for_eutra->time_to_trigger;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        trigger_quantity = trigger_quantity;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_quantity = p_rrm_common_params_for_eutra->report_quantity;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        max_report_cells = p_rrm_common_params_for_eutra->max_report_cells;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_interval =  p_rrm_common_params_for_eutra->report_interval;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_amount = p_rrm_common_params_for_eutra->report_amount;

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: fill_report_config_for_event_A2
 *  INPUT        : p_rrm_common_params_for_eutra,trigger_quantity
 *  OUTPUT       : p_report_config_to_add_mod
 *  DESCRIPTION  : Fills the reproting configuration information for A2 event 
 *                 as part of measurement to be sent to RRC
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_for_event_A2(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_common_params_for_eutra_t *p_rrm_common_params_for_eutra,
        rrm_trigger_quantity_et        trigger_quantity)
{
    RRM_UT_TRACE_ENTER();

    report_config_eutra_trigger_type_event_t *p_event            = RRM_PNULL;
    //    RRM_MEMSET(p_report_config_to_add_mod, 0 , sizeof (report_config_to_add_mod_t)); JVR COMMENT


    p_event = &( p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.event);

    p_report_config_to_add_mod->report_config.
        bitmask  = REPORT_CONFIG_EUTRA_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        trigger_type.bitmask  = REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT;

    p_event->event_id.bitmask =
        REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A2_PRESENT;
    if (trigger_quantity == RRM_RRC_RSRP )
    {
        p_event->event_id.event_a2.a2_threshold.bitmask = \
                                                          THRESHOLD_EUTRA_RSRP_PRESENT;

        p_event->event_id.event_a2.a2_threshold.threshold_rsrp =
            p_rrm_common_params_for_eutra->a2_threshold_rsrp;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRP;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "A2 Report is configured for RSRP");
    }
    /* For limiting UE measurement */
    else if (trigger_quantity == RRM_RRC_RSRQ )
    {
        p_event->event_id.event_a2.a2_threshold.bitmask =\
                                                         THRESHOLD_EUTRA_RSRQ_PRESENT;

        p_event->event_id.event_a2.a2_threshold.threshold_rsrq =
            p_rrm_common_params_for_eutra->a2_threshold_rsrq;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRQ;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "A2 Report is configured for RSRQ");
    }
    /* For limiting UE measurement end */

    p_event->hysteresis = p_rrm_common_params_for_eutra->hysteresis;

    p_event->time_to_trigger = p_rrm_common_params_for_eutra->time_to_trigger;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_quantity = p_rrm_common_params_for_eutra->report_quantity;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        max_report_cells = p_rrm_common_params_for_eutra->max_report_cells;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_interval =  p_rrm_common_params_for_eutra->report_interval;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_amount = p_rrm_common_params_for_eutra->report_amount;

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: fill_report_config_for_event_A1
 *  INPUT        : p_rrm_common_params_for_eutra,trigger_quantity
 *  OUTPUT       : p_report_config_to_add_mod
 *  DESCRIPTION  : Fills the reproting configuration information for A1 event as
 *                 part of measurement to be sent to RRC
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_for_event_A1(
        report_config_to_add_mod_t    *p_report_config_to_add_mod,
        rrm_common_params_for_eutra_t *p_rrm_common_params_for_eutra,
        rrm_trigger_quantity_et        trigger_quantity)
{
    RRM_UT_TRACE_ENTER();
    //    RRM_MEMSET(p_report_config_to_add_mod, 0 , sizeof (report_config_to_add_mod_t)); JVR COMMENT
    // ran_t                         *p_ran_info                    = RRM_PNULL; 
    report_config_eutra_trigger_type_event_t *p_event            = RRM_PNULL;


    p_event = &( p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_type.event);

    p_report_config_to_add_mod->report_config.
        bitmask  = REPORT_CONFIG_EUTRA_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        trigger_type.bitmask  = REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT;

    p_event->event_id.bitmask =
        REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A1_PRESENT;

    if (trigger_quantity == RRM_RRC_RSRP )
    {
        p_event->event_id.event_a1.a1_threshold.bitmask = \
                                                          THRESHOLD_EUTRA_RSRP_PRESENT;

        p_event->event_id.event_a1.a1_threshold.threshold_rsrp =
            p_rrm_common_params_for_eutra->a1_threshold_rsrp;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRP;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "A1 Report is configured for RSRP");
    }
    /* For limiting UE measurement */
    else if (trigger_quantity == RRM_RRC_RSRQ )
    {
        p_event->event_id.event_a1.a1_threshold.bitmask =\
                                                         THRESHOLD_EUTRA_RSRQ_PRESENT;

        p_event->event_id.event_a1.a1_threshold.threshold_rsrq =
            p_rrm_common_params_for_eutra->a1_threshold_rsrq;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRQ;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "A1 Report is configured for RSRQ");
    }
    /* For limiting UE measurement end */

    p_event->hysteresis = p_rrm_common_params_for_eutra->hysteresis;

    p_event->time_to_trigger = p_rrm_common_params_for_eutra->time_to_trigger;


    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_quantity = p_rrm_common_params_for_eutra->report_quantity;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        max_report_cells = p_rrm_common_params_for_eutra->max_report_cells;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_interval =  p_rrm_common_params_for_eutra->report_interval;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_amount = p_rrm_common_params_for_eutra->report_amount;

    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_2432_start*/
/******************************************************************************
 *  FUNCTION NAME: fill_report_config_info_none
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran reporting configuration in reporting config
 *                 list with values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_info_none(rrm_meas_config_t                *p_meas_config,
        rrm_ue_context_t                 *p_rrm_ue_context,
        rrm_common_params_for_eutra_t    *p_common_params_for_eutra,
        U8                               intra_freq_neighbour_count)
{
    U8                      report_config_id = RRM_ZERO;
    rrm_trigger_quantity_et trigger_quantity = RRM_RRC_RSRP;
    U8                      index            = RRM_ZERO;
    U8 delayed_a3_enabled = RRM_ZERO;

    RRM_UT_TRACE_ENTER();

    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);
    delayed_a3_enabled  = get_delayed_a3_configure(p_rrm_ue_context->cell_index);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
            report_config_id = report_config_id;
        //configuring only RSRP for limiting UE measurement            
        fill_report_config_for_event_A1(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[index]), p_common_params_for_eutra, RRM_RRC_RSRP);

        index++;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }

    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <=  MAX_MEAS_OBJECT_ID)
    {
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
            report_config_id = report_config_id;
        /*configuring only RSRP for limiting UE measurement*/
        fill_report_config_for_event_A2(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[index]), p_common_params_for_eutra, RRM_RRC_RSRP);
        index++;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }

    /* If the operator configured trigger quantity for A1 and A2 report as RSRQ */
    /* For Limiting UE measurement we won't configure RSRQ */
    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
            report_config_id = report_config_id;
        fill_report_config_for_event_A1(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[index]), p_common_params_for_eutra, RRM_RRC_RSRQ);

        index++;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }

    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
            report_config_id = report_config_id;
        fill_report_config_for_event_A2(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[index]), p_common_params_for_eutra, RRM_RRC_RSRQ);
        index++;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    /* For Limiting UE measurement end*/
    /* SPR_14564_fix: start */
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Intra Freq Neighbour Count = %d", intra_freq_neighbour_count);
    /*A3 FIX Start*/
    if (delayed_a3_enabled == RRM_ZERO) 
        /*A3 FIX End*/
    {
        /* SPR_14564_fix: end */
        report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

        if(report_config_id <= MAX_MEAS_OBJECT_ID)
            trigger_quantity = get_a3_a4_trigger_quantity (p_rrm_ue_context->cell_index);
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
            report_config_id = report_config_id;
        fill_report_config_for_event_A3(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[index]), p_common_params_for_eutra, trigger_quantity);
        p_rrm_ue_context->is_a3_configured = RRM_ONE;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    RRM_UT_TRACE_EXIT();
}
/* eICIC_PHASE_1_2_CHANGES_START */
/******************************************************************************
 *  FUNCTION NAME: rrm_fill_report_config_info_for_victim 
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran reporting configuration in reporting config
 *                 list with values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t
rrm_fill_report_config_info_for_victim(rrm_meas_config_t               *p_meas_config,
        rrm_ue_context_t                *p_rrm_ue_context,
        /*SPR 17777 +-*/
        rrm_common_params_for_eutra_t    *p_common_params_for_eutra,
        U8                    *p_index) 
{
    U8 report_config_id = RRM_ZERO;

    /* SPR 14247 start */
    /* bug_13865_start */     
    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        if(rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_SIXTEEN))
        {
            p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[(*p_index)].
                report_config_id = report_config_id;

            p_rrm_ue_context->rrm_ue_eicic_info.meas_report_id = report_config_id;

            /* configuring RSRQ for UE measurement */
            rrm_fill_report_config_for_victim(&(p_meas_config->report_config_to_add_mod_list.
                        report_config_to_add_mod[(*p_index)]), p_common_params_for_eutra,RRM_RRC_RSRP,
                    p_rrm_ue_context);/*SPR 17777 +-*/
            (*p_index)++;
            p_meas_config->report_config_to_add_mod_list.count++;
        }
        /* SPR 14247 end */
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    " report config is not configured for RSRP " 
                    " due to feature group indicator is not supporting ");
            rrm_uem_deallocate_used_id(
                    p_rrm_ue_context,
                    REP_CONFIG,
                    report_config_id);
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }
}
/* eICIC_PHASE_1_2_CHANGES_END */
/******************************************************************************
 *  FUNCTION NAME: fill_report_config_info_event_a4
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran reporting configuration in reporting config
 *                 list with values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_info_event_a4(rrm_meas_config_t               *p_meas_config,
        rrm_ue_context_t                *p_rrm_ue_context,
        rrm_cell_context_t    *p_cell_ctxt,
        rrm_common_params_for_eutra_t    *p_common_params_for_eutra,
        U8                    *index) 
{
    U8 report_config_id = RRM_ZERO;
    rrm_trigger_quantity_et trigger_quantity = RRM_RRC_RSRP;

    rrm_block_cell_params_ho_t *p_sl_ho_param = RRM_PNULL;

    p_sl_ho_param = rrm_cellm_get_block_cell_ho_params (p_rrm_ue_context->cell_index);
    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        if (p_cell_ctxt->administrative_procedure_ongoing) /*For SL HO trigger_quantity is taken as RSRP*/
        {
            trigger_quantity = p_sl_ho_param->trigger_quantity_for_block;
        }
        else /* For Normal HO */
        {
            trigger_quantity = get_a3_a4_trigger_quantity (p_rrm_ue_context->cell_index);
        }
        /* SPR 6133 START */
        /*Start:Bug 762*/
        if(rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_FOURTEEN))
            /*End:Bug 762*/
            /* SPR 6133 END */
        {
            p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[(*index)].
                report_config_id = report_config_id;
            //configuring only RSRP for limiting UE measurement
            fill_report_config_for_event_A4(&(p_meas_config->report_config_to_add_mod_list.
                        report_config_to_add_mod[(*index)]), p_common_params_for_eutra,trigger_quantity,
                    p_sl_ho_param, p_rrm_ue_context, RRM_FALSE);
            (*index)++;
            p_meas_config->report_config_to_add_mod_list.count++;
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "A4 report config is not configured " 
                    "due to feature group indicator is not supporting");
            rrm_uem_deallocate_used_id(
                    p_rrm_ue_context,
                    REP_CONFIG,
                    report_config_id);
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }
}
/******************************************************************************
 *  FUNCTION NAME: rrm_fill_blr_inter_rat_ho
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran reporting configuration in reporting config
 *                 list with values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t
rrm_fill_blr_inter_rat_ho(rrm_meas_config_t     *p_meas_config,
        rrm_ue_context_t      *p_rrm_ue_context,
        U8                    inter_freq_neighbour_count,
        rrm_common_params_for_eutra_t *p_common_params_for_eutra,
        U8                    *index ,
        /* SPR 15261 start - last parameter added*/
        rrm_bool_et           a3_report_flag)
/* SPR 15261 end */
{
    U8 counter                                =  RRM_ZERO;
    U8 meas_object_starting_index             = RRM_ZERO;
    U8 meas_obj_count                         =  RRM_ZERO;
    rrm_trigger_quantity_et trigger_quantity  = RRM_RRC_RSRP;
    U8 eutra_thresh_rsrx                      = RRM_ZERO;
    U8 report_config_id                       = RRM_ZERO;
    U8 eutra_inter_freq_rep_id_flag           = RRM_ZERO;
    U8 utra_inter_rat_freq_rep_id_flag        = RRM_ZERO;
    U8 geran_inter_rat_freq_rep_id_flag       = RRM_ZERO;
    U8 cdma2000_inter_rat_freq_rep_id_flag    = RRM_ZERO;
    U8      geran_trigger_quantity            = RRM_TWO;
    U8      cdma2000_trigger_quantity         = RRM_THREE;
    rf_params_t         *p_rf_params          = RRM_PNULL;
    U8 utran_neighbour_count                  = RRM_ZERO;
    U8 geran_neighbour_count                  = RRM_ZERO;
    U8 cdma2000_neighbour_count               = RRM_ZERO;
    ncl_params_t               *p_ncl_params  = RRM_PNULL;

    p_ncl_params               = rrm_cellm_get_ncl_info(p_rrm_ue_context->cell_index);
    geran_neighbour_count       = p_ncl_params->inter_rat_ncl.num_valid_geran_cell;
    cdma2000_neighbour_count    = p_ncl_params->inter_rat_ncl.num_valid_cdma2000_freq_cells;
    utran_neighbour_count                     = p_ncl_params->inter_rat_ncl.num_valid_utran_freq_cell;
    quantity_config_utra_meas_fdd_et  utran_trigger_quantity = RRM_RRC_CPICH_RSCP;
    rrm_irat_params_t             *p_rrm_irat_params         = RRM_PNULL;

    /* SPR 15261 start */
    U32             feature_group_indicators = (U32)-RRM_ONE;
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_context->cell_index);
    /*CID:88452 Fix Start*/
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "p_cell_ctxt is NULL for cell_index[%d]", p_rrm_ue_context->cell_index);
        return;
    }
    /*CID:88452 Fix End*/
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");

        feature_group_indicators = p_rrm_ue_context->ue_capability_params.
            rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.feature_group_indicators;
    }
    /* SPR 15261 end */

    meas_object_to_add_mod_list_t 
        *p_meas_object_to_add_mod_list = RRM_PNULL;

    p_meas_object_to_add_mod_list = &(p_meas_config->meas_object_to_add_mod_list);

    meas_obj_count = p_meas_object_to_add_mod_list->count;

    p_rf_params = rrm_cellm_get_rf_params(p_rrm_ue_context->cell_index);

    p_rrm_irat_params         =  rrm_cellm_get_connected_mode_irat_params\
                                 (p_rrm_ue_context->cell_index);

/* SPR 21464 Changes Start */
    for( counter = meas_object_starting_index; counter < meas_obj_count; counter++ )
/* SPR 21464 Changes End */
    {
        if((p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                    meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT)           &&
                (p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].meas_object.meas_object_eutra.carrier_freq !=
                 p_rf_params->rf_configurations.dl_earfcn))
        {
            /* End :SPR 8883 */
            /* SPR_14564_fix: start */

            /* SPR 15261 start */
            if ((feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_THIRTEEN))))
            {
                /* SPR_14564_fix: end */
                if( eutra_inter_freq_rep_id_flag == RRM_ZERO)
                {
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {
                        if(feature_group_indicators &
                                (RRM_ONE <<(RRM_THIRTY_TWO - RRM_FOURTEEN)))
                        {
                            if(!p_cell_ctxt->administrative_procedure_ongoing)
                            {

                                trigger_quantity = get_a5_trigger_quantity (
                                        p_rrm_ue_context->cell_index);

                                p_meas_config->report_config_to_add_mod_list.\
                                    report_config_to_add_mod[(*index)].\
                                    report_config_id = report_config_id;
                                //configuring only RSRP for limiting UE measurement
                                fill_report_config_for_event_A5(
                                        &(p_meas_config->report_config_to_add_mod_list.
                                            report_config_to_add_mod[(*index)]),
                                        p_common_params_for_eutra,trigger_quantity);
                                (*index)++;
                                p_meas_config->report_config_to_add_mod_list.count++;
                                eutra_inter_freq_rep_id_flag = RRM_ONE;
                            }
                        }
                        else
                        {
                            if(RRM_FALSE == a3_report_flag)
                            {
                                trigger_quantity = get_a3_a4_trigger_quantity (p_rrm_ue_context->cell_index);
                                p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[(*index)].
                                    report_config_id = report_config_id;
                                fill_report_config_for_event_A3(&(p_meas_config->report_config_to_add_mod_list.
                                            report_config_to_add_mod[(*index)]), p_common_params_for_eutra,trigger_quantity);
                                index++;
                                p_meas_config->report_config_to_add_mod_list.count++;
                                eutra_inter_freq_rep_id_flag = RRM_ONE;
                            }
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Not configued A5 meas event as no inter frequency neighbours are present"
                        "Or feature group indicator is not supporting inter freq EUTRAN");
            }
        }
        /* SPR 15261 end */
        else  if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_UTRA_PRESENT)

        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    " MEAS_OBJECT_TO_ADD_UTRA_PRESENT" );	
            if ((utran_neighbour_count >= RRM_ONE) &&
                    /*Start:Bug 762*/ 
                    (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_TWO))
                    && (rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_EIGHT)))  
                /*End:Bug 762*/
                //for testing  
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "UTRAN present and utran_neighbour_count %d",utran_neighbour_count );	         

                /*coverity 23138:typecasted to quantity_config_utra_meas_fdd_et: simerjit, rel 1.3*/
                utran_trigger_quantity =(quantity_config_utra_meas_fdd_et) get_b2_utra_trigger_quantity (p_rrm_ue_context->cell_index);
                trigger_quantity = get_b2_eutra_trigger_quantity (p_rrm_ue_context->cell_index);
                eutra_thresh_rsrx = get_b2_event_eutran_thresh_rsrx (p_rrm_ue_context->cell_index);

                if (utra_inter_rat_freq_rep_id_flag == RRM_ONE)
                {
                    continue;
                }
                else if ( utra_inter_rat_freq_rep_id_flag  == RRM_ZERO) 
                {   
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            " utra_inter_rat_freq_rep_id_flag present " );
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {
                        /* Trigger quanitity for B2 EUTRA and UTRA */

                        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[(*index)].
                            report_config_id = report_config_id;
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                " MAX_MEAS_OBJECT_ID present for UTRAN " );
                        /* Start fix for bug 715 */
                        /* SPR_14564_fix: start */
                        if((p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                                    meas_object.meas_object_utra.cells_to_add_mod_list.bitmask & 
                                    MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT))
                        {
                            fill_report_config_for_event_B2(&(p_meas_config->report_config_to_add_mod_list.
                                        report_config_to_add_mod[*index]), p_rrm_irat_params,
                                    trigger_quantity,utran_trigger_quantity,eutra_thresh_rsrx);
                        }
                        else if((p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                                    meas_object.meas_object_utra.cells_to_add_mod_list.bitmask & 
                                    MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT))
                        {
                            fill_report_config_for_event_B2(&(p_meas_config->report_config_to_add_mod_list.
                                        report_config_to_add_mod[*index]), p_rrm_irat_params,
                                    trigger_quantity, RRM_RRC_PCCPCH_RSCP, eutra_thresh_rsrx);
                        }
                        else
                        {
                            /* This code will be hit only when there are no UTRAN neighbors & hence the
                             * report config for Utra will go for Periodical measurement with purpose 
                             * reportStrongestCellsForSON
                             */
                            rrm_fill_report_config_for_StrongestCellsForSON(&(p_meas_config->report_config_to_add_mod_list.
                                        report_config_to_add_mod[*index]));
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "UTRAN FDD/TDD Report Config Id no configured for B2");
                        }
                        /* SPR_14564_fix: end */
                        /* End fix for bug 715 */ 

                        /*Raise Event RRM_EVENT_LOCAL_EVENT_MEAS_CONFIG_B2_UTRA -START */
                        rrm_raise_event_meas_config_b2_utra(/*SPR 17777 +-*/
                                &(p_meas_config->report_config_to_add_mod_list.
                                    report_config_to_add_mod[(*index)]),
                                trigger_quantity,
                                eutra_thresh_rsrx);
                        /*Raise Event RRM_EVENT_LOCAL_EVENT_MEAS_CONFIG_B2_UTRA -END     */

                        utra_inter_rat_freq_rep_id_flag = RRM_ONE;
                        (*index)++;
                        p_meas_config->report_config_to_add_mod_list.count++;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Not configued B2 meas event for UTRAN as no UTRAN neighbours are present"
                        "Or feature group indicator is not supporting UTRAN");
            }
        }
        else  if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_GERAN_PRESENT)
            /* for testing if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
               meas_object.bitmask & MEAS_OBJECT_TO_ADD_GERAN_PRESENT)*/
        {
            /*Start:Bug 762*/
            if (geran_neighbour_count >= RRM_ONE && rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_THREE))
                /*End:Bug 762*/

            {

                if ( geran_inter_rat_freq_rep_id_flag == RRM_ONE)
                {
                    continue;
                }
                else if ( geran_inter_rat_freq_rep_id_flag  == RRM_ZERO)
                {
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {
                        /* Trigger quanitity for B2 EUTRA */
                        trigger_quantity = get_b2_eutra_trigger_quantity (p_rrm_ue_context->cell_index);
                        eutra_thresh_rsrx = get_b2_event_eutran_thresh_rsrx (p_rrm_ue_context->cell_index);

                        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[(*index)].
                            report_config_id = report_config_id;
                        fill_report_config_for_event_B2(&(p_meas_config->report_config_to_add_mod_list.
                                    report_config_to_add_mod[(*index)]), p_rrm_irat_params,
                                trigger_quantity,geran_trigger_quantity,eutra_thresh_rsrx);

                        /*Raise Event RRM_EVENT_LOCAL_EVENT_MEAS_CONFIG_B2_GERAN -START     */
                        rrm_raise_event_meas_config_b2_geran(/*SPR 17777 +-*/
                                &(p_meas_config->report_config_to_add_mod_list.
                                    report_config_to_add_mod[(*index)]),
                                trigger_quantity,
                                eutra_thresh_rsrx); 
                        /*Raise Event RRM_EVENT_LOCAL_EVENT_MEAS_CONFIG_B2_GERAN -END     */
                        geran_inter_rat_freq_rep_id_flag = 1;
                        (*index)++;
                        p_meas_config->report_config_to_add_mod_list.count++;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Not configued B2 meas event for GERAN as no GERAN frequency neighbours are present"
                        "Or feature group indicator is not supporting GERAN");
            }
        }
        /* BLR Inter RAT Hamdover changes end */
        /* For CDMA2000 START */
        else  if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                meas_object.bitmask & MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT)
        {
            if (cdma2000_neighbour_count >= RRM_ONE &&
                    /*Start:Bug 762*/
                    (( rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_FOUR) ||
                       ( rrm_is_set_fgi_bit(p_rrm_ue_context,RRM_TWENTY_SIX)))))
                /*End:Bug 762*/

            {
                if (cdma2000_inter_rat_freq_rep_id_flag == RRM_ONE)
                {
                    continue;
                }
                else if ( cdma2000_inter_rat_freq_rep_id_flag  == RRM_ZERO)
                {
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {
                        /* Trigger quanitity for B2 EUTRA */
                        trigger_quantity = get_b2_eutra_trigger_quantity (p_rrm_ue_context->cell_index);
                        eutra_thresh_rsrx = get_b2_event_eutran_thresh_rsrx (p_rrm_ue_context->cell_index);

                        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[(*index)].
                            report_config_id = report_config_id;

                        fill_report_config_for_event_B2(&(p_meas_config->report_config_to_add_mod_list.
                                    report_config_to_add_mod[(*index)]), p_rrm_irat_params,
                                trigger_quantity,cdma2000_trigger_quantity,eutra_thresh_rsrx);

                        cdma2000_inter_rat_freq_rep_id_flag = RRM_ONE;
                        (*index)++;
                        p_meas_config->report_config_to_add_mod_list.count++;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }
                }
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Not configued B2 meas event for CDMA2000 as no CDMA2000 frequency neighbours are present"
                        "Or feature group indicator is not supporting CDMA2000");
            }
            /* For CDMA2000 END */
        }
        /* Start :SPR 8883 */
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "At meas_object_index =%d, meas_object_id=%d of type =%d is not for INTER, UTRAN, GERAN or CDMA carrier_freq=%d ",
                    counter,
                    p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].meas_object_id,
                    p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].meas_object.bitmask,
                    p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].meas_object.meas_object_eutra.carrier_freq
                    );
        }
        /* End :SPR 8883 */
    }            
}
/*Klockwork_fix_2432_end*/
/******************************************************************************
 *  FUNCTION NAME: fill_report_config_info
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran reporting configuration in reporting config 
 *                 list with values received from OAM to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_report_config_info(
        rrm_meas_config_t                *p_meas_config,
        rrm_ue_context_t                 *p_rrm_ue_context ,
        rrm_cell_index_t                 cell_index
        )
{
    RRM_UT_TRACE_ENTER();
    /*SPR 20098 Fix Start*/
    /*Code Removed*/
    /*SPR 20098 Fix Stop*/
    U8 index = RRM_ZERO;
    U8 report_config_id = RRM_ZERO;
    U8 a4_meas_cfg_enabled = RRM_ZERO;
    U8 intra_freq_neighbour_count = RRM_ZERO;
    U8 inter_freq_neighbour_count = RRM_ZERO;
    rrm_trigger_quantity_et trigger_quantity = RRM_RRC_RSRP;

    rrmc_meas_report_event_et  report_event = MEAS_REP_EVENT_NONE;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;

    ncl_params_t               *p_ncl_params  = RRM_PNULL;
    /* SPR 16406 8SEP start */
    /*Coverity 93539*/
    report_event                = (rrmc_meas_report_event_et)p_rrm_ue_context->meas_rep_event;
    /* SPR 16406 8SEP end */
    p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                (cell_index);
    p_ncl_params               = rrm_cellm_get_ncl_info(cell_index);

    /* Soft Lock start - Global variable removal */
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(cell_index);
    /* Soft Lock end - Global variable removal */

    intra_freq_neighbour_count =  p_ncl_params->lte_ncl.num_valid_intra_freq_cell;
    inter_freq_neighbour_count =  p_ncl_params->lte_ncl.num_valid_inter_freq_cell;
    //default_a3_event_enabled = 1;// p_rrm_ue_context->ho_params.default_a3_event_enabled;
    a4_meas_cfg_enabled = get_a4_meas_cfg_enabled(cell_index);  //new

    /* SPR 15261 start */
    rrm_bool_et a3_report_flag = RRM_FALSE;
    /* SPR 15261 end */

    /* BLR Inter rat changes start */
    meas_object_to_add_mod_list_t
        *p_meas_object_to_add_mod_list = RRM_PNULL; //new

    U8 meas_obj_count                         =  RRM_ZERO;

    p_meas_object_to_add_mod_list = &(p_meas_config->meas_object_to_add_mod_list);  //new

    meas_obj_count = p_meas_object_to_add_mod_list->count;   //new


    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");
    }

    if (RRM_PNULL != p_cell_ctxt)
    {
        /* BLR Inter RAT changes end */

        if  ((report_event == MEAS_REP_EVENT_NONE) &&
                /* Start fix for SPR 8391 */
                (p_rrm_ue_context->proximity_ind == RRM_FALSE))
            /* End fix for SPR 8391 */
        {
            /*Klockwork_fix_2432_start*/
            fill_report_config_info_none(p_meas_config ,p_rrm_ue_context ,
                    p_common_params_for_eutra ,intra_freq_neighbour_count);
            /*Klockwork_fix_2432_end*/
        }
        /* SPR 13445 Fix Start */
        else if((report_event == MEAS_REP_EVENT_A2) ||
                (report_event == MEAS_REP_EVENT_FORCEFUL_A2) ||
                /* Start fix for SPR 8391 */
                (p_rrm_ue_context->proximity_ind == RRM_TRUE))
            /* End fix for SPR 8391 */
            /* SPR 13445 Fix End */
        {
            /* SPR_14564_fix: start */
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Intra Freq Neighbour Count = %d", intra_freq_neighbour_count);

            if (p_rrm_ue_context->is_a3_configured != RRM_ONE && !p_cell_ctxt->administrative_procedure_ongoing)
                /* SPR_14564_fix: end */
            {
                report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

                if(report_config_id <= MAX_MEAS_OBJECT_ID)
                {
                    trigger_quantity = get_a3_a4_trigger_quantity (p_rrm_ue_context->cell_index);
                    p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
                        report_config_id = report_config_id;
                    fill_report_config_for_event_A3(&(p_meas_config->report_config_to_add_mod_list.
                                report_config_to_add_mod[index]),p_common_params_for_eutra,trigger_quantity);
                    //p_rrm_ue_context->is_a3_configured = 1;
                    /* SPR 15261 start */
                    a3_report_flag = RRM_TRUE;
                    /* SPR 15261 end */
                    index++;
                    p_meas_config->report_config_to_add_mod_list.count++;
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                            "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                }
            }
            /* Fix for Event A4 for administrative process ongoing */  
            /* Start :SPR 8720 */
            /* SPR_14564_fix: start */

            /*SPR 20098 Fix Start*/
            /*Code Removed*/
            /*SPR 20098 Fix Stop*/

            /* PR 1002 START */
            /*SPR 20098 Fix Start*/
            if  (p_cell_ctxt->administrative_procedure_ongoing ?( RRM_ONE) : ( a4_meas_cfg_enabled )) 
                /*Condition removed*/
                /*SPR 20098 Fix Stop*/
                /* PR 1002 END */
            {
                /* SPR_14564_fix: start */
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "a4_meas_cfg_enabled == 1 check %u",a4_meas_cfg_enabled);		
                /* SPR_14564_fix: end */
                /*Klockwork_fix_2432_start*/
                fill_report_config_info_event_a4(p_meas_config ,p_rrm_ue_context ,
                        p_cell_ctxt ,p_common_params_for_eutra ,&index);

                /*Klockwork_fix_2432_end*/
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "A4 report config event is not configured " 
                        "due to A4 configuration parameter is disabled or"
                        "no intra frequency neighbours are configured"
                        "a4_meas_cfg_enabled=  %d" ,a4_meas_cfg_enabled);
            }

            /* BLR Inter RAT Hamdover changes started */
            /* SPR 15261 start */
            if (meas_obj_count >= RRM_ONE)
                /* SPR 15261 end */
            {
                /* Start :SPR 8883 */
                /*Klockwork_fix_2432_start*/
                /* SPR 15261 start - last parameter added*/
                rrm_fill_blr_inter_rat_ho(p_meas_config ,p_rrm_ue_context ,
                        inter_freq_neighbour_count ,p_common_params_for_eutra ,&index ,a3_report_flag);
                /* SPR 15261 end */
                /*Klockwork_fix_2432_end*/
            }

        }
        /* eICIC_PHASE_1_2_CHANGES_START */
        /*CA HARDENING CHANGES*/
        /* SPR 14247 start */
        if(MEAS_REP_EVENT_PERIODICAL == p_rrm_ue_context->meas_rep_event)
            /* SPR 14247 end */
        {
            rrm_fill_report_config_info_for_victim(p_meas_config ,p_rrm_ue_context ,
                    p_common_params_for_eutra ,&index);/*SPR 17777 +-*/
        }
        /* eICIC_PHASE_1_2_CHANGES_END */
    }
    if (p_meas_config->report_config_to_add_mod_list.count != RRM_ZERO)
    {
        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
    }
    RRM_UT_TRACE_EXIT();
}

/* SPR 15797 Start*/
/******************************************************************************
 *  FUNCTION NAME: fill_eutran_meas_config_of_anr_to_remove_list
 *  INPUT        : p_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with the meas 
 *                 obj configuration created for SON-ANR UE measurements
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_eutran_meas_config_of_anr_to_remove_list(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_ue_context
        )
{
    RRM_UT_TRACE_ENTER();

    U8    meas_object_index   = RRM_ZERO;
    U8    report_config_index   = RRM_ZERO;

    U8    id_index   = RRM_ZERO;
    /*SPR 18194 fix start*/
    U8    idx        = RRM_ZERO;
    /*SPR 18194 fix stop*/
    /* SPR 16053 start */
    /* Code deleted */
    /* SPR 16053 end */
    /* SPR 15598 Start */
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;
    YLNODE*                  p_node             = RRM_PNULL;
    /* SPR 15598 End */
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context Not found");
        RRM_UT_TRACE_EXIT();
        return;
    } 

    for(id_index = RRM_ZERO; id_index < RRM_ANR_MAX_MEAS_OBJ; id_index++)
    {
        /* SPR 18194 fix start */
        /* Code deleted */
        /* SPR 18194 fix stop */
        /* CSR00069389 Start */
        /* To exclude serving freq meas object which is not to be removed */
        if((RRM_ZERO < p_ue_context->p_meas_config[id_index].meas_obj_id) &&
            /* Spr 18488 Changes Start */    
            ((MAX_SON_MEAS_CONFIG_OBJECT + RRM_ONE) >= p_ue_context->p_meas_config[id_index].meas_obj_id))
            /* Spr 18488 Changes End */
        {
            /* SPR 18194 fix start*/
            if(p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn != p_ue_context->p_meas_config[id_index].carr_freq)
            { 
            p_meas_config->meas_object_to_remove_list.meas_object_id[meas_object_index++]
                = p_ue_context->p_meas_config[id_index].meas_obj_id;
            /* SPR 16053 start */
            p_ue_context->p_meas_config[id_index].meas_config_status = RRM_MEAS_CONFIG_TO_DELETE;
            p_ue_context->p_meas_config[id_index].obj_created_at_ue = RRM_FALSE;
            /* SPR 16053 end */
            }
            /*SPR_17942_Fix_Start*/
            for(idx = RRM_ZERO;idx< report_config_index ;idx++)
            {
                if(p_meas_config->report_config_to_remove_list.report_config_id[idx] == p_ue_context->p_meas_config[id_index].report_id)
                {
                    break;
                }
            }
            /*In case the RC is removed and then A2 is reported for in the removal case.*/
            if((idx == report_config_index) &&(p_ue_context->p_meas_config[id_index].report_id) )
            { 
                p_meas_config->report_config_to_remove_list.report_config_id[report_config_index++]
                    = p_ue_context->p_meas_config[id_index].report_id;
            }
            /* SPR 18194 fix stop*/
            /*SPR_17942_Fix_End*/
            /* SPR 16053 start */
            p_ue_context->p_meas_config[id_index].meas_config_status = RRM_MEAS_CONFIG_TO_DELETE;
            p_ue_context->p_meas_config[id_index].obj_created_at_ue = RRM_FALSE;
            /* SPR 16053 end */

        }
        /* CSR00069389 End */
    }

    /* If some objects were created for the UE */
    if(meas_object_index || report_config_index)
    {
        if(meas_object_index != RRM_ZERO )
        {
            p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT;
            p_meas_config->meas_object_to_remove_list.count  = meas_object_index;
        }

        if(report_config_index != RRM_ZERO )
        {
            p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT;
            p_meas_config->report_config_to_remove_list.count  = report_config_index;
        }

        /* SPR 16053 start */
        /* Code deleted */
        /* SPR 16053 end */

        /* SPR 15598 Start */
        /// Change the status of ANR Meas id object to RRM_MEAS_CONFIG_TO_DELETE

        p_node = ylFirst(&(p_ue_context->meas_report_list));

        while (p_node)
        {
            p_meas_report_node = (meas_report_info_node_t*)p_node;

            /// Add Into Remove List if meas Id belongs to ANR range
            if((p_meas_report_node->meas_id >= RRM_MEAS_OBJ_ID_OFFSET) &&
                    (p_meas_report_node->meas_id <= RRM_ANR_MAX_MEAS_OBJ))
            {
                /// Add CGI To Remove List if present
                if((p_meas_report_node->reporting_event == MEAS_REP_EVENT_FOR_CGI) &&
                        (p_meas_report_node->meas_status != RRM_MEAS_CONFIG_TO_DELETE))
                {
                    p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
                }
            }
            p_node = ylNext(p_node);
        }
        /* SPR 15598 Start */

    }

    RRM_UT_TRACE_EXIT();
}
/* SPR 15797 End */
/* SPR_15538_fix: Start */
/******************************************************************************
 *  FUNCTION NAME: rrm_send_meas_config_req_for_cgi_rep_meas_id_removal 
 *  INPUT        : p_meas_config, p_ue_context
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with values 
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/

/* SPR 15860 Start */
/* SPR 15861 Start */
/* SPR 15862 Start */
/* SPR 15698 Start */
rrm_return_et
    rrm_send_meas_config_req_for_cgi_rep_meas_id_removal
(
 rrm_meas_config_t  *p_meas_config,
 rrm_ue_context_t   *p_ue_context
 )
{

    YLNODE                       *p_node                               = RRM_PNULL;
    /*SPR 15787:Start*/
    rrm_return_et                 ret_val                              = RRM_SUCCESS;
    /*SPR 15787:End*/
    U8                            report_config_index                  = RRM_ZERO;
    /* SPR_15755_Fix: Start */
    /* Code Removed */
    /* SPR_15755_Fix: End */
    U8                            i                                    = RRM_ZERO;
    /* SPR_15755_Fix: Start */
    /* Code Removed */
    /* SPR_15755_Fix: End */
    /*SPR 18194 fix start*/
    /* Code deleted */
    /*SPR 18194 fix stop*/
    /*SPR 17877 START*/
    U8    meas_object_id_hash_map[MAX_MEAS_OBJECT_ID] = {RRM_ZERO};;
    U8                            meas_config_object_index             = RRM_ZERO;
    /*SPR 17877 END*/

    meas_report_info_node_t      *p_meas_report_info                   = RRM_PNULL;

    rrm_bool_et is_inter_freq_events_configured = RRM_FALSE;
    /* SPR 16053 start */
    U8                            k                                    = RRM_ZERO;
    /* SPR 16053 end */

    RRM_UT_TRACE_ENTER();
    /*SPR 17877 START*/
    p_meas_report_info = (meas_report_info_node_t*)ylFirst(&(p_ue_context->meas_report_list));
    while (p_meas_report_info)
    {
        meas_object_id_hash_map[p_meas_report_info->meas_object_id]++;
        p_meas_report_info = (meas_report_info_node_t*)ylNext(&p_meas_report_info->sNode);
    }
    /*SPR 17877 END*/

    p_node = ylFirst(&(p_ue_context->meas_report_list));

    while (p_node) 
    { 
        p_meas_report_info = (meas_report_info_node_t*) p_node;
        /* SPR 15608 Start */
        /*SPR 15787:Start*/
        /* SPR 15853 Start */
        if ((p_meas_report_info->reporting_event == MEAS_REP_EVENT_FOR_CGI) &&
                ((p_meas_report_info->is_meas_report_pending == RRM_FALSE)|| 
                 (p_ue_context->stored_meas_id_for_cgi == p_meas_report_info->meas_id)) )
            /* SPR 15853 End */
            /*SPR 15787:End*/
        {
            /* SPR_15755_Fix: Start */
            /* Code Removed */
            /* SPR_15755_Fix: End */
            /*SPR 18194 fix start*/
            /*SPR 17877 START*/
            if( meas_object_id_hash_map[p_meas_report_info->meas_object_id] == RRM_ONE)
            {
                p_meas_config->meas_object_to_remove_list.\
                    meas_object_id[meas_config_object_index]
                    = p_meas_report_info->meas_object_id;
                meas_config_object_index++;

                /// Check whether a Reconfig Id is already added in Remove list
                for(i = RRM_ZERO; i < report_config_index; i++)
                {
                    if((p_meas_config->report_config_to_remove_list.report_config_id[i]) == 
                            (p_meas_report_info->report_config_id))
                    {
                        break;
                    }
                }
                if(i == report_config_index)
                {
                    /*SPR 18139 fix start*/
                    p_meas_config->report_config_to_remove_list.report_config_id[report_config_index]
                        = p_meas_report_info->report_config_id;
                    report_config_index++;
                    /* Spr 18488 Changes Start */    
                } 
                /* Spr 18488 Changes End */
                for (k = RRM_ZERO; k < RRM_ANR_MAX_MEAS_OBJ; k++)
                {
                    if (p_meas_report_info->meas_object_id  == p_ue_context->p_meas_config[k].meas_obj_id)
                    {
                        p_ue_context->p_meas_config[k].meas_config_status = RRM_MEAS_CONFIG_TO_DELETE;
                        p_ue_context->p_meas_config[k].obj_created_at_ue = RRM_FALSE;
                    }
                }
                /* Spr 18488 Changes Start */    
                /* Code Removed */
                /* Spr 18488 Changes End */
                /*SPR 18139 fix stop*/
            }
            else
            {    
                for(i = RRM_ZERO; i < report_config_index; i++)
                {
                    if((p_meas_config->report_config_to_remove_list.report_config_id[i]) == 
                            (p_meas_report_info->report_config_id))
                    {
                        break;
                    }
                }
                if(i == report_config_index)
                {
                    p_meas_config->report_config_to_remove_list.report_config_id[report_config_index]
                        = p_meas_report_info->report_config_id;
                    report_config_index++;
                    /* Spr 18488 Changes Start */    
                }
                /* Spr 18488 Changes End */
                /*SPR 17877 END*/
                /* SPR 16053 start */
                for (k = RRM_ZERO; k < RRM_ANR_MAX_MEAS_OBJ; k++)
                {
                    if (p_meas_report_info->report_config_id == p_ue_context->p_meas_config[k].report_id)
                    {
                        p_ue_context->p_meas_config[k].meas_config_status = RRM_MEAS_CONFIG_TO_DELETE;
                    }
                }
                /* Spr 18488 Changes Start */    
                /* Code Removed */
                /* Spr 18488 Changes End */
                /* SPR 16053 end */
            }
            /*SPR 18194 fix stop*/

            /* SPR 16053 start */
            p_meas_report_info->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
            /* SPR 16053 end */
        }
        /* SPR 15608 End */
        p_node = ylNext(p_node);
    }

    /* SPR_15755_Fix: Start */
    /// We should only release the meas gap if no inter-frequency event is configured
    if (RRM_ZERO != p_ue_context->meas_gap_config.meas_gap_config_type)
    {
        is_inter_freq_events_configured = rrm_is_any_interfreq_interrat_meas_configured(p_ue_context); 
        if(RRM_FALSE == is_inter_freq_events_configured)
        {
            p_meas_config->bitmask |= UE_ADM_MEAS_GAP_CONFIG_PRESENT;
            p_meas_config->meas_gap_config.bitmask |= RRM_ZERO;
            p_meas_config->meas_gap_config.meas_gap_config_type = RRM_ZERO;
            /*SPR_17047_START*/
            p_ue_context->meas_gap_sent_status = RRM_UNSET;
            /*SPR_17047_END*/

            /* SPR 16053 start */
            /* Code removed */
            /* SPR 16053 end */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Meas Gap Configuration can't be released since inter frequency events are configured");
        }
    }
    /* CID 97198 End */
    /* SPR_15755_Fix: End */


    /*SPR 17877 START*/
    if (meas_config_object_index != RRM_ZERO)
    {
        p_meas_config->meas_object_to_remove_list.count = meas_config_object_index;
        p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT;
    }    
    /*SPR 17877 END*/

    if( report_config_index != RRM_ZERO)
    { 
        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT;
        p_meas_config->report_config_to_remove_list.count = report_config_index;
    }
    /* SPR_15755_Fix: Start */
    /* Code Removed */
    /* SPR_15755_Fix: End */

    RRM_UT_TRACE_EXIT();
    return ret_val;

}
/* SPR_15538_fix: End */
/* SPR 15698 End*/
/* SPR 15860 End */
/* SPR 15861 End */
/* SPR 15862 End */
/*Klockwork_fix_start*/
/******************************************************************************
 *  FUNCTION NAME: fill_meas_config_to_remove_list
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with values
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_meas_config_to_remove_list(rrm_cell_context_t          *p_cell_ctxt ,
        rrm_ue_context_t            *p_rrm_ue_ctx ,
        U8                          *meas_object_id_hash_map ,
        meas_report_info_node_t     *p_meas_report_node ,
        rrm_meas_config_t           *p_meas_config ,
        U8                          *meas_object_index ,
        U8                          *report_config_index , 
        U8                          *meas_report_index)
{
    /* Coverity 93960_93964 Fix Start */
    /* SPR 21641 Fix Start */
    U8 count =  RRM_ZERO;
    /* SPR 21641 Fix Stop */
    /* SPR 16041 start */
    /* SPR 18194 fix start */
    /* Code deleted */
    /* SPR 18194 fix stop */
    /* Coverity 93960_93964 Fix End */
    /* SPR 16041 end */
    /* SPR-13400-fix start */
    /* Code Deleted */
    /* SPR-13400-fix end */
    /* SPR 18194 fix start */
    U8 idx = RRM_ZERO;
    /* SPR 18194 fix stop */
    RRM_UT_TRACE_ENTER();
    /* SPR 15728 Start */
    if (p_meas_report_node->reporting_event == MEAS_REP_EVENT_A3)
    {
        if(p_rrm_ue_ctx->is_a3_configured == RRM_ZERO)
        {                
            /*SPR_18174_Fix_Start*/
            if((*report_config_index) < MAX_REPORT_CONFIG_ID )
            {    
                /* Code Deleted */ 
                p_meas_config->report_config_to_remove_list.report_config_id[(*report_config_index)]
                    = p_meas_report_node->report_config_id;
                (*report_config_index) ++;
            }
            /*SPR_18174_Fix_End */

            /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - START*/
            /*For aborting A3,meas_id to be 17*/
            rrm_raise_event_ue_meas_abort(/*SPR 17777 +-*/
                    p_meas_report_node->meas_id);
            /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - END*/

            p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
        }
        if(p_meas_report_node->carrier_earfcn !=
                p_cell_ctxt->ran_info.rf_params.rf_configurations.dl_earfcn)
        {
            if( meas_object_id_hash_map[p_meas_report_node->meas_object_id] == RRM_ONE)
            {
                /* Coverity fix 88251 */
                if ( *(meas_object_index) < MAX_MEAS_OBJECT_ID)
                {
                    /*SPR 17516 Start*/
                    p_meas_config->meas_object_to_remove_list.meas_object_id[(*meas_object_index)++]
                        = p_meas_report_node->meas_object_id;
                    /*SPR 17516 End*/
                    /* SPR 21641 Fix Start */
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,"meas_object_id: %d is added in the list",
                            p_meas_report_node->meas_object_id);
                    /* SPR 21641 Fix Start */
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"meas_object_index is out of range ");
                }
                /* Coverity fix 88251 */
            }
            /* SPR 21641 Fix Start */
            for(count = RRM_ZERO; count < p_meas_config->report_config_to_remove_list.count ; count++ )
            {
                if(p_meas_config->report_config_to_remove_list.report_config_id[count] == p_meas_report_node->report_config_id)
                {
                    break;
                }    
            }
            if (count == p_meas_config->report_config_to_remove_list.count)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "report_config_id:[%d] is added in the list",
                        p_meas_report_node->report_config_id, p_meas_config->report_config_to_remove_list.report_config_id[count]);

                p_meas_config->report_config_to_remove_list.report_config_id[count]
                    = p_meas_report_node->report_config_id;
                (*report_config_index) ++;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "No need to add in the list. report_config_id:[%d] is already present on index :%d",
                        p_meas_report_node->report_config_id,count);
            }
            /* SPR 21641 Fix Stop */
            /* SPR 16041 start */
            /* Code Removed */
            /* SPR 16041 end */
            /* SPR 18194 fix stop */
            p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
        }
    }
    /* SPR 15728 End */

    else if (p_meas_report_node->reporting_event == MEAS_REP_EVENT_A4)
    {
        if(RRM_CARRIER_REDIRECT_ON_UE == p_rrm_ue_ctx->action_triggered)
        {
            if(p_cell_ctxt->ran_info.rf_params.rf_configurations.dl_earfcn == p_meas_report_node->carrier_earfcn)
            {
                p_meas_config->meas_id_to_remove_list.meas_id[(*meas_report_index)++]
                    = p_meas_report_node->meas_id;
                p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
            }
        }
        else
        {
            if(RRM_HO_ACTN_ON_UE != p_rrm_ue_ctx->action_triggered)
            {
                /*SPR 17527 START*/
                p_meas_config->report_config_to_remove_list.report_config_id[*report_config_index]
                    = p_meas_report_node->report_config_id;
                (*report_config_index) ++;
                /*SPR 17527 END*/
                p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
            }
        }

        /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - START*/
        /*For aborting A4,meas_id to be 18*/
        rrm_raise_event_ue_meas_abort(/*SPR 17777 +-*/
                p_meas_report_node->meas_id);
        /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - END*/
    }

    else if (p_meas_report_node->reporting_event == MEAS_REP_EVENT_A5)
    {
        /* Fix start for 973 */
        if( meas_object_id_hash_map[p_meas_report_node->meas_object_id] == RRM_ONE)
        {
            /* Coverity fix 88251 */
            if ( *(meas_object_index) < MAX_MEAS_OBJECT_ID)
            {
                p_meas_config->meas_object_to_remove_list.meas_object_id[(*meas_object_index)]
                    = p_meas_report_node->meas_object_id;
                (*meas_object_index) ++;
                /* SPR 21641 Fix Start */
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,"meas_object_id: %d added in the list",
                        p_meas_report_node->meas_object_id);
                /* SPR 21641 Fix Start */
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"meas_object_index is out of range ");
            }
            /* Coverity fix 88251 */
            /*SPR_18174_Fix_Start */
            /* Spr 18488 Changes Start */
            /* SPR 21641 Fix Start */
            for(count= RRM_ZERO;count < p_meas_config->report_config_to_remove_list.count; count++ )
            {
                if(p_meas_config->report_config_to_remove_list.report_config_id[count] == p_meas_report_node->report_config_id)
                {
                    break;
                }    
            }
            if ((count) == p_meas_config->report_config_to_remove_list.count)
            {
                p_meas_config->report_config_to_remove_list.report_config_id[count]
                    = p_meas_report_node->report_config_id;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        " report_config_id: %d added in the list",
                        p_meas_config->report_config_to_remove_list.report_config_id[count]);

                (*report_config_index)++;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "No need to add in the list. report config id :%d is already present on index :%d",
                        p_meas_report_node->report_config_id,count);
            }
        }
        /*SPR_18174_Fix_End */
        /* SPR 18194 fix start */
        /* SPR 21641 Fix Stop */ 
        /* Code removed */
        /* SPR 18194 fix stop */
        /* Fix end for 973 */

        /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - START*/
        /*For aborting A5,meas_id will be in range 19-32*/
        rrm_raise_event_ue_meas_abort(/*SPR 17777 +-*/
                p_meas_report_node->meas_id);
        /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - END*/


        p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
    }

    /*  Inter RAT : BLR Changes START */
    /* SPR 16041 start */
    /* SPR 16406 8SEP start */
    else if ((p_rrm_ue_ctx->meas_rep_event == MEAS_REP_EVENT_A1) &&
            /* SPR 16406 8SEP end */
            (((p_meas_report_node->reporting_event == MEAS_REP_EVENT_B2) &&
              (p_rrm_ue_ctx->ho_params.bitmask & RRM_UE_B2_MEAS_OBJ_CONFIGURED)) ||
             (p_meas_report_node->reporting_event == MEAS_REP_EVENT_STRONGEST_CELL_FOR_SON)))
        /* SPR 16041 end */
    {

        /* SPR 16041 start */
        if( meas_object_id_hash_map[p_meas_report_node->meas_object_id] == RRM_ONE)
        {
            p_meas_config->meas_object_to_remove_list.meas_object_id[(*meas_object_index)++]
                = p_meas_report_node->meas_object_id;
        }
        /* SPR 18194 fix start */
        for(idx = RRM_ZERO;idx < (*report_config_index);idx++)
        {
            if(p_meas_config->report_config_to_remove_list.report_config_id[idx] == p_meas_report_node->report_config_id)
            {
                break;
            }
        }
        if(idx == (*report_config_index))
        {
            p_meas_config->report_config_to_remove_list.report_config_id[(*report_config_index)]
                = p_meas_report_node->report_config_id;
            (*report_config_index) ++;
        }
        /* Code removed */
        /* SPR 18194 fix stop */

        /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - START*/
        /*For aborting B2*/
        rrm_raise_event_ue_meas_abort(/*SPR 17777 +-*/
                p_meas_report_node->meas_id);
        /*Raise event RRM_EVENT_LOCAL_EVENT_UE_MEAS_ABORT - END*/

        p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
    }
    /* SPR 15598 Start */
    /// Add CGI To Remove List if present
    else if((p_meas_report_node->reporting_event == MEAS_REP_EVENT_FOR_CGI) &&
            (p_meas_report_node->meas_status != RRM_MEAS_CONFIG_TO_DELETE))
    {
        /*SPR 17527 START*/
        if((p_meas_report_node->meas_id >= HO_MEAS_START_ID) &&
                (p_meas_report_node->meas_id <= MAX_MEAS_OBJECT_ID) &&
                /*SPR 16292 Fix Start*/
                (RRM_TRUE != p_meas_report_node->is_ask_cgi_configured_via_intra_a3)&&
                (RRM_TRUE != p_meas_report_node->is_meas_report_pending))
            /*SPR 17527 END*/
            /*SPR 16292 Fix Stop*/
        {
            /*SPR 17527 START*/
            p_meas_config->report_config_to_remove_list.report_config_id[(*report_config_index)++]
                = p_meas_report_node->report_config_id;
            /*SPR 17527 END*/
            p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
        }
    }
    /* SPR 15598 End */
    /*CA HARDENING 6.3.0 HARDENING START*/    
    if(*meas_object_index != RRM_ZERO )
    {
        p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT;
        p_meas_config->meas_object_to_remove_list.count  = *meas_object_index;
    } 
    if( *report_config_index != RRM_ZERO)
    { 
        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT;
        p_meas_config->report_config_to_remove_list.count = *report_config_index;
    }
    if ( *meas_report_index != RRM_ZERO)
    {
        p_meas_config->bitmask |= UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT;
        p_meas_config->meas_id_to_remove_list.count = *meas_report_index;
    }
    /*CA HARDENING 6.30 END*/
    RRM_UT_TRACE_EXIT();
}
/*Klockwork_fix_end*/
/******************************************************************************
 *  FUNCTION NAME: fill_eutran_meas_config_to_remove_list
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with values 
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
fill_eutran_meas_config_to_remove_list(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx
        )
{
    RRM_UT_TRACE_ENTER();

    U8    meas_object_index   = RRM_ZERO;
    U8    report_config_index = RRM_ZERO;
    U8    meas_report_index   = RRM_ZERO;
    U8    meas_object_id_hash_map[MAX_MEAS_OBJECT_ID] = {RRM_ZERO};;
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;

    /* Fix start for 973 */
    rrm_cell_context_t    *p_cell_ctxt = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_ctx->cell_index);
    /*CID 65828:start*/
    if (RRM_PNULL != p_cell_ctxt)
    {
        YLNODE *p_node = RRM_PNULL;

        p_node = ylFirst(&(p_rrm_ue_ctx->meas_report_list));

        /* Start CSR: 00058590 */
        if(p_meas_config->bitmask & UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT &&
                p_meas_config->meas_object_to_remove_list.count < MAX_MEAS_OBJECT_ID)
        {
            meas_object_index = p_meas_config->meas_object_to_remove_list.count;
        }
        /* End CSR: 00058590 */

        while (p_node)
        {
            p_meas_report_node = (meas_report_info_node_t*)p_node;
            meas_object_id_hash_map[p_meas_report_node->meas_object_id]++;
            p_node = ylNext(p_node);
        }
        /* Fix end for 973 */

        p_node = RRM_PNULL;

        p_node = ylFirst(&(p_rrm_ue_ctx->meas_report_list));

        while (p_node)
        {
            p_meas_report_node = (meas_report_info_node_t*)p_node;
            /*CID:88251 Fix Start*/
            if (meas_object_index == (MAX_MEAS_OBJECT_ID))
            {
                break;
            }
            /*CID:88251 Fix End*/
            /*Klockwork_fix_start*/
            fill_meas_config_to_remove_list(p_cell_ctxt , p_rrm_ue_ctx ,meas_object_id_hash_map ,
                    p_meas_report_node ,p_meas_config ,&meas_object_index ,
                    &report_config_index ,&meas_report_index);
            /*Klockwork_fix_end*/


            /*  Inter RAT : BLR Changes END */
            p_node = ylNext(p_node);
            //ylDelete(&(p_rrm_ue_ctx->meas_report_list),p_node);
        }

        if(meas_object_index != RRM_ZERO )
        {
            p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT;
            p_meas_config->meas_object_to_remove_list.count  = meas_object_index;
        } 
        if( report_config_index != RRM_ZERO)
        { 
            p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT;
            p_meas_config->report_config_to_remove_list.count = report_config_index;
        }
        if ( meas_report_index != RRM_ZERO)
        {
            p_meas_config->bitmask |= UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT;
            p_meas_config->meas_id_to_remove_list.count = meas_report_index;
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctxt is null");
    }
    /*CID 65828:end*/
    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: rrm_ue_fill_meas_remove_list_from_ho_adm_req
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the meas config to remove list from the meas config 
 *                 received as part of HO ADM Req
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
rrm_ue_fill_meas_remove_list_from_ho_adm_req(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx          
        )
{
    RRM_UT_TRACE_ENTER();
    U8    index             = RRM_ZERO;

    /* SPR 21496 Fix Start */ 
    if (RRM_PNULL != p_rrm_ue_ctx->p_meas_config_of_ho_adm)
    /* SPR 21496 Fix End */ 
    {
      for (index = RRM_ZERO; index < p_rrm_ue_ctx->p_meas_config_of_ho_adm->meas_object_to_add_mod_list.\
          count; index ++)
      {
        p_meas_config->meas_object_to_remove_list.\
          meas_object_id[index] = p_rrm_ue_ctx->p_meas_config_of_ho_adm->\
          meas_object_to_add_mod_list.meas_object_to_add_mod[index].meas_object_id;
        p_meas_config->meas_object_to_remove_list.count ++;
        p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT;
      }     
      /* SPR 18194 fix start */
      for (index = 0; index < p_rrm_ue_ctx->p_meas_config_of_ho_adm->report_config_to_add_mod_list.\
          count; index ++)
      {
        p_meas_config->report_config_to_remove_list.report_config_id[index] =
          p_rrm_ue_ctx->p_meas_config_of_ho_adm->report_config_to_add_mod_list.\
          report_config_to_add_mod[index].report_config_id;
        p_meas_config->report_config_to_remove_list.count ++;
        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT;
      }                     
      /* SPR 21496 Fix Start */ 
      /* Code removed */
      /* SPR 21496 Fix End */ 

      RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
          "The bitmask of ho_meas_remove  : %d %d %d",
          p_meas_config->bitmask, UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT,
          p_meas_config->meas_id_to_remove_list.count);
    }

    RRM_UT_TRACE_EXIT();
}
/* BLR Inter RAT B1 configuration start */

/******************************************************************************
 *  FUNCTION NAME : fill_report_config_for_event_B1
 *  INPUT         : p_rrm_irat_params,p_common_params_for_eutra,utran_trigger_quantity
 *  OUTPUT        : p_report_config_to_add_mod
 *  DESCRIPTIONi  : Fills the reproting configuration information for B1 event 
 *                  as part of measurement to be sent to RRC
 *  RETURNS       : None
 ******************************************************************************/
rrm_void_t
fill_report_config_for_event_B1(
        report_config_to_add_mod_t      *p_report_config_to_add_mod,
        rrm_irat_params_t               *p_rrm_irat_params,
        /*SPR 17777 +-*/
        U8                               utran_trigger_quantity
        )
{
    RRM_UT_TRACE_ENTER();

    report_config_interrat_trigger_type_event_t *p_event            = RRM_PNULL;

    p_event = &( p_report_config_to_add_mod->report_config.report_config_interrat.
            trigger_type.event);

    p_report_config_to_add_mod->report_config.
        bitmask  |= REPORT_CONFIG_INTERRAT_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        trigger_type.bitmask  |= REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_PRESENT;

    p_event->event_id.bitmask |=
        REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_PRESENT;

    p_event->hysteresis = p_rrm_irat_params->hysteresis;

    p_event->time_to_trigger = p_rrm_irat_params->time_to_trigger;

    /* SPR:757- Coverity fix start */
    /* Start fix for bug 715 */
    if (utran_trigger_quantity == RRM_RRC_CPICH_RSCP)
        /* (utran_trigger_quantity == RRM_RRC_PCCPCH_RSCP) ) */
        /* SPR:757- Coverity fix end */
    {
        p_event->event_id.event_b1.bitmask  = 
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT;
        p_event->event_id.event_b1.b1_threshold_utra.bitmask = THRESHOLD_UTRA_RSCP_PRESENT;
        p_event->event_id.event_b1.b1_threshold_utra.threshold_rscp = 
            p_rrm_irat_params->b1_threshold_utra_rscp; 
    }
    else if (utran_trigger_quantity == RRM_RRC_CPICH_ECN0)
    {
        p_event->event_id.event_b1.bitmask  =
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT;
        p_event->event_id.event_b1.b1_threshold_utra.bitmask = THRESHOLD_UTRA_ECN0_PRESENT;
        p_event->event_id.event_b1.b1_threshold_utra.threshold_ecn0 =
            p_rrm_irat_params->b1_threshold_utra_ecn0;
    }
    /* End fix for bug 715 */
    else if(utran_trigger_quantity == RRM_TWO)
    {
        p_event->event_id.event_b1.bitmask  = 
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT;
        p_event->event_id.event_b1.b1_threshold_geran =  
            p_rrm_irat_params->b1_threshold_geran; 
    } 
    else if(utran_trigger_quantity == RRM_THREE)
    {
        p_event->event_id.event_b1.bitmask |=
            REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT;
        /* SPR 16880: start */
        p_event->event_id.event_b1.b1_threshold_cdma2000 =
            p_rrm_irat_params->b1_threshold_cdma2000;
        /* SPR 16880: end */
    }

    p_report_config_to_add_mod->report_config.report_config_interrat.
        max_report_cells = p_rrm_irat_params->max_report_cells;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        report_interval =  p_rrm_irat_params->report_interval;

    p_report_config_to_add_mod->report_config.report_config_interrat.
        report_amount = p_rrm_irat_params->report_amount;

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME  : fill_report_config_info_for_B1 
 *  INPUT          : p_rrm_ue_context
 *  OUTPUT         : p_meas_config
 *  DESCRIPTION    : Fills the reporting configuration in the report config list 
 *                   with values received from OAM that to be sent to RRC.
 *  RETURNS        : None
 ******************************************************************************/

    rrm_void_t
fill_report_config_info_for_B1(
        rrm_meas_config_t                *p_meas_config,
        rrm_ue_context_t                 *p_rrm_ue_context
        )
{
    RRM_UT_TRACE_ENTER();

    U8 report_config_id = RRM_ZERO;
    U8 utran_trigger_quantity = RRM_RRC_CPICH_RSCP;

    rrm_irat_params_t             *p_rrm_irat_params         = RRM_PNULL;
    /*SPR 17777 +-*/
    p_rrm_irat_params         =  rrm_cellm_get_connected_mode_irat_params\
                                 (p_rrm_ue_context->cell_index);
    /*SPR 17777 +-*/
    U8 meas_obj_count                         =  RRM_ZERO;
    U8 counter                                =  RRM_ZERO;
    meas_object_to_add_mod_list_t 
        *p_meas_object_to_add_mod_list = RRM_PNULL;
    U8 utra_inter_rat_freq_rep_id_flag        = RRM_ZERO;
    U8 geran_inter_rat_freq_rep_id_flag        = RRM_ZERO;
    U8 cdma2000_inter_rat_freq_rep_id_flag     = RRM_ZERO;
    U8 index                                  = RRM_ZERO;
    U8 geran_trigger_quantity                 = RRM_TWO;
    /* coverity : CID 30247*/
    U8 cdma2000_trigger_quantity              = RRM_THREE;
    p_meas_object_to_add_mod_list = &(p_meas_config->meas_object_to_add_mod_list);
    index = p_meas_config->report_config_to_add_mod_list.count;
    meas_obj_count = p_meas_object_to_add_mod_list->count;

    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_rrm_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");
    }

    if (meas_obj_count >= RRM_ONE)
    {
        for( counter = RRM_ZERO; counter < meas_obj_count; counter++ )
        {
            if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                    meas_object.bitmask & MEAS_OBJECT_TO_ADD_UTRA_PRESENT)  
            {

                /* coverity : CID 21806*/
                utran_trigger_quantity = (U8) get_b1_trigger_quantity (p_rrm_ue_context->cell_index);

                /*
                   if (geran_inter_rat_freq_rep_id_flag == 1 && utra_inter_rat_freq_rep_id_flag == 0)
                   {
                   p_event = &(p_meas_config->report_config_to_add_mod_list.\
                   report_config_to_add_mod[b1_geran_report_config_index].\
                   report_config.report_config_interrat.trigger_type.event);

                   if( utran_trigger_quantity == RRM_RRC_CPICH_RSCP )
                   {
                   p_event->event_id.event_b1.bitmask  =
                   REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT;
                   p_event->event_id.event_b1.b1_threshold_utra.bitmask =\
                   THRESHOLD_UTRA_RSCP_PRESENT;
                   p_event->event_id.event_b1.b1_threshold_utra.threshold_rscp =
                   p_rrm_irat_params->b1_threshold_utra_rscp;
                   }
                //For limiting UE measurement 
                else if (utran_trigger_quantity == RRM_RRC_CPICH_ECN0)
                {
                p_event->event_id.event_b1.bitmask  =
                REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT;
                p_event->event_id.event_b1.b1_threshold_utra.bitmask = THRESHOLD_UTRA_ECN0_PRESENT;
                p_event->event_id.event_b1.b1_threshold_utra.threshold_ecn0 =
                p_rrm_irat_params->b1_threshold_utra_ecn0;
                }
                // For limiting UE measurement end 
                utra_inter_rat_freq_rep_id_flag = 1;
                }

*/

                if ( utra_inter_rat_freq_rep_id_flag  == RRM_ZERO)
                {
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {

                        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
                            report_config_id = report_config_id;

                        /* Start fix for bug 715 */
                        /* configuring RSCP/ECN0 for UTRAN TDD/FDD (limiting UE) measurement */
                        if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                                meas_object.meas_object_utra.cells_to_add_mod_list.bitmask & MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT)
                        {
                            fill_report_config_for_event_B1(
                                    &(p_meas_config->report_config_to_add_mod_list.
                                        report_config_to_add_mod[index]), p_rrm_irat_params,
                                    /*SPR 17777 +-*/
                                    utran_trigger_quantity);
                        }
                        else if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                                meas_object.meas_object_utra.cells_to_add_mod_list.bitmask & MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT)
                        {
                            fill_report_config_for_event_B1(&(p_meas_config->report_config_to_add_mod_list.
                                        report_config_to_add_mod[index]), p_rrm_irat_params,
                                    /*SPR 17777 +-*/
                                    (U8)RRM_RRC_PCCPCH_RSCP); /*coverity : CID 41572
info : strongly typecasted */
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "UTRAN TDD/FDD Report Config Id not configured for B1");
                        }
                        /* End fix for bug 715 */
                        utra_inter_rat_freq_rep_id_flag = RRM_ONE;
                        index++;
                        p_meas_config->report_config_to_add_mod_list.count++;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }
                }
            }
            else if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                    meas_object.bitmask & MEAS_OBJECT_TO_ADD_GERAN_PRESENT)           
            {

                /*
                   if (utra_inter_rat_freq_rep_id_flag == 1 && geran_inter_rat_freq_rep_id_flag == 0)
                   {
                   p_event = &(p_meas_config->report_config_to_add_mod_list.\
                   report_config_to_add_mod[b1_utran_report_config_index].\
                   report_config.report_config_interrat.trigger_type.event);
                   p_event->event_id.event_b1.bitmask |=
                   REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT;
                   p_event->event_id.event_b1.b1_threshold_geran =
                   p_rrm_irat_params->b1_threshold_geran;         
                   geran_inter_rat_freq_rep_id_flag = 1;
                   b1_geran_report_config_index = b1_utran_report_config_index;
                   }
                   */

                if(geran_inter_rat_freq_rep_id_flag == RRM_ZERO)
                {
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);
                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {
                        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
                            report_config_id = report_config_id;

                        /* configuring only RSCP for limiting UE measurement */  
                        fill_report_config_for_event_B1(
                                &(p_meas_config->report_config_to_add_mod_list.
                                    report_config_to_add_mod[index]), p_rrm_irat_params,
                                /*SPR 17777 +-*/
                                geran_trigger_quantity);

                        geran_inter_rat_freq_rep_id_flag = RRM_ONE;
                        index++;
                        p_meas_config->report_config_to_add_mod_list.count++;
                    }           
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }                     
                }                    
            }           
            else if(p_meas_object_to_add_mod_list->meas_object_to_add_mod[counter].
                    meas_object.bitmask & MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT)           
            {

                /*

                   if (utra_inter_rat_freq_rep_id_flag == 1 && 
                   geran_inter_rat_freq_rep_id_flag == 0 &&
                   cdma2000_inter_rat_freq_rep_id_flag == 0)
                   {
                   p_event = &(p_meas_config->report_config_to_add_mod_list.\
                   report_config_to_add_mod[b1_utran_report_config_index].\
                   report_config.report_config_interrat.trigger_type.event);
                   p_event->event_id.event_b1.bitmask |=
                   REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT;
                   p_event->event_id.event_b1.b1_threshold_cdma2000 =
                   p_common_params_for_eutra->b1_threshold_cdma2000;         
                   cdma2000_inter_rat_freq_rep_id_flag = 1;
                   }
                   else if(geran_inter_rat_freq_rep_id_flag == 1 && 
                   cdma2000_inter_rat_freq_rep_id_flag == 0)
                   {
                   p_event = &(p_meas_config->report_config_to_add_mod_list.\
                   report_config_to_add_mod[b1_geran_report_config_index].\
                   report_config.report_config_interrat.trigger_type.event);
                   p_event->event_id.event_b1.bitmask |=
                   REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT;
                   p_event->event_id.event_b1.b1_threshold_cdma2000 =
                   p_common_params_for_eutra->b1_threshold_cdma2000;         
                   cdma2000_inter_rat_freq_rep_id_flag = 1;
                   }

*/
                if(cdma2000_inter_rat_freq_rep_id_flag == RRM_ZERO)
                {
                    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);
                    if(report_config_id <= MAX_MEAS_OBJECT_ID)
                    {
                        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
                            report_config_id = report_config_id;

                        /* configuring only RSCP for limiting UE measurement */  
                        fill_report_config_for_event_B1(
                                &(p_meas_config->report_config_to_add_mod_list.
                                    report_config_to_add_mod[index]), p_rrm_irat_params,
                                /*SPR 17777 +-*/
                                cdma2000_trigger_quantity);

                        cdma2000_inter_rat_freq_rep_id_flag = RRM_ONE;
                        index++;
                        p_meas_config->report_config_to_add_mod_list.count++;
                    }           
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
                    }                     
                }                    
            }           
        }
    }
    if (p_meas_config->report_config_to_add_mod_list.count != RRM_ZERO)
    {
        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
    }
    RRM_UT_TRACE_EXIT();
}

/*start supported_rat*/
/******************************************************************************
 *  FUNCTION NAME: rrm_get_rat_priority_from_spid
 *  INPUT        : p_ue_ctxt
 *  OUTPUT       : is_eutran_spprted_by_spid,is_utran_spprted_by_spid,
 is_geran_supported_by_spid,spid_found
 *  DESCRIPTION  : get the highest priority rat/rats depending upom spid
 *  RETURNS      : None
 ******************************************************************************/
void
rrm_get_rat_priority_from_spid(
        rrm_ue_context_t *p_ue_ctxt, /*incoming param*/
        rrm_bool_et *is_eutran_spprted_by_spid, /*outgoing param*/
        rrm_bool_et *is_utran_spprted_by_spid, /*outgoing param*/
        rrm_bool_et *is_geran_supported_by_spid, /*outgoing param*/
        /* SPR 13205 Fix Start */
        rrm_bool_et *is_cdma2000_supported_by_spid, /*outgoing param*/
        /* SPR 13205 Fix End */
        rrm_bool_et *spid_found /*outgoing param*/
        )
{
    rrm_oam_spid_table_t *spid_table_params = RRM_PNULL;
    /* coverity : CID 29728*/
    /* SPR 20653 Fix Start */
    rrm_oam_rat_priority_info_et get_eutran_priority  = RRM_OAM_RAT_PRIORITY_HIGH;
    /* coverity : CID 29730*/
    rrm_oam_rat_priority_info_et get_utran_priority   = RRM_OAM_RAT_PRIORITY_HIGH;
    /* coverity : CID 29729*/
    rrm_oam_rat_priority_info_et get_geran_priority   = RRM_OAM_RAT_PRIORITY_HIGH;
    /* coverity : CID 29731*/
    /* SPR 13205 Fix Start */
    rrm_oam_rat_priority_info_et get_cdma_priority   = RRM_OAM_RAT_PRIORITY_HIGH;
    /* SPR 13205 Fix End */
    /* CID 112964 Start */
    rrm_oam_rat_priority_info_et highest_priority_rat = RRM_OAM_RAT_PRIORITY_HIGH;
    /* CID 112964 End */
    /* SPR 20653 Fix End */
    U8  higher_priority_rat = RRM_ZERO;
    U8 index = RRM_ZERO;
    U32 i = RRM_ZERO;
    RRM_UT_TRACE_ENTER();
    /*find eutran,utran,geran frequencies from SPID table*/
    spid_table_params = rrm_cellm_get_spid_table_params(p_ue_ctxt->cell_index);

    if(RRM_PNULL != spid_table_params && RRM_ZERO != spid_table_params->spid_count)
    {
        /* Bug_12476_Fix: Start */
        for(i =RRM_ZERO ; i<spid_table_params->spid_count ; i++)
            /* Bug_12476_Fix: End */
        {
                /* SPR 20653 Fix Start */
                 /* SPR 22553 FIXED START*/
            if(spid_table_params->spid_config[i].spid == p_ue_ctxt->rrm_sub_profile_id_info.sub_profile_id)
            {
                get_rat_priority(&spid_table_params->spid_config[i].rrm_rat_priority_list,
                            &get_eutran_priority, &get_utran_priority, &get_geran_priority, &get_cdma_priority);
                 /* SPR 22553 FIXED END*/
                /* SPR 20653 Fix End */
                /* SPR 13205 Fix End */

                *spid_found = RRM_TRUE;
                break;
            }
        }
    }
    if(RRM_TRUE == *spid_found)
    {
        /*get highest priority rat*/
        /* SPR 13168 Start */
        /* SPR 13205 Fix Start */
        *is_eutran_spprted_by_spid = RRM_TRUE;
        *is_utran_spprted_by_spid = RRM_TRUE;
        *is_geran_supported_by_spid = RRM_TRUE;
        *is_cdma2000_supported_by_spid = RRM_TRUE;
        highest_priority_rat =  RRM_GET_MIN(RRM_GET_MIN(RRM_GET_MIN(get_eutran_priority,get_utran_priority),get_geran_priority),get_cdma_priority);
        /* SPR 13205 Fix End */
        if (!(p_ue_ctxt->ho_params.bitmask & RRM_UE_CSFB_RECD))
        {
            highest_priority_rat = RRM_GET_MIN(highest_priority_rat, get_eutran_priority);
        }
        /* SPR 13168 End */

        /*select all the highest priority rats*/
        if(get_eutran_priority == highest_priority_rat)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "eutran supported by spid");
            p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
            higher_priority_rat = RRM_GET_MIN(RRM_GET_MIN(get_utran_priority, get_geran_priority),get_cdma_priority);
            if (get_utran_priority == higher_priority_rat)
            {
                /* Bug_13214_fix: Start */
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;

                higher_priority_rat = RRM_GET_MIN(get_geran_priority,get_cdma_priority);
                if (get_geran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                }
                else if (get_cdma_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                }
                /* Bug_13214_fix: End*/
            }
            else if(get_geran_priority == higher_priority_rat)
            {
                /* Bug_13214_fix: Start */
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;

                higher_priority_rat = RRM_GET_MIN(get_utran_priority,get_cdma_priority);
                if (get_utran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                }
                else if (get_cdma_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                }
                /* Bug_13214_fix: End */
            }
            else if(get_cdma_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                higher_priority_rat = RRM_GET_MIN(get_utran_priority,get_geran_priority);
                if (get_utran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                }
                else if (get_geran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                }
            }

        }
        else if(get_utran_priority == highest_priority_rat)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "utran supported by spid");
            p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
            higher_priority_rat = RRM_GET_MIN(RRM_GET_MIN(get_eutran_priority, get_geran_priority),get_cdma_priority);
            if (get_eutran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                higher_priority_rat = RRM_GET_MIN(get_geran_priority,get_cdma_priority);
                if (get_geran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                }
                else if (get_cdma_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                }
            }
            else if(get_geran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                higher_priority_rat = RRM_GET_MIN(get_eutran_priority,get_cdma_priority);
                if (get_eutran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                }
                else if (get_cdma_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                }
            }
            else if(get_cdma_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                higher_priority_rat = RRM_GET_MIN(get_eutran_priority,get_geran_priority);
                if (get_eutran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                }
                else if (get_geran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                }
            }
        }
        else if(get_geran_priority == highest_priority_rat)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "geran supported by spid");
            p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
            higher_priority_rat = RRM_GET_MIN(RRM_GET_MIN(get_eutran_priority, get_utran_priority),get_cdma_priority);
            if (get_eutran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                higher_priority_rat = RRM_GET_MIN(get_utran_priority,get_cdma_priority);
                if (get_utran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                }
                else if (get_cdma_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                }
            }
            else if(get_utran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                higher_priority_rat = RRM_GET_MIN(get_eutran_priority,get_cdma_priority);
                if (get_eutran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                }
                else if (get_cdma_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                }
            }
            else if(get_cdma_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
                higher_priority_rat = RRM_GET_MIN(get_eutran_priority,get_utran_priority);
                if (get_eutran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                }
                else if (get_utran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                }
            }
        }
        else if(get_cdma_priority == highest_priority_rat)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "cdma supported by spid");
            p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_CDMA2000;
            higher_priority_rat = RRM_GET_MIN(RRM_GET_MIN(get_eutran_priority, get_utran_priority),get_geran_priority);
            if (get_eutran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                higher_priority_rat = RRM_GET_MIN(get_utran_priority,get_geran_priority);
                if (get_utran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                }
                else if (get_geran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                }
            }
            else if(get_utran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                higher_priority_rat = RRM_GET_MIN(get_eutran_priority,get_geran_priority);
                if (get_eutran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                }
                else if (get_geran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                }
            }
            else if(get_geran_priority == higher_priority_rat)
            {
                p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_GERAN;
                higher_priority_rat = RRM_GET_MIN(get_eutran_priority,get_utran_priority);
                if (get_eutran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                }
                else if (get_utran_priority == higher_priority_rat)
                {
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_UTRAN;
                    p_ue_ctxt->irat_priority.irat_priority_list[index++] = RRM_IRAT_PRIORITY_EUTRAN;
                }
            }
        }
    }
    /* SPR 13205 Fix End */
    else
    {
         /* SPR 22553 FIXED START*/
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                "SPID entry [%d] not found in SPID table", p_ue_ctxt->rrm_sub_profile_id_info.sub_profile_id);
         /* SPR 22553 FIXED END*/
    }

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: rrm_find_supported_rat
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : eutran_supported,utran_supported,grean_supported,cdma_supported
 *  DESCRIPTION  : Find out supported rat based on the parameter supported_rat 
 *                 from oam and SPID
 *  RETURNS      : None
 ******************************************************************************/
void
rrm_find_supported_rat(
        rrm_ue_context_t *p_rrm_ue_context, /*incoming parameter*/
        rrm_bool_et *eutran_supported, /*outgoing parameter*/
        rrm_bool_et *utran_supported, /*outgoing parameter*/
        rrm_bool_et *grean_supported, /*outgoing parameter*/
        /* SPR 11760 FIX START */
        rrm_bool_et *cdma_supported /*outgoing parameter*/
        /* SPR 11760 FIX END */
        )
{
    epc_params_t *p_epc_params = RRM_NULL; 
    U8 drb_donfigd_index = RRM_ZERO;
    U8 qos_param_index = RRM_ZERO;
    U8 spportd_rat_btmp_per_qci = RRM_ZERO;
    U8 spportd_rat_btmp = RRM_ZERO;
    rrm_bool_et is_eutran_spprted_by_spid = RRM_FALSE;
    rrm_bool_et is_utran_spprted_by_spid = RRM_FALSE;
    rrm_bool_et is_geran_supported_by_spid = RRM_FALSE;
    /* SPR 13205 Fix Start */
    rrm_bool_et is_cdma2000_supported_by_spid = RRM_FALSE;
    /* SPR 13205 Fix End */
    rrm_bool_et spid_found = RRM_FALSE;
    U8 index = RRM_ZERO;

    RRM_UT_TRACE_ENTER();   

    /*initialize supported rat as false*/
    *eutran_supported = RRM_FALSE;
    *utran_supported = RRM_FALSE;
    *grean_supported = RRM_FALSE;
    /* SPR 11760 FIX START */
    *cdma_supported = RRM_FALSE;
    /* SPR 11760 FIX END */

    /*If no bearer is created yet, no need to
      check supported rat on qci basis and spid*/
    if(RRM_ZERO == p_rrm_ue_context->drb_configured.num_of_list)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "No bearer is created yet, skipping validation "
                "for per qci supported rat");
        *eutran_supported = RRM_TRUE;
        *utran_supported = RRM_TRUE;
        *grean_supported = RRM_TRUE;
        /* SPR 11760 FIX START */
        *cdma_supported = RRM_TRUE;
        /* SPR 11760 FIX END */
        return;
    }

    p_epc_params = rrm_cellm_get_erb_service_profile(p_rrm_ue_context->cell_index);

    /*find out supported rat based on configured drbs*/
    for(drb_donfigd_index = RRM_ZERO; 
            drb_donfigd_index < p_rrm_ue_context->drb_configured.num_of_list;
            drb_donfigd_index++)
    {
        for(qos_param_index = RRM_ZERO; 
                qos_param_index < p_epc_params->num_valid_qos_profiles;
                qos_param_index++) 
        {
            if(p_rrm_ue_context->drb_configured.erab_item[drb_donfigd_index].
                    erab_level_qos_params.qci == 
                    p_epc_params->qos_config_params[qos_param_index].qci)
            {
                if(p_epc_params->qos_config_params[qos_param_index].bitmask &
                        RRMCM_RMIF_SUPPORTED_RAT_PRESENT) 
                {
                    spportd_rat_btmp_per_qci = p_epc_params->qos_config_params[qos_param_index].supported_rat;
                    /*take union of all supported rat per qci*/
                    spportd_rat_btmp |= spportd_rat_btmp_per_qci;
                }
                break;
            }
        }
        /*If all supported bit are already set in spportd_rat_btmp
          no need to check configured drbs further*/
        if(0x1F == spportd_rat_btmp)
        {
            break;
        }
    }

    /*get supported rats based on spid based
      priority*/
    rrm_get_rat_priority_from_spid(p_rrm_ue_context,
            &is_eutran_spprted_by_spid,
            &is_utran_spprted_by_spid ,
            &is_geran_supported_by_spid,
            /* SPR 13205 Fix Start */
            &is_cdma2000_supported_by_spid,
            /* SPR 13205 Fix End */
            &spid_found);

    if(RRM_TRUE == spid_found)
    {
        p_rrm_ue_context->spid_found = RRM_TRUE;
    }

    if(RRM_ZERO == spportd_rat_btmp)
    {
        *eutran_supported = RRM_TRUE;
        *utran_supported = RRM_TRUE;
        *grean_supported = RRM_TRUE;
        /* SPR 13205 Fix Start */
        *cdma_supported = RRM_TRUE;
        /* SPR 13205 Fix End */
        /* SPR 11760 FIX START */
        spportd_rat_btmp = 0x1F;
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Union of supported rats is calculated as zero, "
                "setting all supported rat as TRUE to consider SPID priorities");
        /* SPR 11760 FIX END */
    }
    /*eutran_supported,utran_supported and geran_supported 
      are by default set as false.We have to make them false
      depending upon supported_rat and spid*/
    if(spportd_rat_btmp & RRM_EUTRAN_RAT_SUPPORTED)
    {
        if(RRM_TRUE == spid_found)
        {
            if(RRM_TRUE == is_eutran_spprted_by_spid)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "eutran is supported by per qci supported rat and spid ");
                *eutran_supported = RRM_TRUE;
            }
            /* SPR 11760 FIX START */
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "eutran is supported by per qci supported rat but NOT by spid ");
            }
            /* SPR 11760 FIX END */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "eutran is supported by per qci supported rat");
            *eutran_supported = RRM_TRUE;
        }
    }

    if(spportd_rat_btmp & RRM_UTRAN_RAT_SUPPORTED)
    {
        if(RRM_TRUE == spid_found)
        {
            if(RRM_TRUE == is_utran_spprted_by_spid)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "utran is supported by per qci supported rat and spid ");
                *utran_supported = RRM_TRUE;
            }
            /* SPR 11760 FIX START */
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "utran is supported by per qci supported rat but NOT by spid ");
            }
            /* SPR 11760 FIX END */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "utran is supported by per qci supported rat ");
            *utran_supported = RRM_TRUE;
        }
    }

    if(spportd_rat_btmp & RRM_GERAN_RAT_SUPPORTED)
    {
        if(RRM_TRUE == spid_found)
        {
            if(RRM_TRUE == is_geran_supported_by_spid)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "geran is supported by per qci supported rat and spid ");
                *grean_supported = RRM_TRUE;
            }
            /* SPR 11760 FIX START */
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "geran is supported by per qci supported rat but NOT by spid ");
            }
            /* SPR 11760 FIX END */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "utran is supported by per qci supported rat");
            *grean_supported = RRM_TRUE;    
        }
    }

    /* SPR 11760 FIX START */

    /* Check not required for SPID as SPID priority for CDMA is always assigned and i.e with the lowest priority */
    /* SPR 13205 Fix Start */
    if((spportd_rat_btmp & RRM_CDMA_1xRTT_RAT_SUPPORTED) || (spportd_rat_btmp & RRM_CDMA_HPRD_RAT_SUPPORTED))
    {
        if(RRM_TRUE == spid_found)
        {
            if(RRM_TRUE == is_cdma2000_supported_by_spid)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "cdma is supported by per qci supported rat and spid ");
                *cdma_supported = RRM_TRUE;                
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "cdma is supported by per qci supported rat ");
            *cdma_supported = RRM_TRUE;                
        }
    }
    /* SPR 13205 Fix End */

    /* Remove the RAT types which are not supported from the priority list */
    if(RRM_FALSE == *eutran_supported)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "eutran is NOT supported by per qci supported rat and/or spid");

        for(index = RRM_ZERO; index < RRM_IRAT_TYPE; index++)
        {
            if(p_rrm_ue_context->irat_priority.irat_priority_list[index] == RRM_IRAT_PRIORITY_EUTRAN)
            {
                /* coverity : CID 30251 */
            /* SPR 20653 Fix Start */
                p_rrm_ue_context->irat_priority.irat_priority_list[index] = RRM_IRAT_PRIORITY_MAX;
            /* SPR 20653 Fix End */
                break;
            }
        }
    }
    if(RRM_FALSE == *utran_supported)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "utran is NOT supported by per qci supported rat and/or spid");

        for(index = RRM_ZERO; index < RRM_IRAT_TYPE; index++)
        {
            if(p_rrm_ue_context->irat_priority.irat_priority_list[index] == RRM_IRAT_PRIORITY_UTRAN)
            {
                /* coverity : CID 30251 */
                /* SPR 20653 Fix Start */
                p_rrm_ue_context->irat_priority.irat_priority_list[index] = RRM_IRAT_PRIORITY_MAX;
                /* SPR 20653 Fix End */
                break;
            }
        }
    }
    if(RRM_FALSE == *grean_supported)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "geran is NOT supported by per qci supported rat and/or spid");

        for(index = RRM_ZERO; index < RRM_IRAT_TYPE; index++)
        {
            if(p_rrm_ue_context->irat_priority.irat_priority_list[index] == RRM_IRAT_PRIORITY_GERAN)
            {
                /* coverity : CID 30251 */
                /* SPR 20653 Fix Start */
                p_rrm_ue_context->irat_priority.irat_priority_list[index] = RRM_IRAT_PRIORITY_MAX;
                /* SPR 20653 Fix End */
                break;
            }
        }
    }

    if(RRM_FALSE == *cdma_supported)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "CDMA 1xrtt and HRPD is NOT supported by per qci supported rat");

        for(index = RRM_ZERO; index < RRM_IRAT_TYPE; index++)
        {
            if(p_rrm_ue_context->irat_priority.irat_priority_list[index] == RRM_IRAT_PRIORITY_CDMA2000)
            {
                /* SPR 20653 Fix Start */
                p_rrm_ue_context->irat_priority.irat_priority_list[index] = RRM_IRAT_PRIORITY_MAX;
                /* SPR 20653 Fix End */
                break;
            }
        }
    }

    /* SPR 11760 FIX END */

    RRM_UT_TRACE_EXIT();   
}
/*end supported_rat*/



/******************************************************************************
 *  FUNCTION NAME: get_next_priority_rat
 *  INPUT        : index,p_rrm_ue_ctx
 *  OUTPUT       : none
 *  DESCRIPTION  : get next RAT according to spid and qci priority 
 *  RETURNS      : None
 ******************************************************************************/
    U8
get_next_priority_rat(
        U8                            index,
        rrm_ue_context_t              *p_rrm_ue_ctx          
        )
{
    U8 rat_type = RRM_ZERO;
    rrm_cell_context_t   *p_cell_ctx = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    p_cell_ctx = rrm_cellm_get_cell_context(p_rrm_ue_ctx->cell_index);
    /*CID 65765:start*/
    if(RRM_PNULL != p_cell_ctx)
    {
        if(p_rrm_ue_ctx->spid_found == RRM_TRUE)
        {
            rat_type = p_rrm_ue_ctx->irat_priority.irat_priority_list[index];
        }
        else if(p_cell_ctx->operator_info.bitmask & 
                RRMCM_RMIF_RAT_PRIORITY_INFO_CFG_PRESENT)
        {
            rat_type = p_cell_ctx->operator_info.rat_priority_info.
                irat_priority_list_cfg[index];
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,"p_cell_ctx is null");
    }
    /*CID 65765:end*/
    RRM_UT_TRACE_EXIT();

    return rat_type;
}
/*CA Stage3: start*/
/******************************************************************************
 *  FUNCTION NAME: rrm_fill_a1_a2_report_config 
 *  INPUT        : p_rrm_ue_ctx,cell_index, index
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the report config of A1 A2 event in meas config list with values 
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : RRM_SUCCESS/RRM_FAILURE
 ******************************************************************************/
    rrm_return_et
rrm_fill_a1_a2_report_config(
        rrm_meas_config_t *p_meas_config,
        rrm_ue_context_t  *p_rrm_ue_context, 
        U8                *index,
        rrm_cell_index_t  cell_index
        )
{
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;
    U8 report_config_id = RRM_ZERO;
    rrm_return_et ret_val = RRM_FAILURE;
    RRM_UT_TRACE_ENTER();
    p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                (cell_index);

    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[*index].
            report_config_id = report_config_id;
        fill_report_config_for_event_A1(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[*index]), p_common_params_for_eutra, RRM_RRC_RSRQ);

        (*index)++;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }

    report_config_id = rrm_uem_allocate_free_id (p_rrm_ue_context, REP_CONFIG);

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[*index].
            report_config_id = report_config_id;
        fill_report_config_for_event_A2(&(p_meas_config->report_config_to_add_mod_list.
                    report_config_to_add_mod[*index]), p_common_params_for_eutra, RRM_RRC_RSRQ);
        (*index)++;
        p_meas_config->report_config_to_add_mod_list.count++;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Maximum Report Config Id reached for [UE:%d]", p_rrm_ue_context->ue_index);
    }
    if (RRM_ZERO != p_meas_config->report_config_to_add_mod_list.count)
    {
        ret_val = RRM_SUCCESS;
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/******************************************************************************
 *  FUNCTION NAME: rrm_fill_a1_a2_meas_config_object 
 *  INPUT        : p_rrm_ue_ctx,cell_index
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the A1 A2 meas object in meas config list with values 
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : RRM_SUCCESS/RRM_FAILURE
 ******************************************************************************/
    rrm_return_et
rrm_fill_a1_a2_meas_config_object(
        rrm_meas_config_t *p_meas_config,
        rrm_ue_context_t  *p_rrm_ue_ctx, 
        rrm_cell_index_t cell_index)
{
    rrm_bool_et eutran_supported =  RRM_FALSE;
    rrm_bool_et utran_supported  =  RRM_FALSE;
    rrm_bool_et grean_supported  =  RRM_FALSE;
    U8 index = RRM_ZERO;
    rrm_return_et   ret_val = RRM_FAILURE;
    rrm_cell_context_t  *p_cell_ctx = RRM_PNULL;
    U8                  meas_report_index = RRM_ZERO;
    meas_report_info_node_t  *p_meas_report_node = RRM_PNULL;
    /* SPR 11760 FIX START */
    rrm_bool_et cdma_supported                  =  RRM_FALSE;
    /* SPR 11760 FIX END */

    RRM_UT_TRACE_ENTER();
    /* SPR 18431 Fix Start */
    rrm_cell_context_t  *p_p_cell_ctx = RRM_PNULL;

    /* SPR 18431 Fix End*/
    p_cell_ctx = rrm_cellm_get_cell_context(cell_index);
    if (RRM_PNULL == p_cell_ctx)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "Cell context is NULL for cell index[%d]", cell_index); 
        return RRM_FAILURE;
    }

    /* SPR 18431 Fix Start */
    p_p_cell_ctx = rrm_cellm_get_cell_context(p_rrm_ue_ctx->cell_index);
    if (RRM_PNULL == p_p_cell_ctx)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "Cell context is NULL for cell index[%d]", cell_index); 
        return RRM_FAILURE;
    }




    if(RRM_IS_MASA_INFO_PRESENT & p_p_cell_ctx->operator_info.ca_config.bitmask)
    {
        /* Setting the flag as false. Now measurment of A4 is possible */
        if (p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn == p_p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn)
        {
            p_rrm_ue_ctx->ca_data.a4_recv_for_ca = RRM_FALSE;

            rrm_find_supported_rat(
                    p_rrm_ue_ctx,
                    &eutran_supported,
                    &utran_supported,
                    &grean_supported,
                    /* SPR 11760 FIX START */
                    &cdma_supported
                    /* SPR 11760 FIX END */
                    );
            if (RRM_TRUE == eutran_supported)
            {
                fill_intra_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, \
                        &index,
                        /* start supported_rat */
                        eutran_supported,
                        /* end supported_rat */
                        cell_index
                        );
                if((p_meas_config->bitmask & UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT) 
                        && (p_cell_ctx->operator_info.bitmask & RRMCM_RMIF_CA_CONFIG_INFO_PRESENT)
                        && (p_cell_ctx->operator_info.ca_config.bitmask & RRM_CA_MEAS_CYCLE_SCELL_PRESENT)) 
                {
                    p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[index].
                        meas_object.meas_object_eutra.meas_cycle_scell_r10 = p_cell_ctx->operator_info.ca_config.meas_cycle_scell; 
                }

                ret_val = rrm_fill_a1_a2_report_config(p_meas_config,p_rrm_ue_ctx,&index,cell_index);

                if (p_meas_config->report_config_to_add_mod_list.count != RRM_ZERO)
                {
                    p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
                }
                fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,cell_index, RRM_TRUE);
                /* Add meas_id_to_remove_list for which A4 is triggered */
                p_meas_report_node = (meas_report_info_node_t *)ylFirst(&(p_rrm_ue_ctx->meas_report_list));
                while(p_meas_report_node)
                {
                    if ((MEAS_TYPE_CA == p_meas_report_node->meas_type) &&
                            (MEAS_REP_EVENT_A4 == p_meas_report_node->reporting_event) &&
                            (RRM_MEAS_CONFIG_TO_DELETE != p_meas_report_node->meas_status))
                    {
                        p_meas_config->report_config_to_remove_list.report_config_id[meas_report_index++]
                            = p_meas_report_node->report_config_id;
                        p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
                        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT; 

                    }
                    p_meas_report_node = (meas_report_info_node_t *)ylNext(&p_meas_report_node->sNode);
                }
                p_meas_config->meas_id_to_remove_list.count = meas_report_index;

                fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);
                rrm_raise_event_meas_config_a1( p_rrm_ue_ctx );
                rrm_raise_event_meas_config_a2( p_rrm_ue_ctx );
            }
        }
        else
        {
            p_rrm_ue_ctx->ca_data.a4_recv_for_ca = RRM_FALSE;

            rrm_find_supported_rat(
                    p_rrm_ue_ctx,
                    &eutran_supported,
                    &utran_supported,
                    &grean_supported,
                    /* SPR 11760 FIX START */
                    &cdma_supported
                    /* SPR 11760 FIX END */
                    );
            if (RRM_TRUE == eutran_supported)
            {
                fill_inter_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, \
                        RRM_INTER_FREQ_FOR_SCELL                
                        );
                if((p_meas_config->bitmask & UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT) 
                        && (p_cell_ctx->operator_info.bitmask & RRMCM_RMIF_CA_CONFIG_INFO_PRESENT)
                        && (p_cell_ctx->operator_info.ca_config.bitmask & RRM_CA_MEAS_CYCLE_SCELL_PRESENT)) 
                {
                    p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[index].
                        meas_object.meas_object_eutra.meas_cycle_scell_r10 = p_cell_ctx->operator_info.ca_config.meas_cycle_scell; 
                }

                ret_val = rrm_fill_a1_a2_report_config(p_meas_config,p_rrm_ue_ctx,&index,cell_index);

                if (p_meas_config->report_config_to_add_mod_list.count != RRM_ZERO)
                {
                    p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
                }
                fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,cell_index, RRM_TRUE);
                /* Add meas_id_to_remove_list for which A4 is triggered */
                p_meas_report_node = (meas_report_info_node_t *)ylFirst(&(p_rrm_ue_ctx->meas_report_list));
                while(p_meas_report_node)
                {
                    if ((MEAS_TYPE_CA == p_meas_report_node->meas_type) &&
                            (MEAS_REP_EVENT_A4 == p_meas_report_node->reporting_event) &&
                            (RRM_MEAS_CONFIG_TO_DELETE != p_meas_report_node->meas_status))
                    {
                        p_meas_config->report_config_to_remove_list.report_config_id[meas_report_index++]
                            = p_meas_report_node->report_config_id;
                        p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
                        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT;
                    }
                    p_meas_report_node = (meas_report_info_node_t *)ylNext(&p_meas_report_node->sNode);
                }
                p_meas_config->report_config_to_remove_list.count = meas_report_index;

                fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);
                rrm_raise_event_meas_config_a1( p_rrm_ue_ctx );
                rrm_raise_event_meas_config_a2( p_rrm_ue_ctx );
            }
        }
    }
    else
    {

        /* SPR 18431 Fix End*/

        /* Setting the flag as false. Now measurment of A4 is possible */
        p_rrm_ue_ctx->ca_data.a4_recv_for_ca = RRM_FALSE;

        rrm_find_supported_rat(
                p_rrm_ue_ctx,
                &eutran_supported,
                &utran_supported,
                &grean_supported,
                /* SPR 11760 FIX START */
                &cdma_supported
                /* SPR 11760 FIX END */
                );
        if (RRM_TRUE == eutran_supported)
        {
            fill_intra_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, \
                    &index,
                    /* start supported_rat */
                    eutran_supported,
                    /* end supported_rat */
                    cell_index
                    );
            if((p_meas_config->bitmask & UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT) 
                    && (p_cell_ctx->operator_info.bitmask & RRMCM_RMIF_CA_CONFIG_INFO_PRESENT)
                    && (p_cell_ctx->operator_info.ca_config.bitmask & RRM_CA_MEAS_CYCLE_SCELL_PRESENT)) 
            {
                p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[index].
                    meas_object.meas_object_eutra.meas_cycle_scell_r10 = p_cell_ctx->operator_info.ca_config.meas_cycle_scell; 
            }

            ret_val = rrm_fill_a1_a2_report_config(p_meas_config,p_rrm_ue_ctx,&index,cell_index);

            if (p_meas_config->report_config_to_add_mod_list.count != RRM_ZERO)
            {
                p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
            }
            fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,cell_index, RRM_TRUE);
            /* Add meas_id_to_remove_list for which A4 is triggered */
            p_meas_report_node = (meas_report_info_node_t *)ylFirst(&(p_rrm_ue_ctx->meas_report_list));
            while(p_meas_report_node)
            {
                if ((MEAS_TYPE_CA == p_meas_report_node->meas_type) &&
                        (MEAS_REP_EVENT_A4 == p_meas_report_node->reporting_event) &&
                        (RRM_MEAS_CONFIG_TO_DELETE != p_meas_report_node->meas_status))
                {
                    p_meas_config->meas_id_to_remove_list.meas_id[meas_report_index++]
                        = p_meas_report_node->meas_id;
                    p_meas_report_node->meas_status = RRM_MEAS_CONFIG_TO_DELETE;
                    p_meas_config->bitmask |= UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT;
                }
                p_meas_report_node = (meas_report_info_node_t *)ylNext(&p_meas_report_node->sNode);
            }
            p_meas_config->meas_id_to_remove_list.count = meas_report_index;

            fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);
            rrm_raise_event_meas_config_a1( p_rrm_ue_ctx );
            rrm_raise_event_meas_config_a2( p_rrm_ue_ctx );
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Eutra RAT not supported for [UE:%d]", p_rrm_ue_ctx->ue_index); 
            ret_val = RRM_FAILURE;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/*CA Stage3: end*/

/* eICIC_PHASE_1_2_CHANGES_START */
/******************************************************************************
 *  FUNCTION NAME: rrm_ue_fill_meas_config_req_for_victim_ue 
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with values 
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : RRM_SUCCESS/RRM_FAILURE
 ******************************************************************************/
    rrm_return_et
rrm_ue_fill_meas_config_req_for_victim_ue(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx          
        )
{
    U8 index = RRM_ZERO;
    /* bug_13863_start */
    rrm_bool_et  eutran_supported   =  RRM_FALSE;
    rrm_bool_et  utran_supported    =  RRM_FALSE;
    rrm_bool_et  grean_supported    =  RRM_FALSE;
    rrm_bool_et  cdma_supported     =  RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    RRM_MEMSET(p_meas_config, RRM_ZERO ,sizeof(rrm_meas_config_t));

    rrm_find_supported_rat(
            p_rrm_ue_ctx,
            &eutran_supported,
            &utran_supported,
            &grean_supported,
            &cdma_supported
            );
    /* bug_13863_end */

    if(eutran_supported)
    {
        /*CA HARDENING CHANGES*/
        /* SPR 14247 start */
        p_rrm_ue_ctx->meas_rep_event = MEAS_REP_EVENT_PERIODICAL;
        /* SPR 14247 end */
        fill_intra_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, \
                &index,
                eutran_supported,
                p_rrm_ue_ctx->cell_index
                );

        fill_report_config_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index);
        fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index, RRM_FALSE);
        fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);

    }    
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "eutran is not supported by per qci supported rat");

        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;

    }

    RRM_UT_TRACE_EXIT();
    return RRM_SUCCESS;

}
/* eICIC_PHASE_1_2_CHANGES_END */

/* SPR 16053 start */
/******************************************************************************
 *  FUNCTION NAME: rrm_is_anr_configured_for_inter_object
 *  INPUT        : p_ue_context
 *  OUTPUT       : none
 *  DESCRIPTION  : Check if any inter frequency or Inter RAT object configured
 *                 or not due to ANR procedure
 *  RETURNS      : RRM_TRUE/RRM_FALSE
 ******************************************************************************/
rrm_bool_et rrm_is_anr_configured_for_inter_object(
        rrm_ue_context_t *p_ue_context)
{
    rrm_bool_et         ret_val     = RRM_FALSE;
    rrm_cell_context_t  *p_cell_ctx = RRM_PNULL;
    U8                  index       = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    p_cell_ctx = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if (RRM_NULL != p_cell_ctx)
    {
        for(index = RRM_ZERO; index < RRM_ANR_MAX_MEAS_OBJ; index++)
        {
            if(RRM_CELL_UNDEFINED != p_ue_context->p_meas_config[index].rat_type)
            {
                /* Checking if strongest cell report is being configured or not */
                if ((p_ue_context->p_meas_config[index].carr_freq !=
                            p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn) &&
                        (RRM_MEAS_CONFIG_RESP_SUCCESS == p_ue_context->p_meas_config[index].meas_config_status))
                {
                    /* Found Inter Freq object configured for ANR */
                    ret_val = RRM_TRUE;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Inter Freq/RAT object found");
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* SPR 16053 end */

/* SPR 15260 start */
/******************************************************************************
 *  FUNCTION NAME: rrm_is_required_meas_gap_config
 *  INPUT        : p_ue_context
 *  OUTPUT       : none
 *  DESCRIPTION  : check if meas gap configuration is required
 *  RETURNS      : RRM_TRUE/RRM_FALSE
 ******************************************************************************/
rrm_bool_et
    rrm_is_required_meas_gap_config
(
 rrm_ue_context_t  *p_ue_context,
 rrm_meas_config_t *p_meas_config
 )
{
    U8 index = RRM_ZERO;
    rrm_bool_et ret_val = RRM_TRUE;
    meas_report_info_node_t *p_meas_report_info = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    rrm_cell_context_t *p_cell_ctx = rrm_cellm_get_cell_context(p_ue_context->cell_index);
    if(RRM_PNULL == p_cell_ctx)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "cell context not found");
        RRM_UT_TRACE_EXIT();
        return RRM_FALSE;
    }
    if (RRM_ZERO != ylCount(&(p_ue_context->meas_report_list)))
    {
        p_meas_report_info = (meas_report_info_node_t*)ylFirst(&(p_ue_context->meas_report_list));
        while(RRM_PNULL != p_meas_report_info)
        {
            /* Check if there exist any inter frequency object */
            if(p_meas_report_info->carrier_earfcn !=
                    p_cell_ctx->ran_info.rf_params.rf_configurations.dl_earfcn)
            {
                /* Found Inter Freq object configured during HO */
                ret_val = RRM_FALSE;
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        "Already found Inter Freq object in meas report list so not configuring meas gap");
                break;
            }

            p_meas_report_info = (meas_report_info_node_t*)ylNext(&p_meas_report_info->sNode);
        }
    }

    if(RRM_TRUE == ret_val)
    {
        /* SPR 16053 start */
        ret_val = rrm_is_anr_configured_for_inter_object(p_ue_context);
        /* SPR 16053 end */

    }

    if(RRM_TRUE == ret_val)
    {

        ret_val = RRM_FALSE;
        if(p_meas_config->meas_object_to_add_mod_list.count >= RRM_ONE)
        {

            for(index = RRM_ZERO; index < p_meas_config->meas_object_to_add_mod_list.count; index++)
            {

                if(p_meas_config->meas_object_to_add_mod_list.
                        meas_object_to_add_mod[index].
                        meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT)
                {
                    if(p_cell_ctx->ran_info.rf_params.
                            rf_configurations.dl_earfcn !=
                            p_meas_config->meas_object_to_add_mod_list.
                            meas_object_to_add_mod[index].
                            meas_object.meas_object_eutra.carrier_freq)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "Adding new Inter freq object so configuring meas gap");
                        ret_val = RRM_TRUE;
                        break;
                    }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Adding new Inter rat object so configuring meas gap");
                    ret_val = RRM_TRUE;
                    break;
                }

            }


        }
    }

    /* Start fix for Coverity_ID : 81740 */
    RRM_UT_TRACE_EXIT();
    /* End fix for Coverity_ID : 81740 */

    return ret_val;
}

/* SPR 15260 end */


/* BLR Inter RAT B1 configuration end */
/******************************************************************************
 *  FUNCTION NAME: rrm_e_fill_eas_config_req_for_ho
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the eutran configuration in meas config list with values 
 *                 received from OAM that to be sent to RRC.
 *  RETURNS      : RRM_SUCCESS/RRM_FAILURE
 ******************************************************************************/
/* Fix for SPR 10310 start */
rrm_return_et
/* Fix for SPR 10310 end */
rrm_ue_fill_meas_config_req_for_ho(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx          
        )
{
    RRM_UT_TRACE_ENTER();
    U8 index       = RRM_ZERO;
    U8 index_rat   = RRM_ZERO;

    /*coverity 23147:removing "trigger_quantity":simerjit, rel 1.3*/
    rrmc_meas_report_event_et  report_event     = MEAS_REP_EVENT_NONE;
    /* Freq priority list for RAT corresponding to SPID */

    /* SPR_14564_fix: start */
    /* Lines Deleted */
    /* SPR_14564_fix: end */
    /* SPR 16406 8SEP start */
    /*+-Coverity 93545*/
    report_event                                = (rrmc_meas_report_event_et)p_rrm_ue_ctx->meas_rep_event;
    /* SPR 16406 8SEP end */
    U8     delayed_a3_enabled                   = \
                                                  get_delayed_a3_configure(p_rrm_ue_ctx->cell_index);

    rrm_bool_et       configure_intra_meas_obj  = RRM_TRUE;
    //    p_ncl_params                                = rrm_cellm_get_ncl_info(p_rrm_ue_ctx->cell_index); CA HARDENING CHANGES 6.3.0

    /* SPR_14564_fix: start */
    /* Lines Deleted */
    /* SPR_14564_fix: end */
    /*start supported_rat*/
    rrm_bool_et eutran_supported                =  RRM_FALSE;
    rrm_bool_et utran_supported                 =  RRM_FALSE;
    rrm_bool_et grean_supported                 =  RRM_FALSE;
    /* SPR 11760 FIX START */
    rrm_bool_et cdma_supported                  =  RRM_FALSE;
    /* SPR 11760 FIX END */

    rrm_bool_et is_utran_supported              =  RRM_FALSE;
    rrm_bool_et is_geran_supported              =  RRM_FALSE;
    rrm_bool_et is_cdma2000_supported           =  RRM_FALSE;
    /*SPR 21365 fix start*/
    U8    meas_config_count                     = RRM_ZERO;
    U8    starting_meas_config_count            = RRM_ZERO;
    U8 intra_freq_neighbour_count               = RRM_NULL;
    ncl_params_t   *p_ncl_params                = RRM_PNULL;
    /*SPR 21365 fix stop*/

    U8 meas_obj_count = RRM_ZERO;
    /*SPR 21687 Start*/
    rrm_bool_et HRPD_MEAS_OBJ_CONFIGURED = RRM_FALSE;
    /* code removed */
    /*SPR 21687 End*/
    /* SPR 14239 Fix End */

    /*spr 6964 start*/
    /* SPR 15260 start */
    /* code deleted */
    /* SPR 15260 end */
    /*spr 6964 end*/
    /* SPR 10349 Start */
    rrm_bool_et meas_configured_until_daho = RRM_FALSE;
    /* SPR 10349 End */
    /*Bug:9923*/
    rrm_return_et     ret_val = RRM_SUCCESS;
    rrm_meas_config_info_t *p_rrm_meas_config_info = RRM_NULL;
    p_rrm_meas_config_info = rrm_cellm_get_meas_config_info(p_rrm_ue_ctx->cell_index);
    /*Bug:9923*/

    /*end supported_rat*/

    /* Soft Lock start - Global variable removal */
    rrm_cell_context_t    *p_cell_ctxt  = RRM_PNULL;
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_ctx->cell_index);
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        /* Fix for SPR 10310 start */
        return RRM_FAILURE;
        /* Fix for SPR 10310 end */
    }
    /*SPR 21365 fix start*/
    p_ncl_params                         = rrm_cellm_get_ncl_info(p_rrm_ue_ctx->cell_index);

    if (NULL == p_ncl_params)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "p_ncl_params is NULL for cell_index=%u",p_rrm_ue_ctx->cell_index);
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }

    intra_freq_neighbour_count           =  p_ncl_params->lte_ncl.num_valid_intra_freq_cell;
    starting_meas_config_count  =  rrm_count_measurements_for_ho_configured(p_rrm_ue_ctx);
    meas_config_count = starting_meas_config_count;
    /*SPR 21365 fix stop*/
    /* Soft Lock end - Global variable removal */

    RRM_MEMSET(p_meas_config, RRM_ZERO ,sizeof(rrm_meas_config_t));


    /*start supported_rat*/
    /*Find supported rat by supported_rat parameter
      configd from oam per qci basis and spid*/
    rrm_find_supported_rat(
            p_rrm_ue_ctx,
            &eutran_supported,
            &utran_supported,
            &grean_supported,
            /* SPR 11760 FIX START */
            &cdma_supported
            /* SPR 11760 FIX END */
            );
    /*end supported_rat*/
    // This block will be invoked when configuring Measurements for the UE
    // 1st time which will be sent as part of ERB_SETUP_RESP
    /* SPR 14239 Fix Start */
    /*SPR 21687 +-*/

    // This block will be invoked when configuring Measurements for the UE
    // 1st time which will be sent as part of ERB_SETUP_RESP
    if ((report_event == MEAS_REP_EVENT_NONE) &&
            /* Start fix for SPR 8391 */
            (p_rrm_ue_ctx->proximity_ind == RRM_FALSE))
        /* End fix for SPR 8391 */
    {
        if(!(p_rrm_ue_ctx->ho_params.bitmask & RRM_UE_CSFB_RECD)\
                && !(p_rrm_ue_ctx->ho_params.bitmask & \
                    RRM_UE_PRIMARY_MEAS_OBJ_CONFIGURED))
        {
            fill_intra_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, \
                    &index,
                    /*start supported_rat*/
                    eutran_supported,
                    /*end supported_rat*/
                    p_rrm_ue_ctx->cell_index
                    );

            /*changes done for limiting UE measurement*/
            fill_report_config_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index);
            /* START: CSR 00057390*/
            fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index, RRM_FALSE);
            fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);
            /* END: CSR 00057390*/
            p_rrm_ue_ctx->ho_params.bitmask |= \
                                               RRM_UE_PRIMARY_MEAS_OBJ_CONFIGURED;

            rrm_raise_event_meas_config_a1( p_rrm_ue_ctx );
            rrm_raise_event_meas_config_a2( p_rrm_ue_ctx );
            if (1==p_rrm_ue_ctx->is_a3_configured)
            {
                rrm_raise_event_meas_config_a3( p_rrm_ue_ctx );
            }
            RRM_UT_TRACE_EXIT();
        }
        else
        {
            /*SPR 21365 fix start*/
            while((index_rat < RRM_IRAT_TYPE) && (meas_config_count < (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE)))
                /*SPR 21365 fix stop*/
            {
                switch(get_next_priority_rat(index_rat, p_rrm_ue_ctx))
                {
                    /*Coverity_fix_start_62519*/
                    case RRM_IRAT_PRIORITY_UTRAN:
                        if ((RRM_CSFB_TGT_SELECT_DAHO ==
                                    p_rrm_meas_config_info->csfb_tgt_selection.utran_csfb_tgt_selection)&& 
                                (p_rrm_ue_ctx->ue_context_mod_req.bitmask & RRM_UEM_CTX_CSF_INDICATOR_PRESENT)&&
                                (RRM_ZERO == p_meas_config->meas_object_to_add_mod_list.count))
                        {
                            /* SPR 14239 Fix Start */
                            /* Check if UTRAN is restricted in forbidden inter rat list
                             * present in ho_restriction list */
                            /*SPR 21687 Start*/
                            if(is_ue_restricted_for_given_rat(p_rrm_ue_ctx, RRM_UTRAN_CELL))
                            {
                                break;
                            }
                            /*SPR 21687 End*/
                            /* SPR 14239 Fix End */
                            /* SPR 15336 Fix Start */
                            /* In case of CSFB to DAHO it is not required to send SRVCC IE in HO required, So when DAHO PCI 
                               needs to be validated it is required that ue_srvcc_criteria needs to be RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED */
                            p_rrm_ue_ctx->srvcc_config_info.ue_srvcc_criteria = RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED; 
                            /* SPR 15336 Fix End */

                            ret_val = rrm_ho_to_utra_daho(p_rrm_ue_ctx,RRM_ONE);			
                            if (ret_val == RRM_FAILURE)
                            {
                                if( RRM_FAILURE == 	 rrm_build_and_send_ue_connection_release_ind(
                                            p_rrm_ue_ctx->ue_index,
                                            RADIO_NETWORK_LAYER_TYPE,
                                            CS_FALLBACK_TRIGGERED,
                                            RRM_ONE,
                                            p_rrm_ue_ctx,
                                            /* SPR_10729_fix */
                                            p_rrm_ue_ctx->cell_index))
                                {
                                    RRM_TRACE(g_uem_log_on_off,
                                            p_rrm_uem_facility_name,
                                            RRM_ERROR,
                                            "[UE:%d]: Failure Received While Sending"
                                            " ue_connection_release_ind",
                                            p_rrm_ue_ctx->ue_index);
                                }
                                else
                                {
                                    if(RRM_FAILURE == rrm_ue_resource_clean_up(p_rrm_ue_ctx))
                                    {
                                        RRM_TRACE(g_uem_log_on_off,
                                                p_rrm_uem_facility_name,
                                                RRM_DETAILED,
                                                "ue_connection_release_ind sent, \
                                                Failed to clean up ue resources");
                                    }
                                }
                            }
                            /* Fix for SPR 10310 start */
                            /* Returning RRM failure as there is no need to send meas_config_req in this case */
                            return RRM_FAILURE;
                            /* Fix for SPR 10310 end */
                        }
                        is_utran_supported = RRM_TRUE;
                        break;
                    case RRM_IRAT_PRIORITY_GERAN:
                        /*26_March_Fix:Start */
                        if ((RRM_CSFB_TGT_SELECT_DAHO ==
                                    p_rrm_meas_config_info->csfb_tgt_selection.geran_csfb_tgt_selection)&&
                                (p_rrm_ue_ctx->ue_context_mod_req.bitmask & RRM_UEM_CTX_CSF_INDICATOR_PRESENT)&&
                                (RRM_ZERO == p_meas_config->meas_object_to_add_mod_list.count))
                        {
                            /* SPR 14239 Fix Start */
                            /* Check if GERAN is restricted in forbidden inter rat list
                             * present in ho_restriction list */
                            /*SPR 21687 Start*/
                            if(is_ue_restricted_for_given_rat(p_rrm_ue_ctx, RRM_GERAN_CELL))
                            {
                                break;
                            }
                            /*SPR 21687 End*/
                            /* SPR 14239 Fix End */
                            /* SPR 10349 End */

                            ret_val = rrm_ho_to_geran_daho(p_rrm_ue_ctx,RRM_ONE);
                            if (ret_val == RRM_FAILURE)
                            {
                                if( RRM_FAILURE ==       rrm_build_and_send_ue_connection_release_ind(
                                            p_rrm_ue_ctx->ue_index,
                                            RADIO_NETWORK_LAYER_TYPE,
                                            CS_FALLBACK_TRIGGERED,
                                            RRM_ONE,
                                            p_rrm_ue_ctx,
                                            /* SPR_10729_fix */
                                            p_rrm_ue_ctx->cell_index))
                                {
                                    RRM_TRACE(g_uem_log_on_off,
                                            p_rrm_uem_facility_name,
                                            RRM_ERROR,
                                            "[UE:%d]: Failure Received While Sending"
                                            " ue_connection_release_ind",
                                            p_rrm_ue_ctx->ue_index);
                                }
                                else
                                {
                                    if(RRM_FAILURE == rrm_ue_resource_clean_up(p_rrm_ue_ctx))
                                    {
                                        RRM_TRACE(g_uem_log_on_off,
                                                p_rrm_uem_facility_name,
                                                RRM_DETAILED,
                                                "ue_connection_release_ind sent, \
                                                Failed to clean up ue resources");
                                    }
                                }

                            }
                            /* Fix for SPR 10310 start */
                            /* Returning RRM failure as there is no need to send meas_config_req in this case */
                            return RRM_FAILURE;
                            /* Fix for SPR 10310 end */
                        }
                        /*26_March_Fix:End */
                        /*Bug:9923*/

                        is_geran_supported = RRM_TRUE;
                        break;
                    case RRM_IRAT_PRIORITY_CDMA2000:
                        /* SPR 11760 FIX START */
                        if((RRM_FALSE == p_rrm_ue_ctx->spid_found) && (RRM_FALSE == cdma_supported))
                        {
                            index_rat++;
                            continue;
                        }
                        else
                        {
                            is_cdma2000_supported = RRM_TRUE; 
                            break;
                        }
                        /* SPR 11760 FIX END */
                    default:
                        index_rat++;
                        continue;
                        /*Coverity_fix_end_62519*/
                }
                /* SPR 10349 Start */
                if(RRM_TRUE == meas_configured_until_daho)
                { 
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "All measurements configured before DAHO rat");
                    break;
                }
                /* SPR 10349 End */

                fill_inter_rat_freq_meas_config_object(
                        p_meas_config,
                        p_rrm_ue_ctx,
                        is_utran_supported,
                        is_geran_supported,
                        is_cdma2000_supported
                        );

                index_rat++;
                is_utran_supported = RRM_FALSE;
                is_geran_supported = RRM_FALSE;
                is_cdma2000_supported = RRM_FALSE;
                /*SPR 21365 fix start*/
                meas_config_count = starting_meas_config_count + p_meas_config->meas_object_to_add_mod_list.count; 
                /*SPR 21365 fix stop*/

            }
            /*SPR 21365 fix start*/
            if(RRM_ZERO == p_meas_config->meas_object_to_add_mod_list.count)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "No Meas Object is being configured");
                RRM_UT_TRACE_EXIT();
                return RRM_FAILURE;
            }   
            /*SPR 21365 fix stop*/




            /* If Feature Group indicator bit 15 - b1 event is set,
             * and atleast one of bit 22 - UTRAN or bit 23 - GERAN is set
             * then only create reporting event B1*/

                                /*Start:Bug 762*/
                                if((rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_FIFTEEN)) &&
                                        (((rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_TWENTY_TWO))|| 
                                          (rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_EIGHT))) ||
                                         (rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_TWENTY_THREE)) || 
                                         (rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_TWENTY_FOUR)) ||
                                         (rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_TWENTY_SIX)))) 
                                    /*End:Bug 762*/
                                { 
                                    /* configuring only RSCP for limiting UE measurement */
                                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                            "B1 event configured");	          
                                    fill_report_config_info_for_B1(p_meas_config, p_rrm_ue_ctx);
                                }
                                else
                                {
                                    RRM_TRACE(g_uem_log_on_off, 
                                            p_rrm_uem_facility_name, RRM_DETAILED,
                                            "B1 report config is not configured " 
                                            "due to feature group indicator is not supporting");
                                }
                                /* START: CSR 00057390*/
                                fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index, RRM_FALSE);
                                fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);
                                /* END: CSR 00057390*/

                                /* SPR 15260 start */
                                /* code deleted */
                                /* SPR 15260 end */
                                /* SPR 22184/SES-478 Fix Start */
                                for(meas_obj_count = RRM_ZERO; 
                                        (meas_obj_count < p_meas_config->meas_object_to_add_mod_list.count) && 
                                        (meas_obj_count < MAX_MEAS_OBJECT_ID);
                                        meas_obj_count++)
                                    /* SPR 22184/SES-478 Fix End */
                                {
                                    if(p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[meas_obj_count].
                                            meas_object.meas_object_cdma2000.cdma2000_type == RRM_RRC_TYPE_HRPD)
                                    {
                                        HRPD_MEAS_OBJ_CONFIGURED = RRM_TRUE;
                                        break;
                                    }
                                }
                                if((RRM_TRUE == HRPD_MEAS_OBJ_CONFIGURED) && ((p_rrm_ue_ctx->cdma2000_capabilities.is_cdma2000_present) &&
                                            /*Start:Bug 762*/
                                            (rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_TWENTY_SIX))))
                                    /*End:Bug 762*/
                                {
                                    fill_pre_registration_info_hrpd(p_meas_config, p_rrm_ue_ctx);

                                }
                                /* Start CSR: 00058590 */
                                fill_eutran_meas_config_of_anr_to_remove_list(p_meas_config, p_rrm_ue_ctx);
                                /* Set interfreq_interrat_meas_configured to TRUE to restrict SON ANR measurement */
                                p_rrm_ue_ctx->interfreq_interrat_meas_configured = RRM_TRUE;
                                /* End CSR: 00058590 */

                                /* Spr 18748 Fix Start */
                                if(RRM_TRUE == rrm_check_if_sec_meas_configured(p_rrm_ue_ctx))
                                {
                                    /* Spr 18748 Fix End */

                                    fill_eutran_meas_config_to_remove_list(
                                            p_meas_config, 
                                            p_rrm_ue_ctx);
                                }
        }
    }

    // This block will be invoked when configuring Measurements A4, A5 and A3 (based on OAM 
    // parameter) after receiving A2 for the UE that will be sent as part of MEAS_CONFIG_REQ
    /* Start fix for SPR 8391 */
    /* 
     ** Inserted check for proximity indication feature, RRM shall configure measurements in case 
     ** proximity ind API received with carrier freq of inter freq or UTRAN cell in entering condition.
     */ 

                                    /*spr_12273_fix_start*/
                                    /* SPR 13445 Fix Start */
                                    /* Spr 18748 Fix Start */
                                    if (((report_event == MEAS_REP_EVENT_A2) && (RRM_FALSE == rrm_check_if_sec_meas_configured(p_rrm_ue_ctx)))|| 
                                            /* Spr 18748 Fix End */
                                            (report_event == MEAS_REP_EVENT_FORCEFUL_A2) || 
                                            (p_rrm_ue_ctx->proximity_ind == RRM_TRUE))
                                        /* SPR 13445 Fix End */
                                        /*spr_12273_fix_end*/
                                        /* End fix for SPR 8391 */
                                    {
                                        /* SPR 15608 Start */
                                        fill_eutran_meas_config_of_anr_to_remove_list(p_meas_config, p_rrm_ue_ctx);
                                        /* SPR 15608 End */
                                        if (delayed_a3_enabled == RRM_ZERO && p_rrm_ue_ctx->is_a3_configured)
                                        {
                                            /* A3 is configured as part of Primary Meas Object configuration.
                                             * If A4 is not supported as per FGI then no need to configure
                                             * intra-freq meas object */
                                            /*Start:Bug 762*/
                                            if(!(rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_FOURTEEN)))
                                                /*End:Bug 762*/
                                            {
                                                RRM_TRACE(
                                                        g_uem_log_on_off, 
                                                        p_rrm_uem_facility_name, 
                                                        RRM_DETAILED,
                                                        "A4 reporting is not supported as indicated by FGI");
                                                configure_intra_meas_obj = RRM_FALSE;   
                                            }
                                        }
                                        /* Bug_13128_fix: Start */ 
                                        /* If Eutran not supported per qci as configured by OAM then no need to check for configuring A4 & A5 meas config */
                                        /* SPR 22310 CID 62519 Fix Start */
                                        if(RRM_TRUE == eutran_supported)
                                        {    
                                            if(configure_intra_meas_obj == RRM_TRUE)
                                            {
                                                /*SPR 21365 fix start*/
                                                /* SPR-22308 START */
                                                if ((meas_config_count < (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE)))
                                                    /* SPR-22308 END */
                                                {
                                                    meas_config_count++;
                                                    /* SPR_14564_fix: start */
                                                    /* Line deleted */
                                                    /* SPR_14564_fix: end */
                                                    fill_intra_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, &index,
                                                            /*start supported_rat*/
                                                            eutran_supported,
                                                            /*end supported_rat*/
                                                            p_rrm_ue_ctx->cell_index
                                                            );

                                                    rrm_raise_event_meas_config_a4( p_rrm_ue_ctx );
                                                    /* SPR_14564_fix: start */
                                                    /* Line deleted */
                                                    /* SPR_14564_fix: end */
                                                }
                                                else
                                                {
                                                    /* Coverity Fix 115989 */
                                                    /* Check removed which introduced under SPR 21365 */
                                                    if(!(intra_freq_neighbour_count >=RRM_ONE))
                                                    {
                                                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,
                                                            RRM_INFO, "Intra frequency object is not "
                                                            "configured as no intra freq neighbours "
                                                            "are configured in ncl list");
                                                    }
                                                }
                                                index++;
                                                /*SPR 21365 fix stop*/
                                            }

                                            /* FGI START*/
                                            /*Start:Bug 762*/
                                            if((rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_THIRTEEN)) &&
                                                    (rrm_is_set_fgi_bit(p_rrm_ue_ctx,RRM_TWENTY_FIVE)))
                                                /*End:Bug 762*/
                                            {
                                                /* SPR_14564_fix: start */
                                                if(RRM_TRUE == eutran_supported)
                                                    /* SPR_14564_fix: end */
                                                {
                                                    fill_inter_freq_meas_config_object(p_meas_config, p_rrm_ue_ctx, RRM_INTER_FREQ_FOR_HO);
                                                    /*SPR 21365 fix start*/
                                                    meas_config_count = starting_meas_config_count + p_meas_config->meas_object_to_add_mod_list.count; 
                                                    /*SPR 21365 fix stop*/

                                                    /* SPR_14564_fix: end */
                                                }
                                                else
                                                {
                                                    /*SPR 21365 fix start*/
                                                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                                            "inter Frequency Measurement Object is not configured " 
                                                            "There are no inter frequency neighbours are configured as in ncl list");
                                                    /*SPR 21365 fix stop*/
                                                }		    
                                            }
                                            else
                                            {
                                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                                        "inter Frequency Measurement Object is not configured " 
                                                        "due to feature group indicator is not supporting");
                                            }
                                        }
                                        /* SPR 22310 CID 62519 Fix End */
                                        else
                                        {
                                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                                    "Eutran not supported per QCI, hence not configuring A4 & A5 meas reporting."); 
                                        }
                                        /* Bug_13128_fix: End */
                                        /* Inter RAT BLR changes Started */

                                        if (!p_cell_ctxt->administrative_procedure_ongoing)
                                        {
                                            // HO Enhancement 3/12/2013
                                            /*SPR 21365 fix start*/
                                            while((index_rat < RRM_IRAT_TYPE) && (meas_config_count < (MAX_MEAS_OBJECT_ID - HO_MEAS_START_ID + RRM_ONE)))
                                                /*SPR 21365 fix stop*/
                                            {
                                                switch(get_next_priority_rat(index_rat, p_rrm_ue_ctx))
                                                {
                                                    case RRM_IRAT_PRIORITY_UTRAN:
                                                        /*Bug:9923*/
                                                        /*If utran is selected as priority rat and if already meas obj is configured for some other priority 				RAT. 
                                                         * RRM should not send rrm_ho_to_utra_daho
                                                         */
                                                    if ((RRM_CSFB_TGT_SELECT_DAHO ==
                                                                p_rrm_meas_config_info->csfb_tgt_selection.utran_csfb_tgt_selection)&& 
                                                            (p_rrm_ue_ctx->ue_context_mod_req.bitmask & RRM_UEM_CTX_CSF_INDICATOR_PRESENT)&&
                                                            (RRM_ZERO == p_meas_config->meas_object_to_add_mod_list.count))
                                                    {
                                                        /* SPR 14239 Fix Start */
                                                        /* Check if UTRAN is restricted in forbidden inter rat list
                                                         * present in ho_restriction list */
                                                        /*SPR 21687 Start*/
                                                        if(is_ue_restricted_for_given_rat(p_rrm_ue_ctx, RRM_UTRAN_CELL))
                                                        {
                                                            break;
                                                        }
                                                        /*SPR 21687 End*/
                                                        /* SPR 14239 Fix End */
                                                        /* SPR 15336 Fix Start */
                                                        /* In case of CSFB to DAHO it is not required to send SRVCC IE in HO required, So when DAHO PCI 
                                                           needs to be validated it is required that ue_srvcc_criteria needs to be RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED */
                                                        p_rrm_ue_ctx->srvcc_config_info.ue_srvcc_criteria = RRM_UE_SRVCC_IND_NOT_TO_BE_INCLUDED; 
                                                        /* SPR 15336 Fix End */
                                                        ret_val = rrm_ho_to_utra_daho(p_rrm_ue_ctx,RRM_ONE);			
                                                        if (ret_val == RRM_FAILURE)
                                                        {
                                                            if( RRM_FAILURE == 	 rrm_build_and_send_ue_connection_release_ind(
                                                                        p_rrm_ue_ctx->ue_index,
                                                                        RADIO_NETWORK_LAYER_TYPE,
                                                                        CS_FALLBACK_TRIGGERED,
                                                                        RRM_ONE,
                                                                        p_rrm_ue_ctx,
                                                                        /* SPR_10729_fix */
                                                                        p_rrm_ue_ctx->cell_index))
                                                            {
                                                                RRM_TRACE(g_uem_log_on_off,
                                                                        p_rrm_uem_facility_name,
                                                                        RRM_ERROR,
                                                                        "[UE:%d]: Failure Received While Sending"
                                                                        " ue_connection_release_ind",
                                                                        p_rrm_ue_ctx->ue_index);
                                                            }
                                                            else
                                                            {
                                                                if(RRM_FAILURE == rrm_ue_resource_clean_up(p_rrm_ue_ctx))
                                                                {
                                                                    RRM_TRACE(g_uem_log_on_off,
                                                                            p_rrm_uem_facility_name,
                                                                            RRM_DETAILED,
                                                                            "ue_connection_release_ind sent, \
                                                                            Failed to clean up ue resources");
                                                                }
                                                            }


                                                        }
                                                        /* Fix for SPR 10310 start */
                                                        /* Returning RRM failure as there is no need to send meas_config_req in this case */
                                                        return RRM_FAILURE;
                                                        /* Fix for SPR 10310 end */
                                                    }
                                                    is_utran_supported = RRM_TRUE;
                                                    break;
                                                    case RRM_IRAT_PRIORITY_GERAN:
                                                    /*26_March_Fix:Start */

                                                    if ((RRM_CSFB_TGT_SELECT_DAHO ==
                                                                p_rrm_meas_config_info->csfb_tgt_selection.geran_csfb_tgt_selection)&&
                                                            (p_rrm_ue_ctx->ue_context_mod_req.bitmask & RRM_UEM_CTX_CSF_INDICATOR_PRESENT)&&
                                                            (RRM_ZERO == p_meas_config->meas_object_to_add_mod_list.count))
                                                    {
                                                        /* SPR 14239 Fix Start */
                                                        /* Check if GERAN is restricted in forbidden inter rat list
                                                         * present in ho_restriction list */
                                                        /*SPR 21687 Start*/
                                                        if(is_ue_restricted_for_given_rat(p_rrm_ue_ctx, RRM_GERAN_CELL))
                                                        {
                                                            break;
                                                        }
                                                        /*SPR 21687 End*/
                                                        /* SPR 14239 Fix End */
                                                        ret_val = rrm_ho_to_geran_daho(p_rrm_ue_ctx,RRM_ONE);
                                                        if (ret_val == RRM_FAILURE)
                                                        {
                                                            if( RRM_FAILURE == rrm_build_and_send_ue_connection_release_ind(
                                                                        p_rrm_ue_ctx->ue_index,
                                                                        RADIO_NETWORK_LAYER_TYPE,
                                                                        CS_FALLBACK_TRIGGERED,
                                                                        RRM_ONE,
                                                                        p_rrm_ue_ctx,
                                                                        /* SPR_10729_fix */
                                                                        p_rrm_ue_ctx->cell_index))
                                                            {
                                                                RRM_TRACE(g_uem_log_on_off,
                                                                        p_rrm_uem_facility_name,
                                                                        RRM_ERROR,
                                                                        "[UE:%d]: Failure Received While Sending"
                                                                        " ue_connection_release_ind",
                                                                        p_rrm_ue_ctx->ue_index);
                                                            }
                                                            else
                                                            {
                                                                if (RRM_FAILURE == rrm_ue_resource_clean_up(p_rrm_ue_ctx))
                                                                {
                                                                    return RRM_FAILURE;
                                                                }
                                                            }


                                                        }
                                                        /* Fix for SPR 10310 start */ 
                                                        /* Returning RRM failure as there is no need to send meas_config_req in this case */
                                                        return RRM_FAILURE;
                                                        /* Fix for SPR 10310 end */
                                                    }

                                                    /*26_March_Fix:end */
                                                    /*Bug:9923*/

                                                    is_geran_supported = RRM_TRUE;
                                                    break;
                                                    case RRM_IRAT_PRIORITY_CDMA2000:
                                                    is_cdma2000_supported = RRM_TRUE; 
                                                    break;
                                                    default:
                                                    index_rat++;
                                                    continue;
                                                }
                                                fill_inter_rat_freq_meas_config_object(
                                                        p_meas_config,
                                                        p_rrm_ue_ctx,
                                                        is_utran_supported,
                                                        is_geran_supported,
                                                        is_cdma2000_supported
                                                        );

                                                index_rat++;
                                                is_utran_supported = RRM_FALSE;
                                                is_geran_supported = RRM_FALSE;
                                                is_cdma2000_supported = RRM_FALSE;
                                                /*SPR 21365 fix start*/
                                                meas_config_count = starting_meas_config_count + p_meas_config->meas_object_to_add_mod_list.count; 
                                                /*SPR 21365 fix stop*/



                                            }

                                            if(!(p_meas_config->bitmask & \
                                                        UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT))
                                            {
                                                /* No meas object to be configured so return */
                                                RRM_TRACE(
                                                        g_uem_log_on_off, 
                                                        p_rrm_uem_facility_name, 
                                                        RRM_DETAILED,
                                                        "No meas object to be added/modified");
                                                RRM_UT_TRACE_EXIT();
                                                /* Fix for SPR 10310 start */
                                                /* Returing RRM_SUCCESS to preserve the existing functionality */
                                                return RRM_SUCCESS;
                                                /* Fix for SPR 10310 end */
                                            }
                                            /* FGI END*/
                                        }
                                        /* Inter RAT BLR changes END */


                                        /*spr_12273_fix */

                                        /*CA HARDENING CHANGES 6.3.0*/
                                        fill_report_config_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index); 

                                        /* START: CSR 00057390*/
                                        fill_meas_report_ids_info(p_meas_config, p_rrm_ue_ctx,p_rrm_ue_ctx->cell_index, RRM_FALSE);

                                        fill_quantity_config_info (p_meas_config, p_rrm_ue_ctx);
                                        /* END: CSR 00057390*/
                                        /* For Limiting UE measurement */

                                        /*CA HARDENING CHANGES 6.3.0*/


                                        /*CA HAREDNING CHANGES 6.3.0 +-*/
                                        /* For Limiting UE measurement end */
                                        if ((1==p_rrm_ue_ctx->is_a3_configured) && (!p_cell_ctxt->administrative_procedure_ongoing))
                                        {
                                            rrm_raise_event_meas_config_a3( p_rrm_ue_ctx );
                                        }

                                        /* Start CSR: 00058590 */
                                        /* SPR 16041 start */
                                        /* Code deleted */
                                        /* SPR 16041 end */
                                        p_rrm_ue_ctx->interfreq_interrat_meas_configured = RRM_TRUE;
                                        /* End CSR: 00058590 */

                                        if (!p_cell_ctxt->administrative_procedure_ongoing)
                                        {
                                            rrm_raise_event_meas_config_a5( p_rrm_ue_ctx );
                                        }
                                    }

                                // This block will be invoked when removing Measurements A4, A5 and A3 (based on OAM 
                                // parameter) after receving A1 for the UE that will be sent as part of MEAS_CONFIG_REQ
                                    else if (report_event == MEAS_REP_EVENT_A1 && \
                                            /* Spr 18748 Fix Start */
                                            (RRM_TRUE == rrm_check_if_sec_meas_configured(p_rrm_ue_ctx)))
                                        /* Spr 18748 Fix End */
                                    {
                                        fill_eutran_meas_config_to_remove_list(p_meas_config, p_rrm_ue_ctx);
                                        /*SPR 17061 Start*/
                                        /* SPR 15397 Start */
                                        /* Release Measurement Gap(if configured) when A1 is received. */
                                        if(RRM_ZERO !=  p_rrm_ue_ctx->meas_gap_config.meas_gap_config_type)
                                        {
                                            p_meas_config->bitmask                              |= UE_ADM_MEAS_GAP_CONFIG_PRESENT;
                                            p_meas_config->meas_gap_config.bitmask              |= RRM_ZERO;
                                            p_meas_config->meas_gap_config.meas_gap_config_type  = RRM_ZERO;
                                            /*SPR_17047_START*/
                                            p_rrm_ue_ctx->meas_gap_sent_status = RRM_UNSET;
                                            /*SPR_17047_END*/

                                            /* SPR 16053 start */
                                            /* Code deleted */
                                            /* SPR 16053 end */
                                        }
                                        /* SPR 15397 End */
                                        /*SPR 17061 End*/
                                    }
                                /* SPR_15538_fix: Start */
                                    else if ((report_event == MEAS_REP_EVENT_FOR_CGI) ||
                                            (RRM_TRUE == p_rrm_ue_ctx->is_cgi_process_in_progress))
                                    {
                                        rrm_send_meas_config_req_for_cgi_rep_meas_id_removal(p_meas_config, p_rrm_ue_ctx);
                                    }
                                /* SPR_15538_fix: End */

                                /*fix 866 start*/
                                /* Start: SPR 9320 */
                                if(p_meas_config->bitmask)
                                {
                                    p_meas_config->s_measure = get_s_measure(p_rrm_ue_ctx->cell_index);
                                    p_meas_config->bitmask |= UE_ADM_MEAS_S_MEASURE_PRESENT;
                                }
                                /* End: SPR 9320 */
                                /*fix 866 start*/

                                RRM_UT_TRACE_EXIT();
                                /* Fix for SPR 10310 start */
                                return RRM_SUCCESS;
                                /* Fix for SPR 10310 end */
}


/******************************************************************************
 *  FUNCTION NAME: update_meas_config_status
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : none
 *  DESCRIPTION  : Updates the value of meas_status param after receiving a 
 *                 successful or failure respons
 *  RETURNS      : None
 ******************************************************************************/
    rrm_void_t
update_meas_config_status(
        rrm_ue_context_t              *p_rrm_ue_ctx,
        rrm_bool_et                   *p_is_ecid_meas_resp,
        U8                            *p_meas_id,
        rrm_meas_ecid_resp_et         *p_ecid_meas_resp_action
        )
{
    /* SPR 16053 start */
    /* SPR 17784 Start */
    /* Code Removed */
    /* SPR 17784 End */

    /*Coverity 88201 Fix Start*/
    rrm_ue_timer_buf_t              timer_buff;
    /* SPR 16053 end */
    RRM_MEMSET(&timer_buff, RRM_ZERO, sizeof(rrm_ue_timer_buf_t));
    /*Coverity 88201 Fix End*/

    RRM_UT_TRACE_ENTER();

    rrm_ue_meas_config_status_et meas_config_result = p_rrm_ue_ctx->
        meas_status;
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;
    U8   delayed_a3_enabled = get_delayed_a3_configure(p_rrm_ue_ctx->cell_index);
    YLNODE *p_node = RRM_PNULL;
    YLNODE *p_tmp_node = RRM_PNULL;

    /*SPR 17527 START*/
    U8    meas_object_id_hash_map[MAX_MEAS_OBJECT_ID] = {RRM_ZERO};;
    /*SPR 17527 END*/


    /* SPR 15728 Start */
    rrm_cell_context_t *p_cell_context = rrm_cellm_get_cell_context(p_rrm_ue_ctx->cell_index);
    if(RRM_PNULL == p_cell_context)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "cell context not found");
        RRM_UT_TRACE_EXIT();
        return;
    }
    /* SPR 15728 End */

    p_node = ylFirst(&(p_rrm_ue_ctx->meas_report_list));
    /*SPR 17527 START*/
    p_meas_report_node = (meas_report_info_node_t*)ylFirst(&(p_rrm_ue_ctx->meas_report_list));
    while (p_meas_report_node)
    {
        meas_object_id_hash_map[p_meas_report_node->meas_object_id]++;
        p_meas_report_node = (meas_report_info_node_t*)ylNext(&p_meas_report_node->sNode);
    }
    /*SPR 17527 END*/

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
            "meas_status: [%d], meas_type: [%d], reporting event: [%d]",
            meas_config_result, ((meas_report_info_node_t*)(&p_node))->meas_type, ((meas_report_info_node_t*)(&p_node))->reporting_event);
    if (meas_config_result == RRM_MEAS_CONFIG_RESP_SUCCESS)
    {
        while (p_node)
        {
            p_tmp_node = p_node;
            p_meas_report_node = (meas_report_info_node_t*)p_node;
            p_node = ylNext(p_node);

            if (p_meas_report_node->meas_status == RRM_MEAS_CONFIG_REQ_SENT)
            {
                p_meas_report_node->meas_status = RRM_MEAS_CONFIG_RESP_SUCCESS;
                if(MEAS_TYPE_ECID == p_meas_report_node->meas_type)
                {
                    *p_is_ecid_meas_resp        = RRM_TRUE;
                    *p_meas_id                  = p_meas_report_node->meas_id;
                    *p_ecid_meas_resp_action    = RRM_ECID_MEAS_CONTINUE;

                }
                else if(MEAS_TYPE_CA == p_meas_report_node->meas_type)
                {   
                    /*No need to do anything*/
                }
                /* RACH_OPTIMIZATION_CHANGES_START */
                else if(MEAS_TYPE_RACH == p_meas_report_node->meas_type)
                {
                    /*No updation is required as response is not for ECID, it is for RACH*/
                    *p_is_ecid_meas_resp        = RRM_FALSE;
                }
                /* RACH_OPTIMIZATION_CHANGES_END */
                else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_A3)
                {
                    /* SPR 15728 Start */
                    if((p_meas_report_node->carrier_earfcn !=
                                p_cell_context->ran_info.rf_params.rf_configurations.dl_earfcn)
                            || (delayed_a3_enabled) )
                        /* SPR 15728 End */
                    {
                        /*Start: BUG 974*/
                        if (RRM_CARRIER_REDIRECT_ON_UE != p_rrm_ue_ctx->action_triggered)
                        {
                            p_rrm_ue_ctx->ho_params.bitmask |= \
                                                               RRM_UE_SECONDARY_MEAS_OBJ_CONFIGURED;
                            /* SPR 17784 Start */
                            if ((p_cell_context->ho_parameters.ho_blind_timer_duration) 
                                    && (!p_rrm_ue_ctx->ho_params.blind_timer)) 
                            {
                                RRM_MEMSET (&timer_buff,RRM_ZERO,sizeof(rrm_ue_timer_buf_t));
                                timer_buff.timer_type = RRM_UE_BLIND_HO_TIMER;
                                timer_buff.ue_index = p_rrm_ue_ctx->ue_index;
				/* SPR 18757 Fix Start */
                                timer_buff.cell_index = p_rrm_ue_ctx->cell_index;
				/* SPR 18757 Fix End */

                                p_rrm_ue_ctx->ho_params.blind_timer = rrm_ue_start_timer (
                                        (p_cell_context->ho_parameters.ho_blind_timer_duration*RRM_SEC_TO_MILLI_SEC_CONV),
                                        &timer_buff, 
                                        sizeof(rrm_ue_timer_buf_t),
                                        RRM_FALSE);
                                if(p_rrm_ue_ctx->ho_params.blind_timer == RRM_PNULL)
                                {
                                    RRM_TRACE(g_uem_log_on_off, 
                                            p_rrm_uem_facility_name, 
                                            RRM_ERROR, 
                                            "Failed to start Blind  Timer for [UE:%u]",
                                            p_rrm_ue_ctx->ue_index);
                                }
                            }
                            else
                            {
                                RRM_TRACE(g_uem_log_on_off,
                                        p_rrm_uem_facility_name,
                                        RRM_ERROR,
                                        "Blind  Timer value for [UE:%u] is ZERO",
                                        p_rrm_ue_ctx->ue_index);
                            }
                            /* SPR 17784 End */

                        }
                        /*End: BUG 974*/
                    }
                }
                else if((p_meas_report_node->reporting_event >=  \
                            MEAS_REP_EVENT_A4) && (p_meas_report_node->reporting_event \
                                <=MEAS_REP_EVENT_A5))
                {
                    /* As of now there is no scenario where only A3/A4/A5/B2 
                     * will be configured. If configured then all the secondary 
                     * objects will be configured */
                    /*Start: BUG 974*/
                    if (RRM_CARRIER_REDIRECT_ON_UE != p_rrm_ue_ctx->action_triggered)
                    {
                        p_rrm_ue_ctx->ho_params.bitmask |= \
                                                           RRM_UE_SECONDARY_MEAS_OBJ_CONFIGURED;
                        /* SPR 17784 Start */
                        if ((p_cell_context->ho_parameters.ho_blind_timer_duration) 
                                && (!p_rrm_ue_ctx->ho_params.blind_timer)) 
                        {
                            RRM_MEMSET (&timer_buff,RRM_ZERO,sizeof(rrm_ue_timer_buf_t));
                            timer_buff.timer_type = RRM_UE_BLIND_HO_TIMER;
                            timer_buff.ue_index = p_rrm_ue_ctx->ue_index;
			    /* SPR 18757 Fix Start */ 
			    timer_buff.cell_index = p_rrm_ue_ctx->cell_index;
			    /* SPR 18757 Fix Start */
                            p_rrm_ue_ctx->ho_params.blind_timer = rrm_ue_start_timer (
                                    (p_cell_context->ho_parameters.ho_blind_timer_duration*RRM_SEC_TO_MILLI_SEC_CONV),
                                    &timer_buff, 
                                    sizeof(rrm_ue_timer_buf_t),
                                    RRM_FALSE);
                            if(p_rrm_ue_ctx->ho_params.blind_timer == RRM_PNULL)
                            {
                                RRM_TRACE(g_uem_log_on_off, 
                                        p_rrm_uem_facility_name, 
                                        RRM_ERROR, 
                                        "Failed to start Blind  Timer for [UE:%u]",
                                        p_rrm_ue_ctx->ue_index);
                            }
                        }
                        else
                        {
                            /* SPR 21251 +- */
                            /*RRM_TRACE(g_uem_log_on_off,
                                    p_rrm_uem_facility_name,
                                    RRM_ERROR,
                                    "Blind  Timer value for [UE:%u] is ZERO",
                                    p_rrm_ue_ctx->ue_index);*/
                        }
                        /* SPR 17784 End */

                    }
                    /*End: BUG 974*/
                }
                else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_B1)
                {
                    p_rrm_ue_ctx->ho_params.bitmask |= \
                                                       RRM_UE_B1_MEAS_OBJ_CONFIGURED; 
                    /* SPR 17784 Start */
                    if ((p_cell_context->ho_parameters.ho_blind_timer_duration) 
                            && (!p_rrm_ue_ctx->ho_params.blind_timer)) 
                    {
                        RRM_MEMSET (&timer_buff,RRM_ZERO,sizeof(rrm_ue_timer_buf_t));
                        timer_buff.timer_type = RRM_UE_BLIND_HO_TIMER;
                        timer_buff.ue_index = p_rrm_ue_ctx->ue_index;
			/* SPR 18757 Fix Start */
			timer_buff.cell_index = p_rrm_ue_ctx->cell_index;
			/* SPR 18757 Fix End */

                        p_rrm_ue_ctx->ho_params.blind_timer = rrm_ue_start_timer (
                                (p_cell_context->ho_parameters.ho_blind_timer_duration*RRM_SEC_TO_MILLI_SEC_CONV),
                                &timer_buff, 
                                sizeof(rrm_ue_timer_buf_t),
                                RRM_FALSE);
                        if(p_rrm_ue_ctx->ho_params.blind_timer == RRM_PNULL)
                        {
                            RRM_TRACE(g_uem_log_on_off, 
                                    p_rrm_uem_facility_name, 
                                    RRM_ERROR, 
                                    "Failed to start Blind  Timer for [UE:%u]",
                                    p_rrm_ue_ctx->ue_index);
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_rrm_uem_facility_name,
                                RRM_ERROR,
                                "Blind  Timer value for [UE:%u] is ZERO",
                                p_rrm_ue_ctx->ue_index);
                    }
                    /* SPR 17784 End */

                }
                else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_B2)
                {
                    p_rrm_ue_ctx->ho_params.bitmask |= \
                                                       RRM_UE_B2_MEAS_OBJ_CONFIGURED;
                    /* SPR 17784 Start */
                    if ((p_cell_context->ho_parameters.ho_blind_timer_duration) 
                            && (!p_rrm_ue_ctx->ho_params.blind_timer)) 
                    {
                        RRM_MEMSET (&timer_buff,RRM_ZERO,sizeof(rrm_ue_timer_buf_t));
                        timer_buff.timer_type = RRM_UE_BLIND_HO_TIMER;
                        timer_buff.ue_index = p_rrm_ue_ctx->ue_index;
			/* SPR 18757 Fix Start */
			timer_buff.cell_index = p_rrm_ue_ctx->cell_index;
			/* SPR 18757 Fix End */
                        p_rrm_ue_ctx->ho_params.blind_timer = rrm_ue_start_timer (
                                (p_cell_context->ho_parameters.ho_blind_timer_duration*RRM_SEC_TO_MILLI_SEC_CONV),
                                &timer_buff, 
                                sizeof(rrm_ue_timer_buf_t),
                                RRM_FALSE);
                        if(p_rrm_ue_ctx->ho_params.blind_timer == RRM_PNULL)
                        {
                            RRM_TRACE(g_uem_log_on_off, 
                                    p_rrm_uem_facility_name, 
                                    RRM_ERROR, 
                                    "Failed to start Blind  Timer for [UE:%u]",
                                    p_rrm_ue_ctx->ue_index);
                        }
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off,
                                p_rrm_uem_facility_name,
                                RRM_ERROR,
                                "Blind  Timer value for [UE:%u] is ZERO",
                                p_rrm_ue_ctx->ue_index);
                    }
                    /* SPR 17784 End */

                }
                /* SPR 16053 start */
                else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_FOR_CGI)
                {
                    if ((RRM_CGI_REP_CAUSE_NEW_PCI == p_rrm_ue_ctx->cgi_reporting_triggered_for_meas_event) ||
                            (RRM_CGI_REP_CAUSE_PCI_CONFUSION == p_rrm_ue_ctx->cgi_reporting_triggered_for_meas_event))
                    {
                        /* SPR 17784 Start */
                        if(p_rrm_ue_ctx->ho_params.cgi_timer)
                        {
                            /* Stop CGI Timer */
                            RRM_TRACE (g_uem_log_on_off, p_rrm_uem_facility_name,
                                    RRM_DETAILED, "Stopping CGI Timer & eNb send CGI req for strongest cell"); 
                            rrm_ue_stop_timer(p_rrm_ue_ctx->ho_params.cgi_timer);
                            p_rrm_ue_ctx->ho_params.cgi_timer = RRM_NULL;
                        }

                        /* start timer to guard the procedure of CGI which
                         * is triggered either due to the new PCI reported
                         * in the measurement report or due to the PCI
                         * confusion occured during the measurement
                         * reporting */

                        if ((p_cell_context->ho_parameters.cgi_timer_duration) 
                                && (!p_rrm_ue_ctx->ho_params.cgi_timer)) 
                        {
                            timer_buff.timer_type = RRM_UE_CGI_TIMER;
                            timer_buff.ue_index = p_rrm_ue_ctx->ue_index;
                            timer_buff.phy_cell_id =  p_rrm_ue_ctx->pci_for_which_cgi_required;
                            timer_buff.earfcn = p_rrm_ue_ctx->cgi_trigerred_for_earfcn;
			    /* SPR 18757 Fix Start */
			    timer_buff.cell_index = p_rrm_ue_ctx->cell_index;
			    /* SPR 18757 Fix End */

                            p_rrm_ue_ctx->ho_params.cgi_timer = rrm_ue_start_timer (
                                    (p_cell_context->ho_parameters.ho_blind_timer_duration * RRM_SEC_TO_MILLI_SEC_CONV),
                                    &timer_buff, 
                                    sizeof(rrm_ue_timer_buf_t),
                                    RRM_FALSE);

                            if(!p_rrm_ue_ctx->ho_params.cgi_timer)
                            {
                                RRM_TRACE(g_uem_log_on_off, 
                                        p_rrm_uem_facility_name, 
                                        RRM_ERROR, 
                                        "Failed to start CGI Timer for [UE:%u]",
                                        p_rrm_ue_ctx->ue_index);
                            }
                            else
                            {
                                RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name, 
                                        RRM_ERROR,"CGI Timer for [UE:%u] is started",
                                        p_rrm_ue_ctx->ue_index);
                            }
                            /* SPR 17784 End */
                        }
                    }
                }

                /* SPR 16053 end */

            }
            else if(p_meas_report_node->meas_status == RRM_MEAS_CONFIG_TO_DELETE)
            {
                if(MEAS_TYPE_ECID == p_meas_report_node->meas_type)
                {
                    *p_is_ecid_meas_resp        = RRM_TRUE;
                    *p_meas_id                  = p_meas_report_node->meas_id;
                    *p_ecid_meas_resp_action    = RRM_ECID_MEAS_DELETE;
                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,REP_CONFIG, \
                            p_meas_report_node->report_config_id);
                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                            p_meas_report_node->meas_id);
                }
                else
                {
                    if((p_meas_report_node->reporting_event >=  \
                                MEAS_REP_EVENT_A3) && (p_meas_report_node->reporting_event \
                                    <=MEAS_REP_EVENT_A5))
                    {
                        /* As of now there is no scenario where only A3/A4/A5 
                         * will be deleted. If deleted then all the secondary 
                         * objects will be deleted */
                        p_rrm_ue_ctx->ho_params.bitmask &= \
                                                           ~(RRM_UE_SECONDARY_MEAS_OBJ_CONFIGURED);
                    }
                    else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_B2)
                    {
                        p_rrm_ue_ctx->ho_params.bitmask &= \
                                                           ~(RRM_UE_B2_MEAS_OBJ_CONFIGURED);
                    }
                    else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_B1)
                    {
                        p_rrm_ue_ctx->ho_params.bitmask &= \
                                                           ~(RRM_UE_B1_MEAS_OBJ_CONFIGURED);
                    }
                    /* SPR 15698 Start */
                    else if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_FOR_CGI)
                    {
                        p_rrm_ue_ctx->meas_config_sent_bitmask &= ~(REPORT_CONFIG_EUTRA_CGI_SENT);
                    }
                    /* SPR 15698 End */
                    if ((p_meas_report_node->reporting_event >= MEAS_REP_EVENT_A5)&&\
                            (p_meas_report_node->reporting_event <= \
                             MEAS_REP_EVENT_B2))
                    {
                        rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_CONFIG,\
                                p_meas_report_node->meas_object_id);
                    }
                    /*SPR 17527 START*/
                    if( meas_object_id_hash_map[p_meas_report_node->meas_object_id] == RRM_ONE)
                    {
                        rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_CONFIG,\
                                p_meas_report_node->meas_object_id);
                    }
                    /*SPR 17527 END*/
                    /* SPR 16042 start */
                    if (p_meas_report_node->report_config_id >= HO_MEAS_START_ID)
                    {

                        if(p_rrm_ue_ctx->report_config_id_list[\
                                p_meas_report_node->report_config_id - HO_MEAS_START_ID])
                        {
                            rrm_uem_deallocate_used_id(p_rrm_ue_ctx,REP_CONFIG, \
                                    p_meas_report_node->report_config_id);
                        }
                    }
                    else
                    {
                        rrm_uem_remove_cgi_report_config(p_rrm_ue_ctx, p_meas_report_node->report_config_id);
                    }
                    if (p_meas_report_node->meas_id >= HO_MEAS_START_ID)
                    {
                        rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                                p_meas_report_node->meas_id);
                    }
                    /* SPR 16042 end */
                    /*spr_12273_fix*/
                    /* SPR 13445 Fix Start */
                    if((p_meas_report_node->reporting_event == MEAS_REP_EVENT_A2) ||
                            (p_meas_report_node->reporting_event == MEAS_REP_EVENT_FORCEFUL_A2))
                        /* SPR 13445 Fix End */
                    {
                        rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_CONFIG, \
                                p_meas_report_node->report_config_id);
                    }

                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                            p_meas_report_node->meas_id);

                }
                ylDelete(&(p_rrm_ue_ctx->meas_report_list),p_tmp_node);
                /* SPR 15608 Start */
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                        " Deleting Meas ID : %d",p_meas_report_node->meas_id);
                /* SPR 15608 End */
                RRM_MEM_FREE(p_tmp_node);
            }
        }
    }
    else if(meas_config_result ==  RRM_MEAS_CONFIG_RESP_FAILED)
    {
        while (p_node)
        { 
            p_tmp_node = p_node;
            p_meas_report_node = (meas_report_info_node_t*)p_node;

            p_node = ylNext(p_node);

            if (p_meas_report_node->meas_status == RRM_MEAS_CONFIG_TO_DELETE)
            {
                if(MEAS_TYPE_ECID == p_meas_report_node->meas_type)
                {
                    *p_is_ecid_meas_resp        = RRM_TRUE;
                    *p_meas_id                  = p_meas_report_node->meas_id;
                    *p_ecid_meas_resp_action    = RRM_ECID_MEAS_DELETE_FAILED;
                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,REP_CONFIG, \
                            p_meas_report_node->report_config_id);
                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                            p_meas_report_node->meas_id);
                    ylDelete(&(p_rrm_ue_ctx->meas_report_list),p_tmp_node);
                    rrm_mem_free(p_tmp_node);
                }
                else
                {
                    p_meas_report_node->meas_status = RRM_MEAS_CONFIG_RESP_SUCCESS;
                    /* SPR 16053 start */
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "Not deleting measId: %d because of procedure failure",
                            p_meas_report_node->meas_id);
                    /* SPR 16053 end */

                }
            }
            else if (p_meas_report_node->meas_status == RRM_MEAS_CONFIG_REQ_SENT)
            {
                if(MEAS_TYPE_ECID == p_meas_report_node->meas_type)
                {
                    *p_is_ecid_meas_resp        = RRM_TRUE;
                    *p_meas_id                  = p_meas_report_node->meas_id;
                    *p_ecid_meas_resp_action    = RRM_ECID_MEAS_FAILED;
                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,REP_CONFIG, \
                            p_meas_report_node->report_config_id);
                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                            p_meas_report_node->meas_id);
                }
                else
                {
                    /* SPR 15797 Start */
                    if (p_rrm_ue_ctx->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT)
                    {
                        p_rrm_ue_ctx->a2_trigger_quantity_bitmask &= ~(THRESHOLD_EUTRA_RSRP_PRESENT);
                    }
                    if (p_rrm_ue_ctx->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT)
                    {
                        p_rrm_ue_ctx->a2_trigger_quantity_bitmask &= ~(THRESHOLD_EUTRA_RSRQ_PRESENT);
                    }
                    /* SPR 15797 End */
                    /*SPR 17527 START*/
                    if( meas_object_id_hash_map[p_meas_report_node->meas_object_id] == RRM_ONE)
                    {
                        rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_CONFIG,\
                                p_meas_report_node->meas_object_id); 
                    }
                    /*SPR 17527 END*/
                    /* SPR 16042 start */
                    if (p_meas_report_node->report_config_id >= HO_MEAS_START_ID)
                    {

                        if(p_rrm_ue_ctx->report_config_id_list[\
                                p_meas_report_node->report_config_id - HO_MEAS_START_ID])
                        {
                            rrm_uem_deallocate_used_id(p_rrm_ue_ctx,REP_CONFIG, \
                                    p_meas_report_node->report_config_id);
                        }
                    }
                    /* SPR 16053 start */
                    if(p_meas_report_node->reporting_event == MEAS_REP_EVENT_FOR_CGI)
                    {
                        /* Spr 18488 Changes Start */
                        rrm_delete_entry_in_ho_cgi_pending_list(p_rrm_ue_ctx,p_rrm_ue_ctx->pci_for_which_cgi_required,p_rrm_ue_ctx->cgi_trigerred_for_earfcn);
                        /* Spr 18488 Changes End */ 

                        if ((RRM_CGI_REP_CAUSE_NEW_PCI == p_rrm_ue_ctx->cgi_reporting_triggered_for_meas_event) ||
                                (RRM_CGI_REP_CAUSE_PCI_CONFUSION == p_rrm_ue_ctx->cgi_reporting_triggered_for_meas_event))
                        {
                            if(RRM_CGI_REP_CAUSE_PCI_CONFUSION == p_rrm_ue_ctx->cgi_reporting_triggered_for_meas_event)
                            {
                                p_rrm_ue_ctx->cgi_trigerred_for_earfcn = RRM_ZERO;
                                p_rrm_ue_ctx->rrm_cgi_triggered_for_rat_type = NO_RAT_CONFIGURED;
                            }
                            p_rrm_ue_ctx->cgi_reporting_triggered_for_meas_event = RRM_CGI_REP_CAUSE_MAX;
                            p_rrm_ue_ctx->pci_for_which_cgi_required = INVALID_PHYSICAL_CELL_ID;
                        }
                    }
                    /* SPR 16053 end */
                    if (p_meas_report_node->meas_id >= HO_MEAS_START_ID)
                    {
                        rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                                p_meas_report_node->meas_id);
                    }
                    /* SPR 16042 end */

                    rrm_uem_deallocate_used_id(p_rrm_ue_ctx,MEAS_REPORT, \
                            p_meas_report_node->meas_id);
                }
                /* SPR 21364 Fix start */
                if((p_meas_report_node->reporting_event == MEAS_REP_EVENT_A4) && (p_meas_report_node->meas_type == MEAS_TYPE_CA))
                {
                    p_rrm_ue_ctx->ca_data.a4_recv_for_ca = RRM_FALSE;
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "a4_recv_for_ca is updated as [%d] if the meas_type is CA",p_rrm_ue_ctx->ca_data.a4_recv_for_ca);
                }    
                /* SPR 21364 Fix stop */
                ylDelete(&(p_rrm_ue_ctx->meas_report_list),p_tmp_node);
                RRM_MEM_FREE(p_tmp_node);
            }
        }
    }
    p_rrm_ue_ctx->meas_status = RRM_MEAS_CONFIG_NULL;

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_ue_get_configured_meas_event
 * Inputs         : meas_id received in meas result ind message and fetched ue context.
 * Outputs        : Trigger Quantity (RSRP or RSRQ based) on success 
 * Returns        :  Measurment event if config object is found 
 *                   otherwise MEAS_REP_EVENT_NONE
 * Description    : Retrieves the configured meas  event and the associated trigger quantity.
 ****************************************************************************/
/* SPR_15712_Fix: Start */
/* SPR 16406 8SEP start */
    rrmc_meas_report_event_et
rrm_ue_get_configured_meas_event (
        U8                        meas_id,         /* IN  MeasID obtained in meas res ind */
        rrm_ue_context_t*         p_ue_context) /* IN  ue Context Pointer */
    /* SPR_15712_Fix: End */
    /* SPR 16406 8SEP end */
{
    RRM_UT_TRACE_ENTER();

    meas_report_info_node_t *p_meas_report_info = RRM_PNULL;
    rrmc_meas_report_event_et    meas_rep_event = MEAS_REP_EVENT_NONE;

    /* SPR 16406 8SEP start */
    p_meas_report_info = (meas_report_info_node_t*)ylFirst(&(p_ue_context->meas_report_list));

    while (p_meas_report_info) 
    {
        if (p_meas_report_info->meas_id == meas_id)
        {
            /*Coverity 93549 Fix Start*/
            p_ue_context->trigger_quantity = (rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity;
            /*Coverity 93549 Fix End*/
            p_ue_context->meas_rep_event = (U8)p_meas_report_info->reporting_event;
            p_ue_context->carrier_earfcn = p_meas_report_info->carrier_earfcn;
            p_ue_context->band_indicator = p_meas_report_info->band_indicator;
            meas_rep_event = (rrmc_meas_report_event_et)p_ue_context->meas_rep_event;/*cov 93552 +-*/
            RRM_TRACE(g_uem_log_on_off, 
                    p_rrm_uem_facility_name, 
                    RRM_INFO, 
                    "Meas report event[%s], Meas_id [%d] and trigger_quantity [%s], Carrier_EARFCN [%d], BandIndicator[%d]",
                    rrm_map_meas_event(p_ue_context->meas_rep_event), meas_id,
                    rrm_map_trigger_quantity(p_ue_context->trigger_quantity), p_ue_context->carrier_earfcn, p_ue_context->band_indicator);
            break;
        }
        p_meas_report_info = (meas_report_info_node_t*)ylNext(&p_meas_report_info->sNode);
    }
    /* SPR 16406 8SEP end */

    /* SPR 16406 8SEP start */
    do
    {
        if (RRM_PNULL != p_meas_report_info)
        {
            /* SPR 16406 8SEP end */
            /* Fix start for 7750 */
            if(meas_rep_event == MEAS_REP_EVENT_A3)
            {
                if ((rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity == RRM_RRC_RSRP)
                {
                    p_ue_context->a3_trigger_quantity_bitmask |= THRESHOLD_EUTRA_RSRP_PRESENT; 
                }
                else if((rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity == RRM_RRC_RSRQ)
                {
                    p_ue_context->a3_trigger_quantity_bitmask |= THRESHOLD_EUTRA_RSRQ_PRESENT;
                }
            }
            /* Fix end for 7750 */
            /* SPR 16041 start */
            /* To Handle only 1 A2 measurement report with RSRP or RSRQ comes */
            /* SPR 16041 end */
            else if (meas_rep_event == MEAS_REP_EVENT_A2)
            {
                if ((rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity == RRM_RRC_RSRP)
                {
                    if (p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT)
                    {
                        /* SPR 16041 start */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "EVENT_A2 received with trigger quantity RSRP followed by RSRP" 
                                " So ignoring Meas ID : %d", meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                        /* SPR 16041 end */
                    }
                    /* SPR 16041 start */
                    if (!(p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT))
                    {
                        p_ue_context->a2_trigger_quantity_bitmask |= THRESHOLD_EUTRA_RSRP_PRESENT;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "EVENT_A2 received with trigger quantity RSRP followed by RSRQ" 
                                " So ignoring Meas ID : %d", meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                    /* SPR 16041 end */
                    /* SPR 16041 start */
                    /* Code deleted */
                    /* SPR 16041 end */
                }
                else if ((rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity == RRM_RRC_RSRQ)
                {
                    if (p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT)
                    {
                        /* SPR 16041 start */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "EVENT_A2 received with trigger quantity RSRQ followed by RSRQ"
                                " So ignoring Meas ID : %d", meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                        /* SPR 16041 end */
                    }
                    /* SPR 16041 start */
                    if (!(p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT))
                    {
                        p_ue_context->a2_trigger_quantity_bitmask |= THRESHOLD_EUTRA_RSRQ_PRESENT;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "EVENT_A2 received with trigger quantity RSRQ followed by RSRP" 
                                " So ignoring Meas ID : %d", meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                    /* SPR 16041 end */
                }
            } 
            /* SPR 16041 start */
            /* To validate A1 received after A1 with same trigger quantity */
            /* A1 should be handled only after receiving A2 with same trigger quantity */
            /* SPR 16041 end */

            else if (meas_rep_event == MEAS_REP_EVENT_A1)
            {
                if ((rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity == RRM_RRC_RSRP)
                {

                    if ( p_ue_context->a1_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "EVENT_A1 received with trigger quantity RSRP followed by RSRP"
                                " So ignoring Meas ID : %d",meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }

                    /* Fix start for 7750 */
                    if(!(p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT) &&
                            !(p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT))
                    {         
                        if ( p_ue_context->a3_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT)
                        {
                            p_ue_context->a1_trigger_quantity_bitmask = RRM_ZERO ;
                            p_ue_context->a3_trigger_quantity_bitmask = RRM_ZERO ;
                            /* SPR 16406 8SEP start */
                            break;
                            /* SPR 16406 8SEP end */
                        }
                        else if( p_ue_context->a3_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT)    
                        {
                            p_ue_context->a1_trigger_quantity_bitmask |=  THRESHOLD_EUTRA_RSRP_PRESENT;
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                    "Event A1 received with trigger quantity RSRP, But event A3"
                                    "received for RSRQ, So dropping meas_id : %d", meas_id);
                            meas_rep_event =  MEAS_REP_EVENT_NONE;
                            /* SPR 16406 8SEP start */
                            break;
                            /* SPR 16406 8SEP end */
                        }
                    }         
                    /* Fix end for 7750 */

                    /* SPR 16041 start */
                    /* Code deleted */
                    /* SPR 16041 end */
                    else if (p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT)
                    {
                        p_ue_context->a2_trigger_quantity_bitmask = RRM_ZERO;
                        p_ue_context->a1_trigger_quantity_bitmask = RRM_ZERO;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Event A1 received with trigger quantity RSRP with out any A2 report, So dropping"
                                "the Meas_ID : %d",meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                }
                else if ((rrm_trigger_quantity_et)p_meas_report_info->trigger_quantity == RRM_RRC_RSRQ)
                {
                    if ( p_ue_context->a1_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "EVENT_A1 received with trigger quantity RSRQ followed by RSRQ"
                                " So ignoring Meas ID : %d",meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }

                    /* Fix start for 7750 */
                    if(!(p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT) &&
                            !(p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT))
                    {
                        if ( p_ue_context->a3_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT)
                        {
                            p_ue_context->a1_trigger_quantity_bitmask = RRM_ZERO ;
                            p_ue_context->a3_trigger_quantity_bitmask = RRM_ZERO ;
                            /* SPR 16406 8SEP start */
                            break;
                            /* SPR 16406 8SEP end */
                        }
                        else if( p_ue_context->a3_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRP_PRESENT)    
                        {
                            p_ue_context->a1_trigger_quantity_bitmask |=  THRESHOLD_EUTRA_RSRQ_PRESENT;
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                    "Event A1 received with trigger quantity RSRQ, But event A3"
                                    "received for RSRP, So dropping meas_id : %d", meas_id);
                            meas_rep_event =  MEAS_REP_EVENT_NONE;
                            /* SPR 16406 8SEP start */
                            break;
                            /* SPR 16406 8SEP end */
                        }      
                    }   
                    /* Fix end for 7750 */

                    /* SPR 16041 start */
                    /* Code deleted */
                    /* SPR 16041 end */
                    else if (p_ue_context->a2_trigger_quantity_bitmask & THRESHOLD_EUTRA_RSRQ_PRESENT)
                    {
                        p_ue_context->a2_trigger_quantity_bitmask = RRM_ZERO;
                        p_ue_context->a1_trigger_quantity_bitmask = RRM_ZERO;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                "Event A1 received with trigger quantity RSRQ with out any A2 report, So dropping"
                                "the Meas_ID : %d",meas_id);
                        meas_rep_event =  MEAS_REP_EVENT_NONE;
                        /* SPR 16406 8SEP start */
                        break;
                        /* SPR 16406 8SEP end */
                    }

                }
            }
            /* SPR 16406 8SEP start */
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR, "No event found for measId = %u", meas_id);
        }
    }while (RRM_ZERO);
    /* SPR 16406 8SEP end */

    /* SPR 16406 8SEP start */
    if (meas_rep_event == MEAS_REP_EVENT_NONE)
    {
        /*Coverity 93551 Fix Start*/
        p_ue_context->trigger_quantity = (rrm_trigger_quantity_et)RRM_ZERO;
        /*Coverity 93551 Fix End*/
        p_ue_context->meas_rep_event = RRM_ZERO;
        p_ue_context->carrier_earfcn = RRM_ZERO;
        p_ue_context->band_indicator = RRM_ZERO;
    }
    /* SPR 16406 8SEP end */
    RRM_UT_TRACE_EXIT();
    return meas_rep_event;
}
/****************************************************************************
 * Function Name  : rrm_get_cell_specific_info
 * Inputs         : physical cell ID of the neighbour,meas_id
 * Outputs        : Configured parameter values for the physical cell  
 * Returns        : TRUE if physical cell was present in neighbour list otherwise FALSE
 * Description    : Retrieves the configured neighbour information and detects 
 *                  PCI confusion. 
 ****************************************************************************/
rrm_bool_et
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
        )
{
    rrm_bool_et phy_cell_found_in_ncl   = RRM_FALSE;
    U8          valid_cells       = RRM_NULL;
    U8          cell_count        = RRM_NULL;
    U8          is_meas_id_found  = RRM_NULL;
    meas_report_info_node_t
        *p_meas_report_info = RRM_PNULL;
    lte_ncl_t    *p_lte_ncl       = RRM_PNULL;
    rf_params_t  *p_rf_params = RRM_PNULL; 
    rrm_bool_et match_intra_freq = RRM_FALSE;
    /*+- SPR 15603*/
    RRM_UT_TRACE_ENTER();
    rrm_common_params_for_eutra_t
        *p_common_params_for_eutra  = RRM_PNULL;

    p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                (p_rrm_ue_context->cell_index);

    /* SPR 15232 start */
    *p_pci_confusion_detected = RRM_FALSE;
    /* SPR 15232 end */
    /*SPR 15525 Fix Start*/ 
    *p_pci_blacklisted = RRM_FALSE;
    /*SPR 15525 Fix End*/

    YLNODE *p_node = RRM_PNULL;

    p_node = ylFirst(&(p_rrm_ue_context->meas_report_list));

    while (p_node) {
        p_meas_report_info = (meas_report_info_node_t*) p_node;
        if (p_meas_report_info->meas_id == meas_id) {
            p_meas_data->thresh1 = p_meas_report_info->thresh1;
            p_meas_data->thresh2 = p_meas_report_info->thresh2;
            p_meas_data->offset = p_meas_report_info->offset;
            /* SPR 15674 Start */
            p_meas_data->carr_earfcn = p_meas_report_info->carrier_earfcn;
            /* SPR 15674 End */
            /* SPR +- 15603 */
            is_meas_id_found       = RRM_ONE;
            break;
        }
        p_node = ylNext(p_node);
    }
    if (is_meas_id_found == RRM_NULL)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "measurement id:%d not yet configured",meas_id);
        return RRM_FALSE;
    }       
    /* Fix for Event A4 for administrative process ongoing */
    /*SPR 15603 Start Removing if*/
    /* Coverity_fix_88255_start */
    p_rf_params = rrm_cellm_get_rf_params(p_rrm_ue_context->cell_index); 

    if (p_meas_report_info->carrier_earfcn == 
            p_rf_params->rf_configurations.dl_earfcn)
    {
        match_intra_freq = RRM_TRUE;
    }

    p_lte_ncl = rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
    if(RRM_PNULL != p_lte_ncl)
    {

        p_lte_ncl = rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
        if(RRM_PNULL != p_lte_ncl)
        {

            if(match_intra_freq == RRM_TRUE)
            {
                valid_cells = p_lte_ncl-> num_valid_intra_freq_cell;

                for(cell_count=RRM_ZERO; cell_count < valid_cells; cell_count++)
                {
                    /*SPR 15525 Fix Start - If conditions shuffled*/
                    if (phy_cell_id == p_lte_ncl->intra_freq_cells[cell_count].phy_cell_id)
                    {
                        /* Fix for CSR 58972 start*/
                        if (RRM_TRUE != phy_cell_found_in_ncl)
                        {
                            p_meas_data->freq_specific_offset = p_lte_ncl->intra_freq_cells[cell_count].
                                q_offset;
                            p_meas_data->cell_specific_offset = p_lte_ncl->intra_freq_cells[cell_count].
                                cio;
                            p_meas_data->hysteresis           = p_common_params_for_eutra->hysteresis;
                            phy_cell_found_in_ncl = RRM_TRUE;
                            /* SPR_15712_Fix: Start */
                            if(p_lte_ncl->intra_freq_cells\
                                    [cell_count].blacklisted == RRM_TRUE)
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                        "Intra Freq PCI %d is blacklisted.", phy_cell_id);
                                *p_pci_blacklisted = RRM_TRUE;
                            }
                            /* SPR_15712_Fix: End */
                        }
                        else
                        {
                            /* SPR_15712_Fix: Start */
                            if((p_lte_ncl->intra_freq_cells\
                                        [cell_count].blacklisted == RRM_TRUE) && (*p_pci_blacklisted == RRM_TRUE))
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                        "Intra freq PCI confusion detected for PCI %d is blacklisted.", phy_cell_id);

                                *p_pci_confusion_detected = RRM_FALSE;
                            }
                            else
                            {
                                /*SPR 17527 START*/
                                /*code deleted*/
                                *p_pci_confusion_detected = RRM_TRUE;
                                break;
                                /*SPR 17527 END*/
                            }
                            /* SPR_15712_Fix: End */
                        }
                        /* Fix for CSR 58972 end*/
                        /* SPR_15712_Fix: Start */
                        /* Code Deleted */
                        /* SPR_15712_Fix: End */
                        /*SPR 15525 Fix End*/
                    }
                }
            }
            else       
            {
                valid_cells =  p_lte_ncl-> num_valid_inter_freq_cell;

                for(cell_count=RRM_ZERO; cell_count < valid_cells; cell_count++)
                {
                    /*SPR 15525 Fix Start - If conditions shuffled*/

                    /* CSR_97730_fix: start */
                    /* SPR 15674 Start */
                    if ((phy_cell_id == p_lte_ncl->inter_freq_cells[cell_count].phy_cell_id) &&
                            ( p_meas_report_info->carrier_earfcn == p_lte_ncl->inter_freq_cells[cell_count].eutra_carrier_arfcn ))
                    {
                        /* SPR 15674 End */
                        if (RRM_TRUE != phy_cell_found_in_ncl)
                        {
                            p_meas_data->freq_specific_offset = p_lte_ncl->inter_freq_cells[cell_count].
                                q_offset;
                            p_meas_data->cell_specific_offset = p_lte_ncl->inter_freq_cells[cell_count].
                                cio;
                            p_meas_data->hysteresis           = p_common_params_for_eutra->hysteresis;

                            phy_cell_found_in_ncl = RRM_TRUE;
                            /* break stmt removed */
                            /* SPR_15712_Fix: Start */
                            if(p_lte_ncl->inter_freq_cells\
                                    [cell_count].blacklisted == RRM_TRUE)
                            {
                                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                        "Inter freq PCI %d is blacklisted.", phy_cell_id);
                                *p_pci_blacklisted = RRM_TRUE;
                            }
                            /* SPR_15712_Fix: End */
                        }
                        else
                        {
                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                                    "Inter freq PCI confusion detected for PCI %d in inter freq neighbrs %d ", phy_cell_id , p_meas_report_info->carrier_earfcn);
                            /*SPR 17527 START*/
                            /*code deleted*/
                            *p_pci_confusion_detected = RRM_TRUE;
                            break;
                            /*SPR 17527 END*/
                        }
                        /* CSR_97730_fix: end */
                        /* SPR_15712_Fix: Start */
                        /* Code Deleted */
                        /* SPR_15712_Fix: End */
                        /*SPR 15525 Fix End*/
                    }
                }
            }
        }
    }
    /* Coverity_fix_88255_end */
    else
    {
        /* CSR_97730_fix: start */
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "NCL list empty");
        /* CSR_97730_fix: end */
    }
    /* SPR 15603 end */
    RRM_UT_TRACE_EXIT();
    return phy_cell_found_in_ncl;
}
/****************************************************************************
 * Function Name  : rrm_verify_ue_for_inter_freq_meas 
 * Inputs         : Ue context.
 * Outputs        : none  
 * Returns        : SUCCESS/FAILURE
 * Description    : Fill inter frequency measurement object for meas config req. 
 *                : If UE is capable of doing inter freq measurement or not
 ****************************************************************************/
    rrm_return_et 
rrm_verify_ue_for_inter_freq_meas(rrm_ue_context_t *p_ue_context, rrm_bool_et is_meas_for_masa)
{
    rrm_trigger_quantity_et trigger_quantity = RRM_RRC_RSRP;
    rrc_rrm_meas_config_req_t  *p_rrc_rrm_meas_config_req = RRM_PNULL;
    rrm_common_params_for_eutra_t *p_common_params_for_eutra = RRM_PNULL;
    rrm_block_cell_params_ho_t    *p_sl_ho_param = RRM_PNULL;
    U8                      report_config_id           = RRM_ZERO;
    rrm_return_et           ret_val = RRM_FAILURE;
    U8 meas_index = RRM_ZERO;
    U8 meas_id = RRM_ZERO;
    /* START: CSR 00057390*/
    meas_object_to_add_mod_list_t   *new_added_meas_object_list = RRM_PNULL;
    /* END: CSR 00057390*/
    rrm_meas_func_ptr          meas_func_ptr = RRM_PNULL;
    meas_type_et              meas_type      = MEAS_TYPE_INVALID;
    /* SPR 15261 start */
    U32 feature_group_indicators = RRM_ZERO;
    /* SPR 15261 end */
    RRM_UT_TRACE_ENTER();


    /*Check that whether UE can do inter freq measurement */
    if (RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT &
            p_ue_context->ue_capability_params.rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.bitmask)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT");
        /* SPR 15261 start */
        feature_group_indicators = p_ue_context->ue_capability_params.
            rrm_eutra_radio_capability_info.
            eutra_radio_capability_info_def.feature_group_indicators;
        /*Checking the 25  and 14 bit is set or not*/
        if(feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_TWENTY_FIVE)))
            /* SPR 15261 end */
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "BIT:25  and 14 set of FGI UE is capable of doing inter freq measurement");
            p_rrc_rrm_meas_config_req = (rrc_rrm_meas_config_req_t *) rrm_mem_get (sizeof(rrc_rrm_meas_config_req_t));
            if(RRM_PNULL != p_rrc_rrm_meas_config_req)
            {
                RRM_MEMSET(p_rrc_rrm_meas_config_req, RRM_ZERO, sizeof(rrc_rrm_meas_config_req_t));
                /*fill the cell index and ue_index for meas req*/
                p_rrc_rrm_meas_config_req->ue_index = p_ue_context->ue_index; 
                /*Fill inter frequency measurement config object*/
                /* START: CSR 00057390*/
                new_added_meas_object_list = (meas_object_to_add_mod_list_t *)rrm_mem_get(sizeof(meas_object_to_add_mod_list_t));
                if(RRM_PNULL != new_added_meas_object_list)
                {
                    RRM_MEMSET(new_added_meas_object_list, RRM_ZERO, sizeof(meas_object_to_add_mod_list_t));
                    ret_val= fill_inter_freq_meas_config_object_frm_ncl(&(p_rrc_rrm_meas_config_req->meas_config), 
                            p_ue_context,
                            new_added_meas_object_list, is_meas_for_masa);
                    /* END: CSR 00057390*/
                    if(RRM_SUCCESS == ret_val)
                    {
                        /* START: CSR 00057390*/
                        if(p_rrc_rrm_meas_config_req->meas_config.meas_object_to_add_mod_list.count > RRM_ZERO)
                        { 
                            /* END: CSR 00057390*/
                            report_config_id = rrm_uem_allocate_free_id (p_ue_context, REP_CONFIG);
                            if(report_config_id <= MAX_MEAS_OBJECT_ID)
                            {
                                p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.report_config_to_add_mod[RRM_ZERO].
                                    report_config_id = report_config_id;
                                /*triggering quantity is RSRP read from xml file*/
                                trigger_quantity = get_a3_a4_trigger_quantity (p_ue_context->cell_index);
                                p_common_params_for_eutra = rrm_cellm_get_connected_mode_common_params\
                                                            (p_ue_context->cell_index);
                                /* SPR 15261 start */
                                if(feature_group_indicators & (RRM_ONE <<(RRM_THIRTY_TWO - RRM_FOURTEEN)))
                                {
                                    fill_report_config_for_event_A4(&(p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.
                                                report_config_to_add_mod[RRM_ZERO]), p_common_params_for_eutra,trigger_quantity,
                                            p_sl_ho_param, p_ue_context, RRM_TRUE);
                                }
                                else
                                {
                                    fill_report_config_for_event_A3(&(p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.
                                                report_config_to_add_mod[RRM_ZERO] ), p_common_params_for_eutra, trigger_quantity);
                                }
                                /* SPR 15261 end */
                                /*set the bitmask for report config*/
                                p_rrc_rrm_meas_config_req->meas_config.bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
                                p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.count++;
                                /*fill meas_report_id*/
                                for(meas_index = RRM_ZERO; meas_index< 
                                        p_rrc_rrm_meas_config_req->meas_config.meas_object_to_add_mod_list.count;meas_index++ )
                                {
                                    meas_id = rrm_uem_allocate_free_id (p_ue_context, MEAS_REPORT);
                                    if(meas_id <= MAX_MEAS_OBJECT_ID)
                                    {
                                        p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.meas_id_to_add_mod[meas_index].
                                            meas_id = meas_id;
                                        p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.meas_id_to_add_mod[meas_index].meas_object_id  = 
                                            p_rrc_rrm_meas_config_req->meas_config.meas_object_to_add_mod_list.\
                                            meas_object_to_add_mod[meas_index].meas_object_id;
                                        /*report_config_to_add_mod of 0 index as ony one report is generated*/
                                        p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.meas_id_to_add_mod[meas_index].report_config_id=
                                            p_rrc_rrm_meas_config_req->meas_config.report_config_to_add_mod_list.\
                                            report_config_to_add_mod[RRM_ZERO].report_config_id;

                                        p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.count++;
                                        if (RRM_TRUE == is_meas_for_masa)
                                        {
                                            meas_func_ptr = rrm_uem_masa_meas_config_ind;
                                            meas_type = MEAS_TYPE_CA;
                                        }
                                        else
                                        {
                                            /*SPR 16764 START*/
                                            meas_func_ptr = rrm_uem_meas_results_ind;
                                            /*SPR 16764 END*/
                                            meas_type = MEAS_TYPE_REDIRECTION;
                                        }
                                        store_meas_report_info(&(p_rrc_rrm_meas_config_req->meas_config),
                                                p_ue_context, meas_id,meas_func_ptr, meas_type);
                                    }
                                    else
                                    {
                                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                                                "Maximum Measurement Id reached for [UE:%d]", p_ue_context->ue_index);
                                        /*Deallocate all configured meas id report id and meas_obj_id
                                        */
                                        rrm_uem_deallocate_used_id(
                                                p_ue_context,
                                                REP_CONFIG,
                                                report_config_id);
                                        rrm_uem_deallocate_used_id(
                                                p_ue_context,
                                                MEAS_REPORT,
                                                meas_id);
                                        ret_val = RRM_FAILURE;
                                        /* SPR_15852_fix: start */
                                        RRM_MEM_FREE (new_added_meas_object_list);
                                        /* SPR_15852_fix: end */
                                        RRM_MEM_FREE (p_rrc_rrm_meas_config_req);
                                        return ret_val;
                                    }

                                    if ( RRM_ZERO != p_rrc_rrm_meas_config_req->meas_config.meas_id_to_add_mod_list.count)
                                    {
                                        p_rrc_rrm_meas_config_req->meas_config.bitmask |= UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT;
                                    }
                                }
                                /* START: CSR 00057390*/
                                if(new_added_meas_object_list->count > RRM_ZERO)
                                {
                                    RRM_MEMCPY(&p_rrc_rrm_meas_config_req->meas_config.meas_object_to_add_mod_list,new_added_meas_object_list,
                                            sizeof(meas_object_to_add_mod_list_t));
                                    p_rrc_rrm_meas_config_req->meas_config.bitmask |=UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
                                }
                                else
                                {
                                    p_rrc_rrm_meas_config_req->meas_config.bitmask &=(~UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT);
                                }
                                /* END: CSR 00057390*/
                                /* SPR 14326 Fix START */
                                if (p_rrc_rrm_meas_config_req->meas_config.bitmask & UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT
                                        &&( RRM_TRUE == p_ue_context->send_measgap_in_meas_config)) 

                                {
                                    p_ue_context->send_measgap_in_meas_config = RRM_FALSE;
                                    /* SPR 14326 Fix End */
                                    RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,
                                            RRM_DETAILED,"fill_measurement_gap_info ");	         
                                    fill_measurement_gap_info (&(p_rrc_rrm_meas_config_req->meas_config), p_ue_context);
                                    p_rrc_rrm_meas_config_req->meas_config.bitmask |= UE_ADM_MEAS_GAP_CONFIG_PRESENT;
                                    /*spr_22361_changes_start*/
                                    if(is_meas_for_masa)
                                    {
                                        p_ue_context->ca_data.is_meas_gap_enabled_for_ca = RRM_TRUE;
                                    }
                                    /*spr_22361_changes_end*/
                                }
                                if(RRM_ZERO == p_rrc_rrm_meas_config_req->meas_config.bitmask)
                                {
                                    RRM_TRACE(g_uem_log_on_off,p_rrm_uem_facility_name,
                                            RRM_DETAILED,"Bitmask is set as zero, not sending meas config req");	    
                                    /* SPR_15852_fix: start */
                                    RRM_MEM_FREE (new_added_meas_object_list);
                                    RRM_MEM_FREE (p_rrc_rrm_meas_config_req);
                                    /* SPR_15852_fix: end */
                                    return RRM_FAILURE;
                                }
                                /* Start CSR: 00058590 */
                                fill_eutran_meas_config_of_anr_to_remove_list(&p_rrc_rrm_meas_config_req->meas_config, p_ue_context);
                                /* Set interfreq_interrat_meas_configured to TRUE to restrict SON ANR measurement */
                                p_ue_context->interfreq_interrat_meas_configured = RRM_TRUE;
                                /* End CSR: 00058590 */
                                /*Send the measurment request for UE*/
                                ret_val = rrm_il_send_rrc_rrm_meas_config_req(p_rrc_rrm_meas_config_req,
                                        RRM_MODULE_ID, RRC_MODULE_ID,RRM_ZERO,p_ue_context->cell_index);
                                if(RRM_FAILURE == ret_val)
                                {
                                    /*need to free all the allocated measid reconfig id*/
                                }
                                else
                                {
                                    if(is_meas_for_masa)
                                    {
                                        p_ue_context->ca_data.a4_recv_for_ca = RRM_TRUE;
                                    }
                                }
                            }
                        }
                        /* START: CSR 00057390*/
                    }
                    /* END: CSR 00057390*/
                    /* SPR_15852_fix: start */
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "Filing of inter_freq ncl failed");
                    }
                    RRM_MEM_FREE (new_added_meas_object_list);
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Unable to allocate memory to new_added_meas_object_list");
                }
                RRM_MEM_FREE (p_rrc_rrm_meas_config_req);
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                        "Memory allocation failed for p_rrc_rrm_meas_config_req for [UE:%u]",
                        p_ue_context->ue_index);
            }
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                    "Required FGI bit 25 and 14 needs to be set. [UE:%u]",
                    p_ue_context->ue_index);
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "RRM_UE_EUTRA_FEATURE_GROUP_INDICATORS_PRESENT is not present. ;UE:%u]",
                p_ue_context->ue_index);
    }
    /* SPR_15852_fix: end */
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* Ue positioing code changes start */
/******************************************************************************
 *   FUNCTION NAME: fill_eutran_report_config_to_add_mod_list_for_ue_positioning
 *   INPUT        : purpose,report_id,report_interval,report_type
 *   OUTPUT       : p_report_config_to_add_mod
 *   DESCRIPTION  : Fills the eutran configuration in report config for UE 
 *                  Positioing to be sent to RRC.
 *   RETURNS      : None
 ******************************************************************************/
    static rrm_void_t
fill_eutran_report_config_to_add_mod_list_for_ue_positioning(
        report_config_to_add_mod_t             *p_report_config_to_add_mod,
        rrm_trigger_type_periodical_purpose_et  purpose,
        U8                                      report_id,
        rrm_lppa_meas_peridicity_et             report_interval,
        U32                                     report_type,
        U8                                      report_config_trigger_qty_bitmask
        )
{
    RRM_UT_TRACE_ENTER();

    p_report_config_to_add_mod->report_config_id = report_id;

    p_report_config_to_add_mod->report_config.bitmask |=
        REPORT_CONFIG_EUTRA_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_eutra.trigger_type.
        bitmask |= REPORT_CONFIG_EUTRA_TRIGGER_TYPE_PERIODICAL_PRESENT;

    p_report_config_to_add_mod->report_config.report_config_eutra.
        trigger_type.periodical.purpose = purpose;

    if ( (report_config_trigger_qty_bitmask & RRM_UE_POS_TRIGGER_QTY_RSRP) &&
            (report_config_trigger_qty_bitmask & RRM_UE_POS_TRIGGER_QTY_RSRQ) )
    {
        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRP;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_quantity = RRM_RRC_BOTH;
    }
    else if(report_config_trigger_qty_bitmask & RRM_UE_POS_TRIGGER_QTY_RSRP)
    {
        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRP;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_quantity = RRM_RRC_SAME_AS_TRIGGER_QUANTITY; 
    }
    else if (report_config_trigger_qty_bitmask & RRM_UE_POS_TRIGGER_QTY_RSRQ)
    {
        p_report_config_to_add_mod->report_config.report_config_eutra.
            trigger_quantity = RRM_RRC_RSRQ;

        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_quantity = RRM_RRC_SAME_AS_TRIGGER_QUANTITY;
    }

    p_report_config_to_add_mod->report_config.report_config_eutra.
        max_report_cells = RRM_MAX_CELLS_REPORTED;  

    p_report_config_to_add_mod->report_config.report_config_eutra.
        report_interval = report_interval;

    if(RRM_LPPA_ONDEMAND == report_type)
    {
        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_amount = RRM_RRC_REPORT_AMOUNT_1;
    }
    else
    {
        p_report_config_to_add_mod->report_config.report_config_eutra.
            report_amount = RRM_RRC_REPORT_AMOUNT_INFINITY;
    }

    if(report_config_trigger_qty_bitmask & RRM_UE_POS_TRIGGER_QTY_UE_RXTX_DIFF)
    {
        p_report_config_to_add_mod->report_config.bitmask |=
            REPORT_CONFIG_EUTRA_EXT_PRESENT;

        p_report_config_to_add_mod->report_config.report_config_eutra_ext.
            bitmask |= REPORT_CFG_EUTRA_UE_RXTX_TIME_DIFF_PRESENT;

        p_report_config_to_add_mod->report_config.report_config_eutra_ext.
            ue_rxtx_time_diff = RRM_ZERO;
    }

    RRM_UT_TRACE_EXIT();
}

/*SPR_8004:fix_start */
/******************************************************************************
 *  FUNCTION NAME: fill_inter_freq_meas_config_object_for_ue_positioning 
 *  INPUT        : p_rrm_ue_context
 *  OUTPUT       : p_meas_config
 *   DESCRIPTION : Fills the meas_config for ue_positioning
 *  RETURNS      : None
 ******************************************************************************/

rrm_void_t
    fill_inter_freq_meas_config_object_for_ue_positioning
(
 rrm_meas_config_t           *p_meas_config,
 rrm_ue_context_t            *p_rrm_ue_context
 )
{

    RRM_UT_TRACE_ENTER();
    U8 valid_freqs         = RRM_ZERO;
    U8 initial_index       = RRM_ZERO;
    U8 meas_object_id     = RRM_ZERO;
    U8  freq_count         = RRM_ZERO;
    U8  carrier_freq_configured = RRM_ZERO;
    U32 carrier_freq       = RRM_ZERO;
    U8  counter            = RRM_ZERO;
    idle_mode_mobility_params_t   *p_idle_mode_mobility_params = RRM_PNULL;
    rrm_bool_et                   ret_car_freq_supported       = RRM_FALSE;
    rrm_bool_et  freq_cells_exist = RRM_FALSE;
    lte_ncl_t    *p_lte_ncl       = RRM_PNULL;
    U8 valid_cells         = RRM_ZERO;
    U8   object_count       = p_meas_config->meas_object_to_add_mod_list.count;
    meas_report_info_node_t  *p_meas_object_config = RRM_PNULL;

    p_idle_mode_mobility_params = rrm_cellm_get_idle_mode_params(p_rrm_ue_context->cell_index);
    valid_freqs            =  p_idle_mode_mobility_params->num_valid_inter_freq_list;
    p_lte_ncl              =  rrm_cellm_get_lte_ncl_info(p_rrm_ue_context->cell_index);
    if(RRM_PNULL != p_lte_ncl)
    {
        valid_cells            =  p_lte_ncl->num_valid_inter_freq_cell;
        meas_object_to_add_mod_t  *p_meas_object_to_add_mod = RRM_PNULL;
        for(freq_count=RRM_ZERO; freq_count < valid_freqs; freq_count++)
        {
            carrier_freq_configured = RRM_ZERO;
            carrier_freq = p_idle_mode_mobility_params->idle_mode_mobility_inter_freq_params[freq_count].
                eutra_carrier_arfcn;

            freq_cells_exist = RRM_FALSE;
            for( counter = RRM_ZERO; counter < valid_cells; counter++)
            {
                if(carrier_freq == p_lte_ncl->inter_freq_cells[counter].eutra_carrier_arfcn)
                {
                    freq_cells_exist = RRM_TRUE;
                    break;
                }
            }
            if(RRM_FALSE == freq_cells_exist)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "Not creating meas object for EUTRA frequency[%d] since no cells exists in \
                        ncl for this frequency", carrier_freq);
                continue;
            }
            for (counter = initial_index; counter < object_count; counter++)
            {
                p_meas_object_to_add_mod = &(p_meas_config->\
                        meas_object_to_add_mod_list.meas_object_to_add_mod[counter]);
                if ( p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq
                        == carrier_freq )
                {
                    carrier_freq_configured = RRM_TRUE;
                    break;
                }
            }


            if (carrier_freq_configured != RRM_TRUE)
            {

                /* carrier freq supported by UE */
                /*CID 66797:start*/
                ret_car_freq_supported = (rrm_bool_et)rrm_ue_is_ue_supported_eutra_frequency (
                        carrier_freq,
                        p_rrm_ue_context);
                /*CID 66797:end*/
                if (!ret_car_freq_supported)
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                            "Carrier Freq = %d, is not supported by [UE:%d]",
                            carrier_freq,
                            p_rrm_ue_context->ue_index);
                    continue;
                } 
                p_meas_object_to_add_mod = &(p_meas_config->meas_object_to_add_mod_list.\
                        meas_object_to_add_mod[counter]);

                if( RRM_TRUE == rrm_is_meas_object_configured_for_frequency(p_rrm_ue_context,
                            carrier_freq,
                            RRM_ZERO,
                            RRM_IRAT_PRIORITY_EUTRAN,&p_meas_object_config))
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            " Inter-freq meas_object is already configured for carrier_freq =%d for  [UE:%d]",
                            carrier_freq,p_rrm_ue_context->ue_index);
                    p_meas_object_to_add_mod->meas_object.bitmask
                        = MEAS_OBJECT_TO_ADD_EUTRA_PRESENT;
                    p_meas_object_to_add_mod->meas_object_id
                        = p_meas_object_config->meas_object_id;
                    p_meas_object_to_add_mod->meas_object.meas_object_eutra.carrier_freq
                        =
                        carrier_freq;
                }
                else
                {
                    meas_object_id = rrm_uem_allocate_free_id (p_rrm_ue_context, MEAS_CONFIG);
                    if(meas_object_id <= MAX_MEAS_OBJECT_ID)
                    {
                        p_meas_object_to_add_mod->meas_object_id = meas_object_id; 
                        fill_inter_freq_meas_object_info(p_meas_object_to_add_mod,
                                p_rrm_ue_context, p_idle_mode_mobility_params, freq_count);
                    }
                }
                object_count++;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                        "carrier freq is already configured:%d",carrier_freq);
            }
        }
        if (object_count > p_meas_config->meas_object_to_add_mod_list.count)
        {
            /* SPR 15260 start */
            p_meas_config->bitmask  = UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;
            /* SPR 15260 end */
            p_meas_config->meas_object_to_add_mod_list.count = object_count;
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "NCL list is empty");
    }
    RRM_UT_TRACE_EXIT();
}

/*SPR_8004:fix_end */

/****************************************************************************
 * Function Name  : rrm_fill_meas_config_req_for_ue_positioning 
 * Inputs         : p_ue_context,meas_object_id,esmlc_meas_id,report_interval,
 report_type
 * Outputs        : p_meas_config  
 * Returns        : SUCCESS/FAILURE
 * Description    : Fill EUTRAN frequency measurement object for meas config req for UE positioing. 
 ****************************************************************************/
    rrm_return_et 
rrm_fill_meas_config_req_for_ue_positioning (
        rrm_meas_config_t       *p_meas_config,
        U8                       meas_object_id,
        rrm_ue_context_t        *p_ue_context,
        U32                      esmlc_meas_id,
        rrm_lppa_meas_peridicity_et   report_interval,
        U32                           report_type,
        U8                            report_config_trigger_qty_bitmask,
        rrm_bool_et                 is_req_for_rach_info
        )
{
    rrm_return_et     ret_val = RRM_FAILURE;
    U8     meas_id = MAX_MEAS_OBJECT_ID + RRM_ONE;
    U8     report_config_id =  MAX_MEAS_OBJECT_ID + RRM_ONE;
    meas_id_to_add_mod_list_t       *p_meas_id         = RRM_PNULL;
    report_config_to_add_mod_list_t *p_report_config   = RRM_PNULL;
    rrm_meas_func_ptr          meas_func_ptr = RRM_PNULL;

    /*SPR_8004:fix_start */
    U8              index                                = RRM_ONE;
    U8              meas_obj_id                          = RRM_ZERO;
    U8              meas_index                           = RRM_ZERO;
    ncl_params_t    *p_ncl_params                        = RRM_PNULL;
    /*SPR_8004:fix_end */

    RRM_UT_TRACE_ENTER();
    if((RRM_PNULL != p_meas_config) && 
            (meas_object_id <= MAX_MEAS_OBJECT_ID  )&&
            (RRM_PNULL != p_ue_context))
    {
        p_meas_id       =  &(p_meas_config->meas_id_to_add_mod_list);
        p_report_config =  &(p_meas_config->report_config_to_add_mod_list);

        report_config_id = rrm_uem_allocate_free_id (p_ue_context, REP_CONFIG);
        meas_id = rrm_uem_allocate_free_id (p_ue_context, MEAS_REPORT);

        /*SPR_8004:fix_start */
        if(RRM_FALSE == is_req_for_rach_info)
        {
            p_ncl_params = rrm_cellm_get_ncl_info(p_ue_context->cell_index);
            if ((RRM_PNULL != p_ncl_params) && 
                    (p_ncl_params->lte_ncl.num_valid_inter_freq_cell >= RRM_ONE))
            {       
                fill_inter_freq_meas_config_object_for_ue_positioning(p_meas_config,p_ue_context);
            }
        }

        if(report_config_id <= MAX_MEAS_OBJECT_ID && meas_id <= MAX_MEAS_OBJECT_ID)
        {   
            /*fill report_config_info */

            p_report_config->count = RRM_ONE;
            p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;
            fill_eutran_report_config_to_add_mod_list_for_ue_positioning(&(p_report_config->report_config_to_add_mod[RRM_ZERO]),
                    RRM_RRC_REPORT_STRONGEST_CELLS,
                    report_config_id,
                    report_interval,
                    report_type,
                    report_config_trigger_qty_bitmask);
            p_meas_id->count = RRM_ONE;
            p_meas_config->bitmask |= UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT;
            p_meas_id->meas_id_to_add_mod[RRM_ZERO].meas_id = meas_id;
            p_meas_id->meas_id_to_add_mod[RRM_ZERO].meas_object_id = meas_object_id;
            p_meas_id->meas_id_to_add_mod[RRM_ZERO].report_config_id = report_config_id;

	    /* SPR 21768 Fix Start */	
	    if(RRM_PNULL != p_ue_context->p_ue_positioing_data)
	    {	
		    p_ue_context->p_ue_positioing_data[esmlc_meas_id].ue_meas_id_allocate = RRM_TRUE;
		    p_ue_context->p_ue_positioing_data[esmlc_meas_id].meas_id_and_meas_obj_id[RRM_ZERO].\
			    meas_id = meas_id;
		    p_ue_context->p_ue_positioing_data[esmlc_meas_id].meas_id_and_meas_obj_id[RRM_ZERO].\
			    meas_object_id = meas_object_id;
		    p_ue_context->p_ue_positioing_data[esmlc_meas_id].meas_id_and_meas_obj_id[RRM_ZERO].\
			    report_config_id = report_config_id;
	    }
	    /* SPR 21768 Fix End */	
            if(RRM_FALSE == is_req_for_rach_info)
            {
                for(meas_index = RRM_ONE; meas_index <= p_meas_config->meas_object_to_add_mod_list.count; meas_index ++)
                {
                    meas_id = rrm_uem_allocate_free_id (p_ue_context, MEAS_REPORT);
                    if(meas_id <= MAX_MEAS_OBJECT_ID)
                    {
                        if ( p_meas_config->meas_object_to_add_mod_list.\
                                meas_object_to_add_mod[meas_index - RRM_ONE].meas_object.bitmask &
                                MEAS_OBJECT_TO_ADD_EUTRA_PRESENT ) 
                        {
                            p_meas_id->meas_id_to_add_mod[index].meas_id = meas_id;
                            meas_obj_id = p_meas_config->meas_object_to_add_mod_list.\
                                          meas_object_to_add_mod[meas_index - RRM_ONE].meas_object_id;
                            p_meas_id->meas_id_to_add_mod[index].meas_object_id = meas_obj_id ;
                            report_config_id = p_report_config->report_config_to_add_mod[RRM_ZERO].report_config_id;
                            p_meas_id->meas_id_to_add_mod[index].report_config_id = report_config_id;

                            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                    "Measurement Id %d is created for Measurement_object_id = %d and Report_config_id = %d",
                                    meas_id, meas_obj_id, report_config_id);

                            p_meas_id->count++;

			    /* SPR 21768 Fix Start */	
			    if(RRM_PNULL != p_ue_context->p_ue_positioing_data)
			    {	
				    p_ue_context->p_ue_positioing_data[esmlc_meas_id].meas_id_and_meas_obj_id[index].\
					    meas_id = meas_id;
				    p_ue_context->p_ue_positioing_data[esmlc_meas_id].meas_id_and_meas_obj_id[index].\
					    meas_object_id = meas_obj_id;
				    p_ue_context->p_ue_positioing_data[esmlc_meas_id].meas_id_and_meas_obj_id[index].\
					    report_config_id = report_config_id;
			    }
			    /* SPR 21768 Fix End */	

                            index ++ ;  

                        }
                    }
                }
            }
            else
            {
                p_ue_context->temp_rach_data.meas_id = meas_id;
                /* SPR-18789 START*/
                p_ue_context->temp_rach_data.report_config_id = report_config_id;
                p_ue_context->temp_rach_data.is_rach_report_config_to_remove = RRM_FALSE;
                /* SPR-18789 END*/
            }
            /*SPR 16764 START*/
            meas_func_ptr = rrm_uem_meas_results_ind;
            /*SPR 16764 END*/
            store_meas_report_info_for_ue_pos(p_meas_config,
                    p_ue_context, is_req_for_rach_info,meas_func_ptr);

            ret_val =  RRM_SUCCESS;

        }
        /*SPR_8004:fix_end */

    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "P_meas_config or p_ue_context is NULL or meas_object_id[%u] > MAX_MEAS_OBJECT_ID",
                meas_object_id);
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_uem_deallocate_meas_user_id_for_ue_positioing 
 * Inputs         : p_ue_context,meas_id,report_config_id
 * Outputs        : none 
 * Returns        : none 
 * Description    : Deallocate the meas_id and report_id for ue_positioing . 
 ****************************************************************************/
rrm_void_t
    rrm_uem_deallocate_meas_user_id_for_ue_positioing
(
 rrm_ue_context_t  *p_ue_context,
 U8  meas_id,
 U8  report_config_id
 )
{
    RRM_UT_TRACE_ENTER();
    if(RRM_PNULL != p_ue_context )
    {
        if( meas_id <= MAX_MEAS_OBJECT_ID && report_config_id <= MAX_MEAS_OBJECT_ID)
        {
            rrm_uem_deallocate_used_id (p_ue_context,MEAS_REPORT,meas_id);
            rrm_uem_deallocate_used_id (p_ue_context,REP_CONFIG,report_config_id);
        }
        else
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "meas_id[%d] > MAX_MEAS_OBJECT_ID or report_config_id[%d] > MAX_MEAS_OBJECT_ID",
                    meas_id, report_config_id);
        }
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                "p_ue_context is NULL ");
    }

    RRM_UT_TRACE_EXIT();

}
/* Ue positioing code changes end */

/* START: CSR 00057390*/
/****************************************************************************
 * Function Name  : rrm_is_meas_event_configured
 * Inputs         : p_ue_context,
 carrier_freq,
 event_type
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Determine is meas_event already configured for given frequency/meas_object_id
 ****************************************************************************/
rrm_bool_et
    rrm_is_meas_event_configured
(
 rrm_ue_context_t       *p_ue_context,
 U8                     meas_object_id,
 rrmc_meas_report_event_et meas_event
 )
{
    rrm_bool_et ret_val = RRM_FALSE;
    YLNODE *p_node = RRM_PNULL;
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Checking is meas_event =%d for meas_object_id=%d already configured for [UE:%d]",
            meas_event,
            meas_object_id,
            p_ue_context->ue_index);
    p_node = ylFirst(&(p_ue_context->meas_report_list));
    while (p_node)
    {
        p_meas_report_node = (meas_report_info_node_t*)p_node;
        if(meas_object_id == p_meas_report_node->meas_object_id)
        {
            if( meas_event == p_meas_report_node->reporting_event)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        " meas_event =%d is already configured for meas_object_id=%d for freq=%d for [UE:%d]",
                        meas_event,
                        p_meas_report_node->meas_object_id,
                        p_meas_report_node->carrier_earfcn,
                        p_ue_context->ue_index);
                ret_val = RRM_TRUE;
                break;
            }
        }
        p_node = ylNext(p_node);
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_is_meas_object_configured_for_frequency
 * Inputs         : p_ue_context,
 acarrier_freq,
 band_class
 freq_type,
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Determine is meas_object already configured for given frequency
 ****************************************************************************/
rrm_bool_et
    rrm_is_meas_object_configured_for_frequency
(
 rrm_ue_context_t         *p_ue_context,
 U32                      carrier_earfcn,
 U8                       band_indicator, 
 rrm_irat_priority_et     freq_type,
 meas_report_info_node_t  **p_meas_object_config
 )
{
    rrm_bool_et ret_val = RRM_FALSE;
    YLNODE *p_node = RRM_PNULL;
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;
    cell_index_list_t     cells_to_remove_list;

    RRM_MEMSET(&cells_to_remove_list,RRM_ZERO,sizeof(cell_index_list_t) );
    RRM_UT_TRACE_ENTER();

    p_node = ylFirst(&(p_ue_context->meas_report_list));
    while (p_node)
    {
        p_meas_report_node = (meas_report_info_node_t*)p_node;
        /* SPR 15608 Start */
        if(p_meas_report_node->meas_status != RRM_MEAS_CONFIG_TO_DELETE)
        {
            /* SPR 15608 End */
            if(RRM_IRAT_PRIORITY_EUTRAN == freq_type &&
                    carrier_earfcn           == p_meas_report_node->carrier_earfcn)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "EUTRAN meas_object =%d is already configured for carrier_earfcn =%d for [UE:%d]",
                        p_meas_report_node->meas_object_id,
                        p_meas_report_node->carrier_earfcn,
                        p_ue_context->ue_index);
                *p_meas_object_config = p_meas_report_node;
                ret_val = RRM_TRUE;
                break;
            }
            else if(RRM_IRAT_PRIORITY_UTRAN == freq_type &&
                    carrier_earfcn           == p_meas_report_node->carrier_earfcn)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "UTRAN meas_object =%d is already configured for carrier_earfcn =%d for [UE:%d]",
                        p_meas_report_node->meas_object_id,
                        p_meas_report_node->carrier_earfcn,
                        p_ue_context->ue_index);
                *p_meas_object_config = p_meas_report_node;
                ret_val = RRM_TRUE;
                break;
            }
            else if(RRM_IRAT_PRIORITY_GERAN == freq_type &&
                    carrier_earfcn          == p_meas_report_node->carrier_earfcn &&
                    band_indicator          == p_meas_report_node->band_indicator  )
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "GERAN meas_object =%d is already configured for carrier_earfcn =%d,band_indicator=%d for [UE:%d]",
                        p_meas_report_node->meas_object_id,
                        p_meas_report_node->carrier_earfcn,
                        p_meas_report_node->band_indicator,
                        p_ue_context->ue_index);
                *p_meas_object_config = p_meas_report_node;
                ret_val = RRM_TRUE;
                break;
            }
            else if(RRM_IRAT_PRIORITY_CDMA2000 == freq_type &&
                    carrier_earfcn             == p_meas_report_node->carrier_earfcn &&
                    band_indicator             == p_meas_report_node->band_indicator  )
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_INFO,
                        "CDMA2000 meas_object =%d is already configured for carrier_earfcn =%d,band_indicator=%d for [UE:%d]",
                        p_meas_report_node->meas_object_id,
                        p_meas_report_node->carrier_earfcn,
                        p_meas_report_node->band_indicator,
                        p_ue_context->ue_index);
                *p_meas_object_config = p_meas_report_node;
                ret_val = RRM_TRUE;
                break;
            }
            else
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "carrier_earfcn =%d, band_indicator=%d, freq_type = %d  is not configured at meas_object_id=%d for [UE:%d]",
                        p_meas_report_node->carrier_earfcn,
                        p_meas_report_node->band_indicator,
                        freq_type,
                        p_meas_report_node->meas_object_id,
                        p_ue_context->ue_index);
            }
            /* SPR 15608 Start */
        }
        /* SPR 15608 End */
        p_node = ylNext(p_node);
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/****************************************************************************
 * Function Name  : rrm_is_meas_object_configured
 * Inputs         : p_ue_context,
 meas_object_id
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Determine is meas_object already configured
 ****************************************************************************/
rrm_bool_et
    rrm_is_meas_object_configured
(
 rrm_ue_context_t       *p_ue_context,
 U8                      meas_object_id
 )
{
    rrm_bool_et ret_val = RRM_FALSE;
    YLNODE *p_node = RRM_PNULL;
    meas_report_info_node_t* p_meas_report_node = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Checking is meas_object_id = %d already configured for [UE:%d]",
            meas_object_id,
            p_ue_context->ue_index);
    p_node = ylFirst(&(p_ue_context->meas_report_list));
    while (p_node)
    {
        p_meas_report_node = (meas_report_info_node_t*)p_node;
        if( meas_object_id   == p_meas_report_node->meas_object_id)
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                    "meas_object =%d is already configured for carrier_earfcn =%d for [UE:%d]",
                    p_meas_report_node->meas_object_id,
                    p_meas_report_node->carrier_earfcn,
                    p_ue_context->ue_index);
            ret_val = RRM_TRUE;
            break;
        }
        p_node = ylNext(p_node);
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_fill_new_added_meas_object
 * Inputs         : p_ue_context,
 p_meas_config,
 p_new_added_meas_object
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Determine and populate only new added meas_object 
 ****************************************************************************/
rrm_bool_et
    rrm_fill_new_added_meas_object
(
 rrm_ue_context_t                *p_rrm_ue_context,
 rrm_meas_config_t               *p_meas_config,
 meas_object_to_add_mod_list_t   *p_new_added_meas_object
 )
{
    U8 meas_object_index = RRM_ZERO;
    U8 new_meas_object_index = RRM_ZERO;

    RRM_UT_TRACE_ENTER();
    rrm_bool_et ret_val = RRM_FALSE;
    if(RRM_PNULL != p_rrm_ue_context && RRM_PNULL != p_meas_config && RRM_PNULL != p_new_added_meas_object)
    {
        if(p_meas_config->bitmask & UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT)
        {
            if(p_meas_config->meas_object_to_add_mod_list.count > RRM_ZERO)
            {
                for (meas_object_index = RRM_ZERO;
                        (meas_object_index < p_meas_config->meas_object_to_add_mod_list.count) && (meas_object_index < MAX_MEAS_OBJECT_ID);
                        meas_object_index++)
                {
                    if(RRM_FALSE == rrm_is_meas_object_configured(p_rrm_ue_context,
                                p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[meas_object_index].meas_object_id))
                    {
                        p_new_added_meas_object->count++;
                        RRM_MEMCPY(&(p_new_added_meas_object->meas_object_to_add_mod[new_meas_object_index]),
                                &p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[meas_object_index],
                                sizeof(meas_object_to_add_mod_t));
                        new_meas_object_index++;
                        ret_val = RRM_TRUE;
                    }
                }
            }
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* END: CSR 00057390*/
/* Start CSR: 00058590 */
/****************************************************************************
 * Function Name  : rrm_is_any_interfreq_interrat_meas_configured
 * Inputs         : p_ue_context,
 * Outputs        : none
 * Returns        : RRM_TRUE/RRM_FALSE
 * Description    : Determine is any inter-freq or inter-rat measurement configured or not
 ****************************************************************************/
rrm_bool_et
    rrm_is_any_interfreq_interrat_meas_configured
(
 rrm_ue_context_t       *p_ue_context
 )
{
    rrm_bool_et                  ret_val             = RRM_FALSE;
    YLNODE                       *p_node             = RRM_PNULL;
    meas_report_info_node_t      *p_meas_report_node = RRM_PNULL;
    rf_params_t                  *p_rf_params        = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_rf_params   = rrm_cellm_get_rf_params(p_ue_context->cell_index);

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
            "Checking is any inter-freq or inter-rat measurement configured or not for [UE:%d]",
            p_ue_context->ue_index);
    p_node = ylFirst(&(p_ue_context->meas_report_list));
    while (p_node)
    {
        p_meas_report_node = (meas_report_info_node_t*)p_node;

        /* SPR 15598 Start*/
        if((p_meas_report_node->meas_id >= HO_MEAS_START_ID) &&
                (p_meas_report_node->meas_id <= MAX_MEAS_OBJECT_ID))
        {
            if(  p_rf_params->rf_configurations.dl_earfcn   != p_meas_report_node->carrier_earfcn)
            {
                RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                        "meas_object =%d is already configured for (inter-freq or inter-rat) carrier_earfcn =%d for [UE:%d]",
                        p_meas_report_node->meas_object_id,
                        p_meas_report_node->carrier_earfcn,
                        p_ue_context->ue_index);
                ret_val = RRM_TRUE;
                break;
            }
        }
        /* SPR 15598 Stop*/
        p_node = ylNext(p_node);
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* End CSR: 00058590 */
/*SPR 21365 fix start*/
/****************************************************************************
 * Function Name  : rrm_count_measurements_for_ho_configured
 * Inputs         : p_ue_context,
 * Outputs        : none
 * Returns        : U8
 * Description    : Determines the number of HO objects being configured.
 ****************************************************************************/
U8 rrm_count_measurements_for_ho_configured
( rrm_ue_context_t *p_rrm_ue_context
)
{
    U8    count  = RRM_ZERO;
    meas_report_info_node_t *p_meas_report_node = RRM_PNULL;
    RRM_UT_TRACE_ENTER();
    if(RRM_PNULL != p_rrm_ue_context)
    {    
        p_meas_report_node = (meas_report_info_node_t*)ylFirst(&(p_rrm_ue_context->meas_report_list));
        while(p_meas_report_node)
        {
            if((RRM_MEAS_CONFIG_RESP_SUCCESS == p_meas_report_node->meas_status) && (p_meas_report_node->meas_id >= HO_MEAS_START_ID))
            {
                count++;
            }    
            p_meas_report_node =  (meas_report_info_node_t*)ylNext((YLNODE *)&p_meas_report_node->sNode);
        }     
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_BRIEF,
                "Number of HO measurement configured %u",count);
    }
    RRM_UT_TRACE_EXIT();
    return count;
}    
/*SPR 21365 fix stop*/
/* ENDC_MEAS_CHANGES_START */
#ifdef ENDC_ENABLED
/* NR_BAND_FIX_START */
rrm_bool_et
check_nr_cell_freq_band_ind_supported_by_ue
(

  U64                       nr_band_bitmap_fdd[],
  rrm_x2ap_nr_fdd_info_t   *p_srv_cell_fdd_info,
  rrm_ue_index_t            ue_index
)
{
/* NR_BAND_FIX_START */
    rrm_bool_et   ret_val       = RRM_FALSE;
/* NR_BAND_FIX_END */
    U16           freq_count    = RRM_ZERO;
    U16           freq_ind_nr   = RRM_ZERO;
    U16           freq_band_arr_index = RRM_ZERO;
    U64           freq_band_bitmask   = RRM_ZERO;
    rrm_freq_band_nr_info_t      *p_rrm_freq_band_nr_info = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
    for(freq_count = 0; 
            freq_count < p_srv_cell_fdd_info->nr_dl_freq_info.num_freq_nr;
            freq_count++)
    {
        p_rrm_freq_band_nr_info = &(p_srv_cell_fdd_info->nr_dl_freq_info. 
                                            freq_band_nr_info[freq_count]);

        freq_ind_nr = p_rrm_freq_band_nr_info->freq_band_indicator_nr;
        freq_band_bitmask |= (1<<(freq_ind_nr-1));

        freq_band_arr_index = freq_ind_nr / (sizeof(U64) * 8);

        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "DL Freq band indicator for cell is:[%u] bitmask [%u]",freq_ind_nr,freq_band_bitmask);

        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "DL Freq band indicator for UE[%d] is: %u at index[%u]",ue_index,
                 nr_band_bitmap_fdd[freq_band_arr_index],freq_band_arr_index);

        if(nr_band_bitmap_fdd[freq_band_arr_index] & freq_band_bitmask)
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                    "Nr cell freq band is matched with UE[%d] supported ",ue_index);
            ret_val = RRM_TRUE;
            break;
        }
        freq_band_bitmask = RRM_ZERO;
    }

    if( RRM_FALSE == ret_val)
    {
        RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_DETAILED,
                "None of the Frequemcy bands is matched with the UE supported freq Band");
    }
 
    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/* NR_BAND_FIX_END */
/******************************************************************************
 *  FUNCTION NAME: fill_ssb_subcarrier_spacing_meas_obj_nr 
 *  INPUT        : x2_ssb_spacing - subcarrier ssb spacing in x2 link up message
 *  OUTPUT       : p_meas_obj_ssb_r15 - subcarrier ssb spacing for meas obj
 *  DESCRIPTION  : Maps the subcarrier ssb spacing in x2 link up message
 *                 to subcarrier ssb spacing for meas obj 
 *  RETURNS      : RRM_SUCCESS/ RRM_FAILURE
 ******************************************************************************/
rrm_return_et
fill_ssb_subcarrier_spacing_meas_obj_nr
(
  U8 *p_meas_obj_ssb_r15,
  U8 x2_ssb_spacing
)
{
    rrm_return_et ret_val = RRM_SUCCESS;
    RRM_UT_TRACE_ENTER();

    switch(x2_ssb_spacing)
    {
        case (U8)RRM_X2AP_KHZ_15:
        { 
            *p_meas_obj_ssb_r15 = (U8)SUBCARRIER_SPACING_KHZ_15;
            break;
        }
        case RRM_X2AP_KHZ_30:
        { 
            *p_meas_obj_ssb_r15 = (U8)SUBCARRIER_SPACING_KHZ_30;
            break;
        }
        case RRM_X2AP_KHZ_120:
        { 
            *p_meas_obj_ssb_r15 = (U8)SUBCARRIER_SPACING_KHZ_120;
            break;
        }
        case RRM_X2AP_KHZ_240:
        { 
            *p_meas_obj_ssb_r15 = (U8)SUBCARRIER_SPACING_KHZ_240;
            break;
        }
        case RRM_X2AP_KHZ_60:
        default:
        {
            RRM_TRACE(g_uem_log_on_off, p_g_rrm_uem_facility_name, RRM_ERROR,
                    "In valid ssb subcarrier spacing in x2 link up [%d] \n",x2_ssb_spacing);
            ret_val = RRM_FAILURE;
        }

    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/******************************************************************************
 *  FUNCTION NAME: fill_nr_meas_object_config 
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the NR configuration in meas config list with values 
 *                 received from OAM and RRC.
 *  RETURNS      : RRM_SUCCESS/ RRM_FAILURE
 ******************************************************************************/
rrm_return_et 
fill_nr_meas_object_config
(
  rrm_meas_config_t*     p_meas_config,
  rrm_ue_context_t*      p_rrm_ue_context,
  rrm_cell_context_t*    p_cell_ctxt
)
{
    rrm_return_et              ret_val          = RRM_FAILURE;
    U8                         index            = RRM_ZERO;
    U8                         meas_index       = RRM_ZERO;
    U8                         meas_object_id   = RRM_ZERO;
    U8                         cell_count       = RRM_ZERO;
    U8                         sgnb_count       = RRM_ZERO;
    U8                         valid_cell_count = RRM_ZERO;
    meas_object_nr_t          *p_meas_obj_nr    = RRM_PNULL;
    YLNODE                    *p_node           = RRM_PNULL;
    rrm_x2ap_enb_node_t       *p_x2ap_enb_node  = RRM_PNULL;
    rrm_x2ap_srvd_nr_cell_info_t  
        *p_rrm_x2ap_srvd_nr_cell_info = RRM_PNULL;
    rrm_irat_params_t*        p_rrm_irat_params = RRM_PNULL;
    rrm_x2ap_ssb_mtc_t  *p_ssb_meas_timing_conf = RRM_PNULL;
    rrm_x2ap_meas_timing_config_t 
        *p_nr_meas_timing_conf = RRM_PNULL;
    rrm_bool_et         meas_obj_added_for_cell = RRM_FALSE;

    RRM_UT_TRACE_ENTER();

    p_node = ylFirst(&(p_g_rrm_cell_ctx->rrm_x2_enb_list));

    while(p_node)
    {
        p_x2ap_enb_node = (rrm_x2ap_enb_node_t *)p_node;

        if((p_x2ap_enb_node->link_up_data.bitmask &
                    RRM_NR_CELL_INFO_PRESENT) && (sgnb_count < RRM_TWO))
        {
            valid_cell_count = RRM_ZERO;

            for(cell_count = RRM_ZERO; 
                    (cell_count < MAX_CELL_REPORT )&& 
                    ( cell_count < p_x2ap_enb_node->link_up_data.nr_cell_info.\
                      num_served_nr_cell);
                    cell_count++)
            {
                p_rrm_x2ap_srvd_nr_cell_info = 
                    &(p_x2ap_enb_node->link_up_data.nr_cell_info.\
                            cell_info[cell_count].served_cell_info);

                meas_obj_added_for_cell = RRM_FALSE;

                if(p_rrm_x2ap_srvd_nr_cell_info->choice_nr_mode.bitmask & 
                        RRM_X2AP_NR_MODE_FDD_PRESENT)
                {
                    if(RRM_TRUE == 
                            check_nr_cell_freq_band_ind_supported_by_ue(
/* NR_BAND_FIX_START */
                               p_rrm_ue_context->nr_band_bitmap_fdd,&(p_rrm_x2ap_srvd_nr_cell_info->choice_nr_mode.fdd_info),p_rrm_ue_context->ue_index))
/* NR_BAND_FIX_END */
                    {
                    index = p_meas_config->meas_object_to_add_mod_list.count;

                    p_meas_obj_nr = 
                        &(p_meas_config->meas_object_to_add_mod_list.\
                                meas_object_to_add_mod[index].\
                                meas_object.meas_object_nr);

                    /* Allocate the meas object Id */
                    meas_object_id = 
                        rrm_uem_allocate_free_id(p_rrm_ue_context, MEAS_CONFIG);
                    if(meas_object_id > MAX_MEAS_OBJECT_ID)
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,\
                                RRM_DETAILED,\
                                "Failed to allocate new measurement object");
                            return ret_val; 
                    }

                    meas_obj_added_for_cell = RRM_TRUE;

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,\
                                RRM_DETAILED,\
                                "meas len[%d] \n",p_rrm_x2ap_srvd_nr_cell_info->meas_timing_config_len);
                    for ( meas_index = RRM_ZERO; meas_index < p_rrm_x2ap_srvd_nr_cell_info->
                        meas_timing_config_len; meas_index++ )
                    {
                        p_nr_meas_timing_conf = 
                            &(p_rrm_x2ap_srvd_nr_cell_info->meas_timing_config[meas_index]);

                        p_meas_obj_nr->carrier_freq_r15 =
                            p_nr_meas_timing_conf->carrier_freq;
/* NR_REP_CONFIG_FIX_START */
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,\
                                RRM_DETAILED,\
                                "Create meas object for carrier freq = [%lu] pci[%u]\n",
                                p_meas_obj_nr->carrier_freq_r15,
                                  p_rrm_x2ap_srvd_nr_cell_info->nr_pci);
/* NR_REP_CONFIG_FIX_END */
                        if( RRM_FAILURE == 
                              fill_ssb_subcarrier_spacing_meas_obj_nr(
                                &(p_meas_obj_nr->rs_config_ssb_nr_r15.\
                                     subcarrier_spacing_ssb_r15),
                                 p_nr_meas_timing_conf->ssb_subcarrier_spacing))
                        {
                           break;
                        }
                        p_ssb_meas_timing_conf = 
                                &(p_rrm_x2ap_srvd_nr_cell_info->meas_timing_config[meas_index].\
                                    ssb_meas_timimg_configuration);
                        p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                            ssb_duration_r15 = p_ssb_meas_timing_conf->mtc_duration; 

                        p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                            periodicity_and_offfset_r15.bitmask= 0x00;
                        p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                            periodicity_and_offfset_r15.bitmask =  
                            p_ssb_meas_timing_conf->periodicity_and_offset.bitmask;
                        if(p_ssb_meas_timing_conf->periodicity_and_offset.bitmask &
                                RRM_X2AP_PERIODICTY_OFFSET_SF5_PRESENT )
                        {
                            p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                                periodicity_and_offfset_r15.sf5_r15= \
                                p_ssb_meas_timing_conf->periodicity_and_offset.sf5;
                        }
                        else if(p_ssb_meas_timing_conf->periodicity_and_offset.bitmask &
                                RRM_X2AP_PERIODICTY_OFFSET_SF10_PRESENT )
                        {
                            p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                                periodicity_and_offfset_r15.sf10_r15= \
                                p_ssb_meas_timing_conf->periodicity_and_offset.sf10;
                        }
                        else if(p_ssb_meas_timing_conf->periodicity_and_offset.bitmask &
                                RRM_X2AP_PERIODICTY_OFFSET_SF20_PRESENT )
                        {
                            p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                                periodicity_and_offfset_r15.sf20_r15= \
                                p_ssb_meas_timing_conf->periodicity_and_offset.sf20;
                        }
                        else if(p_ssb_meas_timing_conf->periodicity_and_offset.bitmask &
                                RRM_X2AP_PERIODICTY_OFFSET_SF40_PRESENT )
                        {
                            p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                                periodicity_and_offfset_r15.sf40_r15= \
                                p_ssb_meas_timing_conf->periodicity_and_offset.sf40;
                        }
                        else if(p_ssb_meas_timing_conf->periodicity_and_offset.bitmask &
                                RRM_X2AP_PERIODICTY_OFFSET_SF80_PRESENT )
                        {
                            p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                                periodicity_and_offfset_r15.sf80_r15= \
                                p_ssb_meas_timing_conf->periodicity_and_offset.sf80;
                        }
                        else if(p_ssb_meas_timing_conf->periodicity_and_offset.bitmask &
                                RRM_X2AP_PERIODICTY_OFFSET_SF160_PRESENT )
                        {
                            p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                                periodicity_and_offfset_r15.sf160_r15= \
                                p_ssb_meas_timing_conf->periodicity_and_offset.sf160;
                        }
                    }
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,\
                                RRM_DETAILED,\
                                " bitmak [%d]\n",
                                p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
                            periodicity_and_offfset_r15.bitmask);
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name,\
                                RRM_DETAILED,\
                                "meas object nr offset [%d] \n",
                        p_meas_obj_nr->rs_config_ssb_nr_r15.meas_timing_config_r15.\
periodicity_and_offfset_r15.sf5_r15);


                    p_rrm_irat_params = 
                        &(p_cell_ctxt->ran_info.connected_mode_mobility_params.irat);

                    p_meas_obj_nr->offset_freq_r15 = p_rrm_irat_params->offset_freq_r15;
                    p_meas_obj_nr->quantity_config_set_r15 = p_rrm_irat_params->quantity_config_set_r15;
                    p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[index].
                        meas_object_id = meas_object_id;

                    p_meas_config->meas_object_to_add_mod_list.meas_object_to_add_mod[index].
                        meas_object.bitmask |= MEAS_OBJECT_TO_ADD_NR_PRESENT;

                    /* Increment the count of configured meas objects */
                    p_meas_config->meas_object_to_add_mod_list.count++;
                }
                }
                else
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "NR mode is not FDD for cell pci :%d",p_rrm_x2ap_srvd_nr_cell_info->nr_pci);
                }

                if( RRM_TRUE  == meas_obj_added_for_cell)
                {
                    p_rrm_ue_context->rrm_endc_nr_data.rrm_endc_nr_meas_data[RRM_ZERO].\
                        rrm_endc_nr_sgnb_meas_data[sgnb_count].\
                        rrm_endc_nr_cell_meas_info[valid_cell_count].\
                        meas_object_id = meas_object_id;
                    p_rrm_ue_context->rrm_endc_nr_data.rrm_endc_nr_meas_data[RRM_ZERO].\
                        rrm_endc_nr_sgnb_meas_data[sgnb_count].\
                        rrm_endc_nr_cell_meas_info[valid_cell_count].\
                        phy_cell_id = p_rrm_x2ap_srvd_nr_cell_info->nr_pci;
                    
                    valid_cell_count++;
                }
            }
            if(valid_cell_count > RRM_ZERO)
            {
                p_rrm_ue_context->rrm_endc_nr_data.\
                    rrm_endc_nr_meas_data[RRM_ZERO].\
                    rrm_endc_nr_sgnb_meas_data[sgnb_count].cell_count = valid_cell_count;
                RRM_MEMCPY(&(p_rrm_ue_context->rrm_endc_nr_data.\
                            rrm_endc_nr_meas_data[RRM_ZERO].\
                            rrm_endc_nr_sgnb_meas_data[sgnb_count].global_gnb_id),
                        &(p_x2ap_enb_node->link_up_data.nr_cell_info.global_gnb_id),
                        sizeof(rrm_x2_gb_gnb_id_t));
                sgnb_count++;
            }
        }
        p_node = ylNext(p_node);
    }

    /* Set the bitmask */
    if ( sgnb_count > RRM_ZERO)
    {
        p_rrm_ue_context->rrm_endc_nr_data.\
            rrm_endc_nr_meas_data[RRM_ZERO].sgnb_count = sgnb_count ;
        ret_val = RRM_SUCCESS;
    }

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
/******************************************************************************
 *  FUNCTION NAME: fill_report_quantity
 *  INPUT        : p_report_quantity 
 *  OUTPUT       : p_report_quantity
 *  DESCRIPTION  : Fills the report quantity.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t 
fill_report_quantity
(
    report_quantity_cell_nr_r15_t*  p_report_quantity,
/* NR_REP_CONFIG_FIX_START */
    rrm_irat_params_t*           p_rrm_irat_params,
    U8                           trigger_quantity
)
{
    RRM_ASSERT(RRM_PNULL != p_report_quantity);
    RRM_ASSERT(RRM_PNULL != p_rrm_irat_params);
 
    RRM_UT_TRACE_ENTER();

    switch(p_rrm_irat_params->report_quantity_nr)
    {
      case (U8)RRM_RRC_NR_RSRP:
      {
        p_report_quantity->ss_rsrp = RRM_TRUE;
        p_report_quantity->ss_rsrq = RRM_FALSE;
        p_report_quantity->ss_sinr = RRM_FALSE;
        break; 
      }
      case (U8)RRM_RRC_NR_RSRQ:
      {
        p_report_quantity->ss_rsrp = RRM_FALSE;
        p_report_quantity->ss_rsrq = RRM_TRUE;
        p_report_quantity->ss_sinr = RRM_FALSE;
        break; 
      }
      case (U8)RRM_RRC_NR_SINR:
      {
        p_report_quantity->ss_rsrp = RRM_FALSE;
        p_report_quantity->ss_rsrq = RRM_FALSE;
        p_report_quantity->ss_sinr = RRM_TRUE;
        break; 
      }
      case (U8)RRM_RRC_NR_RSRP_RSRQ:
      {
        p_report_quantity->ss_rsrp = RRM_TRUE;
        p_report_quantity->ss_rsrq = RRM_TRUE;
        p_report_quantity->ss_sinr = RRM_FALSE;
        break; 
      }
      case (U8)RRM_RRC_NR_RSRP_SINR:
      {
        p_report_quantity->ss_rsrp = RRM_TRUE;
        p_report_quantity->ss_rsrq = RRM_FALSE;
        p_report_quantity->ss_sinr = RRM_TRUE;
        break; 
      }
      case (U8)RRM_RRC_NR_RSRQ_SINR:
      {
        p_report_quantity->ss_rsrp = RRM_FALSE;
        p_report_quantity->ss_rsrq = RRM_TRUE;
        p_report_quantity->ss_sinr = RRM_TRUE;
        break; 
      }
      case (U8)RRM_RRC_NR_ALL:
      {
        p_report_quantity->ss_rsrp = RRM_TRUE;
        p_report_quantity->ss_rsrq = RRM_TRUE;
        p_report_quantity->ss_sinr = RRM_TRUE;
        break; 
      }
      case (U8)RRM_RRC_SAME_AS_TRIGGER:
      {
          if( RRM_NR_TRIGGER_RSRP == trigger_quantity  ) 
          {
              p_report_quantity->ss_rsrp = RRM_TRUE;
          }
          else if (RRM_NR_TRIGGER_RSRQ == trigger_quantity  )
          {
              p_report_quantity->ss_rsrq = RRM_TRUE;
          }
          else if (RRM_NR_TRIGGER_SINR == trigger_quantity  )
          {
              p_report_quantity->ss_sinr = RRM_TRUE;
          }
          break; 
      }
      default:
      {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                  "Invalid Report Quantity is configured [%d],\
                   so enabling RSRP",p_rrm_irat_params->report_quantity_nr);
        p_report_quantity->ss_rsrp = RRM_TRUE;
        p_report_quantity->ss_rsrq = RRM_FALSE;
        p_report_quantity->ss_sinr = RRM_FALSE;
        break; 
      }
    }
/* NR_REP_CONFIG_FIX_END */

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: fill_report_config_for_event_B1_nr 
 *  INPUT        :  
 *  OUTPUT       : p_report_config_to_add_mod 
 *  DESCRIPTION  : Fills the reporting configuration for event W1.
 *  RETURNS      : None
 ******************************************************************************/
rrm_void_t 
fill_report_config_for_event_B1_nr
(
    report_config_to_add_mod_t*  p_report_config_to_add_mod,
/* NR_REP_CONFIG_FIX_START */
    rrm_irat_params_t*           p_rrm_irat_params,
    U8                           trigger_quantity
)
{
    report_config_interrat_trigger_type_event_t* p_event              = RRM_NULL;
    report_config_interrat_t*                    p_report_config_irat = RRM_NULL;
    report_quantity_cell_nr_r15_t *p_report_quantity = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    /* Fetch the pointer to report_config_irat */
    p_report_config_irat = &(p_report_config_to_add_mod->report_config.
            report_config_interrat);

    /* Fetch the pointer to the event to be configured */
    p_event = &(p_report_config_to_add_mod->report_config.
            report_config_interrat.trigger_type.event);

    /* Set the bitmask to indicate that report configuration is for 
     * inter RAT measurement */
    p_report_config_to_add_mod->report_config.bitmask  
        |= REPORT_CONFIG_INTERRAT_PRESENT;

    /* Set the bitmask to indicate that trigger type is 'event' */
    p_report_config_irat->trigger_type.bitmask 
        = REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_PRESENT;

    /* Set the bitmask to indicate that configuration is for event B1 */
    p_event->event_id.bitmask 
        = REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_NR_PRESENT;

    /* Set the value of attributes for event W1 with values configured from OAM */
    p_event->hysteresis     
        = p_rrm_irat_params->hysteresis;
    p_event->time_to_trigger  
        = p_rrm_irat_params->time_to_trigger;

    p_report_config_irat->max_report_cells      
        =  p_rrm_irat_params->max_report_cells; 
    p_report_config_irat->report_interval       
        =  p_rrm_irat_params->report_interval;
    p_report_config_irat->report_amount    
        =  p_rrm_irat_params->report_amount;

    /* Set the bitmask to indicate that report quantity for WLAN is present */
    p_report_config_to_add_mod->report_config.report_config_interrat.bitmask 
        |= REPORT_QUANTITY_CELL_NR_R15_PRESENT;

    /* Fetch the pointer to report quantity */
    p_report_quantity = &(p_report_config_to_add_mod->report_config.
            report_config_interrat.report_quantity_cell_nr_r15);


    p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
        bitmask = RRM_ZERO;
    if( RRM_NR_TRIGGER_RSRP == trigger_quantity  ) 
    {
        p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
            bitmask |= THRESHOLD_NR_R15_RSRP_PRESENT;
        p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
            threshold_rsrp = p_rrm_irat_params->b1_nr_threshold_rsrp;
    }
    else if( RRM_NR_TRIGGER_RSRQ == trigger_quantity  ) 
    {
        p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
            bitmask |= THRESHOLD_NR_R15_RSRQ_PRESENT;
        p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
            threshold_rsrq = p_rrm_irat_params->b1_nr_threshold_rsrq;
    }
    else if( RRM_NR_TRIGGER_SINR == trigger_quantity  ) 
    {
        p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
            bitmask |= THRESHOLD_NR_R15_SINR_PRESENT;
        p_event->event_id.event_b1_nr.b1_threshold_nr_r15.\
            threshold_sinr = p_rrm_irat_params->b1_nr_threshold_sinr;
    }
    /* Populate report quantity attributes */
    fill_report_quantity(p_report_quantity,p_rrm_irat_params,trigger_quantity);
    p_event->event_id.event_b1_nr.report_on_leave_r15 
        = p_rrm_irat_params->report_on_leave_r15;;

/* NR_REP_CONFIG_FIX_END */
    RRM_UT_TRACE_EXIT();
}

/* NR_REP_CONFIG_FIX_START */
rrm_void_t
rrm_get_trigger_quantity_list
(
  U8  trigger_quantiy_nr, 
  U8 *p_trigger_quantity_list,
  U8 *p_trigger_quantity_count
)
{
    RRM_UT_TRACE_ENTER();

    *p_trigger_quantity_count = RRM_ZERO;

    switch(trigger_quantiy_nr)
    {
        case (U8)RRM_RRC_NR_TRIGGER_RSRP:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRP;
            break;
        }
        case (U8)RRM_RRC_NR_TRIGGER_RSRQ:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRQ;
            break;
        }
        case (U8)RRM_RRC_NR_TRIGGER_SINR:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_SINR;
            break;
        }
        case (U8)RRM_RRC_NR_TRIGGER_RSRP_RSRQ:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRP;
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRQ;
            break;
        }
        case (U8)RRM_RRC_NR_TRIGGER_RSRP_SINR:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRP;
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_SINR;
            break;
        }
        case (U8)RRM_RRC_NR_TRIGGER_RSRQ_SINR:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRQ;
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_SINR;
            break;
        }
        case (U8)RRM_RRC_NR_TRIGGER_ALL:
        {
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRP;
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_RSRQ;
            p_trigger_quantity_list[(*p_trigger_quantity_count)++] = RRM_NR_TRIGGER_SINR;
            break;
        }
        default:
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                    "Invalid Trigger quantity configured [%d] \n",
                    trigger_quantiy_nr);

            break;
        }
    }

    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
              "Trigger quantity configured is [%d] , Trigger quantity list updated with [%d]"
              "values \n",trigger_quantiy_nr,*p_trigger_quantity_count);

    RRM_UT_TRACE_EXIT();
}
/* NR_REP_CONFIG_FIX_END */

/******************************************************************************
 *  FUNCTION NAME: fill_nr_meas_report_config 
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the NR  measurements reporting configuration in meas 
 *                 config list with values received from OAM.
 *  RETURNS      : RRM_SUCCESS/ RRM_FAILURE
 ******************************************************************************/
rrm_return_et 
fill_nr_meas_report_config
(
    rrm_meas_config_t*         p_meas_config,
    rrm_ue_context_t*          p_rrm_ue_ctx,
    rrm_cell_context_t*        p_cell_ctxt,
    rrmc_meas_report_event_et  report_event
)
{
/* NR_REP_CONFIG_FIX_START */
    U8                       index               = RRM_ZERO;
    U8                       report_config_id    = RRM_ZERO;
    rrm_irat_params_t       *p_rrm_irat_params   = RRM_PNULL;
    rrm_endc_nr_meas_data_t *p_b1_meas_data      = RRM_PNULL;
    rrm_return_et            ret_val             = RRM_SUCCESS;
    U8 trigger_quantity_list[3] ;
    U8 trigger_quantity_count = RRM_ZERO;
    U8 count = RRM_ZERO;


    RRM_UT_TRACE_ENTER();
    RRM_MEMSET(trigger_quantity_list,RRM_ZERO,sizeof(U8)*3);
    /* Fetch pointer to irat param from the ran_info stored in cell context */
    p_rrm_irat_params = &(p_cell_ctxt->ran_info.connected_mode_mobility_params.irat);

    rrm_get_trigger_quantity_list(p_rrm_irat_params->trigger_quantity_nr,
                                  trigger_quantity_list,&trigger_quantity_count);

    for( count = RRM_ZERO ; count < trigger_quantity_count ; count++)
    {
    /* Allocate a new report config Id */
    report_config_id = rrm_uem_allocate_free_id(p_rrm_ue_ctx, REP_CONFIG);
    index = p_meas_config->report_config_to_add_mod_list.count;

    if(report_config_id <= MAX_MEAS_OBJECT_ID)
    {
        /* Populate report config Id with value allocated above */
        p_meas_config->report_config_to_add_mod_list.report_config_to_add_mod[index].
            report_config_id = report_config_id;
        /* Populate report configuration for event W1 */
        fill_report_config_for_event_B1_nr(&(p_meas_config->report_config_to_add_mod_list.
                        report_config_to_add_mod[index]), p_rrm_irat_params, trigger_quantity_list[count]);

        /* Increment count of report configurations present in add/modify list */
        p_meas_config->report_config_to_add_mod_list.count++;
        p_b1_meas_data = &(p_rrm_ue_ctx->rrm_endc_nr_data.\
                rrm_endc_nr_meas_data[RRM_ZERO]);
            p_b1_meas_data->report_config_id_list[\
               p_b1_meas_data->report_config_id_count++] = report_config_id;
    }
    else
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                "Maximum Report Config Id reached for UE = %d", 
                p_rrm_ue_ctx->ue_index);
        ret_val = RRM_FAILURE;
            break;
        }
    }
    RRM_UT_TRACE_EXIT();
    return ret_val;
/* NR_REP_CONFIG_FIX_END */
}

/******************************************************************************
 *  FUNCTION NAME: fill_nr_meas_id_config 
 *  INPUT        : p_meas_config_
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the NR measurements ids by mapping meas config ids 
 *                 to the corresponding reporting config ids configuration.
 *  RETURNS      : RRM_SUCCESS / RRM_FAILURE
 ******************************************************************************/
rrm_return_et
fill_nr_meas_id_config
(
  rrm_meas_config_t*   p_meas_config,
  rrm_ue_context_t*    p_rrm_ue_context
)
{
/* NR_REP_CONFIG_FIX_START */
    U8                 meas_id_index  = RRM_ZERO;
    U8                 meas_id        = RRM_ZERO;
    U8                 cell_count     = RRM_ZERO;
    U8                 report_count     = RRM_ZERO;
    U8                 sgnb_count     = RRM_ZERO;
    rrm_meas_func_ptr  meas_func_ptr  = RRM_PNULL;
    rrm_return_et      ret_val        = RRM_SUCCESS;
    rrm_endc_nr_meas_data_t 
        *p_b1_meas_data = RRM_PNULL;
    rrm_endc_nr_sgnb_meas_data_t 
        *p_b1_sgnb_meas_data = RRM_PNULL;
    rrm_endc_nr_cell_meas_info_t 
        *p_b1_sgnb_cell_meas_data = RRM_PNULL;
    RRM_UT_TRACE_ENTER();

    p_b1_meas_data = &(p_rrm_ue_context->rrm_endc_nr_data.\
            rrm_endc_nr_meas_data[RRM_ZERO]);

    for(sgnb_count = RRM_ZERO ; 
           (sgnb_count < p_b1_meas_data->sgnb_count) && 
           (ret_val == RRM_SUCCESS );sgnb_count++)
    {
        p_b1_sgnb_meas_data = 
            &(p_b1_meas_data->rrm_endc_nr_sgnb_meas_data[sgnb_count]);

        for(cell_count = RRM_ZERO ; 
                cell_count < p_b1_sgnb_meas_data->cell_count; cell_count++)
        {
            p_b1_sgnb_cell_meas_data = &(p_b1_sgnb_meas_data-> \
                    rrm_endc_nr_cell_meas_info[cell_count]);

            for(report_count = RRM_ZERO ; 
                   report_count < p_b1_meas_data->report_config_id_count; report_count++)
            {
                meas_id_index = p_meas_config->meas_id_to_add_mod_list.count;
                /* Allocate a new meas Id */
                meas_id = rrm_uem_allocate_free_id(p_rrm_ue_context, MEAS_REPORT);

                if(meas_id <= MAX_MEAS_OBJECT_ID)
                {
                    /* Populate the meas Id with the value allocated above */
                    p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[meas_id_index].
                        meas_id  = meas_id;

                    /* Populate the meas object Id with the value fetched above */
                    p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[meas_id_index].
                        meas_object_id   = p_b1_sgnb_cell_meas_data->meas_object_id;

                    /* Populate the meas report Id with the received value */
                    p_meas_config->meas_id_to_add_mod_list.meas_id_to_add_mod[meas_id_index].
                        report_config_id = p_b1_meas_data->report_config_id_list[report_count];

                    /* Increment the count of configured meas Id */
                    p_meas_config->meas_id_to_add_mod_list.count++;

                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "ENDC:- For SgNB gnb[[%d][%d][%d][%d]] plmn[[%d][%d][%d]] & PCI [%d], \n"
                            "MeasId[%d],MeasObjectId[%d],ReportConfigId[%d] created \n", 
                            p_b1_sgnb_meas_data->global_gnb_id.gNB_id[0],
                            p_b1_sgnb_meas_data->global_gnb_id.gNB_id[1],
                            p_b1_sgnb_meas_data->global_gnb_id.gNB_id[2],
                            p_b1_sgnb_meas_data->global_gnb_id.gNB_id[3],
                            p_b1_sgnb_meas_data->global_gnb_id.plmn_identity.plmn_id[0],
                            p_b1_sgnb_meas_data->global_gnb_id.plmn_identity.plmn_id[1],
                            p_b1_sgnb_meas_data->global_gnb_id.plmn_identity.plmn_id[2],
                            p_b1_sgnb_cell_meas_data->phy_cell_id,meas_id, \
                            p_b1_sgnb_cell_meas_data->meas_object_id ,\
                            p_b1_meas_data->report_config_id_list[report_count]);

                    /* update meas data for sgnb */
                    p_b1_sgnb_meas_data->rrm_endc_nr_sgnb_meas_info[\
                        p_b1_sgnb_meas_data->meas_count].meas_id = meas_id;
                    p_b1_sgnb_meas_data->rrm_endc_nr_sgnb_meas_info[\
                        p_b1_sgnb_meas_data->meas_count].meas_object_id = 
                        p_b1_sgnb_cell_meas_data->meas_object_id;
                    p_b1_sgnb_meas_data->rrm_endc_nr_sgnb_meas_info[\
                        p_b1_sgnb_meas_data->meas_count].report_config_id = 
                        p_b1_meas_data->report_config_id_list[report_count];
                    p_b1_sgnb_meas_data->meas_count++;

                /* Store the measurement configuration in UE context for later use */
                meas_func_ptr = rrm_uem_meas_results_ind;
                store_meas_report_info(p_meas_config, p_rrm_ue_context, meas_id, meas_func_ptr, MEAS_TYPE_EUTRA_NR);
            }
            else
            {
                    return RRM_FAILURE;
                }
            }
        }
    }

/* NR_REP_CONFIG_FIX_END */
    RRM_UT_TRACE_EXIT();
    return ret_val;
}

/******************************************************************************
 *  FUNCTION NAME: fill_nr_meas_gap_config 
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills meas gap configuration for NR measurement.
 *  RETURNS      : None 
 ******************************************************************************/
rrm_void_t 
fill_nr_meas_gap_config
(
    rrm_meas_config_t*   p_meas_config, 
    rrm_ue_context_t*    p_rrm_ue_context
)
{
    meas_gap_config_t            *p_meas_gap_config = RRM_PNULL;
    meas_gap_setup_config_info_t *p_meas_gap_setup_info = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

        p_meas_config->bitmask |= UE_ADM_MEAS_GAP_CONFIG_PRESENT;
        p_meas_gap_config = &(p_meas_config->meas_gap_config);

        p_meas_gap_config->meas_gap_config_type = RRM_ONE;
        p_meas_gap_config->bitmask = 0x00;
        p_meas_gap_config->bitmask |= MEAS_GAP_CONFIG_SETUP_CONFIG_INFO_PRESENT;

        p_meas_gap_setup_info = 
             &(p_meas_config->meas_gap_config.setup_config_info);

        switch(p_rrm_ue_context->meas_gap_type)
        {
            case RRM_MEAS_GAP_TYPE_4:
            {
                p_meas_gap_setup_info->bitmask |= MEAS_GAP_CONFIG_GP4_PRESENT;
                p_meas_gap_setup_info->gp4_r15 = p_rrm_ue_context->meas_gap_offset;
                break;
            }
            case RRM_MEAS_GAP_TYPE_5:
            {
                p_meas_gap_setup_info->bitmask |= MEAS_GAP_CONFIG_GP5_PRESENT;
                p_meas_gap_setup_info->gp5_r15 = p_rrm_ue_context->meas_gap_offset;
                break;
            }
            default :
            {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                            "Invalid meas gap type [%d], dont' set meas gap\n",
                              p_rrm_ue_context->meas_gap_type);
                break;
            }
        }
    RRM_UT_TRACE_EXIT();
}
/******************************************************************************
 *  FUNCTION NAME: fill_nr_quantity_config 
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills quantity configuration for NR measurement.
 *  RETURNS      : None 
 ******************************************************************************/
rrm_void_t 
fill_nr_quantity_config
(
    rrm_meas_config_t*   p_meas_config, 
    rrm_cell_context_t*  p_cell_ctxt
)
{
    RRM_UT_TRACE_ENTER();

    quantity_config_rs_nr_r15_t *p_nr_quantity_config = RRM_PNULL;
    rrm_irat_params_t    *p_rrm_irat_params = RRM_PNULL;
    U8   count = RRM_ZERO; 

    /* Fetch pointer to irat param from the ran_info stored in ue context */
    p_rrm_irat_params = &(p_cell_ctxt->ran_info.connected_mode_mobility_params.irat);
    
    /* Set the bitmask to indicate that quantity configuration is present */
    p_meas_config->bitmask |= UE_ADM_MEAS_QUANTITY_CONFIG_PRESENT;

    /* Set the bitmask to indicate that NR quantity configuration is present */
    p_meas_config->quantity_config.bitmask |= QUANTITY_CONFIG_NR_LIST_R15_PRESENT;

    p_meas_config->quantity_config.quantity_config_nr_list_r15.count = 
                      p_rrm_irat_params->quantity_config_set_r15;
          
    for( count = RRM_ZERO ; count  < p_rrm_irat_params->quantity_config_set_r15 ; 
                                count++) 
    {
       p_nr_quantity_config = &( p_meas_config->quantity_config.quantity_config_nr_list_r15.\
                                 quantity_config_nr_r15[count].meas_quantity_cell_nr_r15);

       p_nr_quantity_config->filter_coeff_rsrp_r15 = \
                            p_rrm_irat_params->filter_coeff_rsrp_r15;
       p_nr_quantity_config->filter_coeff_rsrq_r15 = \
                            p_rrm_irat_params->filter_coeff_rsrq_r15;
       p_nr_quantity_config->filter_coefficient_sinr_r13 = \
                            p_rrm_irat_params->filter_coeff_sinr_r15;
    }

    RRM_UT_TRACE_EXIT();
}

/******************************************************************************
 *  FUNCTION NAME: configured_meas_gap_type_supported_by_ue 
 *  INPUT        :  endc_gap_offset_type : endc gap offset configured by 
 *                  operator
 *                 p_rrm_ue_ctx: UE context
 *  OUTPUT       : p_ue_specific_gap_type :  UE supported 1st gap type
 *  DESCRIPTION  : Checks whether UE supports configured meas gap offset or not.
 *  RETURNS      : RRM_TRUE/RRM_FALSE 
 ******************************************************************************/
rrm_bool_et
configured_meas_gap_type_supported_by_ue
(
  U8                   gap_offset_type,
  rrm_ue_context_t*    p_rrm_ue_ctx,
  U8                   *p_ue_specific_gap_type
)
{
    rrm_bool_et gap_type_supported = RRM_FALSE;
    U32          bit_check_pattern = 0x80000000;
    U32          bit_count = RRM_ZERO;
    U32           supported_gap_pattern = RRM_ZERO;
    U8           ue_supported_gap_type = 0xFF;

    RRM_UT_TRACE_ENTER();

    *p_ue_specific_gap_type = ue_supported_gap_type;

    if(p_rrm_ue_ctx->ue_capability_params.bitmask &
            RRM_UE_NR_RADIO_CAPABILITY_PRESENT)
    {
        if( RRM_NR_RADIO_CAPABILITY_NR_MEAS_PRESENT &
                p_rrm_ue_ctx->ue_capability_params.\
                rrm_nr_radio_capability_info.bitmask)
        {
            supported_gap_pattern = p_rrm_ue_ctx->ue_capability_params.\
                                    rrm_nr_radio_capability_info.\
                                    rrm_nr_meas_parameters.supported_gap_pattern;
            while(bit_check_pattern)
            {
                if(supported_gap_pattern & bit_check_pattern)
                {
                    RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                            "bit [%d] is set,so gap offset [%d] is supported by UE",\
                            bit_count,bit_count + RRM_TWO)
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_DETAILED,
                                "gap offset configured by opertaor is [%d]",\
                                gap_offset_type);
                    if ((bit_count > RRM_ONE) &&
                           (ue_supported_gap_type == 0xFF))
                    {
                        ue_supported_gap_type = bit_count + RRM_TWO;

                    }
                    if((bit_count + RRM_TWO) == gap_offset_type)
                    {
                        gap_type_supported  = RRM_TRUE;
                        break;
                    }
                }
                bit_count++;
                bit_check_pattern = bit_check_pattern >> 1;
            }
            if( RRM_FALSE  == gap_type_supported)
            {
               /* pick the 1st nR specific suuported gap */
               *p_ue_specific_gap_type = ue_supported_gap_type;
            }
        }
    }
    RRM_UT_TRACE_EXIT();
    return gap_type_supported;
} 
/******************************************************************************
 *  FUNCTION NAME: fill_eutra_nr_meas_info 
 *  INPUT        : p_rrm_ue_ctx
 *  OUTPUT       : p_meas_config
 *  DESCRIPTION  : Fills the EUTRA-NR specific measurement configuration.
 *  RETURNS      : RRM_SUCCESS / RRM_FAILURE
 ******************************************************************************/
rrm_return_et 
fill_eutra_nr_meas_info
(
  rrm_meas_config_t*   p_meas_config, 
  rrm_ue_context_t*    p_rrm_ue_ctx
) 
{
    RRM_UT_TRACE_ENTER();

    rrmc_meas_report_event_et  report_event  = MEAS_REP_EVENT_NONE;
    rrm_cell_context_t        *p_cell_ctxt   = RRM_PNULL;
    rrm_return_et              ret_val       = RRM_FAILURE;
    rrm_meas_config_info_t    *p_cell_meas_config_info = RRM_PNULL;
    U8                         endc_gap_offset_type = RRM_ZERO;
    U8                         ue_supprted_gap_type = RRM_ZERO;
    RRM_UT_TRACE_ENTER();

    p_cell_meas_config_info = (rrm_meas_config_info_t*)\
                              rrm_cellm_get_meas_config_info(p_rrm_ue_ctx->cell_index);

    /* Fetch cell context using cell index */
    p_cell_ctxt =  rrm_cellm_get_cell_context(p_rrm_ue_ctx->cell_index);
    if (RRM_PNULL == p_cell_ctxt)
    {
        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_WARNING,
                "Cell Context not found");
        return ret_val;
    }

    do
    {
        /* Set the event which triggered the NR measurement configuration */ 
        p_rrm_ue_ctx->meas_rep_event = MEAS_REP_EVENT_B1_NR;

        /* Fill the ENDC-NR measurement object configuration */
        if (RRM_FAILURE == fill_nr_meas_object_config(p_meas_config, \
                    p_rrm_ue_ctx, p_cell_ctxt))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Failed to populate NR meas object configuration");
            RRM_MEMSET(&(p_rrm_ue_ctx->rrm_endc_nr_data.\
                rrm_endc_nr_meas_data[RRM_ZERO]),RRM_ZERO,
                sizeof(rrm_endc_nr_meas_data_t));
            break;
        }
        p_meas_config->bitmask |= UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT;

        /* Fill the NR measurement report configuration */
        if (RRM_FAILURE == fill_nr_meas_report_config(p_meas_config,\
                    p_rrm_ue_ctx, p_cell_ctxt, report_event))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Failed to populate NR Report Config configuration");
            RRM_MEMSET(&(p_rrm_ue_ctx->rrm_endc_nr_data.\
                rrm_endc_nr_meas_data[RRM_ZERO]),RRM_ZERO,
                sizeof(rrm_endc_nr_meas_data_t));
            break;
        } 
        p_meas_config->bitmask |= UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT;    

        /* Fill the NR measurement Ids configuration */
        if (RRM_FAILURE == fill_nr_meas_id_config(p_meas_config, p_rrm_ue_ctx))
        {
            RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                    "Failed to populate NR Meas ID configuration");
            RRM_MEMSET(&(p_rrm_ue_ctx->rrm_endc_nr_data.\
                rrm_endc_nr_meas_data[RRM_ZERO]),RRM_ZERO,
                sizeof(rrm_endc_nr_meas_data_t));
            break;
        }
        p_meas_config->bitmask |= UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT;

        /* Fill the quantity configuration for NR measurements */
        fill_nr_quantity_config(p_meas_config, p_cell_ctxt);

        /* Fill the Meas Gap configuration for NR measurements */
        if(p_cell_meas_config_info->bitmask &
                RRMCM_RMIF_MEAS_GAP_CONFIG_PRESENT)
        {
            if(p_cell_meas_config_info->meas_gap_config.bitmask &
                    RRMCM_RMIF_ENDC_GAP_OFFSET_TYPE_PRESENT)
            {
                if(RRM_TRUE == 
                        configured_meas_gap_type_supported_by_ue(
                            p_cell_meas_config_info->meas_gap_config.\
                            endc_gap_offset_type,
                            p_rrm_ue_ctx,&ue_supprted_gap_type))
                {
                    endc_gap_offset_type = p_cell_meas_config_info->meas_gap_config.\
                                           endc_gap_offset_type;
                }
                else
                {
                    if( ue_supprted_gap_type == 0xFF )
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "Incorrect ue_supprted_gap_type ,dont' set meas gap");
                        break;
                    }
                    else if( ue_supprted_gap_type > RRM_FIVE )
                    {

                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                " Currently no support for gap type [%d],dont' set meas gap\n",
                                ue_supprted_gap_type);
                        break;
                    }
                    else
                    {
                        RRM_TRACE(g_uem_log_on_off, p_rrm_uem_facility_name, RRM_ERROR,
                                "Using UE's supported gap type [%d] \n",
                                ue_supprted_gap_type);
                        endc_gap_offset_type = ue_supprted_gap_type;
                    }
                }

                p_rrm_ue_ctx->rrm_endc_nr_data.lte_meas_gap_type = p_rrm_ue_ctx->meas_gap_type;
                p_rrm_ue_ctx->rrm_endc_nr_data.lte_meas_gap_offset = p_rrm_ue_ctx->meas_gap_offset;

                p_rrm_ue_ctx->meas_gap_type = endc_gap_offset_type;
                p_rrm_ue_ctx->meas_gap_offset = rrm_ue_assign_gap_offset(
                        p_rrm_ue_ctx, p_rrm_ue_ctx->meas_gap_type);
                fill_nr_meas_gap_config(p_meas_config, p_rrm_ue_ctx);
            }
        }
        ret_val = RRM_SUCCESS;
    }while(0);

    RRM_UT_TRACE_EXIT();
    return ret_val;
}
#endif
/* ENDC_MEAS_CHANGES_END */
