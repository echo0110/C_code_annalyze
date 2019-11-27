/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_rrc_asn1_convertor.c,v 1.2 2010/03/24 10:42:40 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : Converts from RRM interface structures into RRC ASN1
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rrm_rrc_asn1_convertor.c,v $
 * Revision 1.2  2010/03/24 10:42:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.1.2.2  2010/02/01 07:58:00  gur18569
 * removed tabs
 *
 * Revision 1.1.2.1  2010/01/14 14:34:17  ukr15916
 * redirectedCarrierInfo and idleModeMobilityControlInfo RRM->RRC ASN message construction added
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrm_rrc_asn1_convertor.h"
#include "rrc_rrm_intf.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#define ARRSIZE(array_name)     (sizeof(array_name) / sizeof(array_name[0]))

/******************************************************************************
 *   FUNCTION NAME: check_and_convert_carrier_freqs_geran_following_arfcns
 *   INPUT        : OSCTXT                            *p_asn1_ctx
 *                  CarrierFreqsGERAN_followingARFCNs *p_asn1_arfcns
 *                  geran_following_arfcns_t          *p_arfcns
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS - if all data are valid
 *                  RRC_FAILURE - if data is invalid or if there are not enough memory
 *   DESCRIPTION  : This function convert geran_following_arfcns_t to
 *                  CarrierFreqsGERAN_followingARFCNs
 *                  and check if input data are valid.
 ******************************************************************************/
rrc_return_et check_and_convert_carrier_freqs_geran_following_arfcns
(
 /* for memory allocation */
 OSCTXT                            *p_asn1_ctx,
 CarrierFreqsGERAN_followingARFCNs *p_asn1_arfcns,  /* destination */
 geran_following_arfcns_t          *p_arfcns        /* source */
 )
{
    rrc_counter_t   index = RRC_NULL;
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_arfcns);
    RRC_ASSERT(PNULL != p_arfcns);

    p_asn1_arfcns->t = 0;

    if (GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG == p_arfcns->presence_bitmask)
    {
        p_asn1_arfcns->t =
            T_CarrierFreqsGERAN_followingARFCNs_explicitListOfARFCNs;
        p_asn1_arfcns->u.explicitListOfARFCNs = rtxMemAllocType(p_asn1_ctx,
                ExplicitListOfARFCNs);
        if (PNULL == p_asn1_arfcns->u.explicitListOfARFCNs)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* SPR:6995 start */  
          if (p_arfcns->explicit_list_of_arfcns.count > ARRSIZE(p_arfcns->
                                                                   explicit_list_of_arfcns.data))
        /* SPR:6995 stop */  
        {
            RRC_TRACE(RRC_WARNING,
                    "Incorrect explicit_list_arfcns_t::count=%u. Should be 1..%u",
                    p_arfcns->explicit_list_of_arfcns.count,
                    ARRSIZE(p_arfcns->explicit_list_of_arfcns.data)
                    );
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        
        p_asn1_arfcns->u.explicitListOfARFCNs->n = p_arfcns->
            explicit_list_of_arfcns.count;
        for (index = 0; index < p_arfcns->explicit_list_of_arfcns.count;
                index++)
        {
            p_asn1_arfcns->u.explicitListOfARFCNs->elem[index] =
                p_arfcns->explicit_list_of_arfcns.data[index];
        }
    }
    else if (GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG == p_arfcns->presence_bitmask)
    {
        p_asn1_arfcns->t =
            T_CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs;
        p_asn1_arfcns->u.equallySpacedARFCNs = rtxMemAllocType(p_asn1_ctx,
                CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs);
        if (PNULL == p_asn1_arfcns->u.equallySpacedARFCNs)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        p_asn1_arfcns->u.equallySpacedARFCNs->arfcn_Spacing =
            p_arfcns->equally_spaced_arfcns.arfcn_spacing;
        p_asn1_arfcns->u.equallySpacedARFCNs->numberOfFollowingARFCNs =
            p_arfcns->equally_spaced_arfcns.num_of_following_arfcns;
    }
    else if (GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG == p_arfcns->
            presence_bitmask)
    {
        p_asn1_arfcns->t =
            T_CarrierFreqsGERAN_followingARFCNs_variableBitMapOfARFCNs;
        p_asn1_arfcns->u.variableBitMapOfARFCNs =
            rtxMemAllocType(p_asn1_ctx,
                    CarrierFreqsGERAN_followingARFCNs_variableBitMapOfARFCNs);
        if (PNULL == p_asn1_arfcns->u.variableBitMapOfARFCNs)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        if ((p_arfcns->var_bitmap_of_arfcns.count < 1) ||
                (p_arfcns->var_bitmap_of_arfcns.count > ARRSIZE(p_arfcns->
                                                                var_bitmap_of_arfcns.data)))
        {
            RRC_TRACE(RRC_WARNING,
                    "Incorrect var_bitmap_of_arfcns_t::count=%u. Should be 1..%u",
                    p_arfcns->var_bitmap_of_arfcns.count,
                    ARRSIZE(p_arfcns->var_bitmap_of_arfcns.data)
                    );
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        
        p_asn1_arfcns->u.variableBitMapOfARFCNs->numocts = p_arfcns->
            var_bitmap_of_arfcns.count;
        for (index = 0; index < p_arfcns->var_bitmap_of_arfcns.count; index++)
        {
            p_asn1_arfcns->u.variableBitMapOfARFCNs->data[index] =
                p_arfcns->var_bitmap_of_arfcns.data[index];
        }
    }
    else
    {
        RRC_TRACE(RRC_WARNING,
                "Incorrect presence_bitmask = %u for geran_following_arfcns_t",
                p_arfcns->presence_bitmask);
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME: check_and_convert_carrier_freqs_geran
 *   INPUT        : OSCTXT                  *p_asn1_ctx
 *                  CarrierFreqsGERAN       *p_asn1_cf_geran
 *                  carrier_freqs_geran_t   *p_cf_geran
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS - if all data are valid
 *                  RRC_FAILURE - if data is invalid or if there are not enough memory
 *   DESCRIPTION  : This function convert carrier_freqs_geran_t to
 *                  CarrierFreqsGERAN and check if input data are valid.
 ******************************************************************************/
rrc_return_et check_and_convert_carrier_freqs_geran
(
 OSCTXT                  *p_asn1_ctx,       /* for memory allocation */
 CarrierFreqsGERAN       *p_asn1_cf_geran,  /* destination */
 carrier_freqs_geran_t   *p_cf_geran        /* source */
 )
{
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_cf_geran);
    RRC_ASSERT(PNULL != p_cf_geran);

    p_asn1_cf_geran->startingARFCN = p_cf_geran->starting_arfcn;
    p_asn1_cf_geran->bandIndicator = p_cf_geran->band_indicator;

    /* followingARFCNs */
    if (RRC_SUCCESS !=
            check_and_convert_carrier_freqs_geran_following_arfcns(
                p_asn1_ctx,
                &p_asn1_cf_geran->followingARFCNs,
                &p_cf_geran->following_arfcns
                ))
    {
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_rrc_fill_cell_info_geran_r9
 *   INPUT        : OSCTXT                  *p_asn1_ctx
 *                  CellInfoListGERAN_r9        *p_asn_geran_r9
 *                  cell_info_list_geran_r9_t   *p_geran_r9
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS - all data are valid
 *                  RRC_FAILURE - data are invalid or if there are not enough memory
 *   DESCRIPTION  : This function converts cell_info_list_geran_r9_t to
 *                  CellInfoListGERAN_r9 and checks whether input data are valid.
 ******************************************************************************/
rrc_return_et rrm_rrc_fill_cell_info_geran_r9
(
 /* for memory allocation */
 OSCTXT                                    *p_asn1_ctx,
 CellInfoListGERAN_r9        *p_asn_geran_r9, /* destination */
 cell_info_list_geran_r9_t   *p_geran_r9       /* source */
 )
{
    rrc_counter_t                   geran_index        = RRC_NULL;
    rrc_counter_t                   si_index        = RRC_NULL;
    CellInfoGERAN_r9                *p_asn1_geran_r9_elem = PNULL;
    OSRTDListNode                   *p_asn1_list_node  = PNULL;

    RRC_UT_TRACE_ENTER();

    if ((p_geran_r9->count < 1) || (p_geran_r9->count > ARRSIZE(p_geran_r9->
                    cell_info_geran_r9)))
    {
        RRC_TRACE(RRC_WARNING,
                "Incorrect neigh_cells_per_bandclass_list_cdma2000_t count=%u. "
                "Should be 1..%u",
                p_geran_r9->count,
                ARRSIZE(p_geran_r9->cell_info_geran_r9)
                );
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill CellInfoListGERAN_r9 */
    rtxDListInit(p_asn_geran_r9);
    for (geran_index = 0; geran_index < p_geran_r9->count; geran_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, CellInfoGERAN_r9,
                &p_asn1_list_node, &p_asn1_geran_r9_elem);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_CellInfoGERAN_r9(p_asn1_geran_r9_elem);

        /* Fill PhysCellIdGERAN */
        asn1Init_PhysCellIdGERAN(&p_asn1_geran_r9_elem->physCellId_r9);

        p_asn1_geran_r9_elem->physCellId_r9.networkColourCode.numbits = 3;

/* SPR 19765 Fix Start */ 
        p_asn1_geran_r9_elem->physCellId_r9.networkColourCode.data[0] =
            p_geran_r9->cell_info_geran_r9[geran_index].phys_cell_id_r9.
            network_colour_code << 5;

        p_asn1_geran_r9_elem->physCellId_r9.baseStationColourCode.numbits= 3;

        p_asn1_geran_r9_elem->physCellId_r9.baseStationColourCode.data[0]=
            p_geran_r9->cell_info_geran_r9[geran_index].phys_cell_id_r9.
            base_station_colour_code << 5;
/* SPR 19765 Fix Stop */ 

        /* Fill CarrierFreqGERAN */
        memset_wrapper(&p_asn1_geran_r9_elem->carrierFreq_r9, RRC_NULL, sizeof(CarrierFreqGERAN));

        p_asn1_geran_r9_elem->carrierFreq_r9.arfcn =
            p_geran_r9->cell_info_geran_r9[geran_index].carrier_freq_r9.arfcn;

        p_asn1_geran_r9_elem->carrierFreq_r9.bandIndicator =
            p_geran_r9->cell_info_geran_r9[geran_index].
            carrier_freq_r9.band_indicator;

        /*Fill SystemInfoListGERAN*/
        asn1Init_SystemInfoListGERAN(&p_asn1_geran_r9_elem->systemInformation_r9);

        p_asn1_geran_r9_elem->systemInformation_r9.n =
            p_geran_r9->cell_info_geran_r9[geran_index].
            system_information_r9.count;
        for (si_index = 0; si_index < p_geran_r9->
                cell_info_geran_r9[geran_index].system_information_r9.count; 
                si_index++)
        {
            if (RRC_RRM_SYS_INFO_CONTAINER_ENCODED_PRESENT &
                    p_geran_r9->cell_info_geran_r9[geran_index].
                    system_information_r9.system_info_geran_elem[si_index].bitmask)
            {
                p_asn1_geran_r9_elem->systemInformation_r9.elem[si_index].numocts =
                    (p_geran_r9->cell_info_geran_r9[geran_index].
                     system_information_r9.system_info_geran_elem[si_index].
                     sys_info_container_encoded_si_info.length);

                l3_memcpy_wrapper(p_asn1_geran_r9_elem->systemInformation_r9.elem[si_index].data,
                        p_geran_r9->cell_info_geran_r9[geran_index].
                        system_information_r9.system_info_geran_elem[si_index].
                        sys_info_container_encoded_si_info.data,
                        p_asn1_geran_r9_elem->systemInformation_r9.elem[si_index].numocts);
            }
            else if (RRC_RRM_SYS_INFO_CONTAINTER_EXPANDED_PRESENT &
                    p_geran_r9->cell_info_geran_r9[geran_index].
                    system_information_r9.system_info_geran_elem[si_index].bitmask)
            {
                RRC_TRACE (RRC_WARNING, 
                        "GERAN si container is in expanded si format "
                        " presently we are not handling this ");
                RRC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            else
            {
                RRC_TRACE (RRC_WARNING, 
                        "Improper Bitmask for GERAN-si conatiner");
                RRC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }
        rtxDListAppendNode(p_asn_geran_r9, p_asn1_list_node);
    }
    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_rrc_fill_cell_info_utra_fdd_r9
 *   INPUT        : OSCTXT                                    *p_asn1_ctx
 *                  CellInfoListUTRA_FDD_r9        *p_asn_utra_fdd_r9
 *                  cell_info_list_utra_fdd_r9_t   *p_utra_fdd_r9
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS - all data are valid
 *                  RRC_FAILURE - data are invalid or if there are not enough memory
 *   DESCRIPTION  : This function converts cell_info_list_utra_fdd_r9_t to
 *                  CellInfoListUTRA_FDD_r9 and checks whether input data are valid.
 ******************************************************************************/
rrc_return_et rrm_rrc_fill_cell_info_utra_fdd_r9
(
 /* for memory allocation */
 OSCTXT                                    *p_asn1_ctx,
 CellInfoListUTRA_FDD_r9        *p_asn_utra_fdd_r9, /* destination */
 cell_info_list_utra_fdd_r9_t   *p_utra_fdd_r9       /* source */
 )
{
    rrc_counter_t                   fdd_index        = RRC_NULL;
    CellInfoUTRA_FDD_r9             *p_asn1_utra_fdd_r9_elem = PNULL;
    OSRTDListNode                   *p_asn1_list_node  = PNULL;

    RRC_UT_TRACE_ENTER();

    if ((p_utra_fdd_r9->count < 1) || (p_utra_fdd_r9->count >
                ARRSIZE(p_utra_fdd_r9->cell_info_utra_fdd_r9)))
    {
        RRC_TRACE(RRC_WARNING,
                "Incorrect neigh_cells_per_bandclass_list_cdma2000_t count=%u. "
                "Should be 1..%u",
                p_utra_fdd_r9->count,
                ARRSIZE(p_utra_fdd_r9->cell_info_utra_fdd_r9)
                );
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill CellInfoListUTRA_FDD_r9 */
    rtxDListInit(p_asn_utra_fdd_r9);
    for (fdd_index = 0; fdd_index < p_utra_fdd_r9->count; fdd_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, CellInfoUTRA_FDD_r9,
                &p_asn1_list_node, &p_asn1_utra_fdd_r9_elem);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_CellInfoUTRA_FDD_r9(p_asn1_utra_fdd_r9_elem);
        /* Fill PhysCellIdUTRA_FDD */
        p_asn1_utra_fdd_r9_elem->physCellId_r9 = 
            p_utra_fdd_r9->cell_info_utra_fdd_r9[fdd_index].phys_cell_id_r9;

        /*Fill utra_BCCH_Container_r9*/
        if (RRC_RRM_UTRA_FDD_BCCH_CONTAINER_ENCODED_SI_PRESENT &
                p_utra_fdd_r9->cell_info_utra_fdd_r9[fdd_index].bitmask)
        {
            p_asn1_utra_fdd_r9_elem->utra_BCCH_Container_r9.numocts = 
                p_utra_fdd_r9->cell_info_utra_fdd_r9[fdd_index].
                utra_fdd_bcch_cont_r9_encoded_si_info.utra_bcch_container_length;

            p_asn1_utra_fdd_r9_elem->utra_BCCH_Container_r9.data = 
                (OSOCTET *)rtxMemAlloc(p_asn1_ctx,
                        p_asn1_utra_fdd_r9_elem->utra_BCCH_Container_r9.numocts);
            /* SPR 12964 Fix Start */
            if(PNULL == p_asn1_utra_fdd_r9_elem->utra_BCCH_Container_r9.data)
            {
                RRC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                RRC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            /* SPR 12964 Fix Stop */
            /*SPR 20553 FIX START */

            l3_memcpy_wrapper((void *)p_asn1_utra_fdd_r9_elem->utra_BCCH_Container_r9.data,
                    p_utra_fdd_r9->cell_info_utra_fdd_r9[fdd_index].
                    utra_fdd_bcch_cont_r9_encoded_si_info.utra_bcch_container,
                    p_asn1_utra_fdd_r9_elem->utra_BCCH_Container_r9.numocts);
            /*SPR 20553 FIX END */

        }
        else if (RRC_RRM_UTRA_FDD_BCCH_CONTAINTER_EXPANDED_SI_INFO_PRESENT &
                p_utra_fdd_r9->cell_info_utra_fdd_r9[fdd_index].bitmask)
        {
            RRC_TRACE (RRC_WARNING, 
                    "UTRA FDD container is in expanded si format "
                    " presently we are not handling this ");
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        else
        {
            RRC_TRACE (RRC_WARNING, 
                    "Improper Bitmask for utra-fdd-bcch conatiner");
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        rtxDListAppendNode(p_asn_utra_fdd_r9, p_asn1_list_node);
    }
    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: rrm_rrc_fill_cell_info_utra_tdd_r9
 *   INPUT        : OSCTXT                                    *p_asn1_ctx
 *                  CellInfoListUTRA_TDD_r9        *p_asn_utra_tdd_r9
 *                  cell_info_list_utra_tdd_r9_t   *p_utra_tdd_r9
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS - all data are valid
 *                  RRC_FAILURE - data are invalid or if there are not enough memory
 *   DESCRIPTION  : This function converts cell_info_list_utra_tdd_r9_t to
 *                  CellInfoListUTRA_TDD_r9 and checks whether input data are valid.
 *
 ******************************************************************************/
rrc_return_et rrm_rrc_fill_cell_info_utra_tdd_r9
(
 /* for memory allocation */
 OSCTXT                                    *p_asn1_ctx,
 CellInfoListUTRA_TDD_r9        *p_asn_utra_tdd_r9, /* destination */
 cell_info_list_utra_tdd_r9_t   *p_utra_tdd_r9       /* source */
 )
{
    rrc_counter_t                   tdd_index        = RRC_NULL;
    CellInfoUTRA_TDD_r9             *p_asn1_utra_tdd_r9_elem = PNULL;
    OSRTDListNode                   *p_asn1_list_node  = PNULL;

    RRC_UT_TRACE_ENTER();

    if ((p_utra_tdd_r9->count < 1) || (p_utra_tdd_r9->count >
                ARRSIZE(p_utra_tdd_r9->cell_info_utra_tdd_r9)))
    {
        RRC_TRACE(RRC_WARNING,
                "Incorrect neigh_cells_per_bandclass_list_cdma2000_t count=%u. "
                "Should be 1..%u",
                p_utra_tdd_r9->count,
                ARRSIZE(p_utra_tdd_r9->cell_info_utra_tdd_r9)
                );
        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill CellInfoListUTRA_TDD_r9 */
    rtxDListInit(p_asn_utra_tdd_r9);
    for (tdd_index = 0; tdd_index < p_utra_tdd_r9->count; tdd_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, CellInfoUTRA_TDD_r9,
                &p_asn1_list_node, &p_asn1_utra_tdd_r9_elem);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_CellInfoUTRA_TDD_r9(p_asn1_utra_tdd_r9_elem);
        /* Fill PhysCellIdUTRA_TDD */
        p_asn1_utra_tdd_r9_elem->physCellId_r9 = 
            p_utra_tdd_r9->cell_info_utra_tdd_r9[tdd_index].phys_cell_id_r9;

        /*Fill utra_BCCH_Container_r9*/
        if (RRC_RRM_UTRA_TDD_BCCH_CONTAINER_ENCODED_SI_PRESENT &
                p_utra_tdd_r9->cell_info_utra_tdd_r9[tdd_index].bitmask)
        {
            p_asn1_utra_tdd_r9_elem->utra_BCCH_Container_r9.numocts = 
                p_utra_tdd_r9->cell_info_utra_tdd_r9[tdd_index].
                utra_tdd_bcch_cont_r9_encoded_si_info.utra_bcch_container_length;

            p_asn1_utra_tdd_r9_elem->utra_BCCH_Container_r9.data =
                (OSOCTET *)rtxMemAlloc(p_asn1_ctx,
                        p_asn1_utra_tdd_r9_elem->utra_BCCH_Container_r9.numocts);
            /* SPR 12964 Fix Start */
            if(PNULL == p_asn1_utra_tdd_r9_elem->utra_BCCH_Container_r9.data)
            {
                RRC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                RRC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            /* SPR 12964 Fix Stop */
            p_asn1_utra_tdd_r9_elem->utra_BCCH_Container_r9.data =
                    p_utra_tdd_r9->cell_info_utra_tdd_r9[tdd_index].
                    utra_tdd_bcch_cont_r9_encoded_si_info.utra_bcch_container;
        }
        else if (RRC_RRM_UTRA_TDD_BCCH_CONTAINTER_EXPANDED_SI_INFO_PRESENT &
                p_utra_tdd_r9->cell_info_utra_tdd_r9[tdd_index].bitmask)
        {
            RRC_TRACE (RRC_WARNING, 
                    "UTRA TDD container is in expanded si format "
                    " presently we are not handling this ");
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        else
        {
            RRC_TRACE (RRC_WARNING, 
                    "Improper Bitmask for utra-tdd-bcch conatiner");
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        rtxDListAppendNode(p_asn_utra_tdd_r9, p_asn1_list_node);
    }
    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/*CR 713 changes start*/
/******************************************************************************
 *   FUNCTION NAME: rrm_rrc_fill_cell_info_utra_tdd_r10 
 *
 *   DESCRIPTION:
 *       This function converts cell_info_list_utra_tdd_r10_t to
 *          CellInfoListUTRA_TDD_r10 
 *       and checks whether input data are valid.
 *
 *   RETURNS:
 *       RRC_SUCCESS - all data are valid
 *       RRC_FAILURE - data are invalid or if there are not enough memory
 *
 ******************************************************************************/
rrc_return_et rrm_rrc_fill_cell_info_utra_tdd_r10
(
    OSCTXT                            *p_asn1_ctx,
    CellInfoListUTRA_TDD_r10          *p_asn_utra_tdd_r10,
    cell_info_list_utra_tdd_r10_t     *p_utra_tdd_r10 
)
{
    rrc_counter_t                   tdd_index        = RRC_NULL;
    OSRTDListNode                   *p_asn1_list_node  = PNULL;
    CellInfoUTRA_TDD_r10            *p_asn1_utra_tdd_r10_elem = PNULL;

    RRC_UT_TRACE_ENTER();

    if ((p_utra_tdd_r10->count < 1) || (p_utra_tdd_r10->count >
                ARRSIZE(p_utra_tdd_r10->cell_info_utra_tdd_r10)))
    {
        RRC_TRACE(RRC_WARNING,
                "Incorrect cell_info_utra_tdd_r10 count=%u. "
                "Should be 1..%u",
                p_utra_tdd_r10->count,
                ARRSIZE(p_utra_tdd_r10->cell_info_utra_tdd_r10)
                );

        RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    
    /* Fill CellInfoListUTRA_TDD_r10*/
    rtxDListInit(p_asn_utra_tdd_r10);

    for (tdd_index = 0; tdd_index < p_utra_tdd_r10->count; tdd_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                                 CellInfoUTRA_TDD_r10,
                                 &p_asn1_list_node, 
                                 &p_asn1_utra_tdd_r10_elem);

        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        
        asn1Init_CellInfoUTRA_TDD_r10(p_asn1_utra_tdd_r10_elem);
        
        /* Fill PhysCellIdUTRA_TDD */
        p_asn1_utra_tdd_r10_elem->physCellId_r10 = p_utra_tdd_r10->
            cell_info_utra_tdd_r10[tdd_index].phys_cell_id_r10;

        /*Fill utra_BCCH_Container_r10*/
        p_asn1_utra_tdd_r10_elem->utra_BCCH_Container_r10.numocts =
            p_utra_tdd_r10->cell_info_utra_tdd_r10[tdd_index].utra_BCCH_Container_r10.
            utra_bcch_container_length;

        p_asn1_utra_tdd_r10_elem->utra_BCCH_Container_r10.data = 
            (OSOCTET *)rtxMemAlloc(p_asn1_ctx,
                    p_asn1_utra_tdd_r10_elem->utra_BCCH_Container_r10.numocts);
        /* SPR 12964 Fix Start */
        if(PNULL == p_asn1_utra_tdd_r10_elem->utra_BCCH_Container_r10.data)
        {
            RRC_TRACE(RRC_ERROR, "Unable to allocate memory.");
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* SPR 12964 Fix Stop */

        p_asn1_utra_tdd_r10_elem->utra_BCCH_Container_r10.data =
            p_utra_tdd_r10->cell_info_utra_tdd_r10[tdd_index].utra_BCCH_Container_r10.
            utra_bcch_container;

        /* fill carrierFreq_r10*/
        p_asn1_utra_tdd_r10_elem->carrierFreq_r10 = p_utra_tdd_r10->
            cell_info_utra_tdd_r10[tdd_index].carrier_freq_r10;

        rtxDListAppendNode(p_asn_utra_tdd_r10, p_asn1_list_node);
    }
    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
/*CR 713 changes stop*/
                                      

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_check_and_convert_cell_info_list_r9
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  OSCTXT                            *p_asn1_ctx
 *                  RRCConnectionRelease_v920_IEs_cellInfoList_r9 *p_asn1_cell_info_list
 *                  cell_info_list_r9_t          *p_cell_info_list
 *   OUTPUT       : None
 *   RETURNS      : RRC_SUCCESS - if all data are valid
 *                  RRC_FAILURE - if data is invalid or if there are not enough memory
 *   DESCRIPTION  : This function convert geran_following_arfcns_t to
 *                  CarrierFreqsGERAN_followingARFCNs and check if input data are valid.
 ******************************************************************************/
rrc_return_et uecc_llim_check_and_convert_cell_info_list_r9
(
 uecc_ue_context_t*  p_uecc_ue_context,        /* UE context */
 /* for memory allocation */
 OSCTXT                            *p_asn1_ctx,
 RRCConnectionRelease_v920_IEs_cellInfoList_r9 *p_asn1_cell_info_list,  /* destination */
 cell_info_list_r9_t          *p_cell_info_list        /* source */
 )
{
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_cell_info_list);
    RRC_ASSERT(PNULL != p_cell_info_list);

    asn1Init_RRCConnectionRelease_v920_IEs_cellInfoList_r9(p_asn1_cell_info_list);

    if ((RRM_REDIRECTED_CARRIER_INFO_GERAN_PRESENT == 
                p_uecc_ue_context->p_redirected_carrier_info->bitmask)&&
            RRC_RRM_CELL_INFO_LIST_GERAN_R9_PRESENT & p_cell_info_list->bitmask)
    {
        p_asn1_cell_info_list->t =
            T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_geran_r9;

        p_asn1_cell_info_list->u.geran_r9 = rtxMemAllocType(
                p_asn1_ctx,CellInfoListGERAN_r9);
        /* SPR 12964 Fix Start */
        if (PNULL == p_asn1_cell_info_list->u.geran_r9)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* SPR 12964 Fix Stop */

        /* Fill CellInfoGERAN_r9 */
        if (RRC_SUCCESS !=
                rrm_rrc_fill_cell_info_geran_r9(
                    p_asn1_ctx,
                    p_asn1_cell_info_list->u.geran_r9,
                    &p_cell_info_list->geran_r9
                    ))
        {
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }
    else if ((RRM_REDIRECTED_CARRIER_INFO_UTRA_FDD_PRESENT == 
                p_uecc_ue_context->p_redirected_carrier_info->bitmask) && 
            (RRC_TRUE == p_uecc_ue_context->m.e_redirection_utra_r9_present) && 
            (RRC_RRM_CELL_INFO_LIST_UTRA_FDD_R9_PRESENT & p_cell_info_list->bitmask))
    {
        RRC_TRACE(RRC_INFO, "e_redirection utra_r9 Present....");

        p_asn1_cell_info_list->t =
            T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_utra_FDD_r9;

        p_asn1_cell_info_list->u.utra_FDD_r9 = rtxMemAllocType(
                p_asn1_ctx,CellInfoListUTRA_FDD_r9);
        /* SPR 12964 Fix Start */
        if (PNULL == p_asn1_cell_info_list->u.utra_FDD_r9)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* SPR 12964 Fix Stop */

        /* Fill CellInfoListUTRA_FDD_r9 */
        if (RRC_SUCCESS !=
                rrm_rrc_fill_cell_info_utra_fdd_r9(
                    p_asn1_ctx,
                    p_asn1_cell_info_list->u.utra_FDD_r9,
                    &p_cell_info_list->utra_fdd_r9
                    ))
        {
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }
    else if ((RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_PRESENT == 
                p_uecc_ue_context->p_redirected_carrier_info->bitmask) && 
            (RRC_TRUE == p_uecc_ue_context->m.e_redirection_utra_r9_present) && 
            (RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R9_PRESENT & p_cell_info_list->bitmask))
    {
        RRC_TRACE(RRC_INFO, "e_redirection utra_r9 Present....");

        p_asn1_cell_info_list->t =
            T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_utra_TDD_r9;

        p_asn1_cell_info_list->u.utra_TDD_r9 = rtxMemAllocType(
                p_asn1_ctx,CellInfoListUTRA_TDD_r9);
        /* SPR 12964 Fix Start */
        if (PNULL == p_asn1_cell_info_list->u.utra_TDD_r9)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* SPR 12964 Fix Stop */

        /* Fill CellInfoListUTRA_TDD_r9 */
        if (RRC_SUCCESS !=
                rrm_rrc_fill_cell_info_utra_tdd_r9(
                    p_asn1_ctx,
                    p_asn1_cell_info_list->u.utra_TDD_r9,
                    &p_cell_info_list->utra_tdd_r9
                    ))
        {
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }/* CR 713 changes start*/
    else if ((RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_R10_PRESENT == 
                p_uecc_ue_context->p_redirected_carrier_info->bitmask) &&
                (p_cell_info_list->bitmask & RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R10_PRESENT))
    {
        RRC_TRACE(RRC_INFO, "e_redirection utra_tdd_r10 Present....");

        p_asn1_cell_info_list->t = 
            T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_utra_TDD_r10;

        p_asn1_cell_info_list->u.utra_TDD_r10 = rtxMemAllocType(
                p_asn1_ctx,CellInfoListUTRA_TDD_r10);
        /* SPR 12964 Fix Start */
        if (PNULL == p_asn1_cell_info_list->u.utra_TDD_r10)
        {
            /* Not enough memory */
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* SPR 12964 Fix Stop */

        /* Fill CellInfoListUTRA_TDD_r10*/
        if (RRC_SUCCESS !=
                rrm_rrc_fill_cell_info_utra_tdd_r10(
                    p_asn1_ctx,
                    p_asn1_cell_info_list->u.utra_TDD_r10,
                    &p_cell_info_list->utra_tdd_r10))
        {
            RRC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }/* CR 713 changes stop*/
    else
    {
        RRC_TRACE(RRC_WARNING,
                "Either - Wrong cell_info_list Bitmask:%d or it is "
                "Mismtach with redirected_carrier_info Bitmask or "
                "ue capability e_redirection utra_r9 or utra_tdd_r10 is not present",
                p_cell_info_list->bitmask);
    }
    RRC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
