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
 *  File Description : The file rrc_rlc_il_parser.c contains the definitions 
 *                     of rrc-rlc interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_rlc_il_parser.h"
#include "rrc_rlc_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_rrc_rlc_qci_info
(
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_tx_bit_rate_info
(
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_queue_size_info
(
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_dc_bearer_type_info
(
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_create_tx_um_rlc
(
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_rx_bit_rate_info
(
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_create_rx_um_rlc
(
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_create_tx_rx_um_rlc
(
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_create_tx_rx_am_rlc
(
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_ue_bit_rate_info
(
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_sps_dl_setup_info
(
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_sps_dl_info
(
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_sps_ul_setup_info
(
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_sps_ul_info
(
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_sps_config
(
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_create_entity_error
(
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_delete_tx_um_rlc
(
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_delete_rx_um_rlc
(
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_delete_tx_rx_um_rlc
(
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_delete_tx_rx_am_rlc
(
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_tx_um_rlc
(
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_rx_um_rlc
(
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_tx_rx_um_rlc
(
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_tx_rx_am_rlc
(
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_delete_entity_error
(
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_entity_error
(
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_lc_state
(
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_entity_lcid
(
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_rlc_entity_lc_id_error
(
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rlc_communication_info
(
    rlc_communication_info_t *p_rlc_communication_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_rrc_rlc_create_ue_entity_req
(
    rrc_rlc_create_ue_entity_req_t *p_rrc_rlc_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_ue_entity_req, 0, sizeof(rrc_rlc_create_ue_entity_req_t));

    /* This function parses rrc_rlc_create_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_ue_entity_req->rnti, p_src + *p_length_read, "rnti");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_CREATE_TX_UM_RLC_ENTITY:
                /* rrc_rlc_create_tx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_tx_um_rlc(
                    &p_rrc_rlc_create_ue_entity_req->create_tx_um_rlc[p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_CREATE_RX_UM_RLC_ENTITY:
                /* rrc_rlc_create_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_rx_um_rlc(
                    &p_rrc_rlc_create_ue_entity_req->create_rx_um_rlc[p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_CREATE_TX_RX_UM_RLC_ENTITY:
                /* rrc_rlc_create_tx_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_tx_rx_um_rlc(
                    &p_rrc_rlc_create_ue_entity_req->create_tx_rx_um_rlc[p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_CREATE_TX_RX_AM_RLC_ENTITY:
                /* rrc_rlc_create_tx_rx_am_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc >= (S32)ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_rx_am_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_tx_rx_am_rlc(
                    &p_rrc_rlc_create_ue_entity_req->create_tx_rx_am_rlc[p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_UE_BIT_RATE_INFO:
                /* rrc_rlc_ue_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_ue_bit_rate_info(
                    &p_rrc_rlc_create_ue_entity_req->rlc_ue_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_req->bitmask |= RRC_RLC_UE_BIT_RATE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_SPS_CONFIG_TAG:
                /* rrc_rlc_sps_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_sps_config(
                    &p_rrc_rlc_create_ue_entity_req->sps_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_req->bitmask |= RRC_RLC_SPS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_qci_info
(
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_qci_info, 0, sizeof(rrc_rlc_qci_info_t));

    /* This function parses rrc_rlc_qci_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_qci_info->qci, p_src + *p_length_read, "qci");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rlc_qci_info->qci < 1) || (p_rrc_rlc_qci_info->qci > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_qci_info->qci] should be in range "
            "1 to 9. Incorrect value %u received.", p_rrc_rlc_qci_info->qci);
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
rrc_il_parse_rrc_rlc_tx_bit_rate_info
(
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_tx_bit_rate_info, 0, sizeof(rrc_rlc_tx_bit_rate_info_t));

    /* This function parses rrc_rlc_tx_bit_rate_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlc_tx_bit_rate_info->bit_rate_tx, p_src + *p_length_read, "bit_rate_tx");
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
rrc_il_parse_rrc_rlc_queue_size_info
(
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_queue_size_info, 0, sizeof(rrc_rlc_queue_size_info_t));

    /* This function parses rrc_rlc_queue_size_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlc_queue_size_info->rlc_queue_size, p_src + *p_length_read, "rlc_queue_size");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_queue_size_info->rlc_queue_size > 28311552))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_queue_size_info->rlc_queue_size] should be less than"
            " or equal to 28311552. Incorrect value %u received.", p_rrc_rlc_queue_size_info->rlc_queue_size);
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
rrc_il_parse_rrc_rlc_dc_bearer_type_info
(
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_dc_bearer_type_info, 0, sizeof(rrc_rlc_dc_bearer_type_info));

    /* This function parses rrc_rlc_dc_bearer_type_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_dc_bearer_type_info->dc_bearer_type, p_src + *p_length_read, "dc_bearer_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_dc_bearer_type_info->dc_bearer_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_dc_bearer_type_info->dc_bearer_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_dc_bearer_type_info->dc_bearer_type);
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
rrc_il_parse_rrc_rlc_create_tx_um_rlc
(
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_tx_um_rlc, 0, sizeof(rrc_rlc_create_tx_um_rlc_t));

    /* This function parses rrc_rlc_create_tx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_create_tx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_um_rlc->sn_field_length, p_src + *p_length_read, "sn_field_length");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_create_tx_um_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_um_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_TX_BIT_RATE_INFO:
                /* rrc_rlc_tx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_tx_bit_rate_info(
                    &p_rrc_rlc_create_tx_um_rlc->rrc_rlc_tx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_um_rlc->optional_elems_present |= RRC_RLC_TX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_QUEUE_SIZE_INFO:
                /* rrc_rlc_queue_size_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_queue_size_info(
                    &p_rrc_rlc_create_tx_um_rlc->rrc_rlc_queue_size_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_um_rlc->optional_elems_present |= RRC_RLC_QUEUE_SIZE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_DC_BEARER_TYPE_INFO:
                /* rrc_rlc_dc_bearer_type_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_dc_bearer_type_info(
                    &p_rrc_rlc_create_tx_um_rlc->dc_bearer_type_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_um_rlc->optional_elems_present |= RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_UM;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_rx_bit_rate_info
(
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_rx_bit_rate_info, 0, sizeof(rrc_rlc_rx_bit_rate_info_t));

    /* This function parses rrc_rlc_rx_bit_rate_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlc_rx_bit_rate_info->bit_rate_rx, p_src + *p_length_read, "bit_rate_rx");
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
rrc_il_parse_rrc_rlc_create_rx_um_rlc
(
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_rx_um_rlc, 0, sizeof(rrc_rlc_create_rx_um_rlc_t));

    /* This function parses rrc_rlc_create_rx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_create_rx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_rx_um_rlc->sn_field_length, p_src + *p_length_read, "sn_field_length");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_rx_um_rlc->t_reordering, p_src + *p_length_read, "t_reordering");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_create_rx_um_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_rx_um_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_RX_BIT_RATE_INFO:
                /* rrc_rlc_rx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_rx_bit_rate_info(
                    &p_rrc_rlc_create_rx_um_rlc->rrc_rlc_rx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_rx_um_rlc->optional_elems_present |= RRC_RLC_RX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_DC_BEARER_TYPE_INFO:
                /* rrc_rlc_dc_bearer_type_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_dc_bearer_type_info(
                    &p_rrc_rlc_create_rx_um_rlc->dc_bearer_type_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_rx_um_rlc->optional_elems_present |= RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_RX_UM;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_create_tx_rx_um_rlc
(
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_tx_rx_um_rlc, 0, sizeof(rrc_rlc_create_tx_rx_um_rlc_t));

    /* This function parses rrc_rlc_create_tx_rx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_create_tx_rx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_tx, p_src + *p_length_read, "sn_field_length_tx");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_rx, p_src + *p_length_read, "sn_field_length_rx");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_um_rlc->t_reordering, p_src + *p_length_read, "t_reordering");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_TX_BIT_RATE_INFO:
                /* rrc_rlc_tx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_tx_bit_rate_info(
                    &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_tx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_TX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_RX_BIT_RATE_INFO:
                /* rrc_rlc_rx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_rx_bit_rate_info(
                    &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_rx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_RX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_QUEUE_SIZE_INFO:
                /* rrc_rlc_queue_size_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_queue_size_info(
                    &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_queue_size_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_QUEUE_SIZE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_DC_BEARER_TYPE_INFO:
                /* rrc_rlc_dc_bearer_type_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_dc_bearer_type_info(
                    &p_rrc_rlc_create_tx_rx_um_rlc->dc_bearer_type_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_UM;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_create_tx_rx_am_rlc
(
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_tx_rx_am_rlc, 0, sizeof(rrc_rlc_create_tx_rx_am_rlc_t));

    /* This function parses rrc_rlc_create_tx_rx_am_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_create_tx_rx_am_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_am_rlc->t_poll_retransmission, p_src + *p_length_read, "t_poll_retransmission");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_am_rlc->poll_pdu, p_src + *p_length_read, "poll_pdu");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_am_rlc->poll_byte, p_src + *p_length_read, "poll_byte");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_am_rlc->max_re_tx_thrsh_ld, p_src + *p_length_read, "max_re_tx_thrsh_ld");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_am_rlc->t_reordering, p_src + *p_length_read, "t_reordering");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_tx_rx_am_rlc->t_status_prohibit, p_src + *p_length_read, "t_status_prohibit");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_TX_BIT_RATE_INFO:
                /* rrc_rlc_tx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_tx_bit_rate_info(
                    &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_tx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_TX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_RX_BIT_RATE_INFO:
                /* rrc_rlc_rx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_rx_bit_rate_info(
                    &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_rx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_RX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_QUEUE_SIZE_INFO:
                /* rrc_rlc_queue_size_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_queue_size_info(
                    &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_queue_size_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_QUEUE_SIZE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_DC_BEARER_TYPE_INFO:
                /* rrc_rlc_dc_bearer_type_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_dc_bearer_type_info(
                    &p_rrc_rlc_create_tx_rx_am_rlc->dc_bearer_type_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_AM;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_ue_bit_rate_info
(
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_ue_bit_rate_info, 0, sizeof(rrc_rlc_ue_bit_rate_info_t));

    /* This function parses rrc_rlc_ue_bit_rate_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlc_ue_bit_rate_info->bit_rate_tx, p_src + *p_length_read, "bit_rate_tx");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_rlc_ue_bit_rate_info->bit_rate_rx, p_src + *p_length_read, "bit_rate_rx");
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
rrc_il_parse_rrc_rlc_sps_dl_setup_info
(
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_sps_dl_setup_info, 0, sizeof(rrc_rlc_sps_dl_setup_info_t));

    /* This function parses rrc_rlc_sps_dl_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl, p_src + *p_length_read, "semi_persistent_sched_interval_dl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl);
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
rrc_il_parse_rrc_rlc_sps_dl_info
(
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_sps_dl_info, 0, sizeof(rrc_rlc_sps_dl_info_t));

    /* This function parses rrc_rlc_sps_dl_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_sps_dl_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_dl_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_dl_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_sps_dl_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_SPS_DL_SETUP_INFO_TAG:
                /* rrc_rlc_sps_dl_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_sps_dl_setup_info(
                    &p_rrc_rlc_sps_dl_info->sps_dl_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_sps_dl_info->optional_elems_present |= RRC_RLC_SPS_DL_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_sps_ul_setup_info
(
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_sps_ul_setup_info, 0, sizeof(rrc_rlc_sps_ul_setup_info_t));

    /* This function parses rrc_rlc_sps_ul_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul, p_src + *p_length_read, "semi_persistent_sched_interval_ul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul);
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
rrc_il_parse_rrc_rlc_sps_ul_info
(
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_sps_ul_info, 0, sizeof(rrc_rlc_sps_ul_info_t));

    /* This function parses rrc_rlc_sps_ul_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_sps_ul_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_ul_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_ul_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_sps_ul_info->request_type);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_SPS_UL_SETUP_INFO_TAG:
                /* rrc_rlc_sps_ul_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_sps_ul_setup_info(
                    &p_rrc_rlc_sps_ul_info->sps_ul_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_sps_ul_info->optional_elems_present |= RRC_RLC_SPS_UL_SETUP_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_sps_config
(
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_sps_config, 0, sizeof(rrc_rlc_sps_config_t));

    /* This function parses rrc_rlc_sps_config */

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_SPS_DL_INFO_TAG:
                /* rrc_rlc_sps_dl_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_sps_dl_info(
                    &p_rrc_rlc_sps_config->sps_dl_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_sps_config->optional_elems_present |= RRC_RLC_SPS_DL_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_SPS_UL_INFO_TAG:
                /* rrc_rlc_sps_ul_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_sps_ul_info(
                    &p_rrc_rlc_sps_config->sps_ul_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_sps_config->optional_elems_present |= RRC_RLC_SPS_UL_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_create_ue_entity_cnf
(
    rrc_rlc_create_ue_entity_cnf_t *p_rrc_rlc_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_ue_entity_cnf, 0, sizeof(rrc_rlc_create_ue_entity_cnf_t));

    /* This function parses rrc_rlc_create_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_create_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_create_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_create_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_CREATE_ENTITY_ERROR:
                /* rrc_rlc_create_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error >= (S32)ARRSIZE(p_rrc_rlc_create_ue_entity_cnf->create_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_entity_error(
                    &p_rrc_rlc_create_ue_entity_cnf->create_error_entities[p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_create_entity_error
(
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_entity_error, 0, sizeof(rrc_rlc_create_entity_error_t));

    /* This function parses rrc_rlc_create_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_create_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_entity_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_ue_entity_req
(
    rrc_rlc_reconfig_ue_entity_req_t *p_rrc_rlc_reconfig_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_ue_entity_req, 0, sizeof(rrc_rlc_reconfig_ue_entity_req_t));

    /* This function parses rrc_rlc_reconfig_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_CREATE_TX_UM_RLC_ENTITY:
                /* rrc_rlc_create_tx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_tx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->create_tx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_CREATE_RX_UM_RLC_ENTITY:
                /* rrc_rlc_create_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_rx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->create_rx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_CREATE_TX_RX_UM_RLC_ENTITY:
                /* rrc_rlc_create_tx_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_tx_rx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_CREATE_TX_RX_AM_RLC_ENTITY:
                /* rrc_rlc_create_tx_rx_am_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_am_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_tx_rx_am_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_am_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_DELETE_TX_UM_RLC_ENTITY:
                /* rrc_rlc_delete_tx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_delete_tx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->delete_tx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_DELETE_RX_UM_RLC_ENTITY:
                /* rrc_rlc_delete_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_delete_rx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->delete_rx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_DELETE_TX_RX_UM_RLC_ENTITY:
                /* rrc_rlc_delete_tx_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_delete_tx_rx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_DELETE_TX_RX_AM_RLC_ENTITY:
                /* rrc_rlc_delete_tx_rx_am_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_am_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_delete_tx_rx_am_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_am_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_RECONFIG_TX_UM_RLC_ENTITY:
                /* rrc_rlc_reconfig_tx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_reconfig_tx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_RECONFIG_RX_UM_RLC_ENTITY:
                /* rrc_rlc_reconfig_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_reconfig_rx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->reconfig_rx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_RECONFIG_TX_RX_UM_RLC_ENTITY:
                /* rrc_rlc_reconfig_tx_rx_um_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_um_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_reconfig_tx_rx_um_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_um_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_RECONFIG_TX_RX_AM_RLC_ENTITY:
                /* rrc_rlc_reconfig_tx_rx_am_rlc TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_am_rlc))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_reconfig_tx_rx_am_rlc(
                    &p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_am_rlc[p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_UE_BIT_RATE_INFO:
                /* rrc_rlc_ue_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_ue_bit_rate_info(
                    &p_rrc_rlc_reconfig_ue_entity_req->rlc_ue_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->bitmask |= RRC_RLC_RECONFIG_UE_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_SPS_CONFIG_TAG:
                /* rrc_rlc_sps_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_sps_config(
                    &p_rrc_rlc_reconfig_ue_entity_req->sps_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_req->bitmask |= RRC_RLC_RECONFIG_UE_SPS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_delete_tx_um_rlc
(
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_tx_um_rlc, 0, sizeof(rrc_rlc_delete_tx_um_rlc_t));

    /* This function parses rrc_rlc_delete_tx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_delete_tx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
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
rrc_il_parse_rrc_rlc_delete_rx_um_rlc
(
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_rx_um_rlc, 0, sizeof(rrc_rlc_delete_rx_um_rlc_t));

    /* This function parses rrc_rlc_delete_rx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_delete_rx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
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
rrc_il_parse_rrc_rlc_delete_tx_rx_um_rlc
(
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_tx_rx_um_rlc, 0, sizeof(rrc_rlc_delete_tx_rx_um_rlc_t));

    /* This function parses rrc_rlc_delete_tx_rx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_delete_tx_rx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
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
rrc_il_parse_rrc_rlc_delete_tx_rx_am_rlc
(
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_tx_rx_am_rlc, 0, sizeof(rrc_rlc_delete_tx_rx_am_rlc_t));

    /* This function parses rrc_rlc_delete_tx_rx_am_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_delete_tx_rx_am_rlc->lc_id, p_src + *p_length_read, "lc_id");
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
rrc_il_parse_rrc_rlc_reconfig_tx_um_rlc
(
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_tx_um_rlc, 0, sizeof(rrc_rlc_reconfig_tx_um_rlc_t));

    /* This function parses rrc_rlc_reconfig_tx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_reconfig_tx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_TX_BIT_RATE_INFO:
                /* rrc_rlc_tx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_tx_bit_rate_info(
                    &p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_tx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present |= RRC_RLC_TX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_QUEUE_SIZE_INFO:
                /* rrc_rlc_queue_size_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_queue_size_info(
                    &p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_queue_size_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present |= RRC_RLC_QUEUE_SIZE_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_reconfig_rx_um_rlc
(
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_rx_um_rlc, 0, sizeof(rrc_rlc_reconfig_rx_um_rlc_t));

    /* This function parses rrc_rlc_reconfig_rx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_reconfig_rx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_rx_um_rlc->t_reordering, p_src + *p_length_read, "t_reordering");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_reconfig_rx_um_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_rx_um_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_RX_BIT_RATE_INFO:
                /* rrc_rlc_rx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_rx_bit_rate_info(
                    &p_rrc_rlc_reconfig_rx_um_rlc->rrc_rlc_rx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_rx_um_rlc->optional_elems_present |= RRC_RLC_RX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_reconfig_tx_rx_um_rlc
(
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_tx_rx_um_rlc, 0, sizeof(rrc_rlc_reconfig_tx_rx_um_rlc_t));

    /* This function parses rrc_rlc_reconfig_tx_rx_um_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_reconfig_tx_rx_um_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_um_rlc->t_reordering, p_src + *p_length_read, "t_reordering");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_TX_BIT_RATE_INFO:
                /* rrc_rlc_tx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_tx_bit_rate_info(
                    &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_tx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_TX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_RX_BIT_RATE_INFO:
                /* rrc_rlc_rx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_rx_bit_rate_info(
                    &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_rx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_RX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_QUEUE_SIZE_INFO:
                /* rrc_rlc_queue_size_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_queue_size_info(
                    &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_queue_size_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present |= RRC_RLC_QUEUE_SIZE_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_reconfig_tx_rx_am_rlc
(
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_tx_rx_am_rlc, 0, sizeof(rrc_rlc_reconfig_tx_rx_am_rlc_t));

    /* This function parses rrc_rlc_reconfig_tx_rx_am_rlc */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_reconfig_tx_rx_am_rlc->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_am_rlc->t_poll_retransmission, p_src + *p_length_read, "t_poll_retransmission");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_pdu, p_src + *p_length_read, "poll_pdu");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_byte, p_src + *p_length_read, "poll_byte");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_am_rlc->max_re_tx_thrsh_ld, p_src + *p_length_read, "max_re_tx_thrsh_ld");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_am_rlc->t_reordering, p_src + *p_length_read, "t_reordering");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_tx_rx_am_rlc->t_status_prohibit, p_src + *p_length_read, "t_status_prohibit");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_QCI_INFO:
                /* rrc_rlc_qci_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_qci_info(
                    &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_qci_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_QCI_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_TX_BIT_RATE_INFO:
                /* rrc_rlc_tx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_tx_bit_rate_info(
                    &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_tx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_TX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_RX_BIT_RATE_INFO:
                /* rrc_rlc_rx_bit_rate_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_rx_bit_rate_info(
                    &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_rx_bit_rate_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_RX_BIT_RATE_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_RLC_QUEUE_SIZE_INFO:
                /* rrc_rlc_queue_size_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_queue_size_info(
                    &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_queue_size_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present |= RRC_RLC_QUEUE_SIZE_INFO_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_reconfig_ue_entity_cnf
(
    rrc_rlc_reconfig_ue_entity_cnf_t *p_rrc_rlc_reconfig_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_ue_entity_cnf, 0, sizeof(rrc_rlc_reconfig_ue_entity_cnf_t));

    /* This function parses rrc_rlc_reconfig_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_reconfig_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_reconfig_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_reconfig_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_CREATE_ENTITY_ERROR:
                /* rrc_rlc_create_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->create_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_create_entity_error(
                    &p_rrc_rlc_reconfig_ue_entity_cnf->create_error_entities[p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_DELETE_ENTITY_ERROR:
                /* rrc_rlc_delete_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->delete_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_delete_entity_error(
                    &p_rrc_rlc_reconfig_ue_entity_cnf->delete_error_entities[p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_RLC_RECONFIG_ENTITY_ERROR:
                /* rrc_rlc_reconfig_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error >= (S32)ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->reconfig_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_reconfig_entity_error(
                    &p_rrc_rlc_reconfig_ue_entity_cnf->reconfig_error_entities[p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_delete_entity_error
(
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_entity_error, 0, sizeof(rrc_rlc_delete_entity_error_t));

    /* This function parses rrc_rlc_delete_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_delete_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_delete_entity_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_il_parse_rrc_rlc_reconfig_entity_error
(
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_reconfig_entity_error, 0, sizeof(rrc_rlc_reconfig_entity_error_t));

    /* This function parses rrc_rlc_reconfig_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_reconfig_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_reconfig_entity_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_rlc_delete_ue_entity_req
(
    rrc_rlc_delete_ue_entity_req_t *p_rrc_rlc_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_ue_entity_req, 0, sizeof(rrc_rlc_delete_ue_entity_req_t));

    /* This function parses rrc_rlc_delete_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_delete_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_rlc_delete_ue_entity_cnf
(
    rrc_rlc_delete_ue_entity_cnf_t *p_rrc_rlc_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_ue_entity_cnf, 0, sizeof(rrc_rlc_delete_ue_entity_cnf_t));

    /* This function parses rrc_rlc_delete_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_delete_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_delete_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_delete_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_delete_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_delete_ue_entity_cnf->response);
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
rrc_il_parse_rrc_rlc_re_establish_ue_entity_req
(
    rrc_rlc_re_establish_ue_entity_req_t *p_rrc_rlc_re_establish_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_re_establish_ue_entity_req, 0, sizeof(rrc_rlc_re_establish_ue_entity_req_t));

    /* This function parses rrc_rlc_re_establish_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_re_establish_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_ENTITY_LCID:
                /* rrc_rlc_entity_lcid TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id >= (S32)ARRSIZE(p_rrc_rlc_re_establish_ue_entity_req->entity_lcids))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_entity_lcid(
                    &p_rrc_rlc_re_establish_ue_entity_req->entity_lcids[p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_lc_state
(
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_lc_state, 0, sizeof(rrc_rlc_lc_state_t));

    /* This function parses rrc_rlc_lc_state */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_lc_state->state, p_src + *p_length_read, "state");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_lc_state->state > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_lc_state->state] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_lc_state->state);
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
rrc_il_parse_rrc_rlc_entity_lcid
(
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_entity_lcid, 0, sizeof(rrc_rlc_entity_lcid_t));

    /* This function parses rrc_rlc_entity_lcid */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_entity_lcid->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_LC_STATE:
                /* rrc_rlc_lc_state TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_lc_state(
                    &p_rrc_rlc_entity_lcid->lc_state,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_entity_lcid->bitmask |= RRC_RLC_LC_STATE_PRESENT;

                *p_length_read += length;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_re_establish_ue_entity_cnf
(
    rrc_rlc_re_establish_ue_entity_cnf_t *p_rrc_rlc_re_establish_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_re_establish_ue_entity_cnf, 0, sizeof(rrc_rlc_re_establish_ue_entity_cnf_t));

    /* This function parses rrc_rlc_re_establish_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_re_establish_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_re_establish_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_re_establish_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_re_establish_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_re_establish_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_ENTITY_LC_ID_ERROR:
                /* rrc_rlc_entity_lc_id_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity >= (S32)ARRSIZE(p_rrc_rlc_re_establish_ue_entity_cnf->error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_entity_lc_id_error(
                    &p_rrc_rlc_re_establish_ue_entity_cnf->error_entities[p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_entity_lc_id_error
(
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_entity_lc_id_error, 0, sizeof(rrc_rlc_entity_lc_id_error_t));

    /* This function parses rrc_rlc_entity_lc_id_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_entity_lc_id_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_entity_lc_id_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rlc_ue_entity_error_ind
(
    rlc_ue_entity_error_ind_t *p_rlc_ue_entity_error_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rlc_ue_entity_error_ind, 0, sizeof(rlc_ue_entity_error_ind_t));

    /* This function parses rlc_ue_entity_error_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rlc_ue_entity_error_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rlc_ue_entity_error_ind->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_rlc_change_crnti_req
(
    rrc_rlc_change_crnti_req_t *p_rrc_rlc_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_change_crnti_req, 0, sizeof(rrc_rlc_change_crnti_req_t));

    /* This function parses rrc_rlc_change_crnti_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_change_crnti_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_change_crnti_req->old_crnti, p_src + *p_length_read, "old_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_change_crnti_req->new_crnti, p_src + *p_length_read, "new_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_rlc_change_crnti_cnf
(
    rrc_rlc_change_crnti_cnf_t *p_rrc_rlc_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_change_crnti_cnf, 0, sizeof(rrc_rlc_change_crnti_cnf_t));

    /* This function parses rrc_rlc_change_crnti_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_change_crnti_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_change_crnti_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_change_crnti_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_change_crnti_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_change_crnti_cnf->response);
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
rrc_il_parse_rrc_rlc_re_establish_ue_entity_complete_ind
(
    rrc_rlc_re_establish_ue_entity_complete_ind_t *p_rrc_rlc_re_establish_ue_entity_complete_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_re_establish_ue_entity_complete_ind, 0, sizeof(rrc_rlc_re_establish_ue_entity_complete_ind_t));

    /* This function parses rrc_rlc_re_establish_ue_entity_complete_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_re_establish_ue_entity_complete_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    /* Parse TLVs */

    /* Check whether size is enough to store TL */
    while ((length_left - *p_length_read) >= (S32)(PUP_TL_LENGTH))
    {
        U16 tag = RRC_NULL, length = RRC_NULL;

        /* Unpack tag */
        rrc_cp_unpack_U16(&tag, p_src + *p_length_read, "<tag>");
        *p_length_read += sizeof(U16);

        /* Unpack length */
        rrc_cp_unpack_U16(&length, p_src + *p_length_read, "<length>");
        *p_length_read += sizeof(U16);

        /* Check whether length is enough to store TLV */
        if ((length < (S32)PUP_TL_LENGTH) || ((length - (S32)PUP_TL_LENGTH) > (length_left - *p_length_read)))
        {
            RRC_TRACE(RRC_WARNING, "Received incorrect length %u. Stopping parsing..",length);
            return(RRC_FAILURE);
        }

        length -= PUP_TL_LENGTH;

        switch(tag)
        {
            case RRC_RLC_ENTITY_LCID:
                /* rrc_rlc_entity_lcid TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id >= (S32)ARRSIZE(p_rrc_rlc_re_establish_ue_entity_complete_ind->entity_lcids))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_rlc_entity_lcid(
                    &p_rrc_rlc_re_establish_ue_entity_complete_ind->entity_lcids[p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id++;

                *p_length_read += length_read;

                break;
            default:
                RRC_TRACE(RRC_WARNING, "Received unknown tag %u", tag);
                /* Unknown tag, move to length bytes */
                *p_length_read += length;

                break;
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
rrc_il_parse_rrc_rlc_re_establish_ue_entity_complete_cnf
(
    rrc_rlc_re_establish_ue_entity_complete_cnf_t *p_rrc_rlc_re_establish_ue_entity_complete_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_re_establish_ue_entity_complete_cnf, 0, sizeof(rrc_rlc_re_establish_ue_entity_complete_cnf_t));

    /* This function parses rrc_rlc_re_establish_ue_entity_complete_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_re_establish_ue_entity_complete_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_re_establish_ue_entity_complete_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_re_establish_ue_entity_complete_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_re_establish_ue_entity_complete_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_re_establish_ue_entity_complete_cnf->response);
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
rrc_il_parse_rrc_rlc_create_mbms_area_entity_req
(
    rrc_rlc_create_mbms_area_entity_req_t *p_rrc_rlc_create_mbms_area_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_mbms_area_entity_req, 0, sizeof(rrc_rlc_create_mbms_area_entity_req_t));

    /* This function parses rrc_rlc_create_mbms_area_entity_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_create_mbms_area_entity_req->num_of_area, p_src + *p_length_read, "num_of_area");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rlc_create_mbms_area_entity_req->num_of_area < 1) || (p_rrc_rlc_create_mbms_area_entity_req->num_of_area > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_create_mbms_area_entity_req->num_of_area] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_rlc_create_mbms_area_entity_req->num_of_area);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrc_rlc_create_mbms_area_entity_req->num_of_area * sizeof(p_rrc_rlc_create_mbms_area_entity_req->area_id[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rlc_create_mbms_area_entity_req->num_of_area; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_rlc_create_mbms_area_entity_req->area_id[loop], (void*)(p_src + *p_length_read), "area_id[]");
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
rrc_il_parse_rrc_rlc_create_mbms_area_entity_cnf
(
    rrc_rlc_create_mbms_area_entity_cnf_t *p_rrc_rlc_create_mbms_area_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_create_mbms_area_entity_cnf, 0, sizeof(rrc_rlc_create_mbms_area_entity_cnf_t));

    /* This function parses rrc_rlc_create_mbms_area_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_create_mbms_area_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_create_mbms_area_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_create_mbms_area_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_create_mbms_area_entity_cnf->response_code);
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
rrc_il_parse_rrc_rlc_delete_mbms_area_entity_req
(
    rrc_rlc_delete_mbms_area_entity_req_t *p_rrc_rlc_delete_mbms_area_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_mbms_area_entity_req, 0, sizeof(rrc_rlc_delete_mbms_area_entity_req_t));

    /* This function parses rrc_rlc_delete_mbms_area_entity_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_delete_mbms_area_entity_req->num_of_area, p_src + *p_length_read, "num_of_area");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rlc_delete_mbms_area_entity_req->num_of_area < 1) || (p_rrc_rlc_delete_mbms_area_entity_req->num_of_area > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_delete_mbms_area_entity_req->num_of_area] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_rlc_delete_mbms_area_entity_req->num_of_area);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_rrc_rlc_delete_mbms_area_entity_req->num_of_area * sizeof(p_rrc_rlc_delete_mbms_area_entity_req->area_id[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rlc_delete_mbms_area_entity_req->num_of_area; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_rlc_delete_mbms_area_entity_req->area_id[loop], (void*)(p_src + *p_length_read), "area_id[]");
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
rrc_il_parse_rrc_rlc_delete_mbms_area_entity_cnf
(
    rrc_rlc_delete_mbms_area_entity_cnf_t *p_rrc_rlc_delete_mbms_area_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_delete_mbms_area_entity_cnf, 0, sizeof(rrc_rlc_delete_mbms_area_entity_cnf_t));

    /* This function parses rrc_rlc_delete_mbms_area_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_delete_mbms_area_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_delete_mbms_area_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_delete_mbms_area_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_delete_mbms_area_entity_cnf->response_code);
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
rrc_il_parse_rrc_rlc_config_cell_req
(
    rrc_rlc_config_cell_req_t *p_rrc_rlc_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_config_cell_req, 0, sizeof(rrc_rlc_config_cell_req_t));

    /* This function parses rrc_rlc_config_cell_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_rlc_config_cell_req->Cell_index, p_src + *p_length_read, "Cell_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_config_cell_req->Cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_config_cell_req->Cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_rlc_config_cell_req->Cell_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_rlc_communication_info(
        &p_rrc_rlc_config_cell_req->rlc_communication_info,
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
rrc_il_parse_rlc_communication_info
(
    rlc_communication_info_t *p_rlc_communication_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rlc_communication_info, 0, sizeof(rlc_communication_info_t));

    /* This function parses rlc_communication_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rlc_communication_info->pdcpInstance, p_src + *p_length_read, "pdcpInstance");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rlc_communication_info->pdcpRxPort, p_src + *p_length_read, "pdcpRxPort");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_rlc_communication_info->pdcpIpAddr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rlc_communication_info->pdcpIpAddr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rlc_communication_info->pdcpIpAddr[loop], (void*)(p_src + *p_length_read), "pdcpIpAddr[]");
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
rrc_il_parse_rrc_rlc_config_cell_cnf
(
    rrc_rlc_config_cell_cnf_t *p_rrc_rlc_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_rlc_config_cell_cnf, 0, sizeof(rrc_rlc_config_cell_cnf_t));

    /* This function parses rrc_rlc_config_cell_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_config_cell_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_rlc_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_rlc_config_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

