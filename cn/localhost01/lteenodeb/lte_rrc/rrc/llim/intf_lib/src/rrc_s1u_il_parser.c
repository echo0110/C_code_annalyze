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
 *  File Description : The file rrc_s1u_il_parser.c contains the definitions 
 *                     of rrc-s1u interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_s1u_il_parser.h"
#include "rrc_s1u_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_drb_id
(
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_buffer_ind
(
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_teid_self
(
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_qos_profile
(
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_seq_num
(
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_seq_disable
(
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_reordering_reqd
(
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_pdcp_ext
(
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_sap_param
(
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_dc_bearer_type
(
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_teid_peer
(
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_gsn_addr
(
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_tunnel_info
(
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_relay_setup_sap_req
(
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_release_tunnel_info
(
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_relay_rel_sap_req
(
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_tunnel_info_cnf
(
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_relay_setup_sap_cnf
(
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_rrc_s1u_ie_relay_rel_sap_cnf
(
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_s1u_communication_info
(
    s1u_communication_info_t *p_s1u_communication_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_rrc_s1u_delete_ue_entity_req
(
    rrc_s1u_delete_ue_entity_req_t *p_rrc_s1u_delete_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_delete_ue_entity_req, 0, sizeof(rrc_s1u_delete_ue_entity_req_t));

    /* This function parses rrc_s1u_delete_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_delete_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_delete_ue_entity_cnf
(
    rrc_s1u_delete_ue_entity_cnf_t *p_rrc_s1u_delete_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_delete_ue_entity_cnf, 0, sizeof(rrc_s1u_delete_ue_entity_cnf_t));

    /* This function parses rrc_s1u_delete_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_delete_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_delete_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_reconfigure_ue_entity_req
(
    rrc_s1u_reconfigure_ue_entity_req_t *p_rrc_s1u_reconfigure_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_reconfigure_ue_entity_req, 0, sizeof(rrc_s1u_reconfigure_ue_entity_req_t));

    /* This function parses rrc_s1u_reconfigure_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_reconfigure_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_S1U_IE_RELAY_SETUP_SAP_REQ_TAG:
                /* rrc_s1u_ie_relay_setup_sap_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req >= (S32)ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_req->setup_sap_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_relay_setup_sap_req(
                    &p_rrc_s1u_reconfigure_ue_entity_req->setup_sap_req[p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req++;

                *p_length_read += length_read;

                break;
            case RRC_S1U_IE_RELAY_REL_SAP_REQ_TAG:
                /* rrc_s1u_ie_relay_rel_sap_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req >= (S32)ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_req->rel_sap_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_relay_rel_sap_req(
                    &p_rrc_s1u_reconfigure_ue_entity_req->rel_sap_req[p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req++;

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
rrc_il_parse_rrc_s1u_ie_drb_id
(
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_drb_id, 0, sizeof(rrc_s1u_ie_drb_id_t));

    /* This function parses rrc_s1u_ie_drb_id */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_drb_id->drb_id, p_src + *p_length_read, "drb_id");
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
rrc_il_parse_rrc_s1u_ie_buffer_ind
(
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_buffer_ind, 0, sizeof(rrc_s1u_ie_buffer_ind_t));

    /* This function parses rrc_s1u_ie_buffer_ind */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_buffer_ind->buffer_ind, p_src + *p_length_read, "buffer_ind");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_buffer_ind->buffer_ind > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_buffer_ind->buffer_ind] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_s1u_ie_buffer_ind->buffer_ind);
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
rrc_il_parse_rrc_s1u_ie_teid_self
(
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_teid_self, 0, sizeof(rrc_s1u_ie_teid_self_t));

    /* This function parses rrc_s1u_ie_teid_self */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_s1u_ie_teid_self->teid, p_src + *p_length_read, "teid");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_s1u_ie_teid_self->teid < 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_teid_self->teid] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_ie_teid_self->teid);
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
rrc_il_parse_rrc_s1u_ie_qos_profile
(
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_qos_profile, 0, sizeof(rrc_s1u_ie_qos_profile_t));

    /* This function parses rrc_s1u_ie_qos_profile */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_qos_profile->allocation_retention_priority, p_src + *p_length_read, "allocation_retention_priority");
    *p_length_read += sizeof(U8);

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_s1u_ie_qos_profile->qos_profile_data) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_s1u_ie_qos_profile->qos_profile_data_length %u is exceeding p_rrc_s1u_ie_qos_profile->qos_profile_data array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_s1u_ie_qos_profile->qos_profile_data[loop], p_src + *p_length_read + loop, "qos_profile_data[]");
        }

        p_rrc_s1u_ie_qos_profile->qos_profile_data_length = length_left - *p_length_read;
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
rrc_il_parse_rrc_s1u_ie_seq_num
(
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_seq_num, 0, sizeof(rrc_s1u_ie_seq_num_t));

    /* This function parses rrc_s1u_ie_seq_num */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_ie_seq_num->ul_sequence_number, p_src + *p_length_read, "ul_sequence_number");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_ie_seq_num->dl_sequence_number, p_src + *p_length_read, "dl_sequence_number");
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
rrc_il_parse_rrc_s1u_ie_seq_disable
(
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_seq_disable, 0, sizeof(rrc_s1u_ie_seq_disable_t));

    /* This function parses rrc_s1u_ie_seq_disable */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_seq_disable->disable, p_src + *p_length_read, "disable");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_seq_disable->disable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_seq_disable->disable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_ie_seq_disable->disable);
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
rrc_il_parse_rrc_s1u_reordering_reqd
(
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_reordering_reqd, 0, sizeof(rrc_s1u_reordering_reqd_t));

    /* This function parses rrc_s1u_reordering_reqd */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_reordering_reqd->reordering_reqd, p_src + *p_length_read, "reordering_reqd");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_reordering_reqd->reordering_reqd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_reordering_reqd->reordering_reqd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_reordering_reqd->reordering_reqd);
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
rrc_il_parse_rrc_s1u_ie_pdcp_ext
(
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_pdcp_ext, 0, sizeof(rrc_s1u_ie_pdcp_ext_t));

    /* This function parses rrc_s1u_ie_pdcp_ext */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag, p_src + *p_length_read, "pdcp_ext_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag);
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
rrc_il_parse_rrc_s1u_sap_param
(
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_sap_param, 0, sizeof(rrc_s1u_sap_param_t));

    /* This function parses rrc_s1u_sap_param */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_sap_param->sn_size, p_src + *p_length_read, "sn_size");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_sap_param->sn_size < 12) || (p_rrc_s1u_sap_param->sn_size > 18))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_sap_param->sn_size] should be in range "
            "12 to 18. Incorrect value %u received.", p_rrc_s1u_sap_param->sn_size);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_sap_param->rlc_mode, p_src + *p_length_read, "rlc_mode");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_sap_param->rlc_mode > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_sap_param->rlc_mode] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_s1u_sap_param->rlc_mode);
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
rrc_il_parse_rrc_dc_bearer_type
(
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_dc_bearer_type, 0, sizeof(rrc_dc_bearer_type_t));

    /* This function parses rrc_dc_bearer_type */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_dc_bearer_type->dc_bearer_type, p_src + *p_length_read, "dc_bearer_type");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dc_bearer_type->dc_bearer_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dc_bearer_type->dc_bearer_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_dc_bearer_type->dc_bearer_type);
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
            case RRC_S1U_IE_SAP_PARAM_TAG:
                /* rrc_s1u_sap_param TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_sap_param(
                    &p_rrc_dc_bearer_type->sap_param,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_dc_bearer_type->bitmask |= RRC_S1U_IE_SAP_PARAM_PRESENT;

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
rrc_il_parse_rrc_s1u_ie_teid_peer
(
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_teid_peer, 0, sizeof(rrc_s1u_ie_teid_peer_t));

    /* This function parses rrc_s1u_ie_teid_peer */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_s1u_ie_teid_peer->teid, p_src + *p_length_read, "teid");
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
rrc_il_parse_rrc_s1u_ie_gsn_addr
(
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_gsn_addr, 0, sizeof(rrc_s1u_ie_gsn_addr_t));

    /* This function parses rrc_s1u_ie_gsn_addr */

    /* Parse OCTET_STRING, LIMITED_TILL_THE_END */
    {
        if((S32)ARRSIZE(p_rrc_s1u_ie_gsn_addr->data) < (length_left - *p_length_read))
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_s1u_ie_gsn_addr->data_length %u is exceeding p_rrc_s1u_ie_gsn_addr->data array boundary", (length_left - *p_length_read));
            return RRC_FAILURE;
        }
        U16 loop;
        for (loop = 0; loop < length_left - *p_length_read; loop++)
        {
            rrc_cp_unpack_U8(&p_rrc_s1u_ie_gsn_addr->data[loop], p_src + *p_length_read + loop, "data[]");
        }

        p_rrc_s1u_ie_gsn_addr->data_length = length_left - *p_length_read;
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
rrc_il_parse_rrc_s1u_ie_tunnel_info
(
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_tunnel_info, 0, sizeof(rrc_s1u_ie_tunnel_info_t));

    /* This function parses rrc_s1u_ie_tunnel_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_tunnel_info->sap_flag, p_src + *p_length_read, "sap_flag");
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
            case RRC_S1U_IE_TEID_PEER_TAG:
                /* rrc_s1u_ie_teid_peer TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_teid_peer(
                    &p_rrc_s1u_ie_tunnel_info->teid_peer,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_tunnel_info->bitmask |= RRC_S1U_IE_TUNNEL_INFO_TEID_PEER_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_GSN_ADDR_TAG:
                /* rrc_s1u_ie_gsn_addr TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_gsn_addr(
                    &p_rrc_s1u_ie_tunnel_info->transport_addr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_tunnel_info->bitmask |= RRC_S1U_IE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT;

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
rrc_il_parse_rrc_s1u_ie_relay_setup_sap_req
(
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_relay_setup_sap_req, 0, sizeof(rrc_s1u_ie_relay_setup_sap_req_t));

    /* This function parses rrc_s1u_ie_relay_setup_sap_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_relay_setup_sap_req->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_ie_relay_setup_sap_req->lc_id < 3) || (p_rrc_s1u_ie_relay_setup_sap_req->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_setup_sap_req->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_ie_relay_setup_sap_req->lc_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_s1u_ie_relay_setup_sap_req->qos_id, p_src + *p_length_read, "qos_id");
    *p_length_read += sizeof(U32);

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
            case RRC_S1U_IE_DRBID_SELF_TAG:
                /* rrc_s1u_ie_drb_id TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_drb_id(
                    &p_rrc_s1u_ie_relay_setup_sap_req->drb_id,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_DRBID_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_BUFFER_IND_TAG:
                /* rrc_s1u_ie_buffer_ind TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_buffer_ind(
                    &p_rrc_s1u_ie_relay_setup_sap_req->buffer_ind,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_BUFFER_IND_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_TEID_SELF_TAG:
                /* rrc_s1u_ie_teid_self TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_teid_self(
                    &p_rrc_s1u_ie_relay_setup_sap_req->teid_self,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_TEID_SELF_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_QOS_PROFILE_TAG:
                /* rrc_s1u_ie_qos_profile TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_qos_profile(
                    &p_rrc_s1u_ie_relay_setup_sap_req->qos_profile,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_QOS_PROFILE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_SEQ_NUM_TAG:
                /* rrc_s1u_ie_seq_num TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_seq_num(
                    &p_rrc_s1u_ie_relay_setup_sap_req->seq_num,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_SEQ_NUM_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_SEQ_DISABLE_TAG:
                /* rrc_s1u_ie_seq_disable TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_seq_disable(
                    &p_rrc_s1u_ie_relay_setup_sap_req->seq_flag,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_SEQ_FLAG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_REORDERING_REQD_TAG:
                /* rrc_s1u_reordering_reqd TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_reordering_reqd(
                    &p_rrc_s1u_ie_relay_setup_sap_req->reordering_reqd,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_REORDERING_REQD_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_PDCP_EXT_TAG:
                /* rrc_s1u_ie_pdcp_ext TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_pdcp_ext(
                    &p_rrc_s1u_ie_relay_setup_sap_req->pdcp_ext,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_PDCP_EXT_FLAG_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_DC_BEARER_FLAG:
                /* rrc_dc_bearer_type TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_dc_bearer_type(
                    &p_rrc_s1u_ie_relay_setup_sap_req->dc_bearer_type,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->bitmask |= RRC_S1U_IE_SETUP_DC_BEARER_TYPE_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_TUNNEL_INFO_TAG:
                /* rrc_s1u_ie_tunnel_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info >= (S32)ARRSIZE(p_rrc_s1u_ie_relay_setup_sap_req->tunnel_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_tunnel_info(
                    &p_rrc_s1u_ie_relay_setup_sap_req->tunnel_info[p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info++;

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
rrc_il_parse_rrc_release_tunnel_info
(
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_release_tunnel_info, 0, sizeof(rrc_release_tunnel_info_t));

    /* This function parses rrc_release_tunnel_info */

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_release_tunnel_info->teid_self, p_src + *p_length_read, "teid_self");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_release_tunnel_info->teid_self < 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_release_tunnel_info->teid_self] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_release_tunnel_info->teid_self);
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
            case RRC_S1U_IE_GSN_ADDR_TAG:
                /* rrc_s1u_ie_gsn_addr TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_gsn_addr(
                    &p_rrc_release_tunnel_info->transport_addr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_release_tunnel_info->bitmask |= RRC_S1U_IE_RELEASE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT;

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
rrc_il_parse_rrc_s1u_ie_relay_rel_sap_req
(
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_relay_rel_sap_req, 0, sizeof(rrc_s1u_ie_relay_rel_sap_req_t));

    /* This function parses rrc_s1u_ie_relay_rel_sap_req */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_relay_rel_sap_req->lc_id, p_src + *p_length_read, "lc_id");
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
            case RRC_S1U_IE_RELEASE_TUNNEL_INFO_TAG:
                /* rrc_release_tunnel_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info >= (S32)ARRSIZE(p_rrc_s1u_ie_relay_rel_sap_req->release_tunnel_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_release_tunnel_info(
                    &p_rrc_s1u_ie_relay_rel_sap_req->release_tunnel_info[p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info++;

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
rrc_il_parse_rrc_s1u_reconfigure_ue_entity_cnf
(
    rrc_s1u_reconfigure_ue_entity_cnf_t *p_rrc_s1u_reconfigure_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_reconfigure_ue_entity_cnf, 0, sizeof(rrc_s1u_reconfigure_ue_entity_cnf_t));

    /* This function parses rrc_s1u_reconfigure_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_reconfigure_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_S1U_IE_RELAY_SETUP_SAP_CNF_TAG:
                /* rrc_s1u_ie_relay_setup_sap_cnf TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf >= (S32)ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_cnf->setup_sap_cnf))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_relay_setup_sap_cnf(
                    &p_rrc_s1u_reconfigure_ue_entity_cnf->setup_sap_cnf[p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf++;

                *p_length_read += length_read;

                break;
            case RRC_S1U_IE_RELAY_REL_SAP_CNF_TAG:
                /* rrc_s1u_ie_relay_rel_sap_cnf TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf >= (S32)ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_cnf->rel_sap_cnf))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_relay_rel_sap_cnf(
                    &p_rrc_s1u_reconfigure_ue_entity_cnf->rel_sap_cnf[p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf++;

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
rrc_il_parse_rrc_s1u_ie_tunnel_info_cnf
(
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_tunnel_info_cnf, 0, sizeof(rrc_s1u_ie_tunnel_info_cnf_t));

    /* This function parses rrc_s1u_ie_tunnel_info_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_tunnel_info_cnf->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_tunnel_info_cnf->cause > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_tunnel_info_cnf->cause] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_s1u_ie_tunnel_info_cnf->cause);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_ie_tunnel_info_cnf->rb_direction, p_src + *p_length_read, "rb_direction");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_tunnel_info_cnf->rb_direction > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_tunnel_info_cnf->rb_direction] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_s1u_ie_tunnel_info_cnf->rb_direction);
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
            case RRC_S1U_IE_TEID_SELF_TAG:
                /* rrc_s1u_ie_teid_self TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_teid_self(
                    &p_rrc_s1u_ie_tunnel_info_cnf->teid_self,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_tunnel_info_cnf->bitmask |= RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_SELF_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_TEID_PEER_TAG:
                /* rrc_s1u_ie_teid_peer TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_teid_peer(
                    &p_rrc_s1u_ie_tunnel_info_cnf->teid_peer,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_tunnel_info_cnf->bitmask |= RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_PEER_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_GSN_ADDR_TAG:
                /* rrc_s1u_ie_gsn_addr TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_gsn_addr(
                    &p_rrc_s1u_ie_tunnel_info_cnf->transport_addr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_tunnel_info_cnf->bitmask |= RRC_S1U_IE_TUNNEL_INFO_CNF_TRANSPORT_ADDR_PRESENT;

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
rrc_il_parse_rrc_s1u_ie_relay_setup_sap_cnf
(
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_relay_setup_sap_cnf, 0, sizeof(rrc_s1u_ie_relay_setup_sap_cnf_t));

    /* This function parses rrc_s1u_ie_relay_setup_sap_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id < 3) || (p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_relay_setup_sap_cnf->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_relay_setup_sap_cnf->cause > EGTPU_PEER_ADDRESS_MISSING))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_setup_sap_cnf->cause] should be less than"
            " or equal to EGTPU_PEER_ADDRESS_MISSING. Incorrect value %u received.", p_rrc_s1u_ie_relay_setup_sap_cnf->cause);
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
            case RRC_S1U_IE_TUNNEL_INFO_CNF_TAG:
                /* rrc_s1u_ie_tunnel_info_cnf TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf >= (S32)ARRSIZE(p_rrc_s1u_ie_relay_setup_sap_cnf->tunnel_info_cnf))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_tunnel_info_cnf(
                    &p_rrc_s1u_ie_relay_setup_sap_cnf->tunnel_info_cnf[p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf++;

                *p_length_read += length_read;

                break;
            case RRC_S1U_IE_SEQ_NUM_TAG:
                /* rrc_s1u_ie_seq_num TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_seq_num(
                    &p_rrc_s1u_ie_relay_setup_sap_cnf->seq_num,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_setup_sap_cnf->bitmask |= RRC_S1U_IE_SETUP_SAP_CNF_SEQ_NUM_PRESENT;

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
rrc_il_parse_rrc_s1u_ie_relay_rel_sap_cnf
(
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_ie_relay_rel_sap_cnf, 0, sizeof(rrc_s1u_ie_relay_rel_sap_cnf_t));

    /* This function parses rrc_s1u_ie_relay_rel_sap_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id < 3) || (p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_ie_relay_rel_sap_cnf->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_relay_rel_sap_cnf->cause > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_rel_sap_cnf->cause] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_s1u_ie_relay_rel_sap_cnf->cause);
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
            case RRC_S1U_IE_SEQ_NUM_TAG:
                /* rrc_s1u_ie_seq_num TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_seq_num(
                    &p_rrc_s1u_ie_relay_rel_sap_cnf->seq_num,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_rel_sap_cnf->bitmask |= RRC_S1U_IE_REL_CNF_SEQ_NUM_PRESENT;

                *p_length_read += length;

                break;
            case RRC_S1U_IE_RELEASE_TUNNEL_INFO_TAG:
                /* rrc_release_tunnel_info TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info >= (S32)ARRSIZE(p_rrc_s1u_ie_relay_rel_sap_cnf->release_tunnel_info))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_release_tunnel_info(
                    &p_rrc_s1u_ie_relay_rel_sap_cnf->release_tunnel_info[p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info++;

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
rrc_il_parse_rrc_s1u_create_ue_entity_req
(
    rrc_s1u_create_ue_entity_req_t *p_rrc_s1u_create_ue_entity_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_create_ue_entity_req, 0, sizeof(rrc_s1u_create_ue_entity_req_t));

    /* This function parses rrc_s1u_create_ue_entity_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_create_ue_entity_req->ue_index, p_src + *p_length_read, "ue_index");
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
            case RRC_S1U_IE_RELAY_SETUP_SAP_REQ_TAG:
                /* rrc_s1u_ie_relay_setup_sap_req TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_create_ue_entity_req->num_setup_sap_req >= (S32)ARRSIZE(p_rrc_s1u_create_ue_entity_req->setup_sap_req))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_relay_setup_sap_req(
                    &p_rrc_s1u_create_ue_entity_req->setup_sap_req[p_rrc_s1u_create_ue_entity_req->num_setup_sap_req],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_create_ue_entity_req->num_setup_sap_req++;

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
rrc_il_parse_rrc_s1u_create_ue_entity_cnf
(
    rrc_s1u_create_ue_entity_cnf_t *p_rrc_s1u_create_ue_entity_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_create_ue_entity_cnf, 0, sizeof(rrc_s1u_create_ue_entity_cnf_t));

    /* This function parses rrc_s1u_create_ue_entity_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_create_ue_entity_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_create_ue_entity_cnf->response_code, p_src + *p_length_read, "response_code");
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
            case RRC_S1U_IE_RELAY_SETUP_SAP_CNF_TAG:
                /* rrc_s1u_ie_relay_setup_sap_cnf TLV SEQUENCE */

                /* Check whether counter is correct */
                if (p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf >= (S32)ARRSIZE(p_rrc_s1u_create_ue_entity_cnf->setup_sap_cnf))
                {
                    RRC_TRACE(RRC_WARNING, "Received more TLV SEQUENCE than expected!");
                    return RRC_FAILURE;
                }

                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_relay_setup_sap_cnf(
                    &p_rrc_s1u_create_ue_entity_cnf->setup_sap_cnf[p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf],
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf++;

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
rrc_il_parse_rrc_s1u_error_ind
(
    rrc_s1u_error_ind_t *p_rrc_s1u_error_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_error_ind, 0, sizeof(rrc_s1u_error_ind_t));

    /* This function parses rrc_s1u_error_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_error_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_error_ind->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_error_ind->lc_id < 3) || (p_rrc_s1u_error_ind->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_error_ind->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_error_ind->lc_id);
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
rrc_il_parse_rrc_s1u_path_failure_ind
(
    rrc_s1u_path_failure_ind_t *p_rrc_s1u_path_failure_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_path_failure_ind, 0, sizeof(rrc_s1u_path_failure_ind_t));

    /* This function parses rrc_s1u_path_failure_ind */

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
            case RRC_S1U_IE_GSN_ADDR_TAG:
                /* rrc_s1u_ie_gsn_addr TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_gsn_addr(
                    &p_rrc_s1u_path_failure_ind->transport_addr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_path_failure_ind->bitmask |= RRC_TRANSPORT_ADDR_PRESENT;

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
rrc_il_parse_rrc_s1u_end_marker_ind
(
    rrc_s1u_end_marker_ind_t *p_rrc_s1u_end_marker_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_end_marker_ind, 0, sizeof(rrc_s1u_end_marker_ind_t));

    /* This function parses rrc_s1u_end_marker_ind */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_end_marker_ind->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_end_marker_ind->lc_id, p_src + *p_length_read, "lc_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_end_marker_ind->lc_id < 3) || (p_rrc_s1u_end_marker_ind->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_end_marker_ind->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_end_marker_ind->lc_id);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_rrc_s1u_end_marker_ind->teid_self, p_src + *p_length_read, "teid_self");
    *p_length_read += sizeof(U32);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_s1u_end_marker_ind->teid_self < 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_end_marker_ind->teid_self] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_end_marker_ind->teid_self);
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
            case RRC_S1U_IE_GSN_ADDR_TAG:
                /* rrc_s1u_ie_gsn_addr TLV */
                if (RRC_FAILURE == rrc_il_parse_rrc_s1u_ie_gsn_addr(
                    &p_rrc_s1u_end_marker_ind->transport_addr,
                    p_src + *p_length_read,
                    length,
                    &length_read))
                {
                    return RRC_FAILURE;
                }

                p_rrc_s1u_end_marker_ind->bitmask |= RRC_S1U_END_MARKER_IND_TRANSPORT_ADDR_PRESENT;

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
rrc_il_parse_rrc_s1u_path_success_ind
(
    rrc_s1u_path_success_ind_t *p_rrc_s1u_path_success_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_path_success_ind, 0, sizeof(rrc_s1u_path_success_ind_t));

    /* This function parses rrc_s1u_path_success_ind */

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_intra_enb_data_fwd_req
(
    rrc_s1u_intra_enb_data_fwd_req_t *p_rrc_s1u_intra_enb_data_fwd_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_intra_enb_data_fwd_req, 0, sizeof(rrc_s1u_intra_enb_data_fwd_req_t));

    /* This function parses rrc_s1u_intra_enb_data_fwd_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_intra_enb_data_fwd_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_intra_enb_data_fwd_req->target_ue_index, p_src + *p_length_read, "target_ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_intra_enb_data_fwd_cnf
(
    rrc_s1u_intra_enb_data_fwd_cnf_t *p_rrc_s1u_intra_enb_data_fwd_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_intra_enb_data_fwd_cnf, 0, sizeof(rrc_s1u_intra_enb_data_fwd_cnf_t));

    /* This function parses rrc_s1u_intra_enb_data_fwd_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_intra_enb_data_fwd_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_intra_enb_data_fwd_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_local_path_switch_req
(
    rrc_s1u_local_path_switch_req_t *p_rrc_s1u_local_path_switch_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_local_path_switch_req, 0, sizeof(rrc_s1u_local_path_switch_req_t));

    /* This function parses rrc_s1u_local_path_switch_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_local_path_switch_req->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_local_path_switch_req->ue_index_for_path_switch, p_src + *p_length_read, "ue_index_for_path_switch");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_local_path_switch_cnf
(
    rrc_s1u_local_path_switch_cnf_t *p_rrc_s1u_local_path_switch_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_local_path_switch_cnf, 0, sizeof(rrc_s1u_local_path_switch_cnf_t));

    /* This function parses rrc_s1u_local_path_switch_cnf */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_local_path_switch_cnf->ue_index, p_src + *p_length_read, "ue_index");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_local_path_switch_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_il_parse_rrc_s1u_config_cell_req
(
    rrc_s1u_config_cell_req_t *p_rrc_s1u_config_cell_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_config_cell_req, 0, sizeof(rrc_s1u_config_cell_req_t));

    /* This function parses rrc_s1u_config_cell_req */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_rrc_s1u_config_cell_req->Cell_index, p_src + *p_length_read, "Cell_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_config_cell_req->Cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_config_cell_req->Cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_s1u_config_cell_req->Cell_index);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_il_parse_s1u_communication_info(
        &p_rrc_s1u_config_cell_req->s1u_communication_info,
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
rrc_il_parse_s1u_communication_info
(
    s1u_communication_info_t *p_s1u_communication_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_s1u_communication_info, 0, sizeof(s1u_communication_info_t));

    /* This function parses s1u_communication_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1u_communication_info->pdcpInstance, p_src + *p_length_read, "pdcpInstance");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1u_communication_info->pdcpRxPort, p_src + *p_length_read, "pdcpRxPort");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(p_s1u_communication_info->pdcpIpAddr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1u_communication_info->pdcpIpAddr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_s1u_communication_info->pdcpIpAddr[loop], (void*)(p_src + *p_length_read), "pdcpIpAddr[]");
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
rrc_il_parse_rrc_s1u_config_cell_cnf
(
    rrc_s1u_config_cell_cnf_t *p_rrc_s1u_config_cell_cnf,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_rrc_s1u_config_cell_cnf, 0, sizeof(rrc_s1u_config_cell_cnf_t));

    /* This function parses rrc_s1u_config_cell_cnf */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_config_cell_cnf->cell_index, p_src + *p_length_read, "cell_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_s1u_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_rrc_s1u_config_cell_cnf->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

