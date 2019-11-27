/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rrm_parser.c $
 *
 *******************************************************************************
 *
 * File Description: All SON RRM parser function defination to Parse SON RRM
 *                   APIs.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "rrm_pack_unpack_utils.h"
#include "rrm_son_parser.h"
#include "rrm_utils.h"
#include "son_oam_intf.h"
#include "son_rrm_intf.h"

#include <stdio.h>
#include <string.h>

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrm_return_et
rrm_parse_son_rrm_cell_plmn_info
(
    son_rrm_cell_plmn_info_t *p_son_rrm_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_global_cell_id
(
    son_rrm_intra_rat_global_cell_id_t *p_son_rrm_intra_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_subframe_allocation
(
    son_subframe_allocation_t *p_son_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_mbsfn_info
(
    son_mbsfn_info_t *p_son_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_mbms_configuration
(
    son_mbms_configuration_t *p_son_mbms_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_neighbor_cell
(
    son_rrm_intra_rat_neighbor_cell_t *p_son_rrm_intra_rat_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_neighbor_list
(
    son_rrm_intra_rat_neighbor_list_t *p_son_rrm_intra_rat_neighbor_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_neighbor_cell_list
(
    son_rrm_intra_rat_neighbor_cell_list_t *p_son_rrm_intra_rat_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_utran_geran_global_cell_id
(
    son_rrm_utran_geran_global_cell_id_t *p_son_rrm_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_utran_physical_cell_id
(
    son_rrm_utran_physical_cell_id_t *p_son_rrm_utran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_utran_neighbor_cell
(
    son_rrm_utran_neighbor_cell_t *p_son_rrm_utran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_utran_neighbor_list
(
    son_rrm_utran_neighbor_list_t *p_son_rrm_utran_neighbor_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_utran_neighbor_cell_list
(
    son_rrm_utran_neighbor_cell_list_t *p_son_rrm_utran_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_geran_physical_cell_id
(
    son_rrm_geran_physical_cell_id_t *p_son_rrm_geran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_geran_neighbor_cell
(
    son_rrm_geran_neighbor_cell_t *p_son_rrm_geran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_geran_neighbor_list
(
    son_rrm_geran_neighbor_list_t *p_son_rrm_geran_neighbor_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_geran_neighbor_cell_list
(
    son_rrm_geran_neighbor_cell_list_t *p_son_rrm_geran_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_global_cell_id
(
    son_rrm_cdma_global_cell_id_t *p_son_rrm_cdma_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_cdma_pz_hyst_params_included
(
    son_cdma_pz_hyst_params_included_t *p_son_cdma_pz_hyst_params_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_cdma_fpc_fch_included
(
    son_cdma_fpc_fch_included_t *p_son_cdma_fpc_fch_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_cell_specific_params
(
    son_rrm_cdma_cell_specific_params_t *p_son_rrm_cdma_cell_specific_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_xrtt
(
    son_rrm_cdma_neighbor_cell_xrtt_t *p_son_rrm_cdma_neighbor_cell_xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_list_xrtt
(
    son_rrm_cdma_neighbor_list_xrtt_t *p_son_rrm_cdma_neighbor_list_xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_list_xrtt
(
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_son_rrm_cdma_neighbor_cell_list_xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_hrpd
(
    son_rrm_cdma_neighbor_cell_hrpd_t *p_son_rrm_cdma_neighbor_cell_hrpd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_list_hrpd
(
    son_rrm_cdma_neighbor_list_hrpd_t *p_son_rrm_cdma_neighbor_list_hrpd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_list_hrpd
(
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_son_rrm_cdma_neighbor_cell_list_hrpd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_inter_rat_neighbor_cell_list
(
    son_rrm_inter_rat_neighbor_cell_list_t *p_son_rrm_inter_rat_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_neighbor_cell_signal_power
(
    son_rrm_neighbor_cell_signal_power_t *p_son_rrm_neighbor_cell_signal_power,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_carrier_freq_and_dl_power_params_resp
(
    son_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_rrm_carrier_freq_and_dl_power_params_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrm_return_et
rrm_parse_son_rrm_global_enb_id
(
    son_rrm_global_enb_id_t *p_son_rrm_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrm_return_et
rrm_parse_son_anr_rrm_updated_nrt_info
(
    son_anr_rrm_updated_nrt_info_t *p_son_anr_rrm_updated_nrt_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_anr_rrm_updated_nrt_info, 0, sizeof(son_anr_rrm_updated_nrt_info_t));

    /* This function parses son_anr_rrm_updated_nrt_info */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_anr_rrm_updated_nrt_info");



    if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_global_cell_id(
        &p_son_anr_rrm_updated_nrt_info->src_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;


    if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_neighbor_cell_list(
        &p_son_anr_rrm_updated_nrt_info->intra_rat_ncl,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;


    if (RRM_FAILURE == rrm_parse_son_rrm_inter_rat_neighbor_cell_list(
        &p_son_anr_rrm_updated_nrt_info->inter_rat_ncl,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cell_plmn_info
(
    son_rrm_cell_plmn_info_t *p_son_rrm_cell_plmn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cell_plmn_info, 0, sizeof(son_rrm_cell_plmn_info_t));

    /* This function parses son_rrm_cell_plmn_info */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cell_plmn_info");


    if (*p_length_read + (S32)sizeof(p_son_rrm_cell_plmn_info->mcc) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_cell_plmn_info->mcc); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_cell_plmn_info->mcc[loop], (void*)(p_src + *p_length_read), "mcc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cell_plmn_info->num_mnc_digit, p_src + *p_length_read, "num_mnc_digit");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_cell_plmn_info->num_mnc_digit < 2) || (p_son_rrm_cell_plmn_info->num_mnc_digit > SON_MAX_MNC_OCTET_SIZE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cell_plmn_info->num_mnc_digit] should be in range [2 <= %d <= SON_MAX_MNC_OCTET_SIZE] !", p_son_rrm_cell_plmn_info->num_mnc_digit);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)(p_son_rrm_cell_plmn_info->num_mnc_digit * sizeof(p_son_rrm_cell_plmn_info->mnc[0])) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cell_plmn_info->num_mnc_digit; loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_cell_plmn_info->mnc[loop], (void*)(p_src + *p_length_read), "mnc[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_global_cell_id
(
    son_rrm_intra_rat_global_cell_id_t *p_son_rrm_intra_rat_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_intra_rat_global_cell_id, 0, sizeof(son_rrm_intra_rat_global_cell_id_t));

    /* This function parses son_rrm_intra_rat_global_cell_id */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_intra_rat_global_cell_id");



    if (RRM_FAILURE == rrm_parse_son_rrm_cell_plmn_info(
        &p_son_rrm_intra_rat_global_cell_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_rrm_intra_rat_global_cell_id->cell_identity) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_intra_rat_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_intra_rat_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_subframe_allocation
(
    son_subframe_allocation_t *p_son_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_subframe_allocation, 0, sizeof(son_subframe_allocation_t));

    /* This function parses son_subframe_allocation */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_subframe_allocation");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_subframe_allocation->oneframe) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_subframe_allocation->oneframe); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_subframe_allocation->oneframe[loop], (void*)(p_src + *p_length_read), "oneframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_subframe_allocation->fourframe) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_subframe_allocation->fourframe); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_subframe_allocation->fourframe[loop], (void*)(p_src + *p_length_read), "fourframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_mbsfn_info
(
    son_mbsfn_info_t *p_son_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_mbsfn_info, 0, sizeof(son_mbsfn_info_t));

    /* This function parses son_mbsfn_info */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_mbsfn_info");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_mbsfn_info->radioframe_alloc_period, p_src + *p_length_read, "radioframe_alloc_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_mbsfn_info->radioframe_alloc_period] should be in range [%d <= 5] !", p_son_mbsfn_info->radioframe_alloc_period);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_mbsfn_info->radioframe_alloc_offset, p_src + *p_length_read, "radioframe_alloc_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_mbsfn_info->radioframe_alloc_offset] should be in range [%d <= 7] !", p_son_mbsfn_info->radioframe_alloc_offset);
        return RRM_FAILURE;
    }


    if (RRM_FAILURE == rrm_parse_son_subframe_allocation(
        &p_son_mbsfn_info->subframe_alloc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_mbms_configuration
(
    son_mbms_configuration_t *p_son_mbms_configuration,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_mbms_configuration, 0, sizeof(son_mbms_configuration_t));

    /* This function parses son_mbms_configuration */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_mbms_configuration");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_mbms_configuration->num_mbsfn, p_src + *p_length_read, "num_mbsfn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mbms_configuration->num_mbsfn < 1) || (p_son_mbms_configuration->num_mbsfn > 8))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_mbms_configuration->num_mbsfn] should be in range [1 <= %d <= 8] !", p_son_mbms_configuration->num_mbsfn);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_mbms_configuration->num_mbsfn; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_mbsfn_info(
                &p_son_mbms_configuration->mbsfn_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_neighbor_cell
(
    son_rrm_intra_rat_neighbor_cell_t *p_son_rrm_intra_rat_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_intra_rat_neighbor_cell, 0, sizeof(son_rrm_intra_rat_neighbor_cell_t));

    /* This function parses son_rrm_intra_rat_neighbor_cell */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_intra_rat_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);


    if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_global_cell_id(
        &p_son_rrm_intra_rat_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_intra_rat_neighbor_cell->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->pci > 503))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->pci] should be in range [%d <= 503] !", p_son_rrm_intra_rat_neighbor_cell->pci);
        return RRM_FAILURE;
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_RAT_NR_INFO_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset > 30))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset] should be in range [%d <= 30] !", p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_ENABLE_X2_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->x2_status, p_src + *p_length_read, "x2_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_intra_rat_neighbor_cell->x2_status < 3) || (p_son_rrm_intra_rat_neighbor_cell->x2_status > 7))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->x2_status] should be in range [3 <= %d <= 7] !", p_son_rrm_intra_rat_neighbor_cell->x2_status);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_TAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_rrm_intra_rat_neighbor_cell->tac) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_intra_rat_neighbor_cell->tac); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_intra_rat_neighbor_cell->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset, p_src + *p_length_read, "cm_cell_specific_offset");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset > 30))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset] should be in range [%d <= 30] !", p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_ACCESS_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->access_mode, p_src + *p_length_read, "access_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->access_mode > 3))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->access_mode] should be in range [%d <= 3] !", p_son_rrm_intra_rat_neighbor_cell->access_mode);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_BROADCAST_STATUS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->broadcast_status, p_src + *p_length_read, "broadcast_status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->broadcast_status > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->broadcast_status] should be in range [%d <= 1] !", p_son_rrm_intra_rat_neighbor_cell->broadcast_status);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_CSG_IDENTITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_rrm_intra_rat_neighbor_cell->csg_identity) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_intra_rat_neighbor_cell->csg_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_intra_rat_neighbor_cell->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_CELL_INTF_SCHEME_TYPE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_cell->interference_scheme, p_src + *p_length_read, "interference_scheme");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->interference_scheme > 2))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->interference_scheme] should be in range [%d <= 2] !", p_son_rrm_intra_rat_neighbor_cell->interference_scheme);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_MBSFN_CONFIG_LIST_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_mbms_configuration(
        &p_son_rrm_intra_rat_neighbor_cell->mbms_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_TDD_SF_ASSN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_intra_rat_neighbor_cell->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->subframe_assn > 6))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->subframe_assn] should be in range [%d <= 6] !", p_son_rrm_intra_rat_neighbor_cell->subframe_assn);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_neighbor_list
(
    son_rrm_intra_rat_neighbor_list_t *p_son_rrm_intra_rat_neighbor_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_intra_rat_neighbor_list, 0, sizeof(son_rrm_intra_rat_neighbor_list_t));

    /* This function parses son_rrm_intra_rat_neighbor_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_intra_rat_neighbor_list");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_intra_rat_neighbor_list->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_intra_rat_neighbor_list->cell_list_size, p_src + *p_length_read, "cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_list->cell_list_size > SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN] !", p_son_rrm_intra_rat_neighbor_list->cell_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_list->cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_neighbor_cell(
                &p_son_rrm_intra_rat_neighbor_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_intra_rat_neighbor_cell_list
(
    son_rrm_intra_rat_neighbor_cell_list_t *p_son_rrm_intra_rat_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_intra_rat_neighbor_cell_list, 0, sizeof(son_rrm_intra_rat_neighbor_cell_list_t));

    /* This function parses son_rrm_intra_rat_neighbor_cell_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_intra_rat_neighbor_cell_list");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size, p_src + *p_length_read, "intra_rat_cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_neighbor_list(
                &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size, p_src + *p_length_read, "intra_rat_cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_neighbor_list(
                &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size, p_src + *p_length_read, "intra_rat_black_list_cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_neighbor_list(
                &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size, p_src + *p_length_read, "intra_rat_black_list_cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_neighbor_list(
                &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_utran_geran_global_cell_id
(
    son_rrm_utran_geran_global_cell_id_t *p_son_rrm_utran_geran_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_utran_geran_global_cell_id, 0, sizeof(son_rrm_utran_geran_global_cell_id_t));

    /* This function parses son_rrm_utran_geran_global_cell_id */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_utran_geran_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_utran_geran_global_cell_id->rat_type, p_src + *p_length_read, "rat_type");
    *p_length_read += sizeof(U32);


    if (RRM_FAILURE == rrm_parse_son_rrm_cell_plmn_info(
        &p_son_rrm_utran_geran_global_cell_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_rrm_utran_geran_global_cell_id->cell_identity) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_utran_geran_global_cell_id->cell_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_utran_geran_global_cell_id->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_utran_physical_cell_id
(
    son_rrm_utran_physical_cell_id_t *p_son_rrm_utran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_utran_physical_cell_id, 0, sizeof(son_rrm_utran_physical_cell_id_t));

    /* This function parses son_rrm_utran_physical_cell_id */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_utran_physical_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_utran_physical_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_rrm_utran_physical_cell_id->bitmask & SON_PCI_FDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_physical_cell_id->pci_fdd, p_src + *p_length_read, "pci_fdd");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_physical_cell_id->pci_fdd > 511))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_physical_cell_id->pci_fdd] should be in range [%d <= 511] !", p_son_rrm_utran_physical_cell_id->pci_fdd);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_utran_physical_cell_id->bitmask & SON_PCI_TDD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_utran_physical_cell_id->pci_tdd, p_src + *p_length_read, "pci_tdd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_physical_cell_id->pci_tdd > 127))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_physical_cell_id->pci_tdd] should be in range [%d <= 127] !", p_son_rrm_utran_physical_cell_id->pci_tdd);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_utran_neighbor_cell
(
    son_rrm_utran_neighbor_cell_t *p_son_rrm_utran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_utran_neighbor_cell, 0, sizeof(son_rrm_utran_neighbor_cell_t));

    /* This function parses son_rrm_utran_neighbor_cell */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_utran_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_utran_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);


    if (RRM_FAILURE == rrm_parse_son_rrm_utran_geran_global_cell_id(
        &p_son_rrm_utran_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;


    if (RRM_FAILURE == rrm_parse_son_rrm_utran_physical_cell_id(
        &p_son_rrm_utran_neighbor_cell->pci,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_NR_INFO_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_son_rrm_utran_neighbor_cell->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_utran_neighbor_cell->cell_specific_offset < -15) || (p_son_rrm_utran_neighbor_cell->cell_specific_offset > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_utran_neighbor_cell->cell_specific_offset);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_ACCESS_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_utran_neighbor_cell->access_mode, p_src + *p_length_read, "access_mode");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell->access_mode > 3))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->access_mode] should be in range [%d <= 3] !", p_son_rrm_utran_neighbor_cell->access_mode);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_LAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_rrm_utran_neighbor_cell->lac) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_utran_neighbor_cell->lac); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_utran_neighbor_cell->lac[loop], (void*)(p_src + *p_length_read), "lac[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_RAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_utran_neighbor_cell->rac, p_src + *p_length_read, "rac");
    *p_length_read += sizeof(U8);
    }

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_CSG_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_rrm_utran_neighbor_cell->csg_identity) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_utran_neighbor_cell->csg_identity); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_utran_neighbor_cell->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_utran_neighbor_cell->ps_ho_supported, p_src + *p_length_read, "ps_ho_supported");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell->ps_ho_supported > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->ps_ho_supported] should be in range [%d <= 1] !", p_son_rrm_utran_neighbor_cell->ps_ho_supported);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_utran_neighbor_cell->voip_capable, p_src + *p_length_read, "voip_capable");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell->voip_capable > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->voip_capable] should be in range [%d <= 1] !", p_son_rrm_utran_neighbor_cell->voip_capable);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_utran_neighbor_list
(
    son_rrm_utran_neighbor_list_t *p_son_rrm_utran_neighbor_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_utran_neighbor_list, 0, sizeof(son_rrm_utran_neighbor_list_t));

    /* This function parses son_rrm_utran_neighbor_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_utran_neighbor_list");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_neighbor_list->uarfcn, p_src + *p_length_read, "uarfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_list->uarfcn > 16383))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_list->uarfcn] should be in range [%d <= 16383] !", p_son_rrm_utran_neighbor_list->uarfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_neighbor_list->cell_list_size, p_src + *p_length_read, "cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_list->cell_list_size > SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN] !", p_son_rrm_utran_neighbor_list->cell_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_list->cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_utran_neighbor_cell(
                &p_son_rrm_utran_neighbor_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_utran_neighbor_cell_list
(
    son_rrm_utran_neighbor_cell_list_t *p_son_rrm_utran_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_utran_neighbor_cell_list, 0, sizeof(son_rrm_utran_neighbor_cell_list_t));

    /* This function parses son_rrm_utran_neighbor_cell_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_utran_neighbor_cell_list");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size, p_src + *p_length_read, "cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_utran_neighbor_list(
                &p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size, p_src + *p_length_read, "cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_utran_neighbor_list(
                &p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size, p_src + *p_length_read, "black_list_cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_utran_neighbor_list(
                &p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size, p_src + *p_length_read, "black_list_cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_utran_neighbor_list(
                &p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_geran_physical_cell_id
(
    son_rrm_geran_physical_cell_id_t *p_son_rrm_geran_physical_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_geran_physical_cell_id, 0, sizeof(son_rrm_geran_physical_cell_id_t));

    /* This function parses son_rrm_geran_physical_cell_id */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_geran_physical_cell_id");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_geran_physical_cell_id->ncc, p_src + *p_length_read, "ncc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_physical_cell_id->ncc > 7))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_physical_cell_id->ncc] should be in range [%d <= 7] !", p_son_rrm_geran_physical_cell_id->ncc);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_geran_physical_cell_id->bscc, p_src + *p_length_read, "bscc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_physical_cell_id->bscc > 7))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_physical_cell_id->bscc] should be in range [%d <= 7] !", p_son_rrm_geran_physical_cell_id->bscc);
        return RRM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_geran_neighbor_cell
(
    son_rrm_geran_neighbor_cell_t *p_son_rrm_geran_neighbor_cell,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_geran_neighbor_cell, 0, sizeof(son_rrm_geran_neighbor_cell_t));

    /* This function parses son_rrm_geran_neighbor_cell */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_geran_neighbor_cell");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_geran_neighbor_cell->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);


    if (RRM_FAILURE == rrm_parse_son_rrm_utran_geran_global_cell_id(
        &p_son_rrm_geran_neighbor_cell->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;


    if (RRM_FAILURE == rrm_parse_son_rrm_geran_physical_cell_id(
        &p_son_rrm_geran_neighbor_cell->pci,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_NR_INFO_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_son_rrm_geran_neighbor_cell->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_geran_neighbor_cell->cell_specific_offset < -15) || (p_son_rrm_geran_neighbor_cell->cell_specific_offset > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_geran_neighbor_cell->cell_specific_offset);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_NR_DTM_SUPPORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_geran_neighbor_cell->dtm_supported, p_src + *p_length_read, "dtm_supported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell->dtm_supported > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell->dtm_supported] should be in range [%d <= 1] !", p_son_rrm_geran_neighbor_cell->dtm_supported);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_RAC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_geran_neighbor_cell->rac, p_src + *p_length_read, "rac");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_geran_neighbor_list
(
    son_rrm_geran_neighbor_list_t *p_son_rrm_geran_neighbor_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_geran_neighbor_list, 0, sizeof(son_rrm_geran_neighbor_list_t));

    /* This function parses son_rrm_geran_neighbor_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_geran_neighbor_list");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_geran_neighbor_list->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->arfcn > 1023))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->arfcn] should be in range [%d <= 1023] !", p_son_rrm_geran_neighbor_list->arfcn);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_geran_neighbor_list->band_ind, p_src + *p_length_read, "band_ind");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->band_ind > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->band_ind] should be in range [%d <= 1] !", p_son_rrm_geran_neighbor_list->band_ind);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_geran_neighbor_list->cell_list_size, p_src + *p_length_read, "cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->cell_list_size > SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN] !", p_son_rrm_geran_neighbor_list->cell_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_list->cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_geran_neighbor_cell(
                &p_son_rrm_geran_neighbor_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_geran_neighbor_cell_list
(
    son_rrm_geran_neighbor_cell_list_t *p_son_rrm_geran_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_geran_neighbor_cell_list, 0, sizeof(son_rrm_geran_neighbor_cell_list_t));

    /* This function parses son_rrm_geran_neighbor_cell_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_geran_neighbor_cell_list");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size, p_src + *p_length_read, "cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_geran_neighbor_list(
                &p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size, p_src + *p_length_read, "cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_geran_neighbor_list(
                &p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size, p_src + *p_length_read, "black_list_cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_geran_neighbor_list(
                &p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size, p_src + *p_length_read, "black_list_cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_geran_neighbor_list(
                &p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_global_cell_id
(
    son_rrm_cdma_global_cell_id_t *p_son_rrm_cdma_global_cell_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_global_cell_id, 0, sizeof(son_rrm_cdma_global_cell_id_t));

    /* This function parses son_rrm_cdma_global_cell_id */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_global_cell_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_cdma_global_cell_id->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_rrm_cdma_global_cell_id->bitmask & SON_RRM_CGI_1X_RTT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_rrm_cdma_global_cell_id->cell_global_id_1X_RTT) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_cdma_global_cell_id->cell_global_id_1X_RTT); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_cdma_global_cell_id->cell_global_id_1X_RTT[loop], (void*)(p_src + *p_length_read), "cell_global_id_1X_RTT[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_son_rrm_cdma_global_cell_id->bitmask & SON_RRM_CGI_HRPD_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_son_rrm_cdma_global_cell_id->cell_global_id_HRPD) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_cdma_global_cell_id->cell_global_id_HRPD); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_cdma_global_cell_id->cell_global_id_HRPD[loop], (void*)(p_src + *p_length_read), "cell_global_id_HRPD[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_cdma_pz_hyst_params_included
(
    son_cdma_pz_hyst_params_included_t *p_son_cdma_pz_hyst_params_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_cdma_pz_hyst_params_included, 0, sizeof(son_cdma_pz_hyst_params_included_t));

    /* This function parses son_cdma_pz_hyst_params_included */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_cdma_pz_hyst_params_included");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_cdma_pz_hyst_params_included->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_ENABLED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_pz_hyst_params_included->pz_hyst_enabled, p_src + *p_length_read, "pz_hyst_enabled");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_enabled > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_enabled] should be in range [%d <= 1] !", p_son_cdma_pz_hyst_params_included->pz_hyst_enabled);
        return RRM_FAILURE;
    }
    }

    if (p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_INFO_INCL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl, p_src + *p_length_read, "pz_hyst_info_incl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl] should be in range [%d <= 1] !", p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl);
        return RRM_FAILURE;
    }
    }

    if (p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_pz_hyst_params_included->pz_hyst_act_timer, p_src + *p_length_read, "pz_hyst_act_timer");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_MUL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul, p_src + *p_length_read, "pz_hyst_timer_mul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul > 7))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul] should be in range [%d <= 7] !", p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul);
        return RRM_FAILURE;
    }
    }

    if (p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_EXP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp, p_src + *p_length_read, "pz_hyst_timer_exp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp > 31))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp] should be in range [%d <= 31] !", p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp);
        return RRM_FAILURE;
    }
    }

    if (p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_LIST_LEN_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_pz_hyst_params_included->pz_hyst_list_len, p_src + *p_length_read, "pz_hyst_list_len");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_list_len > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_list_len] should be in range [%d <= 1] !", p_son_cdma_pz_hyst_params_included->pz_hyst_list_len);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_cdma_fpc_fch_included
(
    son_cdma_fpc_fch_included_t *p_son_cdma_fpc_fch_included,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_cdma_fpc_fch_included, 0, sizeof(son_cdma_fpc_fch_included_t));

    /* This function parses son_cdma_fpc_fch_included */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_cdma_fpc_fch_included");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_cdma_fpc_fch_included->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC3_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc3, p_src + *p_length_read, "fpc_fch_init_setpt_rc3");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC4_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc4, p_src + *p_length_read, "fpc_fch_init_setpt_rc4");
    *p_length_read += sizeof(U8);
    }

    if (p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC5_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc5, p_src + *p_length_read, "fpc_fch_init_setpt_rc5");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_cell_specific_params
(
    son_rrm_cdma_cell_specific_params_t *p_son_rrm_cdma_cell_specific_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_cell_specific_params, 0, sizeof(son_rrm_cdma_cell_specific_params_t));

    /* This function parses son_rrm_cdma_cell_specific_params */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_cell_specific_params");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_cdma_cell_specific_params->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_cdma_pz_hyst_params_included(
        &p_son_rrm_cdma_cell_specific_params->pz_hyst_params_included,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_P_REV_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->p_rev, p_src + *p_length_read, "p_rev");
    *p_length_read += sizeof(U8);
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MIN_P_REV_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->min_p_rev, p_src + *p_length_read, "min_p_rev");
    *p_length_read += sizeof(U8);
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup, p_src + *p_length_read, "neg_slot_cycle_index_sup");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ENCRYPT_MODE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->encrypt_mode, p_src + *p_length_read, "encrypt_mode");
    *p_length_read += sizeof(U8);
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->encrypt_suported, p_src + *p_length_read, "encrypt_suported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->encrypt_suported > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->encrypt_suported] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->encrypt_suported);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SYG_ENCRYPT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->syg_encrypt_suported, p_src + *p_length_read, "syg_encrypt_suported");
    *p_length_read += sizeof(U8);
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->msg_integrity_sup, p_src + *p_length_read, "msg_integrity_sup");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->msg_integrity_sup > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->msg_integrity_sup] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->msg_integrity_sup);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl, p_src + *p_length_read, "sig_integrity_sup_incl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->sig_integrity_sup, p_src + *p_length_read, "sig_integrity_sup");
    *p_length_read += sizeof(U8);
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind, p_src + *p_length_read, "ms_init_pos_loc_sup_ind");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->band_class_info_req, p_src + *p_length_read, "band_class_info_req");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->band_class_info_req > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->band_class_info_req] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->band_class_info_req);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_BAND_CLASS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_cdma_cell_specific_params->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->band_class > 31))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_cell_specific_params->band_class);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ALT_BAND_CLASS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_cdma_cell_specific_params->alt_band_class, p_src + *p_length_read, "alt_band_class");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->alt_band_class > 31))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->alt_band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_cell_specific_params->alt_band_class);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_TKZ_MODE_SUP_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->tkz_mode_supported, p_src + *p_length_read, "tkz_mode_supported");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->tkz_mode_supported > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->tkz_mode_supported] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->tkz_mode_supported);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_TKZ_ID_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->tkz_id, p_src + *p_length_read, "tkz_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->tkz_id > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->tkz_id] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->tkz_id);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_FPC_FCH_INCLUDED)
    {


    if (RRM_FAILURE == rrm_parse_son_cdma_fpc_fch_included(
        &p_son_rrm_cdma_cell_specific_params->fpc_fch_included,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_T_ADD)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->t_add, p_src + *p_length_read, "t_add");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->t_add > 63))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->t_add] should be in range [%d <= 63] !", p_son_rrm_cdma_cell_specific_params->t_add);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_PILOT_INC_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_cdma_cell_specific_params->pilot_inc, p_src + *p_length_read, "pilot_inc");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->pilot_inc > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->pilot_inc] should be in range [%d <= 15] !", p_son_rrm_cdma_cell_specific_params->pilot_inc);
        return RRM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_xrtt
(
    son_rrm_cdma_neighbor_cell_xrtt_t *p_son_rrm_cdma_neighbor_cell_xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_neighbor_cell_xrtt, 0, sizeof(son_rrm_cdma_neighbor_cell_xrtt_t));

    /* This function parses son_rrm_cdma_neighbor_cell_xrtt */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_neighbor_cell_xrtt");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_cdma_neighbor_cell_xrtt->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_xrtt->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);


    if (RRM_FAILURE == rrm_parse_son_rrm_cdma_global_cell_id(
        &p_son_rrm_cdma_neighbor_cell_xrtt->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_xrtt->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    if (p_son_rrm_cdma_neighbor_cell_xrtt->bitmask & SON_RRM_CDMA_XRTT_CELL_SPECIFIC_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset < -15) || (p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_neighbor_cell_xrtt->bitmask & SON_RRM_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_rrm_cdma_cell_specific_params(
        &p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_list_xrtt
(
    son_rrm_cdma_neighbor_list_xrtt_t *p_son_rrm_cdma_neighbor_list_xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_neighbor_list_xrtt, 0, sizeof(son_rrm_cdma_neighbor_list_xrtt_t));

    /* This function parses son_rrm_cdma_neighbor_list_xrtt */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_neighbor_list_xrtt");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_list_xrtt->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_xrtt->band_class > 31))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_xrtt->band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_neighbor_list_xrtt->band_class);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size, p_src + *p_length_read, "cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size > SON_RRM_CDMA_XRTT_NBR_SIZE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size] should be in range [%d <= SON_RRM_CDMA_XRTT_NBR_SIZE] !", p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_cell_xrtt(
                &p_son_rrm_cdma_neighbor_list_xrtt->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_list_xrtt
(
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_son_rrm_cdma_neighbor_cell_list_xrtt,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_neighbor_cell_list_xrtt, 0, sizeof(son_rrm_cdma_neighbor_cell_list_xrtt_t));

    /* This function parses son_rrm_cdma_neighbor_cell_list_xrtt */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_neighbor_cell_list_xrtt");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size, p_src + *p_length_read, "cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_xrtt(
                &p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size, p_src + *p_length_read, "cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_xrtt(
                &p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size, p_src + *p_length_read, "black_list_cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_xrtt(
                &p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size, p_src + *p_length_read, "black_list_cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_xrtt(
                &p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_hrpd
(
    son_rrm_cdma_neighbor_cell_hrpd_t *p_son_rrm_cdma_neighbor_cell_hrpd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_neighbor_cell_hrpd, 0, sizeof(son_rrm_cdma_neighbor_cell_hrpd_t));

    /* This function parses son_rrm_cdma_neighbor_cell_hrpd */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_neighbor_cell_hrpd");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_cdma_neighbor_cell_hrpd->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_hrpd->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);


    if (RRM_FAILURE == rrm_parse_son_rrm_cdma_global_cell_id(
        &p_son_rrm_cdma_neighbor_cell_hrpd->cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_hrpd->arfcn, p_src + *p_length_read, "arfcn");
    *p_length_read += sizeof(U16);

    if (p_son_rrm_cdma_neighbor_cell_hrpd->bitmask & SON_RRM_CDMA_HRPD_CELL_SPECIFIC_OFFSET_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset, p_src + *p_length_read, "cell_specific_offset");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset < -15) || (p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset > 15))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset);
        return RRM_FAILURE;
    }
    }

    if (p_son_rrm_cdma_neighbor_cell_hrpd->bitmask & SON_RRM_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_rrm_cdma_cell_specific_params(
        &p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_params,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_list_hrpd
(
    son_rrm_cdma_neighbor_list_hrpd_t *p_son_rrm_cdma_neighbor_list_hrpd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_neighbor_list_hrpd, 0, sizeof(son_rrm_cdma_neighbor_list_hrpd_t));

    /* This function parses son_rrm_cdma_neighbor_list_hrpd */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_neighbor_list_hrpd");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_list_hrpd->band_class, p_src + *p_length_read, "band_class");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_hrpd->band_class > 31))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_hrpd->band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_neighbor_list_hrpd->band_class);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size, p_src + *p_length_read, "cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size > SON_RRM_CDMA_HRPD_NBR_SIZE))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size] should be in range [%d <= SON_RRM_CDMA_HRPD_NBR_SIZE] !", p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_cell_hrpd(
                &p_son_rrm_cdma_neighbor_list_hrpd->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_cdma_neighbor_cell_list_hrpd
(
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_son_rrm_cdma_neighbor_cell_list_hrpd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_cdma_neighbor_cell_list_hrpd, 0, sizeof(son_rrm_cdma_neighbor_cell_list_hrpd_t));

    /* This function parses son_rrm_cdma_neighbor_cell_list_hrpd */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_cdma_neighbor_cell_list_hrpd");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size, p_src + *p_length_read, "cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_hrpd(
                &p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size, p_src + *p_length_read, "cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_hrpd(
                &p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size, p_src + *p_length_read, "black_list_cells_to_add_mod_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_hrpd(
                &p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size, p_src + *p_length_read, "black_list_cells_to_remove_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_list_hrpd(
                &p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_inter_rat_neighbor_cell_list
(
    son_rrm_inter_rat_neighbor_cell_list_t *p_son_rrm_inter_rat_neighbor_cell_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_inter_rat_neighbor_cell_list, 0, sizeof(son_rrm_inter_rat_neighbor_cell_list_t));

    /* This function parses son_rrm_inter_rat_neighbor_cell_list */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_inter_rat_neighbor_cell_list");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_inter_rat_neighbor_cell_list->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U32);

    if (p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_UTRAN_NR_LIST_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_rrm_utran_neighbor_cell_list(
        &p_son_rrm_inter_rat_neighbor_cell_list->utran_neighbor_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_GERAN_NR_LIST_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_rrm_geran_neighbor_cell_list(
        &p_son_rrm_inter_rat_neighbor_cell_list->geran_neighbor_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_CDMA_NR_LIST_XRTT_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_cell_list_xrtt(
        &p_son_rrm_inter_rat_neighbor_cell_list->cdma_neighbor_list_xrtt,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_CDMA_NR_LIST_HRPD_PRESENT)
    {


    if (RRM_FAILURE == rrm_parse_son_rrm_cdma_neighbor_cell_list_hrpd(
        &p_son_rrm_inter_rat_neighbor_cell_list->cdma_neighbor_list_hrpd,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_apps_rrm_carrier_freq_and_dl_power_params_req, 0, sizeof(son_apps_rrm_carrier_freq_and_dl_power_params_req_t));

    /* This function parses son_apps_rrm_carrier_freq_and_dl_power_params_req */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_apps_rrm_carrier_freq_and_dl_power_params_req");


    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth, p_src + *p_length_read, "meas_bandwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth < 6) || (p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth > 100))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn, p_src + *p_length_read, "num_earfcn");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn * sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->earfcn[0])) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn; loop++)
        {
            rrm_cp_unpack_U32((void*)&p_son_apps_rrm_carrier_freq_and_dl_power_params_req->earfcn[loop], (void*)(p_src + *p_length_read), "earfcn[]");
            *p_length_read += sizeof(U32);
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp, 0, sizeof(son_apps_rrm_carrier_freq_and_dl_power_params_resp_t));

    /* This function parses son_apps_rrm_carrier_freq_and_dl_power_params_resp */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_apps_rrm_carrier_freq_and_dl_power_params_resp");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result, p_src + *p_length_read, "result");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result > 1))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result] should be in range [%d <= 1] !", p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U32);


    if (RRM_FAILURE == rrm_parse_son_rrm_carrier_freq_and_dl_power_params_resp(
        &p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->carrier_freq_and_dl_power_params_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_neighbor_cell_signal_power
(
    son_rrm_neighbor_cell_signal_power_t *p_son_rrm_neighbor_cell_signal_power,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_neighbor_cell_signal_power, 0, sizeof(son_rrm_neighbor_cell_signal_power_t));

    /* This function parses son_rrm_neighbor_cell_signal_power */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_neighbor_cell_signal_power");


    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_neighbor_cell_signal_power->pci_id, p_src + *p_length_read, "pci_id");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_neighbor_cell_signal_power->pci_id > 503))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_neighbor_cell_signal_power->pci_id] should be in range [%d <= 503] !", p_son_rrm_neighbor_cell_signal_power->pci_id);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_neighbor_cell_signal_power->rsrq, p_src + *p_length_read, "rsrq");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_neighbor_cell_signal_power->rsrq > 34))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_neighbor_cell_signal_power->rsrq] should be in range [%d <= 34] !", p_son_rrm_neighbor_cell_signal_power->rsrq);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_neighbor_cell_signal_power->rsrp, p_src + *p_length_read, "rsrp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_neighbor_cell_signal_power->rsrp > 128))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_neighbor_cell_signal_power->rsrp] should be in range [%d <= 128] !", p_son_rrm_neighbor_cell_signal_power->rsrp);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_S8(&p_son_rrm_neighbor_cell_signal_power->reference_signal_power, p_src + *p_length_read, "reference_signal_power");
    *p_length_read += sizeof(S8);

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_carrier_freq_and_dl_power_params_resp
(
    son_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_rrm_carrier_freq_and_dl_power_params_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_carrier_freq_and_dl_power_params_resp, 0, sizeof(son_rrm_carrier_freq_and_dl_power_params_resp_t));

    /* This function parses son_rrm_carrier_freq_and_dl_power_params_resp */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_carrier_freq_and_dl_power_params_resp");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_carrier_freq_and_dl_power_params_resp->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U8(&p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi, p_src + *p_length_read, "rssi");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi > 128))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi] should be in range [%d <= 128] !", p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi);
        return RRM_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U16(&p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size, p_src + *p_length_read, "neighbor_cell_list_size");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size > SON_MAX_NO_CELLS_PER_EARFCN))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size] should be in range [%d <= SON_MAX_NO_CELLS_PER_EARFCN] !", p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size);
        return RRM_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size; loop++)
        {
            if (RRM_FAILURE == rrm_parse_son_rrm_neighbor_cell_signal_power(
                &p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_signal_power[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_son_rrm_ttt_update_ind
(
    son_rrm_ttt_update_ind_t *p_son_rrm_ttt_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_ttt_update_ind, 0, sizeof(son_rrm_ttt_update_ind_t));

    /* This function parses son_rrm_ttt_update_ind */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_ttt_update_ind");



    if (RRM_FAILURE == rrm_parse_son_rrm_intra_rat_global_cell_id(
        &p_son_rrm_ttt_update_ind->srv_cgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_ttt_update_ind->updated_ttt, p_src + *p_length_read, "updated_ttt");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

rrm_return_et
rrm_parse_son_rrm_stop_tnl_discovery_ind
(
    son_rrm_stop_tnl_discovery_ind_t *p_son_rrm_stop_tnl_discovery_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_stop_tnl_discovery_ind, 0, sizeof(son_rrm_stop_tnl_discovery_ind_t));

    /* This function parses son_rrm_stop_tnl_discovery_ind */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_stop_tnl_discovery_ind");



    if (RRM_FAILURE == rrm_parse_son_rrm_global_enb_id(
        &p_son_rrm_stop_tnl_discovery_ind->target_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

static
rrm_return_et
rrm_parse_son_rrm_global_enb_id
(
    son_rrm_global_enb_id_t *p_son_rrm_global_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;
    *p_length_read = 0;

    RRM_MEMSET(p_son_rrm_global_enb_id, 0, sizeof(son_rrm_global_enb_id_t));

    /* This function parses son_rrm_global_enb_id */
    RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_COMP_PARSE, "Parsing son_rrm_global_enb_id");


    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrm_cp_unpack_U32(&p_son_rrm_global_enb_id->enb_type, p_src + *p_length_read, "enb_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_global_enb_id->enb_type < 1) || (p_son_rrm_global_enb_id->enb_type > 2))
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Parameter [p_son_rrm_global_enb_id->enb_type] should be in range [1 <= %d <= 2] !", p_son_rrm_global_enb_id->enb_type);
        return RRM_FAILURE;
    }


    if (RRM_FAILURE == rrm_parse_son_rrm_cell_plmn_info(
        &p_son_rrm_global_enb_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_son_rrm_global_enb_id->enb_id) > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrm_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_global_enb_id->enb_id); loop++)
        {
            rrm_cp_unpack_U8((void*)&p_son_rrm_global_enb_id->enb_id[loop], (void*)(p_src + *p_length_read), "enb_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRM_TRACE(RRM_TRUE, RRM_PNULL, RRM_WARNING, "Incoming message damaged!");
        return RRM_FAILURE;
    }

    return(RRM_SUCCESS);
}

