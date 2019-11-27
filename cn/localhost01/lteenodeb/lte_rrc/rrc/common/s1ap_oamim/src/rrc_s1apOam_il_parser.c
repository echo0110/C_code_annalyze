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
 *  File Description : The file rrc_s1apOam_il_parser.c contains the definitions 
 *                     of rrc-s1apOam interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifdef _MSC_VER
#include "pc_test.h"
#endif
#include "rrc_cp_common.h"
#include "rrc_s1apOam_il_parser.h"
#include "rrc_s1apOam_intf.h"
#include "rrc_ext_api.h"
#ifndef _MSC_VER
#include "rrc_common_utils.h"
#endif


#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_s1ap_plmn_identity
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_bc_plmn_list
(
    bc_plmn_list_t *p_bc_plmn_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_supp_ta
(
    supp_ta_t *p_supp_ta,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_supp_ta_list
(
    supp_ta_list_t *p_supp_ta_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_csg_id_info
(
    csg_id_info_t *p_csg_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_csg_id_list
(
    csg_id_list_t *p_csg_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_id_list_info
(
    s1ap_oam_mme_id_list_info_t *p_s1ap_oam_mme_id_list_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_conn_info
(
    s1ap_oam_mme_conn_info_t *p_s1ap_oam_mme_conn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_ip_addr
(
    ip_addr_t *p_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_ipv6_addr
(
    ipv6_addr_t *p_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_mme_comm_info
(
    mme_comm_info_t *p_mme_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_enb_comm_info
(
    enb_comm_info_t *p_enb_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_enb_id_info
(
    enb_id_info_t *p_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_gb_enb_id
(
    gb_enb_id_t *p_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1_setup_req
(
    s1_setup_req_t *p_s1_setup_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_sctp_conf_info
(
    s1ap_sctp_conf_info_t *p_s1ap_sctp_conf_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1apInfo
(
    s1apInfo_t *p_s1apInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_active_mme_ctx
(
    active_mme_ctx_t *p_active_mme_ctx,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_served_group_id
(
    s1ap_served_group_id_t *p_s1ap_served_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_served_mmec
(
    s1ap_served_mmec_t *p_s1ap_served_mmec,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_served_gummei_info
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_mme_info
(
    s1ap_mme_info_t *p_s1ap_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_timeval
(
    rrc_timeval_t *p_rrc_timeval,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_id_status_params
(
    s1ap_oam_mme_id_status_params_t *p_s1ap_oam_mme_id_status_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_status_params_list_info
(
    s1ap_oam_mme_status_params_list_info_t *p_s1ap_oam_mme_status_params_list_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_oam_failed_mme_info
(
    s1ap_oam_failed_mme_info_t *p_s1ap_oam_failed_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_failed_list
(
    s1ap_oam_mme_failed_list_t *p_s1ap_oam_mme_failed_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_s1ap_oam_enb_config_update
(
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_enb_config_update, 0, sizeof(s1ap_oam_enb_config_update_t));

    /* This function parses s1ap_oam_enb_config_update */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_enb_config_update->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_enb_config_update->enb_name) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_enb_config_update->enb_name); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_enb_config_update->enb_name[loop], (void*)(p_src + *p_length_read), "enb_name[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_supp_ta_list(
        &p_s1ap_oam_enb_config_update->supp_ta_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_csg_id_list(
        &p_s1ap_oam_enb_config_update->csg_id_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_oam_enb_config_update->default_paging_drx, p_src + *p_length_read, "default_paging_drx");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_enb_config_update->default_paging_drx > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_enb_config_update->default_paging_drx] should be less than"
            " or equal to 3. Incorrect value %u received.", p_s1ap_oam_enb_config_update->default_paging_drx);
        return RRC_FAILURE;
    }
    }

    if (p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_oam_mme_id_list_info(
        &p_s1ap_oam_enb_config_update->s1ap_oam_mme_id_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_plmn_identity
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_plmn_identity, 0, sizeof(s1ap_plmn_identity_t));

    /* This function parses s1ap_plmn_identity */

    if (*p_length_read + (S32)sizeof(p_s1ap_plmn_identity->plmn_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_bc_plmn_list
(
    bc_plmn_list_t *p_bc_plmn_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_bc_plmn_list, 0, sizeof(bc_plmn_list_t));

    /* This function parses bc_plmn_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_bc_plmn_list->num_bplmn, p_src + *p_length_read, "num_bplmn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_bc_plmn_list->num_bplmn < 1) || (p_bc_plmn_list->num_bplmn > MAX_BC_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_bc_plmn_list->num_bplmn] should be in range "
            "1 to MAX_BC_PLMN. Incorrect value %u received.", p_bc_plmn_list->num_bplmn);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_bc_plmn_list->num_bplmn; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_plmn_identity(
                &p_bc_plmn_list->plmn_identity[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_supp_ta
(
    supp_ta_t *p_supp_ta,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_supp_ta, 0, sizeof(supp_ta_t));

    /* This function parses supp_ta */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_supp_ta->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(p_supp_ta->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_supp_ta->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_supp_ta->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (RRC_FAILURE == rrc_il_parse_bc_plmn_list(
        &p_supp_ta->bc_plmn_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_supp_ta_list
(
    supp_ta_list_t *p_supp_ta_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_supp_ta_list, 0, sizeof(supp_ta_list_t));

    /* This function parses supp_ta_list */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_supp_ta_list->num_supported_tais, p_src + *p_length_read, "num_supported_tais");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_supp_ta_list->num_supported_tais < 1) || (p_supp_ta_list->num_supported_tais > MAX_NUM_TAC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_supp_ta_list->num_supported_tais] should be in range "
            "1 to MAX_NUM_TAC. Incorrect value %u received.", p_supp_ta_list->num_supported_tais);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_supp_ta_list->num_supported_tais; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_supp_ta(
                &p_supp_ta_list->supp_tais[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_csg_id_info
(
    csg_id_info_t *p_csg_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_csg_id_info, 0, sizeof(csg_id_info_t));

    /* This function parses csg_id_info */

    if (*p_length_read + (S32)sizeof(p_csg_id_info->csg_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_csg_id_info->csg_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_csg_id_info->csg_id[loop], (void*)(p_src + *p_length_read), "csg_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_csg_id_list
(
    csg_id_list_t *p_csg_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_csg_id_list, 0, sizeof(csg_id_list_t));

    /* This function parses csg_id_list */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_csg_id_list->num_csg_ids, p_src + *p_length_read, "num_csg_ids");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_csg_id_list->num_csg_ids < 1) || (p_csg_id_list->num_csg_ids > MAX_CSG_ID_LIST))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_csg_id_list->num_csg_ids] should be in range "
            "1 to MAX_CSG_ID_LIST. Incorrect value %u received.", p_csg_id_list->num_csg_ids);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_csg_id_list->num_csg_ids; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_csg_id_info(
                &p_csg_id_list->csg_ids[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_id_list_info
(
    s1ap_oam_mme_id_list_info_t *p_s1ap_oam_mme_id_list_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_id_list_info, 0, sizeof(s1ap_oam_mme_id_list_info_t));

    /* This function parses s1ap_oam_mme_id_list_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_id_list_info->num_mme_id, p_src + *p_length_read, "num_mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_id_list_info->num_mme_id < 1) || (p_s1ap_oam_mme_id_list_info->num_mme_id > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_id_list_info->num_mme_id] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_mme_id_list_info->num_mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_s1ap_oam_mme_id_list_info->num_mme_id * sizeof(p_s1ap_oam_mme_id_list_info->mme_id[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_id_list_info->num_mme_id; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_mme_id_list_info->mme_id[loop], (void*)(p_src + *p_length_read), "mme_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_mme_conn_status_response
(
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_conn_status_response, 0, sizeof(s1ap_oam_mme_conn_status_response_t));

    /* This function parses s1ap_oam_mme_conn_status_response */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_conn_status_response->mme_count, p_src + *p_length_read, "mme_count");
    *p_length_read += sizeof(U8);

    /* Parse OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_conn_status_response->oam_mme_conn_info_list); loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_oam_mme_conn_info(
                &p_s1ap_oam_mme_conn_status_response->oam_mme_conn_info_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_conn_info
(
    s1ap_oam_mme_conn_info_t *p_s1ap_oam_mme_conn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_conn_info, 0, sizeof(s1ap_oam_mme_conn_info_t));

    /* This function parses s1ap_oam_mme_conn_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_conn_info->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_mme_conn_info->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_conn_info->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_oam_mme_conn_info->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_conn_info->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_mme_conn_info->response > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_conn_info->response] should be less than"
            " or equal to 4. Incorrect value %u received.", p_s1ap_oam_mme_conn_info->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_enb_config_update_response
(
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_enb_config_update_response, 0, sizeof(s1ap_oam_enb_config_update_response_t));

    /* This function parses s1ap_oam_enb_config_update_response */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_enb_config_update_response->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_enb_config_update_response->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_enb_config_update_response->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_oam_enb_config_update_response->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_enb_config_update_response->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_enb_config_update_response->response > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_enb_config_update_response->response] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1ap_oam_enb_config_update_response->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_close_mme_conn
(
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_close_mme_conn, 0, sizeof(s1ap_oam_close_mme_conn_t));

    /* This function parses s1ap_oam_close_mme_conn */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_close_mme_conn->mme_count, p_src + *p_length_read, "mme_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_close_mme_conn->mme_count < 1) || (p_s1ap_oam_close_mme_conn->mme_count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_close_mme_conn->mme_count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_close_mme_conn->mme_count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_close_mme_conn->mme_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_close_mme_conn->mme_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_close_mme_conn->mme_id[loop], (void*)(p_src + *p_length_read), "mme_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_reestab_mme_conn
(
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_reestab_mme_conn, 0, sizeof(s1ap_oam_reestab_mme_conn_t));

    /* This function parses s1ap_oam_reestab_mme_conn */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_reestab_mme_conn->mme_count, p_src + *p_length_read, "mme_count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_reestab_mme_conn->mme_count < 1) || (p_s1ap_oam_reestab_mme_conn->mme_count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_reestab_mme_conn->mme_count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_reestab_mme_conn->mme_count);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_reestab_mme_conn->mme_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_reestab_mme_conn->mme_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_reestab_mme_conn->mme_id[loop], (void*)(p_src + *p_length_read), "mme_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_provision_req
(
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_provision_req, 0, sizeof(s1ap_oam_provision_req_t));

    /* This function parses s1ap_oam_provision_req */

    if (RRC_FAILURE == rrc_il_parse_s1apInfo(
        &p_s1ap_oam_provision_req->s1apInfo,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_ip_addr
(
    ip_addr_t *p_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_ip_addr, 0, sizeof(ip_addr_t));

    /* This function parses ip_addr */

    if (*p_length_read + (S32)sizeof(p_ip_addr->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ip_addr->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_ip_addr->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_ipv6_addr
(
    ipv6_addr_t *p_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_ipv6_addr, 0, sizeof(ipv6_addr_t));

    /* This function parses ipv6_addr */

    if (*p_length_read + (S32)sizeof(p_ipv6_addr->ipv6_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ipv6_addr->ipv6_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_ipv6_addr->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_mme_comm_info
(
    mme_comm_info_t *p_mme_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_mme_comm_info, 0, sizeof(mme_comm_info_t));

    /* This function parses mme_comm_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_mme_comm_info->num_ip_addr, p_src + *p_length_read, "num_ip_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be less than"
            " or equal to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_mme_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_ip_addr(
                &p_mme_comm_info->ip_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_mme_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_mme_comm_info->num_streams, p_src + *p_length_read, "num_streams");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_streams < 2) || (p_mme_comm_info->num_streams > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_streams] should be in range "
            "2 to 10. Incorrect value %u received.", p_mme_comm_info->num_streams);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_mme_comm_info->heart_beat_timer, p_src + *p_length_read, "heart_beat_timer");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_mme_comm_info->rel_of_mme, p_src + *p_length_read, "rel_of_mme");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->rel_of_mme > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->rel_of_mme] should be less than"
            " or equal to 2. Incorrect value %u received.", p_mme_comm_info->rel_of_mme);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_mme_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_mme_comm_info->num_ipv6_addr, p_src + *p_length_read, "num_ipv6_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_ipv6_addr < 1) || (p_mme_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_mme_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }
    }

    if (p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_ipv6_addr(
                &p_mme_comm_info->ipv6_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_enb_comm_info
(
    enb_comm_info_t *p_enb_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_enb_comm_info, 0, sizeof(enb_comm_info_t));

    /* This function parses enb_comm_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_enb_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_enb_comm_info->num_ip_addr, p_src + *p_length_read, "num_ip_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_enb_comm_info->num_ip_addr < 1) || (p_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }
    }

    if (p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_enb_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_ip_addr(
                &p_enb_comm_info->ip_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_enb_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_enb_comm_info->num_ipv6_addr, p_src + *p_length_read, "num_ipv6_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_enb_comm_info->num_ipv6_addr < 1) || (p_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }
    }

    if (p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_enb_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_ipv6_addr(
                &p_enb_comm_info->ipv6_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_macro_enb_id, 0, sizeof(macro_enb_id_t));

    /* This function parses macro_enb_id */

    if (*p_length_read + (S32)sizeof(p_macro_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_macro_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_home_enb_id, 0, sizeof(home_enb_id_t));

    /* This function parses home_enb_id */

    if (*p_length_read + (S32)sizeof(p_home_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_home_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_enb_id_info
(
    enb_id_info_t *p_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_enb_id_info, 0, sizeof(enb_id_info_t));

    /* This function parses enb_id_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_enb_id_info->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U8);

    if (p_enb_id_info->presence_bitmask & 1)
    {

    if (RRC_FAILURE == rrc_il_parse_macro_enb_id(
        &p_enb_id_info->macro_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_enb_id_info->presence_bitmask & 2)
    {

    if (RRC_FAILURE == rrc_il_parse_home_enb_id(
        &p_enb_id_info->home_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_gb_enb_id
(
    gb_enb_id_t *p_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_gb_enb_id, 0, sizeof(gb_enb_id_t));

    /* This function parses gb_enb_id */

    if (RRC_FAILURE == rrc_il_parse_s1ap_plmn_identity(
        &p_gb_enb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_enb_id_info(
        &p_gb_enb_id->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1_setup_req
(
    s1_setup_req_t *p_s1_setup_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1_setup_req, 0, sizeof(s1_setup_req_t));

    /* This function parses s1_setup_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1_setup_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_gb_enb_id(
        &p_s1_setup_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_s1_setup_req->bitmask & 1)
    {

    if (*p_length_read + (S32)sizeof(p_s1_setup_req->enb_name) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1_setup_req->enb_name); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1_setup_req->enb_name[loop], (void*)(p_src + *p_length_read), "enb_name[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (RRC_FAILURE == rrc_il_parse_supp_ta_list(
        &p_s1_setup_req->supp_ta_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_s1_setup_req->bitmask & S1AP_OAM_CSG_ID_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_csg_id_list(
        &p_s1_setup_req->csg_id_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1_setup_req->default_paging_drx, p_src + *p_length_read, "default_paging_drx");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1_setup_req->default_paging_drx > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1_setup_req->default_paging_drx] should be less than"
            " or equal to 3. Incorrect value %u received.", p_s1_setup_req->default_paging_drx);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_sctp_conf_info
(
    s1ap_sctp_conf_info_t *p_s1ap_sctp_conf_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_sctp_conf_info, 0, sizeof(s1ap_sctp_conf_info_t));

    /* This function parses s1ap_sctp_conf_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_sctp_conf_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_sctp_conf_info->init_max_attempts, p_src + *p_length_read, "init_max_attempts");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_sctp_conf_info->rto_initial, p_src + *p_length_read, "rto_initial");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_sctp_conf_info->rto_max, p_src + *p_length_read, "rto_max");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_sctp_conf_info->rto_min, p_src + *p_length_read, "rto_min");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_sctp_conf_info->assoc_max_retrans, p_src + *p_length_read, "assoc_max_retrans");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_sctp_conf_info->valid_cookie_life, p_src + *p_length_read, "valid_cookie_life");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_sctp_conf_info->path_max_retrans, p_src + *p_length_read, "path_max_retrans");
    *p_length_read += sizeof(U16);

    if (p_s1ap_sctp_conf_info->bitmask & S1AP_OAM_DSCP_VALUE_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_sctp_conf_info->s1ap_dscp_value, p_src + *p_length_read, "s1ap_dscp_value");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_sctp_conf_info->s1ap_dscp_value > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_sctp_conf_info->s1ap_dscp_value] should be less than"
            " or equal to 63. Incorrect value %u received.", p_s1ap_sctp_conf_info->s1ap_dscp_value);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1apInfo
(
    s1apInfo_t *p_s1apInfo,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1apInfo, 0, sizeof(s1apInfo_t));

    /* This function parses s1apInfo */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1apInfo->num_mme, p_src + *p_length_read, "num_mme");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1apInfo->num_mme < 1) || (p_s1apInfo->num_mme > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->num_mme] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1apInfo->num_mme);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1apInfo->num_mme; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_mme_comm_info(
                &p_s1apInfo->mme_comm_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (RRC_FAILURE == rrc_il_parse_enb_comm_info(
        &p_s1apInfo->enb_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_s1_setup_req(
        &p_s1apInfo->s1_setup_req_parameters,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1apInfo->s1_config_timer, p_src + *p_length_read, "s1_config_timer");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1_config_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1_config_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1_config_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1apInfo->reset_ep_timer, p_src + *p_length_read, "reset_ep_timer");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->reset_ep_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->reset_ep_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->reset_ep_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1apInfo->max_s1_config_retries, p_src + *p_length_read, "max_s1_config_retries");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1apInfo->max_reset_retries, p_src + *p_length_read, "max_reset_retries");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1apInfo->s1ap_pws_timer, p_src + *p_length_read, "s1ap_pws_timer");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1ap_pws_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1ap_pws_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1ap_pws_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1apInfo->s1ap_kill_timer, p_src + *p_length_read, "s1ap_kill_timer");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1ap_kill_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1ap_kill_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1ap_kill_timer);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1apInfo->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_s1apInfo->bitmask & S1AP_OAM_MME_SELECTION_ALGO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1apInfo->mme_selection_algo, p_src + *p_length_read, "mme_selection_algo");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1apInfo->mme_selection_algo > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->mme_selection_algo] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1apInfo->mme_selection_algo);
        return RRC_FAILURE;
    }
    }

    if (p_s1apInfo->bitmask & S1AP_OAM_S1_CONN_RECOVERY_DURATION_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1apInfo->s1_conn_recovery_duration, p_src + *p_length_read, "s1_conn_recovery_duration");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1_conn_recovery_duration < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1_conn_recovery_duration] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1_conn_recovery_duration);
        return RRC_FAILURE;
    }
    }

    if (p_s1apInfo->bitmask & S1AP_OAM_SCTP_CONF_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_sctp_conf_info(
        &p_s1apInfo->s1ap_sctp_conf_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1apInfo->bitmask & S1AP_OAM_S1_HEALTH_MONITORING_TIMER_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1apInfo->s1ap_health_mon_time_int, p_src + *p_length_read, "s1ap_health_mon_time_int");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1apInfo->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1apInfo->log_level > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1apInfo->log_level);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_provision_resp
(
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_provision_resp, 0, sizeof(s1ap_oam_provision_resp_t));

    /* This function parses s1ap_oam_provision_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_provision_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_provision_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_provision_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_provision_resp->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_get_log_level_resp
(
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_get_log_level_resp, 0, sizeof(s1ap_oam_get_log_level_resp_t));

    /* This function parses s1ap_oam_get_log_level_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_get_log_level_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_get_log_level_resp->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_get_log_level_resp->log_on_off > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_get_log_level_resp->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_get_log_level_resp->log_on_off);
        return RRC_FAILURE;
    }

    if (p_s1ap_oam_get_log_level_resp->bitmask & S1AP_OAM_LOG_LEVEL_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_get_log_level_resp->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_get_log_level_resp->log_level > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_get_log_level_resp->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1ap_oam_get_log_level_resp->log_level);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_set_log_level_req
(
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_set_log_level_req, 0, sizeof(s1ap_oam_set_log_level_req_t));

    /* This function parses s1ap_oam_set_log_level_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_set_log_level_req->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_set_log_level_req->log_level > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_set_log_level_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1ap_oam_set_log_level_req->log_level);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_set_log_level_resp
(
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_set_log_level_resp, 0, sizeof(s1ap_oam_set_log_level_resp_t));

    /* This function parses s1ap_oam_set_log_level_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_set_log_level_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_set_log_level_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_set_log_level_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_set_log_level_resp->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_log_enable_req
(
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_log_enable_req, 0, sizeof(s1ap_oam_log_enable_req_t));

    /* This function parses s1ap_oam_log_enable_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_log_enable_req->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_log_enable_req->log_on_off > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_log_enable_req->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_log_enable_req->log_on_off);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_log_enable_resp
(
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_log_enable_resp, 0, sizeof(s1ap_oam_log_enable_resp_t));

    /* This function parses s1ap_oam_log_enable_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_log_enable_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_log_enable_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_log_enable_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_log_enable_resp->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_reset_resp
(
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_reset_resp, 0, sizeof(s1ap_oam_reset_resp_t));

    /* This function parses s1ap_oam_reset_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_reset_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_reset_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_reset_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_reset_resp->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_s1ap_link_status_ind
(
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_s1ap_link_status_ind, 0, sizeof(s1ap_oam_s1ap_link_status_ind_t));

    /* This function parses s1ap_oam_s1ap_link_status_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_s1ap_link_status_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_s1ap_link_status_ind->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_s1ap_link_status_ind->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_s1ap_link_status_ind->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_oam_s1ap_link_status_ind->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_s1ap_link_status_ind->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_s1ap_link_status_ind->status > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_s1ap_link_status_ind->status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_s1ap_link_status_ind->status);
        return RRC_FAILURE;
    }

    if (p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_IPv4_ADDRESS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_s1ap_link_status_ind->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_s1ap_link_status_ind->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_s1ap_link_status_ind->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_IPv6_ADDRESS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_s1ap_link_status_ind->ipv6_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_s1ap_link_status_ind->ipv6_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_s1ap_link_status_ind->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_PORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_s1ap_link_status_ind->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_status_resp
(
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_status_resp, 0, sizeof(s1ap_oam_status_resp_t));

    /* This function parses s1ap_oam_status_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_status_resp->active_mme_count, p_src + *p_length_read, "active_mme_count");
    *p_length_read += sizeof(U8);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_status_resp->active_mme_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_active_mme_ctx(
                &p_s1ap_oam_status_resp->active_mme_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_active_mme_ctx
(
    active_mme_ctx_t *p_active_mme_ctx,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_active_mme_ctx, 0, sizeof(active_mme_ctx_t));

    /* This function parses active_mme_ctx */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_active_mme_ctx->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_active_mme_ctx->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_active_mme_ctx->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_active_mme_ctx->mme_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_active_mme_ctx->count_of_ue, p_src + *p_length_read, "count_of_ue");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_mme_info
(
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_info, 0, sizeof(s1ap_oam_mme_info_t));

    /* This function parses s1ap_oam_mme_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_mme_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_info->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_mme_info->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_info->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_mme_info->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_mme_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_s1ap_mme_info(
        &p_s1ap_oam_mme_info->mme_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_served_group_id
(
    s1ap_served_group_id_t *p_s1ap_served_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_served_group_id, 0, sizeof(s1ap_served_group_id_t));

    /* This function parses s1ap_served_group_id */

    if (*p_length_read + (S32)sizeof(p_s1ap_served_group_id->mmegi) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_served_group_id->mmegi); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_served_group_id->mmegi[loop], (void*)(p_src + *p_length_read), "mmegi[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_served_mmec
(
    s1ap_served_mmec_t *p_s1ap_served_mmec,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_served_mmec, 0, sizeof(s1ap_served_mmec_t));

    /* This function parses s1ap_served_mmec */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_served_mmec->mmec, p_src + *p_length_read, "mmec");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_served_gummei_info
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_served_gummei_info, 0, sizeof(s1ap_served_gummei_info_t));

    /* This function parses s1ap_served_gummei_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_served_gummei_info->num_served_plmns, p_src + *p_length_read, "num_served_plmns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_plmns < 1) || (p_s1ap_served_gummei_info->num_served_plmns > MAX_NUM_SERVED_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_plmns] should be in range "
            "1 to MAX_NUM_SERVED_PLMN. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_plmns);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_plmns; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_plmn_identity(
                &p_s1ap_served_gummei_info->plmn_identity[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_served_gummei_info->num_served_group_ids, p_src + *p_length_read, "num_served_group_ids");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_group_ids < 1) || (p_s1ap_served_gummei_info->num_served_group_ids > MAX_NUM_SERVED_GROUP_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_group_ids] should be in range "
            "1 to MAX_NUM_SERVED_GROUP_ID. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_group_ids);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_group_ids; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_served_group_id(
                &p_s1ap_served_gummei_info->served_group_id[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_served_gummei_info->num_served_mmecs, p_src + *p_length_read, "num_served_mmecs");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_mmecs < 1) || (p_s1ap_served_gummei_info->num_served_mmecs > MAX_NUM_SERVED_MMEC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_mmecs] should be in range "
            "1 to MAX_NUM_SERVED_MMEC. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_mmecs);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_mmecs; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_served_mmec(
                &p_s1ap_served_gummei_info->served_mmec[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_mme_info
(
    s1ap_mme_info_t *p_s1ap_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_mme_info, 0, sizeof(s1ap_mme_info_t));

    /* This function parses s1ap_mme_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_mme_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_s1ap_mme_info->bitmask & S1AP_MME_NAME_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_s1ap_mme_info->mme_name) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_mme_info->mme_name); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_mme_info->mme_name[loop], (void*)(p_src + *p_length_read), "mme_name[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_s1ap_mme_info->bitmask & S1AP_SERVED_GUMMEI_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_served_gummei_info(
        &p_s1ap_mme_info->served_gummei_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_mme_info->bitmask & S1AP_RELATIVE_MME_CAPACITY_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_mme_info->relative_mme_capacity, p_src + *p_length_read, "relative_mme_capacity");
    *p_length_read += sizeof(U8);
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_get_sctp_status_req, 0, sizeof(s1ap_oam_get_sctp_status_req_t));

    /* This function parses s1ap_oam_get_sctp_status_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_get_sctp_status_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_s1ap_oam_get_sctp_status_req->bitmask & S1AP_OAM_MME_STATUS_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_oam_mme_id_list_info(
        &p_s1ap_oam_get_sctp_status_req->s1ap_oam_mme_status_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_oam_get_sctp_status_resp
(
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_get_sctp_status_resp, 0, sizeof(s1ap_oam_get_sctp_status_resp_t));

    /* This function parses s1ap_oam_get_sctp_status_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_oam_get_sctp_status_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_get_sctp_status_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_get_sctp_status_resp->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_get_sctp_status_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_s1ap_oam_get_sctp_status_resp->response);
        return RRC_FAILURE;
    }

    if (p_s1ap_oam_get_sctp_status_resp->bitmask & S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_oam_mme_status_params_list_info(
        &p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_s1ap_oam_get_sctp_status_resp->bitmask & S1AP_OAM_MME_FAILED_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_s1ap_oam_mme_failed_list(
        &p_s1ap_oam_get_sctp_status_resp->mme_failed_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_timeval
(
    rrc_timeval_t *p_rrc_timeval,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_timeval, 0, sizeof(rrc_timeval_t));

    /* This function parses rrc_timeval */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_timeval->tv_sec, p_src + *p_length_read, "tv_sec");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_timeval->tv_usec, p_src + *p_length_read, "tv_usec");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_id_status_params
(
    s1ap_oam_mme_id_status_params_t *p_s1ap_oam_mme_id_status_params,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_id_status_params, 0, sizeof(s1ap_oam_mme_id_status_params_t));

    /* This function parses s1ap_oam_mme_id_status_params */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_id_status_params->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_oam_mme_id_status_params->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_mme_id_status_params->status > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_id_status_params->status] should be less than"
            " or equal to 9. Incorrect value %u received.", p_s1ap_oam_mme_id_status_params->status);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_oam_mme_id_status_params->in_streams, p_src + *p_length_read, "in_streams");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_oam_mme_id_status_params->out_streams, p_src + *p_length_read, "out_streams");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_s1ap_oam_mme_id_status_params->local_port, p_src + *p_length_read, "local_port");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_rrc_timeval(
        &p_s1ap_oam_mme_id_status_params->start_time,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_s1ap_oam_mme_id_status_params->primary_peer_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_id_status_params->primary_peer_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1ap_oam_mme_id_status_params->primary_peer_addr[loop], (void*)(p_src + *p_length_read), "primary_peer_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_status_params_list_info
(
    s1ap_oam_mme_status_params_list_info_t *p_s1ap_oam_mme_status_params_list_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_status_params_list_info, 0, sizeof(s1ap_oam_mme_status_params_list_info_t));

    /* This function parses s1ap_oam_mme_status_params_list_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_status_params_list_info->num_mme_id, p_src + *p_length_read, "num_mme_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_status_params_list_info->num_mme_id < 1) || (p_s1ap_oam_mme_status_params_list_info->num_mme_id > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_status_params_list_info->num_mme_id] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_mme_status_params_list_info->num_mme_id);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_status_params_list_info->num_mme_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_oam_mme_id_status_params(
                &p_s1ap_oam_mme_status_params_list_info->staus_params[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_oam_failed_mme_info
(
    s1ap_oam_failed_mme_info_t *p_s1ap_oam_failed_mme_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_failed_mme_info, 0, sizeof(s1ap_oam_failed_mme_info_t));

    /* This function parses s1ap_oam_failed_mme_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_failed_mme_info->mme_id, p_src + *p_length_read, "mme_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_failed_mme_info->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_s1ap_oam_mme_failed_list
(
    s1ap_oam_mme_failed_list_t *p_s1ap_oam_mme_failed_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_s1ap_oam_mme_failed_list, 0, sizeof(s1ap_oam_mme_failed_list_t));

    /* This function parses s1ap_oam_mme_failed_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_oam_mme_failed_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_failed_list->count < 1) || (p_s1ap_oam_mme_failed_list->count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_failed_list->count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_mme_failed_list->count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_failed_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_s1ap_oam_failed_mme_info(
                &p_s1ap_oam_mme_failed_list->failed_mme_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

#ifdef LINUX_PC_TEST
rrc_return_et
rrc_il_parse_test_sctp_message
(
    test_sctp_message_t *p_test_sctp_message,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

	memset_wrapper(p_test_sctp_message, 0, sizeof(test_sctp_message_t));

    rrc_cp_unpack_U32(&p_test_sctp_message->ulSocketID, p_src + *p_length_read, "ulSocketID");
    *p_length_read += sizeof(U32);	

	rrc_cp_unpack_U16(&p_test_sctp_message->usStreamID, p_src + *p_length_read, "usStreamID");
    *p_length_read += sizeof(U16);	

	l3_memcpy_wrapper(p_test_sctp_message->pucPayload,p_src + *p_length_read,length_left-*p_length_read);

	return(RRC_SUCCESS);
}


rrc_return_et
rrc_il_parse_test_x2ap_lksctp_message
(
    test_x2ap_lksctp_message_t *p_test_x2ap_lksctp_message,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    struct sockaddr_in *pstSockAddr = NULL;
    *p_length_read = 0;

    memset_wrapper(p_test_x2ap_lksctp_message, 0, sizeof(test_x2ap_lksctp_message_t));

    rrc_cp_unpack_U32(&p_test_x2ap_lksctp_message->ulSocketID, p_src + *p_length_read, "ulSocketID");
    *p_length_read += sizeof(U32);	

    rrc_cp_unpack_U16(&p_test_x2ap_lksctp_message->usStreamID, p_src + *p_length_read, "usStreamID");
    *p_length_read += sizeof(U16);	

    pstSockAddr = (struct sockaddr_in*)(p_src + *p_length_read);
    rrc_cp_unpack_U16(&p_test_x2ap_lksctp_message->usPort, &(pstSockAddr->sin_port), "sin_port");
    strncpy(p_test_x2ap_lksctp_message->aucIpv4, inet_ntoa(pstSockAddr->sin_addr), 16);
    *p_length_read += sizeof(struct sockaddr_storage);

    l3_memcpy_wrapper(p_test_x2ap_lksctp_message->pucPayload,p_src + *p_length_read, length_left-*p_length_read);

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_test_x2ap_close_sctp
(
	test_x2ap_close_sctp_t* p_test_x2ap_close_sctp,
	U8* p_src,
	S32 length_left,
	S32* p_length_read
)
{
	*p_length_read = 0;

	memset_wrapper(p_test_x2ap_close_sctp, 0, sizeof(test_x2ap_lksctp_message_t));

	rrc_cp_unpack_U32(&p_test_x2ap_close_sctp->ulSocketID, p_src + *p_length_read, "ulSocketID");
	*p_length_read += sizeof(U32);

	return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_s1ap_gb_status_resp
(
	s1ap_gb_status_resp_t* p_test_s1ap_gb_status_resp,
	U8* p_src,
	S32 length_left,
	S32* p_length_read
)
{
	*p_length_read = 0;

	memset_wrapper(p_test_s1ap_gb_status_resp, 0, sizeof(test_x2ap_lksctp_message_t));

	rrc_cp_unpack_U8(&p_test_s1ap_gb_status_resp->mme_count, p_src + *p_length_read, "mme_count");
	*p_length_read += sizeof(U8);

	rrc_counter_t loop;
	for (loop = 0; loop < p_test_s1ap_gb_status_resp->mme_count; loop++)
	{
		rrc_cp_unpack_U8(&p_test_s1ap_gb_status_resp->mme_info[loop].current_mme_state, p_src + *p_length_read, "current_mme_state");
		*p_length_read += sizeof(U8);
	
		rrc_cp_unpack_U8(&p_test_s1ap_gb_status_resp->mme_info[loop].mme_id, p_src + *p_length_read, "mme_id");
		*p_length_read += sizeof(U8);

		rrc_cp_unpack_U16(&p_test_s1ap_gb_status_resp->mme_info[loop].ue_count, p_src + *p_length_read, "ue_count");
		*p_length_read += sizeof(U16);

	}

	rrc_cp_unpack_U8(&p_test_s1ap_gb_status_resp->cell_count, p_src + *p_length_read, "cell_count");
	*p_length_read += sizeof(U8);

	rrc_cp_unpack_U8(&p_test_s1ap_gb_status_resp->s1ap_current_state, p_src + *p_length_read, "s1ap_current_state");
	*p_length_read += sizeof(U8);

	rrc_cp_unpack_U8(&p_test_s1ap_gb_status_resp->current_enb_mgr_state, p_src + *p_length_read, "current_enb_mgr_state");
	*p_length_read += sizeof(U8);

	return(RRC_SUCCESS);
}

#endif
