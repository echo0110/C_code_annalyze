/******************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc . All Rights Reserved.
*
*******************************************************************************
*
* $$Id: son_anr_utils.c $
*
*******************************************************************************
*
* File Description: This file contains ANR Utility functions
*
*******************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*  Sep, 2012        Shekhar               Initial
******************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_utils.h>
#include <son_anr_utils.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_rrm_composer.h>
#include <son_x2_intf.h>

/****************************************************************************
* Global Variables
****************************************************************************/
extern const son_8 *p_son_anr_facility_name;
extern son_global_config_params_t g_config_params; //Added for SRVCC

/*****************************************************************************
 * Function Name  : anr_get_ho_status_of_utran_nr
 * Inputs         : p_inter_nr  - Pointer to the utran neighbor record structure
 * Outputs        : None
 * Returns        : son_ho_status_et - If handover is ALLOWED, PROHIBITED or
 *                                     BLACKLISTED.
 * Description    : This function returns Handover Status of UTRAN neighbors
 ******************************************************************************/
son_ho_status_et
anr_get_ho_status_of_utran_nr
(
    son_void_t *p_inter_nr
)
{
    son_ho_status_et ret = SON_HO_PROHIBITED;


    SON_UT_TRACE_ENTER();

    /*If HO Status is allowed*/
    if (SON_HO_ALLOWED == ((son_utran_neighbor_info_t *)p_inter_nr)->ho_status)
    {
        ret = SON_HO_ALLOWED;
    } /*If HO status is blacklisted*/
    else if (SON_NR_LOCKED == ((son_utran_neighbor_info_t *)p_inter_nr)->nr_status)
    {
        ret = SON_HO_UNDEFINED;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_get_ho_status_of_utran_nr */

/*****************************************************************************
 * Function Name  : is_cdma_hrpd_nbrs_available_in_rrm_notification
 * Inputs         : p_rrm_cdma_list  - Pointer to  cdma HRPD neighbour list of
 *                                     notification message to RRM
 * Outputs        : None.
 * Returns        : son_bool_et -
 *                  SON_TRUE -   CDMA HRPD  neighbors are available in list.
 *                  SON_FALSE -  CDMA HRPD  neighbors are not available in list.
 * Description    : Function to check if cdma HRPD nbr are available in notification
 *                  message to RRM.
 ******************************************************************************/
son_bool_et
is_cdma_hrpd_nbrs_available_in_rrm_notification
(
    son_rrm_cdma_neighbor_cell_list_hrpd_t  *p_rrm_cdma_list
)
{
    SON_UT_TRACE_ENTER();
    son_bool_et ret = SON_FALSE;
    if (p_rrm_cdma_list->cells_to_add_mod_list_size            ||
        p_rrm_cdma_list->cells_to_remove_list_size             ||
        p_rrm_cdma_list->black_list_cells_to_add_mod_list_size ||
        p_rrm_cdma_list->black_list_cells_to_remove_list_size)
    {
        ret = SON_TRUE;
    }

    SON_UT_TRACE_EXIT();
    return ret;
}

/*****************************************************************************
 * Function Name  : is_cdma_xrtt_nbrs_available_in_rrm_notification
 * Inputs         : p_rrm_cdma_list  - Pointer to cdma XRTT neighbour list
 *                                     in notifcation message to RRM.
 * Outputs        : None
 * Returns        : son_bool_et -
 *                  SON_TRUE -   CDMA XRTT  neighbors are available in list.
 *                  SON_FALSE -  CDMA XRTT  neighbors are not available in list.
 * Description    : Function to check if cdma XRTT nbr
 *                  are available in notification  message to RRM.
 ******************************************************************************/
son_bool_et
is_cdma_xrtt_nbrs_available_in_rrm_notification
(
    son_rrm_cdma_neighbor_cell_list_xrtt_t  *p_rrm_cdma_list
)
{
    SON_UT_TRACE_ENTER();
    son_bool_et ret = SON_FALSE;
    if (p_rrm_cdma_list->cells_to_add_mod_list_size            ||
        p_rrm_cdma_list->cells_to_remove_list_size             ||
        p_rrm_cdma_list->black_list_cells_to_add_mod_list_size ||
        p_rrm_cdma_list->black_list_cells_to_remove_list_size)
    {
        ret = SON_TRUE;
    }

    SON_UT_TRACE_EXIT();

    return ret;
}

/*****************************************************************************
 * Function Name  : is_utran_nbrs_available_in_rrm_notification
 * Inputs         : p_rrm_utran_list  - Pointer to utran neighbor list available
 *                                      in the notification message to RRM.
 * Outputs        : None
 * Returns        : son_bool_et -
 *                  SON_TRUE -   UTRAN neighbors are available in list.
 *                  SON_FALSE -  UTRAN neighbors are not available in list.
 * Description    : Function to check if UTRAN neighbors are available in the
 *                  notification message to RRM.
 ******************************************************************************/
son_bool_et
is_utran_nbrs_available_in_rrm_notification
(
    son_rrm_utran_neighbor_cell_list_t  *p_rrm_utran_list
)
{
    SON_UT_TRACE_ENTER();
    son_bool_et ret = SON_FALSE;
    if (p_rrm_utran_list->cells_to_add_mod_list_size            ||
        p_rrm_utran_list->cells_to_remove_list_size             ||
        p_rrm_utran_list->black_list_cells_to_add_mod_list_size ||
        p_rrm_utran_list->black_list_cells_to_remove_list_size)
    {
        ret = SON_TRUE;
    }

    SON_UT_TRACE_EXIT();
    return ret;
}

/*****************************************************************************
 * Function Name  : is_geran_nbrs_available_in_rrm_notification
 * Inputs         : p_rrm_geran_list  - Pointer to geran neighbour list
 *                                      available in the notification
 *                                      message to RRM.
 * Outputs        : None.
 * Returns        : son_bool_et-
 *                  SON_TRUE -   GERAN neighbors are available in list.
 *                  SON_FALSE -  GERAN neighbors are not available in list.
 * Description    : Function to check if GERAN neighbors available
 *                  in notification message to RRM.
 ******************************************************************************/
son_bool_et
is_geran_nbrs_available_in_rrm_notification
(
    son_rrm_geran_neighbor_cell_list_t  *p_rrm_geran_list
)
{
    SON_UT_TRACE_ENTER();
    son_bool_et ret = SON_FALSE;
    if (p_rrm_geran_list->cells_to_add_mod_list_size            ||
        p_rrm_geran_list->cells_to_remove_list_size             ||
        p_rrm_geran_list->black_list_cells_to_add_mod_list_size ||
        p_rrm_geran_list->black_list_cells_to_remove_list_size)
    {
        ret = SON_TRUE;
    }

    SON_UT_TRACE_EXIT();
    return ret;
}

/*****************************************************************************
 * Function Name  : anr_get_ho_status_of_cdma_nr
 * Inputs         : p_inter_nr -Pointer to CDMA neighbor record structure
 * Outputs        : None
 * Returns        : son_ho_status_et - if handover is ALLOWED, PROHIBITED or
 *                                     BLACKLISTED.
 * Description    : This function returns HO Status of CDMA neighbors.
 ******************************************************************************/
son_ho_status_et
anr_get_ho_status_of_cdma_nr
(
    son_void_t *p_inter_nr
)
{
    son_ho_status_et ret = SON_HO_PROHIBITED;


    SON_UT_TRACE_ENTER();

    if (SON_HO_ALLOWED == ((son_cdma_neighbor_info_t *)p_inter_nr)->ho_status)
    {
        ret = SON_HO_ALLOWED;
    }
    else if (SON_NR_LOCKED  == ((son_cdma_neighbor_info_t *)p_inter_nr)->nr_status)
    {
        ret = SON_HO_UNDEFINED;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_get_ho_status_of_cdma_nr */

/*****************************************************************************
 * Function Name  : anr_get_ho_status_of_geran_nr
 * Inputs         : p_inter_nr    - Pointer to GERAN neighbor record structure.
 * Outputs        : None.
 * Returns        : son_ho_status_et - if handover is ALLOWED, PROHIBITED or
 *                                     BLACKLISTED.
 * Description    : This function returns Handover status of the GERAN
 *                  neighbors.
 ******************************************************************************/
son_ho_status_et
anr_get_ho_status_of_geran_nr
(
    son_void_t *p_inter_nr
)
{
    son_ho_status_et ret = SON_HO_PROHIBITED;


    SON_UT_TRACE_ENTER();

    if (SON_HO_ALLOWED == ((son_geran_neighbor_info_t *)p_inter_nr)->ho_status)
    {
        ret = SON_HO_ALLOWED;
    }
    else if (SON_NR_LOCKED  == ((son_geran_neighbor_info_t *)p_inter_nr)->nr_status)
    {
        ret = SON_HO_UNDEFINED;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_get_ho_status_of_geran_nr */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_inter_rat_neighbor_list_for_utran_nbr
 * Inputs         : length                - size of utran nbrs list
 *                : p_new_utran_nbr_info  - Pointer containing new utran neighbor
 *                                           record to be inserted in the list.
 *                :  is_del_nr_performed  - boolean value to determine if function is
 *                                           invoked by del nr opertion or not
 *                : p_utran_list          - Pointer to utran neighbout list.
 * Outputs        : p_utran_list          - Pointer to utran neighbout list. 
 * Returns        : son_bool_et           -(SON_TRUE/SON_FALSE)
 * Description    : This function inserts the given UTRAN neighbor in the
 *                  UTRAN neighbor list.
 ******************************************************************************/
static
son_bool_et
anr_populate_rrm_inter_rat_neighbor_list_for_utran_nbr
(
    son_rrm_utran_neighbor_list_t   *p_utran_list,
    son_u16 length,
    son_utran_neighbor_info_t       *p_new_utran_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_u16 idx = 0;
    son_bool_et ret = SON_TRUE;


    SON_UT_TRACE_ENTER();

    for (idx = 0; idx < length; idx++)
    {
        if (p_utran_list[idx].uarfcn == p_new_utran_nbr_info->uarfcn)
        {
            ret  = SON_FALSE;
            break;
        }
    }

    if (SON_TRUE == ret && length >= SON_RRM_MAX_UTRAN_FREQ_ALLOWED)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "anr_populate_rrm_inter_rat_neighbor_list_for_utran_nbr:"
            "UTRAN neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            " Max list size reached" );
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    p_utran_list[idx].uarfcn  = p_new_utran_nbr_info->uarfcn;
    if (p_utran_list[idx].cell_list_size < SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN)
    {
        SON_MEMCPY(&p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].pci,
            &p_new_utran_nbr_info->pci, sizeof(son_rrm_utran_physical_cell_id_t));

        SON_MEMCPY(&p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].cgi,
            &p_new_utran_nbr_info->nbr_cgi, sizeof(son_rrm_utran_geran_global_cell_id_t));

        if (SON_FALSE == is_del_nr_performed)
        {
            p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].cell_specific_offset
                = p_new_utran_nbr_info->cell_specific_offset;

            if (SON_UTRAN_ACCESS_MODE_PRESENT &  p_new_utran_nbr_info->bitmask)
            {
                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].access_mode
                    = p_new_utran_nbr_info->access_mode;
                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].bitmask |=
                    SON_RRM_UTRAN_ACCESS_MODE_PRESENT;
            }

            p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].rac =
                p_new_utran_nbr_info->rac;

            SON_MEMCPY(p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].lac,
                p_new_utran_nbr_info->lac, sizeof(son_u8) * SON_LAC_OCTET_SIZE);

            if (SON_UTRAN_CSG_ID_PRESENT & p_new_utran_nbr_info->bitmask)
            {
                SON_MEMCPY(p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].csg_identity,
                    p_new_utran_nbr_info->csg_identity, sizeof(son_u8) * SON_CSG_ID_OCTET_SIZE);
                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].bitmask |= SON_RRM_UTRAN_CSG_ID_PRESENT;
            }

            /*SRVCC-Start*/
            if(!(SON_UTRAN_PS_HO_SUPPORT_PRESENT & p_new_utran_nbr_info->bitmask) && !(SON_UTRAN_VOIP_CAPABLE_PRESENT & p_new_utran_nbr_info->bitmask))
            {
                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported
                    = g_config_params.anr_config_params.anr_ps_ho_supported;     //default CFG Value

                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable
                    =  g_config_params.anr_config_params.anr_voip_capable;     //default CFG Value

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_INFO,
                        "Both bitmasks,ps_ho_supported and voip_capable not set"
                        "Values of ps_ho_supported and voip_capable sent to RRM are :[%d] [%d]",
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported,
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable);
            }
            else if((SON_UTRAN_PS_HO_SUPPORT_PRESENT & p_new_utran_nbr_info->bitmask) && (SON_UTRAN_VOIP_CAPABLE_PRESENT & p_new_utran_nbr_info->bitmask))
            {
                if((p_new_utran_nbr_info->ps_ho_supported == SON_FALSE) && (p_new_utran_nbr_info->voip_capable == SON_TRUE))
                {
                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported
                        = p_new_utran_nbr_info->ps_ho_supported;

                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable
                        =  SON_FALSE;
                }
                else
                {
                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported
                        = p_new_utran_nbr_info->ps_ho_supported;

                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable
                        = p_new_utran_nbr_info->voip_capable;
                }

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_INFO,
                        "Both bitmasks,ps_ho_supported and voip_capable are set"
                        "Values of ps_ho_supported and voip_capable sent to RRM are :[%d] [%d]",
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported,
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable);
            }
            else if(!(SON_UTRAN_PS_HO_SUPPORT_PRESENT & p_new_utran_nbr_info->bitmask) && (SON_UTRAN_VOIP_CAPABLE_PRESENT & p_new_utran_nbr_info->bitmask))
            {
                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported
                    = g_config_params.anr_config_params.anr_ps_ho_supported;     //default CFG Value

                p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable
                    = p_new_utran_nbr_info->voip_capable;

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_INFO,
                        "ps_ho_supported bitmask not set and voip_capable bitmask set"
                        "Values of ps_ho_supported and voip_capable sent to RRM are :[%d] [%d]",
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported,
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable);
            }
            else if((SON_UTRAN_PS_HO_SUPPORT_PRESENT & p_new_utran_nbr_info->bitmask) && !(SON_UTRAN_VOIP_CAPABLE_PRESENT & p_new_utran_nbr_info->bitmask))
            {
                if(p_new_utran_nbr_info->ps_ho_supported == SON_FALSE)
                {
                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported
                        = p_new_utran_nbr_info->ps_ho_supported;

                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable
                        =  SON_FALSE;
                }
                else
                {
                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported
                        = p_new_utran_nbr_info->ps_ho_supported;

                    p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable
                        = g_config_params.anr_config_params.anr_voip_capable;     //default CFG Value
                }

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_INFO,
                        "ps_ho_supported bitmask set and voip_capable not set"
                        "Values of ps_ho_supported and voip_capable sent to RRM are :[%d] [%d]",
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].ps_ho_supported,
                        p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].voip_capable);
            }
            /*SRVCC-End*/

            p_utran_list[idx].cell_list[p_utran_list[idx].cell_list_size].bitmask
                |= SON_RRM_UTRAN_NR_INFO_OFFSET_PRESENT | SON_RRM_UTRAN_LAC_PRESENT |
                   SON_RRM_UTRAN_RAC_PRESENT | SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT | SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT;
            /*Bitmask SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT and SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT set for SRVCC*/
        }

        p_utran_list[idx].cell_list_size++;
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "UTRAN neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            " Max list size reached" );
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_populate_rrm_inter_rat_neighbor_list_for_utran_nbr */

/*****************************************************************************
 * Function Name  : anr_construct_rrm_inter_rat_ncl_for_utran_nbr
 * Inputs         : p_utran_nbr_info        Pointer to utran neighbor to be inserted
 *                                          in the list.
 *                  is_del_nr_performed     boolean value to determine if function is
 *                :                         invoked by del nr opertion or not
 *                : p_utran_list        -   Pointer to inter rat utran nbr list
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function constructs notification message to RRM with
 *                  UTRAN neighbors on the basis of the handover status of the
 *                  UTRAN neighbors.
 ******************************************************************************/
son_void_t
anr_construct_rrm_inter_rat_ncl_for_utran_nbr
(
    son_void_t                        *p_utran_list,
    son_void_t                        *p_utran_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_rrm_utran_neighbor_list_t   *p_list = SON_PNULL;
    son_u16                         *p_length = SON_PNULL;

    son_rrm_utran_neighbor_cell_list_t *p_utran_nbr_cell_list  = SON_PNULL;
    son_utran_neighbor_info_t          *p_new_utran_nbr_info   = SON_PNULL;


    p_utran_nbr_cell_list = (son_rrm_utran_neighbor_cell_list_t *)p_utran_list;
    p_new_utran_nbr_info  = (son_utran_neighbor_info_t *)p_utran_nbr_info;

    SON_UT_TRACE_ENTER();

    /*If HO is blacklisted*/
    if (SON_HO_PROHIBITED ==  p_new_utran_nbr_info->ho_status &&
        SON_NR_LOCKED     == p_new_utran_nbr_info->nr_status)
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list =  p_utran_nbr_cell_list->black_list_cells_to_add_mod_list;
            p_length = &p_utran_nbr_cell_list->black_list_cells_to_add_mod_list_size;
        }
        else
        {
            p_list =   p_utran_nbr_cell_list->black_list_cells_to_remove_list;
            p_length = &p_utran_nbr_cell_list->black_list_cells_to_remove_list_size;
        }
    }
    else
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list     =  p_utran_nbr_cell_list->cells_to_add_mod_list;
            p_length   =  &p_utran_nbr_cell_list->cells_to_add_mod_list_size;
        }
        else
        {
            p_list      =    p_utran_nbr_cell_list->cells_to_remove_list;
            p_length    =   &p_utran_nbr_cell_list->cells_to_remove_list_size;
        }
    }

    if (SON_PNULL != p_length && SON_PNULL != p_list)
    {
        if (SON_TRUE == anr_populate_rrm_inter_rat_neighbor_list_for_utran_nbr(p_list, *p_length,
                p_new_utran_nbr_info, is_del_nr_performed))
        {
            *p_length = *p_length + 1;
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_rrm_inter_rat_ncl_for_utran_nbr */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_inter_rat_neighbor_list_for_geran_nbr
 * Inputs         : length                  size of geran nbr list
 *                  p_new_geran_nbr_info    Pointer to  geran nbr to be inserted
 *                                          in list.
 *                  is_del_nr_performed     boolean value to determine if function is
 *                                          invoked by del nr opertion or not
 *                : p_geran_list            Pointer to geran list
 * Outputs        : p_geran_list            Pointer to geran list
 * Returns        : son_bool_et             (SON_FALSE/SON_TRUE)
 * Description    : This function constructs the GERAN neighbor list with the
 *                  given GERAN neighbor.
 ******************************************************************************/
static
son_bool_et
anr_populate_rrm_inter_rat_neighbor_list_for_geran_nbr
(
    son_rrm_geran_neighbor_list_t   *p_geran_list,
    son_u16 length,
    son_geran_neighbor_info_t       *p_new_geran_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_u16 idx = 0;
    son_bool_et ret = SON_TRUE;


    SON_UT_TRACE_ENTER();

    for (idx = 0; idx < length; idx++)
    {
        if (p_geran_list[idx].arfcn == p_new_geran_nbr_info->arfcn &&
            p_geran_list[idx].band_ind == p_new_geran_nbr_info->band_ind)
        {
            ret  = SON_FALSE;
            break;
        }
    }

    if ((SON_TRUE == ret) && (length >= SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "anr_populate_rrm_inter_rat_neighbor_list_for_geran_nbr: "
            "UTRAN neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            " Max list size reached" );

        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    p_geran_list[idx].arfcn    = p_new_geran_nbr_info->arfcn;
    p_geran_list[idx].band_ind  = p_new_geran_nbr_info->band_ind;

    if (p_geran_list[idx].cell_list_size < SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN)
    {
        SON_MEMCPY(&p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].pci,
            &p_new_geran_nbr_info->pci, sizeof(son_rrm_geran_physical_cell_id_t));

        SON_MEMCPY(&p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].cgi,
            &p_new_geran_nbr_info->nbr_cgi, sizeof(son_rrm_utran_geran_global_cell_id_t));

        if (SON_FALSE == is_del_nr_performed)
        {
            p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].cell_specific_offset
                = p_new_geran_nbr_info->cell_specific_offset;

            p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].bitmask
                |= SON_RRM_GERAN_NR_INFO_OFFSET_PRESENT;

            /*SPR-11121-Start*/
            p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].dtm_supported
                = p_new_geran_nbr_info->dtm_supported;
            p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].bitmask
                |= SON_RRM_GERAN_NR_DTM_SUPPORT_PRESENT;

            /*SPR-11121-End*/
            /*SPR 15393 Fix Start*/
            if(p_new_geran_nbr_info->bitmask & SON_GERAN_RAC_PRESENT)
            {
                p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].rac
                    = p_new_geran_nbr_info->rac; 
                p_geran_list[idx].cell_list[p_geran_list[idx].cell_list_size].bitmask 
                    |= SON_RRM_GERAN_RAC_PRESENT; 
            }
            /*SPR 15393 Fix Stop*/
        }

        p_geran_list[idx].cell_list_size++;
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "anr_populate_rrm_inter_rat_neighbor_list_for_geran_nbr: "
            "UTRAN neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            " Max list size reached" );
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_populate_rrm_inter_rat_neighbor_list_for_geran_nbr */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_inter_rat_neighbor_list_for_cdma_hrpd_nbr
 * Inputs         : length                - Size of cdma nbr list
 *                  p_new_cdma_nbr_info   - Pointer to inserted/modified cdma nbr
 *                  is_del_nr_performed   - Boolean value to determine if function is
 *                                          invoked by del nr opertion or not
 *                : p_cdma_list           - Pointer to cdma list
 * Outputs        : p_cdma_list           - Pointer to cdma list
 * Returns        : son_bool_et           - (SON_TRUE/SON_FALSE)
 * Description    : This function constructs the  CDMA Neighbor List from
 *                  given CDMA HRPD neighbor.
 ******************************************************************************/
static
son_bool_et
anr_populate_rrm_inter_rat_neighbor_list_for_cdma_hrpd_nbr
(
    son_rrm_cdma_neighbor_list_hrpd_t   *p_cdma_list,
    son_u16 length,
    son_cdma_neighbor_info_t       *p_new_cdma_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_u16 idx = 0;
    son_bool_et ret = SON_TRUE;


    SON_UT_TRACE_ENTER();

    for (idx = 0; idx < length; idx++)
    {
        if (p_cdma_list[idx].band_class == p_new_cdma_nbr_info->band_class)
        {
            ret  = SON_FALSE;
            break;
        }
    }

    if ((SON_TRUE == ret) && (length >= SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "anr_populate_rrm_inter_rat_neighbor_list_for_cdma_hrpd_nbr:"
            "CDMA HRPD neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            "Max list size reached" );

        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    p_cdma_list[idx].band_class    = p_new_cdma_nbr_info->band_class;

    if (p_cdma_list[idx].cell_list_size <= SON_RRM_CDMA_HRPD_NBR_SIZE)
    {
        p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].pci =
            p_new_cdma_nbr_info->pci;

        p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].arfcn =
            p_new_cdma_nbr_info->arfcn;

        SON_MEMCPY(&p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].cgi,
            &p_new_cdma_nbr_info->nbr_cgi,
            sizeof(son_rrm_cdma_global_cell_id_t));

        if (SON_FALSE == is_del_nr_performed)
        {
            p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].cell_specific_offset
                = p_new_cdma_nbr_info->cell_specific_offset;

            p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].bitmask
                |= SON_RRM_CDMA_HRPD_CELL_SPECIFIC_OFFSET_PRESENT;

            if (SON_CDMA_CELL_SPECIFIC_PARAMS_PRESENT & p_new_cdma_nbr_info->bitmask)
            {
                SON_MEMCPY(&p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].cell_specific_params,
                    &p_new_cdma_nbr_info->cell_specific_params,
                    sizeof(son_rrm_cdma_cell_specific_params_t));

                p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].bitmask
                    |= SON_RRM_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT;
            }
        }
    }

    p_cdma_list[idx].cell_list_size++;

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_populate_rrm_inter_rat_neighbor_list_for_cdma_hrpd_nbr */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_inter_rat_neighbor_list_for_cdma_xrtt_nbr
 * Inputs         : length              - Size of CDMA nbr list
 *                  p_new_cdma_nbr_info - Pointer to inserted/modified CDMA nbr
 *                  is_del_nr_performed - boolean value to determine if function is
 *                                        invoked by del nr opertion or not
 *                : p_cdma_list         - Pointer to CDMA XRTT nbr list
 * Outputs        : p_cdma_list         - Pointer to CDMA XRTT nbr list
 * Returns        : son_bool_et         - (SON_TRUE/SON_FALSE)
 * Description    : This function constructs with  the CDMA xrtt list with
 *                  given  CDMA neighbor.
 ******************************************************************************/
static
son_bool_et
anr_populate_rrm_inter_rat_neighbor_list_for_cdma_xrtt_nbr
(
    son_rrm_cdma_neighbor_list_xrtt_t   *p_cdma_list,
    son_u16 length,
    son_cdma_neighbor_info_t       *p_new_cdma_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_u16 idx = 0;
    son_bool_et ret = SON_TRUE;


    SON_UT_TRACE_ENTER();

    for (idx = 0; idx < length; idx++)
    {
        if (p_cdma_list[idx].band_class == p_new_cdma_nbr_info->band_class)
        {
            ret  = SON_FALSE;
            break;
        }
    }

    if (SON_TRUE == ret && length >= SON_RRM_MAX_CDMA_BAND_ALLOWED)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "anr_populate_rrm_inter_rat_neighbor_list_for_cdma_xrtt_nbr:"
            "CDMA XRTT neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            "Max list size reached" );

        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    p_cdma_list[idx].band_class    = p_new_cdma_nbr_info->band_class;

    if (p_cdma_list[idx].cell_list_size < SON_RRM_CDMA_XRTT_NBR_SIZE)
    {
        p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].pci =
            p_new_cdma_nbr_info->pci;

        p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].arfcn =
            p_new_cdma_nbr_info->arfcn;

        SON_MEMCPY(&p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].cgi,
            &p_new_cdma_nbr_info->nbr_cgi,
            sizeof(son_rrm_cdma_global_cell_id_t));

        if (SON_FALSE == is_del_nr_performed)
        {
            p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].cell_specific_offset
                = p_new_cdma_nbr_info->cell_specific_offset;

            p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].bitmask
                |= SON_RRM_CDMA_XRTT_CELL_SPECIFIC_OFFSET_PRESENT;

            if (SON_CDMA_CELL_SPECIFIC_PARAMS_PRESENT & p_new_cdma_nbr_info->bitmask)
            {
                SON_MEMCPY(&p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].cell_specific_params,
                    &p_new_cdma_nbr_info->cell_specific_params,
                    sizeof(son_rrm_cdma_cell_specific_params_t));

                p_cdma_list[idx].cell_list[p_cdma_list[idx].cell_list_size].bitmask
                    |= SON_RRM_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT;
            }
        }

        p_cdma_list[idx].cell_list_size++;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_populate_rrm_inter_rat_neighbor_list_for_cdma_xrtt_nbr */

/*****************************************************************************
 * Function Name  : anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr
 * Inputs         : p_cdma_list         - Pointer to inter rat cdma list
 *                  p_cdma_nbr_info     - Pointer to cdma nbr info
 *                  is_del_nr_performed - Boolean value to determine if function is
 *                                        invoked by del nr opertion or not
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function constructs CDMA neighbor list on the basis
 *                  of the handover status of the given  CDMA HRPD neighbor
 ******************************************************************************/
son_void_t
anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr
(
    son_void_t            *p_cdma_list,
    son_void_t            *p_cdma_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_rrm_cdma_neighbor_list_hrpd_t  *p_list = SON_PNULL;
    son_u16                         *p_length = SON_PNULL;
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_cdma_nbr_cell_list = SON_PNULL;
    son_cdma_neighbor_info_t          *p_new_cdma_nbr_info = SON_PNULL;


    p_cdma_nbr_cell_list = (son_rrm_cdma_neighbor_cell_list_hrpd_t *)p_cdma_list;
    p_new_cdma_nbr_info  = (son_cdma_neighbor_info_t *)p_cdma_nbr_info;

    SON_UT_TRACE_ENTER();

    /*If HO is blacklisted*/
    if (SON_HO_PROHIBITED ==  p_new_cdma_nbr_info->ho_status &&
        SON_NR_LOCKED     == p_new_cdma_nbr_info->nr_status)
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list =  p_cdma_nbr_cell_list->black_list_cells_to_add_mod_list;

            p_length =
                &p_cdma_nbr_cell_list->black_list_cells_to_add_mod_list_size;
        }
        else
        {
            p_list =   p_cdma_nbr_cell_list->black_list_cells_to_remove_list;

            p_length =
                &p_cdma_nbr_cell_list->black_list_cells_to_remove_list_size;
        }
    }
    else
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list     =  p_cdma_nbr_cell_list->cells_to_add_mod_list;
            p_length   =  &p_cdma_nbr_cell_list->cells_to_add_mod_list_size;
        }
        else
        {
            p_list      =    p_cdma_nbr_cell_list->cells_to_remove_list;
            p_length    =   &p_cdma_nbr_cell_list->cells_to_remove_list_size;
        }
    }

    if (SON_PNULL != p_length && SON_PNULL != p_list)
    {
        if (anr_populate_rrm_inter_rat_neighbor_list_for_cdma_hrpd_nbr( p_list, *p_length,
                p_new_cdma_nbr_info, is_del_nr_performed ))
        {
            *p_length = *p_length + 1;
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr */

/*****************************************************************************
 * Function Name  : anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr
 * Inputs         : p_cdma_nbr_info     - Pointer to CDMA nbr info
 *                  is_del_nr_performed - boolean value to determine if function is
 *                                        invoked by del nr opertion or not
 * Outputs        : p_cdma_list         - Pointer to Inter-Rat CDMA XRTT nbr list
 * Returns        : son_void_t
 * Description    : This function constructs CDMA xRTT neighbor list from given
 *                  CDMA xRTT neighbor on the basis of the handover status.
 ******************************************************************************/
son_void_t
anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr
(
    son_void_t            *p_cdma_list,
    son_void_t            *p_cdma_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_rrm_cdma_neighbor_list_xrtt_t   *p_list = SON_PNULL;
    son_u16                         *p_length = SON_PNULL;
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_cdma_nbr_cell_list = SON_PNULL;
    son_cdma_neighbor_info_t          *p_new_cdma_nbr_info = SON_PNULL;


    p_cdma_nbr_cell_list = (son_rrm_cdma_neighbor_cell_list_xrtt_t *)p_cdma_list;
    p_new_cdma_nbr_info  = (son_cdma_neighbor_info_t *)p_cdma_nbr_info;

    SON_UT_TRACE_ENTER();

    /*If HO is blacklisted*/
    if ((SON_HO_PROHIBITED == p_new_cdma_nbr_info->ho_status) &&
        (SON_NR_LOCKED == p_new_cdma_nbr_info->nr_status))
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list =  p_cdma_nbr_cell_list->black_list_cells_to_add_mod_list;

            p_length =
                &p_cdma_nbr_cell_list->black_list_cells_to_add_mod_list_size;
        }
        else
        {
            p_list =   p_cdma_nbr_cell_list->black_list_cells_to_remove_list;

            p_length =
                &p_cdma_nbr_cell_list->black_list_cells_to_remove_list_size;
        }
    }
    else
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list     =  p_cdma_nbr_cell_list->cells_to_add_mod_list;
            p_length   =  &p_cdma_nbr_cell_list->cells_to_add_mod_list_size;
        }
        else
        {
            p_list      =    p_cdma_nbr_cell_list->cells_to_remove_list;
            p_length    =   &p_cdma_nbr_cell_list->cells_to_remove_list_size;
        }
    }

    if (SON_PNULL != p_length && SON_PNULL != p_list)
    {
        if (anr_populate_rrm_inter_rat_neighbor_list_for_cdma_xrtt_nbr( p_list, *p_length,
                p_new_cdma_nbr_info, is_del_nr_performed ))
        {
            *p_length = *p_length + 1;
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr */

/*****************************************************************************
 * Function Name  : anr_construct_rrm_inter_rat_ncl_for_geran_nbr
 * Inputs         : p_geran_list         -   Pointer to inter rat geran list
 *                  p_geran_nbr_info     -   Pointer to geran nbr info
 *                : is_del_nr_performed  -   boolean value to determine if function is
 *                :                          invoked by del nr opertion or not
 *                :
 * Outputs        : p_geran_list            Pointer to inter rat geran list
 * Returns        : son_void_t
 * Description    : This function constructs the GERAN neighbor list from the
 *                  given GERAN neighbor on the basis of the Handover status.
 ******************************************************************************/
son_void_t
anr_construct_rrm_inter_rat_ncl_for_geran_nbr
(
    son_void_t            *p_geran_list,
    son_void_t            *p_geran_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_rrm_geran_neighbor_list_t   *p_list = SON_PNULL;
    son_u16                         *p_length = SON_PNULL;
    son_rrm_geran_neighbor_cell_list_t *p_geran_nbr_cell_list = SON_PNULL;
    son_geran_neighbor_info_t          *p_new_geran_nbr_info = SON_PNULL;


    p_geran_nbr_cell_list = (son_rrm_geran_neighbor_cell_list_t *)p_geran_list;
    p_new_geran_nbr_info  = (son_geran_neighbor_info_t *)p_geran_nbr_info;

    SON_UT_TRACE_ENTER();

    /*If HO is blacklisted*/
    if (SON_HO_PROHIBITED ==  p_new_geran_nbr_info->ho_status &&
        SON_NR_LOCKED     == p_new_geran_nbr_info->nr_status)
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list =  p_geran_nbr_cell_list->black_list_cells_to_add_mod_list;
            p_length = &p_geran_nbr_cell_list->black_list_cells_to_add_mod_list_size;
        }
        else
        {
            p_list =   p_geran_nbr_cell_list->black_list_cells_to_remove_list;
            p_length = &p_geran_nbr_cell_list->black_list_cells_to_remove_list_size;
        }
    }
    else
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list     =  p_geran_nbr_cell_list->cells_to_add_mod_list;
            p_length   =  &p_geran_nbr_cell_list->cells_to_add_mod_list_size;
        }
        else
        {
            p_list      =    p_geran_nbr_cell_list->cells_to_remove_list;
            p_length    =   &p_geran_nbr_cell_list->cells_to_remove_list_size;
        }
    }

    if (SON_PNULL != p_length && SON_PNULL != p_list)
    {
        if (SON_TRUE ==  anr_populate_rrm_inter_rat_neighbor_list_for_geran_nbr(
                p_list, *p_length, p_new_geran_nbr_info, is_del_nr_performed ))
        {
            *p_length = *p_length + 1;
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_rrm_inter_rat_ncl_for_geran_nbr */

/*****************************************************************************
 * Function Name  : anr_populate_rrm_intra_rat_neighbor_list
 * Inputs         : length                  - size of intra rat list
 *                : p_intra_rat_nbr_info    - Pointer to intra rat neighbor info
 *                : is_del_nr_performed     - boolean to determine operation performed
 *                : p_intra_rat_list        - Pointer to intra rat neighbor list
 * Outputs        : p_intra_rat_list        - Pointer to intra rat neighbor list
 * Returns        : son_bool_et             - (SON_TRUE/SON_FALSE)
 * Description    : This function inserts the given EUTRAN neighbor info
 *                  in the given EUTRAN neighbor list.
 ******************************************************************************/
static son_bool_et
anr_populate_rrm_intra_rat_neighbor_list
(
    son_rrm_intra_rat_neighbor_list_t *p_intra_rat_list,
    son_u16 length,
    son_intra_rat_neighbor_info_t     *p_intra_rat_nbr_info,
    son_bool_et is_del_nr_performed
)
{
    son_u16 idx = 0;
    son_bool_et ret = SON_TRUE;


    SON_UT_TRACE_ENTER();

    /*determine the matching frequency in the NCL*/
    for (idx = 0; idx < length; idx++)
    {
        if (p_intra_rat_list[idx].earfcn == p_intra_rat_nbr_info->earfcn)
        {
            ret  = SON_FALSE;
            break;
        }
    }

    /*If matching frequency is not found,and 9 freqs are already configured*/
    if (ret == SON_TRUE && length >= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            " EUTRAN neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO: "
            " EARFCNs max list size is already reached" );
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    p_intra_rat_list[idx].earfcn = p_intra_rat_nbr_info->earfcn;
    if (p_intra_rat_list[idx].cell_list_size <
        SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN)
    {
        p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].pci  = p_intra_rat_nbr_info->pci;

        SON_MEMCPY(&p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].cgi,
            &p_intra_rat_nbr_info->nbr_cgi, sizeof(son_rrm_intra_rat_global_cell_id_t));

        /*SPR_5193*/
        if (SON_FALSE == is_del_nr_performed)
        {
            p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].cell_specific_offset
                = p_intra_rat_nbr_info->cell_specific_offset;

            p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].x2_status
                = p_intra_rat_nbr_info->x2_status;

            p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].cm_cell_specific_offset
                = p_intra_rat_nbr_info->cm_cell_specific_offset;

            p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].broadcast_status
                = p_intra_rat_nbr_info->broadcast_status;
            /*SPR 14209 start*/
            
            if((p_intra_rat_nbr_info->bitmask & SON_EICIC_INFO_PRESENT )&& (p_intra_rat_nbr_info->eicic_info.bitmask & SON_CELL_INTF_SCHEME_TYPE_PRESENT))
            {   
              p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].interference_scheme
                            = p_intra_rat_nbr_info->eicic_info.interference_scheme;
            }
            else
            {
                /*SPR 22419 Fix Start*/
                /* CID 116439 Fix Start */
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].interference_scheme = 
                    (son_intf_scheme_type_et)anr_get_intf_scheme(p_intra_rat_nbr_info->access_mode); 
		/* CID 116439 Fix End */
            }    
            /*SPR 14209 end*/

            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,SON_BRIEF,"Interference_scheme [%d]",
                    p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].interference_scheme );
                /*SPR 22419 Fix Stop*/
            /** eICIC changes start */

            if (p_intra_rat_nbr_info->bitmask & SON_ACCESS_MODE_PRESENT)
            {
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].access_mode
                    = p_intra_rat_nbr_info->access_mode;
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].bitmask |=
                    SON_RRM_ACCESS_MODE_PRESENT;
            }

            if (SON_INTRA_CSG_ID_PRESENT & p_intra_rat_nbr_info->bitmask)
            {
                SON_MEMCPY(p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].csg_identity,
                    p_intra_rat_nbr_info->csg_identity, sizeof(p_intra_rat_nbr_info->csg_identity));
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].bitmask |=
                    SON_RRM_INTRA_CSG_IDENTITY_PRESENT;
            }

            SON_MEMCPY(p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].tac,
                p_intra_rat_nbr_info->tac, sizeof(p_intra_rat_nbr_info->tac));
            /*SPR 14209 start*/
            p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].bitmask
                |= SON_RRM_INTRA_RAT_NR_INFO_OFFSET_PRESENT | SON_RRM_ENABLE_X2_STATUS_PRESENT |
                   SON_RRM_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT | SON_RRM_BROADCAST_STATUS_PRESENT | SON_RRM_TAC_PRESENT | 
                                                                                                        SON_RRM_CELL_INTF_SCHEME_TYPE_PRESENT;
            /*SPR 14209 end*/
#ifdef LTE_EMBMS_SUPPORTED
            if( SON_EMBMS_MBMSFN_INFO_PRESENT & p_intra_rat_nbr_info->bitmask)
            {
                SON_MEMCPY(&(p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].mbms_config),
                        &(p_intra_rat_nbr_info->mbms_config), sizeof(son_mbms_configuration_t));
                /* SPR 18605:start */
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].bitmask 
                    |= SON_RRM_MBSFN_CONFIG_LIST_PRESENT;

                SON_LOG(
                        anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                        "setting SON_RRM_MBSFN_CONFIG_LIST_PRESENT index [%d] ", idx);
                /* SPR 18605:end */
        }

            /*SPR 18605:Fix Start*/
           if( SON_EMBMS_TDD_SF_ASSN_PRESENT & p_intra_rat_nbr_info->bitmask)
            {
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].subframe_assn = 
                        p_intra_rat_nbr_info->subframe_assn;
                p_intra_rat_list[idx].cell_list[p_intra_rat_list[idx].cell_list_size].bitmask 
                    |= SON_RRM_TDD_SF_ASSN_PRESENT;

                SON_LOG(
                        anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                        "setting SON_RRM_TDD_SF_ASSN_PRESENT index [%d] ", idx);
            }
            /*SPR 18605:Fix End*/
#endif
        }
        p_intra_rat_list[idx].cell_list_size++;
    }
    else
    {
        SON_LOG(
            anr_get_log_mode(), p_son_anr_facility_name, SON_WARNING,
            "EARFCN %u EUTRAN neighbor can not be added to SON_ANR_RRM_UPDATED_NRT_INFO"
            " Max size %u reached", p_intra_rat_list[idx].earfcn,
            p_intra_rat_list[idx].cell_list_size);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_populate_rrm_intra_rat_neighbor_list */

/*****************************************************************************
 * Function Name  : anr_send_updated_ncl_to_oam
 * Inputs         : p_notification_msg   - Pointer to notification msg
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Function to send notification message to OAM
 ******************************************************************************/
son_void_t
anr_send_updated_ncl_to_oam
(
    son_anr_updated_nrt_info_t  *p_notification_msg
)
{
    SON_UT_TRACE_ENTER();

    if (p_notification_msg->nr_list.inter_rat_nr_list_size       ||
        p_notification_msg->nr_list.intra_rat_nr_list_size)
    {
        /* Report OAM about the neighbor updated to the NRT */
        son_create_send_buffer((son_u8 *)p_notification_msg,
            SON_ANR_MODULE_ID, SON_MIF_MODULE_ID,
            SONANR_UPDATED_NRT_INFO,
            sizeof(son_anr_updated_nrt_info_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_send_updated_ncl_to_mlb
 * Inputs         : p_notification_msg    Pointer to notification msg
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Function to send notification message to MLB
 ******************************************************************************/
son_void_t
anr_send_updated_ncl_to_mlb
(
    son_anr_updated_nrt_info_t  *p_notification_msg
)
{
    SON_UT_TRACE_ENTER();

    if (p_notification_msg->nr_list.inter_rat_nr_list_size       ||
        p_notification_msg->nr_list.intra_rat_nr_list_size)
    {
        /* Report MLB about the neighbor deleted from the NRT */
        son_create_send_buffer((son_u8 *)p_notification_msg,
            SON_ANR_MODULE_ID, SON_MLB_MODULE_ID,
            SONANR_UPDATED_NRT_INFO,
            sizeof(son_anr_updated_nrt_info_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : anr_send_updated_ncl_to_rrm
 * Inputs         : p_notification_msg   - Pointer to notification msg
 *                : cell_id              - Cell identity
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : Function to send notification message to RRM
 ******************************************************************************/
son_void_t
anr_send_updated_ncl_to_rrm
(
    son_anr_rrm_updated_nrt_info_t  *p_notification_msg,
    son_u32 cell_id
)
{
    SON_UT_TRACE_ENTER();

    if (p_notification_msg->intra_rat_ncl.intra_rat_cells_to_add_mod_list_size       ||
        p_notification_msg->intra_rat_ncl.intra_rat_cells_to_remove_list_size      ||
        p_notification_msg->intra_rat_ncl.intra_rat_black_list_cells_to_add_mod_list_size ||
        p_notification_msg->intra_rat_ncl.intra_rat_black_list_cells_to_remove_list_size ||
        p_notification_msg->inter_rat_ncl.bitmask)
    {
        /*Invoke PUP tool call to send message to RRM*/
        if (SON_FAILURE ==
            son_rrm_send_son_anr_rrm_updated_nrt_info(p_notification_msg,
                SON_ANR_MODULE_ID,
                SON_RRM_MODULE_ID,
                SON_UNDEFINED,
                SON_NULL))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "Updated NRT Info could not"
                "be sent to RRM for cell [Cell Id: 0x%x]",
                cell_id);
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_INFO,
                "SON_ANR_RRM_UPDATED_NRT_INFO for cell [Cell ID: 0x%x]",
                cell_id);
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_send_updated_ncl_to_rrm */

/*****************************************************************************
 * Function Name  : anr_construct_rrm_intra_rat_ncl
 * Inputs         : p_intra_rat_hash_rec   - Pointer to intra rat hash record
 *                : is_del_nr_performed    - boolean value to determine if function is
 *                :                          invoked by del nr opertion or not
 *                :
 * Outputs        : p_intra_rat_ncl        - Pointer to intra rat neighbor cell list
 * Returns        : son_return_et
 * Description    : This function constructs  EUTRAN neighbor list from the given
 *                  EUTRAN neighbor on the basis of the handover status.
 ******************************************************************************/
son_void_t
anr_construct_rrm_intra_rat_ncl
(
    son_rrm_intra_rat_neighbor_cell_list_t  *p_intra_rat_ncl,
    anr_intra_rat_hash_rec_t                *p_intra_rat_hash_rec,
    son_bool_et is_del_nr_performed
)
{
    son_rrm_intra_rat_neighbor_list_t *p_list = SON_PNULL;
    son_u16                           *p_length = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /*If HO is blacklisted*/
    /*SPR 16620 Fix start*/
    if (((p_intra_rat_hash_rec->neighbor_info.nr_status == SON_NR_LOCKED) ||
       (p_intra_rat_hash_rec->neighbor_info.nr_status == SON_NR_UNLOCKED))  &&
        p_intra_rat_hash_rec->neighbor_info.ho_status == SON_HO_PROHIBITED)
    /*SPR 16620 Fix stop*/
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list = p_intra_rat_ncl->intra_rat_black_list_cells_to_add_mod_list;
            p_length = &p_intra_rat_ncl->intra_rat_black_list_cells_to_add_mod_list_size;
        }
        else
        {
            p_list = p_intra_rat_ncl->intra_rat_black_list_cells_to_remove_list;
            p_length = &p_intra_rat_ncl->intra_rat_black_list_cells_to_remove_list_size;
        }
    }
    else
    {
        if (SON_FALSE == is_del_nr_performed) /*addition*/
        {
            p_list = p_intra_rat_ncl->intra_rat_cells_to_add_mod_list;
            p_length = &p_intra_rat_ncl->intra_rat_cells_to_add_mod_list_size;
        }
        else
        {
            p_list = p_intra_rat_ncl->intra_rat_cells_to_remove_list;
            p_length = &p_intra_rat_ncl->intra_rat_cells_to_remove_list_size;
        }
    }

    if (SON_PNULL != p_length && SON_PNULL != p_list)
    {
        if (SON_TRUE == anr_populate_rrm_intra_rat_neighbor_list( p_list, *p_length,
                &p_intra_rat_hash_rec->neighbor_info, is_del_nr_performed))
        {
            *p_length = *p_length + 1;
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_rrm_intra_rat_ncl */

/*****************************************************************************
 * Function Name  : anr_construct_oam_updated_ncl
 * Input          : p_data            - Pointer to the neighbor record.
 *                : is_inter_rat      - Flag to determine if input record
 *                                      is intra rat or inter rat
 * Outputs        : p_nr_list_to     -  Pointer to NR list
 * Returns        : son_void_t
 * Description    : This function is to populate update notification msg to OAM
 ******************************************************************************/
son_void_t
anr_construct_oam_updated_ncl
(
    son_neighbor_info_t *p_nr_list_to,
    son_void_t          *p_data,
    son_bool_et is_inter_rat
)
{
    son_void_t *p_nr_to  = SON_PNULL;
    son_u16 length      = 0;


    SON_UT_TRACE_ENTER();
    if (SON_TRUE ==  is_inter_rat)
    {
        if (p_nr_list_to->inter_rat_nr_list_size < SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS)
        {
            p_nr_to   =  &p_nr_list_to->inter_rat_nr_list[p_nr_list_to->inter_rat_nr_list_size];
            length    =  sizeof(son_inter_rat_neighbor_info_t);
            p_nr_list_to->inter_rat_nr_list_size++;
        }
    }
    else
    {
        if (p_nr_list_to->intra_rat_nr_list_size < SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS)
        {
            p_nr_to   =  &p_nr_list_to->intra_rat_nr_list[p_nr_list_to->intra_rat_nr_list_size];
            length    =  sizeof(son_intra_rat_neighbor_info_t);
            p_nr_list_to->intra_rat_nr_list_size++;
        }
    }

    if (0 != length)
    {
        SON_MEMCPY(p_nr_to, p_data, length);
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
            "Neighbor cell can not be added to SON_OAM_ANR_UPDATED_NRT_INFO: Max list size reached");
    }

    SON_UT_TRACE_EXIT();
} /* anr_construct_oam_updated_ncl */

/******************************************************************************
* Function Name  : get_ecgi_from_enb_id_cell_id
* Inputs         : cell_id             - Cell id
*                 : p_enb_id            - Pointer to the global eNB id
* Outputs        : p_intra_rat_cell_id - Pointer to the intra rat global cell id
* Returns        : son_void_t
* Description    : This constructs the intra rat global cell id from the given
*                   cell id  and global enb id
******************************************************************************/
son_void_t
get_ecgi_from_enb_id_cell_id
(
    son_intra_rat_global_cell_id_t *p_intra_rat_cell_id,
    son_u8 cell_id,
    son_global_enb_id_t            *p_enb_id
)
{
    SON_UT_TRACE_ENTER();

    if (SON_MACRO_ENB == p_enb_id->enb_type)
    {
        SON_MEMCPY(p_intra_rat_cell_id->cell_identity,
            p_enb_id->enb_id, SON_ENB_ID_OCTET_SIZE);

        p_intra_rat_cell_id->cell_identity[2] |= (cell_id >> 4);
        p_intra_rat_cell_id->cell_identity[3]  = (cell_id << 4);
    }
    else
    {
        SON_MEMCPY(p_intra_rat_cell_id->cell_identity,
            p_enb_id->enb_id, SON_ENB_ID_OCTET_SIZE);
    }

    p_intra_rat_cell_id->plmn_id = p_enb_id->plmn_id;

    SON_UT_TRACE_EXIT();
} /* get_ecgi_from_enb_id_cell_id */

/******************************************************************************
* Function Name  : is_matching_enb_id
* Inputs         : p_enb_id        - Pointer to the eNB id
*                : enb type        - enb type
* Outputs        : p_cell_identity
* Returns        : son_bool_et     - (SON_TRUE/SON_FALSE)
* Description    : This function matches the eNB id of the cell with
*                   the given enb id
******************************************************************************/
son_bool_et
is_matching_enb_id
(
    son_u8 *p_cell_identity,
    son_u8 *p_enb_id,
    son_enb_type_et enb_type
)
{
    son_bool_et ret = SON_FALSE;


    SON_UT_TRACE_ENTER();
    if (p_cell_identity[0] == p_enb_id[0] &&
        p_cell_identity[1] == p_enb_id[1])
    {
        if (SON_MACRO_ENB == enb_type &&
            ((p_cell_identity[2] & 0xF0) == p_enb_id[2]))
        {
            ret = SON_TRUE;
        }

        /*Coverity fixes*/
        if (SON_HOME_ENB == enb_type &&
            p_cell_identity[2] == p_enb_id[2] &&
            p_cell_identity[3] == p_enb_id[3])
        {
            ret = SON_TRUE;
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* is_matching_enb_id */

/******************************************************************************
* Function Name  : is_nbr_cell_belongs_to_enb
* Inputs         : p_intra_rat_nbr_info - Pointer to intra rat neighbor info.
*                 : p_g_enb_id - Pointer to global eNB id.
* Outputs        : None.
* Returns        : son_bool_et          -(SON_FALSE/SON_TRUE)
* Description    : This function determines if the given cell belongs to the
*                 : given enb id
******************************************************************************/
son_bool_et
is_nbr_cell_belongs_to_enb
(
    /*spr 17856 fix start*/
    anr_intra_rat_hash_rec_t  *p_hash_info,
    /*spr 17856 fix stop*/
    son_global_enb_id_t       *p_g_enb_id
)
{
    son_bool_et is_plmn_id_matching = SON_FALSE;
    son_bool_et ret = SON_FALSE;
    /*spr 17856 fix start*/

    SON_UT_TRACE_ENTER();


    is_plmn_id_matching =  !SON_MEMCMP(&p_hash_info->enb_id.plmn_id,
        &p_g_enb_id->plmn_id, sizeof(son_cell_plmn_info_t))
                          ? SON_TRUE : SON_FALSE;

    if (SON_FALSE == is_plmn_id_matching)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED, "is_nbr_cell_belongs_to_enb:"
                "PLMN_id of the eNB of the given cell  is different from given eNB Id");
    }
    /* Spr 17753 Changes Start */
    else if (((SON_MACRO_ENB == p_g_enb_id->enb_type) && (SON_MACRO_ENB == p_hash_info->enb_id.enb_type)) ||
     ((SON_HOME_ENB == p_g_enb_id->enb_type) && (SON_HOME_ENB == p_hash_info->enb_id.enb_type)))
     /* Spr 17753 Changes End */
    {
          if(!SON_MEMCMP(p_g_enb_id->enb_id,p_hash_info->enb_id.enb_id,sizeof(p_hash_info->enb_id.enb_id)))
          {
              ret = SON_TRUE; 
              SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                      SON_DETAILED, "is_nbr_cell_belongs_to_enb:"
                      "Serving eNB of the given cell is same from given eNB");
          }
          else
          {
              SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                      SON_DETAILED, "is_nbr_cell_belongs_to_enb:"
                      /* Spr 17753 Changes Start */
                      "Serving eNB Id of the given cell is different from given eNB Id");
                      /* Spr 17753 Changes End */
          }
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_DETAILED, "is_nbr_cell_belongs_to_enb:"
                       "Serving eNB of the given cell is different from given eNB");
    }
    /*spr 17856 fix stop*/
    SON_UT_TRACE_EXIT();
    return ret;
} /* is_nbr_cell_belongs_to_enb */

/******************************************************************************
* Function Name  :anr_update_x2_status_nbr_cells_nrt
* Inputs         : p_enb_id - Pointer to the global eNB id
* Outputs        : None.
* Returns        : son_void_t
* Description    : This function  updates the x2 status of all the cells of given
*                   enB id present in NRT.
******************************************************************************/
son_void_t
anr_update_x2_status_nbr_cells_nrt
(
    son_global_enb_id_t      *p_enb_id
)
{
    anr_cell_context_node_t   *p_cell_ctxt_tpl              = SON_PNULL;
    anr_cell_context_t        *p_cell_ctxt                  = SON_PNULL;
    son_anr_rrm_updated_nrt_info_t      *p_notification_msg = SON_PNULL;
    son_anr_updated_nrt_info_t          *p_oam_notif        = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t   *p_tpl       = SON_PNULL;
    anr_intra_rat_hash_rec_t        *p_hash_rec  = SON_PNULL;
    anr_intra_rat_hash_rec_t tmp_hash_rec;
    SON_HASH *p_hash = SON_PNULL;
    son_error_et error_code = SON_NO_ERROR;


    SON_UT_TRACE_ENTER();
    p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)
                         son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
    if (SON_PNULL == p_notification_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_update_x2_status_nbr_cells_nrt:"
                       "Memory Allocation failure");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_oam_notif = (son_anr_updated_nrt_info_t *)
                  son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_oam_notif)
    {
        if (SON_PNULL != p_notification_msg)
        {
            son_mem_free(p_notification_msg);
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_update_x2_status_nbr_cells_nrt:"
                       "Memory Allocation failure");
        SON_UT_TRACE_EXIT();
        return;
    }

    p_cell_ctxt_tpl =  anr_get_first_cell_ctxt();

    while (SON_PNULL != p_cell_ctxt_tpl)
    {
        p_cell_ctxt = &p_cell_ctxt_tpl->data;

        SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
        SON_MEMSET(p_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));

        SON_MEMCPY(&p_notification_msg->src_cgi, &p_cell_ctxt->cell_id,
            sizeof(son_rrm_intra_rat_global_cell_id_t));

        SON_MEMCPY(&p_oam_notif->src_cgi, &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));

        p_hash = &p_cell_ctxt->intra_rat_neighbor_table;
        p_tpl = anr_get_first_nr_from_intra_rat_hash_table(p_hash);

        while (SON_PNULL != p_tpl)
        {
            p_hash_rec = &p_tpl->data;

            /*validate the enbid of the cell*/
            /*spr 17856 fix start*/
            if (is_nbr_cell_belongs_to_enb(p_hash_rec, p_enb_id))
            /*spr 17856 fix stop*/
            {
                SON_MEMSET(&tmp_hash_rec, 0, sizeof(tmp_hash_rec));
                SON_MEMCPY(&tmp_hash_rec.neighbor_info.nbr_cgi,
                    &p_hash_rec->neighbor_info.nbr_cgi,
                    sizeof(son_intra_rat_global_cell_id_t));

                /* SPR 10103 FIX START */
                SON_MEMCPY(&tmp_hash_rec.enb_id, p_enb_id, sizeof(son_global_enb_id_t));
                if (p_hash_rec->neighbor_info.bitmask & SON_ACCESS_MODE_PRESENT)
                {
                    tmp_hash_rec.neighbor_info.bitmask |= SON_ACCESS_MODE_PRESENT;
                    tmp_hash_rec.neighbor_info.access_mode = p_hash_rec->neighbor_info.access_mode;
                }
                if(p_hash_rec->neighbor_info.bitmask & SON_INTRA_CSG_ID_PRESENT)
                {
                    tmp_hash_rec.neighbor_info.bitmask |= SON_INTRA_CSG_ID_PRESENT;
                    SON_MEMCPY(&tmp_hash_rec.neighbor_info.csg_identity,
                            &p_hash_rec->neighbor_info.csg_identity, sizeof(p_hash_rec->neighbor_info.csg_identity));
                }
                /*SPR-10230-Start*/
                if(p_hash_rec->neighbor_info.bitmask & SON_PCI_PRESENT)
                {
                    tmp_hash_rec.neighbor_info.bitmask |= SON_PCI_PRESENT;
                    tmp_hash_rec.neighbor_info.pci = p_hash_rec->neighbor_info.pci; 
                }
                if(p_hash_rec->neighbor_info.bitmask & SON_EARFCN_PRESENT)
                {
                    tmp_hash_rec.neighbor_info.bitmask |= SON_EARFCN_PRESENT;
                    tmp_hash_rec.neighbor_info.earfcn = p_hash_rec->neighbor_info.earfcn;
                }
                /*SPR-10230-End*/
                /* SPR 10103 FIX END */

                anr_intra_rat_modify_nr(p_cell_ctxt,
                    &tmp_hash_rec,
                    SON_FALSE,
                    &error_code,
                    SON_PNULL,
                    p_oam_notif,
                    p_notification_msg,
                    SON_TRUE,
                    SON_FALSE);
            }

            p_tpl = anr_get_next_nr_from_intra_rat_hash_table(p_hash,
                &p_tpl->anchor);
        }

        anr_send_updated_ncl_to_oam(p_oam_notif);
        anr_send_updated_ncl_to_rrm(p_notification_msg,
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
        p_cell_ctxt_tpl = anr_get_next_node(p_cell_ctxt_tpl);
    }

    if (SON_PNULL != p_oam_notif)
    {
        son_mem_free(p_oam_notif);
    }

    if (SON_PNULL != p_notification_msg)
    {
        son_mem_free(p_notification_msg);
    }

    SON_UT_TRACE_EXIT();
} /* anr_update_x2_status_nbr_cells_nrt */

/******************************************************************************
* Function Name  : anr_delete_nbr_cells_nrt
* Inputs         : p_enb_id  - pointer to  global eNB Id.
* Outputs        : None.
* Returns        : son_void_t
* Description    : This function deletes all the cells served by given eNB Id
*                   from the NRT and place them in "the removed list"
******************************************************************************/
son_void_t
anr_delete_nbr_cells_nrt
(
    son_global_enb_id_t  *p_enb_id
)
{
    anr_cell_context_node_t        *p_cell_ctxt_tpl          = SON_PNULL;
    anr_cell_context_t             *p_cell_ctxt              = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t  *p_intra_rat_hash_tpl     = SON_PNULL;
    anr_intra_rat_hash_rec_t       *p_intra_rat_hash_rec     = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t  *p_nxt_intra_rat_hash_tpl = SON_PNULL;
    son_anr_rrm_updated_nrt_info_t *p_notification_msg       = SON_PNULL;
    son_anr_updated_nrt_info_t     *p_oam_notif              = SON_PNULL;
    anr_intra_rat_hash_rec_t hash_rec;
    son_global_cell_id_t cell_id              = {0};
    son_bool_et is_enb_id_matching      = SON_FALSE;
    son_error_et error_code = SON_NO_ERROR;
    son_x2_nrt_update_ind_t nrt_upd_ind;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_enb_id)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_delete_nbr_cells_nrt:Enb Id is NULL");

        SON_UT_TRACE_EXIT();
        return;
    }

    p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)
                         son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
    if (SON_PNULL == p_notification_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_delete_nbr_cells_nrt:Memory Allocation failure");

        SON_UT_TRACE_EXIT();
        return;
    }

    p_oam_notif = (son_anr_updated_nrt_info_t *)
                  son_mem_get(sizeof(son_anr_updated_nrt_info_t));

    if (SON_PNULL == p_oam_notif)
    {
        if (SON_PNULL != p_notification_msg)
        {
            son_mem_free(p_notification_msg);
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_delete_nbr_cells_nrt:Memory Allocation failure");

        SON_UT_TRACE_EXIT();
        return;
    }

    SON_MEMSET(&nrt_upd_ind, 0, sizeof(son_x2_nrt_update_ind_t));

    p_cell_ctxt_tpl =  anr_get_first_cell_ctxt();

    /*Traverse all the serving cells*/
    while (SON_PNULL != p_cell_ctxt_tpl)
    {
        son_return_et ret  = SON_FAILURE;

        p_cell_ctxt = &p_cell_ctxt_tpl->data;

        error_code = SON_NO_ERROR;

        SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
        SON_MEMSET(p_oam_notif, 0, sizeof(son_anr_updated_nrt_info_t));

        /*Set the serving cell id in notification msg*/
        SON_MEMCPY(&p_notification_msg->src_cgi, &p_cell_ctxt->cell_id,
            sizeof(son_rrm_intra_rat_global_cell_id_t));

        SON_MEMCPY(&p_oam_notif->src_cgi, &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));

        p_intra_rat_hash_tpl = anr_get_first_nr_from_intra_rat_hash_table(
            &p_cell_ctxt->intra_rat_neighbor_table);

        /*traversing all the intra rat NRs of the given serving cell */
        while (SON_PNULL != p_intra_rat_hash_tpl)
        {
            SON_MEMSET(&hash_rec, 0, sizeof(hash_rec));

            p_intra_rat_hash_rec = &p_intra_rat_hash_tpl->data;

            /*spr 17856 fix start*/
            is_enb_id_matching = is_nbr_cell_belongs_to_enb(
                p_intra_rat_hash_rec, p_enb_id);
            /*spr 17856 fix stop*/

            /*1. Get the next intra rat neighbor*/
            p_nxt_intra_rat_hash_tpl  = anr_get_next_nr_from_intra_rat_hash_table(
                &p_cell_ctxt->intra_rat_neighbor_table,
                &p_intra_rat_hash_tpl->anchor);

            if (SON_TRUE == is_enb_id_matching)
            {
                /*2.Create the global cell id structure*/
                anr_convert_intra_rat_nr_to_global_cell_id_struct(
                    &p_intra_rat_hash_rec->neighbor_info, &cell_id);

                /*ENB CONFIG CHANGE REQ SPR:START
                 *3. Delete the neighbor from NRT*/
                if (SON_SUCCESS == anr_intra_rat_del_nr(p_cell_ctxt,
                        &p_intra_rat_hash_rec->neighbor_info.nbr_cgi,
                        /* SPR 17889 Fix Start */
                        SON_FALSE,
                        /* SPR 17889 Fix Stop */
                        &error_code,
                        &hash_rec,
                        p_oam_notif,
                        p_notification_msg,
                        SON_TRUE))
                {
                    ret = SON_SUCCESS;
                }

                /*ENB CONFIG CHANGE REQ SPR:STOP*/
            }

            /*Putting the next intra rat hash tuple*/
            p_intra_rat_hash_tpl = p_nxt_intra_rat_hash_tpl;
        } /*end of traversing all the intra rat NRs of the given serving cell*/

        /*Send the RRM UPDATED NRT Info to the RRM */
        anr_send_updated_ncl_to_rrm(p_notification_msg,
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));

        /*popualte updated_nrt top X2
         *ENB CONFIG CHANGE REQ SPR:START*/
        if (SON_SUCCESS == ret)
        {
            anr_populate_x2_nrt_update_ind(&nrt_upd_ind,
                &p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
        }

        /*ENB CONFIG CHANGE REQ SPR:STOP*/

        anr_send_updated_ncl_to_oam(p_oam_notif);

        /*Loop to the new Cell context*/
        p_cell_ctxt_tpl = anr_get_next_node(p_cell_ctxt_tpl);
    } /*end of traversing all the serving cells.*/
    /*SPR_16835_START*/ 	
    if (nrt_upd_ind.num_srv_cells_list_size && (SON_TRUE == is_x2_connection_available()))
    /*SPR_16835_END*/ 	
    {
        son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
            sizeof(nrt_upd_ind));

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF, "anr_delete_nbr_cells_nrt:SONX2 NRT UPDATE sent to X2 ");
    }

    if (SON_PNULL != p_notification_msg)
    {
        son_mem_free(p_notification_msg);
    }

    if (SON_PNULL != p_oam_notif)
    {
        son_mem_free(p_oam_notif);
    }

    SON_UT_TRACE_EXIT();
} /* anr_delete_nbr_cells_nrt */

/*****************************************************************************
 * Function Name  : is_rrm_notification_required
 * Inputs         : p_old_nr            -  Pointer to old nr record
 *                : p_new_nr            -  Pointer to modified nr record
 * Outputs        : None.
 * Returns        : son_bool_et
 * Description    : This function determines if there is any difference in the
 *                  two given neighbor records for the parameters provided to
 *                  RRM in notification messages.
 ******************************************************************************/
static
son_bool_et
is_rrm_notification_required
(
    son_intra_rat_neighbor_info_t *p_old_nr,
    son_intra_rat_neighbor_info_t *p_new_nr
)
{
    son_bool_et ret = SON_FALSE;


    SON_UT_TRACE_ENTER();

    /*SPR_5193*/
    if ((p_old_nr->pci != p_new_nr->pci) || (p_old_nr->earfcn != p_new_nr->earfcn) ||
        !(p_old_nr->bitmask & SON_CELL_OFFSET_PRESENT) || !(p_old_nr->bitmask & SON_TAC_PRESENT) ||
        (p_old_nr->cell_specific_offset != p_new_nr->cell_specific_offset) ||
        (p_old_nr->x2_status != p_new_nr->x2_status) ||
        SON_MEMCMP(p_old_nr->tac, p_new_nr->tac, sizeof(p_old_nr->tac)))
    {
        ret = SON_TRUE;
    }

    /*SPR_5193*/
    SON_UT_TRACE_EXIT();
    return ret;
} /* is_rrm_notification_required */

/*****************************************************************************
 * Function Name  : anr_validate_construct_rrm_intra_rat_ncl_on_nrt_update
 * Inputs         : p_intra_rat_ncl       -  Pointer to NR List.
 *                : p_old_nr              -  Pointer to Old nr record
 *                : p_new_nr              -  Pointer to New nr record
 * Outputs        :
 * Returns        : son_void_t
 * Description    : This function processes the Intra-RAT Neighbors for the
 *                  notification to the RRM when there is an update in the
 *                  parameter values of the existing neigbor.
 ******************************************************************************/
son_void_t
anr_validate_construct_rrm_intra_rat_ncl_on_nrt_update
(
    son_rrm_intra_rat_neighbor_cell_list_t    *p_intra_rat_ncl,
    anr_intra_rat_hash_rec_t                  *p_old_nr,
    anr_intra_rat_hash_rec_t                  *p_new_nr
)
{
    son_bool_et is_cso_pci_earfcn_changed = SON_FALSE;
    son_bool_et is_old_nr_send            = SON_FALSE;
    son_bool_et is_new_nr_sendable        = SON_FALSE;
    son_ho_status_et old_nr_ho_status         = SON_HO_PROHIBITED;
    son_ho_status_et new_nr_ho_status         = SON_HO_PROHIBITED;


    SON_UT_TRACE_ENTER();

    /*Check if CSO ,PCI ,EARFCN is changed*/
    is_cso_pci_earfcn_changed = is_rrm_notification_required(&p_old_nr->neighbor_info,
        &p_new_nr->neighbor_info );

    old_nr_ho_status = anr_get_intra_rat_ho_status(p_old_nr );

    /* Determine if the OLD NR was sent to RRM */
    if ((p_old_nr->neighbor_info.bitmask & SON_CELL_OFFSET_PRESENT) &&
        SON_HO_PROHIBITED != old_nr_ho_status)
    {
        is_old_nr_send = SON_TRUE;
    }

    new_nr_ho_status = anr_get_intra_rat_ho_status(p_new_nr);
    /* Determine if the NEW NR is to be send to RRM */
    if (SON_HO_PROHIBITED != new_nr_ho_status)
    {
        is_new_nr_sendable = SON_TRUE;
    }

    if (SON_FALSE == is_new_nr_sendable) /*If New NR should not be sent to RRM*/
    {
        if (SON_TRUE == is_old_nr_send) /*Old NR was send to RRM*/
        {
            /*Place the Old NR in the removal list*/
            anr_construct_rrm_intra_rat_ncl(p_intra_rat_ncl, p_old_nr, SON_TRUE);
        }
    }
    else if (SON_FALSE == is_old_nr_send) /*If new NR should be send and Old NR was not send*/
    {
        /*Place the New NR in the addition list*/
        anr_construct_rrm_intra_rat_ncl(p_intra_rat_ncl, p_new_nr, SON_FALSE);
    }
    else /*If New NR should be send and OLd NR was also send*/
    {
        /*If Any parameter has changed*/
        if ((new_nr_ho_status != old_nr_ho_status) || (SON_TRUE == is_cso_pci_earfcn_changed))
        {
            /*Place the New NR in the Addition list*/
            anr_construct_rrm_intra_rat_ncl(p_intra_rat_ncl, p_new_nr, SON_FALSE);
            /*If EARFCN or HO STATUS have changed*/
            if ((old_nr_ho_status != new_nr_ho_status) ||
                (p_old_nr->neighbor_info.earfcn != p_new_nr->neighbor_info.earfcn))
            {
                anr_construct_rrm_intra_rat_ncl(p_intra_rat_ncl, p_old_nr, SON_TRUE);
            }
        }
    }

    SON_UT_TRACE_EXIT();
} /* anr_validate_construct_rrm_intra_rat_ncl_on_nrt_update */

/*****************************************************************************
 * Function Name  : anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor
 * Inputs         : p_nr_mdfd             -  Pointer to the Intra rat hash record
 * Outputs        :
 * Returns        : son_void_t
 * Description    : This function determines sets EARFCNs(UL/DL) on the basis of
                    provided UL/DL EARFCNs
 ******************************************************************************/
 son_void_t
 anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor
 (
   anr_intra_rat_hash_rec_t *p_nr_mdfd  
 )
{
    SON_UT_TRACE_ENTER();
    if(!(SON_UL_EARFCN_PRESENT & p_nr_mdfd->neighbor_info.bitmask))
    {
        if(SON_EARFCN_PRESENT & p_nr_mdfd->neighbor_info.bitmask)
        {
            if(SON_MAX_EARFCN_LIMIT  < p_nr_mdfd->neighbor_info.earfcn)
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_BRIEF, 
                        "The value of the DL EARFCN is :[%d] >[%d]."
                        "So UL EARFCN can not be computed",p_nr_mdfd->neighbor_info.earfcn,
                        SON_MAX_EARFCN_LIMIT);
            }
            else if((SON_MIN_VALUE_TDD_EARFCN <= p_nr_mdfd->neighbor_info.earfcn)&&
                    (SON_MAX_VALUE_TDD_EARFCN >= p_nr_mdfd->neighbor_info.earfcn))
            {
                p_nr_mdfd->neighbor_info.bitmask |= SON_UL_EARFCN_PRESENT;
                p_nr_mdfd->neighbor_info.ul_earfcn = p_nr_mdfd->neighbor_info.earfcn;
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_BRIEF, 
                        "The value of the UL EARFCN computed is :[%d]",
                        p_nr_mdfd->neighbor_info.ul_earfcn);
            }
            else
            {
                p_nr_mdfd->neighbor_info.bitmask |= SON_UL_EARFCN_PRESENT;
                p_nr_mdfd->neighbor_info.ul_earfcn = (p_nr_mdfd->neighbor_info.earfcn +
                        SON_DIFF_IN_UL_DL_EARFCN);
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_BRIEF, 
                        "The value of the UL EARFCN computed is :[%d]",
                        p_nr_mdfd->neighbor_info.ul_earfcn);
            }
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF,
                    "The value of the DL EARFCN not available");
        }
    }
    else if(!(SON_EARFCN_PRESENT & p_nr_mdfd->neighbor_info.bitmask))
    {
        if(SON_DIFF_IN_UL_DL_EARFCN > p_nr_mdfd->neighbor_info.ul_earfcn)
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, 
                    "The value of the UL EARFCN is :[%d] < [%d]."
                    "So DL EARFCN can not be computed",
                    p_nr_mdfd->neighbor_info.ul_earfcn,
                    SON_DIFF_IN_UL_DL_EARFCN);
        }
        else if((SON_MIN_VALUE_TDD_EARFCN <= p_nr_mdfd->neighbor_info.ul_earfcn)&&
                (SON_MAX_VALUE_TDD_EARFCN >= p_nr_mdfd->neighbor_info.ul_earfcn))
        {
            p_nr_mdfd->neighbor_info.bitmask |= SON_EARFCN_PRESENT;
            p_nr_mdfd->neighbor_info.earfcn = p_nr_mdfd->neighbor_info.ul_earfcn;
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, 
                    "The value of the DL EARFCN computed is :[%d]",
                    p_nr_mdfd->neighbor_info.earfcn);
        }
        else
        {
            p_nr_mdfd->neighbor_info.bitmask |= SON_EARFCN_PRESENT;
            p_nr_mdfd->neighbor_info.earfcn = 
                (p_nr_mdfd->neighbor_info.ul_earfcn - SON_DIFF_IN_UL_DL_EARFCN);
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, 
                    "The value of the DL EARFCN computed is :[%d]",
                    p_nr_mdfd->neighbor_info.earfcn);
        }
    }

    SON_UT_TRACE_EXIT();
}
/* Spr 17753 Changes Start */
/*****************************************************************************
 * Function Name  : son_convert_plmd_to_three_byte_form
 * Inputs         : p_son_plmn_id     - Pointer to the plmnid  
 *                : p_plmn_id_3byte   - Pointer to array
 * Outputs        : plmnid in three byte form
 * Returns        : son_void_t
 * Description    : This function convert the PLMN ID structure into array 
 ******************************************************************************/
son_void_t
son_convert_plmd_to_three_byte_form
(
    son_cell_plmn_info_t *p_son_plmn_id,
    son_u8 * p_plmn_id_3byte
)
{
    SON_UT_TRACE_ENTER();

    p_plmn_id_3byte[0] = p_son_plmn_id->mcc[0] |
        (p_son_plmn_id->mcc[1] << 4);

    if (2 == p_son_plmn_id->num_mnc_digit)
    {
        /* Add the filler digit as only 2 mnc digits are there */
        p_plmn_id_3byte[1] = (p_son_plmn_id->mcc[2]) | 0xF0;
        p_plmn_id_3byte[2] = p_son_plmn_id->mnc[0] |
            (p_son_plmn_id->mnc[1] << 4);
    }
    else
    {
        p_plmn_id_3byte[1] = (p_son_plmn_id->mcc[2]) |
            (p_son_plmn_id->mnc[0] << 4);
        p_plmn_id_3byte[2] = (p_son_plmn_id->mnc[2] << 4) |
            p_son_plmn_id->mnc[1];
    }

    SON_UT_TRACE_EXIT();
}


/**********************************************************************
 * Function Name  : son_ecgi_to_u64
 * Inputs         : p_ecgi - Pointer to the eCGI
 * Outputs        : 
 * Returns        : son_u64 
 * Description    : This function convert the cgi to son_u64 
 **********************************************************************/
son_u64 son_ecgi_to_u64
(
 son_intra_rat_global_cell_id_t *p_ecgi
 )
{
    son_u64 ecgi_u64 = 0;
    son_u8  plmn_id_3byte[3] = {0};

    SON_UT_TRACE_ENTER();

    /* Convert plmn id into array */
    son_convert_plmd_to_three_byte_form(&p_ecgi->plmn_id,plmn_id_3byte);

    ecgi_u64 |= plmn_id_3byte[0] & 0xFF;
    ecgi_u64 <<= 8;
    ecgi_u64 |= plmn_id_3byte[1] & 0xFF;
    ecgi_u64 <<= 8;
    ecgi_u64 |= plmn_id_3byte[2] & 0xFF;
    ecgi_u64 <<= 8;
    
    ecgi_u64 |= p_ecgi->cell_identity[0] & 0xFF;
    ecgi_u64 <<= 8;
    ecgi_u64 |= p_ecgi->cell_identity[1] & 0xFF;
    ecgi_u64 <<= 8;
    ecgi_u64 |= p_ecgi->cell_identity[2] & 0xFF;
    ecgi_u64 <<= 8;
    ecgi_u64 |= p_ecgi->cell_identity[3] & 0xFF;

    SON_UT_TRACE_EXIT();
    return ecgi_u64;
}

/**********************************************************************
 * Function Name  : son_check_is_serving_cgi_greater_then_nbr_cgi 
 * Inputs         : p_seving_ecgi - Pointer to servinf ecgi
 *                  p_nbr_ecgi   - Pointer to the nbr eCGI
 * Outputs        : 
 * Returns        : son_bool_et 
 * Description    : This function check if servinf ecgi is greater than
 *                  nbr ecgi 
 **********************************************************************/
son_bool_et 
son_check_is_serving_cgi_greater_then_nbr_cgi
(
 son_intra_rat_global_cell_id_t *p_seving_ecgi,
 son_intra_rat_global_cell_id_t *p_nbr_ecgi
 )
{
    son_u64 servingEcgi                             = 0;                                
    son_u64 neighborEcgi                            = 0;                                  
    son_bool_et retVal                              = SON_FALSE; 

    SON_UT_TRACE_ENTER();

    servingEcgi  = son_ecgi_to_u64(p_seving_ecgi);  
    neighborEcgi = son_ecgi_to_u64(p_nbr_ecgi);

    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
            "son_check_is_serving_cgi_greater_then_nbr_cgi: servingEcgi[%llu]"
            "neighborEcgi[%llu]",servingEcgi,neighborEcgi);

    if(servingEcgi > neighborEcgi)
    {
        retVal = SON_TRUE;
    }
    else
    {
        retVal = SON_FALSE;
    }
    SON_UT_TRACE_EXIT();
    return retVal;
}

/**********************************************************************
 * Function Name  : son_anr_get_random_timer_val 
 * Inputs         : time_duration - time_duration configured by OAM
 *                : randomizer -  used for calculating the 
 *                : the random value (randomizer mod value) 
 * Outputs        : 
 * Returns        : son_timer_duration_t 
 * Description    : This function return the random timer value between 
 *                  the zero and timer val configured by operator
 **********************************************************************/
son_timer_duration_t
son_anr_get_random_timer_val
(
 son_timer_duration_t time_val,
/* SPR 23260 + */
#ifndef __x86_64__ 
son_u32 randomizer
#else
son_u64 randomizer
#endif
/* SPR 23260 - */
 )
{
    son_u32 random_val                              = 0;        

    SON_UT_TRACE_ENTER();
    
    if(SON_NULL != randomizer)
    {
        random_val = randomizer % time_val;

        if(random_val == SON_NULL)
        {
            random_val = time_val;
        }
    }
    else
    {
        random_val = time_val;
    }

    SON_UT_TRACE_EXIT();
    return random_val;
}
/* Spr 17753 Changes End */
/*SPR 19030 changes start*/
/*****************************************************************************
 * Function Name  : son_anr_get_serving_enb_id
 * Inputs         : cell_id-  
 * Outputs        : serving cell id
 * Returns        : son_void_t
 * Description    : This function check that source enb is home or macro
                    after that made the enb id
 ******************************************************************************/
son_bool_et 
son_anr_get_serving_enb_id 
 (
   son_intra_rat_global_cell_id_t      cell_id, 
   son_global_enb_id_t                 *src_enbid
 )
{
  anr_cell_context_t          *p_anr_cell_ctxt = SON_PNULL;
  son_u16                     serving_pci = SON_NULL;
  son_u16                     serving_earfcn = SON_NULL;
  anr_intra_freq_list_node_t  *p_intra_freq_list_node = SON_PNULL;
  son_u16                      temp_open_pci_end = 0;
  son_u16                      temp_closed_pci_end = 0;
  son_u16                      temp_hybrid_pci_end = 0;
  son_bool_et                  ret_val = SON_TRUE;
  son_bool_et                  is_macro_nbr = SON_TRUE;

  SON_UT_TRACE_ENTER();
  p_anr_cell_ctxt = anr_get_cell_context_from_global_context(&cell_id);
  /* SPR 19221: CID 108602 fix start */
  if (SON_PNULL == p_anr_cell_ctxt)
  {
      SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,"unable to fetch anr cell context from global context");
      return SON_FALSE;
  }
  /* SPR 19221: CID 108602 fix stop */
  serving_earfcn = anr_get_earfcn(p_anr_cell_ctxt);
  serving_pci = p_anr_cell_ctxt->pci_value; 
  p_intra_freq_list_node = anr_find_freq_in_intra_freq_list(serving_earfcn);

  if(p_intra_freq_list_node != SON_PNULL )
  {
    if(p_intra_freq_list_node->intra_freq_value.open_pci_range_present == SON_TRUE)
    {
      temp_open_pci_end = son_determine_pci_range_end_value(p_intra_freq_list_node->intra_freq_value.intra_freq_open_pci_range.pci_range);
      if((serving_pci >=  p_intra_freq_list_node->intra_freq_value.intra_freq_open_pci_range.pci_start) &&
          (serving_pci <= (p_intra_freq_list_node->intra_freq_value.intra_freq_open_pci_range.pci_start + temp_open_pci_end -1)))
      {
        is_macro_nbr = SON_FALSE;
      }
    }

    if(p_intra_freq_list_node->intra_freq_value.closed_pci_range_present == SON_TRUE)
    {
      /*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
      temp_closed_pci_end = son_determine_pci_range_end_value(p_intra_freq_list_node->intra_freq_value.intra_freq_closed_pci_range.pci_range);
      if((serving_pci >=  p_intra_freq_list_node->intra_freq_value.intra_freq_closed_pci_range.pci_start) &&
          (serving_pci <= (p_intra_freq_list_node->intra_freq_value.intra_freq_closed_pci_range.pci_start + temp_closed_pci_end -1)))
      {
        is_macro_nbr = SON_FALSE;
      }
    }

    if(p_intra_freq_list_node->intra_freq_value.hybrid_pci_range_present == SON_TRUE)
    {
      /*SPR-11554-Start-End-Function to determine PCI range end value shifted to common area,hence name of function changed*/
      temp_hybrid_pci_end = son_determine_pci_range_end_value(p_intra_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range.pci_range);
      if((serving_pci >=  p_intra_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range.pci_start) &&
          (serving_pci <= (p_intra_freq_list_node->intra_freq_value.intra_freq_hybrid_pci_range.pci_start + temp_hybrid_pci_end -1)))
      {
        is_macro_nbr = SON_FALSE;
      }
    }
    if (is_macro_nbr == SON_FALSE)
    {
      src_enbid->enb_type = SON_HOME_ENB;
      src_enbid->enb_id[0] =  cell_id.cell_identity[0]; 
      src_enbid->enb_id[1] =  cell_id.cell_identity[1]; 
      src_enbid->enb_id[2] =  cell_id.cell_identity[2]; 
      src_enbid->enb_id[3] =  cell_id.cell_identity[3] & 0xF0;
      SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
          SON_BRIEF,"Serving cell is on home eNb");
    }
    else
    {
      src_enbid->enb_type = SON_MACRO_ENB;
      src_enbid->enb_id[0] = cell_id.cell_identity[0];
      src_enbid->enb_id[1] = cell_id.cell_identity[1];
      src_enbid->enb_id[2] = cell_id.cell_identity[2] & 0xF0;

      SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,SON_DETAILED, 
          "Serving cell is on MACRO eNb");
    }
  }
  else
  {
    ret_val = SON_FALSE;
    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
        SON_ERROR,"p_intra_freq_list_node is empty ");
  }
  SON_UT_TRACE_EXIT();
  return ret_val;
}
/*SPR 19030 changes end*/
