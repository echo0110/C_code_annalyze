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
 *  File Description : The file rrc_s1apCommon_il_parser.c contains the definitions 
 *                     of rrc-s1apCommon interface message parsing functions.
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
#include "rrc_s1apCommon_il_composer.h"
#include "rrc_s1apCommon_intf.h"
#include "rrc_ext_api.h"
#ifndef _MSC_VER
#include "rrc_common_utils.h"
#endif
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_parse_ip_addr
(
    ip_addr_t *p_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_ipv6_addr
(
    ipv6_addr_t *p_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_mme_comm_info
(
    mme_comm_info_t *p_mme_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_parse_s1ap_add_mme_req
(
    s1ap_add_mme_req_t *p_s1ap_add_mme_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_s1ap_add_mme_req, 0, sizeof(s1ap_add_mme_req_t));

    /* This function parses s1ap_add_mme_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing s1ap_add_mme_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_add_mme_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_add_mme_req->num_mme, p_src + *p_length_read, "num_mme");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_add_mme_req->num_mme < 1) || (p_s1ap_add_mme_req->num_mme > MAX_ADD_DYNAMIC_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_add_mme_req->num_mme] should be in range [1 <= %d <= MAX_ADD_DYNAMIC_MME] !", p_s1ap_add_mme_req->num_mme);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_add_mme_req->num_mme; loop++)
        {
            if (RRC_FAILURE == rrc_parse_mme_comm_info(
                &p_s1ap_add_mme_req->mme_comm_info_arr[loop],
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
rrc_parse_ip_addr
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
    RRC_TRACE(RRC_COMP_PARSE, "Parsing ip_addr");

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
rrc_parse_ipv6_addr
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
    RRC_TRACE(RRC_COMP_PARSE, "Parsing ipv6_addr");

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
rrc_parse_mme_comm_info
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
    RRC_TRACE(RRC_COMP_PARSE, "Parsing mme_comm_info");

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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be in range [%d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_parse_ip_addr(
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_streams] should be in range [2 <= %d <= 10] !", p_mme_comm_info->num_streams);
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->rel_of_mme] should be in range [%d <= 2] !", p_mme_comm_info->rel_of_mme);
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ipv6_addr);
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
            if (RRC_FAILURE == rrc_parse_ipv6_addr(
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

rrc_return_et
rrc_parse_s1ap_add_mme_res
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_s1ap_add_mme_res, 0, sizeof(s1ap_add_mme_res_t));

    /* This function parses s1ap_add_mme_res */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing s1ap_add_mme_res");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_s1ap_add_mme_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_s1ap_add_mme_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_add_mme_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_add_mme_res->response] should be in range [%d <= 1] !", p_s1ap_add_mme_res->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

