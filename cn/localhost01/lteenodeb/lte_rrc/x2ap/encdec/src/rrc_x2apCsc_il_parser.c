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
 *  File Description : The file rrc_x2apCsc_il_parser.c contains the definitions 
 *                     of rrc-x2apCsc interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_x2apCsc_il_parser.h"
#include "rrc_x2apCsc_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_tac
(
    x2ap_tac_t *p_x2ap_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2apOam_srvd_cell_info
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_neighbour_info
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_nbour_cell_arry
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2apOam_srvd_cell_arry
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_csc_cell_status_info_req
(
    x2ap_csc_cell_status_info_req_t *p_x2ap_csc_cell_status_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2ap_csc_cell_status_info_resp
(
    x2ap_csc_cell_status_info_resp_t *p_x2ap_csc_cell_status_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_x2ap_csc_cell_add
(
    x2ap_csc_cell_add_t *p_x2ap_csc_cell_add,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_add, 0, sizeof(x2ap_csc_cell_add_t));

    /* This function parses x2ap_csc_cell_add */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_add->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_add->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_add->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_add->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_add->eutran_cell_id, p_src + *p_length_read, "eutran_cell_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_csc_cell_add->phys_cell_id, p_src + *p_length_read, "phys_cell_id");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_csc_cell_add->dl_earfcn, p_src + *p_length_read, "dl_earfcn");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_csc_cell_add->ul_earfcn, p_src + *p_length_read, "ul_earfcn");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_il_parse_x2ap_broadcast_plmn(
        &p_x2ap_csc_cell_add->broadcast_plmn_info,
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
rrc_il_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_plmn_identity, 0, sizeof(x2ap_plmn_identity_t));

    /* This function parses x2ap_plmn_identity */

    if (*p_length_read + (S32)sizeof(p_x2ap_plmn_identity->plmn_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
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
rrc_il_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_broadcast_plmn, 0, sizeof(x2ap_broadcast_plmn_t));

    /* This function parses x2ap_broadcast_plmn */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_broadcast_plmn->num_bcast_plmn_id, p_src + *p_length_read, "num_bcast_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %u received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
                &p_x2ap_broadcast_plmn->plmn_identity[loop],
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

rrc_return_et
rrc_il_parse_x2ap_csc_cell_upd
(
    x2ap_csc_cell_upd_t *p_x2ap_csc_cell_upd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_upd, 0, sizeof(x2ap_csc_cell_upd_t));

    /* This function parses x2ap_csc_cell_upd */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_upd->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_upd->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_upd->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_upd->cell_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2apOam_srvd_cell_arry(
        &p_x2ap_csc_cell_upd->served_cell_to_upd,
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
rrc_il_parse_x2ap_tac
(
    x2ap_tac_t *p_x2ap_tac,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_tac, 0, sizeof(x2ap_tac_t));

    /* This function parses x2ap_tac */

    if (*p_length_read + (S32)sizeof(p_x2ap_tac->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_tac->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_tac->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
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
rrc_il_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_earfcn, 0, sizeof(x2ap_earfcn_t));

    /* This function parses x2ap_earfcn */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_earfcn->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_x2ap_earfcn->earfcn);
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
rrc_il_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_fdd_info, 0, sizeof(x2ap_fdd_info_t));

    /* This function parses x2ap_fdd_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->ul_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->dl_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_fdd_info->ul_trnsmission_bdwidth, p_src + *p_length_read, "ul_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_fdd_info->dl_trnsmission_bdwidth, p_src + *p_length_read, "dl_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
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
rrc_il_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_special_subframe_info, 0, sizeof(x2ap_special_subframe_info_t));

    /* This function parses x2ap_special_subframe_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->special_subframe_patterns, p_src + *p_length_read, "special_subframe_patterns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be less than"
            " or equal to 8. Incorrect value %u received.", p_x2ap_special_subframe_info->special_subframe_patterns);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->dl_cp, p_src + *p_length_read, "dl_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->dl_cp);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->ul_cp, p_src + *p_length_read, "ul_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->ul_cp);
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
rrc_il_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_tdd_info, 0, sizeof(x2ap_tdd_info_t));

    /* This function parses x2ap_tdd_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_tdd_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_tdd_info->trnsmission_bdwidth, p_src + *p_length_read, "trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_tdd_info->trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_tdd_info->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_tdd_info->subframe_assn);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_special_subframe_info(
        &p_x2ap_tdd_info->special_subframe_info,
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
rrc_il_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_eutra_mode, 0, sizeof(x2ap_choice_eutra_mode_t));

    /* This function parses x2ap_choice_eutra_mode */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_choice_eutra_mode->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_fdd_info(
        &p_x2ap_choice_eutra_mode->fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_tdd_info(
        &p_x2ap_choice_eutra_mode->tdd_info,
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
rrc_il_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_eutra_mode_info, 0, sizeof(x2ap_choice_eutra_mode_info_t));

    /* This function parses x2ap_choice_eutra_mode_info */

    if (RRC_FAILURE == rrc_il_parse_x2ap_choice_eutra_mode(
        &p_x2ap_choice_eutra_mode_info->choice_eutra_mode,
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
rrc_il_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_prach_configuration_info, 0, sizeof(x2ap_prach_configuration_info_t));

    /* This function parses x2ap_prach_configuration_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_prach_configuration_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_prach_configuration_info->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_x2ap_prach_configuration_info->root_sequence_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->zero_correlation_zone_config, p_src + *p_length_read, "zero_correlation_zone_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->high_speed_flag, p_src + *p_length_read, "high_speed_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_prach_configuration_info->high_speed_flag);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    if (p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->prach_config_index, p_src + *p_length_read, "prach_config_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_config_index);
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
rrc_il_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_subframe_allocation, 0, sizeof(x2ap_subframe_allocation_t));

    /* This function parses x2ap_subframe_allocation */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_subframe_allocation->oneframe) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_subframe_allocation->oneframe[loop], (void*)(p_src + *p_length_read), "oneframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_subframe_allocation->fourframe) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_subframe_allocation->fourframe[loop], (void*)(p_src + *p_length_read), "fourframe[]");
            *p_length_read += sizeof(U8);
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
rrc_il_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_mbsfn_info, 0, sizeof(x2ap_mbsfn_info_t));

    /* This function parses x2ap_mbsfn_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_info->radioframe_alloc_period, p_src + *p_length_read, "radioframe_alloc_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_info->radioframe_alloc_offset, p_src + *p_length_read, "radioframe_alloc_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_subframe_allocation(
        &p_x2ap_mbsfn_info->subframe_alloc,
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
rrc_il_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_mbsfn_subframe_info, 0, sizeof(x2ap_mbsfn_subframe_info_t));

    /* This function parses x2ap_mbsfn_subframe_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_subframe_info->num_mbsfn, p_src + *p_length_read, "num_mbsfn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %u received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_mbsfn_info(
                &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop],
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
rrc_il_parse_x2apOam_srvd_cell_info
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2apOam_srvd_cell_info, 0, sizeof(x2apOam_srvd_cell_info_t));

    /* This function parses x2apOam_srvd_cell_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2apOam_srvd_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2apOam_srvd_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2apOam_srvd_cell_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2apOam_srvd_cell_info->pci);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2apOam_srvd_cell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->cell_id > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2apOam_srvd_cell_info->cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_x2apOam_srvd_cell_info->cell_id);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_x2ap_tac(
        &p_x2apOam_srvd_cell_info->track_area_code,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_broadcast_plmn(
        &p_x2apOam_srvd_cell_info->broadcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_choice_eutra_mode_info(
        &p_x2apOam_srvd_cell_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2apOam_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2apOam_srvd_cell_info->num_antenna_port, p_src + *p_length_read, "num_antenna_port");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->num_antenna_port > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2apOam_srvd_cell_info->num_antenna_port] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2apOam_srvd_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }
    }

    if (p_x2apOam_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_prach_configuration_info(
        &p_x2apOam_srvd_cell_info->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2apOam_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2ap_mbsfn_subframe_info(
        &p_x2apOam_srvd_cell_info->mbsfn_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2apOam_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2apOam_srvd_cell_info->csg_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2apOam_srvd_cell_info->csg_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2apOam_srvd_cell_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
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
rrc_il_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_ecgi, 0, sizeof(x2ap_ecgi_t));

    /* This function parses x2ap_ecgi */

    if (RRC_FAILURE == rrc_il_parse_x2ap_plmn_identity(
        &p_x2ap_ecgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_x2ap_ecgi->eutran_cell_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_ecgi->eutran_cell_id[loop], (void*)(p_src + *p_length_read), "eutran_cell_id[]");
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
rrc_il_parse_x2ap_neighbour_info
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_neighbour_info, 0, sizeof(x2ap_neighbour_info_t));

    /* This function parses x2ap_neighbour_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_neighbour_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_il_parse_x2ap_ecgi(
        &p_x2ap_neighbour_info->ecgi,
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
    rrc_cp_unpack_U32(&p_x2ap_neighbour_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    if (RRC_FAILURE == rrc_il_parse_x2ap_earfcn(
        &p_x2ap_neighbour_info->earfcn,
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
rrc_il_parse_x2ap_nbour_cell_arry
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_nbour_cell_arry, 0, sizeof(x2ap_nbour_cell_arry_t));

    /* This function parses x2ap_nbour_cell_arry */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nbour_cell_arry->num_neighbour_cell, p_src + *p_length_read, "num_neighbour_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %u received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_neighbour_info(
                &p_x2ap_nbour_cell_arry->neighbour_info_arr[loop],
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
rrc_il_parse_x2apOam_srvd_cell_arry
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2apOam_srvd_cell_arry, 0, sizeof(x2apOam_srvd_cell_arry_t));

    /* This function parses x2apOam_srvd_cell_arry */

    if (RRC_FAILURE == rrc_il_parse_x2apOam_srvd_cell_info(
        &p_x2apOam_srvd_cell_arry->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_il_parse_x2ap_nbour_cell_arry(
        &p_x2apOam_srvd_cell_arry->nbour_cell_info,
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

rrc_return_et
rrc_il_parse_x2ap_csc_cell_del
(
    x2ap_csc_cell_del_t *p_x2ap_csc_cell_del,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_del, 0, sizeof(x2ap_csc_cell_del_t));

    /* This function parses x2ap_csc_cell_del */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_del->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_del->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_del->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_del->cell_index);
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
rrc_il_parse_x2ap_csc_cell_status_req
(
    x2ap_csc_cell_status_req_t *p_x2ap_csc_cell_status_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_status_req, 0, sizeof(x2ap_csc_cell_status_req_t));

    /* This function parses x2ap_csc_cell_status_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_csc_cell_status_req->num_served_cells, p_src + *p_length_read, "num_served_cells");
    *p_length_read += sizeof(U16);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_csc_cell_status_req->num_served_cells; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_csc_cell_status_info_req(
                &p_x2ap_csc_cell_status_req->cell_info[loop],
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
rrc_il_parse_x2ap_csc_cell_status_info_req
(
    x2ap_csc_cell_status_info_req_t *p_x2ap_csc_cell_status_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_status_info_req, 0, sizeof(x2ap_csc_cell_status_info_req_t));

    /* This function parses x2ap_csc_cell_status_info_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_status_info_req->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_status_info_req->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_status_info_req->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_status_info_req->cell_index);
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
rrc_il_parse_x2ap_csc_cell_status_resp
(
    x2ap_csc_cell_status_resp_t *p_x2ap_csc_cell_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_status_resp, 0, sizeof(x2ap_csc_cell_status_resp_t));

    /* This function parses x2ap_csc_cell_status_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_csc_cell_status_resp->num_served_cells, p_src + *p_length_read, "num_served_cells");
    *p_length_read += sizeof(U16);

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_csc_cell_status_resp->num_served_cells; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2ap_csc_cell_status_info_resp(
                &p_x2ap_csc_cell_status_resp->cell_status_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_status_resp->no_of_uecc_instances, p_src + *p_length_read, "no_of_uecc_instances");
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
rrc_il_parse_x2ap_csc_cell_status_info_resp
(
    x2ap_csc_cell_status_info_resp_t *p_x2ap_csc_cell_status_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2ap_csc_cell_status_info_resp, 0, sizeof(x2ap_csc_cell_status_info_resp_t));

    /* This function parses x2ap_csc_cell_status_info_resp */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_status_info_resp->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_status_info_resp->cell_index > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_status_info_resp->cell_index] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_csc_cell_status_info_resp->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_csc_cell_status_info_resp->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_csc_cell_status_info_resp->status > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_csc_cell_status_info_resp->status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_csc_cell_status_info_resp->status);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

