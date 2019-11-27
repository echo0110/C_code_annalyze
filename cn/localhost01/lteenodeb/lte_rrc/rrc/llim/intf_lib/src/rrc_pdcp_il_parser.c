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
 *  File Description : The file rrc_pdcp_il_parser.c contains the definitions 
 *                     of rrc-pdcp interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_pdcp_il_parser.h"
#include "rrc_pdcp_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_rrc_pdcp_cr_srb_entity
(
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_rohc
(
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_disc_timer
(
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_sn_size
(
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_st_rep_required
(
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_ue_st_rep_required
(
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_cr_drb_entity
(
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_int
(
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_srb_ciph
(
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_drb_ciph
(
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_ue_inactive_time
(
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_ho_triggered
(
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_sps_dl_setup_info
(
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_sps_dl_info
(
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_sps_ul_setup_info
(
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_sps_ul_info
(
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_sps_config
(
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_i_msg
(
    rrc_mac_i_msg_t *p_rrc_mac_i_msg,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_ue_inactivity_timer
(
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_cr_srb_entity_error
(
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_cr_drb_entity_error
(
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_i
(
    rrc_mac_i_t *p_rrc_mac_i,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_rcfg_srb_entity
(
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_lwa_config_lwa_bearer_type
(
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_config_lwa
(
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_rcfg_drb_entity
(
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_sec_offset
(
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_del_srb_entity
(
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_del_drb_entity
(
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_del_srb_entity_error
(
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_del_drb_entity_error
(
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_rcfg_srb_entity_error
(
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_rcfg_drb_entity_error
(
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_data_status_error
(
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_lossy_ho_required
(
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_ul_rcv_sn_val
(
    ul_rcv_sn_val_t *p_ul_rcv_sn_val,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_lc_sn_hfn_status
(
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_message
(
    rrc_message_t *p_rrc_message,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_msg_authentication_code
(
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_rb_entity
(
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_rb_entity_error
(
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_drb_list
(
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_countMSB_uplink
(
    countMSB_uplink_t *p_countMSB_uplink,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_countMSB_downlink
(
    countMSB_downlink_t *p_countMSB_downlink,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_drb_countMSB_info
(
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_pdcp_communication_info
(
    pdcp_communication_info_t *p_pdcp_communication_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_mac_i_msg_req
(
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_ue_inactivity_time_val
(
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_pdcp_inactive_ue_info
(
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_rrc_pdcp_cr_ue_entity_req
(
    rrc_pdcp_cr_ue_entity_req_t *p_rrc_pdcp_cr_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_cr_ue_entity_req, 0, sizeof(rrc_pdcp_cr_ue_entity_req_t));

    /* This function parses rrc_pdcp_cr_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_ue_entity_req->crnti, p_src + *p_length_read, "crnti");
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
            case RRC_PDCP_CREATE_SRB_ENTITY_TAG:
                /* rrc_pdcp_cr_srb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity >= (S32)ARRSIZE(p_rrc_pdcp_cr_ue_entity_req->cr_srb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_srb_entity(
                    &p_rrc_pdcp_cr_ue_entity_req->cr_srb_entities[p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_CREATE_DRB_ENTITY_TAG:
                /* rrc_pdcp_cr_drb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity >= (S32)ARRSIZE(p_rrc_pdcp_cr_ue_entity_req->cr_drb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_drb_entity(
                    &p_rrc_pdcp_cr_ue_entity_req->cr_drb_entities[p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_CONFIGURE_INTEGRITY_PROTECTION_TAG:
                /* rrc_pdcp_config_int TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_int(
                    &p_rrc_pdcp_cr_ue_entity_req->config_integrity_protection,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_SRB_CIPHERING_TAG:
                /* rrc_pdcp_config_srb_ciph TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_srb_ciph(
                    &p_rrc_pdcp_cr_ue_entity_req->config_srb_ciphering,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_DRB_CIPHERING_TAG:
                /* rrc_pdcp_config_drb_ciph TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_drb_ciph(
                    &p_rrc_pdcp_cr_ue_entity_req->config_drb_ciphering,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_HO_TRIGGER_TAG:
                /* rrc_pdcp_ho_triggered TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_ho_triggered(
                    &p_rrc_pdcp_cr_ue_entity_req->ho_triggered,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_HO_TRIGGER_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_SPS_CONFIG_TAG:
                /* rrc_pdcp_sps_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sps_config(
                    &p_rrc_pdcp_cr_ue_entity_req->sps_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_SPS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_MACI_REQ_TAG:
                /* rrc_mac_i_msg TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_i_msg(
                    &p_rrc_pdcp_cr_ue_entity_req->mac_i_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_CONFIG_MACI_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG:
                /* rrc_pdcp_ue_inactivity_timer TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_ue_inactivity_timer(
                    &p_rrc_pdcp_cr_ue_entity_req->ue_inactivity_timer,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_req->optional_elems_present |= RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_PRESENT;

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
rrc_il_parse_rrc_pdcp_cr_srb_entity
(
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_cr_srb_entity, 0, sizeof(rrc_pdcp_cr_srb_entity_t));

    /* This function parses rrc_pdcp_cr_srb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_cr_srb_entity->srb_id, p_src + *p_length_read, "srb_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_srb_entity->rlc_mode, p_src + *p_length_read, "rlc_mode");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_srb_entity->rb_direction, p_src + *p_length_read, "rb_direction");
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
rrc_il_parse_rrc_pdcp_config_rohc
(
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_rohc, 0, sizeof(rrc_pdcp_config_rohc_t));

    /* This function parses rrc_pdcp_config_rohc */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_rohc->profile_id, p_src + *p_length_read, "profile_id");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_rohc->max_cid, p_src + *p_length_read, "max_cid");
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
rrc_il_parse_rrc_pdcp_config_disc_timer
(
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_disc_timer, 0, sizeof(rrc_pdcp_config_disc_timer_t));

    /* This function parses rrc_pdcp_config_disc_timer */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_disc_timer->disc_timer, p_src + *p_length_read, "disc_timer");
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
rrc_il_parse_rrc_pdcp_config_sn_size
(
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_sn_size, 0, sizeof(rrc_pdcp_config_sn_size_t));

    /* This function parses rrc_pdcp_config_sn_size */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_sn_size->sn_size, p_src + *p_length_read, "sn_size");
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
rrc_il_parse_rrc_pdcp_config_st_rep_required
(
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_st_rep_required, 0, sizeof(rrc_pdcp_config_st_rep_required_t));

    /* This function parses rrc_pdcp_config_st_rep_required */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_st_rep_required->st_rep_required, p_src + *p_length_read, "st_rep_required");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_st_rep_required->st_rep_required > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_st_rep_required->st_rep_required] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_config_st_rep_required->st_rep_required);
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
rrc_il_parse_rrc_pdcp_config_ue_st_rep_required
(
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_ue_st_rep_required, 0, sizeof(rrc_pdcp_config_ue_st_rep_required_t));

    /* This function parses rrc_pdcp_config_ue_st_rep_required */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required, p_src + *p_length_read, "ue_st_rep_required");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required);
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
rrc_il_parse_rrc_pdcp_cr_drb_entity
(
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_cr_drb_entity, 0, sizeof(rrc_pdcp_cr_drb_entity_t));

    /* This function parses rrc_pdcp_cr_drb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_cr_drb_entity->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_cr_drb_entity->drb_id, p_src + *p_length_read, "drb_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_drb_entity->rlc_mode, p_src + *p_length_read, "rlc_mode");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_drb_entity->rb_direction, p_src + *p_length_read, "rb_direction");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_cr_drb_entity->qci, p_src + *p_length_read, "qci");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_cr_drb_entity->qci < 1) || (p_rrc_pdcp_cr_drb_entity->qci > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_cr_drb_entity->qci] should be in range "
            "1 to 9. Incorrect value %u received.", p_rrc_pdcp_cr_drb_entity->qci);
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
            case RRC_PDCP_CONFIGURE_ROHC_TAG:
                /* rrc_pdcp_config_rohc TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_rohc(
                    &p_rrc_pdcp_cr_drb_entity->config_rohc,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_drb_entity->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_ROHC_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_DISCARD_TIMER_TAG:
                /* rrc_pdcp_config_disc_timer TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_disc_timer(
                    &p_rrc_pdcp_cr_drb_entity->config_disc_timer,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_drb_entity->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_DISC_TIMER_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_SN_SIZE_TAG:
                /* rrc_pdcp_config_sn_size TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_sn_size(
                    &p_rrc_pdcp_cr_drb_entity->config_sn_size,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_drb_entity->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_SN_SIZE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_ST_REPORT_REQUIRED_TAG:
                /* rrc_pdcp_config_st_rep_required TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_st_rep_required(
                    &p_rrc_pdcp_cr_drb_entity->st_rep_required,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_drb_entity->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_UE_PSR_TAG:
                /* rrc_pdcp_config_ue_st_rep_required TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_ue_st_rep_required(
                    &p_rrc_pdcp_cr_drb_entity->ue_st_rep_required,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_drb_entity->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT;

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
rrc_il_parse_rrc_pdcp_config_int
(
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_int, 0, sizeof(rrc_pdcp_config_int_t));

    /* This function parses rrc_pdcp_config_int */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_int->algorithm_id, p_src + *p_length_read, "algorithm_id");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_rrc_pdcp_config_int->key) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_pdcp_config_int->key); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_pdcp_config_int->key[loop], (void*)(p_src + *p_length_read), "key[]");
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
rrc_il_parse_rrc_pdcp_config_srb_ciph
(
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_srb_ciph, 0, sizeof(rrc_pdcp_config_srb_ciph_t));

    /* This function parses rrc_pdcp_config_srb_ciph */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_srb_ciph->algorithm_id, p_src + *p_length_read, "algorithm_id");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_rrc_pdcp_config_srb_ciph->key) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_pdcp_config_srb_ciph->key); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_pdcp_config_srb_ciph->key[loop], (void*)(p_src + *p_length_read), "key[]");
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
rrc_il_parse_rrc_pdcp_config_drb_ciph
(
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_drb_ciph, 0, sizeof(rrc_pdcp_config_drb_ciph_t));

    /* This function parses rrc_pdcp_config_drb_ciph */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_drb_ciph->algorithm_id, p_src + *p_length_read, "algorithm_id");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_rrc_pdcp_config_drb_ciph->key) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_pdcp_config_drb_ciph->key); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_pdcp_config_drb_ciph->key[loop], (void*)(p_src + *p_length_read), "key[]");
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
rrc_il_parse_rrc_ue_inactive_time
(
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_ue_inactive_time, 0, sizeof(rrc_ue_inactive_time_t));

    /* This function parses rrc_ue_inactive_time */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_ue_inactive_time->ue_inactive_time_val, p_src + *p_length_read, "ue_inactive_time_val");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_ue_inactive_time->ue_inactive_time_val > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_ue_inactive_time->ue_inactive_time_val] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_ue_inactive_time->ue_inactive_time_val);
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
rrc_il_parse_rrc_pdcp_ho_triggered
(
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_ho_triggered, 0, sizeof(rrc_pdcp_ho_triggered_t));

    /* This function parses rrc_pdcp_ho_triggered */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_ho_triggered->ho_trigger, p_src + *p_length_read, "ho_trigger");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_ho_triggered->ho_trigger > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_ho_triggered->ho_trigger] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_ho_triggered->ho_trigger);
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
            case RRC_PDCP_UE_INACTIVE_TIME_LAPSED_TAG:
                /* rrc_ue_inactive_time TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_ue_inactive_time(
                    &p_rrc_pdcp_ho_triggered->rrc_ue_inactive_time_lapsed,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_ho_triggered->optional_elems_present |= RRC_PDCP_UE_INACTIVE_TIME_LAPSED_PRESENT;

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
rrc_il_parse_rrc_pdcp_sps_dl_setup_info
(
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sps_dl_setup_info, 0, sizeof(rrc_pdcp_sps_dl_setup_info_t));

    /* This function parses rrc_pdcp_sps_dl_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl, p_src + *p_length_read, "semi_persistent_sched_interval_dl");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl);
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
rrc_il_parse_rrc_pdcp_sps_dl_info
(
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sps_dl_info, 0, sizeof(rrc_pdcp_sps_dl_info_t));

    /* This function parses rrc_pdcp_sps_dl_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_sps_dl_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_dl_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_dl_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_sps_dl_info->request_type);
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
            case RRC_PDCP_SPS_DL_SETUP_INFO_TAG:
                /* rrc_pdcp_sps_dl_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sps_dl_setup_info(
                    &p_rrc_pdcp_sps_dl_info->sps_dl_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sps_dl_info->optional_elems_present |= RRC_PDCP_SPS_DL_SETUP_INFO_PRESENT;

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
rrc_il_parse_rrc_pdcp_sps_ul_setup_info
(
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sps_ul_setup_info, 0, sizeof(rrc_pdcp_sps_ul_setup_info_t));

    /* This function parses rrc_pdcp_sps_ul_setup_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul, p_src + *p_length_read, "semi_persistent_sched_interval_ul");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul);
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
rrc_il_parse_rrc_pdcp_sps_ul_info
(
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sps_ul_info, 0, sizeof(rrc_pdcp_sps_ul_info_t));

    /* This function parses rrc_pdcp_sps_ul_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_sps_ul_info->request_type, p_src + *p_length_read, "request_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_ul_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_ul_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_sps_ul_info->request_type);
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
            case RRC_PDCP_SPS_UL_SETUP_INFO_TAG:
                /* rrc_pdcp_sps_ul_setup_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sps_ul_setup_info(
                    &p_rrc_pdcp_sps_ul_info->sps_ul_setup_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sps_ul_info->optional_elems_present |= RRC_PDCP_SPS_UL_SETUP_INFO_PRESENT;

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
rrc_il_parse_rrc_pdcp_sps_config
(
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sps_config, 0, sizeof(rrc_pdcp_sps_config_t));

    /* This function parses rrc_pdcp_sps_config */

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
            case RRC_PDCP_SPS_DL_INFO_TAG:
                /* rrc_pdcp_sps_dl_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sps_dl_info(
                    &p_rrc_pdcp_sps_config->sps_dl_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sps_config->optional_elems_present |= RRC_PDCP_SPS_DL_INFO_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_SPS_UL_INFO_TAG:
                /* rrc_pdcp_sps_ul_info TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sps_ul_info(
                    &p_rrc_pdcp_sps_config->sps_ul_info,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sps_config->optional_elems_present |= RRC_PDCP_SPS_UL_INFO_PRESENT;

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
rrc_il_parse_rrc_mac_i_msg
(
    rrc_mac_i_msg_t *p_rrc_mac_i_msg,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_i_msg, 0, sizeof(rrc_mac_i_msg_t));

    /* This function parses rrc_mac_i_msg */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_i_msg->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_i_msg->rb_direction, p_src + *p_length_read, "rb_direction");
    *p_length_read += sizeof(U16);

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_mac_i_msg->encoded_var_short_mac_i) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length %u is exceeding p_rrc_mac_i_msg->encoded_var_short_mac_i array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_mac_i_msg->encoded_var_short_mac_i[loop], p_src + *p_length_read + loop, "encoded_var_short_mac_i[]");
        }

        p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length = length_left - *p_length_read;
        *p_length_read = length_left;
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
rrc_il_parse_rrc_pdcp_ue_inactivity_timer
(
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_ue_inactivity_timer, 0, sizeof(rrc_pdcp_ue_inactivity_timer_t));

    /* This function parses rrc_pdcp_ue_inactivity_timer */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_pdcp_ue_inactivity_timer->ue_inactive_time_config, p_src + *p_length_read, "ue_inactive_time_config");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_cr_ue_entity_cnf
(
    rrc_pdcp_cr_ue_entity_cnf_t *p_rrc_pdcp_cr_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_cr_ue_entity_cnf, 0, sizeof(rrc_pdcp_cr_ue_entity_cnf_t));

    /* This function parses rrc_pdcp_cr_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_cr_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_cr_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_cr_ue_entity_cnf->response_code);
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
            case RRC_PDCP_CREATE_SRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_cr_srb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_cr_ue_entity_cnf->cr_srb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_srb_entity_error(
                    &p_rrc_pdcp_cr_ue_entity_cnf->cr_srb_error_entities[p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_CREATE_DRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_cr_drb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_cr_ue_entity_cnf->cr_drb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_drb_entity_error(
                    &p_rrc_pdcp_cr_ue_entity_cnf->cr_drb_error_entities[p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_MACI_RESP_TAG:
                /* rrc_mac_i TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_i(
                    &p_rrc_pdcp_cr_ue_entity_cnf->mac_i,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_cr_ue_entity_cnf->optional_elems_present |= RRC_PDCP_CONFIG_MACI_RESP_PRESENT;

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
rrc_il_parse_rrc_pdcp_cr_srb_entity_error
(
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_cr_srb_entity_error, 0, sizeof(rrc_pdcp_cr_srb_entity_error_t));

    /* This function parses rrc_pdcp_cr_srb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_cr_srb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_srb_entity_error->response, p_src + *p_length_read, "response");
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
rrc_il_parse_rrc_pdcp_cr_drb_entity_error
(
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_cr_drb_entity_error, 0, sizeof(rrc_pdcp_cr_drb_entity_error_t));

    /* This function parses rrc_pdcp_cr_drb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_cr_drb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_cr_drb_entity_error->response, p_src + *p_length_read, "response");
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
rrc_il_parse_rrc_mac_i
(
    rrc_mac_i_t *p_rrc_mac_i,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_i, 0, sizeof(rrc_mac_i_t));

    /* This function parses rrc_mac_i */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_mac_i->mac_i, p_src + *p_length_read, "mac_i");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_del_ue_entity_req
(
    rrc_pdcp_del_ue_entity_req_t *p_rrc_pdcp_del_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_del_ue_entity_req, 0, sizeof(rrc_pdcp_del_ue_entity_req_t));

    /* This function parses rrc_pdcp_del_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_del_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_del_ue_entity_cnf
(
    rrc_pdcp_del_ue_entity_cnf_t *p_rrc_pdcp_del_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_del_ue_entity_cnf, 0, sizeof(rrc_pdcp_del_ue_entity_cnf_t));

    /* This function parses rrc_pdcp_del_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_del_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_del_ue_entity_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_del_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_del_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_del_ue_entity_cnf->response);
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
rrc_il_parse_rrc_pdcp_update_lwa_traffic_ind
(
    rrc_pdcp_update_lwa_traffic_ind_t *p_rrc_pdcp_update_lwa_traffic_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_update_lwa_traffic_ind, 0, sizeof(rrc_pdcp_update_lwa_traffic_ind_t));

    /* This function parses rrc_pdcp_update_lwa_traffic_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_update_lwa_traffic_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_update_lwa_traffic_ind->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_update_lwa_traffic_ind->traffic_ind, p_src + *p_length_read, "traffic_ind");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_reconf_ue_entity_req
(
    rrc_pdcp_reconf_ue_entity_req_t *p_rrc_pdcp_reconf_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_reconf_ue_entity_req, 0, sizeof(rrc_pdcp_reconf_ue_entity_req_t));

    /* This function parses rrc_pdcp_reconf_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_reconf_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_PDCP_CREATE_SRB_ENTITY_TAG:
                /* rrc_pdcp_cr_srb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity >= (S32)ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->cr_srb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_srb_entity(
                    &p_rrc_pdcp_reconf_ue_entity_req->cr_srb_entities[p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_CREATE_DRB_ENTITY_TAG:
                /* rrc_pdcp_cr_drb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity >= (S32)ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->cr_drb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_drb_entity(
                    &p_rrc_pdcp_reconf_ue_entity_req->cr_drb_entities[p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_RECONFIG_SRB_ENTITY_TAG:
                /* rrc_pdcp_rcfg_srb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity >= (S32)ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->rcfg_srb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rcfg_srb_entity(
                    &p_rrc_pdcp_reconf_ue_entity_req->rcfg_srb_entities[p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_RECONFIG_DRB_ENTITY_TAG:
                /* rrc_pdcp_rcfg_drb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity >= (S32)ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->rcfg_drb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rcfg_drb_entity(
                    &p_rrc_pdcp_reconf_ue_entity_req->rcfg_drb_entities[p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_SECURITY_OFFSET_TAG:
                /* rrc_pdcp_sec_offset TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sec_offset(
                    &p_rrc_pdcp_reconf_ue_entity_req->pdcp_security_offset,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_SECURITY_OFFSET_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_INTEGRITY_PROTECTION_TAG:
                /* rrc_pdcp_config_int TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_int(
                    &p_rrc_pdcp_reconf_ue_entity_req->config_integrity_protection,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_SRB_CIPHERING_TAG:
                /* rrc_pdcp_config_srb_ciph TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_srb_ciph(
                    &p_rrc_pdcp_reconf_ue_entity_req->config_srb_ciphering,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_DRB_CIPHERING_TAG:
                /* rrc_pdcp_config_drb_ciph TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_drb_ciph(
                    &p_rrc_pdcp_reconf_ue_entity_req->config_drb_ciphering,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_DELETE_SRB_ENTITY_TAG:
                /* rrc_pdcp_del_srb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity >= (S32)ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->del_srb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_del_srb_entity(
                    &p_rrc_pdcp_reconf_ue_entity_req->del_srb_entities[p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_DELETE_DRB_ENTITY_TAG:
                /* rrc_pdcp_del_drb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity >= (S32)ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->del_drb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_del_drb_entity(
                    &p_rrc_pdcp_reconf_ue_entity_req->del_drb_entities[p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_SPS_CONFIG_TAG:
                /* rrc_pdcp_sps_config TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_sps_config(
                    &p_rrc_pdcp_reconf_ue_entity_req->sps_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_RECONFIG_UE_SPS_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_MACI_REQ_TAG:
                /* rrc_mac_i_msg TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_i_msg(
                    &p_rrc_pdcp_reconf_ue_entity_req->mac_i_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_RECONFIG_MACI_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG:
                /* rrc_pdcp_ue_inactivity_timer TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_ue_inactivity_timer(
                    &p_rrc_pdcp_reconf_ue_entity_req->ue_inactivity_timer,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present |= RRC_PDCP_RECONFIG_UE_INACTIVITY_TIMER_CONFIG_PRESENT;

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
rrc_il_parse_rrc_pdcp_rcfg_srb_entity
(
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_rcfg_srb_entity, 0, sizeof(rrc_pdcp_rcfg_srb_entity_t));

    /* This function parses rrc_pdcp_rcfg_srb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_rcfg_srb_entity->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_rcfg_srb_entity->rb_direction, p_src + *p_length_read, "rb_direction");
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
rrc_il_parse_rrc_pdcp_config_lwa_config_lwa_bearer_type
(
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_lwa_config_lwa_bearer_type, 0, sizeof(rrc_pdcp_config_lwa_config_lwa_bearer_type_t));

    /* This function parses rrc_pdcp_config_lwa_config_lwa_bearer_type */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_lwa_config_lwa_bearer_type->bearer_type, p_src + *p_length_read, "bearer_type");
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
rrc_il_parse_rrc_pdcp_config_lwa
(
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_lwa, 0, sizeof(rrc_pdcp_config_lwa_t));

    /* This function parses rrc_pdcp_config_lwa */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_lwa->lwa_bearer, p_src + *p_length_read, "lwa_bearer");
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
            case RRC_PDCP_LWA_CONFIG_BEARER_TYPE_TAG:
                /* rrc_pdcp_config_lwa_config_lwa_bearer_type TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_lwa_config_lwa_bearer_type(
                    &p_rrc_pdcp_config_lwa->lwa_bearer_type,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_config_lwa->optional_elems_present |= RRC_PDCP_DRB_CONFIG_LWA_BEARER_TYPE_PRESENT;

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
rrc_il_parse_rrc_pdcp_rcfg_drb_entity
(
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_rcfg_drb_entity, 0, sizeof(rrc_pdcp_rcfg_drb_entity_t));

    /* This function parses rrc_pdcp_rcfg_drb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_rcfg_drb_entity->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_rcfg_drb_entity->rb_direction, p_src + *p_length_read, "rb_direction");
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
            case RRC_PDCP_CONFIGURE_LWA_TAG:
                /* rrc_pdcp_config_lwa TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_lwa(
                    &p_rrc_pdcp_rcfg_drb_entity->lwa_config,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_rcfg_drb_entity->optional_elems_present |= RRC_PDCP_RCFG_DRB_CONFIG_LWA_CONFIG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_ROHC_TAG:
                /* rrc_pdcp_config_rohc TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_rohc(
                    &p_rrc_pdcp_rcfg_drb_entity->config_rohc,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_rcfg_drb_entity->optional_elems_present |= RRC_PDCP_RCFG_DRB_CONFIG_ROHC_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_ST_REPORT_REQUIRED_TAG:
                /* rrc_pdcp_config_st_rep_required TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_st_rep_required(
                    &p_rrc_pdcp_rcfg_drb_entity->st_rep_required,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_rcfg_drb_entity->optional_elems_present |= RRC_PDCP_RCFG_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_CONFIGURE_UE_PSR_TAG:
                /* rrc_pdcp_config_ue_st_rep_required TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_config_ue_st_rep_required(
                    &p_rrc_pdcp_rcfg_drb_entity->ue_st_rep_required,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_rcfg_drb_entity->optional_elems_present |= RRC_PDCP_RCFG_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT;

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
rrc_il_parse_rrc_pdcp_sec_offset
(
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sec_offset, 0, sizeof(rrc_pdcp_sec_offset_t));

    /* This function parses rrc_pdcp_sec_offset */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_sec_offset->configure_sec_offset, p_src + *p_length_read, "configure_sec_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_sec_offset->configure_sec_offset < 1) || (p_rrc_pdcp_sec_offset->configure_sec_offset > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sec_offset->configure_sec_offset] should be in range "
            "1 to 1. Incorrect value %u received.", p_rrc_pdcp_sec_offset->configure_sec_offset);
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
rrc_il_parse_rrc_pdcp_del_srb_entity
(
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_del_srb_entity, 0, sizeof(rrc_pdcp_del_srb_entity_t));

    /* This function parses rrc_pdcp_del_srb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_del_srb_entity->lc_id, p_src + *p_length_read, "lc_id");
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
rrc_il_parse_rrc_pdcp_del_drb_entity
(
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_del_drb_entity, 0, sizeof(rrc_pdcp_del_drb_entity_t));

    /* This function parses rrc_pdcp_del_drb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_del_drb_entity->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_reconfig_ue_entity_cnf
(
    rrc_pdcp_reconfig_ue_entity_cnf_t *p_rrc_pdcp_reconfig_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_reconfig_ue_entity_cnf, 0, sizeof(rrc_pdcp_reconfig_ue_entity_cnf_t));

    /* This function parses rrc_pdcp_reconfig_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_reconfig_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_reconfig_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_reconfig_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_reconfig_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_reconfig_ue_entity_cnf->response_code);
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
            case RRC_PDCP_CREATE_SRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_cr_srb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->cr_srb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_srb_entity_error(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->cr_srb_error_entities[p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_CREATE_DRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_cr_drb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->cr_drb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_cr_drb_entity_error(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->cr_drb_error_entities[p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_DELETE_SRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_del_srb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->del_srb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_del_srb_entity_error(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->del_srb_error_entities[p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_DELETE_DRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_del_drb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->del_drb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_del_drb_entity_error(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->del_drb_error_entities[p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_RECONFIG_SRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_rcfg_srb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_srb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rcfg_srb_entity_error(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_srb_error_entities[p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_RECONFIG_DRB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_rcfg_drb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_drb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rcfg_drb_entity_error(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_drb_error_entities[p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_MACI_RESP_TAG:
                /* rrc_mac_i TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_i(
                    &p_rrc_pdcp_reconfig_ue_entity_cnf->mac_i,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_reconfig_ue_entity_cnf->optional_elems_present |= RRC_PDCP_RECONFIG_MACI_RESP_PRESENT;

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
rrc_il_parse_rrc_pdcp_del_srb_entity_error
(
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_del_srb_entity_error, 0, sizeof(rrc_pdcp_del_srb_entity_error_t));

    /* This function parses rrc_pdcp_del_srb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_del_srb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_del_srb_entity_error->response, p_src + *p_length_read, "response");
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
rrc_il_parse_rrc_pdcp_del_drb_entity_error
(
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_del_drb_entity_error, 0, sizeof(rrc_pdcp_del_drb_entity_error_t));

    /* This function parses rrc_pdcp_del_drb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_del_drb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_del_drb_entity_error->response, p_src + *p_length_read, "response");
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
rrc_il_parse_rrc_pdcp_rcfg_srb_entity_error
(
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_rcfg_srb_entity_error, 0, sizeof(rrc_pdcp_rcfg_srb_entity_error_t));

    /* This function parses rrc_pdcp_rcfg_srb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_rcfg_srb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_rcfg_srb_entity_error->response, p_src + *p_length_read, "response");
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
rrc_il_parse_rrc_pdcp_rcfg_drb_entity_error
(
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_rcfg_drb_entity_error, 0, sizeof(rrc_pdcp_rcfg_drb_entity_error_t));

    /* This function parses rrc_pdcp_rcfg_drb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_rcfg_drb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_rcfg_drb_entity_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_srb_data_req
(
    rrc_pdcp_srb_data_req_t *p_rrc_pdcp_srb_data_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_srb_data_req, 0, sizeof(rrc_pdcp_srb_data_req_t));

    /* This function parses rrc_pdcp_srb_data_req */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_pdcp_srb_data_req->transaction_id, p_src + *p_length_read, "transaction_id");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_srb_data_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_srb_data_req->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_srb_data_req->service_requested, p_src + *p_length_read, "service_requested");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_srb_data_ind
(
    rrc_pdcp_srb_data_ind_t *p_rrc_pdcp_srb_data_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_srb_data_ind, 0, sizeof(rrc_pdcp_srb_data_ind_t));

    /* This function parses rrc_pdcp_srb_data_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_srb_data_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_srb_data_ind->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_srb_data_status_ind
(
    rrc_pdcp_srb_data_status_ind_t *p_rrc_pdcp_srb_data_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_srb_data_status_ind, 0, sizeof(rrc_pdcp_srb_data_status_ind_t));

    /* This function parses rrc_pdcp_srb_data_status_ind */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_pdcp_srb_data_status_ind->transaction_id, p_src + *p_length_read, "transaction_id");
    *p_length_read += sizeof(U32);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_srb_data_status_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_srb_data_status_ind->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_srb_data_status_ind->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_srb_data_status_ind->response_code > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_srb_data_status_ind->response_code] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_srb_data_status_ind->response_code);
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
            case RRC_PDCP_SRB_DATA_STATUS_ERROR_TAG:
                /* rrc_pdcp_data_status_error TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_data_status_error(
                    &p_rrc_pdcp_srb_data_status_ind->srb_data_status_error,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_srb_data_status_ind->optional_elems_present |= RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT;

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
rrc_il_parse_rrc_pdcp_data_status_error
(
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_data_status_error, 0, sizeof(rrc_pdcp_data_status_error_t));

    /* This function parses rrc_pdcp_data_status_error */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_data_status_error->error_code, p_src + *p_length_read, "error_code");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_suspend_ue_entity_req
(
    rrc_pdcp_suspend_ue_entity_req_t *p_rrc_pdcp_suspend_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_suspend_ue_entity_req, 0, sizeof(rrc_pdcp_suspend_ue_entity_req_t));

    /* This function parses rrc_pdcp_suspend_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_suspend_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_suspend_ue_entity_cnf
(
    rrc_pdcp_suspend_ue_entity_cnf_t *p_rrc_pdcp_suspend_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_suspend_ue_entity_cnf, 0, sizeof(rrc_pdcp_suspend_ue_entity_cnf_t));

    /* This function parses rrc_pdcp_suspend_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_suspend_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_suspend_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_sn_hfn_status_req
(
    rrc_pdcp_sn_hfn_status_req_t *p_rrc_pdcp_sn_hfn_status_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sn_hfn_status_req, 0, sizeof(rrc_pdcp_sn_hfn_status_req_t));

    /* This function parses rrc_pdcp_sn_hfn_status_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_sn_hfn_status_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_sn_hfn_status_req->sn_hfn_required, p_src + *p_length_read, "sn_hfn_required");
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
            case RRC_PDCP_LOSSY_HO_REQUIRED_TAG:
                /* rrc_lossy_ho_required TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity >= (S32)ARRSIZE(p_rrc_pdcp_sn_hfn_status_req->lossy_ho_required))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_lossy_ho_required(
                    &p_rrc_pdcp_sn_hfn_status_req->lossy_ho_required[p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity++;

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
rrc_il_parse_rrc_lossy_ho_required
(
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_lossy_ho_required, 0, sizeof(rrc_lossy_ho_required_t));

    /* This function parses rrc_lossy_ho_required */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_lossy_ho_required->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_sn_hfn_status_resp
(
    rrc_pdcp_sn_hfn_status_resp_t *p_rrc_pdcp_sn_hfn_status_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sn_hfn_status_resp, 0, sizeof(rrc_pdcp_sn_hfn_status_resp_t));

    /* This function parses rrc_pdcp_sn_hfn_status_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_sn_hfn_status_resp->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_sn_hfn_status_resp->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sn_hfn_status_resp->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sn_hfn_status_resp->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_sn_hfn_status_resp->response_code);
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
            case RRC_PDCP_LC_SN_HFN_STATUS_TAG:
                /* rrc_lc_sn_hfn_status TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status >= (S32)ARRSIZE(p_rrc_pdcp_sn_hfn_status_resp->lc_sn_hfn_status))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_lc_sn_hfn_status(
                    &p_rrc_pdcp_sn_hfn_status_resp->lc_sn_hfn_status[p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status++;

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
rrc_il_parse_ul_rcv_sn_val
(
    ul_rcv_sn_val_t *p_ul_rcv_sn_val,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_ul_rcv_sn_val, 0, sizeof(ul_rcv_sn_val_t));

    /* This function parses ul_rcv_sn_val */

    if (*p_length_read + (S32)sizeof(p_ul_rcv_sn_val->ul_rcv_sn) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ul_rcv_sn_val->ul_rcv_sn); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_ul_rcv_sn_val->ul_rcv_sn[loop], (void*)(p_src + *p_length_read), "ul_rcv_sn[]");
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
rrc_il_parse_rrc_lc_sn_hfn_status
(
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_lc_sn_hfn_status, 0, sizeof(rrc_lc_sn_hfn_status_t));

    /* This function parses rrc_lc_sn_hfn_status */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_lc_sn_hfn_status->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_lc_sn_hfn_status->dl_sn_cnt, p_src + *p_length_read, "dl_sn_cnt");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->dl_sn_cnt > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->dl_sn_cnt] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->dl_sn_cnt);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_lc_sn_hfn_status->dl_hfn_cnt, p_src + *p_length_read, "dl_hfn_cnt");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->dl_hfn_cnt > 1048575))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->dl_hfn_cnt] should be less than"
            " or equal to 1048575. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->dl_hfn_cnt);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_lc_sn_hfn_status->ul_sn_cnt, p_src + *p_length_read, "ul_sn_cnt");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->ul_sn_cnt > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->ul_sn_cnt] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->ul_sn_cnt);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_lc_sn_hfn_status->ul_hfn_cnt, p_src + *p_length_read, "ul_hfn_cnt");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->ul_hfn_cnt > 1048575))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->ul_hfn_cnt] should be less than"
            " or equal to 1048575. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->ul_hfn_cnt);
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
            case RRC_PDCP_UL_RECV_SN_TAG:
                /* ul_rcv_sn_val TLV */
                if (RRC_FAILURE == rrc_il_parse_ul_rcv_sn_val(
                    &p_rrc_lc_sn_hfn_status->ul_rcv_sn_val,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_lc_sn_hfn_status->optional_elems_present |= RRC_PDCP_UL_RECV_SN_PRESENT;

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
rrc_il_parse_rrc_pdcp_sn_hfn_status_ind
(
    rrc_pdcp_sn_hfn_status_ind_t *p_rrc_pdcp_sn_hfn_status_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_sn_hfn_status_ind, 0, sizeof(rrc_pdcp_sn_hfn_status_ind_t));

    /* This function parses rrc_pdcp_sn_hfn_status_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_sn_hfn_status_ind->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_PDCP_LC_SN_HFN_STATUS_TAG:
                /* rrc_lc_sn_hfn_status TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status >= (S32)ARRSIZE(p_rrc_pdcp_sn_hfn_status_ind->lc_sn_hfn_status))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_lc_sn_hfn_status(
                    &p_rrc_pdcp_sn_hfn_status_ind->lc_sn_hfn_status[p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status++;

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
rrc_il_parse_rrc_pdcp_data_buffer_stop_ind
(
    rrc_pdcp_data_buffer_stop_ind_t *p_rrc_pdcp_data_buffer_stop_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_data_buffer_stop_ind, 0, sizeof(rrc_pdcp_data_buffer_stop_ind_t));

    /* This function parses rrc_pdcp_data_buffer_stop_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_data_buffer_stop_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_mac_i_req
(
    rrc_pdcp_mac_i_req_t *p_rrc_pdcp_mac_i_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_mac_i_req, 0, sizeof(rrc_pdcp_mac_i_req_t));

    /* This function parses rrc_pdcp_mac_i_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_mac_i_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_mac_i_req->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_mac_i_req->rb_direction, p_src + *p_length_read, "rb_direction");
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
            case RRC_PDCP_MESSAGE_TAG:
                /* rrc_message TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_mac_i_req->num_message >= (S32)ARRSIZE(p_rrc_pdcp_mac_i_req->message))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_message(
                    &p_rrc_pdcp_mac_i_req->message[p_rrc_pdcp_mac_i_req->num_message],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_mac_i_req->num_message++;

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
rrc_il_parse_rrc_message
(
    rrc_message_t *p_rrc_message,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_message, 0, sizeof(rrc_message_t));

    /* This function parses rrc_message */

    if (*p_length_read + (S32)sizeof(p_rrc_message->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_message->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_message->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_message->encoded_var_short_mac_i) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_message->encoded_var_short_mac_i_data_length %u is exceeding p_rrc_message->encoded_var_short_mac_i array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_message->encoded_var_short_mac_i[loop], p_src + *p_length_read + loop, "encoded_var_short_mac_i[]");
        }

        p_rrc_message->encoded_var_short_mac_i_data_length = length_left - *p_length_read;
        *p_length_read = length_left;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_mac_i_resp
(
    rrc_pdcp_mac_i_resp_t *p_rrc_pdcp_mac_i_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_mac_i_resp, 0, sizeof(rrc_pdcp_mac_i_resp_t));

    /* This function parses rrc_pdcp_mac_i_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_mac_i_resp->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_mac_i_resp->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_mac_i_resp->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_mac_i_resp->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_mac_i_resp->response_code);
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
            case RRC_PDCP_MSG_AUTHENTICATION_CODE_TAG:
                /* rrc_msg_authentication_code TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_mac_i_resp->num_var_short_mac_i >= (S32)ARRSIZE(p_rrc_pdcp_mac_i_resp->rrc_msg_authentication_code))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_msg_authentication_code(
                    &p_rrc_pdcp_mac_i_resp->rrc_msg_authentication_code[p_rrc_pdcp_mac_i_resp->num_var_short_mac_i],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_mac_i_resp->num_var_short_mac_i++;

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
rrc_il_parse_rrc_msg_authentication_code
(
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_msg_authentication_code, 0, sizeof(rrc_msg_authentication_code_t));

    /* This function parses rrc_msg_authentication_code */

    if (*p_length_read + (S32)sizeof(p_rrc_msg_authentication_code->cell_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_msg_authentication_code->cell_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_rrc_msg_authentication_code->cell_identity[loop], (void*)(p_src + *p_length_read), "cell_identity[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_msg_authentication_code->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_msg_authentication_code->response_code > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_msg_authentication_code->response_code] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_msg_authentication_code->response_code);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_msg_authentication_code->mac_i, p_src + *p_length_read, "mac_i");
    *p_length_read += sizeof(U32);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_re_establish_ue_entity_req
(
    rrc_pdcp_re_establish_ue_entity_req_t *p_rrc_pdcp_re_establish_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_re_establish_ue_entity_req, 0, sizeof(rrc_pdcp_re_establish_ue_entity_req_t));

    /* This function parses rrc_pdcp_re_establish_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_re_establish_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_PDCP_RB_ENTITY_TAG:
                /* rrc_pdcp_rb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity >= (S32)ARRSIZE(p_rrc_pdcp_re_establish_ue_entity_req->re_est_rb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rb_entity(
                    &p_rrc_pdcp_re_establish_ue_entity_req->re_est_rb_entities[p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity++;

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
rrc_il_parse_rrc_pdcp_rb_entity
(
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_rb_entity, 0, sizeof(rrc_pdcp_rb_entity_t));

    /* This function parses rrc_pdcp_rb_entity */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_rb_entity->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_re_establish_ue_entity_cnf
(
    rrc_pdcp_re_establish_ue_entity_cnf_t *p_rrc_pdcp_re_establish_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_re_establish_ue_entity_cnf, 0, sizeof(rrc_pdcp_re_establish_ue_entity_cnf_t));

    /* This function parses rrc_pdcp_re_establish_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_re_establish_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_re_establish_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_re_establish_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_re_establish_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_re_establish_ue_entity_cnf->response_code);
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
            case RRC_PDCP_RB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_rb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_re_establish_ue_entity_cnf->re_est_rb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rb_entity_error(
                    &p_rrc_pdcp_re_establish_ue_entity_cnf->re_est_rb_error_entities[p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error++;

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
rrc_il_parse_rrc_pdcp_rb_entity_error
(
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_rb_entity_error, 0, sizeof(rrc_pdcp_rb_entity_error_t));

    /* This function parses rrc_pdcp_rb_entity_error */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_rb_entity_error->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_rb_entity_error->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_resume_ue_entity_req
(
    rrc_pdcp_resume_ue_entity_req_t *p_rrc_pdcp_resume_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_resume_ue_entity_req, 0, sizeof(rrc_pdcp_resume_ue_entity_req_t));

    /* This function parses rrc_pdcp_resume_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_resume_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_resume_ue_entity_req->resume_direction, p_src + *p_length_read, "resume_direction");
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
            case RRC_PDCP_RB_ENTITY_TAG:
                /* rrc_pdcp_rb_entity TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity >= (S32)ARRSIZE(p_rrc_pdcp_resume_ue_entity_req->resume_rb_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rb_entity(
                    &p_rrc_pdcp_resume_ue_entity_req->resume_rb_entities[p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity++;

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
rrc_il_parse_rrc_pdcp_resume_ue_entity_cnf
(
    rrc_pdcp_resume_ue_entity_cnf_t *p_rrc_pdcp_resume_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_resume_ue_entity_cnf, 0, sizeof(rrc_pdcp_resume_ue_entity_cnf_t));

    /* This function parses rrc_pdcp_resume_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_resume_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_resume_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_resume_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_resume_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_resume_ue_entity_cnf->response_code);
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
            case RRC_PDCP_RB_ENTITY_ERROR_TAG:
                /* rrc_pdcp_rb_entity_error TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error >= (S32)ARRSIZE(p_rrc_pdcp_resume_ue_entity_cnf->resume_rb_error_entities))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_rb_entity_error(
                    &p_rrc_pdcp_resume_ue_entity_cnf->resume_rb_error_entities[p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error++;

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
rrc_il_parse_rrc_pdcp_change_crnti_req
(
    rrc_pdcp_change_crnti_req_t *p_rrc_pdcp_change_crnti_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_change_crnti_req, 0, sizeof(rrc_pdcp_change_crnti_req_t));

    /* This function parses rrc_pdcp_change_crnti_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_change_crnti_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_change_crnti_req->old_crnti, p_src + *p_length_read, "old_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_change_crnti_req->new_crnti, p_src + *p_length_read, "new_crnti");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_change_crnti_cnf
(
    rrc_pdcp_change_crnti_cnf_t *p_rrc_pdcp_change_crnti_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_change_crnti_cnf, 0, sizeof(rrc_pdcp_change_crnti_cnf_t));

    /* This function parses rrc_pdcp_change_crnti_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_change_crnti_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_change_crnti_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_change_crnti_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_change_crnti_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_change_crnti_cnf->response);
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
rrc_il_parse_rrc_pdcp_count_wraparound_ind
(
    rrc_pdcp_count_wraparound_ind_t *p_rrc_pdcp_count_wraparound_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_count_wraparound_ind, 0, sizeof(rrc_pdcp_count_wraparound_ind_t));

    /* This function parses rrc_pdcp_count_wraparound_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_count_wraparound_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_notify_integrity_failure
(
    rrc_pdcp_notify_integrity_failure_t *p_rrc_pdcp_notify_integrity_failure,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_notify_integrity_failure, 0, sizeof(rrc_pdcp_notify_integrity_failure_t));

    /* This function parses rrc_pdcp_notify_integrity_failure */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_notify_integrity_failure->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_notify_integrity_failure->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_notify_integrity_failure->lc_id < 1) || (p_rrc_pdcp_notify_integrity_failure->lc_id > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_notify_integrity_failure->lc_id] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_pdcp_notify_integrity_failure->lc_id);
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
rrc_il_parse_rrc_pdcp_drb_count_msb_req
(
    rrc_pdcp_drb_count_msb_req_t *p_rrc_pdcp_drb_count_msb_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_drb_count_msb_req, 0, sizeof(rrc_pdcp_drb_count_msb_req_t));

    /* This function parses rrc_pdcp_drb_count_msb_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_drb_count_msb_req->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_PDCP_DRB_COUNTMSB_DRB_LIST_TAG:
                /* rrc_pdcp_drb_list TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_drb_count_msb_req->num_drb >= (S32)ARRSIZE(p_rrc_pdcp_drb_count_msb_req->drb_list))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_drb_list(
                    &p_rrc_pdcp_drb_count_msb_req->drb_list[p_rrc_pdcp_drb_count_msb_req->num_drb],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_drb_count_msb_req->num_drb++;

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
rrc_il_parse_rrc_pdcp_drb_list
(
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_drb_list, 0, sizeof(rrc_pdcp_drb_list_t));

    /* This function parses rrc_pdcp_drb_list */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_drb_list->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_drb_list->rb_direction, p_src + *p_length_read, "rb_direction");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_pdcp_drb_count_msb_resp
(
    rrc_pdcp_drb_count_msb_resp_t *p_rrc_pdcp_drb_count_msb_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_drb_count_msb_resp, 0, sizeof(rrc_pdcp_drb_count_msb_resp_t));

    /* This function parses rrc_pdcp_drb_count_msb_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_drb_count_msb_resp->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_drb_count_msb_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_drb_count_msb_resp->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_drb_count_msb_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_drb_count_msb_resp->response);
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
            case RRC_PDCP_DRB_COUNTMSB_INFO_TAG:
                /* rrc_pdcp_drb_countMSB_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_drb_count_msb_resp->num_drb >= (S32)ARRSIZE(p_rrc_pdcp_drb_count_msb_resp->drb_countMSB_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_drb_countMSB_info(
                    &p_rrc_pdcp_drb_count_msb_resp->drb_countMSB_info[p_rrc_pdcp_drb_count_msb_resp->num_drb],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_drb_count_msb_resp->num_drb++;

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
rrc_il_parse_countMSB_uplink
(
    countMSB_uplink_t *p_countMSB_uplink,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_countMSB_uplink, 0, sizeof(countMSB_uplink_t));

    /* This function parses countMSB_uplink */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_countMSB_uplink->countMSB_Uplink, p_src + *p_length_read, "countMSB_Uplink");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_countMSB_uplink->countMSB_Uplink > 33554431))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_countMSB_uplink->countMSB_Uplink] should be less than"
            " or equal to 33554431. Incorrect value %u received.", p_countMSB_uplink->countMSB_Uplink);
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
rrc_il_parse_countMSB_downlink
(
    countMSB_downlink_t *p_countMSB_downlink,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_countMSB_downlink, 0, sizeof(countMSB_downlink_t));

    /* This function parses countMSB_downlink */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_countMSB_downlink->countMSB_Downlink, p_src + *p_length_read, "countMSB_Downlink");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_countMSB_downlink->countMSB_Downlink > 33554431))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_countMSB_downlink->countMSB_Downlink] should be less than"
            " or equal to 33554431. Incorrect value %u received.", p_countMSB_downlink->countMSB_Downlink);
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
rrc_il_parse_rrc_pdcp_drb_countMSB_info
(
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_drb_countMSB_info, 0, sizeof(rrc_pdcp_drb_countMSB_info_t));

    /* This function parses rrc_pdcp_drb_countMSB_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_drb_countMSB_info->lc_id, p_src + *p_length_read, "lc_id");
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
            case RRC_PDCP_COUNTMSB_UPLINK_TAG:
                /* countMSB_uplink TLV */
                if (RRC_FAILURE == rrc_il_parse_countMSB_uplink(
                    &p_rrc_pdcp_drb_countMSB_info->countMSB_Uplink,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_drb_countMSB_info->optional_elems_present |= RRC_PDCP_COUNTMSB_UPLINK_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_COUNTMSB_DOWNLINK_TAG:
                /* countMSB_downlink TLV */
                if (RRC_FAILURE == rrc_il_parse_countMSB_downlink(
                    &p_rrc_pdcp_drb_countMSB_info->countMSB_Downlink,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_drb_countMSB_info->optional_elems_present |= RRC_PDCP_COUNTMSB_DOWNLINK_PRESENT;

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
rrc_il_parse_rrc_pdcp_config_cell_req
(
    rrc_pdcp_config_cell_req_t *p_rrc_pdcp_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_cell_req, 0, sizeof(rrc_pdcp_config_cell_req_t));

    /* This function parses rrc_pdcp_config_cell_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_config_cell_req->Cell_index, p_src + *p_length_read, "Cell_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_cell_req->Cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_cell_req->Cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_pdcp_config_cell_req->Cell_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_pdcp_communication_info(
        &p_rrc_pdcp_config_cell_req->pdcp_communication_info,
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
rrc_il_parse_pdcp_communication_info
(
    pdcp_communication_info_t *p_pdcp_communication_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_pdcp_communication_info, 0, sizeof(pdcp_communication_info_t));

    /* This function parses pdcp_communication_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_pdcp_communication_info->rlcInstance, p_src + *p_length_read, "rlcInstance");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_pdcp_communication_info->rlcRxPort, p_src + *p_length_read, "rlcRxPort");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_pdcp_communication_info->rlcIpAddr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_pdcp_communication_info->rlcIpAddr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_pdcp_communication_info->rlcIpAddr[loop], (void*)(p_src + *p_length_read), "rlcIpAddr[]");
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
rrc_il_parse_rrc_pdcp_config_cell_cnf
(
    rrc_pdcp_config_cell_cnf_t *p_rrc_pdcp_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_config_cell_cnf, 0, sizeof(rrc_pdcp_config_cell_cnf_t));

    /* This function parses rrc_pdcp_config_cell_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_cell_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_pdcp_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_config_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_config_cell_cnf->response);
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
rrc_il_parse_rrc_pdcp_ho_prep_info_req
(
    rrc_pdcp_ho_prep_info_req_t *p_rrc_pdcp_ho_prep_info_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_ho_prep_info_req, 0, sizeof(rrc_pdcp_ho_prep_info_req_t));

    /* This function parses rrc_pdcp_ho_prep_info_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_ho_prep_info_req->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_PDCP_MACI_HO_REQ_TAG:
                /* rrc_mac_i_msg_req TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_mac_i_msg_req(
                    &p_rrc_pdcp_ho_prep_info_req->mac_i_msg_req,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_ho_prep_info_req->optional_elems_present |= RRC_PDCP_MACI_MSG_REQ_PRESENT;

                *p_length_read += length;

                break;
            case RRC_PDCP_UE_INACTIVE_TIME_REQ_TAG:
                /* rrc_ue_inactivity_time_val TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_ue_inactivity_time_val(
                    &p_rrc_pdcp_ho_prep_info_req->ue_inactive_time_val,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_ho_prep_info_req->optional_elems_present |= RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_REQ_PRESENT;

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
rrc_il_parse_rrc_mac_i_msg_req
(
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_mac_i_msg_req, 0, sizeof(rrc_mac_i_msg_req_t));

    /* This function parses rrc_mac_i_msg_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_mac_i_msg_req->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_mac_i_msg_req->rb_direction, p_src + *p_length_read, "rb_direction");
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
            case RRC_PDCP_MESSAGE_TAG:
                /* rrc_message TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_mac_i_msg_req->num_message >= (S32)ARRSIZE(p_rrc_mac_i_msg_req->message))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_message(
                    &p_rrc_mac_i_msg_req->message[p_rrc_mac_i_msg_req->num_message],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_mac_i_msg_req->num_message++;

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
rrc_il_parse_rrc_ue_inactivity_time_val
(
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_ue_inactivity_time_val, 0, sizeof(rrc_ue_inactivity_time_val_t));

    /* This function parses rrc_ue_inactivity_time_val */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag, p_src + *p_length_read, "ue_inactive_time_val_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag);
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
rrc_il_parse_rrc_pdcp_ho_prep_info_resp
(
    rrc_pdcp_ho_prep_info_resp_t *p_rrc_pdcp_ho_prep_info_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_ho_prep_info_resp, 0, sizeof(rrc_pdcp_ho_prep_info_resp_t));

    /* This function parses rrc_pdcp_ho_prep_info_resp */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_ho_prep_info_resp->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_ho_prep_info_resp->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_ho_prep_info_resp->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_ho_prep_info_resp->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_ho_prep_info_resp->response_code);
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
            case RRC_PDCP_MSG_AUTHENTICATION_CODE_TAG:
                /* rrc_msg_authentication_code TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i >= (S32)ARRSIZE(p_rrc_pdcp_ho_prep_info_resp->rrc_msg_authentication_code))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_msg_authentication_code(
                    &p_rrc_pdcp_ho_prep_info_resp->rrc_msg_authentication_code[p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i++;

                *p_length_read += length_read;

                break;
            case RRC_PDCP_UE_INACTIVE_TIME_RESP_TAG:
                /* rrc_ue_inactive_time TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_ue_inactive_time(
                    &p_rrc_pdcp_ho_prep_info_resp->ue_inactivity_time_resp,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_ho_prep_info_resp->optional_elems_present |= RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_RESP_PRESENT;

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
rrc_il_parse_rrc_pdcp_inactive_ues_ind
(
    rrc_pdcp_inactive_ues_ind_t *p_rrc_pdcp_inactive_ues_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_inactive_ues_ind, 0, sizeof(rrc_pdcp_inactive_ues_ind_t));

    /* This function parses rrc_pdcp_inactive_ues_ind */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue, p_src + *p_length_read, "num_of_inactive_ue");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue < 1) || (p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue] should be in range "
            "1 to 10. Incorrect value %u received.", p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue);
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
            case RRC_PDCP_INACTIVE_UE_INFO_TAG:
                /* rrc_pdcp_inactive_ue_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter >= (S32)ARRSIZE(p_rrc_pdcp_inactive_ues_ind->inactive_ue_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_pdcp_inactive_ue_info(
                    &p_rrc_pdcp_inactive_ues_ind->inactive_ue_info[p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter++;

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
rrc_il_parse_rrc_pdcp_inactive_ue_info
(
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_pdcp_inactive_ue_info, 0, sizeof(rrc_pdcp_inactive_ue_info_t));

    /* This function parses rrc_pdcp_inactive_ue_info */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_pdcp_inactive_ue_info->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

