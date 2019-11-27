/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rrm_composer.c $
 *
 *******************************************************************************
 *
 * File Description: File son_rrm_composer.c added for SON module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "son_pack_unpack_utils.h"
#include "son_rrm_composer.h"
#include "son_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
extern const son_8* son_utils_facility_name;


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
son_length_t
son_get_son_rrm_cell_plmn_info_len
(
    son_rrm_cell_plmn_info_t *p_son_rrm_cell_plmn_info
);

static
son_return_et
son_compose_son_rrm_cell_plmn_info
(
    U8  **pp_buffer,
    son_rrm_cell_plmn_info_t *p_son_rrm_cell_plmn_info
);

static
son_length_t
son_get_son_rrm_intra_rat_global_cell_id_len
(
    son_rrm_intra_rat_global_cell_id_t *p_son_rrm_intra_rat_global_cell_id
);

static
son_return_et
son_compose_son_rrm_intra_rat_global_cell_id
(
    U8  **pp_buffer,
    son_rrm_intra_rat_global_cell_id_t *p_son_rrm_intra_rat_global_cell_id
);

static
son_length_t
son_get_son_subframe_allocation_len
(
    son_subframe_allocation_t *p_son_subframe_allocation
);

static
son_return_et
son_compose_son_subframe_allocation
(
    U8  **pp_buffer,
    son_subframe_allocation_t *p_son_subframe_allocation
);

static
son_length_t
son_get_son_mbsfn_info_len
(
    son_mbsfn_info_t *p_son_mbsfn_info
);

static
son_return_et
son_compose_son_mbsfn_info
(
    U8  **pp_buffer,
    son_mbsfn_info_t *p_son_mbsfn_info
);

static
son_length_t
son_get_son_mbms_configuration_len
(
    son_mbms_configuration_t *p_son_mbms_configuration
);

static
son_return_et
son_compose_son_mbms_configuration
(
    U8  **pp_buffer,
    son_mbms_configuration_t *p_son_mbms_configuration
);

static
son_length_t
son_get_son_rrm_intra_rat_neighbor_cell_len
(
    son_rrm_intra_rat_neighbor_cell_t *p_son_rrm_intra_rat_neighbor_cell
);

static
son_return_et
son_compose_son_rrm_intra_rat_neighbor_cell
(
    U8  **pp_buffer,
    son_rrm_intra_rat_neighbor_cell_t *p_son_rrm_intra_rat_neighbor_cell
);

static
son_length_t
son_get_son_rrm_intra_rat_neighbor_list_len
(
    son_rrm_intra_rat_neighbor_list_t *p_son_rrm_intra_rat_neighbor_list
);

static
son_return_et
son_compose_son_rrm_intra_rat_neighbor_list
(
    U8  **pp_buffer,
    son_rrm_intra_rat_neighbor_list_t *p_son_rrm_intra_rat_neighbor_list
);

static
son_length_t
son_get_son_rrm_intra_rat_neighbor_cell_list_len
(
    son_rrm_intra_rat_neighbor_cell_list_t *p_son_rrm_intra_rat_neighbor_cell_list
);

static
son_return_et
son_compose_son_rrm_intra_rat_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_intra_rat_neighbor_cell_list_t *p_son_rrm_intra_rat_neighbor_cell_list
);

static
son_length_t
son_get_son_rrm_utran_geran_global_cell_id_len
(
    son_rrm_utran_geran_global_cell_id_t *p_son_rrm_utran_geran_global_cell_id
);

static
son_return_et
son_compose_son_rrm_utran_geran_global_cell_id
(
    U8  **pp_buffer,
    son_rrm_utran_geran_global_cell_id_t *p_son_rrm_utran_geran_global_cell_id
);

static
son_length_t
son_get_son_rrm_utran_physical_cell_id_len
(
    son_rrm_utran_physical_cell_id_t *p_son_rrm_utran_physical_cell_id
);

static
son_return_et
son_compose_son_rrm_utran_physical_cell_id
(
    U8  **pp_buffer,
    son_rrm_utran_physical_cell_id_t *p_son_rrm_utran_physical_cell_id
);

static
son_length_t
son_get_son_rrm_utran_neighbor_cell_len
(
    son_rrm_utran_neighbor_cell_t *p_son_rrm_utran_neighbor_cell
);

static
son_return_et
son_compose_son_rrm_utran_neighbor_cell
(
    U8  **pp_buffer,
    son_rrm_utran_neighbor_cell_t *p_son_rrm_utran_neighbor_cell
);

static
son_length_t
son_get_son_rrm_utran_neighbor_list_len
(
    son_rrm_utran_neighbor_list_t *p_son_rrm_utran_neighbor_list
);

static
son_return_et
son_compose_son_rrm_utran_neighbor_list
(
    U8  **pp_buffer,
    son_rrm_utran_neighbor_list_t *p_son_rrm_utran_neighbor_list
);

static
son_length_t
son_get_son_rrm_utran_neighbor_cell_list_len
(
    son_rrm_utran_neighbor_cell_list_t *p_son_rrm_utran_neighbor_cell_list
);

static
son_return_et
son_compose_son_rrm_utran_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_utran_neighbor_cell_list_t *p_son_rrm_utran_neighbor_cell_list
);

static
son_length_t
son_get_son_rrm_geran_physical_cell_id_len
(
    son_rrm_geran_physical_cell_id_t *p_son_rrm_geran_physical_cell_id
);

static
son_return_et
son_compose_son_rrm_geran_physical_cell_id
(
    U8  **pp_buffer,
    son_rrm_geran_physical_cell_id_t *p_son_rrm_geran_physical_cell_id
);

static
son_length_t
son_get_son_rrm_geran_neighbor_cell_len
(
    son_rrm_geran_neighbor_cell_t *p_son_rrm_geran_neighbor_cell
);

static
son_return_et
son_compose_son_rrm_geran_neighbor_cell
(
    U8  **pp_buffer,
    son_rrm_geran_neighbor_cell_t *p_son_rrm_geran_neighbor_cell
);

static
son_length_t
son_get_son_rrm_geran_neighbor_list_len
(
    son_rrm_geran_neighbor_list_t *p_son_rrm_geran_neighbor_list
);

static
son_return_et
son_compose_son_rrm_geran_neighbor_list
(
    U8  **pp_buffer,
    son_rrm_geran_neighbor_list_t *p_son_rrm_geran_neighbor_list
);

static
son_length_t
son_get_son_rrm_geran_neighbor_cell_list_len
(
    son_rrm_geran_neighbor_cell_list_t *p_son_rrm_geran_neighbor_cell_list
);

static
son_return_et
son_compose_son_rrm_geran_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_geran_neighbor_cell_list_t *p_son_rrm_geran_neighbor_cell_list
);

static
son_length_t
son_get_son_rrm_cdma_global_cell_id_len
(
    son_rrm_cdma_global_cell_id_t *p_son_rrm_cdma_global_cell_id
);

static
son_return_et
son_compose_son_rrm_cdma_global_cell_id
(
    U8  **pp_buffer,
    son_rrm_cdma_global_cell_id_t *p_son_rrm_cdma_global_cell_id
);

static
son_length_t
son_get_son_cdma_pz_hyst_params_included_len
(
    son_cdma_pz_hyst_params_included_t *p_son_cdma_pz_hyst_params_included
);

static
son_return_et
son_compose_son_cdma_pz_hyst_params_included
(
    U8  **pp_buffer,
    son_cdma_pz_hyst_params_included_t *p_son_cdma_pz_hyst_params_included
);

static
son_length_t
son_get_son_cdma_fpc_fch_included_len
(
    son_cdma_fpc_fch_included_t *p_son_cdma_fpc_fch_included
);

static
son_return_et
son_compose_son_cdma_fpc_fch_included
(
    U8  **pp_buffer,
    son_cdma_fpc_fch_included_t *p_son_cdma_fpc_fch_included
);

static
son_length_t
son_get_son_rrm_cdma_cell_specific_params_len
(
    son_rrm_cdma_cell_specific_params_t *p_son_rrm_cdma_cell_specific_params
);

static
son_return_et
son_compose_son_rrm_cdma_cell_specific_params
(
    U8  **pp_buffer,
    son_rrm_cdma_cell_specific_params_t *p_son_rrm_cdma_cell_specific_params
);

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_xrtt_len
(
    son_rrm_cdma_neighbor_cell_xrtt_t *p_son_rrm_cdma_neighbor_cell_xrtt
);

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_xrtt
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_xrtt_t *p_son_rrm_cdma_neighbor_cell_xrtt
);

static
son_length_t
son_get_son_rrm_cdma_neighbor_list_xrtt_len
(
    son_rrm_cdma_neighbor_list_xrtt_t *p_son_rrm_cdma_neighbor_list_xrtt
);

static
son_return_et
son_compose_son_rrm_cdma_neighbor_list_xrtt
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_list_xrtt_t *p_son_rrm_cdma_neighbor_list_xrtt
);

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_list_xrtt_len
(
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_son_rrm_cdma_neighbor_cell_list_xrtt
);

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_list_xrtt
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_son_rrm_cdma_neighbor_cell_list_xrtt
);

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_hrpd_len
(
    son_rrm_cdma_neighbor_cell_hrpd_t *p_son_rrm_cdma_neighbor_cell_hrpd
);

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_hrpd
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_hrpd_t *p_son_rrm_cdma_neighbor_cell_hrpd
);

static
son_length_t
son_get_son_rrm_cdma_neighbor_list_hrpd_len
(
    son_rrm_cdma_neighbor_list_hrpd_t *p_son_rrm_cdma_neighbor_list_hrpd
);

static
son_return_et
son_compose_son_rrm_cdma_neighbor_list_hrpd
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_list_hrpd_t *p_son_rrm_cdma_neighbor_list_hrpd
);

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_list_hrpd_len
(
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_son_rrm_cdma_neighbor_cell_list_hrpd
);

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_list_hrpd
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_son_rrm_cdma_neighbor_cell_list_hrpd
);

static
son_length_t
son_get_son_rrm_inter_rat_neighbor_cell_list_len
(
    son_rrm_inter_rat_neighbor_cell_list_t *p_son_rrm_inter_rat_neighbor_cell_list
);

static
son_return_et
son_compose_son_rrm_inter_rat_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_inter_rat_neighbor_cell_list_t *p_son_rrm_inter_rat_neighbor_cell_list
);

static
son_length_t
son_get_son_rrm_neighbor_cell_signal_power_len
(
    son_rrm_neighbor_cell_signal_power_t *p_son_rrm_neighbor_cell_signal_power
);

static
son_return_et
son_compose_son_rrm_neighbor_cell_signal_power
(
    U8  **pp_buffer,
    son_rrm_neighbor_cell_signal_power_t *p_son_rrm_neighbor_cell_signal_power
);

static
son_length_t
son_get_son_rrm_carrier_freq_and_dl_power_params_resp_len
(
    son_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_rrm_carrier_freq_and_dl_power_params_resp
);

static
son_return_et
son_compose_son_rrm_carrier_freq_and_dl_power_params_resp
(
    U8  **pp_buffer,
    son_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_rrm_carrier_freq_and_dl_power_params_resp
);

static
son_length_t
son_get_son_rrm_global_enb_id_len
(
    son_rrm_global_enb_id_t *p_son_rrm_global_enb_id
);

static
son_return_et
son_compose_son_rrm_global_enb_id
(
    U8  **pp_buffer,
    son_rrm_global_enb_id_t *p_son_rrm_global_enb_id
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

son_length_t
son_get_son_anr_rrm_updated_nrt_info_len
(
    son_anr_rrm_updated_nrt_info_t *p_son_anr_rrm_updated_nrt_info
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_anr_rrm_updated_nrt_info != SON_PNULL);


    /* Get length of IE */
    length += son_get_son_rrm_intra_rat_global_cell_id_len(&p_son_anr_rrm_updated_nrt_info->src_cgi);

    /* Get length of IE */
    length += son_get_son_rrm_intra_rat_neighbor_cell_list_len(&p_son_anr_rrm_updated_nrt_info->intra_rat_ncl);

    /* Get length of IE */
    length += son_get_son_rrm_inter_rat_neighbor_cell_list_len(&p_son_anr_rrm_updated_nrt_info->inter_rat_ncl);

    return length;
}

son_return_et
son_compose_son_anr_rrm_updated_nrt_info
(
    U8  **pp_buffer,
    son_anr_rrm_updated_nrt_info_t *p_son_anr_rrm_updated_nrt_info
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_anr_rrm_updated_nrt_info != SON_PNULL);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_intra_rat_global_cell_id(pp_buffer, &p_son_anr_rrm_updated_nrt_info->src_cgi))
    {
        return SON_FAILURE;
    }

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_intra_rat_neighbor_cell_list(pp_buffer, &p_son_anr_rrm_updated_nrt_info->intra_rat_ncl))
    {
        return SON_FAILURE;
    }

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_inter_rat_neighbor_cell_list(pp_buffer, &p_son_anr_rrm_updated_nrt_info->inter_rat_ncl))
    {
        return SON_FAILURE;
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_rrm_send_son_anr_rrm_updated_nrt_info
*
*   DESCRIPTION:
*       This function constructs and sends SON_ANR_RRM_UPDATED_NRT_INFO message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_rrm_send_son_anr_rrm_updated_nrt_info
(
    son_anr_rrm_updated_nrt_info_t  *p_son_anr_rrm_updated_nrt_info,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rrm_msg;

    SON_ASSERT(p_son_anr_rrm_updated_nrt_info != SON_PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = son_get_son_anr_rrm_updated_nrt_info_len(p_son_anr_rrm_updated_nrt_info);
   if(msg_length == SON_FAILURE)
    {
        return SON_FAILURE;
    }

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_INFO, "Src(%u)->Dst(%u):SON_ANR_RRM_UPDATED_NRT_INFO", src_module_id, dest_module_id);

    msg_api_length = msg_length + SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_rrm_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_rrm_msg, SON_VERSION_ID, src_module_id,
        dest_module_id, SON_ANR_RRM_UPDATED_NRT_INFO,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_rrm_msg = p_rrm_msg + SON_API_HEADER_SIZE;
    son_construct_interface_api_header(p_rrm_msg, transaction_id,  (son_module_id_et)src_module_id,
        (son_module_id_et)dest_module_id, SON_ANR_RRM_UPDATED_NRT_INFO, msg_length, cell_index);

    /* Fill SON_ANR_RRM_UPDATED_NRT_INFO message */
    p_rrm_msg = p_rrm_msg + EXT_MSG_API_HDR_SIZE;
    if (SON_FAILURE == son_compose_son_anr_rrm_updated_nrt_info(&p_rrm_msg, p_son_anr_rrm_updated_nrt_info))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    if((p_rrm_msg - msg_api_length) != p_msg)
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }
    /* Send message to */
    son_send_msg(p_msg, dest_module_id);

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cell_plmn_info_len
(
    son_rrm_cell_plmn_info_t *p_son_rrm_cell_plmn_info
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cell_plmn_info != SON_PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_cell_plmn_info->mcc);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cell_plmn_info->num_mnc_digit);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_son_rrm_cell_plmn_info->num_mnc_digit * sizeof(p_son_rrm_cell_plmn_info->mnc[0]));

    return length;
}

static
son_return_et
son_compose_son_rrm_cell_plmn_info
(
    U8  **pp_buffer,
    son_rrm_cell_plmn_info_t *p_son_rrm_cell_plmn_info
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cell_plmn_info != SON_PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_cell_plmn_info->mcc); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_cell_plmn_info->mcc[loop], "mcc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_cell_plmn_info->num_mnc_digit < 2) || (p_son_rrm_cell_plmn_info->num_mnc_digit > SON_MAX_MNC_OCTET_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cell_plmn_info->num_mnc_digit] should be in range [2 <= %d <= SON_MAX_MNC_OCTET_SIZE] !", p_son_rrm_cell_plmn_info->num_mnc_digit);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cell_plmn_info->num_mnc_digit, "num_mnc_digit");
    *pp_buffer += sizeof(p_son_rrm_cell_plmn_info->num_mnc_digit);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cell_plmn_info->num_mnc_digit; loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_cell_plmn_info->mnc[loop], "mnc[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_intra_rat_global_cell_id_len
(
    son_rrm_intra_rat_global_cell_id_t *p_son_rrm_intra_rat_global_cell_id
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_intra_rat_global_cell_id != SON_PNULL);


    /* Get length of IE */
    length += son_get_son_rrm_cell_plmn_info_len(&p_son_rrm_intra_rat_global_cell_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_intra_rat_global_cell_id->cell_identity);

    return length;
}

static
son_return_et
son_compose_son_rrm_intra_rat_global_cell_id
(
    U8  **pp_buffer,
    son_rrm_intra_rat_global_cell_id_t *p_son_rrm_intra_rat_global_cell_id
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_intra_rat_global_cell_id != SON_PNULL);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cell_plmn_info(pp_buffer, &p_son_rrm_intra_rat_global_cell_id->plmn_id))
    {
        return SON_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_intra_rat_global_cell_id->cell_identity); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_intra_rat_global_cell_id->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_subframe_allocation_len
(
    son_subframe_allocation_t *p_son_subframe_allocation
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_subframe_allocation != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_subframe_allocation->bitmask);

    /* Optional element */
    if(p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_subframe_allocation->oneframe);
    }

    /* Optional element */
    if(p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_subframe_allocation->fourframe);
    }

    return length;
}

static
son_return_et
son_compose_son_subframe_allocation
(
    U8  **pp_buffer,
    son_subframe_allocation_t *p_son_subframe_allocation
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_subframe_allocation != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_subframe_allocation->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_subframe_allocation->bitmask);

    /* Optional element */
    if(p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_subframe_allocation->oneframe); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_subframe_allocation->oneframe[loop], "oneframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_son_subframe_allocation->bitmask & SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_subframe_allocation->fourframe); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_subframe_allocation->fourframe[loop], "fourframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_mbsfn_info_len
(
    son_mbsfn_info_t *p_son_mbsfn_info
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_mbsfn_info != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_mbsfn_info->radioframe_alloc_period);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_mbsfn_info->radioframe_alloc_offset);

    /* Get length of IE */
    length += son_get_son_subframe_allocation_len(&p_son_mbsfn_info->subframe_alloc);

    return length;
}

static
son_return_et
son_compose_son_mbsfn_info
(
    U8  **pp_buffer,
    son_mbsfn_info_t *p_son_mbsfn_info
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_mbsfn_info != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mbsfn_info->radioframe_alloc_period > 5))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbsfn_info->radioframe_alloc_period] should be in range [%d <= 5] !", p_son_mbsfn_info->radioframe_alloc_period);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_mbsfn_info->radioframe_alloc_period, "radioframe_alloc_period");
    *pp_buffer += sizeof(p_son_mbsfn_info->radioframe_alloc_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_mbsfn_info->radioframe_alloc_offset > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbsfn_info->radioframe_alloc_offset] should be in range [%d <= 7] !", p_son_mbsfn_info->radioframe_alloc_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_mbsfn_info->radioframe_alloc_offset, "radioframe_alloc_offset");
    *pp_buffer += sizeof(p_son_mbsfn_info->radioframe_alloc_offset);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_subframe_allocation(pp_buffer, &p_son_mbsfn_info->subframe_alloc))
    {
        return SON_FAILURE;
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_mbms_configuration_len
(
    son_mbms_configuration_t *p_son_mbms_configuration
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_mbms_configuration != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_mbms_configuration->num_mbsfn);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mbms_configuration->num_mbsfn < 1) || (p_son_mbms_configuration->num_mbsfn > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbms_configuration->num_mbsfn] should be in range [1 <= %d <= 8] !", p_son_mbms_configuration->num_mbsfn);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_mbms_configuration->num_mbsfn; loop++)
        {
            length += son_get_son_mbsfn_info_len(&p_son_mbms_configuration->mbsfn_info[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_mbms_configuration
(
    U8  **pp_buffer,
    son_mbms_configuration_t *p_son_mbms_configuration
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_mbms_configuration != SON_PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_mbms_configuration->num_mbsfn < 1) || (p_son_mbms_configuration->num_mbsfn > 8))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_mbms_configuration->num_mbsfn] should be in range [1 <= %d <= 8] !", p_son_mbms_configuration->num_mbsfn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_mbms_configuration->num_mbsfn, "num_mbsfn");
    *pp_buffer += sizeof(p_son_mbms_configuration->num_mbsfn);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_mbms_configuration->num_mbsfn; loop++)
        {
            if (SON_FAILURE == son_compose_son_mbsfn_info(pp_buffer, &p_son_mbms_configuration->mbsfn_info[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_intra_rat_neighbor_cell_len
(
    son_rrm_intra_rat_neighbor_cell_t *p_son_rrm_intra_rat_neighbor_cell
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_intra_rat_neighbor_cell != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->bitmask);

    /* Get length of IE */
    length += son_get_son_rrm_intra_rat_global_cell_id_len(&p_son_rrm_intra_rat_neighbor_cell->cgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->pci);

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_RAT_NR_INFO_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_ENABLE_X2_STATUS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->x2_status);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_TAC_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->tac);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_ACCESS_MODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->access_mode);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_BROADCAST_STATUS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->broadcast_status);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_CSG_IDENTITY_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->csg_identity);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_CELL_INTF_SCHEME_TYPE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->interference_scheme);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_MBSFN_CONFIG_LIST_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_mbms_configuration_len(&p_son_rrm_intra_rat_neighbor_cell->mbms_config);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_TDD_SF_ASSN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell->subframe_assn);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_intra_rat_neighbor_cell
(
    U8  **pp_buffer,
    son_rrm_intra_rat_neighbor_cell_t *p_son_rrm_intra_rat_neighbor_cell
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_intra_rat_neighbor_cell != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->bitmask);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_intra_rat_global_cell_id(pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->cgi))
    {
        return SON_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->pci > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->pci] should be in range [%d <= 503] !", p_son_rrm_intra_rat_neighbor_cell->pci);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->pci, "pci");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->pci);

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_RAT_NR_INFO_OFFSET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset] should be in range [%d <= 30] !", p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset, "cell_specific_offset");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_ENABLE_X2_STATUS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_intra_rat_neighbor_cell->x2_status < 3) || (p_son_rrm_intra_rat_neighbor_cell->x2_status > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->x2_status] should be in range [3 <= %d <= 7] !", p_son_rrm_intra_rat_neighbor_cell->x2_status);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->x2_status, "x2_status");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->x2_status);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_TAC_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_intra_rat_neighbor_cell->tac); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_RAT_NR_INFO_CM_OFFSET_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset > 30))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset] should be in range [%d <= 30] !", p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset, "cm_cell_specific_offset");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->cm_cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_ACCESS_MODE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->access_mode > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->access_mode] should be in range [%d <= 3] !", p_son_rrm_intra_rat_neighbor_cell->access_mode);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->access_mode, "access_mode");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->access_mode);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_BROADCAST_STATUS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->broadcast_status > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->broadcast_status] should be in range [%d <= 1] !", p_son_rrm_intra_rat_neighbor_cell->broadcast_status);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->broadcast_status, "broadcast_status");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->broadcast_status);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_INTRA_CSG_IDENTITY_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_intra_rat_neighbor_cell->csg_identity); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_CELL_INTF_SCHEME_TYPE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->interference_scheme > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->interference_scheme] should be in range [%d <= 2] !", p_son_rrm_intra_rat_neighbor_cell->interference_scheme);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->interference_scheme, "interference_scheme");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->interference_scheme);
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_MBSFN_CONFIG_LIST_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_mbms_configuration(pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->mbms_config))
    {
        return SON_FAILURE;
    }
    }

    /* Optional element */
    if(p_son_rrm_intra_rat_neighbor_cell->bitmask & SON_RRM_TDD_SF_ASSN_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell->subframe_assn > 6))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell->subframe_assn] should be in range [%d <= 6] !", p_son_rrm_intra_rat_neighbor_cell->subframe_assn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell->subframe_assn, "subframe_assn");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell->subframe_assn);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_intra_rat_neighbor_list_len
(
    son_rrm_intra_rat_neighbor_list_t *p_son_rrm_intra_rat_neighbor_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_intra_rat_neighbor_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_list->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_list->cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_list->cell_list_size > SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN] !", p_son_rrm_intra_rat_neighbor_list->cell_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_list->cell_list_size; loop++)
        {
            length += son_get_son_rrm_intra_rat_neighbor_cell_len(&p_son_rrm_intra_rat_neighbor_list->cell_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_intra_rat_neighbor_list
(
    U8  **pp_buffer,
    son_rrm_intra_rat_neighbor_list_t *p_son_rrm_intra_rat_neighbor_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_intra_rat_neighbor_list != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_intra_rat_neighbor_list->earfcn, "earfcn");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_list->earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_list->cell_list_size > SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_HO_ALLWED_BLKLISTED_NBRS_PER_EARFCN] !", p_son_rrm_intra_rat_neighbor_list->cell_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_intra_rat_neighbor_list->cell_list_size, "cell_list_size");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_list->cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_list->cell_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_intra_rat_neighbor_cell(pp_buffer, &p_son_rrm_intra_rat_neighbor_list->cell_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_intra_rat_neighbor_cell_list_len
(
    son_rrm_intra_rat_neighbor_cell_list_t *p_son_rrm_intra_rat_neighbor_cell_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_intra_rat_neighbor_cell_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_intra_rat_neighbor_list_len(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_intra_rat_neighbor_list_len(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_intra_rat_neighbor_list_len(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_intra_rat_neighbor_list_len(&p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_intra_rat_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_intra_rat_neighbor_cell_list_t *p_son_rrm_intra_rat_neighbor_cell_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_intra_rat_neighbor_cell_list != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size, "intra_rat_cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_intra_rat_neighbor_list(pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size, "intra_rat_cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_intra_rat_neighbor_list(pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size, "intra_rat_black_list_cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_intra_rat_neighbor_list(pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size > SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_INTRA_RAT_FREQ_ALLOWED] !", p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size, "intra_rat_black_list_cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_intra_rat_neighbor_list(pp_buffer, &p_son_rrm_intra_rat_neighbor_cell_list->intra_rat_black_list_cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_utran_geran_global_cell_id_len
(
    son_rrm_utran_geran_global_cell_id_t *p_son_rrm_utran_geran_global_cell_id
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_utran_geran_global_cell_id != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_geran_global_cell_id->rat_type);

    /* Get length of IE */
    length += son_get_son_rrm_cell_plmn_info_len(&p_son_rrm_utran_geran_global_cell_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_utran_geran_global_cell_id->cell_identity);

    return length;
}

static
son_return_et
son_compose_son_rrm_utran_geran_global_cell_id
(
    U8  **pp_buffer,
    son_rrm_utran_geran_global_cell_id_t *p_son_rrm_utran_geran_global_cell_id
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_utran_geran_global_cell_id != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_utran_geran_global_cell_id->rat_type, "rat_type");
    *pp_buffer += sizeof(p_son_rrm_utran_geran_global_cell_id->rat_type);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cell_plmn_info(pp_buffer, &p_son_rrm_utran_geran_global_cell_id->plmn_id))
    {
        return SON_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_utran_geran_global_cell_id->cell_identity); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_utran_geran_global_cell_id->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_utran_physical_cell_id_len
(
    son_rrm_utran_physical_cell_id_t *p_son_rrm_utran_physical_cell_id
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_utran_physical_cell_id != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_physical_cell_id->bitmask);

    /* Optional element */
    if(p_son_rrm_utran_physical_cell_id->bitmask & SON_PCI_FDD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_physical_cell_id->pci_fdd);
    }

    /* Optional element */
    if(p_son_rrm_utran_physical_cell_id->bitmask & SON_PCI_TDD_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_physical_cell_id->pci_tdd);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_utran_physical_cell_id
(
    U8  **pp_buffer,
    son_rrm_utran_physical_cell_id_t *p_son_rrm_utran_physical_cell_id
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_utran_physical_cell_id != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_utran_physical_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_utran_physical_cell_id->bitmask);

    /* Optional element */
    if(p_son_rrm_utran_physical_cell_id->bitmask & SON_PCI_FDD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_physical_cell_id->pci_fdd > 511))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_physical_cell_id->pci_fdd] should be in range [%d <= 511] !", p_son_rrm_utran_physical_cell_id->pci_fdd);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_physical_cell_id->pci_fdd, "pci_fdd");
    *pp_buffer += sizeof(p_son_rrm_utran_physical_cell_id->pci_fdd);
    }

    /* Optional element */
    if(p_son_rrm_utran_physical_cell_id->bitmask & SON_PCI_TDD_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_physical_cell_id->pci_tdd > 127))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_physical_cell_id->pci_tdd] should be in range [%d <= 127] !", p_son_rrm_utran_physical_cell_id->pci_tdd);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_utran_physical_cell_id->pci_tdd, "pci_tdd");
    *pp_buffer += sizeof(p_son_rrm_utran_physical_cell_id->pci_tdd);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_utran_neighbor_cell_len
(
    son_rrm_utran_neighbor_cell_t *p_son_rrm_utran_neighbor_cell
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_utran_neighbor_cell != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell->bitmask);

    /* Get length of IE */
    length += son_get_son_rrm_utran_geran_global_cell_id_len(&p_son_rrm_utran_neighbor_cell->cgi);

    /* Get length of IE */
    length += son_get_son_rrm_utran_physical_cell_id_len(&p_son_rrm_utran_neighbor_cell->pci);

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_NR_INFO_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_ACCESS_MODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell->access_mode);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_LAC_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_utran_neighbor_cell->lac);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_RAC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell->rac);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_CSG_ID_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_utran_neighbor_cell->csg_identity);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell->ps_ho_supported);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell->voip_capable);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_utran_neighbor_cell
(
    U8  **pp_buffer,
    son_rrm_utran_neighbor_cell_t *p_son_rrm_utran_neighbor_cell
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_utran_neighbor_cell != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_utran_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell->bitmask);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_utran_geran_global_cell_id(pp_buffer, &p_son_rrm_utran_neighbor_cell->cgi))
    {
        return SON_FAILURE;
    }

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_utran_physical_cell_id(pp_buffer, &p_son_rrm_utran_neighbor_cell->pci))
    {
        return SON_FAILURE;
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_NR_INFO_OFFSET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_utran_neighbor_cell->cell_specific_offset < -15) || (p_son_rrm_utran_neighbor_cell->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_utran_neighbor_cell->cell_specific_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_S8(*pp_buffer, &p_son_rrm_utran_neighbor_cell->cell_specific_offset, "cell_specific_offset");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_ACCESS_MODE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell->access_mode > 3))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->access_mode] should be in range [%d <= 3] !", p_son_rrm_utran_neighbor_cell->access_mode);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_utran_neighbor_cell->access_mode, "access_mode");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell->access_mode);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_LAC_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_utran_neighbor_cell->lac); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_utran_neighbor_cell->lac[loop], "lac[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_RAC_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_utran_neighbor_cell->rac, "rac");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell->rac);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_CSG_ID_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_utran_neighbor_cell->csg_identity); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_utran_neighbor_cell->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_PS_HO_SUPPORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell->ps_ho_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->ps_ho_supported] should be in range [%d <= 1] !", p_son_rrm_utran_neighbor_cell->ps_ho_supported);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_utran_neighbor_cell->ps_ho_supported, "ps_ho_supported");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell->ps_ho_supported);
    }

    /* Optional element */
    if(p_son_rrm_utran_neighbor_cell->bitmask & SON_RRM_UTRAN_VOIP_CAPABLE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell->voip_capable > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell->voip_capable] should be in range [%d <= 1] !", p_son_rrm_utran_neighbor_cell->voip_capable);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_utran_neighbor_cell->voip_capable, "voip_capable");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell->voip_capable);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_utran_neighbor_list_len
(
    son_rrm_utran_neighbor_list_t *p_son_rrm_utran_neighbor_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_utran_neighbor_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_list->uarfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_list->cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_list->cell_list_size > SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN] !", p_son_rrm_utran_neighbor_list->cell_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_list->cell_list_size; loop++)
        {
            length += son_get_son_rrm_utran_neighbor_cell_len(&p_son_rrm_utran_neighbor_list->cell_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_utran_neighbor_list
(
    U8  **pp_buffer,
    son_rrm_utran_neighbor_list_t *p_son_rrm_utran_neighbor_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_utran_neighbor_list != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_list->uarfcn > 16383))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_list->uarfcn] should be in range [%d <= 16383] !", p_son_rrm_utran_neighbor_list->uarfcn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_neighbor_list->uarfcn, "uarfcn");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_list->uarfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_list->cell_list_size > SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN] !", p_son_rrm_utran_neighbor_list->cell_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_neighbor_list->cell_list_size, "cell_list_size");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_list->cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_list->cell_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_utran_neighbor_cell(pp_buffer, &p_son_rrm_utran_neighbor_list->cell_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_utran_neighbor_cell_list_len
(
    son_rrm_utran_neighbor_cell_list_t *p_son_rrm_utran_neighbor_cell_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_utran_neighbor_cell_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_utran_neighbor_list_len(&p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_utran_neighbor_list_len(&p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_utran_neighbor_list_len(&p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_utran_neighbor_list_len(&p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_utran_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_utran_neighbor_cell_list_t *p_son_rrm_utran_neighbor_cell_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_utran_neighbor_cell_list != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size, "cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_utran_neighbor_list(pp_buffer, &p_son_rrm_utran_neighbor_cell_list->cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size, "cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_utran_neighbor_list(pp_buffer, &p_son_rrm_utran_neighbor_cell_list->cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size, "black_list_cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_utran_neighbor_list(pp_buffer, &p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size > SON_RRM_MAX_UTRAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_UTRAN_FREQ_ALLOWED] !", p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size, "black_list_cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_utran_neighbor_list(pp_buffer, &p_son_rrm_utran_neighbor_cell_list->black_list_cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_geran_physical_cell_id_len
(
    son_rrm_geran_physical_cell_id_t *p_son_rrm_geran_physical_cell_id
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_geran_physical_cell_id != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_physical_cell_id->ncc);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_physical_cell_id->bscc);

    return length;
}

static
son_return_et
son_compose_son_rrm_geran_physical_cell_id
(
    U8  **pp_buffer,
    son_rrm_geran_physical_cell_id_t *p_son_rrm_geran_physical_cell_id
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_geran_physical_cell_id != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_physical_cell_id->ncc > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_physical_cell_id->ncc] should be in range [%d <= 7] !", p_son_rrm_geran_physical_cell_id->ncc);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_geran_physical_cell_id->ncc, "ncc");
    *pp_buffer += sizeof(p_son_rrm_geran_physical_cell_id->ncc);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_physical_cell_id->bscc > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_physical_cell_id->bscc] should be in range [%d <= 7] !", p_son_rrm_geran_physical_cell_id->bscc);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_geran_physical_cell_id->bscc, "bscc");
    *pp_buffer += sizeof(p_son_rrm_geran_physical_cell_id->bscc);

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_geran_neighbor_cell_len
(
    son_rrm_geran_neighbor_cell_t *p_son_rrm_geran_neighbor_cell
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_geran_neighbor_cell != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell->bitmask);

    /* Get length of IE */
    length += son_get_son_rrm_utran_geran_global_cell_id_len(&p_son_rrm_geran_neighbor_cell->cgi);

    /* Get length of IE */
    length += son_get_son_rrm_geran_physical_cell_id_len(&p_son_rrm_geran_neighbor_cell->pci);

    /* Optional element */
    if(p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_NR_INFO_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_NR_DTM_SUPPORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell->dtm_supported);
    }

    /* Optional element */
    if(p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_RAC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell->rac);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_geran_neighbor_cell
(
    U8  **pp_buffer,
    son_rrm_geran_neighbor_cell_t *p_son_rrm_geran_neighbor_cell
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_geran_neighbor_cell != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_geran_neighbor_cell->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell->bitmask);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_utran_geran_global_cell_id(pp_buffer, &p_son_rrm_geran_neighbor_cell->cgi))
    {
        return SON_FAILURE;
    }

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_geran_physical_cell_id(pp_buffer, &p_son_rrm_geran_neighbor_cell->pci))
    {
        return SON_FAILURE;
    }

    /* Optional element */
    if(p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_NR_INFO_OFFSET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_geran_neighbor_cell->cell_specific_offset < -15) || (p_son_rrm_geran_neighbor_cell->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_geran_neighbor_cell->cell_specific_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_S8(*pp_buffer, &p_son_rrm_geran_neighbor_cell->cell_specific_offset, "cell_specific_offset");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_NR_DTM_SUPPORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell->dtm_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell->dtm_supported] should be in range [%d <= 1] !", p_son_rrm_geran_neighbor_cell->dtm_supported);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_geran_neighbor_cell->dtm_supported, "dtm_supported");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell->dtm_supported);
    }

    /* Optional element */
    if(p_son_rrm_geran_neighbor_cell->bitmask & SON_RRM_GERAN_RAC_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_geran_neighbor_cell->rac, "rac");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell->rac);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_geran_neighbor_list_len
(
    son_rrm_geran_neighbor_list_t *p_son_rrm_geran_neighbor_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_geran_neighbor_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_list->arfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_list->band_ind);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_list->cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->cell_list_size > SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN] !", p_son_rrm_geran_neighbor_list->cell_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_list->cell_list_size; loop++)
        {
            length += son_get_son_rrm_geran_neighbor_cell_len(&p_son_rrm_geran_neighbor_list->cell_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_geran_neighbor_list
(
    U8  **pp_buffer,
    son_rrm_geran_neighbor_list_t *p_son_rrm_geran_neighbor_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_geran_neighbor_list != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->arfcn > 1023))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->arfcn] should be in range [%d <= 1023] !", p_son_rrm_geran_neighbor_list->arfcn);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_geran_neighbor_list->arfcn, "arfcn");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_list->arfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->band_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->band_ind] should be in range [%d <= 1] !", p_son_rrm_geran_neighbor_list->band_ind);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_geran_neighbor_list->band_ind, "band_ind");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_list->band_ind);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_list->cell_list_size > SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_list->cell_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_UTRAN_NBRS_PER_EARFCN] !", p_son_rrm_geran_neighbor_list->cell_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_geran_neighbor_list->cell_list_size, "cell_list_size");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_list->cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_list->cell_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_geran_neighbor_cell(pp_buffer, &p_son_rrm_geran_neighbor_list->cell_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_geran_neighbor_cell_list_len
(
    son_rrm_geran_neighbor_cell_list_t *p_son_rrm_geran_neighbor_cell_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_geran_neighbor_cell_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_geran_neighbor_list_len(&p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_geran_neighbor_list_len(&p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_geran_neighbor_list_len(&p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_geran_neighbor_list_len(&p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_geran_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_geran_neighbor_cell_list_t *p_son_rrm_geran_neighbor_cell_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_geran_neighbor_cell_list != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size, "cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_geran_neighbor_list(pp_buffer, &p_son_rrm_geran_neighbor_cell_list->cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size, "cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_geran_neighbor_list(pp_buffer, &p_son_rrm_geran_neighbor_cell_list->cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size, "black_list_cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_geran_neighbor_list(pp_buffer, &p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size > SON_RRM_MAX_GERAN_FREQ_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_GERAN_FREQ_ALLOWED] !", p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size, "black_list_cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_geran_neighbor_list(pp_buffer, &p_son_rrm_geran_neighbor_cell_list->black_list_cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_global_cell_id_len
(
    son_rrm_cdma_global_cell_id_t *p_son_rrm_cdma_global_cell_id
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_global_cell_id != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_global_cell_id->bitmask);

    /* Optional element */
    if(p_son_rrm_cdma_global_cell_id->bitmask & SON_RRM_CGI_1X_RTT_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_cdma_global_cell_id->cell_global_id_1X_RTT);
    }

    /* Optional element */
    if(p_son_rrm_cdma_global_cell_id->bitmask & SON_RRM_CGI_HRPD_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_cdma_global_cell_id->cell_global_id_HRPD);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_global_cell_id
(
    U8  **pp_buffer,
    son_rrm_cdma_global_cell_id_t *p_son_rrm_cdma_global_cell_id
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_global_cell_id != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_cdma_global_cell_id->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_cdma_global_cell_id->bitmask);

    /* Optional element */
    if(p_son_rrm_cdma_global_cell_id->bitmask & SON_RRM_CGI_1X_RTT_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_cdma_global_cell_id->cell_global_id_1X_RTT); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_global_cell_id->cell_global_id_1X_RTT[loop], "cell_global_id_1X_RTT[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_son_rrm_cdma_global_cell_id->bitmask & SON_RRM_CGI_HRPD_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_cdma_global_cell_id->cell_global_id_HRPD); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_global_cell_id->cell_global_id_HRPD[loop], "cell_global_id_HRPD[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_cdma_pz_hyst_params_included_len
(
    son_cdma_pz_hyst_params_included_t *p_son_cdma_pz_hyst_params_included
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_cdma_pz_hyst_params_included != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->bitmask);

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_ENABLED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_enabled);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_INFO_INCL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_act_timer);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_MUL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_EXP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_LIST_LEN_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_list_len);
    }

    return length;
}

static
son_return_et
son_compose_son_cdma_pz_hyst_params_included
(
    U8  **pp_buffer,
    son_cdma_pz_hyst_params_included_t *p_son_cdma_pz_hyst_params_included
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_cdma_pz_hyst_params_included != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_cdma_pz_hyst_params_included->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->bitmask);

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_ENABLED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_enabled > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_enabled] should be in range [%d <= 1] !", p_son_cdma_pz_hyst_params_included->pz_hyst_enabled);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_pz_hyst_params_included->pz_hyst_enabled, "pz_hyst_enabled");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_enabled);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_INFO_INCL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl] should be in range [%d <= 1] !", p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl, "pz_hyst_info_incl");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_info_incl);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_pz_hyst_params_included->pz_hyst_act_timer, "pz_hyst_act_timer");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_act_timer);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_MUL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul > 7))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul] should be in range [%d <= 7] !", p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul, "pz_hyst_timer_mul");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_timer_mul);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_TIMER_EXP_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp] should be in range [%d <= 31] !", p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp, "pz_hyst_timer_exp");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_timer_exp);
    }

    /* Optional element */
    if(p_son_cdma_pz_hyst_params_included->bitmask & SON_RRM_CDMA_PZ_HYST_LIST_LEN_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_cdma_pz_hyst_params_included->pz_hyst_list_len > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_cdma_pz_hyst_params_included->pz_hyst_list_len] should be in range [%d <= 1] !", p_son_cdma_pz_hyst_params_included->pz_hyst_list_len);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_pz_hyst_params_included->pz_hyst_list_len, "pz_hyst_list_len");
    *pp_buffer += sizeof(p_son_cdma_pz_hyst_params_included->pz_hyst_list_len);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_cdma_fpc_fch_included_len
(
    son_cdma_fpc_fch_included_t *p_son_cdma_fpc_fch_included
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_cdma_fpc_fch_included != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_fpc_fch_included->bitmask);

    /* Optional element */
    if(p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC3_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc3);
    }

    /* Optional element */
    if(p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC4_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc4);
    }

    /* Optional element */
    if(p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC5_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc5);
    }

    return length;
}

static
son_return_et
son_compose_son_cdma_fpc_fch_included
(
    U8  **pp_buffer,
    son_cdma_fpc_fch_included_t *p_son_cdma_fpc_fch_included
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_cdma_fpc_fch_included != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_cdma_fpc_fch_included->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_cdma_fpc_fch_included->bitmask);

    /* Optional element */
    if(p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC3_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc3, "fpc_fch_init_setpt_rc3");
    *pp_buffer += sizeof(p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc3);
    }

    /* Optional element */
    if(p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC4_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc4, "fpc_fch_init_setpt_rc4");
    *pp_buffer += sizeof(p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc4);
    }

    /* Optional element */
    if(p_son_cdma_fpc_fch_included->bitmask & SON_RRM_CDMA_FPC_FCH_INCL_RC5_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc5, "fpc_fch_init_setpt_rc5");
    *pp_buffer += sizeof(p_son_cdma_fpc_fch_included->fpc_fch_init_setpt_rc5);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_cell_specific_params_len
(
    son_rrm_cdma_cell_specific_params_t *p_son_rrm_cdma_cell_specific_params
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_cell_specific_params != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->bitmask);

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_cdma_pz_hyst_params_included_len(&p_son_rrm_cdma_cell_specific_params->pz_hyst_params_included);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_P_REV_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->p_rev);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MIN_P_REV_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->min_p_rev);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ENCRYPT_MODE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->encrypt_mode);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->encrypt_suported);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SYG_ENCRYPT_SUPPORTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->syg_encrypt_suported);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->msg_integrity_sup);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->sig_integrity_sup);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->band_class_info_req);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_BAND_CLASS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->band_class);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ALT_BAND_CLASS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->alt_band_class);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_TKZ_MODE_SUP_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->tkz_mode_supported);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_TKZ_ID_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->tkz_id);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_FPC_FCH_INCLUDED)
    {

    /* Get length of IE */
    length += son_get_son_cdma_fpc_fch_included_len(&p_son_rrm_cdma_cell_specific_params->fpc_fch_included);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_T_ADD)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->t_add);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_PILOT_INC_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_cell_specific_params->pilot_inc);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_cell_specific_params
(
    U8  **pp_buffer,
    son_rrm_cdma_cell_specific_params_t *p_son_rrm_cdma_cell_specific_params
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_cell_specific_params != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->bitmask);

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_PZ_HYST_PARAMS_INCLUDED_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_cdma_pz_hyst_params_included(pp_buffer, &p_son_rrm_cdma_cell_specific_params->pz_hyst_params_included))
    {
        return SON_FAILURE;
    }
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_P_REV_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->p_rev, "p_rev");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->p_rev);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MIN_P_REV_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->min_p_rev, "min_p_rev");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->min_p_rev);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_NEG_SLOT_CYCLE_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup, "neg_slot_cycle_index_sup");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->neg_slot_cycle_index_sup);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ENCRYPT_MODE_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->encrypt_mode, "encrypt_mode");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->encrypt_mode);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ENCRYPT_SUPPORTED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->encrypt_suported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->encrypt_suported] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->encrypt_suported);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->encrypt_suported, "encrypt_suported");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->encrypt_suported);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SYG_ENCRYPT_SUPPORTED_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->syg_encrypt_suported, "syg_encrypt_suported");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->syg_encrypt_suported);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MSG_INT_SUPPORTED_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->msg_integrity_sup > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->msg_integrity_sup] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->msg_integrity_sup);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->msg_integrity_sup, "msg_integrity_sup");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->msg_integrity_sup);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_INCL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl, "sig_integrity_sup_incl");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->sig_integrity_sup_incl);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_SIG_INT_SUPPORTED_PRESENT)
    {

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->sig_integrity_sup, "sig_integrity_sup");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->sig_integrity_sup);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_MS_INIT_POS_LOC_SUP_IND_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind, "ms_init_pos_loc_sup_ind");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->ms_init_pos_loc_sup_ind);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_BAND_CLASS_INFO_REQ_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->band_class_info_req > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->band_class_info_req] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->band_class_info_req);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->band_class_info_req, "band_class_info_req");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->band_class_info_req);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_BAND_CLASS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_cell_specific_params->band_class);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->band_class, "band_class");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->band_class);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_ALT_BAND_CLASS_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->alt_band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->alt_band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_cell_specific_params->alt_band_class);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->alt_band_class, "alt_band_class");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->alt_band_class);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_TKZ_MODE_SUP_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->tkz_mode_supported > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->tkz_mode_supported] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->tkz_mode_supported);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->tkz_mode_supported, "tkz_mode_supported");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->tkz_mode_supported);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_TKZ_ID_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->tkz_id > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->tkz_id] should be in range [%d <= 1] !", p_son_rrm_cdma_cell_specific_params->tkz_id);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->tkz_id, "tkz_id");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->tkz_id);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_FPC_FCH_INCLUDED)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_cdma_fpc_fch_included(pp_buffer, &p_son_rrm_cdma_cell_specific_params->fpc_fch_included))
    {
        return SON_FAILURE;
    }
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_T_ADD)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->t_add > 63))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->t_add] should be in range [%d <= 63] !", p_son_rrm_cdma_cell_specific_params->t_add);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->t_add, "t_add");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->t_add);
    }

    /* Optional element */
    if(p_son_rrm_cdma_cell_specific_params->bitmask & SON_RRM_CDMA_CSP_PILOT_INC_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_cell_specific_params->pilot_inc > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_cell_specific_params->pilot_inc] should be in range [%d <= 15] !", p_son_rrm_cdma_cell_specific_params->pilot_inc);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_cdma_cell_specific_params->pilot_inc, "pilot_inc");
    *pp_buffer += sizeof(p_son_rrm_cdma_cell_specific_params->pilot_inc);
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_xrtt_len
(
    son_rrm_cdma_neighbor_cell_xrtt_t *p_son_rrm_cdma_neighbor_cell_xrtt
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_xrtt != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->pci);

    /* Get length of IE */
    length += son_get_son_rrm_cdma_global_cell_id_len(&p_son_rrm_cdma_neighbor_cell_xrtt->cgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->arfcn);

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_xrtt->bitmask & SON_RRM_CDMA_XRTT_CELL_SPECIFIC_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_xrtt->bitmask & SON_RRM_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_rrm_cdma_cell_specific_params_len(&p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_params);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_xrtt
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_xrtt_t *p_son_rrm_cdma_neighbor_cell_xrtt
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_xrtt != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_xrtt->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->bitmask);

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_xrtt->pci, "pci");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->pci);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cdma_global_cell_id(pp_buffer, &p_son_rrm_cdma_neighbor_cell_xrtt->cgi))
    {
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_xrtt->arfcn, "arfcn");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->arfcn);

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_xrtt->bitmask & SON_RRM_CDMA_XRTT_CELL_SPECIFIC_OFFSET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset < -15) || (p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_S8(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset, "cell_specific_offset");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_xrtt->bitmask & SON_RRM_CDMA_XRTT_CELL_SPECIFIC_PARAMS_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cdma_cell_specific_params(pp_buffer, &p_son_rrm_cdma_neighbor_cell_xrtt->cell_specific_params))
    {
        return SON_FAILURE;
    }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_neighbor_list_xrtt_len
(
    son_rrm_cdma_neighbor_list_xrtt_t *p_son_rrm_cdma_neighbor_list_xrtt
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_neighbor_list_xrtt != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_list_xrtt->band_class);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size > SON_RRM_CDMA_XRTT_NBR_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size] should be in range [%d <= SON_RRM_CDMA_XRTT_NBR_SIZE] !", p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_cell_xrtt_len(&p_son_rrm_cdma_neighbor_list_xrtt->cell_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_neighbor_list_xrtt
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_list_xrtt_t *p_son_rrm_cdma_neighbor_list_xrtt
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_neighbor_list_xrtt != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_xrtt->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_xrtt->band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_neighbor_list_xrtt->band_class);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_list_xrtt->band_class, "band_class");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_list_xrtt->band_class);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size > SON_RRM_CDMA_XRTT_NBR_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size] should be in range [%d <= SON_RRM_CDMA_XRTT_NBR_SIZE] !", p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size, "cell_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_list_xrtt->cell_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_cell_xrtt(pp_buffer, &p_son_rrm_cdma_neighbor_list_xrtt->cell_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_list_xrtt_len
(
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_son_rrm_cdma_neighbor_cell_list_xrtt
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_list_xrtt != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_xrtt_len(&p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_xrtt_len(&p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_xrtt_len(&p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_xrtt_len(&p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_list_xrtt
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_list_xrtt_t *p_son_rrm_cdma_neighbor_cell_list_xrtt
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_list_xrtt != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size, "cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_xrtt(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size, "cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_xrtt(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size, "black_list_cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_xrtt(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size, "black_list_cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_xrtt(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_xrtt->black_list_cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_hrpd_len
(
    son_rrm_cdma_neighbor_cell_hrpd_t *p_son_rrm_cdma_neighbor_cell_hrpd
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_hrpd != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->pci);

    /* Get length of IE */
    length += son_get_son_rrm_cdma_global_cell_id_len(&p_son_rrm_cdma_neighbor_cell_hrpd->cgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->arfcn);

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_hrpd->bitmask & SON_RRM_CDMA_HRPD_CELL_SPECIFIC_OFFSET_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_hrpd->bitmask & SON_RRM_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_rrm_cdma_cell_specific_params_len(&p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_params);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_hrpd
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_hrpd_t *p_son_rrm_cdma_neighbor_cell_hrpd
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_hrpd != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_hrpd->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->bitmask);

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_hrpd->pci, "pci");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->pci);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cdma_global_cell_id(pp_buffer, &p_son_rrm_cdma_neighbor_cell_hrpd->cgi))
    {
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_hrpd->arfcn, "arfcn");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->arfcn);

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_hrpd->bitmask & SON_RRM_CDMA_HRPD_CELL_SPECIFIC_OFFSET_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset < -15) || (p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset > 15))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset] should be in range [-15 <= %d <= 15] !", p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_S8(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset, "cell_specific_offset");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_offset);
    }

    /* Optional element */
    if(p_son_rrm_cdma_neighbor_cell_hrpd->bitmask & SON_RRM_CDMA_HRPD_CELL_SPECIFIC_PARAMS_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cdma_cell_specific_params(pp_buffer, &p_son_rrm_cdma_neighbor_cell_hrpd->cell_specific_params))
    {
        return SON_FAILURE;
    }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_neighbor_list_hrpd_len
(
    son_rrm_cdma_neighbor_list_hrpd_t *p_son_rrm_cdma_neighbor_list_hrpd
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_neighbor_list_hrpd != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_list_hrpd->band_class);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size > SON_RRM_CDMA_HRPD_NBR_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size] should be in range [%d <= SON_RRM_CDMA_HRPD_NBR_SIZE] !", p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_cell_hrpd_len(&p_son_rrm_cdma_neighbor_list_hrpd->cell_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_neighbor_list_hrpd
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_list_hrpd_t *p_son_rrm_cdma_neighbor_list_hrpd
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_neighbor_list_hrpd != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_hrpd->band_class > 31))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_hrpd->band_class] should be in range [%d <= 31] !", p_son_rrm_cdma_neighbor_list_hrpd->band_class);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_list_hrpd->band_class, "band_class");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_list_hrpd->band_class);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size > SON_RRM_CDMA_HRPD_NBR_SIZE))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size] should be in range [%d <= SON_RRM_CDMA_HRPD_NBR_SIZE] !", p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size, "cell_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_list_hrpd->cell_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_cell_hrpd(pp_buffer, &p_son_rrm_cdma_neighbor_list_hrpd->cell_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_cdma_neighbor_cell_list_hrpd_len
(
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_son_rrm_cdma_neighbor_cell_list_hrpd
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_list_hrpd != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_hrpd_len(&p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_hrpd_len(&p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_hrpd_len(&p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size; loop++)
        {
            length += son_get_son_rrm_cdma_neighbor_list_hrpd_len(&p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_cdma_neighbor_cell_list_hrpd
(
    U8  **pp_buffer,
    son_rrm_cdma_neighbor_cell_list_hrpd_t *p_son_rrm_cdma_neighbor_cell_list_hrpd
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_cdma_neighbor_cell_list_hrpd != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size, "cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_hrpd(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size, "cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_hrpd(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size, "black_list_cells_to_add_mod_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_hrpd(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_add_mod_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size > SON_RRM_MAX_CDMA_BAND_ALLOWED))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size] should be in range [%d <= SON_RRM_MAX_CDMA_BAND_ALLOWED] !", p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size, "black_list_cells_to_remove_list_size");
    *pp_buffer += sizeof(p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_list_hrpd(pp_buffer, &p_son_rrm_cdma_neighbor_cell_list_hrpd->black_list_cells_to_remove_list[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_inter_rat_neighbor_cell_list_len
(
    son_rrm_inter_rat_neighbor_cell_list_t *p_son_rrm_inter_rat_neighbor_cell_list
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_inter_rat_neighbor_cell_list != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_inter_rat_neighbor_cell_list->bitmask);

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_UTRAN_NR_LIST_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_rrm_utran_neighbor_cell_list_len(&p_son_rrm_inter_rat_neighbor_cell_list->utran_neighbor_list);
    }

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_GERAN_NR_LIST_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_rrm_geran_neighbor_cell_list_len(&p_son_rrm_inter_rat_neighbor_cell_list->geran_neighbor_list);
    }

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_CDMA_NR_LIST_XRTT_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_rrm_cdma_neighbor_cell_list_xrtt_len(&p_son_rrm_inter_rat_neighbor_cell_list->cdma_neighbor_list_xrtt);
    }

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_CDMA_NR_LIST_HRPD_PRESENT)
    {

    /* Get length of IE */
    length += son_get_son_rrm_cdma_neighbor_cell_list_hrpd_len(&p_son_rrm_inter_rat_neighbor_cell_list->cdma_neighbor_list_hrpd);
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_inter_rat_neighbor_cell_list
(
    U8  **pp_buffer,
    son_rrm_inter_rat_neighbor_cell_list_t *p_son_rrm_inter_rat_neighbor_cell_list
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_inter_rat_neighbor_cell_list != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_inter_rat_neighbor_cell_list->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_rrm_inter_rat_neighbor_cell_list->bitmask);

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_UTRAN_NR_LIST_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_utran_neighbor_cell_list(pp_buffer, &p_son_rrm_inter_rat_neighbor_cell_list->utran_neighbor_list))
    {
        return SON_FAILURE;
    }
    }

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_GERAN_NR_LIST_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_geran_neighbor_cell_list(pp_buffer, &p_son_rrm_inter_rat_neighbor_cell_list->geran_neighbor_list))
    {
        return SON_FAILURE;
    }
    }

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_CDMA_NR_LIST_XRTT_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_cell_list_xrtt(pp_buffer, &p_son_rrm_inter_rat_neighbor_cell_list->cdma_neighbor_list_xrtt))
    {
        return SON_FAILURE;
    }
    }

    /* Optional element */
    if(p_son_rrm_inter_rat_neighbor_cell_list->bitmask & SON_RRM_CDMA_NR_LIST_HRPD_PRESENT)
    {

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cdma_neighbor_cell_list_hrpd(pp_buffer, &p_son_rrm_inter_rat_neighbor_cell_list->cdma_neighbor_list_hrpd))
    {
        return SON_FAILURE;
    }
    }

    return SON_SUCCESS;
}

son_length_t
son_get_son_apps_rrm_carrier_freq_and_dl_power_params_req_len
(
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_req
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_apps_rrm_carrier_freq_and_dl_power_params_req != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn * sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->earfcn[0]));

    return length;
}

son_return_et
son_compose_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
    U8  **pp_buffer,
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_req
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_apps_rrm_carrier_freq_and_dl_power_params_req != SON_PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth < 6) || (p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth > 100))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth] should be in range [6 <= %d <= 100] !", p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth, "meas_bandwidth");
    *pp_buffer += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->meas_bandwidth);

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn, "num_earfcn");
    *pp_buffer += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_apps_rrm_carrier_freq_and_dl_power_params_req->num_earfcn; loop++)
        {
            son_cp_pack_U32(*pp_buffer, &p_son_apps_rrm_carrier_freq_and_dl_power_params_req->earfcn[loop], "earfcn[]");
            *pp_buffer += sizeof(U32);
        }
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_rrm_send_son_apps_rrm_carrier_freq_and_dl_power_params_req
*
*   DESCRIPTION:
*       This function constructs and sends SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_rrm_send_son_apps_rrm_carrier_freq_and_dl_power_params_req
(
    son_apps_rrm_carrier_freq_and_dl_power_params_req_t  *p_son_apps_rrm_carrier_freq_and_dl_power_params_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rrm_msg;

    SON_ASSERT(p_son_apps_rrm_carrier_freq_and_dl_power_params_req != SON_PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = son_get_son_apps_rrm_carrier_freq_and_dl_power_params_req_len(p_son_apps_rrm_carrier_freq_and_dl_power_params_req);
   if(msg_length == SON_FAILURE)
    {
        return SON_FAILURE;
    }

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_INFO, "Src(%u)->Dst(%u):SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_rrm_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_rrm_msg, SON_VERSION_ID, src_module_id,
        dest_module_id, SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_rrm_msg = p_rrm_msg + SON_API_HEADER_SIZE;
    son_construct_interface_api_header(p_rrm_msg, transaction_id,  (son_module_id_et)src_module_id,
        (son_module_id_et)dest_module_id, SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ, msg_length, cell_index);

    /* Fill SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ message */
    p_rrm_msg = p_rrm_msg + EXT_MSG_API_HDR_SIZE;
    if (SON_FAILURE == son_compose_son_apps_rrm_carrier_freq_and_dl_power_params_req(&p_rrm_msg, p_son_apps_rrm_carrier_freq_and_dl_power_params_req))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    if((p_rrm_msg - msg_api_length) != p_msg)
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }
    /* Send message to */
    son_send_msg(p_msg, dest_module_id);

    return SON_SUCCESS;
}

son_length_t
son_get_son_apps_rrm_carrier_freq_and_dl_power_params_resp_len
(
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->error_code);

    /* Get length of IE */
    length += son_get_son_rrm_carrier_freq_and_dl_power_params_resp_len(&p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->carrier_freq_and_dl_power_params_resp);

    return length;
}

son_return_et
son_compose_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
    U8  **pp_buffer,
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result > 1))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result] should be in range [%d <= 1] !", p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result, "result");
    *pp_buffer += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->result);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->error_code, "error_code");
    *pp_buffer += sizeof(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->error_code);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_carrier_freq_and_dl_power_params_resp(pp_buffer, &p_son_apps_rrm_carrier_freq_and_dl_power_params_resp->carrier_freq_and_dl_power_params_resp))
    {
        return SON_FAILURE;
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_rrm_send_son_apps_rrm_carrier_freq_and_dl_power_params_resp
*
*   DESCRIPTION:
*       This function constructs and sends SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_rrm_send_son_apps_rrm_carrier_freq_and_dl_power_params_resp
(
    son_apps_rrm_carrier_freq_and_dl_power_params_resp_t  *p_son_apps_rrm_carrier_freq_and_dl_power_params_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rrm_msg;

    SON_ASSERT(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp != SON_PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = son_get_son_apps_rrm_carrier_freq_and_dl_power_params_resp_len(p_son_apps_rrm_carrier_freq_and_dl_power_params_resp);
   if(msg_length == SON_FAILURE)
    {
        return SON_FAILURE;
    }

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_INFO, "Src(%u)->Dst(%u):SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP", src_module_id, dest_module_id);

    msg_api_length = msg_length + SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_rrm_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_rrm_msg, SON_VERSION_ID, src_module_id,
        dest_module_id, SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_rrm_msg = p_rrm_msg + SON_API_HEADER_SIZE;
    son_construct_interface_api_header(p_rrm_msg, transaction_id,  (son_module_id_et)src_module_id,
        (son_module_id_et)dest_module_id, SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP, msg_length, cell_index);

    /* Fill SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP message */
    p_rrm_msg = p_rrm_msg + EXT_MSG_API_HDR_SIZE;
    if (SON_FAILURE == son_compose_son_apps_rrm_carrier_freq_and_dl_power_params_resp(&p_rrm_msg, p_son_apps_rrm_carrier_freq_and_dl_power_params_resp))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    if((p_rrm_msg - msg_api_length) != p_msg)
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }
    /* Send message to */
    son_send_msg(p_msg, dest_module_id);

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_neighbor_cell_signal_power_len
(
    son_rrm_neighbor_cell_signal_power_t *p_son_rrm_neighbor_cell_signal_power
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_neighbor_cell_signal_power != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_neighbor_cell_signal_power->pci_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_neighbor_cell_signal_power->rsrq);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_neighbor_cell_signal_power->rsrp);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_neighbor_cell_signal_power->reference_signal_power);

    return length;
}

static
son_return_et
son_compose_son_rrm_neighbor_cell_signal_power
(
    U8  **pp_buffer,
    son_rrm_neighbor_cell_signal_power_t *p_son_rrm_neighbor_cell_signal_power
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_neighbor_cell_signal_power != SON_PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_neighbor_cell_signal_power->pci_id > 503))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_neighbor_cell_signal_power->pci_id] should be in range [%d <= 503] !", p_son_rrm_neighbor_cell_signal_power->pci_id);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_neighbor_cell_signal_power->pci_id, "pci_id");
    *pp_buffer += sizeof(p_son_rrm_neighbor_cell_signal_power->pci_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_neighbor_cell_signal_power->rsrq > 34))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_neighbor_cell_signal_power->rsrq] should be in range [%d <= 34] !", p_son_rrm_neighbor_cell_signal_power->rsrq);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_neighbor_cell_signal_power->rsrq, "rsrq");
    *pp_buffer += sizeof(p_son_rrm_neighbor_cell_signal_power->rsrq);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_neighbor_cell_signal_power->rsrp > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_neighbor_cell_signal_power->rsrp] should be in range [%d <= 128] !", p_son_rrm_neighbor_cell_signal_power->rsrp);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_neighbor_cell_signal_power->rsrp, "rsrp");
    *pp_buffer += sizeof(p_son_rrm_neighbor_cell_signal_power->rsrp);

    /* Compose parameter of basic type */
    son_cp_pack_S8(*pp_buffer, &p_son_rrm_neighbor_cell_signal_power->reference_signal_power, "reference_signal_power");
    *pp_buffer += sizeof(p_son_rrm_neighbor_cell_signal_power->reference_signal_power);

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_carrier_freq_and_dl_power_params_resp_len
(
    son_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_rrm_carrier_freq_and_dl_power_params_resp
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_carrier_freq_and_dl_power_params_resp != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_carrier_freq_and_dl_power_params_resp->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size > SON_MAX_NO_CELLS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size] should be in range [%d <= SON_MAX_NO_CELLS_PER_EARFCN] !", p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size);
        return SON_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size; loop++)
        {
            length += son_get_son_rrm_neighbor_cell_signal_power_len(&p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_signal_power[loop]);
        }
    }

    return length;
}

static
son_return_et
son_compose_son_rrm_carrier_freq_and_dl_power_params_resp
(
    U8  **pp_buffer,
    son_rrm_carrier_freq_and_dl_power_params_resp_t *p_son_rrm_carrier_freq_and_dl_power_params_resp
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_carrier_freq_and_dl_power_params_resp != SON_PNULL);

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_carrier_freq_and_dl_power_params_resp->earfcn, "earfcn");
    *pp_buffer += sizeof(p_son_rrm_carrier_freq_and_dl_power_params_resp->earfcn);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi > 128))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi] should be in range [%d <= 128] !", p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U8(*pp_buffer, &p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi, "rssi");
    *pp_buffer += sizeof(p_son_rrm_carrier_freq_and_dl_power_params_resp->rssi);

    /* Check for correct range [H - higher boundary] */
    if ((p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size > SON_MAX_NO_CELLS_PER_EARFCN))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size] should be in range [%d <= SON_MAX_NO_CELLS_PER_EARFCN] !", p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U16(*pp_buffer, &p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size, "neighbor_cell_list_size");
    *pp_buffer += sizeof(p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        son_counter_t loop;
        for (loop = 0; loop < p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_list_size; loop++)
        {
            if (SON_FAILURE == son_compose_son_rrm_neighbor_cell_signal_power(pp_buffer, &p_son_rrm_carrier_freq_and_dl_power_params_resp->neighbor_cell_signal_power[loop]))
            {
                return SON_FAILURE;
            }
        }
    }

    return SON_SUCCESS;
}

son_length_t
son_get_son_rrm_ttt_update_ind_len
(
    son_rrm_ttt_update_ind_t *p_son_rrm_ttt_update_ind
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_ttt_update_ind != SON_PNULL);


    /* Get length of IE */
    length += son_get_son_rrm_intra_rat_global_cell_id_len(&p_son_rrm_ttt_update_ind->srv_cgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_ttt_update_ind->updated_ttt);

    return length;
}

son_return_et
son_compose_son_rrm_ttt_update_ind
(
    U8  **pp_buffer,
    son_rrm_ttt_update_ind_t *p_son_rrm_ttt_update_ind
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_ttt_update_ind != SON_PNULL);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_intra_rat_global_cell_id(pp_buffer, &p_son_rrm_ttt_update_ind->srv_cgi))
    {
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_ttt_update_ind->updated_ttt, "updated_ttt");
    *pp_buffer += sizeof(p_son_rrm_ttt_update_ind->updated_ttt);

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_rrm_send_son_rrm_ttt_update_ind
*
*   DESCRIPTION:
*       This function constructs and sends SON_RRM_TTT_UPDATE_IND message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_rrm_send_son_rrm_ttt_update_ind
(
    son_rrm_ttt_update_ind_t  *p_son_rrm_ttt_update_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rrm_msg;

    SON_ASSERT(p_son_rrm_ttt_update_ind != SON_PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = son_get_son_rrm_ttt_update_ind_len(p_son_rrm_ttt_update_ind);
   if(msg_length == SON_FAILURE)
    {
        return SON_FAILURE;
    }

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_INFO, "Src(%u)->Dst(%u):SON_RRM_TTT_UPDATE_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_rrm_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_rrm_msg, SON_VERSION_ID, src_module_id,
        dest_module_id, SON_RRM_TTT_UPDATE_IND,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_rrm_msg = p_rrm_msg + SON_API_HEADER_SIZE;
    son_construct_interface_api_header(p_rrm_msg, transaction_id,  (son_module_id_et)src_module_id,
        (son_module_id_et)dest_module_id, SON_RRM_TTT_UPDATE_IND, msg_length, cell_index);

    /* Fill SON_RRM_TTT_UPDATE_IND message */
    p_rrm_msg = p_rrm_msg + EXT_MSG_API_HDR_SIZE;
    if (SON_FAILURE == son_compose_son_rrm_ttt_update_ind(&p_rrm_msg, p_son_rrm_ttt_update_ind))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    if((p_rrm_msg - msg_api_length) != p_msg)
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }
    /* Send message to */
    son_send_msg(p_msg, dest_module_id);

    return SON_SUCCESS;
}

son_length_t
son_get_son_rrm_stop_tnl_discovery_ind_len
(
    son_rrm_stop_tnl_discovery_ind_t *p_son_rrm_stop_tnl_discovery_ind
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_stop_tnl_discovery_ind != SON_PNULL);


    /* Get length of IE */
    length += son_get_son_rrm_global_enb_id_len(&p_son_rrm_stop_tnl_discovery_ind->target_enb_id);

    return length;
}

son_return_et
son_compose_son_rrm_stop_tnl_discovery_ind
(
    U8  **pp_buffer,
    son_rrm_stop_tnl_discovery_ind_t *p_son_rrm_stop_tnl_discovery_ind
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_stop_tnl_discovery_ind != SON_PNULL);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_global_enb_id(pp_buffer, &p_son_rrm_stop_tnl_discovery_ind->target_enb_id))
    {
        return SON_FAILURE;
    }

    return SON_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: son_rrm_send_son_rrm_stop_tnl_discovery_ind
*
*   DESCRIPTION:
*       This function constructs and sends SON_RRM_STOP_TNL_DISCOVERY_IND message
*
*   RETURNS:
*       SON_FAILURE     - Indicates failed message processing
*       SON_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
son_return_et
son_rrm_send_son_rrm_stop_tnl_discovery_ind
(
    son_rrm_stop_tnl_discovery_ind_t  *p_son_rrm_stop_tnl_discovery_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rrm_msg;

    SON_ASSERT(p_son_rrm_stop_tnl_discovery_ind != SON_PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = son_get_son_rrm_stop_tnl_discovery_ind_len(p_son_rrm_stop_tnl_discovery_ind);
   if(msg_length == SON_FAILURE)
    {
        return SON_FAILURE;
    }

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_INFO, "Src(%u)->Dst(%u):SON_RRM_STOP_TNL_DISCOVERY_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_ERROR,
                "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, 0, msg_api_length);
    p_rrm_msg = p_msg;

    /* Fill CSPL header */
    
    son_construct_cspl_header(p_rrm_msg, SON_VERSION_ID, src_module_id,
        dest_module_id, SON_RRM_STOP_TNL_DISCOVERY_IND,SON_NULL, msg_api_length);
     
    /* Fill interface header */
     p_rrm_msg = p_rrm_msg + SON_API_HEADER_SIZE;
    son_construct_interface_api_header(p_rrm_msg, transaction_id,  (son_module_id_et)src_module_id,
        (son_module_id_et)dest_module_id, SON_RRM_STOP_TNL_DISCOVERY_IND, msg_length, cell_index);

    /* Fill SON_RRM_STOP_TNL_DISCOVERY_IND message */
    p_rrm_msg = p_rrm_msg + EXT_MSG_API_HDR_SIZE;
    if (SON_FAILURE == son_compose_son_rrm_stop_tnl_discovery_ind(&p_rrm_msg, p_son_rrm_stop_tnl_discovery_ind))
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }

    if((p_rrm_msg - msg_api_length) != p_msg)
    {
        son_msg_mem_free(p_msg);

        return SON_FAILURE;
    }
    /* Send message to */
    son_send_msg(p_msg, dest_module_id);

    return SON_SUCCESS;
}

static
son_length_t
son_get_son_rrm_global_enb_id_len
(
    son_rrm_global_enb_id_t *p_son_rrm_global_enb_id
)
{
    son_length_t length = 0;

    SON_ASSERT(p_son_rrm_global_enb_id != SON_PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_son_rrm_global_enb_id->enb_type);

    /* Get length of IE */
    length += son_get_son_rrm_cell_plmn_info_len(&p_son_rrm_global_enb_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_rrm_global_enb_id->enb_id);

    return length;
}

static
son_return_et
son_compose_son_rrm_global_enb_id
(
    U8  **pp_buffer,
    son_rrm_global_enb_id_t *p_son_rrm_global_enb_id
)
{
    SON_ASSERT(pp_buffer != SON_PNULL);
    SON_ASSERT(*pp_buffer != SON_PNULL);
    SON_ASSERT(p_son_rrm_global_enb_id != SON_PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_son_rrm_global_enb_id->enb_type < 1) || (p_son_rrm_global_enb_id->enb_type > 2))
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,  SON_WARNING, "Parameter [p_son_rrm_global_enb_id->enb_type] should be in range [1 <= %d <= 2] !", p_son_rrm_global_enb_id->enb_type);
        return SON_FAILURE;
    }

    /* Compose parameter of basic type */
    son_cp_pack_U32(*pp_buffer, &p_son_rrm_global_enb_id->enb_type, "enb_type");
    *pp_buffer += sizeof(p_son_rrm_global_enb_id->enb_type);

    /* Compose IE */
    if (SON_FAILURE == son_compose_son_rrm_cell_plmn_info(pp_buffer, &p_son_rrm_global_enb_id->plmn_id))
    {
        return SON_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        son_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_rrm_global_enb_id->enb_id); loop++)
        {
            son_cp_pack_U8(*pp_buffer, &p_son_rrm_global_enb_id->enb_id[loop], "enb_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return SON_SUCCESS;
}

