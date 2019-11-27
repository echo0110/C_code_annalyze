/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2014 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: $
 ****************************************************************************
 *
 *  File Description : The file rrc_x2apCsc_il_composer.c contains the definitions 
 *                     of rrc-x2apCsc interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_x2apCsc_il_composer.h"
#include "rrc_x2apCsc_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_return_et
rrc_il_compose_x2ap_plmn_identity
(
    U8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_length_t
rrc_il_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_return_et
rrc_il_compose_x2ap_broadcast_plmn
(
    U8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_length_t
rrc_il_get_x2ap_tac_len
(
    x2ap_tac_t *p_x2ap_tac
);

static
rrc_return_et
rrc_il_compose_x2ap_tac
(
    U8  **pp_buffer,
    x2ap_tac_t *p_x2ap_tac
);

static
rrc_length_t
rrc_il_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_return_et
rrc_il_compose_x2ap_earfcn
(
    U8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_return_et
rrc_il_compose_x2ap_fdd_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_length_t
rrc_il_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_return_et
rrc_il_compose_x2ap_special_subframe_info
(
    U8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_return_et
rrc_il_compose_x2ap_tdd_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_length_t
rrc_il_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_return_et
rrc_il_compose_x2ap_prach_configuration_info
(
    U8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_length_t
rrc_il_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_return_et
rrc_il_compose_x2ap_subframe_allocation
(
    U8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_subframe_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_info_len
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
);

static
rrc_return_et
rrc_il_compose_x2apOam_srvd_cell_info
(
    U8  **pp_buffer,
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_return_et
rrc_il_compose_x2ap_ecgi
(
    U8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_length_t
rrc_il_get_x2ap_neighbour_info_len
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
);

static
rrc_return_et
rrc_il_compose_x2ap_neighbour_info
(
    U8  **pp_buffer,
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
);

static
rrc_length_t
rrc_il_get_x2ap_nbour_cell_arry_len
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
);

static
rrc_return_et
rrc_il_compose_x2ap_nbour_cell_arry
(
    U8  **pp_buffer,
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
);

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_arry_len
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
);

static
rrc_return_et
rrc_il_compose_x2apOam_srvd_cell_arry
(
    U8  **pp_buffer,
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
);

static
rrc_length_t
rrc_il_get_x2ap_csc_cell_status_info_req_len
(
    x2ap_csc_cell_status_info_req_t *p_x2ap_csc_cell_status_info_req
);

static
rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_info_req
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_info_req_t *p_x2ap_csc_cell_status_info_req
);

static
rrc_length_t
rrc_il_get_x2ap_csc_cell_status_info_resp_len
(
    x2ap_csc_cell_status_info_resp_t *p_x2ap_csc_cell_status_info_resp
);

static
rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_info_resp
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_info_resp_t *p_x2ap_csc_cell_status_info_resp
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_x2ap_csc_cell_add_len
(
    x2ap_csc_cell_add_t *p_x2ap_csc_cell_add
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_add != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_add->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_add->eutran_cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_add->phys_cell_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_add->dl_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_add->ul_earfcn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_broadcast_plmn_len(&p_x2ap_csc_cell_add->broadcast_plmn_info);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_csc_cell_add
(
    U8  **pp_buffer,
    x2ap_csc_cell_add_t *p_x2ap_csc_cell_add
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_add != PNULL);

    /* This function composes x2ap_csc_cell_add */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_add->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_add->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_add->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_add->cell_index, "cell_index");
    *pp_buffer += sizeof(p_x2ap_csc_cell_add->cell_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_add->eutran_cell_id, "eutran_cell_id");
    *pp_buffer += sizeof(p_x2ap_csc_cell_add->eutran_cell_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_csc_cell_add->phys_cell_id, "phys_cell_id");
    *pp_buffer += sizeof(p_x2ap_csc_cell_add->phys_cell_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_csc_cell_add->dl_earfcn, "dl_earfcn");
    *pp_buffer += sizeof(p_x2ap_csc_cell_add->dl_earfcn);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_csc_cell_add->ul_earfcn, "ul_earfcn");
    *pp_buffer += sizeof(p_x2ap_csc_cell_add->ul_earfcn);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2ap_csc_cell_add->broadcast_plmn_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCsc_il_send_x2ap_csc_cell_add
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CSC_CELL_ADD_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_add
(
    x2ap_csc_cell_add_t  *p_x2ap_csc_cell_add,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCsc_msg;

    RRC_ASSERT(p_x2ap_csc_cell_add != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_csc_cell_add_len(p_x2ap_csc_cell_add);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CSC_CELL_ADD_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCsc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCsc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_CSC_CELL_ADD_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCsc_msg, transaction_id, src_module_id,
        RRC_X2APCSC_MODULE_ID, X2AP_CSC_CELL_ADD_IND, msg_length,cell_index);

    /* Fill X2AP_CSC_CELL_ADD_IND message */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_add(&p_x2apCsc_msg, p_x2ap_csc_cell_add))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCsc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_plmn_identity != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_plmn_identity->plmn_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_plmn_identity
(
    U8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_plmn_identity != PNULL);

    /* This function composes x2ap_plmn_identity */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_broadcast_plmn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %d received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_broadcast_plmn->plmn_identity[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_broadcast_plmn
(
    U8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_broadcast_plmn != PNULL);

    /* This function composes x2ap_broadcast_plmn */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %u received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_broadcast_plmn->num_bcast_plmn_id, "num_bcast_plmn_id");
    *pp_buffer += sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_broadcast_plmn->plmn_identity[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_csc_cell_upd_len
(
    x2ap_csc_cell_upd_t *p_x2ap_csc_cell_upd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_upd != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_upd->cell_index);

    /* Get length of IE */
    length += rrc_il_get_x2apOam_srvd_cell_arry_len(&p_x2ap_csc_cell_upd->served_cell_to_upd);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_csc_cell_upd
(
    U8  **pp_buffer,
    x2ap_csc_cell_upd_t *p_x2ap_csc_cell_upd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_upd != PNULL);

    /* This function composes x2ap_csc_cell_upd */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_upd->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_upd->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_upd->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_upd->cell_index, "cell_index");
    *pp_buffer += sizeof(p_x2ap_csc_cell_upd->cell_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2apOam_srvd_cell_arry(pp_buffer, &p_x2ap_csc_cell_upd->served_cell_to_upd))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCsc_il_send_x2ap_csc_cell_upd
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CSC_CELL_UPD_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_upd
(
    x2ap_csc_cell_upd_t  *p_x2ap_csc_cell_upd,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCsc_msg;

    RRC_ASSERT(p_x2ap_csc_cell_upd != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_csc_cell_upd_len(p_x2ap_csc_cell_upd);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CSC_CELL_UPD_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCsc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCsc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_CSC_CELL_UPD_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCsc_msg, transaction_id, src_module_id,
        RRC_X2APCSC_MODULE_ID, X2AP_CSC_CELL_UPD_IND, msg_length,cell_index);

    /* Fill X2AP_CSC_CELL_UPD_IND message */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_upd(&p_x2apCsc_msg, p_x2ap_csc_cell_upd))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCsc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_tac_len
(
    x2ap_tac_t *p_x2ap_tac
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_tac != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_tac->tac);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_tac
(
    U8  **pp_buffer,
    x2ap_tac_t *p_x2ap_tac
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_tac != PNULL);

    /* This function composes x2ap_tac */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_tac->tac); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tac->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_earfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_earfcn->earfcn);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_earfcn
(
    U8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_earfcn != PNULL);

    /* This function composes x2ap_earfcn */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_x2ap_earfcn->earfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_earfcn->earfcn, "earfcn");
    *pp_buffer += sizeof(p_x2ap_earfcn->earfcn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_fdd_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_fdd_info->ul_earfcn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_fdd_info->dl_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_fdd_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_fdd_info != PNULL);

    /* This function composes x2ap_fdd_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->ul_earfcn))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->dl_earfcn))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_fdd_info->ul_trnsmission_bdwidth, "ul_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_fdd_info->dl_trnsmission_bdwidth, "dl_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_special_subframe_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->dl_cp);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->ul_cp);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_special_subframe_info
(
    U8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_special_subframe_info != PNULL);

    /* This function composes x2ap_special_subframe_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be less than"
            " or equal to 8. Incorrect value %u received.", p_x2ap_special_subframe_info->special_subframe_patterns);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->special_subframe_patterns, "special_subframe_patterns");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->dl_cp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->dl_cp, "dl_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->dl_cp);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->ul_cp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->ul_cp, "ul_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->ul_cp);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_tdd_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_tdd_info->earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_special_subframe_info_len(&p_x2ap_tdd_info->special_subframe_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_tdd_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_tdd_info != PNULL);

    /* This function composes x2ap_tdd_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_tdd_info->earfcn))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_tdd_info->trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tdd_info->trnsmission_bdwidth, "trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_tdd_info->subframe_assn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tdd_info->subframe_assn, "subframe_assn");
    *pp_buffer += sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_special_subframe_info(pp_buffer, &p_x2ap_tdd_info->special_subframe_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_eutra_mode != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_fdd_info_len(&p_x2ap_choice_eutra_mode->fdd_info);
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_tdd_info_len(&p_x2ap_choice_eutra_mode->tdd_info);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_eutra_mode != PNULL);

    /* This function composes x2ap_choice_eutra_mode */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_choice_eutra_mode->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_fdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->fdd_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->tdd_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_eutra_mode_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_len(&p_x2ap_choice_eutra_mode_info->choice_eutra_mode);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_choice_eutra_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_eutra_mode_info != PNULL);

    /* This function composes x2ap_choice_eutra_mode_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode(pp_buffer, &p_x2ap_choice_eutra_mode_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_prach_configuration_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->root_sequence_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->high_speed_flag);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_prach_configuration_info
(
    U8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_prach_configuration_info != PNULL);

    /* This function composes x2ap_prach_configuration_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_prach_configuration_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_x2ap_prach_configuration_info->root_sequence_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_prach_configuration_info->root_sequence_index, "root_sequence_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->root_sequence_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->zero_correlation_zone_config, "zero_correlation_zone_config");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_prach_configuration_info->high_speed_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->high_speed_flag, "high_speed_flag");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->high_speed_flag);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_frequency_offset, "prach_frequency_offset");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_config_index, "prach_config_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_subframe_allocation != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_subframe_allocation->oneframe);
    }

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_subframe_allocation->fourframe);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_subframe_allocation
(
    U8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_subframe_allocation != PNULL);

    /* This function composes x2ap_subframe_allocation */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_subframe_allocation->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_subframe_allocation->oneframe[loop], "oneframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_subframe_allocation->fourframe[loop], "fourframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mbsfn_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Get length of IE */
    length += rrc_il_get_x2ap_subframe_allocation_len(&p_x2ap_mbsfn_info->subframe_alloc);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mbsfn_info != PNULL);

    /* This function composes x2ap_mbsfn_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_period, "radioframe_alloc_period");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_offset, "radioframe_alloc_offset");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_subframe_allocation(pp_buffer, &p_x2ap_mbsfn_info->subframe_alloc))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mbsfn_subframe_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %d received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            length += rrc_il_get_x2ap_mbsfn_info_len(&p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_mbsfn_subframe_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mbsfn_subframe_info != PNULL);

    /* This function composes x2ap_mbsfn_subframe_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %u received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_subframe_info->num_mbsfn, "num_mbsfn");
    *pp_buffer += sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_mbsfn_info(pp_buffer, &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_info_len
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2apOam_srvd_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->pci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_tac_len(&p_x2apOam_srvd_cell_info->track_area_code);

    /* Get length of IE */
    length += rrc_il_get_x2ap_broadcast_plmn_len(&p_x2apOam_srvd_cell_info->broadcast_plmn_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_info_len(&p_x2apOam_srvd_cell_info->choice_eutra_mode);

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_prach_configuration_info_len(&p_x2apOam_srvd_cell_info->prach_config);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_mbsfn_subframe_info_len(&p_x2apOam_srvd_cell_info->mbsfn_subframe_info);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2apOam_srvd_cell_info->csg_identity);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2apOam_srvd_cell_info
(
    U8  **pp_buffer,
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2apOam_srvd_cell_info != PNULL);

    /* This function composes x2apOam_srvd_cell_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2apOam_srvd_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2apOam_srvd_cell_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2apOam_srvd_cell_info->pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2apOam_srvd_cell_info->pci, "pci");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->cell_id > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2apOam_srvd_cell_info->cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_x2apOam_srvd_cell_info->cell_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2apOam_srvd_cell_info->cell_id, "cell_id");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->cell_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_tac(pp_buffer, &p_x2apOam_srvd_cell_info->track_area_code))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2apOam_srvd_cell_info->broadcast_plmn_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode_info(pp_buffer, &p_x2apOam_srvd_cell_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->num_antenna_port > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2apOam_srvd_cell_info->num_antenna_port] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2apOam_srvd_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2apOam_srvd_cell_info->num_antenna_port, "num_antenna_port");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_prach_configuration_info(pp_buffer, &p_x2apOam_srvd_cell_info->prach_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_mbsfn_subframe_info(pp_buffer, &p_x2apOam_srvd_cell_info->mbsfn_subframe_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2apOam_srvd_cell_info->csg_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2apOam_srvd_cell_info->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_ecgi != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_ecgi->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_ecgi->eutran_cell_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_ecgi
(
    U8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_ecgi != PNULL);

    /* This function composes x2ap_ecgi */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_ecgi->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_ecgi->eutran_cell_id[loop], "eutran_cell_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_neighbour_info_len
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_neighbour_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_neighbour_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ecgi_len(&p_x2ap_neighbour_info->ecgi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_neighbour_info->pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_neighbour_info->earfcn);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_neighbour_info
(
    U8  **pp_buffer,
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_neighbour_info != PNULL);

    /* This function composes x2ap_neighbour_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_neighbour_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_neighbour_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_ecgi(pp_buffer, &p_x2ap_neighbour_info->ecgi))
    {
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_neighbour_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_neighbour_info->pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_neighbour_info->earfcn))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nbour_cell_arry_len
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nbour_cell_arry != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nbour_cell_arry->num_neighbour_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %d received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            length += rrc_il_get_x2ap_neighbour_info_len(&p_x2ap_nbour_cell_arry->neighbour_info_arr[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_nbour_cell_arry
(
    U8  **pp_buffer,
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nbour_cell_arry != PNULL);

    /* This function composes x2ap_nbour_cell_arry */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %u received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nbour_cell_arry->num_neighbour_cell, "num_neighbour_cell");
    *pp_buffer += sizeof(p_x2ap_nbour_cell_arry->num_neighbour_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_neighbour_info(pp_buffer, &p_x2ap_nbour_cell_arry->neighbour_info_arr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_arry_len
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2apOam_srvd_cell_arry != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2apOam_srvd_cell_info_len(&p_x2apOam_srvd_cell_arry->served_cell_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nbour_cell_arry_len(&p_x2apOam_srvd_cell_arry->nbour_cell_info);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2apOam_srvd_cell_arry
(
    U8  **pp_buffer,
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2apOam_srvd_cell_arry != PNULL);

    /* This function composes x2apOam_srvd_cell_arry */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2apOam_srvd_cell_info(pp_buffer, &p_x2apOam_srvd_cell_arry->served_cell_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2ap_nbour_cell_arry(pp_buffer, &p_x2apOam_srvd_cell_arry->nbour_cell_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_csc_cell_del_len
(
    x2ap_csc_cell_del_t *p_x2ap_csc_cell_del
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_del != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_del->cell_index);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_csc_cell_del
(
    U8  **pp_buffer,
    x2ap_csc_cell_del_t *p_x2ap_csc_cell_del
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_del != PNULL);

    /* This function composes x2ap_csc_cell_del */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_del->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_del->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_del->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_del->cell_index, "cell_index");
    *pp_buffer += sizeof(p_x2ap_csc_cell_del->cell_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCsc_il_send_x2ap_csc_cell_del
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CSC_CELL_DEL_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_del
(
    x2ap_csc_cell_del_t  *p_x2ap_csc_cell_del,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCsc_msg;

    RRC_ASSERT(p_x2ap_csc_cell_del != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_csc_cell_del_len(p_x2ap_csc_cell_del);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CSC_CELL_DEL_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCsc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCsc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_CSC_CELL_DEL_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCsc_msg, transaction_id, src_module_id,
        RRC_X2APCSC_MODULE_ID, X2AP_CSC_CELL_DEL_IND, msg_length,cell_index);

    /* Fill X2AP_CSC_CELL_DEL_IND message */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_del(&p_x2apCsc_msg, p_x2ap_csc_cell_del))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCsc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_csc_cell_status_req_len
(
    x2ap_csc_cell_status_req_t *p_x2ap_csc_cell_status_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_status_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_status_req->num_served_cells);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_csc_cell_status_req->num_served_cells; loop++)
        {
            length += rrc_il_get_x2ap_csc_cell_status_info_req_len(&p_x2ap_csc_cell_status_req->cell_info[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_req
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_req_t *p_x2ap_csc_cell_status_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_status_req != PNULL);

    /* This function composes x2ap_csc_cell_status_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_csc_cell_status_req->num_served_cells, "num_served_cells");
    *pp_buffer += sizeof(p_x2ap_csc_cell_status_req->num_served_cells);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_csc_cell_status_req->num_served_cells; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_status_info_req(pp_buffer, &p_x2ap_csc_cell_status_req->cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCsc_il_send_x2ap_csc_cell_status_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CSC_CELL_STATUS_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_status_req
(
    x2ap_csc_cell_status_req_t  *p_x2ap_csc_cell_status_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCsc_msg;

    RRC_ASSERT(p_x2ap_csc_cell_status_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_csc_cell_status_req_len(p_x2ap_csc_cell_status_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CSC_CELL_STATUS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCsc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCsc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_CSC_CELL_STATUS_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCsc_msg, transaction_id, src_module_id,
        RRC_X2APCSC_MODULE_ID, X2AP_CSC_CELL_STATUS_REQ, msg_length,cell_index);

    /* Fill X2AP_CSC_CELL_STATUS_REQ message */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_status_req(&p_x2apCsc_msg, p_x2ap_csc_cell_status_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCsc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_csc_cell_status_info_req_len
(
    x2ap_csc_cell_status_info_req_t *p_x2ap_csc_cell_status_info_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_status_info_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_status_info_req->cell_index);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_info_req
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_info_req_t *p_x2ap_csc_cell_status_info_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_status_info_req != PNULL);

    /* This function composes x2ap_csc_cell_status_info_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_status_info_req->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_status_info_req->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_status_info_req->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_status_info_req->cell_index, "cell_index");
    *pp_buffer += sizeof(p_x2ap_csc_cell_status_info_req->cell_index);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_csc_cell_status_resp_len
(
    x2ap_csc_cell_status_resp_t *p_x2ap_csc_cell_status_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_status_resp->num_served_cells);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_csc_cell_status_resp->num_served_cells; loop++)
        {
            length += rrc_il_get_x2ap_csc_cell_status_info_resp_len(&p_x2ap_csc_cell_status_resp->cell_status_info[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_status_resp->no_of_uecc_instances);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_resp
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_resp_t *p_x2ap_csc_cell_status_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_status_resp != PNULL);

    /* This function composes x2ap_csc_cell_status_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_csc_cell_status_resp->num_served_cells, "num_served_cells");
    *pp_buffer += sizeof(p_x2ap_csc_cell_status_resp->num_served_cells);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_csc_cell_status_resp->num_served_cells; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_status_info_resp(pp_buffer, &p_x2ap_csc_cell_status_resp->cell_status_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_status_resp->no_of_uecc_instances, "no_of_uecc_instances");
    *pp_buffer += sizeof(p_x2ap_csc_cell_status_resp->no_of_uecc_instances);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCsc_il_send_x2ap_csc_cell_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CSC_CELL_STATUS_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCsc_il_send_x2ap_csc_cell_status_resp
(
    x2ap_csc_cell_status_resp_t  *p_x2ap_csc_cell_status_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCsc_msg;

    RRC_ASSERT(p_x2ap_csc_cell_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_csc_cell_status_resp_len(p_x2ap_csc_cell_status_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CSC_CELL_STATUS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCsc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCsc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_CSC_CELL_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCsc_msg, transaction_id, src_module_id,
        RRC_X2APCSC_MODULE_ID, X2AP_CSC_CELL_STATUS_RESP, msg_length,cell_index);

    /* Fill X2AP_CSC_CELL_STATUS_RESP message */
    p_x2apCsc_msg = p_x2apCsc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_csc_cell_status_resp(&p_x2apCsc_msg, p_x2ap_csc_cell_status_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCsc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_csc_cell_status_info_resp_len
(
    x2ap_csc_cell_status_info_resp_t *p_x2ap_csc_cell_status_info_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_csc_cell_status_info_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_status_info_resp->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_csc_cell_status_info_resp->status);

    return length;
}

static
rrc_return_et
rrc_il_compose_x2ap_csc_cell_status_info_resp
(
    U8  **pp_buffer,
    x2ap_csc_cell_status_info_resp_t *p_x2ap_csc_cell_status_info_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_csc_cell_status_info_resp != PNULL);

    /* This function composes x2ap_csc_cell_status_info_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_status_info_resp->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_status_info_resp->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_status_info_resp->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_status_info_resp->cell_index, "cell_index");
    *pp_buffer += sizeof(p_x2ap_csc_cell_status_info_resp->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_status_info_resp->status > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_status_info_resp->status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_csc_cell_status_info_resp->status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_csc_cell_status_info_resp->status, "status");
    *pp_buffer += sizeof(p_x2ap_csc_cell_status_info_resp->status);

    return RRC_SUCCESS;
}

