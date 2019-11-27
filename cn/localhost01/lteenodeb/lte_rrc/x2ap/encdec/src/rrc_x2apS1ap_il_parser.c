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
 *  File Description : The file rrc_x2apS1ap_il_parser.c contains the definitions 
 *                     of rrc-x2apS1ap interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_x2apS1ap_il_parser.h"
#include "rrc_x2apS1ap_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_il_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_il_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_il_parse_x2ap_s1ap_comm_info
(
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2ap_s1ap_comm_info, 0, sizeof(x2ap_s1ap_comm_info_t));

    /* This function parses x2ap_s1ap_comm_info */

    if (RRC_FAILURE == rrc_il_parse_x2_enb_comm_info(
        &p_x2ap_s1ap_comm_info->local_enb_info,
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
rrc_il_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2_ip_addr, 0, sizeof(x2_ip_addr_t));

    /* This function parses x2_ip_addr */

    if (*p_length_read + (S32)sizeof(p_x2_ip_addr->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ip_addr->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_ip_addr->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
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
rrc_il_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2_ipv6_addr, 0, sizeof(x2_ipv6_addr_t));

    /* This function parses x2_ipv6_addr */

    if (*p_length_read + (S32)sizeof(p_x2_ipv6_addr->ipv6_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ipv6_addr->ipv6_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_ipv6_addr->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
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
rrc_il_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    *p_length_read = 0;

    memset_wrapper(p_x2_trans_layer_addrs, 0, sizeof(x2_trans_layer_addrs_t));

    /* This function parses x2_trans_layer_addrs */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_trans_layer_addrs->data_length, p_src + *p_length_read, "data_length");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be less than"
            " or equal to RRC_MAX_IP_ADDR_LEN_IPV6. Incorrect value %u received.", p_x2_trans_layer_addrs->data_length);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2_trans_layer_addrs->data_length * sizeof(p_x2_trans_layer_addrs->data[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_trans_layer_addrs->data_length; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_trans_layer_addrs->data[loop], (void*)(p_src + *p_length_read), "data[]");
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
rrc_il_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2_extend_trans_layer_addrs, 0, sizeof(x2_extend_trans_layer_addrs_t));

    /* This function parses x2_extend_trans_layer_addrs */

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2_extend_trans_layer_addrs->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    if (RRC_FAILURE == rrc_il_parse_x2_trans_layer_addrs(
        &p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_extend_trans_layer_addrs->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %u received.", p_x2_extend_trans_layer_addrs->num_tlas);
        return RRC_FAILURE;
    }
    }

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_extend_trans_layer_addrs->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2_trans_layer_addrs(
                &p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop],
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
rrc_il_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;

    *p_length_read = 0;

    memset_wrapper(p_x2_enb_comm_info, 0, sizeof(x2_enb_comm_info_t));

    /* This function parses x2_enb_comm_info */

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->num_ip_addr, p_src + *p_length_read, "num_ip_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2_ip_addr(
                &p_x2_enb_comm_info->ip_addr[loop],
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
    rrc_cp_unpack_U16(&p_x2_enb_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->num_ipv6_addr, p_src + *p_length_read, "num_ipv6_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2_ipv6_addr(
                &p_x2_enb_comm_info->ipv6_addr[loop],
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

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_tlas < 1) || (p_x2_enb_comm_info->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %u received.", p_x2_enb_comm_info->num_tlas);
        return RRC_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_il_parse_x2_extend_trans_layer_addrs(
                &p_x2_enb_comm_info->extend_trans_layer_addrs[loop],
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

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->default_sgnb, p_src + *p_length_read, "default_sgnb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_enb_comm_info->default_sgnb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2_enb_comm_info->default_sgnb);
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

